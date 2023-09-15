//-------------------------------------------------------------------------------------------------
//HEADER
//-------------------------------------------------------------------------------------------------
#include "MainApp.h"
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_UTIL
//-------------------------------------------------------------------------------------------------
//INSTANCE
MyUtil mUTIL;
//CREATE
MyUtil::MyUtil(void)
{
	mEquipPart[0] = 2;
	mEquipPart[1] = 3;
	mEquipPart[2] = 4;
	mEquipPart[3] = 5;
	mEquipPart[4] = 6;
	mEquipPart[5] = 7;
	mEquipPart[6] = 0;
	mEquipPart[7] = 9;
	mEquipPart[8] = 10;
	mEquipPart[9] = 11;
	mEquipPart[10] = 12;
	mEquipPart[11] = 13;
	mEquipPart[12] = 14;
}

//CHECK_NAME_STRING
BOOL MyUtil::CheckNameString(char* tString)
{
	int index01;
	WORD tUnicode;

	for (index01 = 0; index01 < static_cast<int>(strlen(tString)); index01++)
	{
		if (static_cast<BYTE>(tString[index01]) < 128)
		{
			if (
				(tString[index01] == '0') ||
				(tString[index01] == '1') ||
				(tString[index01] == '2') ||
				(tString[index01] == '3') ||
				(tString[index01] == '4') ||
				(tString[index01] == '5') ||
				(tString[index01] == '6') ||
				(tString[index01] == '7') ||
				(tString[index01] == '8') ||
				(tString[index01] == '9') ||
				(tString[index01] == 'A') ||
				(tString[index01] == 'B') ||
				(tString[index01] == 'C') ||
				(tString[index01] == 'D') ||
				(tString[index01] == 'E') ||
				(tString[index01] == 'F') ||
				(tString[index01] == 'G') ||
				(tString[index01] == 'H') ||
				(tString[index01] == 'I') ||
				(tString[index01] == 'J') ||
				(tString[index01] == 'K') ||
				(tString[index01] == 'L') ||
				(tString[index01] == 'M') ||
				(tString[index01] == 'N') ||
				(tString[index01] == 'O') ||
				(tString[index01] == 'P') ||
				(tString[index01] == 'Q') ||
				(tString[index01] == 'R') ||
				(tString[index01] == 'S') ||
				(tString[index01] == 'T') ||
				(tString[index01] == 'U') ||
				(tString[index01] == 'V') ||
				(tString[index01] == 'W') ||
				(tString[index01] == 'X') ||
				(tString[index01] == 'Y') ||
				(tString[index01] == 'Z') ||
				(tString[index01] == 'a') ||
				(tString[index01] == 'b') ||
				(tString[index01] == 'c') ||
				(tString[index01] == 'd') ||
				(tString[index01] == 'e') ||
				(tString[index01] == 'f') ||
				(tString[index01] == 'g') ||
				(tString[index01] == 'h') ||
				(tString[index01] == 'i') ||
				(tString[index01] == 'j') ||
				(tString[index01] == 'k') ||
				(tString[index01] == 'l') ||
				(tString[index01] == 'm') ||
				(tString[index01] == 'n') ||
				(tString[index01] == 'o') ||
				(tString[index01] == 'p') ||
				(tString[index01] == 'q') ||
				(tString[index01] == 'r') ||
				(tString[index01] == 's') ||
				(tString[index01] == 't') ||
				(tString[index01] == 'u') ||
				(tString[index01] == 'v') ||
				(tString[index01] == 'w') ||
				(tString[index01] == 'x') ||
				(tString[index01] == 'y') ||
				(tString[index01] == 'z')
			)
			{
				continue;
			}
			return FALSE;
		}
#ifdef __TK__
			index01++;
#else
		return FALSE;
#endif
	}
#if 0
	for (index01 = 0; index01 < (int)strlen(tString); index01++)
	{
		if ((BYTE)tString[index01] < 128)
		{
			continue;
		}
		index01++;
		if (!MultiByteToWideChar(CP_ACP, 0, &tString[(index01 - 1)], 2, &tUnicode, 1))
		{
			return FALSE;
		}
		if ((tUnicode >= 0x4E00) && (tUnicode <= 0x9FBF))
		{
			continue;
		}
		if ((tUnicode >= 0x3400) && (tUnicode <= 0x4DBF))
		{
			continue;
		}
		/*
		if(mbstowcs((wchar_t *)&tUnicode, &tString[(index01 - 1)], 1) < 0 ) {
			return FALSE;
		}

		if( ( tUnicode >= 0x4E00 ) && ( tUnicode <= 0x9FFF ) )
		{
			continue;
		}
		*/
#if 0
		if ((tUnicode >= 0x4E00) && (tUnicode <= 0x9FBF))
		{
			continue;
		}
		if ((tUnicode >= 0x3400) && (tUnicode <= 0x4DBF))
		{
			continue;
		}
#endif
		return FALSE;
	}
#endif
	return TRUE;
}

//CHECK_OVER_MAXIMUM
BOOL MyUtil::CheckOverMaximum(int tValue1, int tValue2)
{
	LONGLONG tCheckValue = static_cast<LONGLONG>(tValue1) + static_cast<LONGLONG>(tValue2);

	if (tCheckValue > MAX_NUMBER_SIZE)
	{
		return TRUE;
	}
	return FALSE;
}

//RETURN_FIXED_LEVEL
int MyUtil::ReturnFixedLevel(int tLevel)
{
	if (tLevel < 100)
	{
		return tLevel;
	}
	switch (tLevel)
	{
	case 100: return 102;
	case 101: return 105;
	case 102: return 108;
	case 103: return 111;
	case 104: return 114;
	case 105: return 117;
	case 106: return 120;
	case 107: return 123;
	case 108: return 126;
	case 109: return 129;
	case 110: return 132;
	case 111: return 135;
	case 112: return 138;
	case 113: return 143;
	case 114: return 149;
	case 115: return 155;
	case 116: return 161;
	case 117: return 167;
	case 118: return 173;
	case 119: return 179;
	case 120: return 185;
	case 121: return 191;
	case 122: return 197;
	case 123: return 203;
	case 124: return 209;
	case 125: return 215;
	case 126: return 221;
	case 127: return 227;
	case 128: return 233;
	case 129: return 239;
	case 130: return 245;
	case 131: return 251;
	case 132: return 257;
	case 133: return 263;
	case 134: return 269;
	case 135: return 275;
	case 136: return 281;
	case 137: return 287;
	case 138: return 293;
	case 139: return 299;
	case 140: return 305;
	case 141: return 311;
	case 142: return 317;
	case 143: return 323;
	case 144: return 329;
	case 145: return 335;
#ifdef __GOD__
	case 146: return 355;
	case 147: return 375;
	case 148: return 395;
	case 149: return 415;
	case 150: return 455;
	case 151: return 495;
	case 152: return 535;
	case 153: return 575;
	case 154: return 635;
	case 155: return 695;
	case 156: return 755;
	case 157: return 815;
#endif
	}
	return 1;
}

int MyUtil::GetAbsorbBonusStats(int tUserIndex)
{
	AVATAR_INFO* mAvatar = &mUSER[tUserIndex].mAvatarInfo;
	OBJECT_FOR_AVATAR* oAvatar = &mAVATAR_OBJECT[tUserIndex].mDATA;

	if (oAvatar->aAnimalOn == 0)
	{
		return 0;
	}
	if (mAvatar->aAbsorbState == 0)
	{
		return 0;
	}
	switch (oAvatar->aAnimalNumber)
	{
	case 1301:
	case 1302:
	case 1303:
	case 1313:
	case 1317:
	case 1320:
	case 1323:
	case 1326:
		return 30;
	case 1304:
	case 1305:
	case 1306:
	case 1314:
	case 1318:
	case 1321:
	case 1324:
	case 1327:
		return 20;
	case 1307:
	case 1308:
	case 1309:
	case 1315:
	case 1319:
	case 1322:
	case 1325:
	case 1328:
		return 10;
	case 1329:
	case 1330:
	case 1331:
		return 15;
	default:
		return 0;
	}
}

//CHECK_POSSIBLE_EQUIP_ITEM
BOOL MyUtil::CheckPossibleEquipItem(int iIndex, int tTribe, int tEquipIndex, int tLevel)
{
	ITEM_INFO* tITEM_INFO;

	tITEM_INFO = mITEM.Search(iIndex);
	if (tITEM_INFO == nullptr)
	{
		return FALSE;
	}
	if (tITEM_INFO->iEquipInfo[0] != 1)
	{
		if ((tITEM_INFO->iEquipInfo[0] - 2) != tTribe)
		{
			return FALSE;
		}
	}
	if ((tEquipIndex >= 0) && (tEquipIndex <= (MAX_EQUIP_SLOT_NUM - 1)))
	{
		if (tITEM_INFO->iEquipInfo[1] != mEquipPart[tEquipIndex])
		{
			return FALSE;
		}
	}
#ifdef __GOD__
	if ((tITEM_INFO->iLevelLimit + tITEM_INFO->iMartialLevelLimit) > tLevel)
#else
	if (tITEM_INFO->iLevelLimit > tLevel)
#endif
	{
		return FALSE;
	}
	return TRUE;
}

