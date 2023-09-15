#include "MainApp.h"
#pragma comment(lib,"WS2_32")
BOOL ReadServerInfo(void);
BOOL GetOneValueFromFile(HANDLE hFile, char* tBuffer, int tBufferSize);
VOID CALLBACK TimerProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTimer);
SERVER_INFO mSERVER_INFO;
DWORD mBaseTickCountForLogic;
DWORD mPostTickCountForLogic;
BOOL mCheckLogicFlag;
HWND hMainWindow;

static DWORD WINAPI DetectCar(LPVOID lpParameter);

int _tmain(int argc, _TCHAR* argv[])
{
	int ret;
	mSERVER_INFO.mServerPort = atoi(argv[1]);
	mSERVER_INFO.mServerNumber = atoi(argv[2]);
	if (!ReadServerInfo())
	{
		__LogMgr()->Log("ERROR READSERVERINFO");
	}

	char szClassName[] = "MainWClass";
	WNDCLASSEX wndclass;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WindowProc;
	wndclass.cbWndExtra = 0;
	wndclass.cbClsExtra = 0;
	wndclass.hInstance = nullptr;
	wndclass.hIcon = ::LoadIcon(nullptr, IDI_APPLICATION);
	wndclass.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
	wndclass.hbrBackground = static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
	wndclass.lpszMenuName = nullptr;
	wndclass.lpszClassName = szClassName;
	wndclass.hIconSm = nullptr;
	::RegisterClassEx(&wndclass);

	HWND hWnd = ::CreateWindowEx(
		0,
		szClassName,
		"",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		nullptr,
		nullptr,
		nullptr,
		nullptr);

	if (hWnd == nullptr)
	{
		__LogMgr()->Log(nullptr, "Error Creating Window", "error", MB_OK);
	}

	hMainWindow = hWnd;
	system("color A");
	//__LogMgr()->Log("ServerName: %s", mSERVER_INFO.mServerName);
	//__LogMgr()->Log("Port: %d", mSERVER_INFO.mServerPort);
	//__LogMgr()->Log("MAX USER: %d", mSERVER_INFO.mServerMaxUserNum);
	//__LogMgr()->Log("ZoneNumber: %d", mSERVER_INFO.mServerNumber);
	//__LogMgr()->Log("WorldNumber: %d", mSERVER_INFO.mWorldNumber);
	//__LogMgr()->Log("!WORLD_KTAV: %d", mSERVER_INFO.WORLD_KTAV);

	mSERVER_INFO.m_iDeploymentStage = 4;

	DWORD ID = NULL;
	int tResult = NULL;
	if (!mSERVER.Init(&tResult))
	{
		__LogMgr()->Log("[Error::mSERVER.Init()]-(%d)", tResult);
		return 0;
	}

	if (tResult == NULL)
	{
		__LogMgr()->Log("%s STARTED SUCCESSFULLY - RUNNING AT %d", mSERVER_INFO.mServerName, mSERVER_INFO.mServerPort);
		__LogMgr()->Log("-WORLD_EXP: %0.0f", mSERVER_INFO.WORLD_EXP);
		__LogMgr()->Log("-WORLD_ITEM_DROP: %0.0f", mSERVER_INFO.WORLD_ITEM_DROP);
		__LogMgr()->Log("-WORLD_PATEXP: %0.0f", mSERVER_INFO.WORLD_PATEXP);
		HANDLE hThread1 = CreateThread(nullptr, 0, DetectCar, nullptr, 0, nullptr);
		CloseHandle(hThread1);
	}

	mBaseTickCountForLogic = GetTickCount();
	mPostTickCountForLogic = mBaseTickCountForLogic;
	mCheckLogicFlag = FALSE;
	MSG msg;
	while (::GetMessage(&msg, nullptr, 0, 0))
	{
		::DispatchMessage(&msg);
	}
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int r = NULL;
	switch (uMsg)
	{
	case WM_NETWORK_MESSAGE_1:
		mSERVER.PROCESS_FOR_NETWORK(hWnd, wParam, lParam);
		return 0;
	case WM_NETWORK_MESSAGE_2:
		r = mCENTER_COM.PROCESS_FOR_NETWORK(hWnd, wParam, lParam);
		return r;
	case WM_NETWORK_MESSAGE_3:
		r = mRELAY_COM.PROCESS_FOR_NETWORK(hWnd, wParam, lParam);
		return r;
	}
}

