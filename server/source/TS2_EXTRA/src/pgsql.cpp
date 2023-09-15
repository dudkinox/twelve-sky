/****************************************************************************

   CTSD : Common Transaction Service

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: pgsql.cpp,v 1.3 2007/08/02 17:47:16 sean Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#ifdef __USE_DB_POOL__
#ifdef __DBPOOL_USE_PGSQL__

#include <string.h>
#include "pgsql.h"

CPgSQL::CPgSQL() 
{
	initialized        = false;
	m_hostname_str_ptr = NULL;
	m_portnum_str_ptr  = NULL;
	m_dbname_str_ptr   = NULL;
	m_acct_str_ptr     = NULL;
	m_passwd_str_ptr   = NULL;
	m_ResultCode       = 0;
	m_PGconn           = NULL;
	m_pRes             = NULL;
}

CPgSQL::~CPgSQL()
{
	if(m_PGconn) {
		disconnect();
	}
	unset_db_conn_info();
}

inline void CPgSQL::unset_db_conn_info(void) 
{
	if(m_hostname_str_ptr) ::free(m_hostname_str_ptr);
	if(m_portnum_str_ptr) ::free(m_portnum_str_ptr);
	if(m_dbname_str_ptr) ::free(m_dbname_str_ptr);
	if(m_acct_str_ptr) ::free(m_acct_str_ptr);
	if(m_passwd_str_ptr) ::free(m_passwd_str_ptr);
}

int CPgSQL::set_db_conn_info(const char *hostname_str_const_ptr, int port_num, const char *db_name_str_const_ptr,
                                const char *acct_str_const_ptr, const char *passwd_str_const_ptr)
{
	char portnum_str[8];

	if(initialized) {
		if(m_PGconn) {
			disconnect();
		}
		unset_db_conn_info();
	} 

	::memset(portnum_str, 0, 8);
	if(!hostname_str_const_ptr || !db_name_str_const_ptr ||
	   !acct_str_const_ptr || !passwd_str_const_ptr ) {
		return -1;
	}

	::snprintf(portnum_str, 8, "%d", port_num);
	m_hostname_str_ptr = ::strdup(hostname_str_const_ptr);
	m_portnum_str_ptr = ::strdup(portnum_str);
	m_dbname_str_ptr = ::strdup(db_name_str_const_ptr);
	m_acct_str_ptr = ::strdup(acct_str_const_ptr);
	m_passwd_str_ptr = ::strdup(passwd_str_const_ptr);

	initialized = true;
	return 1;
}

PGconn* CPgSQL::connect(const char *hostname_str_const_ptr, int port_num, const char *db_name_str_const_ptr,
                        const char *acct_str_const_ptr, const char *passwd_str_const_ptr)
{
	if(set_db_conn_info(hostname_str_const_ptr, port_num, db_name_str_const_ptr,
                        acct_str_const_ptr, passwd_str_const_ptr) < 0) {
		return NULL;
	}

	return connect();
}

PGconn* CPgSQL::connect(string hostname_str_obj, int port_num, string db_name_str_obj,
                        string db_acct_str_obj,  string db_passwd_str_obj)
{
	if(set_db_conn_info(hostname_str_obj.c_str(), port_num, db_name_str_obj.c_str(),
                        db_acct_str_obj.c_str(), db_passwd_str_obj.c_str()) < 0) {
		return NULL;
	}

	return connect();
}

PGconn* CPgSQL::connect(void)
{
	if(!initialized) {
		return NULL;
	}
	m_PGconn = PQsetdbLogin(m_hostname_str_ptr, m_portnum_str_ptr, NULL, NULL,
	                        m_dbname_str_ptr, m_acct_str_ptr, m_passwd_str_ptr);
	if(PQstatus(m_PGconn) == CONNECTION_BAD) {
		return NULL;
	}
	return m_PGconn;
}

PGconn* CPgSQL::connect(const char *dbname_str_const_ptr)
{
	m_PGconn = PQsetdb(NULL, NULL, NULL, NULL, dbname_str_const_ptr);
	if(PQstatus(m_PGconn) == CONNECTION_BAD) return NULL;
	return m_PGconn;
}

void CPgSQL::disconnect(void)
{
	if(m_PGconn) {
		PQfinish(m_PGconn);
		m_PGconn = NULL;
	}
}

int CPgSQL::get_result(CFLAG flag)
{
	int fRet = 0;
	int Result;

	Result = PQresultStatus(m_pRes);
	switch(Result) {
		case  PGRES_EMPTY_QUERY :
			fRet = -1;
			break;
		case  PGRES_BAD_RESPONSE :
			fRet = -2;
			break;
		case  PGRES_NONFATAL_ERROR :
			fRet = -3;
			break;
		case  PGRES_FATAL_ERROR :
			fRet = -4;
			break;
		case  PGRES_TUPLES_OK :
			fRet = 1;
			break;
		case  PGRES_COMMAND_OK :
			fRet = 2;
			break;
	}
	if(fRet < 0) {
		m_ErrorMessage = PQresultErrorMessage(m_pRes);
	}
	m_ResultCode = Result;
	if(flag == CLEAR) {
		PQclear(m_pRes);
	}
	return fRet;
}

string &CPgSQL::get_error_msg(void)
{
	return m_ErrorMessage;
}

int CPgSQL::get_cmd_tuples(void)
{
	return (int)::atoi(PQcmdTuples(m_pRes));
}

int CPgSQL::get_num_tuples(void)
{
	return PQntuples(m_pRes);
}

int CPgSQL::get_num_fields(void)
{
	return PQnfields(m_pRes);
}

PGresult *CPgSQL::get_res(void)
{
	return m_pRes;
}

void CPgSQL::clear_res(void)
{
	PQclear(m_pRes);
}

char *CPgSQL::get_value(int tuple, int field)
{
	return PQgetvalue(m_pRes, tuple, field);
}

int CPgSQL::exec_query(char *query_str_ptr)
{
	return this->exec_query(query_str_ptr, NOT_CLEAR);
}

int CPgSQL::exec_query(string &query_str_ref)
{
	return this->exec_query((char *)query_str_ref.c_str(), NOT_CLEAR);
}

int CPgSQL::exec_query(char *query_str_ptr, CFLAG flag)
{
	if(m_PGconn == NULL) return -1;
	m_pRes = PQexec(m_PGconn, query_str_ptr);
	return get_result(flag);
}

int CPgSQL::escape_string(char *to, const char *from, size_t length)
{
	int retval = 0;

	//PQescapeStringConn(m_PGconn, to, from, length, &retval);
	PQescapeString(to, from, length);
	return retval;
}
#endif
#endif
