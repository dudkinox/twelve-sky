/* Test software for gjrand random numbers version 1.0.0.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

/* This one combines pairs of normal deviates to make single deviates. */
/* The combination can be add or subtract, followed by */
/* appropriate normalisation. */

char *progname;

void
usage(void)
{
    fprintf(stderr, "usage: %s [ -a -s ]\n", progname);
    exit(1);
}

void
crash(char *s)
{
    fprintf(stderr, "%s crash [%s]\n", progname, s);
    exit(1);
}

#define MAX 1024

#define ADD 1
#define SUB 2

void
docombo(int combo)
{
    double a[2*MAX], x, y;
    int i, j, f;

    while (1)
    {
	f=fread(a, 2*sizeof(double), MAX, stdin);
	if (f<0) crash("fread");
	if (f==0) return;
	j=0;
	if (combo==ADD)
	    for (i=0; i<f; i++)
	    {
		x=a[j++]; y=a[j++];
		a[i]=(x+y)*M_SQRT1_2;
	    }
	else if (combo==SUB)
	    for (i=0; i<f; i++)
	    {
		x=a[j++]; y=a[j++];
		a[i]=(x-y)*M_SQRT1_2;
	    }
	else crash("bad combo");
	if (fwrite(a, sizeof(double), f, stdout)!=(size_t)f) crash("fwrite");
    }
}

int
main(int argc, char **argv)
{
    int combo=ADD;

    progname=argv[0];

    if (argc>1)
    {
	if (strcmp(argv[1], "-a")==0) combo=ADD;
	else if (strcmp(argv[1], "-s")==0) combo=SUB;
	else crash("bad arg 1");
    }

    docombo(combo);

    return 0;
}
