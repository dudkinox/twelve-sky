//-------------------------------------------------------------------------------------------------
//HEADER
//-------------------------------------------------------------------------------------------------
#include "H01_MainApplication.h"
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
BOOL MyServer::Init( int *tResult )
{
	int index01;

	srand( (unsigned int) time( NULL ) );

	if( !mDB.Init() )
	{
		*tResult = 1;
		return FALSE;
	}
	
#ifdef __CASH_COM__
	if( !mCASH_COM.Init() )
	{
		*tResult = 2;
		LOG_TO_FILE_1("![%s] : Billing Server Init Failed!\n", __FUNCTION__);
		sleep(1);
		return FALSE;
	}
	LOG_TO_FILE_1("![%s] : Billing Server Init OK\n", __FUNCTION__);
#endif

	if( !mGAMELOG.Init( mSERVER_INFO.mUpperServer_2_IP, mSERVER_INFO.mUpperServer_2_Port ) )
	{
		*tResult = 3;
		return FALSE;
	}

	mUSER = (MyUser *)calloc(1, ( MAX_USER_NUM * sizeof( MyUser ) ) );

	if( mUSER == NULL )
	{
		*tResult = 4;
		return FALSE;
	}
	for( index01 = 0 ; index01 < MAX_USER_NUM ; index01++ )
	{
		mUSER[index01].mBUFFER_FOR_SEND = NULL;
		mUSER[index01].mBUFFER_FOR_RECV = NULL;
	}
	for( index01 = 0 ; index01 < MAX_USER_NUM ; index01++ )
	{
		if( !mUSER[index01].Init( MAX_SEND_BUFFER_SIZE, MAX_RECV_BUFFER_SIZE ) )
		{
			*tResult = 5;
			return FALSE;
		}
	}
	LOG_TO_FILE_1("![%s] : User mempry set OK\n", __FUNCTION__);

	if( !mWORK.Init() )
	{
		*tResult = 6;
		return FALSE;
	}
	LOG_TO_FILE_1("![%s] : Work start\n", __FUNCTION__);
	if( !mTRANSFER.Init() )
	{
		*tResult = 7;
		return FALSE;
	}
	LOG_TO_FILE_1("![%s] : Transfer start\n", __FUNCTION__);
	if( !mGAME.Init() )
	{
		*tResult = 8;
		return FALSE;
	}
	LOG_TO_FILE_1("![%s] : Game start\n", __FUNCTION__);
	return TRUE;
}
//FREE
void MyServer::Free( void )
{
	int index01;

	mGAME.Free();
	mTRANSFER.Free();
	mWORK.Free();
	if( mUSER )
	{
		for( index01 = 0 ; index01 < MAX_USER_NUM ; index01++ )
		{
			mUSER[index01].Quit( 0 );
		}
		for( index01 = 0 ; index01 < MAX_USER_NUM ; index01++ )
		{
			mUSER[index01].Free();
		}
		//-------------------------------------------------------------------------------------------------
		// 2008.11.18 Kim SungSoo
		// Intro : 1.  win32 api to linux c++
		//-------------------------------------------------------------------------------------------------		
#ifdef __WIN32__
		GlobalFree( mUSER );
#else
		free(mUSER);
#endif
		//-------------------------------------------------------------------------------------------------		

		mUSER = NULL;
	}

	mGAMELOG.Free();
	//--------------------------------------------------------------------------------------------------
	// 2008.11.18 Kim SungSoo
	// Intro : 1. Don't fixed cash server then no use
	//--------------------------------------------------------------------------------------------------	
#if 0
	mCASH_COM.Free();
#endif
	//--------------------------------------------------------------------------------------------------	


	closesocket( mSocket );


	mDB.Free();

	//-------------------------------------------------------------------------------------------------
	// 2008.11.18 Kim SungSoo
	// Intro : 1.  Delete win32 api 
	//-------------------------------------------------------------------------------------------------	
	//	WSACleanup();
	//	KillTimer( hMainWindow, 1 );
	//-------------------------------------------------------------------------------------------------	

}
//PROCESS_FOR_NETWORK
int MyServer::PROCESS_FOR_NETWORK(SOCKET *cli_fd, UINT fd_status, void *arg = NULL)
{
	int index01;
	//int tAddressLength; //2008.11.18 Kim SungSoo
	SOCKET tSocket;
	//SOCKADDR_IN	tAddress; //2008.11.18 Kim SungSoo
	int tempUserIndex;
	int tRecvSizeFromUser;
	BYTE tProtocol;

	tSocket = *cli_fd;

	//-------------------------------------------------------------------------------------------------
	// 2008.11.18 Kim SungSoo
	// Intro : 1.  Change switch flog ( 'WSAGETSELECTEVENT( lPrm )' to 'fd_status')
	//-------------------------------------------------------------------------------------------------	
	switch( fd_status )
		//-------------------------------------------------------------------------------------------------	
	{
		case FD_READ :
			for( tempUserIndex = 0 ; tempUserIndex < MAX_USER_NUM ; tempUserIndex++ )
			{
				if( !mUSER[tempUserIndex].mCheckConnectState )
				{
					continue;
				}
				if( mUSER[tempUserIndex].mSocket == *cli_fd )
				{
					break;
				}
			}
			if( tempUserIndex == MAX_USER_NUM )
			{
				// 2009.10.26 : 김성수
				// 제거 내용 :mUSER[tempUserIndex].Quit( 10001 );
				return -1;
			}

//AGAIN:
			while(1) {
			        tRecvSizeFromUser = recv(tSocket, &mUSER[tempUserIndex].mBUFFER_FOR_RECV[mUSER[tempUserIndex].mTotalRecvSize]
					        , ( MAX_RECV_BUFFER_SIZE - mUSER[tempUserIndex].mTotalRecvSize ), 0 );
        
			        //LOG_TO_FILE_1("read size : %d\n", tRecvSizeFromUser);
        
			        //-------------------------------------------------------------------------------------------------
			        // 2008.11.18 Kim SungSoo
			        // Intro : 1.  win32 api to linux c++
			        //-------------------------------------------------------------------------------------------------		
        
			        if( tRecvSizeFromUser < 0 )
			        {
				        if(errno == EINTR)
						continue;
				        if(errno != EAGAIN && errno != EWOULDBLOCK)
				        {
						mUSER[tempUserIndex].Quit( 10002 );
					        return 0;
				        }
				        return -1;
			        }
			        else if(tRecvSizeFromUser == 0) 
			        {
				        mUSER[tempUserIndex].Quit( 1001 );
				        return 0;
			        }
			        //-------------------------------------------------------------------------------------------------		
			        mUSER[tempUserIndex].mTotalRecvSize += tRecvSizeFromUser;
				break;
			}

			while( TRUE )
			{

				if( mUSER[tempUserIndex].mTotalRecvSize < 1 )
				{
					mUSER[tempUserIndex].Quit( 10003 );
					return 0;
				}
				tProtocol = (BYTE) mUSER[tempUserIndex].mBUFFER_FOR_RECV[0];
				if( W_FUNCTION[tProtocol].PROC == NULL )
				{
					mUSER[tempUserIndex].Quit( 1002 );
					return 0;
				}

				//LOG_TO_FILE_2("mUSER[tempUserIndex].mTotalRecvSize : %d, mWORK.mWorkerPacketSize[tProtocol] : %d\n" ,  mUSER[tempUserIndex].mTotalRecvSize, mWORK.mWorkerPacketSize[tProtocol]);

				if( mUSER[tempUserIndex].mTotalRecvSize < mWORK.mWorkerPacketSize[tProtocol] )
				{
					return -1;
					//goto AGAIN;
					//return 0;
				}

				W_FUNCTION[tProtocol].PROC( tempUserIndex );

				if( mUSER[tempUserIndex].mCheckConnectState )
				{
					//-------------------------------------------------------------------------------------------------
					// 2008.11.18 Kim SungSoo
					// Intro : 1. Modify win32 api to linuc c++ (MoveMemory - > memmove)
					//-------------------------------------------------------------------------------------------------				
					//memmove(mUSER[tempUserIndex].mBUFFER_FOR_RECV, mUSER[tempUserIndex].mBUFFER_FOR_RECV + mWORK.mWorkerPacketSize[tProtocol]
					//		, ( mUSER[tempUserIndex].mTotalRecvSize - mWORK.mWorkerPacketSize[tProtocol] ));
					MoveMemory( &mUSER[tempUserIndex].mBUFFER_FOR_RECV[0], &mUSER[tempUserIndex].mBUFFER_FOR_RECV[mWORK.mWorkerPacketSize[tProtocol]]
							, ( mUSER[tempUserIndex].mTotalRecvSize - mWORK.mWorkerPacketSize[tProtocol] ) );
					//-------------------------------------------------------------------------------------------------				

					mUSER[tempUserIndex].mTotalRecvSize -= mWORK.mWorkerPacketSize[tProtocol];
				}

				if (0 == mUSER[tempUserIndex].mTotalRecvSize)
				{
					//LOG_TO_FILE_1("MIR 4(%d)\n", tRecvSizeFromUser);
					return tRecvSizeFromUser;
				}
			}
			return tRecvSizeFromUser;
		case FD_WRITE :
			for( tempUserIndex = 0 ; tempUserIndex < MAX_USER_NUM ; tempUserIndex++ )
			{
				if( !mUSER[tempUserIndex].mCheckConnectState )
				{
					continue;
				}
				if( mUSER[tempUserIndex].mSocket == *cli_fd )
				{
					break;
				}
			}
			if( tempUserIndex == MAX_USER_NUM )
			{
				return -1;
			}
			mUSER[tempUserIndex].Send( FALSE, NULL, 0 );
			return 1;
		case FD_ACCEPT :

			//-------------------------------------------------------------------------------------------------
			// 2008.11.18 Kim SungSoo
			// Intro : 1. Delete winsocket 
			//-------------------------------------------------------------------------------------------------	
			//		tAddressLength = sizeof( tAddress );
			//		tSocket = accept( mSocket, (LPSOCKADDR) &tAddress, &tAddressLength );
			//		if( tSocket == INVALID_SOCKET )
			//		{
			//			return;
			//		}		
			//-------------------------------------------------------------------------------------------------	


			for( index01 = 0 ; index01 < MAX_USER_NUM ; index01++ )
			{
				if( !mUSER[index01].mCheckConnectState )
				{
					//LOG_TO_FILE_1("!User index  = %d \n" ,index01 );
					break;
				}
			}
			if( index01 == MAX_USER_NUM )
			{
				//-------------------------------------------------------------------------------------------------
				// 2008.11.18 Kim SungSoo
				// Intro : 1. Change winsocket to fd
				//-------------------------------------------------------------------------------------------------	
				closesocket( tSocket );
				//-------------------------------------------------------------------------------------------------	
				return 0;
			}

			//-------------------------------------------------------------------------------------------------
			// 2008.11.18 Kim SungSoo
			// Intro : 1. Delete winsocket 
			//-------------------------------------------------------------------------------------------------	
			//		if( WSAAsyncSelect( tSocket, hWnd, WM_NETWORK_MESSAGE_1, ( FD_READ | FD_WRITE | FD_CLOSE ) ) == SOCKET_ERROR )
			//		{
			//			closesocket( tSocket );
			//			return;
			//		}
			//-------------------------------------------------------------------------------------------------	

			mUSER[index01].mCheckConnectState = TRUE;
			mUSER[index01].mUsedTime = GetTickCount();
			mUSER[index01].mSocket = tSocket;
			sprintf( mUSER[index01].mIP, "%s", inet_ntoa(*((struct in_addr*)arg)));
			mUSER[index01].mTotalSendSize = 0;
			mUSER[index01].mTotalRecvSize = 0;
			mUSER[index01].mCheckServerType = 0;
			mTRANSFER.B_CONNECT_OK();

			//LOG_TO_BOTH("accept data ok! no send \n" );

			mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			//LOG_TO_FILE_2("!accept data send : data = %s , size = %d\n" , mTRANSFER.mOriginal , mTRANSFER.mOriginalSize  );
			return 1;
		case FD_CLOSE :
			for( tempUserIndex = 0 ; tempUserIndex < MAX_USER_NUM ; tempUserIndex++ )
			{
				if( !mUSER[tempUserIndex].mCheckConnectState )
				{
					continue;
				}
				//-------------------------------------------------------------------------------------------------
				// 2008.11.18 Kim SungSoo
				// Intro : 1. Delete winsocket 
				//-------------------------------------------------------------------------------------------------	
#ifdef __WIN32__
				if( mUSER[tempUserIndex].mSocket == (SOCKET) wPrm )
#else
					if( mUSER[tempUserIndex].mSocket == *cli_fd )
#endif
						//-------------------------------------------------------------------------------------------------	
					{
						break;
					}
			}
			if( tempUserIndex == MAX_USER_NUM )
			{
				return 0;
			}

			mUSER[tempUserIndex].Quit( 0 );
			return 0;
	}
}
//PROCESS_FOR_TIMER
//-------------------------------------------------------------------------------------------------
// 2008.11.18 Kim SungSoo
// Intro : 1. Delete timer function (this job is runing CWorkerProcess::check_time())
//				 2. reuse
//-------------------------------------------------------------------------------------------------

