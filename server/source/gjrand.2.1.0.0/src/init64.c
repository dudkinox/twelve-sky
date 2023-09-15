/* Part of gjrand random number library version 1.0.1.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"

void
gjrand_init64(struct gjrand *s, uint64_t x)
	GJRAND_THROW_NOTHING
{
    s->a=0; s->b=x*123456789ul; s->c=0; s->d=2000001ul;
}
