/* Part of gjrand random number library version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* This is Maurer's Universal Statistical Test on 8 bit words. */
/* (Though i got the description third hand from a dubious source.) */
/* 64 bit counters are used to work on extremely large files if you */
/* have the patience. */

/* maurer8 takes one optional argument for length of input and otherwise */
/* reads from standard input until end of file. */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../../src/my_int_types.h"

int32_t last[256];

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

/* Versions 0.1.5.0 and earlier disagreed in the 14th place. */
/* This should be better, perhaps 15 to 20 places accuracy. */
#define CM 7.1836655534922686447418805

void
initlogcache(NOARGS)
{
    int i;
    invlog2=1.0/log(2.0);
    for (i=0; i<MAXLOGCACHE; i++) mcache[i]=(log((double)i)*invlog2)-CM;
}

INLINE double
fastmlog(int i)
{
    if ((unsigned)i<MAXLOGCACHE) return mcache[i];
    return (log((double)i)*invlog2)-CM;
}

/* This is a very fake, add-hoc way to handle startup. */
/* The advantage is that it is much simpler than my old way. */
/* 162 is a guess. */
void
initlast(NOARGS)
{
    int i;

    for (i=0; i<256; i++) last[i]= -162;
}

double
mbuf(const unsigned char * const buf, int c)
{
    double r=0.0;
    int i, ch, gap;
    const unsigned char * const bend=buf+c;

    for (i=0; i<256; i++)
    {
	last[i]-=c;
	if (last[i]< -1000000000)
	    crash("position overflow, input seriously non-random\n");
    }

    i= -c;
    do
    {
	ch=bend[i];
	gap=i-last[ch];
	last[ch]=i;
	r+=fastmlog(gap);
	i++;
    }
    while (i<0);

    return r;
}

#define MBUFSIZ 8192
#define MBUFMASK (MBUFSIZ-1)

int64_t pos;

double
maurer(int64_t max)
{
    unsigned char buf[MBUFSIZ];
    double r=0.0;
    int64_t pos1=0, t;
    int c;

    initlast();

    while (max<0 || pos1<max)
    {
	t=max-pos1;
	if ((uint64_t)t>MBUFSIZ) t=MBUFSIZ;
	c=fread(buf, 1, t, stdin);
	if (c<=0) break;
	r+=mbuf(buf, c);
	pos1+=c;
    }

    pos=pos1;

    if (max>0 && pos1<max)
	fprintf(stderr, "Warning, expected %.0f samples, saw only %.0f\n",
		(double)max, (double)pos1);


    return r / ((double)(pos1));
}

/* This was carefully calculated but is somehow wrong. */
/* I have a theory for why this is but can't yet do the maths for a */
/* correction. This is the standard deviation for a single measurement */
/* and the calculation below scales it by assuming many independent */
/* measurements of this kind and doing root mean square. The measurements */
/* are in fact not independent since shifting one word forward to increase */
/* prior distance will decrease its next future distance and have some effect */
/* probably small on various other distances. */
/* #define CSD 1.799628339677786348 */

/* Determined emperically, not very accurate. */
#define CSD 1.106

int
main(int argc, char **argv)
{
    double m;
    double dmax;
    int64_t max;

    if (argc<2 || sscanf(argv[1], "%lf", &dmax)!=1 || dmax<1.0) max= -1;
    else max=dmax;

    initlogcache();
    m=maurer(max);
    printf("%15.10f    (deflection %.5e)\n", m+CM, m);
    printf("expected range (sigma) [ -3.09 -2.33 -1.28 1.28 2.33 3.09 ]\n");
    printf("%10.5f sigma\n", m * sqrt((double)pos) * (1.0/CSD));
    return 0;
}
