/****************************************************************************

   CTSD : Common Transaction Service

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: worker_thread.h,v 1.12 2007/09/11 03:50:00 wglee Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/


#ifndef __WORKER_THREAD_H__
#define __WORKER_THREAD_H__

#include <socket.h>
#include <sys/types.h>
#include <pthread.h>

#include "log.h"
#include "local.h"
#ifdef __USE_MPM_WORKER__
#include "tpool.h"
#elif __USE_MPM_WORKER_WITH_QUEUE__
#include "tpool_q.h"
#endif

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

#include "global.h"
#include "db_pool.h"
#include "semaphore.h"

using namespace std;
using namespace SDLog;
using namespace SDSocket;

typedef struct _s_tpool_arg {
	Socket *client_socket_obj_ptr;
	db_conn_pool_t *db_conn_pool_ptr;
#ifdef __USE_USER_DATA__
    void *user_data_ptr;
#endif //__USE_USER_DATA__
} tpool_arg_t;

class CWorkerThread 
{
public:
	CWorkerThread(db_conn_pool_t *db_conn_pool_ptr);
	~CWorkerThread(void) throw();
	int start_service(Socket *client_socket_obj_ptr);

private:
	tpool_t m_tpool;
	bool is_initialized;
	tpool_arg_t m_tpool_arg;
	static Semaphore thread_lock;
};

extern void svc_thread_context(void *arg_ptr);
#endif
