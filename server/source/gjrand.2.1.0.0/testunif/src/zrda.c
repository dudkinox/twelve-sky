/* Test software for gjrand random numbers version 1.3.3.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

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

static int64_t count[6][256];
static int32_t last[6];
static double prob[6];
static const char counts[6] = {9, 13, 15, 17, 19, 23};
static const char types[6] = {0, 1, 1, 1, 1, 0};
static const int dfs[2] = {200, 16};

static void
newline(const double from[], double to[], int fromcount)
{
    int j;

    to[0] = from[0]*0.5;
    for (j=1; j<=fromcount; j++) to[j] = (from[j-1]+from[j])*0.5;
    to[fromcount+1] = from[fromcount]*0.5;
}

static void
pasc(double *pr, int order)
{
    double t[100];
    int j;

    pr[0] = 1.0;

    for (j=0; j<order; j++)
    {
	newline(pr, t, j);
	j++; if (j>=order) break;
	newline(t, pr, j);
    }
}

static void
init(void)
{
    double pr[65];
    double cu[32];
    double acc;
    int j;

    memset(count, 0, sizeof(count));

    pasc(pr, 32);
    acc = 0.0;
    for (j=0; j<16; j++) {acc += pr[j]; cu[j] = acc;}
    for (j=0; j<3; j++) prob[5-j] = prob[j] = cu[(int)(counts[j])];

    /* This is very approximate. But for decently large files */
    /* it has very little effect anyway. */
    for (j=0; j<6; j++) last[j] = 1024-((int)(1.0/prob[j]))-1;
}

static void
dogap(int b, int32_t pos)
{
    int g;

    if (b<=15)
    {
	g = pos-last[2];
	if (g>255) g = 0;
	count[2][g]++;
	last[2] = pos;
	if (b<=13)
	{
	    g = pos-last[1];
	    if (g>255) g = 0;
	    count[1][g]++;
	    last[1] = pos;
	    if (b<=9)
	    {
		g = pos-last[0];
		if (g>255) g = 0;
		count[0][g]++;
		last[0] = pos;
	    }
	}
    }
    else if (b>=17)
    {
	g = pos-last[3];
	if (g>255) g = 0;
	count[3][g]++;
	last[3] = pos;
	if (b>=19)
	{
	    g = pos-last[4];
	    if (g>255) g = 0;
	    count[4][g]++;
	    last[4] = pos;
	    if (b>=23)
	    {
		g = pos-last[5];
		if (g>255) g = 0;
		count[5][g]++;
		last[5] = pos;
	    }
	}
    }
}

static void
doword(uint32_t w, int32_t pos)
{
    w = ((w>>1)&0x55555555) + (w&0x55555555);
    w = ((w>>2)&0x33333333) + (w&0x33333333);
    w = ((w>>4)&0x0f0f0f0f) + (w&0x0f0f0f0f);
    w = ((w>>8)&0x00ff00ff) + (w&0x00ff00ff);
    w = ((w>>16)&0x0000ffff) + (w&0x0000ffff);

    dogap(w, pos);
}

static void
dobuf(const uint32_t *buf, int i)
{
    int j;

    for (j=0; j<i; j++) doword(buf[j], j);
}

static int
mkexpect(double p, int df, double expect[256])
{
    double x=1.0, t;
    int j;

    for (j=1; j<=df; j++)
    {
	t = x*p;
	expect[j] = t;
	x -= t;
    }

    expect[0] = x;

    return j;
}

static void
doan(int offset, double pv[2])
{
    double expect[256];
    double chis, x, t, pval, extr = 999.9;
    int64_t *ct = &(count[offset][0]), l, tot;
    int type = types[offset];
    int df = dfs[type];
    int j, exi = -1, exd = '?';

    mkexpect(prob[offset], df, expect);

    for (j=df+1; j<256; j++) ct[0] += ct[j];
    l = 0;
    for (j=0; j<=df; j++) l += ct[j];
    t = l;
    for (j=0; j<=df; j++) expect[j] *= t;

    chis = 0.0;

    for (j=0; j<=df; j++)
    {
	x = expect[j];
	t = ct[j]-x;
	chis += t*t/x;
    }

    pval = chi2p1(chis, df);
    pv[0] = pval;

    tot = l;
    for (j=1; j<=df; j++)
    {
	int64_t k = ct[j];
	x = sumbino(tot, k, prob[offset]);
	if (x<extr)
	{
	    extr = x; exi = j;
	    if (k>tot*prob[offset]) exd = '+'; else exd = '-';
	}
	tot -= k;
    }

    extr = pco_scale(extr, df);

    pv[1] = extr;

    printf("%2d %8.5f %3d %12.3f %12.6g : (%3d %c) %12.6g\n",
	counts[offset], prob[offset], df, chis, pval, exi, exd, extr);
}

static void
agelast(void)
{
    int j, k;

    for (j=0; j<8; j++)
    {
	k = last[j]-1024;
	if (k < -20000) k = -20000;
	last[j] = k;
    }
}

static void
readstuff(int64_t max)
{
    uint32_t buf[1024];
    double pvals[8], p;
    int64_t pos=0;
    int i;

    while (max<0 || pos<max)
    {
	i=fread(buf, 4, 1024, stdin);
	if (i<=0) break;
	if (max>0 && i>max-pos) i=max-pos;
	agelast();
	dobuf(buf, i);
	pos+=i;
	if (i<1024) break;
    }

    if (max>0 && pos<max)
    {
	fprintf(stderr, "Warning, expected %.0f bytes, saw only %.0f\n",
		(double)max, (double)pos);
	seterr(1);
    }

    if (4*pos<1900000)
    {
	fprintf(stderr, "Warning, < 1900000 bytes, don't trust result.\n");
	seterr(1);
    }

    for (i=0; i<6; i++) doan(i, pvals+2*i);

    p = pcombo(pvals, 12);
    printf("\nOne sided P value (very small numbers are bad)\n");
    printf("P = %.3g\n", p);
}

int
main(int argc, char **argv)
{
    double dmax;
    int64_t max;

    if (argc<2 || sscanf(argv[1], "%lf", &dmax)!=1 || dmax<1.0) max= -1;
    else max=(int64_t)(0.25*dmax);

    init();

    readstuff(max);

    return errorlevel;
}
