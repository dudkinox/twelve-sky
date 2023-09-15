/****************************************************************************

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: tpool_q.cpp,v 1.2 2007/08/03 05:24:03 sean Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <syslog.h>

#include <pthread.h>
#include "tpool.h"
#include "global.h"

void *tpool_thread(void *);

int tpool_init(tpool_t *tpoolp, int num_worker_threads, int max_queue_size, int do_not_block_when_full)
{
	int i, rtn;
	tpool_t tpool;
	 
	/* allocate a pool data structure */ 
	tpool = new (struct tpool);
	if(!tpool) {
		return -1;
	}

	/* initialize th fields */
	tpool->num_threads = num_worker_threads;
	tpool->max_queue_size = max_queue_size;
	tpool->do_not_block_when_full = do_not_block_when_full;

	tpool->threads = new pthread_t[num_worker_threads];
	if(!tpool->threads) {
		return -2;
	}

	tpool->cur_queue_size = 0;
	tpool->queue_head = NULL; 
	tpool->queue_tail = NULL;
	tpool->queue_closed = 0;
	tpool->shutdown = 0; 

	LOG_TO_FILE_1("![%d] Initializing a thread pool...\n", ::getpid());
	if ((rtn = ::pthread_mutex_init(&(tpool->queue_lock), NULL)) != 0) {
		return -3;
	}
	if ((rtn = ::pthread_cond_init(&(tpool->queue_not_empty), NULL)) != 0) {
		return -4;
	}
	if ((rtn = ::pthread_cond_init(&(tpool->queue_not_full), NULL)) != 0) {
		return -5;
	}
	if ((rtn = ::pthread_cond_init(&(tpool->queue_empty), NULL)) != 0) {
		return -6;
	}

	/* create threads */
	for (i = 0; i != num_worker_threads; i++) {
		if ((rtn = ::pthread_create( &(tpool->threads[i]), NULL, tpool_thread, (void *)tpool)) != 0) {
			return -7;
		}
		LOG_TO_FILE_3("![%d] creating [ %d / %d ] thread : [SUCCESS] \n", ::getpid(), (i + 1), num_worker_threads);
	}
	*tpoolp = tpool;
	return 1;
}

int tpool_add_work(tpool_t tpool, void (*routine)(void *arg), void *arg)
{
	int rtn;
	tpool_work_t *workp = NULL;

	if ((rtn = ::pthread_mutex_lock(&(tpool->queue_lock))) != 0) {
		return -1;
	}

	/* no space and this caller doesn't want to wait */
	if ((tpool->cur_queue_size == tpool->max_queue_size) && tpool->do_not_block_when_full) {
		if ((rtn = ::pthread_mutex_unlock(&(tpool->queue_lock))) != 0) {
			return -2;
		}
		return -3;
	}

	while( (tpool->cur_queue_size == tpool->max_queue_size) && (!(tpool->shutdown || tpool->queue_closed))) {
		if ((rtn = ::pthread_cond_wait(&(tpool->queue_not_full), &(tpool->queue_lock))) != 0) {
			return -4;
		}
	}

	/* the pool is in the process of being destroyed */
	if (tpool->shutdown || tpool->queue_closed) {
		if ((rtn = ::pthread_mutex_unlock(&(tpool->queue_lock))) != 0) {
			return -5;
		}
		return -6;
	}

	workp = new tpool_work_t; 
	if(!workp) {
		return -7;
	}

	workp->routine = routine;
	workp->arg = arg;
	workp->next = NULL;

	if (tpool->cur_queue_size == 0) {
		tpool->queue_tail = tpool->queue_head = workp;
		if ((rtn = ::pthread_cond_broadcast(&(tpool->queue_not_empty))) != 0) {
			return -8;
		}

	} else {
		tpool->queue_tail->next = workp;
		tpool->queue_tail = workp;
	}

	tpool->cur_queue_size++; 

	if ((rtn = ::pthread_mutex_unlock(&(tpool->queue_lock))) != 0) {
		return -9;
	}

	return 1;
}

