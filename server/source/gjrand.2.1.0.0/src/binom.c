/* Part of gjrand random number library version 2.0.0.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"
#include "inte.h"
#include <math.h>

/* throw n biased coins with probability prob of heads. */
int
gjrand_inte_binom(struct gjrand *state, double prob, int n)
	GJRAND_THROW_NOTHING
{
    int x, lo, hi, from0=0;
    double px, plo, phi, p, tiny, t;

    p = gjrand_drand(state);

    t = prob*n;

    x = (int)floor(t + 0.5);
    if (x <= 1)
    {
	if (p+t < 1.0) return 0;
	x = 0; px = log1p(-prob) * n; from0 = 1;
    }
    else px = gjrand_inte_logfac(n)
	-gjrand_inte_logfac(x) - gjrand_inte_logfac(n-x)
	+ log1p(-prob)*(n-x) + log(prob)*x;

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
	plo *= (1.0-prob) / prob / (n-lo);
	p -= plo;
	if (p <= 0.0) return lo;

	phi *= n-hi;
	hi++;
	if (hi > n) goto loonly;
	phi *= prob / (1.0-prob) / hi;
	p -= phi;
	if (p <= 0.0) return hi;

	if (phi+plo < tiny) {if (phi>plo) return hi; return lo;}
    }

    loonly:
    while (1)
    {
	plo *= lo;
	lo--;
	if (lo < 0) return 0;
	plo *= (1.0-prob) / prob / (n-lo);
	p -= plo;
	if (p <= 0.0) return lo;
	if (plo < tiny) return lo;
    }

    hionly:
    while (1)
    {
	phi *= n-hi;
	hi++;
	if (hi > n) return n;
	phi *= prob / (1.0-prob) / hi;
	p -= phi;
	if (p <= 0.0) return hi;
	if (phi < tiny) return hi;
    }
}
