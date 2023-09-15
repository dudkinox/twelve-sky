/* Test software for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../src/gjrand.h"

/* Basic sanity check for gjrand_shuffle. */
/* This just checks that all the right numbers are there once each. */

#define MAX 3000000

void
crash(const char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

/* return 1 if basic sanity checks all pass, else return 0 */
int
chk(int size, int a[])
{
    static int b[MAX+3];
    int i, j;

    for (i=0; i<size; i++) b[i]=0;

    for (i=0; i<size; i++)
    {
	j=a[i];
	if ((unsigned)j >= (unsigned)size) return 0;
	b[j]++;
    }

    for (i=0; i<size; i++) if (b[i]!=1) return 0;

    return 1;
}

void
doshuffle(int size, int count, struct gjrand *s)
{
    static int a[MAX+3];
    int i, ok=0;

    for (i=0; i<count; i++)
    {
	gjrand_shuffle(s, size, a);
	ok+=chk(size, a);
    }

    printf("%d ok out of %d\n", ok, count);
    if (ok!=count) printf("=== %d BAD !!! ===\n", count-ok);
}

int
main(int argc, char **argv)
{
    int size, count, seed;
    struct gjrand s;

    if (argc<3) crash("2 or 3 args: size count [ seed ]");
    if (sscanf(argv[1], "%d", &size)!=1) crash("size not a number");
    if (sscanf(argv[2], "%d", &count)!=1) crash("count not a number");
    if (size<2 || size>MAX) crash("count out of range");
    if (argc>3)
    {
	if (sscanf(argv[3], "%d", &seed)!=1) crash("seed not a number");
	gjrand_init(&s, seed);
    }
    else gjrand_initrand(&s);

    doshuffle(size, count, &s);

    return 0;
}
