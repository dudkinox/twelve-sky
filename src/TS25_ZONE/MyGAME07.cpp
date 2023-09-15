//-------------------------------------------------------------------------------------------------
//HEADER
//-------------------------------------------------------------------------------------------------
#include "MainApp.h"
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_GAME
//-------------------------------------------------------------------------------------------------
//PROCESS_FOR_BROADCAST_INFO
void MyGame::ProcessForBroadcastInfo(int tInfoSort, BYTE tInfoData[MAX_BROADCAST_INFO_SIZE])
{
	int index01;
	int index02;
	int index03;
	int tZone049TypeZoneIndex;
	int tZone051TypeZoneIndex;
	int tZone053TypeZoneIndex;
	char tAvatarName[MAX_AVATAR_NAME_LENGTH];
	int tTribeGuardIndex1;
	int tTribeGuardIndex2;
	int tTribe;
	int tTribeSymbolIndex;
	int tWinTribeForSymbol;
	int tAllianceIndex;
	int tAllianceTribe1;
	int tAllianceTribe2;
	int tPossibleAllianceDate01;
	int tPossibleAllianceDate02;
	int tTribeForTribeVote;
	int tIndexForTribeVote;
	char tAvatarNameForTribeVote[MAX_AVATAR_NAME_LENGTH];
	int tLevelForTribeVote;
	int tKillOtherTribeForTribeVote;
	int tPointForTribeVote;
	int tTribeForTribeRole;
	int tIndexForTribeRole;
	char tAvatarNameForTribeRole[MAX_AVATAR_NAME_LENGTH];
	int tZone175TypeZoneIndex1;
	int tZone175TypeZoneIndex2;
	char tGuildName[MAX_GUILD_NAME_LENGTH];
	int tGuildRole;
	char tCallName[MAX_CALL_NAME_LENGTH];
	// @ Tribe Formation Skill @
	int tTribeSkillSort = 0;
	int tZoneNumber;
	int tZone195LevelType;
	// @
#ifdef __GOD__
	int tEventSort;
	int tEventValue;
	int tZone267TypeZoneIndex;
#endif
	switch (tInfoSort)
	{
	//-------------//
	//ZONE_049_TYPE//
	//-------------//
	case 1: break;
	case 2:
		CopyMemory(&tZone049TypeZoneIndex, &tInfoData[0], 4);
		mWorldInfo.mZone049TypeState[tZone049TypeZoneIndex] = 1;
		break;
	case 3:
		CopyMemory(&tZone049TypeZoneIndex, &tInfoData[0], 4);
		mWorldInfo.mZone049TypeState[tZone049TypeZoneIndex] = 2;
		break;
	case 4:
		CopyMemory(&tZone049TypeZoneIndex, &tInfoData[0], 4);
		mWorldInfo.mZone049TypeState[tZone049TypeZoneIndex] = 3;
		break;
	case 5:
		CopyMemory(&tZone049TypeZoneIndex, &tInfoData[0], 4);
		mWorldInfo.mZone049TypeState[tZone049TypeZoneIndex] = 5;
		break;
	case 6:
		CopyMemory(&tZone049TypeZoneIndex, &tInfoData[0], 4);
		mWorldInfo.mZone049TypeState[tZone049TypeZoneIndex] = 5;
		break;
	case 7:
		CopyMemory(&tZone049TypeZoneIndex, &tInfoData[0], 4);
		mWorldInfo.mZone049TypeState[tZone049TypeZoneIndex] = 4;
		break;
	case 8:
		CopyMemory(&tZone049TypeZoneIndex, &tInfoData[0], 4);
		mWorldInfo.mZone049TypeState[tZone049TypeZoneIndex] = 5;
		break;
	case 9:
		CopyMemory(&tZone049TypeZoneIndex, &tInfoData[0], 4);
		mWorldInfo.mZone049TypeState[tZone049TypeZoneIndex] = 0;
		break;
	//-------------//
	//ZONE_051_TYPE//
	//-------------//
	case 10: break;
	case 11:
		CopyMemory(&tZone051TypeZoneIndex, &tInfoData[0], 4);
		mWorldInfo.mZone051TypeState[tZone051TypeZoneIndex] = 1;
		break;
	case 12:
		CopyMemory(&tZone051TypeZoneIndex, &tInfoData[0], 4);
		mWorldInfo.mZone051TypeState[tZone051TypeZoneIndex] = 2;
		break;
	case 13:
		CopyMemory(&tZone051TypeZoneIndex, &tInfoData[0], 4);
		mWorldInfo.mZone051TypeState[tZone051TypeZoneIndex] = 3;
		break;
	case 14:
		CopyMemory(&tZone051TypeZoneIndex, &tInfoData[0], 4);
		mWorldInfo.mZone051TypeState[tZone051TypeZoneIndex] = 5;
		break;
	case 15:
		CopyMemory(&tZone051TypeZoneIndex, &tInfoData[0], 4);
		mWorldInfo.mZone051TypeState[tZone051TypeZoneIndex] = 5;
		break;
	case 16:
		CopyMemory(&tZone051TypeZoneIndex, &tInfoData[0], 4);
		mWorldInfo.mZone051TypeState[tZone051TypeZoneIndex] = 4;
		break;
	case 17:
		CopyMemory(&tZone051TypeZoneIndex, &tInfoData[0], 4);
		mWorldInfo.mZone051TypeState[tZone051TypeZoneIndex] = 5;
		break;
	case 18:
		CopyMemory(&tZone051TypeZoneIndex, &tInfoData[0], 4);
		mWorldInfo.mZone051TypeState[tZone051TypeZoneIndex] = 0;
		break;
	//-------------//
	//ZONE_053_TYPE//
	//-------------//
	case 19: break;
	case 20:
		CopyMemory(&tZone053TypeZoneIndex, &tInfoData[0], 4);
		mWorldInfo.mZone053TypeState[tZone053TypeZoneIndex] = 1;
		break;
	case 21:
		CopyMemory(&tZone053TypeZoneIndex, &tInfoData[0], 4);
		mWorldInfo.mZone053TypeState[tZone053TypeZoneIndex] = 2;
		break;
	case 22:
		CopyMemory(&tZone053TypeZoneIndex, &tInfoData[0], 4);
		mWorldInfo.mZone053TypeState[tZone053TypeZoneIndex] = 3;
		break;
	case 23:
		CopyMemory(&tZone053TypeZoneIndex, &tInfoData[0], 4);
		mWorldInfo.mZone053TypeState[tZone053TypeZoneIndex] = 5;
		break;
	case 24:
		CopyMemory(&tZone053TypeZoneIndex, &tInfoData[0], 4);
		mWorldInfo.mZone053TypeState[tZone053TypeZoneIndex] = 5;
		break;
	case 25: break;
	case 26: break;
	case 27: break;
	case 28:
		CopyMemory(&tZone053TypeZoneIndex, &tInfoData[0], 4);
		mWorldInfo.mZone053TypeState[tZone053TypeZoneIndex] = 4;
		break;
	case 29:
		CopyMemory(&tZone053TypeZoneIndex, &tInfoData[0], 4);
		mWorldInfo.mZone053TypeState[tZone053TypeZoneIndex] = 5;
		break;
	case 30:
		CopyMemory(&tZone053TypeZoneIndex, &tInfoData[0], 4);
		mWorldInfo.mZone053TypeState[tZone053TypeZoneIndex] = 0;
		break;
	//------//
	//수호석//
	//------//
	case 31:
		CopyMemory(&tTribeGuardIndex1, &tInfoData[0], 4);
		CopyMemory(&tTribeGuardIndex2, &tInfoData[4], 4);
		mWorldInfo.mTribeGuardState[tTribeGuardIndex1][tTribeGuardIndex2] = 0;
		break;
	case 32:
		CopyMemory(&tTribeGuardIndex1, &tInfoData[0], 4);
		CopyMemory(&tTribeGuardIndex2, &tInfoData[4], 4);
		mWorldInfo.mTribeGuardState[tTribeGuardIndex1][tTribeGuardIndex2] = 1;
		break;
	//--------//
	//ZONE_038//
	//--------//
	case 33: break;
	case 34: break;
	case 35: break;
	case 36: break;
	case 37: break;
	case 38:
		CopyMemory(&tTribe, &tInfoData[0], 4);
		mWorldInfo.mZone038WinTribe = tTribe;
		if (mCheckZone039TypeServer)
		{
			mZone039TypeBattleState = 1;
			mZone039TypePostTick = 0;
		}
		break;
	//----//
	//성석//
	//----//
	case 39: break;
	case 40:
		mWorldInfo.mTribeSymbolBattle = 1;
	/** 2009. 10. 27 : TribeWar renewal (wolf) */
		mWorldInfo.mTribe1Symbol = 0;
		mWorldInfo.mTribe2Symbol = 1;
		mWorldInfo.mTribe3Symbol = 2;
		mWorldInfo.mTribe4Symbol = 3;
		mSUMMON.SummonTribeSymbol();
	/** 2009. 10. 27 : TribeWar renewal (wolf) */
		for (index01 = 0; index01 < mSERVER.mMAX_USER_NUM; index01++)
		{
			mUSER[index01].mAvatarInfo.aRankPoints = 0;
			mAVATAR_OBJECT[index01].mDATA.aRankPoint = 0;
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(66, mUSER[index01].mAvatarInfo.aRankPoints);
			mUSER[index01].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
			mUSER[index01].mAvatarInfo.aRankAbility = 0;
			mTRANSFER.B_AVATAR_CHANGE_INFO_2(68, mUSER[index01].mAvatarInfo.aRankPoints);
			mUSER[index01].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		}

		break;
	case 41: break;
	case 42:
		CopyMemory(&tTribeSymbolIndex, &tInfoData[0], 4);
		CopyMemory(&tWinTribeForSymbol, &tInfoData[4], 4);
		switch (tTribeSymbolIndex)
		{
		case 0:
			mWorldInfo.mTribe1Symbol = tWinTribeForSymbol;
			break;
		case 1:
			mWorldInfo.mTribe2Symbol = tWinTribeForSymbol;
			break;
		case 2:
			mWorldInfo.mTribe3Symbol = tWinTribeForSymbol;
			break;
		case 3:
			mWorldInfo.mTribe4Symbol = tWinTribeForSymbol;
			break;
		case 4:
			mWorldInfo.mMonsterSymbol = tWinTribeForSymbol;
			break;
		}
		break;
	case 43: break;
	case 44: break;
	case 45:
		mWorldInfo.mTribeSymbolBattle = 0;
	// @ Tribe Formation Skill @
		mWorldInfo.mTribeMasterCallAbility[0] = 0;
		mWorldInfo.mTribeMasterCallAbility[1] = 0;
		mWorldInfo.mTribeMasterCallAbility[2] = 0;
		mWorldInfo.mTribeMasterCallAbility[3] = 0;
	// @
		break;
	//------//
	//동맹석//
	//------//
	case 46:
		CopyMemory(&tAllianceTribe1, &tInfoData[0], 4);
		CopyMemory(&tAllianceTribe2, &tInfoData[4], 4);
		if ((mWorldInfo.mAllianceState[0][0] == -1) && (mWorldInfo.mAllianceState[0][1] == -1))
		{
			tAllianceIndex = 0;
		}
		else
		{
			tAllianceIndex = 1;
		}
		mWorldInfo.mPossibleAllianceInfo[tAllianceTribe1][0] = 0;
		mWorldInfo.mPossibleAllianceInfo[tAllianceTribe1][1] = 0;
		mWorldInfo.mPossibleAllianceInfo[tAllianceTribe2][0] = 0;
		mWorldInfo.mPossibleAllianceInfo[tAllianceTribe2][1] = 0;
		mWorldInfo.mAllianceState[tAllianceIndex][0] = tAllianceTribe1;
		mWorldInfo.mAllianceState[tAllianceIndex][1] = tAllianceTribe2;
		break;
	case 47:
		CopyMemory(&tAllianceTribe1, &tInfoData[0], 4);
		CopyMemory(&tAllianceTribe2, &tInfoData[4], 4);
		CopyMemory(&tPossibleAllianceDate01, &tInfoData[8], 4);
		CopyMemory(&tPossibleAllianceDate02, &tInfoData[12], 4);
		if (((mWorldInfo.mAllianceState[0][0] == tAllianceTribe1) && (mWorldInfo.mAllianceState[0][1] ==
			tAllianceTribe2)) || ((mWorldInfo.mAllianceState[0][1] == tAllianceTribe1) && (mWorldInfo.mAllianceState[0][
			0] == tAllianceTribe2)))
		{
			tAllianceIndex = 0;
		}
		else
		{
			tAllianceIndex = 1;
		}
		mWorldInfo.mPossibleAllianceInfo[tAllianceTribe1][0] = tPossibleAllianceDate01;
		mWorldInfo.mPossibleAllianceInfo[tAllianceTribe1][1] = 1;
		mWorldInfo.mPossibleAllianceInfo[tAllianceTribe2][0] = tPossibleAllianceDate02;
		mWorldInfo.mPossibleAllianceInfo[tAllianceTribe2][1] = 1;
		mWorldInfo.mAllianceState[tAllianceIndex][0] = -1;
		mWorldInfo.mAllianceState[tAllianceIndex][1] = -1;
		break;
	case 48: break;
	case 49:
		CopyMemory(&tAllianceTribe1, &tInfoData[0], 4);
		CopyMemory(&tAllianceTribe2, &tInfoData[4], 4);
		CopyMemory(&tPossibleAllianceDate01, &tInfoData[8], 4);
		CopyMemory(&tPossibleAllianceDate02, &tInfoData[12], 4);
		if (((mWorldInfo.mAllianceState[0][0] == tAllianceTribe1) && (mWorldInfo.mAllianceState[0][1] ==
			tAllianceTribe2)) || ((mWorldInfo.mAllianceState[0][1] == tAllianceTribe1) && (mWorldInfo.mAllianceState[0][
			0] == tAllianceTribe2)))
		{
			tAllianceIndex = 0;
		}
		else
		{
			tAllianceIndex = 1;
		}
		mWorldInfo.mPossibleAllianceInfo[tAllianceTribe1][0] = tPossibleAllianceDate01;
		mWorldInfo.mPossibleAllianceInfo[tAllianceTribe1][1] = 1;
		mWorldInfo.mPossibleAllianceInfo[tAllianceTribe2][0] = tPossibleAllianceDate02;
		mWorldInfo.mPossibleAllianceInfo[tAllianceTribe2][1] = 1;
		mWorldInfo.mAllianceState[tAllianceIndex][0] = -1;
		mWorldInfo.mAllianceState[tAllianceIndex][1] = -1;
		break;
	case 50: break;
	//--------//
	//낭인선언//
	//--------//
	case 51: break;
	//-----------//
	//세력장_투표//
	//-----------//
	case 52:
		mWorldInfo.mTribeVoteState[0] = 1;
		mWorldInfo.mTribeVoteState[1] = 1;
		mWorldInfo.mTribeVoteState[2] = 1;
		mWorldInfo.mTribeVoteState[3] = 1;
		for (index01 = 0; index01 < 4; index01++)
		{
			for (index02 = 0; index02 < MAX_TRIBE_VOTE_AVATAR_NUM; index02++)
			{
				strcpy(mTribeInfo.mTribeVoteName[index01][index02], "");
				mTribeInfo.mTribeVoteLevel[index01][index02] = 0;
				mTribeInfo.mTribeVoteKillOtherTribe[index01][index02] = 0;
				mTribeInfo.mTribeVotePoint[index01][index02] = 0;
			}
		}
		break;
	case 53:
		mWorldInfo.mTribeVoteState[0] = 2;
		mWorldInfo.mTribeVoteState[1] = 2;
		mWorldInfo.mTribeVoteState[2] = 2;
		mWorldInfo.mTribeVoteState[3] = 2;
		break;
	case 54:
		mWorldInfo.mTribeVoteState[0] = 3;
		mWorldInfo.mTribeVoteState[1] = 3;
		mWorldInfo.mTribeVoteState[2] = 3;
		mWorldInfo.mTribeVoteState[3] = 3;
		break;
	case 55:
		mWorldInfo.mTribeVoteState[0] = 4;
		mWorldInfo.mTribeVoteState[1] = 4;
		mWorldInfo.mTribeVoteState[2] = 4;
		mWorldInfo.mTribeVoteState[3] = 4;
		for (index01 = 0; index01 < 4; index01++)
		{
			index03 = -1;
			for (index02 = 0; index02 < MAX_TRIBE_VOTE_AVATAR_NUM; index02++)
			{
				if (strcmp(mTribeInfo.mTribeVoteName[index01][index02], "") == 0)
				{
					continue;
				}
				if (mTribeInfo.mTribeVotePoint[index01][index02] < 1)
				{
					continue;
				}
				if (index03 == -1)
				{
					index03 = index02;
				}
				else
				{
					if (mTribeInfo.mTribeVotePoint[index01][index02] > mTribeInfo.mTribeVotePoint[index01][index03])
					{
						index03 = index02;
					}
				}
			}
			if (index03 == -1)
			{
				strcpy(mTribeInfo.mTribeMaster[index01], "");
			}
			else
			{
				strncpy(mTribeInfo.mTribeMaster[index01], mTribeInfo.mTribeVoteName[index01][index03],
				        MAX_AVATAR_NAME_LENGTH);
			}
			for (index02 = 0; index02 < MAX_TRIBE_SUBMASTER_NUM; index02++)
			{
				strcpy(mTribeInfo.mTribeSubMaster[index01][index02], "");
			}
		}
		break;
	case 56:
		mWorldInfo.mTribeVoteState[0] = 0;
		mWorldInfo.mTribeVoteState[1] = 0;
		mWorldInfo.mTribeVoteState[2] = 0;
		mWorldInfo.mTribeVoteState[3] = 0;
		for (index01 = 0; index01 < 4; index01++)
		{
			for (index02 = 0; index02 < MAX_TRIBE_VOTE_AVATAR_NUM; index02++)
			{
				strcpy(mTribeInfo.mTribeVoteName[index01][index02], "");
				mTribeInfo.mTribeVoteLevel[index01][index02] = 0;
				mTribeInfo.mTribeVoteKillOtherTribe[index01][index02] = 0;
				mTribeInfo.mTribeVotePoint[index01][index02] = 0;
			}
		}
		break;
	case 57:
		CopyMemory(&tTribeForTribeVote, &tInfoData[0], 4);
		CopyMemory(&tIndexForTribeVote, &tInfoData[4], 4);
		CopyMemory(&tAvatarNameForTribeVote[0], &tInfoData[8], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&tLevelForTribeVote, &tInfoData[(8 + MAX_AVATAR_NAME_LENGTH)], 4);
		CopyMemory(&tKillOtherTribeForTribeVote, &tInfoData[(12 + MAX_AVATAR_NAME_LENGTH)], 4);
		strncpy(mTribeInfo.mTribeVoteName[tTribeForTribeVote][tIndexForTribeVote], tAvatarNameForTribeVote,
		        MAX_AVATAR_NAME_LENGTH);
		mTribeInfo.mTribeVoteLevel[tTribeForTribeVote][tIndexForTribeVote] = tLevelForTribeVote;
		mTribeInfo.mTribeVoteKillOtherTribe[tTribeForTribeVote][tIndexForTribeVote] = tKillOtherTribeForTribeVote;
		mTribeInfo.mTribeVotePoint[tTribeForTribeVote][tIndexForTribeVote] = 0;
		break;
	case 58:
		CopyMemory(&tTribeForTribeVote, &tInfoData[0], 4);
		CopyMemory(&tIndexForTribeVote, &tInfoData[4], 4);
		strcpy(mTribeInfo.mTribeVoteName[tTribeForTribeVote][tIndexForTribeVote], "");
		mTribeInfo.mTribeVoteLevel[tTribeForTribeVote][tIndexForTribeVote] = 0;
		mTribeInfo.mTribeVoteKillOtherTribe[tTribeForTribeVote][tIndexForTribeVote] = 0;
		mTribeInfo.mTribeVotePoint[tTribeForTribeVote][tIndexForTribeVote] = 0;
		break;
	case 59:
		CopyMemory(&tTribeForTribeVote, &tInfoData[0], 4);
		CopyMemory(&tIndexForTribeVote, &tInfoData[4], 4);
		CopyMemory(&tPointForTribeVote, &tInfoData[8], 4);
		mTribeInfo.mTribeVotePoint[tTribeForTribeVote][tIndexForTribeVote] += tPointForTribeVote;
		break;
	//--------//
	//부세력장//
	//--------//
	case 60:
		CopyMemory(&tTribeForTribeRole, &tInfoData[0], 4);
		CopyMemory(&tIndexForTribeRole, &tInfoData[4], 4);
		CopyMemory(&tAvatarNameForTribeRole[0], &tInfoData[8], MAX_AVATAR_NAME_LENGTH);
		strncpy(mTribeInfo.mTribeSubMaster[tTribeForTribeRole][tIndexForTribeRole], tAvatarNameForTribeRole,
		        MAX_AVATAR_NAME_LENGTH);
		break;
	case 61:
		CopyMemory(&tTribeForTribeRole, &tInfoData[0], 4);
		CopyMemory(&tIndexForTribeRole, &tInfoData[4], 4);
		CopyMemory(&tAvatarNameForTribeRole[0], &tInfoData[8], MAX_AVATAR_NAME_LENGTH);
		strcpy(mTribeInfo.mTribeSubMaster[tTribeForTribeRole][tIndexForTribeRole], "");
		break;
	//--------//
	//세력금고//
	//--------//
	case 62: break;
	//--------//
	//천신미궁//
	//--------//
	case 63: break;
	case 64:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 1;
		break;
	case 65:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 2;
		break;
	case 66:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 3;
		break;
	case 67:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 4;
		break;
	case 68:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 5;
		break;
	case 69:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 23;
		break;
	case 70:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 23;
		break;
	case 71:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 6;
		break;
	case 72:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 23;
		break;
	case 73:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 7;
		break;
	case 74:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 8;
		break;
	case 75:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 9;
		break;
	case 76:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 23;
		break;
	case 77:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 23;
		break;
	case 78:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 10;
		break;
	case 79:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 23;
		break;
	case 80:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 11;
		break;
	case 81:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 12;
		break;
	case 82:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 13;
		break;
	case 83:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 23;
		break;
	case 84:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 23;
		break;
	case 85:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 14;
		break;
	case 86:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 23;
		break;
	case 87:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 15;
		break;
	case 88:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 16;
		break;
	case 89:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 17;
		break;
	case 90:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 23;
		break;
	case 91:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 23;
		break;
	case 92:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 18;
		break;
	case 93:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 23;
		break;
	case 94:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 19;
		break;
	case 95:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 20;
		break;
	case 96:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 21;
		break;
	case 97:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 23;
		break;
	case 98:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 23;
		break;
	case 99:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 22;
		break;
	case 100:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 23;
		break;
	case 110:
		CopyMemory(&tZone175TypeZoneIndex1, &tInfoData[0], 4);
		CopyMemory(&tZone175TypeZoneIndex2, &tInfoData[4], 4);
		mWorldInfo.mZone175TypeState[tZone175TypeZoneIndex1][tZone175TypeZoneIndex2] = 0;
		break;
	case 111:
		break;
	case 112:
		break;
	//----//
	//문파//
	//----//
	case 101: //[창설]
		break;
	case 102: //[가입]
		break;
	case 103: //[탈퇴]
		break;
	case 104: //[알림]
		CopyMemory(&tGuildName[0], &tInfoData[0], MAX_GUILD_NAME_LENGTH);
		tGuildName[(MAX_GUILD_NAME_LENGTH - 1)] = '\0';
		mTRANSFER.B_BROADCAST_INFO_RECV2(tInfoSort, &tInfoData[0]);
		for (index01 = 0; index01 < mSERVER.mMAX_USER_NUM; index01++)
		{
			if (!mAVATAR_OBJECT[index01].mCheckValidState)
			{
				continue;
			}
			if (strcmp(mAVATAR_OBJECT[index01].mDATA.aGuildName, tGuildName) != 0)
			{
				continue;
			}
			mUSER[index01].Send(TRUE, (char*)&mTRANSFER.mOriginal_C[0], mTRANSFER.mOriginalSize_C);
		}
		return;
	case 105: //[해산]
		break;
	case 106: //[확장]
		break;
	case 107: //[제명]
		CopyMemory(&tAvatarName[0], &tInfoData[0], MAX_AVATAR_NAME_LENGTH);
		tAvatarName[(MAX_AVATAR_NAME_LENGTH - 1)] = '\0';
		CopyMemory(&tGuildName[0], &tInfoData[MAX_AVATAR_NAME_LENGTH], MAX_GUILD_NAME_LENGTH);
		tGuildName[(MAX_GUILD_NAME_LENGTH - 1)] = '\0';
		for (index01 = 0; index01 < mSERVER.mMAX_USER_NUM; index01++)
		{
			if (!mAVATAR_OBJECT[index01].mCheckValidState)
			{
				continue;
			}
			if ((strcmp(mAVATAR_OBJECT[index01].mDATA.aGuildName, tGuildName) == 0) && (strcmp(
				mAVATAR_OBJECT[index01].mDATA.aName, tAvatarName) == 0))
			{
				strcpy(mUSER[index01].mAvatarInfo.aGuildName, "");
				mUSER[index01].mAvatarInfo.aGuildRole = 0;
				strcpy(mUSER[index01].mAvatarInfo.aCallName, "");
				strcpy(mAVATAR_OBJECT[index01].mDATA.aGuildName, "");
				mAVATAR_OBJECT[index01].mDATA.aGuildRole = 0;
				strcpy(mAVATAR_OBJECT[index01].mDATA.aCallName, "");
				break;
			}
		}
		break;
	case 108: //[임명]
		CopyMemory(&tAvatarName[0], &tInfoData[0], MAX_AVATAR_NAME_LENGTH);
		tAvatarName[(MAX_AVATAR_NAME_LENGTH - 1)] = '\0';
		CopyMemory(&tGuildName[0], &tInfoData[MAX_AVATAR_NAME_LENGTH], MAX_GUILD_NAME_LENGTH);
		tGuildName[(MAX_GUILD_NAME_LENGTH - 1)] = '\0';
		CopyMemory(&tGuildRole, &tInfoData[(MAX_AVATAR_NAME_LENGTH + MAX_GUILD_NAME_LENGTH)], 4);
		for (index01 = 0; index01 < mSERVER.mMAX_USER_NUM; index01++)
		{
			if (!mAVATAR_OBJECT[index01].mCheckValidState)
			{
				continue;
			}
			if ((strcmp(mAVATAR_OBJECT[index01].mDATA.aGuildName, tGuildName) == 0) && (strcmp(
				mAVATAR_OBJECT[index01].mDATA.aName, tAvatarName) == 0))
			{
				mUSER[index01].mAvatarInfo.aGuildRole = tGuildRole;
				mAVATAR_OBJECT[index01].mDATA.aGuildRole = tGuildRole;
				break;
			}
		}
		mTRANSFER.B_BROADCAST_INFO_RECV2(tInfoSort, &tInfoData[0]);
		for (index01 = 0; index01 < mSERVER.mMAX_USER_NUM; index01++)
		{
			if (!mAVATAR_OBJECT[index01].mCheckValidState)
			{
				continue;
			}
			if (strcmp(mAVATAR_OBJECT[index01].mDATA.aGuildName, tGuildName) != 0)
			{
				continue;
			}
			mUSER[index01].Send(TRUE, (char*)&mTRANSFER.mOriginal_C[0], mTRANSFER.mOriginalSize_C);
		}
		return;
	case 109: //[칭호]
		CopyMemory(&tAvatarName[0], &tInfoData[0], MAX_AVATAR_NAME_LENGTH);
		tAvatarName[(MAX_AVATAR_NAME_LENGTH - 1)] = '\0';
		CopyMemory(&tGuildName[0], &tInfoData[MAX_AVATAR_NAME_LENGTH], MAX_GUILD_NAME_LENGTH);
		tGuildName[(MAX_GUILD_NAME_LENGTH - 1)] = '\0';
		CopyMemory(&tCallName[0], &tInfoData[(MAX_AVATAR_NAME_LENGTH + MAX_GUILD_NAME_LENGTH)], MAX_CALL_NAME_LENGTH);
		tCallName[(MAX_CALL_NAME_LENGTH - 1)] = '\0';
		for (index01 = 0; index01 < mSERVER.mMAX_USER_NUM; index01++)
		{
			if (!mAVATAR_OBJECT[index01].mCheckValidState)
			{
				continue;
			}
			if ((strcmp(mAVATAR_OBJECT[index01].mDATA.aGuildName, tGuildName) == 0) && (strcmp(
				mAVATAR_OBJECT[index01].mDATA.aName, tAvatarName) == 0))
			{
				strncpy(mUSER[index01].mAvatarInfo.aCallName, tCallName, MAX_CALL_NAME_LENGTH);
				strncpy(mAVATAR_OBJECT[index01].mDATA.aCallName, tCallName, MAX_CALL_NAME_LENGTH);
				break;
			}
		}
		mTRANSFER.B_BROADCAST_INFO_RECV2(tInfoSort, &tInfoData[0]);
		for (index01 = 0; index01 < mSERVER.mMAX_USER_NUM; index01++)
		{
			if (!mAVATAR_OBJECT[index01].mCheckValidState)
			{
				continue;
			}
			if (strcmp(mAVATAR_OBJECT[index01].mDATA.aGuildName, tGuildName) != 0)
			{
				continue;
			}
			mUSER[index01].Send(TRUE, (char*)&mTRANSFER.mOriginal_C[0], mTRANSFER.mOriginalSize_C);
		}
		return;
	/* Mir added 08.12.12 */
	/** 2009. 10. 27 : MonsterSymbol eidt (wolf) */
	case 114: break;
	/** 2009. 10. 27 : MonsterSymbol edit (wolf) */

#if 0
	case 201:
		CopyMemory(&tInfoData[0], &mGAME.mGeneralExperienceUpRatio, 4);
		CopyMemory(&tInfoData[4], &mGAME.mItemDropUpRatio, 4);
		mTRANSFER.B_BROADCAST_INFO_RECV(tInfoSort, &tInfoData[0]);
		for (index01 = 0; index01 < mSERVER.mMAX_USER_NUM; index01++) {
			if (!mAVATAR_OBJECT[index01].mCheckValidState) {
				continue;
			}
			if (strcmp(mAVATAR_OBJECT[index01].mDATA.aGuildName, tGuildName) != 0) {
				continue;
			}
			mUSER[index01].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
		}
		return;
#endif

#ifdef __GOD__
	//--------//
	//ZONE_194//
	//--------//
	case 201: break;
	case 202:
		mWorldInfo.mZone194State = 1;
		break;
	case 203:
		mWorldInfo.mZone194State = 2;
		break;
	case 204:
		mWorldInfo.mZone194State = 3;
		break;
	case 205:
		mWorldInfo.mZone194State = 5;
		break;
	case 206:
		mWorldInfo.mZone194State = 4;
		break;
	case 207:
		mWorldInfo.mZone194State = 5;
		break;
	case 208:
		mWorldInfo.mZone194State = 0;
		break;
	//--------------//
	//종족_혜택_정보//
	//--------------//
	case 301:
		CopyMemory(&tEventSort, &tInfoData[0], 4);
		CopyMemory(&tEventValue, &tInfoData[4], 4);
		switch (tEventSort)
		{
		case 21:
			mWorldInfo.mTribeGeneralExperienceUpRatioInfo[0] = static_cast<float>(tEventValue) * 0.1f;
			break;
		case 22:
			mWorldInfo.mTribeGeneralExperienceUpRatioInfo[1] = static_cast<float>(tEventValue) * 0.1f;
			break;
		case 23:
			mWorldInfo.mTribeGeneralExperienceUpRatioInfo[2] = static_cast<float>(tEventValue) * 0.1f;
			break;
		case 24:
			mWorldInfo.mTribeGeneralExperienceUpRatioInfo[3] = static_cast<float>(tEventValue) * 0.1f;
			break;
		case 31:
			mWorldInfo.mTribeItemDropUpRatioInfo[0] = static_cast<float>(tEventValue) * 0.1f;
			break;
		case 32:
			mWorldInfo.mTribeItemDropUpRatioInfo[1] = static_cast<float>(tEventValue) * 0.1f;
			break;
		case 33:
			mWorldInfo.mTribeItemDropUpRatioInfo[2] = static_cast<float>(tEventValue) * 0.1f;
			break;
		case 34:
			mWorldInfo.mTribeItemDropUpRatioInfo[3] = static_cast<float>(tEventValue) * 0.1f;
			break;
		case 41:
			mWorldInfo.mTribeItemDropUpRatioForMyoungInfo[0] = static_cast<float>(tEventValue) * 0.1f;
			break;
		case 42:
			mWorldInfo.mTribeItemDropUpRatioForMyoungInfo[1] = static_cast<float>(tEventValue) * 0.1f;
			break;
		case 43:
			mWorldInfo.mTribeItemDropUpRatioForMyoungInfo[2] = static_cast<float>(tEventValue) * 0.1f;
			break;
		case 44:
			mWorldInfo.mTribeItemDropUpRatioForMyoungInfo[3] = static_cast<float>(tEventValue) * 0.1f;
			break;
		case 51:
			mWorldInfo.mTribeKillOtherTribeAddValueInfo[0] = tEventValue;
			break;
		case 52:
			mWorldInfo.mTribeKillOtherTribeAddValueInfo[1] = tEventValue;
			break;
		case 53:
			mWorldInfo.mTribeKillOtherTribeAddValueInfo[2] = tEventValue;
			break;
		case 54:
			mWorldInfo.mTribeKillOtherTribeAddValueInfo[3] = tEventValue;
			break;
		}
		break;
#endif

	/* Mir end */
	// @ Tribe Formation Skill @
	//--------//
	//세력진법//
	//--------//
	case 302:
		CopyMemory(&tTribe, &tInfoData[0], 4);
		CopyMemory(&tTribeSkillSort, &tInfoData[4], 4);
		mWorldInfo.mTribeMasterCallAbility[tTribe] = tTribeSkillSort;
		break; // @
#ifdef __GOD__
	//-------------//
	//ZONE_267_TYPE//
	//-------------//
	case 402: break;
	case 403:
		CopyMemory(&tZone267TypeZoneIndex, &tInfoData[0], 4);
		mWorldInfo.mZone267TypeState[tZone267TypeZoneIndex] = 1;
		break;
	case 404:
		CopyMemory(&tZone267TypeZoneIndex, &tInfoData[0], 4);
		mWorldInfo.mZone267TypeState[tZone267TypeZoneIndex] = 2;
		break;
	case 405:
		CopyMemory(&tZone267TypeZoneIndex, &tInfoData[0], 4);
		mWorldInfo.mZone267TypeState[tZone267TypeZoneIndex] = 3;
		break;
	case 406:
		CopyMemory(&tZone267TypeZoneIndex, &tInfoData[0], 4);
		mWorldInfo.mZone267TypeState[tZone267TypeZoneIndex] = 5;
		break;
	case 407:
		CopyMemory(&tZone267TypeZoneIndex, &tInfoData[0], 4);
		mWorldInfo.mZone267TypeState[tZone267TypeZoneIndex] = 5;
		break;
	case 408:
		CopyMemory(&tZone267TypeZoneIndex, &tInfoData[0], 4);
		mWorldInfo.mZone267TypeState[tZone267TypeZoneIndex] = 4;
		break;
	case 409:
		CopyMemory(&tZone267TypeZoneIndex, &tInfoData[0], 4);
		mWorldInfo.mZone267TypeState[tZone267TypeZoneIndex] = 5;
		break;
	case 410:
		CopyMemory(&tZone267TypeZoneIndex, &tInfoData[0], 4);
		mWorldInfo.mZone267TypeState[tZone267TypeZoneIndex] = 0;
		break;
#endif
	case 771: break;
	case 772: break;
	case 773: break;
	case 774:
		CopyMemory(&tTribe, &tInfoData[0], 4);
		CopyMemory(&tZoneNumber, &tInfoData[4], 4);
		CopyMemory(&tZone195LevelType, &tInfoData[8 + MAX_AVATAR_NAME_LENGTH], 4);
		mGAME.mWorldInfo.mProvingGroundsState[tZone195LevelType] = (tTribe + 1);
		mGAME.mWorldInfo.mProvingGroundsPoint[0] = mUTIL.CheckPVNumber(0);
		mGAME.mWorldInfo.mProvingGroundsPoint[1] = mUTIL.CheckPVNumber(1);
		mGAME.mWorldInfo.mProvingGroundsPoint[2] = mUTIL.CheckPVNumber(2);
		mGAME.mWorldInfo.mProvingGroundsPoint[3] = mUTIL.CheckPVNumber(3);
	//mDB.DB_PROCESS_14(tTribe + 1);
		break;
	}
	mTRANSFER.B_BROADCAST_INFO_RECV2(tInfoSort, &tInfoData[0]);
	mUTIL.Broadcast(FALSE, nullptr, 0, false, 2);
	mTRANSFER.B_BROADCAST_WORLD_INFO(&mGAME.mWorldInfo, &mGAME.mTribeInfo);
	for (index01 = 0; index01 < mSERVER.mMAX_USER_NUM; index01++)
	{
		mUSER[index01].Send(TRUE, (char*)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	}
}

//-------------------------------------------------------------------------------------------------
