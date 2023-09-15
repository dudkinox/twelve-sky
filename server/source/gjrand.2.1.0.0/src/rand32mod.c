/* Part of gjrand random number library version 1.0.0.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"

uint32_t
gjrand_rand32mod(struct gjrand *s, uint32_t mod)
	GJRAND_THROW_NOTHING
{
    return gjrand_rand64mod(s, mod);
}
