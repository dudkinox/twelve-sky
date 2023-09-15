/****************************************************************************
 *
 * CTSD : Common Transaction Service
 *
 * CopyRight(C) 2003 Sean Kim All Rights reserved.
 * Author : Sean Kim (sean@0x31.com)
 *
 * $Id: worker_process.cpp,v 1.29 2007/10/20 08:41:13 wglee Exp $
 *
 * Redistribution and use in source and binary forms, with or with out
 * modification, are not permitted without author's permission.
 *
 ****************************************************************************/

#include "log.h"
#include "local.h"
#include "global.h"
#include "socket.h"
#include "pointer.h"
#include "worker_thread.h"
#include "worker_process.h"
#include "cts_util.h"
#include "H01_MainApplication.h"

using namespace SDLog;
using namespace SDSocket;

extern CScoreBoard *g_score_board_obj_ptr;
CWorkerProcess::CWorkerProcess(void) 
{
    m_pid = ::getpid();
}

CWorkerProcess::CWorkerProcess(SOCKET listen_socket) throw(Exception) 
{
    set_listen_socket(listen_socket);
    m_pid = ::getpid();
}

CWorkerProcess::~CWorkerProcess(void) 
{
}

void CWorkerProcess::set_listen_socket(SOCKET listen_socket) throw(Exception) 
{
    if(listen_socket < 0) {
        throw Exception(msg_str_obj.c_str(), "Invalid socket is passed.", -1);
    }
    
    m_listen_socket_obj.SetDescriptor(listen_socket);
}
#if 0
db_conn_pool_t* populate_db_pool(db_conn_pool_t::db_info_t db_info)
{

	db_conn_pool_t *db_conn_pool_ptr = NULL;
	db_conn_pool_t::CDBConnPoolManager *db_conn_pool_manager_ptr = NULL;
    
#ifdef __DEBUG_DB_POOL__
    DEBUG1("CWorkerProcess::populate_db_pool:db_info.m_user_id_str = %s\n", db_info.m_user_id_str);
    DEBUG1("CWorkerProcess::populate_db_pool:db_info.m_user_passwd_str = %s\n", db_info.m_user_passwd_str);
    DEBUG1("CWorkerProcess::populate_db_pool:db_info.m_db_name_str = %s\n", db_info.m_db_name_str);
    DEBUG1("CWorkerProcess::populate_db_pool:db_info.m_db_host_addr_str = %s\n", db_info.m_db_host_addr_str);
    DEBUG1("CWorkerProcess::populate_db_pool:db_info.m_db_host_port = %d\n", db_info.m_db_host_port);
    DEBUG1("CWorkerProcess::populate_db_pool:db_info.m_d_start_up_conn = %d\n", db_info.m_d_start_up_conn);
    DEBUG1("CWorkerProcess::populate_db_pool:db_info.m_d_min_spare_conn = %d\n", db_info.m_d_min_spare_conn);
    DEBUG1("CWorkerProcess::populate_db_pool:db_info.m_d_max_spare_conn = %d\n", db_info.m_d_max_spare_conn);
    DEBUG1("CWorkerProcess::populate_db_pool:db_info.m_d_max_allowed_conn = %d\n", db_info.m_d_max_allowed_conn);
#endif

    try {
        LOG_TO_FILE_1("![%d] Initializing the DB Conn. Pool \n", ::getpid());
        db_conn_pool_ptr = new db_conn_pool_t;
        if(!db_conn_pool_ptr) {
			SHOW_OUT_OF_MEMORY_AND_EXIT();
        }

        db_conn_pool_ptr->set_db_info(db_info);
        if(db_conn_pool_ptr->init_pool() < 0) {
			SHOW_OUT_OF_MEMORY_AND_EXIT();
        } else {
			SHOW_SUCCESS();
        }
        LOG_TO_FILE_1("![%d] the DB Conn. Pool has been initialized successfully.\n", ::getpid());
        LOG_TO_FILE_1("![%d] Initializing the DB Conn. Pool Manager\n", ::getpid());
        db_conn_pool_manager_ptr = new db_conn_pool_t::CDBConnPoolManager(db_conn_pool_ptr);
        if(!db_conn_pool_manager_ptr) {
			SHOW_OUT_OF_MEMORY_AND_EXIT();
        }
        db_conn_pool_manager_ptr->set_mt_time(g_env.tno_db_maintenance_time);
        db_conn_pool_manager_ptr->start();
        LOG_TO_FILE_1("![%d] the DB Conn. pool manager has been initialized successfully\n", ::getpid());
		SHOW_SUCCESS();
    } catch (Exception e) {
        throw Exception(__FUNCTION__, e.getMessage(), e.getCode());
    }
	return db_conn_pool_ptr;
}
#endif
#if 0
void *CWorkerProcess::check_time(void *arg)
{
	int past =  GetTickCount();
	while(1) {
		struct timeval tv = {0, 5000};
		mBaseTickCountForLogic = GetTickCount();
		if( ( mBaseTickCountForLogic - mPostTickCountForLogic ) > 500 ) {
			mPostTickCountForLogic = mBaseTickCountForLogic;
			mCheckLogicFlag = TRUE;
		}
#ifdef __N_PROTECT__
		if( ( mBaseTickCountForLogic - past ) > 60000 ) {
			GGAuthUpdateTimer();
		}
#endif
		if( mCheckLogicFlag ) {
			mCheckLogicFlag = FALSE;
			mGAME.Logic( 0.5f );
		}
		select(0, NULL, NULL, NULL, &tv);
	}
}
#endif
#ifdef __NB__
int CWorkerProcess::set_nonblock(int cli_fd)
{
	int old_flag = 0;

	old_flag = fcntl(cli_fd, F_GETFL);
	return fcntl(cli_fd, F_SETFL, old_flag | O_NONBLOCK);
}
#endif

