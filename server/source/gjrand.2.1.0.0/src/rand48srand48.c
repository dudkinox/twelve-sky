/* Part of gjrand random number library version 1.0.0.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"
#include "inte.h"

void
gjrand_srand48(long int seedval)
	GJRAND_THROW_NOTHING
{
    gjrand_inte_rand48_state[0]=0;
    gjrand_inte_rand48_state[1]=(unsigned short)seedval;
    gjrand_inte_rand48_state[2]=(unsigned short)(seedval>>16);
    gjrand_inte_rand48_state[3]=0;
    gjrand_inte_rand48_state[4]=0;
    gjrand_inte_rand48_state[5]=0;
    gjrand_inte_rand48_state[6]=0;
}
