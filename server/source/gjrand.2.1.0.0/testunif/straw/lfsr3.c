/* Miscellaneous stuff for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "../../src/my_int_types.h"

/* This one writes a file for use by the tests in testunif */

/* first arg is required file size in bytes or "inf" to write forever. */
/* second optional arg is random seed. */

/* This is a linear feadback shift register and unlike the other two */
/* this is simple bitwise no-frills. This is the same primitive polynomial */
/* as in lfib3, so this is actually the least significant bits of lfib3 */
/* packed into words. A different init is used though. */

/* Cycle should be (2 ** 100) - 1 bits. */

/* Quality is poor. I think there is a problem with slow diffusion here. */
/* Ie, if an unusual pattern of bits gets into the shift register, */
/* related unusual patterns are likely to be stuck in there through several */
/* hundred bits of output. Even larger shift registers would help to hide */
/* this effect, but wouldn't make it go away. Fixing it really needs */
/* some method of faster diffusion: eg. more tap points, or the "twist" and */
/* "temper" used in Mersenne Twister. */

void
crash(char *s)
{
    fprintf(stderr, "crash [%s]\n", s);
    exit(1);
}

/* must be divisible by 4 */
#define SIZ 1024

void
update(uint32_t x[SIZ])
{
    uint32_t *xi, *end, a, b, c, d;

    x[0] = a = ((x[SIZ-4]<<28)|(x[SIZ-3]>>4)) ^ ((x[SIZ-2]<<27)|(x[SIZ-1]>>5));
    x[1] = b = ((x[SIZ-3]<<28)|(x[SIZ-2]>>4)) ^ ((x[SIZ-1]<<27)|(a>>5));
    x[2] = c = ((x[SIZ-2]<<28)|(x[SIZ-1]>>4)) ^ ((a<<27)|(b>>5));
    x[3] = d = ((x[SIZ-1]<<28)|(a>>4)) ^ ((b<<27)|(c>>5));

    xi=x+4; end=x+SIZ;
    do
    {
	xi+=4;
	xi[-4] = a = ((a<<28)|(b>>4)) ^ ((c<<27)|(d>>5));
	xi[-3] = b = ((b<<28)|(c>>4)) ^ ((d<<27)|(a>>5));
	xi[-2] = c = ((c<<28)|(d>>4)) ^ ((a<<27)|(b>>5));
	xi[-1] = d = ((d<<28)|(a>>4)) ^ ((b<<27)|(c>>5));
    } while (xi<end);
}

void
writestuff(int64_t s, uint32_t seed)
{
    int i;
    uint32_t j, buf[SIZ];

    buf[0]= -(1ul);
    buf[1]=j=seed;
    for (i=2; i<SIZ; i++)
    {
	j+=123456789;
	j*=987654321;
	j^=(j>>16);
	buf[i]=j;
    }

    while (s)
    {
	size_t c;

	update(buf);

	if ((uint64_t)s>sizeof(buf)) c=sizeof(buf); else c=s;
	if (fwrite(buf, 1, c, stdout)!=c) crash("fwrite fails");

	if (s>0) s-=c;
    }
}

int
main(int argc, char **argv)
{
    double ds;
    int64_t s;
    unsigned long seed;

    if (argc>2) sscanf(argv[2], "%lu", &seed);
    else seed=time(0)*(2*getpid()+1);
    if (argc<2) crash("give a size arg in bytes, or 'inf'");
    if (sscanf(argv[1], "%lf", &ds)==1) s=(int64_t)ds;
    else
    {
	if (strcmp(argv[1],"inf")) crash("not a number");
	s= -1;
    }

    writestuff(s, seed);

    return 0;
}
