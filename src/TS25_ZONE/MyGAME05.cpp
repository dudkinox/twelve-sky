//-------------------------------------------------------------------------------------------------
//HEADER
//-------------------------------------------------------------------------------------------------
#include "MainApp.h"
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MONSTER_OBJECT
//-------------------------------------------------------------------------------------------------
//INSTANCE
MONSTER_OBJECT mMONSTER_OBJECT[MAX_MONSTER_OBJECT_NUM];
//SET_SPACE_INDEX
void MONSTER_OBJECT::SetSpaceIndex(void)
{
	mSPACE_INDEX[0] = static_cast<int>(mDATA.mAction.aLocation[0] / MAX_RADIUS_FOR_NETWORK);
	mSPACE_INDEX[1] = static_cast<int>(mDATA.mAction.aLocation[1] / MAX_RADIUS_FOR_NETWORK);
	mSPACE_INDEX[2] = static_cast<int>(mDATA.mAction.aLocation[2] / MAX_RADIUS_FOR_NETWORK);
}

//UPDATE
void MONSTER_OBJECT::Update(int tObjectIndex, float tPostTime)
{
	if (!mCheckValidState)
	{
		return;
	}
	switch (mDATA.mAction.aSort)
	{
	case 0:
		A001(tObjectIndex, tPostTime);
		return;
	case 1:
		switch (mMONSTER_INFO->mSpecialType)
		{
		case 40:
		case 41:
		case 42:
		case 43:
		case 44:
			A002_FOR_ZONE_175_TYPE_BOSS(tObjectIndex, tPostTime);
			return;
		default:
			A002(tObjectIndex, tPostTime);
			return;
		}
		return;
	case 3:
		A004(tObjectIndex, tPostTime);
		SetSpaceIndex();
		return;
	case 4:
		switch (mMONSTER_INFO->mSpecialType)
		{
		case 40:
		case 41:
		case 42:
		case 43:
		case 44:
			A005_FOR_ZONE_175_TYPE_BOSS(tObjectIndex, tPostTime);
			SetSpaceIndex();
			return;
		default:
			A005(tObjectIndex, tPostTime);
			SetSpaceIndex();
			return;
		}
		return;
	case 5:
		A006(tObjectIndex, tPostTime);
		return;
	case 7:
		A008(tObjectIndex, tPostTime);
		return;
	case 8:
		A009(tObjectIndex, tPostTime);
		return;
	case 12:
		A013(tObjectIndex, tPostTime);
		return;
	case 19:
		A020(tObjectIndex, tPostTime);
		SetSpaceIndex();
	}
}

//SELECT_AVATAR_INDEX_FOR_POSSIBLE_ATTACK
int MONSTER_OBJECT::SelectAvatarIndexForPossibleAttack(void)
{
	int index01;
	int index02;
	int index03;
	int tSelectAvatarIndex;
	unsigned int tRandom = 0;

	if ((mMONSTER_INFO->mAttackType != 1) && (mMONSTER_INFO->mAttackType != 3) && (mMONSTER_INFO->mAttackType != 6))
	{
		return -1;
	}
	if ((GetTickCount64() - mCheckDetectEnemyTime) < 1000)
	{
		return -1;
	}
	mCheckDetectEnemyTime = GetTickCount64();
	if (mMONSTER_INFO->mRadiusInfo[1] < 1)
	{
		return -1;
	}
	tSelectAvatarIndex = -1;
	for (index01 = 0; index01 < mSERVER.mMAX_USER_NUM; index01++)
	{
		if (!mAVATAR_OBJECT[index01].mCheckValidState)
		{
			continue;
		}
		if (1 == mAVATAR_OBJECT[index01].mGod)
		{
			continue;
		}
		if (1 == mUSER[index01].mAuthInfo.AttackFlag)
		{
			continue;
		}
		if (mUSER[index01].mMoveZoneResult == 1)
		{
			continue;
		}
		if (mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0)
		{
			continue;
		}
		if (mAVATAR_OBJECT[index01].mDATA.aLifeValue < 1)
		{
			continue;
		}
		if ((mAVATAR_OBJECT[index01].mDATA.aAction.aSort == 0) || (mAVATAR_OBJECT[index01].mDATA.aAction.aSort == 33))
		{
			continue;
		}
		if ((abs(mAVATAR_OBJECT[index01].mSPACE_INDEX[0] - mSPACE_INDEX[0]) > 1) || (
			abs(mAVATAR_OBJECT[index01].mSPACE_INDEX[1] - mSPACE_INDEX[1]) > 1) || (abs(
			mAVATAR_OBJECT[index01].mSPACE_INDEX[2] - mSPACE_INDEX[2]) > 1))
		{
			continue;
		}
		if (mUTIL.GetDoubleX_Z(&mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mDATA.mAction.aLocation[0]) >
			static_cast<float>(mMONSTER_INFO->mRadiusInfo[1] * mMONSTER_INFO->mRadiusInfo[1]))
		{
			continue;
		}
		index03 = 0;
		for (index02 = 0; index02 < MAX_MONSTER_OBJECT_NUM; index02++)
		{
			if (!mMONSTER_OBJECT[index02].mCheckValidState)
			{
				continue;
			}
			if ((mMONSTER_OBJECT[index02].mDATA.mAction.aSort != 4) && (mMONSTER_OBJECT[index02].mDATA.mAction.aSort !=
				5))
			{
				continue;
			}
			if ((mMONSTER_OBJECT[index02].mDATA.mAction.aTargetObjectIndex != index01) || (mMONSTER_OBJECT[index02].
				mDATA.mAction.aTargetObjectUniqueNumber != mAVATAR_OBJECT[index01].mUniqueNumber))
			{
				continue;
			}
			index03++;
		}
		if (index03 > (mSameTargetPostNum - 1))
		{
			continue;
		}
		tRandom = rand();
		if ((tRandom % 2) == 0)
		{
			tSelectAvatarIndex = index01;
			break;
		}
	}
	return tSelectAvatarIndex;
}

//SELECT_AVATAR_INDEX_FOR_POSSIBLE_ATTACK_FOR_ZONE_175_TYPE_BOSS
void MONSTER_OBJECT::SelectAvatarIndexForPossibleAttackForZone175TypeBoss(void)
{
	int index01;
	int tWhoAttackMonster;
	unsigned int tRandom = 0;

	if ((GetTickCount64() - mCheckDetectEnemyTime) < 1000)
	{
		return;
	}
	mCheckDetectEnemyTime = GetTickCount64();
	mCheckAttackState = FALSE;
	mTotalAttackNum = 0;
	for (index01 = 0; index01 < mSERVER.mMAX_USER_NUM; index01++)
	{
		if (!mAVATAR_OBJECT[index01].mCheckValidState)
		{
			continue;
		}
		if (1 == mAVATAR_OBJECT[index01].mGod)
		{
			continue;
		}
		if (1 == mUSER[index01].mAuthInfo.AttackFlag)
		{
			continue;
		}
		if (mUSER[index01].mMoveZoneResult == 1)
		{
			continue;
		}
		if (mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0)
		{
			continue;
		}
		if (mAVATAR_OBJECT[index01].mDATA.aLifeValue < 1)
		{
			continue;
		}
		if ((mAVATAR_OBJECT[index01].mDATA.aAction.aSort == 0) || (mAVATAR_OBJECT[index01].mDATA.aAction.aSort == 33))
		{
			continue;
		}
		if (mUTIL.GetDoubleX_Z(&mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mDATA.mAction.aLocation[0]) >
			static_cast<float>(mMONSTER_INFO->mRadiusInfo[1] * mMONSTER_INFO->mRadiusInfo[1]))
		{
			continue;
		}
		tRandom = rand();
		if ((tRandom % 2) == 0)
		{
			SetAttackInfoWithAvatar(index01, mAVATAR_OBJECT[index01].mUniqueNumber, &tWhoAttackMonster);
		}
	}
}

//SELECT_AVATAR_INDEX_FOR_IDLE_AI
int MONSTER_OBJECT::SelectAvatarIndexForIdleAI(void)
{
	int index01;
	unsigned int tRandom = 0;

	tRandom = rand();
	if (((tRandom % 100) != 0) || (mMONSTER_INFO->mWalkSpeed < 1))
	{
		return -1;
	}
	for (index01 = 0; index01 < mSERVER.mMAX_USER_NUM; index01++)
	{
		if (!mAVATAR_OBJECT[index01].mCheckValidState)
		{
			continue;
		}
		if (1 == mAVATAR_OBJECT[index01].mGod)
		{
			continue;
		}
		if (1 == mUSER[index01].mAuthInfo.AttackFlag)
		{
			continue;
		}
		if (mUSER[index01].mMoveZoneResult == 1)
		{
			continue;
		}
		if (mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0)
		{
			continue;
		}
		if ((abs(mAVATAR_OBJECT[index01].mSPACE_INDEX[0] - mSPACE_INDEX[0]) > 1) || (
			abs(mAVATAR_OBJECT[index01].mSPACE_INDEX[1] - mSPACE_INDEX[1]) > 1) || (abs(
			mAVATAR_OBJECT[index01].mSPACE_INDEX[2] - mSPACE_INDEX[2]) > 1))
		{
			continue;
		}
		break;
	}
	if (index01 < mSERVER.mMAX_USER_NUM)
	{
		return index01;
	}
	return -1;
}

//ADJUST_VALID_ATTACK_TARGET
void MONSTER_OBJECT::AdjustValidAttackTarget(void)
{
	int index01;
	int index02;
	int index03;
	float tLength;
	int tTotalAttackNum;
	int tAttackIndex[MAX_MONSTER_OBJECT_ATTACK_NUM];
	DWORD tAttackUniqueNumber[MAX_MONSTER_OBJECT_ATTACK_NUM];
	DWORD tAttackDamage[MAX_MONSTER_OBJECT_ATTACK_NUM];
	float tAttackLength[MAX_MONSTER_OBJECT_ATTACK_NUM];

	tTotalAttackNum = 0;
	if ((mMONSTER_INFO->mRadiusInfo[0] > 0) && (mMONSTER_INFO->mRadiusInfo[1] > 0))
	{
		for (index01 = 0; index01 < mTotalAttackNum; index01++)
		{
			if (!mAVATAR_OBJECT[mAttackIndex[index01]].mCheckValidState)
			{
				continue;
			}
			if (1 == mAVATAR_OBJECT[mAttackIndex[index01]].mGod)
			{
				continue;
			}
			if (1 == mUSER[mAttackIndex[index01]].mAuthInfo.AttackFlag)
			{
				continue;
			}
			if (mAVATAR_OBJECT[mAttackIndex[index01]].mUniqueNumber != mAttackUniqueNumber[index01])
			{
				continue;
			}
			if (mUSER[mAttackIndex[index01]].mMoveZoneResult == 1)
			{
				continue;
			}
			if (mAVATAR_OBJECT[mAttackIndex[index01]].mDATA.aVisibleState == 0)
			{
				continue;
			}
			if (mAVATAR_OBJECT[mAttackIndex[index01]].mDATA.aLifeValue < 1)
			{
				continue;
			}
			if ((mAVATAR_OBJECT[mAttackIndex[index01]].mDATA.aAction.aSort == 0) || (mAVATAR_OBJECT[mAttackIndex[
				index01]].mDATA.aAction.aSort == 33))
			{
				continue;
			}
			if ((abs(mAVATAR_OBJECT[mAttackIndex[index01]].mSPACE_INDEX[0] - mSPACE_INDEX[0]) > 1) || (abs(
				mAVATAR_OBJECT[mAttackIndex[index01]].mSPACE_INDEX[1] - mSPACE_INDEX[1]) > 1) || (abs(
				mAVATAR_OBJECT[mAttackIndex[index01]].mSPACE_INDEX[2] - mSPACE_INDEX[2]) > 1))
			{
				continue;
			}
			tLength = mUTIL.GetDoubleX_Z(&mAVATAR_OBJECT[mAttackIndex[index01]].mDATA.aAction.aLocation[0],
			                             &mDATA.mAction.aLocation[0]);
			if (tLength > static_cast<float>(mMONSTER_INFO->mRadiusInfo[1] * mMONSTER_INFO->mRadiusInfo[1]))
			{
				continue;
			}
			if (tLength > static_cast<float>(mMONSTER_INFO->mRadiusInfo[0] * mMONSTER_INFO->mRadiusInfo[0]))
			{
				index03 = 0;
				for (index02 = 0; index02 < MAX_MONSTER_OBJECT_NUM; index02++)
				{
					if (!mMONSTER_OBJECT[index02].mCheckValidState)
					{
						continue;
					}
					if ((mMONSTER_OBJECT[index02].mDATA.mAction.aSort != 4) && (mMONSTER_OBJECT[index02].mDATA.mAction.
						aSort != 5))
					{
						continue;
					}
					if ((mMONSTER_OBJECT[index02].mDATA.mAction.aTargetObjectIndex != mAttackIndex[index01]) || (
						mMONSTER_OBJECT[index02].mDATA.mAction.aTargetObjectUniqueNumber != mAttackUniqueNumber[
							index01]))
					{
						continue;
					}
					index03++;
				}
				if (index03 > (mSameTargetPostNum - 1))
				{
					continue;
				}
			}
			else
			{
				if (fabsf(mAVATAR_OBJECT[mAttackIndex[index01]].mDATA.aAction.aLocation[1] - mDATA.mAction.aLocation[1])
					> static_cast<float>(mMONSTER_INFO->mSize[1]))
				{
					continue;
				}
			}
			tAttackIndex[tTotalAttackNum] = mAttackIndex[index01];
			tAttackUniqueNumber[tTotalAttackNum] = mAttackUniqueNumber[index01];
			tAttackDamage[tTotalAttackNum] = mAttackDamage[index01];
			tAttackLength[tTotalAttackNum] = tLength;
			tTotalAttackNum++;
		}
	}
	if (tTotalAttackNum < 1)
	{
		mCheckAttackState = FALSE;
		mTotalAttackNum = 0;
		return;
	}
	mTotalAttackNum = tTotalAttackNum;
	for (index01 = 0; index01 < tTotalAttackNum; index01++)
	{
		mAttackIndex[index01] = tAttackIndex[index01];
		mAttackUniqueNumber[index01] = tAttackUniqueNumber[index01];
		mAttackDamage[index01] = tAttackDamage[index01];
		mAttackLength[index01] = tAttackLength[index01];
	}
}

