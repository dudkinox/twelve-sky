/* Test software for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include "../../src/gjrand.h"

/* This one writes a file for use by several tests in this directory: */
/* The output consists of binary type double numbers. Note that these */
/* have different formats depending on CPU, OS, and compiler, so try */
/* to generate and use them on the same computer. */
/* They are supposed to be uniformly distributed on [ 0 1 ) */

/* Optional first arg is number of doubles to write. */
/*     Attempts to write forever if negative or absent. */
/* second optional arg is random seed. */

void
crash(char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

void
blat(struct gjrand *g, int64_t s)
{
    double buf[1024];

    while (s)
    {
	int c;

	gjrand_drandv(g, 1024, buf);

	if ((uint64_t)s>1024) c=1024; else c=s;
	if (fwrite(buf, sizeof(double), c, stdout)!=(size_t)c)
	    crash("fwrite fails");

	if (s>0) s-=c;
    }
}

int
main(int argc, char **argv)
{
    double ds;
    int64_t s= -1;
    unsigned long seed;
    struct gjrand g;

    if (argc>2) {sscanf(argv[2], "%lu", &seed); gjrand_init(&g, seed);}
    else gjrand_initrand(&g);

    if (argc>1 && sscanf(argv[1], "%lf", &ds)==1) s=(int64_t)ds;

    blat(&g, s);

    return 0;
}
