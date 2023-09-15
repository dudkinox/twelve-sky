//-------------------------------------------------------------------------------------------------
//HEADER
//-------------------------------------------------------------------------------------------------
#include "H01_MainApplication.h"
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
	mCheckConnectState = FALSE;
	mSocket = socket( PF_INET, SOCK_DGRAM, 0 );
	if (mSocket < 0)
	{
		return FALSE;
	}
	ZeroMemory( &mAddress, sizeof( mAddress ) );
	mAddress.sin_family = AF_INET;
	mAddress.sin_addr.s_addr = inet_addr( tIP );
	mAddress.sin_port = htons( tPort );
	if (connect (mSocket, LPSOCKADDR&mAddress, sizeof (SOCKADDR_IN)) < 0)
	{
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
//GL_201_START_TRIBE_SYMBOL_BATTLE
void MyGameLog::GL_201_START_TRIBE_SYMBOL_BATTLE( void )
{
	mLogSort = 201;
	sprintf( mGameLogData, "[%d]<START_TRIBE_SYMBOL_BATTLE>", mLogSort );
	SendToLogServer();
}
//GL_202_END_TRIBE_SYMBOL_BATTLE
void MyGameLog::GL_202_END_TRIBE_SYMBOL_BATTLE( void )
{
	mLogSort = 202;
	sprintf( mGameLogData, "[%d]<END_TRIBE_SYMBOL_BATTLE>", mLogSort );
	SendToLogServer();
}
//-------------------------------------------------------------------------------------------------
