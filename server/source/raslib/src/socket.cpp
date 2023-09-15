/****************************************************************************

  Ras library FreeBSD Version

  CopyRight(C) 2003 Sean Kim All Rights reserved.
Author : Sean Kim (sean@0x31.com)

$Id: socket.cpp,v 1.4 2007/06/11 12:45:24 sean Exp $

Redistribution and use in source and binary forms, with or with out
modification, are not permitted without author's permission.

 ****************************************************************************/

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "socket.h"

namespace SDSocket
{
	/// Error returncodes of the C API functions.
	namespace ErrorReturnValues
	{
		const int SOCKET_ERROR      = -1;
		const int BIND_ERROR        = -1;
		const int CONNECT_ERROR     = -1;
		const int SEND_ERROR        = -1;
		const int RECV_ERROR        = -1;
		const int LISTEN_ERROR      = -1;
		const int ACCEPT_ERROR      = -1;
		const int GETSOCKNAME_ERROR = -1;
		const int GETPEERNAME_ERROR = -1;
		const int GETSOCKOPT_ERROR  = -1;
		const int SETSOCKOPT_ERROR  = -1;
		const int SHUTDOWN_ERROR    = -1;
		const int FCNTL_ERROR       = -1;
	}

	namespace ErrorMessages
	{
		const string NOT_OPENED  = "Socket not opened yet";
		const string OPENED      = "Socket already opened";
	}

	/// Methodnames that can throw exceptions.
	namespace MethodNames
	{
		const string OPEN        = "Socket::open()";
		const string CLOSE       = "Socket::close()";
		const string BIND        = "Socket::bind()";
		const string CONNECT     = "Socket::connect()";
		const string SEND        = "Socket::send()";
		const string SENDTO      = "Socket::sendto()";
		const string RECV        = "Socket::recv()";
		const string RECVFROM    = "Socket::recvfrom()";
		const string LISTEN      = "Socket::listen()";
		const string ACCEPT      = "Socket::accept()";
		const string GETSOCKNAME = "Socket::getsockname()";
		const string GETPEERNAME = "Socket::getpeername()";
		const string GETSOCKOPT  = "Socket::getsockopt()";
		const string SETSOCKOPT  = "Socket::setsockopt()";
		const string SHUTDOWN    = "Socket::shutdown()";
		const string WAIT        = "Socket::wait()";
		const string RTXTIMER    = "Socket::rtxTimer()";
		const string FCNTL       = "Socket::fcntl()";
	}

	using namespace ErrorReturnValues;
	using namespace ErrorMessages;
	using namespace MethodNames;

	Socket::Socket(void) : descriptor(-1)
	{
	}

	Socket::Socket(int sfd) : descriptor(sfd)
	{
	}

	Socket::Socket(Protocol protocol) throw (Exception)
	{
		open(protocol);
	}

	Socket::~Socket() throw()
	{
#ifdef __DEBUG_SOCKET__
		fprintf(stderr, "SOCKET::SOCKET(desctructor)::descriptor = %d\n", descriptor);
#endif
		if(isOpened()) {
			::close(descriptor);
		}
	}

	void Socket::open(Protocol protocol) throw (Exception)
	{
		if(isOpened()) {
			throw Exception(OPEN, OPENED, OPENED_CODE);
		}

		descriptor = socket(PF_INET, protocol, 0);

		if(descriptor == SOCKET_ERROR) {
			throw LibCException(OPEN);
		}
	}

	void Socket::close() throw (Exception)
	{
		if(isOpened()) {
			::close(descriptor);
		}

		descriptor = -1;
	}

	void Socket::bind(const sockaddr_in& addr) throw (Exception)
	{
		int error;

		if(!isOpened()) {
			throw Exception(BIND, NOT_OPENED, NOT_OPENED_CODE);
		}

		const sockaddr* socketAddress = reinterpret_cast<const sockaddr*>(&addr);

		error = ::bind(descriptor, socketAddress, sizeof(addr));

		if(error == BIND_ERROR) {
			throw LibCException(BIND);
		}
	}

