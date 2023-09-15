/****************************************************************************

   Ras library FreeBSD Version

   Copyright(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: net_io.cpp,v 1.1 2007/03/21 06:14:07 miracle Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "net_io.h"

#ifndef FD_SETSIZE
#define FD_SETSIZE 1024
#endif
#define MaxSizeOfNetIOErrorBuffer 100

namespace SDSocket 
{
	/*
	vector<net_io_event_t>::iterator _net_io_event_get(SOCKET fd) 
	{
		vector<net_io_event_t>::iterator i; 

		for(i = g_io_events.begin(); i != g_io_events.end(); i++) {
			if(i->fd == fd) {
				break;
			}
		}
		return i;
	}
	*/

	typedef map<SOCKET, net_io_event_t> net_io_event_map_t;
	static bool g_flag_net_io_initialized;
	static net_io_event_map_t g_io_events;

	static bool _net_io_call_cbfunc(SOCKET fd, short what);
	static net_io_event_t *_net_io_event_get(SOCKET fd);
#if defined(__NET_IO_USE_EPOLL__) || defined(__NET_IO_USE_KQUEUE__)
	static SOCKET g_net_io_master_fd;
#endif

#ifdef __NET_IO_USE_EPOLL__
	static bool _net_io_init_epoll(unsigned int size_of_event_queue);
	static bool _net_io_event_change_epoll(SOCKET fd, short what, const int action);
	static int _net_io_dispatch_epoll(struct timeval *tv);
#endif

#ifdef __NET_IO_USE_KQUEUE__
	static vector<struct kevent> m_io_kqueue_evcache;
	static bool _net_io_event_kqueue_commit_cache(void);
	static bool _net_io_init_kqueue(unsigned int size_of_event_queue);
	static bool _net_io_event_change_kqueue(SOCKET fd, short what, const int action);
	static int _net_io_dispatch_kqueue(struct timeval *tv);
#endif

#ifdef __NET_IO_USE_SELECT__
	static fd_set g_fd_set_read;
	static fd_set g_fd_set_write;
	static SOCKET g_select_max_fd;

	static bool _net_io_init_select(unsigned int size_of_event_queue);
	static int _net_io_dispatch_select(struct timeval *tv);
#endif

	net_io_event_t* _net_io_event_get(SOCKET fd) 
	{
		net_io_event_t *event_ptr = NULL;
		net_io_event_map_t::iterator i = g_io_events.find(fd);

		if(i != g_io_events.end()) {
			event_ptr = &i->second;
		} else {
			event_ptr = NULL;
		}

		return event_ptr;
	}

	bool _net_io_call_cbfunc(SOCKET fd, short what)
	{
		net_io_event_map_t::iterator i = g_io_events.find(fd);
		if((i != g_io_events.end()) && (i->second.fd > 0)) {
			/* callback might be NULL if a previous callback function called IOEventClose on this fd */
			if(i->second.callback) {
				i->second.callback(fd, (what & NET_IO_ERROR)?i->second.what:what);
			} else {
				return false;
			}
		}
		return true;
	}
			
#ifdef __NET_IO_USE_EPOLL__
	bool _net_io_init_epoll(unsigned int size_of_event_queue) 
	{
		int epoll_hint_size = static_cast<int>(size_of_event_queue);
		
		if(epoll_hint_size <= 0) {
			epoll_hint_size = 128;
		}
		
		g_net_io_master_fd = ::epoll_create(epoll_hint_size);
		if(g_net_io_master_fd < 0) {
			throw Exception("_net_io_init_epoll::epoll_create()", ::strerror(errno), errno);
		}

		return true;
	}
#endif

#ifdef __NET_IO_USE_KQUEUE__
	bool _net_io_init_kqueue(unsigned int size_of_event_queue)
	{
		g_net_io_master_fd = ::kqueue();
		if(g_net_io_master_fd < 0) {
			throw Exception("_net_io_init_kqueue::kqueue()", ::strerror(errno), errno);
		}

		return true;
	}
#endif

#ifdef __NET_IO_USE_SELECT__
	bool _net_io_init_select(unsigned int size_of_event_queue)
	{
		if(size_of_event_queue >= FD_SETSIZE) {
			size_of_event_queue = FD_SETSIZE - 1;
		}

		FD_ZERO(&g_fd_set_read);
		FD_ZERO(&g_fd_set_write);
		return true;
	}
