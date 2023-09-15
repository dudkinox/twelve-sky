//-------------------------------------------------------------------------------------------------
//MAIN_APPLICATION_H
//-------------------------------------------------------------------------------------------------
#ifndef MAIN_APPLICATION_H
#define MAIN_APPLICATION_H
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//WIN32
//-------------------------------------------------------------------------------------------------
//#ifndef WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN
//#endif
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//HEADERS
//-------------------------------------------------------------------------------------------------
// 2008.11.17 Kim SungSoo
// Intro :	1. Modify include header file win32 to linux
//			2. Delete for win32 header file
//				#include <windows.h> 
//				#include <windowsx.h>
//				#include <winsock2.h>  
//				#include <ws2tcpip.h>
//				#include <stdio.h> 
//				#include <time.h>
//				#include "./A00_BugTrap/BugTrap.h"
//-------------------------------------------------------------------------------------------------

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
#include "H02_MyServer.h"
#include "H03_MyUser.h"
#include "H04_MyWork.h"
#include "H05_MyTransfer.h"
#include "H06_MyUpperCom.h"
#include "H07_MyGame.h"
#include "H08_MyDB.h"
#include "BroadCastMgr.h"

//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//DEFINE_GLOBAL_MACRO
//-------------------------------------------------------------------------------------------------
// 2008.11.17 Kim SungSoo
// Intro :	1. Delete for window info (MAIN_WINDOW_SIZE_X  = 300, MAIN_WINDOW_SIZE_Y = 100)
//			2. Define WM_USER (this is window message)
//			3. Define network event (FD_xx...)
//-------------------------------------------------------------------------------------------------

#define WM_USER										10
#define WM_NETWORK_MESSAGE_1						WM_USER + 1

#define FD_READ										101
#define FD_WRITE									102
#define FD_ACCEPT									103
#define FD_CLOSE									104
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//SERVER_INFO
//-------------------------------------------------------------------------------------------------
// 2008.11.17 Kim SungSoo
// Intro :	1. 3DB info to 1DB Info (DB sum 1DB)
//			2. Server is not loaded DB table then delete table info
//-------------------------------------------------------------------------------------------------
typedef struct
{
	char mServerName[100];
	int mServerPort;
	int mWorldNumber;

	// Cash Server Info
	char mUpperServer_1_IP[16];
	int mUpperServer_1_Port;
	// Log Server Info
	char mUpperServer_2_IP[16];
	int mUpperServer_2_Port;

	char mDB_1_IP[2][16];
	int mDB_1_PORT;
	char mDB_1_ID[100];
	char mDB_1_PASSWORD[100];

	char mDB_1_Table01[100];
	char mDB_2_Table01[100];
	char mDB_2_Table02[100];
	char mDB_3_Table01[100];

	//cash db
	char mDB2_1_IP[2][16];
	int mDB2_1_PORT;
	char mDB2_1_ID[100];
	char mDB2_1_PASSWORD[100];

	char mDB2_1_Table01[100];
	//-----
	
	int mCheckTestServer;
}
SERVER_INFO;
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CALLBACK_FUNCTION
//-------------------------------------------------------------------------------------------------
// 2008.11.17 Kim SungSoo
// Intro :	1. Modify WinMessageLoop Function to general process function
//-------------------------------------------------------------------------------------------------
//extern LRESULT CALLBACK WinMainProcedure( HWND hWnd, UINT nMss, WPARAM wPrm, LPARAM lPrm );
extern int ProcessResponse(UINT nMss, SOCKET *socket, UINT fd_status, void *arg = NULL);
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//APPLICATION_FUNCTION
//-------------------------------------------------------------------------------------------------
// 2008.11.17 Kim SungSoo
// Intro :	1. Delete win logic function
//-------------------------------------------------------------------------------------------------
//extern BOOL ReadServerInfo( void );
//extern BOOL GetOneValueFromFile( HANDLE hFile, char *tBuffer, int tBufferSize );
//extern BOOL ApplicationInit( void );
//extern void ApplicationFree( void );
//extern void BugTrapInit( void );
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//GLOBAL_VARIABLE
//-------------------------------------------------------------------------------------------------
// 2008.11.17 Kim SungSoo
// Intro :	1. Delete windows handle
//-------------------------------------------------------------------------------------------------
//extern HWND hMainWindow;
//-------------------------------------------------------------------------------------------------

extern SERVER_INFO mSERVER_INFO;
//-------------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------------
