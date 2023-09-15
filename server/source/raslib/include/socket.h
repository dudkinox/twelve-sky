/****************************************************************************

  Ras library FreeBSD Version

  CopyRight(C) 2003 Sean Kim All Rights reserved.
Author : Sean Kim (sean@0x31.com)

$Id: socket.h,v 1.4 2007/06/21 16:46:15 sean Exp $

Redistribution and use in source and binary forms, with or with out
modification, are not permitted without author's permission.

 ****************************************************************************/

#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <sys/socket.h>
#include <string>
#include <vector>

#include "address.h"
#include "option.h"
#include "exception.h"
#include "buffer.h"
#include "semaphore.h"

typedef int SOCKET;
namespace SDSocket
{
	using std::string;

	class Socket
	{
	public:
		/// Errorcodes in Socket (witout LibC-errors)
		enum ErrorCode
		{
			NOT_OPENED_CODE = 0x0001,
			OPENED_CODE     = 0x0002
		};

		/// Supported Protocols
		enum Protocol
		{
			TCP = SOCK_STREAM,   ///< connection-oriented
			UDP = SOCK_DGRAM     ///< connectionless
		};

		enum Flags
		{
			OOB       = MSG_OOB,        ///< Out-Of-Band transmit
			DONTROUTE = MSG_DONTROUTE,  ///< Do not send packets over a router but directly
			DONTWAIT  = MSG_DONTWAIT,   ///< Use non-blocking mode
#ifdef MSG_NOSIGNAL
			NOSIGNAL  = MSG_NOSIGNAL,   ///< Receive no signal when connected host cancels connection.
#endif		
#ifdef MSG_CONFIRM
			CONFIRM   = MSG_CONFIRM,    ///< Linux 2.3+
#endif
			PEEK      = MSG_PEEK,       ///< Do not clear receive buffer
			WAITALL   = MSG_WAITALL,    ///< Wait until all data is received
			TRUNC     = MSG_TRUNC       ///< Return real packet size
		};

		/// How to shutdown the connection.
		enum Shutdown
		{
			READ       = SHUT_RD,       ///< No more data can be read.
			WRITE      = SHUT_WR,       ///< No more data can be written.
			READ_WRITE = SHUT_RDWR      ///< No data transport in both directions.
		};

		typedef int Descriptor;
		Socket(void); 
		Socket(Descriptor sfd);
		void SetDescriptor(Descriptor sfd) throw() { descriptor = sfd; }
		int GetDescriptor() throw() { return descriptor; }
		explicit Socket(Protocol protocol) throw (Exception);
		~Socket() throw();
		void open(Protocol protocol) throw (Exception);
		void close() throw (Exception);
		operator Descriptor() throw () { return descriptor; }
		operator SOCKET() throw () { return descriptor; }
		bool isOpened() throw () { return descriptor > 0; }
		void bind(const sockaddr_in& addr) throw (Exception);
		void bind(const Address& addr) throw (Exception);
		void connect(const sockaddr_in& addr) throw (Exception);
		int  connect(const sockaddr_in& addr, int Option);
		void connect(const Address& addr) throw (Exception);
		int  connect(const Address& addr, int Option);
		int sendto(const sockaddr_in& addr, const char* data, size_t size, int flags = 0) throw (Exception);
		int sendto(const sockaddr_in& addr, const void* data, size_t size, int flags = 0) throw (Exception);
		int sendto(const sockaddr_in* addr, const char* data, size_t size, int flags = 0) throw (Exception);
		int recv(char* data, size_t size, int flags = 0) throw (Exception);
		int recvfrom(sockaddr_in& addr, char* data, size_t size, int flags = 0) throw (Exception);
		int send(const char* data, size_t size, int flags = 0) throw (Exception);
		int send(const Buffer& data, int flags = 0) throw (Exception);
		int sendto(const Address& addr, const Buffer& data, int flags = 0) throw (Exception);
		int sendto(const Address& addr, const char *pData, int flags = 0) throw (Exception);
		int recv(Buffer& data, int flags = 0) throw (Exception);
		int recvfrom(Address& addr, Buffer& data, int flags = 0) throw (Exception);
		void listen(int queuelen) throw (Exception);
		Descriptor accept(sockaddr_in& addr) throw (Exception);
		Descriptor accept(Address& addr) throw (Exception);
		Address getsockname() throw (Exception);
		Address getpeername() throw (Exception);
		int getsockopt(Option& option) throw (Exception);
		void setsockopt(const Option& option) throw (Exception);
		void setsocktimeout(int Option, int toTime) throw (Exception);
		int rtxTimer(int sec) throw (Exception);
		int shutdown(Shutdown how) throw (Exception);
		bool wait(bool& read, bool& write, bool& exception, int seconds = 0, int useconds = 0) throw (Exception);
		bool setnonblock(void) throw (Exception); 

	private:
		/// filedescriptor; if >= 0 it is valid; else not opened
		void init_socket_lock(void);
		Descriptor descriptor;
	};
}

#endif
