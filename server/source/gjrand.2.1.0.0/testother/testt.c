/* Test software for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../src/gjrand.h"

/* This tests the gjrand_t() function. It generates */
/* a lot of T-distribution distributed random numbers, sorts them */
/* and then prints out various entries from the sorted list. */
/* This could then be compared to statistical tables of the */
/* chisquared distribution. Unfortunately i haven't yet done any */
/* calculation of just how close you should expect to be to the tables. */

#define SIZE 2000000

int
dcmp(const void *a, const void *b)
{
    double da= * (const double *) a;
    double db= * (const double *) b;

    if (da<db) return -1;
    if (da==db) return 0;
    return 1;
}


double d[SIZE+3];

void
ntile(double n)
{
    int i = (int)floor(n*SIZE+0.5);

    if (i<0) i=0; else if (i>=SIZE) i=SIZE-1;

    printf("%10.5f %10.5f\n", n, d[i]);
}

double
mean(void)
{
    double m=0.0;
    int i;
    for (i=0; i<SIZE; i++) m+=d[i];
    m/=SIZE;
    printf("%10s %10.5f\n", "mean", m);
    return m;
}

void
sd(double mean)
{
    double s=0.0;
    int i;
    for (i=0; i<SIZE; i++) {double t=d[i]-mean; s+=t*t;}
    printf("%10s %10.5f\n", "sdev", sqrt(s/SIZE));
}

void
testt(int f)
{
    struct gjrand g;
    int i;
    double m;

    gjrand_initrand(&g);

    for (i=0; i<SIZE; i++) d[i]=gjrand_t(&g, f);

    qsort(d, SIZE, sizeof(double), dcmp);

    ntile(0.001);
    ntile(0.005);
    ntile(0.01);
    ntile(0.025);
    ntile(0.05);
    ntile(0.1);
    ntile(0.5);
    ntile(0.9);
    ntile(0.95);
    ntile(0.975);
    ntile(0.99);
    ntile(0.995);
    ntile(0.999);

    m=mean();
    sd(m);
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
    testt(f);
    return 0;
}
