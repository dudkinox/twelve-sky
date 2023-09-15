/****************************************************************************

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: main.cpp,v 1.29 2007/10/18 21:00:50 wglee Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#include <iostream>
#include <string>
#include <cstdio>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/param.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>

#include "log.h"
#include "util.h"
#include "local.h"
#include "raslib.h"
#include "option.h"
#include "thread.h"
#include "rconfig.h"
#include "db_pool.h"
#include "pointer.h"
#include "cts_util.h"
#include "cts_signal.h"
#include "score_board.h"
#include "worker_thread.h"
#include "worker_process.h"

#include "win2unix.h"
#include "H01_MainApplication.h" 

using namespace std;
using namespace SDLog;
using namespace SDSocket;

#define MaxSizeOfBackLog    255
#define MaxSizeOfHint       2048
#define DBManagerTerm       1000

_g_env g_env;
Socket g_listen_socket;
CLogging *g_log_obj_ptr;
CScoreBoard *g_score_board_obj_ptr;
string g_config_str_obj;

SERVER_INFO mSERVER_INFO;
DWORD mBaseTickCountForLogic;
DWORD mPostTickCountForLogic;
BOOL mCheckLogicFlag;

void ShowVersion(void)
{
	cerr << SVR_NAME << " Version : " << SVR_VERSION << endl;
	::exit(1);
}

void ShowUsage(void)
{
	cerr << SVR_NAME << " Usage : " << endl;
	cerr << "------------------------------------------- " << endl;
	cerr << SVR_NAME << "  -[lL][pP][cC][v][h] " << endl;
	cerr << "         -[lL] : the prefix of a log-file " << endl;
	cerr << "         -[pP] : Port " << endl;
	cerr << "         -[cC] : a location of the configuration file " << endl;
	cerr << "         -[v]  : show the version of the " << SVR_NAME << endl;
	cerr << "         -[h]  : show this message " << endl;
	::exit(1);
}

#define DISPLAY_SIGNAL_ACTION_ERROR(signal) {\
	LOG_TO_BOTH_2("!sigaction : signal [ %s ] :: %s\n", signal, ::strerror(errno));\
}

int SetBeDmon(void)
{
	int cPID = 0;

#ifdef SIGTTOU
	::signal(SIGTTOU, SIG_IGN);
#endif
#ifdef SIGTTIN
	::signal(SIGTTIN, SIG_IGN);
#endif
#ifdef SIGTSTP
	::signal(SIGTSTP, SIG_IGN);
#endif
#ifdef SIGINT
	::signal(SIGINT, SIG_IGN);
#endif
#ifdef SIGSTP
	::signal(SIGSTP, SIG_IGN);
#endif
	
	if((cPID = ::fork()) > 0) {
		::exit(0);
	} else if(cPID == -1) {
		LOG_TO_BOTH("!Cannot make a child process for a daemonlizing.\n");
		::exit(1);
	}

#if 1 /* comate */
	if(::freopen("/dev/null", "r", stdin) == NULL) {
		return -1;
	}

	if(::freopen("/dev/null", "w", stdout) == NULL) {
		return -2;
	}

	if(::freopen("/dev/null", "w", stderr) == NULL) {
		LOG_TO_BOTH ("4\n");
		return -3;
	}
#endif

#ifdef __FreeBSD__
	if(setpgrp(0, getpid()) < 0)
#elif defined(__LINUX__)
	if(setpgrp() < 0)
#else
	if(setpgrp(0, 0) < 0)
#endif
	{
		LOG_TO_FILE_1("!SetBeDmon::setpgrp: %s\n", ::strerror(errno));
		return -4;
	}

	return 1;
}

#define MAX_SIZE_OF_PROC_TITLE  64
static void CallSubMain(int port_num)
{
#ifndef __DEBUG_NO_FORK__
	static unsigned int is_just_started = 1;
	static unsigned int ex_tno_worker_proc = 0;
#endif

#ifdef __RELEASE__
#endif
	init_signals(CHILD);
	try {
		g_listen_socket.open(Socket::TCP);
		OptionTemplate<SOL_SOCKET, SO_REUSEADDR, int> ReuseAddr(1);
		g_listen_socket.setsockopt(ReuseAddr);
		Address SvrAddr(Address::ANY_IP, (in_port_t)port_num, false);
		g_listen_socket.bind(SvrAddr);
		g_listen_socket.listen(MaxSizeOfBackLog);
		LOG_TO_FILE("!The Listening Socket has been initialized successfully.\n");

		LOG_TO_FILE("!Initializing the Worker Processes... \n");
		while(g_score_board_obj_ptr->is_spinning()) {
			pid_t new_pid = 0;
			unsigned int current_svc_process = 0;
			current_svc_process = g_score_board_obj_ptr->maintenance_child_proc(g_env.tno_proc_maintenance_time);

			if((ex_tno_worker_proc != current_svc_process) || (is_just_started)) {
				is_just_started = 0;
#ifndef __LINUX__
				::setproctitle("Manager: %d TSVC & %d BSVC workers are in service", current_svc_process, current_bsvc_process);
#endif
				ex_tno_worker_proc = current_svc_process;
			}

			if(current_svc_process < (unsigned int)g_env.tno_worker_process) {
				if((new_pid = fork()) < 0) {
					g_score_board_obj_ptr->let_cm_stop(CScoreBoard::SHUTDOWN_IMMIDIATELY);
					LOG_TO_FILE_3("![%d] Making a TSVC worker process has failed :: %s (error no = %d)\n", ::getpid(), ::strerror(errno), errno);
					::exit(1);
				} else if(new_pid == 0) {
					int ret_val = 0;

#ifndef __LINUX__
					::setproctitle("TSVC Worker: in service");
#endif
					LOG_TO_FILE_4("![%d] Initializing %s Worker Process:  [ %d / %d ]\n", ::getpid(), SVR_NAME, current_svc_process + 1, g_env.tno_worker_process);

					any_auto_ptr<CWorkerProcess> worker_svc_process_ptr(new CWorkerProcess(g_listen_socket.GetDescriptor()));
					ret_val = worker_svc_process_ptr->start_svr_service();
					if(ret_val < 0) {
						LOG_TO_FILE_2("![%d] The %s service is terminated by error(s)\n", ::getpid(), SVR_NAME);
						::exit(1);
					} 
				}
				g_score_board_obj_ptr->update_slot(new_pid);
			} 
			::sleep(1);
		}
	} catch(Exception &e) {
		g_score_board_obj_ptr->let_cm_stop(CScoreBoard::SHUTDOWN_IMMIDIATELY);
		LOG_TO_FILE_3("!%s: %s (error code = %d) \n", e.getMethod().c_str(), e.getMessage().c_str(), e.getCode());
	}
}


