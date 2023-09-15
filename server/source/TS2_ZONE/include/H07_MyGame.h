//-------------------------------------------------------------------------------------------------
//MY_GAME_H
//-------------------------------------------------------------------------------------------------
#ifndef MY_GAME_H
#define MY_GAME_H
#include "zlib.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_GAME
//-------------------------------------------------------------------------------------------------
//FUNCTION_TYPE
#ifdef __PARTY_BUG_FIX__
typedef struct s_party_check
{
	char aName[MAX_AVATAR_NAME_LENGTH];
	int tSort;
	time_t RegDateTime;
} party_check_t;
#endif
#ifdef __WIN32__
typedef int ( * FCompress )( BYTE *tDest, DWORD *tDestLength, BYTE *tSource, DWORD tSourceLength, int tLevel );
typedef int ( * FUncompress )( BYTE *tDest, DWORD *tDestLength, BYTE *tSource, DWORD tSourceLength );
#endif
//DEFINE
class AVATAR_OBJECT;

class MyGame
{
	unsigned int getCriticalAttackDefValue(AVATAR_OBJECT *_attacker, AVATAR_OBJECT *_defenser );
public :

	ZONE_CONNECTION_INFO mZoneConnectionInfo;

	WORLD_INFO mWorldInfo;
	TRIBE_INFO mTribeInfo;
#ifdef __WIN32__
	HINSTANCE mCompressLibrary;
	FCompress mFCompress;
	FUncompress mFUncompress;
#endif

	int mAutoCheckAnswer1;
	int mAutoCheckAnswer2;
	int mAutoCheckDataSize;
	BYTE mAutoCheckData[1000];
	char *mRankInfo;
	int mRankInfoSize;

	float mGeneralExperienceUpRatio;
	float mItemDropUpRatio;
	float mItemDropUpRatioForMyoung;
	int mKillOtherTribeAddValue;
#ifdef __GOD__
	int mKillOtherTribeUpValue;
#endif

	int mTeacherPointUpRatio;		//... .. (1~2)
	float mPatExperienceUpRatio;	//.. ... .. (10~100)
	float mBonusGainGeneralExperienceUpRatio;	//.. ... .. (10~100)
	int mGeneralExperienceDownRatio;	//.. ... .. (0~2)
	int mKillOtherTribeExperienceUpRatio;

	//@{
	int mAutoUserMonsterCount; ///< 오토유저 검색용 몬스터 카운터 (1~100000)

	SELL_ITEM_INFO mSellItemInfo[MAX_SELL_ITEM_INFO_NUM];

	DWORD mAvatarObjectUniqueNumber;
	DWORD mMonsterObjectUniqueNumber;
	DWORD mItemObjectUniqueNumber;
	int mAvatarDuelUniqueNumber;
	DWORD mAvatarPShopUniqueNumber;

	DWORD mTickCount;

	DWORD mUpdateTimeForTribeBankInfo;
	int mTribeBankInfo[4];

	float mSymbolDamageUp[4];
	float mSymbolDamageDown[4];

	BOOL mCheckActiveBattleLogic;

	BOOL mCheckTribeVoteServer;

	BOOL mCheckAllienceServer;
	DWORD mAlliencePostTick;
	BYTE mAllienceBattleState;
	int mAllienceRemainTime;
	float mAllienceBattlePostLocation[2][3];
	float mAllienceBattlePostRadius[2];
	int mAlliencePostAvatarIndex[2];
	DWORD mAlliencePostAvatarUniqueNumber[2];
	int mAlliencePostAvatarTribe[2];

	BOOL mCheckZone037Server;
	DWORD mZone037PostTick;
	BYTE mZone037BattleState;
	int mZone037RemainTime;

	BOOL mCheckZone124Server;

	BOOL mCheckZone071TypeServer;
	int mZone071TypeZoneIndex;

