//-------------------------------------------------------------------------------------------------
//HEADER
//-------------------------------------------------------------------------------------------------
#include "H01_MainApplication.h"
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_GAME
//-------------------------------------------------------------------------------------------------
//VARIABLE_FOR_ATTACK_FUNCTION
#define PROTECT_TICK                  20
static int tMinDamageValueWithAvatar = 5;
static AVATAR_OBJECT *tAVATAR_OBJECT1;
static AVATAR_OBJECT *tAVATAR_OBJECT2;
static MONSTER_OBJECT *tMONSTER_OBJECT;
static int tChargingValue;
static int tAttackSuccessValue;
static int tAttackBlockValue;
static int tAttackDetermineValue;
static int tAttackPowerValue;
static float tAttackPowerUpRatio;
static int tDefensePowerValue;
static int tDamageValue;
static int tReturnDamageProbability;
static int tReturnDamageValue;
static int tShieldDamageValue;
static int tElementAttackPower;
static float tElementAttackPowerUpRatio;
static int tElementDefensePower;
static float tDeathDirection[3];
static float tDeathLength;
//CHECK_POSSIBLE_ATTACK_TARGET
BOOL MyGame::CheckPossibleAttackTarget( int tTargetSort, int tActionSort )
{
	switch( tTargetSort )
	{
	case 1 :
		switch( tActionSort )
		{
		case  0 :
		case 12 :
			return FALSE;
		}
		return TRUE;
	case 2 :
		switch( tActionSort )
		{
		case  0 :
		case 12 :
		case 19 :
			return FALSE;
		}
		return TRUE;
	default :
		return FALSE;
	}
	return TRUE;
}
//PROCESS_ATTACK_01[아바타->아바타(비무)]

//attacker와 defenser간의 크리티컬 수치를 이용하여 크리티컬 유효 숫자를 리턴합니다.
unsigned int MyGame::getCriticalAttackDefValue( AVATAR_OBJECT *_attacker, AVATAR_OBJECT *_defenser )
{
	unsigned int t_cmp_critical;
	int t_critical;
	int t_critical_defense;

	t_cmp_critical = 0;
	t_critical = _attacker->GetCritical();
	t_critical_defense = _defenser->GetCriticalDefense();
	
	if( t_critical > t_critical_defense )
	{
		t_cmp_critical = ( unsigned int )( t_critical - t_critical_defense );
	}

	return t_cmp_critical;
}

