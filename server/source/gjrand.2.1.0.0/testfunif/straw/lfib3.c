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
/* For serious use longer lags should be used. */
/* This is very weak. */

static double buf[1024];

static void
renewbuf1(void)
{
    double t;

    t=buf[1019]-buf[1022];
    if (t<0.0) t+=1.0;
    buf[0]=t;

    t=buf[1020]-buf[1023];
    if (t<0.0) t+=1.0;
    buf[1]=t;

    t=buf[1021]-buf[0];
    if (t<0.0) t+=1.0;
    buf[2]=t;

    t=buf[1022]-buf[1];
    if (t<0.0) t+=1.0;
    buf[3]=t;

    t=buf[1023]-buf[2];
    if (t<0.0) t+=1.0;
    buf[4]=t;

    t=buf[0]-buf[3];
    if (t<0.0) t+=1.0;
    buf[5]=t;
}

static void
renewbuf2(void)
{
    register double t=buf[4], u=buf[5];
    int j;

    for (j=6; j<1024; j+=2)
    {
	t=buf[j-5]-t;
	u=buf[j-4]-u;
#if 0
	if (t<0.0) t+=1.0;
	if (u<0.0) u+=1.0;
#else
	t -= copysign(0.5, t); t+=0.5;
	u -= copysign(0.5, u); u+=0.5;
#endif
	buf[j]=t;
	buf[j+1]=u;
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

    for (i=1019; i<1023; i++)
    {
	t=t/54321.0 + t/5.0 + 0.001;
	buf[i]=t+0.5;
    }

    buf[1023]=nextafter(1.0, 0.0);

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
	if (fwrite(buf, sizeof(double), 1024, stdout)!=1024) break;
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