void MyUtil::ProcessForExperience(int tUserIndex, int tGainGeneralExperience, int tGainPatExperience,
                                  int tGainLifeValue)
{
	if ((tUserIndex < 0) || (tUserIndex > (mSERVER.mMAX_USER_NUM - 1)) || (!mUSER[tUserIndex].mCheckValidState) || (
		mUSER[tUserIndex].mMoveZoneResult == 1) || (tGainGeneralExperience < 1))
	{
		return;
	}

	int index01;
	float tMaxGeneralExperienceRange;
	int tPresentGeneralExperienceRatio;
	int tNextGeneralExperienceRatio;
	int tPresentAvatarLevel;
	int tNextAvatarLevel;

	if (mUSER[tUserIndex].mAvatarInfo.aLevel1 == MAX_LIMIT_LEVEL_NUM)
	{
		if (mUSER[tUserIndex].mAvatarInfo.aGeneralExperience1 == MAX_NUMBER_SIZE
			&& mUSER[tUserIndex].mAvatarInfo.aLevel2 <= MAX_LIMIT_HIGH_LEVEL_NUM)
		{
			switch (mUSER[tUserIndex].mAvatarInfo.aLevel2)
			{
			case 0:
				mUSER[tUserIndex].mAvatarInfo.aSkillPoint += 100;
				mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 = 0;
				mUSER[tUserIndex].mAvatarInfo.aLevel2 += 1;
				mAVATAR_OBJECT[tUserIndex].mDATA.aLevel2 = mUSER[tUserIndex].mAvatarInfo.aLevel2;
				mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
				mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue = mAVATAR_OBJECT[tUserIndex].GetMaxLife();
				mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue = mAVATAR_OBJECT[tUserIndex].GetMaxMana();
				mTRANSFER.B_AVATAR_CHANGE_INFO_1(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 11,
				                                 mUSER[tUserIndex].mAvatarInfo.aLevel2,
				                                 mUSER[tUserIndex].mAvatarInfo.aSkillPoint, 0);
				mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], 1);
				return;
			case 1:
				if (mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 == mLEVEL.ReturnHighExpValue(
					mUSER[tUserIndex].mAvatarInfo.aLevel2))
				{
					mUSER[tUserIndex].mAvatarInfo.aSkillPoint += 100;
					mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 = 0;
					mUSER[tUserIndex].mAvatarInfo.aLevel2 += 1;
					mAVATAR_OBJECT[tUserIndex].mDATA.aLevel2 = mUSER[tUserIndex].mAvatarInfo.aLevel2;
					mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
					mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue = mAVATAR_OBJECT[tUserIndex].GetMaxLife();
					mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue = mAVATAR_OBJECT[tUserIndex].GetMaxMana();
					mTRANSFER.B_AVATAR_CHANGE_INFO_1(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 11,
					                                 mUSER[tUserIndex].mAvatarInfo.aLevel2,
					                                 mUSER[tUserIndex].mAvatarInfo.aSkillPoint, 0);
					mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], 1);
					return;
				}
				break;
			case 2:
				if (mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 == mLEVEL.ReturnHighExpValue(
					mUSER[tUserIndex].mAvatarInfo.aLevel2))
				{
					mUSER[tUserIndex].mAvatarInfo.aSkillPoint += 100;
					mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 = 0;
					mUSER[tUserIndex].mAvatarInfo.aLevel2 += 1;
					mAVATAR_OBJECT[tUserIndex].mDATA.aLevel2 = mUSER[tUserIndex].mAvatarInfo.aLevel2;
					mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
					mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue = mAVATAR_OBJECT[tUserIndex].GetMaxLife();
					mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue = mAVATAR_OBJECT[tUserIndex].GetMaxMana();
					mTRANSFER.B_AVATAR_CHANGE_INFO_1(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 11,
					                                 mUSER[tUserIndex].mAvatarInfo.aLevel2,
					                                 mUSER[tUserIndex].mAvatarInfo.aSkillPoint, 0);
					mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], 1);
					return;
				}
				break;
			case 3:
				if (mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 == mLEVEL.ReturnHighExpValue(
					mUSER[tUserIndex].mAvatarInfo.aLevel2))
				{
					mUSER[tUserIndex].mAvatarInfo.aSkillPoint += 100;
					mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 = 0;
					mUSER[tUserIndex].mAvatarInfo.aLevel2 += 1;
					mAVATAR_OBJECT[tUserIndex].mDATA.aLevel2 = mUSER[tUserIndex].mAvatarInfo.aLevel2;
					mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
					mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue = mAVATAR_OBJECT[tUserIndex].GetMaxLife();
					mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue = mAVATAR_OBJECT[tUserIndex].GetMaxMana();
					mTRANSFER.B_AVATAR_CHANGE_INFO_1(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 11,
					                                 mUSER[tUserIndex].mAvatarInfo.aLevel2,
					                                 mUSER[tUserIndex].mAvatarInfo.aSkillPoint, 0);
					mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], 1);
					return;
				}
				break;

			case 4:
				if (MAX_LIMIT_HIGH_LEVEL_NUM == mUSER[tUserIndex].mAvatarInfo.aLevel2)
				{
					break;
				}
				if (mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 == mLEVEL.ReturnHighExpValue(
					mUSER[tUserIndex].mAvatarInfo.aLevel2))
				{
					mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 = 0;
					mUSER[tUserIndex].mAvatarInfo.aLevel2 += 1;
					mAVATAR_OBJECT[tUserIndex].mDATA.aLevel2 = mUSER[tUserIndex].mAvatarInfo.aLevel2;
					mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
					mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue = mAVATAR_OBJECT[tUserIndex].GetMaxLife();
					mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue = mAVATAR_OBJECT[tUserIndex].GetMaxMana();
					mTRANSFER.B_AVATAR_CHANGE_INFO_1(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 11,
					                                 mUSER[tUserIndex].mAvatarInfo.aLevel2,
					                                 mUSER[tUserIndex].mAvatarInfo.aSkillPoint, 0);
					mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], 1);
					return;
				}
				break;
			case 5:
				if (mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 == mLEVEL.ReturnHighExpValue(
					mUSER[tUserIndex].mAvatarInfo.aLevel2))
				{
					mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 = 0;
					mUSER[tUserIndex].mAvatarInfo.aLevel2 += 1;
					mAVATAR_OBJECT[tUserIndex].mDATA.aLevel2 = mUSER[tUserIndex].mAvatarInfo.aLevel2;
					mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
					mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue = mAVATAR_OBJECT[tUserIndex].GetMaxLife();
					mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue = mAVATAR_OBJECT[tUserIndex].GetMaxMana();
					mTRANSFER.B_AVATAR_CHANGE_INFO_1(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 11,
					                                 mUSER[tUserIndex].mAvatarInfo.aLevel2,
					                                 mUSER[tUserIndex].mAvatarInfo.aSkillPoint, 0);
					mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], 1);
					return;
				}
				break;
			case 6:
				if (mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 == mLEVEL.ReturnHighExpValue(
					mUSER[tUserIndex].mAvatarInfo.aLevel2))
				{
					mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 = 0;
					mUSER[tUserIndex].mAvatarInfo.aLevel2 += 1;
					mAVATAR_OBJECT[tUserIndex].mDATA.aLevel2 = mUSER[tUserIndex].mAvatarInfo.aLevel2;
					mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
					mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue = mAVATAR_OBJECT[tUserIndex].GetMaxLife();
					mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue = mAVATAR_OBJECT[tUserIndex].GetMaxMana();
					mTRANSFER.B_AVATAR_CHANGE_INFO_1(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 11,
					                                 mUSER[tUserIndex].mAvatarInfo.aLevel2,
					                                 mUSER[tUserIndex].mAvatarInfo.aSkillPoint, 0);
					mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], 1);
					return;
				}
				break;
			case 7:
				if (mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 == mLEVEL.ReturnHighExpValue(
					mUSER[tUserIndex].mAvatarInfo.aLevel2))
				{
					mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 = 0;
					mUSER[tUserIndex].mAvatarInfo.aLevel2 += 1;
					mAVATAR_OBJECT[tUserIndex].mDATA.aLevel2 = mUSER[tUserIndex].mAvatarInfo.aLevel2;
					mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
					mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue = mAVATAR_OBJECT[tUserIndex].GetMaxLife();
					mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue = mAVATAR_OBJECT[tUserIndex].GetMaxMana();
					mTRANSFER.B_AVATAR_CHANGE_INFO_1(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 11,
					                                 mUSER[tUserIndex].mAvatarInfo.aLevel2,
					                                 mUSER[tUserIndex].mAvatarInfo.aSkillPoint, 0);
					mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], 1);
					return;
				}
				break;

			case 8:
				if (MAX_LIMIT_HIGH_LEVEL_NUM == mUSER[tUserIndex].mAvatarInfo.aLevel2)
				{
					break;
				}
				if (mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 == mLEVEL.ReturnHighExpValue(
					mUSER[tUserIndex].mAvatarInfo.aLevel2))
				{
					mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 = 0;
					mUSER[tUserIndex].mAvatarInfo.aLevel2 += 1;
					mAVATAR_OBJECT[tUserIndex].mDATA.aLevel2 = mUSER[tUserIndex].mAvatarInfo.aLevel2;
					mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
					mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue = mAVATAR_OBJECT[tUserIndex].GetMaxLife();
					mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue = mAVATAR_OBJECT[tUserIndex].GetMaxMana();
					mTRANSFER.B_AVATAR_CHANGE_INFO_1(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 11,
					                                 mUSER[tUserIndex].mAvatarInfo.aLevel2,
					                                 mUSER[tUserIndex].mAvatarInfo.aSkillPoint, 0);
					mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], 1);
					return;
				}
				break;
			case 9:
				if (mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 == mLEVEL.ReturnHighExpValue(
					mUSER[tUserIndex].mAvatarInfo.aLevel2))
				{
					mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 = 0;
					mUSER[tUserIndex].mAvatarInfo.aLevel2 += 1;
					mAVATAR_OBJECT[tUserIndex].mDATA.aLevel2 = mUSER[tUserIndex].mAvatarInfo.aLevel2;
					mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
					mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue = mAVATAR_OBJECT[tUserIndex].GetMaxLife();
					mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue = mAVATAR_OBJECT[tUserIndex].GetMaxMana();
					mTRANSFER.B_AVATAR_CHANGE_INFO_1(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 11,
					                                 mUSER[tUserIndex].mAvatarInfo.aLevel2,
					                                 mUSER[tUserIndex].mAvatarInfo.aSkillPoint, 0);
					mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], 1);
					return;
				}
				break;
			case 10:
				if (mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 == mLEVEL.ReturnHighExpValue(
					mUSER[tUserIndex].mAvatarInfo.aLevel2))
				{
					mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 = 0;
					mUSER[tUserIndex].mAvatarInfo.aLevel2 += 1;
					mAVATAR_OBJECT[tUserIndex].mDATA.aLevel2 = mUSER[tUserIndex].mAvatarInfo.aLevel2;
					mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
					mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue = mAVATAR_OBJECT[tUserIndex].GetMaxLife();
					mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue = mAVATAR_OBJECT[tUserIndex].GetMaxMana();
					mTRANSFER.B_AVATAR_CHANGE_INFO_1(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 11,
					                                 mUSER[tUserIndex].mAvatarInfo.aLevel2,
					                                 mUSER[tUserIndex].mAvatarInfo.aSkillPoint, 0);
					mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], 1);
					return;
				}
				break;
			case 11:
				if (mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 == mLEVEL.ReturnHighExpValue(
					mUSER[tUserIndex].mAvatarInfo.aLevel2))
				{
					mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 = 0;
					mUSER[tUserIndex].mAvatarInfo.aLevel2 += 1;
					mAVATAR_OBJECT[tUserIndex].mDATA.aLevel2 = mUSER[tUserIndex].mAvatarInfo.aLevel2;
					mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
					mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue = mAVATAR_OBJECT[tUserIndex].GetMaxLife();
					mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue = mAVATAR_OBJECT[tUserIndex].GetMaxMana();
					mTRANSFER.B_AVATAR_CHANGE_INFO_1(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 11,
					                                 mUSER[tUserIndex].mAvatarInfo.aLevel2,
					                                 mUSER[tUserIndex].mAvatarInfo.aSkillPoint, 0);
					mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], 1);
					return;
				}
				break;
			case 12:
				break;
			}
			if ((mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 + tGainGeneralExperience) > mLEVEL.
				ReturnHighExpValue(mUSER[tUserIndex].mAvatarInfo.aLevel2))
			{
				tGainGeneralExperience = mLEVEL.ReturnHighExpValue(mUSER[tUserIndex].mAvatarInfo.aLevel2) - mUSER[
					tUserIndex].mAvatarInfo.aGeneralExperience2;
			}
			if (tGainGeneralExperience > 0) // God Level Add StatPoints
			{
				if ((mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 + tGainGeneralExperience) < mLEVEL.
					ReturnHighExpValue(mUSER[tUserIndex].mAvatarInfo.aLevel2))
				{
					tMaxGeneralExperienceRange = static_cast<float>(mLEVEL.ReturnHighExpValue(
						mUSER[tUserIndex].mAvatarInfo.aLevel2));
					tPresentGeneralExperienceRatio = static_cast<int>(static_cast<float>(mUSER[tUserIndex].mAvatarInfo.
						aGeneralExperience2) * 100.0f / tMaxGeneralExperienceRange);
					tNextGeneralExperienceRatio = static_cast<int>(static_cast<float>(mUSER[tUserIndex].mAvatarInfo.
							aGeneralExperience2 + tGainGeneralExperience) * 100.0f /
						tMaxGeneralExperienceRange);
					if (tPresentGeneralExperienceRatio < tNextGeneralExperienceRatio)
					{
						/*	mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint += (tNextGeneralExperienceRatio - tPresentGeneralExperienceRatio);
							mTRANSFER.B_AVATAR_CHANGE_INFO_2(2, mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint);
							mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);	*/
					}
				}
				else
				{
					mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint += 100;
					mTRANSFER.B_AVATAR_CHANGE_INFO_2(2, mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint);
					mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				}
				mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2 += tGainGeneralExperience;
				mTRANSFER.B_AVATAR_CHANGE_INFO_2(24, mUSER[tUserIndex].mAvatarInfo.aGeneralExperience2);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			}
			if ((mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue + tGainLifeValue) > mAVATAR_OBJECT[tUserIndex].
				GetMaxLife())
			{
				tGainLifeValue = mAVATAR_OBJECT[tUserIndex].GetMaxLife() - mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue;
			}
			if (tGainLifeValue > 0)
			{
				mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue += tGainLifeValue;
				mTRANSFER.B_AVATAR_CHANGE_INFO_2(10, mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			}
			if (mUSER[tUserIndex].mAvatarInfo.aEquip[8][1] > 0)
			{
				if ((mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1002) ||
					(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1003) ||
					(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1004) ||
					(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1005))
				{
					if ((mUSER[tUserIndex].mAvatarInfo.aEquip[8][2] + tGainPatExperience) > 80000000)
					{
						tGainPatExperience = 80000000 - mUSER[tUserIndex].mAvatarInfo.aEquip[8][2];
					}
				}
				if ((mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1006) ||
					(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1007) ||
					(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1008) ||
					(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1009) ||
					(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1010) ||
					(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1011))
				{
					if ((mUSER[tUserIndex].mAvatarInfo.aEquip[8][2] + tGainPatExperience) > 160000000)
					{
						tGainPatExperience = 160000000 - mUSER[tUserIndex].mAvatarInfo.aEquip[8][2];
					}
				}
				if ((mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1012) ||
					(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1013) ||
					(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1014) ||
					(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1015))
				{
					if ((mUSER[tUserIndex].mAvatarInfo.aEquip[8][2] + tGainPatExperience) > 320000000)
					{
						tGainPatExperience = 320000000 - mUSER[tUserIndex].mAvatarInfo.aEquip[8][2];
					}
				}
				if ((mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1016) ||
					(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1310) ||
					(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1311) ||
					(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1312))
				{
					if ((mUSER[tUserIndex].mAvatarInfo.aEquip[8][2] + tGainPatExperience) > 640000000)
					{
						tGainPatExperience = 640000000 - mUSER[tUserIndex].mAvatarInfo.aEquip[8][2];
					}
				}
			}
			else
			{
				tGainPatExperience = 0;
			}
			if (tGainPatExperience > 0)
			{
				if (mPAT.ReturnGrowStep(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0],
				                        (mUSER[tUserIndex].mAvatarInfo.aEquip[8][2] + tGainPatExperience)) > mPAT.
					ReturnGrowStep(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0],
					               mUSER[tUserIndex].mAvatarInfo.aEquip[8][2]))
				{
					mTRANSFER.B_AVATAR_CHANGE_INFO_1(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 10, 0, 0, 0);
					mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1,
					                false);
				}
				mUSER[tUserIndex].mAvatarInfo.aEquip[8][2] += tGainPatExperience;
				mAVATAR_OBJECT[tUserIndex].mDATA.aEquipForView[8][1] += tGainPatExperience;
				mTRANSFER.B_AVATAR_CHANGE_INFO_2(14, mUSER[tUserIndex].mAvatarInfo.aEquip[8][2]);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			}
			return;
		}
		if ((mUSER[tUserIndex].mAvatarInfo.aGeneralExperience1 + tGainGeneralExperience) > MAX_NUMBER_SIZE)
		{
			tGainGeneralExperience = MAX_NUMBER_SIZE - mUSER[tUserIndex].mAvatarInfo.aGeneralExperience1;
		}
		if (tGainGeneralExperience > 0) // M33 Add Stat Points
		{
			if (mUSER[tUserIndex].mAvatarInfo.aGeneralExperience1 + tGainGeneralExperience < MAX_NUMBER_SIZE)
			{
				tMaxGeneralExperienceRange = static_cast<float>((MAX_NUMBER_SIZE - mLEVEL.ReturnLevelFactor1(
					mUSER[tUserIndex].mAvatarInfo.aLevel1)));
				tPresentGeneralExperienceRatio = static_cast<int>(static_cast<float>(mUSER[tUserIndex].mAvatarInfo.
					aGeneralExperience1 - mLEVEL.
					ReturnLevelFactor1(mUSER[tUserIndex].mAvatarInfo.aLevel1)) * 100.0f / tMaxGeneralExperienceRange);
				tNextGeneralExperienceRatio = static_cast<int>(static_cast<float>(mUSER[tUserIndex].mAvatarInfo.
					aGeneralExperience1 + tGainGeneralExperience - mLEVEL.
					ReturnLevelFactor1(mUSER[tUserIndex].mAvatarInfo.aLevel1)) * 100.0f / tMaxGeneralExperienceRange);
				if (tPresentGeneralExperienceRatio < tNextGeneralExperienceRatio)
				{
					/*	mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint += (tNextGeneralExperienceRatio - tPresentGeneralExperienceRatio);
						mTRANSFER.B_AVATAR_CHANGE_INFO_2(2, mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint);
						mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);	*/
				}
			}
			else
			{
				mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint += 100;
				mTRANSFER.B_AVATAR_CHANGE_INFO_2(2, mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			}
		}
		if (tGainGeneralExperience > 0)
		{
			mUSER[tUserIndex].mAvatarInfo.aGeneralExperience1 += tGainGeneralExperience;
			if (mSERVER_INFO.m_iDeploymentStage <= 0 && mUSER[tUserIndex].mAvatarInfo.aGeneralExperience1 >= 31749751)
			{
				mUSER[tUserIndex].mAvatarInfo.aGeneralExperience1 = 31749751;
			}
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(13, mUSER[tUserIndex].mAvatarInfo.aGeneralExperience1);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			if (mUSER[tUserIndex].mAvatarInfo.aGeneralExperience1 >= MAX_NUMBER_SIZE)
			{
				mUSER[tUserIndex].mAvatarInfo.aDoubleExpTime1 = 0;
				mTRANSFER.B_AVATAR_CHANGE_INFO_2(34, mUSER[tUserIndex].mAvatarInfo.aDoubleExpTime1);
				mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			}
		}
		if ((mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue + tGainLifeValue) > mAVATAR_OBJECT[tUserIndex].GetMaxLife())
		{
			tGainLifeValue = mAVATAR_OBJECT[tUserIndex].GetMaxLife() - mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue;
		}
		if (tGainLifeValue > 0)
		{
			mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue += tGainLifeValue;
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(10, mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		}
		if (mUSER[tUserIndex].mAvatarInfo.aEquip[8][1] > 0)
		{
			if ((mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1002) ||
				(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1003) ||
				(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1004) ||
				(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1005))
			{
				if ((mUSER[tUserIndex].mAvatarInfo.aEquip[8][2] + tGainPatExperience) > 80000000)
				{
					tGainPatExperience = 80000000 - mUSER[tUserIndex].mAvatarInfo.aEquip[8][2];
				}
			}
			if ((mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1006) ||
				(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1007) ||
				(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1008) ||
				(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1009) ||
				(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1010) ||
				(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1011))
			{
				if ((mUSER[tUserIndex].mAvatarInfo.aEquip[8][2] + tGainPatExperience) > 160000000)
				{
					tGainPatExperience = 160000000 - mUSER[tUserIndex].mAvatarInfo.aEquip[8][2];
				}
			}
			if ((mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1012) ||
				(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1013) ||
				(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1014) ||
				(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1015))
			{
				if ((mUSER[tUserIndex].mAvatarInfo.aEquip[8][2] + tGainPatExperience) > 320000000)
				{
					tGainPatExperience = 320000000 - mUSER[tUserIndex].mAvatarInfo.aEquip[8][2];
				}
			}
			if ((mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1016) ||
				(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1310) ||
				(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1311) ||
				(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1312))
			{
				if ((mUSER[tUserIndex].mAvatarInfo.aEquip[8][2] + tGainPatExperience) > 640000000)
				{
					tGainPatExperience = 640000000 - mUSER[tUserIndex].mAvatarInfo.aEquip[8][2];
				}
			}
		}
		else
		{
			tGainPatExperience = 0;
		}
		if (tGainPatExperience > 0)
		{
			if (mPAT.ReturnGrowStep(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0],
			                        (mUSER[tUserIndex].mAvatarInfo.aEquip[8][2] + tGainPatExperience)) > mPAT.
				ReturnGrowStep(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0], mUSER[tUserIndex].mAvatarInfo.aEquip[8][2]))
			{
				mTRANSFER.B_AVATAR_CHANGE_INFO_1(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 10, 0, 0, 0);
				mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1,
				                false);
			}
			mUSER[tUserIndex].mAvatarInfo.aEquip[8][2] += tGainPatExperience;
			mAVATAR_OBJECT[tUserIndex].mDATA.aEquipForView[8][1] += tGainPatExperience;
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(14, mUSER[tUserIndex].mAvatarInfo.aEquip[8][2]);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		}
		return;
	}

	tPresentAvatarLevel = mLEVEL.ReturnLevel(mUSER[tUserIndex].mAvatarInfo.aGeneralExperience1);
	tNextAvatarLevel = mLEVEL.ReturnLevel((mUSER[tUserIndex].mAvatarInfo.aGeneralExperience1 + tGainGeneralExperience));

	if (mSERVER_INFO.m_iDeploymentStage <= 0 && tNextAvatarLevel == 113)
		tNextAvatarLevel = 112;

	if (tNextAvatarLevel > tPresentAvatarLevel)
	{
		mTRANSFER.B_AVATAR_CHANGE_INFO_1(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 1,
		                                 (tNextAvatarLevel - tPresentAvatarLevel), 0, 0);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, tUserIndex,
		                false);
		for (index01 = (tPresentAvatarLevel + 1); index01 <= tNextAvatarLevel; index01++)
		{
			if ((index01 - 1) < 99)
			{
				mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint += 5;
			}
			else if ((index01 - 1) < 112)
			{
				mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint += 15;
			}
			else
			{
				mUSER[tUserIndex].mAvatarInfo.aStateBonusPoint += 30;
			}

			mUSER[tUserIndex].mAvatarInfo.aSkillPoint += mLEVEL.ReturnLevelFactor3(index01);
		}
		mUSER[tUserIndex].mAvatarInfo.aLevel1 = tNextAvatarLevel;
		mAVATAR_OBJECT[tUserIndex].mDATA.aLevel1 = tNextAvatarLevel;
		mAVATAR_OBJECT[tUserIndex].SetBasicAbilityFromEquip();
		mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue = mAVATAR_OBJECT[tUserIndex].GetMaxLife();
		mAVATAR_OBJECT[tUserIndex].mDATA.aManaValue = mAVATAR_OBJECT[tUserIndex].GetMaxMana();

		if (30 < tNextAvatarLevel)
		{
		}
		if (45 == tNextAvatarLevel)
		{
			mUSER[tUserIndex].mAvatarInfo.aBeginnerPackageIcon = 1;
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(107, mUSER[tUserIndex].mAvatarInfo.aBeginnerPackageIcon);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		}
		if (75 == tNextAvatarLevel)
		{
			mUSER[tUserIndex].mAvatarInfo.aBeginnerPackageIcon = 1;
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(107, mUSER[tUserIndex].mAvatarInfo.aBeginnerPackageIcon);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		}
		if (105 == tNextAvatarLevel)
		{
			mUSER[tUserIndex].mAvatarInfo.aBeginnerPackageIcon = 1;
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(107, mUSER[tUserIndex].mAvatarInfo.aBeginnerPackageIcon);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		}
		if (114 == tNextAvatarLevel)
		{
			mUSER[tUserIndex].mAvatarInfo.aBeginnerPackageIcon = 1;
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(107, mUSER[tUserIndex].mAvatarInfo.aBeginnerPackageIcon);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		}
		if (123 == tNextAvatarLevel)
		{
			mUSER[tUserIndex].mAvatarInfo.aBeginnerPackageIcon = 1;
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(107, mUSER[tUserIndex].mAvatarInfo.aBeginnerPackageIcon);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		}
		if (132 == tNextAvatarLevel)
		{
			mUSER[tUserIndex].mAvatarInfo.aBeginnerPackageIcon = 1;
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(107, mUSER[tUserIndex].mAvatarInfo.aBeginnerPackageIcon);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		}
		if (141 == tNextAvatarLevel)
		{
			mUSER[tUserIndex].mAvatarInfo.aBeginnerPackageIcon = 1;
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(107, mUSER[tUserIndex].mAvatarInfo.aBeginnerPackageIcon);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		}
	}

	if (tGainGeneralExperience > 0)
	{
		mUSER[tUserIndex].mAvatarInfo.aGeneralExperience1 += tGainGeneralExperience;
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(13, mUSER[tUserIndex].mAvatarInfo.aGeneralExperience1);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	}
	if ((mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue + tGainLifeValue) > mAVATAR_OBJECT[tUserIndex].GetMaxLife())
	{
		tGainLifeValue = mAVATAR_OBJECT[tUserIndex].GetMaxLife() - mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue;
	}
	if (tGainLifeValue > 0)
	{
		mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue += tGainLifeValue;
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(10, mAVATAR_OBJECT[tUserIndex].mDATA.aLifeValue);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	}
	if (mUSER[tUserIndex].mAvatarInfo.aEquip[8][1] > 0)
	{
		if ((mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1002) ||
			(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1003) ||
			(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1004) ||
			(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1005))
		{
			if ((mUSER[tUserIndex].mAvatarInfo.aEquip[8][2] + tGainPatExperience) > 80000000)
			{
				tGainPatExperience = 80000000 - mUSER[tUserIndex].mAvatarInfo.aEquip[8][2];
			}
		}
		if ((mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1006) ||
			(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1007) ||
			(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1008) ||
			(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1009) ||
			(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1010) ||
			(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1011))
		{
			if ((mUSER[tUserIndex].mAvatarInfo.aEquip[8][2] + tGainPatExperience) > 160000000)
			{
				tGainPatExperience = 160000000 - mUSER[tUserIndex].mAvatarInfo.aEquip[8][2];
			}
		}
		if ((mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1012) ||
			(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1013) ||
			(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1014) ||
			(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1015))
		{
			if ((mUSER[tUserIndex].mAvatarInfo.aEquip[8][2] + tGainPatExperience) > 320000000)
			{
				tGainPatExperience = 320000000 - mUSER[tUserIndex].mAvatarInfo.aEquip[8][2];
			}
		}
		if ((mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1016) ||
			(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1310) ||
			(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1311) ||
			(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0] == 1312))
		{
			if ((mUSER[tUserIndex].mAvatarInfo.aEquip[8][2] + tGainPatExperience) > 640000000)
			{
				tGainPatExperience = 640000000 - mUSER[tUserIndex].mAvatarInfo.aEquip[8][2];
			}
		}
	}
	else
	{
		tGainPatExperience = 0;
	}
	if (tGainPatExperience > 0)
	{
		if (mPAT.ReturnGrowStep(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0],
		                        (mUSER[tUserIndex].mAvatarInfo.aEquip[8][2] + tGainPatExperience)) > mPAT.
			ReturnGrowStep(mUSER[tUserIndex].mAvatarInfo.aEquip[8][0], mUSER[tUserIndex].mAvatarInfo.aEquip[8][2]))
		{
			mTRANSFER.B_AVATAR_CHANGE_INFO_1(tUserIndex, mAVATAR_OBJECT[tUserIndex].mUniqueNumber, 10, 0, 0, 0);
			mUTIL.Broadcast(TRUE, &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false);
		}
		mUSER[tUserIndex].mAvatarInfo.aEquip[8][2] += tGainPatExperience;
		mAVATAR_OBJECT[tUserIndex].mDATA.aEquipForView[8][1] += tGainPatExperience;
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(14, mUSER[tUserIndex].mAvatarInfo.aEquip[8][2]);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	}
}

