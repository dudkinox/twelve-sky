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

    rand_mir_init ();
	if( !mDB.Init() )
	{
		*tResult = 2;
		return FALSE;
	}
	if( !mPLAYUSER_COM.Init( mSERVER_INFO.mUpperServer_1_IP, mSERVER_INFO.mUpperServer_1_Port ) )
	{
		*tResult = 8;
		return FALSE;
	}
	if( !mCENTER_COM.Init( mSERVER_INFO.mUpperServer_2_IP, mSERVER_INFO.mUpperServer_2_Port ) )
	{
		*tResult = 9;
		return FALSE;
	}
	if( !mGAMELOG.Init( mSERVER_INFO.mUpperServer_3_IP, mSERVER_INFO.mUpperServer_3_Port ) )
	{
		*tResult = 10;
		return FALSE;
	}
/*
// 2009.11.30 : 김성수
// 터키 유럽의 경우에는 선물 받시 서버가 준비 중입니다.
// 따라서 아래 코드는 본서버에서도 적용되지 않습니다.	
	if (!mGiftBankComm.Init (mSERVER_INFO.mUpperServer_4_IP, mSERVER_INFO.mUpperServer_4_Port)) {
     *tResult = 16;
     return FALSE;
  }

*/
#ifdef __CHANNEL__
	if (!mCompany.Init (mSERVER_INFO.mUpperServer_5_IP, mSERVER_INFO.mUpperServer_5_Port)) {
		*tResult = 17;
		return FALSE;
	}
#endif
	mUSER = static_cast<MyUser*> (::calloc (1, (MAX_USER_NUM * sizeof (MyUser))));
	if( mUSER == NULL )
	{
		*tResult = 11;
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
			*tResult = 12;
			return FALSE;
		}
	}
	if( !mWORK.Init() )
	{
		*tResult = 13;
		return FALSE;
	}
	if( !mTRANSFER.Init() )
	{
		*tResult = 14;
		return FALSE;
	}
	if( !mGAME.Init() )
	{
		*tResult = 15;
		return FALSE;
	}

	for(index01 = 0; index01 < MAX_SIZE_OF_RCMD; index01++) {
		mGAME.mRcmdIld[index01] = 0;
		mGAME.mRcmdPtt[index01] = 0;
		mGAME.mRcmdWrd[index01] = 0;
	}

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
			mUSER[index01].Quit(__FILE__, __FUNCTION__, __LINE__);
		}
		for( index01 = 0 ; index01 < MAX_USER_NUM ; index01++ )
		{
			mUSER[index01].Free();
		}
		::free (mUSER);
		mUSER = NULL;
	}
	mGAMELOG.Free();
	mCENTER_COM.Free();
	mPLAYUSER_COM.Free();
#if 1
	mGiftBankComm.Free();
#endif
#ifdef __CHANNEL__
	mCompany.Free();
