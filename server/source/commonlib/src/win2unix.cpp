#include "win2unix.h"
#include <time.h>

uint32_t GetTickCount(void)
{
#if 0
	char t[15];
	char t2[15];
	tms tm;

	memset(t, 0, 10);
	snprintf(t, 15, "%u0", (uint32_t)times(&tm));
	snprintf(t2, 15, "1%s", &t[2]);

	return atol(t2);
#endif

	timeval ts;
	::gettimeofday (&ts, NULL);
	return static_cast<uint32_t> (ts.tv_sec * 1000 + ts.tv_usec / 1000);
}

extern void ZeroMemory(void *dst, size_t len)
{
	memset(dst, 0, len);
}

void CopyMemory(void *dst, const void *src, size_t len)
{
	memset(dst, 0, len);
	memcpy(dst, src, len);
}

void MoveMemory(void *dst, const void *src, size_t len)
{
	memmove(dst, src, len);
}

BOOL IsDBCSLeadByte (BYTE TestChar)
{
	return static_cast<BOOL> (0x80 & TestChar);
}
