/* Miscellaneous stuff for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

/* This is like blatnorm but with a different generator. */

/* This is a "Straw-man" that is supposed to fail some tests. */

/* This one uses a binomial approximation to the normal distributon. */
/* Some lowbits are thrown in, but not enough. */
/* The nda, lowbits 22, lowbits 32 and all the chi-squared tests */
/* should fail, and the gaps chis and zeroes. */
/* These could be fixed with more coins and more low bits */
/* but then it would be slow. */

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
"straw4\n"
"straw4 count\n"
"straw4 count integer_seed\n"
"Writes normal variates to standard output in (unportable) binary format.\n"
"Without count, or negative count, writes forever.\n");

    exit(1);
}

double
straw(struct gjrand *g)
{
/* I'm not sure the multiplier is totally correct. It is the */
/* result of naive and possibly wrong theoretical calculation. */
/* 0.2494 gave better results for standard deviation but worse for */
/* some chi-squared. */
    double r=(gjrand_coins(g, 64)-32)*(0.25*143.0/144.0);

#define S2 (0.25)
#define S8 (S2*S2*S2*S2)
#define S32 (S8*S8*S8*S8)
#define S34 (S32*S2)
    r+=((int32_t)gjrand_rand32(g))*S34;

    return r;
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
	double b[1024];
	size_t i, c=1024;
	if (count>=0 && c>count) c=count;

	for (i=0; i<c; i++) b[i]=straw(&g);
	if (fwrite(b, sizeof(double), c, stdout)!=c) crash("fwrite");

	if (count>0) count-=c;
    }

    return 0;
}
