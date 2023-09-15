/* Test software for gjrand random numbers version 1.0.0.0 or later. */
/* Copyright (C) 2004-2006 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../src/my_int_types.h"

/* The idea, although presumably old and obvious, is pinched from */
/* tests mentioned in Christopher Wallace's excellent FastNorm3 */
/* generator. Transforms 1 to 4 are also from there. */
/* (NB only the ideas, Wallace doesn't give source code for this.) */

/* Normal deviates are converted to uniform floating point in the */
/* range [ 0 1 ) and then to fixed point. The fixed point file */
/* should be uniform random bits, it can be tested using the tests */
/* in testunif/ or with other test suites such as Diehard or NIST. eg: */
/* ./blatnorm | ./tounif 4 48 | ../testunif/chi16 10000000 */

/* Alternatively, converts to uniform floats in [ 0 1 ) and writes them */
/* as binary doubles suitable for tests in testfunif */
/* ./blatnorm | ./tounif 1 f | ../testfunif/runif 1000000 */

/* As well as testing the alleged random numbers, some of these */
/* impose tests on the accuracy (and speed) of your math library functions: */
/* Transform 1 : erf() */
/* Transform 2 : exp() */
/* Transform 5 : atan2() */

char *progname;

void
usage(void)
{
    fprintf(stderr, "usage: %s bits transform\n", progname);
    fprintf(stderr, "bits, one of 8 16 32 48 f\n");
    fprintf(stderr, "transform, one of 1 2 4 5\n");
    exit(1);
}

void
crash(char *s)
{
    fprintf(stderr, "%s crash [%s]\n", progname, s);
    exit(1);
}

#define S16 65536.0
#define S32 (S16*S16)

#define MAX 1024

void
blatf(double a[], int n)
{
    if (fwrite(a, sizeof(double), n, stdout)!=(size_t)n) crash("fwrite");
}

void
blat8(double a[], int n)
{
    unsigned char b[MAX];
    int i;

    if (n>MAX) crash("blat8 n too big");
    for (i=0; i<n; i++) b[i]=(uint32_t)floor(256.0*a[i]);
    if (fwrite(b, sizeof(unsigned char), n, stdout)!=(size_t)n) crash("fwrite");
}

void
blat16(double a[], int n)
{
    unsigned short b[MAX];
    int i;

    if (n>MAX) crash("blat16 n too big");
    for (i=0; i<n; i++) b[i]=(uint32_t)floor(S16*a[i]);
    if (fwrite(b, sizeof(unsigned short), n, stdout)!=(size_t)n)
	crash("fwrite");
}

void
blat32(double a[], int n)
{
    uint32_t b[MAX];
    int i;

    if (n>MAX) crash("blat32 n too big");
    for (i=0; i<n; i++) b[i]=(uint32_t)floor(S32*a[i]);
    if (fwrite(b, sizeof(uint32_t), n, stdout)!=(size_t)n) crash("fwrite");
}

void
blat48(double a[], int n)
{
    unsigned short b[3*MAX];
    int i;

    if (n>MAX) crash("blat48 n too big");
    for (i=0; i<n; i++)
    {
	double x=a[i], y=S32*x;
	int32_t j=(uint32_t)floor(y);
	b[3*i]=j; b[3*i+1]=j>>16;
	b[3*i+2]=(uint32_t)floor(S16*(y-j));
    }
    if (fwrite(b, sizeof(unsigned short), 3*n, stdout)!=(size_t)(3*n))
	crash("fwrite");
}

void
trans1(void (*blatfn)(double a[], int n))
{
    double a[MAX];
    int i, f;

    while (1)
    {
	f=fread(a, sizeof(double), MAX, stdin);
	if (f<0) crash("fread");
	if (f==0) return;
	for (i=0; i<f; i++) a[i]=0.5 * (1.0 + erf(a[i] * M_SQRT1_2));
	blatfn(a, f);
    }
}

void
trans2(void (*blatfn)(double a[], int n))
{
    double a[2*MAX], x, y;
    int i, j, f;

    while (1)
    {
	f=fread(a, 2*sizeof(double), MAX, stdin);
	if (f<0) crash("fread");
	if (f==0) return;
	j=0;
	for (i=0; i<f; i++)
	{
	    x=a[j++]; y=a[j++];
	    a[i]=exp(-0.5 * (x*x + y*y));
	}
	blatfn(a, f);
    }
}

void
trans4(void (*blatfn)(double a[], int n))
{
    double a[4*MAX], w, x, y, z;
    int i, j, f;

    while (1)
    {
	f=fread(a, 4*sizeof(double), MAX, stdin);
	if (f<0) crash("fread");
	if (f==0) return;
	j=0;
	for (i=0; i<f; i++)
	{
	    w=a[j++]; x=a[j++]; y=a[j++]; z=a[j++];
	    x=x*x+w*w;
	    a[i] = x / (x + y*y + z*z);
	}
	blatfn(a, f);
    }
}

void
trans5(void (*blatfn)(double a[], int n))
{
    double a[2*MAX], x, y;
    int i, j, f;

    while (1)
    {
	f=fread(a, 2*sizeof(double), MAX, stdin);
	if (f<0) crash("fread");
	if (f==0) return;
	j=0;
	for (i=0; i<f; i++)
	{
	    x=a[j++]; y=a[j++];
	    a[i]=atan2(x,y)*(0.5/M_PI)+0.5;
	}
	blatfn(a, f);
    }
}

int
main(int argc, char **argv)
{
    int trans, bits;
    void (*blatfn)(double a[], int n)=0;

    progname=argv[0];

    if (argc!=3) usage();
    if (argv[1][0]=='f' && argv[1][1]==0) blatfn=blatf;
    else
    {
	if (sscanf(argv[1], "%d", &bits)!=1) usage();
	if (bits==8) blatfn=blat8;
	else if (bits==16) blatfn=blat16;
	else if (bits==32) blatfn=blat32;
	else if (bits==48) blatfn=blat48;
	else usage();
    }

    if (sscanf(argv[2], "%d", &trans)!=1) usage();

    if (trans==1) trans1(blatfn);
    else if (trans==2) trans2(blatfn);
    else if (trans==4) trans4(blatfn);
    else if (trans==5) trans5(blatfn);
    else usage();

    return 0;
}
