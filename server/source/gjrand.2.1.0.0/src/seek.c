/* Part of gjrand random number library version 1.0.1.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdio.h>
#include "gjrand.h"
#include "inte.h"

int64_t
gjrand_seek(struct gjrand *state, int64_t offset, int whence)
	GJRAND_THROW_NOTHING
{
    uint64_t t=state->a;

    if (whence!=SEEK_CUR) t=0;
    t += ((uint64_t)offset)*GJRAND_INTE_INCR;
    state->a=t;
    t *= GJRAND_INTE_INCR_INV;

    return (int64_t)t;
}
