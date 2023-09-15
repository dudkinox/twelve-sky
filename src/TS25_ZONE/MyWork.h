#ifndef MY_WORK_H
#define MY_WORK_H

using WORKER_FUNCTION = struct
{
	void (*PROC)(int tUserIndex);
};
extern WORKER_FUNCTION W_FUNCTION[256];
//W_TEMP_REGISTER_SEND
void W_TEMP_REGISTER_SEND(int tUserIndex);
//W_REGISTER_AVATAR_SEND
void W_REGISTER_AVATAR_SEND(int tUserIndex);
//W_CLIENT_OK_FOR_ZONE_SEND
void W_CLIENT_OK_FOR_ZONE_SEND(int tUserIndex);
//W_AVATAR_ACTION_SEND
void W_AVATAR_ACTION_SEND(int tUserIndex);
//W_UPDATE_AVATAR_ACTION
void W_UPDATE_AVATAR_ACTION(int tUserIndex);
void W_CHANGE_AUTO_INFO(int tUserIndex);
//W_USE_HOTKEY_ITEM_SEND
void W_USE_HOTKEY_ITEM_SEND(int tUserIndex);
//W_USE_INVENTORY_ITEM_SEND
void W_USE_INVENTORY_ITEM_SEND(int tUserIndex);
//W_TRIBE_BANK_SEND
void W_TRIBE_BANK_SEND(int tUserIndex);
//W_GENERAL_CHAT_SEND
void W_GENERAL_CHAT_SEND(int tUserIndex);
//W_SECRET_CHAT_SEND
void W_SECRET_CHAT_SEND(int tUserIndex);
//W_TRIBE_CHAT_SEND
void W_TRIBE_CHAT_SEND(int tUserIndex);
//W_FACTION_NOTICE_SEND
void W_FACTION_NOTICE_SEND(int tUserIndex);
//W_PROCESS_DATA_SEND
void W_PROCESS_DATA_SEND(int tUserIndex);
//W_IMPROVE_ITEM_SEND
void W_IMPROVE_ITEM_SEND(int tUserIndex); //W_TRADE_ASK_SEND
//W_ADD_ITEM_SEND
void W_ADD_ITEM_SEND(int tUserIndex);
//TRADE_ASK
void W_TRADE_ASK_SEND(int tUserIndex);
//W_TRADE_CANCEL_SEND
void W_TRADE_CANCEL_SEND(int tUserIndex);
//W_TRADE_ANSWER_SEND
void W_TRADE_ANSWER_SEND(int tUserIndex);
//W_TRADE_START_SEND
void W_TRADE_START_SEND(int tUserIndex);
//W_TRADE_MENU_SEND
void W_TRADE_MENU_SEND(int tUserIndex);
//W_TRADE_END_SEND
void W_TRADE_END_SEND(int tUserIndex);
//W_PROCESS_ATTACK_SEND
void W_PROCESS_ATTACK_SEND(int tUserIndex);
//W_ANIMAL_STATE_SEND
void W_ANIMAL_STATE_SEND(int tUserIndex);
//W_ANIMAL_STATE_SEND
void W_ABSORB_STATE_SEND(int tUserIndex);
//W_DEMAND_ZONE_SERVER_INFO_2
void W_DEMAND_ZONE_SERVER_INFO_2(int tUserIndex);
//W_FAIL_MOVE_ZONE_2_SEND
void W_FAIL_MOVE_ZONE_2_SEND(int tUserIndex);
//W_START_PSHOP_SEND
void W_START_PSHOP_SEND(int tUserIndex);
//W_END_PSHOP_SEND
void W_END_PSHOP_SEND(int tUserIndex);
//W_DEMAND_PSHOP_SEND
void W_DEMAND_PSHOP_SEND(int tUserIndex);
//W_PSHOP_ITEM_INFO_SEND
void W_PSHOP_ITEM_INFO_SEND(int tUserIndex);
//W_BUY_PSHOP_SEND
void W_BUY_PSHOP_SEND(int tUserIndex);
//W_COSTUME
void W_COSTUME_STATE_RECV(int tUserIndex);
//W_TRIBE_WORK_SEND
void W_TRIBE_WORK_SEND(int tUserIndex);
//W_GET_CASH_SIZE_SEND
void W_GET_CASH_SIZE_SEND(int tUserIndex);
//W_BUY_CASH_ITEM_SEND
void W_BUY_CASH_ITEM_SEND(int tUserIndex);
//W_GET_CASH_ITEM_INFO_SEND
void W_GET_CASH_ITEM_INFO_SEND(int tUserIndex);
//W_FRIEND_ASK_SEND
void W_FRIEND_ASK_SEND(int tUserIndex);
//W_FRIEND_CANCEL_SEND
void W_FRIEND_CANCEL_SEND(int tUserIndex);
//W_FRIEND_ANSWER_SEND
void W_FRIEND_ANSWER_SEND(int tUserIndex);
//W_FRIEND_MAKE_SEND
void W_FRIEND_MAKE_SEND(int tUserIndex);
//W_FRIEND_FIND_SEND
void W_FRIEND_FIND_SEND(int tUserIndex);
//W_FRIEND_DELETE_SEND
void W_FRIEND_DELETE_SEND(int tUserIndex);
//W_DUEL_ASK_SEND
void W_DUEL_ASK_SEND(int tUserIndex);
//W_DUEL_CANCEL_SEND
void W_DUEL_CANCEL_SEND(int tUserIndex);
//W_DUEL_ANSWER_SEND
void W_DUEL_ANSWER_SEND(int tUserIndex);
//W_DUEL_START_SEND
void W_DUEL_START_SEND(int tUserIndex);
//W_MAKE_ITEM_SEND
void W_MAKE_ITEM_SEND(int tUserIndex);


