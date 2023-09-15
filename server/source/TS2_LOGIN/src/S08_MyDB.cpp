//-------------------------------------------------------------------------------------------------
//HEADER
//-------------------------------------------------------------------------------------------------
#include "H01_MainApplication.h"
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_DB
//-------------------------------------------------------------------------------------------------
//INSTANCE
MyDB mDB;
//CREATE
MyDB::MyDB( void )
{
}
//INIT
BOOL MyDB::Init( void )
{
	if(mDB01.set_db_conn_info(mSERVER_INFO.mDB_1_IP[0], mSERVER_INFO.mDB_1_IP[1], mSERVER_INFO.mDB_1_PORT
				, g_env.db_name_str_obj.c_str (), mSERVER_INFO.mDB_1_ID, mSERVER_INFO.mDB_1_PASSWORD) < 0) {
		LOG_TO_FILE_2 ("![%s] DB Set connect info fail(%d).\n", __FUNCTION__, -1);
		return FALSE;
	}
	if (!mDB01.connect()) {
        LOG_TO_FILE_2 ("![%s] DB connect false(%s).\n", __FUNCTION__, 
                       mDB01.get_error_msg ().c_str ());
		return FALSE;
	}
#ifdef __COMMIT__
    if (mDB01.exec_query ("SET AUTOCOMMIT=1;", CMySQL::CLEAR) != 0) {
        LOG_TO_FILE_2 ("![%s] DB character false(%s).\n", __FUNCTION__, 
                       mDB01.get_error_msg ().c_str ());
        return FALSE;
    }
#endif
#ifdef __TK__
	if (mDB01.exec_query ("SET character SET latin5;", CMySQL::CLEAR) != 0) {
        LOG_TO_FILE_2 ("![%s] DB character false(%s).\n", __FUNCTION__, 
                       mDB01.get_error_msg ().c_str ());
        return FALSE;
    }
#endif

	//LOG_TO_FILE_4("%s, %d, %s, %s", mSERVER_INFO.mDB_2_IP[0], mSERVER_INFO.mDB_2_PORT, g_env.db2_name_str_obj.c_str (),mSERVER_INFO.mDB_2_ID);
	//LOG_TO_FILE_2("%s, %s\n", mSERVER_INFO.mDB_2_IP[1], mSERVER_INFO.mDB_2_PASSWORD);

	if(mDB02.set_db_conn_info(mSERVER_INFO.mDB_2_IP[0], mSERVER_INFO.mDB_2_IP[1], mSERVER_INFO.mDB_2_PORT
			, g_env.db2_name_str_obj.c_str (), mSERVER_INFO.mDB_2_ID, mSERVER_INFO.mDB_2_PASSWORD) < 0) {
		LOG_TO_FILE_2 ("![%s] DB Set connect info fail(%d).\n", __FUNCTION__, -1);
        return FALSE;
	}
	if (!mDB02.connect()) {
		return FALSE;
	}

#ifdef __COMMIT__
    if (mDB02.exec_query ("SET AUTOCOMMIT=1;", CMySQL::CLEAR) != 0) {
        LOG_TO_FILE_2 ("![%s] DB character false(%s).\n", __FUNCTION__,
                       mDB02.get_error_msg ().c_str ());
        return FALSE;
    }
#endif
#ifdef __TK__
	if (mDB02.exec_query ("SET character SET latin5;", CMySQL::CLEAR) != 0) {
        LOG_TO_FILE_2 ("![%s] DB character false(%s).\n", __FUNCTION__,
                       mDB02.get_error_msg ().c_str ());
        return FALSE;
    }
#endif
	return TRUE;
}
//FREE
void MyDB::Free( void )
{
}
//RECONNECT_TO_DATABASE01
BOOL MyDB::RECONNECT_TO_DATABASE01( void )
{
	if( mDB01.IsConnected() )
	{
		return TRUE;
	}
	if( !mDB01.Reconnect() )
	{
		return FALSE;
	}
#ifdef __COMMIT__
    if (mDB01.exec_query ("SET AUTOCOMMIT=1;", CMySQL::CLEAR) != 0) {
        LOG_TO_FILE_2 ("![%s] DB character false(%s).\n", __FUNCTION__,
                       mDB01.get_error_msg ().c_str ());
        return FALSE;
    }
#endif
#ifdef __TK__
	if (mDB01.exec_query ("SET character SET latin5;", CMySQL::CLEAR) != 0) {
        LOG_TO_FILE_2 ("![%s] DB character false(%s).\n", __FUNCTION__,
                       mDB01.get_error_msg ().c_str ());
        return FALSE;
    }
#endif
    LOG_TO_FILE ("!Established reconnection with database server#01.\n");
	return TRUE;
}
//RECONNECT_TO_DATABASE02
BOOL MyDB::RECONNECT_TO_DATABASE02( void )
{
	if( mDB02.IsConnected() )
	{
		return TRUE;
	}
	if( !mDB02.Reconnect() )
	{
        LOG_TO_FILE ("!Reconnect to the database failed.\n");
		return FALSE;
	}
#ifdef __COMMIT__
    if (mDB02.exec_query ("SET AUTOCOMMIT=1;", CMySQL::CLEAR) != 0) {
        LOG_TO_FILE_2 ("![%s] DB character false(%s).\n", __FUNCTION__,
                       mDB02.get_error_msg ().c_str ());
        return FALSE;
    }
#endif
#ifdef __TK__
	if (mDB02.exec_query ("SET character SET latin5;", CMySQL::CLEAR) != 0) {
        LOG_TO_FILE_2 ("![%s] DB character false(%s).\n", __FUNCTION__,
                       mDB02.get_error_msg ().c_str ());
        return FALSE;
    }
#endif
    LOG_TO_FILE ("!Established reconnection with database server#02.\n");
	return TRUE;
}
//MAKE_QUERY_FOR_INSERT
void MyDB::MakeQueryForInsert( int tSort, char tID[MAX_USER_ID_LENGTH], AVATAR_INFO *tAvatarInfo )
{
	int index01;
	int index02;
	char tempString01[1000];
	char tTableName[100];
	char aEquip[326];
	char aInventory[3457];
    char aTrade[201];
	char aStoreItem[1401];
	char aSkill[181];
	char aHotKey[421];
	char aQuestInfo[26];
	char aFriend[121];

	switch( tSort )
	{
	case 1 :
		strcpy( tTableName, mSERVER_INFO.mDB_2_Table03 );
		break;
	case 2 :
		strcpy( tTableName, mSERVER_INFO.mDB_2_Table04 );
		break;
	default :
		strcpy( mQUERY, "" );
		return;
	}
	aEquip[0] = '\0';
	for( index01 = 0 ; index01 < MAX_EQUIP_SLOT_NUM ; index01++ )
	{
		sprintf( tempString01, "%05d%03d%09d%08d", ( tAvatarInfo->aEquip[index01][0] % 100000 ), ( tAvatarInfo->aEquip[index01][1] % 1000 ), ( tAvatarInfo->aEquip[index01][2] % 1000000000 ), ( tAvatarInfo->aEquip[index01][3] % 100000000 ) );
		strcat( aEquip, tempString01 );
	}
	aInventory[0] = '\0';
	for( index01 = 0 ; index01 < 2 ; index01++ )
	{
		for( index02 = 0 ; index02 < MAX_INVENTORY_SLOT_NUM ; index02++ )
		{
			sprintf( tempString01, "%05d%01d%01d%03d%09d%08d", ( tAvatarInfo->aInventory[index01][index02][0] % 100000 ), ( tAvatarInfo->aInventory[index01][index02][1] % 10 ), ( tAvatarInfo->aInventory[index01][index02][2] % 10 ), ( tAvatarInfo->aInventory[index01][index02][3] % 1000 ), ( tAvatarInfo->aInventory[index01][index02][4] % 1000000000 ), ( tAvatarInfo->aInventory[index01][index02][5] % 100000000 ) );
			strcat( aInventory, tempString01 );
		}
	}
	aTrade[0] = '\0';
	for( index01 = 0 ; index01 < MAX_TRADE_SLOT_NUM ; index01++ )
	{
		sprintf( tempString01, "%05d%03d%09d%08d", ( tAvatarInfo->aTrade[index01][0] % 100000 ), ( tAvatarInfo->aTrade[index01][1] % 1000 ), ( tAvatarInfo->aTrade[index01][2] % 1000000000 ), ( tAvatarInfo->aTrade[index01][3] % 100000000 ) );
		strcat( aTrade, tempString01 );
	}
	aStoreItem[0] = '\0';
	for( index01 = 0 ; index01 < 2 ; index01++ )
	{
		for( index02 = 0 ; index02 < MAX_STORE_ITEM_SLOT_NUM ; index02++ )
		{
			sprintf( tempString01, "%05d%03d%09d%08d", ( tAvatarInfo->aStoreItem[index01][index02][0] % 100000 ), ( tAvatarInfo->aStoreItem[index01][index02][1] % 1000 ), ( tAvatarInfo->aStoreItem[index01][index02][2] % 1000000000 ), ( tAvatarInfo->aStoreItem[index01][index02][3] % 100000000 ) );
			strcat( aStoreItem, tempString01 );
		}
	}
	aSkill[0] = '\0';
	for( index01 = 0 ; index01 < MAX_SKILL_SLOT_NUM ; index01++ )
	{
		sprintf( tempString01, "%03d%03d", ( tAvatarInfo->aSkill[index01][0] % 1000 ), ( tAvatarInfo->aSkill[index01][1] % 1000 ) );
		strcat( aSkill, tempString01 );
	}
	aHotKey[0] = '\0';
	for( index01 = 0 ; index01 < 3 ; index01++ )
	{
		for( index02 = 0 ; index02 < MAX_HOT_KEY_NUM ; index02++ )
		{
			sprintf( tempString01, "%05d%03d%02d", ( tAvatarInfo->aHotKey[index01][index02][0] % 100000 ), ( tAvatarInfo->aHotKey[index01][index02][1] % 1000 ), ( tAvatarInfo->aHotKey[index01][index02][2] % 100 ) );
			strcat( aHotKey, tempString01 );
		}
	}
	aQuestInfo[0] = '\0';
	sprintf( tempString01, "%05d%05d%05d%05d%05d", ( tAvatarInfo->aQuestInfo[0] % 100000 ), ( tAvatarInfo->aQuestInfo[1] % 100000 ), ( tAvatarInfo->aQuestInfo[2] % 100000 ), ( tAvatarInfo->aQuestInfo[3] % 100000 ), ( tAvatarInfo->aQuestInfo[4] % 100000 ) );
	strcat( aQuestInfo, tempString01 );
    ::memset (aFriend, '@', MAX_FRIEND_NUM * (MAX_AVATAR_NAME_LENGTH - 1));
	for( index01 = 0 ; index01 < MAX_FRIEND_NUM ; index01++ )
	{
		CopyMemory( &aFriend[( index01 * 12 + 0 )], &tAvatarInfo->aFriend[index01][0], strlen( tAvatarInfo->aFriend[index01] ) );
	}
	aFriend[120] = '\0';

#ifdef __TK__
	sprintf( mQUERY, "insert into %s values ( now(),'%s',%d,%d,%d,%d,%d,%d,%d,'%s',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d "
	"	,%d,%d,%d,%d,%d,'%s',%d,%d,'%s',%d,'%s',%d,%d,'%s','%s','%s','%s','%s','%s','%s',%d,'%s',%d,'%s',%d,%d,%d,%d,%d,%d "
	"	,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d "
	"	,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d "
#ifdef __GOD__
	"	,%d, %d, %d, %d "
	"	,%d, %d, %d "
	"	,%d, %d, %d "
#endif
	")",
		tTableName,
		tID,
		tAvatarInfo->aVisibleState,
		tAvatarInfo->aSpecialState,
		tAvatarInfo->aPlayTime1,
		tAvatarInfo->aPlayTime2,
		tAvatarInfo->aPlayTime3,
		tAvatarInfo->aKillOtherTribe,
		mSERVER_INFO.mWorldNumber,
		tAvatarInfo->aName,
		tAvatarInfo->aTribe,
		tAvatarInfo->aPreviousTribe,
		tAvatarInfo->aGender,
		tAvatarInfo->aHeadType,
		tAvatarInfo->aFaceType,
		tAvatarInfo->aLevel1,
		tAvatarInfo->aLevel2,
		tAvatarInfo->aGeneralExperience1,
		tAvatarInfo->aGeneralExperience2,
		tAvatarInfo->aVitality,
		tAvatarInfo->aStrength,
		tAvatarInfo->aKi,
		tAvatarInfo->aWisdom,
		tAvatarInfo->aEatLifePotion,
		tAvatarInfo->aEatManaPotion,
		tAvatarInfo->aStateBonusPoint,
		tAvatarInfo->aSkillPoint,
		aEquip,
		tAvatarInfo->aExpandInventoryDate,
		tAvatarInfo->aMoney,
		aInventory,
		tAvatarInfo->aTradeMoney,
		aTrade,
		tAvatarInfo->aExpandStoreDate,
		tAvatarInfo->aStoreMoney,
		aStoreItem,
		aSkill,
		aHotKey,
		aQuestInfo,
		aFriend,
		tAvatarInfo->aTeacher,
		tAvatarInfo->aStudent,
		tAvatarInfo->aTeacherPoint,
		tAvatarInfo->aGuildName,
		tAvatarInfo->aGuildRole,
		tAvatarInfo->aCallName,
		tAvatarInfo->aGuildMarkNum,
		tAvatarInfo->aGuildMarkEffect,
		tAvatarInfo->aLogoutInfo[0],
		tAvatarInfo->aLogoutInfo[1],
		tAvatarInfo->aLogoutInfo[2],
		tAvatarInfo->aLogoutInfo[3],
		tAvatarInfo->aLogoutInfo[4],
		tAvatarInfo->aLogoutInfo[5],
		tAvatarInfo->aProtectForDeath,
		tAvatarInfo->aProtectForDestroy,
		tAvatarInfo->aDoubleExpTime1,
		tAvatarInfo->aDoubleExpTime2,
		tAvatarInfo->aDoubleKillNumTime,
		tAvatarInfo->aDoubleKillExpTime,
		tAvatarInfo->aNineTowerKeyNum,
		tAvatarInfo->aZone101TypeTime,
		tAvatarInfo->aZone125TypeTime,
		tAvatarInfo->aZone126TypeTime,
		tAvatarInfo->aKillMonsterNum,
		tAvatarInfo->aLevelZoneKeyNum,
		tAvatarInfo->aSearchAndBuyDate,
		tAvatarInfo->aLifePotionConvertNum,
		tAvatarInfo->aManaPotionConvertNum,
		tAvatarInfo->aTribeVoteDate,
		tAvatarInfo->aUse_AutoPotion_Date,
		tAvatarInfo->aUse_AutoPotion_Time,
		tAvatarInfo->aAutoLifeRatio,
		tAvatarInfo->aAutoManaRatio,
		tAvatarInfo->aAnimal[0],
		tAvatarInfo->aAnimal[1],
		tAvatarInfo->aAnimal[2],
		tAvatarInfo->aAnimal[3],
		tAvatarInfo->aAnimal[4],
		tAvatarInfo->aAnimal[5],
		tAvatarInfo->aAnimal[6],
		tAvatarInfo->aAnimal[7],
		tAvatarInfo->aAnimal[8],
		tAvatarInfo->aAnimal[9],
		tAvatarInfo->aAnimalIndex,
		tAvatarInfo->aAnimalTime,
		tAvatarInfo->aDoubleExpTime3,
		tAvatarInfo->aDoubleDropTime
		//@{ auto quiz
		,
		tAvatarInfo->iCriminal_Record,
		tAvatarInfo->iEnter_The_Prison,
		tAvatarInfo->iKill_Monster_Count,
		//@}
		tAvatarInfo->aEatStrengthPotion,
		tAvatarInfo->aEatWisdomPotion,
		tAvatarInfo->aPat_Exp_x2_Time,
		tAvatarInfo->aPat_User_Exp_x2_Time,
		tAvatarInfo->aFiveExpTime,
		tAvatarInfo->aZone84Time,
		/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */
		tAvatarInfo->iSilverOrnamentRemainsTime,
		tAvatarInfo->iGoldOrnamentRemainsTime,
		tAvatarInfo->iUseOrnament,
		/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */
		/** 2009. 11. 23 : 패키지 아이템 개발 (wolf)*/
		tAvatarInfo->aPackSilverDate,
		tAvatarInfo->aPackGoldDate,
		tAvatarInfo->aPackDiamondDate,
		tAvatarInfo->aPackSilverDateMobile,
		tAvatarInfo->aPackGoldDateMobile,
		tAvatarInfo->aPackDiamondDateMobile,
		/** 2009. 11. 23 : 패키지 아이템 개발 (wolf)*/
		tAvatarInfo->aLevelZoneKeyNum2, 
		tAvatarInfo->aDoubleKillNumTime2,
    tAvatarInfo->aKillMonsterNum2
#ifdef __GOD__
    , tAvatarInfo->aAddItemValue, tAvatarInfo->aHighItemValue, tAvatarInfo->aDropItemTime, tAvatarInfo->aTribeCallValue
		, tAvatarInfo->aUpgradeValue, tAvatarInfo->aBonusItemValue, tAvatarInfo->aKillOtherTribeEvent
		, tAvatarInfo->aTeacherPointEvent, tAvatarInfo->aPlayTimeEvent, tAvatarInfo->aProtectForUpgrade
#endif
		);
#else
	sprintf( mQUERY, "insert into %s values ( now(),'%s',%d,%d,%d,%d,%d,%d,%d,'%s',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d "
	",%d,%d,%d,%d,%d,'%s',%d,%d,'%s',%d,'%s',%d,%d,'%s','%s','%s','%s','%s','%s','%s',%d,'%s',%d,'%s',%d,%d,%d,%d,%d,%d "
	",%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d "
	",%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d "
#ifdef __GOD__
	"	,%d, %d, %d, %d "
	"	,%d, %d,  %d "
	"	,%d, %d, %d "
#endif
	")",
		tTableName,
		tID,
		tAvatarInfo->aVisibleState,
		tAvatarInfo->aSpecialState,
		tAvatarInfo->aPlayTime1,
		tAvatarInfo->aPlayTime2,
		tAvatarInfo->aPlayTime3,
		tAvatarInfo->aKillOtherTribe,
		mSERVER_INFO.mWorldNumber,
		tAvatarInfo->aName,
		tAvatarInfo->aTribe,
		tAvatarInfo->aPreviousTribe,
		tAvatarInfo->aGender,
		tAvatarInfo->aHeadType,
		tAvatarInfo->aFaceType,
		tAvatarInfo->aLevel1,
		tAvatarInfo->aLevel2,
		tAvatarInfo->aGeneralExperience1,
		tAvatarInfo->aGeneralExperience2,
		tAvatarInfo->aVitality,
		tAvatarInfo->aStrength,
		tAvatarInfo->aKi,
		tAvatarInfo->aWisdom,
		tAvatarInfo->aEatLifePotion,
		tAvatarInfo->aEatManaPotion,
		tAvatarInfo->aStateBonusPoint,
		tAvatarInfo->aSkillPoint,
		aEquip,
		tAvatarInfo->aExpandInventoryDate,
		tAvatarInfo->aMoney,
		aInventory,
		tAvatarInfo->aTradeMoney,
		aTrade,
		tAvatarInfo->aExpandStoreDate,
		tAvatarInfo->aStoreMoney,
		aStoreItem,
		aSkill,
		aHotKey,
		aQuestInfo,
		aFriend,
		tAvatarInfo->aTeacher,
		tAvatarInfo->aStudent,
		tAvatarInfo->aTeacherPoint,
		tAvatarInfo->aGuildName,
		tAvatarInfo->aGuildRole,
		tAvatarInfo->aCallName,
		tAvatarInfo->aGuildMarkNum,
		tAvatarInfo->aGuildMarkEffect,
		tAvatarInfo->aLogoutInfo[0],
		tAvatarInfo->aLogoutInfo[1],
		tAvatarInfo->aLogoutInfo[2],
		tAvatarInfo->aLogoutInfo[3],
		tAvatarInfo->aLogoutInfo[4],
		tAvatarInfo->aLogoutInfo[5],
		tAvatarInfo->aProtectForDeath,
		tAvatarInfo->aProtectForDestroy,
		tAvatarInfo->aDoubleExpTime1,
		tAvatarInfo->aDoubleExpTime2,
		tAvatarInfo->aDoubleKillNumTime,
		tAvatarInfo->aDoubleKillExpTime,
		tAvatarInfo->aNineTowerKeyNum,
		tAvatarInfo->aZone101TypeTime,
		tAvatarInfo->aZone125TypeTime,
		tAvatarInfo->aZone126TypeTime,
		tAvatarInfo->aKillMonsterNum,
		tAvatarInfo->aLevelZoneKeyNum,
		tAvatarInfo->aSearchAndBuyDate,
		tAvatarInfo->aLifePotionConvertNum,
		tAvatarInfo->aManaPotionConvertNum,
		tAvatarInfo->aTribeVoteDate,
		tAvatarInfo->aUse_AutoPotion_Date,
		tAvatarInfo->aUse_AutoPotion_Time,
		tAvatarInfo->aAutoLifeRatio,
		tAvatarInfo->aAutoManaRatio,
		tAvatarInfo->aAnimal[0],
		tAvatarInfo->aAnimal[1],
		tAvatarInfo->aAnimal[2],
		tAvatarInfo->aAnimal[3],
		tAvatarInfo->aAnimal[4],
		tAvatarInfo->aAnimal[5],
		tAvatarInfo->aAnimal[6],
		tAvatarInfo->aAnimal[7],
		tAvatarInfo->aAnimal[8],
		tAvatarInfo->aAnimal[9],
		tAvatarInfo->aAnimalIndex,
		tAvatarInfo->aAnimalTime,
		tAvatarInfo->aDoubleExpTime3,
		tAvatarInfo->aDoubleDropTime
		//@{ auto quiz
		,
		tAvatarInfo->iCriminal_Record,
		tAvatarInfo->iEnter_The_Prison,
		tAvatarInfo->iKill_Monster_Count,
		//@}
		tAvatarInfo->aEatStrengthPotion,
		tAvatarInfo->aEatWisdomPotion,
		tAvatarInfo->aPat_Exp_x2_Time,
		tAvatarInfo->aPat_User_Exp_x2_Time,
		tAvatarInfo->aFiveExpTime,
		tAvatarInfo->aZone84Time,
		/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */
		tAvatarInfo->iSilverOrnamentRemainsTime,
		tAvatarInfo->iGoldOrnamentRemainsTime,
		tAvatarInfo->iUseOrnament,
		/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */
		tAvatarInfo->aLevelZoneKeyNum2, 
		tAvatarInfo->aDoubleKillNumTime2,
    tAvatarInfo->aKillMonsterNum2
#ifdef __GOD__
    , tAvatarInfo->aAddItemValue, tAvatarInfo->aHighItemValue, tAvatarInfo->aDropItemTime, tAvatarInfo->aTribeCallValue
		,  tAvatarInfo->aUpgradeValue, tAvatarInfo->aBonusItemValue, tAvatarInfo->aKillOtherTribeEvent
		, tAvatarInfo->aTeacherPointEvent, tAvatarInfo->aPlayTimeEvent, tAvatarInfo->aProtectForUpgrade
#endif
		);
#endif
	
}
//DB_PROCESS_01
BOOL MyDB::DB_PROCESS_01( void )
{
	int iAffected;
	int iCounts;
	long tMaxPlayerNum;
	long tAddPlayerNum;
	long tGagePlayerNum;
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;

	sprintf( mQUERY, 
			"SELECT mMaxUserNum, mAddUserNum, mGageUserNum "
			"FROM %s "
			"WHERE aWorldNumber = %d", mSERVER_INFO.mDB_2_Table01, mSERVER_INFO.mWorldNumber);
	if( !RECONNECT_TO_DATABASE02() )
	{
		return FALSE;
	}
	iAffected = 0;
	if ((iAffected = mDB02.exec_query (mQUERY)) < 0)
	{
        LOG_TO_FILE_2 ("![CRITICAL - (1)] The query execution failed in (%s - %d).\n", __FILE__, __LINE__);
        LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB02.get_error_msg ().c_str ());
		return FALSE;
	}

	iCounts = 0;
	res = mDB02.get_res ();
	if (res == NULL) {
		LOG_TO_FILE_1 ("!Query failed - (%s).\n", mQUERY);
        LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB02.get_error_msg ().c_str ());
		return FALSE;
	}

	while ((row = mysql_fetch_row (res))){
		tMaxPlayerNum = ::atol (row[ 0 ]);
		tAddPlayerNum = ::atol (row[ 1 ]);
		tGagePlayerNum = ::atol (row[ 2 ]);
		iCounts++;
	}
	mDB02.clear_res ();
	if (iCounts  != 1){
		LOG_TO_FILE_1 ("!Result record set count = %d.\n", iCounts);
		return FALSE;
	}
	
	if( tMaxPlayerNum < 1 )
	{
		mGAME.mMaxPlayerNum = 0;
		mGAME.mAddPlayerNum = 0;
		mGAME.mGagePlayerNum = tGagePlayerNum;
		mGAME.mPresentPlayerNum = -1;
	}
	else
	{
		mGAME.mMaxPlayerNum = tMaxPlayerNum;
		mGAME.mAddPlayerNum = tAddPlayerNum;
		mGAME.mGagePlayerNum = tGagePlayerNum;
		if( mGAME.mPresentPlayerNum == -1 )
		{
			mGAME.mPresentPlayerNum = 0;
		}
	}

	sprintf( mQUERY, 
			"SELECT Island, `Partition`, World "
			"FROM RecommandInfo "
			"ORDER BY Island, `Partition`, World "
			"LIMIT %d ", MAX_SIZE_OF_RCMD);

	if( !RECONNECT_TO_DATABASE02() )
	{
		return FALSE;
	}
	iAffected = 0;
	if ((iAffected = mDB02.exec_query (mQUERY)) < 0)
	{
        LOG_TO_FILE_2 ("![CRITICAL - (1)] The query execution failed in (%s - %d).\n", __FILE__, __LINE__);
        LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB02.get_error_msg ().c_str ());
		return FALSE;
	}

	iCounts = 0;
	res = mDB02.get_res ();
	if (res == NULL) {
		LOG_TO_FILE_1 ("!Query failed - (%s).\n", mQUERY);
        LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB02.get_error_msg ().c_str ());
		return FALSE;
	}

	while ((row = mysql_fetch_row (res))){
		mGAME.mRcmdIld[iCounts] = ::atoi(row[0]);
		mGAME.mRcmdPtt[iCounts] = ::atoi(row[1]);
		mGAME.mRcmdWrd[iCounts] = ::atoi(row[2]);
		//LOG_TO_FILE_3("Recommand world : %d %d %d\n", mGAME.mRcmdIld[iCounts], mGAME.mRcmdPtt[iCounts], mGAME.mRcmdWrd[iCounts]);
		iCounts++;
	}
	mDB02.clear_res ();

	if (iCounts != 4){
		for(iCounts = 0; iCounts < MAX_SIZE_OF_RCMD; iCounts++) {
			if(mGAME.mRcmdIld[iCounts] > 0) {
				continue;
			} else {
				mGAME.mRcmdIld[iCounts] = 0;
				mGAME.mRcmdPtt[iCounts] = 0;
				mGAME.mRcmdWrd[iCounts] = iCounts + 1;
			}
		}
	}

	LOG_TO_FILE ("!MyDB::DB_PROCESS_01 success.\n");
	return TRUE;
}
//DB_PROCESS_02
#ifdef __CHANNEL__
void MyDB::DB_PROCESS_02( char tIP[16], char tID[MAX_USER_ID_LENGTH], char tPassword[MAX_USER_PASSWORD_LENGTH], char uID[MAX_USER_ID_LENGTH], int *tCheckMOTP, char tMousePassword[MAX_MOUSE_PASSWORD_LENGTH], int *tSecretCard, int *tUserSort, int *tGoodFellow, int *tLoginPlace, int *tLoginPremium, int *tTraceState, char *tBonus100Money, int *tSaveMoney, int tSaveItem[MAX_SAVE_ITEM_SLOT_NUM][4], AVATAR_INFO tAvatarInfo[MAX_USER_AVATAR_NUM], AUTH_INFO *tAuthInfo, int *tResult, char *tFirstLogin, int tAdult )
#else
void MyDB::DB_PROCESS_02( char tIP[16], char tID[MAX_USER_ID_LENGTH], char tPassword[MAX_USER_PASSWORD_LENGTH], char uID[MAX_USER_ID_LENGTH], int *tCheckMOTP, char tMousePassword[MAX_MOUSE_PASSWORD_LENGTH], int *tSecretCard, int *tUserSort, int *tGoodFellow, int *tLoginPlace, int *tLoginPremium, int *tTraceState, char *tBonus100Money, int *tPremiumServerExpirationDate, int *tSaveMoney, int tSaveItem[MAX_SAVE_ITEM_SLOT_NUM][4], AVATAR_INFO tAvatarInfo[MAX_USER_AVATAR_NUM], AUTH_INFO *tAuthInfo, int *tResult, char *tFirstLogin ) // 함수 선언 수정 - int *tPremiumServerExpirationDate 추가. // @_Premium_Server_@
#endif
{
	
	//night_walker oto_kayit
	//oto_kayit:
	//night_walker oto_kayit
	
	int index00;
	int index01;
	int index02;
	int index03;
	int aWorldNumber = 0;
	char tempString01[1000];
	int iAffected;
	int iCounts;
	long uLoginState;
	char uIP[16];
	char uPassword[MAX_USER_PASSWORD_LENGTH];
	long uCheckMOTP;
	char uMousePassword[MAX_MOUSE_PASSWORD_LENGTH];
	long uSecretCard;
	long uBlockInfo;
	long uUserSort;
	long uGoodFellow;
	long uLoginPlace;
	long uLoginPremium;
	long uTraceState;
	char uBonus100Money;
	long uPremiumServerExpirationDate; // 프리미엄 서버(월드) 입장권 유효 기한(YYYYMMDD 형태의 정수). // @_Premium_Server_@
	long uIDNum;
	long uSaveMoney;
	char uSaveItem[701];
	char aName[MAX_USER_AVATAR_NUM][MAX_AVATAR_NAME_LENGTH];
	char uIDForCheck[MAX_USER_ID_LENGTH];
	char aEquip[326];
	char aInventory[3457];
	char aTrade[201];
	char aStoreItem[1401];
	char aSkill[181];
	char aHotKey[421];
	char aQuestInfo[26];
	char aFriend[121];
	char gMember[851];
#ifdef __GOD__
	float tMaxGeneralExperienceRange;
	int tPresentGeneralExperienceRatio;
	int tNextGeneralExperienceRatio;	
#endif
	bool RealGuild[MAX_USER_AVATAR_NUM];
	bool CheckGuild[MAX_USER_AVATAR_NUM];
	long AuthType;
	long FindFlag;
	long MoveFlag;
	long MoveZoneFlag;
	long CallFlag;
	long HideFlag;
	long ShowFlag;
	long KickFlag;
	long BlockFlag;
	long MonCallFlag;
	long EquipFlag;
	long UnequipFlag;
	long YchatFlag;
	long NchatFlag;
	long NoticeFlag;
	long ItemFlag;
	long MoneyFlag;
	long ExpFlag;
	long DieFlag;
	long GodFlag;
	long ChatFlag;
	long TradeFlag;
	long ShopFlag;
	long PShopFlag;
	long GItemFlag;
	long DItemFlag;
	long AttackFlag;
	long UZonemoveFlag;
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;

#ifdef __CHANNEL__
	sprintf(mQUERY, "CALL SP_CreateAccount('%s', %d, @ret)", tID, tAdult);
	if( !RECONNECT_TO_DATABASE02() )
	{
		LOG_TO_FILE_1 ("![WARNING - (6)] Unable to connect to the database[%s].\n",
				mSERVER_INFO.mDB_2_Table02);
		*tResult = 6;
		return;
	}
	iAffected = 0;
	if ((mDB02.exec_query (mQUERY, CMySQL::CLEAR)) != 0) {
		LOG_TO_FILE ("![WARNING - (6)] The query execution failed(SP_CreateAccount).\n");
		LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB02.get_error_msg ().c_str ());
		*tResult = 6;
		return ;
	}
