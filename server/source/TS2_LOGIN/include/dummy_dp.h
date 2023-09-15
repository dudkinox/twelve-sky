/****************************************************************************

   CTSD : Common Transaction Service

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: dummy_dp.h,v 1.1 2007/08/02 17:47:16 sean Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#ifndef __DUMMY_DP_H__
#define	__DUMMY_DP_H__

#include <iostream>
#include <string>

using namespace std;
class CDummyDP
{
public:
	enum CFLAG { CLEAR = 1, NOT_CLEAR };
	void unset_db_conn_info(void);
	int set_db_conn_info(const char *hostname_str_const_ptr, int port_num, const char *db_name_str_const_ptr,
                         const char *acct_str_const_ptr, const char *passwd_str_const_ptr) { return 0; }
	void *connect(const char *hostname_str_const_ptr, int port_num, const char *db_name_str_const_ptr,
            const char *acct_str_const_ptr, const char *passwd_str_const_ptr);
	void *connect(string hostname_str_obj, int port_num, string db_name_str_obj,
            string db_acct_str_obj,  string db_passwd_str_obj);
    void *connect(void);
    void disconnect(void);  
};

#endif	
