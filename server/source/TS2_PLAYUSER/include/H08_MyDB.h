//-------------------------------------------------------------------------------------------------
//MY_DB_H
//-------------------------------------------------------------------------------------------------
#ifndef MY_DB_H
#define MY_DB_H
#include "cts_mysql.h"
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_DB
//-------------------------------------------------------------------------------------------------
//MACRO
#define MAX_QUERY_BUFFER_SIZE										10000
//DEFINE
class MyDB
{

private :

	char mQUERY[MAX_QUERY_BUFFER_SIZE];
	char mQUERY_FOR_THREAD[MAX_QUERY_BUFFER_SIZE];

#ifdef __WIN32__
	GS::DB::ADO::CGSDBManager mDB01;
	GS::DB::ADO::CGSDBManager mDB01_FOR_THREAD;
	GS::DB::ADO::CGSDBManager mDB02;
	GS::DB::ADO::CGSDBManager mDB02_FOR_THREAD;
#else
	CMySQL mDB01;
	CMySQL mDB01_FOR_THREAD;
	CMySQL mDB02;
	CMySQL mDB02_FOR_THREAD;
#endif

	BOOL RECONNECT_TO_DATABASE01( void );
	BOOL RECONNECT_TO_DATABASE01_FOR_THREAD( void );
	BOOL RECONNECT_TO_DATABASE02( void );
	BOOL RECONNECT_TO_DATABASE02_FOR_THREAD( void );

	void MakeQueryForSave( int tSort, char tID[MAX_USER_ID_LENGTH], AVATAR_INFO *tAvatarInfo, int tLoginPremiumPCRoom);

public :

	MyDB( void );

	BOOL Init( void );
	void Free( void );

	BOOL DB_PROCESS_01( char tID[MAX_USER_ID_LENGTH], char *tIp );
	BOOL DB_PROCESS_02( char tID[MAX_USER_ID_LENGTH] );
	BOOL DB_PROCESS_03( int tSort, char tID[MAX_USER_ID_LENGTH], AVATAR_INFO *tAvatarInfo, AUTH_INFO *tAuthInfo, int tLoginPremiumPCRoom );
	void DB_PROCESS_04( char tID[MAX_USER_ID_LENGTH], char tHackName[MAX_HACK_NAME_LENGTH], int tHackSize, int *tResult );
	BOOL DB_PROCESS_05( int tUserNum );
	BOOL DB_PROCESS_06( char tID[MAX_USER_ID_LENGTH], int tSort, char tHackName[MAX_HACK_NAME_LENGTH] );
	BOOL DB_PROCESS_07( char tID[MAX_USER_ID_LENGTH], int tSort, char tHackName[MAX_HACK_NAME_LENGTH] );
	BOOL DB_PROCESS_08( char tID[MAX_USER_ID_LENGTH], char tAvatarName[MAX_AVATAR_NAME_LENGTH], int tWorldNumber, int tZoneNumber );
	void DB_PROCESS_09( char tID[MAX_USER_ID_LENGTH], int tSort );
	bool DB_COMPARE_AUTH_KEY (char tID[MAX_USER_ID_LENGTH], char szAuthKey[MAX_MOUSE_PASSWORD_LENGTH], BYTE& bResult);
	bool DB_UPDATE_AUTH_KEY (char tID[MAX_USER_ID_LENGTH], char szAuthKey[3][MAX_MOUSE_PASSWORD_LENGTH], BYTE& bResult);
	BOOL DB_PROCESS_10( TRIBE_INFO *tTribeInfo );
	void DB_PROCESS_11( char tID[MAX_USER_ID_LENGTH], char tAvatarName[MAX_AVATAR_NAME_LENGTH], char tChangeAvatarName[MAX_AVATAR_NAME_LENGTH], int *tResult );
	void DB_PROCESS_12(char *tID, char *tAvatarName, int *tResult);
	void DB_PROCESS_13(char *tID, int *tResult);
	void DB_PROCESS_14(char *tID, int *tResult);
#ifdef __EVENT_KILL__
	void DB_PROCESS_15(char *tID, char *tName, int tTribe, int tKillNum);
#elif __EVENT_GOLD_PHONE__
	void DB_PROCESS_15(char *tID, char *tName);
#endif
	BOOL DB_PROCESS_16( char tID[MAX_USER_ID_LENGTH], int tCashValue);
	

	void WRITE_FOR_DB_LOG_FOR_QUERY( void );
	void WRITE_FOR_DB_LOG_FOR_QUERY_FOR_THREAD( void );

};
//INSTANCE
extern MyDB mDB;
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------------
