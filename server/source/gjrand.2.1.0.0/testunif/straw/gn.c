/* Miscellaneous stuff for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* This one writes a file for use by the tests in testunif */

/* first arg is required file size in bytes or "inf" to write forever. */
/* second optional arg is random seed. */

/* Geronimo's Nightmare. (Nicknamed when i thought i invented rda, and */
/* when it was one of the best tests i had.) */

/* This one is specially constructed to fool Repeat Distance Analysis */
/* on 8 bit words, while being obviously non-random to almost anything else. */
/* Rda appears to be a good test, though i no longer call it the best in */
/* testunif. It does have one subtle weakness in that all distances greater */
/* than 750 are lumped into one bucket to give a large enough count for a */
/* meaningful chi-squared. That is targeted here. */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include "../../src/gjrand.h"

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

void
initlast(NOARGS)
{
    int i;
    for (i=0; i<256; i++) last[i]= -1;
}

#define MBUFSIZ 4096
#define MBUFMASK (MBUFSIZ-1)

int64_t pos=0;

unsigned char buf[MBUFSIZ];

void
dobuf(int c)
{
    int i, ch;
    int64_t p;

    for (i=0; i<c; i++)
    {
	ch=buf[i];
	if (pos-last[ch]>760)
	{
	    p=pos-760;
	    for (ch=0; ch<256; ch++) if (last[ch]<p) goto found;
	    crash("this can't happen");
	    found:
	    buf[i]=ch;
	}

	last[ch]=pos;

	pos++;
    }

    if (fwrite(buf, 1, c, stdout)!=(size_t)c) crash("fwrite");
}

struct gjrand g;

void
blat(int64_t s, uint32_t seed)
{
    while (s)
    {
	int c;

	if ((uint64_t)s>MBUFSIZ) c=MBUFSIZ; else c=s;

	gjrand_randbytes(&g, c, buf);

	dobuf(c);

	if (s>0) s-=c;
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

    initlast();

    gjrand_init(&g, seed);

    blat(s, seed);

    return 0;
}
