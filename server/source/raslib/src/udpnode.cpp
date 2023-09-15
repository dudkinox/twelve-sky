/****************************************************************************

   Ras library FreeBSD Version

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: udpnode.cpp,v 1.1 2007/03/21 06:14:07 miracle Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#include <cstdio>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "udpnode.h"

namespace SDSocket
{
  void UDPNode::SetSockNonBlocking(void) throw (Exception)
  {
	int fcntlFlag = 0;
    fcntlFlag = fcntl(socket.GetDescriptor(), F_GETFL, 0);
    fcntlFlag = fcntlFlag | O_NONBLOCK | O_NDELAY;
    if(fcntl(socket.GetDescriptor(), F_SETFL, fcntlFlag) < 0)
    {
		throw Exception("UDPNode::NonBlockOpen", ::strerror(errno), -1); 
    }
  }
	
  void UDPNode::NonBlockOpen(Address::Port port) throw (Exception)
  {
	if(socket.isOpened() > 0) socket.close();
    socket.open(Socket::UDP);
    socket.bind(Address(Address::ANY_IP, port, false));
  }

  void UDPNode::open(Address::Port port) throw (Exception)
  {
	if(socket.isOpened() > 0) socket.close();
    socket.open(Socket::UDP);
    socket.bind(Address(Address::ANY_IP, port, false));
  }

  void UDPNode::openClient(Address::Port Port) throw (Exception)
  {
	if(socket.isOpened() > 0) socket.close();
    socket.open(Socket::UDP);
  }

  void UDPNode::openNonBlockClient(Address::Port Port) throw (Exception)
  {
	if(socket.isOpened() > 0) socket.close();
    socket.open(Socket::UDP);
	this->SetSockNonBlocking();
  }
	
  void UDPNode::close() throw (Exception)
  {
    socket.close();
  }

  int UDPNode::sendto(const Address& addr, const Buffer& data) throw (Exception)
  {
    return socket.sendto(addr, data, 0);
  }

  int UDPNode::sendto(const Address& addr, const void *pData, unsigned int Size) throw (Exception)
  {
    return socket.sendto(static_cast<const sockaddr_in>(addr), pData, Size, 0);
  }

  int UDPNode::receivefrom(Address& addr, Buffer& data) throw (Exception)
  {
    return socket.recvfrom(addr, data, 0);
  }
	
  int UDPNode::rxTimer(int sec) 
  {
    fd_set fdset;
    struct timeval tv;

    FD_ZERO(&fdset);
    FD_SET(socket.GetDescriptor(), &fdset);

    tv.tv_sec = sec;
    tv.tv_usec = 0;

    return (::select(socket.GetDescriptor() + 1, &fdset, NULL, NULL, &tv));
  }
   
  int UDPNode::receivefrom(Address& addr, char *pData, unsigned int Size) 
  {
    return socket.recvfrom(reinterpret_cast<sockaddr_in&>(addr), pData, Size, 0);	
  }

  Address UDPNode::getAddress() throw (Exception)
  {
    return socket.getsockname();
  }
}
