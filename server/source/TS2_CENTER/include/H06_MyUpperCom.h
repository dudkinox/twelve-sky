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

public :

	MyGameLog( void );

	BOOL Init( char tIP[16], int tPort );
	void Free( void );

	void SendToLogServer( void );
	void SendToLogServerForDB( void );

	void GL_201_START_TRIBE_SYMBOL_BATTLE( void );
	void GL_202_END_TRIBE_SYMBOL_BATTLE( void );

};
//INSTANCE
extern MyGameLog mGAMELOG;
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------------
