/* Part of gjrand random number library version 2.0.0.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"
#include "inte.h"
#include <math.h>

#define SMALLBUF 64

/* A linear time algorithm that mostly avoids floating point. */
static int
biased_coins_new(struct gjrand *s, double prob, int n)
	GJRAND_THROW_NOTHING
{
    double x;
    uint32_t b[SMALLBUF], t;
    int r, i;

    prob*=(65536.0*65536.0);
    x=floor(prob);
    t=(uint32_t)x;
    prob-=x;
    prob*=(65536.0*65536.0);

    gjrand_rand32v(s, n+1, b);

    if (b[n]<prob)
    {
	t++;
	if (t==0) return n; /* overflow, prob is effectively 1.0 */
    }

    if (t==0) return 0;

    r = 0; i = 0;

    if ((n&1)!=0) {n--; r += b[n]<t;}

    while (n>0)
    {
	n -= 2;
	i += b[n+1]<t;
	r += b[n]<t;
    }

    return r+i;
}

static
int biased_coins_2(struct gjrand *s, double prob, int n)
{
    if (prob <= 0.0) return 0;
    if (n <= 0) return 0;
    if (n<SMALLBUF && prob*n>0.125) return biased_coins_new(s, prob, n);
    return gjrand_inte_binom(s, prob, n);
}

int
gjrand_biased_coins(struct gjrand *s, double prob, int n)
	GJRAND_THROW_NOTHING
{
    if (prob <= 0.5) return biased_coins_2(s, prob, n);
    return n-biased_coins_2(s, 1.0-prob, n);
}
