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
    fputs("usage:\n"
"blatv blocksize\n"
"blatv blocksize count\n"
"blatv blocksize count integer_seed\n"
"Writes normal variates to standard output in (unportable) binary format.\n"
"Without count, or negative count, writes forever.\n",
	stderr);

    exit(1);
}

void
crash(const char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

int
main(int argc, char **argv)
{
    int blocksize;
    long seed;
    int64_t count= -1;
    struct gjrand g;

    if (argc<2) usage();

    if (sscanf(argv[1], "%d", &blocksize)!=1) usage();
    if (blocksize<1 || blocksize>2048)
	crash("blocksize must be in the range 1 to 2048");

    if (argc>2 && strcmp(argv[2], "inf")!=0)
    {
	double t;
	if (sscanf(argv[2], "%lf", &t)!=1) usage();
	count=(int64_t)t;
    }

    if (argc>3)
    {
	if (sscanf(argv[3], "%ld", &seed)!=1) usage();
	gjrand_init(&g, seed);
    }
    else gjrand_initrand(&g);

    while (count)
    {
	double b[2048];
	int c=blocksize;
	if (count>=0 && c>count) c=count;

	gjrand_normalv(&g, c, b);
	if (fwrite(b, sizeof(double), c, stdout)!=(size_t)c)
	    crash("fwrite");

	if (count>0) count-=c;
    }

    return 0;
}
