/****************************************************************************

   CTSD : Common Transaction Service

   CopyRight(C) 2003 Sean Kim All Rights ret_valerved.
   Author : Sean Kim (sean@0x31.com)

   $Id: score_board.cpp,v 1.8 2007/07/16 11:30:03 sean Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/


#include <iostream>
#include <string>
#include <exception>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

#include "local.h"
#include "global.h"
#include "score_board.h"

using namespace std;
using namespace SDLog;
using namespace SDSocket;

Semaphore CScoreBoard::thread_lock;
CScoreBoard::~CScoreBoard(void) throw()
{
}

void CScoreBoard::destroy_score_board_mutex(void) 
{
	//m_score_board_ptr->scoreboard_lock.destroy_mutex();
}

void CScoreBoard::let_cm_stop(unsigned int shutdown_rsn) 
{
	try {
		//m_score_board_ptr->scoreboard_lock.lock();
		m_score_board_ptr->global_loop = STOP;
		m_score_board_ptr->shutdown_rsn = shutdown_rsn;
		//m_score_board_ptr->scoreboard_lock.unlock();
	} catch (Exception &e) {
		throw Exception(e.getMethod(), e.getMessage(), e.getCode());
	}
}

void CScoreBoard::let_cm_spin(void) 
{
	try {
		//m_score_board_ptr->scoreboard_lock.lock();
		m_score_board_ptr->global_loop = RUNNING;
		//m_score_board_ptr->scoreboard_lock.unlock();
	} catch (Exception &e) {
		throw Exception(e.getMethod(), e.getMessage(), e.getCode());
	}
}

unsigned int CScoreBoard::get_cm_work_status(void) 
{
	unsigned int work_status = 0;
	try {
		//m_score_board_ptr->scoreboard_lock.lock();
		work_status = m_score_board_ptr->global_loop;
		//m_score_board_ptr->scoreboard_lock.unlock();
	} catch (Exception &e) {
		throw Exception(e.getMethod(), e.getMessage(), e.getCode());
	}
	return work_status;
}

void CScoreBoard::init_score_board(void) throw (Exception)
{
	try { 
		/* shared memory for scoreboard ! */
		m_shm_id = ::shmget(IPC_PRIVATE, sizeof(scoreboard_t), IPC_CREAT | 0600);
		if(m_shm_id < 0) {
			if(errno == ENOSYS) {
				throw Exception("CScoreBoard::init_score_board", "This OS or Kernel does not support SYSV IPC", -1);
			} else {
				throw Exception("CScoreBoard::init_score_board", ::strerror(errno), -2);
			}
		}

		m_score_board_ptr = (scoreboard_t *)::shmat(m_shm_id, 0, 0);
		if(m_score_board_ptr == (scoreboard_t *)(-1)) {
			throw Exception("CScoreBoard::init_score_board", ::strerror(errno), -3);
		}
		this->destroy_score_board();
		::memset(m_score_board_ptr, 0, sizeof(scoreboard_t));

		m_score_board_ptr->global_loop = CScoreBoard::RUNNING;
		m_score_board_ptr->tno_assigned_slot = 0;
		m_score_board_ptr->created_time = ::time(NULL);
		m_score_board_ptr->updated_time = ::time(NULL);
		m_score_board_ptr->shutdown_rsn = 0;
		//m_score_board_ptr->scoreboard_lock.init_mutex(IPC_PRIVATE, IPC_CREAT | 0666);
	} catch (Exception &e) {
		throw Exception(e.getMethod(), e.getMessage(), e.getCode());
	}
}

void CScoreBoard::destroy_score_board(void) throw (Exception)
{
	int ret_val = 0;

	try {
		if(m_shm_id > 0) {
			ret_val = ::shmctl(m_shm_id, IPC_RMID, NULL);
			if(ret_val < 0) {
				throw Exception("CScoreBoard::destroy_score_board", ::strerror(errno), -1);
			}
		}
	} catch (Exception &e) {
		throw Exception(e.getMethod(), e.getMessage(), e.getCode());
	}
}

