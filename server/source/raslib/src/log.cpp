/****************************************************************************

   Ras library FreeBSD Version

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: log.cpp,v 1.14 2008/07/18 12:31:36 mirshead Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdarg.h>

#include "log.h"
#include "raslib.h"
#include "sysutil.h"

#define MAX_SIZE_OF_LOG_STR  8192
namespace SDLog
{
CLogging::CLogging(void)
{
	m_log_fd = -1;
	m_init_flag = false;
	m_file_obj_ptr = NULL;
	m_tunable_do_lock = ENABLE_LOG_LOCK;

	::sem_init(&sem_write, 0, 1);
}

CLogging::~CLogging(void) throw()
{
	::fclose(m_file_obj_ptr);
}

int CLogging::set_log_file_path(const string file_path_str_obj, const char *mode_str_cptr) 
{
	if(file_path_str_obj.length() <= 1) {
		return -1;
	}

	if((!mode_str_cptr) || (::strlen(mode_str_cptr) < 1)) {
		return -2;
	}

	m_file_path_str_obj = file_path_str_obj;
	m_file_open_mode_str_obj = mode_str_cptr;
	return 1;
}

int CLogging::log_to_file(const char *format_str_ptr, ...) 
{
	int log_fd = -1;
	va_list format_list;
	string file_path_str_obj = m_file_path_str_obj;

	::sem_wait(&sem_write);
	
	file_path_str_obj += sysutil_get_current_date();
	file_path_str_obj += ".log";

	m_file_obj_ptr = ::fopen(file_path_str_obj.c_str(), m_file_open_mode_str_obj.c_str());
	if(!m_file_obj_ptr) { 
		::sem_post (&sem_write);
		return -1;
	}

	if(m_file_obj_ptr) {
#ifdef __FreeBSD__
		log_fd = m_file_obj_ptr->_file;
#else 
		log_fd = m_file_obj_ptr->_fileno;
#endif
	} else if(m_log_fd > 0) {
		log_fd = m_log_fd;
	} else {
		::sem_post (&sem_write);
		::fclose(m_file_obj_ptr);
		return -2;
	}

	va_start (format_list, format_str_ptr);
	do_common_log(log_fd, format_str_ptr, format_list);
	::fclose(m_file_obj_ptr);

	::sem_post(&sem_write);

	return 1;
}
	
void CLogging::log_to_cons_stdout(const char *format_str_ptr, ...)
{
	va_list format_list;

	::va_start(format_list, format_str_ptr);
}

void CLogging::log_to_cons_stderr(const char *format_str_ptr, ...)
{
	va_list format_list;

	::va_start(format_list, format_str_ptr);
	do_common_log(2, format_str_ptr, format_list);
}

int CLogging::do_common_log(int log_fd, const char *format_str_ptr, va_list opt)
{
	int n_write = 0;
	int n_formated = 0;
	char log_str[MAX_SIZE_OF_LOG_STR];
	char log_last_str[MAX_SIZE_OF_LOG_STR];

#if 0
	if(m_tunable_do_lock) {
		if(log_fd >= 3) {
			if(sysutil_lock_file_write(log_fd) < 0) {
				return -1;
			}
		}
	}
#endif

	::memset(log_str, 0, MAX_SIZE_OF_LOG_STR);
	::memset(log_last_str, 0, MAX_SIZE_OF_LOG_STR);
	::vsnprintf(log_str, MAX_SIZE_OF_LOG_STR, format_str_ptr, opt);

	if(log_str[0] == '!') {
		n_formated = ::snprintf(log_last_str, MAX_SIZE_OF_LOG_STR, "[%s] : %s", sysutil_get_current_datetime(), (log_str + 1));
	} else {
		n_formated = ::snprintf(log_last_str, MAX_SIZE_OF_LOG_STR, "%s", log_str);
	}

	::va_end(opt);
	n_write = ::write(log_fd, log_last_str, n_formated);
#if 0
	if(m_tunable_do_lock) {
		if(log_fd >= 3) {
			if(sysutil_unlock_file(log_fd) < 0) {
				return -2;
			}
		}
	}
#endif

	return n_write;
}
}