void MyGame::ProcessAttack01( int tUserIndex, ATTACK_FOR_PROTOCOL *tAttackInfo )
{
#ifdef __GOD__
	int index01;
	int index02;
	int tBonusSkillGrade;
#endif
	unsigned int tRandom = 0;
	if( ( tAttackInfo->mServerIndex1 != tUserIndex ) || ( tAttackInfo->mUniqueNumber1 != mAVATAR_OBJECT[tUserIndex].mUniqueNumber ) )
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			               
			               
		return;
	}
	tAVATAR_OBJECT1 = &mAVATAR_OBJECT[tAttackInfo->mServerIndex1];
	tAVATAR_OBJECT1->mDATA.aAction.aLocation[0] = tAttackInfo->mSenderLocation[0];
	tAVATAR_OBJECT1->mDATA.aAction.aLocation[1] = tAttackInfo->mSenderLocation[1];
	tAVATAR_OBJECT1->mDATA.aAction.aLocation[2] = tAttackInfo->mSenderLocation[2];
	tAVATAR_OBJECT1->SetSpaceIndex();
	if( ( tAttackInfo->mServerIndex2 < 0 ) || ( tAttackInfo->mServerIndex2 > ( mSERVER.mMAX_USER_NUM - 1 ) ) )
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			               
			               
		return;
	}
	tAVATAR_OBJECT2 = &mAVATAR_OBJECT[tAttackInfo->mServerIndex2];
	if( ( !tAVATAR_OBJECT2->mCheckValidState ) || ( tAVATAR_OBJECT2->mUniqueNumber != tAttackInfo->mUniqueNumber2 ) )
	{
		return;
	}
	if( tAVATAR_OBJECT1->mDATA.aLifeValue < 1 )
	{
		return;
	}
	if( ( mTickCount - mUSER[tAttackInfo->mServerIndex1].mTickCountFor01SecondForProtect ) < PROTECT_TICK )
	{
		return;
	}
	if( mUSER[tAttackInfo->mServerIndex2].mMoveZoneResult == 1 )
	{
		return;
	}
	if( tAVATAR_OBJECT2->mDATA.aLifeValue < 1 )
	{
		return;
	}
	if( tAVATAR_OBJECT2->mDATA.aPShopState == 1 )
	{
		return;
	}
	if( !CheckPossibleAttackTarget( 1, tAVATAR_OBJECT2->mDATA.aAction.aSort ) )
	{
		return;
	}
	if( ( mTickCount - mUSER[tAttackInfo->mServerIndex2].mTickCountFor01SecondForProtect ) < PROTECT_TICK )
	{
		return;
	}
	if( ( tAVATAR_OBJECT1->mDATA.aDuelState[0] != 1 ) || ( tAVATAR_OBJECT2->mDATA.aDuelState[0] != 1 ) || ( tAVATAR_OBJECT1->mDATA.aDuelState[1] != tAVATAR_OBJECT2->mDATA.aDuelState[1] ) || ( tAVATAR_OBJECT1->mDATA.aDuelState[2] == tAVATAR_OBJECT2->mDATA.aDuelState[2] ) )
	{
		return;
	}
	tChargingValue = mUSER[tAttackInfo->mServerIndex1].mEffectValue[8][0];
	if( mUSER[tAttackInfo->mServerIndex1].mEffectValue[8][0] > 0 )
	{
		mUSER[tAttackInfo->mServerIndex1].mEffectValue[8][0] = 0;
		mUSER[tAttackInfo->mServerIndex1].mEffectValue[8][1] = 0;
		tAVATAR_OBJECT1->mDATA.aEffectValueForView[8] = 0;
		mTRANSFER.B_AVATAR_CHANGE_INFO_1( tAttackInfo->mServerIndex1, tAttackInfo->mUniqueNumber1, 2, 0, 0, 0 );
        mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
	}
	switch( tAttackInfo->mAttackActionValue1 )
	{
	case 1 :
		tAttackSuccessValue = tAVATAR_OBJECT1->GetAttackSuccess();
		break;
	case 2 :
		if( mUSER[tUserIndex].mCheckMaxAttackPacketNum == 1 )
		{
			if( ( tAVATAR_OBJECT1->mDATA.aAction.aSkillNumber != tAttackInfo->mAttackActionValue2 ) || ( ( tAVATAR_OBJECT1->mDATA.aAction.aSkillGradeNum1 + tAVATAR_OBJECT1->mDATA.aAction.aSkillGradeNum2 ) != tAttackInfo->mAttackActionValue3 ) )
			{
				return;
			}
		}
		tAttackSuccessValue = tAVATAR_OBJECT1->GetAttackSuccess();
		break;
	default :
		return;
	}
	if( tAttackSuccessValue < 1 )
	{
		return;
	}
	tAttackBlockValue = tAVATAR_OBJECT2->GetAttackBlock();
	if( tAttackBlockValue > 0 )
	{
		if( tAttackSuccessValue > tAttackBlockValue )
		{
			tAttackDetermineValue = (int) ( 70.0f + ( ( (float) tAttackSuccessValue / (float) tAttackBlockValue ) - 1.0f ) * 25.0f );
			if( tAttackDetermineValue > 99 )
			{
				tAttackDetermineValue = 99;
			}
		}
		else
		{
			tAttackDetermineValue = (int) ( 70.0f - ( ( (float) tAttackBlockValue / (float) tAttackSuccessValue ) - 1.0f ) * 25.0f );
			if( tAttackDetermineValue < 1 )
			{
				tAttackDetermineValue = 1;
			}
		}
		tRandom = rand_mir();
		if( ( tRandom % 100 ) >= (unsigned int)tAttackDetermineValue )
		{
			tAttackInfo->mAttackResultValue = 0;
			mTRANSFER.B_PROCESS_ATTACK_RECV( tAttackInfo );
			mUSER[tAttackInfo->mServerIndex1].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			return;
		}
	}
	switch( tAttackInfo->mAttackActionValue1 )
	{
	case 1 :
		tAttackPowerValue = tAVATAR_OBJECT1->GetAttackPower();
		break;
	case 2 :
		tAttackPowerValue = tAVATAR_OBJECT1->GetAttackPower();
		tAttackPowerUpRatio = mSKILL.ReturnSkillValue( tAttackInfo->mAttackActionValue2, tAttackInfo->mAttackActionValue3, 7 );
		if( tAttackPowerUpRatio > 0.0f )
		{
			tAttackPowerValue = (int) ( ( float) tAttackPowerValue * ( tAttackPowerUpRatio + 100.0f ) * 0.01f );
		}
		break;
	}
	tDefensePowerValue = tAVATAR_OBJECT2->GetDefensePower();
	tDamageValue = tAttackPowerValue - tDefensePowerValue;
	if( tDamageValue < 1 )
	{
		tDamageValue = 1;
	}
	if( tChargingValue > 0 )
	{
		tDamageValue = (int) ( (float) ( tDamageValue * ( tChargingValue + 100 ) ) * 0.01f );
	} 
	tRandom = rand_mir();
	if( ( tRandom % 2 ) == 0 )
	{
		tRandom = rand_mir();
		tDamageValue += (int) ( (float) ( tDamageValue * ( tRandom % 11 ) ) * 0.01f );
	}
	else
	{
		tRandom = rand_mir();
		tDamageValue -= (int) ( (float) ( tDamageValue * ( tRandom % 11 ) ) * 0.01f );
	}
	if( tDamageValue < tMinDamageValueWithAvatar )
	{
		tDamageValue = tMinDamageValueWithAvatar;
	}
	tAttackInfo->mAttackCriticalExist = 0;
	switch( tAttackInfo->mAttackActionValue1 )
	{
	case 1 :
		tRandom = ( rand_mir() % 100 );
		if( tRandom < getCriticalAttackDefValue( tAVATAR_OBJECT1, tAVATAR_OBJECT2 ) )
		{
			tDamageValue *= 2;
			tAttackInfo->mAttackCriticalExist = 1;
		}
		break;
	case 2 :
		if( tAttackInfo->mAttackActionValue2 == 78 )
		{
			break;
		}
		switch( mSKILL.ReturnAttackType( tAttackInfo->mAttackActionValue2 ) )
		{
		case 2 :
		case 5 :
			tRandom = ( rand_mir() % 100 );
			if( tRandom < getCriticalAttackDefValue( tAVATAR_OBJECT1, tAVATAR_OBJECT2 ) )
			{
				tDamageValue *= 2;
				tAttackInfo->mAttackCriticalExist = 1;
			}
			break;
		}
		break;
	}
	tDamageValue /= tMinDamageValueWithAvatar;
	if( mUSER[tAttackInfo->mServerIndex1].mEffectValue[14][0] > 0 )
	{
		tRandom = rand_mir();
		if( ( tRandom % 1000 ) < (unsigned int)mUSER[tAttackInfo->mServerIndex1].mEffectValue[14][0] )
		{
			mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][0] = 0;
			mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][1] = 0;
			tAVATAR_OBJECT2->mDATA.aEffectValueForView[9] = 0;
			mTRANSFER.B_AVATAR_CHANGE_INFO_1( tAttackInfo->mServerIndex2, tAttackInfo->mUniqueNumber2, 3, 0, 0, 0 );
			mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT2->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
		}
	}
	if( mUSER[tAttackInfo->mServerIndex2].mEffectValue[12][0] > 0 )
	{
		tReturnDamageProbability = mUSER[tAttackInfo->mServerIndex2].mEffectValue[12][0];
		if( tAVATAR_OBJECT1->mDATA.aLevel1 > tAVATAR_OBJECT2->mDATA.aLevel1 )
		{
			tReturnDamageProbability -= ( ( tAVATAR_OBJECT1->mDATA.aLevel1 - tAVATAR_OBJECT2->mDATA.aLevel1 ) * 3 );
		}
		tRandom = rand_mir();
		if(tReturnDamageProbability < 0) {
			tReturnDamageProbability = 0;
		}
		if( ( tRandom % 1000 ) < tReturnDamageProbability )
		{
			tRandom = rand_mir();
			tReturnDamageValue = (int) ( (float) ( tDamageValue * ( 100 + tRandom % 51 ) ) * 0.01f );
			mTRANSFER.B_AVATAR_CHANGE_INFO_1( tAttackInfo->mServerIndex1, tAttackInfo->mUniqueNumber1, 4, tReturnDamageValue, 0, 0 );
            mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
			tAVATAR_OBJECT1->mDATA.aLifeValue -= tReturnDamageValue;
			if( tAVATAR_OBJECT1->mDATA.aLifeValue < 1 )
			{
				tAVATAR_OBJECT1->mDATA.aLifeValue = 0;
				//------------------------//
				//PROCESS_FOR_AVATAR_DEATH//
				//------------------------//
				tDeathDirection[0] = tAVATAR_OBJECT1->mDATA.aAction.aLocation[0] - tAVATAR_OBJECT2->mDATA.aAction.aLocation[0];
				tDeathDirection[2] = tAVATAR_OBJECT1->mDATA.aAction.aLocation[2] - tAVATAR_OBJECT2->mDATA.aAction.aLocation[2];
				tDeathLength = sqrtf( tDeathDirection[0] * tDeathDirection[0] + tDeathDirection[2] * tDeathDirection[2] );
				if( tDeathLength < 1.0f )
				{
					tDeathDirection[0] = 0.0f;
					tDeathDirection[2] = 0.0f;
				}
				else
				{
					tDeathDirection[0] /= tDeathLength;
					tDeathDirection[2] /= tDeathLength;
				}
				tAVATAR_OBJECT1->mDATA.aAction.aType = 0;
				tAVATAR_OBJECT1->mDATA.aAction.aSort = 12;
				tAVATAR_OBJECT1->mDATA.aAction.aFrame = 0.0f;
				tAVATAR_OBJECT1->mDATA.aAction.aTargetLocation[0] = tDeathDirection[0];
				tAVATAR_OBJECT1->mDATA.aAction.aTargetLocation[1] = 0.0f;
				tAVATAR_OBJECT1->mDATA.aAction.aTargetLocation[2] = tDeathDirection[2];
				tAVATAR_OBJECT1->mDATA.aAction.aFront = mUTIL.GetYAngle( tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], tAVATAR_OBJECT1->mDATA.aAction.aLocation[2], tAVATAR_OBJECT2->mDATA.aAction.aLocation[0], tAVATAR_OBJECT2->mDATA.aAction.aLocation[2] );
				tAVATAR_OBJECT1->mDATA.aAction.aTargetFront = tAVATAR_OBJECT1->mDATA.aAction.aFront;
				tAVATAR_OBJECT1->mDATA.aAction.aSkillNumber = 1;
				mTRANSFER.B_AVATAR_ACTION_RECV( tAttackInfo->mServerIndex1, tAVATAR_OBJECT1->mUniqueNumber, &tAVATAR_OBJECT1->mDATA, 1 );
				mUSER[tAttackInfo->mServerIndex1].Send (TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
                mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1 ,  tAttackInfo -> mServerIndex1, false);
				tAVATAR_OBJECT1->ProcessForDeath();
				//------------------------//
				//------------------------//
				//------------------------//
			}
			return;
		}
	}
	if( mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][0] > 0 )
	{
		if( tDamageValue > mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][0] )
		{
			tShieldDamageValue = mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][0];
		}
		else
		{
			tShieldDamageValue = tDamageValue;
		}
		mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][0] -= tShieldDamageValue;
		if( mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][0] < 1 )
		{
			mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][0] = 0;
			mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][1] = 0;
		}
		tAVATAR_OBJECT2->mDATA.aEffectValueForView[9] -= tShieldDamageValue;
		if( tAVATAR_OBJECT2->mDATA.aEffectValueForView[9] < 1 )
		{
			tAVATAR_OBJECT2->mDATA.aEffectValueForView[9] = 0;
		}
		mTRANSFER.B_AVATAR_CHANGE_INFO_1( tAttackInfo->mServerIndex2, tAttackInfo->mUniqueNumber2, 5, tShieldDamageValue, 0, 0 );
        mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT2->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
		tDamageValue -= tShieldDamageValue;
	}
	tElementAttackPower = tAVATAR_OBJECT1->GetElementAttackPower();
	switch( tAttackInfo->mAttackActionValue1 )
	{
	case 3 :
		tElementAttackPowerUpRatio = mSKILL.ReturnSkillValue( tAttackInfo->mAttackActionValue2, tAttackInfo->mAttackActionValue3, 8 );
		if( tElementAttackPowerUpRatio > 0.0f )
		{
			tElementAttackPower = (int) ( ( float) tElementAttackPower * ( tElementAttackPowerUpRatio + 100.0f ) * 0.01f );
		}
		break;
	}
	tElementDefensePower = tAVATAR_OBJECT2->GetElementDefensePower();
	if( tElementAttackPower > tElementDefensePower )
	{
		tDamageValue += ( tElementAttackPower - tElementDefensePower );
		tAttackInfo->mAttackElementDamage = tElementAttackPower - tElementDefensePower;
	}
	else
	{
		tAttackInfo->mAttackElementDamage = 0;
	}
	tAttackInfo->mAttackResultValue = 1 + mUSER[tAttackInfo->mServerIndex1].mAvatarInfo.aEquip[7][0];
	tAttackInfo->mAttackViewDamageValue = tDamageValue;
	if( tAVATAR_OBJECT2->mDATA.aLifeValue < tDamageValue )
	{
		tDamageValue = tAVATAR_OBJECT2->mDATA.aLifeValue;
	}
	tAttackInfo->mAttackRealDamageValue = tDamageValue;
	mTRANSFER.B_PROCESS_ATTACK_RECV( tAttackInfo );
	mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
	tAVATAR_OBJECT2->mDATA.aLifeValue -= tDamageValue;
	if( tAVATAR_OBJECT2->mDATA.aLifeValue > 0 )
	{
		return;
	}
	//------------------------//
	//PROCESS_FOR_AVATAR_DEATH//
	//------------------------//
	tDeathDirection[0] = tAVATAR_OBJECT2->mDATA.aAction.aLocation[0] - tAVATAR_OBJECT1->mDATA.aAction.aLocation[0];
	tDeathDirection[2] = tAVATAR_OBJECT2->mDATA.aAction.aLocation[2] - tAVATAR_OBJECT1->mDATA.aAction.aLocation[2];
	tDeathLength = sqrtf( tDeathDirection[0] * tDeathDirection[0] + tDeathDirection[2] * tDeathDirection[2] );
	if( tDeathLength < 1.0f )
	{
		tDeathDirection[0] = 0.0f;
		tDeathDirection[2] = 0.0f;
	}
	else
	{
		tDeathDirection[0] /= tDeathLength;
		tDeathDirection[2] /= tDeathLength;
	}
	tAVATAR_OBJECT2->mDATA.aAction.aType = 0;
	tAVATAR_OBJECT2->mDATA.aAction.aSort = 12;
	tAVATAR_OBJECT2->mDATA.aAction.aFrame = 0.0f;
	tAVATAR_OBJECT2->mDATA.aAction.aTargetLocation[0] = tDeathDirection[0];
	tAVATAR_OBJECT2->mDATA.aAction.aTargetLocation[1] = 0.0f;
	tAVATAR_OBJECT2->mDATA.aAction.aTargetLocation[2] = tDeathDirection[2];
	tAVATAR_OBJECT2->mDATA.aAction.aFront = mUTIL.GetYAngle( tAVATAR_OBJECT2->mDATA.aAction.aLocation[0], tAVATAR_OBJECT2->mDATA.aAction.aLocation[2], tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], tAVATAR_OBJECT1->mDATA.aAction.aLocation[2] );
	tAVATAR_OBJECT2->mDATA.aAction.aTargetFront = tAVATAR_OBJECT2->mDATA.aAction.aFront;
	tAVATAR_OBJECT2->mDATA.aAction.aSkillNumber = 2;
	mTRANSFER.B_AVATAR_ACTION_RECV( tAttackInfo->mServerIndex2, tAVATAR_OBJECT2->mUniqueNumber, &tAVATAR_OBJECT2->mDATA, 1 );
	mUSER[tAttackInfo->mServerIndex2].Send (TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT2->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, tAttackInfo -> mServerIndex2, false );
	tAVATAR_OBJECT2->ProcessForDeath();
	//------------------------//
	//------------------------//
	//------------------------//
}
//PROCESS_ATTACK_02[아바타->아바타(세력)]
void MyGame::ProcessAttack02( int tUserIndex, ATTACK_FOR_PROTOCOL *tAttackInfo )
{
	unsigned int tRandom = 0;
	if( ( tAttackInfo->mServerIndex1 != tUserIndex ) || ( tAttackInfo->mUniqueNumber1 != mAVATAR_OBJECT[tUserIndex].mUniqueNumber ) )
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			               
		return;
	}
	tAVATAR_OBJECT1 = &mAVATAR_OBJECT[tAttackInfo->mServerIndex1];
	tAVATAR_OBJECT1->mDATA.aAction.aLocation[0] = tAttackInfo->mSenderLocation[0];
	tAVATAR_OBJECT1->mDATA.aAction.aLocation[1] = tAttackInfo->mSenderLocation[1];
	tAVATAR_OBJECT1->mDATA.aAction.aLocation[2] = tAttackInfo->mSenderLocation[2];
	tAVATAR_OBJECT1->SetSpaceIndex();
	if( ( tAttackInfo->mServerIndex2 < 0 ) || ( tAttackInfo->mServerIndex2 > ( mSERVER.mMAX_USER_NUM - 1 ) ) )
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	tAVATAR_OBJECT2 = &mAVATAR_OBJECT[tAttackInfo->mServerIndex2];
	if( ( !tAVATAR_OBJECT2->mCheckValidState ) || ( tAVATAR_OBJECT2->mUniqueNumber != tAttackInfo->mUniqueNumber2 ) )
	{
		return;
	}
	if( tAVATAR_OBJECT1->mDATA.aLifeValue < 1 )
	{
		return;
	}
	if( ( mTickCount - mUSER[tAttackInfo->mServerIndex1].mTickCountFor01SecondForProtect ) < PROTECT_TICK )
	{
		return;
	}
	if( mUSER[tAttackInfo->mServerIndex2].mMoveZoneResult == 1 )
	{
		return;
	}
	if( tAVATAR_OBJECT2->mDATA.aLifeValue < 1 )
	{
		return;
	}
	if( tAVATAR_OBJECT2->mDATA.aPShopState == 1 )
	{
		return;
	}
	if( !CheckPossibleAttackTarget( 1, tAVATAR_OBJECT2->mDATA.aAction.aSort ) )
	{
		return;
	}
	if( ( mTickCount - mUSER[tAttackInfo->mServerIndex2].mTickCountFor01SecondForProtect ) < PROTECT_TICK )
	{
		return;
	}
	if( ( mZONEMAININFO.ReturnZoneTribeInfo2( mSERVER_INFO.mServerNumber ) == 0 ) || ( tAVATAR_OBJECT1->mDATA.aTribe == tAVATAR_OBJECT2->mDATA.aTribe ) || ( tAVATAR_OBJECT2->mDATA.aTribe == ReturnAllianceTribe( tAVATAR_OBJECT1->mDATA.aTribe ) ) )
	{
		return;
	}
	if(1 == tAVATAR_OBJECT2->mGod){
		return;
	}
	if(1 == mUSER[tUserIndex].mAuthInfo.AttackFlag){
		return;
	}
	if(1 == mUSER[tAttackInfo->mServerIndex2].mAuthInfo.AttackFlag){
		return;
	}

	/** 2009. 10. 26 : DarkAttack Potion add (wolf)*/
    if( mUSER[tAttackInfo->mServerIndex1].mEffectValue[15][1] > 0 && mAVATAR_OBJECT[tAttackInfo->mServerIndex2].mDATA.bStateDarkAttack == false )
    {
        tRandom = rand_mir() % 100;

        if( tRandom < mUSER[tAttackInfo->mServerIndex1].mEffectValue[15][0] )
        {
            mAVATAR_OBJECT[tAttackInfo->mServerIndex2].mDATA.bStateDarkAttack = true;
            mUSER[tAttackInfo->mServerIndex2].mTickCountForDarkAttack = GetTickCount();
            mTRANSFER.B_AVATAR_CHANGE_INFO_2( 44, mAVATAR_OBJECT[tAttackInfo->mServerIndex2].mDATA.bStateDarkAttack );
            mUSER[tAttackInfo->mServerIndex2].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
            //LOG_TO_FILE_2("![%s] : %s \n", __FUNCTION__, mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aName);
        }
    }
    /** 2009. 10. 26 : DarkAttack Potion add (wolf)*/

	
	tChargingValue = mUSER[tAttackInfo->mServerIndex1].mEffectValue[8][0];
	if( mUSER[tAttackInfo->mServerIndex1].mEffectValue[8][0] > 0 )
	{
		mUSER[tAttackInfo->mServerIndex1].mEffectValue[8][0] = 0;
		mUSER[tAttackInfo->mServerIndex1].mEffectValue[8][1] = 0;
		tAVATAR_OBJECT1->mDATA.aEffectValueForView[8] = 0;
		mTRANSFER.B_AVATAR_CHANGE_INFO_1( tAttackInfo->mServerIndex1, tAttackInfo->mUniqueNumber1, 2, 0, 0, 0 );
		mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
	}
	switch( tAttackInfo->mAttackActionValue1 )
	{
	case 1 :
		tAttackSuccessValue = tAVATAR_OBJECT1->GetAttackSuccess();
		break;
	case 2 :
		if( mUSER[tUserIndex].mCheckMaxAttackPacketNum == 1 )
		{
			if( ( tAVATAR_OBJECT1->mDATA.aAction.aSkillNumber != tAttackInfo->mAttackActionValue2 ) || ( ( tAVATAR_OBJECT1->mDATA.aAction.aSkillGradeNum1 + tAVATAR_OBJECT1->mDATA.aAction.aSkillGradeNum2 ) != tAttackInfo->mAttackActionValue3 ) )
			{
				return;
			}
		}
		tAttackSuccessValue = tAVATAR_OBJECT1->GetAttackSuccess();
		break;
	default :
		return;
	}
	if( tAttackSuccessValue < 1 ) 
	{
		return;
	}
	tAttackBlockValue = tAVATAR_OBJECT2->GetAttackBlock();
	if( tAttackBlockValue > 0 )
	{
		if( tAttackSuccessValue > tAttackBlockValue )
		{
			tAttackDetermineValue = (int) ( 70.0f + ( ( (float) tAttackSuccessValue / (float) tAttackBlockValue ) - 1.0f ) * 25.0f );
			if( tAttackDetermineValue > 99 )
			{
				tAttackDetermineValue = 99;
			}
		}
		else
		{
			tAttackDetermineValue = (int) ( 70.0f - ( ( (float) tAttackBlockValue / (float) tAttackSuccessValue ) - 1.0f ) * 25.0f );
			if( tAttackDetermineValue < 1 )
			{
				tAttackDetermineValue = 1;
			}
		}
		tRandom = rand_mir();
		if( ( tRandom % 100 ) >= tAttackDetermineValue )
		{
			tAttackInfo->mAttackResultValue = 0;
			mTRANSFER.B_PROCESS_ATTACK_RECV( tAttackInfo );
			mUSER[tAttackInfo->mServerIndex1].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			return;
		}
	}
	switch( tAttackInfo->mAttackActionValue1 )
	{
	case 1 :
		tAttackPowerValue = tAVATAR_OBJECT1->GetAttackPower();
		break;
	case 2 :
		tAttackPowerValue = tAVATAR_OBJECT1->GetAttackPower();
		tAttackPowerUpRatio = mSKILL.ReturnSkillValue( tAttackInfo->mAttackActionValue2, tAttackInfo->mAttackActionValue3, 7 );
		if( tAttackPowerUpRatio > 0.0f )
		{
			tAttackPowerValue = (int) ( ( float) tAttackPowerValue * ( tAttackPowerUpRatio + 100.0f ) * 0.01f );
		}
		break;
	}
    // @ Tribe Formation Skill @
    //--------//
    //맹공진법//
    //--------//
    if( mWorldInfo.mTribeMasterCallAbility[tAVATAR_OBJECT1->mDATA.aTribe] == 1 )
    {
        tAttackPowerValue = (int) ( ( float) tAttackPowerValue * 1.1f );
    }
    //--------//
    //--------//
    //--------//
    // @
	tDefensePowerValue = tAVATAR_OBJECT2->GetDefensePower();
    // @ Tribe Formation Skill @
    //--------//
    //철벽진법//
    //--------//
    if( mWorldInfo.mTribeMasterCallAbility[tAVATAR_OBJECT2->mDATA.aTribe] == 2 )
    {
        tDefensePowerValue = (int) ( ( float) tDefensePowerValue * 1.1f );
    }
    //--------//
    //--------//
    //--------//
    // @
	tDamageValue = tAttackPowerValue - tDefensePowerValue;
	if( tDamageValue < 1 )
	{
		tDamageValue = 1;
	}
	if( tChargingValue > 0 )
	{
		tDamageValue = (int) ( (float) ( tDamageValue * ( tChargingValue + 100 ) ) * 0.01f );
	}
	tRandom = rand_mir();
	if( ( tRandom % 2 ) == 0 )
	{
		tRandom = rand_mir();
		tDamageValue += (int) ( (float) ( tDamageValue * ( tRandom % 11 ) ) * 0.01f );
	}
	else
	{
		tRandom = rand_mir();
		tDamageValue -= (int) ( (float) ( tDamageValue * ( tRandom % 11 ) ) * 0.01f );
	}
	if( tDamageValue < tMinDamageValueWithAvatar )
	{
		tDamageValue = tMinDamageValueWithAvatar;
	}
	tAttackInfo->mAttackCriticalExist = 0;
	switch( tAttackInfo->mAttackActionValue1 )
	{
	case 1 :
        // @ Tribe Formation Skill @
        //-----------------//
        //일격진법_강벽진법//
        //-----------------//
        if( ( mWorldInfo.mTribeMasterCallAbility[tAVATAR_OBJECT1->mDATA.aTribe] != 3 ) && ( mWorldInfo.mTribeMasterCallAbility[tAVATAR_OBJECT2->mDATA.aTribe] != 4 ) )
        {
            tRandom = rand_mir();
            if( ( tRandom % 100 ) < getCriticalAttackDefValue( tAVATAR_OBJECT1, tAVATAR_OBJECT2 ) )
            {
                tDamageValue *= 2;
                tAttackInfo->mAttackCriticalExist = 1;
            }
        }
        else if( ( mWorldInfo.mTribeMasterCallAbility[tAVATAR_OBJECT1->mDATA.aTribe] == 3 ) && ( mWorldInfo.mTribeMasterCallAbility[tAVATAR_OBJECT2->mDATA.aTribe] != 4 ) )
        {
            tRandom = rand_mir();
            if( ( tRandom % 100 ) < (getCriticalAttackDefValue( tAVATAR_OBJECT1, tAVATAR_OBJECT2 ) + 5) )
            {
                tDamageValue *= 2;
                tAttackInfo->mAttackCriticalExist = 1;
            }
        }
        else if( ( mWorldInfo.mTribeMasterCallAbility[tAVATAR_OBJECT1->mDATA.aTribe] != 3 ) && ( mWorldInfo.mTribeMasterCallAbility[tAVATAR_OBJECT2->mDATA.aTribe] == 4 ) )
        {
            tRandom = rand_mir();
            if( ( tRandom % 100 ) < (getCriticalAttackDefValue( tAVATAR_OBJECT1, tAVATAR_OBJECT2 ) - 5) )
            {
                tDamageValue *= 2;
                tAttackInfo->mAttackCriticalExist = 1;
            }
        }
        else if( ( mWorldInfo.mTribeMasterCallAbility[tAVATAR_OBJECT1->mDATA.aTribe] == 3 ) && ( mWorldInfo.mTribeMasterCallAbility[tAVATAR_OBJECT2->mDATA.aTribe] == 4 ) )
        {
            tRandom = rand_mir();
            if( ( tRandom % 100 ) < getCriticalAttackDefValue( tAVATAR_OBJECT1, tAVATAR_OBJECT2 ) )
            {
                tDamageValue *= 2;
                tAttackInfo->mAttackCriticalExist = 1;
            }
        }
        else;
        //-----------------//
        //-----------------//
        //-----------------//
        // @
		break;
	case 2 :
		if( tAttackInfo->mAttackActionValue2 == 78 )
		{
			break;
		}
		switch( mSKILL.ReturnAttackType( tAttackInfo->mAttackActionValue2 ) )
		{
		case 2 :
		case 5 :
            // @ Tribe Formation Skill @
            //-----------------//
            //일격진법_강벽진법//
            //-----------------//
            if( ( mWorldInfo.mTribeMasterCallAbility[tAVATAR_OBJECT1->mDATA.aTribe] != 3 ) && ( mWorldInfo.mTribeMasterCallAbility[tAVATAR_OBJECT2->mDATA.aTribe] != 4 ) )
            {
                tRandom = rand_mir();
                if( ( tRandom % 100 ) < getCriticalAttackDefValue( tAVATAR_OBJECT1, tAVATAR_OBJECT2 ) )
                {
                    tDamageValue *= 2;
                    tAttackInfo->mAttackCriticalExist = 1;
                }
            }
            else if( ( mWorldInfo.mTribeMasterCallAbility[tAVATAR_OBJECT1->mDATA.aTribe] == 3 ) && ( mWorldInfo.mTribeMasterCallAbility[tAVATAR_OBJECT2->mDATA.aTribe] != 4 ) )
            {
                tRandom = rand_mir();
                if( ( tRandom % 100 ) < (getCriticalAttackDefValue( tAVATAR_OBJECT1, tAVATAR_OBJECT2 ) + 5) )
                {
                    tDamageValue *= 2;
                    tAttackInfo->mAttackCriticalExist = 1;
                }
            }
            else if( ( mWorldInfo.mTribeMasterCallAbility[tAVATAR_OBJECT1->mDATA.aTribe] != 3 ) && ( mWorldInfo.mTribeMasterCallAbility[tAVATAR_OBJECT2->mDATA.aTribe] == 4 ) )
            {
                tRandom = rand_mir();
                if( ( tRandom % 100 ) < (getCriticalAttackDefValue( tAVATAR_OBJECT1, tAVATAR_OBJECT2 ) - 5) )
                {
                    tDamageValue *= 2;
                    tAttackInfo->mAttackCriticalExist = 1;
                }
            }
            else if( ( mWorldInfo.mTribeMasterCallAbility[tAVATAR_OBJECT1->mDATA.aTribe] == 3 ) && ( mWorldInfo.mTribeMasterCallAbility[tAVATAR_OBJECT2->mDATA.aTribe] == 4 ) )
            {
                tRandom = rand_mir();
                if( ( tRandom % 100 ) < getCriticalAttackDefValue( tAVATAR_OBJECT1, tAVATAR_OBJECT2 ) )
                {
                    tDamageValue *= 2;
                    tAttackInfo->mAttackCriticalExist = 1;
                }
            }
            else;
            //-----------------//
            //-----------------//
            //-----------------//
            // @
			break;
		}
		break;
	}
	tDamageValue /= tMinDamageValueWithAvatar;
	if( mUSER[tAttackInfo->mServerIndex1].mEffectValue[14][0] > 0 )
	{
		tRandom = rand_mir();
		if( ( tRandom % 1000 ) < mUSER[tAttackInfo->mServerIndex1].mEffectValue[14][0] )
		{
			mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][0] = 0;
			mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][1] = 0;
			tAVATAR_OBJECT2->mDATA.aEffectValueForView[9] = 0;
			mTRANSFER.B_AVATAR_CHANGE_INFO_1( tAttackInfo->mServerIndex2, tAttackInfo->mUniqueNumber2, 3, 0, 0, 0 );
            mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT2->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
		}
	}
	if( mUSER[tAttackInfo->mServerIndex2].mEffectValue[12][0] > 0 )
	{
		tReturnDamageProbability = mUSER[tAttackInfo->mServerIndex2].mEffectValue[12][0];
		if( tAVATAR_OBJECT1->mDATA.aLevel1 > tAVATAR_OBJECT2->mDATA.aLevel1 )
		{
			tReturnDamageProbability -= ( ( tAVATAR_OBJECT1->mDATA.aLevel1 - tAVATAR_OBJECT2->mDATA.aLevel1 ) * 3 );
		}
		tRandom = rand_mir();
		if(tReturnDamageProbability < 0) {
			tReturnDamageProbability = 0;
		}
		if( ( tRandom % 1000 ) < tReturnDamageProbability )
		{
			tRandom = rand_mir();
			tReturnDamageValue = (int) ( (float) ( tDamageValue * ( 100 + tRandom % 51 ) ) * 0.01f );
			mTRANSFER.B_AVATAR_CHANGE_INFO_1( tAttackInfo->mServerIndex1, tAttackInfo->mUniqueNumber1, 4, tReturnDamageValue, 0, 0 );
            mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
			tAVATAR_OBJECT1->mDATA.aLifeValue -= tReturnDamageValue;
			if( tAVATAR_OBJECT1->mDATA.aLifeValue < 1 )
			{
				tAVATAR_OBJECT1->mDATA.aLifeValue = 0;
				//------------------------//
				//PROCESS_FOR_AVATAR_DEATH//
				//------------------------//
				tDeathDirection[0] = tAVATAR_OBJECT1->mDATA.aAction.aLocation[0] - tAVATAR_OBJECT2->mDATA.aAction.aLocation[0];
				tDeathDirection[2] = tAVATAR_OBJECT1->mDATA.aAction.aLocation[2] - tAVATAR_OBJECT2->mDATA.aAction.aLocation[2];
				tDeathLength = sqrtf( tDeathDirection[0] * tDeathDirection[0] + tDeathDirection[2] * tDeathDirection[2] );
				if( tDeathLength < 1.0f )
				{
					tDeathDirection[0] = 0.0f;
					tDeathDirection[2] = 0.0f;
				}
				else
				{
					tDeathDirection[0] /= tDeathLength;
					tDeathDirection[2] /= tDeathLength;
				}
				tAVATAR_OBJECT1->mDATA.aAction.aType = 0;
				tAVATAR_OBJECT1->mDATA.aAction.aSort = 12;
				tAVATAR_OBJECT1->mDATA.aAction.aFrame = 0.0f;
				tAVATAR_OBJECT1->mDATA.aAction.aTargetLocation[0] = tDeathDirection[0];
				tAVATAR_OBJECT1->mDATA.aAction.aTargetLocation[1] = 0.0f;
				tAVATAR_OBJECT1->mDATA.aAction.aTargetLocation[2] = tDeathDirection[2];
				tAVATAR_OBJECT1->mDATA.aAction.aFront = mUTIL.GetYAngle( tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], tAVATAR_OBJECT1->mDATA.aAction.aLocation[2], tAVATAR_OBJECT2->mDATA.aAction.aLocation[0], tAVATAR_OBJECT2->mDATA.aAction.aLocation[2] );
				tAVATAR_OBJECT1->mDATA.aAction.aTargetFront = tAVATAR_OBJECT1->mDATA.aAction.aFront;
				tAVATAR_OBJECT1->mDATA.aAction.aSkillNumber = 3;
				mTRANSFER.B_AVATAR_ACTION_RECV( tAttackInfo->mServerIndex1, tAVATAR_OBJECT1->mUniqueNumber, &tAVATAR_OBJECT1->mDATA, 1 );
				mUSER[tAttackInfo->mServerIndex1].Send (TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
                mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1 , tAttackInfo->mServerIndex1, false);
				tAVATAR_OBJECT1->ProcessForDeath();
				//------------------------//
				//------------------------//
				//------------------------//
				mUTIL.ProcessForKillOtherTribe( tAttackInfo->mServerIndex2, tAttackInfo->mServerIndex1, mSERVER_INFO.mServerNumber );
			}
			return;
		}
	}
	if( mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][0] > 0 )
	{
		if( tDamageValue > mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][0] )
		{
			tShieldDamageValue = mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][0];
		}
		else
		{
			tShieldDamageValue = tDamageValue;
		}
		mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][0] -= tShieldDamageValue;
		if( mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][0] < 1 )
		{
			mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][0] = 0;
			mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][1] = 0;
		}
		tAVATAR_OBJECT2->mDATA.aEffectValueForView[9] -= tShieldDamageValue;
		if( tAVATAR_OBJECT2->mDATA.aEffectValueForView[9] < 1 )
		{
			tAVATAR_OBJECT2->mDATA.aEffectValueForView[9] = 0;
		}
		mTRANSFER.B_AVATAR_CHANGE_INFO_1( tAttackInfo->mServerIndex2, tAttackInfo->mUniqueNumber2, 5, tShieldDamageValue, 0, 0 );
        mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT2->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
		tDamageValue -= tShieldDamageValue;
	}
	tElementAttackPower = tAVATAR_OBJECT1->GetElementAttackPower();
	switch( tAttackInfo->mAttackActionValue1 )
	{
	case 3 :
		tElementAttackPowerUpRatio = mSKILL.ReturnSkillValue( tAttackInfo->mAttackActionValue2, tAttackInfo->mAttackActionValue3, 8 );
		if( tElementAttackPowerUpRatio > 0.0f )
		{
			tElementAttackPower = (int) ( ( float) tElementAttackPower * ( tElementAttackPowerUpRatio + 100.0f ) * 0.01f );
		}
		break;
	}
	tElementDefensePower = tAVATAR_OBJECT2->GetElementDefensePower();
	if( tElementAttackPower > tElementDefensePower )
	{
		tDamageValue += ( tElementAttackPower - tElementDefensePower );
		tAttackInfo->mAttackElementDamage = tElementAttackPower - tElementDefensePower;
	}
	else
	{
		tAttackInfo->mAttackElementDamage = 0;
	}
	tAttackInfo->mAttackResultValue = 1 + mUSER[tAttackInfo->mServerIndex1].mAvatarInfo.aEquip[7][0];
	tAttackInfo->mAttackViewDamageValue = tDamageValue;
	if( tAVATAR_OBJECT2->mDATA.aLifeValue < tDamageValue )
	{
		tDamageValue = tAVATAR_OBJECT2->mDATA.aLifeValue;
	}
	tAttackInfo->mAttackRealDamageValue = tDamageValue;
	mTRANSFER.B_PROCESS_ATTACK_RECV( tAttackInfo );
    mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
	tAVATAR_OBJECT2->mDATA.aLifeValue -= tDamageValue;
	if( tAVATAR_OBJECT2->mDATA.aLifeValue > 0 )
	{
		return;
	}
	//------------------------//
	//PROCESS_FOR_AVATAR_DEATH//
	//------------------------//
	tDeathDirection[0] = tAVATAR_OBJECT2->mDATA.aAction.aLocation[0] - tAVATAR_OBJECT1->mDATA.aAction.aLocation[0];
	tDeathDirection[2] = tAVATAR_OBJECT2->mDATA.aAction.aLocation[2] - tAVATAR_OBJECT1->mDATA.aAction.aLocation[2];
	tDeathLength = sqrtf( tDeathDirection[0] * tDeathDirection[0] + tDeathDirection[2] * tDeathDirection[2] );
	if( tDeathLength < 1.0f )
	{
		tDeathDirection[0] = 0.0f;
		tDeathDirection[2] = 0.0f;
	}
	else
	{
		tDeathDirection[0] /= tDeathLength;
		tDeathDirection[2] /= tDeathLength;
	}
	tAVATAR_OBJECT2->mDATA.aAction.aType = 0;
	tAVATAR_OBJECT2->mDATA.aAction.aSort = 12;
	tAVATAR_OBJECT2->mDATA.aAction.aFrame = 0.0f;
	tAVATAR_OBJECT2->mDATA.aAction.aTargetLocation[0] = tDeathDirection[0];
	tAVATAR_OBJECT2->mDATA.aAction.aTargetLocation[1] = 0.0f;
	tAVATAR_OBJECT2->mDATA.aAction.aTargetLocation[2] = tDeathDirection[2];
	tAVATAR_OBJECT2->mDATA.aAction.aFront = mUTIL.GetYAngle( tAVATAR_OBJECT2->mDATA.aAction.aLocation[0], tAVATAR_OBJECT2->mDATA.aAction.aLocation[2], tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], tAVATAR_OBJECT1->mDATA.aAction.aLocation[2] );
	tAVATAR_OBJECT2->mDATA.aAction.aTargetFront = tAVATAR_OBJECT2->mDATA.aAction.aFront;
	tAVATAR_OBJECT2->mDATA.aAction.aSkillNumber = 4;
	mTRANSFER.B_AVATAR_ACTION_RECV( tAttackInfo->mServerIndex2, tAVATAR_OBJECT2->mUniqueNumber, &tAVATAR_OBJECT2->mDATA, 1 );
	mUSER[tAttackInfo->mServerIndex2].Send (TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
    mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT2->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1,tAttackInfo->mServerIndex2, false );
	tAVATAR_OBJECT2->ProcessForDeath();
	//------------------------//
	//------------------------//
	//------------------------//
	mUTIL.ProcessForKillOtherTribe( tAttackInfo->mServerIndex1, tAttackInfo->mServerIndex2, mSERVER_INFO.mServerNumber );
}
//PROCESS_ATTACK_03[아바타->몬스터]
void MyGame::ProcessAttack03( int tUserIndex, ATTACK_FOR_PROTOCOL *tAttackInfo )
{
	//-----------------------------------------------------------------//
	//[01].일반,[02].성석,[03].수호비,[04].동맹석,[05].수호석,[06].석차//
	//-----------------------------------------------------------------//

	int index01;
	int tBroadcastInfoSort;
	BYTE tBroadcastInfoData[MAX_BROADCAST_INFO_SIZE];
	int tAllianceTribe;
	int tTribeSymbolIndex;
	int tAllianceStoneIndex;
	int tWhoAttackMonster;
	int tSelectAvatarIndex;
	int tGainValue;
	int tTotalPartyNum, iTribe = 0;;
	int tPartyUserIndex[MAX_PARTY_AVATAR_NUM];
	int tBonusGainGeneralExperience;
	int tPatExperience;
	unsigned int tRandom = 0;

	if( ( tAttackInfo->mServerIndex1 != tUserIndex ) || ( tAttackInfo->mUniqueNumber1 != mAVATAR_OBJECT[tUserIndex].mUniqueNumber ) )
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			               
		return;
	}
	tAVATAR_OBJECT1 = &mAVATAR_OBJECT[tAttackInfo->mServerIndex1];
	tAVATAR_OBJECT1->mDATA.aAction.aLocation[0] = tAttackInfo->mSenderLocation[0];
	tAVATAR_OBJECT1->mDATA.aAction.aLocation[1] = tAttackInfo->mSenderLocation[1];
	tAVATAR_OBJECT1->mDATA.aAction.aLocation[2] = tAttackInfo->mSenderLocation[2];
	tAVATAR_OBJECT1->SetSpaceIndex();
	if( ( tAttackInfo->mServerIndex2 < 0 ) || ( tAttackInfo->mServerIndex2 > ( MAX_MONSTER_OBJECT_NUM - 1 ) ) )
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if(1 == mUSER[tUserIndex].mAuthInfo.AttackFlag){
		return;
	}
	tMONSTER_OBJECT = &mMONSTER_OBJECT[tAttackInfo->mServerIndex2];
	if( ( !tMONSTER_OBJECT->mCheckValidState ) || ( tMONSTER_OBJECT->mUniqueNumber != tAttackInfo->mUniqueNumber2 ) )
	{
		return;
	}
	//------
	//Event monster
	if(890 == tMONSTER_OBJECT->mMONSTER_INFO->mIndex){
		if( tAVATAR_OBJECT1->mDATA.aLevel1 > 112 )
			return;
	}
	if(891 == tMONSTER_OBJECT->mMONSTER_INFO->mIndex){
		if( tAVATAR_OBJECT1->mDATA.aLevel1 < 113 )
			return;
	}
	if(1052 == tMONSTER_OBJECT->mMONSTER_INFO->mIndex){
		if( tAVATAR_OBJECT1->mDATA.aLevel1 >= 15 )
			return;
	}
	if(1053 == tMONSTER_OBJECT->mMONSTER_INFO->mIndex){
		if( tAVATAR_OBJECT1->mDATA.aLevel1 >= 25 )
			return;
	}
	//------
	if( tAVATAR_OBJECT1->mDATA.aLifeValue < 1 )
	{
		return;
	}
	/* Mir */
	if( ( mTickCount - mUSER[tAttackInfo->mServerIndex1].mTickCountFor01SecondForProtect ) < PROTECT_TICK )
	{
		return;
	}
	/* Mir */
	if( tMONSTER_OBJECT->mDATA.mLifeValue < 1 )
	{
		return;
	}
	if( !CheckPossibleAttackTarget( 2, tMONSTER_OBJECT->mDATA.mAction.aSort ) )
	{
		return;
	}
    if((::strlen(tMONSTER_OBJECT->mAvatarName) > 0) && (::strncmp(tMONSTER_OBJECT->mAvatarName, mUSER[tUserIndex].mAvatarInfo.aName, MAX_AVATAR_NAME_LENGTH) != 0))
    {
        // 몬스터 소유자가 지정되어 있으면 소유자만 타격 가능.
        return;
    }

    // # Defense Hack # {
    if((mUSER[tUserIndex].mUniqueNumber != tAttackInfo->mUniqueNumber3)) {
        if(mUSER[tUserIndex].mUniqueNumber == 0) {
            mUSER[tUserIndex].mUniqueNumber = tAttackInfo->mUniqueNumber3;
            mUSER[tUserIndex].mAttackTickCount = GetTickCount();
        } else {
            if((GetTickCount() - mUSER[tUserIndex].mAttackTickCount) < 150) {
                mUSER[tUserIndex].mAutoCheckCount++;
                //if(mUSER[tUserIndex].mAutoCheckCount > 9) {
                mGAMELOG.GL_659_AUTO_CHECK(tUserIndex, 5, (float)(GetTickCount() - mUSER[tUserIndex].mAttackTickCount)
                        , mUSER[tUserIndex].mUniqueNumber, tAttackInfo->mUniqueNumber3);
                return;
                //mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
                //}
                mUSER[tUserIndex].mUniqueNumber = tAttackInfo->mUniqueNumber3;
                mUSER[tUserIndex].mAttackTickCount = GetTickCount();
                return;
            }
            mUSER[tUserIndex].mUniqueNumber = tAttackInfo->mUniqueNumber3;
            mUSER[tUserIndex].mAttackTickCount = GetTickCount();
        }
    } else {
        if((GetTickCount() - mUSER[tUserIndex].mAttackTickCount) > 2000) {
            mUSER[tUserIndex].mAutoCheckCount++;
            if(mUSER[tUserIndex].mAutoCheckCount > 21) {
                mGAMELOG.GL_659_AUTO_CHECK(tUserIndex, 6, (GetTickCount() - mUSER[tUserIndex].mAttackTickCount)
                        , mUSER[tUserIndex].mUniqueNumber, tAttackInfo->mUniqueNumber3);
                mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
            }
            return;
        }
    }
    mUSER[tUserIndex].mAutoCheckCount = 0;

    //LOG_TO_FILE_3("tAttackInfo->mUniqueNumber1 : %d %d %d\n", tAttackInfo->mUniqueNumber1, tAttackInfo->mUniqueNumber3, GetTickCount());

    float fHeight = ::abs((int)(tAVATAR_OBJECT1->mDATA.aAction.aLocation[1] - tMONSTER_OBJECT->mDATA.mAction.aLocation[1]));
    if(fHeight > 40) {
        mGAMELOG.GL_659_AUTO_CHECK(tUserIndex, 1, fHeight);
        return;
    }

    //if(!mUTIL.CheckRAttackRange(tUserIndex, tAVATAR_OBJECT1->mDATA.aAction.aSkillNumber, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0]
    //LOG_TO_FILE_1("Monster Size : %3.3f\n", tMONSTER_OBJECT->mRadiusForSize); // ..... .... .. ..
