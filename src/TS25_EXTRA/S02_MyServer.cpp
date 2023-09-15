//-------------------------------------------------------------------------------------------------
//HEADER
//-------------------------------------------------------------------------------------------------
#include "Shengming.h"
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_SERVER
//-------------------------------------------------------------------------------------------------
//INSTANCE
MyServer mSERVER;
//CREATE
MyServer::MyServer(void)
{
}

//INIT
BOOL MyServer::Init(int* tResult)
{
	int index01;

	srand(static_cast<unsigned int>(time(NULL)));
	if (!SetTimer(hMainWindow, 1, 10000, nullptr))
	{
		*tResult = 1;
		return FALSE;
	}

	if (!mDB.Init())
	{
		*tResult = 1;
		return FALSE;
	}

	ZeroMemory(&mWsaData, sizeof( WSADATA ));
	if (WSAStartup(MAKEWORD(2, 2), &mWsaData) != 0)
	{
		*tResult = 3;
		return FALSE;
	}
	mSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mSocket == INVALID_SOCKET)
	{
		*tResult = 4;
		return FALSE;
	}
	mAddress.sin_family = AF_INET;
	mAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	mAddress.sin_port = htons(mSERVER_INFO.mServerPort);
	if (bind(mSocket, (LPSOCKADDR)&mAddress, sizeof(mAddress)) == SOCKET_ERROR)
	{
		*tResult = 5;
		return FALSE;
	}
	if (listen(mSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		*tResult = 6;
		return FALSE;
	}
	if (WSAAsyncSelect(mSocket, hMainWindow, WM_NETWORK_MESSAGE_1, FD_ACCEPT | FD_CLOSE | FD_READ | FD_WRITE) ==
		SOCKET_ERROR)
	{
		*tResult = 7;
		return FALSE;
	}
	mUSER = static_cast<MyUser*>(calloc(1, (MAX_USER_NUM * sizeof(MyUser))));
	if (mUSER == nullptr)
	{
		*tResult = 8;
		return FALSE;
	}
	for (index01 = 0; index01 < MAX_USER_NUM; index01++)
	{
		mUSER[index01].mBUFFER_FOR_SEND = nullptr;
		mUSER[index01].mBUFFER_FOR_RECV = nullptr;
	}
	for (index01 = 0; index01 < MAX_USER_NUM; index01++)
	{
		if (!mUSER[index01].Init(MAX_SEND_BUFFER_SIZE, MAX_RECV_BUFFER_SIZE))
		{
			*tResult = 9;
			return FALSE;
		}
	}
	//__LogMgr()->Log("![%s] : User mempry set OK\n", __FUNCTION__);

	if (!mWORK.Init())
	{
		*tResult = 10;
		return FALSE;
	}
	//__LogMgr()->Log("![%s] : Work start\n", __FUNCTION__);
	if (!mTRANSFER.Init())
	{
		*tResult = 11;
		return FALSE;
	}
	//__LogMgr()->Log("![%s] : Transfer start\n", __FUNCTION__);
	if (!mGAME.Init())
	{
		*tResult = 12;
		return FALSE;
	}
	//__LogMgr()->Log("![%s] : Game start\n", __FUNCTION__);
	return TRUE;
}

//FREE
void MyServer::Free(void)
{
	int index01;

	mGAME.Free();
	mTRANSFER.Free();
	mWORK.Free();
	if (mUSER)
	{
		for (index01 = 0; index01 < MAX_USER_NUM; index01++)
		{
			mUSER[index01].Quit(0);
		}
		for (index01 = 0; index01 < MAX_USER_NUM; index01++)
		{
			mUSER[index01].Free();
		}
		GlobalFree(mUSER);
		mUSER = nullptr;
	}

	closesocket(mSocket);
	mDB.Free();
}

