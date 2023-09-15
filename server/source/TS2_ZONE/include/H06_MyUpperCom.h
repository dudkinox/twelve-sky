//-------------------------------------------------------------------------------------------------
//MY_UPPER_COM_H
//-------------------------------------------------------------------------------------------------
#ifndef MY_UPPER_COM_H
#define MY_UPPER_COM_H
#include "win2unix.h"
#include <pthread.h>
#include <poll.h>
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

private :

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

	BOOL Connect( void );
	BOOL Send( void );
	BOOL Recv( void );
	void Close( void );

public :

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

	MyPlayUserCom( void );

	BOOL Init( int tZoneNumber, char tIP[16], int tPort );
	void Free( void );

	BOOL ProcessForNetwork( void );

	void U_ZONE_OK_FOR_PLAYUSER_SEND( void );
	void U_REGISTER_USER_FOR_ZONE_0_SEND( char tID[MAX_USER_ID_LENGTH] );
	void U_REGISTER_USER_FOR_ZONE_1_SEND( int tPlayUserIndex, char tID[MAX_USER_ID_LENGTH], char tAvatarName[MAX_AVATAR_NAME_LENGTH], int tLogoutInfo[6] );
	void U_REGISTER_USER_FOR_ZONE_2_SEND( int tPlayUserIndex, char tID[MAX_USER_ID_LENGTH], AVATAR_INFO *tAvatarInfo, int tEffectValue[MAX_AVATAR_EFFECT_SORT_NUM][2], AUTH_INFO *tAuthInfo);
	void U_REGISTER_USER_FOR_ZONE_3_SEND( int tPlayUserIndex, char tID[MAX_USER_ID_LENGTH], AVATAR_INFO *tAvatarInfo, int tEffectValue[MAX_AVATAR_EFFECT_SORT_NUM][2], AUTH_INFO *tAuthInfo);
	void U_UNREGISTER_USER_FOR_ZONE_SEND( int tPlayUserIndex, char tID[MAX_USER_ID_LENGTH], AVATAR_INFO *tAvatarInfo, AUTH_INFO *tAuthInfo);
	void U_FIND_AVATAR_FOR_ZONE_SEND( char tAvatarName[MAX_AVATAR_NAME_LENGTH] );
	void U_TRIBE_BANK_INFO_SAVE_SEND( int tMoney[4] );
	void U_TRIBE_BANK_INFO_VIEW_SEND( int tTribe );
	void U_TRIBE_BANK_INFO_LOAD_SEND( int tTribe, int tTribeBankInfoIndex, int tInventoryMoney );
	void U_HACK_NAME_SEND( char tID[MAX_USER_ID_LENGTH], char tHackName[MAX_HACK_NAME_LENGTH], int tHackSize );
	void U_USER_SEND_CASH( int tSort, char tID[MAX_USER_ID_LENGTH] );
	void U_BLOCK_USER_FOR_ZONE_1_SEND( int tSort, char tID[MAX_USER_ID_LENGTH] );
	void U_BLOCK_USER_FOR_ZONE_2_SEND( int tSort, char tID[MAX_USER_ID_LENGTH] );
	void U_ADD_AUTO_USER_LIST_SEND( char tID[MAX_USER_ID_LENGTH], char tAvatarName[MAX_AVATAR_NAME_LENGTH], int tWorldNumber, int tZoneNumber );
	void U_CHECK_KILL_OTHER_TRIBE_SEND( char tAvatarName1[MAX_AVATAR_NAME_LENGTH], char tAvatarName2[MAX_AVATAR_NAME_LENGTH] );
	void U_EVENT_INFO_SEND( char tID[MAX_USER_ID_LENGTH], int tSort );
	void U_CHECK_AUTH_KEY (char szID[MAX_USER_ID_LENGTH], char szAuthKey[MAX_MOUSE_PASSWORD_LENGTH]);
	void U_UPDATE_AUTH_KEY (char szID[MAX_USER_ID_LENGTH], char szAuthKey[2][MAX_MOUSE_PASSWORD_LENGTH]);
	void U_CHANGE_AVATAR_NAME_FOR_ZONE_SEND(int tPlayUserIndex, char *tChangeAvatarName);
	void U_BONUS_100_MONEY_FOR_ZONE_SEND(int tPlayUserIndex);
#ifdef __EVENT_KILL__
	void U_EVENT_FOR_ZONE_SEND(int tPlayUserIndex, int tKillNum);
