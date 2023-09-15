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
//
#include "cts_mysql.h"
#include "win2unix.h"

#define MAX_SIZE_OF_QUERY  8192
class MyServer
{

private :

	//WSADATA mWsaData;

	SOCKET mSocket;
	//SOCKADDR_IN mAddress;
	struct sockaddr_in mAddress;
	CLogging *m_log_obj_ptr;

public :

	MyServer( void );

	BOOL Init( int *tResult );
	void Free( void );

	//void PROCESS_FOR_NETWORK( HWND hWnd, WPARAM wPrm, LPARAM lPrm );
	void PROCESS_FOR_NETWORK( SOCKET *cli_fd, UINT fd_status, void *arg );
	void WRITE_FOR_LOG( char *tLogData );

};
//INSTANCE
extern MyServer mSERVER;
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_DB
//-------------------------------------------------------------------------------------------------
//DEFINE
/* 2008. 11. 18 (Will This Work after CRM DB Work)  */
class MyDB
{

private :

	//GS::DB::ADO::CGSDBManager mDB01;
	CMySQL mDB01;
	BOOL RECONNECT_TO_DATABASE01( void );

public :

	MyDB( void );

	BOOL Init( void );
	void Free( void );

	BOOL DB_PROCESS_01( char *tLogData );
	BOOL CHECK_TABLE(const char *table_name);
};
//INSTANCE
extern MyDB mDB;
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------------
