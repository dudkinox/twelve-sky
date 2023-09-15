/* Test software for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include "../../src/gjrand.h"

/* This one reads uniform random bits on standard input, converts to */
/* uniform binary double in [ 0 , 1 ] and writes on standard output. */

/* No args */

/* eg: */
/* ../testunif/bin/blatrand inf | ./uni2f | ./pi3 10000000 */

void
crash(char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

uint32_t in[2048];
double out[1024];

void
conv(void)
{
#define B2 (0.25)
#define B8 (B2*B2*B2*B2)
#define B32 (B8*B8*B8*B8)

    int i;

    for (i=0; i<1024; i++)
	out[i]=((in[2*i+1]*B32)+in[2*i])*B32;
}

int
main(int argc, char **argv)
{
    int r;

    while (1)
    {
	r=fread(in, sizeof(uint32_t), 2048, stdin);
	if (r<=0) break;
	conv();
	r/=2;
	if (fwrite(out, sizeof(double), r, stdout)!=(size_t)r) break;
    }

    return 0;
}
