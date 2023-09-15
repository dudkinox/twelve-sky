#define _FILE_OFFSET_BITS 64
#define _LARGEFILE_SOURCE 1
#define _LARGEFILE64_SOURCE 1
#define _LARGE_FILES 1

#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <dirent.h>
#include <time.h>
#include <arpa/inet.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/time.h>

#include "sysutil.h"

static unsigned int s_current_umask;
static long s_timezone;

static struct sysutil_sig_details
{
	sighandle_t sync_sig_handler;
	void* p_private;
	int pending;
	int running;
} s_sig_details[NSIG];

static context_io_t s_io_handler;
static void* s_p_io_handler_private;
static int s_io_handler_running;

/* File locals */
static void sysutil_common_sighandler(int signum);
static int sysutil_translate_sig(const enum ESysUtilSignal sig);
static int sysutil_set_sighandler(int sig, void (*p_handlefunc)(int));
static int sysutil_translate_memprot(const enum ESysUtilMapPermission perm);
static void sysutil_alloc_statbuf(struct sysutil_statbuf** stat_d_ptr);
static int lock_internal(int fd, int lock_type);

static void sysutil_common_sighandler(int signum)
{
	if (s_sig_details[signum].sync_sig_handler) {
		s_sig_details[signum].pending = 1;
	}
}

void sysutil_check_pending_actions(const enum ESysUtilInterruptContext context, int retval, int fd)
{
	unsigned int i;

	if (s_io_handler && !s_io_handler_running && context == kSysUtilIO) {
		s_io_handler_running = 1;
		(*s_io_handler)(retval, fd, s_p_io_handler_private);
		s_io_handler_running = 0;
	}

	for (i=0; i < NSIG; ++i) {
		if (s_sig_details[i].pending && !s_sig_details[i].running) {
			s_sig_details[i].running = 1;
			if (s_sig_details[i].sync_sig_handler) {
				s_sig_details[i].pending = 0;
				(*(s_sig_details[i].sync_sig_handler))(s_sig_details[i].p_private);
			}
			s_sig_details[i].running = 0;
		}
	}
}

static int sysutil_translate_sig(const enum ESysUtilSignal sig)
{
	int realsig = 0;
	switch (sig) {
		case kSysUtilSigALRM:
			realsig = SIGALRM;
			break;
		case kSysUtilSigTERM:
			realsig = SIGTERM;
			break;
		case kSysUtilSigCHLD:
			realsig = SIGCHLD;
			break;
		case kSysUtilSigPIPE:
			realsig = SIGPIPE;
			break;
		case kSysUtilSigURG:
			realsig = SIGURG;
			break;
		case kSysUtilSigHUP:
			realsig = SIGHUP;
			break;
		default:
			realsig = -1;
			break;
	}

	if (realsig < 0 || realsig >= NSIG) {
		return -2;
	}

	return realsig;
}

void sysutil_install_sighandler(const enum ESysUtilSignal sig, sighandle_t handler, void* p_private)
{
	int realsig = sysutil_translate_sig(sig);
	s_sig_details[realsig].p_private = p_private;
	s_sig_details[realsig].sync_sig_handler = handler;
	sysutil_set_sighandler(realsig, sysutil_common_sighandler);
}

void sysutil_default_sig(const enum ESysUtilSignal sig) 
{
	int realsig = sysutil_translate_sig(sig);
	sysutil_set_sighandler(realsig, SIG_DFL);
	s_sig_details[realsig].p_private = NULL;
	s_sig_details[realsig].sync_sig_handler = NULL;
}

void sysutil_install_null_sighandler(const enum ESysUtilSignal sig)
{
	int realsig = sysutil_translate_sig(sig);
	s_sig_details[realsig].p_private = NULL;
	s_sig_details[realsig].sync_sig_handler = NULL;
	sysutil_set_sighandler(realsig, sysutil_common_sighandler);
}

void sysutil_install_async_sighandler(const enum ESysUtilSignal sig, async_sighandle_t handler)
{
	int realsig = sysutil_translate_sig(sig);
	s_sig_details[realsig].p_private = NULL;
	s_sig_details[realsig].sync_sig_handler = NULL;
	sysutil_block_sig(sig);
	sysutil_set_sighandler(realsig, handler);
}

