//-------------------------------------------------------------------------------------------------
//HEADER
//-------------------------------------------------------------------------------------------------
#include "H01_MainApplication.h"

//-------------------------------------------------------------------------------------------------
// 2008.11.18 Kim SungSoo
// Intro : 1. include header for linux c++
//-------------------------------------------------------------------------------------------------
#include "global.h"
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_FOR_MY_AMERICA_COM
//-------------------------------------------------------------------------------------------------
//INSTANCE
CCashCom mCASH_COM;
//CONNECT
BOOL CCashCom::Connect( void )
{
	if( mCheckConnectState )
	{
		return TRUE;
	}

	WORD wPacketType;
	WORD wPacketSize;
	DWORD dwSeqNo;
	WORD wLoginServerNo;
	char tSendBuffer[1000];
	int tSendSize;
	int tSendValue01;
	int tSendValue02;
	int tSendValue03;
	char tRecvBuffer[1000];
	int tRecvSize;
	int tRecvCheck;
	BYTE bResult;

	mSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if( mSocket < 0 )
	{
		return FALSE;
	}
	mAddress.sin_family = AF_INET;
	mAddress.sin_addr.s_addr = inet_addr( mIP );
	mAddress.sin_port = htons( mPort );
	if( connect( mSocket, LPSOCKADDR&mAddress, sizeof( SOCKADDR_IN ) ) < 0 )
	{
		closesocket( mSocket );
		return FALSE;
	}
	mCheckConnectState = TRUE;
	wPacketType = 10;
	wPacketSize = 10;
	dwSeqNo = 0;
	wLoginServerNo = (WORD) mSERVER_INFO.mWorldNumber;
	CopyMemory( &tSendBuffer[0], &wPacketType, 2 );
	CopyMemory( &tSendBuffer[2], &wPacketSize, 2 );
	CopyMemory( &tSendBuffer[4], &dwSeqNo, 4 );
	CopyMemory( &tSendBuffer[8], &wLoginServerNo, 2 );
	tSendSize = 10;
	tSendValue01 = tSendSize;
	tSendValue02 = 0;
	while( tSendValue01 > 0 )
	{
		tSendValue03 = send( mSocket, &tSendBuffer[tSendValue02], tSendValue01, 0 );
		if( tSendValue03 < 0 )
		{
			Close();
			return FALSE;
		}
		tSendValue01 -= tSendValue03;
		tSendValue02 += tSendValue03;
	}
	tRecvSize = 0;
	while( TRUE )
	{
		tRecvCheck = recv( mSocket, &tRecvBuffer[tRecvSize], ( 1000 - tRecvSize ), 0 );
		if( tRecvCheck < 1 )
		{
			Close();
			return FALSE;
		}
		tRecvSize += tRecvCheck;
		if( tRecvSize == 9 )
		{
			break;
		}
	}
	CopyMemory( &wPacketType, &tRecvBuffer[0], 2 );
	CopyMemory( &wPacketSize, &tRecvBuffer[2], 2 );
	CopyMemory( &dwSeqNo, &tRecvBuffer[4], 4 );
	CopyMemory( &bResult, &tRecvBuffer[8], 1 );
	if( bResult != 0 )
	{
		Close();
		return FALSE;
	}
	return TRUE;
}
//CLOSE
void CCashCom::Close( void )
{
	if( !mCheckConnectState )
	{
		return;
	}
	mCheckConnectState = FALSE;
	closesocket( mSocket );
}
//INIT
BOOL CCashCom::Init( void )
{
	mCheckConnectState = FALSE;
	strcpy(mIP, g_env.cash_addr_str_obj.c_str());
	mPort = g_env.cash_port ;
	if( !Connect() )
	{
		return FALSE;
	}
	return TRUE;
}
//FREE
void CCashCom::Free( void )
{
	Close();
}
//GET_CASH
int CCashCom::GetCash( char *tID )
{
	WORD wPacketType;
	WORD wPacketSize;
	DWORD dwSeqNo;
	DWORD dwIPAddr;
	char szUserID[51];
	char tSendBuffer[1000];
	int tSendSize;
	int tSendValue01;
	int tSendValue02;
	int tSendValue03;
	char tRecvBuffer[1000];
	int tRecvSize;
	int tRecvCheck;
	BYTE bResult;
	DWORD dwRemainAP;

	if( !Connect() )
	{
		return 0;
	}
	wPacketType = 20;
	wPacketSize = 63;
	dwSeqNo = 0;
	dwIPAddr = 0;
	strcpy( szUserID, tID );
	CopyMemory( &tSendBuffer[0], &wPacketType, 2 );
	CopyMemory( &tSendBuffer[2], &wPacketSize, 2 );
	CopyMemory( &tSendBuffer[4], &dwSeqNo, 4 );
	CopyMemory( &tSendBuffer[8], &dwIPAddr, 4 );
	CopyMemory( &tSendBuffer[12], &szUserID[0], 51 );
	tSendSize = 63;
	tSendValue01 = tSendSize;
	tSendValue02 = 0;
	while( tSendValue01 > 0 )
	{
		tSendValue03 = send( mSocket, &tSendBuffer[tSendValue02], tSendValue01, 0 );
		if( tSendValue03 < 0 )
		{
			Close();
			return 0;
		}
		tSendValue01 -= tSendValue03;
		tSendValue02 += tSendValue03;
	}
	tRecvSize = 0;
	while( TRUE )
	{
		tRecvCheck = recv( mSocket, &tRecvBuffer[tRecvSize], ( 1000 - tRecvSize ), 0 );
		if( tRecvCheck < 1 )
		{
			Close();
			return 0;
		}
		tRecvSize += tRecvCheck;
		if( tRecvSize == 13 )
		{
			break;
		}
	}
	CopyMemory( &wPacketType, &tRecvBuffer[0], 2 );
	CopyMemory( &wPacketSize, &tRecvBuffer[2], 2 );
	CopyMemory( &dwSeqNo, &tRecvBuffer[4], 4 );
	CopyMemory( &bResult, &tRecvBuffer[8], 1 );
	CopyMemory( &dwRemainAP, &tRecvBuffer[9], 4 );
	if( bResult != 0 )
	{
		Close();
		return 0;
	}
	return (int) dwRemainAP;
}
//CHANGE_CASH
int CCashCom::ChangeCash( char *tID, int tCostInfoIndex, char *tSellNo )
{
	WORD wPacketType;
	WORD wPacketSize;
	DWORD dwSeqNo;
	DWORD dwIPAddr;
	char szUserID[51];
	char szItemKey[51];
	DWORD dwPrice;
	char tSendBuffer[1000];
	int tSendSize;
	int tSendValue01;
	int tSendValue02;
	int tSendValue03;
	char tRecvBuffer[1000];
	int tRecvSize;
	int tRecvCheck;
	BYTE bResult;
	DWORD dwRemainAP;
	char szPurchaseNo[16];

	if( !Connect() )
	{
		return 0;
	}
	wPacketType = 30;
	wPacketSize = 169;
	dwSeqNo = 0;
	dwIPAddr = 0;
	strcpy( szUserID, tID );
	sprintf(szItemKey, "%d", mGAME.mCostInfoValue[tCostInfoIndex][1]);
	dwPrice = (DWORD) mGAME.mCostInfoValue[tCostInfoIndex][0];
	CopyMemory( &tSendBuffer[0], &wPacketType, 2 );
	CopyMemory( &tSendBuffer[2], &wPacketSize, 2 );
	CopyMemory( &tSendBuffer[4], &dwSeqNo, 4 );
	CopyMemory( &tSendBuffer[8], &dwIPAddr, 4 );
	CopyMemory( &tSendBuffer[12], &szUserID[0], 51 );
	CopyMemory( &tSendBuffer[63], &szItemKey[0], 51 );
	CopyMemory( &tSendBuffer[114], mGAME.mCostInfoCode[tCostInfoIndex], 51 );
	CopyMemory( &tSendBuffer[165], &dwPrice, 4 );
	LOG_TO_FILE_4("!%s, %s, %s, %d\n", szUserID, szItemKey, mGAME.mCostInfoCode[tCostInfoIndex], dwPrice);
	tSendSize = 169;
	tSendValue01 = tSendSize;
	tSendValue02 = 0;
	while( tSendValue01 > 0 )
	{
		tSendValue03 = send( mSocket, &tSendBuffer[tSendValue02], tSendValue01, 0 );
		if( tSendValue03 < 0 )
		{
			Close();
			return 0;
		}
		tSendValue01 -= tSendValue03;
		tSendValue02 += tSendValue03;
	}
	tRecvSize = 0;
	while( TRUE )
	{
        tRecvCheck = recv( mSocket, &tRecvBuffer[tRecvSize], ( 1000 - tRecvSize ), 0 );
		if( tRecvCheck < 1 )
		{
			Close();
			return 0;
		}
		tRecvSize += tRecvCheck;
		if( tRecvSize == 29 )
		{
			break;
		}
	}
	CopyMemory( &wPacketType, &tRecvBuffer[0], 2 );
	CopyMemory( &wPacketSize, &tRecvBuffer[2], 2 );
	CopyMemory( &dwSeqNo, &tRecvBuffer[4], 4 );
	CopyMemory( &bResult, &tRecvBuffer[8], 1 );
	CopyMemory( &dwRemainAP, &tRecvBuffer[9], 4 );
	CopyMemory( &szPurchaseNo[0], &tRecvBuffer[13], 16 );
	LOG_TO_FILE_3("!ChangeCash : bResult[%d], dwRemainAP[%d], szPurchaseNo[%s]\n"
			, bResult, dwRemainAP, szPurchaseNo); 
	if( bResult != 0 )
	{
		Close();
		return 0;
	}
	strcpy( tSellNo, szPurchaseNo );
	return 1;
}

