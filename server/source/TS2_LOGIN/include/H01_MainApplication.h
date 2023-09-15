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
#include "global.h"
#include "local.h"
#include "TS_MAIN_DataTypeAndProtocol.h"
#include "H02_MyServer.h"
#include "H03_MyUser.h"
#include "H04_MyWork.h"
#include "H05_MyTransfer.h"
#include "H06_MyUpperCom.h"
#include "H07_MyGame.h"
#include "H08_MyDB.h"
#include "cts_util.h"
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//DEFINE_GLOBAL_MACRO
//-------------------------------------------------------------------------------------------------
#define WM_USER                                                     10
#define MAIN_WINDOW_SIZE_X											300
#define MAIN_WINDOW_SIZE_Y											100
#define WM_NETWORK_MESSAGE_1										WM_USER + 1
#define FD_READ                                                     101
#define FD_WRITE                                                    102
#define FD_ACCEPT                                                   103
#define FD_CLOSE                                                    104
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//SERVER_INFO
//-------------------------------------------------------------------------------------------------
typedef struct
{
	char mServerName[100];
	int mServerPort;
	int mIslandNum;
	int mPartitionNum;
	int mWorldNumber;
	int mServerVersion;
	char mUpperServer_1_IP[16];
	int mUpperServer_1_Port;
	char mUpperServer_2_IP[16];
	int mUpperServer_2_Port;
	char mUpperServer_3_IP[16];
	int mUpperServer_3_Port;
	char mUpperServer_4_IP[16];
	int mUpperServer_4_Port;
	int mIyoyoAPNum;
#ifdef __CHANNEL__
	char mUpperServer_5_IP[16];
	int mUpperServer_5_Port;
	int mCompanyNum;
	int mGameNum;
#endif
	char mDB_1_IP[2][16];
	int mDB_1_PORT;
	char mDB_1_ID[100];
	char mDB_1_PASSWORD[100];
	char mDB_1_Table01[100];
	char mDB_1_Table02[100];
	char mDB_1_Table03[100];
	char mDB_1_Table04[100];
	char mDB_1_Table05[100];
	char mDB_1_Table06[100];
	char mDB_1_Table07[100];
	char mDB_1_Table08[100];
	char mDB_1_Table09[100];
	char mDB_1_Table10[100];
	char mDB_2_IP[2][16];
	int mDB_2_PORT;
	char mDB_2_ID[100];
	char mDB_2_PASSWORD[100];
	char mDB_2_Table01[100];
	char mDB_2_Table02[100];
	char mDB_2_Table03[100];
	char mDB_2_Table04[100];
	char mDB_2_Table05[100];
	char mDB_2_Table06[100];
	char mDB_2_Table07[100];
	char mDB_2_Table08[100];
	char mDB_2_Table09[100];
	char mDB_2_Table10[100];
	char mDB_3_IP[2][16];
	int mDB_3_PORT;
	char mDB_3_ID[100];
	char mDB_3_PASSWORD[100];
	char mDB_3_Table01[100];
	char mDB_3_Table02[100];
	char mDB_3_Table03[100];
	char mDB_3_Table04[100];
	char mDB_3_Table05[100];
	char mDB_3_Table06[100];
	char mDB_3_Table07[100];
	char mDB_3_Table08[100];
	char mDB_3_Table09[100];
	char mDB_3_Table10[100];

	int mCheckPremiumServer; // 프리미엄 서버(월드) 여부(0이면 일반 서버, 1이면 프리미엄 서버). // @_Premium_Server_@
}
SERVER_INFO;
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CALLBACK_FUNCTION
//-------------------------------------------------------------------------------------------------
extern int ProcessResponse (UINT nMss, SOCKET* from, UINT status, void* pArg = NULL);
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//GLOBAL_VARIABLE
//-------------------------------------------------------------------------------------------------
extern SERVER_INFO mSERVER_INFO;
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------------
