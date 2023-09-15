/* Test software for gjrand random numbers version 1.3.3.0 or later. */
/* Copyright (C) 2004-2008 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <errno.h>

#include "../testcommon/pcombo.h"
#include "../testcommon/chi2p.h"

#ifdef __cplusplus
#define NO_ARGS
#else
#define NO_ARGS void
#endif

static void
crash(char *s)
{
    if (errno)
    {
	fprintf(stderr,"errno=%d", errno);
	perror(" ");
    }
    fprintf(stderr,"pdb crashing: [%s]\n", s);
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

/* Put normally distributed random numbers into the first count elements */
/* of buf. This reads from standard input. */
/* Some of the tests assume mean=0.0, sd=1.0, so normalise to that */
/* if your generator does something different. */
static void
fill_normal(int count, double buf[])
{
    /* fill array with random numbers. */
    if (fread(buf, sizeof(double), count, stdin)!=(size_t)count) crash("fread");
}

#define SIZ (4*1024*1024)

static double buf[SIZ+5];
static double buf2[SIZ+5];

/* self-correlation */

/* self-correlation is currently quite slow, so we use a smaller data set */
#define CSIZ (256*1024)

static double rbuf[CSIZ+5], productbuf[2*CSIZ+10];
static double tmpbuf[4*CSIZ+5], *tmpptr=tmpbuf;

static inline double *
mark(int siz)
{
    double *r=tmpptr;
    tmpptr=r+siz+3;
    if (tmpptr>=tmpbuf+4*CSIZ) crash("mark out of memory");
    return r;
}

static inline void
release(double *m)
{
    if (m<tmpbuf || m>=tmpptr) crash("release with bad pointer");
    tmpptr=m;
}

static void
polysub2(double *ca, double *cr, int ord)
{
    int i;
    for (i=0; i<ord; i++) cr[i]-=ca[i];
}

static void
polyadd2(double *ca, double *cr, int ord)
{
    int i;
    for (i=0; i<ord; i++) cr[i]+=ca[i];
}

static void
polyadd3(double *ca, double *cb, double *cr, int ord)
{
    int i;
    for (i=0; i<ord; i++) cr[i]=cb[i]+ca[i];
}

static void
polymul3(double *ca, double *cb, double *cr, int ord)
{
    int i,j;

    ca+=ord; cb+=ord; cr+=2*ord;

    for (j= -2*ord; j<0; j++) cr[j]=0.0;

    cr-=ord;

    for (i= -ord; i<0; i++)
    {
	double n=ca[i];
	j= -ord;
	do
	{
	    cr[j]+=cb[j]*n;
	    j++;
	} while (j<0);

	cr++;
    }
}

static void
polymul2(double *ca, double *cb, double *cr, int ord)
{
    if (ord<=64) {polymul3(ca, cb, cr, ord); return;}

    {
	int i;
	double *t1, *t3;

	if (ord&1) crash("polymul2 odd size");

	for (i=0; i<2*ord; i++) cr[i]=0;

	t1=mark(ord);

	polymul2(ca, cb, t1, ord/2);
	polyadd2(t1, cr, ord);
	polysub2(t1, cr+ord/2, ord);

	polymul2(ca+ord/2, cb+ord/2, t1, ord/2);
	polyadd2(t1, cr+ord, ord);
	polysub2(t1, cr+ord/2, ord);

	polyadd3(ca, ca+ord/2, t1, ord/2);
	polyadd3(cb, cb+ord/2, t1+ord/2, ord/2);

	t3=mark(ord);
	polymul2(t1, t1+ord/2, t3, ord/2);

	polyadd2(t3, cr+ord/2, ord);

	release(t1);
    }
}