void CScoreBoard::init_slot(unsigned int num_of_slots) throw(Exception)
{
	if(num_of_slots > MaxSizeOfChldProcList) {
		throw Exception("CScoreBoard::init_slot", "Requested number of slots is exceeded limit of process list", -1);
	}

	try {
		for(unsigned int i = 0; i < num_of_slots; i++) {
			::memset((scoreboard_child_t*)&m_score_board_ptr->child_proc_list[i], 0, sizeof(scoreboard_child_t));

			m_score_board_ptr->child_proc_list[i].pid = (pid_t)0;
			m_score_board_ptr->child_proc_list[i].created_time = (time_t)0;
			m_score_board_ptr->child_proc_list[i].updated_time = (time_t)0;
			m_score_board_ptr->child_proc_list[i].proc_status = CScoreBoard::DEAD;
			m_score_board_ptr->child_proc_list[i].replied_request = (long long)0;
		}
	} catch (Exception &e) {
		throw Exception(e.getMethod(), e.getMessage(), e.getCode());
	} catch (...) {
		throw Exception("CScoreBoard::init_slot", "Unknown exception has been thrown", -2);
	}
	m_score_board_ptr->tno_assigned_slot = num_of_slots;
}

#ifdef __DEBUG_SCORE_BOARD__
void CScoreBoard::dump_score_board_out(void)
{
	cerr << "=========================================================" << endl;
	cerr << "                  START TO DUMP SCOREBOARD               " << endl;
	cerr << "=========================================================" << endl;
	cerr << "parent->created_time = " << m_score_board_ptr->created_time << endl;
	cerr << "parent->updated_time = " << m_score_board_ptr->updated_time << endl;
	cerr << "parent->tno_assigned_slot = " << m_score_board_ptr->tno_assigned_slot << endl;
	cerr << "parent->global_loop = " << m_score_board_ptr->global_loop << endl;

	for(unsigned int i = 0; i < m_score_board_ptr->tno_assigned_slot; i++) {
		cerr << "=========================================================" << endl;
		cerr << "pid = " << m_score_board_ptr->child_proc_list[i].pid << endl;
		cerr << "created_time = " << m_score_board_ptr->child_proc_list[i].created_time << endl;
		cerr << "updated_time = " << m_score_board_ptr->child_proc_list[i].updated_time << endl;
		cerr << "replied_request = " << m_score_board_ptr->child_proc_list[i].replied_request << endl;
		cerr << "proc_status = " << m_score_board_ptr->child_proc_list[i].proc_status << endl;
	}
}
#endif

void CScoreBoard::update_slot(pid_t pid)
{
	unsigned int i = 0;

	try {
		//m_score_board_ptr->scoreboard_lock.lock();
		for(i = 0; i< m_score_board_ptr->tno_assigned_slot; i++) {
			if((m_score_board_ptr->child_proc_list[i].proc_status == CScoreBoard::DEAD) ||
					(m_score_board_ptr->child_proc_list[i].pid == 0)) {
				m_score_board_ptr->child_proc_list[i].pid = pid;
				m_score_board_ptr->child_proc_list[i].created_time = ::time(NULL);
				m_score_board_ptr->child_proc_list[i].updated_time = ::time(NULL);
				m_score_board_ptr->child_proc_list[i].proc_status = CScoreBoard::RUNNING;
				m_score_board_ptr->child_proc_list[i].replied_request = (long long)0;
				break;
			}
		}
		//m_score_board_ptr->scoreboard_lock.unlock();
	} catch (Exception &e) {
		throw Exception(e.getMethod(), e.getMessage(), e.getCode());
	}
}

