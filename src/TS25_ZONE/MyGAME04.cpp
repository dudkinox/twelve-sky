//-------------------------------------------------------------------------------------------------
//HEADER
//-------------------------------------------------------------------------------------------------
#include "MainApp.h"
// # X-Trap # {
// # }
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_AVATAR_OBJECT
//-------------------------------------------------------------------------------------------------
//INSTANCE
AVATAR_OBJECT mAVATAR_OBJECT[MAX_AVATAR_OBJECT_NUM];
//SET_SPACE_INDEX
void AVATAR_OBJECT::Init(void)
{
	mLuck = 0;
	mIndex = 0;
	mMaxLife = 0;
	mMaxMana = 0;
	mCritical = 0;
	mAttackBlock = 0;
	mAttackPower = 0;
	mUniqueNumber = 0;
	mDefensePower = 0;
	mAttackSuccess = 0;
	mSPACE_INDEX[0] = 0;
	mSPACE_INDEX[1] = 0;
	mSPACE_INDEX[2] = 0;
	mCriticalDefense = 0;
	mDuelProcessSort = 0;
	mTradeProcessMenu = 0;
	mDuelProcessState = 0;
	mPartyProcessState = 0;
	mGuildProcessState = 0;
	mTradeProcessState = 0;
	mElementAttackPower = 0;
	mFriendProcessState = 0;
	mElementDefensePower = 0;
	mTeacherProcessState = 0;
	mCheckValidState = false;
	mDuelProcessRemainTime = 0;
	mLastAttackBonusValue = 0.0f;
	mLastAttackBonusProbability = 0;
	mCheckPossibleEatPotion = false;
	memset(&mPShopInfo, 0x00, sizeof(PSHOP_INFO));
	memset(&mDATA, 0x00, SIZE_OF_OBJECT_FOR_AVATAR);
	memset(mDuelProcessAvatarName, 0x00, MAX_AVATAR_NAME_LENGTH);
	memset(mTradeProcessAvatarName, 0x00, MAX_AVATAR_NAME_LENGTH);
	memset(mPartyProcessAvatarName, 0x00, MAX_AVATAR_NAME_LENGTH);
	memset(mGuildProcessAvatarName, 0x00, MAX_AVATAR_NAME_LENGTH);
	memset(mFriendProcessAvatarName, 0x00, MAX_AVATAR_NAME_LENGTH);
	memset(mTeacherProcessAvatarName, 0x00, MAX_AVATAR_NAME_LENGTH);
}

void AVATAR_OBJECT::SetSpaceIndex(void)
{
	mSPACE_INDEX[0] = static_cast<int>(mDATA.aAction.aLocation[0] / MAX_RADIUS_FOR_NETWORK);
	mSPACE_INDEX[1] = static_cast<int>(mDATA.aAction.aLocation[1] / MAX_RADIUS_FOR_NETWORK);
	mSPACE_INDEX[2] = static_cast<int>(mDATA.aAction.aLocation[2] / MAX_RADIUS_FOR_NETWORK);
}

//SET_BASIC_ABILITY_FROM_EQUIP
void AVATAR_OBJECT::SetBasicAbilityFromEquip(void)
{
	if (!mCheckValidState)
	{
		return;
	}

	int index01;
	AVATAR_INFO* tAvatarInfo = &mUSER[mIndex].mAvatarInfo;
	int tVitality;
	int tKi;
	int tStrength;
	int tWisdom;
	int tISValue;
	int tIUValue;
	int tIUEffectValue;
	int tRFValue;

	for (index01 = 0; index01 < MAX_EQUIP_SLOT_NUM; index01++)
	{
		mEquipItem[index01] = mITEM.Search(tAvatarInfo->aEquip[index01][0]);
		mEquipValue[index01] = tAvatarInfo->aEquip[index01][2];
	}
	mMaxLife = 1;
	tVitality = tAvatarInfo->aVit;

	// When Mount is Absorbed
	tVitality += mUTIL.GetAbsorbBonusStats(mIndex);
	/*
		if ((mUSER[mIndex].mAvatarInfo.aAbsorbState > 0) && (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber != 1329) && (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber != 1330) && (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber != 1331))
		{
			tVitality += 10;
		}
		if ((mUSER[mIndex].mAvatarInfo.aAbsorbState > 0) && ((mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber == 1329) || (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber == 1330) || (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber == 1331)))
		{
			tVitality += 15;
		}
	*/
	for (index01 = 0; index01 < MAX_EQUIP_SLOT_NUM; index01++)
	{
		if (mEquipItem[index01] == nullptr)
		{
			continue;
		}
		tVitality += mEquipItem[index01]->iVitality;

		// Refine System
		switch (index01)
		{
		case 4: // Ring
			tRFValue = mUTIL.ReturnRFValue(mEquipValue[index01]);
			if (tRFValue > 0)
			{
				if (tRFValue == 1) tVitality += 5;
				if (tRFValue == 2) tVitality += 10;
				if (tRFValue == 3) tVitality += 15;
				if (tRFValue == 4) tVitality += 20;
				if (tRFValue == 5) tVitality += 25;
				if (tRFValue == 6) tVitality += 30;
				if (tRFValue == 7) tVitality += 35;
				if (tRFValue == 8) tVitality += 40;
				if (tRFValue == 9) tVitality += 45;
				if (tRFValue == 10) tVitality += 50;
				if (tRFValue == 11) tVitality += 55;
				if (tRFValue == 12) tVitality += 60;
				if (tRFValue == 13) tVitality += 65;
				if (tRFValue == 14) tVitality += 70;
				if (tRFValue == 15) tVitality += 75;
				if (tRFValue == 16) tVitality += 80;
				if (tRFValue == 17) tVitality += 85;
				if (tRFValue == 18) tVitality += 90;
				if (tRFValue == 19) tVitality += 95;
				if (tRFValue == 20) tVitality += 100;
				if (tRFValue == 21) tVitality += 105;
				if (tRFValue == 22) tVitality += 110;
				if (tRFValue == 23) tVitality += 115;
				if (tRFValue == 24) tVitality += 120;
				if (tRFValue == 25) tVitality += 130;
			}
			break;
		}
	}
	// Title System
	if (mUSER[mIndex].mAvatarInfo.aTitle == 1) tVitality += 1;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 2) tVitality += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 3) tVitality += 6;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 4) tVitality += 10;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 5) tVitality += 15;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 6) tVitality += 21;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 7) tVitality += 28;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 8) tVitality += 36;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 9) tVitality += 45;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 10) tVitality += 55;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 11) tVitality += 67;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 12) tVitality += 82;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 101) tVitality += 0;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 102) tVitality += 1;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 103) tVitality += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 104) tVitality += 5;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 105) tVitality += 8;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 106) tVitality += 11;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 107) tVitality += 14;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 108) tVitality += 18;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 109) tVitality += 23;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 110) tVitality += 28;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 111) tVitality += 34;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 112) tVitality += 41;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 201) tVitality += 0;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 202) tVitality += 1;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 203) tVitality += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 204) tVitality += 5;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 205) tVitality += 8;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 206) tVitality += 11;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 207) tVitality += 14;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 208) tVitality += 18;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 209) tVitality += 23;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 210) tVitality += 28;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 211) tVitality += 34;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 212) tVitality += 41;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 301) tVitality += 2;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 302) tVitality += 6;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 303) tVitality += 12;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 304) tVitality += 20;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 305) tVitality += 30;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 306) tVitality += 42;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 307) tVitality += 56;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 308) tVitality += 72;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 309) tVitality += 90;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 310) tVitality += 110;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 311) tVitality += 134;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 312) tVitality += 164;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 401) tVitality += 0;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 402) tVitality += 1;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 403) tVitality += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 404) tVitality += 5;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 405) tVitality += 8;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 406) tVitality += 11;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 407) tVitality += 14;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 408) tVitality += 18;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 409) tVitality += 23;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 410) tVitality += 28;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 411) tVitality += 34;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 412) tVitality += 41;

	// Halo System
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 1) tVitality += 1;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 2) tVitality += 2;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 3) tVitality += 3;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 4) tVitality += 4;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 5) tVitality += 5;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 6) tVitality += 6;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 7) tVitality += 7;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 8) tVitality += 8;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 9) tVitality += 9;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 10) tVitality += 10;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 11) tVitality += 11;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 12) tVitality += 12;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 13) tVitality += 13;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 14) tVitality += 14;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 15) tVitality += 15;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 16) tVitality += 16;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 17) tVitality += 17;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 18) tVitality += 18;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 19) tVitality += 19;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 20) tVitality += 20;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 21) tVitality += 21;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 22) tVitality += 22;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 23) tVitality += 23;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 24) tVitality += 24;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 25) tVitality += 25;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 26) tVitality += 26;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 27) tVitality += 27;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 28) tVitality += 28;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 29) tVitality += 29;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 30) tVitality += 30;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 31) tVitality += 31;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 32) tVitality += 32;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 33) tVitality += 33;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 34) tVitality += 34;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 35) tVitality += 35;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 36) tVitality += 36;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 37) tVitality += 37;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 38) tVitality += 38;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 39) tVitality += 39;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 40) tVitality += 40;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 41) tVitality += 41;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 42) tVitality += 42;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 43) tVitality += 43;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 44) tVitality += 44;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 45) tVitality += 45;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 46) tVitality += 46;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 47) tVitality += 47;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 48) tVitality += 48;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 49) tVitality += 49;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 50) tVitality += 50;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 51) tVitality += 51;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 52) tVitality += 52;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 53) tVitality += 53;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 54) tVitality += 54;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 55) tVitality += 55;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 56) tVitality += 56;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 57) tVitality += 57;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 58) tVitality += 58;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 59) tVitality += 59;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 60) tVitality += 60;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 61) tVitality += 61;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 62) tVitality += 62;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 63) tVitality += 63;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 64) tVitality += 64;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 65) tVitality += 65;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 66) tVitality += 66;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 67) tVitality += 67;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 68) tVitality += 68;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 69) tVitality += 69;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 70) tVitality += 70;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 71) tVitality += 71;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 72) tVitality += 72;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 73) tVitality += 73;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 74) tVitality += 74;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 75) tVitality += 75;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 76) tVitality += 76;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 77) tVitality += 77;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 78) tVitality += 78;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 79) tVitality += 79;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 80) tVitality += 80;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 81) tVitality += 81;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 82) tVitality += 82;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 83) tVitality += 83;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 84) tVitality += 84;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 85) tVitality += 85;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 86) tVitality += 86;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 87) tVitality += 87;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 88) tVitality += 88;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 89) tVitality += 89;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 90) tVitality += 90;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 91) tVitality += 91;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 92) tVitality += 92;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 93) tVitality += 93;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 94) tVitality += 94;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 95) tVitality += 95;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 96) tVitality += 96;

	mMaxLife = static_cast<int>((float)tVitality * 20.00f);

	if (mUSER[mIndex].mAvatarInfo.aRankAbility == 6) mMaxLife += 1000;

	/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */
	// 장식 해방 시스템이 사용중인 경우 체력 증가.
	if (IsUsedOrnament() == 1) // 금장서의 경우
	{
		mMaxLife += 825;
	}
	else if (IsUsedOrnament() == 2) // 은장서의 경우
	{
		mMaxLife += 550; // 현재는 은장서만 사용 가능.
	}
	/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */
#ifdef __GOD__
	if ((!mGAME.mCheckZone049TypeServer) && (!mGAME.mCheckZone051TypeServer) && (!mGAME.mCheckZone053TypeServer) && (!
		mGAME.mCheckZone267TypeServer))
#else
	if ((!mGAME.mCheckZone049TypeServer) && (!mGAME.mCheckZone051TypeServer) && (!mGAME.mCheckZone053TypeServer))
#endif
	{
		mMaxLife += (tAvatarInfo->aEatLifePotion * 20);
	}
#ifdef __GOD__
	mMaxLife += mLEVEL.ReturnLevelFactorLife((mDATA.aLevel1 + mDATA.aLevel2));
#else
	mMaxLife += mLEVEL.ReturnLevelFactorLife(mDATA.aLevel1);
#endif

#ifdef __ANIMAL__
	if ((mDATA.aAnimalNumber == ANIMAL_NUM_DONWHOANG) || (mDATA.aAnimalNumber == ANIMAL_NUM_BULL1))
	{
		mMaxLife = static_cast<int>((float)mMaxLife * ANIMAL_RATE_ONE_GRADE);
	}
	if ((mDATA.aAnimalNumber == ANIMAL_NUM_TWODON) || (mDATA.aAnimalNumber == ANIMAL_NUM_BULL2))
	{
		mMaxLife = static_cast<int>((float)mMaxLife * ANIMAL_RATE_TWO_GRADE);
	}
	if ((mDATA.aAnimalNumber == ANIMAL_NUM_SUNGDON) || (mDATA.aAnimalNumber == ANIMAL_NUM_BULL3) || (mDATA.aAnimalNumber
		== ANIMAL_NUM_BLACKLEO))
	{
		mMaxLife = static_cast<int>((float)mMaxLife * ANIMAL_RATE_THREE_GRADE);
	}
	if (mDATA.aAnimalNumber == ANIMAL_NUM_WHITELEO)
	{
		mMaxLife = static_cast<int>((float)mMaxLife * ANIMAL_RATE_FOUR_GRADE);
	}
#endif

	// Health Boost Scrolls
	if (mUSER[mIndex].mAvatarInfo.aHealthBoostScroll > 0)
	{
		mMaxLife *= 1.20f;
	}
	if (mMaxLife < static_cast<int>(mPAT.ReturnLifeValue(tAvatarInfo->aEquip[8][0], tAvatarInfo->aEquip[8][2])))
	{
		mMaxLife += mMaxLife;
	}
	else
	{
		mMaxLife += static_cast<int>(mPAT.ReturnLifeValue(tAvatarInfo->aEquip[8][0], tAvatarInfo->aEquip[8][2]));
	}
	// Refine
	for (index01 = 0; index01 < MAX_EQUIP_SLOT_NUM; index01++)
	{
		switch (index01)
		{
		case 2: //Armor
			tRFValue = mUTIL.ReturnRFValue(mEquipValue[index01]);
			if (tRFValue > 0)
			{
				if (tRFValue == 1) mMaxLife += 400;
				if (tRFValue == 2) mMaxLife += 800;
				if (tRFValue == 3) mMaxLife += 1200;
				if (tRFValue == 4) mMaxLife += 1600;
				if (tRFValue == 5) mMaxLife += 2000;
				if (tRFValue == 6) mMaxLife += 2400;
				if (tRFValue == 7) mMaxLife += 2800;
				if (tRFValue == 8) mMaxLife += 3200;
				if (tRFValue == 9) mMaxLife += 3600;
				if (tRFValue == 10) mMaxLife += 4000;
				if (tRFValue == 11) mMaxLife += 4400;
				if (tRFValue == 12) mMaxLife += 4800;
				if (tRFValue == 13) mMaxLife += 5200;
				if (tRFValue == 14) mMaxLife += 5600;
				if (tRFValue == 15) mMaxLife += 6000;
				if (tRFValue == 16) mMaxLife += 6400;
				if (tRFValue == 17) mMaxLife += 6800;
				if (tRFValue == 18) mMaxLife += 7200;
				if (tRFValue == 19) mMaxLife += 7600;
				if (tRFValue == 20) mMaxLife += 8000;
				if (tRFValue == 21) mMaxLife += 8400;
				if (tRFValue == 22) mMaxLife += 8800;
				if (tRFValue == 23) mMaxLife += 9200;
				if (tRFValue == 24) mMaxLife += 9600;
				if (tRFValue == 25) mMaxLife += 10000;
			}
			break;
		}
	}
	// Mount Ability
	if (mUSER[mIndex].mAvatarInfo.aAnimalIndex >= 10)
	{
		switch ((mUSER[mIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[mIndex].mAvatarInfo.aAnimalIndex - 10] % 1000000) /
			100000)
		{
		case 1:
			mMaxLife += 100;
			break;
		case 2:
			mMaxLife += 200;
			break;
		case 3:
			mMaxLife += 300;
			break;
		case 4:
			mMaxLife += 400;
			break;
		case 5:
			mMaxLife += 500;
			break;
		case 6:
			mMaxLife += 600;
			break;
		case 7:
			mMaxLife += 700;
			break;
		case 8:
			mMaxLife += 800;
			break;
		case 9:
			mMaxLife += 900;
			break;
		}
	}
	// Socket Gem Stats
	for (index01 = 0; index01 < MAX_EQUIP_SLOT_NUM; index01++)
	{
		switch (index01)
		{
		case 0: //[Amulet]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 3)
			{
				mMaxLife += 100;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 8)
			{
				mMaxLife += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 3)
			{
				mMaxLife += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 8)
			{
				mMaxLife += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 3)
			{
				mMaxLife += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 8)
			{
				mMaxLife += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 3)
			{
				mMaxLife += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 8)
			{
				mMaxLife += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 3)
			{
				mMaxLife += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 8)
			{
				mMaxLife += 200;
			}
			break;
		case 1: //[Cape]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 3)
			{
				mMaxLife += 100;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 8)
			{
				mMaxLife += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 3)
			{
				mMaxLife += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 8)
			{
				mMaxLife += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 3)
			{
				mMaxLife += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 8)
			{
				mMaxLife += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 3)
			{
				mMaxLife += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 8)
			{
				mMaxLife += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 3)
			{
				mMaxLife += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 8)
			{
				mMaxLife += 200;
			}
			break;
		case 2: //[Armor]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 3)
			{
				mMaxLife += 100;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 8)
			{
				mMaxLife += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 3)
			{
				mMaxLife += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 8)
			{
				mMaxLife += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 3)
			{
				mMaxLife += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 8)
			{
				mMaxLife += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 3)
			{
				mMaxLife += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 8)
			{
				mMaxLife += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 3)
			{
				mMaxLife += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 8)
			{
				mMaxLife += 200;
			}
			break;
		case 3: //[Gloves]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 3)
			{
				mMaxLife += 100;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 8)
			{
				mMaxLife += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 3)
			{
				mMaxLife += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 8)
			{
				mMaxLife += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 3)
			{
				mMaxLife += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 8)
			{
				mMaxLife += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 3)
			{
				mMaxLife += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 8)
			{
				mMaxLife += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 3)
			{
				mMaxLife += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 8)
			{
				mMaxLife += 200;
			}
			break;
		case 4: //[Ring]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 3)
			{
				mMaxLife += 100;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 8)
			{
				mMaxLife += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 3)
			{
				mMaxLife += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 8)
			{
				mMaxLife += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 3)
			{
				mMaxLife += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 8)
			{
				mMaxLife += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 3)
			{
				mMaxLife += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 8)
			{
				mMaxLife += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 3)
			{
				mMaxLife += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 8)
			{
				mMaxLife += 200;
			}
			break;
		case 5: //[Boots]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 3)
			{
				mMaxLife += 100;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 8)
			{
				mMaxLife += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 3)
			{
				mMaxLife += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 8)
			{
				mMaxLife += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 3)
			{
				mMaxLife += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 8)
			{
				mMaxLife += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 3)
			{
				mMaxLife += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 8)
			{
				mMaxLife += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 3)
			{
				mMaxLife += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 8)
			{
				mMaxLife += 200;
			}
			break;
		case 7: //[Weapon]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 3)
			{
				mMaxLife += 100;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 8)
			{
				mMaxLife += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 3)
			{
				mMaxLife += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 8)
			{
				mMaxLife += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 3)
			{
				mMaxLife += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 8)
			{
				mMaxLife += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 3)
			{
				mMaxLife += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 8)
			{
				mMaxLife += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 3)
			{
				mMaxLife += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 8)
			{
				mMaxLife += 200;
			}
			break;
		}
	}
	// S3R V2
	if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87135) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87136) && (mUSER
		[mIndex].mAvatarInfo.aEquip[5][0] == 87137)) mMaxLife += 1000;
	if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87156) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87157) && (mUSER
		[mIndex].mAvatarInfo.aEquip[5][0] == 87158)) mMaxLife += 1000;
	if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87177) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87178) && (mUSER
		[mIndex].mAvatarInfo.aEquip[5][0] == 87179)) mMaxLife += 1000;
	// S3E V2
	if ((mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87198) && (mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87199)) mMaxLife
		+= 1100;
	if ((mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87220) && (mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87221)) mMaxLife
		+= 1100;
	if ((mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87242) && (mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87243)) mMaxLife
		+= 1100;
	mDATA.aMaxLifeValue = mMaxLife;
	mMaxMana = 1;
	tKi = tAvatarInfo->aInt;

	// When Mount is Absorbed
	tKi += mUTIL.GetAbsorbBonusStats(mIndex);
	/*
		if ((mUSER[mIndex].mAvatarInfo.aAbsorbState > 0) && (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber != 1329) && (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber != 1330) && (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber != 1331))
		{
			tKi += 10;
		}
		if ((mUSER[mIndex].mAvatarInfo.aAbsorbState > 0) && ((mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber == 1329) || (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber == 1330) || (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber == 1331)))
		{
			tKi += 15;
		}
	*/
	// Ninja Suit
	if ((mAVATAR_OBJECT[mIndex].mDATA.aFashionNumber == 385) || (mAVATAR_OBJECT[mIndex].mDATA.aFashionNumber == 386) ||
		(mAVATAR_OBJECT[mIndex].mDATA.aFashionNumber == 387))
	{
		tKi += 50;
	}
	for (index01 = 0; index01 < MAX_EQUIP_SLOT_NUM; index01++)
	{
		if (mEquipItem[index01] == nullptr)
		{
			continue;
		}
		tKi += mEquipItem[index01]->iKi;
		switch (index01)
		{
		case 7: //Add Chi from Refine Weapon
			tRFValue = mUTIL.ReturnRFValue(mEquipValue[index01]);
			if (tRFValue > 0)
			{
				if (tRFValue == 1) tKi += 5;
				if (tRFValue == 2) tKi += 10;
				if (tRFValue == 3) tKi += 15;
				if (tRFValue == 4) tKi += 20;
				if (tRFValue == 5) tKi += 25;
				if (tRFValue == 6) tKi += 30;
				if (tRFValue == 7) tKi += 35;
				if (tRFValue == 8) tKi += 40;
				if (tRFValue == 9) tKi += 45;
				if (tRFValue == 10) tKi += 50;
				if (tRFValue == 11) tKi += 55;
				if (tRFValue == 12) tKi += 60;
				if (tRFValue == 13) tKi += 65;
				if (tRFValue == 14) tKi += 70;
				if (tRFValue == 15) tKi += 75;
				if (tRFValue == 16) tKi += 80;
				if (tRFValue == 17) tKi += 85;
				if (tRFValue == 18) tKi += 90;
				if (tRFValue == 19) tKi += 95;
				if (tRFValue == 20) tKi += 100;
				if (tRFValue == 21) tKi += 105;
				if (tRFValue == 22) tKi += 110;
				if (tRFValue == 23) tKi += 115;
				if (tRFValue == 24) tKi += 120;
				if (tRFValue == 25) tKi += 130;
			}
			break;
		case 2:
			tRFValue = mUTIL.ReturnRFValue(mEquipValue[index01]);
			if (tRFValue > 0)
			{
				if (tRFValue == 1) tKi += 5;
				if (tRFValue == 2) tKi += 10;
				if (tRFValue == 3) tKi += 15;
				if (tRFValue == 4) tKi += 20;
				if (tRFValue == 5) tKi += 25;
				if (tRFValue == 6) tKi += 30;
				if (tRFValue == 7) tKi += 35;
				if (tRFValue == 8) tKi += 40;
				if (tRFValue == 9) tKi += 45;
				if (tRFValue == 10) tKi += 50;
				if (tRFValue == 11) tKi += 55;
				if (tRFValue == 12) tKi += 60;
				if (tRFValue == 13) tKi += 65;
				if (tRFValue == 14) tKi += 70;
				if (tRFValue == 15) tKi += 75;
				if (tRFValue == 16) tKi += 80;
				if (tRFValue == 17) tKi += 85;
				if (tRFValue == 18) tKi += 90;
				if (tRFValue == 19) tKi += 95;
				if (tRFValue == 20) tKi += 100;
				if (tRFValue == 21) tKi += 105;
				if (tRFValue == 22) tKi += 110;
				if (tRFValue == 23) tKi += 115;
				if (tRFValue == 24) tKi += 120;
				if (tRFValue == 25) tKi += 130;
			}
			break;
		}
	}
	// Title System
	if (mUSER[mIndex].mAvatarInfo.aTitle == 1) tKi += 1;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 2) tKi += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 3) tKi += 6;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 4) tKi += 10;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 5) tKi += 15;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 6) tKi += 21;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 7) tKi += 28;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 8) tKi += 36;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 9) tKi += 45;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 10) tKi += 55;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 11) tKi += 67;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 12) tKi += 82;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 101) tKi += 1;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 102) tKi += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 103) tKi += 6;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 104) tKi += 10;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 105) tKi += 15;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 106) tKi += 21;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 107) tKi += 28;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 108) tKi += 36;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 109) tKi += 45;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 110) tKi += 55;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 111) tKi += 67;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 112) tKi += 82;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 201) tKi += 0;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 202) tKi += 1;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 203) tKi += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 204) tKi += 5;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 205) tKi += 8;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 206) tKi += 11;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 207) tKi += 14;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 208) tKi += 18;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 209) tKi += 23;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 210) tKi += 28;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 211) tKi += 34;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 212) tKi += 41;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 301) tKi += 1;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 302) tKi += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 303) tKi += 6;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 304) tKi += 10;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 305) tKi += 15;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 306) tKi += 21;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 307) tKi += 28;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 308) tKi += 36;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 309) tKi += 45;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 310) tKi += 55;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 311) tKi += 67;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 312) tKi += 82;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 401) tKi += 2;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 402) tKi += 6;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 403) tKi += 12;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 404) tKi += 20;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 405) tKi += 30;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 406) tKi += 42;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 407) tKi += 56;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 408) tKi += 72;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 409) tKi += 90;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 410) tKi += 110;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 411) tKi += 134;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 412) tKi += 164;

	// Halo System
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 1) tKi += 1;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 2) tKi += 2;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 3) tKi += 3;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 4) tKi += 4;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 5) tKi += 5;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 6) tKi += 6;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 7) tKi += 7;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 8) tKi += 8;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 9) tKi += 9;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 10) tKi += 10;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 11) tKi += 11;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 12) tKi += 12;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 13) tKi += 13;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 14) tKi += 14;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 15) tKi += 15;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 16) tKi += 16;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 17) tKi += 17;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 18) tKi += 18;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 19) tKi += 19;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 20) tKi += 20;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 21) tKi += 21;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 22) tKi += 22;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 23) tKi += 23;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 24) tKi += 24;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 25) tKi += 25;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 26) tKi += 26;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 27) tKi += 27;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 28) tKi += 28;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 29) tKi += 29;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 30) tKi += 30;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 31) tKi += 31;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 32) tKi += 32;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 33) tKi += 33;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 34) tKi += 34;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 35) tKi += 35;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 36) tKi += 36;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 37) tKi += 37;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 38) tKi += 38;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 39) tKi += 39;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 40) tKi += 40;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 41) tKi += 41;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 42) tKi += 42;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 43) tKi += 43;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 44) tKi += 44;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 45) tKi += 45;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 46) tKi += 46;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 47) tKi += 47;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 48) tKi += 48;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 49) tKi += 49;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 50) tKi += 50;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 51) tKi += 51;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 52) tKi += 52;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 53) tKi += 53;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 54) tKi += 54;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 55) tKi += 55;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 56) tKi += 56;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 57) tKi += 57;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 58) tKi += 58;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 59) tKi += 59;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 60) tKi += 60;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 61) tKi += 61;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 62) tKi += 62;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 63) tKi += 63;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 64) tKi += 64;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 65) tKi += 65;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 66) tKi += 66;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 67) tKi += 67;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 68) tKi += 68;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 69) tKi += 69;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 70) tKi += 70;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 71) tKi += 71;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 72) tKi += 72;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 73) tKi += 73;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 74) tKi += 74;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 75) tKi += 75;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 76) tKi += 76;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 77) tKi += 77;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 78) tKi += 78;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 79) tKi += 79;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 80) tKi += 80;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 81) tKi += 81;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 82) tKi += 82;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 83) tKi += 83;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 84) tKi += 84;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 85) tKi += 85;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 86) tKi += 86;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 87) tKi += 87;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 88) tKi += 88;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 89) tKi += 89;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 90) tKi += 90;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 91) tKi += 91;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 92) tKi += 92;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 93) tKi += 93;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 94) tKi += 94;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 95) tKi += 95;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 96) tKi += 96;

	mMaxMana = static_cast<int>((float)tKi * 15.31f);

	/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */
	// 장식 해방 시스템이 사용중인 경우 내공 증가.
	if (IsUsedOrnament() == 1) // 금장서의 경우
	{
		mMaxMana += 750;
	}
	else if (IsUsedOrnament() == 2) //은장서의 경우
	{
		mMaxMana += 500; // 현재는 은장서만 사용 가능.
	}
	/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */

	if ((!mGAME.mCheckZone049TypeServer) && (!mGAME.mCheckZone051TypeServer) && (!mGAME.mCheckZone053TypeServer))
	{
		mMaxMana += (tAvatarInfo->aEatManaPotion * 25);
	}
#ifdef __GOD__
	mMaxMana += mLEVEL.ReturnLevelFactorMana((mDATA.aLevel1 + mDATA.aLevel2));
#else
	mMaxMana += mLEVEL.ReturnLevelFactorMana(mDATA.aLevel1);
#endif

#ifdef __ANIMAL__
	if ((mDATA.aAnimalNumber == ANIMAL_NUM_BAKHO)
		|| (mDATA.aAnimalNumber == ANIMAL_NUM_DONWHOANG)
		|| (mDATA.aAnimalNumber == ANIMAL_NUM_CHUNLOK)
		|| (mDATA.aAnimalNumber == ANIMAL_NUM_FERRET1)
		|| (mDATA.aAnimalNumber == ANIMAL_NUM_WOLF1)
		|| (mDATA.aAnimalNumber == ANIMAL_NUM_HAECHI1)
		|| (mDATA.aAnimalNumber == ANIMAL_NUM_BEAR1)
		|| (mDATA.aAnimalNumber == ANIMAL_NUM_BULL1))
	{
		mMaxMana = static_cast<int>((float)mMaxMana * ANIMAL_RATE_ONE_GRADE);
	}

	if ((mDATA.aAnimalNumber == ANIMAL_NUM_CHUNGHO)
		|| (mDATA.aAnimalNumber == ANIMAL_NUM_TWODON)
		|| (mDATA.aAnimalNumber == ANIMAL_NUM_BAIKLOK)
		|| (mDATA.aAnimalNumber == ANIMAL_NUM_FERRET2)
		|| (mDATA.aAnimalNumber == ANIMAL_NUM_WOLF2)
		|| (mDATA.aAnimalNumber == ANIMAL_NUM_HAECHI2)
		|| (mDATA.aAnimalNumber == ANIMAL_NUM_BEAR2)
		|| (mDATA.aAnimalNumber == ANIMAL_NUM_BULL2))
	{
		mMaxMana = static_cast<int>((float)mMaxMana * ANIMAL_RATE_TWO_GRADE);
	}
	if ((mDATA.aAnimalNumber == ANIMAL_NUM_CHUKHO)
		|| (mDATA.aAnimalNumber == ANIMAL_NUM_SUNGDON)
		|| (mDATA.aAnimalNumber == ANIMAL_NUM_SHINLOK)
		|| (mDATA.aAnimalNumber == ANIMAL_NUM_FERRET3)
		|| (mDATA.aAnimalNumber == ANIMAL_NUM_WOLF3)
		|| (mDATA.aAnimalNumber == ANIMAL_NUM_HAECHI3)
		|| (mDATA.aAnimalNumber == ANIMAL_NUM_BEAR3)
		|| (mDATA.aAnimalNumber == ANIMAL_NUM_BULL3))
	{
		mMaxMana = static_cast<int>((float)mMaxMana * ANIMAL_RATE_THREE_GRADE);
	}
