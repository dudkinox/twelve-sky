/* Test software for gjrand random numbers version 2.1.0.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../../src/my_int_types.h"

static void
crash(char *s)
{
    fprintf(stderr, "crash addsub [%s]\n", s);
    exit(2);
}

static int errorlevel=0;

static void seterr(int e) {if (errorlevel<e) errorlevel=e;}

#define SHIFT 20
#define SIZE (1<<SHIFT)

static unsigned char buf[SIZE];
static short tmpacc[SIZE];
static int tmpsat=0;
static double acc[SIZE];
static short trans[256];

static void
init(void)
{
    int j;

    for (j=0; j<SIZE; j++) acc[j]=0.0;
    memset(tmpacc, 0, sizeof(tmpacc));
}

static void
desat(void)
{
    int i;

    for (i=0; i<SIZE; i++)
    {
	acc[i]+=tmpacc[i];
	tmpacc[i]=0;
    }

    tmpsat=0;
}

static int
accu(void)
{
    int i;

    for (i=0; i<SIZE; i++) tmpacc[i]+=trans[buf[i]];

    return 1;
}

static void
butterfly(double *x, double *y)
{
    register double a, b;
    int siz=y-x;

    do
    {
	a = *x; b = *y;
	*x = a+b; *y = a-b;
	x++; y++;
	siz--;
    } while (siz>0);

    siz=y-x;
    if (siz>1)
    {
	x-=siz; y-=siz;
	siz>>=1;
	butterfly(x, x+siz);
	butterfly(y, y+siz);
    }
}

static void
dostats(void)
{
    butterfly(acc, acc+SIZE/2);
}

static void
printstats(int64_t bcount)
{
    int i, count;
    double t, x, low[3], high[3];
    long double le;
    double s=0.0, ss=0.0, scale=1.0/sqrt(((double)bcount)*SIZE*21845);

    low[0]=low[1]=low[2]=1e38;
    high[0]=high[1]=high[2]= -1e38;

    for (i=0; i<SIZE; i++)
    {
	x=acc[i]*scale;

	if (x>high[2])
	{
	    high[2]=x;
	    if (x>high[1])
	    {
		high[2]=high[1];
		high[1]=x;
		if (x>high[0])
		{
		    high[1]=high[0];
		    high[0]=x;
		}
	    }
	}
	if (x<low[2])
	{
	    low[2]=x;
	    if (x<low[1])
	    {
		low[2]=low[1];
		low[1]=x;
		if (x<low[0])
		{
		    low[1]=low[0];
		    low[0]=x;
		}
	    }
	}
	s+=x; ss+=x*x;
    }

    count=SIZE;

    printf("mean=%f\n", s/count);
    printf("sd  =%f\n\n", sqrt(ss/count-s*s/((double)count*(double)count)));

    printf("expected range (first high) [ 4.36 4.45 4.59 5.20 5.62 6.01 ]\n");
    printf("high=%f %f %f\n", high[0], high[1], high[2]);
    printf("low =%f %f %f\n\n", low[0], low[1], low[2]);

    t=fabs(low[0]);
    if (high[0]>t) t=high[0];

    le=erfc(M_SQRT1_2*t)*0.5;
    le=le/(1.0-le);
    le*=2*SIZE;
    le/=(le+1.0);
    printf("One sided P value (very small numbers are bad)\n");
    printf("P = %.3g\n", (double)le);
}

void
mktrans(int reverse)
{
    int i, j;

    for (i=0; i<256; i++)
    {
	j=i;
	if (reverse)
	    j=((i&1)<<7) | ((i&2)<<5) | ((i&4)<<3) | ((i&8)<<1)
		| ((i&16)>>1) | ((i&32)>>3) | ((i&64)>>5) | ((i&128)>>7);

	trans[i]=2*j-255;
    }
}

static int64_t
dofile(int64_t s)
{
    int64_t r=0;

    init();

    while (fread(buf, 1, SIZE, stdin)==SIZE)
    {
	accu();
	tmpsat++;
	if (tmpsat>=120) desat();
	r++;
	if (s>0 && r>=s) break;
    }

    desat();

    if (s>0 && r<s)
    {
fprintf(stderr, "Warning, addsub expected %.0f 1MB blocks, saw only %.0f\n",
		(double)s, (double)r);
	seterr(1);
    }

    dostats();

    return r;
}

int
main(int argc, char **argv)
{
    double sd;
    int64_t s, count;
    int reverse=0;

    if (argc!=2 && argc!=3)
crash("give 1 or 2 args: either -f or -r followed by optional size");

    if (strcmp(argv[1], "-r")==0) reverse=1;
    else if (strcmp(argv[1], "-f")!=0) crash("bad first arg (expect -r or -f)");
    mktrans(reverse);
    if (argc==3)
    {
	if (sscanf(argv[2], "%lf", &sd)!=1) crash("not a number");
	s=(int64_t)(sd*(1.0/SIZE));
    }
    else s= -1;

    if (s==0) s++;

    count=dofile(s);

    printstats(count);

    return errorlevel;
}
