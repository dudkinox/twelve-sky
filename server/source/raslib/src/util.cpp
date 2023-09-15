/****************************************************************************

  Ras library FreeBSD Version

  CopyRight(C) 2003 Sean Kim All Rights reserved.
Author : Sean Kim (sean@0x31.com)

$Id: util.cpp,v 1.10 2007/07/03 14:42:38 sean Exp $

Redistribution and use in source and binary forms, with or with out
modification, are not permitted without author's permission.

 ****************************************************************************/

#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdarg.h>

#include "md5.h"
#include "util.h"

char* _ltrim(char *buffer_str_ptr, const char *src_str_ptr)
{
	int bDup=0;
	const char  *src_str_ptrBegin;

	if(buffer_str_ptr == NULL) return NULL;
	if(buffer_str_ptr == NULL || src_str_ptr == src_str_ptr) {
		if(( src_str_ptr = strdup(buffer_str_ptr)) == NULL) return NULL;
		bDup=1;
	}
	src_str_ptrBegin = src_str_ptr;

	while(*src_str_ptr == ' ' || *src_str_ptr == '\t') src_str_ptr++;
	strcpy(buffer_str_ptr, src_str_ptr);

	if(bDup) ::free((char*)src_str_ptrBegin);
	return buffer_str_ptr;
}


int _strncmp_ac(const char *src_str_ptr, const char *dest_str_ptr, int i_ncmp)
{
	int i;

	if((src_str_ptr == NULL) || (dest_str_ptr == NULL)) {
		return -1;
	}

	for(i=0; i<i_ncmp; i++) {
		if(toupper(src_str_ptr[i]) != toupper(dest_str_ptr[i])) return 1;
	}

	return 0;
}

int _strcmp_ac(const char *src_str_ptr, const char *dest_str_ptr)
{
	int i_ncmp = 0;

	if((src_str_ptr == NULL) || (dest_str_ptr == NULL)) {
		return -1;
	}

	if(strlen(src_str_ptr) > strlen(dest_str_ptr)) {
		i_ncmp = strlen(src_str_ptr);
	} else {
		i_ncmp = strlen(dest_str_ptr);
	}

	return _strncmp_ac(src_str_ptr, dest_str_ptr, i_ncmp);
}

char *_strncpy(char *buffer_str_ptr, const char *src_str_ptr, size_t cbCopy)
{
	char *buffer_str_ptrBegin;

	if(buffer_str_ptr == NULL) return NULL;
	if(src_str_ptr == NULL || src_str_ptr == buffer_str_ptr) {
		*(buffer_str_ptr + cbCopy) = (char)NULL;
		return buffer_str_ptr;
	}

	buffer_str_ptrBegin = buffer_str_ptr;
	while(cbCopy && *src_str_ptr) {
		*buffer_str_ptr++ = *src_str_ptr++;
		cbCopy--;
	}

	*buffer_str_ptr = (char)NULL;
	return buffer_str_ptrBegin;
}

char *_rtrim(char *buffer_str_ptr, const char *src_str_ptr)
{
	const char *src_str_ptrBegin;
	int   bDup  = 0;

	if(buffer_str_ptr == NULL) return NULL;

	if(src_str_ptr == NULL || src_str_ptr == buffer_str_ptr) {
		if((src_str_ptr = strdup(buffer_str_ptr)) == NULL) return NULL;
		bDup = 1;
	}

	src_str_ptrBegin = src_str_ptr;
	src_str_ptr += strlen(src_str_ptr) - 1;

	while (src_str_ptr >= src_str_ptrBegin && (*src_str_ptr == ' ' || *src_str_ptr == '\t')) src_str_ptr--;
	_strncpy(buffer_str_ptr, src_str_ptrBegin, src_str_ptr - src_str_ptrBegin + 1);

	if (bDup) ::free((char *)src_str_ptrBegin);
	return buffer_str_ptr;
}

