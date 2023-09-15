/* Part of gjrand random number library version 1.0.0.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"
#include "inte.h"

uint64_t
gjrand_rand64mod(struct gjrand *s, uint64_t mod)
	GJRAND_THROW_NOTHING
{
    uint64_t b[4];
    uint64_t big, t;
#define HUGE ((uint64_t)(-1))

    if (mod<=1) return 0;

    big = HUGE - (HUGE%mod);

    while (1)
    {
	gjrand_inte_crank64(s, b);
	t= b[0]; if (t<big) return t%mod;
	t= b[1]; if (t<big) return t%mod;
	t= b[2]; if (t<big) return t%mod;
	t= b[3]; if (t<big) return t%mod;
    }

}
