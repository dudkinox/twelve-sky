/* Test software for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* Take 5 32 bit words at a time and look at their ordering */
/* There are 5! possibilities. Treat results as a stream of data 0-119 */
/* and do a Kendall and Babington-Smith Gap Test on this stream. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../../src/my_int_types.h"
#include "../../testcommon/chi2p.h"

static int errorlevel=0;
static void seterr(int e) {if (errorlevel<e) errorlevel=e;}

#define FAC (5*4*3*2)

static int32_t pos32[FAC];

#define MAXGAP 366
static int64_t dc[MAXGAP];

static void
mkexpect(int64_t pos, double expect[MAXGAP])
{
    double x, y;
    int i;

    x=pos;
    for (i=1; i<MAXGAP; i++)
    {
	y=x*(1.0/FAC);
	expect[i]=y;
	x-=y;
    }

    expect[0]=x;
}

static void
doan(int64_t pos)
{
    double p;
    double expect[MAXGAP], t=0.0;
    int i;

    mkexpect(pos, expect);

    for (i=0; i<MAXGAP; i++)
    {
	double e=expect[i];
	double x=((double)(dc[i]))-e;
	t+=x*x/e;
    }

    printf("%15.5f\n\n", (double)t);

    p = chi2p1(t, MAXGAP-2);

    printf("One sided P value (very small numbers are bad)\n");
    printf("P = %.3g\n", p);
}

/* If you want to go fast on a RISC chip, totally rewrite this function. */
/* Put all 5 numbers into registers, and then try to get some parallelism. */
static int
order(const uint32_t b[5])
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

    return r;
}

static void
dobuf(const uint32_t * const buf, int n)
{
    const uint32_t *b=buf;
    int i, j, ch;

    for (i=0; i<FAC; i++)
    {
	j=pos32[i]-n;
	if (j< -20000) j= -20000;
	pos32[i]=j;
    }

    i= -n;
    do
    {
	ch=order(b);
	b+=5;
	j=i-pos32[ch];
	if (j>=MAXGAP) j=0;
	pos32[ch]=i;
	dc[j]++;
	i++;
    }
    while (i<0);
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
/* must be multiple of 5 */
#define MYBUFSIZ (5*1024)
    uint32_t buf[MYBUFSIZ];
    int64_t p=0;
    int i;

    memset(dc, 0, sizeof(dc));
    for (i=0; i<FAC; i++) pos32[i]= -76;

    while (max<=0 || p<max)
    {
	i=fread(buf, sizeof(uint32_t)*5, MYBUFSIZ/5, stdin);
	if (max>0 && i>max-p) i=max-p;
	if (i<=0) break;
	if (byteswap) dobyteswap(buf, 5*i);
	dobuf(buf, i);
	p+=i;
    }

    if (max>0 && p<max)
    {
	fprintf(stderr,
		"Warning, expected %.0f 160 bit samples, saw only %.0f\n",
		(double)max, (double)p);
	seterr(1);
    }

    if (p<25000)
    {
	fprintf(stderr, "Warning, < 25000 samples, don't trust result.\n");
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
    else max=(int64_t)((1.0/20.0)*dmax);

    printf("expected range [ 286 304 329 399 430 454 ]\n");

    readstuff(max);

    return errorlevel;
}
