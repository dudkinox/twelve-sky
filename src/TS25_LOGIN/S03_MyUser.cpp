//-------------------------------------------------------------------------------------------------
//HEADER
//-------------------------------------------------------------------------------------------------
#include "Shengming.h"
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_USER
//-------------------------------------------------------------------------------------------------
//INSTANCE
MyUser* mUSER = nullptr;
//CREATE
MyUser::MyUser(void)
{
	mBUFFER_FOR_SEND = nullptr;
	mBUFFER_FOR_RECV = nullptr;
}

//INIT
BOOL MyUser::Init(int tSendBufferSize, int tRecvBufferSize)
{
	mCheckConnectState = FALSE;
	mBUFFER_FOR_SEND = static_cast<char*>(calloc(1, tSendBufferSize));
	if (mBUFFER_FOR_SEND == nullptr)
	{
		return FALSE;
	}
	mBUFFER_FOR_RECV = static_cast<char*>(calloc(1, tRecvBufferSize));
	if (mBUFFER_FOR_RECV == nullptr)
	{
		return FALSE;
	}
	return TRUE;
}

//FREE
void MyUser::Free(void)
{
	if (mBUFFER_FOR_SEND != nullptr)
	{
		free(mBUFFER_FOR_SEND);
		mBUFFER_FOR_SEND = nullptr;
	}
	if (mBUFFER_FOR_RECV != nullptr)
	{
		free(mBUFFER_FOR_RECV);
		mBUFFER_FOR_RECV = nullptr;
	}
}

//SEND
void MyUser::Send(BOOL tCheckValidBuffer, char* tBuffer, int tBufferSize)
{
	int nResult = 0;
	fd_set write_fds;
	if (!mCheckConnectState)
	{
		return;
	}

	int tSendValue01;

	if (tCheckValidBuffer)
	{
		if ((MAX_SEND_BUFFER_SIZE - mTotalSendSize) < tBufferSize)
		{
			Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		CopyMemory(&mBUFFER_FOR_SEND[mTotalSendSize], &tBuffer[0], tBufferSize);
		mTotalSendSize += tBufferSize;
	}
	while (mTotalSendSize > 0)
	{
		FD_ZERO(&write_fds);
		FD_SET(mSocket, &write_fds);
		struct timeval tv = {5, 0};

		if ((nResult = select(mSocket + 1, nullptr, &write_fds, nullptr, &tv)) < 0)
		{
			continue;
		}

		if (nResult == 0)
		{
			MoveMemory(&mBUFFER_FOR_SEND[0], &mBUFFER_FOR_SEND[tSendValue01], ( mTotalSendSize - tSendValue01 ));
			mTotalSendSize -= tSendValue01;
			return;
		}

		if (FD_ISSET(mSocket, &write_fds))
		{
			tSendValue01 = send(mSocket, &mBUFFER_FOR_SEND[0], mTotalSendSize, 0);
			if (tSendValue01 == SOCKET_ERROR)
			{
				if (WSAGetLastError() == WSAEWOULDBLOCK)
				{
					return;
				}
				Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			MoveMemory(&mBUFFER_FOR_SEND[0], &mBUFFER_FOR_SEND[tSendValue01], ( mTotalSendSize - tSendValue01 ));
			mTotalSendSize -= tSendValue01;
		}
	}
}

//QUIT
void MyUser::Quit(const char* tFileName, const char* tFuncName, int tLine)
{
	if (!mCheckConnectState)
	{
		return;
	}
	mCheckConnectState = FALSE;
	if ((mCheckValidState) && (mMoveZoneResult == 0))
	{
		mPLAYUSER_COM.U_UNREGISTER_USER_FOR_LOGIN_SEND(mPlayUserIndex, &uID[0]);
	}
	mTotalRecvSize = 0;
	mTotalSendSize = 0;
	mCheckValidState = FALSE;
	closesocket(mSocket);

	//__LogMgr()->Log("![%s] : %s : %s : %d\n", tFuncName, uID, tFileName, tLine);
	//__LogMgr()->Log("=> Close Connection From : uID[%s] uIP[%s]\n", uID, mIP);

	//closesocket( mSocket );
}

//-------------------------------------------------------------------------------------------------
