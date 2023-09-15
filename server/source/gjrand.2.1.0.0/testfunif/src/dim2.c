/* Test software for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* This one looks for trouble in 2 dimensions. */
/* It provides a second opinion after angle. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../../src/my_int_types.h"
#include "../../testcommon/pcombo.h"

static int errorlevel=0;
static void seterr(int e) {if (errorlevel<e) errorlevel=e;}

static void
crash(char *s)
{
    seterr(1);
    fprintf(stderr, "crash [%s]\n", s); exit(errorlevel);
}

#define BITS 20
#define BSIZE (1<<BITS)
#define BMASK (BSIZE-1)

/* [0]=count [1]=tot [2]=totsq */
static double stats[BSIZE][3];

void
init(void)
{
    int i;

    for (i=0; i<BSIZE; i++)
    {
	stats[i][0]=0.0;
	stats[i][1]=0.0;
	stats[i][2]=0.0;
    }
}

void
dobuf(double b[], int s)
{
    double y;
    int i, j;

    for (i=0; i<s; i++)
    {
	j=((int)(b[i-1]*BSIZE))&BMASK;
	y=b[i]-0.5;
	stats[j][0]++;
	stats[j][1] += y;
	stats[j][2] += y*y;
    }
}

int64_t
dostuff(int64_t max)
{
    double buf[1+512];
    int64_t r=0;
    int j;

    if (fread(buf, sizeof(double), 512, stdin)!=512) crash("fread failed 1");
    dobuf(buf+1, 512-1);
    j=512-1;

    buf[0]=buf[511];

    while (max<0 || r<max)
    {
	j=512;
	if (max>=0 && j>max-r) j=max-r;
	j=fread(buf+1, sizeof(double), j, stdin);
	if (j<=0) break;
	dobuf(buf+1, j);
	buf[0]=buf[j];
	r+=j;
    }

    if (max>=0 && r<max)
    {
	fprintf(stderr, "warning expected %.0f samples saw only %.0f\n",
		(double)max, (double)r);
	seterr(1);
    }

    return r;
}

void
doan(int64_t s)
{
    double pval[3];
    double x, t, e;
    int i;

    if (s<9000000)
    {
	fprintf(stderr, "warning < 9000000 samples, stats may be wonky.\n");
	seterr(1);
    }

    printf("expected range [ -3.09 -2.33 -1.28 1.28 2.33 3.09 ]\n");

    e=((double)s)/((double)BSIZE);
    t=0.0;

    for (i=0; i<BSIZE; i++)
    {
	x=stats[i][0]-e;
	t+=x*x;
    }
    t/=e;

    t-=BMASK;
    t/=sqrt(2.0*BMASK);

    printf("counts  = %10.5f sigma\n", t);
    pval[0]=erfc(fabs(M_SQRT1_2*t));

    t=0.0;
    for (i=0; i<BSIZE; i++)
    {
	e=stats[i][0];
	if (e==0.0) x=1.0;
	else
	{
	    x=stats[i][1];
	    x=x*x*12.0/e;
	}
	t+=x;
    }

    t-=BMASK;
    t/=sqrt(2.0*BMASK);

    printf("sums    = %10.5f sigma\n", t);
    pval[1]=erfc(fabs(M_SQRT1_2*t));

    t=0.0;
    for (i=0; i<BSIZE; i++)
    {
	e=stats[i][0];
	if (e==0.0) x=1.0;
	else
	{
	    x=stats[i][2]-e*(1.0/12.0);
	    x=x*x*180.0/e;
	}
	t+=x;
    }

    t-=BMASK;
    t/=sqrt(2.0*BMASK);

    printf("squares = %10.5f sigma\n", t);
    pval[2]=erfc(fabs(M_SQRT1_2*t));

printf("pvals %.6g %.6g %.6g\n", pval[0], pval[1], pval[2]);

    printf("\nOne sided P value (very small numbers are bad)\n");
    printf("P = %.3g\n", pcombo(pval, 3));
}

int
main(int argc, char ** argv)
{
    int64_t r, count;
    double dc;

    if (argc>=2)
    {
	if (sscanf(argv[1], "%lf", &dc)!=1 || dc<1.0)
	   crash("1 optional arg: point-count");
	count=(int64_t)dc-1;
    }
    else count= -1;

    init();
    r=dostuff(count);
    doan(r);

    return errorlevel;
}