//PROCESS_FOR_DROP_ITEM
BOOL MyUtil::ProcessForDropItem(int tDropSort, int tItemIndex, int tItemQuantity, int tItemValue,
                                int tItemRecognitionNumber, float tLocation[3], char tMaster[MAX_AVATAR_NAME_LENGTH],
                                int tTribe)
{
	int index01;
	ITEM_INFO* tITEM_INFO;

	switch (tDropSort)
	{
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		tITEM_INFO = mITEM.Search(tItemIndex);
		if (tITEM_INFO == nullptr)
		{
			return FALSE;
		}
		if (tITEM_INFO->iSort == 2)
		{
			tItemQuantity = MAX_ITEM_DUPLICATION_NUM;
		}
		break;
	case 5:
		break;
	case 6:
		break;
	case 7:
		break;
	case 8:
		break;
	case 9:
		break;
	case 10:
		break;
	case 11:
		break;
	case 12:
		break;
	default:
		return FALSE;
	}
	tITEM_INFO = mITEM.Search(tItemIndex);
	if (tITEM_INFO == nullptr)
	{
		return FALSE;
	}
	switch (tITEM_INFO->iType)
	{
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	default:
		return FALSE;
	}
	switch (tITEM_INFO->iSort)
	{
	case 1:
		tItemValue = 0;
		if ((tItemQuantity < 1) || (tItemQuantity > MAX_NUMBER_SIZE))
		{
			return FALSE;
		}
	//--------//
	//¼¼·Â±Ý°í//
	//--------//
		if (tDropSort == 1)
		{
			mGAME.AddTribeBankInfo(mGAME.ReturnTribeForSymbol(tTribe),
			                       static_cast<int>(static_cast<float>(tItemQuantity) * 0.1f));
			tItemQuantity -= static_cast<int>(static_cast<float>(tItemQuantity) * 0.1f);
		}
	//--------//
	//--------//
	//--------//
		break;
	case 2:
		tItemValue = 0;
		if ((tItemQuantity < 1) || (tItemQuantity > MAX_ITEM_DUPLICATION_NUM))
		{
			return FALSE;
		}
		break;
	case 3:
	case 4:
	case 5:
	case 6:
		tItemQuantity = 0;
		tItemValue = 0;
		break;
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
		tItemQuantity = 0;
		if ((mUTIL.ReturnISValue(tItemValue) < 0) || (mUTIL.ReturnISValue(tItemValue) > MAX_IMPROVE_ITEM_NUM_NEXT) || (
			mUTIL.ReturnIUValue(tItemValue) < 0) || (mUTIL.ReturnIUValue(tItemValue) > MAX_ADD_ITEM_NUM))
		{
			return FALSE;
		}
		break;
	case 22:
		if ((tItemQuantity < 0) || (tItemQuantity > MAX_PAT_ACTIVITY_SIZE) || (tItemValue < 0) || (tItemValue >
			MAX_NUMBER_SIZE))
		{
			return FALSE;
		}
		break;
	default:
		return FALSE;
	}
	for (index01 = 0; index01 < MAX_ITEM_OBJECT_NUM; index01++)
	{
		if (!mITEM_OBJECT[index01].mCheckValidState)
		{
			break;
		}
	}
	if (index01 == MAX_ITEM_OBJECT_NUM)
	{
		return FALSE;
	}
	mITEM_OBJECT[index01].mCheckValidState = TRUE;
	mITEM_OBJECT[index01].mUniqueNumber = mGAME.mItemObjectUniqueNumber;
	mGAME.mItemObjectUniqueNumber++;
	mITEM_OBJECT[index01].mDATA.iIndex = tItemIndex;
	mITEM_OBJECT[index01].mDATA.iQuantity = tItemQuantity;
	mITEM_OBJECT[index01].mDATA.iValue = tItemValue;
	mITEM_OBJECT[index01].mDATA.iItemRecognitionNumber = tItemRecognitionNumber;
	mITEM_OBJECT[index01].mDATA.iLocation[0] = tLocation[0];
	mITEM_OBJECT[index01].mDATA.iLocation[1] = tLocation[1];
	mITEM_OBJECT[index01].mDATA.iLocation[2] = tLocation[2];
	strncpy(mITEM_OBJECT[index01].mDATA.iMaster, tMaster, MAX_AVATAR_NAME_LENGTH);
	mITEM_OBJECT[index01].mDATA.iCreateTime = GetTickCount();
	mITEM_OBJECT[index01].mDATA.iPresentTime = GetTickCount();
	mITEM_OBJECT[index01].mDATA.iCreateState = 1;
	mITEM_OBJECT[index01].mDATA.iSocket = 0;
	mITEM_OBJECT[index01].mDATA.iUnk00 = 0; //1;
	mITEM_OBJECT[index01].mDATA.iUnk01 = 0; //2;
	mITEM_OBJECT[index01].mDATA.iUnk02 = 0; //3; // PickUpByOthersState 0 = no permission, 1 = have permission
	mITEM_OBJECT[index01].mDATA.iUnk03 = 0; //1;
	mITEM_OBJECT[index01].SetSpaceIndex();
	mITEM_OBJECT[index01].mITEM_INFO = tITEM_INFO;
	mITEM_OBJECT[index01].mUpdateTimeForBroadcast = GetTickCount();
	mTRANSFER.B_ITEM_ACTION_RECV(index01, mITEM_OBJECT[index01].mUniqueNumber, &mITEM_OBJECT[index01].mDATA, 4);
	mUTIL.Broadcast(TRUE, &tLocation[0], UNIT_SCALE_RADIUS3, false);
	return TRUE;
}

//BROADCAST
//MIR
void MyUtil::Broadcast(BOOL tCheckInRange, float tPostLocation[3], int iScale, bool bUDPSend, int type)
{
	int index01;
	int tSpaceIndex[3];
	float tLocation[3];
	float tPostDistance = static_cast<float>(iScale) * MAX_RADIUS_FOR_NETWORK;

	if (tCheckInRange)
	{
		tSpaceIndex[0] = static_cast<int>(tPostLocation[0] / MAX_RADIUS_FOR_NETWORK);
		tSpaceIndex[1] = static_cast<int>(tPostLocation[1] / MAX_RADIUS_FOR_NETWORK);
		tSpaceIndex[2] = static_cast<int>(tPostLocation[2] / MAX_RADIUS_FOR_NETWORK);

		for (index01 = 0; index01 < mSERVER.mMAX_USER_NUM; index01++)
		{
			if (!mAVATAR_OBJECT[index01].mCheckValidState)
			{
				continue;
			}
			if ((abs(mAVATAR_OBJECT[index01].mSPACE_INDEX[0] - tSpaceIndex[0]) > iScale) || (
				abs(mAVATAR_OBJECT[index01].mSPACE_INDEX[1] - tSpaceIndex[1]) > iScale) || (abs(
				mAVATAR_OBJECT[index01].mSPACE_INDEX[2] - tSpaceIndex[2]) > iScale))
			{
				continue;
			}
			CopyMemory(&tLocation[0], &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], 12);
			if (((tLocation[0] - tPostLocation[0]) * (tLocation[0] - tPostLocation[0]) + (tLocation[1] - tPostLocation[
				1]) * (tLocation[1] - tPostLocation[1]) + (tLocation[2] - tPostLocation[2]) * (tLocation[2] -
				tPostLocation[2])) > (tPostDistance * tPostDistance))
			{
				continue;
			}
			if (type == 0)
			{
				mUSER[index01].Send(TRUE, (char*)&mTRANSFER.mOriginal2[0], mTRANSFER.mOriginalSize2, bUDPSend);
			}
			else if (type == 1)
			{
				mUSER[index01].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize, bUDPSend);
			}
			else if (type == 2)
			{
				mUSER[index01].Send(TRUE, (char*)&mTRANSFER.mOriginal_C[0], mTRANSFER.mOriginalSize_C, bUDPSend);
			}
		}
		return;
	}
	for (index01 = 0; index01 < mSERVER.mMAX_USER_NUM; index01++)
	{
		if (mAVATAR_OBJECT[index01].mCheckValidState)
		{
			if (type == 0)
			{
				mUSER[index01].Send(TRUE, (char*)&mTRANSFER.mOriginal2[0], mTRANSFER.mOriginalSize2, bUDPSend);
			}
			else if (type == 1)
			{
				mUSER[index01].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize, bUDPSend);
			}
			else if (type == 2)
			{
				mUSER[index01].Send(TRUE, (char*)&mTRANSFER.mOriginal_C[0], mTRANSFER.mOriginalSize_C, bUDPSend);
			}
		}
	}
}

