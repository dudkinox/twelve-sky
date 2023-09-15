/* Test software for gjrand random numbers version 1.3.3.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "../src/my_int_types.h"
#include "../testcommon/pcombo.h"
#include "../testcommon/chi2p.h"

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

static void
crash(char *s)
{
    fprintf(stderr,"bigpdb crashing: [%s]\n", s);
    exit(1); /* possibly abort() when debugging */
}

static double pvals[99];
static int pvalp=0;

static void
appendpv(double p)
{
    if (pvalp>90) crash("too many P values");
    pvals[pvalp++]=p;
}

/* chisquared */

static double
xsim(double lo, double hi, int n)
{
    double del=(hi-lo)/(double)n;
    double del2=del*0.5;
    double r=0.0;
    int i;

    for (i=n-1; i>=0; i--)
    {
	double x=lo+i*del;
	double x2=x+del;
	r+=exp(x2*x2*-0.5);
	r+=exp(x*x*-0.5);
	x2=x+del2;
	r+=4.0*exp(x2*x2*-0.5);
    }

    return r*del/6.0;
}

static void
mkexpect(double buckets[], int n, double delta, int64_t samples)
{
    double x;
    int i;
    int split=16*1024;

    while (split>500 && n*split>3000000) split>>=1;

    for (i=0; i<n; i++)
    {
	x=xsim(i*delta, (i+1)*delta, split);
	buckets[n-i]=x;
	buckets[1+n+i]=x;
    }

    x=xsim(n*delta, n*delta+8.0, 64*1024);
    buckets[0]=x;
    buckets[2*n+1]=x;

    x=0.0;
    for (i=2*n+1; i>=0; i--) x+=buckets[i];
    x=(double)samples/x;
    for (i=2*n+1; i>=0; i--) buckets[i]*=x;
}

static int64_t bucketa[34], bucketb[194], bucketc[1282], bucketd[10002];

static INLINE
void uchi(double x)
{
    int j=(int)floor(4096.0*x);
    int k;

    k=(j>>10)+17; if (k<0) k=0; if (k>33) k=33; bucketa[k]++;
    k=(j>>7)+97; if (k<0) k=0; if (k>193) k=193; bucketb[k]++;
    k=(j>>4)+641; if (k<0) k=0; if (k>1281) k=1281; bucketc[k]++;
    k=j+5001; if (k<0) k=0; if (k>10001) k=10001; bucketd[k]++;
}

static double
dochis(int64_t buckets[], int n, int invdel, int64_t count)
{
    double ebuckets[11000];
    double chis;
    double delta=1.0/(double)invdel;
    int i;

    if (n>5200) crash("dochis too many buckets");

    printf("Chisquare %d / %d ==============\n", n, invdel);

    mkexpect(ebuckets, n, delta, count);

    chis=0.0;
    for (i=0; i<=2*n+1; i++)
    {
	double x=buckets[i]-ebuckets[i];
	chis+=x*x/ebuckets[i];
    }

    printf("chis=%f\n", chis);

    return chis;
}

static void
chispval(double c, int df)
{
    double pv;

    pv = chi2p2(c, df);
    printf("One sided p-value (very small numbers are bad):\n");
    printf("P = %.3g\n", pv);

    appendpv(pv);

    printf("End chisquare ==============\n\n");
}

static void
printchis(int64_t count)
{
    double c;

    c=dochis(bucketa, 16, 4, count);
    chispval(c, 33);
    c=dochis(bucketb, 96, 32, count);
    chispval(c, 193);
    c=dochis(bucketc, 640, 256, count);
    chispval(c, 1281);
    c=dochis(bucketd, 5000, 4096, count);
    chispval(c, 10001);
}

/* end chisquared */

/* double chisquared */

static int64_t chis2b[50][50];

static void
mkexpect2(double ebuckets[50][50], int n, double delta, int64_t samples)
{
    double e1[50];
    int i, j;
    mkexpect(e1, n, delta, 1);
    for (i=0; i<=2*n+1; i++) for (j=0; j<=2*n+1; j++)
	ebuckets[i][j]=(samples-1)*e1[i]*e1[j];
}

