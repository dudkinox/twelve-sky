/* Test software for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include "../../src/gjrand.h"

/* This one writes a file for use by several tests in this directory. */
/* The output consists of binary type double numbers. Note that these */
/* have different formats depending on CPU, OS, and compiler, so try */
/* to generate and use them on the same computer. */
/* They are supposed to be uniformly distributed on [ 0 1 ) */

/* Optional first arg is number of doubles to write. */
/*     Attempts to write forever if negative or absent. */
/* second optional arg is random seed. */

/* This uses an idea pinched from the sprng2 tests, for testing a random */
/* generator that will be used in a highly parallel application. */
/* Enable a bunch (128 here) of gjrand objects, and seed them with successive */
/* seeds, as would be convenient for parallel use. Generate some (256 here) */
/* random doubles from each gjrand object, moving on to the next, and */
/* eventually cycling back to the first one. Then the usual tests can be run */
/* on the resulting stream. This is likely to show if the streams from */
/* adjacent seeds are correlated. */

void
crash(char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

void
blat(struct gjrand *g, int64_t s)
{
    double buf[256];
    int i=0;

    while (s)
    {
	int c;

	gjrand_drandv(g+i, 256, buf);

	if ((uint64_t)s>256) c=256; else c=s;
	if (fwrite(buf, sizeof(double), c, stdout)!=(size_t)c)
	{
	    if (s>0) crash("fwrite fails");
	    return;
	}

	i++; i&=127;

	if (s>0) s-=c;
    }
}

int
main(int argc, char **argv)
{
    double ds;
    int64_t s= -1;
    unsigned long seed;
    uint64_t seed64;
    struct gjrand g[128];
    int i;

    if (argc>2) {sscanf(argv[2], "%lu", &seed); seed64=((uint64_t)seed)<<7;}
    else {gjrand_initrand(g); seed64=gjrand_rand64(g);}
    for (i=0; i<128; i++) gjrand_init64(g+i, seed64+i);

    if (argc>1 && sscanf(argv[1], "%lf", &ds)==1) s=(int64_t)ds;

    blat(g, s);

    return 0;
}
