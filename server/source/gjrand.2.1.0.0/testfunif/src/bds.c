/* Test software for gjrand random numbers version 1.3.3.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* Inspired by reading the description of Marsaglia's Birthday Seperation */
/* test (in DIEHARD). This is probably not quite the same, but close. */
/* So far it catches straw/lfib3 and straw/lfib3l . */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../../src/my_int_types.h"
#include "../../testcommon/pcombo.h"
#include "../../testcommon/chi2p.h"

static int errorlevel=0;
static void seterr(int e) {if (errorlevel<e) errorlevel=e;}

#ifdef __cplusplus
#define NOARGS
#else
#define NOARGS void
#endif

static void
crash(char *s)
{
    fprintf(stderr, "crash [%s]\n", s); exit(1);
}

/* the extra is for sentinals for sorting. */

#define SIZ 256
#define PAD 125
double ibuf[SIZ];
double buf_padded[PAD+SIZ];
#define buf (buf_padded+PAD)

static void
sortn(int n)
{
    double x, y;
    int i, j;

    for (i=n; i<SIZ; i++)
    {
	x=buf[i];
	for (j=i-n; (y=buf[j])>x; j-=n) buf[j+n]=y;
	buf[j+n]=x;
    }
}

static void
sort(NOARGS)
{
    sortn(119); sortn(41); sortn(13); sortn(4); sortn(1);
}

static void
mkdif(NOARGS)
{
    double x=buf[0]+1.0;
    int i;

    for (i=0; i<SIZ-1; i++) buf[i]=exp((buf[i]-buf[i+1])*SIZ);

    buf[SIZ-1]=exp((buf[SIZ-1]-x)*SIZ);
}

#define COUNT 512

static void
mkchi(int64_t h[COUNT])
{
    int i;

    for (i=0; i<SIZ; i++) h[(int)floor(buf[i]*COUNT)]++;
}

static int64_t his1[COUNT], his2[COUNT], his3[COUNT];
static int64_t hisa[COUNT], hisb[COUNT], hisc[COUNT];

static void
init(NOARGS)
{
    int i;

    for (i=0; i<PAD; i++) buf_padded[i] = -1.0e30;
    memset(his1, 0, sizeof(his1));
    memset(his2, 0, sizeof(his2));
    memset(his3, 0, sizeof(his3));
    memset(hisa, 0, sizeof(hisa));
    memset(hisb, 0, sizeof(hisb));
    memset(hisc, 0, sizeof(hisc));
}

static void
dostat(NOARGS)
{
    double x;
    int i;

    memcpy(buf, ibuf, sizeof(ibuf));
    sort(); mkdif(); mkchi(his1);
    sort(); mkdif(); mkchi(his2);
    sort(); mkdif(); mkchi(his3);

    /* This is supposed to make a second data set different and possibly */
    /* better than the raw one. Later, we compare the two. */
    for (i=0; i<47; i++)
    {
	x=buf[i]-ibuf[i];
	if (x<0.0) x+=1.0;
	x-=ibuf[i+SIZ-47]*(1.0/16.0);
	if (x<0.0) x+=1.0;
	buf[i]=x;
    }
    for (i=47; i<SIZ; i++)
    {
	x=buf[i]-ibuf[i];
	if (x<0.0) x+=1.0;
	x-=ibuf[i-47]*(1.0/16.0);
	if (x<0.0) x+=1.0;
	buf[i]=x;
    }
    sort(); mkdif(); mkchi(hisa);
    sort(); mkdif(); mkchi(hisb);
    sort(); mkdif(); mkchi(hisc);
}

static int64_t
dofile(int64_t max)
{
    int64_t r=0;

    while (max<0 || r<max)
    {
	if (fread(ibuf, sizeof(double), SIZ, stdin)!=SIZ) break;
	dostat();
	r++;
    }

    if (max>0 && r<max)
    {
	fprintf(stderr,"warning expected %.0f blocks, saw only %.0f\n",
		(double)max, (double)r );
	seterr(1);
    }

    return r;
}

/* The expectation is not exactly for equal counts in all buckets */
/* though it probably is to within a percent or so. I can't be bothered */
/* calculating exact expectations, so instead compare two measured values. */
static double
anchi(int64_t h[COUNT], int64_t h2[COUNT], double x)
{
    double r=0.0, y;
    int i;

    for (i=0; i<COUNT; i++)
    {
	y=h[i]-h2[i];
	r+=y*y;
    }

    r/=x;
    r*=0.5;
    printf("chis = %10.5f\n", r);

    return chi2p1(r, COUNT-1);
}

static void
doan(int64_t c)
{
    double pvals[3];
    double x = ((double)c) * (SIZ/(double)COUNT);

    if (x<10)
    {
	fprintf(stderr,
"Warning: expected counts %.2f, should be at least 10. Don't trust result.\n",
		x);
	seterr(1);
    }

    printf("expected range [ 417 439 470 553 589 616 ]\n");

    pvals[0]=anchi(his1, hisa, x);
    pvals[1]=anchi(his2, hisb, x);
    pvals[2]=anchi(his3, hisc, x);
printf("pvals = %.6g %.6g %.6g\n", pvals[0], pvals[1], pvals[2]);

    printf("\nOne sided P value (very small numbers are bad)\n");
    printf("P = %.3g\n", pcombo(pvals, 3));
}

int
main(int argc, char **argv)
{
    double dn;
    int64_t n, r;

    if (argc<2) n= -1;
    else
    {
	if (sscanf(argv[1], "%lf", &dn)!=1)
	    crash("optional arg (word-count) must be numeric");
	n=(int64_t)(dn/SIZ);
    }

    init();

    r=dofile(n);

    doan(r);

    return errorlevel;
}
