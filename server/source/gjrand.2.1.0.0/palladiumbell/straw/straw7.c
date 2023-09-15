/* Miscellaneous stuff for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "../../src/my_int_types.h"

/* This is like blatnorm but with a different generator. */

/* This is a "Straw-man" that might (?) fail some tests. */

/* This one is an idea for a faster generator that might replace */
/* gjrand_normalv() for really heavy use. Here i'm trying to test its */
/* quality. It is partly based on the ideas of Christopher Stewart Wallace */
/* as in FastNorm etc. */

/* (Actually i know of one problem with this generator, but the tests i've */
/* done so far don't show it. Handle with care. */

#ifdef __cplusplus
#define NOARGS
#else
#define NOARGS void
#endif

#if defined(__cplusplus) || defined(__GNUC__)
#define INLINE inline
#else
#define INLINE
#endif

#include "../../src/gjrand.h"
#include "normaltab.h"

static void
usage(NOARGS)
{
    printf("usage:\n"
"straw7\n"
"straw7 count\n"
"straw7 count integer_seed\n"
"Writes normal variates to standard output in (unportable) binary format.\n"
"Without count, or negative count, writes forever.\n");

    exit(1);
}

static double tabnorm;

static void
initnorm(NOARGS)
{
    int i;
    double r=0.0;

    for (i=0; i<256; i++) r+=normal_app[i]*normal_app[i];
    r *= 1.0/256.0;
    tabnorm=1.0/sqrt(r);
}

static void
normnorm(double *b)
{
    double s=tabnorm, *bend=b+1024;

    do
    {
	b[0]*=s;
	b[1]*=s;
	b[2]*=s;
	b[3]*=s;
	b+=4;
    } while (b<bend);
}

static void
startnorm(unsigned char *bytes, double *b)
{
    double *bend=b+1024;

    do
    {
	b[0] = normal_app[bytes[0]];
	b[1] = normal_app[bytes[1]];
	b[2] = normal_app[bytes[2]];
	b[3] = normal_app[bytes[3]];
	b+=4; bytes+=4;
    } while (b<bend);
}

static void
impnorm(double *bin, double *bout)
{
    double *bend=bout+1024, t, a, b, c, d;

    do
    {
	a=bin[0]; b=bin[256]; c=bin[512]; d=bin[768];
	t=(a+b+c+d)*0.5; a-=t; b-=t; c=t-c; d=t-d;
	bout[0]=a; bout[1]=b; bout[2]=c; bout[3]=d;
	bout+=4; bin++;
    } while (bout<bend);
}

static void
donorm(struct gjrand *g, double b[1024])
{
    unsigned char bytes[1024];
    double b2[1024];

    gjrand_randbytes(g, 1024, bytes);
    startnorm(bytes, b);
    impnorm(b, b2);
    impnorm(b2, b);
    impnorm(b, b2);
    impnorm(b2, b);
    normnorm(b);
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
    int64_t count= -1;
    double dc;
    struct gjrand g;

    if (argc>1)
	{if (sscanf(argv[1], "%lf", &dc)==1) count=dc; else usage();}

    if (argc>2)
    {
	if (sscanf(argv[2], "%ld", &seed)!=1) usage();
	gjrand_init(&g, seed);
    }
    else gjrand_initrand(&g);

    initnorm();

    while (count)
    {
	double b[1024];
	size_t c=1024;
	if (count>=0 && c>count) c=count;

	donorm(&g, b);
	if (fwrite(b, sizeof(double), c, stdout)!=c) crash("fwrite");

	if (count>0) count-=c;
    }

    return 0;
}
