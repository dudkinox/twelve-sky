/* Part of gjrand random number library version 1.0.0.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"
#include "inte.h"

unsigned long
gjrand_rand32(struct gjrand *s)
	GJRAND_THROW_NOTHING
{
    uint64_t b[4];

    gjrand_inte_crank64(s, b);

    return ((unsigned long)(b[0]))&0xffffffff;
}
