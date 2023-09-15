/* Test software for gjrand random numbers version 2.0.0.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* This is a cruddy implementation of the erfc() function. */
/* Several test programs for gjrand need erfc(), and nice C libraries */
/* provide it. Here's an emergency implementation in case your's doesn't. */
/* This one probably doesn't meet proper standards of accuracy or speed, */
/* but it's good enough for our purposes. */

/* If you need to use this the easiest way is probably to #include this file. */

/* erfc(x) = (2.0/sqrt(PI)) * integral from x to +infinity exp(-t*t) dt. */

#include "integ2.h"
#include <math.h>

/* 2.0/sqrt(PI) */
#ifndef M_2_SQRTPI
#define M_2_SQRTPI     1.12837916709551257
#endif

static double
erfc_norm(double x)
{
    return exp(-x*x);
}

static double
erfc_1(double x)
{
    return gjrt_integ2(&erfc_norm, x, 0.25, 1.0e-16, 6) * M_2_SQRTPI;
}

double
erfc(double x)
{
    if (x>=0) return erfc_1(x);
    return 2.0-erfc_1(-x);
}
