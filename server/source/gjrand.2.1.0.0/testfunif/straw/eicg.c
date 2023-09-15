/* Miscellaneous stuff for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* Explicit inversive congruential generator. */
/* Since about 1990 these and related methods have got a lot of attention. */
/* Some people think they are really good. I have two complaints: */
/* The period is too short, (just less than 2 ** 32), and making it longer */
/* would add even more complexity and slowness; and it's slow. Granted */
/* it should be possible to improve on this cruddy implementation, but */
/* i can't imagine it getting as fast as gjrand, or lots of respectable */
/* generators that pass the same tests. */

/* It tests out mostly ok, except dim2 suggests it's uniformity in one */
/* dimension may be too good to be random. I haven't had the patience to */
/* do any big tests yet. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "../../src/my_int_types.h"

#define MOD 4294967291UL
#define ADD 12345

static uint32_t
invert(uint32_t x)
{
    uint32_t r;

    x%=MOD;

    if (x<=1) return x;

    r=invert(MOD%x);
    r = (uint64_t)r*(MOD-(MOD/x)) % MOD;

    return r;
}

static uint32_t
block(double *a, uint32_t seed)
{
    uint64_t s=seed;
    int i;

    for (i=0; i<1024; i++)
    {
	s+=ADD; if (s>=MOD) s-=MOD;
	a[i]= (invert((uint32_t)s)+0.5) * (1.0/MOD);
    }

    return (uint32_t)s;
}

int
main(int argc, char **argv)
{
    double a[1024];
    unsigned long t;
    uint32_t s;

    if (argc>1) {sscanf(argv[1], "%lu", &t); s=t;}
    else s=time(0);
    s=block(a, s);

    while (1)
    {
	s=block(a, s);
	if (fwrite(a, sizeof(double), 1024, stdout)!=1024) break;
    }

    return 0;
}
