//-------------------------------------------------------------------------------------------------
//HEADER
//-------------------------------------------------------------------------------------------------
#include "MainApp.h"
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
BOOL MyUser::Init(int tUserIndex, int tSendBufferSize, int tRecvBufferSize)
{
	m_bUDPActivated = false;
	m_bUDPArrivedAtClient = false;
	m_iTryCountForAuth = 0;
	mUserIndex = tUserIndex;
	mCheckConnectState = FALSE;

	mBUFFER_FOR_SEND = static_cast<char*>(GlobalAlloc(GMEM_FIXED, tSendBufferSize));
	if (mBUFFER_FOR_SEND == nullptr)
	{
		return FALSE;
	}
	mBUFFER_FOR_RECV = static_cast<char*>(GlobalAlloc(GMEM_FIXED, tRecvBufferSize));
	if (pthread_mutex_init(&m_user_lock, nullptr) != 0)
	{
		return false;
	}
	if (pthread_mutex_init(&m_user_quit_lock, nullptr) != 0)
	{
		return false;
	}
	if (mBUFFER_FOR_RECV == nullptr)
	{
		return FALSE;
	}

	return TRUE;
}

//FREE
void MyUser::Free(void)
{
	m_bUDPActivated = false;
	m_bUDPArrivedAtClient = false;
	if (mBUFFER_FOR_SEND != nullptr)
	{
		GlobalFree(mBUFFER_FOR_SEND);
		mBUFFER_FOR_SEND = nullptr;
	}
	if (mBUFFER_FOR_RECV != nullptr)
	{
		GlobalFree(mBUFFER_FOR_RECV);
		mBUFFER_FOR_RECV = nullptr;
	}
}

void MyUser::Send(BOOL tCheckValidBuffer, char* tBuffer, int tBufferSize, bool bUDPSend)
{
	if (!mCheckConnectState)
	{
		return;
	}

	int nResult = 0;
	int tSendValue01 = 0;

	if (pthread_mutex_lock(&m_user_lock) != 0)
	{
		return;
	}

	if (tCheckValidBuffer)
	{
		if ((mSERVER.mMAX_SEND_BUFFER_SIZE - mTotalSendSize) < tBufferSize)
		{
			__LogMgr()->Log("![%s] : Send buffer full!(%d, %d) - %s\n", __FUNCTION__
			                , (mSERVER.mMAX_SEND_BUFFER_SIZE - mTotalSendSize)
			                , tBufferSize, mAvatarInfo.aName);
			Quit(__FILE__, __FUNCTION__, __LINE__);
			if (pthread_mutex_unlock(&m_user_lock) != 0)
			{
				return;
			}
			return;
		}
		if (mTotalSendSize < 0)
			mTotalSendSize = 0;
		memcpy(mBUFFER_FOR_SEND + mTotalSendSize, tBuffer, tBufferSize);
		mTotalSendSize += tBufferSize;
	}

	while (mTotalSendSize > 0)
	{
		tSendValue01 = send(mSocket, &mBUFFER_FOR_SEND[0], mTotalSendSize, 0);
		if (tSendValue01 < 0)
		{
			if (errno == EINTR)
			{
				continue;
			}
			if (errno != EAGAIN && errno != EWOULDBLOCK)
			{
				Quit(__FILE__, __FUNCTION__, __LINE__);
				if (pthread_mutex_unlock(&m_user_lock) != 0)
				{
					return;
				}
				return;
			}
			if (pthread_mutex_unlock(&m_user_lock) != 0)
			{
				return;
			}
			return;
		}
		if (tSendValue01 == 0)
		{
			Quit(__FILE__, __FUNCTION__, __LINE__);
			if (pthread_mutex_unlock(&m_user_lock) != 0)
			{
				return;
			}
			return;
		}

		if ((mTotalSendSize - tSendValue01) > 0)
		{
			memmove(mBUFFER_FOR_SEND, mBUFFER_FOR_SEND + tSendValue01,
			        (mTotalSendSize - tSendValue01));
		}
		mTotalSendSize -= tSendValue01;
		if (mTotalSendSize < 0)
		{
			mTotalSendSize = 0;
		}
	}
	if (pthread_mutex_unlock(&m_user_lock) != 0)
	{
	}
}

