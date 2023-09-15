/* Part of gjrand random number library version 1.0.0.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"
#include "inte.h"

long int
gjrand_nrand48(unsigned short int xsubi[3])
	GJRAND_THROW_NOTHING
{
    struct gjrand g;
    long int r;
    gjrand_inte_rand48_set3(&g, xsubi);
    r=gjrand_rand(&g);
    gjrand_inte_rand48_update(&g, xsubi);
    return r;
}
