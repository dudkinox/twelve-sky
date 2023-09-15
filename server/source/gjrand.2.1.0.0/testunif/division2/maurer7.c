/* Part of gjrand random number library version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* This is Maurer's Universal Statistical Test on 7 bit words. */
/* (Though i got the description third hand from a dubious source.) */
/* 64 bit counters are used to work on extremely large files if you */
/* have the patience. */

/* maurer7 takes one optional argument for length of input (now in bytes) */
/* and otherwise reads from standard input until end of file. */

/* The Maurer test is often done with 16 bit words. */
/* testunif has Maurer tests with 7 or 8 bit words. My reasoning in choosing */
/* different word sizes is that it is very easy to create an obviously */
/* non-random file that passes the Maurer test for any single word size */
/* known in advance (straw/mn). It is somewhat harder to pass for two */
/* different word sizes, and harder again if the word sizes are mutually */
/* prime. */

/* Extreme paranoids might want long double */
typedef double myfloat;
/* logl() for long double ; log() for double or float */
#define mylog(x) log(x)

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../../src/my_int_types.h"

int32_t last[128];

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
myfloat mcache[MAXLOGCACHE];
myfloat invlog2;

/* I doubt this is really accurate to 25 decimal places, but with luck */
/* it might be to 15 or 20. It differs from version 0.1.5.0 and earlier */
/* in the 10th decimal place. */
/* NIST "UNIVERSAL" test gives only 7 decimal places, which agree. */
/* RaBiGeTe gives only 7 decimal places, which agree with NIST. */
/* Isaac has a Maurer test but no obvious constants :-) */
#define CM ((myfloat)6.1962506541018770797601967L)

void
initlogcache(NOARGS)
{
    int i;
    invlog2=1.0/mylog(2.0);
    for (i=0; i<MAXLOGCACHE; i++) mcache[i]=(mylog((myfloat)i)*invlog2)-CM;
}

INLINE myfloat
fastmlog(int i)
{
    if ((unsigned)i<MAXLOGCACHE) return mcache[i];
    return (mylog((myfloat)i)*invlog2)-CM;
}

#define MBUFSIZ 8192
#define MBUFMASK (MBUFSIZ-1)

int avail7= -1, used7= -1;
unsigned char buf[MBUFSIZ];

int
refill7(NOARGS)
{
    static unsigned char buf8[MBUFSIZ*7/8+5];
    int i, j, ch;

    i=fread(buf8, 1, MBUFSIZ*7/8, stdin);
    if (i<=0) return i;
    avail7=i*8/7;

    i=MBUFSIZ*7/8-7;
    j=MBUFSIZ-8;

    do
    {
	ch=buf8[i+6];
	buf[j+7]=ch&127;

	ch=(ch>>7)|(buf8[i+5]<<1);
	buf[j+6]=ch&127;

	ch=(ch>>7)|(buf8[i+4]<<2);
	buf[j+5]=ch&127;

	ch=(ch>>7)|(buf8[i+3]<<3);
	buf[j+4]=ch&127;

	ch=(ch>>7)|(buf8[i+2]<<4);
	buf[j+3]=ch&127;

	ch=(ch>>7)|(buf8[i+1]<<5);
	buf[j+2]=ch&127;

	ch=(ch>>7)|(buf8[i]<<6);
	buf[j+1]=ch&127;

	ch=(ch>>7);
	buf[j]=ch&127;

	i-=7; j-=8;
    } while (j>=0);

    return avail7;
}

int
get7(NOARGS)
{
    int ch;

    if (used7>=avail7)
    {
	used7=0;
	refill7();
	if (avail7<=0) return EOF;
    }

    ch=buf[used7];
    used7++;

    return ch;
}

void
initlast(NOARGS)
{
    int i;
    for (i=0; i<128; i++) last[i]= -85;
}

void
updatelast(int c)
{
    int i;

    for (i=0; i<128; i++)
    {
	last[i]-=c;
	if (last[i]< -1000000000)
	    crash("position overflow, input seriously non-random\n");
    }
}

myfloat
mbuf(const unsigned char * const buf, int c)
{
    myfloat r=0.0;
    int i, ch, gap;
    const unsigned char * const bend=buf+c;

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

int64_t pos;

myfloat
maurer(int64_t max)
{
    myfloat r=0.0;
    int64_t pos1=0, t;
    int c;

    initlast();

    while (max<0 || pos1<max)
    {
	t=max-pos1;
	c=refill7();
	if (c<=0) break;
	if (max>0 && c>t) c=t;
	updatelast(c);
	r+=mbuf(buf, c);
	pos1+=c;
    }

    pos=pos1;

    if (max>0 && pos1<max)
	fprintf(stderr,
"Warning, expected %.0f 7 bit samples, saw only %.0f\n",
		(double)max, (double)pos1);

    return r / ((myfloat)(pos1));
}

/* Determined emperically, not very accurate. */
/* I think this relates in some way to the "variance" numbers in NIST */
/* but i'm not sure exactly what the relationship is. */
/* It is tempting to substitute the presumably more accurate values */
/* from NIST or RaBiGeTe, but they are in a different format which */
/* would require digging and mathematics to convert, and anyway, 2 */
/* decimal places is enough for the way we use it here. (NIST probably */
/* needs more because they do many Maurer tests and analyse spread of */
/* results for uniformity.) */
#define CSD 1.05

int
main(int argc, char **argv)
{
    myfloat m, x;
    double dmax, y;
    int64_t max;

    if (argc<2 || sscanf(argv[1], "%lf", &dmax)!=1 || dmax<1.0) max= -1;
    else max=((int64_t)(8.0*dmax))/7;

    initlogcache();
    m=maurer(max);
    printf("%15.10f    (deflection %.5e)\n", (double)(m+CM), (double)m);
    printf("expected range (sigma) [ -3.09 -2.33 -1.28 1.28 2.33 3.09 ]\n");
    x = m * sqrt((myfloat)pos) * (1.0/CSD);
    printf("%10.5f sigma\n\n", (double)x);

    y=erfc(fabs((double)(M_SQRT1_2*x)));
    printf("One sided P value (very small numbers are bad)\n");
    printf("P = %.6g\n", y);

    return 0;
}