#elif __EVENT_GOLD_PHONE__
	void U_EVENT_FOR_ZONE_SEND(int tPlayUserIndex);
#endif
    void U_SET_MOVE_ITEM_FOR_ZONE_SEND( int tPlayUserIndex, MOVE_ITEM_INFO *tMoveItemInfo); // 이동서 사용 정보를 PlayUser에 전송하는 함수.
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

private :

	BOOL *mCheckSocketState;
	BOOL *mCheckConnectState;

	int mZoneNumber;
	int mZonePort;
	char mIP[16];
	int mPort;
	fd_set *m_read_fds;
	pthread_t m_handle;

	SOCKET mSocket;
	SOCKADDR_IN mAddress;

	char mSendData[MAX_SEND_DATA_SIZE];
	int mSendDataSize;
	char mRecvData[MAX_RECV_DATA_SIZE];
	int mRecvDataSize;

	BOOL Connect( void );
	BOOL Send( void );
	void Close( void );

public :

	pthread_mutex_t m_lock;
	MyCenterCom( void );
	void connect_thread(void);

	static void *Start(void *arg);
	BOOL Init( int tZoneNumber, int tZonePort, char tIP[16], int tPort );
	BOOL ReConnect(void);
	void Free( void );

	//void PROCESS_FOR_NETWORK( HWND hWnd, WPARAM wPrm, LPARAM lPrm );
	int PROCESS_FOR_NETWORK(SOCKET cli_fd, UINT fd_status);

	void W_WORLD_INFO( void );
	void W_ZONE_CONNECTION_INFO( void );
	void W_BROADCAST_INFO( void );
	void W_START( void );
	void W_NOTICE( void );
	void W_SET_EVENT( void );
	void W_SHUTDOWN( void );
	void W_TRIBE_CHAT( void );
	void W_SECRET_CHAT( void );
	void W_POLICE_INFO( void );
	void W_SET_GM_ORDER( void );
	void W_SERCH_AVATAR( void );
	void W_SET_GM_ORDER_MOVE_AVATAR( void );

	void W_BAN_PLAYER (void);
	void W_CHECK_EVENT( void );

	void W_RANK_TOTAL_SEND(void);
	void W_RANK_TOTAL_INFO_SEND(int TotalSize);
    // @ Cost Info Download @
    bool mIsUpdateCashItemInfo;
    bool IsUpdateCashItemInfo();
    void SetUpdateCashItemInfo(bool isUpdate);
    void W_UPDATE_CASH_ITEM_INFO_FOR_CENTER(int updateResult);  // 모든 Zone에게 캐쉬 아이템 정보가 업데이트 되었음을 알린다.
    void W_UPDATE_CASH_ITEM_INFO_FOR_ZONE();
    // @

    // # X-Trap # {
    void W_XTRAP_MAP_FILE_CHANGE_FOR_ZONE( void );
    // # }

	BOOL ProcessForSend( void );

	void U_ZONE_OK_FOR_CENTER_SEND( void );
	void U_DEMAND_BROADCAST_INFO( int tInfoSort, BYTE tInfoData[MAX_BROADCAST_INFO_SIZE] );
	
	//int SearchAvatarforGM( char tAvatarName[MAX_AVATAR_NAME_LENGTH] );
	int SearchAvatarforGM(const char *tAvatarName);
	void MoveZone(int tUserIndex, int tZoneNumber);
	void W_RANK_INFO_SEND(void);
	void W_CENTER_GET_RANK_INFO(const char *aName, int tUserIndex);
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

private :

	BOOL *mCheckSocketState;
	BOOL *mCheckConnectState;

	int mZoneNumber;
	char mIP[16];
	int mPort;
	fd_set *m_read_fds;
	pthread_t m_handle;

	SOCKET mSocket;
	SOCKADDR_IN mAddress;

	char mSendData[MAX_SEND_DATA_SIZE];
	int mSendDataSize;
	char mRecvData[MAX_RECV_DATA_SIZE];
	int mRecvDataSize;

	BOOL Connect( void );
	BOOL Send( void );
	void Close( void );

