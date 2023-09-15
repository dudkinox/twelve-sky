#ifndef MAIN_APP_H
#define MAIN_APP_H


#include <iostream>
#include <string>
#include <stdio.h>
#include <Winsock2.h>
#include <fstream>
#include <tchar.h>
#include <time.h>
#include <map>


#define PTW32_STATIC_LIB
#define HAVE_STRUCT_TIMESPEC


#include "../include/TimeLog.h"
#include "../include/struct.h"
#include "MyServer.h"
#include "MyUser.h"
#include "MyWork.h"
#include "MyTransfer.h"
#include "MyGame.h"
#include "MyUpperCOM.h"
#include "MySummonSystem.h"
#include "MyGameSystem.h"
#include "MyWorld.h"


#define MAX_OTHER_KILL_POINT										2000000000
#define MAX_WRONG_ANSWER_COUNT										3
#define LIMIT_SEND_TIME_MINUTE										1

////server
#define WM_USER_NEW                                                 10
#define WM_NETWORK_MESSAGE_1										WM_USER_NEW + 1
#define WM_NETWORK_MESSAGE_2										WM_USER_NEW + 2
#define WM_NETWORK_MESSAGE_3										WM_USER_NEW + 3
#define WM_TIMER_NEW                                                WM_USER_NEW + 10
#define ID_TIMER                                                    0
/////

//-------------------------------------------------------------------------------------------------
//ZONE_NUMBER
//-------------------------------------------------------------------------------------------------
#define ZONE_PRISON						88
#define ZONE_BASE_JUNG					1
#define ZONE_BASE_SA					6
#define ZONE_BASE_MA					11
#define ZONE_BASE_NANG					140
//-------------------------------------------------------------------------------------------------

extern DWORD mBaseTickCountForLogic;
extern DWORD mPostTickCountForLogic;
extern BOOL mCheckLogicFlag;

using SERVER_INFO = struct
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
	int mCheckPremiumServer;

	float WORLD_EXP;
	float WORLD_ITEM_DROP;
	int WORLD_KTAV;
	float WORLD_PATEXP;
};

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

extern HWND hMainWindow;
extern SERVER_INFO mSERVER_INFO;

#endif // !MAIN_APP_H