#endif
		
	void IOEventInit(unsigned int size_of_event_queue) throw(Exception)
	{
		try {
			if(g_flag_net_io_initialized) {
				throw Exception("IOEventInit", "CNetIO is already initialized.", -1);
			} else {
				g_flag_net_io_initialized = true;
			}
#ifdef __NET_IO_USE_EPOLL__
			_net_io_init_epoll(size_of_event_queue);
#endif
#ifdef __NET_IO_USE_KQUEUE__
			_net_io_init_kqueue(size_of_event_queue);
#endif
#ifdef __NET_IO_USE_SELECT__
			_net_io_init_select(size_of_event_queue);
#endif
		} catch (Exception &e) {
			throw Exception(e.getMethod(), e.getMessage(), e.getCode());
		}
	}

	void IOEventDestroy(void) throw()
	{
#ifdef __NET_IO_USE_SELECT__
		FD_ZERO(&g_fd_set_read);
		FD_ZERO(&g_fd_set_write);
#endif
#if defined(__NET_IO_USE_EPOLL__) || defined(__NET_IO_USE_KQUEUE__)
		::close(g_net_io_master_fd);
#endif
		//g_io_events.~vector<net_io_event_t>;
	}

	bool IOEventSetCB(SOCKET fd, NET_IO_CB_PROTOTYPE)
	{
		net_io_event_t *event_ptr = _net_io_event_get(fd);

		if(event_ptr == NULL) {
			return false;
		}

		event_ptr->callback = callback;
		return true;
	}

	/*
	inline net_io_callback_t& IOEventMakeCBType(NET_IO_CALLBACK_TYPE type, void *ptr) throw(Exception)
	{
		net_io_callback_t new_cb;

		new_cb.type = type;
		if(type == NET_IO_CB_TYPE_FUNC) {
			new_cb.cb_ptr.callback_function = ptr;
		} else {
			new_cb.cb_ptr.callback_member = ptr;
		}

		return new_cb;
	}
	*/

#ifdef __NET_IO_USE_EPOLL__
	bool _net_io_event_change_epoll(SOCKET fd, short what, const int action) 
	{
		struct epoll_event ev = { 0, {0} };

		ev.data.fd = fd;
		if(what & NET_IO_WANT_READ) {
			ev.events = EPOLLIN | EPOLLPRI;
		}

		if(what & NET_IO_WANT_WRITE) {
			ev.events |= EPOLLOUT;
		}

		return ::epoll_ctl(g_net_io_master_fd, action, fd, &ev) == 0;
	}
#endif

#ifdef __NET_IO_USE_KQUEUE__
	bool _net_io_event_kqueue_commit_cache(void)
	{
		int ret = 0;

		if(m_io_kqueue_evcache.size() == 0) {
			return true;
		}

		ret = ::kevent(g_net_io_master_fd, &m_io_kqueue_evcache[0], m_io_kqueue_evcache.size(), NULL, 0, NULL);
		if(ret > 0) {
			return false;
		} else {
			m_io_kqueue_evcache.clear(); 
		}

		return true;
	}

	bool _net_io_event_change_kqueue(SOCKET fd, short what, const int action)
	{
		int ret = 0;
		struct kevent kev;

		if(what & NET_IO_WANT_READ) {
			EV_SET(&kev, fd, EVFILT_READ, action, 0, 0, 0);
			m_io_kqueue_evcache.push_back(kev);
		}

		if(what & NET_IO_WANT_WRITE) {
			EV_SET(&kev, fd, EVFILT_WRITE, action, 0, 0, 0);
			m_io_kqueue_evcache.push_back(kev);
		}

		if(m_io_kqueue_evcache.size() >= MaxSizeOfEventCache) {
			ret = _net_io_event_kqueue_commit_cache();
		}

		return (!ret)?false:true;
	}
