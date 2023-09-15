/* Test software for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* This is so you can prepare and view pretty pictures of a generator */
/* with bad 2-D lattice, or one that behaves much better. */
/* This assumes you have available a image viewer program which can */
/* display images in pgm format. Search for display and do some editing */
/* if your viewer program is not called "display". */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../../src/my_int_types.h"

void
crash(char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

static unsigned char buf[512][512];

void
usage(void)
{
    crash("give two numeric args: scale saturation-percentage");
}

void
plot(double scale, double in[], int c)
{
    while (c>0)
    {
	double x=in[--c];
	double y=in[--c];
	int i=scale*x, j=scale*y;

	if (i<512 && j<512) buf[i][j]=200;
    }
}

void
mkbuf(double scale, int64_t count)
{
    double in[1024];
    int c;

    while (count>0)
    {
	c=1024; if (c>2*count) c=2*count;
	if (fread(in, sizeof(double), c, stdin)!=c) crash("fread");
	plot(scale, in, c);
	count-=c>>1;
    }
}

void
run(double scale, double saturation)
{
    /* Substitute for "display -", some program that will display an image */
    /* that it reads in pgm format from its standard input. */
    FILE *fp=popen("display -", "w");

    if (fp==0) crash("popen");

    memset(buf, 0, sizeof(buf));

    saturation/=100.0;

    mkbuf(scale*512.0, (uint64_t)(scale*scale*saturation*512*512));

    fprintf(fp, "P5\n512 512\n255\n");
    if (fwrite(buf, 1, sizeof(buf), fp)!=sizeof(buf)) crash("fwrite");

    pclose(fp);
}

int
main(int argc, char **argv)
{
    double scale, saturation;

    if (argc!=3) usage();
    if (sscanf(argv[1], "%lf", &scale)!=1) usage();
    if (sscanf(argv[2], "%lf", &saturation)!=1
	|| saturation<0.0 || saturation>100.0) usage();

    run(scale, saturation);

    return 0;
}
