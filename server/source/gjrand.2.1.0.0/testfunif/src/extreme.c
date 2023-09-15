/* Test software for gjrand random numbers version 1.3.3.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* This looks at the number of extreme values in each block of numbers. */
/* It is able to eventually crack straw/lfib3l in 200 dimensions. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../../src/my_int_types.h"
#include "../../testcommon/chi2p.h"

/* The expected number of high (and low) numbers per block. */
/* The thresholds vary with block size to achieve this. */
#define PERBLOCK (3.0)

static int errorlevel=0;
static void seterr(int e) {if (errorlevel<e) errorlevel=e;}

static void
crash(char *s)
{
    seterr(1);
    fprintf(stderr, "crash [%s]\n", s); exit(errorlevel);
}

/* Counts >= MAX-1 are aggregated. */
#define MAX 8
static int64_t histo[MAX][MAX];
static double buf[4096];

static void
doblock(int size)
{
    register const double lo=PERBLOCK/size, hi=1.0-lo;
    register double *b=buf+size;
    register double x;
    int i= -size, h=0, l=0;

    do
    {
	x=b[i];
	l += x<lo;
	h += x>hi;
	i++;
    } while (i<0);

    if (l>MAX-1) l=MAX-1;
    if (h>MAX-1) h=MAX-1;
    histo[l][h]++;
}

static int64_t
dostuff(int size, int64_t n)
{
    int64_t r=0;

    memset(histo, 0, sizeof(histo));

    while (n<0 || r<n)
    {
	if (fread(buf, sizeof(double), size, stdin)!=(size_t)size) break;
	doblock(size);
	r++;
    }

    if (n>0 && r<n)
    {
	fprintf(stderr, "expected %.0f blocks, saw only %.0f\n",
	    (double)n, (double)r );
	seterr(1);
    }

    return r;
}

static double expect[MAX][MAX];

static void
update(double in[MAX][MAX], double out[MAX][MAX], double p0)
{
    double p1=1.0-2.0*p0;
    int i, j;

    for (i=0; i<MAX; i++) for (j=0; j<MAX; j++) out[i][j]=in[i][j]*p1;

    for (i=0; i<MAX-1; i++) for (j=0; j<MAX; j++) out[i+1][j]+=in[i][j]*p0;
    for (j=0; j<MAX; j++) out[MAX-1][j]+=in[MAX-1][j]*p0;

    for (i=0; i<MAX; i++)
    {
	for (j=0; j<MAX-1; j++) out[i][j+1]+=in[i][j]*p0;
	out[i][MAX-1]+=in[i][MAX-1]*p0;
    }
}

static void
mkexpect(int s, int64_t n)
{
    double x1[MAX][MAX], x2[MAX][MAX];
    int i, j, r=0;
    const double p0=PERBLOCK/s;

    for (i=0; i<MAX; i++) for (j=0; j<MAX; j++) x1[i][j]=0;
    x1[0][0]=n;

    for (i=0; i<s; i++)
    {
	if (r) {update(x2, x1, p0); r=0;}
	else {update(x1, x2, p0); r=1;}
    }

    if (r) for (i=0; i<MAX; i++) for (j=0; j<MAX; j++) expect[i][j]=x2[i][j];
    else for (i=0; i<MAX; i++) for (j=0; j<MAX; j++) expect[i][j]=x1[i][j];

    if (expect[MAX-1][MAX-1]<1.0 || expect[MAX-1][MAX-2]<5.0)
	printf("Warning expected counts too small (%.2f and %.2f)\n"
	    "should be at least (1.0 and 5.0) for reliable chi-squared.\n"
	    "Don't trust results. Try using more data.\n",
	    expect[MAX-1][MAX-1], expect[MAX-1][MAX-2]);

}

static void
doan(int s, int64_t n)
{
    double x, t, chis=0.0;
    int i, j;
    mkexpect(s, n);

    for (i=0; i<MAX; i++) for (j=0; j<MAX; j++)
    {
	x=expect[i][j];
	t=histo[i][j]-x;
	chis+=t*t/x;
    }

    printf("expected range [ 33.9 39.8 49.1 77.8 92.1 103.4 ]\n");
    printf("chis = %10.5f\n", chis);

    printf("\nOne sided P value (very small numbers are bad)\n");
    printf("P = %.3g\n", chi2p1(chis, 63));
}

int
main(int argc, char **argv)
{
    double dn;
    int64_t n, r;
    int s;

    if (argc<2) crash("one or two args: block-size [ word-count ]");
    if (sscanf(argv[1], "%d", &s)!=1 || s<20 || s>4096)
	crash("first arg (block-size) must be numeric, 20 <= size <= 4096");

    if (argc<3) n= -1;
    else
    {
	if (sscanf(argv[2], "%lf", &dn)!=1)
	    crash("second optional arg (word-count) must be numeric");
	n=(int64_t)(dn/s);
    }

    r=dostuff(s, n);
    doan(s, r);

    return errorlevel;
}
