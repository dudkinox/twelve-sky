/* Test software for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* This is tests related to the balance of 0 and 1 bits in the test data. */
/* This is new with gjrand 1.0.0.0 (though unfinished drafts have been */
/* lurking in division2/ for a few versions). I should really have put in a */
/* test like this much earlier, but i found it hard to believe that any */
/* seriously proposed generator would fail such an obvious test. Silly me. */
/* The fifth moment tests were added in 1.3.0.0 . */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../../src/my_int_types.h"
#include "../../testcommon/pcombo.h"

#ifdef __cplusplus
#define NO_ARGS
#else
#define NO_ARGS void
#endif

static int errorlevel=0;
static void seterr(int e) {if (errorlevel<e) errorlevel=e;}

static unsigned char ones[256];

static double totone=0.0;
static double var[9]={0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static double skew[9]={0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static double five[9]={0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

static double xtot, xvar[9], xskew[9], xfive[9];

static double *
mkex1(int coins, int64_t count)
{
    static double p1[32771], p2[32771];
    double *x1=p1, *x2=p2, *tmp, t;
    int i, j;
    int hi=0, lo=0;

    for (i=0; i<=coins; i++) x1[i]=x2[i]=0.0;
    x1[0]=count; /* with zero coins thrown, zero heads always results */

    for (i=1; i<=coins; i++)
    {
	for (j=lo; j<=hi; j++) {t=x1[j]*0.5; x2[j]+=t; x2[j+1]=t;}
	hi++;
	for (j=lo; j<=hi; j++) x1[j]=0.0;
	while (lo<hi && x2[lo]<1e-30) lo++;
	while (hi>lo && x2[hi]<1e-30) hi--;

	tmp=x1; x1=x2; x2=tmp;
    }

    return x1;
}

static double
getvar(double *p, int coins, int64_t count)
{
    double v=coins*0.25;
    double r=0.0;
    int i, mx=coins/2;

    for (i=0; i<=coins; i++)
    {
	double t=i-mx;
	t*=t;
	t-=v;
	t*=t;
	r+=p[i]*t;
    }

    return sqrt(r)/count;
}

static double
getskew(double *p, int coins, int64_t count)
{
    double r=0.0;
    int i, mx=coins/2;

    for (i=0; i<=coins; i++)
    {
	double t=i-mx;
	t*=t*t;
	t*=t;
	r+=p[i]*t;
    }

    return sqrt(r)/count;
}

static double
getfive(double *p, int coins, int64_t count)
{
    double r=0.0;
    int i, mx=coins/2;

    for (i=0; i<=coins; i++)
    {
	double t=i-mx;
	t*=(t*t)*(t*t);
	t*=t;
	r+=p[i]*t;
    }

    return sqrt(r)/count;
}

static void
mkexpect(int64_t pos)
{
    double *p;
    int i;

    xtot=sqrt(pos*8.0)*0.5;
    for (i=0; i<9; i++)
    {
	p=mkex1(128<<i, pos/(16<<i));
	xvar[i]=getvar(p, 128<<i, pos/(16<<i));
	xskew[i]=getskew(p, 128<<i, pos/(16<<i));
	xfive[i]=getfive(p, 128<<i, pos/(16<<i));
    }
}

static void
doan(int64_t pos)
{
    double pm, pvar[9], pskew[9], pfive[9], x, y, z;
    int i;

    mkexpect(pos);

    x=totone/xtot;
    pm=erfc(fabs(M_SQRT1_2*x));
    printf("total bitcount discrepency %.3f sigma, (p = %.3g)\n", x, pm);

    printf("block   var/sigma          p    skew/sigma          p    five/sigma          p\n");
    for (i=0; i<9; i++)
    {
	x=var[i]*(16<<i)/pos;
	x-=(32<<i);
	x/=xvar[i];
	pvar[i]=erfc(fabs(M_SQRT1_2*x));

	y=skew[i]*(16<<i)/pos;
	y/=xskew[i];
	pskew[i]=erfc(fabs(M_SQRT1_2*y));

	z=five[i]*(16<<i)/pos;
	z/=xfive[i];
	pfive[i]=erfc(fabs(M_SQRT1_2*z));

	printf("%5d: %10.3f (%9.3g) ; %10.3f (%9.3g) ; %10.3f (%9.3g)\n",
		128<<i, x, pvar[i], y, pskew[i], z, pfive[i]);
    }
    pvar[0]=pcombo(pvar, 9);
    pvar[1]=pcombo(pskew, 9);
    pvar[2]=pcombo(pfive, 9);
    pvar[3]=pm;
    printf("(%.3g, %.3g, %.3g)\n", pvar[0], pvar[1], pvar[2]);

    x=pcombo(pvar, 4);
    printf("\nOne sided P value (very small numbers are bad)\n");
    printf("P = %.3g\n", x);
}

static double
do16(const unsigned char * const b)
{
    register double r, s, c;
    const uint32_t * const x=(const uint32_t * const)b;
    uint32_t v=x[0], w=x[1], t=x[2], u=x[3];

    v = ((v>>1)&0x55555555) + (v&0x55555555);
    w = ((w>>1)&0x55555555) + (w&0x55555555);
    t = ((t>>1)&0x55555555) + (t&0x55555555);
    u = ((u>>1)&0x55555555) + (u&0x55555555);

    v = ((v>>2)&0x33333333) + (v&0x33333333);
    w = ((w>>2)&0x33333333) + (w&0x33333333);
    t = ((t>>2)&0x33333333) + (t&0x33333333);
    u = ((u>>2)&0x33333333) + (u&0x33333333);
    v += t; w += u;

    t = v>>4; u = w>>4; v &= 0x0f0f0f0f; w &= 0x0f0f0f0f;
    t &= 0x0f0f0f0f; u &= 0x0f0f0f0f; v+=t; w+=u;
    v+=w;

    w = v>>8; v &= 0x00ff00ff; w &= 0x00ff00ff; v+=w;

    w = v>>16; v &= 0x0000ffff; v+=w;

    r=((int)v)-64;

    s=r*r;
    var[0]+=s;
    c=s*r;
    skew[0]+=c;
    five[0]+=c*s;

    return r;
}

static double
do32(const unsigned char *b)
{
    register double r, s, c;
    r=do16(b); r+=do16(b+16);
    s=r*r;
    var[1]+=s;
    c=s*r;
    skew[1]+=c;
    five[1]+=c*s;
    return r;
}

static double
do64(const unsigned char *b)
{
    register double r, s, c;
    r=do32(b); r+=do32(b+32);
    s=r*r;
    var[2]+=s;
    c=s*r;
    skew[2]+=c;
    five[2]+=c*s;
    return r;
}

static double
do128(const unsigned char *b)
{
    register double r, s, c;
    r=do64(b); r+=do64(b+64);
    s=r*r;
    var[3]+=s;
    c=s*r;
    skew[3]+=c;
    five[3]+=c*s;
    return r;
}

static double
do256(const unsigned char *b)
{
    register double r, s, c;
    r=do128(b); r+=do128(b+128);
    s=r*r;
    var[4]+=s;
    c=s*r;
    skew[4]+=c;
    five[4]+=c*s;
    return r;
}

static double
do512(const unsigned char *b)
{
    register double r, s, c;
    r=do256(b); r+=do256(b+256);
    s=r*r;
    var[5]+=s;
    c=s*r;
    skew[5]+=c;
    five[5]+=c*s;
    return r;
}

static double
do1024(const unsigned char *b)
{
    register double r, s, c;
    r=do512(b); r+=do512(b+512);
    s=r*r;
    var[6]+=s;
    c=s*r;
    skew[6]+=c;
    five[6]+=c*s;
    return r;
}

static double
do2048(const unsigned char *b)
{
    register double r, s, c;
    r=do1024(b); r+=do1024(b+1024);
    s=r*r;
    var[7]+=s;
    c=s*r;
    skew[7]+=c;
    five[7]+=c*s;
    return r;
}

static void
do4096(const unsigned char *b)
{
    register double r, s, c;
    r=do2048(b); r+=do2048(b+2048);
    totone+=r;
    s=r*r;
    var[8]+=s;
    c=s*r;
    skew[8]+=c;
    five[8]+=c*s;
}

static void
mkones(NO_ARGS)
{
    unsigned i;

    for (i=0; i<256; i++) ones[i]=
	((i>>7)&1) + ((i>>6)&1) + ((i>>5)&1) + ((i>>4)&1)
	+ ((i>>3)&1) + ((i>>2)&1) + ((i>>1)&1) + (i&1);
}

static void
readstuff(int64_t max)
{
    uint32_t buf[4096];
    int64_t p=0;
    int i;

    mkones();

    while (max<0 || p<max)
    {
	i=fread(buf, 4096, 1, stdin);
	if (i!=1) break;
	p+=4096;
	do4096((const unsigned char *)buf);
    }

    if (max>0 && p<max)
    {
	fprintf(stderr, "Warning, expected %.0f bytes, saw only %.0f\n",
		(double)max, (double)p);
	seterr(1);
    }

    if (p<50000) /* HERE This is still a guess. But mostly tests ok. */
    {
	fprintf(stderr, "Warning, < 50000 bytes, don't trust result.\n");
	seterr(1);
    }

    doan(p);
}

int
main(int argc, char **argv)
{
    double dmax;
    int64_t max;

    if (argc<2 || sscanf(argv[1], "%lf", &dmax)!=1 || dmax<1.0) max= -1;
    else max=(int64_t)dmax;

    if (max>0) max&= (~(int64_t)4095); /* truncate to multiple of 4096 */

    readstuff(max);

    return errorlevel;
}
