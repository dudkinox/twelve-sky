/* Miscellaneous stuff for gjrand random numbers version 1.3.3.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* calculate calibrations for ../testunif/metachi */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../src/my_int_types.h"

// typedef long double myfloat;
typedef double myfloat;

void
crash(char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
}

myfloat prob[40];

/* n=number of remaining slots in 1K buffer */
/* p=number of remaining numbers */
void
mkprob(int n, int p)
{
    myfloat pr1[40], pr2[40], *p1=pr1, *p2=pr2, *t;
    myfloat psh=1.0/p, plo=1.0-psh;
    int i, j;

    p1[0]=1.0; for (i=1; i<40; i++) p1[i]=0.0;

    for (i=0; i<n; i++)
    {
	p2[0]=p1[0]*plo;
	for (j=1; j<39; j++) p2[j] = p1[j-1]*psh + p1[j]*plo;
	p2[39] = p1[38]*psh + p1[39];
	t=p1; p1=p2; p2=t;
    }

    for (i=0; i<40; i++) prob[i]=p1[i];
}


/* [i][j]: */
/* i=number of remaining slots in 1K buffer */
/* j=sum of squares so far. */
/* data=probability */

myfloat s1[1025][2560], s2[1025][2560];
int sd[40];

/* p=number of remaining numbers, s=incoming prob array, t=outgoing */

void
trans(const int p, myfloat s[1025][2560], myfloat t[1025][2560])
{
    myfloat x;
    int i, j, k, m, n;

    for (i=0; i<1025; i++) for (j=0; j<2560; j++) t[i][j]=0.0;

    for (i=0; i<1025; i++)
    {
	mkprob(i, p);
	for (j=0; j<2560; j++) if ((x=s[i][j])>1.0e-30)
	    for (k=0; k<40; k++)
	    {
		m=i-k; if (m<0) m=0;
		n=sd[k]+j; if (n>=2560) n=2559;
		t[m][n] += prob[k] * x;
	    }
    }
}

void
build(void)
{
    int i, j;

    for (i=0; i<40; i++) sd[i]=(i-4)*(i-4);

    for (i=0; i<1025; i++) for (j=0; j<2560; j++) s1[i][j]=0.0;
    s1[1024][0]=1.0;

    for (i=256; i>0; i-=2)
    {
	trans(i, s1, s2);
	trans(i-1, s2, s1);
	fprintf(stderr, "%d\n", i);
    }
}

void
dostats(myfloat d[2560])
{
    myfloat xprob[12];
    int xidx[15];

    myfloat x, y, s, ss, ssv, s4, s4v, tot;
    int i, j, k, n;

    tot=0.0;
    for (i=0; i<2560; i++) tot+=d[i];
    printf("(should be 1.0) %20.17f\n", (double)tot);

    s=0.0;
    for (i=0; i<2560; i++) s+=d[i]*i*0.25;
    printf("\n====================\n\nmean = %22.17f\n", (double)s);

    ss=0.0;
    for (i=0; i<2560; i++)
    {
	x=i*0.25-s;
	ss+=x*x*d[i];
    }
    printf(" var = %22.17f\n", (double)ss);

    ssv = 0.0;
    for (i=0; i<2560; i++)
    {
	x = i*0.25-s;
	y = x*x - ss;
	ssv += y*y*d[i];
    }
    printf(" sd of var = %22.17f\n", (double)(sqrtl(ssv)));

    s4=0.0;
    for (i=0; i<2560; i++)
    {
	x = i*0.25-s;
	x *= x; x *= x;
	s4+=x*d[i];
    }
    printf(" 4th mom = %22.17f\n", (double)s4);

    s4v = 0.0;
    for (i=0; i<2560; i++)
    {
	x = i*0.25-s;
	x *= x; x *= x;
	y = x - s4;
	s4v += y*y*d[i];
    }
    printf(" sd of 4th mom = %22.17f\n", (double)(sqrtl(s4v)));

    printf("\n==================\n\ndeciles\n");
    j=0;
    for (i=1; i<10; i++)
    {
	x=0.1*i;
	y=0.0;
	for (k=0; y<x; k++)
	{
	    if (k>=2560) crash("array bounds");
	    y+=d[k];
	}
	y=0.0;
	for (n=j; n<k; n++) y+=d[n];
	printf("%4d %22.17f\n", k, (double)y);
	j=k;
    }
    y=0.0;
    for (n=j; n<2560; n++) y+=d[n];
    printf("%4d %22.17f\n", 2560, (double)y);

    printf("\n==================\n\nextremes\n");
    j = 0;
    for (i=0; i<5; i++)
    {
	static const double wanted[5]={3e-4, 1e-3, 3e-3, 1e-2, 3e-2};
	x = wanted[i];
	y = 0.0;
	for (k=0; y<x; k++)
	{
	    if (k>=2560) crash("array bounds");
	    y += d[k];
	}
	xidx[i+1] = k;
	y = 0.0;
	for (n=j; n<k; n++) y += d[n];
	xprob[i] = y;
	j = k;
    }

    j = 2559;
    for (i=0; i<5; i++)
    {
	static const double wanted[5]={3e-4, 1e-3, 3e-3, 1e-2, 3e-2};
	x = wanted[i];
	y = 0.0;
	for (k=2559; y<x; k--)
	{
	    if (k<0) crash("array bounds");
	    y += d[k];
	}
	xidx[10-i] = k;
	y = 0.0;
	for (n=j; n>k; n--) y += d[n];
	xprob[10-i] = y;
	j = k;
    }

    y = 0;
    for (i=0; i<5; i++) {y -= xprob[i]; y -= xprob[10-i];}
    y += 1.0;
    xprob[5] = y;
    xidx[0] = 0;

    for (i=0; i<11; i++)
	printf("%4d %30.17e\n", xidx[i], (double)(xprob[i]));
}

int
main(int argc, char **argv)
{
    myfloat culm[2560];
    myfloat x, acc;
    int j;

    build();

    acc = 0.0;
    for (j=0; j<1020; j++) {acc += s1[0][j]; culm[j] = acc;}
    acc = 0.0;
    for (j=2559; j>=1020; j--) {acc += s1[0][j]; culm[j] = acc;}

    for (j=0; j<2560; j++)
    {
	x=s1[0][j];
	if (x<1.0e-25) continue;
	printf("%8.2f : %30.17e %30.17e\n",
		(double)(0.25*j), (double)x, (double)(culm[j]));
    }

    dostats(&(s1[0][0]));

    return 0;
}