static int sysutil_set_sighandler(int sig, void (*p_handlefunc)(int))
{
	int retval;
	struct sigaction sigact;

	sysutil_memclr(&sigact, sizeof(sigact));
	sigact.sa_handler = p_handlefunc;

	retval = sigfillset(&sigact.sa_mask);
	if (retval != 0) {
		return retval;
	}

	retval = sigaction(sig, &sigact, NULL);
	if (retval != 0) {
		return retval;
	}

	return retval;
}

int sysutil_block_sig(const enum ESysUtilSignal sig)
{
	sigset_t sset;
	int retval;
	int realsig = sysutil_translate_sig(sig);
	retval = sigemptyset(&sset);
	if (retval != 0) {
		return retval;
	}

	retval = sigaddset(&sset, realsig);
	if (retval != 0) {
		return retval;
	}

	retval = sigprocmask(SIG_BLOCK, &sset, NULL);
	if (retval != 0) {
		return retval;
	}
	return retval;
}

int sysutil_unblock_sig(const enum ESysUtilSignal sig)
{
	sigset_t sset;
	int retval;
	int realsig = sysutil_translate_sig(sig);
	retval = sigemptyset(&sset);

	if (retval != 0) {
		return retval;
	}
	retval = sigaddset(&sset, realsig);
	if (retval != 0) {
		return retval;
	}
	retval = sigprocmask(SIG_UNBLOCK, &sset, NULL);
	if (retval != 0) {
		return retval;
	}
	return retval;
}

int sysutil_install_io_handler(context_io_t handler, void* p_private)
{
	if(s_io_handler != NULL) {
		return -1;
	}
	s_io_handler = handler;
	s_p_io_handler_private = p_private;
	return 1;
}

int sysutil_uninstall_io_handler(void)
{
	if (s_io_handler == NULL) {
		return -1;
	}
	s_io_handler = NULL;
	s_p_io_handler_private = NULL;
	return 1;
}

void sysutil_set_alarm(const unsigned int trigger_seconds)
{
	(void) alarm(trigger_seconds);
}

void sysutil_clear_alarm(void)
{
	sysutil_set_alarm(0);
}

int sysutil_read(const int fd, void* p_buf, const unsigned int size)
{
	while (1) {
		int retval = read(fd, (char *)p_buf, size);
		int saved_errno = errno;
		sysutil_check_pending_actions(kSysUtilIO, retval, fd);
		if (retval < 0 && saved_errno == EINTR) {
			continue;
		}
		return retval;
	}
}

int sysutil_write(const int fd, const void* p_buf, const unsigned int size)
{
	while (1) {
		int retval = write(fd, p_buf, size);
		int saved_errno = errno;
		sysutil_check_pending_actions(kSysUtilIO, retval, fd);
		if (retval < 0 && saved_errno == EINTR) {
			continue;
		}
		return retval;
	}
}

int sysutil_read_loop(const int fd, void* p_buf, unsigned int size)
{
	int retval;
	int num_read = 0;

	while (1) {
		retval = sysutil_read(fd, (char*)p_buf + num_read, size);
		if (retval < 0) {
			return retval;
		} else if (retval == 0) {
			return num_read; 
		} if ((unsigned int) retval > size) {
			return -1;
		}

		num_read += retval;
		size -= (unsigned int) retval;
		if (size <= 0) {
			break;
		}
	}

	return num_read;
}

int sysutil_write_loop(const int fd, const void* p_buf, unsigned int size)
{
	int retval = 0;
	int num_written = 0;

	while (1) {
		retval = sysutil_write(fd, (const char*)p_buf + num_written, size);
		if (retval < 0) {
			return retval;
		} else if (retval == 0) {
			return num_written;
		}

		if ((unsigned int) retval > size) {
			return -1;
		}

		num_written += retval;
		size -= (unsigned int) retval;
		if (size <= 0) {
		}
	}

	return num_written;
}

filesize_t sysutil_get_file_offset(const int file_fd)
{
	filesize_t retval = lseek(file_fd, 0, SEEK_CUR);
	if (retval < 0) {
		return -1;
	}
	return retval;
}

