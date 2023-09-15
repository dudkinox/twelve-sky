/* Miscellaneous stuff for gjrand random numbers version 2.0.0.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include "integ2.h"

static double
logf1(double c)
{
    double r=0.0, j;

    if (c<=1.0) return r;

    for (j=2.0; j<=c; j++) r+=log(j);
    j--;
    r+=log(c)*(c-j);

    return r;
}

static double save_x, logfac_x, log_p, log_1mp;

static double
stirling1(double c)
{
    if (c<=1.0) return 0.0;
    /* Mysterious constant : log(2*PI)/2 */
    return log(c)*(c+0.5)+(1.0/12.0)/c-c+0.9189385332046727418;
}

static double
logfac(double c)
{
    double cr, r=0.0;
    if (c>19.5) r=stirling1(c);
    else
    {
	cr=floor(c+0.5);
	r=logf1(cr);
	if (cr!=c) {r-=stirling1(cr); r+=stirling1(c);}
    }

    return r;
}

static void
initxp(double x, double p)
{
    save_x = x;
    logfac_x = logfac(x);
    log_p = log(p);
    log_1mp = log(1.0-p);
}

static double
bino(double y)
{
    return exp(logfac_x - logfac(save_x-y) - logfac(y) +
		log_1mp*(save_x-y) + log_p*y);
}

/* what is the probability that throwing x coins (probability p of heads) */
/* you get a result further from expected than y? */
/* Both surprisingly low and high y makes a small P-value. */
/* Not very accurate? */
static double
sumbino(int64_t x, int64_t y, double p)
{
    double r=0.0, z, incr, tiny;
    int64_t j;

    if (p<=0.0) {if (y<=0) r = 1.0; goto ret;}
    if (p>=1.0) {if (y>=x) r = 1.0; goto ret;}
    if (y<0) goto ret;
    if (y>x) goto ret;

    z = p*x;
    if (y>z) {z = x-z; y = x-y; p = 1.0-p;}

    initxp(x, p);

    if (2.0*z>x) z = x-z;
    incr = sqrt(z);

    if (incr>60.0) r = gjrt_integ2(bino, y, -incr, 1.0e-5, 4) * 2.0;
    else
    {
	r = bino((double)y);
	tiny = r * 1.0e-6;
	for (j=y-1; j>=0; j--)
	{
	    z = bino((double)j);
	    r += z*2.0;
	    if (z < tiny) break;
	}
    }

    ret:
    if (r>1.0) r = 2.0-r;
    return r;
}
