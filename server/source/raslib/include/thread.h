/****************************************************************************

   Ras library FreeBSD Version

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: thread.h,v 1.2 2007/08/17 19:06:49 sean Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#ifndef __THREAD_H__
#define __THREAD_H__

#include <map>
#include <pthread.h>

#include "mutex.h"

namespace SDSocket
{
  class Thread
  {
  public:
    Thread();
    virtual ~Thread();

    void cancel();
    void kill();

    virtual void start();
    virtual void stop();
    virtual void run() = 0; 

    void join();
    static void joinAll();

    void detach();
    
    bool isAlive() const;

    pthread_t getID() const;

    static Thread* currentThread();

    static int activeCount();

    enum State
    {
        STOPPED,    // Thread is not running
        RUNNING,    // Thread executes run() function
        STOPPING    // Thread will stop now!
    };

    State getState() { return state; }

  protected:
    State state;
	pthread_t handle;
	static void* init(void* arg);
    static void add(Thread* thread);
    static void remove(Thread* thread);
	
  private:
    typedef std::map<pthread_t, Thread*> ThreadMap;
    typedef ThreadMap::iterator ThreadMapIterator;
    typedef ThreadMap::value_type ThreadMapType;

    static ThreadMap threads;
    static Semaphore threadsLock;

  };
}
#endif
