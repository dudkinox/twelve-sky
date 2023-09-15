//-------------------------------------------------------------------------------------------------
//HEADER
//-------------------------------------------------------------------------------------------------
#include "MainApp.h"
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_TRANSFER
//-------------------------------------------------------------------------------------------------
//INSTANCE
MyTransfer mTRANSFER;
//CREATE
MyTransfer::MyTransfer(void)
{
}

//INIT
BOOL MyTransfer::Init(void)
{
	return TRUE;
}

//FREE
void MyTransfer::Free(void)
{
}

//B_CONNECT_OK
void MyTransfer::B_CONNECT_OK(int tRandomNumber)
{
	mOriginal[0] = 0;
	CopyMemory(&mOriginal[1], &tRandomNumber, 4);
	mOriginalSize = 5;
}

//B_TEMP_REGISTER_RECV
void MyTransfer::B_TEMP_REGISTER_RECV(int tResult)
{
	mOriginal[0] = P_TEMP_REGISTER_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	mOriginalSize = S_TEMP_REGISTER_RECV;
}

//B_REGISTER_AVATAR_RECV
void MyTransfer::B_REGISTER_AVATAR_RECV(AVATAR_INFO* tAvatarInfo, int tEffectValue[MAX_AVATAR_EFFECT_SORT_NUM][2])
// 함수 선언 수정 - int tPremiumServerExpirationDate 추가. // @_Premium_Server_@
{
	mOriginal[0] = P_REGISTER_AVATAR_RECV;
	CopyMemory(&mOriginal[1], tAvatarInfo, SIZE_OF_AVATAR_INFO);
	CopyMemory(&mOriginal[(1 + SIZE_OF_AVATAR_INFO)], tEffectValue, (MAX_AVATAR_EFFECT_SORT_NUM * 8));
	mOriginalSize = S_REGISTER_AVATAR_RECV;
}

//B_BROADCAST_WORLD_INFO
void MyTransfer::B_BROADCAST_WORLD_INFO(WORLD_INFO* tWorldInfo, TRIBE_INFO* tTribeInfo)
{
	mOriginal[0] = P_BROADCAST_WORLD_INFO;
	CopyMemory(&mOriginal[1], tWorldInfo, SIZE_OF_WORLD_INFO);
	CopyMemory(&mOriginal[(1 + SIZE_OF_WORLD_INFO)], tTribeInfo, SIZE_OF_TRIBE_INFO);
	mOriginalSize = S_BROADCAST_WORLD_INFO;
}

void MyTransfer::B_BROADCAST_WORLD_INFO2(WORLD_INFO* tWorldInfo, TRIBE_INFO* tTribeInfo)
{
	mOriginal_C[0] = P_BROADCAST_WORLD_INFO;
	CopyMemory(&mOriginal_C[1], tWorldInfo, SIZE_OF_WORLD_INFO);
	CopyMemory(&mOriginal_C[(1 + SIZE_OF_WORLD_INFO)], tTribeInfo, SIZE_OF_TRIBE_INFO);
	mOriginalSize_C = S_BROADCAST_WORLD_INFO;
}

//B_AUTO_CHECK_ASK_SEND
void MyTransfer::B_AUTO_CHECK_ASK_SEND(int tDataSize, BYTE tData[1000])
{
	mOriginal[0] = P_AUTO_CHECK_ASK_SEND;
	CopyMemory(&mOriginal[1], &tDataSize, 4);
	CopyMemory(&mOriginal[5], &tData[0], 1000);
	mOriginalSize = S_AUTO_CHECK_ASK_SEND;
}

//B_AVATAR_ACTION_RECV
void MyTransfer::B_AVATAR_ACTION_RECV(int tServerIndex, DWORD tUniqueNumber, OBJECT_FOR_AVATAR* tAvatar,
                                      int tCheckChangeActionState)
{
	mAVATAR_OBJECT[tServerIndex].mUpdateTimeForBroadcast = GetTickCount();
	mOriginal[0] = P_AVATAR_ACTION_RECV;
	CopyMemory(&mOriginal[1], &tServerIndex, 4);
	CopyMemory(&mOriginal[5], &tUniqueNumber, 4);
	CopyMemory(&mOriginal[9], tAvatar, SIZE_OF_OBJECT_FOR_AVATAR);
	CopyMemory(&mOriginal[(9 + SIZE_OF_OBJECT_FOR_AVATAR)], &tCheckChangeActionState, 4);
	mOriginalSize = S_AVATAR_ACTION_RECV;
}

void MyTransfer::B_AVATAR_ACTION_RECV2(int tServerIndex, DWORD tUniqueNumber, OBJECT_FOR_AVATAR* tAvatar,
                                       int tCheckChangeActionState)
{
	mAVATAR_OBJECT[tServerIndex].mUpdateTimeForBroadcast = GetTickCount();
	mOriginal2[0] = P_AVATAR_ACTION_RECV;
	CopyMemory(&mOriginal2[1], &tServerIndex, 4);
	CopyMemory(&mOriginal2[5], &tUniqueNumber, 4);
	CopyMemory(&mOriginal2[9], tAvatar, SIZE_OF_OBJECT_FOR_AVATAR);
	CopyMemory(&mOriginal2[(9 + SIZE_OF_OBJECT_FOR_AVATAR)], &tCheckChangeActionState, 4);
	mOriginalSize2 = S_AVATAR_ACTION_RECV;
}

//B_AVATAR_EFFECT_VALUE_INFO
void MyTransfer::B_AVATAR_EFFECT_VALUE_INFO(int tServerIndex, DWORD tUniqueNumber,
                                            int tEffectValue[MAX_AVATAR_EFFECT_SORT_NUM][2],
                                            int tEffectValueState[MAX_AVATAR_EFFECT_SORT_NUM])
{
	mOriginal[0] = P_AVATAR_EFFECT_VALUE_INFO;
	CopyMemory(&mOriginal[1], &tServerIndex, 4);
	CopyMemory(&mOriginal[5], &tUniqueNumber, 4);
	CopyMemory(&mOriginal[9], tEffectValue, (MAX_AVATAR_EFFECT_SORT_NUM * 8));
	CopyMemory(&mOriginal[(9 + MAX_AVATAR_EFFECT_SORT_NUM * 8)], &tEffectValueState[0],
	           (MAX_AVATAR_EFFECT_SORT_NUM * 4));
	mOriginalSize = S_AVATAR_EFFECT_VALUE_INFO;
}

//B_AVATAR_CHANGE_INFO_1
void MyTransfer::B_AVATAR_CHANGE_INFO_1(int tServerIndex, DWORD tUniqueNumber, int tSort, int tValue01, int tValue02,
                                        int tValue03)
{
	mOriginal[0] = P_AVATAR_CHANGE_INFO_1;
	CopyMemory(&mOriginal[1], &tServerIndex, 4);
	CopyMemory(&mOriginal[5], &tUniqueNumber, 4);
	CopyMemory(&mOriginal[9], &tSort, 4);
	CopyMemory(&mOriginal[13], &tValue01, 4);
	CopyMemory(&mOriginal[17], &tValue02, 4);
	CopyMemory(&mOriginal[21], &tValue03, 4);
	mOriginalSize = S_AVATAR_CHANGE_INFO_1;
}

//B_MONSTER_ACTION_RECV
void MyTransfer::B_MONSTER_ACTION_RECV(int tServerIndex, DWORD tUniqueNumber, OBJECT_FOR_MONSTER* tMonster,
                                       int tCheckChangeActionState)
{
	mMONSTER_OBJECT[tServerIndex].mUpdateTimeForBroadcast = GetTickCount();
	mOriginal2[0] = P_MONSTER_ACTION_RECV;
	CopyMemory(&mOriginal2[1], &tServerIndex, 4);
	CopyMemory(&mOriginal2[5], &tUniqueNumber, 4);
	CopyMemory(&mOriginal2[9], tMonster, sizeof(OBJECT_FOR_MONSTER));
	CopyMemory(&mOriginal2[(9 + sizeof(OBJECT_FOR_MONSTER))], &tCheckChangeActionState, 4);
	mOriginalSize2 = S_MONSTER_ACTION_RECV;
}

//B_ITEM_ACTION_RECV
void MyTransfer::B_ITEM_ACTION_RECV(int tServerIndex, DWORD tUniqueNumber, OBJECT_FOR_ITEM* tItem,
                                    int tCheckChangeActionState)
{
	mITEM_OBJECT[tServerIndex].mUpdateTimeForBroadcast = GetTickCount();
	mOriginal[0] = P_ITEM_ACTION_RECV;
	CopyMemory(&mOriginal[1], &tServerIndex, 4);
	CopyMemory(&mOriginal[5], &tUniqueNumber, 4);
	CopyMemory(&mOriginal[9], tItem, sizeof(OBJECT_FOR_ITEM));
	CopyMemory(&mOriginal[(9 + sizeof(OBJECT_FOR_ITEM))], &tCheckChangeActionState, 4);
	mOriginalSize = S_ITEM_ACTION_RECV;
}