int tpool_destroy(tpool_t tpool, int finish)
{
	int	i, rtn;
	tpool_work_t *cur_nodep;

	if ((rtn = ::pthread_mutex_lock(&(tpool->queue_lock))) != 0) {
		return -1;
	}

	/* Is a shutdown already in progress? */
	if (tpool->queue_closed || tpool->shutdown) {
		if ((rtn = ::pthread_mutex_unlock(&(tpool->queue_lock))) != 0) {
			return -2;
		}
		return 0;
	}

	tpool->queue_closed = 1;

	/* If the finish flag is set, wait for workers to drain queue */ 
	if (finish == 1) {
		while (tpool->cur_queue_size != 0) {
			if ((rtn = ::pthread_cond_wait(&(tpool->queue_empty), &(tpool->queue_lock))) != 0) {
				return -3;
			}
		}
	}

	tpool->shutdown = 1;

	if ((rtn = ::pthread_mutex_unlock(&(tpool->queue_lock))) != 0) {
		return -4;
	}

	/* Wake up any workers so they recheck shutdown flag */
	if ((rtn = ::pthread_cond_broadcast(&(tpool->queue_not_empty))) != 0) {
		return -5;
	}
	if ((rtn = ::pthread_cond_broadcast(&(tpool->queue_not_full))) != 0) {
		return -6;
	}

	/* Wait for workers to exit */
	for(i=0; i < tpool->num_threads; i++) {
		if ((rtn = ::pthread_join(tpool->threads[i],NULL)) != 0) {
			return -7;
		}
	}

	/* Now free pool structures */
	if(tpool->threads) {
		delete tpool->threads;
	}

	while(tpool->queue_head != NULL) {
		cur_nodep = tpool->queue_head->next; 
		tpool->queue_head = tpool->queue_head->next;
		delete cur_nodep;
	}

	if(tpool) {
		delete tpool;
	}

	return 1;
}

void *tpool_thread(void *arg)
{
	int rtn;
	tpool_t tpool = (tpool_t)arg; 
	tpool_work_t *my_workp;
	
	for(;;) {

		/* Check queue for work */ 
		if ((rtn = ::pthread_mutex_lock(&(tpool->queue_lock))) != 0) {
			LOG_TO_FILE_2("Thread pool: %s (error num = %d)",strerror(errno), errno);
			return NULL;
		}

		while ((tpool->cur_queue_size == 0) && (!tpool->shutdown)) {
			if ((rtn = pthread_cond_wait(&(tpool->queue_not_empty), &(tpool->queue_lock))) != 0) {

			}
		}

		if (tpool->shutdown == 1) {
			if ((rtn = pthread_mutex_unlock(&(tpool->queue_lock))) != 0) {
				LOG_TO_FILE_2("Thread pool: %s (error num = %d)",strerror(errno), errno);
				return NULL;
			}
			::pthread_exit(NULL);
		}

		/* Get to work, dequeue the next item */ 
		my_workp = tpool->queue_head;
		tpool->cur_queue_size--;
		if (tpool->cur_queue_size == 0) {
			tpool->queue_head = tpool->queue_tail = NULL;
		} else {
			tpool->queue_head = my_workp->next;
		}
	 
		/* Handle waiting add_work threads */
		if ((!tpool->do_not_block_when_full) && (tpool->cur_queue_size ==(tpool->max_queue_size - 1))) {
			if ((rtn = pthread_cond_broadcast(&(tpool->queue_not_full))) != 0) {
				LOG_TO_FILE_2("Thread pool: %s (error num = %d)",strerror(errno), errno);
			}
		}

		/* Handle waiting destroyer threads */
		if (tpool->cur_queue_size == 0) {
			if ((rtn = pthread_cond_signal(&(tpool->queue_empty))) != 0) {
				LOG_TO_FILE_2("Thread pool: %s (error num = %d)",strerror(errno), errno);
			}
		}
		if ((rtn = pthread_mutex_unlock(&(tpool->queue_lock))) != 0) {
			LOG_TO_FILE_2("Thread pool: %s (error num = %d)",strerror(errno), errno);
		}
			
		/* Do this work item */
		(*(my_workp->routine))(my_workp->arg);
		if(my_workp) {
			delete my_workp;
		}
	} 
	
	return(NULL);		
}

