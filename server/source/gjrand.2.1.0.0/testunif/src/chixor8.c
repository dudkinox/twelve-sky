/* Test software for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../../src/my_int_types.h"
#include "../../testcommon/chi2p.h"

/* chi square test on 8 bit values. */
/* This one actually tests a byte XOR'd with an earlier byte for */
/* all lags 1 to 16. */

static int errorlevel=0;
static void seterr(int e) {if (errorlevel<e) errorlevel=e;}

static int64_t count[16][256];
static int32_t scount[16][256];

static double
doan2(int j, int64_t pos)
{
    double e=(double)pos/256.0, t=0.0, x;
    int i;

    for (i=0; i<256; i++)
    {
	x=(double)count[j][i]-e;
	t+=x*x/e;
    }

    printf("%2d: %15.5f\n", j+1, (double)t);

    return t;
}

static void
doan(int64_t pos)
{
    double x, t=0.0, e;
    int j;

    for (j=0; j<16; j++)
    {
	x=doan2(j, pos);
	x-=255.0;
	t+=x*x;
    }

    t/=510.0;

    printf("\ncombo expected range [ 3.9 5.8 9.2 23.6 32.5 40 ]\n");
    printf("    %15.5f\n\n", t);

    /* chis-squared df=14 has the closest match to skewness. */
    /* we transform to get the right mean and sd */
    /* (actually we mostly care about high side.) */
    x = (t - 20.463) * ((31.320-18.151)/(34.867-20.463)) + 18.151;
    if (x<0.0) x = 0.0;
    e = chi2p1(x, 14);

    printf("One sided P value (very small numbers are bad)\n");
    printf("P = %.3g\n", e);
}

static void
dosflush(void)
{
    int i, j;

    for (i=0; i<16; i++) for (j=0; j<256; j++)
	{count[i][j]+=scount[i][j]; scount[i][j]=0;}
}

static void
readstuff(int64_t max)
{
    static unsigned char buf_b[16+4096];
    unsigned char * const buf=buf_b+16;
    int64_t pos=0;
    int i, k, sflush=0;

    memset(count, 0, sizeof(count));
    memset(scount, 0, sizeof(scount));

    if (fread(buf+4080, 1, 16, stdin)!=16)
    {
	fprintf(stderr,"File has less than 16 samples, no good!\n");
	exit(2);
    }

    while (1)
    {
	for (i=0; i<16; i++) buf_b[i]=buf[4080+i];
	i=fread(buf, 1, 4096, stdin);
	if (i<=0) break;
	if (max>0 && max<pos+i) i=max-pos;
	pos+=i;
	sflush+=i;
	i--;
	do
	{
	    k=buf[i];
#define CINC(x) scount[x][buf[i-x-1]^k]++
	    CINC(0);
	    CINC(1);
	    CINC(2);
	    CINC(3);
	    CINC(4);
	    CINC(5);
	    CINC(6);
	    CINC(7);
	    CINC(8);
	    CINC(9);
	    CINC(10);
	    CINC(11);
	    CINC(12);
	    CINC(13);
	    CINC(14);
	    CINC(15);
#undef CINC

	    i--;
	} while (i>=0);
	if (sflush>40000000) {dosflush(); sflush=0;}
	if (max>0 && pos>=max) break;
    }

    if (sflush) {dosflush(); sflush=0;}

    if (max>0 && pos<max)
    {
	fprintf(stderr, "Warning, expected %.0f byte samples, saw only %.0f\n",
		(double)max, (double)pos);
	seterr(1);
    }

    if (pos<2560)
    {
	fprintf(stderr, "Warning, < 2560 samples, don't trust result.\n");
	seterr(1);
    }

    doan(pos);
}

int
main(int argc, char **argv)
{
    double dmax;
    int64_t max;

    if (argc<2 || sscanf(argv[1], "%lf", &dmax)!=1 || dmax<1.0) max= -1;
    else max=(int64_t)(dmax-15.9);

    printf("expected range [ 191 205 227 284 310 331 ]\n");

    readstuff(max);

    return errorlevel;
}
