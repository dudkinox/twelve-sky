/* Test software for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include "../src/gjrand.h"

/* This one writes a file of random bits. It is much the same as blatrand */
/* but uses mrand48() to generate the numbers. */
/* The output can be tested using the programs in testunif. */

/* first arg is required file size in bytes or "inf" to write forever. */
/* additional 1, 3, or 7 args are for seeds. */

void
crash(char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

void
blat(int64_t s)
{
    uint32_t buf[1024];

    while (s)
    {
	int c;
	int i=1023;
	do
	{
	    buf[i]=mrand48();
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
    unsigned short xsubi[7];
    long int seedval;

    if (argc!=3 && argc!=5 && argc!=9)
crash("give 2, 4 or 8  args:\n"
"blatm size bigseed\n"
"blatm size seed0 seed1 seed2\n"
"blatm size seed0 seed1 seed2 a0 a1 a2 c \n"
"size is required file size in bytes, or inf");

    if (argc==3)
    {
	if (sscanf(argv[2], "%ld", &seedval)!=1) crash("bigseed not numeric");
	srand48(seedval);
    }
    else
    {
	if (sscanf(argv[2], "%hu", xsubi)!=1) crash("seed0 not numeric");
	if (sscanf(argv[3], "%hu", xsubi+1)!=1) crash("seed1 not numeric");
	if (sscanf(argv[4], "%hu", xsubi+2)!=1) crash("seed2 not numeric");

	if (argc==9)
	{
	    if (sscanf(argv[5], "%hu", xsubi+3)!=1) crash("a0 not numeric");
	    if (sscanf(argv[6], "%hu", xsubi+4)!=1) crash("a1 not numeric");
	    if (sscanf(argv[7], "%hu", xsubi+5)!=1) crash("a2 not numeric");
	    if (sscanf(argv[8], "%hu", xsubi+6)!=1) crash("c not numeric");
	    lcong48(xsubi);
	}
	else (void)seed48(xsubi);
    }

    if (strcmp(argv[1],"inf")==0) s= -1;
    else if (sscanf(argv[1], "%lf", &ds)==1) s=(int64_t)ds;
    else crash("not a number");

    blat(s);

    return 0;
}
