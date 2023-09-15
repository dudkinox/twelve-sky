/* Test software for gjrand random numbers version 1.3.3.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../../src/my_int_types.h"
#include "../../testcommon/pcombo.h"
#include "../../testcommon/chi2p.h"

/* Take the numbers as fixed point and look at groups of 8 bits to the */
/* right of the binary point. Do chi-squared tests. */
/* Not very powerful, but perhaps useful as a basic sanity check. */
/* The randoms need at least 48 bits absolute of precision to pass. */

static int errorlevel=0;
static void seterr(int e) {if (errorlevel<e) errorlevel=e;}

int64_t count[6][256];

void
doan(int64_t pos)
{
    double pvals[6];
    double e=(double)pos/256.0, t, x;
    int i, j;

    for (j=0; j<6; j++)
    {
	t=0.0;
	for (i=0; i<256; i++)
	{
	    x=(double)count[j][i]-e;
	    t+=x*x/e;
	}
	printf("%15.5f\n", t);

	pvals[j] = chi2p2(t, 255);
    }

    printf("\nOne sided P value (very small numbers are bad)\n");
    printf("P = %.3g\n", pcombo(pvals,6));
}

void
readstuff(int64_t max)
{
    double f, x, buf[1024];
    int64_t pos=0;
    int i, j;

    for (j=0; j<6; j++) for (i=0; i<256; i++) count[j][i]=0;

    while (1)
    {
	i=fread(buf, sizeof(double), 1024, stdin);
	if (i<=0) break;
	if (max>0 && max<pos+i) i=max-pos;
	pos+=i;
	i--;
	do
	{
	    x=buf[i];
	    for (j=0; j<6; j++)
	    {
		x*=256.0;
		f=floor(x);
		x-=f;
		count[j][((int)f)&255]++;
	    }
	    i--;
	} while (i>=0);
	if (max>0 && pos>=max) break;
    }

    if (max>0 && pos<max)
    {
	fprintf(stderr, "Warning, expected %.0f samples, saw only %.0f\n",
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
    double dm;
    int64_t max;

    if (argc<2 || sscanf(argv[1], "%lf", &dm)!=1 || dm<1) dm= -1;
    max=(int64_t)dm;

    printf("expected range [ 191 205 227 284 310 331 ]\n");

    readstuff(max);

    return errorlevel;
}
