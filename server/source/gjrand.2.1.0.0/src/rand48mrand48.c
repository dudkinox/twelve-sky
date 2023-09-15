/* Part of gjrand random number library version 1.0.0.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"
#include "inte.h"

long int
gjrand_mrand48(GJRAND_NO_ARGS)
	GJRAND_THROW_NOTHING
{
    struct gjrand g;
    int32_t r;
    gjrand_inte_rand48_set7(&g, gjrand_inte_rand48_state);
    r=(int32_t)gjrand_rand32(&g);
    gjrand_inte_rand48_update(&g, gjrand_inte_rand48_state);
    return r;
}