#endif

	sprintf( mQUERY, "select uLoginState,uIP,a.uID,uPassword,uCheckMOTP,ifnull(uMousePassword, ''),uSecretCard,uBlockInfo,uUserSort,uGoodFellow,uLoginPlace,uLoginPremium,uTraceState, ifnull(AuthType, 0), ifnull(FindFlag, 0), ifnull(MoveFlag,0),ifnull(MoveZoneFlag,0), ifnull(CallFlag,0), ifnull(HideFlag,0), ifnull(ShowFlag,0),ifnull(KickFlag,0), ifnull(BlockFlag,0), ifnull(MonCallFlag,0), ifnull(EquipFlag,0),ifnull(UnequipFlag,0), ifnull(YchatFlag,0), ifnull(NchatFlag,0), ifnull(NoticeFlag,0),ifnull(ItemFlag,0), ifnull(MoneyFlag,0), ifnull(ExpFlag,0), ifnull(DieFlag,0),ifnull(GodFlag,0), ifnull(ChatFlag,0), ifnull(TradeFlag,0), ifnull(ShopFlag,0), ifnull(PShopFlag,0), ifnull(GItemFlag,0), ifnull(DItemFlag,0), ifnull(AttackFlag,0), ifnull(UZonemoveFlag,0), uBonus100Money, uSecretCard, uPremiumServerExpirationDate from %s a left join %s b on a.uID = b.uID where a.uID='%s';", mSERVER_INFO.mDB_1_Table01, mSERVER_INFO.mDB_1_Table08, tID ); // uPremiumServerExpirationDate 추가. // @_Premium_Server_@
	//sprintf( mQUERY, "select uLoginState,uIP,a.uID,uPassword,uCheckMOTP,ifnull(uMousePassword, ''),uSecretCard,uBlockInfo,uUserSort,uGoodFellow,uLoginPlace,uLoginPremium,uTraceState, ifnull(AuthType, 0), ifnull(FindFlag, 0), ifnull(MoveFlag,0),ifnull(MoveZoneFlag,0), ifnull(CallFlag,0), ifnull(HideFlag,0), ifnull(ShowFlag,0),ifnull(KickFlag,0), ifnull(BlockFlag,0), ifnull(MonCallFlag,0), ifnull(EquipFlag,0),ifnull(UnequipFlag,0), ifnull(YchatFlag,0), ifnull(NchatFlag,0), ifnull(NoticeFlag,0),ifnull(ItemFlag,0), ifnull(MoneyFlag,0), ifnull(ExpFlag,0), ifnull(DieFlag,0),ifnull(GodFlag,0), ifnull(ChatFlag,0), ifnull(TradeFlag,0), ifnull(ShopFlag,0), ifnull(PShopFlag,0), ifnull(GItemFlag,0), ifnull(DItemFlag,0), ifnull(AttackFlag,0), ifnull(UZonemoveFlag,0), '1' as uBonus100Money from %s a left join %s b on a.uID = b.uID where a.uID='%s';", mSERVER_INFO.mDB_1_Table01, mSERVER_INFO.mDB_1_Table08, tID );
	/*
	   sprintf( mQUERY, "select uLoginState,uIP,a.uID,uPassword,uCheckMOTP,uMousePassword,uSecretCard,uBlockInfo,uUserSort,uGoodFellow,uLoginPlace,uLoginPremium,uTraceState, AuthType, FindFlag, MoveFlag, MoveZoneFlag, CallFlag, HideFlag, ShowFlag, KickFlag, BlockFlag, MonCallFlag, EquipFlag, UnequipFlag, YchatFlag, NchatFlag, NoticeFlag, ItemFlag, MoneyFlag, ExpFlag, DieFlag, GodFlag, ChatFlag from %s a left join %s b on a.uID = b.uID where a.uID='%s';", mSERVER_INFO.mDB_1_Table01, mSERVER_INFO.mDB_1_Table08, tID );
	   */
	if( !RECONNECT_TO_DATABASE02() )
	{
		LOG_TO_FILE_1 ("![WARNING - (6)] Unable to connect to the database[%s].\n",
				mSERVER_INFO.mDB_1_Table01);
		*tResult = 6;
		return;
	}
	iAffected = 0;
	if ((iAffected = mDB02.exec_query (mQUERY)) < 0) {
		LOG_TO_FILE ("![WARNING - (6)] The query execution failed.\n");
		LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB02.get_error_msg ().c_str ());
		*tResult = 6;
		return;
	}
	iCounts = 0;
#if 0
	if((iCounts = mDB01.get_num_rows ()) != 1)
	{
		*tResult = 6;
		return;
	}

	uLoginState = ::atol (mDB01.get_value (0, 0));
	::strncpy (uIP, mDB01.get_value (0, 1), 15);
	::strncpy (uID, mDB01.get_value (0, 2), MAX_USER_ID_LENGTH - 1);
	::strncpy (uPassword, mDB01.get_value (0, 3), MAX_USER_PASSWORD_LENGTH - 1);
	uCheckMOTP = ::atol (mDB01.get_value (0, 4));
	::strncpy (uMousePassword, mDB01.get_value (0, 5), MAX_MOUSE_PASSWORD_LENGTH - 1);
	uSecretCard = ::atol (mDB01.get_value (0, 6));
	uBlockInfo = ::atol (mDB01.get_value (0, 7));
	uUserSort = ::atol (mDB01.get_value (0, 8));
	uGoodFellow = ::atol (mDB01.get_value (0, 9));
	uLoginPlace = ::atol (mDB01.get_value (0, 10));
	uLoginPremium = ::atol (mDB01.get_value (0, 11));
	uTraceState = ::atol (mDB01.get_value (0, 12));
	mDB01.clear_res ();
