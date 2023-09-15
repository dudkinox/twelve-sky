#ifndef _GJRAND_GONE_AWAY_H

#define _GJRAND_GONE_AWAY_H 1

/* Part of gjrand random number library version 2.0.0.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* --- gone away functions: don't use these in new programs. --- */
/* These functions are no longer in the library. This serves as a kind of */
/* log of what functions have gone away, when, and what you could use */
/* instead. */

/* update state and return a uniformly distributed random integer */
/* in the range [0 .. m - 1 ]. m must be sufficiently small to */
/* fit in 32 bits regardless of machine word size. */
/* On 2006-03-03 this was deprecated. */
/* On 2007-09-13 it was listed as gone away. */
/* It was last available in 1.2.0.0 and first absent in 1.3.0.0 . */
/* Use gjrand_rand32mod() or gjrand_rand64mod() instead. */
unsigned long gjrand_randmod(struct gjrand *state, unsigned long m)
	GJRAND_THROW_NOTHING;

/* In versions 0.1.9.0 and older, this was the underlying random generator. */
/* For compatibility it was also available in a few newer versions. */
/* update state then place 4 new random 32 bit unsigned integers in r[] */
/* On 2006-05-22 this was deprecated. */
/* On 2007-09-13 it was listed as gone away. */
/* It was last available in 1.2.0.0 and first absent in 1.3.0.0 . */
/* Use gjrand_rand32v(state, 4, r) instead. */
void gjrand_crankhandle(struct gjrand *state, uint32_t r[4])
	GJRAND_THROW_NOTHING;

/* Print the message s to standard error then exit(1) or maybe abort(). */
/* This function was here since the start and i no longer remember why. */
/* On 2006-10-19 this was deprecated. */
/* On 2007-09-13 it was listed as gone away. */
/* It was last available in 1.2.0.0 and first absent in 1.3.0.0 . */
void gjrand_crash(char *s);

/* Same as gjrand_insphere() but returns square of distance of point from */
/* the origin. */
/* On 2006-12-18 this was deprecated. */
/* On 2008-03-29 it was listed as gone away. */
/* It was last available in 1.3.3.0 and first absent in 2.0.0.0 . */
/* Instead use gjrand_insphere() then calculate distance yourself. */
double gjrand_insphere_2(struct gjrand *s, int n, double *a)
	GJRAND_THROW_NOTHING;

#endif /* _GJRAND_GONE_AWAY_H */
