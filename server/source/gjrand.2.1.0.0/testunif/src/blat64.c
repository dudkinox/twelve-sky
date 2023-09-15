/* Test software for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "../../src/gjrand.h"

/* This one writes a file for use by several tests in this directory. */
/* This does the same as blatrand but uses gjrand_rand64() */

void
crash(char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

void
blat(struct gjrand *g, int64_t s)
{
    uint64_t buf[512];

    while (s)
    {
	int c;
	int i;

	for (i=0; i<512; i++) buf[i]=gjrand_rand64(g);

	if ((uint64_t)s>4096) c=4096; else c=s;
	if (fwrite(buf, 1, c, stdout)!=(size_t)c)
	{
	    if (s>=0) crash("fwrite fails");
	    break;
	}

	if (s>=0) s-=c;
    }
}

int
main(int argc, char **argv)
{
    unsigned long seed;
    double ds;
    int64_t s= -1;
    struct gjrand g;

    if (argc>2) {sscanf(argv[2], "%lu", &seed); gjrand_init(&g, seed);}
    else gjrand_initrand(&g);
    if (argc<2) crash("give a size arg in bytes (or 'inf')");

    if (strcmp(argv[1],"inf")==0) s= -1;
    else if (sscanf(argv[1], "%lf", &ds)==1) s=(int64_t)ds;
    else crash("size arg not a number (or inf)");

    blat(&g, s);

    return 0;
}
