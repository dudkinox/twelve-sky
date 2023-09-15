/* Test software for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "../../src/gjrand.h"

/* This one writes a file for use by several tests in this directory. */

/* first arg is block size in 32 bit words. */
/* second arg is required file size in bytes */
/*     or "inf" to write forever. */
/* third optional arg is random seed. */

void
crash(char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

void
blat(struct gjrand *g, unsigned bsize, int64_t s)
{
    int b4=4*bsize;
    uint32_t buf[4096];

    while (s)
    {
	int c;

	gjrand_rand32v(g, bsize, buf);

	if ((uint64_t)s>(uint64_t)b4) c=b4; else c=s;
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
    unsigned bsize;
    unsigned long seed;
    double ds;
    int64_t s= -1;
    struct gjrand g;

    if (argc>3) {sscanf(argv[3], "%lu", &seed); gjrand_init(&g, seed);}
    else gjrand_initrand(&g);
    if (argc<3) crash("two args: block-size size (in bytes or 'inf')");
    if (sscanf(argv[1], "%u", &bsize)!=1) crash("block-size not a number");
    if (bsize<1 || bsize>4096) crash("block-size out of range (1-4096)");

    if (strcmp(argv[2],"inf")==0) s= -1;
    else if (sscanf(argv[2], "%lf", &ds)==1) s=(int64_t)ds;
    else crash("size not a number (or 'inf')");

    blat(&g, bsize, s);

    return 0;
}