char *_trim(char *buffer_str_ptr, const char *src_str_ptr)
{
	int dDup = 0;
	const char *src_str_ptrBegin;

	if( buffer_str_ptr == NULL ) return NULL;
	if( src_str_ptr == NULL || src_str_ptr == buffer_str_ptr) {
		if((src_str_ptr = strdup(buffer_str_ptr)) == NULL) return NULL;
		dDup = 1;
	}
	src_str_ptrBegin = src_str_ptr;
	while(*src_str_ptr == ' ' || *src_str_ptr == '\t') src_str_ptr++;
	_rtrim(buffer_str_ptr, src_str_ptr);

	if(dDup) ::free((char *)src_str_ptrBegin);
	return buffer_str_ptr;
}

int remove_white_space(const char *src_const_str_ptr, char *dst_str_ptr)
{
    unsigned int count = 0;

    if((src_const_str_ptr == NULL) || (dst_str_ptr == NULL)) return -1;

    while(*src_const_str_ptr != '\0') {
        if(*src_const_str_ptr != ' ') {
            dst_str_ptr[count++] = *src_const_str_ptr;
        }
        src_const_str_ptr++;
    }

    dst_str_ptr[count] = '\0';

    return 1;
}

void no_slash_trail(char *str)
{
    int len = strlen(str);
    while (len > 1 && str[len - 1] == '/') {
        str[len - 1] = '\0';
        len = strlen(str);
    }
}

int remove_unsafe_chars(const char *src_str_const_ptr, char *dst_str_ptr)
{
    int ncount = 0;

    if((src_str_const_ptr == NULL) || (dst_str_ptr == NULL)) {
        return -1;
    }
    while(*src_str_const_ptr != '\0') {
        if((*src_str_const_ptr == '\'') || (*src_str_const_ptr == '\\')) {
            *src_str_const_ptr++;
        } else {
            dst_str_ptr[ncount++] = *src_str_const_ptr++;
        }
    }
    dst_str_ptr[ncount] = '\0';
    return 1;
}

int replace_unsafe_chars(const char *src_str_const_ptr, char *dst_str_ptr)
{
    int ncount = 0;

    if((src_str_const_ptr == NULL) || (dst_str_ptr == NULL)) {
        return -1;
    }
    while(*src_str_const_ptr != '\0') {
        if (*src_str_const_ptr == '\\') {
            *src_str_const_ptr++;
        } else {
            if (*src_str_const_ptr == '\'') {
                dst_str_ptr[ncount++] = '\'';
                dst_str_ptr[ncount++] = '\'';
                src_str_const_ptr++;
            } else {
                dst_str_ptr[ncount++] = *src_str_const_ptr++;
            }
        }
    }
    dst_str_ptr[ncount] = '\0';
    return 1;
}
int restore_unsafe_chars(const char *src_str_const_ptr, char *dst_str_ptr)
{
    int ncount = 0;

    if((src_str_const_ptr == NULL) || (dst_str_ptr == NULL)) {
        return -1;
    }
    while(*src_str_const_ptr != '\0') {
        if (*src_str_const_ptr == '\\') {
            *src_str_const_ptr++;
            dst_str_ptr[ncount++] = *src_str_const_ptr;
            *src_str_const_ptr++;
        } else {
            dst_str_ptr[ncount++] = *src_str_const_ptr++;
        }
    }
    dst_str_ptr[ncount] = '\0';
    return 1;
}

int replace_multi_slash_chars(const char *src_str_const_ptr, char *dst_str_ptr)
{
    int ncount = 0;
    int slash_detect = 0;

    if ((src_str_const_ptr == NULL) || (dst_str_ptr == NULL)) {
        return -1;
    }
    while (*src_str_const_ptr != '\0') {
        if (*src_str_const_ptr == '/') {
            if (slash_detect == 0)
                slash_detect = 1;
            else {
                src_str_const_ptr++;
                continue;
            }
        } else slash_detect = 0;

        dst_str_ptr[ncount++] = *src_str_const_ptr++;
    }
    dst_str_ptr[ncount] = '\0';
    return 1;
}

