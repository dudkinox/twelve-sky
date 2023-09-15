/* Test software for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../src/gjrand.h"
#include "../testcommon/chi2p.h"

/* Chi-squared test for gjrand_rand32mod, using triples of values. */

#define MAX 32

static long histo[MAX][MAX][MAX];

static void
dochisquare(int mod, int count)
{
    int df=mod*mod*mod;
    double e=((double)count)/df, cs=0.0;
    int i, j, k;

    for (i=0; i<mod; i++) for (j=0; j<mod; j++) for (k=0; k<mod; k++)
    {
	double d=histo[i][j][k]-e;
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
    int i, j, k;
    int old=gjrand_rand32mod(s, mod);
    int old2=gjrand_rand32mod(s, mod);

    for (i=0; i<mod; i++) for (j=0; j<mod; j++)
	for (k=0; k<mod; k++) histo[i][j][k]=0;

    for (i=0; i<count; i++)
    {
	j=gjrand_rand32mod(s, mod);
	histo[old][old2][j]++;
	old=old2; old2=j;
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

    if (mod<2 || mod >MAX) crash("must have 2 <= mod <= 32");

    domod(mod, count, &s);

    return 0;
}