public :

	pthread_mutex_t m_lock;
	MyRelayCom( void );
	void connect_thread(void);

	static void *Start(void *arg);
	BOOL Init( int tZoneNumber, char tIP[16], int tPort );
	void Free( void );

	//void PROCESS_FOR_NETWORK( HWND hWnd, WPARAM wPrm, LPARAM lPrm );
	int PROCESS_FOR_NETWORK(SOCKET cli_fd, UINT fd_status);


	void W_BROADCAST_DATA( void );

	BOOL ProcessForSend( void );

	void U_ZONE_OK_FOR_RELAY_SEND( void );
	void U_DEMAND_BROADCAST_DATA( int tSort, BYTE tData[MAX_BROADCAST_DATA_SIZE] );

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

private :

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

	BOOL Connect( void );
	BOOL Send( void );
	BOOL Recv( void );
	void Close( void );

public :

	int mRecv_Result;
	GUILD_INFO mRecv_GuildInfo;

	int mRecv_CashSize;
	int mRecv_BonusCashSize;
	int mRecv_CostSize;
	int mRecv_BonusCostSize;

    // @ Cost Info Download @
    bool mIsValidCashItemInfo;              // 현재 가지고 있는 캐쉬 아이템 정보가 최신인지 여부.
    int mCashItemInfoVersion;               // COST_INFO.TXT의 버전
    int mCashItemInfo[4][10][10][4];        // 클라이언트에서 사용하는 캐쉬 아이템 진열 정보

    BOOL U_UPDATE_CASH_ITEM_INFO_FOR_EXTRA_SEND();  // Update할 필요가 있는 지 검사. - 37번 존에서만 사용
    BOOL U_GET_CASH_ITEM_INFO_FOR_EXTRA_SEND();     // 캐쉬 아이템 정보를 새로 받아 온다.
    // @

	pthread_mutex_t m_lock;

	MyExtraCom( void );

	BOOL Init( int tZoneNumber, char tIP[16], int tPort );
	void Free( void );

	BOOL ProcessForNetwork( void );

	BOOL U_ZONE_OK_FOR_EXTRA_SEND( void );
	BOOL U_GUILD_WORK_FOR_EXTRA_SEND( int tSort, BYTE tData[MAX_GUILD_WORK_SIZE] );
	BOOL U_GET_CASH_SIZE_FOR_EXTRA_SEND( char tID[MAX_USER_ID_LENGTH] );
    // @ Cost Info Download @
    //BOOL U_BUY_CASH_ITEM_FOR_EXTRA_SEND( char tID[MAX_USER_ID_LENGTH], int tCostInfoIndex, int iIndex, int iQuantity, char tAvatarName[MAX_AVATAR_NAME_LENGTH], int tTribe, int tLevel );
    BOOL U_BUY_CASH_ITEM_FOR_EXTRA_SEND( char tID[MAX_USER_ID_LENGTH], int tCostInfoIndex, int iIndex, int iQuantity, char tAvatarName[MAX_AVATAR_NAME_LENGTH], int tTribe, int tLevel, int tVersion );
    // @

    // 2009.12.16 터키에서만 사용합니다.
    // 패키지 아이템(1418 ~ 1423) 사용 시 보너스 캐쉬 지급 요청 함수.
    BOOL U_USE_PACKAGE_ITEM_FOR_EXTRA_SEND( char tID[MAX_USER_ID_LENGTH], int iIndex, char tAvatarName[MAX_AVATAR_NAME_LENGTH] );
};
//INSTANCE
extern MyExtraCom mEXTRA_COM;
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_GAMELOG
//-------------------------------------------------------------------------------------------------
//DEFINE
class MyGameLog
{

private :

	BOOL mCheckConnectState;

	SOCKET mSocket;
	SOCKADDR_IN mAddress;

	int mLogSort;
	char mGameLogData[MAX_GAMELOG_LENGTH];
	char mSendBuffer[( MAX_GAMELOG_LENGTH * 2 )];

	BOOL CheckValidState( int tUserIndex );

public :

	MyGameLog( void );

	//static void *Start(void *arg);
	BOOL Init( char tIP[16], int tPort );
	void Free( void );

	void SendToLogServer( void );
	void SendToLogServerForDB( void );

