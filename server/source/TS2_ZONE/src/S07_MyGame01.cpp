//-------------------------------------------------------------------------------------------------
//HEADER
//-------------------------------------------------------------------------------------------------
#include "H01_MainApplication.h"
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_GAME
//-------------------------------------------------------------------------------------------------
//INSTANCE
MyGame mGAME;
//CREATE
MyGame::MyGame( void )
{
	//mCompressLibrary = NULL;
}
//INIT
BOOL MyGame::Init( void )
{
	int index01;

    time_t tCountSeconds;
	struct tm *tPresentTime;

	if( !LoadForAutoCheckInfo() )
	{
		////MessageBox( NULL, "[Error::( !LoadForAutoCheckInfo() )]", mSERVER_INFO.mServerName, ( MB_OK | MB_SYSTEMMODAL ) );
		return FALSE;
	}

	if( !LoadForSellItemInfo() )
	{
		//MessageBox( NULL, "[Error::( !LoadForSellItemInfo() )]", mSERVER_INFO.mServerName, ( MB_OK | MB_SYSTEMMODAL ) );
		return FALSE;
	}
	
	mGeneralExperienceUpRatio = mSERVER_INFO.x_mGeneralExperienceUpRatio;
	mItemDropUpRatio = mSERVER_INFO.x_mItemDropUpRatio;
	mItemDropUpRatioForMyoung = mSERVER_INFO.x_mItemDropUpRatioForMyoung;
	mKillOtherTribeAddValue = mSERVER_INFO.x_mKillOtherTribeAddValue;
	mTeacherPointUpRatio = mSERVER_INFO.x_mTeacherPointUpRatio;
	mPatExperienceUpRatio = mSERVER_INFO.x_mPatExperienceUpRatio ;
	mBonusGainGeneralExperienceUpRatio = mSERVER_INFO.x_mBonusGainGeneralExperienceUpRatio;
	mGeneralExperienceDownRatio = mSERVER_INFO.x_mGeneralExperienceDownRatio;
	mKillOtherTribeExperienceUpRatio = mSERVER_INFO.x_mKillOtherTribeExperienceUpRatio;

	LOG_TO_BOTH_1("!mGeneralExperienceUpRatio = %f \n", mGeneralExperienceUpRatio);

	//@{ auto quiz
	mAutoUserMonsterCount = 2000;//GetPrivateProfileInt( "AUTO_CONTROL", "SEND_MONSTER_COUNT", 1000, "./CONFIG/AutoControl.INI" );
	//@}

	mAvatarObjectUniqueNumber = 1;
	mMonsterObjectUniqueNumber = 1;
	mItemObjectUniqueNumber = 1;
	mAvatarDuelUniqueNumber = 1;
	mAvatarPShopUniqueNumber = 1;
	mTickCount = 0;
	mUpdateTimeForTribeBankInfo = GetTickCount();
	mTribeBankInfo[0] = 0;
	mTribeBankInfo[1] = 0;
	mTribeBankInfo[2] = 0;
	mTribeBankInfo[3] = 0;
	mSymbolDamageUp[0] = 0.0f;
	mSymbolDamageUp[1] = 0.0f;
	mSymbolDamageUp[2] = 0.0f;
	mSymbolDamageUp[3] = 0.0f;
	mSymbolDamageDown[0] = 0.0f;
	mSymbolDamageDown[1] = 0.0f;
	mSymbolDamageDown[2] = 0.0f;
	mSymbolDamageDown[3] = 0.0f;
	AdjustSymbolDamageInfo();
	mCheckActiveBattleLogic = FALSE;
	mWorldInfo.mTribeVoteState[0] = 0;
	mWorldInfo.mTribeVoteState[1] = 0;
	mWorldInfo.mTribeVoteState[2] = 0;
	mWorldInfo.mTribeVoteState[3] = 0;

	mWorldInfo.mTribeGuardState[0][0] = 1;
	mWorldInfo.mTribeGuardState[0][1] = 1;
	mWorldInfo.mTribeGuardState[0][2] = 1;
	mWorldInfo.mTribeGuardState[0][3] = 1;
	mWorldInfo.mTribeGuardState[1][0] = 1;
	mWorldInfo.mTribeGuardState[1][1] = 1;
	mWorldInfo.mTribeGuardState[1][2] = 1;
	mWorldInfo.mTribeGuardState[1][3] = 1;
	mWorldInfo.mTribeGuardState[2][0] = 1;
	mWorldInfo.mTribeGuardState[2][1] = 1;
	mWorldInfo.mTribeGuardState[2][2] = 1;
	mWorldInfo.mTribeGuardState[2][3] = 1;
	mWorldInfo.mTribeGuardState[3][0] = 1;
	mWorldInfo.mTribeGuardState[3][1] = 1;
	mWorldInfo.mTribeGuardState[3][2] = 1;
	mWorldInfo.mTribeGuardState[3][3] = 1;

	mDuel_124 = FALSE;
	mDuel_124_RemainTime = 0;
	mDuel_124_AvatarNum[0] = 0;
	mDuel_124_AvatarNum[1] = 0;
	mDuel_124_Pvp = FALSE;

	switch( mSERVER_INFO.mServerNumber )
	{
	case  37 :
		mCheckTribeVoteServer = TRUE;
		break;
	default :
		mCheckTribeVoteServer = FALSE;
		break;
	}
	switch( mSERVER_INFO.mServerNumber )
	{
	case  37 :
		mCheckAllienceServer = TRUE;
		mAlliencePostTick = 0;
		mAllienceBattleState = 0;
		mAllienceRemainTime = 0;
		mAllienceBattlePostLocation[0][0] = -41.0f;
		mAllienceBattlePostLocation[0][1] = 8.0f;
		mAllienceBattlePostLocation[0][2] = -272.0f;
		mAllienceBattlePostLocation[1][0] = 35.0f;
		mAllienceBattlePostLocation[1][1] = 8.0f;
		mAllienceBattlePostLocation[1][2] = -272.0f;
		mAllienceBattlePostRadius[0] = 10.0f;
		mAllienceBattlePostRadius[1] = 10.0f;
		mAlliencePostAvatarIndex[0] = -1;
		mAlliencePostAvatarIndex[1] = -1;
		mAlliencePostAvatarUniqueNumber[0] = 0;
		mAlliencePostAvatarUniqueNumber[1] = 0;
		mAlliencePostAvatarTribe[0] = -1;
		mAlliencePostAvatarTribe[1] = -1;
		break;
	default :
		mCheckAllienceServer = FALSE;
		break;
	}
	switch( mSERVER_INFO.mServerNumber )
	{
	case  37 :
		mCheckZone037Server = TRUE;
		mZone037PostTick = 0;
		mZone037BattleState = 0;
		mZone037RemainTime = 0;
		break;
	default :
		mCheckZone037Server = FALSE;
		break;
	}
	switch( mSERVER_INFO.mServerNumber )
	{
	case 124 :
		mCheckZone124Server = TRUE;
		break;
	default :
		mCheckZone124Server = FALSE;
		break;
	}
	switch( mSERVER_INFO.mServerNumber )
	{
	case  71 :
		mCheckZone071TypeServer = TRUE;
		mZone071TypeZoneIndex = 0;
		break;
	case  72 :
		mCheckZone071TypeServer = TRUE;
		mZone071TypeZoneIndex = 1;
		break;
	case  73 :
		mCheckZone071TypeServer = TRUE;
		mZone071TypeZoneIndex = 2;
		break;
	default :
		mCheckZone071TypeServer = FALSE;
		break;
	}
	switch( mSERVER_INFO.mServerNumber )
	{
	case  49 :
		mCheckZone049TypeServer = TRUE;
		mZone049TypeZoneIndex = 0;
		mZone049TypePostTick = 0;
		mZone049TypeBattleState = 0;
		mZone049TypeRemainTime1 = 0;
		mZone049TypeRemainTime2 = 0;
		mZone049TypeBattleResult[0] = 0;
		mZone049TypeBattleResult[1] = 0;
		mZone049TypeBattleResult[2] = 0;
		mZone049TypeBattleResult[3] = 0;
		break;
	case 146 :
		mCheckZone049TypeServer = TRUE;
		mZone049TypeZoneIndex = 1;
		mZone049TypePostTick = 0;
		mZone049TypeBattleState = 0;
		mZone049TypeRemainTime1 = 0;
		mZone049TypeRemainTime2 = 0;
		mZone049TypeBattleResult[0] = 0;
		mZone049TypeBattleResult[1] = 0;
		mZone049TypeBattleResult[2] = 0;
		mZone049TypeBattleResult[3] = 0;
		break;
	case 149 :
		mCheckZone049TypeServer = TRUE;
		mZone049TypeZoneIndex = 2;
		mZone049TypePostTick = 0;
		mZone049TypeBattleState = 0;
		mZone049TypeRemainTime1 = 0;
		mZone049TypeRemainTime2 = 0;
		mZone049TypeBattleResult[0] = 0;
		mZone049TypeBattleResult[1] = 0;
		mZone049TypeBattleResult[2] = 0;
		mZone049TypeBattleResult[3] = 0;
		break;
	case 154 :
		if (mSERVER_INFO.m_iDeploymentStage >= 1) {
			mCheckZone049TypeServer = TRUE;
			mZone049TypeZoneIndex = 3;
			mZone049TypePostTick = 0;
			mZone049TypeBattleState = 0;
			mZone049TypeRemainTime1 = 0;
			mZone049TypeRemainTime2 = 0;
			mZone049TypeBattleResult[0] = 0;
			mZone049TypeBattleResult[1] = 0;
			mZone049TypeBattleResult[2] = 0;
			mZone049TypeBattleResult[3] = 0;
		} else mCheckZone049TypeServer = FALSE;
		break;
	case 157 :
		if (mSERVER_INFO.m_iDeploymentStage >= 1) {
			mCheckZone049TypeServer = TRUE;
			mZone049TypeZoneIndex = 4;
			mZone049TypePostTick = 0;
			mZone049TypeBattleState = 0;
			mZone049TypeRemainTime1 = 0;
			mZone049TypeRemainTime2 = 0;
			mZone049TypeBattleResult[0] = 0;
			mZone049TypeBattleResult[1] = 0;
			mZone049TypeBattleResult[2] = 0;
			mZone049TypeBattleResult[3] = 0;
		} else mCheckZone049TypeServer = FALSE;
		break;
	case 160 :
		if (mSERVER_INFO.m_iDeploymentStage >= 1) {
			mCheckZone049TypeServer = TRUE;
			mZone049TypeZoneIndex = 5;
			mZone049TypePostTick = 0;
			mZone049TypeBattleState = 0;
			mZone049TypeRemainTime1 = 0;
			mZone049TypeRemainTime2 = 0;
			mZone049TypeBattleResult[0] = 0;
			mZone049TypeBattleResult[1] = 0;
			mZone049TypeBattleResult[2] = 0;
			mZone049TypeBattleResult[3] = 0;
		} else mCheckZone049TypeServer = FALSE;
		break;
#ifdef __GOD__
	case 120 :
			mCheckZone049TypeServer = TRUE;
			mZone049TypeZoneIndex = 6;
			mZone049TypePostTick = 0;
			mZone049TypeBattleState = 0;
			mZone049TypeRemainTime1 = 0;
			mZone049TypeRemainTime2 = 0;
			mZone049TypeBattleResult[0] = 0;
			mZone049TypeBattleResult[1] = 0;
			mZone049TypeBattleResult[2] = 0;
			mZone049TypeBattleResult[3] = 0;
			break;
		case 121 :
			mCheckZone049TypeServer = TRUE;
			mZone049TypeZoneIndex = 7;
			mZone049TypePostTick = 0;
			mZone049TypeBattleState = 0;
			mZone049TypeRemainTime1 = 0;
			mZone049TypeRemainTime2 = 0;
			mZone049TypeBattleResult[0] = 0;
			mZone049TypeBattleResult[1] = 0;
			mZone049TypeBattleResult[2] = 0;
			mZone049TypeBattleResult[3] = 0;
			break;
		case 122 :
			mCheckZone049TypeServer = TRUE;
			mZone049TypeZoneIndex = 8;
			mZone049TypePostTick = 0;
			mZone049TypeBattleState = 0;
			mZone049TypeRemainTime1 = 0;
			mZone049TypeRemainTime2 = 0;
			mZone049TypeBattleResult[0] = 0;
			mZone049TypeBattleResult[1] = 0;
			mZone049TypeBattleResult[2] = 0;
			mZone049TypeBattleResult[3] = 0;
			break;
#endif
	default :
		mCheckZone049TypeServer = FALSE;
		break;
	}
	switch( mSERVER_INFO.mServerNumber )
	{
	case  51 :
		mCheckZone051TypeServer = TRUE;
		mZone051TypeZoneIndex = 0;
		mZone051TypePostTick = 0;
		mZone051TypeBattleState = 0;
		mZone051TypeRemainTime = 0;
		break;
	case 147 :
		mCheckZone051TypeServer = TRUE;
		mZone051TypeZoneIndex = 1;
		mZone051TypePostTick = 0;
		mZone051TypeBattleState = 0;
		mZone051TypeRemainTime = 0;
		break;
	case 150 :
		mCheckZone051TypeServer = TRUE;
		mZone051TypeZoneIndex = 2;
		mZone051TypePostTick = 0;
		mZone051TypeBattleState = 0;
		mZone051TypeRemainTime = 0;
		break;
	case 155 :
		if (mSERVER_INFO.m_iDeploymentStage >= 1) {
			mCheckZone051TypeServer = TRUE;
			mZone051TypeZoneIndex = 3;
			mZone051TypePostTick = 0;
			mZone051TypeBattleState = 0;
			mZone051TypeRemainTime = 0;
		} else mCheckZone051TypeServer = FALSE;
		break;
	case 158 :
		if (mSERVER_INFO.m_iDeploymentStage >= 1) {
			mCheckZone051TypeServer = TRUE;
			mZone051TypeZoneIndex = 4;
			mZone051TypePostTick = 0;
			mZone051TypeBattleState = 0;
			mZone051TypeRemainTime = 0;
		} else mCheckZone051TypeServer = FALSE;
		break;
	case 161 :
		if (mSERVER_INFO.m_iDeploymentStage >= 1) {
			mCheckZone051TypeServer = TRUE;
			mZone051TypeZoneIndex = 5;
			mZone051TypePostTick = 0;
			mZone051TypeBattleState = 0;
			mZone051TypeRemainTime = 0;
		} else mCheckZone051TypeServer = FALSE;
		break;
	default :
		mCheckZone051TypeServer = FALSE;
		break;
	}
	switch( mSERVER_INFO.mServerNumber )
	{
	case  53 :
		mCheckZone053TypeServer = TRUE;
		mZone053TypeZoneIndex = 0;
		mZone053TypePostTick = 0;
		mZone053TypeBattleState = 0;
		mZone053TypeRemainTime = 0;
		mZone053TypeBattlePostLocation[0] = 0.0f;
		mZone053TypeBattlePostLocation[1] = 0.0f;
		mZone053TypeBattlePostLocation[2] = 400.0f;
		mZone053TypeBattlePostRadius = 25.0f;
		mZone053TypePostAvatarIndex = -1;
		mZone053TypePostAvatarUniqueNumber = 0;
		mZone053TypeSaveTick = 0;
		break;
	case 148 :
		mCheckZone053TypeServer = TRUE;
		mZone053TypeZoneIndex = 1;
		mZone053TypePostTick = 0;
		mZone053TypeBattleState = 0;
		mZone053TypeRemainTime = 0;
		mZone053TypeBattlePostLocation[0] = 0.0f;
		mZone053TypeBattlePostLocation[1] = 0.0f;
		mZone053TypeBattlePostLocation[2] = 400.0f;
		mZone053TypeBattlePostRadius = 25.0f;
		mZone053TypePostAvatarIndex = -1;
		mZone053TypePostAvatarUniqueNumber = 0;
		mZone053TypeSaveTick = 0;
		break;
	case 151 :
		mCheckZone053TypeServer = TRUE;
		mZone053TypeZoneIndex = 2;
		mZone053TypePostTick = 0;
		mZone053TypeBattleState = 0;
		mZone053TypeRemainTime = 0;
		mZone053TypeBattlePostLocation[0] = 0.0f;
		mZone053TypeBattlePostLocation[1] = 0.0f;
		mZone053TypeBattlePostLocation[2] = 400.0f;
		mZone053TypeBattlePostRadius = 25.0f;
		mZone053TypePostAvatarIndex = -1;
		mZone053TypePostAvatarUniqueNumber = 0;
		mZone053TypeSaveTick = 0;
		break;
	case 152 :
		mCheckZone053TypeServer = TRUE;
		mZone053TypeZoneIndex = 3;
		mZone053TypePostTick = 0;
		mZone053TypeBattleState = 0;
		mZone053TypeRemainTime = 0;
		mZone053TypeBattlePostLocation[0] = 0.0f;
		mZone053TypeBattlePostLocation[1] = 0.0f;
		mZone053TypeBattlePostLocation[2] = 400.0f;
		mZone053TypeBattlePostRadius = 25.0f;
		mZone053TypePostAvatarIndex = -1;
		mZone053TypePostAvatarUniqueNumber = 0;
		mZone053TypeSaveTick = 0;
		break;
	case 153 :
		mCheckZone053TypeServer = TRUE;
		mZone053TypeZoneIndex = 4;
		mZone053TypePostTick = 0;
		mZone053TypeBattleState = 0;
		mZone053TypeRemainTime = 0;
		mZone053TypeBattlePostLocation[0] = 0.0f;
		mZone053TypeBattlePostLocation[1] = 0.0f;
		mZone053TypeBattlePostLocation[2] = 400.0f;
		mZone053TypeBattlePostRadius = 25.0f;
		mZone053TypePostAvatarIndex = -1;
		mZone053TypePostAvatarUniqueNumber = 0;
		mZone053TypeSaveTick = 0;
		break;
	case 156 :
		if (mSERVER_INFO.m_iDeploymentStage >= 1) {
			mCheckZone053TypeServer = TRUE;
			mZone053TypeZoneIndex = 5;
			mZone053TypePostTick = 0;
			mZone053TypeBattleState = 0;
			mZone053TypeRemainTime = 0;
			mZone053TypeBattlePostLocation[0] = 60.0f;
			mZone053TypeBattlePostLocation[1] = 106.0f;
			mZone053TypeBattlePostLocation[2] = 4190.0f;
			mZone053TypeBattlePostRadius = 25.0f;
			mZone053TypePostAvatarIndex = -1;
			mZone053TypePostAvatarUniqueNumber = 0;
			mZone053TypeSaveTick = 0;
		} else mCheckZone053TypeServer = FALSE;
		break;
	case 159 :
		if (mSERVER_INFO.m_iDeploymentStage >= 1) {
			mCheckZone053TypeServer = TRUE;
			mZone053TypeZoneIndex = 6;
			mZone053TypePostTick = 0;
			mZone053TypeBattleState = 0;
			mZone053TypeRemainTime = 0;
			mZone053TypeBattlePostLocation[0] = 60.0f;
			mZone053TypeBattlePostLocation[1] = 106.0f;
			mZone053TypeBattlePostLocation[2] = 4190.0f;
			mZone053TypeBattlePostRadius = 25.0f;
			mZone053TypePostAvatarIndex = -1;
			mZone053TypePostAvatarUniqueNumber = 0;
			mZone053TypeSaveTick = 0;
		} else mCheckZone053TypeServer = FALSE;
		break;
	case 162 :
		if (mSERVER_INFO.m_iDeploymentStage >= 1) {
			mCheckZone053TypeServer = TRUE;
			mZone053TypeZoneIndex = 7;
			mZone053TypePostTick = 0;
			mZone053TypeBattleState = 0;
			mZone053TypeRemainTime = 0;
			mZone053TypeBattlePostLocation[0] = 60.0f;
			mZone053TypeBattlePostLocation[1] = 106.0f;
			mZone053TypeBattlePostLocation[2] = 4190.0f;
			mZone053TypeBattlePostRadius = 25.0f;
			mZone053TypePostAvatarIndex = -1;
			mZone053TypePostAvatarUniqueNumber = 0;
			mZone053TypeSaveTick = 0;
		} else mCheckZone053TypeServer = FALSE;
		break;
	case 163 :
		if (mSERVER_INFO.m_iDeploymentStage >= 1) {
			mCheckZone053TypeServer = TRUE;
			mZone053TypeZoneIndex = 8;
			mZone053TypePostTick = 0;
			mZone053TypeBattleState = 0;
			mZone053TypeRemainTime = 0;
			mZone053TypeBattlePostLocation[0] = 60.0f;
			mZone053TypeBattlePostLocation[1] = 106.0f;
			mZone053TypeBattlePostLocation[2] = 4190.0f;
			mZone053TypeBattlePostRadius = 25.0f;
			mZone053TypePostAvatarIndex = -1;
			mZone053TypePostAvatarUniqueNumber = 0;
			mZone053TypeSaveTick = 0;
		} else mCheckZone053TypeServer = FALSE;
		break;
	case 164 :
		if (mSERVER_INFO.m_iDeploymentStage >= 1) {
			mCheckZone053TypeServer = TRUE;
			mZone053TypeZoneIndex = 9;
			mZone053TypePostTick = 0;
			mZone053TypeBattleState = 0;
			mZone053TypeRemainTime = 0;
			mZone053TypeBattlePostLocation[0] = 60.0f;
			mZone053TypeBattlePostLocation[1] = 106.0f;
			mZone053TypeBattlePostLocation[2] = 4190.0f;
			mZone053TypeBattlePostRadius = 25.0f;
			mZone053TypePostAvatarIndex = -1;
			mZone053TypePostAvatarUniqueNumber = 0;
			mZone053TypeSaveTick = 0;
		} else mCheckZone053TypeServer = FALSE;
		break;
	default :
		mCheckZone053TypeServer = FALSE;
		break;
	}
	switch( mSERVER_INFO.mServerNumber )
	{
	case  38 :
		mCheckZone038Server = TRUE;
		mZone038PostTick = 0;
		mZone038BattleState = 0;
		mZone038RemainTime = 0;
		mZone038BattlePostLocation[0] = -40.0f;
		mZone038BattlePostLocation[1] = 200.0f;
		mZone038BattlePostLocation[2] = 6379.0f;
		mZone038BattlePostRadius = 25.0f;
		mZone038PostAvatarIndex = -1;
		mZone038PostAvatarUniqueNumber = 0;
		break;
	default :
		mCheckZone038Server = FALSE;
		break;
	}
	switch( mSERVER_INFO.mServerNumber )
	{
		case  39 :
		case 144 :
		case 145 :
		case  74 :
			mCheckZone039TypeServer = TRUE;
			mZone039TypePostTick = 0;
			mZone039TypeBattleState = 0;
			break;
		default :
			mCheckZone039TypeServer = FALSE;
			break;
	}
	switch( mSERVER_INFO.mServerNumber )
	{
		case 101 :
		case 102 :
		case 103 :
		case 167 :
		case 104 :
		case 106 :
		case 108 :
		case 168 :
		case 105 :
		case 107 :
		case 109 :
		case 169 :
#ifdef __GOD__
		case 110 :
		case 111 :
		case 112 :
		case 113 :
		case 114 :
		case 115 :
		case 116 :
		case 117 :
		case 251 :
		case 252 :
		case 253 :
		case 254 :
		case 255 :
		case 256 :
		case 257 :
		case 258 :
		case 259 :
		case 260 :
		case 261 :
		case 262 :
		case 263 :
		case 264 :
		case 265 :
		case 266 :
#endif
			mCheckZone101TypeServer = TRUE;
			break;
		default :
			mCheckZone101TypeServer = FALSE;
			break;
	}

	switch( mSERVER_INFO.mServerNumber )
	{
	case 125 :
		mCheckZone125Server = TRUE;
		break;
	default :
		mCheckZone125Server = FALSE;
		break;
	}
	switch( mSERVER_INFO.mServerNumber )
	{
	case 126 :
	case 130 :
	case 134 :
	case 171 :
	case 127 :
	case 131 :
	case 135 :
  case 172 :
	case 128 :
	case 132 :
	case 136 :
	case 173 :
	case 129 :
	case 133 :
	case 137 :
	case 174 :
#ifdef __GOD__
	case 235 :
	case 236 :
	case 237 :
	case 238 :
	case 239 :
	case 240 :
	case 241 :
	case 242 :
	case 243 :
	case 244 :
	case 245 :
	case 246 :
	case 247 :
	case 248 :
	case 249 :
	case 250 :
	case 210 :
	case 211 :
	case 212 :
	case 213 :
	case 222 :
	case 223 :
	case 224 :
	case 225 :
	case 214 :
	case 215 :
	case 216 :
	case 217 :
	case 226 :
	case 227 :
	case 228 :
	case 229 :
	case 218 :
	case 219 :
	case 220 :
	case 221 :
	case 230 :
	case 231 :
	case 232 :
	case 233 :
#endif
		mCheckZone126TypeServer = TRUE;
		break;
	default :
		mCheckZone126TypeServer = FALSE;
		break;
	}
#ifdef __GOD__
	switch( mSERVER_INFO.mServerNumber )
	{
		case 222 :
		case 223 :
		case 224 :
		case 225 :
		case 226 :
		case 227 :
		case 228 :
		case 229 :
		case 230 :
		case 231 :
		case 232 :
		case 233 :
			mCheckZone126RealTypeServer = TRUE;
			break;
		default :
			mCheckZone126RealTypeServer = FALSE;
			break;
	}
#endif
	switch( mSERVER_INFO.mServerNumber )
	{
	case 175 :
        if (mSERVER_INFO.m_iDeploymentStage >= 2) {
		    mCheckZone175TypeServer = TRUE;
		    mZone175TypeZoneIndex1 = 0;
		    mZone175TypeZoneIndex2 = 0;
		    mZone175TypePostTick = 0;
		    mZone175TypeBattleState = 0;
		    mZone175TypeRemainTime = 0;
        } else mCheckZone175TypeServer = FALSE;
		break;
	case 176 :
        if (mSERVER_INFO.m_iDeploymentStage >= 2) {
		    mCheckZone175TypeServer = TRUE;
		    mZone175TypeZoneIndex1 = 1;
		    mZone175TypeZoneIndex2 = 0;
		    mZone175TypePostTick = 0;
		    mZone175TypeBattleState = 0;
		    mZone175TypeRemainTime = 0;
        } else mCheckZone175TypeServer = FALSE;
		break;
	case 177 :
        if (mSERVER_INFO.m_iDeploymentStage >= 2) {
		    mCheckZone175TypeServer = TRUE;
		    mZone175TypeZoneIndex1 = 2;
		    mZone175TypeZoneIndex2 = 0;
		    mZone175TypePostTick = 0;
		    mZone175TypeBattleState = 0;
		    mZone175TypeRemainTime = 0;
        } else mCheckZone175TypeServer = FALSE;
		break;
	case 178 :
        if (mSERVER_INFO.m_iDeploymentStage >= 2) {
		    mCheckZone175TypeServer = TRUE;
		    mZone175TypeZoneIndex1 = 0;
		    mZone175TypeZoneIndex2 = 1;
		    mZone175TypePostTick = 0;
		    mZone175TypeBattleState = 0;
		    mZone175TypeRemainTime = 0;
        } else mCheckZone175TypeServer = FALSE;
		break;
	case 179 :
        if (mSERVER_INFO.m_iDeploymentStage >= 2) {
		    mCheckZone175TypeServer = TRUE;
		    mZone175TypeZoneIndex1 = 1;
		    mZone175TypeZoneIndex2 = 1;
		    mZone175TypePostTick = 0;
		    mZone175TypeBattleState = 0;
		    mZone175TypeRemainTime = 0;
        } else mCheckZone175TypeServer = FALSE;
		break;
	case 180 :
        if (mSERVER_INFO.m_iDeploymentStage >= 2) {
		    mCheckZone175TypeServer = TRUE;
		    mZone175TypeZoneIndex1 = 2;
		    mZone175TypeZoneIndex2 = 1;
		    mZone175TypePostTick = 0;
		    mZone175TypeBattleState = 0;
		    mZone175TypeRemainTime = 0;
        } else mCheckZone175TypeServer = FALSE;
		break;
	case 181 :
        if (mSERVER_INFO.m_iDeploymentStage >= 2) {
		    mCheckZone175TypeServer = TRUE;
		    mZone175TypeZoneIndex1 = 3;
		    mZone175TypeZoneIndex2 = 1;
		    mZone175TypePostTick = 0;
		    mZone175TypeBattleState = 0;
		    mZone175TypeRemainTime = 0;
        } else mCheckZone175TypeServer = FALSE;
		break;
	case 182 :
        if (mSERVER_INFO.m_iDeploymentStage >= 2) {
		    mCheckZone175TypeServer = TRUE;
		    mZone175TypeZoneIndex1 = 0;
		    mZone175TypeZoneIndex2 = 2;
		    mZone175TypePostTick = 0;
		    mZone175TypeBattleState = 0;
		    mZone175TypeRemainTime = 0;
        } else mCheckZone175TypeServer = FALSE;
		break;
	case 183 :
        if (mSERVER_INFO.m_iDeploymentStage >= 2) {
		    mCheckZone175TypeServer = TRUE;
		    mZone175TypeZoneIndex1 = 1;
		    mZone175TypeZoneIndex2 = 2;
		    mZone175TypePostTick = 0;
		    mZone175TypeBattleState = 0;
		    mZone175TypeRemainTime = 0;
        } else mCheckZone175TypeServer = FALSE;
		break;
	case 184 :
        if (mSERVER_INFO.m_iDeploymentStage >= 2) {
		    mCheckZone175TypeServer = TRUE;
		    mZone175TypeZoneIndex1 = 2;
		    mZone175TypeZoneIndex2 = 2;
		    mZone175TypePostTick = 0;
		    mZone175TypeBattleState = 0;
		    mZone175TypeRemainTime = 0;
        } else mCheckZone175TypeServer = FALSE;
		break;
	case 185 :
        if (mSERVER_INFO.m_iDeploymentStage >= 2) {
		    mCheckZone175TypeServer = TRUE;
		    mZone175TypeZoneIndex1 = 3;
		    mZone175TypeZoneIndex2 = 2;
		    mZone175TypePostTick = 0;
		    mZone175TypeBattleState = 0;
		    mZone175TypeRemainTime = 0;
        } else mCheckZone175TypeServer = FALSE;
		break;
	case 186 :
        if (mSERVER_INFO.m_iDeploymentStage >= 2) {
		    mCheckZone175TypeServer = TRUE;
		    mZone175TypeZoneIndex1 = 0;
		    mZone175TypeZoneIndex2 = 3;
		    mZone175TypePostTick = 0;
		    mZone175TypeBattleState = 0;
		    mZone175TypeRemainTime = 0;
        } else mCheckZone175TypeServer = FALSE;
		break;
	case 187 :
        if (mSERVER_INFO.m_iDeploymentStage >= 2) {
		    mCheckZone175TypeServer = TRUE;
		    mZone175TypeZoneIndex1 = 1;
		    mZone175TypeZoneIndex2 = 3;
		    mZone175TypePostTick = 0;
		    mZone175TypeBattleState = 0;
		    mZone175TypeRemainTime = 0;
        } else mCheckZone175TypeServer = FALSE;
		break;
	case 188 :
        if (mSERVER_INFO.m_iDeploymentStage >= 2) {
		    mCheckZone175TypeServer = TRUE;
		    mZone175TypeZoneIndex1 = 2;
		    mZone175TypeZoneIndex2 = 3;
		    mZone175TypePostTick = 0;
		    mZone175TypeBattleState = 0;
		    mZone175TypeRemainTime = 0;
        } else mCheckZone175TypeServer = FALSE;
		break;
	case 189 :
        if (mSERVER_INFO.m_iDeploymentStage >= 2) {
		    mCheckZone175TypeServer = TRUE;
		    mZone175TypeZoneIndex1 = 3;
		    mZone175TypeZoneIndex2 = 3;
		    mZone175TypePostTick = 0;
		    mZone175TypeBattleState = 0;
		    mZone175TypeRemainTime = 0;
        } else mCheckZone175TypeServer = FALSE;
		break;
	case 190 :
        if (mSERVER_INFO.m_iDeploymentStage >= 2) {
		    mCheckZone175TypeServer = TRUE;
		    mZone175TypeZoneIndex1 = 0;
		    mZone175TypeZoneIndex2 = 4;
		    mZone175TypePostTick = 0;
		    mZone175TypeBattleState = 0;
		    mZone175TypeRemainTime = 0;
        } else mCheckZone175TypeServer = FALSE;
		break;
	case 191 :
        if (mSERVER_INFO.m_iDeploymentStage >= 2) {
		    mCheckZone175TypeServer = TRUE;
		    mZone175TypeZoneIndex1 = 1;
		    mZone175TypeZoneIndex2 = 4;
		    mZone175TypePostTick = 0;
		    mZone175TypeBattleState = 0;
		    mZone175TypeRemainTime = 0;
        } else mCheckZone175TypeServer = FALSE;
		break;
	case 192 :
        if (mSERVER_INFO.m_iDeploymentStage >= 2) {
		    mCheckZone175TypeServer = TRUE;
		    mZone175TypeZoneIndex1 = 2;
		    mZone175TypeZoneIndex2 = 4;
		    mZone175TypePostTick = 0;
		    mZone175TypeBattleState = 0;
		    mZone175TypeRemainTime = 0;
        } else mCheckZone175TypeServer = FALSE;
		break;
	case 193 :
        if (mSERVER_INFO.m_iDeploymentStage >= 2) {
		    mCheckZone175TypeServer = TRUE;
		    mZone175TypeZoneIndex1 = 3;
		    mZone175TypeZoneIndex2 = 4;
		    mZone175TypePostTick = 0;
		    mZone175TypeBattleState = 0;
		    mZone175TypeRemainTime = 0;
        } else mCheckZone175TypeServer = FALSE;
		break;
#ifdef __GOD__
	case  19 :
		mCheckZone175TypeServer = TRUE;
		mZone175TypeZoneIndex1 = 0;
		mZone175TypeZoneIndex2 = 5;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		mZone175TypeRemainTime = 0;
		break;
	case  25 :
		mCheckZone175TypeServer = TRUE;
		mZone175TypeZoneIndex1 = 0;
		mZone175TypeZoneIndex2 = 6;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		mZone175TypeRemainTime = 0;
		break;
	case  31 :
		mCheckZone175TypeServer = TRUE;
		mZone175TypeZoneIndex1 = 0;
		mZone175TypeZoneIndex2 = 7;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		mZone175TypeRemainTime = 0;
		break;
	case  20 :
		mCheckZone175TypeServer = TRUE;
		mZone175TypeZoneIndex1 = 1;
		mZone175TypeZoneIndex2 = 5;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		mZone175TypeRemainTime = 0;
		break;
	case  26 :
		mCheckZone175TypeServer = TRUE;
		mZone175TypeZoneIndex1 = 1;
		mZone175TypeZoneIndex2 = 6;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		mZone175TypeRemainTime = 0;
		break;
	case  32 :
		mCheckZone175TypeServer = TRUE;
		mZone175TypeZoneIndex1 = 1;
		mZone175TypeZoneIndex2 = 7;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		mZone175TypeRemainTime = 0;
		break;
	case  21 :
		mCheckZone175TypeServer = TRUE;
		mZone175TypeZoneIndex1 = 2;
		mZone175TypeZoneIndex2 = 5;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		mZone175TypeRemainTime = 0;
		break;
	case  27 :
		mCheckZone175TypeServer = TRUE;
		mZone175TypeZoneIndex1 = 2;
		mZone175TypeZoneIndex2 = 6;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		mZone175TypeRemainTime = 0;
		break;
	case  33 :
		mCheckZone175TypeServer = TRUE;
		mZone175TypeZoneIndex1 = 2;
		mZone175TypeZoneIndex2 = 7;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		mZone175TypeRemainTime = 0;
		break;
	case  34 :
		mCheckZone175TypeServer = TRUE;
		mZone175TypeZoneIndex1 = 3;
		mZone175TypeZoneIndex2 = 5;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		mZone175TypeRemainTime = 0;
		break;
	case  35 :
		mCheckZone175TypeServer = TRUE;
		mZone175TypeZoneIndex1 = 3;
		mZone175TypeZoneIndex2 = 6;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		mZone175TypeRemainTime = 0;
		break;
	case  36 :
		mCheckZone175TypeServer = TRUE;
		mZone175TypeZoneIndex1 = 3;
		mZone175TypeZoneIndex2 = 7;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		mZone175TypeRemainTime = 0;
		break;
#endif
	default :
		mCheckZone175TypeServer = FALSE;
		break;
	}
#ifdef __GOD__
	switch( mSERVER_INFO.mServerNumber )
	{
		case 194 :
			mCheckZone194Server = TRUE;
			mZone194PostTick = 0;
			mZone194BattleState = 0;
			mZone194RemainTime1 = 0;
			mZone194RemainTime2 = 0;
			mZone194BattleResult[0] = 0;
			mZone194BattleResult[1] = 0;
			mZone194BattleResult[2] = 0;
			mZone194BattleResult[3] = 0;
			break;
		default :
			mCheckZone194Server = FALSE;
			break;
	}
	switch( mSERVER_INFO.mServerNumber )
	{
		case 195 :
		case 196 :
		case 197 :
		case 198 :
		case 199 :
		case  85 :
		case  86 :
		case  87 :
			mCheckZone195TypeServer = TRUE;
			break;
		default :
			mCheckZone195TypeServer = FALSE;
			break;
	}	
	switch( mSERVER_INFO.mServerNumber )
	{
		case 267 :
			mCheckZone267TypeServer = TRUE;
			mZone267TypeZoneIndex = 0;
			mZone267TypePostTick = 0;
			mZone267TypeBattleState = 0;
			mZone267TypeRemainTime1 = 0;
			mZone267TypeRemainTime2 = 0;
			mZone267TypeBattleResult[0] = 0;
			mZone267TypeBattleResult[1] = 0;
			mZone267TypeBattleResult[2] = 0;
			mZone267TypeBattleResult[3] = 0;
			break;
		case 268 :
			mCheckZone267TypeServer = TRUE;
			mZone267TypeZoneIndex = 1;
			mZone267TypePostTick = 0;
			mZone267TypeBattleState = 0;
			mZone267TypeRemainTime1 = 0;
			mZone267TypeRemainTime2 = 0;
			mZone267TypeBattleResult[0] = 0;
			mZone267TypeBattleResult[1] = 0;
			mZone267TypeBattleResult[2] = 0;
			mZone267TypeBattleResult[3] = 0;
			break;
		case 269 :
			mCheckZone267TypeServer = TRUE;
			mZone267TypeZoneIndex = 2;
			mZone267TypePostTick = 0;
			mZone267TypeBattleState = 0;
			mZone267TypeRemainTime1 = 0;
			mZone267TypeRemainTime2 = 0;
			mZone267TypeBattleResult[0] = 0;
			mZone267TypeBattleResult[1] = 0;
			mZone267TypeBattleResult[2] = 0;
			mZone267TypeBattleResult[3] = 0;
			break;
		default :
			mCheckZone267TypeServer = FALSE;
			break;
	}
	switch(mSERVER_INFO.mServerNumber) {
		case 290 :
		case 291 :
		case 292 :
		case 293 :
		case 294 :
		case 295 :
		case 296 :
			mCheckZoneAllDuelTypeServer = true;
			break;
		default :
			mCheckZoneAllDuelTypeServer = false;
			break;
	}

	switch(mSERVER_INFO.mServerNumber) {
		case 290 :
		case 291 :
		case 292 :
		case 293 :
		case 294 :
		case 295 :
		case 296 :
			mCheckZoneDuelTypeServer = true;
			break;
		default :
			mCheckZoneDuelTypeServer = false;
			break;
	}

	switch(mSERVER_INFO.mServerNumber) {
		case 84 :
			mCheckZone084TypeServer = true;
			break;
		default :
			mCheckZone084TypeServer = false;
	}

	mWorldInfo.mZone049TypeState[mZone049TypeZoneIndex] = 0;
	mWorldInfo.mZone051TypeState[mZone051TypeZoneIndex] = 0;
	mWorldInfo.mZone053TypeState[mZone053TypeZoneIndex] = 0;
	mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 0;
#endif
	for( index01 = 0 ; index01 < MAX_AVATAR_OBJECT_NUM ; index01++ )
	{
		mAVATAR_OBJECT[index01].mIndex = index01;
        // # Defense Hack # {
    mAVATAR_OBJECT[index01].mCheckDeath = false;
		mAVATAR_OBJECT[index01].mCheckCount = 0;
        // # }
		mAVATAR_OBJECT[index01].mCheckValidState = FALSE;
	}
	for( index01 = 0 ; index01 < MAX_MONSTER_OBJECT_NUM ; index01++ )
	{
		mMONSTER_OBJECT[index01].mCheckValidState = FALSE;
		mMONSTER_OBJECT[index01].mInvalidTimeForSummon = GetTickCount();
#ifdef __GOD__
		memset(mMONSTER_OBJECT[index01].mAvatarName, 0, MAX_AVATAR_NAME_LENGTH);
#endif
	}
	for( index01 = 0 ; index01 < MAX_ITEM_OBJECT_NUM ; index01++ )
	{
		mITEM_OBJECT[index01].mCheckValidState = FALSE;
	}
	if( !mZONEMAININFO.Init() )
	{
		LOG_TO_FILE_1("![%s] : ZoneMainInfo error\n", __FUNCTION__);
		return FALSE;
	}	
	

	if( !mZONENPCINFO.Init() )
	{	
		LOG_TO_FILE_1("![%s] : ZoneNPCInfo error\n", __FUNCTION__);
		return FALSE;
	}
	
	
	if( !mHACKINFO.Init() )
	{	
		LOG_TO_FILE_1("![%s] : HackInfo error\n", __FUNCTION__);
		return FALSE;
	}
	
	
	if( !mLEVEL.Init( "/home/guitar/12sky/twelve-sky/server/service/bin/DATA/005_00001.IMG" ) )
	{
		LOG_TO_FILE_1("![%s] : Level ERROR\n", __FUNCTION__);
		return FALSE;
	}
	
	if( !mITEM.Init( "/home/guitar/12sky/twelve-sky/server/service/bin/DATA/005_00002.IMG" ) )
	{
		LOG_TO_FILE_1("![%s] : Item error_new \n", __FUNCTION__);
		return FALSE;
	}
	
	if( !mSKILL.Init( "/home/guitar/12sky/twelve-sky/server/service/bin/DATA/005_00003.IMG" ) )
	{	
		LOG_TO_FILE_1("![%s] : Skill error\n", __FUNCTION__);
		return FALSE;
	}

	if( !mMONSTER.Init( "/home/guitar/12sky/twelve-sky/server/service/bin/DATA/005_00004.IMG" ) )
	{	
		LOG_TO_FILE_1("![%s] : Monster error\n", __FUNCTION__);
		return FALSE;
	}

	if( !mNPC.Init( "/home/guitar/12sky/twelve-sky/server/service/bin/DATA/005_00005.IMG" ) )
	{
		LOG_TO_FILE_1("![%s] : NPC error\n", __FUNCTION__);
		return FALSE;
	}

	if( !mQUEST.Init( "/home/guitar/12sky/twelve-sky/server/service/bin/DATA/005_00006.IMG" ) )
	{	
		LOG_TO_FILE_1("![%s] : Quest error\n", __FUNCTION__);
		return FALSE;
	}

	if( !mPAT.Init() )
	{
		LOG_TO_FILE_1("![%s] : Pat error\n", __FUNCTION__);
		return FALSE;
	}


	if( !mWORLD.LoadWM() )
	{
		LOG_TO_FILE_1("![%s] : World error\n", __FUNCTION__);
		return FALSE;
	}

	

	mSUMMON.Init();
	mSUMMON.SummonGuard( FALSE, TRUE );
	mSUMMON.SummonGuard( TRUE, TRUE );
	mSUMMON.SummonTribeSymbol();

	time( &tCountSeconds );
	tPresentTime = localtime( &tCountSeconds );
	if(NULL == tPresentTime){
		LOG_TO_FILE("!INIT ERROR [localtime return NULL]\n");
		return FALSE;
	}
	mZone84Time = tPresentTime->tm_mday;

	/** 2009. 10. 27 : MonsterSymbol edit (wolf) */
    bAttackMonsterSymbol = true;
    /** 2009. 10. 27 : MonsterSymbol edit (wolf) */

    mPremiumServerExpirationCountDown = -1; // �����̾� ����(����) ����� ���� 5�� ī��Ʈ �ٿ�� ����. // @_Premium_Server_@

	return TRUE;
}
//FREE
void MyGame::Free( void )
{
	int index01;

	mSUMMON.Free();
	mWORLD.Free();
	mPAT.Free();
	mQUEST.Free();
	mNPC.Free();
	mMONSTER.Free();
	mSKILL.Free();
	mITEM.Free();
	mLEVEL.Free();
	mHACKINFO.Free();
	mZONENPCINFO.Free();
	mZONEMAININFO.Free();

	for( index01 = 0 ; index01 < MAX_ITEM_OBJECT_NUM ; index01++ )
	{
		mITEM_OBJECT[index01].mCheckValidState = FALSE;
	}
	for( index01 = 0 ; index01 < MAX_MONSTER_OBJECT_NUM ; index01++ )
	{
		mMONSTER_OBJECT[index01].mCheckValidState = FALSE;
		mMONSTER_OBJECT[index01].mInvalidTimeForSummon = GetTickCount();
	}
	for( index01 = 0 ; index01 < MAX_AVATAR_OBJECT_NUM ; index01++ )
	{
		mAVATAR_OBJECT[index01].mCheckValidState = FALSE;
	}
#ifdef __WIN32__
	if( mCompressLibrary != NULL )
	{
		FreeLibrary( mCompressLibrary );
		mCompressLibrary = NULL;
	}
#endif
}
//DECOMPRESS
BOOL MyGame::Decompress( DWORD tCompressSize, BYTE *tCompress, DWORD tOriginalSize, BYTE *tOriginal )
{
#ifdef __WIN32__
	if( mFUncompress( tOriginal, &tOriginalSize, tCompress, tCompressSize ) != 0 )
#else
	//tOriginalSize = 0;
	uLongf to = tOriginalSize;
	if( (uncompress( tOriginal, (uLongf *)&to, tCompress, tCompressSize )) != 0 )
#endif
	{
		return FALSE;
	}

	return TRUE;
}
//LOAD_FOR_AUTO_CHECK_INFO
BOOL MyGame::LoadForAutoCheckInfo( void )
{
	char tempString01[1000];
	DWORD tFileSize;
	DWORD nReadByte;
	int tAutoCheckAnswer1;
	int tAutoCheckAnswer2;
	int tAutoCheckDataSize;
	BYTE tAutoCheckData[1000];

	tAutoCheckAnswer1 = rand_mir() % 10;
	tAutoCheckAnswer2 = rand_mir() % 10;
#ifdef __WIN32__
	HANDLE hFile;
	sprintf( tempString01, "A01_AUTOCHECK\\%05d.IMG", ( tAutoCheckAnswer1 * 10 + tAutoCheckAnswer2 + 1 ) );
	hFile = CreateFile( tempString01, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE )
	{
		return FALSE;
	}
	tFileSize = GetFileSize( hFile, NULL );
	if( ( tFileSize == INVALID_FILE_SIZE ) || ( tFileSize > 1000 ) )
	{
		CloseHandle( hFile );
		return FALSE;
	}
	tAutoCheckDataSize = (int) tFileSize;
	if( !ReadFile( hFile, &tAutoCheckData[0], tFileSize, &nReadByte, NULL ) )
	{
		CloseHandle( hFile );
		return FALSE;
	}
	if( nReadByte != tFileSize )
	{
		CloseHandle( hFile );
		return FALSE;
	}
	if( !CloseHandle( hFile ) )
	{
		return FALSE;
	}
#else
	sprintf( tempString01, "/home/guitar/12sky/twelve-sky/server/service/bin/A01_AUTOCHECK/%05d.IMG", ( tAutoCheckAnswer1 * 10 + tAutoCheckAnswer2 + 1 ) );
	FILE *r_fp = NULL;
	struct stat file_stat;

	if(!(r_fp = fopen(tempString01, "r"))) {
		//LOG_TO_FILE_1("%d\n", 1);
		return FALSE;
	}
	if(fstat(fileno(r_fp), &file_stat) < 0) {
		//LOG_TO_FILE_1("%d\n", 2);
		fclose(r_fp);
		return FALSE;
	}

	nReadByte = 0;
	tFileSize = file_stat.st_size;
	tAutoCheckDataSize = (int) tFileSize;

	while(tFileSize > 0) {
		tFileSize -= fread(&tAutoCheckData[0], 1, tFileSize, r_fp);
	}

	if(nReadByte != tFileSize)  {
		fclose(r_fp);
		return FALSE;
	}
	fclose(r_fp);
	//if(r_fp)
#endif
	mAutoCheckAnswer1 = tAutoCheckAnswer1;
	mAutoCheckAnswer2 = tAutoCheckAnswer2;
	mAutoCheckDataSize = tAutoCheckDataSize;
	CopyMemory( &mAutoCheckData[0], &tAutoCheckData[0], 1000 );
	return TRUE;
}
//LOAD_FOR_SELL_ITEM_INFO
BOOL MyGame::LoadForSellItemInfo( void )
{
#ifdef __WIN32__
	int index01;
	HANDLE hFile;
	DWORD nReadByte;

	hFile = CreateFile( "SELL_ITEM_INFO.DAT", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE )
	{
		for( index01 = 0 ; index01 < MAX_SELL_ITEM_INFO_NUM ; index01++ )
		{
			mSellItemInfo[index01].mState = 0;
		}
		return TRUE;
	}
	if( !ReadFile( hFile, &mSellItemInfo[0], ( sizeof( SELL_ITEM_INFO ) * MAX_SELL_ITEM_INFO_NUM ), &nReadByte, NULL ) )
	{
		CloseHandle( hFile );
		return FALSE;
	}
	if( nReadByte != ( sizeof( SELL_ITEM_INFO ) * MAX_SELL_ITEM_INFO_NUM ) )
	{
		CloseHandle( hFile );
		return FALSE;
	}
	if( !CloseHandle( hFile ) )
	{
		return FALSE;
	}
#else
#if 0 //MIR
	int i = 0;
	int nRead = 0;
	FILE *r_fp = NULL;
	struct stat file_stat;

	if(!(r_fp = fopen("/home/guitar/12sky/twelve-sky/server/service/bin/DATA/SELL_ITEM_INFO.DAT", "r"))) {
		return FALSE;
	}
		
	while(1) {
		nRead = fread(mSellItemInfo + nReadByte, 1, 
					sizeof(SELL_ITEM_INFO) * MAX_SELL_ITEM_INFO_NUM, r_fp);
		if(nRead > 0) {
			nReadByte += nRead;
		} else break;
	}	

	if(nReadByte != ( sizeof( SELL_ITEM_INFO ) * MAX_SELL_ITEM_INFO_NUM ))  {
		fclose(r_fp);
		return FALSE;
	}
	if(r_fp)
		fclose(r_fp);
#endif
#endif
	return TRUE;
}
//SAVE_FOR_SELL_ITEM_INFO
void MyGame::SaveForSellItemInfo( void )
{
	DWORD nWriteByte;
#ifdef __WIN32__
	HANDLE hFile;
	hFile = CreateFile(	"SELL_ITEM_INFO.DAT", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE )
	{
		return;
	}
	if( !WriteFile( hFile, &mSellItemInfo[0], ( sizeof( SELL_ITEM_INFO ) * MAX_SELL_ITEM_INFO_NUM ), &nWriteByte, NULL ) )
	{
		CloseHandle( hFile );
		return;
	}
	if( nWriteByte != ( sizeof( SELL_ITEM_INFO ) * MAX_SELL_ITEM_INFO_NUM ) )
	{
		CloseHandle( hFile );
		return;
	}
	if( !CloseHandle( hFile ) )
	{
		return;
	}
#else
	int i = 0;
	nWriteByte = 0;
	FILE *w_fp = NULL;
	//struct stat file_stat;

	if(!(w_fp = fopen("/home/guitar/12sky/twelve-sky/server/service/bin/DATA/SELL_ITEM_INFO.DAT", "w"))) {
		return;
	}

	while((nWriteByte += fwrite(&mSellItemInfo[i], 1, sizeof(SELL_ITEM_INFO), w_fp)) > 0) {
		i++;
	}

	if(nWriteByte != ( sizeof( SELL_ITEM_INFO ) * MAX_SELL_ITEM_INFO_NUM ))  {
		fclose(w_fp);
		return;
	}
	if(w_fp)
		fclose(w_fp);
#endif
}
//PROCESS_FOR_VALID_AVATAR_INFO_WITH_ITEM
void MyGame::ProcessForValidAvatarInfoWithItem( AVATAR_INFO *tAvatarInfo )
{
	int index01;
	int index02;
	ITEM_INFO *tITEM_INFO;

	for( index01 = 0 ; index01 < MAX_EQUIP_SLOT_NUM ; index01++ )
	{
		tITEM_INFO = mITEM.Search( tAvatarInfo->aEquip[index01][0] );
		if( tITEM_INFO == NULL )
		{
			tAvatarInfo->aEquip[index01][0] = 0;
			tAvatarInfo->aEquip[index01][1] = 0;
			tAvatarInfo->aEquip[index01][2] = 0;
			tAvatarInfo->aEquip[index01][3] = 0;
			continue;
		}
		if( tITEM_INFO->iSort ==  2 )
		{
			if( ( tAvatarInfo->aEquip[index01][1] < 1 ) || ( tAvatarInfo->aEquip[index01][1] > MAX_ITEM_DUPLICATION_NUM ) )
			{
				tAvatarInfo->aEquip[index01][0] = 0;
				tAvatarInfo->aEquip[index01][1] = 0;
				tAvatarInfo->aEquip[index01][2] = 0;
				tAvatarInfo->aEquip[index01][3] = 0;
				continue;
			}
		}
	}
	if( tAvatarInfo->aMoney < 1 )
	{
		tAvatarInfo->aMoney = 0;
	}
	for( index01 = 0 ; index01 < 2 ; index01++ )
	{
		for( index02 = 0 ; index02 < MAX_INVENTORY_SLOT_NUM ; index02++ )
		{
			tITEM_INFO = mITEM.Search( tAvatarInfo->aInventory[index01][index02][0] );
			if( tITEM_INFO == NULL )
			{
				tAvatarInfo->aInventory[index01][index02][0] = 0;
				tAvatarInfo->aInventory[index01][index02][1] = 0;
				tAvatarInfo->aInventory[index01][index02][2] = 0;
				tAvatarInfo->aInventory[index01][index02][3] = 0;
				tAvatarInfo->aInventory[index01][index02][4] = 0;
				tAvatarInfo->aInventory[index01][index02][5] = 0;
				continue;
			}
			if( ( tAvatarInfo->aInventory[index01][index02][1] < 0 ) || ( tAvatarInfo->aInventory[index01][index02][1] > 7 ) || ( tAvatarInfo->aInventory[index01][index02][2] < 0 ) || ( tAvatarInfo->aInventory[index01][index02][2] > 7 ) )
			{
				tAvatarInfo->aInventory[index01][index02][0] = 0;
				tAvatarInfo->aInventory[index01][index02][1] = 0;
				tAvatarInfo->aInventory[index01][index02][2] = 0;
				tAvatarInfo->aInventory[index01][index02][3] = 0;
				tAvatarInfo->aInventory[index01][index02][4] = 0;
				tAvatarInfo->aInventory[index01][index02][5] = 0;
				continue;
			}
			if( tITEM_INFO->iSort ==  2 )
			{
				if( ( tAvatarInfo->aInventory[index01][index02][3] < 1 ) || ( tAvatarInfo->aInventory[index01][index02][3] > MAX_ITEM_DUPLICATION_NUM ) )
				{
					tAvatarInfo->aInventory[index01][index02][0] = 0;
					tAvatarInfo->aInventory[index01][index02][1] = 0;
					tAvatarInfo->aInventory[index01][index02][2] = 0;
					tAvatarInfo->aInventory[index01][index02][3] = 0;
					tAvatarInfo->aInventory[index01][index02][4] = 0;
					tAvatarInfo->aInventory[index01][index02][5] = 0;
					continue;
				}
			}
		}
	}
	if( tAvatarInfo->aTradeMoney < 1 )
	{
		tAvatarInfo->aTradeMoney = 0;
	}
	for( index01 = 0 ; index01 < MAX_TRADE_SLOT_NUM ; index01++ )
	{
		tITEM_INFO = mITEM.Search( tAvatarInfo->aTrade[index01][0] );
		if( tITEM_INFO == NULL )
		{
			tAvatarInfo->aTrade[index01][0] = 0;
			tAvatarInfo->aTrade[index01][1] = 0;
			tAvatarInfo->aTrade[index01][2] = 0;
			tAvatarInfo->aTrade[index01][3] = 0;
			continue;
		}
		if( tITEM_INFO->iSort ==  2 )
		{
			if( ( tAvatarInfo->aTrade[index01][1] < 1 ) || ( tAvatarInfo->aTrade[index01][1] > MAX_ITEM_DUPLICATION_NUM ) )
			{
				tAvatarInfo->aTrade[index01][0] = 0;
				tAvatarInfo->aTrade[index01][1] = 0;
				tAvatarInfo->aTrade[index01][2] = 0;
				tAvatarInfo->aTrade[index01][3] = 0;
				continue;
			}
		}
	}
	if( tAvatarInfo->aStoreMoney < 1 )
	{
		tAvatarInfo->aStoreMoney = 0;
	}
	for( index01 = 0 ; index01 < 2 ; index01++ )
	{
		for( index02 = 0 ; index02 < MAX_STORE_ITEM_SLOT_NUM ; index02++ )
		{
			tITEM_INFO = mITEM.Search( tAvatarInfo->aStoreItem[index01][index02][0] );
			if( tITEM_INFO == NULL )
			{
				tAvatarInfo->aStoreItem[index01][index02][0] = 0;
				tAvatarInfo->aStoreItem[index01][index02][1] = 0;
				tAvatarInfo->aStoreItem[index01][index02][2] = 0;
				tAvatarInfo->aStoreItem[index01][index02][3] = 0;
				continue;
			}
			if( tITEM_INFO->iSort ==  2 )
			{
				if( ( tAvatarInfo->aStoreItem[index01][index02][1] < 1 ) || ( tAvatarInfo->aStoreItem[index01][index02][1] > MAX_ITEM_DUPLICATION_NUM ) )
				{
					tAvatarInfo->aStoreItem[index01][index02][0] = 0;
					tAvatarInfo->aStoreItem[index01][index02][1] = 0;
					tAvatarInfo->aStoreItem[index01][index02][2] = 0;
					tAvatarInfo->aStoreItem[index01][index02][3] = 0;
					continue;
				}
			}
		}
	}
	for( index01 = 0 ; index01 < 3 ; index01++ )
	{
		for( index02 = 0 ; index02 < MAX_HOT_KEY_NUM ; index02++ )
		{
			if( tAvatarInfo->aHotKey[index01][index02][2] != 3 )
			{
				continue;
			}
			tITEM_INFO = mITEM.Search( tAvatarInfo->aHotKey[index01][index02][0] );
			if( tITEM_INFO == NULL )
			{
				tAvatarInfo->aHotKey[index01][index02][0] = 0;
				tAvatarInfo->aHotKey[index01][index02][1] = 0;
				tAvatarInfo->aHotKey[index01][index02][2] = 0;
				continue;
			}
			if( tITEM_INFO->iSort != 2 )
			{
				tAvatarInfo->aHotKey[index01][index02][0] = 0;
				tAvatarInfo->aHotKey[index01][index02][1] = 0;
				tAvatarInfo->aHotKey[index01][index02][2] = 0;
				continue;
			}
			if( ( tAvatarInfo->aHotKey[index01][index02][1] < 1 ) || ( tAvatarInfo->aHotKey[index01][index02][1] > MAX_ITEM_DUPLICATION_NUM ) )
			{
				tAvatarInfo->aHotKey[index01][index02][0] = 0;
				tAvatarInfo->aHotKey[index01][index02][1] = 0;
				tAvatarInfo->aHotKey[index01][index02][2] = 0;
				continue;
			}
		}
	}
	if( tAvatarInfo->uSaveMoney < 1 )
	{
		tAvatarInfo->uSaveMoney = 0;
	}
	for( index01 = 0 ; index01 < MAX_SAVE_ITEM_SLOT_NUM ; index01++ )
	{
		tITEM_INFO = mITEM.Search( tAvatarInfo->uSaveItem[index01][0] );
		if( tITEM_INFO == NULL )
		{
			tAvatarInfo->uSaveItem[index01][0] = 0;
			tAvatarInfo->uSaveItem[index01][1] = 0;
			tAvatarInfo->uSaveItem[index01][2] = 0;
			tAvatarInfo->uSaveItem[index01][3] = 0;
			continue;
		}
		if( tITEM_INFO->iSort ==  2 )
		{
			if( ( tAvatarInfo->uSaveItem[index01][1] < 1 ) || ( tAvatarInfo->uSaveItem[index01][1] > MAX_ITEM_DUPLICATION_NUM ) )
			{
				tAvatarInfo->uSaveItem[index01][0] = 0;
				tAvatarInfo->uSaveItem[index01][1] = 0;
				tAvatarInfo->uSaveItem[index01][2] = 0;
				tAvatarInfo->uSaveItem[index01][3] = 0;
				continue;
			}
		}
	}
}
//LOGIC
void MyGame::Logic( float dTime )
{
	//LOG_TO_FILE("LOGIC\n");
	int index01;
	DWORD tGetTickCount;
	time_t PostCheckTime;

	time_t tCountSeconds;
	struct tm *tPresentTime;

	mTickCount++;
	PostCheckTime = ::time(NULL);
	tGetTickCount = GetTickCount();
	
	/** 2009. 10. 27 : MonsterSymbol edit (wolf) */
    if( ( bAttackMonsterSymbol == false ) && ( tGetTickCount - dwAttackMonsterSymbolTime >= 7200000 ) )
    {
        BYTE tBroadcastInfoData[MAX_BROADCAST_INFO_SIZE];
        ZeroMemory( tBroadcastInfoData, MAX_BROADCAST_INFO_SIZE );

        bAttackMonsterSymbol = true;
        mCENTER_COM.U_DEMAND_BROADCAST_INFO( 114, &tBroadcastInfoData[0] );
    }
    /** 2009. 10. 27 : MonsterSymbol edit (wolf) */
#ifdef __PARTY_BUG_FIX__
	multimap<string, party_check_t>::iterator iter;
	pair<multimap<string, party_check_t>::iterator, multimap<string, party_check_t>::iterator> eq_rng;
#endif

	for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
	{
		if( !mUSER[index01].mCheckConnectState )
		{
			continue;
		}
		if( mUSER[index01].mCheckValidState )
		{
			if( ( tGetTickCount - mUSER[index01].mRegisterTime ) >= 180000 )
			{
				if( mUSER[index01].mMoveZoneResult == 0 )
				{
					mUSER[index01].mAvatarInfo.aLogoutInfo[0] = mSERVER_INFO.mServerNumber;
					mUSER[index01].mAvatarInfo.aLogoutInfo[1] = (int) mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0];
					mUSER[index01].mAvatarInfo.aLogoutInfo[2] = (int) mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[1];
					mUSER[index01].mAvatarInfo.aLogoutInfo[3] = (int) mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[2];
					mUSER[index01].mAvatarInfo.aLogoutInfo[4] = mAVATAR_OBJECT[index01].mDATA.aLifeValue;
					mUSER[index01].mAvatarInfo.aLogoutInfo[5] = mAVATAR_OBJECT[index01].mDATA.aManaValue;
					mPLAYUSER_COM.U_REGISTER_USER_FOR_ZONE_2_SEND( mUSER[index01].mPlayUserIndex, &mUSER[index01].uID[0], &mUSER[index01].mAvatarInfo, mUSER[index01].mEffectValue,
							&mUSER[index01].mAuthInfo);
					//LOG_TO_FILE_1("! ---> %d\n", mPLAYUSER_COM.mRecv_Result);
					if( mPLAYUSER_COM.mRecv_Result != 0 )
					{
						mUSER[index01].Quit(__FILE__, __FUNCTION__, __LINE__);
						continue;
					}
					mUSER[index01].mRegisterTime = tGetTickCount;
				}
            }
            // # Defense Hack # {
            mUSER[index01].mSpeedTickCount++;
            mTRANSFER.B_SPEED_CHECK(mUSER[index01].mSpeedTickCount);
            mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
            // # }
		}
		else
		{
			if( ( tGetTickCount - mUSER[index01].mConnectTime ) >= 300000 )
			{
				mUSER[index01].Quit(__FILE__, __FUNCTION__, __LINE__);
			}
		}
	}

    time( &tCountSeconds );
    tPresentTime = localtime( &tCountSeconds );

    // @_Premium_Server_@
    // �����̾� ����(����) ����� ���� 5���� ī��Ʈ �ϱ�.
    if ((mSERVER_INFO.mCheckPremiumServer == 1) && (tPresentTime != NULL)) {
        if ((tPresentTime->tm_hour == 23) && (tPresentTime->tm_min == 55)) {
            if (mPremiumServerExpirationCountDown < 0) {
                mPremiumExpirationCountDownDate = mUTIL.ReturnNowDate();
                mPremiumServerExpirationCountDown = 5; // ī��Ʈ 5������ ����.
            }
        }

        // 5�� ī��Ʈ ����.
        if ((mPremiumServerExpirationCountDown >= 0) && ((mTickCount % 120) == 0)) {
            for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ ) {
                if( !mUSER[index01].mCheckConnectState ) {
                    continue;
                }
                if( mUSER[index01].mCheckValidState ) {
                    if (mUSER[index01].uPremiumServerExpirationDate == mPremiumExpirationCountDownDate) {
                        mTRANSFER.B_AVATAR_CHANGE_INFO_2( 200, mPremiumServerExpirationCountDown);
                        mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
                    }
                }
            }
            --mPremiumServerExpirationCountDown;
            if (mPremiumServerExpirationCountDown < 0) {
                mPremiumServerExpirationCountDown = -1;
                mUSER[index01].uPremiumServerExpirationDate = 0; // ���� DB�� ���� update ������ �ʽ��ϴ�.
            }
        }
    } // @

	if(NULL != tPresentTime){
		if(mZone84Time != tPresentTime->tm_mday){
			for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
			{
				if( !mUSER[index01].mCheckConnectState )
				{
					continue;
				}
				if( mUSER[index01].mCheckValidState )
				{
					if( mUSER[index01].mMoveZoneResult == 0 ){
						mUSER[index01].mAvatarInfo.aZone84Time = mUSER[index01].mAvatarInfo.aZone84Time & 0xFFFFFFF0;
						mTRANSFER.B_AVATAR_CHANGE_INFO_2( 42, mUSER[index01].mAvatarInfo.aZone84Time);
						mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
					}
				}
			}
			mZone84Time = tPresentTime->tm_mday;
		}
	}
	
	for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
	{
		if( !mAVATAR_OBJECT[index01].mCheckValidState )
		{
			continue;
		}
		mAVATAR_OBJECT[index01].Update( dTime );
#ifdef USE_UDP
		if( ( tGetTickCount - mAVATAR_OBJECT[index01].mUpdateTimeForBroadcast ) < 2000 )
        //if( ( tGetTickCount - mAVATAR_OBJECT[index01].mUpdateTimeForBroadcast ) < 5000 )
#else
        if( ( tGetTickCount - mAVATAR_OBJECT[index01].mUpdateTimeForBroadcast ) < 5000 )
#endif
		{
			continue;
		}
		if( mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0 )
		{
			mAVATAR_OBJECT[index01].mUpdateTimeForBroadcast = tGetTickCount;
			continue;
		}
		if( mAVATAR_OBJECT[index01].mDATA.aAction.aSort == 12 )
		{
			mAVATAR_OBJECT[index01].mUpdateTimeForBroadcast = tGetTickCount;
			continue;
		}
#ifdef __PARTY_BUG_FIX__
		eq_rng = mGAME.map_party_check.equal_range(mUSER[index01].mAvatarInfo.aPartyName);
		for(iter = eq_rng.first; iter != eq_rng.second; ++iter) {
			if((mTickCount % 4) == 0) {
			//	LOG_TO_FILE_4("%d %s %s %s\n", iter->second.tSort, iter->first.c_str(), iter->second.aName, mUSER[index01].mAvatarInfo.aPartyName);
				switch(iter->second.tSort) {
					case 106 :   //PARTY_LEAVE
						mTRANSFER.B_PARTY_LEAVE_RECV(iter->second.aName, 0);
						mUSER[index01].Send(TRUE, (char *)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);

						if(::strcmp(mUSER[index01].mAvatarInfo.aName, iter->second.aName) == 0) {
							strcpy(mUSER[index01].mAvatarInfo.aPartyName, "" );
							strcpy(mAVATAR_OBJECT[index01].mDATA.aPartyName, "" );
						}
						LOG_TO_FILE_2("!Logic party leave %s, to %s\n", iter->second.aName, mUSER[index01].mAvatarInfo.aName);
						break; 
					case 107 :   //PARTY_EXILE
						mTRANSFER.B_PARTY_EXILE_RECV(iter->second.aName, 0);
						mUSER[index01].Send(TRUE, (char *)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);

						if(::strcmp(mUSER[index01].mAvatarInfo.aName, iter->second.aName) == 0) {
							::strcpy(mUSER[index01].mAvatarInfo.aPartyName, "");
							::strcpy(mAVATAR_OBJECT[index01].mDATA.aPartyName, "");
						}
						LOG_TO_FILE_2("!Logic party exile %s, to %s\n", iter->second.aName, mUSER[index01].mAvatarInfo.aName);
						break;
					case 109 :   //PARTY_BREAK
						mTRANSFER.B_PARTY_BREAK_RECV(iter->second.aName, 0);
						mUSER[index01].Send(TRUE, (char *)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);

						::strncpy(mUSER[index01].mAvatarInfo.aPartyName, iter->second.aName, MAX_PARTY_NAME_LENGTH );
						::strncpy(mAVATAR_OBJECT[index01].mDATA.aPartyName, iter->second.aName, MAX_PARTY_NAME_LENGTH );
						LOG_TO_FILE_2("!Logic party break %s, to %s\n", iter->second.aName, mUSER[index01].mAvatarInfo.aName);
						break;
				}
			}
		}
		iter = mGAME.map_party_check.begin();
		while(iter != mGAME.map_party_check.end()) {
			if(::strcmp(iter->second.aName, mUSER[index01].mAvatarInfo.aName) == 0 && ((mTickCount % 4) == 0)) {
				switch(iter->second.tSort) {
					case 106 :   //PARTY_LEAVE
						mTRANSFER.B_PARTY_LEAVE_RECV(iter->second.aName, 0);
						mUSER[index01].Send(TRUE, (char *)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);

						if(::strcmp(mUSER[index01].mAvatarInfo.aName, iter->second.aName) == 0) {
							strcpy(mUSER[index01].mAvatarInfo.aPartyName, "" );
							strcpy(mAVATAR_OBJECT[index01].mDATA.aPartyName, "" );
						}
						LOG_TO_FILE_2("!Logic2 party leave %s, to %s\n", iter->second.aName, mUSER[index01].mAvatarInfo.aName);
						break; 
					case 107 :   //PARTY_EXILE
						mTRANSFER.B_PARTY_EXILE_RECV(iter->second.aName, 0);
						mUSER[index01].Send(TRUE, (char *)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);

						if(::strcmp(mUSER[index01].mAvatarInfo.aName, iter->second.aName) == 0) {
							::strcpy(mUSER[index01].mAvatarInfo.aPartyName, "");
							::strcpy(mAVATAR_OBJECT[index01].mDATA.aPartyName, "");
						}
						LOG_TO_FILE_2("!Logic2 party exile %s, to %s\n", iter->second.aName, mUSER[index01].mAvatarInfo.aName);
						break;
					case 109 :   //PARTY_BREAK
						mTRANSFER.B_PARTY_BREAK_RECV(iter->second.aName, 0);
						mUSER[index01].Send(TRUE, (char *)&mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);

						::strncpy(mUSER[index01].mAvatarInfo.aPartyName, iter->second.aName, MAX_PARTY_NAME_LENGTH );
						::strncpy(mAVATAR_OBJECT[index01].mDATA.aPartyName, iter->second.aName, MAX_PARTY_NAME_LENGTH );
						LOG_TO_FILE_2("!Logic2 party brake %s, to %s\n", iter->second.aName, mUSER[index01].mAvatarInfo.aName);
						break;
				}
			}
			if((PostCheckTime - iter->second.RegDateTime) >= 60) {
				mGAME.map_party_check.erase(iter++);
			} else {
				++iter;
			}
		}
#endif
        // # Defense Hack # {
        if (mAVATAR_OBJECT[index01].mCheckCount < 0) {
            mAVATAR_OBJECT[index01].mCheckCount = 0;
        } // # }
		mTRANSFER.B_AVATAR_ACTION_RECV2( index01, mAVATAR_OBJECT[index01].mUniqueNumber, &mAVATAR_OBJECT[index01].mDATA, 2 );
        mUTIL.Broadcast( TRUE, &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, index01 , true, 0);

		if((::time(NULL) - mUSER[index01].mHeartCheckTime) > 35) {
			LOG_TO_FILE_1("!mHeartCheckTime : %d\n", (::time(NULL) - mUSER[index01].mHeartCheckTime));
			mUSER[index01].Quit(__FILE__, __FUNCTION__, __LINE__);
		}

	}

	//LOG_TO_FILE_1("mMONSTER_OBJECT[index01].mSpecialSortNumber0 : %d\n", MAX_MONSTER_OBJECT_NUM);
	for( index01 = 0 ; index01 < MAX_MONSTER_OBJECT_NUM ; index01++ )
	{
		if( !mMONSTER_OBJECT[index01].mCheckValidState )
		{
			continue;
		}
		mMONSTER_OBJECT[index01].Update( index01, dTime );
		//LOG_TO_FILE_3("mMONSTER_OBJECT[index01].mSpecialSortNumber1 : %d %d %d\n", tGetTickCount, mMONSTER_OBJECT[index01].mUpdateTimeForBroadcast, ( tGetTickCount - mMONSTER_OBJECT[index01].mUpdateTimeForBroadcast ));
		if( ( tGetTickCount - mMONSTER_OBJECT[index01].mUpdateTimeForBroadcast ) < 5000 )
		{
			continue;
		}
		if( mMONSTER_OBJECT[index01].mDATA.mAction.aSort == 12 )
		{
			mMONSTER_OBJECT[index01].mUpdateTimeForBroadcast = tGetTickCount;
			continue;
		}
		mTRANSFER.B_MONSTER_ACTION_RECV( index01, mMONSTER_OBJECT[index01].mUniqueNumber, &mMONSTER_OBJECT[index01].mDATA, 2 );
		//-----------------------------------------------------------------//
		//[01].�Ϲ�,[02].����,[03].��ȣ��,[04].���ͼ�,[05].��ȣ��,[06].����//
		//-----------------------------------------------------------------//
		switch( mMONSTER_OBJECT[index01].mSpecialSortNumber )
		{
		case 1 :
            mUTIL.Broadcast( TRUE, &mMONSTER_OBJECT[index01].mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS1, false, 0 );
			break;
		case 2 :
            mUTIL.Broadcast( TRUE, &mMONSTER_OBJECT[index01].mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS3, false, 0 );
			break;
		case 3 :
            mUTIL.Broadcast( TRUE, &mMONSTER_OBJECT[index01].mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS3, false, 0 );
			break;
		case 4 :
            mUTIL.Broadcast( TRUE, &mMONSTER_OBJECT[index01].mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS3, false, 0 );
			break;
		case 5 :
            mUTIL.Broadcast( TRUE, &mMONSTER_OBJECT[index01].mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS2, false, 0 );
			break;
		case 6 :
            mUTIL.Broadcast( TRUE, &mMONSTER_OBJECT[index01].mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS3, false, 0 );
			break;
		}
	}
	for( index01 = 0 ; index01 < MAX_ITEM_OBJECT_NUM ; index01++ )
	{
		if( !mITEM_OBJECT[index01].mCheckValidState )
		{
			continue;
		}
		mITEM_OBJECT[index01].Update( index01 );
		if( ( tGetTickCount - mITEM_OBJECT[index01].mUpdateTimeForBroadcast ) < 5000 )
		{
			continue;
		}
		mTRANSFER.B_ITEM_ACTION_RECV2( index01, mITEM_OBJECT[index01].mUniqueNumber, &mITEM_OBJECT[index01].mDATA, 2 );
        mUTIL.Broadcast( TRUE, &mITEM_OBJECT[index01].mDATA.iLocation[0], UNIT_SCALE_RADIUS1, false, 0 );
	}
	if( ( tGetTickCount - mUpdateTimeForTribeBankInfo ) >= 600000 )
	{
		mUpdateTimeForTribeBankInfo = tGetTickCount;
		mPLAYUSER_COM.U_TRIBE_BANK_INFO_SAVE_SEND( &mTribeBankInfo[0] );
		mTribeBankInfo[0] = 0;
		mTribeBankInfo[1] = 0;
		mTribeBankInfo[2] = 0;
		mTribeBankInfo[3] = 0;
	}
	AdjustSymbolDamageInfo();
	ProcessForGuardState();
	//LOG_TO_FILE_1("-----------> %d\n", mCheckTribeVoteServer);
	if( mCheckTribeVoteServer )
	{
		Process_TribeVote_Server();
	}
	if( mCheckAllienceServer )
	{
		Process_Allience_Server();
	}
	if( mCheckZone037Server )
	{
		Process_Zone_037();
        // @ Cost Info Download @
        CheckUpdateCashItemInfo();
        // @
	}
	else if( mCheckZone124Server )
	{
		Process_Zone_124();
	}
	else if( mCheckZone071TypeServer )
	{
		Process_Zone_071_TYPE();
	}
	else if( mCheckZone049TypeServer )
	{	
		Process_Zone_049_TYPE();
	}
	else if( mCheckZone051TypeServer )
	{
		Process_Zone_051_TYPE();
	}
	else if( mCheckZone053TypeServer )
	{
		Process_Zone_053_TYPE();
	}
	else if( mCheckZone038Server )
	{
		Process_Zone_038();
		if( ( mTickCount % 20 ) == 0 )
		{
			mSUMMON.SummonGuard( FALSE, FALSE );
			mSUMMON.SummonGuard( TRUE, FALSE );
		}
	}
	else if( mCheckZone039TypeServer )
	{
		Process_Zone_039_TYPE();
		if( ( mTickCount % 20 ) == 0 )
		{
			mSUMMON.SummonMonster();
			mSUMMON.SummonBossMonster();
			mSUMMON.SummonTribeSymbol();
		}
	}
	else if( mCheckZone175TypeServer )
	{
		Process_Zone_175_TYPE();
	}
