#include <stdio.h>
#include <sys/time.h>
#include <time.h>

int main(void)
{
	unsigned int  a;
	struct timeval ts;
	gettimeofday (&ts, NULL);

	fprintf(stderr, "%u\n", (ts.tv_sec * 1000 + ts.tv_usec / 10000));
	fprintf(stderr, "%u\n", a | 0xffffffff);

	return 1;

}
