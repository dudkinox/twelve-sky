/* Test software for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* Binary rank order of bit matrices. This one is inspired by similar */
/* tests in DIEHARD. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../../src/my_int_types.h"

typedef uint64_t row_t;
#define WORDS 2
#define BITS 64
#define WBITS (WORDS*BITS)
#define HIGHBIT (((row_t)1)<<(BITS-1))
#define BLOCKSIZE ((sizeof(row_t))*WORDS*WBITS)

static int errorlevel=0;
static void seterr(int e) {if (errorlevel<e) errorlevel=e;}

/* Return binary rank order of the bit matrix m. */
/* m is modified by the calculation. */
static int
binrnk(row_t m[WBITS+1][WORDS])
{
    row_t bit, t;
    int i, j, k, l, r;

    r=0;
    for (i=0; i<WORDS; i++) m[WBITS][i] = -((row_t)1); /* sentinal */

    for (i=0; i<WORDS; i++) for (bit=HIGHBIT; bit; bit>>=1)
    {
	/* find a pivot with non-zero in the column masked by bit. */
	j=r-1;
	do {j++;} while (! (m[j][i]&bit));

	if (j==WBITS) continue; /* no pivot. do not incr rank. */

	/* exchange pivot to top */
	for (k=0; k<WORDS; k++) {t=m[r][k]; m[r][k]=m[j][k]; m[j][k]=t;}

	/* subtract pivot row from lower rows to zero the column. */
	for (k=0; k<WORDS; k++)
	{
	    t=m[r][k];
	    for (l=r+1; l<WBITS; l++) m[l][k] ^= t;
	}

	r++;
    }

    return r;
}

void
readstuff(int64_t max)
{
    int64_t histo[WBITS+1];
    row_t m[WBITS+1][WORDS];
    int64_t p=0;
    int i;

    memset(histo, 0, sizeof(histo));

    while (max<0 || p<max)
    {
	if (fread(m, sizeof(row_t), WORDS*WBITS, stdin)!=WORDS*WBITS) break;
	histo[binrnk(m)]++;
	p++;
    }

    if (max>0 && p<max)
    {
	fprintf(stderr, "Warning, expected %.0f matrices, saw only %.0f\n",
		(double)max, (double)p);
	seterr(1);
    }

    for (i=0; i<=WBITS; i++) if (histo[i])
	printf("%3d: %15.0f\n", i, (double)(histo[i]));
}

int
main(int argc, char **argv)
{
    double dmax;
    int64_t max;

    if (argc<2 || sscanf(argv[1], "%lf", &dmax)!=1 || dmax<1.0) max= -1;
    else max=dmax;

    if (max>0) max/=BLOCKSIZE;

    readstuff(max);

    return errorlevel;
}
