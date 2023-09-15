/* Part of gjrand random number library version 1.2.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <math.h>
#include "gjrand.h"
#include "inte.h"
#include "my_int_types.h"

#define SCALE (1.0/(((uint64_t)1)<<31))
#define TINY (SCALE*SCALE*SCALE*SCALE)

void
gjrand_normalv(struct gjrand *s, int n, double *v)
	GJRAND_THROW_NOTHING
{
    int32_t a[8], *p;

    if (n>0) while (1)
    {
	gjrand_rand32v(s, 8, (uint32_t *)a);
	p=a+6;
	do
	{
	    if ((uint64_t)((int64_t)p[1]*p[1])+(uint64_t)((int64_t)p[0]*p[0])
		< (((uint64_t)1)<<62))
	    {
		double x=p[1]*SCALE, y=p[0]*SCALE;
		double w = x*x + y*y + TINY;
		w = sqrt(log(w) / w * -2.0);
		*v = x*w; v++;
		n--; if (n<=0) return;
		*v = y*w; v++;
		n--; if (n<=0) return;
	    }
	    p-=2;
	} while (p>=a);
    }
}
