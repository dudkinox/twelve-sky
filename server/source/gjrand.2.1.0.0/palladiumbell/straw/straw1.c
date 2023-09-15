/* Miscellaneous stuff for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../../src/gjrand.h"
#include "normaltab.h"

/* This is like blatnorm with a different generator. */

/* This one is the old gjrand_normal() used in versions 1.0.0 to 1.0.3 . */
/* I suggest two exercises for the exceedingly keen student: */
/* 1. Demonstrate a weakness in this generator by actual testing. */
/* 2. Then fix it. */
/* I wasn't quite that keen. */

static void
crash(const char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

void
straw(struct gjrand *g, double out[2])
{
    uint32_t a[8], *b=a ;
    double r;
    uint32_t n;
    int32_t m;
    int i, j;

    gjrand_rand32v(g, 8, b);

#define Q (1.0/4.0)
#define S8 (Q*Q*Q*Q)
#define S32 (S8*S8*S8*S8)

    j=1;
    do
    {
	r=b[0]*S32;
	m=(b[1]>>2)+(b[2]>>2)+(b[3]>>2)-0x60000000;
	r+=m;
	r*=S32;

	i=3;
	do
	{
	    n=b[i];
	    r*=997.0/1024.0;
	    r+=normal_app[n&255];
	    r+=normal_app[(n>>8)&255];
	    r+=normal_app[(n>>16)&255];
	    i--;
	} while (i>=0);

	b+=4;
	out[j]=r;
	j--;
    } while (j>=0);

}

void
usage(GJRAND_NO_ARGS)
{
    printf("usage:\n"
"straw1\n"
"straw1 count\n"
"straw1 count integer_seed\n"
"Writes normal variates to standard output in (unportable) binary format.\n"
"Without count, or negative count, writes forever.\n");

    exit(1);
}

int
main(int argc, char **argv)
{
    long seed;
    long long count= -1;
    struct gjrand g;

    if (argc>1) {if (sscanf(argv[1], "%lld", &count)!=1) usage();}

    if (argc>2)
    {
	if (sscanf(argv[2], "%ld", &seed)!=1) usage();
	gjrand_init(&g, seed);
    }
    else gjrand_initrand(&g);

    while (count)
    {
	double b[1024];
	size_t i, c=1024;
	if (count>=0 && c>count) c=count;

	for (i=0; i<c; i+=2) straw(&g, b+i);
	if (fwrite(b, sizeof(double), c, stdout)!=c) crash("fwrite");

	if (count>0) count-=c;
    }

    return 0;
}
