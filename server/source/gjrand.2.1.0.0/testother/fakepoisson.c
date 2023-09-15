/* Test software for gjrand random numbers version 1.3.3.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../src/gjrand.h"
#include "../testcommon/chi2p.h"

/* Chi-squared test for approximating poisson using gjrand_biased_coins(). */
/* Note: this test is supposed to fail sometimes, especially for small coins */
/* and/or large trials. That just proves binomial is not quite the same */
/* as poisson. */
/* First arg is number of coins (more should be more accurate). */
/* Second arg is simulated poisson interval size. */
/* Third arg is number of trials. */
/* Fourth optional arg is random seed. */

void
crash(const char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

long histo[101];

void
dofish(int coin, long count, struct gjrand *s, double t)
{
    int i, j;

    for (i=0; i<101; i++) histo[i]=0;

    t/=coin;

    do
    {
	j=gjrand_biased_coins(s, t, coin);
	if (j>100) j=100;
	histo[j]++;
	count--;
    } while (count>0);
}

double ex[101];

void
mkexpect(long count, double t)
{
    double p;
    int i;

    ex[0]=p=exp(-t)*count;

    for (i=1; i<100; i++)
    {
	p*=t;
	p/=i;
	ex[i]=p;
    }

    p=count;
    for (i=0; i<100; i++) p-=ex[i];
    ex[100]=p;
}

void
aggregate(int *lo, int *hi)
{
    int i, l, h;
    double t;
    long tl;

    for (l=0; l<100; l++) if (ex[l]>5.0) break;
    *lo=l;

    for (h=100; h>0; h--) if (ex[h-1]>5.0) break;
    *hi=h;

    if (lo>=hi) {fprintf(stderr, "counts too low\n"); exit(1);}

    t=ex[100];
    tl=histo[100];
    for (i=0; i<l; i++) {t+=ex[i]; tl+=histo[i];}
    for (i=h; i<100; i++) {t+=ex[i]; tl+=histo[i];}
    ex[100]=t; histo[100]=tl;
}

void
doan(long count, double t)
{
    int i, lo, hi, df;
    double r=0.0, p;

    mkexpect(count, t);

for (i=0; i<100; i++)
if (ex[i]>1e-16 || histo[i])
printf("%3d: %15.5f %10ld\n", i, ex[i], histo[i]);
printf("  X: %15.5f %10ld\n\n", ex[100], histo[100]);

    aggregate(&lo, &hi);

for (i=lo; i<hi; i++)
printf("%3d: %15.5f %10ld\n", i, ex[i], histo[i]);
printf("  X: %15.5f %10ld\n", ex[100], histo[100]);

    df=hi-lo;

    if (ex[100]>0.5)
	r=(ex[100]-histo[100])*(ex[100]-histo[100])/ex[100];
    else {r=0.0; df--;}
    for (i=lo; i<hi; i++) r+=(ex[i]-histo[i])*(ex[i]-histo[i])/ex[i];

    p = chi2p1(r, df);

    printf("chis = %f     df = %d     P = %.3g\n", r, df, p);
}

int
main(int argc, char **argv)
{
    double t;
    long count, seed;
    int coin;
    struct gjrand s;

    if (argc<4) crash("3 or 4 args: coincount interval count [ seed ]");
    if (sscanf(argv[1], "%d", &coin)!=1) crash("coincount not a number");
    if (sscanf(argv[2], "%lf", &t)!=1) crash("interval not a number");
    if (sscanf(argv[3], "%ld", &count)!=1) crash("count not a number");
    if (argc>4)
    {
	if (sscanf(argv[4], "%ld", &seed)!=1) crash("seed not a number");
	gjrand_init(&s, seed);
    }
    else gjrand_initrand(&s);

    dofish(coin, count, &s, t);
    doan(count, t);

    return 0;
}
