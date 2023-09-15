#ifndef _GJRAND_H

#define _GJRAND_H 1

/* Part of gjrand random number library version 2.0.0.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* String representing library name, version, copyright, etc. */
extern const char gjrand_version_string[];

/* Numbers representing version. */
/* [0] major version number */
/* [1] minor version number */
/* [2] very minor version number */
/* [3] patch level */
extern const int gjrand_version_number[4];

#ifdef __cplusplus
#define GJRAND_NO_ARGS
#define GJRAND_THROW_NOTHING throw()
extern "C" {

#else /* not __cplusplus */
#define GJRAND_NO_ARGS void
#define GJRAND_THROW_NOTHING

#endif /* __cplusplus */

#include <sys/types.h>

#include "my_int_types.h"

/* Please treat the data fields as private, as they are subject to change */
/* without notice. Do all access to struct gjrand objects with the */
/* function calls provided. */
struct gjrand
{
    uint64_t a, b, c, d;
};


/* --- seed the generator --- */

/* In 64 bit gjrand (version 1.0.0.0 and later) there are potentially */
/* 2 ** 192 different cycles (although currently most are not reachable), */
/* each of which has length 2 ** 64 cranks, with up to 32 bytes available at */
/* each crank. */
/* (Each call that generates random data will advance at least one crank, */
/* or more than that if it needs more data.) */
/* gjrand_init() picks from 2 ** 32 different cycles. */
/* gjrand_init64() picks from 2 ** 64 different cycles. */
/* gjrand_init4() picks from 2 ** 128 different cycles. */
/* gjrand_inits() picks from 2 ** 128 different cycles. */
/* gjrand_initrand() picks from 2 ** 128 different cycles. */
/* where each seeding function is not supposed to have any cycles in */
/* common with the others. */
/* (gjrand_oldsrand() is a wrapper over gjrand_init() with the same cycles. */
/* The rand48 functions use yet another set of cycles of their own.) */

/* Seed the generator with one number. */
void gjrand_init(struct gjrand *, uint32_t) GJRAND_THROW_NOTHING;

/* Seed the generator with one number. */
void gjrand_init64(struct gjrand *, uint64_t) GJRAND_THROW_NOTHING;

/* Seed the generator with 4 numbers. */
/* In old versions this required special care to use. */
/* Then it was deprecated for a while. */
/* Since 1.1.0.0 it's back, won't be going away, and should be safe to use. */
void gjrand_init4(struct gjrand *, const uint32_t i[4]) GJRAND_THROW_NOTHING;

/* Seed the generator with an arbitrary chunk of data of length len bytes */
/* starting at *buf. The data is hashed so slightly differing data should */
/* usually result in very different cycles. The hash is not up */
/* to cryptographic quality, so it should be quite easy to engineer two */
/* different data chunks that result in the same seed. However this is */
/* unlikely to happen by accident. */
void gjrand_inits(struct gjrand *, const unsigned char *buf, size_t len)
	GJRAND_THROW_NOTHING;

/* Seed the generator with data from the operating system. This should select */
/* a cycle that is not very easy to predict or repeat. However, it */
/* is not sufficiently unpredictable to be used for security related purposes */
/* such as key generation. */
void gjrand_initrand(struct gjrand *);


/* --- save and restore state ---  */

/* return the size of a byte array needed to store gjrand state. */
int gjrand_statesize(GJRAND_NO_ARGS) GJRAND_THROW_NOTHING;

/* save state into buf. The number of bytes to be saved can be */
/* determined using gjrand_statesize. buf must point to a large */
/* enough buffer for that. Note that the bytes are not null terminated */
/* and can contain arbitrary byte values, including nulls */
void gjrand_savestate(const struct gjrand *state, char *buf)
	GJRAND_THROW_NOTHING;

/* restore state from buf. buf should be an array of bytes as obtained */
/* by gjrand_savestate() */
/* Return 0 for success, non-0 (probably -1 ?) for failure. */
/*    failure is most likely due to checksum non-agreement. */
/* state is probably unchanged on failure, best not to use it in that case. */
int gjrand_restorestate(struct gjrand *state, const char *buf)
	GJRAND_THROW_NOTHING;


/* random access to a position in the random stream. */
/* offset is the position (in cranks) to go to. */
/* whence is SEEK_CUR for relative to current position, */
/* anything else (but preferably SEEK_SET) for relative to start. */
/* these constants are defined in <stdio.h> */
/* Return is the position relative to start after the seek is done. */
/* Hint: to find position without changing it: gjrand_seek(s, 0, SEEK_CUR) */
/* Hint: to return to start for this seed: gjrand_seek(s, 0, SEEK_SET) */
int64_t gjrand_seek(struct gjrand *state, int64_t offset, int whence)
	GJRAND_THROW_NOTHING;


/* --- generate random numbers: integer uniform --- */

/* update state and return a uniformly distributed 31 bit random integer in */
/* the range [0 .. (2**31) - 1 ] */
long gjrand_rand(struct gjrand *) GJRAND_THROW_NOTHING;

/* update state and return a uniformly distributed 32 bit unsigned */
/* random integer in the range [0 .. (2**32) - 1 ] */
unsigned long gjrand_rand32(struct gjrand *) GJRAND_THROW_NOTHING;

/* update state and write n uniformly distributed 32 bit unsigned */
/* random integers into the buffer starting at *v . v must point */
/* at a large enough memory area for that. */
void gjrand_rand32v(struct gjrand *s, int n, uint32_t *v) GJRAND_THROW_NOTHING;

/* update state and return a uniformly distributed 64 bit unsigned */
/* random integer in the range [0 .. (2**64) - 1 ] */
uint64_t gjrand_rand64(struct gjrand *) GJRAND_THROW_NOTHING;

/* update state and write n uniformly distributed 64 bit unsigned */
/* random integers into the buffer starting at *v . v must point */
/* at a large enough memory area for that. */
void gjrand_rand64v(struct gjrand *s, int n, uint64_t *v) GJRAND_THROW_NOTHING;

/* update state and return a uniformly distributed random integer */
/* in the range [0 .. m - 1 ]. m may be any number representable */
/* as a 32 bit unsigned. */
uint32_t gjrand_rand32mod(struct gjrand *state, uint32_t m)
	GJRAND_THROW_NOTHING;

/* update state and return a uniformly distributed random integer */
/* in the range [0 .. m - 1 ]. m may be any number representable */
/* as a 64 bit unsigned. */
uint64_t gjrand_rand64mod(struct gjrand *state, uint64_t m)
	GJRAND_THROW_NOTHING;

/* update state and write n uniformly distributed random unsigned bytes */
/* in the range [0 .. 255] into buffer starting at *b */
void gjrand_randbytes(struct gjrand *state, int n, unsigned char *b)
	GJRAND_THROW_NOTHING;


/* --- generate random numbers: integer non-uniform --- */

/* update state and toss n fair coins. Count how many come up heads, and */
/* return that. for large n time is proportional to sqrt(n) . */
int gjrand_coins(struct gjrand *state, int n) GJRAND_THROW_NOTHING;

/* update state and toss n biased coins. prob is the probability for a coin */
/* to come up heads from one toss. this should be in the range [0.0 .. 1.0] */
/* Count how many come up heads, and return that. */
/* for large n time is proportional to sqrt(n) . */
int gjrand_biased_coins(struct gjrand *state, double prob, int n)
	GJRAND_THROW_NOTHING;

/* update state and throw n fair s sided dice. These dice have sides numbered */
/* from 0 to s-1. Return the total of numbers on top of all dice. time is */
/* proportional to n */
/* Hint: if you want dice numbered 1 to s, use */
/* gjrand_dice(state, s, n) + n */
int gjrand_dice(struct gjrand *state, int s, int n) GJRAND_THROW_NOTHING;

/* update state. Write the integers 0 to n-1 inclusive in random order */
/* into a[]. Note that a must point to a memory area large enough for that. */
void gjrand_shuffle(struct gjrand *state, int n, int *a) GJRAND_THROW_NOTHING;

/* update state. Events happen on average once per time interval. */
/* However, the timing of each individual event is random and independent */
/* of the timing of other events. Observe the events and return the number */
/* that occur in t time intervals. */
/* Note: t does not have to be a whole number, but return value does. */
/* for large t, time is proportional to sqrt(t). */
int gjrand_poisson(struct gjrand *state, double t) GJRAND_THROW_NOTHING;


/* --- generate random numbers: floating point uniform --- */

/* update state. Return a uniform random number in [0.0 .. 1.0). */
/* number has approx 64 bits absolute or 53 bits relative whichever is less. */
double gjrand_drand(struct gjrand *) GJRAND_THROW_NOTHING;

/* update state. Write n uniform random numbers in [0.0 .. 1.0) into v[] . */
/* v must point to a large enough memory area for that. */
void gjrand_drandv(struct gjrand *s, int n, double *v) GJRAND_THROW_NOTHING;

/* update state. Write n uniform random numbers in [0.0 .. 1.0) into v[] . */
/* v must point to a large enough memory area for that. */
/* numbers have 24 bits absolute. */
/* (This kind of assumes float is capable of that much precision, as */
/* IEEE single precision is. If float has less precision than that, you */
/* may get less than 24 bits precision and 1.0 might occasionally show up */
/* due to roundoff. Such implementations should be very rare now.) */
void gjrand_frandv(struct gjrand *s, int n, float *v) GJRAND_THROW_NOTHING;

/* update state. Return a uniform random number in [0.0 .. 1.0). */
/* Return value has approx 96 bits absolute or whatever long double supports */
/* relative whichever is less. (Both ANSI C standards say long double */
/* has at least as much precision as double. For some compilers long double */
/* is the same as double. For others, it has more precision than double. */
/* Fairly commonly, long double is an IEEE extended precision, */
/* but IEEE extended itself cuts a lot of slack.) */
long double gjrand_ldrand(struct gjrand *) GJRAND_THROW_NOTHING;

/* update state. Write n uniform random numbers in [-1.0 .. 1.0] into a[] . */
/* They have approx 63 bits absolute or 53 bits relative whichever is less. */
/* Intended mostly for use by other library functions, but nonetheless */
/* documented and available. */
void gjrand_drand_array(struct gjrand *s, int n, double *a)
	GJRAND_THROW_NOTHING;


/* --- generate random numbers: floating point geometrical --- */

/* update state. Generate a point inside the n-dimensional unit sphere. */
/* (circle for n=2, sphere for n=3, hypersphere for n=4, etc. n may now be */
/* any positive integer.) Write the cartesian coordinates for the */
/* point into a[] */
void gjrand_insphere(struct gjrand *s, int n, double *a) GJRAND_THROW_NOTHING;

/* update state. Generate a point on the surface of the n-dimensional unit */
/* sphere. n may now be any positive integer. Write the cartesian */
/* coordinates for the point into a[]. Due to roundoff error, the point will */
/* usually not be exactly on the surface, but it should be very close. */
void gjrand_onsphere(struct gjrand *s, int n, double *a) GJRAND_THROW_NOTHING;


/* --- generate random numbers: floating point statistical --- */

/* update state. Return a normally distributed random number with mean 0.0 */
/* standard deviation 1.0 */
/* Hint: for mean m and standard deviation sd, use */
/* gjrand_normal(s) * sd + m */
double gjrand_normal(struct gjrand *s) GJRAND_THROW_NOTHING;

/* update state. Write n normally distrubuted random numbers with mean 0.0 */
/* standard deviation 1.0 into v[] . */
/* v must point to a large enough memory area for that. */
void gjrand_normalv(struct gjrand *s, int n, double *v) GJRAND_THROW_NOTHING;

/* update state. Return a T-distributed random number with f degrees */
/* of freedom. f should be >= 1 . Time is proportional to f. */
/* This is the distribution associated with the "Student" T-tests. Note that */
/* f is one less than the sample size for some popular tests. */
/* Hint: if you need T-distribution with extremely large f, approximate with */
/* gjrand_normal(s) */
/* which is faster and reasonably accurate if f is large enough. */
double gjrand_t(struct gjrand *s, int f) GJRAND_THROW_NOTHING;

/* update state. Return a chi-squared distributed random number with f */
/* degrees of freedom. f should be >= 1 . time is proportional to f */
/* Hint: if you need chisquared with extremely large f, approximate with */
/* gjrand_normal(s) * sqrt(2.0 * f) + f */
/* which is faster and reasonably accurate if f is large enough. */
double gjrand_chisquared(struct gjrand *s, int f) GJRAND_THROW_NOTHING;


/* --- generate random numbers: floating point physical --- */

/* The next four are associated with the analysis of ideal gases by Maxwell */
/* and Boltzmann. The idea is that the gas is modelled as a lot of identical */
/* molecules in a 3-D box behaving classically and colliding with each other */
/* and the box in a totally elastic way. */
/* In all these, the RMS speed of molecules in the gas is assumed to be 1. */

/* Update state and return a speed from the distribution */
/* of molecules in the gas. */
double gjrand_mb_speed(struct gjrand *s) GJRAND_THROW_NOTHING;

/* Update state and write a velocity (as a 3-vector) from the distribution */
/* of molecules in the gas into v[]. */
void gjrand_mb_velocity(struct gjrand *s, double v[3]) GJRAND_THROW_NOTHING;

/* Update state and return a speed from the distribution */
/* of gas molecules hitting a wall of the box. */
/* (This is a somewhat different distribution to the one throughout the gas */
/* because a fast molecule is more likely to hit the box than a slow one.) */
double gjrand_mbt_speed(struct gjrand *s) GJRAND_THROW_NOTHING;

/* Update state and write a velocity (as a 3-vector) from the distribution */
/* of gas molecules hitting a wall of the box, into v[]. */
/* v[0] is the velocity component perpendicular to the wall. It will always */
/* be non-negative. */
void gjrand_mbt_velocity(struct gjrand *s, double v[3]) GJRAND_THROW_NOTHING;

/* --- compatibility functions: rand() family --- */

/* this family could replace rand() and srand() which are the standard */
/* C random number generator in SVID 3, BSD 4.3, ISO 9899, and in fact */
/* going back to the Bell Labs C library in the 1970s. */
/* These are not thread safe in that the random state is stored in a */
/* global variable. This can cause subtle problems with repeatability */
/* and perhaps other things, if multiple threads are all calling rand() */
/* and particularly srand(). These replacements faithfully simulate these */
/* subtle problems. The underlying generator is probably different to the */
/* one in your C library, and GJRAND_RAND_MAX is not guaranteed to be the */
/* same as your local RAND_MAX. */

#define GJRAND_RAND_MAX 0x7fffffff

/* Seed the generator. This only affects functions in this family. */
/* Something equivalent to gjrand_oldsrand(1) happens implicitly before */
/* program start, so explicit seeding is not absolutely compulsory. */
void gjrand_oldsrand(unsigned int seed) GJRAND_THROW_NOTHING;

/* return a uniform random integer in the range [ 0 .. GJRAND_RAND_MAX ] */
int gjrand_oldrand(GJRAND_NO_ARGS) GJRAND_THROW_NOTHING;


/* --- compatibility functions: rand48() family --- */

/* This family could replace the rand48 family which was introduced in */
/* an early version of SYSV (TM), but is now sadly deprecated. Clones of */
/* those functions also occur in many other C library implementations. */
/* These have some merit in that the first three functions are threadsafe, */
/* unlike other "standard" random number interfaces. The functions work on */
/* a fixed cycle of 2**48 random numbers and can be started anywhere in the */
/* cycle. This implementation uses a different sequence to the one used */
/* in the original SYSV implementation and most clones. Also the drand48() */
/* and erand48() replacements have 64 significant bits absolute or 53 */
/* relative whichever is less. The originals apparently had 48 bits absolute. */

/* The first 3 are threadsafe. You have to initialise the xsubi[3] array */
/* for yourself before the first call. The functions will update xsubi. */

/* update xsubi[] and return a random float in [0.0, 1.0). */
double gjrand_erand48(unsigned short int xsubi[3]) GJRAND_THROW_NOTHING;

/* update xsubi[] and return a random integer in [ 0 , 2**31-1 ]  */
long int gjrand_nrand48(unsigned short int xsubi[3]) GJRAND_THROW_NOTHING;

/* update xsubi[] and return a random integer in [ -2**31 , 2**31-1 ]  */
long int gjrand_jrand48(unsigned short int xsubi[3]) GJRAND_THROW_NOTHING;

/* The remainder are not threadsafe. They store state in a global buffer. */
/* One of the seeding functions should be called first to initialise the */
/* buffer. These seeding functions only affect the functions in this family */
/* and only those below this point. */

/* Set the high order 32 bits of the global buffer from seedval and the low */
/* order 16 bits to 0. (SYSV had a non-zero constant.) */
void gjrand_srand48(long int seedval) GJRAND_THROW_NOTHING;

/* Set all 48 bits of the global buffer and return a pointer to the */
/* global buffer. */
unsigned short int *gjrand_seed48(unsigned short int seed16v[3])
	GJRAND_THROW_NOTHING;

/* Set all 48 bits of the global buffer from elements 0 to 2. */
/* Use elements 3 to 6 to choose from 2**64 different cycles. The default */
/* cycle used with other initialisation functions is equivalent to setting */
/* elements 3 to 6 all to 0. (In the SYSV implementation the default cycle */
/* used non-zero constants. In the SYSV implementation the quality of random */
/* numbers generated was very dependent on the values chosen for elements */
/* 3 to 6. Some choices (for instance elements 3 to 6 all zeroes) */
/* were totally useless as pseudo-random numbers. */
/* In gjrand, any values should result in acceptable quality.) */
void gjrand_lcong48(unsigned short int param[7]) GJRAND_THROW_NOTHING;

/* update global buffer and return a random float in [0.0, 1.0). */
double gjrand_drand48(GJRAND_NO_ARGS) GJRAND_THROW_NOTHING;

/* update global buffer and return a random integer in [ 0 , 2**31-1 ]  */
long int gjrand_lrand48(GJRAND_NO_ARGS) GJRAND_THROW_NOTHING;

/* update global buffer and return a random integer in [ -2**31 , 2**31-1 ]  */
long int gjrand_mrand48(GJRAND_NO_ARGS) GJRAND_THROW_NOTHING;


/* --- deprecated functions: don't use these in new programs. --- */

/* currently none. */

/* see also goneaway.h for functions that have actually gone away. */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _GJRAND_H */
