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

/* This is a pentanomial, and has longer lag than lfib3. */
/* Pentanomials are slower than trinomials. The floating point */
/* comparisons to bring the number back into range are particularly */
/* annoying. Even so it is not outrageously slow. */

static double buf[1024];

static void
renewbuf1(void)
{
    double t;
    int j;

    for (j=0; j<16; j++)
    {
	t=buf[(j-2)&1023]-buf[(j-3)&1023];
	if (t<0) t+=1.0;
	t-=buf[(j-5)&1023];
	if (t<0) t+=1.0;
	t-=buf[(j-16)&1023];
	if (t<0) t+=1.0;
	buf[j]=t;
    }
}

static void
renewbuf2(void)
{
    double t;
    int j;

    for (j=16; j<1024; j++)
    {
	t=buf[j-2]-buf[j-3];
	if (t<0.0) t+=1.0;
	t-=buf[j-5];
	if (t<0.0) t+=1.0;
	t-=buf[j-16];
	if (t<0.0) t+=1.0;
	buf[j]=t;
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

    for (i=1008; i<1023; i++)
    {
	t=t/3.0 + 0.001 + t/123456789.0;
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
