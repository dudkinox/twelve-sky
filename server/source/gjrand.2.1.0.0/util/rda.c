/* Miscellaneous stuff for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/* repeat distance analysis on byte values. */

/* This is another weird variant inspired by Maurer's universal statistical */
/* test. For each byte read, calculate the distance since the same byte */
/* value was last seen and collect a histogram of these distances. */
/* Finally do a chi-square test on the histogram. */

/* As with nda, this doesn't follow chi-square distribution. */
/* This one emperically finds expected value range. */

long pos[256], dc[751];

void
mkexpect(double expect[751], long pos)
{
    double c=(double)pos, e=c/256.0;
    int i;

    for (i=1; i<751; i++)
    {
	expect[i]=e;
	c-=e;
	e *= 255.0/256.0;
    }

    expect[0]=c;
}

double
doan(long pos)
{
    double expect[751];
    double e, t=0.0, x;
    int i;

    mkexpect(expect, pos);

    for (i=0; i<751; i++)
    {
	e=expect[i];
	x=(double)dc[i]-e;
	t+=x*x/e;
    }

    return t;
}

double
readstuff(long max)
{
    long p=0;
    int i, j, k;
    unsigned char buf[4096];

    for (i=0; i<256; i++) pos[i]=0;
    for (i=0; i<751; i++) dc[i]=0;

    while (1)
    {
	if (fread(buf, 1, 4096, stdin)!=4096)
	    {fprintf(stderr, "fwrite\n"); exit(1);}
	for (k=0; k<4096; k++)
	{
	    i=buf[k];
	    j=p-pos[i];
	    if (j>750) j=0;
	    dc[j]++;
	    pos[i]=p;
	    p++;
	    if (p>=max) goto ret;
	}
    }

    ret:
    return doan(p);
}

#define SIZE 40000

double r[SIZE];

void
ntile(double n)
{
    int i=floor(n*SIZE+0.5);

    if (i<0) i=0; else if (i>=SIZE) i=SIZE-1;

    printf("%10.5f %10.5f\n", n, r[i]);
}

double
mean(void)
{
    double m=0.0;
    int i;
    for (i=0; i<SIZE; i++) m+=r[i];
    m/=SIZE;
    printf("%10s %10.5f\n", "mean", m);
    return m;
}

void
sd(double mean)
{
    double s=0.0;
    int i;
    for (i=0; i<SIZE; i++) {double t=r[i]-mean; s+=t*t;}
    printf("%10s %10.5f\n", "sdev", sqrt(s/SIZE));
}

int
dcmp(const void *a, const void *b)
{
    double da= * (const double *) a;
    double db= * (const double *) b;

    if (da<db) return -1;
    if (da==db) return 0;
    return 1;
}


int
main(int argc, char **argv)
{
    int i;
    double m;

    for (i=0; i<SIZE; i++) r[i]=readstuff(60000);

    qsort(r, SIZE, sizeof(double), dcmp);

    ntile(0.001);
    ntile(0.01);
    ntile(0.1);
    ntile(0.15);
    ntile(0.2);
    ntile(0.3);
    ntile(0.5);
    ntile(0.7);
    ntile(0.8);
    ntile(0.85);
    ntile(0.9);
    ntile(0.95);
    ntile(0.975);
    ntile(0.99);
    ntile(0.995);
    ntile(0.999);

    m=mean();
    sd(m);

    return 0;
}
