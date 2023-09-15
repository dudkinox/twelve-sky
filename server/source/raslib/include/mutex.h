/****************************************************************************

  Ras library FreeBSD Version

  CopyRight(C) 2003 Sean Kim All Rights reserved.
Author : Sean Kim (sean@0x31.com)

$Id: mutex.h,v 1.1 2007/08/18 06:09:21 sean Exp $

Redistribution and use in source and binary forms, with or with out
modification, are not permitted without author's permission.

 ****************************************************************************/

#ifndef __MUTEX_H__
#define __MUTEX_H__

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <pthread.h>
#include "exception.h"


namespace SDSocket
{
class Semaphore
{
public:
		// init semaphore counter with 0.
		Semaphore() throw (Exception);
		~Semaphore() throw();

		int operator++() throw (Exception);
		int operator--() throw (Exception);
		int operator-=(int dec) throw (Exception);
		operator int() throw (Exception);

		void mutexLock() throw (Exception);
		void mutexUnlock() throw (Exception);
		void condWait() throw (Exception);
		void condSignal() throw (Exception);

	private:
		int value;
		pthread_mutex_t mutex;
		pthread_cond_t cond; 
};

class Mutex
{
	public:
		void init_mutex(key_t sem_key, int sem_flag) throw (Exception);
		void destroy_mutex(void) throw (Exception);
		void lock() throw (Exception);
		void unlock() throw (Exception);

	private:
		int m_sem_id;
		bool MutexSetFlag;
		struct sembuf sem_on;
		struct sembuf sem_off;
};
}
#endif
