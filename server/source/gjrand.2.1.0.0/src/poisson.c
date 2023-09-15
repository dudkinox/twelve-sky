/* Part of gjrand random number library version 2.0.0.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"
#include "inte.h"
#include <math.h>

int
gjrand_poisson(struct gjrand *state, double t) GJRAND_THROW_NOTHING
{
    int x, lo, hi, from0=0;
    double px, plo, phi, p, tiny;

    p = gjrand_drand(state);

    x = (int)floor(t + 0.5);
    if (x <= 1)
    {
	if (p+t < 1.0) return 0;
	x = 0; px = -t; from0 = 1;
    }
    else px = log(t)*x - gjrand_inte_logfac(x) - t;

    px = exp(px);

    p -= px;
    if (p <= 0.0) return x;

    tiny = px * 1.0e-11;
    hi = x; phi = px;
    if (from0) goto hionly;
    lo = x; plo = px;

    while (1)
    {
	plo *= lo;
	lo--;
	if (lo < 0) goto hionly;
	plo *= 1.0/t;
	p -= plo;
	if (p <= 0.0) return lo;

	hi++;
	phi *= t/hi;
	p -= phi;
	if (p <= 0.0) return hi;

	if (phi+plo < tiny) {if (phi>plo) return hi; return lo;}
    }

    hionly:
    while (1)
    {
	hi++;
	phi *= t/hi;
	p -= phi;
	if (p <= 0.0) return hi;
	if (phi < tiny) return hi;
    }
}
