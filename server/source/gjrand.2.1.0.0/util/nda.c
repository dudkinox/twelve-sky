/* Miscellaneous stuff for gjrand random numbers version 1.3.3.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/* Nearest Distance Analysis. */

/* There is a suspicion that the chi-squared value doesn't follow the */
/* chi-squared distribution. This is an experiment to do many tests */
/* and emperically determine the distribution. */

static long count[50][16][16];
static long last[16];

static void
init(void)
{
    int i;
    memset(count, 0, sizeof(count));
    for (i=0; i<16; i++) last[i]= -11;
}

static void
donibble(int n, long pos)
{
    int i;

    for (i=0; i<16; i++)
    {
	long d=pos-last[i];
	if (d>49) d=49;
	count[d][i][n]++;
    }

    last[n]=pos;
}

static double expect[49], expectsame[49];

static void
mkexpect(long size)
{
    int i;
    double p=1.0, x;

    for (i=1; i<=48; i++)
    {
	x=p*(1.0/16.0);
	expect[i]=x;
	p-=x;
    }
    expect[0]=p;

    p=(double)size/16.0;
    x=p-1.0;
    p-=0.5;
    for (i=0; i<=48; i++) {expectsame[i]=expect[i]*x; expect[i]*=p;}
}

/* This monster we have created contains too much statistics for the */
/* human mind to comprehend, so we summarise it all in one chi-squared */
/* number with enormous degrees of freedom. */
static double
doan(void)
{
    double t=0.0;
    int i, j, k;

    for (j=0; j<16; j++) for (k=0; k<16; k++) count[0][j][k]=count[49][j][k];

    for (i=0; i<=48; i++) for (j=0; j<16; j++) for (k=0; k<16; k++)
    {
	double x=(double)count[i][j][k];
	double e;
	if (j==k) e=expectsame[i]; else e=expect[i];
	x-=e;
	t+=x*x/e;
    }

    return t;
}

static double
readstuff(long max)
{
    long pos=0;
    int ch;

    init();

    while (pos<max && (ch=getchar())!=EOF)
    {
	donibble(ch&15, pos);
	pos++;
	if (pos>=max) break;
	donibble((ch>>4)&15, pos);
	pos++;
    }

    return doan();
}

#define SIZE 7300
static double r[SIZE+4];

static void
ntile(double n)
{
    int i=floor(n*SIZE+0.5);

    if (i<0) i=0; else if (i>=SIZE) i=SIZE-1;

    printf("%10.5f %10.5f\n", n, r[i]);
}

static double
mean(void)
{
    double m=0.0;
    int i;
    for (i=0; i<SIZE; i++) m+=r[i];
    m/=SIZE;
    printf("%10s %10.5f\n", "mean", m);
    return m;
}

static double
sd(double mean)
{
    double s=0.0;
    int i;
    for (i=0; i<SIZE; i++) {double t=r[i]-mean; s+=t*t;}
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
	double t=r[j];
	t -= mean;
	t /= sd;
	s += t*t*t;
    }
    s /= SIZE;
    printf("%10s %10.5f\n", "skew", s);
}

static int
dcmp(const void *a, const void *b)
{
    double da= * (const double *) a;
    double db= * (const double *) b;

    if (da<db) return -1;
    if (da==db) return 0;
    return 1;
}

#define FILESIZE 140000

int
main(int argc, char **argv)
{
    double m, s;
    int i;

    mkexpect(FILESIZE);
    init();

    for (i=0; i<SIZE; i++) r[i]=readstuff(FILESIZE);

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
    s = sd(m);
    skew(m, s);

    return 0;
}
