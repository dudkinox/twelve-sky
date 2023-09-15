/* Part of gjrand random number library version 1.0.0.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"
#include "inte.h"

double
gjrand_drand(struct gjrand *s)
	GJRAND_THROW_NOTHING
{
    uint64_t b[4];
    double r;

    gjrand_inte_crank64(s, b);

#define Q (1.0/4.0)
#define S8 (Q*Q*Q*Q)
#define S32 (S8*S8*S8*S8)
#define S64 (S32*S32)

    r = ((double)(b[0])) * S64;

    if (r==1.0) r=0.999999;

    return r;
}
