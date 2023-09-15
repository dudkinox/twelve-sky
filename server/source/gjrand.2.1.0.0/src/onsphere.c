/* Part of gjrand random number library version 1.2.0.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <math.h>

#include "gjrand.h"
#include "inte.h"

void
gjrand_onsphere(struct gjrand *s, int n, double *a)
	GJRAND_THROW_NOTHING
{
    gjrand_inte_sphere(s, n, a, 1.0);
}
