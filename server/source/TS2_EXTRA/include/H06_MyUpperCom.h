//-------------------------------------------------------------------------------------------------
// 2008.11.18 Kim SungSoo
// Intro : 1.define to convert that win32 variable type to c++ type
//-------------------------------------------------------------------------------------------------
#include "win2unix.h"
#include <pthread.h>
#include <poll.h>
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//MY_UPPER_COM_H
//-------------------------------------------------------------------------------------------------
#ifndef MY_UPPER_COM_H
#define MY_UPPER_COM_H
//-------------------------------------------------------------------------------------------------

#pragma pack (push, 1)
// 패키지 아이템 사용 시 보너스 캐쉬 요청 패킷 구조체.
typedef struct _BILL_PACK_USE_REQ
{
	WORD wPacketType;   // 전문구분자(10:게임서버연결요청, 20:잔액조회요청, 30:과금요청, 40:패키지 아이템 사용 요청.
	WORD wPacketSize;   // 전문길이
	DWORD dwSeqNo;      // 패킷 일련번호

	DWORD dwIPAddr;     // IP Addr.
	char szUserID[51];  // 사용자 ID
	char szItemKey[51]; // 게임내의 아이템 고유 key(아이템 번호).
} BILL_PACK_USE_REQ;

typedef struct _BILL_PACK_USE_REP
{
	WORD wPacketType;   // 전문구분자(11:게임서버연결응답, 21:잔액조회응답, 31:과금응답, 41:패키지 아이템 사용 응답.
	WORD wPacketSize;   // 전문길이
	DWORD dwSeqNo;      // 패킷 일련번호

	BYTE bResult;       // 응답 코드
	DWORD dwRemainGC;   // Game Cash 잔액
} BILL_PACK_USE_REP;
#pragma pack (pop)

//-------------------------------------------------------------------------------------------------
//DEFINE_CLASS_FOR_CCashCom
//-------------------------------------------------------------------------------------------------
//CLASS
class CCashCom
{

private :

	BOOL mCheckConnectState;

	char mIP[16];
	int mPort;

	SOCKET mSocket;
	SOCKADDR_IN mAddress;

	BOOL Connect( void );
	void Close( void );

public :

	BOOL Init( void );
	void Free( void );

	int GetCash( char *tID );
	int ChangeCash( char *tID, int tCostInfoIndex, char *tSellNo );

    int UsePackageItem( char *tID, int iIndex );

};
//INSTANCE
extern CCashCom mCASH_COM;
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

	void GL_000_ITEM_LOG( char uID[MAX_USER_ID_LENGTH], int uWorld, int uItemNumber, int uPostMoney01, int uPostMoney02, int uCostMoney01, int uCostMoney02, char aName[MAX_AVATAR_NAME_LENGTH], int aTribe, int  aLevel, int uZoneNumber);

};
//INSTANCE
extern MyGameLog mGAMELOG;
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------------
