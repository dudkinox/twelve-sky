/****************************************************************************

   CTSD : Common Transaction Service

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: pgsql.h,v 1.2 2007/08/02 17:47:16 sean Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#ifdef __USE_DB_POOL__
#ifdef __DBPOOL_USE_PGSQL__

#ifndef __PGSQL_H__
#define __PGSQL_H__

#include <iostream>
#include <string>
#include "libpq-fe.h"

#define MAX_SIZE_OF_DB_QUERY		1024*4

using namespace std;
class CPgSQL
{
public:
	enum CFLAG { CLEAR = 1, NOT_CLEAR };
	CPgSQL();
	~CPgSQL();
	void unset_db_conn_info(void);
	int set_db_conn_info(const char *hostname_str_const_ptr, int port_num, const char *db_name_str_const_ptr,
	                     const char *acct_str_const_ptr, const char *passwd_str_const_ptr);
	PGconn* connect(const char *hostname_str_const_ptr, int port_num, const char *db_name_str_const_ptr,
			         const char *acct_str_const_ptr, const char *passwd_str_const_ptr);
	PGconn* connect(string hostname_str_obj, int port_num, string db_name_str_obj,
	                string db_acct_str_obj,  string db_passwd_str_obj);
	PGconn *connect(void);
	PGconn *connect(const char *dbname_str_const_ptr);
	void disconnect(void);
	int get_result(CFLAG flag);
	PGconn *GetPgConn(void);
	PGresult *get_res(void);
	int get_cmd_tuples(void);
	int get_num_tuples(void);
	int get_num_fields(void);
	int get_result_code(void) { return m_ResultCode; }
	char *get_value(int tuple, int field);
	void clear_res(void);
	int exec_query(string &strQuery);
	int exec_query(char *pszQuery);
	int exec_query(char *pszQuery, CFLAG flag);
	int escape_string(char *to, const char *from, size_t length);
	string &get_error_msg(void);
private:
	bool initialized;
	char *m_hostname_str_ptr;
	char *m_portnum_str_ptr;
	char *m_dbname_str_ptr;
	char *m_acct_str_ptr;
	char *m_passwd_str_ptr;
	int m_ResultCode;

	PGconn *m_PGconn;
	PGresult *m_pRes;
	string m_ErrorMessage;
};

#endif

#endif // end of __DBPOOL_USE_PGSQL__
#endif // end of __USE_DB_POOL__