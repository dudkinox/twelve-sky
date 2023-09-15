/****************************************************************************

   CTSD : Common Transaction Service 

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: tpool_q.h,v 1.2 2007/08/03 05:24:03 sean Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/
#ifndef __TPOOL_H__
#define __TPOOL_H__

typedef struct tpool_work {
	void               (*routine)(void *arg);
	void                *arg;
	struct tpool_work   *next;
} tpool_work_t;

typedef struct tpool {
	/* pool characteristics */
	int                 num_threads;
	int                 max_queue_size;
	int                 do_not_block_when_full;
	/* pool state */
	pthread_t           *threads;
	int                 cur_queue_size;
	tpool_work_t        *queue_head;
	tpool_work_t        *queue_tail;
	int                 queue_closed;
	int                 shutdown;
	/* pool synchronization */
	pthread_mutex_t     queue_lock;
	pthread_cond_t      queue_not_empty;
	pthread_cond_t      queue_not_full;
	pthread_cond_t      queue_empty;
} *tpool_t;

int tpool_init(tpool_t *tpoolp, int num_threads, int max_queue_size, int do_not_block_when_full);
int tpool_add_work(tpool_t tpool,void (*routine)(void *arg), void *arg);
int tpool_destroy(tpool_t tpool, int finish);
#endif
