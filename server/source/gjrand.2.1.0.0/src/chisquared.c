/* Part of gjrand random number library version 2.1.0.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <math.h>
#include "gjrand.h"
#include "inte.h"
#include "my_int_types.h"

/* Somewhere in here the Polar Box-Muller for normal distribution */
/* is hiding, and we just sum the squares of f normal numbers. */
/* Then it got optimised. All the sqrt() in PBM can be removed because */
/* we are squaring again. And most of the log() can be removed because */
/* sum of the logs is log of the products. (On most processors sqrt() */
/* is slightly slower than simple arithmetic, and log() is much slower, */
/* so this mess really does go faster.) */

/* Other methods may be faster for large f, but so far i don't know them. */

/* used in desaturation to prevent overflow */
#define BIG (256.0)
/* e to the power BIG */
#define EXPBIG (1.5114276650041035425763e+111)

#define SCALE62 (1.0/(1L<<30)/(1L<<30)/(1L<<2))

double
gjrand_chisquared(struct gjrand *s, int f)
	GJRAND_THROW_NOTHING
{
    double r=0.0;

    if (f&1)
    {
	uint64_t wu, ru;

	while (1)
	{
	    int32_t a[2];

	    gjrand_rand32v(s, 2, (uint32_t *)a);
	    ru = (int64_t)a[1]*a[1];
	    wu = (int64_t)a[0]*a[0] + ru;
	    if (wu < (((uint64_t)1)<<62)) break;
	}
	r = wu+0.5;
	r = log(r*SCALE62)/r * ru;
	f--;
    }

    if (f>0)
    {
	double w = 1.0;

	f >>= 1;

	while (1)
	{
	    double b[4];
	    int j;

	    j = f; if (j>4) j = 4;
	    gjrand_drandv(s, j, b);
	    j--;
	    do
	    {
		w *= b[j];
		j--;
	    } while (j>=0);
	    if (w < 1.0/EXPBIG) {w *= EXPBIG*EXPBIG; r -= BIG+BIG;}
	    f -= 4; if (f<=0) break;
	}
	r += log(w+1.0/EXPBIG);
    }

    return r * -2.0;
}
