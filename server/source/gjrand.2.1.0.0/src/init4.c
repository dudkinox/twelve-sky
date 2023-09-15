/* Part of gjrand random number library version 1.0.0.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"

void
gjrand_init4(struct gjrand *s, const uint32_t x[4])
	GJRAND_THROW_NOTHING
{
    s->a=0;
    s->b = (((uint64_t)x[3])<<32) | x[2];
    s->c = (((uint64_t)x[1])<<32) | x[0];
    s->d=5000001ul;
}
