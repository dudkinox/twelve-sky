/* Part of gjrand random number library version 1.3.0.0 or later. */
/* Copyright (C) 2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <math.h>
#include "gjrand.h"
#include "inte.h"

double
gjrand_mbt_speed(struct gjrand *s) GJRAND_THROW_NOTHING
{
    return sqrt(gjrand_chisquared(s, 4))*GJRAND_INTE_SQRT1_3;
}