#endif

	bool IOEventAdd(SOCKET fd, short what) 
	{
		net_io_event_t *event_ptr = NULL;
		
		event_ptr = _net_io_event_get(fd);

		if(event_ptr == NULL) {
			return false;
		}

		if(event_ptr->what == what) {
			return true;
		}

		event_ptr->what |= what;
#ifdef __NET_IO_USE_EPOLL__
		return _net_io_event_change_epoll(fd, event_ptr->what, EPOLL_CTL_MOD);
#endif
#ifdef __NET_IO_USE_KQUEUE__
		return _net_io_event_change_kqueue(fd, what, EV_ADD | EV_ENABLE);
#endif
#ifdef __NET_IO_USE_SELECT__
		if(fd > g_select_max_fd) {
			g_select_max_fd = fd;
		}

		if(what & NET_IO_WANT_READ) {
			FD_SET(fd, &g_fd_set_read);
		} 
		if(what & NET_IO_WANT_WRITE) {
			FD_SET(fd, &g_fd_set_write);
		}
		return true;
#endif
	}

	bool IOEventCreate(SOCKET fd, short what, NET_IO_CB_PROTOTYPE)
	{
		bool ret = false;
		net_io_event_t new_event;

		if(g_io_events.count(fd)) {
			char buffer[MaxSizeOfNetIOErrorBuffer];
			::snprintf(buffer, MaxSizeOfNetIOErrorBuffer, "Tried to add fd(%d) to multiplexer twice", fd);
			throw Exception("IOEventCreate", buffer, -1);
		}
#ifdef __NET_IO_USE_SELECT__
		if(fd > static_cast<int>(FD_SETSIZE)) {
			throw Exception("IOEventCreate", "exceeds FD_SETSIZE:select cannot handle more file descriptors", -2);
		}
#endif
		new_event.fd = fd;
		new_event.callback = callback;
		new_event.what = 0;

#ifdef __NET_IO_USE_EPOLL__
		ret = _net_io_event_change_epoll(fd, what, EPOLL_CTL_ADD);
#endif
#ifdef __NET_IO_USE_KQUEUE__
		ret = _net_io_event_change_kqueue(fd, what, EV_ADD | EV_ENABLE);
#endif
#ifdef __NET_IO_USE_SELECT__
		ret = IOEventAdd(fd, what);
#endif
		if(ret) new_event.what = what;
		
		g_io_events[fd] = new_event;
		return ret;
	}

	bool IOEventClose(SOCKET fd)
	{
		net_io_event_t *event_ptr = NULL;

#ifdef __NET_IO_USE_SELECT__
		FD_CLR(fd, &g_fd_set_read);
		FD_CLR(fd, &g_fd_set_write);

		/*
		if(fd == g_select_max_fd) {
			while(g_select_max_fd > 0) {
				--select_maxfd;
				*/
#endif
		event_ptr = _net_io_event_get(fd);
		if(event_ptr == NULL) {
			return false;
		}
#ifdef __NET_IO_USE_KQUEUE__
		if(m_io_kqueue_evcache.size() > 0) {
			_net_io_event_kqueue_commit_cache();
		}

		/* 
		 * both kqueue and epoll remove fd from all sets automatically on the last close
		 * of the descriptor. since we don't know if this is the last close we'll have
		 * to remove the set explicitly. 
		 */

		if(event_ptr) {
			_net_io_event_change_kqueue(fd, event_ptr->what, EV_DELETE);
			_net_io_event_kqueue_commit_cache();
		}
#endif
#ifdef __NET_IO_USE_EPOLL__
		_net_io_event_change_epoll(fd, 0, EPOLL_CTL_DEL);
#endif
		if(g_io_events.erase(fd) < 0) {
			fprintf(stderr, "FAIL REMOVE fd : %d\n", fd);
		}
		return ::close(fd) == 0;
	}

	bool IOEventRemove(SOCKET fd, short what)
	{
		net_io_event_t *event_ptr = _net_io_event_get(fd);

		if(!event_ptr) {
			return false;
		}

		event_ptr->what &= ~what;

#ifdef __NET_IO_USE_EPOLL__
		return _net_io_event_change_epoll(fd, event_ptr->what, EPOLL_CTL_MOD);
#endif
#ifdef __NET_IO_USE_KQUEUE__
		return _net_io_event_change_kqueue(fd, what, EV_DISABLE);
#endif
#ifdef __NET_IO_USE_SELECT__
		if(what & NET_IO_WANT_WRITE) {
			FD_CLR(fd, &g_fd_set_write);
		}
		if(what & NET_IO_WANT_READ) {
			FD_CLR(fd, &g_fd_set_read);
		}
		return true;
#endif
	}

#ifdef __NET_IO_USE_EPOLL__
	int _net_io_dispatch_epoll(struct timeval *tv) 
	{
		int i = 0;
		int ret = 0;
		int total = 0; 
		int cb_ret = 0;
		short what = 0;	
		time_t sec = tv->tv_sec * 1000;
		int timeout = tv->tv_usec + static_cast<int>(sec);
		struct epoll_event epoll_event_queue[MaxSizeOfEventCache];
	
		if(timeout < 0) {
			timeout = 1000;
		}

		do {
			ret = ::epoll_wait(g_net_io_master_fd, epoll_event_queue, MaxSizeOfEventCache, timeout);
			//total += ret;

			if(ret <= 0) {
				//return total;
				return ret;
			}

			for(i = 0;i < ret; i++) {
				what = 0;
				if(epoll_event_queue[i].events & (EPOLLERR | EPOLLHUP)) {
					what = NET_IO_ERROR;
				}
				if(epoll_event_queue[i].events & (EPOLLIN | EPOLLPRI)) {
					what = NET_IO_WANT_READ;
				}
				if(epoll_event_queue[i].events & EPOLLOUT) {
					what = NET_IO_WANT_WRITE;
				}
				cb_ret = _net_io_call_cbfunc(epoll_event_queue[i].data.fd, what);
				if(!cb_ret) {
					net_io_event_map_t::const_iterator map_iter;
					for(map_iter=  g_io_events.begin(); map_iter != g_io_events.end(); ++map_iter)  {
						_net_io_call_cbfunc(map_iter->first, what);
					}
				} 
			}
			timeout = 0;
		}while (1); //(ret == MaxSizeOfEventCache);

		return total;
	}
