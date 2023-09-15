/* Test software for gjrand random numbers version 2.0.0.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../src/gjrand.h"
#include "../testcommon/chi2p.h"

/* This tests the gjrand_chisquared() function. It generates */
/* a lot of chisquared distributed random numbers, sorts them */
/* and then prints out various entries from the sorted list. */
/* This could then be compared to statistical tables of the */
/* chisquared distribution. Unfortunately i haven't yet done any */
/* calculation of just how close you should expect to be to the tables. */

/* The third column is calculated P-value for that value, which should */
/* be similar to the first column, but not identical due to inaccurate */
/* P-value calculation as well as error due to limited sample size. */

#define SIZE 2000000

static int
dcmp(const void *a, const void *b)
{
    double da= * (const double *) a;
    double db= * (const double *) b;

    if (da<db) return -1;
    if (da==db) return 0;
    return 1;
}


static double d[SIZE+3];

static void
ntile(double n, int f)
{
    int i = (int)floor(n*SIZE+0.5);

    if (i<0) i=0; else if (i>=SIZE) i=SIZE-1;

    printf("%10.5f %10.5f %10.7f\n", 1.0-n, d[i], chi2p1(d[i], f));
}

static double
mean(void)
{
    double m=0.0;
    int i;
    for (i=0; i<SIZE; i++) m+=d[i];
    m/=SIZE;
    printf("%10s %10.5f\n", "mean", m);
    return m;
}

static double
sd(double mean)
{
    double s=0.0;
    int i;
    for (i=0; i<SIZE; i++) {double t=d[i]-mean; s+=t*t;}
    s = sqrt(s/SIZE);
    printf("%10s %10.5f\n", "sdev", s);
    return s;
}

static void
skew(double mean, double sd)
{
    double s=0.0;
    int j;

    for (j=0; j<SIZE; j++)
    {
	double t=d[j];
	t -= mean;
	t /= sd;
	s += t*t*t;
    }
    s /= SIZE;
    printf("%10s %10.5f\n", "skew", s);
}

static void
testchi(int f)
{
    struct gjrand g;
    int i;
    double m, s;

    gjrand_initrand(&g);

    for (i=0; i<SIZE; i++) d[i]=gjrand_chisquared(&g, f);

    qsort(d, SIZE, sizeof(double), dcmp);

    ntile(0.001, f);
    ntile(0.005, f);
    ntile(0.01, f);
    ntile(0.05, f);
    ntile(0.1, f);
    ntile(0.5, f);
    ntile(0.7, f);
    ntile(0.8, f);
    ntile(0.85, f);
    ntile(0.9, f);
    ntile(0.95, f);
    ntile(0.975, f);
    ntile(0.99, f);
    ntile(0.995, f);
    ntile(0.999, f);

    m=mean();
    s = sd(m);
    skew(m, s);
}

int
main(int argc, char **argv)
{
    int f;
    if (argc!=2 || sscanf(argv[1], "%d", &f)!=1)
    {
	fprintf(stderr, "usage: testchi degrees-of-freedom\n");
	exit(1);
    }
    testchi(f);
    return 0;
}
