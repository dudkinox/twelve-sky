/* Part of gjrand random number library version 1.1.0.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"

double
gjrand_normal(struct gjrand *s)
	GJRAND_THROW_NOTHING
{
    double r;

    gjrand_normalv(s, 1, &r);
    return r;
}
