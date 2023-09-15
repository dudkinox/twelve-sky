/* Test software for gjrand random numbers version 2.1.0.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../../src/my_int_types.h"
#include "../../testcommon/pcombo.h"
#include "../../testcommon/chi2p.h"

/* chi square test on byte values. */
/* do a seperate chi square test for each 1K input */
/* summarise with mean and standard deviation. */

static int errorlevel=0;
static void seterr(int e) {if (errorlevel<e) errorlevel=e;}

#define BSIZ 1024

static uint64_t longcount[256];
static unsigned short count[256];

static double
chisquare(void)
{
    int i, t=0, x;

    for (i=255; i>=0; i--)
    {
	x=count[i]-(BSIZ/256);
	t+=x*x;
    }

    return t*(256.0/BSIZ);
}

static void
docount(const unsigned char *buf)
{
    const unsigned char *b=buf-1;
    const unsigned char * const bend=b+BSIZ-1;

    do
    {
	b+=4;
	count[b[-3]]++;
	count[b[-2]]++;
	count[b[-1]]++;
	count[b[0]]++;
    } while (b<bend);
}

static double
longchi(int64_t pos)
{
    double expect=pos*4, x, r=0.0;
    int i;

    for (i=0; i<256; i++)
    {
	x=((double)(longcount[i]))-expect;
	r+=x*x/expect;
    }

    return r;
}

static int64_t freq[11];
static double fexpect[11];

static void
dofreq(void)
{
    int i;
    unsigned j, k;

    for (i=254; i>=0; i-=2)
    {
	j=count[i]; k=count[1+i];
	longcount[i]+=j; longcount[1+i]+=k;
	if (j>=11) j=10; if (k>=11) k=10;
	freq[j]++; freq[k]++;
    }
}

static void
mkfexpect(int64_t pos)
{
    double e1[11], e2[11], *pe1=e1, *pe2=e2, *pt;
    int i, j;

    pe1[0]=256.0*pos; for (i=1; i<11; i++) pe1[i]=0.0;

    for (i=0; i<BSIZ; i++)
    {
	pe2[0]=(255.0/256.0)*pe1[0];
	for (j=1; j<10; j++)
	    pe2[j] = (1.0/256.0)*pe1[j-1] + (255.0/256.0)*pe1[j];
	pe2[10] = (1.0/256.0)*pe1[9] + pe1[10];

	pt=pe1; pe1=pe2; pe2=pt;
    }

    for (i=0; i<11; i++) fexpect[i]=pe1[i];
}

static double
freqchi(int64_t pos)
{
    int i;
    double r=0.0;

    mkfexpect(pos);

    for (i=0; i<11; i++)
    {
	double e=fexpect[i], d=(double)freq[i]-e;

	r+=d*d/e;
    }

    return (double)r;
}

static const int nidx[11]=
	{0, 907, 945, 971, 995, 1019, 1041, 1067, 1095, 1139, 3000};

/* Approx the last two digits of these constants are suspect. */
/* I don't think this matters much for any practical sized run. */
static const double nexp[10]=
{
	0.10119820276043326, 0.10426628520961570,
	0.09597136330351251, 0.10111064685410051,
	0.10619313126611958, 0.09540112483011044,
	0.10311528203375719, 0.09301983939315027,
	0.10261308514289882, 0.09711103920630204
};

static const int xidx[12]=
	{0, 741, 767, 791, 823, 859,
	1197, 1243, 1287, 1325, 1363, 3000};

static const double xexp[11]=
{
	3.02723026109275246e-04,
	7.66166119642529807e-04, 1.94014157470135708e-03,
	6.99937555010270163e-03, 2.09815171706516437e-02,
	9.38850490318845710e-01,
	2.00759142116959319e-02, 6.97949538770974414e-03,
	2.08461924367735283e-03, 7.11370018216585047e-04,
	3.08187378647143827e-04
};


static double
metachi(int64_t chis[3000], int64_t tot,
	const int idx[], const double exp[], int buckets)
{
    int i, j, k;
    int64_t t;
    double ch=0.0, x, y;

    for (i=0; i<buckets; i++)
    {
	t=0;
	k=idx[i+1];
	for (j=idx[i]; j<k; j++) t+=chis[j];

	x=exp[i]*tot;
	y=t-x;
	ch+=y*y/x;
    }

    return (double)ch;
}

