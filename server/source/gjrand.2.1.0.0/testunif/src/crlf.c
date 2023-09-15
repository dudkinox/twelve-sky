/* Test software for gjrand random numbers version 2.1.0.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* The other tests in this directory are aimed at setting tough tests */
/* for something you think might be very good random bits. This one could */
/* just about work like that, but its main intent is to give you a few */
/* hints for identifying things that aren't random at all. */

/* Someone recommends a random number generator to you and says it */
/* is really good. You run a few tests and it's incredibly bad. */
/* What is going on? A few of the more likely possibilities: */

/* 1. It's incredibly bad. Your informant hasn't got a clue. */
/* 2. The 11MegaByte file you thought was test random data for use with */
/*    DIEHARD is actually something else that isn't very random. */
/* 3. The test file you are using was at some stage on a MS-Windows */
/*    or similar stupid system and someone got careless and allowed */
/*    it be converted from "binary" to "text" or vice-versa. Stats */
/*    relating to byte values 10 and 13 are totally out of whack. */
/* 4. The test file was transferred by FTP and someone carelessly */
/*    forgot to use "binary" mode. All bytes have had their most */
/*    significant bit reset to zero. */
/* 5. Someone heard of internationalisation but didn't understand it */
/*    very well. They assumed your test file was ISO_8859-1, or some other */
/*    narrow-minded regional coding, and converted it into utf-8, converting */
/*    all bytes with the high order bit set into multibyte sequences, */
/* 6. What you thought was uniform random bits is actually uniform */
/*    random floating point numbers on [ 0 .. 1 ) in some binary */
/*    format. */
/* 7. Other miscellaneous mixups. */

/* 2-7 can really happen. It is rumoured that a famous random */
/* number researcher once got stung by 3 and went public in a big way */
/* before the mistake was discovered. */

/* So if a supposedly good random generator tests out really bad, */
/* be careful. Most random tests don't tell you exactly what is wrong. */
/* crlf attempts to detect some of the above a bit more explicitly. */
/* It still requires some intelligence on your part and careful reading */
/* of diagnostics. And it can't categorise every possible data disaster. */
/* And a generator that is genuinely extremely bad will likely fail some */
/* of these tests anyway. */

/* At the moment i have the following kinds of test: */
/* 1. Chi-squared test on byte values. */
/*    Also print a list of the most and least frequent byte values. */
/* 2. Relative frequency of CR, LF, CRLF, LFCR. If highly unlikely, say so. */
/* 3. Relative frequency of characters with high bit set. (If 0, you may */
/*    be a victim of "text mode" FTP or similar. ) */
/* 4. Relative frequency of characters which are printable in iso_8859-1 */
/*    character set. */
/* 5. Number of bytes that would be "wrong" if this is supposed to be a */
/*    utf-8 file. If this is 0, perhaps it really is a utf-8 file. */
/*    Any wide divergence is suspicious though. */
/* 6. Relative frequency of 4 and 8 byte blocks that are valid floats */
/*    in the range [ 0 .. 1 ) for either single or double precision */
/*    native binary format. (Other formats would be nice too but there */
/*    are too many to choose from. If you have one you particularly want */
/*    tested, ask on SourceForge and point me at adequate documentation */
/*    in a format i can read without using MS-Windows.) */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../../src/my_int_types.h"

/* counters */
static int64_t count[256], count_crlf=0, count_lfcr=0, count_badutf=0;
static int64_t count_float=0, count_double=0;

/* state variables to detect multibyte patterns accross block boundaries. */
static int cr_state=0, lf_state=0, utf_state=0;

#define SIZ 4096

static unsigned char buf[SIZ];

static void
chi(int64_t siz)
{
    struct cf {int64_t f; unsigned char c;} cf[256], t;
    double ds=siz, x=ds/256.0, y, chi=0.0;
    int i, j;

    for (i=0; i<256; i++)
    {
	y=x-count[i];
	chi+=y*y/x;
    }
    if (siz<2560) printf("warn < 2560 bytes, don't trust chisquare test\n");
    printf("expect [ 190 205 226 285 311 331 ]\n");
    printf("chis = %.10f\n", chi);

    for (i=0; i<256; i++) {cf[i].f=count[i]; cf[i].c=i;}
    for (i=1; i<256; i++)
    {
	t=cf[i];
	for (j=i-1; j>=0 && cf[j].f>t.f; j--) cf[j+1]=cf[j];
	cf[j+1]=t;
    }
    printf("common byte values (top 10)\n");
    for (i=255; i>=246; i--) printf("%02x %.0f\n", cf[i].c, (double)cf[i].f);
    printf("rare byte values (bottom 10)\n");
    for (i=9; i>=0; i--) printf("%02x %.0f\n", cf[i].c, (double)cf[i].f);

    printf("\n\n");
}

static void
chi1df(int64_t siz, int64_t ct, double rat, const char *name)
{
    double x=siz*rat, y=siz-x, chi, t;

    t=x-ct; chi=t*t/x;
    t=y+ct-siz; chi+=t*t/x;
    printf("expect [ 0.00000 0.00015 0.015 2.8 6.7 10.8 ]\n");
    printf("%s chis = %.10f\n", name, chi);
    if (chi>8.0)
	printf("%s too %s? %.0f (expect %.0f)\n",
		name, (ct<x)?"low":"high", (double)ct, x);
}

