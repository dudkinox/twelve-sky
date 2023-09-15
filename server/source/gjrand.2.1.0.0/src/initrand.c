/* Part of gjrand random number library version 1.0.1.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"
#include "inte.h"
#include <sys/time.h>
#include <unistd.h>
#ifdef USE_POSIX_THREADS
#include <pthread.h>
#endif

/* We try fairly hard to make this do a different seeding for two different */
/* calls. Firstly we fold in the pid so different processes get different */
/* seeds. Secondly if threads are enabled, the thread id is also used. */
/* We also use the seconds and microseconds time values. Unfortunately */
/* this is not always enough: some computers only update the microseconds */
/* field at fairly slow intervals, for instance 0.01 seconds was once */
/* popular on linux. And even with update every microsecond, the time is */
/* coming soon where a computer will be quick enough to do two calls */
/* to this routine in less than a microsecond. To beat even this, */
/* nonce is used, which should give a different seed for subsequent calls */
/* from within the same process, even if the time hasn't changed. This may */
/* appear to not be threadsafe, and perhaps by the strictest definitions */
/* it isn't. Trust me though, unless you are doing some maximum security */
/* audit: in most settings it is safe even in multi-threaded code. */

void
gjrand_initrand(struct gjrand *s)
{
    struct timeval t;
    uint64_t b[4], tn;
    static uint64_t nonce=0;

    tn=nonce+987654321ul; nonce=tn;
    s->a = tn;

    gettimeofday(&t, 0); /* seconds and microseconds timer */
    s->b = (((uint64_t)(t.tv_sec))<<32) | (uint32_t)(t.tv_usec);
    s->c=getpid(); /* process id */

#ifdef USE_POSIX_THREADS
    s->d=pthread_self(); /* thread id, if available */
#else
    s->d=getppid(); /* otherwise something else */
#endif

    gjrand_inte_crank64(s, b);
    s->a=0; s->b=b[0]; s->c=b[1]; s->d=4000001ul;
}
