#include "MainApp.h"


WORKER_FUNCTION W_FUNCTION[256];

void W_TEMP_REGISTER_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (mUSER[tUserIndex].mCheckTempRegister)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	char tID[MAX_USER_ID_LENGTH];
	int tribe;
	CopyMemory(&tID[0], &tPacket[1], MAX_USER_ID_LENGTH);
	tID[(MAX_USER_ID_LENGTH - 1)] = '\0';
	CopyMemory(&tribe, &tPacket[(1 + MAX_USER_ID_LENGTH)], 4);

	int index01;
	int tTribeNum[4];

	switch (mSERVER_INFO.mServerNumber)
	{
	case 49:
	case 51:
	case 53:
	case 146:
	case 147:
	case 148:
	case 149:
	case 150:
	case 151:
	case 152:
	case 153:
#ifdef __GOD__
	case 267:
#endif
		if ((tribe < 0) || (tribe > 2))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		tTribeNum[0] = 0;
		tTribeNum[1] = 0;
		tTribeNum[2] = 0;
		for (index01 = 0; index01 < mSERVER.mMAX_USER_NUM; index01++)
		{
			if (!mUSER[index01].mCheckConnectState)
			{
				continue;
			}
			if (!mUSER[index01].mCheckTempRegister)
			{
				continue;
			}
			tTribeNum[mUSER[index01].mTempRegisterTribe]++;
		}
		if (tTribeNum[tribe] >= (mSERVER.mMAX_USER_NUM / 3))
		{
			mTRANSFER.B_TEMP_REGISTER_RECV(1);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		break;
	case 154:
	case 155:
	case 156:
	case 157:
	case 158:
	case 159:
	case 160:
	case 161:
	case 162:
	case 163:
	case 164:
#ifdef __GOD__
	case 120:
	case 121:
	case 122:
	case 268:
	case 269:
#endif
		if ((tribe < 0) || (tribe > 3))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		tTribeNum[0] = 0;
		tTribeNum[1] = 0;
		tTribeNum[2] = 0;
		tTribeNum[3] = 0;
		for (index01 = 0; index01 < mSERVER.mMAX_USER_NUM; index01++)
		{
			if (!mUSER[index01].mCheckConnectState)
			{
				continue;
			}
			if (!mUSER[index01].mCheckTempRegister)
			{
				continue;
			}
			tTribeNum[mUSER[index01].mTempRegisterTribe]++;
		}
		if (tTribeNum[tribe] >= (mSERVER.mMAX_USER_NUM / 4))
		{
			mTRANSFER.B_TEMP_REGISTER_RECV(1);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		break;
#ifdef __GOD__
	case 194:
		if ((tribe < 0) || (tribe > 3))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		tTribeNum[0] = 0;
		tTribeNum[1] = 0;
		tTribeNum[2] = 0;
		tTribeNum[3] = 0;
		for (index01 = 0; index01 < mSERVER.mMAX_USER_NUM; index01++)
		{
			if (!mUSER[index01].mCheckConnectState)
			{
				continue;
			}
			if (!mUSER[index01].mCheckTempRegister)
			{
				continue;
			}
			tTribeNum[mUSER[index01].mTempRegisterTribe]++;
		}
		if (tTribeNum[tribe] >= 25)
		{
			mTRANSFER.B_TEMP_REGISTER_RECV(1);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		break;
#endif
	}
	mPLAYUSER_COM.U_REGISTER_USER_FOR_ZONE_0_SEND(&tID[0]);
	if (mPLAYUSER_COM.mRecv_Result != 0)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	mTRANSFER.B_TEMP_REGISTER_RECV(0);
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	mUSER[tUserIndex].mCheckTempRegister = TRUE;
	mUSER[tUserIndex].mTempRegisterTribe = tribe;
	mUSER[tUserIndex].mRegisterTime = GetTickCount();
	mUSER[tUserIndex].mPlayUserIndex = mPLAYUSER_COM.mRecv_PlayUserIndex;
	strncpy(mUSER[tUserIndex].uID, tID, MAX_USER_ID_LENGTH);
	mUSER[tUserIndex].mHeartCheckTime = time(nullptr);
}

//W_REGISTER_AVATAR_SEND
void W_REGISTER_AVATAR_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if ((!mUSER[tUserIndex].mCheckTempRegister) || (mUSER[tUserIndex].mCheckValidState))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	char tID[MAX_USER_ID_LENGTH];
	char tAvatarName[MAX_AVATAR_NAME_LENGTH];
	ACTION_INFO tAction;
	CopyMemory(&tID[0], &tPacket[1], MAX_USER_ID_LENGTH);
	tID[(MAX_USER_ID_LENGTH - 1)] = '\0';
	CopyMemory(&tAvatarName[0], &tPacket[(1 + MAX_USER_ID_LENGTH)], MAX_AVATAR_NAME_LENGTH);
	tAvatarName[(MAX_AVATAR_NAME_LENGTH - 1)] = '\0';
	CopyMemory(&tAction, &tPacket[(1 + MAX_USER_ID_LENGTH + MAX_AVATAR_NAME_LENGTH)], SIZE_OF_ACTION_INFO);
	if ((strcmp(tID, mUSER[tUserIndex].uID) != 0) || (tAction.aType != 0) || ((tAction.aSort != 0) && (tAction.aSort !=
		1)))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int index01;
	int tLogoutInfo[6];
	int tSort;
	BYTE tData[MAX_BROADCAST_DATA_SIZE];

	time_t tCountSeconds;
	struct tm* tPresentTime;
	char cDate[100];
	unsigned int iDate = 0;

	time(&tCountSeconds);
	tPresentTime = localtime(&tCountSeconds);

	tLogoutInfo[0] = mSERVER_INFO.mServerNumber;
	tLogoutInfo[1] = static_cast<int>(tAction.aLocation[0]);
	tLogoutInfo[2] = static_cast<int>(tAction.aLocation[1]);
	tLogoutInfo[3] = static_cast<int>(tAction.aLocation[2]);
	mPLAYUSER_COM.U_REGISTER_USER_FOR_ZONE_1_SEND(mUSER[tUserIndex].mPlayUserIndex, &tID[0], &tAvatarName[0],
	                                              &tLogoutInfo[0]);
	if (mPLAYUSER_COM.mRecv_Result != 0)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	//-------------------//
	//EXAMINE_TRIBE_STATE//
	//-------------------//
	switch (mPLAYUSER_COM.mRecv_AvatarInfo.aTribe)
	{
	case 0:
		if (mPLAYUSER_COM.mRecv_AvatarInfo.aPreviousTribe != 0)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		break;
	case 1:
		if (mPLAYUSER_COM.mRecv_AvatarInfo.aPreviousTribe != 1)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		break;
	case 2:
		if (mPLAYUSER_COM.mRecv_AvatarInfo.aPreviousTribe != 2)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		break;
	case 3:
		if ((mPLAYUSER_COM.mRecv_AvatarInfo.aPreviousTribe != 0) && (mPLAYUSER_COM.mRecv_AvatarInfo.aPreviousTribe != 1)
			&& (mPLAYUSER_COM.mRecv_AvatarInfo.aPreviousTribe != 2))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		break;
	default:
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	//-------------------//
	//-------------------//
	//-------------------//
	if (mPLAYUSER_COM.mRecv_AuthInfo.AuthType == 0)
	{
#if 0
#ifdef __GOD__
		if (((mPLAYUSER_COM.mRecv_AvatarInfo.aLevel1 + mPLAYUSER_COM.mRecv_AvatarInfo.aLevel2) < mZONEMAININFO.ReturnMinZoneLevelInfo(mSERVER_INFO.mServerNumber)) || (mPLAYUSER_COM.mRecv_AvatarInfo.aLevel1 > mZONEMAININFO.ReturnMaxZoneLevelInfo(mSERVER_INFO.mServerNumber)))
#else
		if ((mPLAYUSER_COM.mRecv_AvatarInfo.aLevel1 < mZONEMAININFO.ReturnMinZoneLevelInfo(mSERVER_INFO.mServerNumber)) || (mPLAYUSER_COM.mRecv_AvatarInfo.aLevel1 > mZONEMAININFO.ReturnMaxZoneLevelInfo(mSERVER_INFO.mServerNumber)))
#endif
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
#endif
		if (mGAME.mCheckZone101TypeServer)
		{
#ifdef __GOD__
			if (mPLAYUSER_COM.mRecv_AvatarInfo.aLevel2 > 0)
			{
#endif
				if (mPLAYUSER_COM.mRecv_AvatarInfo.aZone101TypeTime < 1)
				{
					mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
					return;
				}
				mPLAYUSER_COM.mRecv_AvatarInfo.aZone101TypeTime--;
#ifdef __GOD__
			}
#endif
		}
		if (mGAME.mCheckZone125Server)
		{
			if (mPLAYUSER_COM.mRecv_AvatarInfo.aZone125TypeTime < 1)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			mPLAYUSER_COM.mRecv_AvatarInfo.aZone125TypeTime--;
		}
		if (mGAME.mCheckZone126TypeServer)
		{
			if (mPLAYUSER_COM.mRecv_LoginPremiumPCRoom != 2)
			{
				if (mPLAYUSER_COM.mRecv_AvatarInfo.aZone126TypeTime < 1)
				{
					mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
					return;
				}
				mPLAYUSER_COM.mRecv_AvatarInfo.aZone126TypeTime--;
			}
		}
		if (mGAME.mCheckZone175TypeServer)
		{
			if (mGAME.mWorldInfo.mZone175TypeState[mGAME.mZone175TypeZoneIndex1][mGAME.mZone175TypeZoneIndex2] != 1)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			if (mPLAYUSER_COM.mRecv_AvatarInfo.aNineTowerKeyNum < 1)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			mPLAYUSER_COM.mRecv_AvatarInfo.aNineTowerKeyNum--;
		}
		if (mGAME.mCheckZone241TypeServer)
		{
			if (mPLAYUSER_COM.mRecv_AvatarInfo.aLifeOrDeath < 1)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			mPLAYUSER_COM.mRecv_AvatarInfo.aLifeOrDeath--;
		}
		if (mSERVER_INFO.mServerNumber == 234)
		{
			if (mPLAYUSER_COM.mRecv_AvatarInfo.aIslandOfBloodTime < 1)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			mPLAYUSER_COM.mRecv_AvatarInfo.aIslandOfBloodTime--;
		}
		if (mSERVER_INFO.mServerNumber == 235)
		{
			if (mPLAYUSER_COM.mRecv_AvatarInfo.aIslandOfSoul1Time < 1)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			mPLAYUSER_COM.mRecv_AvatarInfo.aIslandOfSoul1Time--;
		}
		if (mSERVER_INFO.mServerNumber == 236)
		{
			if (mPLAYUSER_COM.mRecv_AvatarInfo.aIslandOfSoul2Time < 1)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			mPLAYUSER_COM.mRecv_AvatarInfo.aIslandOfSoul2Time--;
		}
		if (mSERVER_INFO.mServerNumber == 237)
		{
			if (mPLAYUSER_COM.mRecv_AvatarInfo.aIslandOfSoul3Time < 1)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			mPLAYUSER_COM.mRecv_AvatarInfo.aIslandOfSoul3Time--;
		}
		if (mSERVER_INFO.mServerNumber == 238)
		{
			if (mPLAYUSER_COM.mRecv_AvatarInfo.aIslandOfSoul4Time < 1)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			mPLAYUSER_COM.mRecv_AvatarInfo.aIslandOfSoul4Time--;
		}
		if (mSERVER_INFO.mServerNumber == 239)
		{
			if (mPLAYUSER_COM.mRecv_AvatarInfo.aIslandOfSoul5Time < 1)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			mPLAYUSER_COM.mRecv_AvatarInfo.aIslandOfSoul5Time--;
		}
		if (mSERVER_INFO.mServerNumber == 240)
		{
			if (mPLAYUSER_COM.mRecv_AvatarInfo.aIslandOfSoul6Time < 1)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			mPLAYUSER_COM.mRecv_AvatarInfo.aIslandOfSoul6Time--;
		}

#if 0
		if(84 == mSERVER_INFO.mServerNumber){
			if( (mPLAYUSER_COM.mRecv_AvatarInfo.aZone84Time & 0x0000000F) < 1 )
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			--mPLAYUSER_COM.mRecv_AvatarInfo.aZone84Time;
		}
#endif
	}
	if (nullptr != tPresentTime)
	{
		memset(cDate, 0, 100);
		_snprintf(cDate, 100, "%04d%02d%02d", tPresentTime->tm_year + 1900, tPresentTime->tm_mon + 1,
		          tPresentTime->tm_mday);
		iDate = atoi(cDate);

#if 0
		if(iDate != (mPLAYUSER_COM.mRecv_AvatarInfo.aZone84Time >> 4))
		{
			mPLAYUSER_COM.mRecv_AvatarInfo.aZone84Time = mPLAYUSER_COM.mRecv_AvatarInfo.aZone84Time & 0xFFFFFFF0;
		}
#endif
	}

	mUSER[tUserIndex].mCheckValidState = TRUE;
	mUSER[tUserIndex].mRegisterTime = GetTickCount();
	mUSER[tUserIndex].uUserSort = mPLAYUSER_COM.mRecv_UserSort;
#if 0
	mUSER[tUserIndex].uGoodFellow =  mPLAYUSER_COM.mRecv_GoodFellow;
	mUSER[tUserIndex].uLoginPlace =  mPLAYUSER_COM.mRecv_LoginPlace;
	mUSER[tUserIndex].uLoginPremium =  mPLAYUSER_COM.mRecv_LoginPremium;
	mUSER[tUserIndex].uLoginPremiumPCRoom =  mPLAYUSER_COM.mRecv_LoginPremiumPCRoom;
	mUSER[tUserIndex].uTraceState =  mPLAYUSER_COM.mRecv_TraceState;
	mUSER[tUserIndex].uBonus100Money = mPLAYUSER_COM.mRecv_Bonus100Money;
	mUSER[tUserIndex].uPremiumServerExpirationDate = mPLAYUSER_COM.mRecv_PremiumServerExpirationDate;
#endif
	CopyMemory(&mUSER[tUserIndex].mAvatarInfo, &mPLAYUSER_COM.mRecv_AvatarInfo, SIZE_OF_AVATAR_INFO);
#ifdef __IVEN_BUG_FIX__
	int iPage = 0;
	int iIndex = 0;
	int iNewPage = 0;
	int iNewIndex = 0;
	bool bItemMove = false;
	ITEM_INFO* pItemInfo = nullptr;

	for (iPage = 0; iPage < 2; iPage++)
	{
		for (iIndex = 0; iIndex < MAX_INVENTORY_SLOT_NUM; iIndex++)
		{
			bItemMove = false;
			if (((mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iIndex][1] >= 7)
					|| (mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iIndex][2] >= 7))
				|| ((mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iIndex][1] < 0)
					|| (mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iIndex][2] < 0)))
			{
				pItemInfo = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iIndex][0]);
				if (pItemInfo == nullptr)
				{
					mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iIndex][0] = 0;
					mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iIndex][1] = 0;
					mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iIndex][2] = 0;
					mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iIndex][3] = 0;
					mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iIndex][4] = 0;
					mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iIndex][5] = 0;
					continue;
				}

				if ((pItemInfo->iSort != 2) && (pItemInfo->iSort != 7) && (pItemInfo->iSort != 11))
				{
					bItemMove = true;
				}
				else if (((mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iIndex][1] >= 8)
						|| (mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iIndex][2] >= 8))
					|| ((mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iIndex][1] < 0)
						|| (mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iIndex][2] < 0)))
				{
					bItemMove = true;
				}
				if (bItemMove)
				{
					if (mUTIL.FindEmptyInvenForItem(tUserIndex, pItemInfo, iNewPage, iNewIndex))
					{
						if (iNewPage == iPage)
						{
							mUSER[tUserIndex].mAvatarInfo.aInventory[iNewPage][iIndex][1]
								= iNewIndex % 8;
							mUSER[tUserIndex].mAvatarInfo.aInventory[iNewPage][iIndex][2]
								= iNewIndex / 8;
						}
						else
						{
							mUSER[tUserIndex].mAvatarInfo.aInventory[iNewPage][iIndex][0]
								= mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iIndex][0];
							mUSER[tUserIndex].mAvatarInfo.aInventory[iNewPage][iIndex][1]
								= iNewIndex % 8;
							mUSER[tUserIndex].mAvatarInfo.aInventory[iNewPage][iIndex][2]
								= iNewIndex / 8;
							mUSER[tUserIndex].mAvatarInfo.aInventory[iNewPage][iIndex][3]
								= mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iIndex][3];
							mUSER[tUserIndex].mAvatarInfo.aInventory[iNewPage][iIndex][4]
								= mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iIndex][4];
							mUSER[tUserIndex].mAvatarInfo.aInventory[iNewPage][iIndex][5]
								= mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iIndex][5];
							mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iIndex][0] = 0;
							mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iIndex][1] = 0;
							mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iIndex][2] = 0;
							mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iIndex][3] = 0;
							mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iIndex][4] = 0;
							mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iIndex][5] = 0;
						}
#if 0
						__LogMgr()->Log("%d %d %d %d\n"
							, mUSER[tUserIndex].mAvatarInfo.aInventory[iNewPage][iIndex][0]
							, mUSER[tUserIndex].mAvatarInfo.aInventory[iNewPage][iIndex][1]
							, mUSER[tUserIndex].mAvatarInfo.aInventory[iNewPage][iIndex][2]
							, mUSER[tUserIndex].mAvatarInfo.aInventory[iNewPage][iIndex][3]);
#endif
					}
				}
			}
		}
	}
#endif
	if (mUSER[tUserIndex].mAvatarInfo.aLogoutInfo[4] < 1)
	{
		mUSER[tUserIndex].mAvatarInfo.aLogoutInfo[4] = 1;
	}
	CopyMemory(mUSER[tUserIndex].mEffectValue, mPLAYUSER_COM.mRecv_EffectValue, (MAX_AVATAR_EFFECT_SORT_NUM * 8));
	switch (mPLAYUSER_COM.mRecv_LoginPremiumPCRoom)
	{
	case 2:
		mUSER[tUserIndex].mGeneralExperienceUpRatio = 0.2f;
		mUSER[tUserIndex].mItemDropUpRatio = 0.5f;
		mUSER[tUserIndex].mGeneralExperienceDownRatio = 0.5f;
		mUSER[tUserIndex].mSupportSkillTimeUpRatio = 1;
		break;
	default:
		mUSER[tUserIndex].mGeneralExperienceUpRatio = 0.0f;
		mUSER[tUserIndex].mItemDropUpRatio = 1.0f;
		mUSER[tUserIndex].mGeneralExperienceDownRatio = 1.0f;
		mUSER[tUserIndex].mSupportSkillTimeUpRatio = 1;
		break;
	}
#if 0
	if (mUSER[tUserIndex].mAvatarInfo.aPlayTime2 < 1)
	{
		mUSER[tUserIndex].mCheckHealthState = FALSE;
		mUSER[tUserIndex].mHealthState = 1;*/
	}
	else
	{
		// Disabled because it interferes with exp gain when login time > 5hours
		mUSER[tUserIndex].mCheckHealthState = TRUE;
		mUSER[tUserIndex].mHealthState = mUTIL.ReturnHealthState(mUSER[tUserIndex].mAvatarInfo.aPlayTime2);
	}
#endif

	/** 2009. 10. 26 : DarkAttack Potion add (wolf)*/
	mUSER[tUserIndex].mTickCountForDarkAttack = 0;
	mUSER[tUserIndex].iKindDarkAttack = 0;
	/** 2009. 10. 26 : DarkAttack Potion add (wolf)*/

	mUSER[tUserIndex].mTickCountFor01Second = mGAME.mTickCount;
	mUSER[tUserIndex].mTickCountFor01SecondForProtect = mGAME.mTickCount;
	mUSER[tUserIndex].mTickCountFor30Second = mGAME.mTickCount;
	mUSER[tUserIndex].mTickCountFor01MinuteForHealth = mGAME.mTickCount;
	mUSER[tUserIndex].mTickCountFor01Minute = mGAME.mTickCount;

	mUSER[tUserIndex].mTickCountFor01Minute_2 = mGAME.mTickCount;
	mUSER[tUserIndex].mTickCountFor03Second = mGAME.mTickCount;

	mUSER[tUserIndex].mXTrapTick = mGAME.mTickCount;


	mUSER[tUserIndex].mCheckMaxAttackPacketNum = 1;
#ifdef __GOD__
	mUSER[tUserIndex].mAttackPacketSort = 0;
#endif
	mUSER[tUserIndex].mMaxAttackPacketNum = 0;
	mUSER[tUserIndex].mNowAttackPacketNum = 0;
	mUSER[tUserIndex].mHackCheckTime = GetTickCount();
	mUSER[tUserIndex].mMoveZoneResult = 0;
	CopyMemory(&mUSER[tUserIndex].mAuthInfo, &mPLAYUSER_COM.mRecv_AuthInfo, sizeof(AUTH_INFO));


	mTRANSFER.B_REGISTER_AVATAR_RECV(&mUSER[tUserIndex].mAvatarInfo, mUSER[tUserIndex].mEffectValue);
	// mUSER[tUserIndex].uPremiumServerExpirationDate Ãß°¡. // @_Premium_Server_@
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);


	mTRANSFER.B_BROADCAST_WORLD_INFO(&mGAME.mWorldInfo, &mGAME.mTribeInfo);
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);


	mTRANSFER.DEMAND_OBJECT_RECV(56, mGAME.mGeneralExperienceUpRatio * 10);
	mUSER[tUserIndex].Send(TRUE, (char*)mTRANSFER.mOriginal, mTRANSFER.mOriginalSize, false);
	mTRANSFER.DEMAND_OBJECT_RECV(57, mGAME.mItemDropUpRatio * 10);
	mUSER[tUserIndex].Send(TRUE, (char*)mTRANSFER.mOriginal, mTRANSFER.mOriginalSize, false);
	mTRANSFER.DEMAND_OBJECT_RECV(58, mGAME.mKillOtherTribeAddValue);
	mUSER[tUserIndex].Send(TRUE, (char*)mTRANSFER.mOriginal, mTRANSFER.mOriginalSize, false);

	mAVATAR_OBJECT[tUserIndex].mCheckValidState = TRUE;
	mAVATAR_OBJECT[tUserIndex].mUniqueNumber = mGAME.mAvatarObjectUniqueNumber;
	mGAME.mAvatarObjectUniqueNumber++;
	mAVATAR_OBJECT[tUserIndex].mDATA.aVisibleState = static_cast<unsigned char>(mUSER[tUserIndex].mAvatarInfo.
		aVisibleState);
	mAVATAR_OBJECT[tUserIndex].mDATA.aSpecialState = static_cast<unsigned char>(mUSER[tUserIndex].mAvatarInfo.
		aSpecialState);
	mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe = mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe;
	strncpy(mAVATAR_OBJECT[tUserIndex].mDATA.aGuildName, mUSER[tUserIndex].mAvatarInfo.aGuildName,
	        MAX_GUILD_NAME_LENGTH);
	mAVATAR_OBJECT[tUserIndex].mDATA.aGuildRole = mUSER[tUserIndex].mAvatarInfo.aGuildRole;
	strncpy(mAVATAR_OBJECT[tUserIndex].mDATA.aCallName, mUSER[tUserIndex].mAvatarInfo.aCallName, MAX_CALL_NAME_LENGTH);
	mAVATAR_OBJECT[tUserIndex].mDATA.aGuildMarkEffect = mUSER[tUserIndex].mAvatarInfo.aGuildMarkEffect;
	strncpy(mAVATAR_OBJECT[tUserIndex].mDATA.aName, mUSER[tUserIndex].mAvatarInfo.aName, MAX_AVATAR_NAME_LENGTH);
	mAVATAR_OBJECT[tUserIndex].mDATA.aTribe = mUSER[tUserIndex].mAvatarInfo.aTribe;
	mAVATAR_OBJECT[tUserIndex].mDATA.aPreviousTribe = mUSER[tUserIndex].mAvatarInfo.aPreviousTribe;
	mAVATAR_OBJECT[tUserIndex].mDATA.aGender = mUSER[tUserIndex].mAvatarInfo.aGender;
	mAVATAR_OBJECT[tUserIndex].mDATA.aHeadType = mUSER[tUserIndex].mAvatarInfo.aHeadType;
	mAVATAR_OBJECT[tUserIndex].mDATA.aFaceType = mUSER[tUserIndex].mAvatarInfo.aFaceType;
	mAVATAR_OBJECT[tUserIndex].mDATA.aLevel1 = mUSER[tUserIndex].mAvatarInfo.aLevel1;
	mAVATAR_OBJECT[tUserIndex].mDATA.aLevel2 = mUSER[tUserIndex].mAvatarInfo.aLevel2;
	mAVATAR_OBJECT[tUserIndex].mDATA.aRebirth = mUSER[tUserIndex].mAvatarInfo.aRebirth;
	mAVATAR_OBJECT[tUserIndex].mDATA.aTitle = mUSER[tUserIndex].mAvatarInfo.aTitle;
	mAVATAR_OBJECT[tUserIndex].mDATA.aHalo = mUSER[tUserIndex].mAvatarInfo.aUpgradeValue;
	mAVATAR_OBJECT[tUserIndex].mDATA.aBotOn1 = 0;
	mAVATAR_OBJECT[tUserIndex].mDATA.aAnimalOn = mUSER[tUserIndex].mAvatarInfo.aAbsorbState;
	mAVATAR_OBJECT[tUserIndex].mDATA.aRankPoint = mUSER[tUserIndex].mAvatarInfo.aRankPoints;
	for (index01 = 0; index01 < MAX_EQUIP_SLOT_NUM; index01++)
	{
		mAVATAR_OBJECT[tUserIndex].mDATA.aEquipForView[index01][0] = mUSER[tUserIndex].mAvatarInfo.aEquip[index01][0];
		mAVATAR_OBJECT[tUserIndex].mDATA.aEquipForView[index01][1] = mUSER[tUserIndex].mAvatarInfo.aEquip[index01][2];
	}
	CopyMemory(&mAVATAR_OBJECT[tUserIndex].mDATA.aAction, &tAction, SIZE_OF_ACTION_INFO);
	mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue = mUSER[tUserIndex].mAvatarInfo.aLogoutInfo[4];
	mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue = mUSER[tUserIndex].mAvatarInfo.aLogoutInfo[5];

	for (index01 = 0; index01 < MAX_AVATAR_EFFECT_SORT_NUM; index01++)
	{
		mAVATAR_OBJECT[tUserIndex].mDATA.aEffectValueForView[index01] = mUSER[tUserIndex].mEffectValue[index01][0];
	}


	if ((mUSER[tUserIndex].mAvatarInfo.aCostumeIndex >= MAX_AVATAR_ANIMAL_NUM) && (mUSER[tUserIndex].mAvatarInfo.
		aCostumeIndex <= (MAX_AVATAR_ANIMAL_NUM + MAX_AVATAR_ANIMAL_NUM - 1)))
	{
		mAVATAR_OBJECT[tUserIndex].mDATA.aFashionNumber = mUSER[tUserIndex].mAvatarInfo.aCostume[(mUSER[tUserIndex].
			mAvatarInfo.aCostumeIndex - MAX_AVATAR_ANIMAL_NUM)];
	}
	else
	{
		mAVATAR_OBJECT[tUserIndex].mDATA.aFashionNumber = 0;
	}


#ifdef __ANIMAL__
	if ((mUSER[tUserIndex].mAvatarInfo.aAnimalIndex >= MAX_AVATAR_ANIMAL_NUM) && (mUSER[tUserIndex].mAvatarInfo.
		aAnimalIndex <= (MAX_AVATAR_ANIMAL_NUM + MAX_AVATAR_ANIMAL_NUM - 1)))
	{
		mAVATAR_OBJECT[tUserIndex].mDATA.aAnimalNumber = mUSER[tUserIndex].mAvatarInfo.aAnimal[(mUSER[tUserIndex].
			mAvatarInfo.aAnimalIndex - MAX_AVATAR_ANIMAL_NUM)];
	}
	else
	{
		mAVATAR_OBJECT[tUserIndex].mDATA.aAnimalNumber = 0;
	}
#endif
	strncpy(mAVATAR_OBJECT[tUserIndex].mDATA.aPartyName, mUSER[tUserIndex].mAvatarInfo.aPartyName[0],
	        MAX_PARTY_NAME_LENGTH);
	mAVATAR_OBJECT[tUserIndex].mDATA.aDuelState[0] = 0;
	mAVATAR_OBJECT[tUserIndex].mDATA.aDuelState[1] = 0;
	mAVATAR_OBJECT[tUserIndex].mDATA.aDuelState[2] = 0;
	mAVATAR_OBJECT[tUserIndex].mDATA.aPShopState = 0;
	mAVATAR_OBJECT[tUserIndex].mCheckPossibleEatPotion = TRUE;
	mAVATAR_OBJECT[tUserIndex].mDuelProcessState = 0;
	mAVATAR_OBJECT[tUserIndex].mTradeProcessState = 0;
	mAVATAR_OBJECT[tUserIndex].mFriendProcessState = 0;
	mAVATAR_OBJECT[tUserIndex].mPartyProcessState = 0;
	mAVATAR_OBJECT[tUserIndex].mGuildProcessState = 0;
	mAVATAR_OBJECT[tUserIndex].mTeacherProcessState = 0;
	mAVATAR_OBJECT[tUserIndex].SetSpaceIndex();
	mAVATAR_OBJECT[tUserIndex].mPRE_LOCATION[0] = mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0];
	mAVATAR_OBJECT[tUserIndex].mPRE_LOCATION[1] = mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[1];
	mAVATAR_OBJECT[tUserIndex].mPRE_LOCATION[2] = mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[2];
	mAVATAR_OBJECT[tUserIndex].mUpdateTimeForBroadcast = GetTickCount();
	mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
	mAVATAR_OBJECT[tUserIndex].mGod = 0;
	mAVATAR_OBJECT[tUserIndex].mChat = 0;
	if (mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue > mAVATAR_OBJECT[tUserIndex].mDATA.aMaxLifeValue)
	{
		mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue = mAVATAR_OBJECT[tUserIndex].mDATA.aMaxLifeValue;
	}
	if (mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue > mAVATAR_OBJECT[tUserIndex].mDATA.aMaxManaValue)
	{
		mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue = mAVATAR_OBJECT[tUserIndex].mDATA.aMaxManaValue;
	}
	mTRANSFER.B_AVATAR_ACTION_RECV(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber,
	                               &mAVATAR_OBJECT[tUserIndex].mDATA, 1);
	if (mUSER[tUserIndex].mAvatarInfo.aVisibleState == 0)
	{
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	}
	else
	{
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, tUserIndex,
		                false);
	}
	if ((mUSER[tUserIndex].mAvatarInfo.aVisibleState == 1) && (mPLAYUSER_COM.mRecv_CheckFirstZone == 1) && (strcmp(
		mUSER[tUserIndex].mAvatarInfo.aGuildName, "") != 0))
	{
		tSort = 110;
		CopyMemory(&tData[0], &mUSER[tUserIndex].mAvatarInfo.aGuildName[0], MAX_GUILD_NAME_LENGTH);
		CopyMemory(&tData[MAX_GUILD_NAME_LENGTH], &mUSER[tUserIndex].mAvatarInfo.aName[0], MAX_AVATAR_NAME_LENGTH);
		mRELAY_COM.U_DEMAND_BROADCAST_DATA(tSort, &tData[0]);
	}

#if 0
	mTRANSFER.B_AVATAR_CHANGE_INFO_2( 29, iTemp_Auto_time );
	mUSER[tUserIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );

	for (index01 = 0; index01 < 1000; index01++)
	{
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(index01, index01);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	}
	mTRANSFER.B_AVATAR_CHANGE_INFO_2( 42, mUSER[tUserIndex].mAvatarInfo.aZone84Time);
	mUSER[tUserIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
#endif

	mUSER[tUserIndex].mHeartCheckTime = time(nullptr);

	mUSER[tUserIndex].mMoveItemInfo = mPLAYUSER_COM.mRecv_MoveItemInfo;

	if (((mUSER[tUserIndex].mMoveItemInfo.mInvenPage > -1) && (mUSER[tUserIndex].mMoveItemInfo.mInvenPage < 2)) &&
		((mUSER[tUserIndex].mMoveItemInfo.mInvenIndex > -1) && (mUSER[tUserIndex].mMoveItemInfo.mInvenIndex <
			MAX_INVENTORY_SLOT_NUM)) &&
		((mUSER[tUserIndex].mMoveItemInfo.mInvenX > -1) && (mUSER[tUserIndex].mMoveItemInfo.mInvenX < 8)) &&
		((mUSER[tUserIndex].mMoveItemInfo.mInvenY > -1) && (mUSER[tUserIndex].mMoveItemInfo.mInvenY < 8)))
	{
		int tPage = mUSER[tUserIndex].mMoveItemInfo.mInvenPage;
		int tIndex = mUSER[tUserIndex].mMoveItemInfo.mInvenIndex;
		if ((mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] < 1) || (mUSER[tUserIndex].mAvatarInfo.
			aInventory[tPage][tIndex][0] != 1109))
		{
			//mGAMELOG.GL_701_CHEAT_USER(tUserIndex, 1);
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}

		//mGAMELOG.GL_606_USE_INVENTORY_ITEM( tUserIndex, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], 0, 0, 0, 0 );

		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3]--;
		if (mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] < 1)
		{
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		}
		//mTRANSFER.B_AVATAR_CHANGE_INFO_2(203, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3]);
		//mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);

		memset(&mUSER[tUserIndex].mMoveItemInfo, -1, sizeof(MOVE_ITEM_INFO));
		mPLAYUSER_COM.U_SET_MOVE_ITEM_FOR_ZONE_SEND(mUSER[tUserIndex].mPlayUserIndex, &mUSER[tUserIndex].mMoveItemInfo);
	}
}

//W_CLIENT_OK_FOR_ZONE_SEND
void W_CLIENT_OK_FOR_ZONE_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	int tTribe;
	time_t tCurrentTime = time(nullptr);
	CopyMemory(&tTribe, &tPacket[1], 4);

	if (mUSER[tUserIndex].mAvatarInfo.aTribe != tTribe)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	if ((tCurrentTime - mUSER[tUserIndex].mHeartCheckTime) <= 3)
	{
		mUSER[tUserIndex].mHeartCheckCount++;
		if (mUSER[tUserIndex].mHeartCheckCount > 10)
		{
			__LogMgr()->Log("![HACK] : %s, %s, %d, %d\n"
			                , mUSER[tUserIndex].uID, mUSER[tUserIndex].mAvatarInfo.aName
			                , (tCurrentTime - mUSER[tUserIndex].mHeartCheckTime), mUSER[tUserIndex].mHeartCheckCount);
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		}
		return;
	}
	mUSER[tUserIndex].mHeartCheckCount = 0;
	mUSER[tUserIndex].mHeartCheckTime = tCurrentTime;
}

//W_AVATAR_ACTION_SEND
void W_AVATAR_ACTION_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	ACTION_INFO tAction;
	CopyMemory(&tAction, &tPacket[1], SIZE_OF_ACTION_INFO);

	int index01;
	int index02;
	int tSkillSort;
	int tUseManaValue;
	int tReduceManaRatio;
#ifdef __GOD__
	if (!mWORK.CheckValidCharacterMotionForSend(tAction.aType, tAction.aSort, tAction.aSkillNumber, &tSkillSort,
	                                            &mUSER[tUserIndex].mCheckMaxAttackPacketNum,
	                                            &mUSER[tUserIndex].mAttackPacketSort,
	                                            &mUSER[tUserIndex].mMaxAttackPacketNum))
#else
	if (!mWORK.CheckValidCharacterMotionForSend(tAction.aType, tAction.aSort, tAction.aSkillNumber, &tSkillSort, &mUSER[tUserIndex].mCheckMaxAttackPacketNum, &mUSER[tUserIndex].mMaxAttackPacketNum))
#endif
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	mUSER[tUserIndex].mNowAttackPacketNum = 0;
	tUseManaValue = 0;
	switch (tSkillSort)
	{
	case 1:
		for (index01 = 0; index01 < 3; index01++)
		{
			for (index02 = 0; index02 < MAX_HOT_KEY_NUM; index02++)
			{
				if ((mUSER[tUserIndex].mAvatarInfo.aHotKey[index01][index02][2] == 1) && (mUSER[tUserIndex].mAvatarInfo.
					aHotKey[index01][index02][0] == tAction.aSkillNumber) && (mUSER[tUserIndex].mAvatarInfo.aHotKey[
					index01][index02][1] == tAction.aSkillGradeNum1))
				{
					break;
				}
			}
			if (index02 < MAX_HOT_KEY_NUM)
			{
				break;
			}
		}
		if (index01 == 3)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mAVATAR_OBJECT[tUserIndex].GetBonusSkillValue(tAction.aSkillNumber) != tAction.aSkillGradeNum2)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		break;
	case 2:
		if ((mUSER[tUserIndex].mAvatarInfo.aAutoHuntState == 1) && ((mUSER[tUserIndex].mAvatarInfo.aAutoHunt7Day > mUTIL
			.ReturnNowDate()) || (mUSER[tUserIndex].mAvatarInfo.aAutoHunt5Hour > 0)))
		{
			for (index01 = 0; index01 < 2; index01++)
			{
				if ((mUSER[tUserIndex].mAvatarInfo.aAutoSkillAttack[index01][0] == tAction.aSkillNumber) && (mUSER[
					tUserIndex].mAvatarInfo.aAutoSkillAttack[index01][1] == tAction.aSkillGradeNum1))
				{
					break;
				}
			}
			if (index01 == 2)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
		}
		else
		{
			for (index01 = 0; index01 < 3; index01++)
			{
				for (index02 = 0; index02 < MAX_HOT_KEY_NUM; index02++)
				{
					if ((mUSER[tUserIndex].mAvatarInfo.aHotKey[index01][index02][2] == 1) && (mUSER[tUserIndex].
						mAvatarInfo.aHotKey[index01][index02][0] == tAction.aSkillNumber) && (mUSER[tUserIndex].
						mAvatarInfo.aHotKey[index01][index02][1] == tAction.aSkillGradeNum1))
					{
						break;
					}
				}
				if (index02 < MAX_HOT_KEY_NUM)
				{
					break;
				}
			}
			if (index01 == 3)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
		}
		if (mAVATAR_OBJECT[tUserIndex].GetBonusSkillValue(tAction.aSkillNumber) != tAction.aSkillGradeNum2)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		tUseManaValue = static_cast<int>(mSKILL.ReturnSkillValue(tAction.aSkillNumber, tAction.aSkillGradeNum1, 1));
		tReduceManaRatio = mAVATAR_OBJECT[tUserIndex].GetReduceManaRatio();
#if 0
		if (mUSER[tUserIndex].mAvatarInfo.aHalfManaTime > 0)
		{
			tUseManaValue = tUseManaValue / 2;
		}
#endif
		if (tReduceManaRatio > 0)
		{
			tUseManaValue -= ((tUseManaValue * tReduceManaRatio) / 100);
		}
		if (mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue < tUseManaValue)
		{
			//mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		break;
	}
	switch (mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aSort)
	{
	case 11:
		return;
	case 12:
		if (tAction.aSort != 0)
		{
			return;
		}
		break;
	}
	if (tUseManaValue > 0)
	{
		mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue -= tUseManaValue;
	}

	if (tAction.aSkillNumber != 0)
	{
		if ((tAction.aSkillNumber != 76) && (tAction.aSkillNumber != 77) && (tAction.aSkillNumber != 78) && (tAction.
			aSkillNumber != 79) && (tAction.aSkillNumber != 80) && (tAction.aSkillNumber != 81))
		{
			if ((tAction.aSkillGradeNum1 > mUSER[tUserIndex].GetMaxSkillGradeNum(tAction.aSkillNumber)) || (tAction.
				aSkillGradeNum2 > mAVATAR_OBJECT[tUserIndex].GetBonusSkillValue(tAction.aSkillNumber)))
			{
				//mGAMELOG.GL_702_CHEAT_AVATAR_ACTION(tUserIndex, tAction.aSkillNumber, tAction.aSkillGradeNum1, tAction.aSkillGradeNum2, tAction.aSkillValue);
				//mPLAYUSER_COM.U_BLOCK_USER_FOR_ZONE_1_SEND(605, &mUSER[tUserIndex].uID[0]);
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
		}
	}

	// # Defense Hack # {
	float fRange = 0.0f;
	fRange = mUTIL.ReturnLengthXYZ(&tAction.aLocation[0], &mAVATAR_OBJECT[tUserIndex].mPRE_LOCATION[0]);

	if ((fRange > 666.0f) && !mAVATAR_OBJECT[tUserIndex].mCheckDeath
		&& (mUSER[tUserIndex].mAuthInfo.AuthType == 0) && (mAVATAR_OBJECT[tUserIndex].mCheckCount < 1)
		&& (!mGAME.mCheckZone049TypeServer) && (!mGAME.mCheckZone051TypeServer) && (!mGAME.mCheckZone053TypeServer))
	{
		//&& ((mUSER[tUserIndex].mAvatarInfo.aAutoStart == 0))) {
		//mGAMELOG.GL_659_AUTO_CHECK(tUserIndex, 4, fRange);
		tAction.aLocation[0] = mAVATAR_OBJECT[tUserIndex].mPRE_LOCATION[0];
		tAction.aLocation[1] = mAVATAR_OBJECT[tUserIndex].mPRE_LOCATION[1];
		tAction.aLocation[2] = mAVATAR_OBJECT[tUserIndex].mPRE_LOCATION[2];
		CopyMemory(&mAVATAR_OBJECT[tUserIndex].mDATA.aAction, &tAction, SIZE_OF_ACTION_INFO);
		mTRANSFER.B_AVATAR_ACTION_RECV(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber,
		                               &mAVATAR_OBJECT[tUserIndex].mDATA, 0);
	}
	else
	{
		CopyMemory(&mAVATAR_OBJECT[tUserIndex].mDATA.aAction, &tAction, SIZE_OF_ACTION_INFO);
		mTRANSFER.B_AVATAR_ACTION_RECV(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber,
		                               &mAVATAR_OBJECT[tUserIndex].mDATA, 1);
	}
	mAVATAR_OBJECT[tUserIndex].mCheckCount--;
	mAVATAR_OBJECT[tUserIndex].SetSpaceIndex();

	// # }
	CopyMemory(&mAVATAR_OBJECT[tUserIndex].mDATA.aAction, &tAction, sizeof( ACTION_INFO ));
	mAVATAR_OBJECT[tUserIndex].SetSpaceIndex();
	mTRANSFER.B_AVATAR_ACTION_RECV(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber,
	                               &mAVATAR_OBJECT[tUserIndex].mDATA, 1);
	if (mUSER[tUserIndex].mAvatarInfo.aVisibleState == 0)
	{
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	}
	else
	{
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, tUserIndex,
		                false);
	}
	//LOG_TO_FILE_2("=>4 %d %d\n", mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aSort, tAction.aSort);
	if (tAction.aSort != 0)
	{
		return;
	}
	mUSER[tUserIndex].mTickCountFor01SecondForProtect = mGAME.mTickCount;
	mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue = (mAVATAR_OBJECT[tUserIndex].GetMaxLife() / 3) + 1;
	//LOG_TO_FILE_2("===> %d %d\n", mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue, ( mAVATAR_OBJECT[tUserIndex].GetMaxLife() / 3 ) + 1);
	mTRANSFER.B_AVATAR_CHANGE_INFO_2(10, mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue);
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//CHECK_VALID_CHARACTER_MOTION_FOR_SEND
#ifdef __GOD__
BOOL MyWork::CheckValidCharacterMotionForSend(int tType, int tSort, int tSkillNumber, int* tSkillSort,
                                              int* tCheckMaxAttackPacketNum, int* tAttackPacketSort,
                                              int* tMaxAttackPacketNum)
#else
BOOL MyWork::CheckValidCharacterMotionForSend(int tType, int tSort, int tSkillNumber, int* tSkillSort, int* tCheckMaxAttackPacketNum, int* tMaxAttackPacketNum)
#endif
{
	*tSkillSort = 0;
	*tCheckMaxAttackPacketNum = 1;
	*tMaxAttackPacketNum = 0;
#ifdef __GOD__
	*tAttackPacketSort = 0;
#endif
	switch (tSort)
	{
	case 0:
		switch (tType)
		{
		case 0:
			return TRUE;
		}
		break;
	case 1:
		switch (tType)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
			return TRUE;
		}
		break;
	case 2:
		switch (tType)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
			return TRUE;
		}
		break;
	case 3:
		switch (tType)
		{
		case 0:
		case 2:
		case 4:
		case 6:
			return TRUE;
		}
		break;
	case 4:
		switch (tType)
		{
		case 1:
		case 3:
		case 5:
		case 7:
			return TRUE;
		}
		break;
	case 5:
		*tMaxAttackPacketNum = 1;
#ifdef __GOD__
		*tSkillSort = 0;
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 1;
#endif
		switch (tType)
		{
		case 1:
		case 3:
		case 5:
		case 7:
			return TRUE;
		}
		break;
	case 6:
		*tMaxAttackPacketNum = 2;
#ifdef __GOD__
		*tSkillSort = 0;
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 1;
#endif
		switch (tType)
		{
		case 1:
		case 3:
		case 5:
		case 7:
			return TRUE;
		}
		break;
	case 7:
		*tMaxAttackPacketNum = 3;
#ifdef __GOD__
		*tSkillSort = 0;
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 1;
#endif
		switch (tType)
		{
		case 1:
		case 3:
		case 5:
		case 7:
			return TRUE;
		}
		break;
	case 9:
		switch (tType)
		{
		case 1:
		case 3:
		case 5:
		case 7:
			return TRUE;
		}
		break;
	case 10:
		switch (tType)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
			return TRUE;
		}
		break;
	case 13:
		switch (tType)
		{
		case 0:
			return TRUE;
		}
		break;
	case 14:
		switch (tType)
		{
		case 0:
			return TRUE;
		}
		break;
	case 15:
		switch (tType)
		{
		case 0:
			return TRUE;
		}
		break;
	case 16:
		switch (tType)
		{
		case 0:
			return TRUE;
		}
		break;
	case 17:
		switch (tType)
		{
		case 0:
			return TRUE;
		}
		break;
	case 18:
		switch (tType)
		{
		case 0:
			return TRUE;
		}
		break;
	case 20:
		switch (tType)
		{
		case 0:
			return TRUE;
		}
		break;
	case 21:
		switch (tType)
		{
		case 0:
			return TRUE;
		}
		break;
	case 22:
		switch (tType)
		{
		case 0:
			return TRUE;
		}
		break;
	case 23:
		switch (tType)
		{
		case 0:
			return TRUE;
		}
		break;
	case 30:
		*tSkillSort = 1;
		switch (tType)
		{
		case 0:
			return TRUE;
		}
		break;
	case 32:
		*tSkillSort = 2;
		switch (tType)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
			return TRUE;
		}
		break;
	case 33:
		*tSkillSort = 2;
		switch (tType)
		{
		case 0:
			return TRUE;
		}
		break;
	case 38:
		*tSkillSort = 2;
		*tMaxAttackPacketNum = 1;
#ifdef __GOD__
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 2;
#endif
		switch (tType)
		{
		case 0:
			return TRUE;
		}
		break;
	case 39:
		*tSkillSort = 2;
		*tMaxAttackPacketNum = 1;
#ifdef __GOD__
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 2;
#endif
		switch (tType)
		{
		case 0:
			return TRUE;
		}
		break;
	case 40:
		*tSkillSort = 2;
		switch (tType)
		{
		case 0:
			return TRUE;
		}
		break;
	case 41:
		*tSkillSort = 2;
		switch (tType)
		{
		case 0:
			return TRUE;
		}
		break;
	case 42:
		*tSkillSort = 2;
		*tMaxAttackPacketNum = 1;
#ifdef __GOD__
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 3;
#endif
		switch (tType)
		{
		case 3:
			return TRUE;
		}
		break;
	case 43:
		*tSkillSort = 2;
		*tMaxAttackPacketNum = 3;
#ifdef __GOD__
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 3;
#endif
		switch (tType)
		{
		case 3:
			return TRUE;
		}
		break;
	case 44:
		*tSkillSort = 2;
		*tMaxAttackPacketNum = 5;
#ifdef __GOD__
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 3;
#endif
		switch (tType)
		{
		case 3:
			return TRUE;
		}
		break;
	case 45:
		*tSkillSort = 2;
		*tMaxAttackPacketNum = 1;
#ifdef __GOD__
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 3;
#endif
		switch (tType)
		{
		case 3:
			return TRUE;
		}
		break;
	case 46:
		*tSkillSort = 2;
		*tMaxAttackPacketNum = 3;
#ifdef __GOD__
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 3;
#endif
		switch (tType)
		{
		case 3:
			return TRUE;
		}
		break;
	case 48:
		*tSkillSort = 2;
		*tMaxAttackPacketNum = 1;
#ifdef __GOD__
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 3;
#endif
		switch (tType)
		{
		case 5:
			return TRUE;
		}
		break;
	case 49:
		*tSkillSort = 2;
		*tMaxAttackPacketNum = 3;
#ifdef __GOD__
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 3;
#endif
		switch (tType)
		{
		case 5:
			return TRUE;
		}
		break;
	case 50:
		*tSkillSort = 2;
		*tMaxAttackPacketNum = 5;
#ifdef __GOD__
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 3;
#endif
		switch (tType)
		{
		case 5:
			return TRUE;
		}
		break;
	case 51:
		*tSkillSort = 2;
		*tMaxAttackPacketNum = 1;
#ifdef __GOD__
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 3;
#endif
		switch (tType)
		{
		case 5:
			return TRUE;
		}
		break;
	case 52:
		*tSkillSort = 2;
		*tMaxAttackPacketNum = 3;
#ifdef __GOD__
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 3;
#endif
		switch (tType)
		{
		case 5:
			return TRUE;
		}
		break;
	case 54:
		*tSkillSort = 2;
#ifdef __GOD__
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 4;
		*tMaxAttackPacketNum = 1;
#else
		* tCheckMaxAttackPacketNum = 0;
#endif
		switch (tType)
		{
		case 7:
			return TRUE;
		}
		break;
	case 55:
		*tSkillSort = 2;
#ifdef __GOD__
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 4;
		*tMaxAttackPacketNum = 3;
#else
		* tCheckMaxAttackPacketNum = 0;
#endif
		switch (tType)
		{
		case 7:
			return TRUE;
		}
		break;
	case 56:
		*tSkillSort = 2;
		*tMaxAttackPacketNum = 5;
#ifdef __GOD__
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 3;
#endif
		switch (tType)
		{
		case 7:
			return TRUE;
		}
		break;
	case 57:
		*tSkillSort = 2;
#ifdef __GOD__
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 4;
		*tMaxAttackPacketNum = 1;
#else
		* tCheckMaxAttackPacketNum = 0;
#endif
		switch (tType)
		{
		case 7:
			return TRUE;
		}
		break;
	case 58:
		*tSkillSort = 2;
#ifdef __GOD__
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 4;
		*tMaxAttackPacketNum = 3;
#else
		* tCheckMaxAttackPacketNum = 0;
#endif
		switch (tType)
		{
		case 7:
			return TRUE;
		}
		break;
	case 60:
		*tSkillSort = 2;
		switch (tType)
		{
		case 3:
			return TRUE;
		}
		break;
	case 61:
		*tSkillSort = 2;
		switch (tType)
		{
		case 5:
			return TRUE;
		}
		break;
	case 62:
		*tSkillSort = 2;
		switch (tType)
		{
		case 7:
			return TRUE;
		}
		break;
	case 63:
		*tSkillSort = 2;
		switch (tType)
		{
		case 0:
			return TRUE;
		}
		break;
	case 64:
		switch (tType)
		{
		case 0:
			return TRUE;
		}
		break;
	case 65:
#ifdef __GOD__
		*tSkillSort = 0;
		*tAttackPacketSort = 5;
		*tMaxAttackPacketNum = 1;
#endif
		*tCheckMaxAttackPacketNum = 0;
		switch (tType)
		{
		case 0:
			return TRUE;
		}
		break;
	case 66:
		*tSkillSort = 2;
		switch (tType)
		{
		case 0:
			return TRUE;
		}
		break;
	case 67:
		*tSkillSort = 2;
		switch (tType)
		{
		case 0:
			return TRUE;
		}
		break;
	case 68:
		*tSkillSort = 2;
		switch (tType)
		{
		case 0:
			return TRUE;
		}
		break;
	case 69:
		*tSkillSort = 2;
		*tMaxAttackPacketNum = 1;
#ifdef __GOD__
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 3;
#endif
		switch (tType)
		{
		case 3:
			return TRUE;
		}
		break;
	case 70:
		*tSkillSort = 2;
		*tMaxAttackPacketNum = 3;
#ifdef __GOD__
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 3;
#endif
		switch (tType)
		{
		case 3:
			return TRUE;
		}
		break;
	case 71:
		*tSkillSort = 2;
		*tMaxAttackPacketNum = 1;
#ifdef __GOD__
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 3;
#endif
		switch (tType)
		{
		case 5:
			return TRUE;
		}
		break;
	case 72:
		*tSkillSort = 2;
		*tMaxAttackPacketNum = 3;
#ifdef __GOD__
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 3;
#endif
		switch (tType)
		{
		case 5:
			return TRUE;
		}
		break;
	case 73:
		*tSkillSort = 2;
#ifdef __GOD__
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 4;
		*tMaxAttackPacketNum = 3;
#else
		* tCheckMaxAttackPacketNum = 0;
#endif
		switch (tType)
		{
		case 7:
			return TRUE;
		}
		break;
	case 74:
		*tSkillSort = 2;
		*tCheckMaxAttackPacketNum = 0;
		switch (tType)
		{
		case 7:
			return TRUE;
		}
		break;
	case 75:
		*tSkillSort = 2;
		switch (tType)
		{
		case 0:
			return TRUE;
		}
		break;
	case 76:
		*tSkillSort = 2;
		switch (tType)
		{
		case 0:
			return TRUE;
		}
		break;
	case 81:
		*tSkillSort = 2;
		*tMaxAttackPacketNum = 5;
#ifdef __GOD__
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 3;
#endif
		switch (tType)
		{
		case 3:
			return TRUE;
		}
		break;
	case 82:
		*tSkillSort = 2;
		*tMaxAttackPacketNum = 5;
#ifdef __GOD__
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 3;
#endif
		switch (tType)
		{
		case 5:
			return TRUE;
		}
		break;
	case 83:
		*tSkillSort = 2;
		*tMaxAttackPacketNum = 5;
#ifdef __GOD__
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 3;
#endif
		switch (tType)
		{
		case 7:
			return TRUE;
		}
		break;
#ifdef __GOD__
	case 85:
		*tSkillSort = 2;
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 3;
		*tMaxAttackPacketNum = 1;
		switch (tType)
		{
		case 3:
			return TRUE;
		}
		break;
	case 86:
		*tSkillSort = 2;
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 3;
		*tMaxAttackPacketNum = 3;
		switch (tType)
		{
		case 3:
			return TRUE;
		}
		break;
	case 87:
		*tSkillSort = 2;
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 3;
		*tMaxAttackPacketNum = 1;
		switch (tType)
		{
		case 5:
			return TRUE;
		}
		break;
	case 88:
		*tSkillSort = 2;
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 3;
		*tMaxAttackPacketNum = 3;
		switch (tType)
		{
		case 5:
			return TRUE;
		}
		break;
	case 89:
		*tSkillSort = 2;
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 4;
		*tMaxAttackPacketNum = 1;
		switch (tType)
		{
		case 7:
			return TRUE;
		}
		break;
	case 90:
		*tSkillSort = 2;
		*tCheckMaxAttackPacketNum = 1;
		*tAttackPacketSort = 4;
		*tMaxAttackPacketNum = 3;
		switch (tType)
		{
		case 7:
			return TRUE;
		}
		break;
	case 255:
		*tSkillSort = 3;
		switch (tType)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
			return TRUE;
		}
		break;
#endif
	}
	return FALSE;
}

BOOL MyWork::SkillForMotion(int tUserIndex, ACTION_INFO* pAction, int tCheckMotion)
{
	switch (pAction->aSkillNumber)
	{
#if 0
	case 6:
	case 25:
	case 44:
		return 40;
#endif
	case 7:
	case 26:
	case 45:
		pAction->aType = 0;
		pAction->aSort = 41;
		pAction->aFrame = 0.0f;
		pAction->aTargetLocation[0] = 0.0f;
		pAction->aTargetLocation[1] = 0.0f;
		pAction->aTargetLocation[2] = 0.0f;
		pAction->aTargetObjectSort = 0;
		pAction->aTargetObjectIndex = -1;
		pAction->aTargetObjectUniqueNumber = 0;
		pAction->aSkillValue = 0;
		return true;
	case 11:
	case 30:
	case 49:
		pAction->aType = mAVATAR_OBJECT[tUserIndex].GetWeaponClass() * 2 + 1; // - tCheckMotion;
		if (pAction->aType == 1)
			return false;
		pAction->aSort = 60;
		pAction->aFrame = 0.0f;
		pAction->aTargetLocation[0] = 0.0f;
		pAction->aTargetLocation[1] = 0.0f;
		pAction->aTargetLocation[2] = 0.0f;
		pAction->aTargetObjectSort = 0;
		pAction->aTargetObjectIndex = -1;
		pAction->aTargetObjectUniqueNumber = 0;
		pAction->aSkillValue = 0;
		return true;
	case 15:
	case 34:
	case 53:
		pAction->aType = mAVATAR_OBJECT[tUserIndex].GetWeaponClass() * 2 + 1; // - tCheckMotion;
		if (pAction->aType == 1)
			return false;
		pAction->aSort = 61;
		pAction->aFrame = 0.0f;
		pAction->aTargetLocation[0] = 0.0f;
		pAction->aTargetLocation[1] = 0.0f;
		pAction->aTargetLocation[2] = 0.0f;
		pAction->aTargetObjectSort = 0;
		pAction->aTargetObjectIndex = -1;
		pAction->aTargetObjectUniqueNumber = 0;
		pAction->aSkillValue = 0;
		return true;
	case 19:
	case 38:
	case 57:
		pAction->aType = mAVATAR_OBJECT[tUserIndex].GetWeaponClass() * 2 + 1; // - tCheckMotion;
		if (pAction->aType == 1)
			return false;
		pAction->aSort = 62;
		pAction->aFrame = 0.0f;
		pAction->aTargetLocation[0] = 0.0f;
		pAction->aTargetLocation[1] = 0.0f;
		pAction->aTargetLocation[2] = 0.0f;
		pAction->aTargetObjectSort = 0;
		pAction->aTargetObjectIndex = -1;
		pAction->aTargetObjectUniqueNumber = 0;
		pAction->aSkillValue = 0;
		return true;
	case 82:
		pAction->aType = 0;
		pAction->aSort = 66;
		pAction->aFrame = 0.0f;
		pAction->aTargetLocation[0] = 0.0f;
		pAction->aTargetLocation[1] = 0.0f;
		pAction->aTargetLocation[2] = 0.0f;
		pAction->aTargetObjectSort = 0;
		pAction->aTargetObjectIndex = -1;
		pAction->aTargetObjectUniqueNumber = 0;
		pAction->aSkillValue = 0;
		return true;
	case 83:
		pAction->aType = 0;
		pAction->aSort = 67;
		pAction->aFrame = 0.0f;
		pAction->aTargetLocation[0] = 0.0f;
		pAction->aTargetLocation[1] = 0.0f;
		pAction->aTargetLocation[2] = 0.0f;
		pAction->aTargetObjectSort = 0;
		pAction->aTargetObjectIndex = -1;
		pAction->aTargetObjectUniqueNumber = 0;
		pAction->aSkillValue = 0;
		return true;
	case 84:
		pAction->aType = 0;
		pAction->aSort = 68;
		pAction->aFrame = 0.0f;
		pAction->aTargetLocation[0] = 0.0f;
		pAction->aTargetLocation[1] = 0.0f;
		pAction->aTargetLocation[2] = 0.0f;
		pAction->aTargetObjectSort = 0;
		pAction->aTargetObjectIndex = -1;
		pAction->aTargetObjectUniqueNumber = 0;
		pAction->aSkillValue = 0;
		return true;
	case 103:
	case 104:
	case 105:
		pAction->aType = 0;
		pAction->aSort = 75;
		pAction->aFrame = 0.0f;
		pAction->aTargetLocation[0] = 0.0f;
		pAction->aTargetLocation[1] = 0.0f;
		pAction->aTargetLocation[2] = 0.0f;
		pAction->aTargetObjectSort = 0;
		pAction->aTargetObjectIndex = -1;
		pAction->aTargetObjectUniqueNumber = 0;
		pAction->aSkillValue = 0;
		return true;
	default:
		return false;
	}

	return false;
}

BOOL MyWork::ReturnEffectState(int tUserIndex, int tSkillNumber)
{
	switch (tSkillNumber)
	{
#if 0
	case 6:
	case 25:
	case 44:
		if (mUSER[tUserIndex].mEffectValue[8][0] == 0)
			return true;
		break;
#endif
	case 7:
	case 26:
	case 45:
		if (mUSER[tUserIndex].mEffectValue[4][0] == 0)
			return true;
		break;
	case 11:
	case 34:
	case 49:
		if (mUSER[tUserIndex].mEffectValue[1][0] == 0)
			return true;
		break;
	case 15:
	case 30:
	case 53:
		if (mUSER[tUserIndex].mEffectValue[0][0] == 0)
			return true;
		break;
	case 19:
	case 38:
	case 57:
		if ((mUSER[tUserIndex].mEffectValue[3][0] == 0)
			|| (mUSER[tUserIndex].mEffectValue[7][0] == 0))
			return true;
		break;
	case 82:
		if (mUSER[tUserIndex].mEffectValue[9][0] == 0)
			return true;
		break;
	case 83:
		if (mUSER[tUserIndex].mEffectValue[10][0] == 0)
			return true;
		break;
	case 84:
		if (mUSER[tUserIndex].mEffectValue[11][0] == 0)
			return true;
		break;
	case 103:
		if (mUSER[tUserIndex].mEffectValue[12][0] == 0)
			return true;
		break;
	case 104:
		if (mUSER[tUserIndex].mEffectValue[13][0] == 0)
			return true;
		break;
	case 105:
		if (mUSER[tUserIndex].mEffectValue[14][0] == 0)
			return true;
		break;
	default:
		return false;
	}
	return false;
}

//W_UPDATE_AVATAR_ACTION
void W_UPDATE_AVATAR_ACTION(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	//LOG_TO_FILE_1("111 ---> %d\n", mUSER[tUserIndex].mMoveZoneResult);
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	ACTION_INFO tAction;
	CopyMemory(&tAction, &tPacket[1], SIZE_OF_ACTION_INFO);
	//LOG_TO_FILE_1("11122 ---> %d\n", tAction.aSort);
	switch (tAction.aSort)
	{
	case 1:
		mAVATAR_OBJECT[tUserIndex].ProcessForCreateEffectValue(&tAction);
		break;
	case 2:
		break;
	case 19:
		if (tAction.aType != 0)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		break;
	case 31:
		if (tAction.aType != 0)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		break;
	case 32:
		break;
	case 64:
		if (tAction.aType != 0)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		break;
	default:
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	// # Defense Hack # {
	mAVATAR_OBJECT[tUserIndex].mPRE_LOCATION[0] = mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0];
	mAVATAR_OBJECT[tUserIndex].mPRE_LOCATION[1] = mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[1];
	mAVATAR_OBJECT[tUserIndex].mPRE_LOCATION[2] = mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[2];
	// # }

	if (tAction.aSkillNumber != 0)
	{
		// µ¿Çà¹«°øÀº °Ë»ç¿¡¼­ Á¦¿Ü.
		if ((tAction.aSkillNumber != 76) && (tAction.aSkillNumber != 77) && (tAction.aSkillNumber != 78) && (tAction.
			aSkillNumber != 79) && (tAction.aSkillNumber != 80) && (tAction.aSkillNumber != 81))
		{
			if ((tAction.aSkillGradeNum1 > mUSER[tUserIndex].GetMaxSkillGradeNum(tAction.aSkillNumber)) || (tAction.
				aSkillGradeNum2 > mAVATAR_OBJECT[tUserIndex].GetBonusSkillValue(tAction.aSkillNumber)))
			{
				//mGAMELOG.GL_702_CHEAT_AVATAR_ACTION(tUserIndex, tAction.aSkillNumber, tAction.aSkillGradeNum1, tAction.aSkillGradeNum2, tAction.aSkillValue);
				//mPLAYUSER_COM.U_BLOCK_USER_FOR_ZONE_1_SEND(605, &mUSER[tUserIndex].uID[0]);
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
		}
	}
	CopyMemory(&mAVATAR_OBJECT[tUserIndex].mDATA.aAction, &tAction, SIZE_OF_ACTION_INFO);
	mAVATAR_OBJECT[tUserIndex].SetSpaceIndex();
}

void W_CHANGE_AUTO_INFO(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int tValue01;
	int tValue02;
	CopyMemory(&tValue01, &tPacket[1], 4);
	CopyMemory(&tValue02, &tPacket[5], 4);

	if ((tValue01 < 0) || (tValue01 > 99) || (tValue02 < 0) || (tValue02 > 99))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	mUSER[tUserIndex].mAvatarInfo.aAutoLifeRatio = tValue01;
	mUSER[tUserIndex].mAvatarInfo.aAutoChiRatio = tValue02;
}

//W_DEMAND_ZONE_SERVER_INFO_2
void W_DEMAND_ZONE_SERVER_INFO_2(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int tSort;
	int nCount = 0;
	int tZoneNumber;
	CopyMemory(&tSort, &tPacket[1], 4);
	CopyMemory(&tZoneNumber, &tPacket[5], 4);

	if ((tZoneNumber < MIN_VALID_ZONE_NUMBER) || (tZoneNumber > MAX_VALID_ZONE_NUMBER) || (tZoneNumber == mSERVER_INFO.
		mServerNumber))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	// 이동서 아이템을 사용한 상태에서 다른 방식의 존 이동 패킷이 들어왔을 경우 실패로 처리.
	if (mUSER[tUserIndex].mMoveItemInfo.mZoneNumber > 0)
	{
		if (tSort != 9)
		{
			memset(&mUSER[tUserIndex].mMoveItemInfo, -1, sizeof(MOVE_ITEM_INFO));
			mPLAYUSER_COM.U_SET_MOVE_ITEM_FOR_ZONE_SEND(mUSER[tUserIndex].mPlayUserIndex,
			                                            &mUSER[tUserIndex].mMoveItemInfo);
			mTRANSFER.B_DEMAND_ZONE_SERVER_INFO_2_RESULT(1, &mGAME.mZoneConnectionInfo.mIP[(tZoneNumber - 1)][0],
			                                             mGAME.mZoneConnectionInfo.mPort[(tZoneNumber - 1)]);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			__LogMgr()->Log("!MoveItem not match sort(%d).\n", tSort);
			return;
		}
	}

	switch (tSort)
	{
	case 2: //[GM]::[MOVEZONE]
		if (mUSER[tUserIndex].mAuthInfo.AuthType == 0)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mUSER[tUserIndex].mAuthInfo.MoveZoneFlag == 0)
		{
			return;
		}
	//break;
	case 3: //[DEATH]
		break;
	case 4: //[PORTAL]
		break;
	case 5: //[NPC(MONEY)]
		break;
	case 6: //[NPC(MOVE)]
		break;
	case 7: //[RETURN]
		break;
	case 8: //[RETURN(ITEM)]
		break;
	case 9: //[MOVE(ITEM)] // 이동서
#if 0
		if (mUSER[tUserIndex].mMoveItemInfo.mZoneNumber == tZoneNumber) // In case of movement of zone through movement book item.
		{
			int tResult = mGAME.CheckMoveZoneForMoveZoneItem(mUSER[tUserIndex].mAvatarInfo.aTribe, mUSER[tUserIndex].mAvatarInfo.aLevel1 + mUSER[tUserIndex].mAvatarInfo.aLevel2, mUSER[tUserIndex].mMoveItemInfo.mZoneNumber);
			if (tResult != 0)
			{
				//mGAMELOG.GL_701_CHEAT_USER(tUserIndex, 1);
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
		}
		else
		{
			//mGAMELOG.GL_701_CHEAT_USER(tUserIndex, 1);
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
#endif
		break;
	case 10: //[NPC(지존알현)]
		break;
	case 11: //[AUTO_TO_ZONE_037]
		break;
	case 12: //Zone 84
		break;
	default:
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (1 == mUSER[tUserIndex].mAuthInfo.UZonemoveFlag)
	{
		mTRANSFER.B_DEMAND_ZONE_SERVER_INFO_2_RESULT(1, &mGAME.mZoneConnectionInfo.mIP[(tZoneNumber - 1)][0],
		                                             mGAME.mZoneConnectionInfo.mPort[(tZoneNumber - 1)]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}

	if (!strcmp(mGAME.mZoneConnectionInfo.mIP[(tZoneNumber - 1)], "0.0.0.0"))
	{
		mCENTER_COM.W_ZONE_OTHER_CONNECTION_INFO(tZoneNumber);
		while (true)
		{
			if (strcmp(mGAME.mZoneConnectionInfo.mIP[(tZoneNumber - 1)], "0.0.0.0") != 0)
			{
				goto RESULT;
			}
			if (nCount > 100)
			{
				break;
			}
			nCount++;
			Sleep(10);
		}
		mTRANSFER.B_DEMAND_ZONE_SERVER_INFO_2_RESULT(1, &mGAME.mZoneConnectionInfo.mIP[(tZoneNumber - 1)][0],
		                                             mGAME.mZoneConnectionInfo.mPort[(tZoneNumber - 1)]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
RESULT:
#if 0
	if (mUSER[tUserIndex].m_bIsSend_Question) {
		mTRANSFER.B_DEMAND_ZONE_SERVER_INFO_2_RESULT(2, &mGAME.mZoneConnectionInfo.mIP[(tZoneNumber - 1)][0], mGAME.mZoneConnectionInfo.mPort[(tZoneNumber - 1)]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
#endif
	mUSER[tUserIndex].mAvatarInfo.aLogoutInfo[0] = mSERVER_INFO.mServerNumber;
	mUSER[tUserIndex].mAvatarInfo.aLogoutInfo[1] = static_cast<int>(mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[
		0]);
	mUSER[tUserIndex].mAvatarInfo.aLogoutInfo[2] = static_cast<int>(mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[
		1]);
	mUSER[tUserIndex].mAvatarInfo.aLogoutInfo[3] = static_cast<int>(mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[
		2]);
	mUSER[tUserIndex].mAvatarInfo.aLogoutInfo[4] = mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue;
	mUSER[tUserIndex].mAvatarInfo.aLogoutInfo[5] = mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue;
	mPLAYUSER_COM.U_REGISTER_USER_FOR_ZONE_3_SEND(mUSER[tUserIndex].mPlayUserIndex, &mUSER[tUserIndex].uID[0],
	                                              &mUSER[tUserIndex].mAvatarInfo, mUSER[tUserIndex].mEffectValue,
	                                              &mUSER[tUserIndex].mAuthInfo);
	if (mPLAYUSER_COM.mRecv_Result != 0)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

#if 0
	if (84 == tZoneNumber) {
		if (50 > mUSER[tUserIndex].mAvatarInfo.aLevel1) {
			mTRANSFER.B_DEMAND_ZONE_SERVER_INFO_2_RESULT(3, &mGAME.mZoneConnectionInfo.mIP[(tZoneNumber - 1)][0], mGAME.mZoneConnectionInfo.mPort[(tZoneNumber - 1)]);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		if (0 >= (mUSER[tUserIndex].mAvatarInfo.aZone84Time & 0x0000000F)) {
			mTRANSFER.B_DEMAND_ZONE_SERVER_INFO_2_RESULT(4, &mGAME.mZoneConnectionInfo.mIP[(tZoneNumber - 1)][0], mGAME.mZoneConnectionInfo.mPort[(tZoneNumber - 1)]);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
#if 0
		mPLAYUSER_COM.U_ZONE_84_FOR_ZONE_SEND(mUSER[tUserIndex].mPlayUserIndex, 0);
		if (0 != mPLAYUSER_COM.mRecv_Result) {
			mTRANSFER.B_DEMAND_ZONE_SERVER_INFO_2_RESULT(5, &mGAME.mZoneConnectionInfo.mIP[(tZoneNumber - 1)][0], mGAME.mZoneConnectionInfo.mPort[(tZoneNumber - 1)]);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
#endif
	}
#endif

	mUSER[tUserIndex].mRegisterTime = GetTickCount();
	mUSER[tUserIndex].mMoveZoneResult = 1;
	mTRANSFER.B_DEMAND_ZONE_SERVER_INFO_2_RESULT(0, &mGAME.mZoneConnectionInfo.mIP[(tZoneNumber - 1)][0],
	                                             mGAME.mZoneConnectionInfo.mPort[(tZoneNumber - 1)]);
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);

	mUSER[tUserIndex].m_bUDPActivated = false;
	mUSER[tUserIndex].m_bUDPArrivedAtClient = false;
}

//W_FAIL_MOVE_ZONE_2_SEND
void W_FAIL_MOVE_ZONE_2_SEND(int tUserIndex)
{
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 0)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	mUSER[tUserIndex].mMoveZoneResult = 0;
	mUSER[tUserIndex].mAvatarInfo.aLogoutInfo[0] = mSERVER_INFO.mServerNumber;
	mUSER[tUserIndex].mAvatarInfo.aLogoutInfo[1] = static_cast<int>(mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[
		0]);
	mUSER[tUserIndex].mAvatarInfo.aLogoutInfo[2] = static_cast<int>(mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[
		1]);
	mUSER[tUserIndex].mAvatarInfo.aLogoutInfo[3] = static_cast<int>(mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[
		2]);
	mUSER[tUserIndex].mAvatarInfo.aLogoutInfo[4] = mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue;
	mUSER[tUserIndex].mAvatarInfo.aLogoutInfo[5] = mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue;
	mPLAYUSER_COM.U_REGISTER_USER_FOR_ZONE_2_SEND(mUSER[tUserIndex].mPlayUserIndex, &mUSER[tUserIndex].uID[0],
	                                              &mUSER[tUserIndex].mAvatarInfo, mUSER[tUserIndex].mEffectValue,
	                                              &mUSER[tUserIndex].mAuthInfo);
	if (mPLAYUSER_COM.mRecv_Result != 0)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	mUSER[tUserIndex].mRegisterTime = GetTickCount();

	// 이동서 아이템 사용 후 존 이동 실패 시 처리.
	if (mUSER[tUserIndex].mMoveItemInfo.mZoneNumber > 0)
	{
		memset(&mUSER[tUserIndex].mMoveItemInfo, -1, sizeof(MOVE_ITEM_INFO));
		mPLAYUSER_COM.U_SET_MOVE_ITEM_FOR_ZONE_SEND(mUSER[tUserIndex].mPlayUserIndex, &mUSER[tUserIndex].mMoveItemInfo);
	}
}

//W_USE_HOTKEY_ITEM_SEND
void W_USE_HOTKEY_ITEM_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	/** 2009. 10. 26 : DarkAttack Potion add (wolf) */
	if (mAVATAR_OBJECT[tUserIndex].mDATA.bStateDarkAttack)
	{
		return;
	}
	/** 2009. 10. 26 : DarkAttack Potion add (wolf) */


	int tPage;
	int tIndex;
	CopyMemory(&tPage, &tPacket[1], 4);
	CopyMemory(&tIndex, &tPacket[5], 4);
	if ((tPage < 0) || (tPage > 2) || (tIndex < 0) || (tIndex > (MAX_HOT_KEY_NUM - 1)))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	ITEM_INFO* tITEM_INFO;
	int tGainLifeValue;
	int tGainManaValue;
	int tGainPatActivityValue;
	int tGainMountActivityValue;
	/** 2009. 10. 26 : DarkAttack Potion add (wolf) */
	int tEffectValueState[MAX_AVATAR_EFFECT_SORT_NUM];
	/** 2009. 10. 26 : DarkAttack Potion add (wolf) */

	switch (mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][2])
	{
	case 3:
		if ((mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aSort == 11) || (mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aSort ==
			12) || (mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aSort == 38))
		{
			mTRANSFER.B_USE_HOTKEY_ITEM_RECV(1, tPage, tIndex);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		tITEM_INFO = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][0]);
		if (tITEM_INFO == nullptr)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (tITEM_INFO->iSort != 2)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][1] < 1)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		tGainLifeValue = 0;
		tGainManaValue = 0;
		tGainPatActivityValue = 0;
		switch (tITEM_INFO->iPotionType[0])
		{
		case 1:
			if (!mAVATAR_OBJECT[tUserIndex].mCheckPossibleEatPotion)
			{
				mTRANSFER.B_USE_HOTKEY_ITEM_RECV(1, tPage, tIndex);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				return;
			}
			if (mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue >= mAVATAR_OBJECT[tUserIndex].GetMaxLife())
			{
				mTRANSFER.B_USE_HOTKEY_ITEM_RECV(1, tPage, tIndex);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				return;
			}
			tGainLifeValue = tITEM_INFO->iPotionType[1];
			if ((mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue + tGainLifeValue) > mAVATAR_OBJECT[tUserIndex].
				GetMaxLife())
			{
				tGainLifeValue = mAVATAR_OBJECT[tUserIndex].GetMaxLife() - mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue;
			}
			break;
		case 2:
			if (!mAVATAR_OBJECT[tUserIndex].mCheckPossibleEatPotion)
			{
				mTRANSFER.B_USE_HOTKEY_ITEM_RECV(1, tPage, tIndex);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				return;
			}
			if (mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue >= mAVATAR_OBJECT[tUserIndex].GetMaxLife())
			{
				mTRANSFER.B_USE_HOTKEY_ITEM_RECV(1, tPage, tIndex);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				return;
			}
			tGainLifeValue = static_cast<int>(static_cast<float>(mAVATAR_OBJECT[tUserIndex].GetMaxLife() * tITEM_INFO->
				iPotionType[1]) * 0.01f);
			if ((mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue + tGainLifeValue) > mAVATAR_OBJECT[tUserIndex].
				GetMaxLife())
			{
				tGainLifeValue = mAVATAR_OBJECT[tUserIndex].GetMaxLife() - mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue;
			}
			break;
		case 3:
			if (!mAVATAR_OBJECT[tUserIndex].mCheckPossibleEatPotion)
			{
				mTRANSFER.B_USE_HOTKEY_ITEM_RECV(1, tPage, tIndex);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				return;
			}
			if (mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue >= mAVATAR_OBJECT[tUserIndex].GetMaxMana())
			{
				mTRANSFER.B_USE_HOTKEY_ITEM_RECV(1, tPage, tIndex);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				return;
			}
			tGainManaValue = tITEM_INFO->iPotionType[1];
			if ((mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue + tGainManaValue) > mAVATAR_OBJECT[tUserIndex].
				GetMaxMana())
			{
				tGainManaValue = mAVATAR_OBJECT[tUserIndex].GetMaxMana() - mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue;
			}
			break;
		case 4:
			if (!mAVATAR_OBJECT[tUserIndex].mCheckPossibleEatPotion)
			{
				mTRANSFER.B_USE_HOTKEY_ITEM_RECV(1, tPage, tIndex);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				return;
			}
			if (mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue >= mAVATAR_OBJECT[tUserIndex].GetMaxMana())
			{
				mTRANSFER.B_USE_HOTKEY_ITEM_RECV(1, tPage, tIndex);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				return;
			}
			tGainManaValue = static_cast<int>(static_cast<float>(mAVATAR_OBJECT[tUserIndex].GetMaxMana() * tITEM_INFO->
				iPotionType[1]) * 0.01f);
			if ((mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue + tGainManaValue) > mAVATAR_OBJECT[tUserIndex].
				GetMaxMana())
			{
				tGainManaValue = mAVATAR_OBJECT[tUserIndex].GetMaxMana() - mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue;
			}
			break;
		case 5:
			if (!mAVATAR_OBJECT[tUserIndex].mCheckPossibleEatPotion)
			{
				mTRANSFER.B_USE_HOTKEY_ITEM_RECV(1, tPage, tIndex);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				return;
			}
			if ((mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue >= mAVATAR_OBJECT[tUserIndex].GetMaxLife()) && (
				mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue >= mAVATAR_OBJECT[tUserIndex].GetMaxMana()))
			{
				mTRANSFER.B_USE_HOTKEY_ITEM_RECV(1, tPage, tIndex);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				return;
			}
			tGainLifeValue = static_cast<int>(static_cast<float>(mAVATAR_OBJECT[tUserIndex].GetMaxLife() * tITEM_INFO->
				iPotionType[1]) * 0.01f);
			if ((mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue + tGainLifeValue) > mAVATAR_OBJECT[tUserIndex].
				GetMaxLife())
			{
				tGainLifeValue = mAVATAR_OBJECT[tUserIndex].GetMaxLife() - mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue;
			}
			tGainManaValue = static_cast<int>(static_cast<float>(mAVATAR_OBJECT[tUserIndex].GetMaxMana() * tITEM_INFO->
				iPotionType[1]) * 0.01f);
			if ((mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue + tGainManaValue) > mAVATAR_OBJECT[tUserIndex].
				GetMaxMana())
			{
				tGainManaValue = mAVATAR_OBJECT[tUserIndex].GetMaxMana() - mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue;
			}
			break;
		case 6:
			if (mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] < 1)
			{
				mTRANSFER.B_USE_HOTKEY_ITEM_RECV(1, tPage, tIndex);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				return;
			}
			if (mUSER[tUserIndex].mAvatarInfo.aEquip[8][1] >= MAX_PAT_ACTIVITY_SIZE)
			{
				mTRANSFER.B_USE_HOTKEY_ITEM_RECV(1, tPage, tIndex);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				return;
			}
			tGainPatActivityValue = tITEM_INFO->iPotionType[1];
			if ((mUSER[tUserIndex].mAvatarInfo.aEquip[8][1] + tGainPatActivityValue) >= MAX_PAT_ACTIVITY_SIZE)
			{
				tGainPatActivityValue = MAX_PAT_ACTIVITY_SIZE - mUSER[tUserIndex].mAvatarInfo.aEquip[8][1];
			}
			break;
		case 9:
			break;
		/** 2009. 10. 26 : DarkAttack Potion add (wolf) */
		case 12:
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
			mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][1]--;
			if (mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][1] < 1)
			{
				mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][0] = 0;
				mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][1] = 0;
				mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][2] = 0;
			}
			mTRANSFER.B_USE_HOTKEY_ITEM_RECV(0, tPage, tIndex);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			mUSER[tUserIndex].mEffectValue[15][0] = 3;
			mUSER[tUserIndex].mEffectValue[15][1] = 10;
			mUSER[tUserIndex].iKindDarkAttack = 1;
			mAVATAR_OBJECT[tUserIndex].mDATA.aEffectValueForView[15] = mUSER[tUserIndex].mEffectValue[15][0];
			tEffectValueState[15] = 1;
			mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber,
			                                     mUSER[tUserIndex].mEffectValue, &tEffectValueState[0]);
			mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1);
			return;
		case 13:
			break;
		case 14: //[Attack Rate  Pill]
			mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][1]--;
			if (mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][1] < 1)
			{
				mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][0] = 0;
				mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][1] = 0;
				mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][2] = 0;
			}
			mTRANSFER.B_USE_HOTKEY_ITEM_RECV(0, tPage, tIndex);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			mUSER[tUserIndex].mEffectValue[17][0] = 25;
			mUSER[tUserIndex].mEffectValue[17][1] = 60;
			mAVATAR_OBJECT[tUserIndex].mDATA.aEffectValueForView[17] = mUSER[tUserIndex].mEffectValue[17][0];
			tEffectValueState[17] = 1;
			mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber,
			                                     mUSER[tUserIndex].mEffectValue, &tEffectValueState[0]);
			mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1);
			return;

		case 15: //[Dodge  Pill]
			mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][1]--;
			if (mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][1] < 1)
			{
				mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][0] = 0;
				mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][1] = 0;
				mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][2] = 0;
			}
			mTRANSFER.B_USE_HOTKEY_ITEM_RECV(0, tPage, tIndex);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			mUSER[tUserIndex].mEffectValue[18][0] = 25;
			mUSER[tUserIndex].mEffectValue[18][1] = 60;
			mAVATAR_OBJECT[tUserIndex].mDATA.aEffectValueForView[18] = mUSER[tUserIndex].mEffectValue[18][0];
			tEffectValueState[18] = 1;
			mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber,
			                                     mUSER[tUserIndex].mEffectValue, &tEffectValueState[0]);
			mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1);
			return;

		case 16: //[Mount Feed]
			if (mUSER[tUserIndex].mAvatarInfo.aAnimalIndex < 10)
			{
				mTRANSFER.B_USE_HOTKEY_ITEM_RECV(1, tPage, tIndex);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				return;
			}
			if (mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] >=
				100000000)
			{
				mTRANSFER.B_USE_HOTKEY_ITEM_RECV(1, tPage, tIndex);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				return;
			}
			mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][1]--;
			if (mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][1] < 1)
			{
				mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][0] = 0;
				mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][1] = 0;
				mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][2] = 0;
			}
			mTRANSFER.B_USE_HOTKEY_ITEM_RECV(0, tPage, tIndex);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			tGainMountActivityValue = tITEM_INFO->iPotionType[1] * 1000000;
		/*if ((mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] + tGainMountActivityValue) > 100100000)
		{
			tGainMountActivityValue =  mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] % 1000000 - mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] + 100000000;
		}*/
			mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] +=
				tGainMountActivityValue;
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(
				71, mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10]);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;

		default:
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][1]--;
		if (mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][1] < 1)
		{
			mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][0] = 0;
			mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][1] = 0;
			mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][2] = 0;
		}
		mTRANSFER.B_USE_HOTKEY_ITEM_RECV(0, tPage, tIndex);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		if (tGainLifeValue > 0)
		{
			mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue += tGainLifeValue;
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(10, mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		}
		if (tGainManaValue > 0)
		{
			mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue += tGainManaValue;
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(11, mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		}
		if (tGainPatActivityValue > 0)
		{
			mUSER[tUserIndex].mAvatarInfo.aEquip[8][1] += tGainPatActivityValue;
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(12, mUSER[tUserIndex].mAvatarInfo.aEquip[8][1]);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		}
		return;
	case 13:
		{
			if (mUSER[tUserIndex].iKindDarkAttack == 1)
			{
				mTRANSFER.B_USE_HOTKEY_ITEM_RECV(2, tPage, tIndex);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				//LOG_TO_FILE_2("![%s] : %d \n",__FUNCTION__,__LINE__);
				return;
			}
			mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][1]--;
			if (mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][1] < 1)
			{
				mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][0] = 0;
				mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][1] = 0;
				mUSER[tUserIndex].mAvatarInfo.aHotKey[tPage][tIndex][2] = 0;
			}
			mTRANSFER.B_USE_HOTKEY_ITEM_RECV(4, tPage, tIndex);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);


			mUSER[tUserIndex].mEffectValue[15][0] = 3;
			mUSER[tUserIndex].mEffectValue[15][1] = 30;
			mUSER[tUserIndex].iKindDarkAttack = 2;
			mAVATAR_OBJECT[tUserIndex].mDATA.aEffectValueForView[15] = mUSER[tUserIndex].mEffectValue[15][0];
			tEffectValueState[15] = 1;
			mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber,
			                                     mUSER[tUserIndex].mEffectValue, &tEffectValueState[0]);
			mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1);
		}
		return;

	default:
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
	}
}

//W_USE_INVENTORY_ITEM_SEND
void W_USE_INVENTORY_ITEM_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int tPage;
	int tIndex;
	int tValue;
	int bParam_1 = 0;
	int bParam_2 = 0;

	CopyMemory(&tPage, &tPacket[1], 4);
	CopyMemory(&tIndex, &tPacket[5], 4);
	CopyMemory(&tValue, &tPacket[9], 4);
	CopyMemory(&bParam_1, &tPacket[13], 1);
	CopyMemory(&bParam_2, &tPacket[14], 1);

	// 2009.11.23 : Seongsu Kim : What code to put in for debugging?
	//__LogMgr()->Log("![INFO] tPage = %d  , tIndex = %d , tValue = %d \n",tPage ,tIndex ,tValue  );

	if ((tPage < 0) || (tPage > 1) || (tIndex < 0) || (tIndex > (MAX_INVENTORY_SLOT_NUM - 1)))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int index01;
	int index02;
	ITEM_INFO* tITEM_INFO;
	SKILL_INFO* tSKILL_INFO;
	int tEquipIndex;
	int tEquip[4];
	int tEquipSocket;
	int tItemLLevel;
	int tItemHLevel;
	int tTribeItemList[9];
	int tRandomValue;
	int tItemType;
	int tItemSort;
	int tSearchNum;
	int tMoney;
	int tWarlordSets[22];
	int tSkillList[24];
	int iInvenSlot = 0;
	int tItemListNum = 5;
	int iPage = 0;
	int iInvenIndex = 0;
	int tItemCount = 0;
	int tPageList[5];
	int tInvenSlotList[5];
	int tInvenIndexList[5];
	int tItemList[5][2];
	//int tItemCountList[5];
	int tItemValueList[5];
	int tItemRecognitionList[5];
	int temp_AddTime = 0;
	int tDumyPageList[5];
	int tDumyInvenSlotList[5];
	int tBroadcastInfoSort;
	BYTE tBroadcastInfoData[MAX_BROADCAST_INFO_SIZE];
	time_t tCountSeconds;
	struct tm* tPresentTime;
	char cDate[100];
	unsigned int iDate = 0;
	unsigned int tZone84Date = 0;
	int tProtectIS = 0;
	int tItemNumber = 0; // 사용할 아이템 번호 저장용 변수.

	ZeroMemory(tBroadcastInfoData, sizeof(tBroadcastInfoData));

	if (tPage == 1)
	{
		if (mUSER[tUserIndex].mAvatarInfo.aExpandInventoryDate < mUTIL.ReturnNowDate())
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
	}
	tITEM_INFO = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0]);
	if (tITEM_INFO == nullptr)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	tItemNumber = tITEM_INFO->iIndex;

	switch (tITEM_INFO->iSort)
	{
	case 5: //[무공비급]
		if (tITEM_INFO->iEquipInfo[0] != 1)
		{
			if (mUSER[tUserIndex].mAvatarInfo.aPreviousTribe != (tITEM_INFO->iEquipInfo[0] - 2))
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
		}
#ifdef __GOD__
		if ((mUSER[tUserIndex].mAvatarInfo.aLevel1 + mUSER[tUserIndex].mAvatarInfo.aLevel2) < (tITEM_INFO->iLevelLimit +
			tITEM_INFO->iMartialLevelLimit))
#else
		if (mUSER[tUserIndex].mAvatarInfo.aLevel1 < tITEM_INFO->iLevelLimit)
#endif
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		for (index01 = 0; index01 < MAX_SKILL_SLOT_NUM; index01++)
		{
			if (mUSER[tUserIndex].mAvatarInfo.aSkill[index01][0] == tITEM_INFO->iGainSkillNumber)
			{
				break;
			}
		}
		if (index01 < MAX_SKILL_SLOT_NUM)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		tSKILL_INFO = mSKILL.Search(tITEM_INFO->iGainSkillNumber);
		if (tSKILL_INFO == nullptr)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aSkillPoint < tSKILL_INFO->sLearnSkillPoint)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		switch (tSKILL_INFO->sType)
		{
		case 1:
			for (index01 = 0; index01 < 10; index01++)
			{
				if (mUSER[tUserIndex].mAvatarInfo.aSkill[index01][0] < 1)
				{
					break;
				}
			}
			if (index01 == 10)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		case 2:
			for (index01 = 20; index01 < 30; index01++)
			{
				if (mUSER[tUserIndex].mAvatarInfo.aSkill[index01][0] < 1)
				{
					break;
				}
			}
			if (index01 == 30)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		case 3:
			for (index01 = 10; index01 < 20; index01++)
			{
				if (mUSER[tUserIndex].mAvatarInfo.aSkill[index01][0] < 1)
				{
					break;
				}
			}
			if (index01 == 20)
			{
				for (index01 = 30; index01 < 40; index01++)
				{
					if (mUSER[tUserIndex].mAvatarInfo.aSkill[index01][0] < 1)
					{
						break;
					}
				}
				if (index01 == 40)
				{
					mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
					return;
				}
			}
			break;
		case 4:
			for (index01 = 10; index01 < 20; index01++)
			{
				if (mUSER[tUserIndex].mAvatarInfo.aSkill[index01][0] < 1)
				{
					break;
				}
			}
			if (index01 == 20)
			{
				for (index01 = 30; index01 < 40; index01++)
				{
					if (mUSER[tUserIndex].mAvatarInfo.aSkill[index01][0] < 1)
					{
						break;
					}
				}
				if (index01 == 40)
				{
					mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
					return;
				}
			}
			break;
		default:
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aSkillPoint -= tSKILL_INFO->sLearnSkillPoint;
		mUSER[tUserIndex].mAvatarInfo.aSkill[index01][0] = tSKILL_INFO->sIndex;
		mUSER[tUserIndex].mAvatarInfo.aSkill[index01][1] = tSKILL_INFO->sLearnSkillPoint;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
	//mGAMELOG.GL_606_USE_INVENTORY_ITEM(tUserIndex, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], 0, 0, 0, 0);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	case 6: //[DECO]
	case 7: //[AMULET]
	case 8: //[CAPE]
	case 9: //[COSTUME]
	case 10: //[GLOVES]
	case 11: //[RING]
	case 12: //[BOOTS]
	case 13: //[SWORD]
	case 14: //[Degree]
	case 15: //[imprisonment]
	case 16: //[attitude]
	case 17: //[dipole]
	case 18: //[loquat]
	case 19: //[Park Do]
	case 20: //[pike]
	case 21: //[the goldfish]
	case 22: //[spirits]
	case 28: //
	case 29: //

		if (mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aSort != 1)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (!mUTIL.CheckPossibleEquipItem(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0]
		                                  , mUSER[tUserIndex].mAvatarInfo.aPreviousTribe, -1
#ifdef __GOD__
		                                  , (mUSER[tUserIndex].mAvatarInfo.aLevel1 + mUSER[tUserIndex].mAvatarInfo.
			                                  aLevel2)
#else
			, mUSER[tUserIndex].mAvatarInfo.aLevel1
#endif
		))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		tEquipIndex = tITEM_INFO->iEquipInfo[1] - 2;
		if ((tEquipIndex < 0) || (tEquipIndex > (MAX_EQUIP_SLOT_NUM - 1)))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		tEquip[0] = mUSER[tUserIndex].mAvatarInfo.aEquip[tEquipIndex][0];
		tEquip[1] = mUSER[tUserIndex].mAvatarInfo.aEquip[tEquipIndex][1];
		tEquip[2] = mUSER[tUserIndex].mAvatarInfo.aEquip[tEquipIndex][2];
		tEquip[3] = mUSER[tUserIndex].mAvatarInfo.aEquip[tEquipIndex][3];
		tEquipSocket = mUSER[tUserIndex].mAvatarInfo.aEquipSocket[tEquipIndex];
		mUSER[tUserIndex].mAvatarInfo.aEquip[tEquipIndex][0] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][
			0];
		mUSER[tUserIndex].mAvatarInfo.aEquip[tEquipIndex][1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][
			3];
		mUSER[tUserIndex].mAvatarInfo.aEquip[tEquipIndex][2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][
			4];
		mUSER[tUserIndex].mAvatarInfo.aEquip[tEquipIndex][3] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][
			5];
	// Transfer Socket
		mUSER[tUserIndex].mAvatarInfo.aEquipSocket[tEquipIndex] = mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage][
			tIndex];
	// Transfer Done
		mAVATAR_OBJECT[tUserIndex].mDATA.aEquipForView[tEquipIndex][0] = mUSER[tUserIndex].mAvatarInfo.aEquip[
			tEquipIndex][0];
		mAVATAR_OBJECT[tUserIndex].mDATA.aEquipForView[tEquipIndex][1] = mUSER[tUserIndex].mAvatarInfo.aEquip[
			tEquipIndex][2];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = tEquip[0];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = tEquip[1];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = tEquip[2];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = tEquip[3];
	// Transfer Socket
		mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage][tIndex] = tEquipSocket;
	// Transfer Done
		mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aType = static_cast<unsigned char>(mAVATAR_OBJECT[tUserIndex].
			GetWeaponClass()) * 2;
		mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aSort = 1;
		mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aFrame = 0.0f;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		if (mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue > mAVATAR_OBJECT[tUserIndex].GetMaxLife())
		{
			mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue = mAVATAR_OBJECT[tUserIndex].GetMaxLife();
		}
		if (mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue > mAVATAR_OBJECT[tUserIndex].GetMaxMana())
		{
			mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue = mAVATAR_OBJECT[tUserIndex].GetMaxMana();
		}
		if (tEquipIndex == 7)
		{
			mAVATAR_OBJECT[tUserIndex].ProcessForDeleteEffectValue();
		}
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}

	//ITEM 
	switch (tITEM_INFO->iIndex)
	{
	case 1017: //[Health Elixir]
	case 506:
		if (mUSER[tUserIndex].mAvatarInfo.aEatLifePotion >= 200)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aEatLifePotion++;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
	//mGAMELOG.GL_606_USE_INVENTORY_ITEM(tUserIndex, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], 0, 0, 0, 0);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	case 1018: //[CHI Elixir]
	case 507:
		if (mUSER[tUserIndex].mAvatarInfo.aEatManaPotion >= 200)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aEatManaPotion++;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
	//mGAMELOG.GL_606_USE_INVENTORY_ITEM(tUserIndex, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], 0, 0, 0, 0);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1026: //[Return Scroll]
		if (124 == mSERVER_INFO.mServerNumber)
			return;
		if (mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] < 1)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		}

		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
	//mGAMELOG.GL_606_USE_INVENTORY_ITEM(tUserIndex, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], 0, 0, 0, 0);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3]--;
		if (mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] < 1)
		{
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		}
		mAVATAR_OBJECT[tUserIndex].mCheckDeath = true;
		mAVATAR_OBJECT[tUserIndex].mCheckCount = 9;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

#ifdef __ANIMAL__
	case ANIMAL_BAB:
		for (index01 = 0; index01 < MAX_AVATAR_ANIMAL_NUM; index01++)
		{
			if (mUSER[tUserIndex].mAvatarInfo.aAnimal[index01] != 0)
			{
				break;
			}
		}
		if (index01 == MAX_AVATAR_ANIMAL_NUM)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		tValue = mUSER[tUserIndex].mAvatarInfo.aAnimalTime + 180;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
	//mGAMELOG.GL_605_USE_CASH_ITEM(tUserIndex, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5]);
		mUSER[tUserIndex].mAvatarInfo.aAnimalTime = tValue;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	case ANIMAL_NUM_BAKHO:
	case ANIMAL_NUM_DONWHOANG:
	case ANIMAL_NUM_CHUNLOK:
#if MAX_LIMIT_LEVEL_NUM+MAX_LIMIT_HIGH_LEVEL_NUM > 145
	case ANIMAL_NUM_CHUNGHO:
	case ANIMAL_NUM_TWODON:
	case ANIMAL_NUM_BAIKLOK:
#if MAX_LIMIT_LEVEL_NUM+MAX_LIMIT_HIGH_LEVEL_NUM > 156
	case ANIMAL_NUM_CHUKHO:
	case ANIMAL_NUM_SUNGDON:
	case ANIMAL_NUM_SHINLOK:
	case ANIMAL_NUM_BEAR1:
	case ANIMAL_NUM_BEAR2:
	case ANIMAL_NUM_BEAR3:
	case ANIMAL_NUM_WOLF1:
	case ANIMAL_NUM_WOLF2:
	case ANIMAL_NUM_WOLF3:
	case ANIMAL_NUM_HAECHI1:
	case ANIMAL_NUM_HAECHI2:
	case ANIMAL_NUM_HAECHI3:
	case ANIMAL_NUM_FERRET1:
	case ANIMAL_NUM_FERRET2:
	case ANIMAL_NUM_FERRET3:
	case ANIMAL_NUM_BLACKLEO:
	case ANIMAL_NUM_BLUELEO:
	case ANIMAL_NUM_WHITELEO:
	case ANIMAL_NUM_BULL1:
	case ANIMAL_NUM_BULL2:
	case ANIMAL_NUM_BULL3:
#endif
#endif
	case ANIMAL_NUM_CRISTMAS:
		for (index01 = 0; index01 < MAX_AVATAR_ANIMAL_NUM; index01++)
		{
			if (mUSER[tUserIndex].mAvatarInfo.aAnimal[index01] == tITEM_INFO->iIndex)
			{
				break;
			}
		}
		if (index01 < MAX_AVATAR_ANIMAL_NUM)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		for (index01 = 0; index01 < MAX_AVATAR_ANIMAL_NUM; index01++)
		{
			if (mUSER[tUserIndex].mAvatarInfo.aAnimal[index01] == 0)
			{
				break;
			}
		}
		if (index01 == MAX_AVATAR_ANIMAL_NUM)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		tValue = index01;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
	//mGAMELOG.GL_606_USE_INVENTORY_ITEM(tUserIndex, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], 0, 0, 0, 0);
		mUSER[tUserIndex].mAvatarInfo.aAnimal[tValue] = tITEM_INFO->iIndex;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);

		__LogMgr()->Log("![INFO] Use animal success!. animal_num = %d , ID = %s\n", tITEM_INFO->iIndex,
		                mUSER[tUserIndex].uID);
		return;
#endif
	case 1109: // TELEPORT-SCROLL
	case 1224: //[Dungeon Teleport Scroll
		if (mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] < 1)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
#if 0
		{
			int tResult = mGAME.CheckMoveZoneForMoveZoneItem(mUSER[tUserIndex].mAvatarInfo.aTribe
				, mUSER[tUserIndex].mAvatarInfo.aLevel1 + mUSER[tUserIndex].mAvatarInfo.aLevel2, tValue);
			if (tResult != 0)
			{
				mTRANSFER.B_USE_INVENTORY_ITEM_RECV(tResult, tPage, tIndex, tValue);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				return;
			}
		}
		mUSER[tUserIndex].mMoveItemInfo.mZoneNumber = tValue;
		mUSER[tUserIndex].mMoveItemInfo.mInvenPage = tPage;
		mUSER[tUserIndex].mMoveItemInfo.mInvenIndex = tIndex;
		mUSER[tUserIndex].mMoveItemInfo.mInvenX = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1];
		mUSER[tUserIndex].mMoveItemInfo.mInvenY = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2];
		mPLAYUSER_COM.U_SET_MOVE_ITEM_FOR_ZONE_SEND(mUSER[tUserIndex].mPlayUserIndex, &mUSER[tUserIndex].mMoveItemInfo);
#endif

		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3]--;
		if (mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] < 1)
		{
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		}
		mAVATAR_OBJECT[tUserIndex].mCheckDeath = true;
		mAVATAR_OBJECT[tUserIndex].mCheckCount = 9;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	//COSTUME
	case 301:
	case 302:
	case 303:
	case 304:
	case 305:
	case 306:
	case 307:
	case 308:
	case 309:
	case 310:
	case 311:
	case 312:
	case 313:
	case 314:
	case 315:
	case 316:
	case 317:
	case 318:
	case 319:
	case 320:
	case 321:
	case 322:
	case 323:
	case 324:
	case 325:
	case 326:
	case 327:
	case 328:
	case 329:
	case 330:
	case 331:
	case 332:
	case 333:
	case 334:
	case 335:
	case 336:
	case 337:
	case 338:
	case 339:
	case 340:
	case 341:
	case 342:
	case 343:
	case 344:
	case 345:
	case 346:
	case 347:
	case 348:
	case 349:
	case 350:
	case 351:
	case 352:
	case 353:
	case 354:
	case 355:
	case 356:
	case 357:
	case 358:
	case 359:
	case 360:
	case 361:
	case 362:
	case 363:
	case 364:
	case 365:
	case 366:
	case 367:
	case 368:
	case 369:
	case 370:
	case 371:
	case 372:
	case 373:
	case 374:
	case 375:
	case 376:
	case 377:
	case 378:
	case 379:
	case 380:
	case 381:
	case 382:
	case 383:
	case 384:
	case 385:
	case 386:
	case 387:
	case 388:
	case 389:
	case 390:
	case 391:
	case 392:
	case 393:
	case 394:
	case 395:
	case 396:
	case 397:
	case 398:
	case 399:
	case 400:
	case 401:
	case 402:
	case 403:
	case 404:
	case 405:
	case 406:
	case 407:
	case 408:
	case 409:
	case 410:
	case 411:
	case 412:
	case 413:
	case 414:
	case 415:
	case 416:
	case 417:
	case 418:
	case 419:
	case 420:
	case 421:
	case 422:
	case 423:
	case 424:
	case 425:
	case 426:
	case 427:
	case 428:
	case 429:
	case 430:
	case 431:
	case 432:
	case 433:
	case 434:
	case 435:
	case 438:
	case 441:

		for (index01 = 0; index01 < 10; index01++)
		{
			if (mUSER[tUserIndex].mAvatarInfo.aCostume[index01] == tITEM_INFO->iIndex)
			{
				break;
			}
		}
		if (index01 < MAX_AVATAR_ANIMAL_NUM)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		for (index01 = 0; index01 < 10; index01++)
		{
			if (mUSER[tUserIndex].mAvatarInfo.aCostume[index01] == 0)
			{
				break;
			}
		}
		if (index01 == MAX_AVATAR_ANIMAL_NUM)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		tValue = index01;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aCostume[tValue] = tITEM_INFO->iIndex;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;


	case 578: //[Elemental Damage Elixir]
		mUSER[tUserIndex].mAvatarInfo.aEatElementAtkDefPotion += 1000;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 579: //[Elemental Defense Elixir]
		mUSER[tUserIndex].mAvatarInfo.aEatElementAtkDefPotion++;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 635: //[Random 15% Mount Box]
		tRandomValue = rand() % 1000;
		if (tRandomValue <= 10)
		{
			tRandomValue = rand() % 18;
			switch (tRandomValue)
			{
			case 0: tValue = 1307;
				break; // 15% Tiger
			case 1: tValue = 1308;
				break; // 15% Boar
			case 2: tValue = 1309;
				break; // 15% Stag
			case 3: tValue = 1315;
				break; // 15% Bear
			case 4: tValue = 1319;
				break; // 15% Ferret
			case 5: tValue = 1322;
				break; // 15% Bull
			case 6: tValue = 1325;
				break; // 15% Wolf
			case 7: tValue = 1328;
				break; // 15% Haechi
			case 8: tValue = 1307;
				break; // 15% Tiger
			case 9: tValue = 1308;
				break; // 15% Boar
			case 10: tValue = 1309;
				break; // 15% Stag
			case 11: tValue = 1315;
				break; // 15% Bear
			case 12: tValue = 1319;
				break; // 15% Ferret
			case 13: tValue = 1322;
				break; // 15% Bull
			case 14: tValue = 1325;
				break; // 15% Wolf
			case 15: tValue = 1328;
				break; // 15% Haechi
			}
		}
		else if (tRandomValue <= 30)
		{
			tRandomValue = rand() % 18;
			switch (tRandomValue)
			{
			case 0: tValue = 1308;
				break; // 15% Boar
			case 1: tValue = 1309;
				break; // 15% Stag
			case 2: tValue = 1315;
				break; // 15% Bear
			case 3: tValue = 1319;
				break; // 15% Ferret
			case 4: tValue = 1325;
				break; // 15% Wolf
			case 5: tValue = 1328;
				break; // 15% Haechi
			case 6: tValue = 1308;
				break; // 15% Boar
			case 7: tValue = 1309;
				break; // 15% Stag
			case 8: tValue = 1315;
				break; // 15% Bear
			case 9: tValue = 1319;
				break; // 15% Ferret
			case 10: tValue = 1325;
				break; // 15% Wolf
			case 11: tValue = 1328;
				break; // 15% Haechi
			}
		}
		else if (tRandomValue <= 70)
		{
			tRandomValue = rand() % 18;
			switch (tRandomValue)
			{
			case 0: tValue = 1308;
				break; // 15% Boar
			case 1: tValue = 1309;
				break; // 15% Stag
			case 2: tValue = 1315;
				break; // 15% Bear
			case 3: tValue = 1319;
				break; // 15% Ferret
			case 4: tValue = 1325;
				break; // 15% Wolf
			case 5: tValue = 1328;
				break; // 15% Haechi
			case 6: tValue = 1308;
				break; // 15% Boar
			case 7: tValue = 1309;
				break; // 15% Stag
			case 8: tValue = 1315;
				break; // 15% Bear
			case 9: tValue = 1319;
				break; // 15% Ferret
			case 10: tValue = 1325;
				break; // 15% Wolf
			case 11: tValue = 1328;
				break; // 15% Haechi
			}
		}
		else
		{
			tRandomValue = rand() % 10;
			switch (tRandomValue)
			{
			case 0: tValue = 1308;
				break; // 15% Boar
			case 1: tValue = 1309;
				break; // 15% Stag
			case 2: tValue = 1319;
				break; // 15% Ferret
			case 3: tValue = 1328;
				break; // 15% Haechi
			case 4: tValue = 1308;
				break; // 15% Boar
			case 5: tValue = 1309;
				break; // 15% Stag
			case 6: tValue = 1319;
				break; // 15% Ferret
			case 7: tValue = 1328;
				break; // 15% Haechi
			}
		}
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = tValue;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 636: //[Health Elixir x10]
		mUSER[tUserIndex].mAvatarInfo.aEatLifePotion += 10;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 637: //[Chi Elixir x10]
		mUSER[tUserIndex].mAvatarInfo.aEatManaPotion += 10;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 638: //[Strength Elixir x10]
		mUSER[tUserIndex].mAvatarInfo.aEatStrengthPotion += 10;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 639: //[Agility Elixir x10]
		mUSER[tUserIndex].mAvatarInfo.aEatAgilityPotion += 10;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 640: //[E Dmg Elixir x10]
		mUSER[tUserIndex].mAvatarInfo.aEatElementAtkDefPotion += 10000;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 641: //[E Def Elixir x10]
		mUSER[tUserIndex].mAvatarInfo.aEatElementAtkDefPotion += 10;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 651: //[Max Elixir Set All Elixir to 200]
		mUSER[tUserIndex].mAvatarInfo.aEatAgilityPotion = 200;
		mUSER[tUserIndex].mAvatarInfo.aEatStrengthPotion = 200;
		mUSER[tUserIndex].mAvatarInfo.aEatLifePotion = 200;
		mUSER[tUserIndex].mAvatarInfo.aEatManaPotion = 200;
		mUSER[tUserIndex].mAvatarInfo.aEatElementAtkDefPotion = 200200;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 691: //[Water Crystal]
		mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 5;
		mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 5;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 692: //[CP Ticket (10)]
		mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 10;
		mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 10;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 693: //[CP Ticket (15)]
		mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 15;
		mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 15;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 694: //[CP Ticket (20)]
		mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 20;
		mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 20;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 706: //Faction Notice Scroll
		mUSER[tUserIndex].mAvatarInfo.aFactionNoticeScroll++;
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(69, mUSER[tUserIndex].mAvatarInfo.aFactionNoticeScroll);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 801: //[Refined Elixir HP]
		if (mUSER[tUserIndex].mAvatarInfo.aEatLifePotion < 200)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aEatLifePotion > 300)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aEatLifePotion++;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 802: //[Refined Chi HP]
		if (mUSER[tUserIndex].mAvatarInfo.aEatManaPotion < 200)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aEatManaPotion > 300)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aEatManaPotion++;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 803: //[Refined Str HP]
		if (mUSER[tUserIndex].mAvatarInfo.aEatStrengthPotion < 200)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aEatStrengthPotion > 300)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aEatStrengthPotion++;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 804: //[Refined Dex HP]
		if (mUSER[tUserIndex].mAvatarInfo.aEatAgilityPotion < 200)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aEatAgilityPotion > 300)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aEatAgilityPotion++;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 805: //[Refined E Dmg Elixir]
		if ((mUSER[tUserIndex].mAvatarInfo.aEatElementAtkDefPotion / 1000) < 200)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if ((mUSER[tUserIndex].mAvatarInfo.aEatElementAtkDefPotion / 1000) > 300)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aEatElementAtkDefPotion += 1000;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 806: //[Refined E Def Elixir]
		if ((mUSER[tUserIndex].mAvatarInfo.aEatElementAtkDefPotion % 1000) < 200)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if ((mUSER[tUserIndex].mAvatarInfo.aEatElementAtkDefPotion % 1000) > 300)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aEatElementAtkDefPotion++;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1027: //[Book of Amnesia]
		for (index01 = 0; index01 < MAX_SKILL_SLOT_NUM; index01++)
		{
			if (mUSER[tUserIndex].mAvatarInfo.aSkill[index01][0] < 1)
			{
				continue;
			}
			mUSER[tUserIndex].mAvatarInfo.aSkillPoint += mUSER[tUserIndex].mAvatarInfo.aSkill[index01][1];
			mUSER[tUserIndex].mAvatarInfo.aSkill[index01][0] = 0;
			mUSER[tUserIndex].mAvatarInfo.aSkill[index01][1] = 0;
		}
		for (index01 = 0; index01 < 3; index01++)
		{
			for (index02 = 0; index02 < MAX_HOT_KEY_NUM; index02++)
			{
				if (mUSER[tUserIndex].mAvatarInfo.aHotKey[index01][index02][2] != 1)
				{
					continue;
				}
				mUSER[tUserIndex].mAvatarInfo.aHotKey[index01][index02][0] = 0;
				mUSER[tUserIndex].mAvatarInfo.aHotKey[index01][index02][1] = 0;
				mUSER[tUserIndex].mAvatarInfo.aHotKey[index01][index02][2] = 0;
			}
		}
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
	//mGAMELOG.GL_606_USE_INVENTORY_ITEM( tUserIndex, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], 0, 0, 0, 0 );
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1035: //[Lucky Ticket]
#ifdef __GOD__
		if (mUSER[tUserIndex].mAvatarInfo.aLevel2 < 1)
		{
#endif
			tItemLLevel = mUSER[tUserIndex].mAvatarInfo.aLevel1 - 5;
			if (tItemLLevel < 1)
			{
				tItemLLevel = 1;
			}
			tItemHLevel = mUSER[tUserIndex].mAvatarInfo.aLevel1 + 5;
			if (tItemHLevel > MAX_LIMIT_LEVEL_NUM)
			{
				tItemHLevel = MAX_LIMIT_LEVEL_NUM;
			}
#ifdef __GOD__
		}
		else
		{
			tItemLLevel = mUSER[tUserIndex].mAvatarInfo.aLevel1 + mUSER[tUserIndex].mAvatarInfo.aLevel2;
			tItemHLevel = mUSER[tUserIndex].mAvatarInfo.aLevel1 + mUSER[tUserIndex].mAvatarInfo.aLevel2;
		}

#endif
		switch (mUSER[tUserIndex].mAvatarInfo.aPreviousTribe)
		{
		case 0:
			tTribeItemList[0] = 7;
			tTribeItemList[1] = 8;
			tTribeItemList[2] = 9;
			tTribeItemList[3] = 10;
			tTribeItemList[4] = 11;
			tTribeItemList[5] = 12;
			tTribeItemList[6] = 13;
			tTribeItemList[7] = 14;
			tTribeItemList[8] = 15;
			break;
		case 1:
			tTribeItemList[0] = 7;
			tTribeItemList[1] = 8;
			tTribeItemList[2] = 9;
			tTribeItemList[3] = 10;
			tTribeItemList[4] = 11;
			tTribeItemList[5] = 12;
			tTribeItemList[6] = 16;
			tTribeItemList[7] = 17;
			tTribeItemList[8] = 18;
			break;
		case 2:
			tTribeItemList[0] = 7;
			tTribeItemList[1] = 8;
			tTribeItemList[2] = 9;
			tTribeItemList[3] = 10;
			tTribeItemList[4] = 11;
			tTribeItemList[5] = 12;
			tTribeItemList[6] = 19;
			tTribeItemList[7] = 20;
			tTribeItemList[8] = 21;
			break;
		}
		tRandomValue = rand() % 10000;
		if (tRandomValue < 1)
		{
			tItemType = 4;
		}
		else if (tRandomValue < 300)
		{
			tItemType = 3;
		}
		else if (tRandomValue < 9000)
		{
			tItemType = 2;
		}
		else
		{
			tItemType = 1;
		}
		tRandomValue = rand();
		tItemSort = tTribeItemList[tRandomValue % 9];
		tSearchNum = 0;
		while (tSearchNum < 10)
		{
			tRandomValue = rand();
			tITEM_INFO = mITEM.Return((tItemLLevel + (tRandomValue % (tItemHLevel - tItemLLevel + 1))), tItemType,
			                          tItemSort);
			if (tITEM_INFO)
			{
				if (((tITEM_INFO->iEquipInfo[0] == 1) || ((tITEM_INFO->iEquipInfo[0] - 2) == mUSER[tUserIndex].
					mAvatarInfo.aPreviousTribe)) && (tITEM_INFO->iCheckMonsterDrop == 2))
				{
					break;
				}
				tITEM_INFO = nullptr;
				tSearchNum++;
			}
			else
			{
				tSearchNum++;
			}
		}
		if (tITEM_INFO == nullptr)
		{
			mTRANSFER.B_USE_INVENTORY_ITEM_RECV(1, tPage, tIndex, tValue);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		tValue = tITEM_INFO->iIndex;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
	//mGAMELOG.GL_606_USE_INVENTORY_ITEM( tUserIndex, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], tValue, 0, 0, 100000001 );
		if (tITEM_INFO->iType == 4)
		{
			//mGAMELOG.GL_607_GAIN_SIN_ITEM( tUserIndex, 1, tITEM_INFO->iIndex, 0, 0, 100000001 );
		}
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = tValue;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 100000001;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1036: //[Big Lucky Ticket]
#ifdef __GOD__
		if (mUSER[tUserIndex].mAvatarInfo.aLevel2 < 1)
		{
#endif
			tItemLLevel = mUSER[tUserIndex].mAvatarInfo.aLevel1 - 5;
			if (tItemLLevel < 1)
			{
				tItemLLevel = 1;
			}
			tItemHLevel = mUSER[tUserIndex].mAvatarInfo.aLevel1 + 5;
			if (tItemHLevel > MAX_LIMIT_LEVEL_NUM)
			{
				tItemHLevel = MAX_LIMIT_LEVEL_NUM;
			}
#ifdef __GOD__
		}
		else
		{
			tItemLLevel = mUSER[tUserIndex].mAvatarInfo.aLevel1 + mUSER[tUserIndex].mAvatarInfo.aLevel2;
			tItemHLevel = mUSER[tUserIndex].mAvatarInfo.aLevel1 + mUSER[tUserIndex].mAvatarInfo.aLevel2;
		}

#endif

		switch (mUSER[tUserIndex].mAvatarInfo.aPreviousTribe)
		{
		case 0:
			tTribeItemList[0] = 7;
			tTribeItemList[1] = 8;
			tTribeItemList[2] = 9;
			tTribeItemList[3] = 10;
			tTribeItemList[4] = 11;
			tTribeItemList[5] = 12;
			tTribeItemList[6] = 13;
			tTribeItemList[7] = 14;
			tTribeItemList[8] = 15;
			break;
		case 1:
			tTribeItemList[0] = 7;
			tTribeItemList[1] = 8;
			tTribeItemList[2] = 9;
			tTribeItemList[3] = 10;
			tTribeItemList[4] = 11;
			tTribeItemList[5] = 12;
			tTribeItemList[6] = 16;
			tTribeItemList[7] = 17;
			tTribeItemList[8] = 18;
			break;
		case 2:
			tTribeItemList[0] = 7;
			tTribeItemList[1] = 8;
			tTribeItemList[2] = 9;
			tTribeItemList[3] = 10;
			tTribeItemList[4] = 11;
			tTribeItemList[5] = 12;
			tTribeItemList[6] = 19;
			tTribeItemList[7] = 20;
			tTribeItemList[8] = 21;
			break;
		}
		tRandomValue = rand() % 10000;
		if (tRandomValue < 2)
		{
			tItemType = 4;
		}
		else if (tRandomValue < 400)
		{
			tItemType = 3;
		}
		else if (tRandomValue < 9000)
		{
			tItemType = 2;
		}
		else
		{
			tItemType = 1;
		}
		tRandomValue = rand();
		tItemSort = tTribeItemList[(tRandomValue % 9)];
		tSearchNum = 0;
		while (tSearchNum < 10)
		{
			tRandomValue = rand();
			tITEM_INFO = mITEM.Return((tItemLLevel + (tRandomValue % (tItemHLevel - tItemLLevel + 1))), tItemType,
			                          tItemSort);
			if (tITEM_INFO)
			{
				if (((tITEM_INFO->iEquipInfo[0] == 1) || ((tITEM_INFO->iEquipInfo[0] - 2) == mUSER[tUserIndex].
					mAvatarInfo.aPreviousTribe)) && (tITEM_INFO->iCheckMonsterDrop == 2))
				{
					break;
				}
				tITEM_INFO = nullptr;
				tSearchNum++;
			}
			else
			{
				tSearchNum++;
			}
		}
		if (tITEM_INFO == nullptr)
		{
			mTRANSFER.B_USE_INVENTORY_ITEM_RECV(1, tPage, tIndex, tValue);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		tValue = tITEM_INFO->iIndex;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
	//mGAMELOG.GL_606_USE_INVENTORY_ITEM( tUserIndex, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], tValue, 0, 0, 100000002 );
		if (tITEM_INFO->iType == 4)
		{
			//mGAMELOG.GL_607_GAIN_SIN_ITEM( tUserIndex, 1, tITEM_INFO->iIndex, 0, 0, 100000002 );
		}
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = tValue;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 100000002;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1037: //[God Lucky Ticket]
#ifdef __GOD__
		if (mUSER[tUserIndex].mAvatarInfo.aLevel2 < 1)
		{
#endif
			tItemLLevel = mUSER[tUserIndex].mAvatarInfo.aLevel1 - 5;
			if (tItemLLevel < 1)
			{
				tItemLLevel = 1;
			}
			tItemHLevel = mUSER[tUserIndex].mAvatarInfo.aLevel1 + 5;
			if (tItemHLevel > MAX_LIMIT_LEVEL_NUM)
			{
				tItemHLevel = MAX_LIMIT_LEVEL_NUM;
			}
#ifdef __GOD__
		}
		else
		{
			tItemLLevel = mUSER[tUserIndex].mAvatarInfo.aLevel1 + mUSER[tUserIndex].mAvatarInfo.aLevel2;
			tItemHLevel = mUSER[tUserIndex].mAvatarInfo.aLevel1 + mUSER[tUserIndex].mAvatarInfo.aLevel2;
		}

#endif
		switch (mUSER[tUserIndex].mAvatarInfo.aPreviousTribe)
		{
		case 0:
			tTribeItemList[0] = 7;
			tTribeItemList[1] = 8;
			tTribeItemList[2] = 9;
			tTribeItemList[3] = 10;
			tTribeItemList[4] = 11;
			tTribeItemList[5] = 12;
			tTribeItemList[6] = 13;
			tTribeItemList[7] = 14;
			tTribeItemList[8] = 15;
			break;
		case 1:
			tTribeItemList[0] = 7;
			tTribeItemList[1] = 8;
			tTribeItemList[2] = 9;
			tTribeItemList[3] = 10;
			tTribeItemList[4] = 11;
			tTribeItemList[5] = 12;
			tTribeItemList[6] = 16;
			tTribeItemList[7] = 17;
			tTribeItemList[8] = 18;
			break;
		case 2:
			tTribeItemList[0] = 7;
			tTribeItemList[1] = 8;
			tTribeItemList[2] = 9;
			tTribeItemList[3] = 10;
			tTribeItemList[4] = 11;
			tTribeItemList[5] = 12;
			tTribeItemList[6] = 19;
			tTribeItemList[7] = 20;
			tTribeItemList[8] = 21;
			break;
		}
		tRandomValue = rand() % 10000;
		if (tRandomValue < 3)
		{
			tItemType = 4;
		}
		else if (tRandomValue < 500)
		{
			tItemType = 3;
		}
		else if (tRandomValue < 9000)
		{
			tItemType = 2;
		}
		else
		{
			tItemType = 1;
		}
		tRandomValue = rand();
		tItemSort = tTribeItemList[(tRandomValue % 9)];
		tSearchNum = 0;
		while (tSearchNum < 10)
		{
			tRandomValue = rand();
			tITEM_INFO = mITEM.Return((tItemLLevel + (tRandomValue % (tItemHLevel - tItemLLevel + 1))), tItemType,
			                          tItemSort);
			if (tITEM_INFO)
			{
				if (((tITEM_INFO->iEquipInfo[0] == 1) || ((tITEM_INFO->iEquipInfo[0] - 2) == mUSER[tUserIndex].
					mAvatarInfo.aPreviousTribe)) && (tITEM_INFO->iCheckMonsterDrop == 2))
				{
					break;
				}
				tITEM_INFO = nullptr;
				tSearchNum++;
			}
			else
			{
				tSearchNum++;
			}
		}
		if (tITEM_INFO == nullptr)
		{
			mTRANSFER.B_USE_INVENTORY_ITEM_RECV(1, tPage, tIndex, tValue);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		tValue = tITEM_INFO->iIndex;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
	//mGAMELOG.GL_606_USE_INVENTORY_ITEM( tUserIndex, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], tValue, 0, 0, 100000003 );
		if (tITEM_INFO->iType == 4)
		{
			//mGAMELOG.GL_607_GAIN_SIN_ITEM( tUserIndex, 1, tITEM_INFO->iIndex, 0, 0, 100000003 );
		}
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = tValue;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 100000003;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1041: //[Superior Pill]
		mUSER[tUserIndex].mAvatarInfo.aDoubleExpTime2 += 180;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1045: //[Fortune Pouch]
		tMoney = (1000 + rand() % 4001) * 100;
		if (mUTIL.CheckOverMaximum(mUSER[tUserIndex].mAvatarInfo.aMoney, tMoney))
		{
			mTRANSFER.B_USE_INVENTORY_ITEM_RECV(1, tPage, tIndex, tValue);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aMoney += tMoney;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
	//mGAMELOG.GL_606_USE_INVENTORY_ITEM( tUserIndex, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], 1, tMoney, 0, 0 );
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, mUSER[tUserIndex].mAvatarInfo.aMoney);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1047: //[Solemn Grounds Ticket]
		if (mUSER[tUserIndex].mAvatarInfo.aLevel1 < 113)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aZone101TypeTime += 180;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, mUSER[tUserIndex].mAvatarInfo.aMoney);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1048: //[Labyrinth Key]
		mUSER[tUserIndex].mAvatarInfo.aNineTowerKeyNum++;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, mUSER[tUserIndex].mAvatarInfo.aMoney);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1049: //[Taiyan Key]
		mUSER[tUserIndex].mAvatarInfo.aZone125TypeTime += 180;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, mUSER[tUserIndex].mAvatarInfo.aMoney);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1066: //[Book of Clear Fog]
		if ((tValue < 0) || (tValue > (MAX_SKILL_SLOT_NUM - 1)))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aSkill[tValue][0] < 1)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		for (index01 = 0; index01 < 3; index01++)
		{
			for (index02 = 0; index02 < MAX_HOT_KEY_NUM; index02++)
			{
				if (mUSER[tUserIndex].mAvatarInfo.aHotKey[index01][index02][2] != 1)
				{
					continue;
				}
				if (mUSER[tUserIndex].mAvatarInfo.aHotKey[index01][index02][0] == mUSER[tUserIndex].mAvatarInfo.aSkill[
					tValue][0])
				{
					mUSER[tUserIndex].mAvatarInfo.aHotKey[index01][index02][0] = 0;
					mUSER[tUserIndex].mAvatarInfo.aHotKey[index01][index02][1] = 0;
					mUSER[tUserIndex].mAvatarInfo.aHotKey[index01][index02][2] = 0;
				}
			}
		}
		mUSER[tUserIndex].mAvatarInfo.aSkillPoint += mUSER[tUserIndex].mAvatarInfo.aSkill[tValue][1];
		mUSER[tUserIndex].mAvatarInfo.aSkill[tValue][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aSkill[tValue][1] = 0;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
	//mGAMELOG.GL_606_USE_INVENTORY_ITEM( tUserIndex, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], 0, 0, 0, 0 );
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1075: //[Guanyin Catapult]
		if ((mGAME.ReturnTribeRole(&mUSER[tUserIndex].mAvatarInfo.aName[0], mUSER[tUserIndex].mAvatarInfo.aTribe) == 0)
			|| (mUSER[tUserIndex].mAvatarInfo.aTribe != 0))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if ((mSERVER_INFO.mServerNumber != 2) && (mSERVER_INFO.mServerNumber != 3) && (mSERVER_INFO.mServerNumber != 4)
			&& (mSERVER_INFO.mServerNumber != 7) && (mSERVER_INFO.mServerNumber != 8) && (mSERVER_INFO.mServerNumber !=
				9) && (mSERVER_INFO.mServerNumber != 12) && (mSERVER_INFO.mServerNumber != 13) && (mSERVER_INFO.
				mServerNumber != 14) && (mSERVER_INFO.mServerNumber != 141) && (mSERVER_INFO.mServerNumber != 142) && (
				mSERVER_INFO.mServerNumber != 143))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (!mSUMMON.SummonMonsterForTribeWeapon(543, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0]))
		{
			mTRANSFER.B_USE_INVENTORY_ITEM_RECV(1, tPage, tIndex, tValue);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
	//mGAMELOG.GL_606_USE_INVENTORY_ITEM( tUserIndex, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], 0, 0, 0, 0 );
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1076: //[Fujin Catapult]
		if ((mGAME.ReturnTribeRole(&mUSER[tUserIndex].mAvatarInfo.aName[0], mUSER[tUserIndex].mAvatarInfo.aTribe) == 0)
			|| (mUSER[tUserIndex].mAvatarInfo.aTribe != 1))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if ((mSERVER_INFO.mServerNumber != 2) && (mSERVER_INFO.mServerNumber != 3) && (mSERVER_INFO.mServerNumber != 4)
			&& (mSERVER_INFO.mServerNumber != 7) && (mSERVER_INFO.mServerNumber != 8) && (mSERVER_INFO.mServerNumber !=
				9) && (mSERVER_INFO.mServerNumber != 12) && (mSERVER_INFO.mServerNumber != 13) && (mSERVER_INFO.
				mServerNumber != 14) && (mSERVER_INFO.mServerNumber != 141) && (mSERVER_INFO.mServerNumber != 142) && (
				mSERVER_INFO.mServerNumber != 143))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (!mSUMMON.SummonMonsterForTribeWeapon(544, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0]))
		{
			mTRANSFER.B_USE_INVENTORY_ITEM_RECV(1, tPage, tIndex, tValue);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
	//mGAMELOG.GL_606_USE_INVENTORY_ITEM( tUserIndex, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], 0, 0, 0, 0 );
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1077: //[Jinong Catapult]
		if ((mGAME.ReturnTribeRole(&mUSER[tUserIndex].mAvatarInfo.aName[0], mUSER[tUserIndex].mAvatarInfo.aTribe) == 0)
			|| (mUSER[tUserIndex].mAvatarInfo.aTribe != 2))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if ((mSERVER_INFO.mServerNumber != 2) && (mSERVER_INFO.mServerNumber != 3) && (mSERVER_INFO.mServerNumber != 4)
			&& (mSERVER_INFO.mServerNumber != 7) && (mSERVER_INFO.mServerNumber != 8) && (mSERVER_INFO.mServerNumber !=
				9) && (mSERVER_INFO.mServerNumber != 12) && (mSERVER_INFO.mServerNumber != 13) && (mSERVER_INFO.
				mServerNumber != 14) && (mSERVER_INFO.mServerNumber != 141) && (mSERVER_INFO.mServerNumber != 142) && (
				mSERVER_INFO.mServerNumber != 143))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (!mSUMMON.SummonMonsterForTribeWeapon(545, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0]))
		{
			mTRANSFER.B_USE_INVENTORY_ITEM_RECV(1, tPage, tIndex, tValue);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
	//mGAMELOG.GL_606_USE_INVENTORY_ITEM( tUserIndex, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], 0, 0, 0, 0 );
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1078: //[Nangin Catapult]
		if ((mGAME.ReturnTribeRole(&mUSER[tUserIndex].mAvatarInfo.aName[0], mUSER[tUserIndex].mAvatarInfo.aTribe) == 0)
			|| (mUSER[tUserIndex].mAvatarInfo.aTribe != 3))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if ((mSERVER_INFO.mServerNumber != 2) && (mSERVER_INFO.mServerNumber != 3) && (mSERVER_INFO.mServerNumber != 4)
			&& (mSERVER_INFO.mServerNumber != 7) && (mSERVER_INFO.mServerNumber != 8) && (mSERVER_INFO.mServerNumber !=
				9) && (mSERVER_INFO.mServerNumber != 12) && (mSERVER_INFO.mServerNumber != 13) && (mSERVER_INFO.
				mServerNumber != 14) && (mSERVER_INFO.mServerNumber != 141) && (mSERVER_INFO.mServerNumber != 142) && (
				mSERVER_INFO.mServerNumber != 143))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (!mSUMMON.SummonMonsterForTribeWeapon(546, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0]))
		{
			mTRANSFER.B_USE_INVENTORY_ITEM_RECV(1, tPage, tIndex, tValue);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
	//mGAMELOG.GL_606_USE_INVENTORY_ITEM( tUserIndex, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], 0, 0, 0, 0 );
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1092: //[Strength Elixir x1]
	case 509:
		if (mUSER[tUserIndex].mAvatarInfo.aEatStrengthPotion >= 200)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aEatStrengthPotion++;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1093: //[Agility Elixir x1]
	case 508:
		if (mUSER[tUserIndex].mAvatarInfo.aEatAgilityPotion >= 200)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aEatAgilityPotion++;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1097: //[Solemn Ground Ticket 2Hours]
		mUSER[tUserIndex].mAvatarInfo.aZone101TypeTime += 120;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1098: //[Solemn Ground Ticket 1 Hour]
		mUSER[tUserIndex].mAvatarInfo.aZone101TypeTime += 60;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1103: //[Protection Charm]
		mUSER[tUserIndex].mAvatarInfo.aProtectForDestroy++;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1185: //[Protection Charm x5]
		mUSER[tUserIndex].mAvatarInfo.aProtectForDestroy += 5;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1358: //[Protection Charm x10]
		mUSER[tUserIndex].mAvatarInfo.aProtectForDestroy += 10;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1104: //[Full Restoration]
		for (index01 = 0; index01 < MAX_SKILL_SLOT_NUM; index01++)
		{
			if (mUSER[tUserIndex].mAvatarInfo.aSkill[index01][0] < 1)
			{
				continue;
			}
			mUSER[tUserIndex].mAvatarInfo.aSkillPoint += (mUSER[tUserIndex].mAvatarInfo.aSkill[index01][1] - 1);
			mUSER[tUserIndex].mAvatarInfo.aSkill[index01][1] = 1;
		}
		for (index01 = 0; index01 < 3; index01++)
		{
			for (index02 = 0; index02 < MAX_HOT_KEY_NUM; index02++)
			{
				if (mUSER[tUserIndex].mAvatarInfo.aHotKey[index01][index02][2] != 1)
				{
					continue;
				}
				mUSER[tUserIndex].mAvatarInfo.aHotKey[index01][index02][0] = 0;
				mUSER[tUserIndex].mAvatarInfo.aHotKey[index01][index02][1] = 0;
				mUSER[tUserIndex].mAvatarInfo.aHotKey[index01][index02][2] = 0;
			}
		}
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
	//mGAMELOG.GL_605_USE_CASH_ITEM( tUserIndex, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] );
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1105: //[Partial Restoration]
		if ((tValue < 0) || (tValue > (MAX_SKILL_SLOT_NUM - 1)))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aSkill[tValue][0] < 1)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		for (index01 = 0; index01 < 3; index01++)
		{
			for (index02 = 0; index02 < MAX_HOT_KEY_NUM; index02++)
			{
				if (mUSER[tUserIndex].mAvatarInfo.aHotKey[index01][index02][2] != 1)
				{
					continue;
				}
				if (mUSER[tUserIndex].mAvatarInfo.aHotKey[index01][index02][0] == mUSER[tUserIndex].mAvatarInfo.aSkill[
					tValue][0])
				{
					mUSER[tUserIndex].mAvatarInfo.aHotKey[index01][index02][0] = 0;
					mUSER[tUserIndex].mAvatarInfo.aHotKey[index01][index02][1] = 0;
					mUSER[tUserIndex].mAvatarInfo.aHotKey[index01][index02][2] = 0;
				}
			}
		}
		mUSER[tUserIndex].mAvatarInfo.aSkillPoint += (mUSER[tUserIndex].mAvatarInfo.aSkill[tValue][1] - 1);
		mUSER[tUserIndex].mAvatarInfo.aSkill[tValue][1] = 1;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1108: //[Protection Scroll]
		mUSER[tUserIndex].mAvatarInfo.aProtectForDeath += 20;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1171: //[Full change scroll]
		if (mUSER[tUserIndex].mAvatarInfo.aGender == 0)
			mUSER[tUserIndex].mAvatarInfo.aGender = 1;
		else
			mUSER[tUserIndex].mAvatarInfo.aGender = 0;
		mUSER[tUserIndex].mAvatarInfo.aHeadType = bParam_1;
		mUSER[tUserIndex].mAvatarInfo.aFaceType = bParam_2;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)mTRANSFER.mOriginal, mTRANSFER.mOriginalSize);

		mAVATAR_OBJECT[tUserIndex].mDATA.aGender = mUSER[tUserIndex].mAvatarInfo.aGender;
		mAVATAR_OBJECT[tUserIndex].mDATA.aHeadType = mUSER[tUserIndex].mAvatarInfo.aHeadType;
		mAVATAR_OBJECT[tUserIndex].mDATA.aFaceType = mUSER[tUserIndex].mAvatarInfo.aFaceType;

		mTRANSFER.B_AVATAR_CHANGE_INFO_1(mAVATAR_OBJECT[tUserIndex].mIndex,
		                                 mAVATAR_OBJECT[tUserIndex].mUniqueNumber,
		                                 90,
		                                 mUSER[tUserIndex].mAvatarInfo.aGender,
		                                 mUSER[tUserIndex].mAvatarInfo.aHeadType,
		                                 mUSER[tUserIndex].mAvatarInfo.aFaceType);
		mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false);
		return;

	case 1118: //[Scroll of Loyalty]
		mUSER[tUserIndex].mAvatarInfo.aDoubleKillNumTime += 30;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1119: //[Scroll of Battle]
		mUSER[tUserIndex].mAvatarInfo.aDoubleKillExpTime += 30;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1120: //[Scroll of the Gods]
		mUSER[tUserIndex].mAvatarInfo.aDoubleKillExpTime += 40;
		mUSER[tUserIndex].mAvatarInfo.aDoubleKillNumTime += 40;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1121: //Experience Potion S
	case 1122: //Experience Potion M
	case 1123: //Experience Potion L
	case 1234: //Experience Potion XL
		if (mUSER[tUserIndex].mAvatarInfo.aLevel1 < 113)
		{
			switch (tITEM_INFO->iIndex)
			{
			case 1121: tMoney = 60;
				break;
			case 1122: tMoney = 120;
				break;
			case 1123: tMoney = 180;
				break;
			case 1234: tMoney = 240;
				break;
			}

			int iCurrentTime = mUSER[tUserIndex].mAvatarInfo.aDoubleExpTime1 & 0xFFFF;
			int iPart2Time = mUSER[tUserIndex].mAvatarInfo.aDoubleExpTime1 & 0xFFFF0000;
			if (iCurrentTime + tMoney <= 600)
			{
				tValue = iCurrentTime + tMoney;
				GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
				           mUSER[tUserIndex].mAvatarInfo.aName,
				           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
				           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
				           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
				           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
				mUSER[tUserIndex].mAvatarInfo.aDoubleExpTime1 = iPart2Time + tValue;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
				mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
			}
			else
			{
				mTRANSFER.B_USE_INVENTORY_ITEM_RECV(2, tPage, tIndex, 0);
			}
		}
		else
		{
			mTRANSFER.B_USE_INVENTORY_ITEM_RECV(1, tPage, tIndex, 0);
		}

		mUSER[tUserIndex].Send(TRUE, (char*)mTRANSFER.mOriginal, mTRANSFER.mOriginalSize);
		return;

	case 1124: //[Scroll of Seekers]
		mUSER[tUserIndex].mAvatarInfo.aZone126TypeTime += 180;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1125: //[Luxury Chest]
		if (45 > mUSER[tUserIndex].mAvatarInfo.aLevel1)
		{
			mTRANSFER.B_USE_INVENTORY_ITEM_RECV(1, tPage, tIndex, tValue);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}

		index02 = 0;

		tRandomValue = rand() % 100;
		if (tRandomValue < 94)
		{
			if (55 > mUSER[tUserIndex].mAvatarInfo.aLevel1)
			{
				index02 = 45;
			}
			else if (65 > mUSER[tUserIndex].mAvatarInfo.aLevel1)
			{
				index02 = 55;
			}
			else if (75 > mUSER[tUserIndex].mAvatarInfo.aLevel1)
			{
				index02 = 65;
			}
			else if (85 > mUSER[tUserIndex].mAvatarInfo.aLevel1)
			{
				index02 = 75;
			}
			else if (95 > mUSER[tUserIndex].mAvatarInfo.aLevel1)
			{
				index02 = 85;
			}
			else if (105 > mUSER[tUserIndex].mAvatarInfo.aLevel1)
			{
				index02 = 95;
			}
			else if (114 > mUSER[tUserIndex].mAvatarInfo.aLevel1)
			{
				index02 = 105;
			}
			else if (117 > mUSER[tUserIndex].mAvatarInfo.aLevel1)
			{
				index02 = 114;
			}
			else if (120 > mUSER[tUserIndex].mAvatarInfo.aLevel1)
			{
				index02 = 117;
			}
			else if (123 > mUSER[tUserIndex].mAvatarInfo.aLevel1)
			{
				index02 = 120;
			}
			else if (126 > mUSER[tUserIndex].mAvatarInfo.aLevel1)
			{
				index02 = 123;
			}
			else if (129 > mUSER[tUserIndex].mAvatarInfo.aLevel1)
			{
				index02 = 126;
			}
			else if (132 > mUSER[tUserIndex].mAvatarInfo.aLevel1)
			{
				index02 = 129;
			}
			else if (135 > mUSER[tUserIndex].mAvatarInfo.aLevel1)
			{
				index02 = 132;
			}
			else if (141 > mUSER[tUserIndex].mAvatarInfo.aLevel1)
			{
				index02 = 135;
			}
			else if (144 > mUSER[tUserIndex].mAvatarInfo.aLevel1)
			{
				index02 = 141;
			}
			else
			{
				index02 = 145;
			}
			ITEM_INFO* pItemPtr = nullptr;

			if (3 != mUSER[tUserIndex].mAvatarInfo.aTribe)
			{
				pItemPtr = mITEM.ReturnRareItem(mUSER[tUserIndex].mAvatarInfo.aTribe, index02);
			}
			else
			{
				pItemPtr = mITEM.ReturnRareItem(mUSER[tUserIndex].mAvatarInfo.aPreviousTribe, index02);
			}
			if (pItemPtr != nullptr)
			{
				index01 = mUTIL.ReturnItemRecognitionNumber(pItemPtr->iType);
				//mGAMELOG.GL_606_USE_INVENTORY_ITEM( tUserIndex, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], pItemPtr->iIndex, 0, 0, index01);
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = pItemPtr->iIndex;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = index01;
				mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, pItemPtr->iIndex);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			}
			else
			{
				mTRANSFER.B_USE_INVENTORY_ITEM_RECV(2, tPage, tIndex, tValue);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			}
			return;
		}
		if (tRandomValue < 97)
		{
			tValue = 1017;
		}
		else
		{
			tValue = 1018;
		}
	//mGAMELOG.GL_606_USE_INVENTORY_ITEM( tUserIndex, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], tValue, 0, 0, 0 );
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = tValue;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1102: //[Hermits Chest]
		tValue = mUTIL.ReturnAddDate(mUSER[tUserIndex].mAvatarInfo.aExpandInventoryDate, 30);
		if (tValue == -1)
		{
			mTRANSFER.B_USE_INVENTORY_ITEM_RECV(1, tPage, tIndex, tValue);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aExpandInventoryDate = tValue;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1130: //[Storage Chest]
		tValue = mUTIL.ReturnAddDate(mUSER[tUserIndex].mAvatarInfo.aExpandStoreDate, 30);
		if (tValue == -1)
		{
			mTRANSFER.B_USE_INVENTORY_ITEM_RECV(1, tPage, tIndex, tValue);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aExpandStoreDate = tValue;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1132: //[Double Buff Time]
		mUSER[tUserIndex].mAvatarInfo.aDoubleBuffTime += 60;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1134: //[Stat Reset L1-99]
		if ((mUSER[tUserIndex].mAvatarInfo.aLevel1 < 1) || (mUSER[tUserIndex].mAvatarInfo.aLevel1 > 99))
		{
			mTRANSFER.B_USE_INVENTORY_ITEM_RECV(1, tPage, tIndex, tValue);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
#ifdef __GOD__
		if (mUSER[tUserIndex].mAvatarInfo.aLevel2 > 0)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
#endif
		mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint += (mUSER[tUserIndex].mAvatarInfo.aVit + mUSER[tUserIndex].
			mAvatarInfo.aStr + mUSER[tUserIndex].mAvatarInfo.aInt + mUSER[tUserIndex].mAvatarInfo.aDex - 4);
		mUSER[tUserIndex].mAvatarInfo.aVit = 1;
		mUSER[tUserIndex].mAvatarInfo.aStr = 1;
		mUSER[tUserIndex].mAvatarInfo.aInt = 1;
		mUSER[tUserIndex].mAvatarInfo.aDex = 1;
		mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue = 1;
		mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue = 0;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
	//mGAMELOG.GL_605_USE_CASH_ITEM( tUserIndex, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] );
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1135: //[Stat Reset L100-112]
		if ((mUSER[tUserIndex].mAvatarInfo.aLevel1 < 100) || (mUSER[tUserIndex].mAvatarInfo.aLevel1 > 112))
		{
			mTRANSFER.B_USE_INVENTORY_ITEM_RECV(1, tPage, tIndex, tValue);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint += (mUSER[tUserIndex].mAvatarInfo.aVit + mUSER[tUserIndex].
			mAvatarInfo.aStr + mUSER[tUserIndex].mAvatarInfo.aInt + mUSER[tUserIndex].mAvatarInfo.aDex - 4);
		mUSER[tUserIndex].mAvatarInfo.aVit = 1;
		mUSER[tUserIndex].mAvatarInfo.aStr = 1;
		mUSER[tUserIndex].mAvatarInfo.aInt = 1;
		mUSER[tUserIndex].mAvatarInfo.aDex = 1;
		mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue = 1;
		mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue = 0;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
	//mGAMELOG.GL_605_USE_CASH_ITEM( tUserIndex, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] );
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1136: //[Stat Reset M1-33]
		if ((mUSER[tUserIndex].mAvatarInfo.aLevel1 < 113) || (mUSER[tUserIndex].mAvatarInfo.aLevel1 > 145))
		{
			mTRANSFER.B_USE_INVENTORY_ITEM_RECV(1, tPage, tIndex, tValue);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint += (mUSER[tUserIndex].mAvatarInfo.aVit + mUSER[tUserIndex].
			mAvatarInfo.aStr + mUSER[tUserIndex].mAvatarInfo.aInt + mUSER[tUserIndex].mAvatarInfo.aDex - 4);
		mUSER[tUserIndex].mAvatarInfo.aVit = 1;
		mUSER[tUserIndex].mAvatarInfo.aStr = 1;
		mUSER[tUserIndex].mAvatarInfo.aInt = 1;
		mUSER[tUserIndex].mAvatarInfo.aDex = 1;
		mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue = 1;
		mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue = 0;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
	//mGAMELOG.GL_605_USE_CASH_ITEM( tUserIndex, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] );
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1137: //[Stat Reset One only L1-99]
		if ((mUSER[tUserIndex].mAvatarInfo.aLevel1 < 1) || (mUSER[tUserIndex].mAvatarInfo.aLevel1 > 99))
		{
			mTRANSFER.B_USE_INVENTORY_ITEM_RECV(1, tPage, tIndex, tValue);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		if ((tValue < 1) || (tValue > 4))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		switch (tValue)
		{
		case 1:
			mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint += (mUSER[tUserIndex].mAvatarInfo.aStr - 1);
			mUSER[tUserIndex].mAvatarInfo.aStr = 1;
			break;
		case 2:
			mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint += (mUSER[tUserIndex].mAvatarInfo.aDex - 1);
			mUSER[tUserIndex].mAvatarInfo.aDex = 1;
			break;
		case 3:
			mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint += (mUSER[tUserIndex].mAvatarInfo.aVit - 1);
			mUSER[tUserIndex].mAvatarInfo.aVit = 1;
			break;
		case 4:
			mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint += (mUSER[tUserIndex].mAvatarInfo.aInt - 1);
			mUSER[tUserIndex].mAvatarInfo.aInt = 1;
			break;
		}
		mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue = 1;
		mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue = 0;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
	//mGAMELOG.GL_605_USE_CASH_ITEM( tUserIndex, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] );
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1138: //[Stat Reset One Only L100-112
		if ((mUSER[tUserIndex].mAvatarInfo.aLevel1 < 100) || (mUSER[tUserIndex].mAvatarInfo.aLevel1 > 112))
		{
			mTRANSFER.B_USE_INVENTORY_ITEM_RECV(1, tPage, tIndex, tValue);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
#ifdef __GOD__
		if (mUSER[tUserIndex].mAvatarInfo.aLevel2 > 0)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
#endif
		if ((tValue < 1) || (tValue > 4))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		switch (tValue)
		{
		case 1:
			mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint += (mUSER[tUserIndex].mAvatarInfo.aStr - 1);
			mUSER[tUserIndex].mAvatarInfo.aStr = 1;
			break;
		case 2:
			mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint += (mUSER[tUserIndex].mAvatarInfo.aDex - 1);
			mUSER[tUserIndex].mAvatarInfo.aDex = 1;
			break;
		case 3:
			mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint += (mUSER[tUserIndex].mAvatarInfo.aVit - 1);
			mUSER[tUserIndex].mAvatarInfo.aVit = 1;
			break;
		case 4:
			mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint += (mUSER[tUserIndex].mAvatarInfo.aInt - 1);
			mUSER[tUserIndex].mAvatarInfo.aInt = 1;
			break;
		}
		mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue = 1;
		mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue = 0;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
	//mGAMELOG.GL_605_USE_CASH_ITEM( tUserIndex, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] );
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1139: //[Stat Reset One Only M1-33]
		if ((mUSER[tUserIndex].mAvatarInfo.aLevel1 < 113) || (mUSER[tUserIndex].mAvatarInfo.aLevel1 > 145))
		{
			mTRANSFER.B_USE_INVENTORY_ITEM_RECV(1, tPage, tIndex, tValue);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
#ifdef __GOD__
		if (mUSER[tUserIndex].mAvatarInfo.aLevel2 > 0)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
#endif
		if ((tValue < 1) || (tValue > 4))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		switch (tValue)
		{
		case 1:
			mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint += (mUSER[tUserIndex].mAvatarInfo.aStr - 1);
			mUSER[tUserIndex].mAvatarInfo.aStr = 1;
			break;
		case 2:
			mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint += (mUSER[tUserIndex].mAvatarInfo.aDex - 1);
			mUSER[tUserIndex].mAvatarInfo.aDex = 1;
			break;
		case 3:
			mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint += (mUSER[tUserIndex].mAvatarInfo.aVit - 1);
			mUSER[tUserIndex].mAvatarInfo.aVit = 1;
			break;
		case 4:
			mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint += (mUSER[tUserIndex].mAvatarInfo.aInt - 1);
			mUSER[tUserIndex].mAvatarInfo.aInt = 1;
			break;
		}
		mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue = 1;
		mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue = 0;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
	//mGAMELOG.GL_605_USE_CASH_ITEM( tUserIndex, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] );
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1140: //[Storage Vault 60Days]
		tValue = mUTIL.ReturnAddDate(mUSER[tUserIndex].mAvatarInfo.aExpandStoreDate, 60);
		mUSER[tUserIndex].mAvatarInfo.aExpandStoreDate += tValue;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1141: //[Hermits Chest 60Days]
		tValue = mUTIL.ReturnAddDate(mUSER[tUserIndex].mAvatarInfo.aExpandInventoryDate, 60);
		mUSER[tUserIndex].mAvatarInfo.aExpandStoreDate += tValue;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1142: //[God Reset Stat]
		if (mUSER[tUserIndex].mAvatarInfo.aLevel1 < 145)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aLevel2 < 1)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint += (mUSER[tUserIndex].mAvatarInfo.aVit + mUSER[tUserIndex].
			mAvatarInfo.aStr + mUSER[tUserIndex].mAvatarInfo.aInt + mUSER[tUserIndex].mAvatarInfo.aDex - 4);
		mUSER[tUserIndex].mAvatarInfo.aVit = 1;
		mUSER[tUserIndex].mAvatarInfo.aStr = 1;
		mUSER[tUserIndex].mAvatarInfo.aInt = 1;
		mUSER[tUserIndex].mAvatarInfo.aDex = 1;
		mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue = 1;
		mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue = 0;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
	//mGAMELOG.GL_605_USE_CASH_ITEM( tUserIndex, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] );
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1143: //[God Reset Stats One Only]
		if (mUSER[tUserIndex].mAvatarInfo.aLevel1 < 145)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aLevel2 < 1)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if ((tValue < 1) || (tValue > 4))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		switch (tValue)
		{
		case 1:
			mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint += (mUSER[tUserIndex].mAvatarInfo.aStr - 1);
			mUSER[tUserIndex].mAvatarInfo.aStr = 1;
			break;
		case 2:
			mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint += (mUSER[tUserIndex].mAvatarInfo.aDex - 1);
			mUSER[tUserIndex].mAvatarInfo.aDex = 1;
			break;
		case 3:
			mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint += (mUSER[tUserIndex].mAvatarInfo.aVit - 1);
			mUSER[tUserIndex].mAvatarInfo.aVit = 1;
			break;
		case 4:
			mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint += (mUSER[tUserIndex].mAvatarInfo.aInt - 1);
			mUSER[tUserIndex].mAvatarInfo.aInt = 1;
			break;
		}
		mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue = 1;
		mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue = 0;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
	//mGAMELOG.GL_605_USE_CASH_ITEM( tUserIndex, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] );
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1145: //[SBF]
		mUSER[tUserIndex].mAvatarInfo.aAnimalTime += 180;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(26, mUSER[tUserIndex].mAvatarInfo.aAnimalTime);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1146: //[Lucky Combine L]
		mUSER[tUserIndex].mAvatarInfo.aLuckyCombine += 3;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1147: //[Lucky Combine M]
		mUSER[tUserIndex].mAvatarInfo.aLuckyCombine += 2;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1148: //[Lucky Combine S]
		mUSER[tUserIndex].mAvatarInfo.aLuckyCombine += 1;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1149: //[Lucky Upgrade L]
		mUSER[tUserIndex].mAvatarInfo.aLuckyUpgrade += 3;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1150: //[Lucky Upgrade M]
		mUSER[tUserIndex].mAvatarInfo.aLuckyUpgrade += 2;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1151: //[Lucky Upgrade S]
		mUSER[tUserIndex].mAvatarInfo.aLuckyUpgrade += 1;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1152: //[Lucky Drop L]
		mUSER[tUserIndex].mAvatarInfo.aDoubleDropTime += 180;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1153: //[Lucky Drop M]
		mUSER[tUserIndex].mAvatarInfo.aDoubleDropTime += 120;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1154: //[Lucky Drop S]
		mUSER[tUserIndex].mAvatarInfo.aDoubleDropTime += 60;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1166: //[CP Protection Charm S]
		mUSER[tUserIndex].mAvatarInfo.aCPProtectionCharm++;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1188: //[CP Protection Charm L]
		mUSER[tUserIndex].mAvatarInfo.aCPProtectionCharm += 3;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1190: //[Pet Exp Boost Scroll]
		mUSER[tUserIndex].mAvatarInfo.aPat_Exp_x2_Time += 180;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1222: //[Mount Absorb (L)
		mUSER[tUserIndex].mAvatarInfo.aMountAbsorbTime += 180;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(78, mUSER[tUserIndex].mAvatarInfo.aMountAbsorbTime);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1237: //[Wing PC]
		mUSER[tUserIndex].mAvatarInfo.aWingProtectionScroll++;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	case 1242: //Ivy Hall Key
		return;

	case 1436: //[Exp Pill L]
	case 1458:
		mUSER[tUserIndex].mAvatarInfo.aExpPill += 180;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1438: //[Exp Pill M]
		mUSER[tUserIndex].mAvatarInfo.aExpPill += 120;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1439: //[Exp Pill S]
		mUSER[tUserIndex].mAvatarInfo.aExpPill += 60;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1444: //[CP Chest]
		mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 500;
		mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 500;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1447: //[CP Charm L]
		mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 1000;
		mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 1000;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1448: //[CP Charm M]
		mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 500;
		mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 500;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1449: //[CP Charm S]
		mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 100;
		mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 100;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1499: //[CP Charm XL]
		mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 5000;
		mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 5000;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1191: //[Damage Boost Scroll L]
		mUSER[tUserIndex].mAvatarInfo.aDamageBoostScroll += 180;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(46, mUSER[tUserIndex].mAvatarInfo.aDamageBoostScroll);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1192: //[Damage Boost Scroll M]
		mUSER[tUserIndex].mAvatarInfo.aDamageBoostScroll += 90;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(46, mUSER[tUserIndex].mAvatarInfo.aDamageBoostScroll);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1193: //[Damage Boost Scroll S]
		mUSER[tUserIndex].mAvatarInfo.aDamageBoostScroll += 30;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(46, mUSER[tUserIndex].mAvatarInfo.aDamageBoostScroll);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1194: //[Health Boost Scroll L]
		mUSER[tUserIndex].mAvatarInfo.aHealthBoostScroll += 180;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(47, mUSER[tUserIndex].mAvatarInfo.aHealthBoostScroll);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1195: //[Health Boost Scroll M]
		mUSER[tUserIndex].mAvatarInfo.aHealthBoostScroll += 90;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(47, mUSER[tUserIndex].mAvatarInfo.aHealthBoostScroll);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1196: //[Health Boost Scroll S]
		mUSER[tUserIndex].mAvatarInfo.aHealthBoostScroll += 30;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(47, mUSER[tUserIndex].mAvatarInfo.aHealthBoostScroll);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1197: //[Critical Boost Scroll L]
		mUSER[tUserIndex].mAvatarInfo.aCriticalBoostScroll += 180;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(48, mUSER[tUserIndex].mAvatarInfo.aCriticalBoostScroll);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1198: //[Critical Boost Scroll M]
		mUSER[tUserIndex].mAvatarInfo.aCriticalBoostScroll += 90;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(48, mUSER[tUserIndex].mAvatarInfo.aCriticalBoostScroll);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1199: //[Critical Boost Scroll S]
		mUSER[tUserIndex].mAvatarInfo.aCriticalBoostScroll += 30;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(48, mUSER[tUserIndex].mAvatarInfo.aCriticalBoostScroll);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1434: //[LoD Ticket]
		mUSER[tUserIndex].mAvatarInfo.aLifeOrDeath++;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 593: //[Preserve Charm]
		mUSER[tUserIndex].mAvatarInfo.aPreventRefineBreak++;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1167: //[Gold Plate Scroll]
		mUSER[tUserIndex].mAvatarInfo.iGoldOrnamentRemainsTime += 240;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(40, mUSER[tUserIndex].mAvatarInfo.iGoldOrnamentRemainsTime);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1370: //[Silver Plate Scroll]
		mUSER[tUserIndex].mAvatarInfo.iSilverOrnamentRemainsTime += 180;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(40, mUSER[tUserIndex].mAvatarInfo.iGoldOrnamentRemainsTime);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1371: //[IOB]
		if (mUSER[tUserIndex].mAvatarInfo.aDoubleIslandOfBloodTimeUse > 0)
		{
			mUSER[tUserIndex].mAvatarInfo.aIslandOfBloodTime += 60;
			mUSER[tUserIndex].mAvatarInfo.aDoubleIslandOfBloodTimeUse--;
		}
		else
		{
			mUSER[tUserIndex].mAvatarInfo.aIslandOfBloodTime += 30;
		}
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1372: // [IOS1]
		mUSER[tUserIndex].mAvatarInfo.aIslandOfSoul1Time += 30;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1373: //[IOS2]
		mUSER[tUserIndex].mAvatarInfo.aIslandOfSoul2Time += 30;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1374: //[IOS3]
		mUSER[tUserIndex].mAvatarInfo.aIslandOfSoul3Time += 30;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1375: //[IOS4]
		mUSER[tUserIndex].mAvatarInfo.aIslandOfSoul4Time += 30;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1376: //[IOS5]
		mUSER[tUserIndex].mAvatarInfo.aIslandOfSoul5Time += 30;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1377: //[IOS6]
		mUSER[tUserIndex].mAvatarInfo.aIslandOfSoul6Time += 30;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1201: //[Auto Buff 7D]
		tValue = mUTIL.ReturnAddDate(mUSER[tUserIndex].mAvatarInfo.aAutoBuffScroll, 7);
		mUSER[tUserIndex].mAvatarInfo.aAutoBuffScroll = tValue;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(73, mUSER[tUserIndex].mAvatarInfo.aAutoBuffScroll);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1215: //[Auto Buff 30D]
		tValue = mUTIL.ReturnAddDate(mUSER[tUserIndex].mAvatarInfo.aAutoBuffScroll, 30);
		mUSER[tUserIndex].mAvatarInfo.aAutoBuffScroll = tValue;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(73, mUSER[tUserIndex].mAvatarInfo.aAutoBuffScroll);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1216: //[Auto Buff 1D]
		tValue = mUTIL.ReturnAddDate(mUSER[tUserIndex].mAvatarInfo.aAutoBuffScroll, 1);
		mUSER[tUserIndex].mAvatarInfo.aAutoBuffScroll = tValue;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(73, mUSER[tUserIndex].mAvatarInfo.aAutoBuffScroll);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 574: //[Auto Hunt 5h]
		mUSER[tUserIndex].mAvatarInfo.aAutoHunt5Hour += 300;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(62, mUSER[tUserIndex].mAvatarInfo.aAutoHunt5Hour);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 610: //[Auto Hunt 7D]
		tValue = mUTIL.ReturnAddDate(mUSER[tUserIndex].mAvatarInfo.aAutoHunt7Day, 7);
		mUSER[tUserIndex].mAvatarInfo.aAutoHunt7Day = tValue;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(61, mUSER[tUserIndex].mAvatarInfo.aAutoHunt7Day);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 686: //[Another Auto Hunt 7D]
		tValue = mUTIL.ReturnAddDate(mUSER[tUserIndex].mAvatarInfo.aAutoHunt7Day, 7);
		mUSER[tUserIndex].mAvatarInfo.aAutoHunt7Day = tValue;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(61, mUSER[tUserIndex].mAvatarInfo.aAutoHunt7Day);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 687: //[Auto Hunt 1D]
		tValue = mUTIL.ReturnAddDate(mUSER[tUserIndex].mAvatarInfo.aAutoHunt7Day, 1);
		mUSER[tUserIndex].mAvatarInfo.aAutoHunt7Day = tValue;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(61, mUSER[tUserIndex].mAvatarInfo.aAutoHunt7Day);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1217: //[Auto Hunt 30D]
		tValue = mUTIL.ReturnAddDate(mUSER[tUserIndex].mAvatarInfo.aAutoHunt7Day, 30);
		mUSER[tUserIndex].mAvatarInfo.aAutoHunt7Day = tValue;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(61, mUSER[tUserIndex].mAvatarInfo.aAutoHunt7Day);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1187: //[SOS 15H]
		mUSER[tUserIndex].mAvatarInfo.aZone126TypeTime += 900;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 613: //[Mount Absorb S]
		mUSER[tUserIndex].mAvatarInfo.aMountAbsorbTime += 60;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(78, mUSER[tUserIndex].mAvatarInfo.aMountAbsorbTime);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 620: //[Rage Burst Pill]
		return;

	case 629: //[Double Rage Pill]
		return;

	case 1378: //[Heaven Chest]
		tRandomValue = rand() % 22;
		switch (mUSER[tUserIndex].mAvatarInfo.aPreviousTribe)
		{
		case 0:
			tWarlordSets[0] = 87063;
			tWarlordSets[1] = 87064;
			tWarlordSets[2] = 87065;
			tWarlordSets[3] = 87066;
			tWarlordSets[4] = 87067;
			tWarlordSets[5] = 87068;
			tWarlordSets[6] = 87069;
			tWarlordSets[7] = 87070;
			tWarlordSets[8] = 87071;
			tWarlordSets[9] = 87072;
			tWarlordSets[10] = 87073;
			tWarlordSets[11] = 87074;
			tWarlordSets[12] = 87075;
			tWarlordSets[13] = 87076;
			tWarlordSets[14] = 87077;
			tWarlordSets[15] = 87078;
			tWarlordSets[16] = 87079;
			tWarlordSets[17] = 87080;
			tWarlordSets[18] = 87081;
			tWarlordSets[19] = 87082;
			tWarlordSets[20] = 87083;
			tWarlordSets[21] = 87084;
			break;

		case 1:
			tWarlordSets[0] = 87085;
			tWarlordSets[1] = 87086;
			tWarlordSets[2] = 87087;
			tWarlordSets[3] = 87088;
			tWarlordSets[4] = 87089;
			tWarlordSets[5] = 87090;
			tWarlordSets[6] = 87091;
			tWarlordSets[7] = 87092;
			tWarlordSets[8] = 87093;
			tWarlordSets[9] = 87094;
			tWarlordSets[10] = 87095;
			tWarlordSets[11] = 87096;
			tWarlordSets[12] = 87097;
			tWarlordSets[13] = 87098;
			tWarlordSets[14] = 87099;
			tWarlordSets[15] = 87100;
			tWarlordSets[16] = 87101;
			tWarlordSets[17] = 87102;
			tWarlordSets[18] = 87103;
			tWarlordSets[19] = 87104;
			tWarlordSets[20] = 87105;
			tWarlordSets[21] = 87106;
			break;

		case 2:
			tWarlordSets[0] = 87107;
			tWarlordSets[1] = 87108;
			tWarlordSets[2] = 87109;
			tWarlordSets[3] = 87110;
			tWarlordSets[4] = 87111;
			tWarlordSets[5] = 87112;
			tWarlordSets[6] = 87113;
			tWarlordSets[7] = 87114;
			tWarlordSets[8] = 87115;
			tWarlordSets[9] = 87116;
			tWarlordSets[10] = 87117;
			tWarlordSets[11] = 87118;
			tWarlordSets[12] = 87119;
			tWarlordSets[13] = 87120;
			tWarlordSets[14] = 87121;
			tWarlordSets[15] = 87122;
			tWarlordSets[16] = 87123;
			tWarlordSets[17] = 87124;
			tWarlordSets[18] = 87125;
			tWarlordSets[19] = 87126;
			tWarlordSets[20] = 87127;
			tWarlordSets[21] = 87128;
			break;
		}
		tValue = tWarlordSets[tRandomValue];
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = tValue;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1379: //[Earth Chest]
		tRandomValue = rand() % 21;
		switch (mUSER[tUserIndex].mAvatarInfo.aPreviousTribe)
		{
		case 0:
			tWarlordSets[0] = 87000;
			tWarlordSets[1] = 87001;
			tWarlordSets[2] = 87002;
			tWarlordSets[3] = 87003;
			tWarlordSets[4] = 87004;
			tWarlordSets[5] = 87005;
			tWarlordSets[6] = 87006;
			tWarlordSets[7] = 87007;
			tWarlordSets[8] = 87008;
			tWarlordSets[9] = 87009;
			tWarlordSets[10] = 87010;
			tWarlordSets[11] = 87011;
			tWarlordSets[12] = 87012;
			tWarlordSets[13] = 87013;
			tWarlordSets[14] = 87014;
			tWarlordSets[15] = 87015;
			tWarlordSets[16] = 87016;
			tWarlordSets[17] = 87017;
			tWarlordSets[18] = 87018;
			tWarlordSets[19] = 87019;
			tWarlordSets[20] = 87020;
			break;

		case 1:
			tWarlordSets[0] = 87021;
			tWarlordSets[1] = 87022;
			tWarlordSets[2] = 87023;
			tWarlordSets[3] = 87024;
			tWarlordSets[4] = 87025;
			tWarlordSets[5] = 87026;
			tWarlordSets[6] = 87027;
			tWarlordSets[7] = 87028;
			tWarlordSets[8] = 87029;
			tWarlordSets[9] = 87030;
			tWarlordSets[10] = 87031;
			tWarlordSets[11] = 87032;
			tWarlordSets[12] = 87033;
			tWarlordSets[13] = 87034;
			tWarlordSets[14] = 87035;
			tWarlordSets[15] = 87036;
			tWarlordSets[16] = 87037;
			tWarlordSets[17] = 87038;
			tWarlordSets[18] = 87039;
			tWarlordSets[19] = 87040;
			tWarlordSets[20] = 87041;
			break;

		case 2:
			tWarlordSets[0] = 87042;
			tWarlordSets[1] = 87043;
			tWarlordSets[2] = 87044;
			tWarlordSets[3] = 87045;
			tWarlordSets[4] = 87046;
			tWarlordSets[5] = 87047;
			tWarlordSets[6] = 87048;
			tWarlordSets[7] = 87049;
			tWarlordSets[8] = 87050;
			tWarlordSets[9] = 87051;
			tWarlordSets[10] = 87052;
			tWarlordSets[11] = 87053;
			tWarlordSets[12] = 87054;
			tWarlordSets[13] = 87055;
			tWarlordSets[14] = 87056;
			tWarlordSets[15] = 87057;
			tWarlordSets[16] = 87058;
			tWarlordSets[17] = 87059;
			tWarlordSets[18] = 87060;
			tWarlordSets[19] = 87061;
			tWarlordSets[20] = 87062;
			break;
		}
		tValue = tWarlordSets[tRandomValue];
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = tValue;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 611: //[Mount Exp Pill 3%]
		if ((mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] %
			100000000) == 100000)
		{
			mTRANSFER.B_USE_INVENTORY_ITEM_RECV(1, tPage, tIndex, tValue);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		if (((mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] %
			100000) + 3000) > 100000)
		{
			mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] += 100000
				- (mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] %
					100000);
		}
		else
		{
			mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] += 3000;
		}
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		tValue = mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 612: //[Mount Exp Pill 1%]
		if ((mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] %
			100000000) == 100000)
		{
			mTRANSFER.B_USE_INVENTORY_ITEM_RECV(1, tPage, tIndex, tValue);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		if (((mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] %
			100000) + 1000) > 100000)
		{
			mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] += 100000
				- (mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] %
					100000);
		}
		else
		{
			mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] += 1000;
		}
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		tValue = mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 652: //[Mount Exp Pill 5%]
		if ((mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] %
			100000000) == 100000)
		{
			mTRANSFER.B_USE_INVENTORY_ITEM_RECV(1, tPage, tIndex, tValue);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		if (((mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] %
			100000) + 5000) > 100000)
		{
			mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] += 100000
				- (mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] %
					100000);
		}
		else
		{
			mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] += 5000;
		}
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		tValue = mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 601: //[Mount Box]
		tRandomValue = rand() % 1000;
		if (tRandomValue <= 10) // 15% Range
		{
			tRandomValue = rand() % 18;
			switch (tRandomValue)
			{
			case 0: tValue = 1307;
				break; // 15% Tiger
			case 1: tValue = 1308;
				break; // 15% Boar
			case 2: tValue = 1309;
				break; // 15% Stag
			case 3: tValue = 1315;
				break; // 15% Bear
			case 4: tValue = 1319;
				break; // 15% Ferret
			case 5: tValue = 1322;
				break; // 15% Bull
			case 6: tValue = 1325;
				break; // 15% Wolf
			case 7: tValue = 1328;
				break; // 15% Haechi
			case 8: tValue = 578;
				break; // E.Dmg Elixir
			case 9: tValue = 579;
				break; // E.Def Elixir
			case 10: tValue = 1166;
				break; // CP Prot Charm
			case 11: tValue = 1103;
				break; // Prot Charm
			case 12: tValue = 1237;
				break; // Wing Prot Charm
			}
		}
		else if (tRandomValue <= 30) // 10% Range
		{
			tRandomValue = rand() % 18;
			switch (tRandomValue)
			{
			case 0: tValue = 1304;
				break; // 10% Tiger
			case 1: tValue = 1305;
				break; // 10% Boar
			case 2: tValue = 1306;
				break; // 10% Stag
			case 3: tValue = 1314;
				break; // 10% Bear
			case 4: tValue = 1318;
				break; // 10% Ferret
			case 5: tValue = 1321;
				break; // 10% Bull
			case 6: tValue = 1324;
				break; // 10% Wolf
			case 7: tValue = 1327;
				break; // 10% Haechi
			case 8: tValue = 578;
				break; // E.Dmg Elixir
			case 9: tValue = 579;
				break; // E.Def Elixir
			case 10: tValue = 1166;
				break; // CP Prot Charm
			case 11: tValue = 1103;
				break; // Prot Charm
			case 12: tValue = 1237;
				break; // Wing Prot Charm
			}
		}
		else if (tRandomValue <= 70) // 5% Range
		{
			tRandomValue = rand() % 18;
			switch (tRandomValue)
			{
			case 0: tValue = 1301;
				break; // 5% Tiger
			case 1: tValue = 1302;
				break; // 5% Boar
			case 2: tValue = 1303;
				break; // 5% Stag
			case 3: tValue = 1313;
				break; // 5% Bear
			case 4: tValue = 1317;
				break; // 5% Ferret
			case 5: tValue = 1320;
				break; // 5% Bull
			case 6: tValue = 1323;
				break; // 5% Wolf
			case 7: tValue = 1326;
				break; // 5% Haechi
			case 8: tValue = 578;
				break; // E.Dmg Elixir
			case 9: tValue = 579;
				break; // E.Def Elixir
			case 10: tValue = 1166;
				break; // CP Prot Charm
			case 11: tValue = 1103;
				break; // Prot Charm
			case 12: tValue = 1237;
				break; // Wing Prot Charm
			}
		}
		else
		{
			tRandomValue = rand() % 10;
			switch (tRandomValue)
			{
			case 0: tValue = 1017;
				break; // Health Elixir
			case 1: tValue = 1018;
				break; // Chi Elixir
			case 2: tValue = 1093;
				break; // Agility Elixir
			case 3: tValue = 1092;
				break; // Strength Elixir
			case 4: tValue = 578;
				break; // E.Dmg Elixir
			case 5: tValue = 579;
				break; // E.Def Elixir
			case 6: tValue = 1166;
				break; // CP Prot Charm
			case 7: tValue = 1103;
				break; // Prot Charm
			case 8: tValue = 1237;
				break; // Wing Prot Charm
			case 9: tValue = 612;
				break; // Mount Exp Pill
			}
		}
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = tValue;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 602: //[Pet Box]
		tRandomValue = rand() % 1000;
		if (tRandomValue <= 10) // M15/M30 Pet
		{
			tRandomValue = rand() % 5;
			switch (tRandomValue)
			{
			case 0: tValue = 1016;
				break; // Levi
			case 1: tValue = 1015;
				break; // Vulture
			case 2: tValue = 1014;
				break; // Siwang
			case 3: tValue = 1013;
				break; // Phoenix
			case 4: tValue = 1012;
				break; // Falcon
			}
		}
		else if (tRandomValue <= 20) // M1 Pets
		{
			tRandomValue = rand() % 6;
			switch (tRandomValue)
			{
			case 0: tValue = 1011;
				break; // Pinyin
			case 1: tValue = 1010;
				break; // Jade Kirin
			case 2: tValue = 1009;
				break; // Turtilias Shell
			case 3: tValue = 1008;
				break; // Bat
			case 4: tValue = 1007;
				break; // Owl
			case 5: tValue = 1006;
				break; // Beetle
			}
		}
		else if (tRandomValue <= 55)
		{
			tRandomValue = rand() % 4;
			switch (tRandomValue)
			{
			case 0: tValue = 1005;
				break; // Moth
			case 1: tValue = 1004;
				break; // Demon
			case 2: tValue = 1003;
				break; // Relic
			case 3: tValue = 1002;
				break; // Bee
			}
		}
		else
		{
			tValue = mUTIL.GetRandomGachaItem();
		}
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = tValue;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 2101: //[Cape Chest]
		tRandomValue = rand() % 1000;
		if (tRandomValue <= 10)
		{
			tRandomValue = rand() % 18;
			switch (tRandomValue)
			{
			case 0: tValue = 1406;
				break; // Dragon Cape
			case 1: tValue = 1017;
				break; // Health Elixir
			case 2: tValue = 1018;
				break; // Chi Elixir
			case 3: tValue = 1093;
				break; // Agility Elixir
			case 4: tValue = 1092;
				break; // Strength Elixir
			case 5: tValue = 578;
				break; // E.Dmg Elixir
			case 6: tValue = 579;
				break; // E.Def Elixir
			case 7: tValue = 1166;
				break; // CP Prot Charm
			case 8: tValue = 1103;
				break; // Prot Charm
			case 9: tValue = 1237;
				break; // Wing Prot Charm
			case 10: tValue = 612;
				break; // Mount Exp Pill
			}
		}
		else if (tRandomValue <= 30)
		{
			tRandomValue = rand() % 18;
			switch (tRandomValue)
			{
			case 0: tValue = 1404;
				break; // Justice Cape
			case 1: tValue = 1017;
				break; // Health Elixir
			case 2: tValue = 1018;
				break; // Chi Elixir
			case 3: tValue = 1093;
				break; // Agility Elixir
			case 4: tValue = 1092;
				break; // Strength Elixir
			case 5: tValue = 578;
				break; // E.Dmg Elixir
			case 6: tValue = 579;
				break; // E.Def Elixir
			case 7: tValue = 1166;
				break; // CP Prot Charm
			case 8: tValue = 1103;
				break; // Prot Charm
			case 9: tValue = 1237;
				break; // Wing Prot Charm
			case 10: tValue = 612;
				break; // Mount Exp Pill
			}
		}
		else if (tRandomValue <= 50)
		{
			tRandomValue = rand() % 18;
			switch (tRandomValue)
			{
			case 0: tValue = 1403;
				break; // Warlord Cape
			case 1: tValue = 1017;
				break; // Health Elixir
			case 2: tValue = 1018;
				break; // Chi Elixir
			case 3: tValue = 1093;
				break; // Agility Elixir
			case 4: tValue = 1092;
				break; // Strength Elixir
			case 5: tValue = 578;
				break; // E.Dmg Elixir
			case 6: tValue = 579;
				break; // E.Def Elixir
			case 7: tValue = 1166;
				break; // CP Prot Charm
			case 8: tValue = 1103;
				break; // Prot Charm
			case 9: tValue = 1237;
				break; // Wing Prot Charm
			case 10: tValue = 612;
				break; // Mount Exp Pill
			}
		}
		else
		{
			tRandomValue = rand() % 10;
			switch (tRandomValue)
			{
			case 0: tValue = 1017;
				break; // Health Elixir
			case 1: tValue = 1018;
				break; // Chi Elixir
			case 2: tValue = 1093;
				break; // Agility Elixir
			case 3: tValue = 1092;
				break; // Strength Elixir
			case 4: tValue = 578;
				break; // E.Dmg Elixir
			case 5: tValue = 579;
				break; // E.Def Elixir
			case 6: tValue = 1166;
				break; // CP Prot Charm
			case 7: tValue = 1103;
				break; // Prot Charm
			case 8: tValue = 1237;
				break; // Wing Prot Charm
			case 9: tValue = 612;
				break; // Mount Exp Pill
			}
		}
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = tValue;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1200: //[Title Remove Scroll 100%]
		switch (mUSER[tUserIndex].mAvatarInfo.aTitle % 100)
		{
		case 1:
			mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 800;
			mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 800;
			break;
		case 2:
			mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 2500;
			mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 2500;
			break;
		case 3:
			mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 5000;
			mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 5000;
			break;
		case 4:
			mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 8400;
			mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 8400;
			break;
		case 5:
			mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 12600;
			mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 12600;
			break;
		case 6:
			mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 17700;
			mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 17700;
			break;
		case 7:
			mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 23600;
			mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 23600;
			break;
		case 8:
			mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 30400;
			mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 30400;
			break;
		case 9:
			mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 38000;
			mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 38000;
			break;
		case 10:
			mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 46500;
			mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 46500;
			break;
		case 11:
			mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 55800;
			mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 55800;
			break;
		case 12:
			mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 65800;
			mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 65800;
			break;
		}
		mUSER[tUserIndex].mAvatarInfo.aTitle = 0;
		mAVATAR_OBJECT[tUserIndex].mDATA.aTitle = 0;
		mTRANSFER.B_AVATAR_CHANGE_INFO_1(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 15,
		                                 mAVATAR_OBJECT[tUserIndex].mDATA.aTitle, 0, 0);
		mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], 1);
		tValue = mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 864: //[Skill Box]
		tRandomValue = rand() % 100;
		switch (mUSER[tUserIndex].mAvatarInfo.aPreviousTribe)
		{
		case 0:
			if (tRandomValue <= 20)
			{
				tRandomValue = rand() % 18;
				switch (tRandomValue)
				{
				case 0: tValue = 90317;
					break;
				case 1: tValue = 90318;
					break;
				case 2: tValue = 90319;
					break;
				case 3: tValue = 90320;
					break;
				case 4: tValue = 90321;
					break;
				case 5: tValue = 90322;
					break;
				case 6: tValue = 90570;
					break;
				case 7: tValue = 90571;
					break;
				case 8: tValue = 90572;
					break;
				case 9: tValue = 90573;
					break;
				case 10: tValue = 90574;
					break;
				case 11: tValue = 90575;
					break;
				case 12: tValue = 90542;
					break;
				case 13: tValue = 90543;
					break;
				case 14: tValue = 90559;
					break;
				case 15: tValue = 90567;
					break;
				case 16: tValue = 90568;
					break;
				case 17: tValue = 90569;
					break;
				}
			}
			else
			{
				tRandomValue = rand() % 6;
				switch (tRandomValue)
				{
				case 0: tValue = 90101;
					break;
				case 1: tValue = 90102;
					break;
				case 2: tValue = 90103;
					break;
				case 3: tValue = 90104;
					break;
				case 4: tValue = 90105;
					break;
				case 5: tValue = 90106;
					break;
				}
			}
			break;
		case 1:
			if (tRandomValue <= 20)
			{
				tRandomValue = rand() % 18;
				switch (tRandomValue)
				{
				case 0: tValue = 90323;
					break;
				case 1: tValue = 90324;
					break;
				case 2: tValue = 90325;
					break;
				case 3: tValue = 90326;
					break;
				case 4: tValue = 90327;
					break;
				case 5: tValue = 90328;
					break;
				case 6: tValue = 90576;
					break;
				case 7: tValue = 90577;
					break;
				case 8: tValue = 90578;
					break;
				case 9: tValue = 90579;
					break;
				case 10: tValue = 90580;
					break;
				case 11: tValue = 90581;
					break;
				case 12: tValue = 90542;
					break;
				case 13: tValue = 90543;
					break;
				case 14: tValue = 90559;
					break;
				case 15: tValue = 90567;
					break;
				case 16: tValue = 90568;
					break;
				case 17: tValue = 90569;
					break;
				}
			}
			else
			{
				tRandomValue = rand() % 6;
				switch (tRandomValue)
				{
				case 0: tValue = 90107;
					break;
				case 1: tValue = 90108;
					break;
				case 2: tValue = 90109;
					break;
				case 3: tValue = 90110;
					break;
				case 4: tValue = 90111;
					break;
				case 5: tValue = 90112;
					break;
				}
			}
			break;
		case 2:
			if (tRandomValue <= 20)
			{
				tRandomValue = rand() % 18;
				switch (tRandomValue)
				{
				case 0: tValue = 90329;
					break;
				case 1: tValue = 90330;
					break;
				case 2: tValue = 90331;
					break;
				case 3: tValue = 90332;
					break;
				case 4: tValue = 90333;
					break;
				case 5: tValue = 90334;
					break;
				case 6: tValue = 90582;
					break;
				case 7: tValue = 90583;
					break;
				case 8: tValue = 90584;
					break;
				case 9: tValue = 90585;
					break;
				case 10: tValue = 90586;
					break;
				case 11: tValue = 90587;
					break;
				case 12: tValue = 90542;
					break;
				case 13: tValue = 90543;
					break;
				case 14: tValue = 90559;
					break;
				case 15: tValue = 90567;
					break;
				case 16: tValue = 90568;
					break;
				case 17: tValue = 90569;
					break;
				}
			}
			else
			{
				tRandomValue = rand() % 6;
				switch (tRandomValue)
				{
				case 0: tValue = 90113;
					break;
				case 1: tValue = 90114;
					break;
				case 2: tValue = 90115;
					break;
				case 3: tValue = 90116;
					break;
				case 4: tValue = 90117;
					break;
				case 5: tValue = 90118;
					break;
				}
			}
			break;
		}
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = tValue;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 800: //[Refined Elixir Chest]
		tRandomValue = rand() % 6;
		switch (tRandomValue)
		{
		case 0: tValue = 801;
			break; // Ref Hp Lix
		case 1: tValue = 802;
			break; // Ref Chi Lix
		case 2: tValue = 803;
			break; // Red Str Lix
		case 3: tValue = 804;
			break; // Red Dex Lix
		case 4: tValue = 805;
			break; // Ref E Dmg Lix
		case 5: tValue = 806;
			break; // Red E Def Lix
		}
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = tValue;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 829: //[Pet Inventory Expand]
		tValue = mUTIL.ReturnAddDate(mUSER[tUserIndex].mAvatarInfo.aPetInventoryDate, 30);
		mUSER[tUserIndex].mAvatarInfo.aPetInventoryDate = tValue;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1043: //[Lucky Refinement]
		tRandomValue = rand() % 500;
		if (tRandomValue <= 50)
		{
			tRandomValue = rand() % 7;
			switch (tRandomValue)
			{
			case 0: tValue = 699;
				break; // Precious Jade
			case 1: tValue = 576;
				break; // Genuine Jade
			case 2: tValue = 1023;
				break; // 15%
			case 3: tValue = 1022;
				break; // 12%
			case 4: tValue = 1021;
				break; // 9%
			case 5: tValue = 1020;
				break; // 6%
			case 6: tValue = 1019;
				break; // 3%
			}
		}
		else
		{
			tRandomValue = rand() % 5;
			switch (tRandomValue)
			{
			case 0: tValue = 1023;
				break; // 15%
			case 1: tValue = 1022;
				break; // 12%
			case 2: tValue = 1021;
				break; // 9%
			case 3: tValue = 1020;
				break; // 6%
			case 4: tValue = 1019;
				break; // 3%
			}
		}
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = tValue;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 591: //[FL Teleport Scroll]
		if (!mGAME.ReturnTribeRole(mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aTribe))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
	/*		{
				int tResult = mGAME.CheckMoveZoneForMoveZoneItem(mUSER[tUserIndex].mAvatarInfo.aTribe
					, mUSER[tUserIndex].mAvatarInfo.aLevel1 + mUSER[tUserIndex].mAvatarInfo.aLevel2, tValue);
				if (tResult != 0)
				{
					mTRANSFER.B_USE_INVENTORY_ITEM_RECV(tResult, tPage, tIndex, tValue);
					mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
					return;
				}
			}
	*/
		mUSER[tUserIndex].mMoveItemInfo.mZoneNumber = tValue;
		mUSER[tUserIndex].mMoveItemInfo.mInvenPage = tPage;
		mUSER[tUserIndex].mMoveItemInfo.mInvenIndex = tIndex;
		mUSER[tUserIndex].mMoveItemInfo.mInvenX = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1];
		mUSER[tUserIndex].mMoveItemInfo.mInvenY = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2];
		mPLAYUSER_COM.U_SET_MOVE_ITEM_FOR_ZONE_SEND(mUSER[tUserIndex].mPlayUserIndex, &mUSER[tUserIndex].mMoveItemInfo);
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);

		mAVATAR_OBJECT[tUserIndex].mCheckDeath = true;
		mAVATAR_OBJECT[tUserIndex].mCheckCount = 9;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1494: //[Title Remove Scroll 70%]
		switch (mUSER[tUserIndex].mAvatarInfo.aTitle % 100)
		{
		case 1:
			mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 560;
			mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 560;
			break;
		case 2:
			mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 1750;
			mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 1750;
			break;
		case 3:
			mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 3500;
			mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 3500;
			break;
		case 4:
			mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 5880;
			mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 5880;
			break;
		case 5:
			mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 8820;
			mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 8820;
			break;
		case 6:
			mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 12390;
			mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 12390;
			break;
		case 7:
			mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 16520;
			mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 16520;
			break;
		case 8:
			mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 21280;
			mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 21280;
			break;
		case 9:
			mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 26600;
			mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 26600;
			break;
		case 10:
			mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 32550;
			mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 32550;
			break;
		case 11:
			mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 39060;
			mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 39060;
			break;
		case 12:
			mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 46060;
			mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 46060;
			break;
		}
		mUSER[tUserIndex].mAvatarInfo.aTitle = 0;
		mAVATAR_OBJECT[tUserIndex].mDATA.aTitle = 0;
		mTRANSFER.B_AVATAR_CHANGE_INFO_1(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 15,
		                                 mAVATAR_OBJECT[tUserIndex].mDATA.aTitle, 0, 0);
		mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], 1);
		tValue = mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1490: //[Instant 1% Exp Pill God]
		if (mUSER[tUserIndex].mAvatarInfo.aLevel2 < 1)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		tValue = (mLEVEL.ReturnHighExpValue(mUSER[tUserIndex].mAvatarInfo.aLevel2) * 0.01f);
		if ((mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 + tValue) > mLEVEL.ReturnHighExpValue(
			mUSER[tUserIndex].mAvatarInfo.aLevel2))
		{
			tValue = mLEVEL.ReturnHighExpValue(mUSER[tUserIndex].mAvatarInfo.aLevel2) - mUSER[tUserIndex].mAvatarInfo.
				aGeneralExperience2;
		}
		mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 += tValue;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(24, mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1489: //[Instant 3% Exp Pill Master]
		if ((mUSER[tUserIndex].mAvatarInfo.aLevel1 < 113) || (mUSER[tUserIndex].mAvatarInfo.aLevel2 > 0))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		tValue = ((mLEVEL.ReturnLevelFactor2(mUSER[tUserIndex].mAvatarInfo.aLevel1) - mLEVEL.ReturnLevelFactor1(
			mUSER[tUserIndex].mAvatarInfo.aLevel1)) * 0.03f);
		if ((mUSER[tUserIndex].mAvatarInfo.aGeneralExperience1 + tValue) > mLEVEL.ReturnLevelFactor2(
			mUSER[tUserIndex].mAvatarInfo.aLevel1))
		{
			tValue = mLEVEL.ReturnLevelFactor2(mUSER[tUserIndex].mAvatarInfo.aLevel1) - mUSER[tUserIndex].mAvatarInfo.
				aGeneralExperience1;
		}
		mUSER[tUserIndex].mAvatarInfo.aGeneralExperience1 += tValue;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(13, mUSER[tUserIndex].mAvatarInfo.aGeneralExperience1);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 649: //[Instant 5% Exp Pill]
		{
			if (mUSER[tUserIndex].mAvatarInfo.aLevel2 < 1)
			{
				tValue = ((mLEVEL.ReturnLevelFactor2(mUSER[tUserIndex].mAvatarInfo.aLevel1) - mLEVEL.ReturnLevelFactor1(
					mUSER[tUserIndex].mAvatarInfo.aLevel1)) * 0.05f);
				if ((mUSER[tUserIndex].mAvatarInfo.aGeneralExperience1 + tValue) > mLEVEL.ReturnLevelFactor2(
					mUSER[tUserIndex].mAvatarInfo.aLevel1))
				{
					tValue = mLEVEL.ReturnLevelFactor2(mUSER[tUserIndex].mAvatarInfo.aLevel1) - mUSER[tUserIndex].
						mAvatarInfo.aGeneralExperience1;
				}
				mUSER[tUserIndex].mAvatarInfo.aGeneralExperience1 += tValue;
				GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
				           mUSER[tUserIndex].mAvatarInfo.aName,
				           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
				           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
				           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
				           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
				mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				mTRANSFER.B_AVATAR_CHANGE_INFO_2(13, mUSER[tUserIndex].mAvatarInfo.aGeneralExperience1);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				return;
			}
			tValue = (mLEVEL.ReturnHighExpValue(mUSER[tUserIndex].mAvatarInfo.aLevel2) * 0.05f);
			if ((mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 + tValue) > mLEVEL.ReturnHighExpValue(
				mUSER[tUserIndex].mAvatarInfo.aLevel2))
			{
				tValue = mLEVEL.ReturnHighExpValue(mUSER[tUserIndex].mAvatarInfo.aLevel2) - mUSER[tUserIndex].
					mAvatarInfo.aGeneralExperience2;
			}
			mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 += tValue;
			GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
			           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
			           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
			           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
			           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
			mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(24, mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}

	case 650: //[Instant 10% Exp Pill]
		{
			if (mUSER[tUserIndex].mAvatarInfo.aLevel2 < 1)
			{
				tValue = ((mLEVEL.ReturnLevelFactor2(mUSER[tUserIndex].mAvatarInfo.aLevel1) - mLEVEL.ReturnLevelFactor1(
					mUSER[tUserIndex].mAvatarInfo.aLevel1)) * 0.1f);
				if ((mUSER[tUserIndex].mAvatarInfo.aGeneralExperience1 + tValue) > mLEVEL.ReturnLevelFactor2(
					mUSER[tUserIndex].mAvatarInfo.aLevel1))
				{
					tValue = mLEVEL.ReturnLevelFactor2(mUSER[tUserIndex].mAvatarInfo.aLevel1) - mUSER[tUserIndex].
						mAvatarInfo.aGeneralExperience1;
				}
				mUSER[tUserIndex].mAvatarInfo.aGeneralExperience1 += tValue;
				GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
				           mUSER[tUserIndex].mAvatarInfo.aName,
				           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
				           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
				           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
				           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
				mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				mTRANSFER.B_AVATAR_CHANGE_INFO_2(13, mUSER[tUserIndex].mAvatarInfo.aGeneralExperience1);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				return;
			}
			tValue = (mLEVEL.ReturnHighExpValue(mUSER[tUserIndex].mAvatarInfo.aLevel2) * 0.1f);
			if ((mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 + tValue) > mLEVEL.ReturnHighExpValue(
				mUSER[tUserIndex].mAvatarInfo.aLevel2))
			{
				tValue = mLEVEL.ReturnHighExpValue(mUSER[tUserIndex].mAvatarInfo.aLevel2) - mUSER[tUserIndex].
					mAvatarInfo.aGeneralExperience2;
			}
			mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 += tValue;
			GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
			           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
			           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
			           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
			           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
			mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(24, mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}

	case 1189: //[Faction Return Scroll]
		mUSER[tUserIndex].mAvatarInfo.aFactionReturnScroll++;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1180: //[Blood Boost]
		mUSER[tUserIndex].mAvatarInfo.aDoubleIslandOfBloodTimeUse++;
		GameLogger(1, "<USE_INVENTORY_ITEM>(%s),(%s), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], mSERVER_INFO.mServerNumber);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = 0;
		mTRANSFER.B_USE_INVENTORY_ITEM_RECV(0, tPage, tIndex, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	default:
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
	}
}

//W_TRIBE_BANK_SEND
void W_TRIBE_BANK_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int tSort;
	int tValue;
	CopyMemory(&tSort, &tPacket[1], 4);
	CopyMemory(&tValue, &tPacket[5], 4);

	int tBroadcastInfoSort;
	BYTE tBroadcastInfoData[MAX_BROADCAST_INFO_SIZE];
	int tMoney;

	switch (tSort)
	{
	case 1: //[Lookup]
		if (mUSER[tUserIndex].mAuthInfo.AuthType == 0)
		{
			if (mGAME.ReturnTribeRole(&mUSER[tUserIndex].mAvatarInfo.aName[0],
			                          mUSER[tUserIndex].mAvatarInfo.aTribe) == 0)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
		}
		mPLAYUSER_COM.U_TRIBE_BANK_INFO_VIEW_SEND(mUSER[tUserIndex].mAvatarInfo.aTribe);
		mTRANSFER.B_TRIBE_BANK_RECV(mPLAYUSER_COM.mRecv_Result, 1, &mPLAYUSER_COM.mRecv_TribeBankInfo[0], 0);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	case 2: //[Withdrawal]
		if ((tValue < 0) || (tValue > (MAX_TRIBE_BANK_SLOT_NUM - 1)) || (mGAME.ReturnTribeRole(
			&mUSER[tUserIndex].mAvatarInfo.aName[0],
			mUSER[tUserIndex].mAvatarInfo.aTribe) != 1) || (mGAME.ReturnTribeSubMasterNum(
			mUSER[tUserIndex].mAvatarInfo.aTribe) < 3))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		mPLAYUSER_COM.U_TRIBE_BANK_INFO_LOAD_SEND(mUSER[tUserIndex].mAvatarInfo.aTribe, tValue,
		                                          mUSER[tUserIndex].mAvatarInfo.aMoney);
		switch (mPLAYUSER_COM.mRecv_Result)
		{
		case 0:
			tMoney = mPLAYUSER_COM.mRecv_Money - mUSER[tUserIndex].mAvatarInfo.aMoney;
		//mGAMELOG.GL_618_TRIBE_MONEY(tUserIndex, tMoney);
			mUSER[tUserIndex].mAvatarInfo.aMoney = mPLAYUSER_COM.mRecv_Money;
			mTRANSFER.B_TRIBE_BANK_RECV(0, 2, &mPLAYUSER_COM.mRecv_TribeBankInfo[0], mPLAYUSER_COM.mRecv_Money);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		//--------------//
		//BROADCAST_INFO//
		//--------------//
			tBroadcastInfoSort = 62;
			CopyMemory(&tBroadcastInfoData[0], &mUSER[tUserIndex].mAvatarInfo.aTribe, 4);
			CopyMemory(&tBroadcastInfoData[4], &tMoney, 4);
			mCENTER_COM.U_DEMAND_BROADCAST_INFO(tBroadcastInfoSort, &tBroadcastInfoData[0]);
		//--------------//
		//--------------//
		//--------------//
			return;
		default:
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		return;
	default:
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
	}
}

//W_TRIBE_WORK_SEND
void W_TRIBE_WORK_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int tSort;
	int iCount = 0;
	BYTE tData[MAX_TRIBE_WORK_SIZE];
	CopyMemory(&tSort, &tPacket[1], 4);
	CopyMemory(&tData[0], &tPacket[5], MAX_TRIBE_WORK_SIZE);

	int index01;
	int tBroadcastInfoSort;
	BYTE tBroadcastInfoData[MAX_BROADCAST_INFO_SIZE];
	char tAvatarName[MAX_AVATAR_NAME_LENGTH];
	int tOtherUserIndex;
	int tTribeWeaponItemNumber;
	int tCPCost;
	// @ Tribe Formation Skill @
	int tTribeSkillSort = 0;
	// @
#ifdef __GOD__
	int tKillOtherTribeNum;
	int tRandomNumber;
	int iIndex;
	ITEM_INFO* tITEM_INFO;
#endif
	switch (tSort)
	{
	case 1: //[FREE_STAT_RESET]
		if (mUSER[tUserIndex].mAvatarInfo.aLevel1 > 39)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint += (mUSER[tUserIndex].mAvatarInfo.aVit + mUSER[tUserIndex].
			mAvatarInfo.aStr + mUSER[tUserIndex].mAvatarInfo.aInt + mUSER[tUserIndex].mAvatarInfo.aDex - 4);
		mUSER[tUserIndex].mAvatarInfo.aVit = 1;
		mUSER[tUserIndex].mAvatarInfo.aStr = 1;
		mUSER[tUserIndex].mAvatarInfo.aInt = 1;
		mUSER[tUserIndex].mAvatarInfo.aDex = 1;
		mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue = 1;
		mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue = 0;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		mTRANSFER.B_TRIBE_WORK_RECV(0, tSort, &tData[0]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 2: //[VFL_APPOINT]
		CopyMemory(&tAvatarName[0], &tData[0], MAX_AVATAR_NAME_LENGTH);
		tAvatarName[(MAX_AVATAR_NAME_LENGTH - 1)] = '\0';
		if (mGAME.ReturnTribeRole(&mUSER[tUserIndex].mAvatarInfo.aName[0], mUSER[tUserIndex].mAvatarInfo.aTribe) != 1)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		switch (mUSER[tUserIndex].mAvatarInfo.aTribe)
		{
		case 0:
			if (mSERVER_INFO.mServerNumber != 71)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		case 1:
			if (mSERVER_INFO.mServerNumber != 72)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		case 2:
			if (mSERVER_INFO.mServerNumber != 73)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		case 3:
			if (mSERVER_INFO.mServerNumber != 140)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		}
		if (strcmp(tAvatarName, "") == 0)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
#ifdef __GOD__
		for (index01 = 0; index01 < MAX_TRIBE_SUBMASTER_NUM; index01++)
		{
			if (strcmp(mGAME.mTribeInfo.mTribeSubMaster[mUSER[tUserIndex].mAvatarInfo.aTribe][index01],
			           tAvatarName) == 0)
			{
				break;
			}
		}
		if (index01 < MAX_TRIBE_SUBMASTER_NUM)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
#endif
		for (index01 = 0; index01 < MAX_TRIBE_SUBMASTER_NUM; index01++)
		{
			if (strcmp(mGAME.mTribeInfo.mTribeSubMaster[mUSER[tUserIndex].mAvatarInfo.aTribe][index01], "") == 0)
			{
				break;
			}
		}
		if (index01 == MAX_TRIBE_SUBMASTER_NUM)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		tOtherUserIndex = mUTIL.SearchAvatar(&tAvatarName[0], tUserIndex);
		if (tOtherUserIndex == -1)
		{
			mTRANSFER.B_TRIBE_WORK_RECV(1, tSort, &tData[0]);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		if (mUSER[tOtherUserIndex].mMoveZoneResult == 1)
		{
			mTRANSFER.B_TRIBE_WORK_RECV(1, tSort, &tData[0]);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		if (mUSER[tOtherUserIndex].mAvatarInfo.aLevel1 < 113)
		{
			mTRANSFER.B_TRIBE_WORK_RECV(2, tSort, &tData[0]);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		if (mUSER[tOtherUserIndex].mAvatarInfo.aKillOtherTribe < 1000)
		{
			mTRANSFER.B_TRIBE_WORK_RECV(3, tSort, &tData[0]);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
	/* SubMaster aName duplicate check */
		for (iCount = 0; iCount < MAX_TRIBE_SUBMASTER_NUM; iCount++)
		{
			if (!strcmp(mGAME.mTribeInfo.mTribeSubMaster[mUSER[tUserIndex].mAvatarInfo.aTribe][iCount], tAvatarName))
			{
				mTRANSFER.B_TRIBE_WORK_RECV(4, tSort, &tData[0]);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				return;
			}
		}
	/* SubMaster duplicate check */
		mTRANSFER.B_TRIBE_WORK_RECV(0, tSort, &tData[0]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	//--------------//
	//BROADCAST_INFO//
	//--------------//
		tBroadcastInfoSort = 60;
		CopyMemory(&tBroadcastInfoData[0], &mUSER[tUserIndex].mAvatarInfo.aTribe, 4);
		CopyMemory(&tBroadcastInfoData[4], &index01, 4);
		CopyMemory(&tBroadcastInfoData[8], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		mCENTER_COM.U_DEMAND_BROADCAST_INFO(tBroadcastInfoSort, &tBroadcastInfoData[0]);
	//--------------//
	//--------------//
	//--------------//
		strncpy(mGAME.mTribeInfo.mTribeSubMaster[mUSER[tUserIndex].mAvatarInfo.aTribe][index01], tAvatarName,
		        MAX_AVATAR_NAME_LENGTH);
		return;
	case 3: //[부세력장_제명]
		CopyMemory(&tAvatarName[0], &tData[0], MAX_AVATAR_NAME_LENGTH);
		tAvatarName[(MAX_AVATAR_NAME_LENGTH - 1)] = '\0';
		if (mGAME.ReturnTribeRole(&mUSER[tUserIndex].mAvatarInfo.aName[0], mUSER[tUserIndex].mAvatarInfo.aTribe) != 1)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		switch (mUSER[tUserIndex].mAvatarInfo.aTribe)
		{
		case 0:
			if (mSERVER_INFO.mServerNumber != 71)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		case 1:
			if (mSERVER_INFO.mServerNumber != 72)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		case 2:
			if (mSERVER_INFO.mServerNumber != 73)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		case 3:
			if (mSERVER_INFO.mServerNumber != 140)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		}
		for (index01 = 0; index01 < MAX_TRIBE_SUBMASTER_NUM; index01++)
		{
			if (strcmp(mGAME.mTribeInfo.mTribeSubMaster[mUSER[tUserIndex].mAvatarInfo.aTribe][index01],
			           tAvatarName) == 0)
			{
				break;
			}
		}
		if (index01 == MAX_TRIBE_SUBMASTER_NUM)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		mTRANSFER.B_TRIBE_WORK_RECV(0, tSort, &tData[0]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	//--------------//
	//BROADCAST_INFO//
	//--------------//
		tBroadcastInfoSort = 61;
		CopyMemory(&tBroadcastInfoData[0], &mUSER[tUserIndex].mAvatarInfo.aTribe, 4);
		CopyMemory(&tBroadcastInfoData[4], &index01, 4);
		CopyMemory(&tBroadcastInfoData[8], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		mCENTER_COM.U_DEMAND_BROADCAST_INFO(tBroadcastInfoSort, &tBroadcastInfoData[0]);
	//--------------//
	//--------------//
	//--------------//
		strcpy(mGAME.mTribeInfo.mTribeSubMaster[mUSER[tUserIndex].mAvatarInfo.aTribe][index01], "");
		return;
	case 4: //[FACTION_WEAPON]
		if ((mGAME.ReturnTribeRole(&mUSER[tUserIndex].mAvatarInfo.aName[0], mUSER[tUserIndex].mAvatarInfo.aTribe) != 1)
			&& (mGAME.ReturnTribeRole(&mUSER[tUserIndex].mAvatarInfo.aName[0], mUSER[tUserIndex].mAvatarInfo.aTribe) !=
				2))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		switch (mUSER[tUserIndex].mAvatarInfo.aTribe)
		{
		case 0:
			if (mSERVER_INFO.mServerNumber != 1)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		case 1:
			if (mSERVER_INFO.mServerNumber != 6)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		case 2:
			if (mSERVER_INFO.mServerNumber != 11)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		case 3:
			if (mSERVER_INFO.mServerNumber != 140)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aMoney < 100000000)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		switch (mUSER[tUserIndex].mAvatarInfo.aTribe)
		{
		case 0:
			tTribeWeaponItemNumber = 1075;
			break;
		case 1:
			tTribeWeaponItemNumber = 1076;
			break;
		case 2:
			tTribeWeaponItemNumber = 1077;
			break;
		case 3:
			tTribeWeaponItemNumber = 1078;
			break;
		}
		if (!mUTIL.ProcessForDropItem(10, tTribeWeaponItemNumber, 0, 0, 0,
		                              &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
		                              &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0], 0))
		{
			mTRANSFER.B_TRIBE_WORK_RECV(1, tSort, &tData[0]);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
	//mGAMELOG.GL_641_MAKE_TRIBE_WEAPON(tUserIndex, tTribeWeaponItemNumber, 0, 0, 0);
		mUSER[tUserIndex].mAvatarInfo.aMoney -= 100000000;
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(19, mUSER[tUserIndex].mAvatarInfo.aMoney);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mTRANSFER.B_TRIBE_WORK_RECV(0, tSort, &tData[0]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	// @ Tribe Formation Skill @
	case 5: //[세력진법]
		CopyMemory(&tTribeSkillSort, &tData[0], 4);
		if ((tTribeSkillSort < 0) || (tTribeSkillSort > 4))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mGAME.ReturnTribeRole(&mUSER[tUserIndex].mAvatarInfo.aName[0], mUSER[tUserIndex].mAvatarInfo.aTribe) != 1)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if ((mGAME.mWorldInfo.mTribePoint[0] <= 100) || (mGAME.mWorldInfo.mTribePoint[1] <= 100) || (mGAME.mWorldInfo.
			mTribePoint[2] <= 100) || (mGAME.mWorldInfo.mTribePoint[3] <= 100))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mGAME.ReturnSmallTribe() != mUSER[tUserIndex].mAvatarInfo.aTribe)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mGAME.mWorldInfo.mTribeSymbolBattle != 1)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		mTRANSFER.B_TRIBE_WORK_RECV(0, tSort, &tData[0]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	//--------------//
	//BROADCAST_INFO//
	//--------------//
		tBroadcastInfoSort = 302;
		CopyMemory(&tBroadcastInfoData[0], &mUSER[tUserIndex].mAvatarInfo.aTribe, 4);
		CopyMemory(&tBroadcastInfoData[4], &tTribeSkillSort, 4);
		mCENTER_COM.U_DEMAND_BROADCAST_INFO(tBroadcastInfoSort, &tBroadcastInfoData[0]);
	//--------------//
	//--------------//
	//--------------//
		return; // @
	case 6: //TITLE SYSTEM
		switch (mUSER[tUserIndex].mAvatarInfo.aTribe)
		{
		case 0:
			if (mSERVER_INFO.mServerNumber != 71)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		case 1:
			if (mSERVER_INFO.mServerNumber != 72)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		case 2:
			if (mSERVER_INFO.mServerNumber != 73)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		case 3:
			if (mSERVER_INFO.mServerNumber != 140)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe < 1000)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe < 1000)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}

		switch (mUSER[tUserIndex].mAvatarInfo.aTitle % 100)
		{
		case 0:
			tCPCost = 800;
			goto LABEL_33;
		case 1:
			tCPCost = 1700;
			goto LABEL_33;
		case 2:
			tCPCost = 2500;
			goto LABEL_33;
		case 3:
			tCPCost = 3400;
			goto LABEL_33;
		case 4:
			tCPCost = 4200;
			goto LABEL_33;
		case 5:
			tCPCost = 5100;
			goto LABEL_33;
		case 6:
			tCPCost = 5900;
			goto LABEL_33;
		case 7:
			tCPCost = 6800;
			goto LABEL_33;
		case 8:
			tCPCost = 7600;
			goto LABEL_33;
		case 9:
			tCPCost = 8500;
			goto LABEL_33;
		case 10:
			tCPCost = 9300;
			goto LABEL_33;
		case 11:
			tCPCost = 10000;
		}

	LABEL_33:
		mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe -= tCPCost;
		mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe -= tCPCost;
		if (mUSER[tUserIndex].mAvatarInfo.aTitle || tData[0] <= 1)
		{
			mUSER[tUserIndex].mAvatarInfo.aTitle++;
			mAVATAR_OBJECT[tUserIndex].mDATA.aTitle++;
		}
		else
		{
			mUSER[tUserIndex].mAvatarInfo.aTitle = 100 * (tData[0] - 1) + 1;
			mAVATAR_OBJECT[tUserIndex].mDATA.aTitle = 100 * (tData[0] - 1) + 1;
		}
		mTRANSFER.B_TRIBE_WORK_RECV(0, tSort, &tData[0]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mTRANSFER.B_AVATAR_CHANGE_INFO_1(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 15,
		                                 mAVATAR_OBJECT[tUserIndex].mDATA.aTitle, 0, 0);
		mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], 1);
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		return;

	case 7: //MAKE_RFC
		switch (mUSER[tUserIndex].mAvatarInfo.aTribe)
		{
		case 0:
			if (mSERVER_INFO.mServerNumber != 1)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		case 1:
			if (mSERVER_INFO.mServerNumber != 6)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		case 2:
			if (mSERVER_INFO.mServerNumber != 11)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		case 3:
			if (mSERVER_INFO.mServerNumber != 140)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aLevel1 < 1)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe < 100)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe < 100)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aUpgradeValue >= 96)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aMoney < 1000000)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}

	//Pay CP
		mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe -= 100;
		mUSER[tUserIndex].mAvatarInfo.aMoney -= 1000000;

		if ((rand() % 100) < 20)
		{
			mTRANSFER.B_TRIBE_WORK_RECV(0, tSort, &tData[0]);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			mUSER[tUserIndex].mAvatarInfo.aUpgradeValue++;
			mAVATAR_OBJECT[tUserIndex].mDATA.aHalo++;

			if (mUSER[tUserIndex].mAvatarInfo.aUpgradeValue == 96)
			{
				CopyMemory(&tData[0], &mUSER[tUserIndex].mAvatarInfo.aTribe, 4);
				CopyMemory(&tData[4], &mUSER[tUserIndex].mAvatarInfo.aName[0], MAX_AVATAR_NAME_LENGTH);
				mCENTER_COM.U_DEMAND_BROADCAST_INFO(672, &tData[0]);
			}
			return;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aUpgradeValue == 0)
		{
			mTRANSFER.B_TRIBE_WORK_RECV(1, tSort, &tData[0]);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			mTRANSFER.B_AVATAR_CHANGE_INFO_1(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 9,
			                                 mAVATAR_OBJECT[tUserIndex].mDATA.aHalo, 0, 0);
			mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], 1);
			return;
		}
		if ((rand() % 10) < 5)
		{
			if (mUSER[tUserIndex].mAvatarInfo.aCPProtectionCharm > 0)
			{
				mTRANSFER.B_TRIBE_WORK_RECV(1, tSort, &tData[0]);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				mUSER[tUserIndex].mAvatarInfo.aCPProtectionCharm--;
				mTRANSFER.B_AVATAR_CHANGE_INFO_2(31, mUSER[tUserIndex].mAvatarInfo.aCPProtectionCharm);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				return;
			}
			mTRANSFER.B_TRIBE_WORK_RECV(2, tSort, &tData[0]);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			mUSER[tUserIndex].mAvatarInfo.aUpgradeValue--;
			mAVATAR_OBJECT[tUserIndex].mDATA.aHalo--;
			return;
		}
		mTRANSFER.B_TRIBE_WORK_RECV(1, tSort, &tData[0]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 8: //[Beginner Reward]
		if (mUSER[tUserIndex].mAvatarInfo.aLevel1 >= 45 && mUSER[tUserIndex].mAvatarInfo.aLevel1 < 75)
		{
			mUSER[tUserIndex].mAvatarInfo.aBeginnerPackageIcon = 1;
			mTRANSFER.B_TRIBE_WORK_RECV(0, tSort, &tData[0]);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);

			switch (mUSER[tUserIndex].mAvatarInfo.aPreviousTribe)
			{
			case 0:
				mUTIL.ProcessForDropItem(3, 13016, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 13046, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 13061, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 13076, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 13091, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 13106, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				break;
			case 1:
				mUTIL.ProcessForDropItem(3, 33001, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 33046, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 33061, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 33076, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 33091, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 33106, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				break;
			case 2:
				mUTIL.ProcessForDropItem(3, 53016, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 53046, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 53061, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 53076, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 53091, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 53106, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				break;
			}
		}
		if (mUSER[tUserIndex].mAvatarInfo.aLevel1 >= 75 && mUSER[tUserIndex].mAvatarInfo.aLevel1 < 105)
		{
			switch (mUSER[tUserIndex].mAvatarInfo.aPreviousTribe)
			{
			case 0:
				mUTIL.ProcessForDropItem(3, 13025, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 13055, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 13070, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 13085, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 13100, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 13115, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				break;
			case 1:
				mUTIL.ProcessForDropItem(3, 33010, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 33055, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 33070, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 33085, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 33100, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 33115, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				break;
			case 2:
				mUTIL.ProcessForDropItem(3, 53025, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 53055, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 53070, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 53085, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 53100, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 53115, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				break;
			}
		}
		if (mUSER[tUserIndex].mAvatarInfo.aLevel1 >= 105 && mUSER[tUserIndex].mAvatarInfo.aLevel1 < 114)
		{
			switch (mUSER[tUserIndex].mAvatarInfo.aPreviousTribe)
			{
			case 0:
				mUTIL.ProcessForDropItem(3, 13172, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 13226, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 13253, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 13280, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 13307, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 13334, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				break;
			case 1:
				mUTIL.ProcessForDropItem(3, 33145, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 33226, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 33253, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 33280, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 33307, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 33334, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				break;
			case 2:
				mUTIL.ProcessForDropItem(3, 53173, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 53226, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 53253, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 53280, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 53307, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 53334, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				break;
			}
		}
		if (mUSER[tUserIndex].mAvatarInfo.aLevel1 >= 114 && mUSER[tUserIndex].mAvatarInfo.aLevel1 < 123)
		{
			switch (mUSER[tUserIndex].mAvatarInfo.aPreviousTribe)
			{
			case 0:
				mUTIL.ProcessForDropItem(3, 61004, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 61010, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 61013, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 61016, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 61019, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 61022, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				break;
			case 1:
				mUTIL.ProcessForDropItem(3, 71001, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 71010, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 71013, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 71016, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 71019, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 71022, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				break;
			case 2:
				mUTIL.ProcessForDropItem(3, 81004, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 81010, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 81013, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 81016, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 81019, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 81022, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				break;
			}
		}
		if (mUSER[tUserIndex].mAvatarInfo.aLevel1 >= 123 && mUSER[tUserIndex].mAvatarInfo.aLevel1 < 132)
		{
			switch (mUSER[tUserIndex].mAvatarInfo.aPreviousTribe)
			{
			case 0:
				mUTIL.ProcessForDropItem(3, 61078, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 61082, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 61085, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 61088, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 61091, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 61094, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				break;
			case 1:
				mUTIL.ProcessForDropItem(3, 71073, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 71082, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 71085, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 71088, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 71091, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 71094, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				break;
			case 2:
				mUTIL.ProcessForDropItem(3, 81076, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 81082, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 81085, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 81088, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 81091, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 81094, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				break;
			}
		}
		if (mUSER[tUserIndex].mAvatarInfo.aLevel1 >= 132 && mUSER[tUserIndex].mAvatarInfo.aLevel1 < 141)
		{
			switch (mUSER[tUserIndex].mAvatarInfo.aPreviousTribe)
			{
			case 0:
				mUTIL.ProcessForDropItem(3, 61148, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 61154, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 61157, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 61160, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 61163, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 61166, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				break;
			case 1:
				mUTIL.ProcessForDropItem(3, 71145, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 71154, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 71157, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 71160, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 71163, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 71166, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				break;
			case 2:
				mUTIL.ProcessForDropItem(3, 81148, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 81154, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 81157, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 81160, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 81163, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 81166, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				break;
			}
		}
		if (mUSER[tUserIndex].mAvatarInfo.aLevel1 >= 141 && mUSER[tUserIndex].mAvatarInfo.aLevel1 <= 145)
		{
			switch (mUSER[tUserIndex].mAvatarInfo.aPreviousTribe)
			{
			case 0:
				mUTIL.ProcessForDropItem(3, 61220, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 61226, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 61229, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 61232, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 61235, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 61238, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				break;
			case 1:
				mUTIL.ProcessForDropItem(3, 71217, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 71226, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 71229, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 71232, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 71235, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 71238, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				break;
			case 2:
				mUTIL.ProcessForDropItem(3, 81220, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 81226, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 81229, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 81232, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 81235, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				mUTIL.ProcessForDropItem(3, 81238, 1, 20, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
				                         &mAVATAR_OBJECT[tUserIndex].mDATA.aName[0],
				                         mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
				break;
			}
		}
		else

			mUSER[tUserIndex].mAvatarInfo.aBeginnerPackageIcon = 0;
		mTRANSFER.B_TRIBE_WORK_RECV(0, tSort, &tData[0]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 9: //[Turn On Ornament]
		mUSER[tUserIndex].mAvatarInfo.iUseOrnament = 1;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		mTRANSFER.B_TRIBE_WORK_RECV(0, tSort, &tData[0]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 10: //[Turn Off Ornament
		mUSER[tUserIndex].mAvatarInfo.iUseOrnament = 0;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		mTRANSFER.B_TRIBE_WORK_RECV(0, tSort, &tData[0]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 11: //[Rebirth]
		switch (mUSER[tUserIndex].mAvatarInfo.aTribe)
		{
		case 0:
			if (mSERVER_INFO.mServerNumber != 71)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		case 1:
			if (mSERVER_INFO.mServerNumber != 72)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		case 2:
			if (mSERVER_INFO.mServerNumber != 73)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		case 3:
			if (mSERVER_INFO.mServerNumber != 140)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aLevel2 != 12)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 != 1481117817)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe < 10000)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe < 10000)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aRebirth >= 12)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}

		mTRANSFER.B_TRIBE_WORK_RECV(0, tSort, &tData[0]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mUSER[tUserIndex].mAvatarInfo.aRebirth++;
		mAVATAR_OBJECT[tUserIndex].mDATA.aRebirth++;
		mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe -= 10000;
		mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe -= 10000;
		mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 = 0;
		mUSER[tUserIndex].mAvatarInfo.aLifeOrDeath += 5; //-5 LoD
		mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint += 100; //Custom 100 stats for rebirth leveling
		mTRANSFER.B_AVATAR_CHANGE_INFO_1(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 14,
		                                 mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe,
		                                 mAVATAR_OBJECT[tUserIndex].mDATA.aRebirth,
		                                 mUSER[tUserIndex].mAvatarInfo.aLifeOrDeath);
		mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], 1);
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		return;


	default: //[오류]
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
	}
}

//W_ADD_ITEM_SEND
void W_ADD_ITEM_SEND(int tUserIndex)
{
	unsigned int tRandom = 0;
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int tPage1;
	int tIndex1;
	int tPage2;
	int tIndex2;
	CopyMemory(&tPage1, &tPacket[1], 4);
	CopyMemory(&tIndex1, &tPacket[5], 4);
	CopyMemory(&tPage2, &tPacket[9], 4);
	CopyMemory(&tIndex2, &tPacket[13], 4);
	if ((tPage1 < 0) || (tPage1 > 1) || (tIndex1 < 0) || (tIndex1 > (MAX_INVENTORY_SLOT_NUM - 1)) || (tPage2 < 0) || (
		tPage2 > 1) || (tIndex2 < 0) || (tIndex2 > (MAX_INVENTORY_SLOT_NUM - 1)))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	ITEM_INFO* tITEM_INFO1;
	ITEM_INFO* tITEM_INFO2;
	int tCost;
	int tProbability;

	if ((tPage1 == 1) || (tPage2 == 1))
	{
		if (mUSER[tUserIndex].mAvatarInfo.aExpandInventoryDate < mUTIL.ReturnNowDate())
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
	}
	tITEM_INFO1 = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0]);
	if (tITEM_INFO1 == nullptr)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if ((tITEM_INFO1->iType != 3) && (tITEM_INFO1->iType != 4))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if ((tITEM_INFO1->iSort != 7) && (tITEM_INFO1->iSort != 8) && (tITEM_INFO1->iSort != 9) && (tITEM_INFO1->iSort !=
		10) && (tITEM_INFO1->iSort != 11) && (tITEM_INFO1->iSort != 12) && (tITEM_INFO1->iSort != 13) && (tITEM_INFO1->
		iSort != 14) && (tITEM_INFO1->iSort != 15) && (tITEM_INFO1->iSort != 16) && (tITEM_INFO1->iSort != 17) && (
		tITEM_INFO1->iSort != 18) && (tITEM_INFO1->iSort != 19) && (tITEM_INFO1->iSort != 20) && (tITEM_INFO1->iSort !=
		21))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (tITEM_INFO1->iCheckHighImprove != 2)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUTIL.ReturnIUValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]) >= MAX_ADD_ITEM_NUM)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	tITEM_INFO2 = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0]);
	if (tITEM_INFO2 == nullptr)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (tITEM_INFO2->iType != 3)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if ((tITEM_INFO2->iSort != 7) && (tITEM_INFO2->iSort != 8) && (tITEM_INFO2->iSort != 9) && (tITEM_INFO2->iSort !=
		10) && (tITEM_INFO2->iSort != 11) && (tITEM_INFO2->iSort != 12) && (tITEM_INFO2->iSort != 13) && (tITEM_INFO2->
		iSort != 14) && (tITEM_INFO2->iSort != 15) && (tITEM_INFO2->iSort != 16) && (tITEM_INFO2->iSort != 17) && (
		tITEM_INFO2->iSort != 18) && (tITEM_INFO2->iSort != 19) && (tITEM_INFO2->iSort != 20) && (tITEM_INFO2->iSort !=
		21))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if ((mUTIL.ReturnISValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][4]) > 0) || (mUTIL.ReturnIUValue(
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][4]) > 0))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (tITEM_INFO1->iSort != tITEM_INFO2->iSort)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	switch (tITEM_INFO1->iType)
	{
	case 3:
#ifdef __GOD__
		if ((tITEM_INFO2->iLevel + tITEM_INFO2->iMartialLevel) != (tITEM_INFO1->iLevel + tITEM_INFO1->iMartialLevel))
#else
		if (tITEM_INFO2->iLevel != tITEM_INFO1->iLevel)
#endif
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		break;
	case 4:
		switch (tITEM_INFO1->iLevel)
		{
		case 100:
			if (tITEM_INFO2->iLevel != 95)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		case 110:
			if (tITEM_INFO2->iLevel != 105)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		case 115:
			if (tITEM_INFO2->iLevel != 114)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		case 118:
			if (tITEM_INFO2->iLevel != 117)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		case 121:
			if (tITEM_INFO2->iLevel != 120)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		case 124:
			if (tITEM_INFO2->iLevel != 123)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		case 127:
			if (tITEM_INFO2->iLevel != 126)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		case 130:
			if (tITEM_INFO2->iLevel != 129)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		case 133:
			if (tITEM_INFO2->iLevel != 132)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		case 136:
			if (tITEM_INFO2->iLevel != 135)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		case 139:
			if (tITEM_INFO2->iLevel != 138)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		case 142:
			if (tITEM_INFO2->iLevel != 141)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			break;
		case 145:
#ifdef __GOD__
			if (tITEM_INFO1->iMartialLevel < 1)
			{
#endif
				if ((tITEM_INFO2->iLevel != 144) && (tITEM_INFO2->iLevel != 145))
				{
					mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
					return;
				}
#ifdef __GOD__
			}
			else
			{
				if ((tITEM_INFO2->iLevel + tITEM_INFO2->iMartialLevel) != (tITEM_INFO1->iLevel + tITEM_INFO1->
					iMartialLevel))
				{
					mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
					return;
				}
			}
#endif
			break;
		}
		break;
	}
	switch (mUTIL.ReturnIUValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]))
	{
	case 0:
		tCost = 1000000;
		tProbability = 65;
		break;
	case 1:
		tCost = 1500000;
		tProbability = 60;
		break;
	case 2:
		tCost = 2000000;
		tProbability = 55;
		break;
	case 3:
		tCost = 2500000;
		tProbability = 50;
		break;
	case 4:
		tCost = 3000000;
		tProbability = 45;
		break;
	case 5:
		tCost = 3500000;
		tProbability = 40;
		break;
	case 6:
		tCost = 4000000;
		tProbability = 35;
		break;
	case 7:
		tCost = 4500000;
		tProbability = 30;
		break;
	case 8:
		tCost = 5000000;
		tProbability = 25;
		break;
	case 9:
		tCost = 5500000;
		tProbability = 20;
		break;
	case 10:
		tCost = 6000000;
		tProbability = 15;
		break;
	case 11:
		tCost = 6500000;
		tProbability = 10;
		break;
	}
	if (mUSER[tUserIndex].mAvatarInfo.aMoney < tCost)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	// Lucky Combine
	if (mUSER[tUserIndex].mAvatarInfo.aLuckyCombine > 0)
	{
		tProbability += 5;
		mUSER[tUserIndex].mAvatarInfo.aLuckyCombine--;
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(28, mUSER[tUserIndex].mAvatarInfo.aLuckyCombine);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	}

	tProbability += static_cast<int>(static_cast<float>(mAVATAR_OBJECT[tUserIndex].GetLuck()) / 300.0f);
	tRandom = rand();
	if ((tRandom % 100) < static_cast<unsigned int>(tProbability))
	{
		mUSER[tUserIndex].mAvatarInfo.aMoney -= tCost;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4] = mUTIL.ChangeIUValue(
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4], 1);
		//mGAMELOG.GL_634_ADD_ITEM(tUserIndex, 1, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5], tCost);
		//mGAMELOG.GL_634_ADD_ITEM(tUserIndex, 2, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][5], tCost);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][5] = 0;
		mTRANSFER.B_ADD_ITEM_RECV(0, tCost);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	mUSER[tUserIndex].mAvatarInfo.aMoney -= tCost;
	//mGAMELOG.GL_634_ADD_ITEM(tUserIndex, 3, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5], tCost);
	mTRANSFER.B_ADD_ITEM_RECV(1, tCost);
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_IMPROVE_ITEM_SEND
void W_IMPROVE_ITEM_SEND(int tUserIndex)
{
	unsigned int tRandom = 0;
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	int tBroadcastInfoSort;
	BYTE tBroadcastInfoData[MAX_BROADCAST_INFO_SIZE];

	int tPage1;
	int tIndex1;
	int tPage2;
	int tIndex2;
	CopyMemory(&tPage1, &tPacket[1], 4);
	CopyMemory(&tIndex1, &tPacket[5], 4);
	CopyMemory(&tPage2, &tPacket[9], 4);
	CopyMemory(&tIndex2, &tPacket[13], 4);
	if ((tPage1 < 0) || (tPage1 > 1) || (tIndex1 < 0) || (tIndex1 > (MAX_INVENTORY_SLOT_NUM - 1)) || (tPage2 < 0) || (
		tPage2 > 1) || (tIndex2 < 0) || (tIndex2 > (MAX_INVENTORY_SLOT_NUM - 1)))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	ITEM_INFO* tITEM_INFO1;
	ITEM_INFO* tITEM_INFO2;
	int tCost;
	int iValue;
	int tVisualValue;
	int tCPCost;
	int tProbability1;
	int tProbability2;
	int tProtectIS = 0;

	if ((tPage1 == 1) || (tPage2 == 1))
	{
		if (mUSER[tUserIndex].mAvatarInfo.aExpandInventoryDate < mUTIL.ReturnNowDate())
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
	}
	tITEM_INFO1 = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0]);
	if (tITEM_INFO1 == nullptr)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if ((tITEM_INFO1->iSort != 6) && (tITEM_INFO1->iSort != 7) && (tITEM_INFO1->iSort != 8) && (tITEM_INFO1->iSort != 9)
		&& (tITEM_INFO1->iSort != 10) && (tITEM_INFO1->iSort != 11) && (tITEM_INFO1->iSort != 12) && (tITEM_INFO1->iSort
			!= 13) && (tITEM_INFO1->iSort != 14) && (tITEM_INFO1->iSort != 15) && (tITEM_INFO1->iSort != 16) && (
			tITEM_INFO1->iSort != 17) && (tITEM_INFO1->iSort != 18) && (tITEM_INFO1->iSort != 19) && (tITEM_INFO1->iSort
			!= 20) && (tITEM_INFO1->iSort != 21))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (tITEM_INFO1->iCheckImprove != 2)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUTIL.ReturnISValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]) >= MAX_IMPROVE_ITEM_NUM)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	tITEM_INFO2 = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0]);
	if (tITEM_INFO2 == nullptr)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if ((tITEM_INFO2->iIndex != 1019) && (tITEM_INFO2->iIndex != 1020) && (tITEM_INFO2->iIndex != 1021) && (tITEM_INFO2
		->iIndex != 1022) && (tITEM_INFO2->iIndex != 1023) && (tITEM_INFO2->iIndex != 695) && (tITEM_INFO2->iIndex !=
		633) && (tITEM_INFO2->iIndex != 696) && (tITEM_INFO2->iIndex != 698) && (tITEM_INFO2->iIndex != 825) && (
		tITEM_INFO2->iIndex != 619) && (tITEM_INFO2->iIndex != 1422) && (tITEM_INFO2->iIndex != 1437) && (tITEM_INFO2->
		iIndex != 826))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	switch (mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0])
	{
	case 633:
		tCost = 0;
		tCPCost = 0;
		iValue = 1;
		tVisualValue = 1;
		break;
	case 695:
		tCost = 0;
		tCPCost = 50;
		iValue = 1;
		tVisualValue = 1;
		break;
	case 696:
		tCost = 0;
		tCPCost = 50;
		iValue = 2;
		tVisualValue = 2;
		break;
	case 698:
		tCost = 0;
		tCPCost = 50;
		iValue = 3;
		tVisualValue = 3;
		break;
	case 825:
		tCost = 0;
		tCPCost = 0;
		iValue = 50;
		tVisualValue = 50;
		break;
	case 826:
		tCost = 0;
		tCPCost = 0;
		iValue = 40;
		tVisualValue = 40;
		break;
	case 619:
		tCost = 0;
		tCPCost = 0;
		iValue = 40;
		tVisualValue = 40;
		break;
	case 1422:
		tCost = 0;
		tCPCost = 0;
		iValue = 1;
		tVisualValue = 1;
		break;
	case 1019:
		tCost = 10000;
		tCPCost = 0;
		iValue = 1;
		tVisualValue = 1;
		break;
	case 1020:
		tCost = 30000;
		tCPCost = 0;
		iValue = 2;
		tVisualValue = 2;
		break;
	case 1021:
		tCost = 50000;
		tCPCost = 0;
		iValue = 3;
		tVisualValue = 3;
		break;
	case 1022:
		tCost = 70000;
		tCPCost = 0;
		iValue = 4;
		tVisualValue = 4;
		break;
	case 1023:
		tCost = 90000;
		tCPCost = 0;
		iValue = 5;
		tVisualValue = 5;
		break;
	case 1437:
		tCost = 90000;
		tCPCost = 0;
		iValue = 2;
		tVisualValue = 2;
		break;
	}
	if (mUSER[tUserIndex].mAvatarInfo.aMoney < tCost)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe < tCPCost)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if ((mUTIL.ReturnISValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]) + iValue) >=
		MAX_IMPROVE_ITEM_NUM)
	{
		iValue = MAX_IMPROVE_ITEM_NUM - mUTIL.ReturnISValue(
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]);
	}
	// Make 15% Mats only 3% On OverUps
	if (mUTIL.ReturnISValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]) > 40)
	{
		if (tITEM_INFO2->iIndex == 1023)
		{
			iValue = 1;
		}
	}
	//tProbability1 = 103 - (mUTIL.ReturnISValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]) + iValue) * 3 + (int)((float)mAVATAR_OBJECT[tUserIndex].GetLuck() / 100.0f);
	tProbability1 = 93 - (mUTIL.ReturnISValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]) + iValue) *
		3 + static_cast<int>(static_cast<float>(mAVATAR_OBJECT[tUserIndex].GetLuck()) / 100.0f);
	if (tProbability1 < 20)
	{
		tProbability1 = 20;
	}
	// Over Ups chance is 30% Success
	if (mUTIL.ReturnISValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]) > 40)
	{
		tProbability1 = 30;
	}
	// Make Tickets 100% Success
	if ((tITEM_INFO2->iIndex == 1422) || (tITEM_INFO2->iIndex == 619) || (tITEM_INFO2->iIndex == 825) || (tITEM_INFO2->
		iIndex == 633) || (tITEM_INFO2->iIndex == 826))
	{
		tProbability1 = 100;
	}
	// Prevent Item from going straight to 150% when used 120 ET on item with existing enchant
	if (tITEM_INFO2->iIndex == 619)
	{
		if ((mUTIL.ReturnISValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]) + iValue) >= 40)
		{
			iValue = 40 - (mUTIL.ReturnISValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]));
		}
	}
	// Prevent Wings Going over 120
	if (tITEM_INFO1->iSort == 6)
	{
		if ((mUTIL.ReturnISValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]) + iValue) >= 40)
		{
			iValue = 40 - (mUTIL.ReturnISValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]));
		}
	}
	//Prevent Going Above 120%
	if ((tITEM_INFO2->iIndex == 1019) || (tITEM_INFO2->iIndex == 1020) || (tITEM_INFO2->iIndex == 1021)
		|| (tITEM_INFO2->iIndex == 1022) || (tITEM_INFO2->iIndex == 1023)
		&& ((mUTIL.ReturnISValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]) < 40)))
	{
		if ((mUTIL.ReturnISValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]) + iValue) >= 40)
		{
			iValue = 40 - (mUTIL.ReturnISValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]));
		}
	}

	tProbability2 = -57 + (mUTIL.ReturnISValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]) + iValue) *
		3 - static_cast<int>(static_cast<float>(mAVATAR_OBJECT[tUserIndex].GetLuck()) / 100.0f);
	if (tProbability2 < 0)
	{
		tProbability2 = 0;
	}
	// Items Above 120% have 100% Chance to Use PC on fail
	if (mUTIL.ReturnISValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]) > 40)
	{
		tProbability2 = 100;
	}

	tRandom = rand();
	if ((tRandom % 100) < static_cast<unsigned int>(tProbability1))
	{
		mUSER[tUserIndex].mAvatarInfo.aMoney -= tCost;
		mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe -= tCPCost;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4] += iValue;
		//mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4] = mUTIL.ChangeISValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4], iValue);
		//mGAMELOG.GL_608_IMPROVE_ITEM(tUserIndex, 1, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5], tCost);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][5] = 0;
		if ((tITEM_INFO2->iIndex == 825) || (tITEM_INFO2->iIndex == 619))
		{
			mTRANSFER.B_IMPROVE_ITEM_RECV(0, tCost, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		}
		else if (tITEM_INFO2->iIndex == 633)
		{
			mTRANSFER.B_IMPROVE_ITEM_RECV(0, tCost, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		}
		else
		{
			mTRANSFER.B_IMPROVE_ITEM_RECV(0, tCost, iValue);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		}


		if (mUTIL.ReturnISValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]) >= MAX_IMPROVE_ITEM_NUM)
		{
			tBroadcastInfoSort = 111;
			CopyMemory(&tBroadcastInfoData[0], &mUSER[tUserIndex].mAvatarInfo.aTribe, 4);
			CopyMemory(&tBroadcastInfoData[4], &mUSER[tUserIndex].mAvatarInfo.aName[0], MAX_AVATAR_NAME_LENGTH);
			mCENTER_COM.U_DEMAND_BROADCAST_INFO(tBroadcastInfoSort, &tBroadcastInfoData[0]);
			//--------------//
			//--------------//
			//--------------//
		}
		// Wings 120% Broadcast Success
		if (tITEM_INFO1->iSort == 6)
		{
			if (mUTIL.ReturnISValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]) >= 40)
			{
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tBroadcastInfoSort = 115;
				CopyMemory(&tBroadcastInfoData[0], &mUSER[tUserIndex].mAvatarInfo.aTribe, 4);
				CopyMemory(&tBroadcastInfoData[4], &mUSER[tUserIndex].mAvatarInfo.aName[0], MAX_AVATAR_NAME_LENGTH);
				mCENTER_COM.U_DEMAND_BROADCAST_INFO(tBroadcastInfoSort, &tBroadcastInfoData[0]);
				//--------------//
				//--------------//
				//--------------//
			}
		}
		return;
	}

	tRandom = rand();
	if ((tRandom % 100) < static_cast<unsigned int>(tProbability2))
	{
		//For Wings
		if (tITEM_INFO1->iSort == 6)
		{
			if (mUSER[tUserIndex].mAvatarInfo.aWingProtectionScroll > 0)
			{
				mUSER[tUserIndex].mAvatarInfo.aMoney -= tCost;
				mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe -= tCPCost;
				mUSER[tUserIndex].mAvatarInfo.aWingProtectionScroll--;
				mTRANSFER.B_AVATAR_CHANGE_INFO_2(99, mUSER[tUserIndex].mAvatarInfo.aWingProtectionScroll);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4] -= 1;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][1] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][2] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][3] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][4] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][5] = 0;
				mTRANSFER.B_IMPROVE_ITEM_RECV(1, tCPCost, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal, mTRANSFER.mOriginalSize);
				return;
			}

			mUSER[tUserIndex].mAvatarInfo.aMoney -= tCost;
			mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe -= tCPCost;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][1] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][2] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][3] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][4] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][5] = 0;
			mTRANSFER.B_IMPROVE_ITEM_RECV(2, tCPCost, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal, mTRANSFER.mOriginalSize);
			return;
		}

		if ((mUSER[tUserIndex].mAvatarInfo.aProtectForDestroy & 0x0000FFFF) > 0)
		{
			if (mUTIL.ReturnISValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]) == 41)
			{
				mUSER[tUserIndex].mAvatarInfo.aMoney -= tCost;
				mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe -= tCPCost;
				//mGAMELOG.GL_608_IMPROVE_ITEM(tUserIndex, 3, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5], tCost);
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4] -= 1;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][1] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][2] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][3] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][4] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][5] = 0;
				mTRANSFER.B_IMPROVE_ITEM_RECV(3, tCost, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				return;
			}
			if (mUTIL.ReturnISValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]) > 40)
			{
				mUSER[tUserIndex].mAvatarInfo.aProtectForDestroy--;
				mTRANSFER.B_AVATAR_CHANGE_INFO_2(15, mUSER[tUserIndex].mAvatarInfo.aProtectForDestroy);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				mUSER[tUserIndex].mAvatarInfo.aMoney -= tCost;
				mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe -= tCPCost;
				//mGAMELOG.GL_608_IMPROVE_ITEM(tUserIndex, 3, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5], tCost);
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4] -= 1;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][1] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][2] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][3] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][4] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][5] = 0;
				mTRANSFER.B_IMPROVE_ITEM_RECV(1, tCost, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				return;
			}

			mUSER[tUserIndex].mAvatarInfo.aProtectForDestroy--;
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(15, mUSER[tUserIndex].mAvatarInfo.aProtectForDestroy);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			mUSER[tUserIndex].mAvatarInfo.aMoney -= tCost;
			mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe -= tCPCost;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4] -= 1;
			//mGAMELOG.GL_608_IMPROVE_ITEM(tUserIndex, 2, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5], tCost);
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][1] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][2] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][3] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][4] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][5] = 0;
			mTRANSFER.B_IMPROVE_ITEM_RECV(1, tCost, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		if (mUTIL.ReturnISValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]) == 41)
		{
			mUSER[tUserIndex].mAvatarInfo.aMoney -= tCost;
			mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe -= tCPCost;
			//mGAMELOG.GL_608_IMPROVE_ITEM(tUserIndex, 3, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5], tCost);
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4] -= 1;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][1] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][2] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][3] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][4] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][5] = 0;
			mTRANSFER.B_IMPROVE_ITEM_RECV(3, tCost, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		if (mUTIL.ReturnISValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]) > 40)
		{
			mUSER[tUserIndex].mAvatarInfo.aMoney -= tCost;
			mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe -= tCPCost;
			//mGAMELOG.GL_608_IMPROVE_ITEM(tUserIndex, 3, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5], tCost);
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4] -= (mUTIL.ReturnISValue(
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]) - 40);
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][1] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][2] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][3] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][4] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][5] = 0;
			mTRANSFER.B_IMPROVE_ITEM_RECV(3, tCost, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aMoney -= tCost;
		mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe -= tCPCost;
		//mGAMELOG.GL_608_IMPROVE_ITEM(tUserIndex, 3, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5], tCost);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][5] = 0;
		mTRANSFER.B_IMPROVE_ITEM_RECV(2, tCost, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	mUSER[tUserIndex].mAvatarInfo.aMoney -= tCost;
	mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe -= tCPCost;
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4] -= 1;
	//mGAMELOG.GL_608_IMPROVE_ITEM(tUserIndex, 4, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5], tCost);
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] = 0;
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][1] = 0;
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][2] = 0;
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][3] = 0;
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][4] = 0;
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][5] = 0;
	mTRANSFER.B_IMPROVE_ITEM_RECV(1, tCost, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]);
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_PROCESS_ATTACK_SEND
void W_PROCESS_ATTACK_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	ATTACK_FOR_PROTOCOL tAttackInfo;
	CopyMemory(&tAttackInfo, &tPacket[1], sizeof(ATTACK_FOR_PROTOCOL));

	switch (tAttackInfo.mCase)
	{
	case 1: //[bismut]
		if ((tAttackInfo.mServerIndex1 == tUserIndex) && (tAttackInfo.mUniqueNumber1 == mAVATAR_OBJECT[tUserIndex].
			mUniqueNumber))
		{
			if (mUSER[tUserIndex].mCheckMaxAttackPacketNum == 1)
			{
				mUSER[tUserIndex].mNowAttackPacketNum++;
				if (mUSER[tUserIndex].mNowAttackPacketNum > mUSER[tUserIndex].mMaxAttackPacketNum)
				{
					return;
				}
				if (tAttackInfo.mAttackActionValue4 != mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aSort)
				{
					return;
				}
			}
		}
		mGAME.ProcessAttack01(tUserIndex, &tAttackInfo);
		return;

	case 2: //[Influence]
		if ((tAttackInfo.mServerIndex1 == tUserIndex) && (tAttackInfo.mUniqueNumber1 == mAVATAR_OBJECT[tUserIndex].
			mUniqueNumber))
		{
			if (mUSER[tUserIndex].mCheckMaxAttackPacketNum == 1)
			{
				mUSER[tUserIndex].mNowAttackPacketNum++;
				if (mUSER[tUserIndex].mNowAttackPacketNum > mUSER[tUserIndex].mMaxAttackPacketNum)
				{
					return;
				}
				if (tAttackInfo.mAttackActionValue4 != mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aSort)
				{
					return;
				}
			}
		}
		mGAME.ProcessAttack02(tUserIndex, &tAttackInfo);
		return;

	case 3: //[Character->Monster]
		if ((tAttackInfo.mServerIndex1 == tUserIndex) && (tAttackInfo.mUniqueNumber1 == mAVATAR_OBJECT[tUserIndex].
			mUniqueNumber))
		{
			if (mUSER[tUserIndex].mCheckMaxAttackPacketNum == 1)
			{
				mUSER[tUserIndex].mNowAttackPacketNum++;
				if (mUSER[tUserIndex].mNowAttackPacketNum > mUSER[tUserIndex].mMaxAttackPacketNum)
				{
					return;
				}
				if (tAttackInfo.mAttackActionValue4 != mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aSort)
				{
					return;
				}
			}
		}
		mGAME.ProcessAttack03(tUserIndex, &tAttackInfo);
		return;

	case 4: //[Monster->Character]
		mGAME.ProcessAttack04(tUserIndex, &tAttackInfo);
		return;

	case 5: //[puncture]
		if ((tAttackInfo.mServerIndex1 == tUserIndex) && (tAttackInfo.mUniqueNumber1 == mAVATAR_OBJECT[tUserIndex].
			mUniqueNumber))
		{
			if (mUSER[tUserIndex].mCheckMaxAttackPacketNum == 1)
			{
				mUSER[tUserIndex].mNowAttackPacketNum++;
				if (mUSER[tUserIndex].mNowAttackPacketNum > mUSER[tUserIndex].mMaxAttackPacketNum)
				{
					return;
				}
				if (tAttackInfo.mAttackActionValue4 != mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aSort)
				{
					return;
				}
			}
		}
		mGAME.ProcessAttack05(tUserIndex, &tAttackInfo);
		return;

	case 6: //[Stun]
		if ((tAttackInfo.mServerIndex1 == tUserIndex) && (tAttackInfo.mUniqueNumber1 == mAVATAR_OBJECT[tUserIndex].
			mUniqueNumber))
		{
			if (mUSER[tUserIndex].mCheckMaxAttackPacketNum == 1)
			{
				mUSER[tUserIndex].mNowAttackPacketNum++;
				if (mUSER[tUserIndex].mNowAttackPacketNum > mUSER[tUserIndex].mMaxAttackPacketNum)
				{
					return;
				}
				if (tAttackInfo.mAttackActionValue4 != mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aSort)
				{
					return;
				}
			}
		}
		mGAME.ProcessAttack06(tUserIndex, &tAttackInfo);
		return;

	default:
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
	}
}

//W_PROCESS_DATA_SEND
void W_PROCESS_DATA_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int tSort;
	BYTE tData[MAX_BROADCAST_DATA_SIZE];
	CopyMemory(&tSort, &tPacket[1], 4);
	CopyMemory(&tData[0], &tPacket[5], MAX_BROADCAST_DATA_SIZE);

	mWORK.ProcessForData(tUserIndex, tSort, &tData[0]);
}

//W_GENERAL_CHAT_SEND
void W_GENERAL_CHAT_SEND(int tUserIndex)
{
	//unsigned int tRandom = 0;
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	char tContent[MAX_CHAT_CONTENT_LENGTH];
	CopyMemory(&tContent[0], &tPacket[1], MAX_CHAT_CONTENT_LENGTH);
	tContent[(MAX_CHAT_CONTENT_LENGTH - 1)] = '\0';
	if (strcmp(tContent, "") == 0)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int index01;
	float tPostLocation[3];
	int tSpaceIndex[3];
	float tLocation[3];
	BYTE tAuthType = 0;

	if (mUSER[tUserIndex].mAvatarInfo.aSpecialState == 2)
	{
		return;
	}
	tPostLocation[0] = mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0];
	tPostLocation[1] = mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[1];
	tPostLocation[2] = mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[2];
	tSpaceIndex[0] = static_cast<int>(tPostLocation[0] / MAX_RADIUS_FOR_NETWORK);
	tSpaceIndex[1] = static_cast<int>(tPostLocation[1] / MAX_RADIUS_FOR_NETWORK);
	tSpaceIndex[2] = static_cast<int>(tPostLocation[2] / MAX_RADIUS_FOR_NETWORK);

	if (1 == mUSER[tUserIndex].mAuthInfo.AuthType)
	{
		tAuthType = 1;
	}

	/*if (mUSER[tUserIndex].uUserSort == 999)
	{
		int index = atoi(tContent);
		for (index01 = 0; index01 < 20; index01++) {
			mUTIL.ProcessForDropItem(4, index, 0, 0, 0, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], "", 0);
		}
		return;
	}*/
	if (strcmp(tContent, "/explock") == 0)
	{
		mUSER[tUserIndex].mAuthInfo.Explock = 1;
		return;
	}
	if (strcmp(tContent, "/expunlock") == 0)
	{
		mUSER[tUserIndex].mAuthInfo.Explock = 0;
		return;
	}

	mTRANSFER.B_GENERAL_CHAT_RECV(&mUSER[tUserIndex].mAvatarInfo.aName[0], tAuthType, &tContent[0]);
	GameLogger(2, "<GENERAL_CHAT>(%s),(%s), (Message: %s),[[%d]]", mUSER[tUserIndex].uID,
	           mUSER[tUserIndex].mAvatarInfo.aName, tContent, mSERVER_INFO.mServerNumber);
	//mCHATLOG.SendToLogServer(mUSER[tUserIndex].mAvatarInfo.aTribe, 0, mUSER[tUserIndex].mAvatarInfo.aName, "", tContent);
	for (index01 = 0; index01 < mSERVER.mMAX_USER_NUM; index01++)
	{
		if (!mAVATAR_OBJECT[index01].mCheckValidState)
		{
			continue;
		}
		if (1 != mUSER[tUserIndex].mAuthInfo.AuthType)
		{
			if ((1 != mUSER[index01].mAuthInfo.AuthType) || (1 != mAVATAR_OBJECT[index01].mChat))
			{
				if ((mAVATAR_OBJECT[index01].mDATA.aTribe != mAVATAR_OBJECT[tUserIndex].mDATA.aTribe)
					&& (mAVATAR_OBJECT[index01].mDATA.aTribe != mGAME.ReturnAllianceTribe(
						mAVATAR_OBJECT[tUserIndex].mDATA.aTribe))
					&& (mSERVER_INFO.mServerNumber != ZONE_PRISON))
				{
					continue;
				}
			}
		}
		else
		{
			if (1 != mAVATAR_OBJECT[tUserIndex].mChat)
			{
				if ((mAVATAR_OBJECT[index01].mDATA.aTribe != mAVATAR_OBJECT[tUserIndex].mDATA.aTribe)
					&& (mAVATAR_OBJECT[index01].mDATA.aTribe != mGAME.ReturnAllianceTribe(
						mAVATAR_OBJECT[tUserIndex].mDATA.aTribe))
					&& (mSERVER_INFO.mServerNumber != ZONE_PRISON))
				{
					continue;
				}
			}
		}
		if ((1 != mUSER[tUserIndex].mAuthInfo.AuthType) && (1 != mUSER[index01].mAuthInfo.AuthType))
		{
			if ((mAVATAR_OBJECT[index01].mDATA.aTribe != mAVATAR_OBJECT[tUserIndex].mDATA.aTribe)
				&& (mAVATAR_OBJECT[index01].mDATA.aTribe != mGAME.ReturnAllianceTribe(
					mAVATAR_OBJECT[tUserIndex].mDATA.aTribe))
				&& (mSERVER_INFO.mServerNumber != ZONE_PRISON))
			{
				continue;
			}
		}
#ifdef __GOD__
		if ((mAVATAR_OBJECT[index01].mDATA.aTribe != mAVATAR_OBJECT[tUserIndex].mDATA.aTribe) && (mAVATAR_OBJECT[
			index01].mDATA.aTribe != mGAME.ReturnAllianceTribe(mAVATAR_OBJECT[tUserIndex].mDATA.aTribe)))
		{
			continue;
		}
#endif
		if ((abs(mAVATAR_OBJECT[index01].mSPACE_INDEX[0] - tSpaceIndex[0]) > 1) || (
			abs(mAVATAR_OBJECT[index01].mSPACE_INDEX[1] - tSpaceIndex[1]) > 1) || (abs(
			mAVATAR_OBJECT[index01].mSPACE_INDEX[2] - tSpaceIndex[2]) > 1))
		{
			continue;
		}
		tLocation[0] = mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0];
		tLocation[1] = mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[1];
		tLocation[2] = mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[2];
		if (((tLocation[0] - tPostLocation[0]) * (tLocation[0] - tPostLocation[0]) + (tLocation[1] - tPostLocation[1]) *
				(tLocation[1] - tPostLocation[1]) + (tLocation[2] - tPostLocation[2]) * (tLocation[2] - tPostLocation[
					2])) >
			(MAX_RADIUS_FOR_NETWORK * MAX_RADIUS_FOR_NETWORK))
		{
			continue;
		}
		mUSER[index01].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	}
}

//W_SECRET_CHAT_SEND
void W_SECRET_CHAT_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	char tAvatarName[MAX_AVATAR_NAME_LENGTH];
	char tContent[MAX_CHAT_CONTENT_LENGTH];
	CopyMemory(&tAvatarName[0], &tPacket[1], MAX_AVATAR_NAME_LENGTH);
	tAvatarName[(MAX_AVATAR_NAME_LENGTH - 1)] = '\0';
	CopyMemory(&tContent[0], &tPacket[(1 + MAX_AVATAR_NAME_LENGTH)], MAX_CHAT_CONTENT_LENGTH);
	tContent[(MAX_CHAT_CONTENT_LENGTH - 1)] = '\0';
	if (strcmp(tContent, "") == 0)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int tSort;
	BYTE tData[MAX_BROADCAST_DATA_SIZE];

	if (mUSER[tUserIndex].mAvatarInfo.aSpecialState == 2)
	{
		return;
	}
	if ((strncmp(mUSER[tUserIndex].mAvatarInfo.aName, tAvatarName, MAX_AVATAR_NAME_LENGTH) == 0)
		|| (strcmp(tAvatarName, "") == 0))
	{
		return;
	}
	mPLAYUSER_COM.U_FIND_AVATAR_FOR_ZONE_SEND(&tAvatarName[0]);
	if (mPLAYUSER_COM.mRecv_ZoneNumber < 1)
	{
		mTRANSFER.B_SECRET_CHAT_RECV(1, 0, &tAvatarName[0], &tContent[0], 0);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize, false);
		return;
	}
	if (mUSER[tUserIndex].mAuthInfo.AuthType == 0)
	{
		if ((mUSER[tUserIndex].mAvatarInfo.aTribe != mPLAYUSER_COM.mRecv_Tribe) && (mUSER[tUserIndex].mAvatarInfo.aTribe
			!= mGAME.ReturnAllianceTribe(mPLAYUSER_COM.mRecv_Tribe)))
		{
			mTRANSFER.B_SECRET_CHAT_RECV(2, 0, &tAvatarName[0], &tContent[0], 0);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize, false);
			return;
		}
	}
	mTRANSFER.B_SECRET_CHAT_RECV(0, mPLAYUSER_COM.mRecv_ZoneNumber, &tAvatarName[0], &tContent[0],
	                             mUSER[tUserIndex].mAuthInfo.AuthType);
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize, false);
	tSort = 103;
	CopyMemory(&tData[0], &mUSER[tUserIndex].mAvatarInfo.aName[0], MAX_AVATAR_NAME_LENGTH);
	CopyMemory(&tData[MAX_AVATAR_NAME_LENGTH], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
	CopyMemory(&tData[(MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH)], &tContent[0], MAX_CHAT_CONTENT_LENGTH);
	CopyMemory(&tData[(MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH + MAX_CHAT_CONTENT_LENGTH)],
	           &mUSER[tUserIndex].mAuthInfo.AuthType, 1);
	mRELAY_COM.U_DEMAND_BROADCAST_DATA(tSort, &tData[0]);
	//mCHATLOG.SendToLogServer( mUSER[tUserIndex].mAvatarInfo.aTribe, 4, mUSER[tUserIndex].mAvatarInfo.aName, tAvatarName, tContent );
}

//W_TRIBE_NOTICE_SEND
void W_TRIBE_NOTICE_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	char tContent[MAX_CHAT_CONTENT_LENGTH];
	CopyMemory(&tContent[0], &tPacket[1], MAX_CHAT_CONTENT_LENGTH);
	tContent[(MAX_CHAT_CONTENT_LENGTH - 1)] = '\0';
	if (strcmp(tContent, "") == 0)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int tSort;
	int FL = 1;
	int VFL = 2;
	BYTE tData[MAX_BROADCAST_DATA_SIZE];

	if (mUSER[tUserIndex].mAvatarInfo.aSpecialState == 2)
	{
		return;
	}
	if (mGAME.ReturnTribeRole(&mUSER[tUserIndex].mAvatarInfo.aName[0], mUSER[tUserIndex].mAvatarInfo.aTribe) == 1)
	{
		tSort = 113;
		CopyMemory(&tData[0], &mUSER[tUserIndex].mAvatarInfo.aName[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&tData[MAX_AVATAR_NAME_LENGTH], &tContent[0], MAX_CHAT_CONTENT_LENGTH);
		CopyMemory(&tData[(MAX_AVATAR_NAME_LENGTH + MAX_CHAT_CONTENT_LENGTH)], &mUSER[tUserIndex].mAvatarInfo.aTribe,
		           4);
		CopyMemory(&tData[(MAX_AVATAR_NAME_LENGTH + MAX_CHAT_CONTENT_LENGTH) + 4], &FL, 4);
		mRELAY_COM.U_DEMAND_BROADCAST_DATA(tSort, &tData[0]);
		return;
	}
	if (mGAME.ReturnTribeRole(&mUSER[tUserIndex].mAvatarInfo.aName[0], mUSER[tUserIndex].mAvatarInfo.aTribe) == 2)
	{
		tSort = 113;
		CopyMemory(&tData[0], &mUSER[tUserIndex].mAvatarInfo.aName[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&tData[MAX_AVATAR_NAME_LENGTH], &tContent[0], MAX_CHAT_CONTENT_LENGTH);
		CopyMemory(&tData[(MAX_AVATAR_NAME_LENGTH + MAX_CHAT_CONTENT_LENGTH)], &mUSER[tUserIndex].mAvatarInfo.aTribe,
		           4);
		CopyMemory(&tData[(MAX_AVATAR_NAME_LENGTH + MAX_CHAT_CONTENT_LENGTH) + 4], &VFL, 4);
		mRELAY_COM.U_DEMAND_BROADCAST_DATA(tSort, &tData[0]);
		return;
	}
	if ((mGAME.ReturnTribeRole(&mUSER[tUserIndex].mAvatarInfo.aName[0], mUSER[tUserIndex].mAvatarInfo.aTribe) != 1) ||
		(mGAME.ReturnTribeRole(&mUSER[tUserIndex].mAvatarInfo.aName[0], mUSER[tUserIndex].mAvatarInfo.aTribe) != 2))
	{
	}
}

//W_GENERAL_NOTICE_SEND
void W_GENERAL_NOTICE_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	char tContent[MAX_CHAT_CONTENT_LENGTH];
	CopyMemory(&tContent[0], &tPacket[1], MAX_CHAT_CONTENT_LENGTH);
	tContent[(MAX_CHAT_CONTENT_LENGTH - 1)] = '\0';
	if (mUSER[tUserIndex].mAuthInfo.AuthType == 0)
	{
		return;
	}
	if (mUSER[tUserIndex].mAuthInfo.NoticeFlag == 0)
	{
		return;
	}

	int tSort;
	BYTE tData[MAX_BROADCAST_DATA_SIZE];

	tSort = 102;
	CopyMemory(&tData[0], &tContent[0], MAX_CHAT_CONTENT_LENGTH);
	mRELAY_COM.U_DEMAND_BROADCAST_DATA(tSort, &tData[0]);
}

//W_TRIBE_CHAT_SEND
void W_TRIBE_CHAT_SEND(int tUserIndex)
{
	//unsigned int tRandom = 0;
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}
	//if (10 > mAVATAR_OBJECT[tUserIndex].mDATA.aLevel1)
	//{
	//	mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
	//	return;
	//}

	char tContent[MAX_CHAT_CONTENT_LENGTH];
	CopyMemory(&tContent[0], &tPacket[1], MAX_CHAT_CONTENT_LENGTH);
	tContent[(MAX_CHAT_CONTENT_LENGTH - 1)] = '\0';
	if (strcmp(tContent, "") == 0)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int index01;
	BYTE tAuthType = 0;

	if (mUSER[tUserIndex].mAvatarInfo.aSpecialState == 2)
	{
		return;
	}
	if (1 == mUSER[tUserIndex].mAuthInfo.AuthType)
		tAuthType = 1;

	mTRANSFER.B_TRIBE_CHAT_RECV(&mUSER[tUserIndex].mAvatarInfo.aName[0], tAuthType, &tContent[0]);
	GameLogger(2, "<TRIBE_CHAT>(%s),(%s), (Message: %s),[[%d]]", mUSER[tUserIndex].uID,
	           mUSER[tUserIndex].mAvatarInfo.aName, tContent, mSERVER_INFO.mServerNumber);
	//mCHATLOG.SendToLogServer(mUSER[tUserIndex].mAvatarInfo.aTribe, 2, mUSER[tUserIndex].mAvatarInfo.aName, "", tContent);
	for (index01 = 0; index01 < mSERVER.mMAX_USER_NUM; index01++)
	{
		if (!mAVATAR_OBJECT[index01].mCheckValidState)
		{
			continue;
		}
		if ((mAVATAR_OBJECT[index01].mDATA.aTribe != mAVATAR_OBJECT[tUserIndex].mDATA.aTribe) && (mAVATAR_OBJECT[
			index01].mDATA.aTribe != mGAME.ReturnAllianceTribe(mAVATAR_OBJECT[tUserIndex].mDATA.aTribe)))
		{
			continue;
		}
		mUSER[index01].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	}
}

//W_FACTION_NOTICE_SEND
void W_FACTION_NOTICE_SEND(int tUserIndex)
{
	//unsigned int tRandom = 0;
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}
	if (mUSER[tUserIndex].mAvatarInfo.aFactionNoticeScroll < 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	char tContent[MAX_CHAT_CONTENT_LENGTH];
	CopyMemory(&tContent[0], &tPacket[1], MAX_CHAT_CONTENT_LENGTH);
	tContent[(MAX_CHAT_CONTENT_LENGTH - 1)] = '\0';
	if (strcmp(tContent, "") == 0)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int index01;
	BYTE tAuthType = 0;
	int tSort;
	BYTE tData[MAX_BROADCAST_DATA_SIZE];

	mUSER[tUserIndex].mAvatarInfo.aFactionNoticeScroll--;
	mTRANSFER.B_AVATAR_CHANGE_INFO_2(69, mUSER[tUserIndex].mAvatarInfo.aFactionNoticeScroll);
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	tSort = 114;
	CopyMemory(&tData[0], &tAuthType, 4);
	CopyMemory(&tData[4], &mUSER[tUserIndex].mAvatarInfo.aName[0], MAX_AVATAR_NAME_LENGTH);
	CopyMemory(&tData[(4 + MAX_AVATAR_NAME_LENGTH)], &tContent, MAX_CHAT_CONTENT_LENGTH);
	CopyMemory(&tData[4 + MAX_AVATAR_NAME_LENGTH + MAX_CHAT_CONTENT_LENGTH], &mUSER[tUserIndex].mAvatarInfo.aTribe, 4);
	mRELAY_COM.U_DEMAND_BROADCAST_DATA(tSort, &tData[0]);
}

//W_TRADE_ASK_SEND
void W_TRADE_ASK_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	char tAvatarName[MAX_AVATAR_NAME_LENGTH];
	CopyMemory(&tAvatarName[0], &tPacket[1], MAX_AVATAR_NAME_LENGTH);
	tAvatarName[(MAX_AVATAR_NAME_LENGTH - 1)] = '\0';

	int tOtherAvatarIndex;
	if (1 == mUSER[tUserIndex].mAuthInfo.TradeFlag)
	{
		mTRANSFER.B_TRADE_ANSWER_RECV(6);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if (mAVATAR_OBJECT[tUserIndex].CheckCommunityWork())
	{
		mTRANSFER.B_TRADE_ANSWER_RECV(3);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if ((mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aSort == 11) || (mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aSort ==
		12))
	{
		mTRANSFER.B_TRADE_ANSWER_RECV(3);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	tOtherAvatarIndex = mUTIL.SearchAvatar(tAvatarName, tUserIndex);
	if (tOtherAvatarIndex == -1)
	{
		mTRANSFER.B_TRADE_ANSWER_RECV(4);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if ((mSERVER_INFO.mServerNumber != 37) && (mSERVER_INFO.mServerNumber != 119) && (mSERVER_INFO.mServerNumber !=
		124))
	{
		if ((mAVATAR_OBJECT[tUserIndex].mDATA.aTribe != mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aTribe) && (
			mAVATAR_OBJECT[tUserIndex].mDATA.aTribe != mGAME.ReturnAllianceTribe(
				mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aTribe)))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
	}
	if ((mUSER[tOtherAvatarIndex].mMoveZoneResult == 1) || (mAVATAR_OBJECT[tOtherAvatarIndex].CheckCommunityWork()))
	{
		mTRANSFER.B_TRADE_ANSWER_RECV(5);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if ((mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aAction.aSort == 11) || (mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.
		aAction.aSort == 12))
	{
		mTRANSFER.B_TRADE_ANSWER_RECV(5);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if (1 == mUSER[tOtherAvatarIndex].mAuthInfo.TradeFlag)
	{
		mTRANSFER.B_TRADE_ANSWER_RECV(7);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	mAVATAR_OBJECT[tUserIndex].mTradeProcessState = 1;
	strncpy(mAVATAR_OBJECT[tUserIndex].mTradeProcessAvatarName, mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aName,
	        MAX_AVATAR_NAME_LENGTH);
	mAVATAR_OBJECT[tOtherAvatarIndex].mTradeProcessState = 2;
	strncpy(mAVATAR_OBJECT[tOtherAvatarIndex].mTradeProcessAvatarName, mAVATAR_OBJECT[tUserIndex].mDATA.aName,
	        MAX_AVATAR_NAME_LENGTH);
	mTRANSFER.B_TRADE_ASK_RECV(&mUSER[tUserIndex].mAvatarInfo.aName[0], mUSER[tUserIndex].mAvatarInfo.aLevel1);
	mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_TRADE_CANCEL_SEND
void W_TRADE_CANCEL_SEND(int tUserIndex)
{
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	int tOtherAvatarIndex;

	if (mAVATAR_OBJECT[tUserIndex].mTradeProcessState != 1)
	{
		return;
	}
	mAVATAR_OBJECT[tUserIndex].mTradeProcessState = 0;
	tOtherAvatarIndex = mUTIL.SearchAvatar(mAVATAR_OBJECT[tUserIndex].mTradeProcessAvatarName, tUserIndex);
	if (tOtherAvatarIndex == -1)
	{
		return;
	}
	if (mAVATAR_OBJECT[tOtherAvatarIndex].mTradeProcessState != 2)
	{
		return;
	}
	if (strcmp(mAVATAR_OBJECT[tOtherAvatarIndex].mTradeProcessAvatarName, mAVATAR_OBJECT[tUserIndex].mDATA.aName) != 0)
	{
		return;
	}
	if (mUSER[tOtherAvatarIndex].mMoveZoneResult == 1)
	{
		return;
	}
	mAVATAR_OBJECT[tOtherAvatarIndex].mTradeProcessState = 0;
	mTRANSFER.B_TRADE_CANCEL_RECV();
	mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_TRADE_ANSWER_SEND
void W_TRADE_ANSWER_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	int tAnswer;
	CopyMemory(&tAnswer, &tPacket[1], 4);

	int tOtherAvatarIndex;

	if (mAVATAR_OBJECT[tUserIndex].mTradeProcessState != 2)
	{
		return;
	}
	switch (tAnswer)
	{
	case 0:
		mAVATAR_OBJECT[tUserIndex].mTradeProcessState = 3;
		break;
	case 1:
		mAVATAR_OBJECT[tUserIndex].mTradeProcessState = 0;
		break;
	case 2:
		mAVATAR_OBJECT[tUserIndex].mTradeProcessState = 0;
		break;
	default:
		return;
	}
	tOtherAvatarIndex = mUTIL.SearchAvatar(mAVATAR_OBJECT[tUserIndex].mTradeProcessAvatarName, tUserIndex);
	if (tOtherAvatarIndex == -1)
	{
		return;
	}
	if (mAVATAR_OBJECT[tOtherAvatarIndex].mTradeProcessState != 1)
	{
		return;
	}
	if (strcmp(mAVATAR_OBJECT[tOtherAvatarIndex].mTradeProcessAvatarName, mAVATAR_OBJECT[tUserIndex].mDATA.aName) != 0)
	{
		return;
	}
	if (mUSER[tOtherAvatarIndex].mMoveZoneResult == 1)
	{
		return;
	}
	switch (tAnswer)
	{
	case 0:
		mAVATAR_OBJECT[tOtherAvatarIndex].mTradeProcessState = 3;
		break;
	case 1:
		mAVATAR_OBJECT[tOtherAvatarIndex].mTradeProcessState = 0;
		break;
	case 2:
		mAVATAR_OBJECT[tOtherAvatarIndex].mTradeProcessState = 0;
		break;
	default:
		return;
	}

	if (1 == mUSER[tUserIndex].mAuthInfo.TradeFlag)
	{
		mAVATAR_OBJECT[tUserIndex].mTradeProcessState = 0;
		mAVATAR_OBJECT[tOtherAvatarIndex].mTradeProcessState = 0;
		mTRANSFER.B_TRADE_ANSWER_RECV(6);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mTRANSFER.B_TRADE_ANSWER_RECV(7);
		mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	}
	else if (1 == mUSER[tOtherAvatarIndex].mAuthInfo.TradeFlag)
	{
		mAVATAR_OBJECT[tUserIndex].mTradeProcessState = 0;
		mAVATAR_OBJECT[tOtherAvatarIndex].mTradeProcessState = 0;
		mTRANSFER.B_TRADE_ANSWER_RECV(7);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mTRANSFER.B_TRADE_ANSWER_RECV(6);
		mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	}
	else
	{
		mTRANSFER.B_TRADE_ANSWER_RECV(tAnswer);
		mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	}
}

//W_TRADE_START_SEND
void W_TRADE_START_SEND(int tUserIndex)
{
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	int tOtherAvatarIndex;

	if (mAVATAR_OBJECT[tUserIndex].mTradeProcessState != 3)
	{
		return;
	}
	tOtherAvatarIndex = mUTIL.SearchAvatar(mAVATAR_OBJECT[tUserIndex].mTradeProcessAvatarName, tUserIndex);
	if (tOtherAvatarIndex == -1)
	{
		mAVATAR_OBJECT[tUserIndex].mTradeProcessState = 0;
		return;
	}
	if (mAVATAR_OBJECT[tOtherAvatarIndex].mTradeProcessState != 3)
	{
		mAVATAR_OBJECT[tUserIndex].mTradeProcessState = 0;
		return;
	}
	if (strcmp(mAVATAR_OBJECT[tOtherAvatarIndex].mTradeProcessAvatarName, mAVATAR_OBJECT[tUserIndex].mDATA.aName) != 0)
	{
		mAVATAR_OBJECT[tUserIndex].mTradeProcessState = 0;
		return;
	}
	if (mUSER[tOtherAvatarIndex].mMoveZoneResult == 1)
	{
		mAVATAR_OBJECT[tUserIndex].mTradeProcessState = 0;
		return;
	}
	if (1 == mUSER[tUserIndex].mAuthInfo.TradeFlag)
	{
		mAVATAR_OBJECT[tUserIndex].mTradeProcessState = 0;
		mTRANSFER.B_TRADE_ANSWER_RECV(6);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mTRANSFER.B_TRADE_ANSWER_RECV(7);
		mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if (1 == mUSER[tOtherAvatarIndex].mAuthInfo.TradeFlag)
	{
		mAVATAR_OBJECT[tUserIndex].mTradeProcessState = 0;
		mTRANSFER.B_TRADE_ANSWER_RECV(7);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mTRANSFER.B_TRADE_ANSWER_RECV(6);
		mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}

	mAVATAR_OBJECT[tUserIndex].mTradeProcessState = 4;
	mAVATAR_OBJECT[tUserIndex].mTradeProcessMenu = 0;
	mTRANSFER.B_TRADE_START_RECV(mUSER[tOtherAvatarIndex].mAvatarInfo.aTradeMoney,
	                             mUSER[tOtherAvatarIndex].mAvatarInfo.aTrade,
	                             mUSER[tOtherAvatarIndex].mAvatarInfo.aTradeMoney2,
	                             mUSER[tOtherAvatarIndex].mAvatarInfo.aTradeSocket);
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	mAVATAR_OBJECT[tOtherAvatarIndex].mTradeProcessState = 4;
	mAVATAR_OBJECT[tOtherAvatarIndex].mTradeProcessMenu = 0;
	mTRANSFER.B_TRADE_START_RECV(mUSER[tUserIndex].mAvatarInfo.aTradeMoney, mUSER[tUserIndex].mAvatarInfo.aTrade,
	                             mUSER[tUserIndex].mAvatarInfo.aTradeMoney2,
	                             mUSER[tUserIndex].mAvatarInfo.aTradeSocket);
	mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_TRADE_MENU_SEND
void W_TRADE_MENU_SEND(int tUserIndex)
{
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	int index01;
	int tOtherAvatarIndex;
	int tTradeMoney;
	int tTradeMoney2;
	int tTrade[MAX_TRADE_SLOT_NUM][4];
	int tTradeSocket[MAX_TRADE_SLOT_NUM];

	if (mAVATAR_OBJECT[tUserIndex].mTradeProcessState != 4)
	{
		return;
	}
	switch (mAVATAR_OBJECT[tUserIndex].mTradeProcessMenu)
	{
	case 0:
		tOtherAvatarIndex = mUTIL.SearchAvatar(mAVATAR_OBJECT[tUserIndex].mTradeProcessAvatarName, tUserIndex);
		if (tOtherAvatarIndex == -1)
		{
			return;
		}
		if (mAVATAR_OBJECT[tOtherAvatarIndex].mTradeProcessState != 4)
		{
			return;
		}
		if (strcmp(mAVATAR_OBJECT[tOtherAvatarIndex].mTradeProcessAvatarName,
		           mAVATAR_OBJECT[tUserIndex].mDATA.aName) != 0)
		{
			return;
		}
		if (mUSER[tOtherAvatarIndex].mMoveZoneResult == 1)
		{
			return;
		}
		if (mAVATAR_OBJECT[tOtherAvatarIndex].mTradeProcessMenu == 2)
		{
			return;
		}
		if (1 == mUSER[tUserIndex].mAuthInfo.TradeFlag)
		{
			return;
		}
		if (1 == mUSER[tOtherAvatarIndex].mAuthInfo.TradeFlag)
		{
			return;
		}
		mAVATAR_OBJECT[tUserIndex].mTradeProcessMenu = 1;
		mTRANSFER.B_TRADE_MENU_RECV(0);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mTRANSFER.B_TRADE_MENU_RECV(1);
		mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	case 1:
		tOtherAvatarIndex = mUTIL.SearchAvatar(mAVATAR_OBJECT[tUserIndex].mTradeProcessAvatarName, tUserIndex);
		if (tOtherAvatarIndex == -1)
		{
			return;
		}
		if (mAVATAR_OBJECT[tOtherAvatarIndex].mTradeProcessState != 4)
		{
			return;
		}
		if (strcmp(mAVATAR_OBJECT[tOtherAvatarIndex].mTradeProcessAvatarName,
		           mAVATAR_OBJECT[tUserIndex].mDATA.aName) != 0)
		{
			return;
		}
		if (mUSER[tOtherAvatarIndex].mMoveZoneResult == 1)
		{
			return;
		}
		if (mAVATAR_OBJECT[tOtherAvatarIndex].mTradeProcessMenu == 0)
		{
			return;
		}
		if (1 == mUSER[tUserIndex].mAuthInfo.TradeFlag)
		{
			return;
		}
		if (1 == mUSER[tOtherAvatarIndex].mAuthInfo.TradeFlag)
		{
			return;
		}
		mAVATAR_OBJECT[tUserIndex].mTradeProcessMenu = 2;
		mTRANSFER.B_TRADE_MENU_RECV(0);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mTRANSFER.B_TRADE_MENU_RECV(1);
		mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		if (mAVATAR_OBJECT[tOtherAvatarIndex].mTradeProcessMenu != 2)
		{
			return;
		}
		tTradeMoney = mUSER[tUserIndex].mAvatarInfo.aTradeMoney;
		tTradeMoney2 = mUSER[tUserIndex].mAvatarInfo.aTradeMoney2;
		for (index01 = 0; index01 < MAX_TRADE_SLOT_NUM; index01++)
		{
			tTrade[index01][0] = mUSER[tUserIndex].mAvatarInfo.aTrade[index01][0];
			tTrade[index01][1] = mUSER[tUserIndex].mAvatarInfo.aTrade[index01][1];
			tTrade[index01][2] = mUSER[tUserIndex].mAvatarInfo.aTrade[index01][2];
			tTrade[index01][3] = mUSER[tUserIndex].mAvatarInfo.aTrade[index01][3];
			tTradeSocket[index01] = mUSER[tUserIndex].mAvatarInfo.aTradeSocket[index01];
		}
		if (mUSER[tOtherAvatarIndex].mAvatarInfo.aTradeMoney > 0)
		{
			GameLogger(1, "<TRADE_MONEY>((%s),(%s) -> (%s),(%s)), (Money: %d),[[%d]]", mUSER[tOtherAvatarIndex].uID,
			           mUSER[tOtherAvatarIndex].mAvatarInfo.aName, mUSER[tUserIndex].uID,
			           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tOtherAvatarIndex].mAvatarInfo.aTradeMoney,
			           mSERVER_INFO.mServerNumber);
			//mGAMELOG.GL_616_TRADE_MONEY(tOtherAvatarIndex, tUserIndex, mUSER[tOtherAvatarIndex].mAvatarInfo.aTradeMoney);
		}
		mUSER[tUserIndex].mAvatarInfo.aTradeMoney = mUSER[tOtherAvatarIndex].mAvatarInfo.aTradeMoney;
		mUSER[tUserIndex].mAvatarInfo.aTradeMoney2 = mUSER[tOtherAvatarIndex].mAvatarInfo.aTradeMoney2;
		for (index01 = 0; index01 < MAX_TRADE_SLOT_NUM; index01++)
		{
			if (mUSER[tOtherAvatarIndex].mAvatarInfo.aTrade[index01][0] > 0)
			{
				GameLogger(1, "<TRADE_ITEM>((%s),(%s) -> (%s),(%s)), (%d, %d, %d, %d),[[%d]]",
				           mUSER[tOtherAvatarIndex].uID, mUSER[tOtherAvatarIndex].mAvatarInfo.aName,
				           mUSER[tUserIndex].uID, mUSER[tUserIndex].mAvatarInfo.aName,
				           mUSER[tOtherAvatarIndex].mAvatarInfo.aTrade[index01][0],
				           mUSER[tOtherAvatarIndex].mAvatarInfo.aTrade[index01][1],
				           mUSER[tOtherAvatarIndex].mAvatarInfo.aTrade[index01][2],
				           mUSER[tOtherAvatarIndex].mAvatarInfo.aTrade[index01][3], mSERVER_INFO.mServerNumber);
				//mGAMELOG.GL_615_TRADE_ITEM(tOtherAvatarIndex, tUserIndex, mUSER[tOtherAvatarIndex].mAvatarInfo.aTrade[index01][0], mUSER[tOtherAvatarIndex].mAvatarInfo.aTrade[index01][1], mUSER[tOtherAvatarIndex].mAvatarInfo.aTrade[index01][2], mUSER[tOtherAvatarIndex].mAvatarInfo.aTrade[index01][3]);
			}
			mUSER[tUserIndex].mAvatarInfo.aTrade[index01][0] = mUSER[tOtherAvatarIndex].mAvatarInfo.aTrade[index01][0];
			mUSER[tUserIndex].mAvatarInfo.aTrade[index01][1] = mUSER[tOtherAvatarIndex].mAvatarInfo.aTrade[index01][1];
			mUSER[tUserIndex].mAvatarInfo.aTrade[index01][2] = mUSER[tOtherAvatarIndex].mAvatarInfo.aTrade[index01][2];
			mUSER[tUserIndex].mAvatarInfo.aTrade[index01][3] = mUSER[tOtherAvatarIndex].mAvatarInfo.aTrade[index01][3];
			mUSER[tUserIndex].mAvatarInfo.aTradeSocket[index01] = mUSER[tOtherAvatarIndex].mAvatarInfo.aTradeSocket[
				index01];
		}
		if (tTradeMoney > 0)
		{
			GameLogger(1, "<TRADE_MONEY>((%s),(%s) -> (%s),(%s)), (Money: %d),[[%d]]", mUSER[tUserIndex].uID,
			           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tOtherAvatarIndex].uID,
			           mUSER[tOtherAvatarIndex].mAvatarInfo.aName, tTradeMoney, mSERVER_INFO.mServerNumber);
			//mGAMELOG.GL_616_TRADE_MONEY(tUserIndex, tOtherAvatarIndex, tTradeMoney);
		}
		mUSER[tOtherAvatarIndex].mAvatarInfo.aTradeMoney = tTradeMoney;
		mUSER[tOtherAvatarIndex].mAvatarInfo.aTradeMoney2 = tTradeMoney2;
		for (index01 = 0; index01 < MAX_TRADE_SLOT_NUM; index01++)
		{
			if (tTrade[index01][0] > 0)
			{
				GameLogger(1, "<TRADE_ITEM>((%s),(%s) -> (%s),(%s)), (%d, %d, %d, %d),[[%d]]", mUSER[tUserIndex].uID,
				           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tOtherAvatarIndex].uID,
				           mUSER[tOtherAvatarIndex].mAvatarInfo.aName, tTrade[index01][0], tTrade[index01][1],
				           tTrade[index01][2], tTrade[index01][3], mSERVER_INFO.mServerNumber);
				//mGAMELOG.GL_615_TRADE_ITEM(tUserIndex, tOtherAvatarIndex, tTrade[index01][0], tTrade[index01][1], tTrade[index01][2], tTrade[index01][3]);
			}
			mUSER[tOtherAvatarIndex].mAvatarInfo.aTrade[index01][0] = tTrade[index01][0];
			mUSER[tOtherAvatarIndex].mAvatarInfo.aTrade[index01][1] = tTrade[index01][1];
			mUSER[tOtherAvatarIndex].mAvatarInfo.aTrade[index01][2] = tTrade[index01][2];
			mUSER[tOtherAvatarIndex].mAvatarInfo.aTrade[index01][3] = tTrade[index01][3];
			mUSER[tOtherAvatarIndex].mAvatarInfo.aTradeSocket[index01] = tTradeSocket[index01];
		}
		mAVATAR_OBJECT[tUserIndex].mTradeProcessState = 0;
		mTRANSFER.B_TRADE_END_RECV(0);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mAVATAR_OBJECT[tOtherAvatarIndex].mTradeProcessState = 0;
		mTRANSFER.B_TRADE_END_RECV(0);
		mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	}
}

//W_TRADE_END_SEND
void W_TRADE_END_SEND(int tUserIndex)
{
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	int tOtherAvatarIndex;

	if (mAVATAR_OBJECT[tUserIndex].mTradeProcessState != 4)
	{
		return;
	}
	mAVATAR_OBJECT[tUserIndex].mTradeProcessState = 0;
	mTRANSFER.B_TRADE_END_RECV(1);
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	tOtherAvatarIndex = mUTIL.SearchAvatar(mAVATAR_OBJECT[tUserIndex].mTradeProcessAvatarName, tUserIndex);
	if (tOtherAvatarIndex == -1)
	{
		return;
	}
	if (mAVATAR_OBJECT[tOtherAvatarIndex].mTradeProcessState != 4)
	{
		return;
	}
	if (strcmp(mAVATAR_OBJECT[tOtherAvatarIndex].mTradeProcessAvatarName, mAVATAR_OBJECT[tUserIndex].mDATA.aName) != 0)
	{
		return;
	}
	if (mUSER[tOtherAvatarIndex].mMoveZoneResult == 1)
	{
		return;
	}
	mAVATAR_OBJECT[tOtherAvatarIndex].mTradeProcessState = 0;
	mTRANSFER.B_TRADE_END_RECV(1);
	mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_FRIEND_ASK_SEND
void W_FRIEND_ASK_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}
	if (124 == mSERVER_INFO.mServerNumber)
		return;

	char tAvatarName[MAX_AVATAR_NAME_LENGTH];
	CopyMemory(&tAvatarName[0], &tPacket[1], MAX_AVATAR_NAME_LENGTH);
	tAvatarName[(MAX_AVATAR_NAME_LENGTH - 1)] = '\0';

	int index01;
	int tOtherAvatarIndex;

	for (index01 = 0; index01 < MAX_FRIEND_NUM; index01++)
	{
		if (strcmp(mUSER[tUserIndex].mAvatarInfo.aFriend[index01], "") == 0)
		{
			break;
		}
	}
	if (index01 == MAX_FRIEND_NUM)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	for (index01 = 0; index01 < MAX_FRIEND_NUM; index01++)
	{
		if (strcmp(mUSER[tUserIndex].mAvatarInfo.aFriend[index01], tAvatarName) == 0)
		{
			break;
		}
	}
	if (index01 < MAX_FRIEND_NUM)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mAVATAR_OBJECT[tUserIndex].CheckCommunityWork())
	{
		mTRANSFER.B_FRIEND_ANSWER_RECV(3);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if ((mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aSort == 11) || (mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aSort ==
		12))
	{
		mTRANSFER.B_FRIEND_ANSWER_RECV(3);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	tOtherAvatarIndex = mUTIL.SearchAvatar(tAvatarName, tUserIndex);
	if (tOtherAvatarIndex == -1)
	{
		mTRANSFER.B_FRIEND_ANSWER_RECV(4);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if (mAVATAR_OBJECT[tUserIndex].mDATA.aTribe != mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aTribe)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if ((mUSER[tOtherAvatarIndex].mMoveZoneResult == 1) || (mAVATAR_OBJECT[tOtherAvatarIndex].CheckCommunityWork()))
	{
		mTRANSFER.B_FRIEND_ANSWER_RECV(5);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if ((mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aAction.aSort == 11) || (mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.
		aAction.aSort == 12))
	{
		mTRANSFER.B_FRIEND_ANSWER_RECV(5);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	mAVATAR_OBJECT[tUserIndex].mFriendProcessState = 1;
	strncpy(mAVATAR_OBJECT[tUserIndex].mFriendProcessAvatarName, mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aName,
	        MAX_AVATAR_NAME_LENGTH);
	mAVATAR_OBJECT[tOtherAvatarIndex].mFriendProcessState = 2;
	strncpy(mAVATAR_OBJECT[tOtherAvatarIndex].mFriendProcessAvatarName, mAVATAR_OBJECT[tUserIndex].mDATA.aName,
	        MAX_AVATAR_NAME_LENGTH);
	mTRANSFER.B_FRIEND_ASK_RECV(&mUSER[tUserIndex].mAvatarInfo.aName[0]);
	mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_FRIEND_CANCEL_SEND
void W_FRIEND_CANCEL_SEND(int tUserIndex)
{
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	int tOtherAvatarIndex;

	if (mAVATAR_OBJECT[tUserIndex].mFriendProcessState != 1)
	{
		return;
	}
	mAVATAR_OBJECT[tUserIndex].mFriendProcessState = 0;
	tOtherAvatarIndex = mUTIL.SearchAvatar(mAVATAR_OBJECT[tUserIndex].mFriendProcessAvatarName, tUserIndex);
	if (tOtherAvatarIndex == -1)
	{
		return;
	}
	if (mAVATAR_OBJECT[tOtherAvatarIndex].mFriendProcessState != 2)
	{
		return;
	}
	if (strcmp(mAVATAR_OBJECT[tOtherAvatarIndex].mFriendProcessAvatarName, mAVATAR_OBJECT[tUserIndex].mDATA.aName) != 0)
	{
		return;
	}
	if (mUSER[tOtherAvatarIndex].mMoveZoneResult == 1)
	{
		return;
	}
	mAVATAR_OBJECT[tOtherAvatarIndex].mFriendProcessState = 0;
	mTRANSFER.B_FRIEND_CANCEL_RECV();
	mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_FRIEND_ANSWER_SEND
void W_FRIEND_ANSWER_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	int tAnswer;
	CopyMemory(&tAnswer, &tPacket[1], 4);

	int tOtherAvatarIndex;

	if (mAVATAR_OBJECT[tUserIndex].mFriendProcessState != 2)
	{
		return;
	}
	switch (tAnswer)
	{
	case 0:
		mAVATAR_OBJECT[tUserIndex].mFriendProcessState = 3;
		break;
	case 1:
		mAVATAR_OBJECT[tUserIndex].mFriendProcessState = 0;
		break;
	case 2:
		mAVATAR_OBJECT[tUserIndex].mFriendProcessState = 0;
		break;
	default:
		return;
	}
	tOtherAvatarIndex = mUTIL.SearchAvatar(mAVATAR_OBJECT[tUserIndex].mFriendProcessAvatarName, tUserIndex);
	if (tOtherAvatarIndex == -1)
	{
		return;
	}
	if (mAVATAR_OBJECT[tOtherAvatarIndex].mFriendProcessState != 1)
	{
		return;
	}
	if (strcmp(mAVATAR_OBJECT[tOtherAvatarIndex].mFriendProcessAvatarName, mAVATAR_OBJECT[tUserIndex].mDATA.aName) != 0)
	{
		return;
	}
	if (mUSER[tOtherAvatarIndex].mMoveZoneResult == 1)
	{
		return;
	}
	switch (tAnswer)
	{
	case 0:
		mAVATAR_OBJECT[tOtherAvatarIndex].mFriendProcessState = 3;
		break;
	case 1:
		mAVATAR_OBJECT[tOtherAvatarIndex].mFriendProcessState = 0;
		break;
	case 2:
		mAVATAR_OBJECT[tOtherAvatarIndex].mFriendProcessState = 0;
		break;
	default:
		return;
	}
	mTRANSFER.B_FRIEND_ANSWER_RECV(tAnswer);
	mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_FRIEND_MAKE_SEND
void W_FRIEND_MAKE_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	int tIndex;
	CopyMemory(&tIndex, &tPacket[1], 4);
	if ((tIndex < 0) || (tIndex > (MAX_FRIEND_NUM - 1)))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int tOtherAvatarIndex;

	if (strcmp(mUSER[tUserIndex].mAvatarInfo.aFriend[tIndex], "") != 0)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mAVATAR_OBJECT[tUserIndex].mFriendProcessState != 3)
	{
		return;
	}
	tOtherAvatarIndex = mUTIL.SearchAvatar(mAVATAR_OBJECT[tUserIndex].mFriendProcessAvatarName, tUserIndex);
	if (tOtherAvatarIndex == -1)
	{
		mAVATAR_OBJECT[tUserIndex].mFriendProcessState = 0;
		return;
	}
	if (mAVATAR_OBJECT[tOtherAvatarIndex].mFriendProcessState != 3)
	{
		mAVATAR_OBJECT[tUserIndex].mFriendProcessState = 0;
		return;
	}
	if (strcmp(mAVATAR_OBJECT[tOtherAvatarIndex].mFriendProcessAvatarName, mAVATAR_OBJECT[tUserIndex].mDATA.aName) != 0)
	{
		mAVATAR_OBJECT[tUserIndex].mFriendProcessState = 0;
		return;
	}
	if (mUSER[tOtherAvatarIndex].mMoveZoneResult == 1)
	{
		mAVATAR_OBJECT[tUserIndex].mFriendProcessState = 0;
		return;
	}
	mAVATAR_OBJECT[tUserIndex].mFriendProcessState = 0;
	strncpy(mUSER[tUserIndex].mAvatarInfo.aFriend[tIndex], mUSER[tOtherAvatarIndex].mAvatarInfo.aName,
	        MAX_AVATAR_NAME_LENGTH);
	mTRANSFER.B_FRIEND_MAKE_RECV(tIndex, &mUSER[tOtherAvatarIndex].mAvatarInfo.aName[0]);
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	mAVATAR_OBJECT[tOtherAvatarIndex].mFriendProcessState = 0;
}

//W_FRIEND_FIND_SEND
void W_FRIEND_FIND_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	int tIndex;
	CopyMemory(&tIndex, &tPacket[1], 4);
	if ((tIndex < 0) || (tIndex > (MAX_FRIEND_NUM - 1)))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	if (strcmp(mUSER[tUserIndex].mAvatarInfo.aFriend[tIndex], "") == 0)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	mPLAYUSER_COM.U_FIND_AVATAR_FOR_ZONE_SEND(&mUSER[tUserIndex].mAvatarInfo.aFriend[tIndex][0]);
	if (mUSER[tUserIndex].mAvatarInfo.aTribe != mPLAYUSER_COM.mRecv_Tribe)
	{
		mTRANSFER.B_FRIEND_FIND_RECV(tIndex, -1);
	}
	else
	{
		mTRANSFER.B_FRIEND_FIND_RECV(tIndex, mPLAYUSER_COM.mRecv_ZoneNumber);
	}
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_FRIEND_DELETE_SEND
void W_FRIEND_DELETE_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	int tIndex;
	CopyMemory(&tIndex, &tPacket[1], 4);
	if ((tIndex < 0) || (tIndex > (MAX_FRIEND_NUM - 1)))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	if (strcmp(mUSER[tUserIndex].mAvatarInfo.aFriend[tIndex], "") == 0)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	strcpy(mUSER[tUserIndex].mAvatarInfo.aFriend[tIndex], "");
	mTRANSFER.B_FRIEND_DELETE_RECV(tIndex);
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

// W_COSTUME_STATE_RECV
void W_COSTUME_STATE_RECV(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	ITEM_INFO* pItem = mITEM.
		Search(mUSER[tUserIndex].mAvatarInfo.aCostume[mUSER[tUserIndex].mAvatarInfo.aCostumeIndex]);
	int tValue01;
	int tValue02;
	int index01;
	CopyMemory(&tValue01, &tPacket[1], 4);
	CopyMemory(&tValue02, &tPacket[5], 4);

	int iPage = 0, iInvenIndex = 0, iInvenSlot = 0;
	int iItemCounts = 0;

	switch (tValue01)
	{
	case 1: //[SELECT]
		if ((tValue02 < 0) || (tValue02 > (MAX_AVATAR_ANIMAL_NUM - 1)))
		{
			return;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aCostume[tValue02] == 0)
		{
			return;
		}
		if ((mUSER[tUserIndex].mAvatarInfo.aCostumeIndex >= MAX_AVATAR_ANIMAL_NUM) && (mUSER[tUserIndex].mAvatarInfo.
			aCostumeIndex <= (MAX_AVATAR_ANIMAL_NUM + MAX_AVATAR_ANIMAL_NUM - 1)))
		{
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aCostumeIndex = tValue02;
		mTRANSFER.B_COSTUME_STATE_RECV(0, tValue01, tValue02, 0, 0, 0, 0);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 2: //[CANCEL SELECT]
		if ((mUSER[tUserIndex].mAvatarInfo.aCostumeIndex >= MAX_AVATAR_ANIMAL_NUM) && (mUSER[tUserIndex].mAvatarInfo.
			aCostumeIndex <= (MAX_AVATAR_ANIMAL_NUM + MAX_AVATAR_ANIMAL_NUM - 1)))
		{
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aCostumeIndex = -1;
		mTRANSFER.B_COSTUME_STATE_RECV(0, tValue01, tValue02, 0, 0, 0, 0);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 3: //[SUMMON]
		if ((mUSER[tUserIndex].mAvatarInfo.aCostumeIndex < 0) || (mUSER[tUserIndex].mAvatarInfo.aCostumeIndex > (
			MAX_AVATAR_ANIMAL_NUM - 1)))
		{
			return;
		}
		if (mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aSort != 1)
		{
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aCostumeIndex += MAX_AVATAR_ANIMAL_NUM;
		mTRANSFER.B_COSTUME_STATE_RECV(0, tValue01, tValue02, 0, 0, 0, 0);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mAVATAR_OBJECT[tUserIndex].mDATA.aFashionNumber = mUSER[tUserIndex].mAvatarInfo.aCostume[(mUSER[tUserIndex].
			mAvatarInfo.aCostumeIndex - MAX_AVATAR_ANIMAL_NUM)];
		mTRANSFER.B_AVATAR_CHANGE_INFO_1(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 16,
		                                 mAVATAR_OBJECT[tUserIndex].mDATA.aFashionNumber, 0, 0);
		mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], 1);
		mUSER[tUserIndex].mTickCountFor01Minute_2 = mGAME.mTickCount;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		return;

	case 4: //[UNSUMMON]
		if ((mUSER[tUserIndex].mAvatarInfo.aCostumeIndex < MAX_AVATAR_ANIMAL_NUM) || (mUSER[tUserIndex].mAvatarInfo.
			aCostumeIndex > (MAX_AVATAR_ANIMAL_NUM + MAX_AVATAR_ANIMAL_NUM - 1)))
		{
			return;
		}

		mUSER[tUserIndex].mAvatarInfo.aCostumeIndex -= MAX_AVATAR_ANIMAL_NUM;
		mTRANSFER.B_COSTUME_STATE_RECV(0, tValue01, tValue02, 0, 0, 0, 0);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mAVATAR_OBJECT[tUserIndex].mDATA.aFashionNumber = 0;
		mTRANSFER.B_AVATAR_CHANGE_INFO_1(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 17, 0, 0, 0);
		mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], 1);
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		return;

	case 5: //[DELETE]
		if (!mUTIL.FindEmptyInvenForItem(tUserIndex, pItem, iPage, iInvenIndex))
		{
			mTRANSFER.B_COSTUME_STATE_RECV(2, tValue01, tValue02, 0, 0, 0, 0);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		if (mUTIL.FindEmptyInvenForItem(tUserIndex, pItem, iPage, iInvenIndex))
		{
			iInvenSlot = mUTIL.GetEmptyInvenSlotIndex(tUserIndex, iPage);
			if (iInvenSlot != -1)
			{
				mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iInvenSlot][0] = pItem->iIndex;
				mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iInvenSlot][1] = iInvenIndex % 8;
				mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iInvenSlot][2] = iInvenIndex / 8;
				mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iInvenSlot][3] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iInvenSlot][4] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iInvenSlot][5] = 0;
			}
		}
		mUSER[tUserIndex].mAvatarInfo.aCostume[mUSER[tUserIndex].mAvatarInfo.aCostumeIndex] = 0;
		for (index01 = mUSER[tUserIndex].mAvatarInfo.aCostumeIndex; index01 < 10; index01++)
		{
			mUSER[tUserIndex].mAvatarInfo.aCostume[index01] = mUSER[tUserIndex].mAvatarInfo.aCostume[index01 + 1];
		}
		mUSER[tUserIndex].mAvatarInfo.aCostumeIndex = -1;
		mTRANSFER.B_COSTUME_STATE_RECV(0, tValue01, tValue02, iPage, iInvenSlot, iInvenIndex, pItem->iIndex);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mAVATAR_OBJECT[tUserIndex].mDATA.aFashionNumber = 0;
		mTRANSFER.B_AVATAR_CHANGE_INFO_1(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 16,
		                                 mAVATAR_OBJECT[tUserIndex].mDATA.aFashionNumber, 0, 0);
		mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], 1);
		return;

	default:
		mTRANSFER.B_COSTUME_STATE_RECV(1, tValue01, tValue02, 0, 0, 0, 0);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
	}
}

//W_ABSORB_STATE_SEND
void W_ABSORB_STATE_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mAvatarInfo.aMountAbsorbTime <= 0)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mAvatarInfo.aAnimalTime <= 0)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}


	int tValue01;
	int tValue02;
	CopyMemory(&tValue01, &tPacket[1], 4);
	CopyMemory(&tValue02, &tPacket[5], 4);

	switch (tValue01)
	{
	case 1:
		mUSER[tUserIndex].mAvatarInfo.aAbsorbState = 1;
		mTRANSFER.DEMAND_OBJECT_RECV(79, mUSER[tUserIndex].mAvatarInfo.aAbsorbState);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mAVATAR_OBJECT[tUserIndex].mDATA.aAnimalOn = mUSER[tUserIndex].mAvatarInfo.aAbsorbState;
		mTRANSFER.B_AVATAR_CHANGE_INFO_1(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 26,
		                                 mAVATAR_OBJECT[tUserIndex].mDATA.aAnimalOn, 0, 0);
		mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], 1);
		mUSER[tUserIndex].mTickCountFor01Minute_2 = mGAME.mTickCount;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		return;

	case 2:
		mUSER[tUserIndex].mAvatarInfo.aAbsorbState = 0;
		mTRANSFER.DEMAND_OBJECT_RECV(79, mUSER[tUserIndex].mAvatarInfo.aAbsorbState);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mAVATAR_OBJECT[tUserIndex].mDATA.aAnimalOn = mUSER[tUserIndex].mAvatarInfo.aAbsorbState;
		mTRANSFER.B_AVATAR_CHANGE_INFO_1(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 26,
		                                 mAVATAR_OBJECT[tUserIndex].mDATA.aAnimalOn, 0, 0);
		mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], 1);
		mUSER[tUserIndex].mTickCountFor01Minute_2 = mGAME.mTickCount;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		return;

	default:
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
	}
}

//W_ANIMAL_STATE_SEND
void W_ANIMAL_STATE_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int tValue01;
	int tValue02;
	int tRandomAbility[8];
	int tRandom;
	CopyMemory(&tValue01, &tPacket[1], 4);
	CopyMemory(&tValue02, &tPacket[5], 4);

	switch (tValue01)
	{
	case 1: //[SELECT]
		if ((tValue02 < 0) || (tValue02 > (MAX_AVATAR_ANIMAL_NUM - 1)))
		{
			return;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aAnimal[tValue02] == 0)
		{
			return;
		}
		if ((mUSER[tUserIndex].mAvatarInfo.aAnimalIndex >= MAX_AVATAR_ANIMAL_NUM) && (mUSER[tUserIndex].mAvatarInfo.
			aAnimalIndex <= (MAX_AVATAR_ANIMAL_NUM + MAX_AVATAR_ANIMAL_NUM - 1)))
		{
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aAnimalIndex = tValue02;
		mTRANSFER.B_ANIMAL_STATE_RECV(tValue01, tValue02);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 2: //[CANCEL SELECT]
		if ((mUSER[tUserIndex].mAvatarInfo.aAnimalIndex >= MAX_AVATAR_ANIMAL_NUM) && (mUSER[tUserIndex].mAvatarInfo.
			aAnimalIndex <= (MAX_AVATAR_ANIMAL_NUM + MAX_AVATAR_ANIMAL_NUM - 1)))
		{
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aAnimalIndex = -1;
		mTRANSFER.B_ANIMAL_STATE_RECV(tValue01, tValue02);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 3: //[SUMMON]
		if ((mUSER[tUserIndex].mAvatarInfo.aAnimalIndex < 0) || (mUSER[tUserIndex].mAvatarInfo.aAnimalIndex > (
			MAX_AVATAR_ANIMAL_NUM - 1)))
		{
			return;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aAnimalTime < 1)
		{
			return;
		}
		if (mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aSort != 1)
		{
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aAnimalIndex += MAX_AVATAR_ANIMAL_NUM;
		mTRANSFER.B_ANIMAL_STATE_RECV(tValue01, tValue02);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mAVATAR_OBJECT[tUserIndex].mDATA.aAnimalNumber = mUSER[tUserIndex].mAvatarInfo.aAnimal[(mUSER[tUserIndex].
			mAvatarInfo.aAnimalIndex - MAX_AVATAR_ANIMAL_NUM)];
		mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aFrame = 0.0f;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		if (mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue > mAVATAR_OBJECT[tUserIndex].mDATA.aMaxLifeValue)
		{
			mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue = mAVATAR_OBJECT[tUserIndex].mDATA.aMaxLifeValue;
		}
		if (mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue > mAVATAR_OBJECT[tUserIndex].mDATA.aMaxManaValue)
		{
			mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue = mAVATAR_OBJECT[tUserIndex].mDATA.aMaxManaValue;
		}
		mTRANSFER.B_AVATAR_CHANGE_INFO_1(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 12,
		                                 mAVATAR_OBJECT[tUserIndex].mDATA.aAnimalNumber, 0, 0);
		mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], 1);
		mUSER[tUserIndex].mTickCountFor01Minute_2 = mGAME.mTickCount;
		mAVATAR_OBJECT[tUserIndex].mDATA.aAnimalOn = 0;
		return;

	case 4: //[UNSUMMON]
		if ((mUSER[tUserIndex].mAvatarInfo.aAnimalIndex < MAX_AVATAR_ANIMAL_NUM) || (mUSER[tUserIndex].mAvatarInfo.
			aAnimalIndex > (MAX_AVATAR_ANIMAL_NUM + MAX_AVATAR_ANIMAL_NUM - 1)))
		{
			return;
		}

		mUSER[tUserIndex].mAvatarInfo.aAnimalIndex -= MAX_AVATAR_ANIMAL_NUM;
		mTRANSFER.B_ANIMAL_STATE_RECV(tValue01, tValue02);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mAVATAR_OBJECT[tUserIndex].mDATA.aAnimalNumber = 0;
		mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aFrame = 0.0f;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		if (mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue > mAVATAR_OBJECT[tUserIndex].mDATA.aMaxLifeValue)
		{
			mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue = mAVATAR_OBJECT[tUserIndex].mDATA.aMaxLifeValue;
		}
		if (mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue > mAVATAR_OBJECT[tUserIndex].mDATA.aMaxManaValue)
		{
			mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue = mAVATAR_OBJECT[tUserIndex].mDATA.aMaxManaValue;
		}
		mTRANSFER.B_AVATAR_CHANGE_INFO_1(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 12, 0, 0, 0);
		mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], 1);

		if ((mUSER[tUserIndex].mAvatarInfo.aAbsorbState) == 1)
		{
			mUSER[tUserIndex].mAvatarInfo.aAbsorbState = 0;
			mTRANSFER.DEMAND_OBJECT_RECV(79, mUSER[tUserIndex].mAvatarInfo.aAbsorbState);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			mAVATAR_OBJECT[tUserIndex].mDATA.aAnimalOn = 0;
			mTRANSFER.B_AVATAR_CHANGE_INFO_1(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 26,
			                                 mAVATAR_OBJECT[tUserIndex].mDATA.aAnimalOn, 0, 0);
			mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], 1);
			mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		}
		return;

	case 5: //[DELETE]
		mUSER[tUserIndex].mAvatarInfo.aAnimal[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex] = 0;
		mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex] = 0;
		mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex] = 0;
		mTRANSFER.B_ANIMAL_STATE_RECV(tValue01, tValue02);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 500;
		mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 500;
		mTRANSFER.B_AVATAR_CHANGE_INFO_1(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 3,
		                                 mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe, 0, 0);
		mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], 1);
		return;

	case 6: //[Mount Exp -> Stats]
		if ((mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] %
			1000000) < 100000)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (((mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] %
			100000000) / 10000000) + ((mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.
			aAnimalIndex - 10] % 10000000) / 1000000) + ((mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[
			tUserIndex].mAvatarInfo.aAnimalIndex - 10] % 1000000) / 100000) + ((mUSER[tUserIndex].mAvatarInfo.
			aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] % 100000) / 10000) + ((mUSER[
				tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] % 10000) /
			1000) + ((mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10]
			%
			1000) / 100) + ((mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.
			aAnimalIndex - 10] % 100) / 10) + ((mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].
			mAvatarInfo.aAnimalIndex - 10] % 10) / 1) > 25)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		tRandomAbility[0] = 1;
		tRandomAbility[1] = 10;
		tRandomAbility[2] = 100;
		tRandomAbility[3] = 1000;
		tRandomAbility[4] = 10000;
		tRandomAbility[5] = 100000;
		tRandomAbility[6] = 1000000;
		tRandomAbility[7] = 10000000;
		tRandom = rand() % 8;
		switch (tRandom)
		{
		case 0:
			{
				if ((mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10]
					% 10) < 9)
				{
					mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] -=
						100000;
					mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10]
						+= tRandomAbility[tRandom];
					mTRANSFER.B_ANIMAL_STATE_RECV(
						tValue01,
						mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex -
							10]);
					mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
					return;
				}
				mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] += 0;
				mTRANSFER.B_ANIMAL_STATE_RECV(
					tValue01,
					mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10]);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				mTRANSFER.B_AVATAR_CHANGE_INFO_2(
					71, mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex -
						10]);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				return;
			}
			break;

		case 1:
			{
				if (((mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10]
					% 100) / 10) < 9)
				{
					mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] -=
						100000;
					mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10]
						+= tRandomAbility[tRandom];
					mTRANSFER.B_ANIMAL_STATE_RECV(
						tValue01,
						mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex -
							10]);
					mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
					return;
				}
				mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] += 0;
				mTRANSFER.B_ANIMAL_STATE_RECV(
					tValue01,
					mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10]);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				mTRANSFER.B_AVATAR_CHANGE_INFO_2(
					71, mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex -
						10]);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				return;
			}
			break;

		case 2:
			{
				if (((mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10]
					% 1000) / 100) < 9)
				{
					mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] -=
						100000;
					mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10]
						+= tRandomAbility[tRandom];
					mTRANSFER.B_ANIMAL_STATE_RECV(
						tValue01,
						mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex -
							10]);
					mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
					return;
				}
				mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] += 0;
				mTRANSFER.B_ANIMAL_STATE_RECV(
					tValue01,
					mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10]);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				mTRANSFER.B_AVATAR_CHANGE_INFO_2(
					71, mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex -
						10]);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				return;
			}
			break;

		case 3:
			{
				if (((mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10]
					% 10000) / 1000) < 9)
				{
					mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] -=
						100000;
					mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10]
						+= tRandomAbility[tRandom];
					mTRANSFER.B_ANIMAL_STATE_RECV(
						tValue01,
						mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex -
							10]);
					mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
					return;
				}
				mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] += 0;
				mTRANSFER.B_ANIMAL_STATE_RECV(
					tValue01,
					mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10]);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				mTRANSFER.B_AVATAR_CHANGE_INFO_2(
					71, mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex -
						10]);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				return;
			}
			break;

		case 4:
			{
				if (((mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10]
					% 100000) / 10000) < 9)
				{
					mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] -=
						100000;
					mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10]
						+= tRandomAbility[tRandom];
					mTRANSFER.B_ANIMAL_STATE_RECV(
						tValue01,
						mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex -
							10]);
					mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
					return;
				}
				mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] += 0;
				mTRANSFER.B_ANIMAL_STATE_RECV(
					tValue01,
					mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10]);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				mTRANSFER.B_AVATAR_CHANGE_INFO_2(
					71, mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex -
						10]);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				return;
			}
			break;

		case 5:
			{
				if (((mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10]
					% 1000000) / 100000) < 9)
				{
					mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] -=
						100000;
					mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10]
						+= tRandomAbility[tRandom];
					mTRANSFER.B_ANIMAL_STATE_RECV(
						tValue01,
						mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex -
							10]);
					mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
					return;
				}
				mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] += 0;
				mTRANSFER.B_ANIMAL_STATE_RECV(
					tValue01,
					mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10]);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				mTRANSFER.B_AVATAR_CHANGE_INFO_2(
					71, mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex -
						10]);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				return;
			}
			break;

		case 6:
			{
				if (((mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10]
					% 10000000) / 1000000) < 9)
				{
					mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] -=
						100000;
					mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10]
						+= tRandomAbility[tRandom];
					mTRANSFER.B_ANIMAL_STATE_RECV(
						tValue01,
						mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex -
							10]);
					mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
					return;
				}
				mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] += 0;
				mTRANSFER.B_ANIMAL_STATE_RECV(
					tValue01,
					mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10]);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				mTRANSFER.B_AVATAR_CHANGE_INFO_2(
					71, mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex -
						10]);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				return;
			}
			break;

		case 7:
			{
				if (((mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10]
					% 100000000) / 10000000) < 9)
				{
					mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] -=
						100000;
					mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10]
						+= tRandomAbility[tRandom];
					mTRANSFER.B_ANIMAL_STATE_RECV(
						tValue01,
						mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex -
							10]);
					mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
					return;
				}
				mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] += 0;
				mTRANSFER.B_ANIMAL_STATE_RECV(
					tValue01,
					mUSER[tUserIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10]);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				mTRANSFER.B_AVATAR_CHANGE_INFO_2(
					71, mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex -
						10]);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				return;
			}
			break;
		}
		return;

	default:
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
	}
}

//W_START_PSHOP_SEND
void W_START_PSHOP_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (124 == mSERVER_INFO.mServerNumber)
		return;

	PSHOP_INFO tPShopInfo;
	CopyMemory(&tPShopInfo, &tPacket[5], sizeof(PSHOP_INFO));

	if (1 == mUSER[tUserIndex].mAuthInfo.PShopFlag)
	{
		mTRANSFER.B_START_PSHOP_RECV(1, &tPShopInfo);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}

	int index01;
	int index02;
	ITEM_INFO* tITEM_INFO;

	if (mAVATAR_OBJECT[tUserIndex].mDATA.aPShopState != 0)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (!mWORK.CheckPossiblePShopRegion(mUSER[tUserIndex].mAvatarInfo.aTribe, mSERVER_INFO.mServerNumber,
	                                    &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0]))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aSort != 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (strcmp(tPShopInfo.mName, "") == 0)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	for (index01 = 0; index01 < MAX_PSHOP_PAGE_NUM; index01++)
	{
		for (index02 = 0; index02 < MAX_PSHOP_SLOT_NUM; index02++)
		{
			if (tPShopInfo.mItemInfo[index01][index02][0] > 0)
			{
				break;
			}
		}
		if (index02 < MAX_PSHOP_SLOT_NUM)
		{
			break;
		}
	}
	/*if (index01 == MAX_PSHOP_PAGE_NUM)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}*/
	for (index01 = 0; index01 < MAX_PSHOP_PAGE_NUM; index01++)
	{
		for (index02 = 0; index02 < MAX_PSHOP_SLOT_NUM; index02++)
		{
			if (tPShopInfo.mItemInfo[index01][index02][0] < 1)
			{
				continue;
			}
			tITEM_INFO = mITEM.Search(tPShopInfo.mItemInfo[index01][index02][0]);
			if (tITEM_INFO == nullptr)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			if (tITEM_INFO->iCheckAvatarShop == 1)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			if ((tPShopInfo.mItemInfo[index01][index02][5] < 0) || (tPShopInfo.mItemInfo[index01][index02][5] > 1) || (
				tPShopInfo.mItemInfo[index01][index02][6] < 0) || (tPShopInfo.mItemInfo[index01][index02][6] > (
				MAX_INVENTORY_SLOT_NUM - 1)) || (tPShopInfo.mItemInfo[index01][index02][7] < 0) || (tPShopInfo.mItemInfo
				[index01][index02][7] > 7) || (tPShopInfo.mItemInfo[index01][index02][8] < 0) || (tPShopInfo.mItemInfo[
				index01][index02][8] > 7))
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			if (tPShopInfo.mItemInfo[index01][index02][5] == 1)
			{
				if (mUSER[tUserIndex].mAvatarInfo.aExpandInventoryDate < mUTIL.ReturnNowDate())
				{
					mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
					return;
				}
			}
			if ((tPShopInfo.mItemInfo[index01][index02][0] != mUSER[tUserIndex].mAvatarInfo.aInventory[tPShopInfo.
				mItemInfo[index01][index02][5]][tPShopInfo.mItemInfo[index01][index02][6]][0]) || (tPShopInfo.mItemInfo[
				index01][index02][1] != mUSER[tUserIndex].mAvatarInfo.aInventory[tPShopInfo.mItemInfo[index01][index02][
				5]][tPShopInfo.mItemInfo[index01][index02][6]][3]) || (tPShopInfo.mItemInfo[index01][index02][2] !=
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPShopInfo.mItemInfo[index01][index02][5]][tPShopInfo.mItemInfo
					[index01][index02][6]][4]) || (tPShopInfo.mItemInfo[index01][index02][3] != mUSER[tUserIndex].
				mAvatarInfo.aInventory[tPShopInfo.mItemInfo[index01][index02][5]][tPShopInfo.mItemInfo[index01][index02]
					[6]][5]) || (tPShopInfo.mItemInfo[index01][index02][7] != mUSER[tUserIndex].mAvatarInfo.aInventory[
				tPShopInfo.mItemInfo[index01][index02][5]][tPShopInfo.mItemInfo[index01][index02][6]][1]) || (tPShopInfo
				.mItemInfo[index01][index02][8] != mUSER[tUserIndex].mAvatarInfo.aInventory[tPShopInfo.mItemInfo[
					index01][index02][5]][tPShopInfo.mItemInfo[index01][index02][6]][2]))
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			if ((tPShopInfo.mItemInfo[index01][index02][4] < 1) || (tPShopInfo.mItemInfo[index01][index02][4] >
				999999999))
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
		}
	}
	mAVATAR_OBJECT[tUserIndex].mDATA.aPShopState = 1;
	strncpy(mAVATAR_OBJECT[tUserIndex].mDATA.aPShopName, tPShopInfo.mName, MAX_PSHOP_NAME_LENGTH);
	CopyMemory(&mAVATAR_OBJECT[tUserIndex].mPShopInfo, &tPShopInfo, sizeof(PSHOP_INFO));
	mAVATAR_OBJECT[tUserIndex].mPShopInfo.mUniqueNumber = mGAME.mAvatarPShopUniqueNumber;
	mGAME.mAvatarPShopUniqueNumber++;
	mTRANSFER.B_START_PSHOP_RECV(0, &tPShopInfo);
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//CHECK_POSSIBLE_PSHOP_REGION
BOOL MyWork::CheckPossiblePShopRegion(int tTribe, int tZoneNumber, float tCoord[3])
{
	float tCenter[3];
	float tRadius;

	switch (tZoneNumber)
	{
	case 1:
		if (tTribe != 0)
		{
			return FALSE;
		}
		tCenter[0] = 4.0f;
		tCenter[1] = 0.0f;
		tCenter[2] = -2.0f;
		tRadius = 1000.0f;
		if (mUTIL.GetLengthXYZ(&tCoord[0], &tCenter[0]) < tRadius)
		{
			return TRUE;
		}
		return FALSE;
	case 6:
		if (tTribe != 1)
		{
			return FALSE;
		}
		tCenter[0] = -189.0f;
		tCenter[1] = 0.0f;
		tCenter[2] = 1150.0f;
		tRadius = 1000.0f;
		if (mUTIL.GetLengthXYZ(&tCoord[0], &tCenter[0]) < tRadius)
		{
			return TRUE;
		}
		return FALSE;
	case 11:
		if (tTribe != 2)
		{
			return FALSE;
		}
		tCenter[0] = 449.0f;
		tCenter[1] = 1.0f;
		tCenter[2] = 439.0f;
		tRadius = 1000.0f;
		if (mUTIL.GetLengthXYZ(&tCoord[0], &tCenter[0]) < tRadius)
		{
			return TRUE;
		}
		return FALSE;
	case 140:
		if (tTribe != 3)
		{
			return FALSE;
		}
		tCenter[0] = 452.0f;
		tCenter[1] = 0.0f;
		tCenter[2] = 487.0f;
		tRadius = 1000.0f;
		if (mUTIL.GetLengthXYZ(&tCoord[0], &tCenter[0]) < tRadius)
		{
			return TRUE;
		}
		return FALSE;
	case 37:
		tCenter[0] = 1.0f;
		tCenter[1] = 0.0f;
		tCenter[2] = -1478.0f;
		tRadius = 1000.0f;
		if (mUTIL.GetLengthXYZ(&tCoord[0], &tCenter[0]) < tRadius)
		{
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}

//W_END_PSHOP_SEND
void W_END_PSHOP_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	int tResult;
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	if (mAVATAR_OBJECT[tUserIndex].mDATA.aPShopState != 1)
	{
		return;
	}

	CopyMemory(&tResult, &tPacket[1], 4);
	mAVATAR_OBJECT[tUserIndex].mDATA.aPShopState = 0;
	mTRANSFER.B_END_PSHOP_RECV(tResult);
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_DEMAND_PSHOP_SEND
void W_DEMAND_PSHOP_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	char tAvatarName[MAX_AVATAR_NAME_LENGTH];
	CopyMemory(&tAvatarName[0], &tPacket[1], MAX_AVATAR_NAME_LENGTH);

	int tOtherAvatarIndex;

	tOtherAvatarIndex = mUTIL.SearchAvatar(&tAvatarName[0], tUserIndex);
	if (tOtherAvatarIndex == -1)
	{
		mTRANSFER.B_DEMAND_PSHOP_RECV(1, &mAVATAR_OBJECT[0].mPShopInfo);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if (mUSER[tOtherAvatarIndex].mMoveZoneResult == 1)
	{
		mTRANSFER.B_DEMAND_PSHOP_RECV(1, &mAVATAR_OBJECT[0].mPShopInfo);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if (mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aPShopState != 1)
	{
		mTRANSFER.B_DEMAND_PSHOP_RECV(2, &mAVATAR_OBJECT[0].mPShopInfo);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	mTRANSFER.B_DEMAND_PSHOP_RECV(0, &mAVATAR_OBJECT[tOtherAvatarIndex].mPShopInfo);
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_PSHOP_ITEM_INFO_SEND
void W_PSHOP_ITEM_INFO_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	int tSort1;
	int tSort2;
	CopyMemory(&tSort1, &tPacket[1], 4);
	CopyMemory(&tSort2, &tPacket[5], 4);
}

//W_BUY_PSHOP_SEND
void W_BUY_PSHOP_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	DWORD tUniqueNumber;
	char tAvatarName[MAX_AVATAR_NAME_LENGTH];
	int tPage1;
	int tIndex1;
	int tQuantity1;
	int tPage2;
	int tIndex2;
	int tXPost2;
	int tYPost2;
	CopyMemory(&tUniqueNumber, &tPacket[1], 4);
	CopyMemory(&tAvatarName[0], &tPacket[5], MAX_AVATAR_NAME_LENGTH);
	CopyMemory(&tPage1, &tPacket[(5 + MAX_AVATAR_NAME_LENGTH)], 4);
	CopyMemory(&tIndex1, &tPacket[(9 + MAX_AVATAR_NAME_LENGTH)], 4);
	CopyMemory(&tQuantity1, &tPacket[(13 + MAX_AVATAR_NAME_LENGTH)], 4);
	CopyMemory(&tPage2, &tPacket[(17 + MAX_AVATAR_NAME_LENGTH)], 4);
	CopyMemory(&tIndex2, &tPacket[(21 + MAX_AVATAR_NAME_LENGTH)], 4);
	CopyMemory(&tXPost2, &tPacket[(25 + MAX_AVATAR_NAME_LENGTH)], 4);
	CopyMemory(&tYPost2, &tPacket[(29 + MAX_AVATAR_NAME_LENGTH)], 4);
	if ((tPage1 < 0) || (tPage1 > (MAX_PSHOP_PAGE_NUM - 1)) || (tIndex1 < 0) || (tIndex1 > (MAX_PSHOP_SLOT_NUM - 1)) ||
		(tPage2 < 0) || (tPage2 > 1) || (tIndex2 < 0) || (tIndex2 > (MAX_INVENTORY_SLOT_NUM - 1)) || (tXPost2 < 0) || (
			tXPost2 > 7) || (tYPost2 < 0) || (tYPost2 > 7))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int tOtherAvatarIndex;
	int tItemInfo[9];
	int tValue[6];
	int tItemSocket;
	// # Defense Hack #
	//float fRange = 0.0f;
	//fRange = mUTIL.ReturnLengthXYZ(&mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aAction.aLocation[1]);

	//if (fRange > 10000.0f) {
	//	//mGAMELOG.GL_659_AUTO_CHECK(tUserIndex, 8, fRange);
	//	//mGAMELOG.GL_659_AUTO_CHECK(tOtherAvatarIndex, 8, fRange);
	//	mTRANSFER.B_BUY_PSHOP_RECV(8, 0, 0, 0, &tValue[0]);
	//	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	//	return;
	//}

	if (1 == mUSER[tUserIndex].mAuthInfo.PShopFlag)
	{
		mTRANSFER.B_BUY_PSHOP_RECV(8, 0, 0, 0, &tValue[0], 0);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	} // # }

	tOtherAvatarIndex = mUTIL.SearchAvatar(&tAvatarName[0], tUserIndex);
	if (tOtherAvatarIndex == -1)
	{
		mTRANSFER.B_BUY_PSHOP_RECV(1, 0, 0, 0, &tValue[0], 0);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if (mUSER[tOtherAvatarIndex].mMoveZoneResult == 1)
	{
		mTRANSFER.B_BUY_PSHOP_RECV(1, 0, 0, 0, &tValue[0], 0);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if (mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aPShopState != 1)
	{
		mTRANSFER.B_BUY_PSHOP_RECV(2, 0, 0, 0, &tValue[0], 0);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if (mAVATAR_OBJECT[tOtherAvatarIndex].mPShopInfo.mUniqueNumber != tUniqueNumber)
	{
		mTRANSFER.B_BUY_PSHOP_RECV(7, 0, 0, 0, &tValue[0], 0);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if (1 == mUSER[tOtherAvatarIndex].mAuthInfo.PShopFlag)
	{
		mTRANSFER.B_BUY_PSHOP_RECV(9, 0, 0, 0, &tValue[0], 0);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}

	CopyMemory(&tItemInfo[0], &mAVATAR_OBJECT[tOtherAvatarIndex].mPShopInfo.mItemInfo[tPage1][tIndex1][0], 36);
	if (tItemInfo[0] < 1)
	{
		mTRANSFER.B_BUY_PSHOP_RECV(3, 0, 0, 0, &tValue[0], 0);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mTRANSFER.B_DEMAND_PSHOP_RECV(0, &mAVATAR_OBJECT[tOtherAvatarIndex].mPShopInfo);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if ((mUSER[tOtherAvatarIndex].mAvatarInfo.aInventory[tItemInfo[5]][tItemInfo[6]][0] != tItemInfo[0]) || (mUSER[
		tOtherAvatarIndex].mAvatarInfo.aInventory[tItemInfo[5]][tItemInfo[6]][1] != tItemInfo[7]) || (mUSER[
		tOtherAvatarIndex].mAvatarInfo.aInventory[tItemInfo[5]][tItemInfo[6]][2] != tItemInfo[8]) || (mUSER[
		tOtherAvatarIndex].mAvatarInfo.aInventory[tItemInfo[5]][tItemInfo[6]][3] != tItemInfo[1]) || (mUSER[
		tOtherAvatarIndex].mAvatarInfo.aInventory[tItemInfo[5]][tItemInfo[6]][4] != tItemInfo[2]) || (mUSER[
		tOtherAvatarIndex].mAvatarInfo.aInventory[tItemInfo[5]][tItemInfo[6]][5] != tItemInfo[3]))
	{
		mTRANSFER.B_BUY_PSHOP_RECV(4, 0, 0, 0, &tValue[0], 0);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if (mUTIL.CheckOverMaximum(mUSER[tOtherAvatarIndex].mAvatarInfo.aMoney, tItemInfo[4]))
	{
		mTRANSFER.B_BUY_PSHOP_RECV(5, 0, 0, 0, &tValue[0], 0);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if (tPage2 == 1)
	{
		if (mUSER[tUserIndex].mAvatarInfo.aExpandInventoryDate < mUTIL.ReturnNowDate())
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
	}
	if (mUSER[tUserIndex].mAvatarInfo.aMoney < tItemInfo[4])
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] > 0)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	tValue[0] = tItemInfo[0];
	tValue[1] = tXPost2;
	tValue[2] = tYPost2;
	tValue[3] = tItemInfo[1];
	tValue[4] = tItemInfo[2];
	tValue[5] = tItemInfo[3];
	tItemSocket = mUSER[tOtherAvatarIndex].mAvatarInfo.aInventorySocket[tItemInfo[5]][tItemInfo[6]];
	//mGAMELOG.GL_609_PSHOP_ITEM(tUserIndex, 1, tItemInfo[0], tItemInfo[1], tItemInfo[2], tItemInfo[3], tItemInfo[4]);
	mUSER[tUserIndex].mAvatarInfo.aMoney -= tItemInfo[4];
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] = tValue[0];
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][1] = tValue[1];
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][2] = tValue[2];
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][3] = tValue[3];
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][4] = tValue[4];
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][5] = tValue[5];
	// Transfer Socket
	mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage2][tIndex2] = tItemSocket;
	// Transfer Done
	mTRANSFER.B_BUY_PSHOP_RECV(0, tItemInfo[4], tPage2, tIndex2, &tValue[0], tItemSocket);
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	//mGAMELOG.GL_609_PSHOP_ITEM(tOtherAvatarIndex, 2, tItemInfo[0], tItemInfo[1], tItemInfo[2], tItemInfo[3], tItemInfo[4]);
	mUSER[tOtherAvatarIndex].mAvatarInfo.aMoney += tItemInfo[4];
	mUSER[tOtherAvatarIndex].mAvatarInfo.aInventory[tItemInfo[5]][tItemInfo[6]][0] = 0;
	mUSER[tOtherAvatarIndex].mAvatarInfo.aInventory[tItemInfo[5]][tItemInfo[6]][1] = 0;
	mUSER[tOtherAvatarIndex].mAvatarInfo.aInventory[tItemInfo[5]][tItemInfo[6]][2] = 0;
	mUSER[tOtherAvatarIndex].mAvatarInfo.aInventory[tItemInfo[5]][tItemInfo[6]][3] = 0;
	mUSER[tOtherAvatarIndex].mAvatarInfo.aInventory[tItemInfo[5]][tItemInfo[6]][4] = 0;
	mUSER[tOtherAvatarIndex].mAvatarInfo.aInventory[tItemInfo[5]][tItemInfo[6]][5] = 0;
	// Transfer Socket
	mUSER[tOtherAvatarIndex].mAvatarInfo.aInventorySocket[tItemInfo[5]][tItemInfo[6]] = 0;
	// Transfer Done
	mTRANSFER.B_BUY_PSHOP_RECV(6, tItemInfo[4], tItemInfo[5], tItemInfo[6], &tValue[0],
	                           mUSER[tOtherAvatarIndex].mAvatarInfo.aInventorySocket[tItemInfo[5]][tItemInfo[6]]);
	mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	mAVATAR_OBJECT[tOtherAvatarIndex].mPShopInfo.mItemInfo[tPage1][tIndex1][0] = 0;
	mAVATAR_OBJECT[tOtherAvatarIndex].mPShopInfo.mItemInfo[tPage1][tIndex1][1] = 0;
	mAVATAR_OBJECT[tOtherAvatarIndex].mPShopInfo.mItemInfo[tPage1][tIndex1][2] = 0;
	mAVATAR_OBJECT[tOtherAvatarIndex].mPShopInfo.mItemInfo[tPage1][tIndex1][3] = 0;
	mAVATAR_OBJECT[tOtherAvatarIndex].mPShopInfo.mItemInfo[tPage1][tIndex1][4] = 0;
	mAVATAR_OBJECT[tOtherAvatarIndex].mPShopInfo.mItemInfo[tPage1][tIndex1][5] = 0;
	mAVATAR_OBJECT[tOtherAvatarIndex].mPShopInfo.mItemInfo[tPage1][tIndex1][6] = 0;
	mAVATAR_OBJECT[tOtherAvatarIndex].mPShopInfo.mItemInfo[tPage1][tIndex1][7] = 0;
	mAVATAR_OBJECT[tOtherAvatarIndex].mPShopInfo.mItemInfo[tPage1][tIndex1][8] = 0;
	// Transfer Socket
	mAVATAR_OBJECT[tOtherAvatarIndex].mPShopInfo.mItemSocket[tPage1][tIndex1] = 0;
	// Transfer Done
	mTRANSFER.B_DEMAND_PSHOP_RECV(0, &mAVATAR_OBJECT[tOtherAvatarIndex].mPShopInfo);
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	mTRANSFER.B_DEMAND_PSHOP_RECV(3, &mAVATAR_OBJECT[tOtherAvatarIndex].mPShopInfo);
	mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_GET_CASH_SIZE_SEND
void W_GET_CASH_SIZE_SEND(int tUserIndex)
{
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	if (!mEXTRA_COM.U_GET_CASH_SIZE_FOR_EXTRA_SEND(&mUSER[tUserIndex].uID[0]))
	{
		mTRANSFER.B_GET_CASH_SIZE_RECV(0, 0);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	mTRANSFER.B_GET_CASH_SIZE_RECV(mEXTRA_COM.mRecv_CashSize, mEXTRA_COM.mRecv_BonusCashSize);
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_BUY_CASH_ITEM_SEND
void W_BUY_CASH_ITEM_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int tCostInfoIndex;
	int tPage;
	int tIndex;
	int tValue[6];
	CopyMemory(&tCostInfoIndex, &tPacket[1], 4);
	CopyMemory(&tPage, &tPacket[5], 4);
	CopyMemory(&tIndex, &tPacket[9], 4);
	CopyMemory(&tValue[0], &tPacket[13], 24);

	int tVersion = 0;
	CopyMemory(&tVersion, &tPacket[37], 4);
	if (tVersion != mEXTRA_COM.mCashItemInfoVersion)
	{
		mTRANSFER.B_BUY_CASH_ITEM_RECV(3, 0, tPage, tIndex, &tValue[0]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mUSER[tUserIndex].mIsValidCashItemInfo = false;
		return;
	}
	if (mEXTRA_COM.mIsValidCashItemInfo == false)
	{
		mTRANSFER.B_BUY_CASH_ITEM_RECV(4, 0, tPage, tIndex, &tValue[0]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}

	if ((tPage < 0) || (tPage > 1) || (tIndex < 0) || (tIndex > (MAX_INVENTORY_SLOT_NUM - 1)))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	if (tPage == 1)
	{
		if (mUSER[tUserIndex].mAvatarInfo.aExpandInventoryDate < mUTIL.ReturnNowDate())
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
	}
	if (!mEXTRA_COM.U_BUY_CASH_ITEM_FOR_EXTRA_SEND(&mUSER[tUserIndex].uID[0], tCostInfoIndex, tValue[0], tValue[3],
	                                               &mUSER[tUserIndex].mAvatarInfo.aName[0],
	                                               mUSER[tUserIndex].mAvatarInfo.aTribe,
	                                               mUSER[tUserIndex].mAvatarInfo.aLevel1, tVersion))
	{
		mTRANSFER.B_BUY_CASH_ITEM_RECV(2, 0, tPage, tIndex, &tValue[0]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if (mEXTRA_COM.mRecv_Result != 0)
	{
		int result = 1;
		if (mEXTRA_COM.mRecv_Result == 2)
		{
			result = 4;
		}
		else if (mEXTRA_COM.mRecv_Result == 3) // 캐쉬 아이템 정보 버전 오류.
		{
			mEXTRA_COM.U_GET_CASH_ITEM_INFO_FOR_EXTRA_SEND();
			result = 3;
		}
		mTRANSFER.B_BUY_CASH_ITEM_RECV(result, 0, tPage, tIndex, &tValue[0]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	tValue[0] = tValue[0];
	tValue[1] = tValue[1];
	tValue[2] = tValue[2];
	tValue[3] = tValue[3];
	tValue[4] = 0;
	tValue[5] = mUTIL.ReturnItemRecognitionNumber(0);
	GameLogger(5, "<BUY_CASH_ITEM>(%s),(%s), (%d, %d, %d, %d),(Cost: %d, Points Left: %d),[[%d]]",
	           mUSER[tUserIndex].uID, mUSER[tUserIndex].mAvatarInfo.aName, tValue[0], tValue[3], tValue[4], tValue[5],
	           mEXTRA_COM.mRecv_CostSize, mEXTRA_COM.mRecv_CashSize, mSERVER_INFO.mServerNumber);
	//mGAMELOG.GL_604_BUY_CASH_ITEM(tUserIndex, tValue[0], tValue[3], tValue[4], tValue[5], mEXTRA_COM.mRecv_CostSize, mEXTRA_COM.mRecv_BonusCostSize);

	if ((mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] == tValue[0]) && (mUSER[tUserIndex].mAvatarInfo.
		aInventory[tPage][tIndex][1] == tValue[1]) && (mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] ==
		tValue[2]))
	{
		ITEM_INFO* tITEM_INFO;
		tITEM_INFO = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0]);
		if ((tITEM_INFO != nullptr) && (tITEM_INFO->iSort == 2) && (mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][
			tIndex][3] + tValue[3] <= 99))
		{
			tValue[3] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] + tValue[3];
		}
	}

	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = tValue[0];
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = tValue[1];
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = tValue[2];
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = tValue[3];
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = tValue[4];
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = tValue[5];
	mTRANSFER.B_BUY_CASH_ITEM_RECV(0, mEXTRA_COM.mRecv_CashSize, tPage, tIndex, &tValue[0]);
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

// @ Cost Info Download @
void W_GET_CASH_ITEM_INFO_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mIsValidCashItemInfo == true)
	{
		return;
	}

	mTRANSFER.B_GET_CASH_ITEM_INFO_RECV(0, mEXTRA_COM.mCashItemInfoVersion, mEXTRA_COM.mCashItemInfo);
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);

	mUSER[tUserIndex].mIsValidCashItemInfo = true;
}

//W_DUEL_ASK_SEND
void W_DUEL_ASK_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}
	if (124 == mSERVER_INFO.mServerNumber)
		return;

	char tAvatarName[MAX_AVATAR_NAME_LENGTH];
	int tSort;
	CopyMemory(&tAvatarName[0], &tPacket[1], MAX_AVATAR_NAME_LENGTH);
	tAvatarName[(MAX_AVATAR_NAME_LENGTH - 1)] = '\0';
	CopyMemory(&tSort, &tPacket[(1 + MAX_AVATAR_NAME_LENGTH)], 4);

	int tOtherAvatarIndex;

	if (mAVATAR_OBJECT[tUserIndex].mDATA.aDuelState[0] != 0)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mAVATAR_OBJECT[tUserIndex].CheckCommunityWork())
	{
		mTRANSFER.B_DUEL_ANSWER_RECV(3);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if ((mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aSort == 11) || (mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aSort ==
		12))
	{
		mTRANSFER.B_DUEL_ANSWER_RECV(3);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	tOtherAvatarIndex = mUTIL.SearchAvatar(tAvatarName, tUserIndex);
	if (tOtherAvatarIndex == -1)
	{
		mTRANSFER.B_DUEL_ANSWER_RECV(4);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if ((mSERVER_INFO.mServerNumber != 37) && (mSERVER_INFO.mServerNumber != 119) && (mSERVER_INFO.mServerNumber !=
		124))
	{
		if ((mAVATAR_OBJECT[tUserIndex].mDATA.aTribe != mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aTribe) && (
			mAVATAR_OBJECT[tUserIndex].mDATA.aTribe != mGAME.ReturnAllianceTribe(
				mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aTribe)))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
	}
	if ((mUSER[tOtherAvatarIndex].mMoveZoneResult == 1) || (mAVATAR_OBJECT[tOtherAvatarIndex].CheckCommunityWork()))
	{
		mTRANSFER.B_DUEL_ANSWER_RECV(5);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if ((mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aAction.aSort == 11) || (mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.
		aAction.aSort == 12))
	{
		mTRANSFER.B_DUEL_ANSWER_RECV(5);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	mAVATAR_OBJECT[tUserIndex].mDuelProcessState = 1;
	strncpy(mAVATAR_OBJECT[tUserIndex].mDuelProcessAvatarName, mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aName,
	        MAX_AVATAR_NAME_LENGTH);
	mAVATAR_OBJECT[tUserIndex].mDuelProcessSort = tSort;
	mAVATAR_OBJECT[tOtherAvatarIndex].mDuelProcessState = 2;
	strncpy(mAVATAR_OBJECT[tOtherAvatarIndex].mDuelProcessAvatarName, mAVATAR_OBJECT[tUserIndex].mDATA.aName,
	        MAX_AVATAR_NAME_LENGTH);
	mAVATAR_OBJECT[tOtherAvatarIndex].mDuelProcessSort = tSort;
	mTRANSFER.B_DUEL_ASK_RECV(&mUSER[tUserIndex].mAvatarInfo.aName[0], tSort);
	mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_DUEL_CANCEL_SEND
void W_DUEL_CANCEL_SEND(int tUserIndex)
{
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	int tOtherAvatarIndex;

	if (mAVATAR_OBJECT[tUserIndex].mDuelProcessState != 1)
	{
		return;
	}
	mAVATAR_OBJECT[tUserIndex].mDuelProcessState = 0;
	tOtherAvatarIndex = mUTIL.SearchAvatar(mAVATAR_OBJECT[tUserIndex].mDuelProcessAvatarName, tUserIndex);
	if (tOtherAvatarIndex == -1)
	{
		return;
	}
	if (mAVATAR_OBJECT[tOtherAvatarIndex].mDuelProcessState != 2)
	{
		return;
	}
	if (strcmp(mAVATAR_OBJECT[tOtherAvatarIndex].mDuelProcessAvatarName, mAVATAR_OBJECT[tUserIndex].mDATA.aName) != 0)
	{
		return;
	}
	if (mUSER[tOtherAvatarIndex].mMoveZoneResult == 1)
	{
		return;
	}
	mAVATAR_OBJECT[tOtherAvatarIndex].mDuelProcessState = 0;
	mTRANSFER.B_DUEL_CANCEL_RECV();
	mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_DUEL_ANSWER_SEND
void W_DUEL_ANSWER_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	int tAnswer;
	CopyMemory(&tAnswer, &tPacket[1], 4);

	int tOtherAvatarIndex;

	if (mAVATAR_OBJECT[tUserIndex].mDuelProcessState != 2)
	{
		return;
	}
	switch (tAnswer)
	{
	case 0:
		mAVATAR_OBJECT[tUserIndex].mDuelProcessState = 3;
		break;
	case 1:
		mAVATAR_OBJECT[tUserIndex].mDuelProcessState = 0;
		break;
	case 2:
		mAVATAR_OBJECT[tUserIndex].mDuelProcessState = 0;
		break;
	default:
		return;
	}
	tOtherAvatarIndex = mUTIL.SearchAvatar(mAVATAR_OBJECT[tUserIndex].mDuelProcessAvatarName, tUserIndex);
	if (tOtherAvatarIndex == -1)
	{
		return;
	}
	if (mAVATAR_OBJECT[tOtherAvatarIndex].mDuelProcessState != 1)
	{
		return;
	}
	if (strcmp(mAVATAR_OBJECT[tOtherAvatarIndex].mDuelProcessAvatarName, mAVATAR_OBJECT[tUserIndex].mDATA.aName) != 0)
	{
		return;
	}
	if (mUSER[tOtherAvatarIndex].mMoveZoneResult == 1)
	{
		return;
	}
	switch (tAnswer)
	{
	case 0:
		mAVATAR_OBJECT[tOtherAvatarIndex].mDuelProcessState = 3;
		break;
	case 1:
		mAVATAR_OBJECT[tOtherAvatarIndex].mDuelProcessState = 0;
		break;
	case 2:
		mAVATAR_OBJECT[tOtherAvatarIndex].mDuelProcessState = 0;
		break;
	default:
		return;
	}
	mTRANSFER.B_DUEL_ANSWER_RECV(tAnswer);
	mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_DUEL_START_SEND
void W_DUEL_START_SEND(int tUserIndex)
{
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	int tAvatarDuelUniqueNumber;
	int tOtherAvatarIndex;

	tAvatarDuelUniqueNumber = mGAME.mAvatarDuelUniqueNumber;
	mGAME.mAvatarDuelUniqueNumber++;
	if (mAVATAR_OBJECT[tUserIndex].mDuelProcessState != 3)
	{
		return;
	}
	mAVATAR_OBJECT[tUserIndex].mDATA.aDuelState[0] = 1;
	mAVATAR_OBJECT[tUserIndex].mDATA.aDuelState[1] = tAvatarDuelUniqueNumber;
	mAVATAR_OBJECT[tUserIndex].mDATA.aDuelState[2] = 1;
	if (mAVATAR_OBJECT[tUserIndex].mDuelProcessSort == 1)
	{
		mAVATAR_OBJECT[tUserIndex].mCheckPossibleEatPotion = FALSE;
	}
	else
	{
		mAVATAR_OBJECT[tUserIndex].mCheckPossibleEatPotion = TRUE;
	}
	mAVATAR_OBJECT[tUserIndex].mDuelProcessState = 4;
	mAVATAR_OBJECT[tUserIndex].mDuelProcessRemainTime = 180;
	mTRANSFER.B_DUEL_START_RECV(&mAVATAR_OBJECT[tUserIndex].mDATA.aDuelState[0],
	                            mAVATAR_OBJECT[tUserIndex].mDuelProcessRemainTime);
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	mTRANSFER.B_AVATAR_CHANGE_INFO_1(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 7,
	                                 mAVATAR_OBJECT[tUserIndex].mDATA.aDuelState[0],
	                                 mAVATAR_OBJECT[tUserIndex].mDATA.aDuelState[1],
	                                 mAVATAR_OBJECT[tUserIndex].mDATA.aDuelState[2]);
	mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false);
	tOtherAvatarIndex = mUTIL.SearchAvatar(mAVATAR_OBJECT[tUserIndex].mDuelProcessAvatarName, tUserIndex);
	if (tOtherAvatarIndex == -1)
	{
		return;
	}
	if (mAVATAR_OBJECT[tOtherAvatarIndex].mDuelProcessState != 3)
	{
		return;
	}
	if (strcmp(mAVATAR_OBJECT[tOtherAvatarIndex].mDuelProcessAvatarName, mAVATAR_OBJECT[tUserIndex].mDATA.aName) != 0)
	{
		return;
	}
	if (mUSER[tOtherAvatarIndex].mMoveZoneResult == 1)
	{
		return;
	}
	mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[0] = 1;
	mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[1] = tAvatarDuelUniqueNumber;
	mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[2] = 2;
	if (mAVATAR_OBJECT[tOtherAvatarIndex].mDuelProcessSort == 1)
	{
		mAVATAR_OBJECT[tOtherAvatarIndex].mCheckPossibleEatPotion = FALSE;
	}
	else
	{
		mAVATAR_OBJECT[tOtherAvatarIndex].mCheckPossibleEatPotion = TRUE;
	}
	mAVATAR_OBJECT[tOtherAvatarIndex].mDuelProcessState = 4;
	mAVATAR_OBJECT[tOtherAvatarIndex].mDuelProcessRemainTime = 180;
	mTRANSFER.B_DUEL_START_RECV(&mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[0],
	                            mAVATAR_OBJECT[tOtherAvatarIndex].mDuelProcessRemainTime);
	mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	mTRANSFER.B_AVATAR_CHANGE_INFO_1(tOtherAvatarIndex, mAVATAR_OBJECT[tOtherAvatarIndex].mUniqueNumber, 7,
	                                 mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[0],
	                                 mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[1],
	                                 mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[2]);
	mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false);
}

void W_CHANGE_PLAYTIMETOTEACHERPOINT(int tUserIndex)
{
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mAvatarInfo.aLevel1 < 50)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	int iTeacherPoints = mUSER[tUserIndex].mAvatarInfo.aPlayTime2 * 250;
	mUSER[tUserIndex].mAvatarInfo.aPlayTime2 = 0;
	mUSER[tUserIndex].mAvatarInfo.aTeacherPoint += iTeacherPoints;

	mTRANSFER.B_CHANGE_PLAYTIMETOTEACHERPOINT(0, iTeacherPoints);
	mUSER[tUserIndex].Send(TRUE, (char*)mTRANSFER.mOriginal, mTRANSFER.mOriginalSize);
}

// W_CHECK_EVENT_INFO
void W_CHECK_EVENT_INFO(int tUserIndex)
{
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	mTRANSFER.B_CHECK_EVENT_INFO();
	mUSER[tUserIndex].Send(TRUE, (char*)mTRANSFER.mOriginal, mTRANSFER.mOriginalSize, false);
}

//W_TRIBE_VOTE_SEND
void W_TRIBE_VOTE_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int tSort;
	int tValue;
	CopyMemory(&tSort, &tPacket[1], 4);
	CopyMemory(&tValue, &tPacket[5], 4);

	int index01 = 0;
	int tBroadcastInfoSort;
	BYTE tBroadcastInfoData[MAX_BROADCAST_INFO_SIZE];
	int tLevelForTribeVote;
	int tPointForTribeVote;

	switch (tSort)
	{
	case 1: //[REGISTER]
		if ((tValue < 0) || (tValue > (MAX_TRIBE_VOTE_AVATAR_NUM - 1)))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mGAME.mWorldInfo.mTribeVoteState[mUSER[tUserIndex].mAvatarInfo.aTribe] != 1)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aLevel1 < 113)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe < 1000)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		for (index01 = 0; index01 < MAX_TRIBE_VOTE_AVATAR_NUM; ++index01)
		{
			if (strcmp(mGAME.mTribeInfo.mTribeVoteName[mUSER[tUserIndex].mAvatarInfo.aTribe][index01],
			           mUSER[tUserIndex].mAvatarInfo.aName) == 0)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
		}
		for (index01 = 0; index01 < MAX_TRIBE_VOTE_AVATAR_NUM; ++index01)
		{
			if (strcmp(mGAME.mTribeInfo.mTribeVoteName[mUSER[tUserIndex].mAvatarInfo.aTribe][index01], "") != 0)
				continue;
			tValue = index01;
			break;
		}
		if (MAX_TRIBE_VOTE_AVATAR_NUM == index01)
		{
			tValue = 0;
			for (index01 = 1; index01 < MAX_TRIBE_VOTE_AVATAR_NUM; ++index01)
			{
				if (mGAME.mTribeInfo.mTribeVoteKillOtherTribe[mUSER[tUserIndex].mAvatarInfo.aTribe][tValue] <
					mGAME.mTribeInfo.mTribeVoteKillOtherTribe[mUSER[tUserIndex].mAvatarInfo.aTribe][index01])
				{
					continue;
				}
				tValue = index01;
			}
			if (mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe <= mGAME.mTribeInfo.mTribeVoteKillOtherTribe[mUSER[
				tUserIndex].mAvatarInfo.aTribe][tValue])
			{
				mTRANSFER.B_TRIBE_VOTE_RECV(1, tSort, tValue);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				return;
			}
		}

#if 0
		if (strcmp(mGAME.mTribeInfo.mTribeVoteName[mUSER[tUserIndex].mAvatarInfo.aTribe][tValue], "") != 0)
		{
			if (mGAME.mTribeInfo.mTribeVoteKillOtherTribe[mUSER[tUserIndex].mAvatarInfo.aTribe][tValue] >= mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
		}
#endif
		mTRANSFER.B_TRIBE_VOTE_RECV(0, tSort, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
#ifdef __GOD__
		tLevelForTribeVote = mUSER[tUserIndex].mAvatarInfo.aLevel1 + mUSER[tUserIndex].mAvatarInfo.aLevel2 - 112;
#else
		tLevelForTribeVote = mUSER[tUserIndex].mAvatarInfo.aLevel1 - 112;
#endif
	//--------------//
	//BROADCAST_INFO//
	//--------------//
		tBroadcastInfoSort = 57;
		CopyMemory(&tBroadcastInfoData[0], &mUSER[tUserIndex].mAvatarInfo.aTribe, 4);
		CopyMemory(&tBroadcastInfoData[4], &tValue, 4);
		CopyMemory(&tBroadcastInfoData[8], &mUSER[tUserIndex].mAvatarInfo.aName[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&tBroadcastInfoData[(8 + MAX_AVATAR_NAME_LENGTH)], &tLevelForTribeVote, 4);
		CopyMemory(&tBroadcastInfoData[(12 + MAX_AVATAR_NAME_LENGTH)], &mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe,
		           4);
		mCENTER_COM.U_DEMAND_BROADCAST_INFO(tBroadcastInfoSort, &tBroadcastInfoData[0]);
	//--------------//
	//--------------//
	//--------------//
		strncpy(mGAME.mTribeInfo.mTribeVoteName[mUSER[tUserIndex].mAvatarInfo.aTribe][tValue],
		        mUSER[tUserIndex].mAvatarInfo.aName, MAX_AVATAR_NAME_LENGTH);
		mGAME.mTribeInfo.mTribeVoteLevel[mUSER[tUserIndex].mAvatarInfo.aTribe][tValue] = tLevelForTribeVote;
		mGAME.mTribeInfo.mTribeVoteKillOtherTribe[mUSER[tUserIndex].mAvatarInfo.aTribe][tValue] = mUSER[tUserIndex].
			mAvatarInfo.aKillOtherTribe;
		mGAME.mTribeInfo.mTribeVotePoint[mUSER[tUserIndex].mAvatarInfo.aTribe][tValue] = 0;
		return;

	case 2: //[취소]
		if ((tValue < 0) || (tValue > (MAX_TRIBE_VOTE_AVATAR_NUM - 1)))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mGAME.mWorldInfo.mTribeVoteState[mUSER[tUserIndex].mAvatarInfo.aTribe] != 1)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (strcmp(mGAME.mTribeInfo.mTribeVoteName[mUSER[tUserIndex].mAvatarInfo.aTribe][tValue],
		           mUSER[tUserIndex].mAvatarInfo.aName) != 0)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		mTRANSFER.B_TRIBE_VOTE_RECV(0, tSort, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	//--------------//
	//BROADCAST_INFO//
	//--------------//
		tBroadcastInfoSort = 58;
		CopyMemory(&tBroadcastInfoData[0], &mUSER[tUserIndex].mAvatarInfo.aTribe, 4);
		CopyMemory(&tBroadcastInfoData[4], &tValue, 4);
		mCENTER_COM.U_DEMAND_BROADCAST_INFO(tBroadcastInfoSort, &tBroadcastInfoData[0]);
	//--------------//
	//--------------//
	//--------------//
		strcpy(mGAME.mTribeInfo.mTribeVoteName[mUSER[tUserIndex].mAvatarInfo.aTribe][tValue], "");
		mGAME.mTribeInfo.mTribeVoteLevel[mUSER[tUserIndex].mAvatarInfo.aTribe][tValue] = 0;
		mGAME.mTribeInfo.mTribeVoteKillOtherTribe[mUSER[tUserIndex].mAvatarInfo.aTribe][tValue] = 0;
		mGAME.mTribeInfo.mTribeVotePoint[mUSER[tUserIndex].mAvatarInfo.aTribe][tValue] = 0;
		return;

	case 3: //[투표]
		if ((tValue < 0) || (tValue > (MAX_TRIBE_VOTE_AVATAR_NUM - 1)))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mGAME.mWorldInfo.mTribeVoteState[mUSER[tUserIndex].mAvatarInfo.aTribe] != 2)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aLevel1 < 113)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (strcmp(mGAME.mTribeInfo.mTribeVoteName[mUSER[tUserIndex].mAvatarInfo.aTribe][tValue], "") == 0)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aTribeVoteDate >= mUTIL.ReturnVoteDate())
		{
			mTRANSFER.B_TRIBE_VOTE_RECV(1, tSort, tValue);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aTribeVoteDate = mUTIL.ReturnNowDate();
		mTRANSFER.B_TRIBE_VOTE_RECV(0, tSort, tValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
#ifdef __GOD__
		tPointForTribeVote = mUSER[tUserIndex].mAvatarInfo.aLevel1 + mUSER[tUserIndex].mAvatarInfo.aLevel2 * 3 - 112;
#else
		tPointForTribeVote = mUSER[tUserIndex].mAvatarInfo.aLevel1 - 112;
#endif
	//--------------//
	//BROADCAST_INFO//
	//--------------//
		tBroadcastInfoSort = 59;
		CopyMemory(&tBroadcastInfoData[0], &mUSER[tUserIndex].mAvatarInfo.aTribe, 4);
		CopyMemory(&tBroadcastInfoData[4], &tValue, 4);
		CopyMemory(&tBroadcastInfoData[8], &tPointForTribeVote, 4);
		mCENTER_COM.U_DEMAND_BROADCAST_INFO(tBroadcastInfoSort, &tBroadcastInfoData[0]);
	//--------------//
	//--------------//
	//--------------//
		return;

	default:
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
	}
}

//W_NPC_FUNCTION_SEND
void W_NPC_FUNCTION_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	int tValue01;

	CTOZ_ACK_INCOMING_UDP pack;
	::CopyMemory(&pack, mUSER[tUserIndex].mBUFFER_FOR_RECV, S_NPC_FUNCTION_SEND);
}

//W_GUILD_ASK_SEND
void W_GUILD_ASK_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}
	if (124 == mSERVER_INFO.mServerNumber)
		return;

	char tAvatarName[MAX_AVATAR_NAME_LENGTH];
	CopyMemory(&tAvatarName[0], &tPacket[1], MAX_AVATAR_NAME_LENGTH);
	tAvatarName[(MAX_AVATAR_NAME_LENGTH - 1)] = '\0';

	int tOtherAvatarIndex;

	if ((strcmp(mUSER[tUserIndex].mAvatarInfo.aGuildName, "") == 0) || ((mUSER[tUserIndex].mAvatarInfo.aGuildRole != 0)
		&& (mUSER[tUserIndex].mAvatarInfo.aGuildRole != 1)))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mAVATAR_OBJECT[tUserIndex].CheckCommunityWork())
	{
		mTRANSFER.B_GUILD_ANSWER_RECV(3);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if ((mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aSort == 11) || (mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aSort ==
		12))
	{
		mTRANSFER.B_GUILD_ANSWER_RECV(3);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	tOtherAvatarIndex = mUTIL.SearchAvatar(tAvatarName, tUserIndex);
	if (tOtherAvatarIndex == -1)
	{
		mTRANSFER.B_GUILD_ANSWER_RECV(4);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if (strcmp(mUSER[tOtherAvatarIndex].mAvatarInfo.aGuildName, "") != 0)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mAVATAR_OBJECT[tUserIndex].mDATA.aTribe != mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aTribe)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if ((mUSER[tOtherAvatarIndex].mMoveZoneResult == 1) || (mAVATAR_OBJECT[tOtherAvatarIndex].CheckCommunityWork()))
	{
		mTRANSFER.B_GUILD_ANSWER_RECV(5);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if ((mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aAction.aSort == 11) || (mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.
		aAction.aSort == 12))
	{
		mTRANSFER.B_GUILD_ANSWER_RECV(5);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	mAVATAR_OBJECT[tUserIndex].mGuildProcessState = 1;
	strncpy(mAVATAR_OBJECT[tUserIndex].mGuildProcessAvatarName, mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aName,
	        MAX_AVATAR_NAME_LENGTH);
	mAVATAR_OBJECT[tOtherAvatarIndex].mGuildProcessState = 2;
	strncpy(mAVATAR_OBJECT[tOtherAvatarIndex].mGuildProcessAvatarName, mAVATAR_OBJECT[tUserIndex].mDATA.aName,
	        MAX_AVATAR_NAME_LENGTH);
	mTRANSFER.B_GUILD_ASK_RECV(&mUSER[tUserIndex].mAvatarInfo.aName[0]);
	mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_GUILD_CANCEL_SEND
void W_GUILD_CANCEL_SEND(int tUserIndex)
{
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	int tOtherAvatarIndex;

	if (mAVATAR_OBJECT[tUserIndex].mGuildProcessState != 1)
	{
		return;
	}
	mAVATAR_OBJECT[tUserIndex].mGuildProcessState = 0;
	tOtherAvatarIndex = mUTIL.SearchAvatar(mAVATAR_OBJECT[tUserIndex].mGuildProcessAvatarName, tUserIndex);
	if (tOtherAvatarIndex == -1)
	{
		return;
	}
	if (mAVATAR_OBJECT[tOtherAvatarIndex].mGuildProcessState != 2)
	{
		return;
	}
	if (strcmp(mAVATAR_OBJECT[tOtherAvatarIndex].mGuildProcessAvatarName, mAVATAR_OBJECT[tUserIndex].mDATA.aName) != 0)
	{
		return;
	}
	if (mUSER[tOtherAvatarIndex].mMoveZoneResult == 1)
	{
		return;
	}
	mAVATAR_OBJECT[tOtherAvatarIndex].mGuildProcessState = 0;
	mTRANSFER.B_GUILD_CANCEL_RECV();
	mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_GUILD_ANSWER_SEND
void W_GUILD_ANSWER_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	int tAnswer;
	CopyMemory(&tAnswer, &tPacket[1], 4);

	int tOtherAvatarIndex;

	if (mAVATAR_OBJECT[tUserIndex].mGuildProcessState != 2)
	{
		return;
	}
	switch (tAnswer)
	{
	case 0:
		mAVATAR_OBJECT[tUserIndex].mGuildProcessState = 3;
		break;
	case 1:
		mAVATAR_OBJECT[tUserIndex].mGuildProcessState = 0;
		break;
	case 2:
		mAVATAR_OBJECT[tUserIndex].mGuildProcessState = 0;
		break;
	default:
		return;
	}
	tOtherAvatarIndex = mUTIL.SearchAvatar(mAVATAR_OBJECT[tUserIndex].mGuildProcessAvatarName, tUserIndex);
	if (tOtherAvatarIndex == -1)
	{
		return;
	}
	if (mAVATAR_OBJECT[tOtherAvatarIndex].mGuildProcessState != 1)
	{
		return;
	}
	if (strcmp(mAVATAR_OBJECT[tOtherAvatarIndex].mGuildProcessAvatarName, mAVATAR_OBJECT[tUserIndex].mDATA.aName) != 0)
	{
		return;
	}
	if (mUSER[tOtherAvatarIndex].mMoveZoneResult == 1)
	{
		return;
	}
	switch (tAnswer)
	{
	case 0:
		mAVATAR_OBJECT[tOtherAvatarIndex].mGuildProcessState = 3;
		break;
	case 1:
		mAVATAR_OBJECT[tOtherAvatarIndex].mGuildProcessState = 0;
		break;
	case 2:
		mAVATAR_OBJECT[tOtherAvatarIndex].mGuildProcessState = 0;
		break;
	default:
		return;
	}
	mTRANSFER.B_GUILD_ANSWER_RECV(tAnswer);
	mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_GUILD_WORK_SEND
void W_GUILD_WORK_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int tSort;
	BYTE tData[MAX_GUILD_WORK_SIZE];
	CopyMemory(&tSort, &tPacket[1], 4);
	CopyMemory(&tData[0], &tPacket[5], MAX_GUILD_WORK_SIZE);

	int index01;
	int tBroadcastInfoSort;
	BYTE tBroadcastInfoData[MAX_BROADCAST_INFO_SIZE];
	int tOtherAvatarIndex;
	char tGuildName[MAX_GUILD_NAME_LENGTH];
	char tGuildNotice[4][MAX_GUILD_NOTICE_LENGTH];
	int tTotalMemberNum;
	int tCost;
	char tAvatarName[MAX_AVATAR_NAME_LENGTH];
	int tGuildRole;
	char tCallName[MAX_CALL_NAME_LENGTH];

	if (124 == mSERVER_INFO.mServerNumber)
	{
		switch (tSort)
		{
		case 2:
		case 5:
		case 9:
			break;
		default:
			return;
		}
	}

	switch (tSort)
	{
	case 1: //[Basic]
		CopyMemory(&tGuildName[0], &tData[0], MAX_GUILD_NAME_LENGTH);
		tGuildName[(MAX_GUILD_NAME_LENGTH - 1)] = '\0';
		if ((strcmp(tGuildName, "") == 0) || (!mUTIL.CheckNameString(tGuildName)) || (strcmp(
			mUSER[tUserIndex].mAvatarInfo.aGuildName, "") != 0))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aLevel1 < 30)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aMoney < 10000000)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		CopyMemory(&tData[0], &tGuildName[0], MAX_GUILD_NAME_LENGTH);
		CopyMemory(&tData[MAX_GUILD_NAME_LENGTH], &mUSER[tUserIndex].mAvatarInfo.aName[0], MAX_AVATAR_NAME_LENGTH);
		if (!mEXTRA_COM.U_GUILD_WORK_FOR_EXTRA_SEND(tSort, &tData[0]))
		{
			mTRANSFER.B_GUILD_WORK_RECV(1, tSort, &mEXTRA_COM.mRecv_GuildInfo);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		if (mEXTRA_COM.mRecv_Result != 0)
		{
			mTRANSFER.B_GUILD_WORK_RECV(mEXTRA_COM.mRecv_Result, tSort, &mEXTRA_COM.mRecv_GuildInfo);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		strncpy(mEXTRA_COM.mRecv_GuildInfo.gName, tGuildName, MAX_GUILD_NAME_LENGTH);
		mTRANSFER.B_GUILD_WORK_RECV(0, tSort, &mEXTRA_COM.mRecv_GuildInfo);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	//--------------//
	//BROADCAST_INFO//
	//--------------//
		tBroadcastInfoSort = 101;
		CopyMemory(&tBroadcastInfoData[0], &mUSER[tUserIndex].mAvatarInfo.aName[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&tBroadcastInfoData[MAX_AVATAR_NAME_LENGTH], &tGuildName[0], MAX_GUILD_NAME_LENGTH);
		mCENTER_COM.U_DEMAND_BROADCAST_INFO(tBroadcastInfoSort, &tBroadcastInfoData[0]);
	//--------------//
	//--------------//
	//--------------//
	//mGAMELOG.GL_617_GUILD_MONEY(tUserIndex, 10000000, 1, mEXTRA_COM.mRecv_GuildInfo.gGrade);
		mUSER[tUserIndex].mAvatarInfo.aMoney -= 10000000;
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(19, mUSER[tUserIndex].mAvatarInfo.aMoney);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		strncpy(mUSER[tUserIndex].mAvatarInfo.aGuildName, tGuildName, MAX_GUILD_NAME_LENGTH);
		mUSER[tUserIndex].mAvatarInfo.aGuildRole = 0;
		strcpy(mUSER[tUserIndex].mAvatarInfo.aCallName, "");
		strncpy(mAVATAR_OBJECT[tUserIndex].mDATA.aGuildName, tGuildName, MAX_GUILD_NAME_LENGTH);
		mAVATAR_OBJECT[tUserIndex].mDATA.aGuildRole = 0;
		strcpy(mAVATAR_OBJECT[tUserIndex].mDATA.aCallName, "");
		return;

	case 2: //[Info]
		if (strcmp(mUSER[tUserIndex].mAvatarInfo.aGuildName, "") == 0)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		CopyMemory(&tData[0], &mUSER[tUserIndex].mAvatarInfo.aGuildName[0], MAX_GUILD_NAME_LENGTH);
		if (!mEXTRA_COM.U_GUILD_WORK_FOR_EXTRA_SEND(tSort, &tData[0]))
		{
			mTRANSFER.B_GUILD_WORK_RECV(1, tSort, &mEXTRA_COM.mRecv_GuildInfo);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		if (mEXTRA_COM.mRecv_Result != 0)
		{
			mTRANSFER.B_GUILD_WORK_RECV(1, tSort, &mEXTRA_COM.mRecv_GuildInfo);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		mTRANSFER.B_GUILD_WORK_RECV(0, tSort, &mEXTRA_COM.mRecv_GuildInfo);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 3: //[Join]
		if ((strcmp(mUSER[tUserIndex].mAvatarInfo.aGuildName, "") == 0) || ((mUSER[tUserIndex].mAvatarInfo.aGuildRole !=
			0) && (mUSER[tUserIndex].mAvatarInfo.aGuildRole != 1)))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mAVATAR_OBJECT[tUserIndex].mGuildProcessState != 3)
		{
			mTRANSFER.B_GUILD_WORK_RECV(1, tSort, &mEXTRA_COM.mRecv_GuildInfo);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		mAVATAR_OBJECT[tUserIndex].mGuildProcessState = 0;
		tOtherAvatarIndex = mUTIL.SearchAvatar(mAVATAR_OBJECT[tUserIndex].mGuildProcessAvatarName, tUserIndex);
		if (tOtherAvatarIndex == -1)
		{
			mTRANSFER.B_GUILD_WORK_RECV(1, tSort, &mEXTRA_COM.mRecv_GuildInfo);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		if (mAVATAR_OBJECT[tOtherAvatarIndex].mGuildProcessState != 3)
		{
			mTRANSFER.B_GUILD_WORK_RECV(1, tSort, &mEXTRA_COM.mRecv_GuildInfo);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		if (strcmp(mAVATAR_OBJECT[tOtherAvatarIndex].mGuildProcessAvatarName,
		           mAVATAR_OBJECT[tUserIndex].mDATA.aName) != 0)
		{
			mTRANSFER.B_GUILD_WORK_RECV(1, tSort, &mEXTRA_COM.mRecv_GuildInfo);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		mAVATAR_OBJECT[tOtherAvatarIndex].mGuildProcessState = 0;
		CopyMemory(&tData[0], &mUSER[tUserIndex].mAvatarInfo.aGuildName[0], MAX_GUILD_NAME_LENGTH);
		if (!mEXTRA_COM.U_GUILD_WORK_FOR_EXTRA_SEND(2, &tData[0]))
		{
			mTRANSFER.B_GUILD_WORK_RECV(1, tSort, &mEXTRA_COM.mRecv_GuildInfo);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		if (mEXTRA_COM.mRecv_Result != 0)
		{
			mTRANSFER.B_GUILD_WORK_RECV(1, tSort, &mEXTRA_COM.mRecv_GuildInfo);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		tTotalMemberNum = 0;
		for (index01 = 0; index01 < MAX_GUILD_AVATAR_NUM; index01++)
		{
			if (strcmp(mEXTRA_COM.mRecv_GuildInfo.gMemberName[index01], "") != 0)
			{
				tTotalMemberNum++;
			}
		}
		if (tTotalMemberNum >= (mEXTRA_COM.mRecv_GuildInfo.gGrade * 10))
		{
			mTRANSFER.B_GUILD_WORK_RECV(2, tSort, &mEXTRA_COM.mRecv_GuildInfo);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		CopyMemory(&tData[0], &mUSER[tUserIndex].mAvatarInfo.aGuildName[0], MAX_GUILD_NAME_LENGTH);
		CopyMemory(&tData[MAX_GUILD_NAME_LENGTH], &mUSER[tOtherAvatarIndex].mAvatarInfo.aName[0],
		           MAX_AVATAR_NAME_LENGTH);
		if (!mEXTRA_COM.U_GUILD_WORK_FOR_EXTRA_SEND(tSort, &tData[0]))
		{
			mTRANSFER.B_GUILD_WORK_RECV(1, tSort, &mEXTRA_COM.mRecv_GuildInfo);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		if (mEXTRA_COM.mRecv_Result != 0)
		{
			mTRANSFER.B_GUILD_WORK_RECV(1, tSort, &mEXTRA_COM.mRecv_GuildInfo);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		mTRANSFER.B_GUILD_WORK_RECV(0, tSort, &mEXTRA_COM.mRecv_GuildInfo);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	//--------------//
	//BROADCAST_INFO//
	//--------------//
		tBroadcastInfoSort = 102;
		CopyMemory(&tBroadcastInfoData[0], &mUSER[tOtherAvatarIndex].mAvatarInfo.aName[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&tBroadcastInfoData[MAX_AVATAR_NAME_LENGTH], &mUSER[tUserIndex].mAvatarInfo.aGuildName[0],
		           MAX_GUILD_NAME_LENGTH);
		mCENTER_COM.U_DEMAND_BROADCAST_INFO(tBroadcastInfoSort, &tBroadcastInfoData[0]);
	//--------------//
	//--------------//
	//--------------//
		strncpy(mUSER[tOtherAvatarIndex].mAvatarInfo.aGuildName, mUSER[tUserIndex].mAvatarInfo.aGuildName,
		        MAX_GUILD_NAME_LENGTH);
		mUSER[tOtherAvatarIndex].mAvatarInfo.aGuildRole = 2;
		strcpy(mUSER[tOtherAvatarIndex].mAvatarInfo.aCallName, "");
		strncpy(mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aGuildName, mUSER[tUserIndex].mAvatarInfo.aGuildName,
		        MAX_GUILD_NAME_LENGTH);
		mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aGuildRole = 2;
		strcpy(mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aCallName, "");
		return;

	case 4: //[Resign]
		if ((strcmp(mUSER[tUserIndex].mAvatarInfo.aGuildName, "") == 0) || (mUSER[tUserIndex].mAvatarInfo.aGuildRole ==
			0))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		CopyMemory(&tData[0], &mUSER[tUserIndex].mAvatarInfo.aGuildName[0], MAX_GUILD_NAME_LENGTH);
		CopyMemory(&tData[MAX_GUILD_NAME_LENGTH], &mUSER[tUserIndex].mAvatarInfo.aName[0], MAX_AVATAR_NAME_LENGTH);
		if (!mEXTRA_COM.U_GUILD_WORK_FOR_EXTRA_SEND(tSort, &tData[0]))
		{
			mTRANSFER.B_GUILD_WORK_RECV(1, tSort, &mEXTRA_COM.mRecv_GuildInfo);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		if (mEXTRA_COM.mRecv_Result != 0)
		{
			mTRANSFER.B_GUILD_WORK_RECV(1, tSort, &mEXTRA_COM.mRecv_GuildInfo);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		mTRANSFER.B_GUILD_WORK_RECV(0, tSort, &mEXTRA_COM.mRecv_GuildInfo);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	//--------------//
	//BROADCAST_INFO//
	//--------------//
		tBroadcastInfoSort = 103;
		CopyMemory(&tBroadcastInfoData[0], &mUSER[tUserIndex].mAvatarInfo.aName[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&tBroadcastInfoData[MAX_AVATAR_NAME_LENGTH], &mUSER[tUserIndex].mAvatarInfo.aGuildName[0],
		           MAX_GUILD_NAME_LENGTH);
		mCENTER_COM.U_DEMAND_BROADCAST_INFO(tBroadcastInfoSort, &tBroadcastInfoData[0]);
	//--------------//
	//--------------//
	//--------------//
		strcpy(mUSER[tUserIndex].mAvatarInfo.aGuildName, "");
		mUSER[tUserIndex].mAvatarInfo.aGuildRole = 0;
		strcpy(mUSER[tUserIndex].mAvatarInfo.aCallName, "");
		strcpy(mAVATAR_OBJECT[tUserIndex].mDATA.aGuildName, "");
		mAVATAR_OBJECT[tUserIndex].mDATA.aGuildRole = 0;
		strcpy(mAVATAR_OBJECT[tUserIndex].mDATA.aCallName, "");
		return;

	case 5: //[NOTICE]
		CopyMemory(tGuildNotice, &tData[0], (4 * MAX_GUILD_NOTICE_LENGTH));
		tGuildNotice[0][(MAX_GUILD_NOTICE_LENGTH - 1)] = '\0';
		tGuildNotice[1][(MAX_GUILD_NOTICE_LENGTH - 1)] = '\0';
		tGuildNotice[2][(MAX_GUILD_NOTICE_LENGTH - 1)] = '\0';
		tGuildNotice[3][(MAX_GUILD_NOTICE_LENGTH - 1)] = '\0';
		if ((strcmp(mUSER[tUserIndex].mAvatarInfo.aGuildName, "") == 0) || (mUSER[tUserIndex].mAvatarInfo.aGuildRole !=
			0))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		CopyMemory(&tData[0], &mUSER[tUserIndex].mAvatarInfo.aGuildName[0], MAX_GUILD_NAME_LENGTH);
		CopyMemory(&tData[MAX_GUILD_NAME_LENGTH], tGuildNotice, (4 * MAX_GUILD_NOTICE_LENGTH));
		if (!mEXTRA_COM.U_GUILD_WORK_FOR_EXTRA_SEND(tSort, &tData[0]))
		{
			mTRANSFER.B_GUILD_WORK_RECV(1, tSort, &mEXTRA_COM.mRecv_GuildInfo);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		if (mEXTRA_COM.mRecv_Result != 0)
		{
			mTRANSFER.B_GUILD_WORK_RECV(1, tSort, &mEXTRA_COM.mRecv_GuildInfo);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		mTRANSFER.B_GUILD_WORK_RECV(0, tSort, &mEXTRA_COM.mRecv_GuildInfo);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	//--------------//
	//BROADCAST_INFO//
	//--------------//
		tBroadcastInfoSort = 104;
		CopyMemory(&tBroadcastInfoData[0], &mUSER[tUserIndex].mAvatarInfo.aGuildName[0], MAX_GUILD_NAME_LENGTH);
		mCENTER_COM.U_DEMAND_BROADCAST_INFO(tBroadcastInfoSort, &tBroadcastInfoData[0]);
	//--------------//
	//--------------//
	//--------------//
		return;

	case 6: //[DISBAND]
		if ((strcmp(mUSER[tUserIndex].mAvatarInfo.aGuildName, "") == 0) || (mUSER[tUserIndex].mAvatarInfo.aGuildRole !=
			0))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		CopyMemory(&tData[0], &mUSER[tUserIndex].mAvatarInfo.aGuildName[0], MAX_GUILD_NAME_LENGTH);
		if (!mEXTRA_COM.U_GUILD_WORK_FOR_EXTRA_SEND(2, &tData[0]))
		{
			mTRANSFER.B_GUILD_WORK_RECV(1, tSort, &mEXTRA_COM.mRecv_GuildInfo);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		if (mEXTRA_COM.mRecv_Result != 0)
		{
			mTRANSFER.B_GUILD_WORK_RECV(1, tSort, &mEXTRA_COM.mRecv_GuildInfo);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		tTotalMemberNum = 0;
		for (index01 = 0; index01 < MAX_GUILD_AVATAR_NUM; index01++)
		{
			if (strcmp(mEXTRA_COM.mRecv_GuildInfo.gMemberName[index01], "") != 0)
			{
				tTotalMemberNum++;
			}
		}
		if (tTotalMemberNum != 1)
		{
			mTRANSFER.B_GUILD_WORK_RECV(2, tSort, &mEXTRA_COM.mRecv_GuildInfo);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		CopyMemory(&tData[0], &mUSER[tUserIndex].mAvatarInfo.aGuildName[0], MAX_GUILD_NAME_LENGTH);
		CopyMemory(&tData[MAX_GUILD_NAME_LENGTH], &mUSER[tUserIndex].mAvatarInfo.aName[0], MAX_AVATAR_NAME_LENGTH);
		if (!mEXTRA_COM.U_GUILD_WORK_FOR_EXTRA_SEND(tSort, &tData[0]))
		{
			mTRANSFER.B_GUILD_WORK_RECV(1, tSort, &mEXTRA_COM.mRecv_GuildInfo);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		if (mEXTRA_COM.mRecv_Result != 0)
		{
			mTRANSFER.B_GUILD_WORK_RECV(1, tSort, &mEXTRA_COM.mRecv_GuildInfo);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
	//mGAMELOG.GL_617_GUILD_MONEY(tUserIndex, 0, 3, mEXTRA_COM.mRecv_GuildInfo.gGrade);
		mTRANSFER.B_GUILD_WORK_RECV(0, tSort, &mEXTRA_COM.mRecv_GuildInfo);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	//--------------//
	//BROADCAST_INFO//
	//--------------//
		tBroadcastInfoSort = 105;
		CopyMemory(&tBroadcastInfoData[0], &mUSER[tUserIndex].mAvatarInfo.aGuildName[0], MAX_GUILD_NAME_LENGTH);
		mCENTER_COM.U_DEMAND_BROADCAST_INFO(tBroadcastInfoSort, &tBroadcastInfoData[0]);
	//--------------//
	//--------------//
	//--------------//
		strcpy(mUSER[tUserIndex].mAvatarInfo.aGuildName, "");
		mUSER[tUserIndex].mAvatarInfo.aGuildRole = 0;
		strcpy(mUSER[tUserIndex].mAvatarInfo.aCallName, "");
		strcpy(mAVATAR_OBJECT[tUserIndex].mDATA.aGuildName, "");
		mAVATAR_OBJECT[tUserIndex].mDATA.aGuildRole = 0;
		strcpy(mAVATAR_OBJECT[tUserIndex].mDATA.aCallName, "");
		return;

	case 7: //[EXPAND]
		if ((strcmp(mUSER[tUserIndex].mAvatarInfo.aGuildName, "") == 0) || (mUSER[tUserIndex].mAvatarInfo.aGuildRole !=
			0))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		CopyMemory(&tData[0], &mUSER[tUserIndex].mAvatarInfo.aGuildName[0], MAX_GUILD_NAME_LENGTH);
		if (!mEXTRA_COM.U_GUILD_WORK_FOR_EXTRA_SEND(2, &tData[0]))
		{
			mTRANSFER.B_GUILD_WORK_RECV(1, tSort, &mEXTRA_COM.mRecv_GuildInfo);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		if (mEXTRA_COM.mRecv_Result != 0)
		{
			mTRANSFER.B_GUILD_WORK_RECV(1, tSort, &mEXTRA_COM.mRecv_GuildInfo);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		tTotalMemberNum = 0;
		for (index01 = 0; index01 < MAX_GUILD_AVATAR_NUM; index01++)
		{
			if (strcmp(mEXTRA_COM.mRecv_GuildInfo.gMemberName[index01], "") != 0)
			{
				tTotalMemberNum++;
			}
		}
		if (tTotalMemberNum < (mEXTRA_COM.mRecv_GuildInfo.gGrade * 10))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		switch (mEXTRA_COM.mRecv_GuildInfo.gGrade)
		{
		case 1:
			if (mUSER[tUserIndex].mAvatarInfo.aLevel1 < 50)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			if (mUSER[tUserIndex].mAvatarInfo.aMoney < 20000000)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			tCost = 20000000;
			break;
		case 2:
			if (mUSER[tUserIndex].mAvatarInfo.aLevel1 < 70)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			if (mUSER[tUserIndex].mAvatarInfo.aMoney < 30000000)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			tCost = 30000000;
			break;
		case 3:
			if (mUSER[tUserIndex].mAvatarInfo.aLevel1 < 90)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			if (mUSER[tUserIndex].mAvatarInfo.aMoney < 40000000)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			tCost = 40000000;
			break;
		case 4:
			if (mUSER[tUserIndex].mAvatarInfo.aLevel1 < 113)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			if (mUSER[tUserIndex].mAvatarInfo.aMoney < 50000000)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			tCost = 50000000;
			break;
		default:
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		CopyMemory(&tData[0], &mUSER[tUserIndex].mAvatarInfo.aGuildName[0], MAX_GUILD_NAME_LENGTH);
		if (!mEXTRA_COM.U_GUILD_WORK_FOR_EXTRA_SEND(tSort, &tData[0]))
		{
			mTRANSFER.B_GUILD_WORK_RECV(1, tSort, &mEXTRA_COM.mRecv_GuildInfo);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		if (mEXTRA_COM.mRecv_Result != 0)
		{
			mTRANSFER.B_GUILD_WORK_RECV(1, tSort, &mEXTRA_COM.mRecv_GuildInfo);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		mTRANSFER.B_GUILD_WORK_RECV(0, tSort, &mEXTRA_COM.mRecv_GuildInfo);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	//--------------//
	//BROADCAST_INFO//
	//--------------//
		tBroadcastInfoSort = 106;
		CopyMemory(&tBroadcastInfoData[0], &mUSER[tUserIndex].mAvatarInfo.aGuildName[0], MAX_GUILD_NAME_LENGTH);
		mCENTER_COM.U_DEMAND_BROADCAST_INFO(tBroadcastInfoSort, &tBroadcastInfoData[0]);
	//--------------//
	//--------------//
	//--------------//
	//mGAMELOG.GL_617_GUILD_MONEY(tUserIndex, tCost, 2, mEXTRA_COM.mRecv_GuildInfo.gGrade);
		mUSER[tUserIndex].mAvatarInfo.aMoney -= tCost;
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(19, mUSER[tUserIndex].mAvatarInfo.aMoney);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 8: //[KICK]
		CopyMemory(&tAvatarName[0], &tData[0], MAX_AVATAR_NAME_LENGTH);
		tAvatarName[(MAX_AVATAR_NAME_LENGTH - 1)] = '\0';
		if ((strcmp(tAvatarName, "") == 0) || (strcmp(mUSER[tUserIndex].mAvatarInfo.aGuildName, "") == 0) || (mUSER[
			tUserIndex].mAvatarInfo.aGuildRole != 0))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		CopyMemory(&tData[0], &mUSER[tUserIndex].mAvatarInfo.aGuildName[0], MAX_GUILD_NAME_LENGTH);
		CopyMemory(&tData[MAX_GUILD_NAME_LENGTH], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		if (!mEXTRA_COM.U_GUILD_WORK_FOR_EXTRA_SEND(tSort, &tData[0]))
		{
			mTRANSFER.B_GUILD_WORK_RECV(1, tSort, &mEXTRA_COM.mRecv_GuildInfo);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		if (mEXTRA_COM.mRecv_Result != 0)
		{
			mTRANSFER.B_GUILD_WORK_RECV(1, tSort, &mEXTRA_COM.mRecv_GuildInfo);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		mTRANSFER.B_GUILD_WORK_RECV(0, tSort, &mEXTRA_COM.mRecv_GuildInfo);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	//--------------//
	//BROADCAST_INFO//
	//--------------//
		tBroadcastInfoSort = 107;
		CopyMemory(&tBroadcastInfoData[0], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&tBroadcastInfoData[MAX_AVATAR_NAME_LENGTH], &mUSER[tUserIndex].mAvatarInfo.aGuildName[0],
		           MAX_GUILD_NAME_LENGTH);
		mCENTER_COM.U_DEMAND_BROADCAST_INFO(tBroadcastInfoSort, &tBroadcastInfoData[0]);
	//--------------//
	//--------------//
	//--------------//
		return;

	case 9: //[임명]
		CopyMemory(&tAvatarName[0], &tData[0], MAX_AVATAR_NAME_LENGTH);
		tAvatarName[(MAX_AVATAR_NAME_LENGTH - 1)] = '\0';
		CopyMemory(&tGuildRole, &tData[MAX_AVATAR_NAME_LENGTH], 4);
		if ((strcmp(tAvatarName, "") == 0) || (tGuildRole < 1) || (tGuildRole > 2) || (
			strcmp(mUSER[tUserIndex].mAvatarInfo.aGuildName, "") == 0) || (mUSER[tUserIndex].mAvatarInfo.aGuildRole !=
			0))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		CopyMemory(&tData[0], &mUSER[tUserIndex].mAvatarInfo.aGuildName[0], MAX_GUILD_NAME_LENGTH);
		CopyMemory(&tData[MAX_GUILD_NAME_LENGTH], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&tData[(MAX_GUILD_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH)], &tGuildRole, 4);
		if (!mEXTRA_COM.U_GUILD_WORK_FOR_EXTRA_SEND(tSort, &tData[0]))
		{
			mTRANSFER.B_GUILD_WORK_RECV(1, tSort, &mEXTRA_COM.mRecv_GuildInfo);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		if (mEXTRA_COM.mRecv_Result != 0)
		{
			mTRANSFER.B_GUILD_WORK_RECV(1, tSort, &mEXTRA_COM.mRecv_GuildInfo);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		mTRANSFER.B_GUILD_WORK_RECV(0, tSort, &mEXTRA_COM.mRecv_GuildInfo);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	//--------------//
	//BROADCAST_INFO//
	//--------------//
		tBroadcastInfoSort = 108;
		CopyMemory(&tBroadcastInfoData[0], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&tBroadcastInfoData[MAX_AVATAR_NAME_LENGTH], &mUSER[tUserIndex].mAvatarInfo.aGuildName[0],
		           MAX_GUILD_NAME_LENGTH);
		CopyMemory(&tBroadcastInfoData[(MAX_AVATAR_NAME_LENGTH + MAX_GUILD_NAME_LENGTH)], &tGuildRole, 4);
		mCENTER_COM.U_DEMAND_BROADCAST_INFO(tBroadcastInfoSort, &tBroadcastInfoData[0]);
	//--------------//
	//--------------//
	//--------------//
		return;

	case 10: //[칭호]
		CopyMemory(&tAvatarName[0], &tData[0], MAX_AVATAR_NAME_LENGTH);
		tAvatarName[(MAX_AVATAR_NAME_LENGTH - 1)] = '\0';
		CopyMemory(&tCallName[0], &tData[MAX_AVATAR_NAME_LENGTH], MAX_CALL_NAME_LENGTH);
		tCallName[(MAX_CALL_NAME_LENGTH - 1)] = '\0';
		if ((strcmp(tAvatarName, "") == 0) || (strcmp(mUSER[tUserIndex].mAvatarInfo.aGuildName, "") == 0) || (mUSER[
			tUserIndex].mAvatarInfo.aGuildRole != 0))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		CopyMemory(&tData[0], &mUSER[tUserIndex].mAvatarInfo.aGuildName[0], MAX_GUILD_NAME_LENGTH);
		CopyMemory(&tData[MAX_GUILD_NAME_LENGTH], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&tData[(MAX_GUILD_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH)], &tCallName[0], MAX_CALL_NAME_LENGTH);
		if (!mEXTRA_COM.U_GUILD_WORK_FOR_EXTRA_SEND(tSort, &tData[0]))
		{
			mTRANSFER.B_GUILD_WORK_RECV(1, tSort, &mEXTRA_COM.mRecv_GuildInfo);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		if (mEXTRA_COM.mRecv_Result != 0)
		{
			mTRANSFER.B_GUILD_WORK_RECV(1, tSort, &mEXTRA_COM.mRecv_GuildInfo);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		mTRANSFER.B_GUILD_WORK_RECV(0, tSort, &mEXTRA_COM.mRecv_GuildInfo);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	//--------------//
	//BROADCAST_INFO//
	//--------------//
		tBroadcastInfoSort = 109;
		CopyMemory(&tBroadcastInfoData[0], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&tBroadcastInfoData[MAX_AVATAR_NAME_LENGTH], &mUSER[tUserIndex].mAvatarInfo.aGuildName[0],
		           MAX_GUILD_NAME_LENGTH);
		CopyMemory(&tBroadcastInfoData[(MAX_AVATAR_NAME_LENGTH + MAX_GUILD_NAME_LENGTH)], &tCallName[0],
		           MAX_CALL_NAME_LENGTH);
		mCENTER_COM.U_DEMAND_BROADCAST_INFO(tBroadcastInfoSort, &tBroadcastInfoData[0]);
	//--------------//
	//--------------//
	//--------------//
		return;

	case 11: //[문장]
		if ((strcmp(mUSER[tUserIndex].mAvatarInfo.aGuildName, "") == 0) || (mUSER[tUserIndex].mAvatarInfo.aGuildRole !=
			0) || (mUSER[tUserIndex].mAvatarInfo.aGuildMarkNum < 1))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aGuildMarkNum--;
		mTRANSFER.B_GUILD_WORK_RECV(0, tSort, &mEXTRA_COM.mRecv_GuildInfo);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 12: //[문파깃발-ON]
		if (strcmp(mUSER[tUserIndex].mAvatarInfo.aGuildName, "") == 0)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aGuildMarkEffect = 1;
		mAVATAR_OBJECT[tUserIndex].mDATA.aGuildMarkEffect = 1;
		mTRANSFER.B_GUILD_WORK_RECV(0, tSort, &mEXTRA_COM.mRecv_GuildInfo);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 13: //[문파깃발-OFF]
		if (strcmp(mUSER[tUserIndex].mAvatarInfo.aGuildName, "") == 0)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aGuildMarkEffect = 0;
		mAVATAR_OBJECT[tUserIndex].mDATA.aGuildMarkEffect = 0;
		mTRANSFER.B_GUILD_WORK_RECV(0, tSort, &mEXTRA_COM.mRecv_GuildInfo);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	default: //[오류]
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
	}
}

//W_GUILD_NOTICE_SEND
void W_GUILD_NOTICE_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	char tContent[MAX_CHAT_CONTENT_LENGTH];
	CopyMemory(&tContent[0], &tPacket[1], MAX_CHAT_CONTENT_LENGTH);
	tContent[(MAX_CHAT_CONTENT_LENGTH - 1)] = '\0';
	if (strcmp(tContent, "") == 0)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int tSort;
	BYTE tData[MAX_BROADCAST_DATA_SIZE];

	if (mUSER[tUserIndex].mAvatarInfo.aSpecialState == 2)
	{
		return;
	}
	if ((strcmp(mUSER[tUserIndex].mAvatarInfo.aGuildName, "") == 0) || (mUSER[tUserIndex].mAvatarInfo.aGuildRole != 0))
	{
		return;
	}
	tSort = 111;
	CopyMemory(&tData[0], &mUSER[tUserIndex].mAvatarInfo.aGuildName[0], MAX_GUILD_NAME_LENGTH);
	CopyMemory(&tData[MAX_GUILD_NAME_LENGTH], &mUSER[tUserIndex].mAvatarInfo.aName[0], MAX_AVATAR_NAME_LENGTH);
	CopyMemory(&tData[(MAX_GUILD_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH)], &tContent[0], MAX_CHAT_CONTENT_LENGTH);
	mRELAY_COM.U_DEMAND_BROADCAST_DATA(tSort, &tData[0]);
}

//W_GUILD_CHAT_SEND
void W_GUILD_CHAT_SEND(int tUserIndex)
{
	//unsigned int tRandom = 0;
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	char tContent[MAX_CHAT_CONTENT_LENGTH];
	CopyMemory(&tContent[0], &tPacket[1], MAX_CHAT_CONTENT_LENGTH);
	tContent[(MAX_CHAT_CONTENT_LENGTH - 1)] = '\0';
	if (strcmp(tContent, "") == 0)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int tSort;
	BYTE tData[MAX_BROADCAST_DATA_SIZE];

	if (mUSER[tUserIndex].mAvatarInfo.aSpecialState == 2)
	{
		return;
	}
	if (strcmp(mUSER[tUserIndex].mAvatarInfo.aGuildName, "") == 0)
	{
		return;
	}
	tSort = 112;
	CopyMemory(&tData[0], &mUSER[tUserIndex].mAvatarInfo.aGuildName[0], MAX_GUILD_NAME_LENGTH);
	CopyMemory(&tData[MAX_GUILD_NAME_LENGTH], &mUSER[tUserIndex].mAvatarInfo.aName[0], MAX_AVATAR_NAME_LENGTH);
	CopyMemory(&tData[(MAX_GUILD_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH)], &tContent[0], MAX_CHAT_CONTENT_LENGTH);
	mRELAY_COM.U_DEMAND_BROADCAST_DATA(tSort, &tData[0]);
	GameLogger(2, "<GUILD_CHAT>(%s),(%s), (Message: %s),[[%d]]", mUSER[tUserIndex].uID,
	           mUSER[tUserIndex].mAvatarInfo.aName, tContent, mSERVER_INFO.mServerNumber);
	//mCHATLOG.SendToLogServer(mUSER[tUserIndex].mAvatarInfo.aTribe, 1, mUSER[tUserIndex].mAvatarInfo.aName, "", tContent);
}

//W_GUILD_FIND_SEND
void W_GUILD_FIND_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	char tAvatarName[MAX_AVATAR_NAME_LENGTH];
	CopyMemory(&tAvatarName[0], &tPacket[1], MAX_AVATAR_NAME_LENGTH);
	tAvatarName[(MAX_AVATAR_NAME_LENGTH - 1)] = '\0';

	if (strcmp(mUSER[tUserIndex].mAvatarInfo.aGuildName, "") == 0)
	{
		return;
	}
	mPLAYUSER_COM.U_FIND_AVATAR_FOR_ZONE_SEND(&tAvatarName[0]);
	mTRANSFER.B_GUILD_FIND_RECV(mPLAYUSER_COM.mRecv_ZoneNumber);
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//RANK_SEND
void W_CHAR_RANK_BUFF_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int tValue01;
	CopyMemory(&tValue01, &tPacket[1], 4);
	switch (tValue01)
	{
	case 1:
		if (mUSER[tUserIndex].mAvatarInfo.aRankPoints >= 100)
		{
			mUSER[tUserIndex].mAvatarInfo.aRankAbility = tValue01;
			mTRANSFER.DEMAND_OBJECT_RECV(68, mUSER[tUserIndex].mAvatarInfo.aRankAbility);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
			return;
		}
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	case 2:
		if (mUSER[tUserIndex].mAvatarInfo.aRankPoints >= 300)
		{
			mUSER[tUserIndex].mAvatarInfo.aRankAbility = tValue01;
			mTRANSFER.DEMAND_OBJECT_RECV(68, mUSER[tUserIndex].mAvatarInfo.aRankAbility);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
			return;
		}
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	case 3:
		if (mUSER[tUserIndex].mAvatarInfo.aRankPoints >= 600)
		{
			mUSER[tUserIndex].mAvatarInfo.aRankAbility = tValue01;
			mTRANSFER.DEMAND_OBJECT_RECV(68, mUSER[tUserIndex].mAvatarInfo.aRankAbility);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
			return;
		}
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	case 4:
		if (mUSER[tUserIndex].mAvatarInfo.aRankPoints >= 1000)
		{
			mUSER[tUserIndex].mAvatarInfo.aRankAbility = tValue01;
			mTRANSFER.DEMAND_OBJECT_RECV(68, mUSER[tUserIndex].mAvatarInfo.aRankAbility);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
			return;
		}
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	case 5:
		if (mUSER[tUserIndex].mAvatarInfo.aRankPoints >= 1500)
		{
			mUSER[tUserIndex].mAvatarInfo.aRankAbility = tValue01;
			mTRANSFER.DEMAND_OBJECT_RECV(68, mUSER[tUserIndex].mAvatarInfo.aRankAbility);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
			return;
		}
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	case 6:
		if (mUSER[tUserIndex].mAvatarInfo.aRankPoints >= 2100)
		{
			mUSER[tUserIndex].mAvatarInfo.aRankAbility = tValue01;
			mTRANSFER.DEMAND_OBJECT_RECV(68, mUSER[tUserIndex].mAvatarInfo.aRankAbility);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
			return;
		}
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	case 7:
		if (mUSER[tUserIndex].mAvatarInfo.aRankPoints >= 3000)
		{
			mUSER[tUserIndex].mAvatarInfo.aRankAbility = tValue01;
			mTRANSFER.DEMAND_OBJECT_RECV(68, mUSER[tUserIndex].mAvatarInfo.aRankAbility);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
			return;
		}
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	default:
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
	}
}

//W_AUTO_CONFIG_SEND
void W_AUTO_CONFIG_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit();
		return;
	}

	ACTION_INFO pAction;
	int index01;

	if ((mUSER[tUserIndex].mAvatarInfo.aAutoHunt7Day < mUTIL.ReturnNowDate()) || (mUSER[tUserIndex].mAvatarInfo.
		aAutoHunt5Hour < 0))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	CopyMemory(&mUSER[tUserIndex].mAvatarInfo.aAutoHuntState, &tPacket[1], 4);
	CopyMemory(&mUSER[tUserIndex].mAvatarInfo.aAutoHuntUNK, &tPacket[5], 68);
	CopyMemory(&mUSER[tUserIndex].mAvatarInfo.aStandStillOrMovingHunt, &tPacket[5 + 68], 44);

	if ((mUSER[tUserIndex].mAvatarInfo.aAutoHuntState == 0) && (mAVATAR_OBJECT[tUserIndex].mDATA.aBotOn1 == 0))
	{
		return;
	}

	if ((mUSER[tUserIndex].mAvatarInfo.aAutoHuntState == 1) && (mUSER[tUserIndex].mAvatarInfo.aAutoHunt7Day != 0))
	{
		mAVATAR_OBJECT[tUserIndex].mDATA.aBotOn1 = 1;
		mTRANSFER.B_AUTOHUNT_RECV(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 1);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mAVATAR_OBJECT[tUserIndex].AutoHuntBuff();
	}
	else
	{
		mAVATAR_OBJECT[tUserIndex].mDATA.aBotOn1 = 0;
		mTRANSFER.B_AUTOHUNT_RECV(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 0);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	}
}

//W_AUTOBUFF_REGISTER_SEND
void W_AUTOBUFF_REGISTER_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit();
		return;
	}

	if (!mUSER[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	CopyMemory(&mUSER[tUserIndex].mAvatarInfo.aAutoBuffScrollSkillSlots, &tPacket[1], sizeof(AUTOBUFF_DATA));
}

//W_AUTOBUFF_SEND
void W_AUTOBUFF_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit();
		return;
	}

	if (!mUSER[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	mAVATAR_OBJECT[tUserIndex].AutoBuff(0);
}

//W_MAKE_ITEM_SEND
void W_MAKE_ITEM_SEND(int tUserIndex)
{
	unsigned int tRandom = 0;
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit();
		return;
	}

	if (!mUSER[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int tSort;
	int tPage1;
	int tIndex1;
	int tPage2;
	int tIndex2;
	int tPage3;
	int tIndex3;
	int tPage4;
	int tIndex4;
	CopyMemory(&tSort, &tPacket[1], 4);
	CopyMemory(&tPage1, &tPacket[5], 4);
	CopyMemory(&tIndex1, &tPacket[9], 4);
	CopyMemory(&tPage2, &tPacket[13], 4);
	CopyMemory(&tIndex2, &tPacket[17], 4);
	CopyMemory(&tPage3, &tPacket[21], 4);
	CopyMemory(&tIndex3, &tPacket[25], 4);
	CopyMemory(&tPage4, &tPacket[29], 4);
	CopyMemory(&tIndex4, &tPacket[33], 4);

	int tRandomNumber;
	int tValue[6];
	int tValue2[6];
	int tValue3[6];
	int tValue4[6];

	switch (tSort)
	{
	case 0: //[Tin -> Mats]
		if ((tPage1 < 0) || (tPage1 > 1) || (tIndex1 < 0) || (tIndex1 > (MAX_INVENTORY_SLOT_NUM - 1)) || (tPage2 < 0) ||
			(tPage2 > 1) || (tIndex2 < 0) || (tIndex2 > (MAX_INVENTORY_SLOT_NUM - 1)) || (tPage3 < 0) || (tPage3 > 1) ||
			(tIndex3 < 0) || (tIndex3 > (MAX_INVENTORY_SLOT_NUM - 1)) || (tPage4 < 0) || (tPage4 > 1) || (tIndex4 < 0)
			|| (tIndex4 > (MAX_INVENTORY_SLOT_NUM - 1)))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if ((tPage1 == 1) || (tPage2 == 1) || (tPage3 == 1) || (tPage4 == 1))
		{
			if (mUSER[tUserIndex].mAvatarInfo.aExpandInventoryDate < mUTIL.ReturnNowDate())
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
		}
		if ((mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0] != 1019) || (mUSER[tUserIndex].mAvatarInfo.
			aInventory[tPage2][tIndex2][0] != 1019) || (mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][0] !=
			1019) || (mUSER[tUserIndex].mAvatarInfo.aInventory[tPage4][tIndex4][0] != 1019))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		tRandom = rand();
		switch ((tRandom % 4))
		{
		case 0:
			tValue[0] = 1020;
			tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
			tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
			tValue[3] = 0;
			tValue[4] = 0;
			tValue[5] = mUTIL.ReturnItemRecognitionNumber(0);
			break;
		case 1:
			tValue[0] = 1021;
			tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
			tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
			tValue[3] = 0;
			tValue[4] = 0;
			tValue[5] = mUTIL.ReturnItemRecognitionNumber(0);
			break;
		case 2:
			tValue[0] = 1022;
			tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
			tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
			tValue[3] = 0;
			tValue[4] = 0;
			tValue[5] = mUTIL.ReturnItemRecognitionNumber(0);
			break;
		case 3:
			tValue[0] = 1023;
			tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
			tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
			tValue[3] = 0;
			tValue[4] = 0;
			tValue[5] = mUTIL.ReturnItemRecognitionNumber(0);
			break;
		}
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0] = tValue[0];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1] = tValue[1];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2] = tValue[2];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3] = tValue[3];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4] = tValue[4];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5] = tValue[5];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][5] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][5] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage4][tIndex4][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage4][tIndex4][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage4][tIndex4][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage4][tIndex4][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage4][tIndex4][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage4][tIndex4][5] = 0;
		GameLogger(1, "<MAKE_ITEM_SEND>(%s),(%s), (%d, %d, %d, %d), [[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5], mSERVER_INFO.mServerNumber);
		mTRANSFER.B_MAKE_ITEM_RECV(0, &tValue[0]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1: //[Purple Jade x2 -> Red Jade]
		if ((tPage1 < 0) || (tPage1 > 1) || (tIndex1 < 0) || (tIndex1 > (MAX_INVENTORY_SLOT_NUM - 1)) || (tPage2 < 0) ||
			(tPage2 > 1) || (tIndex2 < 0) || (tIndex2 > (MAX_INVENTORY_SLOT_NUM - 1)))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if ((tPage1 == 1) || (tPage2 == 1))
		{
			if (mUSER[tUserIndex].mAvatarInfo.aExpandInventoryDate < mUTIL.ReturnNowDate())
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
		}
		if ((mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0] != 1024) || (mUSER[tUserIndex].mAvatarInfo.
			aInventory[tPage2][tIndex2][0] != 1024))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		tValue[0] = 1025;
		tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
		tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
		tValue[3] = 0;
		tValue[4] = 0;
		tValue[5] = mUTIL.ReturnItemRecognitionNumber(0);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0] = tValue[0];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1] = tValue[1];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2] = tValue[2];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3] = tValue[3];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4] = tValue[4];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5] = tValue[5];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][5] = 0;
		GameLogger(1, "<MAKE_ITEM_SEND>(%s),(%s), (%d, %d, %d, %d), [[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5], mSERVER_INFO.mServerNumber);
		mTRANSFER.B_MAKE_ITEM_RECV(0, &tValue[0]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 40:
		if ((tPage1 < 0) || (tPage1 > 1) || (tIndex1 < 0) || (tIndex1 > (MAX_INVENTORY_SLOT_NUM - 1)) || (tPage2 < 0) ||
			(tPage2 > 1) || (tIndex2 < 0) || (tIndex2 > (MAX_INVENTORY_SLOT_NUM - 1)) || (tPage3 < 0) || (tPage3 > 1) ||
			(tIndex3 < 0) || (tIndex3 > (MAX_INVENTORY_SLOT_NUM - 1)) || (tPage4 < 0) || (tPage4 > 1) || (tIndex4 < 0)
			|| (tIndex4 > (MAX_INVENTORY_SLOT_NUM - 1)))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if ((tPage1 == 1) || (tPage2 == 1) || (tPage3 == 1) || (tPage4 == 1))
		{
			if (mUSER[tUserIndex].mAvatarInfo.aExpandInventoryDate < mUTIL.ReturnNowDate())
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
		}
		if ((mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0] != 695) || (mUSER[tUserIndex].mAvatarInfo.
			aInventory[tPage2][tIndex2][0] != 695) || (mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][0] !=
			695))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}

		switch (mUSER[tUserIndex].mAvatarInfo.aPreviousTribe)
		{
		case 0:
			tRandom = rand() % 100;
			if (tRandom == 0)
			{
				tValue[0] = 216;
				tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
				tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
				tValue[3] = 0;
				tValue[4] = 0;
				tValue[5] = 0;
			}
			else if (tRandom <= 5)
			{
				tValue[0] = 213;
				tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
				tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
				tValue[3] = 0;
				tValue[4] = 0;
				tValue[5] = 0;
			}
			else if (tRandom <= 15)
			{
				tValue[0] = 210;
				tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
				tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
				tValue[3] = 0;
				tValue[4] = 0;
				tValue[5] = 0;
			}
			else if (tRandom <= 45)
			{
				tValue[0] = 207;
				tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
				tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
				tValue[3] = 0;
				tValue[4] = 0;
				tValue[5] = 0;
			}
			else if (tRandom <= 75)
			{
				tValue[0] = 204;
				tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
				tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
				tValue[3] = 0;
				tValue[4] = 0;
				tValue[5] = 0;
			}
			else
			{
				tValue[0] = 201;
				tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
				tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
				tValue[3] = 0;
				tValue[4] = 0;
				tValue[5] = 0;
			}
			break;

		case 1:
			tRandom = rand() % 100;
			if (tRandom == 0)
			{
				tValue[0] = 217;
				tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
				tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
				tValue[3] = 0;
				tValue[4] = 0;
				tValue[5] = 0;
			}
			else if (tRandom <= 5)
			{
				tValue[0] = 214;
				tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
				tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
				tValue[3] = 0;
				tValue[4] = 0;
				tValue[5] = 0;
			}
			else if (tRandom <= 15)
			{
				tValue[0] = 211;
				tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
				tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
				tValue[3] = 0;
				tValue[4] = 0;
				tValue[5] = 0;
			}
			else if (tRandom <= 45)
			{
				tValue[0] = 208;
				tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
				tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
				tValue[3] = 0;
				tValue[4] = 0;
				tValue[5] = 0;
			}
			else if (tRandom <= 75)
			{
				tValue[0] = 205;
				tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
				tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
				tValue[3] = 0;
				tValue[4] = 0;
				tValue[5] = 0;
			}
			else
			{
				tValue[0] = 202;
				tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
				tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
				tValue[3] = 0;
				tValue[4] = 0;
				tValue[5] = 0;
			}
			break;

		case 2:
			tRandom = rand() % 100;
			if (tRandom == 0)
			{
				tValue[0] = 218;
				tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
				tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
				tValue[3] = 0;
				tValue[4] = 0;
				tValue[5] = 0;
			}
			else if (tRandom <= 5)
			{
				tValue[0] = 215;
				tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
				tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
				tValue[3] = 0;
				tValue[4] = 0;
				tValue[5] = 0;
			}
			else if (tRandom <= 15)
			{
				tValue[0] = 212;
				tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
				tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
				tValue[3] = 0;
				tValue[4] = 0;
				tValue[5] = 0;
			}
			else if (tRandom <= 45)
			{
				tValue[0] = 209;
				tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
				tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
				tValue[3] = 0;
				tValue[4] = 0;
				tValue[5] = 0;
			}
			else if (tRandom <= 75)
			{
				tValue[0] = 206;
				tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
				tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
				tValue[3] = 0;
				tValue[4] = 0;
				tValue[5] = 0;
			}
			else
			{
				tValue[0] = 203;
				tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
				tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
				tValue[3] = 0;
				tValue[4] = 0;
				tValue[5] = 0;
			}
			break;
		}
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0] = tValue[0];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1] = tValue[1];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2] = tValue[2];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3] = tValue[3];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4] = tValue[4];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5] = tValue[5];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][5] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][5] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage4][tIndex4][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage4][tIndex4][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage4][tIndex4][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage4][tIndex4][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage4][tIndex4][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage4][tIndex4][5] = 0;
		GameLogger(1, "<MAKE_ITEM_SEND>(%s),(%s), (%d, %d, %d, %d), [[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5], mSERVER_INFO.mServerNumber);
		mTRANSFER.B_MAKE_ITEM_RECV(0, &tValue[0]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe -= 50;
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(3, mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 41:
		if ((tPage1 < 0) || (tPage1 > 1) || (tIndex1 < 0) || (tIndex1 > (MAX_INVENTORY_SLOT_NUM - 1)) || (tPage2 < 0) ||
			(tPage2 > 1) || (tIndex2 < 0) || (tIndex2 > (MAX_INVENTORY_SLOT_NUM - 1)) || (tPage3 < 0) || (tPage3 > 1) ||
			(tIndex3 < 0) || (tIndex3 > (MAX_INVENTORY_SLOT_NUM - 1)))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if ((tPage1 == 1) || (tPage2 == 1) || (tPage3 == 1) || (tPage4 == 1))
		{
			if (mUSER[tUserIndex].mAvatarInfo.aExpandInventoryDate < mUTIL.ReturnNowDate())
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
		}
		if ((mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0] != 695) || (mUSER[tUserIndex].mAvatarInfo.
			aInventory[tPage2][tIndex2][0] != 695) || (mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][0] !=
			695))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}

		tValue[0] = 696;
		tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
		tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
		tValue[3] = 0;
		tValue[4] = 0;
		tValue[5] = mUTIL.ReturnItemRecognitionNumber(0);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0] = tValue[0];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1] = tValue[1];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2] = tValue[2];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3] = tValue[3];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4] = tValue[4];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5] = tValue[5];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][5] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][5] = 0;
		GameLogger(1, "<MAKE_ITEM_SEND>(%s),(%s), (%d, %d, %d, %d), [[%d]]", mUSER[tUserIndex].uID,
		           mUSER[tUserIndex].mAvatarInfo.aName, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4],
		           mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5], mSERVER_INFO.mServerNumber);
		mTRANSFER.B_MAKE_ITEM_RECV(0, &tValue[0]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	default:
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
	}
}

//W_PET_COMBINE_SEND
void W_PET_COMBINE_SEND(int tUserIndex)
{
	unsigned int tRandom = 0;
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit();
		return;
	}

	if (!mUSER[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int tSort;
	int tPage1;
	int tIndex1;
	int tPage2;
	int tIndex2;
	int tPage3;
	int tIndex3;
	int tPage4;
	int tIndex4;
	CopyMemory(&tSort, &tPacket[1], 4);
	CopyMemory(&tPage1, &tPacket[5], 4);
	CopyMemory(&tIndex1, &tPacket[9], 4);
	CopyMemory(&tPage2, &tPacket[13], 4);
	CopyMemory(&tIndex2, &tPacket[17], 4);
	CopyMemory(&tPage3, &tPacket[21], 4);
	CopyMemory(&tIndex3, &tPacket[25], 4);
	CopyMemory(&tPage4, &tPacket[29], 4);
	CopyMemory(&tIndex4, &tPacket[33], 4);

	int tRandomNumber;
	int tValue[6];
	int tValue2[6];
	int tValue3[6];
	int tValue4[6];
	int a;
	int b;
	int c;

	switch (tSort)
	{
	case 0:
		if ((tPage1 < 0) || (tPage1 > 1) || (tIndex1 < 0) || (tIndex1 > (MAX_INVENTORY_SLOT_NUM - 1)) || (tPage2 < 0) ||
			(tPage2 > 1) || (tIndex2 < 0) || (tIndex2 > (MAX_INVENTORY_SLOT_NUM - 1)) || (tPage3 < 0) || (tPage3 > 1) ||
			(tIndex3 < 0) || (tIndex3 > (MAX_INVENTORY_SLOT_NUM - 1)) || (tPage4 < 0) || (tPage4 > 1) || (tIndex4 < 0)
			|| (tIndex4 > (MAX_INVENTORY_SLOT_NUM - 1)))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if ((tPage1 == 1) || (tPage2 == 1) || (tPage3 == 1) || (tPage4 == 1))
		{
			if (mUSER[tUserIndex].mAvatarInfo.aExpandInventoryDate < mUTIL.ReturnNowDate())
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
		}
		if ((mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0] == 1002) ||
			(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0] == 1003) ||
			(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0] == 1004) ||
			(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0] == 1005))
		{
			a = 1;
		}
		if ((mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] == 1002) ||
			(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] == 1003) ||
			(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] == 1004) ||
			(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] == 1005))
		{
			b = 2;
		}
		if ((mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][0] == 1002) ||
			(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][0] == 1003) ||
			(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][0] == 1004) ||
			(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][0] == 1005))
		{
			c = 3;
		}
		if ((a != 1) || (b != 2) || (c != 3) || (mUSER[tUserIndex].mAvatarInfo.aInventory[tPage4][tIndex4][0] != 1235))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}

		tRandom = rand();
		switch ((tRandom % 6))
		{
		case 0:
			tValue[0] = 1006;
			tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
			tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
			tValue[3] = 0;
			tValue[4] = 0;
			tValue[5] = mUTIL.ReturnItemRecognitionNumber(0);
			break;
		case 1:
			tValue[0] = 1007;
			tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
			tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
			tValue[3] = 0;
			tValue[4] = 0;
			tValue[5] = mUTIL.ReturnItemRecognitionNumber(0);
			break;
		case 2:
			tValue[0] = 1008;
			tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
			tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
			tValue[3] = 0;
			tValue[4] = 0;
			tValue[5] = mUTIL.ReturnItemRecognitionNumber(0);
			break;
		case 3:
			tValue[0] = 1009;
			tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
			tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
			tValue[3] = 0;
			tValue[4] = 0;
			tValue[5] = mUTIL.ReturnItemRecognitionNumber(0);
			break;
		case 4:
			tValue[0] = 1010;
			tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
			tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
			tValue[3] = 0;
			tValue[4] = 0;
			tValue[5] = mUTIL.ReturnItemRecognitionNumber(0);
			break;
		case 5:
			tValue[0] = 1011;
			tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
			tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
			tValue[3] = 0;
			tValue[4] = 0;
			tValue[5] = mUTIL.ReturnItemRecognitionNumber(0);
			break;
		}
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0] = tValue[0];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1] = tValue[1];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2] = tValue[2];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3] = tValue[3];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4] = tValue[4];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5] = tValue[5];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][5] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][5] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage4][tIndex4][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage4][tIndex4][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage4][tIndex4][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage4][tIndex4][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage4][tIndex4][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage4][tIndex4][5] = 0;
		mTRANSFER.B_MAKE_ITEM_RECV(0, &tValue[0]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 1:
		if ((tPage1 < 0) || (tPage1 > 1) || (tIndex1 < 0) || (tIndex1 > (MAX_INVENTORY_SLOT_NUM - 1)) || (tPage2 < 0) ||
			(tPage2 > 1) || (tIndex2 < 0) || (tIndex2 > (MAX_INVENTORY_SLOT_NUM - 1)) || (tPage3 < 0) || (tPage3 > 1) ||
			(tIndex3 < 0) || (tIndex3 > (MAX_INVENTORY_SLOT_NUM - 1)) || (tPage4 < 0) || (tPage4 > 1) || (tIndex4 < 0)
			|| (tIndex4 > (MAX_INVENTORY_SLOT_NUM - 1)))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if ((tPage1 == 1) || (tPage2 == 1) || (tPage3 == 1) || (tPage4 == 1))
		{
			if (mUSER[tUserIndex].mAvatarInfo.aExpandInventoryDate < mUTIL.ReturnNowDate())
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
		}
		if ((mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0] == 1006) ||
			(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0] == 1007) ||
			(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0] == 1008) ||
			(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0] == 1009) ||
			(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0] == 1010) ||
			(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0] == 1011))
		{
			a = 1;
		}
		if ((mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] == 1006) ||
			(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] == 1007) ||
			(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] == 1008) ||
			(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] == 1009) ||
			(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] == 1010) ||
			(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] == 1011))
		{
			b = 2;
		}
		if ((mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][0] == 1006) ||
			(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][0] == 1007) ||
			(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][0] == 1008) ||
			(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][0] == 1009) ||
			(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][0] == 1010) ||
			(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][0] == 1011))
		{
			c = 3;
		}
		if ((a != 1) || (b != 2) || (c != 3) || (mUSER[tUserIndex].mAvatarInfo.aInventory[tPage4][tIndex4][0] != 1235))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		tRandom = rand();
		switch ((tRandom % 4))
		{
		case 0:
			tValue[0] = 1012;
			tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
			tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
			tValue[3] = 0;
			tValue[4] = 0;
			tValue[5] = mUTIL.ReturnItemRecognitionNumber(0);
			break;
		case 1:
			tValue[0] = 1013;
			tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
			tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
			tValue[3] = 0;
			tValue[4] = 0;
			tValue[5] = mUTIL.ReturnItemRecognitionNumber(0);
			break;
		case 2:
			tValue[0] = 1014;
			tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
			tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
			tValue[3] = 0;
			tValue[4] = 0;
			tValue[5] = mUTIL.ReturnItemRecognitionNumber(0);
			break;
		case 3:
			tValue[0] = 1015;
			tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
			tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
			tValue[3] = 0;
			tValue[4] = 0;
			tValue[5] = mUTIL.ReturnItemRecognitionNumber(0);
			break;
		}
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0] = tValue[0];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1] = tValue[1];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2] = tValue[2];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3] = tValue[3];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4] = tValue[4];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5] = tValue[5];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][5] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage3][tIndex3][5] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage4][tIndex4][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage4][tIndex4][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage4][tIndex4][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage4][tIndex4][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage4][tIndex4][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage4][tIndex4][5] = 0;
		mTRANSFER.B_MAKE_ITEM_RECV(0, &tValue[0]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	default:
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
	}
}

//W_GEAR_BREAK_SEND
void W_GEAR_BREAK_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	if (!mUSER[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int tPage1;
	int tIndex1;
	CopyMemory(&tPage1, &tPacket[1], 4);
	CopyMemory(&tIndex1, &tPacket[5], 4);

	int tMoney;
	int tValue[6];
	ITEM_INFO* tITEM_INFO1;
	int EquipPercent = mUTIL.ReturnISValue(
		mUTIL.ReturnISValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]));


	tITEM_INFO1 = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0]);
	if (tITEM_INFO1 == nullptr)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (tITEM_INFO1->iType != 3)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (tITEM_INFO1->iSort < 7)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	if (EquipPercent - 1 > 9)
	{
		if (EquipPercent - 11 > 9)
		{
			if (EquipPercent - 21 > 9)
			{
				if (EquipPercent - 31 > 8)
				{
					if (EquipPercent != 40)
					{
						mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
						return;
					}
					tValue[0] = 1437;
					tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
					tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
					tValue[3] = 0;
					tValue[4] = 0;
					tValue[5] = 0;
					tMoney = 10000000;
				}
				else
				{
					tValue[0] = 1023;
					tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
					tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
					tValue[3] = 0;
					tValue[4] = 0;
					tValue[5] = 0;
					tMoney = 5000000;
				}
			}
			else
			{
				tValue[0] = 1022;
				tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
				tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
				tValue[3] = 0;
				tValue[4] = 0;
				tValue[5] = 0;
				tMoney = 3000000;
			}
		}
		else
		{
			tValue[0] = 1021;
			tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
			tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
			tValue[3] = 0;
			tValue[4] = 0;
			tValue[5] = 0;
			tMoney = 2000000;
		}
	}
	else
	{
		tValue[0] = 1021;
		tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
		tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
		tValue[3] = 0;
		tValue[4] = 0;
		tValue[5] = 0;
		tMoney = 1000000;
	}

	// Protection
	if (tValue[0] == NULL)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0] = tValue[0];
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1] = tValue[1];
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2] = tValue[2];
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3] = tValue[3];
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4] = tValue[4];
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5] = tValue[5];
	mUSER[tUserIndex].mAvatarInfo.aMoney += tMoney;
	mTRANSFER.B_GEAR_BREAK_RECV(0, tMoney, &tValue[0]);
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_PARTY_ASK_SEND
void W_PARTY_ASK_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}
	if (124 == mSERVER_INFO.mServerNumber)
		return;

	char tAvatarName[MAX_AVATAR_NAME_LENGTH];
	CopyMemory(&tAvatarName[0], &tPacket[1], MAX_AVATAR_NAME_LENGTH);
	tAvatarName[(MAX_AVATAR_NAME_LENGTH - 1)] = '\0';

	int tOtherAvatarIndex;
	int index01;

	/*
	for (index01 = 0; index01 < 5; index01++)
	{
		if (strcmp(mUSER[tUserIndex].mAvatarInfo.aPartyName[index01], "") != 0)
		{
			if (strcmp(mUSER[tUserIndex].mAvatarInfo.aPartyName[index01], mUSER[tUserIndex].mAvatarInfo.aName) != 0)
			{
				mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
		}
	}*/
	if (mAVATAR_OBJECT[tUserIndex].CheckCommunityWork())
	{
		mTRANSFER.B_PARTY_ANSWER_RECV(3);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if ((mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aSort == 11) || (mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aSort ==
		12))
	{
		mTRANSFER.B_PARTY_ANSWER_RECV(3);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	tOtherAvatarIndex = mUTIL.SearchAvatar(tAvatarName, tUserIndex);
	if (tOtherAvatarIndex == -1)
	{
		mTRANSFER.B_PARTY_ANSWER_RECV(4);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if (strcmp(mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[0], "") != 0)
	{
		mTRANSFER.B_PARTY_ANSWER_RECV(6);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if ((mAVATAR_OBJECT[tUserIndex].mDATA.aTribe != mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aTribe) && (mAVATAR_OBJECT[
		tUserIndex].mDATA.aTribe != mGAME.ReturnAllianceTribe(mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aTribe)))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
#ifdef __GOD__
	if (abs((mUSER[tUserIndex].mAvatarInfo.aLevel1 + mUSER[tUserIndex].mAvatarInfo.aLevel2)
		- (mUSER[tOtherAvatarIndex].mAvatarInfo.aLevel1 + mUSER[tOtherAvatarIndex].mAvatarInfo.aLevel2)) > 9)
#else
	if (abs(mUSER[tUserIndex].mAvatarInfo.aLevel1 - mUSER[tOtherAvatarIndex].mAvatarInfo.aLevel1) > 9)
#endif
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if ((mUSER[tOtherAvatarIndex].mMoveZoneResult == 1) || (mAVATAR_OBJECT[tOtherAvatarIndex].CheckCommunityWork()))
	{
		mTRANSFER.B_PARTY_ANSWER_RECV(5);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if ((mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aAction.aSort == 11) || (mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.
		aAction.aSort == 12))
	{
		mTRANSFER.B_PARTY_ANSWER_RECV(5);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	mAVATAR_OBJECT[tUserIndex].mPartyProcessState = 1;
	strncpy(mAVATAR_OBJECT[tUserIndex].mPartyProcessAvatarName, mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aName,
	        MAX_AVATAR_NAME_LENGTH);
	mAVATAR_OBJECT[tOtherAvatarIndex].mPartyProcessState = 2;
	strncpy(mAVATAR_OBJECT[tOtherAvatarIndex].mPartyProcessAvatarName, mAVATAR_OBJECT[tUserIndex].mDATA.aName,
	        MAX_AVATAR_NAME_LENGTH);
	mTRANSFER.B_PARTY_ASK_RECV(&mUSER[tUserIndex].mAvatarInfo.aName[0]);
	mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_PARTY_CANCEL_SEND
void W_PARTY_CANCEL_SEND(int tUserIndex)
{
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	int tOtherAvatarIndex;

	if (mAVATAR_OBJECT[tUserIndex].mPartyProcessState != 1)
	{
		return;
	}
	mAVATAR_OBJECT[tUserIndex].mPartyProcessState = 0;
	tOtherAvatarIndex = mUTIL.SearchAvatar(mAVATAR_OBJECT[tUserIndex].mPartyProcessAvatarName, tUserIndex);
	if (tOtherAvatarIndex == -1)
	{
		return;
	}
	if (mAVATAR_OBJECT[tOtherAvatarIndex].mPartyProcessState != 2)
	{
		return;
	}
	if (strcmp(mAVATAR_OBJECT[tOtherAvatarIndex].mPartyProcessAvatarName, mAVATAR_OBJECT[tUserIndex].mDATA.aName) != 0)
	{
		return;
	}
	if (mUSER[tOtherAvatarIndex].mMoveZoneResult == 1)
	{
		return;
	}
	mAVATAR_OBJECT[tOtherAvatarIndex].mPartyProcessState = 0;
	mTRANSFER.B_PARTY_CANCEL_RECV();
	mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_PARTY_ANSWER_SEND
void W_PARTY_ANSWER_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	int tAnswer;
	int index01;
	int index02;
	BYTE tData[MAX_BROADCAST_DATA_SIZE];
	CopyMemory(&tAnswer, &tPacket[1], 4);

	int tOtherAvatarIndex;

	if (mAVATAR_OBJECT[tUserIndex].mPartyProcessState != 2)
	{
		return;
	}
	switch (tAnswer)
	{
	case 0:
		mAVATAR_OBJECT[tUserIndex].mPartyProcessState = 3;
		break;
	case 1:
		mAVATAR_OBJECT[tUserIndex].mPartyProcessState = 0;
		break;
	case 2:
		mAVATAR_OBJECT[tUserIndex].mPartyProcessState = 0;
		break;
	default:
		return;
	}
	tOtherAvatarIndex = mUTIL.SearchAvatar(mAVATAR_OBJECT[tUserIndex].mPartyProcessAvatarName, tUserIndex);
	if (tOtherAvatarIndex == -1)
	{
		return;
	}
	if (mAVATAR_OBJECT[tOtherAvatarIndex].mPartyProcessState != 1)
	{
		return;
	}
	if (strcmp(mAVATAR_OBJECT[tOtherAvatarIndex].mPartyProcessAvatarName, mAVATAR_OBJECT[tUserIndex].mDATA.aName) != 0)
	{
		return;
	}
	if (mUSER[tOtherAvatarIndex].mMoveZoneResult == 1)
	{
		return;
	}
	switch (tAnswer)
	{
	case 0:
		mAVATAR_OBJECT[tOtherAvatarIndex].mPartyProcessState = 3;
		break;
	case 1:
		mAVATAR_OBJECT[tOtherAvatarIndex].mPartyProcessState = 0;
		break;
	case 2:
		mAVATAR_OBJECT[tOtherAvatarIndex].mPartyProcessState = 0;
		break;
	default:
		return;
	}
	mTRANSFER.B_PARTY_ANSWER_RECV(tAnswer);
	mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	if (tAnswer != 0)
	{
		return;
	}

	//PARTY MAKE
	for (index01 = 0; index01 < 5; index01++)
	{
		if (strcmp(mUSER[tUserIndex].mAvatarInfo.aPartyName[index01], "") != 0)
		{
		}
	}
	tOtherAvatarIndex = mUTIL.SearchAvatar(mAVATAR_OBJECT[tUserIndex].mPartyProcessAvatarName, tUserIndex);
	if (strcmp(mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[0], "") == 0)
	{
		strncpy(mUSER[tUserIndex].mAvatarInfo.aPartyName[0], mUSER[tOtherAvatarIndex].mAvatarInfo.aName,
		        MAX_AVATAR_NAME_LENGTH);
		strncpy(mUSER[tUserIndex].mAvatarInfo.aPartyName[1], mUSER[tUserIndex].mAvatarInfo.aName,
		        MAX_AVATAR_NAME_LENGTH);
		strncpy(mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[0], mUSER[tOtherAvatarIndex].mAvatarInfo.aName,
		        MAX_AVATAR_NAME_LENGTH);
		strncpy(mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[1], mUSER[tUserIndex].mAvatarInfo.aName,
		        MAX_AVATAR_NAME_LENGTH);
		strncpy(mAVATAR_OBJECT[tUserIndex].mDATA.aPartyName, mUSER[tOtherAvatarIndex].mAvatarInfo.aName,
		        MAX_AVATAR_NAME_LENGTH);
		strncpy(mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aPartyName, mUSER[tOtherAvatarIndex].mAvatarInfo.aName,
		        MAX_AVATAR_NAME_LENGTH);
		mTRANSFER.B_PARTY_MAKE_RECV(1, mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[0],
		                            mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[1],
		                            mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[2],
		                            mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[3],
		                            mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[4]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mAVATAR_OBJECT[tUserIndex].mPartyProcessState = 0;
		mAVATAR_OBJECT[tOtherAvatarIndex].mPartyProcessState = 0;
		return;
	}
	/*
	if ( (strcmp(mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[0], mUSER[tOtherAvatarIndex].mAvatarInfo.aName) == 0) && strcmp(mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[2], "") == 0)
	{
		strncpy(mUSER[tUserIndex].mAvatarInfo.aPartyName[0], mUSER[tOtherAvatarIndex].mAvatarInfo.aName, MAX_AVATAR_NAME_LENGTH);
		strncpy(mUSER[tUserIndex].mAvatarInfo.aPartyName[1], mUSER[tUserIndex].mAvatarInfo.aName, MAX_AVATAR_NAME_LENGTH);
		strncpy(mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[0], mUSER[tOtherAvatarIndex].mAvatarInfo.aName, MAX_AVATAR_NAME_LENGTH);
		strncpy(mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[1], mUSER[tUserIndex].mAvatarInfo.aName, MAX_AVATAR_NAME_LENGTH);
		strncpy(mAVATAR_OBJECT[tUserIndex].mDATA.aPartyName, mUSER[tOtherAvatarIndex].mAvatarInfo.aName, MAX_AVATAR_NAME_LENGTH);
		strncpy(mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aPartyName, mUSER[tOtherAvatarIndex].mAvatarInfo.aName, MAX_AVATAR_NAME_LENGTH);
		mTRANSFER.B_PARTY_MAKE_RECV(1, mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[0], mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[1], mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[2], mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[3], mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[4]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mAVATAR_OBJECT[tUserIndex].mPartyProcessState = 0;
		mAVATAR_OBJECT[tOtherAvatarIndex].mPartyProcessState = 0;
		return;
	}*/
	if (strcmp(mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[2], "") == 0)
	{
		strncpy(mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[2], mUSER[tUserIndex].mAvatarInfo.aName,
		        MAX_AVATAR_NAME_LENGTH);
		//strncpy(mUSER[tUserIndex].mAvatarInfo.aPartyName[0], mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[0], 65);
		strncpy(mAVATAR_OBJECT[tUserIndex].mDATA.aPartyName, mUSER[tOtherAvatarIndex].mAvatarInfo.aName,
		        MAX_AVATAR_NAME_LENGTH);
		strncpy(mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aPartyName, mUSER[tOtherAvatarIndex].mAvatarInfo.aName,
		        MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&tData[0], &mUSER[tOtherAvatarIndex].mAvatarInfo.aName[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&tData[MAX_AVATAR_NAME_LENGTH], &mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[0],
		           MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&tData[MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH],
		           &mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[1], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&tData[MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH],
		           &mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[2], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(
			&tData[MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH],
			&mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[3], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(
			&tData[MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH +
				MAX_AVATAR_NAME_LENGTH], &mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[4], MAX_AVATAR_NAME_LENGTH);
		mRELAY_COM.U_DEMAND_BROADCAST_DATA(115, &tData[0]);
		/*
		for (index01 = 0; index01 < mSERVER.mMAX_USER_NUM; index01++)
		{
			if (strcmp(mAVATAR_OBJECT[index01].mDATA.aPartyName, mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aPartyName) == 0)
			{
				for (index02 = 0; index02 < 5; index02++)
				{
					strncpy(mUSER[index01].mAvatarInfo.aPartyName[index02], mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[index02], MAX_AVATAR_NAME_LENGTH);
				}
				mTRANSFER.B_PARTY_MAKE_RECV(1, mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[0], mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[1], mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[2], mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[3], mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[4]);
				mUSER[index01].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			}
		}*/
		mAVATAR_OBJECT[tUserIndex].mPartyProcessState = 0;
		mAVATAR_OBJECT[tOtherAvatarIndex].mPartyProcessState = 0;
		return;
	}
	if (strcmp(mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[3], "") == 0)
	{
		strncpy(mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[3], mUSER[tUserIndex].mAvatarInfo.aName,
		        MAX_AVATAR_NAME_LENGTH);
		strncpy(mAVATAR_OBJECT[tUserIndex].mDATA.aPartyName, mUSER[tOtherAvatarIndex].mAvatarInfo.aName,
		        MAX_AVATAR_NAME_LENGTH);
		strncpy(mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aPartyName, mUSER[tOtherAvatarIndex].mAvatarInfo.aName,
		        MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&tData[0], &mUSER[tOtherAvatarIndex].mAvatarInfo.aName[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&tData[MAX_AVATAR_NAME_LENGTH], &mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[0],
		           MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&tData[MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH],
		           &mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[1], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&tData[MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH],
		           &mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[2], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(
			&tData[MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH],
			&mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[3], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(
			&tData[MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH +
				MAX_AVATAR_NAME_LENGTH], &mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[4], MAX_AVATAR_NAME_LENGTH);
		mRELAY_COM.U_DEMAND_BROADCAST_DATA(115, &tData[0]);
		/*for (index01 = 0; index01 < mSERVER.mMAX_USER_NUM; index01++)
		{
			if (strcmp(mAVATAR_OBJECT[index01].mDATA.aPartyName, mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aPartyName) == 0)
			{
				for (index02 = 0; index02 < 5; index02++)
				{
					strncpy(mUSER[index01].mAvatarInfo.aPartyName[index02], mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[index02], MAX_AVATAR_NAME_LENGTH);
				}
				mTRANSFER.B_PARTY_MAKE_RECV(1, mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[0], mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[1], mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[2], mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[3], mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[4]);
				mUSER[index01].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			}
		}*/
		mAVATAR_OBJECT[tUserIndex].mPartyProcessState = 0;
		mAVATAR_OBJECT[tOtherAvatarIndex].mPartyProcessState = 0;
		return;
	}
	if (strcmp(mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[4], "") == 0)
	{
		strncpy(mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[4], mUSER[tUserIndex].mAvatarInfo.aName,
		        MAX_AVATAR_NAME_LENGTH);
		strncpy(mAVATAR_OBJECT[tUserIndex].mDATA.aPartyName, mUSER[tOtherAvatarIndex].mAvatarInfo.aName,
		        MAX_AVATAR_NAME_LENGTH);
		strncpy(mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aPartyName, mUSER[tOtherAvatarIndex].mAvatarInfo.aName,
		        MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&tData[0], &mUSER[tOtherAvatarIndex].mAvatarInfo.aName[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&tData[MAX_AVATAR_NAME_LENGTH], &mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[0],
		           MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&tData[MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH],
		           &mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[1], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&tData[MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH],
		           &mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[2], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(
			&tData[MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH],
			&mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[3], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(
			&tData[MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH +
				MAX_AVATAR_NAME_LENGTH], &mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[4], MAX_AVATAR_NAME_LENGTH);
		mRELAY_COM.U_DEMAND_BROADCAST_DATA(115, &tData[0]);
		/*{
			if (strcmp(mAVATAR_OBJECT[index01].mDATA.aPartyName, mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aPartyName) == 0)
			{
				for (index02 = 0; index02 < 5; index02++)
				{
					strncpy(mUSER[index01].mAvatarInfo.aPartyName[index02], mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[index02], MAX_AVATAR_NAME_LENGTH);
				}
				mTRANSFER.B_PARTY_MAKE_RECV(1, mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[0], mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[1], mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[2], mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[3], mUSER[tOtherAvatarIndex].mAvatarInfo.aPartyName[4]);
				mUSER[index01].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			}
		}*/
		mAVATAR_OBJECT[tUserIndex].mPartyProcessState = 0;
		mAVATAR_OBJECT[tOtherAvatarIndex].mPartyProcessState = 0;
	}
}

//W_PARTY_CHAT_SEND
void W_PARTY_CHAT_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	char tContent[MAX_CHAT_CONTENT_LENGTH];
	CopyMemory(&tContent[0], &tPacket[1], MAX_CHAT_CONTENT_LENGTH);
	tContent[(MAX_CHAT_CONTENT_LENGTH - 1)] = '\0';
	if (strcmp(tContent, "") == 0)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}


	int tSort;
	BYTE tData[MAX_BROADCAST_DATA_SIZE];

	if (mUSER[tUserIndex].mAvatarInfo.aSpecialState == 2)
	{
		return;
	}
	/*if (strcmp(mUSER[tUserIndex].mAvatarInfo.aPartyName, "") == 0)
	{
		return;
	}*/
	tSort = 105;
	CopyMemory(&tData[0], &mUSER[tUserIndex].mAvatarInfo.aPartyName[0], MAX_PARTY_NAME_LENGTH);
	CopyMemory(&tData[MAX_PARTY_NAME_LENGTH], &mUSER[tUserIndex].mAvatarInfo.aName[0], MAX_AVATAR_NAME_LENGTH);
	CopyMemory(&tData[(MAX_PARTY_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH)], &tContent[0], MAX_CHAT_CONTENT_LENGTH);
	mRELAY_COM.U_DEMAND_BROADCAST_DATA(tSort, &tData[0]);
	GameLogger(2, "<PARTY_CHAT>(%s),(%s), (Message: %s),[[%d]]", mUSER[tUserIndex].uID,
	           mUSER[tUserIndex].mAvatarInfo.aName, tContent, mSERVER_INFO.mServerNumber);
}

//W_PARTY_LEAVE_SEND
void W_PARTY_LEAVE_SEND(int tUserIndex)
{
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	int tSort;
	int index01;
	BYTE tData[MAX_BROADCAST_DATA_SIZE];

	/*for (index01 = 0; index01 < 5; index01++)
	{
		if (strcmp(mUSER[tUserIndex].mAvatarInfo.aPartyName[index01], "") == 0)
		{
			return;
		}
		if (strcmp(mUSER[tUserIndex].mAvatarInfo.aPartyName[index01], mUSER[tUserIndex].mAvatarInfo.aName) == 0)
		{
			return;
		}
	}*/
	tSort = 118;
	CopyMemory(&tData[0], &mUSER[tUserIndex].mAvatarInfo.aPartyName[0], MAX_PARTY_NAME_LENGTH);
	CopyMemory(&tData[MAX_PARTY_NAME_LENGTH], &mUSER[tUserIndex].mAvatarInfo.aName[0], MAX_AVATAR_NAME_LENGTH);
	mRELAY_COM.U_DEMAND_BROADCAST_DATA(tSort, &tData[0]);
}

//W_PARTY_EXILE_SEND
void W_PARTY_EXILE_SEND(int tUserIndex)
{
	Logger("RUN 1");
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	Logger("RUN 2");
	char tAvatarName[MAX_AVATAR_NAME_LENGTH];
	CopyMemory(&tAvatarName[0], &tPacket[1], MAX_AVATAR_NAME_LENGTH);
	tAvatarName[(MAX_AVATAR_NAME_LENGTH - 1)] = '\0';

	int tSort;
	int index01;
	BYTE tData[MAX_BROADCAST_DATA_SIZE];

	/*for (index01 = 0; index01 < 5; index01++)
	{
		if (strncmp(mUSER[tUserIndex].mAvatarInfo.aPartyName[index01], tAvatarName, MAX_AVATAR_NAME_LENGTH) != 0)
		{
			return;
		}
	}*/


	CopyMemory(&tData[0], &mUSER[tUserIndex].mAvatarInfo.aPartyName[0], MAX_PARTY_NAME_LENGTH);
	CopyMemory(&tData[MAX_PARTY_NAME_LENGTH], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
	mRELAY_COM.U_DEMAND_BROADCAST_DATA(116, &tData[0]);
}

//W_PARTY_BREAK_SEND
void W_PARTY_BREAK_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	char tAvatarName[MAX_AVATAR_NAME_LENGTH];
	CopyMemory(&tAvatarName[0], &tPacket[1], MAX_AVATAR_NAME_LENGTH);
	tAvatarName[(MAX_AVATAR_NAME_LENGTH - 1)] = '\0';

	int tSort;
	int index01;
	BYTE tData[MAX_BROADCAST_DATA_SIZE];

	/*for (index01 = 0; index01 < 5; index01++)
	{
		if (strcmp(mUSER[tUserIndex].mAvatarInfo.aPartyName[index01], tAvatarName) != 0)
		{
			return;
		}
	}*/
	tSort = 117;
	CopyMemory(&tData[0], &mUSER[tUserIndex].mAvatarInfo.aPartyName[0], MAX_PARTY_NAME_LENGTH);
	CopyMemory(&tData[MAX_PARTY_NAME_LENGTH], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
	mRELAY_COM.U_DEMAND_BROADCAST_DATA(tSort, &tData[0]);
}

//W_REFINE_SEND
void W_REFINE_SEND(int tUserIndex)
{
	unsigned int tRandom = 0;
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	int tBroadcastInfoSort;
	BYTE tBroadcastInfoData[MAX_BROADCAST_INFO_SIZE];

	int tPage1;
	int tIndex1;
	int tPage2;
	int tIndex2;
	CopyMemory(&tPage1, &tPacket[1], 4);
	CopyMemory(&tIndex1, &tPacket[5], 4);
	CopyMemory(&tPage2, &tPacket[9], 4);
	CopyMemory(&tIndex2, &tPacket[13], 4);
	if ((tPage1 < 0) || (tPage1 > 1) || (tIndex1 < 0) || (tIndex1 > (MAX_INVENTORY_SLOT_NUM - 1)) || (tPage2 < 0) || (
		tPage2 > 1) || (tIndex2 < 0) || (tIndex2 > (MAX_INVENTORY_SLOT_NUM - 1)))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	ITEM_INFO* tITEM_INFO1;
	ITEM_INFO* tITEM_INFO2;
	int iValue;
	int tVisualValue;
	int tCPCost;
	int tProbability1;
	int tProbability2;

	if ((tPage1 == 1) || (tPage2 == 1))
	{
		if (mUSER[tUserIndex].mAvatarInfo.aExpandInventoryDate < mUTIL.ReturnNowDate())
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
	}
	tITEM_INFO1 = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0]);
	if (tITEM_INFO1 == nullptr)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if ((tITEM_INFO1->iSort != 6) && (tITEM_INFO1->iSort != 7) && (tITEM_INFO1->iSort != 8) && (tITEM_INFO1->iSort != 9)
		&& (tITEM_INFO1->iSort != 10) && (tITEM_INFO1->iSort != 11) && (tITEM_INFO1->iSort != 12) && (tITEM_INFO1->iSort
			!= 13) && (tITEM_INFO1->iSort != 14) && (tITEM_INFO1->iSort != 15) && (tITEM_INFO1->iSort != 16) && (
			tITEM_INFO1->iSort != 17) && (tITEM_INFO1->iSort != 18) && (tITEM_INFO1->iSort != 19) && (tITEM_INFO1->iSort
			!= 20) && (tITEM_INFO1->iSort != 21))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	tITEM_INFO2 = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0]);
	if ((tITEM_INFO2->iIndex != 576) && (tITEM_INFO2->iIndex != 699) && (tITEM_INFO2->iIndex != 824))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	switch (mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0])
	{
	case 576:
		iValue = 1;
		tCPCost = 50;
		break;
	case 699:
		if ((rand() % 100) < 20) iValue = 5;
		else if ((rand() % 100) < 40) iValue = 4;
		else if ((rand() % 100) < 50) iValue = 3;
		else if ((rand() % 100) < 60) iValue = 2;

		else if ((rand() % 100) < 42) iValue = 4;
		else if ((rand() % 100) < 52) iValue = 3;
		else if ((rand() % 100) < 62) iValue = 2;

		else if ((rand() % 100) < 44) iValue = 4;
		else if ((rand() % 100) < 54) iValue = 3;
		else if ((rand() % 100) < 64) iValue = 2;

		else if ((rand() % 100) < 46) iValue = 4;
		else if ((rand() % 100) < 56) iValue = 3;
		else if ((rand() % 100) < 66) iValue = 2;

		else if ((rand() % 100) < 48) iValue = 4;
		else if ((rand() % 100) < 58) iValue = 3;
		else if ((rand() % 100) < 68) iValue = 2;
		else iValue = 1;
		tCPCost = 50;
		break;
	case 824:
		iValue = 1;
		tCPCost = 50;
		break;
	}
	if (mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe < tCPCost)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	if (mUTIL.ReturnRFValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]) < 10) tProbability1 = 50;
	else if (mUTIL.ReturnRFValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]) < 15) tProbability1 = 30;
	else if (mUTIL.ReturnRFValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]) < 20) tProbability1 = 20;
	else tProbability1 = 10;

	if (tITEM_INFO2->iIndex == 824) tProbability1 = 100;

	if (mUTIL.ReturnRFValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]) == 0) tProbability1 = 100;


	tRandom = rand();
	if ((tRandom % 100) < tProbability1)
	{
		mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe -= tCPCost;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4] = mUTIL.ChangeRefineValue(
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4], iValue);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][5] = 0;
		mTRANSFER.B_REFINE_ITEM_RECV(0, tCPCost, iValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);

		if (mUTIL.ReturnRFValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]) > 20)
		{
			int tRefineStage = mUTIL.ReturnRFValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]);
			CopyMemory(&tBroadcastInfoData[0], &mUSER[tUserIndex].mAvatarInfo.aTribe, 4);
			CopyMemory(&tBroadcastInfoData[4], &tRefineStage, 4);
			CopyMemory(&tBroadcastInfoData[8], &mUSER[tUserIndex].mAvatarInfo.aName[0], MAX_AVATAR_NAME_LENGTH);
			mCENTER_COM.U_DEMAND_BROADCAST_INFO(673, &tBroadcastInfoData[0]);
		}
		return;
	}
	if ((tRandom % 100) < 40)
	{
		if (mUTIL.ReturnRFValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]) > 7)
		{
			if (mUSER[tUserIndex].mAvatarInfo.aPreventRefineBreak > 0)
			{
				mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe -= tCPCost;
				mUSER[tUserIndex].mAvatarInfo.aPreventRefineBreak--;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4] = mUTIL.ChangeRefineValue(
					mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4], -1);
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][1] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][2] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][3] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][4] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][5] = 0;
				mTRANSFER.B_REFINE_ITEM_RECV(1, tCPCost, -1);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				mTRANSFER.B_AVATAR_CHANGE_INFO_2(63, mUSER[tUserIndex].mAvatarInfo.aPreventRefineBreak);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				return;
			}
			mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe -= tCPCost;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][1] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][2] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][3] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][4] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][5] = 0;
			mTRANSFER.B_REFINE_ITEM_RECV(2, tCPCost, 0);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
	}
	mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe -= tCPCost;
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4] = mUTIL.ChangeRefineValue(
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4], -1);
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] = 0;
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][1] = 0;
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][2] = 0;
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][3] = 0;
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][4] = 0;
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][5] = 0;
	mTRANSFER.B_REFINE_ITEM_RECV(1, tCPCost, -1);
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_HIGH_ITEM_SEND
void W_HIGH_ITEM_SEND(int tUserIndex)
{
	unsigned int tRandom = 0;
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int tPage1;
	int tIndex1;
	int tPage2;
	int tIndex2;
	CopyMemory(&tPage1, &tPacket[1], 4);
	CopyMemory(&tIndex1, &tPacket[5], 4);
	CopyMemory(&tPage2, &tPacket[9], 4);
	CopyMemory(&tIndex2, &tPacket[13], 4);
	if ((tPage1 < 0) || (tPage1 > 1) || (tIndex1 < 0) || (tIndex1 > (MAX_INVENTORY_SLOT_NUM - 1)) || (tPage2 < 0) || (
		tPage2 > 1) || (tIndex2 < 0) || (tIndex2 > (MAX_INVENTORY_SLOT_NUM - 1)))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	ITEM_INFO* tITEM_INFO1;
	ITEM_INFO* tITEM_INFO2;
	ITEM_INFO* tITEM_INFO3;
	int tCost;
	int tItemLevel;
	int tProbability;
	int tValue[6];

	if ((tPage1 == 1) || (tPage2 == 1))
	{
		if (mUSER[tUserIndex].mAvatarInfo.aExpandInventoryDate < mUTIL.ReturnNowDate())
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
	}
	tITEM_INFO1 = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0]);
	if (tITEM_INFO1 == nullptr)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if ((tITEM_INFO1->iType != 3) && (tITEM_INFO1->iType != 4))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if ((tITEM_INFO1->iSort != 7) && (tITEM_INFO1->iSort != 8) && (tITEM_INFO1->iSort != 9) && (tITEM_INFO1->iSort !=
		10) && (tITEM_INFO1->iSort != 11) && (tITEM_INFO1->iSort != 12) && (tITEM_INFO1->iSort != 13) && (tITEM_INFO1->
		iSort != 14) && (tITEM_INFO1->iSort != 15) && (tITEM_INFO1->iSort != 16) && (tITEM_INFO1->iSort != 17) && (
		tITEM_INFO1->iSort != 18) && (tITEM_INFO1->iSort != 19) && (tITEM_INFO1->iSort != 20) && (tITEM_INFO1->iSort !=
		21))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (tITEM_INFO1->iCheckHighItem != 2)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if ((mUTIL.ReturnISValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]) < 4) || (mUTIL.ReturnIUValue(
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]) < 1))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
#ifdef __GOD__
	if (tITEM_INFO1->iMartialLevel >= 12)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
#else
	switch (tITEM_INFO1->iType)
	{
	case 3:
		if (tITEM_INFO1->iLevel >= 145)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		break;
	case 4:
		if (tITEM_INFO1->iLevel >= 145)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		break;
	}
#endif
	tITEM_INFO2 = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0]);
	if (tITEM_INFO2 == nullptr)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if ((tITEM_INFO2->iIndex != 1024) && (tITEM_INFO2->iIndex != 1025))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	switch (tITEM_INFO1->iType)
	{
	case 3:
		switch (tITEM_INFO1->iLevel)
		{
		case 45:
			tCost = 100000;
			tItemLevel = 55;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 520;
			}
			else
			{
				tProbability = 570;
			}
			break;
		case 55:
			tCost = 200000;
			tItemLevel = 65;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 490;
			}
			else
			{
				tProbability = 540;
			}
			break;
		case 65:
			tCost = 300000;
			tItemLevel = 75;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 460;
			}
			else
			{
				tProbability = 510;
			}
			break;
		case 75:
			tCost = 400000;
			tItemLevel = 85;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 430;
			}
			else
			{
				tProbability = 480;
			}
			break;
		case 85:
			tCost = 500000;
			tItemLevel = 95;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 400;
			}
			else
			{
				tProbability = 450;
			}
			break;
		case 95:
			tCost = 600000;
			tItemLevel = 105;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 370;
			}
			else
			{
				tProbability = 420;
			}
			break;
		case 105:
			tCost = 700000;
			tItemLevel = 114;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 340;
			}
			else
			{
				tProbability = 390;
			}
			break;
		case 114:
			tCost = 800000;
			tItemLevel = 117;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 310;
			}
			else
			{
				tProbability = 360;
			}
			break;
		case 117:
			tCost = 900000;
			tItemLevel = 120;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 280;
			}
			else
			{
				tProbability = 330;
			}
			break;
		case 120:
			tCost = 1000000;
			tItemLevel = 123;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 250;
			}
			else
			{
				tProbability = 300;
			}
			break;
		case 123:
			tCost = 1100000;
			tItemLevel = 126;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 220;
			}
			else
			{
				tProbability = 270;
			}
			break;
		case 126:
			tCost = 1200000;
			tItemLevel = 129;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 190;
			}
			else
			{
				tProbability = 240;
			}
			break;
		case 129:
			tCost = 1300000;
			tItemLevel = 132;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 160;
			}
			else
			{
				tProbability = 210;
			}
			break;
		case 132:
			tCost = 1400000;
			tItemLevel = 135;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 130;
			}
			else
			{
				tProbability = 180;
			}
			break;
		case 135:
			tCost = 1500000;
			tItemLevel = 138;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 100;
			}
			else
			{
				tProbability = 150;
			}
			break;
		case 138:
			tCost = 1600000;
			tItemLevel = 141;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 70;
			}
			else
			{
				tProbability = 120;
			}
			break;
		case 141:
			tCost = 1700000;
			tItemLevel = 144;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 40;
			}
			else
			{
				tProbability = 90;
			}
			break;
		case 144:
			tCost = 1800000;
			tItemLevel = 145;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 10;
			}
			else
			{
				tProbability = 60;
			}
			break;
#ifdef __GOD__
		case 145:
			switch (tITEM_INFO1->iMartialLevel)
			{
			case 0:
				tCost = 1900000;
				tItemLevel = 146;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 10;
				}
				else
				{
					tProbability = 20;
				}
				break;
			case 1:
				tCost = 2000000;
				tItemLevel = 147;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 10;
				}
				else
				{
					tProbability = 20;
				}
				break;
			case 2:
				tCost = 2100000;
				tItemLevel = 148;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 10;
				}
				else
				{
					tProbability = 20;
				}
				break;
			case 3:
				tCost = 2200000;
				tItemLevel = 149;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 10;
				}
				else
				{
					tProbability = 20;
				}
				break;
			case 4:
				tCost = 2300000;
				tItemLevel = 150;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 10;
				}
				else
				{
					tProbability = 20;
				}
				break;
			case 5:
				tCost = 2400000;
				tItemLevel = 151;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 10;
				}
				else
				{
					tProbability = 20;
				}
				break;
			case 6:
				tCost = 2500000;
				tItemLevel = 152;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 10;
				}
				else
				{
					tProbability = 20;
				}
				break;
			case 7:
				tCost = 2600000;
				tItemLevel = 153;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 10;
				}
				else
				{
					tProbability = 20;
				}
				break;
			case 8:
				tCost = 2700000;
				tItemLevel = 154;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 10;
				}
				else
				{
					tProbability = 20;
				}
				break;
			case 9:
				tCost = 2800000;
				tItemLevel = 155;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 10;
				}
				else
				{
					tProbability = 20;
				}
				break;
			case 10:
				tCost = 2900000;
				tItemLevel = 156;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 10;
				}
				else
				{
					tProbability = 20;
				}
				break;
			case 11:
				tCost = 3000000;
				tItemLevel = 157;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 10;
				}
				else
				{
					tProbability = 20;
				}
				break;
			}
			break;
#endif
		}
		break;
	case 4:
		switch (tITEM_INFO1->iLevel)
		{
		case 100:
			tCost = 1000000;
			tItemLevel = 110;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 340;
			}
			else
			{
				tProbability = 390;
			}
			break;
		case 110:
			tCost = 2000000;
			tItemLevel = 115;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 310;
			}
			else
			{
				tProbability = 360;
			}
			break;
		case 115:
			tCost = 3000000;
			tItemLevel = 118;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 280;
			}
			else
			{
				tProbability = 330;
			}
			break;
		case 118:
			tCost = 4000000;
			tItemLevel = 121;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 250;
			}
			else
			{
				tProbability = 300;
			}
			break;
		case 121:
			tCost = 5000000;
			tItemLevel = 124;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 220;
			}
			else
			{
				tProbability = 270;
			}
			break;
		case 124:
			tCost = 6000000;
			tItemLevel = 127;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 190;
			}
			else
			{
				tProbability = 240;
			}
			break;
		case 127:
			tCost = 7000000;
			tItemLevel = 130;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 160;
			}
			else
			{
				tProbability = 210;
			}
			break;
		case 130:
			tCost = 8000000;
			tItemLevel = 133;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 130;
			}
			else
			{
				tProbability = 180;
			}
			break;
		case 133:
			tCost = 9000000;
			tItemLevel = 136;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 100;
			}
			else
			{
				tProbability = 150;
			}
			break;
		case 136:
			tCost = 10000000;
			tItemLevel = 139;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 70;
			}
			else
			{
				tProbability = 120;
			}
			break;
		case 139:
			tCost = 11000000;
			tItemLevel = 142;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 40;
			}
			else
			{
				tProbability = 90;
			}
			break;
		case 142:
			tCost = 12000000;
			tItemLevel = 145;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 10;
			}
			else
			{
				tProbability = 60;
			}
			break;
#ifdef __GOD__
		case 145:
			switch (tITEM_INFO1->iMartialLevel)
			{
			case 0:
				tCost = 13000000;
				tItemLevel = 146;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 2;
				}
				else
				{
					tProbability = 4;
				}
				break;
			case 1:
				tCost = 14000000;
				tItemLevel = 147;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 2;
				}
				else
				{
					tProbability = 4;
				}
				break;
			case 2:
				tCost = 15000000;
				tItemLevel = 148;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 2;
				}
				else
				{
					tProbability = 4;
				}
				break;
			case 3:
				tCost = 16000000;
				tItemLevel = 149;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 2;
				}
				else
				{
					tProbability = 4;
				}
				break;
			case 4:
				tCost = 17000000;
				tItemLevel = 150;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 2;
				}
				else
				{
					tProbability = 4;
				}
				break;
			case 5:
				tCost = 18000000;
				tItemLevel = 151;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 2;
				}
				else
				{
					tProbability = 4;
				}
				break;
			case 6:
				tCost = 19000000;
				tItemLevel = 152;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 2;
				}
				else
				{
					tProbability = 4;
				}
				break;
			case 7:
				tCost = 20000000;
				tItemLevel = 153;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 2;
				}
				else
				{
					tProbability = 4;
				}
				break;
			case 8:
				tCost = 21000000;
				tItemLevel = 154;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 2;
				}
				else
				{
					tProbability = 4;
				}
				break;
			case 9:
				tCost = 22000000;
				tItemLevel = 155;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 2;
				}
				else
				{
					tProbability = 4;
				}
				break;
			case 10:
				tCost = 23000000;
				tItemLevel = 156;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 2;
				}
				else
				{
					tProbability = 4;
				}
				break;
			case 11:
				tCost = 24000000;
				tItemLevel = 157;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 2;
				}
				else
				{
					tProbability = 4;
				}
				break;
			}
			break;
#endif
		}
		break;
	}
	if (mUSER[tUserIndex].mAvatarInfo.aMoney < tCost)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
#ifdef __GOD__
	tITEM_INFO3 = mITEM.Return_1(tItemLevel, tITEM_INFO1);
#else
	tITEM_INFO3 = mITEM.Return(tItemLevel, tITEM_INFO1);
#endif
	if (tITEM_INFO3 == nullptr)
	{
		//mGAMELOG.GL_635_HIGH_ITEM( tUserIndex, 4, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5], tCost, tITEM_INFO2->iIndex );
		mTRANSFER.B_HIGH_ITEM_RECV(2, tCost, &tValue[0]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}

	if (mUSER[tUserIndex].mAvatarInfo.aLuckyUpgrade > 0)
	{
		tProbability += 5;
		mUSER[tUserIndex].mAvatarInfo.aLuckyUpgrade--;
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(29, mUSER[tUserIndex].mAvatarInfo.aLuckyUpgrade);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	}

	tProbability += static_cast<int>(static_cast<float>(mAVATAR_OBJECT[tUserIndex].GetLuck()) / 300.0f);
	tRandom = rand();
	if ((tRandom % 1000) < static_cast<unsigned int>(tProbability))
	{
		mUSER[tUserIndex].mAvatarInfo.aMoney -= tCost;
		tValue[0] = tITEM_INFO3->iIndex;
		tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
		tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
		tValue[3] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3];
		tValue[4] = mUTIL.SetISIUValue(
			(mUTIL.ReturnISValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]) - 4),
			(mUTIL.ReturnIUValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]) - 1));
		tValue[5] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5];
		//mGAMELOG.GL_635_HIGH_ITEM( tUserIndex, 1, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5], tCost, tITEM_INFO2->iIndex );
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0] = tValue[0];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1] = tValue[1];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2] = tValue[2];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3] = tValue[3];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4] = tValue[4];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5] = tValue[5];
		//mGAMELOG.GL_635_HIGH_ITEM( tUserIndex, 2, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5], tCost, tITEM_INFO2->iIndex );
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][5] = 0;
		mTRANSFER.B_HIGH_ITEM_RECV(0, tCost, &tValue[0]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	mUSER[tUserIndex].mAvatarInfo.aMoney -= tCost;
	//mGAMELOG.GL_635_HIGH_ITEM( tUserIndex, 3, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5], tCost, tITEM_INFO2->iIndex );
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] = 0;
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][1] = 0;
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][2] = 0;
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][3] = 0;
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][4] = 0;
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][5] = 0;
	mTRANSFER.B_HIGH_ITEM_RECV(1, tCost, &tValue[0]);
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_LOW_ITEM_SEND
void W_LOW_ITEM_SEND(int tUserIndex)
{
	unsigned int tRandom = 0;
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int tPage1;
	int tIndex1;
	int tPage2;
	int tIndex2;
	CopyMemory(&tPage1, &tPacket[1], 4);
	CopyMemory(&tIndex1, &tPacket[5], 4);
	CopyMemory(&tPage2, &tPacket[9], 4);
	CopyMemory(&tIndex2, &tPacket[13], 4);
	if ((tPage1 < 0) || (tPage1 > 1) || (tIndex1 < 0) || (tIndex1 > (MAX_INVENTORY_SLOT_NUM - 1)) || (tPage2 < 0) || (
		tPage2 > 1) || (tIndex2 < 0) || (tIndex2 > (MAX_INVENTORY_SLOT_NUM - 1)))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	ITEM_INFO* tITEM_INFO1;
	ITEM_INFO* tITEM_INFO2;
	ITEM_INFO* tITEM_INFO3;
	int tCost;
	int tItemLevel;
	int tProbability;
	int tValue[6];

	if ((tPage1 == 1) || (tPage2 == 1))
	{
		if (mUSER[tUserIndex].mAvatarInfo.aExpandInventoryDate < mUTIL.ReturnNowDate())
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
	}
	tITEM_INFO1 = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0]);
	if (tITEM_INFO1 == nullptr)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if ((tITEM_INFO1->iType != 3) && (tITEM_INFO1->iType != 4))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if ((tITEM_INFO1->iSort != 7) && (tITEM_INFO1->iSort != 8) && (tITEM_INFO1->iSort != 9) && (tITEM_INFO1->iSort !=
		10) && (tITEM_INFO1->iSort != 11) && (tITEM_INFO1->iSort != 12) && (tITEM_INFO1->iSort != 13) && (tITEM_INFO1->
		iSort != 14) && (tITEM_INFO1->iSort != 15) && (tITEM_INFO1->iSort != 16) && (tITEM_INFO1->iSort != 17) && (
		tITEM_INFO1->iSort != 18) && (tITEM_INFO1->iSort != 19) && (tITEM_INFO1->iSort != 20) && (tITEM_INFO1->iSort !=
		21))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (tITEM_INFO1->iCheckLowItem != 2)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	switch (tITEM_INFO1->iType)
	{
	case 3:
		if (tITEM_INFO1->iLevel <= 45)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		break;
	case 4:
		if (tITEM_INFO1->iLevel <= 100)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		break;
	}
	tITEM_INFO2 = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0]);
	if (tITEM_INFO2 == nullptr)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if ((tITEM_INFO2->iIndex != 1024) && (tITEM_INFO2->iIndex != 1025))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	switch (tITEM_INFO1->iType)
	{
	case 3:
		switch (tITEM_INFO1->iLevel)
		{
		case 55:
			tCost = 100000;
			tItemLevel = 45;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 52;
			}
			else
			{
				tProbability = 57;
			}
			break;
		case 65:
			tCost = 200000;
			tItemLevel = 55;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 49;
			}
			else
			{
				tProbability = 54;
			}
			break;
		case 75:
			tCost = 300000;
			tItemLevel = 65;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 46;
			}
			else
			{
				tProbability = 51;
			}
			break;
		case 85:
			tCost = 400000;
			tItemLevel = 75;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 43;
			}
			else
			{
				tProbability = 48;
			}
			break;
		case 95:
			tCost = 500000;
			tItemLevel = 85;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 40;
			}
			else
			{
				tProbability = 45;
			}
			break;
		case 105:
			tCost = 600000;
			tItemLevel = 95;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 37;
			}
			else
			{
				tProbability = 42;
			}
			break;
		case 114:
			tCost = 700000;
			tItemLevel = 105;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 34;
			}
			else
			{
				tProbability = 39;
			}
			break;
		case 117:
			tCost = 800000;
			tItemLevel = 114;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 31;
			}
			else
			{
				tProbability = 36;
			}
			break;
		case 120:
			tCost = 900000;
			tItemLevel = 117;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 28;
			}
			else
			{
				tProbability = 33;
			}
			break;
		case 123:
			tCost = 1000000;
			tItemLevel = 120;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 25;
			}
			else
			{
				tProbability = 30;
			}
			break;
		case 126:
			tCost = 1100000;
			tItemLevel = 123;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 22;
			}
			else
			{
				tProbability = 27;
			}
			break;
		case 129:
			tCost = 1200000;
			tItemLevel = 126;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 19;
			}
			else
			{
				tProbability = 24;
			}
			break;
		case 132:
			tCost = 1300000;
			tItemLevel = 129;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 16;
			}
			else
			{
				tProbability = 21;
			}
			break;
		case 135:
			tCost = 1400000;
			tItemLevel = 132;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 13;
			}
			else
			{
				tProbability = 18;
			}
			break;
		case 138:
			tCost = 1500000;
			tItemLevel = 135;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 10;
			}
			else
			{
				tProbability = 15;
			}
			break;
		case 141:
			tCost = 1600000;
			tItemLevel = 138;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 7;
			}
			else
			{
				tProbability = 12;
			}
			break;
		case 144:
			tCost = 1700000;
			tItemLevel = 141;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 4;
			}
			else
			{
				tProbability = 9;
			}
			break;
		case 145:
#ifdef __GOD__
			switch (tITEM_INFO1->iMartialLevel)
			{
			case 0:
#endif
				tCost = 1800000;
				tItemLevel = 144;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 1;
				}
				else
				{
					tProbability = 6;
				}
				break;
#ifdef __GOD__
			case 1:
				tCost = 1900000;
				tItemLevel = 145;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 1;
				}
				else
				{
					tProbability = 2;
				}
				break;
			case 2:
				tCost = 2000000;
				tItemLevel = 146;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 1;
				}
				else
				{
					tProbability = 2;
				}
				break;
			case 3:
				tCost = 2100000;
				tItemLevel = 147;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 1;
				}
				else
				{
					tProbability = 2;
				}
				break;
			case 4:
				tCost = 2200000;
				tItemLevel = 148;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 1;
				}
				else
				{
					tProbability = 2;
				}
				break;
			case 5:
				tCost = 2300000;
				tItemLevel = 149;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 1;
				}
				else
				{
					tProbability = 2;
				}
				break;
			case 6:
				tCost = 2400000;
				tItemLevel = 150;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 1;
				}
				else
				{
					tProbability = 2;
				}
				break;
			case 7:
				tCost = 2500000;
				tItemLevel = 151;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 1;
				}
				else
				{
					tProbability = 2;
				}
				break;
			case 8:
				tCost = 2600000;
				tItemLevel = 152;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 1;
				}
				else
				{
					tProbability = 2;
				}
				break;
			case 9:
				tCost = 2700000;
				tItemLevel = 153;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 1;
				}
				else
				{
					tProbability = 2;
				}
				break;
			case 10:
				tCost = 2800000;
				tItemLevel = 154;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 1;
				}
				else
				{
					tProbability = 2;
				}
				break;
			case 11:
				tCost = 2900000;
				tItemLevel = 155;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 1;
				}
				else
				{
					tProbability = 2;
				}
				break;
			case 12:
				tCost = 3000000;
				tItemLevel = 156;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 1;
				}
				else
				{
					tProbability = 2;
				}
				break;
			}
			break;
#endif
		}
		break;
	case 4:
		switch (tITEM_INFO1->iLevel)
		{
		case 110:
			tCost = 1000000;
			tItemLevel = 100;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 34;
			}
			else
			{
				tProbability = 39;
			}
			break;
		case 115:
			tCost = 2000000;
			tItemLevel = 110;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 31;
			}
			else
			{
				tProbability = 36;
			}
			break;
		case 118:
			tCost = 3000000;
			tItemLevel = 115;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 28;
			}
			else
			{
				tProbability = 33;
			}
			break;
		case 121:
			tCost = 4000000;
			tItemLevel = 118;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 25;
			}
			else
			{
				tProbability = 30;
			}
			break;
		case 124:
			tCost = 5000000;
			tItemLevel = 121;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 22;
			}
			else
			{
				tProbability = 27;
			}
			break;
		case 127:
			tCost = 6000000;
			tItemLevel = 124;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 19;
			}
			else
			{
				tProbability = 24;
			}
			break;
		case 130:
			tCost = 7000000;
			tItemLevel = 127;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 16;
			}
			else
			{
				tProbability = 21;
			}
			break;
		case 133:
			tCost = 8000000;
			tItemLevel = 130;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 13;
			}
			else
			{
				tProbability = 18;
			}
			break;
		case 136:
			tCost = 9000000;
			tItemLevel = 133;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 10;
			}
			else
			{
				tProbability = 15;
			}
			break;
		case 139:
			tCost = 10000000;
			tItemLevel = 136;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 7;
			}
			else
			{
				tProbability = 12;
			}
			break;
		case 142:
			tCost = 11000000;
			tItemLevel = 139;
			if (tITEM_INFO2->iIndex == 1024)
			{
				tProbability = 4;
			}
			else
			{
				tProbability = 9;
			}
			break;
		case 145:
#ifdef __GOD__
			switch (tITEM_INFO1->iMartialLevel)
			{
			case 0:
#endif
				tCost = 12000000;
				tItemLevel = 142;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 1;
				}
				else
				{
					tProbability = 6;
				}
				break;
#ifdef __GOD__
			case 1:
				tCost = 13000000;
				tItemLevel = 145;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 1;
				}
				else
				{
					tProbability = 2;
				}
				break;
			case 2:
				tCost = 14000000;
				tItemLevel = 146;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 1;
				}
				else
				{
					tProbability = 2;
				}
				break;
			case 3:
				tCost = 15000000;
				tItemLevel = 147;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 1;
				}
				else
				{
					tProbability = 2;
				}
				break;
			case 4:
				tCost = 16000000;
				tItemLevel = 148;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 1;
				}
				else
				{
					tProbability = 2;
				}
				break;
			case 5:
				tCost = 17000000;
				tItemLevel = 149;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 1;
				}
				else
				{
					tProbability = 2;
				}
				break;
			case 6:
				tCost = 18000000;
				tItemLevel = 150;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 1;
				}
				else
				{
					tProbability = 2;
				}
				break;
			case 7:
				tCost = 19000000;
				tItemLevel = 151;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 1;
				}
				else
				{
					tProbability = 2;
				}
				break;
			case 8:
				tCost = 20000000;
				tItemLevel = 152;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 1;
				}
				else
				{
					tProbability = 2;
				}
				break;
			case 9:
				tCost = 21000000;
				tItemLevel = 153;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 1;
				}
				else
				{
					tProbability = 2;
				}
				break;
			case 10:
				tCost = 22000000;
				tItemLevel = 154;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 1;
				}
				else
				{
					tProbability = 2;
				}
				break;
			case 11:
				tCost = 23000000;
				tItemLevel = 155;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 1;
				}
				else
				{
					tProbability = 2;
				}
				break;
			case 12:
				tCost = 24000000;
				tItemLevel = 156;
				if (tITEM_INFO2->iIndex == 1024)
				{
					tProbability = 1;
				}
				else
				{
					tProbability = 2;
				}
				break;
			}
			break;
#endif
		}
		break;
	}
	if (mUSER[tUserIndex].mAvatarInfo.aMoney < tCost)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
#ifdef __GOD__
	tITEM_INFO3 = mITEM.Return_2(tItemLevel, tITEM_INFO1);
#else
	tITEM_INFO3 = mITEM.Return(tItemLevel, tITEM_INFO1);
#endif
	if (tITEM_INFO3 == nullptr)
	{
		//mGAMELOG.GL_636_LOW_ITEM( tUserIndex, 4, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5], tCost, tITEM_INFO2->iIndex );
		mTRANSFER.B_LOW_ITEM_RECV(2, tCost, &tValue[0]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	tProbability += static_cast<int>(static_cast<float>(mAVATAR_OBJECT[tUserIndex].GetLuck()) / 300.0f);
	tRandom = rand();
	if ((tRandom % 100) < static_cast<unsigned int>(tProbability))
	{
		mUSER[tUserIndex].mAvatarInfo.aMoney -= tCost;
		tValue[0] = tITEM_INFO3->iIndex;
		tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
		tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
		tValue[3] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3];
		tValue[4] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4];
		tValue[5] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5];
		//mGAMELOG.GL_636_LOW_ITEM( tUserIndex, 1, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5], tCost, tITEM_INFO2->iIndex );
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0] = tValue[0];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1] = tValue[1];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2] = tValue[2];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3] = tValue[3];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4] = tValue[4];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5] = tValue[5];
		//mGAMELOG.GL_636_LOW_ITEM( tUserIndex, 2, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5], tCost, tITEM_INFO2->iIndex );
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][5] = 0;
		mTRANSFER.B_LOW_ITEM_RECV(0, tCost, &tValue[0]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	mUSER[tUserIndex].mAvatarInfo.aMoney -= tCost;
	//mGAMELOG.GL_636_LOW_ITEM( tUserIndex, 3, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5], tCost, tITEM_INFO2->iIndex );
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] = 0;
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][1] = 0;
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][2] = 0;
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][3] = 0;
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][4] = 0;
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][5] = 0;
	mTRANSFER.B_LOW_ITEM_RECV(1, tCost, &tValue[0]);
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_TIME_EFFECT_SEND
void W_TIME_EFFECT_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int tValue01;

	CopyMemory(&tValue01, &tPacket[1], 4);
	switch (tValue01)
	{
	case 1:
		mUSER[tUserIndex].mAvatarInfo.aTimeEffect = 240;
		mUSER[tUserIndex].mAvatarInfo.aTimeEffect2 = 1;
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(55, mUSER[tUserIndex].mAvatarInfo.aTimeEffect);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 2:
		mUSER[tUserIndex].mAvatarInfo.aTimeEffect = 360;
		mUSER[tUserIndex].mAvatarInfo.aTimeEffect2 = 2;
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(55, mUSER[tUserIndex].mAvatarInfo.aTimeEffect);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 3:
		mUSER[tUserIndex].mAvatarInfo.aTimeEffect = 480;
		mUSER[tUserIndex].mAvatarInfo.aTimeEffect2 = 3;
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(55, mUSER[tUserIndex].mAvatarInfo.aTimeEffect);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 4:
		mUSER[tUserIndex].mAvatarInfo.aTimeEffect = 600;
		mUSER[tUserIndex].mAvatarInfo.aTimeEffect2 = 4;
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(55, mUSER[tUserIndex].mAvatarInfo.aTimeEffect);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 5:
		mUSER[tUserIndex].mAvatarInfo.aTimeEffect = 720;
		mUSER[tUserIndex].mAvatarInfo.aTimeEffect2 = 5;
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(55, mUSER[tUserIndex].mAvatarInfo.aTimeEffect);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	default:
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
	}
}

//W_DAILY_MISSION_SEND
void W_DAILY_MISSION_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int tValue01;
	int tRandom;
	int tItem;
	int iPage = 0, iInvenIndex = 0, iInvenSlot = 0;

	CopyMemory(&tValue01, &tPacket[1], 4);

	switch (tValue01)
	{
	case 1:
		mTRANSFER.B_DAILY_RECV(tValue01, mUSER[tUserIndex].mAvatarInfo.aDailyWarCount,
		                       mUSER[tUserIndex].mAvatarInfo.aDailyPVPCount,
		                       mUSER[tUserIndex].mAvatarInfo.aDailyPVMKillCount,
		                       mUSER[tUserIndex].mAvatarInfo.aDailyOnlineTime);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 2:
		if ((mUSER[tUserIndex].mAvatarInfo.aDailyWarCount < 3) &&
			(mUSER[tUserIndex].mAvatarInfo.aDailyPVPCount < 10) &&
			(mUSER[tUserIndex].mAvatarInfo.aDailyPVMKillCount < 1000) &&
			(mUSER[tUserIndex].mAvatarInfo.aDailyOnlineTime < 60))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		tRandom = rand() % 6;
		switch (tRandom)
		{
		case 0: tItem = 1017;
			break;
		case 1: tItem = 1018;
			break;
		case 2: tItem = 1092;
			break;
		case 3: tItem = 1093;
			break;
		case 4: tItem = 1041;
			break;
		case 5: tItem = 1438;
			break;
		}
		ITEM_INFO* pItem = mITEM.Search(tItem);
		if (!mUTIL.FindEmptyInvenForItem(tUserIndex, pItem, iPage, iInvenIndex))
		{
			mTRANSFER.B_DAILY_RECV(3, mUSER[tUserIndex].mAvatarInfo.aDailyWarCount,
			                       mUSER[tUserIndex].mAvatarInfo.aDailyPVPCount,
			                       mUSER[tUserIndex].mAvatarInfo.aDailyPVMKillCount,
			                       mUSER[tUserIndex].mAvatarInfo.aDailyOnlineTime);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		if (mUTIL.FindEmptyInvenForItem(tUserIndex, pItem, iPage, iInvenIndex))
		{
			iInvenSlot = mUTIL.GetEmptyInvenSlotIndex(tUserIndex, iPage);
			if (iInvenSlot != -1)
			{
				mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iInvenSlot][0] = pItem->iIndex;
				mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iInvenSlot][1] = iInvenIndex % 8;
				mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iInvenSlot][2] = iInvenIndex / 8;
				mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iInvenSlot][3] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iInvenSlot][4] = 0;
				mUSER[tUserIndex].mAvatarInfo.aInventory[iPage][iInvenSlot][5] = 0;
			}
		}
		mTRANSFER.B_DAILY_GET_REWARD(7001, iPage, iInvenSlot, iInvenIndex, pItem->iIndex);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mUSER[tUserIndex].mAvatarInfo.aDailyWarCount = 0;
		mUSER[tUserIndex].mAvatarInfo.aDailyPVPCount = 0;
		mUSER[tUserIndex].mAvatarInfo.aDailyPVMKillCount = 0;
		mUSER[tUserIndex].mAvatarInfo.aDailyOnlineTime = 0;
		mTRANSFER.B_DAILY_RECV(tValue01, mUSER[tUserIndex].mAvatarInfo.aDailyWarCount,
		                       mUSER[tUserIndex].mAvatarInfo.aDailyPVPCount,
		                       mUSER[tUserIndex].mAvatarInfo.aDailyPVMKillCount,
		                       mUSER[tUserIndex].mAvatarInfo.aDailyOnlineTime);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	}
}

//W_EXCHANGE_ITEM_SEND
void W_EXCHANGE_ITEM_SEND(int tUserIndex)
{
	unsigned int tRandom = 0;
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int tPage;
	int tIndex;
	CopyMemory(&tPage, &tPacket[1], 4);
	CopyMemory(&tIndex, &tPacket[5], 4);
	if ((tPage < 0) || (tPage > 1) || (tIndex < 0) || (tIndex > (MAX_INVENTORY_SLOT_NUM - 1)))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	ITEM_INFO* tITEM_INFO1;
	ITEM_INFO* tITEM_INFO2;
	int tCost;
	int tTribeItemList[9];
	int tValue[6];
	int index01;
	int index02;

	if (tPage == 1)
	{
		if (mUSER[tUserIndex].mAvatarInfo.aExpandInventoryDate < mUTIL.ReturnNowDate())
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
	}
	tITEM_INFO1 = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0]);
	if (tITEM_INFO1 == nullptr)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if ((tITEM_INFO1->iType != 3) && (tITEM_INFO1->iType != 4))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if ((tITEM_INFO1->iSort != 7) && (tITEM_INFO1->iSort != 8) && (tITEM_INFO1->iSort != 9) && (tITEM_INFO1->iSort !=
		10) && (tITEM_INFO1->iSort != 11) && (tITEM_INFO1->iSort != 12) && (tITEM_INFO1->iSort != 13) && (tITEM_INFO1->
		iSort != 14) && (tITEM_INFO1->iSort != 15) && (tITEM_INFO1->iSort != 16) && (tITEM_INFO1->iSort != 17) && (
		tITEM_INFO1->iSort != 18) && (tITEM_INFO1->iSort != 19) && (tITEM_INFO1->iSort != 20) && (tITEM_INFO1->iSort !=
		21))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (tITEM_INFO1->iCheckExchange != 2)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	switch (tITEM_INFO1->iLevel)
	{
	case 45:
		tCost = 1000000;
		break;
	case 55:
		tCost = 1200000;
		break;
	case 65:
		tCost = 1400000;
		break;
	case 75:
		tCost = 1600000;
		break;
	case 85:
		tCost = 1800000;
		break;
	case 95:
	case 100:
		tCost = 2000000;
		break;
	case 105:
	case 110:
		tCost = 2200000;
		break;
	case 114:
		tCost = 2400000;
		break;
	case 117:
	case 115:
		tCost = 2600000;
		break;
	case 120:
	case 118:
		tCost = 2800000;
		break;
	case 123:
	case 121:
		tCost = 3000000;
		break;
	case 126:
	case 124:
		tCost = 3200000;
		break;
	case 129:
	case 127:
		tCost = 3400000;
		break;
	case 132:
	case 130:
		tCost = 3600000;
		break;
	case 135:
	case 133:
		tCost = 3800000;
		break;
	case 138:
	case 136:
		tCost = 4000000;
		break;
	case 141:
	case 139:
		tCost = 4200000;
		break;
	case 144:
	case 142:
		tCost = 4400000;
		break;
	case 145:
#ifdef __GOD__
		switch (tITEM_INFO1->iMartialLevel)
		{
		case 0:
#endif
			tCost = 4600000;
			break;
#ifdef __GOD__
		case 1:
			tCost = 4800000;
			break;
		case 2:
			tCost = 5000000;
			break;
		case 3:
			tCost = 5200000;
			break;
		case 4:
			tCost = 5400000;
			break;
		case 5:
			tCost = 5600000;
			break;
		case 6:
			tCost = 5800000;
			break;
		case 7:
			tCost = 6000000;
			break;
		case 8:
			tCost = 6200000;
			break;
		case 9:
			tCost = 6400000;
			break;
		case 10:
			tCost = 6600000;
			break;
		case 11:
			tCost = 6800000;
			break;
		case 12:
			tCost = 7000000;
			break;
		}
		break;
#endif
	}
	if (tITEM_INFO1->iType == 4)
	{
		tCost *= 2;
	}
	if (mUSER[tUserIndex].mAvatarInfo.aMoney < tCost)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	switch (mUSER[tUserIndex].mAvatarInfo.aPreviousTribe)
	{
	case 0:
		tTribeItemList[0] = 7;
		tTribeItemList[1] = 8;
		tTribeItemList[2] = 9;
		tTribeItemList[3] = 10;
		tTribeItemList[4] = 11;
		tTribeItemList[5] = 12;
		tTribeItemList[6] = 13;
		tTribeItemList[7] = 14;
		tTribeItemList[8] = 15;
		break;
	case 1:
		tTribeItemList[0] = 7;
		tTribeItemList[1] = 8;
		tTribeItemList[2] = 9;
		tTribeItemList[3] = 10;
		tTribeItemList[4] = 11;
		tTribeItemList[5] = 12;
		tTribeItemList[6] = 16;
		tTribeItemList[7] = 17;
		tTribeItemList[8] = 18;
		break;
	case 2:
		tTribeItemList[0] = 7;
		tTribeItemList[1] = 8;
		tTribeItemList[2] = 9;
		tTribeItemList[3] = 10;
		tTribeItemList[4] = 11;
		tTribeItemList[5] = 12;
		tTribeItemList[6] = 19;
		tTribeItemList[7] = 20;
		tTribeItemList[8] = 21;
		break;
	}
	tRandom = rand();

	tITEM_INFO2 = mITEM.Return((tITEM_INFO1->iLevel + tITEM_INFO1->iMartialLevel), tITEM_INFO1->iType,
	                           tTribeItemList[(rand() % 9)], tITEM_INFO1);

	if ((tITEM_INFO2 == nullptr) || ((tITEM_INFO2->iIndex >= 87000 && tITEM_INFO2->iIndex <= 88048)) || ((tITEM_INFO2->
		iIndex >= 86700 && tITEM_INFO2->iIndex <= 86782)))
	{
		mTRANSFER.B_EXCHANGE_ITEM_RECV(1, tCost, &tValue[0]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	// Prevent Giving Warlord Gear
	/*if ((tITEM_INFO2 == NULL) || ( (tITEM_INFO2->iIndex >= 87000) && (tITEM_INFO2->iIndex <= 88048) ) )
	{
		mTRANSFER.B_EXCHANGE_ITEM_RECV(1, tCost, &tValue[0]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}*/
	if ((tITEM_INFO2 == nullptr) || (tITEM_INFO2->iIndex == tITEM_INFO1->iIndex) || (tITEM_INFO2->iIndex == 13553) || (
		tITEM_INFO2->iIndex == 13554) || (tITEM_INFO2->iIndex == 33553) || (tITEM_INFO2->iIndex == 33554) || (
		tITEM_INFO2->iIndex == 53553) || (tITEM_INFO2->iIndex == 53554))
	{
		//mGAMELOG.GL_637_EXCHANGE_ITEM( tUserIndex, 3, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], tCost );
		mTRANSFER.B_EXCHANGE_ITEM_RECV(1, tCost, &tValue[0]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}

	tCost = 0;

	mUSER[tUserIndex].mAvatarInfo.aMoney -= tCost;
	tValue[0] = tITEM_INFO2->iIndex;
	tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1];
	tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2];
	tValue[3] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3];
	tValue[4] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4];
	tValue[5] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5];
	//mGAMELOG.GL_637_EXCHANGE_ITEM( tUserIndex, 1, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], tCost );
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0] = tValue[0];
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][1] = tValue[1];
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][2] = tValue[2];
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3] = tValue[3];
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4] = tValue[4];
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5] = tValue[5];
	//mGAMELOG.GL_637_EXCHANGE_ITEM( tUserIndex, 2, mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][0], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][3], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][4], mUSER[tUserIndex].mAvatarInfo.aInventory[tPage][tIndex][5], tCost );
	mTRANSFER.B_EXCHANGE_ITEM_RECV(0, tCost, &tValue[0]);
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_WARLORD_V2_SEND
void W_WARLORD_V2_SEND(int tUserIndex)
{
	unsigned int tRandom = 0;
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int tPage1;
	int tIndex1;
	int tPage2;
	int tIndex2;
	CopyMemory(&tPage1, &tPacket[1], 4);
	CopyMemory(&tIndex1, &tPacket[5], 4);
	CopyMemory(&tPage2, &tPacket[9], 4);
	CopyMemory(&tIndex2, &tPacket[13], 4);
	if ((tPage1 < 0) || (tPage1 > 1) || (tIndex1 < 0) || (tIndex1 > (MAX_INVENTORY_SLOT_NUM - 1)) || (tPage2 < 0) || (
		tPage2 > 1) || (tIndex2 < 0) || (tIndex2 > (MAX_INVENTORY_SLOT_NUM - 1)))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	ITEM_INFO* tITEM_INFO1;
	ITEM_INFO* tITEM_INFO2;
	int tCost;
	int tV2;
	int tEnchantLoss;
	int tItemLevel;
	int tProbability;
	int tValue[6];

	if ((tPage1 == 1) || (tPage2 == 1))
	{
		if (mUSER[tUserIndex].mAvatarInfo.aExpandInventoryDate < mUTIL.ReturnNowDate())
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
	}
	tITEM_INFO1 = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0]);
	if (tITEM_INFO1 == nullptr)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if ((tITEM_INFO1->iType != 3) && (tITEM_INFO1->iType != 4))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if ((tITEM_INFO1->iSort != 7) && (tITEM_INFO1->iSort != 8) && (tITEM_INFO1->iSort != 9) && (tITEM_INFO1->iSort !=
		10) && (tITEM_INFO1->iSort != 11) && (tITEM_INFO1->iSort != 12) && (tITEM_INFO1->iSort != 13) && (tITEM_INFO1->
		iSort != 14) && (tITEM_INFO1->iSort != 15) && (tITEM_INFO1->iSort != 16) && (tITEM_INFO1->iSort != 17) && (
		tITEM_INFO1->iSort != 18) && (tITEM_INFO1->iSort != 19) && (tITEM_INFO1->iSort != 20) && (tITEM_INFO1->iSort !=
		21))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	tITEM_INFO2 = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0]);
	if (tITEM_INFO2 == nullptr)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if ((tITEM_INFO2->iIndex != 501) && (tITEM_INFO2->iIndex != 502) && (tITEM_INFO2->iIndex != 503) && (tITEM_INFO2->
		iIndex != 504))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	tV2 = tITEM_INFO1->iIndex + 129;

	switch (mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0])
	{
	case 501:
		tCost = 30000000;
		tProbability = 40;
		tEnchantLoss = -8;
		break;
	case 502:
		tCost = 30000000;
		tProbability = 30;
		tEnchantLoss = -6;
		break;
	case 503:
		tCost = 30000000;
		tProbability = 20;
		tEnchantLoss = -4;
		break;
	case 504:
		tCost = 30000000;
		tProbability = 10;
		tEnchantLoss = 0;
		break;
	}
	if (mUSER[tUserIndex].mAvatarInfo.aMoney < tCost)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUTIL.ReturnISValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4]) < 30)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	tRandom = rand();
	if ((tRandom % 100) < tProbability)
	{
		mUSER[tUserIndex].mAvatarInfo.aMoney -= tCost;
		tValue[0] = tV2;
		tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
		tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
		tValue[3] = 0;
		tValue[4] = mUTIL.ChangeISValue(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4], tEnchantLoss);
		tValue[5] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0] = tValue[0];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1] = tValue[1];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2] = tValue[2];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3] = tValue[3];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4] = tValue[4];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5] = tValue[5];
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][5] = 0;
		mTRANSFER.B_WARLORD_V2_RECV(0, tCost, &tValue[0]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	mUSER[tUserIndex].mAvatarInfo.aMoney -= tCost;
	tValue[0] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0];
	tValue[1] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1];
	tValue[2] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2];
	tValue[3] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3];
	tValue[4] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4];
	tValue[5] = mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5];
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0] = tValue[0];
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][1] = tValue[1];
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][2] = tValue[2];
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][3] = tValue[3];
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][4] = tValue[4];
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][5] = tValue[5];
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][0] = 0;
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][1] = 0;
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][2] = 0;
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][3] = 0;
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][4] = 0;
	mUSER[tUserIndex].mAvatarInfo.aInventory[tPage2][tIndex2][5] = 0;
	mTRANSFER.B_WARLORD_V2_RECV(1, tCost, &tValue[0]);
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_GENERAL_SHOUT_SEND
void W_GENERAL_SHOUT_SEND(int tUserIndex)
{
	//unsigned int tRandom = 0;
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	char tContent[MAX_CHAT_CONTENT_LENGTH];
	BYTE tAuthType = 0;
	CopyMemory(&tContent[0], &tPacket[1], MAX_CHAT_CONTENT_LENGTH);
	tContent[(MAX_CHAT_CONTENT_LENGTH - 1)] = '\0';
	if (strcmp(tContent, "") == 0)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	if (mUSER[tUserIndex].mAvatarInfo.aSpecialState == 2)
	{
		return;
	}
	if ((mSERVER_INFO.mServerNumber != 37) && (mSERVER_INFO.mServerNumber != 119) && (mSERVER_INFO.mServerNumber != 124)
		&&
		(mSERVER_INFO.mServerNumber != 84))
	{
		return;
	}
	if (1 == mUSER[tUserIndex].mAuthInfo.AuthType)
		tAuthType = 1;

	mTRANSFER.B_GENERAL_SHOUT_RECV(&mUSER[tUserIndex].mAvatarInfo.aName[0], tAuthType, &tContent[0]);
	mUTIL.Broadcast(FALSE, nullptr, 0, false);
}

//W_SOCKET_ITEM_SEND
void W_SOCKET_ITEM_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	int tSort;
	int tPage1;
	int tIndex1;
	int tRandomSocket[5];
	int tRandom;
	int tCost;
	int tGemEffect[10];
	ITEM_INFO* tITEM_INFO1;
	ITEM_INFO* tITEM_INFO2;
	CopyMemory(&tSort, &tPacket[1], 4);
	CopyMemory(&tPage1, &tPacket[5], 4);
	CopyMemory(&tIndex1, &tPacket[9], 4);

	switch (tSort)
	{
	case 1: //[Socket]
		tITEM_INFO1 = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1][tIndex1][0]);
		if (tITEM_INFO1 == nullptr)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1][tIndex1] > 0)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aMoney < 100000000)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		tRandomSocket[0] = 100000;
		tRandomSocket[1] = 200000;
		tRandomSocket[2] = 300000;
		tRandomSocket[3] = 400000;
		tRandomSocket[4] = 500000;
		tCost = 100000000;

		tRandom = rand() % 5;
		mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1][tIndex1] = tRandomSocket[tRandom];
		mUSER[tUserIndex].mAvatarInfo.aMoney -= tCost;
		mTRANSFER.B_SOCKET_ITEM_RECV(0, ((tRandomSocket[tRandom] / 1000) + 1),
		                             mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1][tIndex1]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 2: //[Socket Remove]
		tITEM_INFO1 = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 % 100][tIndex1 % 100][0]);
		if (tITEM_INFO1 == nullptr)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		tITEM_INFO2 = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aInventory[(tPage1 / 100)][(tIndex1 / 100)][0]);
		if (tITEM_INFO2->iIndex != 1212)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1 % 100][tIndex1 % 100] == 0)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aInventorySocket[(tPage1 % 100)][(tIndex1 % 100)] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[(tPage1 / 100)][(tIndex1 / 100)][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[(tPage1 / 100)][(tIndex1 / 100)][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[(tPage1 / 100)][(tIndex1 / 100)][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[(tPage1 / 100)][(tIndex1 / 100)][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[(tPage1 / 100)][(tIndex1 / 100)][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[(tPage1 / 100)][(tIndex1 / 100)][5] = 0;
		mTRANSFER.B_SOCKET_ITEM_RECV(0, 2, 0);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 3: //[Gem Insert]
		tITEM_INFO1 = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 % 100][tIndex1 % 100][0]);
		if (tITEM_INFO1 == nullptr)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		tITEM_INFO2 = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][0]);
		if ((tITEM_INFO2->iIndex != 531) && (tITEM_INFO2->iIndex != 1244))
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1 % 100][tIndex1 % 100] < 100000)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if ((mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1 % 100][tIndex1 % 100] % 10) < 1)
		{
			tGemEffect[0] = 1;
			tGemEffect[1] = 2;
			tGemEffect[2] = 3;
			tGemEffect[3] = 4;
			tGemEffect[4] = 5;
			tGemEffect[5] = 6;
			tGemEffect[6] = 7;
			tGemEffect[7] = 8;
			tGemEffect[8] = 9;
			tRandom = rand() % 9;

			mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1 % 100][tIndex1 % 100] += tGemEffect[tRandom];
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][0] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][1] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][2] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][3] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][4] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][5] = 0;
			mTRANSFER.B_SOCKET_ITEM_RECV(
				0, 3, mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1 % 100][tIndex1 % 100]);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		if (((mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1 % 100][tIndex1 % 100] % 100) / 10) < 1)
		{
			tGemEffect[0] = 10;
			tGemEffect[1] = 20;
			tGemEffect[2] = 30;
			tGemEffect[3] = 40;
			tGemEffect[4] = 50;
			tGemEffect[5] = 60;
			tGemEffect[6] = 70;
			tGemEffect[7] = 80;
			tGemEffect[8] = 90;
			tRandom = rand() % 9;

			mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1 % 100][tIndex1 % 100] += tGemEffect[tRandom];
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][0] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][1] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][2] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][3] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][4] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][5] = 0;
			mTRANSFER.B_SOCKET_ITEM_RECV(
				0, 3, mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1 % 100][tIndex1 % 100]);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		if (((mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1 % 100][tIndex1 % 100] % 1000) / 100) < 1)
		{
			tGemEffect[0] = 100;
			tGemEffect[1] = 200;
			tGemEffect[2] = 300;
			tGemEffect[3] = 400;
			tGemEffect[4] = 500;
			tGemEffect[5] = 600;
			tGemEffect[6] = 700;
			tGemEffect[7] = 800;
			tGemEffect[8] = 900;
			tRandom = rand() % 9;

			mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1 % 100][tIndex1 % 100] += tGemEffect[tRandom];
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][0] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][1] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][2] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][3] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][4] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][5] = 0;
			mTRANSFER.B_SOCKET_ITEM_RECV(
				0, 3, mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1 % 100][tIndex1 % 100]);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		if (((mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1 % 100][tIndex1 % 100] % 10000) / 1000) < 1)
		{
			tGemEffect[0] = 1000;
			tGemEffect[1] = 2000;
			tGemEffect[2] = 3000;
			tGemEffect[3] = 4000;
			tGemEffect[4] = 5000;
			tGemEffect[5] = 6000;
			tGemEffect[6] = 7000;
			tGemEffect[7] = 8000;
			tGemEffect[8] = 9000;
			tRandom = rand() % 9;

			mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1 % 100][tIndex1 % 100] += tGemEffect[tRandom];
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][0] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][1] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][2] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][3] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][4] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][5] = 0;
			mTRANSFER.B_SOCKET_ITEM_RECV(
				0, 3, mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1 % 100][tIndex1 % 100]);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		if (((mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1 % 100][tIndex1 % 100] % 100000) / 10000) < 1)
		{
			tGemEffect[0] = 10000;
			tGemEffect[1] = 20000;
			tGemEffect[2] = 30000;
			tGemEffect[3] = 40000;
			tGemEffect[4] = 50000;
			tGemEffect[5] = 60000;
			tGemEffect[6] = 70000;
			tGemEffect[7] = 80000;
			tGemEffect[8] = 90000;
			tRandom = rand() % 9;

			mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1 % 100][tIndex1 % 100] += tGemEffect[tRandom];
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][0] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][1] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][2] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][3] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][4] = 0;
			mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][5] = 0;
			mTRANSFER.B_SOCKET_ITEM_RECV(
				0, 3, mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1 % 100][tIndex1 % 100]);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}

	case 104: //[Remove Gem 1]
		tITEM_INFO1 = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 % 100][tIndex1 % 100][0]);
		if (tITEM_INFO1 == nullptr)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		tITEM_INFO2 = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][0]);
		if (tITEM_INFO2->iIndex != 1213)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if ((mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1 % 100][tIndex1 % 100] % 10) < 1)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1 % 100][tIndex1 % 100] -= ((mUSER[tUserIndex].mAvatarInfo.
			aInventorySocket[tPage1 % 100][tIndex1 % 100] % 10) * 1);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][5] = 0;
		mTRANSFER.B_SOCKET_ITEM_RECV(0, 4, mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1 % 100][tIndex1 % 100]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 204: //[Remove Gem 2]
		tITEM_INFO1 = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 % 100][tIndex1 % 100][0]);
		if (tITEM_INFO1 == nullptr)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		tITEM_INFO2 = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][0]);
		if (tITEM_INFO2->iIndex != 1213)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (((mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1 % 100][tIndex1 % 100] % 100) / 10) < 1)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1 % 100][tIndex1 % 100] -= (((mUSER[tUserIndex].mAvatarInfo.
			aInventorySocket[tPage1 % 100][tIndex1 % 100] % 100) / 10) * 10);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][5] = 0;
		mTRANSFER.B_SOCKET_ITEM_RECV(0, 4, mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1 % 100][tIndex1 % 100]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 304: //[Remove Gem 3]
		tITEM_INFO1 = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 % 100][tIndex1 % 100][0]);
		if (tITEM_INFO1 == nullptr)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		tITEM_INFO2 = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][0]);
		if (tITEM_INFO2->iIndex != 1213)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (((mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1 % 100][tIndex1 % 100] % 1000) / 100) < 1)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1 % 100][tIndex1 % 100] -= (((mUSER[tUserIndex].mAvatarInfo.
			aInventorySocket[tPage1 % 100][tIndex1 % 100] % 1000) / 100) * 100);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][5] = 0;
		mTRANSFER.B_SOCKET_ITEM_RECV(0, 4, mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1 % 100][tIndex1 % 100]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 404: //[Remove Gem 4]
		tITEM_INFO1 = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 % 100][tIndex1 % 100][0]);
		if (tITEM_INFO1 == nullptr)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		tITEM_INFO2 = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][0]);
		if (tITEM_INFO2->iIndex != 1213)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (((mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1 % 100][tIndex1 % 100] % 10000) / 1000) < 1)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1 % 100][tIndex1 % 100] -= (((mUSER[tUserIndex].mAvatarInfo.
			aInventorySocket[tPage1 % 100][tIndex1 % 100] % 10000) / 1000) * 1000);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][5] = 0;
		mTRANSFER.B_SOCKET_ITEM_RECV(0, 4, mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1 % 100][tIndex1 % 100]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	case 504: //[Remove Gem 5]
		tITEM_INFO1 = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 % 100][tIndex1 % 100][0]);
		if (tITEM_INFO1 == nullptr)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		tITEM_INFO2 = mITEM.Search(mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][0]);
		if (tITEM_INFO2->iIndex != 1213)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		if (((mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1 % 100][tIndex1 % 100] % 100000) / 10000) < 1)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1 % 100][tIndex1 % 100] -= (((mUSER[tUserIndex].mAvatarInfo.
			aInventorySocket[tPage1 % 100][tIndex1 % 100] % 100000) / 10000) * 10000);
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aInventory[tPage1 / 100][tIndex1 / 100][5] = 0;
		mTRANSFER.B_SOCKET_ITEM_RECV(0, 4, mUSER[tUserIndex].mAvatarInfo.aInventorySocket[tPage1 % 100][tIndex1 % 100]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;

	default:
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
	}
}

//W_TEACHER_ASK_SEND
void W_TEACHER_ASK_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	char tAvatarName[MAX_AVATAR_NAME_LENGTH];
	CopyMemory(&tAvatarName[0], &tPacket[1], MAX_AVATAR_NAME_LENGTH);
	tAvatarName[(MAX_AVATAR_NAME_LENGTH - 1)] = '\0';

	int tOtherAvatarIndex;

	if ((mUSER[tUserIndex].mAvatarInfo.aLevel1 < 113) || (strcmp(mUSER[tUserIndex].mAvatarInfo.aTeacher, "") != 0) || (
		strcmp(mUSER[tUserIndex].mAvatarInfo.aStudent, "") != 0))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mAVATAR_OBJECT[tUserIndex].CheckCommunityWork())
	{
		mTRANSFER.B_TEACHER_ANSWER_RECV(3);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if ((mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aSort == 11) || (mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aSort ==
		12))
	{
		mTRANSFER.B_TEACHER_ANSWER_RECV(3);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	tOtherAvatarIndex = mUTIL.SearchAvatar(tAvatarName, tUserIndex);
	if (tOtherAvatarIndex == -1)
	{
		mTRANSFER.B_TEACHER_ANSWER_RECV(4);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if ((mAVATAR_OBJECT[tUserIndex].mDATA.aTribe != mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aTribe) || (mUSER[
		tOtherAvatarIndex].mAvatarInfo.aLevel1 > 112))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if ((mUSER[tOtherAvatarIndex].mMoveZoneResult == 1) || (mAVATAR_OBJECT[tOtherAvatarIndex].CheckCommunityWork()))
	{
		mTRANSFER.B_TEACHER_ANSWER_RECV(5);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if ((mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aAction.aSort == 11) || (mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.
		aAction.aSort == 12))
	{
		mTRANSFER.B_TEACHER_ANSWER_RECV(5);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if (strcmp(mUSER[tOtherAvatarIndex].mAvatarInfo.aTeacher, "") != 0)
	{
		mTRANSFER.B_TEACHER_ANSWER_RECV(6);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if (strcmp(mUSER[tOtherAvatarIndex].mAvatarInfo.aStudent, "") != 0)
	{
		mTRANSFER.B_TEACHER_ANSWER_RECV(7);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	mAVATAR_OBJECT[tUserIndex].mTeacherProcessState = 1;
	strncpy(mAVATAR_OBJECT[tUserIndex].mTeacherProcessAvatarName, mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aName,
	        MAX_AVATAR_NAME_LENGTH);
	mAVATAR_OBJECT[tOtherAvatarIndex].mTeacherProcessState = 2;
	strncpy(mAVATAR_OBJECT[tOtherAvatarIndex].mTeacherProcessAvatarName, mAVATAR_OBJECT[tUserIndex].mDATA.aName,
	        MAX_AVATAR_NAME_LENGTH);
	mTRANSFER.B_TEACHER_ASK_RECV(&mUSER[tUserIndex].mAvatarInfo.aName[0]);
	mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_TEACHER_ANSWER_SEND
void W_TEACHER_ANSWER_SEND(int tUserIndex)
{
	char* tPacket = &mUSER[tUserIndex].mBUFFER_FOR_RECV[8];
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	int tAnswer;
	CopyMemory(&tAnswer, &tPacket[1], 4);

	int tOtherAvatarIndex;

	if (mAVATAR_OBJECT[tUserIndex].mTeacherProcessState != 2)
	{
		return;
	}
	switch (tAnswer)
	{
	case 0:
		mAVATAR_OBJECT[tUserIndex].mTeacherProcessState = 3;
		break;
	case 1:
		mAVATAR_OBJECT[tUserIndex].mTeacherProcessState = 0;
		break;
	case 2:
		mAVATAR_OBJECT[tUserIndex].mTeacherProcessState = 0;
		break;
	default:
		return;
	}
	tOtherAvatarIndex = mUTIL.SearchAvatar(mAVATAR_OBJECT[tUserIndex].mTeacherProcessAvatarName, tUserIndex);
	if (tOtherAvatarIndex == -1)
	{
		return;
	}
	if (mAVATAR_OBJECT[tOtherAvatarIndex].mTeacherProcessState != 1)
	{
		return;
	}
	if (strcmp(mAVATAR_OBJECT[tOtherAvatarIndex].mTeacherProcessAvatarName,
	           mAVATAR_OBJECT[tUserIndex].mDATA.aName) != 0)
	{
		return;
	}
	if (mUSER[tOtherAvatarIndex].mMoveZoneResult == 1)
	{
		return;
	}
	switch (tAnswer)
	{
	case 0:
		mAVATAR_OBJECT[tOtherAvatarIndex].mTeacherProcessState = 3;
		break;
	case 1:
		mAVATAR_OBJECT[tOtherAvatarIndex].mTeacherProcessState = 0;
		break;
	case 2:
		mAVATAR_OBJECT[tOtherAvatarIndex].mTeacherProcessState = 0;
		break;
	default:
		return;
	}
	mTRANSFER.B_TEACHER_ANSWER_RECV(tAnswer);
	mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_TEACHER_CANCEL_SEND
void W_TEACHER_CANCEL_SEND(int tUserIndex)
{
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	int tOtherAvatarIndex;

	if (mAVATAR_OBJECT[tUserIndex].mTeacherProcessState != 1)
	{
		return;
	}
	mAVATAR_OBJECT[tUserIndex].mTeacherProcessState = 0;
	tOtherAvatarIndex = mUTIL.SearchAvatar(mAVATAR_OBJECT[tUserIndex].mTeacherProcessAvatarName, tUserIndex);
	if (tOtherAvatarIndex == -1)
	{
		return;
	}
	if (mAVATAR_OBJECT[tOtherAvatarIndex].mTeacherProcessState != 2)
	{
		return;
	}
	if (strcmp(mAVATAR_OBJECT[tOtherAvatarIndex].mTeacherProcessAvatarName,
	           mAVATAR_OBJECT[tUserIndex].mDATA.aName) != 0)
	{
		return;
	}
	if (mUSER[tOtherAvatarIndex].mMoveZoneResult == 1)
	{
		return;
	}
	mAVATAR_OBJECT[tOtherAvatarIndex].mTeacherProcessState = 0;
	mTRANSFER.B_TEACHER_CANCEL_RECV();
	mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_TEACHER_START_SEND
void W_TEACHER_START_SEND(int tUserIndex)
{
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	int tOtherAvatarIndex;

	if (mAVATAR_OBJECT[tUserIndex].mTeacherProcessState != 3)
	{
		return;
	}
	tOtherAvatarIndex = mUTIL.SearchAvatar(mAVATAR_OBJECT[tUserIndex].mTeacherProcessAvatarName, tUserIndex);
	if (tOtherAvatarIndex == -1)
	{
		mAVATAR_OBJECT[tUserIndex].mTeacherProcessState = 0;
		return;
	}
	if (mAVATAR_OBJECT[tOtherAvatarIndex].mTeacherProcessState != 3)
	{
		mAVATAR_OBJECT[tUserIndex].mTeacherProcessState = 0;
		return;
	}
	if (strcmp(mAVATAR_OBJECT[tOtherAvatarIndex].mTeacherProcessAvatarName,
	           mAVATAR_OBJECT[tUserIndex].mDATA.aName) != 0)
	{
		mAVATAR_OBJECT[tUserIndex].mTeacherProcessState = 0;
		return;
	}
	if (mUSER[tOtherAvatarIndex].mMoveZoneResult == 1)
	{
		mAVATAR_OBJECT[tUserIndex].mTeacherProcessState = 0;
		return;
	}
	mAVATAR_OBJECT[tUserIndex].mTeacherProcessState = 0;
	strncpy(mUSER[tUserIndex].mAvatarInfo.aStudent, mUSER[tOtherAvatarIndex].mAvatarInfo.aName, MAX_AVATAR_NAME_LENGTH);
	mTRANSFER.B_TEACHER_START_RECV(1, &mUSER[tOtherAvatarIndex].mAvatarInfo.aName[0]);
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	mAVATAR_OBJECT[tOtherAvatarIndex].mTeacherProcessState = 0;
	strncpy(mUSER[tOtherAvatarIndex].mAvatarInfo.aTeacher, mUSER[tUserIndex].mAvatarInfo.aName, MAX_AVATAR_NAME_LENGTH);
	mTRANSFER.B_TEACHER_START_RECV(2, &mUSER[tUserIndex].mAvatarInfo.aName[0]);
	mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_TEACHER_END_SEND
void W_TEACHER_END_SEND(int tUserIndex)
{
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	if ((strcmp(mUSER[tUserIndex].mAvatarInfo.aTeacher, "") == 0) && (strcmp(mUSER[tUserIndex].mAvatarInfo.aStudent, "")
		== 0))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	strcpy(mUSER[tUserIndex].mAvatarInfo.aTeacher, "");
	strcpy(mUSER[tUserIndex].mAvatarInfo.aStudent, "");
	mTRANSFER.B_TEACHER_END_RECV();
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//W_TEACHER_STATE_SEND
void W_TEACHER_STATE_SEND(int tUserIndex)
{
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		return;
	}

	int tOtherAvatarIndex;

	if ((strcmp(mUSER[tUserIndex].mAvatarInfo.aTeacher, "") == 0) && (strcmp(mUSER[tUserIndex].mAvatarInfo.aStudent, "")
		== 0))
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (strcmp(mUSER[tUserIndex].mAvatarInfo.aTeacher, "") != 0)
	{
		tOtherAvatarIndex = mUTIL.SearchAvatar(mUSER[tUserIndex].mAvatarInfo.aTeacher, tUserIndex);
		if (tOtherAvatarIndex == -1)
		{
			return;
		}
		if (strcmp(mUSER[tOtherAvatarIndex].mAvatarInfo.aStudent, mUSER[tUserIndex].mAvatarInfo.aName) == 0)
		{
			mTRANSFER.B_TEACHER_STATE_RECV(0);
		}
		else
		{
			mTRANSFER.B_TEACHER_STATE_RECV(1);
		}
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	if (strcmp(mUSER[tUserIndex].mAvatarInfo.aStudent, "") != 0)
	{
		tOtherAvatarIndex = mUTIL.SearchAvatar(mUSER[tUserIndex].mAvatarInfo.aStudent, tUserIndex);
		if (tOtherAvatarIndex == -1)
		{
			return;
		}
		if (strcmp(mUSER[tOtherAvatarIndex].mAvatarInfo.aTeacher, mUSER[tUserIndex].mAvatarInfo.aName) == 0)
		{
			mTRANSFER.B_TEACHER_STATE_RECV(0);
		}
		else
		{
			mTRANSFER.B_TEACHER_STATE_RECV(1);
		}
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	}
}

//W_CHANGE_TO_TRIBE4_SEND
void W_CHANGE_TO_TRIBE4_SEND(int tUserIndex)
{
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	time_t tCountSeconds;
	struct tm* tPresentTime;

	time(&tCountSeconds);
	tPresentTime = localtime(&tCountSeconds);
	if (tPresentTime == nullptr)
	{
		mTRANSFER.B_CHANGE_TO_TRIBE4_RECV(2);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}

	//saturday, 12:00 ~ 24:00
	if (((tPresentTime->tm_wday != 6) || (tPresentTime->tm_hour < 12)) &&
		((tPresentTime->tm_wday != 0) || (tPresentTime->tm_hour > 0) || (tPresentTime->tm_min > 0)))
	{
		mTRANSFER.B_CHANGE_TO_TRIBE4_RECV(2);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		return;
	}
	int index01;
	int iCount = 0;
	int tBroadcastInfoSort;
	BYTE tBroadcastInfoData[MAX_BROADCAST_INFO_SIZE];

	switch (mUSER[tUserIndex].mAvatarInfo.aTribe)
	{
	case 0:
		if (mSERVER_INFO.mServerNumber != 71)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		break;
	case 1:
		if (mSERVER_INFO.mServerNumber != 72)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		break;
	case 2:
		if (mSERVER_INFO.mServerNumber != 73)
		{
			mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		break;
	}

	for (iCount = 0; iCount < MAX_TRIBE_VOTE_AVATAR_NUM; iCount++)
	{
		if (!strcmp(mGAME.mTribeInfo.mTribeVoteName[mUSER[tUserIndex].mAvatarInfo.aTribe][iCount]
		            , mUSER[tUserIndex].mAvatarInfo.aName))
		{
			mTRANSFER.B_CHANGE_TO_TRIBE4_RECV(1);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
	}

	if (mUSER[tUserIndex].mAvatarInfo.aTribe == 3)
	{
		if (mUSER[tUserIndex].mAvatarInfo.aFactionReturnScroll < 1)
		{
			mTRANSFER.B_CHANGE_TO_TRIBE4_RECV(2);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			return;
		}
		mUSER[tUserIndex].mAvatarInfo.aTribe = mUSER[tUserIndex].mAvatarInfo.aPreviousTribe;
		mUSER[tUserIndex].mAvatarInfo.aQuestInfo[0] = 0;
		mUSER[tUserIndex].mAvatarInfo.aQuestInfo[1] = 0;
		mUSER[tUserIndex].mAvatarInfo.aQuestInfo[2] = 0;
		mUSER[tUserIndex].mAvatarInfo.aQuestInfo[3] = 0;
		mUSER[tUserIndex].mAvatarInfo.aQuestInfo[4] = 0;
		mUSER[tUserIndex].mAvatarInfo.aFactionReturnScroll--;
		mAVATAR_OBJECT[tUserIndex].mDATA.aTribe = mUSER[tUserIndex].mAvatarInfo.aPreviousTribe;
		mTRANSFER.B_CHANGE_TO_TRIBE4_RECV(0);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 419;
		CopyMemory(&tBroadcastInfoData[0], &mUSER[tUserIndex].mAvatarInfo.aPreviousTribe, 4);
		CopyMemory(&tBroadcastInfoData[4], &mUSER[tUserIndex].mAvatarInfo.aName[0], MAX_AVATAR_NAME_LENGTH);
		mCENTER_COM.U_DEMAND_BROADCAST_INFO(tBroadcastInfoSort, &tBroadcastInfoData[0]);
		//--------------//
		//--------------//
		//--------------//
		return;
	}
	if (mUSER[tUserIndex].mAvatarInfo.aLevel1 < 113)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
#if 0
	if (mGAME.ReturnTribePointWithAlliance(mUSER[tUserIndex].mAvatarInfo.aTribe) < 100)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mGAME.CheckPossibleChangeToTribe4(mUSER[tUserIndex].mAvatarInfo.aTribe) != 0)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
#endif
	if (100 > mGAME.mWorldInfo.mTribePoint[mUSER[tUserIndex].mAvatarInfo.aTribe])
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mGAME.CheckPossibleChangeToTribe4_2(mUSER[tUserIndex].mAvatarInfo.aTribe) != 0)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mGAME.ReturnTribeRole(&mUSER[tUserIndex].mAvatarInfo.aName[0], mUSER[tUserIndex].mAvatarInfo.aTribe) != 0)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (strcmp(mUSER[tUserIndex].mAvatarInfo.aGuildName, "") != 0)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (strcmp(mUSER[tUserIndex].mAvatarInfo.aTeacher, "") != 0)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (strcmp(mUSER[tUserIndex].mAvatarInfo.aStudent, "") != 0)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	for (index01 = 0; index01 < MAX_FRIEND_NUM; index01++)
	{
		if (strcmp(mUSER[tUserIndex].mAvatarInfo.aFriend[index01], "") != 0)
		{
			break;
		}
	}
	if (index01 < MAX_FRIEND_NUM)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	mUSER[tUserIndex].mAvatarInfo.aTribe = 3;
	mUSER[tUserIndex].mAvatarInfo.aQuestInfo[0] = 0;
	mUSER[tUserIndex].mAvatarInfo.aQuestInfo[1] = 0;
	mUSER[tUserIndex].mAvatarInfo.aQuestInfo[2] = 0;
	mUSER[tUserIndex].mAvatarInfo.aQuestInfo[3] = 0;
	mUSER[tUserIndex].mAvatarInfo.aQuestInfo[4] = 0;
	mAVATAR_OBJECT[tUserIndex].mDATA.aTribe = 3;
	mTRANSFER.B_CHANGE_TO_TRIBE4_RECV(0);
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	//--------------//
	//BROADCAST_INFO//
	//--------------//
	tBroadcastInfoSort = 51;
	CopyMemory(&tBroadcastInfoData[0], &mUSER[tUserIndex].mAvatarInfo.aPreviousTribe, 4);
	CopyMemory(&tBroadcastInfoData[4], &mUSER[tUserIndex].mAvatarInfo.aName[0], MAX_AVATAR_NAME_LENGTH);
	mCENTER_COM.U_DEMAND_BROADCAST_INFO(tBroadcastInfoSort, &tBroadcastInfoData[0]);
	//--------------//
	//--------------//
	//--------------//
}

//W_HERO_RANKING_SEND
void W_HERO_RANKING_SEND(int tUserIndex)
{
	if (!mAVATAR_OBJECT[tUserIndex].mCheckValidState)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[tUserIndex].mMoveZoneResult == 1)
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
	}
}
