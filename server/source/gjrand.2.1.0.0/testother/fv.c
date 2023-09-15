/* Test software for gjrand random numbers version 1.3.3.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* Some basic sanity checks on gjrand_frandv() */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../src/gjrand.h"
#include "../testcommon/chi2p.h"

#define MAX 1024

void
crash(const char *s)
{
    fprintf(stderr, "crash [%s]\n", s); exit(1);
}

int64_t ct[3][256];

void
do_ct(long double r)
{
    int i, j;

    for (i=0; i<3; i++)
    {
	r *= (long double)256.0;
	j = (int)floor(r);
	r -= (long double)j;
	ct[i][j&255]++;
    }
}

void
chis_1(int64_t count, int i)
{
    double e=count/256.0, x, chi=0.0;
    const int64_t *const p = &(ct[i][0]);
    int j;

    for (j=0; j<255; j++)
    {
	x=p[j]-e;
	chi+=x*x/e;
    }

    x = chi2p2(chi, 255);

    printf("%15.10f     P = %.3g\n", chi, x);
}

void
chis(int64_t count)
{
    int i;

    if (count<2560)
	fprintf(stderr, "less than 2560 samples, don't trust chi-squared.\n");

    printf("expected range [ 191 205 227 284 310 331 ]\n");

    for (i=0; i<3; i++) chis_1(count, i);
}

void
run(int bsize, int64_t count, struct gjrand *g)
{
    float buf[MAX];
    long double r, s=0.0, ss=0.0, max= -1.0e30, min=1.0e30;
    int64_t i=count;
    int j;

    memset(ct, 0, sizeof(ct));

    while (i>0)
    {
	gjrand_frandv(g, bsize, buf);

	for (j=0; j<bsize; j++)
	{
	    r=buf[j];
	    do_ct(r);
	    if (r>max) max=r;
	    if (r<min) min=r;
	    s+=r;
	    r-=0.5;
	    ss+=r*r;
	}

	i--;
    }

    count*=bsize;

    s/=count;
    ss/=count;

    printf(" max = %15.10Lf\n", max);
    printf(" min = %15.10Lf\n", min);
    printf("mean = %15.10Lf\n", s);
    printf("  sd = %15.10f\n", sqrt((double)ss));

    chis(count);
}

int
main(int argc, char **argv)
{
    struct gjrand g;
    double dc;
    int64_t count;
    unsigned long seed;
    int bsize;

    if (argc<3) crash("give two or three args args: blocksize count [ seed ]");

    if (sscanf(argv[1], "%d", &bsize)!=1) crash("blocksize not numeric");
    if (bsize<1 || bsize>MAX)
	crash("blocksize must be in range 1 to 1024 inclusive");

    if (sscanf(argv[2], "%lf", &dc)!=1) crash("count not numeric");
    count = (int64_t)dc;

    if (argc>=4)
    {
	if (sscanf(argv[3], "%lu", &seed)!=1) crash("seed not numeric");
	gjrand_init(&g, seed);
    }
    else gjrand_initrand(&g);

    run(bsize, count, &g);

    return 0;
}
