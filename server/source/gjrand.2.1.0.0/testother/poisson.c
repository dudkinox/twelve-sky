/* Test software for gjrand random numbers version 1.3.3.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../src/gjrand.h"
#include "../testcommon/chi2p.h"

/* Chi-squared test for gjrand_poisson. */
/* First arg is interval size. */
/* Second arg is number of trials. */
/* Third optional arg is random seed. */

void
crash(const char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

#define MAX 900
long histo[MAX+1];
double sum=0.0, ss=0.0, sq=0.0;

void
dofish(long count, struct gjrand *s, double t)
{
    int i, j;

    for (i=0; i<=MAX; i++) histo[i]=0;
    do
    {
	double x;

	j=gjrand_poisson(s, t);
	x=j-t; sum+=x;
	x=x*x; ss+=x;
	x-=t; x=x*x; sq+=x;
	if (j>MAX) j=MAX;
	histo[j]++;
	count--;
    } while (count>0);
}

double ex[MAX+1];

void
mkexpect(long count, double t)
{
    double p;
    int i;

    ex[0]=p=exp(-t)*count;

    for (i=1; i<MAX; i++)
    {
	p*=t;
	p/=i;
	ex[i]=p;
    }

    p=count;
    for (i=0; i<MAX; i++) p-=ex[i];
    ex[MAX]=p;
}

void
aggregate(int *lo, int *hi)
{
    int i, l, h;
    double t;
    long tl;

    for (l=0; l<MAX; l++) if (ex[l]>5.0) break;
    *lo=l;

    for (h=MAX; h>0; h--) if (ex[h-1]>5.0) break;
    *hi=h;

    if (lo>=hi) {fprintf(stderr, "counts too low\n"); exit(1);}

    t=ex[MAX];
    tl=histo[MAX];
    for (i=0; i<l; i++) {t+=ex[i]; tl+=histo[i];}
    for (i=h; i<MAX; i++) {t+=ex[i]; tl+=histo[i];}
    ex[MAX]=t; histo[MAX]=tl;
}

void
doan(long count, double t)
{
    int i, lo, hi, df;
    double r=0.0, p;

    mkexpect(count, t);

for (i=0; i<MAX; i++)
if (ex[i]>1e-16 || histo[i])
printf("%3d: %20.12f %10ld\n", i, ex[i], histo[i]);
printf("  X: %20.12f %10ld\n\n", ex[MAX], histo[MAX]);

    aggregate(&lo, &hi);

for (i=lo; i<hi; i++)
printf("%3d: %20.12f %10ld\n", i, ex[i], histo[i]);
printf("  X: %20.12f %10ld\n", ex[MAX], histo[MAX]);

    df=hi-lo;

    if (ex[MAX]>0.5)
	r=(ex[MAX]-histo[MAX])*(ex[MAX]-histo[MAX])/ex[MAX];
    else {r=0.0; df--;}
    for (i=lo; i<hi; i++) r+=(ex[i]-histo[i])*(ex[i]-histo[i])/ex[i];

    p = chi2p1(r, df);

    printf("chis = %f     df = %d     P = %.3g\n\n", r, df, p);

    printf("mean discr = %f   var = %f\nvar-var = %f   sd-var = %f\n",
	sum/count, ss/count, sq/count, sqrt(sq/count));
}

int
main(int argc, char **argv)
{
    double t;
    long count, seed;
    struct gjrand s;

    if (argc<3) crash("2 or 3 args: interval count [ seed ]");
    if (sscanf(argv[1], "%lf", &t)!=1) crash("interval not a number");
    if (sscanf(argv[2], "%ld", &count)!=1) crash("count not a number");
    if (argc>3)
    {
	if (sscanf(argv[3], "%ld", &seed)!=1) crash("seed not a number");
	gjrand_init(&s, seed);
    }
    else gjrand_initrand(&s);

    dofish(count, &s, t);
    doan(count, t);

    return 0;
}