void W_CHANGE_PLAYTIMETOTEACHERPOINT(int tUserIndex);

void W_CHECK_EVENT_INFO(int tUserIndex);
//W_TRIBE_VOTE_SEND
void W_TRIBE_VOTE_SEND(int tUserIndex);


void W_AUTO_CONFIG_SEND(int tUserIndex);
void W_AUTOBUFF_REGISTER_SEND(int tUserIndex);
void W_AUTOBUFF_SEND(int tUserIndex);
//W_PET_COMBINE_SEND
void W_PET_COMBINE_SEND(int tUserIndex);
//W_GEAR_BREAK_SEND
void W_GEAR_BREAK_SEND(int tUserIndex);

//W_CHAR_RANK_BUFF_SEND
void W_CHAR_RANK_BUFF_SEND(int tUserIndex);

//W_TRIBE_NOTICE_SEND
void W_TRIBE_NOTICE_SEND(int tUserIndex);
//W_GENERAL_NOTICE_SEND
void W_GENERAL_NOTICE_SEND(int tUserIndex);

//W_GUILD_ASK_SEND
void W_GUILD_ASK_SEND(int tUserIndex);
//W_GUILD_CANCEL_SEND
void W_GUILD_CANCEL_SEND(int tUserIndex);
//W_GUILD_ANSWER_SEND
void W_GUILD_ANSWER_SEND(int tUserIndex);
//W_GUILD_WORK_SEND
void W_GUILD_WORK_SEND(int tUserIndex);
//W_GUILD_NOTICE_SEND
void W_GUILD_NOTICE_SEND(int tUserIndex);
//W_GUILD_CHAT_SEND
void W_GUILD_CHAT_SEND(int tUserIndex);
//W_GUILD_FIND_SEND
void W_GUILD_FIND_SEND(int tUserIndex);

void W_NPC_FUNCTION_SEND(int tUserIndex);
//W_PARTY_ASK_SEND
void W_PARTY_ASK_SEND(int tUserIndex);
void W_PARTY_CANCEL_SEND(int tUserIndex);
void W_PARTY_ANSWER_SEND(int tUserIndex);
void W_PARTY_EXILE_SEND(int tUserIndex);
void W_PARTY_BREAK_SEND(int tUserIndex);
void W_PARTY_CHAT_SEND(int tUserIndex);
void W_PARTY_LEAVE_SEND(int tUserIndex);
//W_REFINE_SEND
void W_REFINE_SEND(int tUserIndex);
//W_HIGH_ITEM_SEND
void W_HIGH_ITEM_SEND(int tUserIndex);
//W_LOW_ITEM_SEND
void W_LOW_ITEM_SEND(int tUserIndex);
//W_TIME_EFFECT_SEND
void W_TIME_EFFECT_SEND(int tUserIndex);
//W_DAILY_MISSION_SEND
void W_DAILY_MISSION_SEND(int tUserIndex);
//W_EXCHANGE_ITEM_SEND
void W_EXCHANGE_ITEM_SEND(int tUserIndex);
//W_WARLORD_V2_SEND
void W_WARLORD_V2_SEND(int tUserIndex);
//W_GENERAL_SHOUT_SEND
void W_GENERAL_SHOUT_SEND(int tUserIndex);
//W_SOCKET_ITEM_SEND
void W_SOCKET_ITEM_SEND(int tUserIndex);
//W_TEACHER_ASK_SEND
void W_TEACHER_ASK_SEND(int tUserIndex);
//W_TEACHER_ANSWER_SEND
void W_TEACHER_ANSWER_SEND(int tUserIndex);
//W_TEACHER_CANCEL_SEND
void W_TEACHER_CANCEL_SEND(int tUserIndex);
//W_TEACHER_START_SEND
void W_TEACHER_START_SEND(int tUserIndex);
//W_TEACHER_STATE_SEND
void W_TEACHER_STATE_SEND(int tUserIndex);
//W_TEACHER_END_SEND
void W_TEACHER_END_SEND(int tUserIndex);
//W_CHANGE_TO_TRIBE4_SEND
void W_CHANGE_TO_TRIBE4_SEND(int tUserIndex);
//W_HERO_RANKING_SEND
void W_HERO_RANKING_SEND(int tUserIndex);