#ifdef __GOD__
	else if( mCheckZone194Server )
	{
		Process_Zone_194();
	}
	else if( mCheckZone267TypeServer )
	{
		Process_Zone_267_TYPE();
	}
#endif
	else
	{
		if( ( mTickCount % 20 ) == 0 )
		{
			mSUMMON.SummonMonster();
			mSUMMON.SummonBossMonster();
			mSUMMON.SummonGuard( FALSE, FALSE );
			mSUMMON.SummonTribeSymbol();
		}
	}
	if( (mTickCount >= 480) && ( mTickCount % 120 ) == 0 )
	{
		LoadForAutoCheckInfo();
		mPLAYUSER_COM.U_ZONE_OK_FOR_PLAYUSER_SEND();
		//LOG_TO_FILE("!TICK check 120 - playeruser\n");
		mCENTER_COM.U_ZONE_OK_FOR_CENTER_SEND();
		//LOG_TO_FILE("!TICK check 120 - center\n");
		mRELAY_COM.U_ZONE_OK_FOR_RELAY_SEND();
		//LOG_TO_FILE("!TICK check 120 - relay\n");
		mEXTRA_COM.U_ZONE_OK_FOR_EXTRA_SEND();
		//LOG_TO_FILE("!TICK check 120 - extra\n");
	}

    // @ Cost Info Download @
    if ((mTickCount % 20) == 0)
    {
        if (mCENTER_COM.IsUpdateCashItemInfo() == true)
        {
            if (mEXTRA_COM.U_GET_CASH_ITEM_INFO_FOR_EXTRA_SEND() == TRUE)
            {
                mCENTER_COM.SetUpdateCashItemInfo(false);
                mTRANSFER.B_UPDATE_CASH_ITEM_INFO();

                for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
                {
                    if( !mAVATAR_OBJECT[index01].mCheckValidState )
                    {
                        continue;
                    }
                    if( mUSER[index01].mMoveZoneResult == 1 )
                    {
                        continue;
                    }
                    if( mUSER[index01].mCheckValidState )
                    {
                        mUSER[index01].mIsValidCashItemInfo = false;
                        mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
                    }
                }
            }
            else
            {
                LOG_TO_FILE("!ERROR - GET_CASH_ITEM_INFO\n");
            }
        }
    } // @
}

