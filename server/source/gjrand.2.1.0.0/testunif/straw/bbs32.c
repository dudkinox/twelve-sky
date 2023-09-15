/* Miscellaneous stuff for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* This one writes a file for use by the tests in testunif */

/* first arg is required file size in bytes or "inf" to write forever. */
/* second optional arg is random seed. */

/* This is (a very loose interpretation of) the Blum, Blum, Shub generator. */

/* THIS VERSION IS NOT DONE PROPERLY ! */
/* DO NOT USE IT FOR SECURITY RELATED PURPOSES ! */
/* The most obvious problem is that the modulus is far too small. */

/* This version has a further problem in that it tries for extra speed */
/* by revealing more bits of state than you're supposed to. */

/* This is slow, but not horribly so. It's comparable with glibc rand() */
/* function on my machine, and probably faster than inversive congruential */
/* generators, which have a following for some reason. */

/* Quality looks ok on pmcp --standard . */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "../../src/my_int_types.h"

void
crash(char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

#define MOD1 (4000000559ul)
#define MOD2 (4200000419ul)

static uint32_t state[2];

static void
bbsseed(uint32_t s)
{
    state[0] = s%(MOD1-3) + 2;
    state[1] = ((uint64_t)s)*1234567ul%(MOD2-3) + 2;
}

void
fill(uint32_t *buf)
{
    int i;
    uint32_t a=state[0], b=state[1];

    buf+=1024;

    for (i= -1024; i<0; i++)
    {
	a=(uint64_t)a*a%MOD1;
	b=(uint64_t)b*b%MOD2;

	/* The next line is totally bogus. */
	buf[i]=b*987653421UL+a;
    }

    state[0]=a; state[1]=b;
}

void
blat(int64_t s, uint32_t seed)
{
    uint32_t buf[1024];

    bbsseed(seed);

    while (s)
    {
	size_t c;

	fill(buf);

	if ((uint64_t)s>4096) c=4096; else c=s;
	if (fwrite(buf, 1, c, stdout)!=c) crash("write fails");

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