int CScoreBoard::get_proc_updated(pid_t pid)
{
	unsigned int i = 0;
	unsigned int ret_val = 0;

	try {
		//m_score_board_ptr->scoreboard_lock.lock();
		for(i = 0; i< m_score_board_ptr->tno_assigned_slot; i++) {
			if(m_score_board_ptr->child_proc_list[i].pid == pid) {
				ret_val = 1;
				m_score_board_ptr->child_proc_list[i].updated_time = ::time(NULL);
				m_score_board_ptr->child_proc_list[i].accepted_request += 1;
				break;
			}
		}
		//m_score_board_ptr->scoreboard_lock.unlock();
	} catch (Exception &e) {
		throw Exception(e.getMethod(), e.getMessage(), e.getCode());
	}

	return ret_val;
}

int CScoreBoard::get_thread_updated(pid_t pid)
{
	unsigned int i = 0;
	unsigned int ret_val = 0;

	--thread_lock;
	for(i = 0; i< m_score_board_ptr->tno_assigned_slot; i++) {
		if(m_score_board_ptr->child_proc_list[i].pid == pid) {
			ret_val = 1;
			m_score_board_ptr->child_proc_list[i].updated_time = ::time(NULL);
			m_score_board_ptr->child_proc_list[i].replied_request += 1;
			break;
		}
	}
	++thread_lock;

	return ret_val;
}

int CScoreBoard::mark_proc_dead(pid_t pid)
{
	unsigned int i = 0;
	unsigned int ret_val = 0;

	try {
		//m_score_board_ptr->scoreboard_lock.lock();
		for(i = 0; i< m_score_board_ptr->tno_assigned_slot; i++) {
			if(m_score_board_ptr->child_proc_list[i].pid == pid) {
				ret_val = 1;
				m_score_board_ptr->child_proc_list[i].pid = 0;
				m_score_board_ptr->child_proc_list[i].proc_status = CScoreBoard::DEAD;
				break;
			}
		}
		//m_score_board_ptr->scoreboard_lock.unlock();
	} catch (Exception &e) {
		throw Exception(e.getMethod(), e.getMessage(), e.getCode());
	}

	return ret_val;
}

unsigned int CScoreBoard::maintenance_child_proc(unsigned int proc_work_time)
{
	unsigned int i = 0;
	time_t current_time = 0;
	unsigned int num_of_proc = 0;

	try {
#ifdef __DEBUG_SCORE_BOARD__	
		dump_score_board_out();
#endif
		//m_score_board_ptr->scoreboard_lock.lock();
		current_time = ::time(NULL);
		for(i = 0; i< m_score_board_ptr->tno_assigned_slot; i++) {
			if((m_score_board_ptr->child_proc_list[i].proc_status == CScoreBoard::RUNNING)
					&& (m_score_board_ptr->child_proc_list[i].updated_time + (time_t)proc_work_time <= current_time)
					&& (g_env.tno_proc_maintenance_time != 0)) {
				if(m_score_board_ptr->child_proc_list[i].replied_request >= (uint)g_env.tno_proc_maintenance_rc) { 
					LOG_TO_FILE_1("![%d] will be kiiled by the scoreboard maintainer.\n", m_score_board_ptr->child_proc_list[i].pid);
                    ::kill(m_score_board_ptr->child_proc_list[i].pid, SIGKILL);
                    LOG_TO_FILE_1("![%d] has been killed by the scoreboard maintainer.\n", m_score_board_ptr->child_proc_list[i].pid);
                    m_score_board_ptr->child_proc_list[i].pid = (pid_t)0;
                    m_score_board_ptr->child_proc_list[i].proc_status = CScoreBoard::DEAD;
                }
			} else {
				if(m_score_board_ptr->child_proc_list[i].pid != 0) num_of_proc++;
			}
		}
		//m_score_board_ptr->scoreboard_lock.unlock();
	} catch (Exception &e) {
		throw Exception(e.getMethod(), e.getMessage(), e.getCode());
	}

	return num_of_proc;
}

