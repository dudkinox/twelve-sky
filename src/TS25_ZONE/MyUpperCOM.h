//-------------------------------------------------------------------------------------------------
//MY_UPPER_COM_H
//-------------------------------------------------------------------------------------------------
#ifndef MY_UPPER_COM_H
#define MY_UPPER_COM_H
#include <pthread.h>
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//DEFINE_MACRO_OF_MY_UPPER_COM
//-------------------------------------------------------------------------------------------------
#define MAX_SEND_DATA_SIZE											50000
#define MAX_RECV_DATA_SIZE											50000
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_PLAYUSER_COM
//-------------------------------------------------------------------------------------------------
//DEFINE
class MyPlayUserCom
{
private:
	BOOL mCheckConnectState;

	int mZoneNumber;
	char mIP[16];
	int mPort;
	pthread_t m_handle;
	pthread_mutex_t m_lock;

	SOCKET mSocket;
	SOCKADDR_IN mAddress;

	char mSendData[MAX_SEND_DATA_SIZE];
	int mSendDataSize;
	char mRecvData[MAX_RECV_DATA_SIZE];
	int mRecvDataSize;

	BOOL Connect(void);
	BOOL Send(void);
	BOOL Recv(void);
	void Close(void);

public:
	int mRecv_Result;
	int mRecv_PlayUserIndex;
	int mRecv_UserSort;
	int mRecv_GoodFellow;
	int mRecv_LoginPlace;
	int mRecv_LoginPremium;
	int mRecv_LoginPremiumPCRoom;
	int mRecv_TraceState;
	char mRecv_Bonus100Money;
	int mRecv_PremiumServerExpirationDate; // 프리미엄 서버(월드) 입장권 유효 기한(YYYYMMDD 형태의 정수). // @_Premium_Server_@
	AVATAR_INFO mRecv_AvatarInfo;
	int mRecv_EffectValue[MAX_AVATAR_EFFECT_SORT_NUM][2];
	int mRecv_CheckFirstZone;
	int mRecv_ZoneNumber;
	int mRecv_Tribe;
	int mRecv_TribeBankInfo[MAX_TRIBE_BANK_SLOT_NUM];
	int mRecv_Money;
	AUTH_INFO mRecv_AuthInfo;
	MOVE_ITEM_INFO mRecv_MoveItemInfo; // 이동서 사용 정보.

	MyPlayUserCom(void);

	BOOL Init(int tZoneNumber, char tIP[16], int tPort);
	void Free(void);

	BOOL ProcessForNetwork(void);

	void U_ZONE_OK_FOR_PLAYUSER_SEND(void);
	void U_REGISTER_USER_FOR_ZONE_0_SEND(char tID[MAX_USER_ID_LENGTH]);
	void U_REGISTER_USER_FOR_ZONE_1_SEND(int tPlayUserIndex, char tID[MAX_USER_ID_LENGTH],
	                                     char tAvatarName[MAX_AVATAR_NAME_LENGTH], int tLogoutInfo[6]);
	void U_REGISTER_USER_FOR_ZONE_2_SEND(int tPlayUserIndex, char tID[MAX_USER_ID_LENGTH], AVATAR_INFO* tAvatarInfo,
	                                     int tEffectValue[MAX_AVATAR_EFFECT_SORT_NUM][2], AUTH_INFO* tAuthInfo);
	void U_REGISTER_USER_FOR_ZONE_3_SEND(int tPlayUserIndex, char tID[MAX_USER_ID_LENGTH], AVATAR_INFO* tAvatarInfo,
	                                     int tEffectValue[MAX_AVATAR_EFFECT_SORT_NUM][2], AUTH_INFO* tAuthInfo);
	void U_UNREGISTER_USER_FOR_ZONE_SEND(int tPlayUserIndex, char tID[MAX_USER_ID_LENGTH], AVATAR_INFO* tAvatarInfo,
	                                     AUTH_INFO* tAuthInfo);
	void U_FIND_AVATAR_FOR_ZONE_SEND(char tAvatarName[MAX_AVATAR_NAME_LENGTH]);
	void U_TRIBE_BANK_INFO_SAVE_SEND(int tMoney[4]);
	void U_TRIBE_BANK_INFO_VIEW_SEND(int tTribe);
	void U_TRIBE_BANK_INFO_LOAD_SEND(int tTribe, int tTribeBankInfoIndex, int tInventoryMoney);
	void U_HACK_NAME_SEND(char tID[MAX_USER_ID_LENGTH], char tHackName[MAX_HACK_NAME_LENGTH], int tHackSize);
	void U_BLOCK_USER_FOR_ZONE_1_SEND(int tSort, char tID[MAX_USER_ID_LENGTH]);
	void U_BLOCK_USER_FOR_ZONE_2_SEND(int tSort, char tID[MAX_USER_ID_LENGTH]);
	void U_ADD_AUTO_USER_LIST_SEND(char tID[MAX_USER_ID_LENGTH], char tAvatarName[MAX_AVATAR_NAME_LENGTH],
	                               int tWorldNumber, int tZoneNumber);
	void U_CHECK_KILL_OTHER_TRIBE_SEND(char tAvatarName1[MAX_AVATAR_NAME_LENGTH],
	                                   char tAvatarName2[MAX_AVATAR_NAME_LENGTH]);
	void U_EVENT_INFO_SEND(char tID[MAX_USER_ID_LENGTH], int tSort);
	void U_CHECK_AUTH_KEY(char szID[MAX_USER_ID_LENGTH], char szAuthKey[MAX_MOUSE_PASSWORD_LENGTH]);
	void U_UPDATE_AUTH_KEY(char szID[MAX_USER_ID_LENGTH], char szAuthKey[2][MAX_MOUSE_PASSWORD_LENGTH]);
	void U_CHANGE_AVATAR_NAME_FOR_ZONE_SEND(int tPlayUserIndex, char* tChangeAvatarName);
	void U_BONUS_100_MONEY_FOR_ZONE_SEND(int tPlayUserIndex);
#ifdef __EVENT_KILL__
	void U_EVENT_FOR_ZONE_SEND(int tPlayUserIndex, int tKillNum);
#elif __EVENT_GOLD_PHONE__
	void U_EVENT_FOR_ZONE_SEND(int tPlayUserIndex);
#endif
	void U_SET_MOVE_ITEM_FOR_ZONE_SEND(int tPlayUserIndex, MOVE_ITEM_INFO* tMoveItemInfo);
	// 이동서 사용 정보를 PlayUser에 전송하는 함수.
};