#endif
	res = mDB02.get_res ();
	if (res == NULL) {
		LOG_TO_FILE_1 ("!Query failed - (%s).\n", mQUERY);
		LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB02.get_error_msg ().c_str ());
		*tResult = 6;
		return ;
	}
	while ((row = mysql_fetch_row (res))){
		uLoginState = ::atol (row[0]);
		snprintf (uIP, 16, "%s", row[1]);
		snprintf (uID, MAX_USER_ID_LENGTH, "%s", row[2]);
		snprintf (uPassword, MAX_USER_PASSWORD_LENGTH, "%s", row[3]);
		uCheckMOTP = ::atol (row[4]);
		snprintf (uMousePassword, MAX_MOUSE_PASSWORD_LENGTH, "%s", row[5]);
		uSecretCard = ::atol (row[6]);
		uBlockInfo = ::atol (row[7]);
		uUserSort = ::atol (row[8]);
		uGoodFellow = ::atol (row[9]);
		uLoginPlace = ::atol (row[10]);
		uLoginPremium = ::atol (row[11]);
		uTraceState = ::atol (row[12]);
		AuthType = ::atol(row[13]);
		FindFlag = ::atol(row[14]);
		MoveFlag = ::atol(row[15]);
		MoveZoneFlag = ::atol(row[16]);
		CallFlag = ::atol(row[17]);
		HideFlag = ::atol(row[18]);
		ShowFlag = ::atol(row[19]);
		KickFlag = ::atol(row[20]);
		BlockFlag = ::atol(row[21]);
		MonCallFlag = ::atol(row[22]);
		EquipFlag = ::atol(row[23]);
		UnequipFlag = ::atol(row[24]);
		YchatFlag = ::atol(row[25]);
		NchatFlag = ::atol(row[26]);
		NoticeFlag = ::atol(row[27]);
		ItemFlag = ::atol(row[28]);
		MoneyFlag = ::atol(row[29]);
		ExpFlag = ::atol(row[30]);
		DieFlag = ::atol(row[31]);
		GodFlag = ::atol(row[32]);
		ChatFlag = ::atol(row[33]);
		TradeFlag = ::atol(row[34]);
		ShopFlag = ::atol(row[35]);
		PShopFlag = ::atol(row[36]);
		GItemFlag = ::atol(row[37]);
		DItemFlag = ::atol(row[38]);
		AttackFlag = ::atol(row[39]);
		UZonemoveFlag = ::atol(row[40]);
		uBonus100Money = *row[41];
		uSecretCard = ::atol(row[42]); 
		uPremiumServerExpirationDate = ::atol(row[43]); // @_Premium_Server_@
		++iCounts;
	}
	mDB02.clear_res ();
	if (iCounts != 1){
		
		
		
		//night_walker oto_kayit
		
		
		//sprintf( mQUERY, "insert into %s (uID,uPassword) values ('%s','%s')", mSERVER_INFO.mDB_1_Table01, tID, tPassword );
		//LOG_TO_FILE_3("insert into %s (uID,uPassword) values ('%s','%s')",mSERVER_INFO.mDB_1_Table01, tID, tPassword );
		//mDB01.exec_query (mQUERY);
		//goto oto_kayit;
		//night_walker oto_kayit
		
		
		LOG_TO_FILE ("![WARNING - (6)] As a result of a number of rows is not returned by the query 1.\n");
		LOG_TO_FILE_1 ("\t\t\tRow counts = %d.\n", iCounts);
		*tResult = 6;
		return;
	}
	//if( strcmp( uIP, "0" ) != 0 )
	//{
	//    if( strcmp( uIP, tIP ) != 0 )
	//    {
	//        LOG_TO_FILE ("[WARNING - (12)] Your account is different.\n");
	//        *tResult = 12;
	//        return;
	//    }
	//}
#ifndef __CHANNEL__
	if( strcmp( tPassword, uPassword ) != 0 )
	{
		LOG_TO_FILE ("![WARNING - (7)] Your password is difference.\n");
		*tResult = 7;
		return;
	}
#endif
	if( uLoginState == 1 )
	{
		LOG_TO_FILE ("![WARNING - (8)] This is already logged in with an account.\n");
		*tResult = 8;
		return;
	}
	if( uBlockInfo >= mGAME.ReturnNowDate() )
	{
		LOG_TO_FILE ("![WARNING - (9)] The current account is blocked.\n");
		*tResult = 9;
		return;
	}
	if(0 < uUserSort)
		AuthType = 1;
	
    // @_Premium_Server_@
    // 프리미엄 서버(월드)인 경우 프리미엄 서버 입장권 만료 시간이 지났는 지 검사.
    if (mSERVER_INFO.mCheckPremiumServer == 1)
    {
        if (uPremiumServerExpirationDate < mGAME.ReturnNowDate())
        {
            LOG_TO_FILE ("![WARNING - (30)] Unable login premium server(Premium Server Ticket expired).\n");
            *tResult = 30;
            return;
        }
    } // @

	*tCheckMOTP = uCheckMOTP;
	strcpy( tMousePassword, uMousePassword );
	*tSecretCard = uSecretCard;
	*tUserSort = uUserSort;
	*tGoodFellow = uGoodFellow;
	*tLoginPlace = uLoginPlace;
	*tLoginPremium = uLoginPremium;
	*tTraceState = uTraceState;
	*tBonus100Money = uBonus100Money;
	*tPremiumServerExpirationDate = uPremiumServerExpirationDate; // @_Premium_Server_@
	tAuthInfo->AuthType = AuthType;
	tAuthInfo->FindFlag = FindFlag;
	tAuthInfo->MoveFlag = MoveFlag;
	tAuthInfo->MoveZoneFlag = MoveZoneFlag;
	tAuthInfo->CallFlag = CallFlag;
	tAuthInfo->HideFlag = HideFlag;
	tAuthInfo->ShowFlag = ShowFlag;
	tAuthInfo->KickFlag = KickFlag;
	tAuthInfo->BlockFlag = BlockFlag;
	tAuthInfo->MonCallFlag = MonCallFlag;
	tAuthInfo->EquipFlag = EquipFlag;
	tAuthInfo->UnequipFlag = UnequipFlag;
	tAuthInfo->YchatFlag = YchatFlag;
	tAuthInfo->NchatFlag = NchatFlag;
	tAuthInfo->NoticeFlag = NoticeFlag;
	tAuthInfo->ItemFlag = ItemFlag;
	tAuthInfo->MoneyFlag = MoneyFlag;
	tAuthInfo->ExpFlag = ExpFlag;
	tAuthInfo->DieFlag = DieFlag;
	tAuthInfo->GodFlag = GodFlag;
	tAuthInfo->ChatFlag = ChatFlag;
	tAuthInfo->TradeFlag = TradeFlag;
	tAuthInfo->ShopFlag = ShopFlag;
	tAuthInfo->PShopFlag = PShopFlag;
	tAuthInfo->GItemFlag = GItemFlag;
	tAuthInfo->DItemFlag = DItemFlag;
	tAuthInfo->AttackFlag = AttackFlag;
	tAuthInfo->UZonemoveFlag = UZonemoveFlag;
	//LOG_TO_FILE_1("======> tMousePassword : %s\n", tMousePassword);

	sprintf( mQUERY, "select count(uID) from %s where uID='%s' and aWorldNumber = %d", mSERVER_INFO.mDB_2_Table02, uID, mSERVER_INFO.mWorldNumber);
	if( !RECONNECT_TO_DATABASE01() )
	{
		LOG_TO_FILE_1 ("![WARNING - (6)] Unable to connect to the database[%s].\n",
				mSERVER_INFO.mDB_2_Table02);
		*tResult = 6;
		return;
	}
	iAffected = 0;
	if ((iAffected = mDB01.exec_query (mQUERY)) < 0) {
		LOG_TO_FILE ("![WARNING - (6)] The query execution failed.\n");
		LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB01.get_error_msg ().c_str ());
		*tResult = 6;
		return ;
	}
	res = mDB01.get_res ();
	if (res == NULL) {
		LOG_TO_FILE_1 ("!Query failed - (%s).\n", mQUERY);
		LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB01.get_error_msg ().c_str ());
		*tResult = 6;
		return ;
	}
	iCounts = 0;

	while ((row = mysql_fetch_row(res))){
		uIDNum = ::atol (row[0]);
		++iCounts;
	}
	mDB01.clear_res ();

	if (iCounts != 1){
		LOG_TO_FILE ("![WARNING - (6)] As a result of a number of rows is not returned by the query.\n");
		LOG_TO_FILE_1 ("\t\t\tRow counts = %d.\n", iCounts);
		*tResult = 6;
		return;
	} 

	::memset (uSaveItem, '0', 700);

	if( uIDNum == 0 )
	{
		uSaveMoney = 0;
		uSaveItem[700] = '\0';
		strcpy( aName[0], "" );
		strcpy( aName[1], "" );
		strcpy( aName[2], "" );
		sprintf( mQUERY, "insert into %s values ( now(),%d, '%s',%d,'%s','%s','%s','%s')", mSERVER_INFO.mDB_2_Table02, mSERVER_INFO.mWorldNumber, uID, uSaveMoney, uSaveItem, aName[0], aName[1], aName[2] );
		//*tFirstLogin = '1';
		if( !RECONNECT_TO_DATABASE01() )
		{
			LOG_TO_FILE_1 ("![WARNING - (6)] Unable to connect to the database[%s].\n",
					mSERVER_INFO.mDB_2_Table02);
			*tResult = 6;
			return;
		}
		iAffected = 0;
		if ((mDB01.exec_query (mQUERY, CMySQL::CLEAR)) != 0) {
			LOG_TO_FILE ("![WARNING - (6)] The query execution failed.\n");
			LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB01.get_error_msg ().c_str ());
			*tResult = 6;
			return ;
		}
#if 0
		for( index01 = 0 ; index01 < MAX_USER_AVATAR_NUM ; index01++ ) {
			for( index00 = 0 ; index00 < MAX_SAVE_ITEM_SLOT_NUM ; index00++ ) {
				tAvatarInfo[index01].uSaveItem[index00][0] = tSaveItem[index00][0] = 0;
				tAvatarInfo[index01].uSaveItem[index00][1] = tSaveItem[index00][1] = 0;
				tAvatarInfo[index01].uSaveItem[index00][2] = tSaveItem[index00][2] = 0;
				tAvatarInfo[index01].uSaveItem[index00][3] = tSaveItem[index02][3] = 0;
			}
		}
		*tResult = 0;
		return;