//ADJUST_VALID_ATTACK_TARGET_FOR_ZONE_175_TYPE_BOSS
void MONSTER_OBJECT::AdjustValidAttackTargetForZone175TypeBoss(void)
{
	int index01;
	float tLength;
	int tTotalAttackNum;
	int tAttackIndex[MAX_MONSTER_OBJECT_ATTACK_NUM];
	DWORD tAttackUniqueNumber[MAX_MONSTER_OBJECT_ATTACK_NUM];
	DWORD tAttackDamage[MAX_MONSTER_OBJECT_ATTACK_NUM];
	float tAttackLength[MAX_MONSTER_OBJECT_ATTACK_NUM];

	tTotalAttackNum = 0;
	for (index01 = 0; index01 < mTotalAttackNum; index01++)
	{
		if (!mAVATAR_OBJECT[mAttackIndex[index01]].mCheckValidState)
		{
			continue;
		}
		if (1 == mAVATAR_OBJECT[mAttackIndex[index01]].mGod)
		{
			continue;
		}
		if (1 == mUSER[mAttackIndex[index01]].mAuthInfo.AttackFlag)
		{
			continue;
		}
		if (mAVATAR_OBJECT[mAttackIndex[index01]].mUniqueNumber != mAttackUniqueNumber[index01])
		{
			continue;
		}
		if (mUSER[mAttackIndex[index01]].mMoveZoneResult == 1)
		{
			continue;
		}
		if (mAVATAR_OBJECT[mAttackIndex[index01]].mDATA.aVisibleState == 0)
		{
			continue;
		}
		if (mAVATAR_OBJECT[mAttackIndex[index01]].mDATA.aLifeValue < 1)
		{
			continue;
		}
		if ((mAVATAR_OBJECT[mAttackIndex[index01]].mDATA.aAction.aSort == 0) || (mAVATAR_OBJECT[mAttackIndex[index01]].
			mDATA.aAction.aSort == 33))
		{
			continue;
		}
		tLength = mUTIL.GetDoubleX_Z(&mAVATAR_OBJECT[mAttackIndex[index01]].mDATA.aAction.aLocation[0],
		                             &mDATA.mAction.aLocation[0]);
		if (tLength > static_cast<float>(mMONSTER_INFO->mRadiusInfo[1] * mMONSTER_INFO->mRadiusInfo[1]))
		{
			continue;
		}
		tAttackIndex[tTotalAttackNum] = mAttackIndex[index01];
		tAttackUniqueNumber[tTotalAttackNum] = mAttackUniqueNumber[index01];
		tAttackDamage[tTotalAttackNum] = mAttackDamage[index01];
		tAttackLength[tTotalAttackNum] = tLength;
		tTotalAttackNum++;
	}
	if (tTotalAttackNum < 1)
	{
		mCheckAttackState = FALSE;
		mTotalAttackNum = 0;
		return;
	}
	mTotalAttackNum = tTotalAttackNum;
	for (index01 = 0; index01 < tTotalAttackNum; index01++)
	{
		mAttackIndex[index01] = tAttackIndex[index01];
		mAttackUniqueNumber[index01] = tAttackUniqueNumber[index01];
		mAttackDamage[index01] = tAttackDamage[index01];
		mAttackLength[index01] = tAttackLength[index01];
	}
}

//SELECT_AVATAR_INDEX_FOR_ATTACK_ACTION
int MONSTER_OBJECT::SelectAvatarIndexForAttackAction(void)
{
	int index01;
	int tSelectAvatarIndex;
	unsigned int tRandom = 0;

	if ((mMONSTER_INFO->mAttackType != 1) && (mMONSTER_INFO->mAttackType != 3) && (mMONSTER_INFO->mAttackType != 6))
	{
		return -1;
	}
	if ((GetTickCount64() - mCheckDetectEnemyTime) < 1000)
	{
		return -1;
	}
	mCheckDetectEnemyTime = GetTickCount64();
	if (mMONSTER_INFO->mRadiusInfo[0] < 1)
	{
		return -1;
	}
	tSelectAvatarIndex = -1;
	for (index01 = 0; index01 < mSERVER.mMAX_USER_NUM; index01++)
	{
		if (!mAVATAR_OBJECT[index01].mCheckValidState)
		{
			continue;
		}
		if (mUSER[index01].mMoveZoneResult == 1)
		{
			continue;
		}
		if (mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0)
		{
			continue;
		}
		if (mAVATAR_OBJECT[index01].mDATA.aLifeValue < 1)
		{
			continue;
		}
		if ((mAVATAR_OBJECT[index01].mDATA.aAction.aSort == 0) || (mAVATAR_OBJECT[index01].mDATA.aAction.aSort == 33))
		{
			continue;
		}
		if ((abs(mAVATAR_OBJECT[index01].mSPACE_INDEX[0] - mSPACE_INDEX[0]) > 1) || (
			abs(mAVATAR_OBJECT[index01].mSPACE_INDEX[1] - mSPACE_INDEX[1]) > 1) || (abs(
			mAVATAR_OBJECT[index01].mSPACE_INDEX[2] - mSPACE_INDEX[2]) > 1))
		{
			continue;
		}
		if (mUTIL.GetDoubleX_Z(&mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mDATA.mAction.aLocation[0]) >
			static_cast<float>(mMONSTER_INFO->mRadiusInfo[0] * mMONSTER_INFO->mRadiusInfo[0]))
		{
			continue;
		}
		if (fabsf(mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[1] - mDATA.mAction.aLocation[1]) > static_cast<float>(
			mMONSTER_INFO->mSize[1]))
		{
			continue;
		}
		tRandom = rand();
		if ((tRandom % 2) == 0)
		{
			tSelectAvatarIndex = index01;
			break;
		}
	}
	return tSelectAvatarIndex;
}

//SELECT_AVATAR_INDEX_FOR_GUARD_ATTACK
int MONSTER_OBJECT::SelectAvatarIndexForGuardAttack(void)
{
	int index01;
	int tMyTribe;
	int tAllianceTribe;
	int tSelectAvatarIndex;
	unsigned int tRandom = 0;

	if ((GetTickCount64() - mCheckDetectEnemyTime) < 1000)
	{
		return -1;
	}
	mCheckDetectEnemyTime = GetTickCount64();
	if (mMONSTER_INFO->mRadiusInfo[0] < 1)
	{
		return -1;
	}
	switch (mMONSTER_INFO->mType)
	{
	case 6:
		tMyTribe = 0;
		break;
	case 7:
		tMyTribe = 1;
		break;
	case 8:
		tMyTribe = 2;
		break;
	case 9:
		tMyTribe = 3;
		break;
	default:
		return -1;
	}
	tAllianceTribe = mGAME.ReturnAllianceTribe(tMyTribe);
	tSelectAvatarIndex = -1;
	for (index01 = 0; index01 < mSERVER.mMAX_USER_NUM; index01++)
	{
		if (!mAVATAR_OBJECT[index01].mCheckValidState)
		{
			continue;
		}
		if (1 == mAVATAR_OBJECT[index01].mGod)
		{
			continue;
		}
		if (1 == mUSER[index01].mAuthInfo.AttackFlag)
		{
			continue;
		}
		if (mUSER[index01].mMoveZoneResult == 1)
		{
			continue;
		}
		if (mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0)
		{
			continue;
		}
		if (mAVATAR_OBJECT[index01].mDATA.aLifeValue < 1)
		{
			continue;
		}
		if ((mAVATAR_OBJECT[index01].mDATA.aTribe == tMyTribe) || (mAVATAR_OBJECT[index01].mDATA.aTribe ==
			tAllianceTribe))
		{
			continue;
		}
		if ((mAVATAR_OBJECT[index01].mDATA.aAction.aSort == 0) || (mAVATAR_OBJECT[index01].mDATA.aAction.aSort == 33))
		{
			continue;
		}
		if ((abs(mAVATAR_OBJECT[index01].mSPACE_INDEX[0] - mSPACE_INDEX[0]) > 2) || (
			abs(mAVATAR_OBJECT[index01].mSPACE_INDEX[1] - mSPACE_INDEX[1]) > 2) || (abs(
			mAVATAR_OBJECT[index01].mSPACE_INDEX[2] - mSPACE_INDEX[2]) > 2))
		{
			continue;
		}
		if (mUTIL.GetDoubleX_Z(&mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mDATA.mAction.aLocation[0]) >
			static_cast<float>(mMONSTER_INFO->mRadiusInfo[0] * mMONSTER_INFO->mRadiusInfo[0]))
		{
			continue;
		}
		tRandom = rand();
		if ((tRandom % 2) == 0)
		{
			tSelectAvatarIndex = index01;
			break;
		}
	}
	return tSelectAvatarIndex;
}

//SELECT_AVATAR_INDEX_FOR_IDLE_AI_WITH_THROW_CAR
int MONSTER_OBJECT::SelectAvatarIndexForIdleAIWIthThrowCar(void)
{
	int index01;
	unsigned int tRandom = 0;

	tRandom = rand();
	if (((tRandom % 100) != 0) || (mMONSTER_INFO->mWalkSpeed < 1))
	{
		return -1;
	}
	for (index01 = 0; index01 < mSERVER.mMAX_USER_NUM; index01++)
	{
		if (!mAVATAR_OBJECT[index01].mCheckValidState)
		{
			continue;
		}
		if (1 == mAVATAR_OBJECT[index01].mGod)
		{
			continue;
		}
		if (1 == mUSER[index01].mAuthInfo.AttackFlag)
		{
			continue;
		}
		if (mUSER[index01].mMoveZoneResult == 1)
		{
			continue;
		}
		if (mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0)
		{
			continue;
		}
		if ((abs(mAVATAR_OBJECT[index01].mSPACE_INDEX[0] - mSPACE_INDEX[0]) > 2) || (
			abs(mAVATAR_OBJECT[index01].mSPACE_INDEX[1] - mSPACE_INDEX[1]) > 2) || (abs(
			mAVATAR_OBJECT[index01].mSPACE_INDEX[2] - mSPACE_INDEX[2]) > 2))
		{
			continue;
		}
		break;
	}
	if (index01 < mSERVER.mMAX_USER_NUM)
	{
		return index01;
	}
	return -1;
}

//SELECT_AVATAR_INDEX_FOR_THROW_CAR_ATTACK
int MONSTER_OBJECT::SelectAvatarIndexForThrowCarAttack(void)
{
	int index01;
	int tMyTribe;
	int tAllianceTribe;
	int tSelectAvatarIndex;
	float tLength;
	unsigned int tRandom = 0;

	if ((GetTickCount64() - mCheckDetectEnemyTime) < 1000)
	{
		return -1;
	}
	mCheckDetectEnemyTime = GetTickCount64();
	if (mMONSTER_INFO->mRadiusInfo[0] < 1)
	{
		return -1;
	}
	switch (mMONSTER_INFO->mSpecialType)
	{
	case 35:
		tMyTribe = 0;
		break;
	case 36:
		tMyTribe = 1;
		break;
	case 37:
		tMyTribe = 2;
		break;
	case 38:
		tMyTribe = 3;
		break;
	case 18:
		tMyTribe = -1;
		break;
	default:
		return -1;
	}
	tAllianceTribe = mGAME.ReturnAllianceTribe(tMyTribe);
	tSelectAvatarIndex = -1;
	for (index01 = 0; index01 < mSERVER.mMAX_USER_NUM; index01++)
	{
		if (!mAVATAR_OBJECT[index01].mCheckValidState)
		{
			continue;
		}
		if (1 == mAVATAR_OBJECT[index01].mGod)
		{
			continue;
		}
		if (1 == mUSER[index01].mAuthInfo.AttackFlag)
		{
			continue;
		}
		if (mUSER[index01].mMoveZoneResult == 1)
		{
			continue;
		}
		if (mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0)
		{
			continue;
		}
		if (mAVATAR_OBJECT[index01].mDATA.aLifeValue < 1)
		{
			continue;
		}
		if ((mAVATAR_OBJECT[index01].mDATA.aTribe == tMyTribe) || (mAVATAR_OBJECT[index01].mDATA.aTribe ==
			tAllianceTribe))
		{
			continue;
		}
		if ((mAVATAR_OBJECT[index01].mDATA.aAction.aSort == 0) || (mAVATAR_OBJECT[index01].mDATA.aAction.aSort == 33))
		{
			continue;
		}
		if ((abs(mAVATAR_OBJECT[index01].mSPACE_INDEX[0] - mSPACE_INDEX[0]) > 2) || (
			abs(mAVATAR_OBJECT[index01].mSPACE_INDEX[1] - mSPACE_INDEX[1]) > 2) || (abs(
			mAVATAR_OBJECT[index01].mSPACE_INDEX[2] - mSPACE_INDEX[2]) > 2))
		{
			continue;
		}
		tLength = mUTIL.GetLengthXYZ(&mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mDATA.mAction.aLocation[0]);
		if ((tLength < (static_cast<float>(mMONSTER_INFO->mRadiusInfo[0]) * 0.25f)) || (tLength > static_cast<float>(
			mMONSTER_INFO->mRadiusInfo[0])))
		{
			continue;
		}
		tRandom = rand();
		if ((tRandom % 2) == 0)
		{
			tSelectAvatarIndex = index01;
			break;
		}
	}
	return tSelectAvatarIndex;
}

//A001[생성]
void MONSTER_OBJECT::A001(int tObjectIndex, float tPostTime)
{
	mDATA.mAction.aFrame += (tPostTime * 30.0f);
	if (mDATA.mAction.aFrame < static_cast<float>(mMONSTER_INFO->mFrameInfo[0]))
	{
		return;
	}
	mDATA.mAction.aSort = 1;
	mDATA.mAction.aFrame = 0.0f;
}

