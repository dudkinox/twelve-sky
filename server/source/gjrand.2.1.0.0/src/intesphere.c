/* Part of gjrand random number library version 2.0.0.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <math.h>
#include "gjrand.h"
#include "inte.h"

/* Seen in HAKMEM. */
/* This version is workable for fairly large n, unlike the old version. */

#define SCALE (1.0/(((uint64_t)1)<<31))
#define TINY (SCALE*SCALE*SCALE*SCALE)

void
gjrand_inte_sphere(struct gjrand *s, int n, double *a, double r)
	GJRAND_THROW_NOTHING
{
    double u;
    int j;

    if (n<=0) return;

    gjrand_normalv(s, n, a);
    u=TINY;
    j=n-1;
    do
    {
	double t=a[j];
	u+=t*t;
	j--;
    } while (j>=0);

    u=r/sqrt(u);

    j=n-1;
    do
    {
	a[j] *= u;
	j--;
    } while (j>=0);
}
