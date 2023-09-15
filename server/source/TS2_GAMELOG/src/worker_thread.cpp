/****************************************************************************

   CTSD : Common Transaction Service

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: worker_thread.cpp,v 1.26 2007/12/03 14:50:17 wglee Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#include "log.h"
#include "pointer.h"
#include "worker_thread.h"

#define MAX_SIZE_OF_THREAD_JOB_QUEUE 200

using namespace SDLog;
using namespace SDSocket;

#ifdef __USE_MPM_WORKER_WITH_QUEUE__
Semaphore CWorkerThread::thread_lock;
CWorkerThread::CWorkerThread(db_conn_pool_t *db_conn_pool_ptr) 
{
#ifndef __DEBUG_NO_THREAD__
	int ret_val = 0;
#endif
	is_initialized = false;
	if(!db_conn_pool_ptr) {
		throw Exception("Worker Thread", "Invalid agument is passed.", -1);
	}
	m_tpool_arg.db_conn_pool_ptr = db_conn_pool_ptr;

#ifndef __DEBUG_NO_THREAD__
	ret_val = tpool_init(&m_tpool, g_env.tno_worker_thread, MAX_SIZE_OF_THREAD_JOB_QUEUE, 1);
	if(ret_val < 0) {
		throw Exception("Worker Thread", "Intializing a thread pool has been failed.", -2);
	}
#endif
	is_initialized = true;
}

CWorkerThread::~CWorkerThread(void) throw ()
{
#ifndef __DEBUG_NO_THREAD__
	if(is_initialized) {
		tpool_destroy(m_tpool, 1);
	}
#endif
}

int CWorkerThread::start_service(Socket *client_socket_obj_ptr) 
{
	--thread_lock;
	m_tpool_arg.client_socket_obj_ptr = client_socket_obj_ptr;
	++thread_lock;

#ifndef __DEBUG_NO_THREAD__
	return tpool_add_work(m_tpool, tsvc_thread_context, &m_tpool_arg);
#else
	tsvc_thread_context(&m_tpool_arg);
	return 1;
#endif
}
#endif

/*****************************************************************************/

#define MAX_SIZE_OF_RTX_BUFFER   8196
#define MAX_SIZE_OF_RECV_BUFFER  1024

#define SEND_RESPONSE_ERROR(rsp_code) {\
	memset(&rsp_pkt, 0, sizeof(tsvc_rsp_header_t));\
	rsp_pkt.op_num_w = op_req_header_ptr->op_num_w;\
	rsp_pkt.op_seq_w = htons(OP_SEQ_RSP);\
	rsp_pkt.rsp_cnt_w = 0;\
	rsp_pkt.op_result_code_w = htonl(rsp_code);\
	get_rsp_desc(rsp_code, rsp_pkt.op_result_desc_str);\
	tpool_arg_ptr->client_socket_obj_ptr->send((char *)&rsp_pkt, sizeof(rsp_pkt));\
	break;\
}

static Semaphore thread_context_lock;
void svc_thread_context(void *arg_ptr)
{
	//int ret_val = 0;
	int n_read = 0;
	char recv_buffer[MAX_SIZE_OF_RECV_BUFFER];
	tpool_arg_t *tpool_arg_ptr = reinterpret_cast<tpool_arg_t *>(arg_ptr);
	TCPOption::NoDelay tcp_opt_nodelay(1);
	SocketOption::SNDBuf sock_opt_sndbuf(g_env.sock_sndbuf);
	SocketOption::RCVBuf sock_opt_rcvbuf(g_env.sock_rcvbuf);

	struct timeval tv;
	fd_set rfds;
	int descriptor = 0;
	int retry = 3;
	int ret = -1;

	try {
		if(g_env.tcp_nodelay) {
			tpool_arg_ptr->client_socket_obj_ptr->setsockopt(tcp_opt_nodelay);
		}
		tpool_arg_ptr->client_socket_obj_ptr->setsockopt(sock_opt_sndbuf);
		tpool_arg_ptr->client_socket_obj_ptr->setsockopt(sock_opt_rcvbuf);

		for( ; ;) {
			::memset(recv_buffer, 0, MAX_SIZE_OF_RECV_BUFFER);

#if 0
        	ret_val = tpool_arg_ptr->client_socket_obj_ptr->rtxTimer(g_env.to_read);
        	if(ret_val <= 0) break;
#else
			if(!tpool_arg_ptr->client_socket_obj_ptr->isOpened()) break;

			descriptor = tpool_arg_ptr->client_socket_obj_ptr->GetDescriptor();
			tv.tv_sec = g_env.to_read;
			tv.tv_usec = 0;

			FD_ZERO(&rfds);
			FD_SET(descriptor, &rfds);

			ret = select(descriptor+1, &rfds, NULL, NULL, &tv);

			if(ret == 0)
			{
				LOG_TO_FILE_3("![%d:%x] Timeout: Retry[%d]\n", getpid(), pthread_self(), retry); 
				if(--retry < 0) break;
				continue;
			}
			else if(ret < 0)
			{
				LOG_TO_FILE_1("!ERROR(select): Retry[%d]\n", retry); 
				break;
			}

			if(!FD_ISSET(descriptor, &rfds)) continue;

			retry = 3;
#endif

			n_read = tpool_arg_ptr->client_socket_obj_ptr->recv(recv_buffer, sizeof(MAX_SIZE_OF_RECV_BUFFER), 0);
            if(n_read <= 0)  break;
		}
	} catch (Exception e) {
		//fprintf(stderr, "EXCEPTION OF SERVICE %d \n", ::pthread_self());

		if(tpool_arg_ptr) {
			if(tpool_arg_ptr->client_socket_obj_ptr) {
				tpool_arg_ptr->client_socket_obj_ptr->close();
				delete tpool_arg_ptr->client_socket_obj_ptr;
				tpool_arg_ptr->client_socket_obj_ptr = NULL;
			}
#ifdef  __USE_USER_DATA__
            if(tpool_arg_ptr->user_data_ptr) {
				free(tpool_arg_ptr->user_data_ptr);
				tpool_arg_ptr->user_data_ptr = NULL;
			}
#endif // __USE_USER_DATA__
			delete tpool_arg_ptr;
			tpool_arg_ptr = NULL;
		}
		LOG_TO_BOTH_4("![%d:%d] Thread Pool Exception: %s (error code = %d) \n", ::getpid(), pthread_self(),
				e.getMessage().c_str(), e.getCode());
		return;
	}

#if 0
	if(ret_val < 0 ) {
		fprintf(stderr, "END OF SERVICE %d \n", ::pthread_self());
	}
#endif
	if(tpool_arg_ptr) {
		if(tpool_arg_ptr->client_socket_obj_ptr) {
			tpool_arg_ptr->client_socket_obj_ptr->close();
			delete tpool_arg_ptr->client_socket_obj_ptr;
			tpool_arg_ptr->client_socket_obj_ptr = NULL;
		}
#ifdef  __USE_USER_DATA__
        if(tpool_arg_ptr->user_data_ptr) {
            free(tpool_arg_ptr->user_data_ptr);
            tpool_arg_ptr->user_data_ptr = NULL;
        }   
#endif // __USE_USER_DATA__
		delete tpool_arg_ptr;
		tpool_arg_ptr = NULL;
	}
	return;
}
