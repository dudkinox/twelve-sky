/* Part of gjrand random number library version 2.0.0.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"
#include "inte.h"

#define SMALLBUF 64
#define SMALLC (SMALLBUF*32)

static int
count64(uint32_t *b, int n)
{
    uint32_t x, y;
    int r=0;

    while (n>0)
    {
	x = b[0]; y = b[1];
	x = ((x>>1)&0x55555555) + (x&0x55555555);
	y = ((y>>1)&0x55555555) + (y&0x55555555);
	x = ((x>>2)&0x33333333) + (x&0x33333333);
	y = ((y>>2)&0x33333333) + (y&0x33333333);
	x += y;
	x = ((x>>4)&0xf0f0f0f) + (x&0xf0f0f0f);
	x = ((x>>8)&0xff00ff) + (x&0xff00ff);
	x = ((x>>16)&0xffff) + (x&0xffff);

	r += x;

	b += 2; n--;
    }

    return r;
}

static int
smallcoins(struct gjrand *s, int n)
	GJRAND_THROW_NOTHING
{
    uint32_t b[SMALLBUF];
    int j, k;

    j = (n+63)>>6;
    gjrand_rand32v(s, j+j, b);
    k = (n-1)&63;
    if (k<32) b[1] = 0;
    b[0] &= ((int32_t)0x80000000)>>(k&31);

    return count64(b, j);
}

int
gjrand_coins(struct gjrand *s, int n) GJRAND_THROW_NOTHING
{
    if (n>SMALLC) return gjrand_inte_binom(s, 0.5, n);
    return smallcoins(s, n);
}
