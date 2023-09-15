/* Part of gjrand random number library version 1.0.0.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"
#include "inte.h"

int
gjrand_restorestate(struct gjrand *s, const char *buf)
	GJRAND_THROW_NOTHING
{
    const unsigned char *b=(const unsigned char *)buf;
    unsigned char h[8];
    int i;

    gjrand_inte_hash32_8(b, h);
    for (i=0; i<8; i++) if (b[i]!=h[i]) return -1;

    s->a=gjrand_inte_l64(b+8);
    s->b=gjrand_inte_l64(b+16);
    s->c=gjrand_inte_l64(b+24);
    s->d=gjrand_inte_l64(b+32);

    return 0;
}