filesize_t sysutil_lseek_to(const int fd, filesize_t seek_pos)
{
	if (seek_pos < 0) {
		return -1;
	}
	return lseek(fd, seek_pos, SEEK_SET);
}

struct sysutil_wait_retval sysutil_wait(void)
{
	struct sysutil_wait_retval retval;
	sysutil_memclr(&retval, sizeof(retval));

	while (1) {
		int sys_ret = wait(&retval.exit_status);
		if (sys_ret < 0 && errno == EINTR) {
			sysutil_check_pending_actions(kSysUtilUnknown, 0, 0);
			continue;
		}
		retval.syscall_retval = sys_ret;
		return retval;
	}
}

int sysutil_wait_reap_one(void)
{
	int retval = waitpid(-1, NULL, WNOHANG);
	if (retval == 0 || (retval < 0 && errno == ECHILD)) {
		return 0;
	}
	if (retval < 0) {
		return -1;
	}
	return retval;
}

int sysutil_wait_get_retval(const struct sysutil_wait_retval* p_waitret)
{
	return p_waitret->syscall_retval;
}

int sysutil_wait_exited_normally(const struct sysutil_wait_retval* p_waitret)
{
	return WIFEXITED(p_waitret->exit_status);
}

int sysutil_wait_get_exitcode(const struct sysutil_wait_retval* p_waitret)
{
	if (!sysutil_wait_exited_normally(p_waitret)) {
		return -1;
	}
	return WEXITSTATUS(p_waitret->exit_status);
}

filesize_t sysutil_a_to_filesize_t(const char* str_ptr)
{
	unsigned int i;
	filesize_t mult = 1;
	filesize_t result = 0;
	unsigned int len = sysutil_strlen(str_ptr);

	if (len > 15) {
		return 0;
	}

	for (i=0; i<len; ++i) {
		char the_char = str_ptr[len-(i+1)];
		filesize_t val;
		if (the_char < '0' || the_char > '9') {
			return 0;
		}
		val = the_char - '0';
		val *= mult;
		result += val;
		mult *= 10;
	}

	return result;
}

const char* sysutil_ulong_to_str(unsigned long the_ulong)
{
	static char ulong_buf[32];
	(void) snprintf(ulong_buf, sizeof(ulong_buf), "%lu", the_ulong);
	return ulong_buf;
}

const char* sysutil_filesize_t_to_str(filesize_t the_filesize)
{
	static char filesize_buf[32];

#ifdef __FreeBSD__
	snprintf(filesize_buf, sizeof(filesize_buf), "%ld", the_filesize);
#else 
	snprintf(filesize_buf, sizeof(filesize_buf), "%lld", the_filesize);
#endif

	return filesize_buf;
}

const char* sysutil_double_to_str(double the_double)
{
	static char double_buf[32];
	(void) snprintf(double_buf, sizeof(double_buf), "%.2f", the_double);
	return double_buf;
}

const char* sysutil_uint_to_octal(unsigned int the_uint)
{
	static char octal_buf[32];

	if (the_uint == 0) {
		octal_buf[0] = '0';
		octal_buf[1] = '\0';
	} else {
		(void) snprintf(octal_buf, sizeof(octal_buf), "0%o", the_uint);
	}

	return octal_buf;
}

unsigned int sysutil_octal_to_uint(const char* str_ptr)
{
	unsigned int result = 0;
	int seen_non_zero_digit = 0;

	while (*str_ptr != '\0') {
		int digit = *str_ptr;
		if (!isdigit(digit) || digit > '7') {
			break;
		}
		if (digit != '0') {
			seen_non_zero_digit = 1;
		}
		if (seen_non_zero_digit) {
			result <<= 3;
			result += (digit - '0');
		}
		str_ptr++;
	}
	return result;
}

char *sysutil_getcwd(char* dest_ptr, const unsigned int buf_size)
{
	char* p_retval = getcwd(dest_ptr, buf_size);
	dest_ptr[buf_size - 1] = '\0';
	return p_retval;
}

int sysutil_mkdir(const char* dirname_ptr, const unsigned int mode)
{
	return mkdir(dirname_ptr, mode);
}

