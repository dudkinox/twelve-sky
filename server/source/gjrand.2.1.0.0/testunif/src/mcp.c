/* Test software for gjrand random numbers version 1.3.3.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2. */

/* Master Control Program. */

/* Old-time Burroughs mainframe operators and people who saw "War Games" */
/* know that the Master Control Program should be powerful and dangerous. */
/* This one is neither. */

/* It just runs a lot of the other test programs from this directory. */

/* You probably need to be in the testunif directory (not a subdirectory */
/* of it) when you run. */

/* This is a sprintf format string that will build the actual command */
/* lines to be run for each program. */
/* %.50s will be taken from progs[i].name */
/* %.0f will be a numeric argument that the programs expect. */
/* A different version of comfmt will likely be needed if your system */
/* doesn't have a POSIX compatible sh program. (I'm thinking MS-Windows?) */

static const char * const comfmt="bin/%.50s %.0f >> report.txt 2>&1";

static const char * const mcp_version="MCP version 4";

struct prog
{
    const char *name;
    int divisor;
};

static struct prog progs[]=
{
    {"metachi", 1},
    {"v256", 1},
    {"shuffle -r", 1},
    {"shuffle", 1},
    {"sh5da -r", 1},
    {"sh5da", 1},
    {"zero", 1},
    {"zrda", 2},
    {"selfcor -r", 2},
    {"selfcor -f", 2},
    {"rda", 2},
    {"slicerda", 2},
    {"partrda", 2},
    {"addsub -r", 3},
    {"addsub -f", 3},
    {"chi16", 3},
    {"chixor8", 6},
    {"shiftrda", 12},
    {"nda", 32},
    {0, 0}
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
"usage: mcp [ args ]\n"
"    --tiny          (10 MB)\n"
"    --small         (100 MB)\n"
"    --standard      (1 GB) (default)\n"
"    --big           (10 GB)\n"
"    --huge          (128 GB)\n"
"    --tera          (1 TB)\n"
"    --ten-tera      (10 TB)\n"
"    --no-rewind     (don't rewind to start of file between different tests)\n"
"Data to be tested for randomness is read from standard input. eg:\n"
"./mcp --small < somefile.dat\n"
"./gen/blatrand inf | ./mcp --huge\n",
	stderr);

    exit(1);
}

#define K ((int64_t)1024)
#define M (K*K)
#define G (K*M)

#define TINY (10*M)
#define SMALL (100*M)
#define STANDARD (G)
#define BIG (10*G)
#define REALLYHUGE (128*G)
#define TERA (K*G)
#define TENTERA (10*TERA)

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
    int i, trouble;
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
	else if (strcmp(argv[argi], "--tera")==0) size=TERA;
	else if (strcmp(argv[argi], "--ten-tera")==0) size=TENTERA;
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

	if (wind && pos+s>size) {rewind(stdin); pos=0;}

	fprintf(fp, "\n\n=================\n%s 1/%d (%.0f bytes)\n========\n",
		n, progs[i].divisor, (double)s);
	fprintf(stdout,
		"\n\n=================\n%s 1/%d (%.0f bytes)\n========\n",
		n, progs[i].divisor, (double)s);

	sprintf(combuf, comfmt, n, (double)s);

	/* I'm working on the assumption that system() returns 0 */
	/* if and only if the command is run succesfully and */
	/* exits with 0 (by convention, successful) status. */
	/* That tends to be true on POSIX-like systems. */
	trouble=system(combuf);

	/* This does nothing, but may be necessary between writing and */
	/* reading on the same stream, according to various documentation. */
	/* It's probably not needed on an unbuffered stream, but better safe */
	/* than sorry. */
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
		pdone=(sscanf(txt+3, "%lg", pval+i)==1);
	    }
	}
	clearerr(fp);


	if (trouble)
	{
	    fseek(fp, 0L, SEEK_END);
	    fprintf(fp, "trouble [%s]\n", combuf);
	    fprintf(stderr, "trouble [%s]\n", combuf);
	    fprintf(fp, "completed %d ok, 1 crashed\n", i);
	    fprintf(stderr, "completed %d ok, 1 crashed\n", i);
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
    }

    fseek(fp, 0L, SEEK_END);
    fprintf(fp, "\n\n====================\ncompleted %d tests\n", i);
    fprintf(stdout, "\n\n====================\ncompleted %d tests\n", i);
    for (j=0; j<22; j++) badness[j]=0;
    for (j=0; j<i; j++)
    {
	double d=pval[j], b=0.1;
	int k=0;

	while (d<b && k<20) {b*=0.1; k++;}
	if (d==0) k=21;
	badness[k]++;
    }

    fprintf(fp, "%d out of %d tests ok.\n", badness[0], i);
    fprintf(stdout, "%d out of %d tests ok.\n", badness[0], i);
    worse=i-badness[0];

    for (j=1; j<22; j++) if (badness[j])
    {
	static const int badmax[5][2]=
	{
	    {99, 99},
	    {4, 9},
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

if (worse!=0) fprintf(stderr, "worse=%d huh!\n", worse);

    pc=pcombo(pval, i);
    fprintf(fp, "\n\nOverall summary one sided P-value (smaller numbers bad)\n"
	"P = %.3g\n", pc);
    fprintf(stdout,
"\n\nOverall summary one sided P-value (smaller numbers bad)\nP = %.3g\n",
pc);

    return 0;
}