#endif
	if (mMaxMana < static_cast<int>(mPAT.ReturnManaValue(tAvatarInfo->aEquip[8][0], tAvatarInfo->aEquip[8][2])))
	{
		mMaxMana += mMaxMana;
	}
	else
	{
		mMaxMana += static_cast<int>(mPAT.ReturnManaValue(tAvatarInfo->aEquip[8][0], tAvatarInfo->aEquip[8][2]));
	}
	// Mount Ability
	if (mUSER[mIndex].mAvatarInfo.aAnimalIndex >= 10)
	{
		switch ((mUSER[mIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[mIndex].mAvatarInfo.aAnimalIndex - 10] % 100000) /
			10000)
		{
		case 1:
			mMaxMana += 200;
			break;
		case 2:
			mMaxMana += 400;
			break;
		case 3:
			mMaxMana += 600;
			break;
		case 4:
			mMaxMana += 800;
			break;
		case 5:
			mMaxMana += 1000;
			break;
		case 6:
			mMaxMana += 1200;
			break;
		case 7:
			mMaxMana += 1400;
			break;
		case 8:
			mMaxMana += 1600;
			break;
		case 9:
			mMaxMana += 1800;
			break;
		}
	}
	// Socket Gem Stats
	for (index01 = 0; index01 < MAX_EQUIP_SLOT_NUM; index01++)
	{
		switch (index01)
		{
		case 0: //[Amulet]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 3)
			{
				mMaxMana += 100;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 8)
			{
				mMaxMana += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 3)
			{
				mMaxMana += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 8)
			{
				mMaxMana += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 3)
			{
				mMaxMana += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 8)
			{
				mMaxMana += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 3)
			{
				mMaxMana += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 8)
			{
				mMaxMana += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 3)
			{
				mMaxMana += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 8)
			{
				mMaxMana += 200;
			}
			break;
		case 1: //[Cape]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 3)
			{
				mMaxMana += 100;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 8)
			{
				mMaxMana += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 3)
			{
				mMaxMana += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 8)
			{
				mMaxMana += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 3)
			{
				mMaxMana += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 8)
			{
				mMaxMana += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 3)
			{
				mMaxMana += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 8)
			{
				mMaxMana += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 3)
			{
				mMaxMana += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 8)
			{
				mMaxMana += 200;
			}
			break;
		case 2: //[Armor]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 3)
			{
				mMaxMana += 100;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 8)
			{
				mMaxMana += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 3)
			{
				mMaxMana += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 8)
			{
				mMaxMana += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 3)
			{
				mMaxMana += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 8)
			{
				mMaxMana += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 3)
			{
				mMaxMana += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 8)
			{
				mMaxMana += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 3)
			{
				mMaxMana += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 8)
			{
				mMaxMana += 200;
			}
			break;
		case 3: //[Gloves]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 3)
			{
				mMaxMana += 100;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 8)
			{
				mMaxMana += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 3)
			{
				mMaxMana += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 8)
			{
				mMaxMana += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 3)
			{
				mMaxMana += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 8)
			{
				mMaxMana += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 3)
			{
				mMaxMana += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 8)
			{
				mMaxMana += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 3)
			{
				mMaxMana += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 8)
			{
				mMaxMana += 200;
			}
			break;
		case 4: //[Ring]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 3)
			{
				mMaxMana += 100;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 8)
			{
				mMaxMana += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 3)
			{
				mMaxMana += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 8)
			{
				mMaxMana += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 3)
			{
				mMaxMana += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 8)
			{
				mMaxMana += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 3)
			{
				mMaxMana += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 8)
			{
				mMaxMana += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 3)
			{
				mMaxMana += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 8)
			{
				mMaxMana += 200;
			}
			break;
		case 5: //[Boots]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 3)
			{
				mMaxMana += 100;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 8)
			{
				mMaxMana += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 3)
			{
				mMaxMana += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 8)
			{
				mMaxMana += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 3)
			{
				mMaxMana += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 8)
			{
				mMaxMana += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 3)
			{
				mMaxMana += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 8)
			{
				mMaxMana += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 3)
			{
				mMaxMana += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 8)
			{
				mMaxMana += 200;
			}
			break;
		case 7: //[Weapon]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 3)
			{
				mMaxMana += 100;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 8)
			{
				mMaxMana += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 3)
			{
				mMaxMana += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 8)
			{
				mMaxMana += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 3)
			{
				mMaxMana += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 8)
			{
				mMaxMana += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 3)
			{
				mMaxMana += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 8)
			{
				mMaxMana += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 3)
			{
				mMaxMana += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 8)
			{
				mMaxMana += 200;
			}
			break;
		}
	}
	// S2R V2
	if (mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87134) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87131) || (mUSER
		[mIndex].mAvatarInfo.aEquip[7][0] == 87132) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87133)) mMaxMana +=
		1000;
	if (mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87155) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87152) || (mUSER
		[mIndex].mAvatarInfo.aEquip[7][0] == 87153) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87154)) mMaxMana +=
		1000;
	if (mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87176) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87173) || (mUSER
		[mIndex].mAvatarInfo.aEquip[7][0] == 87174) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87175)) mMaxMana +=
		1000;
	// S2E V2
	if (mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87197) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87194) || (mUSER
		[mIndex].mAvatarInfo.aEquip[7][0] == 87195) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87196)) mMaxMana +=
		1100;
	if (mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87219) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87216) || (mUSER
		[mIndex].mAvatarInfo.aEquip[7][0] == 87217) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87218)) mMaxMana +=
		1100;
	if (mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87241) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87238) || (mUSER
		[mIndex].mAvatarInfo.aEquip[7][0] == 87239) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87240)) mMaxMana +=
		1100;
	mDATA.aMaxManaValue = mMaxMana;
	mAttackPower = 0;
	tStrength = tAvatarInfo->aStr;
	tKi = tAvatarInfo->aInt;

	// When Mount is Absorbed
	tStrength += mUTIL.GetAbsorbBonusStats(mIndex);
	/*
		if ((mUSER[mIndex].mAvatarInfo.aAbsorbState > 0) && (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber != 1329) && (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber != 1330) && (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber != 1331))
		{
			tStrength += 10;
		}
		if ((mUSER[mIndex].mAvatarInfo.aAbsorbState > 0) && ((mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber == 1329) || (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber == 1330) || (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber == 1331)))
		{
			tStrength += 15;
		}
	*/
	// Ninja Suit
	if ((mAVATAR_OBJECT[mIndex].mDATA.aFashionNumber == 385) || (mAVATAR_OBJECT[mIndex].mDATA.aFashionNumber == 386) ||
		(mAVATAR_OBJECT[mIndex].mDATA.aFashionNumber == 387))
	{
		tKi += 50;
	}

	// When Mount is Absorbed
	tKi += mUTIL.GetAbsorbBonusStats(mIndex);
	/*
		if ((mUSER[mIndex].mAvatarInfo.aAbsorbState > 0) && (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber != 1329) && (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber != 1330) && (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber != 1331))
		{
			tKi += 10;
		}
		if ((mUSER[mIndex].mAvatarInfo.aAbsorbState > 0) && ((mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber == 1329) || (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber == 1330) || (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber == 1331)))
		{
			tKi += 15;
		}
	*/
	// Refine System
	for (index01 = 0; index01 < MAX_EQUIP_SLOT_NUM; index01++)
	{
		switch (index01)
		{
		// Refine System
		case 7: //Add Chi from Refine Weapon
			tRFValue = mUTIL.ReturnRFValue(mEquipValue[index01]);
			if (tRFValue > 0)
			{
				if (tRFValue == 1) tKi += 5;
				if (tRFValue == 2) tKi += 10;
				if (tRFValue == 3) tKi += 15;
				if (tRFValue == 4) tKi += 20;
				if (tRFValue == 5) tKi += 25;
				if (tRFValue == 6) tKi += 30;
				if (tRFValue == 7) tKi += 35;
				if (tRFValue == 8) tKi += 40;
				if (tRFValue == 9) tKi += 45;
				if (tRFValue == 10) tKi += 50;
				if (tRFValue == 11) tKi += 55;
				if (tRFValue == 12) tKi += 60;
				if (tRFValue == 13) tKi += 65;
				if (tRFValue == 14) tKi += 70;
				if (tRFValue == 15) tKi += 75;
				if (tRFValue == 16) tKi += 80;
				if (tRFValue == 17) tKi += 85;
				if (tRFValue == 18) tKi += 90;
				if (tRFValue == 19) tKi += 95;
				if (tRFValue == 20) tKi += 100;
				if (tRFValue == 21) tKi += 105;
				if (tRFValue == 22) tKi += 110;
				if (tRFValue == 23) tKi += 115;
				if (tRFValue == 24) tKi += 120;
				if (tRFValue == 25) tKi += 130;
			}
			break;
		case 2:
			tRFValue = mUTIL.ReturnRFValue(mEquipValue[index01]);
			if (tRFValue > 0)
			{
				if (tRFValue == 1) tKi += 5;
				if (tRFValue == 2) tKi += 10;
				if (tRFValue == 3) tKi += 15;
				if (tRFValue == 4) tKi += 20;
				if (tRFValue == 5) tKi += 25;
				if (tRFValue == 6) tKi += 30;
				if (tRFValue == 7) tKi += 35;
				if (tRFValue == 8) tKi += 40;
				if (tRFValue == 9) tKi += 45;
				if (tRFValue == 10) tKi += 50;
				if (tRFValue == 11) tKi += 55;
				if (tRFValue == 12) tKi += 60;
				if (tRFValue == 13) tKi += 65;
				if (tRFValue == 14) tKi += 70;
				if (tRFValue == 15) tKi += 75;
				if (tRFValue == 16) tKi += 80;
				if (tRFValue == 17) tKi += 85;
				if (tRFValue == 18) tKi += 90;
				if (tRFValue == 19) tKi += 95;
				if (tRFValue == 20) tKi += 100;
				if (tRFValue == 21) tKi += 105;
				if (tRFValue == 22) tKi += 110;
				if (tRFValue == 23) tKi += 115;
				if (tRFValue == 24) tKi += 120;
				if (tRFValue == 25) tKi += 130;
			}
			break;

		case 0: // Amulet
			tRFValue = mUTIL.ReturnRFValue(mEquipValue[index01]);
			if (tRFValue > 0)
			{
				if (tRFValue == 1) tStrength += 5;
				if (tRFValue == 2) tStrength += 10;
				if (tRFValue == 3) tStrength += 15;
				if (tRFValue == 4) tStrength += 20;
				if (tRFValue == 5) tStrength += 25;
				if (tRFValue == 6) tStrength += 30;
				if (tRFValue == 7) tStrength += 35;
				if (tRFValue == 8) tStrength += 40;
				if (tRFValue == 9) tStrength += 45;
				if (tRFValue == 10) tStrength += 50;
				if (tRFValue == 11) tStrength += 55;
				if (tRFValue == 12) tStrength += 60;
				if (tRFValue == 13) tStrength += 65;
				if (tRFValue == 14) tStrength += 70;
				if (tRFValue == 15) tStrength += 75;
				if (tRFValue == 16) tStrength += 80;
				if (tRFValue == 17) tStrength += 85;
				if (tRFValue == 18) tStrength += 90;
				if (tRFValue == 19) tStrength += 95;
				if (tRFValue == 20) tStrength += 100;
				if (tRFValue == 21) tStrength += 105;
				if (tRFValue == 22) tStrength += 110;
				if (tRFValue == 23) tStrength += 115;
				if (tRFValue == 24) tStrength += 120;
				if (tRFValue == 25) tStrength += 130;
			}
			break;
		}
	}
	// Title System
	if (mUSER[mIndex].mAvatarInfo.aTitle == 1) tKi += 1;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 2) tKi += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 3) tKi += 6;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 4) tKi += 10;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 5) tKi += 15;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 6) tKi += 21;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 7) tKi += 28;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 8) tKi += 36;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 9) tKi += 45;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 10) tKi += 55;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 11) tKi += 67;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 12) tKi += 82;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 101) tKi += 1;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 102) tKi += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 103) tKi += 6;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 104) tKi += 10;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 105) tKi += 15;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 106) tKi += 21;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 107) tKi += 28;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 108) tKi += 36;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 109) tKi += 45;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 110) tKi += 55;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 111) tKi += 67;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 112) tKi += 82;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 201) tKi += 0;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 202) tKi += 1;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 203) tKi += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 204) tKi += 5;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 205) tKi += 8;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 206) tKi += 11;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 207) tKi += 14;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 208) tKi += 18;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 209) tKi += 23;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 210) tKi += 28;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 211) tKi += 34;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 212) tKi += 41;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 301) tKi += 1;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 302) tKi += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 303) tKi += 6;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 304) tKi += 10;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 305) tKi += 15;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 306) tKi += 21;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 307) tKi += 28;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 308) tKi += 36;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 309) tKi += 45;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 310) tKi += 55;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 311) tKi += 67;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 312) tKi += 82;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 401) tKi += 2;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 402) tKi += 6;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 403) tKi += 12;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 404) tKi += 20;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 405) tKi += 30;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 406) tKi += 42;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 407) tKi += 56;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 408) tKi += 72;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 409) tKi += 90;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 410) tKi += 110;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 411) tKi += 134;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 412) tKi += 164;

	// Title System
	if (mUSER[mIndex].mAvatarInfo.aTitle == 1) tStrength += 1;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 2) tStrength += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 3) tStrength += 6;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 4) tStrength += 10;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 5) tStrength += 15;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 6) tStrength += 21;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 7) tStrength += 28;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 8) tStrength += 36;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 9) tStrength += 45;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 10) tStrength += 55;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 11) tStrength += 67;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 12) tStrength += 82;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 101) tStrength += 2;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 102) tStrength += 6;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 103) tStrength += 12;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 104) tStrength += 20;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 105) tStrength += 30;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 106) tStrength += 42;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 107) tStrength += 56;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 108) tStrength += 72;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 109) tStrength += 99;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 110) tStrength += 110;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 111) tStrength += 134;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 112) tStrength += 164;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 201) tStrength += 1;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 202) tStrength += 2;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 203) tStrength += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 204) tStrength += 5;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 205) tStrength += 7;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 206) tStrength += 10;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 207) tStrength += 14;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 208) tStrength += 18;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 209) tStrength += 22;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 210) tStrength += 27;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 211) tStrength += 33;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 212) tStrength += 41;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 301) tStrength += 0;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 302) tStrength += 1;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 303) tStrength += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 304) tStrength += 5;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 305) tStrength += 8;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 306) tStrength += 11;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 307) tStrength += 14;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 308) tStrength += 18;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 309) tStrength += 23;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 310) tStrength += 28;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 311) tStrength += 34;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 312) tStrength += 41;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 401) tStrength += 1;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 402) tStrength += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 403) tStrength += 6;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 404) tStrength += 10;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 405) tStrength += 15;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 406) tStrength += 21;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 407) tStrength += 28;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 408) tStrength += 36;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 409) tStrength += 45;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 410) tStrength += 55;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 411) tStrength += 67;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 412) tStrength += 82;

	// Halo System
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 1) tStrength += 1;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 2) tStrength += 2;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 3) tStrength += 3;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 4) tStrength += 4;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 5) tStrength += 5;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 6) tStrength += 6;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 7) tStrength += 7;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 8) tStrength += 8;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 9) tStrength += 9;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 10) tStrength += 10;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 11) tStrength += 11;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 12) tStrength += 12;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 13) tStrength += 13;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 14) tStrength += 14;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 15) tStrength += 15;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 16) tStrength += 16;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 17) tStrength += 17;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 18) tStrength += 18;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 19) tStrength += 19;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 20) tStrength += 20;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 21) tStrength += 21;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 22) tStrength += 22;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 23) tStrength += 23;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 24) tStrength += 24;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 25) tStrength += 25;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 26) tStrength += 26;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 27) tStrength += 27;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 28) tStrength += 28;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 29) tStrength += 29;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 30) tStrength += 30;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 31) tStrength += 31;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 32) tStrength += 32;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 33) tStrength += 33;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 34) tStrength += 34;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 35) tStrength += 35;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 36) tStrength += 36;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 37) tStrength += 37;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 38) tStrength += 38;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 39) tStrength += 39;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 40) tStrength += 40;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 41) tStrength += 41;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 42) tStrength += 42;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 43) tStrength += 43;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 44) tStrength += 44;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 45) tStrength += 45;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 46) tStrength += 46;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 47) tStrength += 47;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 48) tStrength += 48;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 49) tStrength += 49;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 50) tStrength += 50;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 51) tStrength += 51;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 52) tStrength += 52;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 53) tStrength += 53;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 54) tStrength += 54;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 55) tStrength += 55;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 56) tStrength += 56;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 57) tStrength += 57;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 58) tStrength += 58;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 59) tStrength += 59;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 60) tStrength += 60;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 61) tStrength += 61;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 62) tStrength += 62;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 63) tStrength += 63;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 64) tStrength += 64;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 65) tStrength += 65;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 66) tStrength += 66;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 67) tStrength += 67;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 68) tStrength += 68;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 69) tStrength += 69;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 70) tStrength += 70;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 71) tStrength += 71;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 72) tStrength += 72;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 73) tStrength += 73;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 74) tStrength += 74;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 75) tStrength += 75;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 76) tStrength += 76;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 77) tStrength += 77;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 78) tStrength += 78;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 79) tStrength += 79;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 80) tStrength += 80;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 81) tStrength += 81;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 82) tStrength += 82;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 83) tStrength += 83;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 84) tStrength += 84;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 85) tStrength += 85;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 86) tStrength += 86;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 87) tStrength += 87;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 88) tStrength += 88;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 89) tStrength += 89;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 90) tStrength += 90;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 91) tStrength += 91;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 92) tStrength += 92;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 93) tStrength += 93;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 94) tStrength += 94;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 95) tStrength += 95;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 96) tStrength += 96;

	// Halo System
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 1) tKi += 1;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 2) tKi += 2;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 3) tKi += 3;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 4) tKi += 4;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 5) tKi += 5;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 6) tKi += 6;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 7) tKi += 7;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 8) tKi += 8;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 9) tKi += 9;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 10) tKi += 10;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 11) tKi += 11;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 12) tKi += 12;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 13) tKi += 13;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 14) tKi += 14;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 15) tKi += 15;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 16) tKi += 16;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 17) tKi += 17;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 18) tKi += 18;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 19) tKi += 19;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 20) tKi += 20;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 21) tKi += 21;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 22) tKi += 22;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 23) tKi += 23;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 24) tKi += 24;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 25) tKi += 25;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 26) tKi += 26;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 27) tKi += 27;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 28) tKi += 28;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 29) tKi += 29;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 30) tKi += 30;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 31) tKi += 31;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 32) tKi += 32;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 33) tKi += 33;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 34) tKi += 34;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 35) tKi += 35;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 36) tKi += 36;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 37) tKi += 37;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 38) tKi += 38;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 39) tKi += 39;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 40) tKi += 40;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 41) tKi += 41;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 42) tKi += 42;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 43) tKi += 43;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 44) tKi += 44;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 45) tKi += 45;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 46) tKi += 46;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 47) tKi += 47;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 48) tKi += 48;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 49) tKi += 49;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 50) tKi += 50;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 51) tKi += 51;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 52) tKi += 52;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 53) tKi += 53;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 54) tKi += 54;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 55) tKi += 55;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 56) tKi += 56;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 57) tKi += 57;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 58) tKi += 58;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 59) tKi += 59;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 60) tKi += 60;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 61) tKi += 61;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 62) tKi += 62;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 63) tKi += 63;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 64) tKi += 64;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 65) tKi += 65;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 66) tKi += 66;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 67) tKi += 67;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 68) tKi += 68;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 69) tKi += 69;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 70) tKi += 70;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 71) tKi += 71;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 72) tKi += 72;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 73) tKi += 73;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 74) tKi += 74;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 75) tKi += 75;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 76) tKi += 76;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 77) tKi += 77;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 78) tKi += 78;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 79) tKi += 79;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 80) tKi += 80;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 81) tKi += 81;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 82) tKi += 82;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 83) tKi += 83;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 84) tKi += 84;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 85) tKi += 85;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 86) tKi += 86;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 87) tKi += 87;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 88) tKi += 88;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 89) tKi += 89;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 90) tKi += 90;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 91) tKi += 91;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 92) tKi += 92;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 93) tKi += 93;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 94) tKi += 94;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 95) tKi += 95;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 96) tKi += 96;

	for (index01 = 0; index01 < MAX_EQUIP_SLOT_NUM; index01++)
	{
		if (mEquipItem[index01] == nullptr)
		{
			continue;
		}
		tStrength += mEquipItem[index01]->iStrength;
		tKi += mEquipItem[index01]->iKi;
	}
	if (mEquipItem[7])
	{
		switch (mEquipItem[7]->iSort)
		{
		case 13:
		case 17:
		case 19:
			mAttackPower += static_cast<int>((float)tStrength * 2.65f);
			mAttackPower += static_cast<int>((float)tKi * 1.43f);
			break;
		case 14:
		case 16:
		case 20:
			mAttackPower += static_cast<int>((float)tStrength * 2.80f);
			mAttackPower += static_cast<int>((float)tKi * 1.51f);
			break;
		case 15:
		case 18:
		case 21:
			mAttackPower += static_cast<int>((float)tStrength * 2.51f);
			mAttackPower += static_cast<int>((float)tKi * 1.35f);
			break;
		}
	}
	else
	{
		mAttackPower += static_cast<int>((float)tStrength * 1.25f);
		mAttackPower += static_cast<int>((float)tKi * 0.67f);
	}

	if (mUSER[mIndex].mAvatarInfo.aRankAbility == 7) mAttackPower += 500;

	if (IsUsedOrnament() == 1) // 금장서의 경우
	{
		mAttackPower += 413;
	}
	else if (IsUsedOrnament() == 2)
	{
		mAttackPower += 275;
	}

	/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */
#ifdef __GOD__
	mAttackPower += mLEVEL.ReturnLevelFactorAttackPower((mDATA.aLevel1 + mDATA.aLevel2));
#else
	mAttackPower += mLEVEL.ReturnLevelFactorAttackPower(mDATA.aLevel1);
#endif

	for (index01 = 0; index01 < MAX_EQUIP_SLOT_NUM; index01++)
	{
		if (mEquipItem[index01] == nullptr)
		{
			continue;
		}
		mAttackPower += mEquipItem[index01]->iAttackPower;
		// Warlord Set
		// S2R
		if (mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87005) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87002) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87003) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87004))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 0.4000000059604645);
		if (mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87026) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87023) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87024) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87025))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 0.4000000059604645);
		if (mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87047) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87044) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87045) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87046))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 0.4000000059604645);
		// S4R
		if (mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87012) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87009) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87010) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87011))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 0.6000000238418579);
		if (mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87033) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87030) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87031) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87032))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 0.6000000238418579);
		if (mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87054) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87051) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87052) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87053))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 0.6000000238418579);
		// S5R
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87016) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87017) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87018) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87019) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87020)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87013) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87014) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87015))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 1.0);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87037) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87038) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87039) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87040) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87041)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87034) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87035) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87036))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 1.0);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87058) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87059) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87060) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87061) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87062)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87055) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87056) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87057))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 1.0);
		// S2E
		if (mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87068) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87065) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87066) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87067))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 0.4000000059604645);
		if (mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87090) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87087) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87088) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87089))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 0.4000000059604645);
		if (mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87112) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87109) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87110) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87111))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 0.4000000059604645);
		// S4E
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87074) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87075) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87076)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87071) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87072) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87073))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 0.6000000238418579);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87096) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87097) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87098)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87093) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87094) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87095))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 0.6000000238418579);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87118) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87119) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87120)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87115) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87116) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87117))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 0.6000000238418579);
		// S5E
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87080) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87081) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87082) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87083) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87084)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87077) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87078) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87079))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 1.0);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87102) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87103) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87104) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87105) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87106)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87099) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87100) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87101))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 1.0);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87124) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87125) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87126) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87127) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87128)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87121) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87122) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87123))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 1.0);
		// S2R V2
		if (mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87134) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87131) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87132) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87133))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 0.6000000238418579);
		if (mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87155) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87152) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87153) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87154))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 0.6000000238418579);
		if (mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87176) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87173) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87174) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87175))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 0.6000000238418579);
		// S4R V2
		if (mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87141) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87138) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87139) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87140))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 0.699999988079071);
		if (mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87162) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87159) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87160) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87161))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 0.699999988079071);
		if (mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87183) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87180) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87181) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87182))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 0.699999988079071);
		// S5R V2
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87145) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87146) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87147) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87148) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87149)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87142) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87143) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87144))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 1.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87166) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87167) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87168) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87169) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87170)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87163) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87164) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87165))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 1.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87187) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87188) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87189) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87190) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87191)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87184) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87185) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87186))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 1.100000023841858);
		// S2E V2
		if (mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87197) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87194) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87195) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87196))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 0.6000000238418579);
		if (mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87219) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87216) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87217) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87218))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 0.6000000238418579);
		if (mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87241) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87238) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87239) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87240))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 0.6000000238418579);
		// S4E V2
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87203) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87204) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87205)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87200) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87201) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87202))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 0.699999988079071);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87225) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87226) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87227)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87222) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87223) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87224))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 0.699999988079071);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87247) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87248) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87249)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87244) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87245) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87246))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 0.699999988079071);
		// S5E V2
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87209) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87210) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87211) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87212) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87213)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87206) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87207) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87208))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 1.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87231) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87232) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87233) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87234) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87235)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87228) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87229) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87230))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 1.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87253) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87254) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87255) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87256) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87257)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87250) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87251) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87252))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 1.100000023841858);
		// 88001 Set
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88004) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88005) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88006) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88007) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88008)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88001) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88002) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88003))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 2.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88012) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88013) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88014) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88015) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88016)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88009) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88010) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88011))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 2.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88020) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88021) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88022) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88023) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88024)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88017) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88018) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88019))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 2.100000023841858);
		// 88025 Set
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88028) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88029) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88030) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88031) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88032)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88025) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88026) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88027))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 3.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88036) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88037) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88038) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88039) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88040)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88033) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88034) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88035))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 3.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88044) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88045) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88046) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88047) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88048)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88041) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88042) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88043))
			mAttackPower += (static_cast<int>((double)mEquipItem[index01]->iAttackPower) * 3.100000023841858);


		if (index01 == 7)
		{
			tIUValue = mUTIL.ReturnIUValueWithLevelLimit(tAvatarInfo->aLevel1,
			                                             mUTIL.ReturnIUValue(mEquipValue[index01]));
			tIUEffectValue = mUTIL.ReturnIUEffectValue(mEquipItem[index01], 1);
			mAttackPower += (tIUValue * tIUEffectValue);
			tISValue = mUTIL.ReturnISValueWithLevelLimit(tAvatarInfo->aLevel1,
			                                             mUTIL.ReturnISValue(mEquipValue[index01]));
			if (tISValue > 0)
			{
				mAttackPower += static_cast<int>((float)(mEquipItem[index01]->iAttackPower + (tIUValue *
					tIUEffectValue)) * ((float)tISValue * 0.03f));
			}
		}
		if (index01 == 10)
		{
			switch (mEquipItem[index01]->iIndex)
			{
			case 201:
			case 202:
			case 203:
			case 204:
			case 205:
			case 206:
			case 207:
			case 208:
			case 209:
			case 210:
			case 211:
				tISValue = mUTIL.ReturnISValueWithLevelLimit(tAvatarInfo->aLevel1,
				                                             mUTIL.ReturnISValue(mEquipValue[index01]));
				mAttackPower += static_cast<int>((float)tISValue * 11.7f);
				break;
			case 213:
			case 214:
			case 215:
			case 216:
			case 217:
			case 218:
				tISValue = mUTIL.ReturnISValueWithLevelLimit(tAvatarInfo->aLevel1,
				                                             mUTIL.ReturnISValue(mEquipValue[index01]));
				mAttackPower += static_cast<int>((float)tISValue * 23.4f);
				break;
			}
		}
	}

	mAttackPower += (tAvatarInfo->aEatStrengthPotion * 3);

#ifdef __ANIMAL__
	if ((mDATA.aAnimalNumber == ANIMAL_NUM_BAKHO)
		|| (mDATA.aAnimalNumber == ANIMAL_NUM_FERRET1)
		|| (mDATA.aAnimalNumber == ANIMAL_NUM_BEAR1))
	{
		mAttackPower = static_cast<int>((float)mAttackPower * ANIMAL_RATE_ONE_GRADE);
	}
	if ((mDATA.aAnimalNumber == ANIMAL_NUM_CHUNGHO)
		|| (mDATA.aAnimalNumber == ANIMAL_NUM_FERRET2)
		|| (mDATA.aAnimalNumber == ANIMAL_NUM_BEAR2))
	{
		mAttackPower = static_cast<int>((float)mAttackPower * ANIMAL_RATE_TWO_GRADE);
	}
	if ((mDATA.aAnimalNumber == ANIMAL_NUM_CHUKHO)
		|| (mDATA.aAnimalNumber == ANIMAL_NUM_FERRET3)
		|| (mDATA.aAnimalNumber == ANIMAL_NUM_BLACKLEO)
		|| (mDATA.aAnimalNumber == ANIMAL_NUM_BEAR3))
	{
		mAttackPower = static_cast<int>((float)mAttackPower * ANIMAL_RATE_THREE_GRADE);
	}
	if (mDATA.aAnimalNumber == ANIMAL_NUM_BLUELEO)
	{
		mAttackPower = static_cast<int>((float)mAttackPower * ANIMAL_RATE_FOUR_GRADE);
	}