int sysutil_rmdir(const char* dirname_ptr)
{
	return rmdir(dirname_ptr);
}

int sysutil_chdir(const char* dirname_ptr)
{
	return chdir(dirname_ptr);
}

int sysutil_rename(const char* p_from, const char* p_to)
{
	return rename(p_from, p_to);
}

struct sysutil_dir* sysutil_opendir(const char* dirname_ptr)
{
	return (struct sysutil_dir*) opendir(dirname_ptr);
}

int sysutil_closedir(struct sysutil_dir* dir_ptr)
{
	DIR* p_real_dir = (DIR*) dir_ptr;
	return closedir(p_real_dir);
}

const char* sysutil_next_dirent(struct sysutil_dir* dir_ptr)
{
	DIR* p_real_dir = (DIR*) dir_ptr;
	struct dirent* dir_ptrent = readdir(p_real_dir);
	if (dir_ptrent == NULL) {
		return NULL;
	}

	return dir_ptrent->d_name;
}

int sysutil_strlen(const char* text_ptr)
{
	unsigned int ret = strlen(text_ptr);
	
	if (ret > (INT_MAX / 8)) {
		return -1;
	}
	return ret;
}

char* sysutil_strdup(const char* str_ptr)
{
	return strdup(str_ptr);
}

void sysutil_memclr(void* dest_ptr, unsigned int size)
{
	if (size == 0) {
		return;
	}

	memset(dest_ptr, '\0', size);
}

void sysutil_memcpy(void* dest_ptr, const void* src_ptr, const unsigned int size)
{
	if (size == 0) {
		return;
	}

	memcpy(dest_ptr, src_ptr, size);
}

void sysutil_strcpy(char* dest_ptr, const char* src_ptr, unsigned int maxsize)
{
	if (maxsize == 0) {
		return;
	}

	strncpy(dest_ptr, src_ptr, maxsize);
	dest_ptr[maxsize - 1] = '\0';
}

int sysutil_memcmp(const void* src_ptr1, const void* src_ptr2, unsigned int size)
{
	if (size == 0) {
		return 0;
	}

	return memcmp(src_ptr1, src_ptr2, size);
}

int sysutil_strcmp(const char* src_ptr1, const char* src_ptr2)
{
	return strcmp(src_ptr1, src_ptr2);
}

int sysutil_getpagesize(void)
{
	static unsigned int s_page_size;
	if (s_page_size == 0) {
		s_page_size = getpagesize();
		if (s_page_size == 0) {
			return -1;
		}
	}

	return s_page_size;
}

static int sysutil_translate_memprot(const enum ESysUtilMapPermission perm)
{
	int retval = 0;

	switch (perm) {
		case kSysUtilMapProtReadOnly:
			retval = PROT_READ;
			break;
		case kSysUtilMapProtNone:
			retval = PROT_NONE;
			break;
		default:
			return -1;
			break;
	}
	return retval;
}

int sysutil_memprotect(void* p_addr, unsigned int len, const enum ESysUtilMapPermission perm)
{
	int prot = sysutil_translate_memprot(perm);
	int retval = mprotect(p_addr, len, prot);
	if (retval != 0) {
		return -1;
	}
	return 0;
}

int sysutil_memunmap(void* start_ptr, unsigned int length)
{
	return munmap(start_ptr, length);
}

int sysutil_dupfd2(int old_fd, int new_fd)
{
	int retval;

	if (old_fd == new_fd) {
		return -1;
	}
	retval = dup2(old_fd, new_fd);
	if (retval != new_fd) {
		return -2;
	}

	return 0;
}

static void sysutil_alloc_statbuf(struct sysutil_statbuf** stat_d_ptr)
{
	if(*stat_d_ptr == NULL) {
		*stat_d_ptr = reinterpret_cast<struct sysutil_statbuf *>(new (struct stat *));
	}
}

int sysutil_fstat(int fd, struct sysutil_statbuf** stat_d_ptr)
{
	sysutil_alloc_statbuf(stat_d_ptr);
	return  fstat(fd, (struct stat*) (*stat_d_ptr));
}