//BROADCAST
void MyUtil::Broadcast(BOOL tCheckInRange, float tPostLocation[3], int iScale, int tUserIndexForNoSend, bool bUDPSend,
                       int type)
{
	int index01;
	int tSpaceIndex[3];
	float tLocation[3];
	float tPostDistance = static_cast<float>(iScale) * MAX_RADIUS_FOR_NETWORK;

	if (tCheckInRange)
	{
		tSpaceIndex[0] = static_cast<int>(tPostLocation[0] / MAX_RADIUS_FOR_NETWORK);
		tSpaceIndex[1] = static_cast<int>(tPostLocation[1] / MAX_RADIUS_FOR_NETWORK);
		tSpaceIndex[2] = static_cast<int>(tPostLocation[2] / MAX_RADIUS_FOR_NETWORK);
		for (index01 = 0; index01 < mSERVER.mMAX_USER_NUM; index01++)
		{
			if (!mAVATAR_OBJECT[index01].mCheckValidState)
			{
				continue;
			}
			if (index01 == tUserIndexForNoSend)
			{
				continue;
			}
			if ((abs(mAVATAR_OBJECT[index01].mSPACE_INDEX[0] - tSpaceIndex[0]) > iScale) || (
				abs(mAVATAR_OBJECT[index01].mSPACE_INDEX[1] - tSpaceIndex[1]) > iScale) || (abs(
				mAVATAR_OBJECT[index01].mSPACE_INDEX[2] - tSpaceIndex[2]) > iScale))
			{
				continue;
			}
			CopyMemory(&tLocation[0], &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], 12);
			if (((tLocation[0] - tPostLocation[0]) * (tLocation[0] - tPostLocation[0]) + (tLocation[1] - tPostLocation[
				1]) * (tLocation[1] - tPostLocation[1]) + (tLocation[2] - tPostLocation[2]) * (tLocation[2] -
				tPostLocation[2])) > (tPostDistance * tPostDistance))
			{
				continue;
			}
			if (type)
			{
				mUSER[index01].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize, bUDPSend);
			}
			else
			{
				mUSER[index01].Send(TRUE, (char*)&mTRANSFER.mOriginal2[0], mTRANSFER.mOriginalSize2, bUDPSend);
			}
		}
		return;
	}
	for (index01 = 0; index01 < mSERVER.mMAX_USER_NUM; index01++)
	{
		if (mAVATAR_OBJECT[index01].mCheckValidState)
		{
			if (type)
			{
				mUSER[index01].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize, bUDPSend);
			}
			else
			{
				mUSER[index01].Send(TRUE, (char*)&mTRANSFER.mOriginal2[0], mTRANSFER.mOriginalSize2, bUDPSend);
			}
		}
	}
}

//RANDOM_NUMBER
float MyUtil::RandomNumber(float tMinValue, float tMaxValue)
{
	unsigned int tRandom = 0;
	tRandom = rand();
	return (tMinValue + (((tMaxValue - tMinValue) * static_cast<float>(tRandom % 10001)) / 10000.0f));
}

//RANDOM_NUMBER
int MyUtil::RandomNumber(void)
{
	unsigned int tRandom1 = 0;
	unsigned int tRandom2 = 0;
	tRandom1 = rand();
	tRandom2 = rand();
	return ((1 + (tRandom1 % 1000)) * (1 + (tRandom2 % 1000)));
}

//CHECK_IN_RANGE
BOOL MyUtil::CheckInRange(float tLocation01[3], float tLocation02[3], float tDistance)
{
	if (((tLocation01[0] - tLocation02[0]) * (tLocation01[0] - tLocation02[0]) + (tLocation01[1] - tLocation02[1]) * (
		tLocation01[1] - tLocation02[1]) + (tLocation01[2] - tLocation02[2]) * (tLocation01[2] - tLocation02[2])) > (
		tDistance * tDistance))
	{
		return FALSE;
	}
	return TRUE;
}

//GET_LENGTH_XYZ
float MyUtil::GetLengthXYZ(float tLocation01[3], float tLocation02[3])
{
	return sqrtf(
		(tLocation01[0] - tLocation02[0]) * (tLocation01[0] - tLocation02[0]) + (tLocation01[1] - tLocation02[1]) * (
			tLocation01[1] - tLocation02[1]) + (tLocation01[2] - tLocation02[2]) * (tLocation01[2] - tLocation02[2]));
}

//GET_DOUBLE_XYZ
float MyUtil::GetDoubleXYZ(float tLocation01[3], float tLocation02[3])
{
	return ((tLocation01[0] - tLocation02[0]) * (tLocation01[0] - tLocation02[0]) + (tLocation01[1] - tLocation02[1]) *
		(tLocation01[1] - tLocation02[1]) + (tLocation01[2] - tLocation02[2]) * (tLocation01[2] - tLocation02[2]));
}

//GET_LENGTH_X_Z
float MyUtil::GetLengthX_Z(float tLocation01[3], float tLocation02[3])
{
	return sqrtf(
		(tLocation01[0] - tLocation02[0]) * (tLocation01[0] - tLocation02[0]) + (tLocation01[2] - tLocation02[2]) * (
			tLocation01[2] - tLocation02[2]));
}

//GET_DOUBLE_X_Z
float MyUtil::GetDoubleX_Z(float tLocation01[3], float tLocation02[3])
{
	return ((tLocation01[0] - tLocation02[0]) * (tLocation01[0] - tLocation02[0]) + (tLocation01[2] - tLocation02[2]) *
		(tLocation01[2] - tLocation02[2]));
}

//GET_Y_ANGLE
float MyUtil::GetYAngle(float x1, float z1, float x2, float z2)
{
	if ((x1 == x2) && (z1 == z2))
	{
		return 0.0f;
	}

	float tVector[2];
	float tLength;
	float tAngle;
	float tDegreeAngle;

	tVector[0] = x2 - x1;
	tVector[1] = z2 - z1;
	tLength = sqrtf(tVector[0] * tVector[0] + tVector[1] * tVector[1]);
	if (tLength > 0.0f)
	{
		tVector[0] /= tLength;
		tVector[1] /= tLength;
	}
	tVector[1] -= 1.0f;
	tLength = sqrtf(tVector[0] * tVector[0] + tVector[1] * tVector[1]);
	if ((tLength / 2.0f) > 1.0f)
	{
		tAngle = 2.0f * asinf(1.0f);
	}
	else
	{
		tAngle = 2.0f * asinf(tLength / 2.0f);
	}
	if (x1 > x2)
	{
		tAngle = 2.0f * 3.14159265359f - tAngle;
	}
	tDegreeAngle = ((180.0f * tAngle) / 3.14159265359f) + 180.0f;
	if (tDegreeAngle >= 360.0f)
	{
		tDegreeAngle -= 360.0f;
	}
	return tDegreeAngle;
}

//RETURN_ITEM_RECOGNITION_NUMBER
int MyUtil::ReturnItemRecognitionNumber(int tItemType)
{
	time_t tCountSeconds;
	struct tm* tPresentTime;
	char tempString01[1000];
	char tempString02[1000];

	if ((tItemType != 0) && (tItemType != 3) && (tItemType != 4))
	{
		return 0;
	}
	time(&tCountSeconds);
	tPresentTime = localtime(&tCountSeconds);
	if (tPresentTime == nullptr)
	{
		return 0;
	}

	strcpy(tempString01, "");
	if (tPresentTime->tm_mday < 10)
	{
		tempString02[0] = '0';
		tempString02[1] = tPresentTime->tm_mday + '0';
		tempString02[2] = '\0';
	}
	else
	{
		itoa(tPresentTime->tm_mday, tempString02, 10);
	}
	strcat(tempString01, tempString02);
	if (tPresentTime->tm_hour < 10)
	{
		tempString02[0] = '0';
		tempString02[1] = tPresentTime->tm_hour + '0';
		tempString02[2] = '\0';
	}
	else
	{
		itoa(tPresentTime->tm_hour, tempString02, 10);
	}
	strcat(tempString01, tempString02);
	if (tPresentTime->tm_min < 10)
	{
		tempString02[0] = '0';
		tempString02[1] = tPresentTime->tm_min + '0';
		tempString02[2] = '\0';
	}
	else
	{
		itoa(tPresentTime->tm_min, tempString02, 10);
	}
	strcat(tempString01, tempString02);
	if (tPresentTime->tm_sec < 10)
	{
		tempString02[0] = '0';
		tempString02[1] = tPresentTime->tm_sec + '0';
		tempString02[2] = '\0';
	}
	else
	{
		itoa(tPresentTime->tm_sec, tempString02, 10);
	}
	strcat(tempString01, tempString02);
	return atoi(tempString01);
}

//RETURN_NOW_DATE
int MyUtil::ReturnNowDate(void)
{
	time_t tCountSeconds;
	struct tm* tPresentTime;
	char tempString01[1000];
	char tempString02[1000];

	time(&tCountSeconds);
	tPresentTime = localtime(&tCountSeconds);
	if (tPresentTime == nullptr)
	{
		return 0;
	}
#ifdef __WIN32__
	strcpy(tempString01, "");
	itoa((tPresentTime->tm_year + 1900), tempString02, 10);
	strcat(tempString01, tempString02);
	if (tPresentTime->tm_mon < 9)
	{
		tempString02[0] = '0';
		tempString02[1] = tPresentTime->tm_mon + 1 + '0';
		tempString02[2] = '\0';
	}
	else
	{
		itoa((tPresentTime->tm_mon + 1), tempString02, 10);
	}
	strcat(tempString01, tempString02);
	if (tPresentTime->tm_mday < 10)
	{
		tempString02[0] = '0';
		tempString02[1] = tPresentTime->tm_mday + '0';
		tempString02[2] = '\0';
	}
	else
	{
		itoa(tPresentTime->tm_mday, tempString02, 10);
	}
	strcat(tempString01, tempString02);
#else
	_snprintf(tempString01, 1000, "%4.4d%2.2d%2.2d"
	          , tPresentTime->tm_year + 1900, tPresentTime->tm_mon + 1
	          , tPresentTime->tm_mday);
#endif
	return atoi(tempString01);
}

//RETURN_ADD_DATE
int MyUtil::ReturnAddDate(int tPostDateValue, int tAddDayValue)
{
	if (tAddDayValue < 0)
	{
		return -1;
	}

	time_t tCountSeconds;
	struct tm* tPresentTime;
	char tempString01[1000];
	char tempString02[1000];
	int tPresentDateValue;

	time(&tCountSeconds);
	tPresentTime = localtime(&tCountSeconds);
	if (tPresentTime == nullptr)
	{
		return -1;
	}

	_snprintf(tempString01, 1000, "%4.4d%2.2d%2.2d"
	          , tPresentTime->tm_year + 1900, tPresentTime->tm_mon + 1
	          , tPresentTime->tm_mday);

	tPresentDateValue = atoi(tempString01);
	if (tPostDateValue > tPresentDateValue)
	{
		//tAddDayValue += ReturnSubDate( tPostDateValue, tCountSeconds );
		tAddDayValue += ReturnSubDate(tPostDateValue, tPresentDateValue);
	}
	time(&tCountSeconds);
	tCountSeconds += (tAddDayValue * 24 * 3600);
	tPresentTime = localtime(&tCountSeconds);
	if (tPresentTime == nullptr)
	{
		return -1;
	}

	_snprintf(tempString01, 1000, "%4.4d%2.2d%2.2d"
	          , tPresentTime->tm_year + 1900, tPresentTime->tm_mon + 1
	          , tPresentTime->tm_mday);

	return atoi(tempString01);
}

//RETURN_SUB_DATE
int MyUtil::ReturnSubDate(int tPostDateValue, int tSubDayValue)
{
	if (tPostDateValue <= tSubDayValue)
	{
		return 0;
	}

#if 0
	struct tm time_str;
	time_t uPostDataValue;
	char tmpBuffer[8];
	char tempString01[1000];

	::snprintf(tempString01, 1000, "%s", tPostDateValue);
	memcpy(&tmpBuffer, &tempString01[0], 4);
	tmpBuffer[4] = '\0';
	time_str.tm_year = atoi(tmpBuffer) - 1900;
	memcpy(&tmpBuffer, &tempString01[4], 2);
	tmpBuffer[2] = '\0';
	time_str.tm_mon = atoi(tmpBuffer) - 1;
	tmpBuffer[2] = '\0';
	memcpy(&tmpBuffer, &tempString01[6], 2);
	time_str.tm_mday = atoi(tmpBuffer);
	time_str.tm_hour = 0;
	time_str.tm_min = 0;
	time_str.tm_sec = 0;

	if ((uPostDataValue = mktime(&time_str)) == -1) {
		return 0;
	}

	return ((uPostDataValue - tSubDayValue) / (24 * 3600)) + 1;
#endif
#if 1
	int index01;
	char tempString01[1000];
	char tempString02[1000];
	int tMonthDay[12];
	int tYear[2];
	int tMonth[2];
	int tDay[2];

	tMonthDay[0] = 31;
	tMonthDay[1] = 29;
	tMonthDay[2] = 31;
	tMonthDay[3] = 30;
	tMonthDay[4] = 31;
	tMonthDay[5] = 30;
	tMonthDay[6] = 31;
	tMonthDay[7] = 31;
	tMonthDay[8] = 30;
	tMonthDay[9] = 31;
	tMonthDay[10] = 30;
	tMonthDay[11] = 31;
#ifdef __WIN32__
	itoa(tPostDateValue, &tempString01[0], 10);
#else
	sprintf(&tempString01[0], "%d", tPostDateValue);
#endif
	CopyMemory(&tempString02[0], &tempString01[0], 4);
	tempString02[4] = '\0';
	tYear[0] = atoi(tempString02);
	if ((tYear[0] < 1) || (tYear[0] > 9000))
	{
		return 0;
	}
	CopyMemory(&tempString02[0], &tempString01[4], 2);
	tempString02[2] = '\0';
	tMonth[0] = atoi(tempString02);
	if ((tMonth[0] < 1) || (tMonth[0] > 12))
	{
		return 0;
	}
	CopyMemory(&tempString02[0], &tempString01[6], 2);
	tempString02[2] = '\0';
	tDay[0] = atoi(tempString02);
	if ((tDay[0] < 1) || (tDay[0] > 31))
	{
		return 0;
	}
	tDay[0] += (tYear[0] * 365);
	for (index01 = 0; index01 < (tMonth[0] - 1); index01++)
	{
		tDay[0] += tMonthDay[index01];
	}
#ifdef __WIN32__
	itoa(tSubDayValue, &tempString01[0], 10);
#else
	sprintf(&tempString01[0], "%d", tSubDayValue);
#endif
	CopyMemory(&tempString02[0], &tempString01[0], 4);
	tempString02[4] = '\0';
	tYear[1] = atoi(tempString02);
	if ((tYear[1] < 1) || (tYear[1] > 9000))
	{
		return 0;
	}
	CopyMemory(&tempString02[0], &tempString01[4], 2);
	tempString02[2] = '\0';
	tMonth[1] = atoi(tempString02);
	if ((tMonth[1] < 1) || (tMonth[1] > 12))
	{
		return 0;
	}
	CopyMemory(&tempString02[0], &tempString01[6], 2);
	tempString02[2] = '\0';
	tDay[1] = atoi(tempString02);
	if ((tDay[1] < 1) || (tDay[1] > 31))
	{
		return 0;
	}
	tDay[1] += (tYear[1] * 365);
	for (index01 = 0; index01 < (tMonth[1] - 1); index01++)
	{
		tDay[1] += tMonthDay[index01];
	}
	return (tDay[0] - tDay[1]);
#endif
}