#if 0
    if(!mUTIL.CheckRAttackRange(tUserIndex, tAttackInfo->mAttackActionValue2, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0]
                , &tMONSTER_OBJECT->mDATA.mAction.aLocation[0], 0.0f)) {
        return;
    }
#endif
    // # }
	if(!mUTIL.CheckRAttackRange(tUserIndex, tAttackInfo->mAttackActionValue2, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0]
                , &tMONSTER_OBJECT->mDATA.mAction.aLocation[0], tMONSTER_OBJECT->mRadiusForSize )) { // 0.0f)) {
        return;
    }
	tChargingValue = mUSER[tAttackInfo->mServerIndex1].mEffectValue[8][0];
	if( mUSER[tAttackInfo->mServerIndex1].mEffectValue[8][0] > 0 )
	{
		mUSER[tAttackInfo->mServerIndex1].mEffectValue[8][0] = 0;
		mUSER[tAttackInfo->mServerIndex1].mEffectValue[8][1] = 0;
		tAVATAR_OBJECT1->mDATA.aEffectValueForView[8] = 0;
		mTRANSFER.B_AVATAR_CHANGE_INFO_1( tAttackInfo->mServerIndex1, tAttackInfo->mUniqueNumber1, 2, 0, 0, 0 );
		mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
	}
	//-------------//
	//공격허용_검사//
	//-------------//
	switch( tMONSTER_OBJECT->mSpecialSortNumber )
	{
	case 1 :
		break;
	case 2 :
		tAllianceTribe = ReturnAllianceTribe( tAVATAR_OBJECT1->mDATA.aTribe );
		switch( tMONSTER_OBJECT->mMONSTER_INFO->mSpecialType )
		{
		case 11 :
			if( ( mWorldInfo.mTribeSymbolBattle == 0 ) || ( mWorldInfo.mTribe1Symbol == tAVATAR_OBJECT1->mDATA.aTribe ) || ( mWorldInfo.mTribe1Symbol == tAllianceTribe ) )
			{
				return;
			}
			if( !tMONSTER_OBJECT->mCheckFirstAttackForTribeSymbol )
			{
				tMONSTER_OBJECT->mCheckFirstAttackForTribeSymbol = TRUE;
				tMONSTER_OBJECT->mFirstAttackTimeForTribeSymbol = GetTickCount();
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tTribeSymbolIndex = 0;
				tBroadcastInfoSort = 41;
				CopyMemory( &tBroadcastInfoData[0], &tTribeSymbolIndex, 4 );
				iTribe = static_cast<int> (tAVATAR_OBJECT1->mDATA.aTribe);
				CopyMemory( &tBroadcastInfoData[4], &iTribe, 4 );
				CopyMemory( &tBroadcastInfoData[8], &tAVATAR_OBJECT1->mDATA.aName[0], MAX_AVATAR_NAME_LENGTH );
				mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
				//--------------//
				//--------------//
				//--------------//
			}
			break;
		case 12 :
			if( ( mWorldInfo.mTribeSymbolBattle == 0 ) || ( mWorldInfo.mTribe2Symbol == tAVATAR_OBJECT1->mDATA.aTribe ) || ( mWorldInfo.mTribe2Symbol == tAllianceTribe ) )
			{
				return;
			}
			if( !tMONSTER_OBJECT->mCheckFirstAttackForTribeSymbol )
			{
				tMONSTER_OBJECT->mCheckFirstAttackForTribeSymbol = TRUE;
				tMONSTER_OBJECT->mFirstAttackTimeForTribeSymbol = GetTickCount();
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tTribeSymbolIndex = 1;
				tBroadcastInfoSort = 41;
				CopyMemory( &tBroadcastInfoData[0], &tTribeSymbolIndex, 4 );
				iTribe = static_cast<int> (tAVATAR_OBJECT1->mDATA.aTribe);
				CopyMemory( &tBroadcastInfoData[4], &iTribe, 4 );
				CopyMemory( &tBroadcastInfoData[8], &tAVATAR_OBJECT1->mDATA.aName[0], MAX_AVATAR_NAME_LENGTH );
				mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
				//--------------//
				//--------------//
				//--------------//
			}
			break;
		case 13 :
			if( ( mWorldInfo.mTribeSymbolBattle == 0 ) || ( mWorldInfo.mTribe3Symbol == tAVATAR_OBJECT1->mDATA.aTribe ) || ( mWorldInfo.mTribe3Symbol == tAllianceTribe ) )
			{
				return;
			}
			if( !tMONSTER_OBJECT->mCheckFirstAttackForTribeSymbol )
			{
				tMONSTER_OBJECT->mCheckFirstAttackForTribeSymbol = TRUE;
				tMONSTER_OBJECT->mFirstAttackTimeForTribeSymbol = GetTickCount();
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tTribeSymbolIndex = 2;
				tBroadcastInfoSort = 41;
				CopyMemory( &tBroadcastInfoData[0], &tTribeSymbolIndex, 4 );
				iTribe = static_cast<int> (tAVATAR_OBJECT1->mDATA.aTribe);
				CopyMemory( &tBroadcastInfoData[4], &iTribe, 4 );
				CopyMemory( &tBroadcastInfoData[8], &tAVATAR_OBJECT1->mDATA.aName[0], MAX_AVATAR_NAME_LENGTH );
				mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
				//--------------//
				//--------------//
				//--------------//
			}
			break;
		case 28 :
			if( ( mWorldInfo.mTribeSymbolBattle == 0 ) || ( mWorldInfo.mTribe4Symbol == tAVATAR_OBJECT1->mDATA.aTribe ) || ( mWorldInfo.mTribe4Symbol == tAllianceTribe ) )
			{
				return;
			}
			if( !tMONSTER_OBJECT->mCheckFirstAttackForTribeSymbol )
			{
				tMONSTER_OBJECT->mCheckFirstAttackForTribeSymbol = TRUE;
				tMONSTER_OBJECT->mFirstAttackTimeForTribeSymbol = GetTickCount();
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tTribeSymbolIndex = 3;
				tBroadcastInfoSort = 41;
				CopyMemory( &tBroadcastInfoData[0], &tTribeSymbolIndex, 4 );
				iTribe = static_cast<int> (tAVATAR_OBJECT1->mDATA.aTribe);
				CopyMemory( &tBroadcastInfoData[4], &iTribe, 4 );
				CopyMemory( &tBroadcastInfoData[8], &tAVATAR_OBJECT1->mDATA.aName[0], MAX_AVATAR_NAME_LENGTH );
				mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
				//--------------//
				//--------------//
				//--------------//
			}
			break;
		case 14 :
			if( mWorldInfo.mMonsterSymbol == tAVATAR_OBJECT1->mDATA.aTribe )
			{
				return;
			}
			if( tAllianceTribe != -1 )
			{
				if( mWorldInfo.mMonsterSymbol == tAllianceTribe )
				{
					return;
				}
			}
			/** 2009. 10. 27 : Monster Symbol edit (wolf) */
            if( !bAttackMonsterSymbol )
            {
                return;
            }
            /** 2009. 10. 27 : Monster Symbol edit (wolf) */
			if( !tMONSTER_OBJECT->mCheckFirstAttackForTribeSymbol )
			{
				tMONSTER_OBJECT->mCheckFirstAttackForTribeSymbol = TRUE;
				tMONSTER_OBJECT->mFirstAttackTimeForTribeSymbol = GetTickCount();
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tTribeSymbolIndex = 4;
				tBroadcastInfoSort = 41;
				CopyMemory( &tBroadcastInfoData[0], &tTribeSymbolIndex, 4 );
				iTribe = static_cast<int> (tAVATAR_OBJECT1->mDATA.aTribe);
				CopyMemory( &tBroadcastInfoData[4], &iTribe, 4 );
				CopyMemory( &tBroadcastInfoData[8], &tAVATAR_OBJECT1->mDATA.aName[0], MAX_AVATAR_NAME_LENGTH );
				mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
				//--------------//
				//--------------//
				//--------------//
			}
			break;
		case 15 :
			break;
		}
		break;
	case 3 :
        switch( tMONSTER_OBJECT->mMONSTER_INFO->mSpecialType )
		{
		case 21 :
			if( ( tAVATAR_OBJECT1->mDATA.aTribe == 0 ) || ( tAVATAR_OBJECT1->mDATA.aTribe == ReturnAllianceTribe( 0 ) ) )
			{
				return;
			}
			break;
		case 22 :
			if( ( tAVATAR_OBJECT1->mDATA.aTribe == 1 ) || ( tAVATAR_OBJECT1->mDATA.aTribe == ReturnAllianceTribe( 1 ) ) )
			{
				return;
			}
			break;
		case 23 :
			if( ( tAVATAR_OBJECT1->mDATA.aTribe == 2 ) || ( tAVATAR_OBJECT1->mDATA.aTribe == ReturnAllianceTribe( 2 ) ) )
			{
				return;
			}
			break;
		case 29 :
			if( ( tAVATAR_OBJECT1->mDATA.aTribe == 3 ) || ( tAVATAR_OBJECT1->mDATA.aTribe == ReturnAllianceTribe( 3 ) ) )
			{
				return;
			}
			break;
		}
		break;
	case 4 :
		switch( tMONSTER_OBJECT->mMONSTER_INFO->mSpecialType )
		{
		case 31 :
			tAllianceTribe = ReturnAllianceTribe( 0 );
			if( ( tAVATAR_OBJECT1->mDATA.aTribe == 0 ) || ( tAVATAR_OBJECT1->mDATA.aTribe == tAllianceTribe ) )
			{
				return;
			}
			if( !tMONSTER_OBJECT->mCheckFirstAttackForAllianceStone )
			{
				tMONSTER_OBJECT->mCheckFirstAttackForAllianceStone = TRUE;
				tMONSTER_OBJECT->mFirstAttackTimeForAllianceStone = GetTickCount();
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tAllianceStoneIndex = 0;
				tBroadcastInfoSort = 48;
				CopyMemory( &tBroadcastInfoData[0], &tAllianceStoneIndex, 4 );
				iTribe = static_cast<int> (tAVATAR_OBJECT1->mDATA.aTribe);
				CopyMemory( &tBroadcastInfoData[4], &iTribe, 4 );
				CopyMemory( &tBroadcastInfoData[8], &tAVATAR_OBJECT1->mDATA.aName[0], MAX_AVATAR_NAME_LENGTH );
				mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
				//--------------//
				//--------------//
				//--------------//
			}
			break;
		case 32 :
			tAllianceTribe = ReturnAllianceTribe( 1 );
			if( ( tAVATAR_OBJECT1->mDATA.aTribe == 1 ) || ( tAVATAR_OBJECT1->mDATA.aTribe == tAllianceTribe ) )
			{
				return;
			}
			if( !tMONSTER_OBJECT->mCheckFirstAttackForAllianceStone )
			{
				tMONSTER_OBJECT->mCheckFirstAttackForAllianceStone = TRUE;
				tMONSTER_OBJECT->mFirstAttackTimeForAllianceStone = GetTickCount();
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tAllianceStoneIndex = 1;
				tBroadcastInfoSort = 48;
				CopyMemory( &tBroadcastInfoData[0], &tAllianceStoneIndex, 4 );
				iTribe = static_cast<int> (tAVATAR_OBJECT1->mDATA.aTribe);
				CopyMemory( &tBroadcastInfoData[4], &iTribe, 4 );
				CopyMemory( &tBroadcastInfoData[8], &tAVATAR_OBJECT1->mDATA.aName[0], MAX_AVATAR_NAME_LENGTH );
				mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
				//--------------//
				//--------------//
				//--------------//
			}
			break;
		case 33 :
			tAllianceTribe = ReturnAllianceTribe( 2 );
			if( ( tAVATAR_OBJECT1->mDATA.aTribe == 2 ) || ( tAVATAR_OBJECT1->mDATA.aTribe == tAllianceTribe ) )
			{
				return;
			}
			if( !tMONSTER_OBJECT->mCheckFirstAttackForAllianceStone )
			{
				tMONSTER_OBJECT->mCheckFirstAttackForAllianceStone = TRUE;
				tMONSTER_OBJECT->mFirstAttackTimeForAllianceStone = GetTickCount();
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tAllianceStoneIndex = 2;
				tBroadcastInfoSort = 48;
				CopyMemory( &tBroadcastInfoData[0], &tAllianceStoneIndex, 4 );
				iTribe = static_cast<int> (tAVATAR_OBJECT1->mDATA.aTribe);
				CopyMemory( &tBroadcastInfoData[4], &iTribe, 4 );
				CopyMemory( &tBroadcastInfoData[8], &tAVATAR_OBJECT1->mDATA.aName[0], MAX_AVATAR_NAME_LENGTH );
				mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
				//--------------//
				//--------------//
				//--------------//
			}
			break;
		case 34 :
			tAllianceTribe = ReturnAllianceTribe( 3 );
			if( ( tAVATAR_OBJECT1->mDATA.aTribe == 3 ) || ( tAVATAR_OBJECT1->mDATA.aTribe == tAllianceTribe ) )
			{
				return;
			}
			if( !tMONSTER_OBJECT->mCheckFirstAttackForAllianceStone )
			{
				tMONSTER_OBJECT->mCheckFirstAttackForAllianceStone = TRUE;
				tMONSTER_OBJECT->mFirstAttackTimeForAllianceStone = GetTickCount();
				//--------------//
				//BROADCAST_INFO//
				//--------------//
				tAllianceStoneIndex = 3;
				tBroadcastInfoSort = 48;
				CopyMemory( &tBroadcastInfoData[0], &tAllianceStoneIndex, 4 );
				iTribe = static_cast<int> (tAVATAR_OBJECT1->mDATA.aTribe);
				CopyMemory( &tBroadcastInfoData[4], &iTribe, 4 );
				CopyMemory( &tBroadcastInfoData[8], &tAVATAR_OBJECT1->mDATA.aName[0], MAX_AVATAR_NAME_LENGTH );
				mCENTER_COM.U_DEMAND_BROADCAST_INFO( tBroadcastInfoSort, &tBroadcastInfoData[0] );
				//--------------//
				//--------------//
				//--------------//
			}
			break;
		}
		break;
	case 5 :
		tAllianceTribe = ReturnAllianceTribe( tAVATAR_OBJECT1->mDATA.aTribe );
		switch( tMONSTER_OBJECT->mMONSTER_INFO->mType )
		{
		case 6 :
			tAllianceTribe = ReturnAllianceTribe( 0 );
			if( ( tAVATAR_OBJECT1->mDATA.aTribe == 0 ) || ( tAVATAR_OBJECT1->mDATA.aTribe == tAllianceTribe ) )
			{
				return;
			}
			break;
		case 7 :
			tAllianceTribe = ReturnAllianceTribe( 1 );
			if( ( tAVATAR_OBJECT1->mDATA.aTribe == 1 ) || ( tAVATAR_OBJECT1->mDATA.aTribe == tAllianceTribe ) )
			{
				return;
			}
			break;
		case 8 :
			tAllianceTribe = ReturnAllianceTribe( 2 );
			if( ( tAVATAR_OBJECT1->mDATA.aTribe == 2 ) || ( tAVATAR_OBJECT1->mDATA.aTribe == tAllianceTribe ) )
			{
				return;
			}
			break;
		case 9 :
			tAllianceTribe = ReturnAllianceTribe( 3 );
			if( ( tAVATAR_OBJECT1->mDATA.aTribe == 3 ) || ( tAVATAR_OBJECT1->mDATA.aTribe == tAllianceTribe ) )
			{
				return;
			}
			break;
		}
		break;
	case 6 :
		switch( tMONSTER_OBJECT->mMONSTER_INFO->mSpecialType )
		{
		case 35 :
			tAllianceTribe = ReturnAllianceTribe( 0 );
			if( ( tAVATAR_OBJECT1->mDATA.aTribe == 0 ) || ( tAVATAR_OBJECT1->mDATA.aTribe == tAllianceTribe ) )
			{
				return;
			}
			break;
		case 36 :
			tAllianceTribe = ReturnAllianceTribe( 1 );
			if( ( tAVATAR_OBJECT1->mDATA.aTribe == 1 ) || ( tAVATAR_OBJECT1->mDATA.aTribe == tAllianceTribe ) )
			{
				return;
			}
			break;
		case 37 :
			tAllianceTribe = ReturnAllianceTribe( 2 );
			if( ( tAVATAR_OBJECT1->mDATA.aTribe == 2 ) || ( tAVATAR_OBJECT1->mDATA.aTribe == tAllianceTribe ) )
			{
				return;
			}
			break;
		case 38 :
			tAllianceTribe = ReturnAllianceTribe( 3 );
			if( ( tAVATAR_OBJECT1->mDATA.aTribe == 3 ) || ( tAVATAR_OBJECT1->mDATA.aTribe == tAllianceTribe ) )
			{
				return;
			}
			break;
		case 18 :
			break;
		}
		break;
	default :
		return;
	}
	//-------------//
	//-------------//
	//-------------//
	//-------------//
	//공격정보_지정//
	//-------------//
	switch( tMONSTER_OBJECT->mSpecialSortNumber )
	{
	case 1 :
	case 6 :
		tMONSTER_OBJECT->SetAttackInfoWithAvatar( tAttackInfo->mServerIndex1, tAttackInfo->mUniqueNumber1, &tWhoAttackMonster );
		break;
	}
	//-------------//
	//-------------//
	//-------------//
	switch( tAttackInfo->mAttackActionValue1 )
	{
	case 1 :
		tAttackSuccessValue = tAVATAR_OBJECT1->GetAttackSuccess();
		break;
	case 2 :
		if( mUSER[tUserIndex].mCheckMaxAttackPacketNum == 1 )
		{
			if( ( tAVATAR_OBJECT1->mDATA.aAction.aSkillNumber != tAttackInfo->mAttackActionValue2 ) || ( ( tAVATAR_OBJECT1->mDATA.aAction.aSkillGradeNum1 + tAVATAR_OBJECT1->mDATA.aAction.aSkillGradeNum2 ) != tAttackInfo->mAttackActionValue3 ) )
			{
				return;
			}
		}
		tAttackSuccessValue = tAVATAR_OBJECT1->GetAttackSuccess();
		break;
	default :
		return;
	}
	if( tAttackSuccessValue < 1 )
	{
		return;
	}
	tAttackBlockValue = tMONSTER_OBJECT->mMONSTER_INFO->mAttackBlock;
	if( tAttackBlockValue > 0 )
	{
		if( tAttackSuccessValue > tAttackBlockValue )
		{
			tAttackDetermineValue = (int) ( 70.0f + ( ( (float) tAttackSuccessValue / (float) tAttackBlockValue ) - 1.0f ) * 25.0f );
			if( tAttackDetermineValue > 99 )
			{
				tAttackDetermineValue = 99;
			}
		}
		else
		{
			tAttackDetermineValue = (int) ( 70.0f - ( ( (float) tAttackBlockValue / (float) tAttackSuccessValue ) - 1.0f ) * 25.0f );
			if( tAttackDetermineValue < 1 )
			{
				tAttackDetermineValue = 1;
			}
		}
		tRandom = rand_mir();
		if( ( tRandom % 100 ) >= tAttackDetermineValue )
		{
			tAttackInfo->mAttackResultValue = 0;
			mTRANSFER.B_PROCESS_ATTACK_RECV( tAttackInfo );
			mUSER[tAttackInfo->mServerIndex1].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			return;
		}
	}
	switch( tAttackInfo->mAttackActionValue1 )
	{
	case 1 :
		tAttackPowerValue = tAVATAR_OBJECT1->GetAttackPower();
		break;
	case 2 :
		tAttackPowerValue = tAVATAR_OBJECT1->GetAttackPower();
		tAttackPowerUpRatio = mSKILL.ReturnSkillValue( tAttackInfo->mAttackActionValue2, tAttackInfo->mAttackActionValue3, 7 );
		if( tAttackPowerUpRatio > 0.0f )
		{
			tAttackPowerValue = (int) ( ( float) tAttackPowerValue * ( tAttackPowerUpRatio + 100.0f ) * 0.01f );
		}
		break;
	}
	tDefensePowerValue = tMONSTER_OBJECT->mMONSTER_INFO->mDefensePower;
	tDamageValue = tAttackPowerValue - tDefensePowerValue;
	if( tDamageValue < 1 )
	{
		tDamageValue = 1;
	}
	if( tChargingValue > 0 )
	{
		tDamageValue = (int) ( (float) ( tDamageValue * ( tChargingValue + 100 ) ) * 0.01f );
	}
	tRandom = rand_mir();
	if( ( tRandom % 2 ) == 0 )
	{
		tRandom = rand_mir();
		tDamageValue += (int) ( (float) ( tDamageValue * ( tRandom % 11 ) ) * 0.01f );
	}
	else
	{
		tRandom = rand_mir();
		tDamageValue -= (int) ( (float) ( tDamageValue * ( tRandom % 11 ) ) * 0.01f );
	}
	if( tDamageValue < 1 )
	{
		tDamageValue = 1;
	}
	tAttackInfo->mAttackCriticalExist = 0;
	/* Mir Check  critical attack*/
	//LOG_TO_FILE_1("--> mAttackActionValue1 %d\n", tAttackInfo->mAttackActionValue1);
	switch( tAttackInfo->mAttackActionValue1 )
	{
	case 1 :
		tRandom = rand_mir();
		if( ( tRandom % 100 ) < tAVATAR_OBJECT1->GetCritical() )
		{
			tDamageValue *= 2;
			tAttackInfo->mAttackCriticalExist = 1;
		}
		break;
	case 2 :
		if( tAttackInfo->mAttackActionValue2 == 78 )
		{
			break;
		}
		switch( mSKILL.ReturnAttackType( tAttackInfo->mAttackActionValue2 ) )
		{
		case 2 :
		case 5 :
			tRandom = rand_mir();
			if( ( tRandom % 100 ) < tAVATAR_OBJECT1->GetCritical() )
			{
				tDamageValue *= 2;
				tAttackInfo->mAttackCriticalExist = 1;
			}
			break;
		}
		break;
	}
	tElementAttackPower = tAVATAR_OBJECT1->GetElementAttackPower();
	//LOG_TO_FILE_1("--> tElementAttackPower %d\n", tElementAttackPower);
	switch( tAttackInfo->mAttackActionValue1 )
	{
	case 3 :
		tElementAttackPowerUpRatio = mSKILL.ReturnSkillValue( tAttackInfo->mAttackActionValue2, tAttackInfo->mAttackActionValue3, 8 );
		if( tElementAttackPowerUpRatio > 0.0f )
		{
			tElementAttackPower = (int) ( ( float) tElementAttackPower * ( tElementAttackPowerUpRatio + 100.0f ) * 0.01f );
		}
		break;
	}
	tElementDefensePower = tMONSTER_OBJECT->mMONSTER_INFO->mElementDefensePower;
	if( tElementAttackPower > tElementDefensePower )
	{
		tDamageValue += ( tElementAttackPower - tElementDefensePower );
		tAttackInfo->mAttackElementDamage = tElementAttackPower - tElementDefensePower;
	}
	else
	{
		tAttackInfo->mAttackElementDamage = 0;
	}
	//-------------//
	//성석소유_반영//
	//-------------//
	if( ReturnSymbolDamageUp( tAVATAR_OBJECT1->mDATA.aTribe ) > 0.0f )
	{
		tDamageValue += (int) ( (float) tDamageValue * ReturnSymbolDamageUp( tAVATAR_OBJECT1->mDATA.aTribe ) );
	}
	if( tAVATAR_OBJECT1->mDATA.aLevel1 > 112 )
	{
		if( ReturnSymbolDamageDown( tAVATAR_OBJECT1->mDATA.aTribe ) > 0.0f )
		{
			tDamageValue -= (int) ( (float) tDamageValue * ReturnSymbolDamageDown( tAVATAR_OBJECT1->mDATA.aTribe ) );
		}
	}
	//-------------//
	//-------------//
	//-------------//
	tAttackInfo->mAttackResultValue = 1 + mUSER[tAttackInfo->mServerIndex1].mAvatarInfo.aEquip[7][0];
	tAttackInfo->mAttackViewDamageValue = tDamageValue;
	if( tMONSTER_OBJECT->mDATA.mLifeValue < tDamageValue )
	{
		tDamageValue = tMONSTER_OBJECT->mDATA.mLifeValue;
	}
	tAttackInfo->mAttackRealDamageValue = tDamageValue;
	mTRANSFER.B_PROCESS_ATTACK_RECV( tAttackInfo );
	mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
	tMONSTER_OBJECT->mDATA.mLifeValue -= tDamageValue;
	//-------------//
	//공격정보_지정//
	//-------------//
	switch( tMONSTER_OBJECT->mSpecialSortNumber )
	{
	case 1 :
	case 6 :
		tMONSTER_OBJECT->mAttackDamage[tWhoAttackMonster] += tDamageValue;
		break;
	case 2 :
		tMONSTER_OBJECT->mTribeDamageForTribeSymbol[tAVATAR_OBJECT1->mDATA.aTribe] += tDamageValue;
		break;
	}
	//-------------//
	//-------------//
	//-------------//
	if( tMONSTER_OBJECT->mDATA.mLifeValue > 0 )
	{
		tRandom = rand_mir();
		if( ( tMONSTER_OBJECT->mMONSTER_INFO->mDamageType != 1 ) && ( ( tRandom % 2 ) == 0 ) && ( tDamageValue > (int) ( (float) tMONSTER_OBJECT->mMONSTER_INFO->mLife * 0.10f ) ) && ( tMONSTER_OBJECT->mDATA.mAction.aSort != 8 ) )
		{
			tMONSTER_OBJECT->mDATA.mAction.aSort = 8;
			tMONSTER_OBJECT->mDATA.mAction.aFrame = 0.0f;
			tMONSTER_OBJECT->mDATA.mAction.aFront = mUTIL.GetYAngle( tMONSTER_OBJECT->mDATA.mAction.aLocation[0], tMONSTER_OBJECT->mDATA.mAction.aLocation[2], tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], tAVATAR_OBJECT1->mDATA.aAction.aLocation[2] );
			//------------------------//
			//BROADCAST_MONSTER_ACTION//
			//------------------------//
			mTRANSFER.B_MONSTER_ACTION_RECV( tAttackInfo->mServerIndex2, tMONSTER_OBJECT->mUniqueNumber, &tMONSTER_OBJECT->mDATA, 1 );
			//-----------------------------------------------------------------//
			//[01].일반,[02].성석,[03].수호비,[04].동맹석,[05].수호석,[06].석차//
			//-----------------------------------------------------------------//
			switch( tMONSTER_OBJECT->mSpecialSortNumber )
			{
			case 1 :
                mUTIL.Broadcast( TRUE, &tMONSTER_OBJECT->mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS1, false, 0 );
				break;
			case 2 :
                mUTIL.Broadcast( TRUE, &tMONSTER_OBJECT->mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS3, false, 0 );
				break;
			case 3 :
                mUTIL.Broadcast( TRUE, &tMONSTER_OBJECT->mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS3, false, 0 );
				break;
			case 4 :
                mUTIL.Broadcast( TRUE, &tMONSTER_OBJECT->mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS3, false, 0 );
				break;
			case 5 :
                mUTIL.Broadcast( TRUE, &tMONSTER_OBJECT->mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS2, false, 0 );
				break;
			case 6 :
                mUTIL.Broadcast( TRUE, &tMONSTER_OBJECT->mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS3, false, 0 );
				break;
			}
			//------------------------//
			//------------------------//
			//------------------------//
		}
		return;
	}
	//-----//
	//QUEST//
	//-----//
	switch( mUSER[tUserIndex].mAvatarInfo.aQuestInfo[2] )
	{
	case 1 :
		if( mAVATAR_OBJECT[tUserIndex].ReturnQuestPresentState() != 2 )
		{
			break;
		}
		if( mUSER[tUserIndex].mAvatarInfo.aQuestInfo[3] != tMONSTER_OBJECT->mMONSTER_INFO->mIndex )
		{
			break;
		}
		if( mUSER[tUserIndex].mHealthState == 5 )
		{
			break;
		}
		mUSER[tUserIndex].mAvatarInfo.aQuestInfo[4]++;
		mTRANSFER.B_PROCESS_QUEST_RECV( 0, 6, 0, 0, 0, 0 );
		mUSER[tUserIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
		if( strcmp( mUSER[tUserIndex].mAvatarInfo.aPartyName, "" ) != 0 )
		{
			for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
			{
				if( index01 == tUserIndex )
				{
					continue;
				}
				if( !mAVATAR_OBJECT[index01].mCheckValidState )
				{
					continue;
				}
				if( mUSER[index01].mMoveZoneResult == 1 )
				{
					continue;
				}
				if( mUSER[index01].mAvatarInfo.aVisibleState == 0 )
				{
					continue;
				}
				if( mAVATAR_OBJECT[index01].mDATA.aLifeValue < 1 )
				{
					continue;
				}
				if( strcmp( mAVATAR_OBJECT[index01].mDATA.aPartyName, mUSER[tUserIndex].mAvatarInfo.aPartyName ) != 0 )
				{
					continue;
				}
				if( mUSER[index01].mAvatarInfo.aQuestInfo[2] != 1 )
				{
					continue;
				}
				if( mAVATAR_OBJECT[index01].ReturnQuestPresentState() != 2 )
				{
					break;
				}
				if( mUSER[index01].mAvatarInfo.aQuestInfo[3] != tMONSTER_OBJECT->mMONSTER_INFO->mIndex )
				{
					break;
				}
				mUSER[index01].mAvatarInfo.aQuestInfo[4]++;
				mTRANSFER.B_PROCESS_QUEST_RECV( 0, 8, 0, 0, 0, 0 );
				mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			}
		}
		break;
	case 5 :
		if( mAVATAR_OBJECT[tUserIndex].ReturnQuestPresentState() != 2 )
		{
			break;
		}
		if( mUSER[tUserIndex].mAvatarInfo.aQuestInfo[3] != tMONSTER_OBJECT->mMONSTER_INFO->mIndex )
		{
			break;
		}
		if( mUSER[tUserIndex].mHealthState == 5 )
		{
			break;
		}
		mUSER[tUserIndex].mAvatarInfo.aQuestInfo[4]++;
		mTRANSFER.B_PROCESS_QUEST_RECV( 0, 8, 0, 0, 0, 0 );
		mUSER[tUserIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
		if( strcmp( mUSER[tUserIndex].mAvatarInfo.aPartyName, "" ) != 0 )
		{
			for( index01 = 0 ; index01 < mSERVER.mMAX_USER_NUM ; index01++ )
			{
				if( index01 == tUserIndex )
				{
					continue;
				}
				if( !mAVATAR_OBJECT[index01].mCheckValidState )
				{
					continue;
				}
				if( mUSER[index01].mMoveZoneResult == 1 )
				{
					continue;
				}
				if( mUSER[index01].mAvatarInfo.aVisibleState == 0 )
				{
					continue;
				}
				if( mAVATAR_OBJECT[index01].mDATA.aLifeValue < 1 )
				{
					continue;
				}
				if( strcmp( mAVATAR_OBJECT[index01].mDATA.aPartyName, mUSER[tUserIndex].mAvatarInfo.aPartyName ) != 0 )
				{
					continue;
				}
				if( mUSER[index01].mAvatarInfo.aQuestInfo[2] != 5 )
				{
					continue;
				}
				if( mAVATAR_OBJECT[index01].ReturnQuestPresentState() != 2 )
				{
					break;
				}
				if( mUSER[index01].mAvatarInfo.aQuestInfo[3] != tMONSTER_OBJECT->mMONSTER_INFO->mIndex )
				{
					break;
				}
				mUSER[index01].mAvatarInfo.aQuestInfo[4]++;
				mTRANSFER.B_PROCESS_QUEST_RECV( 0, 8, 0, 0, 0, 0 );
				mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			}
		}
		break;
	}
	//-----//
	//-----//
	//-----//
	//-------------//
	//최종유저_지정//
	//-------------//
	mUSER[tUserIndex].mAvatarInfo.aKillMonsterNum++;
	tMONSTER_OBJECT->mLastAttackUserIndex = tUserIndex;
	tMONSTER_OBJECT->mLastAttackUniqueNumber = mAVATAR_OBJECT[tUserIndex].mUniqueNumber;
	tMONSTER_OBJECT->mLastAttackUserTribe = mAVATAR_OBJECT[tUserIndex].mDATA.aTribe;
	strncpy( tMONSTER_OBJECT->mLastAttackUserName, mAVATAR_OBJECT[tUserIndex].mDATA.aName, MAX_AVATAR_NAME_LENGTH );

	/** 2009. 10. 27 : MonsterSymbol edit (wolf) */
    if( tMONSTER_OBJECT->mMONSTER_INFO->mSpecialType == 14 )
    {
        mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 100;
        mTRANSFER.B_AVATAR_CHANGE_INFO_2( 3, mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe );
        mUSER[tUserIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
    }
    /** 2009. 10. 27 : MonsterSymbol edit (wolf) */

	//-------------//
	//@{ auto quiz kaldirdik- Bot Kontrol Deaktif
	//mUSER[ tUserIndex ].mAvatarInfo.iKill_Monster_Count++;
	//mWORK.Sphinx_Question_Control( tUserIndex );
	//@}
	//-------------//
	//-------------//

    //----------------------//
    //몬스터사냥_기여도_처리//
    //----------------------//
    if( ( mUTIL.ReturnFixedLevel( ( mUSER[tUserIndex].mAvatarInfo.aLevel1 + mUSER[tUserIndex].mAvatarInfo.aLevel2 ) ) - tMONSTER_OBJECT->mMONSTER_INFO->mRealLevel ) < 10 )
    {
        mUSER[tUserIndex].mAvatarInfo.aKillMonsterNum2++;
        if( mUSER[tUserIndex].mAvatarInfo.aKillMonsterNum2 == 1000 )
        {
            mUSER[tUserIndex].mAvatarInfo.aKillMonsterNum2 = 0;
            mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 1;
            mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe = mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe;

            if( mUSER[tUserIndex].mAvatarInfo.aDoubleKillNumTime2 > 0 )
            {
                mUSER[tUserIndex].mAvatarInfo.aDoubleKillNumTime2--;
                mTRANSFER.B_AVATAR_CHANGE_INFO_2( 67, mUSER[tUserIndex].mAvatarInfo.aDoubleKillNumTime2 );
                mUSER[tUserIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
                mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 1;
                mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe = mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe;

//                if( mUSER[tUserIndex].uLoginPremium == 2 )
//                {
//                    mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe += 1;
//                    mAVATAR_OBJECT[tUserIndex].mDATA.aKillOtherTribe = mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe;
//                }
            }
            mTRANSFER.B_AVATAR_CHANGE_INFO_2( 3, mUSER[tUserIndex].mAvatarInfo.aKillOtherTribe );
            mUSER[tUserIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
        }
    }
    //----------------------//
    //----------------------//
    //----------------------//

	//-------------//
	//권한정보_지정//
	//-------------//
	switch( tMONSTER_OBJECT->mSpecialSortNumber )
	{
	case 1 :
	case 6 :
		tSelectAvatarIndex = tMONSTER_OBJECT->SelectAvatarIndexForMaxAttackDamage();
		break;
	case 2 :
	case 3 :
	case 4 :
	case 5 :
		tSelectAvatarIndex = -1;
		break;
	}
	//-------------//
	//-------------//
	//-------------//
	if( tSelectAvatarIndex != -1 )
	{
		::memset(tMONSTER_OBJECT->mAvatarName, 0, MAX_AVATAR_NAME_LENGTH);
		tMONSTER_OBJECT->ProcessForDropItem( tSelectAvatarIndex );
	}
	if( tMONSTER_OBJECT->mMONSTER_INFO->mDamageType != 1 )
	{
		tDeathDirection[0] = tMONSTER_OBJECT->mDATA.mAction.aLocation[0] - tAVATAR_OBJECT1->mDATA.aAction.aLocation[0];
		tDeathDirection[2] = tMONSTER_OBJECT->mDATA.mAction.aLocation[2] - tAVATAR_OBJECT1->mDATA.aAction.aLocation[2];

		tDeathLength = sqrtf( tDeathDirection[0] * tDeathDirection[0] + tDeathDirection[2] * tDeathDirection[2] );
		if( tDeathLength < 1.0f )
		{
			tDeathDirection[0] = 0.0f;
			tDeathDirection[2] = 0.0f;
		}
		else
		{
			tRandom = rand_mir() % 4;
			if(tRandom == 0 || tRandom == 1 || tAttackInfo->mAttackCriticalExist) {
				tDeathDirection[0] /= tDeathLength;
				tDeathDirection[2] /= tDeathLength;
				//LOG_TO_FILE_2("1 %1.1f %1.1f\n", tDeathDirection[0], tDeathDirection[2]);
			} else if(tRandom == 2) {
				tDeathDirection[0] /= (tDeathLength / 4.0f);
				tDeathDirection[2] /= (tDeathLength / 4.0f);
				//LOG_TO_FILE_3("2 %1.1f %1.1f %1.1f\n", tDeathDirection[0], tDeathDirection[2], tDeathLength);
			} else {
				tDeathDirection[0] /= (tDeathLength / 3.7f);
				tDeathDirection[2] /= (tDeathLength / 3.7f);
				//LOG_TO_FILE_3("3 %1.1f %1.1f %1.1f\n", tDeathDirection[0], tDeathDirection[2], tDeathLength);
			}
		}
	}
	else
	{
		tDeathDirection[0] = 0.0f;
		tDeathDirection[2] = 0.0f;
	}
	/* Mir */

	tMONSTER_OBJECT->mDATA.mAction.aSort = 12;
	tMONSTER_OBJECT->mDATA.mAction.aFrame = 0.0f;
	tMONSTER_OBJECT->mDATA.mAction.aTargetLocation[0] = tDeathDirection[0];
	tMONSTER_OBJECT->mDATA.mAction.aTargetLocation[1] = 0.0f;
	tMONSTER_OBJECT->mDATA.mAction.aTargetLocation[2] = tDeathDirection[2];
	tMONSTER_OBJECT->mDATA.mAction.aFront = mUTIL.GetYAngle( tMONSTER_OBJECT->mDATA.mAction.aLocation[0], tMONSTER_OBJECT->mDATA.mAction.aLocation[2], tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], tAVATAR_OBJECT1->mDATA.aAction.aLocation[2] );
	tMONSTER_OBJECT->mDATA.mAction.aSkillNumber = 0;
	if( ( tMONSTER_OBJECT->mMONSTER_INFO->mDamageType != 1 ) && ( tMONSTER_OBJECT->mMONSTER_INFO->mType != 2 ) )
	{
		switch( tAttackInfo->mAttackActionValue1 )
		{
		case 1 :
			tRandom = rand_mir();
			if( ( tAttackInfo->mAttackActionValue2 == 1 ) && ( (tRandom % 2 ) == 0 ) )
			{
				tMONSTER_OBJECT->mDATA.mAction.aSkillNumber = 1;
			}
			break;
		case 2 :
			tRandom = rand_mir();
			if( ( tRandom % 5 ) == 0 )
			{
				tMONSTER_OBJECT->mDATA.mAction.aSkillNumber = 1;
			}
			break;
		}
	}
	//------------------------//
	//BROADCAST_MONSTER_ACTION//
	//------------------------//
	mTRANSFER.B_MONSTER_ACTION_RECV( tAttackInfo->mServerIndex2, tMONSTER_OBJECT->mUniqueNumber, &tMONSTER_OBJECT->mDATA, 1 );
	//-----------------------------------------------------------------//
	//[01].일반,[02].성석,[03].수호비,[04].동맹석,[05].수호석,[06].석차//
	//-----------------------------------------------------------------//
	//LOG_TO_FILE_2("tMONSTER_OBJECT->mSpecialSortNumber : %d %d\n", tMONSTER_OBJECT->mSpecialSortNumber, tMONSTER_OBJECT->mDATA.mAction.aSort);
	switch( tMONSTER_OBJECT->mSpecialSortNumber )
	{
	case 1 :
        mUTIL.Broadcast( TRUE, &tMONSTER_OBJECT->mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS1, false, 0 );
		break;
	case 2 :
        mUTIL.Broadcast( TRUE, &tMONSTER_OBJECT->mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS3, false, 0 );
		break;
	case 3 :
        mUTIL.Broadcast( TRUE, &tMONSTER_OBJECT->mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS3, false, 0 );
		break;
	case 4 :
        mUTIL.Broadcast( TRUE, &tMONSTER_OBJECT->mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS3, false, 0 );
		break;
	case 5 :
        mUTIL.Broadcast( TRUE, &tMONSTER_OBJECT->mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS2, false, 0 );
		break;
	case 6 :
        mUTIL.Broadcast( TRUE, &tMONSTER_OBJECT->mDATA.mAction.aLocation[0], UNIT_SCALE_RADIUS3, false, 0 );
		break;
	}
	//------------------------//
	//------------------------//
	//------------------------//
	if( ( tSelectAvatarIndex == -1 ) || ( tMONSTER_OBJECT->mMONSTER_INFO->mGeneralExperience < 1 ) || ( ( mUTIL.ReturnFixedLevel( mUSER[tSelectAvatarIndex].mAvatarInfo.aLevel1 ) - tMONSTER_OBJECT->mMONSTER_INFO->mRealLevel ) > 9 ) )
	{
		return;
	}
	//------//
	//몬스터//
	//------//
	if( tMONSTER_OBJECT->mMONSTER_INFO->mRealLevel > mUTIL.ReturnFixedLevel( mUSER[tSelectAvatarIndex].mAvatarInfo.aLevel1 ) )
	{
		if( ( tMONSTER_OBJECT->mMONSTER_INFO->mRealLevel - mUTIL.ReturnFixedLevel( mUSER[tSelectAvatarIndex].mAvatarInfo.aLevel1 ) ) > 20 )
		{
			tGainValue = (int) ( (float) tMONSTER_OBJECT->mMONSTER_INFO->mGeneralExperience * ( 3.0f + mUSER[tSelectAvatarIndex].mGeneralExperienceUpRatio ) );
		}
		else
		{
			tGainValue = (int) ( (float) tMONSTER_OBJECT->mMONSTER_INFO->mGeneralExperience * ( ( 1.0f + mUSER[tSelectAvatarIndex].mGeneralExperienceUpRatio ) + (float) ( tMONSTER_OBJECT->mMONSTER_INFO->mRealLevel - mUTIL.ReturnFixedLevel( mUSER[tSelectAvatarIndex].mAvatarInfo.aLevel1 ) ) * 0.1f ) );
		}
	}
	else
	{
		tGainValue = (int) ( (float) tMONSTER_OBJECT->mMONSTER_INFO->mGeneralExperience * ( ( 1.0f + mUSER[tSelectAvatarIndex].mGeneralExperienceUpRatio ) - (float) ( mUTIL.ReturnFixedLevel( mUSER[tSelectAvatarIndex].mAvatarInfo.aLevel1 ) - tMONSTER_OBJECT->mMONSTER_INFO->mRealLevel ) * 0.1f ) );
	}
	//------//
	//------//
	//------//
	//-------------//
	//막타정보_반영//
	//-------------//
	if( strcmp( mUSER[tSelectAvatarIndex].mAvatarInfo.aPartyName, "" ) == 0 )
	{
		tRandom = rand_mir();
		if( ( tRandom % 100 ) < mAVATAR_OBJECT[tSelectAvatarIndex].GetLastAttackBonusProbability() )
		{
			tGainValue += (int) ( (float) tMONSTER_OBJECT->mMONSTER_INFO->mGeneralExperience * mAVATAR_OBJECT[tSelectAvatarIndex].GetLastAttackBonusValue() );
		}
	}
	//-------------//
	//-------------//
	//-------------//
	if( mUSER[tSelectAvatarIndex].mAvatarInfo.aLevel1 < 113 )
	{
		if(30 >= mUSER[tSelectAvatarIndex].mAvatarInfo.aLevel1){
			if((0 < mUSER[tSelectAvatarIndex].mAvatarInfo.aDoubleExpTime3) || ( mUSER[tSelectAvatarIndex].mAvatarInfo.aDoubleExpTime1 > 0 ) || ( mUSER[tSelectAvatarIndex].mAvatarInfo.aDoubleExpTime2 > 0 )) {
				tGainValue *= 2;
			}
		}
		//----------------//
		//지각수_OR_지각환//
		//----------------//
		else{
			if( ( mUSER[tSelectAvatarIndex].mAvatarInfo.aDoubleExpTime1 > 0 ) || ( mUSER[tSelectAvatarIndex].mAvatarInfo.aDoubleExpTime2 > 0 ) )
			{
				tGainValue *= 2;
			}
		}
		//----------------//
		//----------------//
		//----------------//
		if((mUSER[tSelectAvatarIndex].mAvatarInfo.aPat_User_Exp_x2_Time & 0xFFFF) > 0){
			tGainValue *= 2;
		}
	}
	else
	{
		//------//
		//지각환//
		//------//
		if(( mUSER[tSelectAvatarIndex].mAvatarInfo.aDoubleExpTime2 > 0 ) || ( mUSER[tSelectAvatarIndex].mAvatarInfo.aDoubleExpTime1 > 0 ))
		{
			tGainValue *= 2;
		}
		//------//
		//------//
		//------//
		if((mUSER[tSelectAvatarIndex].mAvatarInfo.aPat_User_Exp_x2_Time & 0xFFFF0000) > 0){
			tGainValue *= 2;
		}
	}
	if(112 == mUSER[tSelectAvatarIndex].mAvatarInfo.aLevel1){
		if(0 < mUSER[tSelectAvatarIndex].mAvatarInfo.aFiveExpTime){
			tGainValue *= 5;
		}
	}
	//------//
	//용폭진//
	//------//
	if( ( mUSER[tSelectAvatarIndex].mAvatarInfo.aTribe == mWorldInfo.mZone038WinTribe ) || ( mUSER[tSelectAvatarIndex].mAvatarInfo.aTribe == ReturnAllianceTribe( mGAME.mWorldInfo.mZone038WinTribe ) ) )
	{
		tGainValue += (int) ( (float) tMONSTER_OBJECT->mMONSTER_INFO->mGeneralExperience * 0.1f );
	}
	//------//
	//------//
	//------//
	//------//
	//---------//
	//사제_반영//
	//---------//
	if( ( strcmp( mUSER[tSelectAvatarIndex].mAvatarInfo.aTeacher, "" ) != 0 ) && ( mUSER[tSelectAvatarIndex].mAvatarInfo.aLevel1 < 113 ) && ( strcmp( mUSER[tSelectAvatarIndex].mAvatarInfo.aPartyName, "" ) == 0 ) )
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
			if( mUSER[index01].mAvatarInfo.aVisibleState == 0 )
			{
				continue;
			}
			if( mAVATAR_OBJECT[index01].mDATA.aLifeValue < 1 )
			{
				continue;
			}
			if( ( strcmp( mUSER[index01].mAvatarInfo.aName, mUSER[tSelectAvatarIndex].mAvatarInfo.aTeacher ) == 0 ) && ( strcmp( mUSER[index01].mAvatarInfo.aStudent, mUSER[tSelectAvatarIndex].mAvatarInfo.aName ) == 0 ) )
			{
				break;
			}
		}
		if( index01 < mSERVER.mMAX_USER_NUM )
		{
			tGainValue += (int) ( (float) tMONSTER_OBJECT->mMONSTER_INFO->mGeneralExperience * 0.5f ) * mGAME.mTeacherPointUpRatio;
			/*if( mUSER[index01].mAvatarInfo.aTeacherPoint < MAX_NUMBER_SIZE )
			{
				mUSER[index01].mAvatarInfo.aTeacherPoint += tMONSTER_OBJECT->mMONSTER_INFO->mItemLevel * mGAME.mTeacherPointUpRatio;
				mTRANSFER.B_AVATAR_CHANGE_INFO_2( 6, mUSER[index01].mAvatarInfo.aTeacherPoint );
				mUSER[index01].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			}*/
		}
	}
	//---------//
	//---------//
	//---------//
	//------//
	//이벤트//
	//------//
	
	if( mGeneralExperienceUpRatio > 1.0f )
	{
		tGainValue = (int) ( (float) tGainValue * mGeneralExperienceUpRatio );
	}

	if( mPatExperienceUpRatio > 1.0f )
	{
		tPatExperience = (int) ( (float) tMONSTER_OBJECT->mMONSTER_INFO->mPatExperience * mPatExperienceUpRatio );
	}else{
		tPatExperience = tMONSTER_OBJECT->mMONSTER_INFO->mPatExperience;
	}

	//------//
	//------//
	//------//
	if ((mUSER[tSelectAvatarIndex].mAvatarInfo.aPat_User_Exp_x2_Time & 0xFFFF) > 0 ||
			( mUSER[tSelectAvatarIndex].mAvatarInfo.aPat_User_Exp_x2_Time & 0xFFFF0000 ) > 0) {
		tPatExperience *= 2;
	} else {
		if(mUSER[tSelectAvatarIndex].mAvatarInfo.aPat_Exp_x2_Time > 0 ) {
			tPatExperience *= 2;
		}
	}

	/** 2009. 11. 23 : 패키지 아이템 개발 (wolf) */
#ifdef __TK__
	int iValue = (int)((float)mUSER[tSelectAvatarIndex].mSelfPatExperienceUpRatio * tPatExperience);
	tPatExperience += iValue;	
#endif
	/** 2009. 11. 23 : 패키지 아이템 개발 (wolf) */

	//mUTIL.ProcessForExperience( tSelectAvatarIndex, tGainValue, tMONSTER_OBJECT->mMONSTER_INFO->mPatExperience, ( tMONSTER_OBJECT->mMONSTER_INFO->mLife / 100 ) );
	mUTIL.ProcessForExperience( tSelectAvatarIndex, tGainValue, tPatExperience, ( tMONSTER_OBJECT->mMONSTER_INFO->mLife / 100 ) );
	if( strcmp( mUSER[tSelectAvatarIndex].mAvatarInfo.aPartyName, "" ) == 0 )
	{
		return;
	}
	//---------//
	//동행_반영//
	//---------//
	tTotalPartyNum = 0;
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
		if( mUSER[index01].mAvatarInfo.aVisibleState == 0 )
		{
			continue;
		}
		if( mAVATAR_OBJECT[index01].mDATA.aLifeValue < 1 )
		{
			continue;
		}
		if( strcmp( mAVATAR_OBJECT[index01].mDATA.aPartyName, mUSER[tSelectAvatarIndex].mAvatarInfo.aPartyName ) != 0 )
		{
			continue;
		}
		tPartyUserIndex[tTotalPartyNum] = index01;
		tTotalPartyNum++;
		if( tTotalPartyNum == MAX_PARTY_AVATAR_NUM )
		{
			break;
		}
	}
	if( tTotalPartyNum < 2 )
	{
		return;
	}
	tBonusGainGeneralExperience = 0;
	switch( tTotalPartyNum )
	{
	case 2 : tBonusGainGeneralExperience = (int) ( (float) tMONSTER_OBJECT->mMONSTER_INFO->mGeneralExperience * 0.1f ); break;
	case 3 : tBonusGainGeneralExperience = (int) ( (float) tMONSTER_OBJECT->mMONSTER_INFO->mGeneralExperience * 0.2f ); break;
	case 4 : tBonusGainGeneralExperience = (int) ( (float) tMONSTER_OBJECT->mMONSTER_INFO->mGeneralExperience * 0.3f ); break;
	case 5 : tBonusGainGeneralExperience = (int) ( (float) tMONSTER_OBJECT->mMONSTER_INFO->mGeneralExperience * 0.5f ); break;
	}
	if( mBonusGainGeneralExperienceUpRatio > 1.0f )
	{
		tBonusGainGeneralExperience = (int) ( (float) tBonusGainGeneralExperience * mBonusGainGeneralExperienceUpRatio );
	}

	if( tBonusGainGeneralExperience < 1 )
	{
		return;
	}
	for( index01 = 0 ; index01 < tTotalPartyNum ; index01++ )
	{
		mUTIL.ProcessForExperience( tPartyUserIndex[index01], tBonusGainGeneralExperience, 0, 0 );
	}
	//---------//
	//---------//
	//---------//
}
//PROCESS_ATTACK_04[몬스터->아바타]
void MyGame::ProcessAttack04( int tUserIndex, ATTACK_FOR_PROTOCOL *tAttackInfo )
{
	//-----------------------------------------------------------------//
	//[01].일반,[02].성석,[03].수호비,[04].동맹석,[05].수호석,[06].석차//
	//-----------------------------------------------------------------//

	int tMinusGeneralExperience;
	unsigned int tRandom = 0;

	if( ( tAttackInfo->mServerIndex2 != tUserIndex ) || ( tAttackInfo->mUniqueNumber2 != mAVATAR_OBJECT[tUserIndex].mUniqueNumber ) )
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	tAVATAR_OBJECT1 = &mAVATAR_OBJECT[tAttackInfo->mServerIndex2];
	tAVATAR_OBJECT1->mDATA.aAction.aLocation[0] = tAttackInfo->mSenderLocation[0];
	tAVATAR_OBJECT1->mDATA.aAction.aLocation[1] = tAttackInfo->mSenderLocation[1];
	tAVATAR_OBJECT1->mDATA.aAction.aLocation[2] = tAttackInfo->mSenderLocation[2];
	tAVATAR_OBJECT1->SetSpaceIndex();
	if( ( tAttackInfo->mServerIndex1 < 0 ) || ( tAttackInfo->mServerIndex1 > ( MAX_MONSTER_OBJECT_NUM - 1 ) ) )
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	tMONSTER_OBJECT = &mMONSTER_OBJECT[tAttackInfo->mServerIndex1];
	if( ( !tMONSTER_OBJECT->mCheckValidState ) || ( tMONSTER_OBJECT->mUniqueNumber != tAttackInfo->mUniqueNumber1 ) )
	{
		return;
	}
	tMONSTER_OBJECT->mCheckAttackPacketRecv = FALSE;
	if( mUSER[tAttackInfo->mServerIndex2].mMoveZoneResult == 1 )
	{
		return;
	}
	if( tAVATAR_OBJECT1->mDATA.aLifeValue < 1 )
	{
		return;
	}
	if( tAVATAR_OBJECT1->mDATA.aPShopState == 1 )
	{
		return;
	}
	if( !CheckPossibleAttackTarget( 1, tAVATAR_OBJECT1->mDATA.aAction.aSort ) )
	{
		return;
	}
	if( ( mTickCount - mUSER[tAttackInfo->mServerIndex2].mTickCountFor01SecondForProtect ) < PROTECT_TICK )
	{
		return;
	}
	if( tMONSTER_OBJECT->mDATA.mLifeValue < 1 )
	{
		return;
	}
	if( mUTIL.GetDoubleX_Z( &tMONSTER_OBJECT->mDATA.mAction.aLocation[0], &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0] ) > (float) ( tMONSTER_OBJECT->mMONSTER_INFO->mRadiusInfo[1] * tMONSTER_OBJECT->mMONSTER_INFO->mRadiusInfo[1] ) )
	{
		return;
	}
	tAttackSuccessValue = tMONSTER_OBJECT->mMONSTER_INFO->mAttackSuccess;
	if( tAttackSuccessValue < 1 )
	{
		return;
	}
	tAttackBlockValue = tAVATAR_OBJECT1->GetAttackBlock();
	if( tAttackBlockValue > 0 )
	{
		if( tAttackSuccessValue > tAttackBlockValue )
		{
			tAttackDetermineValue = (int) ( 70.0f + ( ( (float) tAttackSuccessValue / (float) tAttackBlockValue ) - 1.0f ) * 25.0f );
			if( tAttackDetermineValue > 99 )
			{
				tAttackDetermineValue = 99;
			}
		}
		else
		{
			tAttackDetermineValue = (int) ( 70.0f - ( ( (float) tAttackBlockValue / (float) tAttackSuccessValue ) - 1.0f ) * 25.0f );
			if( tAttackDetermineValue < 1 )
			{
				tAttackDetermineValue = 1;
			}
		}
		tRandom = rand_mir();
		if( ( tRandom % 100 ) >= tAttackDetermineValue )
		{
			tAttackInfo->mAttackResultValue = 0;
			mTRANSFER.B_PROCESS_ATTACK_RECV( tAttackInfo );
			mUSER[tAttackInfo->mServerIndex2].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			return;
		}
	}
	tAttackPowerValue = tMONSTER_OBJECT->mMONSTER_INFO->mAttackPower;
	tDefensePowerValue = tAVATAR_OBJECT1->GetDefensePower();
	tDamageValue = tAttackPowerValue - tDefensePowerValue;
	if( tDamageValue < 1 )
	{
		tDamageValue = 1;
	}
	tRandom = rand_mir();
	if( ( tRandom % 2 ) == 0 )
	{
		tRandom = rand_mir();
		tDamageValue += (int) ( (float) ( tDamageValue * ( tRandom % 11 ) ) * 0.01f );
	}
	else
	{
		tRandom = rand_mir();
		tDamageValue -= (int) ( (float) ( tDamageValue * ( tRandom % 11 ) ) * 0.01f );
	}
	if( tDamageValue < 1 )
	{
		tDamageValue = 1;
	}
	tAttackInfo->mAttackCriticalExist = 0;
	if( tMONSTER_OBJECT->mMONSTER_INFO->mCritical > tAVATAR_OBJECT1->GetCriticalDefense() )
	{
		tRandom = rand_mir();
		if( ( tRandom % 100 ) < ( tMONSTER_OBJECT->mMONSTER_INFO->mCritical - tAVATAR_OBJECT1->GetCriticalDefense() ) )
		{
			tDamageValue *= 2;
			tAttackInfo->mAttackCriticalExist = 1;
		}
	}
	else
	{
		tRandom = rand_mir();
		if( ( tRandom % 100 ) < 1 )
		{
			tDamageValue *= 2;
			tAttackInfo->mAttackCriticalExist = 1;
		}
	}
	//------------------//
	//천신미궁_보스_처리//
	//------------------//
	if( tAttackInfo->mAttackActionValue4 == 0 )
	{
		switch( tMONSTER_OBJECT->mMONSTER_INFO->mSpecialType )
		{
		case 40 :
			tRandom = rand_mir();
			if( ( tRandom % 100 ) < 10 )
			{
				mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][0] = 0;
				mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][1] = 0;
				tAVATAR_OBJECT1->mDATA.aEffectValueForView[9] = 0;
				mTRANSFER.B_AVATAR_CHANGE_INFO_1( tAttackInfo->mServerIndex2, tAttackInfo->mUniqueNumber2, 3, 0, 0, 0 );
					mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
			}
			tRandom = rand_mir();
			if( ( tRandom % 100 ) < 10 )
			{
				tAVATAR_OBJECT1->mDATA.aAction.aType = 0;
				tAVATAR_OBJECT1->mDATA.aAction.aSort = 11;
				tAVATAR_OBJECT1->mDATA.aAction.aFrame = 0.0f;
				tAVATAR_OBJECT1->mDATA.aAction.aSkillValue = 10;
				mTRANSFER.B_AVATAR_ACTION_RECV( tAttackInfo->mServerIndex2, tAVATAR_OBJECT1->mUniqueNumber, &tAVATAR_OBJECT1->mDATA, 1 );
					mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
			}
			break;
		case 41 :
			tRandom = rand_mir();
			if( ( tRandom % 100 ) < 15 )
			{
				mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][0] = 0;
				mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][1] = 0;
				tAVATAR_OBJECT1->mDATA.aEffectValueForView[9] = 0;
				mTRANSFER.B_AVATAR_CHANGE_INFO_1( tAttackInfo->mServerIndex2, tAttackInfo->mUniqueNumber2, 3, 0, 0, 0 );
					mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
			}
			tRandom = rand_mir();
			if( ( tRandom % 100 ) < 15 )
			{
				tAVATAR_OBJECT1->mDATA.aAction.aType = 0;
				tAVATAR_OBJECT1->mDATA.aAction.aSort = 11;
				tAVATAR_OBJECT1->mDATA.aAction.aFrame = 0.0f;
				tAVATAR_OBJECT1->mDATA.aAction.aSkillValue = 10;
				mTRANSFER.B_AVATAR_ACTION_RECV( tAttackInfo->mServerIndex2, tAVATAR_OBJECT1->mUniqueNumber, &tAVATAR_OBJECT1->mDATA, 1 );
					mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
			}
			break;
		case 42 :
			tRandom = rand_mir();
			if( ( tRandom % 100 ) < 20 )
			{
				mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][0] = 0;
				mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][1] = 0;
				tAVATAR_OBJECT1->mDATA.aEffectValueForView[9] = 0;
				mTRANSFER.B_AVATAR_CHANGE_INFO_1( tAttackInfo->mServerIndex2, tAttackInfo->mUniqueNumber2, 3, 0, 0, 0 );
					mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
			}
			tRandom = rand_mir();
			if( ( tRandom % 100 ) < 20 )
			{
				tAVATAR_OBJECT1->mDATA.aAction.aType = 0;
				tAVATAR_OBJECT1->mDATA.aAction.aSort = 11;
				tAVATAR_OBJECT1->mDATA.aAction.aFrame = 0.0f;
				tAVATAR_OBJECT1->mDATA.aAction.aSkillValue = 10;
				mTRANSFER.B_AVATAR_ACTION_RECV( tAttackInfo->mServerIndex2, tAVATAR_OBJECT1->mUniqueNumber, &tAVATAR_OBJECT1->mDATA, 1 );
					mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
			}
			break;
		case 43 :
			tRandom = rand_mir();
			if( ( tRandom % 100 ) < 25 )
			{
				mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][0] = 0;
				mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][1] = 0;
				tAVATAR_OBJECT1->mDATA.aEffectValueForView[9] = 0;
				mTRANSFER.B_AVATAR_CHANGE_INFO_1( tAttackInfo->mServerIndex2, tAttackInfo->mUniqueNumber2, 3, 0, 0, 0 );
					mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
			}
			tRandom = rand_mir();
			if( ( tRandom % 100 ) < 25 )
			{
				tAVATAR_OBJECT1->mDATA.aAction.aType = 0;
				tAVATAR_OBJECT1->mDATA.aAction.aSort = 11;
				tAVATAR_OBJECT1->mDATA.aAction.aFrame = 0.0f;
				tAVATAR_OBJECT1->mDATA.aAction.aSkillValue = 10;
				mTRANSFER.B_AVATAR_ACTION_RECV( tAttackInfo->mServerIndex2, tAVATAR_OBJECT1->mUniqueNumber, &tAVATAR_OBJECT1->mDATA, 1 );
					mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
			}
			break;
		case 44 :
			tRandom = rand_mir();
			if( ( tRandom % 100 ) < 30 )
			{
				mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][0] = 0;
				mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][1] = 0;
				tAVATAR_OBJECT1->mDATA.aEffectValueForView[9] = 0;
				mTRANSFER.B_AVATAR_CHANGE_INFO_1( tAttackInfo->mServerIndex2, tAttackInfo->mUniqueNumber2, 3, 0, 0, 0 );
					mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
			}
			tRandom = rand_mir();
			if( ( tRandom % 100 ) < 30 )
			{
				tAVATAR_OBJECT1->mDATA.aAction.aType = 0;
				tAVATAR_OBJECT1->mDATA.aAction.aSort = 11;
				tAVATAR_OBJECT1->mDATA.aAction.aFrame = 0.0f;
				tAVATAR_OBJECT1->mDATA.aAction.aSkillValue = 10;
				mTRANSFER.B_AVATAR_ACTION_RECV( tAttackInfo->mServerIndex2, tAVATAR_OBJECT1->mUniqueNumber, &tAVATAR_OBJECT1->mDATA, 1 );
					mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
			}
			break;
		}
	}
	else if( tAttackInfo->mAttackActionValue4 == 1 )
	{
		switch( tMONSTER_OBJECT->mMONSTER_INFO->mSpecialType )
		{
		case 40 :
			tRandom = rand_mir();
			if( ( tRandom % 100 ) < 50 )
			{
				mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][0] = 0;
				mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][1] = 0;
				tAVATAR_OBJECT1->mDATA.aEffectValueForView[9] = 0;
				mTRANSFER.B_AVATAR_CHANGE_INFO_1( tAttackInfo->mServerIndex2, tAttackInfo->mUniqueNumber2, 3, 0, 0, 0 );
					mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
			}
			tRandom = rand_mir();
			if( ( tRandom % 100 ) < 50 )
			{
				tAVATAR_OBJECT1->mDATA.aAction.aType = 0;
				tAVATAR_OBJECT1->mDATA.aAction.aSort = 11;
				tAVATAR_OBJECT1->mDATA.aAction.aFrame = 0.0f;
				tAVATAR_OBJECT1->mDATA.aAction.aSkillValue = 10;
				mTRANSFER.B_AVATAR_ACTION_RECV( tAttackInfo->mServerIndex2, tAVATAR_OBJECT1->mUniqueNumber, &tAVATAR_OBJECT1->mDATA, 1 );
					mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
			}
			break;
		case 41 :
			tRandom = rand_mir();
			if( ( tRandom % 100 ) < 60 )
			{
				mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][0] = 0;
				mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][1] = 0;
				tAVATAR_OBJECT1->mDATA.aEffectValueForView[9] = 0;
				mTRANSFER.B_AVATAR_CHANGE_INFO_1( tAttackInfo->mServerIndex2, tAttackInfo->mUniqueNumber2, 3, 0, 0, 0 );
					mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
			}
			tRandom = rand_mir();
			if( ( tRandom % 100 ) < 60 )
			{
				tAVATAR_OBJECT1->mDATA.aAction.aType = 0;
				tAVATAR_OBJECT1->mDATA.aAction.aSort = 11;
				tAVATAR_OBJECT1->mDATA.aAction.aFrame = 0.0f;
				tAVATAR_OBJECT1->mDATA.aAction.aSkillValue = 10;
				mTRANSFER.B_AVATAR_ACTION_RECV( tAttackInfo->mServerIndex2, tAVATAR_OBJECT1->mUniqueNumber, &tAVATAR_OBJECT1->mDATA, 1 );
					mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
			}
			break;
		case 42 :
			tRandom = rand_mir();
			if( ( tRandom % 100 ) < 70 )
			{
				mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][0] = 0;
				mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][1] = 0;
				tAVATAR_OBJECT1->mDATA.aEffectValueForView[9] = 0;
				mTRANSFER.B_AVATAR_CHANGE_INFO_1( tAttackInfo->mServerIndex2, tAttackInfo->mUniqueNumber2, 3, 0, 0, 0 );
					mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
			}
			tRandom = rand_mir();
			if( ( tRandom % 100 ) < 70 )
			{
				tAVATAR_OBJECT1->mDATA.aAction.aType = 0;
				tAVATAR_OBJECT1->mDATA.aAction.aSort = 11;
				tAVATAR_OBJECT1->mDATA.aAction.aFrame = 0.0f;
				tAVATAR_OBJECT1->mDATA.aAction.aSkillValue = 10;
				mTRANSFER.B_AVATAR_ACTION_RECV( tAttackInfo->mServerIndex2, tAVATAR_OBJECT1->mUniqueNumber, &tAVATAR_OBJECT1->mDATA, 1 );
					mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
			}
			break;
		case 43 :
			tRandom = rand_mir();
			if( ( tRandom % 100 ) < 80 )
			{
				mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][0] = 0;
				mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][1] = 0;
				tAVATAR_OBJECT1->mDATA.aEffectValueForView[9] = 0;
				mTRANSFER.B_AVATAR_CHANGE_INFO_1( tAttackInfo->mServerIndex2, tAttackInfo->mUniqueNumber2, 3, 0, 0, 0 );
					mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
			}
			tRandom = rand_mir();
			if( ( tRandom % 100 ) < 80 )
			{
				tAVATAR_OBJECT1->mDATA.aAction.aType = 0;
				tAVATAR_OBJECT1->mDATA.aAction.aSort = 11;
				tAVATAR_OBJECT1->mDATA.aAction.aFrame = 0.0f;
				tAVATAR_OBJECT1->mDATA.aAction.aSkillValue = 10;
				mTRANSFER.B_AVATAR_ACTION_RECV( tAttackInfo->mServerIndex2, tAVATAR_OBJECT1->mUniqueNumber, &tAVATAR_OBJECT1->mDATA, 1 );
					mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
			}
			break;
		case 44 :
			tRandom = rand_mir();
			if( ( tRandom % 100 ) < 90 )
			{
				mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][0] = 0;
				mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][1] = 0;
				tAVATAR_OBJECT1->mDATA.aEffectValueForView[9] = 0;
				mTRANSFER.B_AVATAR_CHANGE_INFO_1( tAttackInfo->mServerIndex2, tAttackInfo->mUniqueNumber2, 3, 0, 0, 0 );
					mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
			}
			tRandom = rand_mir();
			if( ( tRandom % 100 ) < 90 )
			{
				tAVATAR_OBJECT1->mDATA.aAction.aType = 0;
				tAVATAR_OBJECT1->mDATA.aAction.aSort = 11;
				tAVATAR_OBJECT1->mDATA.aAction.aFrame = 0.0f;
				tAVATAR_OBJECT1->mDATA.aAction.aSkillValue = 10;
				mTRANSFER.B_AVATAR_ACTION_RECV( tAttackInfo->mServerIndex2, tAVATAR_OBJECT1->mUniqueNumber, &tAVATAR_OBJECT1->mDATA, 1 );
					mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
			}
			break;
		}
	}
	else;
	//------------------//
	//------------------//
	//------------------//
	if( mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][0] > 0 )
	{
		if( tDamageValue > mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][0] )
		{
			tShieldDamageValue = mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][0];
		}
		else
		{
			tShieldDamageValue = tDamageValue;
		}
		mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][0] -= tShieldDamageValue;
		if( mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][0] < 1 )
		{
			mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][0] = 0;
			mUSER[tAttackInfo->mServerIndex2].mEffectValue[9][1] = 0;
		}
		tAVATAR_OBJECT1->mDATA.aEffectValueForView[9] -= tShieldDamageValue;
		if( tAVATAR_OBJECT1->mDATA.aEffectValueForView[9] < 1 )
		{
			tAVATAR_OBJECT1->mDATA.aEffectValueForView[9] = 0;
		}
		mTRANSFER.B_AVATAR_CHANGE_INFO_1( tAttackInfo->mServerIndex2, tAttackInfo->mUniqueNumber2, 6, tShieldDamageValue, 0, 0 );
		mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
		tDamageValue -= tShieldDamageValue;
	}
	tElementAttackPower = tMONSTER_OBJECT->mMONSTER_INFO->mElementAttackPower;
	tElementDefensePower = tAVATAR_OBJECT1->GetElementDefensePower();
	if( tElementAttackPower > tElementDefensePower )
	{
		tDamageValue += ( tElementAttackPower - tElementDefensePower );
		tAttackInfo->mAttackElementDamage = tElementAttackPower - tElementDefensePower;
	}
	else
	{
		tAttackInfo->mAttackElementDamage = 0;
	}
	tAttackInfo->mAttackResultValue = 1;
	tAttackInfo->mAttackViewDamageValue = tDamageValue;
	if( tAVATAR_OBJECT1->mDATA.aLifeValue < tDamageValue )
	{
		tDamageValue = tAVATAR_OBJECT1->mDATA.aLifeValue;
	}
	tAttackInfo->mAttackRealDamageValue = tDamageValue;
	mTRANSFER.B_PROCESS_ATTACK_RECV( tAttackInfo );
	mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
	tAVATAR_OBJECT1->mDATA.aLifeValue -= tDamageValue;
	if( tAVATAR_OBJECT1->mDATA.aLifeValue > 0 )
	{
		return;
	}
	//------------------------//
	//PROCESS_FOR_AVATAR_DEATH//
	//------------------------//
	tDeathDirection[0] = tAVATAR_OBJECT1->mDATA.aAction.aLocation[0] - tMONSTER_OBJECT->mDATA.mAction.aLocation[0];
	tDeathDirection[2] = tAVATAR_OBJECT1->mDATA.aAction.aLocation[2] - tMONSTER_OBJECT->mDATA.mAction.aLocation[2];
	tDeathLength = sqrtf( tDeathDirection[0] * tDeathDirection[0] + tDeathDirection[2] * tDeathDirection[2] );
	if( tDeathLength < 1.0f )
	{
		tDeathDirection[0] = 0.0f;
		tDeathDirection[2] = 0.0f;
	}
	else
	{
		tDeathDirection[0] /= tDeathLength;
		tDeathDirection[2] /= tDeathLength;
	}
	tAVATAR_OBJECT1->mDATA.aAction.aType = 0;
	tAVATAR_OBJECT1->mDATA.aAction.aSort = 12;
	tAVATAR_OBJECT1->mDATA.aAction.aFrame = 0.0f;
	tAVATAR_OBJECT1->mDATA.aAction.aTargetLocation[0] = tDeathDirection[0];
	tAVATAR_OBJECT1->mDATA.aAction.aTargetLocation[1] = 0.0f;
	tAVATAR_OBJECT1->mDATA.aAction.aTargetLocation[2] = tDeathDirection[2];
	tAVATAR_OBJECT1->mDATA.aAction.aFront = mUTIL.GetYAngle( tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], tAVATAR_OBJECT1->mDATA.aAction.aLocation[2], tMONSTER_OBJECT->mDATA.mAction.aLocation[0], tMONSTER_OBJECT->mDATA.mAction.aLocation[2] );
	tAVATAR_OBJECT1->mDATA.aAction.aTargetFront = tAVATAR_OBJECT1->mDATA.aAction.aFront;
	tAVATAR_OBJECT1->mDATA.aAction.aSkillNumber = 5;
	mTRANSFER.B_AVATAR_ACTION_RECV( tAttackInfo->mServerIndex2, tAVATAR_OBJECT1->mUniqueNumber, &tAVATAR_OBJECT1->mDATA, 1 );
	mUSER[tAttackInfo->mServerIndex2].Send (TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
	mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT1->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, tAttackInfo->mServerIndex1, false);
	tAVATAR_OBJECT1->ProcessForDeath();
	if ((890 == tMONSTER_OBJECT->mMONSTER_INFO->mIndex) ||
			(891 == tMONSTER_OBJECT->mMONSTER_INFO->mIndex)){
		return;
	}
	//------------------------//
	//------------------------//
	//------------------------//
	//-----------------------------------------------------------------//
	//[01].일반,[02].성석,[03].수호비,[04].동맹석,[05].수호석,[06].석차//
	//-----------------------------------------------------------------//
	switch( tMONSTER_OBJECT->mSpecialSortNumber )
	{
	case 1 :
		if( ( mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aLevel1 < 10 ) || ( mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aLevel1 >= 145 ) )
		{
			// 2009.12.29 - 33갑 수호부 기능 - 수호부 1회 감소시켜 달라는 요청으로 수정합니다.
			if( mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aLevel1 >= 145 )
			{
				if (mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aProtectForDeath > 0)
                {
                	// 수호부 감소 처리.
                    mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aProtectForDeath--;
                    mTRANSFER.B_AVATAR_CHANGE_INFO_2( 8, mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aProtectForDeath );
                    mUSER[tAttackInfo->mServerIndex2].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
                }
                else
                {
                    mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aKillOtherTribe -= 10;
                    if( mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aKillOtherTribe < 0 )
                    {
                        mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aKillOtherTribe = 0;
                    }
                    tAVATAR_OBJECT1->mDATA.aKillOtherTribe -= 10;
                    if( tAVATAR_OBJECT1->mDATA.aKillOtherTribe < 0 )
                    {
                        tAVATAR_OBJECT1->mDATA.aKillOtherTribe = 0;
                    }
                }
			}
			mTRANSFER.B_AVATAR_CHANGE_INFO_2( 3, mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aKillOtherTribe );
			mUSER[tAttackInfo->mServerIndex2].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			return;
		}

#ifdef __TK__
		if( mUSER[tAttackInfo->mServerIndex2].bUsePackItem )
			tMinusGeneralExperience = (int) ( (float) ( mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aGeneralExperience1 - mLEVEL.ReturnLevelFactor1( mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aLevel1 ) ) * ( 0.03f * mUSER[tAttackInfo->mServerIndex2].mGeneralExperienceDownRatio ) );
		else
			tMinusGeneralExperience = (int) ( (float) ( mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aGeneralExperience1 - mLEVEL.ReturnLevelFactor1( mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aLevel1 ) ) * ( 0.05f * mUSER[tAttackInfo->mServerIndex2].mGeneralExperienceDownRatio ) );
#else
		tMinusGeneralExperience = (int) ( (float) ( mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aGeneralExperience1 - mLEVEL.ReturnLevelFactor1( mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aLevel1 ) ) * ( 0.05f * mUSER[tAttackInfo->mServerIndex2].mGeneralExperienceDownRatio ) );
#endif
#if 0
		if(mGeneralExperienceDownRatio > 1 || mGeneralExperienceDownRatio < 10) {
			mGeneralExperienceDownRatio = mGeneralExperienceDownRatio;
		} else {
			mGeneralExperienceDownRatio = 1;
		}
		LOG_TO_FILE_3("aName %s, %d, %d, \n", mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aName, mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aGeneralExperience1, tMinusGeneralExperience);
#endif
		tMinusGeneralExperience = tMinusGeneralExperience * mGeneralExperienceDownRatio;

		if( tMinusGeneralExperience < 1 )
		{
			return;
		}
		/* Mir added */
		if(mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aGeneralExperience1 <= tMinusGeneralExperience)
			tMinusGeneralExperience = mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aGeneralExperience1;
		/* Mir */

		if( mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aProtectForDeath < 1 )
		{
			mGAMELOG.GL_633_MINUS_EXP_WITH_DIE( tAttackInfo->mServerIndex2, mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aGeneralExperience1, ( mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aGeneralExperience1 - tMinusGeneralExperience ) );
			mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aGeneralExperience1 -= tMinusGeneralExperience;
			mTRANSFER.B_AVATAR_CHANGE_INFO_2( 7, mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aGeneralExperience1 );
			mUSER[tAttackInfo->mServerIndex2].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
		}
		else
		{
			mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aProtectForDeath--;
			mTRANSFER.B_AVATAR_CHANGE_INFO_2( 8, mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aProtectForDeath );
			mUSER[tAttackInfo->mServerIndex2].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
		}
		return;
	}
}
//PROCESS_ATTACK_05[점혈]
void MyGame::ProcessAttack05( int tUserIndex, ATTACK_FOR_PROTOCOL *tAttackInfo )
{
	int index01;
	unsigned int tRandom = 0;

	if( ( tAttackInfo->mServerIndex1 != tUserIndex ) || ( tAttackInfo->mUniqueNumber1 != mAVATAR_OBJECT[tUserIndex].mUniqueNumber ) )
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			               
		return;
	}
	tAVATAR_OBJECT1 = &mAVATAR_OBJECT[tAttackInfo->mServerIndex1];
	tAVATAR_OBJECT1->mDATA.aAction.aLocation[0] = tAttackInfo->mSenderLocation[0];
	tAVATAR_OBJECT1->mDATA.aAction.aLocation[1] = tAttackInfo->mSenderLocation[1];
	tAVATAR_OBJECT1->mDATA.aAction.aLocation[2] = tAttackInfo->mSenderLocation[2];
	tAVATAR_OBJECT1->SetSpaceIndex();
	if( ( tAttackInfo->mServerIndex2 < 0 ) || ( tAttackInfo->mServerIndex2 > ( mSERVER.mMAX_USER_NUM - 1 ) ) )
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if(1 == mUSER[tUserIndex].mAuthInfo.AttackFlag){
		return;
	}
	tAVATAR_OBJECT2 = &mAVATAR_OBJECT[tAttackInfo->mServerIndex2];
	if( ( !tAVATAR_OBJECT2->mCheckValidState ) || ( tAVATAR_OBJECT2->mUniqueNumber != tAttackInfo->mUniqueNumber2 ) )
	{
		return;
	}
	if( tAVATAR_OBJECT1->mDATA.aLifeValue < 1 )
	{
		return;
	}
	if( ( mTickCount - mUSER[tAttackInfo->mServerIndex1].mTickCountFor01SecondForProtect ) < PROTECT_TICK )
	{
		return;
	}
	if( mUSER[tAttackInfo->mServerIndex2].mMoveZoneResult == 1 )
	{
		return;
	}
	if( tAVATAR_OBJECT2->mDATA.aLifeValue < 1 )
	{
		return;
	}
	if(1 == tAVATAR_OBJECT2->mGod){
		return;
	}
	if(1 == mUSER[tAttackInfo->mServerIndex2].mAuthInfo.AttackFlag){
		return;
	}
	if( tAVATAR_OBJECT2->mDATA.aPShopState == 1 )
	{
		return;
	}
	if( !CheckPossibleAttackTarget( 1, tAVATAR_OBJECT2->mDATA.aAction.aSort ) )
	{
		return;
	}
	if( ( mTickCount - mUSER[tAttackInfo->mServerIndex2].mTickCountFor01SecondForProtect ) < PROTECT_TICK )
	{
		return;
	}
	if( ( mZONEMAININFO.ReturnZoneTribeInfo2( mSERVER_INFO.mServerNumber ) == 0 ) || ( tAVATAR_OBJECT1->mDATA.aTribe == tAVATAR_OBJECT2->mDATA.aTribe ) || ( tAVATAR_OBJECT2->mDATA.aTribe == ReturnAllianceTribe( tAVATAR_OBJECT1->mDATA.aTribe ) ) )
	{
		if( ( tAVATAR_OBJECT1->mDATA.aDuelState[0] != 1 ) || ( tAVATAR_OBJECT2->mDATA.aDuelState[0] != 1 ) || ( tAVATAR_OBJECT1->mDATA.aDuelState[1] != tAVATAR_OBJECT2->mDATA.aDuelState[1] ) || ( tAVATAR_OBJECT1->mDATA.aDuelState[2] == tAVATAR_OBJECT2->mDATA.aDuelState[2] ) )
		{
			return;
		}
	}
	//LOG_TO_FILE_1("mUSER[tUserIndex].mCheckMaxAttackPacketNum : %d\n", mUSER[tUserIndex].mCheckMaxAttackPacketNum);
	if( mUSER[tUserIndex].mCheckMaxAttackPacketNum == 1 )
	{
		if( ( tAVATAR_OBJECT1->mDATA.aAction.aSkillNumber != tAttackInfo->mAttackActionValue2 ) || ( ( tAVATAR_OBJECT1->mDATA.aAction.aSkillGradeNum1 + tAVATAR_OBJECT1->mDATA.aAction.aSkillGradeNum2 ) != tAttackInfo->mAttackActionValue3 ) )
		{
			return;
		}
	}
	tAttackSuccessValue = (int) mSKILL.ReturnSkillValue( tAttackInfo->mAttackActionValue2, tAttackInfo->mAttackActionValue3, 4 );
	//LOG_TO_FILE_1("====> tAttackSuccessValue : %d\n", tAttackSuccessValue);
	if( tAttackSuccessValue < 1 )
	{
		return;
	}
	//LOG_TO_FILE_1("tAttackInfo->mAttackActionValue2 : %d\n", tAttackInfo->mAttackActionValue2);
	if( tAttackInfo->mAttackActionValue2 != 80 )
	{
		for( index01 = 0 ; index01 < MAX_SKILL_SLOT_NUM ; index01++ )
		{
			if( ( mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aSkill[index01][0] ==  5 ) || ( mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aSkill[index01][0] == 24 ) || ( mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aSkill[index01][0] == 43 ) )
			{
				break;
			}
		}
		if( index01 < MAX_SKILL_SLOT_NUM )
		{
			tAttackBlockValue = (int) mSKILL.ReturnSkillValue( mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aSkill[index01][0], ( mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aSkill[index01][1] + tAVATAR_OBJECT2->GetBonusSkillValue( mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aSkill[index01][0] ) ), 5 );
			//LOG_TO_FILE_4("tAttackBlockValue : %d, %d, %d, %d\n", mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aSkill[index01][1], tAVATAR_OBJECT2->GetBonusSkillValue( mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aSkill[index01][0] ), mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aSkill[index01][0], ( mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aSkill[index01][1] + tAVATAR_OBJECT2->GetBonusSkillValue( mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aSkill[index01][0] ) ));
		}
		else
		{
			tAttackBlockValue = 0;
		}
			//LOG_TO_FILE_1("mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aLevel1 : %d\n", mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aLevel1);
		if( mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aLevel1 > 99 )
		{
			tAttackBlockValue += ( 1 + ( mUSER[tAttackInfo->mServerIndex2].mAvatarInfo.aLevel1 - 100 ) / 2 );
		}
			//LOG_TO_FILE_1("tAttackBlockValue : %d\n", tAttackBlockValue);
			//LOG_TO_FILE_1("tAVATAR_OBJECT2->mDATA.aEffectValueForView[13] : %d\n", tAVATAR_OBJECT2->mDATA.aEffectValueForView[13]);
		if( tAVATAR_OBJECT2->mDATA.aEffectValueForView[13] > 0 )
		{
			tAttackBlockValue = (int) ( (float) ( tAttackBlockValue * ( tAVATAR_OBJECT2->mDATA.aEffectValueForView[13] + 100 ) ) * 0.01f );
			//LOG_TO_FILE_1("tAttackBlockValue : %d\n", tAttackBlockValue);
		}
	}
	else
	{
		tAttackBlockValue = 0;
	}

	tRandom = rand_mir() % 100;
	//LOG_TO_FILE_4("====> 1tAttackSuccessValue : %d %d, rand_num : %d %d\n", tAttackSuccessValue, tAttackBlockValue, tRandom, ( tAttackSuccessValue - tAttackBlockValue - 1 ));
	if( (int) tRandom  > (int)( tAttackSuccessValue - tAttackBlockValue - 1 ) )
	{
		return;
	}
	tAVATAR_OBJECT2->mDATA.aAction.aType = 0;
	tAVATAR_OBJECT2->mDATA.aAction.aSort = 11;
	tAVATAR_OBJECT2->mDATA.aAction.aFrame = 0.0f;
	tAVATAR_OBJECT2->mDATA.aAction.aSkillValue = (int) mSKILL.ReturnSkillValue( tAttackInfo->mAttackActionValue2, tAttackInfo->mAttackActionValue3, 10 );
	mTRANSFER.B_AVATAR_ACTION_RECV( tAttackInfo->mServerIndex2, tAVATAR_OBJECT2->mUniqueNumber, &tAVATAR_OBJECT2->mDATA, 1 );
    mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT2->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
}
//PROCESS_ATTACK_06[해혈]
void MyGame::ProcessAttack06( int tUserIndex, ATTACK_FOR_PROTOCOL *tAttackInfo )
{
	unsigned int tRandom = 0;
	if( ( tAttackInfo->mServerIndex1 != tUserIndex ) || ( tAttackInfo->mUniqueNumber1 != mAVATAR_OBJECT[tUserIndex].mUniqueNumber ) )
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	tAVATAR_OBJECT1 = &mAVATAR_OBJECT[tAttackInfo->mServerIndex1];
	tAVATAR_OBJECT1->mDATA.aAction.aLocation[0] = tAttackInfo->mSenderLocation[0];
	tAVATAR_OBJECT1->mDATA.aAction.aLocation[1] = tAttackInfo->mSenderLocation[1];
	tAVATAR_OBJECT1->mDATA.aAction.aLocation[2] = tAttackInfo->mSenderLocation[2];
	tAVATAR_OBJECT1->SetSpaceIndex();
	if( ( tAttackInfo->mServerIndex2 < 0 ) || ( tAttackInfo->mServerIndex2 > ( mSERVER.mMAX_USER_NUM - 1 ) ) )
	{
		mUSER[tUserIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	if(1 == mUSER[tUserIndex].mAuthInfo.AttackFlag){
		return;
	}
	tAVATAR_OBJECT2 = &mAVATAR_OBJECT[tAttackInfo->mServerIndex2];
	if( ( !tAVATAR_OBJECT2->mCheckValidState ) || ( tAVATAR_OBJECT2->mUniqueNumber != tAttackInfo->mUniqueNumber2 ) )
	{
		return;
	}
	if(1 == mUSER[tAttackInfo->mServerIndex2].mAuthInfo.AttackFlag){
		return;
	}
	if( tAVATAR_OBJECT1->mDATA.aLifeValue < 1 )
	{
		return;
	}
	if( mUSER[tAttackInfo->mServerIndex2].mMoveZoneResult == 1 )
	{
		return;
	}
	if( tAVATAR_OBJECT2->mDATA.aLifeValue < 1 )
	{
		return;
	}
	if( tAVATAR_OBJECT2->mDATA.aPShopState == 1 )
	{
		return;
	}
	if( tAVATAR_OBJECT2->mDATA.aAction.aSort != 11 )
	{
		return;
	}
	if( ( tAVATAR_OBJECT1->mDATA.aTribe != tAVATAR_OBJECT2->mDATA.aTribe ) && ( tAVATAR_OBJECT1->mDATA.aTribe != ReturnAllianceTribe( tAVATAR_OBJECT2->mDATA.aTribe ) ) )
	{
		return;
	}
	if( mUSER[tUserIndex].mCheckMaxAttackPacketNum == 1 )
	{
		if( ( tAVATAR_OBJECT1->mDATA.aAction.aSkillNumber != tAttackInfo->mAttackActionValue2 ) || ( ( tAVATAR_OBJECT1->mDATA.aAction.aSkillGradeNum1 + tAVATAR_OBJECT1->mDATA.aAction.aSkillGradeNum2 ) != tAttackInfo->mAttackActionValue3 ) )
		{
			return;
		}
	}
	tAttackSuccessValue = (int) mSKILL.ReturnSkillValue( tAttackInfo->mAttackActionValue2, tAttackInfo->mAttackActionValue3, 5 );
	if( tAttackSuccessValue < 1 )
	{
		return;
	}
	tRandom = rand_mir();
	if( ( tRandom % 100 ) > ( ( tAttackSuccessValue * 2 ) - 1 ) )
	{
		return;
	}
	tAVATAR_OBJECT2->mDATA.aAction.aType = mAVATAR_OBJECT[tAttackInfo->mServerIndex2].GetWeaponClass() * 2;
	tAVATAR_OBJECT2->mDATA.aAction.aSort = 1;
	tAVATAR_OBJECT2->mDATA.aAction.aFrame = 0.0f;
	mTRANSFER.B_AVATAR_ACTION_RECV( tAttackInfo->mServerIndex2, tAVATAR_OBJECT2->mUniqueNumber, &tAVATAR_OBJECT2->mDATA, 1 );
    mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT2->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
}
//-------------------------------------------------------------------------------------------------