	void GL_601_GM_CREATE_MONEY( int tUserIndex, int tValue );
	void GL_602_GM_CREATE_ITEM( int tUserIndex, int iIndex, int iQuantity, int iValue, int iRecognitionNumber );
	void GL_603_TRACE_USER( int tUserIndex, int tValue );
	void GL_604_BUY_CASH_ITEM( int tUserIndex, int iIndex, int iQuantity, int iValue, int iRecognitionNumber, int iCostSize, int iBonusCostSize );
	void GL_605_USE_CASH_ITEM( int tUserIndex, int iIndex, int iQuantity, int iValue, int iRecognitionNumber );
	void GL_606_USE_INVENTORY_ITEM( int tUserIndex, int iIndex, int iQuantity, int iValue, int iRecognitionNumber, int tIndex, int tQuantity, int tValue, int tRecognitionNumber );
	void GL_607_GAIN_SIN_ITEM( int tUserIndex, int tAction, int iIndex, int iQuantity, int iValue, int iRecognitionNumber );
	void GL_608_IMPROVE_ITEM( int tUserIndex, int tAction, int iIndex, int iQuantity, int iValue, int iRecognitionNumber, int tPrice );
	void GL_609_PSHOP_ITEM( int tUserIndex, int tAction, int iIndex, int iQuantity, int iValue, int iRecognitionNumber, int tPrice );
	void GL_610_QUEST_ITEM( int tUserIndex, int iIndex, int iQuantity, int iValue, int iRecognitionNumber );
	void GL_611_QUEST_MONEY( int tUserIndex, int tValue );
	void GL_612_QUEST_KILL_NUM( int tUserIndex, int tValue );
	void GL_613_QUEST_EXP( int tUserIndex, int tValue );
	void GL_614_QUEST_TEACHER_POINT( int tUserIndex, int tValue );
	void GL_615_TRADE_ITEM( int tUserIndex, int tOtherIndex, int iIndex, int iQuantity, int iValue, int iRecognitionNumber );
	void GL_616_TRADE_MONEY( int tUserIndex, int tOtherIndex, int tValue );
	void GL_617_GUILD_MONEY( int tUserIndex, int tValue, int tAction, int tGrade );
	void GL_618_TRIBE_MONEY( int tUserIndex, int tValue );
	void GL_619_GAIN_ITEM( int tUserIndex, int iIndex, int iQuantity, int iValue, int iRecognitionNumber );
	void GL_620_DROP_ITEM( int tUserIndex, int iIndex, int iQuantity, int iValue, int iRecognitionNumber );
	void GL_621_NSHOP_ITEM( int tUserIndex, int tAction, int iIndex, int iQuantity, int iValue, int iRecognitionNumber, int tPrice );
	void GL_622_TRADESLOT_ITEM( int tUserIndex, int tAction, int iIndex, int iQuantity, int iValue, int iRecognitionNumber );
	void GL_623_TRADESLOT_MONEY( int tUserIndex, int tAction, int tValue );
	void GL_624_STROESLOT_ITEM( int tUserIndex, int tAction, int iIndex, int iQuantity, int iValue, int iRecognitionNumber );
	void GL_625_STORESLOT_MONEY( int tUserIndex, int tAction, int tValue );
	void GL_626_SAVESLOT_ITEM( int tUserIndex, int tAction, int iIndex, int iQuantity, int iValue, int iRecognitionNumber );
	void GL_627_SAVESLOT_MONEY( int tUserIndex, int tAction, int tValue );
	void GL_628_GM_MONCALL( int tUserIndex, int tValue );
	void GL_629_GM_NCHAT( int tUserIndex, int tOtherIndex );
	void GL_630_GM_YCHAT( int tUserIndex, int tOtherIndex );
	void GL_631_GM_KICK( int tUserIndex, int tOtherIndex );
	void GL_632_GM_BLOCK( int tUserIndex, int tOtherIndex );
	void GL_633_MINUS_EXP_WITH_DIE( int tUserIndex, int tValue1, int tValue2 );
	void GL_634_ADD_ITEM( int tUserIndex, int tAction, int iIndex, int iQuantity, int iValue, int iRecognitionNumber, int tPrice );
	void GL_635_HIGH_ITEM( int tUserIndex, int tAction, int iIndex, int iQuantity, int iValue, int iRecognitionNumber, int tPrice, int tItem2 );
	void GL_636_LOW_ITEM( int tUserIndex, int tAction, int iIndex, int iQuantity, int iValue, int iRecognitionNumber, int tPrice, int tItem2 );
	void GL_637_EXCHANGE_ITEM( int tUserIndex, int tAction, int iIndex, int iQuantity, int iValue, int iRecognitionNumber, int tPrice );
	void GL_638_MAKE_ITEM( int tUserIndex, int tAction, int iIndex, int iQuantity, int iValue, int iRecognitionNumber );
	void GL_639_MAKE_SKILL( int tUserIndex, int tAction, int iIndex, int iQuantity, int iValue, int iRecognitionNumber );
	void GL_640_USER_CREATE_ITEM( int tUserIndex, int iIndex, int iQuantity, int iValue, int iRecognitionNumber );
	void GL_641_MAKE_TRIBE_WEAPON( int tUserIndex, int iIndex, int iQuantity, int iValue, int iRecognitionNumber );
	void GL_642_N_PROTECT( int tUserIndex, int tValue );
	void GL_643_CHECK_MONSTER_ATTACK_PACKET( int tUserIndex );
	void GL_644_CHECK_AUTH_KEY (int tUserIndex);
#ifndef __ORG__
	void GL_645_GMTOOL_KICK( int tOtherIndex );
	void GL_646_GMTOOL_NCHAT( int tOtherIndex );
	void GL_647_GMTOOL_YCHAT( int tOtherIndex );
#endif
	void GL_648_GM_GOD(int tUserIndex, int tValue);
	void GL_649_GM_CHAT(int tUserIndex);
	void GL_650_GM_UMOVE(int tUserIndex, int tOtherUserIndex, int tZone);
	void GL_651_GM_LEVEL(int tUserIndex, int tLevel);
	void GL_652_GM_ZNOTICE(int tUserIndex);
	void GL_653_GM_RES(int tUserIndex, int tOtherUserIndex, int tRes);
	void GL_654_GM_RENAME(int tUserIndex, int tOtherUserIndex, char *tName, char* tChangeName);
	void GL_655_GM_CALL(int tUserIndex, int tOtherUserIndex);
	void GL_656_GM_EXP(int tUserIndex, int tExp);
	void GL_657_GM_DIE(int tUserIndex, int tMonsterNumber);
	void GL_658_USE_INVENTORY_ITEM( int tUserIndex, int iIndex, int iQuantity, int iValue, int iRecognitionNumber, int tIndex, int tQuantity, int tValue, int tRecognitionNumber, int tCount );
    // # Defense Hack # {
	void GL_659_AUTO_CHECK (int tUserIndex, int tType, float fValue, int iValue1 = 0, int iValue2 = 0);
	//void GL_659_AUTO_CHECK (int tUserIndex, int tType, float fValue);
    // # }
	void GL_660_AUTO_CHECK (int tUserIndex);
	void GL_661_START_MISSION_LABYRINTH (void);
	void GL_662_END_MISSION_LABYRINTH (void);
    // # X-Trap # {
    void GL_663_X_TRAP( int tUserIndex, unsigned long tErrCode );
    // # }
    void GL_664_BONUS_CASH_FAIL_FOR_PACKAGE_ITEM(int tUserIndex, int iIndex, int failType);
    void GL_700_SPHINX_HACK(int tUserIndex);
    void GL_701_CHEAT_USER(int tUserIndex, int cheatSort);
    void GL_702_CHEAT_AVATAR_ACTION(int tUserIndex, int skillNum, int skillGrad1, int skillGrad2, int skillVal);

