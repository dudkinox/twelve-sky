/****************************************************************************

   Ras library FreeBSD Version

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: tcpclient.cpp,v 1.1 2007/03/21 06:14:07 miracle Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#include "tcpclient.h"

namespace SDSocket
{
  void TCPClient::connect(const Address& addr) throw (Exception)
  {
    socket.open(Socket::TCP);

    socket.connect(addr);
    
    setConnected(true);
  }

  Address TCPClient::getOwnAddress() throw (Exception)
  {
    return socket.getsockname();
  }
}
