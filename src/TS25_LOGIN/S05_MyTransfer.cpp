//-------------------------------------------------------------------------------------------------
//HEADER
//-------------------------------------------------------------------------------------------------
#include "Shengming.h"
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_TRANSFER
//-------------------------------------------------------------------------------------------------
//INSTANCE
MyTransfer mTRANSFER;
//CREATE
MyTransfer::MyTransfer(void)
{
}

//INIT
BOOL MyTransfer::Init(void)
{
	return TRUE;
}

//FREE
void MyTransfer::Free(void)
{
}

//B_CONNECT_OK
void MyTransfer::B_CONNECT_OK(int tRandomNumber, int tMaxPlayerNum, int tGagePlayerNum, int tPresentPlayerNum)
{
	int i = 0;
	mOriginal[0] = 0;
	CopyMemory(&mOriginal[1], &tRandomNumber, 4);
	CopyMemory(&mOriginal[5], &tMaxPlayerNum, 4);
	CopyMemory(&mOriginal[9], &tGagePlayerNum, 4);
	CopyMemory(&mOriginal[13], &tPresentPlayerNum, 4);
	mOriginalSize = 17;
}

//B_LOGIN_RECV
void MyTransfer::B_LOGIN_RECV(int tResult, char tID[MAX_USER_ID_LENGTH], int tUserSort, int tSecondLoginSort,
                              char tMousePassword[MAX_MOUSE_PASSWORD_LENGTH], char tFirstLogin)
{
	mOriginal[0] = P_LOGIN_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &tID[0], MAX_USER_ID_LENGTH);
	CopyMemory(&mOriginal[(5 + MAX_USER_ID_LENGTH)], &tUserSort, 4);
	CopyMemory(&mOriginal[(21 + MAX_USER_ID_LENGTH)], &tSecondLoginSort, 4);
	CopyMemory(&mOriginal[(20 + MAX_USER_ID_LENGTH + 5)], &tMousePassword[0], MAX_MOUSE_PASSWORD_LENGTH);
	CopyMemory(&mOriginal[(30 + MAX_USER_ID_LENGTH + MAX_MOUSE_PASSWORD_LENGTH)], &tFirstLogin, 1);
	mOriginalSize = S_LOGIN_RECV;
}

//B_USER_AVATAR_INFO
void MyTransfer::B_USER_AVATAR_INFO(AVATAR_INFO* tAvatarInfo)
{
	mOriginal[0] = P_USER_AVATAR_INFO;
	CopyMemory(&mOriginal[1], tAvatarInfo, SIZE_OF_AVATAR_INFO);
	mOriginalSize = S_USER_AVATAR_INFO;
}

//B_CREATE_MOUSE_PASSWORD_RECV
void MyTransfer::B_CREATE_MOUSE_PASSWORD_RECV(int tResult, char tMousePassword[MAX_MOUSE_PASSWORD_LENGTH])
{
	mOriginal[0] = P_CREATE_MOUSE_PASSWORD_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &tMousePassword[0], MAX_MOUSE_PASSWORD_LENGTH);
	mOriginalSize = S_CREATE_MOUSE_PASSWORD_RECV;
}

//B_CHANGE_MOUSE_PASSWORD_RECV
void MyTransfer::B_CHANGE_MOUSE_PASSWORD_RECV(int tResult, char tMousePassword[MAX_MOUSE_PASSWORD_LENGTH])
{
	mOriginal[0] = P_CHANGE_MOUSE_PASSWORD_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &tMousePassword[0], MAX_MOUSE_PASSWORD_LENGTH);
	mOriginalSize = S_CHANGE_MOUSE_PASSWORD_RECV;
}

//B_LOGIN_MOUSE_PASSWORD_RECV
void MyTransfer::B_LOGIN_MOUSE_PASSWORD_RECV(int tResult)
{
	mOriginal[0] = P_LOGIN_MOUSE_PASSWORD_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	mOriginalSize = S_LOGIN_MOUSE_PASSWORD_RECV;
}

//B_LOGIN_SECRET_CARD_RECV
void MyTransfer::B_LOGIN_SECRET_CARD_RECV(int tResult)
{
	mOriginal[0] = P_LOGIN_SECRET_CARD_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	mOriginalSize = S_LOGIN_SECRET_CARD_RECV;
}

//B_CREATE_AVATAR_RECV
void MyTransfer::B_CREATE_AVATAR_RECV(int tResult, AVATAR_INFO* tAvatarInfo)
{
	mOriginal[0] = P_CREATE_AVATAR_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], tAvatarInfo, SIZE_OF_AVATAR_INFO);
	mOriginalSize = S_CREATE_AVATAR_RECV;
}

//B_DELETE_AVATAR_RECV
void MyTransfer::B_DELETE_AVATAR_RECV(int tResult)
{
	mOriginal[0] = P_DELETE_AVATAR_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	mOriginalSize = S_DELETE_AVATAR_RECV;
}

//B_CHANGE_AVATAR_NAME_RECV
void MyTransfer::B_CHANGE_AVATAR_NAME_RECV(int tResult)
{
	mOriginal[0] = P_CHANGE_AVATAR_NAME_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	mOriginalSize = S_CHANGE_AVATAR_NAME_RECV;
}

//B_DEMAND_GIFT_RECV
void MyTransfer::B_DEMAND_GIFT_RECV(int tResult, int tGiftInfo[10])
{
	mOriginal[0] = P_DEMAND_GIFT_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &tGiftInfo[0], 40);
	mOriginalSize = S_DEMAND_GIFT_RECV;
}

//B_WANT_GIFT_RECV
void MyTransfer::B_WANT_GIFT_RECV(int tResult)
{
	mOriginal[0] = P_WANT_GIFT_RECV;
	CopyMemory(&mOriginal[1], &tResult, 4);
	mOriginalSize = S_WANT_GIFT_RECV;
}

//B_DEMAND_ZONE_SERVER_INFO_1_RESULT
void MyTransfer::B_DEMAND_ZONE_SERVER_INFO_1_RESULT(int tResult, char tIP[16], int tPort, int tZone)
{
	mOriginal[0] = P_DEMAND_ZONE_SERVER_INFO_1_RESULT;
	CopyMemory(&mOriginal[1], &tResult, 4);
	CopyMemory(&mOriginal[5], &tIP[0], 16);
	CopyMemory(&mOriginal[21], &tPort, 4);
	CopyMemory(&mOriginal[25], &tZone, 4);
	mOriginalSize = S_DEMAND_ZONE_SERVER_INFO_1_RESULT;
}

//B_RCMD_WORLD_SEND
void MyTransfer::B_RCMD_WORLD_SEND()
{
	int i = 0;

	mOriginal[0] = P_RECOMMAND_WORLD_RECV;
	for (i = 0; i < MAX_SIZE_OF_RCMD; i++)
	{
		mOriginal[(i * 3) + 1] = mGAME.mRcmdIld[i % MAX_SIZE_OF_RCMD];
		mOriginal[(i * 3) + 2] = mGAME.mRcmdPtt[i % MAX_SIZE_OF_RCMD];
		mOriginal[(i * 3) + 3] = mGAME.mRcmdWrd[i % MAX_SIZE_OF_RCMD];
	}
	mOriginalSize = S_RECOMMAND_WORLD_RECV;
}

//-------------------------------------------------------------------------------------------------
