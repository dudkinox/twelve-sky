/* Test software for gjrand random numbers version 1.3.3.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../src/gjrand.h"
#include "../testcommon/chi2p.h"

/* Tests for gjrand_coins and gjrand_biased_coins . */
/* First arg probability of heads, or -f for fair. */
/* Second arg number of coins per trial */
/* Third arg number of trials */
/* Optional fourth arg random seed. */

#define MAX 1000000
#define STRMAX "1000000"
static long histo[MAX+5];

static void
crash(const char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

static void
docoins(int coins, long count, struct gjrand *s)
{
    int i;
    for (i=0; i<=coins; i++) histo[i]=0;
    do
    {
	histo[gjrand_coins(s, coins)]++;
	count--;
    } while (count>0);
}

static void
dobiased(double prob, int coins, long count, struct gjrand *s)
{
    int i;
    for (i=0; i<=coins; i++) histo[i]=0;
    do
    {
	histo[gjrand_biased_coins(s, prob, coins)]++;
	count--;
    } while (count>0);
}

static double ex1[MAX+5], ex2[MAX+5];

static double *
mkexpect(double prob, int coins, long count)
{
    double *x1=ex1, *x2=ex2, *tmp;
    double p2=1.0-prob; /* probability of tails */
    int i, j;
    int hi=0, lo=0;

    x1[0]=count; /* with zero coins thrown, zero heads always results */
    for (i=1; i<=coins; i++) x1[i]=x2[i]=0.0;

    for (i=1; i<=coins; i++)
    {
	x2[0]=0.0;

	for (j=lo; j<=hi; j++) {x2[j]+=x1[j]*p2; x2[j+1]=x1[j]*prob;}
	hi++;
	for (j=lo; j<=hi; j++) x1[j]=0.0;
	while (lo<hi && x2[lo]<1e-20) lo++;
	while (hi>lo && x2[hi]<1e-20) hi--;

	tmp=x1; x1=x2; x2=tmp;
    }

    return x1;
}

static void
moments(double *ex, int coins, long count)
{
    double mx, mh, dx, dh, sx, sh, tx, msig, dsig, ssig;
    int i;

    printf("Moments\n%1s %20s %20s %20s\n",
	"n", "expect", "observe", "diff/sigma");

    /* means */
    mx=0.0; mh=0.0;
    for (i=0; i<=coins; i++)
    {
	mx+=ex[i]*i;
	mh+=histo[i]*(double)i;
    }
    mx/=count; mh/=count;

    /* standard deviations */
    dx=0.0; dh=0.0;
    for (i=0; i<=coins; i++)
    {
	dx+=ex[i]*(i-mx)*(i-mx);
	dh+=histo[i]*(i-mx)*(i-mx);
    }
    dx=sqrt(dx/count); dh=sqrt(dh/count);

    /* expected standard deviation of the mean for this sample size. */
    tx=dx/sqrt((double)count);

    /* discrepency of mean in units of sigma. */
    msig=(mh-mx)/tx;

    /* expected standard deviation of the variance for this sample size. */
    tx=0;
    for (i=0; i<=coins; i++)
    {
	double t=i-mx;
	t*=t;
	t-=dx*dx;
	t*=t;
	tx+=ex[i]*t;
    }
    tx=sqrt(tx)/count;

    /* discrepency of variance in units of sigma. */
    dsig=(dh*dh-dx*dx)/tx;

    /* skew */
    sx=0.0; sh=0.0;
    for (i=0; i<=coins; i++)
    {
	sx+=ex[i]*(i-mx)*(i-mx)*(i-mx);
	sh+=histo[i]*(i-mx)*(i-mx)*(i-mx);
    }
    sx/=count; sh/=count;

    /* expected standard deviation of the skew for this sample size. */
    tx=0;
    for (i=0; i<=coins; i++)
    {
	double t=i-mx;
	t*=t*t;
	t-=sx;
	t*=t;
	tx+=ex[i]*t;
    }
    tx=sqrt(tx)/count;

    /* discrepency of skew in units of sigma. */
    ssig=(sh-sx)/tx;

/* HERE this is probably not the right way to normalise skew */
    sx/=dx*dx*dx; sh/=dh*dh*dh;

    printf("%1d %20.5f %20.5f %20.5f\n", 1, mx, mh, msig);
    printf("%1d %20.5f %20.5f %20.5f\n", 2, dx, dh, dsig);
    printf("%1d %20.5f %20.5f %20.5f\n", 3, sx, sh, ssig);
}

static void
doan(double prob, int coins, long count)
{
    int i;
    double *ex, r=0.0, p;
    int peak=(int)floor(coins*prob+0.5), start, end, df;

    ex=mkexpect(prob, coins, count);
    ex[coins+1]=0.0;

    if (ex[peak]<10.0) crash(
"not enough data for useful chi-squared, try again with more trials");

    start=peak;
    while (start>0 && ex[start-1]>=10.0) start--;
    end=peak+1;
    while (end<=coins && ex[end]>=10.0) end++;
    df=end-start-1;
    if (start>0)
    {
	double e=0.0;
	long o=0;

	df++;
	for (i=0; i<start; i++) {e+=ex[i]; o+=histo[i];}
	r+=(e-o)*(e-o)/e;
    }
    if (end<=coins)
    {
	double e=0.0;
	long o=0;

	df++;
	for (i=end; i<=coins; i++) {e+=ex[i]; o+=histo[i];}
	r+=(e-o)*(e-o)/e;
    }
    for (i=start; i<end; i++)
    {
	double e=ex[i];
	long o=histo[i];
	r+=(e-o)*(e-o)/e;
    }

    p = chi2p2(r, df);
    printf("chis = %.5f    df = %d    P = %.3g\n\n", r, df, p);

    moments(ex, coins, count);
}

static void
usage(void)
{
    crash(
"3 or 4 args: prob coins trials [ seed ]\n"
"prob is -f for fair, or a number between 0 and 1\n"
"coins is number of coins to throw per trial, between 1 and "
STRMAX
"\n"
	);
}

int
main(int argc, char **argv)
{
    long count, seed;
    struct gjrand s;
    int fair=0, coins;
    double prob;

    if (argc<4) usage();

    if (strcmp(argv[1], "-f")==0) {fair=1; prob=0.5;}
    else if (sscanf(argv[1], "%lf", &prob)==1 && prob>0.0 && prob<1.0) fair=0;
    else usage();

    if (sscanf(argv[2], "%d", &coins)!=1 || coins<1 || coins>MAX) usage();

    if (sscanf(argv[3], "%ld", &count)!=1 || count<1) usage();

    if (argc>4)
    {
	if (sscanf(argv[4], "%ld", &seed)!=1) crash("seed not a number");
	gjrand_init(&s, seed);
    }
    else gjrand_initrand(&s);

    if (fair) docoins(coins, count, &s);
    else dobiased(prob, coins, count, &s);

    doan(prob, coins, count);

    return 0;
}
