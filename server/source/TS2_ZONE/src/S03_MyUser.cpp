//-------------------------------------------------------------------------------------------------
//HEADER
//-------------------------------------------------------------------------------------------------
#include <sys/epoll.h>
#include "H01_MainApplication.h"
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_USER
//-------------------------------------------------------------------------------------------------
//INSTANCE
MyUser *mUSER = NULL;
//CREATE
MyUser::MyUser( void )
{
	mBUFFER_FOR_SEND = NULL;
	mBUFFER_FOR_RECV = NULL;
	//---------//
	//N-PROTECT//
	//---------//
#ifdef __N_PROTECT__	
	mNPROTECT = NULL;
#endif
	//---------//
	//---------//
	//---------//
}
//INIT
BOOL MyUser::Init( int tUserIndex, int tSendBufferSize, int tRecvBufferSize )
{
    m_bUDPActivated = false;
#ifndef __ORG__
	m_bUDPArrivedAtClient = false;
#endif
	m_iTryCountForAuth = 0;
	mUserIndex = tUserIndex;
#ifdef __GOD__
	mCheckConnectState = FALSE;
#endif
	
#ifdef __WIN32__
	mBUFFER_FOR_SEND = (char *) GlobalAlloc( GMEM_FIXED, tSendBufferSize );
#else
	mBUFFER_FOR_SEND = (char *)calloc(1, tSendBufferSize);
#endif
	if( mBUFFER_FOR_SEND == NULL )
	{
		return FALSE;
	}
#ifdef __WIN32__
	mBUFFER_FOR_RECV = (char *) GlobalAlloc( GMEM_FIXED, tRecvBufferSize );
#else
	mBUFFER_FOR_RECV = (char *)calloc(1, tRecvBufferSize);
#endif
#ifdef __LOCK__
	if(pthread_mutex_init(&m_user_lock, NULL) != 0) {
		return false;
	}
    if(pthread_mutex_init(&m_user_quit_lock, NULL) != 0) {
        return false;
    }
#endif
	if( mBUFFER_FOR_RECV == NULL )
	{
		return FALSE;
	}

	//@{ auto quize
	m_bIsBonusRecv = FALSE;
	m_bIsSend_Question = FALSE;			
	::ZeroMemory( m_tSphinx_Question, sizeof(m_tSphinx_Question) );
	m_dSend_Time = 0;
	m_iWrong_Count = 0;
	//@}

	//---------//
	//N-PROTECT//
	//---------//
#ifdef __N_PROTECT__
	mNPROTECT = new CCSAuth2();
	if( mNPROTECT == NULL )
	{
		return FALSE;
	}
	mNPROTECT->Init();
#endif
	//---------//
	//---------//
	//---------//
	return TRUE;
}
//FREE
void MyUser::Free( void )
{
    m_bUDPActivated = false;
#ifndef __ORG__
	m_bUDPArrivedAtClient = false;
#endif
	if( mBUFFER_FOR_SEND != NULL )
	{
		//GlobalFree( mBUFFER_FOR_SEND );
		free( mBUFFER_FOR_SEND );
		mBUFFER_FOR_SEND = NULL;
	}
	if( mBUFFER_FOR_RECV != NULL )
	{
		//GlobalFree( mBUFFER_FOR_RECV );
		free( mBUFFER_FOR_RECV );
		mBUFFER_FOR_RECV = NULL;
	}
	//---------//
	//N-PROTECT//
	//---------//
#ifdef __N_PROTECT__	
	if( mNPROTECT != NULL )
	{
		delete mNPROTECT;
		mNPROTECT = NULL;
	}
#endif
	//---------//
	//---------//
	//---------//

	//@{ auto quiz
	m_bIsSend_Question = FALSE;			
	::ZeroMemory( m_tSphinx_Question, sizeof(m_tSphinx_Question) );
	m_dSend_Time = 0;
	m_iWrong_Count = 0;
	//@}

	bUsePackItem = false;
}
//SEND
void MyUser::Send( BOOL tCheckValidBuffer, char *tBuffer, int tBufferSize, bool bUDPSend)
{ 
	if( !mCheckConnectState ) {
		return;
	}

	int nResult = 0;
	int tSendValue01 = 0;

	if (bUDPSend == true && m_bUDPActivated == true && m_bUDPArrivedAtClient == true) {
		if(sendto(g_BroadCastMgr.GetHandle(), tBuffer, tBufferSize, 0
					, LPSOCKADDR&m_AddrFrom, sizeof(SOCKADDR_IN)) < 0) {
			//LOG_TO_FILE_1("Udp error : %s\n", strerror(errno));
			return;
		}
		return;
	}

#ifdef __LOCK__
	if(pthread_mutex_lock(&m_user_lock) != 0) {
		return;
	}
#endif
	if( tCheckValidBuffer )
	{
		if( ( mSERVER.mMAX_SEND_BUFFER_SIZE - mTotalSendSize ) < tBufferSize )
		{
#if 0
			LOG_TO_FILE_4("![%s] : Send buffer full!(%d, %d) - %s\n", __FUNCTION__
					, ( mSERVER.mMAX_SEND_BUFFER_SIZE - mTotalSendSize )
					, tBufferSize, mAvatarInfo.aName);
#endif
			Quit(__FILE__, __FUNCTION__, __LINE__);
#ifdef __LOCK__
			if(pthread_mutex_unlock(&m_user_lock) != 0) {
				return;
			}
#endif
			return;
		}
		if(mTotalSendSize < 0) 
			mTotalSendSize = 0;
		::memcpy(mBUFFER_FOR_SEND + mTotalSendSize, tBuffer, tBufferSize);
		mTotalSendSize += tBufferSize;
	}

	while( mTotalSendSize > 0 )
	{
		tSendValue01 = send( mSocket, &mBUFFER_FOR_SEND[0], mTotalSendSize, 0 );
		if(tSendValue01 < 0) {
			if(errno == EINTR) {
				continue;
			}
			if(errno != EAGAIN && errno != EWOULDBLOCK) {
				//LOG_TO_FILE_2("![%s] : Error(%s)\n", __FUNCTION__, strerror(errno));
				Quit(__FILE__, __FUNCTION__, __LINE__);
#ifdef __LOCK__
				if(pthread_mutex_unlock(&m_user_lock) != 0) {
					return;
				}
#endif
				return;

			}
#ifdef __LOCK__
		    if(pthread_mutex_unlock(&m_user_lock) != 0) {
			    return;
			}
#endif
			return;
		} else if(tSendValue01 == 0) {
			Quit(__FILE__, __FUNCTION__, __LINE__);
#ifdef __LOCK__
			if(pthread_mutex_unlock(&m_user_lock) != 0) {
				return;
			}
#endif
			return;
		} 

		if((mTotalSendSize - tSendValue01) > 0) {
			::memmove(mBUFFER_FOR_SEND, mBUFFER_FOR_SEND + tSendValue01,
					( mTotalSendSize - tSendValue01 ) );
		} 
		mTotalSendSize -= tSendValue01;
		if(mTotalSendSize < 0) {
			mTotalSendSize = 0;
		}
	}
#ifdef __LOCK__
	if(pthread_mutex_unlock(&m_user_lock) != 0) {
		return;
	}
#endif
}
//QUIT
//void MyUser::Quit( void )
void MyUser::Quit(const char *tFileName, const char *tFuncName, int tLine)
{
#ifdef __LOCK__
    if(pthread_mutex_lock(&m_user_quit_lock) != 0) {
        LOG_TO_FILE_1("![Quit] : ID(%s) MUTEX LOCK ERROR!!\n", mUSER[mUserIndex].uID);
    }
#endif
	//SVR_TRACE();
	if( !mCheckConnectState )
	{
		return;
	}

	closesocket( mSocket );
	mSocket = -1;
	mCheckConnectState = FALSE;

	mTotalSendSize = 0;
	mTotalRecvSize = 0;

	m_iTryCountForAuth = 0;

	/** 2009. 11. 25 : 패키지 아이템 개발(wolf) */
	bUsePackItem = false;
	/** 2009. 11. 25 : 패키지 아이템 개발(wolf) */

    // ÇØ½¬ Å×ÀÌºí¿¡¼­ À¯Àú Á¤º¸¸¦ »èÁ¦ÇÕ´Ï´Ù.
	m_bUDPActivated = false;
#ifndef __ORG__
	m_bUDPArrivedAtClient = false;
#endif
    g_BroadCastMgr.Delete (mAvatarInfo.aName);


	//@{ auto quiz
	if ( TRUE == IsSend_Question() )
	{
		mAvatarInfo.iEnter_The_Prison = 1;
		mAvatarInfo.aKillOtherTribe = mAvatarInfo.aKillOtherTribe - 1;
		if ( 0 >= mAvatarInfo.aKillOtherTribe ) mAvatarInfo.aKillOtherTribe = 0;
		else;
	}
	//@}

	int tSort;
	BYTE tData[MAX_BROADCAST_DATA_SIZE];

	//*mCliFd = -1;
	//epoll_ctl(mEPFD, EPOLL_CTL_DEL, mSocket, NULL);
	//---------//
	//N-PROTECT//
	//---------//
#ifdef __N_PROTECT__
	mNPROTECT->Close();
#endif
	//---------//
	//---------//
	//---------//
	if( ( mCheckValidState ) && ( mMoveZoneResult == 0 ) )
	{
		mCheckValidState = FALSE;

		if( strcmp( mAvatarInfo.aPartyName, "" ) != 0 )
		{
			if( strcmp( mAvatarInfo.aPartyName, mAvatarInfo.aName ) != 0 )
			{
				tSort = 106;
				CopyMemory( &tData[0], &mAvatarInfo.aPartyName[0], MAX_PARTY_NAME_LENGTH );
				CopyMemory( &tData[MAX_PARTY_NAME_LENGTH], &mAvatarInfo.aName[0], MAX_AVATAR_NAME_LENGTH );
				mRELAY_COM.U_DEMAND_BROADCAST_DATA( tSort, &tData[0] );
			}
			else
			{
				tSort = 109;
				CopyMemory( &tData[0], &mAvatarInfo.aPartyName[0], MAX_PARTY_NAME_LENGTH );
				tData[MAX_PARTY_NAME_LENGTH] = '\0';
				mRELAY_COM.U_DEMAND_BROADCAST_DATA( tSort, &tData[0] );
			}
		}
		mAvatarInfo.aLogoutInfo[0] = mSERVER_INFO.mServerNumber;
		mAvatarInfo.aLogoutInfo[1] = (int) mAVATAR_OBJECT[mUserIndex].mDATA.aAction.aLocation[0];
		mAvatarInfo.aLogoutInfo[2] = (int) mAVATAR_OBJECT[mUserIndex].mDATA.aAction.aLocation[1];
		mAvatarInfo.aLogoutInfo[3] = (int) mAVATAR_OBJECT[mUserIndex].mDATA.aAction.aLocation[2];
		mAvatarInfo.aLogoutInfo[4] = mAVATAR_OBJECT[mUserIndex].mDATA.aLifeValue;
		mAvatarInfo.aLogoutInfo[5] = mAVATAR_OBJECT[mUserIndex].mDATA.aManaValue;
		mPLAYUSER_COM.U_UNREGISTER_USER_FOR_ZONE_SEND( mPlayUserIndex, &uID[0], &mAvatarInfo, &mAuthInfo );
	}
	mCheckValidState = FALSE;
	mAVATAR_OBJECT[mUserIndex].mCheckValidState = false;
	::memset(&mAvatarInfo, 0x00, sizeof(AVATAR_INFO));
	//mAVATAR_OBJECT[mUserIndex].Init();

	//@{ auto quiz
	m_bIsBonusRecv = FALSE;
	m_bIsSend_Question = FALSE;			
	::ZeroMemory( m_tSphinx_Question, sizeof(m_tSphinx_Question) );
	m_dSend_Time = 0;
	m_iWrong_Count = 0;
	//@}
	
	LOG_TO_FILE_4("![%s] : %s : %s : %d\n", tFuncName, mUSER[mUserIndex].uID, tFileName, tLine);
#ifdef __LOCK__
    if(pthread_mutex_unlock(&m_user_quit_lock) != 0) {
        LOG_TO_FILE_1("![Quit] : ID(%s) MUTEX UNLOCK ERROR!!\n", mUSER[mUserIndex].uID);
    }
#endif
}

