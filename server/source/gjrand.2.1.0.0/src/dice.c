/* Part of gjrand random number library version 1.0.0.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"

int
gjrand_dice(struct gjrand *s, int sides, int n)
	GJRAND_THROW_NOTHING
{
    uint32_t b[8];
    uint32_t big;
    int i, r=0;

    if (n<=0 || sides<=1) return 0;

    big=0xffffffff - (0xffffffff%sides);

    while (1)
    {
	gjrand_rand32v(s, 8, b);
	for (i=7; i>=0; i--) if (b[i]<big)
	{
	    r+=b[i]%sides;
	    n--;
	    if (n<=0) return r;
	}
    }
}
