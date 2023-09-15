/* Miscellaneous stuff for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* Experiments with negative denormals. */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void
dotest(void)
{
    volatile float x, y;
    int i;

    x= -1.5;
    i=0;
    while (x<0.0)
    {
	y=x;
	x *= 0.5;
	i++;
    }
    printf("iter = %d   delta = %e\n", i, y);

    if (i>122 && i<132) printf("Negative denormals probably are zero.\n");
    else if (i>147 && i<157)
	printf("Negative denormals probably are negative.\n");
    else printf("Strange format for float. ???\n");
}

int
main(int argc, char **argv)
{
    dotest();
    return 0;
}
