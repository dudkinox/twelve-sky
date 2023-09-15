/* Test software for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* "Kendall and Babington-Smith Gap Test" on 4 bit parts of 32 bit words. */
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

static int64_t dc[12][51];
static int pos32[12][16];

static void
mkexpect(double expect[51], int64_t pos)
{
    double c=(double)pos, e=c/16.0;
    int i;

    for (i=1; i<50; i++)
    {
	expect[i]=e;
	c-=e;
	e *= 15.0/16.0;
    }

    expect[50]=c;
}

static void
doan(int64_t pos)
{
    static const int shifts[12]={28, 26, 24,  20, 18, 16,  12, 10, 8,  4, 2, 0};
    double expect[51];
    double p[12], extreme;
    int i, j, exi, exj, exd;

    mkexpect(expect, pos);

    for (j=0; j<12; j++)
    {
	int64_t *d= &(dc[j][0]);
	double e, x, t=0.0;
	for (i=1; i<51; i++)
	{
	    e=expect[i];
	    x=(double)d[i]-e;
	    t+=x*x/e;
	}
	printf("%2d : %15.5f\n", shifts[j], t);

	p[j] = chi2p1(t, 49);
    }

    p[0]=pcombo(p, 12);

    extreme=999.0; exi= -1; exj= -1; exd='?';
    for (j=0; j<12; j++)
    {
	int64_t tot=pos;
	for (i=1; i<50; i++)
	{
	    int64_t k=dc[j][i];
	    double x=sumbino(tot, k, 1.0/16.0);
	    if (x<extreme)
	    {
		extreme=x; exi=i; exj=j;
		if (k*16>tot) exd='+'; else exd='-';
	    }
	    tot-=k;
	}
    }
    printf("extreme = %.3g (%d %d %c)\n", extreme, shifts[exj], exi, exd);
    p[1]=pco_scale(extreme, 12*49);

    printf("pvals (%.3g %.3g)\n\n", p[0], p[1]);

    printf("One sided P value (very small numbers are bad)\n");
    printf("P = %.3g\n", pcombo(p, 2));
}

static void
dobuf(const uint32_t * const buf, int n)
{
    int i, j, k;
    const uint32_t * const bend=buf+n;

    for (k=0; k<12; k++)
    {
	for (i=0; i<16; i++)
	{
	    j=pos32[k][i]-n;
	    if (j< -20000) j= -20000;
	    pos32[k][i]=j;
	}
    }

    i= -n;
    do
    {
	uint32_t ch=bend[i];

#define DOPART(k,sh) \
	{ \
	    int part=(ch>>sh)&15; \
	    j=i-pos32[k][part]; \
	    if (j>50) j=50; \
	    pos32[k][part]=i; \
	    dc[k][j]++; \
	}

	DOPART(0, 28);
	DOPART(1, 26);
	DOPART(2, 24);
	DOPART(3, 20);
	DOPART(4, 18);
	DOPART(5, 16);
	DOPART(6, 12);
	DOPART(7, 10);
	DOPART(8, 8);
	DOPART(9, 4);
	DOPART(10, 2);
	DOPART(11, 0);

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
    for (i=0; i<12; i++)
    {
	int j;
	for (j=0; j<16; j++) pos32[i][j]= -18;
    }

    while (max<0 || p<max)
    {
	i=fread(buf, sizeof(uint32_t), MYBUFSIZ, stdin);
	if (i<=0) break;
	if (max>0 && i>max-p) i=max-p;
	dobuf(buf, i);
	p+=i;
    }

    if (max>0 && p<max)
    {
	fprintf(stderr,
		"Warning, expected %.0f 32 bit samples, saw only %.0f\n",
		(double)max, (double)p);
	seterr(1);
    }

    if (p<1000)
    {
	fprintf(stderr, "Warning, < 1000 samples, don't trust result.\n");
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

    if (argc<2 || sscanf(argv[1], "%lf", &dmax)!=1 || dmax<0.0) max= -1;
    else max=(int64_t)(0.25*dmax);

    /* approximate, determined emperically. */
    printf("expected range [ 23.8 28.7 36.6 62.0 75.0 85.9 ]\n");

    readstuff(max);

    return errorlevel;
}