class MyWork
{
public:
	int mWorkerPacketSize[256];

	MyWork(void);

	BOOL Init(void);
	void Free(void);
	BOOL CheckValidCharacterMotionForSend(int tType, int tSort, int tSkillNumber, int* tSkillSort,
	                                      int* tCheckMaxAttackPacketNum, int* tAttackPacketSort,
	                                      int* tMaxAttackPacketNum);
	BOOL SkillForMotion(int tUserIndex, ACTION_INFO* pAction, int tCheckMotion);
	BOOL ReturnEffectState(int tUserIndex, int tSkillNumber);
	BOOL CheckPossiblePShopRegion(int tTribe, int tZoneNumber, float tCoord[3]);

	void ProcessForRelay(int tSort, BYTE tData[MAX_BROADCAST_DATA_SIZE], int type = 0);

	void ProcessForData(int tUserIndex, int tSort, BYTE tData[MAX_BROADCAST_DATA_SIZE]);

	BOOL ProcessForGetItem(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2, int tIndex2,
	                       int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForLearnSkill1(int tUserIndex, int nIndex, int sIndex, int* tResult);
	BOOL ProcessForSkillUpgrade(int tUserIndex, int tSkillIndex, int* tResult);
	BOOL ProcessForSkillToHotKey(int tUserIndex, int tSkillSort, int tSkillIndex, int tSkillGrade, int tHotKeyPage,
	                             int tHotKeyIndex, int* tResult);
	BOOL ProcessForHotKeyToNo(int tUserIndex, int tHotKeyPage, int tHotKeyIndex, int* tResult);
	BOOL ProcessForStatPlus(int tUserIndex, int tStatSort, int* tResult);
	BOOL ProcessForInventoryToInventory(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2,
	                                    int tIndex2, int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForInventoryToWorld(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2, int tIndex2,
	                                int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForInventoryToEquip(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2, int tIndex2,
	                                int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForInventoryToHotKey(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2, int tIndex2,
	                                 int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForInventoryToNPCShop(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2, int tIndex2,
	                                  int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForEquipToInventory(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2, int tIndex2,
	                                int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForHotKeyToInventory(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2, int tIndex2,
	                                 int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForNPCShopToInventory(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2, int tIndex2,
	                                  int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForHotKeyToHotKey(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2, int tIndex2,
	                              int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForInventoryMoneyToWorld(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2,
	                                     int tIndex2, int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForInventoryToTrade(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2, int tIndex2,
	                                int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForTradeToInventory(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2, int tIndex2,
	                                int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForInventoryToPetInventory(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2,
	                                       int tIndex2, int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForPetInventoryToInventory(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2,
	                                       int tIndex2, int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForPetInventoryToPetInventory(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2,
	                                          int tIndex2, int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForTradeToTrade(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2, int tIndex2,
	                            int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForInventoryMoneyToTradeMoney(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2,
	                                          int tIndex2, int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForInventoryMoneyToInventoryMoney2(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2,
	                                               int tIndex2, int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForInventoryMoney2ToInventoryMoney(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2,
	                                               int tIndex2, int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForTradeMoneyToInventoryMoney(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2,
	                                          int tIndex2, int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForInventoryMoney2ToTradeMoney2(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2,
	                                            int tIndex2, int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForTradeMoney2ToInventoryMoney2(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2,
	                                            int tIndex2, int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForInventoryMoney2ToStoreMoney2(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2,
	                                            int tIndex2, int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForStoreMoney2ToInventoryMoney2(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2,
	                                            int tIndex2, int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForInventoryMoney2ToSaveMoney2(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2,
	                                           int tIndex2, int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForSaveMoney2ToInventoryMoney2(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2,
	                                           int tIndex2, int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForInventoryToStore(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2, int tIndex2,
	                                int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForStoreToInventory(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2, int tIndex2,
	                                int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForStoreToStore(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2, int tIndex2,
	                            int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForInventoryMoneyToStoreMoney(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2,
	                                          int tIndex2, int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForStoreMoneyToInventoryMoney(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2,
	                                          int tIndex2, int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForInventoryToSave(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2, int tIndex2,
	                               int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForSaveToInventory(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2, int tIndex2,
	                               int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForSaveToSave(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2, int tIndex2,
	                          int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForInventoryMoneyToSaveMoney(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2,
	                                         int tIndex2, int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForSaveMoneyToInventoryMoney(int tUserIndex, int tPage1, int tIndex1, int tQuantity1, int tPage2,
	                                         int tIndex2, int tXPost2, int tYPost2, int* tResult);
	BOOL ProcessForLearnSkill2(int tUserIndex, int nIndex, int sIndex, int* tResult);

	void Sphinx_Question_Control(int iIndex);
	void Sphinx_Question_TimeCheck(int iIndex);
	void Sphinx_Punishment(int iIndex);
	void Sphinx_Requital(int iIndex);
};

//INSTANCE
extern MyWork mWORK;
#endif