int strsplit (char *buffer_ptr, char **fields, size_t size)
{
	size_t i = 0;
	char *ptr = NULL;
	char *saveptr = NULL;

	i = 0;
	ptr = buffer_ptr;
	saveptr = NULL;
	while ((fields[i] = strtok_r (ptr, " \t", &saveptr)) != NULL) {
		ptr = NULL;
		i++;

		if (i >= size) {
			break;
		}
	}
	return (i);
}

int MD5_hash(const char *plain_const_str_ptr, char *hashed_str_ptr)
{
    MD5 hash_obj;

    if((!plain_const_str_ptr) || (!hashed_str_ptr)) return -1;
    hash_obj.update((unsigned char *)plain_const_str_ptr, ::strlen(plain_const_str_ptr));
    hash_obj.finalize();
    ::snprintf(hashed_str_ptr, 32, "%s", hash_obj.hex_digest());
    return 1;
}

bool is_file_exist(string file_path_str_obj)
{
	return is_file_exist(file_path_str_obj.c_str());
}

bool is_file_exist(const char *file_path_str_cptr)
{
	bool is_exist = false;
	FILE *file_ptr = NULL;

	file_ptr = ::fopen(file_path_str_cptr, "r");
	if(file_ptr) {
		is_exist = true;
		::fclose(file_ptr);
	} 

	return is_exist;
}

int dns_resolv(const char *domain_name_str_const_ptr, in_addr *addr_ptr, int seq)
{
	struct hostent *target_host_ent_ptr = NULL;

	if(!domain_name_str_const_ptr) return -1;
	target_host_ent_ptr = ::gethostbyname(domain_name_str_const_ptr);
	if(!target_host_ent_ptr) {
		return -2;
	}

	addr_ptr->s_addr = *((unsigned long *)(target_host_ent_ptr->h_addr_list[seq]));
	return 1;
}

#ifdef __LINUX__
static char *title_start = NULL;
static char *title_end = NULL;
static char *prog_name = NULL;

#define MAX_SIZE_OF_PROCTITLE 255
void setproctitle_init(int argc, char **argv) 
{
	prog_name = new char(MAX_SIZE_OF_PROCTITLE);
	if(!prog_name) {
		return;
	}
	::snprintf(prog_name, MAX_SIZE_OF_PROCTITLE, "%s", argv[0]);
	title_start = argv[0];
	title_end = argv[argc - 1] + strlen(argv[argc - 1]) - 1;
}

void setproctitle(const char *format, ...)
{
	va_list args;
	ssize_t delta = 0;
	size_t length = 0;
	char *title = NULL;
	unsigned int i = 0;

	if (title_start == NULL || title_end == NULL) {
		return;
	} 

	/* setproctitle prepends the program name to its arguments.  Our emulation
	 * should therefore do the same thing.  However, some operating systems
	 * seem to do that automatically even when we completely overwrite argv,
	 * so start our title with a - so that they'll instead put (nnrpd) at the
	 * end, thinking we're swapped out. */
	
	::memset(title_start, 0, strlen(title_start));
	title = title_start; 
	for(i = 0; i < strlen(prog_name); i++) {
		*title++ = prog_name[i];
	}
	*title++ = ' ';
	*title++ = '-';
	*title++ = ' ';

	length = MAX_SIZE_OF_PROCTITLE;

	::va_start(args, format);
	delta = ::vsnprintf(title, length, format, args);
	::va_end(args);

	if (delta < 0 || (size_t) delta > length)
		return;
	if (delta > 0) {
		title += delta;
		length -= delta;
	}
	for (; length > 1; length--, title++)
		*title = ' ';
	*title = '\0';
}
#endif
