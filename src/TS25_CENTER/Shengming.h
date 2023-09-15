#ifndef S_NETGLOBAL_H_
#define S_NETGLOBAL_H_

#include <iostream>
#include <string>

#include <Winsock2.h>
#include <fstream>
#include <tchar.h>
#include <time.h>
#include "../include/struct.h"
#include "../include/TimeLog.h"

#include "H02_MyServer.h"
#include "H03_MyUser.h"
#include "H04_MyWork.h"
#include "H05_MyTransfer.h"

#include "H07_MyGame.h"
#include "H08_MyDB.h"

////server
#define WM_USER_NEW                                                 10
#define WM_NETWORK_MESSAGE_1										WM_USER_NEW + 1
/////

using SERVER_INFO = struct
{
	char mServerName[100];
	int mServerPort;
	int mWorldNumber;
	char mUpperServer_1_IP[16];
	int mUpperServer_1_Port;
	char mDB_1_IP[26];
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
	char mDB_2_IP[26];
	int mDB_2_PORT;
	char mDB_2_ID[200];
	char mDB_2_PASSWORD[200];
	char ServerIP[200];
};

using _g_env = struct __g_env
{
	int world_num;
	string db_name_str_obj;
};


extern SERVER_INFO mSERVER_INFO;
extern _g_env g_env;
extern HWND hMainWindow;
#endif
