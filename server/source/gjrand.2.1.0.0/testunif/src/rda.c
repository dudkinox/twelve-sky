/* Test software for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
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

/* For each byte read, calculate the distance since the same byte */
/* value was last seen and collect a histogram of these distances. */
/* Finally do a chi-square test on the histogram. */

static int errorlevel=0;
static void seterr(int e) {if (errorlevel<e) errorlevel=e;}

static int64_t dc[752];
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
    double expect[752], pvs[2];
    double e, t=0.0, x, extreme;
    int64_t tot;
    int i, exi, exd;

    mkexpect(expect, pos);

    for (i=1; i<752; i++)
    {
	e=expect[i];
	x=(double)dc[i]-e;
	t+=x*x/e;
    }

    printf("%15.5f\n\n", t);

    /* This appears as if it might be a chi-square distribution but with */
    /* 748 or 749 degrees of freedom where 750 might be expected. */
    pvs[0] = chi2p1(t, 749);

    extreme=999.0; exi= -1; exd='?';
    tot=pos;
    for (i=1; i<751; i++)
    {
	int64_t k=dc[i];
	x=sumbino(tot, k, 1.0/256.0);
	if (x<extreme)
	{
	    extreme=x; exi=i;
	    if (k*256>tot) exd='+'; else exd='-';
	}
	tot-=k;
    }
    printf("extreme = %.3g (%d %c)\n", extreme, exi, exd);
    pvs[1]=pco_scale(extreme, 750);

    printf("pvals (%.3g %.3g)\n\n", pvs[0], pvs[1]);

    printf("One sided P value (very small numbers are bad)\n");
    printf("P = %.3g\n", pcombo(pvs, 2));
}

static void
dobuf(const unsigned char *buf, int n)
{
    int i, j, ch;
    const unsigned char * const bend=buf+n;

    for (i=0; i<256; i++)
    {
	j=pos32[i]-n;
	if (j< -20000) j= -20000;
	pos32[i]=j;
    }

    i= -n;
    do
    {
	ch=bend[i];
	j=i-pos32[ch];
	if (j>751) j=751;
	pos32[ch]=i;
	dc[j]++;
	i++;
    }
    while (i<0);
}

static void
readstuff(int64_t max)
{
#define MYBUFSIZ 8192
    unsigned char buf[MYBUFSIZ];
    int64_t p=0;
    int i;

    for (i=0; i<256; i++) pos32[i]= -275;
    for (i=0; i<752; i++) dc[i]=0;

    while (max<=0 || p<max)
    {
	i=fread(buf, 1, MYBUFSIZ, stdin);
	if (i<=0) break;
	if (max>0 && i>max-p) i=max-p;
	dobuf(buf, i);
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

    /* "expected range" is approximate, +- 4 */
    printf("expected range [ 633 662 699 799 841 875 ]\n");

    readstuff(max);

    return errorlevel;
}