	BOOL mCheckZone049TypeServer;
	int mZone049TypeZoneIndex;
	DWORD mZone049TypePostTick;
	BYTE mZone049TypeBattleState;
	int mZone049TypeRemainTime1;
	int mZone049TypeRemainTime2;
	int mZone049TypeBattleResult[4];

	BOOL mCheckZone051TypeServer;
	int mZone051TypeZoneIndex;
	DWORD mZone051TypePostTick;
	BYTE mZone051TypeBattleState;
	int mZone051TypeRemainTime;

	BOOL mCheckZone053TypeServer;
	int mZone053TypeZoneIndex;
	DWORD mZone053TypePostTick;
	BYTE mZone053TypeBattleState;
	int mZone053TypeRemainTime;
	float mZone053TypeBattlePostLocation[3];
	float mZone053TypeBattlePostRadius;
	int mZone053TypePostAvatarIndex;
	DWORD mZone053TypePostAvatarUniqueNumber;
	DWORD mZone053TypeSaveTick;

	BOOL mCheckZone038Server;
	DWORD mZone038PostTick;
	BYTE mZone038BattleState;
	int mZone038RemainTime;
	float mZone038BattlePostLocation[3];
	float mZone038BattlePostRadius;
	int mZone038PostAvatarIndex;
	DWORD mZone038PostAvatarUniqueNumber;

	BOOL mCheckZone039TypeServer;
	DWORD mZone039TypePostTick;
	BYTE mZone039TypeBattleState;

	BOOL mCheckZone101TypeServer;

	BOOL mCheckZone125Server;

	BOOL mCheckZone126TypeServer;
#ifdef __GOD__
	BOOL mCheckZone126RealTypeServer;
#endif
	BOOL mCheckZone175TypeServer;
#ifdef __GOD__
	bool mCheckZoneDuelTypeServer;
	bool mCheckZoneAllDuelTypeServer;
	bool mCheckZone084TypeServer;
#endif
	int mZone175TypeZoneIndex1;
	int mZone175TypeZoneIndex2;
	DWORD mZone175TypePostTick;
	BYTE mZone175TypeBattleState;
	int mZone175TypeRemainTime;
	
	
#ifdef __GOD__	
	BOOL mCheckZone194Server;
	DWORD mZone194PostTick;
	BYTE mZone194BattleState;
	int mZone194RemainTime1;
	int mZone194RemainTime2;
	int mZone194BattleResult[4];

	BOOL mCheckZone195TypeServer;

	BOOL mCheckZone267TypeServer;
	int mZone267TypeZoneIndex;
	DWORD mZone267TypePostTick;
	BYTE mZone267TypeBattleState;
	int mZone267TypeRemainTime1;
	int mZone267TypeRemainTime2;
	int mZone267TypeBattleResult[4];
#endif
	/** 2009. 10. 27 : MonsterSymbol edit (wolf) */
    bool bAttackMonsterSymbol;
    DWORD dwAttackMonsterSymbolTime;
    /** 2009. 10. 27 : MonsterSymbol edit (wolf) */


#ifdef __PARTY_BUG_FIX__
	multimap<string, party_check_t> map_party_check;
#endif

	BOOL mDuel_124;
	int mDuel_124_RemainTime;
	int mDuel_124_AvatarNum[2];
	BOOL mDuel_124_Pvp;

	int mZone84Time;

    int mPremiumExpirationCountDownDate;    // 프리미엄 서버(월드) 입장권 만료 5분 카운트 시작하는 일자. // @_Premium_Server_@
    int mPremiumServerExpirationCountDown;  // 프리미엄 서버(월드) 입장권 만료 5분 카운트 다운용 변수. // @_Premium_Server_@

	MyGame( void );

	BOOL Init( void );
	void Free( void );

	BOOL Decompress( DWORD tCompressSize, BYTE *tCompress, DWORD tOriginalSize, BYTE *tOriginal );

	BOOL LoadForAutoCheckInfo( void );

