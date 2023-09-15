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
	srand( (unsigned int) time( NULL ) );

	if( !mDB.Init() )
	{
		*tResult = 1;
		return FALSE;
	}
		
	return TRUE;
}
//FREE
void MyServer::Free( void )
{
	closesocket( mSocket );
	/*
	WSACleanup();
	mDB.Free();
	*/
}
//PROCESS_FOR_NETWORK
void MyServer::PROCESS_FOR_NETWORK( SOCKET *cli_fd, UINT fd_status, void *arg = NULL )
{
	//int tAddressLength;
	socklen_t tAddressLength;
	//SOCKADDR_IN	tAddress;
	struct sockaddr_in tAddress;
	char tRecvData[10000];
	int tRecvSizeFromUser;
	int tTotalPacketSize;
	int tTotalQuerySize;
    
		
	switch( fd_status )
	{
	case FD_READ :
		tAddressLength = sizeof( tAddress );
		tRecvSizeFromUser = recvfrom( *cli_fd, &tRecvData[0], 10000, 0, (struct sockaddr*) &tAddress, &tAddressLength );

		if( tRecvSizeFromUser < 4 )
		{
			return;
		}
		CopyMemory( &tTotalPacketSize, &tRecvData[0], 4 );
		if( tTotalPacketSize != -1 )
		{
			if( tRecvSizeFromUser != tTotalPacketSize )
			{
				return;
			}
			tRecvData[9999] = '\0';
			LOG_TO_FILE_1( "!%s\n", &tRecvData[4] );


			if(strstr(&tRecvData[4], "<GM")) {
				gm_log_obj_ptr->log_to_file( "!%s\n", &tRecvData[4] );
			}
			else if (strstr(&tRecvData[4], "<CL")){
				netc_log_obj_ptr->log_to_file( "!%s\n", &tRecvData[4] );
			}
			else {
				net_log_obj_ptr->log_to_file( "!%s\n", &tRecvData[4] );
			}
		}
		else
		{
			if( tRecvSizeFromUser < 8 )
			{
				return;
			}
			CopyMemory( &tTotalQuerySize, &tRecvData[4], 4 );
			if( tRecvSizeFromUser != ( tTotalQuerySize + 8 ) )
			{
				return;
			}
			tRecvData[9999] = '\0';

			LOG_TO_FILE_1( "!sql %s\n", &tRecvData[8] );
			mDB.DB_PROCESS_01( &tRecvData[8] );
		}
		return;
	}
}

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_SERVER
//-------------------------------------------------------------------------------------------------
//INSTANCE
MyDB mDB;
//CREATE
MyDB::MyDB( void )
{
}
//RECONNECT_TO_DATABASE01
BOOL MyDB::RECONNECT_TO_DATABASE01( void )
{
	if( mDB01.IsConnected() ) {
		return TRUE;
	}

	if( !mDB01.Reconnect() ) {
		LOG_TO_FILE_1("![%s] : reconnect failed\n", __FUNCTION__);
		return FALSE;
	}

#ifdef __TK__
	if(mDB01.exec_query("SET character SET latin5;", CMySQL::CLEAR) != 0)  {
		LOG_TO_FILE_2("[%s] DB character false(%s)! \n" , __FUNCTION__, mDB01.get_error_msg().c_str());
		return FALSE;
	}
#endif
	LOG_TO_FILE_1("![%s] : reconnected\n", __FUNCTION__);

	return TRUE;
}
//INIT
BOOL MyDB::Init( void )
{
	if( mSERVER_INFO.mCheckTestServer == 1 ) {
		return TRUE;
	}

	if(!mDB01.connect(mSERVER_INFO.mDB_1_IP, mSERVER_INFO.mDB_1_PORT
				, mSERVER_INFO.mDB_1_Name, mSERVER_INFO.mDB_1_ID, mSERVER_INFO.mDB_1_PASSWORD)) {
		LOG_TO_FILE_2("[%s] DB connect false(%s)! \n" , __FUNCTION__, mDB01.get_error_msg().c_str());
		return FALSE;
	}
#ifdef __TK__
	if(mDB01.exec_query("SET character SET latin5;", CMySQL::CLEAR) != 0)  {
		LOG_TO_FILE_2("[%s] DB character false(%s)! \n" , __FUNCTION__, mDB01.get_error_msg().c_str());
		return FALSE;
	}
#endif

	return TRUE;
}
//FREE
void MyDB::Free( void )
{
	if( mSERVER_INFO.mCheckTestServer == 1 ) {
		return;
	}
}
//DB_PROCESS_01
BOOL MyDB::DB_PROCESS_01( char *tLogData )
{
	char *sc_ptr = NULL;
	char szTableName[64];
	char szNewQuery[1024];
	::memset(szTableName, 0, 64);
	::memset(szNewQuery, 0, 1024);
	LOG_TO_FILE("DB YAZ \n");
	if( mSERVER_INFO.mCheckTestServer == 1 ) {
		return TRUE;
	}

	sc_ptr = ::strstr(tLogData, "t_log_");
	::snprintf(szTableName, 23, "%s", sc_ptr);

	if(!CHECK_TABLE(szTableName)) {
		LOG_TO_FILE_3("![%s] Table check fail(%s, %s)\n"
				, __FUNCTION__, szTableName, tLogData);

		return FALSE;
	}

	if( !RECONNECT_TO_DATABASE01() ) {
		LOG_TO_FILE_3("![%s] DB reconnect fail(%s, %s)\n"
				, __FUNCTION__, szTableName, tLogData);
		return FALSE;
	}

	sc_ptr = NULL;
	sc_ptr = ::strchr(tLogData, '(') + 1;

	if(::strstr(szTableName, "itemlog")) {
		snprintf(szNewQuery, 1024, 
				"INSERT INTO %s (islandNumber, countryCode, indate_kor, UserNo "
				"	, GameID, Item_Code, LogTime, PostMoney01 "
				"	, PostMoney02, BuyMoney01, BuyMoney02, ZoneNumber "
				"	, WorldNumber, Class, GameLevel) "
				"	VALUES (%d, '%s', ADDTIME(UTC_TIMESTAMP(), '09:00:00'), %s", 
				szTableName, g_env.island_num, g_env.contry_code_str_obj.c_str(), sc_ptr);
	} else {
	snprintf(szNewQuery, 1024
			, "INSERT INTO %s VALUES (%d, '%s', ADDTIME(UTC_TIMESTAMP(), '09:00:00'), %s"
			, szTableName, g_env.island_num, g_env.contry_code_str_obj.c_str(), sc_ptr);
	}
	
	if(mDB01.exec_query(szNewQuery, CMySQL::CLEAR) != 0) {
		LOG_TO_FILE_3("![%s] DB excute fail(%s, %s)\n"
				, __FUNCTION__, szTableName, szNewQuery);
		return FALSE;
	}
	return TRUE;
}

