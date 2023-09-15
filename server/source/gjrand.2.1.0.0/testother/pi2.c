/* Test software for gjrand random numbers version 1.3.3.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../src/my_int_types.h"
#include "../src/gjrand.h"
#include "../testcommon/chi2p.h"

/* Monte-Carlo calculation of PI, using a circle. */

void
crash(const char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

int64_t
countin(int64_t count, struct gjrand *s)
{
    int64_t r=0;

    do
    {
	double x[4];
	gjrand_drand_array(s, 4, x);
	r += x[0]*x[0]+x[1]*x[1] < 1.0;
	r += x[2]*x[2]+x[3]*x[3] < 1.0;
	count-=2;
    } while (count>0);

    return r;
}

int
main(int argc, char **argv)
{
    struct gjrand s;
    double pi, p, chis, e;
    int64_t count, v;
    unsigned long seed;

    if (argc!=3 && argc!=2) crash("usage: pi2 count [ seed ]");
    if (sscanf(argv[1], "%lf", &e)!=1) crash("count not a number");
    count = (uint64_t)e; count &= ~((uint64_t)1);
    if (argc==3)
    {
	if (sscanf(argv[2], "%lu", &seed)!=1) crash("seed not a number");
	gjrand_init(&s, seed);
    }
    else gjrand_initrand(&s);

    v=countin(count, &s);
    pi = 4.0 * v / count;
    printf("(approx) PI = %15.12f\n", pi);

    e = (M_PI/4.0)*count;
    chis = (v-e)*(v-e)/e;

    e = count-e;
    v = count-v;
    chis += (v-e)*(v-e)/e;

    p = chi2p2(chis, 1);

    printf("expect [ 0.00000 0.00016 0.0157 2.71 6.65 10.8 ]\n");
    printf("chis = %15.12f     df = 1     P = %.3g\n", chis, p);

    return 0;
}
