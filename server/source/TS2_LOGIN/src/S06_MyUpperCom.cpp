//-------------------------------------------------------------------------------------------------
//HEADER
//-------------------------------------------------------------------------------------------------
#include "H01_MainApplication.h"
#include <signal.h>
#include <unistd.h>
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_PLAYUSER_COM
//-------------------------------------------------------------------------------------------------

#define BLOCK_SIGNAL() {\
    if (::sigprocmask (SIG_BLOCK, &signew, &sigold) < 0) { \
        LOG_TO_FILE_2 ("![%s] : sigprocmask (%s)\n", __FUNCTION__, ::strerror (errno)); \
        return FALSE; \
    } \
}

#define UNBLOCK_SIGNAL() {\
    if (::sigprocmask (SIG_UNBLOCK, &sigold, NULL) < 0) { \
        LOG_TO_FILE_2 ("![%s] : sigprocmask (%s)\n", __FUNCTION__, ::strerror (errno)); \
        return FALSE; \
    } \
}

//INSTANCE
MyPlayUserCom mPLAYUSER_COM;
//CREATE
MyPlayUserCom::MyPlayUserCom( void )
{
}
//CONNECT
BOOL MyPlayUserCom::Connect( void )
{
	if( mCheckConnectState )
	{
		return TRUE;
	}

	int tConnectRecvCheck;
	char tConnectDataCheck;
	char tSendData[1000];
	int tSendDataSize;
	int tSendValue01;
	int tSendValue02;
	int tSendValue03;
	int tOKRecvCheck;
	char tOKDataCheck;

	mSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if( mSocket < 0 )
	{
		return FALSE;
	}
	mAddress.sin_family = AF_INET;
	mAddress.sin_addr.s_addr = inet_addr( mIP );
	mAddress.sin_port = htons( static_cast<uint16_t> (mPort));
	if (connect (mSocket, (sockaddr*) &mAddress, sizeof (sockaddr_in)) < 0 )
	{
		closesocket( mSocket );
		return FALSE;
	}
	tConnectRecvCheck = recv( mSocket, &tConnectDataCheck, 1, 0 );
	if( tConnectRecvCheck != 1 )
	{
		closesocket( mSocket );
		return FALSE;
	}
	tSendData[0] = P_LOGIN_FOR_PLAYUSER_SEND;
	tSendDataSize = S_LOGIN_FOR_PLAYUSER_SEND;
	tSendValue01 = tSendDataSize;
	tSendValue02 = 0;
	while( tSendValue01 > 0 )
	{
		tSendValue03 = send( mSocket, &tSendData[tSendValue02], tSendValue01, 0 );
		if (tSendValue03 <= 0) {
			if(errno == EINTR)
				continue;
			closesocket (mSocket);
			return FALSE;
		}
		tSendValue01 -= tSendValue03;
		tSendValue02 += tSendValue03;
	}
	tOKRecvCheck = recv( mSocket, &tOKDataCheck, 1, 0 );
	if( tOKRecvCheck != 1 )
	{
		closesocket( mSocket );
		return FALSE;
	}
	mCheckConnectState = TRUE;

#if 0
    int old_flag = 0;
    old_flag = ::fcntl (mSocket, F_GETFL);
    ::fcntl (mSocket, F_SETFL, old_flag | O_NONBLOCK);
#endif

	return TRUE;
}
//SEND
BOOL MyPlayUserCom::Send( void )
{
	int tSendValue01;
	int tSendValue02;
	int tSendValue03;

	tSendValue01 = mSendDataSize;
	tSendValue02 = 0;
	while( tSendValue01 > 0 )
	{
		tSendValue03 = send( mSocket, &mSendData[tSendValue02], tSendValue01, 0 );
        if (tSendValue03 < 0) {
			if(errno == EINTR)
				continue;
            //if (errno != EAGAIN && errno != EWOULDBLOCK) {
			Close ();
			return FALSE;
			//}
            //continue;
        } else if (tSendValue03 == 0) {
            Close ();
            return FALSE;
        }
		tSendValue01 -= tSendValue03;
		tSendValue02 += tSendValue03;
	}
	return TRUE;
}
//RECV
BOOL MyPlayUserCom::Recv( void )
{
    fd_set readfds;
    int iResult = 0;
	int tRecvValue01;
	int tRecvValue02;

	if( mRecvDataSize > 0 )
	{
		tRecvValue01 = 0;
		while( TRUE )
		{
#if 0
            FD_ZERO (&readfds);
            FD_SET (mSocket, &readfds);
            struct timeval tv = {2, 0};

            if ((iResult = ::select (mSocket + 1, &readfds, NULL, NULL, &tv)) < 0) {
                if (errno == EINTR) continue;
                Close ();
                return FALSE;
            }

            if (iResult == 0) {
                LOG_TO_FILE ("!Tired of waiting for an answer from the PlayUser server to give up.\n");
                LOG_TO_FILE_2 ("\t\t\tin (%s - %d).\n", __FILE__, __LINE__);
                Close ();
                return FALSE;
            }

            if (FD_ISSET (mSocket, &readfds)) { 
#endif
                tRecvValue02 = ::recv (mSocket, &mRecvData[tRecvValue01], mRecvDataSize - tRecvValue01, 0);
    			if( tRecvValue02 < 0 )
    			{
					if(errno == EINTR)
						continue;
    				//if (errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR) {
					Close();
					return FALSE;
					//}
                    //continue;
    			} else if (tRecvValue02 == 0) {
                    Close ();
                    return FALSE;
                }
			    tRecvValue01 += tRecvValue02;
			    if( tRecvValue01 == mRecvDataSize )
			    {
			    	break;
			    }
#if 0
		    }
#endif
	    }
    }

	return TRUE;
}
//CLOSE
void MyPlayUserCom::Close( void )
{
	if( !mCheckConnectState )
	{
		return;
	}
	mCheckConnectState = FALSE;
	closesocket( mSocket );
}
//INIT
BOOL MyPlayUserCom::Init( char tIP[16], int tPort )
{
	mCheckConnectState = FALSE;
	strcpy( mIP, tIP );
	mPort = tPort;
	if( !Connect() )
	{
		return FALSE;
	}
	return TRUE;
}
//FREE
void MyPlayUserCom::Free( void )
{
	Close();
}
//PROCESS_FOR_NETWORK
BOOL MyPlayUserCom::ProcessForNetwork( void )
{
    sigset_t signew, sigold;
    ::sigfillset (&signew);

    // 보내고 받는 과정에 발생하는 시그널은 차단합니다.
    BLOCK_SIGNAL ();

	if( !Connect() )
	{
        UNBLOCK_SIGNAL ();
		return FALSE;
	}
	if( !Send() )
	{
        UNBLOCK_SIGNAL ();
		return FALSE;
	}
	if( !Recv() )
	{
        UNBLOCK_SIGNAL ();
		return FALSE;
	}

    UNBLOCK_SIGNAL ();
	return TRUE;
}
//U_LOGIN_OK_FOR_PLAYUSER_SEND
void MyPlayUserCom::U_LOGIN_OK_FOR_PLAYUSER_SEND( void )
{
	mSendData[0] = P_LOGIN_OK_FOR_PLAYUSER_SEND;
	mSendDataSize = S_LOGIN_OK_FOR_PLAYUSER_SEND;
	mRecvDataSize = 0;
	while( TRUE )
	{
		if( ProcessForNetwork() )
		{
			break;
		}
	}
}
//U_REGISTER_USER_FOR_LOGIN_1_SEND
void MyPlayUserCom::U_REGISTER_USER_FOR_LOGIN_1_SEND( char tIP[16], char tID[MAX_USER_ID_LENGTH], int tUserSort, int tGoodFellow, int tLoginPlace, int tLoginPremium, int tLoginPremiumPCRoom, int tTraceState, char tBonus100Money, int tPremiumServerExpirationDate, AUTH_INFO *tAuthInfo ) // 함수 선언 변경 - int tPremiumServerExpirationDate 추가. // @_Premium_Server_@
{
	mSendData[0] = P_REGISTER_USER_FOR_LOGIN_1_SEND;
	CopyMemory( &mSendData[1], &tIP[0], 16 );
	CopyMemory( &mSendData[17], &tID[0], MAX_USER_ID_LENGTH );
	CopyMemory( &mSendData[( 17 + MAX_USER_ID_LENGTH )], &tUserSort, 4 );
	CopyMemory( &mSendData[( 21 + MAX_USER_ID_LENGTH )], &tGoodFellow, 4 );
	CopyMemory( &mSendData[( 25 + MAX_USER_ID_LENGTH )], &tLoginPlace, 4 );
	CopyMemory( &mSendData[( 29 + MAX_USER_ID_LENGTH )], &tLoginPremium, 4 );
	CopyMemory( &mSendData[( 33 + MAX_USER_ID_LENGTH )], &tLoginPremiumPCRoom, 4 );
	CopyMemory( &mSendData[( 37 + MAX_USER_ID_LENGTH )], &tTraceState, 4 );
	CopyMemory( &mSendData[( 41 + MAX_USER_ID_LENGTH )], &tBonus100Money, 1 );
	CopyMemory( &mSendData[( 42 + MAX_USER_ID_LENGTH )], &tPremiumServerExpirationDate, 4 ); // @_Premium_Server_@
	CopyMemory( &mSendData[( 46 + MAX_USER_ID_LENGTH )], tAuthInfo, sizeof(AUTH_INFO) );
	mSendDataSize = S_REGISTER_USER_FOR_LOGIN_1_SEND;
	mRecvDataSize = S_REGISTER_USER_FOR_LOGIN_RECV;
	while( TRUE )
	{
		if( ProcessForNetwork() )
		{
			break;
		}
	}
	CopyMemory( &mRecv_Result, &mRecvData[1], 4 );
	CopyMemory( &mRecv_PlayUserIndex, &mRecvData[5], 4 );
}
//U_REGISTER_USER_FOR_LOGIN_2_SEND
void MyPlayUserCom::U_REGISTER_USER_FOR_LOGIN_2_SEND( int tPlayUserIndex, char tID[MAX_USER_ID_LENGTH] )
{
	mSendData[0] = P_REGISTER_USER_FOR_LOGIN_2_SEND;
	CopyMemory( &mSendData[1], &tPlayUserIndex, 4 );
	CopyMemory( &mSendData[5], &tID[0], MAX_USER_ID_LENGTH );
	mSendDataSize = S_REGISTER_USER_FOR_LOGIN_2_SEND;
	mRecvDataSize = S_REGISTER_USER_FOR_LOGIN_RECV;
	while( TRUE )
	{
		if( ProcessForNetwork() )
		{
			break;
		}
	}
	CopyMemory( &mRecv_Result, &mRecvData[1], 4 );
	CopyMemory( &mRecv_PlayUserIndex, &mRecvData[5], 4 );
}
//U_REGISTER_USER_FOR_LOGIN_3_SEND
void MyPlayUserCom::U_REGISTER_USER_FOR_LOGIN_3_SEND( int tPlayUserIndex, char tID[MAX_USER_ID_LENGTH], AVATAR_INFO *tAvatarInfo )
{
	mSendData[0] = P_REGISTER_USER_FOR_LOGIN_3_SEND;
	CopyMemory( &mSendData[1], &tPlayUserIndex, 4 );
	CopyMemory( &mSendData[5], &tID[0], MAX_USER_ID_LENGTH );
	CopyMemory( &mSendData[( 5 + MAX_USER_ID_LENGTH )], tAvatarInfo, sizeof( AVATAR_INFO ) );
	mSendDataSize = S_REGISTER_USER_FOR_LOGIN_3_SEND;
	mRecvDataSize = S_REGISTER_USER_FOR_LOGIN_RECV;
	while( TRUE )
	{
		if( ProcessForNetwork() )
		{
			break;
		}
	}
	CopyMemory( &mRecv_Result, &mRecvData[1], 4 );
	CopyMemory( &mRecv_PlayUserIndex, &mRecvData[5], 4 );
}
//U_UNREGISTER_USER_FOR_LOGIN_SEND
void MyPlayUserCom::U_UNREGISTER_USER_FOR_LOGIN_SEND( int tPlayUserIndex, char tID[MAX_USER_ID_LENGTH] )
{
	mSendData[0] = P_UNREGISTER_USER_FOR_LOGIN_SEND;
	CopyMemory( &mSendData[1], &tPlayUserIndex, 4 );
	CopyMemory( &mSendData[5], &tID[0], MAX_USER_ID_LENGTH );
	mSendDataSize = S_UNREGISTER_USER_FOR_LOGIN_SEND;
	mRecvDataSize = S_UNREGISTER_USER_FOR_LOGIN_RECV;
	while( TRUE )
	{
		if( ProcessForNetwork() )
		{
			break;
		}
	}
}
//U_GET_PRESENT_USER_NUM_FOR_LOGIN_SEND
void MyPlayUserCom::U_GET_PRESENT_USER_NUM_FOR_LOGIN_SEND( void )
{
	mSendData[0] = P_GET_PRESENT_USER_NUM_FOR_LOGIN_SEND;
	mSendDataSize = S_GET_PRESENT_USER_NUM_FOR_LOGIN_SEND;
	mRecvDataSize = S_GET_PRESENT_USER_NUM_FOR_LOGIN_RECV;
	while( TRUE )
	{
		if( ProcessForNetwork() )
		{
			break;
		}
	}
	CopyMemory( &mRecv_PresentUserNum, &mRecvData[1], 4 );
}
//U_BLOCK_USER_FOR_LOGIN_1_SEND
void MyPlayUserCom::U_BLOCK_USER_FOR_LOGIN_1_SEND( int tSort, char tID[MAX_USER_ID_LENGTH] )
{
	mSendData[0] = P_BLOCK_USER_FOR_LOGIN_1_SEND;
	CopyMemory( &mSendData[1], &tSort, 4 );
	CopyMemory( &mSendData[5], &tID[0], MAX_USER_ID_LENGTH );
	mSendDataSize = S_BLOCK_USER_FOR_LOGIN_1_SEND;
	mRecvDataSize = S_BLOCK_USER_FOR_LOGIN_RECV;
	while( TRUE )
	{
		if( ProcessForNetwork() )
		{
			break;
		}
	}
	CopyMemory( &mRecv_Result, &mRecvData[1], 4 );
}
//U_BLOCK_USER_FOR_LOGIN_2_SEND
void MyPlayUserCom::U_BLOCK_USER_FOR_LOGIN_2_SEND( int tSort, char tID[MAX_USER_ID_LENGTH] )
{
	mSendData[0] = P_BLOCK_USER_FOR_LOGIN_2_SEND;
	CopyMemory( &mSendData[1], &tSort, 4 );
	CopyMemory( &mSendData[5], &tID[0], MAX_USER_ID_LENGTH );
	mSendDataSize = S_BLOCK_USER_FOR_LOGIN_2_SEND;
	mRecvDataSize = S_BLOCK_USER_FOR_LOGIN_RECV;
	while( TRUE )
	{
		if( ProcessForNetwork() )
		{
			break;
		}
	}
	CopyMemory( &mRecv_Result, &mRecvData[1], 4 );
}