	BOOL LoadForSellItemInfo( void );
	void SaveForSellItemInfo( void );
#ifdef __GOD__
	BOOL LoadForItemMoneyInfo( void );
	int ReturnBigTribe( void );
	void Process_Zone_194( void );
	void Process_Zone_267_TYPE( void );
	void ReturnLevelWarReward3( int tLevel1, int tLevel2, int *tGainExperience, int *tGainMoney );
#endif
	void ProcessForValidAvatarInfoWithItem( AVATAR_INFO *tAvatarInfo );

	void Logic( float dTime );

	int ReturnTribeRole( char tAvatarName[MAX_AVATAR_NAME_LENGTH], int tTribe );
	int ReturnTribeSubMasterNum( int tTribe );

	void AddTribeBankInfo( int tTribe, int tMoney );

	int ReturnTribeForSymbol( int tTribe );
	int ReturnAllianceTribe( int tTribe );

	int ReturnTribePointWithAlliance( int tTribe );
	int CheckPossibleChangeToTribe4( int tTribe );
	int CheckPossibleChangeToTribe4_2(int tTribe);
	BOOL CheckSmallTribeForAdvantage( int tTribe );
    // @ Tribe Formation Skill @
    int ReturnSmallTribe( void );
    // @

	void AdjustSymbolDamageInfo( void );
	float ReturnSymbolDamageUp( int tTribe );
	float ReturnSymbolDamageDown( int tTribe );

	void ProcessForGuardState( void );

	void Process_TribeVote_Server( void );
	void Process_Allience_Server( void );
	void Process_Zone_037( void );
	void Process_Zone_124( void );
	void Process_Zone_071_TYPE( void );
	void Process_Zone_049_TYPE( void );
	void Process_Zone_051_TYPE( void );
	void Process_Zone_053_TYPE( void );
	void GetLevelBattleRewardInfo( int tLevel1, int tLevel2, int *tGainExperience, int *tGainMoney, BOOL tWin );
	void Process_Zone_038( void );
	void Process_Zone_039_TYPE( void );
	void Process_Zone_175_TYPE( void );
#ifdef __GOD__
	void GetZone175TypeRewardInfo( int tLevel, int tLevel2, int tStep, int *tGainExperience, int *tGainMoney );
#else
	void GetZone175TypeRewardInfo( int tLevel, int tStep, int *tGainExperience, int *tGainMoney );
#endif

	void ProcessForBroadcastInfo( int tInfoSort, BYTE tInfoData[MAX_BROADCAST_INFO_SIZE] );

	BOOL CheckPossibleAttackTarget( int tTargetSort, int tActionSort );

	//01-[아바타->아바타(비무)]
	void ProcessAttack01( int tUserIndex, ATTACK_FOR_PROTOCOL *tAttackInfo );
	//02-[아바타->아바타(세력)]
	void ProcessAttack02( int tUserIndex, ATTACK_FOR_PROTOCOL *tAttackInfo );
	//03-[아바타->몬스터]
	void ProcessAttack03( int tUserIndex, ATTACK_FOR_PROTOCOL *tAttackInfo );
	//04-[몬스터->아바타]
	void ProcessAttack04( int tUserIndex, ATTACK_FOR_PROTOCOL *tAttackInfo );
	//05-[점혈]
	void ProcessAttack05( int tUserIndex, ATTACK_FOR_PROTOCOL *tAttackInfo );
	//06-[해혈]
	void ProcessAttack06( int tUserIndex, ATTACK_FOR_PROTOCOL *tAttackInfo );
	void HealthCheck(void);

	//Auto potion.
	BOOL Add_Use_Auto_Day(int iIndex, int iDay);
	int Check_Use_Auto_Day(int mIndex);
    // @ Cost Info Download @
    void CheckUpdateCashItemInfo();
    // @
    int ReturnAlliance(int tTribe);
    int CheckMoveZoneForMoveZoneItem(int tTribe, int tLevel, int tZoneNumber);
};
//INSTANCE
extern MyGame mGAME;
//-------------------------------------------------------------------------------------------------