int sysutil_stat(const char* p_name, struct sysutil_statbuf** stat_d_ptr)
{
	sysutil_alloc_statbuf(stat_d_ptr);
	return stat(p_name, (struct stat*) (*stat_d_ptr));
}

int sysutil_lstat(const char* p_name, struct sysutil_statbuf** stat_d_ptr)
{
	sysutil_alloc_statbuf(stat_d_ptr);
	return lstat(p_name, (struct stat*) (*stat_d_ptr));
}

int sysutil_dir_stat(const struct sysutil_dir* dir_ptr, struct sysutil_statbuf** stat_d_ptr)
{
	int fd = dirfd((DIR*) dir_ptr);
	return sysutil_fstat(fd, stat_d_ptr);
}

int sysutil_statbuf_is_regfile(const struct sysutil_statbuf* stat_ptr)
{
	const struct stat* p_realstat = (const struct stat*) stat_ptr;
	return S_ISREG(p_realstat->st_mode);
}

int sysutil_statbuf_is_symlink(const struct sysutil_statbuf* stat_ptr)
{
	const struct stat* p_realstat = (const struct stat*) stat_ptr;
	return S_ISLNK(p_realstat->st_mode);
}

int sysutil_statbuf_is_socket(const struct sysutil_statbuf* stat_ptr)
{
	const struct stat* p_realstat = (const struct stat*) stat_ptr;
	return S_ISSOCK(p_realstat->st_mode);
}

int sysutil_statbuf_is_dir(const struct sysutil_statbuf* stat_ptr)
{
	const struct stat* p_realstat = (const struct stat*) stat_ptr;
	return S_ISDIR(p_realstat->st_mode);
}

const char* sysutil_statbuf_get_perms(const struct sysutil_statbuf* statbuf_ptr)
{
	int i;
	static char perms[11];
	const struct stat* stat_ptr = (const struct stat*) statbuf_ptr;

	for (i=0; i<10; i++) {
		perms[i] = '-';
	}

	perms[0] = '?';
	switch (stat_ptr->st_mode & S_IFMT) {
		case S_IFREG: perms[0] = '-'; break;
		case S_IFDIR: perms[0] = 'd'; break;
		case S_IFLNK: perms[0] = 'l'; break;
		case S_IFIFO: perms[0] = 'p'; break;
		case S_IFSOCK: perms[0] = 's'; break;
		case S_IFCHR: perms[0] = 'c'; break;
		case S_IFBLK: perms[0] = 'b'; break;
	}

	if (stat_ptr->st_mode & S_IRUSR) perms[1] = 'r';
	if (stat_ptr->st_mode & S_IWUSR) perms[2] = 'w';
	if (stat_ptr->st_mode & S_IXUSR) perms[3] = 'x';
	if (stat_ptr->st_mode & S_IRGRP) perms[4] = 'r';
	if (stat_ptr->st_mode & S_IWGRP) perms[5] = 'w';
	if (stat_ptr->st_mode & S_IXGRP) perms[6] = 'x';
	if (stat_ptr->st_mode & S_IROTH) perms[7] = 'r';
	if (stat_ptr->st_mode & S_IWOTH) perms[8] = 'w';
	if (stat_ptr->st_mode & S_IXOTH) perms[9] = 'x';
	if (stat_ptr->st_mode & S_ISUID) perms[3] = (perms[3] == 'x') ? 's' : 'S';
	if (stat_ptr->st_mode & S_ISGID) perms[6] = (perms[6] == 'x') ? 's' : 'S';
	if (stat_ptr->st_mode & S_ISVTX) perms[9] = (perms[9] == 'x') ? 't' : 'T';
	perms[10] = '\0';
	return perms;
}

const char *sysutil_statbuf_get_date(const struct sysutil_statbuf* statbuf_ptr)
{
	int retval = 0;
	static char datebuf[64];
	struct tm* tm_ptr;
	const struct stat* stat_ptr = (const struct stat*) statbuf_ptr;
	const char* date_format_str_ptr = "%Y/%m/%d %H:%M:%S";

	tm_ptr = localtime(&stat_ptr->st_mtime);
	retval = strftime(datebuf, sizeof(datebuf), date_format_str_ptr, tm_ptr);
	datebuf[sizeof(datebuf)-1] = '\0';

	if (retval == 0) {
		return NULL;
	}

	return datebuf;
}

