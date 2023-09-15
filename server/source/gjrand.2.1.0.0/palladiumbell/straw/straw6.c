/* Miscellaneous stuff for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

/* This is like blatnorm but with a different generator. */

/* This is a "Straw-man" that is supposed to fail some tests. */

/* This one is quite elaborate and in some ways is "too good". */
/* It is likely to fail the chi-squared tests on the low side, for instance. */
/* It inverts the library function erf() to generate n-tiles of the normal */
/* distribution, then uses calls to gjrand_rand32mod() to scramble the order. */

/* This is a good test of your library erf() function, if you have one. */

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

void
usage(NOARGS)
{
    printf("usage:\n"
"straw6\n"
"straw6 count\n"
"straw6 count integer_seed\n"
"Writes normal variates to standard output in (unportable) binary format.\n"
"Without count, or negative count, writes forever.\n");

    exit(1);
}

double
inverf(double x, double high, double *gap)
{
    double h=high, l=h-1.3*(*gap), y, n, i=0;

    while (1)
    {
	n=(l+h)*0.5;
	if (l>=n) break;
	if (n>=h) break;
	y=erf(n*M_SQRT1_2);
	if (y==x) break;
	else if (y>x) h=n;
	else l=n;
	i++;
	if (i>1100)
	{
	    /* this shouldn't happen */
	    fprintf(stderr, "loop %.20e %.20e %.20e\n", l, n, h);
	    exit(1);
	}
    }

    *gap=high-n;
    return n;
}

#define SIZ (4*1024*1024)
// #define SIZ 4096

struct gjrand g;
double buf[SIZ];

void
mkbuf(void)
{
    double t=10.0, x, lastgap=10.0;
    int i, j;

    i=SIZ/2-1; j=SIZ/2;

    do
    {
	x=(double)(2*i+1)*(1.0/SIZ);
	t=inverf(x, t, &lastgap);
	buf[i]=t; buf[j]= -t;
	i--; j++;
    } while (i>=0);
}

void
scramble(void)
{
    int i, j;
    double t;

    for (i=1; i<SIZ; i++)
    {
	j=gjrand_rand32mod(&g, i+1);
	t=buf[j]; buf[j]=buf[i]; buf[i]=t;
    }
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
    size_t c;

    if (argc>1)
	{if (sscanf(argv[1], "%lld", &count)!=1) usage();}

    mkbuf();

    if (argc>2)
    {
	if (sscanf(argv[2], "%ld", &seed)!=1) usage();
	gjrand_init(&g, seed);
    }
    else gjrand_initrand(&g);

    while (count)
    {
	c=SIZ;
	if (count>=0 && c>count) c=count;

	scramble();
	if (fwrite(buf, sizeof(double), c, stdout)!=c) crash("fwrite");

	if (count>0) count-=c;
    }

    return 0;
}
