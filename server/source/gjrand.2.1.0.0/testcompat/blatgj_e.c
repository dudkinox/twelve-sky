/* Test software for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "../src/gjrand.h"

/* This one writes a file of random binary doubles on [0,1). It uses */
/* gjrand_erand48() to generate the numbers. */
/* The output can be tested using the programs in testfunif. */

/* 3 optional args are random seed. */

/* It attempts to write forever. */

void
crash(char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

void
blat(unsigned short xsubi[3])
{
    double buf[1024];

    while (1)
    {
	int i;

	for (i=0; i<1024; i++)
	    buf[i]=gjrand_erand48(xsubi);

	if (fwrite(buf, sizeof(double), 1024, stdout)!=1024) break;
    }
}

int
main(int argc, char **argv)
{
    unsigned short xsubi[3];

    if (argc!=4 && argc!=1) crash("give 3 args: seed0 seed1 seed2\n"
				  "   or no arg\n");

    if (argc==4)
    {
	if (sscanf(argv[1], "%hd", xsubi)!=1) crash("seed0 not numeric");
	if (sscanf(argv[2], "%hd", xsubi+1)!=1) crash("seed1 not numeric");
	if (sscanf(argv[3], "%hd", xsubi+2)!=1) crash("seed2 not numeric");
    }
    else
    {
	uint32_t t=time(0);

	xsubi[0]=t; xsubi[1]=t>>16; xsubi[2]=getpid();
    }

    blat(xsubi);

    return 0;
}