const char* sysutil_statbuf_get_numeric_date(const struct sysutil_statbuf* statbuf_ptr, int use_localtime)
{
	int retval = 0;
	struct tm* tm_ptr;
	static char datebuf[15];
	const struct stat* stat_ptr = (const struct stat*) statbuf_ptr;

	if (!use_localtime) {
		tm_ptr = gmtime(&stat_ptr->st_mtime);
	} else {
		tm_ptr = localtime(&stat_ptr->st_mtime);
	}
	retval = strftime(datebuf, sizeof(datebuf), "%Y/%m/%d %H:%M:%S", tm_ptr);
	if (retval == 0) {
		return NULL;
	}
	return datebuf;
}

filesize_t sysutil_statbuf_get_size(const struct sysutil_statbuf* statbuf_ptr)
{
	const struct stat* stat_ptr = (const struct stat*) statbuf_ptr;

	if (stat_ptr->st_size < 0) {
		return 0;
	}

	return stat_ptr->st_size;
}

int sysutil_statbuf_get_uid(const struct sysutil_statbuf* statbuf_ptr)
{
	const struct stat* stat_ptr = (const struct stat*) statbuf_ptr;
	return stat_ptr->st_uid;
}

int sysutil_statbuf_get_gid(const struct sysutil_statbuf* statbuf_ptr)
{
	const struct stat* stat_ptr = (const struct stat*) statbuf_ptr;
	return stat_ptr->st_gid;
}

unsigned int sysutil_statbuf_get_links(const struct sysutil_statbuf* statbuf_ptr)
{
	const struct stat* stat_ptr = (const struct stat*) statbuf_ptr;
	return stat_ptr->st_nlink;
}

int sysutil_statbuf_is_readable_other(const struct sysutil_statbuf* statbuf_ptr)
{
	const struct stat* stat_ptr = (const struct stat*) statbuf_ptr;
	
	if (stat_ptr->st_mode & S_IROTH) {
		return 1;
	}

	return 0;
}

const char* sysutil_statbuf_get_sortkey_mtime(const struct sysutil_statbuf* statbuf_ptr)
{
	static char intbuf[32];
	const struct stat* stat_ptr = (const struct stat*) statbuf_ptr;

	snprintf(intbuf, sizeof(intbuf), "%030ld", (long) stat_ptr->st_mtime);
	return intbuf;
}

int sysutil_fchown(const int fd, const int uid, const int gid)
{
	return fchown(fd, uid, gid);
}

int sysutil_fchmod(const int fd, unsigned int mode)
{
	mode = mode & 0777;
	return fchmod(fd, mode);
}

int sysutil_chmod(const char* file_ptrname, unsigned int mode)
{
	mode = mode & 0777;
	return chmod(file_ptrname, mode);
}

int sysutil_lock_file_write(int fd)
{
	return lock_internal(fd, F_WRLCK);
}

int sysutil_lock_file_read(int fd)
{
	return lock_internal(fd, F_RDLCK);
}

static int lock_internal(int fd, int lock_type)
{
	int retval = 0;
	int saved_errno = 0;
	struct flock the_lock;
	sysutil_memclr(&the_lock, sizeof(the_lock));

	the_lock.l_type = lock_type;
	the_lock.l_whence = SEEK_SET;
	the_lock.l_start = 0;
	the_lock.l_len = 0;

	do {
		retval = fcntl(fd, F_SETLKW, &the_lock);
		saved_errno = errno;
		sysutil_check_pending_actions(kSysUtilUnknown, 0, 0);
	} while (retval < 0 && saved_errno == EINTR);

	return retval;
}

int sysutil_unlock_file(int fd)
{
	struct flock the_lock;
	sysutil_memclr(&the_lock, sizeof(the_lock));

	the_lock.l_type = F_UNLCK;
	the_lock.l_whence = SEEK_SET;
	the_lock.l_start = 0;
	the_lock.l_len = 0;

	return fcntl(fd, F_SETLK, &the_lock);
}