//A002[아이들]
void MONSTER_OBJECT::A002(int tObjectIndex, float tPostTime)
{
	//-----------------------------------------------------------------//
	//[01].일반,[02].성석,[03].수호비,[04].동맹석,[05].수호석,[06].석차//
	//-----------------------------------------------------------------//

	int index01;
	int tBroadcastInfoSort;
	BYTE tBroadcastInfoData[MAX_BROADCAST_INFO_SIZE];
	int tTribeSymbolIndex;
	int tAllianceStoneIndex;
	int tSelectAvatarIndex;
	int tWhoAttackMonster;
	float tPossibleLocation[3];
	float tDirection[3];
	float tLength;
	float tRadius;
	float tLocation[3];
	int tSelectTargetIndex;
	float tAttackLength;
	float tAttackRadius;
	float tAttackDirect;
	float tSinValue;
	float tCosValue;
	unsigned int tRandom = 0;

	switch (mSpecialSortNumber)
	{
	case 1:
		break;
	case 2:
		if (!mCheckFirstAttackForTribeSymbol)
		{
			return;
		}
		if ((GetTickCount64() - mFirstAttackTimeForTribeSymbol) < 3600000)
		{
			return;
		}
		switch (mMONSTER_INFO->mSpecialType)
		{
		case 11:
			tTribeSymbolIndex = 0;
			break;
		case 12:
			tTribeSymbolIndex = 1;
			break;
		case 13:
			tTribeSymbolIndex = 2;
			break;
		case 28:
			tTribeSymbolIndex = 3;
			break;
		case 14:
			tTribeSymbolIndex = 4;
			break;
		case 15:
			return;
		}
	//--------------//
	//BROADCAST_INFO//
	//--------------//
		tBroadcastInfoSort = 43;
		CopyMemory(&tBroadcastInfoData[0], &tTribeSymbolIndex, 4);
		mCENTER_COM.U_DEMAND_BROADCAST_INFO(tBroadcastInfoSort, &tBroadcastInfoData[0]);
	//--------------//
	//--------------//
	//--------------//
		mCheckFirstAttackForTribeSymbol = FALSE;
		mFirstAttackTimeForTribeSymbol = GetTickCount64();
		mTribeDamageForTribeSymbol[0] = 0;
		mTribeDamageForTribeSymbol[1] = 0;
		mTribeDamageForTribeSymbol[2] = 0;
		mTribeDamageForTribeSymbol[3] = 0;
		mDATA.mLifeValue = mMONSTER_INFO->mLife;
		return;
	case 3:
		return;
	case 4:
		if (!mCheckFirstAttackForAllianceStone)
		{
			return;
		}
		if ((GetTickCount64() - mFirstAttackTimeForAllianceStone) < 3600000)
		{
			return;
		}
		switch (mMONSTER_INFO->mSpecialType)
		{
		case 31:
			tAllianceStoneIndex = 0;
			break;
		case 32:
			tAllianceStoneIndex = 1;
			break;
		case 33:
			tAllianceStoneIndex = 2;
			break;
		case 34:
			tAllianceStoneIndex = 3;
			break;
		}
	//--------------//
	//BROADCAST_INFO//
	//--------------//
		tBroadcastInfoSort = 50;
		CopyMemory(&tBroadcastInfoData[0], &tAllianceStoneIndex, 4);
		mCENTER_COM.U_DEMAND_BROADCAST_INFO(tBroadcastInfoSort, &tBroadcastInfoData[0]);
	//--------------//
	//--------------//
	//--------------//
		mCheckFirstAttackForAllianceStone = FALSE;
		mFirstAttackTimeForAllianceStone = GetTickCount64();
		mDATA.mLifeValue = mMONSTER_INFO->mLife;
		return;
	case 5:
		tSelectAvatarIndex = SelectAvatarIndexForGuardAttack();
		if (tSelectAvatarIndex == -1)
		{
			return;
		}
		mDATA.mAction.aSort = 5;
		mDATA.mAction.aFrame = 0.0f;
		mDATA.mAction.aFront = mUTIL.GetYAngle(mDATA.mAction.aLocation[0], mDATA.mAction.aLocation[2],
		                                       mAVATAR_OBJECT[tSelectAvatarIndex].mDATA.aAction.aLocation[0],
		                                       mAVATAR_OBJECT[tSelectAvatarIndex].mDATA.aAction.aLocation[2]);
		mDATA.mAction.aTargetObjectIndex = tSelectAvatarIndex;
		mDATA.mAction.aTargetObjectUniqueNumber = mAVATAR_OBJECT[tSelectAvatarIndex].mUniqueNumber;
	//------------------------//
	//BROADCAST_MONSTER_ACTION//
	//------------------------//
		mTRANSFER.B_MONSTER_ACTION_RECV(tObjectIndex, mUniqueNumber, &mDATA, 1);
		mUTIL.Broadcast(TRUE, &mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS2, false, 0);
	//------------------------//
	//------------------------//
	//------------------------//
		return;
	case 6:
		if (SelectAvatarIndexForIdleAIWIthThrowCar() != -1)
		{
			tDirection[0] = mUTIL.RandomNumber(-100.0f, 100.0f);
			tDirection[2] = mUTIL.RandomNumber(-100.0f, 100.0f);
			tLength = sqrtf(tDirection[0] * tDirection[0] + tDirection[2] * tDirection[2]);
			if (tLength > 0.0f)
			{
				tDirection[0] /= tLength;
				tDirection[2] /= tLength;
			}
			tRandom = rand();
			tRadius = static_cast<float>(50 + (tRandom % 51));
			tLocation[0] = mDATA.mAction.aLocation[0] + tDirection[0] * tRadius;
			tLocation[1] = mDATA.mAction.aLocation[1];
			tLocation[2] = mDATA.mAction.aLocation[2] + tDirection[2] * tRadius;
			mWORLD.Path(&mDATA.mAction.aLocation[0], &tLocation[0], static_cast<float>(mMONSTER_INFO->mWalkSpeed),
			            0.033f, &tPossibleLocation[0]);
			if (mUTIL.GetDoubleXYZ(&tPossibleLocation[0], &mDATA.mAction.aLocation[0]) < (50.0f * 50.0f))
			{
				return;
			}
			mDATA.mAction.aSort = 3;
			mDATA.mAction.aFrame = 0.0f;
			mDATA.mAction.aTargetLocation[0] = tPossibleLocation[0];
			mDATA.mAction.aTargetLocation[1] = tPossibleLocation[1];
			mDATA.mAction.aTargetLocation[2] = tPossibleLocation[2];
			mDATA.mAction.aFront = mUTIL.GetYAngle(mDATA.mAction.aLocation[0], mDATA.mAction.aLocation[2],
			                                       tPossibleLocation[0], tPossibleLocation[2]);
			//------------------------//
			//BROADCAST_MONSTER_ACTION//
			//------------------------//
			mTRANSFER.B_MONSTER_ACTION_RECV(tObjectIndex, mUniqueNumber, &mDATA, 1);
			mUTIL.Broadcast(TRUE, &mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS2, false, 0);
			//------------------------//
			//------------------------//
			//------------------------//
			return;
		}
		tSelectAvatarIndex = SelectAvatarIndexForThrowCarAttack();
		if (tSelectAvatarIndex == -1)
		{
			return;
		}
		mDATA.mAction.aSort = 5;
		mDATA.mAction.aFrame = 0.0f;
		mDATA.mAction.aTargetLocation[0] = mAVATAR_OBJECT[tSelectAvatarIndex].mDATA.aAction.aLocation[0];
		mDATA.mAction.aTargetLocation[1] = mAVATAR_OBJECT[tSelectAvatarIndex].mDATA.aAction.aLocation[1];
		mDATA.mAction.aTargetLocation[2] = mAVATAR_OBJECT[tSelectAvatarIndex].mDATA.aAction.aLocation[2];
		mDATA.mAction.aFront = mUTIL.GetYAngle(mDATA.mAction.aLocation[0], mDATA.mAction.aLocation[2],
		                                       mAVATAR_OBJECT[tSelectAvatarIndex].mDATA.aAction.aLocation[0],
		                                       mAVATAR_OBJECT[tSelectAvatarIndex].mDATA.aAction.aLocation[2]);
		mDATA.mAction.aTargetObjectIndex = tSelectAvatarIndex;
		mDATA.mAction.aTargetObjectUniqueNumber = mAVATAR_OBJECT[tSelectAvatarIndex].mUniqueNumber;
	//------------------------//
	//BROADCAST_MONSTER_ACTION//
	//------------------------//
		mTRANSFER.B_MONSTER_ACTION_RECV(tObjectIndex, mUniqueNumber, &mDATA, 1);
		mUTIL.Broadcast(TRUE, &mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS2, false, 0);
	//------------------------//
	//------------------------//
	//------------------------//
		return;
	default:
		return;
	}
	if (!mCheckAttackState)
	{
		tSelectAvatarIndex = SelectAvatarIndexForPossibleAttack();
		if (tSelectAvatarIndex != -1)
		{
			SetAttackInfoWithAvatar(tSelectAvatarIndex, mAVATAR_OBJECT[tSelectAvatarIndex].mUniqueNumber,
			                        &tWhoAttackMonster);
		}
		else
		{
			if (SelectAvatarIndexForIdleAI() == -1)
			{
				return;
			}
			if ((GetTickCount64() - mCheckFirstLocationTime) > 60000)
			{
				mCheckFirstLocationTime = GetTickCount64();
				mWORLD.Path(&mDATA.mAction.aLocation[0], &mFirstLocation[0],
				            static_cast<float>(mMONSTER_INFO->mWalkSpeed), 0.033f, &tPossibleLocation[0]);
				if (mUTIL.GetDoubleXYZ(&tPossibleLocation[0], &mFirstLocation[0]) > (1.0f * 1.0f))
				{
					mDATA.mAction.aSort = 19;
					mDATA.mAction.aFrame = 0.0f;
					//------------------------//
					//BROADCAST_MONSTER_ACTION//
					//------------------------//
					mTRANSFER.B_MONSTER_ACTION_RECV(tObjectIndex, mUniqueNumber, &mDATA, 1);
					mUTIL.Broadcast(TRUE, &mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS1, false, 0);
					//------------------------//
					//------------------------//
					//------------------------//
					return;
				}
			}
			tDirection[0] = mUTIL.RandomNumber(-100.0f, 100.0f);
			tDirection[2] = mUTIL.RandomNumber(-100.0f, 100.0f);
			tLength = sqrtf(tDirection[0] * tDirection[0] + tDirection[2] * tDirection[2]);
			if (tLength > 0.0f)
			{
				tDirection[0] /= tLength;
				tDirection[2] /= tLength;
			}
			tRandom = rand();
			tRadius = static_cast<float>(50 + (tRandom % 51));
			tLocation[0] = mDATA.mAction.aLocation[0] + tDirection[0] * tRadius;
			tLocation[1] = mDATA.mAction.aLocation[1];
			tLocation[2] = mDATA.mAction.aLocation[2] + tDirection[2] * tRadius;
			mWORLD.Path(&mDATA.mAction.aLocation[0], &tLocation[0], static_cast<float>(mMONSTER_INFO->mWalkSpeed),
			            0.033f, &tPossibleLocation[0]);
			if (mUTIL.GetDoubleXYZ(&tPossibleLocation[0], &mDATA.mAction.aLocation[0]) < (50.0f * 50.0f))
			{
				return;
			}
			mDATA.mAction.aSort = 3;
			mDATA.mAction.aFrame = 0.0f;
			mDATA.mAction.aTargetLocation[0] = tPossibleLocation[0];
			mDATA.mAction.aTargetLocation[1] = tPossibleLocation[1];
			mDATA.mAction.aTargetLocation[2] = tPossibleLocation[2];
			mDATA.mAction.aFront = mUTIL.GetYAngle(mDATA.mAction.aLocation[0], mDATA.mAction.aLocation[2],
			                                       tPossibleLocation[0], tPossibleLocation[2]);
			//------------------------//
			//BROADCAST_MONSTER_ACTION//
			//------------------------//
			mTRANSFER.B_MONSTER_ACTION_RECV(tObjectIndex, mUniqueNumber, &mDATA, 1);
			mUTIL.Broadcast(TRUE, &mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS1, false, 0);
			//------------------------//
			//------------------------//
			//------------------------//
			return;
		}
	}
	AdjustValidAttackTarget();
	if (!mCheckAttackState)
	{
		return;
	}
	tSelectTargetIndex = -1;
	for (index01 = 0; index01 < mTotalAttackNum; index01++)
	{
		if (mAttackLength[index01] > static_cast<float>(mMONSTER_INFO->mRadiusInfo[0] * mMONSTER_INFO->mRadiusInfo[0]))
		{
			continue;
		}
		tRandom = rand();
		if ((tRandom % 2) == 0)
		{
			tSelectTargetIndex = index01;
			break;
		}
	}
	if (tSelectTargetIndex == -1)
	{
		tRandom = rand();
		tSelectTargetIndex = (tRandom % mTotalAttackNum);
	}
	if (mAttackLength[tSelectTargetIndex] <= static_cast<float>(mMONSTER_INFO->mRadiusInfo[0] * mMONSTER_INFO->
		mRadiusInfo[0]))
	{
		mDATA.mAction.aSort = 5;
		mDATA.mAction.aFrame = 0.0f;
		mDATA.mAction.aFront = mUTIL.GetYAngle(mDATA.mAction.aLocation[0], mDATA.mAction.aLocation[2],
		                                       mAVATAR_OBJECT[mAttackIndex[tSelectTargetIndex]].mDATA.aAction.aLocation[
			                                       0], mAVATAR_OBJECT[mAttackIndex[tSelectTargetIndex]].mDATA.aAction.
		                                       aLocation[2]);
		mDATA.mAction.aTargetObjectIndex = mAttackIndex[tSelectTargetIndex];
		mDATA.mAction.aTargetObjectUniqueNumber = mAttackUniqueNumber[tSelectTargetIndex];
		if ((mMONSTER_INFO->mAttackType == 1) || (mMONSTER_INFO->mAttackType == 2))
		{
			mCheckAttackPacketRecv = TRUE;
			mAttackPacketRecvTime = GetTickCount64();
		}
		//------------------------//
		//BROADCAST_MONSTER_ACTION//
		//------------------------//
		mTRANSFER.B_MONSTER_ACTION_RECV(tObjectIndex, mUniqueNumber, &mDATA, 1);
		mUTIL.Broadcast(TRUE, &mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS1, false, 0);
		//------------------------//
		//------------------------//
		//------------------------//
		return;
	}
	if ((mMONSTER_INFO->mRunSpeed < 1) || (mMONSTER_INFO->mRadiusInfo[0] < 1))
	{
		return;
	}
	tAttackLength = sqrtf(mAttackLength[tSelectTargetIndex]);
	tRandom = rand();
	if ((tRandom % 2) == 0)
	{
		tRandom = rand();
		tAttackRadius = static_cast<float>(tRandom % mMONSTER_INFO->mRadiusInfo[0]);
	}
	else
	{
		tRandom = rand();
		tAttackRadius = static_cast<float>(tRandom % mMONSTER_INFO->mRadiusInfo[0]);
		tAttackRadius = -1.0f * tAttackRadius;
	}
	tAttackDirect = sqrtf((tAttackLength * tAttackLength) - (tAttackRadius * tAttackRadius));
	tSinValue = tAttackRadius / tAttackLength;
	tCosValue = tAttackDirect / tAttackLength;
	tLocation[0] = mAVATAR_OBJECT[mAttackIndex[tSelectTargetIndex]].mDATA.aAction.aLocation[0] - mDATA.mAction.aLocation
		[0];
	tLocation[2] = mAVATAR_OBJECT[mAttackIndex[tSelectTargetIndex]].mDATA.aAction.aLocation[2] - mDATA.mAction.aLocation
		[2];
	tDirection[0] = tLocation[0] * tCosValue - tLocation[2] * tSinValue;
	tDirection[2] = tLocation[0] * tSinValue + tLocation[2] * tCosValue;
	tLength = sqrtf((tDirection[0] * tDirection[0]) + (tDirection[2] * tDirection[2]));
	if (tLength > 0.0f)
	{
		tDirection[0] /= tLength;
		tDirection[2] /= tLength;
	}
	tLocation[0] = mDATA.mAction.aLocation[0] + tDirection[0] * tAttackLength;
	tLocation[1] = mDATA.mAction.aLocation[1];
	tLocation[2] = mDATA.mAction.aLocation[2] + tDirection[2] * tAttackLength;
	if (!mWORLD.PathForMonsterAttack(&mDATA.mAction.aLocation[0], &tLocation[0],
	                                 static_cast<float>(mMONSTER_INFO->mRunSpeed), 0.033f,
	                                 &mAVATAR_OBJECT[mAttackIndex[tSelectTargetIndex]].mDATA.aAction.aLocation[0],
	                                 static_cast<float>(mMONSTER_INFO->mRadiusInfo[0]), &tPossibleLocation[0]))
	{
		mDATA.mAction.aSort = 19;
		mDATA.mAction.aFrame = 0.0f;
		//------------------------//
		//BROADCAST_MONSTER_ACTION//
		//------------------------//
		mTRANSFER.B_MONSTER_ACTION_RECV(tObjectIndex, mUniqueNumber, &mDATA, 1);
		mUTIL.Broadcast(TRUE, &mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS1, false, 0);
		//------------------------//
		//------------------------//
		//------------------------//
		return;
	}
	mDATA.mAction.aSort = 4;
	mDATA.mAction.aFrame = 0.0f;
	mDATA.mAction.aTargetLocation[0] = tPossibleLocation[0];
	mDATA.mAction.aTargetLocation[1] = tPossibleLocation[1];
	mDATA.mAction.aTargetLocation[2] = tPossibleLocation[2];
	mDATA.mAction.aFront = mUTIL.GetYAngle(mDATA.mAction.aLocation[0], mDATA.mAction.aLocation[2], tPossibleLocation[0],
	                                       tPossibleLocation[2]);
	mDATA.mAction.aTargetObjectIndex = mAttackIndex[tSelectTargetIndex];
	mDATA.mAction.aTargetObjectUniqueNumber = mAttackUniqueNumber[tSelectTargetIndex];
	//------------------------//
	//BROADCAST_MONSTER_ACTION//
	//------------------------//
	mTRANSFER.B_MONSTER_ACTION_RECV(tObjectIndex, mUniqueNumber, &mDATA, 1);
	mUTIL.Broadcast(TRUE, &mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS1, false, 0);
	//------------------------//
	//------------------------//
	//------------------------//
}

