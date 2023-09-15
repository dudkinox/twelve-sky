/* Part of gjrand random number library version 1.0.0.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"
#include "inte.h"

int
gjrand_oldrand(GJRAND_NO_ARGS)
	GJRAND_THROW_NOTHING
{
    uint64_t b[4];

    gjrand_inte_crank64(&gjrand_inte_oldrand_state, b);

    return (int)(b[0])&GJRAND_RAND_MAX;
}