	void Socket::bind(const Address& addr) throw (Exception)
	{
		bind(static_cast<const sockaddr_in>(addr));
	}

	void Socket::connect(const sockaddr_in& addr) throw (Exception)
	{
		int error;

		if(!isOpened()) {
			throw Exception(CONNECT, NOT_OPENED, NOT_OPENED_CODE);
		}

		const sockaddr* socketAddress = reinterpret_cast<const sockaddr*>(&addr);

		error = ::connect(descriptor, socketAddress, sizeof(addr));

		if(error == CONNECT_ERROR) {
			throw LibCException(CONNECT);
		}
	}

	int Socket::connect(const sockaddr_in& addr, int Option) 
	{
		int error;

		if(!isOpened()) {
			throw Exception(CONNECT, NOT_OPENED, NOT_OPENED_CODE);
		}

		const sockaddr* socketAddress = reinterpret_cast<const sockaddr*>(&addr);

		error = ::connect(descriptor, socketAddress, sizeof(addr));

		if(error == CONNECT_ERROR) {
			return -1;
		}
		return 1;
	}

	void Socket::connect(const Address& addr) throw (Exception)
	{
		connect(static_cast<const sockaddr_in>(addr));
	}

	int Socket::connect(const Address& addr, int Option) 
	{
		return connect(static_cast<const sockaddr_in>(addr), Option);
	}

	int Socket::send(const char* data, size_t size, int flags) throw (Exception)
	{
		if(!isOpened()) {
			throw Exception(SEND, NOT_OPENED, NOT_OPENED_CODE);
		}

		return ::send(descriptor, data, size, flags);
	}

	int Socket::sendto(const sockaddr_in& addr, const char* data, size_t size, int flags) throw (Exception)
	{
		if(!isOpened()) {
			throw Exception(SENDTO, NOT_OPENED, NOT_OPENED_CODE);
		}

		const sockaddr* socketAddress = reinterpret_cast<const sockaddr*>(&addr);

		return ::sendto(descriptor, data, size, flags, socketAddress, sizeof(addr));
	}

	int Socket::sendto(const sockaddr_in *addr, const char* data, size_t size, int flags) throw (Exception)
	{
		if(!isOpened()) {
			throw Exception(SENDTO, NOT_OPENED, NOT_OPENED_CODE);
		}
		const sockaddr* socketAddress = reinterpret_cast<const sockaddr*>(addr);
		return ::sendto(descriptor, data, size, flags, socketAddress, sizeof(sockaddr_in));
	}

	int Socket::sendto(const sockaddr_in& addr, const void* data, size_t size, int flags) throw (Exception)
	{
		if(!isOpened()) {
			throw Exception(SENDTO, NOT_OPENED, NOT_OPENED_CODE);
		}

		const sockaddr* socketAddress = reinterpret_cast<const sockaddr*>(&addr);
		return ::sendto(descriptor, data, size, flags, socketAddress, sizeof(addr));
	}

	int Socket::recv(char* data, size_t size, int flags) throw (Exception)
	{
		if(!isOpened()) {
			throw Exception(RECV, NOT_OPENED, NOT_OPENED_CODE);
		}

		return ::recv(descriptor, data, size, flags);
	}

	int Socket::recvfrom(sockaddr_in& addr, char* data, size_t size, int flags) throw (Exception)
	{
		if(!isOpened()) {
			throw Exception(RECVFROM, NOT_OPENED, NOT_OPENED_CODE);
		}
		socklen_t addrLen = sizeof(addr);
		sockaddr* socketAddress = reinterpret_cast<sockaddr*>(&addr);

		return ::recvfrom(descriptor, data, size, flags, socketAddress, &addrLen);
	}

	int Socket::send(const Buffer& data, int flags) throw (Exception)
	{
		return send(data.pointer(), data.size(), flags);
	}

