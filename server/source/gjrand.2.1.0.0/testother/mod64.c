/* Test software for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../src/gjrand.h"

static void
domod(uint64_t mod, int count, struct gjrand *s)
{
    long double t, mean=0.0, sd=0.0;
    long double offset=((long double)(mod-1))*0.5;
    uint64_t min = -((uint64_t)1), max=0, l;
    int i;

    for (i=0; i<count; i++)
    {
	l=gjrand_rand64mod(s, mod);
	if (l>max) max=l;
	if (l<min) min=l;
	t=(long double)l;
	mean+=t;
	t-=offset;
	sd+=t*t;
    }

    printf("min = %.0f\n", (double)min);
    printf("max = %.0f\n", (double)max);
    mean/=count;
    printf("mean= %f\n", (double)mean);
    sd = sqrtl(sd/count);
    printf("sd  = %f\n", (double)sd);
}

static void
crash(const char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

int
main(int argc, char **argv)
{
    double dmod;
    uint64_t mod;
    int count, seed;
    struct gjrand s;

    if (argc<3) crash("2 or 3 args: mod count [ seed ]");

    if (sscanf(argv[1], "%lf", &dmod)!=1) crash("mod not a number");
    mod = (uint64_t)dmod;

    if (sscanf(argv[2], "%d", &count)!=1) crash("count not a number");
    if (argc>3)
    {
	if (sscanf(argv[3], "%d", &seed)!=1) crash("seed not a number");
	gjrand_init(&s, seed);
    }
    else gjrand_initrand(&s);

    domod(mod, count, &s);

    return 0;
}
