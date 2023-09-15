/* Part of gjrand random number library version 1.0.0.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"
#include "inte.h"

void
gjrand_rand32v(struct gjrand *s, int n, uint32_t *v)
	GJRAND_THROW_NOTHING
{
    int m=n&7;
    if (m)
    {
	uint64_t buf[4], *b=buf;
	gjrand_inte_crank64(s, b);
	if (m&1)
	{
	    *v++ = (uint32_t)(*b++);
	    m--;
	}
	while (m>0)
	{
	    v[0] = ((uint32_t)(*b)) & 0xffffffff;
	    v[1] = ((uint32_t)((*b)>>32)) & 0xffffffff;
	    v+=2; b++;
	    m-=2;
	}

	n &= (~7);
    }

    while (n>0)
    {
	uint64_t buf[4];

	gjrand_inte_crank64(s, buf);

	v[0] = ((uint32_t)(buf[0])) & 0xffffffff;
	v[1] = ((uint32_t)(buf[0]>>32)) & 0xffffffff;
	v[2] = ((uint32_t)(buf[1])) & 0xffffffff;
	v[3] = ((uint32_t)(buf[1]>>32)) & 0xffffffff;
	v[4] = ((uint32_t)(buf[2])) & 0xffffffff;
	v[5] = ((uint32_t)(buf[2]>>32)) & 0xffffffff;
	v[6] = ((uint32_t)(buf[3])) & 0xffffffff;
	v[7] = ((uint32_t)(buf[3]>>32)) & 0xffffffff;

	v+=8;
	n-=8;
    }
}
