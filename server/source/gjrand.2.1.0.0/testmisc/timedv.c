/* Test software for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* Silly timing benchmark to see how fast the basic double precision */
/* generator is. This does 1e8 random numbers. */

#include <stdlib.h>
#include <stdio.h>
#include "../src/gjrand.h"

static double
testthou(struct gjrand *g)
{
    static double buf[1000];
    double r=0;
    int i;

    gjrand_drandv(g, 1000, buf);
    for (i=0; i<1000; i++) r+=buf[i];

    return r;
}

static double
test(struct gjrand *g)
{
    int i;
    double r=0.0;
    for (i=0; i<100000; i++) r+=testthou(g);
    return r;
}

int
main(int argc, char **argv)
{
    struct gjrand g;

    gjrand_initrand(&g);
    printf("%f\n", test(&g));

    return 0;
}