void MyServer::PROCESS_FOR_TIMER( void )
{
	int index01;
	DWORD tPresentTime;

	tPresentTime = GetTickCount();
	for( index01 = 0 ; index01 < MAX_USER_NUM ; index01++ )
	{
		if( !mUSER[index01].mCheckConnectState )
		{
			continue;
		}
		if( ( tPresentTime - mUSER[index01].mUsedTime ) > 1800000 )
		{
			LOG_TO_FILE_3("![%s] : %u %u\n", __FUNCTION__, tPresentTime, mUSER[index01].mUsedTime);
			mUSER[index01].Quit( 1003 );
		}
	}
	mGAME.mTickCount++;
#if 0
	if( ( mGAME.mTickCount % 6 ) == 0 )
	{
		//--------------------------------------------------------------------------------------------------
		// 2008.11.18 Kim SungSoo
		// Intro : 1. Don't fixed cash server then no use
		//--------------------------------------------------------------------------------------------------
#if 0
		mCASH_COM.GetInfo();
#endif
		//--------------------------------------------------------------------------------------------------

	}
#endif
}

//-------------------------------------------------------------------------------------------------

//WRITE_FOR_ERROR
/*
   void MyServer::WRITE_FOR_ERROR( char *tErrorData )
   {
//-------------------------------------------------------------------------------------------------
// 2008.11.18 Kim SungSoo
// Intro : 1. Don't use error logger function (this job is runing LOG_TO_FILE*())
//-------------------------------------------------------------------------------------------------

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
strcpy( tempString01, "LOG\\ERROR_" );
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
sprintf( tempString02, "[%d:%d:%d] %s\r\n", tPresentTime->tm_hour, tPresentTime->tm_min, tPresentTime->tm_sec, tErrorData );
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

//-------------------------------------------------------------------------------------------------

}*/
//-------------------------------------------------------------------------------------------------
