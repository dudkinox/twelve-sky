/* Part of gjrand random number library version 1.0.1.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"
#include "inte.h"

uint64_t
gjrand_inte_l64(const unsigned char *b) GJRAND_THROW_NOTHING
{
    return
	( ((uint64_t)b[0]) << 56 )
	| ( ((uint64_t)b[1]) << 48 )
	| ( ((uint64_t)b[2]) << 40 )
	| ( ((uint64_t)b[3]) << 32 )
	| ( ((uint64_t)b[4]) << 24 )
	| ( ((uint64_t)b[5]) << 16 )
	| ( ((uint64_t)b[6]) << 8 )
	| ((uint64_t)b[7]);
}
