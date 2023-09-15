//-------------------------------------------------------------------------------------------------
//MY_TRANSFER_H
//-------------------------------------------------------------------------------------------------
#ifndef MY_TRANSFER_H
#define MY_TRANSFER_H
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_TRANSFER
//-------------------------------------------------------------------------------------------------
//MACRO
#define MAX_TRANSFER_BUFFER_SIZE									50000

//DEFINE
class MyTransfer
{
public:
	DWORD mOriginalSize;
	BYTE mOriginal[MAX_TRANSFER_BUFFER_SIZE];
	DWORD mOriginalSize2;
	BYTE mOriginal2[MAX_TRANSFER_BUFFER_SIZE];
	DWORD mOriginalSize_C;
	BYTE mOriginal_C[MAX_TRANSFER_BUFFER_SIZE];
	DWORD mOriginalSize_R;
	BYTE mOriginal_R[MAX_TRANSFER_BUFFER_SIZE];

	MyTransfer(void);

	BOOL Init(void);
	void Free(void);

	void B_CONNECT_OK(int tRandomNumber);
	void B_TEMP_REGISTER_RECV(int tResult);
	void B_REGISTER_AVATAR_RECV(AVATAR_INFO* tAvatarInfo, int tEffectValue[MAX_AVATAR_EFFECT_SORT_NUM][2]);
	void B_BROADCAST_WORLD_INFO(WORLD_INFO* tWorldInfo, TRIBE_INFO* tTribeInfo);
	void B_BROADCAST_WORLD_INFO2(WORLD_INFO* tWorldInfo, TRIBE_INFO* tTribeInfo);
	void B_AUTO_CHECK_ASK_SEND(int tDataSize, BYTE tData[1000]);
	void B_AVATAR_ACTION_RECV(int tServerIndex, DWORD tUniqueNumber, OBJECT_FOR_AVATAR* tAvatar,
	                          int tCheckChangeActionState);
	void B_AVATAR_ACTION_RECV2(int tServerIndex, DWORD tUniqueNumber, OBJECT_FOR_AVATAR* tAvatar,
	                           int tCheckChangeActionState);
	void B_AVATAR_EFFECT_VALUE_INFO(int tServerIndex, DWORD tUniqueNumber,
	                                int tEffectValue[MAX_AVATAR_EFFECT_SORT_NUM][2],
	                                int tEffectValueState[MAX_AVATAR_EFFECT_SORT_NUM]);
	void B_AVATAR_CHANGE_INFO_1(int tServerIndex, DWORD tUniqueNumber, int tSort, int tValue01, int tValue02,
	                            int tValue03);
	void B_MONSTER_ACTION_RECV(int tServerIndex, DWORD tUniqueNumber, OBJECT_FOR_MONSTER* tMonster,
	                           int tCheckChangeActionState);
	void B_ITEM_ACTION_RECV(int tServerIndex, DWORD tUniqueNumber, OBJECT_FOR_ITEM* tItem, int tCheckChangeActionState);
	void B_ITEM_ACTION_RECV2(int tServerIndex, DWORD tUniqueNumber, OBJECT_FOR_ITEM* tItem,
	                         int tCheckChangeActionState);
	void B_GENERAL_NOTICE_RECV(char tContent[MAX_CHAT_CONTENT_LENGTH], int type = 0);
	void B_PROCESS_ATTACK_RECV(ATTACK_FOR_PROTOCOL* tAttackInfo);
	void B_AVATAR_CHANGE_INFO_2(int tSort, int tValue);
	void B_PROCESS_DATA_RECV(int tResult, int tSort, BYTE tData[MAX_BROADCAST_DATA_SIZE]);
	void B_DEMAND_ZONE_SERVER_INFO_2_RESULT(int tResult, char tIp[16], int tPort);
	void B_USE_HOTKEY_ITEM_RECV(int tResult, int tPage, int tIndex);
	void B_USE_INVENTORY_ITEM_RECV(int tResult, int tPage, int tIndex, int tValue);
	void B_IMPROVE_ITEM_RECV(int tResult, int tCost, int iValue);
	void B_REFINE_ITEM_RECV(int tResult, int tCPCost, int iValue);
	void DEMAND_OBJECT_RECV(int tSort, int tValue);
	//void B_HALO_RFC_RECV(int tResult, int tCost);
	void B_ADD_ITEM_RECV(int tResult, int tCost);
	void B_EXCHANGE_ITEM_RECV(int tResult, int tCost, int tValue[6]);
	void B_HIGH_ITEM_RECV(int tResult, int tCost, int tValue[6]);
	void B_LOW_ITEM_RECV(int tResult, int tCost, int tValue[6]);
	void B_MAKE_ITEM_RECV(int tResult, int tValue[6]);
	void B_GEAR_BREAK_RECV(int tResult, int tMoney, int tValue[6]);
	void B_MAKE_SKILL_RECV(int tResult, int tValue[6]);
	void B_START_PSHOP_RECV(int tResult, PSHOP_INFO* tPShopInfo);
	void B_END_PSHOP_RECV(int tResult);
	void B_DEMAND_PSHOP_RECV(int tResult, PSHOP_INFO* tPShopInfo);
	void B_PSHOP_ITEM_INFO_RECV(DWORD tUniqueNumber, char tAvatarName[MAX_AVATAR_NAME_LENGTH], int tPage, int tIndex,
	                            int tPShopItemInfo[9]);
	void B_BUY_PSHOP_RECV(int tResult, int tCost, int tPage, int tIndex, int tValue[6], int tSocket);
	void B_PROCESS_QUEST_RECV(int tResult, int tSort, int tPage, int tIndex, int tXPost, int tYPost);
	void B_CHANGE_TO_TRIBE4_RECV(int tResult);
	void B_GENERAL_CHAT_RECV(char tAvatarName[MAX_AVATAR_NAME_LENGTH], BYTE tAuthType,
	                         char tContent[MAX_CHAT_CONTENT_LENGTH]);
	void B_SECRET_CHAT_RECV(int tResult, int tZoneNumber, char tAvatarName[MAX_AVATAR_NAME_LENGTH],
	                        char tContent[MAX_CHAT_CONTENT_LENGTH], BYTE tAuthType, int type = 0);
	void B_GENERAL_SHOUT_RECV(char tAvatarName[MAX_AVATAR_NAME_LENGTH], BYTE tAuthType,
	                          char tContent[MAX_CHAT_CONTENT_LENGTH]);
	void B_GET_CASH_SIZE_RECV(int tCashSize, int tBonusCashSize);
	void B_BUY_CASH_ITEM_RECV(int tResult, int tCashSize, int tPage, int tIndex, int tValue[6]);
	void B_DUEL_ASK_RECV(char tAvatarName[MAX_AVATAR_NAME_LENGTH], int tSort);
	void B_DUEL_CANCEL_RECV(void);
	void B_DUEL_ANSWER_RECV(int tAnswer);
	void B_DUEL_START_RECV(int tDuelState[3], int tRemainTime);
	void B_DUEL_TIME_INFO(int tRemainTime);
	void B_DUEL_END_RECV(int tResult);
	void B_TRADE_ASK_RECV(char tAvatarName[MAX_AVATAR_NAME_LENGTH], int tLevel);
	void B_TRADE_CANCEL_RECV(void);
	void B_TRADE_ANSWER_RECV(int tAnswer);
	void B_TRADE_START_RECV(int tTradeMoney, int tTrade[MAX_TRADE_SLOT_NUM][4], int tTradeMoney2, int tTradeSocket[8]);
	void B_TRADE_STATE_RECV(int tTradeMoney, int tTrade[MAX_TRADE_SLOT_NUM][4], int tTradeMoney2, int tTradeSocket[8]);
	void B_TRADE_MENU_RECV(int tCheckMe);
	void B_TRADE_END_RECV(int tResult);
	void B_FRIEND_ASK_RECV(char tAvatarName[MAX_AVATAR_NAME_LENGTH]);
	void B_FRIEND_CANCEL_RECV(void);
	void B_FRIEND_ANSWER_RECV(int tAnswer);
	void B_FRIEND_MAKE_RECV(int tIndex, char tAvatarName[MAX_AVATAR_NAME_LENGTH]);
	void B_FRIEND_FIND_RECV(int tIndex, int tZoneNumber);
	void B_FRIEND_DELETE_RECV(int tIndex);
	void B_TEACHER_ASK_RECV(char tAvatarName[MAX_AVATAR_NAME_LENGTH]);
	void B_TEACHER_CANCEL_RECV(void);
	void B_TEACHER_ANSWER_RECV(int tAnswer);
	void B_TEACHER_START_RECV(int tSort, char tAvatarName[MAX_AVATAR_NAME_LENGTH]);
	void B_TEACHER_END_RECV(void);
	void B_TEACHER_STATE_RECV(int tResult);
	void B_PARTY_ASK_RECV(char tAvatarName[MAX_AVATAR_NAME_LENGTH]);
	void B_PARTY_CANCEL_RECV(void);
	void B_PARTY_ANSWER_RECV(int tAnswer);
	void B_PARTY_MAKE_RECV(int tResult, char tAvatarName01[MAX_AVATAR_NAME_LENGTH],
	                       char tAvatarName02[MAX_AVATAR_NAME_LENGTH], char tAvatarName03[MAX_AVATAR_NAME_LENGTH],
	                       char tAvatarName04[MAX_AVATAR_NAME_LENGTH], char tAvatarName05[MAX_AVATAR_NAME_LENGTH]);
	void B_PARTY_JOIN_RECV(char tAvatarName[MAX_AVATAR_NAME_LENGTH], int type = 0);
	void B_PARTY_CHAT_RECV(char tAvatarName[MAX_AVATAR_NAME_LENGTH], char tContent[MAX_CHAT_CONTENT_LENGTH],
	                       int type = 0);
	void B_PARTY_LEAVE_RECV(char tAvatarName[MAX_AVATAR_NAME_LENGTH], int type = 0);
	void B_PARTY_EXILE_RECV(char tAvatarName[MAX_AVATAR_NAME_LENGTH], int type = 0);
	void B_PARTY_INFO_RECV(char tAvatarName01[MAX_AVATAR_NAME_LENGTH], char tAvatarName02[MAX_AVATAR_NAME_LENGTH],
	                       char tAvatarName03[MAX_AVATAR_NAME_LENGTH], char tAvatarName04[MAX_AVATAR_NAME_LENGTH],
	                       char tAvatarName05[MAX_AVATAR_NAME_LENGTH], int type = 0);
	void B_PARTY_BREAK_RECV(int tResult, char tAvatarName[MAX_AVATAR_NAME_LENGTH], int type);
	void B_GUILD_ASK_RECV(char tAvatarName[MAX_AVATAR_NAME_LENGTH]);
	void B_GUILD_CANCEL_RECV(void);
	void B_GUILD_ANSWER_RECV(int tAnswer);
	void B_GUILD_WORK_RECV(int tResult, int tSort, GUILD_INFO* tGuildInfo);
	void B_GUILD_NOTICE_RECV(char tAvatarName[MAX_AVATAR_NAME_LENGTH], char tContent[MAX_CHAT_CONTENT_LENGTH],
	                         int type = 0);
	void B_GUILD_CHAT_RECV(char tAvatarName[MAX_AVATAR_NAME_LENGTH], char tContent[MAX_CHAT_CONTENT_LENGTH],
	                       int type = 0);
	void B_GUILD_FIND_RECV(int tResult);
	void B_GUILD_LOGIN_INFO(char tAvatarName[MAX_AVATAR_NAME_LENGTH], int type = 0);
	void B_TRIBE_WORK_RECV(int tResult, int tSort, BYTE tData[MAX_TRIBE_WORK_SIZE]);
	void B_TRIBE_NOTICE_RECV(int tType, char tAvatarName[MAX_AVATAR_NAME_LENGTH],
	                         char tContent[MAX_CHAT_CONTENT_LENGTH], int type = 0);
	void B_TRIBE_CHAT_RECV(char tAvatarName[MAX_AVATAR_NAME_LENGTH], BYTE tAuthType,
	                       char tContent[MAX_CHAT_CONTENT_LENGTH]);
	void B_TRIBE_CHAT_RECV2(char tAvatarName[MAX_AVATAR_NAME_LENGTH], char tContent[MAX_CHAT_CONTENT_LENGTH]);
	void B_FACTION_NOTICE_RECV(char tAvatarName[MAX_AVATAR_NAME_LENGTH], BYTE tAuthType,
	                           char tContent[MAX_CHAT_CONTENT_LENGTH], int type = 0);
	void B_TRIBE_BANK_RECV(int tResult, int tSort, int tTribeBankInfo[MAX_TRIBE_BANK_SLOT_NUM], int tMoney);
	void B_TRIBE_VOTE_RECV(int tResult, int tSort, int tValue);
	void B_TRIBE_ALLIANCE_INFO(int tSort, int tValue);
	void B_BROADCAST_INFO_RECV(int tInfoSort, BYTE tInfoData[MAX_BROADCAST_INFO_SIZE]);
	void B_BROADCAST_INFO_RECV2(int tInfoSort, BYTE tInfoData[MAX_BROADCAST_INFO_SIZE]);
	void B_049_TYPE_BATTLE_INFO(int tTribeUserNum[4], int tRemainTime);
	void B_051_TYPE_BATTLE_INFO(int tExistStone[4], int tRemainTime);
	void B_053_TYPE_BATTLE_INFO(int tRemainTime);
#ifdef __GOD__
	void B_194_BATTLE_INFO(int tTribeUserNum[4]);
	void B_194_BATTLE_COUNTDOWN(int tTime);
	void B_267_TYPE_BATTLE_INFO(int tTribeUserNum[4], int tRemainTime);
#endif

