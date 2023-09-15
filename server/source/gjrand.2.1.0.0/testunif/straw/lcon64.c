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

/* This is a linear congruential generator. */
/* These tend to be very weak in the least significant bits */
/* and reasonably good in the more significant bits. */
/* This one returns the most significant 32 bits out of 64. */
/* This is bordering on acceptable, but might still cause trouble in */
/* the most demanding applications. */

/* selfcor -r complains loudest, rejecting it somewhere less than 1GB. */
/* DIEHARD is happy. Neither gzip nor bzip2 can compress it. */

void
crash(char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

void
blat(int64_t s, uint32_t seed)
{
    uint32_t buf[1024];
    uint64_t j=seed;

    while (s)
    {
	unsigned c;
	int i= -1024;
	do
	{
	    j=j*3098765421UL+1;
	    (buf+1024)[i]=j>>32;
	    i++;
	} while (i<0);

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
    unsigned long seed;

    if (argc>2) sscanf(argv[2], "%lu", &seed);
    else seed=time(0)+getpid();
    if (argc<2) crash("give a size arg in bytes, or 'inf'");
    if (sscanf(argv[1], "%lf", &ds)==1) s=(int64_t)ds;
    else
    {
	if (strcmp(argv[1],"inf")) crash("not a number");
	s= -1;
    }

    blat(s, seed);

    return 0;
}
