/* Part of gjrand random number library version 1.0.0.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"
#include "inte.h"

void
gjrand_lcong48(unsigned short int seed16v[7])
	GJRAND_THROW_NOTHING
{
    gjrand_inte_rand48_state[0]=seed16v[0];
    gjrand_inte_rand48_state[1]=seed16v[1];
    gjrand_inte_rand48_state[2]=seed16v[2];
    gjrand_inte_rand48_state[3]=seed16v[3];
    gjrand_inte_rand48_state[4]=seed16v[4];
    gjrand_inte_rand48_state[5]=seed16v[5];
    gjrand_inte_rand48_state[6]=seed16v[6];
}