#endif
	}
	else
	{
		sprintf( mQUERY, "select uSaveMoney,uSaveItem,aName01,aName02,aName03 from %s where uID='%s' and aWorldNumber = %d", mSERVER_INFO.mDB_2_Table02, uID, mSERVER_INFO.mWorldNumber);
		//*tFirstLogin = '0';
		if( !RECONNECT_TO_DATABASE01() )
		{
			LOG_TO_FILE_1 ("![WARNING - (6)] Unable to connect to the database[%s].\n",
					mSERVER_INFO.mDB_2_Table02);
			*tResult = 6;
			return;
		}
		iAffected = 0;
		if ((iAffected = mDB01.exec_query (mQUERY)) < 0) {
			LOG_TO_FILE ("![WARNING - (6)] The query execution failed.\n");
			LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB01.get_error_msg ().c_str ());
			*tResult = 6;
			return ;
		}
		iCounts = 0;

		res = mDB01.get_res ();
		if (res == NULL) {
			LOG_TO_FILE_1 ("!Query failed - (%s).\n", mQUERY);
			LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB01.get_error_msg ().c_str ());
			*tResult = 6;
			return ;
		}
		while (row = mysql_fetch_row (res)){
			uSaveMoney = ::atol (row[0]);
			snprintf (uSaveItem, 701, "%s", row[1]);
			//LOG_TO_FILE_2("------> %s %d\n", uSaveItem, strlen(uSaveItem));
			::snprintf(aName[0], MAX_AVATAR_NAME_LENGTH, "%s", row[2]);
			::snprintf(aName[1], MAX_AVATAR_NAME_LENGTH, "%s", row[3]);
			::snprintf(aName[2], MAX_AVATAR_NAME_LENGTH, "%s", row[4]);
			++iCounts;
		}
		mDB01.clear_res ();

		if (1 != iCounts){
			LOG_TO_FILE ("![WARNING - (6)] As a result of a number of rows is not returned by the query.\n");
			LOG_TO_FILE_1 ("\t\t\tRow counts = %d.\n", iCounts);
			*tResult = 6;
			return;
		}
	}

	*tSaveMoney = uSaveMoney;
	//LOG_TO_FILE_1("------> Start %s\n", uID);
	for( index01 = 0 ; index01 < MAX_SAVE_ITEM_SLOT_NUM ; index01++ )
	{
		CopyMemory( &tempString01[0], &uSaveItem[( index01 * 25 + 0 )], 5 );
		tempString01[5] = '\0';
		tSaveItem[index01][0] = atoi( tempString01 );
		CopyMemory( &tempString01[0], &uSaveItem[( index01 * 25 + 5 )], 3 );
		tempString01[3] = '\0';
		tSaveItem[index01][1] = atoi( tempString01 );
		CopyMemory( &tempString01[0], &uSaveItem[( index01 * 25 + 8 )], 9 );
		tempString01[9] = '\0';
		tSaveItem[index01][2] = atoi( tempString01 );
		CopyMemory( &tempString01[0], &uSaveItem[( index01 * 25 + 17 )], 8 );
		tempString01[8] = '\0';
		tSaveItem[index01][3] = atoi( tempString01 );
		//LOG_TO_FILE_4("------> %d %d %d %d\n", tSaveItem[index01][0], tSaveItem[index01][1], tSaveItem[index01][2], tSaveItem[index01][3]);
	}
	//LOG_TO_FILE_1("------> end %s\n", uID);
	for( index01 = 0 ; index01 < MAX_USER_AVATAR_NUM ; index01++ )
	{
		RealGuild[index01] = false;
		CheckGuild[index01] = false;
		::memset(gMember, 0, 851);
		strcpy( tAvatarInfo[index01].aName, aName[index01] );
		if( strcmp( tAvatarInfo[index01].aName, "" ) == 0 )
		{
			continue;
		}
#ifdef __TK__
		sprintf(mQUERY,
			"SELECT IFNULL(b.aWorldNumber, %d), IFNULL(b.gMember, '') "
			"	, a.uID, a.aVisibleState, a.aSpecialState, a.aPlayTime1, a.aPlayTime2 "
			"	, a.aKillOtherTribe, a.aTribe, a.aPreviousTribe, a.aGender, a.aHeadType "
			"	, a.aFaceType, a.aLevel1, a.aLevel2, a.aGeneralExperience1, a.aGeneralExperience2 "
			"	, a.aVitality, a.aStrength, a.aKi, a.aWisdom, a.aEatLifePotion "
			"	, a.aEatManaPotion, a.aStateBonusPoint, a.aSkillPoint, a.aEquip "
			"	, a.aExpandInventoryDate, a.aMoney, a.aInventory, a.aTradeMoney, a.aTrade "
			"	, a.aExpandStoreDate, a.aStoreMoney, a.aStoreItem, a.aSkill, a.aHotKey "
			"	, a.aQuestInfo, a.aFriend, a.aTeacher, a.aStudent, a.aTeacherPoint "
			"	, a.aGuildName, a.aGuildRole, a.aCallName, a.aGuildMarkNum, a.aGuildMarkEffect "
			"	, a.aLogoutInfo01, a.aLogoutInfo02, a.aLogoutInfo03, a.aLogoutInfo04, a.aLogoutInfo05 "
			"	, a.aLogoutInfo06, a.aProtectForDeath, a.aProtectForDestroy, a.aDoubleExpTime1, a.aDoubleExpTime2 "
			"	, a.aDoubleKillNumTime, a.aDoubleKillExpTime, a.aNineTowerKeyNum, a.aZone101TypeTime, a.aZone125TypeTime "
			"	, a.aZone126TypeTime, a.aKillMonsterNum, a.aLevelZoneKeyNum, a.aSearchAndBuyDate, a.aLifePotionConvertNum "
			"	, a.aManaPotionConvertNum, a.aTribeVoteDate, a.aUse_AutoPotion_Date, a.aUse_AutoPotion_Time, a.aAutoLifeRatio "
			"	, a.aAutoManaRatio, a.aAnimal01, a.aAnimal02, a.aAnimal03, a.aAnimal04 "
			"	, a.aAnimal05, a.aAnimal06, a.aAnimal07, a.aAnimal08, a.aAnimal09 "
			"	, a.aAnimal10, a.aAnimalIndex, a.aAnimalTime, a.aPlayTime3, a.aDoubleExpTime3 "
			"	, a.aDoubleDropTime, iCriminal_Record, iEnter_The_Prison, iKill_Monster_Count, a.aEatStrengthPotion "
			"	, a.aEatWisdomPotion,aPat_Exp_x2_Time,aPat_User_Exp_x2_Time,aFiveExpTime,aZone84Time "
			"	, a.iSilverOrnamentRemainsTime,a.iGoldOrnamentRemainsTime, a.iUseOrnament "
			"	, a.aPackSilverDate,a.aPackGoldDate, a.aPackDiamondDate "
			"	, a.aPackSilverDateMobile,a.aPackGoldDateMobile, a.aPackDiamondDateMobile "
			" , a.aLevelZoneKeyNum2, a.aDoubleKillNumTime2, a.aKillMonsterNum2 "
#ifdef __GOD__
			"   , a.aAddItemValue, a.aHighItemValue, a.aDropItemTime, a.aTribeCallValue, a.aUpgradeValue "
			"   , a.aBonusItemValue, a.aKillOtherTribeEvent, a.aTeacherPointEvent, a.aPlayTimeEvent, a.aProtectForUpgrade "
#endif
			"FROM %s a "
			"LEFT OUTER JOIN GuildInfo b ON a.aGuildName = b.gName "
			"WHERE a.aName='%s' AND a.aWorldNumber = %d "
			, mSERVER_INFO.mWorldNumber, mSERVER_INFO.mDB_2_Table03, tAvatarInfo[index01].aName, mSERVER_INFO.mWorldNumber);
#else
		sprintf(mQUERY,
			"SELECT IFNULL(b.aWorldNumber, %d), IFNULL(b.gMember, '') "
			"	, a.uID, a.aVisibleState, a.aSpecialState, a.aPlayTime1, a.aPlayTime2 "
			"	, a.aKillOtherTribe, a.aTribe, a.aPreviousTribe, a.aGender, a.aHeadType "
			"	, a.aFaceType, a.aLevel1, a.aLevel2, a.aGeneralExperience1, a.aGeneralExperience2 "
			"	, a.aVitality, a.aStrength, a.aKi, a.aWisdom, a.aEatLifePotion "
			"	, a.aEatManaPotion, a.aStateBonusPoint, a.aSkillPoint, a.aEquip "
			"	, a.aExpandInventoryDate, a.aMoney, a.aInventory, a.aTradeMoney, a.aTrade "
			"	, a.aExpandStoreDate, a.aStoreMoney, a.aStoreItem, a.aSkill, a.aHotKey "
			"	, a.aQuestInfo, a.aFriend, a.aTeacher, a.aStudent, a.aTeacherPoint "
			"	, a.aGuildName, a.aGuildRole, a.aCallName, a.aGuildMarkNum, a.aGuildMarkEffect "
			"	, a.aLogoutInfo01, a.aLogoutInfo02, a.aLogoutInfo03, a.aLogoutInfo04, a.aLogoutInfo05 "
			"	, a.aLogoutInfo06, a.aProtectForDeath, a.aProtectForDestroy, a.aDoubleExpTime1, a.aDoubleExpTime2 "
			"	, a.aDoubleKillNumTime, a.aDoubleKillExpTime, a.aNineTowerKeyNum, a.aZone101TypeTime, a.aZone125TypeTime "
			"	, a.aZone126TypeTime, a.aKillMonsterNum, a.aLevelZoneKeyNum, a.aSearchAndBuyDate, a.aLifePotionConvertNum "
			"	, a.aManaPotionConvertNum, a.aTribeVoteDate, a.aUse_AutoPotion_Date, a.aUse_AutoPotion_Time, a.aAutoLifeRatio "
			"	, a.aAutoManaRatio, a.aAnimal01, a.aAnimal02, a.aAnimal03, a.aAnimal04 "
			"	, a.aAnimal05, a.aAnimal06, a.aAnimal07, a.aAnimal08, a.aAnimal09 "
			"	, a.aAnimal10, a.aAnimalIndex, a.aAnimalTime, a.aPlayTime3, a.aDoubleExpTime3 "
			"	, a.aDoubleDropTime, iCriminal_Record, iEnter_The_Prison, iKill_Monster_Count, a.aEatStrengthPotion "
			"	, a.aEatWisdomPotion,aPat_Exp_x2_Time,aPat_User_Exp_x2_Time,aFiveExpTime,aZone84Time "
			"	, a.iSilverOrnamentRemainsTime,a.iGoldOrnamentRemainsTime, a.iUseOrnament "
			"   , a.aLevelZoneKeyNum2, a.aDoubleKillNumTime2, a.aKillMonsterNum2 "
#ifdef __GOD__
			"   , a.aAddItemValue, a.aHighItemValue, a.aDropItemTime, a.aTribeCallValue, a.aUpgradeValue "
			"   , a.aBonusItemValue, a.aKillOtherTribeEvent, a.aTeacherPointEvent, a.aPlayTimeEvent, a.aProtectForUpgrade "
#endif
			"FROM %s a "
			"LEFT OUTER JOIN GuildInfo b ON a.aGuildName = b.gName "
			"WHERE a.aName='%s' AND a.aWorldNumber = %d "
			, mSERVER_INFO.mWorldNumber, mSERVER_INFO.mDB_2_Table03, tAvatarInfo[index01].aName, mSERVER_INFO.mWorldNumber);
#endif
		

		if( !RECONNECT_TO_DATABASE01() )
		{
			LOG_TO_FILE_1 ("![WARNING - (6)] Unable to connect to the database[%s].\n",
					mSERVER_INFO.mDB_2_Table03);
			*tResult = 6;
			return;
		}
		iAffected = 0;
		if ((iAffected = mDB01.exec_query (mQUERY)) < 0) {
			LOG_TO_FILE ("![WARNING - (6)] The query execution failed.\n");
			LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB01.get_error_msg ().c_str ());
			*tResult = 6;
			return ;
		}
		iCounts = 0;
		res = mDB01.get_res();
		if (res == NULL) {
			LOG_TO_FILE_1 ("!Query failed - (%s).\n", mQUERY);
			LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB01.get_error_msg ().c_str ());
			*tResult = 6;
			return ;
		}
		while (row = mysql_fetch_row(res)){
			aWorldNumber = ::atoi(row[0]);
			if(aWorldNumber == mSERVER_INFO.mWorldNumber) {
				RealGuild[index01] = true;
				::snprintf(gMember, 851, "%s", row[1]);
			} else if(!RealGuild[index01]) {
				::strcpy(gMember, "");
			}

#ifdef __TK__
			if(iCounts == 0) {
				::strncpy (uIDForCheck, row[2], MAX_USER_ID_LENGTH - 1);
				tAvatarInfo[index01].aVisibleState = ::atol (row[3]);
				tAvatarInfo[index01].aSpecialState = ::atol (row[4]);
				tAvatarInfo[index01].aPlayTime1 = ::atol (row[5]);
				tAvatarInfo[index01].aPlayTime2 = ::atol (row[6]);
				tAvatarInfo[index01].aKillOtherTribe = ::atol (row[7]);
				tAvatarInfo[index01].aTribe = ::atol (row[8]);
				tAvatarInfo[index01].aPreviousTribe = ::atol (row[9]);
				tAvatarInfo[index01].aGender = ::atol (row[10]);
				tAvatarInfo[index01].aHeadType = ::atol (row[11]);
				tAvatarInfo[index01].aFaceType = ::atol (row[12]);
				tAvatarInfo[index01].aLevel1 = ::atol (row[13]);
				tAvatarInfo[index01].aLevel2 = ::atol (row[14]);
				tAvatarInfo[index01].aGeneralExperience1 = ::atol (row[15]);
				tAvatarInfo[index01].aGeneralExperience2 = ::atol (row[16]);
				tAvatarInfo[index01].aVitality = ::atol (row[17]);
				tAvatarInfo[index01].aStrength = ::atol (row[18]);
				tAvatarInfo[index01].aKi = ::atol (row[19]);
				tAvatarInfo[index01].aWisdom = ::atol (row[20]);
				tAvatarInfo[index01].aEatLifePotion = ::atol (row[21]);
				tAvatarInfo[index01].aEatManaPotion = ::atol (row[22]);
				tAvatarInfo[index01].aStateBonusPoint = ::atol (row[23]);
				tAvatarInfo[index01].aSkillPoint = ::atol (row[24]);
				::strncpy (aEquip, row[25], 326);
				tAvatarInfo[index01].aExpandInventoryDate = ::atol (row[26]);
				tAvatarInfo[index01].aMoney = ::atol (row[27]);
				::strncpy (aInventory, row[28], 3457);
				tAvatarInfo[index01].aTradeMoney = ::atol (row[29]);
				::strncpy (aTrade, row[30], 201);
				tAvatarInfo[index01].aExpandStoreDate = ::atol (row[31]);
				tAvatarInfo[index01].aStoreMoney = ::atol (row[32]);
				::strncpy (aStoreItem, row[33], 1401);
				::strncpy (aSkill, row[34], 181);
				::strncpy (aHotKey, row[35], 421);
				::strncpy (aQuestInfo, row[36], 26);
				::strncpy (aFriend, row[37], 121);
				::strncpy (tAvatarInfo[index01].aTeacher, row[38], MAX_AVATAR_NAME_LENGTH - 1);
				::strncpy (tAvatarInfo[index01].aStudent, row[39], MAX_AVATAR_NAME_LENGTH - 1);
				tAvatarInfo[index01].aTeacherPoint = ::atol (row[40]);
				::strncpy (tAvatarInfo[index01].aGuildName, row[41], MAX_GUILD_NAME_LENGTH - 1);
				tAvatarInfo[index01].aGuildRole = ::atol (row[42]);
				::strncpy (tAvatarInfo[index01].aCallName, row[43], MAX_CALL_NAME_LENGTH - 1);
				tAvatarInfo[index01].aGuildMarkNum = ::atol (row[44]);
				tAvatarInfo[index01].aGuildMarkEffect = ::atol (row[45]);
				tAvatarInfo[index01].aLogoutInfo[0] = ::atol (row[46]);
				tAvatarInfo[index01].aLogoutInfo[1] = ::atol (row[47]);
				tAvatarInfo[index01].aLogoutInfo[2] = ::atol (row[48]);
				tAvatarInfo[index01].aLogoutInfo[3] = ::atol (row[49]);
				tAvatarInfo[index01].aLogoutInfo[4] = ::atol (row[50]);
				tAvatarInfo[index01].aLogoutInfo[5] = ::atol (row[51]);
				tAvatarInfo[index01].aProtectForDeath = ::atol (row[52]);
				tAvatarInfo[index01].aProtectForDestroy = ::atol (row[53]);
				tAvatarInfo[index01].aDoubleExpTime1 = ::atol (row[54]);
				tAvatarInfo[index01].aDoubleExpTime2 = ::atol (row[55]);
				tAvatarInfo[index01].aDoubleKillNumTime = ::atol (row[56]);
				tAvatarInfo[index01].aDoubleKillExpTime = ::atol (row[57]);
				tAvatarInfo[index01].aNineTowerKeyNum = ::atol (row[58]);
				tAvatarInfo[index01].aZone101TypeTime = ::atol (row[59]);
				tAvatarInfo[index01].aZone125TypeTime = ::atol (row[60]);
				tAvatarInfo[index01].aZone126TypeTime = ::atol (row[61]);
				tAvatarInfo[index01].aKillMonsterNum = ::atol (row[62]);
				tAvatarInfo[index01].aLevelZoneKeyNum = ::atol (row[63]);
				tAvatarInfo[index01].aSearchAndBuyDate = ::atol (row[64]);
				tAvatarInfo[index01].aLifePotionConvertNum = ::atol (row[65]);
				tAvatarInfo[index01].aManaPotionConvertNum = ::atol (row[66]);
				tAvatarInfo[index01].aTribeVoteDate = ::atol (row[67]);
				tAvatarInfo[index01].aUse_AutoPotion_Date = ::atol (row[68]);
				tAvatarInfo[index01].aUse_AutoPotion_Time = ::atol (row[69]);
				tAvatarInfo[index01].aAutoLifeRatio = ::atol (row[70]);
				tAvatarInfo[index01].aAutoManaRatio = ::atol (row[71]);
				tAvatarInfo[index01].aAnimal[0] = ::atol (row[72]);
				tAvatarInfo[index01].aAnimal[1] = ::atol (row[73]);
				tAvatarInfo[index01].aAnimal[2] = ::atol (row[74]);
				tAvatarInfo[index01].aAnimal[3] = ::atol (row[75]);
				tAvatarInfo[index01].aAnimal[4] = ::atol (row[76]);
				tAvatarInfo[index01].aAnimal[5] = ::atol (row[77]);
				tAvatarInfo[index01].aAnimal[6] = ::atol (row[78]);
				tAvatarInfo[index01].aAnimal[7] = ::atol (row[79]);
				tAvatarInfo[index01].aAnimal[8] = ::atol (row[80]);
				tAvatarInfo[index01].aAnimal[9] = ::atol (row[81]);
				tAvatarInfo[index01].aAnimalIndex = ::atol (row[82]);
				tAvatarInfo[index01].aAnimalTime = ::atol (row[83]);
				tAvatarInfo[index01].aPlayTime3 = ::atol (row[84]);
				tAvatarInfo[index01].aDoubleExpTime3 = ::atol (row[85]);
				tAvatarInfo[index01].aDoubleDropTime = ::atol (row[86]);
				//@{ auto quiz
				tAvatarInfo[index01].iCriminal_Record = ::atol (row[87]);
				tAvatarInfo[index01].iEnter_The_Prison = ::atol (row[88]);
				tAvatarInfo[index01].iKill_Monster_Count = ::atol (row[89]);
				//@}
				tAvatarInfo[index01].aEatStrengthPotion = ::atol (row[90]);
				tAvatarInfo[index01].aEatWisdomPotion = ::atol (row[91]);
				tAvatarInfo[index01].aPat_Exp_x2_Time = ::atol (row[92]);
				tAvatarInfo[index01].aPat_User_Exp_x2_Time = ::atol (row[93]);
				tAvatarInfo[index01].aFiveExpTime = ::atol (row[94]);
				tAvatarInfo[index01].aZone84Time = ::atol (row[95]);
				/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */
				tAvatarInfo[index01].iSilverOrnamentRemainsTime = ::atol (row[96]);
				tAvatarInfo[index01].iGoldOrnamentRemainsTime = ::atol (row[97]);
				tAvatarInfo[index01].iUseOrnament = ::atol (row[98]);
				/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */
				/** 2009. 11. 23 : 패키지 아이템개발 (wolf) */
				tAvatarInfo[index01].aPackSilverDate = ::atol (row[99]);
				tAvatarInfo[index01].aPackGoldDate = ::atol (row[100]);
				tAvatarInfo[index01].aPackDiamondDate = ::atol (row[101]);
				tAvatarInfo[index01].aPackSilverDateMobile = ::atol (row[102]);
				tAvatarInfo[index01].aPackGoldDateMobile = ::atol (row[103]);
				tAvatarInfo[index01].aPackDiamondDateMobile = ::atol (row[104]);
				/** 2009. 11. 23 : 패키지 아이템개발 (wolf) */
				/** 2009. 12. 16 : 강화, 조합, 승급부(특) 아이템 개발 (wolf) */
				tAvatarInfo[index01].aLevelZoneKeyNum2 = ::atol (row[105]);
				/** 2009. 12. 16 : 강화, 조합, 승급부(특) 아이템 개발 (wolf) */
				/** 2009. 12. 16 : 귀멸부 아이템 개발 (wolf) */
				tAvatarInfo[index01].aDoubleKillNumTime2 = ::atol (row[106]);
				/** 2009. 12. 16 : 귀멸부 아이템 개발 (wolf) */
				tAvatarInfo[index01].aKillMonsterNum2 = ::atol (row[107]);
#ifdef __GOD__	
				tAvatarInfo[index01].aAddItemValue = ::atol (row[108]);
				tAvatarInfo[index01].aHighItemValue = ::atol (row[109]);
				tAvatarInfo[index01].aDropItemTime = ::atol (row[110]);
				tAvatarInfo[index01].aTribeCallValue = ::atol (row[111]);
				tAvatarInfo[index01].aUpgradeValue = ::atol (row[112]);
				tAvatarInfo[index01].aBonusItemValue = ::atol (row[113]);
				tAvatarInfo[index01].aKillOtherTribeEvent = ::atol (row[114]);
				tAvatarInfo[index01].aTeacherPointEvent = ::atol (row[115]);
				tAvatarInfo[index01].aPlayTimeEvent = ::atol (row[116]);
				tAvatarInfo[index01].aProtectForUpgrade = ::atol (row[117]);
#endif
				++iCounts;

			}
#else
			if(iCounts == 0) {
				::strncpy (uIDForCheck, row[2], MAX_USER_ID_LENGTH - 1);
				tAvatarInfo[index01].aVisibleState = ::atol (row[3]);
				tAvatarInfo[index01].aSpecialState = ::atol (row[4]);
				tAvatarInfo[index01].aPlayTime1 = ::atol (row[5]);
				tAvatarInfo[index01].aPlayTime2 = ::atol (row[6]);
				tAvatarInfo[index01].aKillOtherTribe = ::atol (row[7]);
				tAvatarInfo[index01].aTribe = ::atol (row[8]);
				tAvatarInfo[index01].aPreviousTribe = ::atol (row[9]);
				tAvatarInfo[index01].aGender = ::atol (row[10]);
				tAvatarInfo[index01].aHeadType = ::atol (row[11]);
				tAvatarInfo[index01].aFaceType = ::atol (row[12]);
				tAvatarInfo[index01].aLevel1 = ::atol (row[13]);
				tAvatarInfo[index01].aLevel2 = ::atol (row[14]);
				tAvatarInfo[index01].aGeneralExperience1 = ::atol (row[15]);
				tAvatarInfo[index01].aGeneralExperience2 = ::atol (row[16]);
				tAvatarInfo[index01].aVitality = ::atol (row[17]);
				tAvatarInfo[index01].aStrength = ::atol (row[18]);
				tAvatarInfo[index01].aKi = ::atol (row[19]);
				tAvatarInfo[index01].aWisdom = ::atol (row[20]);
				tAvatarInfo[index01].aEatLifePotion = ::atol (row[21]);
				tAvatarInfo[index01].aEatManaPotion = ::atol (row[22]);
				tAvatarInfo[index01].aStateBonusPoint = ::atol (row[23]);
				tAvatarInfo[index01].aSkillPoint = ::atol (row[24]);
				::strncpy (aEquip, row[25], 326);
				tAvatarInfo[index01].aExpandInventoryDate = ::atol (row[26]);
				tAvatarInfo[index01].aMoney = ::atol (row[27]);
				::strncpy (aInventory, row[28], 3457);
				tAvatarInfo[index01].aTradeMoney = ::atol (row[29]);
				::strncpy (aTrade, row[30], 201);
				tAvatarInfo[index01].aExpandStoreDate = ::atol (row[31]);
				tAvatarInfo[index01].aStoreMoney = ::atol (row[32]);
				::strncpy (aStoreItem, row[33], 1401);
				::strncpy (aSkill, row[34], 181);
				::strncpy (aHotKey, row[35], 421);
				::strncpy (aQuestInfo, row[36], 26);
				::strncpy (aFriend, row[37], 121);
				::strncpy (tAvatarInfo[index01].aTeacher, row[38], MAX_AVATAR_NAME_LENGTH - 1);
				::strncpy (tAvatarInfo[index01].aStudent, row[39], MAX_AVATAR_NAME_LENGTH - 1);
				tAvatarInfo[index01].aTeacherPoint = ::atol (row[40]);
				::strncpy (tAvatarInfo[index01].aGuildName, row[41], MAX_GUILD_NAME_LENGTH - 1);
				tAvatarInfo[index01].aGuildRole = ::atol (row[42]);
				::strncpy (tAvatarInfo[index01].aCallName, row[43], MAX_CALL_NAME_LENGTH - 1);
				tAvatarInfo[index01].aGuildMarkNum = ::atol (row[44]);
				tAvatarInfo[index01].aGuildMarkEffect = ::atol (row[45]);
				tAvatarInfo[index01].aLogoutInfo[0] = ::atol (row[46]);
				tAvatarInfo[index01].aLogoutInfo[1] = ::atol (row[47]);
				tAvatarInfo[index01].aLogoutInfo[2] = ::atol (row[48]);
				tAvatarInfo[index01].aLogoutInfo[3] = ::atol (row[49]);
				tAvatarInfo[index01].aLogoutInfo[4] = ::atol (row[50]);
				tAvatarInfo[index01].aLogoutInfo[5] = ::atol (row[51]);
				tAvatarInfo[index01].aProtectForDeath = ::atol (row[52]);
				tAvatarInfo[index01].aProtectForDestroy = ::atol (row[53]);
				tAvatarInfo[index01].aDoubleExpTime1 = ::atol (row[54]);
				tAvatarInfo[index01].aDoubleExpTime2 = ::atol (row[55]);
				tAvatarInfo[index01].aDoubleKillNumTime = ::atol (row[56]);
				tAvatarInfo[index01].aDoubleKillExpTime = ::atol (row[57]);
				tAvatarInfo[index01].aNineTowerKeyNum = ::atol (row[58]);
				tAvatarInfo[index01].aZone101TypeTime = ::atol (row[59]);
				tAvatarInfo[index01].aZone125TypeTime = ::atol (row[60]);
				tAvatarInfo[index01].aZone126TypeTime = ::atol (row[61]);
				tAvatarInfo[index01].aKillMonsterNum = ::atol (row[62]);
				tAvatarInfo[index01].aLevelZoneKeyNum = ::atol (row[63]);
				tAvatarInfo[index01].aSearchAndBuyDate = ::atol (row[64]);
				tAvatarInfo[index01].aLifePotionConvertNum = ::atol (row[65]);
				tAvatarInfo[index01].aManaPotionConvertNum = ::atol (row[66]);
				tAvatarInfo[index01].aTribeVoteDate = ::atol (row[67]);
				tAvatarInfo[index01].aUse_AutoPotion_Date = ::atol (row[68]);
				tAvatarInfo[index01].aUse_AutoPotion_Time = ::atol (row[69]);
				tAvatarInfo[index01].aAutoLifeRatio = ::atol (row[70]);
				tAvatarInfo[index01].aAutoManaRatio = ::atol (row[71]);
				tAvatarInfo[index01].aAnimal[0] = ::atol (row[72]);
				tAvatarInfo[index01].aAnimal[1] = ::atol (row[73]);
				tAvatarInfo[index01].aAnimal[2] = ::atol (row[74]);
				tAvatarInfo[index01].aAnimal[3] = ::atol (row[75]);
				tAvatarInfo[index01].aAnimal[4] = ::atol (row[76]);
				tAvatarInfo[index01].aAnimal[5] = ::atol (row[77]);
				tAvatarInfo[index01].aAnimal[6] = ::atol (row[78]);
				tAvatarInfo[index01].aAnimal[7] = ::atol (row[79]);
				tAvatarInfo[index01].aAnimal[8] = ::atol (row[80]);
				tAvatarInfo[index01].aAnimal[9] = ::atol (row[81]);
				tAvatarInfo[index01].aAnimalIndex = ::atol (row[82]);
				tAvatarInfo[index01].aAnimalTime = ::atol (row[83]);
				tAvatarInfo[index01].aPlayTime3 = ::atol (row[84]);
				tAvatarInfo[index01].aDoubleExpTime3 = ::atol (row[85]);
				tAvatarInfo[index01].aDoubleDropTime = ::atol (row[86]);
				//@{ auto quiz
				tAvatarInfo[index01].iCriminal_Record = ::atol (row[87]);
				tAvatarInfo[index01].iEnter_The_Prison = ::atol (row[88]);
				tAvatarInfo[index01].iKill_Monster_Count = ::atol (row[89]);
				//@}
				tAvatarInfo[index01].aEatStrengthPotion = ::atol (row[90]);
				tAvatarInfo[index01].aEatWisdomPotion = ::atol (row[91]);
				tAvatarInfo[index01].aPat_Exp_x2_Time = ::atol (row[92]);
				tAvatarInfo[index01].aPat_User_Exp_x2_Time = ::atol (row[93]);
				tAvatarInfo[index01].aFiveExpTime = ::atol (row[94]);
				tAvatarInfo[index01].aZone84Time = ::atol (row[95]);
				/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */
				tAvatarInfo[index01].iSilverOrnamentRemainsTime = ::atol (row[96]);
				tAvatarInfo[index01].iGoldOrnamentRemainsTime = ::atol (row[97]);
				tAvatarInfo[index01].iUseOrnament = ::atol (row[98]);
				/** 2009. 12. 16 : 강화, 조합, 승급부(특) 아이템 개발 (wolf) */
				tAvatarInfo[index01].aLevelZoneKeyNum2 = ::atol (row[99]);
				/** 2009. 12. 16 : 강화, 조합, 승급부(특) 아이템 개발 (wolf) */
				/** 2009. 12. 16 : 귀멸부 아이템 개발 (wolf) */
				tAvatarInfo[index01].aDoubleKillNumTime2 = ::atol (row[100]);
				/** 2009. 12. 16 : 귀멸부 아이템 개발 (wolf) */
				/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */

				tAvatarInfo[index01].aKillMonsterNum2 = ::atol (row[101]);
#ifdef __GOD__	
				tAvatarInfo[index01].aAddItemValue = ::atol (row[102]);
				tAvatarInfo[index01].aHighItemValue = ::atol (row[103]);
				tAvatarInfo[index01].aDropItemTime = ::atol (row[104]);
				tAvatarInfo[index01].aTribeCallValue = ::atol (row[105]);
				tAvatarInfo[index01].aUpgradeValue = ::atol (row[106]);
				tAvatarInfo[index01].aBonusItemValue = ::atol (row[107]);
				tAvatarInfo[index01].aKillOtherTribeEvent = ::atol (row[108]);
				tAvatarInfo[index01].aTeacherPointEvent = ::atol (row[109]);
				tAvatarInfo[index01].aPlayTimeEvent = ::atol (row[110]);
				tAvatarInfo[index01].aProtectForUpgrade = ::atol (row[111]);
#endif

				++iCounts;
			}
#endif
			

			if((::strstr(gMember, tAvatarInfo[index01].aName) == NULL)
					&& (RealGuild[index01])
					&& (::strlen(tAvatarInfo[index01].aGuildName) > 0)) {
					::strcpy(tAvatarInfo[index01].aGuildName, "");
					tAvatarInfo[index01].aGuildRole = 0;
					::strcpy(tAvatarInfo[index01].aCallName, "");
					CheckGuild[index01] = true;
				}
		}
		mDB01.clear_res();

		if (1 != iCounts){
			LOG_TO_FILE ("![WARNING - (6)] As a result of a number of rows is not returned by the query.\n");
			LOG_TO_FILE_1 ("\t\t\tRow counts = %d.\n", iCounts);
			*tResult = 6;
			return;
		}

		if(CheckGuild[index01]) {
			sprintf(mQUERY, 
				"UPDATE AvatarInfo "
				"SET aGuildName = '', aGuildRole = 0, aCallName = '' "
				"WHERE aName = '%s' AND aWorldNumber = %d "
				, tAvatarInfo[index01].aName, mSERVER_INFO.mWorldNumber);
			if( !RECONNECT_TO_DATABASE01() ) {
				*tResult = 7;
				return;
			}

			if(mDB01.exec_query(mQUERY, CMySQL::CLEAR) != 0)
			{
				LOG_TO_FILE_2("[%s] Query execute false : %s! \n" , __FUNCTION__ , mQUERY);
				*tResult = 8;
				return;
			}
			LOG_TO_FILE_2("![%s] - Guild bug fix(%s)\n", __FUNCTION__, mQUERY);
		}

		if( strcmp( uIDForCheck, uID ) != 0 )
		{
			LOG_TO_FILE ("![WARNING - (10)] Your account is different.\n");
			*tResult = 10;
			return;
		}
		tAvatarInfo[index01].aPlayTime2 = 0;
		for( index02 = 0 ; index02 < MAX_EQUIP_SLOT_NUM ; index02++ )
		{
			CopyMemory( &tempString01[0], &aEquip[( index02 * 25 + 0 )], 5 );
			tempString01[5] = '\0';
			tAvatarInfo[index01].aEquip[index02][0] = atoi( tempString01 );
			CopyMemory( &tempString01[0], &aEquip[( index02 * 25 + 5 )], 3 );
			tempString01[3] = '\0';
			tAvatarInfo[index01].aEquip[index02][1] = atoi( tempString01 );
			CopyMemory( &tempString01[0], &aEquip[( index02 * 25 + 8 )], 9 );
			tempString01[9] = '\0';
			tAvatarInfo[index01].aEquip[index02][2] = atoi( tempString01 );
			CopyMemory( &tempString01[0], &aEquip[( index02 * 25 + 17 )], 8 );
			tempString01[8] = '\0';
			tAvatarInfo[index01].aEquip[index02][3] = atoi( tempString01 );
		}
		for( index02 = 0 ; index02 < ( MAX_INVENTORY_SLOT_NUM * 2 ) ; index02++ )
		{
			CopyMemory( &tempString01[0], &aInventory[( index02 * 27 + 0 )], 5 );
			tempString01[5] = '\0';
			tAvatarInfo[index01].aInventory[( index02 / MAX_INVENTORY_SLOT_NUM )][( index02 % MAX_INVENTORY_SLOT_NUM )][0] = atoi( tempString01 );
			CopyMemory( &tempString01[0], &aInventory[( index02 * 27 + 5 )], 1 );
			tempString01[1] = '\0';
			tAvatarInfo[index01].aInventory[( index02 / MAX_INVENTORY_SLOT_NUM )][( index02 % MAX_INVENTORY_SLOT_NUM )][1] = atoi( tempString01 );
			CopyMemory( &tempString01[0], &aInventory[( index02 * 27 + 6 )], 1 );
			tempString01[1] = '\0';
			tAvatarInfo[index01].aInventory[( index02 / MAX_INVENTORY_SLOT_NUM )][( index02 % MAX_INVENTORY_SLOT_NUM )][2] = atoi( tempString01 );
			CopyMemory( &tempString01[0], &aInventory[( index02 * 27 + 7 )], 3 );
			tempString01[3] = '\0';
			tAvatarInfo[index01].aInventory[( index02 / MAX_INVENTORY_SLOT_NUM )][( index02 % MAX_INVENTORY_SLOT_NUM )][3] = atoi( tempString01 );
			CopyMemory( &tempString01[0], &aInventory[( index02 * 27 + 10 )], 9 );
			tempString01[9] = '\0';
			tAvatarInfo[index01].aInventory[( index02 / MAX_INVENTORY_SLOT_NUM )][( index02 % MAX_INVENTORY_SLOT_NUM )][4] = atoi( tempString01 );
			CopyMemory( &tempString01[0], &aInventory[( index02 * 27 + 19 )], 8 );
			tempString01[8] = '\0';
			tAvatarInfo[index01].aInventory[( index02 / MAX_INVENTORY_SLOT_NUM )][( index02 % MAX_INVENTORY_SLOT_NUM )][5] = atoi( tempString01 );
			if(tAvatarInfo[index01].aInventory[( index02 / MAX_INVENTORY_SLOT_NUM )][( index02 % MAX_INVENTORY_SLOT_NUM )][0] == 0) {
				tAvatarInfo[index01].aInventory[( index02 / MAX_INVENTORY_SLOT_NUM )][( index02 % MAX_INVENTORY_SLOT_NUM )][1] = 0;
				tAvatarInfo[index01].aInventory[( index02 / MAX_INVENTORY_SLOT_NUM )][( index02 % MAX_INVENTORY_SLOT_NUM )][2] = 0;
				tAvatarInfo[index01].aInventory[( index02 / MAX_INVENTORY_SLOT_NUM )][( index02 % MAX_INVENTORY_SLOT_NUM )][3] = 0;
				tAvatarInfo[index01].aInventory[( index02 / MAX_INVENTORY_SLOT_NUM )][( index02 % MAX_INVENTORY_SLOT_NUM )][4] = 0;
				tAvatarInfo[index01].aInventory[( index02 / MAX_INVENTORY_SLOT_NUM )][( index02 % MAX_INVENTORY_SLOT_NUM )][5] = 0;
			}
#if 0
			LOG_TO_FILE_4("%d\n%d\n%d\n%d\n"
					, tAvatarInfo[index01].aInventory[( index02 / MAX_INVENTORY_SLOT_NUM )][( index02 % MAX_INVENTORY_SLOT_NUM )][0] 
					, tAvatarInfo[index01].aInventory[( index02 / MAX_INVENTORY_SLOT_NUM )][( index02 % MAX_INVENTORY_SLOT_NUM )][1] 
					, tAvatarInfo[index01].aInventory[( index02 / MAX_INVENTORY_SLOT_NUM )][( index02 % MAX_INVENTORY_SLOT_NUM )][2] 
					, tAvatarInfo[index01].aInventory[( index02 / MAX_INVENTORY_SLOT_NUM )][( index02 % MAX_INVENTORY_SLOT_NUM )][3]);
			LOG_TO_FILE_2("%d\n%d\n\n"
					, tAvatarInfo[index01].aInventory[( index02 / MAX_INVENTORY_SLOT_NUM )][( index02 % MAX_INVENTORY_SLOT_NUM )][4] 
					, tAvatarInfo[index01].aInventory[( index02 / MAX_INVENTORY_SLOT_NUM )][( index02 % MAX_INVENTORY_SLOT_NUM )][5]);
#endif
		}
		for( index02 = 0 ; index02 < MAX_TRADE_SLOT_NUM ; index02++ )
		{
			CopyMemory( &tempString01[0], &aTrade[( index02 * 25 + 0 )], 5 );
			tempString01[5] = '\0';
			tAvatarInfo[index01].aTrade[index02][0] = atoi( tempString01 );
			CopyMemory( &tempString01[0], &aTrade[( index02 * 25 + 5 )], 3 );
			tempString01[3] = '\0';
			tAvatarInfo[index01].aTrade[index02][1] = atoi( tempString01 );
			CopyMemory( &tempString01[0], &aTrade[( index02 * 25 + 8 )], 9 );
			tempString01[9] = '\0';
			tAvatarInfo[index01].aTrade[index02][2] = atoi( tempString01 );
			CopyMemory( &tempString01[0], &aTrade[( index02 * 25 + 17 )], 8 );
			tempString01[8] = '\0';
			tAvatarInfo[index01].aTrade[index02][3] = atoi( tempString01 );
		}
		for( index02 = 0 ; index02 < ( MAX_STORE_ITEM_SLOT_NUM * 2 ) ; index02++ )
		{
			CopyMemory( &tempString01[0], &aStoreItem[( index02 * 25 + 0 )], 5 );
			tempString01[5] = '\0';
			tAvatarInfo[index01].aStoreItem[( index02 / MAX_STORE_ITEM_SLOT_NUM )][( index02 % MAX_STORE_ITEM_SLOT_NUM )][0] = atoi( tempString01 );
			CopyMemory( &tempString01[0], &aStoreItem[( index02 * 25 + 5 )], 3 );
			tempString01[3] = '\0';
			tAvatarInfo[index01].aStoreItem[( index02 / MAX_STORE_ITEM_SLOT_NUM )][( index02 % MAX_STORE_ITEM_SLOT_NUM )][1] = atoi( tempString01 );
			CopyMemory( &tempString01[0], &aStoreItem[( index02 * 25 + 8 )], 9 );
			tempString01[9] = '\0';
			tAvatarInfo[index01].aStoreItem[( index02 / MAX_STORE_ITEM_SLOT_NUM )][( index02 % MAX_STORE_ITEM_SLOT_NUM )][2] = atoi( tempString01 );
			CopyMemory( &tempString01[0], &aStoreItem[( index02 * 25 + 17 )], 8 );
			tempString01[8] = '\0';
			tAvatarInfo[index01].aStoreItem[( index02 / MAX_STORE_ITEM_SLOT_NUM )][( index02 % MAX_STORE_ITEM_SLOT_NUM )][3] = atoi( tempString01 );
		}
		for( index02 = 0 ; index02 < MAX_SKILL_SLOT_NUM ; index02++ )
		{
			CopyMemory( &tempString01[0], &aSkill[( index02 * 6 + 0 )], 3 );
			tempString01[3] = '\0';
			tAvatarInfo[index01].aSkill[index02][0] = atoi( tempString01 );
			CopyMemory( &tempString01[0], &aSkill[( index02 * 6 + 3 )], 3 );
			tempString01[3] = '\0';
			tAvatarInfo[index01].aSkill[index02][1] = atoi( tempString01 );
		}
		for( index02 = 0 ; index02 < ( MAX_HOT_KEY_NUM * 3 ) ; index02++ )
		{
			CopyMemory( &tempString01[0], &aHotKey[( index02 * 10 + 0 )], 5 );
			tempString01[5] = '\0';
			tAvatarInfo[index01].aHotKey[( index02 / MAX_HOT_KEY_NUM )][( index02 % MAX_HOT_KEY_NUM )][0] = atoi( tempString01 );
			CopyMemory( &tempString01[0], &aHotKey[( index02 * 10 + 5 )], 3 );
			tempString01[3] = '\0';
			tAvatarInfo[index01].aHotKey[( index02 / MAX_HOT_KEY_NUM )][( index02 % MAX_HOT_KEY_NUM )][1] = atoi( tempString01 );
			CopyMemory( &tempString01[0], &aHotKey[( index02 * 10 + 8 )], 2 );
			tempString01[2] = '\0';
			tAvatarInfo[index01].aHotKey[( index02 / MAX_HOT_KEY_NUM )][( index02 % MAX_HOT_KEY_NUM )][2] = atoi( tempString01 );
		}
		CopyMemory( &tempString01[0], &aQuestInfo[0], 5 );
		tempString01[5] = '\0';
		tAvatarInfo[index01].aQuestInfo[0] = atoi( tempString01 );
		CopyMemory( &tempString01[0], &aQuestInfo[5], 5 );
		tempString01[5] = '\0';
		tAvatarInfo[index01].aQuestInfo[1] = atoi( tempString01 );
		CopyMemory( &tempString01[0], &aQuestInfo[10], 5 );
		tempString01[5] = '\0';
		tAvatarInfo[index01].aQuestInfo[2] = atoi( tempString01 );
		CopyMemory( &tempString01[0], &aQuestInfo[15], 5 );
		tempString01[5] = '\0';
		tAvatarInfo[index01].aQuestInfo[3] = atoi( tempString01 );
		CopyMemory( &tempString01[0], &aQuestInfo[20], 5 );
		tempString01[5] = '\0';
		tAvatarInfo[index01].aQuestInfo[4] = atoi( tempString01 );
		for( index02 = 0 ; index02 < MAX_FRIEND_NUM ; index02++ )
		{
			CopyMemory( &tAvatarInfo[index01].aFriend[index02][0], &aFriend[( index02 * 12 + 0 )], 12 );
			for( index03 = 0 ; index03 < 12 ; index03++ )
			{
				if (::IsDBCSLeadByte (tAvatarInfo[index01].aFriend[index02][index03]) == TRUE) {
					if (index03 != 11) index03++;
					continue;
				}

				if( tAvatarInfo[index01].aFriend[index02][index03] == '@' )
				{
					break;
				}
			}
			tAvatarInfo[index01].aFriend[index02][index03] = '\0';
		}
		tAvatarInfo[index01].uSaveMoney = *tSaveMoney;
		//LOG_TO_FILE_1("%s---", uID);
		for( index02 = 0 ; index02 < MAX_SAVE_ITEM_SLOT_NUM ; index02++ )
		{
			tAvatarInfo[index01].uSaveItem[index02][0] = tSaveItem[index02][0];
			tAvatarInfo[index01].uSaveItem[index02][1] = tSaveItem[index02][1];
			tAvatarInfo[index01].uSaveItem[index02][2] = tSaveItem[index02][2];
			tAvatarInfo[index01].uSaveItem[index02][3] = tSaveItem[index02][3];
			//LOG_TO_FILE_4("------> %d %d %d %d\n", tSaveItem[index02][0], tSaveItem[index02][1], tSaveItem[index02][2], tSaveItem[index02][3]);
		}
		
		//-----------------------//
		//무신경_누적_경험치_처리//
		//-----------------------//
