/* Test software for gjrand random numbers version 1.3.3.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* This reads a large number of 2d points and has a method which selects */
/* pairs which are fairly close together. (It would be nice to select the */
/* N pairs that are closest together, but that would be slow. This tries */
/* to do something good but not that good in a simple linear time algorithm */
/* with a finite bound on memory size.) Then look at the angles that */
/* the lines between the pairs make with the x-axis. */

/* Certain tests are performed on the angles for uniformity and for */
/* non-dependence of angle on position. As usual, the particular tests were */
/* made up on the spur of the moment without checking the literature and may */
/* be inferior to, or poor approximations to, well known tests. But they */
/* seem to work well for this purpose. */

/* Using more points does make the test more powerful because */
/* it is then able to choose pairs with less seperation, which */
/* usually brings out the worst in faulty generators. */

/* The beauty of angle with x-axis, instead of distance, is that i don't */
/* have to do any stochastic geometry theory to know the distribution. */
/* For someone as lazy and mathematically challenged as me, that matters :-) */

/* For a long time this was the strongest 2D lattice buster that i knew how */
/* to do. It really is quite vicious, but a 2D modified version of diff3 */
/* appears to be even tougher. */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../../src/my_int_types.h"
#include "../../testcommon/pcombo.h"
#include "../../testcommon/chi2p.h"

static int errorlevel=0;
static void seterr(int e) {if (errorlevel<e) errorlevel=e;}

/* Because i do some silly optimisations from force of habit, SIZ1 */
/* really does have to be a power of two. */
/* Worse, if you change it at all, it breaks the bit-twiddling in */
/* rdaang(). So be careful. */
#define SIZ1 (1<<10)
#define SIZ (SIZ1*SIZ1)

struct ang {double x, y, d, a; int n;};
static struct ang ang[SIZ+3];

static void
init(void)
{
    int i;

    for (i=0; i<SIZ; i++)
	{ang[i].x=999.0; ang[i].d=1.0/16.0; ang[i].a=999.0; ang[i].n=i;}
}

#define INS 10

static void
ins(struct ang *a, struct ang *b)
{
    struct ang tmp, *i, *j;
    double c;

    for (i=a+1; i<=b; i++)
    {
	tmp= *i;
	c=tmp.a;
	j=i-1;
	while (j>=a && j->a>c) {j[1]= *j; j--;}
	j[1]=tmp;
    }
}

static void
qs1(struct ang *a, struct ang *b)
{
    struct ang tmp, *l, *m, *h;
    int n;

    start:

    l=a; h=b; n=b-a+1;

    m=a+n/2;
    if (l->a > h->a) {tmp= *l; *l= *h; *h=tmp;}
    if (h->a > m->a) {tmp= *h; *h= *m; *m=tmp;}
    if (l->a > h->a) {tmp= *l; *l= *h; *h=tmp;}

    while (1)
    {
	while (l<h && l->a < h->a) l++;
	if (l>=h) break;
	tmp= *l; *l= *h; *h=tmp;
	h--;
	while (l<h && l->a < h->a) h--;
	if (l>=h) break;
	tmp= *l; *l= *h; *h=tmp;
	l++;
    }

    if (h-a<n/2)
    {
	if (a+INS<h-1) qs1(a, h-1);
	if (l+INS<b) {a=l+1; goto start;}
    }
    else
    {
	if (l+INS<b) qs1(l+1, b);
	if (a+INS<h-1) {b=h-1; goto start;}
    }
}

static int
sortang(void)
{
    int i, j;

    qs1(ang, ang+SIZ-1);
    ins(ang, ang+SIZ-1);

    for (i=0; i<SIZ; i++)
    {
	if (ang[i].a>900.0) break;
	if (ang[i].a< -M_PI || ang[i].a>M_PI)
	{
	    fprintf(stderr, "i=%d atan=%20.10f\nbad atan\n", i, ang[i].a);
	    seterr(1);
	    exit(errorlevel);
	}
    }

    j=i;

    for (i=j; i<SIZ; i++) if (ang[i].a<900.0)
    {
	fprintf(stderr, "sortang mixup\n");
	seterr(1);
	exit(errorlevel);
    }

    ang[j].a=ang[0].a+2.0*M_PI;

    printf("found %d angles for processing\n", j);

    return j;
}

static double
chiang(int max)
{
    int histo[1024];
    double mul=1024.0/(2.0*M_PI);
    double ex=max/1024.0;
    double chi=0.0, r;
    int i;

    if (max<20000)
    {
	fprintf(stderr, "not enough angles: try with more data\n");
	seterr(1);
	exit(errorlevel);
    }

    for (i=0; i<1024; i++) histo[i]=0;

    for (i=0; i<max; i++) histo[((int)floor(ang[i].a*mul))&1023]++;

    for (i=0; i<1024; i++) {double d=histo[i]-ex; chi+=d*d;}
    chi/=ex;

    r = chi2p1(chi, 1023);

    printf("\nexpected range [ 890 921 966 1083 1133 1170 ]\n");
    printf("chis = %10.5f     P = %.6g\n", chi, r);

    return r;
}