	/** 2009. 12. 18 : 기여도 관련 로그 추가 (wolf) */
	void GL_800_USE_KILLNUM_TO_ITEM(int tUserIndex, int iBeforeKillOtherTribe, int iAfterKillOtherTribe);
	void GL_801_UPGRADE_AVATAR( int tUserIndex, int tValue01, int tValue02 );
	void GL_802_USE_KILLOTHERTRIBE_TO_ITEM (int tUserIndex, int iBeforeKillOtherTribe, int iAfterKillOtherTribe);
	void CHAT_LOG_SEND( int tUserIndex, char *aName, char *Chat,char *Type );
	/** 2009. 12. 18 : 기여도 관련 로그 추가 (wolf) */
};
//INSTANCE
extern MyGameLog mGAMELOG;
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_CHATLOG
//-------------------------------------------------------------------------------------------------
//DEFINE
class MyChatLog
{

private :

	BOOL mCheckConnectState;

	SOCKET mSocket;
	SOCKADDR_IN mAddress;

public :

	MyChatLog( void );

	//static void *Start(void *arg);
	BOOL Init( char tIP[16], int tPort );
	void Free( void );

	void SendToLogServer( int tTribe, int tChatSort, char tAvatarName01[MAX_AVATAR_NAME_LENGTH], char tAvatarName02[MAX_AVATAR_NAME_LENGTH], char tContent[MAX_CHAT_CONTENT_LENGTH] );

};
//INSTANCE
extern MyChatLog mCHATLOG;
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------------
