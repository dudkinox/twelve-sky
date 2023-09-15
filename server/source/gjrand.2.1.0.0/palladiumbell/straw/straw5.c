/* Miscellaneous stuff for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

/* This is like blatnorm but with a different generator. */

/* This is a "Straw-man" that is supposed to fail some tests. */

/* This one is actually gjrand_normal() with some post-processing */
/* to try to get some correlations with short lags. It should fail */
/* the self-correlation. */

#ifdef __cplusplus
#define NOARGS
#else
#define NOARGS void
#endif

#if defined(__cplusplus) || defined(__GNUC__)
#define INLINE inline
#else
#define INLINE
#endif

#include "../../src/gjrand.h"

void
usage(NOARGS)
{
    printf("usage:\n"
"straw5\n"
"straw5 count\n"
"straw5 count integer_seed\n"
"Writes normal variates to standard output in (unportable) binary format.\n"
"Without count, or negative count, writes forever.\n");

    exit(1);
}

static void
crash(const char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

int
main(int argc, char **argv)
{
    long seed;
    long long count= -1;
    struct gjrand g;

    if (argc>1)
	{if (sscanf(argv[1], "%lld", &count)!=1) usage();}

    if (argc>2)
    {
	if (sscanf(argv[2], "%ld", &seed)!=1) usage();
	gjrand_init(&g, seed);
    }
    else gjrand_initrand(&g);

    while (count)
    {
	double b[16*1024];
	size_t i, c=16*1024;
	if (count>=0 && c>count) c=count;

	for (i=0; i<c; i++)
	{
	    double x=gjrand_normal(&g);
	    if (i>1000 && (gjrand_rand(&g)&31)==0)
		x=(x+b[i-42]-b[i-747]+b[i-999])*0.5;
	    b[i]=x;
	}
	if (fwrite(b, sizeof(double), c, stdout)!=c) crash("fwrite");

	if (count>0) count-=c;
    }

    return 0;
}