/* Return 1 sided p-value */
static double
gapang(int max)
{
    int histo[64];
    double ex=2.0*M_PI/((double)max);
    double rex=1.0/ex;
    double tot=0.0, r;
    int i;

    if (max<20000)
    {
	fprintf(stderr, "not enough angles: try with more data\n");
	seterr(1);
	exit(errorlevel);
    }

    for (i=0; i<64; i++) histo[i]=0;
    for (i=0; i<max; i++)
    {
	tot=exp((ang[i].a-ang[i+1].a)*rex)*64.0;
	histo[(int)tot]++;
    }
    tot=0.0;
    ex=((double)max)/64.0;
    for (i=0; i<64; i++) {double d=histo[i]-ex; tot+=d*d;}
    tot/=ex;

    r = chi2p1(tot, 63);

    printf("\nexpected range [ 33.9 39.8 49.1 77.8 92.1 103.3 ]\n");
    printf("gapang = %10.5f     P = %.6g\n", tot, r);

    return r;
}

static unsigned char ind[SIZ];
static double expect[150];

static void
mkexpect(int max)
{
    double t=max, x;
    int i;

    for (i=1; i<150; i++)
    {
	x=t*(1.0/64.0);
	expect[i]=x;
	t-=x;
    }

    expect[0]=t;

    printf("\nexpected range [ 100 110 126 171 191 207 ]\n");
}

/* return p-value */
static double
dorda(int max)
{
    int last[64], count[150];
    double x, t, chi, r;
    int i;

    for (i=0; i<150; i++) count[i]=0;
    for (i=0; i<64; i++) last[i]= -999;
    for (i=max-160; i<max; i++) last[ind[i]]=i-max;

    for (i=0; i<max; i++)
    {
	unsigned j=ind[i];
	int k=i-last[j];

	if (k>=150) k=0;
	count[k]++;
	last[j]=i;
    }

    chi = 0.0;
    for (i=0; i<150; i++)
    {
	x=expect[i];
	t=count[i]-x;
	chi += t*t/x;
    }

    r = chi2p1(chi, 148);

    printf("rda = %15.10f     P = %.6g\n", chi, r);

    return r;
}

static double
rdaang(int max)
{
    int i;

    mkexpect(max);

    for (i=0; i<max; i++)
    {
	unsigned n=ang[i].n;
	n=((n>>14)&070) | ((n>>7)&7);
	ind[i]=n;
    }

    return dorda(max);
}

static void
doan(void)
{
    double pvals[3], pv;
    int s=sortang();

    pvals[0]=chiang(s);
    pvals[1]=gapang(s);
    pvals[2]=rdaang(s);
    pv=pcombo(pvals, 3);

    printf("\nOne sided P value (very small numbers are bad)\n");
    printf("P = %.3g\n", pv);
}

static void
dopoint(double v[2])
{
    double t, u, x, y;
    int i, j;
    struct ang *ptr;

    t=v[0]*SIZ1;
    u=floor(t);
    x=t-u;
    i=(int)u;
    i&=SIZ1-1;

    t=v[1]*SIZ1;
    u=floor(t);
    y=t-u;
    j=(int)u;
    j&=(SIZ1-1);

    ptr=SIZ1*i+j+ang;

    if (ptr->x>2.0)
    {
	if (x>0.25 && x<0.75 && y>0.25 && y<0.75)
	    {ptr->x=x; ptr->y=y;}
    }
    else
    {
	x-=ptr->x; y-=ptr->y;
	t=x*x+y*y;
	if (t<ptr->d)
	{
	    ptr->d=t;
	    ptr->a=atan2(x,y);
	}
    }
}

static void
dodata(int64_t n)
{
    double v[2], dx, dy;
    int64_t i=0;

    init();

    if (fread(v, sizeof(double), 2, stdin)!=2)
    {
	fprintf(stderr, "no data?\n");
	seterr(1);
	exit(errorlevel);
    }
    dx=v[0]/1024.0; dy=v[1]/1024.0;

    do
    {
	if (fread(v, sizeof(double), 2, stdin)!=2) break;
	v[0]+=dx; v[1]+=dy;
	dopoint(v);
	i++;
    } while (n<0 || i<n);

    if (n>=0 && i<n)
    {
	fprintf(stderr, "warning expected %.0f points, saw only %.0f\n",
		(double)n, (double)i);
	seterr(1);
    }

    doan();
}

int
main(int argc, char **argv)
{
    double count;

    if (argc<2 || sscanf(argv[1], "%lf", &count)!=1) count= -2;

    dodata((int64_t)count/2);

    return errorlevel;
}