	int Socket::sendto(const Address& addr, const char *pData, int flags) throw (Exception)
	{
		return sendto(addr, pData, strlen(pData), flags);
	}

	int Socket::sendto(const Address& addr, const Buffer& data, int flags) throw (Exception)
	{
		return sendto(addr, data.pointer(), data.size(), flags);
	}

	int Socket::rtxTimer(int sec) throw (Exception)
	{
		fd_set fdsetRead;
		fd_set fdsetWrite;
		struct timeval tv;

		if(!isOpened())
		{
			throw Exception(RTXTIMER, NOT_OPENED, NOT_OPENED_CODE);
		}
		FD_ZERO(&fdsetRead);
		FD_ZERO(&fdsetWrite);
		FD_SET(descriptor, &fdsetRead);
		FD_SET(descriptor, &fdsetWrite);

		tv.tv_sec = sec;
		tv.tv_usec = 0;

		return (::select(descriptor + 1, &fdsetRead, &fdsetWrite, NULL, &tv));
	}

	int Socket::recv(Buffer& data, int flags) throw (Exception)
	{
		int bytes;

		data.prepare();

		bytes = recv(data.pointer(), data.size(), flags);

		data.update(bytes);

		return bytes;
	}

	int Socket::recvfrom(Address& addr, Buffer& data, int flags) throw (Exception)
	{
		int bytes;

		data.prepare();

		sockaddr_in address = addr;

		bytes = recvfrom(address, data.pointer(), data.size(), flags);

		addr = address;

		data.update(bytes);

		return bytes;
	}

	void Socket::listen(int queuelen) throw (Exception)
	{
		int error;

		if(!isOpened()) {
			throw Exception(LISTEN, NOT_OPENED, NOT_OPENED_CODE);
		}

		error = ::listen(descriptor, queuelen);

		if(error == LISTEN_ERROR) {
			throw LibCException(LISTEN);
		}
	}

	Socket::Descriptor Socket::accept(sockaddr_in& addr) throw (Exception)
	{
		Descriptor sfd;

		if(!isOpened()) {
			throw Exception(ACCEPT, NOT_OPENED, NOT_OPENED_CODE);
		}

		socklen_t addrLen = sizeof(addr);

		sockaddr* socketAddress = reinterpret_cast<sockaddr*>(&addr);

		sfd = ::accept(descriptor, socketAddress, &addrLen);

		if(sfd == ACCEPT_ERROR) {
			throw LibCException(ACCEPT);
		}

		return sfd;
	}

	Socket::Descriptor Socket::accept(Address& addr) throw (Exception)
	{
		Descriptor tempSocket;

		sockaddr_in address = addr;

		tempSocket = accept(address);

		addr = address;

		return tempSocket;
	}

	Address Socket::getsockname() throw (Exception)
	{
		int error;

		if(!isOpened()) {
			throw Exception(GETSOCKNAME, NOT_OPENED, NOT_OPENED_CODE);
		}

		sockaddr_in address;
		socklen_t addrLen = sizeof(address);

		sockaddr* socketAddress = reinterpret_cast<sockaddr*>(&address);

		error = ::getsockname(descriptor, socketAddress, &addrLen);

		if(error == GETSOCKNAME_ERROR) {
			throw LibCException(GETSOCKNAME);
		} else if(!strcmp(inet_ntoa(address.sin_addr), "0.0.0.0")) {
			address.sin_addr.s_addr = inet_addr("127.0.0.1");
		}
		// in case of server binds ip address "0.0.0.0" for accepting clients

		return Address(address);
	}

	Address Socket::getpeername() throw (Exception)
	{
		int error;

		if(!isOpened()) {
			throw Exception(GETPEERNAME, NOT_OPENED, NOT_OPENED_CODE);
		}

		sockaddr_in address;
		socklen_t addrLen = sizeof(address);

		sockaddr* socketAddress = reinterpret_cast<sockaddr*>(&address);

		error = ::getpeername(descriptor, socketAddress, &addrLen);

		if(error == GETPEERNAME_ERROR) {
			throw LibCException(GETPEERNAME);
		}

		return Address(address);
	}

