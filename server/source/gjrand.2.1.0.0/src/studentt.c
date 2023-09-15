/* Part of gjrand random number library version 1.2.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <math.h>
#include "gjrand.h"
#include "inte.h"
#include "my_int_types.h"

/* Somewhere in here the Polar Box-Muller for normal distribution */
/* is hiding. */

/* Other methods may be faster for large f, but so far i don't know them. */

#define SCALE (1.0/(((uint64_t)1)<<31))
#define TINY (SCALE*SCALE*SCALE*SCALE)

double
gjrand_t(struct gjrand *s, int f)
	GJRAND_THROW_NOTHING
{
    int32_t a[8], *p, n;
    double sum=0.0, ss=0.0, mean, var, r;

    if (f<=0) return 0.0;

    n=f+1;

    while (1)
    {
	gjrand_rand32v(s, 8, (uint32_t *)a);
	p=a+6;
	do
	{
	    if ((uint64_t)((int64_t)p[1]*p[1])+(uint64_t)((int64_t)p[0]*p[0])
		< (((uint64_t)1)<<62))
	    {
		double x=p[1]*SCALE, y=p[0]*SCALE, w = x*x + y*y + TINY;

		w = sqrt(log(w) / w * -2.0);
		x *= w;
		sum += x;
		ss += x*x;
		n--; if (n<=0) goto done;
		y *= w;
		sum += y;
		ss += y*y;
		n--; if (n<=0) goto done;
	    }
	    p-=2;
	} while (p>=a);
    }

    done:

    n=f+1;

    mean = sum/n;
    var = (ss - sum*mean)/f;
    if (var<TINY) var=TINY;

    r = mean / sqrt(var / n);

    return r;
}
