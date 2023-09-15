/****************************************************************************

   CTSD : Common Transaction Service 

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: signal.cpp,v 1.8 2007/08/03 05:24:03 sean Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "local.h"
#include "global.h"
#include "socket.h"
#include "cts_util.h"
#include "cts_signal.h"
#include "score_board.h"

using namespace std;
using namespace SDLog;
using namespace SDSocket;

extern CLogging *g_log_obj_ptr;
extern Socket g_listen_socket;
extern CScoreBoard *g_score_board_obj_ptr;

static void signal_child(int);
static void signal_guard(int);
static void signal_gstop(int);
static sigset_t mask;

static struct {
	int sig;
	void (*hand)(int);
} child_sighandlers[] = {
	{ SIGCHLD, signal_child },
	{ SIGTERM, signal_child },
	{ SIGQUIT, signal_child },
	{ SIGILL,  signal_child },
	{ SIGUSR1, SIG_IGN },
	{ SIGUSR2, SIG_IGN },
	{ SIGINT,  SIG_IGN },
	{ SIGPIPE, SIG_IGN },
	{ SIGALRM, SIG_IGN }
};

static struct {
	int sig;
	void (*hand)(int);
} guard_sighandlers[] = {
	{ SIGCHLD, signal_guard },
	{ SIGTERM, signal_guard },
	{ SIGQUIT, signal_guard },
	{ SIGILL,  signal_guard },
	{ SIGUSR1, signal_gstop },
	{ SIGUSR2, SIG_IGN },
	{ SIGINT,  SIG_IGN },
	{ SIGPIPE, SIG_IGN },
	{ SIGALRM, SIG_IGN }
};

static void signal_child(int sig)
{
	pid_t pid;
	int status;

	if ((pid = ::waitpid(-1, &status, WNOHANG)) < 0 ) {
		sleep(10);
		LOG_TO_FILE_2("![%d] : %s\n", ::getpid(), ::strerror(errno));
	}
	//LOG_TO_FILE_2("![%d] has been signaled (%d)\n", pid, sig);
	g_score_board_obj_ptr->mark_proc_dead(pid);
}

static void signal_guard(int sig)
{
	pid_t pid;
	int status;

	if ((pid = ::waitpid(-1, &status, WNOHANG)) < 0 ) {
		LOG_TO_FILE_2("![%d] : %s\n", ::getpid(), ::strerror(errno));
	}
	LOG_TO_FILE_2("![PARENT:%d] has been signaled (%d)\n", ::getpid(), sig);

	g_listen_socket.close();
	unlink_pid_file();
}

static void signal_gstop(int sig)
{
	LOG_TO_FILE_1("![PARENT:%d] starts to shutdown gracefully. \n", ::getpid());
	g_score_board_obj_ptr->let_cm_stop(CScoreBoard::SHUTDOWN_GRACEFULLY);
}

static int set_sighandlers_child(void) 
{
	int sh = 0;
	struct sigaction sa;

	::sigemptyset(&mask);
	for (sh = 0; sh < (int)(sizeof(child_sighandlers)/sizeof(child_sighandlers[0])); sh++) {
		::sigaddset(&mask, child_sighandlers[sh].sig);
		::memset(&sa, 0, sizeof(sa));
		sa.sa_mask = mask;
		sa.sa_handler = child_sighandlers[sh].hand;
		if (::sigaction(child_sighandlers[sh].sig, &sa, NULL) < 0 ) {
			LOG_TO_FILE_2("!setting action against signal (%d) has been failed : %s\n", child_sighandlers[sh].sig, strerror(errno));
			return -1;
		}
	}
	
	if (::sigprocmask(SIG_UNBLOCK, &mask, NULL) < 0 ) {
		LOG_TO_FILE_1(" failed : %s", strerror(errno));
		return -2;
	}

	return 1;
}

static int set_sighandlers_guard(void) 
{
	int sh = 0;

	struct sigaction sa;
	::sigemptyset(&mask);
	for (sh = 0; sh < (int)(sizeof(guard_sighandlers)/sizeof(guard_sighandlers[0])); sh++) {
		::sigaddset(&mask, guard_sighandlers[sh].sig);
		::memset(&sa, 0, sizeof(sa));
		sa.sa_mask = mask;
		sa.sa_handler = guard_sighandlers[sh].hand;
		if (::sigaction(guard_sighandlers[sh].sig, &sa, NULL) < 0 ) {
			LOG_TO_FILE_2("!setting action against signal (%d) has been failed : %s\n", guard_sighandlers[sh].sig, strerror(errno));
			return -1;
		}
	}
	
	if (::sigprocmask(SIG_UNBLOCK, &mask, NULL) < 0 ) {
		LOG_TO_FILE_1("!setting process signal mask has been failed : %s\n", strerror(errno));
		return -2;
	}
	return 1;
}

int init_signals(int flag)
{
	int ret_val = 0;
	
	switch(flag) {
		case PARENT: 
			ret_val = set_sighandlers_guard();
			break;
		case CHILD:
			ret_val = set_sighandlers_child();
			break;
		default:
			ret_val = -1;
	}
	return ret_val;
}