void MyTransfer::B_ITEM_ACTION_RECV2(int tServerIndex, DWORD tUniqueNumber, OBJECT_FOR_ITEM* tItem,
                                     int tCheckChangeActionState)
{
	mITEM_OBJECT[tServerIndex].mUpdateTimeForBroadcast = GetTickCount();
	mOriginal2[0] = P_ITEM_ACTION_RECV;
	CopyMemory(&mOriginal2[1], &tServerIndex, 4);
	CopyMemory(&mOriginal2[5], &tUniqueNumber, 4);
	CopyMemory(&mOriginal2[9], tItem, sizeof(OBJECT_FOR_ITEM));
	CopyMemory(&mOriginal2[(9 + sizeof(OBJECT_FOR_ITEM))], &tCheckChangeActionState, 4);
	mOriginalSize2 = S_ITEM_ACTION_RECV;
}

//B_GENERAL_NOTICE_RECV
void MyTransfer::B_GENERAL_NOTICE_RECV(char tContent[MAX_CHAT_CONTENT_LENGTH], int type)
{
	if (type == 1)
	{
		mOriginal_C[0] = P_GENERAL_NOTICE_RECV;
		CopyMemory(&mOriginal_C[1], &tContent[0], MAX_CHAT_CONTENT_LENGTH);
		mOriginalSize_C = S_GENERAL_NOTICE_RECV;
	}
	else if (type == 2)
	{
		mOriginal_R[0] = P_GENERAL_NOTICE_RECV;
		CopyMemory(&mOriginal_R[1], &tContent[0], MAX_CHAT_CONTENT_LENGTH);
		mOriginalSize_R = S_GENERAL_NOTICE_RECV;
	}
	else
	{
		mOriginal[0] = P_GENERAL_NOTICE_RECV;
		CopyMemory(&mOriginal[1], &tContent[0], MAX_CHAT_CONTENT_LENGTH);
		mOriginalSize = S_GENERAL_NOTICE_RECV;
	}
}

//B_PROCESS_ATTACK_RECV
void MyTransfer::B_PROCESS_ATTACK_RECV(ATTACK_FOR_PROTOCOL* tAttackInfo)
{
	mOriginal[0] = P_PROCESS_ATTACK_RECV;
	CopyMemory(&mOriginal[1], tAttackInfo, sizeof(ATTACK_FOR_PROTOCOL));
	mOriginalSize = S_PROCESS_ATTACK_RECV;
}

//B_AVATAR_CHANGE_INFO_2
void MyTransfer::B_AVATAR_CHANGE_INFO_2(int tSort, int tValue)
{
	mOriginal[0] = P_AVATAR_CHANGE_INFO_2;
	CopyMemory(&mOriginal[1], &tSort, 4);
	CopyMemory(&mOriginal[5], &tValue, 4);
	mOriginalSize = S_AVATAR_CHANGE_INFO_2;
}

//B_PROCESS_DATA_RECV
void MyTransfer::B_PROCESS_DATA_RECV(int tResult, int tSort, BYTE tData[MAX_BROADCAST_DATA_SIZE])
{
	mOriginal[0] = P_PROCESS_DATA_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &tSort, 4);
	CopyMemory(&mOriginal[9], &tData[0], MAX_BROADCAST_DATA_SIZE);
	mOriginalSize = S_PROCESS_DATA_RECV;
}

//B_DEMAND_ZONE_SERVER_INFO_2_RESULT
void MyTransfer::B_DEMAND_ZONE_SERVER_INFO_2_RESULT(int tResult, char tIp[16], int tPort)
{
	mOriginal[0] = P_DEMAND_ZONE_SERVER_INFO_2_RESULT;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &tIp[0], 16);
	CopyMemory(&mOriginal[21], &tPort, 4);
	mOriginalSize = S_DEMAND_ZONE_SERVER_INFO_2_RESULT;
}

//B_USE_HOTKEY_ITEM_RECV
void MyTransfer::B_USE_HOTKEY_ITEM_RECV(int tResult, int tPage, int tIndex)
{
	mOriginal[0] = P_USE_HOTKEY_ITEM_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &tPage, 4);
	CopyMemory(&mOriginal[9], &tIndex, 4);
	mOriginalSize = S_USE_HOTKEY_ITEM_RECV;
}

//B_USE_INVENTORY_ITEM_RECV
void MyTransfer::B_USE_INVENTORY_ITEM_RECV(int tResult, int tPage, int tIndex, int tValue)
{
	mOriginal[0] = P_USE_INVENTORY_ITEM_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &tPage, 4);
	CopyMemory(&mOriginal[9], &tIndex, 4);
	CopyMemory(&mOriginal[13], &tValue, 4);
	mOriginalSize = S_USE_INVENTORY_ITEM_RECV;
}

//B_IMPROVE_ITEM_RECV
void MyTransfer::B_IMPROVE_ITEM_RECV(int tResult, int tCost, int iValue)
{
	mOriginal[0] = P_IMPROVE_ITEM_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &tCost, 4);
	CopyMemory(&mOriginal[9], &iValue, 4);
	mOriginalSize = S_IMPROVE_ITEM_RECV;
}

//B_REFINE_ITEM_RECV
void MyTransfer::B_REFINE_ITEM_RECV(int tResult, int tCPCost, int iValue)
{
	mOriginal[0] = P_REFINE_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &tCPCost, 4);
	CopyMemory(&mOriginal[9], &iValue, 4);
	mOriginalSize = S_REFINE_RECV;
}

//B_ADD_ITEM_RECV
void MyTransfer::B_ADD_ITEM_RECV(int tResult, int tCost)
{
	mOriginal[0] = P_ADD_ITEM_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &tCost, 4);
	mOriginalSize = S_ADD_ITEM_RECV;
}

//B_EXCHANGE_ITEM_RECV
void MyTransfer::B_EXCHANGE_ITEM_RECV(int tResult, int tCost, int tValue[6])
{
	mOriginal[0] = P_EXCHANGE_ITEM_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &tCost, 4);
	CopyMemory(&mOriginal[9], &tValue[0], 24);
	mOriginalSize = S_EXCHANGE_ITEM_RECV;
}

//B_HIGH_ITEM_RECV
void MyTransfer::B_HIGH_ITEM_RECV(int tResult, int tCost, int tValue[6])
{
	mOriginal[0] = P_HIGH_ITEM_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &tCost, 4);
	CopyMemory(&mOriginal[9], &tValue[0], 24);
	mOriginalSize = S_HIGH_ITEM_RECV;
}

//B_LOW_ITEM_RECV
void MyTransfer::B_LOW_ITEM_RECV(int tResult, int tCost, int tValue[6])
{
	mOriginal[0] = P_LOW_ITEM_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &tCost, 4);
	CopyMemory(&mOriginal[9], &tValue[0], 24);
	mOriginalSize = S_LOW_ITEM_RECV;
}

//B_MAKE_ITEM_RECV
void MyTransfer::B_MAKE_ITEM_RECV(int tResult, int tValue[6])
{
	mOriginal[0] = P_MAKE_ITEM_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &tValue[0], 24);
	mOriginalSize = S_MAKE_ITEM_RECV;
}

//B_MAKE_SKILL_RECV
void MyTransfer::B_MAKE_SKILL_RECV(int tResult, int tValue[6])
{
	mOriginal[0] = P_MAKE_SKILL_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &tValue[0], 24);
	mOriginalSize = S_MAKE_SKILL_RECV;
}

//B_GEAR_BREAK_RECV
void MyTransfer::B_GEAR_BREAK_RECV(int tResult, int tMoney, int tValue[6])
{
	mOriginal[0] = P_GEAR_BREAK_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &tMoney, 4);
	CopyMemory(&mOriginal[9], &tValue[0], 24);
	mOriginalSize = S_GEAR_BREAK_RECV;
}

//B_START_PSHOP_RECV
void MyTransfer::B_START_PSHOP_RECV(int tResult, PSHOP_INFO* tPShopInfo)
{
	mOriginal[0] = P_START_PSHOP_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], tPShopInfo, sizeof(PSHOP_INFO));
	mOriginalSize = S_START_PSHOP_RECV;
}

//B_END_PSHOP_RECV
void MyTransfer::B_END_PSHOP_RECV(int tResult)
{
	mOriginal[0] = P_END_PSHOP_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	mOriginalSize = S_END_PSHOP_RECV;
}

//B_DEMAND_PSHOP_RECV
void MyTransfer::B_DEMAND_PSHOP_RECV(int tResult, PSHOP_INFO* tPShopInfo)
{
	mOriginal[0] = P_DEMAND_PSHOP_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], tPShopInfo, sizeof(PSHOP_INFO));
	mOriginalSize = S_DEMAND_PSHOP_RECV;
}

//B_PSHOP_ITEM_INFO_RECV
void MyTransfer::B_PSHOP_ITEM_INFO_RECV(DWORD tUniqueNumber, char tAvatarName[MAX_AVATAR_NAME_LENGTH], int tPage,
                                        int tIndex, int tPShopItemInfo[9])
{
	mOriginal[0] = P_PSHOP_ITEM_INFO_RECV;
	CopyMemory(&mOriginal[1], &tUniqueNumber, 4);
	CopyMemory(&mOriginal[5], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
	CopyMemory(&mOriginal[(5 + MAX_AVATAR_NAME_LENGTH)], &tPage, 4);
	CopyMemory(&mOriginal[(9 + MAX_AVATAR_NAME_LENGTH)], &tIndex, 4);
	CopyMemory(&mOriginal[(13 + MAX_AVATAR_NAME_LENGTH)], &tPShopItemInfo[0], 36);
	mOriginalSize = S_PSHOP_ITEM_INFO_RECV;
}

//B_BUY_PSHOP_RECV
void MyTransfer::B_BUY_PSHOP_RECV(int tResult, int tCost, int tPage, int tIndex, int tValue[6], int tSocket)
{
	mOriginal[0] = P_BUY_PSHOP_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &tCost, 4);
	CopyMemory(&mOriginal[9], &tPage, 4);
	CopyMemory(&mOriginal[13], &tIndex, 4);
	CopyMemory(&mOriginal[17], &tValue[0], 24);
	CopyMemory(&mOriginal[41], &tSocket, 4);
	mOriginalSize = S_BUY_PSHOP_RECV;
}