	void B_GM_COMMAND_INFO(int tSort, BYTE tData[MAX_TRIBE_WORK_SIZE]);
	void B_GM_COMMAND_INFO2(int tSort, BYTE tData[MAX_TRIBE_WORK_SIZE]);
	void B_RETURN_TO_AUTO_ZONE(void);
#ifdef __N_PROTECT__
	void B_CHECK_N_PROTECT_1(GG_AUTH_DATA* tValue);
#endif
	void B_CHECK_AUTH_KEY(BYTE bResult);
	void B_UPDATE_AUTH_KEY(BYTE bResult);
	void B_REQUEST_PRESENT(BYTE bResult, int iPage, int iInvenSlot, int iInvenIndex, int iItemIndex);
	void B_CHANGE_PLAYTIMETOTEACHERPOINT(BYTE bResult, int iAddTeacherPoints);
	void B_CHECK_EVENT_INFO(void);
	void B_GET_RANK_INFO(const char* aName, int tUserIndex);

#ifdef __ANIMAL__
	void B_ANIMAL_STATE_RECV(int tValue01, int tValue02);
#endif
	void B_ABSORB_STATE_RECV(int tResult, int tValue01, int tValue02);
	void B_SOCKET_ITEM_RECV(int tResult, int tSort, int tSocket);
	void B_COSTUME_STATE_RECV(int tResult, int tValue01, int tValue02, int iPage, int iInvenSlot, int iInvenIndex,
	                          int pItemIndex);
	void B_AUTOHUNT_RECV(int tServerIndex, DWORD tUniqueNumber, int tAutoState);
	void B_DAILY_RECV(int tResult, int tWars, int tPVPKills, int tPVMKills, int tOnlineTime);
	void B_DAILY_GET_REWARD(int tResult, int iPage, int iInvenSlot, int iInvenIndex, int pItem);
	void B_WARLORD_V2_RECV(int tResult, int tCost, int tValue[6]);
	void B_REMAIN_TIME_RECV(int tRemainTime);
	void B_ITEM_LIKE_1336_RECV(int* tCount, int* tPage, int* tInvenSlot, int* tInvenIndex, int (*tItemList)[2],
	                           int* tItemValue, int* tItemRecognitionNum);
	void B_MEDAL_EXCHANGE_RECV(int tResult, int tPage, int tSlot, int tIndex, int tValue);
	//@{ auto quiz
	void B_SEND_QUESTION(int iCount, BYTE* tContents);
	void B_CORRECT_ANSWER_RESULT(int tSort);
	//@}

	// @ Cost Info Download @
	void B_GET_CASH_ITEM_INFO_RECV(int tResult, int tVersion, const int tCashItemInfo[4][14][10][4]);
	void B_UPDATE_CASH_ITEM_INFO();
	// @

	// # Defense Hack # {
	void B_SPEED_CHECK(unsigned int tValue);
	// # }	

	// # X-Trap # {
	int B_XTRAP_STEP_1(char userSessionBuf[XTRAP_STEP_1_BUF_SIZE]);
	// # }

	/** 2009. 11. 18 : Àå½Ä ÇØ¹æ ½Ã½ºÅÛ (wolf) */
	void B_TOGGLE_STOPWATCH(BYTE bKind, BYTE bResult, BYTE bUse);
	/** 2009. 11. 18 : Àå½Ä ÇØ¹æ ½Ã½ºÅÛ (wolf) */
};

//INSTANCE
extern MyTransfer mTRANSFER;
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------------