//INSTANCE
extern MyPlayUserCom mPLAYUSER_COM;
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_CENTER_COM
//-------------------------------------------------------------------------------------------------
//DEFINE
class MyCenterCom
{
private:
	BOOL* mCheckSocketState;
	BOOL* mCheckConnectState;

	int mZoneNumber;
	int mZonePort;
	char mIP[16];
	int mPort;
	fd_set* m_read_fds;
	pthread_t m_handle;

	SOCKET mSocket;
	SOCKADDR_IN mAddress;

	char mSendData[MAX_SEND_DATA_SIZE];
	int mSendDataSize;
	char mRecvData[MAX_RECV_DATA_SIZE];
	int mRecvDataSize;

	BOOL Connect(void);
	BOOL Send(void);
	void Close(void);

public:
	pthread_mutex_t m_lock;
	MyCenterCom(void);
	void connect_thread(void);

	static void* Start(void* arg);
	BOOL Init(int tZoneNumber, int tZonePort, char tIP[16], int tPort);
	BOOL ReConnect(void);
	void Free(void);

	//void PROCESS_FOR_NETWORK( HWND hWnd, WPARAM wPrm, LPARAM lPrm );
	int PROCESS_FOR_NETWORK(HWND hWnd, WPARAM wPrm, LPARAM lPrm);

	void W_WORLD_INFO(void);
	void W_ZONE_CONNECTION_INFO(void);
	void W_BROADCAST_INFO(void);
	void W_START(void);
	void W_NOTICE(void);
	void W_SET_EVENT(void);
	void W_SHUTDOWN(void);
	void W_TRIBE_CHAT(void);
	void W_SECRET_CHAT(void);
	void W_POLICE_INFO(void);
	void W_SET_GM_ORDER(void);
	void W_SERCH_AVATAR(void);
	void W_SET_GM_ORDER_MOVE_AVATAR(void);

	void W_BAN_PLAYER(void);
	void W_CHECK_EVENT(void);

	void W_RANK_TOTAL_SEND(void);
	void W_RANK_TOTAL_INFO_SEND(int TotalSize);
	// @ Cost Info Download @
	bool mIsUpdateCashItemInfo;
	bool IsUpdateCashItemInfo();
	void SetUpdateCashItemInfo(bool isUpdate);
	void W_UPDATE_CASH_ITEM_INFO_FOR_CENTER(int updateResult); // 모든 Zone에게 캐쉬 아이템 정보가 업데이트 되었음을 알린다.
	void W_UPDATE_CASH_ITEM_INFO_FOR_ZONE();
	// @

	// # X-Trap # {
	void W_XTRAP_MAP_FILE_CHANGE_FOR_ZONE(void);
	// # }

	BOOL ProcessForSend(void);

	void U_ZONE_OK_FOR_CENTER_SEND(void);
	void U_DEMAND_BROADCAST_INFO(int tInfoSort, BYTE tInfoData[MAX_BROADCAST_INFO_SIZE]);

	//int SearchAvatarforGM( char tAvatarName[MAX_AVATAR_NAME_LENGTH] );
	int SearchAvatarforGM(const char* tAvatarName);
	void MoveZone(int tUserIndex, int tZoneNumber);
	void W_RANK_INFO_SEND(void);
	void W_CENTER_GET_RANK_INFO(const char* aName, int tUserIndex);
	void W_ZONE_OTHER_CONNECTION_INFO(int tZoneNumber);
};

