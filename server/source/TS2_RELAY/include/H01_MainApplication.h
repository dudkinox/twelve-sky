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
#if 0

	#include <windows.h>
	#include <windowsx.h>
	#include <winsock2.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <time.h>
	#include "resource.h"
	#include "..\..\P01_Common\TS_MAIN_DataTypeAndProtocol.h"
	#include "H02_MyServer.h"
	#include "H03_MyUser.h"
	#include "H04_MyWork.h"
	#include "H05_MyTransfer.h"
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
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <map>
#include "global.h"
#include "local.h"
#include "TS_MAIN_DataTypeAndProtocol.h"
#include "H02_MyServer.h"
#include "H03_MyUser.h"
#include "H04_MyWork.h"
#include "H05_MyTransfer.h"

//-------------------------------------------------------------------------------------------------

#ifndef __WIN32__

	#include <sys/ipc.h>
	#include <sys/shm.h>
	#define KEY_SHM_ITEM											1111
	#define KEY_SHM_MST												2222
	#define KEY_SHM_NPC												3333
	#define KEY_SHM_QUEST											4444

#endif

//-------------------------------------------------------------------------------------------------
//DEFINE_GLOBAL_MACRO
//-------------------------------------------------------------------------------------------------
#define WM_USER													10
#define MAIN_WINDOW_SIZE_X										300
#define MAIN_WINDOW_SIZE_Y										100
#define WM_NETWORK_MESSAGE_1									WM_USER + 1


#define FD_READ													101
#define FD_WRITE												102
#define FD_ACCEPT												103
#define FD_CLOSE												104


//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//SERVER_INFO
//-------------------------------------------------------------------------------------------------
typedef struct
{
	char mServerName[100];
	int mServerPort;
	int mWorldNumber;
}
SERVER_INFO;
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CALLBACK_FUNCTION
//-------------------------------------------------------------------------------------------------
#if 0
extern LRESULT CALLBACK WinMainProcedure( HWND hWnd, UINT nMss, WPARAM wPrm, LPARAM lPrm );
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//APPLICATION_FUNCTION
//-------------------------------------------------------------------------------------------------
extern BOOL ReadServerInfo( void );
extern BOOL GetOneValueFromFile( HANDLE hFile, char *tBuffer, int tBufferSize );
extern BOOL ApplicationInit( void );
extern void ApplicationFree( void );
extern void BugTrapInit( void );
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//GLOBAL_VARIABLE
//-------------------------------------------------------------------------------------------------
extern HWND hMainWindow;
extern SERVER_INFO mSERVER_INFO;

#endif



extern int ProcessResponse(UINT nMss, SOCKET *socket, UINT fd_status, void *arg = NULL);
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//CLOBAL_VARIABLE
//-------------------------------------------------------------------------------------------------
extern SERVER_INFO mSERVER_INFO;
//extern DWORD mBaseTickCountForLogic;
//extern DWORD mPostTickCountForLogic;
extern BOOL mCheckLogicFlag;
//-------------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------------
