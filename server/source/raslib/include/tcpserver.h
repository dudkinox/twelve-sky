/****************************************************************************

   Ras library FreeBSD Version

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: tcpserver.h,v 1.1 2007/03/21 06:14:07 miracle Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include "tcpconnection.h"

namespace SDSocket
{
  class TCPServer
  {
  public:
    void start(Address::Port port = 0, int queue = 5) throw (Exception);
    void stop() throw (Exception);
    TCPConnection* getConnection(Address& addr) throw (Exception);
	SOCKET getServerSock(void) { return socket.GetDescriptor(); }

    // Returns server (own) address. Use when you started the server with port 0, to get the assigned number.
    Address getAddress() throw (Exception);

  protected:
    // The socket where the server listens for new connections.
    Socket socket;
  };
}

#endif
