/* Part of gjrand random number library version 2.0.0.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"
#include "inte.h"

void
gjrand_inte_rand48_set7(struct gjrand *g, const unsigned short x[7])
	GJRAND_THROW_NOTHING
{
    g->a = (((uint64_t)(x[2]))<<32) | (((uint32_t)(x[1]))<<16) | x[0];
    g->b = (((uint64_t)(x[6]))<<48) | (((uint64_t)(x[5]))<<32)
	| (((uint32_t)(x[4]))<<16) | x[3];
    g->c=0;
    g->d=48000001ul;
}
