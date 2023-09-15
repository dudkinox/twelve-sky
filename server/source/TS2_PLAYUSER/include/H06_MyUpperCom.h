//-------------------------------------------------------------------------------------------------
//MY_UPPER_COM_H
//-------------------------------------------------------------------------------------------------
#ifndef MY_UPPER_COM_H
#define MY_UPPER_COM_H
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_GAMELOG
//-------------------------------------------------------------------------------------------------
//DEFINE
class MyGameLog
{

private :

	BOOL mCheckConnectState;

	SOCKET mSocket;
	SOCKADDR_IN mAddress;

	int mLogSort;
	char mGameLogData[MAX_GAMELOG_LENGTH];
	char mSendBuffer[( MAX_GAMELOG_LENGTH * 2 )];

	BOOL CheckValidState( char tID[MAX_USER_ID_LENGTH] );

public :

	MyGameLog( void );

	BOOL Init( char tIP[16], int tPort );
	void Free( void );

	void SendToLogServer( void );
	void SendToLogServerForDB( void );

	void GL_000_PLAY_LOG( int uLogType, int uPlayTime, char uID[MAX_USER_ID_LENGTH], char uIP[16], int uLoginPremium, int uWorld, char aName[MAX_AVATAR_NAME_LENGTH], int aTribe, int aLevel, int aExperience, int aKillOtherTribe, int aMoney, int aStoreMoney, int uZoneNumber );

	void GL_101_LOGIN( char tID[MAX_USER_ID_LENGTH], char tIP[16] );
	void GL_102_LOGOUT_1( char tID[MAX_USER_ID_LENGTH] );
	void GL_103_WAIT_LOGOUT_1( char tID[MAX_USER_ID_LENGTH] );
	void GL_104_ENTER_ZONE( char tID[MAX_USER_ID_LENGTH], char tAvatarName[MAX_AVATAR_NAME_LENGTH], int tZoneNumber );
	void GL_105_MOVE_ZONE( char tID[MAX_USER_ID_LENGTH], char tAvatarName[MAX_AVATAR_NAME_LENGTH], int tZoneNumber );
	void GL_106_LOGOUT_2( char tID[MAX_USER_ID_LENGTH], char tAvatarName[MAX_AVATAR_NAME_LENGTH], int tZoneNumber );
	void GL_107_WAIT_LOGOUT_2( char tID[MAX_USER_ID_LENGTH], char tAvatarName[MAX_AVATAR_NAME_LENGTH], int tZoneNumber );

};
//INSTANCE
extern MyGameLog mGAMELOG;
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------------
