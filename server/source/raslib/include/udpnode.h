/****************************************************************************

   Ras library FreeBSD Version

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: udpnode.h,v 1.1 2007/03/21 06:14:07 miracle Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#ifndef __UDPNODE_H__
#define __UDPNODE_H__

#include "socket.h"
#include "bytebuffer.h"

namespace SDSocket
{
  class UDPNode 
  {
  public:
    void open(Address::Port port = 0) throw (Exception);
	void NonBlockOpen(Address::Port port) throw (Exception);
	void openClient(Address::Port Port) throw (Exception);
	void openNonBlockClient(Address::Port Port) throw (Exception);
    void close() throw (Exception);
    int sendto(const Address& addr, const Buffer& data) throw (Exception);
	int sendto(const Address& addr, const void *pData, unsigned int Size) throw (Exception);
    int receivefrom(Address& addr, Buffer& data) throw (Exception);
	int receivefrom(Address& addr, char *pData, unsigned int Size);
	int rxTimer(int sec);
    Address getAddress() throw (Exception);
	int getSockFD(void) throw () { return socket.GetDescriptor(); }

  protected:
	void SetSockNonBlocking(void) throw (Exception);
    Socket socket;
  };
}

#endif