#endif
	// Damage Boost Scrolls
	if (mUSER[mIndex].mAvatarInfo.aDamageBoostScroll > 0)
	{
		mAttackPower *= 1.10f;
	}
	// Mount Ability
	if (mUSER[mIndex].mAvatarInfo.aAnimalIndex >= 10)
	{
		switch ((mUSER[mIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[mIndex].mAvatarInfo.aAnimalIndex - 10] % 100000000)
			/ 10000000)
		{
		case 1:
			mAttackPower += 50;
			break;
		case 2:
			mAttackPower += 100;
			break;
		case 3:
			mAttackPower += 150;
			break;
		case 4:
			mAttackPower += 200;
			break;
		case 5:
			mAttackPower += 250;
			break;
		case 6:
			mAttackPower += 300;
			break;
		case 7:
			mAttackPower += 350;
			break;
		case 8:
			mAttackPower += 400;
			break;
		case 9:
			mAttackPower += 450;
			break;
		}
	}
	// Socket Gem Stats
	for (index01 = 0; index01 < MAX_EQUIP_SLOT_NUM; index01++)
	{
		switch (index01)
		{
		case 0: //[Amulet]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 1)
			{
				mAttackPower += 25;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 6)
			{
				mAttackPower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 1)
			{
				mAttackPower += 25;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 6)
			{
				mAttackPower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 1)
			{
				mAttackPower += 25;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 6)
			{
				mAttackPower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 1)
			{
				mAttackPower += 25;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 6)
			{
				mAttackPower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 1)
			{
				mAttackPower += 25;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 6)
			{
				mAttackPower += 50;
			}
			break;
		case 1: //[Cape]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 1)
			{
				mAttackPower += 25;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 6)
			{
				mAttackPower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 1)
			{
				mAttackPower += 25;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 6)
			{
				mAttackPower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 1)
			{
				mAttackPower += 25;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 6)
			{
				mAttackPower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 1)
			{
				mAttackPower += 25;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 6)
			{
				mAttackPower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 1)
			{
				mAttackPower += 25;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 6)
			{
				mAttackPower += 50;
			}
			break;
		case 2: //[Armor]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 1)
			{
				mAttackPower += 25;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 6)
			{
				mAttackPower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 1)
			{
				mAttackPower += 25;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 6)
			{
				mAttackPower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 1)
			{
				mAttackPower += 25;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 6)
			{
				mAttackPower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 1)
			{
				mAttackPower += 25;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 6)
			{
				mAttackPower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 1)
			{
				mAttackPower += 25;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 6)
			{
				mAttackPower += 50;
			}
			break;
		case 3: //[Gloves]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 1)
			{
				mAttackPower += 25;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 6)
			{
				mAttackPower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 1)
			{
				mAttackPower += 25;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 6)
			{
				mAttackPower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 1)
			{
				mAttackPower += 25;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 6)
			{
				mAttackPower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 1)
			{
				mAttackPower += 25;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 6)
			{
				mAttackPower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 1)
			{
				mAttackPower += 25;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 6)
			{
				mAttackPower += 50;
			}
			break;
		case 4: //[Ring]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 1)
			{
				mAttackPower += 25;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 6)
			{
				mAttackPower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 1)
			{
				mAttackPower += 25;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 6)
			{
				mAttackPower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 1)
			{
				mAttackPower += 25;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 6)
			{
				mAttackPower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 1)
			{
				mAttackPower += 25;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 6)
			{
				mAttackPower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 1)
			{
				mAttackPower += 25;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 6)
			{
				mAttackPower += 50;
			}
			break;
		case 5: //[Boots]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 1)
			{
				mAttackPower += 25;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 6)
			{
				mAttackPower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 1)
			{
				mAttackPower += 25;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 6)
			{
				mAttackPower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 1)
			{
				mAttackPower += 25;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 6)
			{
				mAttackPower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 1)
			{
				mAttackPower += 25;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 6)
			{
				mAttackPower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 1)
			{
				mAttackPower += 25;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 6)
			{
				mAttackPower += 50;
			}
			break;
		case 7: //[Weapon]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 1)
			{
				mAttackPower += 25;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 6)
			{
				mAttackPower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 1)
			{
				mAttackPower += 25;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 6)
			{
				mAttackPower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 1)
			{
				mAttackPower += 25;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 6)
			{
				mAttackPower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 1)
			{
				mAttackPower += 25;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 6)
			{
				mAttackPower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 1)
			{
				mAttackPower += 25;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 6)
			{
				mAttackPower += 50;
			}
			break;
		}
	}

	mDefensePower = 0;
	tWisdom = tAvatarInfo->aDex;

	// When Mount is Absorbed
	tWisdom += mUTIL.GetAbsorbBonusStats(mIndex);
	/*
		if ((mUSER[mIndex].mAvatarInfo.aAbsorbState > 0) && (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber != 1329) && (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber != 1330) && (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber != 1331))
		{
			tWisdom += 10;
		}
		if ((mUSER[mIndex].mAvatarInfo.aAbsorbState > 0) && ((mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber == 1329) || (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber == 1330) || (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber == 1331)))
		{
			tWisdom += 15;
		}
	*/
	// Ninja Suit
	if ((mAVATAR_OBJECT[mIndex].mDATA.aFashionNumber == 385) || (mAVATAR_OBJECT[mIndex].mDATA.aFashionNumber == 386) ||
		(mAVATAR_OBJECT[mIndex].mDATA.aFashionNumber == 387))
	{
		tWisdom += 50;
	}
	for (index01 = 0; index01 < MAX_EQUIP_SLOT_NUM; index01++)
	{
		if (mEquipItem[index01] == nullptr)
		{
			continue;
		}
		tWisdom += mEquipItem[index01]->iWisdom;

		switch (index01)
		{
		// Refine System
		case 1: //Cape
			tRFValue = mUTIL.ReturnRFValue(mEquipValue[index01]);
			if (tRFValue > 0)
			{
				if (tRFValue == 1) tWisdom += 5;
				if (tRFValue == 2) tWisdom += 10;
				if (tRFValue == 3) tWisdom += 15;
				if (tRFValue == 4) tWisdom += 20;
				if (tRFValue == 5) tWisdom += 25;
				if (tRFValue == 6) tWisdom += 30;
				if (tRFValue == 7) tWisdom += 35;
				if (tRFValue == 8) tWisdom += 40;
				if (tRFValue == 9) tWisdom += 45;
				if (tRFValue == 10) tWisdom += 50;
				if (tRFValue == 11) tWisdom += 55;
				if (tRFValue == 12) tWisdom += 60;
				if (tRFValue == 13) tWisdom += 65;
				if (tRFValue == 14) tWisdom += 70;
				if (tRFValue == 15) tWisdom += 75;
				if (tRFValue == 16) tWisdom += 80;
				if (tRFValue == 17) tWisdom += 85;
				if (tRFValue == 18) tWisdom += 90;
				if (tRFValue == 19) tWisdom += 95;
				if (tRFValue == 20) tWisdom += 100;
				if (tRFValue == 21) tWisdom += 105;
				if (tRFValue == 22) tWisdom += 110;
				if (tRFValue == 23) tWisdom += 115;
				if (tRFValue == 24) tWisdom += 120;
				if (tRFValue == 25) tWisdom += 130;
			}
			break;

		case 3: // Gloves
			tRFValue = mUTIL.ReturnRFValue(mEquipValue[index01]);
			if (tRFValue > 0)
			{
				if (tRFValue == 1) tWisdom += 5;
				if (tRFValue == 2) tWisdom += 10;
				if (tRFValue == 3) tWisdom += 15;
				if (tRFValue == 4) tWisdom += 20;
				if (tRFValue == 5) tWisdom += 25;
				if (tRFValue == 6) tWisdom += 30;
				if (tRFValue == 7) tWisdom += 35;
				if (tRFValue == 8) tWisdom += 40;
				if (tRFValue == 9) tWisdom += 45;
				if (tRFValue == 10) tWisdom += 50;
				if (tRFValue == 11) tWisdom += 55;
				if (tRFValue == 12) tWisdom += 60;
				if (tRFValue == 13) tWisdom += 65;
				if (tRFValue == 14) tWisdom += 70;
				if (tRFValue == 15) tWisdom += 75;
				if (tRFValue == 16) tWisdom += 80;
				if (tRFValue == 17) tWisdom += 85;
				if (tRFValue == 18) tWisdom += 90;
				if (tRFValue == 19) tWisdom += 95;
				if (tRFValue == 20) tWisdom += 100;
				if (tRFValue == 21) tWisdom += 105;
				if (tRFValue == 22) tWisdom += 110;
				if (tRFValue == 23) tWisdom += 115;
				if (tRFValue == 24) tWisdom += 120;
				if (tRFValue == 25) tWisdom += 130;
			}
			break;

		case 5: // Boots
			tRFValue = mUTIL.ReturnRFValue(mEquipValue[index01]);
			if (tRFValue > 0)
			{
				if (tRFValue == 1) tWisdom += 5;
				if (tRFValue == 2) tWisdom += 10;
				if (tRFValue == 3) tWisdom += 15;
				if (tRFValue == 4) tWisdom += 20;
				if (tRFValue == 5) tWisdom += 25;
				if (tRFValue == 6) tWisdom += 30;
				if (tRFValue == 7) tWisdom += 35;
				if (tRFValue == 8) tWisdom += 40;
				if (tRFValue == 9) tWisdom += 45;
				if (tRFValue == 10) tWisdom += 50;
				if (tRFValue == 11) tWisdom += 55;
				if (tRFValue == 12) tWisdom += 60;
				if (tRFValue == 13) tWisdom += 65;
				if (tRFValue == 14) tWisdom += 70;
				if (tRFValue == 15) tWisdom += 75;
				if (tRFValue == 16) tWisdom += 80;
				if (tRFValue == 17) tWisdom += 85;
				if (tRFValue == 18) tWisdom += 90;
				if (tRFValue == 19) tWisdom += 95;
				if (tRFValue == 20) tWisdom += 100;
				if (tRFValue == 21) tWisdom += 105;
				if (tRFValue == 22) tWisdom += 110;
				if (tRFValue == 23) tWisdom += 115;
				if (tRFValue == 24) tWisdom += 120;
				if (tRFValue == 25) tWisdom += 130;
			}
			break;
		}
	}
	// Title System
	if (mUSER[mIndex].mAvatarInfo.aTitle == 1) tWisdom += 1;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 2) tWisdom += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 3) tWisdom += 6;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 4) tWisdom += 10;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 5) tWisdom += 15;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 6) tWisdom += 21;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 7) tWisdom += 28;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 8) tWisdom += 36;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 9) tWisdom += 45;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 10) tWisdom += 55;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 11) tWisdom += 67;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 12) tWisdom += 82;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 101) tWisdom += 1;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 102) tWisdom += 2;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 103) tWisdom += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 104) tWisdom += 5;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 105) tWisdom += 7;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 106) tWisdom += 10;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 107) tWisdom += 14;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 108) tWisdom += 18;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 109) tWisdom += 22;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 110) tWisdom += 27;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 111) tWisdom += 33;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 112) tWisdom += 41;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 201) tWisdom += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 202) tWisdom += 8;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 203) tWisdom += 15;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 204) tWisdom += 25;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 205) tWisdom += 37;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 206) tWisdom += 52;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 207) tWisdom += 70;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 208) tWisdom += 90;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 209) tWisdom += 112;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 210) tWisdom += 137;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 211) tWisdom += 167;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 212) tWisdom += 205;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 301) tWisdom += 1;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 302) tWisdom += 2;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 303) tWisdom += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 304) tWisdom += 5;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 305) tWisdom += 7;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 306) tWisdom += 10;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 307) tWisdom += 14;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 308) tWisdom += 18;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 309) tWisdom += 22;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 310) tWisdom += 27;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 311) tWisdom += 33;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 312) tWisdom += 41;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 401) tWisdom += 1;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 402) tWisdom += 2;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 403) tWisdom += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 404) tWisdom += 5;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 405) tWisdom += 7;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 406) tWisdom += 10;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 407) tWisdom += 14;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 408) tWisdom += 18;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 409) tWisdom += 22;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 410) tWisdom += 27;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 411) tWisdom += 33;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 412) tWisdom += 41;

	// Halo System
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 1) tWisdom += 1;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 2) tWisdom += 2;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 3) tWisdom += 3;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 4) tWisdom += 4;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 5) tWisdom += 5;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 6) tWisdom += 6;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 7) tWisdom += 7;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 8) tWisdom += 8;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 9) tWisdom += 9;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 10) tWisdom += 10;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 11) tWisdom += 11;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 12) tWisdom += 12;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 13) tWisdom += 13;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 14) tWisdom += 14;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 15) tWisdom += 15;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 16) tWisdom += 16;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 17) tWisdom += 17;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 18) tWisdom += 18;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 19) tWisdom += 19;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 20) tWisdom += 20;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 21) tWisdom += 21;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 22) tWisdom += 22;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 23) tWisdom += 23;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 24) tWisdom += 24;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 25) tWisdom += 25;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 26) tWisdom += 26;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 27) tWisdom += 27;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 28) tWisdom += 28;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 29) tWisdom += 29;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 30) tWisdom += 30;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 31) tWisdom += 31;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 32) tWisdom += 32;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 33) tWisdom += 33;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 34) tWisdom += 34;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 35) tWisdom += 35;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 36) tWisdom += 36;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 37) tWisdom += 37;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 38) tWisdom += 38;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 39) tWisdom += 39;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 40) tWisdom += 40;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 41) tWisdom += 41;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 42) tWisdom += 42;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 43) tWisdom += 43;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 44) tWisdom += 44;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 45) tWisdom += 45;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 46) tWisdom += 46;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 47) tWisdom += 47;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 48) tWisdom += 48;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 49) tWisdom += 49;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 50) tWisdom += 50;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 51) tWisdom += 51;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 52) tWisdom += 52;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 53) tWisdom += 53;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 54) tWisdom += 54;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 55) tWisdom += 55;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 56) tWisdom += 56;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 57) tWisdom += 57;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 58) tWisdom += 58;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 59) tWisdom += 59;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 60) tWisdom += 60;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 61) tWisdom += 61;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 62) tWisdom += 62;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 63) tWisdom += 63;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 64) tWisdom += 64;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 65) tWisdom += 65;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 66) tWisdom += 66;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 67) tWisdom += 67;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 68) tWisdom += 68;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 69) tWisdom += 69;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 70) tWisdom += 70;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 71) tWisdom += 71;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 72) tWisdom += 72;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 73) tWisdom += 73;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 74) tWisdom += 74;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 75) tWisdom += 75;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 76) tWisdom += 76;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 77) tWisdom += 77;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 78) tWisdom += 78;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 79) tWisdom += 79;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 80) tWisdom += 80;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 81) tWisdom += 81;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 82) tWisdom += 82;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 83) tWisdom += 83;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 84) tWisdom += 84;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 85) tWisdom += 85;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 86) tWisdom += 86;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 87) tWisdom += 87;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 88) tWisdom += 88;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 89) tWisdom += 89;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 90) tWisdom += 90;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 91) tWisdom += 91;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 92) tWisdom += 92;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 93) tWisdom += 93;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 94) tWisdom += 94;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 95) tWisdom += 95;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 96) tWisdom += 96;

	mDefensePower += static_cast<int>((float)tWisdom * 1.63f);

	/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */
	// 장식 해방 시스템이 사용중인 경우 방어력 증가.
	if (IsUsedOrnament() == 1)
	{
		mDefensePower += 825;
	}
	else if (IsUsedOrnament() == 2)
	{
		mDefensePower += 550;
	}
	/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */

	// Rank Ability
	if (mUSER[mIndex].mAvatarInfo.aRankAbility == 1) mDefensePower += 1000;

#ifdef __GOD__
	mDefensePower += mLEVEL.ReturnLevelFactorDefensePower((mDATA.aLevel1 + mDATA.aLevel2));
#else
	mDefensePower += mLEVEL.ReturnLevelFactorDefensePower(mDATA.aLevel1);
#endif
	for (index01 = 0; index01 < MAX_EQUIP_SLOT_NUM; index01++)
	{
		if (mEquipItem[index01] == nullptr)
		{
			continue;
		}
		mDefensePower += mEquipItem[index01]->iDefensePower;
		//Warlord Set
		// S2R
		if (mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87005) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87002) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87003) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87004))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 0.4000000059604645);
		if (mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87026) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87023) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87024) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87025))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 0.4000000059604645);
		if (mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87047) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87044) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87045) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87046))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 0.4000000059604645);
		// S3R
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87006) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87007) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87008)) mDefensePower += (static_cast<int>((double)mEquipItem[
			index01]->iDefensePower) * 0.6000000238418579);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87027) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87028) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87029)) mDefensePower += (static_cast<int>((double)mEquipItem[
			index01]->iDefensePower) * 0.6000000238418579);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87048) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87049) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87050)) mDefensePower += (static_cast<int>((double)mEquipItem[
			index01]->iDefensePower) * 0.6000000238418579);
		// S5R
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87016) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87017) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87018) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87019) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87020)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87013) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87014) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87015))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 1.0);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87037) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87038) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87039) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87040) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87041)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87034) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87035) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87036))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 1.0);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87058) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87059) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87060) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87061) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87062)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87055) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87056) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87057))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 1.0);
		// S2E
		if (mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87068) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87065) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87066) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87067))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 0.4000000059604645);
		if (mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87090) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87087) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87088) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87089))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 0.4000000059604645);
		if (mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87112) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87109) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87110) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87111))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 0.4000000059604645);
		// S4E
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87074) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87075) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87076)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87071) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87072) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87073))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 0.6000000238418579);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87096) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87097) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87098)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87093) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87094) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87095))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 0.6000000238418579);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87118) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87119) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87120)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87115) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87116) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87117))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 0.6000000238418579);
		// S5E
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87080) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87081) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87082) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87083) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87084)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87077) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87078) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87079))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 1.0);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87102) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87103) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87104) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87105) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87106)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87099) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87100) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87101))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 1.0);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87124) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87125) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87126) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87127) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87128)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87121) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87122) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87123))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 1.0);
		// S2R V2
		if (mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87134) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87131) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87132) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87133))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 0.6000000238418579);
		if (mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87155) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87152) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87153) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87154))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 0.6000000238418579);
		if (mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87176) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87173) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87174) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87175))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 0.6000000238418579);
		// S3R V2
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87135) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87136) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87137)) mDefensePower += (static_cast<int>((double)mEquipItem[
			index01]->iDefensePower) * 0.699999988079071);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87156) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87157) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87158)) mDefensePower += (static_cast<int>((double)mEquipItem[
			index01]->iDefensePower) * 0.699999988079071);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87177) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87178) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87179)) mDefensePower += (static_cast<int>((double)mEquipItem[
			index01]->iDefensePower) * 0.699999988079071);
		// S5R V2
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87145) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87146) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87147) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87148) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87149)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87142) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87143) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87144))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 1.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87166) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87167) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87168) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87169) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87170)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87163) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87164) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87165))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 1.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87187) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87188) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87189) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87190) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87191)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87184) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87185) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87186))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 1.100000023841858);
		// S2E V2
		if (mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87197) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87194) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87195) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87196))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 0.6000000238418579);
		if (mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87219) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87216) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87217) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87218))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 0.6000000238418579);
		if (mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87241) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87238) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87239) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87240))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 0.6000000238418579);
		// S4E V2
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87203) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87204) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87205)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87200) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87201) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87202))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 0.699999988079071);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87225) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87226) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87227)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87222) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87223) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87224))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 0.699999988079071);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87247) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87248) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87249)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87244) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87245) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87246))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 0.699999988079071);
		// S5E V2
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87209) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87210) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87211) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87212) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87213)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87206) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87207) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87208))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 1.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87231) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87232) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87233) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87234) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87235)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87228) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87229) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87230))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 1.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87253) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87254) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87255) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87256) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87257)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87250) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87251) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87252))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 1.100000023841858);
		// 88001 Set
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88004) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88005) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88006) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88007) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88008)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88001) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88002) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88003))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 2.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88012) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88013) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88014) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88015) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88016)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88009) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88010) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88011))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 2.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88020) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88021) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88022) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88023) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88024)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88017) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88018) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88019))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 2.100000023841858);
		// 88025 Set
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88028) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88029) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88030) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88031) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88032)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88025) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88026) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88027))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 3.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88036) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88037) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88038) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88039) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88040)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88033) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88034) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88035))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 3.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88044) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88045) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88046) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88047) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88048)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88041) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88042) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88043))
			mDefensePower += (static_cast<int>((double)mEquipItem[index01]->iDefensePower) * 3.100000023841858);

		switch (index01)
		{
		case 1:
			tIUValue = mUTIL.ReturnIUValueWithLevelLimit(tAvatarInfo->aLevel1,
			                                             mUTIL.ReturnIUValue(mEquipValue[index01]));
			tIUEffectValue = mUTIL.ReturnIUEffectValue(mEquipItem[index01], 2);
			mDefensePower += (tIUValue * tIUEffectValue);
			tISValue = mUTIL.ReturnISValueWithLevelLimit(tAvatarInfo->aLevel1,
			                                             mUTIL.ReturnISValue(mEquipValue[index01]));
			if (tISValue > 0)
			{
				mDefensePower += (tISValue * 6);
			}
			break;
		case 2:
			tIUValue = mUTIL.ReturnIUValueWithLevelLimit(tAvatarInfo->aLevel1,
			                                             mUTIL.ReturnIUValue(mEquipValue[index01]));
			tIUEffectValue = mUTIL.ReturnIUEffectValue(mEquipItem[index01], 2);
			mDefensePower += (tIUValue * tIUEffectValue);
			tISValue = mUTIL.ReturnISValueWithLevelLimit(tAvatarInfo->aLevel1,
			                                             mUTIL.ReturnISValue(mEquipValue[index01]));
			if (tISValue > 0)
			{
				mDefensePower += static_cast<int>((float)(mEquipItem[index01]->iDefensePower + (tIUValue *
					tIUEffectValue)) * ((float)tISValue * 0.03f));
			}
			break;
		case 3:
			tIUValue = mUTIL.ReturnIUValueWithLevelLimit(tAvatarInfo->aLevel1,
			                                             mUTIL.ReturnIUValue(mEquipValue[index01]));
			tIUEffectValue = mUTIL.ReturnIUEffectValue(mEquipItem[index01], 2);
			mDefensePower += (tIUValue * tIUEffectValue);
			break;
		case 5:
			tIUValue = mUTIL.ReturnIUValueWithLevelLimit(tAvatarInfo->aLevel1,
			                                             mUTIL.ReturnIUValue(mEquipValue[index01]));
			tIUEffectValue = mUTIL.ReturnIUEffectValue(mEquipItem[index01], 2);
			mDefensePower += (tIUValue * tIUEffectValue);
			break;
		case 10:
			switch (mEquipItem[index01]->iIndex)
			{
			case 201:
			case 202:
			case 203:
			case 207:
			case 208:
			case 209:
			case 210:
			case 211:
			case 212:
			case 213:
			case 214:
			case 215:
				tISValue = mUTIL.ReturnISValueWithLevelLimit(tAvatarInfo->aLevel1,
				                                             mUTIL.ReturnISValue(mEquipValue[index01]));
				mDefensePower += static_cast<int>((float)tISValue * 24.35f);
				break;

			case 204:
			case 205:
			case 206:
			case 216:
			case 217:
			case 218:
				tISValue = mUTIL.ReturnISValueWithLevelLimit(tAvatarInfo->aLevel1,
				                                             mUTIL.ReturnISValue(mEquipValue[index01]));
				mDefensePower += static_cast<int>((float)tISValue * 48.75f);
				break;
			}
			break;
		}
	}

#ifdef __ANIMAL__

	if (mDATA.aAnimalNumber == ANIMAL_NUM_HAECHI1)
	{
		mDefensePower = static_cast<int>((float)mDefensePower * ANIMAL_RATE_ONE_GRADE);
	}
	if (mDATA.aAnimalNumber == ANIMAL_NUM_HAECHI2)
	{
		mDefensePower = static_cast<int>((float)mDefensePower * ANIMAL_RATE_TWO_GRADE);
	}
	if (mDATA.aAnimalNumber == ANIMAL_NUM_HAECHI3)
	{
		mDefensePower = static_cast<int>((float)mDefensePower * ANIMAL_RATE_THREE_GRADE);
	}
#endif
	// Mount Ability
	if (mUSER[mIndex].mAvatarInfo.aAnimalIndex >= 10)
	{
		switch ((mUSER[mIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[mIndex].mAvatarInfo.aAnimalIndex - 10] % 10000000)
			/ 1000000)
		{
		case 1:
			mDefensePower += 100;
			break;
		case 2:
			mDefensePower += 200;
			break;
		case 3:
			mDefensePower += 300;
			break;
		case 4:
			mDefensePower += 400;
			break;
		case 5:
			mDefensePower += 500;
			break;
		case 6:
			mDefensePower += 600;
			break;
		case 7:
			mDefensePower += 700;
			break;
		case 8:
			mDefensePower += 800;
			break;
		case 9:
			mDefensePower += 900;
			break;
		}
	}
	// Socket Gem Stats
	for (index01 = 0; index01 < MAX_EQUIP_SLOT_NUM; index01++)
	{
		switch (index01)
		{
		case 0: //[Amulet]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 2)
			{
				mDefensePower += 50;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 7)
			{
				mDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 2)
			{
				mDefensePower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 7)
			{
				mDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 2)
			{
				mDefensePower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 7)
			{
				mDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 2)
			{
				mDefensePower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 7)
			{
				mDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 2)
			{
				mDefensePower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 7)
			{
				mDefensePower += 100;
			}
			break;
		case 1: //[Cape]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 2)
			{
				mDefensePower += 50;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 7)
			{
				mDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 2)
			{
				mDefensePower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 7)
			{
				mDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 2)
			{
				mDefensePower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 7)
			{
				mDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 2)
			{
				mDefensePower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 7)
			{
				mDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 2)
			{
				mDefensePower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 7)
			{
				mDefensePower += 100;
			}
			break;
		case 2: //[Armor]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 2)
			{
				mDefensePower += 50;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 7)
			{
				mDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 2)
			{
				mDefensePower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 7)
			{
				mDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 2)
			{
				mDefensePower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 7)
			{
				mDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 2)
			{
				mDefensePower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 7)
			{
				mDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 2)
			{
				mDefensePower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 7)
			{
				mDefensePower += 100;
			}
			break;
		case 3: //[Gloves]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 2)
			{
				mDefensePower += 50;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 7)
			{
				mDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 2)
			{
				mDefensePower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 7)
			{
				mDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 2)
			{
				mDefensePower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 7)
			{
				mDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 2)
			{
				mDefensePower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 7)
			{
				mDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 2)
			{
				mDefensePower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 7)
			{
				mDefensePower += 100;
			}
			break;
		case 4: //[Ring]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 2)
			{
				mDefensePower += 50;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 7)
			{
				mDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 2)
			{
				mDefensePower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 7)
			{
				mDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 2)
			{
				mDefensePower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 7)
			{
				mDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 2)
			{
				mDefensePower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 7)
			{
				mDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 2)
			{
				mDefensePower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 7)
			{
				mDefensePower += 100;
			}
			break;
		case 5: //[Boots]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 2)
			{
				mDefensePower += 50;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 7)
			{
				mDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 2)
			{
				mDefensePower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 7)
			{
				mDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 2)
			{
				mDefensePower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 7)
			{
				mDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 2)
			{
				mDefensePower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 7)
			{
				mDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 2)
			{
				mDefensePower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 7)
			{
				mDefensePower += 100;
			}
			break;
		case 7: //[Weapon]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 2)
			{
				mDefensePower += 50;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 7)
			{
				mDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 2)
			{
				mDefensePower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 7)
			{
				mDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 2)
			{
				mDefensePower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 7)
			{
				mDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 2)
			{
				mDefensePower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 7)
			{
				mDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 2)
			{
				mDefensePower += 50;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 7)
			{
				mDefensePower += 100;
			}
			break;
		}
	}

	mAttackSuccess = 0;
	tStrength = tAvatarInfo->aStr;

	// When Mount is Absorbed
	tStrength += mUTIL.GetAbsorbBonusStats(mIndex);
	/*
		if ((mUSER[mIndex].mAvatarInfo.aAbsorbState > 0) && (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber != 1329) && (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber != 1330) && (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber != 1331))
		{
			tStrength += 10;
		}
		if ((mUSER[mIndex].mAvatarInfo.aAbsorbState > 0) && ((mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber == 1329) || (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber == 1330) || (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber == 1331)))
		{
			tStrength += 15;
		}
	*/
	for (index01 = 0; index01 < MAX_EQUIP_SLOT_NUM; index01++)
	{
		if (mEquipItem[index01] == nullptr)
		{
			continue;
		}
		tStrength += mEquipItem[index01]->iStrength;

		//Refine System
		switch (index01)
		{
		case 0: // Amulet
			tRFValue = mUTIL.ReturnRFValue(mEquipValue[index01]);
			if (tRFValue > 0)
			{
				if (tRFValue == 1) tStrength += 5;
				if (tRFValue == 2) tStrength += 10;
				if (tRFValue == 3) tStrength += 15;
				if (tRFValue == 4) tStrength += 20;
				if (tRFValue == 5) tStrength += 25;
				if (tRFValue == 6) tStrength += 30;
				if (tRFValue == 7) tStrength += 35;
				if (tRFValue == 8) tStrength += 40;
				if (tRFValue == 9) tStrength += 45;
				if (tRFValue == 10) tStrength += 50;
				if (tRFValue == 11) tStrength += 55;
				if (tRFValue == 12) tStrength += 60;
				if (tRFValue == 13) tStrength += 65;
				if (tRFValue == 14) tStrength += 70;
				if (tRFValue == 15) tStrength += 75;
				if (tRFValue == 16) tStrength += 80;
				if (tRFValue == 17) tStrength += 85;
				if (tRFValue == 18) tStrength += 90;
				if (tRFValue == 19) tStrength += 95;
				if (tRFValue == 20) tStrength += 100;
				if (tRFValue == 21) tStrength += 105;
				if (tRFValue == 22) tStrength += 110;
				if (tRFValue == 23) tStrength += 115;
				if (tRFValue == 24) tStrength += 120;
				if (tRFValue == 25) tStrength += 130;
			}
			break;
		}
	}
	// Title System
	if (mUSER[mIndex].mAvatarInfo.aTitle == 1) tStrength += 1;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 2) tStrength += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 3) tStrength += 6;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 4) tStrength += 10;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 5) tStrength += 15;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 6) tStrength += 21;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 7) tStrength += 28;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 8) tStrength += 36;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 9) tStrength += 45;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 10) tStrength += 55;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 11) tStrength += 67;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 12) tStrength += 82;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 101) tStrength += 2;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 102) tStrength += 6;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 103) tStrength += 12;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 104) tStrength += 20;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 105) tStrength += 30;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 106) tStrength += 42;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 107) tStrength += 56;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 108) tStrength += 72;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 109) tStrength += 99;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 110) tStrength += 110;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 111) tStrength += 134;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 112) tStrength += 164;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 201) tStrength += 1;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 202) tStrength += 2;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 203) tStrength += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 204) tStrength += 5;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 205) tStrength += 7;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 206) tStrength += 10;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 207) tStrength += 14;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 208) tStrength += 18;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 209) tStrength += 22;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 210) tStrength += 27;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 211) tStrength += 33;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 212) tStrength += 41;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 301) tStrength += 0;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 302) tStrength += 1;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 303) tStrength += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 304) tStrength += 5;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 305) tStrength += 8;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 306) tStrength += 11;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 307) tStrength += 14;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 308) tStrength += 18;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 309) tStrength += 23;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 310) tStrength += 28;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 311) tStrength += 34;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 312) tStrength += 41;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 401) tStrength += 1;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 402) tStrength += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 403) tStrength += 6;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 404) tStrength += 10;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 405) tStrength += 15;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 406) tStrength += 21;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 407) tStrength += 28;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 408) tStrength += 36;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 409) tStrength += 45;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 410) tStrength += 55;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 411) tStrength += 67;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 412) tStrength += 82;

	// Halo System
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 1) tStrength += 1;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 2) tStrength += 2;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 3) tStrength += 3;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 4) tStrength += 4;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 5) tStrength += 5;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 6) tStrength += 6;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 7) tStrength += 7;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 8) tStrength += 8;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 9) tStrength += 9;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 10) tStrength += 10;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 11) tStrength += 11;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 12) tStrength += 12;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 13) tStrength += 13;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 14) tStrength += 14;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 15) tStrength += 15;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 16) tStrength += 16;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 17) tStrength += 17;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 18) tStrength += 18;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 19) tStrength += 19;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 20) tStrength += 20;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 21) tStrength += 21;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 22) tStrength += 22;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 23) tStrength += 23;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 24) tStrength += 24;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 25) tStrength += 25;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 26) tStrength += 26;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 27) tStrength += 27;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 28) tStrength += 28;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 29) tStrength += 29;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 30) tStrength += 30;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 31) tStrength += 31;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 32) tStrength += 32;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 33) tStrength += 33;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 34) tStrength += 34;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 35) tStrength += 35;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 36) tStrength += 36;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 37) tStrength += 37;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 38) tStrength += 38;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 39) tStrength += 39;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 40) tStrength += 40;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 41) tStrength += 41;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 42) tStrength += 42;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 43) tStrength += 43;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 44) tStrength += 44;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 45) tStrength += 45;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 46) tStrength += 46;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 47) tStrength += 47;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 48) tStrength += 48;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 49) tStrength += 49;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 50) tStrength += 50;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 51) tStrength += 51;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 52) tStrength += 52;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 53) tStrength += 53;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 54) tStrength += 54;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 55) tStrength += 55;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 56) tStrength += 56;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 57) tStrength += 57;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 58) tStrength += 58;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 59) tStrength += 59;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 60) tStrength += 60;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 61) tStrength += 61;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 62) tStrength += 62;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 63) tStrength += 63;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 64) tStrength += 64;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 65) tStrength += 65;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 66) tStrength += 66;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 67) tStrength += 67;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 68) tStrength += 68;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 69) tStrength += 69;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 70) tStrength += 70;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 71) tStrength += 71;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 72) tStrength += 72;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 73) tStrength += 73;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 74) tStrength += 74;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 75) tStrength += 75;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 76) tStrength += 76;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 77) tStrength += 77;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 78) tStrength += 78;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 79) tStrength += 79;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 80) tStrength += 80;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 81) tStrength += 81;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 82) tStrength += 82;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 83) tStrength += 83;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 84) tStrength += 84;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 85) tStrength += 85;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 86) tStrength += 86;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 87) tStrength += 87;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 88) tStrength += 88;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 89) tStrength += 89;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 90) tStrength += 90;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 91) tStrength += 91;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 92) tStrength += 92;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 93) tStrength += 93;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 94) tStrength += 94;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 95) tStrength += 95;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 96) tStrength += 96;

	mAttackSuccess += static_cast<int>((float)tStrength * 1.71f);

	if (mUSER[mIndex].mAvatarInfo.aRankAbility == 5) mAttackSuccess += 1000;
#ifdef __GOD__
	mAttackSuccess += mLEVEL.ReturnLevelFactorAttackSuccess((mDATA.aLevel1 + mDATA.aLevel2));
#else
	mAttackSuccess += mLEVEL.ReturnLevelFactorAttackSuccess(mDATA.aLevel1);
#endif
	for (index01 = 0; index01 < MAX_EQUIP_SLOT_NUM; index01++)
	{
		if (mEquipItem[index01] == nullptr)
		{
			continue;
		}
		mAttackSuccess += mEquipItem[index01]->iAttackSucess;
		//Warlord Set
		// S3R
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87006) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87007) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87008)) mAttackSuccess += (static_cast<int>((double)mEquipItem[
			index01]->iAttackSucess) * 0.6000000238418579);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87027) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87028) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87029)) mAttackSuccess += (static_cast<int>((double)mEquipItem[
			index01]->iAttackSucess) * 0.6000000238418579);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87048) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87049) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87050)) mAttackSuccess += (static_cast<int>((double)mEquipItem[
			index01]->iAttackSucess) * 0.6000000238418579);
		// S5R
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87016) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87017) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87018) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87019) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87020)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87013) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87014) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87015))
			mAttackSuccess += (static_cast<int>((double)mEquipItem[index01]->iAttackSucess) * 1.0);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87037) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87038) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87039) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87040) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87041)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87034) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87035) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87036))
			mAttackSuccess += (static_cast<int>((double)mEquipItem[index01]->iAttackSucess) * 1.0);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87058) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87059) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87060) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87061) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87062)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87055) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87056) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87057))
			mAttackSuccess += (static_cast<int>((double)mEquipItem[index01]->iAttackSucess) * 1.0);
		// S3E
		if ((mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87069) && (mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87070))
			mAttackSuccess += (static_cast<int>((double)mEquipItem[index01]->iAttackSucess) * 0.4000000059604645);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87091) && (mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87092))
			mAttackSuccess += (static_cast<int>((double)mEquipItem[index01]->iAttackSucess) * 0.4000000059604645);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87113) && (mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87114))
			mAttackSuccess += (static_cast<int>((double)mEquipItem[index01]->iAttackSucess) * 0.4000000059604645);
		// S4E
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87074) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87075) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87076)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87071) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87072) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87073))
			mAttackSuccess += (static_cast<int>((double)mEquipItem[index01]->iAttackSucess) * 0.6000000238418579);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87096) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87097) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87098)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87093) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87094) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87095))
			mAttackSuccess += (static_cast<int>((double)mEquipItem[index01]->iAttackSucess) * 0.6000000238418579);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87118) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87119) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87120)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87115) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87116) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87117))
			mAttackSuccess += (static_cast<int>((double)mEquipItem[index01]->iAttackSucess) * 0.6000000238418579);
		// S5E
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87080) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87081) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87082) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87083) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87084)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87077) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87078) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87079))
			mAttackSuccess += (static_cast<int>((double)mEquipItem[index01]->iAttackSucess) * 1.0);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87102) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87103) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87104) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87105) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87106)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87099) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87100) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87101))
			mAttackSuccess += (static_cast<int>((double)mEquipItem[index01]->iAttackSucess) * 1.0);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87124) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87125) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87126) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87127) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87128)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87121) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87122) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87123))
			mAttackSuccess += (static_cast<int>((double)mEquipItem[index01]->iAttackSucess) * 1.0);
		// S3R V2
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87135) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87136) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87137)) mAttackSuccess += (static_cast<int>((double)mEquipItem[
			index01]->iAttackSucess) * 0.699999988079071);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87156) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87157) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87158)) mAttackSuccess += (static_cast<int>((double)mEquipItem[
			index01]->iAttackSucess) * 0.699999988079071);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87177) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87178) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87179)) mAttackSuccess += (static_cast<int>((double)mEquipItem[
			index01]->iAttackSucess) * 0.699999988079071);
		// S5R V2
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87145) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87146) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87147) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87148) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87149)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87142) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87143) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87144))
			mAttackSuccess += (static_cast<int>((double)mEquipItem[index01]->iAttackSucess) * 1.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87166) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87167) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87168) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87169) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87170)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87163) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87164) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87165))
			mAttackSuccess += (static_cast<int>((double)mEquipItem[index01]->iAttackSucess) * 1.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87187) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87188) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87189) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87190) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87191)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87184) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87185) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87186))
			mAttackSuccess += (static_cast<int>((double)mEquipItem[index01]->iAttackSucess) * 1.100000023841858);
		// S3E V2
		if ((mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87198) && (mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87199))
			mAttackSuccess += (static_cast<int>((double)mEquipItem[index01]->iAttackSucess) * 0.699999988079071);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87220) && (mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87221))
			mAttackSuccess += (static_cast<int>((double)mEquipItem[index01]->iAttackSucess) * 0.699999988079071);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87242) && (mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87243))
			mAttackSuccess += (static_cast<int>((double)mEquipItem[index01]->iAttackSucess) * 0.699999988079071);
		// S4E V2
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87203) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87204) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87205)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87200) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87201) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87202))
			mAttackSuccess += (static_cast<int>((double)mEquipItem[index01]->iAttackSucess) * 0.699999988079071);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87225) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87226) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87227)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87222) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87223) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87224))
			mAttackSuccess += (static_cast<int>((double)mEquipItem[index01]->iAttackSucess) * 0.699999988079071);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87247) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87248) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87249)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87244) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87245) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87246))
			mAttackSuccess += (static_cast<int>((double)mEquipItem[index01]->iAttackSucess) * 0.699999988079071);
		// S5E V2
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87209) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87210) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87211) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87212) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87213)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87206) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87207) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87208))
			mAttackSuccess += (static_cast<int>((double)mEquipItem[index01]->iAttackSucess) * 1.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87231) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87232) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87233) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87234) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87235)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87228) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87229) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87230))
			mAttackSuccess += (static_cast<int>((double)mEquipItem[index01]->iAttackSucess) * 1.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87253) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87254) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87255) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87256) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87257)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87250) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87251) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87252))
			mAttackSuccess += (static_cast<int>((double)mEquipItem[index01]->iAttackSucess) * 1.100000023841858);
		// 88001 Set
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88004) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88005) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88006) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88007) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88008)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88001) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88002) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88003))
			mAttackSuccess += (static_cast<int>((double)mEquipItem[index01]->iAttackSucess) * 2.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88012) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88013) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88014) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88015) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88016)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88009) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88010) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88011))
			mAttackSuccess += (static_cast<int>((double)mEquipItem[index01]->iAttackSucess) * 2.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88020) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88021) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88022) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88023) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88024)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88017) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88018) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88019))
			mAttackSuccess += (static_cast<int>((double)mEquipItem[index01]->iAttackSucess) * 2.100000023841858);
		// 88025 Set
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88028) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88029) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88030) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88031) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88032)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88025) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88026) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88027))
			mAttackSuccess += (static_cast<int>((double)mEquipItem[index01]->iAttackSucess) * 3.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88036) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88037) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88038) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88039) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88040)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88033) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88034) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88035))
			mAttackSuccess += (static_cast<int>((double)mEquipItem[index01]->iAttackSucess) * 3.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88044) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88045) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88046) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88047) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88048)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88041) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88042) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88043))
			mAttackSuccess += (static_cast<int>((double)mEquipItem[index01]->iAttackSucess) * 3.100000023841858);


		switch (index01)
		{
		case 3:
			tIUValue = mUTIL.ReturnIUValueWithLevelLimit(tAvatarInfo->aLevel1,
			                                             mUTIL.ReturnIUValue(mEquipValue[index01]));
			tIUEffectValue = mUTIL.ReturnIUEffectValue(mEquipItem[index01], 3);
			mAttackSuccess += (tIUValue * tIUEffectValue);
			tISValue = mUTIL.ReturnISValueWithLevelLimit(tAvatarInfo->aLevel1,
			                                             mUTIL.ReturnISValue(mEquipValue[index01]));
			if (tISValue > 0)
			{
				mAttackSuccess += static_cast<int>((float)(mEquipItem[index01]->iAttackSucess + (tIUValue *
					tIUEffectValue)) * ((float)tISValue * 0.03f));
			}
			break;
		case 7:
			tIUValue = mUTIL.ReturnIUValueWithLevelLimit(tAvatarInfo->aLevel1,
			                                             mUTIL.ReturnIUValue(mEquipValue[index01]));
			tIUEffectValue = mUTIL.ReturnIUEffectValue(mEquipItem[index01], 3);
			mAttackSuccess += (tIUValue * tIUEffectValue);
			break;
		}
	}
	//Original Korea code is not useable in battle zone.