static void
crlf(int64_t siz)
{
    if (siz<2560) printf("warn < 2560 bytes, don't trust CR and LF test\n");

    chi1df(siz, count['\r'], 1.0/256.0, "CR");
    chi1df(siz, count['\n'], 1.0/256.0, "LF");

    if (siz<655360) printf("warn < 655360 bytes, don't trust CRLF tests\n");

    chi1df(siz, count_crlf, 1.0/65536.0, "CRLF");
    chi1df(siz, count_lfcr, 1.0/65536.0, "LFCR");

    printf("\n\n");
}

static void
highbit(int64_t siz)
{
    int64_t h;
    int i;

    if (siz<20) printf("warn < 20 bytes, don't trust high bit chis\n");

    h=0;
    for (i=128; i<256; i++) h+=count[i];

    chi1df(siz, h, 128.0/256.0, "high bit");

    printf("\n\n");
}

/* This counts the number of characters that are considered unprintable */
/* according to the ISO_8859 8 bit character sets. Mostly this follows */
/* ISO_8859-1, but they are probably the same for the others too. */
static void
iso_8859(int64_t siz)
{
    int64_t b;
    int i;

    if (siz<50) printf("warn < 50 bytes, don't trust iso_8859 chis\n");

    b=0;
    for (i=0; i<9; i++) b+=count[i];
    for (i=14; i<32; i++) b+=count[i];
    for (i=127; i<160; i++) b+=count[i];

    chi1df(siz, b, (9-0 + 32-14 + 160-127)/256.0, "iso_8859 unprintable");

    printf("\n\n");
}

static
void badutf8(int64_t siz)
{
    /* Yes, i should do the full Markov chain calculation for mean */
    /* and variance and do a proper chi-squared. But so far i'm too lazy. */
    printf("expect somewhere near 0.4095\n");
    printf("fraction bad utf8 = %.10f\n", (double)count_badutf / (double)siz);
    printf("\n\n");
}

static
void okfloat(int64_t siz)
{
    /* This expectation is probably only right if it is */
    /* IEEE single precision in 32 bits or very near offer. */
    /* In addition, the result depends on whether a negative de-normal */
    /* is treated as negative or zero. Negative would be nice, but */
    /* some machines make it zero. */
    /* You want the first OKFRAT if negative, second if zero. */
    /* If you don't know, try both ways, and pick the one that works */
    /* better with a good random file of a hundred megabytes or so. */
    /* Alternatively, ../util/denormal will attempt to guess for you. */
    /* Also comment out the #error line. */
#error "FIX THE NEXT TWO LINES!"
#define OKFRAT (127.0/512.0)
/* #define OKFRAT (128.0/512.0) */

    chi1df(siz/sizeof(float), count_float, OKFRAT, "float [0,1)");

#undef OKFRAT

    printf("\n\n");
}

static
void okdouble(int64_t siz)
{
    /* This expectation is probably only right if it is */
    /* IEEE double precision in 64 bits or very near offer. */
    /* Read comments in okfloat(). Same story, pick one OKDRAT. */
#error "FIX THE NEXT TWO LINES!"
#define OKDRAT (1023.0/4096.0)
/* #define OKDRAT (1024.0/4096.0) */

    chi1df(siz/sizeof(double), count_double, OKDRAT, "double [0,1)");

#undef OKDRAT

    printf("\n\n");
}

static void
doblock(int size)
{
    int cr=cr_state, lf=lf_state, utf=utf_state;
    int i, j, sz2;

    for (i=0; i<size; i++)
    {
	j=buf[i];
	count[j]++;

	if (j=='\n')
	{
	    if (cr) count_crlf++;
	    cr=0; lf=1;
	}
	else if (j=='\r')
	{
	    if (lf) count_lfcr++;
	    lf=0; cr=1;
	}
	else {cr=0; lf=0;}

	if (utf)
	{
	    if ((j&0xc0)!=0x80) count_badutf++;
	    utf--;
	}
	else if (j&0x80)
	{
	    if ((j&0xe0)==0xc0) utf=1;
	    else if ((j&0xf0)==0xe0) utf=2;
	    else if ((j&0xf8)==0xf0) utf=3;
	    else if ((j&0xfc)==0xf8) utf=4;
	    else if ((j&0xfe)==0xfc) utf=5;
	    else count_badutf++;
	}
    }

    sz2=size-size%sizeof(float);
    for (i=0; i<sz2; i+=sizeof(float))
    {
	double x = * ((float *)(buf+i));
	if (finite(x) && x>=0.0 && x<1.0) count_float++;
    }
    sz2=size-size%sizeof(double);
    for (i=0; i<sz2; i+=sizeof(double))
    {
	double x = * ((double *)(buf+i));
	if (finite(x) && x>=0.0 && x<1.0) count_double++;
    }

    cr_state=cr; lf_state=lf; utf_state=utf;
}

static void
dofile(int64_t size)
{
    int64_t s=0;

    memset((void *)count, 0, sizeof(count));

    while (size<0 || s<size)
    {
	int i=4096;

	if (size>0 && i>size-s) i=size-s;
	i=fread(buf, 1, i, stdin);
	if (i==0) break;
	doblock(i);
	s+=i;
    }

    if (size>=0 && s!=size)
	fprintf(stderr, "warning: expected %.0f bytes, saw only %0.f\n",
		(double)size, (double)s);

    chi(s);
    crlf(s);
    highbit(s);
    iso_8859(s);
    badutf8(s);
    okfloat(s);
    okdouble(s);
}

int
main(int argc, char **argv)
{
    double ds;
    int64_t s= -1;

    if (argc>=2)
    {
	if (sscanf(argv[1], "%lf", &ds)!=1)
	{
	    fprintf(stderr, "size arg is not a number\n");
	    exit(1);
	}
	s = (int64_t)ds;
    }

    dofile(s);

    return 0;
}
