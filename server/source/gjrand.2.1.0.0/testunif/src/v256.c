/* Test software for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../../src/my_int_types.h"
#include "../../testcommon/pcombo.h"
#include "../../testcommon/chi2p.h"

/* Each test generates a block of 256 bytes and counts how many distinct */
/* byte values exist in it. */
/* This is repeated for each block and the counts accumulate. */
/* When finished, do chi-squared test, and print extrema. */

/* Only whole blocks are tested. If a partial block remains at the end, */
/* it is ignored. */

/* This is Geronimo's invention dating from September 2005. */
/* If this is actually an older known method, and especially if it has */
/* a name, i would like to hear. */

static int errorlevel=0;
static void seterr(int e) {if (errorlevel<e) errorlevel=e;}

#define LO 145
#define HI 178

static int64_t count[257];

static void
mkexpect(double expect[257], int64_t pos)
{
    double t[257];
    int i, j;

    for (i=0; i<257; i++) expect[i]=0.0;
    expect[1]=(double)(pos>>8);

    for (i=2; i<=256; i++)
    {
	for (j=0; j<257; j++) t[j]=expect[j];
	for (j=0; j<257; j++) expect[j]=0.0;

	for (j=1; j<i; j++)
	{
	    expect[j]+=t[j]*((double)j/256.0);
	    expect[j+1]+=t[j]*((double)(256-j)/256.0);
	}
    }
}

static void
compact(double expect[])
{
    int64_t ag;
    double xag;
    int i;

    ag=0; xag=0.0;
    for (i=0; i<=LO; i++) {ag+=count[i]; xag+=expect[i];}
    count[LO]=ag; expect[LO]=xag;
    for (i=0; i<LO; i++) {count[i]=0; expect[i]=0.0;}

    ag=0; xag=0.0;
    for (i=256; i>=HI; i--) {ag+=count[i]; xag+=expect[i];}
    count[HI]=ag; expect[HI]=xag;
    for (i=256; i>HI; i--) {count[i]=0; expect[i]=0.0;}
}

static double
oneside(double x, double y)
{
    double r;

    if (x<0.5)
    {
	if (y>0.5) return 1.0;
	return 2.0*y;
    }
    return 2.0*(1.0-x);

    return r;
}

static void
doan(int64_t pos)
{
    double p[3], s;
    double expect[257];
    double e, t=0.0, x, y;
    int i, m;

    mkexpect(expect, pos);

    m=256;
    for (i=255; i>0; i--) if (count[i]) m=i;
    x=0.0;
    for (i=1; i<m; i++) x+=expm1(-expect[i])*(x-1.0);
    y=expm1(-expect[m])*(x-1.0)+x;
    printf("minimum = %d  (prob = %15.10f %15.10f)\n", m, x, y);

    p[0]=oneside(x, y);

    m=1;
    for (i=2; i<=256; i++) if (count[i]) m=i;
    x=0.0;
    for (i=256; i>m; i--) x+=expm1(-expect[i])*(x-1.0);
    y=expm1(-expect[m])*(x-1.0)+x;
    printf("maximum = %d  (prob = %15.10f %15.10f)\n\n", m, x, y);

    p[1]=oneside(x, y);

    compact(expect);

    for (i=1; i<257; i++)
    {
	e=expect[i];
	x=(double)(count[i])-e;
	t+=x*x/(e+0.0001);
    }

    printf("expected range [ 14.0 17.8 23.9 44.9 56.1 65.3 ]\n");

    printf("chi-squared = %15.5f\n\n", t);

    /* This one is yet another bad normal approximation on the high-side */
    /* tail. */
    p[2] = chi2p1(t, HI-LO);

    s=pcombo(p, 3);
    printf("One sided P value (very small numbers are bad)\n");
    printf("P = %.3g\n", s);
}

static int
dobuf(const unsigned char * const buf)
{
    int i, i2;
    const unsigned char *j;
    static unsigned char c[256];

    memset(c, 0, 256*sizeof(unsigned char));

    j=buf+252;
    do
    {
	c[j[3]]=1;
	c[j[2]]=1;
	c[j[1]]=1;
	c[j[0]]=1;
	j-=4;
    } while (j>=buf);

    i=0; i2=0;

    j=c+252;
    do
    {
	i+=j[3];
	i2+=j[2];
	i+=j[1];
	i2+=j[0];
	j-=4;
    } while (j>=c);

    return i+i2;
}

static void
readstuff(int64_t max)
{
#define MYBUFSIZ 8192
    unsigned char buf[MYBUFSIZ];
    int64_t p=0;
    int i;

    for (i=0; i<257; i++) count[i]=0;

    while (max<0 || p<max)
    {
	i=fread(buf, 1, MYBUFSIZ, stdin);
	if (i<=0) break;
	if (max>0 && i>max-p) i=max-p;
	i &= (~255);
	p+=i;
	do
	{
	    i-=256;
	    count[dobuf(buf+i)]++;
	}
	while (i>0);
    }

    if (max>0 && p<max)
    {
	fprintf(stderr, "Warning, expected %.0f samples, saw only %.0f\n",
		(double)max, (double)p);
	seterr(1);
    }

    if (p<1000000)
    {
	fprintf(stderr, "Warning, < 1000000 samples, don't trust result.\n");
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

    if (argc<2 || sscanf(argv[1], "%lf", &dmax)!=1 || dmax<1.0) max= -1;
    else max=(int64_t)dmax;

    if (max>0) max&= (~(int64_t)255); /* truncate to multiple of 256 */

    readstuff(max);

    return errorlevel;
}