//B_PROCESS_QUEST_RECV
void MyTransfer::B_PROCESS_QUEST_RECV(int tResult, int tSort, int tPage, int tIndex, int tXPost, int tYPost)
{
	mOriginal[0] = P_PROCESS_QUEST_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &tSort, 4);
	CopyMemory(&mOriginal[9], &tPage, 4);
	CopyMemory(&mOriginal[13], &tIndex, 4);
	CopyMemory(&mOriginal[17], &tXPost, 4);
	CopyMemory(&mOriginal[21], &tYPost, 4);
	mOriginalSize = S_PROCESS_QUEST_RECV;
}

//B_CHANGE_TO_TRIBE4_RECV
void MyTransfer::B_CHANGE_TO_TRIBE4_RECV(int tResult)
{
	mOriginal[0] = P_CHANGE_TO_TRIBE4_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	mOriginalSize = S_CHANGE_TO_TRIBE4_RECV;
}

//B_GENERAL_CHAT_RECV
void MyTransfer::B_GENERAL_CHAT_RECV(char tAvatarName[MAX_AVATAR_NAME_LENGTH], BYTE tAuthType,
                                     char tContent[MAX_CHAT_CONTENT_LENGTH])
{
	mOriginal[0] = P_GENERAL_CHAT_RECV;
	CopyMemory(&mOriginal[1], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
	CopyMemory(&mOriginal[(1 + MAX_AVATAR_NAME_LENGTH)], &tContent[0], MAX_CHAT_CONTENT_LENGTH);
	CopyMemory(&mOriginal[1 + MAX_CHAT_CONTENT_LENGTH + MAX_AVATAR_NAME_LENGTH], &tAuthType, 1);
	mOriginalSize = S_GENERAL_CHAT_RECV;
}

//B_SECRET_CHAT_RECV
void MyTransfer::B_SECRET_CHAT_RECV(int tResult, int tZoneNumber, char tAvatarName[MAX_AVATAR_NAME_LENGTH],
                                    char tContent[MAX_CHAT_CONTENT_LENGTH], BYTE tAuthType, int type)
{
	if (type == 0)
	{
		mOriginal[0] = P_SECRET_CHAT_RECV;
		CopyMemory(&mOriginal[1], &tResult, 4);
		CopyMemory(&mOriginal[5], &tZoneNumber, 4);
		CopyMemory(&mOriginal[9], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&mOriginal[(9 + MAX_AVATAR_NAME_LENGTH)], &tContent[0], MAX_CHAT_CONTENT_LENGTH);
		CopyMemory(&mOriginal[(9 + MAX_AVATAR_NAME_LENGTH + MAX_CHAT_CONTENT_LENGTH)], &tAuthType, 1);
		mOriginalSize = S_SECRET_CHAT_RECV;
	}
	else if (type == 1)
	{
		mOriginal_C[0] = P_SECRET_CHAT_RECV;
		CopyMemory(&mOriginal_C[1], &tResult, 4);
		CopyMemory(&mOriginal_C[5], &tZoneNumber, 4);
		CopyMemory(&mOriginal_C[9], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&mOriginal_C[(9 + MAX_AVATAR_NAME_LENGTH)], &tContent[0], MAX_CHAT_CONTENT_LENGTH);
		CopyMemory(&mOriginal_C[(9 + MAX_AVATAR_NAME_LENGTH + MAX_CHAT_CONTENT_LENGTH)], &tAuthType, 1);
		mOriginalSize_C = S_SECRET_CHAT_RECV;
	}
	else if (type == 2)
	{
		mOriginal_R[0] = P_SECRET_CHAT_RECV;
		CopyMemory(&mOriginal_R[1], &tResult, 4);
		CopyMemory(&mOriginal_R[5], &tZoneNumber, 4);
		CopyMemory(&mOriginal_R[9], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&mOriginal_R[(9 + MAX_AVATAR_NAME_LENGTH)], &tContent[0], MAX_CHAT_CONTENT_LENGTH);
		CopyMemory(&mOriginal_R[(9 + MAX_AVATAR_NAME_LENGTH + MAX_CHAT_CONTENT_LENGTH)], &tAuthType, 1);
		mOriginalSize_R = S_SECRET_CHAT_RECV;
	}
}

//B_GENERAL_SHOUT_RECV
void MyTransfer::B_GENERAL_SHOUT_RECV(char tAvatarName[MAX_AVATAR_NAME_LENGTH], BYTE tAuthType,
                                      char tContent[MAX_CHAT_CONTENT_LENGTH])
{
	mOriginal[0] = P_GENERAL_SHOUT_RECV;
	CopyMemory(&mOriginal[1], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
	CopyMemory(&mOriginal[(1 + MAX_AVATAR_NAME_LENGTH)], &tContent[0], MAX_CHAT_CONTENT_LENGTH);
	mOriginalSize = S_GENERAL_SHOUT_RECV;
}

//B_GET_CASH_SIZE_RECV
void MyTransfer::B_GET_CASH_SIZE_RECV(int tCashSize, int tBonusCashSize)
{
	mOriginal[0] = P_GET_CASH_SIZE_RECV;
	CopyMemory(&mOriginal[1], &tCashSize, 4);
	CopyMemory(&mOriginal[5], &tBonusCashSize, 4);
	mOriginalSize = S_GET_CASH_SIZE_RECV;
}

//B_BUY_CASH_ITEM_RECV
void MyTransfer::B_BUY_CASH_ITEM_RECV(int tResult, int tCashSize, int tPage, int tIndex, int tValue[6])
{
	mOriginal[0] = P_BUY_CASH_ITEM_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &tCashSize, 4);
	CopyMemory(&mOriginal[9], &tPage, 4);
	CopyMemory(&mOriginal[13], &tIndex, 4);
	CopyMemory(&mOriginal[17], &tValue[0], 24);
	mOriginalSize = S_BUY_CASH_ITEM_RECV;
}

//B_DUEL_ASK_RECV
void MyTransfer::B_DUEL_ASK_RECV(char tAvatarName[MAX_AVATAR_NAME_LENGTH], int tSort)
{
	mOriginal[0] = P_DUEL_ASK_RECV;
	CopyMemory(&mOriginal[1], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
	CopyMemory(&mOriginal[(1 + MAX_AVATAR_NAME_LENGTH)], &tSort, 4);
	mOriginalSize = S_DUEL_ASK_RECV;
}

//B_DUEL_CANCEL_RECV
void MyTransfer::B_DUEL_CANCEL_RECV(void)
{
	mOriginal[0] = P_DUEL_CANCEL_RECV;
	mOriginalSize = S_DUEL_CANCEL_RECV;
}

//B_DUEL_ANSWER_RECV
void MyTransfer::B_DUEL_ANSWER_RECV(int tAnswer)
{
	mOriginal[0] = P_DUEL_ANSWER_RECV;
	CopyMemory(&mOriginal[1], &tAnswer, 4);
	mOriginalSize = S_DUEL_ANSWER_RECV;
}

//B_DUEL_START_RECV
void MyTransfer::B_DUEL_START_RECV(int tDuelState[3], int tRemainTime)
{
	mOriginal[0] = P_DUEL_START_RECV;
	CopyMemory(&mOriginal[1], &tDuelState[0], 12);
	CopyMemory(&mOriginal[13], &tRemainTime, 4);
	mOriginalSize = S_DUEL_START_RECV;
}

//B_DUEL_TIME_INFO
void MyTransfer::B_DUEL_TIME_INFO(int tRemainTime)
{
	mOriginal[0] = P_DUEL_TIME_INFO;
	CopyMemory(&mOriginal[1], &tRemainTime, 4);
	mOriginalSize = S_DUEL_TIME_INFO;
}

//B_DUEL_END_RECV
void MyTransfer::B_DUEL_END_RECV(int tResult)
{
	mOriginal[0] = P_DUEL_END_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	mOriginalSize = S_DUEL_END_RECV;
}

//B_TRADE_ASK_RECV
void MyTransfer::B_TRADE_ASK_RECV(char tAvatarName[MAX_AVATAR_NAME_LENGTH], int tLevel)
{
	mOriginal[0] = P_TRADE_ASK_RECV;
	CopyMemory(&mOriginal[1], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
	CopyMemory(&mOriginal[(1 + MAX_AVATAR_NAME_LENGTH)], &tLevel, 4);
	mOriginalSize = S_TRADE_ASK_RECV;
}

//B_TRADE_CANCEL_RECV
void MyTransfer::B_TRADE_CANCEL_RECV(void)
{
	mOriginal[0] = P_TRADE_CANCEL_RECV;
	mOriginalSize = S_TRADE_CANCEL_RECV;
}

//B_TRADE_ANSWER_RECV
void MyTransfer::B_TRADE_ANSWER_RECV(int tAnswer)
{
	mOriginal[0] = P_TRADE_ANSWER_RECV;
	CopyMemory(&mOriginal[1], &tAnswer, 4);
	mOriginalSize = S_TRADE_ANSWER_RECV;
}

//B_TRADE_START_RECV
void MyTransfer::B_TRADE_START_RECV(int tTradeMoney, int tTrade[MAX_TRADE_SLOT_NUM][4], int tTradeMoney2,
                                    int tTradeSocket[8])
{
	mOriginal[0] = P_TRADE_START_RECV;
	CopyMemory(&mOriginal[1], &tTradeMoney, 4);
	CopyMemory(&mOriginal[5], tTrade, (MAX_TRADE_SLOT_NUM * 16));
	CopyMemory(&mOriginal[5 + (MAX_TRADE_SLOT_NUM * 16)], tTradeSocket, 32);
	CopyMemory(&mOriginal[5 + (MAX_TRADE_SLOT_NUM * 16) + 32], &tTradeMoney2, 4);
	mOriginalSize = S_TRADE_START_RECV;
}

//B_TRADE_STATE_RECV
void MyTransfer::B_TRADE_STATE_RECV(int tTradeMoney, int tTrade[MAX_TRADE_SLOT_NUM][4], int tTradeMoney2,
                                    int tTradeSocket[8])
{
	mOriginal[0] = P_TRADE_STATE_RECV;
	CopyMemory(&mOriginal[1], &tTradeMoney, 4);
	CopyMemory(&mOriginal[5], tTrade, (MAX_TRADE_SLOT_NUM * 16));
	CopyMemory(&mOriginal[5 + (MAX_TRADE_SLOT_NUM * 16)], tTradeSocket, 32);
	CopyMemory(&mOriginal[5 + (MAX_TRADE_SLOT_NUM * 16) + 32], &tTradeMoney2, 4);
	mOriginalSize = S_TRADE_STATE_RECV;
}

//B_TRADE_MENU_RECV
void MyTransfer::B_TRADE_MENU_RECV(int tCheckMe)
{
	mOriginal[0] = P_TRADE_MENU_RECV;
	CopyMemory(&mOriginal[1], &tCheckMe, 4);
	mOriginalSize = S_TRADE_MENU_RECV;
}

//B_TRADE_END_RECV
void MyTransfer::B_TRADE_END_RECV(int tResult)
{
	mOriginal[0] = P_TRADE_END_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	mOriginalSize = S_TRADE_END_RECV;
}

//B_FRIEND_ASK_RECV
void MyTransfer::B_FRIEND_ASK_RECV(char tAvatarName[MAX_AVATAR_NAME_LENGTH])
{
	mOriginal[0] = P_FRIEND_ASK_RECV;
	CopyMemory(&mOriginal[1], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
	mOriginalSize = S_FRIEND_ASK_RECV;
}

//B_FRIEND_CANCEL_RECV
void MyTransfer::B_FRIEND_CANCEL_RECV(void)
{
	mOriginal[0] = P_FRIEND_CANCEL_RECV;
	mOriginalSize = S_FRIEND_CANCEL_RECV;
}

//B_FRIEND_ANSWER_RECV
void MyTransfer::B_FRIEND_ANSWER_RECV(int tAnswer)
{
	mOriginal[0] = P_FRIEND_ANSWER_RECV;
	CopyMemory(&mOriginal[1], &tAnswer, 4);
	mOriginalSize = S_FRIEND_ANSWER_RECV;
}

//B_FRIEND_MAKE_RECV
void MyTransfer::B_FRIEND_MAKE_RECV(int tIndex, char tAvatarName[MAX_AVATAR_NAME_LENGTH])
{
	mOriginal[0] = P_FRIEND_MAKE_RECV;
	CopyMemory(&mOriginal[1], &tIndex, 4);
	CopyMemory(&mOriginal[5], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
	mOriginalSize = S_FRIEND_MAKE_RECV;
}

//B_FRIEND_FIND_RECV
void MyTransfer::B_FRIEND_FIND_RECV(int tIndex, int tZoneNumber)
{
	mOriginal[0] = P_FRIEND_FIND_RECV;
	CopyMemory(&mOriginal[1], &tIndex, 4);
	CopyMemory(&mOriginal[5], &tZoneNumber, 4);
	mOriginalSize = S_FRIEND_FIND_RECV;
}

//B_FRIEND_DELETE_RECV
void MyTransfer::B_FRIEND_DELETE_RECV(int tIndex)
{
	mOriginal[0] = P_FRIEND_DELETE_RECV;
	CopyMemory(&mOriginal[1], &tIndex, 4);
	mOriginalSize = S_FRIEND_DELETE_RECV;
}

//B_TEACHER_ASK_RECV
void MyTransfer::B_TEACHER_ASK_RECV(char tAvatarName[MAX_AVATAR_NAME_LENGTH])
{
	mOriginal[0] = P_TEACHER_ASK_RECV;
	CopyMemory(&mOriginal[1], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
	mOriginalSize = S_TEACHER_ASK_RECV;
}

//B_TEACHER_CANCEL_RECV
void MyTransfer::B_TEACHER_CANCEL_RECV(void)
{
	mOriginal[0] = P_TEACHER_CANCEL_RECV;
	mOriginalSize = S_TEACHER_CANCEL_RECV;
}

//B_TEACHER_ANSWER_RECV
void MyTransfer::B_TEACHER_ANSWER_RECV(int tAnswer)
{
	mOriginal[0] = P_TEACHER_ANSWER_RECV;
	CopyMemory(&mOriginal[1], &tAnswer, 4);
	mOriginalSize = S_TEACHER_ANSWER_RECV;
}

//B_TEACHER_START_RECV
void MyTransfer::B_TEACHER_START_RECV(int tSort, char tAvatarName[MAX_AVATAR_NAME_LENGTH])
{
	mOriginal[0] = P_TEACHER_START_RECV;
	CopyMemory(&mOriginal[1], &tSort, 4);
	CopyMemory(&mOriginal[5], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
	mOriginalSize = S_TEACHER_START_RECV;
}

//B_TEACHER_END_RECV
void MyTransfer::B_TEACHER_END_RECV(void)
{
	mOriginal[0] = P_TEACHER_END_RECV;
	mOriginalSize = S_TEACHER_END_RECV;
}

//B_TEACHER_STATE_RECV
void MyTransfer::B_TEACHER_STATE_RECV(int tResult)
{
	mOriginal[0] = P_TEACHER_STATE_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	mOriginalSize = S_TEACHER_STATE_RECV;
}

//B_PARTY_ASK_RECV
void MyTransfer::B_PARTY_ASK_RECV(char tAvatarName[MAX_AVATAR_NAME_LENGTH])
{
	mOriginal[0] = P_PARTY_ASK_RECV;
	CopyMemory(&mOriginal[1], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
	mOriginalSize = S_PARTY_ASK_RECV;
}

//B_PARTY_CANCEL_RECV
void MyTransfer::B_PARTY_CANCEL_RECV(void)
{
	mOriginal[0] = P_PARTY_CANCEL_RECV;
	mOriginalSize = S_PARTY_CANCEL_RECV;
}

//B_PARTY_ANSWER_RECV
void MyTransfer::B_PARTY_ANSWER_RECV(int tAnswer)
{
	mOriginal[0] = P_PARTY_ANSWER_RECV;
	CopyMemory(&mOriginal[1], &tAnswer, 4);
	mOriginalSize = S_PARTY_ANSWER_RECV;
}

//B_PARTY_MAKE_RECV
void MyTransfer::B_PARTY_MAKE_RECV(int tResult, char tAvatarName01[MAX_AVATAR_NAME_LENGTH],
                                   char tAvatarName02[MAX_AVATAR_NAME_LENGTH],
                                   char tAvatarName03[MAX_AVATAR_NAME_LENGTH],
                                   char tAvatarName04[MAX_AVATAR_NAME_LENGTH],
                                   char tAvatarName05[MAX_AVATAR_NAME_LENGTH])
{
	mOriginal[0] = P_PARTY_MAKE_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &tAvatarName01[0], MAX_AVATAR_NAME_LENGTH);
	CopyMemory(&mOriginal[5 + MAX_AVATAR_NAME_LENGTH ], &tAvatarName02[0], MAX_AVATAR_NAME_LENGTH);
	CopyMemory(&mOriginal[5 + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH], &tAvatarName03[0],
	           MAX_AVATAR_NAME_LENGTH);
	CopyMemory(&mOriginal[5 + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH],
	           &tAvatarName04[0], MAX_AVATAR_NAME_LENGTH);
	CopyMemory(
		&mOriginal[5 + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH
		], &tAvatarName05[0], MAX_AVATAR_NAME_LENGTH);
	mOriginalSize = S_PARTY_MAKE_RECV;
}

//B_PARTY_JOIN_RECV
void MyTransfer::B_PARTY_JOIN_RECV(char tAvatarName[MAX_AVATAR_NAME_LENGTH], int type)
{
	if (type == 0)
	{
		mOriginal[0] = P_PARTY_JOIN_RECV;
		CopyMemory(&mOriginal[1], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		mOriginalSize = S_PARTY_JOIN_RECV;
	}
	else if (type == 1)
	{
		mOriginal_C[0] = P_PARTY_JOIN_RECV;
		CopyMemory(&mOriginal_C[1], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		mOriginalSize_C = S_PARTY_JOIN_RECV;
	}
	else if (type == 2)
	{
		mOriginal_R[0] = P_PARTY_JOIN_RECV;
		CopyMemory(&mOriginal_R[1], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		mOriginalSize_R = S_PARTY_JOIN_RECV;
	}
}

//B_PARTY_CHAT_RECV
void MyTransfer::B_PARTY_CHAT_RECV(char tAvatarName[MAX_AVATAR_NAME_LENGTH], char tContent[MAX_CHAT_CONTENT_LENGTH],
                                   int type)
{
	if (type == 0)
	{
		mOriginal[0] = P_PARTY_CHAT_RECV;
		CopyMemory(&mOriginal[1], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&mOriginal[(1 + MAX_AVATAR_NAME_LENGTH)], &tContent[0], MAX_CHAT_CONTENT_LENGTH);
		mOriginalSize = S_PARTY_CHAT_RECV;
	}
	else if (type == 1)
	{
		mOriginal_C[0] = P_PARTY_CHAT_RECV;
		CopyMemory(&mOriginal_C[1], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&mOriginal_C[(1 + MAX_AVATAR_NAME_LENGTH)], &tContent[0], MAX_CHAT_CONTENT_LENGTH);
		mOriginalSize_C = S_PARTY_CHAT_RECV;
	}
	else if (type == 2)
	{
		mOriginal_R[0] = P_PARTY_CHAT_RECV;
		CopyMemory(&mOriginal_R[1], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&mOriginal_R[(1 + MAX_AVATAR_NAME_LENGTH)], &tContent[0], MAX_CHAT_CONTENT_LENGTH);
		mOriginalSize_R = S_PARTY_CHAT_RECV;
	}
}

//B_PARTY_LEAVE_RECV
void MyTransfer::B_PARTY_LEAVE_RECV(char tAvatarName[MAX_AVATAR_NAME_LENGTH], int type)
{
	if (type == 0)
	{
		mOriginal[0] = P_PARTY_LEAVE_RECV;
		CopyMemory(&mOriginal[1], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		mOriginalSize = S_PARTY_LEAVE_RECV;
	}
	else if (type == 1)
	{
		mOriginal_C[0] = P_PARTY_LEAVE_RECV;
		CopyMemory(&mOriginal_C[1], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		mOriginalSize_C = S_PARTY_LEAVE_RECV;
	}
	else if (type == 2)
	{
		mOriginal_R[0] = P_PARTY_LEAVE_RECV;
		CopyMemory(&mOriginal_R[1], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		mOriginalSize_R = S_PARTY_LEAVE_RECV;
	}
}

//B_PARTY_EXILE_RECV
void MyTransfer::B_PARTY_EXILE_RECV(char tAvatarName[MAX_AVATAR_NAME_LENGTH], int type)
{
	if (type == 0)
	{
		mOriginal[0] = P_PARTY_EXILE_RECV;
		CopyMemory(&mOriginal[1], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		mOriginalSize = S_PARTY_EXILE_RECV;
	}
	else if (type == 1)
	{
		mOriginal_C[0] = P_PARTY_EXILE_RECV;
		CopyMemory(&mOriginal_C[1], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		mOriginalSize_C = S_PARTY_EXILE_RECV;
	}
	else if (type == 2)
	{
		mOriginal_R[0] = P_PARTY_EXILE_RECV;
		CopyMemory(&mOriginal_R[1], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		mOriginalSize_R = S_PARTY_EXILE_RECV;
	}
}

//B_PARTY_INFO_RECV
void MyTransfer::B_PARTY_INFO_RECV(char tAvatarName01[MAX_AVATAR_NAME_LENGTH],
                                   char tAvatarName02[MAX_AVATAR_NAME_LENGTH],
                                   char tAvatarName03[MAX_AVATAR_NAME_LENGTH],
                                   char tAvatarName04[MAX_AVATAR_NAME_LENGTH],
                                   char tAvatarName05[MAX_AVATAR_NAME_LENGTH], int type)
{
	if (type == 0)
	{
		mOriginal[0] = P_PARTY_INFO_RECV;
		CopyMemory(&mOriginal[1], &tAvatarName01[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&mOriginal[(1 + MAX_AVATAR_NAME_LENGTH)], &tAvatarName02[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&mOriginal[(1 + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH)], &tAvatarName03[0],
		           MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&mOriginal[(1 + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH)],
		           &tAvatarName04[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(
			&mOriginal[(1 + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH +
				MAX_AVATAR_NAME_LENGTH)], &tAvatarName05[0], MAX_AVATAR_NAME_LENGTH);
		mOriginalSize = S_PARTY_INFO_RECV;
	}
	else if (type == 1)
	{
		mOriginal_C[0] = P_PARTY_INFO_RECV;
		CopyMemory(&mOriginal_C[1], &tAvatarName01[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&mOriginal_C[(1 + MAX_AVATAR_NAME_LENGTH)], &tAvatarName02[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&mOriginal_C[(1 + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH)], &tAvatarName03[0],
		           MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&mOriginal_C[(1 + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH)],
		           &tAvatarName04[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(
			&mOriginal_C[(1 + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH +
				MAX_AVATAR_NAME_LENGTH)], &tAvatarName05[0], MAX_AVATAR_NAME_LENGTH);
		mOriginalSize_C = S_PARTY_INFO_RECV;
	}
	else if (type == 2)
	{
		mOriginal_R[0] = P_PARTY_INFO_RECV;
		CopyMemory(&mOriginal_R[1], &tAvatarName01[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&mOriginal_R[(1 + MAX_AVATAR_NAME_LENGTH)], &tAvatarName02[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&mOriginal_R[(1 + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH)], &tAvatarName03[0],
		           MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&mOriginal_R[(1 + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH)],
		           &tAvatarName04[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(
			&mOriginal_R[(1 + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH + MAX_AVATAR_NAME_LENGTH +
				MAX_AVATAR_NAME_LENGTH)], &tAvatarName05[0], MAX_AVATAR_NAME_LENGTH);
		mOriginalSize_R = S_PARTY_INFO_RECV;
	}
}

//B_PARTY_BREAK_RECV
void MyTransfer::B_PARTY_BREAK_RECV(int tResult, char tAvatarName[MAX_AVATAR_NAME_LENGTH], int type)
{
	if (type == 0)
	{
		mOriginal[0] = P_PARTY_BREAK_RECV;
		CopyMemory(&mOriginal[1], &tResult, 4);
		CopyMemory(&mOriginal[5], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		mOriginalSize = S_PARTY_BREAK_RECV;
	}
	else if (type == 1)
	{
		mOriginal_C[0] = P_PARTY_BREAK_RECV;
		CopyMemory(&mOriginal_C[1], &tResult, 4);
		CopyMemory(&mOriginal_C[5], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		mOriginalSize_C = S_PARTY_BREAK_RECV;
	}
	else if (type == 2)
	{
		mOriginal_R[0] = P_PARTY_BREAK_RECV;
		CopyMemory(&mOriginal_R[1], &tResult, 4);
		CopyMemory(&mOriginal_R[5], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		mOriginalSize_R = S_PARTY_BREAK_RECV;
	}
}

//B_GUILD_ASK_RECV
void MyTransfer::B_GUILD_ASK_RECV(char tAvatarName[MAX_AVATAR_NAME_LENGTH])
{
	mOriginal[0] = P_GUILD_ASK_RECV;
	CopyMemory(&mOriginal[1], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
	mOriginalSize = S_GUILD_ASK_RECV;
}

//B_GUILD_CANCEL_RECV
void MyTransfer::B_GUILD_CANCEL_RECV(void)
{
	mOriginal[0] = P_GUILD_CANCEL_RECV;
	mOriginalSize = S_GUILD_CANCEL_RECV;
}

//B_GUILD_ANSWER_RECV
void MyTransfer::B_GUILD_ANSWER_RECV(int tAnswer)
{
	mOriginal[0] = P_GUILD_ANSWER_RECV;
	CopyMemory(&mOriginal[1], &tAnswer, 4);
	mOriginalSize = S_GUILD_ANSWER_RECV;
}

//B_GUILD_WORK_RECV
void MyTransfer::B_GUILD_WORK_RECV(int tResult, int tSort, GUILD_INFO* tGuildInfo)
{
	mOriginal[0] = P_GUILD_WORK_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &tSort, 4);
	CopyMemory(&mOriginal[9], tGuildInfo, sizeof(GUILD_INFO));
	mOriginalSize = S_GUILD_WORK_RECV;
}

//B_GUILD_NOTICE_RECV
void MyTransfer::B_GUILD_NOTICE_RECV(char tAvatarName[MAX_AVATAR_NAME_LENGTH], char tContent[MAX_CHAT_CONTENT_LENGTH],
                                     int type)
{
	if (type == 0)
	{
		mOriginal[0] = P_GUILD_NOTICE_RECV;
		CopyMemory(&mOriginal[1], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&mOriginal[(1 + MAX_AVATAR_NAME_LENGTH)], &tContent[0], MAX_CHAT_CONTENT_LENGTH);
		mOriginalSize = S_GUILD_NOTICE_RECV;
	}
	else if (type == 1)
	{
		mOriginal_C[0] = P_GUILD_NOTICE_RECV;
		CopyMemory(&mOriginal_C[1], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&mOriginal_C[(1 + MAX_AVATAR_NAME_LENGTH)], &tContent[0], MAX_CHAT_CONTENT_LENGTH);
		mOriginalSize_C = S_GUILD_NOTICE_RECV;
	}
	else if (type == 2)
	{
		mOriginal_R[0] = P_GUILD_NOTICE_RECV;
		CopyMemory(&mOriginal_R[1], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&mOriginal_R[(1 + MAX_AVATAR_NAME_LENGTH)], &tContent[0], MAX_CHAT_CONTENT_LENGTH);
		mOriginalSize_R = S_GUILD_NOTICE_RECV;
	}
}

//B_GUILD_CHAT_RECV
void MyTransfer::B_GUILD_CHAT_RECV(char tAvatarName[MAX_AVATAR_NAME_LENGTH], char tContent[MAX_CHAT_CONTENT_LENGTH],
                                   int type)
{
	if (type == 0)
	{
		mOriginal[0] = P_GUILD_CHAT_RECV;
		CopyMemory(&mOriginal[1], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&mOriginal[(1 + MAX_AVATAR_NAME_LENGTH)], &tContent[0], MAX_CHAT_CONTENT_LENGTH);
		mOriginalSize = S_GUILD_CHAT_RECV;
	}
	else if (type == 1)
	{
		mOriginal_C[0] = P_GUILD_CHAT_RECV;
		CopyMemory(&mOriginal_C[1], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&mOriginal_C[(1 + MAX_AVATAR_NAME_LENGTH)], &tContent[0], MAX_CHAT_CONTENT_LENGTH);
		mOriginalSize_C = S_GUILD_CHAT_RECV;
	}
	else if (type == 2)
	{
		mOriginal_R[0] = P_GUILD_CHAT_RECV;
		CopyMemory(&mOriginal_R[1], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&mOriginal_R[(1 + MAX_AVATAR_NAME_LENGTH)], &tContent[0], MAX_CHAT_CONTENT_LENGTH);
		mOriginalSize_R = S_GUILD_CHAT_RECV;
	}
}

//B_GUILD_FIND_RECV
void MyTransfer::B_GUILD_FIND_RECV(int tResult)
{
	mOriginal[0] = P_GUILD_FIND_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	mOriginalSize = S_GUILD_FIND_RECV;
}

//B_GUILD_LOGIN_INFO
void MyTransfer::B_GUILD_LOGIN_INFO(char tAvatarName[MAX_AVATAR_NAME_LENGTH], int type)
{
	if (type == 0)
	{
		mOriginal[0] = P_GUILD_LOGIN_INFO;
		CopyMemory(&mOriginal[1], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		mOriginalSize = S_GUILD_LOGIN_INFO;
	}
	else if (type == 1)
	{
		mOriginal_C[0] = P_GUILD_LOGIN_INFO;
		CopyMemory(&mOriginal_C[1], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		mOriginalSize_C = S_GUILD_LOGIN_INFO;
	}
	else if (type == 2)
	{
		mOriginal_R[0] = P_GUILD_LOGIN_INFO;
		CopyMemory(&mOriginal_R[1], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		mOriginalSize_R = S_GUILD_LOGIN_INFO;
	}
}

//B_TRIBE_WORK_RECV
void MyTransfer::B_TRIBE_WORK_RECV(int tResult, int tSort, BYTE tData[MAX_TRIBE_WORK_SIZE])
{
	mOriginal[0] = P_TRIBE_WORK_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &tSort, 4);
	CopyMemory(&mOriginal[9], &tData[0], MAX_TRIBE_WORK_SIZE);
	mOriginalSize = S_TRIBE_WORK_RECV;
}

//B_TRIBE_NOTICE_RECV
void MyTransfer::B_TRIBE_NOTICE_RECV(int tType, char tAvatarName[MAX_AVATAR_NAME_LENGTH],
                                     char tContent[MAX_CHAT_CONTENT_LENGTH], int type)
{
	if (type == 0)
	{
		mOriginal[0] = P_TRIBE_NOTICE_RECV;
		CopyMemory(&mOriginal[1], &tType, 4);
		CopyMemory(&mOriginal[5], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&mOriginal[(5 + MAX_AVATAR_NAME_LENGTH)], &tContent[0], MAX_CHAT_CONTENT_LENGTH);
		mOriginalSize = S_TRIBE_NOTICE_RECV;
	}
	else if (type == 1)
	{
		mOriginal_C[0] = P_TRIBE_NOTICE_RECV;
		CopyMemory(&mOriginal_C[1], &tType, 4);
		CopyMemory(&mOriginal_C[5], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&mOriginal_C[(5 + MAX_AVATAR_NAME_LENGTH)], &tContent[0], MAX_CHAT_CONTENT_LENGTH);
		mOriginalSize_C = S_TRIBE_NOTICE_RECV;
	}
	else if (type == 2)
	{
		mOriginal_R[0] = P_TRIBE_NOTICE_RECV;
		CopyMemory(&mOriginal_R[1], &tType, 4);
		CopyMemory(&mOriginal_R[5], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&mOriginal_R[(5 + MAX_AVATAR_NAME_LENGTH)], &tContent[0], MAX_CHAT_CONTENT_LENGTH);
		mOriginalSize_R = S_TRIBE_NOTICE_RECV;
	}
}

//B_TRIBE_CHAT_RECV
void MyTransfer::B_TRIBE_CHAT_RECV(char tAvatarName[MAX_AVATAR_NAME_LENGTH], BYTE tAuthType,
                                   char tContent[MAX_CHAT_CONTENT_LENGTH])
{
	mOriginal[0] = P_TRIBE_CHAT_RECV;
	CopyMemory(&mOriginal[1], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
	CopyMemory(&mOriginal[1 + MAX_AVATAR_NAME_LENGTH + MAX_CHAT_CONTENT_LENGTH], &tAuthType, 1);
	CopyMemory(&mOriginal[(1 + MAX_AVATAR_NAME_LENGTH)], &tContent[0], MAX_CHAT_CONTENT_LENGTH);
	mOriginalSize = S_TRIBE_CHAT_RECV;
}

void MyTransfer::B_TRIBE_CHAT_RECV2(char tAvatarName[MAX_AVATAR_NAME_LENGTH], char tContent[MAX_CHAT_CONTENT_LENGTH])
{
	BYTE tAuthType = 1;
	mOriginal_C[0] = P_TRIBE_CHAT_RECV;
	CopyMemory(&mOriginal_C[1], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
	CopyMemory(&mOriginal_C[1 + MAX_AVATAR_NAME_LENGTH], &tAuthType, 1);
	CopyMemory(&mOriginal_C[(2 + MAX_AVATAR_NAME_LENGTH)], &tContent[0], MAX_CHAT_CONTENT_LENGTH);
	mOriginalSize_C = S_TRIBE_CHAT_RECV;
}

//B_FACTION_NOTICE_RECV
void MyTransfer::B_FACTION_NOTICE_RECV(char tAvatarName[MAX_AVATAR_NAME_LENGTH], BYTE tAuthType,
                                       char tContent[MAX_CHAT_CONTENT_LENGTH], int type)
{
	if (type == 0)
	{
		mOriginal[0] = P_FACTION_NOTICE_RECV;
		CopyMemory(&mOriginal[1], &tAuthType, 4);
		CopyMemory(&mOriginal[1 + 4], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&mOriginal[(1 + 4 + MAX_AVATAR_NAME_LENGTH)], &tContent[0], MAX_CHAT_CONTENT_LENGTH);
		mOriginalSize = S_FACTION_NOTICE_RECV;
	}
	if (type == 1)
	{
		mOriginal_C[0] = P_FACTION_NOTICE_RECV;
		CopyMemory(&mOriginal_C[1], &tAuthType, 4);
		CopyMemory(&mOriginal_C[1 + 4], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&mOriginal_C[(1 + 4 + MAX_AVATAR_NAME_LENGTH)], &tContent[0], MAX_CHAT_CONTENT_LENGTH);
		mOriginalSize_C = S_FACTION_NOTICE_RECV;
	}
	if (type == 2)
	{
		mOriginal_R[0] = P_FACTION_NOTICE_RECV;
		CopyMemory(&mOriginal_R[1], &tAuthType, 4);
		CopyMemory(&mOriginal_R[1 + 4], &tAvatarName[0], MAX_AVATAR_NAME_LENGTH);
		CopyMemory(&mOriginal_R[(1 + 4 + MAX_AVATAR_NAME_LENGTH)], &tContent[0], MAX_CHAT_CONTENT_LENGTH);
		mOriginalSize_R = S_FACTION_NOTICE_RECV;
	}
}

//B_TRIBE_BANK_RECV
void MyTransfer::B_TRIBE_BANK_RECV(int tResult, int tSort, int tTribeBankInfo[MAX_TRIBE_BANK_SLOT_NUM], int tMoney)
{
	mOriginal[0] = P_TRIBE_BANK_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &tSort, 4);
	CopyMemory(&mOriginal[9], &tTribeBankInfo[0], (MAX_TRIBE_BANK_SLOT_NUM * 4));
	CopyMemory(&mOriginal[(9 + MAX_TRIBE_BANK_SLOT_NUM * 4)], &tMoney, 4);
	mOriginalSize = S_TRIBE_BANK_RECV;
}

//B_TRIBE_VOTE_RECV
void MyTransfer::B_TRIBE_VOTE_RECV(int tResult, int tSort, int tValue)
{
	mOriginal[0] = P_TRIBE_VOTE_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &tSort, 4);
	CopyMemory(&mOriginal[9], &tValue, 4);
	mOriginalSize = S_TRIBE_VOTE_RECV;
}

//B_TRIBE_ALLIANCE_INFO
void MyTransfer::B_TRIBE_ALLIANCE_INFO(int tSort, int tValue)
{
	mOriginal[0] = P_TRIBE_ALLIANCE_INFO;
	CopyMemory(&mOriginal[1], &tSort, 4);
	CopyMemory(&mOriginal[5], &tValue, 4);
	mOriginalSize = S_TRIBE_ALLIANCE_INFO;
}

//B_BROADCAST_INFO_RECV
void MyTransfer::B_BROADCAST_INFO_RECV(int tInfoSort, BYTE tInfoData[MAX_BROADCAST_INFO_SIZE])
{
	mOriginal[0] = P_BROADCAST_INFO_RECV;
	CopyMemory(&mOriginal[1], &tInfoSort, 4);
	CopyMemory(&mOriginal[5], &tInfoData[0], MAX_BROADCAST_INFO_SIZE);
	mOriginalSize = S_BROADCAST_INFO_RECV;
}

void MyTransfer::B_BROADCAST_INFO_RECV2(int tInfoSort, BYTE tInfoData[MAX_BROADCAST_INFO_SIZE])
{
	mOriginal_C[0] = P_BROADCAST_INFO_RECV;
	CopyMemory(&mOriginal_C[1], &tInfoSort, 4);
	CopyMemory(&mOriginal_C[5], &tInfoData[0], MAX_BROADCAST_INFO_SIZE);
	mOriginalSize_C = S_BROADCAST_INFO_RECV;
}

//B_049_TYPE_BATTLE_INFO
void MyTransfer::B_049_TYPE_BATTLE_INFO(int tTribeUserNum[4], int tRemainTime)
{
	mOriginal[0] = P_049_TYPE_BATTLE_INFO;
	CopyMemory(&mOriginal[1], &tTribeUserNum[0], 16);
	CopyMemory(&mOriginal[17], &tRemainTime, 4);
	mOriginalSize = S_049_TYPE_BATTLE_INFO;
}

//B_051_TYPE_BATTLE_INFO
void MyTransfer::B_051_TYPE_BATTLE_INFO(int tExistStone[4], int tRemainTime)
{
	mOriginal[0] = P_051_TYPE_BATTLE_INFO;
	CopyMemory(&mOriginal[1], &tExistStone[0], 16);
	CopyMemory(&mOriginal[17], &tRemainTime, 4);
	mOriginalSize = S_051_TYPE_BATTLE_INFO;
}

//B_053_TYPE_BATTLE_INFO
void MyTransfer::B_053_TYPE_BATTLE_INFO(int tRemainTime)
{
	mOriginal[0] = P_053_TYPE_BATTLE_INFO;
	CopyMemory(&mOriginal[1], &tRemainTime, 4);
	mOriginalSize = S_053_TYPE_BATTLE_INFO;
}

//B_GM_COMMAND_INFO
void MyTransfer::B_GM_COMMAND_INFO(int tSort, BYTE tData[MAX_TRIBE_WORK_SIZE])
{
	mOriginal[0] = P_GM_COMMAND_INFO;
	CopyMemory(&mOriginal[1], &tSort, 4);
	CopyMemory(&mOriginal[5], &tData[0], MAX_TRIBE_WORK_SIZE);
	mOriginalSize = S_GM_COMMAND_INFO;
}

void MyTransfer::B_GM_COMMAND_INFO2(int tSort, BYTE tData[MAX_TRIBE_WORK_SIZE])
{
	mOriginal_C[0] = P_GM_COMMAND_INFO;
	CopyMemory(&mOriginal_C[1], &tSort, 4);
	CopyMemory(&mOriginal_C[5], &tData[0], MAX_TRIBE_WORK_SIZE);
	mOriginalSize_C = S_GM_COMMAND_INFO;
}

//B_RETURN_TO_AUTO_ZONE
void MyTransfer::B_RETURN_TO_AUTO_ZONE(void)
{
	mOriginal[0] = P_RETURN_TO_AUTO_ZONE;
	mOriginalSize = S_RETURN_TO_AUTO_ZONE;
}

//B_CHECK_N_PROTECT_1
#ifdef __N_PROTECT__
void MyTransfer::B_CHECK_N_PROTECT_1(GG_AUTH_DATA* tValue)
{
	mOriginal[0] = P_CHECK_N_PROTECT_1;
	CopyMemory(&mOriginal[1], tValue, sizeof(GG_AUTH_DATA));
	mOriginalSize = S_CHECK_N_PROTECT_1;
}
#endif

void MyTransfer::B_CHECK_AUTH_KEY(BYTE bResult)
{
	ZTOC_CHECK_AUTH_KEY pack;
	pack.m_bResult = bResult;
	::CopyMemory(mOriginal, &pack, ZTOC_CHECK_AUTH_KEY_SIZE);
	mOriginalSize = ZTOC_CHECK_AUTH_KEY_SIZE;
}

void MyTransfer::B_UPDATE_AUTH_KEY(BYTE bResult)
{
	ZTOC_UPDATE_AUTH_KEY pack;
	pack.m_bResult = bResult;
	::CopyMemory(mOriginal, &pack, ZTOC_UPDATE_AUTH_KEY_SIZE);
	mOriginalSize = ZTOC_UPDATE_AUTH_KEY_SIZE;
}

void MyTransfer::B_REQUEST_PRESENT(BYTE bResult, int iPage, int iInvenSlot, int iInvenIndex, int iItemIndex)
{
	ZTOC_REQUEST_PRESENT pack;
	pack.m_bResult = bResult;
	pack.m_bPage = static_cast<BYTE>(iPage);
	pack.m_bInvenIndex = static_cast<BYTE>(iInvenIndex);
	pack.m_iInvenSlot = iInvenSlot;
	pack.m_iItemIndex = iItemIndex;
	::CopyMemory(mOriginal, &pack, ZTOC_REQUEST_PRESENT_SIZE);
	mOriginalSize = ZTOC_REQUEST_PRESENT_SIZE;
}

void MyTransfer::B_CHANGE_PLAYTIMETOTEACHERPOINT(BYTE bResult, int iAddTeacherPoints)
{
	ZTOC_CHANGE_PLAYTIMETOTEACHERPOINT pack;
	pack.m_bResult = bResult;
	pack.m_iAddTeacherPoints = iAddTeacherPoints;
	::CopyMemory(mOriginal, &pack, ZTOC_CHANGE_PLAYTIMETOTEACHERPOINT_SIZE);
	mOriginalSize = ZTOC_CHANGE_PLAYTIMETOTEACHERPOINT_SIZE;
}

void MyTransfer::B_CHECK_EVENT_INFO(void)
{
	mOriginal[0] = P_CHECK_EVENT_INFO_SEND;
	CopyMemory(&mOriginal[1], &mGAME.mGeneralExperienceUpRatio, 4);
	CopyMemory(&mOriginal[5], &mGAME.mItemDropUpRatio, 4);
	mOriginalSize = S_CHECK_EVENT_INFO_SEND;
}


void MyTransfer::DEMAND_OBJECT_RECV(int tSort, int tValue)
{
	mOriginal[0] = P_DEMAND_OBJECT_RECV;

	CopyMemory(&mOriginal[1], &tSort, 4);
	CopyMemory(&mOriginal[5], &tValue, 4);
	mOriginalSize = S_DEMAND_OBJECT_RECV;
}

void MyTransfer::B_GET_RANK_INFO(const char* aName, int tUserIndex)
{
	//LOG_TO_FILE_2("aName : %s %d\n", aName, tUserIndex);
	//mCENTER_COM.W_CENTER_GET_RANK_INFO(aName, tUserIndex);

	mTRANSFER.mOriginal[0] = P_ZTOP_RANK_INFO_SEND;

	if (mGAME.mRankInfoSize > 0)
	{
		memset(&mTRANSFER.mOriginal[1], 0, sizeof(rank_info_t));
		memcpy(&mTRANSFER.mOriginal[1 + sizeof(rank_info_t)]
		       , mGAME.mRankInfo, mGAME.mRankInfoSize);
		mTRANSFER.mOriginalSize = 1 + sizeof(rank_info_t) + mGAME.mRankInfoSize;
	}
	else
	{
		memset(&mTRANSFER.mOriginal[1 + sizeof(rank_info_t)], 0, 4);
		memset(&mTRANSFER.mOriginal[1 + sizeof(rank_info_t) + 4], 0, 4);
		memset(&mTRANSFER.mOriginal[1 + sizeof(rank_info_t) + 8], 0, 4);
		mTRANSFER.mOriginalSize = 1 + sizeof(rank_info_t) + 12;
	}
}
#ifdef __GOD__
//B_194_BATTLE_INFO
void MyTransfer::B_194_BATTLE_INFO(int tTribeUserNum[4])
{
	mOriginal[0] = P_194_BATTLE_INFO;
	CopyMemory(&mOriginal[1], &tTribeUserNum[0], 16);
	mOriginalSize = S_194_BATTLE_INFO;
}

//B_194_BATTLE_COUNTDOWN
void MyTransfer::B_194_BATTLE_COUNTDOWN(int tTime)
{
	mOriginal[0] = P_194_BATTLE_COUNTDOWN;
	CopyMemory(&mOriginal[1], &tTime, 4);
	mOriginalSize = S_194_BATTLE_COUNTDOWN;
}

//B_267_TYPE_BATTLE_INFO
void MyTransfer::B_267_TYPE_BATTLE_INFO(int tTribeUserNum[4], int tRemainTime)
{
	mOriginal[0] = P_267_TYPE_BATTLE_INFO;
	CopyMemory(&mOriginal[1], &tTribeUserNum[0], 16);
	CopyMemory(&mOriginal[17], &tRemainTime, 4);
	mOriginalSize = S_267_TYPE_BATTLE_INFO;
}
#endif

#ifdef __ANIMAL__
void MyTransfer::B_ANIMAL_STATE_RECV(int tValue01, int tValue02)
{
	mOriginal[0] = P_ANIMAL_STATE_RECV;
	CopyMemory(&mOriginal[1], &tValue01, 4);
	CopyMemory(&mOriginal[5], &tValue02, 4);
	mOriginalSize = S_ANIMAL_STATE_RECV;
}
#endif


void MyTransfer::B_ABSORB_STATE_RECV(int tResult, int tValue01, int tValue02)
{
	mOriginal[0] = P_ABSORB_STATE_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &tValue01, 4);
	CopyMemory(&mOriginal[9], &tValue02, 4);
	mOriginalSize = S_ABSORB_STATE_RECV;
}

void MyTransfer::B_SOCKET_ITEM_RECV(int tResult, int tSort, int tSocket)
{
	mOriginal[0] = P_SOCKET_ITEM_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &tSort, 4);
	CopyMemory(&mOriginal[9], &tSocket, 4);
	mOriginalSize = S_SOCKET_ITEM_RECV;
}


void MyTransfer::B_COSTUME_STATE_RECV(int tResult, int tValue01, int tValue02, int iPage, int iInvenSlot,
                                      int iInvenIndex, int pItemIndex)
{
	mOriginal[0] = P_COSTUME_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &tValue01, 4);
	CopyMemory(&mOriginal[9], &tValue02, 4);
	CopyMemory(&mOriginal[13], &iPage, 4);
	CopyMemory(&mOriginal[17], &iInvenSlot, 4);
	CopyMemory(&mOriginal[21], &iInvenIndex, 4);
	CopyMemory(&mOriginal[25], &pItemIndex, 4);
	mOriginalSize = S_COSTUME_RECV;
}

void MyTransfer::B_AUTOHUNT_RECV(int tServerIndex, DWORD tUniqueNumber, int tAutoState)
{
	mOriginal[0] = P_AUTOHUNT_RECV;
	CopyMemory(&mOriginal[1], &tServerIndex, 4);
	CopyMemory(&mOriginal[5], &tUniqueNumber, 4);
	CopyMemory(&mOriginal[9], &tAutoState, 4);
	mOriginalSize = S_AUTOHUNT_RECV;
}

void MyTransfer::B_DAILY_RECV(int tResult, int tWars, int tPVPKills, int tPVMKills, int tOnlineTime)
{
	mOriginal[0] = P_DAILY_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	memset(&mOriginal[5], 0, 4);
	CopyMemory(&mOriginal[9], &tWars, 4);
	CopyMemory(&mOriginal[13], &tPVPKills, 4);
	CopyMemory(&mOriginal[17], &tPVMKills, 4);
	CopyMemory(&mOriginal[21], &tOnlineTime, 4);
	mOriginalSize = S_DAILY_RECV;
}

void MyTransfer::B_DAILY_GET_REWARD(int tResult, int iPage, int iInvenSlot, int iInvenIndex, int pItem)
{
	mOriginal[0] = P_DAILY_GET_REWARD;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &iPage, 4);
	CopyMemory(&mOriginal[9], &iInvenSlot, 4);
	memset(&mOriginal[13], 0, 4);
	CopyMemory(&mOriginal[17], &iInvenIndex, 4);
	memset(&mOriginal[21], 0, 4);
	CopyMemory(&mOriginal[25], &pItem, 4);
	mOriginalSize = S_DAILY_GET_REWARD;
}

void MyTransfer::B_WARLORD_V2_RECV(int tResult, int tCost, int tValue[6])
{
	mOriginal[0] = P_WARLORD_V2_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &tCost, 4);
	CopyMemory(&mOriginal[9], &tValue[0], 24);
	mOriginalSize = S_WARLORD_V2_RECV;
}

void MyTransfer::B_REMAIN_TIME_RECV(int tRemainTime)
{
	mOriginal[0] = P_REMAIN_TIME_RECV;
	CopyMemory(&mOriginal[1], &tRemainTime, 4);
	mOriginalSize = S_REMAIN_TIME_RECV;
}

void MyTransfer::B_ITEM_LIKE_1336_RECV(int* tCount, int* tPage, int* tInvenSlot, int* tInvenIndex, int (*tItemList)[2],
                                       int* tItemValue, int* tItemRecognitionNum)
{
	int index01 = 0;
	mOriginal[0] = P_ITEM_LIKE_1336_RECV;
	CopyMemory(&mOriginal[1], tCount, 4);
	for (index01 = 0; index01 < *tCount; ++index01)
	{
		CopyMemory(&mOriginal[5 + (index01 * 28)], &tPage[index01], 4);
		CopyMemory(&mOriginal[9 + (index01 * 28)], &tInvenSlot[index01], 4);
		CopyMemory(&mOriginal[13 + (index01 * 28)], &tInvenIndex[index01], 4);
		CopyMemory(&mOriginal[17 + (index01 * 28)], &tItemList[index01][0], 4);
		CopyMemory(&mOriginal[21 + (index01 * 28)], &tItemList[index01][1], 4);
		CopyMemory(&mOriginal[25 + (index01 * 28)], &tItemValue[index01], 4);
		CopyMemory(&mOriginal[29 + (index01 * 28)], &tItemRecognitionNum[index01], 4);
	}
	mOriginalSize = S_ITEM_LIKE_1336_RECV;
}

void MyTransfer::B_MEDAL_EXCHANGE_RECV(int tResult, int tPage, int tSlot, int tIndex, int tValue)
{
	mOriginal[0] = P_MEDAL_EXCHANGE_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &tPage, 4);
	CopyMemory(&mOriginal[9], &tSlot, 4);
	CopyMemory(&mOriginal[13], &tIndex, 4);
	CopyMemory(&mOriginal[17], &tValue, 4);
	mOriginalSize = S_MEDAL_EXCHANGE_RECV;
}

void MyTransfer::B_SEND_QUESTION(int iCount, BYTE* tContents)
{
	mOriginal[0] = P_QUESTION_OF_SPHINX;
	::CopyMemory(&mOriginal[1], &iCount, 4);
	::CopyMemory(&mOriginal[1 + 4], &tContents[0], 4140);
	mOriginalSize = S_QUESTION_OF_SPHINX;
}

void MyTransfer::B_CORRECT_ANSWER_RESULT(int tSort)
{
	mOriginal[0] = P_CORRECT_ANSWER_RESULT;
	::CopyMemory(&mOriginal[1], &tSort, 4);
	mOriginalSize = S_CORRECT_ANSWER_RESULT;
}

// @ Cost Info Download @
void MyTransfer::B_GET_CASH_ITEM_INFO_RECV(int tResult, int tVersion, const int tCashItemInfo[4][14][10][4])
{
	mOriginal[0] = P_GET_CASH_ITEM_INFO_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &tVersion, 4);
	CopyMemory(&mOriginal[9], tCashItemInfo, 8960);
	mOriginalSize = S_GET_CASH_ITEM_INFO_RECV;
}

void MyTransfer::B_UPDATE_CASH_ITEM_INFO()
{
	mOriginal[0] = P_UPDATE_CASH_ITEM_INFO;
	mOriginalSize = S_UPDATE_CASH_ITEM_INFO;
} // @

// # Defense Hack # {
void MyTransfer::B_SPEED_CHECK(unsigned int tValue)
{
	mOriginal[0] = P_SPEED_CHECK_RECV;
	::CopyMemory(&mOriginal[1], &tValue, 4);
	mOriginalSize = S_SPEED_CHECK_RECV;
} // # }

// # X-Trap # {
int MyTransfer::B_XTRAP_STEP_1(char tXTrapCheckBuf[XTRAP_STEP_1_BUF_SIZE])
{
	mOriginal[0] = P_XTRAP_STEP_1;
	::CopyMemory(&mOriginal[1], &tXTrapCheckBuf[0], XTRAP_STEP_1_BUF_SIZE);
	mOriginalSize = S_XTRAP_STEP_1;
	return 0;
} // # }

/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */
void MyTransfer::B_TOGGLE_STOPWATCH(BYTE bResult, BYTE bKind, BYTE bUse)
{
	ZTOC_TOGGLE_STOPWATCH pack;

	pack.m_bResult = bResult;
	pack.m_bKind = bKind;
	pack.m_bUse = bUse;

	::CopyMemory(mOriginal, &pack, ZTOC_TOGGLE_STOPWATCH_TYPESIZE);
	mOriginalSize = ZTOC_TOGGLE_STOPWATCH_TYPESIZE;
}

/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */

//-------------------------------------------------------------------------------------------------
