/****************************************************************************

   CTSD : Common Transaction Service

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: worker_process.h,v 1.10 2007/10/18 22:18:04 wglee Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/


#include <iostream>

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

#include "poll.h"
#include "local.h"
#include "raslib.h"
#include "global.h"
#include "db_pool.h"
#include "exception.h"

using namespace std;
using namespace SDSocket;

extern db_conn_pool_t* populate_db_pool(db_conn_pool_t::db_info_t db_info);

class CWorkerProcess {
public:
	CWorkerProcess(void);
	CWorkerProcess(SOCKET listen_socket) throw(Exception);
	~CWorkerProcess(void);
	void set_listen_socket(SOCKET listen_socket) throw(Exception);
	int start_svr_service(void);

protected:
	int set_nodelay(int cli_fd);
#ifdef __NB__
	int set_nonblock(int cli_fd);
#endif

private:
	pid_t m_pid;
	string msg_str_obj;
	Socket m_listen_socket_obj;
};