//A002_FOR_ZONE_175_TYPE_BOSS[아이들]
void MONSTER_OBJECT::A002_FOR_ZONE_175_TYPE_BOSS(int tObjectIndex, float tPostTime)
{
	int index01;
	int tSelectTargetIndex;
	float tDirection[3];
	float tLength;
	float tLocation[3];
	float tPossibleLocation[3];
	unsigned int tRandom = 0;

	SelectAvatarIndexForPossibleAttackForZone175TypeBoss();
	if (!mCheckAttackState)
	{
		return;
	}
	AdjustValidAttackTargetForZone175TypeBoss();
	if (!mCheckAttackState)
	{
		return;
	}
	tRandom = rand();
	if ((tRandom % 3) == 0)
	{
		tSelectTargetIndex = -1;
		for (index01 = 0; index01 < mTotalAttackNum; index01++)
		{
			if (mAttackLength[index01] < static_cast<float>(mMONSTER_INFO->mRadiusInfo[0] * mMONSTER_INFO->mRadiusInfo[
				0]))
			{
				continue;
			}
			tRandom = rand();
			if ((tRandom % 2) == 0)
			{
				tSelectTargetIndex = index01;
				break;
			}
		}
		if (tSelectTargetIndex == -1)
		{
			return;
		}
		mDATA.mAction.aSort = 7;
		mDATA.mAction.aFrame = 0.0f;
		mDATA.mAction.aTargetLocation[0] = mAVATAR_OBJECT[mAttackIndex[tSelectTargetIndex]].mDATA.aAction.aLocation[0];
		mDATA.mAction.aTargetLocation[1] = mAVATAR_OBJECT[mAttackIndex[tSelectTargetIndex]].mDATA.aAction.aLocation[1];
		mDATA.mAction.aTargetLocation[2] = mAVATAR_OBJECT[mAttackIndex[tSelectTargetIndex]].mDATA.aAction.aLocation[2];
		mDATA.mAction.aFront = mUTIL.GetYAngle(mDATA.mAction.aLocation[0], mDATA.mAction.aLocation[2],
		                                       mAVATAR_OBJECT[mAttackIndex[tSelectTargetIndex]].mDATA.aAction.aLocation[
			                                       0], mAVATAR_OBJECT[mAttackIndex[tSelectTargetIndex]].mDATA.aAction.
		                                       aLocation[2]);
		mDATA.mAction.aTargetObjectIndex = mAttackIndex[tSelectTargetIndex];
		mDATA.mAction.aTargetObjectUniqueNumber = mAttackUniqueNumber[tSelectTargetIndex];
		//------------------------//
		//BROADCAST_MONSTER_ACTION//
		//------------------------//
		mTRANSFER.B_MONSTER_ACTION_RECV(tObjectIndex, mUniqueNumber, &mDATA, 1);
		mUTIL.Broadcast(TRUE, &mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS3, false, 0);
		//------------------------//
		//------------------------//
		//------------------------//
		return;
	}
	tSelectTargetIndex = -1;
	for (index01 = 0; index01 < mTotalAttackNum; index01++)
	{
		if (mAttackLength[index01] > static_cast<float>(mMONSTER_INFO->mRadiusInfo[0] * mMONSTER_INFO->mRadiusInfo[0]))
		{
			continue;
		}
		tRandom = rand();
		if ((tRandom % 2) == 0)
		{
			tSelectTargetIndex = index01;
			break;
		}
	}
	if (tSelectTargetIndex == -1)
	{
		tRandom = rand();
		tSelectTargetIndex = (tRandom % mTotalAttackNum);
	}
	if (mAttackLength[tSelectTargetIndex] <= static_cast<float>(mMONSTER_INFO->mRadiusInfo[0] * mMONSTER_INFO->
		mRadiusInfo[0]))
	{
		mDATA.mAction.aSort = 5;
		mDATA.mAction.aFrame = 0.0f;
		mDATA.mAction.aFront = mUTIL.GetYAngle(mDATA.mAction.aLocation[0], mDATA.mAction.aLocation[2],
		                                       mAVATAR_OBJECT[mAttackIndex[tSelectTargetIndex]].mDATA.aAction.aLocation[
			                                       0], mAVATAR_OBJECT[mAttackIndex[tSelectTargetIndex]].mDATA.aAction.
		                                       aLocation[2]);
		mDATA.mAction.aTargetObjectIndex = mAttackIndex[tSelectTargetIndex];
		mDATA.mAction.aTargetObjectUniqueNumber = mAttackUniqueNumber[tSelectTargetIndex];
		if ((mMONSTER_INFO->mAttackType == 1) || (mMONSTER_INFO->mAttackType == 2))
		{
			mCheckAttackPacketRecv = TRUE;
			mAttackPacketRecvTime = GetTickCount64();
		}
		//------------------------//
		//BROADCAST_MONSTER_ACTION//
		//------------------------//
		mTRANSFER.B_MONSTER_ACTION_RECV(tObjectIndex, mUniqueNumber, &mDATA, 1);
		mUTIL.Broadcast(TRUE, &mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS3, false, 0);
		//------------------------//
		//------------------------//
		//------------------------//
		return;
	}
	tDirection[0] = mAVATAR_OBJECT[mAttackIndex[tSelectTargetIndex]].mDATA.aAction.aLocation[0] - mDATA.mAction.
		aLocation[0];
	tDirection[2] = mAVATAR_OBJECT[mAttackIndex[tSelectTargetIndex]].mDATA.aAction.aLocation[2] - mDATA.mAction.
		aLocation[2];
	tLength = sqrtf((tDirection[0] * tDirection[0]) + (tDirection[2] * tDirection[2]));
	if (tLength > 0.0f)
	{
		tDirection[0] /= tLength;
		tDirection[2] /= tLength;
	}
	tLocation[0] = mDATA.mAction.aLocation[0] + tDirection[0] * 100.0f;
	tLocation[1] = mDATA.mAction.aLocation[1];
	tLocation[2] = mDATA.mAction.aLocation[2] + tDirection[2] * 100.0f;
	mWORLD.Path(&mDATA.mAction.aLocation[0], &tLocation[0], static_cast<float>(mMONSTER_INFO->mRunSpeed), 0.033f,
	            &tPossibleLocation[0]);
	if (mUTIL.GetDoubleXYZ(&tPossibleLocation[0], &mDATA.mAction.aLocation[0]) < (50.0f * 50.0f))
	{
		return;
	}
	mDATA.mAction.aSort = 4;
	mDATA.mAction.aFrame = 0.0f;
	mDATA.mAction.aTargetLocation[0] = tPossibleLocation[0];
	mDATA.mAction.aTargetLocation[1] = tPossibleLocation[1];
	mDATA.mAction.aTargetLocation[2] = tPossibleLocation[2];
	mDATA.mAction.aFront = mUTIL.GetYAngle(mDATA.mAction.aLocation[0], mDATA.mAction.aLocation[2], tPossibleLocation[0],
	                                       tPossibleLocation[2]);
	mDATA.mAction.aTargetObjectIndex = mAttackIndex[tSelectTargetIndex];
	mDATA.mAction.aTargetObjectUniqueNumber = mAttackUniqueNumber[tSelectTargetIndex];
	//------------------------//
	//BROADCAST_MONSTER_ACTION//
	//------------------------//
	mTRANSFER.B_MONSTER_ACTION_RECV(tObjectIndex, mUniqueNumber, &mDATA, 1);
	mUTIL.Broadcast(TRUE, &mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS3, false, 0);
	//------------------------//
	//------------------------//
	//------------------------//
}

//A004[걷기]
void MONSTER_OBJECT::A004(int tObjectIndex, float tPostTime)
{
	//-----------------------------------------------------------------//
	//[01].일반,[02].성석,[03].수호비,[04].동맹석,[05].수호석,[06].석차//
	//-----------------------------------------------------------------//

	int tSelectAvatarIndex;
	int tWhoAttackMonster;
	BOOL tCheckArrival;

	switch (mSpecialSortNumber)
	{
	case 1:
		if (mCheckAttackState)
		{
			mDATA.mAction.aSort = 1;
			mDATA.mAction.aFrame = 0.0f;
			//------------------------//
			//BROADCAST_MONSTER_ACTION//
			//------------------------//
			mTRANSFER.B_MONSTER_ACTION_RECV(tObjectIndex, mUniqueNumber, &mDATA, 1);
			mUTIL.Broadcast(TRUE, &mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS1, false, 0);
			//------------------------//
			//------------------------//
			//------------------------//
			return;
		}
		tSelectAvatarIndex = SelectAvatarIndexForPossibleAttack();
		if (tSelectAvatarIndex != -1)
		{
			SetAttackInfoWithAvatar(tSelectAvatarIndex, mAVATAR_OBJECT[tSelectAvatarIndex].mUniqueNumber,
			                        &tWhoAttackMonster);
			mDATA.mAction.aSort = 1;
			mDATA.mAction.aFrame = 0.0f;
			//------------------------//
			//BROADCAST_MONSTER_ACTION//
			//------------------------//
			mTRANSFER.B_MONSTER_ACTION_RECV(tObjectIndex, mUniqueNumber, &mDATA, 1);
			mUTIL.Broadcast(TRUE, &mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS1, false, 0);
			//------------------------//
			//------------------------//
			//------------------------//
			return;
		}
		break;
	}
	if (!mWORLD.Move(&mDATA.mAction.aLocation[0], &mDATA.mAction.aTargetLocation[0],
	                 static_cast<float>(mMONSTER_INFO->mWalkSpeed), tPostTime, &tCheckArrival))
	{
		mDATA.mAction.aSort = 1;
		mDATA.mAction.aFrame = 0.0f;
		//------------------------//
		//BROADCAST_MONSTER_ACTION//
		//------------------------//
		mTRANSFER.B_MONSTER_ACTION_RECV(tObjectIndex, mUniqueNumber, &mDATA, 1);
		//-----------------------------------------------------------------//
		//[01].일반,[02].성석,[03].수호비,[04].동맹석,[05].수호석,[06].석차//
		//-----------------------------------------------------------------//
		switch (mSpecialSortNumber)
		{
		case 1:
			mUTIL.Broadcast(TRUE, &mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS1, false, 0);
			break;
		case 2:
			mUTIL.Broadcast(TRUE, &mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS1, false, 0);
			break;
		case 3:
			mUTIL.Broadcast(TRUE, &mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS3, false, 0);
			break;
		case 4:
			mUTIL.Broadcast(TRUE, &mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS3, false, 0);
			break;
		case 5:
			mUTIL.Broadcast(TRUE, &mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS2, false, 0);
			break;
		case 6:
			mUTIL.Broadcast(TRUE, &mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS3, false, 0);
			break;
		}
		//------------------------//
		//------------------------//
		//------------------------//
		return;
	}
	if (tCheckArrival)
	{
		mDATA.mAction.aSort = 1;
		mDATA.mAction.aFrame = 0.0f;
	}
}