//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_CENTER_COM
//-------------------------------------------------------------------------------------------------
//INSTANCE
MyCenterCom mCENTER_COM;
//CREATE
MyCenterCom::MyCenterCom( void )
{
}
//CONNECT
BOOL MyCenterCom::Connect( void )
{
	if( mCheckConnectState )
	{
		return TRUE;
	}

	int tConnectRecvCheck;
	char tConnectDataCheck;
	char tSendData[1000];
	int tSendDataSize;
	int tSendValue01;
	int tSendValue02;
	int tSendValue03;
	int tOKRecvCheck;
	char tOKDataCheck;

	mSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if( mSocket < 0 )
	{
		return FALSE;
	}
	mAddress.sin_family = AF_INET;
	mAddress.sin_addr.s_addr = inet_addr( mIP );
	mAddress.sin_port = htons( mPort );
	if (connect (mSocket, (sockaddr*) &mAddress, sizeof (sockaddr_in)) < 0) 
	{
		closesocket( mSocket );
		return FALSE;
	}
	tConnectRecvCheck = recv( mSocket, &tConnectDataCheck, 1, 0 );
	if( tConnectRecvCheck != 1 )
	{
		closesocket( mSocket );
		return FALSE;
	}
	tSendData[0] = P_LOGIN_FOR_CENTER_SEND;
	tSendDataSize = S_LOGIN_FOR_CENTER_SEND;
	tSendValue01 = tSendDataSize;
	tSendValue02 = 0;
	while( tSendValue01 > 0 )
	{
		tSendValue03 = send( mSocket, &tSendData[tSendValue02], tSendValue01, 0 );
        if (tSendValue03 <= 0) {
			if(errno == EINTR)
				continue;
            closesocket (mSocket);
            return FALSE;
        }
		tSendValue01 -= tSendValue03;
		tSendValue02 += tSendValue03;
	}
	tOKRecvCheck = recv( mSocket, &tOKDataCheck, 1, 0 );
	if( tOKRecvCheck != 1 )
	{
		closesocket( mSocket );
		return FALSE;
	}
	mCheckConnectState = TRUE;
	return TRUE;
}
//SEND
BOOL MyCenterCom::Send( void )
{
	int tSendValue01;
	int tSendValue02;
	int tSendValue03;

	tSendValue01 = mSendDataSize;
	tSendValue02 = 0;
	while( tSendValue01 > 0 )
	{
		tSendValue03 = send( mSocket, &mSendData[tSendValue02], tSendValue01, 0 );
        if (tSendValue03 <= 0) {
			if(errno == EINTR)
				continue;
            Close ();
            return FALSE;
        }
		tSendValue01 -= tSendValue03;
		tSendValue02 += tSendValue03;
	}
	return TRUE;
}
//RECV
BOOL MyCenterCom::Recv( void )
{
    fd_set readfds;
    int iResult = 0;
	int tRecvValue01;
	int tRecvValue02;

	if( mRecvDataSize > 0 )
	{
		tRecvValue01 = 0;
		while( TRUE )
		{
#if 0
            FD_ZERO (&readfds);
            FD_SET (mSocket, &readfds);
            struct timeval tv = {2, 0};

            if ((iResult = ::select (mSocket + 1, &readfds, NULL, NULL, &tv)) < 0) {
                if (errno == EINTR) continue;
                Close ();
                return FALSE;
            }

            if (iResult == 0) {
                LOG_TO_FILE ("!Tired of waiting for an answer from the PlayUser server to give up.\n");
                LOG_TO_FILE_2 ("\t\t\tin (%s - %d).\n", __FILE__, __LINE__);
                Close ();
                return FALSE;
            }

            if (FD_ISSET (mSocket, &readfds)) {
#endif
                tRecvValue02 = ::recv (mSocket, &mRecvData[tRecvValue01], mRecvDataSize - tRecvValue01, 0);
			    if( tRecvValue02 <= 0 )
			    {
					if(errno == EINTR)
						continue;
				    //if (errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR) {
					Close();
					return FALSE;
					//}
			    }
			    tRecvValue01 += tRecvValue02;
			    if( tRecvValue01 == mRecvDataSize )
			    {
				    break;
			    }
#if 0
		    }
#endif
	    }
    }

	return TRUE;
}
//CLOSE
void MyCenterCom::Close( void )
{
	if( !mCheckConnectState )
	{
		return;
	}
	mCheckConnectState = FALSE;
	closesocket( mSocket );
}
//INIT
BOOL MyCenterCom::Init( char tIP[16], int tPort )
{
	mCheckConnectState = FALSE;
	strcpy( mIP, tIP );
	mPort = tPort;
	if( !Connect() )
	{
		return FALSE;
	}
	return TRUE;
}
//FREE
void MyCenterCom::Free( void )
{
	Close();
}
//PROCESS_FOR_NETWORK
BOOL MyCenterCom::ProcessForNetwork( void )
{
    sigset_t signew, sigold;
    ::sigfillset (&signew);

    // 보내고 받는 과정에 발생하는 시그널은 차단합니다.
    BLOCK_SIGNAL ();

	if( !Connect() )
	{
        UNBLOCK_SIGNAL ();
		return FALSE;
	}
	if( !Send() )
	{
        UNBLOCK_SIGNAL ();
		return FALSE;
	}
	if( !Recv() )
	{
        UNBLOCK_SIGNAL ();
		return FALSE;
	}

    UNBLOCK_SIGNAL ();
	return TRUE;
}
//U_LOGIN_OK_FOR_CENTER_SEND
void MyCenterCom::U_LOGIN_OK_FOR_CENTER_SEND( void )
{
	mSendData[0] = P_LOGIN_OK_FOR_CENTER_SEND;
	mSendDataSize = S_LOGIN_OK_FOR_CENTER_SEND;
	mRecvDataSize = 0;
	while( TRUE )
	{
		if( ProcessForNetwork() )
		{
			break;
		}
	}
}
//U_GET_ZONE_SERVER_INFO_1
void MyCenterCom::U_GET_ZONE_SERVER_INFO_1( int tZoneNumber )
{
	mSendData[0] = P_GET_ZONE_SERVER_INFO_1;
	CopyMemory( &mSendData[1], &tZoneNumber, 4 );
	mSendDataSize = S_GET_ZONE_SERVER_INFO_1;
	mRecvDataSize = S_GET_ZONE_SERVER_INFO_1_RESULT;
	while( TRUE )
	{
		if( ProcessForNetwork() )
		{
			break;
		}
	}
	CopyMemory( &mRecv_Result, &mRecvData[1], 4 );
	CopyMemory( &mRecv_IP[0], &mRecvData[5], 16 );
	CopyMemory( &mRecv_Port, &mRecvData[21], 4 );
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
	if (connect (mSocket, (sockaddr*) &mAddress, sizeof (sockaddr_in)) < 0 )
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
//GL_501_CHANGE_AVATAR_NAME
void MyGameLog::GL_501_CHANGE_AVATAR_NAME( char tID[MAX_USER_ID_LENGTH], char tAvatarName[MAX_AVATAR_NAME_LENGTH], char tChangeAvatarName[MAX_AVATAR_NAME_LENGTH] )
{
	if( !CheckValidState( &tID[0] ) )
	{
		return;
	}
	mLogSort = 501;
	sprintf( mGameLogData, "[%d]<CHANGE_AVATAR_NAME>(%s),(%s)->(%s),[[0]]", mLogSort, tID, tAvatarName, tChangeAvatarName );
	SendToLogServer();
}
//GL_502_MISMATCH_MOUSE_PASSWORD
void MyGameLog::GL_502_MISMATCH_MOUSE_PASSWORD( char tID[MAX_USER_ID_LENGTH], char tMousePassword[MAX_MOUSE_PASSWORD_LENGTH], char tInputMousePassword[MAX_MOUSE_PASSWORD_LENGTH] )
{
	if( !CheckValidState( &tID[0] ) )
	{
		return;
	}
	mLogSort = 502;
	sprintf( mGameLogData, "[%d]<MISMATCH_MOUSE_PASSWORD>(%s),(%s),(%s),[[0]]", mLogSort, tID, tMousePassword, tInputMousePassword );
	SendToLogServer();
}
//GL_503_GIFT
void MyGameLog::GL_503_GIFT( char tID[MAX_USER_ID_LENGTH], int tValue )
{
	if( !CheckValidState( &tID[0] ) )
	{
		return;
	}
	mLogSort = 503;
	sprintf( mGameLogData, "[%d]<GIFT>(%s),(%d),[[0]]", mLogSort, tID, tValue );
	SendToLogServer();
}
//GL_504_MOUSE_PASS_THREE_OUT
void MyGameLog::GL_504_MOUSE_PASS_THREE_OUT( char tID[MAX_USER_ID_LENGTH])
{
	if( !CheckValidState( &tID[0] ) )
	{
		return;
	}
	mLogSort = 504;
	sprintf( mGameLogData, "[%d]<MOUSER_PASSWORD_BLOCK>(%s),(3),[[0]]", mLogSort, tID );
	SendToLogServer();
}
// @ __WEB_MALL__
//GL_505_WEBMALL
void MyGameLog::GL_505_WEB_MALL(char tID[MAX_USER_ID_LENGTH], int *tList)
{
    if( !CheckValidState( &tID[0] ) )
    {    
        return;
    }    
    mLogSort = 505; 
    sprintf( mGameLogData, "[%d]<WEB_MALL>(%s),(%d),(%d)[[0]]", mLogSort, tID, tList[0], tList[2] );
    SendToLogServer();
} // __WEB_MALL__ 2

//INSTANCE
CGiftBankComm mGiftBankComm;

//CREATE
CGiftBankComm::CGiftBankComm( void )
{
}
//CONNECT
BOOL CGiftBankComm::Connect( void )
{
	if( mCheckConnectState )
	{
		return TRUE;
	}

	int tConnectRecvCheck;
	char tConnectDataCheck;
	char tSendData[1000];
	int tSendDataSize;
	int tSendValue01;
	int tSendValue02;
	int tSendValue03;
	int tOKRecvCheck;
	char tOKDataCheck;

	mSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if( mSocket < 0 )
	{
		return FALSE;
	}
	mAddress.sin_family = AF_INET;
	mAddress.sin_addr.s_addr = inet_addr( mIP );
	mAddress.sin_port = htons( static_cast<uint16_t> (mPort));
	if (connect (mSocket, (sockaddr*) &mAddress, sizeof (sockaddr_in)) < 0 )
	{
        LOG_TO_FILE_1 ("!For the following reasons: the connection failed.(%s).\n", ::strerror (errno));
		closesocket( mSocket );
		return FALSE;
	}

	mCheckConnectState = TRUE;

    int old_flag = 0;
    old_flag = ::fcntl (mSocket, F_GETFL);
    ::fcntl (mSocket, F_SETFL, old_flag | O_NONBLOCK);

	return TRUE;
}
//SEND
BOOL CGiftBankComm::Send( void )
{
	int tSendValue01;
	int tSendValue02;
	int tSendValue03;

	tSendValue01 = mSendDataSize;
	tSendValue02 = 0;
	while( tSendValue01 > 0 )
	{
		tSendValue03 = send( mSocket, &mSendData[tSendValue02], tSendValue01, 0 );
        if (tSendValue03 < 0) {
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                Close ();
                return FALSE;
            }
            continue;
        } else if (tSendValue03 == 0) {
            Close ();
            return FALSE;
        }
		tSendValue01 -= tSendValue03;
		tSendValue02 += tSendValue03;
	}
	return TRUE;
}
//RECV
BOOL CGiftBankComm::Recv( short nType )
{
    fd_set readfds;
    int iResult = 0;
	int tRecvValue01;
	int tRecvValue02;
    short nPacketLen = 0, nPacketType = 0;

	if( mRecvDataSize > 0 )
	{
        while (true) {
            tRecvValue01 = 0;
    		while( TRUE )
    		{
                FD_ZERO (&readfds);
                FD_SET (mSocket, &readfds);
                struct timeval tv = {2, 0};

                if ((iResult = ::select (mSocket + 1, &readfds, NULL, NULL, &tv)) < 0) {
                    if (errno == EINTR) continue;
                    Close ();
                    return FALSE;
                }

                if (iResult == 0) {
                    LOG_TO_FILE ("!Tired of waiting for an answer from the CGiftBankComm server to give up.\n");
                    LOG_TO_FILE_2 ("\t\t\tin (%s - %d).\n", __FILE__, __LINE__);
                    Close ();
                    return FALSE;
                }

                if (FD_ISSET (mSocket, &readfds)) {
                    tRecvValue02 = ::recv (mSocket, &mRecvData[tRecvValue01], mRecvDataSize - tRecvValue01, 0);
                    if( tRecvValue02 < 0 )
                    {
                        if (errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR) {
					        Close();
					        return FALSE;
				        } 
                        continue;
			        } else if (tRecvValue02 == 0) {
                        Close ();
                        return FALSE;
                    }
			        tRecvValue01 += tRecvValue02;
			        if( tRecvValue01 == mRecvDataSize )
			        {
			    	    break;
			        }
		        }
            }

            CopyMemory (&nPacketLen, mRecvData, sizeof (short));
            CopyMemory (&nPacketType, mRecvData + 2, sizeof (short));
            nPacketLen = ntohs (nPacketLen);
            nPacketType = ntohs (nPacketType);

            //if (nPacketType == 0x01) U_SEND_HEARTBEAT ();
            if (nPacketType == nType) break;
        }

        mRecvDataSize += nPacketLen;
        while (true) {
            FD_ZERO (&readfds);
            FD_SET (mSocket, &readfds);
            struct timeval tv = {2, 0};

            if ((iResult = ::select (mSocket + 1, &readfds, NULL, NULL, &tv)) < 0) {
                if (errno == EINTR) continue;
                Close ();
                return FALSE;
            }

            if (iResult == 0) {
                LOG_TO_FILE ("!Tired of waiting for an answer from the CGiftBankComm server to give up.\n");
                LOG_TO_FILE_2 ("\t\t\tin (%s - %d).\n", __FILE__, __LINE__);
                Close ();
                return FALSE;
            }

            if (FD_ISSET (mSocket, &readfds)) {
                tRecvValue02 = ::recv (mSocket, &mRecvData[tRecvValue01], mRecvDataSize - tRecvValue01, 0);
                if( tRecvValue02 < 0 )
                {
                    if (errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR) {
                        Close();
                        return FALSE;
                    }
                    continue;
                } else if (tRecvValue02 == 0) {
                    Close ();
                    return FALSE;
                }
                tRecvValue01 += tRecvValue02;
                if( tRecvValue01 == mRecvDataSize )
                {
                    break;
                }
            }
        }
	}
	return TRUE;
}
//CLOSE
void CGiftBankComm::Close( void )
{
	if( !mCheckConnectState )
	{
		return;
	}
	mCheckConnectState = FALSE;
	closesocket( mSocket );
}
//INIT
BOOL CGiftBankComm::Init( char tIP[16], int tPort )
{
	mCheckConnectState = FALSE;
	strcpy( mIP, tIP );
	mPort = tPort;
	if( !Connect() )
	{
		return FALSE;
	}
	return TRUE;
}
//FREE
void CGiftBankComm::Free( void )
{
	Close();
}
//PROCESS_FOR_NETWORK
BOOL CGiftBankComm::ProcessForNetwork( short nType )
{
    sigset_t signew, sigold;
    ::sigfillset (&signew);

    // 보내고 받는 과정에 발생하는 시그널은 차단합니다.
    BLOCK_SIGNAL ();

	if( !Connect() )
	{
        UNBLOCK_SIGNAL ();
		return FALSE;
	}
	if( !Send() )
	{
        UNBLOCK_SIGNAL ();
		return FALSE;
	}
	if( !Recv(nType) )
	{
        UNBLOCK_SIGNAL ();
		return FALSE;
	}

    UNBLOCK_SIGNAL ();
	return TRUE;
}