#define MAX_SIZE_OF_FD       1024
int CWorkerProcess::start_svr_service(void) 
{
	int i = 0;
	int n_read = 0;
	int max_num = 0;
    int ret_val = 0;
	pthread_t check_thd;
	socklen_t cli_len = 0;
	int client_sockfd = -1;
	struct sockaddr_in cli_addr;
#ifdef __SELECT__
	int n_ready = 0;
	int listen_fd = 0;
	fd_set read_fds, write_fds;
	int cli_fd[MAX_SIZE_OF_FD];
#elif __POLL__
	struct pollfd cli_fd[MAX_SIZE_OF_FD];
#endif
	//db_conn_pool_t *db_conn_pool_ptr = NULL;

    try {
#ifdef __USE_DB_POOL__
		db_conn_pool_t::db_info_t db_info;

		::memset((char *)&db_info, 0, sizeof(db_info));
		::snprintf(db_info.m_user_id_str, MAX_SIZE_OF_DB_ACCT, "%s", g_env.db_acct_str_obj.c_str());
		::snprintf(db_info.m_user_passwd_str, MAX_SIZE_OF_DB_PASSWD, "%s", g_env.db_acct_passwd_str_obj.c_str());
		::snprintf(db_info.m_db_name_str, MAX_SIZE_OF_DB_NAME, "%s", g_env.db_name_str_obj.c_str());
		::snprintf(db_info.m_db_host_addr_str, MAX_SIZE_OF_DB_HOST_ADDR, "%s", g_env.db_host_addr_str_obj.c_str());
		db_info.m_db_host_port = g_env.db_host_port;
		db_info.m_d_start_up_conn = g_env.tno_db_start_up_conn;
		db_info.m_d_min_spare_conn = g_env.tno_db_min_spare_conn;
		db_info.m_d_max_spare_conn = g_env.tno_db_max_spare_conn;
		db_info.m_d_max_allowed_conn = g_env.tno_db_max_allowed_conn;
		db_conn_pool_ptr = populate_db_pool(db_info);
#endif
		if(!mSERVER.Init(&ret_val)) {
			LOG_TO_FILE_2("![%s] : ret_val %d\n", __FUNCTION__, ret_val);
			throw Exception("CWorkerProcess::start_svr_service", "server object initialization failed", -1);
		}

        /*
		mBaseTickCountForLogic = GetTickCount();
		mPostTickCountForLogic = mBaseTickCountForLogic;
		mCheckLogicFlag = FALSE;
		
		
		if(pthread_create(&check_thd, NULL, CWorkerProcess::check_time, this) != 0) {
			throw Exception("CWorkerProcess::start_svr_service", "Check time thread initialization failed", -1);
		}
		pthread_detach(check_thd);
		*/

#ifdef __SELECT__
#if 0
		int max_num = 0;
		FD_ZERO(&read_fds);
		FD_ZERO(&write_fds);
		listen_fd = m_listen_socket_obj.GetDescriptor();
		for(i = 0; i < MAX_SIZE_OF_FD; i++) {
			cli_fd[i] = -1;
		}

		while(g_score_board_obj_ptr->is_spinning()) {
			struct timeval tv = {1, 0};
			LOG_TO_FILE("Main SELECT \n");
			FD_SET(listen_fd, &read_fds);
			if((n_ready = select(listen_fd + 1, &read_fds, &write_fds, NULL, &tv)) < 0) {
				LOG_TO_FILE_4("![%s] : %s(%d)-%d\n", __FUNCTION__, strerror(errno), errno, listen_fd);
				throw Exception("CWorkerProcess::start_svr_service", "server object initialization failed", -2);
				return FALSE;
			}
			if(FD_ISSET(listen_fd, &read_fds)) {
				cli_len = sizeof(cli_addr);
				client_sockfd = accept(listen_fd, (struct sockaddr *)&cli_addr, &cli_len);
				//LOG_TO_FILE_1("-------------------------------- accept : %d\n", client_sockfd);

				//old_flag = fcntl(client_sockfd, F_GETFL);
				//fcntl(client_sockfd, F_SETFL, old_flag | O_NONBLOCK);

				for(i = 0; i < MAX_SIZE_OF_FD; i++) {
					if(cli_fd[i] < 0) {
						cli_fd[i] = client_sockfd;
						old_flag = fcntl(cli_fd[i], F_GETFL);
						fcntl(cli_fd[i], F_SETFL, old_flag | O_NONBLOCK);
						//sprintf( mUSER[index01].mIP, "%s", inet_ntoa(cli_addr.sin_addr));
						//LOG_TO_FILE_1("muser ip :%s\n", inet_ntoa(cli_addr.sin_addr));
						ProcessResponse(WM_NETWORK_MESSAGE_1, cli_fd[i], FD_ACCEPT);
						break;
					}
				}
				//FD_SET(cli_fd[i], &read_fds);
				//FD_SET(cli_fd[i], &write_fds);

				if((i - 1) == MAX_SIZE_OF_FD) {
					LOG_TO_FILE_2("![%s] : %s\n", __FUNCTION__, "Too many client");
					close(cli_fd[i]);
					continue;
				}
			}

			for(i = 0; i < MAX_SIZE_OF_FD; i++) {
				if(cli_fd[i] < 0)
					continue;
				//LOG_TO_FILE_2("![%s] : cli_fd %d\n", __FUNCTION__, cli_fd[i]);
				FD_SET(cli_fd[i], &read_fds);
				if(FD_ISSET(cli_fd[i], &read_fds)) {
					ProcessResponse(WM_NETWORK_MESSAGE_1, cli_fd[i], FD_READ);
				} else if(FD_ISSET(cli_fd[i], &write_fds)) {
					ProcessResponse(WM_NETWORK_MESSAGE_1, cli_fd[i], FD_WRITE);
				}
			}
		}
#endif
#elif __POLL__
		cli_fd[0].fd = m_listen_socket_obj.GetDescriptor();
		cli_fd[0].events = POLLIN;
		for(i = 1; i < MAX_SIZE_OF_FD; i++) {
			cli_fd[i].fd = -1;
		}
		max_num = 0;
        //This While loop is Cause of LogFile
		while(g_score_board_obj_ptr->is_spinning()) {
			n_read = poll(cli_fd, max_num + i , 500);
			if(n_read == 0) {
				continue;
			} else if(n_read < 0) {
				LOG_TO_FILE_2("![%s] : %s\n", __FUNCTION__, strerror(errno));
				exit(0);
			}

			if(cli_fd[0].revents & (POLLIN)) 
			{
				ProcessResponse(WM_NETWORK_MESSAGE_1, &cli_fd[0].fd, FD_READ);
			}

#if 0
			//LOG_TO_FILE_4("![%s] : %d %d %d\n", __FUNCTION__, n_read, i, max_num);
			if(cli_fd[0].revents & POLLIN) 
			{
				cli_len = sizeof(cli_addr);
				client_sockfd = accept(cli_fd[0].fd, (struct sockaddr *)&cli_addr, &cli_len);
				if(client_sockfd < 0) {
					LOG_TO_FILE_2("![%s] : %s\n", __FUNCTION__, strerror(errno));
					continue;
				}
#ifdef __NB__
				set_nonblock(client_sockfd);
#endif
				for(i = 1; i < MAX_SIZE_OF_FD; i++) {
					if(cli_fd[i].fd < 0) {
						cli_fd[i].fd = client_sockfd;
						ProcessResponse(WM_NETWORK_MESSAGE_1, &cli_fd[i].fd, FD_ACCEPT, (void *)&cli_addr.sin_addr);
						break;
					}
				}

				if((i - 1) == MAX_SIZE_OF_FD) {
					LOG_TO_FILE_2("![%s] : %s\n", __FUNCTION__, "Too many client");
					continue;
				}

				cli_fd[i].events = POLLIN;// | POLLOUT;

				if(i > max_num)
					max_num = i;

				if(--n_read <= 0)
					continue;
			} 
			
			for(i = 1; i <= max_num; i++) {
				if(cli_fd[i].fd < 0) 
					continue;
				
				if(cli_fd[i].revents & (POLLIN)) 
				{
					ProcessResponse(WM_NETWORK_MESSAGE_1, &cli_fd[i].fd, FD_READ);
				} 
				else if(cli_fd[i].revents & (POLLOUT)) 
				{
					ProcessResponse(WM_NETWORK_MESSAGE_1, &cli_fd[i].fd, FD_WRITE);
				}/* else if(cli_fd[i].revents & (POLLHUP | POLLERR)) {
					cli_fd[i].fd = -1;
					ProcessResponse(WM_NETWORK_MESSAGE_1, &cli_fd[i].fd, FD_CLOSE);
				}
				*/
			}
#endif	
		}
#endif
	} catch (Exception e) {
		LOG_TO_FILE_3("![%d] %s (error code = %d) \n", m_pid, e.getMessage().c_str(), e.getCode());
		return -2;
	}
	return 1;
}
