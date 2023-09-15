/* Test software for gjrand random numbers version 1.0.0.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "../src/my_int_types.h"

#ifdef __cplusplus
#define NOARGS
#else
#define NOARGS void
#endif

#if defined(__cplusplus) || defined(__GNUC__)
#define INLINE inline
#else
#define INLINE
#endif

#include "../src/gjrand.h"

void
usage(NOARGS)
{
    fprintf(stderr, "usage:\n"
"blatnorm\n"
"blatnorm count\n"
"blatnorm count integer_seed\n"
"Writes normal variates to standard output in (unportable) binary format.\n"
"Without count, or negative count, writes forever.\n");

    exit(1);
}

int
main(int argc, char **argv)
{
    long seed;
    int64_t count= -1;
    struct gjrand g;

    if (argc>1 && strcmp(argv[1], "inf")!=0)
    {
	double t;
	if (sscanf(argv[1], "%lf", &t)!=1) usage();
	count=(int64_t)t;
    }

    if (argc>2)
    {
	if (sscanf(argv[2], "%ld", &seed)!=1) usage();
	gjrand_init(&g, seed);
    }
    else gjrand_initrand(&g);

    while (count)
    {
	double b[1024];
	int i, c=1024;
	if (count>=0 && c>count) c=count;

	for (i=0; i<c; i++) b[i]=gjrand_normal(&g);
	if (fwrite(b, sizeof(double), c, stdout)!=(size_t)c)
	{
	    if (count>0) {fputs("crash fwrite\n", stderr); exit(1);}
	    return 0;
	}

	if (count>0) count-=c;
    }

    return 0;
}
