/* Part of gjrand random number library version 1.3.0.0 or later. */
/* Copyright (C) 2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <math.h>
#include "gjrand.h"
#include "inte.h"

void
gjrand_mb_velocity(struct gjrand *s, double v[3]) GJRAND_THROW_NOTHING
{
    gjrand_normalv(s, 3, v);
    v[0] *= GJRAND_INTE_SQRT1_3;
    v[1] *= GJRAND_INTE_SQRT1_3;
    v[2] *= GJRAND_INTE_SQRT1_3;
}