BOOL MyDB::CHECK_TABLE(const char *table_name)
{
	MYSQL_ROW row;
	int row_num = 0;
	MYSQL_RES *res = NULL;
	char szQuery[MAX_SIZE_OF_QUERY];
	::memset(szQuery, 0, MAX_SIZE_OF_QUERY);

	if(mSERVER_INFO.mCheckTestServer == 1) {
		return true;
	}

	::snprintf(szQuery, MAX_SIZE_OF_QUERY, 
			"SELECT COUNT(TABLE_NAME) as cnt "
			"FROM information_schema.TABLES "
			"WHERE TABLE_NAME = '%s' AND TABLE_SCHEMA = '%s'", table_name, mSERVER_INFO.mDB_1_Name);
	
	if(!RECONNECT_TO_DATABASE01()) {
		return false;
	}

	if(mDB01.exec_query(szQuery) != 0) {
		LOG_TO_FILE_2("![%s] Query excute fail(%s)\n", __FUNCTION__, szQuery);
		return false;
	}

	res = mDB01.get_res();
	if(res == NULL) {
		return false;
	}
	while(row = mysql_fetch_row(res)) {
		row_num = ::atoi(row[0]);
	}
	mDB01.clear_res();

	if(row_num == 0) {
		::memset(szQuery, 0, MAX_SIZE_OF_QUERY);
		if(::strstr(table_name, "t_log_gamelog")) {
#ifdef __TK__
			::snprintf(szQuery, MAX_SIZE_OF_QUERY, 
					"Create Table %s( "
					"	islandNumber int(11) NOT NULL, "
					"	countryCode varchar(2)  NOT NULL, "
					"	indate_kor datetime NOT NULL, "
					"	UserNo varchar(%d) collate latin1_bin NOT NULL,"
					"	GameID varchar(%d) CHARACTER SET latin5 COLLATE latin5_turkish_ci NOT NULL, "
					"	ZoneNumber int NOT NULL, "
					"	WorldNumber int NOT NULL, "
					"	UserIp varchar(15) NOT NULL, "
					"	LogType int Not NULL DEFAULT '0', "
					"	LoginTime datetime NOt NULL, "
					"	GameTime int NOT NULL DEFAULt '0', "
					"	LV int NOt NULL DEFAULT '0', "
					"	EXP int NOt NULL DEFAULT '0', "
					"	WIN int NOt NULL DEFAULT '0', "
					"	LOSE int NOt NULL DEFAULT '0', "
					"	SCORE int NOt NULL DEFAULT '0', "
					"	POINT int NOt NULL DEFAULT '0', "
					"	PCBANG int NOt NULL DEFAULT '0', "
					"	KEY LoginTime (LoginTime) "
					") Engine=MyISAM DEFAULT Charset=latin1 COLLATE=latin1_bin; "
					, table_name, MAX_USER_ID_LENGTH, MAX_AVATAR_NAME_LENGTH);
#else
			::snprintf(szQuery, MAX_SIZE_OF_QUERY, 
					"Create Table %s( "
					"	islandNumber int(11) NOT NULL, "
					"	countryCode varchar(2)  NOT NULL, "
					"	indate_kor datetime NOT NULL, "
					"	UserNo varchar(%d) collate latin1_bin NOT NULL, "
					"	GameID varchar(%d) collate latin1_bin NOT NULL, "
					"	ZoneNumber int NOT NULL, "
					"	WorldNumber int NOT NULL, "
					"	UserIp varchar(15) NOT NULL, "
					"	LogType int Not NULL DEFAULT '0', "
					"	LoginTime datetime NOt NULL, "
					"	GameTime int NOT NULL DEFAULt '0', "
					"	LV int NOt NULL DEFAULT '0', "
					"	EXP int NOt NULL DEFAULT '0', "
					"	WIN int NOt NULL DEFAULT '0', "
					"	LOSE int NOt NULL DEFAULT '0', "
					"	SCORE int NOt NULL DEFAULT '0', "
					"	POINT int NOt NULL DEFAULT '0', "
					"	PCBANG int NOt NULL DEFAULT '0', "
					"	KEY LoginTime (LoginTime) "
					") Engine=MyISAM DEFAULT Charset=latin1 COLLATE=latin1_bin; "
					, table_name, MAX_USER_ID_LENGTH, MAX_AVATAR_NAME_LENGTH);
#endif
		} else if(::strstr(table_name, "t_log_itemlog")) {
#ifdef __TK__
			::snprintf(szQuery, MAX_SIZE_OF_QUERY,
					"CREATE Table %s( "
					"	islandNumber int(11) NOT NULL, "
					"	countryCode varchar(2)  NOT NULL, "
					"	indate_kor datetime NOT NULL, "
					"	Idx int NOt NULL auto_increment, "
					"	UserNo varchar(%d) collate latin1_bin NOT NULL,"
					"	GameID varchar(%d) CHARACTER SET latin5 COLLATE latin5_turkish_ci NOT NULL, "
					"	Item_Code int NOt NULL DEFAULT '0', "
					"	LogTime datetime NOt NULL, "
					"	PostMoney01 int NOt NULL DEFAULT '0', "
					"	PostMoney02 int NOt NULL DEFAULT '0', "
					"	BuyMoney01 int NOt NULL DEFAULT '0', "
					"	BuyMoney02 int NOt NULL DEFAULT '0', "
					"	ZoneNumber int NOt NULL DEFAULT '0', "
					"	WorldNumber int NOt NULL DEFAULT '0', "
					"	Class int NOt NULL DEFAULT '0', "
					"	GameLevel int NOt NULL DEFAULT '0', "
					"	PRIMARY KEY (Idx), "
					"	UNIQUE KEY Idx (Idx), "
					"	KEY LogTime (LogTime) "
					") Engine=MyISAM DEFAULT Charset=latin1 COLLATE=latin1_bin; "
					, table_name, MAX_USER_ID_LENGTH, MAX_AVATAR_NAME_LENGTH);
#else
			::snprintf(szQuery, MAX_SIZE_OF_QUERY,
					"CREATE Table %s( "
					"	islandNumber int(11) NOT NULL, "
					"	countryCode varchar(2)  NOT NULL, "
					"	indate_kor datetime NOT NULL, "
					"	Idx int NOt NULL auto_increment, "
					"	UserNo varchar(%d) collate latin1_bin NOT NULL, "
					"	GameID varchar(%d) CHARACTER SET latin5 COLLATE latin5_turkish_ci NOT NULL,"
					"	Item_Code int NOt NULL DEFAULT '0', "
					"	LogTime datetime NOt NULL, "
					"	PostMoney01 int NOt NULL DEFAULT '0', "
					"	PostMoney02 int NOt NULL DEFAULT '0', "
					"	BuyMoney01 int NOt NULL DEFAULT '0', "
					"	BuyMoney02 int NOt NULL DEFAULT '0', "
					"	ZoneNumber int NOt NULL DEFAULT '0', "
					"	WorldNumber int NOt NULL DEFAULT '0', "
					"	Class int NOt NULL DEFAULT '0', "
					"	GameLevel int NOt NULL DEFAULT '0', "
					"	PRIMARY KEY (Idx), "
					"	UNIQUE KEY Idx (Idx), "
					"	KEY LogTime (LogTime) "
					") Engine=MyISAM DEFAULT Charset=latin1 COLLATE=latin1_bin; "
					, table_name, MAX_USER_ID_LENGTH, MAX_AVATAR_NAME_LENGTH);
#endif
		} else {
			return false;
		}
	
		//LOG_TO_FILE_1("szQuery : %d\n", szQuery);

		if(!RECONNECT_TO_DATABASE01()) {
			return false;
		}
	
		if(mDB01.exec_query(szQuery, CMySQL::CLEAR) != 0) {
			LOG_TO_FILE_2("![%s] Query excute fail(%s)\n", __FUNCTION__, szQuery);
			return false;
		}
	}

	return true;
}
//-------------------------------------------------------------------------------------------------
