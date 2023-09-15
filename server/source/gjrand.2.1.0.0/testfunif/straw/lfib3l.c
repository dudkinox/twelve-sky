/* Miscellaneous stuff for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "../../src/my_int_types.h"

/* This one writes a file for use by the tests in testfunif */

/* optional arg is random seed. */

/* This is a lagged fibonacci generator. */
/* This one is a trinomial with moderately large lags. */
/* It is essentially a floating point version of ../testunif/straw/lfib3 */

#define LAG1 100
#define LAG2 37
#define BSIZE 1024

static double buf[BSIZE];

static void
renewbuf1(void)
{
    double t;
    int j;

    for (j=0; j<LAG2; j++)
    {
	t=buf[j+BSIZE-LAG1]-buf[j+BSIZE-LAG2];
	if (t<0.0) t+=1.0;
	buf[j]=t;
    }

    for (j=LAG2; j<LAG1; j++)
    {
	t=buf[j+BSIZE-LAG1]-buf[j-LAG2];
	if (t<0.0) t+=1.0;
	buf[j]=t;
    }
}

/* this is an unrolled version. It assumes BSIZE-LAG1 divisible by 4. */
static void
renewbuf2(void)
{
    double t, u, v, w;
    int j;

    for (j=LAG1; j<BSIZE; j+=4)
    {
	t=buf[j-LAG1];
	u=buf[j-LAG1+1];
	v=buf[j-LAG1+2];
	w=buf[j-LAG1+3];
	t-=buf[j-LAG2];
	u-=buf[j-LAG2+1];
	v-=buf[j-LAG2+2];
	w-=buf[j-LAG2+3];
	if (t<0.0) t+=1.0;
	if (u<0.0) u+=1.0;
	if (v<0.0) v+=1.0;
	if (w<0.0) w+=1.0;
	buf[j]=t;
	buf[j+1]=u;
	buf[j+2]=v;
	buf[j+3]=w;
    }
}

#define S2 0.25
#define S8 (S2*S2*S2*S2)
#define S32 (S8*S8*S8*S8)

static void
doseed(uint32_t seed)
{
    double t=seed*(S32);
    int i;

    for (i=BSIZE-LAG1; i<BSIZE-1; i++)
    {
	t=t/54321.0 + t/3.0 + 0.001;
	buf[i]=t+0.5;
    }

    buf[BSIZE-1]=nextafter(1.0, 0.0);

    renewbuf1();
    renewbuf2();
}

static void
blat(void)
{
    while (1)
    {
	renewbuf1();
	renewbuf2();
	if (fwrite(buf, sizeof(double), BSIZE, stdout)!=BSIZE) break;
    }
}

int
main(int argc, char **argv)
{
    unsigned long t;
    uint32_t seed;

    if (argc>1) {sscanf(argv[1], "%lu", &t); seed=t;}
    else seed=time(0);

    doseed(seed);
    blat();

    return 0;
}
