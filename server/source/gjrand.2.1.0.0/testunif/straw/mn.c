/* Miscellaneous stuff for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* This one writes a file for use by the tests in testunif */

/* first arg is required file size in bytes or "inf" to write forever. */
/* second optional arg is random seed. */

/* Maurer's Nightmare. */

/* This one is specially constructed to fool Maurer's Universal Statistical */
/* Test on 8 bit words, while being obviously non-random to almost */
/* anything else. MUST is quite a good test. The point of this is to */
/* remind people that no single test can prove randomness. The best test is */
/* said to be one that you build specially which encapsulates the actual */
/* properties of randomness needed for your application. If that's too hard, */
/* try several different reputable tests, and hope. */

/* maurer7 is not fooled by this, and any other word size would not be */
/* either. The program could be reworked to fool MUST for any single */
/* word size. Fooling MUST with several different words sizes simultaneously */
/* would be much more complex, but possible. */

/* Trouble is seen by almost everything except maurer8 on quite small files. */
/* This doesn't actually resemble random numbers in any obvious way. */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include "../../src/my_int_types.h"

int64_t last[256];

#if defined(__cplusplus) || defined(__GNUC__)
#define INLINE inline
#else
#define INLINE
#endif

#ifdef __cplusplus
#define NOARGS
#else
#define NOARGS void
#endif

void
crash(char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

/* We do a lot of calls to log() which can be quite slow. */
/* These are always on integer values and usually on fairly small ones */
/* so put the most common ones in a table, saving both log() and */
/* convert to double. */

#define MAXLOGCACHE 4000
double mcache[MAXLOGCACHE];
double invlog2;

#define CM 7.183665553492224909

void
initlogcache(NOARGS)
{
    int i;
    invlog2=1.0/log(2.0);
    for (i=0; i<MAXLOGCACHE; i++) mcache[i]=(log((double)i)*invlog2)-CM;
}

INLINE double
fastmlog(int64_t i)
{
    if ((uint64_t)i<MAXLOGCACHE) return mcache[i];
    return (log((double)i)*invlog2)-CM;
}

void
initlast(NOARGS)
{
    int i;
    for (i=0; i<256; i++) last[i]= -162;
}

#define MBUFSIZ 256
#define MBUFMASK (MBUFSIZ-1)

unsigned char buf1[MBUFSIZ], buf2[MBUFSIZ];

void
initbuf(NOARGS)
{
    int i;

    for (i=0; i<MBUFSIZ; i++) buf1[i]=i&255;
    for (i=0; i<MBUFSIZ*200/256; i++) buf2[i]=i&255;
    for ( ; i<MBUFSIZ; i++) buf2[i]=0;
}

int64_t pos=0;
double must=0.0;
double bias;

void
dobuf(unsigned char buf[MBUFSIZ], int c)
{
    int i, ch;
    int64_t gap;

    if (fwrite(buf, 1, c, stdout)!=(size_t)c) crash("fwrite");

    for (i=0; i<c; i++)
    {
	ch=buf[i];
	gap=pos-last[ch];
	last[ch]=pos;
	must+=fastmlog(gap);
	pos++;
    }
}

/* Determined emperically, not very accurate. */
#define CSD 1.106

void
blat(int64_t s, uint32_t seed)
{
    while (s)
    {
	unsigned char *buf;
	int c;

	if ((uint64_t)s>MBUFSIZ) c=MBUFSIZ; else c=s;

	if (must>sqrt((double)pos)*bias) buf=buf2; else buf=buf1;

	dobuf(buf, c);

	if (s>0) s-=c;
    }
}

int
main(int argc, char **argv)
{
    double ds;
    int64_t s;
    unsigned long seed;
    long bits;

    if (argc>2) sscanf(argv[2], "%lu", &seed);
    else seed=time(0)+getpid();
    if (argc<2) crash("give a size arg in bytes, or 'inf'");
    if (sscanf(argv[1], "%lf", &ds)==1) s=(int64_t)ds;
    else
    {
	if (strcmp(argv[1],"inf")) crash("not a number");
	s= -1;
    }

    seed=seed*987653421+1;
    bits=seed;
    bias=(((bits>>24)&255)+((bits>>16)&255)-255)*0.008;

    initlogcache();
    initlast();
    initbuf();

    blat(s, seed);

    return 0;
}
