/* Miscellaneous stuff for gjrand random numbers version 1.3.1.0 or later. */
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
/* This one uses a prime modulus, which is supposed to solve the bad low */
/* bits problem. It fails diff3 and diff10 anyway. Otherwise looks good. */

/* MOD is supposed to be prime with 2**33 as a generator for the */
/* multiplicative group */
#define MOD ((((uint64_t)0x3b7de98d)<<32)|0x86768fe7)

uint64_t state;

static double
drnd(void)
{
    uint64_t l=state;
    int j;

    for (j=33; j>0; j--)
    {
	l += l;
	if (l>=MOD) l -= MOD;
    }

    state = l;

    return l * (1.0/MOD);
}

static void
init(uint32_t seed)
{
    int j;

    state = seed;
    state++;

    for (j=17; j>0; j--) (void)(drnd());
}

static void
blat(uint32_t seed)
{
    double buf[1024];

    init(seed);

    while (1)
    {
	int i;

	for (i=0; i<1024; i++) buf[i]=drnd();

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