void CGiftBankComm::U_SEND_HEARTBEAT (void)
{
    short nTemp = 0;
    int iDataSize = 0;
    nTemp = htons (iDataSize);

    // 패킷의 데이타 부분 길이를 보내기 버퍼에 기록합니다.
    CopyMemory (&mSendData[0], &nTemp, sizeof (short));

    // 패킷 타입을 보내기 버퍼에 기록합니다.
    nTemp = htons (0x01);
    CopyMemory (&mSendData[2], &nTemp, sizeof (short));

    mRecvDataSize = 0;
    mSendDataSize = 4;

    while (true)
        if (ProcessForNetwork (0x01) == TRUE) 
            break;       
}

void CGiftBankComm::U_CHECK_GIFTISSUESERIAL (int tUserIndex, char szSerial[MAX_USER_CARD_NUMBER_LENGTH])
{
    short nTemp = 0;
    int iDataSize = 0;
    char buffer[512] = {0, };

    if (::strlen (szSerial) > 16) {
        mUSER[tUserIndex].mCheckDemandGiftIfo = FALSE;
        mTRANSFER.B_DEMAND_GIFT_RECV( 1, &mUSER[tUserIndex].mGiftInfo[0] );
        mUSER[tUserIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
        LOG_TO_FILE_3 ("![%s] There are too many of the digits of the serial (%s) number in (%s).\n", 
                       mUSER[tUserIndex].uID, szSerial, __FUNCTION__);
        return ;
    }

    // 패킷의 데이타 부분을 완성하고 길이를 측정합니다.
    //::snprintf (buffer, 512, "%d|%s", mSERVER_INFO.mIyoyoAPNum, szSerial);
    ::snprintf (buffer, 512, "%03d;%03d;%03d|%s"
			, mSERVER_INFO.mIslandNum, mSERVER_INFO.mPartitionNum
			, mSERVER_INFO.mWorldNumber, szSerial);
    iDataSize = ::strlen (buffer);

    // 패킷의 데이타 부분 길이를 보내기 버퍼에 기록합니다.
    nTemp = htons (iDataSize);
    CopyMemory (&mSendData[0], &nTemp, sizeof (short));
    // 패킷 타입을 보내기 버퍼에 기록합니다.
    nTemp = htons (0x10);
    CopyMemory (&mSendData[2], &nTemp, sizeof (short));
    // 패킷의 데이타 부분을 보내기 버퍼에 기록합니다.
    CopyMemory (&mSendData[4], buffer, iDataSize);

    // 전체 보내야 할 패킷의 사이즈는 header (4) + data 입니다.
    mRecvDataSize = 4;
    mSendDataSize = 4 + iDataSize;

    while (true) 
        if (ProcessForNetwork (0x10) == TRUE) 
            break;

    char* pToken = NULL;
    char* pSavePtr = NULL;
    int iResultCode = 0, iGiftNum = 0;

    ::memset (buffer, 0, sizeof (buffer));
    CopyMemory (buffer, &mRecvData[4], mRecvDataSize - 4);
    pToken = ::strtok_r (buffer, "|", &pSavePtr);
    if (pToken == NULL) {
        LOG_TO_FILE_3 ("!The following packet is not a normal packet.\n"\
                       "\t[%s] in (%s - %d).\n", buffer, __FILE__, __LINE__);
        mUSER[tUserIndex].mCheckDemandGiftIfo = FALSE;
        mTRANSFER.B_DEMAND_GIFT_RECV( 1, &mUSER[tUserIndex].mGiftInfo[0] );
        mUSER[tUserIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
        return ;
    }
    iResultCode = ::atoi (pToken);

    pToken = ::strtok_r (NULL, "|", &pSavePtr);
    if (pToken == NULL) {
        LOG_TO_FILE_3 ("!The following packet is not a normal packet.\n"\
                       "\t[%s] in (%s - %d).\n", buffer, __FILE__, __LINE__);
        mUSER[tUserIndex].mCheckDemandGiftIfo = FALSE;
        mTRANSFER.B_DEMAND_GIFT_RECV( 1, &mUSER[tUserIndex].mGiftInfo[0] );
        mUSER[tUserIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
        return ;
    }
    iGiftNum = (::atoi (pToken) % 1000);

    if (iResultCode != 1) {
        char reason[512] = {0, };
        mUSER[tUserIndex].mCheckDemandGiftIfo = FALSE;
        mTRANSFER.B_DEMAND_GIFT_RECV( 1, &mUSER[tUserIndex].mGiftInfo[0] );
        mUSER[tUserIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
        switch (iResultCode) {
            case 0: snprintf (reason, sizeof (reason), "Unknown problem");  
                break;
            case 2: snprintf (reason, sizeof (reason), "Already use");      
                break;
            case 3: snprintf (reason, sizeof (reason), "No identified");    
                break;
            case 4: snprintf (reason, sizeof (reason), "The serial is not available on the server");    
                break;
        }
        LOG_TO_FILE_3 ("![%s] User (%d - %s) because you can not receive gifts.\n",
                       mUSER[tUserIndex].uID, iResultCode, reason);
        return ;
    } 

    mUSER[tUserIndex].mCheckDemandGiftIfo = TRUE;
    strcpy( mUSER[tUserIndex].mCardNumber, szSerial );
    if (iGiftNum < 0 || iGiftNum >= MAX_GIFT_SORT_NUM) {
        mUSER[tUserIndex].mCheckDemandGiftIfo = FALSE;
        mTRANSFER.B_DEMAND_GIFT_RECV( 1, &mUSER[tUserIndex].mGiftInfo[0] );
        mUSER[tUserIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
        LOG_TO_FILE_1 ("![%s] The range of product code is unusual.\n", mUSER[tUserIndex].uID);
        return ;
    }

    ::memset (mUSER[tUserIndex].mGiftInfo, 0, sizeof (int) * 10);
    mUSER[tUserIndex].mGiftInfo[0] = iGiftNum;
    mUSER[tUserIndex].mGiftInfo[1] = 1; // 1개만 지급.

    mTRANSFER.B_DEMAND_GIFT_RECV( 0, &mUSER[tUserIndex].mGiftInfo[0] );
    mUSER[tUserIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
    LOG_TO_FILE_1 ("![%s] User can receive the gifts.\n", mUSER[tUserIndex].uID);
}

bool CGiftBankComm::U_UPDATE_GIFTISSUEINFO (int tUserIndex, char szSerial[MAX_USER_CARD_NUMBER_LENGTH],
                                            char szID[MAX_USER_ID_LENGTH])
{
    short nTemp = 0;   
    int iDataSize = 0;
    char buffer[512] = {0, };

    if (::strlen (szSerial) > 16) {
        mTRANSFER.B_WANT_GIFT_RECV( 4 );
        mUSER[tUserIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
        LOG_TO_FILE_3 ("![%s] There are too many of the digits of the serial (%s) number in (%s).\n", 
                       mUSER[tUserIndex].uID, szSerial, __FUNCTION__);
        return false;
    }

    // 패킷의 데이타 부분을 완성하고 길이를 측정합니다.
    ::snprintf (buffer, 512, "%03d;%03d;%03d|%s|%s"
			, mSERVER_INFO.mIslandNum, mSERVER_INFO.mPartitionNum
			, mSERVER_INFO.mWorldNumber, szSerial, szID);
    iDataSize = ::strlen (buffer);

    // 패킷의 데이타 부분 길이를 보내기 버퍼에 기록합니다.
    nTemp = htons (iDataSize);
    CopyMemory (&mSendData[0], &nTemp, sizeof (short));
    // 패킷 타입을 보내기 버퍼에 기록합니다.
    nTemp = htons (0x20);
    CopyMemory (&mSendData[2], &nTemp, sizeof (short));
    // 패킷의 데이타 부분을 보내기 버퍼에 기록합니다.
    CopyMemory (&mSendData[4], buffer, iDataSize);

    mRecvDataSize = 4;
    // 전체 보내야 할 패킷의 사이즈는 header (4) + data 입니다.
    mSendDataSize = 4 + iDataSize;

    while (true) 
        if (ProcessForNetwork (0x20) == TRUE) 
            break;

    ::memset (buffer, 0, sizeof (buffer));
    CopyMemory (buffer, &mRecvData[4], mRecvDataSize - 4);
    if (buffer[0] != '1') {
        mTRANSFER.B_WANT_GIFT_RECV( 5 );
        mUSER[tUserIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
        LOG_TO_FILE_2 ("![%d] Iyoyo AP can not be written to the log, to stop payment products(%c).\n",
                       mUSER[tUserIndex].uID, buffer[0]);
        return false;
    }

    return true;
}

#ifdef __CHANNEL__
//INSTANCE
CCompany mCompany;

//CREATE
CCompany::CCompany( void )
{
}
//CONNECT
BOOL CCompany::Connect( void )
{
	if( mCheckConnectState )
	{
		return TRUE;
	}

	int tConnectRecvCheck;
	char tConnectDataCheck;
	char tSendData[1000];
	int tSendDataSize;
	int tSendValue01;
	int tSendValue02;
	int tSendValue03;
	int tOKRecvCheck;
	char tOKDataCheck;

	mSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if( mSocket < 0 )
	{
		return FALSE;
	}
	mAddress.sin_family = AF_INET;
	mAddress.sin_addr.s_addr = inet_addr( mIP );
	mAddress.sin_port = htons( static_cast<uint16_t> (mPort));
	if (connect (mSocket, (sockaddr*) &mAddress, sizeof (sockaddr_in)) < 0 )
	{
        LOG_TO_FILE_1 ("!For the following reasons: the connection failed.(%s).\n", ::strerror (errno));
		closesocket( mSocket );
		return FALSE;
	}

	mCheckConnectState = TRUE;

    int old_flag = 0;
    old_flag = ::fcntl (mSocket, F_GETFL);
    ::fcntl (mSocket, F_SETFL, old_flag | O_NONBLOCK);

	return TRUE;
}
//SEND
BOOL CCompany::Send( void )
{
	int tSendValue01;
	int tSendValue02;
	int tSendValue03;

	tSendValue01 = mSendDataSize;
	tSendValue02 = 0;
	while( tSendValue01 > 0 )
	{
		tSendValue03 = send( mSocket, &mSendData[tSendValue02], tSendValue01, 0 );
        if (tSendValue03 < 0) {
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                Close ();
                return FALSE;
            }
            continue;
        } else if (tSendValue03 == 0) {
            Close ();
            return FALSE;
        }
		tSendValue01 -= tSendValue03;
		tSendValue02 += tSendValue03;
	}
	return TRUE;
}
//RECV
BOOL CCompany::Recv( short nType )
{
    fd_set readfds;
    int iResult = 0;
	int tRecvValue01;
	int tRecvValue02;
    short nPacketLen = 0, nPacketType = 0;

	if( mRecvDataSize > 0 )
	{
        while (true) {
            tRecvValue01 = 0;
    		while( TRUE )
    		{
                FD_ZERO (&readfds);
                FD_SET (mSocket, &readfds);
                struct timeval tv = {30, 0};

                if ((iResult = ::select (mSocket + 1, &readfds, NULL, NULL, &tv)) < 0) {
                    if (errno == EINTR) continue;
                    Close ();
                    return FALSE;
                }

                if (iResult == 0) {
                    LOG_TO_FILE ("!Tired of waiting for an answer from the CCompany server to give up.\n");
                    LOG_TO_FILE_2 ("\t\t\tin (%s - %d).\n", __FILE__, __LINE__);
                    Close ();
                    return FALSE;
                }

                if (FD_ISSET (mSocket, &readfds)) {
                    tRecvValue02 = ::recv (mSocket, &mRecvData[tRecvValue01], mRecvDataSize - tRecvValue01, 0);
                    if( tRecvValue02 < 0 )
                    {
                        if (errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR) {
					        Close();
					        return FALSE;
				        } 
                        continue;
			        } else if (tRecvValue02 == 0) {
                        Close ();
                        return FALSE;
                    }
			        tRecvValue01 += tRecvValue02;
			        if( tRecvValue01 == mRecvDataSize )
			        {
			    	    break;
			        }
		        }
            }

            CopyMemory (&nPacketLen, mRecvData, sizeof (short));
            CopyMemory (&nPacketType, mRecvData + 2, sizeof (short));
            nPacketLen = ntohs (nPacketLen);
            nPacketType = ntohs (nPacketType);

            //if (nPacketType == 0x01) U_SEND_HEARTBEAT ();
            if (nPacketType == nType) break;
        }

        mRecvDataSize += nPacketLen;
        while (true) {
            FD_ZERO (&readfds);
            FD_SET (mSocket, &readfds);
            struct timeval tv = {30, 0};

            if ((iResult = ::select (mSocket + 1, &readfds, NULL, NULL, &tv)) < 0) {
                if (errno == EINTR) continue;
                Close ();
                return FALSE;
            }

            if (iResult == 0) {
                LOG_TO_FILE ("!Tired of waiting for an answer from the CCompany server to give up.\n");
                LOG_TO_FILE_2 ("\t\t\tin (%s - %d).\n", __FILE__, __LINE__);
                Close ();
                return FALSE;
            }

            if (FD_ISSET (mSocket, &readfds)) {
                tRecvValue02 = ::recv (mSocket, &mRecvData[tRecvValue01], mRecvDataSize - tRecvValue01, 0);
                if( tRecvValue02 < 0 )
                {
                    if (errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR) {
                        Close();
                        return FALSE;
                    }
                    continue;
                } else if (tRecvValue02 == 0) {
                    Close ();
                    return FALSE;
                }
                tRecvValue01 += tRecvValue02;
                if( tRecvValue01 == mRecvDataSize )
                {
                    break;
                }
            }
        }
	}
	return TRUE;
}
//CLOSE
void CCompany::Close( void )
{
	if( !mCheckConnectState )
	{
		return;
	}
	mCheckConnectState = FALSE;
	closesocket( mSocket );
}
//INIT
BOOL CCompany::Init( char tIP[16], int tPort )
{
	mCheckConnectState = FALSE;
	strcpy( mIP, tIP );
	mPort = tPort;
	if( !Connect() )
	{
		return FALSE;
	}
	return TRUE;
}
//FREE
void CCompany::Free( void )
{
	Close();
}
//PROCESS_FOR_NETWORK
BOOL CCompany::ProcessForNetwork( short nType )
{
    sigset_t signew, sigold;
    ::sigfillset (&signew);

    // ....?... ...??...?.. ?..?.? ...?..??
    BLOCK_SIGNAL ();

	if( !Connect() )
	{
        UNBLOCK_SIGNAL ();
		return FALSE;
	}
	if( !Send() )
	{
        UNBLOCK_SIGNAL ();
		return FALSE;
	}
	if( !Recv(nType) )
	{
        UNBLOCK_SIGNAL ();
		return FALSE;
	}

    UNBLOCK_SIGNAL ();
	return TRUE;
}

BOOL CCompany::U_CHECK_ACCOUNT (char* tID, char* tPassword, int* tAdult, int* tResult )
{
    short nTemp = 0;
    int iDataSize = 0;
    char buffer[4096] = {0, };
	char uID[MAX_USER_ID_LENGTH];

	if(mSERVER_INFO.mCompanyNum == 1) { //Pubwin
		::snprintf (buffer, 4096, "%d;%d;%s %s", mSERVER_INFO.mCompanyNum, mSERVER_INFO.mGameNum, tID, tPassword );
	} else if(mSERVER_INFO.mCompanyNum == 2) { //Sina
		::snprintf (buffer, 4096, "17;1002;%s|%s", tID, tPassword );
	} else if(mSERVER_INFO.mCompanyNum == 3) { //KuGu
		::snprintf (buffer, 4096, "%d;%d;%s|%s", mSERVER_INFO.mCompanyNum, mSERVER_INFO.mGameNum, tID, tPassword );
	} else {
		return false;
	}
	LOG_TO_FILE_2("![%s] : SendBuffer[%s]\n", __FUNCTION__, buffer);

    iDataSize = ::strlen (buffer);
    nTemp = htons (iDataSize);
    CopyMemory (&mSendData[0], &nTemp, sizeof (short));
    nTemp = htons (0x50);
    CopyMemory (&mSendData[2], &nTemp, sizeof (short));
    CopyMemory (&mSendData[4], buffer, iDataSize);
    mRecvDataSize = 4;
    mSendDataSize = 4 + iDataSize;

	//while (true) 
	if(!ProcessForNetwork(0x50)) {
		*tResult = 8;
		return false;
	}

    char* pToken = NULL;
    char* pSavePtr = NULL;
    int iResultCode = 0, iGiftNum = 0;

    ::memset (buffer, 0, sizeof (buffer));
    CopyMemory (buffer, &mRecvData[4], mRecvDataSize - 4);
	LOG_TO_FILE_2("![%s] : RecvBuffer[%s]\n", __FUNCTION__, buffer);

	pToken = ::strtok_r (buffer, ";", &pSavePtr);
    if (pToken == NULL) {
        LOG_TO_FILE_3 ("!The following packet is not a normal packet.\n"\
                       "\t[%s] in (%s - %d).\n", buffer, __FILE__, __LINE__);
		*tResult = 7;
        return FALSE;
    }
    iResultCode = ::atoi (pToken);
	*tResult = iResultCode;

    pToken = ::strtok_r (NULL, ";", &pSavePtr);
    if (pToken == NULL) {
        LOG_TO_FILE_3 ("!The following packet is not a normal packet.\n"\
                       "\t[%s] in (%s - %d).\n", buffer, __FILE__, __LINE__);
		*tResult = 8;
        return FALSE;
    }
	//snprintf(uID, MAX_USER_ID_LENGTH, "%s", pToken);
	//if((mSERVER_INFO.mCompanyNum == 1) || (mSERVER_INFO.mCompanyNum == 2)) {
	//snprintf(tID, MAX_USER_ID_LENGTH, "%s", uID);
	//}
	snprintf(tID, MAX_USER_ID_LENGTH, "%s", pToken);

	pToken = ::strtok_r (NULL, ";", &pSavePtr);
    if (pToken == NULL) {
        LOG_TO_FILE_3 ("!The following packet is not a normal packet.\n"\
                       "\t[%s] in (%s - %d).\n", buffer, __FILE__, __LINE__);
		*tResult = 9;
        return FALSE;
    }
    *tAdult = ::atoi (pToken);

    if (iResultCode != 0) {
        char reason[512] = {0, };
        switch (iResultCode) {
            case 1: snprintf (reason, sizeof (reason), "%s", "Password fail or not exist ID");  
                break;
            case 2: snprintf (reason, sizeof (reason), "%s", "Block ID");      
                break;
            case 3: snprintf (reason, sizeof (reason), "%s", "Not exist ID");    
                break;
            case 4: snprintf (reason, sizeof (reason), "%s", "Server information fail");    
                break;
            case 5: snprintf (reason, sizeof (reason), "%s", "Over time of execution");    
                break;
            case 6: snprintf (reason, sizeof (reason), "%s", "Hash check fail");    
                break;
        }
        LOG_TO_FILE_4 ("![%s] User (%d - %s - %s) then you can not enjoy game.\n",
                       tID, iResultCode, tID, reason);
        return FALSE;
    } 
	return TRUE;
}
#endif

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