static INLINE int
uchis2(int64_t b[50][50], int n, int was, double mul, double x)
{
    int i=(int)floor(x*mul)+n+1;

    if (i<0) i=0; if (i>2*n+1) i=2*n+1;
    b[was][i]++;

    return i;
}

static void
printchis2(int64_t c)
{
    double ebuckets[50][50];
    int i, j;
    double chis=0.0;

    printf("Double chisquare 24 / 16 ==============\n");

    mkexpect2(ebuckets, 24, 1.0/16.0, c);

    for (i=0; i<=49; i++) for (j=0; j<=49; j++)
    {
	double e=ebuckets[i][j];
	double x=chis2b[i][j]-e;
	chis+=x*x/e;
    }

    printf("chis=%f\n", chis);

    chispval(chis, 2499);
}

/* end double chisquared */

/* lowbits */

static int64_t lbucket2[65536], lbucket12[65536], lbucket22[65536],
	lbucket32[65536];

static INLINE void
ulb(int64_t bucket[], double mul, double x)
{
    int j;

    x*=mul;
    x-=floor(x);
    j=(int)(floor(x*65536.0));
    if (j<0) j=0; if (j>=65536) j=65535;
    bucket[j]++;
}

static void
printlow(int64_t bucket[], int shift, int64_t siz)
{
    int i;
    double sum=0.0, chis=0.0, expect=siz/65536.0, pv;
    int64_t hi= -1, lo=1LL<<60;

    printf("Low order bits %d =====================\n", shift);

    for (i=0; i<65536; i++)
    {
	int64_t c=bucket[i];
	double d;
	if (c>hi) hi=c;
	if (c<lo) lo=c;
	d=c;
	sum+=d;
	d-=expect;
	chis+=d*d;
    }

    chis/=expect;

    if (fabs(sum-siz)>1e-10) printf("buckets don't add up!\n");
    printf("chis = %.6f\n", chis);
    printf("high = %.0f\n", (double)hi);
    printf("low  = %.0f\n", (double)lo);

    pv = chi2p2(chis, 65535);

    printf("One sided p-value (very small numbers are bad):\n");
    printf("P = %.3g\n", pv);

    appendpv(pv);

    printf("End low order bits %d =====================\n\n", shift);
}

static void
printlows(int64_t c)
{
    printlow(lbucket2, 2, c);
    printlow(lbucket12, 12, c);
    printlow(lbucket22, 22, c);
    printlow(lbucket32, 32, c);
}

/* end lowbits */

/* moments */

static double moments[13];

static INLINE void
umoments(double x)
{
    double y=x;
    int i;

    i=1;
    goto start;
    do
    {
	y*=x;
	start:
	moments[i]+=y;
	i++;
    } while (i<=8);
}

static void
printmoments(int64_t c)
{
    double pv[13], pc;
    double x, s, y;
    int i;

    printf("Moments =======================\n");

    x=1.0/(double)c;
    s=sqrt((double)c);
    y=1.0;
    for (i=1; i<=8; i++)
    {
	static const short m[] = {0, 0, 1, 0, 3, 0, 15, 0, 105};
	double z, z3;
	y *= 2*i-1;
	z = moments[i]*x;
	z3 = (z-m[i])*s/sqrt(y);
	pv[i] = erfc(fabs(M_SQRT1_2*z3));
	printf("%2d: %15.8f %15.8f %.3g\n", i, z, z3, pv[i]);
    }

    pc=pcombo(pv+1, 8);

    printf("One sided p-value (very small numbers are bad):\n");
    printf("P = %.3g\n", pc);

    appendpv(pc);

    printf("End moments =======================\n\n");
}

/* end moments */

/* nda */

static int
tohextile(double x)
{
    int s=0;
    if (x<0.0) {s=8; x= -x;}
    if (x<0.6744897501961370)
    {
	if (x<0.3186393639644102)
	{
	    if (x>=0.1573106846101825) s++;
	    return s;
	}
	else
	{
	    s+=2;
	    if (x>=0.4887764111147070) s++;
	    return s;
	}
    }
    else
    {
	s+=4;
	if (x<1.1503493803761058)
	{
	    if (x>=0.8871465590189737) s++;
	    return s;
	}
	else
	{
	    s+=2;
	    if (x>=1.5341205443525574) s++;
	    return s;
	}
    }
}

