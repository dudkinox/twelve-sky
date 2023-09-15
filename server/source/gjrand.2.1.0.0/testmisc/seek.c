/* Test software for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* Basic demonstration of the gjrand_seek function. */
/* This one is interactive. Run with no args. */
/* At the prompt ":", press enter for a random number, */
/*   and to advance one crank. */
/* Press . (then enter) to print the current position relative to start. */
/* Press .+nnn or .-nnn to jump to a position relative to current */
/*   and print pos. */
/* Press +nnn or -nnn to jump to a position relative to start */
/*   and print pos. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../src/gjrand.h"

int
main(int argc, char **argv)
{
    char cbuf[105];
    struct gjrand s;
    double t;
    int64_t p;

    gjrand_initrand(&s);

    while (putchar(':'), fflush(stdout), fgets(cbuf, 100, stdin))
    {
	if (cbuf[0]=='\n' || cbuf[0]=='\r') printf("%ld\n", gjrand_rand(&s));
	else if (cbuf[0]=='.')
	{
	    if (sscanf(cbuf+1, "%lf", &t)==1) p = (int64_t)t;
	    else p=0;
	    printf("pos = %.0f\n", (double)gjrand_seek(&s, p, SEEK_CUR));
	}
	else if (sscanf(cbuf, "%lf", &t)==1)
	{
	    p = (int64_t)t;
	    printf("pos = %.0f\n", (double)gjrand_seek(&s, p, SEEK_SET));
	}
	else printf("huh?\n");
    }

    return 0;
}