#endif

#ifdef __NET_IO_USE_KQUEUE__
	int _net_io_dispatch_kqueue(struct timeval *tv)
	{
		int i = 0;
		int ret = 0;
		int total = 0; 
		int num_of_new_event = 0;
		struct timespec ts;
		struct kevent *new_event_ptr = NULL;
		struct kevent kqueue_event[MaxSizeOfEventCache];

		ts.tv_sec = tv->tv_sec;
		ts.tv_nsec = tv->tv_usec * 1000;

		do {
			num_of_new_event = m_io_kqueue_evcache.size();
			new_event_ptr = (num_of_new_event > 0)?(&m_io_kqueue_evcache.front()):NULL;

			ret = ::kevent(g_net_io_master_fd, new_event_ptr, num_of_new_event,
					       kqueue_event, MaxSizeOfEventCache, &ts);
			if((num_of_new_event > 0) && ret < 0) {
				m_io_kqueue_evcache.clear();
			}

			total += ret;
			if(ret <= 0) {
				return total;
			}

			for(i = 0; i < ret; i++) {
				if(kqueue_event[i].flags & EV_EOF) {
					_net_io_call_cbfunc(static_cast<int>(kqueue_event[i].ident), NET_IO_ERROR);
					continue;
				}

				switch(kqueue_event[i].filter) {
					case EVFILT_READ:
						_net_io_call_cbfunc(static_cast<int>(kqueue_event[i].ident), NET_IO_WANT_READ);
						break;
					case EVFILT_WRITE:
						_net_io_call_cbfunc(static_cast<int>(kqueue_event[i].ident), NET_IO_WANT_WRITE);
						break;
					case EV_ERROR:
						_net_io_call_cbfunc(static_cast<int>(kqueue_event[i].ident), NET_IO_ERROR);
						break;
					default:
						break;
				}

				ts.tv_sec = 0;
				ts.tv_nsec = 0;
			}
		} while (ret == MaxSizeOfEventCache);

		return total;
	}
#endif

#ifdef __NET_IO_USE_SELECT__
	int _net_io_dispatch_select(struct timeval *tv)
	{
		int i = 0;
		int ret = 0;
		short what = 0;
		int fds_in_ready = 0; 
		fd_set tmp_fd_set_read = g_fd_set_read;
		fd_set tmp_fd_set_write = g_fd_set_write;

		ret = ::select(g_select_max_fd + 1, &tmp_fd_set_read, &tmp_fd_set_write, NULL, tv);
		if(ret <= 0) {
			return ret;
		}

		fds_in_ready = ret;

		for(i = 0; i <= g_select_max_fd; i++) {
			what = 0;
			if(FD_ISSET(i, &tmp_fd_set_read)) {
				what = NET_IO_WANT_READ;
				fds_in_ready--;
			}
			if(FD_ISSET(i, &tmp_fd_set_write)) {
				what = NET_IO_WANT_WRITE;
				fds_in_ready--;
			}
			if(what) {
				_net_io_call_cbfunc(i, what);
			}
			if(fds_in_ready <= 0) {
				break;
			}
		}

		return ret;
	}
#endif

	int IOEventDispatch(struct timeval *tv) 
	{
#ifdef __NET_IO_USE_EPOLL__
		return _net_io_dispatch_epoll(tv);
#endif
#ifdef __NET_IO_USE_KQUEUE__
		return _net_io_dispatch_kqueue(tv);
#endif
#ifdef __NET_IO_USE_SELECT__
		return _net_io_dispatch_select(tv);
#endif
	}
	
	void PrintAll(void)
	{
		net_io_event_map_t::const_iterator i;
		for(i=  g_io_events.begin(); i != g_io_events.end(); ++i)  {
			fprintf(stderr, "net_io::ident = %d\n", i->first);
			fprintf(stderr, "net_io::Socket = %d\n", i->second.fd);
		}
	}
}

