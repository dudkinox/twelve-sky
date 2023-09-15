//-------------------------------------------------------------------------------------------------
//MY_UPPER_COM_H
//-------------------------------------------------------------------------------------------------
#ifndef MY_UPPER_COM_H
#define MY_UPPER_COM_H
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

	char mIP[16];
	int mPort;

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
	int mRecv_PresentUserNum;

	MyPlayUserCom( void );

	BOOL Init( char tIP[16], int tPort );
	void Free( void );

	BOOL ProcessForNetwork( void );

	void U_LOGIN_OK_FOR_PLAYUSER_SEND( void );
	void U_REGISTER_USER_FOR_LOGIN_1_SEND( char tIP[16], char tID[MAX_USER_ID_LENGTH], int tUserSort, int tGoodFellow, int tLoginPlace, int tLoginPremium, int tLoginPremiumPCRoom, int tTraceState, char tBonus100Money, int tPremiumServerExpirationDate, AUTH_INFO *tAuthInfo ); // 함수 선언 수정 - int tPremiumServerExpirationDate 추가. // @_Premium_Server_@
	void U_REGISTER_USER_FOR_LOGIN_2_SEND( int tPlayUserIndex, char tID[MAX_USER_ID_LENGTH] );
	void U_REGISTER_USER_FOR_LOGIN_3_SEND( int tPlayUserIndex, char tID[MAX_USER_ID_LENGTH], AVATAR_INFO *tAvatarInfo );
	void U_UNREGISTER_USER_FOR_LOGIN_SEND( int tPlayUserIndex, char tID[MAX_USER_ID_LENGTH] );
	void U_GET_PRESENT_USER_NUM_FOR_LOGIN_SEND( void );
	void U_BLOCK_USER_FOR_LOGIN_1_SEND( int tSort, char tID[MAX_USER_ID_LENGTH] );
	void U_BLOCK_USER_FOR_LOGIN_2_SEND( int tSort, char tID[MAX_USER_ID_LENGTH] );

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

	BOOL mCheckConnectState;

	char mIP[16];
	int mPort;

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
	char mRecv_IP[16];
	int mRecv_Port;

	MyCenterCom( void );

	BOOL Init( char tIP[16], int tPort );
	void Free( void );

	BOOL ProcessForNetwork( void );

	void U_LOGIN_OK_FOR_CENTER_SEND( void );
	void U_GET_ZONE_SERVER_INFO_1( int tZoneNumber );

};
//INSTANCE
extern MyCenterCom mCENTER_COM;
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

	BOOL CheckValidState( char tID[MAX_USER_ID_LENGTH] );

public :

	MyGameLog( void );

	BOOL Init( char tIP[16], int tPort );
	void Free( void );

	void SendToLogServer( void );
	void SendToLogServerForDB( void );

	void GL_501_CHANGE_AVATAR_NAME( char tID[MAX_USER_ID_LENGTH], char tAvatarName[MAX_AVATAR_NAME_LENGTH], char tChangeAvatarName[MAX_AVATAR_NAME_LENGTH] );
	void GL_502_MISMATCH_MOUSE_PASSWORD( char tID[MAX_USER_ID_LENGTH], char tMousePassword[MAX_MOUSE_PASSWORD_LENGTH], char tInputMousePassword[MAX_MOUSE_PASSWORD_LENGTH] );
	void GL_503_GIFT( char tID[MAX_USER_ID_LENGTH], int tValue );
	void GL_504_MOUSE_PASS_THREE_OUT( char tID[MAX_USER_ID_LENGTH]);
    // @ __WEB_MALL__
	void GL_505_WEB_MALL(char tID[MAX_USER_ID_LENGTH], int *tList);
    // __WEB_MALL__ @
};
//INSTANCE
extern MyGameLog mGAMELOG;

class CGiftBankComm
{

private :

	BOOL mCheckConnectState;

	char mIP[16];
	int mPort;

	SOCKET mSocket;
	SOCKADDR_IN mAddress;

	char mSendData[MAX_SEND_DATA_SIZE];
	int mSendDataSize;
	char mRecvData[MAX_RECV_DATA_SIZE];
	int mRecvDataSize;

	BOOL Connect( void );
	BOOL Send( void );
	BOOL Recv( short nType );
	void Close( void );

public :

	int mRecv_Result;
	int mRecv_PlayUserIndex;
	int mRecv_PresentUserNum;

	CGiftBankComm( void );

	BOOL Init( char tIP[16], int tPort );
	void Free( void );

	BOOL ProcessForNetwork( short nType );

    void    U_SEND_HEARTBEAT (void);
    void    U_CHECK_GIFTISSUESERIAL (int tUserIndex, char szSerial[MAX_USER_CARD_NUMBER_LENGTH]);
    bool    U_UPDATE_GIFTISSUEINFO (int tUserIndex, char szSerial[MAX_USER_CARD_NUMBER_LENGTH], 
                                    char szID[MAX_USER_ID_LENGTH]);
};
//INSTANCE
extern CGiftBankComm mGiftBankComm;

#ifdef __CHANNEL__
class CCompany
{

private :

	BOOL mCheckConnectState;

	char mIP[16];
	int mPort;

	SOCKET mSocket;
	SOCKADDR_IN mAddress;

	char mSendData[MAX_SEND_DATA_SIZE];
	int mSendDataSize;
	char mRecvData[MAX_RECV_DATA_SIZE];
	int mRecvDataSize;

	BOOL Connect( void );
	BOOL Send( void );
	BOOL Recv( short nType );
	void Close( void );

public :

	int mRecv_Result;
	int mRecv_PlayUserIndex;
	int mRecv_PresentUserNum;

	CCompany( void );

	BOOL Init( char tIP[16], int tPort );
	void Free( void );

	BOOL ProcessForNetwork( short nType );
	BOOL U_CHECK_ACCOUNT (char* tID, char* tPassword, int* tAdult, int* tResult );

};
//INSTANCE
extern CCompany mCompany;
#endif
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------------
