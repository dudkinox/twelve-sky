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

/* This one perhaps represents modern practice better than lfib.c . */

/* A trinomial with fairly large lags is used. This particular one was */
/* apparently recommended by Knuth, but i haven't seen the context. */
/* It's a primitive polynomial, according to Schneier. */

/* Large trinomial based generators are certainly fast. They are in the same */
/* league as the "fast" generator in this directory, maybe faster for */
/* some machines. */

/* shiftrda busts this easily. */

void
crash(char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

/* Unrolled loop in update assumes (SIZ-LAG1) is divisible by 4. */
#define SIZ 1024
#define LAG1 100
#define LAG2 37

void
update(uint32_t x[SIZ])
{
    uint32_t *xi, *end;

    /* unroll the first two loops too, if you're desperate, but it's */
    /* maybe 10% on most machines. */

    xi=x; end=xi+LAG2;
    do {*xi = xi[SIZ-LAG2] + xi[SIZ-LAG1]; xi++;} while (xi<end);

    end=x+LAG1;
    do {*xi = xi[-LAG2] + xi[SIZ-LAG1]; xi++;} while (xi<end);

    /* The unrolling seems to help more than can be accounted for just */
    /* by loop overhead. I think it leads to improved pipelining */
    /* and parallelism. Probably the data-flow engine (at least in my CPU) */
    /* is not smart enough to see that the memory references from different */
    /* iterations can't interfere until LAG2 iterations later. */

    end=x+SIZ;
    do
    {
	uint32_t a, b, c, d;
	a=xi[-LAG2]; b=xi[1-LAG2]; c=xi[2-LAG2]; d=xi[3-LAG2];
	a+=xi[-LAG1]; b+=xi[1-LAG1]; c+=xi[2-LAG1]; d+=xi[3-LAG1];
	xi+=4;
	xi[-4]=a; xi[-3]=b; xi[-2]=c; xi[-1]=d;
    } while (xi<end);
}

void
writestuff(int64_t s, uint32_t seed)
{
    int i;
    uint32_t j, buf[SIZ];

    buf[0] = -(1ul);
    buf[1]=j=seed;
    for (i=2; i<SIZ; i++)
    {
	j+=123456789;
	j*=987654321;
	j^=(j>>16);
	buf[i]=j;
    }

    while (s)
    {
	size_t c;

	update(buf);

	if ((uint64_t)s>sizeof(buf)) c=sizeof(buf); else c=s;
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
    else seed=time(0)*(2*getpid()+1);
    if (argc<2) crash("give a size arg in bytes, or 'inf'");
    if (sscanf(argv[1], "%lf", &ds)==1) s=(int64_t)ds;
    else
    {
	if (strcmp(argv[1],"inf")) crash("not a number");
	s= -1;
    }

    writestuff(s, seed);

    return 0;
}
