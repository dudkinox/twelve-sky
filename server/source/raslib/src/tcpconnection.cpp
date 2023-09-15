/****************************************************************************

   Ras library FreeBSD Version

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: tcpconnection.cpp,v 1.1 2007/03/21 06:14:07 miracle Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#include <iostream>

#include "tcpconnection.h"
#include "stringbuffer.h"

namespace SDSocket
{
  TCPConnection::TCPConnection(Socket::Descriptor descriptor) throw()
    : socket(descriptor)
    , connected(true)
  {}

  TCPConnection::TCPConnection() throw()
    : connected(false)
  {}

  void TCPConnection::disconnect() throw (Exception)
  {
    socket.close();
    
    connected = false;
  }

  bool TCPConnection::isConnected() throw ()
  {
    return connected;
  }
 
  void TCPConnection::setConnected(bool _connected) throw()
  {
    connected = _connected;
  }
  
  int TCPConnection::send(const char* data, size_t size) 
  {
    return socket.send(data, size, flags);
  }

  int TCPConnection::send(const Buffer& data) throw (Exception)
  {
    return socket.send(data, flags);
  }

  int TCPConnection::receive(Buffer& data) throw (Exception)
  {
    return socket.recv(data, flags);
  }

  int TCPConnection::receive(char *data, int size) throw (Exception)
  {
    return socket.recv(data, size, flags);
  }

  TCPConnection& TCPConnection::operator<<(const string& text) throw (Exception)
  {
    StringBuffer buffer = text;

    send(buffer);

    return *this;
  }

  void TCPConnection::operator>>(string& text) throw (Exception)
  {
    StringBuffer buffer;

    receive(buffer);

    text = buffer;
  }

  Address TCPConnection::getConnectedAddress() throw (Exception)
  {
    return socket.getpeername();
  }

  void TCPConnection::setFlags(Socket::Flags _flags) throw()
  {
    flags = _flags;
  }
}
