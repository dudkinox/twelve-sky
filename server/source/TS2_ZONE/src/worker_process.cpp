/****************************************************************************
 *
 * CTSD : Common Transaction Service
 *
 * CopyRight(C) 2003 Sean Kim All Rights reserved.
 * Author : Sean Kim (sean@0x31.com)
 *
 * $Id: worker_process.cpp,v 1.29 2007/10/20 08:41:13 wglee Exp $
 *
 * Redistribution and use in source and binary forms, with or with out
 * modification, are not permitted without author's permission.
 *
 ****************************************************************************/

#include "log.h"
#include "local.h"
#include "global.h"
#include "socket.h"
#include "pointer.h"
#include "worker_thread.h"
#include "worker_process.h"
#include "cts_util.h"
#include "H01_MainApplication.h"
#include <sys/epoll.h>
#include <signal.h>

using namespace SDLog;
using namespace SDSocket;

extern CScoreBoard *g_score_board_obj_ptr;

inline static void sig_handler(int signo)
{
	int index01;
	for(index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++) {
		mUSER[index01].Quit(__FILE__, __FUNCTION__, __LINE__);
	}
	::kill(0, SIGKILL);
	::kill(0, SIGKILL);
	::kill(0, SIGKILL);
#if 0
	mBaseTickCountForLogic = GetTickCount();
	if( ( mBaseTickCountForLogic - mPostTickCountForLogic ) >= 500 ) {
		mPostTickCountForLogic = mBaseTickCountForLogic;
		mCheckLogicFlag = TRUE;
	}
	if( mCheckLogicFlag ) {
		mCheckLogicFlag = FALSE;
		mGAME.Logic( 0.5f );
	}
#endif
}

CWorkerProcess::CWorkerProcess(void) 
{
    m_pid = ::getpid();
}

CWorkerProcess::CWorkerProcess(SOCKET listen_socket) throw(Exception) 
{
    set_listen_socket(listen_socket);
    m_pid = ::getpid();
}

CWorkerProcess::~CWorkerProcess(void) 
{
}

void CWorkerProcess::set_listen_socket(SOCKET listen_socket) throw(Exception) 
{
    if(listen_socket < 0) {
        throw Exception(msg_str_obj.c_str(), "Invalid socket is passed.", -1);
    }
    
    m_listen_socket_obj.SetDescriptor(listen_socket);
}

#ifdef __NB__
int CWorkerProcess::set_nonblock(int cli_fd)
{
	int old_flag = 0;

	old_flag = fcntl(cli_fd, F_GETFL);
	return fcntl(cli_fd, F_SETFL, old_flag | O_NONBLOCK);
}

int CWorkerProcess::set_nodelay(int cli_fd)
{
	int optval = 1;
	socklen_t sock_len = sizeof(int);

	return setsockopt(cli_fd, IPPROTO_TCP, TCP_NODELAY, (const char *)&optval, sock_len);
}
#endif

