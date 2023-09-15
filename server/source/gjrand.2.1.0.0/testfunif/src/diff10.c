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

#define DIM 10
#define SIZ (1<<14)

struct res {uint16_t x[DIM]; uint64_t n;};
static struct res res[DIM][SIZ];

static void
init(void)
{
    int i, j, k;

    for (j=0; j<DIM; j++) for (i=0; i<SIZ; i++)
    {
	struct res *p = &(res[j][i]);
	for (k=0; k<DIM; k++) p->x[k] = SIZ;
	p->n = 0;
    }
}

#define makeint(x) floor(x)

static void
dopoint(const double v[DIM])
{
    uint16_t x[DIM], y[DIM];
    int i, j, k;
    struct res *ptr;

    for (j=0; j<DIM; j++) x[j] = ((unsigned)(makeint(v[j]*SIZ))) & (SIZ-1);

    for (i=0; i<DIM; i++)
    {
	for (j=i; j<DIM; j++) y[j] = x[j];
	ptr = &(res[i][y[i]]);
	ptr->n++;
	for (j=i; j<DIM; j++) x[j] = (x[j] - ptr->x[j]) & (SIZ-1);
	for (j=i; j<DIM; j++)
	{
	    k = ((int)(y[j])) - ((int)(ptr->x[j]));
	    if (k>0) goto done;
	    if (k<0) goto doit;
	}

	doit:
	for (j=i; j<DIM; j++) ptr->x[j] = y[j];

	done: ;
    }
}

static void
doan(void)
{
    double pval[DIM], p, e, t;
    uint64_t tot;
    struct res *ptr;
    int j, k;

    for (j=0; j<DIM; j++)
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
	printf("order = %d : chis = %10.0f", j, p);
	p = chi2p2(p, SIZ-1);
	printf(" ;  p = %.6g\n", p);
	pval[j]=p;
    }

    if (j==0)
    {
	fprintf(stderr, "blocks = %15.0f ; wanted >= %ld\n",
		(double)tot, 10L*SIZ);
	crash("not enough data, try with more");
    }

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