#ifdef __GOD__
		if( ( tAvatarInfo[index01].aLevel2 < 1 ) && ( tAvatarInfo[index01].aGeneralExperience2 > 0 ) )
		{
			tAvatarInfo[index01].aGeneralExperience2 = (int) ( (float) tAvatarInfo[index01].aGeneralExperience2 / 2.3f );
			tAvatarInfo[index01].aSkillPoint += 100;
			tAvatarInfo[index01].aLevel2 = 1;
			tMaxGeneralExperienceRange = 962105896.0f;
			tPresentGeneralExperienceRatio = 0;
			tNextGeneralExperienceRatio = (int) ( (float) tAvatarInfo[index01].aGeneralExperience2 * 100.0f / tMaxGeneralExperienceRange );
			tAvatarInfo[index01].aStateBonusPoint += ( tNextGeneralExperienceRatio - tPresentGeneralExperienceRatio );
			tAvatarInfo[index01].aStateBonusPoint += ( tAvatarInfo[index01].aVitality + tAvatarInfo[index01].aStrength + tAvatarInfo[index01].aKi + tAvatarInfo[index01].aWisdom - 4 );
			tAvatarInfo[index01].aVitality = 1;
			tAvatarInfo[index01].aStrength = 1;
			tAvatarInfo[index01].aKi = 1;
			tAvatarInfo[index01].aWisdom = 1;
			tAvatarInfo[index01].aLogoutInfo[4] = 1;
			tAvatarInfo[index01].aLogoutInfo[5] = 0;
		}