//RETURN_VOTE_DATE
int MyUtil::ReturnVoteDate(void)
{
	time_t tCountSeconds;
	struct tm* tPresentTime;
	char tempString01[1000];
	char tempString02[1000];

	time(&tCountSeconds);
	tPresentTime = localtime(&tCountSeconds);
	if (tPresentTime == nullptr)
	{
		return 0;
	}
#ifdef __WIN32__
	strcpy(tempString01, "");
	itoa((tPresentTime->tm_year + 1900), tempString02, 10);
	strcat(tempString01, tempString02);
	if (tPresentTime->tm_mon < 9)
	{
		tempString02[0] = '0';
		tempString02[1] = tPresentTime->tm_mon + 1 + '0';
		tempString02[2] = '\0';
	}
	else
	{
		itoa((tPresentTime->tm_mon + 1), tempString02, 10);
	}
	strcat(tempString01, tempString02);
	tempString02[0] = '0';
	tempString02[1] = '6';
	tempString02[2] = '\0';
	strcat(tempString01, tempString02);
#else
	_snprintf(tempString01, 1000, "%4.4d%2.2d06", tPresentTime->tm_year + 1900
	          , tPresentTime->tm_mon + 1);
#endif
	return atoi(tempString01);
}

//RETURN_I_S_VALUE
int MyUtil::ReturnISValue(int tValue)
{
	char tempValue01[4];

	CopyMemory(&tempValue01[0], &tValue, 4);
	return tempValue01[0];
}

//RETURN_RF_VALUE
int MyUtil::ReturnRFValue(int tValue)
{
	char tempValue01[4];

	CopyMemory(&tempValue01[0], &tValue, 4);
	return tempValue01[2];
}

//RETURN_I_U_VALUE
int MyUtil::ReturnIUValue(int tValue)
{
	char tempValue01[4];

	CopyMemory(&tempValue01[0], &tValue, 4);
	return tempValue01[1];
}

//CHANGE_I_S_VALUE
int MyUtil::ChangeISValue(int tValue, int tISValue)
{
	char tempValue01[4];
	int tResultIValue;
	CopyMemory(&tempValue01[0], &tValue, 4);
	tempValue01[0] += static_cast<char>(tISValue);
	CopyMemory(&tResultIValue, &tempValue01[0], 4);
	return tResultIValue;
}

//CHANGE_REFINE_VALUE
int MyUtil::ChangeRefineValue(int tValue, int tRFValue)
{
	char tempValue01[4];
	int tResultIValue;
	CopyMemory(&tempValue01[0], &tValue, 4);
	tempValue01[2] += static_cast<char>(tRFValue);
	CopyMemory(&tResultIValue, &tempValue01[0], 4);
	return tResultIValue;
}

//CHANGE_I_U_VALUE
int MyUtil::ChangeIUValue(int tValue, int tIUValue)
{
	char tempValue01[4];
	int tResultIValue;

	CopyMemory(&tempValue01[0], &tValue, 4);
	tempValue01[1] += static_cast<char>(tIUValue);
	CopyMemory(&tResultIValue, &tempValue01[0], 4);
	return tResultIValue;
}

//SET_I_S_I_U_VALUE
int MyUtil::SetISIUValue(int tISValue, int tIUValue)
{
	char tempValue01[4];
	int tResultIValue;

	tempValue01[0] = static_cast<char>(tISValue);
	tempValue01[1] = static_cast<char>(tIUValue);
	tempValue01[2] = 0;
	tempValue01[3] = 0;
	CopyMemory(&tResultIValue, &tempValue01[0], 4);
	return tResultIValue;
}

//RETURN_I_U_EFFECT_VALUE
int MyUtil::ReturnIUEffectValue(ITEM_INFO* tITEM_INFO, int tEffectSort)
{
	switch (tEffectSort)
	{
	case 1: //[°ø°Ý·Â]
		switch (tITEM_INFO->iSort)
		{
		case 13: //[¹«±â]
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
			if (tITEM_INFO->iLevel < 100)
			{
				return static_cast<int>(14.34f + (0.00f + static_cast<float>(tITEM_INFO->iLevel - 45) * 0.10f) * 0.72f);
			}
			if (tITEM_INFO->iLevel < 113)
			{
				return static_cast<int>(14.34f + (6.00f + static_cast<float>(tITEM_INFO->iLevel - 100) * 0.20f) *
					0.72f);
			}
			if (tITEM_INFO->iLevel < 146)
			{
				return static_cast<int>(14.34f + (8.00f + static_cast<float>(tITEM_INFO->iLevel - 113) * 0.50f) *
					0.72f);
			}
			break;
		}
		break;
	case 2: //[¹æ¾î·Â]
		switch (tITEM_INFO->iSort)
		{
		case 8: //[¸ÁÅä]
			if (tITEM_INFO->iLevel < 100)
			{
				return static_cast<int>(2.00f + (0.00f + static_cast<float>(tITEM_INFO->iLevel - 45) * 0.10f) * 0.10f);
			}
			if (tITEM_INFO->iLevel < 113)
			{
				return static_cast<int>(2.00f + (6.00f + static_cast<float>(tITEM_INFO->iLevel - 100) * 0.20f) * 0.10f);
			}
			if (tITEM_INFO->iLevel < 146)
			{
				return static_cast<int>(2.00f + (8.00f + static_cast<float>(tITEM_INFO->iLevel - 113) * 0.50f) * 0.10f);
			}
			break;
		case 9: //[º¹Àå]
			if (tITEM_INFO->iLevel < 100)
			{
				return static_cast<int>(6.36f + (0.00f + static_cast<float>(tITEM_INFO->iLevel - 45) * 0.10f) * 0.32f);
			}
			if (tITEM_INFO->iLevel < 113)
			{
				return static_cast<int>(6.36f + (6.00f + static_cast<float>(tITEM_INFO->iLevel - 100) * 0.20f) * 0.32f);
			}
			if (tITEM_INFO->iLevel < 146)
			{
				return static_cast<int>(6.36f + (8.00f + static_cast<float>(tITEM_INFO->iLevel - 113) * 0.50f) * 0.32f);
			}
			break;
		case 10: //[Àå°©]
			if (tITEM_INFO->iLevel < 100)
			{
				return static_cast<int>(1.82f + (0.00f + static_cast<float>(tITEM_INFO->iLevel - 45) * 0.10f) * 0.09f);
			}
			if (tITEM_INFO->iLevel < 113)
			{
				return static_cast<int>(1.82f + (6.00f + static_cast<float>(tITEM_INFO->iLevel - 100) * 0.20f) * 0.09f);
			}
			if (tITEM_INFO->iLevel < 146)
			{
				return static_cast<int>(1.82f + (8.00f + static_cast<float>(tITEM_INFO->iLevel - 113) * 0.50f) * 0.09f);
			}
			break;
		case 12: //[½Å¹ß]
			if (tITEM_INFO->iLevel < 100)
			{
				return static_cast<int>(0.91f + (0.00f + static_cast<float>(tITEM_INFO->iLevel - 45) * 0.10f) * 0.05f);
			}
			if (tITEM_INFO->iLevel < 113)
			{
				return static_cast<int>(0.91f + (6.00f + static_cast<float>(tITEM_INFO->iLevel - 100) * 0.20f) * 0.05f);
			}
			if (tITEM_INFO->iLevel < 146)
			{
				return static_cast<int>(0.91f + (8.00f + static_cast<float>(tITEM_INFO->iLevel - 113) * 0.50f) * 0.05f);
			}
			break;
		}
		break;
	case 3: //[°ø°Ý¼º°ø]
		switch (tITEM_INFO->iSort)
		{
		case 10: //[Àå°©]
			if (tITEM_INFO->iLevel < 100)
			{
				return static_cast<int>(13.36f + (0.00f + static_cast<float>(tITEM_INFO->iLevel - 45) * 0.10f) * 0.67f);
			}
			if (tITEM_INFO->iLevel < 113)
			{
				return static_cast<int>(13.36f + (6.00f + static_cast<float>(tITEM_INFO->iLevel - 100) * 0.20f) *
					0.67f);
			}
			if (tITEM_INFO->iLevel < 146)
			{
				return static_cast<int>(13.36f + (8.00f + static_cast<float>(tITEM_INFO->iLevel - 113) * 0.50f) *
					0.67f);
			}
			break;
		case 13: //[¹«±â]
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
			if (tITEM_INFO->iLevel < 100)
			{
				return static_cast<int>(5.73f + (0.00f + static_cast<float>(tITEM_INFO->iLevel - 45) * 0.10f) * 0.29f);
			}
			if (tITEM_INFO->iLevel < 113)
			{
				return static_cast<int>(5.73f + (6.00f + static_cast<float>(tITEM_INFO->iLevel - 100) * 0.20f) * 0.29f);
			}
			if (tITEM_INFO->iLevel < 146)
			{
				return static_cast<int>(5.73f + (8.00f + static_cast<float>(tITEM_INFO->iLevel - 113) * 0.50f) * 0.29f);
			}
			break;
		}
		break;
	case 4: //[°ø°ÝÈ¸ÇÇ]
		switch (tITEM_INFO->iSort)
		{
		case 9: //[º¹Àå]
			if (tITEM_INFO->iLevel < 100)
			{
				return static_cast<int>(0.95f + (0.00f + static_cast<float>(tITEM_INFO->iLevel - 45) * 0.10f) * 0.05f);
			}
			if (tITEM_INFO->iLevel < 113)
			{
				return static_cast<int>(0.95f + (6.00f + static_cast<float>(tITEM_INFO->iLevel - 100) * 0.20f) * 0.05f);
			}
			if (tITEM_INFO->iLevel < 146)
			{
				return static_cast<int>(0.95f + (8.00f + static_cast<float>(tITEM_INFO->iLevel - 113) * 0.50f) * 0.05f);
			}
			break;
		case 12: //[½Å¹ß]
			if (tITEM_INFO->iLevel < 100)
			{
				return static_cast<int>(2.23f + (0.00f + static_cast<float>(tITEM_INFO->iLevel - 45) * 0.10f) * 0.11f);
			}
			if (tITEM_INFO->iLevel < 113)
			{
				return static_cast<int>(2.23f + (6.00f + static_cast<float>(tITEM_INFO->iLevel - 100) * 0.20f) * 0.11f);
			}
			if (tITEM_INFO->iLevel < 146)
			{
				return static_cast<int>(2.23f + (8.00f + static_cast<float>(tITEM_INFO->iLevel - 113) * 0.50f) * 0.11f);
			}
			break;
		}
		break;
	case 5: //[¼Ó¼º°ø°Ý]
		switch (tITEM_INFO->iSort)
		{
		case 11: //[¹ÝÁö]
			if (tITEM_INFO->iLevel < 100)
			{
				return static_cast<int>(2.00f + (0.00f + static_cast<float>(tITEM_INFO->iLevel - 45) * 0.10f) * 0.26f);
			}
			if (tITEM_INFO->iLevel < 113)
			{
				return static_cast<int>(2.00f + (6.00f + static_cast<float>(tITEM_INFO->iLevel - 100) * 0.20f) * 0.26f);
			}
			if (tITEM_INFO->iLevel < 146)
			{
				return static_cast<int>(2.00f + (8.00f + static_cast<float>(tITEM_INFO->iLevel - 113) * 0.50f) * 0.26f);
			}
			break;
		}
		break;
	case 6: //[¼Ó¼º¹æ¾î]
		switch (tITEM_INFO->iSort)
		{
		case 7: //[¸ñ°ÉÀÌ]
			if (tITEM_INFO->iLevel < 100)
			{
				return static_cast<int>(1.00f + (0.00f + static_cast<float>(tITEM_INFO->iLevel - 45) * 0.10f) * 0.13f);
			}
			if (tITEM_INFO->iLevel < 113)
			{
				return static_cast<int>(1.00f + (6.00f + static_cast<float>(tITEM_INFO->iLevel - 100) * 0.20f) * 0.13f);
			}
			if (tITEM_INFO->iLevel < 146)
			{
				return static_cast<int>(1.00f + (8.00f + static_cast<float>(tITEM_INFO->iLevel - 113) * 0.50f) * 0.13f);
			}
			break;
		}
		break;
	}
	return 0;
}

//RETURN_IS_VALUE_WITH_LEVEL_LIMIT
int MyUtil::ReturnISValueWithLevelLimit(int tLevel, int tISValue)
{
#ifdef __GOD__
	if ((!mGAME.mCheckZone049TypeServer) && (!mGAME.mCheckZone051TypeServer) && (!mGAME.mCheckZone053TypeServer) && (!
		mGAME.mCheckZone267TypeServer))
#else
	if ((!mGAME.mCheckZone049TypeServer) && (!mGAME.mCheckZone051TypeServer) && (!mGAME.mCheckZone053TypeServer))
#endif
	{
		if (tISValue < 50) //if (tISValue < 40)
		{
			return tISValue;
		}
		return 50;
	}
	if (tLevel <= 15)
	{
		if (tISValue < 4)
		{
			return tISValue;
		}
		return 4;
	}
	if (tLevel <= 45)
	{
		if (tISValue < 8)
		{
			return tISValue;
		}
		return 8;
	}
	if (tLevel <= 55)
	{
		if (tISValue < 12)
		{
			return tISValue;
		}
		return 12;
	}
	if (tLevel <= 65)
	{
		if (tISValue < 16)
		{
			return tISValue;
		}
		return 16;
	}
	if (tLevel <= 75)
	{
		if (tISValue < 20)
		{
			return tISValue;
		}
		return 20;
	}
	if (tLevel <= 85)
	{
		if (tISValue < 24)
		{
			return tISValue;
		}
		return 24;
	}
	if (tLevel <= 95)
	{
		if (tISValue < 28)
		{
			return tISValue;
		}
		return 28;
	}
	if (tLevel <= 105)
	{
		if (tISValue < 32)
		{
			return tISValue;
		}
		return 32;
	}
	if (tLevel <= 112)
	{
		if (tISValue < 36)
		{
			return tISValue;
		}
		return 36;
	}
	if (tISValue < 50) //if (tISValue < 40)
	{
		return tISValue;
	}
	return 50;
	return 0;
}

//RETURN_IU_VALUE_WITH_LEVEL_LIMIT
int MyUtil::ReturnIUValueWithLevelLimit(int tLevel, int tIUValue)
{
#ifdef __GOD__
	if ((!mGAME.mCheckZone049TypeServer) && (!mGAME.mCheckZone051TypeServer) && (!mGAME.mCheckZone053TypeServer) && (!
		mGAME.mCheckZone267TypeServer))
#else
	if ((!mGAME.mCheckZone049TypeServer) && (!mGAME.mCheckZone051TypeServer) && (!mGAME.mCheckZone053TypeServer))
#endif
	{
		if (tIUValue < 12)
		{
			return tIUValue;
		}
		return 12;
	}
	if (tLevel <= 15)
	{
		if (tIUValue < 1)
		{
			return tIUValue;
		}
		return 1;
	}
	if (tLevel <= 25)
	{
		if (tIUValue < 2)
		{
			return tIUValue;
		}
		return 2;
	}
	if (tLevel <= 35)
	{
		if (tIUValue < 3)
		{
			return tIUValue;
		}
		return 3;
	}
	if (tLevel <= 45)
	{
		if (tIUValue < 4)
		{
			return tIUValue;
		}
		return 4;
	}
	if (tLevel <= 55)
	{
		if (tIUValue < 5)
		{
			return tIUValue;
		}
		return 5;
	}
	if (tLevel <= 65)
	{
		if (tIUValue < 6)
		{
			return tIUValue;
		}
		return 6;
	}
	if (tLevel <= 75)
	{
		if (tIUValue < 7)
		{
			return tIUValue;
		}
		return 7;
	}
	if (tLevel <= 85)
	{
		if (tIUValue < 8)
		{
			return tIUValue;
		}
		return 8;
	}
	if (tLevel <= 95)
	{
		if (tIUValue < 9)
		{
			return tIUValue;
		}
		return 9;
	}
	if (tLevel <= 105)
	{
		if (tIUValue < 10)
		{
			return tIUValue;
		}
		return 10;
	}
	if (tLevel <= 112)
	{
		if (tIUValue < 11)
		{
			return tIUValue;
		}
		return 11;
	}
	if (tIUValue < 12)
	{
		return tIUValue;
	}
	return 12;
	return 0;
}

