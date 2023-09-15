/* Miscellaneous stuff for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "../../src/my_int_types.h"

/* This one writes a file for use by the tests in testfunif */

/* optional arg is random seed. */

/* This is a linear congruential generator. */
/* The worst thing about these is the least significant bits. */
/* By going to 64 bits, this one should banish those so far right */
/* they hardly matter. Therefore this should behave much better. */
/* 2007-12-10 : new test diff spots it quickly. older tests all */
/* appear ok up --standard at least. */

#define S2 0.25
#define S8 (S2*S2*S2*S2)
#define S32 (S8*S8*S8*S8)
#define S53 (S32*S8*S8*S2*S2*0.5)

static void
blat(uint32_t seed)
{
    double buf[1024];
    uint64_t j=seed;

    while (1)
    {
	int i;

	for (i=0; i<1024; i++)
	{
	    j=j*3098765421UL+1;
	    buf[i]=(j>>11)*S53;
	}

	if (fwrite(buf, sizeof(double), 1024, stdout)!=1024) break;
    }
}

int
main(int argc, char **argv)
{
    unsigned long t;
    uint32_t seed;

    if (argc>1) {sscanf(argv[1], "%lu", &t); seed=t;}
    else seed=time(0);

    blat(seed);

    return 0;
}
