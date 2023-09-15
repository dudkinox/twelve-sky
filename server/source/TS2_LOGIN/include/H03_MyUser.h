//-------------------------------------------------------------------------------------------------
//MY_USER_H
//-------------------------------------------------------------------------------------------------
#ifndef MY_USER_H
#define MY_USER_H
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_USER
//-------------------------------------------------------------------------------------------------
//DEFINE
class MyUser
{

public :

	BOOL mCheckConnectState;
	BOOL mCheckValidState;

	DWORD mUsedTime;
	SOCKET mSocket;
	char mIP[16];
	int mIPPart[4];

	char mPacketEncryptionValue[2];

	char *mBUFFER_FOR_SEND;
	int mTotalSendSize;
	char *mBUFFER_FOR_RECV;
	int mTotalRecvSize;

	//------------------------//
	//VARIABLE_FOR_THIS_SERVER//
	//------------------------//
	DWORD mRegisterTime;
	int mPlayUserIndex;
	char uID[MAX_USER_ID_LENGTH];
	int uCheckMOTP;
	char uMousePassword[MAX_MOUSE_PASSWORD_LENGTH];
	int uSecretCard;
	int uUserSort;
	int uGoodFellow;
	int uLoginPlace;
	int uLoginPremium;
	int uLoginPremiumPCRoom;
	int uTraceState;
	char uBonus100Money;
	int uPremiumServerExpirationDate; // 프리미엄 서버(월드) 입장권 유효 기한(YYYYMMDD 형태의 정수). // @_Premium_Server_@
	int uSaveMoney;
	int uSaveItem[MAX_SAVE_ITEM_SLOT_NUM][4];
	AVATAR_INFO mAvatarInfo[MAX_USER_AVATAR_NUM];
	int mSecondLoginTryNum;
	int mSecondLoginSort;
	int mSecretCardIndex01;
	int mSecretCardIndex02;
	BOOL mCheckDemandGiftIfo;
	char mCardNumber[MAX_USER_CARD_NUMBER_LENGTH];
	int mGiftInfo[10];
	int mMoveZoneResult;
	int mZone164Type;
	AUTH_INFO mAuthInfo;
	//------------------------//
	//------------------------//
	//------------------------//

	MyUser( void );

	BOOL Init( int tSendBufferSize, int tRecvBufferSize );
	void Free( void );

	void Send( BOOL tCheckValidBuffer, char *tBuffer, int tBufferSize );
	//void Quit( void );
	void Quit(const char *tFileName = __FILE__, const char *tFuncName = __FUNCTION__ , int tLine = __LINE__);

};
//INSTANCE
extern MyUser *mUSER;
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------------
