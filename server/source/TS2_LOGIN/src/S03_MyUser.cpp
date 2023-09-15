//-------------------------------------------------------------------------------------------------
//HEADER
//-------------------------------------------------------------------------------------------------
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
}
//INIT
BOOL MyUser::Init( int tSendBufferSize, int tRecvBufferSize )
{
	mCheckConnectState = FALSE;
    mBUFFER_FOR_SEND = static_cast<char*> (::calloc (1, tSendBufferSize));
	if( mBUFFER_FOR_SEND == NULL )
	{
		return FALSE;
	}
    mBUFFER_FOR_RECV = static_cast<char*> (::calloc (1, tRecvBufferSize));
	if( mBUFFER_FOR_RECV == NULL )
	{
		return FALSE;
	}
	return TRUE;
}
//FREE
void MyUser::Free( void )
{
	if( mBUFFER_FOR_SEND != NULL )
	{
		::free (mBUFFER_FOR_SEND);
		mBUFFER_FOR_SEND = NULL;
	}
	if( mBUFFER_FOR_RECV != NULL )
	{
		::free (mBUFFER_FOR_RECV);
		mBUFFER_FOR_RECV = NULL;
	}
}
//SEND
void MyUser::Send( BOOL tCheckValidBuffer, char *tBuffer, int tBufferSize )
{
	if( !mCheckConnectState )
	{
		return;
	}

	int tSendValue01;

	if( tCheckValidBuffer )
	{
		if( ( MAX_SEND_BUFFER_SIZE - mTotalSendSize ) < tBufferSize )
		{
			Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
		CopyMemory( &mBUFFER_FOR_SEND[mTotalSendSize], &tBuffer[0], tBufferSize );
		mTotalSendSize += tBufferSize;
	}
	while( mTotalSendSize > 0 )
	{
		tSendValue01 = send( mSocket, &mBUFFER_FOR_SEND[0], mTotalSendSize, 0 );
        if (tSendValue01 <= 0) {
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                Quit ();
                return ;
            }
            return;
        }
		MoveMemory( &mBUFFER_FOR_SEND[0], &mBUFFER_FOR_SEND[tSendValue01], ( mTotalSendSize - tSendValue01 ) );
		mTotalSendSize -= tSendValue01;
	}
}
//QUIT
void MyUser::Quit(const char *tFileName, const char *tFuncName, int tLine)
{
	if( !mCheckConnectState )
	{
		return;
	}
	mCheckConnectState = FALSE;
	if( ( mCheckValidState ) && ( mMoveZoneResult == 0 ) )
	{
		mPLAYUSER_COM.U_UNREGISTER_USER_FOR_LOGIN_SEND( mPlayUserIndex, &uID[0] );
	}
	mTotalRecvSize = 0;
	mTotalSendSize = 0;
	mCheckValidState = FALSE;
	closesocket( mSocket );
	
	LOG_TO_FILE_2("![%s] : %s ", tFuncName, uID);
	LOG_TO_FILE_2(" : %s : %d\n",  tFileName, tLine);
}
//-------------------------------------------------------------------------------------------------
