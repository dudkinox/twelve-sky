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
#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <map>

#include "local.h"
#include "TS_MAIN_DataTypeAndProtocol.h"
#ifdef __N_PROTECT__
#include "ggsrv25.h"
#endif
#include "H02_MyServer.h"
#include "H03_MyUser.h"
#include "H04_MyWork.h"
#include "H05_MyTransfer.h"
#include "H06_MyUpperCom.h"
#include "H07_MyGame.h"
#include "H08_MyGameSystem.h"
#include "H09_MyWorld.h"
#include "H10_MySummonSystem.h"
#include "BroadCastMgr.h"
#include "cts_util.h"
//-------------------------------------------------------------------------------------------------

#ifndef __WIN32__
#include <sys/ipc.h>
#include <sys/shm.h>
#define KEY_SHM_ITEM                                                1111 
#define KEY_SHM_MST                                                 2222 
#define KEY_SHM_NPC                                                 3333 
#define KEY_SHM_QUEST                                               4444 
#endif
//-------------------------------------------------------------------------------------------------
//DEFINE_GLOBAL_MACRO
//-------------------------------------------------------------------------------------------------
#define WM_USER                                                     10
#define MAIN_WINDOW_SIZE_X											300
#define MAIN_WINDOW_SIZE_Y											100
#define WM_NETWORK_MESSAGE_1										WM_USER + 1
#define WM_NETWORK_MESSAGE_2										WM_USER + 2
#define WM_NETWORK_MESSAGE_3										WM_USER + 3
#define WM_TIMER                                                    WM_USER + 10

#define FD_READ                                                     101
#define FD_WRITE                                                    102
#define FD_ACCEPT                                                   103
#define FD_CLOSE                                                    104

#define MAX_OTHER_KILL_POINT										2000000000
#define MAX_WRONG_ANSWER_COUNT										3
#define LIMIT_SEND_TIME_MINUTE										1

//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//SERVER_INFO
//-------------------------------------------------------------------------------------------------
typedef struct
{
	char mServerName[100];
	int mServerPort;
	int mServerMaxUserNum;
	int mServerNumber;
	int mWorldNumber;
	char mUpperServer_1_IP[16];
	int mUpperServer_1_Port;
	char mUpperServer_2_IP[16];
	int mUpperServer_2_Port;
	char mUpperServer_3_IP[16];
	int mUpperServer_3_Port;
	char mUpperServer_4_IP[16];
	int mUpperServer_4_Port;
	char mUpperServer_5_IP[16];
	int mUpperServer_5_Port;
	int m_iDeploymentStage;
	int mCheckPremiumServer; // 프리미엄 서버(월드) 여부(0이면 일반 서버, 1이면 프리미엄 서버). // @_Premium_Server_@
	float x_mGeneralExperienceUpRatio;
	float x_mItemDropUpRatio;
	float x_mItemDropUpRatioForMyoung;
	int x_mKillOtherTribeAddValue;
	int x_mTeacherPointUpRatio;
	float x_mPatExperienceUpRatio;
	float x_mBonusGainGeneralExperienceUpRatio;
	int x_mGeneralExperienceDownRatio;
	int x_mKillOtherTribeExperienceUpRatio;
}
SERVER_INFO;
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CALLBACK_FUNCTION
//-------------------------------------------------------------------------------------------------
//extern LRESULT CALLBACK WinMainProcedure( HWND hWnd, UINT nMss, WPARAM wPrm, LPARAM lPrm );
extern int ProcessResponse(UINT nMss, SOCKET *socket, UINT fd_status, void *arg = NULL);
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//GLOBAL_VARIABLE
//-------------------------------------------------------------------------------------------------
extern SERVER_INFO mSERVER_INFO;
extern DWORD mBaseTickCountForLogic;
extern DWORD mPostTickCountForLogic;
extern BOOL mCheckLogicFlag;
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//ZONE_NUMBER
//-------------------------------------------------------------------------------------------------
#define ZONE_PRISON						88
#define ZONE_BASE_JUNG					1
#define ZONE_BASE_SA					6
#define ZONE_BASE_MA					11
#define ZONE_BASE_NANG					140
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------------
