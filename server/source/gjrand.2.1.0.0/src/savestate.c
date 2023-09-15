/* Part of gjrand random number library version 1.0.1.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"
#include "inte.h"

void
gjrand_savestate(const struct gjrand *s, char *buf)
	GJRAND_THROW_NOTHING
{
    unsigned char *b=(unsigned char *)buf;
    gjrand_inte_s64(s->a, b+8);
    gjrand_inte_s64(s->b, b+16);
    gjrand_inte_s64(s->c, b+24);
    gjrand_inte_s64(s->d, b+32);
    gjrand_inte_hash32_8(b, b);
}
