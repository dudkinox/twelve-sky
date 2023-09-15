/* Test software for gjrand random numbers version 1.3.3.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../../src/my_int_types.h"
#include "../../testcommon/pcombo.h"
#include "../../testcommon/chi2p.h"

static int errorlevel=0;
static void
seterr(int e) {if (errorlevel<e) errorlevel=e;}

static void
crash(char *s)
{
    fflush(stdout);
    seterr(1); fprintf(stderr, "crash [%s]\n", s); exit(errorlevel);
}

#define ITER 11
#define DIM 3
#define SIZ1 (1<<4)
#define SIZ (SIZ1*SIZ1*SIZ1)

struct res {double x[DIM]; uint64_t n;};
static struct res res[ITER][SIZ];

static void
init(void)
{
    int i, j, k;

    for (j=0; j<ITER; j++) for (i=0; i<SIZ; i++)
    {
	struct res *p = &(res[j][i]);
	for (k=0; k<DIM; k++) p->x[k] = -1.0;
	p->n = 0;
    }
}

#define makeint(x) floor(x)

static void
dopoint(double v[DIM])
{
    double x[DIM], y[DIM];
    double t, u;
    int i, j, k;
    struct res *ptr;

    i = 0;
    for (j=0; j<DIM; j++)
    {
	i *= SIZ1;
	t = v[j]*SIZ1;
	u = makeint(t);
	x[j] = t-u;
	i += ((int)u) & (SIZ1-1);
    }

    k = 0;
    while (1)
    {
	ptr = &(res[k][i]);
	ptr->n++;
	if (k>=ITER-1) break;
	if (ptr->x[0] < 0.0)
	{
	    for (j=0; j<DIM; j++) ptr->x[j] = x[j];
	    break;
	}

	i = 0;
	for (j=0; j<DIM; j++)
	{
	    i *= SIZ1;
	    t = (x[j]-ptr->x[j])*SIZ1;
	    u = makeint(t);
	    y[j] = t-u;
	    i += ((int)u) & (SIZ1-1);
	}

	t = 1.0; u = 1.0;
	for (j=0; j<DIM; j++) {t *= x[j]; u *= ptr->x[j];}
	if (t>u) for (j=0; j<DIM; j++) ptr->x[j] = x[j];

	for (j=0; j<DIM; j++) x[j] = y[j];

	k++;
    }
}

static void
doan(void)
{
    double pval[ITER], p, e, t;
    uint64_t tot;
    struct res *ptr;
    int j, k;

    for (j=0; j<ITER; j++)
    {
	tot = 0;
	ptr = &(res[j][0]);
	for (k=0; k<SIZ; k++) tot += ptr[k].n;
	if (tot < 10*SIZ) break;

	e = tot/((double)SIZ);
	p = 0.0;
	for (k=0; k<SIZ; k++)
	{
	    t = ptr[k].n-e;
	    p += t*t;
	}
	p /= e;
	printf("chis = %10.0f", p);
	p = chi2p2(p, SIZ-1);
	printf("  (%.6g)\n", p);
	pval[j]=p;
    }

    if (j==0) crash("not enough data, try with more");

    printf("\nP = %.3g\n", pcombo(pval, j));
}

static void
dodata(int64_t n)
{
    double v[DIM];
    int64_t i=0;

    init();

    do
    {
	if (fread(v, sizeof(double), DIM, stdin)!=DIM) break;
	dopoint(v);
	i++;
    } while (n<0 || i<n);

    if (n>=0 && i<n)
    {
	fprintf(stderr, "warning expected %.0f points, saw only %.0f\n",
		(double)n, (double)i);
	seterr(1);
    }

    doan();
}

int
main(int argc, char **argv)
{
    double count;

    if (argc<2 || sscanf(argv[1], "%lf", &count)!=1) count= -DIM;

    dodata((int64_t)count/DIM);

    return errorlevel;
}