int sysutil_readlink(const char* file_ptrname, char* dest_ptr, unsigned int bufsiz)
{
	int retval = readlink(file_ptrname, dest_ptr, bufsiz - 1);
	if (retval < 0) {
		return retval;
	}
	dest_ptr[retval] = '\0';
	return retval;
}

int sysutil_retval_is_error(int retval)
{
	if (retval < 0) {
		return 1;
	}
	return 0;
}

enum ESysUtilError sysutil_get_error(void)
{
	enum ESysUtilError retval = kSysUtilErrUnknown;
	switch (errno) {
		case EADDRINUSE:
			retval = kSysUtilErrADDRINUSE;
			break;
		case ENOSYS:
			retval = kSysUtilErrNOSYS;
			break;
		case EINTR:
			retval = kSysUtilErrINTR;
			break;
		case EINVAL:
			retval = kSysUtilErrINVAL;
			break;
		case EOPNOTSUPP:
			retval = kSysUtilErrOPNOTSUPP;
			break;
	}

	return retval;
}

struct sysutil_user* sysutil_getpwuid(const int uid)
{
	if (uid < 0) {
		return NULL;
	}

	return (struct sysutil_user*) getpwuid((unsigned int) uid);
}

struct sysutil_user* sysutil_getpwnam(const char* user_ptr)
{
	return (struct sysutil_user*) getpwnam(user_ptr);
}

const char* sysutil_user_getname(const struct sysutil_user* user_ptr)
{
	const struct passwd* p_passwd = (const struct passwd*) user_ptr;
	return p_passwd->pw_name;
}

const char* sysutil_user_get_homedir(const struct sysutil_user* user_ptr)
{
	const struct passwd* p_passwd = (const struct passwd*) user_ptr;
	return p_passwd->pw_dir;
}

int sysutil_user_getuid(const struct sysutil_user* user_ptr)
{
	const struct passwd* p_passwd = (const struct passwd*) user_ptr;
	return p_passwd->pw_uid;
}

int sysutil_user_getgid(const struct sysutil_user* user_ptr)
{ 
	const struct passwd* p_passwd = (const struct passwd*) user_ptr;
	return p_passwd->pw_gid;
}

struct sysutil_group* sysutil_getgrgid(const int gid)
{
	if (gid < 0) {
		return NULL;
	}
	return (struct sysutil_group*) getgrgid((unsigned int) gid);
}

const char* sysutil_group_getname(const struct sysutil_group* p_group)
{
	const struct group* p_grp = (const struct group*) p_group;
	return p_grp->gr_name;
}

int sysutil_running_as_root(void)
{
	return (getuid() == 0);
}

int sysutil_setuid(const struct sysutil_user* user_ptr)
{
	const struct passwd* p_passwd = (const struct passwd*) user_ptr;
	return sysutil_setuid_numeric(p_passwd->pw_uid);
}

int sysutil_setuid_numeric(int uid)
{
	return setuid(uid);
}

int sysutil_setgid(const struct sysutil_user* user_ptr)
{
	const struct passwd* p_passwd = (const struct passwd*) user_ptr;
	return sysutil_setgid_numeric(p_passwd->pw_gid);
}

int sysutil_setgid_numeric(int gid)
{
	return setgid(gid);
}

int sysutil_geteuid(void)
{
	return geteuid();
}

int sysutil_getegid(void)
{
	return getegid();
}

int sysutil_seteuid(const struct sysutil_user* user_ptr)
{
	const struct passwd* p_passwd = (const struct passwd*) user_ptr;
	return sysutil_seteuid_numeric(p_passwd->pw_uid);
}

int sysutil_setegid(const struct sysutil_user* user_ptr)
{
	const struct passwd* p_passwd = (const struct passwd*) user_ptr;
	return sysutil_setegid_numeric(p_passwd->pw_gid);
}

int sysutil_seteuid_numeric(int uid)
{
	return setreuid(static_cast<uid_t>(-1), uid);
}

int sysutil_setegid_numeric(int gid)
{
	return setregid(static_cast<uid_t>(-1), gid);
}

int sysutil_clear_supp_groups(void)
{
	return setgroups(0, NULL);
}