#if 0
	if ((!mGAME.mCheckZone049TypeServer) && (!mGAME.mCheckZone051TypeServer) && (!mGAME.mCheckZone053TypeServer))
	{
		mAttackSuccess += (tAvatarInfo->aEatWisdomPotion * 2);
	}
#endif
	mAttackSuccess += (tAvatarInfo->aEatAgilityPotion * 2);
#ifdef __ANIMAL__
	if (mDATA.aAnimalNumber == ANIMAL_NUM_BEAR1)
	{
		mAttackSuccess = static_cast<int>((float)mAttackSuccess * ANIMAL_RATE_ONE_GRADE);
	}
	if (mDATA.aAnimalNumber == ANIMAL_NUM_BEAR2)
	{
		mAttackSuccess = static_cast<int>((float)mAttackSuccess * ANIMAL_RATE_TWO_GRADE);
	}
	if (mDATA.aAnimalNumber == ANIMAL_NUM_BEAR3)
	{
		mAttackSuccess = static_cast<int>((float)mAttackSuccess * ANIMAL_RATE_THREE_GRADE);
	}
	// S4R V2
	if (mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87141) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87138) || (mUSER
		[mIndex].mAvatarInfo.aEquip[7][0] == 87139) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87140)) mAttackSuccess
		+= 100;
	if (mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87162) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87159) || (mUSER
		[mIndex].mAvatarInfo.aEquip[7][0] == 87160) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87161)) mAttackSuccess
		+= 100;
	if (mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87183) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87180) || (mUSER
		[mIndex].mAvatarInfo.aEquip[7][0] == 87181) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87182)) mAttackSuccess
		+= 100;

	// Mount Ability
	if (mUSER[mIndex].mAvatarInfo.aAnimalIndex >= 10)
	{
		switch ((mUSER[mIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[mIndex].mAvatarInfo.aAnimalIndex - 10] % 10000) /
			1000)
		{
		case 1:
			mAttackSuccess += 100;
			break;
		case 2:
			mAttackSuccess += 200;
			break;
		case 3:
			mAttackSuccess += 300;
			break;
		case 4:
			mAttackSuccess += 400;
			break;
		case 5:
			mAttackSuccess += 500;
			break;
		case 6:
			mAttackSuccess += 600;
			break;
		case 7:
			mAttackSuccess += 700;
			break;
		case 8:
			mAttackSuccess += 800;
			break;
		case 9:
			mAttackSuccess += 900;
			break;
		}
	}
	// Socket Gem Stats
	for (index01 = 0; index01 < MAX_EQUIP_SLOT_NUM; index01++)
	{
		switch (index01)
		{
		case 0: //[Amulet]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 5)
			{
				mAttackSuccess += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 5)
			{
				mAttackSuccess += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 5)
			{
				mAttackSuccess += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 5)
			{
				mAttackSuccess += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 5)
			{
				mAttackSuccess += 62;
			}
			break;
		case 1: //[Cape]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 5)
			{
				mAttackSuccess += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 5)
			{
				mAttackSuccess += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 5)
			{
				mAttackSuccess += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 5)
			{
				mAttackSuccess += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 5)
			{
				mAttackSuccess += 62;
			}
			break;
		case 2: //[Armor]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 5)
			{
				mAttackSuccess += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 5)
			{
				mAttackSuccess += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 5)
			{
				mAttackSuccess += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 5)
			{
				mAttackSuccess += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 5)
			{
				mAttackSuccess += 62;
			}
			break;
		case 3: //[Gloves]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 5)
			{
				mAttackSuccess += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 5)
			{
				mAttackSuccess += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 5)
			{
				mAttackSuccess += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 5)
			{
				mAttackSuccess += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 5)
			{
				mAttackSuccess += 62;
			}
			break;
		case 4: //[Ring]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 5)
			{
				mAttackSuccess += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 5)
			{
				mAttackSuccess += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 5)
			{
				mAttackSuccess += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 5)
			{
				mAttackSuccess += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 5)
			{
				mAttackSuccess += 62;
			}
			break;
		case 5: //[Boots]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 5)
			{
				mAttackSuccess += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 5)
			{
				mAttackSuccess += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 5)
			{
				mAttackSuccess += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 5)
			{
				mAttackSuccess += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 5)
			{
				mAttackSuccess += 62;
			}
			break;
		case 7: //[Weapon]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 5)
			{
				mAttackSuccess += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 5)
			{
				mAttackSuccess += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 5)
			{
				mAttackSuccess += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 5)
			{
				mAttackSuccess += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 5)
			{
				mAttackSuccess += 62;
			}
			break;
		}
	}
#endif
	mAttackBlock = 0;
	tWisdom = tAvatarInfo->aDex;
	tVitality = tAvatarInfo->aVit;

	// When Mount is Absorbed
	tVitality += mUTIL.GetAbsorbBonusStats(mIndex);
	/*
		if ((mUSER[mIndex].mAvatarInfo.aAbsorbState > 0) && (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber != 1329) && (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber != 1330) && (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber != 1331))
		{
			tVitality += 10;
		}
		if ((mUSER[mIndex].mAvatarInfo.aAbsorbState > 0) && ((mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber == 1329) || (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber == 1330) || (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber == 1331)))
		{
			tVitality += 15;
		}
	*/
	// Ninja Suit
	if ((mAVATAR_OBJECT[mIndex].mDATA.aFashionNumber == 385) || (mAVATAR_OBJECT[mIndex].mDATA.aFashionNumber == 386) ||
		(mAVATAR_OBJECT[mIndex].mDATA.aFashionNumber == 387))
	{
		tWisdom += 50;
	}

	// When Mount is Absorbed
	tWisdom += mUTIL.GetAbsorbBonusStats(mIndex);
	/*
		if ((mUSER[mIndex].mAvatarInfo.aAbsorbState > 0) && (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber != 1329) && (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber != 1330) && (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber != 1331))
		{
			tWisdom += 10;
		}
		if ((mUSER[mIndex].mAvatarInfo.aAbsorbState > 0) && ((mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber == 1329) || (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber == 1330) || (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber == 1331)))
		{
			tWisdom += 15;
		}
	*/
	for (index01 = 0; index01 < MAX_EQUIP_SLOT_NUM; index01++)
	{
		if (mEquipItem[index01] == nullptr)
		{
			continue;
		}
		tWisdom += mEquipItem[index01]->iWisdom;
		tVitality += mEquipItem[index01]->iVitality;

		switch (index01)
		{
		// Refine System
		case 1: //Cape
			tRFValue = mUTIL.ReturnRFValue(mEquipValue[index01]);
			if (tRFValue > 0)
			{
				if (tRFValue == 1) tWisdom += 5;
				if (tRFValue == 2) tWisdom += 10;
				if (tRFValue == 3) tWisdom += 15;
				if (tRFValue == 4) tWisdom += 20;
				if (tRFValue == 5) tWisdom += 25;
				if (tRFValue == 6) tWisdom += 30;
				if (tRFValue == 7) tWisdom += 35;
				if (tRFValue == 8) tWisdom += 40;
				if (tRFValue == 9) tWisdom += 45;
				if (tRFValue == 10) tWisdom += 50;
				if (tRFValue == 11) tWisdom += 55;
				if (tRFValue == 12) tWisdom += 60;
				if (tRFValue == 13) tWisdom += 65;
				if (tRFValue == 14) tWisdom += 70;
				if (tRFValue == 15) tWisdom += 75;
				if (tRFValue == 16) tWisdom += 80;
				if (tRFValue == 17) tWisdom += 85;
				if (tRFValue == 18) tWisdom += 90;
				if (tRFValue == 19) tWisdom += 95;
				if (tRFValue == 20) tWisdom += 100;
				if (tRFValue == 21) tWisdom += 105;
				if (tRFValue == 22) tWisdom += 110;
				if (tRFValue == 23) tWisdom += 115;
				if (tRFValue == 24) tWisdom += 120;
				if (tRFValue == 25) tWisdom += 130;
			}
			break;

		case 3: // Gloves
			tRFValue = mUTIL.ReturnRFValue(mEquipValue[index01]);
			if (tRFValue > 0)
			{
				if (tRFValue == 1) tWisdom += 5;
				if (tRFValue == 2) tWisdom += 10;
				if (tRFValue == 3) tWisdom += 15;
				if (tRFValue == 4) tWisdom += 20;
				if (tRFValue == 5) tWisdom += 25;
				if (tRFValue == 6) tWisdom += 30;
				if (tRFValue == 7) tWisdom += 35;
				if (tRFValue == 8) tWisdom += 40;
				if (tRFValue == 9) tWisdom += 45;
				if (tRFValue == 10) tWisdom += 50;
				if (tRFValue == 11) tWisdom += 55;
				if (tRFValue == 12) tWisdom += 60;
				if (tRFValue == 13) tWisdom += 65;
				if (tRFValue == 14) tWisdom += 70;
				if (tRFValue == 15) tWisdom += 75;
				if (tRFValue == 16) tWisdom += 80;
				if (tRFValue == 17) tWisdom += 85;
				if (tRFValue == 18) tWisdom += 90;
				if (tRFValue == 19) tWisdom += 95;
				if (tRFValue == 20) tWisdom += 100;
				if (tRFValue == 21) tWisdom += 105;
				if (tRFValue == 22) tWisdom += 110;
				if (tRFValue == 23) tWisdom += 115;
				if (tRFValue == 24) tWisdom += 120;
				if (tRFValue == 25) tWisdom += 130;
			}
			break;

		case 5: // Boots
			tRFValue = mUTIL.ReturnRFValue(mEquipValue[index01]);
			if (tRFValue > 0)
			{
				if (tRFValue == 1) tWisdom += 5;
				if (tRFValue == 2) tWisdom += 10;
				if (tRFValue == 3) tWisdom += 15;
				if (tRFValue == 4) tWisdom += 20;
				if (tRFValue == 5) tWisdom += 25;
				if (tRFValue == 6) tWisdom += 30;
				if (tRFValue == 7) tWisdom += 35;
				if (tRFValue == 8) tWisdom += 40;
				if (tRFValue == 9) tWisdom += 45;
				if (tRFValue == 10) tWisdom += 50;
				if (tRFValue == 11) tWisdom += 55;
				if (tRFValue == 12) tWisdom += 60;
				if (tRFValue == 13) tWisdom += 65;
				if (tRFValue == 14) tWisdom += 70;
				if (tRFValue == 15) tWisdom += 75;
				if (tRFValue == 16) tWisdom += 80;
				if (tRFValue == 17) tWisdom += 85;
				if (tRFValue == 18) tWisdom += 90;
				if (tRFValue == 19) tWisdom += 95;
				if (tRFValue == 20) tWisdom += 100;
				if (tRFValue == 21) tWisdom += 105;
				if (tRFValue == 22) tWisdom += 110;
				if (tRFValue == 23) tWisdom += 115;
				if (tRFValue == 24) tWisdom += 120;
				if (tRFValue == 25) tWisdom += 130;
			}
			break;

		case 4: // Ring
			tRFValue = mUTIL.ReturnRFValue(mEquipValue[index01]);
			if (tRFValue > 0)
			{
				if (tRFValue == 1) tVitality += 5;
				if (tRFValue == 2) tVitality += 10;
				if (tRFValue == 3) tVitality += 15;
				if (tRFValue == 4) tVitality += 20;
				if (tRFValue == 5) tVitality += 25;
				if (tRFValue == 6) tVitality += 30;
				if (tRFValue == 7) tVitality += 35;
				if (tRFValue == 8) tVitality += 40;
				if (tRFValue == 9) tVitality += 45;
				if (tRFValue == 10) tVitality += 50;
				if (tRFValue == 11) tVitality += 55;
				if (tRFValue == 12) tVitality += 60;
				if (tRFValue == 13) tVitality += 65;
				if (tRFValue == 14) tVitality += 70;
				if (tRFValue == 15) tVitality += 75;
				if (tRFValue == 16) tVitality += 80;
				if (tRFValue == 17) tVitality += 85;
				if (tRFValue == 18) tVitality += 90;
				if (tRFValue == 19) tVitality += 95;
				if (tRFValue == 20) tVitality += 100;
				if (tRFValue == 21) tVitality += 105;
				if (tRFValue == 22) tVitality += 110;
				if (tRFValue == 23) tVitality += 115;
				if (tRFValue == 24) tVitality += 120;
				if (tRFValue == 25) tVitality += 130;
			}
			break;
		}
	}
	// Title System
	if (mUSER[mIndex].mAvatarInfo.aTitle == 1) tVitality += 1;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 2) tVitality += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 3) tVitality += 6;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 4) tVitality += 10;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 5) tVitality += 15;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 6) tVitality += 21;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 7) tVitality += 28;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 8) tVitality += 36;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 9) tVitality += 45;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 10) tVitality += 55;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 11) tVitality += 67;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 12) tVitality += 82;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 101) tVitality += 0;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 102) tVitality += 1;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 103) tVitality += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 104) tVitality += 5;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 105) tVitality += 8;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 106) tVitality += 11;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 107) tVitality += 14;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 108) tVitality += 18;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 109) tVitality += 23;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 110) tVitality += 28;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 111) tVitality += 34;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 112) tVitality += 41;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 201) tVitality += 0;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 202) tVitality += 1;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 203) tVitality += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 204) tVitality += 5;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 205) tVitality += 8;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 206) tVitality += 11;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 207) tVitality += 14;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 208) tVitality += 18;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 209) tVitality += 23;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 210) tVitality += 28;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 211) tVitality += 34;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 212) tVitality += 41;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 301) tVitality += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 302) tVitality += 6;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 303) tVitality += 12;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 304) tVitality += 20;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 305) tVitality += 30;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 306) tVitality += 42;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 307) tVitality += 56;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 308) tVitality += 72;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 309) tVitality += 90;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 310) tVitality += 110;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 311) tVitality += 134;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 312) tVitality += 164;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 401) tVitality += 0;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 402) tVitality += 1;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 403) tVitality += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 404) tVitality += 5;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 405) tVitality += 8;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 406) tVitality += 11;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 407) tVitality += 14;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 408) tVitality += 18;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 409) tVitality += 23;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 410) tVitality += 28;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 411) tVitality += 34;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 412) tVitality += 41;

	// Title System
	if (mUSER[mIndex].mAvatarInfo.aTitle == 1) tWisdom += 1;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 2) tWisdom += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 3) tWisdom += 6;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 4) tWisdom += 10;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 5) tWisdom += 15;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 6) tWisdom += 21;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 7) tWisdom += 28;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 8) tWisdom += 36;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 9) tWisdom += 45;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 10) tWisdom += 55;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 11) tWisdom += 67;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 12) tWisdom += 82;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 101) tWisdom += 1;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 102) tWisdom += 2;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 103) tWisdom += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 104) tWisdom += 5;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 105) tWisdom += 7;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 106) tWisdom += 10;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 107) tWisdom += 14;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 108) tWisdom += 18;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 109) tWisdom += 22;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 110) tWisdom += 27;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 111) tWisdom += 33;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 112) tWisdom += 41;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 201) tWisdom += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 202) tWisdom += 8;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 203) tWisdom += 15;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 204) tWisdom += 25;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 205) tWisdom += 37;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 206) tWisdom += 52;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 207) tWisdom += 70;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 208) tWisdom += 90;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 209) tWisdom += 112;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 210) tWisdom += 137;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 211) tWisdom += 167;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 212) tWisdom += 205;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 301) tWisdom += 1;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 302) tWisdom += 2;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 303) tWisdom += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 304) tWisdom += 5;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 305) tWisdom += 7;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 306) tWisdom += 10;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 307) tWisdom += 14;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 308) tWisdom += 18;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 309) tWisdom += 22;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 310) tWisdom += 27;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 311) tWisdom += 33;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 312) tWisdom += 41;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 401) tWisdom += 1;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 402) tWisdom += 2;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 403) tWisdom += 3;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 404) tWisdom += 5;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 405) tWisdom += 7;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 406) tWisdom += 10;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 407) tWisdom += 14;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 408) tWisdom += 18;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 409) tWisdom += 22;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 410) tWisdom += 27;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 411) tWisdom += 33;
	if (mUSER[mIndex].mAvatarInfo.aTitle == 412) tWisdom += 41;

	// Halo System
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 1) tVitality += 1;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 2) tVitality += 2;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 3) tVitality += 3;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 4) tVitality += 4;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 5) tVitality += 5;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 6) tVitality += 6;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 7) tVitality += 7;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 8) tVitality += 8;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 9) tVitality += 9;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 10) tVitality += 10;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 11) tVitality += 11;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 12) tVitality += 12;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 13) tVitality += 13;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 14) tVitality += 14;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 15) tVitality += 15;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 16) tVitality += 16;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 17) tVitality += 17;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 18) tVitality += 18;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 19) tVitality += 19;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 20) tVitality += 20;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 21) tVitality += 21;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 22) tVitality += 22;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 23) tVitality += 23;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 24) tVitality += 24;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 25) tVitality += 25;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 26) tVitality += 26;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 27) tVitality += 27;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 28) tVitality += 28;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 29) tVitality += 29;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 30) tVitality += 30;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 31) tVitality += 31;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 32) tVitality += 32;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 33) tVitality += 33;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 34) tVitality += 34;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 35) tVitality += 35;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 36) tVitality += 36;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 37) tVitality += 37;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 38) tVitality += 38;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 39) tVitality += 39;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 40) tVitality += 40;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 41) tVitality += 41;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 42) tVitality += 42;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 43) tVitality += 43;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 44) tVitality += 44;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 45) tVitality += 45;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 46) tVitality += 46;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 47) tVitality += 47;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 48) tVitality += 48;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 49) tVitality += 49;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 50) tVitality += 50;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 51) tVitality += 51;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 52) tVitality += 52;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 53) tVitality += 53;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 54) tVitality += 54;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 55) tVitality += 55;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 56) tVitality += 56;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 57) tVitality += 57;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 58) tVitality += 58;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 59) tVitality += 59;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 60) tVitality += 60;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 61) tVitality += 61;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 62) tVitality += 62;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 63) tVitality += 63;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 64) tVitality += 64;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 65) tVitality += 65;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 66) tVitality += 66;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 67) tVitality += 67;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 68) tVitality += 68;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 69) tVitality += 69;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 70) tVitality += 70;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 71) tVitality += 71;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 72) tVitality += 72;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 73) tVitality += 73;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 74) tVitality += 74;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 75) tVitality += 75;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 76) tVitality += 76;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 77) tVitality += 77;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 78) tVitality += 78;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 79) tVitality += 79;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 80) tVitality += 80;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 81) tVitality += 81;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 82) tVitality += 82;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 83) tVitality += 83;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 84) tVitality += 84;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 85) tVitality += 85;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 86) tVitality += 86;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 87) tVitality += 87;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 88) tVitality += 88;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 89) tVitality += 89;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 90) tVitality += 90;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 91) tVitality += 91;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 92) tVitality += 92;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 93) tVitality += 93;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 94) tVitality += 94;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 95) tVitality += 95;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 96) tVitality += 96;

	// Halo System
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 1) tWisdom += 1;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 2) tWisdom += 2;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 3) tWisdom += 3;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 4) tWisdom += 4;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 5) tWisdom += 5;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 6) tWisdom += 6;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 7) tWisdom += 7;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 8) tWisdom += 8;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 9) tWisdom += 9;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 10) tWisdom += 10;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 11) tWisdom += 11;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 12) tWisdom += 12;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 13) tWisdom += 13;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 14) tWisdom += 14;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 15) tWisdom += 15;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 16) tWisdom += 16;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 17) tWisdom += 17;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 18) tWisdom += 18;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 19) tWisdom += 19;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 20) tWisdom += 20;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 21) tWisdom += 21;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 22) tWisdom += 22;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 23) tWisdom += 23;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 24) tWisdom += 24;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 25) tWisdom += 25;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 26) tWisdom += 26;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 27) tWisdom += 27;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 28) tWisdom += 28;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 29) tWisdom += 29;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 30) tWisdom += 30;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 31) tWisdom += 31;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 32) tWisdom += 32;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 33) tWisdom += 33;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 34) tWisdom += 34;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 35) tWisdom += 35;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 36) tWisdom += 36;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 37) tWisdom += 37;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 38) tWisdom += 38;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 39) tWisdom += 39;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 40) tWisdom += 40;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 41) tWisdom += 41;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 42) tWisdom += 42;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 43) tWisdom += 43;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 44) tWisdom += 44;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 45) tWisdom += 45;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 46) tWisdom += 46;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 47) tWisdom += 47;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 48) tWisdom += 48;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 49) tWisdom += 49;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 50) tWisdom += 50;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 51) tWisdom += 51;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 52) tWisdom += 52;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 53) tWisdom += 53;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 54) tWisdom += 54;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 55) tWisdom += 55;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 56) tWisdom += 56;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 57) tWisdom += 57;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 58) tWisdom += 58;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 59) tWisdom += 59;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 60) tWisdom += 60;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 61) tWisdom += 61;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 62) tWisdom += 62;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 63) tWisdom += 63;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 64) tWisdom += 64;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 65) tWisdom += 65;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 66) tWisdom += 66;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 67) tWisdom += 67;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 68) tWisdom += 68;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 69) tWisdom += 69;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 70) tWisdom += 70;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 71) tWisdom += 71;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 72) tWisdom += 72;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 73) tWisdom += 73;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 74) tWisdom += 74;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 75) tWisdom += 75;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 76) tWisdom += 76;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 77) tWisdom += 77;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 78) tWisdom += 78;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 79) tWisdom += 79;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 80) tWisdom += 80;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 81) tWisdom += 81;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 82) tWisdom += 82;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 83) tWisdom += 83;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 84) tWisdom += 84;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 85) tWisdom += 85;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 86) tWisdom += 86;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 87) tWisdom += 87;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 88) tWisdom += 88;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 89) tWisdom += 89;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 90) tWisdom += 90;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 91) tWisdom += 91;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 92) tWisdom += 92;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 93) tWisdom += 93;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 94) tWisdom += 94;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 95) tWisdom += 95;
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 96) tWisdom += 96;

	mAttackBlock += static_cast<int>((float)tWisdom * 1.67f);
	mAttackBlock += static_cast<int>((float)tVitality * 0.90f);

	if (mUSER[mIndex].mAvatarInfo.aRankAbility == 4) mAttackBlock += 1000;

#ifdef __GOD__
	mAttackBlock += mLEVEL.ReturnLevelFactorAttackBlock((mDATA.aLevel1 + mDATA.aLevel2));
#else
	mAttackBlock += mLEVEL.ReturnLevelFactorAttackBlock(mDATA.aLevel1);
#endif
	//mAttackBlock *= 2;
	for (index01 = 0; index01 < MAX_EQUIP_SLOT_NUM; index01++)
	{
		if (mEquipItem[index01] == nullptr)
		{
			continue;
		}
		mAttackBlock += mEquipItem[index01]->iAttackBlock;
		//Warlord Set
		// S3R
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87006) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87007) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87008)) mAttackBlock += (static_cast<int>((double)mEquipItem[
			index01]->iAttackBlock) * 0.6000000238418579);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87027) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87028) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87029)) mAttackBlock += (static_cast<int>((double)mEquipItem[
			index01]->iAttackBlock) * 0.6000000238418579);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87048) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87049) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87050)) mAttackBlock += (static_cast<int>((double)mEquipItem[
			index01]->iAttackBlock) * 0.6000000238418579);
		// S4R
		if (mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87012) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87009) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87010) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87011))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 0.6000000238418579);
		if (mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87033) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87030) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87031) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87032))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 0.6000000238418579);
		if (mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87054) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87051) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87052) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87053))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 0.6000000238418579);
		// S5R
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87016) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87017) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87018) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87019) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87020)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87013) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87014) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87015))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 1.0);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87037) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87038) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87039) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87040) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87041)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87034) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87035) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87036))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 1.0);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87058) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87059) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87060) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87061) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87062)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87055) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87056) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87057))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 1.0);
		// S3E
		if ((mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87069) && (mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87070))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 0.4000000059604645);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87091) && (mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87092))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 0.4000000059604645);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87113) && (mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87114))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 0.4000000059604645);
		// S4E
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87074) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87075) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87076)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87071) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87072) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87073))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 0.6000000238418579);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87096) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87097) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87098)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87093) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87094) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87095))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 0.6000000238418579);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87118) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87119) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87120)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87115) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87116) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87117))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 0.6000000238418579);
		// S5E
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87080) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87081) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87082) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87083) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87084)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87077) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87078) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87079))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 1.0);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87102) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87103) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87104) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87105) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87106)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87099) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87100) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87101))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 1.0);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87124) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87125) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87126) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87127) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87128)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87121) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87122) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87123))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 1.0);
		// S3R V2
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87135) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87136) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87137)) mAttackBlock += (static_cast<int>((double)mEquipItem[
			index01]->iAttackBlock) * 0.699999988079071);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87156) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87157) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87158)) mAttackBlock += (static_cast<int>((double)mEquipItem[
			index01]->iAttackBlock) * 0.699999988079071);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87177) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87178) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87179)) mAttackBlock += (static_cast<int>((double)mEquipItem[
			index01]->iAttackBlock) * 0.699999988079071);
		// S4R V2
		if (mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87141) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87138) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87139) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87140))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 0.699999988079071);
		if (mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87162) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87159) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87160) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87161))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 0.699999988079071);
		if (mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87183) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87180) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87181) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87182))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 0.699999988079071);
		// S5R V2
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87145) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87146) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87147) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87148) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87149)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87142) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87143) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87144))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 1.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87166) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87167) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87168) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87169) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87170)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87163) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87164) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87165))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 1.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87187) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87188) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87189) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87190) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87191)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87184) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87185) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87186))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 1.100000023841858);
		// S3E V2
		if ((mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87198) && (mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87199))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 0.699999988079071);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87220) && (mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87221))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 0.699999988079071);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87242) && (mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87243))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 0.699999988079071);
		// S4E V2
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87203) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87204) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87205)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87200) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87201) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87202))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 0.699999988079071);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87225) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87226) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87227)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87222) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87223) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87224))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 0.699999988079071);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87247) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87248) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87249)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87244) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87245) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87246))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 0.699999988079071);
		// S5E V2
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87209) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87210) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87211) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87212) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87213)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87206) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87207) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87208))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 1.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87231) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87232) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87233) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87234) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87235)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87228) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87229) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87230))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 1.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87253) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87254) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87255) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87256) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87257)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87250) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87251) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87252))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 1.100000023841858);
		// 88001 Set
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88004) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88005) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88006) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88007) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88008)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88001) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88002) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88003))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 2.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88012) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88013) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88014) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88015) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88016)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88009) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88010) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88011))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 2.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88020) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88021) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88022) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88023) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88024)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88017) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88018) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88019))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 2.100000023841858);
		// 88025 Set
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88028) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88029) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88030) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88031) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88032)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88025) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88026) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88027))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 3.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88036) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88037) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88038) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88039) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88040)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88033) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88034) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88035))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 3.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88044) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88045) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88046) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88047) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88048)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88041) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88042) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88043))
			mAttackBlock += (static_cast<int>((double)mEquipItem[index01]->iAttackBlock) * 3.100000023841858);


		switch (index01)
		{
		case 2:
			tIUValue = mUTIL.ReturnIUValueWithLevelLimit(tAvatarInfo->aLevel1,
			                                             mUTIL.ReturnIUValue(mEquipValue[index01]));
			tIUEffectValue = mUTIL.ReturnIUEffectValue(mEquipItem[index01], 4);
			mAttackBlock += (tIUValue * tIUEffectValue);
			break;
		case 5:
			tIUValue = mUTIL.ReturnIUValueWithLevelLimit(tAvatarInfo->aLevel1,
			                                             mUTIL.ReturnIUValue(mEquipValue[index01]));
			tIUEffectValue = mUTIL.ReturnIUEffectValue(mEquipItem[index01], 4);
			mAttackBlock += (tIUValue * tIUEffectValue);
			tISValue = mUTIL.ReturnISValueWithLevelLimit(tAvatarInfo->aLevel1,
			                                             mUTIL.ReturnISValue(mEquipValue[index01]));
			if (tISValue > 0)
			{
				mAttackBlock += static_cast<int>((float)(mEquipItem[index01]->iAttackBlock + (tIUValue *
					tIUEffectValue)) * ((float)tISValue * 0.03f));
			}
			break;
		}
	}
	//Original Korea code is not useable in battle zone.
#if 0
	if ((!mGAME.mCheckZone049TypeServer) && (!mGAME.mCheckZone051TypeServer) && (!mGAME.mCheckZone053TypeServer))
	{
		mAttackBlock += (tAvatarInfo->aEatWisdomPotion * 2);
	}
#endif
	mAttackBlock += (tAvatarInfo->aEatAgilityPotion * 2);

#ifdef __ANIMAL__
	if ((mDATA.aAnimalNumber == ANIMAL_NUM_WOLF1) || (mDATA.aAnimalNumber == ANIMAL_NUM_HAECHI1))
	{
		mAttackBlock = static_cast<int>((float)mAttackBlock * ANIMAL_RATE_ONE_GRADE);
	}
	if ((mDATA.aAnimalNumber == ANIMAL_NUM_WOLF2) || (mDATA.aAnimalNumber == ANIMAL_NUM_HAECHI2))
	{
		mAttackBlock = static_cast<int>((float)mAttackBlock * ANIMAL_RATE_TWO_GRADE);
	}
	if ((mDATA.aAnimalNumber == ANIMAL_NUM_WOLF3) || (mDATA.aAnimalNumber == ANIMAL_NUM_HAECHI3))
	{
		mAttackBlock = static_cast<int>((float)mAttackBlock * ANIMAL_RATE_THREE_GRADE);
	}
	// Mount Ability
	if (mUSER[mIndex].mAvatarInfo.aAnimalIndex >= 10)
	{
		switch ((mUSER[mIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[mIndex].mAvatarInfo.aAnimalIndex - 10] % 1000) /
			100)
		{
		case 1:
			mAttackBlock += 100;
			break;
		case 2:
			mAttackBlock += 200;
			break;
		case 3:
			mAttackBlock += 300;
			break;
		case 4:
			mAttackBlock += 400;
			break;
		case 5:
			mAttackBlock += 500;
			break;
		case 6:
			mAttackBlock += 600;
			break;
		case 7:
			mAttackBlock += 700;
			break;
		case 8:
			mAttackBlock += 800;
			break;
		case 9:
			mAttackBlock += 900;
			break;
		}
	}
	// Socket Gem Stats
	for (index01 = 0; index01 < MAX_EQUIP_SLOT_NUM; index01++)
	{
		switch (index01)
		{
		case 0: //[Amulet]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 5)
			{
				mAttackBlock += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 5)
			{
				mAttackBlock += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 5)
			{
				mAttackBlock += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 5)
			{
				mAttackBlock += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 5)
			{
				mAttackBlock += 62;
			}
			break;
		case 1: //[Cape]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 5)
			{
				mAttackBlock += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 5)
			{
				mAttackBlock += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 5)
			{
				mAttackBlock += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 5)
			{
				mAttackBlock += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 5)
			{
				mAttackBlock += 62;
			}
			break;
		case 2: //[Armor]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 5)
			{
				mAttackBlock += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 5)
			{
				mAttackBlock += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 5)
			{
				mAttackBlock += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 5)
			{
				mAttackBlock += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 5)
			{
				mAttackBlock += 62;
			}
			break;
		case 3: //[Gloves]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 5)
			{
				mAttackBlock += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 5)
			{
				mAttackBlock += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 5)
			{
				mAttackBlock += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 5)
			{
				mAttackBlock += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 5)
			{
				mAttackBlock += 62;
			}
			break;
		case 4: //[Ring]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 5)
			{
				mAttackBlock += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 5)
			{
				mAttackBlock += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 5)
			{
				mAttackBlock += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 5)
			{
				mAttackBlock += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 5)
			{
				mAttackBlock += 62;
			}
			break;
		case 5: //[Boots]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 5)
			{
				mAttackBlock += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 5)
			{
				mAttackBlock += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 5)
			{
				mAttackBlock += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 5)
			{
				mAttackBlock += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 5)
			{
				mAttackBlock += 62;
			}
			break;
		case 7: //[Weapon]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 5)
			{
				mAttackBlock += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 5)
			{
				mAttackBlock += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 5)
			{
				mAttackBlock += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 5)
			{
				mAttackBlock += 62;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 5)
			{
				mAttackBlock += 62;
			}
			break;
		}
	}


#endif

#ifdef __GOD__
	mElementAttackPower = mLEVEL.ReturnLevelFactorElementAttack((mDATA.aLevel1 + mDATA.aLevel2));
#else
	mElementAttackPower = mLEVEL.ReturnLevelFactorElementAttack(mDATA.aLevel1);
