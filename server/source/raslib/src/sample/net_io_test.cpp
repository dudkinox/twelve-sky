#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "net_io.h"

using namespace std;
using namespace SDSocket;

#define MaxSizeOfHint 2048
#define PRINT_ERROR()  ::fprintf(stderr, "%s: %s(%d)\n", e.getMethod().c_str(), e.getMessage().c_str(), e.getCode())

static CNetIO net_io_multiplexer;

void cb_accept_new_client(SOCKET listen_fd, short _not_use)
{
	Socket new_client_sock;
	Address new_client_addr;

	try {
		new_client_sock.SetDescriptor(net_io_multiplexer.accept(new_client_addr));

	} catch (Exception &e) {
	} catch (...) {
	}
}

void do_accept(CNetIO &net_io_multiplexer)
{
	int fRet = 0;
	try {
		net_io_multiplexer.IOEventInit(MaxSizeOfHint);
		fRet = net_io_multiplexer.IOEventCreate(net_io_multiplexer.GetDescriptor(), CNetIO::NET_IO_WANT_READ, cb_accept_new_client);
		if(!fRet) {
			net_io_multiplexer.close();
		}
	} catch (Exception &e) {
		PRINT_ERROR();
		::exit(1);
	}
}

int main(void)
{
	int pid = 0;
	int status = 0;

	try {
		net_io_multiplexer.open(Socket::TCP);
		net_io_multiplexer.bind(Address(Address::ANY_IP, 2007, 255));
		net_io_multiplexer.listen(255);
		net_io_multiplexer.setnonblock();

	} catch (Exception &e) {
		PRINT_ERROR();
		::exit(1);
	}

	if((pid = ::fork()) < 0) {
		fprintf(stderr, "fork: %s(%d)\n", ::strerror(errno), errno);
		::exit(2);
	} else if(pid == 0) {
		do_accept(net_io_multiplexer);
	}
	::wait(&status);

	return 0;
}
