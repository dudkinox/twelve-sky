//-------------------------------------------------------------------------------------------------
//MAIN_APPLICATION_H
//-------------------------------------------------------------------------------------------------
#ifndef MAIN_APPLICATION_H
#define MAIN_APPLICATION_H
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//WIN32
//-------------------------------------------------------------------------------------------------
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//HEADERS
//-------------------------------------------------------------------------------------------------
#ifdef __WIN32__
#include <windows.h>
#include <windowsx.h>
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "GSDBManager.h"
#include "resource.h"
#include "H02_MyServer.h"
#include "./A00_BugTrap/BugTrap.h"
#endif

#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "local.h"
#include "TS_MAIN_DataTypeAndProtocol.h"

#ifdef __N_PROTECT__
#include "ggsrv25.h"
#endif

#include "H02_MyServer.h"

//-------------------------------------------------------------------------------------------------

#ifndef __WIN32__
#include <sys/ipc.h>
#include <sys/shm.h>
#endif
//-------------------------------------------------------------------------------------------------
//DEFINE_GLOBAL_MACRO
//-------------------------------------------------------------------------------------------------
#define	WM_USER														10
#define MAIN_WINDOW_SIZE_X											300
#define MAIN_WINDOW_SIZE_Y											100
#define WM_NETWORK_MESSAGE_1										WM_USER + 1

#define FD_READ														101
#define FD_WRITE													102
#define FD_ACCEPT													103
#define FD_CLOSE													104
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//SERVER_INFO
//-------------------------------------------------------------------------------------------------
typedef struct
{
	char mServerName[100];
	int mServerPort;
	int mWorldNumber;
	char mDB_1_IP[16];
	int mDB_1_PORT;
	char mDB_1_Name[100];
	char mDB_1_ID[100];
	char mDB_1_PASSWORD[100];
	int mCheckTestServer;
}
SERVER_INFO;
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CALLBACK_FUNCTION
//-------------------------------------------------------------------------------------------------
//extern LRESULT CALLBACK WinMainProcedure( HWND hWnd, UINT nMss, WPARAM wPrm, LPARAM lPrm );
extern int ProcessResponse( UINT nMss, SOCKET *socket, UINT fd_status, void *arg = NULL );
//-------------------------------------------------------------------------------------------------

/*
//-------------------------------------------------------------------------------------------------
//APPLICATION_FUNCTION
//-------------------------------------------------------------------------------------------------
extern BOOL ReadServerInfo( void );
extern BOOL GetOneValueFromFile( HANDLE hFile, char *tBuffer, int tBufferSize );
extern BOOL ApplicationInit( void );
extern void ApplicationFree( void );
extern void BugTrapInit( void );
//-------------------------------------------------------------------------------------------------
*/

//-------------------------------------------------------------------------------------------------
//GLOBAL_VARIABLE
//-------------------------------------------------------------------------------------------------
//extern HWND hMainWindow;
extern SERVER_INFO mSERVER_INFO;
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------------