#endif
	for (index01 = 0; index01 < MAX_EQUIP_SLOT_NUM; index01++)
	{
		if (mEquipItem[index01] == nullptr)
		{
			continue;
		}
		mElementAttackPower += mEquipItem[index01]->iElementAttackPower;
		//Warlord Set
		// S1R
		if ((mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87000) && (mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87001))
			mElementAttackPower += (static_cast<int>((double)mEquipItem[index01]->iElementAttackPower) *
				0.4000000059604645);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87021) && (mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87022))
			mElementAttackPower += (static_cast<int>((double)mEquipItem[index01]->iElementAttackPower) *
				0.4000000059604645);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87042) && (mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87043))
			mElementAttackPower += (static_cast<int>((double)mEquipItem[index01]->iElementAttackPower) *
				0.4000000059604645);
		// S5R
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87016) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87017) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87018) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87019) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87020)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87013) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87014) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87015))
			mElementAttackPower += (static_cast<int>((double)mEquipItem[index01]->iElementAttackPower) * 1.0);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87037) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87038) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87039) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87040) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87041)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87034) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87035) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87036))
			mElementAttackPower += (static_cast<int>((double)mEquipItem[index01]->iElementAttackPower) * 1.0);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87058) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87059) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87060) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87061) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87062)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87055) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87056) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87057))
			mElementAttackPower += (static_cast<int>((double)mEquipItem[index01]->iElementAttackPower) * 1.0);
		// S1E
		if ((mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87063) && (mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87064))
			mElementAttackPower += (static_cast<int>((double)mEquipItem[index01]->iElementAttackPower) *
				0.4000000059604645);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87085) && (mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87086))
			mElementAttackPower += (static_cast<int>((double)mEquipItem[index01]->iElementAttackPower) *
				0.4000000059604645);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87107) && (mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87108))
			mElementAttackPower += (static_cast<int>((double)mEquipItem[index01]->iElementAttackPower) *
				0.4000000059604645);
		// S5E
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87080) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87081) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87082) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87083) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87084)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87077) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87078) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87079))
			mElementAttackPower += (static_cast<int>((double)mEquipItem[index01]->iElementAttackPower) * 1.0);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87102) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87103) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87104) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87105) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87106)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87099) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87100) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87101))
			mElementAttackPower += (static_cast<int>((double)mEquipItem[index01]->iElementAttackPower) * 1.0);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87124) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87125) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87126) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87127) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87128)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87121) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87122) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87123))
			mElementAttackPower += (static_cast<int>((double)mEquipItem[index01]->iElementAttackPower) * 1.0);
		// S1R V2
		if ((mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87129) && (mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87130))
			mElementAttackPower += (static_cast<int>((double)mEquipItem[index01]->iElementAttackPower) *
				0.6000000238418579);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87150) && (mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87151))
			mElementAttackPower += (static_cast<int>((double)mEquipItem[index01]->iElementAttackPower) *
				0.6000000238418579);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87171) && (mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87172))
			mElementAttackPower += (static_cast<int>((double)mEquipItem[index01]->iElementAttackPower) *
				0.6000000238418579);
		// S5R V2
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87145) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87146) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87147) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87148) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87149)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87142) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87143) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87144))
			mElementAttackPower += (static_cast<int>((double)mEquipItem[index01]->iElementAttackPower) *
				1.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87166) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87167) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87168) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87169) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87170)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87163) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87164) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87165))
			mElementAttackPower += (static_cast<int>((double)mEquipItem[index01]->iElementAttackPower) *
				1.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87187) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87188) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87189) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87190) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87191)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87184) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87185) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87186))
			mElementAttackPower += (static_cast<int>((double)mEquipItem[index01]->iElementAttackPower) *
				1.100000023841858);
		// S1E V2
		if ((mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87192) && (mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87193))
			mElementAttackPower += (static_cast<int>((double)mEquipItem[index01]->iElementAttackPower) *
				0.6000000238418579);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87214) && (mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87215))
			mElementAttackPower += (static_cast<int>((double)mEquipItem[index01]->iElementAttackPower) *
				0.6000000238418579);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87236) && (mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87237))
			mElementAttackPower += (static_cast<int>((double)mEquipItem[index01]->iElementAttackPower) *
				0.6000000238418579);
		// S5E V2
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87209) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87210) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87211) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87212) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87213)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87206) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87207) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87208))
			mElementAttackPower += (static_cast<int>((double)mEquipItem[index01]->iElementAttackPower) *
				1.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87231) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87232) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87233) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87234) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87235)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87228) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87229) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87230))
			mElementAttackPower += (static_cast<int>((double)mEquipItem[index01]->iElementAttackPower) *
				1.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87253) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87254) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87255) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87256) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87257)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87250) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87251) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87252))
			mElementAttackPower += (static_cast<int>((double)mEquipItem[index01]->iElementAttackPower) *
				1.100000023841858);
		// 88001 Set
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88004) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88005) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88006) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88007) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88008)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88001) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88002) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88003))
			mElementAttackPower += (static_cast<int>((double)mEquipItem[index01]->iElementAttackPower) *
				2.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88012) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88013) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88014) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88015) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88016)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88009) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88010) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88011))
			mElementAttackPower += (static_cast<int>((double)mEquipItem[index01]->iElementAttackPower) *
				2.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88020) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88021) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88022) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88023) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88024)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88017) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88018) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88019))
			mElementAttackPower += (static_cast<int>((double)mEquipItem[index01]->iElementAttackPower) *
				2.100000023841858);
		// 88025 Set
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88028) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88029) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88030) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88031) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88032)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88025) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88026) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88027))
			mElementAttackPower += (static_cast<int>((double)mEquipItem[index01]->iElementAttackPower) *
				3.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88036) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88037) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88038) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88039) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88040)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88033) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88034) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88035))
			mElementAttackPower += (static_cast<int>((double)mEquipItem[index01]->iElementAttackPower) *
				3.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88044) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88045) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88046) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88047) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88048)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88041) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88042) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88043))
			mElementAttackPower += (static_cast<int>((double)mEquipItem[index01]->iElementAttackPower) *
				3.100000023841858);

		switch (index01)
		{
		case 4:
			tIUValue = mUTIL.ReturnIUValueWithLevelLimit(tAvatarInfo->aLevel1,
			                                             mUTIL.ReturnIUValue(mEquipValue[index01]));
			tIUEffectValue = mUTIL.ReturnIUEffectValue(mEquipItem[index01], 5);
			mElementAttackPower += (tIUValue * tIUEffectValue);
			break;


		case 10:
			switch (mEquipItem[index01]->iIndex)
			{
			case 201:
			case 202:
			case 203:
			case 204:
			case 205:
			case 206:
			case 207:
			case 208:
			case 209:
			case 213:
			case 214:
			case 215:
				tISValue = mUTIL.ReturnISValueWithLevelLimit(tAvatarInfo->aLevel1,
				                                             mUTIL.ReturnISValue(mEquipValue[index01]));
				mElementAttackPower += (static_cast<float>(tISValue) * 3.9f);
				break;

			case 210:
			case 211:
			case 212:
			case 216:
			case 217:
			case 218:
				tISValue = mUTIL.ReturnISValueWithLevelLimit(tAvatarInfo->aLevel1,
				                                             mUTIL.ReturnISValue(mEquipValue[index01]));
				mElementAttackPower += (static_cast<float>(tISValue) * 7.8f);
				break;
			}

			break;
		}
	}
	mElementAttackPower += 10 * (mUSER[mIndex].mAvatarInfo.aEatElementAtkDefPotion / 1000);

	if ((mUSER[mIndex].mAvatarInfo.aTitle == 1) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 101) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 201) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 301) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 401))
	{
		mElementAttackPower *= 1.01;
	}
	if ((mUSER[mIndex].mAvatarInfo.aTitle == 2) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 102) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 202) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 302) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 402))
	{
		mElementAttackPower *= 1.02;
	}
	if ((mUSER[mIndex].mAvatarInfo.aTitle == 3) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 103) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 203) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 303) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 403))
	{
		mElementAttackPower *= 1.03;
	}
	if ((mUSER[mIndex].mAvatarInfo.aTitle == 4) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 104) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 204) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 304) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 404))
	{
		mElementAttackPower *= 1.04;
	}
	if ((mUSER[mIndex].mAvatarInfo.aTitle == 5) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 105) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 205) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 305) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 405))
	{
		mElementAttackPower *= 1.05;
	}
	if ((mUSER[mIndex].mAvatarInfo.aTitle == 6) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 106) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 206) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 306) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 406))
	{
		mElementAttackPower *= 1.06;
	}
	if ((mUSER[mIndex].mAvatarInfo.aTitle == 7) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 107) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 207) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 307) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 407))
	{
		mElementAttackPower *= 1.07;
	}
	if ((mUSER[mIndex].mAvatarInfo.aTitle == 8) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 108) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 208) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 308) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 408))
	{
		mElementAttackPower *= 1.08;
	}
	if ((mUSER[mIndex].mAvatarInfo.aTitle == 9) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 109) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 209) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 309) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 409))
	{
		mElementAttackPower *= 1.09;
	}
	if ((mUSER[mIndex].mAvatarInfo.aTitle == 10) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 110) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 210) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 310) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 410))
	{
		mElementAttackPower *= 1.10;
	}
	if ((mUSER[mIndex].mAvatarInfo.aTitle == 11) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 111) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 211) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 311) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 411))
	{
		mElementAttackPower *= 1.11;
	}
	if ((mUSER[mIndex].mAvatarInfo.aTitle == 12) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 112) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 212) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 312) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 412))
	{
		mElementAttackPower *= 1.12;
	}

	if (mUSER[mIndex].mAvatarInfo.aRankAbility == 3) mElementAttackPower += 1000;

#ifdef __ANIMAL__
	if ((mDATA.aAnimalNumber == ANIMAL_NUM_BAKHO) || (mDATA.aAnimalNumber == ANIMAL_NUM_CHUNLOK)
		|| (mDATA.aAnimalNumber == ANIMAL_NUM_BULL1))
	{
		mElementAttackPower = static_cast<int>((float)mElementAttackPower * ANIMAL_RATE_ONE_GRADE);
	}
	if ((mDATA.aAnimalNumber == ANIMAL_NUM_CHUNGHO) || (mDATA.aAnimalNumber == ANIMAL_NUM_BAIKLOK)
		|| (mDATA.aAnimalNumber == ANIMAL_NUM_BULL2))
	{
		mElementAttackPower = static_cast<int>((float)mElementAttackPower * ANIMAL_RATE_TWO_GRADE);
	}
	if ((mDATA.aAnimalNumber == ANIMAL_NUM_CHUKHO) || (mDATA.aAnimalNumber == ANIMAL_NUM_SHINLOK)
		|| (mDATA.aAnimalNumber == ANIMAL_NUM_BULL3))
	{
		mElementAttackPower = static_cast<int>((float)mElementAttackPower * ANIMAL_RATE_THREE_GRADE);
	}
#endif
	// S4E V2
	if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87203) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87204) && (mUSER
		[mIndex].mAvatarInfo.aEquip[5][0] == 87205)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87200) || (mUSER[
		mIndex].mAvatarInfo.aEquip[7][0] == 87201) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87202))
		mElementAttackPower += 500;
	if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87225) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87226) && (mUSER
		[mIndex].mAvatarInfo.aEquip[5][0] == 87227)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87222) || (mUSER[
		mIndex].mAvatarInfo.aEquip[7][0] == 87223) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87224))
		mElementAttackPower += 500;
	if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87247) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87248) && (mUSER
		[mIndex].mAvatarInfo.aEquip[5][0] == 87249)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87244) || (mUSER[
		mIndex].mAvatarInfo.aEquip[7][0] == 87245) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87246))
		mElementAttackPower += 500;
	// Mount Ability
	if (mUSER[mIndex].mAvatarInfo.aAnimalIndex >= 10)
	{
		switch ((mUSER[mIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[mIndex].mAvatarInfo.aAnimalIndex - 10] % 100) / 10)
		{
		case 1:
			mElementAttackPower += 50;
			break;
		case 2:
			mElementAttackPower += 100;
			break;
		case 3:
			mElementAttackPower += 150;
			break;
		case 4:
			mElementAttackPower += 200;
			break;
		case 5:
			mElementAttackPower += 250;
			break;
		case 6:
			mElementAttackPower += 300;
			break;
		case 7:
			mElementAttackPower += 350;
			break;
		case 8:
			mElementAttackPower += 400;
			break;
		case 9:
			mElementAttackPower += 450;
			break;
		}
	}
	// Socket Gem Stats
	for (index01 = 0; index01 < MAX_EQUIP_SLOT_NUM; index01++)
	{
		switch (index01)
		{
		case 0: //[Amulet]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 4)
			{
				mElementAttackPower += 100;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 9)
			{
				mElementAttackPower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 4)
			{
				mElementAttackPower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 9)
			{
				mElementAttackPower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 4)
			{
				mElementAttackPower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 9)
			{
				mElementAttackPower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 4)
			{
				mElementAttackPower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 9)
			{
				mElementAttackPower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 4)
			{
				mElementAttackPower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 9)
			{
				mElementAttackPower += 200;
			}
			break;
		case 1: //[Cape]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 4)
			{
				mElementAttackPower += 100;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 9)
			{
				mElementAttackPower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 4)
			{
				mElementAttackPower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 9)
			{
				mElementAttackPower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 4)
			{
				mElementAttackPower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 9)
			{
				mElementAttackPower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 4)
			{
				mElementAttackPower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 9)
			{
				mElementAttackPower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 4)
			{
				mElementAttackPower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 9)
			{
				mElementAttackPower += 200;
			}
			break;
		case 2: //[Armor]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 4)
			{
				mElementAttackPower += 100;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 9)
			{
				mElementAttackPower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 4)
			{
				mElementAttackPower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 9)
			{
				mElementAttackPower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 4)
			{
				mElementAttackPower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 9)
			{
				mElementAttackPower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 4)
			{
				mElementAttackPower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 9)
			{
				mElementAttackPower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 4)
			{
				mElementAttackPower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 9)
			{
				mElementAttackPower += 200;
			}
			break;
		case 3: //[Gloves]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 4)
			{
				mElementAttackPower += 100;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 9)
			{
				mElementAttackPower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 4)
			{
				mElementAttackPower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 9)
			{
				mElementAttackPower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 4)
			{
				mElementAttackPower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 9)
			{
				mElementAttackPower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 4)
			{
				mElementAttackPower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 9)
			{
				mElementAttackPower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 4)
			{
				mElementAttackPower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 9)
			{
				mElementAttackPower += 200;
			}
			break;
		case 4: //[Ring]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 4)
			{
				mElementAttackPower += 100;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 9)
			{
				mElementAttackPower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 4)
			{
				mElementAttackPower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 9)
			{
				mElementAttackPower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 4)
			{
				mElementAttackPower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 9)
			{
				mElementAttackPower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 4)
			{
				mElementAttackPower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 9)
			{
				mElementAttackPower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 4)
			{
				mElementAttackPower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 9)
			{
				mElementAttackPower += 200;
			}
			break;
		case 5: //[Boots]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 4)
			{
				mElementAttackPower += 100;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 9)
			{
				mElementAttackPower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 4)
			{
				mElementAttackPower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 9)
			{
				mElementAttackPower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 4)
			{
				mElementAttackPower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 9)
			{
				mElementAttackPower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 4)
			{
				mElementAttackPower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 9)
			{
				mElementAttackPower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 4)
			{
				mElementAttackPower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 9)
			{
				mElementAttackPower += 200;
			}
			break;
		case 7: //[Weapon]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 4)
			{
				mElementAttackPower += 100;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 9)
			{
				mElementAttackPower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 4)
			{
				mElementAttackPower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 9)
			{
				mElementAttackPower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 4)
			{
				mElementAttackPower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 9)
			{
				mElementAttackPower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 4)
			{
				mElementAttackPower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 9)
			{
				mElementAttackPower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 4)
			{
				mElementAttackPower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 9)
			{
				mElementAttackPower += 200;
			}
			break;
		}
	}


	mElementDefensePower = 0;
	for (index01 = 0; index01 < MAX_EQUIP_SLOT_NUM; index01++)
	{
		if (mEquipItem[index01] == nullptr)
		{
			continue;
		}
		mElementDefensePower += mEquipItem[index01]->iElementDefensePower;
		//Warlord Set
		// S1R
		if ((mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87000) && (mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87001))
			mElementDefensePower += (static_cast<int>((double)mEquipItem[index01]->iElementDefensePower) *
				0.4000000059604645);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87021) && (mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87022))
			mElementDefensePower += (static_cast<int>((double)mEquipItem[index01]->iElementDefensePower) *
				0.4000000059604645);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87042) && (mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87043))
			mElementDefensePower += (static_cast<int>((double)mEquipItem[index01]->iElementDefensePower) *
				0.4000000059604645);
		// S5R
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87016) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87017) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87018) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87019) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87020)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87013) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87014) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87015))
			mElementDefensePower += (static_cast<int>((double)mEquipItem[index01]->iElementDefensePower) * 1.0);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87037) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87038) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87039) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87040) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87041)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87034) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87035) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87036))
			mElementDefensePower += (static_cast<int>((double)mEquipItem[index01]->iElementDefensePower) * 1.0);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87058) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87059) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87060) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87061) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87062)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87055) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87056) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87057))
			mElementDefensePower += (static_cast<int>((double)mEquipItem[index01]->iElementDefensePower) * 1.0);
		// S1E
		if ((mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87063) && (mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87064))
			mElementDefensePower += (static_cast<int>((double)mEquipItem[index01]->iElementDefensePower) *
				0.4000000059604645);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87085) && (mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87086))
			mElementDefensePower += (static_cast<int>((double)mEquipItem[index01]->iElementDefensePower) *
				0.4000000059604645);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87107) && (mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87108))
			mElementDefensePower += (static_cast<int>((double)mEquipItem[index01]->iElementDefensePower) *
				0.4000000059604645);
		// S5E
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87080) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87081) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87082) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87083) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87084)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87077) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87078) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87079))
			mElementDefensePower += (static_cast<int>((double)mEquipItem[index01]->iElementDefensePower) * 1.0);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87102) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87103) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87104) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87105) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87106)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87099) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87100) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87101))
			mElementDefensePower += (static_cast<int>((double)mEquipItem[index01]->iElementDefensePower) * 1.0);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87124) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87125) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87126) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87127) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87128)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87121) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87122) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87123))
			mElementDefensePower += (static_cast<int>((double)mEquipItem[index01]->iElementDefensePower) * 1.0);
		// S1R V2
		if ((mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87129) && (mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87130))
			mElementDefensePower += (static_cast<int>((double)mEquipItem[index01]->iElementDefensePower) *
				0.6000000238418579);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87150) && (mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87151))
			mElementDefensePower += (static_cast<int>((double)mEquipItem[index01]->iElementDefensePower) *
				0.6000000238418579);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87171) && (mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87172))
			mElementDefensePower += (static_cast<int>((double)mEquipItem[index01]->iElementDefensePower) *
				0.6000000238418579);
		// S5R V2
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87145) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87146) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87147) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87148) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87149)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87142) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87143) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87144))
			mElementDefensePower += (static_cast<int>((double)mEquipItem[index01]->iElementDefensePower) *
				1.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87166) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87167) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87168) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87169) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87170)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87163) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87164) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87165))
			mElementDefensePower += (static_cast<int>((double)mEquipItem[index01]->iElementDefensePower) *
				1.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87187) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87188) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87189) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87190) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87191)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87184) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87185) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87186))
			mElementDefensePower += (static_cast<int>((double)mEquipItem[index01]->iElementDefensePower) *
				1.100000023841858);
		// S1E V2
		if ((mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87192) && (mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87193))
			mElementDefensePower += (static_cast<int>((double)mEquipItem[index01]->iElementDefensePower) *
				0.6000000238418579);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87214) && (mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87215))
			mElementDefensePower += (static_cast<int>((double)mEquipItem[index01]->iElementDefensePower) *
				0.6000000238418579);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87236) && (mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87237))
			mElementDefensePower += (static_cast<int>((double)mEquipItem[index01]->iElementDefensePower) *
				0.6000000238418579);
		// S5E V2
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87209) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87210) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87211) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87212) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87213)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87206) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87207) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87208))
			mElementDefensePower += (static_cast<int>((double)mEquipItem[index01]->iElementDefensePower) *
				1.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87231) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87232) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87233) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87234) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87235)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87228) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87229) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87230))
			mElementDefensePower += (static_cast<int>((double)mEquipItem[index01]->iElementDefensePower) *
				1.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87253) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87254) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87255) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87256) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87257)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87250) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87251) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87252))
			mElementDefensePower += (static_cast<int>((double)mEquipItem[index01]->iElementDefensePower) *
				1.100000023841858);
		// 88001 Set
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88004) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88005) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88006) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88007) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88008)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88001) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88002) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88003))
			mElementDefensePower += (static_cast<int>((double)mEquipItem[index01]->iElementDefensePower) *
				2.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88012) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88013) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88014) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88015) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88016)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88009) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88010) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88011))
			mElementDefensePower += (static_cast<int>((double)mEquipItem[index01]->iElementDefensePower) *
				2.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88020) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88021) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88022) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88023) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88024)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88017) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88018) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88019))
			mElementDefensePower += (static_cast<int>((double)mEquipItem[index01]->iElementDefensePower) *
				2.100000023841858);
		// 88025 Set
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88028) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88029) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88030) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88031) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88032)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88025) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88026) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88027))
			mElementDefensePower += (static_cast<int>((double)mEquipItem[index01]->iElementDefensePower) *
				3.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88036) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88037) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88038) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88039) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88040)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88033) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88034) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88035))
			mElementDefensePower += (static_cast<int>((double)mEquipItem[index01]->iElementDefensePower) *
				3.100000023841858);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88044) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88045) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88046) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88047) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88048)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88041) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88042) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88043))
			mElementDefensePower += (static_cast<int>((double)mEquipItem[index01]->iElementDefensePower) *
				3.100000023841858);

		switch (index01)
		{
		case 0:
			tIUValue = mUTIL.ReturnIUValueWithLevelLimit(tAvatarInfo->aLevel1,
			                                             mUTIL.ReturnIUValue(mEquipValue[index01]));
			tIUEffectValue = mUTIL.ReturnIUEffectValue(mEquipItem[index01], 6);
			mElementDefensePower += (tIUValue * tIUEffectValue);
			break;

		case 10:
			switch (mEquipItem[index01]->iIndex)
			{
			case 201:
			case 202:
			case 203:
			case 204:
			case 205:
			case 206:
			case 210:
			case 211:
			case 212:
			case 213:
			case 214:
			case 215:
				tISValue = mUTIL.ReturnISValueWithLevelLimit(tAvatarInfo->aLevel1,
				                                             mUTIL.ReturnISValue(mEquipValue[index01]));
				mElementDefensePower += (static_cast<float>(tISValue) * 3.9f);
				break;

			case 207:
			case 208:
			case 209:
			case 216:
			case 217:
			case 218:
				tISValue = mUTIL.ReturnISValueWithLevelLimit(tAvatarInfo->aLevel1,
				                                             mUTIL.ReturnISValue(mEquipValue[index01]));
				mElementDefensePower += (static_cast<float>(tISValue) * 7.8f);
				break;
			}
			break;
		}
	}
	mElementDefensePower += 10 * (mUSER[mIndex].mAvatarInfo.aEatElementAtkDefPotion % 1000);

	if ((mUSER[mIndex].mAvatarInfo.aTitle == 1) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 101) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 201) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 301) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 401))
	{
		mElementDefensePower *= 1.01;
	}
	if ((mUSER[mIndex].mAvatarInfo.aTitle == 2) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 102) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 202) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 302) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 402))
	{
		mElementDefensePower *= 1.02;
	}
	if ((mUSER[mIndex].mAvatarInfo.aTitle == 3) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 103) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 203) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 303) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 403))
	{
		mElementDefensePower *= 1.03;
	}
	if ((mUSER[mIndex].mAvatarInfo.aTitle == 4) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 104) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 204) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 304) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 404))
	{
		mElementDefensePower *= 1.04;
	}
	if ((mUSER[mIndex].mAvatarInfo.aTitle == 5) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 105) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 205) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 305) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 405))
	{
		mElementDefensePower *= 1.05;
	}
	if ((mUSER[mIndex].mAvatarInfo.aTitle == 6) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 106) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 206) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 306) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 406))
	{
		mElementDefensePower *= 1.06;
	}
	if ((mUSER[mIndex].mAvatarInfo.aTitle == 7) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 107) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 207) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 307) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 407))
	{
		mElementDefensePower *= 1.07;
	}
	if ((mUSER[mIndex].mAvatarInfo.aTitle == 8) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 108) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 208) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 308) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 408))
	{
		mElementDefensePower *= 1.08;
	}
	if ((mUSER[mIndex].mAvatarInfo.aTitle == 9) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 109) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 209) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 309) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 409))
	{
		mElementDefensePower *= 1.09;
	}
	if ((mUSER[mIndex].mAvatarInfo.aTitle == 10) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 110) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 210) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 310) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 410))
	{
		mElementDefensePower *= 1.10;
	}
	if ((mUSER[mIndex].mAvatarInfo.aTitle == 11) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 111) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 211) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 311) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 411))
	{
		mElementDefensePower *= 1.11;
	}
	if ((mUSER[mIndex].mAvatarInfo.aTitle == 12) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 112) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 212) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 312) ||
		(mUSER[mIndex].mAvatarInfo.aTitle == 412))
	{
		mElementDefensePower *= 1.12;
	}

	if (mUSER[mIndex].mAvatarInfo.aRankAbility == 2) mElementDefensePower += 1000;

#ifdef __ANIMAL__
	if ((mDATA.aAnimalNumber == ANIMAL_NUM_DONWHOANG) || (mDATA.aAnimalNumber == ANIMAL_NUM_FERRET1))
	{
		mElementDefensePower = static_cast<int>((float)mElementDefensePower * ANIMAL_RATE_ONE_GRADE);
	}
	if ((mDATA.aAnimalNumber == ANIMAL_NUM_TWODON) || (mDATA.aAnimalNumber == ANIMAL_NUM_FERRET2))
	{
		mElementDefensePower = static_cast<int>((float)mElementDefensePower * ANIMAL_RATE_TWO_GRADE);
	}
	if ((mDATA.aAnimalNumber == ANIMAL_NUM_SUNGDON) || (mDATA.aAnimalNumber == ANIMAL_NUM_FERRET3))
	{
		mElementDefensePower = static_cast<int>((float)mElementDefensePower * ANIMAL_RATE_THREE_GRADE);
	}
#endif
	// Mount Ability
	if (mUSER[mIndex].mAvatarInfo.aAnimalIndex >= 10)
	{
		switch ((mUSER[mIndex].mAvatarInfo.aAnimalAbilityStats[mUSER[mIndex].mAvatarInfo.aAnimalIndex - 10] % 10) / 1)
		{
		case 1:
			mElementDefensePower += 50;
			break;
		case 2:
			mElementDefensePower += 100;
			break;
		case 3:
			mElementDefensePower += 150;
			break;
		case 4:
			mElementDefensePower += 200;
			break;
		case 5:
			mElementDefensePower += 250;
			break;
		case 6:
			mElementDefensePower += 300;
			break;
		case 7:
			mElementDefensePower += 350;
			break;
		case 8:
			mElementDefensePower += 400;
			break;
		case 9:
			mElementDefensePower += 450;
			break;
		}
	}
	// Socket Gem Stats
	for (index01 = 0; index01 < MAX_EQUIP_SLOT_NUM; index01++)
	{
		switch (index01)
		{
		case 0: //[Amulet]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 4)
			{
				mElementDefensePower += 100;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 9)
			{
				mElementDefensePower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 4)
			{
				mElementDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 9)
			{
				mElementDefensePower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 4)
			{
				mElementDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 9)
			{
				mElementDefensePower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 4)
			{
				mElementDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 9)
			{
				mElementDefensePower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 4)
			{
				mElementDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 9)
			{
				mElementDefensePower += 200;
			}
			break;
		case 1: //[Cape]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 4)
			{
				mElementDefensePower += 100;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 9)
			{
				mElementDefensePower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 4)
			{
				mElementDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 9)
			{
				mElementDefensePower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 4)
			{
				mElementDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 9)
			{
				mElementDefensePower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 4)
			{
				mElementDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 9)
			{
				mElementDefensePower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 4)
			{
				mElementDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 9)
			{
				mElementDefensePower += 200;
			}
			break;
		case 2: //[Armor]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 4)
			{
				mElementDefensePower += 100;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 9)
			{
				mElementDefensePower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 4)
			{
				mElementDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 9)
			{
				mElementDefensePower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 4)
			{
				mElementDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 9)
			{
				mElementDefensePower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 4)
			{
				mElementDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 9)
			{
				mElementDefensePower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 4)
			{
				mElementDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 9)
			{
				mElementDefensePower += 200;
			}
			break;
		case 3: //[Gloves]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 4)
			{
				mElementDefensePower += 100;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 9)
			{
				mElementDefensePower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 4)
			{
				mElementDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 9)
			{
				mElementDefensePower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 4)
			{
				mElementDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 9)
			{
				mElementDefensePower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 4)
			{
				mElementDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 9)
			{
				mElementDefensePower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 4)
			{
				mElementDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 9)
			{
				mElementDefensePower += 200;
			}
			break;
		case 4: //[Ring]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 4)
			{
				mElementDefensePower += 100;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 9)
			{
				mElementDefensePower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 4)
			{
				mElementDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 9)
			{
				mElementDefensePower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 4)
			{
				mElementDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 9)
			{
				mElementDefensePower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 4)
			{
				mElementDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 9)
			{
				mElementDefensePower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 4)
			{
				mElementDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 9)
			{
				mElementDefensePower += 200;
			}
			break;
		case 5: //[Boots]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 4)
			{
				mElementDefensePower += 100;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 9)
			{
				mElementDefensePower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 4)
			{
				mElementDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 9)
			{
				mElementDefensePower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 4)
			{
				mElementDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 9)
			{
				mElementDefensePower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 4)
			{
				mElementDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 9)
			{
				mElementDefensePower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 4)
			{
				mElementDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 9)
			{
				mElementDefensePower += 200;
			}
			break;
		case 7: //[Weapon]
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 4)
			{
				mElementDefensePower += 100;
			}
			if ((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10) == 9)
			{
				mElementDefensePower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 4)
			{
				mElementDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100) / 10) == 9)
			{
				mElementDefensePower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 4)
			{
				mElementDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 1000) / 100) == 9)
			{
				mElementDefensePower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 4)
			{
				mElementDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 10000) / 1000) == 9)
			{
				mElementDefensePower += 200;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 4)
			{
				mElementDefensePower += 100;
			}
			if (((mUSER[mIndex].mAvatarInfo.aEquipSocket[index01] % 100000) / 10000) == 9)
			{
				mElementDefensePower += 200;
			}
			break;
		}
	}


	mCritical = 2;
	for (index01 = 0; index01 < MAX_EQUIP_SLOT_NUM; index01++)
	{
		if (mEquipItem[index01] == nullptr)
		{
			continue;
		}
		mCritical += mEquipItem[index01]->iCritical;
		//Warlord Set
		// S5R
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87016) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87017) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87018) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87019) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87020)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87013) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87014) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87015))
			mCritical += (static_cast<int>((double)mEquipItem[index01]->iCritical) * 0.05000000074505806);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87037) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87038) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87039) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87040) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87041)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87034) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87035) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87036))
			mCritical += (static_cast<int>((double)mEquipItem[index01]->iCritical) * 0.05000000074505806);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87058) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87059) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87060) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87061) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87062)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87055) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87056) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87057))
			mCritical += (static_cast<int>((double)mEquipItem[index01]->iCritical) * 0.05000000074505806);
		// S4E
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87074) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87075) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87076)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87071) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87072) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87073))
			mCritical += (static_cast<int>((double)mEquipItem[index01]->iCritical) * 0.05000000074505806);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87096) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87097) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87098)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87093) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87094) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87095))
			mCritical += (static_cast<int>((double)mEquipItem[index01]->iCritical) * 0.05000000074505806);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87118) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87119) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87120)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87115) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87116) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87117))
			mCritical += (static_cast<int>((double)mEquipItem[index01]->iCritical) * 0.05000000074505806);
		// S5E
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87080) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87081) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87082) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87083) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87084)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87077) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87078) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87079))
			mCritical += (static_cast<int>((double)mEquipItem[index01]->iCritical) * 0.05000000074505806);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87102) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87103) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87104) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87105) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87106)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87099) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87100) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87101))
			mCritical += (static_cast<int>((double)mEquipItem[index01]->iCritical) * 0.05000000074505806);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87124) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87125) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87126) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87127) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87128)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87121) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87122) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87123))
			mCritical += (static_cast<int>((double)mEquipItem[index01]->iCritical) * 0.05000000074505806);
		// 88001 Set
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88004) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88005) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88006) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88007) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88008)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88001) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88002) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88003))
			mCritical += (static_cast<int>((double)mEquipItem[index01]->iCritical) * 0.06000000074505806);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88012) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88013) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88014) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88015) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88016)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88009) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88010) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88011))
			mCritical += (static_cast<int>((double)mEquipItem[index01]->iCritical) * 0.06000000074505806);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88020) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88021) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88022) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88023) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88024)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88017) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88018) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88019))
			mCritical += (static_cast<int>((double)mEquipItem[index01]->iCritical) * 0.06000000074505806);
		// 88025 Set
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88028) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88029) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88030) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88031) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88032)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88025) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88026) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88027))
			mCritical += (static_cast<int>((double)mEquipItem[index01]->iCritical) * 0.07000000074505806);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88036) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88037) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88038) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88039) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88040)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88033) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88034) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88035))
			mCritical += (static_cast<int>((double)mEquipItem[index01]->iCritical) * 0.07000000074505806);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88044) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88045) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 88046) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88047) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 88048)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88041) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88042) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88043))
			mCritical += (static_cast<int>((double)mEquipItem[index01]->iCritical) * 0.07000000074505806);

		switch (index01)
		{
		case 4:
			tISValue = mUTIL.ReturnISValueWithLevelLimit(tAvatarInfo->aLevel1,
			                                             mUTIL.ReturnISValue(mEquipValue[index01]));
			if (tISValue > 0)
			{
				mCritical += (tISValue / 4);
			}
			break;
		}
	}
	// Critical Boost Scrolls
	if (mUSER[mIndex].mAvatarInfo.aCriticalBoostScroll > 0)
	{
		mCritical += 5;
	}

#ifdef __ANIMAL__
	if ((mDATA.aAnimalNumber == ANIMAL_NUM_CHUNLOK) || (mDATA.aAnimalNumber == ANIMAL_NUM_WOLF1))
	{
		mCritical = static_cast<int>((float)mCritical * ANIMAL_RATE_ONE_GRADE);
	}
	if ((mDATA.aAnimalNumber == ANIMAL_NUM_BAIKLOK) || (mDATA.aAnimalNumber == ANIMAL_NUM_WOLF2))
	{
		mCritical = static_cast<int>((float)mCritical * ANIMAL_RATE_TWO_GRADE);
	}
	if ((mDATA.aAnimalNumber == ANIMAL_NUM_SHINLOK) || (mDATA.aAnimalNumber == ANIMAL_NUM_WOLF3))
	{
		mCritical = static_cast<int>((float)mCritical * ANIMAL_RATE_THREE_GRADE);
	}
