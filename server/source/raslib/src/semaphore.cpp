/****************************************************************************

  Ras library FreeBSD Version

  CopyRight(C) 2003 Sean Kim All Rights reserved.
Author : Sean Kim (sean@0x31.com)

$Id: semaphore.cpp,v 1.10 2007/08/21 07:12:53 sean Exp $

Redistribution and use in source and binary forms, with or with out
modification, are not permitted without author's permission.

 ****************************************************************************/

#include "mutex.h"

#ifdef __USE_SEMUN__
union semun {
	int val;                        /* value for SETVAL */
	struct semid_ds *buf;           /* buffer for IPC_STAT & IPC_SET */
	unsigned short *array;          /* array for GETALL & SETALL */
	struct seminfo *__buf;          /* buffer for IPC_INFO */
	void *__pad;
};  
#endif

namespace SDSocket
{
	Semaphore::Semaphore() throw (Exception)
	{
		int ret_val=0;
		ret_val = pthread_mutex_init(&mutex, 0);
		if(ret_val < 0)
			throw Exception("Semaphore::Semaphore()", "Can't init mutex", ret_val);

		ret_val = pthread_cond_init(&cond, 0);
		if(ret_val < 0)
			throw Exception("Semaphore::Semaphore()", "Can't init condition", ret_val);

		value = 0;
	}

	Semaphore::~Semaphore() throw()
	{
		pthread_mutex_destroy(&mutex);
		pthread_cond_destroy(&cond);
	}

	int Semaphore::operator++() throw (Exception)
	{
		int valueAfterOp;

		mutexLock();
		++value;
		valueAfterOp = value;
		mutexUnlock();

		condSignal();

		return valueAfterOp;
	}

	int Semaphore::operator--() throw (Exception)
	{
		int valueAfterOp;

		mutexLock();
		while(value < 0)
			condWait();

		--value;
		valueAfterOp = value;
		mutexUnlock();

		return valueAfterOp;
	}

	int Semaphore::operator-=(int dec) throw (Exception)
	{
		int valueAfterOp;

		mutexLock();
		value -= dec;
		valueAfterOp = value;
		mutexUnlock();

		return valueAfterOp;
	}

	Semaphore::operator int() throw (Exception)
	{
		int valueAfterOp;

		mutexLock();
		valueAfterOp = value;
		mutexUnlock();

		return valueAfterOp;
	}

	void Semaphore::mutexLock() throw (Exception)
	{
		int ret_val=0;
		ret_val = pthread_mutex_lock(&mutex);
		if(ret_val < 0)
			throw Exception("Semaphore::mutexLock()", "Can't lock mutex", ret_val);
	}

	void Semaphore::mutexUnlock() throw (Exception)
	{
		int ret_val=0;
		ret_val = pthread_mutex_unlock(&mutex);
		if(ret_val < 0)
			throw Exception("Semaphore::mutexUnlock()", "Can't unlock mutex", ret_val);
	}

	void Semaphore::condWait() throw (Exception)
	{
		int ret_val=0;
		ret_val = pthread_cond_wait(&cond, &mutex);
		if(ret_val < 0)
			throw Exception("Semaphore::condWait()", "", ret_val);
	}

	void Semaphore::condSignal() throw (Exception)
	{
		int ret_val=0;
		ret_val = pthread_cond_signal(&cond);
		if(ret_val < 0)
			throw Exception("Semaphore::condSignal()", "", ret_val);
	}

	void Mutex::init_mutex(key_t sem_key, int sem_flag) throw (Exception)
	{
		int ret_val=0;
		union semun ick;

		m_sem_id = ::semget(sem_key, 10, sem_flag);
		if(m_sem_id < 0) {
			throw Exception("Mutex::Mutex()", ::strerror(errno), errno);
		}

		ick.val = 1;
		ret_val = ::semctl(m_sem_id, 0, SETVAL, ick);
		if(ret_val < 0) {
			throw Exception("Mutex::init_mutex()", ::strerror(errno), errno);
		}

		sem_on.sem_num = 0;
		sem_on.sem_op = -1;
		sem_on.sem_flg = SEM_UNDO;
		sem_off.sem_num = 0;
		sem_off.sem_op = 1;
		sem_off.sem_flg = SEM_UNDO;
		MutexSetFlag = 0;
	}

	void Mutex::destroy_mutex(void) throw (Exception)
	{
		union semun ick;

		if(MutexSetFlag) {
			this->unlock();
		}

		::semctl(m_sem_id, 0, IPC_RMID, ick);
	}

	void Mutex::lock() throw (Exception)
	{
		while(::semop(m_sem_id, &sem_on, 1) < 0) {
			if(errno != EINTR) {
				throw Exception("Mutex::lock()", ::strerror(errno), errno);
			}
		}
		MutexSetFlag = true;
	}

	void Mutex::unlock() throw (Exception)
	{
		while(::semop(m_sem_id, &sem_off, 1) < 0) {
			if(errno != EINTR) {
				throw Exception("Mutex::unlock()", ::strerror(errno), errno);
			}
		}
		MutexSetFlag = false;
	}
}


