/****************************************************************************

   Ras library FreeBSD Version

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: net_io.h,v 1.1 2007/03/21 06:14:07 miracle Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#ifndef __NET_IO_H__
#define __NET_IO_H__

#include <iostream>
#include <map>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

#ifdef __NET_IO_USE_EPOLL__
#include <sys/epoll.h>
#endif

#ifdef __NET_IO_USE_KQUEUE__
#include <sys/event.h>
#endif

#ifdef __NET_IO_USE_SELECT__
#include <sys/select.h>
#endif

#include "raslib.h"
#include "socket.h"
#include "exception.h"
#include "pointer.h"

using namespace std;
using namespace SDSocket;

#define MaxSizeOfEventCache 1000
#define INIT_NET_IO_EVENT    { NULL, -1, 0, NULL }
#define NET_IO_CB_PROTOTYPE void(*callback)(SOCKET, short)

namespace SDSocket
{
	enum NET_IO_MODE {
		NET_IO_WANT_READ  = 1,
		NET_IO_WANT_WRITE = 2,
		NET_IO_ERROR = 4
	};

	typedef struct _s_net_io_event {
		SOCKET fd;
		NET_IO_CB_PROTOTYPE;
		short what;
	} net_io_event_t;

	void IOEventInit(unsigned int size_of_event_queue) throw (Exception);
	bool IOEventCreate(SOCKET fd, short what, NET_IO_CB_PROTOTYPE);
	bool IOEventSetCB(SOCKET fd, NET_IO_CB_PROTOTYPE);
	bool IOEventAdd(SOCKET fd, short what);
	bool IOEventRemove(SOCKET fd, short what);
	bool IOEventClose(SOCKET fd);
	int  IOEventDispatch(struct timeval *tv);
	void IOEventDestroy(void) throw();
	void PrintAll(void);
}
#endif