static void
cor_printstats(double p[], int order)
{
    int i, count, clow[3], chigh[3];
    double x, low[3], high[3];
    double s=0.0, ss=0.0;

    low[0]=low[1]=low[2]=1e38;
    high[0]=high[1]=high[2]= -1e38;

    for (i=order/4-1; i<order/2-1; i++)
    {
	x=p[i]/sqrt((double)i);
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

    for (i=0; i<3; i++) {chigh[i]=order/2-chigh[i]; clow[i]=order/2-clow[i];}

    count=order/4;

    printf("mean=%f\n", s/count);
    printf("sd  =%f\n", sqrt(ss/count-s*s/((double)count*(double)count)));
    printf("high=%f (%d) %f (%d) %f (%d)\n",
	high[0], chigh[0], high[1], chigh[1], high[2], chigh[2]);
    printf("low =%f (%d) %f (%d) %f (%d)\n",
	low[0], clow[0], low[1], clow[1], low[2], clow[2]);

    {
	double pv;

	pv = -low[0];
	if (high[0]>pv) pv=high[0];
	pv=erfc(M_SQRT1_2*pv)*0.5;
	pv=1.0-pow(1.0-pv, order/2);

	printf("One sided p-value (very small numbers are bad):\n");
	printf("P = %.3g\n", pv);

	appendpv(pv);
    }
}

static void
selfcor(double buf[], int order)
{
    int i;
    printf("Self-correlation =======================================\n");
    for (i=0; i<order; i++) rbuf[order-i]=buf[i];
    polymul2(buf, rbuf, productbuf, order);
    cor_printstats(productbuf, 2*order);
    printf("End self-correlation =======================================\n\n");
}

/* end self-correlation */

/* lowbits test */
static void
lowbits(double buf[], int siz, int shift)
{
    int i;
    double dsh=ldexp(1.0, shift);
    double chis=0.0, expect=siz/(64.0*1024.0), pv;
    long lbcount[64*1024], hi= -1, lo=0x7fffffff;

    printf("Low order bits %d =====================\n", shift);

    for (i=0; i<64*1024; i++) lbcount[i]=0;

    for (i=0; i<siz; i++)
    {
	int j;
	double x=buf[i]*dsh;
	x=x-floor(x);
	j=(int)(floor(x*(64.0*1024.0)));
	if (j<0) j=0; else if (j>=64*1024) j=64*1024-1;
	lbcount[j]++;
    }

    for (i=0; i<64*1024; i++)
    {
	long c=lbcount[i];
	double d;
	if (c>hi) hi=c;
	if (c<lo) lo=c;
	d=c;
	d-=expect;
	chis+=d*d;
    }
    chis/=expect;

    printf("chis=%10.3f\n", chis);
    printf("high= %ld\n", hi);
    printf("low = %ld\n", lo);

    pv = chi2p2(chis, 65535);

    printf("One sided p-value (very small numbers are bad):\n");
    printf("P = %.3g\n", pv);

    appendpv(pv);

    printf("End low order bits %d =====================\n\n", shift);
}

/* end lowbits */

/* moments */

static double moments[13];

static void
umoments(const double in[], const int count, const int maxm)
{
    int i, j;

    for (j=0; j<=maxm; j++) moments[j]=0.0;

    for (j=0; j<count; j++)
    {
	double x=in[j];
	double y=x;

	i=1;
	goto start;
	do
	{
	    y*=x;
	    start:
	    moments[i]+=y;
	    i++;
	} while (i<=maxm);
    }
}

static void
printmoments(int max)
{
    double pv[13], pc, x, s, y;
    int i;

    if (max>12) crash("printmoments m too big");

    printf("Moments =======================\n");

    umoments(buf, SIZ, max);

    x=1.0/(double)SIZ;
    s=sqrt((double)SIZ);
    y=1.0;
    for (i=1; i<=max; i++)
    {
	static const short m[] =
	    {0, 0, 1, 0, 3, 0, 15, 0, 105, 0, 945, 0, 10395};
	double z, z3;
	y *= 2*i-1;
	z = moments[i]*x;
	z3 = (z-m[i])*s/sqrt(y);
	pv[i] = erfc(fabs(M_SQRT1_2*z3));
	printf("%2d: %15.8f %15.8f %.3g\n", i, z, z3, pv[i]);
    }

    pc=pcombo(pv+1, max);

    printf("One sided p-value (very small numbers are bad):\n");
    printf("P = %.3g\n", pc);

    appendpv(pc);

    printf("End moments =======================\n\n");
}

/* end moments */

/* extrema */

static void
extrema(int n)
{
    double hi, lo, hihi= -999, lohi=999, hilo= -999, lolo=999, pv[4], pc, x, y;
    int i, j, k;

    printf("Extrema ===================\nhigh: ");

    for (i=0; i<SIZ; i+=n)
    {
	k=i+n; if (k>SIZ) k=SIZ;
	hi= -999;
	for (j=i; j<k; j++) if (buf[j]>hi) hi=buf[j];
	printf("%7.3f ",hi);
	if (hi>hihi) hihi=hi;
	if (hi<lohi) lohi=hi;
    }
    printf("\nlow : ");

    for (i=0; i<SIZ; i+=n)
    {
	k=i+n; if (k>SIZ) k=SIZ;
	lo=999;
	for (j=i; j<k; j++) if (buf[j]<lo) lo=buf[j];
	printf("%7.3f ",lo);
	if (lo>hilo) hilo=lo;
	if (lo<lolo) lolo=lo;
    }
    putchar('\n');

    printf("hi %9.3f %9.3f lo %9.3f %9.3f\n", lohi, hihi, lolo, hilo);
    lohi=erfc(M_SQRT1_2*lohi)*0.5; lohi=log1p(-lohi); lohi = exp(lohi*n);
    hihi=erfc(M_SQRT1_2*hihi)*0.5; hihi=log1p(-hihi); hihi = -expm1(hihi*n);
    lolo=erfc(-M_SQRT1_2*lolo)*0.5; lolo=log1p(-lolo); lolo = -expm1(lolo*n);
    hilo=erfc(-M_SQRT1_2*hilo)*0.5; hilo=log1p(-hilo); hilo = exp(hilo*n);
    printf("hi %9.6f %9.6f lo %9.6f %9.6f\n", lohi, hihi, lolo, hilo);

    x=((double)SIZ/n);
    y=pco_scale(lohi, x)*2.0; if (y>1.0) y=2.0-y; pv[0]=y;
    y=pco_scale(hihi, x)*2.0; if (y>1.0) y=2.0-y; pv[1]=y;
    y=pco_scale(lolo, x)*2.0; if (y>1.0) y=2.0-y; pv[2]=y;
    y=pco_scale(hilo, x)*2.0; if (y>1.0) y=2.0-y; pv[3]=y;
    printf("(pv %.3g %.3g %.3g %.3g)\n", pv[0], pv[1], pv[2], pv[3]);

    pc=pcombo(pv, 4);
    printf("One sided p-value (very small numbers are bad):\n");
    printf("P = %.3g\n", pc);
    appendpv(pc);

    printf("End extrema ===================\n\n");
}

/* end extrema */

/* gaps */

static int
dcmp(const void *a, const void *b)
{
    double da=*(const double *)a;
    double db=*(const double *)b;
    if (da<db) return -1;
    if (da>db) return 1;
    return 0;
}

static void
gaps(double lo, double hi, int n)
{
    int histo[256];
    double max= -1e30, rms=0.0, chis=0.0, t, pv;
    double ex=sqrt(2.0*M_PI), rex = -n/ex;
    int i, j, k, zeroes;

    printf("Gaps ===================\n");

    j=0;
    for (i=0; i<n; i++)
    {
	double v=buf[i];
	if (v>lo && v<hi) buf2[j++]=v;
    }

    qsort(buf2, j, sizeof(double), dcmp);

    for (i=0; i<256; i++) histo[i]=0;
    zeroes=0;
    for (i=1; i<j; i++)
    {
	double l=buf2[i-1], h=buf2[i], g=h-l;
	zeroes+=(g==0.0);
	g*=exp(h*l*-0.5);
	if (g>max) max=g;
	rms+=g*g;

	g *= rex;
	k=(int)(floor(exp(g)*256.0));
	histo[k]++;
    }

    rms=sqrt(rms/(j-1));

    ex=j/256.0;
    for (i=0; i<256; i++) {t=histo[i]-ex; chis+=t*t;}
    chis/=ex;

    printf("max = %.4e\nrms = %.4e\nchis = %.4f\nzeroes = %d\n",
	max, rms, chis, zeroes);

    pv = chi2p1(chis, 255);

    printf("One sided p-value (very small numbers are bad):\n");
    printf("P = %.3g\n", pv);

    appendpv(pv);

    printf("End gaps ===================\n\n");
}

/* end gaps */

/* chisquared */

static double
xsim(double lo, double hi, int n)
{
    double del=(hi-lo)/(double)n;
    double del2=del*0.5;
    double r=0.0;
    int i;

    {
	double x=hi-del;
	double x2=hi;
	r+=exp(x2*x2*-0.5);
	r+=2*exp(x*x*-0.5);
	x2=hi-del2;
	r+=4.0*exp(x2*x2*-0.5);
    }

    for (i=n-2; i>0; i--)
    {
	double x=lo+i*del;
	r+=2.0*exp(x*x*-0.5);
	x+=del2;
	r+=4.0*exp(x*x* -0.5);
    }

    {
	double x=lo;
	double x2;
	r+=exp(x*x*-0.5);
	x2=lo+del2;
	r+=4.0*exp(x2*x2*-0.5);
    }

    return r*del/6.0;
}

static void
mkexpect(double buckets[], int n, double delta, int samples)
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

static void
mkobserved(int buckets[], int n, double delta, int samples)
{
    double mul=1.0/delta;
    int i, j;

    for (i=0; i<=2*n+1; i++) buckets[i]=0;

    for (i=0; i<samples; i++)
    {
	j=(int)floor(buf[i]*mul)+n+1;
	if (j<0) j=0; if (j>2*n+1) j=2*n+1;
	buckets[j]++;
    }
}

static double
dochis(int n, int invdel)
{
    int i;
    double ebuckets[11000];
    int buckets[11000];
    double chis;
    double delta=1.0/(double)invdel;

    if (n>5200) crash("dochis too many buckets");

    printf("Chisquare %d / %d ==============\n", n, invdel);

    mkexpect(ebuckets, n, delta, SIZ);
    mkobserved(buckets, n, delta, SIZ);

    chis=0.0;
    for (i=0; i<=2*n+1; i++)
    {
	double x=buckets[i]-ebuckets[i];
	chis+=x*x/ebuckets[i];
    }

    printf("chis=%f\n", chis);

    return chis;
}

/* end chisquared */

/* double chisquared */

static void
mkexpect2(double ebuckets[101][101], int n, double delta, int samples)
{
    double e1[101];
    int i, j;

    mkexpect(e1, n, delta, 1);
    for (i=0; i<=2*n+1; i++) for (j=0; j<=2*n+1; j++)
	ebuckets[i][j]=(samples-1)*e1[i]*e1[j];
}

static void
mkobserved2(int buckets[101][101], int n, double delta, int samples)
{
    double mul=1.0/delta;
    int i, j, k;

    for (i=0; i<=2*n+1; i++) for (j=0; j<=2*n+1; j++) buckets[i][j]=0;

    j=(int)floor(buf[0]*mul)+n+1;
    if (j<0) j=0; if (j>2*n+1) j=2*n+1;

    for (i=1; i<samples; i++)
    {
	k=(int)floor(buf[i]*mul)+n+1;
	if (k<0) k=0; if (k>2*n+1) k=2*n+1;
	buckets[j][k]++;
	j=k;
    }
}

static double
dochis2(int n, int invdel)
{
    int i, j;
    double ebuckets[101][101];
    int buckets[101][101];
    double chis;
    double delta=1.0/(double)invdel;

    if (n>48) crash("dochis too many buckets");

    printf("Double chisquare %d / %d ==============\n", n, invdel);

    mkexpect2(ebuckets, n, delta, SIZ);
    mkobserved2(buckets, n, delta, SIZ);

    chis=0.0;
    for (i=0; i<=2*n+1; i++) for (j=0; j<=2*n+1; j++)
    {
	double e=ebuckets[i][j];
	double x=buckets[i][j]-e;
	chis+=x*x/e;
    }

    printf("chis=%f\n", chis);

    return chis;
}

/* end double chisquared */

/* more misc chisquared stuff */
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

/* nearest distance analysis. */

/* convert a normal variate to an integer 0 to 15 with equal probability */
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

static long ndacount[49][16][16];
static long ndalast[16];

static void
ndainit(NO_ARGS)
{
    int i;
    memset(ndacount, 0, sizeof(ndacount));
    for (i=0; i<16; i++) ndalast[i]= -1;
}

static void
donibble(int n, long pos)
{
    int i;

    for (i=0; i<16; i++) if (ndalast[i]>=0)
    {
	long d=pos-ndalast[i];
	if (d>48) d=0;
	ndacount[d][i][n]++;
    }

    ndalast[n]=pos;
}

static double expectnda[49];

static void
mkexpectnda(long size)
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
ndaan(NO_ARGS)
{
    double t=0.0, pv, x;
    int i, j, k;

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
}

static void
nda(NO_ARGS)
{
    long pos=0;

    ndainit();

    printf("Nearest distance analysis ===============\n");
    for (pos=0; pos<SIZ; pos++) donibble(tohextile(buf[pos]), pos);

    mkexpectnda(SIZ);
    ndaan();
    printf("End nearest distance analysis ===============\n\n");
}

/* end nearest distance analysis */

/* Overall summary */
static void
printsum(NO_ARGS)
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
	    {3, 8},
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


int
main(int argc, char **argv)
{
    double c;

    if (argc!=1)
	crash("give no arguments. Numbers are read from stdin.");

    printf("*** pdb version 4 ***\n\n");

    fill_normal(SIZ, buf);

    selfcor(buf, CSIZ);

    lowbits(buf, SIZ, 2);
    lowbits(buf, SIZ, 12);
    lowbits(buf, SIZ, 22);
    lowbits(buf, SIZ, 32);

    printmoments(8);

    extrema(SIZ/8);

    gaps(-4.0, 4.0, SIZ);

    c=dochis(16, 4);
    chispval(c, 33);
    c=dochis(96, 32);
    chispval(c, 193);
    c=dochis(640, 256);
    chispval(c, 1281);
    c=dochis(5000, 4096);
    chispval(c, 10001);

    c=dochis2(24, 16);
    chispval(c, 2499);

    nda();

    printsum();

    return 0;
}
