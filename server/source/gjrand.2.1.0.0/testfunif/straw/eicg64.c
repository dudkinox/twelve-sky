/* Miscellaneous stuff for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* Explicit inversive congruential generator. */
/* This one runs modulo 2 ** 64 . That results in period 2 ** 63 */
/* (all odd numbers are reachable once each) which is much nicer than */
/* eicg. On the other hand prime modulus has a much better reputation */
/* than power of 2. Let's see how it goes. */

/* It's a bit slow. */
/* It looks mostly ok on tests, but fails chi somewhere before 7 million */
/* numbers. This is because the least significant byte (of 6) is too */
/* uniform. That's the least troubling failure mode i can think of, */
/* but nonetheless, be careful with this one. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "../../src/my_int_types.h"

#define ADD 12345678910111234

static uint64_t
invert(uint64_t x)
{
    uint64_t r=(x&1), mask;

    for (mask=2; mask; mask+=mask) if ((r*x)&mask) r|=mask;

    return r;
}

#define Q (0.25)
#define S8 (Q*Q*Q*Q)
#define S32 (S8*S8*S8*S8)
#define MUL (S32*S32)

static uint64_t
block(double *a, uint64_t seed)
{
    uint64_t s=seed;
    int i;

    for (i=0; i<1024; i++)
    {
	s+=ADD;
	a[i]= (invert(s)+0.5) * MUL;
    }

    return s;
}

int
main(int argc, char **argv)
{
    double a[1024];
    unsigned long t;
    uint64_t s;

    if (argc>1) {sscanf(argv[1], "%lu", &t); s=t;}
    else s=time(0);

    s=2*s+1; s*=54321ul;

    while (1)
    {
	s=block(a, s);
	if (fwrite(a, sizeof(double), 1024, stdout)!=1024) break;
    }

    return 0;
}