#endif
	closesocket( mSocket );
	mDB.Free();
}
//PROCESS_FOR_NETWORK
int MyServer::PROCESS_FOR_NETWORK(SOCKET* from, UINT status, void* pArgs )
{
	int index01;
	int tAddressLength;
	SOCKET tSocket = *from;
    in_addr     tAddress;
	int tempUserIndex;
	int tRecvSizeFromUser;
	BYTE tProtocol;
	char tPacketEncryptionValue;
	int tRandomNumber;
    unsigned int rand1 = 0, rand2 = 0;

    tSocket = *from;
	switch (status)
	{
	case FD_READ :
		for( tempUserIndex = 0 ; tempUserIndex < MAX_USER_NUM ; tempUserIndex++ )
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
		if( tempUserIndex == MAX_USER_NUM )
		{
			// 2009.10.26 : 김성수
			// 제거 내용 :mUSER[tempUserIndex].Quit ();
			return -1;
		}

		//AGAIN:
		while(1) {
		        tRecvSizeFromUser = recv( tSocket, &mUSER[tempUserIndex].mBUFFER_FOR_RECV[mUSER[tempUserIndex].mTotalRecvSize], ( MAX_RECV_BUFFER_SIZE - mUSER[tempUserIndex].mTotalRecvSize ), 0 );
		        if( tRecvSizeFromUser < 0 )
		        {
			        if (errno != EAGAIN && errno != EWOULDBLOCK) {
					mUSER[tempUserIndex].Quit ();
				        return 0;
			        }
			        return -1;
		        } else if (tRecvSizeFromUser == 0) {
			        mUSER[tempUserIndex].Quit ();
			        return 0;
		        }
        
		        for( index01 = mUSER[tempUserIndex].mTotalRecvSize ; index01 < ( mUSER[tempUserIndex].mTotalRecvSize + tRecvSizeFromUser ) ; index01++ )
		        {
			        mUSER[tempUserIndex].mBUFFER_FOR_RECV[index01] = static_cast<char> (mUSER[tempUserIndex].mBUFFER_FOR_RECV[index01] ^ mUSER[tempUserIndex].mPacketEncryptionValue[0]);
		        }
		        mUSER[tempUserIndex].mTotalRecvSize += tRecvSizeFromUser;
			break;
		}
        while (true) {
		    if( mUSER[tempUserIndex].mTotalRecvSize < 9 )
    		{
				return -1;
    		}
    		tProtocol = (BYTE) mUSER[tempUserIndex].mBUFFER_FOR_RECV[8];
			//LOG_TO_FILE_2("protocol : %d %d\n", tProtocol, mWORK.mWorkerPacketSize[tProtocol]);
    		if( W_FUNCTION[tProtocol].PROC == NULL )
    		{
    			if( mUSER[tempUserIndex].mCheckValidState )
    			{
	    			mPLAYUSER_COM.U_BLOCK_USER_FOR_LOGIN_2_SEND( 501, &mUSER[tempUserIndex].uID[0] );
		    	}
			    mUSER[tempUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			    //LOG_TO_FILE_3 ("!Block User[%s] - Unknown Header[%d], Length[%d].\n", 
                //              mUSER[tempUserIndex].uID, tProtocol, tRecvSizeFromUser);
			    return 0;
		    }

			if( mUSER[tempUserIndex].mTotalRecvSize < mWORK.mWorkerPacketSize[tProtocol] )
			{
				return -1;
				//LOG_TO_FILE_1("!Did not receive the whole packet. Remains (%d) bytes.\n", 
                //             mWORK.mWorkerPacketSize[tProtocol] - mUSER[tempUserIndex].mTotalRecvSize);
				//goto AGAIN;
				//return -1;
			}
			CopyMemory( &tPacketEncryptionValue, &mUSER[tempUserIndex].mBUFFER_FOR_RECV[7], 1 );
			if( tPacketEncryptionValue != mUSER[tempUserIndex].mPacketEncryptionValue[1] )
			{
				if( mUSER[tempUserIndex].mCheckValidState )
				{
					mPLAYUSER_COM.U_BLOCK_USER_FOR_LOGIN_2_SEND( 502, &mUSER[tempUserIndex].uID[0] );
				}
				mUSER[tempUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
                //LOG_TO_FILE_1 ("!Block User[%s] - Encryption keys are different.\n", 
                //               mUSER[tempUserIndex].uID);
				return 0;
			}
			mUSER[tempUserIndex].mPacketEncryptionValue[1]++;
			W_FUNCTION[tProtocol].PROC( tempUserIndex );
			if( mUSER[tempUserIndex].mCheckConnectState )
			{
				MoveMemory( &mUSER[tempUserIndex].mBUFFER_FOR_RECV[0], &mUSER[tempUserIndex].mBUFFER_FOR_RECV[mWORK.mWorkerPacketSize[tProtocol]], ( mUSER[tempUserIndex].mTotalRecvSize - mWORK.mWorkerPacketSize[tProtocol] ) );
				mUSER[tempUserIndex].mTotalRecvSize -= mWORK.mWorkerPacketSize[tProtocol];
			}
			if (mUSER[tempUserIndex].mTotalRecvSize == 0) break;
		}
		return tRecvSizeFromUser;
	case FD_WRITE :
		for( tempUserIndex = 0 ; tempUserIndex < MAX_USER_NUM ; tempUserIndex++ )
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
		if( tempUserIndex == MAX_USER_NUM )
		{
			return -1;
		}
		mUSER[tempUserIndex].Send( FALSE, NULL, 0 );
		return 0;
	case FD_ACCEPT :
		for( index01 = 0 ; index01 < MAX_USER_NUM ; index01++ )
		{
			if( !mUSER[index01].mCheckConnectState )
			{
				break;
			}
		}
		if( index01 == MAX_USER_NUM )
		{
			closesocket( tSocket );
			return 0;
		}
        rand1 = rand_mir ();
        rand2 = rand_mir ();
		mUSER[index01].mCheckConnectState = TRUE;
		mUSER[index01].mCheckValidState = FALSE;
		mUSER[index01].mUsedTime = GetTickCount();
		mUSER[index01].mSocket = tSocket;
        ::memcpy (&tAddress, pArgs, sizeof (in_addr));
        strncpy (mUSER[index01].mIP, ::inet_ntoa (tAddress), 15);
		mUSER[index01].mIPPart[0] = (tAddress.s_addr & 0xFF);
		mUSER[index01].mIPPart[1] = ((tAddress.s_addr & 0xFF00) >> 8);
		mUSER[index01].mIPPart[2] = ((tAddress.s_addr & 0xFF0000) >> 16);
		mUSER[index01].mIPPart[3] = ((tAddress.s_addr & 0xFF000000) >> 24);
		tRandomNumber = ( rand1 % 1001 ) * ( rand2 % 1001 );
		CopyMemory( &mUSER[index01].mPacketEncryptionValue[0], &tRandomNumber, 1 );
		CopyMemory( &mUSER[index01].mPacketEncryptionValue[1], &tRandomNumber, 1 );
		mUSER[index01].mPacketEncryptionValue[1] = static_cast<char> (mUSER[index01].mPacketEncryptionValue[1] + 127);
		mUSER[index01].mTotalSendSize = 0;
		mUSER[index01].mTotalRecvSize = 0;
		::memset(&mUSER[index01].uSaveItem, 0, MAX_SAVE_ITEM_SLOT_NUM * 4 * sizeof(int));
		::memset(&mUSER[index01].mGiftInfo, 0, 10);
		::memset(&mUSER[index01].mAvatarInfo, 0, (sizeof(AVATAR_INFO) * MAX_USER_AVATAR_NUM));
		mTRANSFER.B_CONNECT_OK( tRandomNumber, mGAME.mMaxPlayerNum, mGAME.mGagePlayerNum, ( mGAME.mPresentPlayerNum + mGAME.mAddPlayerNum ) );
		mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
		return 1;
	case FD_CLOSE :
		for( tempUserIndex = 0 ; tempUserIndex < MAX_USER_NUM ; tempUserIndex++ )
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
		if( tempUserIndex == MAX_USER_NUM )
		{
			return 0;
		}
		mUSER[tempUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return 0;
	}
}
//PROCESS_FOR_TIMER
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
		if( ( tPresentTime - mUSER[index01].mUsedTime ) > 60000 )
		{
			mUSER[index01].Quit(__FILE__, __FUNCTION__, __LINE__);
			continue;
		}
		if( !mUSER[index01].mCheckValidState )
		{
			continue;
		}
		if( ( tPresentTime - mUSER[index01].mRegisterTime ) > 180000 )
		{
			if( mUSER[index01].mMoveZoneResult == 0 )
			{
				mPLAYUSER_COM.U_REGISTER_USER_FOR_LOGIN_2_SEND( mUSER[index01].mPlayUserIndex, &mUSER[index01].uID[0] );
				if( mPLAYUSER_COM.mRecv_Result != 0 )
				{
					mUSER[index01].Quit(__FILE__, __FUNCTION__, __LINE__);
					continue;
				}
				mUSER[index01].mRegisterTime = tPresentTime;
			}
		}
	}

	mPLAYUSER_COM.U_LOGIN_OK_FOR_PLAYUSER_SEND();
	mCENTER_COM.U_LOGIN_OK_FOR_CENTER_SEND();
	mDB.DB_PROCESS_01();
	if( mGAME.mMaxPlayerNum > 0 )
	{
		mPLAYUSER_COM.U_GET_PRESENT_USER_NUM_FOR_LOGIN_SEND();
		mGAME.mPresentPlayerNum = mPLAYUSER_COM.mRecv_PresentUserNum;
	}
}
//-------------------------------------------------------------------------------------------------
