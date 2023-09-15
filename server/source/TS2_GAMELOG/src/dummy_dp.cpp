/****************************************************************************

   CTSD : Common Transaction Service 

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: dummy_dp.cpp,v 1.2 2007/08/03 05:24:03 sean Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#include "dummy_dp.h"

void *CDummyDP::connect(const char *hostname_str_const_ptr, int port_num, const char *db_name_str_const_ptr,
                        const char *acct_str_const_ptr, const char *passwd_str_const_ptr)
{
    return NULL;
}

void *CDummyDP::connect(string hostname_str_obj, int port_num, string db_name_str_obj,
                        string db_acct_str_obj,  string db_passwd_str_obj)
{
    return NULL;
}

void *CDummyDP::connect(void)
{
    return NULL;
}

void CDummyDP::disconnect(void)
{
    return;
}
