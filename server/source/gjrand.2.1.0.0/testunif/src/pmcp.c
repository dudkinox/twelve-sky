/* Test software for gjrand random numbers version 1.3.3.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* Parallel Master Control Program. */

/* It runs a lot of the other test programs from this directory. */
/* This one runs somewhat in parallel. If you have multiple CPUs it may */
/* well be faster than mcp. Even on a single CPU box, it will be faster */
/* if you are piping input from a slow generator since all tests use */
/* the same data which is generated just once. */
/* Results should be fairly close to those from mcp on the same data */
/* but not identical, because the different tests here all start at */
/* the start of the data stream (they don't in mcp). */

/* You probably need to be in the testunif directory (not a subdirectory */
/* of it) when you run. */

/* This is a sprintf format string that will build the actual command */
/* lines to be run for each program. */
/* %.50s will be taken from progs[i].name */
/* %.0f will be a numeric argument that the programs expect. */
/* %.20s will be a temporary filename assigned by pmcp. */
/* A different version of comfmt will likely be needed if your system */
/* doesn't have a POSIX compatible sh program. (I'm thinking MS-Windows?) */

static const char * const comfmt="bin/%.50s %.0f > %.20s 2>&1";

static const char * const mcp_version="PMCP version 4p";

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

#define MBUFSIZ 4096

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "../../src/my_int_types.h"
#include "../../testcommon/pcombo.h"

static void
usage(void)
{
    fputs(
"usage: pmcp [ args ]\n"
"    --tiny          (10 MB)\n"
"    --small         (100 MB)\n"
"    --standard      (1 GB) (default)\n"
"    --big           (10 GB)\n"
"    --huge          (128 GB)\n"
"    --tera          (1 TB)\n"
"    --ten-tera      (10 TB)\n"
"Data to be tested for randomness is read from standard input. eg:\n"
"./pmcp --small < somefile.dat\n"
"./gen/blatrand inf | ./pmcp --huge\n",
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

static char *
tmpfname(int i)
{
    static char buf[30];
    sprintf(buf, "tmp_rpt_%02d", i);
    return buf;
}

int
main(int argc, char **argv)
{
#define PVALMAX 99
    FILE * pipes[PVALMAX];
    double pval[PVALMAX+3], pc;
    int j, badness[22], worse;
    char combuf[200], txt[200];
    int64_t pos=0, size=STANDARD;
    int argi=1;
    int i, progcount;
    const char *n, *sizearg="--standard";
    FILE *fp, *tfp;

    setvbuf(stdin, 0, _IONBF, 0);
    setvbuf(stdout, 0, _IONBF, 0);
    setvbuf(stderr, 0, _IONBF, 0);

    signal(SIGPIPE, SIG_IGN);

    /* Argument processing. */

    while (argi<argc)
    {
	if (strcmp(argv[argi], "--tiny")==0) size=TINY;
	else if (strcmp(argv[argi], "--small")==0) size=SMALL;
	else if (strcmp(argv[argi], "--standard")==0) size=STANDARD;
	else if (strcmp(argv[argi], "--big")==0) size=BIG;
	else if (strcmp(argv[argi], "--huge")==0) size=REALLYHUGE;
	else if (strcmp(argv[argi], "--tera")==0) size=TERA;
	else if (strcmp(argv[argi], "--ten-tera")==0) size=TENTERA;
	else if (strcmp(argv[argi], "--no-rewind")==0)
	    {/* ignore for mcp compatibility? */}
	else usage();

	if (strcmp(argv[argi], "--no-rewind")!=0) sizearg=argv[argi];

	argi++;
    }

    /* Start the test programs on pipes. */
    for (i=0; (n=progs[i].name)!=0; i++)
    {
	int64_t s=size/progs[i].divisor;
	FILE *f;
	sprintf(combuf, comfmt, n, (double)s, tmpfname(i));
	f=popen(combuf, "w");
	if (f==0)
	{
	    fprintf(stderr, "pmcp: can't start [%s], crashing.\n", combuf);
	    exit(1);
	}
	setvbuf(f, 0, _IONBF, 0);
	pipes[i]=f;
    }
    progcount=i;

    /* Copy our input to all test programs still working. */
    /* Continue until no test programs still working or end of input. */
    while (1)
    {
	char buf[MBUFSIZ];
	int alldone;
	int s=fread(buf, 1, MBUFSIZ, stdin);

	if (s<=0)
	{
	    fprintf(stderr,
"warning: End of standard input. Some test programs may hang if short.\n");
	    break;
	}

	alldone=1;

	for (i=0; i<progcount; i++) if (pipes[i])
	{
	    int r=fwrite(buf, 1, s, pipes[i]);
	    if (r==s) alldone=0;
	    else
	    {
		fprintf(stderr, "short on %d %s\n", i, progs[i].name);
		pclose(pipes[i]);
		pipes[i]=0;
	    }
	}

	if (alldone) break;
    }

    /* Now collect the individual reports from the temporary files, */
    /* print them to preport.txt and stdout, collecting P-values */
    /* as we go. */

    fp=fopen("preport.txt", "a");
    if (fp==0) {fprintf(stderr, "can't open preport.txt\n"); exit(1);}

    fprintf(fp, "\n***** %s %s *****\n\n", mcp_version, sizearg);
    fprintf(stdout, "\n***** %s %s *****\n\n", mcp_version, sizearg);
    fprintf(stdout, "see preport.txt for details\n");

    for (i=0; (n=progs[i].name)!=0; i++)
    {
	int64_t s=size/progs[i].divisor;
	int pdone=0;

	fprintf(fp, "\n\n=================\n%s 1/%d (%.0f bytes)\n========\n",
		n, progs[i].divisor, (double)s);

	tfp=fopen(tmpfname(i), "r");
	if (tfp==0)
	{
	    fseek(fp, 0L, SEEK_END);
	    fprintf(fp, "trouble [%s]\n", progs[i].name);
	    fprintf(stderr, "trouble [%s]\n", progs[i].name);
	    fprintf(fp, "completed %d ok, 1 crashed\n", i);
	    fprintf(stderr, "completed %d ok, 1 crashed\n", i);
	    return 1;
	}

	while (fgets(txt, 180, tfp))
	{
	    fputs(txt, fp);
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
	fclose(tfp);

	if (!pdone)
	{
	    fseek(fp, 0L, SEEK_END);
	    fprintf(fp, "Missing P value [%s] [%s]\n",
		progs[i].name, tmpfname(i));
	    fprintf(stderr, "Missing P value [%s] [%s]\n",
		progs[i].name, tmpfname(i));
	    fprintf(fp, "completed %d ok, 1 crashed\n", i);
	    fprintf(stderr, "completed %d ok, 1 crashed\n", i);
	    return 1;
	}

	/* if not debugging? */
	unlink(tmpfname(i));

	pos+=s;
    }

    /* print summaries */
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
