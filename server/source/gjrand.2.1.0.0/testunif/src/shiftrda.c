/* Test software for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../../src/my_int_types.h"
#include "../../testcommon/pcombo.h"
#include "../../testcommon/binom.h"
#include "../../testcommon/chi2p.h"

/* "Kendall and Babington-Smith Gap Test" on byte values. */

/* This one hacked to do unaligned bytes. */

/* For each byte read, calculate the distance since the same byte */
/* value was last seen and collect a histogram of these distances. */
/* Finally do a chi-square test on the histogram. */

/* This appears as if it might be a chi-square distribution but with */
/* 748 or 749 degrees of freedom where 750 might be expected. */
/* "expected range" is approximate, +- 4 */

static int errorlevel=0;
static void seterr(int e) {if (errorlevel<e) errorlevel=e;}

static int64_t dc[15][752];
static unsigned short dc16[15][752];
static int pos32[256];

static void
mkexpect(double expect[752], int64_t pos)
{
    double c=(double)pos, e=c/256.0;
    int i;

    for (i=1; i<751; i++)
    {
	expect[i]=e;
	c-=e;
	e *= 255.0/256.0;
    }

    expect[751]=c;
}

static void
doan(int64_t pos)
{
    double expect[752];
    double pv[15], pvs[2];
    double e, t, x, extreme;
    int i, j, exi, exj, exd;

    mkexpect(expect, pos);

    for (j=0; j<15; j++)
    {
	t=0.0;
	for (i=1; i<=751; i++)
	{
	    e=expect[i];
	    x=(double)(dc[j][i])-e;
	    t+=x*x/e;
	}

	printf("%15.5f\n", t);

	pv[j] = chi2p1(t, 749);
    }

    pvs[0]=pcombo(pv, 15);

    extreme=999.0; exi= -1; exj= -1; exd='?';
    for (j=0; j<15; j++)
    {
	int64_t tot=pos;
	for (i=1; i<=750; i++)
	{
	    int64_t k=dc[j][i];

	    x=sumbino(tot, k, 1.0/256.0);
	    if (x<extreme)
	    {
		extreme=x; exj=j; exi=i;
		if (k*256>tot) exd='+'; else exd='-';
	    }
	    tot-=k;
	}
    }

    printf("extreme = %.3g  (%d %d %c)\n", extreme, exj, exi, exd);
    pvs[1] = pco_scale(extreme, 16*750);

    printf("pvals (%.3g %.3g)\n", pvs[0], pvs[1]);

    e=pcombo(pvs, 2);
    printf("\nOne sided P value (very small numbers are bad)\n");
    printf("P = %.3g\n", e);
}

static int
dobuf(const unsigned char *buf, int n, int prev)
{
    int i, j, ch, sr;

    for (i=0; i<256; i++)
    {
	j=pos32[i]-n;
	if (j< -20000) j= -20000;
	pos32[i]=j;
    }

    i= -n; buf+=n;
    do
    {
	ch=buf[i];
	sr=(prev<<8)|(ch<<16)|ch;

	sr>>=1; j=i-pos32[sr&0xff]; if (j>751) j=751; dc16[0][j]++;
	sr>>=1; j=i-pos32[sr&0xff]; if (j>751) j=751; dc16[1][j]++;
	sr>>=1; j=i-pos32[sr&0xff]; if (j>751) j=751; dc16[2][j]++;
	sr>>=1; j=i-pos32[sr&0xff]; if (j>751) j=751; dc16[3][j]++;
	sr>>=1; j=i-pos32[sr&0xff]; if (j>751) j=751; dc16[4][j]++;
	sr>>=1; j=i-pos32[sr&0xff]; if (j>751) j=751; dc16[5][j]++;
	sr>>=1; j=i-pos32[sr&0xff]; if (j>751) j=751; dc16[6][j]++;
	sr>>=1; j=i-pos32[sr&0xff]; if (j>751) j=751; dc16[7][j]++;
	sr>>=1; j=i-pos32[sr&0xff]; if (j>751) j=751; dc16[8][j]++;
	sr>>=1; j=i-pos32[sr&0xff]; if (j>751) j=751; dc16[9][j]++;
	sr>>=1; j=i-pos32[sr&0xff]; if (j>751) j=751; dc16[10][j]++;
	sr>>=1; j=i-pos32[sr&0xff]; if (j>751) j=751; dc16[11][j]++;
	sr>>=1; j=i-pos32[sr&0xff]; if (j>751) j=751; dc16[12][j]++;
	sr>>=1; j=i-pos32[sr&0xff]; if (j>751) j=751; dc16[13][j]++;
	sr>>=1; j=i-pos32[sr&0xff]; if (j>751) j=751; dc16[14][j]++;

	pos32[prev]=i;
	prev=ch;
	i++;
    }
    while (i<0);

    return prev;
}

static void
desat(long satu)
{
    long t;
    int i, j;

    for (i=0; i<15; i++)
    {
	t=0;
	for (j=0; j<752; j++)
	{
	    t+=dc16[i][j];
	    dc[i][j]+=dc16[i][j];
	    dc16[i][j]=0;
	}
	if (t!=satu)
	{
	    seterr(1);
	    fprintf(stderr,
"Overflow in counters. This is seriously non-random. Quitting\n");
	    exit(errorlevel);
	}
    }
}

static void
readstuff(int64_t max)
{
#define MYBUFSIZ 4096
    unsigned char buf[MYBUFSIZ];
    int64_t p=0;
    long satu=0;
    int i, prev;

    memset(dc, 0, sizeof(dc));
    memset(dc16, 0, sizeof(dc16));
    for (i=0; i<256; i++) pos32[i]= -275;

    prev=0;

    while (max<=0 || p<max)
    {
	if (satu>1000000) {desat(satu); satu=0;}
	i=fread(buf, 1, MYBUFSIZ, stdin);
	if (i<=0) break;
	if (max>0 && i>max-p) i=max-p;
	prev=dobuf(buf, i, prev);
	satu+=i;
	p+=i;
    }

    if (max>0 && p<max)
    {
	fprintf(stderr, "Warning, expected %.0f samples, saw only %.0f\n",
		(double)max, (double)p);
	seterr(1);
    }

    if (p<50000)
    {
	fprintf(stderr, "Warning, < 50000 samples, don't trust result.\n");
	seterr(1);
    }

    if (satu) desat(satu);
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

    printf("expected range [ 633 662 699 799 841 875 ]\n");

    readstuff(max);

    return errorlevel;
}