int sysutil_initgroups(const struct sysutil_user* user_ptr)
{
	const struct passwd* p_passwd = (const struct passwd*) user_ptr;
	return initgroups(p_passwd->pw_name, p_passwd->pw_gid);
}

int sysutil_chroot(const char* p_root_path)
{
	return chroot(p_root_path);
}

unsigned int sysutil_get_umask(void)
{
	return s_current_umask;
}

int sysutil_set_umask(unsigned int new_umask)
{
	s_current_umask = (new_umask & 0777);
	return umask(s_current_umask);
}

int sysutil_make_session_leader(void)
{
	(void) setsid();
	if (getpid() != getpgrp()) {
		return -1;
	}
	return 1;
}

int sysutil_tzset(void)
{
	int retval;
	char tzbuf[sizeof("+HHMM!")];
	time_t the_time = time(NULL);
	struct tm* tm_ptr;

	tzset();
	tm_ptr = localtime(&the_time);
	if (tm_ptr == NULL) {
		return -1;
	}

	retval = strftime(tzbuf, sizeof(tzbuf), "%z", tm_ptr);
	tzbuf[sizeof(tzbuf) - 1] = '\0';
	if (retval == 5) {
		static char envtz[sizeof("TZ=UTC-hh:mm")];
		tzbuf[5] = tzbuf[4];
		tzbuf[4] = tzbuf[3];
		tzbuf[3] = ':';

		if (tzbuf[0] == '+') {
			tzbuf[0] = '-';
		} else {
			tzbuf[0] = '+';
		}
		snprintf(envtz, sizeof(envtz), "TZ=UTC%s", tzbuf);
		putenv(envtz);
		s_timezone = ((tzbuf[1] - '0') * 10 + (tzbuf[2] - '0')) * 60 * 60;
		s_timezone += ((tzbuf[4] - '0') * 10 + (tzbuf[5] - '0')) * 60;
		if (tzbuf[0] == '-') {
			s_timezone *= -1;
		}
	}
	return 1;
}

const char* sysutil_get_current_datetime(void)
{
	int i = 0;
	static char datebuf[64];
	const struct tm* tm_ptr;
	const time_t curr_time = time(NULL);
	tm_ptr = localtime(&curr_time);
	if (strftime(datebuf, sizeof(datebuf), "%Y/%m/%d %H:%M:%S", tm_ptr) == 0) {
		return NULL;
	}
	datebuf[sizeof(datebuf) - 1] = '\0';

	while (datebuf[i] != '!' && datebuf[i] != '\0') {
		++i;
	}

	if (datebuf[i] == '!') {
		datebuf[i] = ' ';
		if (datebuf[i+1] == '0') {
			datebuf[i+1] = ' ';
		}
	}

	return datebuf;
}

const char* sysutil_get_current_date(void)
{
	int i = 0;
	static char datebuf[64];
	const struct tm* tm_ptr;
	const time_t curr_time = time(NULL);
	tm_ptr = localtime(&curr_time);
	if (strftime(datebuf, sizeof(datebuf), "%Y%m%d", tm_ptr) == 0) {
		return NULL;
	}
	datebuf[sizeof(datebuf) - 1] = '\0';

	while (datebuf[i] != '!' && datebuf[i] != '\0') {
		++i;
	}

	if (datebuf[i] == '!') {
		datebuf[i] = ' ';
		if (datebuf[i+1] == '0') {
			datebuf[i+1] = ' ';
		}
	}

	return datebuf;
}

void sysutil_sleep(double seconds)
{
	int retval;
	int saved_errno;
	double fractional;
	time_t secs;
	struct timespec ts;
	secs = (time_t) seconds;
	fractional = seconds - (double) secs;
	ts.tv_sec = secs;
	ts.tv_nsec = (long) (fractional * (double) 1000000000);

	do {
		retval = nanosleep(&ts, &ts);
		saved_errno = errno;
		sysutil_check_pending_actions(kSysUtilUnknown, 0, 0);
	} while (retval == -1 && saved_errno == EINTR);
}

char* sysutil_getenv(const char* p_var)
{
	return getenv(p_var);
}

