#include "Shengming.h"
#pragma comment(lib,"WS2_32")
HWND hMainWindow;
SERVER_INFO mSERVER_INFO;
_g_env g_env;

BOOL ReadServerInfo(void);
BOOL GetOneValueFromFile(HANDLE hFile, char* tBuffer, int tBufferSize);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int _tmain(int argc, _TCHAR* argv[])
{
	char szClassName[] = "MainWClass";
	WNDCLASSEX wndclass;
	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WindowProc;
	wndclass.cbWndExtra = 0;
	wndclass.cbClsExtra = 0;
	wndclass.hInstance = nullptr;
	wndclass.hIcon = ::LoadIcon(nullptr,IDI_APPLICATION);
	wndclass.hCursor = ::LoadCursor(nullptr,IDC_ARROW);
	wndclass.hbrBackground = static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
	wndclass.lpszMenuName = nullptr;
	wndclass.lpszClassName = szClassName;
	wndclass.hIconSm = nullptr;
	::RegisterClassEx(&wndclass);
	//创建主窗口
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
		__LogMgr()->Log("Error Creating Window");
		return -1;
	}

	hMainWindow = hWnd;
	system("color A");
	MSG msg;
	int ret;

	if (!ReadServerInfo())
	{
		__LogMgr()->Log("ERROR READSERVERINFO");
		//return -1;
	}
	g_env.world_num = 1;
	// 
	//__LogMgr()->Log("!ServerName: %s", mSERVER_INFO.mServerName);
	//__LogMgr()->Log("!ServerPort: %d", mSERVER_INFO.mServerPort); 
	// 	__LogMgr()->Log("!最大用户: %d", mSERVER_INFO.mServerMaxUserNum);
	//	__LogMgr()->Log("!服务器标识: %d", mSERVER_INFO.mServerNumber);
	//	__LogMgr()->Log("!世界标识: %d", mSERVER_INFO.mWorldNumber);
	// 
	// 
	int tResult;
	if (!mSERVER.Init(&tResult))
	{
		__LogMgr()->Log("Error mServerINIT[%d]", tResult);
	}

	__LogMgr()->Log("%s STARTED SUCCESSFULLY - RUNNING AT %d", mSERVER_INFO.mServerName, mSERVER_INFO.mServerPort);
	while (::GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	return msg.wParam; //当GETMESSAGE返回0时结束
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_NETWORK_MESSAGE_1:
		mSERVER.PROCESS_FOR_NETWORK(hWnd, wParam, lParam);
		return 0;
	case WM_TIMER:
		mSERVER.PROCESS_FOR_TIMER();
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


BOOL ReadServerInfo(void)
{
	HANDLE hFile;
	char tempString01[100];

	hFile = CreateFile("CONFIG\\PLAYUSER_SERVER_INFO.TXT", GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING,
	                   FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	strcpy(mSERVER_INFO.mServerName, tempString01);
	if (strcmp(mSERVER_INFO.mServerName, "") == 0)
	{
		CloseHandle(hFile);
		return FALSE;
	}
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	mSERVER_INFO.mServerPort = atoi(tempString01);
	if ((mSERVER_INFO.mServerPort < 10000) || (mSERVER_INFO.mServerPort > 99999))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	mSERVER_INFO.mWorldNumber = atoi(tempString01);
	if ((mSERVER_INFO.mWorldNumber < 1) || (mSERVER_INFO.mWorldNumber > 999))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	strcpy(mSERVER_INFO.mDB_1_IP, tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	mSERVER_INFO.mDB_1_PORT = atoi(tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	strcpy(mSERVER_INFO.mDB_1_ID, tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	strcpy(mSERVER_INFO.mDB_1_PASSWORD, tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	strcpy(mSERVER_INFO.mDB_1_Table01, tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	strcpy(mSERVER_INFO.mDB_1_Table02, tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	strcpy(mSERVER_INFO.mDB_1_Table03, tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	strcpy(mSERVER_INFO.mDB_1_Table04, tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	strcpy(mSERVER_INFO.mDB_1_Table05, tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	strcpy(mSERVER_INFO.mDB_1_Table06, tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	strcpy(mSERVER_INFO.mDB_1_Table07, tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	strcpy(mSERVER_INFO.mDB_1_Table08, tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	strcpy(mSERVER_INFO.mDB_1_Table09, tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	strcpy(mSERVER_INFO.mDB_1_Table10, tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	strcpy(mSERVER_INFO.mDB_2_IP, tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	mSERVER_INFO.mDB_2_PORT = atoi(tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	strcpy(mSERVER_INFO.mDB_2_ID, tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	strcpy(mSERVER_INFO.mDB_2_PASSWORD, tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	strcpy(mSERVER_INFO.mDB_2_Table01, tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	strcpy(mSERVER_INFO.mDB_2_Table02, tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	strcpy(mSERVER_INFO.mDB_2_Table03, tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	strcpy(mSERVER_INFO.mDB_2_Table04, tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	strcpy(mSERVER_INFO.mDB_3_IP, tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	mSERVER_INFO.mDB_3_PORT = atoi(tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	strcpy(mSERVER_INFO.mDB_3_ID, tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	strcpy(mSERVER_INFO.mDB_3_PASSWORD, tempString01);
	if (!GetOneValueFromFile(hFile, &tempString01[0], 100))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	strcpy(mSERVER_INFO.mDB_3_Table01, tempString01);
	CloseHandle(hFile);
	return TRUE;
}

//GET_ONE_VALUE_FROM_FILE
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
