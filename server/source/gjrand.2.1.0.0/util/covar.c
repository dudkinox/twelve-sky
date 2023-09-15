/* Miscellaneous stuff for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define BC 500
#define TR 1600

static void
crash(char *s)
{
    fprintf(stderr, "crash [%s]\n", s); exit(1);
}

static
unsigned rnd(void)
{
    static unsigned s=0;
    unsigned r=(s+1)*987653421;
    s=r;
    return r>>12;
}

static unsigned short ct[1<<20];

static unsigned char buf[4096];

static int
block(int idx)
{
    int r=idx;
    int i;

    i= -4096;
    do
    {
	r= ((r<<1)|((buf+4096)[i]&1)) & ((1<<20)-1);
	ct[r]++;
	i++;
    } while (i<0);

    return r;
}

static double
chi(void)
{
    double r=0.0, e, x;
    int idx=rnd();
    int i;

    memset(ct, 0, sizeof(ct));

    for (i=0; i<BC; i++)
    {
	if (fread(buf, 1, 4096, stdin)!=4096) crash("fread");
	idx=block(idx);
    }

    e=BC*4096.0/(1<<20);
    for (i=0; i<(1<<20); i++)
    {
	x=ct[i]-e;
	r+=x*x;
    }

    return r/e;
}

static double res[TR];

int
main(int argc, char ** argv)
{
    double m=(1<<20)-1;
    double s=sqrt(2.0*m);
    double sum=0.0, ss=0.0, x;
    int i;

    for (i=0; i<TR; i++) res[i]=(chi()-m)/s;

    for (i=0; i<TR; i++) sum+=res[i];
    sum/=TR;
    for (i=0; i<TR; i++)
    {
	x=res[i]-sum;
	ss+=x*x;
    }
    ss/=TR;
    ss=sqrt(ss);

    printf("mean = %15.10f\n", sum);
    printf("  sd = %15.10f\n", ss);

    return 0;
}