#define DISPLAY_CONFIG_ERROR(CONFIG_KEY) {\
	LOG_TO_BOTH_1("!the configuration file has syntax error : %s is NULL.\n", CONFIG_KEY);\
	LOG_TO_BOTH("!Cannot proceed futher procdure.. exit..");\
	::exit(1);\
}

#define CHECK_CONFIG_VARIABLE(read_var, config_key) {\
	if(read_var != NULL) {\
		if(!::strlen(read_var)) {\
			DISPLAY_CONFIG_ERROR(config_key);\
		}\
	} else {\
		LOG_TO_BOTH_1("!The Value of %s is empty.\n", config_key);\
	}\
}

#define CHECK_CONFIG_VAR_RANGE(var, config_key, from, to) {\
	if((::atoi(var) < from) && (::atoi(var) > to)) {\
		LOG_TO_BOTH_3("!The Value of %s is not valid. : valid range is from %d to %d \n", config_key, from, to);\
		::exit(1);\
	}\
}

#define CHECK_CONFIG_VAR_BOOLEAN(var, config_key) {\
	if((_strcmp_ac(var, "YES")) && (_strcmp_ac(var, "NO"))) {\
		LOG_TO_BOTH_1("!The Value of %s is not valid. : please, specify YES or NO.\n", config_key);\
		::exit(1);\
	}\
}

