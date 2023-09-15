/* Test software for gjrand random numbers version 2.1.0.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../../src/my_int_types.h"

/* Bytes of input are interpreted as signed numbers (two schemes are */
/* available with -f or -r). Block size currently 64kB. */
/* We add the same byte position from each block to get a 64k sized */
/* block of totals. Then do an autocorrelation for this totals block */
/* for all lags up to 32k. It seems to me that this should be less powerful */
/* than doing autocorrelation seperately on each block and then analysing */
/* all those results (RMS?). On the other hand, this method is hugely faster. */
/* Even using FFT for autocorrelation wouldn't make it very practical to */
/* seperately autocorrelate each block for GigaBytes. */

static int errorlevel=0;
static void seterr(int e) {if (errorlevel<e) errorlevel=e;}

static void
crash(char *s)
{
    fprintf(stderr, "crash selfcor [%s]\n", s);
    exit(2);
}

#define SHIFT 16
#define SIZE (1<<SHIFT)

#define MAXMEM (SIZE*10)

static double memory[MAXMEM];
static double *memptr=memory;

static short tmpacc[SIZE];
static int tmpsat=0;

static double *
memmark(int siz)
{
    double *r=memptr;

    memptr=r+siz;
if (memptr>memory+MAXMEM) crash("out of memory");

    return r;
}

static void
memrelease(double *mark)
{
    memptr=mark;
}


struct poly {int order; double *coef;};

static void
polysub2(double *ca, double *cr, int ord)
{
    int i=ord-2;

    do {cr[1+i]-=ca[1+i]; cr[i]-=ca[i]; i-=2;} while (i>=0);
}

static void
polyadd2(double *ca, double *cr, int ord)
{
    int i=ord-2;

    do {cr[1+i]+=ca[1+i]; cr[i]+=ca[i]; i-=2;} while (i>=0);
}

static void
polyadd3(double *ca, double *cb, double *cr, int ord)
{
    int i=ord-2;

    do {cr[1+i]=ca[1+i]+cb[1+i]; cr[i]=ca[i]+cb[i]; i-=2;} while (i>=0);
}

static void
polymul4(double *ca, double *cb, double *cr, int ord)
{
    cr[0]=ca[0]*cb[0];
    cr[1]=ca[0]*cb[1]+ca[1]*cb[0];
    cr[2]=ca[0]*cb[2]+ca[1]*cb[1]+ca[2]*cb[0];
    cr[3]=ca[0]*cb[3]+ca[1]*cb[2]+ca[2]*cb[1]+ca[3]*cb[0];
    cr[4]=ca[1]*cb[3]+ca[2]*cb[2]+ca[3]*cb[1];
    cr[5]=ca[2]*cb[3]+ca[3]*cb[2];
    cr[6]=ca[3]*cb[3];
    cr[7]=0;
}

static void
polymul2(double *ca, double *cb, double *cr, int ord)
{
    if (ord==4) {polymul4(ca, cb, cr, ord); return;}

    {
	int i;
	struct poly t1, t3;

	for (i=0; i<2*ord; i++) cr[i]=0;

	t1.order=ord; t3.order=ord;
	t1.coef=memmark(ord);

	polymul2(ca, cb, t1.coef, ord/2);
	polyadd2(t1.coef, cr, ord);
	polysub2(t1.coef, cr+ord/2, ord);

	polymul2(ca+ord/2, cb+ord/2, t1.coef, ord/2);
	polyadd2(t1.coef, cr+ord, ord);
	polysub2(t1.coef, cr+ord/2, ord);

	polyadd3(ca, ca+ord/2, t1.coef, ord/2);
	polyadd3(cb, cb+ord/2, t1.coef+ord/2, ord/2);

	t3.coef=memmark(ord);
	polymul2(t1.coef, t1.coef+ord/2, t3.coef, ord/2);

	polyadd2(t3.coef, cr+ord/2, ord);

	memrelease(t1.coef);
    }
}

static struct poly
polymul(struct poly a, struct poly b)
{
    struct poly r;

    if (a.order!=b.order) crash("polymul order mismatch");
    r.order=2*a.order;
    r.coef=memmark(r.order);
    polymul2(a.coef, b.coef, r.coef, a.order);

    return r;
}

static struct poly
revpoly(struct poly p)
{
    struct poly r;
    int i;

    r.order=p.order;
    r.coef=memmark(p.order);

    for (i=0; i<p.order; i++) r.coef[i]=p.coef[p.order-i-1];

    return r;
}

