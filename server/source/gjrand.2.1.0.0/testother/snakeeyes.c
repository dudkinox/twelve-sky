/* Test software for gjrand random numbers version 1.3.3.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../src/gjrand.h"
#include "../testcommon/chi2p.h"

/* Chi-squared test for gjrand_dice. */
/* We use two six sided dice. */
/* First arg is number of trials. */
/* Second optional arg is random seed. */

void
crash(const char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

long histo[11]; /* 2 dice with sides numbered 0 to 5. */

void
dodice(long count, struct gjrand *s)
{
    int i;
    for (i=0; i<11; i++) histo[i]=0;
    do
    {
	histo[gjrand_dice(s, 6, 2)]++;
	count--;
    } while (count>0);
}

void
doan(long count)
{
    double ex[11], r=0.0, p;
    int i;

    for (i=0; i<11; i++) printf("%2d: %10ld\n", i+2, histo[i]);

    ex[0]=ex[10]=count*(1.0/36.0);
    ex[1]=ex[9]=count*(2.0/36.0);
    ex[2]=ex[8]=count*(3.0/36.0);
    ex[3]=ex[7]=count*(4.0/36.0);
    ex[4]=ex[6]=count*(5.0/36.0);
    ex[5]=count*(6.0/36.0);

    for (i=0; i<11; i++) r+=(ex[i]-histo[i])*(ex[i]-histo[i])/ex[i];

    p = chi2p2(r, 10);

    printf("expect [ 1.5 2.6 4.9 16.0 23.2 29.6 ]\n");
    printf("chis = %f     df = 10     P = %.3g\n", r, p);
}

int
main(int argc, char **argv)
{
    long count, seed;
    struct gjrand s;

    if (argc<2) crash("1 or 2 args: count [ seed ]");
    if (sscanf(argv[1], "%ld", &count)!=1) crash("count not a number");
    if (argc>2)
    {
	if (sscanf(argv[2], "%ld", &seed)!=1) crash("seed not a number");
	gjrand_init(&s, seed);
    }
    else gjrand_initrand(&s);

    dodice(count, &s);
    doan(count);

    return 0;
}
