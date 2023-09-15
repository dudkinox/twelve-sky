/* Part of gjrand random number library version 1.0.0.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"
#include "inte.h"

unsigned short int *
gjrand_seed48(unsigned short int seed16v[3])
	GJRAND_THROW_NOTHING
{
    gjrand_inte_rand48_state[0]=seed16v[0];
    gjrand_inte_rand48_state[1]=seed16v[1];
    gjrand_inte_rand48_state[2]=seed16v[2];
    gjrand_inte_rand48_state[3]=0;
    gjrand_inte_rand48_state[4]=0;
    gjrand_inte_rand48_state[5]=0;
    gjrand_inte_rand48_state[6]=0;

    return gjrand_inte_rand48_state;
}
