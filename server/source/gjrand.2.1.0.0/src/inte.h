/* Part of gjrand random number library version 2.0.0.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

/* Contains stuff used internally in the library. */
/* Application programmers keep out. */

#define GJRAND_INTE_INCR (987654321ul)

#define GJRAND_INTE_INCR_INV (13958015002975068241ul)

/* sqrt(1.0/3.0) probably accurate to about 20 places */
/* (more than enough for double) */
#define GJRAND_INTE_SQRT1_3 (0.57735026918962576450651)

void gjrand_inte_crank64(struct gjrand *g, uint64_t b[4])
	GJRAND_THROW_NOTHING;
uint64_t gjrand_inte_l64(const unsigned char b[8])
	GJRAND_THROW_NOTHING;
void gjrand_inte_s64(const uint64_t i, unsigned char *b)
	GJRAND_THROW_NOTHING;

void gjrand_inte_hash32_8(const unsigned char in[32], unsigned char out[8])
	GJRAND_THROW_NOTHING;

void gjrand_inte_sphere(struct gjrand *s, int d, double *r, double x)
	GJRAND_THROW_NOTHING;

extern struct gjrand gjrand_inte_oldrand_state;

extern unsigned short gjrand_inte_rand48_state[7];

void gjrand_inte_rand48_set7(struct gjrand *g, const unsigned short x[7])
	GJRAND_THROW_NOTHING;
void gjrand_inte_rand48_set3(struct gjrand *g, const unsigned short x[3])
	GJRAND_THROW_NOTHING;
void gjrand_inte_rand48_update(const struct gjrand *g, unsigned short x[3])
	GJRAND_THROW_NOTHING;

double gjrand_inte_logfac(int x) GJRAND_THROW_NOTHING;
int gjrand_inte_binom(struct gjrand *state, double prob, int n)
	GJRAND_THROW_NOTHING;