//A005[달려들기]
void MONSTER_OBJECT::A005(int tObjectIndex, float tPostTime)
{
	int tSelectAvatarIndex;
	BOOL tCheckArrival;

	if ((!mAVATAR_OBJECT[mDATA.mAction.aTargetObjectIndex].mCheckValidState) || (mAVATAR_OBJECT[mDATA.mAction.
		aTargetObjectIndex].mUniqueNumber != mDATA.mAction.aTargetObjectUniqueNumber) || (mAVATAR_OBJECT[mDATA.mAction.
		aTargetObjectIndex].mDATA.aLifeValue < 1))
	{
		mDATA.mAction.aSort = 1;
		mDATA.mAction.aFrame = 0.0f;
		//------------------------//
		//BROADCAST_MONSTER_ACTION//
		//------------------------//
		mTRANSFER.B_MONSTER_ACTION_RECV(tObjectIndex, mUniqueNumber, &mDATA, 1);
		mUTIL.Broadcast(TRUE, &mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS1, false, 0);
		//------------------------//
		//------------------------//
		//------------------------//
		return;
	}
	if (mUTIL.GetDoubleX_Z(&mDATA.mAction.aLocation[0],
	                       &mAVATAR_OBJECT[mDATA.mAction.aTargetObjectIndex].mDATA.aAction.aLocation[0]) > static_cast<
		float>(mMONSTER_INFO->mRadiusInfo[1] * mMONSTER_INFO->mRadiusInfo[1]))
	{
		mDATA.mAction.aSort = 1;
		mDATA.mAction.aFrame = 0.0f;
		//------------------------//
		//BROADCAST_MONSTER_ACTION//
		//------------------------//
		mTRANSFER.B_MONSTER_ACTION_RECV(tObjectIndex, mUniqueNumber, &mDATA, 1);
		mUTIL.Broadcast(TRUE, &mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS1, false, 0);
		//------------------------//
		//------------------------//
		//------------------------//
		return;
	}
	tSelectAvatarIndex = SelectAvatarIndexForAttackAction();
	if (tSelectAvatarIndex != -1)
	{
		mDATA.mAction.aSort = 5;
		mDATA.mAction.aFrame = 0.0f;
		mDATA.mAction.aFront = mUTIL.GetYAngle(mDATA.mAction.aLocation[0], mDATA.mAction.aLocation[2],
		                                       mAVATAR_OBJECT[tSelectAvatarIndex].mDATA.aAction.aLocation[0],
		                                       mAVATAR_OBJECT[tSelectAvatarIndex].mDATA.aAction.aLocation[2]);
		mDATA.mAction.aTargetObjectIndex = mAVATAR_OBJECT[tSelectAvatarIndex].mIndex;
		mDATA.mAction.aTargetObjectUniqueNumber = mAVATAR_OBJECT[tSelectAvatarIndex].mUniqueNumber;
		if ((mMONSTER_INFO->mAttackType == 1) || (mMONSTER_INFO->mAttackType == 2))
		{
			mCheckAttackPacketRecv = TRUE;
			mAttackPacketRecvTime = GetTickCount64();
		}
		//------------------------//
		//BROADCAST_MONSTER_ACTION//
		//------------------------//
		mTRANSFER.B_MONSTER_ACTION_RECV(tObjectIndex, mUniqueNumber, &mDATA, 1);
		mUTIL.Broadcast(TRUE, &mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS1, false, 0);
		//------------------------//
		//------------------------//
		//------------------------//
		return;
	}
	if (!mWORLD.Move(&mDATA.mAction.aLocation[0], &mDATA.mAction.aTargetLocation[0],
	                 static_cast<float>(mMONSTER_INFO->mRunSpeed), tPostTime, &tCheckArrival))
	{
		mDATA.mAction.aSort = 1;
		mDATA.mAction.aFrame = 0.0f;
		//------------------------//
		//BROADCAST_MONSTER_ACTION//
		//------------------------//
		mTRANSFER.B_MONSTER_ACTION_RECV(tObjectIndex, mUniqueNumber, &mDATA, 1);
		mUTIL.Broadcast(TRUE, &mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS1, false, 0);
		//------------------------//
		//------------------------//
		//------------------------//
		return;
	}
	if (mUTIL.GetDoubleX_Z(&mDATA.mAction.aLocation[0],
	                       &mAVATAR_OBJECT[mDATA.mAction.aTargetObjectIndex].mDATA.aAction.aLocation[0]) <= static_cast<
		float>(mMONSTER_INFO->mRadiusInfo[0] * mMONSTER_INFO->mRadiusInfo[0]))
	{
		if (fabsf(
				mAVATAR_OBJECT[mDATA.mAction.aTargetObjectIndex].mDATA.aAction.aLocation[1] - mDATA.mAction.aLocation[
					1]) <=
			static_cast<float>(mMONSTER_INFO->mSize[1]))
		{
			mDATA.mAction.aSort = 5;
			mDATA.mAction.aFrame = 0.0f;
			mDATA.mAction.aFront = mUTIL.GetYAngle(mDATA.mAction.aLocation[0], mDATA.mAction.aLocation[2],
			                                       mAVATAR_OBJECT[mDATA.mAction.aTargetObjectIndex].mDATA.aAction.
			                                       aLocation[0],
			                                       mAVATAR_OBJECT[mDATA.mAction.aTargetObjectIndex].mDATA.aAction.
			                                       aLocation[2]);
			if ((mMONSTER_INFO->mAttackType == 1) || (mMONSTER_INFO->mAttackType == 2))
			{
				mCheckAttackPacketRecv = TRUE;
				mAttackPacketRecvTime = GetTickCount64();
			}
			//------------------------//
			//BROADCAST_MONSTER_ACTION//
			//------------------------//
			mTRANSFER.B_MONSTER_ACTION_RECV(tObjectIndex, mUniqueNumber, &mDATA, 1);
			mUTIL.Broadcast(TRUE, &mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS1, false, 0);
			//------------------------//
			//------------------------//
			//------------------------//
			return;
		}
		mDATA.mAction.aSort = 19;
		mDATA.mAction.aFrame = 0.0f;
		//------------------------//
		//BROADCAST_MONSTER_ACTION//
		//------------------------//
		mTRANSFER.B_MONSTER_ACTION_RECV(tObjectIndex, mUniqueNumber, &mDATA, 1);
		mUTIL.Broadcast(TRUE, &mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS1, false, 0);
		//------------------------//
		//------------------------//
		//------------------------//
		return;
	}
	if (tCheckArrival)
	{
		mDATA.mAction.aSort = 1;
		mDATA.mAction.aFrame = 0.0f;
	}
}

//A005_FOR_ZONE_175_TYPE_BOSS[달려들기]
void MONSTER_OBJECT::A005_FOR_ZONE_175_TYPE_BOSS(int tObjectIndex, float tPostTime)
{
	int tSelectAvatarIndex;
	BOOL tCheckArrival;

	if ((!mAVATAR_OBJECT[mDATA.mAction.aTargetObjectIndex].mCheckValidState) || (mAVATAR_OBJECT[mDATA.mAction.
		aTargetObjectIndex].mUniqueNumber != mDATA.mAction.aTargetObjectUniqueNumber) || (mAVATAR_OBJECT[mDATA.mAction.
		aTargetObjectIndex].mDATA.aLifeValue < 1))
	{
		mDATA.mAction.aSort = 1;
		mDATA.mAction.aFrame = 0.0f;
		//------------------------//
		//BROADCAST_MONSTER_ACTION//
		//------------------------//
		mTRANSFER.B_MONSTER_ACTION_RECV(tObjectIndex, mUniqueNumber, &mDATA, 1);
		mUTIL.Broadcast(TRUE, &mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS3, false, 0);
		//------------------------//
		//------------------------//
		//------------------------//
		return;
	}
	if (mUTIL.GetDoubleX_Z(&mDATA.mAction.aLocation[0],
	                       &mAVATAR_OBJECT[mDATA.mAction.aTargetObjectIndex].mDATA.aAction.aLocation[0]) > static_cast<
		float>(mMONSTER_INFO->mRadiusInfo[1] * mMONSTER_INFO->mRadiusInfo[1]))
	{
		mDATA.mAction.aSort = 1;
		mDATA.mAction.aFrame = 0.0f;
		//------------------------//
		//BROADCAST_MONSTER_ACTION//
		//------------------------//
		mTRANSFER.B_MONSTER_ACTION_RECV(tObjectIndex, mUniqueNumber, &mDATA, 1);
		mUTIL.Broadcast(TRUE, &mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS3, false, 0);
		//------------------------//
		//------------------------//
		//------------------------//
		return;
	}
	tSelectAvatarIndex = SelectAvatarIndexForAttackAction();
	if (tSelectAvatarIndex != -1)
	{
		mDATA.mAction.aSort = 5;
		mDATA.mAction.aFrame = 0.0f;
		mDATA.mAction.aFront = mUTIL.GetYAngle(mDATA.mAction.aLocation[0], mDATA.mAction.aLocation[2],
		                                       mAVATAR_OBJECT[tSelectAvatarIndex].mDATA.aAction.aLocation[0],
		                                       mAVATAR_OBJECT[tSelectAvatarIndex].mDATA.aAction.aLocation[2]);
		mDATA.mAction.aTargetObjectIndex = mAVATAR_OBJECT[tSelectAvatarIndex].mIndex;
		mDATA.mAction.aTargetObjectUniqueNumber = mAVATAR_OBJECT[tSelectAvatarIndex].mUniqueNumber;
		if ((mMONSTER_INFO->mAttackType == 1) || (mMONSTER_INFO->mAttackType == 2))
		{
			mCheckAttackPacketRecv = TRUE;
			mAttackPacketRecvTime = GetTickCount64();
		}
		//------------------------//
		//BROADCAST_MONSTER_ACTION//
		//------------------------//
		mTRANSFER.B_MONSTER_ACTION_RECV(tObjectIndex, mUniqueNumber, &mDATA, 1);
		mUTIL.Broadcast(TRUE, &mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS3, false, 0);
		//------------------------//
		//------------------------//
		//------------------------//
		return;
	}
	if (!mWORLD.Move(&mDATA.mAction.aLocation[0], &mDATA.mAction.aTargetLocation[0],
	                 static_cast<float>(mMONSTER_INFO->mRunSpeed), tPostTime, &tCheckArrival))
	{
		mDATA.mAction.aSort = 1;
		mDATA.mAction.aFrame = 0.0f;
		//------------------------//
		//BROADCAST_MONSTER_ACTION//
		//------------------------//
		mTRANSFER.B_MONSTER_ACTION_RECV(tObjectIndex, mUniqueNumber, &mDATA, 1);
		mUTIL.Broadcast(TRUE, &mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS3, false, 0);
		//------------------------//
		//------------------------//
		//------------------------//
		return;
	}
	if (tCheckArrival)
	{
		mDATA.mAction.aSort = 1;
		mDATA.mAction.aFrame = 0.0f;
	}
}

//A006[공격]
void MONSTER_OBJECT::A006(int tObjectIndex, float tPostTime)
{
	mDATA.mAction.aFrame += (tPostTime * 30.0f);
	if (mDATA.mAction.aFrame < static_cast<float>(mMONSTER_INFO->mFrameInfo[2]))
	{
		return;
	}
	if (mCheckAttackPacketRecv)
	{
		if ((GetTickCount64() - mAttackPacketRecvTime) < 10000)
		{
			return;
		}
		mCheckAttackPacketRecv = FALSE;
		if ((mAVATAR_OBJECT[mDATA.mAction.aTargetObjectIndex].mCheckValidState) && (mAVATAR_OBJECT[mDATA.mAction.
			aTargetObjectIndex].mUniqueNumber == mDATA.mAction.aTargetObjectUniqueNumber))
		{
			//mGAMELOG.GL_643_CHECK_MONSTER_ATTACK_PACKET(mDATA.mAction.aTargetObjectIndex);

#ifndef SOFTWORLD_CCBT
			mUSER[mDATA.mAction.aTargetObjectIndex].Quit(__FILE__, __FUNCTION__, __LINE__);


#endif
		}
	}
	mDATA.mAction.aSort = 1;
	mDATA.mAction.aFrame = 0.0f;
}

//A008[기술]
void MONSTER_OBJECT::A008(int tObjectIndex, float tPostTime)
{
	mDATA.mAction.aFrame += (tPostTime * 30.0f);
	if (mDATA.mAction.aFrame < static_cast<float>(mMONSTER_INFO->mFrameInfo[3]))
	{
		return;
	}
	mDATA.mAction.aSort = 1;
	mDATA.mAction.aFrame = 0.0f;
}

//A009[피격]
void MONSTER_OBJECT::A009(int tObjectIndex, float tPostTime)
{
	mDATA.mAction.aFrame += (tPostTime * 30.0f);
	if (mDATA.mAction.aFrame < static_cast<float>(mMONSTER_INFO->mFrameInfo[1]))
	{
		return;
	}
	mDATA.mAction.aSort = 1;
	mDATA.mAction.aFrame = 0.0f;
}

