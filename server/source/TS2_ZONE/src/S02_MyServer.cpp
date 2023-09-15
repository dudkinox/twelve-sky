//-------------------------------------------------------------------------------------------------
//HEADER
//-------------------------------------------------------------------------------------------------
#include "H01_MainApplication.h"
#include "H11_Sphinx.h"

// # X-Trap # {
// # }
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_SERVER
//-------------------------------------------------------------------------------------------------
//INSTANCE
MyServer mSERVER;
//CREATE
MyServer::MyServer( void )
{
}
//INIT
BOOL MyServer::Init( int *tResult)
{
	int index01;
	int tPort;


	mMAX_USER_NUM = mSERVER_INFO.mServerMaxUserNum;
	switch( mSERVER_INFO.mServerNumber )
	{
	case  38 :
#ifdef __GOD__
		mMAX_SEND_BUFFER_SIZE = 500000;
		break;
#endif
	case 175 :
	case 176 :
	case 177 :
	case 178 :
	case 179 :
	case 180 :
	case 181 :
	case 182 :
	case 183 :
	case 184 :
	case 185 :
	case 186 :
	case 187 :
	case 188 :
	case 189 :
	case 190 :
	case 191 :
	case 192 :
	case 193 :
#ifdef __GOD__
	case  19 :
	case  20 :
	case  21 :
	case  25 :
	case  26 :
	case  27 :
	case  31 :
	case  32 :
	case  33 :
	case  34 :
	case  35 :
	case  36 :
		mMAX_SEND_BUFFER_SIZE = 600000;
		break;
#else
		mMAX_SEND_BUFFER_SIZE = 300000;
		break;
#endif
	case  49 :
#ifdef __GOD__
		mMAX_SEND_BUFFER_SIZE = 800000;
		break;
#else
		mMAX_SEND_BUFFER_SIZE = 900000;
		break;
#endif
	case  51 :
	case  53 :
	case 146 :
	case 147 :
	case 148 :
	case 149 :
	case 150 :
	case 151 :
	case 152 :
	case 153 :
	case 154 :
	case 155 :
	case 156 :
	case 157 :
	case 158 :
	case 159 :
	case 160 :
	case 161 :
	case 162 :
	case 163 :
	case 164 :
#ifdef __GOD__
	case 120 :
	case 121 :
	case 122 :
		mMAX_SEND_BUFFER_SIZE = 400000;
		break;
	case 194 :
		mMAX_SEND_BUFFER_SIZE = 500000;
		break;
	case 195 :
	case 196 :
	case 197 :
	case 198 :
	case 199 :
	case  85 :
	case  86 :
	case  87 :
		mMAX_SEND_BUFFER_SIZE = 300000;
		break;
	case 267 :
	case 268 :
	case 269 :
		mMAX_SEND_BUFFER_SIZE = 800000;
		break;
#else
		mMAX_SEND_BUFFER_SIZE = 500000;
		break;
#endif
	default :
		mMAX_SEND_BUFFER_SIZE = 153600;
		break;
	}
	mMAX_RECV_BUFFER_SIZE = 10240;
	rand_mir_init();
	//srand( (unsigned int) time( NULL ) );
	
	mRankCheck = true;
	if(((mSERVER_INFO.mServerNumber >= 101) && (mSERVER_INFO.mServerNumber <= 109))
			|| ((mSERVER_INFO.mServerNumber >= 126) && (mSERVER_INFO.mServerNumber <= 137))
			|| ((mSERVER_INFO.mServerNumber >= 154) && (mSERVER_INFO.mServerNumber <= 164))
			|| ((mSERVER_INFO.mServerNumber >= 167) && (mSERVER_INFO.mServerNumber <= 169))
			|| ((mSERVER_INFO.mServerNumber >= 171) && (mSERVER_INFO.mServerNumber <= 193))) {
		mRankCheck = false;
	}

	LOG_TO_FILE_1("![%s] : Main server start\n", __FUNCTION__);
#ifdef __WIN32__
	mUSER = (MyUser *) GlobalAlloc( GMEM_FIXED, ( mMAX_USER_NUM * sizeof( MyUser ) ) );
#else
	mUSER = (MyUser *)calloc(1, ( mMAX_USER_NUM * sizeof( MyUser ) ) );
#endif

	if( mUSER == NULL )
	{
		*tResult = 13;
		return FALSE;
	}
	for( index01 = 0 ; index01 < mMAX_USER_NUM ; index01++ )
	{
		mUSER[index01].mBUFFER_FOR_SEND = NULL;
		mUSER[index01].mBUFFER_FOR_RECV = NULL;
		//---------//
		//N-PROTECT//
		//---------//
#ifdef __N_PROTECT__
		mUSER[index01].mNPROTECT = NULL;
#endif
		//---------//
		//---------//
		//---------//
	}
	for( index01 = 0 ; index01 < mMAX_USER_NUM ; index01++ )
	{
		if( !mUSER[index01].Init( index01, mMAX_SEND_BUFFER_SIZE, mMAX_RECV_BUFFER_SIZE ) )
		{
			*tResult = 14;
			return FALSE;
		}
	}

	if( !mPLAYUSER_COM.Init( mSERVER_INFO.mServerNumber, mSERVER_INFO.mUpperServer_1_IP, mSERVER_INFO.mUpperServer_1_Port ) )
	{
		*tResult = 7;
		return FALSE;
	}
	LOG_TO_FILE_1("![%s] : Player server start\n", __FUNCTION__);
	if( !mCENTER_COM.Init( mSERVER_INFO.mServerNumber, mSERVER_INFO.mServerPort, mSERVER_INFO.mUpperServer_2_IP, mSERVER_INFO.mUpperServer_2_Port ) )
	{
		*tResult = 8;
		return FALSE;
	}
	LOG_TO_FILE_1("![%s] : Center server start\n", __FUNCTION__);
	if( !mRELAY_COM.Init( mSERVER_INFO.mServerNumber, mSERVER_INFO.mUpperServer_3_IP, mSERVER_INFO.mUpperServer_3_Port ) )
	{
		*tResult = 9;
		return FALSE;
	}
	LOG_TO_FILE_1("![%s] : Relay server start\n", __FUNCTION__);

	if( !mEXTRA_COM.Init( mSERVER_INFO.mServerNumber, mSERVER_INFO.mUpperServer_4_IP, mSERVER_INFO.mUpperServer_4_Port ) )
	{
		*tResult = 10;
		return FALSE;
	}
	LOG_TO_FILE_1("![%s] : Extra server start\n", __FUNCTION__);

	if( !mGAMELOG.Init( mSERVER_INFO.mUpperServer_5_IP, mSERVER_INFO.mUpperServer_5_Port ) )
	{
		*tResult = 11;
		return FALSE;
	}
	LOG_TO_FILE_1("![%s] : Gamelog server start\n", __FUNCTION__);

#ifdef USE_UDP
    if (g_BroadCastMgr.Prepare (mSERVER_INFO.mServerPort) == false) {
        *tResult = 21;
        return FALSE;
    }
	LOG_TO_FILE_1("![%s] : Udp server start\n", __FUNCTION__);
#endif

	tPort = 0;

	switch( (mSERVER_INFO.mWorldNumber % 4) )
	{
		case 1:
			tPort = 55001;
			break;
		case 2:
			tPort = 55002;
			break;
		case 3:
			tPort = 55003;
			break;
		case 0:
			tPort = 55004;
			break;
	}
	if( !mCHATLOG.Init( mSERVER_INFO.mUpperServer_5_IP, tPort ) )
	{
		*tResult = 12;
		return FALSE;
	}
	LOG_TO_FILE_1("![%s] : Chatlog server start\n", __FUNCTION__);
	if( !mWORK.Init() )
	{
		*tResult = 15;
		return FALSE;
	}
	LOG_TO_FILE_1("![%s] : Work start\n", __FUNCTION__);
	if( !mTRANSFER.Init() )
	{
		*tResult = 16;
		return FALSE;
	}
	LOG_TO_FILE_1("![%s] : Transfer start\n", __FUNCTION__);
	if( !mGAME.Init() )
	{
		*tResult = 17;
		return FALSE;
	}
	LOG_TO_FILE_1("![%s] : Game start\n", __FUNCTION__);
	
	
	mGAME.mCheckActiveBattleLogic = TRUE;
	//---------//
	//N-PROTECT//
	//---------//
#ifdef __N_PROTECT__	
	if( InitGameguardAuth( "", 100, TRUE, 0x00 ) != 0 )
	{
		*tResult = 18;
		return FALSE;
	}
	if( SetUpdateCondition( 30, 50 ) != 0 )
	{
		*tResult = 19;
		return FALSE;
	}
	/*
	if( !SetTimer( hMainWindow, 2, 600000, NULL ) )
	{
		*tResult = 20;
		return FALSE;
	}
	*/
	LOG_TO_FILE_1("![%s] : nProtect start\n", __FUNCTION__);
#endif	
	//---------//
	//---------//
	//---------//

	mSphinx.WakeUp_Sphinx();

	return TRUE;
}
//FREE
void MyServer::Free( void )
{
	int index01;

	//---------//
	//N-PROTECT//
	//---------//
#ifdef __N_PROTECT__	
	//KillTimer( hMainWindow, 2 );
	CleanupGameguardAuth();
#endif	
	//---------//
	//---------//
	//---------//
	mSphinx.ToSleep_Sphinx();

	mGAME.Free();
	mTRANSFER.Free();
	mWORK.Free();
	if( mUSER )
	{
		for( index01 = 0 ; index01 < mMAX_USER_NUM ; index01++ )
		{
			mUSER[index01].Quit(__FILE__, __FUNCTION__, __LINE__);
		}
		for( index01 = 0 ; index01 < mMAX_USER_NUM ; index01++ )
		{
			mUSER[index01].Free();
		}
#ifdef __WIN32__
		GlobalFree( mUSER );
#else
		free(mUSER);
#endif
		mUSER = NULL;
	}
	mCHATLOG.Free();
	mGAMELOG.Free();
	mEXTRA_COM.Free();
	mRELAY_COM.Free();
	mCENTER_COM.Free();
	mPLAYUSER_COM.Free();
	closesocket( mSocket );
}
//PROCESS_FOR_NETWORK
int MyServer::PROCESS_FOR_NETWORK(SOCKET *cli_fd, UINT fd_status, void *arg = NULL)
{
	BYTE tProtocol;
	int index01 = 0;
	int tRandomNumber = 0;
	int tempUserIndex = 0;
	int tRecvSizeFromUser = 0;
	char tPacketEncryptionValue;
	int tSocket = *cli_fd;

	switch(fd_status)
	{
		case FD_READ :
			for( tempUserIndex = 0 ; tempUserIndex < mMAX_USER_NUM ; tempUserIndex++ )
			{
				if( !mUSER[tempUserIndex].mCheckConnectState )
				{ 
					continue;
				}
				if( mUSER[tempUserIndex].mSocket == tSocket )
				{
					break;
				}
			}

			if( tempUserIndex == mMAX_USER_NUM )
			{
				// 2009.10.26 : 김성수
				// 제거 내용 :mUSER[tempUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				return -1;
			}
//AGAIN:
			while(1) {
			        tRecvSizeFromUser = recv(tSocket
					        , &mUSER[tempUserIndex].mBUFFER_FOR_RECV[mUSER[tempUserIndex].mTotalRecvSize]
					        , ( mMAX_RECV_BUFFER_SIZE - mUSER[tempUserIndex].mTotalRecvSize ), 0 );
			        //LOG_TO_FILE_1("tRecvSizeFromUser :%d\n", tRecvSizeFromUser);
        
			        if( tRecvSizeFromUser < 0 ) {
				        if(errno == EINTR)
						continue;
						//goto AGAIN;
				        if(errno != EAGAIN && errno != EWOULDBLOCK) {
					        mUSER[tempUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
					        return 0;
				        }
				        return -1;
			        } else if(tRecvSizeFromUser == 0) {
				        mUSER[tempUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				        return 0;
			        } 
        
			        for( index01 = mUSER[tempUserIndex].mTotalRecvSize ; index01 < ( mUSER[tempUserIndex].mTotalRecvSize + tRecvSizeFromUser ) ; index01++ )
			        {
				        mUSER[tempUserIndex].mBUFFER_FOR_RECV[index01] = (char)(mUSER[tempUserIndex].mBUFFER_FOR_RECV[index01] ^ mUSER[tempUserIndex].mPacketEncryptionValue[0]);
			        }
			        mUSER[tempUserIndex].mTotalRecvSize += tRecvSizeFromUser;
				break;
			}
			//nCount = 0;
			while( TRUE )
			{
				if( mUSER[tempUserIndex].mTotalRecvSize < 9 )
				{
					return -1;
				}
				tProtocol = (BYTE) mUSER[tempUserIndex].mBUFFER_FOR_RECV[8];

				if( W_FUNCTION[tProtocol].PROC == NULL )
				{
					if( mUSER[tempUserIndex].mCheckValidState )
					{
						mPLAYUSER_COM.U_BLOCK_USER_FOR_ZONE_2_SEND( 601, &mUSER[tempUserIndex].uID[0] );
					}
					mUSER[tempUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
					return 0;
				}
				if( mUSER[tempUserIndex].mTotalRecvSize < mWORK.mWorkerPacketSize[tProtocol] )
				{
					return -1;
					//LOG_TO_FILE_4("![%s] : %d, %d, %d\n", __FUNCTION__, mUSER[tempUserIndex].mTotalRecvSize
					//		, mWORK.mWorkerPacketSize[tProtocol], tProtocol);
					//goto AGAIN;
				}
				//CopyMemory( &tPacketEncryptionValue, &mUSER[tempUserIndex].mBUFFER_FOR_RECV[7], 1 );
				tPacketEncryptionValue = mUSER[tempUserIndex].mBUFFER_FOR_RECV[7];

				if( tPacketEncryptionValue != mUSER[tempUserIndex].mPacketEncryptionValue[1] )
				{
					if( mUSER[tempUserIndex].mCheckValidState )
					{
						mPLAYUSER_COM.U_BLOCK_USER_FOR_ZONE_2_SEND( 602, &mUSER[tempUserIndex].uID[0] );
					}
					//mUSER[tempUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
					return 0;
				}
				mUSER[tempUserIndex].mPacketEncryptionValue[1]++;
				if( ( mUSER[tempUserIndex].mCheckValidState ) && ( mUSER[tempUserIndex].uTraceState == 1 ) )
				{
					mGAMELOG.GL_603_TRACE_USER( tempUserIndex, (int) tProtocol );
				}
				mUSER[tempUserIndex].mCheckTime = ::time(NULL);
				W_FUNCTION[tProtocol].PROC( tempUserIndex );
				if( mUSER[tempUserIndex].mCheckConnectState ) {
					memmove(mUSER[tempUserIndex].mBUFFER_FOR_RECV, mUSER[tempUserIndex].mBUFFER_FOR_RECV + mWORK.mWorkerPacketSize[tProtocol]
							, ( mUSER[tempUserIndex].mTotalRecvSize - mWORK.mWorkerPacketSize[tProtocol] ));
					mUSER[tempUserIndex].mTotalRecvSize -= mWORK.mWorkerPacketSize[tProtocol];
				}
				if(mUSER[tempUserIndex].mTotalRecvSize == 0)
					break;
			}
			return tRecvSizeFromUser;
		case FD_WRITE :
				for( tempUserIndex = 0 ; tempUserIndex < mMAX_USER_NUM ; tempUserIndex++ )
				{
					if( !mUSER[tempUserIndex].mCheckConnectState )
					{
						continue;
					}
					if( mUSER[tempUserIndex].mSocket == tSocket )
					{
						break;
					}
				}
				if( tempUserIndex == mMAX_USER_NUM )
				{
					return -1;
				}
				mUSER[tempUserIndex].Send( FALSE, NULL, 0 );
			return 0;

		case FD_ACCEPT :
			for( index01 = 0 ; index01 < mMAX_USER_NUM ; index01++ )
			{
				if( !mUSER[index01].mCheckConnectState )
				{
					break;
				}
			}
			if( index01 == mMAX_USER_NUM )
			{
				closesocket( tSocket );
				return -1;
			}

			mUSER[index01].mCheckConnectState = TRUE;
			mUSER[index01].mCheckValidState = FALSE;
			mUSER[index01].mConnectTime = GetTickCount();
			mUSER[index01].mSocket = tSocket;
			mUSER[index01].mCliFd = cli_fd;
			sprintf( mUSER[index01].mIP, "%s", inet_ntoa(*((struct in_addr*)arg)));
			//LOG_TO_FILE_2("![%s] : End User address = %s\n", __FUNCTION__, mUSER[index01].mIP);
			tRandomNumber = ( rand_mir() % 1001 ) * ( rand_mir() % 1001 );
			CopyMemory( &mUSER[index01].mPacketEncryptionValue[0], &tRandomNumber, 1 );
			CopyMemory( &mUSER[index01].mPacketEncryptionValue[1], &tRandomNumber, 1 );
			mUSER[index01].mPacketEncryptionValue[1] += 127;
			mUSER[index01].mTotalSendSize = 0;
			mUSER[index01].mTotalRecvSize = 0;
      // # Defense Hack # {
      mUSER[index01].mUniqueNumber = 0;
      mUSER[index01].mAttackTickCount = 0;
      mUSER[index01].mAutoCheckCount = 0;
      mUSER[index01].mSpeedTickCount = 0;
      // # }
			mUSER[index01].mCheckTempRegister = FALSE;
			::memset(&mUSER[index01].mAvatarInfo, 0x00, sizeof(AVATAR_INFO));
			::memset(&mAVATAR_OBJECT[index01].mDATA, 0x00, sizeof(OBJECT_FOR_AVATAR));
      // @ Cost Info Download @
      mUSER[index01].mIsValidCashItemInfo = false;
      // @
      mUSER[index01].Set_Send_Question( FALSE );

#ifdef __GOD__
      mUSER[index01].mEffectCheckTime = ::time(NULL);
			mUSER[index01].mCheckTempRegister = FALSE;
			//mUSER[index01].mCheckTime = GetTickCount();
#endif

			//---------//
			//N-PROTECT//
			//---------//
#ifdef __N_PROTECT__
			mUSER[index01].mNPROTECT->Init();
#endif
			//---------//
			//---------//
			//---------//


			mTRANSFER.B_CONNECT_OK( tRandomNumber );
			mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			return 0;
		case FD_CLOSE :
			for( tempUserIndex = 0 ; tempUserIndex < mMAX_USER_NUM ; tempUserIndex++ )
			{
				if( !mUSER[tempUserIndex].mCheckConnectState )
				{
					continue;
				}
				if( mUSER[tempUserIndex].mSocket == tSocket )
				{
					break;
				}
			}
			if( tempUserIndex == mMAX_USER_NUM )
			{
				return 0;
			}
			//LOG_TO_FILE_1("![FD_CLOSE] - %s\n", tempUserIndex);
			mUSER[tempUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return 0;
	}
	return tRecvSizeFromUser;
}
//WRITE_FOR_LOG
void MyServer::WRITE_FOR_LOG( char *tLogData )
{
#if 0
	char tempString01[10000];
	char tempString02[10000];
	time_t tCountSeconds;
	struct tm *tPresentTime;
	HANDLE hFile;
	DWORD nWriteByte;

	time( &tCountSeconds );
	tPresentTime = localtime( &tCountSeconds );
	if( tPresentTime == NULL )
	{
		return;
	}
	strcpy( tempString01, "LOG_" );
	itoa( ( tPresentTime->tm_year + 1900 ), tempString02, 10 );
	strcat( tempString01, tempString02 );
	strcat( tempString01, "-" );
	if( tPresentTime->tm_mon < 9 )
	{
		tempString02[0] = '0';
		tempString02[1] = tPresentTime->tm_mon + 1 + '0';
		tempString02[2] = '\0';
	}
	else
	{
		itoa( ( tPresentTime->tm_mon + 1 ), tempString02, 10 );
	}
	strcat( tempString01, tempString02 );
	strcat( tempString01, "-" );
	if( tPresentTime->tm_mday < 10 )
	{
		tempString02[0] = '0';
		tempString02[1] = tPresentTime->tm_mday + '0';
		tempString02[2] = '\0';
	}
	else
	{
		itoa( tPresentTime->tm_mday, tempString02, 10 );
	}
	strcat( tempString01, tempString02 );
	strcat( tempString01, ".TXT" );
	sprintf( tempString02, "[%d:%d:%d] %s\r\n", tPresentTime->tm_hour, tPresentTime->tm_min, tPresentTime->tm_sec, tLogData );
	hFile = CreateFile(	tempString01, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE )
	{
		return;
	}
	if( SetFilePointer( hFile, 0, 0, FILE_END ) == 0xFFFFFFFF )
	{
		CloseHandle( hFile );
		return;
	}
	if( !WriteFile( hFile, tempString02, strlen( tempString02 ), &nWriteByte, NULL ) )
	{
		CloseHandle( hFile );
		return;
	}
	if( nWriteByte != strlen( tempString02 ) )
	{
		CloseHandle( hFile );
		return;
	}
	if( !CloseHandle( hFile ) )
	{
		return;
	}
#endif
}
//-------------------------------------------------------------------------------------------------
