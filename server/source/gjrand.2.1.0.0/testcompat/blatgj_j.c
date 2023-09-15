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

/* This one writes a file of random bits. It is much the same as blatrand */
/* but uses gjrand_jrand48() to generate the numbers. */
/* The output can be tested using the programs in testunif. */

/* first arg is required file size in bytes or "inf" to write forever. */
/* next 3 args is random seed. */

void
crash(char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

void
blat(unsigned short xsubi[3], int64_t s)
{
    uint32_t buf[1024];

    while (s)
    {
	int c;
	int i=1023;
	do
	{
	    buf[i]=gjrand_jrand48(xsubi);
	    i--;
	} while (i>=0);

	if ((uint64_t)s>4096) c=4096; else c=s;
	if (write(1, buf, c)!=c) crash("write fails");

	if (s>=0) s-=c;
    }
}

int
main(int argc, char **argv)
{
    double ds;
    int64_t s= -1;
    unsigned short xsubi[3];

    if (argc!=5 && argc!=2) crash("give 4 args: size seed0 seed1 seed2\n"
				  "   or 1 arg: size\n"
			"size is required file size in bytes, or inf");

    if (argc==5)
    {
	if (sscanf(argv[2], "%hd", xsubi)!=1) crash("seed0 not numeric");
	if (sscanf(argv[3], "%hd", xsubi+1)!=1) crash("seed1 not numeric");
	if (sscanf(argv[4], "%hd", xsubi+2)!=1) crash("seed2 not numeric");
    }
    else
    {
	uint32_t t=time(0);

	xsubi[0]=t; xsubi[1]=t>>16; xsubi[2]=getpid();
    }

    if (strcmp(argv[1],"inf")==0) s= -1;
    else if (sscanf(argv[1], "%lf", &ds)==1) s=(int64_t)ds;
    else crash("not a number");

    blat(xsubi, s);

    return 0;
}
