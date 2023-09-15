#ifndef S_NETGLOBAL_H_
#define S_NETGLOBAL_H_

#include <iostream>
#include <string>
#include <map>

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


////server
#define WM_USER_NEW                                                 10
#define WM_NETWORK_MESSAGE_1										WM_USER_NEW + 1
/////


using SERVER_INFO = struct
{
	char mServerName[100];
	int mServerPort;
	int mWorldNumber;
};

extern SERVER_INFO mSERVER_INFO;
extern HWND hMainWindow;
#endif
