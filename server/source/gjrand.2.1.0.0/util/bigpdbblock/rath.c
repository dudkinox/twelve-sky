/* Test software for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "../src/my_int_types.h"

#include "../testcommon/pcombo.h"

#ifdef __cplusplus
#define NOARGS
#else
#define NOARGS void
#endif

#if defined(__cplusplus) || defined(__GNUC__)
#define INLINE inline
#else
#define INLINE
#endif

#define SIZE 204800
#define COUNT 250

static void
crash(char *s)
{
    fprintf(stderr,"bigpdb crashing: [%s]\n", s);
    exit(1);
}


/* blocks */

static double blockstat[9][4];
static double ablockstat[9][4][COUNT];

static void
block16(double b[16], double s[4])
{
    double x, t, s0=0.0, s1= -16.0, s2=0.0, s3= -48.0;
    int i;

    for (i=0; i<16; i++)
    {
	x=b[i]; s0+=x;
	t=x*x; s1+=t;
	t*=x; s2+=t;
	t*=x; s3+=t;
    }

    s[0]=s0; s[1]=s1; s[2]=s2; s[3]=s3;

    blockstat[0][0] += s0*s0;
    blockstat[0][1] += s1*s1;
    blockstat[0][2] += s2*s2;
    blockstat[0][3] += s3*s3;
}

static void
block32(double b[32], double s[4])
{
    double s2[4], t;

    block16(b, s); block16(b+16, s2);
    t=s[0]+s2[0]; s[0]=t; blockstat[1][0]+=t*t;
    t=s[1]+s2[1]; s[1]=t; blockstat[1][1]+=t*t;
    t=s[2]+s2[2]; s[2]=t; blockstat[1][2]+=t*t;
    t=s[3]+s2[3]; s[3]=t; blockstat[1][3]+=t*t;
}

static void
block64(double b[64], double s[4])
{
    double s2[4], t;

    block32(b, s); block32(b+32, s2);
    t=s[0]+s2[0]; s[0]=t; blockstat[2][0]+=t*t;
    t=s[1]+s2[1]; s[1]=t; blockstat[2][1]+=t*t;
    t=s[2]+s2[2]; s[2]=t; blockstat[2][2]+=t*t;
    t=s[3]+s2[3]; s[3]=t; blockstat[2][3]+=t*t;
}

static void
block128(double b[128], double s[4])
{
    double s2[4], t;

    block64(b, s); block64(b+64, s2);
    t=s[0]+s2[0]; s[0]=t; blockstat[3][0]+=t*t;
    t=s[1]+s2[1]; s[1]=t; blockstat[3][1]+=t*t;
    t=s[2]+s2[2]; s[2]=t; blockstat[3][2]+=t*t;
    t=s[3]+s2[3]; s[3]=t; blockstat[3][3]+=t*t;
}

static void
block256(double b[256], double s[4])
{
    double s2[4], t;

    block128(b, s); block128(b+128, s2);
    t=s[0]+s2[0]; s[0]=t; blockstat[4][0]+=t*t;
    t=s[1]+s2[1]; s[1]=t; blockstat[4][1]+=t*t;
    t=s[2]+s2[2]; s[2]=t; blockstat[4][2]+=t*t;
    t=s[3]+s2[3]; s[3]=t; blockstat[4][3]+=t*t;
}

static void
block512(double b[512], double s[4])
{
    double s2[4], t;

    block256(b, s); block256(b+256, s2);
    t=s[0]+s2[0]; s[0]=t; blockstat[5][0]+=t*t;
    t=s[1]+s2[1]; s[1]=t; blockstat[5][1]+=t*t;
    t=s[2]+s2[2]; s[2]=t; blockstat[5][2]+=t*t;
    t=s[3]+s2[3]; s[3]=t; blockstat[5][3]+=t*t;
}

static void
block1024(double b[1024], double s[4])
{
    double s2[4], t;

    block512(b, s); block512(b+512, s2);
    t=s[0]+s2[0]; s[0]=t; blockstat[6][0]+=t*t;
    t=s[1]+s2[1]; s[1]=t; blockstat[6][1]+=t*t;
    t=s[2]+s2[2]; s[2]=t; blockstat[6][2]+=t*t;
    t=s[3]+s2[3]; s[3]=t; blockstat[6][3]+=t*t;
}

