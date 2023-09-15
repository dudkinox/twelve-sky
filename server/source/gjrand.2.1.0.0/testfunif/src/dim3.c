/* Test software for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* This one looks for bad things in 3 dimensions. */

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

#define DIM 3
#define DIMSIZE (1<<(6*DIM))
#define DIMMASK (DIMSIZE-1)

/* [0..2]=tots [3..5]=sqrs [6]=count */
static double stats[DIMSIZE][7];

void
init(void)
{
    int i;

    for (i=0; i<DIMSIZE; i++)
    {
	stats[i][0]=0.0;
	stats[i][1]=0.0;
	stats[i][2]=0.0;
	stats[i][3]=0.0;
	stats[i][4]=0.0;
	stats[i][5]=0.0;
	stats[i][6]=0.0;
    }
}

void
dobuf(double fpart[], unsigned char ipart[], int s)
{
    double x;
    int i, index=0;

    for (i= -DIM; i<0; i++) index=ipart[i]+64*index;

    for (i=0; i<s; i++)
    {
	index = (ipart[i]+64*index) & DIMMASK;
	stats[index][6]++;
	x=fpart[i];
	stats[index][0] += x;
	stats[index][3] += x*x;
	x=fpart[i-1];
	stats[index][1] += x;
	stats[index][4] += x*x;
	x=fpart[i-2];
	stats[index][2] += x;
	stats[index][5] += x*x;
    }
}

void
dosplit(double b[], int s, unsigned char ipart[], double fpart[])
{
    double x, f;
    int i;

    for (i=0; i<s; i++)
    {
	x=b[i]*64.0;
	f=floor(x);
	ipart[i]=(unsigned char)f;
	fpart[i]=x-f-0.5;
    }
}

int64_t
dostuff(int64_t max)
{
    double buf[DIM+512], fpart[DIM+512];
    unsigned char ipart[DIM+512];
    int64_t r=0;
    int i, j;

    if (fread(buf, sizeof(double), 512, stdin)!=512) crash("fread failed 1");
    dosplit(buf, 512, ipart, fpart);
    dobuf(fpart+DIM, ipart+DIM, 512-DIM);

    j=512-DIM;
    r=j;

    while (max<0 || r<max)
    {
	for (i=0; i<DIM; i++) {ipart[i]=ipart[j+i]; fpart[i]=fpart[j+i];}
	j=512;
	if (max>=0 && j>max-r) j=max-r;
	j=fread(buf+DIM, sizeof(double), j, stdin);
	if (j<=0) break;
	dosplit(buf+DIM, j, ipart+DIM, fpart+DIM);
	dobuf(fpart+DIM, ipart+DIM, j);
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
    double pval[7];
    double x, t, e;
    int i, j;

    if (s<2600000)
    {
	fprintf(stderr, "warning < 2600000 samples, stats may be wonky.\n");
	seterr(1);
    }

    printf("expected range [ -3.09 -2.33 -1.28 1.28 2.33 3.09 ]\n");

    e=((double)s)/((double)DIMSIZE);
    t=0.0;

    for (i=0; i<DIMSIZE; i++)
    {
	x=stats[i][6]-e;
	t+=x*x;
    }
    t/=e;

    t-=DIMMASK;
    t/=sqrt(2.0*DIMMASK);
    /* Because of covariance, the simple chi-square calculation here */
    /* should be wrong. Similations suggests it's not wrong by much */
    /* ( less than 4% ? ) so i'm just going to use it as is. */

    printf("counts  = %10.5f sigma\n", t);
    pval[6]=erfc(fabs(M_SQRT1_2*t));

    for (j=0; j<DIM; j++)
    {
	t=0.0;
	for (i=0; i<DIMSIZE; i++)
	{
	    e=stats[i][6];
	    if (e==0.0) x=1.0;
	    else
	    {
		x=stats[i][j];
		x=x*x*12.0/e;
	    }
	    t+=x;
	}

	t-=DIMMASK;
	t/=sqrt(2.0*DIMMASK);

	printf("sums %d  = %10.5f sigma\n", j, t);
	pval[j]=erfc(fabs(M_SQRT1_2*t));
    }

    for (j=0; j<DIM; j++)
    {
	t=0.0;
	for (i=0; i<DIMSIZE; i++)
	{
	    e=stats[i][6];
	    if (e==0.0) x=1.0;
	    else
	    {
		x=stats[i][DIM+j]-e*(1.0/12.0);
		x=x*x*180.0/e;
	    }
	    t+=x;
	}

	t-=DIMMASK;
	t/=sqrt(2.0*DIMMASK);

	printf("squares %d  = %10.5f sigma\n", j, t);
	pval[DIM+j]=erfc(fabs(M_SQRT1_2*t));
    }

printf("pval ");
for (j=0; j<7; j++) printf("%.5g ",pval[j]);
putchar('\n');

    printf("\nOne sided P value (very small numbers are bad)\n");
    printf("P = %.3g\n", pcombo(pval, 7));
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
	count=(int64_t)dc-DIM;
    }
    else count= -1;

    init();
    r=dostuff(count);
    doan(r);

    return errorlevel;
}
