/* Miscellaneous stuff for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "../../src/my_int_types.h"

/* This one writes a file for use by the tests in testunif */

/* first arg is required file size in bytes or "inf" to write forever. */
/* second optional arg is random seed. */

/* This is a lagged Fibonacci generator, or additive feedback shift register. */
/* It is the same as lfsr.c except that addition modulo 2**32 is used */
/* instead of xor. A more general lagged Fibonacci generator can */
/* multiply feedback taps by constants as well. Note that the least */
/* significant bits here are still a lfsr with period 65535. All the other */
/* bits are more complicated because of the carry bits that feed in. */
/* The period of the whole generator is apparently very long. */

/* This one is based on a "pentanomial", which should have slightly */
/* better properties than the "trinomials" that are usually favoured. */
/* On the other hand trinomials are faster. Sometimes extremely long lags */
/* are recommended, and for the most part those do have better properties */
/* than short-lag generators like this one. */

void
crash(char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

void
update(uint32_t x[16])
{
    int i;

    for (i=0; i<16; i++) x[i] += x[(i-5)&15]+x[(i-3)&15]+x[(i-2)&15];
}

void
blat(int64_t s, uint32_t seed)
{
    int i;
    uint32_t buf[16];

    buf[0]=seed;
    buf[1]= -(1ul);
    buf[2]=987654321;
    for (i=3; i<16; i++) buf[i]=buf[i-1]+123456789;
    update(buf);
    buf[0]^=buf[0]>>16;
    buf[1]^=buf[1]>>24;
    buf[2]^=buf[2]>>28;
    buf[3]^=buf[3]>>30;
    buf[4]^=buf[4]>>31;
    update(buf);

    while (s)
    {
	size_t c;

	update(buf);

	if ((uint64_t)s>64) c=64; else c=s;
	if (fwrite(buf, 1, c, stdout)!=c) crash("fwrite fails");

	if (s>0) s-=c;
    }
}

int
main(int argc, char **argv)
{
    double ds;
    int64_t s;
    unsigned long seed;

    if (argc>2) sscanf(argv[2], "%lu", &seed);
    else seed=time(0)+getpid();
    if (argc<2) crash("give a size arg in bytes, or 'inf'");
    if (sscanf(argv[1], "%lf", &ds)==1) s=(int64_t)ds;
    else
    {
	if (strcmp(argv[1],"inf")) crash("not a number");
	s= -1;
    }

    blat(s, seed);

    return 0;
}