#define MAX_SIZE_OF_FD       4000
int CWorkerProcess::start_svr_service(void) 
{
	int i = 0;
	int n_read = 0;
	int n_send =0;
    int ret_val = 0;
	struct sigaction si, so;
	struct itimerval ival, oval;

	socklen_t cli_len = 0;
	int client_sockfd = -1;
	struct sockaddr_in cli_addr;
#ifdef __EPOLL__
	int res = 0;
	int epfd = -1;
	struct epoll_event ev;
	struct epoll_event *events;
	int listenfd = m_listen_socket_obj.GetDescriptor();
#endif

    try {

		if(!mSERVER.Init(&ret_val)) {
			LOG_TO_FILE_2("![%s] : ret_val %d\n", __FUNCTION__, ret_val);
			throw Exception("CWorkerProcess::start_svr_service", "server object initialization failed", -1);
		}

		mBaseTickCountForLogic = GetTickCount();
		mPostTickCountForLogic = mBaseTickCountForLogic;
		mCheckLogicFlag = FALSE;

#ifdef __EPOLL__
		memset(&si, 0x00, sizeof(si));
		memset(&so, 0x00, sizeof(so));
		memset(&ival, 0x00, sizeof(ival));
		memset(&oval, 0x00, sizeof(oval));

		si.sa_handler = sig_handler;
		sigfillset(&si.sa_mask);
		sigaction(SIGTERM, &si, NULL);
#if 0
		si.sa_handler = sig_handler;
		sigaction(SIGALRM, &si, &so);

		ival.it_interval.tv_usec = 500000;
		ival.it_value.tv_sec = 1;
		setitimer(ITIMER_REAL, &ival, &oval);
#endif

		events = (struct epoll_event *)malloc(sizeof(events) * MAX_SIZE_OF_FD);

		epfd = epoll_create(MAX_SIZE_OF_FD);
		if(!epfd) {
			LOG_TO_FILE_2("![%s] : %s\n", __FUNCTION__, strerror(errno));
			throw Exception("CWorkerProcess::start_svr_service", "epoll create failed", -2);
		}
		set_nonblock(listenfd);
		ev.events = EPOLLIN | EPOLLERR | EPOLLHUP;
		ev.data.fd = listenfd;
		if(epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev) < 0) {
			LOG_TO_FILE_2("![%s] : %s\n", __FUNCTION__, strerror(errno));
			throw Exception("CWorkerProcess::start_svr_service", "epoll control failed", -3);
		}

		for( ; ; ) {
			res = epoll_wait(epfd, events, MAX_SIZE_OF_FD, 100);
			if(res == -1) {
				if(errno != EINTR) {
					LOG_TO_FILE_2("![%s] : %s\n", __FUNCTION__, strerror(errno));
					continue;
					//throw Exception("CWorkerProcess::start_svr_service", "epoll wait failed", -4);
				}
				continue;
			}

			for(i = 0; i < res; i++) {
				if(events[i].data.fd == listenfd) {
					//LOG_TO_FILE("!readable\n");
					cli_len = sizeof(cli_addr);
					while((client_sockfd = accept(listenfd, (struct sockaddr *)&cli_addr, &cli_len)) < 0) {
						if(errno == EINTR)
							continue;
						if (errno != EAGAIN && errno != EWOULDBLOCK) {
							LOG_TO_FILE_2("! : [%s] accept failed(%s)\n", __FUNCTION__, strerror(errno));
							break;
						}
					}
					if(client_sockfd > 0) {
#if 1
						if(set_nodelay(client_sockfd) != 0) {
							LOG_TO_FILE_2("![%s] : set_nodelay(%s)\n", __FUNCTION__, strerror(errno));
						}
#endif
						if(set_nonblock(client_sockfd) < 0) {
							LOG_TO_FILE_2("![%s] : set_nonblock(%s)\n", __FUNCTION__, strerror(errno));
						}
						ev.events = EPOLLIN | EPOLLET;
						ev.data.fd = client_sockfd;
						if(epoll_ctl(epfd, EPOLL_CTL_ADD, client_sockfd, &ev) < 0) {
							LOG_TO_FILE_2("![%s] : 1 : %s\n", __FUNCTION__, strerror(errno));
							ProcessResponse(WM_NETWORK_MESSAGE_1, &client_sockfd, FD_CLOSE);
							continue;
						}
						//LOG_TO_FILE_2("%s, fd : %d\n", __FUNCTION__, client_sockfd);
						if(ProcessResponse(WM_NETWORK_MESSAGE_1, &client_sockfd, FD_ACCEPT, (void *)&cli_addr.sin_addr) < 0) {
							ProcessResponse(WM_NETWORK_MESSAGE_1, &client_sockfd, FD_CLOSE);
							continue;
						}
						ev.events |= EPOLLOUT;
						if(epoll_ctl(epfd, EPOLL_CTL_MOD, client_sockfd, &ev) < 0) {
							LOG_TO_FILE_2("![%s] : 2 : %s\n", __FUNCTION__, strerror(errno));
							ProcessResponse(WM_NETWORK_MESSAGE_1, &client_sockfd, FD_CLOSE);
							continue;
						}
					} 
				} else {
					if((events[i].events & EPOLLIN)) {
						n_read = ProcessResponse(WM_NETWORK_MESSAGE_1, &events[i].data.fd, FD_READ);
#if 0
						if(n_read < 0) {
							if(errno == EINTR)
								continue;
							if(errno != EAGAIN && errno != EWOULDBLOCK) {
								ev.data.fd = events[i].data.fd;
								ev.events = EPOLLIN | EPOLLERR | EPOLLHUP;
								epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, &ev);
								ProcessResponse(WM_NETWORK_MESSAGE_1, &events[i].data.fd, FD_CLOSE);
							}
							ev.data.fd = events[i].data.fd;
							ev.events = EPOLLIN | EPOLLERR | EPOLLHUP;
							if(!(events[i].events & EPOLLIN)) {
								epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, &ev);
								ProcessResponse(WM_NETWORK_MESSAGE_1, &events[i].data.fd, FD_CLOSE);
							}
						} else if(n_read == 0) {
							ev.data.fd = events[i].data.fd;
							ev.events = EPOLLIN | EPOLLERR | EPOLLHUP;
							epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, &ev);
							ProcessResponse(WM_NETWORK_MESSAGE_1, &events[i].data.fd, FD_CLOSE);
						} 
#endif
					}

					if((events[i].events & EPOLLOUT)) {
						n_send = ProcessResponse(WM_NETWORK_MESSAGE_1, &events[i].data.fd, FD_WRITE);
					}
				}
			}
#if 1
			mBaseTickCountForLogic = GetTickCount();
			//LOG_TO_FILE_3("-------------->%u, %u, %u\n", mBaseTickCountForLogic, mPostTickCountForLogic, GetTickCount());
			if( ( mBaseTickCountForLogic - mPostTickCountForLogic ) >= 500 ) {
				mPostTickCountForLogic = mBaseTickCountForLogic;
				mCheckLogicFlag = TRUE;
			}
			if( mCheckLogicFlag ) {
				mCheckLogicFlag = FALSE;
				mGAME.Logic( 0.5f );
			}
#endif
		}
#endif
	} catch (Exception e) {
		LOG_TO_FILE_3("![%d] %s (error code = %d) \n", m_pid, e.getMessage().c_str(), e.getCode());
		return -2;
	}
	return 1;
}
