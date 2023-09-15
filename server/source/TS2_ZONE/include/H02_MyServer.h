//-------------------------------------------------------------------------------------------------
//MY_SERVER_H
//-------------------------------------------------------------------------------------------------
#ifndef MY_SERVER_H
#define MY_SERVER_H
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_SERVER
//-------------------------------------------------------------------------------------------------
//DEFINE
#include "win2unix.h"

class MyServer
{

private :
	SOCKET mSocket;
	//SOCKADDR_IN mAddress;
	struct sockaddr_in mAddress;

public :

	bool mRankCheck;
	int mMAX_USER_NUM;
	int mMAX_SEND_BUFFER_SIZE;
	int mMAX_RECV_BUFFER_SIZE;

	MyServer( void );

	BOOL Init( int *tResult );
	void Free( void );

	//void PROCESS_FOR_NETWORK( HWND hWnd, WPARAM wPrm, LPARAM lPrm );
	int PROCESS_FOR_NETWORK(SOCKET *cli_fd, UINT fd_status, void *arg);

	void WRITE_FOR_LOG( char *tLogData );

	void SetSocket(SOCKET listen_socket) { mSocket = listen_socket; }
};
//INSTANCE
extern MyServer mSERVER;
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------------
