/* Test software for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* Basic demonstration of the gjrand_save and gjrand_restore functions. */

#include <stdlib.h>
#include <stdio.h>
#include "../src/gjrand.h"

static void
crash(char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

int
main(int argc, char **argv)
{
    struct gjrand s;
    char *b = (char *)malloc(gjrand_statesize());
    int r;

    if (b==0) crash("malloc failed");

    gjrand_initrand(&s);
    printf("%ld\n", gjrand_rand(&s));
    printf("%ld\n", gjrand_rand(&s));
    printf("%ld\n", gjrand_rand(&s));

    printf("saving\n");
    gjrand_savestate(&s, b);
    printf("%ld\n", gjrand_rand(&s));
    printf("%ld\n", gjrand_rand(&s));
    printf("%ld\n", gjrand_rand(&s));

    printf("restoring\n");
    r=gjrand_restorestate(&s, b);
    printf("restore status = %d (non-zero means failure)\n", r);
    printf("%ld\n", gjrand_rand(&s));
    printf("%ld\n", gjrand_rand(&s));
    printf("%ld\n", gjrand_rand(&s));

    printf("damaging save buffer then restoring\n");
    b[12]++;
    r=gjrand_restorestate(&s, b);
    printf("restore status = %d (non-zero means failure)\n", r);

    free(b); b=0;

    return 0;
}
