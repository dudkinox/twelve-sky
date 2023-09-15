/* Test software for gjrand random numbers version 1.3.3.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../src/gjrand.h"
#include "../testcommon/chi2p.h"

/* Chi-squared test for gjrand_shuffle. */

static void
crash(const char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

#define HSIZE (1<<21)
static int64_t histo[HSIZE];

static int
toi(int size, int a[])
{
#ifdef DBG
    int ok=0;
#endif
    int r=0, i;

    size-=2;
    do
    {
	i=a[size];
#ifdef DBG
	ok|=i;
#endif
	r=i+8*r;
	size--;
    } while (size>=0);

#ifdef DBG
    if ((ok & (~7))!=0) crash("bad a[] in toi");
    if ((r & (~(HSIZE-1))) !=0) crash("bad r in toi");
#endif

    return r;
}

static int64_t tot;

static double
leaf(int a[], int size, double e)
{
    int64_t x=histo[toi(size, a)];

    tot+=x;
    e-=x;
    return e*e;
}

static double
permute(int a[], int size, int i, double e)
{
    int j, k;
    double r;

    if (i>=size) return leaf(a, size, e);

    r=0.0;
    for (j=0; j<size; j++)
    {
	for (k=i-1; k>=0; k--) if (j==a[k]) goto next;
	a[i]=j;
	r+=permute(a, size, i+1, e);
	next: ;
    }

    return r;
}

static void
dochisquare(int size, int64_t count)
{
    double e, cs, p;
    int i, df;
    int a[8];

    df = 1;
    for (i=size; i>1; i--) df *= i;
    e = ((double)count)/df;
    if (e<10.0)
	fprintf(stderr, "warning counts quite low, don't trust chisquared\n");
    df--;
    tot=0;
    cs=permute(a, size, 0, e)/e;
    if (tot!=count) fprintf(stderr, "some missing numbers\n");
    p = chi2p2(cs, df);
    printf("chis = %f     df = %d     P = %.3g\n", cs, df, p);
}

static void
doshuffle(int size, int64_t count, struct gjrand *s)
{
    int64_t i;
    int a[8];

    memset(histo, 0, sizeof(histo));

    for (i=0; i<count; i++)
    {
	gjrand_shuffle(s, size, a);
	histo[toi(size,a)]++;
    }

    dochisquare(size, count);
}

int
main(int argc, char **argv)
{
    struct gjrand s;
    double dcount;
    int64_t count;
    int size, seed;

    if (argc<3) crash("2 or 3 args: size count [ seed ]");
    if (sscanf(argv[1], "%d", &size)!=1) crash("size not a number");
    if (sscanf(argv[2], "%lf", &dcount)!=1) crash("count not a number");
    else if (dcount>9.1e18) crash("count too big");
    if (argc>3)
    {
	if (sscanf(argv[3], "%d", &seed)!=1) crash("seed not a number");
	gjrand_init(&s, seed);
    }
    else gjrand_initrand(&s);

    count = (int64_t)dcount;

    if (size==2) printf("expect [ 0.000 0.000 0.016 2.705 6.630 10.883 ]\n");
    else if (size==3)
	printf("expect [ 0.21 0.55 1.61 9.24 15.11 20.58 ]\n");
    else if (size==4)
	printf("expect [ 7.5 10.2 14.8 32.0 41.6 49.7 ]\n");
    else if (size==5)
	printf("expect [ 77 86 100 139 158 172 ]\n");
    else if (size==6)
	printf("expect [ 607 634 671 768 810 841 ]\n");
    else if (size==7)
	printf("expect [ 4729 4805 4910 5168 5273 5349 ]\n");
    else if (size==8)
	printf("expect [ 39441 39658 39955 40683 40980 41197 ]\n");
    else crash("size must be in range 2 to 8");

    doshuffle(size, count, &s);

    return 0;
}
