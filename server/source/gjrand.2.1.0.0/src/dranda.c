/* Part of gjrand random number library version 1.0.0.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"
#include "inte.h"

void
gjrand_drand_array(struct gjrand *s, int n, double *a)
	GJRAND_THROW_NOTHING
{
    int64_t b[4];
    int i;

    while (n>0)
    {
	gjrand_inte_crank64(s, (uint64_t *)b);

#define Q (1.0/4.0)
#define S8 (Q*Q*Q*Q)
#define S32 (S8*S8*S8*S8)
#define S64 (S32*S32)

	i=n-1; if (i>3) i=3;
	do
	{
	    a[i] = ((double)(b[i]))*(2.0*S64);
	    i--;
	} while (i>=0);

	a+=4;
	n-=4;
    }
}
