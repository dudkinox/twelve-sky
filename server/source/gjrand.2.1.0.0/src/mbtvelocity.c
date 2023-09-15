/* Part of gjrand random number library version 1.3.0.0 or later. */
/* Copyright (C) 2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <math.h>
#include "gjrand.h"
#include "inte.h"

void
gjrand_mbt_velocity(struct gjrand *s, double v[3]) GJRAND_THROW_NOTHING
{
    double t[4];

    gjrand_normalv(s, 4, t);
    v[0] = sqrt(t[0]*t[0]+t[3]*t[3]) *  GJRAND_INTE_SQRT1_3;
    v[1] = t[1] * GJRAND_INTE_SQRT1_3;
    v[2] = t[2] * GJRAND_INTE_SQRT1_3;
}
