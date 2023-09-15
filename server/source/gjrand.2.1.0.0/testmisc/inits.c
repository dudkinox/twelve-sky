/* Test software for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../src/gjrand.h"

/* This tests gjrand_inits(). It should illustrate that changing one bit */
/* in the string usually makes major changes in the actual initial state. */

/* Note this program examines the data fields of the struct gjrand . */
/* This is quick and dirty and you really shouldn't do it in your own */
/* code because one day the data field layout might change and break */
/* your programs. If this was C++ the data fields would be "private". */
/* For instance i had to rewrite this going from version 0.1.9.0 to */
/* 1.0.0.0 . */

/* Note that in version 1.0.0.0 although the struct gjrand has 256 bits */
/* only 128 of them are affected by the seed value. Another 64 start at */
/* 0 and change as we cycle. And 64 more have a fixed magic value to */
/* distinguish gjrand_inits() from other seeding methods. */

static struct gjrand g0;

static int low=129, high= -1, total=0;

static int
hw(unsigned long w)
{
    long r=0;

    while (w) {r+=w&1; w>>=1;}

    return r;
}

static int
hamming(struct gjrand *g0, struct gjrand *g)
{
    int r;
    uint64_t t;

    t = g0->b ^ g->b;
    r=hw((unsigned long)(t>>32));
    r+=hw((unsigned long)(t&0xffffffff));
    t = g0->c ^ g->c;
    r+=hw((unsigned long)(t>>32));
    r+=hw((unsigned long)(t&0xffffffff));

    return r;
}

static void
dochange(unsigned char *s, int l, int i, int mask)
{
    struct gjrand g;
    int h;

    s[i]^=mask;
    gjrand_inits(&g, s, l);
    s[i]^=mask;

    h=hamming(&g0, &g);
    if (h<low) low=h;
    if (h>high) high=h;
    total+=h;
}

static void
printstats(int l)
{
    printf("Number of bits of state changed (out of 128) by changing\n"
	   "one bit of the seed string:\n\n");
    printf("low  = %3d\nhigh = %3d\nmean = %7.3f\n",
	low, high, (double)total/(8*l));
}

static void
crash(char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

int
main(int argc, char **argv)
{
    unsigned char *s;
    int l, mask, i;

    if (argc!=2) crash("give an argument string for a seed");
    s = (unsigned char *)strdup(argv[1]);
    if (s==0) crash("strdup failed");
    l=strlen((const char *)s);

    gjrand_inits(&g0, s, l);

    for (i=0; i<l; i++) for (mask=128; mask; mask>>=1) dochange(s, l, i, mask);

    printstats(l);

    free(s); s=0;

    return 0;
}
