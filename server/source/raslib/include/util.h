/****************************************************************************

   Ras library FreeBSD Version

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: util.h,v 1.7 2007/07/03 14:42:38 sean Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#ifndef __UTIL_H__
#define __UTIL_H__

#define ltrim(psz)			_ltrim(psz, NULL)
#define rtrim(psz)			_rtrim(psz, NULL)
#define trim(psz)				_trim(psz,NULL)

#include <iostream>
#include <string>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

using namespace std;
extern char* _ltrim(char *pszBuf, const char *pszSrc);
extern int _strncmp_ac(const char *pszS1, const char *pszS2, int i_ncmp);
extern int _strcmp_ac(const char *pszS1, const char *pszS2);
extern char *_trim(char *pszBuf, const char *pszSrc);
extern char *_rtrim(char *pszBuf, const char *pszSrc);
extern char *_strncpy(char *pszBuf, const char *pszSrc, size_t cbCopy);
extern int MD5_hash(const char *plain_const_str_ptr, char *hashed_str_ptr);
extern int replace_multi_slash_chars(const char *src_str_const_ptr, char *dst_str_ptr);
extern int restore_unsafe_chars(const char *src_str_const_ptr, char *dst_str_ptr);
extern int replace_unsafe_chars(const char *src_str_const_ptr, char *dst_str_ptr);
extern int remove_unsafe_chars(const char *src_str_const_ptr, char *dst_str_ptr);
extern void no_slash_trail(char *str);
extern int remove_white_space(const char *src_const_str_ptr, char *dst_str_ptr);
extern int dns_resolv(const char *domain_name_str_const_ptr, in_addr *addr_ptr, int seq);
extern bool is_file_exist(string file_path_str_obj);
extern bool is_file_exist(const char *file_path_str_cptr);
extern int strsplit (char *buffer_ptr, char **fields, size_t size);

#ifdef __LINUX__
extern void setproctitle_init(int argc, char **argv);
extern void setproctitle(const char *format, ...);
#endif

#endif
