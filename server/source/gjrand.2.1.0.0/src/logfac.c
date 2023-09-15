/* Part of gjrand random number library version 2.0.0.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"
#include "inte.h"
#include <math.h>

/* return log of factorial. Believed accurate to about 1 part in 1e15. */
/* this is not quite IEEE double precision. */

/* may overflow for large x. Also slow. */
static double
simple(int x)
{
    double r=1.0;
    int j;

    for (j=2; j<=x; j++) r *= j;

    return log(r);
}

/* inaccurate for small x. */
static double
stirling(int x)
{
    double y=x, z=1.0/y, s=z*z, c=s*z, r;

    r = (1.0/1260.0)*c*s;
    r -= (1.0/360.0)*c;
    r += (1.0/12.0)*z;
    r += 0.9189385332046727418;
    r -= y;
    r += log(y)*(y+0.5);
    return r;

    return log(y)*(y+0.5) - y
	+ (1.0/12.0)*z - (1.0/360.0)*c + (1.0/1260.0)*c*s
	+ 0.9189385332046727418;
}

double
gjrand_inte_logfac(int x) GJRAND_THROW_NOTHING
{
    if (x<32) return simple(x);
    return stirling(x);
}
