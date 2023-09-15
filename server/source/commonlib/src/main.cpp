#include <stdio.h>
#include "../include/win2unix.h"

int main(void)
{
	uint32_t tick_time;
	time_t t;

	tick_time = GetTickCount();

	fprintf(stderr, "time : %u\n", tick_time);


	return 1;
}
