/* Test software for gjrand random numbers version 1.3.3.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../src/gjrand.h"
#include "../testcommon/chi2p.h"

/* Chi-squared test for gjrand_rand32mod, using pairs of values. */

#define MAX 128

static long histo[MAX][MAX];

static void
dochisquare(int mod, int count)
{
    int df=mod*mod;
    double e=((double)count)/df, cs=0.0;
    int i, j;

    for (i=0; i<mod; i++) for (j=0; j<mod; j++)
    {
	double d=histo[i][j]-e;
	cs+=d*d;
    }
    cs /= e;

    df--;
    e = chi2p2(cs, df);

    printf("chis = %f     df = %d     P = %.3g\n", cs, df, e);
}

static void
domod(int mod, int count, struct gjrand *s)
{
    int i, j, old=gjrand_rand32mod(s, mod);

    for (i=0; i<mod; i++) for (j=0; j<mod; j++) histo[i][j]=0;

    for (i=0; i<count; i++)
    {
	j=gjrand_rand32mod(s, mod);
	histo[old][j]++;
	old=j;
    }

    dochisquare(mod, count);
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
    int mod, count, seed;
    struct gjrand s;

    if (argc<3) crash("2 or 3 args: mod count [ seed ]");
    if (sscanf(argv[1], "%d", &mod)!=1) crash("mod not a number");
    if (sscanf(argv[2], "%d", &count)!=1) crash("count not a number");
    if (argc>3)
    {
	if (sscanf(argv[3], "%d", &seed)!=1) crash("seed not a number");
	gjrand_init(&s, seed);
    }
    else gjrand_initrand(&s);

    if (mod<2 || mod >MAX) crash("must have 2 <= mod <= 128");

    domod(mod, count, &s);

    return 0;
}
