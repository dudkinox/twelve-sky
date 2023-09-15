/* Test software for gjrand random numbers version 1.3.3.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../src/gjrand.h"
#include "../testcommon/chi2p.h"

/* Simple chisquared test for gjrand_shuffle. */
/* This tests that each number has about the same chance of turning up */
/* in each position. */

#define MAX 2045

void
crash(const char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

int b[MAX+3][MAX+3];

void
chk(int size, int a[])
{
    int i;

    for (i=0; i<size; i++) b[i][a[i]]++;
}

void
chisquare(int size, int count)
{
    double c, d, x, p;
    int i, j, df;

    c=0.0;
    x=((double)count)/((double)size);
    for (i=0; i<size; i++) for (j=0; j<size; j++)
    {
	d=b[i][j]-x;
	c+=d*d;
    }
    c /= x;
    df = (size-1)*size; /* Is this right? */

    p = chi2p2(c, df);

    printf("chis = %15.5f     df = %d     P = %.3g\n", c, df, p);
}

void
doshuffle(int size, int count, struct gjrand *s)
{
    static int a[MAX+3];
    int i, ok=0;

    for (i=0; i<size; i++) for (ok=0; ok<size; ok++) b[i][ok]=0;

    ok=0;

    for (i=0; i<count; i++)
    {
	gjrand_shuffle(s, size, a);
	chk(size, a);
    }

    chisquare(size, count);
}

int
main(int argc, char **argv)
{
    int size, count, seed;
    struct gjrand s;

    if (argc<3) crash("2 or 3 args: size count [ seed ]");
    if (sscanf(argv[1], "%d", &size)!=1) crash("size not a number");
    if (sscanf(argv[2], "%d", &count)!=1) crash("count not a number");
    if (size<2 || size>MAX) crash("count out of range");
    if (argc>3)
    {
	if (sscanf(argv[3], "%d", &seed)!=1) crash("seed not a number");
	gjrand_init(&s, seed);
    }
    else gjrand_initrand(&s);

    doshuffle(size, count, &s);

    return 0;
}
