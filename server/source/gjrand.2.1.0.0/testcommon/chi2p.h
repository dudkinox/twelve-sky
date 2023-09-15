/* Test software for gjrand random numbers version 2.0.0.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "integ2.h"

/* Convert chi-squared scores into P-values. */
/* The useful functions to be called from outside are chi2p1() and chi2p2() . */
/* chi2p1() maps chi-squared values on the high side to small p-values. */
/* chi2p2() maps chi-squared values on both sides to small p-values. */

/* I don't make any great claims for this code. It's not thread-safe. */
/* It's too slow to use in inside loops. I don't care how accurate it is */
/* as long as it manages 3 significant digits most of the time. */
/* Much of the complexity is concerned with avoiding overflow and underflow. */
/* This is supposed to work over a very wide range of (positive, real) chis */
/* and df values. */

static double chi2p_funoff, chi2p_dfm1;

static double
chi2p_logfun(double x)
{
    if (x<=0.0) return -1.0E35;
    return log(x)*chi2p_dfm1 - 0.5*x*x;
}

static double
chi2p_fun(double x)
{
    return exp(chi2p_logfun(x) - chi2p_funoff);
}

static double
chi2p_integrate(double base, double incr)
{
    double lo, inc;
    lo = sqrt(base); inc = incr*2.0/lo;
    return gjrt_integ2(&chi2p_fun, lo, inc, 1.0e-5, 4);
}

/* Used internally. With positive return value, P-value is measured from */
/* the high side, with negative return, from the low side. This avoids */
/* some roundoff errors for extreme results. */
static int
chi2p_val(double chis, int df, double *result)
{
    double mean=df;
    double sdev=sqrt(2.0*mean);
    double tot, tl, th, ot, fuo;

    chi2p_dfm1 = df-1.0;
    chi2p_funoff = fuo = chi2p_logfun(sqrt((double)df));

    tl = chi2p_integrate(mean, -sdev);
    th = chi2p_integrate(mean, sdev);
    tot = tl+th;

    chi2p_funoff = chi2p_logfun(sqrt(chis));
    fuo = exp(chi2p_funoff-fuo);

    if (chis<mean)
    {
	ot = chi2p_integrate(chis, -sdev);
	ot /= tot;
	ot *= fuo;
	*result = ot;
	return -1;
    }
    else
    {
	ot = chi2p_integrate(chis, sdev);
	ot /= tot;
	ot *= fuo;
	*result = ot;
	return 1;
    }
}

double
chi2p1(double chis, int df)
{
    double t;
    int j;

    j = chi2p_val(chis, df, &t);
    if (j<0) t = 1.0-t;

    return t;
}

double
chi2p2(double chis, int df)
{
    double t;
    int j;

    j = chi2p_val(chis, df, &t);
    if (t>0.5) t = 1.0-t;
    t *= 2.0;

    return t;
}