#ifdef __GOD__
//RETURN_UPGRADE_VALUE_WITH_LEVEL_LIMIT
int MyUtil::ReturnUpgradeValueWithLevelLimit(int tLevel, int tUpgradeValue)
{
	if ((!mGAME.mCheckZone049TypeServer) && (!mGAME.mCheckZone051TypeServer) && (!mGAME.mCheckZone053TypeServer) && (!
		mGAME.mCheckZone267TypeServer))
	{
		if (tUpgradeValue < 96)
		{
			return tUpgradeValue;
		}
		return 96;
	}
	if (tLevel <= 19)
	{
		if (tUpgradeValue < 4)
		{
			return tUpgradeValue;
		}
		return 4;
	}
	if (tLevel <= 29)
	{
		if (tUpgradeValue < 8)
		{
			return tUpgradeValue;
		}
		return 8;
	}
	if (tLevel <= 39)
	{
		if (tUpgradeValue < 13)
		{
			return tUpgradeValue;
		}
		return 13;
	}
	if (tLevel <= 45)
	{
		if (tUpgradeValue < 17)
		{
			return tUpgradeValue;
		}
		return 17;
	}
	if (tLevel <= 55)
	{
		if (tUpgradeValue < 21)
		{
			return tUpgradeValue;
		}
		return 21;
	}
	if (tLevel <= 65)
	{
		if (tUpgradeValue < 25)
		{
			return tUpgradeValue;
		}
		return 25;
	}
	if (tLevel <= 75)
	{
		if (tUpgradeValue < 29)
		{
			return tUpgradeValue;
		}
		return 29;
	}
	if (tLevel <= 85)
	{
		if (tUpgradeValue < 33)
		{
			return tUpgradeValue;
		}
		return 33;
	}
	if (tLevel <= 95)
	{
		if (tUpgradeValue < 38)
		{
			return tUpgradeValue;
		}
		return 38;
	}
	if (tLevel <= 105)
	{
		if (tUpgradeValue < 42)
		{
			return tUpgradeValue;
		}
		return 42;
	}
	if (tLevel <= 112)
	{
		if (tUpgradeValue < 46)
		{
			return tUpgradeValue;
		}
		return 46;
	}
	if (tLevel <= 115)
	{
		if (tUpgradeValue < 50)
		{
			return tUpgradeValue;
		}
		return 50;
	}
	if (tLevel <= 118)
	{
		if (tUpgradeValue < 54)
		{
			return tUpgradeValue;
		}
		return 54;
	}
	if (tLevel <= 121)
	{
		if (tUpgradeValue < 58)
		{
			return tUpgradeValue;
		}
		return 58;
	}
	if (tLevel <= 124)
	{
		if (tUpgradeValue < 63)
		{
			return tUpgradeValue;
		}
		return 63;
	}
	if (tLevel <= 127)
	{
		if (tUpgradeValue < 67)
		{
			return tUpgradeValue;
		}
		return 67;
	}
	if (tLevel <= 130)
	{
		if (tUpgradeValue < 71)
		{
			return tUpgradeValue;
		}
		return 71;
	}
	if (tLevel <= 133)
	{
		if (tUpgradeValue < 75)
		{
			return tUpgradeValue;
		}
		return 75;
	}
	if (tLevel <= 136)
	{
		if (tUpgradeValue < 79)
		{
			return tUpgradeValue;
		}
		return 79;
	}
	if (tLevel <= 139)
	{
		if (tUpgradeValue < 83)
		{
			return tUpgradeValue;
		}
		return 83;
	}
	if (tLevel <= 142)
	{
		if (tUpgradeValue < 88)
		{
			return tUpgradeValue;
		}
		return 88;
	}
	if (tLevel <= 145)
	{
		if (tUpgradeValue < 92)
		{
			return tUpgradeValue;
		}
		return 92;
	}
	if (tUpgradeValue < 96)
	{
		return tUpgradeValue;
	}
	return 96;
	return 0;
}
#endif

//PROCESS_ITEM_FOR_KILL_OTHER_TRIBE
void MyUtil::ProcessForKillOtherTribe(int tUserIndex, int tOtherIndex, int tZoneNumber)
{
	if ((!mUSER[tUserIndex].mCheckValidState) || (!mUSER[tOtherIndex].mCheckValidState))
	{
		return;
	}

	int tGainGeneralExperience;
	BYTE tData[MAX_BROADCAST_DATA_SIZE];

	if (mUSER[tUserIndex].mHealthState >= 4)
	{
		return;
	}
	if (mGAME.mCheckZone049TypeServer)
	{
		mGAME.mZone049TypeBattleResult[mUSER[tUserIndex].mAvatarInfo.aTribe]++;
	}
#ifdef __GOD__
	if (mGAME.mCheckZone194Server)
	{
		mGAME.mZone194BattleResult[mUSER[tUserIndex].mAvatarInfo.aTribe]++;
	}
	if (mGAME.mCheckZone267TypeServer)
	{
		mGAME.mZone267TypeBattleResult[mUSER[tUserIndex].mAvatarInfo.aTribe]++;
	}
#endif
	if (((mUSER[tUserIndex].mAvatarInfo.aLevel1 + mUSER[tUserIndex].mAvatarInfo.aLevel2) - (mUSER[tOtherIndex].
		mAvatarInfo.aLevel1 + mUSER[tOtherIndex].mAvatarInfo.aLevel2)) > 9)
	{
		return;
	}
	mPLAYUSER_COM.U_CHECK_KILL_OTHER_TRIBE_SEND(&mUSER[tUserIndex].mAvatarInfo.aName[0],
	                                            &mUSER[tOtherIndex].mAvatarInfo.aName[0]);
	if (mPLAYUSER_COM.mRecv_Result != 0)
	{
		return;
	}
	DropItemForKillOtherTribe(tUserIndex, tOtherIndex, tZoneNumber);
	mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += (1 + mGAME.mKillOtherTribeAddValue);
	mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += (1 + mGAME.mKillOtherTribeAddValue);
	if (mUSER[tUserIndex].mAvatarInfo.aTimeEffect == 600 && mUSER[tUserIndex].mAvatarInfo.aTimeEffect2 == 4)
	{
		mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 1;
		mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += 1;
	}
	if (mUSER[tUserIndex].mAvatarInfo.aDailyPVPCount < 10) { mUSER[tUserIndex].mAvatarInfo.aDailyPVPCount++; }
	// HSB Rank Point
	if (mGAME.mWorldInfo.mTribeSymbolBattle == 1)
	{
		if ((mUSER[tUserIndex].mAvatarInfo.aRankPoints + 10) > 3000)
		{
			mUSER[tUserIndex].mAvatarInfo.aRankPoints += 3000 - mUSER[tUserIndex].mAvatarInfo.aRankPoints;
			mAVATAR_OBJECT[tUserIndex].mDATA.aRankPoint += 3000 - mUSER[tUserIndex].mAvatarInfo.aRankPoints;
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(66, mUSER[tUserIndex].mAvatarInfo.aRankPoints);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		}
		else
		{
			mUSER[tUserIndex].mAvatarInfo.aRankPoints += 10;
			mAVATAR_OBJECT[tUserIndex].mDATA.aRankPoint += 10;
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(66, mUSER[tUserIndex].mAvatarInfo.aRankPoints);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			// 1st
			if (mUSER[tUserIndex].mAvatarInfo.aRankPoints == 3000)
			{
				CopyMemory(&tData[0], &mUSER[tUserIndex].mAvatarInfo.aTribe, 4);
				CopyMemory(&tData[4], &mUSER[tUserIndex].mAvatarInfo.aName[0], MAX_AVATAR_NAME_LENGTH);
				mCENTER_COM.U_DEMAND_BROADCAST_INFO(671, &tData[0]);
			}
		}
	}
	/*
	//Mount Exp
	//if ((mUSER[tUserIndex].mAvatarInfo.aAnimalIndex >= 10) && ( ((mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] % 100000000) / 1000000) > 0))
	if ((mUSER[tUserIndex].mAvatarInfo.aAnimalIndex >= 10) && ( ((100000000 % mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10]) / 1000000) > 0))
	{
		//if (((mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] % 100000) + 1000) > 100000)
		if (((100000 % mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10]) + 1000) > 100000)
		{
			//mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] += 100000 - (mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] % 100000);
			mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] += 100000 - (100000 % mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10]);
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(71, mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10]);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		}
		else
		{
			mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10] += 5;
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(71, mUSER[tUserIndex].mAvatarInfo.aAnimalExpActivity[mUSER[tUserIndex].mAvatarInfo.aAnimalIndex - 10]);
			mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		}
	}*/
	if (mUSER[tUserIndex].mAvatarInfo.aDoubleKillNumTime > 0)
	{
		mUSER[tUserIndex].mAvatarInfo.aDoubleKillNumTime--;
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(4, mUSER[tUserIndex].mAvatarInfo.aDoubleKillNumTime);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += (1 + mGAME.mKillOtherTribeAddValue);
		mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe += (1 + mGAME.mKillOtherTribeAddValue);
	}
	mTRANSFER.B_AVATAR_CHANGE_INFO_2(3, mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe);
	mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	tGainGeneralExperience = mLEVEL.ReturnGainExpForKillOtherTribe(mUSER[tUserIndex].mAvatarInfo.aLevel1,
	                                                               mUSER[tOtherIndex].mAvatarInfo.aLevel1);

	switch (tZoneNumber)
	{
	case 49:
	case 51:
	case 53:
	case 146:
	case 147:
	case 149:
	case 150:
	case 151:
	case 153:
		tGainGeneralExperience *= 3;
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
		tGainGeneralExperience *= 5;
		break;
	default:
		tGainGeneralExperience *= mGAME.mKillOtherTribeExperienceUpRatio;
		break;
	}

	if (mUSER[tUserIndex].mAvatarInfo.aDoubleKillExpTime > 0)
	{
		mUSER[tUserIndex].mAvatarInfo.aDoubleKillExpTime--;
		mTRANSFER.B_AVATAR_CHANGE_INFO_2(5, mUSER[tUserIndex].mAvatarInfo.aDoubleKillExpTime);
		mUSER[tUserIndex].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		tGainGeneralExperience *= 2;
	}
	ProcessForExperience(tUserIndex, tGainGeneralExperience, 0, 0);
}

