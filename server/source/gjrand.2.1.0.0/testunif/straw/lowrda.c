/* Miscellaneous stuff for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "../../src/gjrand.h"

/* This one writes a file for use by the tests in testunif */

/* first arg is required file size in bytes or "inf" to write forever. */
/* second optional arg is random seed. */

/* This one is an experiment to see if i can make it fail the rda test */
/* on the low side, preferably while doing fairly well on other tests. */
/* The thing is that bad generators usually fail rda on the high side. */
/* I can't think why this would be important theoretically or practically, */
/* but i was just wondering. */

/* It certainly fails low on rda (and r4da). */
/* Otherwise it looks mostly ok. */
/* It fails metachi after about 100MB (not enough variance). */
/* It fails v256 somewhere near 20MB. */

void
crash(char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

struct gjrand g;

#define MAX 4101
double expect[MAX];
double count[MAX];
int64_t last[256];

void
init(void)
{
    double c, e;
    int i;

    for (i=0; i<MAX; i++) count[i]=0;

    for (i=0; i<256; i++) last[i]= -1;

    c=1.0; e=1.0/256.0;

    for (i=1; i<MAX; i++)
    {
	expect[i]=e;
	c-=e;
	e *= 255.0/256.0;
    }
    expect[0]=c;
}

static int64_t pos=0;

void
fill1buf(unsigned char *buf)
{
    int i;

    gjrand_randbytes(&g, 4096, buf);

    for (i=0; i<4096; i++)
    {
	int j=buf[i], k;

	pos++;
	k=pos-last[j]; if (k>=MAX) k=0;
	last[j]=pos;
	count[k]++;
    }
}

void
fillbuf(unsigned char *buf)
{
    unsigned char buf2[4096];
    int i;

    gjrand_randbytes(&g, 4096, buf);
    gjrand_randbytes(&g, 4096, buf2);

    for (i=0; i<4096; i++)
    {
	int j=buf[i], j2=buf2[i], k, k2;

	pos++;
	k=pos-last[j]; if (k>=MAX) k=0;
	k2=pos-last[j2]; if (k2>=MAX) k2=0;

	if (count[k]*expect[k2] > count[k2]*expect[k])
	    {buf[i]=j=j2; k=k2;}

	last[j]=pos;
	count[k]++;
    }
}

void
blat(int64_t s)
{
    unsigned char buf[4096];
    size_t c;

    init();

    if (s==0) return;
    fill1buf(buf);
    if ((uint64_t)s>4096) c=4096; else c=(size_t)s;
    if (fwrite(buf, 1, c, stdout)!=c) crash("fwrite fails");
    s-=c;

    while (s)
    {
	fillbuf(buf);

	if ((uint64_t)s>4096) c=4096; else c=s;
	if (fwrite(buf, 1, c, stdout)!=c) crash("fwrite fails");

	if (s>=0) s-=c;
    }
}

int
main(int argc, char **argv)
{
    double ds;
    unsigned long seed;
    int64_t s;

    if (argc>2) {sscanf(argv[2], "%lu", &seed); gjrand_init(&g, seed);}
    else gjrand_initrand(&g);
    if (argc<2) crash("give a size arg in bytes, or 'inf'");
    if (sscanf(argv[1], "%lf", &ds)==1) s=(int64_t)ds;
    else
    {
	if (strcmp(argv[1],"inf")) crash("not a number");
	s= -1;
    }

    blat(s);

    return 0;
}