#endif
	// Critical Increase from Rebirth
	if (mUSER[mIndex].mAvatarInfo.aRebirth == 7) mCritical += 1;
	if (mUSER[mIndex].mAvatarInfo.aRebirth == 8) mCritical += 2;
	if (mUSER[mIndex].mAvatarInfo.aRebirth == 9) mCritical += 3;
	if (mUSER[mIndex].mAvatarInfo.aRebirth == 10) mCritical += 4;
	if (mUSER[mIndex].mAvatarInfo.aRebirth == 11) mCritical += 5;
	if (mUSER[mIndex].mAvatarInfo.aRebirth == 12) mCritical += 8;

	// S1R V2
	if ((mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87129) && (mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87130))
		mCritical += 2;
	if ((mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87150) && (mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87151))
		mCritical += 2;
	if ((mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87171) && (mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87172))
		mCritical += 2;
	// S5R V2
	if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87145) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87146) && (mUSER
		[mIndex].mAvatarInfo.aEquip[5][0] == 87147) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87148) && (mUSER[
		mIndex].mAvatarInfo.aEquip[0][0] == 87149)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87142) || (mUSER[
		mIndex].mAvatarInfo.aEquip[7][0] == 87143) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87144)) mCritical += 2;
	if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87166) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87167) && (mUSER
		[mIndex].mAvatarInfo.aEquip[5][0] == 87168) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87169) && (mUSER[
		mIndex].mAvatarInfo.aEquip[0][0] == 87170)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87163) || (mUSER[
		mIndex].mAvatarInfo.aEquip[7][0] == 87164) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87165)) mCritical += 2;
	if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87187) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87188) && (mUSER
		[mIndex].mAvatarInfo.aEquip[5][0] == 87189) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87190) && (mUSER[
		mIndex].mAvatarInfo.aEquip[0][0] == 87191)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87184) || (mUSER[
		mIndex].mAvatarInfo.aEquip[7][0] == 87185) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87186)) mCritical += 2;
	// S1E V2
	if ((mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87192) && (mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87193))
		mCritical += 2;
	if ((mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87214) && (mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87215))
		mCritical += 2;
	if ((mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87236) && (mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87237))
		mCritical += 2;
	// S4E V2
	if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87203) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87204) && (mUSER
		[mIndex].mAvatarInfo.aEquip[5][0] == 87205)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87200) || (mUSER[
		mIndex].mAvatarInfo.aEquip[7][0] == 87201) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87202)) mCritical += 5;
	if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87225) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87226) && (mUSER
		[mIndex].mAvatarInfo.aEquip[5][0] == 87227)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87222) || (mUSER[
		mIndex].mAvatarInfo.aEquip[7][0] == 87223) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87224)) mCritical += 5;
	if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87247) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87248) && (mUSER
		[mIndex].mAvatarInfo.aEquip[5][0] == 87249)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87244) || (mUSER[
		mIndex].mAvatarInfo.aEquip[7][0] == 87245) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87246)) mCritical += 5;
	// S5E V2
	if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87209) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87210) && (mUSER
		[mIndex].mAvatarInfo.aEquip[5][0] == 87211) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87212) && (mUSER[
		mIndex].mAvatarInfo.aEquip[0][0] == 87213)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87206) || (mUSER[
		mIndex].mAvatarInfo.aEquip[7][0] == 87207) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87208)) mCritical += 7;
	if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87231) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87232) && (mUSER
		[mIndex].mAvatarInfo.aEquip[5][0] == 87233) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87234) && (mUSER[
		mIndex].mAvatarInfo.aEquip[0][0] == 87235)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87228) || (mUSER[
		mIndex].mAvatarInfo.aEquip[7][0] == 87229) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87230)) mCritical += 7;
	if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87253) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87254) && (mUSER
		[mIndex].mAvatarInfo.aEquip[5][0] == 87255) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87256) && (mUSER[
		mIndex].mAvatarInfo.aEquip[0][0] == 87257)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87250) || (mUSER[
		mIndex].mAvatarInfo.aEquip[7][0] == 87251) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87252)) mCritical += 7;

	//Refine
	mCritical = static_cast<int>((mUTIL.ReturnRFValue(mUSER[mIndex].mAvatarInfo.aEquip[7][2]) * 0.009999999776482582) *
		mCritical) + mCritical;

	mCriticalDefense = 0;
	for (index01 = 0; index01 < MAX_EQUIP_SLOT_NUM; index01++)
	{
		if (mEquipItem[index01] == nullptr)
		{
			continue;
		}
		mCriticalDefense += mEquipItem[index01]->iCapeInfo[1];
		// Warlord Set
		// S4E
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87074) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87075) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87076)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87071) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87072) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87073))
			mCriticalDefense += (static_cast<int>((double)mEquipItem[index01]->iCapeInfo[1]) * 0.05000000074505806);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87096) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87097) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87098)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87093) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87094) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87095))
			mCriticalDefense += (static_cast<int>((double)mEquipItem[index01]->iCapeInfo[1]) * 0.05000000074505806);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87118) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87119) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87120)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87115) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87116) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87117))
			mCriticalDefense += (static_cast<int>((double)mEquipItem[index01]->iCapeInfo[1]) * 0.05000000074505806);
	}
	// S5R V2
	if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87145) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87146) && (mUSER
		[mIndex].mAvatarInfo.aEquip[5][0] == 87147) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87148) && (mUSER[
		mIndex].mAvatarInfo.aEquip[0][0] == 87149)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87142) || (mUSER[
		mIndex].mAvatarInfo.aEquip[7][0] == 87143) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87144))
		mCriticalDefense += 2;
	if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87166) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87167) && (mUSER
		[mIndex].mAvatarInfo.aEquip[5][0] == 87168) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87169) && (mUSER[
		mIndex].mAvatarInfo.aEquip[0][0] == 87170)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87163) || (mUSER[
		mIndex].mAvatarInfo.aEquip[7][0] == 87164) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87165))
		mCriticalDefense += 2;
	if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87187) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87188) && (mUSER
		[mIndex].mAvatarInfo.aEquip[5][0] == 87189) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87190) && (mUSER[
		mIndex].mAvatarInfo.aEquip[0][0] == 87191)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87184) || (mUSER[
		mIndex].mAvatarInfo.aEquip[7][0] == 87185) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87186))
		mCriticalDefense += 2;
	// S5E
	if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87080) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87081) && (mUSER
		[mIndex].mAvatarInfo.aEquip[5][0] == 87082) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87083) && (mUSER[
		mIndex].mAvatarInfo.aEquip[0][0] == 87084)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87077) || (mUSER[
		mIndex].mAvatarInfo.aEquip[7][0] == 87078) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87079))
		mCriticalDefense += 6;
	if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87102) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87103) && (mUSER
		[mIndex].mAvatarInfo.aEquip[5][0] == 87104) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87105) && (mUSER[
		mIndex].mAvatarInfo.aEquip[0][0] == 87106)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87099) || (mUSER[
		mIndex].mAvatarInfo.aEquip[7][0] == 87100) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87101))
		mCriticalDefense += 6;
	if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87124) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87125) && (mUSER
		[mIndex].mAvatarInfo.aEquip[5][0] == 87126) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87127) && (mUSER[
		mIndex].mAvatarInfo.aEquip[0][0] == 87128)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87121) || (mUSER[
		mIndex].mAvatarInfo.aEquip[7][0] == 87122) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87123))
		mCriticalDefense += 6;
	// S5E V2
	if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87209) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87210) && (mUSER
		[mIndex].mAvatarInfo.aEquip[5][0] == 87211) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87212) && (mUSER[
		mIndex].mAvatarInfo.aEquip[0][0] == 87213)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87206) || (mUSER[
		mIndex].mAvatarInfo.aEquip[7][0] == 87207) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87208))
		mCriticalDefense += 7;
	if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87231) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87232) && (mUSER
		[mIndex].mAvatarInfo.aEquip[5][0] == 87233) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87234) && (mUSER[
		mIndex].mAvatarInfo.aEquip[0][0] == 87235)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87228) || (mUSER[
		mIndex].mAvatarInfo.aEquip[7][0] == 87229) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87230))
		mCriticalDefense += 7;
	if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87253) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87254) && (mUSER
		[mIndex].mAvatarInfo.aEquip[5][0] == 87255) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87256) && (mUSER[
		mIndex].mAvatarInfo.aEquip[0][0] == 87257)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87250) || (mUSER[
		mIndex].mAvatarInfo.aEquip[7][0] == 87251) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87252))
		mCriticalDefense += 7;
	// 88001 Set
	if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88004) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88005) && (mUSER
		[mIndex].mAvatarInfo.aEquip[5][0] == 88006) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88007) && (mUSER[
		mIndex].mAvatarInfo.aEquip[0][0] == 88008)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88001) || (mUSER[
		mIndex].mAvatarInfo.aEquip[7][0] == 88002) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88003))
		mCriticalDefense += 8;
	if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88012) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88013) && (mUSER
		[mIndex].mAvatarInfo.aEquip[5][0] == 88014) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88015) && (mUSER[
		mIndex].mAvatarInfo.aEquip[0][0] == 88016)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88009) || (mUSER[
		mIndex].mAvatarInfo.aEquip[7][0] == 88010) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88011))
		mCriticalDefense += 8;
	if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88020) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88021) && (mUSER
		[mIndex].mAvatarInfo.aEquip[5][0] == 88022) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88023) && (mUSER[
		mIndex].mAvatarInfo.aEquip[0][0] == 88024)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88017) || (mUSER[
		mIndex].mAvatarInfo.aEquip[7][0] == 88018) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88019))
		mCriticalDefense += 8;
	// 88025 Set
	if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88028) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88029) && (mUSER
		[mIndex].mAvatarInfo.aEquip[5][0] == 88030) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88031) && (mUSER[
		mIndex].mAvatarInfo.aEquip[0][0] == 88032)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88025) || (mUSER[
		mIndex].mAvatarInfo.aEquip[7][0] == 88026) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88027))
		mCriticalDefense += 9;
	if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88036) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88037) && (mUSER
		[mIndex].mAvatarInfo.aEquip[5][0] == 88038) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88039) && (mUSER[
		mIndex].mAvatarInfo.aEquip[0][0] == 88040)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88033) || (mUSER[
		mIndex].mAvatarInfo.aEquip[7][0] == 88034) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88035))
		mCriticalDefense += 9;
	if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 88044) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 88045) && (mUSER
		[mIndex].mAvatarInfo.aEquip[5][0] == 88046) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 88047) && (mUSER[
		mIndex].mAvatarInfo.aEquip[0][0] == 88048)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88041) || (mUSER[
		mIndex].mAvatarInfo.aEquip[7][0] == 88042) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 88043))
		mCriticalDefense += 9;
	//Critical Defense From Rebirth
	if (mUSER[mIndex].mAvatarInfo.aRebirth == 1) mCriticalDefense += 1;
	if (mUSER[mIndex].mAvatarInfo.aRebirth == 2) mCriticalDefense += 2;
	if (mUSER[mIndex].mAvatarInfo.aRebirth == 3) mCriticalDefense += 3;
	if (mUSER[mIndex].mAvatarInfo.aRebirth == 4) mCriticalDefense += 4;
	if (mUSER[mIndex].mAvatarInfo.aRebirth == 5) mCriticalDefense += 5;
	if (mUSER[mIndex].mAvatarInfo.aRebirth >= 6) mCriticalDefense += 6;

	//Refine
	mCriticalDefense = static_cast<int>((mUTIL.ReturnRFValue(mUSER[mIndex].mAvatarInfo.aEquip[1][2]) *
		0.01600000075995922) * mCriticalDefense) + mCriticalDefense;

	// Critical Defense from Halo
	if (mUSER[mIndex].mAvatarInfo.aUpgradeValue == 96)
	{
		mCriticalDefense += 10;
	}
	else
	{
		mCriticalDefense += mUSER[mIndex].mAvatarInfo.aUpgradeValue / 10;
	}


	mLastAttackBonusProbability = 0;
	for (index01 = 0; index01 < MAX_EQUIP_SLOT_NUM; index01++)
	{
		if (mEquipItem[index01] == nullptr)
		{
			continue;
		}
		mLastAttackBonusProbability += mEquipItem[index01]->iLastAttackBonusInfo[0];
	}
	mLastAttackBonusValue = 0.0f;
	for (index01 = 0; index01 < MAX_EQUIP_SLOT_NUM; index01++)
	{
		if (mEquipItem[index01] == nullptr)
		{
			continue;
		}
		mLastAttackBonusValue += static_cast<float>(mEquipItem[index01]->iLastAttackBonusInfo[1]);
	}
	mLastAttackBonusValue *= 0.01f;
	mLuck = 0;
	for (index01 = 0; index01 < MAX_EQUIP_SLOT_NUM; index01++)
	{
		if (mEquipItem[index01] == nullptr)
		{
			continue;
		}
		mLuck += mEquipItem[index01]->iLuck;
		//Warlord
		// S5E
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87080) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87081) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87082) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87083) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87084)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87077) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87078) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87079)) mLuck
			+= (static_cast<int>((double)mEquipItem[index01]->iLuck) * 0.05000000074505806);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87102) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87103) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87104) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87105) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87106)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87099) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87100) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87101)) mLuck
			+= (static_cast<int>((double)mEquipItem[index01]->iLuck) * 0.05000000074505806);
		if ((mUSER[mIndex].mAvatarInfo.aEquip[2][0] == 87124) && (mUSER[mIndex].mAvatarInfo.aEquip[3][0] == 87125) && (
			mUSER[mIndex].mAvatarInfo.aEquip[5][0] == 87126) && (mUSER[mIndex].mAvatarInfo.aEquip[4][0] == 87127) && (
			mUSER[mIndex].mAvatarInfo.aEquip[0][0] == 87128)) if ((mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87121) || (
			mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87122) || (mUSER[mIndex].mAvatarInfo.aEquip[7][0] == 87123)) mLuck
			+= (static_cast<int>((double)mEquipItem[index01]->iLuck) * 0.05000000074505806);

		switch (index01)
		{
		case 0:
			tISValue = mUTIL.ReturnISValueWithLevelLimit(tAvatarInfo->aLevel1,
			                                             mUTIL.ReturnISValue(mEquipValue[index01]));
			if (tISValue > 0)
			{
				mLuck += (tISValue * 12);
			}
			break;
		}
	}

	/*	Logger("mMaxLife : %d", mMaxLife);
		Logger("mMaxMana : %d", mMaxMana);
		Logger("mAttackPower : %d", mAttackPower);
		Logger("mDefensePower : %d", mDefensePower);
		Logger("mAttackSuccess : %d", mAttackSuccess);
		Logger("mAttackBlock : %d", mAttackBlock);
		Logger("mElementAttackPower : %d", mElementAttackPower);
		Logger("mElementDefensePower : %d", mElementDefensePower);
		Logger("mCritical : %d", mCritical);
		Logger("mCriticalDefense : %d", mCriticalDefense);
	*/
}

//GET_WEAPON_CLASS
int AVATAR_OBJECT::GetWeaponClass(void)
{
	if (mEquipItem[7] == nullptr)
	{
		return 0;
	}
	switch (mEquipItem[7]->iSort)
	{
	case 13: return 1;
	case 14: return 2;
	case 15: return 3;
	case 16: return 1;
	case 17: return 2;
	case 18: return 3;
	case 19: return 1;
	case 20: return 2;
	case 21: return 3;
	}
	return 0;
}

//GET_MAX_LIFE
int AVATAR_OBJECT::GetMaxLife(void)
{
	return mMaxLife;
}

//GET_MAX_MANA
int AVATAR_OBJECT::GetMaxMana(void)
{
	return mMaxMana;
}

//GET_ATTACK_POWER
int AVATAR_OBJECT::GetAttackPower(void)
{
	int tValue = mAttackPower;

	if (mUSER[mIndex].mEffectValue[0][0] > 0)
	{
		tValue = static_cast<int>((float)(tValue * (mUSER[mIndex].mEffectValue[0][0] + 100)) * 0.01f);
	}
	if (tValue < static_cast<int>(mPAT.ReturnAttackPower(mUSER[mIndex].mAvatarInfo.aEquip[8][0],
	                                                     mUSER[mIndex].mAvatarInfo.aEquip[8][2],
	                                                     mUSER[mIndex].mAvatarInfo.aEquip[8][1])))
	{
		tValue += tValue;
	}
	else
	{
		tValue += static_cast<int>(mPAT.ReturnAttackPower(mUSER[mIndex].mAvatarInfo.aEquip[8][0],
		                                                  mUSER[mIndex].mAvatarInfo.aEquip[8][2],
		                                                  mUSER[mIndex].mAvatarInfo.aEquip[8][1]));
	}
	// Proving Ground Bonus Damage
	if ((mGAME.mWorldInfo.mProvingGroundsPoint[0] > 0) && (mUSER[mIndex].mAvatarInfo.aTribe == 0))
	{
		tValue += mGAME.mWorldInfo.mProvingGroundsPoint[0] * 100;
	}
	if ((mGAME.mWorldInfo.mProvingGroundsPoint[1] > 0) && (mUSER[mIndex].mAvatarInfo.aTribe == 1))
	{
		tValue += mGAME.mWorldInfo.mProvingGroundsPoint[1] * 100;
	}
	if ((mGAME.mWorldInfo.mProvingGroundsPoint[2] > 0) && (mUSER[mIndex].mAvatarInfo.aTribe == 2))
	{
		tValue += mGAME.mWorldInfo.mProvingGroundsPoint[2] * 100;
	}
	if ((mGAME.mWorldInfo.mProvingGroundsPoint[3] > 0) && (mUSER[mIndex].mAvatarInfo.aTribe == 3))
	{
		tValue += mGAME.mWorldInfo.mProvingGroundsPoint[3] * 100;
	}
	return tValue;
}

//GET_DEFENSE_POWER
int AVATAR_OBJECT::GetDefensePower(void)
{
	int tValue = mDefensePower;

	if (mUSER[mIndex].mEffectValue[1][0] > 0)
	{
		tValue = static_cast<int>((float)(tValue * (mUSER[mIndex].mEffectValue[1][0] + 100)) * 0.01f);
	}
	if (tValue < static_cast<int>(mPAT.ReturnDefensePower(mUSER[mIndex].mAvatarInfo.aEquip[8][0],
	                                                      mUSER[mIndex].mAvatarInfo.aEquip[8][2],
	                                                      mUSER[mIndex].mAvatarInfo.aEquip[8][1])))
	{
		tValue += tValue;
	}
	else
	{
		tValue += static_cast<int>(mPAT.ReturnDefensePower(mUSER[mIndex].mAvatarInfo.aEquip[8][0],
		                                                   mUSER[mIndex].mAvatarInfo.aEquip[8][2],
		                                                   mUSER[mIndex].mAvatarInfo.aEquip[8][1]));
	}
	return tValue;
}

//GET_ATTACK_SUCCESS
int AVATAR_OBJECT::GetAttackSuccess(void)
{
	int tValue = mAttackSuccess;

	if (mUSER[mIndex].mEffectValue[2][0] > 0)
	{
		tValue = static_cast<int>((float)(tValue * (mUSER[mIndex].mEffectValue[2][0] + 100)) * 0.01f);
	}
	if (mUSER[mIndex].mEffectValue[17][0] > 0)
	{
		tValue *= 1.25f;
	}
	return tValue;
}

//GET_ATTACK_BLOCK
int AVATAR_OBJECT::GetAttackBlock(void)
{
	int tValue = mAttackBlock;

	if (mUSER[mIndex].mEffectValue[3][0] > 0)
	{
		tValue = static_cast<int>((float)(tValue * (mUSER[mIndex].mEffectValue[3][0] + 100)) * 0.01f);
	}
	if (mUSER[mIndex].mEffectValue[18][0] > 0)
	{
		tValue *= 1.25f;
	}
	return tValue;
}

//GET_ELEMENT_ATTACK_POWER
int AVATAR_OBJECT::GetElementAttackPower(void)
{
	int tValue = mElementAttackPower;

	if (mUSER[mIndex].mEffectValue[4][0] > 0)
	{
		tValue = static_cast<int>((float)(tValue * (mUSER[mIndex].mEffectValue[4][0] + 100)) * 0.01f);
	}
	return tValue;
}

//GET_ELEMENT_DEFENSE_POWER
int AVATAR_OBJECT::GetElementDefensePower(void)
{
	int tValue = mElementDefensePower;

	if (mUSER[mIndex].mEffectValue[5][0] > 0)
	{
		tValue = static_cast<int>((float)(tValue * (mUSER[mIndex].mEffectValue[5][0] + 100)) * 0.01f);
	}
	return tValue;
}

//GET_CRITICAL
int AVATAR_OBJECT::GetCritical(void)
{
	int tValue = mCritical;

	if (mUSER[mIndex].mEffectValue[10][0] > 0)
	{
		tValue = static_cast<int>((float)(tValue * (mUSER[mIndex].mEffectValue[10][0] + 100)) * 0.01f);
	}
	if (tValue > 150) tValue = 0; // Hack Protection
	return tValue;
}

//GET_CRITICAL_DEFENSE
int AVATAR_OBJECT::GetCriticalDefense(void)
{
	return mCriticalDefense;
}

//GET_LAST_ATTACK_BONUS_PROBABILITY
int AVATAR_OBJECT::GetLastAttackBonusProbability(void)
{
	return mLastAttackBonusProbability;
}

//GET_LAST_ATTACK_BONUS_VALUE
float AVATAR_OBJECT::GetLastAttackBonusValue(void)
{
	return mLastAttackBonusValue;
}

//GET_LUCK
int AVATAR_OBJECT::GetLuck(void)
{
	int tValue = mLuck;

	if (mUSER[mIndex].mEffectValue[11][0] > 0)
	{
		tValue = static_cast<int>((float)(tValue * (mUSER[mIndex].mEffectValue[11][0] + 100)) * 0.01f);
	}
	return tValue;
}

//GET_BONUS_SKILL_VALUE
int AVATAR_OBJECT::GetBonusSkillValue(int sIndex)
{
	int index01;
	int tValue = 0;

	for (index01 = 0; index01 < MAX_EQUIP_SLOT_NUM; index01++)
	{
		if (mEquipItem[index01] == nullptr)
		{
			continue;
		}
		if (mEquipItem[index01]->iBonusSkillInfo[0][0] == sIndex)
		{
			tValue += mEquipItem[index01]->iBonusSkillInfo[0][1];
		}
		if (mEquipItem[index01]->iBonusSkillInfo[1][0] == sIndex)
		{
			tValue += mEquipItem[index01]->iBonusSkillInfo[1][1];
		}
		if (mEquipItem[index01]->iBonusSkillInfo[2][0] == sIndex)
		{
			tValue += mEquipItem[index01]->iBonusSkillInfo[2][1];
		}
		if (mEquipItem[index01]->iBonusSkillInfo[3][0] == sIndex)
		{
			tValue += mEquipItem[index01]->iBonusSkillInfo[3][1];
		}
		if (mEquipItem[index01]->iBonusSkillInfo[4][0] == sIndex)
		{
			tValue += mEquipItem[index01]->iBonusSkillInfo[4][1];
		}
#ifdef __GOD__
		if (mEquipItem[index01]->iBonusSkillInfo[5][0] == sIndex)
		{
			tValue += mEquipItem[index01]->iBonusSkillInfo[5][1];
		}
		if (mEquipItem[index01]->iBonusSkillInfo[6][0] == sIndex)
		{
			tValue += mEquipItem[index01]->iBonusSkillInfo[6][1];
		}
		if (mEquipItem[index01]->iBonusSkillInfo[7][0] == sIndex)
		{
			tValue += mEquipItem[index01]->iBonusSkillInfo[7][1];
		}
#endif
		tValue += mEquipItem[index01]->iCapeInfo[2];
	}
	return tValue;
}

//GET_REDUCE_MANA_RATIO
int AVATAR_OBJECT::GetReduceManaRatio(void)
{
	int index01;
	int tValue = 0;

	for (index01 = 0; index01 < MAX_EQUIP_SLOT_NUM; index01++)
	{
		if (mEquipItem[index01] == nullptr)
		{
			continue;
		}
		tValue += mEquipItem[index01]->iCapeInfo[0];
	}
	return tValue;
}

//CHECK_COMMUNITY_WORK
BOOL AVATAR_OBJECT::CheckCommunityWork(void)
{
	if (mDATA.aPShopState != 0)
	{
		return TRUE;
	}
	if (mDuelProcessState != 0)
	{
		return TRUE;
	}
	if (mTradeProcessState != 0)
	{
		return TRUE;
	}
	if (mFriendProcessState != 0)
	{
		return TRUE;
	}
	if (mPartyProcessState != 0)
	{
		return TRUE;
	}
	if (mGuildProcessState != 0)
	{
		return TRUE;
	}
	if (mTeacherProcessState != 0)
	{
		return TRUE;
	}
	return FALSE;
}