//DROP_ITEM_FOR_KILL_OTHER_TRIBE
void MyUtil::DropItemForKillOtherTribe(int tUserIndex, int tOtherIndex, int tZoneNumber)
{
	ITEM_INFO* tITEM_INFO;
	int tItemLLevel;
	int tItemHLevel;
	int tTribeItemList[9];
	int tRandomValue;
	int tItemType;
	int tItemSort;
	int tSearchNum;
	unsigned int tRandom = 0;

	// Drop Labyrinth Key
	tRandom = rand();
	if (mSERVER_INFO.m_iDeploymentStage > 1)
	{
		if ((tRandom % 100) == 0)
		{
			tITEM_INFO = mITEM.Search(1048);
			if (tITEM_INFO == nullptr)
			{
				return;
			}
			if (!ProcessForDropItem(2, tITEM_INFO->iIndex, 0, 0, ReturnItemRecognitionNumber(0),
			                        &mAVATAR_OBJECT[tOtherIndex].mDATA.aAction.aLocation[0],
			                        &mUSER[tUserIndex].mAvatarInfo.aName[0], 0))
			{
				return;
			}
			return;
		}
	}

	// Drop Taiyan Key
	if (mSERVER_INFO.m_iDeploymentStage > 3)
	{
		tRandom = rand();
		if ((tRandom % 100) == 0)
		{
			tITEM_INFO = mITEM.Search(1049);
			if (tITEM_INFO == nullptr)
			{
				return;
			}
			if (!ProcessForDropItem(2, tITEM_INFO->iIndex, 0, 0, ReturnItemRecognitionNumber(0),
			                        &mAVATAR_OBJECT[tOtherIndex].mDATA.aAction.aLocation[0],
			                        &mUSER[tUserIndex].mAvatarInfo.aName[0], 0))
			{
				return;
			}
			return;
		}
	}

	// Drop Item For Kill Other Tribe
	tRandom = rand();
	if ((tRandom % 10) == 0) //10%
	{
		tRandom = rand() % 17;
		switch (tRandom)
		{
		case 0:
			tITEM_INFO = mITEM.Search(1372);
			break;
		case 1:
			tITEM_INFO = mITEM.Search(1373);
			break;
		case 2:
			tITEM_INFO = mITEM.Search(1374);
			break;
		case 3:
			tITEM_INFO = mITEM.Search(1375);
			break;
		case 4:
			tITEM_INFO = mITEM.Search(1376);
			break;
		case 5:
			tITEM_INFO = mITEM.Search(1377);
			break;
		case 6:
			tITEM_INFO = mITEM.Search(1371);
			break;
		case 7:
			tITEM_INFO = mITEM.Search(1437);
			break;
		case 8:
			tITEM_INFO = mITEM.Search(576);
			break;
		case 9:
			tITEM_INFO = mITEM.Search(1301);
			break;
		case 10:
			tITEM_INFO = mITEM.Search(1302);
			break;
		case 11:
			tITEM_INFO = mITEM.Search(1303);
			break;
		case 12:
			tITEM_INFO = mITEM.Search(1313);
			break;
		case 13:
			tITEM_INFO = mITEM.Search(1317);
			break;
		case 14:
			tITEM_INFO = mITEM.Search(1320);
			break;
		case 15:
			tITEM_INFO = mITEM.Search(1323);
			break;
		case 16:
			tITEM_INFO = mITEM.Search(1326);
			break;
		}
		if (tITEM_INFO == nullptr)
		{
			return;
		}
		if (!ProcessForDropItem(2, tITEM_INFO->iIndex, 1, 0, 0, &mAVATAR_OBJECT[tOtherIndex].mDATA.aAction.aLocation[0],
		                        &mUSER[tUserIndex].mAvatarInfo.aName[0], mUSER[tUserIndex].mAvatarInfo.aTribe))
		{
			return;
		}
	}

	// Drop Equipment For Kill Other Tribe
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
	tRandomValue = (rand() % 10000);
	if (tRandomValue < 1)
	{
		if (mSERVER_INFO.m_iDeploymentStage > 0)
		{
			tItemType = 4;
		}
		else
		{
			tItemType = 3;
		}
	}
	else if (tRandomValue < 100)
	{
		tItemType = 3;
	}
	else
	{
		tItemType = 2;
	}
	tRandom = rand();
	tItemSort = tTribeItemList[(tRandom % 9)];
	tSearchNum = 0;

	bool bIsBlockedItem = false;
	while (tSearchNum < 10)
	{
		tRandom = rand();
		tITEM_INFO = mITEM.Return((tItemLLevel + (tRandom % (tItemHLevel - tItemLLevel + 1))), tItemType, tItemSort);
		if (tITEM_INFO)
		{
			//Skillpiece don't drop.
			if ((tITEM_INFO->iIndex >= 1054) && (tITEM_INFO->iIndex <= 1065))
			{
				tSearchNum++;
				continue;
			}
			switch (mSERVER_INFO.m_iDeploymentStage)
			{
			case 0:
				if (((tITEM_INFO->iIndex >= 1002) && (tITEM_INFO->iIndex <= 1005)) ||
					((tITEM_INFO->iIndex >= 1006) && (tITEM_INFO->iIndex <= 1010)) ||
					((tITEM_INFO->iIndex >= 1011) && (tITEM_INFO->iIndex <= 1014)) ||
					//((tITEM_INFO->iIndex >= 1054) && (tITEM_INFO->iIndex <= 1065)) ||
					((tITEM_INFO->iIndex >= 90567) && (tITEM_INFO->iIndex <= 90569)) ||
					((tITEM_INFO->iIndex >= 1015) && (tITEM_INFO->iIndex <= 1016)))
				{
					bIsBlockedItem = true;
				}
				break;
			case 1:
				if (((tITEM_INFO->iIndex >= 1006) && (tITEM_INFO->iIndex <= 1010)) ||
					((tITEM_INFO->iIndex >= 1011) && (tITEM_INFO->iIndex <= 1014)) ||
					//((tITEM_INFO->iIndex >= 1054) && (tITEM_INFO->iIndex <= 1065)) ||
					((tITEM_INFO->iIndex >= 90567) && (tITEM_INFO->iIndex <= 90569)) ||
					((tITEM_INFO->iIndex >= 1015) && (tITEM_INFO->iIndex <= 1016)))
				{
					bIsBlockedItem = true;
				}
				break;
			case 2:
				if (((tITEM_INFO->iIndex >= 1011) && (tITEM_INFO->iIndex <= 1014)) ||
					//((tITEM_INFO->iIndex >= 1054) && (tITEM_INFO->iIndex <= 1065)) ||
					((tITEM_INFO->iIndex >= 90567) && (tITEM_INFO->iIndex <= 90569)) ||
					((tITEM_INFO->iIndex >= 1015) && (tITEM_INFO->iIndex <= 1016)))
				{
					bIsBlockedItem = true;
				}
				break;
			case 3:
				if (((tITEM_INFO->iIndex >= 1015) && (tITEM_INFO->iIndex <= 1016)))
				{
					bIsBlockedItem = true;
				}
				break;
			default:
				break;
			}

			if (bIsBlockedItem == true)
			{
				tSearchNum++;
				continue;
			}

			if (((tITEM_INFO->iEquipInfo[0] == 1) || ((tITEM_INFO->iEquipInfo[0] - 2) == mUSER[tUserIndex].mAvatarInfo.
				aPreviousTribe)) && (tITEM_INFO->iCheckMonsterDrop == 2))
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
		return;
	}
	if (!ProcessForDropItem(2, tITEM_INFO->iIndex, 0, 0, 100000004,
	                        &mAVATAR_OBJECT[tOtherIndex].mDATA.aAction.aLocation[0],
	                        &mUSER[tUserIndex].mAvatarInfo.aName[0], 0))
	{
		return;
	}
	if (tITEM_INFO->iType == 4)
	{
		//mGAMELOG.GL_607_GAIN_SIN_ITEM( tUserIndex, 5, tITEM_INFO->iIndex, 0, 0, 100000004 );
	}
}

//////mGAMELOG
int MyUtil::SearchAvatar(char tAvatarName[MAX_AVATAR_NAME_LENGTH], int tInvalidIndex)
{
	int index01;

	for (index01 = 0; index01 < mSERVER.mMAX_USER_NUM; index01++)
	{
		if (!mAVATAR_OBJECT[index01].mCheckValidState)
		{
			continue;
		}
		if (index01 == tInvalidIndex)
		{
			continue;
		}
		if (strcmp(mAVATAR_OBJECT[index01].mDATA.aName, tAvatarName) == 0)
		{
			break;
		}
	}
	if (index01 < mSERVER.mMAX_USER_NUM)
	{
		return index01;
	}
	return -1;
}

// # Defense Hack # {
bool MyUtil::CheckInvenForItem(int* tUserIndex, ITEM_INFO* pItem, int* tPage, int* tIndex, int* tXPost, int* tYPost)
{
	int iCount = 0;
	int iOccupyRange = 1;
	ITEM_INFO* tItemInfo = nullptr;

	if (pItem->iSort != 2 && pItem->iSort != 7 && pItem->iSort != 11)
	{
		iOccupyRange = 2;
	}

	for (iCount = 0; iCount < MAX_INVENTORY_SLOT_NUM; iCount++)
	{
		if ((mUSER[*tUserIndex].mAvatarInfo.aInventory[*tPage][*tIndex][1] == (*tXPost - 1)
			&& (mUSER[*tUserIndex].mAvatarInfo.aInventory[*tPage][*tIndex][2] == (*tYPost - 1))))
		{
			if (mUSER[*tUserIndex].mAvatarInfo.aInventory[*tPage][*tIndex][0] < 1)
				continue;
			tItemInfo = mITEM.Search(mUSER[*tUserIndex].mAvatarInfo.aInventory[*tPage][*tIndex][0]);
			if (tItemInfo == nullptr)
			{
				mUSER[*tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return false;
			}
			if (pItem->iSort != 2 && pItem->iSort != 7 && pItem->iSort != 11)
			{
				return false;
			}
		}
		else if ((mUSER[*tUserIndex].mAvatarInfo.aInventory[*tPage][*tIndex][1] == (*tXPost)
			&& (mUSER[*tUserIndex].mAvatarInfo.aInventory[*tPage][*tIndex][2] == (*tYPost - 1))))
		{
			if (mUSER[*tUserIndex].mAvatarInfo.aInventory[*tPage][*tIndex][0] < 1)
				continue;
			tItemInfo = mITEM.Search(mUSER[*tUserIndex].mAvatarInfo.aInventory[*tPage][*tIndex][0]);
			if (tItemInfo == nullptr)
			{
				mUSER[*tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return false;
			}
			if (pItem->iSort != 2 && pItem->iSort != 7 && pItem->iSort != 11)
			{
				return false;
			}
		}
		else if ((mUSER[*tUserIndex].mAvatarInfo.aInventory[*tPage][*tIndex][1] == (*tXPost + 1)
			&& (mUSER[*tUserIndex].mAvatarInfo.aInventory[*tPage][*tIndex][2] == (*tYPost - 1))))
		{
			if (mUSER[*tUserIndex].mAvatarInfo.aInventory[*tPage][*tIndex][0] < 1)
				continue;
			tItemInfo = mITEM.Search(mUSER[*tUserIndex].mAvatarInfo.aInventory[*tPage][*tIndex][0]);
			if (tItemInfo == nullptr)
			{
				mUSER[*tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return false;
			}
			if (pItem->iSort != 2 && pItem->iSort != 7 && pItem->iSort != 11)
			{
				return false;
			}
		}
		else if ((mUSER[*tUserIndex].mAvatarInfo.aInventory[*tPage][*tIndex][1] == (*tXPost - 1)
			&& (mUSER[*tUserIndex].mAvatarInfo.aInventory[*tPage][*tIndex][2] == (*tYPost))))
		{
			if (mUSER[*tUserIndex].mAvatarInfo.aInventory[*tPage][*tIndex][0] < 1)
				continue;
			tItemInfo = mITEM.Search(mUSER[*tUserIndex].mAvatarInfo.aInventory[*tPage][*tIndex][0]);
			if (tItemInfo == nullptr)
			{
				mUSER[*tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return false;
			}
			if (pItem->iSort != 2 && pItem->iSort != 7 && pItem->iSort != 11)
			{
				return false;
			}
		}

		if (iOccupyRange == 2)
		{
			if ((mUSER[*tUserIndex].mAvatarInfo.aInventory[*tPage][*tIndex][1] == (*tXPost + 1)
				&& (mUSER[*tUserIndex].mAvatarInfo.aInventory[*tPage][*tIndex][2] == (*tYPost))))
			{
				if (mUSER[*tUserIndex].mAvatarInfo.aInventory[*tPage][*tIndex][0] > 0)
				{
					return false;
				}
			}
			else if ((mUSER[*tUserIndex].mAvatarInfo.aInventory[*tPage][*tIndex][1] == (*tXPost)
				&& (mUSER[*tUserIndex].mAvatarInfo.aInventory[*tPage][*tIndex][2] == (*tYPost + 1))))
			{
				if (mUSER[*tUserIndex].mAvatarInfo.aInventory[*tPage][*tIndex][0] > 0)
				{
					return false;
				}
			}
			else if ((mUSER[*tUserIndex].mAvatarInfo.aInventory[*tPage][*tIndex][1] == (*tXPost + 1)
				&& (mUSER[*tUserIndex].mAvatarInfo.aInventory[*tPage][*tIndex][2] == (*tYPost + 1))))
			{
				if (mUSER[*tUserIndex].mAvatarInfo.aInventory[*tPage][*tIndex][0] > 0)
				{
					return false;
				}
			}
		}
	}
	return true;
}

// # }
bool MyUtil::FindEmptyInvenForItem(int iUserIndex, ITEM_INFO* pItem, int& iPage, int& iIndex)
{
	bool bResult = true;
	int iMaxPage = 1, iRange = 8, iOccupyRange = 1;
	AVATAR_INFO* pAvatar = &mUSER[iUserIndex].mAvatarInfo;

	if (pAvatar->aExpandInventoryDate >= mUTIL.ReturnNowDate())
		iMaxPage = 2;

	if (pItem->iSort != 2 && pItem->iSort != 7 && pItem->iSort != 11)
	{
		iRange = 7;
		iOccupyRange = 2;
	}

	bool bInvenSlots[MAX_INVENTORY_SLOT_NUM];

	for (int i = 0; i < iMaxPage; i++)
	{
		SetInvenSlotHumanReadable(iUserIndex, i, bInvenSlots);
		for (int j = 0; j < iRange; j++)
		{
			for (int k = 0; k < iRange; k++)
			{
				if (bInvenSlots[j * 8 + k] == false)
				{
					// .. .. .. ....
					bResult = true;
					for (int m = 0; m < iOccupyRange; m++)
					{
						for (int n = 0; n < iOccupyRange; n++)
						{
							int iPlus = j * 8 + k + m * 8 + n;
							if (bInvenSlots[iPlus] == true)
							{
								bResult = false;
								break;
							}
						}

						if (bResult == false) break;
					}

					if (bResult == true)
					{
						iPage = i;
						iIndex = j * 8 + k;
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool MyUtil::SetInvenSlotHumanReadable(int iUserIndex, int iPage, bool bInvenSlots[MAX_INVENTORY_SLOT_NUM])
{
	AVATAR_INFO* pAvatar = &mUSER[iUserIndex].mAvatarInfo;

	for (int i = 0; i < MAX_INVENTORY_SLOT_NUM; i++)
		bInvenSlots[i] = false;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (pAvatar->aInventory[iPage][i * 8 + j][0] > 0)
			{
				int iItemIndex = pAvatar->aInventory[iPage][i * 8 + j][0];
				ITEM_INFO* pItem = mITEM.Search(iItemIndex);
				if (pItem != nullptr)
				{
					int iX = pAvatar->aInventory[iPage][i * 8 + j][1];
					int iY = pAvatar->aInventory[iPage][i * 8 + j][2];
					// .. 1 X 1 ... ..... ... 1.. ...
					bInvenSlots[iY * 8 + iX] = true;

					if (pItem->iSort != 2 && pItem->iSort != 7 && pItem->iSort != 11)
					{
						if (iX < 7)
						{
							bInvenSlots[iY * 8 + iX + 1] = true;
						}

						if (iY < 7)
						{
							bInvenSlots[(iY + 1) * 8 + iX] = true;

							if (iX < 7)
							{
								bInvenSlots[(iY + 1) * 8 + iX + 1] = true;
							}
						}
					}
				}
			}
		}
	}

	return true;
}

int MyUtil::GetEmptyInvenSlotIndex(int iUserIndex, int iPage)
{
	AVATAR_INFO* pAvatar = &mUSER[iUserIndex].mAvatarInfo;

	if (pAvatar == nullptr)
		return -1;

	for (int i = 0; i < MAX_INVENTORY_SLOT_NUM; i++)
	{
		if (pAvatar->aInventory[iPage][i][0] < 1)
			return i;
	}

	return -1;
}

BOOL MyUtil::CheckIfEmptyHPPills(int tUserIndex)
{
	AVATAR_INFO* pAvatar = &mUSER[tUserIndex].mAvatarInfo;
	ITEM_INFO* pItem;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < MAX_HOT_KEY_NUM; j++)
		{
			pItem = mITEM.Search(pAvatar->aHotKey[i][j][0]);
			if ((pItem != nullptr) && (pAvatar->aHotKey[i][j][2] == 3) && (((pItem->iPotionType[0] == 1) || (pItem->
				iPotionType[0] == 2) || (pItem->iPotionType[0] == 5))))
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL MyUtil::CheckIfEmptyMPPills(int tUserIndex)
{
	AVATAR_INFO* pAvatar = &mUSER[tUserIndex].mAvatarInfo;
	ITEM_INFO* pItem;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < MAX_HOT_KEY_NUM; j++)
		{
			pItem = mITEM.Search(pAvatar->aHotKey[i][j][0]);
			if ((pItem != nullptr) && (pAvatar->aHotKey[i][j][2] == 3) && (((pItem->iPotionType[0] == 3) || (pItem->
				iPotionType[0] == 4) || (pItem->iPotionType[0] == 5))))
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL MyUtil::GetEmptyHotKeyPage(int tUserIndex)
{
	AVATAR_INFO* pAvatar = &mUSER[tUserIndex].mAvatarInfo;
	ITEM_INFO* pItem;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < MAX_HOT_KEY_NUM; j++)
		{
			if (pAvatar->aHotKey[i][j][0] < 1)
			{
				return i;
			}
		}
	}
	return -1;
}

BOOL MyUtil::GetEmptyHotKeyIndex(int tUserIndex)
{
	AVATAR_INFO* pAvatar = &mUSER[tUserIndex].mAvatarInfo;
	ITEM_INFO* pItem;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < MAX_HOT_KEY_NUM; j++)
		{
			if (pAvatar->aHotKey[i][j][0] < 1)
			{
				return j;
			}
		}
	}
	return -1;
}

int MyUtil::GetInventoryHPPillPage(int tUserIndex)
{
	AVATAR_INFO* pAvatar = &mUSER[tUserIndex].mAvatarInfo;
	ITEM_INFO* pItem;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < MAX_INVENTORY_SLOT_NUM; j++)
		{
			pItem = mITEM.Search(pAvatar->aInventory[i][j][0]);
			if (pItem == nullptr)
			{
				continue;
			}
			if ((pItem->iPotionType[0] == 1) || (pItem->iPotionType[0] == 2) || (pItem->iPotionType[0] == 5))
			{
				return i;
			}
		}
	}
	return -1;
}

int MyUtil::GetInventoryHPPillIndex(int tUserIndex)
{
	AVATAR_INFO* pAvatar = &mUSER[tUserIndex].mAvatarInfo;
	ITEM_INFO* pItem;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < MAX_INVENTORY_SLOT_NUM; j++)
		{
			pItem = mITEM.Search(pAvatar->aInventory[i][j][0]);
			if (pItem == nullptr)
			{
				continue;
			}
			if ((pItem->iPotionType[0] == 1) || (pItem->iPotionType[0] == 2) || (pItem->iPotionType[0] == 5))
			{
				return j;
			}
		}
	}
	return -1;
}

int MyUtil::GetInventoryMPPillPage(int tUserIndex)
{
	AVATAR_INFO* pAvatar = &mUSER[tUserIndex].mAvatarInfo;
	ITEM_INFO* pItem;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < MAX_INVENTORY_SLOT_NUM; j++)
		{
			pItem = mITEM.Search(pAvatar->aInventory[i][j][0]);
			if (pItem == nullptr)
			{
				continue;
			}
			if ((pItem->iPotionType[0] == 3) || (pItem->iPotionType[0] == 4) || (pItem->iPotionType[0] == 5))
			{
				return i;
			}
		}
	}
	return -1;
}

int MyUtil::GetInventoryMPPillIndex(int tUserIndex)
{
	AVATAR_INFO* pAvatar = &mUSER[tUserIndex].mAvatarInfo;
	ITEM_INFO* pItem;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < MAX_INVENTORY_SLOT_NUM; j++)
		{
			pItem = mITEM.Search(pAvatar->aInventory[i][j][0]);
			if (pItem == nullptr)
			{
				continue;
			}
			if ((pItem->iPotionType[0] == 3) || (pItem->iPotionType[0] == 4) || (pItem->iPotionType[0] == 5))
			{
				return j;
			}
		}
	}
	return -1;
}

