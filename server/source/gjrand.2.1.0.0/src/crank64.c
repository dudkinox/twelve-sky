#include "gjrand.h"
#include "inte.h"

const int gjrand_version_number[4]={2,1,0,0};

const char gjrand_version_string[]=
"gjrand library 2.1.0.0\n"
"Written by G. Jones.\n"
"\n"
"Copyright (C) 2004-2008 G. Jones.\n"
"This is free software released under the Gnu General Public License,\n"
"Version 2 or 3. You should have received a copy of the GPL v2 or v3 with\n"
"this software. If not, contact your supplier, or visit the www.fsf.org web\n"
"site.\n"
"There is NO warranty; not even for MERCHANTABILITY or\n"
"FITNESS FOR A PARTICULAR PURPOSE.\n";

/* Update the state and then put 4 new random words into r[]. */
/* This is the underlying generator that does the work for all the */
/* others. */
void
gjrand_inte_crank64(struct gjrand *s, uint64_t r[4])
	GJRAND_THROW_NOTHING
{
    uint64_t a,b,c,d, t;
    int j;

    a=s->a+GJRAND_INTE_INCR;
    b=s->b; c=s->c; d=s->d;
    s->a=a;

#define ROT(x,r) ( ((x)<<r) | ((x)>>(64-r)) )

#if defined(GJRAND_ALTCODE) && GJRAND_ALTCODE
#define ROUND2(s1,s2) \
    t = a+b; c ^= t; t += b; t += 875; d ^= ROT(t, s1); \
    t = c+d; a ^= t; t += d; t += 875; b ^= ROT(t, s2);

#else /* not GJRAND_ALTCODE */
#define ROUND2(s1,s2) \
    c ^= a+b; t = a+2*b+875; d ^= ROT(t, s1); \
    a ^= c+d; t = c+2*d+875; b ^= ROT(t, s2)

#endif /* GJRAND_ALTCODE */

    j = 4;
    goto start;
    do
    {
	ROUND2(24, 11);
	start:
	ROUND2(32, 16);
    } while (--j);

    r[0]=a; r[1]=b; r[2]=c; r[3]=d;
}