void MyUser::Quit(const char* tFileName, const char* tFuncName, int tLine)
{
	if (pthread_mutex_lock(&m_user_quit_lock) != 0)
	{
	}

	if (!mCheckConnectState)
	{
		return;
	}

	closesocket(mSocket);
	mSocket = -1;
	mCheckConnectState = FALSE;

	mTotalSendSize = 0;
	mTotalRecvSize = 0;

	m_iTryCountForAuth = 0;

	m_bUDPActivated = false;
	m_bUDPArrivedAtClient = false;

	int tSort;
	BYTE tData[MAX_BROADCAST_DATA_SIZE];

	if ((mCheckValidState) && (mMoveZoneResult == 0))
	{
		mCheckValidState = FALSE;

		if (strcmp(mAvatarInfo.aPartyName[0], "") != 0)
		{
			if (strcmp(mAvatarInfo.aPartyName[0], mAvatarInfo.aName) != 0)
			{
				tSort = 118;
				CopyMemory(&tData[0], &mAvatarInfo.aPartyName[0], MAX_PARTY_NAME_LENGTH);
				CopyMemory(&tData[MAX_PARTY_NAME_LENGTH], &mAvatarInfo.aName[0], MAX_AVATAR_NAME_LENGTH);
				mRELAY_COM.U_DEMAND_BROADCAST_DATA(tSort, &tData[0]);
			}
			else
			{
				tSort = 117;
				CopyMemory(&tData[0], &mAvatarInfo.aPartyName[0], MAX_PARTY_NAME_LENGTH);
				tData[MAX_PARTY_NAME_LENGTH] = '\0';
				mRELAY_COM.U_DEMAND_BROADCAST_DATA(tSort, &tData[0]);
			}
		}
		mAvatarInfo.aLogoutInfo[0] = mSERVER_INFO.mServerNumber;
		mAvatarInfo.aLogoutInfo[1] = static_cast<int>(mAVATAR_OBJECT[mUserIndex].mDATA.aAction.aLocation[0]);
		mAvatarInfo.aLogoutInfo[2] = static_cast<int>(mAVATAR_OBJECT[mUserIndex].mDATA.aAction.aLocation[1]);
		mAvatarInfo.aLogoutInfo[3] = static_cast<int>(mAVATAR_OBJECT[mUserIndex].mDATA.aAction.aLocation[2]);
		mAvatarInfo.aLogoutInfo[4] = mAVATAR_OBJECT[mUserIndex].mDATA.aLifeValue;
		mAvatarInfo.aLogoutInfo[5] = mAVATAR_OBJECT[mUserIndex].mDATA.aManaValue;
		mPLAYUSER_COM.U_UNREGISTER_USER_FOR_ZONE_SEND(mPlayUserIndex, &uID[0], &mAvatarInfo, &mAuthInfo);
	}
	mCheckValidState = FALSE;
	mAVATAR_OBJECT[mUserIndex].mCheckValidState = false;
	memset(&mAvatarInfo, 0x00, SIZE_OF_AVATAR_INFO);
	//mAVATAR_OBJECT[mUserIndex].Init();

	//__LogMgr()->Log("![%s] : %s : %s : %d\n", tFuncName, mUSER[mUserIndex].uID, tFileName, tLine);
	GameLogger(4, "![%s] : %s : %s : %d\n", tFuncName, mUSER[mUserIndex].uID, tFileName, tLine);

	if (pthread_mutex_unlock(&m_user_quit_lock) != 0)
	{
	}
}

bool MyUser::IsValidSkillNumber(int skillNumber)
{
	for (int index = 0; index < MAX_SKILL_SLOT_NUM; ++index)
	{
		if (mAvatarInfo.aSkill[index][0] == skillNumber)
		{
			return true;
		}
	}
	return false;
}

int MyUser::GetMaxSkillGradeNum(int skillNumber)
{
	for (int index = 0; index < MAX_SKILL_SLOT_NUM; ++index)
	{
		if (mAvatarInfo.aSkill[index][0] == skillNumber)
		{
			return mAvatarInfo.aSkill[index][1];
		}
	}
	return -1;
}

//-------------------------------------------------------------------------------------------------