#define     UNIT_SCALE_RADIUS1      1   // 브로드 캐스팅 범위 (MAX_RADIUS_FOR_NETWORK * 1)
#define     UNIT_SCALE_RADIUS2      2   // 브로드 캐스팅 범위 (MAX_RADIUS_FOR_NETWORK * 2)
#define     UNIT_SCALE_RADIUS3      3   // 브로드 캐스팅 범위 (MAX_RADIUS_FOR_NETWORK * 3)

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_UTIL
//-------------------------------------------------------------------------------------------------
//DEFINE
class MyUtil
{

private :

	int mEquipPart[MAX_EQUIP_SLOT_NUM];

public :

	MyUtil( void );

	BOOL CheckNameString( char *tString );

	BOOL CheckOverMaximum( int tValue1, int tValue2 );

	int ReturnFixedLevel( int tLevel );

	BOOL CheckPossibleEquipItem( int iIndex, int tTribe, int tEquipIndex, int tLevel );

	void ProcessForExperience( int tUserIndex, int tGainGeneralExperience, int tGainPatExperience, int tGainLifeValue );
	BOOL ProcessForDropItem( int tDropSort, int tItemIndex, int tItemQuantity, int tItemValue, int tItemRecognitionNumber, float tLocation[3], char tMaster[MAX_AVATAR_NAME_LENGTH], int tTribe );

	void Broadcast( BOOL tCheckInRange, float tPostLocation[3], int iScale, bool bUDPSend = true, int type = 1 );
	void Broadcast( BOOL tCheckInRange, float tPostLocation[3], int iScale, int tUserIndexForNoSend, bool bUDPSend = true, int type = 1 );

	float RandomNumber( float tMinValue, float tMaxValue );
	int RandomNumber( void );

	BOOL CheckInRange( float tLocation01[3], float tLocation02[3], float tDistance );

	float GetLengthXYZ( float tLocation01[3], float tLocation02[3] );
	float GetDoubleXYZ( float tLocation01[3], float tLocation02[3] );
	float GetLengthX_Z( float tLocation01[3], float tLocation02[3] );
	float GetDoubleX_Z( float tLocation01[3], float tLocation02[3] );

	float GetYAngle( float x1, float z1, float x2, float z2 );

	int ReturnItemRecognitionNumber( int tItemType );

	int ReturnNowDate( void );
	int ReturnAddDate( int tPostDateValue, int tAddDayValue );
	int ReturnSubDate( int tPostDateValue, int tSubDayValue );

	int ReturnVoteDate( void );

	int ReturnISValue( int tValue );
	int ReturnIUValue( int tValue );
	int ChangeISValue( int tValue, int tISValue );
	int ChangeIUValue( int tValue, int tIUValue );
	int SetISIUValue( int tISValue, int tIUValue );
	int ReturnIUEffectValue( ITEM_INFO *tITEM_INFO, int tEffectSort );
	int ReturnISValueWithLevelLimit( int tLevel, int tISValue );
	int ReturnIUValueWithLevelLimit( int tLevel, int tIUValue );

	void ProcessForKillOtherTribe( int tUserIndex, int tOtherIndex, int tZoneNumber );
	void DropItemForKillOtherTribe( int tUserIndex, int tOtherIndex, int tZoneNumber );

	int SearchAvatar( char tAvatarName[MAX_AVATAR_NAME_LENGTH], int tInvalidIndex );

