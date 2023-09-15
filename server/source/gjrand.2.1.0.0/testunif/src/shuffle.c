/* Test software for gjrand random numbers version 1.3.3.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* Take 8 32 bit words at a time and look at their ordering */
/* There are 8! possibilities. Count occurences. Chi-squared test at the end. */
/* DIEHARD does one of these but a bit differently. */
/* In DIEHARD: only 5 words to a sequence; sequences overlap; this */
/* requires some horrible analysis of covariance matrices and weak inverses. */

static int errorlevel=0;
static void seterr(int e) {if (errorlevel<e) errorlevel=e;}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../../src/my_int_types.h"
#include "../../testcommon/chi2p.h"

#define FAC (8*7*6*5*4*3*2)

static int64_t dc[FAC];

static void
doan(int64_t pos)
{
    double expect, t=0.0, p;
    int i;

    expect=((double)pos)/FAC;

    for (i=0; i<FAC; i++)
    {
	double x=((double)(dc[i]))-expect;
	t+=x*x/expect;
    }

    printf("%15.5f\n\n", t);

    p = chi2p2(t, FAC-1);

    printf("One sided P value (very small numbers are bad)\n");
    printf("P = %.3g\n", p);
}

/* If you want to go fast on a RISC chip, totally rewrite this function. */
/* Put all 8 numbers into registers, and then try to get some parallelism. */
static int
order(const uint32_t b[8])
{
    uint32_t x;
    unsigned r=b[1]<b[0];

    r*=3;
    x=b[2];
    r+=x<b[1]; r+=x<b[0];

    r*=4;
    x=b[3];
    r+=x<b[2]; r+=x<b[1]; r+=x<b[0];

    r*=5;
    x=b[4];
    r+=x<b[3]; r+=x<b[2]; r+=x<b[1]; r+=x<b[0];

    r*=6;
    x=b[5];
    r+=x<b[4]; r+=x<b[3]; r+=x<b[2]; r+=x<b[1]; r+=x<b[0];

    r*=7;
    x=b[6];
    r+=x<b[5]; r+=x<b[4]; r+=x<b[3]; r+=x<b[2]; r+=x<b[1]; r+=x<b[0];

    r*=8;
    x=b[7];
    r+=x<b[6]; r+=x<b[5]; r+=x<b[4]; r+=x<b[3]; r+=x<b[2]; r+=x<b[1]; r+=x<b[0];

    return r;
}

static void
dobuf(const uint32_t * const buf, int n)
{
    const uint32_t *b=buf;
    const uint32_t * const bend=b+8*n;

    do
    {
	dc[order(b)]++;
	b+=8;
    }
    while (b<bend);
}

static void
dobyteswap(uint32_t *buf, int n)
{
    char *b=(char *)buf, *bend=b+4*n;

    do
    {
	char x, y;

	x=b[0]; y=b[3]; b[3]=x; b[0]=y;
	x=b[1]; y=b[2]; b[2]=x; b[1]=y;

	b+=4;
    } while (b<bend);
}

static int byteswap=0;

static void
readstuff(int64_t max)
{
#define MYBUFSIZ 1024
    uint32_t buf[MYBUFSIZ];
    int64_t p=0;
    int i;

    memset(dc, 0, sizeof(dc));

    while (max<=0 || p<max)
    {
	i=fread(buf, sizeof(uint32_t)*8, MYBUFSIZ/8, stdin);
	if (max>0 && i>max-p) i=max-p;
	if (i<=0) break;
	if (byteswap) dobyteswap(buf, 8*i);
	dobuf(buf, i);
	p+=i;
    }

    if (max>0 && p<max)
    {
	fprintf(stderr,
		"Warning, expected %.0f 256 bit samples, saw only %.0f\n",
		(double)max, (double)p);
	seterr(1);
    }

    if (p<310000)
    {
	fprintf(stderr, "Warning, < 310000 samples, don't trust result.\n");
	seterr(1);
    }

    doan(p);
#undef MYBUFSIZ
}

int
main(int argc, char **argv)
{
    double dmax;
    int64_t max;
    int arg=1;

    if (argc>1 && strcmp(argv[1], "-r")==0) {byteswap=1; arg++;}

    if (arg>=argc || sscanf(argv[arg], "%lf", &dmax)!=1 || dmax<1.0) max= -1;
    else max=(int64_t)((1.0/32.0)*dmax);

    /* Normal approx to chi-squared */
    {
	double sq=(int)sqrt((double)(2*FAC-2));
	printf("expected range [ %.0f %.0f %.0f %.0f %.0f %.0f ]\n",
	    FAC-0.5-3.09*sq, FAC-0.5-2.33*sq, FAC-0.5-1.28*sq,
	    FAC-0.5+1.28*sq, FAC-0.5+2.33*sq, FAC-0.5+3.09*sq);
    }

    readstuff(max);

    return errorlevel;
}
