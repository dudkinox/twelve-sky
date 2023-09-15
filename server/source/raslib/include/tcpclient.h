/****************************************************************************

   Ras library FreeBSD Version

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: tcpclient.h,v 1.1 2007/03/21 06:14:07 miracle Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#ifndef __TCPCLIENT_H__
#define __TCPCLIENT_H__

#include "tcpconnection.h"

namespace SDSocket
{
  class TCPClient : public TCPConnection
  {
  public:
    virtual void connect(const Address& addr) throw (Exception);
    virtual Address getOwnAddress() throw (Exception);
  };
}

#endif
