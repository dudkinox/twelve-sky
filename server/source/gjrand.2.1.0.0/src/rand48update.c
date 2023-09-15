/* Part of gjrand random number library version 2.0.0.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"
#include "inte.h"

void
gjrand_inte_rand48_update(const struct gjrand *g, unsigned short x[3])
	GJRAND_THROW_NOTHING
{
    x[0]=g->a;
    x[1]=(g->a)>>16;
    x[2]=(g->a)>>32;
}
