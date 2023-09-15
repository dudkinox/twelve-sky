/* Test software for gjrand random numbers version 1.3.1.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* Master Control Program (for Floating point). */

/* It runs a lot of the other test programs from this directory. */

/* You probably need to be in the testfunif directory (not a subdirectory */
/* of it) when you run. */

/* This is a sprintf format string that will build the actual command */
/* lines to be run for each program. */
/* %.50s will be taken from progs[i].name */
/* %.0f will be a numeric argument that the programs expect. */
/* A different version of comfmt will likely be needed if your system */
/* doesn't have a POSIX compatible sh program. (I'm thinking MS-Windows?) */

static const char * const comfmt="bin/%.50s %.0f >> report.txt 2>&1";

static const char * const mcp_version="MCPF version 6";

struct prog
{
    const char *name;
    int divisor;
    int minimum;
};

static struct prog progs[]=
{
    {"extreme 20",	1, 400000},//		0.4 tiny
    {"extreme 64",	1, 400000},//		0.5 tiny
    {"extreme 200",	1, 800000},//		0.4 tiny
    {"extreme 4096",	1, 15000000},//		0.4 standard
    {"nda",		1, 800000},//		2.1 tiny
    {"angle",		3, 3000000},//		8.7 small
    {"walk",		3, 330000},//		3.2 tiny
    {"dim2",		3, 10000000},//		5.7 standard
    {"dim3",		3, 3000000},//		6.5 small
    {"dim6",		3, 3000000},//		6.5 small
    {"dim20",		3, 10000000},//		5.0 standard
    {"dim40",		3, 10000000},//		5.1 standard
    {"diff10",		4, 1700000},//		6.1 small
    {"diff3",		20, 250000},//		22.4 small
    {"chi",		16, 3000},//		6.8 tiny
    {"bds",		32, 10000},//		26.0 tiny
    {0, 0, 0}
};

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../../src/my_int_types.h"
#include "../../testcommon/pcombo.h"

static void
usage(void)
{
    fputs(
"usage: mcpf [ args ]\n"
"    --tiny          (1 M doubles)\n"
"    --small         (10 M doubles)\n"
"    --standard      (100 M doubles) (default)\n"
"    --big           (1 G doubles)\n"
"    --huge          (16 G doubles)\n"
"    --even-huger    (128 G doubles)\n"
"    --tera          (1 T doubles)\n"
"    --no-rewind     (don't rewind to start of file between different tests)\n"
"Data to be tested for randomness is read from standard input. eg:\n"
"./mcpf --small < somefile.dat\n"
"./blatdvf | ./mcpf --huge\n",
	stderr);

    exit(1);
}

#define K ((int64_t)1024)
#define M (K*K)
#define G (K*M)

#define TINY (1*M)
#define SMALL (10*M)
#define STANDARD (100*M)
#define BIG (G)
#define REALLYHUGE (16*G)
#define HUGER (128*G)
#define TERA (K*G)

