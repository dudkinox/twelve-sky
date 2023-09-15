/* Miscellaneous stuff for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "../../src/my_int_types.h"

/* This one writes a file for use by the tests in testunif */

/* first arg is required file size in bytes or "inf" to write forever. */
/* second optional arg is random seed. */

/* This one is based on the least significant 8 bits of the rand() */
/* library function supplied with your system. Some older rand() */
/* gave only 15 bits result, so 8 bits is the most that can conveniently */
/* be used here. Most recent rand() functions give 31 bits. */
/* In some old rand() implementations, the least significant bits are */
/* extremely bad. However, some newer ones are mostly ok. */

/* Quality will vary enormously depending on your C library. */

void
crash(char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

void
blat(int64_t s)
{
    char buf[4096];

    while (s)
    {
	size_t c;
	int i=4095;
	do
	{
	    buf[i]=rand();
	    i--;
	} while (i>=0);

	if ((uint64_t)s>4096) c=4096; else c=s;
	if (fwrite(buf, 1, c, stdout)!=c) crash("fwrite fails");

	if (s>=0) s-=c;
    }
}

int
main(int argc, char **argv)
{
    double ds;
    int64_t s;
    unsigned int seed;

    if (argc>2) sscanf(argv[2], "%u", &seed);
    else seed=time(0)+getpid();
    if (argc<2) crash("give a size arg in bytes, or 'inf'");
    if (sscanf(argv[1], "%lf", &ds)==1) s=(int64_t)ds;
    else
    {
	if (strcmp(argv[1],"inf")) crash("not a number");
	s= -1;
    }

    srand(seed);

    blat(s);

    return 0;
}