int CCashCom::UsePackageItem( char *tID, int iIndex )
{
    if( !Connect() )
    {
        return 0;
    }

    // 패키지 아이템 사용 처리 - 보너스 캐쉬를 지급받는다.
    // 패킷 작성.
    const int SEND_SIZE = sizeof(BILL_PACK_USE_REQ);
    BILL_PACK_USE_REQ sendPacket;
    ::memset(&sendPacket, 0, SEND_SIZE);

    sendPacket.wPacketType = 40;

    sendPacket.wPacketSize = SEND_SIZE;

	::strcpy(sendPacket.szUserID, tID);

	::sprintf(sendPacket.szItemKey, "%d", iIndex);

    char tSendBuffer[1000] = {0};
    CopyMemory( &tSendBuffer[0], &sendPacket, SEND_SIZE);

    // send.
	int tSendValue01 = SEND_SIZE;
	int tSendValue02 = 0;
	int tSendValue03 = 0;

    while( tSendValue01 > 0 )
    {
        tSendValue03 = send( mSocket, &tSendBuffer[tSendValue02], tSendValue01, 0 );
        if( tSendValue03 < 0 )
        {
            Close();
            return 0;
        }
        tSendValue01 -= tSendValue03;
        tSendValue02 += tSendValue03;
    }

    // recv.
    const int RECV_SIZE = sizeof(BILL_PACK_USE_REP);

    char tRecvBuffer[1000] = {0};
    int tRecvSize = 0;
    int tRecvCheck = 0;
    while( TRUE )
    {
        tRecvCheck = recv( mSocket, &tRecvBuffer[tRecvSize], ( 1000 - tRecvSize ), 0 );
        if( tRecvCheck < 1 )
        {
            Close();
            return 0;
        }
        tRecvSize += tRecvCheck;
        if( tRecvSize == RECV_SIZE )
        {
            break;
        }
        if ((tRecvSize > RECV_SIZE) || (tRecvSize == 0))
        {
        	LOG_TO_FILE_3("!UsePackageItem : recv fail - tID(%s) tRecvSize(%d), RECV_SIZE(%d)\n", tID, tRecvSize, RECV_SIZE);
        }
    }
    BILL_PACK_USE_REP recvPacket;
    CopyMemory( &recvPacket, &tRecvBuffer[0], RECV_SIZE);

	LOG_TO_FILE_3("!UsePackageItem : tID(%s) - bResult[%d], dwRemainGC[%d]\n", tID, recvPacket.bResult, recvPacket.dwRemainGC); 
	if( recvPacket.bResult != 0 )
	{
		Close();
		return 0;
	}

    return 1;
}
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_GAMELOG
//-------------------------------------------------------------------------------------------------
//INSTANCE
MyGameLog mGAMELOG;
//CREATE
MyGameLog::MyGameLog( void )
{
}

