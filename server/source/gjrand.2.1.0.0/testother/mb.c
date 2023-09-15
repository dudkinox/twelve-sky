/* Test software for gjrand random numbers version 1.3.3.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../src/gjrand.h"
#include "../testcommon/chi2p.h"

/* Various tests for gjrand_mb_speed and gjrand_mbt_speed. */
/* First arg is number of trials. */
/* Second optional arg is random seed. */

#define STEPS 64
#define LO (0.06)
#define HI (2.61)
#define DELTA ((HI-LO)*(1.0/STEPS))
#define IDEL (STEPS/(HI-LO))

static void
crash(const char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

static long histo[STEPS+1];
static double expect[STEPS+1];
static double sum=0.0, ss=0.0;

static double
vdist(const double x)
{

    return exp(x*x*(-1.5))*x*x*4.145929793656;
}

static double
tdist(const double x)
{
    return exp(x*x*(-1.5))*x*x*x*4.5;
}

static double
chis(long count, double *pv)
{
    double tot=0, x, d;
    int j;

    for (j=0; j<STEPS; j++)
    {
	x = expect[j]*count;
	d = histo[j]-x;
	tot += d*d/x;
    }

    *pv = chi2p2(tot, STEPS-1);

    return tot;
}

static void
mkdist(double (*f)(double x))
{
    double t, m=0.0, rms=0.0, x, x2, y;
    int j, k;

    for (k = 0; k < STEPS; k++)
    {
	x = k*DELTA+LO;
	t = -(*f)(x);
	for (j = 0; j < 32768; j++)
	{
	    x2 = j*(DELTA/32768.0)+x;
	    t += (*f)(x2)*2.0;
	    x2 += (DELTA/65536.0);
	    t += (*f)(x2)*4.0;
	}
	x2 = x+DELTA;
	t += (*f)(x2);
	expect[k] = t*(DELTA/6.0/32768.0);
    }

    y = (*f)(0.0);
    t = -y;

    for (j=999999; j>=0; j--)
    {
	x = j*(1.0/65536.0);
	x2 = x+(0.5/65536.0);
	y = (*f)(x)*2.0;
	t += y; m += x*y; rms += x*x*y;
	y = (*f)(x2)*4.0;
	t += y; m += x2*y; rms += x2*x2*y;
    }

    x = 999999/65536.0;
    y = (*f)(x);
    t += y; m += x*y; rms += x*x*y;

    m /= t; rms = sqrt(rms/t); t /= 6.0*65536.0;

    printf("total = %20.15f\nmean = %20.15f\nrms  = %20.15f\n\n", t, m, rms);
}

static double
mbv(struct gjrand *s)
{
    double v[3];

    gjrand_mb_velocity(s, v);
    return sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
}

static double
mbvt(struct gjrand *s)
{
    double v[3];

    gjrand_mbt_velocity(s, v);
    return sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
}

static void
dobox(long count, struct gjrand *s, double (*fn)(struct gjrand *s))
{
    int j;

    sum=0.0; ss=0.0;
    for (j=0; j<STEPS; j++) histo[j]=0;

    while (count>0)
    {
	double t=(*fn)(s);
	int k=(int)floor((t-LO)*IDEL);
	if ((unsigned)k<STEPS) histo[k]++;
	sum+=t;
	ss+=t*t;
	count--;
    }
}

static void
doan(long count, const char *caption)
{
    double c, p;

    printf("Maxwell's %s distribution\n", caption);
    printf("mean = %12.6f\n", sum/count);
    printf("RMS  = %12.6f\n", sqrt(ss/count));
    printf("SD   = %12.6f\n", sqrt(ss/count-(sum/count)*(sum/count)));
    c = chis(count, &p);
    printf("CHIS = %12.6f     P = %.3g\n\n", c, p);
}

int
main(int argc, char **argv)
{
    long count, seed;
    struct gjrand s;

    if (argc<2) crash("1 or 2 args: count [ seed ]");
    if (sscanf(argv[1], "%ld", &count)!=1) crash("count not a number");
    if (argc>2)
    {
	if (sscanf(argv[2], "%ld", &seed)!=1) crash("seed not a number");
	gjrand_init(&s, seed);
    }
    else gjrand_initrand(&s);

    mkdist(vdist);

    dobox(count, &s, gjrand_mb_speed);
    doan(count, "speed");

    dobox(count, &s, mbv);
    doan(count, "velocity");

    mkdist(tdist);

    dobox(count, &s, gjrand_mbt_speed);
    doan(count, "transport");

    dobox(count, &s, mbvt);
    doan(count, "transport velocity");

    return 0;
}
