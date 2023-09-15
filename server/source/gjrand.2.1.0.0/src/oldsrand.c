/* Part of gjrand random number library version 1.0.0.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"
#include "inte.h"

void
gjrand_oldsrand(unsigned int seed)
	GJRAND_THROW_NOTHING
{
    gjrand_init(&gjrand_inte_oldrand_state, (uint32_t)seed);
}
