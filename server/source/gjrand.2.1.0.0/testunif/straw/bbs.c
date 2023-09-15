/* Miscellaneous stuff for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* This one writes a file for use by the tests in testunif */

/* first arg is required file size in bytes or "inf" to write forever. */
/* second optional arg is random seed. */

/* This is the Blum, Blum, Shub generator. If done properly with a very */
/* large well-chosen modulus, this is supposed to be a "secure" random */
/* number generator, but then it is hideously slow. This version has a */
/* much smaller modulus to see if it can go a bit faster, and if it might */
/* then still be ok for non-secure purposes. */

/* It is still slower than most people would want. Quality appears ok, */
/* but i don't have the patience to try it on a really big data set. */

/* THIS VERSION IS NOT DONE PROPERLY ! */
/* DO NOT USE IT FOR SECURITY RELATED PURPOSES ! */
/* The most obvious problem is that the modulus is far too small. */
/* But there are other problems too. */


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

/* return a 4 bit pseudo-random number */
/* It's a good idea to seed state[] with numbers */
/* in the range 2 <= x <= MOD-2 */
/* before you call bbsrand the first time. */
static unsigned
bbsrand(void)
{
    uint32_t a=state[0], b=state[1];

    a=(uint64_t)a*a%MOD1; state[0]=a;
    b=(uint64_t)b*b%MOD2; state[1]=b;

    /* Now, actually, for a strictly correct implementation */
    /* a and b have to be multiplied by mysterious constants */
    /* at this stage. But i'm too lazy to figure out what */
    /* the correct constants are. And i'm pretty sure the security */
    /* and random properties are just as good without doing */
    /* the multiplications. But i'm too lazy to prove that too. */

    return (a+b)&0xF;
}

static void
bbsseed(uint32_t s)
{
    state[0] = s%(MOD1-3) + 2;
    state[1] = ((uint64_t)(s+999))*1234567ul%(MOD2-3) + 2;
}

void
fill(unsigned char *buf)
{
    int i;
    unsigned j;

    buf+=4096;

    for (i= -4096; i<0; i++)
    {
	j=bbsrand()<<4;
	j|=bbsrand();
	buf[i]=j;
    }
}

void
blat(int64_t s, uint32_t seed)
{
    unsigned char buf[4096];

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