//A013[죽기]
void MONSTER_OBJECT::A013(int tObjectIndex, float tPostTime)
{
	//-----------------------------------------------------------------//
	//[01].일반,[02].성석,[03].수호비,[04].동맹석,[05].수호석,[06].석차//
	//-----------------------------------------------------------------//

	int tBroadcastInfoSort;
	BYTE tBroadcastInfoData[MAX_BROADCAST_INFO_SIZE];
	int tTribeSymbolIndex;
	int tWinTribeForSymbol;
	int tAllianceIndex;
	int tAllianceTribe1;
	int tAllianceTribe2;
	int tPossibleAllianceDate01;
	int tPossibleAllianceDate02;
	int tAllianceStoneIndex;

	mDATA.mAction.aFrame += (tPostTime * 30.0f);
	if (mDATA.mAction.aFrame < static_cast<float>(mMONSTER_INFO->mFrameInfo[4]))
	{
		return;
	}
	mCheckValidState = FALSE;
	mInvalidTimeForSummon = GetTickCount64();
	switch (mSpecialSortNumber)
	{
	case 2:
		switch (mMONSTER_INFO->mSpecialType)
		{
		case 11:
			tTribeSymbolIndex = 0;
			break;
		case 12:
			tTribeSymbolIndex = 1;
			break;
		case 13:
			tTribeSymbolIndex = 2;
			break;
		case 28:
			tTribeSymbolIndex = 3;
			break;
		case 14:
			tTribeSymbolIndex = 4;
			break;
		case 15:
			return;
		}
		tWinTribeForSymbol = -1;
		if ((mTribeDamageForTribeSymbol[0] >= mTribeDamageForTribeSymbol[1]) && (mTribeDamageForTribeSymbol[0] >=
			mTribeDamageForTribeSymbol[2]) && (mTribeDamageForTribeSymbol[0] >= mTribeDamageForTribeSymbol[3]))
		{
			tWinTribeForSymbol = 0;
		}
		if ((mTribeDamageForTribeSymbol[1] >= mTribeDamageForTribeSymbol[0]) && (mTribeDamageForTribeSymbol[1] >=
			mTribeDamageForTribeSymbol[2]) && (mTribeDamageForTribeSymbol[1] >= mTribeDamageForTribeSymbol[3]))
		{
			tWinTribeForSymbol = 1;
		}
		if ((mTribeDamageForTribeSymbol[2] >= mTribeDamageForTribeSymbol[0]) && (mTribeDamageForTribeSymbol[2] >=
			mTribeDamageForTribeSymbol[1]) && (mTribeDamageForTribeSymbol[2] >= mTribeDamageForTribeSymbol[3]))
		{
			tWinTribeForSymbol = 2;
		}
		if ((mTribeDamageForTribeSymbol[3] >= mTribeDamageForTribeSymbol[0]) && (mTribeDamageForTribeSymbol[3] >=
			mTribeDamageForTribeSymbol[1]) && (mTribeDamageForTribeSymbol[3] >= mTribeDamageForTribeSymbol[2]))
		{
			tWinTribeForSymbol = 3;
		}
		if (tWinTribeForSymbol == -1)
		{
			return;
		}
	//--------------//
	//BROADCAST_INFO//
	//--------------//
		tBroadcastInfoSort = 42;
		CopyMemory(&tBroadcastInfoData[0], &tTribeSymbolIndex, 4);
		CopyMemory(&tBroadcastInfoData[4], &tWinTribeForSymbol, 4);
		mCENTER_COM.U_DEMAND_BROADCAST_INFO(tBroadcastInfoSort, &tBroadcastInfoData[0]);
	//--------------//
	//--------------//
	//--------------//
		switch (tTribeSymbolIndex)
		{
		case 0:
			mGAME.mWorldInfo.mTribe1Symbol = tWinTribeForSymbol;
			break;
		case 1:
			mGAME.mWorldInfo.mTribe2Symbol = tWinTribeForSymbol;
			break;
		case 2:
			mGAME.mWorldInfo.mTribe3Symbol = tWinTribeForSymbol;
			break;
		case 3:
			mGAME.mWorldInfo.mTribe4Symbol = tWinTribeForSymbol;
			break;
		case 4:
			mGAME.mWorldInfo.mMonsterSymbol = tWinTribeForSymbol;
			break;
		}
		return;
	case 4:
		switch (mMONSTER_INFO->mSpecialType)
		{
		case 31:
			tAllianceStoneIndex = 0;
			break;
		case 32:
			tAllianceStoneIndex = 1;
			break;
		case 33:
			tAllianceStoneIndex = 2;
			break;
		case 34:
			tAllianceStoneIndex = 3;
			break;
		}
		if ((mGAME.mWorldInfo.mAllianceState[0][0] == tAllianceStoneIndex) || (mGAME.mWorldInfo.mAllianceState[0][1] ==
			tAllianceStoneIndex))
		{
			tAllianceIndex = 0;
			tAllianceTribe1 = mGAME.mWorldInfo.mAllianceState[0][0];
			tAllianceTribe2 = mGAME.mWorldInfo.mAllianceState[0][1];
		}
		else if ((mGAME.mWorldInfo.mAllianceState[1][0] == tAllianceStoneIndex) || (mGAME.mWorldInfo.mAllianceState[1][
			1] == tAllianceStoneIndex))
		{
			tAllianceIndex = 1;
			tAllianceTribe1 = mGAME.mWorldInfo.mAllianceState[1][0];
			tAllianceTribe2 = mGAME.mWorldInfo.mAllianceState[1][1];
		}
		else
		{
			return;
		}
		tPossibleAllianceDate01 = mUTIL.ReturnAddDate(mUTIL.ReturnNowDate(), 28);
		tPossibleAllianceDate02 = mUTIL.ReturnAddDate(mUTIL.ReturnNowDate(), 28);
	//--------------//
	//BROADCAST_INFO//
	//--------------//
		tBroadcastInfoSort = 49;
		CopyMemory(&tBroadcastInfoData[0], &tAllianceTribe1, 4);
		CopyMemory(&tBroadcastInfoData[4], &tAllianceTribe2, 4);
		CopyMemory(&tBroadcastInfoData[8], &tPossibleAllianceDate01, 4);
		CopyMemory(&tBroadcastInfoData[12], &tPossibleAllianceDate02, 4);
		CopyMemory(&tBroadcastInfoData[16], &tAllianceStoneIndex, 4);
		CopyMemory(&tBroadcastInfoData[20], &mLastAttackUserTribe, 4);
		CopyMemory(&tBroadcastInfoData[24], &mLastAttackUserName[0], MAX_AVATAR_NAME_LENGTH);
		mCENTER_COM.U_DEMAND_BROADCAST_INFO(tBroadcastInfoSort, &tBroadcastInfoData[0]);
	//--------------//
	//--------------//
	//--------------//
		mGAME.mWorldInfo.mPossibleAllianceInfo[tAllianceTribe1][0] = tPossibleAllianceDate01;
		mGAME.mWorldInfo.mPossibleAllianceInfo[tAllianceTribe1][1] = 1;
		mGAME.mWorldInfo.mPossibleAllianceInfo[tAllianceTribe2][0] = tPossibleAllianceDate02;
		mGAME.mWorldInfo.mPossibleAllianceInfo[tAllianceTribe2][1] = 1;
		mGAME.mWorldInfo.mAllianceState[tAllianceIndex][0] = -1;
		mGAME.mWorldInfo.mAllianceState[tAllianceIndex][1] = -1;
	}
}

//A020[귀환]
void MONSTER_OBJECT::A020(int tObjectIndex, float tPostTime)
{
	mDATA.mAction.aFrame += (tPostTime * 30.0f);
	if (mDATA.mAction.aFrame < static_cast<float>(mMONSTER_INFO->mFrameInfo[5]))
	{
		return;
	}
	mDATA.mAction.aSort = 0;
	mDATA.mAction.aFrame = 0.0f;
	mDATA.mAction.aLocation[0] = mFirstLocation[0];
	mDATA.mAction.aLocation[1] = mFirstLocation[1];
	mDATA.mAction.aLocation[2] = mFirstLocation[2];
	//------------------------//
	//BROADCAST_MONSTER_ACTION//
	//------------------------//
	mTRANSFER.B_MONSTER_ACTION_RECV(tObjectIndex, mUniqueNumber, &mDATA, 1);
	mUTIL.Broadcast(TRUE, &mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS1, false, 0);
	//------------------------//
	//------------------------//
	//------------------------//
}

//SET_ATTACK_INFO_WITH_AVATAR
void MONSTER_OBJECT::SetAttackInfoWithAvatar(int tServerIndex, DWORD tUniqueNumber, int* tWhoAttackMonster)
{
	if (!mCheckValidState)
	{
		return;
	}
	int index01;

	if (!mCheckAttackState)
	{
		mCheckAttackState = TRUE;
		mTotalAttackNum = 0;
		mAttackIndex[mTotalAttackNum] = tServerIndex;
		mAttackUniqueNumber[mTotalAttackNum] = tUniqueNumber;
		mAttackDamage[mTotalAttackNum] = 0;
		mTotalAttackNum++;
		*tWhoAttackMonster = mTotalAttackNum - 1;
		return;
	}
	for (index01 = 0; index01 < mTotalAttackNum; index01++)
	{
		if ((mAttackIndex[index01] == tServerIndex) && (mAttackUniqueNumber[index01] = tUniqueNumber))
		{
			break;
		}
	}
	if (index01 < mTotalAttackNum)
	{
		*tWhoAttackMonster = index01;
		return;
	}
	if (mTotalAttackNum == MAX_MONSTER_OBJECT_ATTACK_NUM)
	{
		for (index01 = 1; index01 < mTotalAttackNum; index01++)
		{
			mAttackIndex[(index01 - 1)] = mAttackIndex[index01];
			mAttackUniqueNumber[(index01 - 1)] = mAttackUniqueNumber[index01];
			mAttackDamage[(index01 - 1)] = mAttackDamage[index01];
		}
		mTotalAttackNum--;
	}
	mAttackIndex[mTotalAttackNum] = tServerIndex;
	mAttackUniqueNumber[mTotalAttackNum] = tUniqueNumber;
	mAttackDamage[mTotalAttackNum] = 0;
	mTotalAttackNum++;
	*tWhoAttackMonster = mTotalAttackNum - 1;
}

//SELECT_AVATAR_INDEX_FOR_MAX_ATTACK_DAMAGE
int MONSTER_OBJECT::SelectAvatarIndexForMaxAttackDamage(void)
{
	if (!mCheckValidState)
	{
		return -1;
	}

	int index01;
	int tSelectAvatarIndex;

	if (!mCheckAttackState)
	{
		return -1;
	}
	tSelectAvatarIndex = -1;
	for (index01 = 0; index01 < mTotalAttackNum; index01++)
	{
		if (!mAVATAR_OBJECT[mAttackIndex[index01]].mCheckValidState)
		{
			continue;
		}
		if (mAVATAR_OBJECT[mAttackIndex[index01]].mUniqueNumber != mAttackUniqueNumber[index01])
		{
			continue;
		}
		if (mUSER[mAttackIndex[index01]].mMoveZoneResult == 1)
		{
			continue;
		}
		if (mAVATAR_OBJECT[mAttackIndex[index01]].mDATA.aVisibleState == 0)
		{
			continue;
		}
		if (mAVATAR_OBJECT[mAttackIndex[index01]].mDATA.aLifeValue < 1)
		{
			continue;
		}
		if (tSelectAvatarIndex == -1)
		{
			tSelectAvatarIndex = index01;
		}
		else
		{
			if (mAttackDamage[index01] > mAttackDamage[tSelectAvatarIndex])
			{
				tSelectAvatarIndex = index01;
			}
		}
	}
	if (tSelectAvatarIndex == -1)
	{
		return -1;
	}
	return mAttackIndex[tSelectAvatarIndex];
}

