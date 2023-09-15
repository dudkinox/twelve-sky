/* Test software for gjrand random numbers version 1.3.3.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../../src/my_int_types.h"
#include "../../testcommon/chi2p.h"

/* chi square test on 16 bit values. */

static int errorlevel=0;
static void seterr(int e) {if (errorlevel<e) errorlevel=e;}

static uint64_t count[65536];

static void
doan(uint64_t pos)
{
    double e=(double)pos/65536.0, t=0.0, x, p;
    int i;

    for (i=0; i<65536; i++)
    {
	x=(double)count[i]-e;
	t+=x*x/e;
    }

    printf("%15.5f\n\n", (double)t);

    p = chi2p2(t, 65535);

    printf("One sided P value (very small numbers are bad)\n");
    printf("P = %.3g\n", p);
}

static void
readstuff(int64_t max)
{
    unsigned short buf[4096];
    int64_t pos=0;
    int i;

    for (i=0; i<65536; i++) count[i]=0;

    while (1)
    {
	i=fread(buf, 2, 4096, stdin);
	if (i<=0) break;
	if (max>0 && max<pos+i) i=max-pos;
	pos+=i;
	i--;
	do {count[buf[i]]++; i--;} while (i>=0);
	if (max>0 && pos>=max) break;
    }

    if (max>0 && pos<max)
    {
	fprintf(stderr,
		"Warning, expected %.0f 16 bit samples, saw only %.0f\n",
		(double)max, (double)pos);
	seterr(1);
    }

    if (pos<500000)
    {
fprintf(stderr, "Warning, < 500000 16 bit words, don't trust result.\n");
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
    else max=(int64_t)(0.5*dmax);

    printf("expected range [ 64416 64693 65071 65999 66377 66654 ]\n");

    readstuff(max);

    return errorlevel;
}
