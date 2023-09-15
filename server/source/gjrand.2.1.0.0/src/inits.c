/* Part of gjrand random number library version 1.0.1.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"
#include "inte.h"
#include <string.h>

#define L64(b) gjrand_inte_l64(b)

void
gjrand_inits(struct gjrand *s, const unsigned char *buf, size_t len)
	GJRAND_THROW_NOTHING
{
    uint64_t x[4];
    size_t l=len;
    unsigned char b2[32];

    x[0]=0; x[1]=0; x[2]=0; x[3]=0;

    while (l>=32)
    {
	x[0]^=L64(buf); x[1]^=L64(buf+8); x[2]^=L64(buf+16); x[3]^=L64(buf+24);
	l-=32; buf+=32;
	s->a=x[0]; s->b=x[1]; s->c=x[2]; s->d=x[3];
	gjrand_inte_crank64(s, x);
    }
    if (l>0)
    {
	memset(b2, 0, 32);
	memcpy(b2, buf, l);
	x[0]^=L64(b2); x[1]^=L64(b2+8); x[2]^=L64(b2+16); x[3]^=L64(b2+24);
	s->a=x[0]; s->b=x[1]; s->c=x[2]; s->d=x[3];
	gjrand_inte_crank64(s, x);
    }
    x[0]^=len;
    s->a=x[0]; s->b=x[1]; s->c=x[2]; s->d=x[3];
    gjrand_inte_crank64(s, x);
    s->a=0; s->b=x[0]; s->c=x[1]; s->d=3000001ul;
}