static int64_t ndacount[49][16][16];
static int64_t ndalast[16];

static void
donibble(int n, int64_t pos)
{
    int i;

    for (i=0; i<16; i++)
    {
	long d=pos-ndalast[i];
	if (d>48) d=0;
	ndacount[d][i][n]++;
    }

    ndalast[n]=pos;
}

static void
mkexpectnda(int64_t size, double expectnda[])
{
    int i;
    double p=1.0, x;

    for (i=1; i<=48; i++)
    {
	x=p*(1.0/16.0);
	expectnda[i]=x;
	p-=x;
    }
    expectnda[0]=p;

    p=(double)size/16.0-1.0;
    for (i=0; i<=48; i++) expectnda[i]*=p;
}

static void
printnda(int64_t size)
{
    double expectnda[50];
    double t=0.0, pv, x;
    int i, j, k;

    printf("Nearest distance analysis ===============\n");

    mkexpectnda(size, expectnda);
    for (i=0; i<=48; i++) for (j=0; j<16; j++) for (k=0; k<16; k++)
    {
	double e=expectnda[i];
	x=(double)ndacount[i][j][k];
	x-=e;
	t+=x*x/e;
    }

    printf("nda = %15.5f\n", t);

    x = (t - 12761) * ((82.001-60.332)/(13379-12761)) + 60.332;
    if (x<0.0) x = 0.0;
    pv = chi2p1(x, 52);

    printf("One sided p-value (very small numbers are bad):\n");
    printf("P = %.3g\n", pv);

    appendpv(pv);

    printf("End nearest distance analysis ===============\n\n");
}

/* end nda */


/* self-correlation */

static double corr[64];

static INLINE void
docorr(double *p)
{
    double x= *p;
    int i;

    for (i= -64; i<0; i++) (corr+64)[i]+=p[i]*x;
}

static void
printcorr(int64_t size)
{
    int i, clow[3], chigh[3];
    double low[3], high[3], s=0.0, ss=0.0, pv;

    printf("Self correlation ===============\n");
    size-=1024;
    if (size<256) goto ret;

    low[0]=low[1]=low[2]=1e30;
    high[0]=high[1]=high[2]= -1e30;

    for (i=0; i<64; i++)
    {
	double x=corr[i]/sqrt((double)size);

	corr[i]=x;

	printf("%8.3f ", x);
	if ((i&7)==7) putchar('\n');

	if (x>high[2])
	{
	    high[2]=x; chigh[2]=i;
	    if (x>high[1])
	    {
		high[2]=high[1]; chigh[2]=chigh[1];
		high[1]=x; chigh[1]=i;
		if (x>high[0])
		{
		    high[1]=high[0]; chigh[1]=chigh[0];
		    high[0]=x; chigh[0]=i;
		}
	    }
	}
	if (x<low[2])
	{
	    low[2]=x; clow[2]=i;
	    if (x<low[1])
	    {
		low[2]=low[1]; clow[2]=clow[1];
		low[1]=x; clow[1]=i;
		if (x<low[0])
		{
		    low[1]=low[0]; clow[1]=clow[0];
		    low[0]=x; clow[0]=i;
		}
	    }
	}
	s+=x; ss+=x*x;
    }

    for (i=0; i<3; i++) {chigh[i]=64-chigh[i]; clow[i]=64-clow[i];}

    printf("\nmean=%f\n", s/64.0);
    printf("sd  =%f\n", sqrt(ss/64.0-s*s/4096.0));
    printf("high=%f (%d) %f (%d) %f (%d)\n",
	high[0], chigh[0], high[1], chigh[1], high[2], chigh[2]);
    printf("low =%f (%d) %f (%d) %f (%d)\n",
	low[0], clow[0], low[1], clow[1], low[2], clow[2]);

    pv = -low[0]; if (high[0]>pv) pv=high[0];
    pv=erfc(M_SQRT1_2*pv)*0.5;
    pv=1.0-pow(1.0-pv, 128);

    printf("One sided p-value (very small numbers are bad):\n");
    printf("P = %.3g\n", pv);

    appendpv(pv);

    ret:
    printf("End self correlation ===============\n\n");
}

