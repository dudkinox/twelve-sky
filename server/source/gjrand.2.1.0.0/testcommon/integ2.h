/* Test software for gjrand random numbers version 2.0.0.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#ifndef GJRAND_INTEG2_H
#define GJRAND_INTEG2_H

#include "integrate.h"

/* Integrate f from lo to either +inf or 0 with increment starting */
/* at inc. acc and l are accuracy parameters. */
/* f must max out somewhere quite close to base and tend to zero */
/* as we move away. */
static double
gjrt_integ2(double (*f)(double x), double lo, double inc, double acc, int l)
{
    double r, oom, t, hi;
    int j;

    hi = lo+inc; if (hi<0.0) hi = 0.0;
    r = gjrt_integrate(f, lo, hi, l);

    oom = fabs(r*acc);

    for (j=16; j>=0; j--)
    {
	if (hi<=0.0) break;
	lo = hi; inc *= 2.0; hi = lo+inc; if (hi<0.0) hi = 0.0;
	t = gjrt_integrate(f, lo, hi, l);
	r += t;
	if (fabs(t)<oom) break;
    }

    return r;
}

#endif /* GJRAND_INTEG2_H */
