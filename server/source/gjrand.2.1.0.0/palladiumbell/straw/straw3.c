/* Miscellaneous stuff for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../../src/gjrand.h"

/* This is like blatnorm with a different generator. */

/* This is a "Straw-man" that is supposed to fail some of the tests. */

/* I originally thought this approach might result in a fast generator */
/* that is mostly good but with some correlations corresponding to the */
/* lags. Perhaps with work it could be made that good. The current */
/* version is a total disaster and should fail almost any reasonable test. */

void
normalise(double state[256])
{
    int i;
    double t;

    /* fix the mean */
    t=0.0;
    for (i=0; i<256; i++) t+=state[i];
    t*=1.0/256.0;
    for (i=0; i<256; i++) state[i]-=t;
//fprintf(stderr,"mean=%f\n", t);

    /* fix the sd */
    t=0.0;
    for (i=0; i<256; i++) t+=state[i]*state[i];
    t=sqrt(t*(1.0/256.0));
//fprintf(stderr,"sd=%f\n", t);
    t=1.0/t;
    for (i=0; i<256; i++) state[i]*=t;
}


void
straw(double state[256], double out[256])
{
    int i;

    out[0]=state[255];
    for (i=1; i<256; i++) out[i]=state[-1+i];

    for (i=0; i<4; i++) out[i]-=state[252+i];
    for (i=4; i<256; i++) out[i]-=state[-4+i];

    for (i=0; i<99; i++) out[i]+=state[157+i];
    for (i=99; i<256; i++) out[i]+=state[-99+i];

    for (i=0; i<256; i++) out[i]+=state[i];

    normalise(out);

    for (i=0; i<256; i++) state[i]=out[i];
}

void
initstraw(struct gjrand *g, double out[256])
{
    int i;
    double tmp[256];
    for (i=0; i<256; i++) out[i]=(double)gjrand_rand(g);
    /* This is not normalised, and has totally the wrong distribution. */
    /* Each call to straw should improve this a lot. */
    for (i=0; i<16; i++) straw(out, tmp);
}

void
usage(GJRAND_NO_ARGS)
{
    printf("usage:\n"
"straw3\n"
"straw3 count\n"
"straw3 count integer_seed\n"
"Writes normal variates to standard output in (unportable) binary format.\n"
"Without count, or negative count, writes forever.\n");

    exit(1);
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
    long seed;
    long long count= -1;
    struct gjrand g;
    double state[256];

    if (argc>1) {if (sscanf(argv[1], "%lld", &count)!=1) usage();}

    if (argc>2)
    {
	if (sscanf(argv[2], "%ld", &seed)!=1) usage();
	gjrand_init(&g, seed);
    }
    else gjrand_initrand(&g);

    initstraw(&g, state);

    while (count)
    {
	double b[1024];
	size_t i, c=1024;
	if (count>=0 && c>count) c=count;

	for (i=0; i<c; i+=256) straw(state, b+i);
	if (fwrite(b, sizeof(double), c, stdout)!=c) crash("fwrite");

	if (count>0) count-=c;
    }

    return 0;
}
