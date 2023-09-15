#include "Shengming.h"

MyServer mSERVER;

MyServer::MyServer(void)
{
}

BOOL MyServer::Init(int* tResult)
{
	int index01;

	srand(static_cast<unsigned int>(time(NULL)));

	if (!mDB.Init())
	{
		*tResult = 2;
		return FALSE;
	}

	ZeroMemory(&mWsaData, sizeof(WSADATA));
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
	mAddress.sin_port = htons(g_env.listen_port);
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

	mUSER = static_cast<MyUser*>(GlobalAlloc(GMEM_FIXED, (MAX_USER_NUM * sizeof(MyUser))));
	if (mUSER == nullptr)
	{
		*tResult = 9;
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
			*tResult = 10;
			return FALSE;
		}
	}

	if (!mPLAYUSER_COM.Init(mSERVER_INFO.mUpperServer_1_IP, mSERVER_INFO.mUpperServer_1_Port))
	{
		*tResult = 8;
		return FALSE;
	}
	if (!mCENTER_COM.Init(mSERVER_INFO.mUpperServer_2_IP, mSERVER_INFO.mUpperServer_2_Port))
	{
		*tResult = 9;
		return FALSE;
	}

	if (!mWORK.Init())
	{
		*tResult = 11;
		return FALSE;
	}
	if (!mTRANSFER.Init())
	{
		*tResult = 12;
		return FALSE;
	}
	if (!mGAME.Init())
	{
		*tResult = 13;
		return FALSE;
	}

	return TRUE;
}


void MyServer::Free(void)
{
	int index01;
	mDB.Free();
	mGAME.Free();
	mTRANSFER.Free();
	mWORK.Free();
	if (mUSER)
	{
		for (index01 = 0; index01 < MAX_USER_NUM; index01++)
		{
			mUSER[index01].Quit(__FILE__, __FUNCTION__, __LINE__);
		}
		for (index01 = 0; index01 < MAX_USER_NUM; index01++)
		{
			mUSER[index01].Free();
		}
		GlobalFree(mUSER);
		mUSER = nullptr;
	}
	closesocket(mSocket);
}

int MyServer::PROCESS_FOR_NETWORK(HWND hWnd, WPARAM wPrm, LPARAM lPrm)
{
	int index01;
	int tAddressLength;
	SOCKET tSocket = wPrm;
	in_addr tAddress;
	int tempUserIndex;
	int tRecvSizeFromUser;
	BYTE tProtocol;
	char tPacketEncryptionValue;
	int tRandomNumber;
	struct sockaddr_in ClientAddr;

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
			mUSER[tempUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return 0;
		}

	//AGAIN:
		while (true)
		{
			tRecvSizeFromUser = recv(
				tSocket, &mUSER[tempUserIndex].mBUFFER_FOR_RECV[mUSER[tempUserIndex].mTotalRecvSize],
				(MAX_RECV_BUFFER_SIZE - mUSER[tempUserIndex].mTotalRecvSize), 0);
			if (tRecvSizeFromUser <= 0)
			{
				if (tRecvSizeFromUser == SOCKET_ERROR)
				{
					if (WSAGetLastError() == WSAEWOULDBLOCK)
					{
						return 0;
					}
				}
				mUSER[tempUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return 0;
			}
			mUSER[tempUserIndex].mTotalRecvSize += tRecvSizeFromUser;
			break;
		}
		while (true)
		{
			if (mUSER[tempUserIndex].mTotalRecvSize < 9)
			{
				return -1;
			}
			tProtocol = static_cast<BYTE>(mUSER[tempUserIndex].mBUFFER_FOR_RECV[8]);
			if (W_FUNCTION[tProtocol].PROC == nullptr)
			{
				if (mUSER[tempUserIndex].mCheckValidState)
				{
					__LogMgr()->Log("Block User[%s] - Unknown Header[%d], Length[%d].\n", mUSER[tempUserIndex].uID,
					                tProtocol, tRecvSizeFromUser);
					mPLAYUSER_COM.U_BLOCK_USER_FOR_LOGIN_2_SEND(501, &mUSER[tempUserIndex].uID[0]);
				}
				mUSER[tempUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return 0;
			}

			if (mUSER[tempUserIndex].mTotalRecvSize < mWORK.mWorkerPacketSize[tProtocol])
			{
				return -1;
			}
			if (mUSER[tempUserIndex].mTotalRecvSize >= mWORK.mWorkerPacketSize[tProtocol])
			{
				W_FUNCTION[tProtocol].PROC(tempUserIndex);
				if (mUSER[tempUserIndex].mCheckConnectState)
				{
					MoveMemory(&mUSER[tempUserIndex].mBUFFER_FOR_RECV[0],
					           &mUSER[tempUserIndex].mBUFFER_FOR_RECV[mWORK.mWorkerPacketSize[tProtocol]],
					           (mUSER[tempUserIndex].mTotalRecvSize - mWORK.mWorkerPacketSize[tProtocol]));
					mUSER[tempUserIndex].mTotalRecvSize -= mWORK.mWorkerPacketSize[tProtocol];
				}
			}
			if (mUSER[tempUserIndex].mTotalRecvSize <= 0) break; //Fix
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
		return 0;
	case FD_ACCEPT:
		tAddressLength = sizeof(ClientAddr);
		tSocket = accept(tSocket, (struct sockaddr*)&ClientAddr, &tAddressLength);
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
		mUSER[index01].mCheckValidState = FALSE;
		mUSER[index01].mUsedTime = GetTickCount();
		mUSER[index01].mSocket = tSocket;
		strncpy(mUSER[index01].mIP, inet_ntoa(ClientAddr.sin_addr), 15);
	//__LogMgr()->Log("IN ADDRESS [%s]", mUSER[index01].mIP);
		mUSER[index01].mTotalSendSize = 0;
		mUSER[index01].mTotalRecvSize = 0;
		memset(&mUSER[index01].uSaveItem, 0, MAX_SAVE_ITEM_SLOT_NUM * 4 * sizeof(int));
		memset(&mUSER[index01].mGiftInfo, 0, 10);
		memset(&mUSER[index01].mAvatarInfo, 0, (sizeof(AVATAR_INFO) * MAX_USER_AVATAR_NUM));
		mTRANSFER.B_CONNECT_OK(0, mGAME.mMaxPlayerNum, mGAME.mGagePlayerNum,
		                       (mGAME.mPresentPlayerNum + mGAME.mAddPlayerNum));
		mUSER[index01].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return 1;
	case FD_CLOSE:
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
			return 0;
		}
		mUSER[tempUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return 0;
	}
}
