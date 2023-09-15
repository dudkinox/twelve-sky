/* Test software for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* This file contains code to combine several single-sided P-values into */
/* just one. */

/* The approach here only considers the lowest value. Other approaches */
/* have been considered but make me nervous. */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* Probability that the smallest of n numbers in [ 0 .. 1 ) is <= x . */
static double
pco_scale(double x, double n)
{
    if (x>=1.0 || x<=0.0) return x;

    /* This is the result we want: */
    /* return 1.0-pow(1.0-x, n); */
    /* except the important cases are with x very small */
    /* so this method gives better accuracy: */

    return -expm1(log1p(-x)*n);
}

/* There are n different one-sided P-values in p[]. */
/* Summarise as a single one. */
double
pcombo(double p[], int n)
{
    double r=1.0;
    int i;

    if (n<=1) return p[0];

    for (i=0; i<n; i++)
    {
	if (p[i]<0.0 || p[i]>1.0) /* basic sanity check */
	    fprintf(stderr, "pcombo(): dubious P-value %g\n", p[i]);
	if (p[i]<r) r=p[i];
    }

    return pco_scale(r, n);
}
