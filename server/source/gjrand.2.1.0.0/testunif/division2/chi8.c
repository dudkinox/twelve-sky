/* Part of gjrand random number library version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../../src/my_int_types.h"

/* chi square test on byte values. */
/* actually this one is occasionally good, and it is quick. */
/* it got relegated because metachi now does this test. */

int64_t count[256];

void
doann(int64_t pos)
{
    double e=(double)pos/256.0, t=0.0, x;
    int i;

    for (i=0; i<256; i++)
    {
	x=(double)count[i]-e;
	t+=x*x/e;
    }

    printf("%20.10f\n", t);
}

void
readstuff(int64_t max)
{
    unsigned char buf[4096];
    int64_t pos=0;
    int i;

    for (i=0; i<256; i++) count[i]=0;

    while (1)
    {
	i=fread(buf, 1, 4096, stdin);
	if (i<=0) break;
	if (max>0 && max<pos+i) i=max-pos;
	pos+=i;
	i--;
	do {count[buf[i]]++; i--;} while (i>=0);
	if (max>0 && pos>=max) break;
    }

    if (max>0 && pos<max)
	fprintf(stderr, "Warning, expected %.0f samples, saw only %.0f\n",
		(double)max, (double)pos);

    if (pos<2560)
	fprintf(stderr, "Warning, < 2560 samples, don't trust result.\n");

    doann(pos);
}

int
main(int argc, char **argv)
{
    double dmax;
    int64_t max;

    if (argc<2 || sscanf(argv[1], "%lf", &dmax)!=1 || dmax<1.0) max= -1;
    else max=dmax;

    printf("expected range [ 191 205 227 284 310 331 ]\n");

    readstuff(max);

    return 0;
}