#endif
		//-----------------------//
		//-----------------------//
		//-----------------------//
	}
	*tResult = 0;
}
//DB_PROCESS_03
BOOL MyDB::DB_PROCESS_03( char tID[MAX_USER_ID_LENGTH], char tMousePassword[MAX_MOUSE_PASSWORD_LENGTH] )
{
	int iAffected;

	sprintf( mQUERY, "update %s set uMousePassword='%s' where uID='%s'", mSERVER_INFO.mDB_1_Table01, tMousePassword, tID );
	if( !RECONNECT_TO_DATABASE02() )
	{
		return FALSE;
	}

	iAffected = 0;
	if (mDB02.exec_query (mQUERY, CMySQL::CLEAR) != 0) 
		return FALSE;

	return TRUE;
}
//DB_PROCESS_04
BOOL MyDB::DB_PROCESS_04( int tSecretCardNumber, int tSecretCardIndex, char tSecretCardContent[5] )
{
	int iAffected;
	int iCounts;
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;

	switch( tSecretCardIndex )
	{
	case  0 : sprintf( mQUERY, "select cNumber01 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case  1 : sprintf( mQUERY, "select cNumber02 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case  2 : sprintf( mQUERY, "select cNumber03 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case  3 : sprintf( mQUERY, "select cNumber04 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case  4 : sprintf( mQUERY, "select cNumber05 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case  5 : sprintf( mQUERY, "select cNumber06 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case  6 : sprintf( mQUERY, "select cNumber07 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case  7 : sprintf( mQUERY, "select cNumber08 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case  8 : sprintf( mQUERY, "select cNumber09 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case  9 : sprintf( mQUERY, "select cNumber10 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 10 : sprintf( mQUERY, "select cNumber11 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 11 : sprintf( mQUERY, "select cNumber12 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 12 : sprintf( mQUERY, "select cNumber13 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 13 : sprintf( mQUERY, "select cNumber14 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 14 : sprintf( mQUERY, "select cNumber15 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 15 : sprintf( mQUERY, "select cNumber16 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 16 : sprintf( mQUERY, "select cNumber17 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 17 : sprintf( mQUERY, "select cNumber18 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 18 : sprintf( mQUERY, "select cNumber19 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 19 : sprintf( mQUERY, "select cNumber20 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 20 : sprintf( mQUERY, "select cNumber21 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 21 : sprintf( mQUERY, "select cNumber22 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 22 : sprintf( mQUERY, "select cNumber23 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 23 : sprintf( mQUERY, "select cNumber24 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 24 : sprintf( mQUERY, "select cNumber25 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 25 : sprintf( mQUERY, "select cNumber26 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 26 : sprintf( mQUERY, "select cNumber27 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 27 : sprintf( mQUERY, "select cNumber28 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 28 : sprintf( mQUERY, "select cNumber29 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 29 : sprintf( mQUERY, "select cNumber30 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 30 : sprintf( mQUERY, "select cNumber31 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 31 : sprintf( mQUERY, "select cNumber32 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 32 : sprintf( mQUERY, "select cNumber33 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 33 : sprintf( mQUERY, "select cNumber34 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 34 : sprintf( mQUERY, "select cNumber35 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 35 : sprintf( mQUERY, "select cNumber36 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 36 : sprintf( mQUERY, "select cNumber37 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 37 : sprintf( mQUERY, "select cNumber38 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 38 : sprintf( mQUERY, "select cNumber39 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 39 : sprintf( mQUERY, "select cNumber40 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 40 : sprintf( mQUERY, "select cNumber41 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 41 : sprintf( mQUERY, "select cNumber42 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 42 : sprintf( mQUERY, "select cNumber43 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 43 : sprintf( mQUERY, "select cNumber44 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 44 : sprintf( mQUERY, "select cNumber45 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 45 : sprintf( mQUERY, "select cNumber46 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 46 : sprintf( mQUERY, "select cNumber47 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 47 : sprintf( mQUERY, "select cNumber48 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 48 : sprintf( mQUERY, "select cNumber49 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	case 49 : sprintf( mQUERY, "select cNumber50 from %s where cID=%d", mSERVER_INFO.mDB_1_Table04, tSecretCardNumber ); break;
	default :
		return FALSE;
	}
	if( !RECONNECT_TO_DATABASE02() )
	{
		return FALSE;
	}
	iAffected = 0;
    if ((iAffected = mDB02.exec_query (mQUERY)) < 0) 
        return FALSE;
	iCounts = 0;
#if 0
    if ((iCounts = mDB01.get_num_rows ()) != 1)
        return FALSE;

    ::strncpy (tSecretCardContent, mDB01.get_value (0, 0), 5); 
    mDB01.clear_res ();
#endif
	res = mDB02.get_res ();
	if (res == NULL) {
	    LOG_TO_FILE_1 ("!Query failed - (%s).\n", mQUERY);
        LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB02.get_error_msg ().c_str ());
		return FALSE;
	}
	while (row = mysql_fetch_row (res)){
		::strncpy (tSecretCardContent, row[0], 5);
		++iCounts;
	}
	mDB02.clear_res ();
	if (1 != iCounts){
		return FALSE;
	}
			
	return TRUE;
}
//DB_PROCESS_05
void MyDB::DB_PROCESS_05( int tUserIndex, int tAvatarPost, AVATAR_INFO *tAvatarInfo, int *tResult )
{
	int iAffected;

	sprintf( mQUERY, "insert into %s values ( now(),'%s' )", mSERVER_INFO.mDB_1_Table02, tAvatarInfo->aName );
	if( !RECONNECT_TO_DATABASE02() )
	{
		*tResult = 1;
		LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB02.get_error_msg ().c_str ());
		LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mQUERY);
		return;
	}
	iAffected = 0;
	if (mDB02.exec_query (mQUERY, CMySQL::CLEAR) != 0) {
		*tResult = 2;
		LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB02.get_error_msg ().c_str ());
		LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mQUERY);
		return ;
	}
	MakeQueryForInsert( 1, &mUSER[tUserIndex].uID[0], tAvatarInfo );
	if( !RECONNECT_TO_DATABASE01() )
	{
		sprintf( mQUERY, "delete from %s where aName='%s'", mSERVER_INFO.mDB_1_Table02, tAvatarInfo->aName );
		RECONNECT_TO_DATABASE02();
		iAffected = 0;
		if (mDB02.exec_query (mQUERY, CMySQL::CLEAR) != 0) {
			*tResult = 3;
			LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB02.get_error_msg ().c_str ());
			LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mQUERY);
			return ;
		}
		*tResult = 1;
		return;
	}
	iAffected = 0;
	if (mDB01.exec_query (mQUERY, CMySQL::CLEAR) != 0)
	{
		LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB01.get_error_msg ().c_str ());
		LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mQUERY);
		sprintf( mQUERY, "delete from %s where aName='%s'", mSERVER_INFO.mDB_1_Table02, tAvatarInfo->aName );
		RECONNECT_TO_DATABASE02();
		iAffected = 0;
		if (mDB02.exec_query (mQUERY, CMySQL::CLEAR) != 0) {
			*tResult = 4;
			LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB02.get_error_msg ().c_str ());
			LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mQUERY);
			return ;
		}
		*tResult = 2;
		return;
	}
	switch( tAvatarPost )
	{
		case 0 :
			sprintf( mQUERY, "update %s set aName01='%s' where uID='%s' and aWorldNumber = %d", mSERVER_INFO.mDB_2_Table02, tAvatarInfo->aName, mUSER[tUserIndex].uID, mSERVER_INFO.mWorldNumber);
			break;
		case 1 :
			sprintf( mQUERY, "update %s set aName02='%s' where uID='%s' and aWorldNumber = %d", mSERVER_INFO.mDB_2_Table02, tAvatarInfo->aName, mUSER[tUserIndex].uID, mSERVER_INFO.mWorldNumber);
			break;
		case 2 :
			sprintf( mQUERY, "update %s set aName03='%s' where uID='%s' and aWorldNumber = %d", mSERVER_INFO.mDB_2_Table02, tAvatarInfo->aName, mUSER[tUserIndex].uID, mSERVER_INFO.mWorldNumber);
			break;
		default :
			sprintf( mQUERY, "delete from %s where aName='%s'", mSERVER_INFO.mDB_1_Table02, tAvatarInfo->aName );
			RECONNECT_TO_DATABASE02();
			iAffected = 0;
			if (mDB02.exec_query (mQUERY, CMySQL::CLEAR) != 0) {
				*tResult = 5;
				LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB02.get_error_msg ().c_str ());
				LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mQUERY);
				return ;
			}
			*tResult = 1;
			return;
	}
	if( !RECONNECT_TO_DATABASE01() )
	{
		sprintf( mQUERY, "delete from %s where aName='%s' and aWorldNumber = %d", mSERVER_INFO.mDB_2_Table03, tAvatarInfo->aName, mSERVER_INFO.mWorldNumber);
		RECONNECT_TO_DATABASE02();
		iAffected = 0;
		if (mDB01.exec_query (mQUERY, CMySQL::CLEAR) != 0) {
			*tResult = 6;
			LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB01.get_error_msg ().c_str ());
			LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mQUERY);
			return ;
		}
		sprintf( mQUERY, "delete from %s where aName='%s'", mSERVER_INFO.mDB_1_Table02, tAvatarInfo->aName );
		RECONNECT_TO_DATABASE02();
		iAffected = 0;
		if (mDB02.exec_query (mQUERY, CMySQL::CLEAR) != 0) {
			*tResult = 7;
			LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB02.get_error_msg ().c_str ());
			LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mQUERY);
			return ;
		}
		*tResult = 1;
		return;
	}
	iAffected = 0;
	if (mDB01.exec_query (mQUERY, CMySQL::CLEAR) != 0)
	{
		sprintf( mQUERY, "delete from %s where aName='%s' and aWorldNumber = %d", mSERVER_INFO.mDB_2_Table03, tAvatarInfo->aName, mSERVER_INFO.mWorldNumber);
		RECONNECT_TO_DATABASE01();
		iAffected = 0;
		if (mDB01.exec_query (mQUERY, CMySQL::CLEAR) != 0) {
			*tResult = 8;
			LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB01.get_error_msg ().c_str ());
			LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mQUERY);
			return ;
		}
		sprintf( mQUERY, "delete from %s where aName='%s'", mSERVER_INFO.mDB_1_Table02, tAvatarInfo->aName );
		RECONNECT_TO_DATABASE02();
		iAffected = 0;
		if (mDB02.exec_query (mQUERY, CMySQL::CLEAR) != 0) {
			*tResult = 9;
			LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB02.get_error_msg ().c_str ());
			LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mQUERY);
			return ;
		}
		*tResult = 1;
		return;
	}

	RECONNECT_TO_DATABASE01();
	switch(tAvatarInfo->aTribe) {
		case 0 :
			sprintf(mQUERY, 
					"UPDATE TribeControl "
					"SET aTribeType0Count = aTribeType0Count + 1 "
					"WHERE aWorldNumber = %d", mSERVER_INFO.mWorldNumber);
			break;
		case 1 :
			sprintf(mQUERY, 
					"UPDATE TribeControl "
					"SET aTribeType1Count = aTribeType1Count + 1 "
					"WHERE aWorldNumber = %d", mSERVER_INFO.mWorldNumber);
			break;
		case 2 :
			sprintf(mQUERY, 
					"UPDATE TribeControl "
					"SET aTribeType2Count = aTribeType2Count + 1 "
					"WHERE aWorldNumber = %d", mSERVER_INFO.mWorldNumber);
			break;
		default :
			*tResult = 10;
			return;
	}

	if (mDB01.exec_query (mQUERY, CMySQL::CLEAR) != 0) {
		*tResult = 11;
		LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB01.get_error_msg ().c_str ());
		LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mQUERY);
		return ;
	}

	*tResult = 0;
}
//DB_PROCESS_06
BOOL MyDB::DB_PROCESS_06( int tUserIndex, int tAvatarPost )
{
	int iAffected;
	int TribeCount[3];
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;

	switch( tAvatarPost )
	{
	case 0 :
		sprintf( mQUERY, "update %s set aName01='' where uID='%s' and aWorldNumber = %d", mSERVER_INFO.mDB_2_Table02, mUSER[tUserIndex].uID, mSERVER_INFO.mWorldNumber);
		break;
	case 1 :
		sprintf( mQUERY, "update %s set aName02='' where uID='%s' and aWorldNumber = %d", mSERVER_INFO.mDB_2_Table02, mUSER[tUserIndex].uID, mSERVER_INFO.mWorldNumber);
		break;
	case 2 :
		sprintf( mQUERY, "update %s set aName03='' where uID='%s' and aWorldNumber = %d", mSERVER_INFO.mDB_2_Table02, mUSER[tUserIndex].uID, mSERVER_INFO.mWorldNumber);
		break;
	default :
		return FALSE;
	}
	if( !RECONNECT_TO_DATABASE01() )
	{
        LOG_TO_FILE_1 ("![WARNING] Unable to connect to the database[%s].\n",
                       mSERVER_INFO.mDB_2_Table02);
		return FALSE;
	}
	iAffected = 0;
    if (mDB01.exec_query (mQUERY, CMySQL::CLEAR) != 0) {
        LOG_TO_FILE_1 ("![WARNING] The query execution failed in %s.\n", __FUNCTION__);
        LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB01.get_error_msg ().c_str ());
		//SVR_TRACE();
		//LOG_TO_FILE_1("%s\n", mQUERY);
        return FALSE;
    }
	MakeQueryForInsert( 2, &mUSER[tUserIndex].uID[0], &mUSER[tUserIndex].mAvatarInfo[tAvatarPost] );
	RECONNECT_TO_DATABASE01();
	iAffected = 0;
    if (mDB01.exec_query (mQUERY, CMySQL::CLEAR) != 0) {
        LOG_TO_FILE_1 ("![WARNING] The query execution failed in %s.\n", __FUNCTION__);
        LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB01.get_error_msg ().c_str ());
		//SVR_TRACE();
		//LOG_TO_FILE_1("%s\n", mQUERY);
        return FALSE;
    }
	sprintf( mQUERY, "delete from %s where aName='%s' and aWorldNumber = %d", mSERVER_INFO.mDB_2_Table03, mUSER[tUserIndex].mAvatarInfo[tAvatarPost].aName, mSERVER_INFO.mWorldNumber);
	RECONNECT_TO_DATABASE01();
	iAffected = 0;
    if (mDB01.exec_query (mQUERY, CMySQL::CLEAR) != 0) {
        LOG_TO_FILE_1 ("![WARNING] The query execution failed in %s.\n", __FUNCTION__);
        LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB01.get_error_msg ().c_str ());
		//SVR_TRACE();
		//LOG_TO_FILE_1("%s\n", mQUERY);
        return FALSE;
    }
	sprintf( mQUERY, "delete from %s where aName='%s'", mSERVER_INFO.mDB_1_Table02, mUSER[tUserIndex].mAvatarInfo[tAvatarPost].aName );
	RECONNECT_TO_DATABASE02();
	iAffected = 0;
    if (mDB02.exec_query (mQUERY, CMySQL::CLEAR) != 0) {
        LOG_TO_FILE_1 ("![WARNING] The query execution failed in %s.\n", __FUNCTION__);
        LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB02.get_error_msg ().c_str ());
		//SVR_TRACE();
		//LOG_TO_FILE_1("%s\n", mQUERY);
        return FALSE;
    }
	sprintf( mQUERY, "delete from %s where aName='%s' and aWorldNumber = %d", mSERVER_INFO.mDB_1_Table05, mUSER[tUserIndex].mAvatarInfo[tAvatarPost].aName, mSERVER_INFO.mWorldNumber);
	RECONNECT_TO_DATABASE01();
	iAffected = 0;
    if (mDB01.exec_query (mQUERY, CMySQL::CLEAR) != 0) {
        LOG_TO_FILE_1 ("![WARNING] The query execution failed in %s.\n", __FUNCTION__);
        LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB01.get_error_msg ().c_str ());
		//SVR_TRACE();
		//LOG_TO_FILE_1("%s\n", mQUERY);
        return FALSE;
    }
	
	sprintf( mQUERY, 
			"SELECT aTribeType0Count, aTribeType1Count, aTribeType2Count "
			"FROM TribeControl "
			"WHERE aWorldNumber=%d", mSERVER_INFO.mWorldNumber);

	if(!RECONNECT_TO_DATABASE01()) {
		return false;
	}

    if((mDB01.exec_query (mQUERY)) != 0) {
		return false;
	}

	res = mDB01.get_res();
	if (res == NULL) {
	    LOG_TO_FILE_1 ("!Query failed - (%s).\n", mQUERY);
        LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB01.get_error_msg ().c_str ());
		return false;
	}
	while (row = mysql_fetch_row(res)){
		TribeCount[0] = ::atoi(row[0]);
		TribeCount[1] = ::atoi(row[1]);
		TribeCount[2] = ::atoi(row[2]);
	}
    mDB01.clear_res();

	if(TribeCount[mUSER[tUserIndex].mAvatarInfo[tAvatarPost].aTribe] == 0) {
		return true;
	}

	switch(mUSER[tUserIndex].mAvatarInfo[tAvatarPost].aTribe) {
		case 0 :
			sprintf(mQUERY, 
					"UPDATE TribeControl "
					"SET aTribeType0Count = aTribeType0Count - 1 "
					"WHERE aWorldNumber = %d", mSERVER_INFO.mWorldNumber);
			break;
		case 1 :
			sprintf(mQUERY, 
					"UPDATE TribeControl "
					"SET aTribeType1Count = aTribeType1Count - 1 "
					"WHERE aWorldNumber = %d", mSERVER_INFO.mWorldNumber);
			break;
		case 2 :
			sprintf(mQUERY, 
					"UPDATE TribeControl "
					"SET aTribeType2Count = aTribeType2Count - 1 "
					"WHERE aWorldNumber = %d", mSERVER_INFO.mWorldNumber);
			break;
		default :
			return FALSE;
	}

	RECONNECT_TO_DATABASE01();
	if (mDB01.exec_query (mQUERY, CMySQL::CLEAR) != 0) {
		LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB01.get_error_msg ().c_str ());
		LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mQUERY);
		return TRUE;
	}

	return TRUE;
}
//DB_PROCESS_07
void MyDB::DB_PROCESS_07( char tID[MAX_USER_ID_LENGTH], int tAvatarPost, char tAvatarName[MAX_AVATAR_NAME_LENGTH], char tChangeAvatarName[MAX_AVATAR_NAME_LENGTH], int *tResult )
{
	int iAffected;

	sprintf( mQUERY, "update %s set aName='%s' where aName='%s'", mSERVER_INFO.mDB_1_Table02, tChangeAvatarName, tAvatarName );
	if( !RECONNECT_TO_DATABASE02() )
	{
		*tResult = 1;
		return;
	}
	iAffected = 0;
	if (mDB02.exec_query (mQUERY, CMySQL::CLEAR) != 0) {
        *tResult = 2;
        return ;
    }
	sprintf( mQUERY, "update %s set aName='%s' where aName='%s' and aWorldNumber = %d", mSERVER_INFO.mDB_2_Table03, tChangeAvatarName, tAvatarName, mSERVER_INFO.mWorldNumber);
	if( !RECONNECT_TO_DATABASE01() )
	{
		sprintf( mQUERY, "update %s set aName='%s' where aName='%s'", mSERVER_INFO.mDB_1_Table02, tAvatarName, tChangeAvatarName );
		RECONNECT_TO_DATABASE02();
		iAffected = 0;
        if (mDB02.exec_query (mQUERY, CMySQL::CLEAR) != 0) {
            *tResult = 3;
            return ;
        }
		*tResult = 1;
		return;
	}
	iAffected = 0;
    if (mDB01.exec_query (mQUERY, CMySQL::CLEAR) != 0) {
		sprintf( mQUERY, "update %s set aName='%s' where aName='%s'", mSERVER_INFO.mDB_1_Table02, tAvatarName, tChangeAvatarName );
		RECONNECT_TO_DATABASE02();
		iAffected = 0;
        if (mDB02.exec_query (mQUERY, CMySQL::CLEAR) != 0) {
            *tResult = 4;
            return ;
        }
		*tResult = 1;
		return;
	}
	switch( tAvatarPost )
	{
	case 0 :
		sprintf( mQUERY, "update %s set aName01='%s' where uID='%s' and aWorldNumber = %d", mSERVER_INFO.mDB_2_Table02, tChangeAvatarName, tID, mSERVER_INFO.mWorldNumber);
		break;
	case 1 :
		sprintf( mQUERY, "update %s set aName02='%s' where uID='%s' and aWorldNumber = %d", mSERVER_INFO.mDB_2_Table02, tChangeAvatarName, tID, mSERVER_INFO.mWorldNumber);
		break;
	case 2 :
		sprintf( mQUERY, "update %s set aName03='%s' where uID='%s' and aWorldNumber = %d", mSERVER_INFO.mDB_2_Table02, tChangeAvatarName, tID, mSERVER_INFO.mWorldNumber);
		break;
	default :
		sprintf( mQUERY, "update %s set aName='%s' where aName='%s' and aWorldNumber = %d", mSERVER_INFO.mDB_2_Table03, tAvatarName, tChangeAvatarName, mSERVER_INFO.mWorldNumber);
		RECONNECT_TO_DATABASE01();
		iAffected = 0;
        if (mDB01.exec_query (mQUERY, CMySQL::CLEAR) != 0) {
            *tResult = 5;
            return ;
        }
		sprintf( mQUERY, "update %s set aName='%s' where aName='%s'", mSERVER_INFO.mDB_1_Table02, tAvatarName, tChangeAvatarName );
		RECONNECT_TO_DATABASE02();
		iAffected = 0;
        if (mDB02.exec_query (mQUERY, CMySQL::CLEAR) != 0) {
            *tResult = 6;
            return ;
        }
		*tResult = 1;
		return;
	}
	if( !RECONNECT_TO_DATABASE01() )
	{
		sprintf( mQUERY, "update %s set aName='%s' where aName='%s' and aWorldNumber = %d", mSERVER_INFO.mDB_2_Table03, tAvatarName, tChangeAvatarName, mSERVER_INFO.mWorldNumber);
		RECONNECT_TO_DATABASE01();
		iAffected = 0;
        if (mDB01.exec_query (mQUERY, CMySQL::CLEAR) != 0) {
            *tResult = 7;
            return ;
        }
		sprintf( mQUERY, "update %s set aName='%s' where aName='%s'", mSERVER_INFO.mDB_1_Table02, tAvatarName, tChangeAvatarName );
		RECONNECT_TO_DATABASE02();
		iAffected = 0;
        if (mDB02.exec_query (mQUERY, CMySQL::CLEAR) != 0) {
            *tResult = 8;
            return ;
        }
		*tResult = 1;
		return;
	}
	iAffected = 0;
    if (mDB01.exec_query (mQUERY, CMySQL::CLEAR) != 0) {
		sprintf( mQUERY, "update %s set aName='%s' where aName='%s' and aWorldNumber = %d", mSERVER_INFO.mDB_2_Table03, tAvatarName, tChangeAvatarName, mSERVER_INFO.mWorldNumber);
		RECONNECT_TO_DATABASE01();
		iAffected = 0;
        if (mDB01.exec_query (mQUERY, CMySQL::CLEAR) != 0) {
            *tResult = 9;
            return ;
        }
		sprintf( mQUERY, "update %s set aName='%s' where aName='%s'", mSERVER_INFO.mDB_1_Table02, tAvatarName, tChangeAvatarName );
		RECONNECT_TO_DATABASE02();
		iAffected = 0;
        if (mDB02.exec_query (mQUERY, CMySQL::CLEAR) != 0) {
            *tResult = 10;
            return ;
        }
		*tResult = 1;
		return;
	}
	sprintf( mQUERY, "update %s set aName='%s' where aName='%s' and aWorldNumber = %d", mSERVER_INFO.mDB_1_Table05, tChangeAvatarName, tAvatarName, mSERVER_INFO.mWorldNumber);
	if( !RECONNECT_TO_DATABASE01() )
	{
		*tResult = 1;
		return;
	}
	iAffected = 0;
	if (mDB01.exec_query (mQUERY, CMySQL::CLEAR) != 0) {
		*tResult = 11;
		return ;
	}

	*tResult = 0;
}
//DB_PROCESS_08
BOOL MyDB::DB_PROCESS_08( char tAvatarName[MAX_AVATAR_NAME_LENGTH], int tInventory[2][MAX_INVENTORY_SLOT_NUM][6] )
{
	int index01;
	int index02;
	char tempString01[1000];
	int iAffected;
	char aInventory[3457];

	aInventory[0] = '\0';
	for( index01 = 0 ; index01 < 2 ; index01++ )
	{
		for( index02 = 0 ; index02 < MAX_INVENTORY_SLOT_NUM ; index02++ )
		{
			sprintf( tempString01, "%05d%01d%01d%03d%09d%08d", ( tInventory[index01][index02][0] % 100000 ), ( tInventory[index01][index02][1] % 10 ), ( tInventory[index01][index02][2] % 10 ), ( tInventory[index01][index02][3] % 1000 ), ( tInventory[index01][index02][4] % 1000000000 ), ( tInventory[index01][index02][5] % 100000000 ) );
			strcat( aInventory, tempString01 );
		}
	}
	sprintf( mQUERY, "update %s set aInventory='%s' where aName='%s' and aWorldNumber = %d", mSERVER_INFO.mDB_2_Table03, aInventory, tAvatarName, mSERVER_INFO.mWorldNumber);
	if( !RECONNECT_TO_DATABASE01() )
	{
		return FALSE;
	}
	iAffected = 0;
    if (mDB01.exec_query (mQUERY, CMySQL::CLEAR) != 0) 
        return FALSE;

	return TRUE;
}
//DB_PROCESS_09
BOOL MyDB::DB_PROCESS_09( char tCardNumber[MAX_USER_CARD_NUMBER_LENGTH], char tID[MAX_USER_ID_LENGTH], int tGiftInfo[10] )
{
	int iAffected;
	int iCounts;
	char uID[MAX_USER_ID_LENGTH];
	long gData1;
	long gNumber1;
	long gData2;
	long gNumber2;
	long gData3;
	long gNumber3;
	long gData4;
	long gNumber4;
	long gData5;
	long gNumber5;
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;

	sprintf( mQUERY, "select uID,gData1,gNumber1,gData2,gNumber2,gData3,gNumber3,gData4,gNumber4,gData5,gNumber5 from %s where gCard='%s'", mSERVER_INFO.mDB_1_Table03, tCardNumber );
	if( !RECONNECT_TO_DATABASE02() )
	{
		return FALSE;
	}
	iAffected = 0;
    if ((iAffected = mDB02.exec_query (mQUERY)) < 0)
        return FALSE;
	iCounts = 0;
#if 0
    if ((iCounts = mDB01.get_num_rows ()) != 1)
		return FALSE;
#endif
	res = mDB02.get_res ();
	if (res == NULL) {
	    LOG_TO_FILE_1 ("!Query failed - (%s).\n", mQUERY);
        LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB02.get_error_msg ().c_str ());
		return FALSE;
	}
	while (row = mysql_fetch_row (res)){
		::strncpy (uID, row [ 0], MAX_USER_ID_LENGTH - 1);
		gData1 = ::atol (row [ 1]);
		gNumber1 = ::atol (row [ 2]);
		gData2 = ::atol (row [ 3]);
		gNumber2 = ::atol (row [ 4]);
		gData3 = ::atol (row [ 5]);
		gNumber3 = ::atol (row [ 6]);
		gData4 = ::atol (row [ 7]);
		gNumber4 = ::atol (row [ 8]);
		gData5 = ::atol (row [ 9]);
		gNumber5 = ::atol (row [ 10]);
		++iCounts;
	}
    mDB01.clear_res ();
	if (1 != iCounts){
		return FALSE;
	}
	strcpy( tID, uID );
	tGiftInfo[0] = gData1;
	tGiftInfo[1] = gNumber1;
	tGiftInfo[2] = gData2;
	tGiftInfo[3] = gNumber2;
	tGiftInfo[4] = gData3;
	tGiftInfo[5] = gNumber3;
	tGiftInfo[6] = gData4;
	tGiftInfo[7] = gNumber4;
	tGiftInfo[8] = gData5;
	tGiftInfo[9] = gNumber5;
	return TRUE;
}
//DB_PROCESS_10
BOOL MyDB::DB_PROCESS_10( char tCardNumber[MAX_USER_CARD_NUMBER_LENGTH], int tGiftInfo[10] )
{
	int iAffected;

	sprintf( mQUERY, "update %s set gData1=%d,gNumber1=%d,gData2=%d,gNumber2=%d,gData3=%d,gNumber3=%d,gData4=%d,gNumber4=%d,gData5=%d,gNumber5=%d where gCard='%s'", mSERVER_INFO.mDB_1_Table03, tGiftInfo[0], tGiftInfo[1], tGiftInfo[2], tGiftInfo[3], tGiftInfo[4], tGiftInfo[5], tGiftInfo[6], tGiftInfo[7], tGiftInfo[8], tGiftInfo[9], tCardNumber );
	if( !RECONNECT_TO_DATABASE02() )
	{
		return FALSE;
	}
	iAffected = 0;
    if (mDB02.exec_query (mQUERY, CMySQL::CLEAR) != 0)
		return FALSE;

	return TRUE;
}
//DB_PROCESS_11
BOOL MyDB::DB_PROCESS_11( char tID[MAX_USER_ID_LENGTH], int tSaveItem[MAX_SAVE_ITEM_SLOT_NUM][4] )
{
	int index01;
	char tempString01[1000];
	int iAffected;
	char uSaveItem[701];

	uSaveItem[0] = '\0';
	for( index01 = 0 ; index01 < MAX_SAVE_ITEM_SLOT_NUM ; index01++ )
	{
		sprintf( tempString01, "%05d%03d%09d%08d", ( tSaveItem[index01][0] % 100000 ), ( tSaveItem[index01][1] % 1000 ), ( tSaveItem[index01][2] % 1000000000 ), ( tSaveItem[index01][3] % 100000000 ) );
		strcat( uSaveItem, tempString01 );
	}
	sprintf( mQUERY, "update %s set uSaveItem='%s' where uID='%s' and aWorldNumber = %d", mSERVER_INFO.mDB_2_Table02, uSaveItem, tID, mSERVER_INFO.mWorldNumber);
	if( !RECONNECT_TO_DATABASE01() )
	{
		return FALSE;
	}
	iAffected = 0;
    if (mDB01.exec_query (mQUERY, CMySQL::CLEAR) != 0)
		return FALSE;

	return TRUE;
}
//DB_PROCESS_12
BOOL MyDB::DB_PROCESS_12( WORLD_INFO *tWorldInfo )
{
	int iAffected;
	int iCounts;
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;

	sprintf( mQUERY, "select mZone38WinTribe,mTribe1Symbol,mTribe2Symbol,mTribe3Symbol,mTribe4Symbol,mMonsterSymbol,mTribe1Point,mTribe2Point,mTribe3Point,mTribe4Point,mCloseTime,mCloseTribe,mPossibleAlliance01Date,mPossibleAlliance01Info,mPossibleAlliance02Date,mPossibleAlliance02Info,mPossibleAlliance03Date,mPossibleAlliance03Info,mPossibleAlliance04Date,mPossibleAlliance04Info,mAlliance0101,mAlliance0102,mAlliance0201,mAlliance0202,mTribe1VoteState,mTribe2VoteState,mTribe3VoteState,mTribe4VoteState,mClose1VoteState,mClose2VoteState,mClose3VoteState,mClose4VoteState,mTribe4QuestDate,mTribe4QuestState,mTribe4QuestName from %s WHERE aWorldNumber = %d", mSERVER_INFO.mDB_1_Table06, mSERVER_INFO.mWorldNumber);
	if( !RECONNECT_TO_DATABASE01() )
	{
		return FALSE;
	}
	iAffected = 0;
    if ((iAffected = mDB01.exec_query (mQUERY)) < 0)
		return FALSE;
	iCounts = 0;
#if 0
    if ((iCounts = mDB01.get_num_rows ()) != 1)
		return FALSE;
#endif
	res = mDB01.get_res ();
	if (res == NULL) {
	    LOG_TO_FILE_1 ("!Query failed - (%s).\n", mQUERY);
        LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB01.get_error_msg ().c_str ());
		return FALSE;
	}
	while (row = mysql_fetch_row (res)){
		tWorldInfo -> mZone038WinTribe = ::atol (row [0]);
		tWorldInfo -> mTribe1Symbol = ::atol (row [1]);
		tWorldInfo -> mTribe2Symbol = ::atol (row [2]);
		tWorldInfo -> mTribe3Symbol = ::atol (row [3]);
		tWorldInfo -> mTribe4Symbol = ::atol (row [4]);
		tWorldInfo -> mMonsterSymbol = ::atol (row [5]);
		tWorldInfo -> mTribePoint[0] = ::atol (row [6]);
		tWorldInfo -> mTribePoint[1] = ::atol (row [7]);
		tWorldInfo -> mTribePoint[2] = ::atol (row [8]);
		tWorldInfo -> mTribePoint[3] = ::atol (row [9]);
		tWorldInfo -> mTribeCloseInfo[0] = ::atol (row [10]);
		tWorldInfo -> mTribeCloseInfo[1] = ::atol (row [11]);
		tWorldInfo -> mPossibleAllianceInfo[0][0] = ::atol (row [12]);
		tWorldInfo -> mPossibleAllianceInfo[0][1] = ::atol (row [13]);
		tWorldInfo -> mPossibleAllianceInfo[1][0] = ::atol (row [14]);
		tWorldInfo -> mPossibleAllianceInfo[1][1] = ::atol (row [15]);
		tWorldInfo -> mPossibleAllianceInfo[2][0] = ::atol (row [16]);
		tWorldInfo -> mPossibleAllianceInfo[2][1] = ::atol (row [17]);
		tWorldInfo -> mPossibleAllianceInfo[3][0] = ::atol (row [18]);
		tWorldInfo -> mPossibleAllianceInfo[3][1] = ::atol (row [19]);
		tWorldInfo -> mAllianceState[0][0] = ::atol (row [20]);
		tWorldInfo -> mAllianceState[0][1] = ::atol (row [21]);
		tWorldInfo -> mAllianceState[1][0] = ::atol (row [22]);
		tWorldInfo -> mAllianceState[1][1] = ::atol (row [23]);
		tWorldInfo -> mTribeVoteState[0] = ::atol (row [24]);
		tWorldInfo -> mTribeVoteState[1] = ::atol (row [25]);
		tWorldInfo -> mTribeVoteState[2] = ::atol (row [26]);
		tWorldInfo -> mTribeVoteState[3] = ::atol (row [27]);
		tWorldInfo -> mCloseVoteState[0] = ::atol (row [28]);
		tWorldInfo -> mCloseVoteState[1] = ::atol (row [29]);
		tWorldInfo -> mCloseVoteState[2] = ::atol (row [30]);
		tWorldInfo -> mCloseVoteState[3] = ::atol (row [31]);
		tWorldInfo -> mTribe4QuestDate = ::atol (row [32]);
		tWorldInfo -> mTribe4QuestState = ::atol (row [33]);
		::strncpy (tWorldInfo -> mTribe4QuestName, row [34], MAX_TRIBE4_QUEST_NAME_LENGTH);
		++iCounts;
	}

    mDB01.clear_res ();
	if (1 != iCounts){
		return FALSE;
	}

	return TRUE;
}
//DB_PROCESS_13
BOOL MyDB::DB_PROCESS_13( TRIBE_INFO *tTribeInfo )
{
	int index01;
	int index02;
	int index03;
	char tempString01[1000];
	int iAffected;
	int iCounts;
	char mTribeVoteInfo[1041];
	char mTribe01Master[MAX_AVATAR_NAME_LENGTH];
	char mTribe01SubMasterInfo[145];
	char mTribe02Master[MAX_AVATAR_NAME_LENGTH];
	char mTribe02SubMasterInfo[145];
	char mTribe03Master[MAX_AVATAR_NAME_LENGTH];
	char mTribe03SubMasterInfo[145];
	char mTribe04Master[MAX_AVATAR_NAME_LENGTH];
	char mTribe04SubMasterInfo[145];
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;

	sprintf( mQUERY, "select mTribeVoteInfo,mTribe01Master,mTribe01SubMasterInfo,mTribe02Master,mTribe02SubMasterInfo,mTribe03Master,mTribe03SubMasterInfo,mTribe04Master,mTribe04SubMasterInfo from %s WHERE aWorldNumber = %d", mSERVER_INFO.mDB_1_Table07, mSERVER_INFO.mWorldNumber);
	if( !RECONNECT_TO_DATABASE01() )
	{
        LOG_TO_FILE_2 ("!Unable to connect to the database[%s] in %s method.\n", 
                       mSERVER_INFO.mDB_1_Table07, __FUNCTION__);
		return FALSE;
	}
	iAffected = 0;
    if ((iAffected = mDB01.exec_query (mQUERY)) < 0) {
        LOG_TO_FILE_1 ("!The query execution failed in %s.\n", __FUNCTION__);
        LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB01.get_error_msg ().c_str ());
		return FALSE;
    }
	iCounts = 0;
	res = mDB01.get_res ();
	if (res == NULL) {
	    LOG_TO_FILE_1 ("!Query failed - (%s).\n", mQUERY);
        LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB01.get_error_msg ().c_str ());
		return FALSE;
	}
	while (row = mysql_fetch_row(res)){
		::strncpy (mTribeVoteInfo, row [0], 1041);
		::strncpy (mTribe01Master, row [1], MAX_AVATAR_NAME_LENGTH - 1);
		::strncpy (mTribe01SubMasterInfo, row [2], 145);
		::strncpy (mTribe02Master, row [3], MAX_AVATAR_NAME_LENGTH - 1);
		::strncpy (mTribe02SubMasterInfo, row [4], 145);
		::strncpy (mTribe03Master, row [5], MAX_AVATAR_NAME_LENGTH - 1);
		::strncpy (mTribe03SubMasterInfo, row [6], 145);
		::strncpy (mTribe04Master, row [7], MAX_AVATAR_NAME_LENGTH - 1);
		::strncpy (mTribe04SubMasterInfo, row [8], 145);
		++iCounts;
	}
    mDB01.clear_res ();
	if (1 != iCounts){
        LOG_TO_FILE ("!As a result of a number of rows is not returned by the query.\n");
        LOG_TO_FILE_1 ("\t\t\tRow counts = %d.\n", iCounts);
		return FALSE;
	}
	if( strlen( mTribeVoteInfo ) == 0 )
	{
		strcpy( mTribeVoteInfo, "" );
		for( index01 = 0 ; index01 < ( 4 * MAX_TRIBE_VOTE_AVATAR_NUM ) ; index01++ )
		{
			strcat( mTribeVoteInfo, "@@@@@@@@@@@@00000000000000" );
		}
	}
	if( strlen( mTribeVoteInfo ) != 1040 )
	{
        LOG_TO_FILE_1 ("!TribeVoteInfo, the length of the problem was different in %s.\n",
                       __FUNCTION__);
		return FALSE;
	}
	if( strlen( mTribe01SubMasterInfo ) == 0 )
	{
		strcpy( mTribe01SubMasterInfo, "" );
		for( index01 = 0 ; index01 < MAX_TRIBE_SUBMASTER_NUM ; index01++ )
		{
			strcat( mTribe01SubMasterInfo, "@@@@@@@@@@@@" );
		}
	}
	if( strlen( mTribe01SubMasterInfo ) != 144 )
	{
        LOG_TO_FILE_1 ("!Tribe01SubMasterInfo, the length of the problem was different in %s.\n",
                       __FUNCTION__);
		return FALSE;
	}
	if( strlen( mTribe02SubMasterInfo ) == 0 )
	{
		strcpy( mTribe02SubMasterInfo, "" );
		for( index01 = 0 ; index01 < MAX_TRIBE_SUBMASTER_NUM ; index01++ )
		{
			strcat( mTribe02SubMasterInfo, "@@@@@@@@@@@@" );
		}
	}
	if( strlen( mTribe02SubMasterInfo ) != 144 )
	{
        LOG_TO_FILE_1 ("!Tribe02SubMasterInfo, the length of the problem was different in %s.\n",
                       __FUNCTION__);
		return FALSE;
	}
	if( strlen( mTribe03SubMasterInfo ) == 0 )
	{
		strcpy( mTribe03SubMasterInfo, "" );
		for( index01 = 0 ; index01 < MAX_TRIBE_SUBMASTER_NUM ; index01++ )
		{
			strcat( mTribe03SubMasterInfo, "@@@@@@@@@@@@" );
		}
	}
	if( strlen( mTribe03SubMasterInfo ) != 144 )
	{
        LOG_TO_FILE_1 ("!Tribe03SubMasterInfo, the length of the problem was different in %s.\n",
                       __FUNCTION__);
		return FALSE;
	}
	if( strlen( mTribe04SubMasterInfo ) == 0 )
	{
		strcpy( mTribe04SubMasterInfo, "" );
		for( index01 = 0 ; index01 < MAX_TRIBE_SUBMASTER_NUM ; index01++ )
		{
			strcat( mTribe04SubMasterInfo, "@@@@@@@@@@@@" );
		}
	}
	if( strlen( mTribe04SubMasterInfo ) != 144 )
	{
        LOG_TO_FILE_1 ("!Tribe04SubMasterInfo, the length of the problem was different in %s.\n",
                       __FUNCTION__);
		return FALSE;
	}
	for( index01 = 0 ; index01 < 4 ; index01++ )
	{
		for( index02 = 0 ; index02 < MAX_TRIBE_VOTE_AVATAR_NUM ; index02++ )
		{
			CopyMemory( &tTribeInfo->mTribeVoteName[index01][index02][0], &mTribeVoteInfo[( ( index01 * MAX_TRIBE_VOTE_AVATAR_NUM + index02 ) * 26 + 0 )], 12 );
			for( index03 = 0 ; index03 < 12 ; index03++ )
			{
				if (::IsDBCSLeadByte (tTribeInfo->mTribeVoteName[index01][index02][index03]) == TRUE) {
					if (index03 != 11) index03++;
					continue;
				}

				if( tTribeInfo->mTribeVoteName[index01][index02][index03] == '@' )
				{
					break;
				}
			}
			tTribeInfo->mTribeVoteName[index01][index02][index03] = '\0';
			CopyMemory( &tempString01[0], &mTribeVoteInfo[( ( index01 * MAX_TRIBE_VOTE_AVATAR_NUM + index02 ) * 26 + 12 )], 2 );
			tempString01[2] = '\0';
			tTribeInfo->mTribeVoteLevel[index01][index02] = atoi( tempString01 );
			CopyMemory( &tempString01[0], &mTribeVoteInfo[( ( index01 * MAX_TRIBE_VOTE_AVATAR_NUM + index02 ) * 26 + 14 )], 6 );
			tempString01[6] = '\0';
			tTribeInfo->mTribeVoteKillOtherTribe[index01][index02] = atoi( tempString01 );
			CopyMemory( &tempString01[0], &mTribeVoteInfo[( ( index01 * MAX_TRIBE_VOTE_AVATAR_NUM + index02 ) * 26 + 20 )], 6 );
			tempString01[6] = '\0';
			tTribeInfo->mTribeVotePoint[index01][index02] = atoi( tempString01 );
		}
	}
	strcpy( tTribeInfo->mTribeMaster[0], mTribe01Master );
	for( index01 = 0 ; index01 < MAX_TRIBE_SUBMASTER_NUM ; index01++ )
	{
		CopyMemory( &tTribeInfo->mTribeSubMaster[0][index01][0], &mTribe01SubMasterInfo[( index01 * 12 + 0 )], 12 );
		for( index02 = 0 ; index02 < 12 ; index02++ )
		{
			if (::IsDBCSLeadByte (tTribeInfo->mTribeSubMaster[0][index01][index02]) == TRUE) {\
				if (index02 != 11) index02++;
				continue;
			}

			if( tTribeInfo->mTribeSubMaster[0][index01][index02] == '@' )
			{
				break;
			}
		}
		tTribeInfo->mTribeSubMaster[0][index01][index02] = '\0';
	}
	strcpy( tTribeInfo->mTribeMaster[1], mTribe02Master );
	for( index01 = 0 ; index01 < MAX_TRIBE_SUBMASTER_NUM ; index01++ )
	{
		CopyMemory( &tTribeInfo->mTribeSubMaster[1][index01][0], &mTribe02SubMasterInfo[( index01 * 12 + 0 )], 12 );
		for( index02 = 0 ; index02 < 12 ; index02++ )
		{
			if (::IsDBCSLeadByte (tTribeInfo->mTribeSubMaster[1][index01][index02]) == TRUE) {
				if (index02 != 11) index02++;
				continue;
			}

			if( tTribeInfo->mTribeSubMaster[1][index01][index02] == '@' )
			{
				break;
			}
		}
		tTribeInfo->mTribeSubMaster[1][index01][index02] = '\0';
	}
	strcpy( tTribeInfo->mTribeMaster[2], mTribe03Master );
	for( index01 = 0 ; index01 < MAX_TRIBE_SUBMASTER_NUM ; index01++ )
	{
		CopyMemory( &tTribeInfo->mTribeSubMaster[2][index01][0], &mTribe03SubMasterInfo[( index01 * 12 + 0 )], 12 );
		for( index02 = 0 ; index02 < 12 ; index02++ )
		{
			if (::IsDBCSLeadByte (tTribeInfo->mTribeSubMaster[2][index01][index02]) == TRUE) {
				if (index02 != 11) index02++;
				continue;
			}

			if( tTribeInfo->mTribeSubMaster[2][index01][index02] == '@' )
			{
				break;
			}
		}
		tTribeInfo->mTribeSubMaster[2][index01][index02] = '\0';
	}
	strcpy( tTribeInfo->mTribeMaster[3], mTribe04Master );
	for( index01 = 0 ; index01 < MAX_TRIBE_SUBMASTER_NUM ; index01++ )
	{
		CopyMemory( &tTribeInfo->mTribeSubMaster[3][index01][0], &mTribe04SubMasterInfo[( index01 * 12 + 0 )], 12 );
		for( index02 = 0 ; index02 < 12 ; index02++ )
		{
			if (::IsDBCSLeadByte (tTribeInfo->mTribeSubMaster[3][index01][index02]) == TRUE) {
				if (index02 != 11) index02++;
				continue;
			}

			if( tTribeInfo->mTribeSubMaster[3][index01][index02] == '@' )
			{
				break;
			}
		}
		tTribeInfo->mTribeSubMaster[3][index01][index02] = '\0';
	}
	
	return TRUE;
}

