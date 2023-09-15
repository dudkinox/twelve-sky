/* Test software for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* "Kendall and Babington-Smith Gap Test" on 8 bit things assembled from */
/* 2 bit slices of 4 consecutive 32 bit words. */
/* This is most likely to be useful on generators which produce 32 bit words */
/* with some bits much worse quality than others. I am thinking mostly */
/* of linear congruential generators, but perhaps it will catch others too. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../../src/my_int_types.h"
#include "../../testcommon/pcombo.h"
#include "../../testcommon/binom.h"
#include "../../testcommon/chi2p.h"

static int errorlevel=0;
static void seterr(int e) {if (errorlevel<e) errorlevel=e;}

static int64_t dc[16][751];
static int pos32[16][256];

static void
mkexpect(double expect[751], int64_t pos)
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

static void
doan(int64_t pos)
{
    double expect[751];
    double p[16], extreme;
    int i, j, exi, exj, exd;

    mkexpect(expect, pos);

    for (j=0; j<16; j++)
    {
	int64_t *d= &(dc[j][0]);
	double e, x, t=0.0;
	for (i=0; i<751; i++)
	{
	    e=expect[i];
	    x=(double)d[i]-e;
	    t+=x*x/e;
	}
	printf("%2d : %15.5f\n", 2*j, t);

	p[j] = chi2p1(t, 749);
    }

    p[0]=pcombo(p, 16);

    extreme=999.0; exi= -1; exj= -1; exd='?';
    for (j=0; j<16; j++)
    {
	int64_t tot=pos;
	for (i=1; i<750; i++)
	{
	    int64_t k=dc[j][i];
	    double x=sumbino(tot, k, 1.0/256.0);
	    if (x<extreme)
	    {
		extreme=x; exi=i; exj=j;
		if (k*256>tot) exd='+'; else exd='-';
	    }
	    tot-=k;
	}
    }
    printf("extreme = %.3g (%d %d %c)\n", extreme, 2*exj, exi, exd);
    p[1]=pco_scale(extreme, 16*749);

    printf("pvals (%.3g %.3g)\n\n", p[0], p[1]);

    printf("One sided P value (very small numbers are bad)\n");
    printf("P = %.3g\n", pcombo(p, 2));
}

static void
dobuf(const uint32_t * const buf, int n)
{
    int i, j, k;
    const uint32_t * const bend=buf+4*n;

    for (k=0; k<16; k++)
    {
	for (i=0; i<256; i++)
	{
	    j=pos32[k][i]-n;
	    if (j< -20000) j= -20000;
	    pos32[k][i]=j;
	}
    }

    i= -n;
    do
    {
	uint32_t a=bend[4*i], b=bend[4*i+1], c=bend[4*i+2], d=bend[4*i+3];
	uint32_t part;

#define DOPART(k) \
	j=i-pos32[k][part]; \
	if (j>=751) j=0; \
	pos32[k][part]=i; \
	dc[k][j]++;

	part=((a<<6)&0xc0) | ((b<<4)&0x30) | ((c<<2)&0xc) | (d&0x3);
	DOPART(0);

	part=((a<<4)&0xc0) | ((b<<2)&0x30) | (c&0xc) | ((d>>2)&0x3);
	DOPART(1);

	part=((a<<2)&0xc0) | (b&0x30) | ((c>>2)&0xc) | ((d>>4)&0x3);
	DOPART(2);

	part=(a&0xc0) | ((b>>2)&0x30) | ((c>>4)&0xc) | ((d>>6)&0x3);
	DOPART(3);

	part=((a>>2)&0xc0) | ((b>>4)&0x30) | ((c>>6)&0xc) | ((d>>8)&0x3);
	DOPART(4);

	part=((a>>4)&0xc0) | ((b>>6)&0x30) | ((c>>8)&0xc) | ((d>>10)&0x3);
	DOPART(5);

	part=((a>>6)&0xc0) | ((b>>8)&0x30) | ((c>>10)&0xc) | ((d>>12)&0x3);
	DOPART(6);

	part=((a>>8)&0xc0) | ((b>>10)&0x30) | ((c>>12)&0xc) | ((d>>14)&0x3);
	DOPART(7);

	part=((a>>10)&0xc0) | ((b>>12)&0x30) | ((c>>14)&0xc) | ((d>>16)&0x3);
	DOPART(8);

	part=((a>>12)&0xc0) | ((b>>14)&0x30) | ((c>>16)&0xc) | ((d>>18)&0x3);
	DOPART(9);

	part=((a>>14)&0xc0) | ((b>>16)&0x30) | ((c>>18)&0xc) | ((d>>20)&0x3);
	DOPART(10);

	part=((a>>16)&0xc0) | ((b>>18)&0x30) | ((c>>20)&0xc) | ((d>>22)&0x3);
	DOPART(11);

	part=((a>>18)&0xc0) | ((b>>20)&0x30) | ((c>>22)&0xc) | ((d>>24)&0x3);
	DOPART(12);

	part=((a>>20)&0xc0) | ((b>>22)&0x30) | ((c>>24)&0xc) | ((d>>26)&0x3);
	DOPART(13);

	part=((a>>22)&0xc0) | ((b>>24)&0x30) | ((c>>26)&0xc) | ((d>>28)&0x3);
	DOPART(14);

	part=((a>>24)&0xc0) | ((b>>26)&0x30) | ((c>>28)&0xc) | ((d>>30)&0x3);
	DOPART(15);


#undef DOPART

	i++;
    }
    while (i<0);
}

static void
readstuff(int64_t max)
{
#define MYBUFSIZ 1024
    uint32_t buf[MYBUFSIZ];
    int64_t p=0;
    int i;

    memset(dc, 0, sizeof(dc));
    for (i=0; i<16; i++)
    {
	int j;
	for (j=0; j<256; j++) pos32[i][j] = -275;
    }

    while (max<=0 || p<max)
    {
	i=fread(buf, sizeof(uint32_t)*4, MYBUFSIZ/4, stdin);
	if (i<=0) break;
	if (max>0 && i>max-p) i=max-p;
	dobuf(buf, i);
	p+=i;
    }

    if (max>0 && p<max)
    {
	fprintf(stderr,
		"Warning, expected %.0f 128 bit samples, saw only %.0f\n",
		(double)max, (double)p);
	seterr(1);
    }

    if (p<50000)
    {
	fprintf(stderr, "Warning, < 50000 samples, don't trust result.\n");
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
    else max=(int64_t)((1.0/16)*dmax);

    /* approximate, determined emperically. */
    printf("expected range [ 633 662 699 799 841 875 ]\n");

    readstuff(max);

    return errorlevel;
}
