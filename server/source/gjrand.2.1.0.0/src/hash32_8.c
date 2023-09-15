/* Part of gjrand random number library version 2.0.0.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"
#include "inte.h"

void gjrand_inte_hash32_8(const unsigned char in[40], unsigned char out[8])
	GJRAND_THROW_NOTHING
{
    uint32_t t;
    int i;

    /* Start with magic numbers including pseudo-version info in [1]. */
    /* The pseudo-version number is currently 2 and should change */
    /* whenever the save format does. */
    out[0]=242;
    out[1]=2;
    out[2]=gjrand_statesize();
    out[3]=0;

    t=1;
    for (i=8; i<40; i++) {t+=in[i]; t *= 987653421ul;}

    out[4]=t>>24;
    out[5]=t>>16;
    out[6]=t>>8;
    out[7]=t;
}
