/****************************************************************************

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: local.h,v 1.9 2007/10/21 20:03:09 sean Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/


#ifndef __LOCAL_H__
#define __LOCAL_H__

#include <iostream>
#include <string>

#include "global.h"
#include "score_board.h"

#define DEFAULT_LOGFILE_PREFIX           SVR_NAME
#define DEFAULT_BUFFER_SIZE              9216 					
#define MAX_SIZE_OF_DB_QUERY		     1024*16

#ifdef __RELEASE__
#define DEFAULT_CONFIG_FILE              PROGPATH "/etc/" SVR_NAME ".conf"
#define DEFAULT_LOGFILE_PATH             PROGPATH "/logs/" SVR_NAME "/" SVR_NAME
#define DEFAULT_CORE_DUMP_PATH           PROGPATH "/logs/" SVR_NAME "/" 

#endif

#ifdef __DEBUG__
#undef DEFAULT_LOGFILE_PATH
#undef DEFAULT_CONFIG_FILE
#undef DEFAULT_CORE_DUMP_PATH
#define DEFAULT_LOGFILE_PATH             "./" SVR_NAME
#define DEFAULT_CONFIG_FILE              "./" SVR_NAME ".conf"
#define DEFAULT_CORE_DUMP_PATH           "./" 
#endif

#define DEFAULT_SVC_PORT                 13110
#define SAFTY_TIME_GAP                   500000

using namespace std;

typedef struct __g_env
{
	string svr_name_str_obj;
	int tno_worker_process;
	int tno_startup_worker_thread;
    int tno_min_spare_worker_thread;
    int tno_max_spare_worker_thread;
    int tno_max_allowed_worker_thread;
    int tno_tmp_spare_worker_thread_to;
	int to_read;
	int listen_port;
	bool tcp_nodelay;
	int sock_sndbuf;
	int sock_rcvbuf;
#ifdef __USE_DB_POOL__
	string db_acct_str_obj;
	string db_acct_passwd_str_obj;
	string db_name_str_obj;
	string db_host_addr_str_obj;
	int db_host_port;
	int tno_db_start_up_conn;
	int tno_db_min_spare_conn;
	int tno_db_max_spare_conn;
	int tno_db_max_allowed_conn;
	int tno_db_maintenance_time;
#endif
    int tno_proc_maintenance_rc;
    int tno_proc_maintenance_time;

	// game server configuration
	int island_num;
	int world_num;
	string contry_code_str_obj;
	/*
	int svr_num;
	int svr_max_user_num;
	string upper_svr1_addr_str_obj;
	int    upper_svr1_port;
	string upper_svr2_addr_str_obj;
	int    upper_svr2_port;
	string upper_svr3_addr_str_obj;
	int    upper_svr3_port;
	string upper_svr4_addr_str_obj;
	int    upper_svr4_port;
	string upper_svr5_addr_str_obj;
	int    upper_svr5_port;
	int    level_limit;
	*/
	string	db_1_addr_str_obj;
	int		db_1_port;
	string  db_1_name_str_obj;
	string	db_1_ID_str_obj;
	string	db_1_PW_str_obj;
	int		db_1_test_server;

} _g_env;

#define MAX_SIZE_OF_CONFIG_ENTRY         100
#define MAX_SIZE_OF_FILE_PATH            2048
#define MAX_SIZE_OF_VAL_KEY              64
#define MAX_SIZE_OF_VAL_VAL              512


// global variable ( it should be unique. )
extern _g_env g_env;

