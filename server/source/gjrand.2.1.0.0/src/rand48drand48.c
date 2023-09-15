/* Part of gjrand random number library version 1.0.0.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"
#include "inte.h"

double
gjrand_drand48(GJRAND_NO_ARGS)
	GJRAND_THROW_NOTHING
{
    struct gjrand g;
    double r;
    gjrand_inte_rand48_set7(&g, gjrand_inte_rand48_state);
    r=gjrand_drand(&g);
    gjrand_inte_rand48_update(&g, gjrand_inte_rand48_state);
    return r;
}