//UPDATE
void AVATAR_OBJECT::Update(float tPostTime)
{
	if (!mCheckValidState)
	{
		return;
	}
	if ((mUSER[mIndex].mMoveZoneResult == 1))
	{
		mUSER[mIndex].mHeartCheckCount = 0;
		mUSER[mIndex].mHeartCheckTime = time(nullptr);
		return;
	}

	int index01;
	int HotKeyPage;
	int HotKeyIndex;
	int PotionPage;
	int PotionIndex;
	int PotionQuantity;
	BYTE tData[MAX_BROADCAST_DATA_SIZE];
	int tMaxAvatarLife;
	int tRecoverAvatarLife;
	int tMaxAvatarMana;
	int tRecoverAvatarMana;
	float tLifeRecoverTime;
	float tManaRecoverTime;
	int tEffectValueState[MAX_AVATAR_EFFECT_SORT_NUM];
	BOOL tCheckChangeEffectValue;
	int tOtherAvatarIndex;
	int iTemp_Auto_time;

	if (mUSER[mIndex].mAuthInfo.AuthType == 0)
	{
		if ((mGAME.mCheckZone049TypeServer) || (mGAME.mCheckZone051TypeServer) || (mGAME.mCheckZone053TypeServer))
		{
			if ((mDATA.aAction.aLocation[0] < mWORLD.mQuadtree[0].mBoxMin[0]) || (mDATA.aAction.aLocation[0] > mWORLD.
				mQuadtree[0].mBoxMax[0]) || (mDATA.aAction.aLocation[1] < mWORLD.mQuadtree[0].mBoxMin[1]) || (mDATA.
				aAction.aLocation[1] > mWORLD.mQuadtree[0].mBoxMax[1]) || (mDATA.aAction.aLocation[2] < mWORLD.mQuadtree
				[0].mBoxMin[2]) || (mDATA.aAction.aLocation[2] > mWORLD.mQuadtree[0].mBoxMax[2]))
			{
				mUSER[mIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
		}
	}
	if (mDATA.aPShopState == 1)
	{
		if (!mWORK.CheckPossiblePShopRegion(mDATA.aTribe, mSERVER_INFO.mServerNumber, &mDATA.aAction.aLocation[0]))
		{
			mUSER[mIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
	}
	/* Quest Summon */
	SummonQuestBoss();
	/* DeathStrike Unable to Pots Update */
	if ((mAVATAR_OBJECT[mIndex].mDATA.bStateDarkAttack == true) && ((GetTickCount() - mUSER[mIndex].
		mTickCountForDarkAttack) > 2000))
	{
		mAVATAR_OBJECT[mIndex].mDATA.bStateDarkAttack = false;
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(45, mAVATAR_OBJECT[mIndex].mDATA.bStateDarkAttack);
		mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	}

	if ((mGAME.mTickCount - mUSER[mIndex].mTickCountFor01Second) == 2)
	{
		mUSER[mIndex].mTickCountFor01Second = mGAME.mTickCount;
		if (mDATA.aAction.aSort == 11)
		{
			mDATA.aAction.aSkillValue--;
			if (mDATA.aAction.aSkillValue < 1)
			{
				mDATA.aAction.aType = GetWeaponClass() * 2;
				mDATA.aAction.aSort = 1;
				mDATA.aAction.aFrame = 0.0f;
				mTRANSFER.B_AVATAR_ACTION_RECV(mIndex, mUniqueNumber, &mDATA, 1);
				mUTIL.Broadcast(TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false);
			}
		}
		tMaxAvatarLife = GetMaxLife();
		tRecoverAvatarLife = 0;
		if (mDATA.aAction.aSort == 31)
		{
			if (mDATA.aLifeValue < tMaxAvatarLife)
			{
				tLifeRecoverTime = mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
				                                           (mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.
					                                           aSkillGradeNum2), 2);
				if (tLifeRecoverTime > 0.0f)
				{
					tRecoverAvatarLife = static_cast<int>((float)tMaxAvatarLife / tLifeRecoverTime);
					if (tRecoverAvatarLife < 1)
					{
						tRecoverAvatarLife = 1;
					}
					else
					{
						if ((mDATA.aLifeValue + tRecoverAvatarLife) > tMaxAvatarLife)
						{
							tRecoverAvatarLife = tMaxAvatarLife - mDATA.aLifeValue;
						}
					}
				}
			}
		}
		if (tRecoverAvatarLife > 0)
		{
			mDATA.aLifeValue += tRecoverAvatarLife;
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(10, mDATA.aLifeValue);
			mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		}
		tMaxAvatarMana = GetMaxMana();
		tRecoverAvatarMana = 0;
		if (mDATA.aAction.aSort == 31)
		{
			if (mDATA.aManaValue < tMaxAvatarMana)
			{
				tManaRecoverTime = mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
				                                           (mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.
					                                           aSkillGradeNum2), 3);
				if (tManaRecoverTime > 0.0f)
				{
					tRecoverAvatarMana = static_cast<int>((float)tMaxAvatarMana / tManaRecoverTime);
					if (tRecoverAvatarMana < 1)
					{
						tRecoverAvatarMana = 1;
					}
					else
					{
						if ((mDATA.aManaValue + tRecoverAvatarMana) > tMaxAvatarMana)
						{
							tRecoverAvatarMana = tMaxAvatarMana - mDATA.aManaValue;
						}
					}
				}
			}
		}
		if (tRecoverAvatarMana > 0)
		{
			mDATA.aManaValue += tRecoverAvatarMana;
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(11, mDATA.aManaValue);
			mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		}
		if (mUSER[mIndex].mAvatarInfo.aAutoHuntState == 1)
		{
			if (mUTIL.CheckIfEmptyHPPills(mIndex))
			{
				if ((mUTIL.GetEmptyHotKeyPage(mIndex) != -1) && (mUTIL.GetEmptyHotKeyIndex(mIndex) != -1))
				{
					if ((mUTIL.GetInventoryHPPillPage(mIndex) != -1) && (mUTIL.GetInventoryHPPillIndex(mIndex) != -1))
					{
						HotKeyPage = mUTIL.GetEmptyHotKeyPage(mIndex);
						HotKeyIndex = mUTIL.GetEmptyHotKeyIndex(mIndex);
						PotionPage = mUTIL.GetInventoryHPPillPage(mIndex);
						PotionIndex = mUTIL.GetInventoryHPPillIndex(mIndex);
						PotionQuantity = mUTIL.GetInventoryHPPillQuantity(mIndex);
						CopyMemory(&tData[0], &PotionPage, 4);
						CopyMemory(&tData[4], &PotionIndex, 4);
						CopyMemory(&tData[8], &PotionQuantity, 4);
						CopyMemory(&tData[12], &HotKeyPage, 4);
						CopyMemory(&tData[16], &HotKeyIndex, 4);
						mWORK.ProcessForData(mIndex, 253, &tData[0]);
					}
				}
			}
			if (mUTIL.CheckIfEmptyMPPills(mIndex))
			{
				if ((mUTIL.GetEmptyHotKeyPage(mIndex) != -1) && (mUTIL.GetEmptyHotKeyIndex(mIndex) != -1))
				{
					if ((mUTIL.GetInventoryMPPillPage(mIndex) != -1) && (mUTIL.GetInventoryMPPillIndex(mIndex) != -1))
					{
						HotKeyPage = mUTIL.GetEmptyHotKeyPage(mIndex);
						HotKeyIndex = mUTIL.GetEmptyHotKeyIndex(mIndex);
						PotionPage = mUTIL.GetInventoryMPPillPage(mIndex);
						PotionIndex = mUTIL.GetInventoryMPPillIndex(mIndex);
						PotionQuantity = mUTIL.GetInventoryMPPillQuantity(mIndex);
						CopyMemory(&tData[0], &PotionPage, 4);
						CopyMemory(&tData[4], &PotionIndex, 4);
						CopyMemory(&tData[8], &PotionQuantity, 4);
						CopyMemory(&tData[12], &HotKeyPage, 4);
						CopyMemory(&tData[16], &HotKeyIndex, 4);
						mWORK.ProcessForData(mIndex, 253, &tData[0]);
					}
				}
			}
		}
		tCheckChangeEffectValue = FALSE;
		for (index01 = 0; index01 < MAX_AVATAR_EFFECT_SORT_NUM; index01++)
		{
			tEffectValueState[index01] = 0;
		}
		for (index01 = 0; index01 < MAX_AVATAR_EFFECT_SORT_NUM; index01++)
		{
			if (mUSER[mIndex].mEffectValue[index01][0] < 1)
			{
				continue;
			}
			mUSER[mIndex].mEffectValue[index01][1]--;
			if (mUSER[mIndex].mEffectValue[index01][1] <= 0)
			{
				tCheckChangeEffectValue = TRUE;
				mUSER[mIndex].mEffectValue[index01][0] = 0;
				mUSER[mIndex].mEffectValue[index01][1] = 0;
				mDATA.aEffectValueForView[index01] = 0;
				tEffectValueState[index01] = 2;

				/* DeathStrike Update */
				if (index01 == 15)
				{
					mUSER[mIndex].iKindDarkAttack = 0;
				}
			}
		}
		if (tCheckChangeEffectValue)
		{
			mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO(mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue,
			                                     &tEffectValueState[0]);
			mUTIL.Broadcast(TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false);
			if (mUSER[mIndex].mAvatarInfo.aAutoHuntState == 1)
			{
				AutoHuntBuff();
			}
		}
		if (mDuelProcessState == 3)
		{
			if (mUTIL.SearchAvatar(mDuelProcessAvatarName, mIndex) == -1)
			{
				mDuelProcessState = 0;
			}
		}
		if (124 != mSERVER_INFO.mServerNumber)
		{
			if (mDATA.aDuelState[0] == 1)
			{
				switch (mDATA.aDuelState[2])
				{
				case 1:
					tOtherAvatarIndex = mUTIL.SearchAvatar(mDuelProcessAvatarName, mIndex);
					if (tOtherAvatarIndex == -1)
					{
						mTRANSFER.B_DUEL_END_RECV(3);
						mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
						mDATA.aDuelState[0] = 0;
						mDATA.aDuelState[1] = 0;
						mDATA.aDuelState[2] = 0;
						mCheckPossibleEatPotion = TRUE;
						mDuelProcessState = 0;
						mTRANSFER.B_AVATAR_CHANGE_INFO_1(mIndex, mUniqueNumber, 7, mDATA.aDuelState[0],
						                                 mDATA.aDuelState[1], mDATA.aDuelState[2]);
						mUTIL.Broadcast(TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false);
						break;
					}
					if (mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aLifeValue < 1)
					{
						mTRANSFER.B_DUEL_END_RECV(1);
						mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
						mTRANSFER.B_DUEL_END_RECV(1);
						mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
						mDATA.aDuelState[0] = 0;
						mDATA.aDuelState[1] = 0;
						mDATA.aDuelState[2] = 0;
						mCheckPossibleEatPotion = TRUE;
						mDuelProcessState = 0;
						mTRANSFER.B_AVATAR_CHANGE_INFO_1(mIndex, mUniqueNumber, 7, mDATA.aDuelState[0],
						                                 mDATA.aDuelState[1], mDATA.aDuelState[2]);
						mUTIL.Broadcast(TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false);
						mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[0] = 0;
						mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[1] = 0;
						mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[2] = 0;
						mAVATAR_OBJECT[tOtherAvatarIndex].mCheckPossibleEatPotion = TRUE;
						mAVATAR_OBJECT[tOtherAvatarIndex].mDuelProcessState = 0;
						mTRANSFER.B_AVATAR_CHANGE_INFO_1(tOtherAvatarIndex,
						                                 mAVATAR_OBJECT[tOtherAvatarIndex].mUniqueNumber, 7,
						                                 mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[0],
						                                 mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[1],
						                                 mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[2]);
						mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aAction.aLocation[0],
						                UNIT_SCALE_RADIUS1, false);
						break;
					}
					if (mDATA.aLifeValue < 1)
					{
						mTRANSFER.B_DUEL_END_RECV(2);
						mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
						mTRANSFER.B_DUEL_END_RECV(2);
						mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
						mDATA.aDuelState[0] = 0;
						mDATA.aDuelState[1] = 0;
						mDATA.aDuelState[2] = 0;
						mCheckPossibleEatPotion = TRUE;
						mDuelProcessState = 0;
						mTRANSFER.B_AVATAR_CHANGE_INFO_1(mIndex, mUniqueNumber, 7, mDATA.aDuelState[0],
						                                 mDATA.aDuelState[1], mDATA.aDuelState[2]);
						mUTIL.Broadcast(TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false);
						mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[0] = 0;
						mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[1] = 0;
						mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[2] = 0;
						mAVATAR_OBJECT[tOtherAvatarIndex].mCheckPossibleEatPotion = TRUE;
						mAVATAR_OBJECT[tOtherAvatarIndex].mDuelProcessState = 0;
						mTRANSFER.B_AVATAR_CHANGE_INFO_1(tOtherAvatarIndex,
						                                 mAVATAR_OBJECT[tOtherAvatarIndex].mUniqueNumber, 7,
						                                 mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[0],
						                                 mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[1],
						                                 mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[2]);
						mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aAction.aLocation[0],
						                UNIT_SCALE_RADIUS1, false);
						break;
					}
					if (mDuelProcessRemainTime == 1)
					{
						mTRANSFER.B_DUEL_END_RECV(0);
						mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
						mTRANSFER.B_DUEL_END_RECV(0);
						mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
						mDATA.aDuelState[0] = 0;
						mDATA.aDuelState[1] = 0;
						mDATA.aDuelState[2] = 0;
						mCheckPossibleEatPotion = TRUE;
						mDuelProcessState = 0;
						mTRANSFER.B_AVATAR_CHANGE_INFO_1(mIndex, mUniqueNumber, 7, mDATA.aDuelState[0],
						                                 mDATA.aDuelState[1], mDATA.aDuelState[2]);
						mUTIL.Broadcast(TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false);
						mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[0] = 0;
						mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[1] = 0;
						mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[2] = 0;
						mAVATAR_OBJECT[tOtherAvatarIndex].mCheckPossibleEatPotion = TRUE;
						mAVATAR_OBJECT[tOtherAvatarIndex].mDuelProcessState = 0;
						mTRANSFER.B_AVATAR_CHANGE_INFO_1(tOtherAvatarIndex,
						                                 mAVATAR_OBJECT[tOtherAvatarIndex].mUniqueNumber, 7,
						                                 mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[0],
						                                 mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[1],
						                                 mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[2]);
						mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aAction.aLocation[0],
						                UNIT_SCALE_RADIUS1, false);
						break;
					}
					mDuelProcessRemainTime--;
					mTRANSFER.B_DUEL_TIME_INFO(mDuelProcessRemainTime);
					mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
					mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
					break;
				case 2:
					tOtherAvatarIndex = mUTIL.SearchAvatar(mDuelProcessAvatarName, mIndex);
					if (tOtherAvatarIndex == -1)
					{
						mTRANSFER.B_DUEL_END_RECV(3);
						mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
						mDATA.aDuelState[0] = 0;
						mDATA.aDuelState[1] = 0;
						mDATA.aDuelState[2] = 0;
						mCheckPossibleEatPotion = TRUE;
						mDuelProcessState = 0;
						mTRANSFER.B_AVATAR_CHANGE_INFO_1(mIndex, mUniqueNumber, 7, mDATA.aDuelState[0],
						                                 mDATA.aDuelState[1], mDATA.aDuelState[2]);
						mUTIL.Broadcast(TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false);
					}
					break;
				}
			}
		}
		else
		{
			if (mDATA.aLifeValue < 1)
			{
				if (1 == mDATA.aDuelState[0])
				{
					mGAME.mDuel_124_Pvp = TRUE;
					if (mDATA.aDuelState[2] == 1)
					{
						mGAME.mDuel_124_AvatarNum[0]--;
						mDATA.aAction.aLocation[0] = -157.0f;
						mDATA.aAction.aLocation[1] = 5.0f;
						mDATA.aAction.aLocation[2] = 1.0f;
#if 0
						mDATA.aAction.aLocation[0] = -232.0f;
						mDATA.aAction.aLocation[1] = 36.0f;
						mDATA.aAction.aLocation[2] = 2.0f;
#endif
					}
					else if (mDATA.aDuelState[2] == 2)
					{
						mGAME.mDuel_124_AvatarNum[1]--;
						mDATA.aAction.aLocation[0] = 157.0f;
						mDATA.aAction.aLocation[1] = 5.0f;
						mDATA.aAction.aLocation[2] = 1.0f;
#if 0
						mDATA.aAction.aLocation[0] = 232.0f;
						mDATA.aAction.aLocation[1] = 36.0f;
						mDATA.aAction.aLocation[2] = 2.0f;
#endif
					}

					mDATA.aDuelState[0] = 0;
					mDATA.aDuelState[1] = 0;
					mDATA.aDuelState[2] = 0;
					mCheckPossibleEatPotion = TRUE;
					mDuelProcessState = 0;
					mTRANSFER.B_AVATAR_CHANGE_INFO_1(mIndex, mUniqueNumber, 7, mDATA.aDuelState[0], mDATA.aDuelState[1],
					                                 mDATA.aDuelState[2]);
					mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				}
			}
		}

		if (mTradeProcessState == 3)
		{
			if (mUTIL.SearchAvatar(mTradeProcessAvatarName, mIndex) == -1)
			{
				mTradeProcessState = 0;
			}
		}
		if (mFriendProcessState == 3)
		{
			if (mUTIL.SearchAvatar(mFriendProcessAvatarName, mIndex) == -1)
			{
				mFriendProcessState = 0;
			}
		}
		if (mTeacherProcessState == 3)
		{
			if (mUTIL.SearchAvatar(mTeacherProcessAvatarName, mIndex) == -1)
			{
				mTeacherProcessState = 0;
			}
		}
		if (mPartyProcessState == 3)
		{
			if (mUTIL.SearchAvatar(mPartyProcessAvatarName, mIndex) == -1)
			{
				mPartyProcessState = 0;
			}
		}
		if (mGuildProcessState == 3)
		{
			if (mUTIL.SearchAvatar(mGuildProcessAvatarName, mIndex) == -1)
			{
				mGuildProcessState = 0;
			}
		}
	}
	if ((mGAME.mTickCount - mUSER[mIndex].mTickCountFor30Second) == 60)
	{
		mUSER[mIndex].mTickCountFor30Second = mGAME.mTickCount;
		if (mUSER[mIndex].mAvatarInfo.aEquip[8][1] > 0)
		{
			mUSER[mIndex].mAvatarInfo.aEquip[8][1]--;
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(12, mUSER[mIndex].mAvatarInfo.aEquip[8][1]);
			mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		}
	}

	/* Tick Count for 1Minute Health */
	if ((mGAME.mTickCount - mUSER[mIndex].mTickCountFor01MinuteForHealth) == 110)
	{
		mUSER[mIndex].mTickCountFor01MinuteForHealth = mGAME.mTickCount;
		if (mUSER[mIndex].mCheckHealthState)
		{
			mUSER[mIndex].mAvatarInfo.aPlayTime2++;
			if (mUSER[mIndex].mAvatarInfo.aPlayTime2 > 300)
			{
				mUSER[mIndex].mAvatarInfo.aPlayTime2 = 300;
			}
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(16, mUSER[mIndex].mAvatarInfo.aPlayTime2);
			mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(24, mUSER[mIndex].uLoginPremium);
			mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			mUSER[mIndex].mHealthState = mUTIL.ReturnHealthState(mUSER[mIndex].mAvatarInfo.aPlayTime2);
		}
	}

	/* TickCount for 1 Minute */
	if ((mGAME.mTickCount - mUSER[mIndex].mTickCountFor01Minute) == 120)
	{
		mUSER[mIndex].mTickCountFor01Minute = mGAME.mTickCount;
		mUSER[mIndex].mAvatarInfo.aPlayTime1++;
		++mUSER[mIndex].mAvatarInfo.aPlayTime2;
		if (mUSER[mIndex].mAvatarInfo.aDailyOnlineTime < 60) { mUSER[mIndex].mAvatarInfo.aDailyOnlineTime++; }
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(32, mUSER[mIndex].mAvatarInfo.aPlayTime2);
		mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);

		// Deduct Boost Scrolls Time
		if (mUSER[mIndex].mAvatarInfo.aDamageBoostScroll > 0)
		{
			mUSER[mIndex].mAvatarInfo.aDamageBoostScroll--;
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(46, mUSER[mIndex].mAvatarInfo.aDamageBoostScroll);
			mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			if (mUSER[mIndex].mAvatarInfo.aDamageBoostScroll == 0)
			{
				SetBasicAbilityFromEquip();
			}
		}

		if (mUSER[mIndex].mAvatarInfo.aHealthBoostScroll > 0)
		{
			mUSER[mIndex].mAvatarInfo.aHealthBoostScroll--;
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(47, mUSER[mIndex].mAvatarInfo.aHealthBoostScroll);
			mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			if (mUSER[mIndex].mAvatarInfo.aHealthBoostScroll == 0)
			{
				SetBasicAbilityFromEquip();
			}
		}

		if (mUSER[mIndex].mAvatarInfo.aCriticalBoostScroll > 0)
		{
			mUSER[mIndex].mAvatarInfo.aCriticalBoostScroll--;
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(48, mUSER[mIndex].mAvatarInfo.aCriticalBoostScroll);
			mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			if (mUSER[mIndex].mAvatarInfo.aCriticalBoostScroll == 0)
			{
				SetBasicAbilityFromEquip();
			}
		}

		if (mUSER[mIndex].mAvatarInfo.aDoubleBuffTime > 0)
		{
			mUSER[mIndex].mAvatarInfo.aDoubleBuffTime--;
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(42, mUSER[mIndex].mAvatarInfo.aDoubleBuffTime);
			mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		}

		if (mUSER[mIndex].mAvatarInfo.aAutoHuntState == 1)
		{
			if (mUSER[mIndex].mAvatarInfo.aAutoHunt5Hour > 0)
			{
				mUSER[mIndex].mAvatarInfo.aAutoHunt5Hour--;
				mTRANSFER.B_AVATAR_CHANGE_INFO_2(62, mUSER[mIndex].mAvatarInfo.aAutoHunt5Hour);
				mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			}
		}

		if ((mSERVER_INFO.mServerNumber != 1) && /*Guanyin Town*/
			(mSERVER_INFO.mServerNumber != 6) && /*Fujin Town*/
			(mSERVER_INFO.mServerNumber != 11) && /*Jinong town*/
			(mSERVER_INFO.mServerNumber != 37) && /*Saigo*/
			(mSERVER_INFO.mServerNumber != 38) && /*Yanggok Valley*/
			(mSERVER_INFO.mServerNumber != 140)) /*Nangin Town*/
		{
			// Deduct Exp Pill Time
			if (mUSER[mIndex].mAvatarInfo.aExpPill > 0)
			{
				mUSER[mIndex].mAvatarInfo.aExpPill--;
				mTRANSFER.B_AVATAR_CHANGE_INFO_2(43, mUSER[mIndex].mAvatarInfo.aExpPill);
				mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			}

			// Deduct Superior Pill Time
			if (mUSER[mIndex].mAvatarInfo.aDoubleExpTime2 > 0)
			{
				mUSER[mIndex].mAvatarInfo.aDoubleExpTime2--;
				mTRANSFER.B_AVATAR_CHANGE_INFO_2(17, mUSER[mIndex].mAvatarInfo.aDoubleExpTime2);
				mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			}

			//Deduct Pet Exp Boost Scroll
			if (mUSER[mIndex].mAvatarInfo.aPat_Exp_x2_Time > 0)
			{
				mUSER[mIndex].mAvatarInfo.aPat_Exp_x2_Time--;
				mTRANSFER.B_AVATAR_CHANGE_INFO_2(44, mUSER[mIndex].mAvatarInfo.aPat_Exp_x2_Time);
				mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			}

			//Deduct Lucky Drop Time
			if (mUSER[mIndex].mAvatarInfo.aDoubleDropTime > 0)
			{
				mUSER[mIndex].mAvatarInfo.aDoubleDropTime--;
				mTRANSFER.B_AVATAR_CHANGE_INFO_2(0, mUSER[mIndex].mAvatarInfo.aDoubleDropTime);
				mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			}
		}
		iTemp_Auto_time = 99;
		if (0 == iTemp_Auto_time)
		{
			mUSER[mIndex].mAvatarInfo.aAutoLifeRatio = 0;
			mUSER[mIndex].mAvatarInfo.aAutoChiRatio = 0;
		}
	}

	if ((mGAME.mTickCount - mUSER[mIndex].mTickCountFor01Minute_2) == 120)
	{
		mUSER[mIndex].mTickCountFor01Minute_2 = mGAME.mTickCount;
		mUSER[mIndex].mAvatarInfo.aPlayTime1++;
		++mUSER[mIndex].mAvatarInfo.aPlayTime2;
		if (mUSER[mIndex].mAvatarInfo.aDailyOnlineTime < 60) { mUSER[mIndex].mAvatarInfo.aDailyOnlineTime++; }
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(32, mUSER[mIndex].mAvatarInfo.aPlayTime2);
		mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);

		/*
		if (mUSER[mIndex].mAuthInfo.AuthType == 0)
		{
		*/
		if (mGAME.mCheckZone101TypeServer)
		{
#ifdef __GOD__
			if (mUSER[mIndex].mAvatarInfo.aLevel2 > 0)
			{
#endif
				if (mUSER[mIndex].mAvatarInfo.aZone101TypeTime > 0)
				{
					mUSER[mIndex].mAvatarInfo.aZone101TypeTime--;
				}
				else if (mUSER[mIndex].mAvatarInfo.aZone101TypeTime < 0)
				{
					mUSER[mIndex].mAvatarInfo.aZone101TypeTime = 0;
					mUSER[mIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
					return;
				}
				mTRANSFER.B_AVATAR_CHANGE_INFO_2(18, mUSER[mIndex].mAvatarInfo.aZone101TypeTime);
				mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
#ifdef __GOD__
			}
#endif
		}

		if (mGAME.mCheckZone125Server)
		{
			if (mUSER[mIndex].mAvatarInfo.aZone125TypeTime > 0)
			{
				mUSER[mIndex].mAvatarInfo.aZone125TypeTime--;
			}
			else if (mUSER[mIndex].mAvatarInfo.aZone125TypeTime < 0)
			{
				mUSER[mIndex].mAvatarInfo.aZone125TypeTime = 0;
				mUSER[mIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(21, mUSER[mIndex].mAvatarInfo.aZone125TypeTime);
			mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		}

		if (mGAME.mCheckZone126TypeServer)
		{
			if (mUSER[mIndex].uLoginPremiumPCRoom != 2)
			{
				if (mUSER[mIndex].mAvatarInfo.aZone126TypeTime > 0)
				{
					mUSER[mIndex].mAvatarInfo.aZone126TypeTime--;
				}
				else if (mUSER[mIndex].mAvatarInfo.aZone126TypeTime < 0)
				{
					mUSER[mIndex].mAvatarInfo.aZone126TypeTime = 0;
					mUSER[mIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
					return;
				}
				mTRANSFER.B_AVATAR_CHANGE_INFO_2(22, mUSER[mIndex].mAvatarInfo.aZone126TypeTime);
				mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			}
		}

		if (mSERVER_INFO.mServerNumber == 234)
		{
			if (mUSER[mIndex].mAvatarInfo.aIslandOfBloodTime > 0)
			{
				mUSER[mIndex].mAvatarInfo.aIslandOfBloodTime--;
				mTRANSFER.B_AVATAR_CHANGE_INFO_2(33, mUSER[mIndex].mAvatarInfo.aIslandOfBloodTime);
				mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			}
			else if (mUSER[mIndex].mAvatarInfo.aIslandOfBloodTime < 0)
			{
				mUSER[mIndex].mAvatarInfo.aIslandOfBloodTime = 0;
				mUSER[mIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			/*
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(33, mUSER[mIndex].mAvatarInfo.aIslandOfBloodTime);
			mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			*/
		}

		if (mSERVER_INFO.mServerNumber == 235)
		{
			if (mUSER[mIndex].mAvatarInfo.aIslandOfSoul1Time > 0)
			{
				mUSER[mIndex].mAvatarInfo.aIslandOfSoul1Time--;
				mTRANSFER.B_AVATAR_CHANGE_INFO_2(34, mUSER[mIndex].mAvatarInfo.aIslandOfSoul1Time);
				mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			}
			else if (mUSER[mIndex].mAvatarInfo.aIslandOfSoul1Time < 0)
			{
				mUSER[mIndex].mAvatarInfo.aIslandOfSoul1Time = 0;
				mUSER[mIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
		}

		if (mSERVER_INFO.mServerNumber == 236)
		{
			if (mUSER[mIndex].mAvatarInfo.aIslandOfSoul2Time > 0)
			{
				mUSER[mIndex].mAvatarInfo.aIslandOfSoul2Time--;
			}
			else if (mUSER[mIndex].mAvatarInfo.aIslandOfSoul2Time < 0)
			{
				mUSER[mIndex].mAvatarInfo.aIslandOfSoul2Time = 0;
				mUSER[mIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(35, mUSER[mIndex].mAvatarInfo.aIslandOfSoul2Time);
			mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		}

		if (mSERVER_INFO.mServerNumber == 237)
		{
			if (mUSER[mIndex].mAvatarInfo.aIslandOfSoul3Time > 0)
			{
				mUSER[mIndex].mAvatarInfo.aIslandOfSoul3Time--;
			}
			else if (mUSER[mIndex].mAvatarInfo.aIslandOfSoul3Time < 0)
			{
				mUSER[mIndex].mAvatarInfo.aIslandOfSoul3Time = 0;
				mUSER[mIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(36, mUSER[mIndex].mAvatarInfo.aIslandOfSoul3Time);
			mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		}

		if (mSERVER_INFO.mServerNumber == 238)
		{
			if (mUSER[mIndex].mAvatarInfo.aIslandOfSoul4Time > 0)
			{
				mUSER[mIndex].mAvatarInfo.aIslandOfSoul4Time--;
			}
			else if (mUSER[mIndex].mAvatarInfo.aIslandOfSoul4Time < 0)
			{
				mUSER[mIndex].mAvatarInfo.aIslandOfSoul4Time = 0;
				mUSER[mIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(37, mUSER[mIndex].mAvatarInfo.aIslandOfSoul4Time);
			mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		}

		if (mSERVER_INFO.mServerNumber == 239)
		{
			if (mUSER[mIndex].mAvatarInfo.aIslandOfSoul5Time > 0)
			{
				mUSER[mIndex].mAvatarInfo.aIslandOfSoul5Time--;
			}
			else if (mUSER[mIndex].mAvatarInfo.aIslandOfSoul5Time < 0)
			{
				mUSER[mIndex].mAvatarInfo.aIslandOfSoul5Time = 0;
				mUSER[mIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(38, mUSER[mIndex].mAvatarInfo.aIslandOfSoul5Time);
			mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		}

		if (mSERVER_INFO.mServerNumber == 240)
		{
			if (mUSER[mIndex].mAvatarInfo.aIslandOfSoul6Time > 0)
			{
				mUSER[mIndex].mAvatarInfo.aIslandOfSoul6Time--;
			}
			else if (mUSER[mIndex].mAvatarInfo.aIslandOfSoul6Time < 0)
			{
				mUSER[mIndex].mAvatarInfo.aIslandOfSoul6Time = 0;
				mUSER[mIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return;
			}
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(39, mUSER[mIndex].mAvatarInfo.aIslandOfSoul6Time);
			mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		}
		/*}*/

#ifdef __ANIMAL__

		if ((mUSER[mIndex].mAvatarInfo.aAnimalIndex >= 0)
			&& (mUSER[mIndex].mAvatarInfo.aAnimalIndex <= (MAX_AVATAR_ANIMAL_NUM + MAX_AVATAR_ANIMAL_NUM - 1))
			&& (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber != 0))
		{
			if (mUSER[mIndex].mAvatarInfo.aAnimalTime > 0)
			{
				mUSER[mIndex].mAvatarInfo.aAnimalTime--;
			}
			else
			{
				mUSER[mIndex].mAvatarInfo.aAnimalTime = 0;
			}
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(26, mUSER[mIndex].mAvatarInfo.aAnimalTime);
			mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		}
#endif
		// Absorb Time Decrease
		if (mUSER[mIndex].mAvatarInfo.aAbsorbState == 1)
		{
			if (mUSER[mIndex].mAvatarInfo.aMountAbsorbTime > 0)
			{
				mUSER[mIndex].mAvatarInfo.aMountAbsorbTime--;
				if (mUSER[mIndex].mAvatarInfo.aMountAbsorbTime == 0)
				{
					mUSER[mIndex].mAvatarInfo.aAbsorbState = 0;
					mAVATAR_OBJECT[mIndex].mDATA.aAnimalOn = 0;
					mTRANSFER.B_AVATAR_CHANGE_INFO_2(79, mUSER[mIndex].mAvatarInfo.aAbsorbState);
					mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
					mTRANSFER.B_AVATAR_CHANGE_INFO_1(mIndex, mAVATAR_OBJECT[mIndex].mUniqueNumber, 26,
					                                 mAVATAR_OBJECT[mIndex].mDATA.aAnimalOn, 0, 0);
					mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[mIndex].mDATA.aAction.aLocation[0], 1);
					SetBasicAbilityFromEquip();
				}
			}
			else
			{
				mUSER[mIndex].mAvatarInfo.aMountAbsorbTime = 0;
			}
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(78, mUSER[mIndex].mAvatarInfo.aMountAbsorbTime);
			mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		}

		// Deco Stats
		if (mUSER[mIndex].mAvatarInfo.iUseOrnament == 1)
		{
			if (IsUsedOrnament() == 1)
			{
				mUSER[mIndex].mAvatarInfo.iGoldOrnamentRemainsTime--;
				mTRANSFER.B_AVATAR_CHANGE_INFO_2(40, mUSER[mIndex].mAvatarInfo.iGoldOrnamentRemainsTime);
			}
			else if (IsUsedOrnament() == 2)
			{
				mUSER[mIndex].mAvatarInfo.iSilverOrnamentRemainsTime--;
				mTRANSFER.B_AVATAR_CHANGE_INFO_2(32, mUSER[mIndex].mAvatarInfo.iSilverOrnamentRemainsTime);
			}

			if (mUSER[mIndex].mAvatarInfo.iSilverOrnamentRemainsTime <= 0 && mUSER[mIndex].mAvatarInfo.
				iGoldOrnamentRemainsTime <= 0)
			{
				mUSER[mIndex].mAvatarInfo.iUseOrnament = 0;
			}
			mUSER[mIndex].Send(TRUE, reinterpret_cast<char*>(mTRANSFER.mOriginal), mTRANSFER.mOriginalSize);
			SetBasicAbilityFromEquip();
		}
	}
}

//PROCESS_FOR_CREATE_EFFECT_VALUE
void AVATAR_OBJECT::ProcessForCreateEffectValue(ACTION_INFO* tAction)
{
	if ((mDATA.aAction.aSkillNumber != tAction->aSkillNumber) || (mDATA.aAction.aSkillGradeNum1 != tAction->
		aSkillGradeNum1) || (mDATA.aAction.aSkillGradeNum2 != tAction->aSkillGradeNum2))
	{
		return;
	}
	if (tAction->aSkillNumber != 0)
	{
		// 동행무공은 검사에서 제외.
		if ((tAction->aSkillNumber != 76) && (tAction->aSkillNumber != 77) && (tAction->aSkillNumber != 78) && (tAction
			->aSkillNumber != 79) && (tAction->aSkillNumber != 80) && (tAction->aSkillNumber != 81))
		{
			if ((tAction->aSkillGradeNum1 > mUSER[mIndex].GetMaxSkillGradeNum(tAction->aSkillNumber)) || (tAction->
				aSkillGradeNum2 > this->GetBonusSkillValue(tAction->aSkillNumber)))
			{
				return;
			}
		}
	}

	int index01;
	int tEffectValueState[MAX_AVATAR_EFFECT_SORT_NUM];
	AVATAR_OBJECT* tAVATAR_OBJECT;
	int tRecoverLifeValue;
	int tRecoverManaValue;
	// Double Buff Time
	if (mUSER[mIndex].mAvatarInfo.aDoubleBuffTime > 0)
	{
		mUSER[mIndex].mSupportSkillTimeUpRatio = 2;
	}
	else
	{
		mUSER[mIndex].mSupportSkillTimeUpRatio = 1;
	}


	for (index01 = 0; index01 < MAX_AVATAR_EFFECT_SORT_NUM; index01++)
	{
		tEffectValueState[index01] = 0;
	}
	switch (mDATA.aAction.aSkillNumber)
	{
	case 6: //[축경]
	case 25:
	case 44:
		mUSER[mIndex].mEffectValue[8][0] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2), 11));
		mUSER[mIndex].mEffectValue[8][1] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2),
			10)) * mUSER[mIndex].mSupportSkillTimeUpRatio;
		mDATA.aEffectValueForView[8] = mUSER[mIndex].mEffectValue[8][0];
		tEffectValueState[8] = 1;
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO(mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0]);
		mUTIL.Broadcast(TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false);
		mDATA.aAction.aSkillNumber = 0;
		return;
	case 7: //[속성공격력],[공격속도]
	case 26:
	case 45:
		mUSER[mIndex].mEffectValue[4][0] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2), 16));
		mUSER[mIndex].mEffectValue[4][1] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2),
			10)) * mUSER[mIndex].mSupportSkillTimeUpRatio;
		mDATA.aEffectValueForView[4] = mUSER[mIndex].mEffectValue[4][0];
		tEffectValueState[4] = 1;
		mUSER[mIndex].mEffectValue[6][0] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2), 18));
		mUSER[mIndex].mEffectValue[6][1] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2),
			10)) * mUSER[mIndex].mSupportSkillTimeUpRatio;
		mDATA.aEffectValueForView[6] = mUSER[mIndex].mEffectValue[6][0];
		tEffectValueState[6] = 1;
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO(mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0]);
		mUTIL.Broadcast(TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false);
		mDATA.aAction.aSkillNumber = 0;
		return;
	case 11: //[방어력]
	case 34:
	case 49:
		if ((mEquipItem[7] == nullptr) || ((mEquipItem[7]->iSort != 13) && (mEquipItem[7]->iSort != 17) && (mEquipItem[
			7]->iSort != 19)))
		{
			return;
		}
		mUSER[mIndex].mEffectValue[1][0] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2), 13));
		mUSER[mIndex].mEffectValue[1][1] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2),
			10)) * mUSER[mIndex].mSupportSkillTimeUpRatio;
		mDATA.aEffectValueForView[1] = mUSER[mIndex].mEffectValue[1][0];
		tEffectValueState[1] = 1;
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO(mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0]);
		mUTIL.Broadcast(TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false);
		mDATA.aAction.aSkillNumber = 0;
		return;
	case 15: //[공격력]
	case 30:
	case 53:
		if ((mEquipItem[7] == nullptr) || ((mEquipItem[7]->iSort != 14) && (mEquipItem[7]->iSort != 16) && (mEquipItem[
			7]->iSort != 20)))
		{
			return;
		}
		mUSER[mIndex].mEffectValue[0][0] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2), 12));
		mUSER[mIndex].mEffectValue[0][1] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2),
			10)) * mUSER[mIndex].mSupportSkillTimeUpRatio;
		mDATA.aEffectValueForView[0] = mUSER[mIndex].mEffectValue[0][0];
		tEffectValueState[0] = 1;
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO(mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0]);
		mUTIL.Broadcast(TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false);
		mDATA.aAction.aSkillNumber = 0;
		return;
	case 19: //[이동속도],[공격회피]
	case 38:
	case 57:
		if ((mEquipItem[7] == nullptr) || ((mEquipItem[7]->iSort != 15) && (mEquipItem[7]->iSort != 18) && (mEquipItem[
			7]->iSort != 21)))
		{
			return;
		}
		mUSER[mIndex].mEffectValue[3][0] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2), 15));
		mUSER[mIndex].mEffectValue[3][1] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2),
			10)) * mUSER[mIndex].mSupportSkillTimeUpRatio;
		mDATA.aEffectValueForView[3] = mUSER[mIndex].mEffectValue[3][0];
		tEffectValueState[3] = 1;
		mUSER[mIndex].mEffectValue[7][0] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2), 19));
		mUSER[mIndex].mEffectValue[7][1] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2),
			10)) * mUSER[mIndex].mSupportSkillTimeUpRatio;
		mDATA.aEffectValueForView[7] = mUSER[mIndex].mEffectValue[7][0];
		tEffectValueState[7] = 1;
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO(mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0]);
		mUTIL.Broadcast(TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false);
		mDATA.aAction.aSkillNumber = 0;
		return;
	case 82: //[호신강기]
		mUSER[mIndex].mEffectValue[9][0] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2),
			20) * (float)GetMaxLife() * 0.01f);
		mUSER[mIndex].mEffectValue[9][1] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2),
			10)) * mUSER[mIndex].mSupportSkillTimeUpRatio;
		mDATA.aEffectValueForView[9] = mUSER[mIndex].mEffectValue[9][0];
		tEffectValueState[9] = 1;
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO(mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0]);
		mUTIL.Broadcast(TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false);
		mDATA.aAction.aSkillNumber = 0;
		return;
	case 83: //[치명]
		mUSER[mIndex].mEffectValue[10][0] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2), 22));
		mUSER[mIndex].mEffectValue[10][1] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2),
			10)) * mUSER[mIndex].mSupportSkillTimeUpRatio;
		mDATA.aEffectValueForView[10] = mUSER[mIndex].mEffectValue[10][0];
		tEffectValueState[10] = 1;
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO(mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0]);
		mUTIL.Broadcast(TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false);
		mDATA.aAction.aSkillNumber = 0;
		return;
	case 84: //[운]
		mUSER[mIndex].mEffectValue[11][0] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2), 21));
		mUSER[mIndex].mEffectValue[11][1] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2),
			10)) * mUSER[mIndex].mSupportSkillTimeUpRatio;
		mDATA.aEffectValueForView[11] = mUSER[mIndex].mEffectValue[11][0];
		tEffectValueState[11] = 1;
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO(mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0]);
		mUTIL.Broadcast(TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false);
		mDATA.aAction.aSkillNumber = 0;
		return;
	case 106: //[체력채우기]
	case 108:
	case 110:
		if ((mIndex == mDATA.aAction.aTargetObjectIndex) && (mUniqueNumber == mDATA.aAction.aTargetObjectUniqueNumber))
		{
			mDATA.aAction.aSkillNumber = 0;
			return;
		}
		if ((mDATA.aAction.aTargetObjectIndex < 0) || (mDATA.aAction.aTargetObjectIndex > (mSERVER.mMAX_USER_NUM - 1)))
		{
			mDATA.aAction.aSkillNumber = 0;
			return;
		}
		tAVATAR_OBJECT = &mAVATAR_OBJECT[mDATA.aAction.aTargetObjectIndex];
		if ((!tAVATAR_OBJECT->mCheckValidState) || (tAVATAR_OBJECT->mUniqueNumber != mDATA.aAction.
			aTargetObjectUniqueNumber))
		{
			mDATA.aAction.aSkillNumber = 0;
			return;
		}
		if (tAVATAR_OBJECT->mDATA.aLifeValue < 1)
		{
			mDATA.aAction.aSkillNumber = 0;
			return;
		}
		if (tAVATAR_OBJECT->mDATA.aPShopState == 1)
		{
			mDATA.aAction.aSkillNumber = 0;
			return;
		}
		if (!mGAME.CheckPossibleAttackTarget(1, tAVATAR_OBJECT->mDATA.aAction.aSort))
		{
			mDATA.aAction.aSkillNumber = 0;
			return;
		}
		if (tAVATAR_OBJECT->mDATA.aLifeValue >= tAVATAR_OBJECT->GetMaxLife())
		{
			mDATA.aAction.aSkillNumber = 0;
			return;
		}
		tRecoverLifeValue = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
		                                                             (mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.
			                                                             aSkillGradeNum2), 2));
		if ((tAVATAR_OBJECT->mDATA.aLifeValue + tRecoverLifeValue) > tAVATAR_OBJECT->GetMaxLife())
		{
			tRecoverLifeValue = tAVATAR_OBJECT->GetMaxLife() - tAVATAR_OBJECT->mDATA.aLifeValue;
		}
		if (tRecoverLifeValue < 1)
		{
			mDATA.aAction.aSkillNumber = 0;
			return;
		}
		tAVATAR_OBJECT->mDATA.aLifeValue += tRecoverLifeValue;
		mTRANSFER.B_AVATAR_CHANGE_INFO_1(mDATA.aAction.aTargetObjectIndex, mDATA.aAction.aTargetObjectUniqueNumber, 8,
		                                 tRecoverLifeValue, 0, 0);
		mUTIL.Broadcast(TRUE, &tAVATAR_OBJECT->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false);
		mDATA.aAction.aSkillNumber = 0;
		return;
	case 107: //[공력채우기]
	case 109:
	case 111:
		if ((mIndex == mDATA.aAction.aTargetObjectIndex) && (mUniqueNumber == mDATA.aAction.aTargetObjectUniqueNumber))
		{
			mDATA.aAction.aSkillNumber = 0;
			return;
		}
		if ((mDATA.aAction.aTargetObjectIndex < 0) || (mDATA.aAction.aTargetObjectIndex > (mSERVER.mMAX_USER_NUM - 1)))
		{
			mDATA.aAction.aSkillNumber = 0;
			return;
		}
		tAVATAR_OBJECT = &mAVATAR_OBJECT[mDATA.aAction.aTargetObjectIndex];
		if ((!tAVATAR_OBJECT->mCheckValidState) || (tAVATAR_OBJECT->mUniqueNumber != mDATA.aAction.
			aTargetObjectUniqueNumber))
		{
			mDATA.aAction.aSkillNumber = 0;
			return;
		}
		if (tAVATAR_OBJECT->mDATA.aLifeValue < 1)
		{
			mDATA.aAction.aSkillNumber = 0;
			return;
		}
		if (tAVATAR_OBJECT->mDATA.aPShopState == 1)
		{
			mDATA.aAction.aSkillNumber = 0;
			return;
		}
		if (!mGAME.CheckPossibleAttackTarget(1, tAVATAR_OBJECT->mDATA.aAction.aSort))
		{
			mDATA.aAction.aSkillNumber = 0;
			return;
		}
		if (tAVATAR_OBJECT->mDATA.aManaValue >= tAVATAR_OBJECT->GetMaxMana())
		{
			mDATA.aAction.aSkillNumber = 0;
			return;
		}
		tRecoverManaValue = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
		                                                             (mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.
			                                                             aSkillGradeNum2), 3));
		if ((tAVATAR_OBJECT->mDATA.aManaValue + tRecoverManaValue) > tAVATAR_OBJECT->GetMaxMana())
		{
			tRecoverManaValue = tAVATAR_OBJECT->GetMaxMana() - tAVATAR_OBJECT->mDATA.aManaValue;
		}
		if (tRecoverManaValue < 1)
		{
			mDATA.aAction.aSkillNumber = 0;
			return;
		}
		tAVATAR_OBJECT->mDATA.aManaValue += tRecoverManaValue;
		mTRANSFER.B_AVATAR_CHANGE_INFO_1(mDATA.aAction.aTargetObjectIndex, mDATA.aAction.aTargetObjectUniqueNumber, 9,
		                                 tRecoverManaValue, 0, 0);
		mUTIL.Broadcast(TRUE, &tAVATAR_OBJECT->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false);
		mDATA.aAction.aSkillNumber = 0;
		return;
	case 103: //[대극방탄세]
		mUSER[mIndex].mEffectValue[12][0] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2), 23));
		mUSER[mIndex].mEffectValue[12][1] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2),
			10)) * mUSER[mIndex].mSupportSkillTimeUpRatio;
		mDATA.aEffectValueForView[12] = mUSER[mIndex].mEffectValue[12][0];
		tEffectValueState[12] = 1;
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO(mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0]);
		mUTIL.Broadcast(TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false);
		mDATA.aAction.aSkillNumber = 0;
		return;
	case 104: //[천문역근공]
		mUSER[mIndex].mEffectValue[13][0] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2), 24));
		mUSER[mIndex].mEffectValue[13][1] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2),
			10)) * mUSER[mIndex].mSupportSkillTimeUpRatio;
		mDATA.aEffectValueForView[13] = mUSER[mIndex].mEffectValue[13][0];
		tEffectValueState[13] = 1;
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO(mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0]);
		mUTIL.Broadcast(TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false);
		mDATA.aAction.aSkillNumber = 0;
		return;
	case 105: //[탈망소혼역]
		mUSER[mIndex].mEffectValue[14][0] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2), 25));
		mUSER[mIndex].mEffectValue[14][1] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2),
			10)) * mUSER[mIndex].mSupportSkillTimeUpRatio;
		mDATA.aEffectValueForView[14] = mUSER[mIndex].mEffectValue[14][0];
		tEffectValueState[14] = 1;
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO(mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0]);
		mUTIL.Broadcast(TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false);
		mDATA.aAction.aSkillNumber = 0;
		return;
	case 76: //[오성연환진-공격성공상승]
		mUSER[mIndex].mEffectValue[2][0] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2), 14));
		mUSER[mIndex].mEffectValue[2][1] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2), 10));
		mDATA.aEffectValueForView[2] = mUSER[mIndex].mEffectValue[2][0];
		tEffectValueState[2] = 1;
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO(mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0]);
		mUTIL.Broadcast(TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false);
		mDATA.aAction.aSkillNumber = 0;
		return;
	case 77: //[오성환원진-공격회피상승]
		mUSER[mIndex].mEffectValue[3][0] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2), 15));
		mUSER[mIndex].mEffectValue[3][1] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2), 10));
		mDATA.aEffectValueForView[3] = mUSER[mIndex].mEffectValue[3][0];
		tEffectValueState[3] = 1;
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO(mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0]);
		mUTIL.Broadcast(TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false);
		mDATA.aAction.aSkillNumber = 0;
		return;
	case 79: //[천왕호각진-호신강기]
		mUSER[mIndex].mEffectValue[9][0] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2),
			20) * (float)GetMaxLife() * 0.01f);
		mUSER[mIndex].mEffectValue[9][1] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2), 10));
		mDATA.aEffectValueForView[9] = mUSER[mIndex].mEffectValue[9][0];
		tEffectValueState[9] = 1;
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO(mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0]);
		mUTIL.Broadcast(TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false);
		mDATA.aAction.aSkillNumber = 0;
		return;
	case 81: //[무극천살진-치명타상승]
		mUSER[mIndex].mEffectValue[10][0] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2), 22));
		mUSER[mIndex].mEffectValue[10][1] = static_cast<int>(mSKILL.ReturnSkillValue(mDATA.aAction.aSkillNumber,
			(mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2), 10));
		mDATA.aEffectValueForView[10] = mUSER[mIndex].mEffectValue[10][0];
		tEffectValueState[10] = 1;
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO(mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0]);
		mUTIL.Broadcast(TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false);
		mDATA.aAction.aSkillNumber = 0;
	}
}

