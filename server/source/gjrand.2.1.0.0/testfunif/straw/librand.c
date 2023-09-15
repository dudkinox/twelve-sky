/* Miscellaneous stuff for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/* This one writes a file for use by the tests in testfunif */

/* optional arg is random seed. */

/* This one uses one call to your C library's rand(). */
/* Quality of these varies a lot from library to library. */
/* Usually rand() won't make enough bits to make chi happy. */

#define MULT (1.0/(RAND_MAX+1.0))

static void
blat(unsigned long seed)
{
    double buf[1024];

    srand(seed);

    while (1)
    {
	int i;

	for (i=0; i<1024; i++) buf[i]=rand()*MULT;

	if (fwrite(buf, sizeof(double), 1024, stdout)!=1024) break;
    }
}

int
main(int argc, char **argv)
{
    unsigned long t;

    if (argc>1) sscanf(argv[1], "%lu", &t);
    else t=time(0);

    blat(t);

    return 0;
}