static void
zeropoly(struct poly r)
{
    int i;
    for (i=0; i<SIZE; i++) r.coef[i]=0.0;
}

double accstats[SIZE/2];

static void
dostats(struct poly p)
{
    int i;

if (p.order!=2*SIZE) crash("dostats bad p.order");

    for (i=0; i<SIZE/2; i++) accstats[i]=p.coef[SIZE+i]+p.coef[i];
}

static void
printstats(int64_t bcount)
{
    int i, j, count;
    double t, x, low[3], high[3];
    long double le;
    int ll[3], hl[3];
    double s=0.0, ss=0.0, scale=(sqrt(SIZE)/3.0/21845.0/21845.0)/bcount;

    low[0]=low[1]=low[2]=1e38;
    high[0]=high[1]=high[2]= -1e38;

    for (i=0; i<SIZE/2; i++)
    {
	j=i+1;

	x=accstats[i]*scale;

	if (x>high[2])
	{
	    high[2]=x; hl[2]=j;
	    if (x>high[1])
	    {
		high[2]=high[1]; hl[2]=hl[1];
		high[1]=x; hl[1]=j;
		if (x>high[0])
		{
		    high[1]=high[0]; hl[1]=hl[0];
		    high[0]=x; hl[0]=j;
		}
	    }
	}
	if (x<low[2])
	{
	    low[2]=x; ll[2]=j;
	    if (x<low[1])
	    {
		low[2]=low[1]; ll[2]=ll[1];
		low[1]=x; ll[1]=j;
		if (x<low[0])
		{
		    low[1]=low[0]; ll[1]=ll[0];
		    low[0]=x; ll[0]=j;
		}
	    }
	}
	s+=x; ss+=x*x;
    }

    count=SIZE/2;

    printf("mean=%f\n", s/count);
    printf("sd  =%f\n\n", sqrt(ss/count-s*s/((double)count*(double)count)));

    /* Somewhere here i suspect my probability calculations are wrong. */
    /* On the high side, they seem to be somewhere near right. On the */
    /* low side, very low numbers seem to be much rarer than these */
    /* figures suggest. Perhaps one of my dubious assumptions about */
    /* normality and independence is biting. Never mind. If a generator */
    /* blows out big-time (P<1.0e-6) you can rest assured it has done */
    /* something unlikely and suspicious, even if the exact P-value is */
    /* not quite right. */

    /* change this if you change SIZE  ( hinorm SIZE ) */
    printf("expected range (first high) [ 3.53 3.63 3.81 4.51 4.97 5.42 ]\n");

    printf("high=%f (%d)  %f (%d)  %f (%d)\n",
	high[0], hl[0], high[1], hl[1], high[2], hl[2]);
    printf("low =%f (%d)  %f (%d)  %f (%d)\n\n",
	low[0], ll[0], low[1], ll[1], low[2], ll[2]);

    t=fabs(low[0]);
    if (high[0]>t) t=high[0];

    le=erfc(M_SQRT1_2*t)*0.5;
    le=le/(1.0-le);
    le*=SIZE;
    le/=(le+1.0);
    printf("One sided P value (very small numbers are bad)\n");
    printf("P = %.3g\n", (double)le);
}

static short trans[256];

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

static void
dotmpacc(unsigned char *buf)
{
    int i;

    for (i=0; i<SIZE; i++) tmpacc[i]+=trans[buf[i]];
}

static void
desat(double *b)
{
    int i;

    for (i=0; i<SIZE; i++)
    {
	b[i]+=tmpacc[i];
	tmpacc[i]=0;
    }

    tmpsat=0;
}

static int64_t
dofile(int64_t s)
{
    unsigned char buf[SIZE];
    struct poly acc, q, m;
    int64_t r=0;

    memset(tmpacc, 0, sizeof(tmpacc));

    acc.order=SIZE; acc.coef=memmark(SIZE);
    zeropoly(acc);

    while (fread(buf, 1, SIZE, stdin)==SIZE)
    {
	dotmpacc(buf);
	tmpsat++;
	if (tmpsat>=120) desat(acc.coef);
	r++;
	if (s>0 && r>=s) break;
    }

    desat(acc.coef);

    if (s>0 && r<s)
    {
fprintf(stderr, "Warning, selfcor expected %.0f 64kB blocks, saw only %.0f\n",
		(double)s, (double)r);
	seterr(1);
    }

    q=revpoly(acc);
    m=polymul(acc, q);
    dostats(m);

    memrelease(acc.coef);

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
