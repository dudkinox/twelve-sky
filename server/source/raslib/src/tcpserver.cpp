/****************************************************************************

   Ras library FreeBSD Version

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: tcpserver.cpp,v 1.1 2007/03/21 06:14:07 miracle Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#include <sys/time.h>

#include "tcpserver.h"

namespace SDSocket
{
  void TCPServer::start(Address::Port port, int queue) throw (Exception)
  {
    socket.open(Socket::TCP);

    socket.bind(Address(Address::ANY_IP, port, false));

    socket.listen(queue);
  }

  void TCPServer::stop() throw (Exception)
  {
    socket.close();
  }

  TCPConnection* TCPServer::getConnection(Address& addr) throw (Exception)
  {
    return new TCPConnection(socket.accept(addr));
  }

  Address TCPServer::getAddress() throw (Exception)
  {
    return socket.getsockname();
  }
}
