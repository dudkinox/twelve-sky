/* Part of gjrand random number library version 1.0.1.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"
#include "inte.h"

void
gjrand_inte_s64(const uint64_t i, unsigned char *b) GJRAND_THROW_NOTHING
{
    b[0]=i>>56;
    b[1]=i>>48;
    b[2]=i>>40;
    b[3]=i>>32;
    b[4]=i>>24;
    b[5]=i>>16;
    b[6]=i>>8;
    b[7]=i;
}
