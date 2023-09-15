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

/* A small linear feadback shift register with some massaging of the output. */

/* The polynomial is x**57 + x**32 + 1 . It is irreducable and therefore has */
/* a fixed period 20587884010836553 (according to Carlo Wood), but it is not */
/* primitive. There are apparently 7 different cycles of this length, plus */
/* the trivial cycle of length 1. */
/* This method is simple, compact, fast, allows random access seeking to a */
/* point in the stream with reasonable effort (not demonstrated here). It */
/* passes pmcp --big and should be of satisfactory quality for many */
/* purposes, however, i don't entirely trust it for the most demanding */
/* non-crypto applications. Use with care. */

/* I think this approach shows promise for an industrial strength generator. */
/* To build such, a larger polynomial is needed. x**217 + x**64 + 1 might */
/* be suitable, with the post-processing done with 64 bit operations. */
/* But perhaps a pentanomial would be prefered for faster diffusion, at some */
/* expense of speed. */

static void
crash(char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

static uint32_t state[2];

static uint32_t
xrand(void)
{
    uint32_t d = state[0], a=state[1], b, c;

    state[0] = a;
    b = (d<<7) ^ (a>>25) ^ a;
    state[1]=b;

    c = a+b; d = 2*a+b+987654321;
    c *= 9; d = (d<<16)|(d>>16);
    a = c+d; b = 2*c+d+987654321;
    a *= 9; b = (b<<12)|(b>>20);

    return a^b;
}

static void
fillxrand(uint32_t *buf)
{
    int i = -1024;

    buf+=1024;
    do
    {
	buf[i] = xrand();
	i++;
    } while (i<0);
}

static void
blat(int64_t s, uint32_t seed)
{
    uint32_t buf[1024];
    int i;

    state[0] = -(42ul);
    state[1] = seed;
    for (i=99; i>=0; i--) (void)xrand();

    while (s)
    {
	size_t c;

	fillxrand(buf);

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