void MyGame::HealthCheck(void)
{
	if( ( mTickCount % 120 ) == 0 ) {
		mPLAYUSER_COM.U_ZONE_OK_FOR_PLAYUSER_SEND();
		mCENTER_COM.U_ZONE_OK_FOR_CENTER_SEND();
		mRELAY_COM.U_ZONE_OK_FOR_RELAY_SEND();
		mEXTRA_COM.U_ZONE_OK_FOR_EXTRA_SEND();
	}
}

//RETURN_TRIBE_ROLE
int MyGame::ReturnTribeRole( char tAvatarName[MAX_AVATAR_NAME_LENGTH], int tTribe )
{
	int index01;

	if( strcmp( tAvatarName, mTribeInfo.mTribeMaster[tTribe] ) == 0 )
	{
		return 1;
	}
	for( index01 = 0 ; index01 < MAX_TRIBE_SUBMASTER_NUM ; index01++ )
	{
		if( strcmp( tAvatarName, mTribeInfo.mTribeSubMaster[tTribe][index01] ) == 0 )
		{
			return 2;
		}
	}
	return 0;
}
//RETURN_TRIBE_SUB_MASTER_NUM
int MyGame::ReturnTribeSubMasterNum( int tTribe )
{
	int index01;
	int index02;

	index02 = 0;
	for( index01 = 0 ; index01 < MAX_TRIBE_SUBMASTER_NUM ; index01++ )
	{
		if( strcmp( mTribeInfo.mTribeSubMaster[tTribe][index01], "" ) != 0 )
		{
			index02++;
		}
	}
	return index02;
}
//ADD_TRIBE_BANK_INFO
void MyGame::AddTribeBankInfo( int tTribe, int tMoney )
{
	if( ( tTribe < 0 ) || ( tTribe > 3 ) )
	{
		return;
	}
	if( mUTIL.CheckOverMaximum( mTribeBankInfo[tTribe], tMoney ) )
	{
		return;
	}
	mTribeBankInfo[tTribe] += tMoney;
}
//RETURN_TRIBE_FOR_SYMBOL
int MyGame::ReturnTribeForSymbol( int tTribe )
{
	switch( tTribe )
	{
	case 0 : return mWorldInfo.mTribe1Symbol;
	case 1 : return mWorldInfo.mTribe2Symbol;
	case 2 : return mWorldInfo.mTribe3Symbol;
	case 3 : return mWorldInfo.mTribe4Symbol;
	}
	return -1;
}
//RETURN_ALLIANCE_TRIBE
// �Լ��� ȣ���ϴ� ������ ������� ���ͼ����� �����ϴ� �Լ��� ������ �ۼ��Ͽ� ReturnAllianceTribe �Լ��� �����մϴ�.
int MyGame::ReturnAllianceTribe( int tTribe )
{
    if((49 == mSERVER_INFO.mServerNumber) || (51 == mSERVER_INFO.mServerNumber) || (53 == mSERVER_INFO.mServerNumber)
            || (146 == mSERVER_INFO.mServerNumber) || (147 == mSERVER_INFO.mServerNumber) || (148 == mSERVER_INFO.mServerNumber)
            || (149 == mSERVER_INFO.mServerNumber) || (150 == mSERVER_INFO.mServerNumber) || (151 == mSERVER_INFO.mServerNumber)
            || (152 == mSERVER_INFO.mServerNumber) || (153 == mSERVER_INFO.mServerNumber)){
        return -1;
    }

	return ReturnAlliance(tTribe);
}
//RETURN_TRIBE_POINT_WITH_ALLIANCE
int MyGame::ReturnTribePointWithAlliance( int tTribe )
{
	int tAllianceTribe;

	tAllianceTribe = ReturnAllianceTribe( tTribe );
	switch( tTribe )
	{
	case 0 :
		if( tAllianceTribe == -1 )
		{
			return mWorldInfo.mTribePoint[0];
		}
		else
		{
			return ( mWorldInfo.mTribePoint[0] + mWorldInfo.mTribePoint[tAllianceTribe] );
		}
		break;
	case 1 :
		if( tAllianceTribe == -1 )
		{
			return mWorldInfo.mTribePoint[1];
		}
		else
		{
			return ( mWorldInfo.mTribePoint[1] + mWorldInfo.mTribePoint[tAllianceTribe] );
		}
		break;
	case 2 :
		if( tAllianceTribe == -1 )
		{
			return mWorldInfo.mTribePoint[2];
		}
		else
		{
			return ( mWorldInfo.mTribePoint[2] + mWorldInfo.mTribePoint[tAllianceTribe] );
		}
		break;
	case 3 :
		if( tAllianceTribe == -1 )
		{
			return mWorldInfo.mTribePoint[3];
		}
		else
		{
			return ( mWorldInfo.mTribePoint[3] + mWorldInfo.mTribePoint[tAllianceTribe] );
		}
		break;
	}
	return 0;
}
//CHECK_POSSIBLE_CHANGE_TO_TRIBE_4
int MyGame::CheckPossibleChangeToTribe4( int tTribe )
{
	int index01;
	int tBigTribe;
	int tSmallTribe;

	tBigTribe = 0;
	for( index01 = 1 ; index01 < 4 ; index01++ )
	{
		if( ReturnTribePointWithAlliance( index01 ) >= ReturnTribePointWithAlliance( tBigTribe ) )
		{
			tBigTribe = index01;
		}
	}
	if( tBigTribe == 3 )
	{
		return 1;
	}
	tSmallTribe = 0;
	for( index01 = 1 ; index01 < 4 ; index01++ )
	{
		if( ReturnTribePointWithAlliance( index01 ) <= ReturnTribePointWithAlliance( tSmallTribe ) )
		{
			tSmallTribe = index01;
		}
	}
	if( tTribe == tSmallTribe )
	{
		return 2;
	}
	return 0;
}
//CHECK_POSSIBLE_CHANGE_TO_TRIBE_4_2
int MyGame::CheckPossibleChangeToTribe4_2( int tTribe )
{
	int index01;
	int tBigTribe;

	tBigTribe = 0;
	for( index01 = 1 ; index01 < 4 ; index01++ )
	{
		if( mWorldInfo.mTribePoint[index01] >= mWorldInfo.mTribePoint[tBigTribe] )
		{
			tBigTribe = index01;
		}
	}
	if( tBigTribe == 3 )
	{
		return 1;
	}
	if(tTribe != tBigTribe){
		return 2;
	}
	return 0;
}
//CHECK_SMALL_TRIBE_FOR_ADVANTAGE
BOOL MyGame::CheckSmallTribeForAdvantage( int tTribe )
{
	int index01;
	int tSmallTribePointTribe;

	tSmallTribePointTribe = -1;
	for( index01 = 0 ; index01 < 4 ; index01++ )
	{
		if( ReturnTribePointWithAlliance( index01 ) < 10 )
		{
			continue;
		}
		if( tSmallTribePointTribe == -1 )
		{
			tSmallTribePointTribe = index01;
		}
		else
		{
			if( ReturnTribePointWithAlliance( index01 ) < ReturnTribePointWithAlliance ( tSmallTribePointTribe ) )
			{
				tSmallTribePointTribe = index01;
			}
		}
	}
	if( tSmallTribePointTribe == tTribe )
	{
		return TRUE;
	}
	return FALSE;
}
#ifdef __GOD__
//RETURN_BIG_TRIBE
int MyGame::ReturnBigTribe( void )
{
	int index01;
	int tBigTribePointTribe;

	tBigTribePointTribe = 0;
	for( index01 = 1 ; index01 < 4 ; index01++ )
	{
		if( mWorldInfo.mTribePoint[index01] > mWorldInfo.mTribePoint[tBigTribePointTribe] )
		{
			tBigTribePointTribe = index01;
		}
	}
	return tBigTribePointTribe;
}
#endif
// @ Tribe Formation Skill @
//RETURN_SMALL_TRIBE
int MyGame::ReturnSmallTribe( void )
{
    int index01;
    int tSmallTribePointTribe;

    tSmallTribePointTribe = 0;
    for( index01 = 1 ; index01 < 4 ; index01++ )
    {
        if( mWorldInfo.mTribePoint[index01] < mWorldInfo.mTribePoint[tSmallTribePointTribe] )
        {
            tSmallTribePointTribe = index01;
        }
    }
    return tSmallTribePointTribe;
} // @
//ADJUST_SYMBOL_DAMAGE_INFO
void MyGame::AdjustSymbolDamageInfo( void )
{
	int tAllianceTribe;

	tAllianceTribe = ReturnAllianceTribe( 0 );
	mSymbolDamageUp[0] = 0.0f;
	if( ( mWorldInfo.mTribe1Symbol == 0 ) || ( mWorldInfo.mTribe1Symbol == tAllianceTribe ) )
	{
		if( ( mWorldInfo.mTribe2Symbol == 0 ) || ( mWorldInfo.mTribe2Symbol == tAllianceTribe ) )
		{
			mSymbolDamageUp[0] += 0.1f;
		}
		if( ( mWorldInfo.mTribe3Symbol == 0 ) || ( mWorldInfo.mTribe3Symbol == tAllianceTribe ) )
		{
			mSymbolDamageUp[0] += 0.1f;
		}
		if( ( mWorldInfo.mTribe4Symbol == 0 ) || ( mWorldInfo.mTribe4Symbol == tAllianceTribe ) )
		{
			mSymbolDamageUp[0] += 0.1f;
		}
		if( mWorldInfo.mMonsterSymbol == 0 )
		{
			mSymbolDamageUp[0] += 0.1f;
		}
		else
		{
			if( tAllianceTribe != -1 )
			{
				if( mWorldInfo.mMonsterSymbol == tAllianceTribe )
				{
					mSymbolDamageUp[0] += 0.1f;
				}
			}
		}
		if( mSymbolDamageUp[0] == 0.0f )
		{
			if( CheckSmallTribeForAdvantage( 0 ) )
			{
				mSymbolDamageUp[0] += 0.1f;
			}
		}
	}
	mSymbolDamageDown[0] = 0.0f;
	if( ( mWorldInfo.mTribe1Symbol != 0 ) && ( mWorldInfo.mTribe1Symbol != tAllianceTribe ) )
	{
		mSymbolDamageDown[0] += 0.1f;
	}
	tAllianceTribe = ReturnAllianceTribe( 1 );
	mSymbolDamageUp[1] = 0.0f;
	if( ( mWorldInfo.mTribe2Symbol == 1 ) || ( mWorldInfo.mTribe2Symbol == tAllianceTribe ) )
	{
		if( ( mWorldInfo.mTribe1Symbol == 1 ) || ( mWorldInfo.mTribe1Symbol == tAllianceTribe ) )
		{
			mSymbolDamageUp[1] += 0.1f;
		}
		if( ( mWorldInfo.mTribe3Symbol == 1 ) || ( mWorldInfo.mTribe3Symbol == tAllianceTribe ) )
		{
			mSymbolDamageUp[1] += 0.1f;
		}
		if( ( mWorldInfo.mTribe4Symbol == 1 ) || ( mWorldInfo.mTribe4Symbol == tAllianceTribe ) )
		{
			mSymbolDamageUp[1] += 0.1f;
		}
		if( mWorldInfo.mMonsterSymbol == 1 )
		{
			mSymbolDamageUp[1] += 0.1f;
		}
		else
		{
			if( tAllianceTribe != -1 )
			{
				if( mWorldInfo.mMonsterSymbol == tAllianceTribe )
				{
					mSymbolDamageUp[1] += 0.1f;
				}
			}
		}
		if( mSymbolDamageUp[1] == 0.0f )
		{
			if( CheckSmallTribeForAdvantage( 1 ) )
			{
				mSymbolDamageUp[1] += 0.1f;
			}
		}
	}
	mSymbolDamageDown[1] = 0.0f;
	if( ( mWorldInfo.mTribe2Symbol != 1 ) && ( mWorldInfo.mTribe2Symbol != tAllianceTribe ) )
	{
		mSymbolDamageDown[1] += 0.1f;
	}
	tAllianceTribe = ReturnAllianceTribe( 2 );
	mSymbolDamageUp[2] = 0.0f;
	if( ( mWorldInfo.mTribe3Symbol == 2 ) || ( mWorldInfo.mTribe3Symbol == tAllianceTribe ) )
	{
		if( ( mWorldInfo.mTribe1Symbol == 2 ) || ( mWorldInfo.mTribe1Symbol == tAllianceTribe ) )
		{
			mSymbolDamageUp[2] += 0.1f;
		}
		if( ( mWorldInfo.mTribe2Symbol == 2 ) || ( mWorldInfo.mTribe2Symbol == tAllianceTribe ) )
		{
			mSymbolDamageUp[2] += 0.1f;
		}
		if( ( mWorldInfo.mTribe4Symbol == 2 ) || ( mWorldInfo.mTribe4Symbol == tAllianceTribe ) )
		{
			mSymbolDamageUp[2] += 0.1f;
		}
		if( mWorldInfo.mMonsterSymbol == 2 )
		{
			mSymbolDamageUp[2] += 0.1f;
		}
		else
		{
			if( tAllianceTribe != -1 )
			{
				if( mWorldInfo.mMonsterSymbol == tAllianceTribe )
				{
					mSymbolDamageUp[2] += 0.1f;
				}
			}
		}
		if( mSymbolDamageUp[2] == 0.0f )
		{
			if( CheckSmallTribeForAdvantage( 2 ) )
			{
				mSymbolDamageUp[2] += 0.1f;
			}
		}
	}
	mSymbolDamageDown[2] = 0.0f;
	if( ( mWorldInfo.mTribe3Symbol != 2 ) && ( mWorldInfo.mTribe3Symbol != tAllianceTribe ) )
	{
		mSymbolDamageDown[2] += 0.1f;
	}
	tAllianceTribe = ReturnAllianceTribe( 3 );
	mSymbolDamageUp[3] = 0.0f;
	if( ( mWorldInfo.mTribe4Symbol == 3 ) || ( mWorldInfo.mTribe4Symbol == tAllianceTribe ) )
	{
		if( ( mWorldInfo.mTribe1Symbol == 3 ) || ( mWorldInfo.mTribe1Symbol == tAllianceTribe ) )
		{
			mSymbolDamageUp[3] += 0.1f;
		}
		if( ( mWorldInfo.mTribe2Symbol == 3 ) || ( mWorldInfo.mTribe2Symbol == tAllianceTribe ) )
		{
			mSymbolDamageUp[3] += 0.1f;
		}
		if( ( mWorldInfo.mTribe3Symbol == 3 ) || ( mWorldInfo.mTribe3Symbol == tAllianceTribe ) )
		{
			mSymbolDamageUp[3] += 0.1f;
		}
		if( mWorldInfo.mMonsterSymbol == 3 )
		{
			mSymbolDamageUp[3] += 0.1f;
		}
		else
		{
			if( tAllianceTribe != -1 )
			{
				if( mWorldInfo.mMonsterSymbol == tAllianceTribe )
				{
					mSymbolDamageUp[3] += 0.1f;
				}
			}
		}
		if( mSymbolDamageUp[3] == 0.0f )
		{
			if( CheckSmallTribeForAdvantage( 3 ) )
			{
				mSymbolDamageUp[3] += 0.1f;
			}
		}
	}
	mSymbolDamageDown[3] = 0.0f;
	if( ( mWorldInfo.mTribe4Symbol != 3 ) && ( mWorldInfo.mTribe4Symbol != tAllianceTribe ) )
	{
		mSymbolDamageDown[3] += 0.1f;
	}
}
//RETURN_SYMBOL_DAMAGE_UP
float MyGame::ReturnSymbolDamageUp( int tTribe )
{
	switch( tTribe )
	{
	case 0 : return mSymbolDamageUp[0];
	case 1 : return mSymbolDamageUp[1];
	case 2 : return mSymbolDamageUp[2];
	case 3 : return mSymbolDamageUp[3];
	}
	return 0.0f;
}
//RETURN_SYMBOL_DAMAGE_DOWN
float MyGame::ReturnSymbolDamageDown( int tTribe )
{
	switch( tTribe )
	{
	case 0 : return mSymbolDamageDown[0];
	case 1 : return mSymbolDamageDown[1];
	case 2 : return mSymbolDamageDown[2];
	case 3 : return mSymbolDamageDown[3];
	}
	return 0.0f;
}
//PROCESS_FOR_GUARD_STATE
void MyGame::ProcessForGuardState( void )
{
	int index01;
	int index02;
	int index03;
	int tBroadcastInfoSort;
	BYTE tBroadcastInfoData[MAX_BROADCAST_INFO_SIZE];
	const int tGuardTribePostNum = 5;
	BOOL tExamineGuardState[4];
	int tExamineGuardIndex[4];
	BOOL tResultGuardState[4];

	tExamineGuardState[0] = FALSE;
	tExamineGuardState[1] = FALSE;
	tExamineGuardState[2] = FALSE;
	tExamineGuardState[3] = FALSE;
	switch( mSERVER_INFO.mServerNumber )
	{
	case  38 :
		tExamineGuardState[0] = TRUE;
		tExamineGuardIndex[0] = 0;
		tResultGuardState[0] = FALSE;
		tExamineGuardState[1] = TRUE;
		tExamineGuardIndex[1] = 0;
		tResultGuardState[1] = FALSE;
		tExamineGuardState[2] = TRUE;
		tExamineGuardIndex[2] = 0;
		tResultGuardState[2] = FALSE;
		tExamineGuardState[3] = TRUE;
		tExamineGuardIndex[3] = 0;
		tResultGuardState[3] = FALSE;
		break;
	case   4 :
		tExamineGuardState[0] = TRUE;
		tExamineGuardIndex[0] = 1;
		tResultGuardState[0] = FALSE;
		break;
	case   3 :
		tExamineGuardState[0] = TRUE;
		tExamineGuardIndex[0] = 2;
		tResultGuardState[0] = FALSE;
		break;
	case   2 :
		tExamineGuardState[0] = TRUE;
		tExamineGuardIndex[0] = 3;
		tResultGuardState[0] = FALSE;
		break;
	case   9 :
		tExamineGuardState[1] = TRUE;
		tExamineGuardIndex[1] = 1;
		tResultGuardState[1] = FALSE;
		break;
	case   8 :
		tExamineGuardState[1] = TRUE;
		tExamineGuardIndex[1] = 2;
		tResultGuardState[1] = FALSE;
		break;
	case   7 :
		tExamineGuardState[1] = TRUE;
		tExamineGuardIndex[1] = 3;
		tResultGuardState[1] = FALSE;
		break;
	case  14 :
		tExamineGuardState[2] = TRUE;
		tExamineGuardIndex[2] = 1;
		tResultGuardState[2] = FALSE;
		break;
	case  13 :
		tExamineGuardState[2] = TRUE;
		tExamineGuardIndex[2] = 2;
		tResultGuardState[2] = FALSE;
		break;
	case  12 :
		tExamineGuardState[2] = TRUE;
		tExamineGuardIndex[2] = 3;
		tResultGuardState[2] = FALSE;
		break;
	case 143 :
		tExamineGuardState[3] = TRUE;
		tExamineGuardIndex[3] = 1;
		tResultGuardState[3] = FALSE;
		break;
	case 142 :
		tExamineGuardState[3] = TRUE;
		tExamineGuardIndex[3] = 2;
		tResultGuardState[3] = FALSE;
		break;
	case 141 :
		tExamineGuardState[3] = TRUE;
		tExamineGuardIndex[3] = 3;
		tResultGuardState[3] = FALSE;
		break;
	default :
		return;
	}
	index03 = MAX_MONSTER_OBJECT_BASE_1_NUM;
	for( index01 = 0 ; index01 < 4 ; index01++ )
	{
		if( !tExamineGuardState[index01] )
		{
			index03 += tGuardTribePostNum;
			continue;
		}
		for( index02 = 0 ; index02 < tGuardTribePostNum ; index02++ )
		{
			if( mMONSTER_OBJECT[index03].mCheckValidState )
			{
				tResultGuardState[index01] = TRUE;
			}
			index03++;
		}
		if( !tResultGuardState[index01] )
		{
			if( mWorldInfo.mTribeGuardState[index01][tExamineGuardIndex[index01]] == 1 )
			{
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tBroadcastInfoSort = 31;
				CopyMemory( &tBroadcastInfoData[0], &index01, 4 );
				CopyMemory( &tBroadcastInfoData[4], &tExamineGuardIndex[index01], 4 );
				mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
				//--------------//
				//--------------//
				//--------------//
				mWorldInfo.mTribeGuardState[index01][tExamineGuardIndex[index01]] = 0;
			}
		}
		else
		{
			if( mWorldInfo.mTribeGuardState[index01][tExamineGuardIndex[index01]] == 0 )
			{
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tBroadcastInfoSort = 32;
				CopyMemory( &tBroadcastInfoData[0], &index01, 4 );
				CopyMemory( &tBroadcastInfoData[4], &tExamineGuardIndex[index01], 4 );
				mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
				//--------------//
				//--------------//
				//--------------//
				mWorldInfo.mTribeGuardState[index01][tExamineGuardIndex[index01]] = 1;
			}
		}
	}
}
//PROCESS_TRIBE_VOTE_SERVER
void MyGame::Process_TribeVote_Server( void )
{
	int index01;
	int index02;
	int index03;
	time_t tCountSeconds;
	struct tm *tPresentTime;
	int tBroadcastInfoSort;
	BYTE tBroadcastInfoData[MAX_BROADCAST_INFO_SIZE];

	//LOG_TO_FILE_4("------------->0 %d %d %d %d\n", mWorldInfo.mTribeVoteState[0], mWorldInfo.mTribeVoteState[1], mWorldInfo.mTribeVoteState[2], mWorldInfo.mTribeVoteState[3]);
	if( !mCheckActiveBattleLogic )
	{
		return;
	}
	time( &tCountSeconds );
	tPresentTime = localtime( &tCountSeconds );
	//LOG_TO_FILE_2("------------->1 %d %d\n", tPresentTime->tm_mday, mCheckActiveBattleLogic);
	if( tPresentTime == NULL )
	{
		return;
	}
	//LOG_TO_FILE_2("------------->2 %d %d\n", tPresentTime->tm_mday, mCheckActiveBattleLogic);
	if( ( tPresentTime->tm_mday >=  1 ) && ( tPresentTime->tm_mday <=  5 ) )
	{
		//LOG_TO_FILE("check #1\n");
		if( mWorldInfo.mTribeVoteState[0] == 1 )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 52;
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mTribeVoteState[0] = 1;
		mWorldInfo.mTribeVoteState[1] = 1;
		mWorldInfo.mTribeVoteState[2] = 1;
		mWorldInfo.mTribeVoteState[3] = 1;
		for( index01 = 0 ; index01 < 4 ; index01++ )
		{
			for( index02 = 0 ; index02 < MAX_TRIBE_VOTE_AVATAR_NUM ; index02++ )
			{
				strcpy( mTribeInfo.mTribeVoteName[index01][index02], "" );
				mTribeInfo.mTribeVoteLevel[index01][index02] = 0;
				mTribeInfo.mTribeVoteKillOtherTribe[index01][index02] = 0;
				mTribeInfo.mTribeVotePoint[index01][index02] = 0;
			}
		}
		return;
	}
	if( ( tPresentTime->tm_mday >=  6 ) && ( tPresentTime->tm_mday <= 10 ) )
	{
		//LOG_TO_FILE("check #2\n");
		if( mWorldInfo.mTribeVoteState[0] == 2 )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 53;
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mTribeVoteState[0] = 2;
		mWorldInfo.mTribeVoteState[1] = 2;
		mWorldInfo.mTribeVoteState[2] = 2;
		mWorldInfo.mTribeVoteState[3] = 2;
		return;
	}
	if( ( tPresentTime->tm_mday >= 11 ) && ( tPresentTime->tm_mday <= 11 ) )
	{
		//LOG_TO_FILE("check #3\n");
		switch( tPresentTime->tm_hour )
		{
		case  0 :
			if( mWorldInfo.mTribeVoteState[0] == 3 )
			{
				return;
			}
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 54;
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mTribeVoteState[0] = 3;
			mWorldInfo.mTribeVoteState[1] = 3;
			mWorldInfo.mTribeVoteState[2] = 3;
			mWorldInfo.mTribeVoteState[3] = 3;
			return;
		case 21 :
			if( mWorldInfo.mTribeVoteState[0] == 4 )
			{
				return;
			}
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 55;
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mTribeVoteState[0] = 4;
			mWorldInfo.mTribeVoteState[1] = 4;
			mWorldInfo.mTribeVoteState[2] = 4;
			mWorldInfo.mTribeVoteState[3] = 4;
			for( index01 = 0 ; index01 < 4 ; index01++ )
			{
				index03 = -1;
				for( index02 = 0 ; index02 < MAX_TRIBE_VOTE_AVATAR_NUM ; index02++ )
				{
					if( strcmp( mTribeInfo.mTribeVoteName[index01][index02], "" ) == 0 )
					{
						continue;
					}
					if( mTribeInfo.mTribeVotePoint[index01][index02] < 1 )
					{
						continue;
					}
					if( index03 == -1 )
					{
						index03 = index02;
					}
					else
					{
						if( mTribeInfo.mTribeVotePoint[index01][index02] > mTribeInfo.mTribeVotePoint[index01][index03] )
						{
							index03 = index02;
						}
					}
				}
				if( index03 == -1 )
				{
					strcpy( mTribeInfo.mTribeMaster[index01], "" );
				}
				else
				{
					strncpy( mTribeInfo.mTribeMaster[index01], mTribeInfo.mTribeVoteName[index01][index03], MAX_AVATAR_NAME_LENGTH );
				}
				for( index02 = 0 ; index02 < MAX_TRIBE_SUBMASTER_NUM ; index02++ )
				{
					strcpy( mTribeInfo.mTribeSubMaster[index01][index02], "" );
				}
			}
			return;
		case 22 :
			if( mWorldInfo.mTribeVoteState[0] == 0 )
			{
				return;
			}
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 56;
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mTribeVoteState[0] = 0;
			mWorldInfo.mTribeVoteState[1] = 0;
			mWorldInfo.mTribeVoteState[2] = 0;
			mWorldInfo.mTribeVoteState[3] = 0;
			return;
		}
		return;
	}
}
//PROCESS_ALLIENCE_SERVER
void MyGame::Process_Allience_Server( void )
{
	int index01;
	int index02;
	int tBroadcastInfoSort;
	BYTE tBroadcastInfoData[MAX_BROADCAST_INFO_SIZE];
    BOOL tCheckBigTribePoint[2];
	int tBigTribePointTribe;
	int tAllianceIndex;
	int tPossibleAllianceDate01;
	int tPossibleAllianceDate02;

	if( !mCheckActiveBattleLogic )
	{
		return;
	}

	if (mSERVER_INFO.m_iDeploymentStage < 3) return;

	mAlliencePostTick++;

	switch( mAllienceBattleState )
	{
	case 0 :
		mAlliencePostAvatarIndex[0] = -1;
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
		{
			if( !mAVATAR_OBJECT[index01].mCheckValidState )
			{
				continue;
			}
			if( mUSER[index01].mMoveZoneResult == 1 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aLifeValue < 1 )
			{
				continue;
			}
			if( ReturnTribeRole( &mUSER[index01].mAvatarInfo.aName[0], mUSER[index01].mAvatarInfo.aTribe ) != 1 )
			{
				continue;
			}
			if( mUTIL.CheckInRange( &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAllienceBattlePostLocation[0][0], mAllienceBattlePostRadius[0] ) )
			{
				mAlliencePostAvatarIndex[0] = index01;
				mAlliencePostAvatarUniqueNumber[0] = mAVATAR_OBJECT[index01].mUniqueNumber;
				mAlliencePostAvatarTribe[0] = mAVATAR_OBJECT[index01].mDATA.aTribe;
				tCheckBigTribePoint[0] = FALSE;
				tBigTribePointTribe = 0;
				for( index02 = 1 ; index02 < 4 ; index02++ )
				{
					if( ReturnTribePointWithAlliance( index02 ) > ReturnTribePointWithAlliance( tBigTribePointTribe ) )
					{
						tBigTribePointTribe = index02;
					}
				}
				if( tBigTribePointTribe == mAlliencePostAvatarTribe[0] )
				{
					tCheckBigTribePoint[0] = TRUE;
				}
				break;
			}
		}
		mAlliencePostAvatarIndex[1] = -1;
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
		{
			if( !mAVATAR_OBJECT[index01].mCheckValidState )
			{
				continue;
			}
			if( mUSER[index01].mMoveZoneResult == 1 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aLifeValue < 1 )
			{
				continue;
			}
			if( ReturnTribeRole( &mUSER[index01].mAvatarInfo.aName[0], mUSER[index01].mAvatarInfo.aTribe ) != 1 )
			{
				continue;
			}
			if( mUTIL.CheckInRange( &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAllienceBattlePostLocation[1][0], mAllienceBattlePostRadius[1] ) )
			{
				mAlliencePostAvatarIndex[1] = index01;
				mAlliencePostAvatarUniqueNumber[1] = mAVATAR_OBJECT[index01].mUniqueNumber;
				mAlliencePostAvatarTribe[1] = mAVATAR_OBJECT[index01].mDATA.aTribe;
				tCheckBigTribePoint[1] = FALSE;
				tBigTribePointTribe = 0;
				for( index02 = 1 ; index02 < 4 ; index02++ )
				{
					if( ReturnTribePointWithAlliance( index02 ) > ReturnTribePointWithAlliance( tBigTribePointTribe ) )
					{
						tBigTribePointTribe = index02;
					}
				}
				if( tBigTribePointTribe == mAlliencePostAvatarTribe[1] )
				{
					tCheckBigTribePoint[1] = TRUE;
				}
				break;
			}
		}
		if( ( mAlliencePostAvatarIndex[0] == -1 ) || ( mAlliencePostAvatarIndex[1] == -1 ) )
		{
			return;
		}
		if( mAlliencePostAvatarTribe[0] != ReturnAllianceTribe( mAlliencePostAvatarTribe[1] ) )
		{
			if(
			  ( mWorldInfo.mAllianceState[0][0] == mAlliencePostAvatarTribe[0] ) ||
			  ( mWorldInfo.mAllianceState[0][1] == mAlliencePostAvatarTribe[0] ) ||
			  ( mWorldInfo.mAllianceState[1][0] == mAlliencePostAvatarTribe[0] ) ||
			  ( mWorldInfo.mAllianceState[1][1] == mAlliencePostAvatarTribe[0] ) ||
			  ( mWorldInfo.mAllianceState[0][0] == mAlliencePostAvatarTribe[1] ) ||
			  ( mWorldInfo.mAllianceState[0][1] == mAlliencePostAvatarTribe[1] ) ||
			  ( mWorldInfo.mAllianceState[1][0] == mAlliencePostAvatarTribe[1] ) ||
			  ( mWorldInfo.mAllianceState[1][1] == mAlliencePostAvatarTribe[1] ) ||
			  ( ReturnTribePointWithAlliance( mAlliencePostAvatarTribe[0] ) < 100 ) ||
			  ( ReturnTribePointWithAlliance( mAlliencePostAvatarTribe[1] ) < 100 ) ||
			  ( tCheckBigTribePoint[0] ) ||
			  ( tCheckBigTribePoint[1] ) ||
			  ( ( mWorldInfo.mPossibleAllianceInfo[mAlliencePostAvatarTribe[0]][1] == 1 ) && ( mUTIL.ReturnNowDate() <= mWorldInfo.mPossibleAllianceInfo[mAlliencePostAvatarTribe[0]][0] ) ) ||
			  ( ( mWorldInfo.mPossibleAllianceInfo[mAlliencePostAvatarTribe[1]][1] == 1 ) && ( mUTIL.ReturnNowDate() <= mWorldInfo.mPossibleAllianceInfo[mAlliencePostAvatarTribe[1]][0] ) )
			  )
			{
				mTRANSFER.B_TRIBE_ALLIANCE_INFO( 1, 0 );
				mUSER[mAlliencePostAvatarIndex[0]].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
				mTRANSFER.B_TRIBE_ALLIANCE_INFO( 1, 0 );
				mUSER[mAlliencePostAvatarIndex[1]].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
				mAlliencePostTick = 0;
				mAllienceBattleState = 1;
				return;
			}
			else
			{
				mAlliencePostTick = 0;
				mAllienceBattleState = 2;
				mAllienceRemainTime = 60;
				return;
			}
		}
		else
		{
			mAlliencePostTick = 0;
			mAllienceBattleState = 3;
			mAllienceRemainTime = 60;
			return;
		}
		return;
	case 1 :
		if( mAlliencePostTick < ( 120 * 1 ) )
		{
			return;
		}
		mAlliencePostTick = 0;
		mAllienceBattleState = 0;
		return;
	case 2 :
		if( ( mAlliencePostTick % 2 ) != 0 )
		{
			return;
		}
		if(
		  ( !mAVATAR_OBJECT[mAlliencePostAvatarIndex[0]].mCheckValidState ) ||
		  ( mAVATAR_OBJECT[mAlliencePostAvatarIndex[0]].mUniqueNumber != mAlliencePostAvatarUniqueNumber[0] ) ||
		  ( mAVATAR_OBJECT[mAlliencePostAvatarIndex[0]].mDATA.aLifeValue < 1 ) ||
		  ( !mUTIL.CheckInRange( &mAVATAR_OBJECT[mAlliencePostAvatarIndex[0]].mDATA.aAction.aLocation[0], &mAllienceBattlePostLocation[0][0], mAllienceBattlePostRadius[0] ) ) ||
		  ( mUSER[mAlliencePostAvatarIndex[0]].mMoveZoneResult == 1 ) ||
		  ( mAVATAR_OBJECT[mAlliencePostAvatarIndex[0]].mDATA.aVisibleState == 0 ) ||
		  ( !mAVATAR_OBJECT[mAlliencePostAvatarIndex[1]].mCheckValidState ) ||
		  ( mAVATAR_OBJECT[mAlliencePostAvatarIndex[1]].mUniqueNumber != mAlliencePostAvatarUniqueNumber[1] ) ||
		  ( mAVATAR_OBJECT[mAlliencePostAvatarIndex[1]].mDATA.aLifeValue < 1 ) ||
		  ( !mUTIL.CheckInRange( &mAVATAR_OBJECT[mAlliencePostAvatarIndex[1]].mDATA.aAction.aLocation[0], &mAllienceBattlePostLocation[1][0], mAllienceBattlePostRadius[1] ) ) ||
		  ( mUSER[mAlliencePostAvatarIndex[1]].mMoveZoneResult == 1 ) ||
		  ( mAVATAR_OBJECT[mAlliencePostAvatarIndex[1]].mDATA.aVisibleState == 0 )
		  )
		{
			mTRANSFER.B_TRIBE_ALLIANCE_INFO( 3, 0 );
			mUSER[mAlliencePostAvatarIndex[0]].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			mTRANSFER.B_TRIBE_ALLIANCE_INFO( 3, 0 );
			mUSER[mAlliencePostAvatarIndex[1]].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			mAlliencePostTick = 0;
			mAllienceBattleState = 0;
			return;
		}
		mTRANSFER.B_TRIBE_ALLIANCE_INFO( 2, mAllienceRemainTime );
		mUSER[mAlliencePostAvatarIndex[0]].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
		mTRANSFER.B_TRIBE_ALLIANCE_INFO( 2, mAllienceRemainTime );
		mUSER[mAlliencePostAvatarIndex[1]].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
		mAllienceRemainTime--;
		if( mAllienceRemainTime > 0 )
		{
			return;
		}
		if( ( mWorldInfo.mAllianceState[0][0] == -1 ) && ( mWorldInfo.mAllianceState[0][1] == -1 ) )
		{
			tAllianceIndex = 0;
		}
		else
		{
			tAllianceIndex = 1;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 46;
		CopyMemory( &tBroadcastInfoData[0], &mAlliencePostAvatarTribe[0], 4 );
		CopyMemory( &tBroadcastInfoData[4], &mAlliencePostAvatarTribe[1], 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mPossibleAllianceInfo[mAlliencePostAvatarTribe[0]][0] = 0;
		mWorldInfo.mPossibleAllianceInfo[mAlliencePostAvatarTribe[0]][1] = 0;
		mWorldInfo.mPossibleAllianceInfo[mAlliencePostAvatarTribe[1]][0] = 0;
		mWorldInfo.mPossibleAllianceInfo[mAlliencePostAvatarTribe[1]][1] = 0;
		mWorldInfo.mAllianceState[tAllianceIndex][0] = mAlliencePostAvatarTribe[0];
		mWorldInfo.mAllianceState[tAllianceIndex][1] = mAlliencePostAvatarTribe[1];
		mAlliencePostTick = 0;
		mAllienceBattleState = 4;
		return;
	case 3 :
		if( ( mAlliencePostTick % 2 ) != 0 )
		{
			return;
		}
		if(
		  ( !mAVATAR_OBJECT[mAlliencePostAvatarIndex[0]].mCheckValidState ) ||
		  ( mAVATAR_OBJECT[mAlliencePostAvatarIndex[0]].mUniqueNumber != mAlliencePostAvatarUniqueNumber[0] ) ||
		  ( mAVATAR_OBJECT[mAlliencePostAvatarIndex[0]].mDATA.aLifeValue < 1 ) ||
		  ( !mUTIL.CheckInRange( &mAVATAR_OBJECT[mAlliencePostAvatarIndex[0]].mDATA.aAction.aLocation[0], &mAllienceBattlePostLocation[0][0], mAllienceBattlePostRadius[0] ) ) ||
		  ( mUSER[mAlliencePostAvatarIndex[0]].mMoveZoneResult == 1 ) ||
		  ( mAVATAR_OBJECT[mAlliencePostAvatarIndex[0]].mDATA.aVisibleState == 0 ) ||
		  ( !mAVATAR_OBJECT[mAlliencePostAvatarIndex[1]].mCheckValidState ) ||
		  ( mAVATAR_OBJECT[mAlliencePostAvatarIndex[1]].mUniqueNumber != mAlliencePostAvatarUniqueNumber[1] ) ||
		  ( mAVATAR_OBJECT[mAlliencePostAvatarIndex[1]].mDATA.aLifeValue < 1 ) ||
		  ( !mUTIL.CheckInRange( &mAVATAR_OBJECT[mAlliencePostAvatarIndex[1]].mDATA.aAction.aLocation[0], &mAllienceBattlePostLocation[1][0], mAllienceBattlePostRadius[1] ) ) ||
		  ( mUSER[mAlliencePostAvatarIndex[1]].mMoveZoneResult == 1 ) ||
		  ( mAVATAR_OBJECT[mAlliencePostAvatarIndex[1]].mDATA.aVisibleState == 0 )
		  )
		{
			mTRANSFER.B_TRIBE_ALLIANCE_INFO( 5, 0 );
			mUSER[mAlliencePostAvatarIndex[0]].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			mTRANSFER.B_TRIBE_ALLIANCE_INFO( 5, 0 );
			mUSER[mAlliencePostAvatarIndex[1]].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			mAlliencePostTick = 0;
			mAllienceBattleState = 0;
			return;
		}
		mTRANSFER.B_TRIBE_ALLIANCE_INFO( 4, mAllienceRemainTime );
		mUSER[mAlliencePostAvatarIndex[0]].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
		mTRANSFER.B_TRIBE_ALLIANCE_INFO( 4, mAllienceRemainTime );
		mUSER[mAlliencePostAvatarIndex[1]].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
		mAllienceRemainTime--;
		if( mAllienceRemainTime > 0 )
		{
			return;
		}
		if( ( ( mWorldInfo.mAllianceState[0][0] == mAlliencePostAvatarTribe[0] ) && ( mWorldInfo.mAllianceState[0][1] == mAlliencePostAvatarTribe[1] ) ) || ( ( mWorldInfo.mAllianceState[0][1] == mAlliencePostAvatarTribe[0] ) && ( mWorldInfo.mAllianceState[0][0] == mAlliencePostAvatarTribe[1] ) ) )
		{
			tAllianceIndex = 0;
		}
		else
		{
			tAllianceIndex = 1;
		}
		tPossibleAllianceDate01 = mUTIL.ReturnAddDate( mUTIL.ReturnNowDate(), 14 );
		tPossibleAllianceDate02 = mUTIL.ReturnAddDate( mUTIL.ReturnNowDate(), 14 );
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 47;
		CopyMemory( &tBroadcastInfoData[0], &mAlliencePostAvatarTribe[0], 4 );
		CopyMemory( &tBroadcastInfoData[4], &mAlliencePostAvatarTribe[1], 4 );
		CopyMemory( &tBroadcastInfoData[8], &tPossibleAllianceDate01, 4 );
		CopyMemory( &tBroadcastInfoData[12], &tPossibleAllianceDate02, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mGAME.mWorldInfo.mPossibleAllianceInfo[mAlliencePostAvatarTribe[0]][0] = tPossibleAllianceDate01;
		mGAME.mWorldInfo.mPossibleAllianceInfo[mAlliencePostAvatarTribe[0]][1] = 1;
		mGAME.mWorldInfo.mPossibleAllianceInfo[mAlliencePostAvatarTribe[1]][0] = tPossibleAllianceDate02;
		mGAME.mWorldInfo.mPossibleAllianceInfo[mAlliencePostAvatarTribe[1]][1] = 1;
		mGAME.mWorldInfo.mAllianceState[tAllianceIndex][0] = -1;
		mGAME.mWorldInfo.mAllianceState[tAllianceIndex][1] = -1;
		mAlliencePostTick = 0;
		mAllienceBattleState = 4;
		return;
	case 4 :
		if( mAlliencePostTick < ( 120 * 60 ) )
		{
			return;
		}
		mAlliencePostTick = 0;
		mAllienceBattleState = 0;
		return;
	}
}
//PROCESS_ZONE_037
void MyGame::Process_Zone_037( void )
{
	time_t tCountSeconds;
	struct tm *tPresentTime;
	int tBroadcastInfoSort;
	BYTE tBroadcastInfoData[MAX_BROADCAST_INFO_SIZE];

	if( !mCheckActiveBattleLogic )
	{
		return;
	}
	mZone037PostTick++;
	switch( mWorldInfo.mTribeSymbolBattle )
	{
	case 0 :
		switch( mZone037BattleState )
		{
		case 0 :
			time( &tCountSeconds );
			tPresentTime = localtime( &tCountSeconds );
			if( tPresentTime == NULL )
			{
				return;
			}
			
#ifdef __TK__
			//tuesday, thursday, saturday, 20:00 ~ 23:00
			if( ( ( tPresentTime->tm_wday != 2 ) && ( tPresentTime->tm_wday != 4 ) && ( tPresentTime->tm_wday != 6 ) ) || ( tPresentTime->tm_hour != 20 ) || ( tPresentTime->tm_min > 10 ) )
			{
				return;
			}
#else
			//tuesday, thursday, saturday, 18:00 ~ 21:00
			if( ( ( tPresentTime->tm_wday != 2 ) && ( tPresentTime->tm_wday != 4 ) && ( tPresentTime->tm_wday != 6 ) ) || ( tPresentTime->tm_hour != 18 ) || ( tPresentTime->tm_min > 10 ) )
			{
				return;
			}
#endif

			mZone037PostTick = 0;
			mZone037BattleState = 1;
			mZone037RemainTime = 10;
			return;
		case 1 :
			if( ( mZone037PostTick % ( 120 * 1 ) ) != 0 )
			{
				return;
			}
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 39;
			CopyMemory( &tBroadcastInfoData[0], &mZone037RemainTime, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mZone037RemainTime--;
			if( mZone037RemainTime == 0 )
			{
				mZone037PostTick = 0;
				mZone037BattleState = 2;
			}
			return;
		case 2 :
			if( mZone037PostTick < ( 120 * 1 ) )
			{
				return;
			}
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 40;
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mTribeSymbolBattle = 1;
			mZone037PostTick = 0;
			mZone037BattleState = 0;
			mSUMMON.SummonGuard( FALSE, TRUE );
			return;
		}
		return;
	case 1 :
		switch( mZone037BattleState )
		{
		case 0 :
			time( &tCountSeconds );
			tPresentTime = localtime( &tCountSeconds );
			if( tPresentTime == NULL )
			{
				return;
			}
			
#ifdef __TK__
			//tuesday, thursday, saturday, 20:00 ~ 23:00
			if( ( ( tPresentTime->tm_wday != 2 ) && ( tPresentTime->tm_wday != 4 ) && ( tPresentTime->tm_wday != 6 ) ) || ( tPresentTime->tm_hour != 23 ) || ( tPresentTime->tm_min > 10 ) )
			{
				return;
			}
#else
			if( ( ( tPresentTime->tm_wday != 2 ) && ( tPresentTime->tm_wday != 4 ) && ( tPresentTime->tm_wday != 6 ) ) || ( tPresentTime->tm_hour != 21 ) || ( tPresentTime->tm_min > 10 ) )
			{
				return;
			}
#endif
			mZone037PostTick = 0;
			mZone037BattleState = 1;
			mZone037RemainTime = 10;
			return;
		case 1 :
			if( ( mZone037PostTick % ( 120 * 1 ) ) != 0 )
			{
				return;
			}
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 44;
			CopyMemory( &tBroadcastInfoData[0], &mZone037RemainTime, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mZone037RemainTime--;
			if( mZone037RemainTime == 0 )
			{
				mZone037PostTick = 0;
				mZone037BattleState = 2;
			}
			return;
		case 2 :
			if( mZone037PostTick < ( 120 * 1 ) )
			{
				return;
			}
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 45;
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mTribeSymbolBattle = 0;
            // @ Tribe Formation Skill @
            mWorldInfo.mTribeMasterCallAbility[0] = 0;
            mWorldInfo.mTribeMasterCallAbility[1] = 0;
            mWorldInfo.mTribeMasterCallAbility[2] = 0;
            mWorldInfo.mTribeMasterCallAbility[3] = 0;
            // @
			mZone037PostTick = 0;
			mZone037BattleState = 0;
			return;
		}
		return;
	}
}
//PROCESS_ZONE_124
void MyGame::Process_Zone_124( void )
{
	int index01 = 0;

	if(!mDuel_124)
		return;

	if(0 < mDuel_124_RemainTime) {
		if(mDuel_124_AvatarNum[0] < 1 || mDuel_124_AvatarNum[1] < 1) {
			for(index01 = 0; index01 < mSERVER.mMAX_USER_NUM; ++index01) {
				if(!mAVATAR_OBJECT[index01].mCheckValidState) {
					continue;
				}
				if( mUSER[index01].mMoveZoneResult == 1 ) {
					continue;
				}
				if(mDuel_124_AvatarNum[0] < 1) {
					mTRANSFER.B_DUEL_END_RECV( 2 );
				} else if(mDuel_124_AvatarNum[1] < 1) {
					mTRANSFER.B_DUEL_END_RECV( 1 );
				}
				mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );

				if(mAVATAR_OBJECT[index01].mDuelProcessState != 4) {
					continue;
				}
				mAVATAR_OBJECT[index01].mDATA.aDuelState[0] = 0;
				mAVATAR_OBJECT[index01].mDATA.aDuelState[1] = 0;
				mAVATAR_OBJECT[index01].mDATA.aDuelState[2] = 0;
				mAVATAR_OBJECT[index01].mCheckPossibleEatPotion = TRUE;
				mAVATAR_OBJECT[index01].mDuelProcessState = 0;
				mAVATAR_OBJECT[index01].mDuelProcessRemainTime = 0;
				mTRANSFER.B_AVATAR_CHANGE_INFO_1(index01, mAVATAR_OBJECT[index01].mUniqueNumber, 7, mAVATAR_OBJECT[index01].mDATA.aDuelState[0], mAVATAR_OBJECT[index01].mDATA.aDuelState[1], mAVATAR_OBJECT[index01].mDATA.aDuelState[2] );
				mUTIL.Broadcast( TRUE, &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
			}
			mDuel_124 = FALSE;
			mDuel_124_RemainTime = 0;
			mDuel_124_AvatarNum[0] = 0;
			mDuel_124_AvatarNum[1] = 0;
			mDuel_124_Pvp = FALSE;
			return;
		}
		if(((mTickCount % 2) == 0 ) && (0 < mDuel_124_RemainTime)) {
			if((mDuel_124_RemainTime % 5) == 0) {
				mTRANSFER.B_DUEL_TIME_INFO( mDuel_124_RemainTime );
				for(index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++) {
					if(mAVATAR_OBJECT[index01].mCheckValidState) {
						mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
					}
				}
			} else {
				if(mDuel_124_RemainTime == 1) {
					for(index01 = 0; index01 < mSERVER.mMAX_USER_NUM; ++index01) {
						if(!mAVATAR_OBJECT[index01].mCheckValidState) {
							continue;
						}
						if( mUSER[index01].mMoveZoneResult == 1 ) {
							continue;
						}
						if((mDuel_124_AvatarNum[0] == mDuel_124_AvatarNum[1]) || (!mDuel_124_Pvp)) {
							mTRANSFER.B_DUEL_END_RECV( 0 );
						} else if(mDuel_124_AvatarNum[0] > mDuel_124_AvatarNum[1]) {
							mTRANSFER.B_DUEL_END_RECV( 1 );
						} else {
							mTRANSFER.B_DUEL_END_RECV( 2 );
						}
						mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
						if(mUSER[index01].mAuthInfo.AuthType == 1) {
							continue;
						}
						if(mAVATAR_OBJECT[index01].mDuelProcessState != 4) {
							continue;
						}
						mAVATAR_OBJECT[index01].mDATA.aDuelState[0] = 0;
						mAVATAR_OBJECT[index01].mDATA.aDuelState[1] = 0;
						mAVATAR_OBJECT[index01].mDATA.aDuelState[2] = 0;
						mAVATAR_OBJECT[index01].mCheckPossibleEatPotion = TRUE;
						mAVATAR_OBJECT[index01].mDuelProcessState = 0;
						mAVATAR_OBJECT[index01].mDuelProcessRemainTime = 0;
						mTRANSFER.B_AVATAR_CHANGE_INFO_1(index01, mAVATAR_OBJECT[index01].mUniqueNumber, 7, mAVATAR_OBJECT[index01].mDATA.aDuelState[0], mAVATAR_OBJECT[index01].mDATA.aDuelState[1], mAVATAR_OBJECT[index01].mDATA.aDuelState[2] );
						mUTIL.Broadcast( TRUE, &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
					}
					mDuel_124 = FALSE;
					mDuel_124_RemainTime = 0;
					mDuel_124_AvatarNum[0] = 0;
					mDuel_124_AvatarNum[1] = 0;
					mDuel_124_Pvp = FALSE;
					return;
				}
			}
			--mDuel_124_RemainTime;
		}
	}
}
//PROCESS_ZONE_071_TYPE
void MyGame::Process_Zone_071_TYPE( void )
{
}
//PROCESS_ZONE_049_TYPE (������)
void MyGame::Process_Zone_049_TYPE( void )
{
	int index01;
	int tBroadcastInfoSort;
	BYTE tBroadcastInfoData[MAX_BROADCAST_INFO_SIZE];
	int tTribeUserNum[4];
	int tWinTribe;
	int tGainExperience;
	int tGainMoney;
	int tMedalCnt = 0;



	LOG_TO_FILE_1("savas baslat1\n", mZone049TypePostTick);
	if( !mCheckActiveBattleLogic )
	{
		return;
	}
	mZone049TypePostTick++;
	switch( mWorldInfo.mZone049TypeState[mZone049TypeZoneIndex] )
	{
	case 0 :
		switch( mZone049TypeBattleState )
		{
		case 0 :
			if( ( mZone049TypeZoneIndex >= 3 ) && ( mZone049TypeZoneIndex <= 5 ) )
			{
				if( mWorldInfo.mTribeSymbolBattle == 1 )
				{
					return;
				}
			}
			if( mZone049TypePostTick < ( 120 * 5 ) )
			{
				return;
			}
			mZone049TypePostTick = 0;
			mZone049TypeBattleState = 1;
			mZone049TypeRemainTime1 = 10;
			return;
		case 1 :
			if( ( mZone049TypePostTick % ( 120 * 1 ) ) != 0 )
			{
				return;
			}
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 1;
			CopyMemory( &tBroadcastInfoData[0], &mZone049TypeZoneIndex, 4 );
			CopyMemory( &tBroadcastInfoData[4], &mZone049TypeRemainTime1, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mZone049TypeRemainTime1--;
			if( mZone049TypeRemainTime1 == 0 )
			{
				mZone049TypePostTick = 0;
				mZone049TypeBattleState = 2;
			}
			return;
		case 2 :
			if( mZone049TypePostTick < ( 120 * 1 ) )
			{
				return;
			}
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 2;
			CopyMemory( &tBroadcastInfoData[0], &mZone049TypeZoneIndex, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mZone049TypeState[mZone049TypeZoneIndex] = 1;
			mZone049TypePostTick = 0;
			mZone049TypeBattleState = 0;
			return;
		}
		return;
	case 1 :
		if( mZone049TypePostTick < ( 120 * 3 ) )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 3;
		CopyMemory( &tBroadcastInfoData[0], &mZone049TypeZoneIndex, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone049TypeState[mZone049TypeZoneIndex] = 2;
		mZone049TypePostTick = 0;
		mZone049TypeBattleState = 0;
		return;
	case 2 :
		if( mZone049TypePostTick < ( 120 * 1 ) )
		{
			return;
		}
		mWorldInfo.mZone049TypeState[mZone049TypeZoneIndex] = 3;
		mZone049TypePostTick = 0;
		mZone049TypeBattleState = 0;
		mZone049TypeRemainTime2 = 1200;
		mZone049TypeBattleResult[0] = 0;
		mZone049TypeBattleResult[1] = 0;
		mZone049TypeBattleResult[2] = 0;
		mZone049TypeBattleResult[3] = 0;
		//--------------//
		//SUMMON_MONSTER//
		//--------------//
		mSUMMON.SummonMonsterWithoutTimeLimit();
		//--------------//
		//--------------//
		//--------------//
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 4;
		CopyMemory( &tBroadcastInfoData[0], &mZone049TypeZoneIndex, 4 );
		CopyMemory( &tBroadcastInfoData[4], &mZone049TypeRemainTime2, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		return;
	case 3 :
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
		{
			if( !mAVATAR_OBJECT[index01].mCheckValidState )
			{
				continue;
			}
			if( mUSER[index01].mMoveZoneResult == 1 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0 )
			{
				continue;
			}
			break;
		}
		if( index01 == mSERVER.mMAX_USER_NUM )
		{
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 5;
			CopyMemory( &tBroadcastInfoData[0], &mZone049TypeZoneIndex, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mZone049TypeState[mZone049TypeZoneIndex] = 5;
			mZone049TypePostTick = 0;
			mZone049TypeBattleState = 0;
			//--------------//
			//DELETE_MONSTER//
			//--------------//
			mSUMMON.DeleteMonster();
			//--------------//
			//--------------//
			//--------------//
			return;
		}
		mZone049TypeRemainTime2--;
		if( ( mZone049TypeRemainTime2 % 10 ) == 0 )
		{
			mTRANSFER.B_049_TYPE_BATTLE_INFO( &mZone049TypeBattleResult[0], mZone049TypeRemainTime2 );
			mUTIL.Broadcast( FALSE, NULL, 0, false );
		}
#if 0
		if( mZone049TypePostTick == ( 120 * 60 ) )
		{
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 6;
			CopyMemory( &tBroadcastInfoData[0], &mZone049TypeZoneIndex, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mZone049TypeState[mZone049TypeZoneIndex] = 5;
			mZone049TypePostTick = 0;
			mZone049TypeBattleState = 0;
			//--------------//
			//DELETE_MONSTER//
			//--------------//
			mSUMMON.DeleteMonster();
			//--------------//
			//--------------//
			//--------------//
			return;
		}
		tTribeUserNum[0] = 0;
		tTribeUserNum[1] = 0;
		tTribeUserNum[2] = 0;
		tTribeUserNum[3] = 0;
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
		{
			if( !mAVATAR_OBJECT[index01].mCheckValidState )
			{
				continue;
			}
			if( mUSER[index01].mMoveZoneResult == 1 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aLifeValue < 1 )
			{
				continue;
			}
			switch( mAVATAR_OBJECT[index01].mDATA.aTribe )
			{
			case 0 : tTribeUserNum[0]++; break;
			case 1 : tTribeUserNum[1]++; break;
			case 2 : tTribeUserNum[2]++; break;
			case 3 : tTribeUserNum[3]++; break;
			}
		}
		//-----------//
		//BATTLE_INFO//
		//-----------//
		if( ( mZone049TypePostTick % 10 ) == 1 )
		{
			mTRANSFER.B_049_TYPE_BATTLE_INFO( &tTribeUserNum[0] );
			mUTIL.Broadcast( FALSE, NULL, 0 );
		}
		//-----------//
		//-----------//
		//-----------//
#endif
		if(0 != mZone049TypeRemainTime2){
			return;
		}

		tWinTribe = -1;
		if( tWinTribe == -1 )
		{
			switch( ReturnAllianceTribe( 0 ) )
			{
				case 1 :
					if( ( mZone049TypeBattleResult[0] > mZone049TypeBattleResult[2] ) && ( mZone049TypeBattleResult[0] > mZone049TypeBattleResult[3] ) )
					{
						tWinTribe = 0;
					}
					break;
				case 2 :
					if( ( mZone049TypeBattleResult[0] > mZone049TypeBattleResult[1] ) && ( mZone049TypeBattleResult[0] > mZone049TypeBattleResult[3] ) )
					{
						tWinTribe = 0;
					}
					break;
				case 3 :
					if( ( mZone049TypeBattleResult[0] > mZone049TypeBattleResult[1] ) && ( mZone049TypeBattleResult[0] > mZone049TypeBattleResult[2] ) )
					{
						tWinTribe = 0;
					}
					break;
				default :
					if( ( mZone049TypeBattleResult[0] > mZone049TypeBattleResult[1] ) && ( mZone049TypeBattleResult[0] > mZone049TypeBattleResult[2] ) && ( mZone049TypeBattleResult[0] > mZone049TypeBattleResult[3] ) )
					{
						tWinTribe = 0;
					}
					break;
			}
		}
		if( tWinTribe == -1 )
		{
			switch( ReturnAllianceTribe( 1 ) )
			{
				case 0 :
					if( ( mZone049TypeBattleResult[1] > mZone049TypeBattleResult[2] ) && ( mZone049TypeBattleResult[1] > mZone049TypeBattleResult[3] ) )
					{
						tWinTribe = 1;
					}
					break;
				case 2 :
					if( ( mZone049TypeBattleResult[1] > mZone049TypeBattleResult[0] ) && ( mZone049TypeBattleResult[1] > mZone049TypeBattleResult[3] ) )
					{
						tWinTribe = 1;
					}
					break;
				case 3 :
					if( ( mZone049TypeBattleResult[1] > mZone049TypeBattleResult[0] ) && ( mZone049TypeBattleResult[1] > mZone049TypeBattleResult[2] ) )
					{
						tWinTribe = 1;
					}
					break;
				default :
					if( ( mZone049TypeBattleResult[1] > mZone049TypeBattleResult[0] ) && ( mZone049TypeBattleResult[1] > mZone049TypeBattleResult[2] ) && ( mZone049TypeBattleResult[1] > mZone049TypeBattleResult[3] ) )
					{
						tWinTribe = 1;
					}
					break;
			}
		}
		if( tWinTribe == -1 )
		{
			switch( ReturnAllianceTribe( 2 ) )
			{
				case 0 :
					if( ( mZone049TypeBattleResult[2] > mZone049TypeBattleResult[1] ) && ( mZone049TypeBattleResult[2] > mZone049TypeBattleResult[3] ) )
					{
						tWinTribe = 2;
					}
					break;
				case 1 :
					if( ( mZone049TypeBattleResult[2] > mZone049TypeBattleResult[0] ) && ( mZone049TypeBattleResult[2] > mZone049TypeBattleResult[3] ) )
					{
						tWinTribe = 2;
					}
					break;
				case 3 :
					if( ( mZone049TypeBattleResult[2] > mZone049TypeBattleResult[0] ) && ( mZone049TypeBattleResult[2] > mZone049TypeBattleResult[1] ) )
					{
						tWinTribe = 2;
					}
					break;
				default :
					if( ( mZone049TypeBattleResult[2] > mZone049TypeBattleResult[0] ) && ( mZone049TypeBattleResult[2] > mZone049TypeBattleResult[1] ) && ( mZone049TypeBattleResult[2] > mZone049TypeBattleResult[3] ) )
					{
						tWinTribe = 2;
					}
					break;
			}
		}
		if( tWinTribe == -1 )
		{
			switch( ReturnAllianceTribe( 3 ) )
			{
				case 0 :
					if( ( mZone049TypeBattleResult[3] > mZone049TypeBattleResult[1] ) && ( mZone049TypeBattleResult[3] > mZone049TypeBattleResult[2] ) )
					{
						tWinTribe = 3;
					}
					break;
				case 1 :
					if( ( mZone049TypeBattleResult[3] > mZone049TypeBattleResult[0] ) && ( mZone049TypeBattleResult[3] > mZone049TypeBattleResult[2] ) )
					{
						tWinTribe = 3;
					}
					break;
				case 2 :
					if( ( mZone049TypeBattleResult[3] > mZone049TypeBattleResult[0] ) && ( mZone049TypeBattleResult[3] > mZone049TypeBattleResult[1] ) )
					{
						tWinTribe = 3;
					}
					break;
				default :
					if( ( mZone049TypeBattleResult[3] > mZone049TypeBattleResult[0] ) && ( mZone049TypeBattleResult[3] > mZone049TypeBattleResult[1] ) && ( mZone049TypeBattleResult[3] > mZone049TypeBattleResult[2] ) )
					{
						tWinTribe = 3;
					}
					break;
			}
		}
#if 0
			if( tWinTribe == -1 )
			{
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tBroadcastInfoSort = 6;
				CopyMemory( &tBroadcastInfoData[0], &mZone049TypeZoneIndex, 4 );
				mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
				//--------------//
				//--------------//
				//--------------//
				mWorldInfo.mZone049TypeState[mZone049TypeZoneIndex] = 5;
				mZone049TypePostTick = 0;
				mZone049TypeBattleState = 0;
				//--------------//
				//DELETE_MONSTER//
				//--------------//
				mSUMMON.DeleteMonster();
				//--------------//
				//--------------//
				//--------------//
				return;
			}
		if( tWinTribe == -1 )
		{
			return;
		}
#endif
		if(-1 == tWinTribe){
			tWinTribe = -2;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 7;
		CopyMemory( &tBroadcastInfoData[0], &mZone049TypeZoneIndex, 4 );
		CopyMemory( &tBroadcastInfoData[4], &tWinTribe, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone049TypeState[mZone049TypeZoneIndex] = 4;
		mZone049TypePostTick = 0;
		mZone049TypeBattleState = 0;
		//----------------//
		//BATTLE_WIN_BONUS//
		//----------------//
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
		{
			if( !mAVATAR_OBJECT[index01].mCheckValidState )
			{
				continue;
			}
			if( mUSER[index01].mMoveZoneResult == 1 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0 )
			{
				continue;
			}
			if( ( mAVATAR_OBJECT[index01].mDATA.aTribe != tWinTribe ) && ( mAVATAR_OBJECT[index01].mDATA.aTribe != ReturnAllianceTribe( tWinTribe ) ) )
			{
				GetLevelBattleRewardInfo( mAVATAR_OBJECT[index01].mDATA.aLevel1, mAVATAR_OBJECT[index01].mDATA.aLevel2, &tGainExperience, &tGainMoney, FALSE );
				tMedalCnt = 3;
			}
			else{
				GetLevelBattleRewardInfo( mAVATAR_OBJECT[index01].mDATA.aLevel1, mAVATAR_OBJECT[index01].mDATA.aLevel2, &tGainExperience, &tGainMoney, TRUE );
				tMedalCnt = 10;
			}
#if 0
			if( mAVATAR_OBJECT[index01].mDATA.aLifeValue < 1 )
			{
				continue;
			}
			GetLevelBattleRewardInfo( mAVATAR_OBJECT[index01].mDATA.aLevel1, &tGainExperience, &tGainMoney );
#endif
			if( tGainExperience < 1 )
			{
				continue;
			}
			mTRANSFER.B_AVATAR_CHANGE_INFO_2( 1, tGainExperience );
			mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			mUTIL.ProcessForExperience( index01, tGainExperience, 0, 0 );
			if( !mUTIL.CheckOverMaximum( mUSER[index01].mAvatarInfo.aMoney, tGainMoney ) )
			{
				mUSER[index01].mAvatarInfo.aMoney += tGainMoney;
				mTRANSFER.B_AVATAR_CHANGE_INFO_2( 23, tGainMoney );
				mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			}
			if(((mSERVER_INFO.mServerNumber >= 154) && (mSERVER_INFO.mServerNumber <= 164))
#ifdef __GOD__
				||((mSERVER_INFO.mServerNumber >= 120) && (mSERVER_INFO.mServerNumber <= 122))
#endif
			)
				{
				mUTIL.ProcessForDropItem( 8, 1328, tMedalCnt, 0, 0, &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], mAVATAR_OBJECT[index01].mDATA.aTribe );
			}
			else if((mSERVER_INFO.mServerNumber >= 150) && (mSERVER_INFO.mServerNumber <= 153)){

				mUTIL.ProcessForDropItem( 8, 1329, tMedalCnt, 0, 0, &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], mAVATAR_OBJECT[index01].mDATA.aTribe );
			}
			else{ 
				mUTIL.ProcessForDropItem( 8, 1330, tMedalCnt, 0, 0, &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], mAVATAR_OBJECT[index01].mDATA.aTribe );
			}
			//-----------//
			//.._...//
			//-----------//
			if( mUSER[index01].mHealthState == 5 )
			{
				continue;
			}
			//-----------//
			//-----------//
			//-----------//
			//-----//
			//QUEST//
			//-----//
			if( ( mUSER[index01].mAvatarInfo.aQuestInfo[2] == 8 ) && ( mUSER[index01].mAvatarInfo.aQuestInfo[3] == mSERVER_INFO.mServerNumber ) && ( mAVATAR_OBJECT[index01].ReturnQuestPresentState() == 2 ) )
			{
				mUSER[index01].mAvatarInfo.aQuestInfo[4]++;
				mTRANSFER.B_PROCESS_QUEST_RECV( 0, 9, 0, 0, 0, 0 );
				mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			}
			//-----//
			//-----//
			//-----//
		}
		//----------------//
		//----------------//
		//----------------//
		return;
	case 4 :
		if( mZone049TypePostTick < ( 60 * 1 ) )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 8;
		CopyMemory( &tBroadcastInfoData[0], &mZone049TypeZoneIndex, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone049TypeState[mZone049TypeZoneIndex] = 5;
		mZone049TypePostTick = 0;
		mZone049TypeBattleState = 0;
		//--------------//
		//DELETE_MONSTER//
		//--------------//
		mSUMMON.DeleteMonster();
		//--------------//
		//--------------//
		//--------------//
		return;
	case 5 :
		switch( mZone049TypeZoneIndex )
		{
		case 0 :
			if( mZone049TypePostTick < ( 120 * 30 ) )
			{
				return;
			}
			break;
		case 1 :
			if( mZone049TypePostTick < ( 120 * 60 ) )
			{
				return;
			}
			break;
		case 2 :
			if( mZone049TypePostTick < ( 120 * 60 ) )
			{
				return;
			}
			break;
		case 3 :
			if( mZone049TypePostTick < ( 120 * 120 ) )
			{
				return;
			}
			break;
		case 4 :
			if( mZone049TypePostTick < ( 120 * 120 ) )
			{
				return;
			}
			break;
		case 5 :
			if( mZone049TypePostTick < ( 120 * 120 ) )
			{
				return;
			}
			break;
#ifdef __GOD__
		case 6 :

			LOG_TO_FILE_1("120 map 50 dakika\n", mZone049TypePostTick);
			if( mZone049TypePostTick < ( 120 * 25 ) )
			{
				return;
			}
			break;
		case 7 :
			if( mZone049TypePostTick < ( 120 * 120 ) )
			{
				return;
			}
			break;
		case 8 :
			if( mZone049TypePostTick < ( 120 * 120 ) )
			{
				return;
			}
			break;
#endif
		}
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
		{
			mUSER[index01].Quit(__FILE__, __FUNCTION__, __LINE__);
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 9;
		CopyMemory( &tBroadcastInfoData[0], &mZone049TypeZoneIndex, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone049TypeState[mZone049TypeZoneIndex] = 0;
		mZone049TypePostTick = 0;
		mZone049TypeBattleState = 0;
		return;
	}
}
//PROCESS_ZONE_051_TYPE (������ 20 ~ 29).
void MyGame::Process_Zone_051_TYPE( void )
{
	int index01;
	int tBroadcastInfoSort;
	BYTE tBroadcastInfoData[MAX_BROADCAST_INFO_SIZE];
	int tExistStone[4];
	int tWinTribe;
	int tGainExperience;
	int tGainMoney;
	int tMedalCnt = 0;
	int tRemainTime = 1800;

	if( !mCheckActiveBattleLogic )
	{
		return;
	}
	mZone051TypePostTick++;
	switch( mWorldInfo.mZone051TypeState[mZone051TypeZoneIndex] )
	{
	case 0 :
		switch( mZone051TypeBattleState )
		{
		case 0 :
			if( ( mZone051TypeZoneIndex >= 3 ) && ( mZone051TypeZoneIndex <= 5 ) )
			{
				if( mWorldInfo.mTribeSymbolBattle == 1 )
				{
					return;
				}
			}
			if( mZone051TypePostTick < ( 120 * 5 ) )
			{
				return;
			}
			mZone051TypePostTick = 0;
			mZone051TypeBattleState = 1;
			mZone051TypeRemainTime = 10;
			return;
		case 1 :
			if( ( mZone051TypePostTick % ( 120 * 1 ) ) != 0 )
			{
				return;
			}
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 10;
			CopyMemory( &tBroadcastInfoData[0], &mZone051TypeZoneIndex, 4 );
			CopyMemory( &tBroadcastInfoData[4], &mZone051TypeRemainTime, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mZone051TypeRemainTime--;
			if( mZone051TypeRemainTime == 0 )
			{
				mZone051TypePostTick = 0;
				mZone051TypeBattleState = 2;
			}
			return;
		case 2 :
			if( mZone051TypePostTick < ( 120 * 1 ) )
			{
				return;
			}
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 11;
			CopyMemory( &tBroadcastInfoData[0], &mZone051TypeZoneIndex, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mZone051TypeState[mZone051TypeZoneIndex] = 1;
			mZone051TypePostTick = 0;
			mZone051TypeBattleState = 0;
			return;
		}
		return;
	case 1 :
		if( mZone051TypePostTick < ( 120 * 3 ) )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 12;
		CopyMemory( &tBroadcastInfoData[0], &mZone051TypeZoneIndex, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone051TypeState[mZone051TypeZoneIndex] = 2;
		mZone051TypePostTick = 0;
		mZone051TypeBattleState = 0;
		return;
	case 2 :
		if( mZone051TypePostTick < ( 120 * 1 ) )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 13;
		CopyMemory( &tBroadcastInfoData[0], &mZone051TypeZoneIndex, 4 );
		CopyMemory( &tBroadcastInfoData[4], &tRemainTime, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone051TypeState[mZone051TypeZoneIndex] = 3;
		mZone051TypePostTick = 0;
		mZone051TypeBattleState = 0;
		//--------------//
		//SUMMON_MONSTER//
		//--------------//
		mSUMMON.SummonMonsterWithoutTimeLimit();
		//--------------//
		//--------------//
		//--------------//
		return;
	case 3 :
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
		{
			if( !mAVATAR_OBJECT[index01].mCheckValidState )
			{
				continue;
			}
			if( mUSER[index01].mMoveZoneResult == 1 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0 )
			{
				continue;
			}
			break;
		}
		if( index01 == mSERVER.mMAX_USER_NUM )
		{
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 14;
			CopyMemory( &tBroadcastInfoData[0], &mZone051TypeZoneIndex, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mZone051TypeState[mZone051TypeZoneIndex] = 5;
			mZone051TypePostTick = 0;
			mZone051TypeBattleState = 0;
			//--------------//
			//DELETE_MONSTER//
			//--------------//
			mSUMMON.DeleteMonster();
			//--------------//
			//--------------//
			//--------------//
			return;
		}
#if 0
		if( mZone051TypePostTick == ( 120 * 30 ) )
		{
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 15;
			CopyMemory( &tBroadcastInfoData[0], &mZone051TypeZoneIndex, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mZone051TypeState[mZone051TypeZoneIndex] = 5;
			mZone051TypePostTick = 0;
			mZone051TypeBattleState = 0;
			//--------------//
			//DELETE_MONSTER//
			//--------------//
			mSUMMON.DeleteMonster();
			//--------------//
			//--------------//
			//--------------//
			return;
		}
#endif
		tExistStone[0] = 0;
		tExistStone[1] = 0;
		tExistStone[2] = 0;
		tExistStone[3] = 0;
		for( index01 = 0 ; index01 < MAX_MONSTER_OBJECT_BASE_1_NUM ; index01++ )
		{
			if( !mMONSTER_OBJECT[index01].mCheckValidState )
			{
				continue;
			}
			switch( mMONSTER_OBJECT[index01].mMONSTER_INFO->mSpecialType )
			{
			case 21 :
				tExistStone[0] = 1;
				break;
			case 22 :
				tExistStone[1] = 1;
				break;
			case 23 :
				tExistStone[2] = 1;
				break;
			case 29 :
				tExistStone[3] = 1;
				break;
			}
		}
		//-----------//
		//BATTLE_INFO//
		//-----------//
		if( ( mZone051TypePostTick % 10 ) == 0 )
		{
			mTRANSFER.B_051_TYPE_BATTLE_INFO( &tExistStone[0], tRemainTime - (int) (mZone051TypePostTick * 0.5));
			mUTIL.Broadcast( FALSE, NULL, 0, false );
		}
		//-----------//
		//-----------//
		//-----------//

		tWinTribe = -1;
		if( tWinTribe == -1 )
		{
			switch( ReturnAllianceTribe( 0 ) )
			{
			case 1 :
				if( ( tExistStone[0] ) && ( !tExistStone[2] ) && ( !tExistStone[3] ) )
				{
					tWinTribe = 0;
				}
				break;
			case 2 :
				if( ( tExistStone[0] ) && ( !tExistStone[1] ) && ( !tExistStone[3] ) )
				{
					tWinTribe = 0;
				}
				break;
			case 3 :
				if( ( tExistStone[0] ) && ( !tExistStone[1] ) && ( !tExistStone[2] ) )
				{
					tWinTribe = 0;
				}
				break;
			default :
				if( ( tExistStone[0] ) && ( !tExistStone[1] ) && ( !tExistStone[2] ) && ( !tExistStone[3] ) )
				{
					tWinTribe = 0;
				}
				break;
			}
		}
		if( tWinTribe == -1 )
		{
			switch( ReturnAllianceTribe( 1 ) )
			{
			case 0 :
				if( ( tExistStone[1] ) && ( !tExistStone[2] ) && ( !tExistStone[3] ) )
				{
					tWinTribe = 1;
				}
				break;
			case 2 :
				if( ( !tExistStone[0] ) && ( tExistStone[1] ) && ( !tExistStone[3] ) )
				{
					tWinTribe = 1;
				}
				break;
			case 3 :
				if( ( !tExistStone[0] ) && ( tExistStone[1] ) && ( !tExistStone[2] ) )
				{
					tWinTribe = 1;
				}
				break;
			default :
				if( ( !tExistStone[0] ) && ( tExistStone[1] ) && ( !tExistStone[2] ) && ( !tExistStone[3] ) )
				{
					tWinTribe = 1;
				}
				break;
			}
		}
		if( tWinTribe == -1 )
		{
			switch( ReturnAllianceTribe( 2 ) )
			{
			case 0 :
				if( ( !tExistStone[1] ) && ( tExistStone[2] ) && ( !tExistStone[3] ) )
				{
					tWinTribe = 2;
				}
				break;
			case 1 :
				if( ( !tExistStone[0] ) && ( tExistStone[2] ) && ( !tExistStone[3] ) )
				{
					tWinTribe = 2;
				}
				break;
			case 3 :
				if( ( !tExistStone[0] ) && ( !tExistStone[1] ) && ( tExistStone[2] ) )
				{
					tWinTribe = 2;
				}
				break;
			default :
				if( ( !tExistStone[0] ) && ( !tExistStone[1] ) && ( tExistStone[2] ) && ( !tExistStone[3] ) )
				{
					tWinTribe = 2;
				}
				break;
			}
		}
		if( tWinTribe == -1 )
		{
			switch( ReturnAllianceTribe( 3 ) )
			{
			case 0 :
				if( ( !tExistStone[1] ) && ( !tExistStone[2] ) && ( tExistStone[3] ) )
				{
					tWinTribe = 3;
				}
				break;
			case 1 :
				if( ( !tExistStone[0] ) && ( !tExistStone[2] ) && ( tExistStone[3] ) )
				{
					tWinTribe = 3;
				}
				break;
			case 2 :
				if( ( !tExistStone[0] ) && ( !tExistStone[1] ) && ( tExistStone[3] ) )
				{
					tWinTribe = 3;
				}
				break;
			default :
				if( ( !tExistStone[0] ) && ( !tExistStone[1] ) && ( !tExistStone[2] ) && ( tExistStone[3] ) )
				{
					tWinTribe = 3;
				}
				break;
			}
		}
#if 0
		if( tWinTribe == -1 )
		{
			return;
		}
#endif
		if (-1 == tWinTribe){
			if( mZone051TypePostTick != ( 120 * 30 )){
				return;
			}
			else{
				tWinTribe = -2;
			}
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 16;
		CopyMemory( &tBroadcastInfoData[0], &mZone051TypeZoneIndex, 4 );
		CopyMemory( &tBroadcastInfoData[4], &tWinTribe, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone051TypeState[mZone051TypeZoneIndex] = 4;
		mZone051TypePostTick = 0;
		mZone051TypeBattleState = 0;
		//----------------//
		//BATTLE_WIN_BONUS//
		//----------------//
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
		{
			if( !mAVATAR_OBJECT[index01].mCheckValidState )
			{
				continue;
			}
			if( mUSER[index01].mMoveZoneResult == 1 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0 )
			{
				continue;
			}
			if( ( mAVATAR_OBJECT[index01].mDATA.aTribe != tWinTribe ) && ( mAVATAR_OBJECT[index01].mDATA.aTribe != ReturnAllianceTribe( tWinTribe ) ) )
			{
				GetLevelBattleRewardInfo( mAVATAR_OBJECT[index01].mDATA.aLevel1, mAVATAR_OBJECT[index01].mDATA.aLevel2, &tGainExperience, &tGainMoney, FALSE );
				tMedalCnt = 3;
			}
			else{
				GetLevelBattleRewardInfo( mAVATAR_OBJECT[index01].mDATA.aLevel1, mAVATAR_OBJECT[index01].mDATA.aLevel2, &tGainExperience, &tGainMoney, TRUE );
				tMedalCnt = 10;
			}
#if 0
			if( ( mAVATAR_OBJECT[index01].mDATA.aTribe != tWinTribe ) && ( mAVATAR_OBJECT[index01].mDATA.aTribe != ReturnAllianceTribe( tWinTribe ) ) )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aLifeValue < 1 )
			{
				continue;
			}
			GetLevelBattleRewardInfo( mAVATAR_OBJECT[index01].mDATA.aLevel1, &tGainExperience, &tGainMoney );
#endif
			if( tGainExperience < 1 )
			{
				continue;
			}
			mTRANSFER.B_AVATAR_CHANGE_INFO_2( 1, tGainExperience );
			mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			mUTIL.ProcessForExperience( index01, tGainExperience, 0, 0 );
			if( !mUTIL.CheckOverMaximum( mUSER[index01].mAvatarInfo.aMoney, tGainMoney ) )
			{
				mUSER[index01].mAvatarInfo.aMoney += tGainMoney;
				mTRANSFER.B_AVATAR_CHANGE_INFO_2( 23, tGainMoney );
				mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			}
			if((mSERVER_INFO.mServerNumber >= 154) && (mSERVER_INFO.mServerNumber <= 164)){
				mUTIL.ProcessForDropItem( 8, 1328, tMedalCnt, 0, 0, &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], mAVATAR_OBJECT[index01].mDATA.aTribe );
			}
			else if((mSERVER_INFO.mServerNumber >= 150) && (mSERVER_INFO.mServerNumber <= 153)){

				mUTIL.ProcessForDropItem( 8, 1329, tMedalCnt, 0, 0, &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], mAVATAR_OBJECT[index01].mDATA.aTribe );
			}
			else{ 
				mUTIL.ProcessForDropItem( 8, 1330, tMedalCnt, 0, 0, &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], mAVATAR_OBJECT[index01].mDATA.aTribe );
			}
			//-----------//
			//.._...//
			//-----------//
			if( mUSER[index01].mHealthState == 5 )
			{
				continue;
			}
			//-----------//
			//-----------//
			//-----------//
			//-----//
			//QUEST//
			//-----//
			if( ( mUSER[index01].mAvatarInfo.aQuestInfo[2] == 8 ) && ( mUSER[index01].mAvatarInfo.aQuestInfo[3] == mSERVER_INFO.mServerNumber ) && ( mAVATAR_OBJECT[index01].ReturnQuestPresentState() == 2 ) )
			{
				mUSER[index01].mAvatarInfo.aQuestInfo[4]++;
				mTRANSFER.B_PROCESS_QUEST_RECV( 0, 9, 0, 0, 0, 0 );
				mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			}
			//-----//
			//-----//
			//-----//
		}
		//----------------//
		//----------------//
		//----------------//
		return;
	case 4 :
		if( mZone051TypePostTick < ( 120 * 1 ) )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 17;
		CopyMemory( &tBroadcastInfoData[0], &mZone051TypeZoneIndex, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone051TypeState[mZone051TypeZoneIndex] = 5;
		mZone051TypePostTick = 0;
		mZone051TypeBattleState = 0;
		//--------------//
		//DELETE_MONSTER//
		//--------------//
		mSUMMON.DeleteMonster();
		//--------------//
		//--------------//
		//--------------//
		return;
	case 5 :
		switch( mZone051TypeZoneIndex )
		{
		case 0 :
			if( mZone051TypePostTick < ( 120 * 60 ) )
			{
				return;
			}
			break;
		case 1 :
			if( mZone051TypePostTick < ( 120 * 60 ) )
			{
				return;
			}
			break;
		case 2 :
			if( mZone051TypePostTick < ( 120 * 60 ) )
			{
				return;
			}
			break;
		case 3 :
			if( mZone051TypePostTick < ( 120 * 120 ) )
			{
				return;
			}
			break;
		case 4 :
			if( mZone051TypePostTick < ( 120 * 120 ) )
			{
				return;
			}
			break;
		case 5 :
			if( mZone051TypePostTick < ( 120 * 120 ) )
			{
				return;
			}
			break;
		}
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
		{
			mUSER[index01].Quit(__FILE__, __FUNCTION__, __LINE__);
			               
			               
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 18;
		CopyMemory( &tBroadcastInfoData[0], &mZone051TypeZoneIndex, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone051TypeState[mZone051TypeZoneIndex] = 0;
		mZone051TypePostTick = 0;
		mZone051TypeBattleState = 0;
		return;
	}
}
//PROCESS_ZONE_053_TYPE
void MyGame::Process_Zone_053_TYPE( void )
{
	int index01;
	int tBroadcastInfoSort;
	BYTE tBroadcastInfoData[MAX_BROADCAST_INFO_SIZE];
	int tGainExperience;
	int tGainMoney, tWinTribe = 0;
	BOOL tEnd = FALSE;
	int tMedalCnt = 0;
	int tRemainTime = 1800;

	if( !mCheckActiveBattleLogic )
	{
		return;
	}
	mZone053TypePostTick++;
	switch( mWorldInfo.mZone053TypeState[mZone053TypeZoneIndex] )
	{
	case 0 :
		switch( mZone053TypeBattleState )
		{
		case 0 :
			if( ( mZone053TypeZoneIndex >= 2 ) && ( mZone053TypeZoneIndex <= 9 ) )
			{
				if( mWorldInfo.mTribeSymbolBattle == 1 )
				{
					return;
				}
			}
			if( mZone053TypePostTick < ( 120 * 5 ) )
			{
				return;
			}
			mZone053TypePostTick = 0;
			mZone053TypeBattleState = 1;
			mZone053TypeRemainTime = 10;
			return;
		case 1 :
			if( ( mZone053TypePostTick % ( 120 * 1 ) ) != 0 )
			{
				return;
			}
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 19;
			CopyMemory( &tBroadcastInfoData[0], &mZone053TypeZoneIndex, 4 );
			CopyMemory( &tBroadcastInfoData[4], &mZone053TypeRemainTime, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mZone053TypeRemainTime--;
			if( mZone053TypeRemainTime == 0 )
			{
				mZone053TypePostTick = 0;
				mZone053TypeBattleState = 2;
			}
			return;
		case 2 :
			if( mZone053TypePostTick < ( 120 * 1 ) )
			{
				return;
			}
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 20;
			CopyMemory( &tBroadcastInfoData[0], &mZone053TypeZoneIndex, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mZone053TypeState[mZone053TypeZoneIndex] = 1;
			mZone053TypePostTick = 0;
			mZone053TypeBattleState = 0;
			return;
		}
		return;
	case 1 :
		if( mZone053TypePostTick < ( 120 * 3 ) )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 21;
		CopyMemory( &tBroadcastInfoData[0], &mZone053TypeZoneIndex, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone053TypeState[mZone053TypeZoneIndex] = 2;
		mZone053TypePostTick = 0;
		mZone053TypeBattleState = 0;
		return;
	case 2 :
		if( mZone053TypePostTick < ( 120 * 1 ) )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 22;
		CopyMemory( &tBroadcastInfoData[0], &mZone053TypeZoneIndex, 4 );
		CopyMemory( &tBroadcastInfoData[4], &tRemainTime, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone053TypeState[mZone053TypeZoneIndex] = 3;
		mZone053TypePostTick = 0;
		mZone053TypeBattleState = 0;
		//--------------//
		//SUMMON_MONSTER//
		//--------------//
		mSUMMON.SummonMonsterWithoutTimeLimit();
		//--------------//
		//--------------//
		//--------------//
		return;
	case 3 :
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
		{
			if( !mAVATAR_OBJECT[index01].mCheckValidState )
			{
				continue;
			}
			if( mUSER[index01].mMoveZoneResult == 1 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0 )
			{
				continue;
			}
			break;
		}
		if( index01 == mSERVER.mMAX_USER_NUM )
		{
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 23;
			CopyMemory( &tBroadcastInfoData[0], &mZone053TypeZoneIndex, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mZone053TypeState[mZone053TypeZoneIndex] = 5;
			mZone053TypePostTick = 0;
			mZone053TypeBattleState = 0;
			//--------------//
			//DELETE_MONSTER//
			//--------------//
			mSUMMON.DeleteMonster();
			//--------------//
			//--------------//
			//--------------//
			return;
		}
		if( mZone053TypePostTick == ( 120 * 30 ) ){
			tEnd = TRUE;
		}
#if 0
		if( mZone053TypePostTick == ( 120 * 30 ) )
		{
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 24;
			CopyMemory( &tBroadcastInfoData[0], &mZone053TypeZoneIndex, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mZone053TypeState[mZone053TypeZoneIndex] = 5;
			mZone053TypePostTick = 0;
			mZone053TypeBattleState = 0;
			//--------------//
			//DELETE_MONSTER//
			//--------------//
			mSUMMON.DeleteMonster();
			//--------------//
			//--------------//
			//--------------//
			return;
		}
#endif
		if(!tEnd){
			if(( mZone053TypePostTick % 10 ) == 0 )
			{
				mTRANSFER.B_053_TYPE_BATTLE_INFO(tRemainTime - (int)(mZone053TypePostTick * 0.5));
				mUTIL.Broadcast( FALSE, NULL, 0, false );
			}
			if( mZone053TypePostAvatarIndex == -1 )
			{
				for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
				{
					if( !mAVATAR_OBJECT[index01].mCheckValidState )
					{
						continue;
					}
					if( mUSER[index01].mMoveZoneResult == 1 )
					{
						continue;
					}
					if( mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0 )
					{
						continue;
					}
					if( mAVATAR_OBJECT[index01].mDATA.aLifeValue < 1 )
					{
						continue;
					}
					if( mAVATAR_OBJECT[index01].mDATA.aAction.aSort == 33 )
					{
						continue;
					}
					if( !mUTIL.CheckInRange( &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mZone053TypeBattlePostLocation[0], mZone053TypeBattlePostRadius ) )
					{
						continue;
					}
					break;
				}
				if( index01 == mSERVER.mMAX_USER_NUM )
				{
					return;
				}
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tBroadcastInfoSort = 25;
				CopyMemory( &tBroadcastInfoData[0], &mZone053TypeZoneIndex, 4 );
				tWinTribe = static_cast<int> (mAVATAR_OBJECT[index01].mDATA.aTribe);
				//CopyMemory( &tBroadcastInfoData[4], &mAVATAR_OBJECT[index01].mDATA.aTribe, 4 );
				CopyMemory( &tBroadcastInfoData[4], &tWinTribe, 4 );
				CopyMemory( &tBroadcastInfoData[8], &mAVATAR_OBJECT[index01].mDATA.aName[0], MAX_AVATAR_NAME_LENGTH );
				mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
				//--------------//
				//--------------//
				//--------------//
				mZone053TypeRemainTime = 5;
				mZone053TypePostAvatarIndex = index01;
				mZone053TypePostAvatarUniqueNumber = mAVATAR_OBJECT[index01].mUniqueNumber;
				mZone053TypeSaveTick = mZone053TypePostTick;
				return;
			}
			if( ( !mAVATAR_OBJECT[mZone053TypePostAvatarIndex].mCheckValidState ) || ( mAVATAR_OBJECT[mZone053TypePostAvatarIndex].mUniqueNumber != mZone053TypePostAvatarUniqueNumber ) || ( mAVATAR_OBJECT[mZone053TypePostAvatarIndex].mDATA.aLifeValue < 1 ) || ( !mUTIL.CheckInRange( &mAVATAR_OBJECT[mZone053TypePostAvatarIndex].mDATA.aAction.aLocation[0], &mZone053TypeBattlePostLocation[0], mZone053TypeBattlePostRadius ) ) || ( mUSER[mZone053TypePostAvatarIndex].mMoveZoneResult == 1 ) || ( mAVATAR_OBJECT[mZone053TypePostAvatarIndex].mDATA.aVisibleState == 0 ) || ( mAVATAR_OBJECT[mZone053TypePostAvatarIndex].mDATA.aAction.aSort == 33 ) )
			{
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tBroadcastInfoSort = 26;
				CopyMemory( &tBroadcastInfoData[0], &mZone053TypeZoneIndex, 4 );
				mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
				//--------------//
				//--------------//
				//--------------//
				mZone053TypePostAvatarIndex = -1;
				mZone053TypePostAvatarUniqueNumber = 0;
				return;
			}
			if( ( ( mZone053TypePostTick - mZone053TypeSaveTick ) % 120 ) != 20 )
			{
				return;
			}
			if( mZone053TypeRemainTime > 0 )
			{
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tBroadcastInfoSort = 27;
				CopyMemory( &tBroadcastInfoData[0], &mZone053TypeZoneIndex, 4 );
				CopyMemory( &tBroadcastInfoData[4], &mZone053TypeRemainTime, 4 );
				mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
				//--------------//
				//--------------//
				//--------------//
				mZone053TypeRemainTime--;
				return;
			}
			tWinTribe = (int) mAVATAR_OBJECT[mZone053TypePostAvatarIndex].mDATA.aTribe;
		}
		else{
			tWinTribe = -2;
		}

		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 28;
		CopyMemory( &tBroadcastInfoData[0], &mZone053TypeZoneIndex, 4 );
		//iTribe = static_cast<int> (mAVATAR_OBJECT[mZone053TypePostAvatarIndex].mDATA.aTribe);
		CopyMemory( &tBroadcastInfoData[4], &tWinTribe, 4 );
		CopyMemory( &tBroadcastInfoData[8], &mAVATAR_OBJECT[mZone053TypePostAvatarIndex].mDATA.aName[0], MAX_AVATAR_NAME_LENGTH );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone053TypeState[mZone053TypeZoneIndex] = 4;
		mZone053TypePostTick = 0;
		mZone053TypeBattleState = 0;
		//----------------//
		//BATTLE_WIN_BONUS//
		//----------------//
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
		{
			if( !mAVATAR_OBJECT[index01].mCheckValidState )
			{
				continue;
			}
			if( mUSER[index01].mMoveZoneResult == 1 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0 )
			{
				continue;
			}
#if 0
			if( ( mAVATAR_OBJECT[index01].mDATA.aTribe != mAVATAR_OBJECT[mZone053TypePostAvatarIndex].mDATA.aTribe ) && ( mAVATAR_OBJECT[index01].mDATA.aTribe != ReturnAllianceTribe( mAVATAR_OBJECT[mZone053TypePostAvatarIndex].mDATA.aTribe ) ) )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aLifeValue < 1 )
			{
				continue;
			}
			GetLevelBattleRewardInfo( mAVATAR_OBJECT[index01].mDATA.aLevel1, mAVATAR_OBJECT[index01].mDATA.aLevel2, &tGainExperience, &tGainMoney );
#endif
			if( ( mAVATAR_OBJECT[index01].mDATA.aTribe != tWinTribe ) && ( mAVATAR_OBJECT[index01].mDATA.aTribe != ReturnAllianceTribe( tWinTribe ) ) ){
				tMedalCnt = 3;
				GetLevelBattleRewardInfo( mAVATAR_OBJECT[index01].mDATA.aLevel1, mAVATAR_OBJECT[index01].mDATA.aLevel2, &tGainExperience, &tGainMoney, FALSE );
			}
			else{
				tMedalCnt = 10;
				GetLevelBattleRewardInfo( mAVATAR_OBJECT[index01].mDATA.aLevel1, mAVATAR_OBJECT[index01].mDATA.aLevel2, &tGainExperience, &tGainMoney, TRUE );
			}

			if( tGainExperience < 1 )
			{
				continue;
			}
			if(!tEnd){
				if( index01 == mZone053TypePostAvatarIndex )
				{
					tGainExperience *= 2;
					tGainMoney *= 2;
				}
				mTRANSFER.B_AVATAR_CHANGE_INFO_2( 1, tGainExperience );
				mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
				mUTIL.ProcessForExperience( index01, tGainExperience, 0, 0 );
				if( !mUTIL.CheckOverMaximum( mUSER[index01].mAvatarInfo.aMoney, tGainMoney ) )
				{
					mUSER[index01].mAvatarInfo.aMoney += tGainMoney;
					mTRANSFER.B_AVATAR_CHANGE_INFO_2( 23, tGainMoney );
					mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
				}
			}
			if((mSERVER_INFO.mServerNumber >= 154) && (mSERVER_INFO.mServerNumber <= 164)){
				mUTIL.ProcessForDropItem( 8, 1328, tMedalCnt, 0, 0, &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], mAVATAR_OBJECT[index01].mDATA.aTribe );
			}
			else if((mSERVER_INFO.mServerNumber >= 150) && (mSERVER_INFO.mServerNumber <= 153)){

				mUTIL.ProcessForDropItem( 8, 1329, tMedalCnt, 0, 0, &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], mAVATAR_OBJECT[index01].mDATA.aTribe );
			}
			else{ 
				mUTIL.ProcessForDropItem( 8, 1330, tMedalCnt, 0, 0, &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], mAVATAR_OBJECT[index01].mDATA.aTribe );
			}
			//-----------//
			//.._...//
			//-----------//
			if( mUSER[index01].mHealthState == 5 )
			{
				continue;
			}
			//-----------//
			//-----------//
			//-----------//
			//-----//
			//QUEST//
			//-----//
			if( ( mUSER[index01].mAvatarInfo.aQuestInfo[2] == 8 ) && ( mUSER[index01].mAvatarInfo.aQuestInfo[3] == mSERVER_INFO.mServerNumber ) && ( mAVATAR_OBJECT[index01].ReturnQuestPresentState() == 2 ) )
			{
				mUSER[index01].mAvatarInfo.aQuestInfo[4]++;
				mTRANSFER.B_PROCESS_QUEST_RECV( 0, 9, 0, 0, 0, 0 );
				mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			}
			//-----//
			//-----//
			//-----//
		}
		//----------------//
		//----------------//
		//----------------//
		return;
	case 4 :
		if( mZone053TypePostTick < ( 120 * 1 ) )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 29;
		CopyMemory( &tBroadcastInfoData[0], &mZone053TypeZoneIndex, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone053TypeState[mZone053TypeZoneIndex] = 5;
		mZone053TypePostTick = 0;
		mZone053TypeBattleState = 0;
		//--------------//
		//DELETE_MONSTER//
		//--------------//
		mSUMMON.DeleteMonster();
		//--------------//
		//--------------//
		//--------------//
		return;
	case 5 :
		switch( mZone053TypeZoneIndex )
		{
		case 0 :
			if( mZone053TypePostTick < ( 120 * 60 ) )
			{
				return;
			}
			break;
		case 1 :
			if( mZone053TypePostTick < ( 120 * 60 ) )
			{
				return;
			}
			break;
		case 2 :
			if( mZone053TypePostTick < ( 120 * 60 ) )
			{
				return;
			}
			break;
		case 3 :
			if( mZone053TypePostTick < ( 120 * 90 ) )
			{
				return;
			}
			break;
		case 4 :
			if( mZone053TypePostTick < ( 120 * 90 ) )
			{
				return;
			}
			break;
		case 5 :
			if( mZone053TypePostTick < ( 120 * 120 ) )
			{
				return;
			}
			break;
		case 6 :
			if( mZone053TypePostTick < ( 120 * 120 ) )
			{
				return;
			}
			break;
		case 7 :
			if( mZone053TypePostTick < ( 120 * 120 ) )
			{
				return;
			}
			break;
		case 8 :
			if( mZone053TypePostTick < ( 120 * 150 ) )
			{
				return;
			}
			break;
		case 9 :
			if( mZone053TypePostTick < ( 120 * 150 ) )
			{
				return;
			}
			break;
		}
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ ) {
			mUSER[index01].Quit(__FILE__, __FUNCTION__, __LINE__);
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 30;
		CopyMemory( &tBroadcastInfoData[0], &mZone053TypeZoneIndex, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone053TypeState[mZone053TypeZoneIndex] = 0;
		mZone053TypePostTick = 0;
		mZone053TypeBattleState = 0;
		return;
	}
}
//GET_LEVEL_BATTLE_REWARD_INFO
void MyGame::GetLevelBattleRewardInfo( int tLevel1, int tLevel2, int *tGainExperience, int *tGainMoney, BOOL tWin )
{
	*tGainExperience = 0;
	*tGainMoney = 0;
	
	
	
	// 2009.10.26 : �輺��
	// 26�� ������ 4�б�10�� ������Ʈ ������ ��ġ��
	// �Ʒ��� /*/�� //*/�� �����ָ� �ش� ������Ʈ���� ����˴ϴ�.
	// ���� �ʸ��� ��ġ�� �ִ� ����� �۾������� �ּ����� ���� �����ϴ�.
	// 2009.11.06 : �輺��
	// ������ �ִ� ������ ������, int ������ ���� �÷ξ �߻��ϴ� �ϱ� ������ �ϵ� �ڵ����� �ٽ� ����
	
	if( tLevel2 > 0 )
	{
		switch( tLevel2 )
		{
			case  1 :	
				*tGainExperience = 4810529;
				*tGainMoney = 10000000;
				break;
			case  2 :	
				*tGainExperience = 5002950;
				*tGainMoney = 10000000;
				break;
			case  3 :	
				*tGainExperience = 5203068;
				*tGainMoney = 10000000;
				break;
			case  4 :	
				*tGainExperience = 5411191;
				*tGainMoney = 10000000;
				break;
			case  5 :	
				*tGainExperience = 5627638;
				*tGainMoney = 15000000;
				break;
			case  6 :	
				*tGainExperience = 5852744;
				*tGainMoney = 15000000;
				break;
			case  7 :	
				*tGainExperience = 6086854;
				*tGainMoney = 15000000;
				break;
			case  8 :	
				*tGainExperience = 6330328;
				*tGainMoney = 15000000;
				break;
			case  9 :	
				*tGainExperience = 6583541;
				*tGainMoney = 20000000;
				break;
			case 10 :	
				*tGainExperience = 6846883;
				*tGainMoney = 20000000;
				break;
			case 11 :	
				*tGainExperience = 7120758;
				*tGainMoney = 20000000;
				break;
			case 12 :	
				*tGainExperience = 7405588;
				*tGainMoney = 20000000;
				break;
		}
		if(FALSE == tWin){
			*tGainExperience /= 2;
			*tGainMoney /= 2;
		}
		return;
	}
	switch( tLevel1 )
	{	
		case  10 :
			*tGainExperience = 140;
			*tGainMoney = 100000;
			break;
		case  11 :
			*tGainExperience = 315;
			*tGainMoney = 100000;
			break;
		case  12 :
			*tGainExperience = 499;
			*tGainMoney = 100000;
			break;
		case  13 :
			*tGainExperience = 676;
			*tGainMoney = 100000;
			break;
		case  14 :
			*tGainExperience = 847;
			*tGainMoney = 100000;
			break;
		case  15 :
			*tGainExperience = 1015;
			*tGainMoney = 100000;
			break;
		case  16 :
			*tGainExperience = 1180;
			*tGainMoney = 100000;
			break;
		case  17 :
			*tGainExperience = 1342;
			*tGainMoney = 100000;
			break;
		case  18 :
			*tGainExperience = 1502;
			*tGainMoney = 100000;
			break;
		case  19 :
			*tGainExperience = 1660;
			*tGainMoney = 100000;
			break;
		case  20 :
			*tGainExperience = 1817;
			*tGainMoney = 100000;
			break;
		case  21 :
			*tGainExperience = 1972;
			*tGainMoney = 100000;
			break;
		case  22 :
			*tGainExperience = 2126;
			*tGainMoney = 100000;
			break;
		case  23 :
			*tGainExperience = 2278;
			*tGainMoney = 100000;
			break;
		case  24 :
			*tGainExperience = 2429;
			*tGainMoney = 100000;
			break;
		case  25 :
			*tGainExperience = 2580;
			*tGainMoney = 100000;
			break;
		case  26 :
			*tGainExperience = 2729;
			*tGainMoney = 100000;
			break;
		case  27 :
			*tGainExperience = 2877;
			*tGainMoney = 100000;
			break;
		case  28 :
			*tGainExperience = 3025;
			*tGainMoney = 100000;
			break;
		case  29 :
			*tGainExperience = 3172;
			*tGainMoney = 100000;
			break;
		case  30 :
			*tGainExperience = 3318;
			*tGainMoney = 300000;
			break;
		case  31 :
			*tGainExperience = 3463;
			*tGainMoney = 300000;
			break;
		case  32 :
			*tGainExperience = 3608;
			*tGainMoney = 300000;
			break;
		case  33 :
			*tGainExperience = 3752;
			*tGainMoney = 300000;
			break;
		case  34 :
			*tGainExperience = 3895;
			*tGainMoney = 300000;
			break;
		case  35 :
			*tGainExperience = 4038;
			*tGainMoney = 300000;
			break;
		case  36 :
			*tGainExperience = 4180;
			*tGainMoney = 300000;
			break;
		case  37 :
			*tGainExperience = 4322;
			*tGainMoney = 300000;
			break;
		case  38 :
			*tGainExperience = 4463;
			*tGainMoney = 300000;
			break;
		case  39 :
			*tGainExperience = 4604;
			*tGainMoney = 300000;
			break;
		case  40 :
			*tGainExperience = 4744;
			*tGainMoney = 300000;
			break;
		case  41 :
			*tGainExperience = 4884;
			*tGainMoney = 300000;
			break;
		case  42 :
			*tGainExperience = 5023;
			*tGainMoney = 300000;
			break;
		case  43 :
			*tGainExperience = 5162;
			*tGainMoney = 300000;
			break;
		case  44 :
			*tGainExperience = 5300;
			*tGainMoney = 300000;
			break;
		case  45 :
			*tGainExperience = 5438;
			*tGainMoney = 300000;
			break;
		case  46 :
			*tGainExperience = 5576;
			*tGainMoney = 300000;
			break;
		case  47 :
			*tGainExperience = 5713;
			*tGainMoney = 300000;
			break;
		case  48 :
			*tGainExperience = 5850;
			*tGainMoney = 300000;
			break;
		case  49 :
			*tGainExperience = 5987;
			*tGainMoney = 300000;
			break;
		case  50 :
			*tGainExperience = 6123;
			*tGainMoney = 500000;
			break;
		case  51 :
			*tGainExperience = 6259;
			*tGainMoney = 500000;
			break;
		case  52 :
			*tGainExperience = 6395;
			*tGainMoney = 500000;
			break;
		case  53 :
			*tGainExperience = 6530;
			*tGainMoney = 500000;
			break;
		case  54 :
			*tGainExperience = 6665;
			*tGainMoney = 500000;
			break;
		case  55 :
			*tGainExperience = 6799;
			*tGainMoney = 500000;
			break;
		case  56 :
			*tGainExperience = 6934;
			*tGainMoney = 500000;
			break;
		case  57 :
			*tGainExperience = 7068;
			*tGainMoney = 500000;
			break;
		case  58 :
			*tGainExperience = 7202;
			*tGainMoney = 500000;
			break;
		case  59 :
			*tGainExperience = 7335;
			*tGainMoney = 500000;
			break;
		case  60 :
			*tGainExperience = 7468;
			*tGainMoney = 500000;
			break;
		case  61 :
			*tGainExperience = 7601;
			*tGainMoney = 500000;
			break;
		case  62 :
			*tGainExperience = 7734;
			*tGainMoney = 500000;
			break;
		case  63 :
			*tGainExperience = 7866;
			*tGainMoney = 500000;
			break;
		case  64 :
			*tGainExperience = 7999;
			*tGainMoney = 500000;
			break;
		case  65 :
			*tGainExperience = 8131;
			*tGainMoney = 500000;
			break;
		case  66 :
			*tGainExperience = 8262;
			*tGainMoney = 500000;
			break;
		case  67 :
			*tGainExperience = 8394;
			*tGainMoney = 500000;
			break;
		case  68 :
			*tGainExperience = 8525;
			*tGainMoney = 500000;
			break;
		case  69 :
			*tGainExperience = 8656;
			*tGainMoney = 500000;
			break;
		case  70 :
			*tGainExperience = 8787;
			*tGainMoney = 700000;
			break;
		case  71 :
			*tGainExperience = 8918;
			*tGainMoney = 700000;
			break;
		case  72 :
			*tGainExperience = 9048;
			*tGainMoney = 700000;
			break;
		case  73 :
			*tGainExperience = 9178;
			*tGainMoney = 700000;
			break;
		case  74 :
			*tGainExperience = 9308;
			*tGainMoney = 700000;
			break;
		case  75 :
			*tGainExperience = 9438;
			*tGainMoney = 700000;
			break;
		case  76 :
			*tGainExperience = 9568;
			*tGainMoney = 700000;
			break;
		case  77 :
			*tGainExperience = 9697;
			*tGainMoney = 700000;
			break;
		case  78 :
			*tGainExperience = 9826;
			*tGainMoney = 700000;
			break;
		case  79 :
			*tGainExperience = 9955;
			*tGainMoney = 700000;
			break;
		case  80 :
			*tGainExperience = 10084;
			*tGainMoney = 700000;
			break;
		case  81 :
			*tGainExperience = 10213;
			*tGainMoney = 700000;
			break;
		case  82 :
			*tGainExperience = 10341;
			*tGainMoney = 700000;
			break;
		case  83 :
			*tGainExperience = 10469;
			*tGainMoney = 700000;
			break;
		case  84 :
			*tGainExperience = 10598;
			*tGainMoney = 700000;
			break;
		case  85 :
			*tGainExperience = 10725;
			*tGainMoney = 700000;
			break;
		case  86 :
			*tGainExperience = 10853;
			*tGainMoney = 700000;
			break;
		case  87 :
			*tGainExperience = 10981;
			*tGainMoney = 700000;
			break;
		case  88 :
			*tGainExperience = 11108;
			*tGainMoney = 700000;
			break;
		case  89 :
			*tGainExperience = 17585;
			*tGainMoney = 700000;
			break;
		case  90 :
			*tGainExperience = 11408;
			*tGainMoney = 1000000;
			break;
		case  91 :
			*tGainExperience = 12660;
			*tGainMoney = 1000000;
			break;
		case  92 :
			*tGainExperience = 13548;
			*tGainMoney = 1000000;
			break;
		case  93 :
			*tGainExperience = 14250;
			*tGainMoney = 1000000;
			break;
		case  94 :
			*tGainExperience = 14835;
			*tGainMoney = 1000000;
			break;
		case  95 :
			*tGainExperience = 15340;
			*tGainMoney = 1000000;
			break;
		case  96 :
			*tGainExperience = 15786;
			*tGainMoney = 1000000;
			break;
		case  97 :
			*tGainExperience = 16186;
			*tGainMoney = 1000000;
			break;
		case  98 :
			*tGainExperience = 16551;
			*tGainMoney = 1000000;
			break;
		case  99 :
			*tGainExperience = 19254;
			*tGainMoney = 1000000;
			break;
		case 100 :
			*tGainExperience = 24979;
			*tGainMoney = 1000000;
			break;
		case 101 :
			*tGainExperience = 27721;
			*tGainMoney = 1000000;
			break;
		case 102 :
			*tGainExperience = 29667;
			*tGainMoney = 1000000;
			break;
		case 103 :
			*tGainExperience = 31203;
			*tGainMoney = 1000000;
			break;
		case 104 :
			*tGainExperience = 32484;
			*tGainMoney = 1000000;
			break;
		case 105 :
			*tGainExperience = 33590;
			*tGainMoney = 1000000;
			break;
		case 106 :
			*tGainExperience = 34566;
			*tGainMoney = 1000000;
			break;
		case 107 :
			*tGainExperience = 35444;
			*tGainMoney = 1000000;
			break;
		case 108 :
			*tGainExperience = 36241;
			*tGainMoney = 1000000;
			break;
		case 109 :
			*tGainExperience = 36975;
			*tGainMoney = 1000000;
			break;
		case 110 :
			*tGainExperience = 37654;
			*tGainMoney = 1000000;
			break;
		case 111 :
			*tGainExperience = 38287;
			*tGainMoney = 1000000;
			break;
		case 112 :
			*tGainExperience = 797423;
			*tGainMoney = 1000000;
			break;
		case 113 :
			*tGainExperience = 206915;
			*tGainMoney = 3000000;
			break;
		case 114 :
			*tGainExperience = 229625;
			*tGainMoney = 3000000;
			break;
		case 115 :
			*tGainExperience = 245740;
			*tGainMoney = 3000000;
			break;
		case 116 :
			*tGainExperience = 258464;
			*tGainMoney = 3000000;
			break;
		case 117 :
			*tGainExperience = 269077;
			*tGainMoney = 3000000;
			break;
		case 118 :
			*tGainExperience = 278236;
			*tGainMoney = 3000000;
			break;
		case 119 :
			*tGainExperience = 286326;
			*tGainMoney = 3000000;
			break;
		case 120 :
			*tGainExperience = 293592;
			*tGainMoney = 3000000;
			break;
		case 121 :
			*tGainExperience = 300201;
			*tGainMoney = 3000000;
			break;
		case 122 :
			*tGainExperience = 306275;
			*tGainMoney = 3000000;
			break;
		case 123 :
			*tGainExperience = 311902;
			*tGainMoney = 3000000;
			break;
		case 124 :
			*tGainExperience = 317149;
			*tGainMoney = 3000000;
			break;
		case 125 :
			*tGainExperience = 322070;
			*tGainMoney = 3000000;
			break;
		case 126 :
			*tGainExperience = 326708;
			*tGainMoney = 3000000;
			break;
		case 127 :
			*tGainExperience = 331096;
			*tGainMoney = 3000000;
			break;
		case 128 :
			*tGainExperience = 335263;
			*tGainMoney = 3000000;
			break;
		case 129 :
			*tGainExperience = 339233;
			*tGainMoney = 3000000;
			break;
		case 130 :
			*tGainExperience = 343025;
			*tGainMoney = 3000000;
			break;
		case 131 :
			*tGainExperience = 346657;
			*tGainMoney = 3000000;
			break;
		case 132 :
			*tGainExperience = 350142;
			*tGainMoney = 3000000;
			break;
		case 133 :
			*tGainExperience = 353494;
			*tGainMoney = 3000000;
			break;
		case 134 :
			*tGainExperience = 356723;
			*tGainMoney = 3000000;
			break;
		case 135 :
			*tGainExperience = 359839;
			*tGainMoney = 3000000;
			break;
		case 136 :
			*tGainExperience = 362851;
			*tGainMoney = 3000000;
			break;
		case 137 :
			*tGainExperience = 365766;
			*tGainMoney = 5000000;
			break;
		case 138 :
			*tGainExperience = 368592;
			*tGainMoney = 5000000;
			break;
		case 139 :
			*tGainExperience = 371332;
			*tGainMoney = 5000000;
			break;
		case 140 :
			*tGainExperience = 373996;
			*tGainMoney = 5000000;
			break;
		case 141 :
			*tGainExperience = 376583;
			*tGainMoney = 5000000;
			break;
		case 142 :
			*tGainExperience = 379103;
			*tGainMoney = 5000000;
			break;
		case 143 :
			*tGainExperience = 381556;
			*tGainMoney = 5000000;
			break;
		case 144 :
			*tGainExperience = 383950;
			*tGainMoney = 5000000;
			break;
		case 145 :
			*tGainExperience = 4625509;
			*tGainMoney = 5000000;
			break;
	}
	if(FALSE == tWin){
		*tGainExperience /= 2;
		*tGainMoney /= 2;
	}

}
//PROCESS_ZONE_038
void MyGame::Process_Zone_038( void )
{
	int index01, iTribe;
	int tBroadcastInfoSort;
	BYTE tBroadcastInfoData[MAX_BROADCAST_INFO_SIZE];
	
	
	mSUMMON.SummonMonster();
	mSUMMON.SummonBossMonster();

	if( !mCheckActiveBattleLogic )
	{
		return;
	}
	mZone038PostTick++;
	switch( mZone038BattleState )
	{
	case 0 :
		if( mWorldInfo.mTribeSymbolBattle == 1 )
		{
			return;
		}
		if( mZone038PostTick < ( 120 * 5 ) )
		{
			return;
		}
		mZone038PostTick = 0;
		mZone038BattleState = 1;
		mZone038RemainTime = 10;
		return;
	case 1 :
		if( ( mZone038PostTick % ( 120 * 1 ) ) != 0 )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 33;
		CopyMemory( &tBroadcastInfoData[0], &mZone038RemainTime, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mZone038RemainTime--;
		if( mZone038RemainTime == 0 )
		{
			mZone038PostTick = 0;
			mZone038BattleState = 2;
		}
		return;
	case 2 :
		if( mZone038PostTick < ( 120 * 1 ) )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 34;
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mZone038PostTick = 0;
		mZone038BattleState = 3;
		return;
	case 3 :
		if( mZone038PostAvatarIndex == -1 )
		{
			for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
			{
				if( !mAVATAR_OBJECT[index01].mCheckValidState )
				{
					continue;
				}
				if( mUSER[index01].mMoveZoneResult == 1 )
				{
					continue;
				}
				if( mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0 )
				{
					continue;
				}
				if( mAVATAR_OBJECT[index01].mDATA.aLifeValue < 1 )
				{
					continue;
				}
				if( mAVATAR_OBJECT[index01].mDATA.aAction.aSort == 33 )
				{
					continue;
				}
				if( ( mAVATAR_OBJECT[index01].mDATA.aTribe == mWorldInfo.mZone038WinTribe ) || ( mAVATAR_OBJECT[index01].mDATA.aTribe == ReturnAllianceTribe( mWorldInfo.mZone038WinTribe ) ) )
				{
					continue;
				}
				if( !mUTIL.CheckInRange( &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mZone038BattlePostLocation[0], mZone038BattlePostRadius ) )
				{
					continue;
				}
				break;
			}
			if( index01 == mSERVER.mMAX_USER_NUM )
			{
				return;
			}
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 35;
			iTribe = static_cast<int> (mAVATAR_OBJECT[index01].mDATA.aTribe);
			CopyMemory( &tBroadcastInfoData[0], &iTribe, 4 );
			CopyMemory( &tBroadcastInfoData[4], &mAVATAR_OBJECT[index01].mDATA.aName[0], MAX_AVATAR_NAME_LENGTH );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mZone038PostTick = 0;
			mZone038RemainTime = 5;
			mZone038PostAvatarIndex = index01;
			mZone038PostAvatarUniqueNumber = mAVATAR_OBJECT[index01].mUniqueNumber;
			return;
		}
		if( ( !mAVATAR_OBJECT[mZone038PostAvatarIndex].mCheckValidState ) || ( mAVATAR_OBJECT[mZone038PostAvatarIndex].mUniqueNumber != mZone038PostAvatarUniqueNumber ) || ( mAVATAR_OBJECT[mZone038PostAvatarIndex].mDATA.aLifeValue < 1 ) || ( !mUTIL.CheckInRange( &mAVATAR_OBJECT[mZone038PostAvatarIndex].mDATA.aAction.aLocation[0], &mZone038BattlePostLocation[0], mZone038BattlePostRadius ) ) || ( mUSER[mZone038PostAvatarIndex].mMoveZoneResult == 1 ) || ( mAVATAR_OBJECT[mZone038PostAvatarIndex].mDATA.aVisibleState == 0 ) || ( mAVATAR_OBJECT[mZone038PostAvatarIndex].mDATA.aAction.aSort == 33 ) )
		{
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 36;
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mZone038PostAvatarIndex = -1;
			mZone038PostAvatarUniqueNumber = 0;
			return;
		}
		if( ( mZone038PostTick % 120 ) != 20 )
		{
			return;
		}
		if( mZone038RemainTime > 0 )
		{
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 37;
			CopyMemory( &tBroadcastInfoData[0], &mZone038RemainTime, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mZone038RemainTime--;
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 38;
		iTribe = static_cast<int> (mAVATAR_OBJECT[mZone038PostAvatarIndex].mDATA.aTribe);
		CopyMemory( &tBroadcastInfoData[0], &iTribe, 4 );
		CopyMemory( &tBroadcastInfoData[4], &mAVATAR_OBJECT[mZone038PostAvatarIndex].mDATA.aName[0], MAX_AVATAR_NAME_LENGTH );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone038WinTribe = mAVATAR_OBJECT[mZone038PostAvatarIndex].mDATA.aTribe;
		mZone038PostTick = 0;
		mZone038BattleState = 4;
		mSUMMON.SummonGuard( TRUE, TRUE );
		//-----//
		//QUEST//
		//-----//
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
		{
			if( !mAVATAR_OBJECT[index01].mCheckValidState )
			{
				continue;
			}
			if( mUSER[index01].mMoveZoneResult == 1 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aTribe != mAVATAR_OBJECT[mZone038PostAvatarIndex].mDATA.aTribe )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aLifeValue < 1 )
			{
				continue;
			}
			if( mUSER[index01].mHealthState == 5 )
			{
				continue;
			}
			if( ( mUSER[index01].mAvatarInfo.aQuestInfo[2] == 8 ) && ( mUSER[index01].mAvatarInfo.aQuestInfo[3] == mSERVER_INFO.mServerNumber ) && ( mAVATAR_OBJECT[index01].ReturnQuestPresentState() == 2 ) )
			{
				mUSER[index01].mAvatarInfo.aQuestInfo[4]++;
				mTRANSFER.B_PROCESS_QUEST_RECV( 0, 9, 0, 0, 0, 0 );
				mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			}
		}

		//-----//
		//-----//
		//-----//
		return;
	case 4 :
		if( mZone038PostTick < ( 120 * 180 ) )
		{
			return;
		}
		mZone038PostTick = 0;
		mZone038BattleState = 0;
		return;
	}
}
//PROCESS_ZONE_039_TYPE
void MyGame::Process_Zone_039_TYPE( void )
{
	int index01;

	if( !mCheckActiveBattleLogic )
	{
		return;
	}
	mZone039TypePostTick++;
	switch( mZone039TypeBattleState )
	{
	case 0 :
		return;
	case 1 :
		if( mZone039TypePostTick < ( 120 * 1 ) )
		{
			return;
		}
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
		{
			if( !mUSER[index01].mCheckValidState )
			{
				continue;
			}
			if( ( mUSER[index01].mAvatarInfo.aTribe != mWorldInfo.mZone038WinTribe ) && ( mUSER[index01].mAvatarInfo.aTribe != ReturnAllianceTribe( mWorldInfo.mZone038WinTribe ) ) )
			{
				mUSER[index01].Quit(__FILE__, __FUNCTION__, __LINE__);
			               
			               
			}
		}
		mZone039TypeBattleState = 0;
		mZone039TypePostTick = 0;
		return;
	}
}
//PROCESS_ZONE_175_TYPE
void MyGame::Process_Zone_175_TYPE( void )
{
	int index01;
	time_t tCountSeconds;
	struct tm *tPresentTime;
	int tBroadcastInfoSort;
	BYTE tBroadcastInfoData[MAX_BROADCAST_INFO_SIZE];
	int tGainExperience;
	int tGainMoney;

	if( !mCheckActiveBattleLogic )
	{
		return;
	}
	mZone175TypePostTick++;
	switch( mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] )
	{
	case  0 :
		switch( mZone175TypeBattleState )
		{
		case 0 :
			time( &tCountSeconds );
			tPresentTime = localtime( &tCountSeconds );
			if( tPresentTime == NULL )
			{
				return;
			}
			
#ifdef __TK__
			//wendensday, friday, sunday 20:00
			if( ( ( tPresentTime->tm_wday != 3 ) && ( tPresentTime->tm_wday != 5 ) && ( tPresentTime->tm_wday != 0 ) ) || ( tPresentTime->tm_hour != 20 ) || ( tPresentTime->tm_min > 10 ) )
			{
				return;
			}
#else
			//wendensday, friday, sunday 18:00
			if( ( ( tPresentTime->tm_wday != 3 ) && ( tPresentTime->tm_wday != 5 ) && ( tPresentTime->tm_wday != 0 ) ) || ( tPresentTime->tm_hour != 18 ) || ( tPresentTime->tm_min > 10 ) )
			{
				return;
			}
#endif

			mZone175TypePostTick = 0;
			mZone175TypeBattleState = 1;
			mZone175TypeRemainTime = 10;
			return;
		case 1 :
			if( ( mZone175TypePostTick % ( 120 * 1 ) ) != 0 )
			{
				return;
			}
			if (10 == mZone175TypeRemainTime){
				mGAMELOG.GL_661_START_MISSION_LABYRINTH ();
			}
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 63;
			CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
			CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
			CopyMemory( &tBroadcastInfoData[8], &mZone175TypeRemainTime, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mZone175TypeRemainTime--;
			if( mZone175TypeRemainTime == 0 )
			{
				mZone175TypePostTick = 0;
				mZone175TypeBattleState = 2;
			}
			return;
		case 2 :
			if( mZone175TypePostTick < ( 120 * 1 ) )
			{
				return;
			}
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 64;
			CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
			CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 1;
			mZone175TypePostTick = 0;
			mZone175TypeBattleState = 0;
			return;
		}
		return;
	case  1 :
		if( mZone175TypePostTick < ( 120 * 3 ) )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 65;
		CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
		CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 2;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		return;
	case  2 : //[1�ܰ�_�Ա�����]
		if( mZone175TypePostTick < ( 120 * 1 ) )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 66;
		CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
		CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 3;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		return;
	case  3 : //[1�ܰ�_�Ա����]
		if( mZone175TypePostTick < ( 120 * 2 ) )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 67;
		CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
		CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 4;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		return;
	case  4 : //[1�ܰ�_����]
		if( mZone175TypePostTick < ( 120 * 1 ) )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 68;
		CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
		CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 5;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		//--------------//
		//SUMMON_MONSTER//
		//--------------//
		mSUMMON.SummonMonsterWithoutTimeLimitForZone175Type( 1 );
		//--------------//
		//--------------//
		//--------------//
		return;
	case  5 : //[1�ܰ�_����]
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
		{
			if( !mAVATAR_OBJECT[index01].mCheckValidState )
			{
				continue;
			}
			if( mUSER[index01].mMoveZoneResult == 1 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0 )
			{
				continue;
			}
			break;
		}
		if( index01 == mSERVER.mMAX_USER_NUM )
		{
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 69;
			CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
			CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 23;
			mZone175TypePostTick = 0;
			mZone175TypeBattleState = 0;
			//--------------//
			//DELETE_MONSTER//
			//--------------//
			mSUMMON.DeleteMonster();
			//--------------//
			//--------------//
			//--------------//
			return;
		}
		if( mZone175TypePostTick == ( 120 * 60 ) )
		{
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 70;
			CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
			CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 23;
			mZone175TypePostTick = 0;
			mZone175TypeBattleState = 0;
			//--------------//
			//DELETE_MONSTER//
			//--------------//
			mSUMMON.DeleteMonster();
			//--------------//
			//--------------//
			//--------------//
			return;
		}
		if( ( mZone175TypePostTick % 20 ) == 0 )
		{
			mSUMMON.SummonMonsterForZone175Type();
		}
		for( index01 = 0 ; index01 < MAX_MONSTER_OBJECT_NUM ; index01++ )
		{
			if( !mMONSTER_OBJECT[index01].mCheckValidState )
			{
				continue;
			}
			if( ( mMONSTER_OBJECT[index01].mMONSTER_INFO->mSpecialType == 40 ) || ( mMONSTER_OBJECT[index01].mMONSTER_INFO->mSpecialType == 41 ) || ( mMONSTER_OBJECT[index01].mMONSTER_INFO->mSpecialType == 42 ) || ( mMONSTER_OBJECT[index01].mMONSTER_INFO->mSpecialType == 43 ) || ( mMONSTER_OBJECT[index01].mMONSTER_INFO->mSpecialType == 44 ) )
			{
				break;
			}
		}
		if( index01 < MAX_MONSTER_OBJECT_NUM )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 71;
		CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
		CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 6;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		//--------------//
		//DELETE_MONSTER//
		//--------------//
		mSUMMON.DeleteMonster();
		//--------------//
		//--------------//
		//--------------//
		//----------------//
		//BATTLE_WIN_BONUS//
		//----------------//
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
		{
			if( !mAVATAR_OBJECT[index01].mCheckValidState )
			{
				continue;
			}
			if( mUSER[index01].mMoveZoneResult == 1 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aLifeValue < 1 )
			{
				continue;
			}
			// 2010.1.18 : �輺�� : ���� ���̸鼭 ó���Ѱ� 
			//�� ����ÿ� ���������ϋ� .. �ش� �ɸ����� ���� ���¸� Ǯ�� �ش�
			if( mAVATAR_OBJECT[index01].mDATA.aAction.aSort == 11 )
			{
				mAVATAR_OBJECT[index01].mDATA.aAction.aType = mAVATAR_OBJECT[index01].GetWeaponClass() * 2;
				mAVATAR_OBJECT[index01].mDATA.aAction.aSort = 1;
				mAVATAR_OBJECT[index01].mDATA.aAction.aFrame = 0.0f;
				mAVATAR_OBJECT[index01].mDATA.aAction.aSkillValue = 0;
				mTRANSFER.B_AVATAR_ACTION_RECV( index01, mAVATAR_OBJECT[index01].mUniqueNumber, &mAVATAR_OBJECT[index01].mDATA, 1 );
				mUTIL.Broadcast( TRUE, &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], 1, false, 1);
			}
			
#ifdef __GOD__
			GetZone175TypeRewardInfo( mAVATAR_OBJECT[index01].mDATA.aLevel1, mAVATAR_OBJECT[index01].mDATA.aLevel2, 1, &tGainExperience, &tGainMoney );
#else
			GetZone175TypeRewardInfo( mAVATAR_OBJECT[index01].mDATA.aLevel1, 1, &tGainExperience, &tGainMoney );
#endif
			if( tGainExperience < 1 )
			{
				continue;
			}
			mTRANSFER.B_AVATAR_CHANGE_INFO_2( 1, tGainExperience );
			mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			mUTIL.ProcessForExperience( index01, tGainExperience, 0, 0 );
			if( !mUTIL.CheckOverMaximum( mUSER[index01].mAvatarInfo.aMoney, tGainMoney ) )
			{
				mUSER[index01].mAvatarInfo.aMoney += tGainMoney;
				mTRANSFER.B_AVATAR_CHANGE_INFO_2( 23, tGainMoney );
				mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			}
			//mUTIL.ProcessForDropItem( 11, 1037, 0, 0, mUTIL.ReturnItemRecognitionNumber( 0 ), &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], 0 );
			//mUTIL.ProcessForDropItem( 11, 1022, 0, 0, mUTIL.ReturnItemRecognitionNumber( 0 ), &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], 0 );
			//mUTIL.ProcessForDropItem( 11, 1024, 0, 0, mUTIL.ReturnItemRecognitionNumber( 0 ), &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], 0 );
		}
		//----------------//
		//----------------//
		//----------------//
		return;
	case  6 : //[������ȯ_OR_2�ܰ�_�Ա�����]
		if( mZone175TypePostTick < ( 120 * 1 ) )
		{
			return;
		}
		if( mZone175TypeZoneIndex2 < 1 )
		{
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 72;
			CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
			CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 23;
			mZone175TypePostTick = 0;
			mZone175TypeBattleState = 0;
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 73;
		CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
		CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 7;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		return;
	case  7 : //[2�ܰ�_�Ա����]
		if( mZone175TypePostTick < ( 120 * 2 ) )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 74;
		CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
		CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 8;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		return;
	case  8 : //[2�ܰ�_����]
		if( mZone175TypePostTick < ( 120 * 1 ) )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 75;
		CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
		CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 9;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		//--------------//
		//SUMMON_MONSTER//
		//--------------//
		mSUMMON.SummonMonsterWithoutTimeLimitForZone175Type( 2 );
		//--------------//
		//--------------//
		//--------------//
		return;
	case  9 : //[2�ܰ�_����]
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
		{
			if( !mAVATAR_OBJECT[index01].mCheckValidState )
			{
				continue;
			}
			if( mUSER[index01].mMoveZoneResult == 1 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0 )
			{
				continue;
			}
			break;
		}
		if( index01 == mSERVER.mMAX_USER_NUM )
		{
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 76;
			CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
			CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 23;
			mZone175TypePostTick = 0;
			mZone175TypeBattleState = 0;
			//--------------//
			//DELETE_MONSTER//
			//--------------//
			mSUMMON.DeleteMonster();
			//--------------//
			//--------------//
			//--------------//
			return;
		}
		if( mZone175TypePostTick == ( 120 * 60 ) )
		{
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 77;
			CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
			CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 23;
			mZone175TypePostTick = 0;
			mZone175TypeBattleState = 0;
			//--------------//
			//DELETE_MONSTER//
			//--------------//
			mSUMMON.DeleteMonster();
			//--------------//
			//--------------//
			//--------------//
			return;
		}
		if( ( mZone175TypePostTick % 20 ) == 0 )
		{
			mSUMMON.SummonMonsterForZone175Type();
		}
		for( index01 = 0 ; index01 < MAX_MONSTER_OBJECT_NUM ; index01++ )
		{
			if( !mMONSTER_OBJECT[index01].mCheckValidState )
			{
				continue;
			}
			if( ( mMONSTER_OBJECT[index01].mMONSTER_INFO->mSpecialType == 40 ) || ( mMONSTER_OBJECT[index01].mMONSTER_INFO->mSpecialType == 41 ) || ( mMONSTER_OBJECT[index01].mMONSTER_INFO->mSpecialType == 42 ) || ( mMONSTER_OBJECT[index01].mMONSTER_INFO->mSpecialType == 43 ) || ( mMONSTER_OBJECT[index01].mMONSTER_INFO->mSpecialType == 44 ) )
			{
				break;
			}
		}
		if( index01 < MAX_MONSTER_OBJECT_NUM )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 78;
		CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
		CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 10;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		//--------------//
		//DELETE_MONSTER//
		//--------------//
		mSUMMON.DeleteMonster();
		//--------------//
		//--------------//
		//--------------//
		//----------------//
		//BATTLE_WIN_BONUS//
		//----------------//
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
		{
			if( !mAVATAR_OBJECT[index01].mCheckValidState )
			{
				continue;
			}
			if( mUSER[index01].mMoveZoneResult == 1 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aLifeValue < 1 )
			{
				continue;
			}
			// 2010.1.18 : �輺�� : ���� ���̸鼭 ó���Ѱ� 
			//�� ����ÿ� ���������ϋ� .. �ش� �ɸ����� ���� ���¸� Ǯ�� �ش�
			if( mAVATAR_OBJECT[index01].mDATA.aAction.aSort == 11 )
			{
				mAVATAR_OBJECT[index01].mDATA.aAction.aType = mAVATAR_OBJECT[index01].GetWeaponClass() * 2;
				mAVATAR_OBJECT[index01].mDATA.aAction.aSort = 1;
				mAVATAR_OBJECT[index01].mDATA.aAction.aFrame = 0.0f;
				mAVATAR_OBJECT[index01].mDATA.aAction.aSkillValue = 0;
				mTRANSFER.B_AVATAR_ACTION_RECV( index01, mAVATAR_OBJECT[index01].mUniqueNumber, &mAVATAR_OBJECT[index01].mDATA, 1 );
				mUTIL.Broadcast( TRUE, &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], 1, false, 1);
			}
			
#ifdef __GOD__
			GetZone175TypeRewardInfo( mAVATAR_OBJECT[index01].mDATA.aLevel1, mAVATAR_OBJECT[index01].mDATA.aLevel2, 2, &tGainExperience, &tGainMoney );
#else	
			GetZone175TypeRewardInfo( mAVATAR_OBJECT[index01].mDATA.aLevel1, 2, &tGainExperience, &tGainMoney );
#endif
			if( tGainExperience < 1 )
			{
				continue;
			}
			mTRANSFER.B_AVATAR_CHANGE_INFO_2( 1, tGainExperience );
			mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			mUTIL.ProcessForExperience( index01, tGainExperience, 0, 0 );
			if( !mUTIL.CheckOverMaximum( mUSER[index01].mAvatarInfo.aMoney, tGainMoney ) )
			{
				mUSER[index01].mAvatarInfo.aMoney += tGainMoney;
				mTRANSFER.B_AVATAR_CHANGE_INFO_2( 23, tGainMoney );
				mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			}
			mUTIL.ProcessForDropItem( 11, 1023, 0, 0, mUTIL.ReturnItemRecognitionNumber( 0 ), &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], 0 );
			mUTIL.ProcessForDropItem( 11, 1025, 0, 0, mUTIL.ReturnItemRecognitionNumber( 0 ), &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], 0 );
		}
		//----------------//
		//----------------//
		//----------------//
		return;
	case 10 : //[������ȯ_OR_3�ܰ�_�Ա�����]
		if( mZone175TypePostTick < ( 120 * 1 ) )
		{
			return;
		}
		if( mZone175TypeZoneIndex2 < 2 )
		{
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 79;
			CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
			CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 23;
			mZone175TypePostTick = 0;
			mZone175TypeBattleState = 0;
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 80;
		CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
		CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 11;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		return;
	case 11 : //[3�ܰ�_�Ա����]
		if( mZone175TypePostTick < ( 120 * 2 ) )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 81;
		CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
		CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 12;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		return;
	case 12 : //[3�ܰ�_����]
		if( mZone175TypePostTick < ( 120 * 1 ) )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 82;
		CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
		CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 13;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		//--------------//
		//SUMMON_MONSTER//
		//--------------//
		mSUMMON.SummonMonsterWithoutTimeLimitForZone175Type( 3 );
		//--------------//
		//--------------//
		//--------------//
		return;
	case 13 : //[3�ܰ�_����]
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
		{
			if( !mAVATAR_OBJECT[index01].mCheckValidState )
			{
				continue;
			}
			if( mUSER[index01].mMoveZoneResult == 1 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0 )
			{
				continue;
			}
			break;
		}
		if( index01 == mSERVER.mMAX_USER_NUM )
		{
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 83;
			CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
			CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 23;
			mZone175TypePostTick = 0;
			mZone175TypeBattleState = 0;
			//--------------//
			//DELETE_MONSTER//
			//--------------//
			mSUMMON.DeleteMonster();
			//--------------//
			//--------------//
			//--------------//
			return;
		}
		if( mZone175TypePostTick == ( 120 * 60 ) )
		{
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 84;
			CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
			CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 23;
			mZone175TypePostTick = 0;
			mZone175TypeBattleState = 0;
			//--------------//
			//DELETE_MONSTER//
			//--------------//
			mSUMMON.DeleteMonster();
			//--------------//
			//--------------//
			//--------------//
			return;
		}
		if( ( mZone175TypePostTick % 20 ) == 0 )
		{
			mSUMMON.SummonMonsterForZone175Type();
		}
		for( index01 = 0 ; index01 < MAX_MONSTER_OBJECT_NUM ; index01++ )
		{
			if( !mMONSTER_OBJECT[index01].mCheckValidState )
			{
				continue;
			}
			if( ( mMONSTER_OBJECT[index01].mMONSTER_INFO->mSpecialType == 40 ) || ( mMONSTER_OBJECT[index01].mMONSTER_INFO->mSpecialType == 41 ) || ( mMONSTER_OBJECT[index01].mMONSTER_INFO->mSpecialType == 42 ) || ( mMONSTER_OBJECT[index01].mMONSTER_INFO->mSpecialType == 43 ) || ( mMONSTER_OBJECT[index01].mMONSTER_INFO->mSpecialType == 44 ) )
			{
				break;
			}
		}
		if( index01 < MAX_MONSTER_OBJECT_NUM )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 85;
		CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
		CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		
		
		mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 14;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		//--------------//
		//DELETE_MONSTER//
		//--------------//
		mSUMMON.DeleteMonster();
		//--------------//
		//--------------//
		//--------------//
		//----------------//
		//BATTLE_WIN_BONUS//
		//----------------//
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
		{
			if( !mAVATAR_OBJECT[index01].mCheckValidState )
			{
				continue;
			}
			if( mUSER[index01].mMoveZoneResult == 1 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aLifeValue < 1 )
			{
				continue;
			}
			
			// 2010.1.18 : �輺�� : ���� ���̸鼭 ó���Ѱ� 
			//�� ����ÿ� ���������ϋ� .. �ش� �ɸ����� ���� ���¸� Ǯ�� �ش�
			if( mAVATAR_OBJECT[index01].mDATA.aAction.aSort == 11 )
			{
				mAVATAR_OBJECT[index01].mDATA.aAction.aType = mAVATAR_OBJECT[index01].GetWeaponClass() * 2;
				mAVATAR_OBJECT[index01].mDATA.aAction.aSort = 1;
				mAVATAR_OBJECT[index01].mDATA.aAction.aFrame = 0.0f;
				mAVATAR_OBJECT[index01].mDATA.aAction.aSkillValue = 0;
				mTRANSFER.B_AVATAR_ACTION_RECV( index01, mAVATAR_OBJECT[index01].mUniqueNumber, &mAVATAR_OBJECT[index01].mDATA, 1 );
				mUTIL.Broadcast( TRUE, &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], 1, false, 1);
			}
#ifdef __GOD__
			GetZone175TypeRewardInfo( mAVATAR_OBJECT[index01].mDATA.aLevel1, mAVATAR_OBJECT[index01].mDATA.aLevel2, 3, &tGainExperience, &tGainMoney );
#else
			GetZone175TypeRewardInfo( mAVATAR_OBJECT[index01].mDATA.aLevel1, 3, &tGainExperience, &tGainMoney );
#endif
			if( tGainExperience < 1 )
			{
				continue;
			}
			mTRANSFER.B_AVATAR_CHANGE_INFO_2( 1, tGainExperience );
			mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			mUTIL.ProcessForExperience( index01, tGainExperience, 0, 0 );
			if( !mUTIL.CheckOverMaximum( mUSER[index01].mAvatarInfo.aMoney, tGainMoney ) )
			{
				mUSER[index01].mAvatarInfo.aMoney += tGainMoney;
				mTRANSFER.B_AVATAR_CHANGE_INFO_2( 23, tGainMoney );
				mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			}
			mUTIL.ProcessForDropItem( 11, 1125, 0, 0, mUTIL.ReturnItemRecognitionNumber( 0 ), &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], 0 );
			mUTIL.ProcessForDropItem( 11, 1125, 0, 0, mUTIL.ReturnItemRecognitionNumber( 0 ), &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], 0 );
			mUTIL.ProcessForDropItem( 11, 1023, 0, 0, mUTIL.ReturnItemRecognitionNumber( 0 ), &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], 0 );
			mUTIL.ProcessForDropItem( 11, 1023, 0, 0, mUTIL.ReturnItemRecognitionNumber( 0 ), &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], 0 );
			mUTIL.ProcessForDropItem( 11, 1025, 0, 0, mUTIL.ReturnItemRecognitionNumber( 0 ), &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], 0 );
			mUTIL.ProcessForDropItem( 11, 1025, 0, 0, mUTIL.ReturnItemRecognitionNumber( 0 ), &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], 0 );
		}
		//----------------//
		//----------------//
		//----------------//
		return;
	case 14 : //[������ȯ_OR_4�ܰ�_�Ա�����]
		if( mZone175TypePostTick < ( 120 * 1 ) )
		{
			return;
		}
		if( mZone175TypeZoneIndex2 < 3 )
		{
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 86;
			CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
			CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 23;
			mZone175TypePostTick = 0;
			mZone175TypeBattleState = 0;
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 87;
		CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
		CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 15;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		return;
	case 15 : //[4�ܰ�_�Ա����]
		if( mZone175TypePostTick < ( 120 * 2 ) )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 88;
		CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
		CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 16;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		return;
	case 16 : //[4�ܰ�_����]
		if( mZone175TypePostTick < ( 120 * 1 ) )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 89;
		CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
		CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 17;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		//--------------//
		//SUMMON_MONSTER//
		//--------------//
		mSUMMON.SummonMonsterWithoutTimeLimitForZone175Type( 4 );
		//--------------//
		//--------------//
		//--------------//
		return;
	case 17 : //[4�ܰ�_����]
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
		{
			if( !mAVATAR_OBJECT[index01].mCheckValidState )
			{
				continue;
			}
			if( mUSER[index01].mMoveZoneResult == 1 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0 )
			{
				continue;
			}
			break;
		}
		if( index01 == mSERVER.mMAX_USER_NUM )
		{
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 90;
			CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
			CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 23;
			mZone175TypePostTick = 0;
			mZone175TypeBattleState = 0;
			//--------------//
			//DELETE_MONSTER//
			//--------------//
			mSUMMON.DeleteMonster();
			//--------------//
			//--------------//
			//--------------//
			return;
		}
		if( mZone175TypePostTick == ( 120 * 60 ) )
		{
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 91;
			CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
			CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 23;
			mZone175TypePostTick = 0;
			mZone175TypeBattleState = 0;
			//--------------//
			//DELETE_MONSTER//
			//--------------//
			mSUMMON.DeleteMonster();
			//--------------//
			//--------------//
			//--------------//
			return;
		}
		if( ( mZone175TypePostTick % 20 ) == 0 )
		{
			mSUMMON.SummonMonsterForZone175Type();
		}
		for( index01 = 0 ; index01 < MAX_MONSTER_OBJECT_NUM ; index01++ )
		{
			if( !mMONSTER_OBJECT[index01].mCheckValidState )
			{
				continue;
			}
			if( ( mMONSTER_OBJECT[index01].mMONSTER_INFO->mSpecialType == 40 ) || ( mMONSTER_OBJECT[index01].mMONSTER_INFO->mSpecialType == 41 ) || ( mMONSTER_OBJECT[index01].mMONSTER_INFO->mSpecialType == 42 ) || ( mMONSTER_OBJECT[index01].mMONSTER_INFO->mSpecialType == 43 ) || ( mMONSTER_OBJECT[index01].mMONSTER_INFO->mSpecialType == 44 ) )
			{
				break;
			}
		}
		if( index01 < MAX_MONSTER_OBJECT_NUM )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 92;
		CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
		CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 18;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		//--------------//
		//DELETE_MONSTER//
		//--------------//
		mSUMMON.DeleteMonster();
		//--------------//
		//--------------//
		//--------------//
		//----------------//
		//BATTLE_WIN_BONUS//
		//----------------//
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
		{
			if( !mAVATAR_OBJECT[index01].mCheckValidState )
			{
				continue;
			}
			if( mUSER[index01].mMoveZoneResult == 1 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aLifeValue < 1 )
			{
				continue;
			}
			// 2010.1.18 : �輺�� : ���� ���̸鼭 ó���Ѱ� 
			//�� ����ÿ� ���������ϋ� .. �ش� �ɸ����� ���� ���¸� Ǯ�� �ش�
			if( mAVATAR_OBJECT[index01].mDATA.aAction.aSort == 11 )
			{
				mAVATAR_OBJECT[index01].mDATA.aAction.aType = mAVATAR_OBJECT[index01].GetWeaponClass() * 2;
				mAVATAR_OBJECT[index01].mDATA.aAction.aSort = 1;
				mAVATAR_OBJECT[index01].mDATA.aAction.aFrame = 0.0f;
				mAVATAR_OBJECT[index01].mDATA.aAction.aSkillValue = 0;
				mTRANSFER.B_AVATAR_ACTION_RECV( index01, mAVATAR_OBJECT[index01].mUniqueNumber, &mAVATAR_OBJECT[index01].mDATA, 1 );
				mUTIL.Broadcast( TRUE, &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], 1, false, 1);
			}
#ifdef __GOD__
			GetZone175TypeRewardInfo( mAVATAR_OBJECT[index01].mDATA.aLevel1, mAVATAR_OBJECT[index01].mDATA.aLevel2, 4, &tGainExperience, &tGainMoney );
#else
			GetZone175TypeRewardInfo( mAVATAR_OBJECT[index01].mDATA.aLevel1, 4, &tGainExperience, &tGainMoney );
#endif	
			
			if( tGainExperience < 1 )
			{
				continue;
			}
			mTRANSFER.B_AVATAR_CHANGE_INFO_2( 1, tGainExperience );
			mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			mUTIL.ProcessForExperience( index01, tGainExperience, 0, 0 );
			if( !mUTIL.CheckOverMaximum( mUSER[index01].mAvatarInfo.aMoney, tGainMoney ) )
			{
				mUSER[index01].mAvatarInfo.aMoney += tGainMoney;
				mTRANSFER.B_AVATAR_CHANGE_INFO_2( 23, tGainMoney );
				mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			}
			mUTIL.ProcessForDropItem( 11, 1125, 0, 0, mUTIL.ReturnItemRecognitionNumber( 0 ), &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], 0 );
			mUTIL.ProcessForDropItem( 11, 1125, 0, 0, mUTIL.ReturnItemRecognitionNumber( 0 ), &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], 0 );
			mUTIL.ProcessForDropItem( 11, 1023, 0, 0, mUTIL.ReturnItemRecognitionNumber( 0 ), &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], 0 );
			mUTIL.ProcessForDropItem( 11, 1023, 0, 0, mUTIL.ReturnItemRecognitionNumber( 0 ), &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], 0 );
			mUTIL.ProcessForDropItem( 11, 1025, 0, 0, mUTIL.ReturnItemRecognitionNumber( 0 ), &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], 0 );
			mUTIL.ProcessForDropItem( 11, 1025, 0, 0, mUTIL.ReturnItemRecognitionNumber( 0 ), &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], 0 );
			mUTIL.ProcessForDropItem( 11, 1940, 0, 0, mUTIL.ReturnItemRecognitionNumber( 0 ), &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], 0 );
		}
		//----------------//
		//----------------//
		//----------------//
		return;
	case 18 : //[������ȯ_OR_5�ܰ�_�Ա�����]
		if( mZone175TypePostTick < ( 120 * 1 ) )
		{
			return;
		}
		if( mZone175TypeZoneIndex2 < 4 )
		{
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 93;
			CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
			CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 23;
			mZone175TypePostTick = 0;
			mZone175TypeBattleState = 0;
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 94;
		CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
		CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 19;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		return;
	case 19 : //[5�ܰ�_�Ա����]
		if( mZone175TypePostTick < ( 120 * 2 ) )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 95;
		CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
		CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 20;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		return;
	case 20 : //[5�ܰ�_����]
		if( mZone175TypePostTick < ( 120 * 1 ) )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 96;
		CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
		CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 21;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		//--------------//
		//SUMMON_MONSTER//
		//--------------//
		mSUMMON.SummonMonsterWithoutTimeLimitForZone175Type( 5 );
		//--------------//
		//--------------//
		//--------------//
		return;
	case 21 : //[5�ܰ�_����]
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
		{
			if( !mAVATAR_OBJECT[index01].mCheckValidState )
			{
				continue;
			}
			if( mUSER[index01].mMoveZoneResult == 1 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0 )
			{
				continue;
			}
			break;
		}
		if( index01 == mSERVER.mMAX_USER_NUM )
		{
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 97;
			CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
			CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 23;
			mZone175TypePostTick = 0;
			mZone175TypeBattleState = 0;
			//--------------//
			//DELETE_MONSTER//
			//--------------//
			mSUMMON.DeleteMonster();
			//--------------//
			//--------------//
			//--------------//
			return;
		}
		if( mZone175TypePostTick == ( 120 * 60 ) )
		{
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 98;
			CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
			CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 23;
			mZone175TypePostTick = 0;
			mZone175TypeBattleState = 0;
			//--------------//
			//DELETE_MONSTER//
			//--------------//
			mSUMMON.DeleteMonster();
			//--------------//
			//--------------//
			//--------------//
			return;
		}
		if( ( mZone175TypePostTick % 20 ) == 0 )
		{
			mSUMMON.SummonMonsterForZone175Type();
		}
		for( index01 = 0 ; index01 < MAX_MONSTER_OBJECT_NUM ; index01++ )
		{
			if( !mMONSTER_OBJECT[index01].mCheckValidState )
			{
				continue;
			}
			if( ( mMONSTER_OBJECT[index01].mMONSTER_INFO->mSpecialType == 40 ) || ( mMONSTER_OBJECT[index01].mMONSTER_INFO->mSpecialType == 41 ) || ( mMONSTER_OBJECT[index01].mMONSTER_INFO->mSpecialType == 42 ) || ( mMONSTER_OBJECT[index01].mMONSTER_INFO->mSpecialType == 43 ) || ( mMONSTER_OBJECT[index01].mMONSTER_INFO->mSpecialType == 44 ) )
			{
				break;
			}
		}
		if( index01 < MAX_MONSTER_OBJECT_NUM )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 99;
		CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
		CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 22;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		//--------------//
		//DELETE_MONSTER//
		//--------------//
		mSUMMON.DeleteMonster();
		//--------------//
		//--------------//
		//--------------//
		//----------------//
		//BATTLE_WIN_BONUS//
		//----------------//
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
		{
			if( !mAVATAR_OBJECT[index01].mCheckValidState )
			{
				continue;
			}
			if( mUSER[index01].mMoveZoneResult == 1 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aLifeValue < 1 )
			{
				continue;
			}
			// 2010.1.18 : �輺�� : ���� ���̸鼭 ó���Ѱ� 
			//�� ����ÿ� ���������ϋ� .. �ش� �ɸ����� ���� ���¸� Ǯ�� �ش�
			if( mAVATAR_OBJECT[index01].mDATA.aAction.aSort == 11 )
			{
				mAVATAR_OBJECT[index01].mDATA.aAction.aType = mAVATAR_OBJECT[index01].GetWeaponClass() * 2;
				mAVATAR_OBJECT[index01].mDATA.aAction.aSort = 1;
				mAVATAR_OBJECT[index01].mDATA.aAction.aFrame = 0.0f;
				mAVATAR_OBJECT[index01].mDATA.aAction.aSkillValue = 0;
				mTRANSFER.B_AVATAR_ACTION_RECV( index01, mAVATAR_OBJECT[index01].mUniqueNumber, &mAVATAR_OBJECT[index01].mDATA, 1 );
				mUTIL.Broadcast( TRUE, &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], 1, false, 1);
			}
#ifdef __GOD__
			GetZone175TypeRewardInfo( mAVATAR_OBJECT[index01].mDATA.aLevel1, mAVATAR_OBJECT[index01].mDATA.aLevel2, 5, &tGainExperience, &tGainMoney );
#else
			GetZone175TypeRewardInfo( mAVATAR_OBJECT[index01].mDATA.aLevel1, 5, &tGainExperience, &tGainMoney );
#endif
			if( tGainExperience < 1 )
			{
				continue;
			}
			mTRANSFER.B_AVATAR_CHANGE_INFO_2( 1, tGainExperience );
			mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			mUTIL.ProcessForExperience( index01, tGainExperience, 0, 0 );
			if( !mUTIL.CheckOverMaximum( mUSER[index01].mAvatarInfo.aMoney, tGainMoney ) )
			{
				mUSER[index01].mAvatarInfo.aMoney += tGainMoney;
				mTRANSFER.B_AVATAR_CHANGE_INFO_2( 23, tGainMoney );
				mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			}
			mUTIL.ProcessForDropItem( 11, 1415, 0, 0, mUTIL.ReturnItemRecognitionNumber( 0 ), &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], 0 );
			mUTIL.ProcessForDropItem( 11, 1415, 0, 0, mUTIL.ReturnItemRecognitionNumber( 0 ), &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], 0 );
			mUTIL.ProcessForDropItem( 11, 1415, 0, 0, mUTIL.ReturnItemRecognitionNumber( 0 ), &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], 0 );
			mUTIL.ProcessForDropItem( 11, 1023, 0, 0, mUTIL.ReturnItemRecognitionNumber( 0 ), &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], 0 );
			mUTIL.ProcessForDropItem( 11, 1023, 0, 0, mUTIL.ReturnItemRecognitionNumber( 0 ), &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], 0 );
			mUTIL.ProcessForDropItem( 11, 1023, 0, 0, mUTIL.ReturnItemRecognitionNumber( 0 ), &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], 0 );
			mUTIL.ProcessForDropItem( 11, 1025, 0, 0, mUTIL.ReturnItemRecognitionNumber( 0 ), &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], 0 );
			mUTIL.ProcessForDropItem( 11, 1025, 0, 0, mUTIL.ReturnItemRecognitionNumber( 0 ), &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], 0 );
			mUTIL.ProcessForDropItem( 11, 1025, 0, 0, mUTIL.ReturnItemRecognitionNumber( 0 ), &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], 0 );
			mUTIL.ProcessForDropItem( 11, 1938, 0, 0, mUTIL.ReturnItemRecognitionNumber( 0 ), &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], 0 );
			mUTIL.ProcessForDropItem( 11, 1938, 0, 0, mUTIL.ReturnItemRecognitionNumber( 0 ), &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], 0 );
			mUTIL.ProcessForDropItem( 11, 1103, 0, 0, mUTIL.ReturnItemRecognitionNumber( 0 ), &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], 0 );
			mUTIL.ProcessForDropItem( 11, 1048, 0, 0, mUTIL.ReturnItemRecognitionNumber( 0 ), &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mAVATAR_OBJECT[index01].mDATA.aName[0], 0 );
		}
		//----------------//
		//----------------//
		//----------------//
		return;
	case 22 : //[������ȯ]
		if( mZone175TypePostTick < ( 120 * 1 ) )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 100;
		CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
		CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 23;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		return;
	case 23 : //[�ʱ�ȭ]
		if( mZone175TypePostTick < ( 120 * 60 ) )
		{
			return;
		}
		mGAMELOG.GL_662_END_MISSION_LABYRINTH ();
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
		{
			mUSER[index01].Quit(__FILE__, __FUNCTION__, __LINE__);
			               
			               
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 110;
		CopyMemory( &tBroadcastInfoData[0], &mZone175TypeZoneIndex1, 4 );
		CopyMemory( &tBroadcastInfoData[4], &mZone175TypeZoneIndex2, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone175TypeState[mZone175TypeZoneIndex1][mZone175TypeZoneIndex2] = 0;
		mZone175TypePostTick = 0;
		mZone175TypeBattleState = 0;
		return;
	}
}
//GET_ZONE_175_TYPE_REWARD_INFO
#ifdef __GOD__
void MyGame::GetZone175TypeRewardInfo( int tLevel, int tLevel2, int tStep, int *tGainExperience, int *tGainMoney )
#else
void MyGame::GetZone175TypeRewardInfo( int tLevel, int tStep, int *tGainExperience, int *tGainMoney )
#endif
{
	*tGainExperience = 0;
	*tGainMoney = 0;
#ifdef __GOD__
	if( tLevel2 > 0 )
	{
		switch( tLevel2 )
		{
		case  1 :
			*tGainExperience = 4810529;
			break;
		case  2 :
			*tGainExperience = 5002950;
			break;
		case  3 :
			*tGainExperience = 5203068;
			break;
		case  4 :
			*tGainExperience = 5411191;
			break;
		case  5 :
			*tGainExperience = 2813819;
			break;
		case  6 :
			*tGainExperience = 2926372;
			break;
		case  7 :
			*tGainExperience = 3043427;
			break;
		case  8 :
			*tGainExperience = 3165164;
			break;
		case  9 :
			*tGainExperience = 2106733;
			break;
		case 10 :
			*tGainExperience = 2191002;
			break;
		case 11 :
			*tGainExperience = 2278642;
			break;
		case 12 :
			*tGainExperience = 2369788;
			break;
		}
		switch( tStep )
		{
		case 1 :
			*tGainMoney = 13000000;
			break;
		case 2 :
			*tGainMoney = 13500000;
			break;
		case 3 :
			*tGainMoney = 14000000;
			break;
		case 4 :
			*tGainMoney = 14500000;
			break;
		case 5 :
			*tGainMoney = 15000000;
			break;
		}
		return;
	}
#endif	
	switch( tLevel )
	{
	case 100 :
		*tGainExperience = 49959;
		*tGainMoney = 5000000;
		break;
	case 101 :
		*tGainExperience = 55443;
		*tGainMoney = 5000000;
		break;
	case 102 :
		*tGainExperience = 59333;
		*tGainMoney = 5000000;
		break;
	case 103 :
		*tGainExperience = 62405;
		*tGainMoney = 5000000;
		break;
	case 104 :
		*tGainExperience = 64968;
		*tGainMoney = 5000000;
		break;
	case 105 :
		*tGainExperience = 67180;
		*tGainMoney = 5000000;
		break;
	case 106 :
		*tGainExperience = 69133;
		*tGainMoney = 5000000;
		break;
	case 107 :
		*tGainExperience = 70887;
		*tGainMoney = 5000000;
		break;
	case 108 :
		*tGainExperience = 72483;
		*tGainMoney = 5000000;
		break;
	case 109 :
		*tGainExperience = 73949;
		*tGainMoney = 5000000;
		break;
	case 110 :
		*tGainExperience = 75308;
		*tGainMoney = 5000000;
		break;
	case 111 :
		*tGainExperience = 76575;
		*tGainMoney = 5000000;
		break;
	case 112 :
		*tGainExperience = 1594846;
		*tGainMoney = 5000000;
		break;
	case 113 :
		*tGainExperience = 206915;
		switch( tStep )
		{
		case 1 :
			*tGainMoney = 6000000;
			break;
		case 2 :
			*tGainMoney = 6500000;
			break;
		}
		break;
	case 114 :
		*tGainExperience = 229625;
		switch( tStep )
		{
		case 1 :
			*tGainMoney = 6000000;
			break;
		case 2 :
			*tGainMoney = 6500000;
			break;
		}
		break;
	case 115 :
		*tGainExperience = 245740;
		switch( tStep )
		{
		case 1 :
			*tGainMoney = 6000000;
			break;
		case 2 :
			*tGainMoney = 6500000;
			break;
		}
		break;
	case 116 :
		*tGainExperience = 258464;
		switch( tStep )
		{
		case 1 :
			*tGainMoney = 6000000;
			break;
		case 2 :
			*tGainMoney = 6500000;
			break;
		}
		break;
	case 117 :
		*tGainExperience = 269077;
		switch( tStep )
		{
		case 1 :
			*tGainMoney = 6000000;
			break;
		case 2 :
			*tGainMoney = 6500000;
			break;
		}
		break;
	case 118 :
		*tGainExperience = 278236;
		switch( tStep )
		{
		case 1 :
			*tGainMoney = 6000000;
			break;
		case 2 :
			*tGainMoney = 6500000;
			break;
		}
		break;
	case 119 :
		*tGainExperience = 286326;
		switch( tStep )
		{
		case 1 :
			*tGainMoney = 6000000;
			break;
		case 2 :
			*tGainMoney = 6500000;
			break;
		}
		break;
	case 120 :
		*tGainExperience = 293592;
		switch( tStep )
		{
		case 1 :
			*tGainMoney = 6000000;
			break;
		case 2 :
			*tGainMoney = 6500000;
			break;
		}
		break;
	case 121 :
		*tGainExperience = 300201;
		switch( tStep )
		{
		case 1 :
			*tGainMoney = 6000000;
			break;
		case 2 :
			*tGainMoney = 6500000;
			break;
		}
		break;
	case 122 :
		*tGainExperience = 306275;
		switch( tStep )
		{
		case 1 :
			*tGainMoney = 6000000;
			break;
		case 2 :
			*tGainMoney = 6500000;
			break;
		}
		break;
	case 123 :
		*tGainExperience = 311902;
		switch( tStep )
		{
		case 1 :
			*tGainMoney = 7000000;
			break;
		case 2 :
			*tGainMoney = 7500000;
			break;
		case 3 :
			*tGainMoney = 8000000;
			break;
		}
		break;
	case 124 :
		*tGainExperience = 317149;
		switch( tStep )
		{
		case 1 :
			*tGainMoney = 7000000;
			break;
		case 2 :
			*tGainMoney = 7500000;
			break;
		case 3 :
			*tGainMoney = 8000000;
			break;
		}
		break;
	case 125 :
		*tGainExperience = 322070;
		switch( tStep )
		{
		case 1 :
			*tGainMoney = 7000000;
			break;
		case 2 :
			*tGainMoney = 7500000;
			break;
		case 3 :
			*tGainMoney = 8000000;
			break;
		}
		break;
	case 126 :
		*tGainExperience = 326708;
		switch( tStep )
		{
		case 1 :
			*tGainMoney = 7000000;
			break;
		case 2 :
			*tGainMoney = 7500000;
			break;
		case 3 :
			*tGainMoney = 8000000;
			break;
		}
		break;
	case 127 :
		*tGainExperience = 331096;
		switch( tStep )
		{
		case 1 :
			*tGainMoney = 7000000;
			break;
		case 2 :
			*tGainMoney = 7500000;
			break;
		case 3 :
			*tGainMoney = 8000000;
			break;
		}
		break;
	case 128 :
		*tGainExperience = 335263;
		switch( tStep )
		{
		case 1 :
			*tGainMoney = 7000000;
			break;
		case 2 :
			*tGainMoney = 7500000;
			break;
		case 3 :
			*tGainMoney = 8000000;
			break;
		}
		break;
	case 129 :
		*tGainExperience = 339233;
		switch( tStep )
		{
		case 1 :
			*tGainMoney = 7000000;
			break;
		case 2 :
			*tGainMoney = 7500000;
			break;
		case 3 :
			*tGainMoney = 8000000;
			break;
		}
		break;
	case 130 :
		*tGainExperience = 343025;
		switch( tStep )
		{
		case 1 :
			*tGainMoney = 7000000;
			break;
		case 2 :
			*tGainMoney = 7500000;
			break;
		case 3 :
			*tGainMoney = 8000000;
			break;
		}
		break;
	case 131 :
		*tGainExperience = 346657;
		switch( tStep )
		{
		case 1 :
			*tGainMoney = 7000000;
			break;
		case 2 :
			*tGainMoney = 7500000;
			break;
		case 3 :
			*tGainMoney = 8000000;
			break;
		}
		break;
	case 132 :
		*tGainExperience = 350142;
		switch( tStep )
		{
		case 1 :
			*tGainMoney =  7000000;
			break;
		case 2 :
			*tGainMoney =  7500000;
			break;
		case 3 :
			*tGainMoney =  8000000;
			break;
		}
		break;
	case 133 :
		*tGainExperience = 353494;
		switch( tStep )
		{
		case 1 :
			*tGainMoney =  8500000;
			break;
		case 2 :
			*tGainMoney =  9000000;
			break;
		case 3 :
			*tGainMoney =  9500000;
			break;
		case 4 :
			*tGainMoney = 10000000;
			break;
		}
		break;
	case 134 :
		*tGainExperience = 356723;
		switch( tStep )
		{
		case 1 :
			*tGainMoney =  8500000;
			break;
		case 2 :
			*tGainMoney =  9000000;
			break;
		case 3 :
			*tGainMoney =  9500000;
			break;
		case 4 :
			*tGainMoney = 10000000;
			break;
		}
		break;
	case 135 :
		*tGainExperience = 359839;
		switch( tStep )
		{
		case 1 :
			*tGainMoney =  8500000;
			break;
		case 2 :
			*tGainMoney =  9000000;
			break;
		case 3 :
			*tGainMoney =  9500000;
			break;
		case 4 :
			*tGainMoney = 10000000;
			break;
		}
		break;
	case 136 :
		*tGainExperience = 362851;
		switch( tStep )
		{
		case 1 :
			*tGainMoney =  8500000;
			break;
		case 2 :
			*tGainMoney =  9000000;
			break;
		case 3 :
			*tGainMoney =  9500000;
			break;
		case 4 :
			*tGainMoney = 10000000;
			break;
		}
		break;
	case 137 :
		*tGainExperience = 365766;
		switch( tStep )
		{
		case 1 :
			*tGainMoney =  8500000;
			break;
		case 2 :
			*tGainMoney =  9000000;
			break;
		case 3 :
			*tGainMoney =  9500000;
			break;
		case 4 :
			*tGainMoney = 10000000;
			break;
		}
		break;
	case 138 :
		*tGainExperience = 368592;
		switch( tStep )
		{
		case 1 :
			*tGainMoney =  8500000;
			break;
		case 2 :
			*tGainMoney =  9000000;
			break;
		case 3 :
			*tGainMoney =  9500000;
			break;
		case 4 :
			*tGainMoney = 10000000;
			break;
		}
		break;
	case 139 :
		*tGainExperience = 371332;
		switch( tStep )
		{
		case 1 :
			*tGainMoney =  8500000;
			break;
		case 2 :
			*tGainMoney =  9000000;
			break;
		case 3 :
			*tGainMoney =  9500000;
			break;
		case 4 :
			*tGainMoney = 10000000;
			break;
		}
		break;
	case 140 :
		*tGainExperience = 373996;
		switch( tStep )
		{
		case 1 :
			*tGainMoney =  8500000;
			break;
		case 2 :
			*tGainMoney =  9000000;
			break;
		case 3 :
			*tGainMoney =  9500000;
			break;
		case 4 :
			*tGainMoney = 10000000;
			break;
		}
		break;
	case 141 :
		*tGainExperience = 376583;
		switch( tStep )
		{
		case 1 :
			*tGainMoney =  8500000;
			break;
		case 2 :
			*tGainMoney =  9000000;
			break;
		case 3 :
			*tGainMoney =  9500000;
			break;
		case 4 :
			*tGainMoney = 10000000;
			break;
		}
		break;
	case 142 :
		*tGainExperience = 379103;
		switch( tStep )
		{
		case 1 :
			*tGainMoney =  8500000;
			break;
		case 2 :
			*tGainMoney =  9000000;
			break;
		case 3 :
			*tGainMoney =  9500000;
			break;
		case 4 :
			*tGainMoney = 10000000;
			break;
		}
		break;
	case 143 :
		*tGainExperience = 381556;
		switch( tStep )
		{
		case 1 :
			*tGainMoney = 10500000;
			break;
		case 2 :
			*tGainMoney = 11000000;
			break;
		case 3 :
			*tGainMoney = 11500000;
			break;
		case 4 :
			*tGainMoney = 12000000;
			break;
		case 5 :
			*tGainMoney = 12500000;
			break;
		}
		break;
	case 144 :
		*tGainExperience = 383950;
		switch( tStep )
		{
		case 1 :
			*tGainMoney = 10500000;
			break;
		case 2 :
			*tGainMoney = 11000000;
			break;
		case 3 :
			*tGainMoney = 11500000;
			break;
		case 4 :
			*tGainMoney = 12000000;
			break;
		case 5 :
			*tGainMoney = 12500000;
			break;
		}
		break;
	case 145 :
		*tGainExperience = 4625509;
		switch( tStep )
		{
		case 1 :
			*tGainMoney = 10500000;
			break;
		case 2 :
			*tGainMoney = 11000000;
			break;
		case 3 :
			*tGainMoney = 11500000;
			break;
		case 4 :
			*tGainMoney = 12000000;
			break;
		case 5 :
			*tGainMoney = 12500000;
			break;
		}
		break;
	}
}

// 2010.01.07 ������ �⵵�� �ٲ�� �ڵ����� ���� �ʵ尡 �ʱ�ȭ�Ǵ� ������ �־�
// ���� �Լ�(MyGame::Add_Use_Auto_Day, MyGame::Check_Use_Auto_Day)�� �����Ͽ����ϴ�.
//
// �������� aUse_AutoPotion_Time�� ���� �ϼ��� ����Ǿ� �־��µ� ���� ������ ������ �Ŀ���
// aUse_AutoPotion_Time�� ������� �ʽ��ϴ�(��, Client������ aUse_AutoPotion_Time ������ ��� -
//                                          �������� ������ ��¥�� �ӽ÷� �����ϴ� ����).
// aUse_AutoPotion_Date���� time() ��(1970.01.01 ���� �ڵ����� ��� �����ϱ����� ��(sec.))�� �����մϴ�.
// time() - 2038�� ������ �����մϴ�.
BOOL MyGame::Add_Use_Auto_Day(int iIndex, int iDay)
{
    const int SEC_PER_DAY = 86400; // 1���� �ʷ� ��� - 60 * 60 * 24 == 86400��
    int addSec = iDay * SEC_PER_DAY;
    if (mUSER[iIndex].mAvatarInfo.aUse_AutoPotion_Date > 0)
    {
        mUSER[iIndex].mAvatarInfo.aUse_AutoPotion_Date += addSec;
    }
    else
    {
        time_t tNow = ::time(NULL);
        if (tNow == (time_t)(-1))
        {
            LOG_TO_FILE_1("!Add_Use_Auto_Day - uID(%s), time error(-1)\n", mUSER[iIndex].uID);
            return FALSE;
        }
        int nowTimeSec = (int)tNow;
        mUSER[iIndex].mAvatarInfo.aUse_AutoPotion_Date = nowTimeSec + addSec;
    }

    // 2010.01.07 - ���������� aUse_AutoPotion_Time ����(DB �ʵ� ����)�� ������� �ʽ��ϴ�.
    // Client������ aUse_AutoPotion_Time ������ ��������� �������� ������ ��¥��
    // �ӽ÷� �����ϴ� ������ ����մϴ�.
    mUSER[iIndex].mAvatarInfo.aUse_AutoPotion_Time = 0;

    return TRUE;

//	time_t tt; //... ... .. tt. ..(time_t. 4... ....)
//	time(&tt); //tt... ..... ..
//
//	struct tm *tm;
//	tm = localtime(&tt);
//
//	char temp_Date[ 100 ];
//	ZeroMemory( temp_Date, sizeof( char ) * 100 );
//	/// .... ..
//	if ( 99999 < mUSER[iIndex].mAvatarInfo.aUse_AutoPotion_Time  + iDay )
//	{
//		return FALSE;
//	}
//	else
//	{
//		/// .. ... ...., .. ...
//		if( 0 == mUSER[iIndex].mAvatarInfo.aUse_AutoPotion_Time  )
//		{
//			/// ... .. ....	( ..., ., .., .., . )
//			snprintf(temp_Date, 100, "%d%.2d%.2d%.2d%.2d",1, tm->tm_mon + 1 ,tm->tm_mday, tm->tm_hour, tm->tm_min );
//			mUSER[iIndex].mAvatarInfo.aUse_AutoPotion_Date = atoi( temp_Date );
//		}
//		else;
//
//		mUSER[iIndex].mAvatarInfo.aUse_AutoPotion_Time += iDay;
//	}
//	return TRUE;
}

int MyGame::Check_Use_Auto_Day(int mIndex)
{
    const int SEC_PER_DAY = 86400; // 1���� �ʷ� ��� - 60 * 60 * 24 == 86400��

    if (mUSER[mIndex].mAvatarInfo.aUse_AutoPotion_Date > 0)
    {
        time_t tNow = ::time(NULL);
        if (tNow == (time_t)(-1))
        {
            LOG_TO_FILE_1("!Check_Use_Auto_Day - uID(%s), time error(-1)\n", mUSER[mIndex].uID);
            return -1;
        }

        int nowTimeSec = (int)tNow;
        if (mUSER[mIndex].mAvatarInfo.aUse_AutoPotion_Date < nowTimeSec)
        {
            mUSER[mIndex].mAvatarInfo.aUse_AutoPotion_Date = 0;
            mUSER[mIndex].mAvatarInfo.aUse_AutoPotion_Time = 0;
            return 0;
        }

        return ( ((int)((mUSER[mIndex].mAvatarInfo.aUse_AutoPotion_Date - nowTimeSec) / SEC_PER_DAY)) + 1 ); // �ܿ� �ϼ�.
    }
    else
    {
        mUSER[mIndex].mAvatarInfo.aUse_AutoPotion_Date = 0;
        mUSER[mIndex].mAvatarInfo.aUse_AutoPotion_Time = 0;
        return 0;
    }

    return 0;

//	if ( 0 >= mUSER[mIndex].mAvatarInfo.aUse_AutoPotion_Time ) return 0;
//	else;
//	if ( 0 >= mUSER[mIndex].mAvatarInfo.aUse_AutoPotion_Date ) return 0;
//	else;
//
//	char m_tDays[ 12 ] = {	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
//
//	time_t tt; //... ... .. tt. ..(time_t. 4... ....)
//	time(&tt); //tt... ..... ..
//
//	struct tm *tm;
//	tm = localtime(&tt);
//
//	char temp_Date[ 100 ];
//	ZeroMemory( temp_Date, sizeof( char ) * 100 );
//
//	snprintf(temp_Date, 100, "%d", mUSER[mIndex].mAvatarInfo.aUse_AutoPotion_Date);
//	//itoa( mUSER[mIndex].mAvatarInfo.aUse_AutoPotion_Date, temp_Date, 10 );
//
//	char temp_mon[ 10 ];
//	ZeroMemory( temp_mon, sizeof( char ) * 10 );
//	char temp_day[ 10 ];
//	ZeroMemory( temp_day, sizeof( char ) * 10 );
//	char temp_hour[ 10 ];
//	ZeroMemory( temp_hour, sizeof( char ) * 10 );
//	char temp_min[ 10 ];
//	ZeroMemory( temp_min, sizeof( char ) * 10 );
//
//	memcpy( &temp_mon[ 0 ], &temp_Date[ 1 ], 2  );
//	memcpy( &temp_day[ 0 ], &temp_Date[ 3 ], 2  );
//	memcpy( &temp_hour[ 0 ], &temp_Date[ 5 ], 2  );
//	memcpy( &temp_min[ 0 ], &temp_Date[ 7 ], 2  );
//
//	int temp_Date_list[ 4 ];
//	temp_Date_list[ 0 ] = atoi( temp_mon );
//	temp_Date_list[ 1 ] = atoi( temp_day );
//	temp_Date_list[ 2 ] = atoi( temp_hour );
//	temp_Date_list[ 3 ] = atoi( temp_min );
//
//	int iTemp_sub_date = 0;
//	int iTemp_now_mon	= ( tm->tm_mon + 1 );
//	int iTemp_now_day		= tm->tm_mday;
//	int iTemp_now_hour	= tm->tm_hour;
//	int iTemp_now_min		= tm->tm_min;
//
//	/// 	102242253
//	///     03152250
//	int iTemp_gap_mon	= iTemp_now_mon	- temp_Date_list[ 0 ];
//	int iTemp_gap_day		= iTemp_now_day	- temp_Date_list[ 1 ];
//	int iTemp_gap_hour	= iTemp_now_hour	- temp_Date_list[ 2 ];
//	int iTemp_gap_min		= iTemp_now_min	- temp_Date_list[ 3 ];
//
//	//////////////////////////////////////////////////////////////////////////
//	if( 0 !=  iTemp_gap_mon )
//	{
//		/// .... . .. .. .... ....
//		iTemp_sub_date += m_tDays[ temp_Date_list[ 0 ] - 1 ] - temp_Date_list[ 1 ];
//
//		for( int i = 0 ; i < iTemp_gap_mon - 1 ; i++ )
//		{
//			/// ... .... ... ....
//			iTemp_sub_date += m_tDays[ temp_Date_list[ 0 ] + i ];
//		}
//
//		/// .. .. .... ....
//		iTemp_sub_date += iTemp_now_day;
//	}
//	else
//	{
//		iTemp_sub_date = iTemp_now_day - temp_Date_list[ 1 ];
//	}
//	//////////////////////////////////////////////////////////////////////////
//	if( 0 > iTemp_gap_hour )
//	{
//		iTemp_sub_date -= 1;
//		if( 0 >= iTemp_sub_date )	iTemp_sub_date = 0;
//	}
//	else if( 0 == iTemp_gap_hour )
//	{
//		if( 0 > iTemp_gap_min )
//		{
//			iTemp_sub_date -= 1;
//			if( 0 >= iTemp_sub_date )	iTemp_sub_date = 0;
//		}
//		else;
//	}
//	else;
//
//	mUSER[mIndex].mAvatarInfo.aUse_AutoPotion_Time -= iTemp_sub_date;
//
//	if ( 0 >= mUSER[mIndex].mAvatarInfo.aUse_AutoPotion_Time )
//	{
//		mUSER[mIndex].mAvatarInfo.aUse_AutoPotion_Time = 0;
//		mUSER[mIndex].mAvatarInfo.aUse_AutoPotion_Date = 0;
//	}
//	else
//	{
//		//////////////////////////////////////////////////////////////////////////
//		while( m_tDays[ temp_Date_list[ 0 ] - 1 ]  < temp_Date_list[ 1 ] +iTemp_sub_date )
//		{
//			int temp_value = m_tDays[ temp_Date_list[ 0 ] - 1 ] - temp_Date_list[ 1 ];
//			iTemp_sub_date -= temp_value;
//			temp_Date_list[ 1 ] = 0;
//			temp_Date_list[ 0 ] += 1;
//		}
//		temp_Date_list[ 1 ] += iTemp_sub_date;
//		//////////////////////////////////////////////////////////////////////////
//
//		snprintf (temp_Date, 100, "%d%.2d%.2d%.2d%.2d", 1, temp_Date_list[ 0 ], temp_Date_list[ 1 ], iTemp_now_hour, iTemp_now_min  );
//		mUSER[mIndex].mAvatarInfo.aUse_AutoPotion_Date = atoi( temp_Date );
//
//	}
//	return mUSER[mIndex].mAvatarInfo.aUse_AutoPotion_Time;
}
// @ Cost Info Download @
void MyGame::CheckUpdateCashItemInfo()
{
    if ((mTickCount % 120) == 0)
    {
        if (mEXTRA_COM.U_UPDATE_CASH_ITEM_INFO_FOR_EXTRA_SEND() == TRUE)
        {
            mCENTER_COM.W_UPDATE_CASH_ITEM_INFO_FOR_CENTER(0);
        }
    }
} // @

int MyGame::ReturnAlliance(int tTribe)
{
	if( mWorldInfo.mAllianceState[0][0] == tTribe )
	{
		return mWorldInfo.mAllianceState[0][1];
	}
	else if( mWorldInfo.mAllianceState[0][1] == tTribe )
	{
		return mWorldInfo.mAllianceState[0][0];
	}
	else if( mWorldInfo.mAllianceState[1][0] == tTribe )
	{
		return mWorldInfo.mAllianceState[1][1];
	}
	else if( mWorldInfo.mAllianceState[1][1] == tTribe )
	{
		return mWorldInfo.mAllianceState[1][0];
	}
	else;
	return -1;
}

// �̵����� �̵� ������ ���� �˻��ϴ� �Լ�.
int MyGame::CheckMoveZoneForMoveZoneItem(int tTribe, int tLevel, int tZoneNumber)
{
	// return 0:�̵�����,  1: ��������,  2:��������, 3:�̵��Ұ� ��
	if (mSERVER_INFO.mServerNumber == tZoneNumber) {
		return 3;
    }

	int tCheckTribe = -1;
	int tCheckLevelMin = 1;

    switch (tZoneNumber)
    {
    case   1 :
    case   2 :
    case   3 :
    case   4 :
        tCheckTribe = 0;
        tCheckLevelMin = 1;
        break;
    case   6 :
    case   7 :
    case   8 :
    case   9 :
        tCheckTribe = 1;
        tCheckLevelMin = 1;
        break;
    case  11 :
    case  12 :
    case  13 :
    case  14 :
        tCheckTribe = 2;
        tCheckLevelMin = 1;
        break;
    case 140 :
    case 141 :
    case 142 :
    case 143 :
        tCheckTribe = 3;
        tCheckLevelMin = 113;
        break;
    // �� ���º�(��������) �������� ��.
    case  40 :
    case  43 :
    case  56 :
        tCheckTribe = 0;
        tCheckLevelMin = 90;
        break;
    case  41 :
    case  44 :
    case  57 :
        tCheckTribe = 1;
        tCheckLevelMin = 90;
        break;
    case  42 :
    case  45 :
    case  58 :
        tCheckTribe = 2;
        tCheckLevelMin = 90;
        break;
    // ���� �� - 90�� �̻�
    case  38 :
    case  55 :
    case  89 :
        tCheckTribe = -1;
        tCheckLevelMin = 90;
        break;
    // ���� �� - 1�� �̻�
    case  75 :
        tCheckTribe = -1;
        tCheckLevelMin = 146;
        break;
    // ���� �� - 5�� �̻�
    case  90 :
        tCheckTribe = -1;
        tCheckLevelMin = 150;
        break;
    // ���� �� - 9�� �̻�
    case 201 :
        tCheckTribe = -1;
        tCheckLevelMin = 154;
        break;
    default :
        return 3; // �� ��ȣ ����.
    }

    // ���尡�� ���� �� ���� �˻�.
    if (tCheckTribe != -1) { // ���� ������ �ִ� �� �˻�.
        if ((tTribe == tCheckTribe) || (tTribe == ReturnAlliance(tCheckTribe))) {
        	if (tLevel >= tCheckLevelMin) {
        		return 0;
            } else {
            	return 1;
            }
        }
        return 2;
    } else { // ���� �� �˻�.
        if (tLevel >= tCheckLevelMin) {
            return 0;
        } else {
            return 1;
        }
    }
}

#ifdef __GOD__
//PROCESS_ZONE_194
void MyGame::Process_Zone_194( void )
{
	int index01;
	time_t tCountSeconds;
	struct tm *tPresentTime;
	int tBroadcastInfoSort;
	BYTE tBroadcastInfoData[MAX_BROADCAST_INFO_SIZE];
	int tTribeUserNum[4];
	int tWinTribe;
	int tGainMoney;
	int tEventSort;
	int tEventValue;

	if( !mCheckActiveBattleLogic )
	{
		return;
	}
	mZone194PostTick++;
	switch( mWorldInfo.mZone194State )
	{
	case 0 :
		switch( mZone194BattleState )
		{
		case 0 :
			time( &tCountSeconds );
			tPresentTime = localtime( &tCountSeconds );
			if( tPresentTime == NULL )
			{
				return;
			}
			if( ( tPresentTime->tm_hour != 20 ) || ( tPresentTime->tm_min < 30 ) || ( tPresentTime->tm_min > 40 ) )
			{
				return;
			}
			mZone194PostTick = 0;
			mZone194BattleState = 1;
			return;
		case 1 :
			mZone194PostTick = 0;
			mZone194BattleState = 2;
			mZone194RemainTime1 = 5;
			return;
		case 2 :
			if( ( mZone194PostTick % ( 120 * 1 ) ) != 0 )
			{
				return;
			}
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 201;
			CopyMemory( &tBroadcastInfoData[0], &mZone194RemainTime1, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mZone194RemainTime1--;
			if( mZone194RemainTime1 == 0 )
			{
				mZone194PostTick = 0;
				mZone194BattleState = 3;
			}
			return;
		case 3 :
			if( mZone194PostTick < ( 120 * 1 ) )
			{
				return;
			}
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 202;
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mZone194State = 1;
			mZone194PostTick = 0;
			mZone194BattleState = 0;
			return;
		}
		return;
	case 1 :
		if( mZone194PostTick < ( 120 * 3 ) )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 203;
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone194State = 2;
		mZone194PostTick = 0;
		mZone194BattleState = 0;
		return;
	case 2 :
		if( mZone194PostTick < ( 120 * 1 ) )
		{
			if( ( mZone194PostTick % 2 ) == 1 )
			{
				mTRANSFER.B_194_BATTLE_COUNTDOWN( ( 60 - ( mZone194PostTick / 2 ) ) );
				mUTIL.Broadcast( FALSE, NULL, 1 );
			}
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 204;
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone194State = 3;
		mZone194PostTick = 0;
		mZone194BattleState = 0;
		mZone194RemainTime2 = 1200;
		mZone194BattleResult[0] = 0;
		mZone194BattleResult[1] = 0;
		mZone194BattleResult[2] = 0;
		mZone194BattleResult[3] = 0;
		return;
	case 3 :
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
		{
			if( !mAVATAR_OBJECT[index01].mCheckValidState )
			{
				continue;
			}
			if( mUSER[index01].mMoveZoneResult == 1 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0 )
			{
				continue;
			}
			break;
		}
		if( index01 == mSERVER.mMAX_USER_NUM )
		{
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 205;
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mZone194State = 5;
			mZone194PostTick = 0;
			mZone194BattleState = 0;
			return;
		}
		mZone194RemainTime2--;
		if( ( mZone194RemainTime2 % 2 ) != 0 )
		{
			return;
		}
		mTRANSFER.B_194_BATTLE_INFO( &mZone194BattleResult[0] );
		mUTIL.Broadcast( FALSE, NULL, 1 );
		tTribeUserNum[0] = 0;
		tTribeUserNum[1] = 0;
		tTribeUserNum[2] = 0;
		tTribeUserNum[3] = 0;
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
		{
			if( !mAVATAR_OBJECT[index01].mCheckValidState )
			{
				continue;
			}
			if( mUSER[index01].mMoveZoneResult == 1 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0 )
			{
				continue;
			}
			switch( mAVATAR_OBJECT[index01].mDATA.aTribe )
			{
			case 0 : tTribeUserNum[0]++; break;
			case 1 : tTribeUserNum[1]++; break;
			case 2 : tTribeUserNum[2]++; break;
			case 3 : tTribeUserNum[3]++; break;
			}
		}
		tWinTribe = -1;
		if( mZone194RemainTime2 == 0 )
		{
			if( tWinTribe == -1 )
			{
				if( ( mZone194BattleResult[0] > 0 ) && ( mZone194BattleResult[0] > mZone194BattleResult[1] ) && ( mZone194BattleResult[0] > mZone194BattleResult[2] ) && ( mZone194BattleResult[0] > mZone194BattleResult[3] ) )
				{
					tWinTribe = 0;
				}
			}
			if( tWinTribe == -1 )
			{
				if( ( mZone194BattleResult[1] > 0 ) && ( mZone194BattleResult[1] > mZone194BattleResult[0] ) && ( mZone194BattleResult[1] > mZone194BattleResult[2] ) && ( mZone194BattleResult[1] > mZone194BattleResult[3] ) )
				{
					tWinTribe = 1;
				}
			}
			if( tWinTribe == -1 )
			{
				if( ( mZone194BattleResult[2] > 0 ) && ( mZone194BattleResult[2] > mZone194BattleResult[0] ) && ( mZone194BattleResult[2] > mZone194BattleResult[1] ) && ( mZone194BattleResult[2] > mZone194BattleResult[3] ) )
				{
					tWinTribe = 2;
				}
			}
			if( tWinTribe == -1 )
			{
				if( ( mZone194BattleResult[3] > 0 ) && ( mZone194BattleResult[3] > mZone194BattleResult[0] ) && ( mZone194BattleResult[3] > mZone194BattleResult[1] ) && ( mZone194BattleResult[3] > mZone194BattleResult[2] ) )
				{
					tWinTribe = 3;
				}
			}
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 206;
			CopyMemory( &tBroadcastInfoData[0], &tWinTribe, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mZone194State = 4;
			mZone194PostTick = 0;
			mZone194BattleState = 0;
			if( tWinTribe == -1 )
			{
				return;
			}
		}
		else
		{
			if( tWinTribe == -1 )
			{
				if( ( tTribeUserNum[0] > 0 ) && ( tTribeUserNum[1] == 0 ) && ( tTribeUserNum[2] == 0 ) && ( tTribeUserNum[3] == 0 ) )
				{
					tWinTribe = 0;
				}
			}
			if( tWinTribe == -1 )
			{
				if( ( tTribeUserNum[1] > 0 ) && ( tTribeUserNum[0] == 0 ) && ( tTribeUserNum[2] == 0 ) && ( tTribeUserNum[3] == 0 ) )
				{
					tWinTribe = 1;
				}
			}
			if( tWinTribe == -1 )
			{
				if( ( tTribeUserNum[2] > 0 ) && ( tTribeUserNum[0] == 0 ) && ( tTribeUserNum[1] == 0 ) && ( tTribeUserNum[3] == 0 ) )
				{
					tWinTribe = 2;
				}
			}
			if( tWinTribe == -1 )
			{
				if( ( tTribeUserNum[3] > 0 ) && ( tTribeUserNum[0] == 0 ) && ( tTribeUserNum[1] == 0 ) && ( tTribeUserNum[2] == 0 ) )
				{
					tWinTribe = 3;
				}
			}
			if( tWinTribe == -1 )
			{
				return;
			}
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 206;
			CopyMemory( &tBroadcastInfoData[0], &tWinTribe, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mZone194State = 4;
			mZone194PostTick = 0;
			mZone194BattleState = 0;
		}
		//----------------//
		//BATTLE_WIN_BONUS//
		//----------------//
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
		{
			if( !mAVATAR_OBJECT[index01].mCheckValidState )
			{
				continue;
			}
			if( mUSER[index01].mMoveZoneResult == 1 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aTribe != tWinTribe )
			{
				continue;
			}
			tGainMoney = 30000000;
			if( !mUTIL.CheckOverMaximum( mUSER[index01].mAvatarInfo.aMoney, tGainMoney ) )
			{
				mUSER[index01].mAvatarInfo.aMoney += tGainMoney;
				mTRANSFER.B_AVATAR_CHANGE_INFO_2( 23, tGainMoney );
				mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			}
			if( mAVATAR_OBJECT[index01].mDATA.aLifeValue < 1 )
			{
				continue;
			}
			mUTIL.ProcessForDropItem( 8, 1041, 0, 0, mUTIL.ReturnItemRecognitionNumber( 0 ), &mAVATAR_OBJECT[index01].mDATA.aAction.aLocation[0], &mUSER[index01].mAvatarInfo.aName[0], 0 );
		}
		//----------------//
		//----------------//
		//----------------//
		//---------//
		//����_����//
		//---------//
		switch( ( rand() % 4 ) )
		{
		case 0 :
			switch( tWinTribe )
			{
			case 0 : //[21]
				tEventSort = 21;
				tEventValue = 1;
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tBroadcastInfoSort = 301;
				CopyMemory( &tBroadcastInfoData[0], &tEventSort, 4 );
				CopyMemory( &tBroadcastInfoData[4], &tEventValue, 4 );
				mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
				//--------------//
				//--------------//
				//--------------//
				break;
			case 1 : //[22]
				tEventSort = 22;
				tEventValue = 1;
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tBroadcastInfoSort = 301;
				CopyMemory( &tBroadcastInfoData[0], &tEventSort, 4 );
				CopyMemory( &tBroadcastInfoData[4], &tEventValue, 4 );
				mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
				//--------------//
				//--------------//
				//--------------//
				break;
			case 2 : //[23]
				tEventSort = 23;
				tEventValue = 1;
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tBroadcastInfoSort = 301;
				CopyMemory( &tBroadcastInfoData[0], &tEventSort, 4 );
				CopyMemory( &tBroadcastInfoData[4], &tEventValue, 4 );
				mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
				//--------------//
				//--------------//
				//--------------//
				break;
			case 3 : //[24]
				tEventSort = 24;
				tEventValue = 1;
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tBroadcastInfoSort = 301;
				CopyMemory( &tBroadcastInfoData[0], &tEventSort, 4 );
				CopyMemory( &tBroadcastInfoData[4], &tEventValue, 4 );
				mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
				//--------------//
				//--------------//
				//--------------//
				break;
			}
			break;
		case 1 :
			switch( tWinTribe )
			{
			case 0 : //[31]
				tEventSort = 31;
				tEventValue = 1;
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tBroadcastInfoSort = 301;
				CopyMemory( &tBroadcastInfoData[0], &tEventSort, 4 );
				CopyMemory( &tBroadcastInfoData[4], &tEventValue, 4 );
				mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
				//--------------//
				//--------------//
				//--------------//
				break;
			case 1 : //[32]
				tEventSort = 32;
				tEventValue = 1;
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tBroadcastInfoSort = 301;
				CopyMemory( &tBroadcastInfoData[0], &tEventSort, 4 );
				CopyMemory( &tBroadcastInfoData[4], &tEventValue, 4 );
				mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
				//--------------//
				//--------------//
				//--------------//
				break;
			case 2 : //[33]
				tEventSort = 33;
				tEventValue = 1;
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tBroadcastInfoSort = 301;
				CopyMemory( &tBroadcastInfoData[0], &tEventSort, 4 );
				CopyMemory( &tBroadcastInfoData[4], &tEventValue, 4 );
				mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
				//--------------//
				//--------------//
				//--------------//
				break;
			case 3 : //[34]
				tEventSort = 34;
				tEventValue = 1;
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tBroadcastInfoSort = 301;
				CopyMemory( &tBroadcastInfoData[0], &tEventSort, 4 );
				CopyMemory( &tBroadcastInfoData[4], &tEventValue, 4 );
				mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
				//--------------//
				//--------------//
				//--------------//
				break;
			}
			break;
		case 2 :
			switch( tWinTribe )
			{
			case 0 : //[41]
				tEventSort = 41;
				tEventValue = 1;
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tBroadcastInfoSort = 301;
				CopyMemory( &tBroadcastInfoData[0], &tEventSort, 4 );
				CopyMemory( &tBroadcastInfoData[4], &tEventValue, 4 );
				mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
				//--------------//
				//--------------//
				//--------------//
				break;
			case 1 : //[42]
				tEventSort = 42;
				tEventValue = 1;
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tBroadcastInfoSort = 301;
				CopyMemory( &tBroadcastInfoData[0], &tEventSort, 4 );
				CopyMemory( &tBroadcastInfoData[4], &tEventValue, 4 );
				mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
				//--------------//
				//--------------//
				//--------------//
				break;
			case 2 : //[43]
				tEventSort = 43;
				tEventValue = 1;
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tBroadcastInfoSort = 301;
				CopyMemory( &tBroadcastInfoData[0], &tEventSort, 4 );
				CopyMemory( &tBroadcastInfoData[4], &tEventValue, 4 );
				mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
				//--------------//
				//--------------//
				//--------------//
				break;
			case 3 : //[44]
				tEventSort = 44;
				tEventValue = 1;
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tBroadcastInfoSort = 301;
				CopyMemory( &tBroadcastInfoData[0], &tEventSort, 4 );
				CopyMemory( &tBroadcastInfoData[4], &tEventValue, 4 );
				mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
				//--------------//
				//--------------//
				//--------------//
				break;
			}
			break;
		case 3 :
			switch( tWinTribe )
			{
			case 0 : //[51]
				tEventSort = 51;
				tEventValue = 1;
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tBroadcastInfoSort = 301;
				CopyMemory( &tBroadcastInfoData[0], &tEventSort, 4 );
				CopyMemory( &tBroadcastInfoData[4], &tEventValue, 4 );
				mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
				//--------------//
				//--------------//
				//--------------//
				break;
			case 1 : //[52]
				tEventSort = 52;
				tEventValue = 1;
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tBroadcastInfoSort = 301;
				CopyMemory( &tBroadcastInfoData[0], &tEventSort, 4 );
				CopyMemory( &tBroadcastInfoData[4], &tEventValue, 4 );
				mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
				//--------------//
				//--------------//
				//--------------//
				break;
			case 2 : //[53]
				tEventSort = 53;
				tEventValue = 1;
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tBroadcastInfoSort = 301;
				CopyMemory( &tBroadcastInfoData[0], &tEventSort, 4 );
				CopyMemory( &tBroadcastInfoData[4], &tEventValue, 4 );
				mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
				//--------------//
				//--------------//
				//--------------//
				break;
			case 3 : //[54]
				tEventSort = 54;
				tEventValue = 1;
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tBroadcastInfoSort = 301;
				CopyMemory( &tBroadcastInfoData[0], &tEventSort, 4 );
				CopyMemory( &tBroadcastInfoData[4], &tEventValue, 4 );
				mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
				//--------------//
				//--------------//
				//--------------//
				break;
			}
			break;
		}
		return;
	case 4 :
		if( mZone194PostTick < ( 120 * 1 ) )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 207;
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone194State = 5;
		mZone194PostTick = 0;
		mZone194BattleState = 0;
		return;
	case 5 :
		if( mZone194PostTick < ( 120 * 1 ) )
		{
			return;
		}
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
		{
			mUSER[index01].Quit(__FILE__, __FUNCTION__, __LINE__);
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 208;
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone194State = 0;
		mZone194PostTick = 0;
		mZone194BattleState = 0;
		return;
	}
}
//PROCESS_ZONE_267_TYPE
void MyGame::Process_Zone_267_TYPE( void )
{
	int index01;
	time_t tCountSeconds;
	struct tm *tPresentTime;
	int tBroadcastInfoSort;
	BYTE tBroadcastInfoData[MAX_BROADCAST_INFO_SIZE];
	int tTribeUserNum[4];
	int tWinTribe;
	int tGainExperience;
	int tGainMoney;

	if( !mCheckActiveBattleLogic )
	{
		return;
	}
	mZone267TypePostTick++;
	switch( mWorldInfo.mZone267TypeState[mZone267TypeZoneIndex] )
	{
	case 0 :
		switch( mZone267TypeBattleState )
		{
		case 0 :
			time( &tCountSeconds );
			tPresentTime = localtime( &tCountSeconds );
			if( tPresentTime == NULL )
			{
				return;
			}
			if( ( tPresentTime->tm_hour != 20 ) || ( tPresentTime->tm_min > 10 ) )
			{
				return;
			}
			mZone267TypePostTick = 0;
			mZone267TypeBattleState = 1;
			mZone267TypeRemainTime1 = 10;
			return;
		case 1 :
			if( ( mZone267TypePostTick % ( 120 * 1 ) ) != 0 )
			{
				return;
			}
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 402;
			CopyMemory( &tBroadcastInfoData[0], &mZone267TypeZoneIndex, 4 );
			CopyMemory( &tBroadcastInfoData[4], &mZone267TypeRemainTime1, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mZone267TypeRemainTime1--;
			if( mZone267TypeRemainTime1 == 0 )
			{
				mZone267TypePostTick = 0;
				mZone267TypeBattleState = 2;
			}
			return;
		case 2 :
			if( mZone267TypePostTick < ( 120 * 1 ) )
			{
				return;
			}
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 403;
			CopyMemory( &tBroadcastInfoData[0], &mZone267TypeZoneIndex, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mZone267TypeState[mZone267TypeZoneIndex] = 1;
			mZone267TypePostTick = 0;
			mZone267TypeBattleState = 0;
			return;
		}
		return;
	case 1 :
		if( mZone267TypePostTick < ( 120 * 3 ) )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 404;
		CopyMemory( &tBroadcastInfoData[0], &mZone267TypeZoneIndex, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone267TypeState[mZone267TypeZoneIndex] = 2;
		mZone267TypePostTick = 0;
		mZone267TypeBattleState = 0;
		return;
	case 2 :
		if( mZone267TypePostTick < ( 120 * 1 ) )
		{
			return;
		}
		mWorldInfo.mZone267TypeState[mZone267TypeZoneIndex] = 3;
		mZone267TypePostTick = 0;
		mZone267TypeBattleState = 0;
		mZone267TypeRemainTime2 = 1800;
		mZone267TypeBattleResult[0] = 0;
		mZone267TypeBattleResult[1] = 0;
		mZone267TypeBattleResult[2] = 0;
		mZone267TypeBattleResult[3] = 0;
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 405;
		CopyMemory( &tBroadcastInfoData[0], &mZone267TypeZoneIndex, 4 );
		CopyMemory( &tBroadcastInfoData[4], &mZone267TypeRemainTime2, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		return;
	case 3 :
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
		{
			if( !mAVATAR_OBJECT[index01].mCheckValidState )
			{
				continue;
			}
			if( mUSER[index01].mMoveZoneResult == 1 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0 )
			{
				continue;
			}
			break;
		}
		if( index01 == mSERVER.mMAX_USER_NUM )
		{
			//--------------//
			//BROADCAST_INFO//
			//--------------//
			tBroadcastInfoSort = 406;
			CopyMemory( &tBroadcastInfoData[0], &mZone267TypeZoneIndex, 4 );
			mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
			//--------------//
			//--------------//
			//--------------//
			mWorldInfo.mZone267TypeState[mZone267TypeZoneIndex] = 5;
			mZone267TypePostTick = 0;
			mZone267TypeBattleState = 0;
			return;
		}
		mZone267TypeRemainTime2--;
		if( ( mZone267TypeRemainTime2 % 10 ) != 0 )
		{
			return;
		}
		mTRANSFER.B_267_TYPE_BATTLE_INFO( &mZone267TypeBattleResult[0], mZone267TypeRemainTime2 );
		mUTIL.Broadcast( FALSE, NULL, 1 );
		tTribeUserNum[0] = 0;
		tTribeUserNum[1] = 0;
		tTribeUserNum[2] = 0;
		tTribeUserNum[3] = 0;
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
		{
			if( !mAVATAR_OBJECT[index01].mCheckValidState )
			{
				continue;
			}
			if( mUSER[index01].mMoveZoneResult == 1 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0 )
			{
				continue;
			}
			switch( mAVATAR_OBJECT[index01].mDATA.aTribe )
			{
			case 0 : tTribeUserNum[0]++; break;
			case 1 : tTribeUserNum[1]++; break;
			case 2 : tTribeUserNum[2]++; break;
			case 3 : tTribeUserNum[3]++; break;
			}
		}
		tWinTribe = -1;
		if( mZone267TypeRemainTime2 == 0 )
		{
			if( tWinTribe == -1 )
			{
				switch( ReturnAllianceTribe( 0 ) )
				{
				case 1 :
					if( ( mZone267TypeBattleResult[0] > mZone267TypeBattleResult[2] ) && ( mZone267TypeBattleResult[0] > mZone267TypeBattleResult[3] ) )
					{
						tWinTribe = 0;
					}
					break;
				case 2 :
					if( ( mZone267TypeBattleResult[0] > mZone267TypeBattleResult[1] ) && ( mZone267TypeBattleResult[0] > mZone267TypeBattleResult[3] ) )
					{
						tWinTribe = 0;
					}
					break;
				case 3 :
					if( ( mZone267TypeBattleResult[0] > mZone267TypeBattleResult[1] ) && ( mZone267TypeBattleResult[0] > mZone267TypeBattleResult[2] ) )
					{
						tWinTribe = 0;
					}
					break;
				default :
					if( ( mZone267TypeBattleResult[0] > mZone267TypeBattleResult[1] ) && ( mZone267TypeBattleResult[0] > mZone267TypeBattleResult[2] ) && ( mZone267TypeBattleResult[0] > mZone267TypeBattleResult[3] ) )
					{
						tWinTribe = 0;
					}
					break;
				}
			}
			if( tWinTribe == -1 )
			{
				switch( ReturnAllianceTribe( 1 ) )
				{
				case 0 :
					if( ( mZone267TypeBattleResult[1] > mZone267TypeBattleResult[2] ) && ( mZone267TypeBattleResult[1] > mZone267TypeBattleResult[3] ) )
					{
						tWinTribe = 1;
					}
					break;
				case 2 :
					if( ( mZone267TypeBattleResult[1] > mZone267TypeBattleResult[0] ) && ( mZone267TypeBattleResult[1] > mZone267TypeBattleResult[3] ) )
					{
						tWinTribe = 1;
					}
					break;
				case 3 :
					if( ( mZone267TypeBattleResult[1] > mZone267TypeBattleResult[0] ) && ( mZone267TypeBattleResult[1] > mZone267TypeBattleResult[2] ) )
					{
						tWinTribe = 1;
					}
					break;
				default :
					if( ( mZone267TypeBattleResult[1] > mZone267TypeBattleResult[0] ) && ( mZone267TypeBattleResult[1] > mZone267TypeBattleResult[2] ) && ( mZone267TypeBattleResult[1] > mZone267TypeBattleResult[3] ) )
					{
						tWinTribe = 1;
					}
					break;
				}
			}
			if( tWinTribe == -1 )
			{
				switch( ReturnAllianceTribe( 2 ) )
				{
				case 0 :
					if( ( mZone267TypeBattleResult[2] > mZone267TypeBattleResult[1] ) && ( mZone267TypeBattleResult[2] > mZone267TypeBattleResult[3] ) )
					{
						tWinTribe = 2;
					}
					break;
				case 1 :
					if( ( mZone267TypeBattleResult[2] > mZone267TypeBattleResult[0] ) && ( mZone267TypeBattleResult[2] > mZone267TypeBattleResult[3] ) )
					{
						tWinTribe = 2;
					}
					break;
				case 3 :
					if( ( mZone267TypeBattleResult[2] > mZone267TypeBattleResult[0] ) && ( mZone267TypeBattleResult[2] > mZone267TypeBattleResult[1] ) )
					{
						tWinTribe = 2;
					}
					break;
				default :
					if( ( mZone267TypeBattleResult[2] > mZone267TypeBattleResult[0] ) && ( mZone267TypeBattleResult[2] > mZone267TypeBattleResult[1] ) && ( mZone267TypeBattleResult[2] > mZone267TypeBattleResult[3] ) )
					{
						tWinTribe = 2;
					}
					break;
				}
			}
			if( tWinTribe == -1 )
			{
				switch( ReturnAllianceTribe( 3 ) )
				{
				case 0 :
					if( ( mZone267TypeBattleResult[3] > mZone267TypeBattleResult[1] ) && ( mZone267TypeBattleResult[3] > mZone267TypeBattleResult[2] ) )
					{
						tWinTribe = 3;
					}
					break;
				case 1 :
					if( ( mZone267TypeBattleResult[3] > mZone267TypeBattleResult[0] ) && ( mZone267TypeBattleResult[3] > mZone267TypeBattleResult[2] ) )
					{
						tWinTribe = 3;
					}
					break;
				case 2 :
					if( ( mZone267TypeBattleResult[3] > mZone267TypeBattleResult[0] ) && ( mZone267TypeBattleResult[3] > mZone267TypeBattleResult[1] ) )
					{
						tWinTribe = 3;
					}
					break;
				default :
					if( ( mZone267TypeBattleResult[3] > mZone267TypeBattleResult[0] ) && ( mZone267TypeBattleResult[3] > mZone267TypeBattleResult[1] ) && ( mZone267TypeBattleResult[3] > mZone267TypeBattleResult[2] ) )
					{
						tWinTribe = 3;
					}
					break;
				}
			}
			if( tWinTribe == -1 )
			{
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tBroadcastInfoSort = 407;
				CopyMemory( &tBroadcastInfoData[0], &mZone267TypeZoneIndex, 4 );
				mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
				//--------------//
				//--------------//
				//--------------//
				mWorldInfo.mZone267TypeState[mZone267TypeZoneIndex] = 5;
				mZone267TypePostTick = 0;
				mZone267TypeBattleState = 0;
				return;
			}
		}
		else
		{
			if( tWinTribe == -1 )
			{
				switch( ReturnAllianceTribe( 0 ) )
				{
				case 1 :
					if( ( tTribeUserNum[0] > 0 ) && ( tTribeUserNum[2] == 0 ) && ( tTribeUserNum[3] == 0 ) )
					{
						tWinTribe = 0;
					}
					break;
				case 2 :
					if( ( tTribeUserNum[0] > 0 ) && ( tTribeUserNum[1] == 0 ) && ( tTribeUserNum[3] == 0 ) )
					{
						tWinTribe = 0;
					}
					break;
				case 3 :
					if( ( tTribeUserNum[0] > 0 ) && ( tTribeUserNum[1] == 0 ) && ( tTribeUserNum[2] == 0 ) )
					{
						tWinTribe = 0;
					}
					break;
				default :
					if( ( tTribeUserNum[0] > 0 ) && ( tTribeUserNum[1] == 0 ) && ( tTribeUserNum[2] == 0 ) && ( tTribeUserNum[3] == 0 ) )
					{
						tWinTribe = 0;
					}
					break;
				}
			}
			if( tWinTribe == -1 )
			{
				switch( ReturnAllianceTribe( 1 ) )
				{
				case 0 :
					if( ( tTribeUserNum[1] > 0 ) && ( tTribeUserNum[2] == 0 ) && ( tTribeUserNum[3] == 0 ) )
					{
						tWinTribe = 1;
					}
					break;
				case 2 :
					if( ( tTribeUserNum[1] > 0 ) && ( tTribeUserNum[0] == 0 ) && ( tTribeUserNum[3] == 0 ) )
					{
						tWinTribe = 1;
					}
					break;
				case 3 :
					if( ( tTribeUserNum[1] > 0 ) && ( tTribeUserNum[0] == 0 ) && ( tTribeUserNum[2] == 0 ) )
					{
						tWinTribe = 1;
					}
					break;
				default :
					if( ( tTribeUserNum[1] > 0 ) && ( tTribeUserNum[0] == 0 ) && ( tTribeUserNum[2] == 0 ) && ( tTribeUserNum[3] == 0 ) )
					{
						tWinTribe = 1;
					}
					break;
				}
			}
			if( tWinTribe == -1 )
			{
				switch( ReturnAllianceTribe( 2 ) )
				{
				case 0 :
					if( ( tTribeUserNum[2] > 0 ) && ( tTribeUserNum[1] == 0 ) && ( tTribeUserNum[3] == 0 ) )
					{
						tWinTribe = 2;
					}
					break;
				case 1 :
					if( ( tTribeUserNum[2] > 0 ) && ( tTribeUserNum[0] == 0 ) && ( tTribeUserNum[3] == 0 ) )
					{
						tWinTribe = 2;
					}
					break;
				case 3 :
					if( ( tTribeUserNum[2] > 0 ) && ( tTribeUserNum[0] == 0 ) && ( tTribeUserNum[1] == 0 ) )
					{
						tWinTribe = 2;
					}
					break;
				default :
					if( ( tTribeUserNum[2] > 0 ) && ( tTribeUserNum[0] == 0 ) && ( tTribeUserNum[1] == 0 ) && ( tTribeUserNum[3] == 0 ) )
					{
						tWinTribe = 2;
					}
					break;
				}
			}
			if( tWinTribe == -1 )
			{
				switch( ReturnAllianceTribe( 3 ) )
				{
				case 0 :
					if( ( tTribeUserNum[3] > 0 ) && ( tTribeUserNum[1] == 0 ) && ( tTribeUserNum[2] == 0 ) )
					{
						tWinTribe = 3;
					}
					break;
				case 1 :
					if( ( tTribeUserNum[3] > 0 ) && ( tTribeUserNum[0] == 0 ) && ( tTribeUserNum[2] == 0 ) )
					{
						tWinTribe = 3;
					}
					break;
				case 2 :
					if( ( tTribeUserNum[3] > 0 ) && ( tTribeUserNum[0] == 0 ) && ( tTribeUserNum[1] == 0 ) )
					{
						tWinTribe = 3;
					}
					break;
				default :
					if( ( tTribeUserNum[3] > 0 ) && ( tTribeUserNum[0] == 0 ) && ( tTribeUserNum[1] == 0 ) && ( tTribeUserNum[2] == 0 ) )
					{
						tWinTribe = 3;
					}
					break;
				}
			}
			if( tWinTribe == -1 )
			{
				return;
			}
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 408;
		CopyMemory( &tBroadcastInfoData[0], &mZone267TypeZoneIndex, 4 );
		CopyMemory( &tBroadcastInfoData[4], &tWinTribe, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone267TypeState[mZone267TypeZoneIndex] = 4;
		mZone267TypePostTick = 0;
		mZone267TypeBattleState = 0;
		//----------------//
		//BATTLE_WIN_BONUS//
		//----------------//
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
		{
			if( !mAVATAR_OBJECT[index01].mCheckValidState )
			{
				continue;
			}
			if( mUSER[index01].mMoveZoneResult == 1 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aVisibleState == 0 )
			{
				continue;
			}
			if( ( mAVATAR_OBJECT[index01].mDATA.aTribe != tWinTribe ) && ( mAVATAR_OBJECT[index01].mDATA.aTribe != ReturnAllianceTribe( tWinTribe ) ) )
			{
				ReturnLevelWarReward3( mAVATAR_OBJECT[index01].mDATA.aLevel1, mAVATAR_OBJECT[index01].mDATA.aLevel2, &tGainExperience, &tGainMoney );
				tGainExperience /= 2;
				tGainMoney /= 2;
				if( tGainExperience < 1 )
				{
					continue;
				}
				mTRANSFER.B_AVATAR_CHANGE_INFO_2( 1, tGainExperience );
				mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
				mUTIL.ProcessForExperience( index01, tGainExperience, 0, 0 );
				if( !mUTIL.CheckOverMaximum( mUSER[index01].mAvatarInfo.aMoney, tGainMoney ) )
				{
					mUSER[index01].mAvatarInfo.aMoney += tGainMoney;
					mTRANSFER.B_AVATAR_CHANGE_INFO_2( 23, tGainMoney );
					mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
				}
			}
			else
			{
				ReturnLevelWarReward3( mAVATAR_OBJECT[index01].mDATA.aLevel1, mAVATAR_OBJECT[index01].mDATA.aLevel2, &tGainExperience, &tGainMoney );
				if( tGainExperience < 1 )
				{
					continue;
				}
				mTRANSFER.B_AVATAR_CHANGE_INFO_2( 1, tGainExperience );
				mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
				mUTIL.ProcessForExperience( index01, tGainExperience, 0, 0 );
				if( !mUTIL.CheckOverMaximum( mUSER[index01].mAvatarInfo.aMoney, tGainMoney ) )
				{
					mUSER[index01].mAvatarInfo.aMoney += tGainMoney;
					mTRANSFER.B_AVATAR_CHANGE_INFO_2( 23, tGainMoney );
					mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
				}
			}
		}
		//----------------//
		//----------------//
		//----------------//
		return;
	case 4 :
		if( mZone267TypePostTick < ( 120 * 1 ) )
		{
			return;
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 409;
		CopyMemory( &tBroadcastInfoData[0], &mZone267TypeZoneIndex, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone267TypeState[mZone267TypeZoneIndex] = 5;
		mZone267TypePostTick = 0;
		mZone267TypeBattleState = 0;
		return;
	case 5 :
		if( mZone267TypePostTick < ( 120 * 1 ) )
		{
			return;
		}
		for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
		{
			mUSER[index01].Quit(__FILE__, __FUNCTION__, __LINE__);
		}
		//--------------//
		//BROADCAST_INFO//
		//--------------//
		tBroadcastInfoSort = 410;
		CopyMemory( &tBroadcastInfoData[0], &mZone267TypeZoneIndex, 4 );
		mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
		//--------------//
		//--------------//
		//--------------//
		mWorldInfo.mZone267TypeState[mZone267TypeZoneIndex] = 0;
		mZone267TypePostTick = 0;
		mZone267TypeBattleState = 0;
		return;
	}
}
//RETURN_LEVEL_WAR_REWARD_3
void MyGame::ReturnLevelWarReward3( int tLevel1, int tLevel2, int *tGainExperience, int *tGainMoney )
{
	*tGainExperience = 0;
	*tGainMoney = 0;
	switch( tLevel2 )
	{
	case 1 :
		*tGainExperience = 9621058;
		*tGainMoney = 15000000;
		return;
	case 2 :
		*tGainExperience = 10005901;
		*tGainMoney = 15000000;
		return;
	case 3 :
		*tGainExperience = 10406137;
		*tGainMoney = 15000000;
		return;
	case 4 :
		*tGainExperience = 10822382;
		*tGainMoney = 15000000;
		return;
	case 5 :
		*tGainExperience = 5627638;
		*tGainMoney = 20000000;
		return;
	case 6 :
		*tGainExperience = 5852744;
		*tGainMoney = 20000000;
		return;
	case 7 :
		*tGainExperience = 6086854;
		*tGainMoney = 20000000;
		return;
	case 8 :
		*tGainExperience = 6330328;
		*tGainMoney = 20000000;
		return;
	case 9 :
		*tGainExperience = 3291770;
		*tGainMoney = 25000000;
		return;
	case 10 :
		*tGainExperience = 3423441;
		*tGainMoney = 25000000;
		return;
	case 11 :
		*tGainExperience = 3560379;
		*tGainMoney = 25000000;
		return;
	case 12 :
		*tGainExperience = 3702794;
		*tGainMoney = 25000000;
		return;
	}
	switch( tLevel1 )
	{
	case  70 :
		*tGainExperience = 43936;
		*tGainMoney = 500000;
		break;
	case  71 :
		*tGainExperience = 44588;
		*tGainMoney = 500000;
		break;
	case  72 :
		*tGainExperience = 45240;
		*tGainMoney = 500000;
		break;
	case  73 :
		*tGainExperience = 45891;
		*tGainMoney = 500000;
		break;
	case  74 :
		*tGainExperience = 46541;
		*tGainMoney = 500000;
		break;
	case  75 :
		*tGainExperience = 47190;
		*tGainMoney = 500000;
		break;
	case  76 :
		*tGainExperience = 47838;
		*tGainMoney = 500000;
		break;
	case  77 :
		*tGainExperience = 48485;
		*tGainMoney = 500000;
		break;
	case  78 :
		*tGainExperience = 49131;
		*tGainMoney = 500000;
		break;
	case  79 :
		*tGainExperience = 49775;
		*tGainMoney = 500000;
		break;
	case  80 :
		*tGainExperience = 50421;
		*tGainMoney = 500000;
		break;
	case  81 :
		*tGainExperience = 51064;
		*tGainMoney = 500000;
		break;
	case  82 :
		*tGainExperience = 51705;
		*tGainMoney = 500000;
		break;
	case  83 :
		*tGainExperience = 52346;
		*tGainMoney = 500000;
		break;
	case  84 :
		*tGainExperience = 52988;
		*tGainMoney = 500000;
		break;
	case  85 :
		*tGainExperience = 53627;
		*tGainMoney = 500000;
		break;
	case  86 :
		*tGainExperience = 54266;
		*tGainMoney = 500000;
		break;
	case  87 :
		*tGainExperience = 54905;
		*tGainMoney = 500000;
		break;
	case  88 :
		*tGainExperience = 55542;
		*tGainMoney = 500000;
		break;
	case  89 :
		*tGainExperience = 87927;
		*tGainMoney = 500000;
		break;
	case  90 :
		*tGainExperience = 114075;
		*tGainMoney = 500000;
		break;
	case  91 :
		*tGainExperience = 50639;
		*tGainMoney = 2000000;
		break;
	case  92 :
		*tGainExperience = 54193;
		*tGainMoney = 2000000;
		break;
	case  93 :
		*tGainExperience = 56998;
		*tGainMoney = 2000000;
		break;
	case  94 :
		*tGainExperience = 59339;
		*tGainMoney = 2000000;
		break;
	case  95 :
		*tGainExperience = 61359;
		*tGainMoney = 2000000;
		break;
	case  96 :
		*tGainExperience = 63143;
		*tGainMoney = 2000000;
		break;
	case  97 :
		*tGainExperience = 64745;
		*tGainMoney = 2000000;
		break;
	case  98 :
		*tGainExperience = 66203;
		*tGainMoney = 2000000;
		break;
	case  99 :
		*tGainExperience = 77015;
		*tGainMoney = 2000000;
		break;
	case 100 :
		*tGainExperience = 99918;
		*tGainMoney = 2000000;
		break;
	case 101 :
		*tGainExperience = 110885;
		*tGainMoney = 2000000;
		break;
	case 102 :
		*tGainExperience = 118666;
		*tGainMoney = 2000000;
		break;
	case 103 :
		*tGainExperience = 124811;
		*tGainMoney = 2000000;
		break;
	case 104 :
		*tGainExperience = 129936;
		*tGainMoney = 2000000;
		break;
	case 105 :
		*tGainExperience = 134359;
		*tGainMoney = 2000000;
		break;
	case 106 :
		*tGainExperience = 138265;
		*tGainMoney = 2000000;
		break;
	case 107 :
		*tGainExperience = 141774;
		*tGainMoney = 2000000;
		break;
	case 108 :
		*tGainExperience = 144966;
		*tGainMoney = 2000000;
		break;
	case 109 :
		*tGainExperience = 147899;
		*tGainMoney = 2000000;
		break;
	case 110 :
		*tGainExperience = 150616;
		*tGainMoney = 2000000;
		break;
	case 111 :
		*tGainExperience = 153150;
		*tGainMoney = 2000000;
		break;
	case 112 :
		*tGainExperience = 3189692;
		*tGainMoney = 2000000;
		break;
	case 113 :
		*tGainExperience = 1034574;
		*tGainMoney = 5000000;
		break;
	case 114 :
		*tGainExperience = 1148127;
		*tGainMoney = 5000000;
		break;
	case 115 :
		*tGainExperience = 1228701;
		*tGainMoney = 5000000;
		break;
	case 116 :
		*tGainExperience = 1292318;
		*tGainMoney = 5000000;
		break;
	case 117 :
		*tGainExperience = 1345386;
		*tGainMoney = 5000000;
		break;
	case 118 :
		*tGainExperience = 1391182;
		*tGainMoney = 5000000;
		break;
	case 119 :
		*tGainExperience = 1431630;
		*tGainMoney = 5000000;
		break;
	case 120 :
		*tGainExperience = 1467959;
		*tGainMoney = 5000000;
		break;
	case 121 :
		*tGainExperience = 1501007;
		*tGainMoney = 5000000;
		break;
	case 122 :
		*tGainExperience = 1531376;
		*tGainMoney = 5000000;
		break;
	case 123 :
		*tGainExperience = 1559509;
		*tGainMoney = 5000000;
		break;
	case 124 :
		*tGainExperience = 1585746;
		*tGainMoney = 5000000;
		break;
	case 125 :
		*tGainExperience = 1610352;
		*tGainMoney = 5000000;
		break;
	case 126 :
		*tGainExperience = 1633542;
		*tGainMoney = 5000000;
		break;
	case 127 :
		*tGainExperience = 1655482;
		*tGainMoney = 5000000;
		break;
	case 128 :
		*tGainExperience = 1676317;
		*tGainMoney = 5000000;
		break;
	case 129 :
		*tGainExperience = 1696166;
		*tGainMoney = 5000000;
		break;
	case 130 :
		*tGainExperience = 1715126;
		*tGainMoney = 5000000;
		break;
	case 131 :
		*tGainExperience = 1733283;
		*tGainMoney = 5000000;
		break;
	case 132 :
		*tGainExperience = 1750710;
		*tGainMoney = 5000000;
		break;
	case 133 :
		*tGainExperience = 1767469;
		*tGainMoney = 5000000;
		break;
	case 134 :
		*tGainExperience = 1783613;
		*tGainMoney = 5000000;
		break;
	case 135 :
		*tGainExperience = 1799197;
		*tGainMoney = 5000000;
		break;
	case 136 :
		*tGainExperience = 1814256;
		*tGainMoney = 5000000;
		break;
	case 137 :
		*tGainExperience = 1097299;
		*tGainMoney = 7000000;
		break;
	case 138 :
		*tGainExperience = 1105776;
		*tGainMoney = 7000000;
		break;
	case 139 :
		*tGainExperience = 1113995;
		*tGainMoney = 7000000;
		break;
	case 140 :
		*tGainExperience = 1121987;
		*tGainMoney = 7000000;
		break;
	case 141 :
		*tGainExperience = 1129749;
		*tGainMoney = 7000000;
		break;
	case 142 :
		*tGainExperience = 1137310;
		*tGainMoney = 7000000;
		break;
	case 143 :
		*tGainExperience = 1144669;
		*tGainMoney = 7000000;
		break;
	case 144 :
		*tGainExperience = 1151850;
		*tGainMoney = 7000000;
		break;
	case 145 :
		*tGainExperience = 9251018;
		*tGainMoney = 8000000;
		break;
	}
}
#endif
//-------------------------------------------------------------------------------------------------