//DB_PROCESS_14
int MyDB::DB_PROCESS_14(const char *ip_addr, const char *user_id)
{
	int iAffected;
	int iCounts;
	int iCheckValue = 0;
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;

#if 0
	sprintf( mQUERY, 
			"SELECT COUNT(uStartIP) AS cnt "
			"FROM PcRoomIpInfo "
			"WHERE uStartIP <= '%s' AND uEndIP >= '%s' "
			, ip_addr, ip_addr);
#endif
	sprintf( mQUERY, 
			"SELECT COUNT(uStartIP) AS cnt "
			"FROM PcRoomIpInfo "
			"WHERE uStartIP = '%s' ", ip_addr);

	if( !RECONNECT_TO_DATABASE02() )
	{
		return 0;
	}
	iAffected = 0;
    if ((iAffected = mDB02.exec_query (mQUERY)) < 0) {
		return 0;
	}
	iCounts = 0;

	res = mDB02.get_res ();
	if (res == NULL) {
	    LOG_TO_FILE_1 ("!Query failed - (%s).\n", mQUERY);
        LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB02.get_error_msg ().c_str ());
		return 0;
	}
	while (row = mysql_fetch_row(res)){
		iCheckValue = ::atoi(row[0]);
		++iCounts;
	}
    mDB02.clear_res ();

	if(iCheckValue > 0) {
		if(iCheckValue > 1) {
			LOG_TO_FILE_2("![%s] : duplicate ip address : %s\n", __FUNCTION__, ip_addr);
		}
		return 2;
	}

	return 0;
}

