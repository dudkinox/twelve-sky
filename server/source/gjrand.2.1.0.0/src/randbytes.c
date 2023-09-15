/* Part of gjrand random number library version 1.0.0.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "gjrand.h"
#include "inte.h"
#include <string.h>

void
gjrand_randbytes(struct gjrand *s, int n, unsigned char *b)
	GJRAND_THROW_NOTHING
{
    uint64_t t[4];
    int j=n&31;

    if (j)
    {
	int k=j&7, i=0;
	n-=j;
	gjrand_inte_crank64(s, t);
	if (k)
	{
	    uint64_t v=t[0];
	    i++; j-=k;
	    do
	    {
		*b++ = v;
		v>>=8;
		k--;
	    } while (k);
	}

	while (j>0)
	{
	    uint64_t v=t[i++];
	    b[0]=v; b[1]=v>>8; b[2]=v>>16; b[3]=v>>24;
	    b[4]=v>>32; b[5]=v>>40; b[6]=v>>48; b[7]=v>>56;
	    b+=8; j-=8;
	}
    }

    while (n>0)
    {
	int i;
	gjrand_inte_crank64(s, t);
	for (i=3; i>=0; i--)
	{
	    uint64_t v=t[i];
	    b[0]=v; b[1]=v>>8; b[2]=v>>16; b[3]=v>>24;
	    b[4]=v>>32; b[5]=v>>40; b[6]=v>>48; b[7]=v>>56;
	    b+=8;
	}
	n-=32;
    }
}
