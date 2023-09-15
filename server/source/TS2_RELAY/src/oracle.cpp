/****************************************************************************
/

   CTSD : Common Transaction Service

   CopyRight(C) 2007 Solutionbox All Rights reserved.

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/


#include <string.h>
#include "global.h"
#include "cts_oracle.h"

#ifdef __USE_DB_POOL__
#ifdef __DBPOOL_USE_ORACLE__
//comate
void lib_log(const char* log_str)
{
	LOG_TO_FILE_1("%s\n", log_str);
}

COracle::COracle() 
{
	initialized        = false;
	m_dbname_str_ptr   = NULL;
	m_acct_str_ptr     = NULL;
	m_passwd_str_ptr   = NULL;
	m_con              = NULL;
//	log_to(lib_log); //comate
}

COracle::~COracle()
{
	if(m_con) {
		disconnect();
	}
	unset_db_conn_info();
}

inline void COracle::unset_db_conn_info(void) 
{
    //if(m_hostname_str_ptr) ::free(m_hostname_str_ptr);
	if(m_dbname_str_ptr) ::free(m_dbname_str_ptr);
	if(m_acct_str_ptr) ::free(m_acct_str_ptr);
	if(m_passwd_str_ptr) ::free(m_passwd_str_ptr);
}

int COracle::set_db_conn_info(const char *hostname_str_const_ptr, int port_num,
                              const char *db_name_str_const_ptr,
                              const char *acct_str_const_ptr, const char *passwd_str_const_ptr)
{
	if(initialized) {
		if(m_con) {
			disconnect();
		}
		unset_db_conn_info();
	} 

	if(!db_name_str_const_ptr || !acct_str_const_ptr || !passwd_str_const_ptr ) {
		LOG_TO_FILE_4("[%d:%x] COracle:set_db_conn_info Invalid Param[%s:%s]\n", 
					getpid(), pthread_self(), 
					acct_str_const_ptr, passwd_str_const_ptr);
		return -1;
	}

	m_dbname_str_ptr = ::strdup(db_name_str_const_ptr);
	m_acct_str_ptr = ::strdup(acct_str_const_ptr);
	m_passwd_str_ptr = ::strdup(passwd_str_const_ptr);

	initialized = true;

	return 1;
}

Connection* COracle::connect(const char *hostname_str_const_ptr, int port_num,
                      const char *db_name_str_const_ptr,
                      const char *acct_str_const_ptr, const char *passwd_str_const_ptr)
{
	if(set_db_conn_info((const char*)0x00, 0,
	                    db_name_str_const_ptr,
                        acct_str_const_ptr, passwd_str_const_ptr) < 0) {
		return (Connection*)NULL;
	}

	LOG_TO_FILE_2("[%d:%x] COracle:connect\n", getpid(), pthread_self());

	return connect();
}

Connection* COracle::connect(string hostname_str_obj, int port_num,
                      string db_name_str_obj,
                      string db_acct_str_obj,  string db_passwd_str_obj)
{
	if(set_db_conn_info((const char*)0x00, 0,
                       	db_name_str_obj.c_str(),
                        db_acct_str_obj.c_str(), db_passwd_str_obj.c_str()) < 0) {
		return (Connection*)NULL;
	}

	LOG_TO_FILE_2("[%d:%x] COracle:connect\n", getpid(), pthread_self());
	return connect();
}

Connection* COracle::connect(void)
{
	if(!initialized) {
		return NULL;
	}
    
	m_con = new Connection();
	if(!m_con) {
	    LOG_TO_FILE_2("[%d:%x] COracle:connect Failed to new()\n", getpid(), pthread_self());
		return NULL;
	}

    
    if(!m_con->connect(m_dbname_str_ptr, m_acct_str_ptr, m_passwd_str_ptr))
	{

	    LOG_TO_FILE_3("[%d:%x] COracle:connect Failed to conn(%s)\n", 
							getpid(), pthread_self(), m_con->error().c_str());
		delete m_con;
		m_con = NULL;

        return NULL;
    }
    
	LOG_TO_FILE_3("[%d:%x] COracle:connect success(%x)\n", 
					getpid(), pthread_self(), m_con);

	return m_con;
}

void COracle::disconnect(void)
{
	if(m_con) {
        m_con->disconnect();
		delete m_con;
		m_con = NULL;
	}
}


string COracle::get_error_msg(void)
{
	if(m_con)
		return m_con->error();
	else
		return string("??? no connection");
}

void COracle::clear_query(void)
{
	if(m_q) m_q->reset();
}

bool COracle::exec_query(char *query_str_ptr)
{
	m_q = m_con->query();
	if(!m_q) 
	{
		LOG_TO_FILE_2("[%d:%x] COracle:exec_query(Failed to get query)\n", 
					  getpid(), pthread_self());

		return false;
	}

	if(!m_q->execute(query_str_ptr))
	{
		LOG_TO_FILE_3("[%d:%x] COracle:exec_query(Failed to execute:%s)\n", 
					  getpid(), pthread_self(), get_error_msg().c_str());
		return false;	
	}

	return true;
}

bool COracle::exec_query(string &query_str_ref)
{
	m_q = m_con->query();
	if(!m_q) 
	{
		LOG_TO_FILE_2("[%d:%x] COracle:exec_query(Failed to get query)\n", 
					  getpid(), pthread_self());
		return false;
	}

	if(!m_q->execute((char *)query_str_ref.c_str()))
	{
		LOG_TO_FILE_3("[%d:%x] COracle:exec_query(Failed to execute:%s)\n", 
					  getpid(), pthread_self(), get_error_msg().c_str());
		return false;	
	}
	return true;
}

int  COracle::get_num_rows(void)
{
	if(m_q)
		return (int)m_q->rows();
	else
		return (int)0;
}
#endif
#endif