//INSTANCE
extern MyCenterCom mCENTER_COM;
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_RELAY_COM
//-------------------------------------------------------------------------------------------------
//DEFINE
class MyRelayCom
{
private:
	BOOL* mCheckSocketState;
	BOOL* mCheckConnectState;

	int mZoneNumber;
	char mIP[16];
	int mPort;
	fd_set* m_read_fds;
	pthread_t m_handle;

	SOCKET mSocket;
	SOCKADDR_IN mAddress;

	char mSendData[MAX_SEND_DATA_SIZE];
	int mSendDataSize;
	char mRecvData[MAX_RECV_DATA_SIZE];
	int mRecvDataSize;

	BOOL Connect(void);
	BOOL Send(void);
	void Close(void);

public:
	pthread_mutex_t m_lock;

	CRITICAL_SECTION m_lock2;
	MyRelayCom(void);
	void connect_thread(void);

	static void* Start(void* arg);
	BOOL Init(int tZoneNumber, char tIP[16], int tPort);
	void Free(void);

	//void PROCESS_FOR_NETWORK( HWND hWnd, WPARAM wPrm, LPARAM lPrm );
	int PROCESS_FOR_NETWORK(HWND hWnd, WPARAM wPrm, LPARAM lPrm);


	void W_BROADCAST_DATA(void);

	BOOL ProcessForSend(void);

	void U_ZONE_OK_FOR_RELAY_SEND(void);
	void U_DEMAND_BROADCAST_DATA(int tSort, BYTE tData[MAX_BROADCAST_DATA_SIZE]);
};

//INSTANCE
extern MyRelayCom mRELAY_COM;
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_EXTRA_COM
//-------------------------------------------------------------------------------------------------
//DEFINE
class MyExtraCom
{
private:
	BOOL mCheckConnectState;

	int mZoneNumber;
	char mIP[16];
	int mPort;
	pthread_t m_handle;

	SOCKET mSocket;
	SOCKADDR_IN mAddress;

	char mSendData[MAX_SEND_DATA_SIZE];
	int mSendDataSize;
	char mRecvData[MAX_RECV_DATA_SIZE];
	int mRecvDataSize;

	BOOL Connect(void);
	BOOL Send(void);
	BOOL Recv(void);
	void Close(void);

public:
	int mRecv_Result;
	GUILD_INFO mRecv_GuildInfo;

	int mRecv_CashSize;
	int mRecv_BonusCashSize;
	int mRecv_CostSize;
	int mRecv_BonusCostSize;

	// @ Cost Info Download @
	bool mIsValidCashItemInfo; // 현재 가지고 있는 캐쉬 아이템 정보가 최신인지 여부.
	int mCashItemInfoVersion; // COST_INFO.TXT의 버전
	int mCashItemInfo[4][14][10][4]; // 클라이언트에서 사용하는 캐쉬 아이템 진열 정보

	BOOL U_UPDATE_CASH_ITEM_INFO_FOR_EXTRA_SEND(); // Update할 필요가 있는 지 검사. - 37번 존에서만 사용
	BOOL U_GET_CASH_ITEM_INFO_FOR_EXTRA_SEND(); // 캐쉬 아이템 정보를 새로 받아 온다.
	// @

	pthread_mutex_t m_lock;

	MyExtraCom(void);

	BOOL Init(int tZoneNumber, char tIP[16], int tPort);
	void Free(void);

	BOOL ProcessForNetwork(void);

	BOOL U_ZONE_OK_FOR_EXTRA_SEND(void);
	BOOL U_GUILD_WORK_FOR_EXTRA_SEND(int tSort, BYTE tData[MAX_GUILD_WORK_SIZE]);
	BOOL U_GET_CASH_SIZE_FOR_EXTRA_SEND(char tID[MAX_USER_ID_LENGTH]);
	// @ Cost Info Download @
	//BOOL U_BUY_CASH_ITEM_FOR_EXTRA_SEND( char tID[MAX_USER_ID_LENGTH], int tCostInfoIndex, int iIndex, int iQuantity, char tAvatarName[MAX_AVATAR_NAME_LENGTH], int tTribe, int tLevel );
	BOOL U_BUY_CASH_ITEM_FOR_EXTRA_SEND(char tID[MAX_USER_ID_LENGTH], int tCostInfoIndex, int iIndex, int iQuantity,
	                                    char tAvatarName[MAX_AVATAR_NAME_LENGTH], int tTribe, int tLevel, int tVersion);
	// @

	// 2009.12.16 터키에서만 사용합니다.
	// 패키지 아이템(1418 ~ 1423) 사용 시 보너스 캐쉬 지급 요청 함수.
	BOOL U_USE_PACKAGE_ITEM_FOR_EXTRA_SEND(char tID[MAX_USER_ID_LENGTH], int iIndex,
	                                       char tAvatarName[MAX_AVATAR_NAME_LENGTH]);
};

//INSTANCE
extern MyExtraCom mEXTRA_COM;
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_GAMELOG
//-------------------------------------------------------------------------------------------------
#endif
