/* Part of gjrand random number library version 1.0.0.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"
#include <math.h>

long double
gjrand_ldrand(struct gjrand *s)
	GJRAND_THROW_NOTHING
{
    uint32_t b[3];
    long double r;

    gjrand_rand32v(s, 3, b);

#define Q ((long double)(1.0/4.0))
#define S8 (Q*Q*Q*Q)
#define S32 (S8*S8*S8*S8)

    r=( ( ((long double)(b[0]))*S32 + ((long double)(b[1])) )*S32
	+ ((long double)(b[2])) )*S32;

    if (b[2]==(uint32_t)(-1l) && r==(long double)1.0) r=0.999999;

    return r;
}
