/* Test software for gjrand random numbers version 2.0.0.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#ifndef GJRAND_INTEGRATE_H
#define GJRAND_INTEGRATE_H 1

/* Numerically integrate f from lo to hi. Romberg's method. */
/* Relies on f being "well-behaved". */

static double
gjrt_integrate(double (*f)(double x), double lo, double hi, int l)
{
    double pt[65];
    double app1[7], app2[7], *in=app1, *out=app2, *tp;
    double sc, t, m1, m2;
    int j, k, c, s;

    if (l>6) l = 6; if (l<0) l = 0;

    c = 1<<l;
    sc = (hi-lo)/c;

    for (j=c; j>=0; j--) pt[j] = (*f)(j*sc+lo);
    pt[0] = (pt[c]+pt[0]) * 0.5;

    s = c;
    for (j=0; j<=l; j++)
    {
	t = 0.0;
	for (k=0; k<c; k+=s) t += pt[k];
	in[j] = t * s / c;
	s >>= 1;
    }

    sc = 4.0;
    while (l>0)
    {
	m2 = 1.0/(sc-1.0); m1 = sc * m2;
	for (j=0; j<l; j++)
	    out[j] = in[j+1]*m1 - in[j]*m2;
	tp = in; in = out; out = tp;
	sc *= 4.0;
	l--;
    }

    return fabs(hi-lo) * in[0];
}

#endif /* GJRAND_INTEGRATE_H */
