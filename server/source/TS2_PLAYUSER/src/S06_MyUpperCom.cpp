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
	if( mSocket < 0 )
	{
		return FALSE;
	}
	ZeroMemory( &mAddress, sizeof( mAddress ) );
	mAddress.sin_family = AF_INET;
	mAddress.sin_addr.s_addr = inet_addr( tIP );
	mAddress.sin_port = htons( tPort );
	if( connect( mSocket, LPSOCKADDR&mAddress, sizeof( SOCKADDR_IN ) ) < 0 )
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
//GL_000_PLAY_LOG
void MyGameLog::GL_000_PLAY_LOG( int uLogType, int uPlayTime, char uID[MAX_USER_ID_LENGTH], char uIP[16], int uLoginPremiumPCRoom, int uWorld, char aName[MAX_AVATAR_NAME_LENGTH], int aTribe, int  aLevel, int aExperience, int aKillOtherTribe, int aMoney, int aStoreMoney, int uZoneNumber )
{
	struct tm *tm_ptr = NULL;
	time_t r_time = ::time(NULL);
	if( !CheckValidState( &uID[0] ) )
	{
		return;
	}
	//sprintf( mGameLogData, "insert into %s values ( %d,getdate(),%d,'%s','%s',%d,%d,'%s',%d,%d,%d,%d,%d,%d )", mSERVER_INFO.mDB_3_Table01, uLogType, uPlayTime, uID, uIP, uLoginPremium, uWorld, aName, aTribe, aLevel, aExperience, aKillOtherTribe, aMoney, aStoreMoney );
	tm_ptr = ::localtime(&r_time);
	sprintf( mGameLogData, 
			"INSERT INTO %s_%04d%02d%02d "
			"VALUES('%s', '%s', %d, %d "
#ifdef __TK__
			"	, '%s', %d, ADDTIME(UTC_TIMESTAMP(), '03:00:00'), %d "
#else
			"	, '%s', %d, now(), %d "
#endif
			"	, %d, %d, %d, %d "
			"	, %d, %d, %d) "
			, mSERVER_INFO.mDB_3_Table01
			, tm_ptr->tm_year + 1900, tm_ptr->tm_mon + 1, tm_ptr->tm_mday
			, uID, aName, uZoneNumber, uWorld
			, uIP, uLogType, uPlayTime
			, aLevel, aExperience, aTribe, aKillOtherTribe
			, aMoney, aStoreMoney, uLoginPremiumPCRoom);

#if 0
	sprintf( mGameLogData, 
			"INSERT INTO %s_%04d%02d%02d "
			"	(UserNo, GameID, ZoneNumber, WorldNumber "
			"	, UserIP, LogType, LoginTime, GameTime "
			"	, Lv, Exp, Win, Lose "
			"	, Score, Point, PCBang) "
			"VALUES ('%s', '%s', %d, %d "
			"	, '%s', %d, now(), %d "
			"	, %d, %d, %d, %d "
			"	, %d, %d, %d) "
			, mSERVER_INFO.mDB_3_Table01
			, tm_ptr->tm_year + 1900, tm_ptr->tm_mon + 1, tm_ptr->tm_mday
			, uID, aName, uZoneNumber, uWorld
			, uIP, uLogType, uPlayTime
			, aLevel, aExperience, aTribe, aKillOtherTribe
			, aMoney, aStoreMoney, uLoginPremiumPCRoom);
#endif
	SendToLogServerForDB();
}
//GL_101_LOGIN
void MyGameLog::GL_101_LOGIN( char tID[MAX_USER_ID_LENGTH], char tIP[16] )
{
	if( !CheckValidState( &tID[0] ) )
	{
		return;
	}
	mLogSort = 101;
	sprintf( mGameLogData, "[%d]<LOGIN>(%s),(%s),[[0]]", mLogSort, tID, tIP );
	SendToLogServer();
}
//GL_102_LOGOUT_1
void MyGameLog::GL_102_LOGOUT_1( char tID[MAX_USER_ID_LENGTH] )
{
	if( !CheckValidState( &tID[0] ) )
	{
		return;
	}
	mLogSort = 102;
	sprintf( mGameLogData, "[%d]<LOGOUT_1>(%s),[[0]]", mLogSort, tID );
	SendToLogServer();
}
//GL_103_WAIT_LOGOUT_1
void MyGameLog::GL_103_WAIT_LOGOUT_1( char tID[MAX_USER_ID_LENGTH] )
{
	if( !CheckValidState( &tID[0] ) )
	{
		return;
	}
	mLogSort = 103;
	sprintf( mGameLogData, "[%d]<WAIT_LOGOUT_1>(%s),[[0]]", mLogSort, tID );
	SendToLogServer();
}
//GL_104_ENTER_ZONE
void MyGameLog::GL_104_ENTER_ZONE( char tID[MAX_USER_ID_LENGTH], char tAvatarName[MAX_AVATAR_NAME_LENGTH], int tZoneNumber )
{
	if( !CheckValidState( &tID[0] ) )
	{
		return;
	}
	mLogSort = 104;
	sprintf( mGameLogData, "[%d]<ENTER_ZONE>(%s),(%s),[[%d]]", mLogSort, tID, tAvatarName, tZoneNumber );
	SendToLogServer();
}
//GL_105_MOVE_ZONE
void MyGameLog::GL_105_MOVE_ZONE( char tID[MAX_USER_ID_LENGTH], char tAvatarName[MAX_AVATAR_NAME_LENGTH], int tZoneNumber )
{
	if( !CheckValidState( &tID[0] ) )
	{
		return;
	}
	mLogSort = 105;
	sprintf( mGameLogData, "[%d]<MOVE_ZONE>(%s),(%s),[[%d]]", mLogSort, tID, tAvatarName, tZoneNumber );
	SendToLogServer();
}
//GL_106_LOGOUT_2
void MyGameLog::GL_106_LOGOUT_2( char tID[MAX_USER_ID_LENGTH], char tAvatarName[MAX_AVATAR_NAME_LENGTH], int tZoneNumber )
{
	if( !CheckValidState( &tID[0] ) )
	{
		return;
	}
	mLogSort = 106;
	sprintf( mGameLogData, "[%d]<LOGOUT_2>(%s),(%s),[[%d]]", mLogSort, tID, tAvatarName, tZoneNumber );
	SendToLogServer();
}
//GL_107_WAIT_LOGOUT_2
void MyGameLog::GL_107_WAIT_LOGOUT_2( char tID[MAX_USER_ID_LENGTH], char tAvatarName[MAX_AVATAR_NAME_LENGTH], int tZoneNumber )
{
	if( !CheckValidState( &tID[0] ) )
	{
		return;
	}
	mLogSort = 107;
	sprintf( mGameLogData, "[%d]<WAIT_LOGOUT_2>(%s),(%s),[[%d]]", mLogSort, tID, tAvatarName, tZoneNumber );
	SendToLogServer();
}
//-------------------------------------------------------------------------------------------------
