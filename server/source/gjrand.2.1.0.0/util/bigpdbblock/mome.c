#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define SQR2OPI (sqrtl(M_2_PI))

void
thing(void)
{
    int i;
    long double x, e, y, z, s0=0.0, s1, s2=0.0, s3, s4=0.0;

    for (i=(1<<21)-1; i>=0; i--)
    {
	x=(16.0/(1<<21))*i;
	y=x*x;
	e=exp(y*-0.5);
	s0+=e;
	e*=y; s2+=e;
	e*=y; s4+=e;

	x+=(16.0/(1<<22));
	y=x*x;
	e=exp(y*-0.5);
	e+=e;
	s0+=e;
	e*=y; s2+=e;
	e*=y; s4+=e;
    }

    s0-=0.5;
    s0 *= 16.0/(3<<21)*SQR2OPI;
    s2 *= 16.0/(3<<21)*SQR2OPI;
    s4 *= 16.0/(3<<21)*SQR2OPI;

    printf("%20.15Lf\n%20.15Lf\n%20.15Lf\n\n", s0, s2, s4);

    s1=0.0; s2=0.0; s3=0.0; s4=0.0;

    for (i=(1<<21)-1; i>=0; i--)
    {
	x=(16.0/(1<<21))*i;
	y=x*x;
	e=exp(y*-0.5);
	s1+=e*y;
	s3+=e*y*y*y;
	y-=1.0; s2+=e*y*y;
	y=x*x; y*=y; y-=3.0; s4+=e*y*y;

	x+=(16.0/(1<<22));
	y=x*x;
	e=exp(y*-0.5);
	e+=e;
	s1+=e*y;
	s3+=e*y*y*y;
	y-=1.0; s2+=e*y*y;
	y=x*x; y*=y; y-=3.0; s4+=e*y*y;
    }

    s2-=0.5;
    s4-=4.5;
    s1 *= 16.0/(3<<21)*SQR2OPI;
    s2 *= 16.0/(3<<21)*SQR2OPI;
    s3 *= 16.0/(3<<21)*SQR2OPI;
    s4 *= 16.0/(3<<21)*SQR2OPI;

    printf("%20.15Lf\n%20.15Lf\n%20.15Lf\n%20.15Lf\n\n", s1, s2, s3, s4);

    s1=0.0; s2=0.0; s3=0.0; s4=0.0;

    for (i=(1<<21)-1; i>=0; i--)
    {
	x=(16.0/(1<<21))*i;
	y=x*x;
	e=exp(y*-0.5);
	z=y; z-=1.0; s1+=z*z*e;
	z=y*y*y; z-=15.0; s3+=z*z*e;
	y-=1.0; z=y*y; z-=2.0; s2+=z*z*e;
	y=x*x; y*=y; y-=3.0; z=y*y; z-=96.0; s4+=z*z*e;

	x+=(16.0/(1<<22));
	y=x*x;
	e=exp(y*-0.5);
	e+=e;
	z=y; z-=1.0; s1+=z*z*e;
	z=y*y*y; z-=15.0; s3+=z*z*e;
	y-=1.0; z=y*y; z-=2.0; s2+=z*z*e;
	y=x*x; y*=y; y-=3.0; z=y*y; z-=96.0; s4+=z*z*e;
    }

    s1-=0.5;
    s2-=0.5;
    s3-=112.5;
    s4-=87.0*87.0*0.5;

    s1 *= 16.0/(3<<21)*SQR2OPI;
    s2 *= 16.0/(3<<21)*SQR2OPI;
    s3 *= 16.0/(3<<21)*SQR2OPI;
    s4 *= 16.0/(3<<21)*SQR2OPI;

    printf("%20.15Lf\n%20.15Lf\n%20.15Lf\n%20.15Lf\n\n",
	sqrtl(s1), sqrtl(s2), sqrtl(s3), sqrtl(s4));
}

int
main(int argc, char **argv)
{
    thing();
    return 0;
}
