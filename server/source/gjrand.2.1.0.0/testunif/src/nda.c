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

/* Nearest distance analysis. */
/* Like Kendall and Babington Smith Gap Test but bigger and less elegant. */
/* Culminates in a huge Chi-Squared test. */
/* This is Geronimo's own invention (dating from February 2005), though */
/* someone else may have done it before. Please mention in gjrand forums on */
/* SourceForge if this already has a name. */

/* Now uses 64 bit counters so should be able to do huge files, but slowly. */
/* Random bytes are read from standard input. There is an optional */
/* size arg (now in bytes), otherwise it just reads until end of file. */

static int errorlevel=0;
static void seterr(int e) {if (errorlevel<e) errorlevel=e;}

static int64_t count[16][16][50];
static int32_t c32[16][16][50];
static int32_t last[16];

static void
init(void)
{
    int i;
    memset(count, 0, sizeof(count));
    memset(c32, 0, sizeof(c32));
    for (i=0; i<16; i++) last[i]= -11;
}

static void
dobyte(const int b, int pos)
{
    int i;
    int n;
    int32_t *c;

    n=b&15;
    c= &(c32[n][0][0]);

    for (i= -16; i<0; i++)
    {
	int d=pos-(last+16)[i];
	if (d>49) d=49;
	c[d]++;
	c+=50;
    }

    last[n]=pos;

    pos++;
    n=(b>>4)&15;
    c= &(c32[n][0][0]);

    for (i= -16; i<0; i++)
    {
	int d=pos-(last+16)[i];
	if (d>49) d=49;
	c[d]++;
	c+=50;
    }

    last[n]=pos;
}

static void
dobuf(const unsigned char *buf, int pos, int i)
{
    int j;

    buf+=i;

    for (j= -i; j<0; j++)
    {
	dobyte(buf[j], pos);
	pos+=2;
    }
}

static double expect[50];

static void
mkexpect(int64_t size)
{
    int i;
    double p=size/16.0, x;

    for (i=1; i<=48; i++)
    {
	x=p*(1.0/16.0);
	expect[i]=x;
	p-=x;
    }
    expect[49]=p;
}

/* This monster we have created contains too much statistics for the */
/* human mind to comprehend, so we summarise it all in one chi-squared */
/* number with enormous degrees of freedom. */
static void
doan(void)
{
    double p[2];
    double t=0.0, extreme;
    double x;
    int exi, exj, exk, exd='?';
    int i, j, k;

    for (i=0; i<16; i++) for (j=0; j<16; j++) for (k=1; k<=49; k++)
    {
	double x=(double)(count[i][j][k]);
	double e=expect[k];
	x-=e;
	t+=x*x/e;
    }

    printf("%15.5f\n", (double)t);

    x = (t - 12761) * ((82.001-60.332)/(13379-12761)) + 60.332;
    if (x<0.0) x = 0.0;
    p[0] = chi2p1(x, 52);

    t=0.0; extreme= 999.0; exi=exj=exk= -1;
    for (i=0; i<16; i++) for (j=0; j<16; j++)
    {
	uint64_t tot=0;
	for (k=49; k>0; k--) tot+=count[i][j][k];
	for (k=1; k<=48; k++)
	{
	    uint64_t x=count[i][j][k];
	    double y=sumbino(tot, x, 1.0/16.0);

	    if (y<extreme)
	    {
		extreme=y; exi=i; exj=j; exk=k;
		if (16*x>tot) exd='+'; else exd='-';
	    }
	    tot-=x;
	}
    }

    printf("extreme = %.3g   (%d %d %d %c)\n", extreme, exi, exj, exk, exd);
    p[1]=pco_scale(extreme, 16*16*48);
    printf("pvals (%.3g %.3g)\n\n", p[0], p[1]);

    printf("One sided P value (very small numbers are bad)\n");
    printf("P = %.3g\n", pcombo(p, 2));
}

static void
adjust(int pos)
{
    int i, j, k;

    for (i=0; i<16; i++)
    {
	j=last[i]-pos;
	if (j< -9000000) j= -9000000;
	last[i]=j;
    }

    for (i=0; i<16; i++) for (j=0; j<16; j++) for (k=1; k<=49; k++)
    {
	count[i][j][k]+=c32[i][j][k];
	c32[i][j][k]=0;
    }
}

static void
readstuff(int64_t max)
{
    unsigned char buf[4096];
    int64_t posll=0;
    int pos=0;
    int i;

    while (max<0 || posll<max)
    {
	i=fread(buf, 1, 4096, stdin);
	if (i<=0) break;
	i+=i;
	if (max>0 && i>max-posll) i=max-posll;
	dobuf(buf, pos, i>>1);
	posll+=i; pos+=i;
	if (pos>=4000000) {adjust(pos); pos=0;}
    }

    adjust(pos);

    if (max>0 && posll<max)
    {
	fprintf(stderr, "Warning, expected %.0f 4bit samples, saw only %.0f\n",
		(double)max, (double)posll);
	seterr(1);
    }

    if (posll<100000)
    {
	fprintf(stderr, "Warning, < 100000 samples, don't trust result.\n");
	seterr(1);
    }

    mkexpect(posll);
    doan();
}

int
main(int argc, char **argv)
{
    double dmax;
    int64_t max;

    if (argc<2 || sscanf(argv[1], "%lf", &dmax)!=1 || dmax<1.0) max= -1;
    else max=(int64_t)(2.0*dmax);

    /* although we do a chi-square test, when tested on several random */
    /* generators i trust a lot, the results do not follow the chi-square */
    /* distribution. The mean is about right near 12520, but the standard */
    /* deviation is near 285 compared to about 150 for the chi-square */
    /* distribution. It may be that this is because the counts we are */
    /* studying correlate with each other in different ways than is usually */
    /* the case with the chi-square test. These expected values are obtained */
    /* emperically and are only approximate. Theoretical analysis is invited. */

    init();
    printf("expected range [ 11770 11940 12170 12890 13270 13570 ]\n");

    readstuff(max);

    return errorlevel;
}
