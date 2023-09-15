/* Test software for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include "../src/gjrand.h"

int
main(int argc, char **argv)
{
    printf("Version string\n[%s]\n\n",gjrand_version_string);
    printf("Major version     = %d\n", gjrand_version_number[0]);
    printf("Minor version     = %d\n", gjrand_version_number[1]);
    printf("Very minor version= %d\n", gjrand_version_number[2]);
    printf("Patch level       = %d\n\n", gjrand_version_number[3]);
    return 0;
}
