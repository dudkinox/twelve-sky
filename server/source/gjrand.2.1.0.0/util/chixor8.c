/* Miscellaneous stuff for gjrand random numbers version 1.3.3.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../src/my_int_types.h"

/* chi square test on 8 bit values. */
/* This one actually tests a byte XOR'd with an earlier byte for */
/* all lags 1 to 16. */

/* Here we are experimenting to get a one-sided P value that */
/* summarizes the whole test. The 16 different chi-squared tests */
/* probably correlate with each other in various small ways. */

/* Add sum of squares of deviations from 255.0 */

/* We do many tests on fixed size blocks. */

int32_t scount[16][256];

#define BSIZ (8*1024-16)
#define COUNT 1000000

double result[COUNT];
int rcount=0;

void
doan(void)
{
    double e=BSIZ/256.0, t, gt=0.0, x;
    int i, j;

    for (j=0; j<16; j++)
    {
	t=0.0;
	for (i=0; i<256; i++)
	{
	    x=scount[j][i]-e;
	    t+=x*x/e;
	}
	t-=255.0;
	gt+=t*t;
    }

    result[rcount]=gt;
}

int
dcmp(const void *a, const void *b)
{
    const double *da=a, *db=b;

    if (*da > *db) return 1;
    return -1;
}

void
ntile(double n)
{
    printf("%10.5f : %10.5f\n", n, result[(int)(COUNT*n)]);
}

void
final(void)
{
   double sum=0.0, ss=0.0, s3=0.0, scale;
   int i;

   scale=1.0/(510.0);

   for (i=0; i<COUNT; i++) result[i] *= scale;

   for (i=0; i<COUNT; i++) sum+=result[i];
   sum/=COUNT;
   for (i=0; i<COUNT; i++) ss+=(result[i]-sum)*(result[i]-sum);
   ss=sqrt(ss/COUNT);

    for (i=0; i<COUNT; i++)
    {
	double t=result[i];
	t -= sum;
	t /= ss;
	s3 += t*t*t;
    }
    s3 /= COUNT;

    printf("mean = %10.5f\n", sum);
    printf("  sd = %10.5f\n", ss);
    printf("skew = %10.5f\n\n", s3);

    qsort(result, COUNT, sizeof(double), dcmp);

    ntile(0.001);
    ntile(0.01);
    ntile(0.1);
    ntile(0.5);
    ntile(0.8);
    ntile(0.9);
    ntile(0.95);
    ntile(0.99);
    ntile(0.995);
    ntile(0.999);
}

void
readstuff(void)
{
    static unsigned char buf_b[16+BSIZ];
    unsigned char * const buf=buf_b+16;
    int i, k;

    memset(scount, 0, sizeof(scount));

    if (fread(buf_b, 1, 16+BSIZ, stdin)!=16+BSIZ)
    {
	fprintf(stderr,"Unexpected EOF\n");
	exit(1);
    }

    {
	i=BSIZ-1;
	do
	{
	    k=buf[i];
#define CINC(x) scount[x][(buf-x-1)[i]^k]++
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
    }

    doan();
}

int
main(int argc, char **argv)
{
    for (rcount=0; rcount<COUNT; rcount++) readstuff();
    final();
    return 0;
}