	bool FindEmptyInvenForItem (int iUserIndex, ITEM_INFO* pItem, int& iPage, int& iIndex);
	bool SetInvenSlotHumanReadable (int iUserIndex, int iPage, bool bInvenSlots[MAX_INVENTORY_SLOT_NUM]);
	int  GetEmptyInvenSlotIndex (int iUserIndex, int iPage);
	int ReturnHealthState( int tPlayTime );
	BOOL ProcessForChange( int tUserIndex );
    // # Defense Hack # {
    bool CheckInvenForItem(int *tUserIndex, ITEM_INFO *pItem, int *tPage, int *tIndex, int *tXPost, int *tYPost);
    int GetAttackRadius(int tSkillNumber);
    bool CheckRAttackRange(int tUserIndex, int tSkillNumber, float tLocation1[3], float tLocation2[3], float tObjectSize);
    float ReturnLengthXYZ(float tLocation01[3], float tLocation02[3]);
    // # }
    
#ifdef __GOD__
	int ReturnUpgradeValueWithLevelLimit( int tLevel, int tUpgradeValue );
	BOOL CheckChangeTribeTime( void );
#endif
};
//INSTANCE
extern MyUtil mUTIL;
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_AVATAR_OBJECT
//-------------------------------------------------------------------------------------------------
//DEFINE
class AVATAR_OBJECT
{

private :

	ITEM_INFO *mEquipItem[MAX_EQUIP_SLOT_NUM];
	int mEquipValue[MAX_EQUIP_SLOT_NUM];

	int mMaxLife;
	int mMaxMana;
	int mAttackPower;
	int mDefensePower;
	int mAttackSuccess;
	int mAttackBlock;
	int mElementAttackPower;
	int mElementDefensePower;
	int mCritical;
	int mCriticalDefense;
	int mLastAttackBonusProbability;
	float mLastAttackBonusValue;
	int mLuck;

public :

	int mIndex;

	BOOL mCheckValidState;
	DWORD mUniqueNumber;

	OBJECT_FOR_AVATAR mDATA;

	BOOL mCheckPossibleEatPotion;

	PSHOP_INFO mPShopInfo;

	int mDuelProcessState;
	char mDuelProcessAvatarName[MAX_AVATAR_NAME_LENGTH];
	int mDuelProcessSort;
	int mDuelProcessRemainTime;
	int mTradeProcessState;
	char mTradeProcessAvatarName[MAX_AVATAR_NAME_LENGTH];
	int mTradeProcessMenu;
	int mFriendProcessState;
	char mFriendProcessAvatarName[MAX_AVATAR_NAME_LENGTH];
	int mPartyProcessState;
	char mPartyProcessAvatarName[MAX_AVATAR_NAME_LENGTH];
	int mGuildProcessState;
	char mGuildProcessAvatarName[MAX_AVATAR_NAME_LENGTH];
	int mTeacherProcessState;
	char mTeacherProcessAvatarName[MAX_AVATAR_NAME_LENGTH];

	int mSPACE_INDEX[3];
    // # Defense Hack # {
    bool mCheckDeath;
	int mCheckCount;
    //DWORD mPRE_TICKCOUNT;
    float mPRE_LOCATION[3];
    // # }
	DWORD mUpdateTimeForBroadcast;
	int mGod;
	int mChat;

	void Init(void);
	void SetSpaceIndex( void );

	void SetBasicAbilityFromEquip( void );

	int GetWeaponClass( void );
	int GetMaxLife( void );
	int GetMaxMana( void );
	int GetAttackPower( void );
	int GetDefensePower( void );
	int GetAttackSuccess( void );
	int GetAttackBlock( void );
	int GetElementAttackPower( void );
	int GetElementDefensePower( void );
	int GetCritical( void );
	int GetCriticalDefense( void );
	int GetLastAttackBonusProbability( void );
	float GetLastAttackBonusValue( void );
	int GetLuck( void );
	int GetBonusSkillValue( int sIndex );
	int GetReduceManaRatio( void );

	BOOL CheckCommunityWork( void );

	void Update( float tPostTime );

	void ProcessForCreateEffectValue( ACTION_INFO *tAction );
	void ProcessForDeleteEffectValue( void );

