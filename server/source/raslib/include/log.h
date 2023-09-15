/****************************************************************************

   Ras library FreeBSD Version

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: log.h,v 1.6 2008/07/18 12:31:36 mirshead Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#ifndef __LOG_H__
#define __LOG_H__

#include <iostream>
#include <fstream>
#include <string>
#include <sys/file.h>
#include <semaphore.h>

using namespace std;
namespace SDLog
{
	enum e_log_lock {
		ENABLE_LOG_LOCK  = true,
		DISABLE_LOG_LOCK = false
	};

	class CLogging {
	public:
		CLogging(void);
		~CLogging(void) throw();
		bool is_initialized(void) { return m_init_flag; };
		void set_log_fd(const int log_fd) { m_log_fd = log_fd; }
		int  set_log_file_path(const string file_path_str_obj, const char* mode_str_cptr); 
		void set_log_do_lock_stat(const e_log_lock do_lock) { m_tunable_do_lock = do_lock; }
		bool get_log_do_lock_stat(void) { return m_tunable_do_lock; }

		int log_to_file(const char *format_str_ptr, ...);
		void log_to_cons_stdout(const char *format_str_ptr, ...);
		void log_to_cons_stderr(const char *format_str_ptr, ...);

	protected:
		int do_common_log(int log_fd, const char *format_str_ptr, va_list opt);

	private:
		int m_log_fd;
		bool m_init_flag;
		FILE* m_file_obj_ptr;
		bool m_tunable_do_lock;
		string m_file_path_str_obj;
		string m_file_open_mode_str_obj;
		sem_t sem_write;
	};
}

#endif
