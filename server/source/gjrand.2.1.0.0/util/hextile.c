/* Miscellaneous stuff for gjrand random numbers version 1.3.0.0 or later. */
/* Copyright (C) 2004-2007 G. Jones. */
/* Licensed under the GNU General Public License version 2 or 3. */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* prints the "hexadeciles" ? of the normal distribution. */

double buf[12*1024*1024];

void
hextile(void)
{
    int i, j;
    static const double del=1.0/(1024.0*1024.0);
    double t=0.0;

    for (i=12*1024*1024-1; i>=0; i--)
    {
	double x=i*del;
	double x1=x+del/2.0;
	double x2=x+del;
	double y = exp(x*x* -0.5) + exp(x2*x2* -0.5) + 4.0*exp(x1*x1* -0.5);
	t+=y;
	buf[i]=t;
    }

    t=0.5/t;
    for (i=0; i<12*1024*1024; i++) buf[i]*=t;

    i=12*1024*1024-1;
    for (j=1; j<7; j++)
    {
	t = j/16.0;
	while (1)
	{
	    if (i<0) return;
	    if (buf[i]>=t)
	    {
		double y1=buf[i], y2=buf[i+1];
		double f=(t-y1)/(y2-y1);
		double x = (i+f) * del;
		printf("%10.6f: %20.16f\n", t, x);
		goto next;
	    }
	    i--;
	}

	next: ;
    }
}

int
main(int argc, char **argv)
{
    hextile();

    return 0;
}
