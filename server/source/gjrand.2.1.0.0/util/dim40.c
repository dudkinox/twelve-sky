/* Test software for gjrand random numbers version 1.3.1.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* This one looks for trouble in 41 dimensions. It is only likely to catch */
/* the grossest ugliness, but should be good enough for a lagged */
/* Fibonacci with both lags less than about 40. */

/* This one is to correctly calibrate the "counts". */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../src/my_int_types.h"
#include "../testcommon/pcombo.h"

#define SIZ (15<<20)
#define REPS 100
#define FUDGE (1.0)

double results[REPS];

static int errorlevel=0;
static void seterr(int e) {if (errorlevel<e) errorlevel=e;}

static void
crash(char *s)
{
    fprintf(stderr, "crash [%s]\n", s); exit(1);
}

#define DIM 20
#define DIM2 (2*(DIM))
#define DIMSIZE (1<<DIM)
#define DIMMASK (DIMSIZE-1)

/* [0]=count [1]=tot [2]=totsq */
static double stats[DIMSIZE][1];

void
init(void)
{
    int i;

    for (i=0; i<DIMSIZE; i++)
    {
	stats[i][0]=0.0;
    }
}

void
dobuf(double b[], int s)
{
//    double y;
    int i, index0=0, index1=0;

    for (i= -DIM2; i<-2; i+=2) index0=(b[i]>b[i+1])+2*index0;
    for (i= 1-DIM2; i<-1; i+=2) index1=(b[i]>b[i+1])+2*index1;

    for (i=0; i<s; i++)
    {
	index0 = ((b[i-2]>b[i-1])+2*index0) & DIMMASK;
	stats[index0][0]++;
	i++; if (i>=s) break;
	index1 = ((b[i-2]>b[i-1])+2*index1) & DIMMASK;
	stats[index1][0]++;
    }
}

int64_t
dostuff(int64_t max)
{
    double buf[DIM2+512];
    int64_t r=0;
    int i, j;

    if (fread(buf, sizeof(double), 512, stdin)!=512) crash("fread failed 1");
    dobuf(buf+DIM2, 512-DIM2);

    j=512-DIM2;
    r=j;

    while (max<0 || r<max)
    {
	for (i=0; i<DIM2; i++) buf[i]=buf[j+i];
	j=512;
	if (max>=0 && j>max-r) j=max-r;
	j=fread(buf+DIM2, sizeof(double), j, stdin);
	if (j<=0) break;
	dobuf(buf+DIM2, j);
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

double
doan(int64_t s)
{
    double x, t, e;
    int i;

    if (0 && s<9000000)
    {
	fprintf(stderr, "warning < 9000000 samples, stats may be wonky.\n");
	seterr(1);
    }

//    printf("expected range [ -3.09 -2.33 -1.28 1.28 2.33 3.09 ]\n");

    e=((double)s)/((double)DIMSIZE);
    t=0.0;

    for (i=0; i<DIMSIZE; i++)
    {
	x=stats[i][0]-e;
	t+=x*x;
    }
    t/=e;

    t-=DIMMASK;
    t/=sqrt(2.0*DIMMASK);
    t*=FUDGE; /* very approx fudge factor to compensate for co-variance. */

    return t;
}

void
dometaan(void)
{
    double x, y, z;
    int j;

    x = 0;
    for (j=0; j<REPS; j++)
    {
	x += results[j];
    }
    x /= REPS;
    printf("mean = %10.5f\n", x);

    y = 0;
    for (j=0; j<REPS; j++)
    {
	z = results[j];
	y += z*z;
    }
    y /= REPS;
    y = sqrt(y);
    printf("sd   = %10.5f\n", y);
}

int
main(int argc, char ** argv)
{
    int64_t r, count;
    double dc;
    int j;

    if (argc>=2)
    {
	if (sscanf(argv[1], "%lf", &dc)!=1 || dc<1.0)
	   crash("1 optional arg: point-count");
	count=(int64_t)dc-DIM;
    }
    else count= -1;

    for (j=0; j<REPS; j++)
    {
	init();
	r=dostuff(SIZ);
	results[j]=doan(r);
    }

    dometaan();

    return errorlevel;
}