/* end self-correlation */


/* blocks */

static double blockstat[8][4];

static const double sdstat[8][4]=
{
    {1.42, 3.3, 32, 370},
    {1.42, 3.0, 27, 280},
    {1.42, 2.9, 25, 220},
    {1.42, 2.8, 23, 180},
    {1.42, 2.8, 22, 160},
    {1.42, 2.8, 22, 150},
    {1.42, 2.8, 21, 140},
    {1.42, 2.8, 21, 139}
};

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
printblocks(int64_t size)
{
    double p[4], max[4], s, pc;
    int i, j, b;

    printf("Blocks ===============\n");

    size &= ~((int64_t)2047);
    s=1.0/size;

    for (i=0; i<8; i++) for (j=0; j<4; j++) blockstat[i][j] *= s;

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

    for (i=0; i<8; i++) for (j=0; j<4; j++) blockstat[i][j]/=sdstat[i][j];

    printf("%4s  %12s %12s %12s %12s\n",
	"size", "mean", "variance", "3rd moment", "4th moment");
    b=16;
    for (i=0; i<8; i++)
    {
	printf("%4d  ", b); b+=b;
	for (j=0; j<4; j++) printf("%12.6f ", blockstat[i][j]);
	putchar('\n');
    }

    for (j=0; j<4; j++)
    {
	s=fabs(blockstat[0][j]);
	for (i=1; i<8; i++)
	{
	    double t=fabs(blockstat[i][j]);
	    if (t>s) s=t;
	}
	max[j]=s;
    }
    printf("%4s  ", "max");
    for (j=0; j<4; j++) printf("%12.6f ", max[j]);
    putchar('\n');

    for (j=0; j<4; j++)
    {
	double t=max[j]-1.07;
	p[j]=erfc(M_SQRT1_2*t)*0.5;
    }
    printf("%4s  ", "p");
    for (j=0; j<4; j++) printf("%12.3g ", p[j]);
    putchar('\n');
    pc=pcombo(p, 4);
    printf("One sided p-value (very small numbers are bad):\n");
    printf("P = %.3g\n", pc);
    appendpv(pc);

    printf("End blocks ===============\n\n");
}

/* end blocks */


/* Overall summary */
static void
printsum(NOARGS)
{
    int badness[23];
    double pc;
    int j, worse;

    fprintf(stdout, "\n\n====================\ncompleted %d tests\n", pvalp);
    for (j=0; j<22; j++) badness[j]=0;
    for (j=0; j<pvalp; j++)
    {
	double d=pvals[j], b=0.1;
	int k=0;

	while (d<b && k<20) {b*=0.1; k++;}
	if (d==0.0) k=21;
	badness[k]++;
    }

    fprintf(stdout, "%d out of %d tests ok.\n", badness[0], pvalp);
    worse=pvalp-badness[0];

    for (j=1; j<22; j++) if (badness[j])
    {
	static const int badmax[5][2]=
	{
	    {99, 99},
	    {3, 7},
	    {1, 3},
	    {0, 1},
	    {0, 1},
	};
	char *msg;

	if (j>4 || worse>badmax[j][1]) msg="(SERIOUSLY BAD)";
	else if (worse>badmax[j][0]) msg="(slightly bad)";
	else msg="(probably ok)";

	fprintf(stdout, "%d grade %d failures %s.\n", badness[j], j, msg);

	worse-=badness[j];
    }

if (worse!=0) fprintf(stderr, "worse=%d huh?\n", worse);

    pc=pcombo(pvals, pvalp);
    fprintf(stdout,
"\n\nOverall summary one sided P-value (smaller numbers bad)\nP = %.3g\n",
pc);

}

/* end overall summary */

