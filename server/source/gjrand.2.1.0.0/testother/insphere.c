/* Test software for gjrand random numbers version 2.1.0.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../src/gjrand.h"

#define MAX 100000

void
crash(const char *s)
{
    fprintf(stderr,"crash [%s]\n", s);
    exit(1);
}

void
test(int dim, unsigned long count, struct gjrand *s)
{
    double point[MAX];
    double mean[MAX], msq[MAX];
    unsigned long c;
    int i;
    double hi= -1e30, lo=1e30;


    for (i=0; i<MAX; i++) mean[i]=msq[i]=0.0;

    c=count;
    do
    {
	double y=0.0;
	gjrand_insphere(s, dim, point);
	i=dim-1;
	do
	{
	    double x=point[i];
	    mean[i]+=x;
	    x*=x;
	    msq[i]+=x;
	    y+=x;
	    i--;
	} while (i>=0);

	if (y>hi) hi=y; if (y<lo) lo=y;

	c--;
    } while (c);

    printf("radius max = %.10f   min = %.10f\n\n", sqrt(hi),sqrt(lo));
    for (i=0; i<dim; i++)
	printf("%d  %15.12f  %15.12f\n",i,mean[i]/count,msq[i]/count);
}

int
main(int argc, char **argv)
{
    struct gjrand s;
    unsigned long seed, count;
    int dim;

    if (argc!=4 && argc!=3) crash("usage: insphere dim count [ seed ]");

    if (sscanf(argv[1],"%d",&dim)!=1 || dim<=0)
	crash("first arg dim should be a positive integer");
    if (dim>=MAX) crash("first arg dim too big");
    if (sscanf(argv[2],"%lu",&count)!=1 || count<=0)
	crash("second arg count should be an integer > 0");
    if (argc==4)
    {
	if (sscanf(argv[3],"%lu",&seed)!=1)
	    crash("third arg seed should be an integer");
	gjrand_init(&s, seed);
    }
    else gjrand_initrand(&s);

    test(dim, count, &s);

    return 0;
}