static void
blocks(double b[2048])
{
    double s[4], s2[4], t;

    block1024(b, s); block1024(b+1024, s2);
    t=s[0]+s2[0]; blockstat[7][0]+=t*t;
    t=s[1]+s2[1]; blockstat[7][1]+=t*t;
    t=s[2]+s2[2]; blockstat[7][2]+=t*t;
    t=s[3]+s2[3]; blockstat[7][3]+=t*t;
}

static void
maxblocks(void)
{
    int i, j;
    double t, x;

    for (j=0; j<4; j++)
    {
	t=0.0;
	for (i=0; i<8; i++)
	{
	    x=fabs(blockstat[i][j]);
	    if (x>t) t=x;
	}
	blockstat[8][j]=t;
    }
}

static void
copyblocks(int n)
{
    int i, j;

    for (i=0; i<9; i++) for (j=0; j<4; j++) ablockstat[i][j][n]=blockstat[i][j];
}

static void
statblocks(int64_t size)
{
    int i, j;
    double s;

    size/=2048; size*=2048;
    s=1.0/size;

    for (i=0; i<8; i++)
	for (j=0; j<4; j++) blockstat[i][j] *= s;

    for (i=0; i<8; i++)
    {
	blockstat[i][0]-=1.0; blockstat[i][1]-=2.0;
	blockstat[i][2]-=15.0; blockstat[i][3]-=96.0;
    }

    s=sqrt((double)size)*0.25;
    for (i=0; i<8; i++)
    {
	for (j=0; j<4; j++) blockstat[i][j] *= s;
	s*=M_SQRT1_2;
    }

    for (i=0; i<8; i++)
    {
	blockstat[i][0]*=(1.0/1.4142);
	blockstat[i][1]*=(1.0/7.4833);
	blockstat[i][2]*=(1.0/100.85);
	blockstat[i][3]*=(1.0/1377.9);
    }

    maxblocks();
}

static void
printblocks(double pc)
{
    int i, j, b, pi;

    printf("Blocks (%.6f) ===============\n", pc);

    pi=pc*COUNT; if (pi<0) pi=0; if (pi>=COUNT) pi=COUNT-1;

    printf("%4s  %12s %12s %12s %12s\n",
	"size", "mean", "variance", "3rd moment", "4th moment");
    b=16;
    for (i=0; i<8; i++)
    {
	printf("%4d  ", b); b+=b;
	for (j=0; j<4; j++) printf("%12.6f ", ablockstat[i][j][pi]);
	putchar('\n');
    }

    printf("%4s  ", "max");
    for (j=0; j<4; j++) printf("%12.6f ", ablockstat[8][j][pi]);
    putchar('\n');
    printf("End blocks ===============\n\n");
}

/* end blocks */

static int64_t
update(int64_t max)
{
    int64_t r=0;
    double buffer[64+2048];
#define b (buffer+64)

    while (max<0 || r<max)
    {
	//int i;
	int c, t;
	t=2048; if (max>0 && t>max-r) t=max-r;
	c=fread(b, sizeof(double), t, stdin);
	if (c==0) break;
	if (c<0) crash("fread fails");

	if (c==2048) blocks(b);

	r+=c;
    }

#undef b

    if (max>0 && r<max)
	printf("Warning: expected %.0f variates, saw only %.0f\n\n",
		(double)max, (double)r);

    return r;
}

static void
init(NOARGS)
{
    int i, j;

    for (i=0; i<9; i++) for (j=0; j<4; j++) blockstat[i][j]=0.0;
}

int
dcomp(const void *a, const void *b)
{
    const double *da=a, va= *da, *db=b, vb= *db;

    if (va<vb) return -1;
    if (va>vb) return 1;
    return 0;
}

void
dsort(double a[COUNT])
{
    qsort(a, COUNT, sizeof(double), dcomp);
}

int
main(int argc, char **argv)
{
    int i, j;

    for (i=0; i<COUNT; i++)
    {
	init();
	update(SIZE);
	statblocks(SIZE);
	copyblocks(i);
    }

    for (i=0; i<9; i++) for (j=0; j<4; j++) dsort(&(ablockstat[i][j][0]));

    printblocks(0.001);
    printblocks(0.01);
    printblocks(0.1);
    printblocks(0.5);
    printblocks(0.9);
    printblocks(0.99);
    printblocks(0.999);

    return 0;
}