	void ProcessForDeath( void );

	void ProcessForTradeInfo( void );

	int ReturnQuestPresentState( void );
	int ReturnQuestEndConditionState( void );
	int ReturnQuestNextNPCNumber( void );
	int ReturnItemNumberForQuestReward( void );
	int ReturnItemQuantityForQuestReward( void );
	int SummonQuestBoss( void );
	int ChangeQuestItem( int tItemNumber1, int tItemNumber2 );
	int DeleteQuestItem( int tItemNumber );

	/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */
	int IsUsedOrnament (void);
	/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */

};
//INSTANCE
extern AVATAR_OBJECT mAVATAR_OBJECT[MAX_AVATAR_OBJECT_NUM];
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MONSTER_OBJECT
//-------------------------------------------------------------------------------------------------
//DEFINE
class MONSTER_OBJECT
{

private :

	int SelectAvatarIndexForPossibleAttack( void );
	void SelectAvatarIndexForPossibleAttackForZone175TypeBoss( void );
	int SelectAvatarIndexForIdleAI( void );
	void AdjustValidAttackTarget( void );
	void AdjustValidAttackTargetForZone175TypeBoss( void );
	int SelectAvatarIndexForAttackAction( void );
	int SelectAvatarIndexForGuardAttack( void );
	int SelectAvatarIndexForIdleAIWIthThrowCar( void );
	int SelectAvatarIndexForThrowCarAttack( void );

	void A001( int tObjectIndex, float tPostTime );
	void A002( int tObjectIndex, float tPostTime );
	void A002_FOR_ZONE_175_TYPE_BOSS( int tObjectIndex, float tPostTime );
	void A004( int tObjectIndex, float tPostTime );
	void A005( int tObjectIndex, float tPostTime );
	void A005_FOR_ZONE_175_TYPE_BOSS( int tObjectIndex, float tPostTime );
	void A006( int tObjectIndex, float tPostTime );
	void A008( int tObjectIndex, float tPostTime );
	void A009( int tObjectIndex, float tPostTime );
	void A013( int tObjectIndex, float tPostTime );
	void A020( int tObjectIndex, float tPostTime );

public :

	BOOL mCheckValidState;
	DWORD mUniqueNumber;

	OBJECT_FOR_MONSTER mDATA;

	int mSPACE_INDEX[3];

	MONSTER_INFO *mMONSTER_INFO;
	int mSameTargetPostNum;

	DWORD mInvalidTimeForSummon;

	DWORD mCheckFirstLocationTime;
	float mFirstLocation[3];

    float mRadiusForSize;

	DWORD mCheckDetectEnemyTime;

	BOOL mCheckAttackState;
	int mTotalAttackNum;
	char mAvatarName[MAX_AVATAR_NAME_LENGTH]; // 몬스터 소유자. - 소유자 외에는 몬스터 타격 불가.
	int mAttackIndex[MAX_MONSTER_OBJECT_ATTACK_NUM];
	DWORD mAttackUniqueNumber[MAX_MONSTER_OBJECT_ATTACK_NUM];
	DWORD mAttackDamage[MAX_MONSTER_OBJECT_ATTACK_NUM];
	float mAttackLength[MAX_MONSTER_OBJECT_ATTACK_NUM];

	DWORD mUpdateTimeForBroadcast;

	int mLastAttackUserIndex;
	DWORD mLastAttackUniqueNumber;
	int mLastAttackUserTribe;
	char mLastAttackUserName[MAX_AVATAR_NAME_LENGTH];

	int mSpecialSortNumber;

	BOOL mCheckFirstAttackForTribeSymbol;
	DWORD mFirstAttackTimeForTribeSymbol;
	DWORD mTribeDamageForTribeSymbol[4];

	BOOL mCheckFirstAttackForAllianceStone;
	DWORD mFirstAttackTimeForAllianceStone;