//PROCESS_FOR_DROP_ITEM
void MONSTER_OBJECT::ProcessForDropItem(int tUserIndex)
{
	if (!mCheckValidState)
	{
		return;
	}
	unsigned int tRandom = 0;
	unsigned int tRandom1 = 0;
	unsigned int tRandom2 = 0;

	int index01;
	ITEM_INFO* tITEM_INFO;
	char tMasterName[MAX_AVATAR_NAME_LENGTH];
	int tMasterLuck;
	BOOL tCheckPossibleDrop;
	BOOL tCheckDropEvent;
	int tDropMoneySize;
	int tTribeItemList[10];
	int tItemLLevel;
	int tItemHLevel;
	int tItemType;
	int tItemSort;
	int tSearchItemTryNumForSameTribe;
	int tRandomValue;
	int tItemIndex;
	int tLuckyItemInfo[3];
	int tImproveStoneInfo[4];
	int tAddStone[2];
	int tSkillPieceInfo[3][4];
	int tPatInfo[4][6];
#ifdef __ANIMAL__
	int tAnimalInfo[3];
#endif
	int temp_itemdrop_x2 = 1;
	int temp_ratio = 0;
	BYTE tData[MAX_BROADCAST_DATA_SIZE];


	strncpy(tMasterName, mAVATAR_OBJECT[tUserIndex].mDATA.aName, MAX_AVATAR_NAME_LENGTH);
	tMasterLuck = mAVATAR_OBJECT[tUserIndex].GetLuck() * 10;
	if (mUSER[tUserIndex].mAuthInfo.AuthType == 0)
	{
		if (mMONSTER_INFO->mMartialItemLevel < 1)
		{
			if ((mAVATAR_OBJECT[tUserIndex].mDATA.aLevel1 - mMONSTER_INFO->mItemLevel) > 9)
			{
				tCheckPossibleDrop = FALSE;
			}
			else
			{
				if (mAVATAR_OBJECT[tUserIndex].mDATA.aLevel2 < 1)
				{
					tCheckPossibleDrop = TRUE;
				}
				else
				{
					tCheckPossibleDrop = FALSE;
				}
			}
		}
		else
		{
			if ((mAVATAR_OBJECT[tUserIndex].mDATA.aLevel2 - mMONSTER_INFO->mMartialItemLevel) > 0)
			{
				tCheckPossibleDrop = FALSE;
			}
			else
			{
				tCheckPossibleDrop = TRUE;
			}
		}
	}
	else
	{
		tCheckPossibleDrop = TRUE;
	}
	//----------//
	//DROP_MONEY//
	//----------//
	if (tCheckPossibleDrop)
	{
		tCheckDropEvent = FALSE;
		if ((mMONSTER_INFO->mDropMoneyInfo[0] > 0) && (mUTIL.RandomNumber() <= static_cast<int>(static_cast<float>(
			mMONSTER_INFO->mDropMoneyInfo[0] + tMasterLuck) * mGAME.mItemDropUpRatio)))
		{
			tCheckDropEvent = TRUE;
		}

		if (tCheckDropEvent)
		{
			tRandom = rand();
			tDropMoneySize = mMONSTER_INFO->mDropMoneyInfo[1] + (tRandom % (mMONSTER_INFO->mDropMoneyInfo[2] -
				mMONSTER_INFO->mDropMoneyInfo[1] + 1));
			if (tDropMoneySize > 0)
			{
				mGAME.AddTribeBankInfo(mGAME.ReturnTribeForSymbol(mAVATAR_OBJECT[tUserIndex].mDATA.aTribe),
				                       (tDropMoneySize * 0.10f));
				tDropMoneySize -= tDropMoneySize * 0.10f;
				if (!mUTIL.CheckOverMaximum(mUSER[tUserIndex].mAvatarInfo.aMoney, tDropMoneySize))
				{
					mUSER[tUserIndex].mAvatarInfo.aMoney += tDropMoneySize;
					mTRANSFER.B_AVATAR_CHANGE_INFO_2(19, mUSER[tUserIndex].mAvatarInfo.aMoney);
					mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				}
				//mUTIL.ProcessForDropItem(1, 1, tDropMoneySize, 0, 0, &mDATA.mAction.aLocation[0], &tMasterName[0], mAVATAR_OBJECT[tUserIndex].mDATA.aTribe);
			}
		}
	}
	//-----------//
	//DROP_POTION//
	//-----------//
	if (tCheckPossibleDrop)
	{
		for (index01 = 0; index01 < 5; index01++)
		{
			tCheckDropEvent = FALSE;
			if ((mMONSTER_INFO->mDropPotionInfo[index01][0] > 0) && (mUTIL.RandomNumber() <= static_cast<int>(
				static_cast<float>(mMONSTER_INFO->mDropPotionInfo[index01][0]) + tMasterLuck) * mGAME.mItemDropUpRatio))
			{
				tCheckDropEvent = TRUE;
			}
			if (tCheckDropEvent)
			{
				mUTIL.ProcessForDropItem(1, mMONSTER_INFO->mDropPotionInfo[index01][1], 1, 0, 0,
				                         &mDATA.mAction.aLocation[0], &tMasterName[0], 0);
			}
		}
	}
	//-----------------//
	//DROP_GENERAL_ITEM//
	//-----------------//
	if (tCheckPossibleDrop)
	{
		switch (mAVATAR_OBJECT[tUserIndex].mDATA.aPreviousTribe)
		{
		case 0:
			tTribeItemList[0] = 5;
			tTribeItemList[1] = 7;
			tTribeItemList[2] = 8;
			tTribeItemList[3] = 9;
			tTribeItemList[4] = 10;
			tTribeItemList[5] = 11;
			tTribeItemList[6] = 12;
			tTribeItemList[7] = 13;
			tTribeItemList[8] = 14;
			tTribeItemList[9] = 15;
			break;
		case 1:
			tTribeItemList[0] = 5;
			tTribeItemList[1] = 7;
			tTribeItemList[2] = 8;
			tTribeItemList[3] = 9;
			tTribeItemList[4] = 10;
			tTribeItemList[5] = 11;
			tTribeItemList[6] = 12;
			tTribeItemList[7] = 16;
			tTribeItemList[8] = 17;
			tTribeItemList[9] = 18;
			break;
		case 2:
			tTribeItemList[0] = 5;
			tTribeItemList[1] = 7;
			tTribeItemList[2] = 8;
			tTribeItemList[3] = 9;
			tTribeItemList[4] = 10;
			tTribeItemList[5] = 11;
			tTribeItemList[6] = 12;
			tTribeItemList[7] = 19;
			tTribeItemList[8] = 20;
			tTribeItemList[9] = 21;
			break;
		}
		for (index01 = 0; index01 < 12; index01++)
		{
			tCheckDropEvent = FALSE;
			if (index01 < 3)
			{
				temp_itemdrop_x2 = 1;
				if (mUSER[tUserIndex].mAvatarInfo.aDoubleDropTime > 0)
				{
					temp_itemdrop_x2 = 2;
				}
				//Time Effect
				if ((mUSER[tUserIndex].mAvatarInfo.aTimeEffect == 240) && (mUSER[tUserIndex].mAvatarInfo.aTimeEffect2 ==
					1))
				{
					temp_itemdrop_x2 += static_cast<int>(static_cast<float>(temp_itemdrop_x2) * 0.1f);
				}
				temp_ratio = static_cast<int>(static_cast<float>(mMONSTER_INFO->mDropItemInfo[index01] + tMasterLuck) *
					((mUSER[tUserIndex].mItemDropUpRatio *
						temp_itemdrop_x2) + mGAME.mItemDropUpRatio));
				if ((mMONSTER_INFO->mDropItemInfo[index01] > 0) && (mUTIL.RandomNumber() <= temp_ratio))
				{
					tCheckDropEvent = TRUE;
					if (mMONSTER_INFO->mMartialItemLevel < 1)
					{
						tItemLLevel = mMONSTER_INFO->mItemLevel;
						tItemHLevel = mMONSTER_INFO->mItemLevel + 5;
						if (tItemHLevel > MAX_LIMIT_LEVEL_NUM)
						{
							tItemHLevel = MAX_LIMIT_LEVEL_NUM;
						}
					}
					else
					{
						tItemLLevel = mMONSTER_INFO->mItemLevel + mMONSTER_INFO->mMartialItemLevel;
						tItemHLevel = mMONSTER_INFO->mItemLevel + mMONSTER_INFO->mMartialItemLevel;
					}
					tItemType = 1;
					tRandom = rand();
					tItemSort = tTribeItemList[(tRandom % 10)];
				}
			}
			else if (index01 < 6)
			{
				temp_itemdrop_x2 = 1;
				if (mUSER[tUserIndex].mAvatarInfo.aDoubleDropTime > 0)
				{
					temp_itemdrop_x2 = 2;
				}
				//Time Effect
				if ((mUSER[tUserIndex].mAvatarInfo.aTimeEffect == 240) && (mUSER[tUserIndex].mAvatarInfo.aTimeEffect2 ==
					1))
				{
					temp_itemdrop_x2 += static_cast<int>(static_cast<float>(temp_itemdrop_x2) * 0.1f);
				}
				temp_ratio = static_cast<int>(static_cast<float>(mMONSTER_INFO->mDropItemInfo[index01] + tMasterLuck) *
					((mUSER[tUserIndex].mItemDropUpRatio *
						temp_itemdrop_x2) + mGAME.mItemDropUpRatio));
				if ((mMONSTER_INFO->mDropItemInfo[index01] > 0) && (mUTIL.RandomNumber() <= temp_ratio))
				{
					tCheckDropEvent = TRUE;
					if (mMONSTER_INFO->mMartialItemLevel < 1)
					{
						tItemLLevel = mMONSTER_INFO->mItemLevel;
						tItemHLevel = mMONSTER_INFO->mItemLevel + 5;
						if (tItemHLevel > MAX_LIMIT_LEVEL_NUM)
						{
							tItemHLevel = MAX_LIMIT_LEVEL_NUM;
						}
					}
					else
					{
						tItemLLevel = mMONSTER_INFO->mItemLevel + mMONSTER_INFO->mMartialItemLevel;
						tItemHLevel = mMONSTER_INFO->mItemLevel + mMONSTER_INFO->mMartialItemLevel;
					}
					tItemType = 2;
					tRandom = rand();
					tItemSort = tTribeItemList[(tRandom % 10)];
				}
			}
			else if (index01 < 9)
			{
				temp_itemdrop_x2 = 1;
				if (mUSER[tUserIndex].mAvatarInfo.aDoubleDropTime > 0)
				{
					temp_itemdrop_x2 = 2;
				}
				//Time Effect
				if ((mUSER[tUserIndex].mAvatarInfo.aTimeEffect == 240) && (mUSER[tUserIndex].mAvatarInfo.aTimeEffect2 ==
					1))
				{
					temp_itemdrop_x2 += static_cast<int>(static_cast<float>(temp_itemdrop_x2) * 0.1f);
				}
				temp_ratio = static_cast<int>(static_cast<float>(mMONSTER_INFO->mDropItemInfo[index01]) * ((mUSER[
						tUserIndex].mItemDropUpRatio * temp_itemdrop_x2)
					+ mGAME.mItemDropUpRatio));

				if ((mMONSTER_INFO->mDropItemInfo[index01] > 0) && (mUTIL.RandomNumber() <= temp_ratio))
				{
					tCheckDropEvent = TRUE;
					if (mMONSTER_INFO->mMartialItemLevel < 1)
					{
						tItemLLevel = mMONSTER_INFO->mItemLevel;
						tItemHLevel = mMONSTER_INFO->mItemLevel + 5;
						if (tItemHLevel > MAX_LIMIT_LEVEL_NUM)
						{
							tItemHLevel = MAX_LIMIT_LEVEL_NUM;
						}
					}
					else
					{
						tItemLLevel = mMONSTER_INFO->mItemLevel + mMONSTER_INFO->mMartialItemLevel;
						tItemHLevel = mMONSTER_INFO->mItemLevel + mMONSTER_INFO->mMartialItemLevel;
					}
					tItemType = 3;
					tRandom = rand();
					tItemSort = tTribeItemList[(tRandom % 10)];
				}
			}
			else
			{
				if ((mMONSTER_INFO->mDropItemInfo[index01] > 0) && (mUTIL.RandomNumber() <= mMONSTER_INFO->mDropItemInfo
					[index01] + mGAME.mItemDropUpRatio))
				{
					tCheckDropEvent = TRUE;
					if (mMONSTER_INFO->mMartialItemLevel < 1)
					{
						tItemLLevel = mMONSTER_INFO->mItemLevel;
						tItemHLevel = mMONSTER_INFO->mItemLevel + 5;
						if (tItemHLevel > MAX_LIMIT_LEVEL_NUM)
						{
							tItemHLevel = MAX_LIMIT_LEVEL_NUM;
						}
					}
					else
					{
						tItemLLevel = mMONSTER_INFO->mItemLevel + mMONSTER_INFO->mMartialItemLevel;
						tItemHLevel = mMONSTER_INFO->mItemLevel + mMONSTER_INFO->mMartialItemLevel;
					}
					if (mSERVER_INFO.m_iDeploymentStage > 0)
					{
						tItemType = 4;
					}
					else
					{
						tItemType = 3;
					}
					tRandom = rand();
					tItemSort = tTribeItemList[(tRandom % 10)];
				}
			}
			if (tCheckDropEvent)
			{
				tSearchItemTryNumForSameTribe = 0;
				while (tSearchItemTryNumForSameTribe < 10)
				{
					tRandom = rand();
					tITEM_INFO = mITEM.Return((tItemLLevel + (tRandom % (tItemHLevel - tItemLLevel + 1))), tItemType,
					                          tItemSort);
					if (tITEM_INFO)
					{
						if (((tITEM_INFO->iEquipInfo[0] == 1) || (tITEM_INFO->iEquipInfo[0] == (mAVATAR_OBJECT[
							tUserIndex].mDATA.aPreviousTribe + 2))) && (tITEM_INFO->iCheckMonsterDrop == 2))
						{
							break;
						}
						tITEM_INFO = nullptr;
						tSearchItemTryNumForSameTribe++;
					}
					else
					{
						tSearchItemTryNumForSameTribe++;
					}
				}
				if (tITEM_INFO)
				{
					mUTIL.ProcessForDropItem(1, tITEM_INFO->iIndex, 0, 0, mUTIL.ReturnItemRecognitionNumber(tItemType),
					                         &mDATA.mAction.aLocation[0], &tMasterName[0], 0);
				}
			}
		}
	}
	//------------------//
	//-WARLORD_IOS_BOSS-//
	//------------------//
	if (tCheckPossibleDrop)
	{
		if ((mMONSTER_INFO->mIndex == 722) || (mMONSTER_INFO->mIndex == 723) || (mMONSTER_INFO->mIndex == 724))
		{
			tCheckDropEvent = FALSE;
			tRandom = rand();

			if ((tRandom % 100) < 70)
			{
				tCheckDropEvent = TRUE;
				tRandom = (rand() % 129) + 87000;
				tITEM_INFO = mITEM.Search(tRandom);
			}

			if (tCheckDropEvent)
			{
				if (tITEM_INFO)
				{
					mUTIL.ProcessForDropItem(1, tITEM_INFO->iIndex, 0, 0, mUTIL.ReturnItemRecognitionNumber(tItemType),
					                         &mDATA.mAction.aLocation[0], &tMasterName[0], 0);
					sprintf((char*)&tData[0], "(IOS) %s gained %s", &mUSER[tUserIndex].mAvatarInfo.aName[0],
					        tITEM_INFO->iName);
					mRELAY_COM.U_DEMAND_BROADCAST_DATA(102, &tData[0]);
				}
			}
		}
	}
	//------------------//
	//---LOD-----BOSS---//
	//------------------//
	if (tCheckPossibleDrop)
	{
		if ((mMONSTER_INFO->mIndex == 725) || (mMONSTER_INFO->mIndex == 726) || (mMONSTER_INFO->mIndex == 727) || (
			mMONSTER_INFO->mIndex == 728) || (mMONSTER_INFO->mIndex == 729) || (mMONSTER_INFO->mIndex == 730))
		{
			tCheckDropEvent = FALSE;
			tRandom = rand() % 100;
			if (tRandom <= 10)
			{
				tCheckDropEvent = TRUE;
				tRandom = rand() % 2;
				switch (tRandom)
				{
				case 0:
					tITEM_INFO = mITEM.Search(1378);
					break;
				case 1:
					tITEM_INFO = mITEM.Search(1379);
					break;
				}

				if (tCheckDropEvent)
				{
					if (tITEM_INFO)
					{
						mUTIL.ProcessForDropItem(1, tITEM_INFO->iIndex, 0, 0,
						                         mUTIL.ReturnItemRecognitionNumber(tItemType),
						                         &mDATA.mAction.aLocation[0], &tMasterName[0], 0);
						mUTIL.ProcessForDropItem(1, 1444, 0, 0, mUTIL.ReturnItemRecognitionNumber(tItemType),
						                         &mDATA.mAction.aLocation[0], &tMasterName[0], 0);
						sprintf((char*)&tData[0], "(LOD) %s gained %s", &mUSER[tUserIndex].mAvatarInfo.aName[0],
						        tITEM_INFO->iName);
						mRELAY_COM.U_DEMAND_BROADCAST_DATA(102, &tData[0]);
					}
				}
			}
		}
	}
	//-------------------//
	//---DROP_V2_STONE---//
	//-------------------//
	if (tCheckPossibleDrop)
	{
		if ((mMONSTER_INFO->mIndex == 725) || (mMONSTER_INFO->mIndex == 726) || (mMONSTER_INFO->mIndex == 727) || (
			mMONSTER_INFO->mIndex == 728) || (mMONSTER_INFO->mIndex == 729) || (mMONSTER_INFO->mIndex == 730))
		{
			tCheckDropEvent = FALSE;
			tRandom = rand() % 300;
			if (tRandom <= 50)
			{
				tCheckDropEvent = TRUE;
				tRandom = rand() % 4;
				switch (tRandom)
				{
				case 0:
					tITEM_INFO = mITEM.Search(501);
					break;
				case 1:
					tITEM_INFO = mITEM.Search(502);
					break;
				case 2:
					tITEM_INFO = mITEM.Search(503);
					break;
				case 3:
					tITEM_INFO = mITEM.Search(504);
					break;
				}

				if (tCheckDropEvent)
				{
					if (tITEM_INFO)
					{
						mUTIL.ProcessForDropItem(1, tITEM_INFO->iIndex, 0, 0,
						                         mUTIL.ReturnItemRecognitionNumber(tItemType),
						                         &mDATA.mAction.aLocation[0], &tMasterName[0], 0);
					}
				}
			}
		}
	}
	/* Drop Key Item */
	if (tCheckPossibleDrop)
	{
		tCheckDropEvent = FALSE;
		if (mUTIL.RandomNumber() <= 250)
		{
			tCheckDropEvent = TRUE;
			tRandom = rand() % 2;
			switch (tRandom)
			{
			case 0:
				tItemIndex = 1049;
				break;
			case 1:
				tItemIndex = 1048;
				break;
			}
		}

		if (tCheckDropEvent)
		{
			mUTIL.ProcessForDropItem(1, tItemIndex, 0, 0, mUTIL.ReturnItemRecognitionNumber(0),
			                         &mDATA.mAction.aLocation[0], &tMasterName[0], 0);
		}
	}
#if 0
	//---------------//
	//DROP_QUEST_ITEM//
	//---------------//
	tCheckDropEvent = FALSE;
	if ((mMONSTER_INFO->mDropQuestItemInfo[0] > 0) && (mUTIL.RandomNumber() <= mMONSTER_INFO->mDropQuestItemInfo[0]))
	{
		tCheckDropEvent = TRUE;
	}
	if (mUSER[tUserIndex].mHealthState == 4)
	{
		if (tCheckDropEvent)
		{
			tRandom = rand();
			if ((tRandom % 2) == 0)
			{
				tCheckDropEvent = FALSE;
			}
		}
	}
	else if (mUSER[tUserIndex].mHealthState == 5) {
		tCheckDropEvent = false;
	}
	if (tCheckDropEvent)
	{
		//-----//
		//QUEST//
		//-----//
		if ((mUSER[tUserIndex].mAvatarInfo.aQuestInfo[2] == 2) && (mUSER[tUserIndex].mAvatarInfo.aQuestInfo[3] == mMONSTER_INFO->mDropQuestItemInfo[1]) && (mAVATAR_OBJECT[tUserIndex].ReturnQuestPresentState() == 2))
		{
			mUTIL.ProcessForDropItem(1, mMONSTER_INFO->mDropQuestItemInfo[1], 0, 0, 0, &mDATA.mAction.aLocation[0], &tMasterName[0], 0);
		}
		//-----//
		//-----//
		//-----//
	}
#endif
	//---------------//
	//DROP_EXTRA_ITEM//
	//---------------//
	if (tCheckPossibleDrop)
	{
		for (index01 = 0; index01 < 50; index01++)
		{
			tCheckDropEvent = FALSE;
			if ((mMONSTER_INFO->mDropExtraItemInfo[index01][0] > 0) && (mUTIL.RandomNumber() <= mMONSTER_INFO->
				mDropExtraItemInfo[index01][0]))
			{
				tCheckDropEvent = TRUE;
			}
			if (tCheckDropEvent)
			{
				mUTIL.ProcessForDropItem(1, mMONSTER_INFO->mDropExtraItemInfo[index01][1], 0, 0, 0,
				                         &mDATA.mAction.aLocation[0], &tMasterName[0], 0);
			}
		}
	}
	//---------------//
	//DROP_LUCKY_ITEM//
	//---------------//
	if (tCheckPossibleDrop)
	{
		tLuckyItemInfo[0] = 1035;
		tLuckyItemInfo[1] = 1036;
		tLuckyItemInfo[2] = 1037;
		tCheckDropEvent = FALSE;
		if (mUTIL.RandomNumber() <= 100)
		{
			tCheckDropEvent = TRUE;
			tRandomValue = rand() % 100;
			if (tRandomValue < 50)
			{
				tItemIndex = tLuckyItemInfo[0];
			}
			else if (tRandomValue < 80)
			{
				tItemIndex = tLuckyItemInfo[1];
			}
			else
			{
				tItemIndex = tLuckyItemInfo[2];
			}
		}
		if (tCheckDropEvent)
		{
			mUTIL.ProcessForDropItem(1, tItemIndex, 0, 0, mUTIL.ReturnItemRecognitionNumber(0),
			                         &mDATA.mAction.aLocation[0], &tMasterName[0], 0);
		}
	}
	//------------------//
	//DROP_IMPROVE_STONE//
	//------------------//
	if (tCheckPossibleDrop)
	{
		tImproveStoneInfo[0] = 1019;
		tImproveStoneInfo[1] = 1020;
		tImproveStoneInfo[2] = 1021;
		tImproveStoneInfo[3] = 1022;
		tCheckDropEvent = FALSE;
		if (mUTIL.RandomNumber() <= 150)
		{
			tCheckDropEvent = TRUE;
			tRandomValue = rand() % 100;
			if (tRandomValue < 40)
			{
				tItemIndex = tImproveStoneInfo[0];
			}
			else if (tRandomValue < 70)
			{
				tItemIndex = tImproveStoneInfo[1];
			}
			else if (tRandomValue < 90)
			{
				tItemIndex = tImproveStoneInfo[2];
			}
			else
			{
				tItemIndex = tImproveStoneInfo[3];
			}
		}
		if (tCheckDropEvent)
		{
			mUTIL.ProcessForDropItem(1, tItemIndex, 0, 0, mUTIL.ReturnItemRecognitionNumber(0),
			                         &mDATA.mAction.aLocation[0], &tMasterName[0], 0);
		}
	}
	//--------------//
	//DROP_ADD_STONE//
	//--------------//
	if (tCheckPossibleDrop)
	{
		tAddStone[0] = 1024;
		tAddStone[1] = 1025;
		tCheckDropEvent = FALSE;
		if (mUTIL.RandomNumber() <= 100)
		{
			tCheckDropEvent = TRUE;
			tRandomValue = rand() % 100;
			if (tRandomValue < 70)
			{
				tItemIndex = tAddStone[0];
			}
			else
			{
				tItemIndex = tAddStone[1];
			}
		}
		if (tCheckDropEvent)
		{
			mUTIL.ProcessForDropItem(1, tItemIndex, 0, 0, mUTIL.ReturnItemRecognitionNumber(0),
			                         &mDATA.mAction.aLocation[0], &tMasterName[0], 0);
		}
	}
	//---------------------//
	//DROP_SKILL_PIECE_ITEM//
	//---------------------//
#if 0
	if (mSERVER_INFO.m_iDeploymentStage > 2) {
		if (tCheckPossibleDrop)
		{
			if (mMONSTER_INFO->mItemLevel >= 80)
			{
				tSkillPieceInfo[0][0] = 1054; //...
				tSkillPieceInfo[0][1] = 1055; //...
				tSkillPieceInfo[0][2] = 1056; //...
				tSkillPieceInfo[0][3] = 1057; //...  -> [.....]
				tSkillPieceInfo[1][0] = 1058; //....
				tSkillPieceInfo[1][1] = 1059; //....
				tSkillPieceInfo[1][2] = 1060; //....
				tSkillPieceInfo[1][3] = 1061; //....-> [.....]
				tSkillPieceInfo[2][0] = 1062; //...
				tSkillPieceInfo[2][1] = 1063; //...
				tSkillPieceInfo[2][2] = 1064; //...
				tSkillPieceInfo[2][3] = 1065; //...  -> [.....]
				tCheckDropEvent = FALSE;
				if (mUTIL.RandomNumber() <= 10)
				{
					tCheckDropEvent = TRUE;
				}
				if (mUSER[tUserIndex].mHealthState == 4)
				{
					if (tCheckDropEvent)
					{
						tRandom = rand();
						if ((tRandom % 2) == 0)
						{
							tCheckDropEvent = FALSE;
						}
					}
				}
				else  if (mUSER[tUserIndex].mHealthState == 5) {
					tCheckDropEvent = false;
				}

				if (tCheckDropEvent)
				{
					tRandom1 = rand();
					tRandom2 = rand();
					mUTIL.ProcessForDropItem(1, tSkillPieceInfo[(tRandom1 % 3)][(tRandom2 % 4)], 0, 0, mUTIL.ReturnItemRecognitionNumber(0), &mDATA.mAction.aLocation[0], &tMasterName[0], 0);
				}
			}
		}
	}
#endif
	//-------------//
	//DROP_PAT_ITEM//
	//-------------//
	if (tCheckPossibleDrop)
	{
		tPatInfo[0][0] = 1002;
		tPatInfo[0][1] = 1003;
		tPatInfo[0][2] = 1004;
		tPatInfo[0][3] = 1005;
		tPatInfo[0][4] = 0;
		tPatInfo[0][5] = 0;
		tPatInfo[1][0] = 1006;
		tPatInfo[1][1] = 1007;
		tPatInfo[1][2] = 1008;
		tPatInfo[1][3] = 1009;
		tPatInfo[1][4] = 1010;
		tPatInfo[1][5] = 0;
		tPatInfo[2][0] = 1011;
		tPatInfo[2][1] = 1012;
		tPatInfo[2][2] = 1013;
		tPatInfo[2][3] = 1014;
		tPatInfo[2][4] = 0;
		tPatInfo[2][5] = 0;
		tPatInfo[3][0] = 1015;
		tPatInfo[3][1] = 1016;
		tPatInfo[3][2] = 0;
		tPatInfo[3][3] = 0;
		tPatInfo[3][4] = 0;
		tPatInfo[3][5] = 0;
		tCheckDropEvent = FALSE;

		int iModulusBy1 = 0, iModulusBy2 = 0, iModulusBy3 = 0, iModulusBy4 = 0;

		switch (mSERVER_INFO.m_iDeploymentStage)
		{
		case 0:
			break;
		case 1:
			iModulusBy1 = 4;
			break;
		case 2:
			iModulusBy1 = 4;
			iModulusBy2 = 5;
			break;
		case 3:
			iModulusBy1 = 4;
			iModulusBy2 = 5;
			iModulusBy3 = 4;
			break;
		case 4:
			iModulusBy1 = 4;
			iModulusBy2 = 5;
			iModulusBy3 = 4;
			iModulusBy4 = 2;
			break;
		}

		tRandom = rand();
		switch (tRandom % 4)
		{
		case 0:
			if (mUTIL.RandomNumber() <= 4)
			{
				if (iModulusBy1 > 0)
				{
					tCheckDropEvent = TRUE;
					tRandom = rand();
					tItemIndex = tPatInfo[0][(tRandom % iModulusBy1)];
				}
			}
			break;
		case 1:
			if (mUTIL.RandomNumber() <= 3)
			{
				if (mAVATAR_OBJECT[tUserIndex].mDATA.aLevel1 < 100)
				{
					if (iModulusBy1 > 0)
					{
						tCheckDropEvent = TRUE;
						tRandom = rand();
						tItemIndex = tPatInfo[0][(tRandom % iModulusBy1)];
					}
				}
				else
				{
					if (iModulusBy2 > 0)
					{
						tCheckDropEvent = TRUE;
						tRandom = rand();
						tItemIndex = tPatInfo[1][(tRandom % iModulusBy2)];
					}
					else if (iModulusBy1 > 0)
					{
						tCheckDropEvent = TRUE;
						tRandom = rand();
						tItemIndex = tPatInfo[0][(tRandom % iModulusBy1)];
					}
				}
			}
			break;
		case 2:
			if (mUTIL.RandomNumber() <= 2)
			{
				if (mAVATAR_OBJECT[tUserIndex].mDATA.aLevel1 < 100)
				{
					if (iModulusBy1 > 0)
					{
						tCheckDropEvent = TRUE;
						tRandom = rand();
						tItemIndex = tPatInfo[0][(tRandom % iModulusBy1)];
					}
				}
				else if (mAVATAR_OBJECT[tUserIndex].mDATA.aLevel1 < 113)
				{
					if (iModulusBy2 > 0)
					{
						tCheckDropEvent = TRUE;
						tRandom = rand();
						tItemIndex = tPatInfo[1][(tRandom % iModulusBy2)];
					}
					else if (iModulusBy1 > 0)
					{
						tCheckDropEvent = TRUE;
						tRandom = rand();
						tItemIndex = tPatInfo[0][(tRandom % iModulusBy1)];
					}
				}
				else
				{
					if (iModulusBy3 > 0)
					{
						tCheckDropEvent = TRUE;
						tRandom = rand();
						tItemIndex = tPatInfo[2][(tRandom % iModulusBy3)];
					}
					else if (iModulusBy2 > 0)
					{
						tCheckDropEvent = TRUE;
						tRandom = rand();
						tItemIndex = tPatInfo[1][(tRandom % iModulusBy2)];
					}
					else if (iModulusBy1 > 0)
					{
						tCheckDropEvent = TRUE;
						tRandom = rand();
						tItemIndex = tPatInfo[0][(tRandom % iModulusBy1)];
					}
				}
			}
			break;
		case 3:
			if (mGAME.mCheckZone125Server)
			{
				if (mUTIL.RandomNumber() <= 1)
				{
					if (iModulusBy4 > 0)
					{
						tCheckDropEvent = TRUE;
						tItemIndex = tPatInfo[3][0];
					}
				}
			}
			else
			{
				if (mUTIL.RandomNumber() <= 1)
				{
					if (mAVATAR_OBJECT[tUserIndex].mDATA.aLevel1 < 100)
					{
						if (iModulusBy1 > 0)
						{
							tCheckDropEvent = TRUE;
							tRandom = rand();
							tItemIndex = tPatInfo[0][(tRandom % iModulusBy1)];
						}
					}
					else if (mAVATAR_OBJECT[tUserIndex].mDATA.aLevel1 < 113)
					{
						if (iModulusBy2 > 0)
						{
							tCheckDropEvent = TRUE;
							tRandom = rand();
							tItemIndex = tPatInfo[1][(tRandom % iModulusBy2)];
						}
						else if (iModulusBy1 > 0)
						{
							tCheckDropEvent = TRUE;
							tRandom = rand();
							tItemIndex = tPatInfo[0][(tRandom % iModulusBy1)];
						}
					}
					else
					{
						if (iModulusBy3 > 0)
						{
							tCheckDropEvent = TRUE;
							tRandom = rand();
							tItemIndex = tPatInfo[2][(tRandom % iModulusBy3)];
						}
						else if (iModulusBy2 > 0)
						{
							tCheckDropEvent = TRUE;
							tRandom = rand();
							tItemIndex = tPatInfo[1][(tRandom % iModulusBy2)];
						}
						else if (iModulusBy1 > 0)
						{
							tCheckDropEvent = TRUE;
							tRandom = rand();
							tItemIndex = tPatInfo[0][(tRandom % iModulusBy1)];
						}
					}
				}
			}
			break;
		}

		if (tCheckDropEvent)
		{
			mUTIL.ProcessForDropItem(1, tItemIndex, 0, 0, mUTIL.ReturnItemRecognitionNumber(0),
			                         &mDATA.mAction.aLocation[0], &tMasterName[0], 0);
		}
	}
	//-------------//
	//DROP_KEY_ITEM//
	//-------------//
	if (mSERVER_INFO.m_iDeploymentStage > 1)
	{
		if (tCheckPossibleDrop)
		{
			if (mUTIL.RandomNumber() <= 20)
			{
				tCheckDropEvent = TRUE;
			}

			if (tCheckDropEvent)
			{
				mUTIL.ProcessForDropItem(1, 1048, 0, 0, mUTIL.ReturnItemRecognitionNumber(0),
				                         &mDATA.mAction.aLocation[0], &tMasterName[0], 0);
			}
		}
	}
}
