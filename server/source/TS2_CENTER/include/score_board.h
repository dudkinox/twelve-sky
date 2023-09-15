/****************************************************************************

   CTSD : Common Transaction Service

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: score_board.h,v 1.4 2007/06/11 13:21:21 sean Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/


#ifndef __SCORE_BOARD_H__
#define __SCORE_BOARD_H__

#include <iostream>
#include <list>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

#include "log.h"
#include "global.h"
#include "exception.h"
#include "semaphore.h"

using namespace std;
using namespace SDLog;
using namespace SDSocket;


#define MaxSizeOfChldProcList 100

typedef struct _scoreboard_child {
	pid_t pid;
	time_t created_time;
	time_t updated_time;
	unsigned long accepted_request;
	unsigned long replied_request;
	unsigned int num_of_thread;
	unsigned int proc_status;
} scoreboard_child_t;

typedef struct _scoreboard {
	scoreboard_child_t child_proc_list[MaxSizeOfChldProcList];
	time_t created_time;
	time_t updated_time;
	unsigned int tno_assigned_slot;
	unsigned int global_loop;
	unsigned int shutdown_rsn;
} scoreboard_t;

class CScoreBoard
{
public:

	enum eProcStatus {
		DEAD = 0,
		ALIVE = 1
	};

	enum eLoopStatus {
		STOP = 0,
		RUNNING = 1
	};

	enum eShutDownRsn {
		SHUTDOWN_IMMIDIATELY    = 1,
		SHUTDOWN_GRACEFULLY     = 2,
		SHUTDOWN_RELOAD_MODULES = 3
	};

	CScoreBoard(void)  { m_shm_id = -1; m_score_board_ptr = NULL; }
	~CScoreBoard(void) throw();
	void init_score_board(void) throw(Exception);
	void init_slot(unsigned int num_of_slots) throw(Exception);
	void update_slot(pid_t pid);

	int get_proc_updated(pid_t pid);
	int get_thread_updated(pid_t pid);
	int mark_proc_dead(pid_t pid);

	unsigned int maintenance_child_proc(unsigned int proc_work_time);
	inline unsigned int is_spinning(void) { return m_score_board_ptr->global_loop; }
	void let_cm_stop(unsigned int shutdown_rsn); 
	void let_cm_spin(void); 
	unsigned int get_cm_work_status(void);
#ifdef __DEBUG_SCORE_BOARD__
	void dump_score_board_out(void);
#endif
	void destroy_score_board(void) throw(Exception);
	void destroy_score_board_mutex(void);

private:
	int m_shm_id;
	static Semaphore thread_lock;
	scoreboard_t *m_score_board_ptr;
};
#endif 