static int64_t
update(int64_t max)
{
    int64_t r=0;
    double buffer[64+2048];
#define b (buffer+64)
    int wasuchi2=0;
    int first=1;

    while (max<0 || r<max)
    {
	int i;
	int c, t;
	t=2048; if (max>0 && t>max-r) t=max-r;
	c=fread(b, sizeof(double), t, stdin);
	if (c==0) break;
	if (c<0) crash("fread fails");

	for (i=0; i<c; i++) uchi(b[i]);

	for (i=0; i<c; i++) wasuchi2=uchis2(chis2b, 24, wasuchi2, 16.0, b[i]);
	for (i=0; i<c; i++) ulb(lbucket2, (double)(1<<2), b[i]);
	for (i=0; i<c; i++) ulb(lbucket12, (double)(1<<12), b[i]);
	for (i=0; i<c; i++) ulb(lbucket22, (double)(1<<22), b[i]);
	for (i=0; i<c; i++) ulb(lbucket32, (65536.0*65536.0), b[i]);
	for (i=0; i<c; i++) umoments(b[i]);
	for (i=0; i<c; i++) donibble(tohextile(b[i]), r+i);

	if (!first) for (i=0; i<c; i++) docorr(b+i);
	for (i=0; i<64; i++) buffer[i]=(b+c-64)[i];
	first=0;

	if (c==2048) blocks(b);

	r+=c;
    }

#undef b

    if (max>0 && r<max)
	printf("Warning: expected %.0f variates, saw only %.0f\n\n",
		(double)max, (double)r);
    else printf("Successfully read %.0f variates.\n\n", (double)r);

    return r;
}

static void
init(NOARGS)
{
    int i, j;

    for (i=0; i<64; i++) corr[i]=0.0;
    for (i=0; i<8; i++) for (j=0; j<4; j++) blockstat[i][j]=0.0;
    memset(bucketa, 0, sizeof(bucketa));
    memset(bucketb, 0, sizeof(bucketb));
    memset(bucketc, 0, sizeof(bucketc));
    memset(bucketd, 0, sizeof(bucketd));
    memset(chis2b, 0, sizeof(chis2b));
    memset(lbucket2, 0, sizeof(lbucket2));
    memset(lbucket12, 0, sizeof(lbucket12));
    memset(lbucket22, 0, sizeof(lbucket22));
    memset(lbucket32, 0, sizeof(lbucket32));
    memset(moments, 0, sizeof(moments));
    memset(ndalast, 0, sizeof(ndalast));
    memset(ndacount, 0, sizeof(ndacount));
}

static void
usage(NOARGS)
{
    crash("zero or one args. Variates are read from stdin.\n"
	  "--tiny          1M variates\n"
	  "--small         10M variates\n"
	  "--standard      100M variates\n"
	  "--big           1G variates\n"
	  "--huge          16G variates\n"
	  "--even-huger    128G variates\n"
	  "--tera          1T variates\n"
	  "otherwise read until end of file.\n");
}

#define K ((int64_t)1024)
#define M (K*K)
#define G (K*M)
#define TERA (K*G)

#define TINY (M)
#define SMALL (10*M)
#define STANDARD (100*M)
#define BIG (G)
#define REALLYHUGE (16*G)
#define HUGER (128*G)

int
main(int argc, char **argv)
{
    int64_t count, c=(int64_t)(-1);

    init();

    if (argc==2)
    {
	if (strcmp(argv[1], "--tiny")==0) c=TINY;
	else if (strcmp(argv[1], "--small")==0) c=SMALL;
	else if (strcmp(argv[1], "--standard")==0) c=STANDARD;
	else if (strcmp(argv[1], "--big")==0) c=BIG;
	else if (strcmp(argv[1], "--huge")==0) c=REALLYHUGE;
	else if (strcmp(argv[1], "--even-huger")==0) c=HUGER;
	else if (strcmp(argv[1], "--tera")==0) c=TERA;
	else usage();
    }
    else if (argc!=1) usage();

    printf("*** bigpdb version 4");
    if (argc==2) printf(" %s", argv[1]);
    printf(" ***\n\n");

    count=update(c);

    printlows(count);
    printmoments(count);
    printchis(count);
    printchis2(count);
    printnda(count);
    printcorr(count);
    printblocks(count);

    printsum();

    return 0;
}
