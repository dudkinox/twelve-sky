#ifndef __WIN_TO_UNIX_H__
#define __WIN_TO_UNIX_H__

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define TRUE                true
#define FALSE               false
#define UINT                unsigned int
#define BOOL				bool
#define BYTE                unsigned char
#define WORD                uint16_t
#define DWORD               uint32_t
#define HANDLE              FILE *
#define SOCKET              int
#define LONGLONG            long long
#define LPSOCKADDR          (struct sockaddr *)
#define SOCKADDR_IN         struct sockaddr_in
#define closesocket(SOCK_FD) {\
	close(SOCK_FD);\
}

extern uint32_t GetTickCount(void);
extern void ZeroMemory(void *dst, size_t len);
extern void CopyMemory(void *dst, const void *src, size_t len);
extern void MoveMemory(void *dst, const void *src, size_t len);
extern BOOL IsDBCSLeadByte (BYTE TestChar);

#endif
