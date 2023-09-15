/* Part of gjrand random number library version 2.0.0.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"

/* This makes some assumptions that you have something quite similar */
/* to IEEE single precision for float. If float has less precision than */
/* that you will get 1.0 returned occasionally. */

/* Results have 24 bits absolute. */

/* Leave GJRAND_FAST_387 undefined for fairly portable code with a minimum */
/* of assumptions. */
/* Define it to get something that was tested only on 387 compatibles. */
/* There's probably quite a few other IEEE compatible systems it also */
/* works on, but if you enable it, test carefully before you trust it. */
/* It is certainly possible to build a fully IEEE compatible system */
/* which this trick doesn't work for. I don't know of such a system, */
/* but i wouldn't be surprised if they are quite common. */
/* In addition, sizeof(float) must equal sizeof(uint32_t). */
/* testother/fv should tell the story. */
/* If it works it's probably faster, but not enormously so. */
/* testmisc/timefv is a speed test. */

void
gjrand_frandv(struct gjrand *s, int n, float *v)
	GJRAND_THROW_NOTHING
{
    uint32_t b[8];

#define Q (1.0/4.0)
#define S8 (Q*Q*Q*Q)
#define S24 (S8*S8*S8)

    while (n>0)
    {
	int i;

	gjrand_rand32v(s, 8, b);

	i=n-1; if (i>7) i=7;

	do
	{
	    uint32_t j=b[i];
#if defined(GJRAND_FAST_387) && GJRAND_FAST_387
	    static const float adj[2]={-0.5, 0.0};
	    /* The point of the "union" is to try to convince the */
	    /* compiler that the following two lines must be done in the */
	    /* correct order, even through they affect operands of */
	    /* different types, and therefore by standard C shouldn't alias. */
	    /* I previously used "volatile" for this, but maybe "union" */
	    /* gets better code on some machines? */
	    union {uint32_t i; float f;} u;
	    u.i = (j&0x7fffff) | 0x3f000000;
	    v[i] = adj[(j>>23)&1] + u.f;

#else /* not GJRAND_FAST_387 */
	    v[i] = (j&0xffffff)*S24;
#endif /* GJRAND_FAST_387 */
	    i--;
	} while (i>=0);

	v+=8;
	n-=8;
    }
}
