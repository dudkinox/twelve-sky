/****************************************************************************

  Ras library FreeBSD Version

  CopyRight(C) 2003 Sean Kim All Rights reserved.
Author : Sean Kim (sean@0x31.com)

$Id: thread.cpp,v 1.2 2007/08/17 19:06:49 sean Exp $

Redistribution and use in source and binary forms, with or with out
modification, are not permitted without author's permission.

 ****************************************************************************/

#include <iostream>
#include <signal.h>

#include "thread.h"

using namespace SDSocket;
namespace SDSocket
{
	Semaphore Thread::threadsLock;

	Thread::ThreadMap Thread::threads;

	Thread::Thread()
		: state(STOPPED)
		  , handle(0)
	{}

	Thread::~Thread()
	{
	}

	void Thread::start()
	{
		state = RUNNING;
		::pthread_create(&handle, 0, Thread::init, this);
	}

	void Thread::stop()
	{
		join();
	}

	void Thread::cancel()
	{
		::pthread_cancel(handle);

		state = STOPPED;
	}

	void Thread::join()
	{
		state = STOPPING;

		int *dummy;

		::pthread_join(handle, (void**)&dummy);

		state = STOPPED;
	}

	void Thread::kill()
	{
		if(isAlive()) {
			::pthread_kill(handle, SIGKILL);
			remove(this);
		}
	}

	void Thread::detach()
	{
		::pthread_detach(handle);
	}

	void Thread::joinAll()
	{
		ThreadMapIterator iter;

		while(threads.size() > 0) {
			iter = threads.begin();

			if(iter != threads.end()) {
				iter->second->join();
			}
		}
	}

	bool Thread::isAlive() const
	{
		pthread_t id = getID();

		ThreadMapIterator iterator = threads.find(id);

		if(iterator != threads.end()) {
			return true;
		}

		return false;
	}

	pthread_t Thread::getID() const
	{
		return handle;
	}

	int Thread::activeCount()
	{
		return threads.size();
	}

	void* Thread::init(void* arg)
	{
		Thread* thread = reinterpret_cast<Thread*>(arg);

		add(thread);
		thread->run();
		remove(thread);
		if(thread != NULL) {
			delete thread;
		}

		return NULL;
	}

	void Thread::add(Thread* thread)
	{
		pthread_t id = thread->getID();

		--threadsLock;
		threads.insert(ThreadMapType(id, thread));
		++threadsLock;
	}

	void Thread::remove(Thread* thread)
	{
		pthread_t id = thread->getID();
		ThreadMapIterator iter = threads.find(id);

		if( iter != threads.end() ) {
			--threadsLock;
			threads.erase(iter);
			++threadsLock;
		}
	}
}
