/****************************************************************************

   Ras library FreeBSD Version

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: tcpconnection.h,v 1.1 2007/03/21 06:14:07 miracle Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__

#include "socket.h"

namespace SDSocket
{
  class TCPConnection
  {
  public:
    // Create a closed and uninitialzed connection.
    TCPConnection() throw();
    virtual ~TCPConnection() throw() {}
    
    // Close connection.
    virtual void disconnect() throw (Exception);
    virtual int send(const Buffer& datas) throw (Exception);
    virtual int send(const char* data, size_t size);
    virtual int receive(Buffer& data) throw (Exception);
	virtual int receive(char *data, int size) throw (Exception);
    virtual TCPConnection& operator<<(const string& text) throw (Exception);
    virtual void operator>>(string& text) throw (Exception);
    virtual Address getConnectedAddress() throw (Exception);
    // Returns true if the connection is open.
	virtual bool isConnected() throw();
	SOCKET getSocketDescriptor(void) { return socket.GetDescriptor(); }
	void setSocketDescriptor(SOCKET fd) { return socket.SetDescriptor(fd); }
	void setFlags(Socket::Flags flags) throw();
	int getFlags() throw() { return flags; }

  protected:
	Socket socket;
    void setConnected(bool _connected) throw();  
	friend class TCPServer;
    TCPConnection(Socket::Descriptor descriptor) throw();

  private:
	Socket::Flags flags;
    // Flag for connection status. Abstracts from Socket::isOpened()
    bool connected;
  };
}

#endif