//PROCESS_FOR_DELETE_EFFECT_VALUE
void AVATAR_OBJECT::ProcessForDeleteEffectValue(void)
{
	int index01;
	BOOL tCheckChangeEffectValueState;
	int tEffectValueState[MAX_AVATAR_EFFECT_SORT_NUM];

	tCheckChangeEffectValueState = FALSE;
	for (index01 = 0; index01 < MAX_AVATAR_EFFECT_SORT_NUM; index01++)
	{
		tEffectValueState[index01] = 0;
	}
	for (index01 = 0; index01 < MAX_AVATAR_EFFECT_SORT_NUM; index01++)
	{
		if (mUSER[mIndex].mEffectValue[index01][0] < 1)
		{
			continue;
		}
		tCheckChangeEffectValueState = TRUE;
		mUSER[mIndex].mEffectValue[index01][0] = 0;
		mUSER[mIndex].mEffectValue[index01][1] = 0;
		mDATA.aEffectValueForView[index01] = 0;
		tEffectValueState[index01] = 2;
	}
	if (tCheckChangeEffectValueState)
	{
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO(mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0]);
		mUTIL.Broadcast(TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false);
	}
}

//PROCESS_FOR_DEATH
void AVATAR_OBJECT::ProcessForDeath(void)
{
	int index01;

	mCheckDeath = true;
	mCheckCount = 9;

	BOOL tCheckChangeEffectValueState;
	int tEffectValueState[MAX_AVATAR_EFFECT_SORT_NUM];

	tCheckChangeEffectValueState = FALSE;
	for (index01 = 0; index01 < MAX_AVATAR_EFFECT_SORT_NUM; index01++)
	{
		tEffectValueState[index01] = 0;
	}
	for (index01 = 0; index01 < MAX_AVATAR_EFFECT_SORT_NUM; index01++)
	{
		if (mUSER[mIndex].mEffectValue[index01][0] < 1)
		{
			continue;
		}
		tCheckChangeEffectValueState = TRUE;
		mUSER[mIndex].mEffectValue[index01][0] = 0;
		mUSER[mIndex].mEffectValue[index01][1] = 0;
		mDATA.aEffectValueForView[index01] = 0;
		tEffectValueState[index01] = 2;
	}
	if (tCheckChangeEffectValueState)
	{
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO(mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0]);
		mUTIL.Broadcast(TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false);
	}
}

//PROCESS_FOR_TRADE_INFO
void AVATAR_OBJECT::ProcessForTradeInfo(void)
{
	int tOtherAvatarIndex;

	if (mTradeProcessState != 4)
	{
		return;
	}
	tOtherAvatarIndex = mUTIL.SearchAvatar(mTradeProcessAvatarName, mIndex);
	if (tOtherAvatarIndex == -1)
	{
		return;
	}
	if (mAVATAR_OBJECT[tOtherAvatarIndex].mTradeProcessState != 4)
	{
		return;
	}
	if (strcmp(mAVATAR_OBJECT[tOtherAvatarIndex].mTradeProcessAvatarName, mDATA.aName) != 0)
	{
		return;
	}
	mTRANSFER.B_TRADE_STATE_RECV(mUSER[mIndex].mAvatarInfo.aTradeMoney, mUSER[mIndex].mAvatarInfo.aTrade,
	                             mUSER[mIndex].mAvatarInfo.aTradeMoney2, mUSER[mIndex].mAvatarInfo.aTradeSocket);
	mUSER[tOtherAvatarIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
}

//RETURN_QUEST_PRESENT_STATE
int AVATAR_OBJECT::ReturnQuestPresentState(void)
{
	int index01;
	int index02;
	QUEST_INFO* tQUEST_INFO;

	if ((mUSER[mIndex].mAvatarInfo.aQuestInfo[1] == 0) && (mUSER[mIndex].mAvatarInfo.aQuestInfo[2] == 0) && (mUSER[
		mIndex].mAvatarInfo.aQuestInfo[3] == 0) && (mUSER[mIndex].mAvatarInfo.aQuestInfo[4] == 0))
	{
		tQUEST_INFO = mQUEST.Search(mUSER[mIndex].mAvatarInfo.aTribe, (mUSER[mIndex].mAvatarInfo.aQuestInfo[0] + 1));
		if (tQUEST_INFO == nullptr)
		{
			return 0;
		}
		if (mUSER[mIndex].mAvatarInfo.aLevel1 < tQUEST_INFO->qLevel)
		{
			return 0;
		}
		return 1;
	}
	if (mUSER[mIndex].mAvatarInfo.aQuestInfo[1] == 1)
	{
		tQUEST_INFO = mQUEST.Search(mUSER[mIndex].mAvatarInfo.aTribe, mUSER[mIndex].mAvatarInfo.aQuestInfo[0]);
		if (tQUEST_INFO == nullptr)
		{
			return 0;
		}
		switch (mUSER[mIndex].mAvatarInfo.aQuestInfo[2])
		{
		case 1: //[몬스터 죽이기]
			if (mUSER[mIndex].mAvatarInfo.aQuestInfo[3] != tQUEST_INFO->qSolution[0])
			{
				return 0;
			}
			if (mUSER[mIndex].mAvatarInfo.aQuestInfo[4] < tQUEST_INFO->qSolution[1])
			{
				return 2;
			}
			return 3;
		case 2: //[아이템 입수]
			if (mUSER[mIndex].mAvatarInfo.aQuestInfo[3] != tQUEST_INFO->qSolution[0])
			{
				return 0;
			}
			for (index01 = 0; index01 < 2; index01++)
			{
				for (index02 = 0; index02 < MAX_INVENTORY_SLOT_NUM; index02++)
				{
					if (mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][0] == tQUEST_INFO->qSolution[0])
					{
						break;
					}
				}
				if (index02 < MAX_INVENTORY_SLOT_NUM)
				{
					break;
				}
			}
			if (index01 == 2)
			{
				return 2;
			}
			return 3;
		case 3: //[아이템 전달]
			if (mUSER[mIndex].mAvatarInfo.aQuestInfo[3] != tQUEST_INFO->qSolution[0])
			{
				return 0;
			}
			for (index01 = 0; index01 < 2; index01++)
			{
				for (index02 = 0; index02 < MAX_INVENTORY_SLOT_NUM; index02++)
				{
					if (mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][0] == tQUEST_INFO->qSolution[0])
					{
						break;
					}
				}
				if (index02 < MAX_INVENTORY_SLOT_NUM)
				{
					break;
				}
			}
			if (index01 == 2)
			{
				return 2;
			}
			return 3;
		case 4: //[아이템 받아오기]
			if (mUSER[mIndex].mAvatarInfo.aQuestInfo[3] != tQUEST_INFO->qSolution[0])
			{
				return 0;
			}
			for (index01 = 0; index01 < 2; index01++)
			{
				for (index02 = 0; index02 < MAX_INVENTORY_SLOT_NUM; index02++)
				{
					if (mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][0] == tQUEST_INFO->qSolution[0])
					{
						break;
					}
				}
				if (index02 < MAX_INVENTORY_SLOT_NUM)
				{
					break;
				}
			}
			if (index01 == 2)
			{
				return 2;
			}
			return 3;
		case 5: //[대장 죽이기]
			if (mUSER[mIndex].mAvatarInfo.aQuestInfo[3] != tQUEST_INFO->qSolution[0])
			{
				return 0;
			}
			if (mUSER[mIndex].mAvatarInfo.aQuestInfo[4] < 1)
			{
				return 2;
			}
			return 3;
		case 6: //[아이템 교환하기]
			switch (mUSER[mIndex].mAvatarInfo.aQuestInfo[3])
			{
			case 1: //[교환 전]
				if (mUSER[mIndex].mAvatarInfo.aQuestInfo[4] != tQUEST_INFO->qSolution[0])
				{
					return 0;
				}
				for (index01 = 0; index01 < 2; index01++)
				{
					for (index02 = 0; index02 < MAX_INVENTORY_SLOT_NUM; index02++)
					{
						if (mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][0] == tQUEST_INFO->qSolution[0])
						{
							break;
						}
					}
					if (index02 < MAX_INVENTORY_SLOT_NUM)
					{
						break;
					}
				}
				if (index01 == 2)
				{
					return 2;
				}
				return 3;
			case 2: //[교환 후]
				if (mUSER[mIndex].mAvatarInfo.aQuestInfo[4] != tQUEST_INFO->qSolution[1])
				{
					return 0;
				}
				for (index01 = 0; index01 < 2; index01++)
				{
					for (index02 = 0; index02 < MAX_INVENTORY_SLOT_NUM; index02++)
					{
						if (mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][0] == tQUEST_INFO->qSolution[1])
						{
							break;
						}
					}
					if (index02 < MAX_INVENTORY_SLOT_NUM)
					{
						break;
					}
				}
				if (index01 == 2)
				{
					return 4;
				}
				return 5;
			}
			break;
		case 7: //[NPC 만나기]
			if (mUSER[mIndex].mAvatarInfo.aQuestInfo[3] != tQUEST_INFO->qSolution[0])
			{
				return 0;
			}
			return 2;
		case 8: //[진 점령 참여]
			if (mUSER[mIndex].mAvatarInfo.aQuestInfo[3] != tQUEST_INFO->qSolution[0])
			{
				return 0;
			}
			if (mUSER[mIndex].mAvatarInfo.aQuestInfo[4] < 1)
			{
				return 2;
			}
			return 3;
		}
	}
	return 0;
}

//RETURN_QUEST_END_CONDITION_STATE
int AVATAR_OBJECT::ReturnQuestEndConditionState(void)
{
	switch (mUSER[mIndex].mAvatarInfo.aQuestInfo[2])
	{
	case 1:
		if (ReturnQuestPresentState() == 3)
		{
			return 1;
		}
		return 0;
	case 2:
		if (ReturnQuestPresentState() == 3)
		{
			return 1;
		}
		return 0;
	case 3:
		if (ReturnQuestPresentState() == 3)
		{
			return 1;
		}
		return 0;
	case 4:
		if (ReturnQuestPresentState() == 3)
		{
			return 1;
		}
		return 0;
	case 5:
		if (ReturnQuestPresentState() == 3)
		{
			return 1;
		}
		return 0;
	case 6:
		if (ReturnQuestPresentState() == 5)
		{
			return 1;
		}
		return 0;
	case 7:
		if (ReturnQuestPresentState() == 2)
		{
			return 1;
		}
		return 0;
	case 8:
		if (ReturnQuestPresentState() == 3)
		{
			return 1;
		}
		return 0;
	}
	return 0;
}

//RETURN_QUEST_NEXT_NPC_NUMBER
int AVATAR_OBJECT::ReturnQuestNextNPCNumber(void)
{
	int index01;
	int index02;
	QUEST_INFO* tQUEST_INFO;

	if ((mUSER[mIndex].mAvatarInfo.aQuestInfo[1] == 0) && (mUSER[mIndex].mAvatarInfo.aQuestInfo[2] == 0) && (mUSER[
		mIndex].mAvatarInfo.aQuestInfo[3] == 0) && (mUSER[mIndex].mAvatarInfo.aQuestInfo[4] == 0))
	{
		tQUEST_INFO = mQUEST.Search(mUSER[mIndex].mAvatarInfo.aTribe, (mUSER[mIndex].mAvatarInfo.aQuestInfo[0] + 1));
		if (tQUEST_INFO == nullptr)
		{
			return 0;
		}
		if (mUSER[mIndex].mAvatarInfo.aLevel1 < tQUEST_INFO->qLevel)
		{
			return 0;
		}
		return tQUEST_INFO->qStartNPCNumber;
	}
	if (mUSER[mIndex].mAvatarInfo.aQuestInfo[1] == 1)
	{
		tQUEST_INFO = mQUEST.Search(mUSER[mIndex].mAvatarInfo.aTribe, mUSER[mIndex].mAvatarInfo.aQuestInfo[0]);
		if (tQUEST_INFO == nullptr)
		{
			return 0;
		}
		switch (mUSER[mIndex].mAvatarInfo.aQuestInfo[2])
		{
		case 1: //[몬스터 죽이기]
			if (mUSER[mIndex].mAvatarInfo.aQuestInfo[3] != tQUEST_INFO->qSolution[0])
			{
				return 0;
			}
			if (mUSER[mIndex].mAvatarInfo.aQuestInfo[4] < tQUEST_INFO->qSolution[1])
			{
				return tQUEST_INFO->qStartNPCNumber;
			}
			return tQUEST_INFO->qEndNPCNumber;
		case 2: //[아이템 입수]
			if (mUSER[mIndex].mAvatarInfo.aQuestInfo[3] != tQUEST_INFO->qSolution[0])
			{
				return 0;
			}
			for (index01 = 0; index01 < 2; index01++)
			{
				for (index02 = 0; index02 < MAX_INVENTORY_SLOT_NUM; index02++)
				{
					if (mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][0] == tQUEST_INFO->qSolution[0])
					{
						break;
					}
				}
				if (index02 < MAX_INVENTORY_SLOT_NUM)
				{
					break;
				}
			}
			if (index01 == 2)
			{
				return tQUEST_INFO->qStartNPCNumber;
			}
			return tQUEST_INFO->qEndNPCNumber;
		case 3: //[아이템 전달]
			if (mUSER[mIndex].mAvatarInfo.aQuestInfo[3] != tQUEST_INFO->qSolution[0])
			{
				return 0;
			}
			for (index01 = 0; index01 < 2; index01++)
			{
				for (index02 = 0; index02 < MAX_INVENTORY_SLOT_NUM; index02++)
				{
					if (mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][0] == tQUEST_INFO->qSolution[0])
					{
						break;
					}
				}
				if (index02 < MAX_INVENTORY_SLOT_NUM)
				{
					break;
				}
			}
			if (index01 == 2)
			{
				return tQUEST_INFO->qStartNPCNumber;
			}
			return tQUEST_INFO->qEndNPCNumber;
		case 4: //[아이템 받아오기]
			if (mUSER[mIndex].mAvatarInfo.aQuestInfo[3] != tQUEST_INFO->qSolution[0])
			{
				return 0;
			}
			for (index01 = 0; index01 < 2; index01++)
			{
				for (index02 = 0; index02 < MAX_INVENTORY_SLOT_NUM; index02++)
				{
					if (mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][0] == tQUEST_INFO->qSolution[0])
					{
						break;
					}
				}
				if (index02 < MAX_INVENTORY_SLOT_NUM)
				{
					break;
				}
			}
			if (index01 == 2)
			{
				return tQUEST_INFO->qKeyNPCNumber[0];
			}
			return tQUEST_INFO->qEndNPCNumber;
		case 5: //[대장 죽이기]
			if (mUSER[mIndex].mAvatarInfo.aQuestInfo[3] != tQUEST_INFO->qSolution[0])
			{
				return 0;
			}
			if (mUSER[mIndex].mAvatarInfo.aQuestInfo[4] < 1)
			{
				return tQUEST_INFO->qStartNPCNumber;
			}
			return tQUEST_INFO->qEndNPCNumber;
		case 6: //[아이템 교환하기]
			switch (mUSER[mIndex].mAvatarInfo.aQuestInfo[3])
			{
			case 1: //[교환 전]
				if (mUSER[mIndex].mAvatarInfo.aQuestInfo[4] != tQUEST_INFO->qSolution[0])
				{
					return 0;
				}
				for (index01 = 0; index01 < 2; index01++)
				{
					for (index02 = 0; index02 < MAX_INVENTORY_SLOT_NUM; index02++)
					{
						if (mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][0] == tQUEST_INFO->qSolution[0])
						{
							break;
						}
					}
					if (index02 < MAX_INVENTORY_SLOT_NUM)
					{
						break;
					}
				}
				if (index01 == 2)
				{
					return tQUEST_INFO->qStartNPCNumber;
				}
				return tQUEST_INFO->qKeyNPCNumber[0];
			case 2: //[교환 후]
				if (mUSER[mIndex].mAvatarInfo.aQuestInfo[4] != tQUEST_INFO->qSolution[1])
				{
					return 0;
				}
				for (index01 = 0; index01 < 2; index01++)
				{
					for (index02 = 0; index02 < MAX_INVENTORY_SLOT_NUM; index02++)
					{
						if (mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][0] == tQUEST_INFO->qSolution[1])
						{
							break;
						}
					}
					if (index02 < MAX_INVENTORY_SLOT_NUM)
					{
						break;
					}
				}
				if (index01 == 2)
				{
					return tQUEST_INFO->qStartNPCNumber;
				}
				return tQUEST_INFO->qEndNPCNumber;
			}
			break;
		case 7: //[NPC 만나기]
			if (mUSER[mIndex].mAvatarInfo.aQuestInfo[3] != tQUEST_INFO->qSolution[0])
			{
				return 0;
			}
			return tQUEST_INFO->qEndNPCNumber;
		case 8: //[진 점령 참여]
			if (mUSER[mIndex].mAvatarInfo.aQuestInfo[3] != tQUEST_INFO->qSolution[0])
			{
				return 0;
			}
			if (mUSER[mIndex].mAvatarInfo.aQuestInfo[4] < 1)
			{
				return tQUEST_INFO->qStartNPCNumber;
			}
			return tQUEST_INFO->qEndNPCNumber;
		}
	}
	return 0;
}

//RETURN_ITEM_NUMBER_FOR_QUEST_REWARD
int AVATAR_OBJECT::ReturnItemNumberForQuestReward(void)
{
	int index01;
	QUEST_INFO* tQUEST_INFO;

	tQUEST_INFO = mQUEST.Search(mUSER[mIndex].mAvatarInfo.aTribe, mUSER[mIndex].mAvatarInfo.aQuestInfo[0]);
	if (tQUEST_INFO == nullptr)
	{
		return 0;
	}
	for (index01 = 0; index01 < 3; index01++)
	{
		if (tQUEST_INFO->qReward[index01][0] == 6)
		{
			return tQUEST_INFO->qReward[index01][1];
		}
	}
	return 0;
}

//RETURN_ITEM_QUANTITY_FOR_QUEST_REWARD
int AVATAR_OBJECT::ReturnItemQuantityForQuestReward(void)
{
	int index01;
	QUEST_INFO* tQUEST_INFO;
	ITEM_INFO* tITEM_INFO;

	tQUEST_INFO = mQUEST.Search(mUSER[mIndex].mAvatarInfo.aTribe, mUSER[mIndex].mAvatarInfo.aQuestInfo[0]);
	if (tQUEST_INFO == nullptr)
	{
		return 0;
	}
	for (index01 = 0; index01 < 3; index01++)
	{
		if (tQUEST_INFO->qReward[index01][0] != 6)
		{
			continue;
		}
		tITEM_INFO = mITEM.Search(tQUEST_INFO->qReward[index01][1]);
		if (tITEM_INFO == nullptr)
		{
			return 0;
		}
		if (tITEM_INFO->iSort == 2)
		{
			return 1;
		}
		return 0;
	}
	return 0;
}

//SUMMON_QUEST_BOSS
int AVATAR_OBJECT::SummonQuestBoss(void)
{
	QUEST_INFO* tQUEST_INFO;
	float tCoord[3];

	if ((mUSER[mIndex].mAvatarInfo.aQuestInfo[2] != 5) || (ReturnQuestPresentState() != 2))
	{
		return 0;
	}
	tQUEST_INFO = mQUEST.Search(mUSER[mIndex].mAvatarInfo.aTribe, mUSER[mIndex].mAvatarInfo.aQuestInfo[0]);
	if (tQUEST_INFO == nullptr)
	{
		return 0;
	}
	if (mSERVER_INFO.mServerNumber != tQUEST_INFO->qSummonInfo[0])
	{
		return 0;
	}
	tCoord[0] = static_cast<float>(tQUEST_INFO->qSummonInfo[1]);
	tCoord[1] = static_cast<float>(tQUEST_INFO->qSummonInfo[2]);
	tCoord[2] = static_cast<float>(tQUEST_INFO->qSummonInfo[3]);
	if (mUTIL.GetLengthXYZ(&mDATA.aAction.aLocation[0], &tCoord[0]) > 300.0f)
	{
		return 0;
	}
	mSUMMON.SummonMonsterForSpecial(tQUEST_INFO->qSolution[0], &tCoord[0], TRUE);
	return 0;
}

//CHANGE_QUEST_ITEM
int AVATAR_OBJECT::ChangeQuestItem(int tItemNumber1, int tItemNumber2)
{
	int index01;
	int index02;

	for (index01 = 0; index01 < 2; index01++)
	{
		for (index02 = 0; index02 < MAX_INVENTORY_SLOT_NUM; index02++)
		{
			if (mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][0] == tItemNumber1)
			{
				mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][0] = tItemNumber2;
				mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][3] = 0;
				mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][4] = 0;
				mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][5] = 0;
				return index01;
			}
		}
	}
	return -1;
}

//DELETE_QUEST_ITEM
int AVATAR_OBJECT::DeleteQuestItem(int tItemNumber)
{
	int index01;
	int index02;

	for (index01 = 0; index01 < 2; index01++)
	{
		for (index02 = 0; index02 < MAX_INVENTORY_SLOT_NUM; index02++)
		{
			if (mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][0] == tItemNumber)
			{
				mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][0] = 0;
				mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][1] = 0;
				mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][2] = 0;
				mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][3] = 0;
				mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][4] = 0;
				mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][5] = 0;
				return index01;
			}
		}
	}
	return -1;
}

/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */
int AVATAR_OBJECT::IsUsedOrnament(void)
{
	if (mUSER[mIndex].mAvatarInfo.iUseOrnament == true && mUSER[mIndex].mAvatarInfo.aLevel1 > 112)
	{
		if (mUSER[mIndex].mAvatarInfo.aEquip[9][0] > 0 && // 안면?
			mUSER[mIndex].mAvatarInfo.aEquip[10][0] > 0 && // 가슴?
			mUSER[mIndex].mAvatarInfo.aEquip[11][0] > 0 && // 복부?
			mUSER[mIndex].mAvatarInfo.aEquip[12][0] > 0)
		{
			// 팔, 다리, 어깨?
			// 장식 해방 시스템 옵션이 켜져 있고 모든 장식 아이템을
			// 착용 중이면서 갑 레벨 이상인 경우에 활성화 된다.
			if (mUSER[mIndex].mAvatarInfo.iGoldOrnamentRemainsTime > 0)
			{
				return 1;
			}
			if (mUSER[mIndex].mAvatarInfo.iSilverOrnamentRemainsTime > 0)
			{
				return 2;
			}
			return 0;
		}
	}

	return 0;
}

/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */

void AVATAR_OBJECT::AutoBuff(int tCheckMotion)
{
	int tCount = 0;
	ACTION_INFO tAction;

	if (mUSER[mIndex].mAvatarInfo.aAutoBuffScroll < mUTIL.ReturnNowDate())
	{
		mUSER[mIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}

	if (mDATA.aManaValue < (GetMaxMana() * 0.9f))
	{
		return;
	}
	mDATA.aManaValue -= (GetMaxMana() * 0.9f);

	for (tCount = 0; tCount < 8; tCount++)
	{
		if ((mUSER[mIndex].mAvatarInfo.aAutoBuffScrollSkillSlots[tCount][0] == 0))
		{
			continue;
		}
		memset(&tAction, 0, sizeof(ACTION_INFO));
		tAction.aSkillNumber = mUSER[mIndex].mAvatarInfo.aAutoBuffScrollSkillSlots[tCount][0];

		tAction.aSkillGradeNum1 = mUSER[mIndex].mAvatarInfo.aAutoBuffScrollSkillSlots[tCount][1];
		tAction.aSkillGradeNum2 = mAVATAR_OBJECT[mIndex].GetBonusSkillValue(tAction.aSkillNumber);
		tAction.aType = 0;
		tAction.aSort = 41;
		tAction.aFrame = 0.0f;
		tAction.aTargetLocation[0] = 0.0f;
		tAction.aTargetLocation[1] = 0.0f;
		tAction.aTargetLocation[2] = 0.0f;
		tAction.aTargetObjectSort = 0;
		tAction.aTargetObjectIndex = -1;
		tAction.aTargetObjectUniqueNumber = 0;
		tAction.aSkillValue = 0;
		AutoBuffStart(&tAction);
	}
}

void AVATAR_OBJECT::AutoHuntBuff()
{
	int tCount = 0;
	ACTION_INFO tAction;

	if ((mUSER[mIndex].mAvatarInfo.aAutoHunt7Day < mUTIL.ReturnNowDate()) || (mUSER[mIndex].mAvatarInfo.aAutoHunt5Hour <
		0))
	{
		mUSER[mIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if (mUSER[mIndex].mAvatarInfo.aAutoBuffScroll > mUTIL.ReturnNowDate())
	{
		for (tCount = 0; tCount < 8; tCount++)
		{
			if ((mUSER[mIndex].mAvatarInfo.aAutoHuntData[tCount][0] == 0))
			{
				continue;
			}
			if (!mWORK.ReturnEffectState(mIndex, mUSER[mIndex].mAvatarInfo.aAutoHuntData[tCount][0]))
			{
				continue;
			}
			memset(&tAction, 0, sizeof(ACTION_INFO));
			tAction.aSkillNumber = mUSER[mIndex].mAvatarInfo.aAutoHuntData[tCount][0];
			tAction.aSkillGradeNum1 = mUSER[mIndex].mAvatarInfo.aAutoHuntData[tCount][1];
			tAction.aSkillGradeNum2 = mAVATAR_OBJECT[mIndex].GetBonusSkillValue(tAction.aSkillNumber);
			mWORK.SkillForMotion(mIndex, &tAction, 0);
			AutoBuffStart(&tAction);
		}
	}
	else
	{
		for (tCount = 0; tCount < 2; tCount++)
		{
			if ((mUSER[mIndex].mAvatarInfo.aAutoHuntData[tCount][0] == 0))
			{
				continue;
			}
			if (!mWORK.ReturnEffectState(mIndex, mUSER[mIndex].mAvatarInfo.aAutoHuntData[tCount][0]))
			{
				continue;
			}
			memset(&tAction, 0, sizeof(ACTION_INFO));
			tAction.aSkillNumber = mUSER[mIndex].mAvatarInfo.aAutoHuntData[tCount][0];
			tAction.aSkillGradeNum1 = mUSER[mIndex].mAvatarInfo.aAutoHuntData[tCount][1];
			tAction.aSkillGradeNum2 = mAVATAR_OBJECT[mIndex].GetBonusSkillValue(tAction.aSkillNumber);
			mWORK.SkillForMotion(mIndex, &tAction, 0);
			AutoBuffStart(&tAction);
		}
	}
}

void AVATAR_OBJECT::AutoBuffStart(ACTION_INFO* pAction)
{
	int tUseManaValue = 0;
	int tReduceManaRatio = 0;

	if (mUSER[mIndex].mAvatarInfo.aAutoHuntState == 0)
	{
		goto NONAUTOHUNT;
	}

	tUseManaValue = static_cast<int>(mSKILL.ReturnSkillValue(pAction->aSkillNumber, pAction->aSkillGradeNum1, 1));
	tReduceManaRatio = GetReduceManaRatio();

	if (tReduceManaRatio > 0)
	{
		tUseManaValue -= ((tUseManaValue * tReduceManaRatio) / 100);
	}
	if (mDATA.aManaValue < tUseManaValue)
	{
		return;
	}
	if (tUseManaValue > 0)
	{
		mDATA.aManaValue -= tUseManaValue;
	}

NONAUTOHUNT:
	mDATA.aAction.aSkillNumber = pAction->aSkillNumber;
	mDATA.aAction.aSkillGradeNum1 = pAction->aSkillGradeNum1;
	mDATA.aAction.aSkillGradeNum2 = pAction->aSkillGradeNum2;
	mDATA.aAction.aSort = pAction->aSort;
	mDATA.aAction.aType = pAction->aType;
	mDATA.aAction.aFrame = pAction->aFrame;
	mDATA.aAction.aTargetLocation[0] = pAction->aTargetLocation[0];
	mDATA.aAction.aTargetLocation[1] = pAction->aTargetLocation[1];
	mDATA.aAction.aTargetLocation[2] = pAction->aTargetLocation[2];
	mDATA.aAction.aTargetObjectSort = pAction->aTargetObjectSort;
	mDATA.aAction.aTargetObjectIndex = pAction->aTargetObjectIndex;
	mDATA.aAction.aTargetObjectUniqueNumber = pAction->aTargetObjectUniqueNumber;
	mDATA.aAction.aSkillValue = pAction->aSkillValue;

	mTRANSFER.B_AVATAR_ACTION_RECV(mIndex, mAVATAR_OBJECT[mIndex].mUniqueNumber, &mDATA, 1);
	if (mUSER[mIndex].mAvatarInfo.aVisibleState == 0)
	{
		mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	}
	else
	{
		mUSER[mIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[mIndex].mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, mIndex, false);
	}
	ProcessForCreateEffectValue(pAction);
}
