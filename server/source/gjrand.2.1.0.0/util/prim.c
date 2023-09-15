/* Miscellaneous stuff for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* This finds primitive polynomials over GF2. These are allegedly useful */
/* for making some kinds of random number generators. */

#include <stdlib.h>
#include <stdio.h>

#include "../src/my_int_types.h"

typedef uint32_t u;

void
crash(char *s)
{
    fprintf(stderr,"crash [%s]\n", s);
    exit(1);
}

/* remainder of x/y ; ox, oy are polynomial orders */
int
rem(u x, int ox, u y, int oy)
{
    int i;

    for (i=ox; i>=oy; i--) if ((1<<i)&x) x^=y<<(i-oy);

    return x;
}

int
test2(u x, int ox)
{
    u mask=1<<ox;
    u y=1;
    int i=0;

    while (1)
    {
	i++;
	y<<=1;
	if (y&mask) y^=x;
	if (y==1)
	{
	    printf("cycle %d\n", i);
	    return (i==mask-1);
	}
    }
}

int
test(u x, int ox)
{
    u y, hy;
    int oy;

    hy=2;
    for (oy=1; oy<=8; oy++)
    {
	for (y=hy, hy+=hy; y<hy; y++) if (rem(x, ox, y, oy)==0) return 0;
    }

    return 1;
}

void
printp(u x, int ox)
{
    int i;

    printf("( ");
    for (i=ox; i>=0; i--)
	if ((1<<i)&x) printf("%d ", i);
    printf(")\n");
}

int
main(int argc, char **argv)
{
    u x;
    int found=0, found2=0;

    for (x=(1<<16)+1; x<(1<<17) /* && found<40 */ ; x+=2)
	if (test(x,16))
	{
	    found++;
	    if (test2(x, 16)) {found2++; printf("PRIMITIVE:\n");}
	    printp(x,16);

	}

    fprintf(stderr, "found %d irreducable, %d primitive\n", found, found2);

    return 0;
}