//PROCESS_FOR_NETWORK
int MyServer::PROCESS_FOR_NETWORK(HWND hWnd, WPARAM wPrm, LPARAM lPrm)
{
	int index01;
	int tAddressLength;
	SOCKET tSocket;
	SOCKADDR_IN tAddress;
	int tempUserIndex;
	int tRecvSizeFromUser;
	BYTE tProtocol;

	tSocket = wPrm;

	switch (WSAGETSELECTEVENT(lPrm))
	{
	case FD_READ:
		for (tempUserIndex = 0; tempUserIndex < MAX_USER_NUM; tempUserIndex++)
		{
			if (!mUSER[tempUserIndex].mCheckConnectState)
			{
				continue;
			}
			if (mUSER[tempUserIndex].mSocket == tSocket)
			{
				break;
			}
		}
		if (tempUserIndex == MAX_USER_NUM)
		{
			return -1;
		}

		while (true)
		{
			tRecvSizeFromUser = recv(
				tSocket, &mUSER[tempUserIndex].mBUFFER_FOR_RECV[mUSER[tempUserIndex].mTotalRecvSize]
				, (MAX_RECV_BUFFER_SIZE - mUSER[tempUserIndex].mTotalRecvSize), 0);

			if (tRecvSizeFromUser <= 0)
			{
				if (tRecvSizeFromUser == SOCKET_ERROR)
				{
					if (WSAGetLastError() == WSAEWOULDBLOCK)
					{
						return 0;
					}
				}
				mUSER[tempUserIndex].Quit(1001);
				return 0;
			}
			if (tRecvSizeFromUser == 0)
			{
				mUSER[tempUserIndex].Quit(1001);
				return 0;
			}
			mUSER[tempUserIndex].mTotalRecvSize += tRecvSizeFromUser;
			break;
		}

		while (TRUE)
		{
			if (mUSER[tempUserIndex].mTotalRecvSize < 1)
			{
				mUSER[tempUserIndex].Quit(10003);
				return 0;
			}
			tProtocol = static_cast<BYTE>(mUSER[tempUserIndex].mBUFFER_FOR_RECV[0]);
			if (W_FUNCTION[tProtocol].PROC == nullptr)
			{
				mUSER[tempUserIndex].Quit(1002);
				return 0;
			}


			if (mUSER[tempUserIndex].mTotalRecvSize < mWORK.mWorkerPacketSize[tProtocol])
			{
				return -1;
			}

			W_FUNCTION[tProtocol].PROC(tempUserIndex);

			if (mUSER[tempUserIndex].mCheckConnectState)
			{
				MoveMemory(&mUSER[tempUserIndex].mBUFFER_FOR_RECV[0],
				           &mUSER[tempUserIndex].mBUFFER_FOR_RECV[mWORK.mWorkerPacketSize[tProtocol]]
				           , ( mUSER[tempUserIndex].mTotalRecvSize - mWORK.mWorkerPacketSize[tProtocol] ));

				mUSER[tempUserIndex].mTotalRecvSize -= mWORK.mWorkerPacketSize[tProtocol];
			}

			if (0 == mUSER[tempUserIndex].mTotalRecvSize)
			{
				return tRecvSizeFromUser;
			}
		}
		return tRecvSizeFromUser;
	case FD_WRITE:
		for (tempUserIndex = 0; tempUserIndex < MAX_USER_NUM; tempUserIndex++)
		{
			if (!mUSER[tempUserIndex].mCheckConnectState)
			{
				continue;
			}
			if (mUSER[tempUserIndex].mSocket == tSocket)
			{
				break;
			}
		}
		if (tempUserIndex == MAX_USER_NUM)
		{
			return -1;
		}
		mUSER[tempUserIndex].Send(FALSE, nullptr, 0);
		return 1;
	case FD_ACCEPT:
		tAddressLength = sizeof(tAddress);
		tSocket = accept(mSocket, (LPSOCKADDR)&tAddress, &tAddressLength);

		if (tSocket == INVALID_SOCKET)
		{
			return 0;
		}
		for (index01 = 0; index01 < MAX_USER_NUM; index01++)
		{
			if (!mUSER[index01].mCheckConnectState)
			{
				break;
			}
		}
		if (index01 == MAX_USER_NUM)
		{
			closesocket(tSocket);
			return 0;
		}

		mUSER[index01].mCheckConnectState = TRUE;
		mUSER[index01].mUsedTime = GetTickCount();
		mUSER[index01].mSocket = tSocket;
	//__LogMgr()->Log("IN ADDRESS :%s:%d",inet_ntoa(tAddress.sin_addr),tAddress.sin_port);
		sprintf(mUSER[index01].mIP, "%s", inet_ntoa(tAddress.sin_addr));
		mUSER[index01].mTotalSendSize = 0;
		mUSER[index01].mTotalRecvSize = 0;
		mUSER[index01].mCheckServerType = 0;
		mTRANSFER.B_CONNECT_OK();
		mUSER[index01].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return 1;
	case FD_CLOSE:
		for (tempUserIndex = 0; tempUserIndex < MAX_USER_NUM; tempUserIndex++)
		{
			if (!mUSER[tempUserIndex].mCheckConnectState)
			{
				continue;
			}
			if (mUSER[tempUserIndex].mSocket == wPrm)
			{
				break;
			}
		}
		if (tempUserIndex == MAX_USER_NUM)
		{
			return 0;
		}

		mUSER[tempUserIndex].Quit(0);
		return 0;
	}
}

void MyServer::PROCESS_FOR_TIMER(void)
{
	int index01;
	DWORD tPresentTime;

	tPresentTime = GetTickCount();
	for (index01 = 0; index01 < MAX_USER_NUM; index01++)
	{
		if (!mUSER[index01].mCheckConnectState)
		{
			continue;
		}
		if ((tPresentTime - mUSER[index01].mUsedTime) > 1800000)
		{
			//mUSER[index01].Quit( 1003 );
		}
	}
	mGAME.mTickCount++;
#if 0
	if( ( mGAME.mTickCount % 6 ) == 0 )
	{
	}
#endif
}
