/* Part of gjrand random number library version 1.3.3.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"

void
gjrand_shuffle(struct gjrand *s, int n, int *a)
	GJRAND_THROW_NOTHING
{
    uint32_t b[8];
    uint32_t t, big;
    unsigned nu=n;
    int i, j;

    if (n<=0) return;

    for (i=0; i<n; i++) a[i]=i;

    i=0;

    while (nu>0)
    {
	do
	{
	    i--;
	    if (i<0) {gjrand_rand32v(s, 8, b); i=7;}
	    big = 0xffffffffUL - (0xffffffffUL%nu);
	    t=b[i];
	} while (t>=big);

	t %= nu;

	j= *a; *a=a[t]; a[t]=j;

	a++; nu--;
    }
}
