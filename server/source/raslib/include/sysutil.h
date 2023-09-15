/****************************************************************************

   Ras library FreeBSD Version

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: sysutil.h,v 1.2 2007/04/23 09:28:58 sean Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#ifndef __SYSUTIL_H__
#define __SYSUTIL_H_

#include <time.h>
#include <stdlib.h>
#include <unistd.h>

typedef off_t filesize_t;
#ifndef INT_MAX
#define INT_MAX 4294967295
#endif

/* Return value queries */
int sysutil_retval_is_error(int retval);
enum ESysUtilError
{
  kSysUtilErrUnknown = 1,
  kSysUtilErrADDRINUSE,
  kSysUtilErrNOSYS,
  kSysUtilErrINTR,
  kSysUtilErrINVAL,
  kSysUtilErrOPNOTSUPP
};
enum ESysUtilError sysutil_get_error(void);

/* Signal handling utility functions */
enum ESysUtilSignal
{
  kSysUtilSigALRM = 1,
  kSysUtilSigTERM,
  kSysUtilSigCHLD,
  kSysUtilSigPIPE,
  kSysUtilSigURG,
  kSysUtilSigHUP
};

enum ESysUtilInterruptContext
{
  kSysUtilUnknown,
  kSysUtilIO
};

typedef void (*sighandle_t)(void*);
typedef void (*async_sighandle_t)(int);
typedef void (*context_io_t)(int, int, void*);

void sysutil_install_null_sighandler(const enum ESysUtilSignal sig);
void sysutil_install_sighandler(const enum ESysUtilSignal, sighandle_t handler, void* p_private);
void sysutil_install_async_sighandler(const enum ESysUtilSignal sig, async_sighandle_t handler);
void sysutil_default_sig(const enum ESysUtilSignal sig);
int sysutil_install_io_handler(context_io_t handler, void* p_private);
int sysutil_uninstall_io_handler(void);
void sysutil_check_pending_actions(const enum ESysUtilInterruptContext context, int retval, int fd);
int sysutil_block_sig(const enum ESysUtilSignal sig);
int sysutil_unblock_sig(const enum ESysUtilSignal sig);

/* Alarm setting/clearing utility functions */
void sysutil_set_alarm(const unsigned int trigger_seconds);
void sysutil_clear_alarm(void);

/* Directory related things */
char* sysutil_getcwd(char* dest_ptr, const unsigned int buf_size);
int sysutil_mkdir(const char* dirname_ptr, const unsigned int mode);
int sysutil_rmdir(const char* dirname_ptr);
int sysutil_chdir(const char* dirname_ptr);
int sysutil_rename(const char* p_from, const char* p_to);

struct sysutil_dir;
struct sysutil_dir* sysutil_opendir(const char* dirname_ptr);
int sysutil_closedir(struct sysutil_dir* dir_ptr);
const char* sysutil_next_dirent(struct sysutil_dir* dir_ptr);

/* File create/open/close etc. */
enum ESysUtilOpenMode
{
  kSysUtilOpenReadOnly = 1,
  kSysUtilOpenWriteOnly,
  kSysUtilOpenReadWrite
};

int sysutil_dupfd2(int old_fd, int new_fd);

filesize_t sysutil_lseek_to(const int fd, filesize_t seek_pos);
filesize_t sysutil_get_file_offset(const int file_fd);
int sysutil_read(const int fd, void* buf_ptr, const unsigned int size);
int sysutil_write(const int fd, const void* buf_ptr, const unsigned int size);

int sysutil_read_loop(const int fd, void* buf_ptr, unsigned int size);
int sysutil_write_loop(const int fd, const void* buf_ptr, unsigned int size);

struct sysutil_statbuf;
int sysutil_stat(const char* p_name, struct sysutil_statbuf** stat_d_ptr);
int sysutil_lstat(const char* p_name, struct sysutil_statbuf** stat_d_ptr);
int sysutil_fstat(int fd, struct sysutil_statbuf** stat_d_ptr);
int sysutil_dir_stat(const struct sysutil_dir* dir_ptr, struct sysutil_statbuf** stat_d_ptr);
int sysutil_statbuf_is_regfile(const struct sysutil_statbuf* stat_ptr);
int sysutil_statbuf_is_symlink(const struct sysutil_statbuf* stat_ptr);
int sysutil_statbuf_is_socket(const struct sysutil_statbuf* stat_ptr);
int sysutil_statbuf_is_dir(const struct sysutil_statbuf* stat_ptr);
filesize_t sysutil_statbuf_get_size(const struct sysutil_statbuf* stat_ptr);
const char* sysutil_statbuf_get_perms(const struct sysutil_statbuf* stat_ptr);
const char* sysutil_statbuf_get_date(const struct sysutil_statbuf* stat_ptr);
const char* sysutil_statbuf_get_numeric_date(const struct sysutil_statbuf* stat_ptr, int use_localtime);
unsigned int sysutil_statbuf_get_links(const struct sysutil_statbuf* stat_ptr);
int sysutil_statbuf_get_uid(const struct sysutil_statbuf* stat_ptr);
int sysutil_statbuf_get_gid(const struct sysutil_statbuf* stat_ptr);
int sysutil_statbuf_is_readable_other(const struct sysutil_statbuf* stat_ptr);
const char* sysutil_statbuf_get_sortkey_mtime(const struct sysutil_statbuf* stat_ptr);

