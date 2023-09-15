/* Part of gjrand random number library version 2.0.0.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <math.h>

#include "gjrand.h"
#include "inte.h"

void
gjrand_insphere(struct gjrand *s, int n, double *a)
	GJRAND_THROW_NOTHING
{
    register double d;

    d=gjrand_drand(s);

    if (n==2) goto neq2;
#ifdef __GNUC__
    if (n==3) {d=cbrt(d); goto done;}
#endif
    if (n==4) {d=sqrt(d); neq2: d=sqrt(d); goto done;}
    d=pow(d, 1.0/n);

    done:
    gjrand_inte_sphere(s, n, a, d);
}