int
main(int argc, char **argv)
{
#define PVALMAX 99
    double pval[PVALMAX+3], pc;
    int j, badness[22], worse;
    char combuf[200], txt[200];
    int64_t pos=0, size=STANDARD;
    int argi=1;
    int wind=1;
    int i, trouble, success=0;
    const char *n, *sizearg="--standard";
    FILE *fp;

    setvbuf(stdin, 0, _IONBF, 0);
    setvbuf(stdout, 0, _IONBF, 0);
    setvbuf(stderr, 0, _IONBF, 0);

    while (argi<argc)
    {
	if (strcmp(argv[argi], "--tiny")==0) size=TINY;
	else if (strcmp(argv[argi], "--small")==0) size=SMALL;
	else if (strcmp(argv[argi], "--standard")==0) size=STANDARD;
	else if (strcmp(argv[argi], "--big")==0) size=BIG;
	else if (strcmp(argv[argi], "--huge")==0) size=REALLYHUGE;
	else if (strcmp(argv[argi], "--even-huger")==0) size=HUGER;
	else if (strcmp(argv[argi], "--tera")==0) size=TERA;
	else if (strcmp(argv[argi], "--no-rewind")==0) wind=0;
	else usage();

	if (strcmp(argv[argi], "--no-rewind")!=0) sizearg=argv[argi];

	argi++;
    }

    fp=fopen("report.txt", "a+");
    if (fp==0) {fprintf(stderr, "can't open report.txt\n"); exit(1);}
    setvbuf(fp, 0, _IONBF, 0);

    fprintf(fp, "\n***** %s %s *****\n\n", mcp_version, sizearg);
    fprintf(stdout, "\n***** %s %s *****\n\n", mcp_version, sizearg);

    for (i=0; (n=progs[i].name)!=0; i++)
    {
	int64_t s=size/progs[i].divisor;
	int pdone=0;

	if (s<progs[i].minimum) continue;

	if (wind && pos+s>size) {rewind(stdin); pos=0;}

	fprintf(fp, "\n\n=================\n%s 1/%d (%.0f doubles)\n========\n",
		n, progs[i].divisor, (double)s);
	fprintf(stdout,
		"\n\n=================\n%s 1/%d (%.0f doubles)\n========\n",
		n, progs[i].divisor, (double)s);

	sprintf(combuf, comfmt, n, (double)s);

	/* I'm working on the assumption that system() returns 0 */
	/* if and only if the command is run succesfully and */
	/* exits with 0 (by convention, successful) status. */
	/* That tends to be true on POSIX-like systems. */
	trouble=system(combuf);

	/* Doesn't actually seek. Docs say this should be done between */
	/* writing and reading on the same stream. */
        (void)fseek(fp, 0L, SEEK_CUR);

	while (fgets(txt, 180, fp))
	{
	    fputs(txt, stdout);
	    if (txt[0]=='P' && txt[1]==' ' && txt[2]=='=')
	    {
		if (i>=PVALMAX)
		{
		    fseek(fp, 0L, SEEK_END);
		    fprintf(fp, "too many tests, pval too small\n");
		    fprintf(stderr, "too many tests, pval too small\n");
		    return 1;
		}
		pdone=(sscanf(txt+3, "%lg", pval+success)==1);
	    }
	}
	clearerr(fp);


	if (trouble)
	{
	    fseek(fp, 0L, SEEK_END);
	    fprintf(fp, "trouble [%s]\n", combuf);
	    fprintf(stderr, "trouble [%s]\n", combuf);
	    fprintf(fp, "completed %d ok, 1 crashed\n", success);
	    fprintf(stderr, "completed %d ok, 1 crashed\n", success);
	    return 1;
	}

	if (!pdone)
	{
	    fseek(fp, 0L, SEEK_END);
	    fprintf(fp, "Missing P value [%s]\n", combuf);
	    fprintf(stderr, "Missing P value [%s]\n", combuf);
	    fprintf(fp, "completed %d ok, 1 crashed\n", i);
	    fprintf(stderr, "completed %d ok, 1 crashed\n", i);
	    return 1;
	}

	pos+=s;
	success++;
    }

    fseek(fp, 0L, SEEK_END);
    fprintf(fp, "\n\n====================\ncompleted %d tests\n", success);
    fprintf(stdout, "\n\n====================\ncompleted %d tests\n", success);
    for (j=0; j<22; j++) badness[j]=0;
    for (j=0; j<success; j++)
    {
	double d=pval[j], b=0.1;
	int k=0;

	while (d<b && k<20) {b*=0.1; k++;}
	if (d==0.0) k=21;
	badness[k]++;
    }

    fprintf(fp, "%d out of %d tests ok.\n", badness[0], success);
    fprintf(stdout, "%d out of %d tests ok.\n", badness[0], success);
    worse=success-badness[0];

    for (j=1; j<22; j++) if (badness[j])
    {
	static const int badmax[5][2]=
	{
	    {99, 99},
	    {3, 9},
	    {1, 3},
	    {0, 1},
	    {0, 1},
	};
	char *msg;

	if (j>4 || worse>badmax[j][1]) msg="(SERIOUSLY BAD)";
	else if (worse>badmax[j][0]) msg="(slightly bad)";
	else msg="(probably ok)";

	fprintf(fp, "%d grade %d failures %s.\n", badness[j], j, msg);
	fprintf(stdout, "%d grade %d failures %s.\n", badness[j], j, msg);

	worse-=badness[j];
    }

if (worse!=0) fprintf(stderr, "worse=%d huh?\n", worse);

    pc=pcombo(pval, success);
    fprintf(fp, "\n\nOverall summary one sided P-value (smaller numbers bad)\n"
	"P = %.3g\n", pc);
    fprintf(stdout,
"\n\nOverall summary one sided P-value (smaller numbers bad)\nP = %.3g\n",
pc);

    return 0;
}