int sysutil_chmod(const char* p_filename, unsigned int mode);
int sysutil_fchown(const int fd, const int uid, const int gid);
int sysutil_fchmod(const int fd, unsigned int mode);
int sysutil_readlink(const char* p_filename, char* dest_ptr, unsigned int bufsiz);

int sysutil_lock_file_write(int fd);
int sysutil_lock_file_read(int fd);
int sysutil_unlock_file(int fd);

enum ESysUtilMapPermission {
  kSysUtilMapProtReadOnly = 1,
  kSysUtilMapProtNone
};

int sysutil_memprotect(void* p_addr, unsigned int len, const enum ESysUtilMapPermission perm);
int sysutil_memunmap(void* p_start, unsigned int length);

struct sysutil_wait_retval {
  int syscall_retval;
  int exit_status;
};

struct sysutil_wait_retval sysutil_wait(void);
int sysutil_wait_reap_one(void);
int sysutil_wait_get_retval(const struct sysutil_wait_retval* p_waitret);
int sysutil_wait_exited_normally(const struct sysutil_wait_retval* p_waitret);
int sysutil_wait_get_exitcode(const struct sysutil_wait_retval* p_waitret);

/* Various string functions */
int sysutil_strlen(const char* p_text);
char* sysutil_strdup(const char* str_ptr);
void sysutil_memclr(void* dest_ptr, unsigned int size);
void sysutil_memcpy(void* dest_ptr, const void* src_ptr, const unsigned int size);
void sysutil_strcpy(char* dest_ptr, const char* src_ptr, unsigned int maxsize);
int sysutil_memcmp(const void* src_ptr1, const void* src_ptr2, unsigned int size);
int sysutil_strcmp(const char* src_ptr1, const char* src_ptr2);
int sysutil_atoi(const char* str_ptr);
filesize_t sysutil_a_to_filesize_t(const char* str_ptr);
const char* sysutil_ulong_to_str(unsigned long the_ulong);
const char* sysutil_filesize_t_to_str(filesize_t the_filesize);
const char* sysutil_double_to_str(double the_double);
const char* sysutil_uint_to_octal(unsigned int the_uint);
unsigned int sysutil_octal_to_uint(const char* str_ptr);

/* User database queries etc. */
struct sysutil_user;
struct sysutil_group;

struct sysutil_user* sysutil_getpwuid(const int uid);
struct sysutil_user* sysutil_getpwnam(const char* user_ptr);
const char* sysutil_user_getname(const struct sysutil_user* user_ptr);
const char* sysutil_user_get_homedir(const struct sysutil_user* user_ptr);
int sysutil_user_getuid(const struct sysutil_user* user_ptr);
int sysutil_user_getgid(const struct sysutil_user* user_ptr);

struct sysutil_group* sysutil_getgrgid(const int gid);
const char* sysutil_group_getname(const struct sysutil_group* p_group);

int sysutil_getpagesize(void);
unsigned int sysutil_get_umask(void);
int sysutil_set_umask(unsigned int umask);
int sysutil_make_session_leader(void);
int sysutil_tzset(void);
const char* sysutil_get_current_date(void);
const char* sysutil_get_current_datetime(void);
char* sysutil_getenv(const char* p_var);
typedef void (*exitfunc_t)(void);
void sysutil_set_exit_func(exitfunc_t exitfunc);

/* Credentials handling */
int sysutil_running_as_root(void);
int sysutil_setuid(const struct sysutil_user* user_ptr);
int sysutil_setgid(const struct sysutil_user* user_ptr);
int sysutil_setuid_numeric(int uid);
int sysutil_setgid_numeric(int gid);
int sysutil_geteuid(void);
int sysutil_getegid(void);
int sysutil_seteuid(const struct sysutil_user* user_ptr);
int sysutil_setegid(const struct sysutil_user* user_ptr);
int sysutil_seteuid_numeric(int uid);
int sysutil_setegid_numeric(int gid);
int sysutil_clear_supp_groups(void);
int sysutil_initgroups(const struct sysutil_user* user_ptr);
int sysutil_chroot(const char* p_root_path);

void sysutil_sleep(double seconds);
#endif