//INIT
BOOL MyGameLog::Init( char tIP[16], int tPort )
{
	//LOG_TO_FILE_2("---> %s %d\n", tIP, tPort);
	mCheckConnectState = FALSE;
	mSocket = socket( PF_INET, SOCK_DGRAM, 0 );
	if( mSocket < 0 ) {
		return FALSE;
	}
	ZeroMemory( &mAddress, sizeof( mAddress ) );
	mAddress.sin_family = AF_INET;
	mAddress.sin_addr.s_addr = inet_addr( tIP );
	mAddress.sin_port = htons( tPort );
	
	if( connect( mSocket, LPSOCKADDR&mAddress, sizeof( SOCKADDR_IN ) ) < 0 ) {
		closesocket( mSocket );
		return FALSE;
	}
	mCheckConnectState = TRUE;
	return TRUE;
}

//FREE
void MyGameLog::Free( void )
{
	if( !mCheckConnectState )
	{
		return;
	}
	mCheckConnectState = FALSE;
	closesocket( mSocket );
}
//CHECK_VALID_STATE
BOOL MyGameLog::CheckValidState( char tID[MAX_USER_ID_LENGTH] )
{
	return TRUE;
}
//SEND_TO_LOG_SERVER
void MyGameLog::SendToLogServer( void )
{
	int tTotalPacketSize;

	tTotalPacketSize = 5 + strlen( mGameLogData );
	CopyMemory( &mSendBuffer[0], &tTotalPacketSize, 4 );
	CopyMemory( &mSendBuffer[4], &mGameLogData[0], ( strlen( mGameLogData ) + 1 ) );
	send( mSocket, &mSendBuffer[0], tTotalPacketSize, 0 );
}
//SEND_TO_LOG_SERVER_FOR_DB
void MyGameLog::SendToLogServerForDB( void )
{
	int tTotalPacketSize;
	int tTotalQuerySize;

	tTotalPacketSize = -1;
	tTotalQuerySize = strlen( mGameLogData ) + 1;
	CopyMemory( &mSendBuffer[0], &tTotalPacketSize, 4 );
	CopyMemory( &mSendBuffer[4], &tTotalQuerySize, 4 );
	CopyMemory( &mSendBuffer[8], &mGameLogData[0], ( strlen( mGameLogData ) + 1 ) );
	send( mSocket, &mSendBuffer[0], ( tTotalQuerySize + 8 ), 0 );
}
//GL_000_ITEM_LOG
void MyGameLog::GL_000_ITEM_LOG( char uID[MAX_USER_ID_LENGTH], int uWorld, int uItemNumber,
		int uPostMoney01, int uPostMoney02, int uCostMoney01, int uCostMoney02
		, char aName[MAX_AVATAR_NAME_LENGTH], int aTribe, int  aLevel, int uZoneNumber)
{
	struct tm *tm_ptr = NULL;
	time_t r_time = ::time (NULL);
	if( !CheckValidState( &uID[0] ) ) {
		return;
	}
	//sprintf( mGameLogData, "insert into %s values ( getdate(),'%s',%d,%d,%d,%d,%d,%d,'%s',%d,%d )", mSERVER_INFO.mDB_3_Table01
	//, uID, uWorld, uItemNumber, uPostMoney01, uPostMoney02, uCostMoney01, uCostMoney02, aName, aTribe, aLevel );
	tm_ptr = ::localtime(&r_time);
#if 0
	sprintf( mGameLogData, 
			"INSERT INTO %s_%04d%02d%02d "
			"	(UserNo, GameID, Item_Code, LogTime "
			"	, PostMoney01, PostMoney02, BuyMoney01, BuyMoney02 "
			"	, ZoneNumber, WorldNumber, Class, GameLevel) "
			"VALUES('%s', '%s', %d, now() "
			"	, %d, %d, %d, %d "
			"	, %d, %d, %d, %d) "
			, mSERVER_INFO.mDB_3_Table01
			, tm_ptr->tm_year + 1900, tm_ptr->tm_mon + 1, tm_ptr->tm_mday
			, uID, aName, uItemNumber
			, uPostMoney01, uPostMoney02, uCostMoney01, uCostMoney02
		, uZoneNumber, uWorld, aTribe, aLevel);
#endif
#ifdef __TK__
	sprintf( mGameLogData, 
			"INSERT INTO %s_%04d%02d%02d "
			"VALUES('%s', '%s', %d, ADDTIME(UTC_TIMESTAMP(), '03:00:00') "
			"	, %d, %d, %d, %d "
			"	, %d, %d, %d, %d) "
			, mSERVER_INFO.mDB_3_Table01
			, tm_ptr->tm_year + 1900, tm_ptr->tm_mon + 1, tm_ptr->tm_mday
			, uID, aName, uItemNumber
			, uPostMoney01, uPostMoney02, uCostMoney01, uCostMoney02
		, uZoneNumber, uWorld, aTribe, aLevel);
#else
	sprintf( mGameLogData, 
			"INSERT INTO %s_%04d%02d%02d "
			"VALUES('%s', '%s', %d, now() "
			"	, %d, %d, %d, %d "
			"	, %d, %d, %d, %d) "
			, mSERVER_INFO.mDB_3_Table01
			, tm_ptr->tm_year + 1900, tm_ptr->tm_mon + 1, tm_ptr->tm_mday
			, uID, aName, uItemNumber
			, uPostMoney01, uPostMoney02, uCostMoney01, uCostMoney02
			, uZoneNumber, uWorld, aTribe, aLevel);
#endif
	//LOG_TO_FILE_1("===> %s\n", mGameLogData);
	SendToLogServerForDB();
}
//-------------------------------------------------------------------------------------------------
