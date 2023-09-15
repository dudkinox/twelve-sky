/* Test software for gjrand random numbers version 1.3.3.0 or later. */
/* Copyright (C) 2006-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* Various random walks in multiple dimensions. */
/* Which face of the box does the walker crash out of? */
/* Which corner is it nearest at the time? */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../../src/my_int_types.h"
#include "../../testcommon/pcombo.h"
#include "../../testcommon/chi2p.h"

#ifdef __cplusplus
#define NOARGS
#else
#define NOARGS void
#endif

static int errorlevel=0;
static void seterr(int e) {if (errorlevel<e) errorlevel=e;}

static double
achi(int n, uint64_t c[])
{
    double x, t, e;
    int i;

    e=0.0;
    for (i=0; i<n; i++) e+=c[i];
    e /= n;

    if (e<10.0)
    {
	fprintf(stderr,"warning: not enough data, stats may be wonky\n");
	fprintf(stderr,"  (if running with more than about 300k samples,\n");
	fprintf(stderr,"  and you see this, your generator is stuffed.)\n");
	seterr(1);
    }

    x=0.0;
    for (i=0; i<n; i++)
    {
	t=c[i]-e;
	x+=t*t;
    }

    x /= e;
    t = chi2p1(x, n-1);
    printf(" %10.5f (%10g)", x, t);

    return t;
}

static double loc[9][60];
static uint64_t face[9][120], fpair[8][196], vert[8][128];
static int inpair[8];

static void
doan(NOARGS)
{
    double pval[19];
    int pc=0, i;

    printf(
"dim           face                 face pair                 vertex\n");

    for (i=2; i<=7; i++)
    {
	printf("%2d :", i);
	pval[pc++] = achi(2*i, &(face[i][0]));
	pval[pc++] = achi(4*i*i, &(fpair[i][0]));
	pval[pc++] = achi(1<<i, &(vert[i][0]));
	putchar('\n');
    }
    printf("%2d :", 60);
    pval[pc] = achi(120, &(face[8][0]));

    printf("\n\nOne sided P value (very small numbers are bad)\n");
    printf("P = %.3g\n", pcombo(pval, 19));
}

static void
init(NOARGS)
{
    int i, j;

    for (i=0; i<8; i++) inpair[i]= -1;
    for (i=0; i<9; i++) for (j=0; j<60; j++) loc[i][j]=0.0;
    for (i=0; i<9; i++) for (j=0; j<120; j++) face[i][j]=0;
    for (i=0; i<8; i++) for (j=0; j<196; j++) fpair[i][j]=0;
    for (i=0; i<8; i++) for (j=0; j<128; j++) vert[i][j]=0;
}

static void
doface(int ind, int bs, int other)
{
    double bt, t;
    int b, j;

    b = (loc[ind][0]>0.0); bt=fabs(loc[ind][0]);
    for (j=1; j<bs; j++)
    {
	t=fabs(loc[ind][j]);
	if (t>bt) {bt=t; b = 2*j | (loc[ind][j]>0.0);}
    }

    face[ind][b]++;

    if (other)
    {
	if (inpair[ind]<0) inpair[ind]=b;
	else
	{
	    fpair[ind][inpair[ind]*bs*2+b]++;
	    inpair[ind]= -1;
	}
    }
}

static void
dovert(int ind, int bs)
{
    int i, j;

    i=0;
    for (j=0; j<bs; j++)
    {
	i<<=1;
	i |= (loc[ind][j] > 0.0);
    }

    vert[ind][i]++;
}

static void
dopoint(double *v, int ind, int bs, int other)
{
    double t;
    int out=0, j;

    for (j=0; j<bs; j++)
    {
	t=loc[ind][j]+v[j]-0.5; if (fabs(t)>1.0) out=1;
	loc[ind][j]=t;
    }

    if (out)
    {
	doface(ind, bs, other);
	if (other) dovert(ind, bs);
	for (j=0; j<bs; j++) loc[ind][j]=0.0;
    }
}

static void
donblock(double v[420], int ind, int bs, int other)
{
    int i;

    for (i=0; i<420; i+=bs) dopoint(v+i, ind, bs, other);
}

static void
doblock(double v[420])
{
    int i;

    for (i=2; i<=7; i++) donblock(v, i, i, 1);
    donblock(v, 8, 60, 0);
}

static void
dodata(int64_t n)
{
    double v[420];
    int64_t i=0;

    init();

    do
    {
	if (fread(v, sizeof(double), 420, stdin)!=420) break;
	doblock(v);
	i++;
    } while (n<0 || i<n);

    if (n>=0 && i<n)
    {
	fprintf(stderr, "warning expected %.0f numbers, saw only %.0f\n",
	    420.0*n, 420.0*i);
	seterr(1);
    }

    doan();
}

int
main(int argc, char **argv)
{
    double count;

    if (argc<2 || sscanf(argv[1], "%lf", &count)!=1) count= -420;

    dodata((int64_t)count/420);

    return errorlevel;
}