BOOL ReadServerInfo(void)
{
	HANDLE hFile;
	char tempString01[100];

	hFile = CreateFile("CONFIG\\ZONE_SERVER_INFO.TXT", GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING,
	                   FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		__LogMgr()->Log("[#001]");
		CloseHandle(hFile);
		return FALSE;
	}
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		__LogMgr()->Log("[#002]");
		CloseHandle(hFile);
		return FALSE;
	}
	strcpy(mSERVER_INFO.mServerName, tempString01);
	if (strcmp(mSERVER_INFO.mServerName, "") == 0)
	{
		__LogMgr()->Log("[#003]");
		CloseHandle(hFile);
		return FALSE;
	}
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		__LogMgr()->Log("[#006]");
		CloseHandle(hFile);
		return FALSE;
	}
	mSERVER_INFO.mServerMaxUserNum = atoi(tempString01);
	if ((mSERVER_INFO.mServerMaxUserNum < 1) || (mSERVER_INFO.mServerMaxUserNum > MAX_AVATAR_OBJECT_NUM))
	{
		__LogMgr()->Log("[#007]");
		CloseHandle(hFile);
		return FALSE;
	}
	//hFile = CreateFile( "CONFIG\\ZONE_SERVER_INFO.TXT", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		__LogMgr()->Log("[#012]");
		CloseHandle(hFile);
		return FALSE;
	}
	mSERVER_INFO.mWorldNumber = atoi(tempString01);
	if ((mSERVER_INFO.mWorldNumber < 1) || (mSERVER_INFO.mWorldNumber > 999))
	{
		__LogMgr()->Log("[#013]");
		CloseHandle(hFile);
		return FALSE;
	}
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		__LogMgr()->Log("[#014]");
		CloseHandle(hFile);
		return FALSE;
	}
	strcpy(mSERVER_INFO.mUpperServer_1_IP, tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		__LogMgr()->Log("[#015]");
		CloseHandle(hFile);
		return FALSE;
	}
	mSERVER_INFO.mUpperServer_1_Port = atoi(tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		__LogMgr()->Log("[#016]");
		CloseHandle(hFile);
		return FALSE;
	}
	strcpy(mSERVER_INFO.mUpperServer_2_IP, tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		__LogMgr()->Log("[#017]");
		CloseHandle(hFile);
		return FALSE;
	}
	mSERVER_INFO.mUpperServer_2_Port = atoi(tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		__LogMgr()->Log("[#018]");
		CloseHandle(hFile);
		return FALSE;
	}
	strcpy(mSERVER_INFO.mUpperServer_3_IP, tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		__LogMgr()->Log("[#019]");
		CloseHandle(hFile);
		return FALSE;
	}
	mSERVER_INFO.mUpperServer_3_Port = atoi(tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		__LogMgr()->Log("[#020]");
		CloseHandle(hFile);
		return FALSE;
	}
	strcpy(mSERVER_INFO.mUpperServer_4_IP, tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		__LogMgr()->Log("[#021]");
		CloseHandle(hFile);
		return FALSE;
	}
	mSERVER_INFO.mUpperServer_4_Port = atoi(tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		__LogMgr()->Log("[#022]");
		CloseHandle(hFile);
		return FALSE;
	}
	strcpy(mSERVER_INFO.mUpperServer_5_IP, tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		__LogMgr()->Log("[#023]");
		CloseHandle(hFile);
		return FALSE;
	}
	mSERVER_INFO.mUpperServer_5_Port = atoi(tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		__LogMgr()->Log("[#023]");
		CloseHandle(hFile);
		return FALSE;
	}
	mSERVER_INFO.mCheckPremiumServer = atoi(tempString01);

	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		__LogMgr()->Log("[#023]");
		CloseHandle(hFile);
		return FALSE;
	}
	mSERVER_INFO.WORLD_EXP = atof(tempString01);

	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		__LogMgr()->Log("[#023]");
		CloseHandle(hFile);
		return FALSE;
	}
	mSERVER_INFO.WORLD_ITEM_DROP = atof(tempString01);

	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		__LogMgr()->Log("[#023]");
		CloseHandle(hFile);
		return FALSE;
	}
	mSERVER_INFO.WORLD_KTAV = atoi(tempString01);

	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		__LogMgr()->Log("[#023]");
		CloseHandle(hFile);
		return FALSE;
	}
	mSERVER_INFO.WORLD_PATEXP = atof(tempString01);

	if (!CloseHandle(hFile))
	{
		__LogMgr()->Log("[#024]");
		CloseHandle(hFile);
		return FALSE;
	}
	//	CloseHandle( hFile );
	return TRUE;
}

BOOL GetOneValueFromFile(HANDLE hFile, char* tBuffer, int tBufferSize)
{
	int index01;
	DWORD nReadByte;
	BOOL tCheckSaveState;
	char tData;

	index01 = 0;
	tCheckSaveState = FALSE;
	while (TRUE)
	{
		if (!ReadFile(hFile, &tData, 1, &nReadByte, nullptr))
		{
			return FALSE;
		}
		if (nReadByte != 1)
		{
			return FALSE;
		}
		if (!tCheckSaveState)
		{
			if (tData == '[')
			{
				tCheckSaveState = TRUE;
			}
		}
		else
		{
			if (tData == ']')
			{
				tBuffer[index01] = '\0';
				break;
			}
			if (index01 < (tBufferSize - 1))
			{
				tBuffer[index01] = tData;
				index01++;
			}
			else
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

DWORD WINAPI DetectCar(LPVOID lpParameter)
{
	while (TRUE)
	{
		mBaseTickCountForLogic = GetTickCount();
		if ((mBaseTickCountForLogic - mPostTickCountForLogic) > 500)
		{
			mPostTickCountForLogic = mBaseTickCountForLogic;
			//mCheckLogicFlag = TRUE;
			mGAME.Logic(0.5f);
		}
		Sleep(100);
	}
	return 0;
}

VOID CALLBACK TimerProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTimer)
{
	mBaseTickCountForLogic = GetTickCount();
	if ((mBaseTickCountForLogic - mPostTickCountForLogic) > 500)
	{
		mPostTickCountForLogic = mBaseTickCountForLogic;
		mCheckLogicFlag = TRUE;
	}
}
