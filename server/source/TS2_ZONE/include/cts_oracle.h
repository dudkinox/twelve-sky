/****************************************************************************

   CTSD : Common Transaction Service

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: cts_oracle.h,v 1.3 2007/10/18 20:55:41 wglee Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#ifdef __USE_DB_POOL__
#ifdef __DBPOOL_USE_ORACLE__

#ifndef __CTS_ORACLE_H__
#define	__CTS_ORACLE_H__

#include <iostream>
#include <string>

extern "C" {
#include <oci.h>
}
#include "orapp.hh"

using namespace std;
using namespace ORAPP;

class COracle
{
public:
	enum CFLAG { CLEAR = 1, NOT_CLEAR };
	COracle();
	~COracle();

	void unset_db_conn_info(void);
	int  set_db_conn_info(const char *hostname_str_const_ptr, int port_num, 
						  const char *db_name_str_const_ptr,
	                      const char *acct_str_const_ptr, 
						  const char *passwd_str_const_ptr);

	Connection* connect(const char *hostname_str_const_ptr, int port_num,
	                    const char *db_name_str_const_ptr,
			            const char *acct_str_const_ptr, 
				        const char *passwd_str_const_ptr);
	Connection* connect(string hostname_str_obj, int port_num,
	                    string db_name_str_obj,
	                    string db_acct_str_obj,  
				        string db_passwd_str_obj);


	bool exec_query(string &strQuery);
	bool exec_query(char *pszQuery);

	void disconnect(void);
	void clear_query(void);

	int get_num_rows(void);

	string     get_error_msg(void);
	Connection *GetConnection(void){ return m_con;};
	Query      *GetQuery(void) {return m_q;};

protected:
	Connection* connect(void);

private:
	bool initialized;
	char *m_dbname_str_ptr;
	char *m_acct_str_ptr;
	char *m_passwd_str_ptr;

	Connection *m_con;
	Query      *m_q;
};

#endif	/* __CTS_ORACLE_H__ */
#endif
#endif