	BOOL mCheckAttackPacketRecv;
	DWORD mAttackPacketRecvTime;

	void SetSpaceIndex( void );

	void Update( int tObjectIndex, float tPostTime );

	void SetAttackInfoWithAvatar( int tServerIndex, DWORD tUniqueNumber, int *tWhoAttackMonster );

	int SelectAvatarIndexForMaxAttackDamage( void );

	void ProcessForDropItem( int tUserIndex );
#ifdef __GOD__
	void ProcessForDropItem( int tUserIndex, int tMonsterIndex );
#endif

};
//INSTANCE
extern MONSTER_OBJECT mMONSTER_OBJECT[MAX_MONSTER_OBJECT_NUM];
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_ITEM_OBJECT
//-------------------------------------------------------------------------------------------------
//DEFINE
class ITEM_OBJECT
{

public :

	BOOL mCheckValidState;
	DWORD mUniqueNumber;

	OBJECT_FOR_ITEM mDATA;

	int mSPACE_INDEX[3];

	ITEM_INFO *mITEM_INFO;

	DWORD mUpdateTimeForBroadcast;

	void SetSpaceIndex( void );

	void Update( int tObjectIndex );

	BOOL CheckPossibleGetItem( int tUserIndex );

};
//INSTANCE
extern ITEM_OBJECT mITEM_OBJECT[MAX_ITEM_OBJECT_NUM];
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_ZONEMAININFO
//-------------------------------------------------------------------------------------------------
//DEFINE
class ZONEMAININFO
{

private :

	int mZoneLevelInfo[MAX_ZONE_NUMBER_NUM][2];
	int mZoneTribeInfo[MAX_ZONE_NUMBER_NUM][2];

public :

	ZONEMAININFO( void );

	BOOL Init( void );
	void Free( void );

	int ReturnMinZoneLevelInfo( int tZoneNumber );
	int ReturnMaxZoneLevelInfo( int tZoneNumber );
	int ReturnZoneTribeInfo1( int tZoneNumber );
	int ReturnZoneTribeInfo2( int tZoneNumber );

};
//INSTANCE
extern ZONEMAININFO mZONEMAININFO;
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_ZONENPCINFO
//-------------------------------------------------------------------------------------------------
//STRUCTURE-[ZONENPCINFODATA]
typedef struct
{
	int mTotalNPCNum;
	int mNPCNumber[MAX_NPC_OBJECT_NUM];
	float mNPCCoord[MAX_NPC_OBJECT_NUM][3];
	float mNPCAngle[MAX_NPC_OBJECT_NUM];
}
ZONENPCINFODATA;
//DEFINE
class ZONENPCINFO
{

public :

	ZONENPCINFODATA mDATA[MAX_ZONE_NUMBER_NUM];

	ZONENPCINFO( void );

	BOOL Init( void );
	void Free( void );

	int ReturnZoneNumber( int tNPCNumber );
	int ReturnZoneCoord( int tNPCNumber, float tCoord[3] );

	BOOL CheckNPCFunction( float tCoord[3], int tMenuIndex );

};
//INSTANCE
extern ZONENPCINFO mZONENPCINFO;
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_HACKINFO
//-------------------------------------------------------------------------------------------------
//MACRO
#define MAX_HACK_INFO_NUM											10000
//DEFINE
class HACKINFO
{

private :

	//BOOL GetOneValueFromFile( HANDLE hFile, int tBufferSize, char *tBuffer );
	//BOOL GetOneValueFromFile(FILE *hFile, int tBufferSize, char *tBuffer );
	int GetOneValueFromFile(const char *pBuffer);


public :

	int mTotalHackInfoNum;
	int mHackSize[MAX_HACK_INFO_NUM];

	HACKINFO( void );

	BOOL Init( void );
	void Free( void );

	BOOL Check( int tHackSize );

};
//INSTANCE
extern HACKINFO mHACKINFO;
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------------
