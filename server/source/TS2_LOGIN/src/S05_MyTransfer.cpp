//-------------------------------------------------------------------------------------------------
//HEADER
//-------------------------------------------------------------------------------------------------
#include "H01_MainApplication.h"
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_TRANSFER
//-------------------------------------------------------------------------------------------------
//INSTANCE
MyTransfer mTRANSFER;
//CREATE
MyTransfer::MyTransfer( void )
{
}
//INIT
BOOL MyTransfer::Init( void )
{
	return TRUE;
}
//FREE
void MyTransfer::Free( void )
{
}
//B_CONNECT_OK
void MyTransfer::B_CONNECT_OK( int tRandomNumber, int tMaxPlayerNum, int tGagePlayerNum, int tPresentPlayerNum )
{
	int i = 0;
	mOriginal[0] = 0;
	CopyMemory( &mOriginal[1], &tRandomNumber, 4 );
	CopyMemory( &mOriginal[5], &tMaxPlayerNum, 4 );
	CopyMemory( &mOriginal[9], &tGagePlayerNum, 4 );
	CopyMemory( &mOriginal[13], &tPresentPlayerNum, 4 );
	for(i = 0; i < MAX_SIZE_OF_RCMD; i++) {
		mOriginal[(i + 17)] = mGAME.mRcmdWrd[i % MAX_SIZE_OF_RCMD];
	}
	mOriginalSize = 17 + MAX_SIZE_OF_RCMD;
}
//B_LOGIN_RECV
void MyTransfer::B_LOGIN_RECV( int tResult, char tID[MAX_USER_ID_LENGTH], int tUserSort, int tGoodFellow, int tLoginPlace, int tLoginPremium, int tLoginPremiumPCRoom, int tSecondLoginSort, char tMousePassword[MAX_MOUSE_PASSWORD_LENGTH], int tSecretCardIndex01, int tSecretCardIndex02, char tFirstLogin )
{
	mOriginal[0] = P_LOGIN_RECV;
	CopyMemory( &mOriginal[1], &tResult, 4 );
	CopyMemory( &mOriginal[5], &tID[0], MAX_USER_ID_LENGTH );
	CopyMemory( &mOriginal[( 5 + MAX_USER_ID_LENGTH )], &tUserSort, 4 );
	CopyMemory( &mOriginal[( 9 + MAX_USER_ID_LENGTH )], &tGoodFellow, 4 );
	CopyMemory( &mOriginal[( 13 + MAX_USER_ID_LENGTH )], &tLoginPlace, 4 );
	CopyMemory( &mOriginal[( 17 + MAX_USER_ID_LENGTH )], &tLoginPremium, 4 );
	CopyMemory( &mOriginal[( 21 + MAX_USER_ID_LENGTH )], &tLoginPremiumPCRoom, 4 );
	CopyMemory( &mOriginal[( 25 + MAX_USER_ID_LENGTH )], &tSecondLoginSort, 4 );
	CopyMemory( &mOriginal[( 29 + MAX_USER_ID_LENGTH )], &tMousePassword[0], MAX_MOUSE_PASSWORD_LENGTH );
	CopyMemory( &mOriginal[( 29 + MAX_USER_ID_LENGTH + MAX_MOUSE_PASSWORD_LENGTH )], &tSecretCardIndex01, 4 );
	CopyMemory( &mOriginal[( 33 + MAX_USER_ID_LENGTH + MAX_MOUSE_PASSWORD_LENGTH )], &tSecretCardIndex02, 4 );
	CopyMemory( &mOriginal[( 37 + MAX_USER_ID_LENGTH + MAX_MOUSE_PASSWORD_LENGTH )], &tFirstLogin, 1);
	mOriginalSize = S_LOGIN_RECV + 1;
}
//B_USER_AVATAR_INFO
void MyTransfer::B_USER_AVATAR_INFO( AVATAR_INFO *tAvatarInfo )
{
	int temp_Criminal_Record	= tAvatarInfo->iCriminal_Record;
	int temp_Enter_The_Prison	= tAvatarInfo->iEnter_The_Prison;
	int temp_Monster_Count		= tAvatarInfo->iKill_Monster_Count;

	tAvatarInfo->iCriminal_Record	= 0;
	tAvatarInfo->iEnter_The_Prison	=	0;
	tAvatarInfo->iKill_Monster_Count = 0;

	mOriginal[0] = P_USER_AVATAR_INFO;
	CopyMemory( &mOriginal[1], tAvatarInfo, sizeof( AVATAR_INFO ) );
	mOriginalSize = S_USER_AVATAR_INFO;

	tAvatarInfo->iCriminal_Record	= temp_Criminal_Record;
	tAvatarInfo->iEnter_The_Prison	=	temp_Enter_The_Prison;
	tAvatarInfo->iKill_Monster_Count = temp_Monster_Count;
}
//B_CREATE_MOUSE_PASSWORD_RECV
void MyTransfer::B_CREATE_MOUSE_PASSWORD_RECV( int tResult, char tMousePassword[MAX_MOUSE_PASSWORD_LENGTH] )
{
	mOriginal[0] = P_CREATE_MOUSE_PASSWORD_RECV;
	CopyMemory( &mOriginal[1], &tResult, 4 );
	CopyMemory( &mOriginal[5], &tMousePassword[0], MAX_MOUSE_PASSWORD_LENGTH );
	mOriginalSize = S_CREATE_MOUSE_PASSWORD_RECV;
}
//B_CHANGE_MOUSE_PASSWORD_RECV
void MyTransfer::B_CHANGE_MOUSE_PASSWORD_RECV( int tResult, char tMousePassword[MAX_MOUSE_PASSWORD_LENGTH] )
{
	mOriginal[0] = P_CHANGE_MOUSE_PASSWORD_RECV;
	CopyMemory( &mOriginal[1], &tResult, 4 );
	CopyMemory( &mOriginal[5], &tMousePassword[0], MAX_MOUSE_PASSWORD_LENGTH );
	mOriginalSize = S_CHANGE_MOUSE_PASSWORD_RECV;
}
//B_LOGIN_MOUSE_PASSWORD_RECV
void MyTransfer::B_LOGIN_MOUSE_PASSWORD_RECV( int tResult )
{
	mOriginal[0] = P_LOGIN_MOUSE_PASSWORD_RECV;
	CopyMemory( &mOriginal[1], &tResult, 4 );
	mOriginalSize = S_LOGIN_MOUSE_PASSWORD_RECV;
}
//B_LOGIN_SECRET_CARD_RECV
void MyTransfer::B_LOGIN_SECRET_CARD_RECV( int tResult )
{
	mOriginal[0] = P_LOGIN_SECRET_CARD_RECV;
	CopyMemory( &mOriginal[1], &tResult, 4 );
	mOriginalSize = S_LOGIN_SECRET_CARD_RECV;
}
//B_CREATE_AVATAR_RECV
void MyTransfer::B_CREATE_AVATAR_RECV( int tResult, AVATAR_INFO *tAvatarInfo )
{
	mOriginal[0] = P_CREATE_AVATAR_RECV;
	CopyMemory( &mOriginal[1], &tResult, 4 );
	CopyMemory( &mOriginal[5], tAvatarInfo, sizeof( AVATAR_INFO ) );
	mOriginalSize = S_CREATE_AVATAR_RECV;
}
//B_DELETE_AVATAR_RECV
void MyTransfer::B_DELETE_AVATAR_RECV( int tResult )
{
	mOriginal[0] = P_DELETE_AVATAR_RECV;
	CopyMemory( &mOriginal[1], &tResult, 4 );
	mOriginalSize = S_DELETE_AVATAR_RECV;
}
//B_CHANGE_AVATAR_NAME_RECV
void MyTransfer::B_CHANGE_AVATAR_NAME_RECV( int tResult )
{
	mOriginal[0] = P_CHANGE_AVATAR_NAME_RECV;
	CopyMemory( &mOriginal[1], &tResult, 4 );
	mOriginalSize = S_CHANGE_AVATAR_NAME_RECV;
}
//B_DEMAND_GIFT_RECV
void MyTransfer::B_DEMAND_GIFT_RECV( int tResult, int tGiftInfo[10] )
{
	mOriginal[0] = P_DEMAND_GIFT_RECV;
	CopyMemory( &mOriginal[1], &tResult, 4 );
	CopyMemory( &mOriginal[5], &tGiftInfo[0], 40 );
	mOriginalSize = S_DEMAND_GIFT_RECV;
}
//B_WANT_GIFT_RECV
void MyTransfer::B_WANT_GIFT_RECV( int tResult )
{
	mOriginal[0] = P_WANT_GIFT_RECV;
	CopyMemory( &mOriginal[1], &tResult, 4 );
	mOriginalSize = S_WANT_GIFT_RECV;
}
//B_DEMAND_ZONE_SERVER_INFO_1_RESULT
void MyTransfer::B_DEMAND_ZONE_SERVER_INFO_1_RESULT( int tResult, char tIP[16], int tPort )
{
	mOriginal[0] = P_DEMAND_ZONE_SERVER_INFO_1_RESULT;
	CopyMemory( &mOriginal[1], &tResult, 4 );
	CopyMemory( &mOriginal[5], &tIP[0], 16 );
	CopyMemory( &mOriginal[21], &tPort, 4 );
	mOriginalSize = S_DEMAND_ZONE_SERVER_INFO_1_RESULT;
}
//B_RCMD_WORLD_SEND
void MyTransfer::B_RCMD_WORLD_SEND()
{
	int i = 0;

	mOriginal[0] = P_RECOMMAND_WORLD_RECV;
	for(i = 0; i < MAX_SIZE_OF_RCMD; i++) {
		mOriginal[(i * 3) + 1] = mGAME.mRcmdIld[i % MAX_SIZE_OF_RCMD];
		mOriginal[(i * 3) + 2] = mGAME.mRcmdPtt[i % MAX_SIZE_OF_RCMD];
		mOriginal[(i * 3) + 3] = mGAME.mRcmdWrd[i % MAX_SIZE_OF_RCMD];
	}
	mOriginalSize = S_RECOMMAND_WORLD_RECV;
}
//-------------------------------------------------------------------------------------------------
