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
/* It is the same as the one in lfsr.c except for the rotate. */
/* I think this turns it into one huge 512 bit shift register instead */
/* of 32 small ones of 16 bits each. */
/* The underlying polynomial is probably not primitive (i haven't checked). */
/* The period is unknown but likely to be long. */

/* This was changed between version 0.1.5.0 and 0.1.6.0 . */
/* The underlying polynomial without the rotate is now primitive, */
/* though with the rotate, it probably still isn't. */
/* I suspect that didn't change the quality much. */
/* In addition, a better seeding procedure is now used, but that doesn't */
/* seem to help much either. */

/* This changed again between 1.2.0.0 and 1.2.1.0 . Due to a stupid */
/* programming error, the older versions use a much smaller and probably */
/* weaker shift register. */

/* 2007-06-24: problems seen (eg): */
/* addsub -r (20MB), addsub -f (20MB), chi16 (2MB), nda (10MB), metachi (3MB) */
/* partrda (4MB), rda (10MB), selfcor -f (8MB), selfcor -r (9MB) */
/* sh5da (10MB), sh5da -r (10MB), shiftrda (10MB), shuffle (7MB) */
/* shuffle -r (7MB), slicerda (3MB), v256 (40MB), zero (70MB) */

/* Neither gzip nor bzip2 can compress the output. */

/* This fails horribly at Diehard. */
/* Some of the p-values are 0 to 6 decimal places. */


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
    {
	uint32_t t = x[(j-16)&1023]^x[(j-5)&1023]^x[(j-3)&1023]^x[(j-2)&1023];
	x[j] = (t>>31) | (t<<1);
    }
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
	a = (a>>31) | (a<<1);
	b = (b>>31) | (b<<1);
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