static void load_config(void) 
{
	int ret_val = 0;
	char svr_name_str[MAX_SIZE_OF_CONFIG_ENTRY];
	char to_read_str[MAX_SIZE_OF_CONFIG_ENTRY];
    char listen_port_str[MAX_SIZE_OF_CONFIG_ENTRY];
	char tcp_nodelay_str[MAX_SIZE_OF_CONFIG_ENTRY];
	char sock_sndbuf_str[MAX_SIZE_OF_CONFIG_ENTRY];
	char sock_rcvbuf_str[MAX_SIZE_OF_CONFIG_ENTRY];
    char tno_worker_process_str[MAX_SIZE_OF_CONFIG_ENTRY];
#ifdef __USE_DB_POOL__
    char db_name_str[MAX_SIZE_OF_CONFIG_ENTRY];
    char db_acct_str[MAX_SIZE_OF_CONFIG_ENTRY];
    char db_host_addr_str[MAX_SIZE_OF_CONFIG_ENTRY];
    char db_host_port_str[MAX_SIZE_OF_CONFIG_ENTRY];
    char db_acct_passwd_str[MAX_SIZE_OF_CONFIG_ENTRY];
    char db_start_up_conn_str[MAX_SIZE_OF_CONFIG_ENTRY];
    char db_min_spare_conn_str[MAX_SIZE_OF_CONFIG_ENTRY];
    char db_max_spare_conn_str[MAX_SIZE_OF_CONFIG_ENTRY];
	char db_max_allowed_conn_str[MAX_SIZE_OF_CONFIG_ENTRY];
	char db_maintenance_time_str[MAX_SIZE_OF_CONFIG_ENTRY];
#endif
    char proc_maintenance_rc_str[MAX_SIZE_OF_CONFIG_ENTRY];
    char proc_maintenance_time_str[MAX_SIZE_OF_CONFIG_ENTRY];
	char tno_startup_worker_thread_str[MAX_SIZE_OF_CONFIG_ENTRY];
	char tno_min_spare_worker_thread_str[MAX_SIZE_OF_CONFIG_ENTRY];
	char tno_max_spare_worker_thread_str[MAX_SIZE_OF_CONFIG_ENTRY];
	char tno_max_allowed_worker_thread_str[MAX_SIZE_OF_CONFIG_ENTRY];
    char temporary_spare_thread_timeout_str[MAX_SIZE_OF_CONFIG_ENTRY];

	/*****************************************************************/
	char world_num_str[MAX_SIZE_OF_CONFIG_ENTRY];
	char db_name_str[MAX_SIZE_OF_CONFIG_ENTRY];
	char db_acct_str[MAX_SIZE_OF_CONFIG_ENTRY];
	char db_host_addr_str[2][MAX_SIZE_OF_CONFIG_ENTRY];
	char db_host_port_str[MAX_SIZE_OF_CONFIG_ENTRY];
	char db_acct_passwd_str[MAX_SIZE_OF_CONFIG_ENTRY];
	//Account DB
	char db2_name_str[MAX_SIZE_OF_CONFIG_ENTRY];
	char db2_acct_str[MAX_SIZE_OF_CONFIG_ENTRY];
	char db2_host_addr_str[2][MAX_SIZE_OF_CONFIG_ENTRY];
	char db2_host_port_str[MAX_SIZE_OF_CONFIG_ENTRY];
	char db2_acct_passwd_str[MAX_SIZE_OF_CONFIG_ENTRY];
	//---------
	char gamelog_ip_addr_str[MAX_SIZE_OF_CONFIG_ENTRY];
	char gamelog_port_str[MAX_SIZE_OF_CONFIG_ENTRY];
	
	Config *config_obj_ptr = new Config(g_config_str_obj.c_str());
	if(!config_obj_ptr) {
		LOG_TO_BOTH("!Initializing the configuration object failed. : out of memory\n");
		::exit(1);
	}

	if(config_obj_ptr->LoadConfig() < 0) {
		LOG_TO_BOTH("!Loading the configuration file to memory has failed.\n");
		LOG_TO_BOTH("!SVR cannot proceed futher procedure.. exit..\n");
		::exit(1);
	}

	ret_val = config_obj_ptr->GetValue(CONFIG_KEY_SVR_NAME, svr_name_str);
	ret_val += config_obj_ptr->GetValue(CONFIG_KEY_TNO_WORKER_PROCESS, tno_worker_process_str);
	ret_val += config_obj_ptr->GetValue(CONFIG_KEY_TNO_WORKER_PROCESS, tno_worker_process_str);
	ret_val += config_obj_ptr->GetValue(CONFIG_KEY_TNO_STARTUP_WORKER_THREAD, tno_startup_worker_thread_str);
	ret_val += config_obj_ptr->GetValue(CONFIG_KEY_TNO_MIN_SPARE_WORKER_THREAD, tno_min_spare_worker_thread_str);
	ret_val += config_obj_ptr->GetValue(CONFIG_KEY_TNO_MAX_SPARE_WORKER_THREAD, tno_max_spare_worker_thread_str);
	ret_val += config_obj_ptr->GetValue(CONFIG_KEY_TNO_MAX_ALLOWED_WORKER_THREAD, tno_max_allowed_worker_thread_str);
    ret_val += config_obj_ptr->GetValue(CONFIG_KEY_TO_READ, to_read_str);
	ret_val += config_obj_ptr->GetValue(CONFIG_KEY_LISTEN_PORT, listen_port_str);
	ret_val += config_obj_ptr->GetValue(CONFIG_KEY_TCP_NODELAY, tcp_nodelay_str);
	ret_val += config_obj_ptr->GetValue(CONFIG_KEY_SOCK_SNDBUF, sock_sndbuf_str);
	ret_val += config_obj_ptr->GetValue(CONFIG_KEY_SOCK_RCVBUF, sock_rcvbuf_str);
	ret_val += config_obj_ptr->GetValue(CONFIG_KEY_PROC_MAINTENANCE_TIME, proc_maintenance_time_str);
	ret_val += config_obj_ptr->GetValue(CONFIG_KEY_PROC_MAINTENANCE_REPLY_COUNT, proc_maintenance_rc_str);
    ret_val += config_obj_ptr->GetValue(CONFIG_KEY_TEMPORARY_SPARE_THREAD_TIMEOUT, temporary_spare_thread_timeout_str);
	/*****************************************************************/
    ret_val += config_obj_ptr->GetValue(CONFIG_KEY_WORLD_NUM, world_num_str);
    ret_val += config_obj_ptr->GetValue(CONFIG_KEY_DATABASE_NAME, db_name_str);
    ret_val += config_obj_ptr->GetValue(CONFIG_KEY_DATABASE_ACCT, db_acct_str);
    ret_val += config_obj_ptr->GetValue(CONFIG_KEY_DATABASE_PW, db_acct_passwd_str);
    ret_val += config_obj_ptr->GetValue(CONFIG_KEY_DATABASE_HOST_ADDR, db_host_addr_str[0]);
    ret_val += config_obj_ptr->GetValue(CONFIG_KEY_DATABASE_HOST_ADDR2, db_host_addr_str[1]);
    ret_val += config_obj_ptr->GetValue(CONFIG_KEY_DATABASE_HOST_PORT, db_host_port_str);
	//Account DB
	ret_val += config_obj_ptr->GetValue(CONFIG_KEY_DATABASE2_NAME, db2_name_str);
    ret_val += config_obj_ptr->GetValue(CONFIG_KEY_DATABASE2_ACCT, db2_acct_str);
    ret_val += config_obj_ptr->GetValue(CONFIG_KEY_DATABASE2_PW, db2_acct_passwd_str);
    ret_val += config_obj_ptr->GetValue(CONFIG_KEY_DATABASE2_HOST_ADDR, db2_host_addr_str[0]);
    ret_val += config_obj_ptr->GetValue(CONFIG_KEY_DATABASE2_HOST_ADDR2, db2_host_addr_str[1]);
    ret_val += config_obj_ptr->GetValue(CONFIG_KEY_DATABASE2_HOST_PORT, db2_host_port_str);
	//---------
    ret_val += config_obj_ptr->GetValue(CONFIG_KEY_SVR_GAMELOG_IP_ADDR, gamelog_ip_addr_str);
    ret_val += config_obj_ptr->GetValue(CONFIG_KEY_SVR_GAMELOG_PORT, gamelog_port_str);
	/*****************************************************************/
    
#ifdef __USE_DB_POOL__
	ret_val += config_obj_ptr->GetValue(CONFIG_KEY_DATABASE_NAME, db_name_str);
	ret_val += config_obj_ptr->GetValue(CONFIG_KEY_DATABASE_ACCT, db_acct_str);
	ret_val += config_obj_ptr->GetValue(CONFIG_KEY_DATABASE_PW, db_acct_passwd_str);
	ret_val += config_obj_ptr->GetValue(CONFIG_KEY_DATABASE_HOST_ADDR, db_host_addr_str);
	ret_val += config_obj_ptr->GetValue(CONFIG_KEY_DATABASE_HOST_PORT, db_host_port_str);
	ret_val += config_obj_ptr->GetValue(CONFIG_KEY_DATABASE_START_UP_CONN, db_start_up_conn_str);
	ret_val += config_obj_ptr->GetValue(CONFIG_KEY_DATABASE_MIN_SPARE_CONN, db_min_spare_conn_str);
	ret_val += config_obj_ptr->GetValue(CONFIG_KEY_DATABASE_MAX_SPARE_CONN, db_max_spare_conn_str);
	ret_val += config_obj_ptr->GetValue(CONFIG_KEY_DATABASE_MAX_ALLOWED_CONN, db_max_allowed_conn_str);
	ret_val += config_obj_ptr->GetValue(CONFIG_KEY_DATABASE_MAINTENANCE_TIME, db_maintenance_time_str);
	if (ret_val != 43) {
#else
	if (ret_val != 30) {
#endif
		LOG_TO_BOTH_2("!Insuffient configuration value. see %s file (code = %d) \n", DEFAULT_CONFIG_FILE, ret_val);
		::exit(1);
	}

	CHECK_CONFIG_VARIABLE(svr_name_str,                              CONFIG_KEY_SVR_NAME);
	CHECK_CONFIG_VARIABLE(tno_worker_process_str,                    CONFIG_KEY_TNO_WORKER_PROCESS);
	CHECK_CONFIG_VARIABLE(to_read_str,                               CONFIG_KEY_TO_READ);
	CHECK_CONFIG_VARIABLE(listen_port_str,                           CONFIG_KEY_LISTEN_PORT);
	CHECK_CONFIG_VARIABLE(tcp_nodelay_str,                           CONFIG_KEY_TCP_NODELAY);
	CHECK_CONFIG_VARIABLE(sock_sndbuf_str,                           CONFIG_KEY_SOCK_SNDBUF);
	CHECK_CONFIG_VARIABLE(sock_rcvbuf_str,                           CONFIG_KEY_SOCK_RCVBUF);
	CHECK_CONFIG_VARIABLE(proc_maintenance_time_str,                 CONFIG_KEY_PROC_MAINTENANCE_TIME);
	CHECK_CONFIG_VARIABLE(proc_maintenance_rc_str,                   CONFIG_KEY_PROC_MAINTENANCE_REPLY_COUNT);
    CHECK_CONFIG_VARIABLE(tno_startup_worker_thread_str,             CONFIG_KEY_TNO_STARTUP_WORKER_THREAD);
    CHECK_CONFIG_VARIABLE(tno_min_spare_worker_thread_str,           CONFIG_KEY_TNO_MIN_SPARE_WORKER_THREAD);
    CHECK_CONFIG_VARIABLE(tno_max_spare_worker_thread_str,           CONFIG_KEY_TNO_MAX_SPARE_WORKER_THREAD);
    CHECK_CONFIG_VARIABLE(tno_max_allowed_worker_thread_str,         CONFIG_KEY_TNO_MAX_ALLOWED_WORKER_THREAD);
#ifdef __USE_DB_POOL__
	CHECK_CONFIG_VARIABLE(db_acct_str,                               CONFIG_KEY_DATABASE_ACCT);
	CHECK_CONFIG_VARIABLE(db_acct_passwd_str,                        CONFIG_KEY_DATABASE_PW);
	CHECK_CONFIG_VARIABLE(db_host_addr_str,                          CONFIG_KEY_DATABASE_HOST_ADDR);
	CHECK_CONFIG_VARIABLE(db_host_port_str,                          CONFIG_KEY_DATABASE_HOST_PORT);
	CHECK_CONFIG_VARIABLE(db_name_str,                               CONFIG_KEY_DATABASE_NAME);
	CHECK_CONFIG_VARIABLE(db_min_spare_conn_str,                     CONFIG_KEY_DATABASE_MIN_SPARE_CONN);
	CHECK_CONFIG_VARIABLE(db_max_spare_conn_str,                     CONFIG_KEY_DATABASE_MAX_SPARE_CONN);
	CHECK_CONFIG_VARIABLE(db_max_allowed_conn_str,                   CONFIG_KEY_DATABASE_MAX_ALLOWED_CONN);
	CHECK_CONFIG_VARIABLE(db_maintenance_time_str,                   CONFIG_KEY_DATABASE_MAINTENANCE_TIME);
	CHECK_CONFIG_VARIABLE(db_start_up_conn_str,                      CONFIG_KEY_DATABASE_START_UP_CONN);
#endif

    /*****************************************************************/
    CHECK_CONFIG_VARIABLE(world_num_str,                             CONFIG_KEY_WORLD_NUM);
    CHECK_CONFIG_VARIABLE(db_acct_str,                               CONFIG_KEY_DATABASE_ACCT);
    CHECK_CONFIG_VARIABLE(db_acct_passwd_str,                        CONFIG_KEY_DATABASE_PW);
    CHECK_CONFIG_VARIABLE(db_host_addr_str[0],                       CONFIG_KEY_DATABASE_HOST_ADDR);
    CHECK_CONFIG_VARIABLE(db_host_addr_str[1],                       CONFIG_KEY_DATABASE_HOST_ADDR2);
    CHECK_CONFIG_VARIABLE(db_host_port_str,                          CONFIG_KEY_DATABASE_HOST_PORT);
    CHECK_CONFIG_VARIABLE(db_name_str,                               CONFIG_KEY_DATABASE_NAME);
	//Account DB
	CHECK_CONFIG_VARIABLE(db2_acct_str,                               CONFIG_KEY_DATABASE2_ACCT);
	CHECK_CONFIG_VARIABLE(db2_acct_passwd_str,                        CONFIG_KEY_DATABASE2_PW);
	CHECK_CONFIG_VARIABLE(db2_host_addr_str[0],                       CONFIG_KEY_DATABASE2_HOST_ADDR);
	CHECK_CONFIG_VARIABLE(db2_host_addr_str[1],                       CONFIG_KEY_DATABASE2_HOST_ADDR2);
	CHECK_CONFIG_VARIABLE(db2_host_port_str,                          CONFIG_KEY_DATABASE2_HOST_PORT);
	CHECK_CONFIG_VARIABLE(db2_name_str,                               CONFIG_KEY_DATABASE2_NAME);
	//----------
    CHECK_CONFIG_VARIABLE(gamelog_ip_addr_str,                       CONFIG_KEY_SVR_GAMELOG_IP_ADDR);
    CHECK_CONFIG_VARIABLE(gamelog_port_str,                          CONFIG_KEY_SVR_GAMELOG_PORT);
    /*****************************************************************/
    CHECK_CONFIG_VAR_RANGE(tno_worker_process_str,                   CONFIG_KEY_TNO_WORKER_PROCESS,              1,    200);
	CHECK_CONFIG_VAR_RANGE(tno_startup_worker_thread_str,            CONFIG_KEY_TNO_STARTUP_WORKER_THREAD ,      1,    250);
	CHECK_CONFIG_VAR_RANGE(to_read_str,                              CONFIG_KEY_TO_READ ,                        1,    360);
	CHECK_CONFIG_VAR_RANGE(listen_port_str,                          CONFIG_KEY_LISTEN_PORT,                     1,    65535);
	CHECK_CONFIG_VAR_RANGE(sock_sndbuf_str,                          CONFIG_KEY_SOCK_SNDBUF,                     512,  65535);
	CHECK_CONFIG_VAR_RANGE(sock_rcvbuf_str,                          CONFIG_KEY_SOCK_RCVBUF,                     512,  65535);
	CHECK_CONFIG_VAR_RANGE(proc_maintenance_time_str,                CONFIG_KEY_PROC_MAINTENANCE_TIME,           300,  6000);
	CHECK_CONFIG_VAR_RANGE(proc_maintenance_rc_str,                  CONFIG_KEY_PROC_MAINTENANCE_REPLY_COUNT,    1,    65535);
    CHECK_CONFIG_VAR_RANGE(tno_startup_worker_thread_str,            CONFIG_KEY_TNO_STARTUP_WORKER_THREAD,       1,    250);
    CHECK_CONFIG_VAR_RANGE(tno_min_spare_worker_thread_str,          CONFIG_KEY_TNO_MIN_SPARE_WORKER_THREAD,     1,    250);
    CHECK_CONFIG_VAR_RANGE(tno_max_spare_worker_thread_str,          CONFIG_KEY_TNO_MAX_SPARE_WORKER_THREAD,     1,    250);
    CHECK_CONFIG_VAR_RANGE(tno_max_allowed_worker_thread_str,        CONFIG_KEY_TNO_MAX_ALLOWED_WORKER_THREAD,   1,    250);
    CHECK_CONFIG_VAR_RANGE(temporary_spare_thread_timeout_str,       CONFIG_KEY_TEMPORARY_SPARE_THREAD_TIMEOUT,  1,    3600);
    
	CHECK_CONFIG_VAR_BOOLEAN(tcp_nodelay_str,                        CONFIG_KEY_TCP_NODELAY);
#ifdef __USE_DB_POOL__
	CHECK_CONFIG_VAR_RANGE(db_host_port_str,                         CONFIG_KEY_DATABASE_HOST_PORT,              1,    65535);
	CHECK_CONFIG_VAR_RANGE(db_min_spare_conn_str,                    CONFIG_KEY_DATABASE_MIN_SPARE_CONN,         1,    100);
	CHECK_CONFIG_VAR_RANGE(db_max_spare_conn_str,                    CONFIG_KEY_DATABASE_MAX_SPARE_CONN,         1,    200);
	CHECK_CONFIG_VAR_RANGE(db_max_allowed_conn_str,                  CONFIG_KEY_DATABASE_MAX_ALLOWED_CONN,       1,    3600);
	CHECK_CONFIG_VAR_RANGE(db_start_up_conn_str,                     CONFIG_KEY_DATABASE_START_UP_CONN,          1,    100);
	CHECK_CONFIG_VAR_RANGE(db_maintenance_time_str  ,                CONFIG_KEY_DATABASE_MAINTENANCE_TIME,       5,    6000);
#endif
    /*****************************************************************/
    CHECK_CONFIG_VAR_RANGE(world_num_str,                            CONFIG_KEY_WORLD_NUM,                       1,    999);
    CHECK_CONFIG_VAR_RANGE(db_host_port_str,                         CONFIG_KEY_DATABASE_HOST_PORT,              1,    65535);
	//Account DB
    CHECK_CONFIG_VAR_RANGE(db2_host_port_str,                         CONFIG_KEY_DATABASE2_HOST_PORT,              1,    65535);
	//--------
    CHECK_CONFIG_VAR_RANGE(gamelog_port_str,                             CONFIG_KEY_SVR_GAMELOG_PORT,                1,    65535);
    /*****************************************************************/

	g_env.svr_name_str_obj = svr_name_str;
	g_env.to_read = ::atoi(to_read_str);
	g_env.tno_worker_process = ::atoi(tno_worker_process_str);
	g_env.tno_startup_worker_thread = ::atoi(tno_startup_worker_thread_str);
	g_env.tno_proc_maintenance_rc = ::atoi(proc_maintenance_rc_str);
    g_env.tno_proc_maintenance_time = ::atoi(proc_maintenance_time_str);
    g_env.tno_startup_worker_thread = ::atoi(tno_startup_worker_thread_str);
    g_env.tno_min_spare_worker_thread = ::atoi(tno_min_spare_worker_thread_str);
    g_env.tno_max_spare_worker_thread = ::atoi(tno_max_spare_worker_thread_str);
    g_env.tno_max_allowed_worker_thread = ::atoi(tno_max_allowed_worker_thread_str);
    g_env.tno_tmp_spare_worker_thread_to = ::atoi(temporary_spare_thread_timeout_str);
    
	g_env.listen_port = ::atoi(listen_port_str);
	g_env.sock_sndbuf = ::atoi(sock_sndbuf_str);
	g_env.sock_rcvbuf = ::atoi(sock_rcvbuf_str);

	if(!_strcmp_ac(tcp_nodelay_str, "YES")) {
		g_env.tcp_nodelay = true;
	} else {
		g_env.tcp_nodelay = false;
	}

#ifdef __USE_DB_POOL__
	g_env.db_name_str_obj = db_name_str;
	g_env.db_acct_str_obj = db_acct_str;
	g_env.db_acct_passwd_str_obj = db_acct_passwd_str;
	g_env.db_host_addr_str_obj = db_host_addr_str;
	g_env.db_host_port = ::atoi(db_host_port_str);
    g_env.tno_db_start_up_conn = ::atoi(db_start_up_conn_str);
	g_env.tno_db_min_spare_conn = ::atoi(db_min_spare_conn_str);
	g_env.tno_db_max_spare_conn = ::atoi(db_max_spare_conn_str);
	g_env.tno_db_max_allowed_conn = ::atoi(db_max_allowed_conn_str);
	g_env.tno_db_maintenance_time = ::atoi(db_maintenance_time_str);
#endif

	/*****************************************************************/
    g_env.world_num = ::atoi(world_num_str);
    g_env.db_name_str_obj = db_name_str;
    g_env.db_acct_str_obj = db_acct_str;
    g_env.db_acct_passwd_str_obj = db_acct_passwd_str;
    g_env.db_host_addr_str_obj[0] = db_host_addr_str[0];
    g_env.db_host_addr_str_obj[1] = db_host_addr_str[1];
    g_env.db_host_port = ::atoi(db_host_port_str);
	//Account DB
	g_env.db2_name_str_obj = db2_name_str;
    g_env.db2_acct_str_obj = db2_acct_str;
    g_env.db2_acct_passwd_str_obj = db2_acct_passwd_str;
    g_env.db2_host_addr_str_obj[0] = db2_host_addr_str[0];
    g_env.db2_host_addr_str_obj[1] = db2_host_addr_str[1];
    g_env.db2_host_port = ::atoi(db2_host_port_str);
	//---------
    g_env.gamelog_addr_str_obj = gamelog_ip_addr_str;
    g_env.gamelog_port = ::atoi(gamelog_port_str);

    ::snprintf(mSERVER_INFO.mServerName, 100, "%s", svr_name_str);
    mSERVER_INFO.mServerPort = g_env.listen_port;
    mSERVER_INFO.mWorldNumber = g_env.world_num;
    ::snprintf(mSERVER_INFO.mUpperServer_1_IP, 16, "%s", g_env.gamelog_addr_str_obj.c_str());
    mSERVER_INFO.mUpperServer_1_Port = g_env.gamelog_port;

    ::snprintf(mSERVER_INFO.mDB_1_IP[0], 16, "%s", db_host_addr_str[0]);
    ::snprintf(mSERVER_INFO.mDB_1_IP[1], 16, "%s", db_host_addr_str[1]);
    mSERVER_INFO.mDB_1_PORT = g_env.db_host_port;
    ::snprintf(mSERVER_INFO.mDB_1_ID, 100, "%s", g_env.db_acct_str_obj.c_str());
    ::snprintf(mSERVER_INFO.mDB_1_PASSWORD, 100, "%s", g_env.db_acct_passwd_str_obj.c_str());
    ::snprintf (mSERVER_INFO.mDB_1_Table01, 100, "WorldInfo");
	::snprintf (mSERVER_INFO.mDB_1_Table02, 100, "TribeInfo");
	//Account DB
	::snprintf(mSERVER_INFO.mDB_2_IP[0], 16, "%s", db2_host_addr_str[0]);
	::snprintf(mSERVER_INFO.mDB_2_IP[1], 16, "%s", db2_host_addr_str[1]);
	mSERVER_INFO.mDB_2_PORT = g_env.db2_host_port;
	::snprintf(mSERVER_INFO.mDB_2_ID, 100, "%s", g_env.db2_acct_str_obj.c_str());
	::snprintf(mSERVER_INFO.mDB_2_PASSWORD, 100, "%s", g_env.db2_acct_passwd_str_obj.c_str());
	//---------
	/*****************************************************************/
}

#define check_opt_arg()  if(optarg == NULL) \
  cerr << "invaid parameter !! using default values...." << endl; break

int main(int argc, char **argv)
{
#ifdef __RELEASE__
	//int status = 0;
#endif
	int opt_arg_c;
	string log_file_path_str_obj;

	while((opt_arg_c = getopt(argc, argv, "vhf:l:")) != -1) {
		switch(opt_arg_c) {
			case 'l' :
				log_file_path_str_obj = optarg;
				break;
			case 'f' :
				g_config_str_obj = optarg;
				break;
			case 'v' :
				ShowVersion();
				break;
			case 'h' :
				ShowUsage();
				break;
			default :
				ShowUsage();
				break;
		}
	}

	g_log_obj_ptr = new CLogging;
	if(!g_log_obj_ptr) {
		LOG_TO_CONS2("!Initialization of log object has been failed :: out of memory\n");
		::exit(1);
	}
	if(!log_file_path_str_obj.length()) {
		g_log_obj_ptr->set_log_file_path(DEFAULT_LOGFILE_PATH, "a+");
	} else {
		g_log_obj_ptr->set_log_file_path(log_file_path_str_obj.c_str(), "a+");
	}

	LOG_TO_BOTH  ("!********************************************************\n");
	LOG_TO_BOTH_2("!                     %s VER. %s                         \n", SVR_NAME, SVR_VERSION); 
	LOG_TO_BOTH  ("!********************************************************\n");

	if(!g_config_str_obj.length()) {
		g_config_str_obj = DEFAULT_CONFIG_FILE;
		LOG_TO_BOTH_1("!Using Default Config File: %s\n", DEFAULT_CONFIG_FILE);
	} else {
		LOG_TO_BOTH_1("!Using Config File: %s\n", g_config_str_obj.c_str());
	}

	load_config();
	LOG_TO_BOTH_1("!The server name: %s\n", g_env.svr_name_str_obj.c_str());
	LOG_TO_BOTH_1("!The log files location: %s\n", DEFAULT_LOGFILE_PATH);
	LOG_TO_BOTH_1("!The number of worker processes: %d\n", g_env.tno_worker_process);
	LOG_TO_BOTH_1("!The number of startup worker threads: %d\n", g_env.tno_startup_worker_thread);
	LOG_TO_BOTH_1("!The number of minimum spare worker threads: %d\n", g_env.tno_min_spare_worker_thread);
	LOG_TO_BOTH_1("!The number of maximum spare worker threads: %d\n", g_env.tno_max_spare_worker_thread);
	LOG_TO_BOTH_1("!The number of maximum allowed worker threads: %d\n", g_env.tno_max_allowed_worker_thread);
	LOG_TO_BOTH_1("!The listen port number: %d\n", g_env.listen_port); 
	LOG_TO_BOTH_1("!The time out value of socket: %d\n", g_env.to_read); 
	LOG_TO_BOTH_1("!Use tcp nodelay flag on socket: %s\n", (g_env.tcp_nodelay)?"YES":"NO"); 
	LOG_TO_BOTH_1("!The size of socket send buffer: %d\n", g_env.sock_sndbuf); 
	LOG_TO_BOTH_1("!The size of socket receive buffer: %d\n", g_env.sock_rcvbuf);
#ifdef __USE_DB_POOL__
	LOG_TO_BOTH_1("!Database Addr: %s\n", g_env.db_host_addr_str_obj.c_str());
	LOG_TO_BOTH_1("!Database Port: %d\n", g_env.db_host_port);
	LOG_TO_BOTH_1("!Database Name: %s\n", g_env.db_name_str_obj.c_str());
	LOG_TO_BOTH_1("!Database Account: %s\n", g_env.db_acct_str_obj.c_str());
	LOG_TO_BOTH_1("!Database Password: %s\n",  g_env.db_acct_passwd_str_obj.c_str());
	LOG_TO_BOTH_1("!the number of start-up DB connections: %d\n", g_env.tno_db_start_up_conn);
	LOG_TO_BOTH_1("!the number of minimum spare DB connections: %d\n", g_env.tno_db_min_spare_conn);
	LOG_TO_BOTH_1("!the number of maximum spare DB connections: %d\n", g_env.tno_db_max_spare_conn);
	LOG_TO_BOTH_1("!the number of maximum allowed DB connections: %d\n", g_env.tno_db_max_allowed_conn);
	LOG_TO_BOTH_1("!Interval of DB connection maintenance: %d secs\n", g_env.tno_db_maintenance_time); 
#endif
	LOG_TO_BOTH_1("!Interval of worker process maintenance: %d secs\n", g_env.tno_proc_maintenance_time); 
    LOG_TO_BOTH_1("!Maximum response count per worker process: %d \n", g_env.tno_proc_maintenance_rc); 


	LOG_TO_BOTH  ("!********************************************************\n");
    LOG_TO_BOTH_1("!The server belongs to world: %s\n", g_env.svr_name_str_obj.c_str());
    LOG_TO_BOTH_1("!Database Addr: %s\n", mSERVER_INFO.mDB_1_IP[0]); 
    LOG_TO_BOTH_1("!Database Addr2: %s\n", mSERVER_INFO.mDB_1_IP[1]);
    LOG_TO_BOTH_1("!Database Port: %d\n", g_env.db_host_port);
    LOG_TO_BOTH_1("!Database Name: %s\n", g_env.db_name_str_obj.c_str());
    LOG_TO_BOTH_1("!Database Account: %s\n", g_env.db_acct_str_obj.c_str());
    LOG_TO_BOTH_1("!Database Password: %s\n",  g_env.db_acct_passwd_str_obj.c_str());
    LOG_TO_BOTH_1("!The gamelog server IP Addr: %s\n", g_env.gamelog_addr_str_obj.c_str());
    LOG_TO_BOTH_1("!The gamelog server Port: %d\n", g_env.gamelog_port);

	if(SetBeDmon() < 0) {
		LOG_TO_BOTH_1("!Error has occured during becoming a daemon: %s\n", ::strerror(errno));
		::exit(1);
	}

#ifndef __LINUX__
	::setproctitle("Parent: initializing");
#endif
	init_signals(PARENT);
	g_score_board_obj_ptr = new CScoreBoard;
	if(g_score_board_obj_ptr == NULL) {
		LOG_TO_FILE("!Cannot create the process scorebaord :: out of memory\n");
		LOG_TO_FILE("!Cannot proceed futher procedure.. Exit.\n"); 
		::exit(1);
	}
	g_score_board_obj_ptr->init_score_board();
	g_score_board_obj_ptr->init_slot((g_env.tno_worker_process + 2));
	LOG_TO_FILE("!The Process Score Board has been initialized successfully. \n");

	for( ; ; ) {
		int status = 0;
		pid_t new_pid = 0;
		if((new_pid = fork()) < 0) {
			LOG_TO_FILE_2("![%d] Fork :: %s\n", ::getpid(), ::strerror(errno));
			break;
		} else if(new_pid == 0) {
			int ret_val = 0;

			ret_val = create_pid_file();
			if(ret_val == -1) {
				LOG_TO_BOTH_3("!pid file is already exist. may be %s is already running ?(pid = %d)%d\n", SVR_NAME, (int)get_pid_file(), ::getpid());
				g_score_board_obj_ptr->let_cm_stop(CScoreBoard::SHUTDOWN_IMMIDIATELY);
				::exit(1);
			} else if(ret_val == -2) {
				LOG_TO_BOTH_1("!Could not create a pid file: %s\n", ::strerror(errno));
				g_score_board_obj_ptr->let_cm_stop(CScoreBoard::SHUTDOWN_IMMIDIATELY);
				::exit(1);
			}
			CallSubMain(g_env.listen_port);
		}

		::wait(&status);
		if(g_score_board_obj_ptr->is_spinning() == CScoreBoard::STOP) {
			if(g_score_board_obj_ptr->get_cm_work_status() == CScoreBoard::SHUTDOWN_RELOAD_MODULES) {
				g_score_board_obj_ptr->let_cm_spin();
			} else {
				break;
			}
		}
	}
    LOG_TO_BOTH_1("!The %s service has been stopped.\n", SVR_NAME);
	return 1;
}
