/****************************************************************************

   CTSD : Common Transaction Service

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: tpool.cpp,v 1.14 2007/11/20 08:19:20 wglee Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#include "tpool.h"
#include "local.h"
#include "global.h"
#include "cts_util.h"

#include <sys/types.h>
#include <signal.h>

#define DEFAULT_ADD_JOB_TIME_OUT  3000 // in usec

Semaphore CThreadPool::thread_lock;
CThreadPool::CThreadPool(unsigned int num_threads)
{
	int ret_val = 0;

    m_num_spare_threads = (unsigned int)0;
    m_num_temp_threads = (unsigned int)0;
	set_num_of_threads(num_threads);
	::sem_init(&m_sem_tpool_map_in_use, 0, 1);
	try {
		ret_val = init_thread_pool();
		if(!ret_val) {
			throw Exception("Thread Pool", "initialization of a thread pool has been failed", -1);
		}
	} catch (Exception &e) {
		throw Exception(e.getMethod(), e.getMessage(), e.getCode());
	}

}

CThreadPool::~CThreadPool(void) 
{
	::sem_destroy(&m_sem_tpool_map_in_use);
	this->destroy();
}

void CThreadPool::destroy(void) 
{
	tpool_map_iterator iter;

	for(iter = m_tpool_map.begin(); iter != m_tpool_map.end(); ++iter) {
		iter->second->thread_stat = STOPPED;
		::sem_post(&(iter->second->thd_is_ready));
		::pthread_join(iter->first, NULL);
	}

	for(iter = m_tpool_map.begin(); iter != m_tpool_map.end(); ++iter) {
		if(iter->second) {
			delete iter->second;
		}
	}
}

int CThreadPool::add_job(void (*routine)(void *arg_ptr), void *arg_ptr)
{
	int ret_val = 1;
	int ret_kill = 1;
	bool is_found = false;
	tpool_map_iterator iter;

	::sem_wait(&m_sem_tpool_map_in_use);
	for(iter = m_tpool_map.begin(); iter != m_tpool_map.end(); ++iter) {
		if(iter->second->thread_stat == WAITING && iter->second->thread_type != TEMPORARY) {

			ret_kill = pthread_kill(iter->first, 0);
			if(ret_kill != 0) continue;

			iter->second->thread_job.routine = routine;
			iter->second->thread_job.arg_ptr = arg_ptr;
			iter->second->thread_stat = RUNNING;
			iter->second->last_run_at = ::time(NULL);
			is_found = true;
		
			::sem_post(&(iter->second->thd_is_ready));
			break;
		}
	}

	for(iter = m_tpool_map.begin(); 
			(!is_found) && m_num_temp_threads > 0 && iter != m_tpool_map.end(); ++iter) {
		if(iter->second->thread_stat == WAITING) {

			ret_kill = pthread_kill(iter->first, 0);
			if(ret_kill != 0) continue;

			iter->second->thread_job.routine = routine;
			iter->second->thread_job.arg_ptr = arg_ptr;
			iter->second->thread_stat = RUNNING;
			iter->second->last_run_at = ::time(NULL);
			is_found = true;

			::sem_post(&(iter->second->thd_is_ready));
			break;
		}
	}

	for( ; ; ) {
		if((is_found) || (ret_val <= 0)) {
			break;
		}
		if((!is_found) && (ret_val > 0))  {
			pthread_t thread_id = 0;
			eThreadType thread_type = STARTUP;

			if(m_num_spare_threads 
					< (unsigned int)(g_env.tno_min_spare_worker_thread 
									+ g_env.tno_max_spare_worker_thread)) {
				thread_type = SPARE;
			} else {
				if(m_num_temp_threads+m_num_spare_threads+m_num_threads 
						< (unsigned int)g_env.tno_max_allowed_worker_thread) {
					thread_type = TEMPORARY;
				}
				else {
					ret_val = -5; 
					break;
				}
			}

			int ret_1 = add_single_thread(thread_type, &thread_id);
			if(ret_1 < 0) {
				ret_val = -2; 
				break;
			} else {
				if(thread_type == SPARE) {
					m_num_spare_threads++;
				} else if(thread_type == TEMPORARY) {
					m_num_temp_threads++;
				} else {
					LOG_TO_FILE_1("!illigal thread type [%d]\n", thread_type); 
				}
			}

			for(iter = m_tpool_map.begin();iter != m_tpool_map.end();++iter) {
				if(::pthread_equal(iter->first, thread_id)) {
					is_found = true;
					break;
				}
			}

			if(is_found) {
				ret_val = 1;
				iter->second->thread_job.routine = routine;
				iter->second->thread_job.arg_ptr = arg_ptr;
				iter->second->thread_stat = RUNNING;
				iter->second->last_run_at = ::time(NULL);
				is_found = true;

				::sem_post(&(iter->second->thd_is_ready));
				break;
			} else {
				ret_val = -4;
				break;
			}
		} 
	}
    
	::sem_post(&m_sem_tpool_map_in_use);
	return ((is_found) && (ret_val > 0)) ? 1: ret_val;
}

int CThreadPool::add_single_thread(eThreadType thread_type, pthread_t *thread_id_ptr, bool to_be_inserted)
{ 
	int ret_val = 0;
	sigset_t sig_set;
	pthread_attr_t attr;
	size_t      stacksize = 0;
	tpool_map_iterator iter;

	tpool_t *tpool_thread_ptr = new tpool_t;
	tpool_thread_ptr->thread_id = *thread_id_ptr;
	tpool_thread_ptr->thread_job.routine = NULL;
	tpool_thread_ptr->thread_job.arg_ptr = NULL;
	tpool_thread_ptr->thread_stat = WAITING;
    tpool_thread_ptr->thread_type = thread_type;
	tpool_thread_ptr->last_ack_at = ::time(NULL);
    tpool_thread_ptr->last_run_at = (time_t)0;
    tpool_thread_ptr->created_at = ::time(NULL);

	::sem_init(&tpool_thread_ptr->thd_is_ready, 0, 0);

	::pthread_attr_init(&attr);
	::pthread_attr_getstacksize(&attr, &stacksize);
	::pthread_attr_setstacksize(&attr, (size_t)stacksize*4);
	::pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	::sigaddset(&sig_set, SIGPIPE);
	::pthread_sigmask(SIG_BLOCK, &sig_set, NULL);

	--thread_lock;
	ret_val = ::pthread_create(thread_id_ptr, &attr, CThreadPool::init_single_thread, this);
	::pthread_attr_destroy(&attr);

	if(ret_val) {
		++thread_lock;
		delete tpool_thread_ptr;
		return -3;
	}

	if(to_be_inserted) {
		iter = m_tpool_map.find(*thread_id_ptr);
//		if(iter != NULL) LOG_TO_FILE_1("!duplication thread id [%d] \n", *thread_id_ptr);  
		m_tpool_map.insert(tpool_map_type(*thread_id_ptr, tpool_thread_ptr));
	}
	++thread_lock;

    return 1;
}

int CThreadPool::mtpool_thread(void)
{
	int ret_val = 0;
	unsigned int i = 0;
	pthread_t thread_id;
	tpool_map_iterator iter;
	unsigned int tbrt_sut_count = 0;
	unsigned int tbrt_st_count = 0;
	unsigned int tbrt_tt_count = 0;
	thread_list_t tl_to_be_removed;
	thread_list_t::iterator c_iter;
	time_t c_time = ::time(NULL);

	::sem_wait(&m_sem_tpool_map_in_use);
	for(iter = m_tpool_map.begin();iter != m_tpool_map.end();++iter) {
//		fprintf(stderr, "thread id = %d, thread_stat = %d, thread_type = %d\n", 
//				iter->first, iter->second->thread_stat, iter->second->thread_type);
		ret_val = pthread_kill(iter->first, 0);
		if(ret_val != 0) {
			tl_to_be_removed.push_back(iter->first);
			LOG_TO_FILE_2("!worker thread : type[%d] %d is dead. clean this up from the list.\n", 
					iter->second->thread_type, iter->first); 
			if(iter->second) {
				delete iter->second;
				iter->second = NULL;
			}
		} 
		else 
		{
			if(iter->second->thread_type == STARTUP) {
				tbrt_sut_count++;
			} else if(iter->second->thread_type == SPARE) {
				tbrt_st_count++;
			} else if(iter->second->thread_type == TEMPORARY) {
				tbrt_tt_count++;
			}
		}

		if((ret_val == 0) && (iter->second->thread_type == TEMPORARY)) {
//			fprintf(stderr, "thread id = %d, last_run_at = %ld, c_time = %ld, gap = %ld\n",
//					        iter->first, iter->second->last_run_at, c_time, (long)(c_time - iter->second->last_run_at));
			if((c_time - iter->second->last_run_at) > g_env.tno_tmp_spare_worker_thread_to) {
				iter->second->thread_stat = STOPPED;
				::sem_post(&(iter->second->thd_is_ready));
				tl_to_be_removed.push_back(iter->first);
				LOG_TO_FILE_1("!temp worker thread : %d killed. clean this up from the list.\n", iter->first); 
				if(iter->second) {
					delete iter->second;
					iter->second = NULL;
				}
				tbrt_tt_count--;
			}
		}
	}

    for(c_iter = tl_to_be_removed.begin(); c_iter != tl_to_be_removed.end(); ++c_iter) {
		m_tpool_map.erase(*c_iter);
    }

	m_num_temp_threads = tbrt_tt_count;

	for(i = 0; i < m_num_threads - tbrt_sut_count; i++ ) {
		ret_val = add_single_thread(STARTUP, &thread_id);
		if(ret_val < 0) {
			break;
		}
		LOG_TO_FILE_2("!add_single_thread STARTUP : %d :: %d\n", i, thread_id); 
	}

	if(ret_val > 0) {
		for(i = 0; i < m_num_spare_threads - tbrt_st_count; i++ ) {
			ret_val = add_single_thread(SPARE, &thread_id);
			if(ret_val < 0) {
				break;
			}
			LOG_TO_FILE_2("!add_single_thread SPARE : %d :: %d\n", i, thread_id); 
		}
	}

	::sem_post(&m_sem_tpool_map_in_use);

	return ret_val;
}

CThreadPool::CThreadPoolMThread::CThreadPoolMThread(CThreadPool *tp_obj_ptr) 
{
	m_tp_obj_ptr = tp_obj_ptr;
}

void CThreadPool::CThreadPoolMThread::run(void) 
{
	for ( ; ; ) {
		m_tp_obj_ptr->mtpool_thread();
		spin_wait(DEFAULT_TPOOL_MTIME, 0);
	}
}

int CThreadPool::init_thread_pool(void)
{
    int ret_val = 0;
	pthread_t thread_id;
	unsigned int t_count = 0;
    eThreadType thread_type;
    
	for(t_count = 0; t_count < (m_num_threads + g_env.tno_min_spare_worker_thread); t_count++) {
        if(t_count < m_num_threads) {
            thread_type = STARTUP;
        } else {
            thread_type = SPARE;
            m_num_spare_threads++;
        }
        ret_val = add_single_thread(thread_type, &thread_id);
        if(ret_val < 0) {
            return ret_val;
        }
	} 
	return 1;
}

void *CThreadPool::init_single_thread(void *arg_ptr)
{
	CThreadPool *tpool_ptr = reinterpret_cast<CThreadPool*>(arg_ptr);

	tpool_ptr->run();
	return NULL;
}

int CThreadPool::run(void)
{
	bool is_found = false;
	tpool_map_iterator iter;

	--thread_lock;
	for(iter = m_tpool_map.begin();iter != m_tpool_map.end();++iter) {
		if(::pthread_equal(iter->first, ::pthread_self())) {
			is_found = true;
			break;
		}
	}
	++thread_lock;

	if(!is_found) {
		LOG_TO_FILE("!Thread pool: found bug !\n");
		return -1;
	}

	for( ; ; ) {
		::sem_wait(&(iter->second->thd_is_ready));
		if(iter->second->thread_stat == STOPPED) {
			::pthread_exit(NULL);
		} 
		iter->second->thread_stat = RUNNING;
		if(iter->second->thread_job.routine) {
#ifdef __DEBUG_THREAD_POOL__
			DEBUG2("iter->second->thread_job.routine = 0x%x, iter->second->thread_job.arg_ptr = 0x%x\n"
					, iter->second->thread_job.routine, iter->second->thread_job.arg_ptr);
#endif
			(*(iter->second->thread_job.routine))(iter->second->thread_job.arg_ptr);
			iter->second->thread_job.routine = NULL;
			iter->second->thread_job.arg_ptr = NULL;
			iter->second->thread_stat = WAITING;
		} else {
			iter->second->thread_job.routine = NULL;
			iter->second->thread_job.arg_ptr = NULL;
			iter->second->thread_stat = WAITING;
			LOG_TO_FILE("!Thread pool: found bug ! (2)\n");
		}
	}
	return -6;
}
