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

/* This is a linear feedback shift register generator. */

/* This one is not a particularly good example of the breed. For better */
/* results, a longer shift register with more tap points should be used. */
/* The underlying GF2 polynomial should be primitive, as this one is. */
/* Here the shift register is 16 bits, and there are effectively 32 */
/* similar ones working in parallel to make efficient use of a 32 bit */
/* computer. */

/* The cycle length will be only 65535 32 bit words. A longer shift */
/* register will give a longer cycle, provided the polynomial is */
/* primitive. It is important to initialise so that each shift register */
/* contains at least one non-zero bit, because if it starts all zeroes, it */
/* will stay all zeroes. It is important to initialise each shift register */
/* differently to the others, because if they start the same, they will stay */
/* the same. */

static void
crash(char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

#if 0
void
update(uint32_t x[16])
{
    int i;

    for (i=0; i<16; i++) x[i] ^= x[(i-5)&15]^x[(i-3)&15]^x[(i-2)&15];
}
#endif

static void
preupdate(uint32_t x[1024])
{
    int j;

    for (j=0; j<16; j++)
	x[j] = x[(j-16)&1023]^x[(j-5)&1023]^x[(j-3)&1023]^x[(j-2)&1023];
}

static void
update(uint32_t *x)
{
    uint32_t a=x[14], b=x[15];
    uint32_t *xi=x+16, *xe=x+1024;

    do
    {
	a ^= xi[-16]; b ^= xi[-15];
	a ^= xi[-5]; b ^= xi[-4];
	a ^= xi[-3]; b ^= xi[-2];
	xi[0] = a; xi[1]=b;
	xi+=2;
    } while (xi<xe);
}

static void
blat(int64_t s, uint32_t seed)
{
    int i;
    uint32_t buf[1024];

#define be (buf+1024-16)

    be[0]=seed;
    be[1]= -(1ul);
    be[2]=987654321;
    for (i=3; i<16; i++)
    {
	uint32_t t=be[i-3]+be[i-1]+123456789;
	be[i]=(t<<29)|(t>>3);
    }

    while (s)
    {
	size_t c;

	preupdate(buf);
	update(buf);

	if ((uint64_t)s>4096) c=4096; else c=s;
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