BOOL MyUser::IsSend_TimeOver( void )
{
	if ( FALSE == IsSend_Question() ) return FALSE; 
	else;

	if ( ( ( GetTickCount() - m_dSend_Time ) / 1000 ) > (  LIMIT_SEND_TIME_MINUTE * 60 )  )
	{
		return TRUE;
	}
	else;

	return FALSE;
}
BOOL MyUser::IsWrong_CountOver( void )
{
	m_iWrong_Count++;

	if ( MAX_WRONG_ANSWER_COUNT <= m_iWrong_Count )
	{
		m_iWrong_Count = 0;
		return TRUE;
	}
	else;

	return FALSE;
}
bool MyUser::IsValidSkillNumber(int skillNumber)
{
    for (int index = 0; index < MAX_SKILL_SLOT_NUM; ++index) {
        if (mAvatarInfo.aSkill[index][0] == skillNumber) {
            return true;
        }
    }
    return false;
}
int MyUser::GetMaxSkillGradeNum(int skillNumber)
{
    for (int index = 0; index < MAX_SKILL_SLOT_NUM; ++index) {
        if (mAvatarInfo.aSkill[index][0] == skillNumber) {
            return mAvatarInfo.aSkill[index][1];
        }
    }
    return -1;
}
//-------------------------------------------------------------------------------------------------