int MyUtil::GetInventoryHPPillQuantity(int tUserIndex)
{
	AVATAR_INFO* pAvatar = &mUSER[tUserIndex].mAvatarInfo;
	ITEM_INFO* pItem;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < MAX_INVENTORY_SLOT_NUM; j++)
		{
			pItem = mITEM.Search(pAvatar->aInventory[i][j][0]);
			if (pItem == nullptr)
			{
				continue;
			}
			if ((pItem->iPotionType[0] == 1) || (pItem->iPotionType[0] == 2) || (pItem->iPotionType[0] == 5))
			{
				return pAvatar->aInventory[i][j][3];
			}
		}
	}
	return -1;
}

int MyUtil::GetInventoryMPPillQuantity(int tUserIndex)
{
	AVATAR_INFO* pAvatar = &mUSER[tUserIndex].mAvatarInfo;
	ITEM_INFO* pItem;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < MAX_INVENTORY_SLOT_NUM; j++)
		{
			pItem = mITEM.Search(pAvatar->aInventory[i][j][0]);
			if (pItem == nullptr)
			{
				continue;
			}
			if ((pItem->iPotionType[0] == 3) || (pItem->iPotionType[0] == 4) || (pItem->iPotionType[0] == 5))
			{
				return pAvatar->aInventory[i][j][3];
			}
		}
	}
	return -1;
}

int MyUtil::ReturnHealthState(int tPlayTime)
{
	if (tPlayTime <= 60)
	{
		return 1;
	}
	if (tPlayTime <= 120)
	{
		return 2;
	}
	if (tPlayTime <= 180)
	{
		return 3;
	}
	if (tPlayTime <= 299)
	{
		return 4;
	}
	return 5;
}

//PROCESS_FOR_CHANGE
BOOL MyUtil::ProcessForChange(int tUserIndex)
{
	int tRandomNumber;
	int tSort;
	ITEM_INFO* tITEM_INFO;
	int tItemLLevel;
	int tItemHLevel;
	int tTribeItemList[9];
	int tItemType;
	int tItemSort;
	int tSearchNum;

	tRandomNumber = rand() % 10000;
	if (tRandomNumber < 3000)
	{
		tSort = 1;
	}
	else if (tRandomNumber < 5000)
	{
		tSort = 2;
	}
	else if (tRandomNumber < 6000)
	{
		tSort = 3;
	}
	else if (tRandomNumber < 6400)
	{
		tSort = 4;
	}
	else if (tRandomNumber < 6500)
	{
		tSort = 5;
	}
	else if (tRandomNumber < 6600)
	{
		tSort = 6;
	}
	else if (tRandomNumber < 6700)
	{
		tSort = 7;
	}
	else if (tRandomNumber < 6800)
	{
		tSort = 8;
	}
	else if (tRandomNumber < 9999)
	{
		tSort = 9;
		tItemType = 3;
	}
	else
	{
		tSort = 9;
		tItemType = 4;
	}
	switch (tSort)
	{
	case 1:
		tITEM_INFO = mITEM.Search(1022);
		if (tITEM_INFO == nullptr)
		{
			return FALSE;
		}
		if (!ProcessForDropItem(12, tITEM_INFO->iIndex, 0, 0, ReturnItemRecognitionNumber(0),
		                        &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
		                        &mUSER[tUserIndex].mAvatarInfo.aName[0], 0))
		{
			return FALSE;
		}
	////mGAMELOG.GL_647_KILLOTHERTRIBE_TO_ITEM( tUserIndex, tITEM_INFO->iIndex );
		return TRUE;
	case 2:
		tITEM_INFO = mITEM.Search(1023);
		if (tITEM_INFO == nullptr)
		{
			return FALSE;
		}
		if (!ProcessForDropItem(12, tITEM_INFO->iIndex, 0, 0, ReturnItemRecognitionNumber(0),
		                        &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
		                        &mUSER[tUserIndex].mAvatarInfo.aName[0], 0))
		{
			return FALSE;
		}
	////mGAMELOG.GL_647_KILLOTHERTRIBE_TO_ITEM( tUserIndex, tITEM_INFO->iIndex );
		return TRUE;
	case 3:
		tITEM_INFO = mITEM.Search(1037);
		if (tITEM_INFO == nullptr)
		{
			return FALSE;
		}
		if (!ProcessForDropItem(12, tITEM_INFO->iIndex, 0, 0, ReturnItemRecognitionNumber(0),
		                        &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
		                        &mUSER[tUserIndex].mAvatarInfo.aName[0], 0))
		{
			return FALSE;
		}
	////mGAMELOG.GL_647_KILLOTHERTRIBE_TO_ITEM( tUserIndex, tITEM_INFO->iIndex );
		return TRUE;
	case 4:
		tITEM_INFO = mITEM.Search(1041);
		if (tITEM_INFO == nullptr)
		{
			return FALSE;
		}
		if (!ProcessForDropItem(12, tITEM_INFO->iIndex, 0, 0, ReturnItemRecognitionNumber(0),
		                        &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
		                        &mUSER[tUserIndex].mAvatarInfo.aName[0], 0))
		{
			return FALSE;
		}
	////mGAMELOG.GL_647_KILLOTHERTRIBE_TO_ITEM( tUserIndex, tITEM_INFO->iIndex );
		return TRUE;
	case 5:
		tITEM_INFO = mITEM.Search(1017);
		if (tITEM_INFO == nullptr)
		{
			return FALSE;
		}
		if (!ProcessForDropItem(12, tITEM_INFO->iIndex, 0, 0, ReturnItemRecognitionNumber(0),
		                        &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
		                        &mUSER[tUserIndex].mAvatarInfo.aName[0], 0))
		{
			return FALSE;
		}
	////mGAMELOG.GL_647_KILLOTHERTRIBE_TO_ITEM( tUserIndex, tITEM_INFO->iIndex );
		return TRUE;
	case 6:
		tITEM_INFO = mITEM.Search(1018);
		if (tITEM_INFO == nullptr)
		{
			return FALSE;
		}
		if (!ProcessForDropItem(12, tITEM_INFO->iIndex, 0, 0, ReturnItemRecognitionNumber(0),
		                        &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
		                        &mUSER[tUserIndex].mAvatarInfo.aName[0], 0))
		{
			return FALSE;
		}
	////mGAMELOG.GL_647_KILLOTHERTRIBE_TO_ITEM( tUserIndex, tITEM_INFO->iIndex );
		return TRUE;
	case 7:
		tITEM_INFO = mITEM.Search(1092);
		if (tITEM_INFO == nullptr)
		{
			return FALSE;
		}
		if (!ProcessForDropItem(12, tITEM_INFO->iIndex, 0, 0, ReturnItemRecognitionNumber(0),
		                        &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
		                        &mUSER[tUserIndex].mAvatarInfo.aName[0], 0))
		{
			return FALSE;
		}
	////mGAMELOG.GL_647_KILLOTHERTRIBE_TO_ITEM( tUserIndex, tITEM_INFO->iIndex );
		return TRUE;
	case 8:
		tITEM_INFO = mITEM.Search(1093);
		if (tITEM_INFO == nullptr)
		{
			return FALSE;
		}
		if (!ProcessForDropItem(12, tITEM_INFO->iIndex, 0, 0, ReturnItemRecognitionNumber(0),
		                        &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
		                        &mUSER[tUserIndex].mAvatarInfo.aName[0], 0))
		{
			return FALSE;
		}
	////mGAMELOG.GL_647_KILLOTHERTRIBE_TO_ITEM( tUserIndex, tITEM_INFO->iIndex );
		return TRUE;
	case 9:
		if (mUSER[tUserIndex].mAvatarInfo.aLevel2 < 1)
		{
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
		}
		else
		{
			tItemLLevel = mUSER[tUserIndex].mAvatarInfo.aLevel1 + mUSER[tUserIndex].mAvatarInfo.aLevel2;
			tItemHLevel = mUSER[tUserIndex].mAvatarInfo.aLevel1 + mUSER[tUserIndex].mAvatarInfo.aLevel2;
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
		tItemSort = tTribeItemList[(rand() % 9)];
		tSearchNum = 0;
		while (tSearchNum < 10)
		{
			tITEM_INFO = mITEM.Return((tItemLLevel + (rand() % (tItemHLevel - tItemLLevel + 1))), tItemType, tItemSort);
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
			return FALSE;
		}
		if (!ProcessForDropItem(12, tITEM_INFO->iIndex, 0, 0, ReturnItemRecognitionNumber(0),
		                        &mAVATAR_OBJECT[tUserIndex].mDATA.aAction.aLocation[0],
		                        &mUSER[tUserIndex].mAvatarInfo.aName[0], 0))
		{
			return FALSE;
		}
	////mGAMELOG.GL_647_KILLOTHERTRIBE_TO_ITEM( tUserIndex, tITEM_INFO->iIndex );
		return TRUE;
	}
	return FALSE;
}
#ifdef __GOD__
//CHECK_CHANGE_TRIBE_TIME
BOOL MyUtil::CheckChangeTribeTime(void)
{
	time_t tCountSeconds;
	struct tm* tPresentTime;

	time(&tCountSeconds);
	tPresentTime = localtime(&tCountSeconds);
	if (tPresentTime == nullptr)
	{
		return FALSE;
	}
	if (tPresentTime->tm_wday != 6)
	{
		return FALSE;
	}
	if (tPresentTime->tm_hour < 12)
	{
		return FALSE;
	}
	return TRUE;
}
#endif
// # Defense Hack # {
int MyUtil::GetAttackRadius(int tSkillNumber)
{
	switch (tSkillNumber)
	{
	case 1: return 25;
	case 2: return 25;
	case 3: return 25;
	case 4: return 15;
	case 5: return 15;
	case 6: return 0;
	case 7: return 0;
	case 8: return 20;
	case 9: return 20;
	case 10: return 45;
	case 11: return 0;
	case 12: return 25;
	case 13: return 25;
	case 14: return 50;
	case 15: return 0;
	case 16: return 120;
	case 17: return 120;
	case 18: return 45;
	case 19: return 0;
	case 20: return 0;
	case 21: return 0;
	case 22: return 0;
	case 23: return 15;
	case 24: return 15;
	case 25: return 0;
	case 26: return 0;
	case 27: return 25;
	case 28: return 25;
	case 29: return 50;
	case 30: return 0;
	case 31: return 15;
	case 32: return 15;
	case 33: return 45;
	case 34: return 0;
	case 35: return 120;
	case 36: return 120;
	case 37: return 45;
	case 38: return 0;
	case 39: return 0;
	case 40: return 0;
	case 41: return 0;
	case 42: return 15;
	case 43: return 15;
	case 44: return 0;
	case 45: return 0;
	case 46: return 20;
	case 47: return 20;
	case 48: return 45;
	case 49: return 0;
	case 50: return 25;
	case 51: return 25;
	case 52: return 50;
	case 53: return 0;
	case 54: return 120;
	case 55: return 120;
	case 56: return 45;
	case 57: return 0;
	case 58: return 20;
	case 59: return 20;
	case 60: return 25;
	case 61: return 25;
	case 62: return 120;
	case 63: return 120;
	case 64: return 25;
	case 65: return 25;
	case 66: return 15;
	case 67: return 15;
	case 68: return 120;
	case 69: return 120;
	case 70: return 20;
	case 71: return 20;
	case 72: return 25;
	case 73: return 25;
	case 74: return 120;
	case 75: return 120;
	case 76: return 30;
	case 77: return 30;
	case 78: return 30;
	case 79: return 30;
	case 80: return 150;
	case 81: return 30;
	case 82: return 0;
	case 83: return 0;
	case 84: return 0;
	case 85: return 20;
	case 86: return 20;
	case 87: return 25;
	case 88: return 25;
	case 89: return 120;
	case 90: return 120;
	case 91: return 25;
	case 92: return 25;
	case 93: return 15;
	case 94: return 15;
	case 95: return 120;
	case 96: return 120;
	case 97: return 20;
	case 98: return 20;
	case 99: return 25;
	case 100: return 25;
	case 101: return 120;
	case 102: return 120;
	case 103: return 0;
	case 104: return 0;
	case 105: return 0;
	case 106: return 300;
	case 107: return 300;
	case 108: return 300;
	case 109: return 300;
	case 110: return 300;
	case 111: return 300;
	case 112: return 70;
	case 113: return 70;
	case 114: return 70;
	case 115: return 70;
	case 116: return 70;
	case 117: return 70;
	case 118: return 70;
	case 119: return 70;
	case 120: return 70;
#ifdef __GOD__
	case 121: return 20;
	case 122: return 20;
	case 123: return 25;
	case 124: return 25;
	case 125: return 120;
	case 126: return 120;
	case 127: return 25;
	case 128: return 25;
	case 129: return 15;
	case 130: return 15;
	case 131: return 120;
	case 132: return 120;
	case 133: return 20;
	case 134: return 20;
	case 135: return 25;
	case 136: return 25;
	case 137: return 120;
	case 138: return 120;
#endif
	}
	return 0;
}

float MyUtil::ReturnLengthXYZ(float tLocation01[3], float tLocation02[3])
{
	return sqrtf(
		powf((tLocation01[0] - tLocation02[0]), 2) + powf((tLocation01[1] - tLocation02[1]), 2) + powf(
			(tLocation01[2] - tLocation02[2]), 2));
}

bool MyUtil::CheckRAttackRange(int tUserIndex, int tSkillNumber, float tLocation1[3], float tLocation2[3],
                               float tObjectSize)
{
	int iTemp = 0;
	float fTemp = 0.0f;
	fTemp = ReturnLengthXYZ(&tLocation1[0], &tLocation2[0]);
	//iTemp = GetAttackRadius(tSkillNumber) + 40;

	//iTemp = GetAttackRadius(tSkillNumber) + tObjectSize + 40;
	//LOG_TO_FILE_2("GetAttackRadius : %d, %d\n", GetAttackRadius(tSkillNumber), tObjectSize);

	// # Defense Hack Add02 #
#if 0
	if (tObjectSize < 20.0f) {
		tObjectSize = 20.0f;
	}
#endif
	iTemp = GetAttackRadius(tSkillNumber) + static_cast<int>(ceilf(tObjectSize));

	if (static_cast<int>(fTemp) <= iTemp)
	{
		return true;
	}
	return false;
}

int MyUtil::CheckPVNumber(int tTribe)
{
	int a = 0;

	if (mGAME.mWorldInfo.mProvingGroundsState[0] == (tTribe + 1))
	{
		a++;
	}
	if (mGAME.mWorldInfo.mProvingGroundsState[1] == (tTribe + 1))
	{
		a++;
	}
	if (mGAME.mWorldInfo.mProvingGroundsState[2] == (tTribe + 1))
	{
		a++;
	}
	if (mGAME.mWorldInfo.mProvingGroundsState[3] == (tTribe + 1))
	{
		a++;
	}

	return a;
}

int MyUtil::GetRandomGachaItem()
{
	int tRandom;
	tRandom = rand() % 9;
	switch (tRandom)
	{
	case 0: return 1023; // 15% Mats
	case 1: return 1018; // Chi Elixir
	case 2: return 1092; // Strength Elixir
	case 3: return 1017; // Health Elixir
	case 4: return 1022; // 12% Mats
	case 5: return 1103; // Protection Charm
	case 6: return 692; // CP Ticket (10)
	case 7: return 695; // White Feather
	case 8: return 1021; // 9% Mats
	}
}