/* 2 to 1 sided */
static double top2(double x, double m, double sd)
{
    x-=m;
    x/=sd;
    return erfc(fabs(M_SQRT1_2*x));
}

#define MEAN (255.0)
#define VAR (509.501953125000)
#define SD_MEAN (sqrt(VAR))
#define SD_VAR (733.16553473107774153)

static void
readstuff(int64_t max)
{
    double p[6];
    unsigned char buf[BSIZ];
    int64_t chis[3005];
    double ch, s=0.0, var=0.0;
    double wid, t;
    int64_t pos=0;
    int i, tests=5;

    memset(freq, 0, sizeof(freq));
    memset(chis, 0, sizeof(chis));
    memset(longcount, 0, sizeof(longcount));

    while (1)
    {
	i=fread(buf, 1, BSIZ, stdin);
	if (i<BSIZ) break;
	pos++;
	memset(count, 0, sizeof(count));
	docount(buf);
	dofreq();
	ch=chisquare();
	i=(int)(floor(ch*4.0+0.5));
	if (i>=3000) i=2999;
	chis[i]++;
	ch-=MEAN;
	s+=ch; var+=ch*ch-VAR;

	if (max>0 && pos>=max) break;
    }

    if (max>0 && pos<max)
    {
	fprintf(stderr, "Warning, expected %.0f 1kB blocks, saw only %.0f\n",
		(double)max, (double)pos);
	seterr(1);
    }

    s/=pos;
    var/=pos;

    /* Accuracy of wid isn't critical. */
    wid = SD_MEAN / sqrt((double)pos);
    printf("expect [ %.7f %.7f %.7f %.7f %.7f %.7f ]\n",
	-3.0902290344*wid, -2.3263473511*wid, -1.2815513611*wid,
	1.2815513611*wid, 2.3263473511*wid, 3.0902290344*wid);
    printf("mean = %16.10f\n\n", (double)s);
    p[0]=top2((double)s, 0.0, wid);

    wid = SD_VAR / sqrt((double)pos);
    printf("expect [ %.6f %.6f %.6f %.6f %.6f %.6f ]\n",
	-3.0902290344*wid, -2.3263473511*wid, -1.2815513611*wid,
	1.2815513611*wid, 2.3263473511*wid, 3.0902290344*wid);
    printf("var  = %16.10f\n\n", (double)var);
    p[1]=top2((double)var, 0.0, wid);

    t=longchi(pos);
    printf("expect [ 191 205 227 284 310 331 ]\n");
    printf("longchi = %16.10f\n\n", t);

    p[2] = chi2p2(t, 255);

    /* two dodgy normal approximations on the high side. */
    t=freqchi(pos);
    printf("expect [ 1.48 2.5 4.8 16.0 23.2 29.6 ]\n");
    printf("freqchi = %16.10f\n\n", t);
    p[3] = chi2p1(t, 10);

    if (pos<100) printf("warning < 100 blocks, don't trust metachi\n");
    t=metachi(chis, pos, nidx, nexp, 10);
    printf("expect [ 1.15 2.09 4.16 14.7 21.7 27.9 ]\n");
    printf("metachi = %14.10f\n\n", t);
    p[4] = chi2p1(t, 9);

    if (pos>=990)
    {
	t=metachi(chis, pos, xidx, xexp, 11);
	printf("expect [ 1.48 2.5 4.8 16.0 23.2 29.6 ]\n");
	printf("xtrmeta = %14.10f\n\n", t);
	p[5] = chi2p1(t, 10);
	tests++;
    }

    for (i=0; i<tests; i++) printf("%.6g ", p[i]);

    t=pcombo(p, tests);
    printf("\n\nOne sided P value (very small numbers are bad)\n");
    printf("P = %.3g\n", t);
}

int
main(int argc, char **argv)
{
    double dmax;
    int64_t max;

    if (argc<2 || sscanf(argv[1], "%lf", &dmax)!=1 || dmax<1.0) max= -1;
    else max=(int64_t)(dmax*(1.0/BSIZ));
    if (max==0) max++;

    readstuff(max);

    return errorlevel;
}