#define CONFIG_KEY_SVR_NAME                           "SVR_NAME"
#define CONFIG_KEY_TNO_WORKER_PROCESS                 "TNO_WORKER_PROCESS"
#define CONFIG_KEY_TNO_STARTUP_WORKER_THREAD          "TNO_STARTUP_WORKER_THREAD"
#define CONFIG_KEY_TNO_MIN_SPARE_WORKER_THREAD        "TNO_MIN_SPARE_WORKER_THREAD"
#define CONFIG_KEY_TNO_MAX_SPARE_WORKER_THREAD        "TNO_MAX_SPARE_WORKER_THREAD"
#define CONFIG_KEY_TNO_MAX_ALLOWED_WORKER_THREAD      "TNO_MAX_ALLOWED_WORKER_THREAD"
#define CONFIG_KEY_TO_READ                            "TIME_OUT_READ"
#define CONFIG_KEY_LISTEN_PORT                        "LISTEN_PORT"
#define CONFIG_KEY_TCP_NODELAY                        "TCP_NODELAY"
#define CONFIG_KEY_SOCK_SNDBUF                        "SOCK_SNDBUF"
#define CONFIG_KEY_SOCK_RCVBUF                        "SOCK_RCVBUF"
#define CONFIG_KEY_DATABASE_ACCT					  "DATABASE_ACCT"
#define CONFIG_KEY_DATABASE_PW						  "DATABASE_PW"
#define CONFIG_KEY_DATABASE_HOST_ADDR                 "DATABASE_HOST_ADDR"
#define CONFIG_KEY_DATABASE_HOST_PORT                 "DATABASE_HOST_PORT"
#define CONFIG_KEY_DATABASE_NAME                      "DATABASE_NAME"
#define CONFIG_KEY_DATABASE_START_UP_CONN             "DATABASE_START_UP_CONN"
#define CONFIG_KEY_DATABASE_MAX_ALLOWED_CONN          "DATABASE_MAX_ALLOWED_CONN"
#define CONFIG_KEY_DATABASE_MIN_SPARE_CONN            "DATABASE_MIN_SPARE_CONN"
#define CONFIG_KEY_DATABASE_MAX_SPARE_CONN            "DATABASE_MAX_SPARE_CONN"
#define CONFIG_KEY_DATABASE_MAINTENANCE_TIME          "DATABASE_MAINTENANCE_TIME"
#define CONFIG_KEY_PROC_MAINTENANCE_TIME              "WORKER_PROCESS_MAINTENANCE_TIME"
#define CONFIG_KEY_PROC_MAINTENANCE_REPLY_COUNT       "WORKER_PROCESS_MAINTENANCE_REPLY_COUNT"
#define CONFIG_KEY_TEMPORARY_SPARE_THREAD_TIMEOUT     "TEMPORARY_SPARE_THREAD_TIMEOUT"

/*****************************************************************************
 * game server configuration 
 ****************************************************************************/

//#define CONFIG_KEY_SVR_MAX_USER_NUM                   "SERVER_MAX_USER_NUM"
#define CONFIG_KEY_ISLAND_NUM                         "ISLAND_NUM"
#define CONFIG_KEY_WORLD_NUM                          "WORLD_NUM"
#define CONFIG_KEY_CONTRY_CODE                        "CONTRY_CODE"
/*
#define CONFIG_KEY_SVR_NUM                            "SERVER_NUM"
#define CONFIG_KEY_SVR_UPPER_SVR1_IP_ADDR             "PLAYER_IP"
#define CONFIG_KEY_SVR_UPPER_SVR1_PORT                "PLAYER_PORT"
#define CONFIG_KEY_SVR_UPPER_SVR2_IP_ADDR             "CENTER_IP"
#define CONFIG_KEY_SVR_UPPER_SVR2_PORT                "CENTER_PORT"
#define CONFIG_KEY_SVR_UPPER_SVR3_IP_ADDR             "RELAY_IP"
#define CONFIG_KEY_SVR_UPPER_SVR3_PORT                "RELAY_PORT"
#define CONFIG_KEY_SVR_UPPER_SVR4_IP_ADDR             "EXTRA_IP"
#define CONFIG_KEY_SVR_UPPER_SVR4_PORT                "EXTRA_PORT"
#define CONFIG_KEY_SVR_UPPER_SVR5_IP_ADDR             "LOG_IP"
#define CONFIG_KEY_SVR_UPPER_SVR5_PORT                "LOG_PORT"
#define CONFIG_KEY_LEVEL_LIMIT                        "LEVEL_LIMIT"
#define config_key_EXTRA_IP                           "EXTRA_IP"
*/
#define CONFIG_KEY_CRM_DB_IP						"CRM_DB_IP"
#define CONFIG_KEY_DB_PORT							"DB_PORT"
#define CONFIG_KEY_DB_ID							"DB_ID"
#define CONFIG_KEY_DB_NAME							"DB_NAME"
#define	CONFIG_KEY_DB_PASSWORD						"DB_PASSWORD"
#define CONFIG_KEY_TEST_SERVER						"TEST_SERVER"
#endif