	int Socket::getsockopt(Option& option) throw (Exception)
	{
		int error;

		if(!isOpened()) {
			throw Exception(GETSOCKOPT, NOT_OPENED, NOT_OPENED_CODE);
		}

		socklen_t optSize = option.size();

		error = ::getsockopt(descriptor, option.level(), option.id(), option.pointer(), &optSize);

		if(error == GETSOCKOPT_ERROR) {
			throw LibCException(GETSOCKOPT);
		}

		return optSize;
	}

	void Socket::setsockopt(const Option& option) throw (Exception)
	{
		int error;

		if(!isOpened()) {
			throw Exception(SETSOCKOPT, NOT_OPENED, NOT_OPENED_CODE);
		}

		error = ::setsockopt(descriptor, option.level(), option.id(), option.pointer(), option.size());

		if(error == SETSOCKOPT_ERROR) {
			throw LibCException(SETSOCKOPT);
		}
	}

	void Socket::setsocktimeout(int Option, int toTime) throw (Exception)
	{
		int error;
		int Op;
		struct timeval tv;

		tv.tv_sec = toTime;
		tv.tv_usec = 0;

		if(!isOpened()) {
			throw Exception(SETSOCKOPT, NOT_OPENED, NOT_OPENED_CODE);
		}

		if(Option == 0) {
			Op = SO_RCVTIMEO;
		} else {
			Op = SO_SNDTIMEO;
		}

		error = ::setsockopt(descriptor, SOL_SOCKET, Op, (struct timeval *)&tv, sizeof(tv));

		if(error == SETSOCKOPT_ERROR) {
			throw LibCException(SETSOCKOPT);
		}
	}

	int Socket::shutdown(Shutdown how) throw (Exception)
	{
		int error;

		if(!isOpened()) {
			throw Exception(SHUTDOWN, NOT_OPENED, NOT_OPENED_CODE);
		}

		error = ::shutdown(descriptor, how);
		return error;
	}

	bool Socket::wait(bool& read, bool& write, bool& exception, int seconds, int useconds) throw (Exception)
	{
		if(!isOpened()) {
			throw Exception(WAIT, NOT_OPENED, NOT_OPENED_CODE);
		}

		struct timeval time;
		time.tv_sec  = seconds;
		time.tv_usec = useconds;

		fd_set* readfds = 0;
		fd_set* writefds = 0;
		fd_set* exceptfds = 0;

		if(read) {
			readfds = new fd_set;

			FD_ZERO(readfds);
			FD_SET(descriptor, readfds);
		}

		if(write) {
			writefds = new fd_set;

			FD_ZERO(writefds);
			FD_SET(descriptor, writefds);
		}

		if(exception) {
			exceptfds = new fd_set;

			FD_ZERO(exceptfds);
			FD_SET(descriptor, exceptfds);
		}

		int ret = select(descriptor+1, readfds, writefds, exceptfds, &time);

		if(read) {
			read = FD_ISSET(descriptor, readfds);
		}

		if(write) {
			write = FD_ISSET(descriptor, writefds);
		}

		if(exception) {
			exception = FD_ISSET(descriptor, exceptfds);
		}

		delete readfds;
		delete writefds;
		delete exceptfds;

		if(ret < 0) {
			throw LibCException(WAIT);
		}

		return (ret != 0);
	}

	bool Socket::setnonblock(void) throw (Exception)
	{
		int flags = ::fcntl(descriptor, F_GETFL);

		if(flags == -1) {
			throw Exception(FCNTL, ::strerror(errno), flags);
		}
#ifndef O_NONBLOCK
#define O_NONBLOCK O_NDELAY
#endif
		flags	|= O_NONBLOCK;

		return ::fcntl(descriptor, F_SETFL, flags) == 0;
	}
}
