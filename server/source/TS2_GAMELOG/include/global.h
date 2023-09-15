/****************************************************************************

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: global.h,v 1.8 2007/09/11 03:50:00 wglee Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/


#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "log.h"
#ifdef __USE_DB_POOL__
#ifdef __DBPOOL_USE_PGSQL__
#include "pgsql.h"
#endif
#ifdef  __DBPOOL_USE_MYSQL__
#include "cts_mysql.h"
#endif
#ifdef  __DBPOOL_USE_ORACLE__
#include "cts_oracle.h"
#endif
#else 
#include "dummy_dp.h"
#endif

#include "db_pool.h"
#include "score_board.h"

using namespace std;
using namespace SDLog;

#ifdef __USE_DB_POOL__
#ifdef __DBPOOL_USE_PGSQL__
typedef CDBConnPool<CPgSQL> db_conn_pool_t;
#endif
#ifdef  __DBPOOL_USE_MYSQL__
typedef CDBConnPool<CMySQL> db_conn_pool_t;
#endif
#ifdef  __DBPOOL_USE_ORACLE__
typedef CDBConnPool<COracle> db_conn_pool_t;
#endif
#else 
typedef CDBConnPool<CDummyDP> db_conn_pool_t;
#endif

extern CLogging *g_log_obj_ptr;
extern CLogging *net_log_obj_ptr;
extern CLogging *netc_log_obj_ptr;
extern CLogging *gm_log_obj_ptr;

#define MAX_SIZE_OF_LINE_BUFFER       1024

#ifdef LOG
#undef LOG
#endif
#define LOG g_log_obj_ptr 
#define LOG_TO_FILE(format)                        LOG->log_to_file(format) 
#define LOG_TO_FILE_1(format, op1)                 LOG->log_to_file(format, op1)
#define LOG_TO_FILE_2(format, op1, op2)            LOG->log_to_file(format, op1, op2)
#define LOG_TO_FILE_3(format, op1, op2, op3)       LOG->log_to_file(format, op1, op2, op3)
#define LOG_TO_FILE_4(format, op1, op2, op3, op4)  LOG->log_to_file(format, op1, op2, op3, op4)

#define LOG_TO_CONS1(format)                       LOG->log_to_cons_stdout(format)
#define LOG_TO_CONS1_1(format, op1)                LOG->log_to_cons_stdout(format, op1)
#define LOG_TO_CONS1_2(format, op1, op2)           LOG->log_to_cons_stdout(format, op1, op2)
#define LOG_TO_CONS1_3(format, op1, op2, op3)      LOG->log_to_cons_stdout(format, op1, op2, op3)
#define LOG_TO_CONS1_4(format, op1, op2, op3, op4) LOG->log_to_cons_stdout(format, op1, op2, op3, op4)

#define LOG_TO_CONS2(format)  LOG->log_to_cons_stderr(format)
#define LOG_TO_CONS2_1(format, op1)                LOG->log_to_cons_stderr(format, op1)
#define LOG_TO_CONS2_2(format, op1, op2)           LOG->log_to_cons_stderr(format, op1, op2)
#define LOG_TO_CONS2_3(format, op1, op2, op3)      LOG->log_to_cons_stderr(format, op1, op2, op3)
#define LOG_TO_CONS2_4(format, op1, op2, op3, op4) LOG->log_to_cons_stderr(format, op1, op2, op3, op4)


#define LOG_TO_BOTH(format)  {\
	LOG_TO_FILE(format);\
	LOG_TO_CONS2(format);\
}

#define LOG_TO_BOTH_1(format, op1)  {\
	LOG_TO_FILE_1(format, op1);\
	LOG_TO_CONS2_1(format, op1);\
}

#define LOG_TO_BOTH_2(format, op1, op2)  {\
	LOG_TO_FILE_2(format, op1, op2);\
	LOG_TO_CONS2_2(format, op1, op2);\
}

#define LOG_TO_BOTH_3(format, op1, op2, op3)  {\
	LOG_TO_FILE_3(format, op1, op2, op3);\
	LOG_TO_CONS2_3(format, op1, op2, op3);\
}
#define LOG_TO_BOTH_4(format, op1, op2, op3, op4)  {\
	LOG_TO_FILE_4(format, op1, op2, op3, op4);\
	LOG_TO_CONS2_4(format, op1, op2, op3, op4);\
}

#define SETCOLOR_SUCCESS        "\033\1331;32m"
#define SETCOLOR_FAILURE        "\033\1331;31m"
#define SETCOLOR_WARNING        "\033\1331;33m"
#define SETCOLOR_NORMAL         "\033\1330;39m" 

//	LOG_TO_CONS2_2("%s[FAILED]%s\n", SETCOLOR_FAILURE, SETCOLOR_NORMAL);
#define SHOW_OUT_OF_MEMORY_AND_EXIT() {\
	LOG_TO_FILE("[FAILED]\n");\
}

#define SAFE_PROC_RUN_SPIN_WAIT(usec) {\
	struct timeval tv = { 0, usec};\
	::select(0, NULL, NULL, NULL, &tv);\
}

//LOG_TO_CONS2_2("%s[SUCCESS]%s\n", SETCOLOR_SUCCESS, SETCOLOR_NORMAL);
#define SHOW_SUCCESS() {\
	LOG_TO_FILE("[SUCCESS]\n");\
}

#define SVR_TRACE() {\
	LOG_TO_FILE_3("!%s::%s::%d\n",__FILE__, __FUNCTION__, __LINE__);\
}
#endif