bool MyDB::DB_PROCESS_15(int aTribe)
{
	MYSQL_ROW row;
	MYSQL_RES *res = NULL;
	int iCounts = 0;
	char CreateFlag[3];

	::memset(CreateFlag, 0, 3);

	sprintf( mQUERY, 
			"SELECT aCreateFlag0, aCreateFlag1, aCreateFlag2 "
			"FROM TribeControl "
			"WHERE aWorldNumber=%d", mSERVER_INFO.mWorldNumber);

	if(!RECONNECT_TO_DATABASE01()) {
		return false;
	}

    if((mDB01.exec_query (mQUERY)) != 0) {
		return false;
	}

	res = mDB01.get_res();
	if (res == NULL) {
	    LOG_TO_FILE_1 ("!Query failed - (%s).\n", mQUERY);
        LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB01.get_error_msg ().c_str ());
		return 0;
	}
	while (row = mysql_fetch_row(res)){
		CreateFlag[0] = *(row[0]);
		CreateFlag[1] = *(row[1]);
		CreateFlag[2] = *(row[2]);
		++iCounts;
	}
    mDB01.clear_res();

	if(iCounts != 1) {
		return false;
	}

	if(CreateFlag[aTribe] == '0') {
		return false;
	}

	return true;
}

// @ __WEB_MALL__
//DB_PROCESS_16 // 북미, 유럽, 터키는 Web Mall을 사용해서 선물 받기 처리함.
//DB_PROCESS_17 // 선물 받기 처리 별도로 하지 않음.

//DB_PROCESS_18
BOOL MyDB::DB_PROCESS_18(char *tID, int tLimit, int *tList, int &tCount, WEBMALL_INFO *tGiftInfo)
{
    int iAffected;
    int iCounts;
    int index01 = 0;

    MYSQL_ROW row;
    MYSQL_RES *res = NULL;

    sprintf (mQUERY,
            "select a.gGiftItemSetNumber, (a.gPurchaseCnt - a.gUsedCnt), ifnull(b.gGiftItem01, 0), ifnull(b.gGiftItem01Cnt, 0), "
            "ifnull(b.gGiftItem02, 0), ifnull(b.gGiftItem02Cnt, 0), ifnull(b.gGiftItem03, 0), ifnull(b.gGiftItem03Cnt, 0), ifnull(b.gGiftItem04, 0), ifnull(b.gGiftItem04Cnt, 0), "
            "ifnull(b.gGiftItem05, 0), ifnull(b.gGiftItem05Cnt, 0), ifnull(b.gGiftItem06, 0), ifnull(b.gGiftItem06Cnt, 0), ifnull(b.gGiftItem07, 0), ifnull(b.gGiftItem07Cnt, 0), "
            "ifnull(b.gGiftItem08, 0), ifnull(b.gGiftItem08Cnt, 0), ifnull(b.gGiftItem09, 0), ifnull(b.gGiftItem09Cnt, 0), ifnull(b.gGiftItem10, 0), ifnull(b.gGiftItem10Cnt, 0), "
            "a.gWebMallSeq "
            "from %s a left join %s b on a.gGiftItemSetNumber = b.gGiftItemSetNumber where a.gUseID = '%s' and 0 < (a.gPurchaseCnt - a.gUsedCnt) order by a.gWebMallSeq asc limit %d",
            mSERVER_INFO.mDB_1_Table09, mSERVER_INFO.mDB_1_Table10, tID, tLimit);

    if( !RECONNECT_TO_DATABASE02() )
    {
        return FALSE;
    }
    iAffected = 0;
    if ((iAffected = mDB02.exec_query (mQUERY)) < 0)
        return FALSE;
    iCounts = 0;
#if 0
    if ((iCounts = mDB01.get_num_rows ()) != 1)
        return FALSE;
#endif
    res = mDB02.get_res ();
    if (res == NULL) {
        LOG_TO_FILE_1 ("!Query failed - (%s).\n", mQUERY);
        LOG_TO_FILE_1 ("\t\t\tWhy - %s\n", mDB02.get_error_msg ().c_str ());
        return FALSE;
    }
    while (row = mysql_fetch_row (res)){
        tList[iCounts * 3] = ::atoi (row[0]);
        tList[iCounts * 3 + 1] = ::atoi (row[1]);
        tList[iCounts * 3 + 2] = 0;
        tGiftInfo[iCounts].mGiftData[0][0] = ::atoi (row[2]);
        tGiftInfo[iCounts].mGiftData[0][1] = ::atoi (row[3]);
        tGiftInfo[iCounts].mGiftData[1][0] = ::atoi (row[4]);
        tGiftInfo[iCounts].mGiftData[1][1] = ::atoi (row[5]);
        tGiftInfo[iCounts].mGiftData[2][0] = ::atoi (row[6]);
        tGiftInfo[iCounts].mGiftData[2][1] = ::atoi (row[7]);
        tGiftInfo[iCounts].mGiftData[3][0] = ::atoi (row[8]);
        tGiftInfo[iCounts].mGiftData[3][1] = ::atoi (row[9]);
        tGiftInfo[iCounts].mGiftData[4][0] = ::atoi (row[10]);
        tGiftInfo[iCounts].mGiftData[4][1] = ::atoi (row[11]);
        tGiftInfo[iCounts].mGiftData[5][0] = ::atoi (row[12]);
        tGiftInfo[iCounts].mGiftData[5][1] = ::atoi (row[13]);
        tGiftInfo[iCounts].mGiftData[6][0] = ::atoi (row[14]);
        tGiftInfo[iCounts].mGiftData[6][1] = ::atoi (row[15]);
        tGiftInfo[iCounts].mGiftData[7][0] = ::atoi (row[16]);
        tGiftInfo[iCounts].mGiftData[7][1] = ::atoi (row[17]);
        tGiftInfo[iCounts].mGiftData[8][0] = ::atoi (row[18]);
        tGiftInfo[iCounts].mGiftData[8][1] = ::atoi (row[19]);
        tGiftInfo[iCounts].mGiftData[9][0] = ::atoi (row[20]);
        tGiftInfo[iCounts].mGiftData[9][1] = ::atoi (row[21]);
        //2009.10.20 : 김성수: 웹몰의 구매 일련 번호를 넣기 위한 변수 추가
        tGiftInfo[iCounts].mWebMallSeq = ::atoi (row[22]);


        for(index01 = 0; index01 < MAX_GIFT_ITEM_NUM; ++index01){
            if(0 != tGiftInfo[iCounts].mGiftData[index01][0])
                continue;
            tGiftInfo[iCounts].mTotalGiftNum = index01;
            break;
        }
        ++iCounts;
    }
    tCount = iCounts;
    mDB02.clear_res ();

    return TRUE;
}
//DB_PROCESS_19
BOOL MyDB::DB_PROCESS_19(char *tID, int tItemSetNumber, int tUsedCnt , int tWebMallSeq)
{
    int iAffected;

    sprintf (mQUERY, "update %s set gUsedCnt = gUsedCnt + %d where gUseID = '%s' and gWebMallSeq = %d ",
            mSERVER_INFO.mDB_1_Table09, tUsedCnt, tID, tWebMallSeq);

    if( !RECONNECT_TO_DATABASE02() )
    {
        return FALSE;
    }

    iAffected = 0;
    if (mDB02.exec_query (mQUERY, CMySQL::CLEAR) != 0)
        return FALSE;

    return TRUE;
} // __WEB_MALL__ @
//-------------------------------------------------------------------------------------------------
