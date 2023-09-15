/* Test software for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* This one writes a file for use by several tests in this directory. */

/* first arg is required file size in bytes */
/*     or "inf" to write forever. */
/* second optional arg is random seed. */

/* The idea of this one is to test independence of the streams generated */
/* by different streams. This idea is inspired by a quick look through */
/* SPRNG and its tests. The SPRNG tests i think do a lot more than this. */
/* I'd like to credit the SPRNG people, but after a quick browse though */
/* i'm not sure who they are. I got the package from Florida State University */
/* But some messages suggest it came from NCSA at UIUC. */

/* 512 different generator objects are used and we output 32 bytes from */
/* each in turn. (In versions 0.1.9.0 and earlier it was 16 bytes.) */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "../../src/gjrand.h"

static void
crash(char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

static void
blat(struct gjrand g[512], int64_t s)
{
    uint64_t buf[2048];

    while (s)
    {
	int c;
	int i=2044, j=511;
	do
	{
	    gjrand_rand64v(g+j, 4, buf+i);
	    j--; i-=4;
	} while (i>=0);

	if ((uint64_t)s>sizeof(buf)) c=sizeof(buf); else c=s;
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
    struct gjrand g[512];
    unsigned long seed;
    double ds;
    int64_t s= -1;
    int i;

    if (argc>2)
    {
	sscanf(argv[2], "%lu", &seed);
	for (i=0; i<512; i++) gjrand_init(g+i, seed+i);
    }
    else for (i=0; i<512; i++) gjrand_initrand(g+i);
    if (argc<2) crash("give a size arg in bytes (or 'inf')");

    if (strcmp(argv[1],"inf")==0) s= -1;
    else if (sscanf(argv[1], "%lf", &ds)==1) s=(int64_t)ds;
    else crash("size not a number (or 'inf')");

    blat(g, s);

    return 0;
}
