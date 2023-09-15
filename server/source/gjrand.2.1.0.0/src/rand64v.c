/* Part of gjrand random number library version 1.0.0.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"
#include "inte.h"

void
gjrand_rand64v(struct gjrand *s, int n, uint64_t *v)
	GJRAND_THROW_NOTHING
{

    if (n&3)
    {
	uint64_t b[4];
	int i, j=n&3;
	gjrand_inte_crank64(s, b);
	for (i=0; i<j; i++) v[i] = b[i];
	v+=j; n-=j;
    }

    while (n>0)
    {
	gjrand_inte_crank64(s, v);
	v+=4;
	n-=4;
    }
}
