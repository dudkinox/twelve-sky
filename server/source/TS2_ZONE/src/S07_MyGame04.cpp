//-------------------------------------------------------------------------------------------------
//HEADER
//-------------------------------------------------------------------------------------------------
#include "H01_MainApplication.h"
//-------------------------------------------------------------------------------------------------
//CLASS_OF_AVATAR_OBJECT
//-------------------------------------------------------------------------------------------------
//INSTANCE
AVATAR_OBJECT mAVATAR_OBJECT[MAX_AVATAR_OBJECT_NUM];
//SET_SPACE_INDEX
void AVATAR_OBJECT::Init(void)
{
	mLuck = 0;
	mIndex = 0;
	mMaxLife = 0;
	mMaxMana = 0;
	mCritical = 0;
	mAttackBlock = 0;
	mAttackPower = 0;
	mUniqueNumber = 0;
	mDefensePower = 0;
	mAttackSuccess = 0;
	mSPACE_INDEX[0] = 0;
	mSPACE_INDEX[1] = 0;
	mSPACE_INDEX[2] = 0;
	mCriticalDefense = 0;
	mDuelProcessSort = 0;
	mTradeProcessMenu = 0;
	mDuelProcessState = 0;
	mPartyProcessState = 0;
	mGuildProcessState = 0;
	mTradeProcessState = 0;
	mElementAttackPower = 0;
	mFriendProcessState = 0;
	mElementDefensePower = 0;
	mTeacherProcessState = 0;
	mCheckValidState = false;
	mDuelProcessRemainTime = 0;
	mLastAttackBonusValue = 0.0f;
	mLastAttackBonusProbability = 0;
	mCheckPossibleEatPotion = false;
	::memset(&mPShopInfo, 0x00, sizeof(PSHOP_INFO));
	::memset(&mDATA, 0x00, sizeof(OBJECT_FOR_AVATAR));
	::memset(mDuelProcessAvatarName, 0x00, MAX_AVATAR_NAME_LENGTH);
	::memset(mTradeProcessAvatarName, 0x00, MAX_AVATAR_NAME_LENGTH);
	::memset(mPartyProcessAvatarName, 0x00, MAX_AVATAR_NAME_LENGTH);
	::memset(mGuildProcessAvatarName, 0x00, MAX_AVATAR_NAME_LENGTH);
	::memset(mFriendProcessAvatarName, 0x00, MAX_AVATAR_NAME_LENGTH);
	::memset(mTeacherProcessAvatarName, 0x00, MAX_AVATAR_NAME_LENGTH);
}
void AVATAR_OBJECT::SetSpaceIndex( void )
{
	mSPACE_INDEX[0] = (int) ( mDATA.aAction.aLocation[0] / MAX_RADIUS_FOR_NETWORK );
	mSPACE_INDEX[1] = (int) ( mDATA.aAction.aLocation[1] / MAX_RADIUS_FOR_NETWORK );
	mSPACE_INDEX[2] = (int) ( mDATA.aAction.aLocation[2] / MAX_RADIUS_FOR_NETWORK );
}
//SET_BASIC_ABILITY_FROM_EQUIP
void AVATAR_OBJECT::SetBasicAbilityFromEquip( void )
{
	if( !mCheckValidState )
	{
		return;
	}

	int index01;
	AVATAR_INFO *tAvatarInfo = &mUSER[mIndex].mAvatarInfo;
	int tVitality;
	int tKi;
	int tStrength;
	int tWisdom;
	int tISValue;
	int tIUValue;
	int tIUEffectValue;

	for( index01 = 0 ; index01 < MAX_EQUIP_SLOT_NUM ; index01++ )
	{
		mEquipItem[index01] = mITEM.Search( tAvatarInfo->aEquip[index01][0] );
		mEquipValue[index01] = tAvatarInfo->aEquip[index01][2];
	}
	mMaxLife = 1;
	tVitality = tAvatarInfo->aVitality;
	for( index01 = 0 ; index01 < MAX_EQUIP_SLOT_NUM ; index01++ )
	{
		if( mEquipItem[index01] == NULL )
		{
			continue;
		}
		tVitality += mEquipItem[index01]->iVitality;
	}
	mMaxLife = (int) ( (float) tVitality * 20.00f );

	/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */
	// 장식 해방 시스템이 사용중인 경우 체력 증가.
	if (IsUsedOrnament () == 1) // 금장서의 경우
	{
		mMaxLife += 825;
	}
	else if (IsUsedOrnament () == 2) // 은장서의 경우
	{
		mMaxLife += 550; // 현재는 은장서만 사용 가능.
	}
	/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */
#ifdef __GOD__
	if( ( !mGAME.mCheckZone049TypeServer ) && ( !mGAME.mCheckZone051TypeServer ) && ( !mGAME.mCheckZone053TypeServer ) && ( !mGAME.mCheckZone267TypeServer ) )
#else
	if( ( !mGAME.mCheckZone049TypeServer ) && ( !mGAME.mCheckZone051TypeServer ) && ( !mGAME.mCheckZone053TypeServer ) )
#endif
	{
		mMaxLife += ( tAvatarInfo->aEatLifePotion * 20 );
	}
#ifdef __GOD__
	mMaxLife += mLEVEL.ReturnLevelFactorLife( ( mDATA.aLevel1 + mDATA.aLevel2 ) );
#else
	mMaxLife += mLEVEL.ReturnLevelFactorLife( mDATA.aLevel1 );
#endif


#ifdef __ANIMAL__
	if( mDATA.aAnimalNumber == ANIMAL_NUM_DONWHOANG )
	{
		mMaxLife = (int) ( (float) mMaxLife * ANIMAL_RATE_ONE_GRADE );
	}
	if( mDATA.aAnimalNumber == ANIMAL_NUM_TWODON )
	{
		mMaxLife = (int) ( (float) mMaxLife * ANIMAL_RATE_TWO_GRADE );
	}
	if( mDATA.aAnimalNumber == ANIMAL_NUM_SUNGDON )
	{
		mMaxLife = (int) ( (float) mMaxLife * ANIMAL_RATE_THREE_GRADE );
	}	
	if( mDATA.aAnimalNumber == ANIMAL_NUM_CRISTMAS )
	{
		mMaxLife = (int) ( (float) mMaxLife * ANIMAL_RATE_THREE_GRADE );
	}
#endif


	if( mMaxLife < (int) mPAT.ReturnLifeValue( tAvatarInfo->aEquip[8][0], tAvatarInfo->aEquip[8][2] ) )
	{
		mMaxLife += mMaxLife;
	}
	else
	{
		mMaxLife += (int) mPAT.ReturnLifeValue( tAvatarInfo->aEquip[8][0], tAvatarInfo->aEquip[8][2] );
	}
	mDATA.aMaxLifeValue = mMaxLife;
	mMaxMana = 1;
	tKi = tAvatarInfo->aKi;
	for( index01 = 0 ; index01 < MAX_EQUIP_SLOT_NUM ; index01++ )
	{
		if( mEquipItem[index01] == NULL )
		{
			continue;
		}
		tKi += mEquipItem[index01]->iKi;
	}
	mMaxMana = (int) ( (float) tKi * 15.31f );

	/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */
	// 장식 해방 시스템이 사용중인 경우 내공 증가.
	if (IsUsedOrnament () == 1) // 금장서의 경우
	{
		mMaxMana += 750; 
	}
	else if (IsUsedOrnament () == 2) //은장서의 경우
	{
		mMaxMana += 500; // 현재는 은장서만 사용 가능.
	}
	/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */

	if( ( !mGAME.mCheckZone049TypeServer ) && ( !mGAME.mCheckZone051TypeServer ) && ( !mGAME.mCheckZone053TypeServer ) )
	{
		mMaxMana += ( tAvatarInfo->aEatManaPotion * 25 );
	}
#ifdef __GOD__
	mMaxMana += mLEVEL.ReturnLevelFactorMana( ( mDATA.aLevel1 + mDATA.aLevel2 ) );
#else
	mMaxMana += mLEVEL.ReturnLevelFactorMana( mDATA.aLevel1 );
#endif

#ifdef __ANIMAL__
	if( ( mDATA.aAnimalNumber == ANIMAL_NUM_BAKHO ) 
		|| ( mDATA.aAnimalNumber == ANIMAL_NUM_DONWHOANG ) 
		|| ( mDATA.aAnimalNumber == ANIMAL_NUM_CHUNLOK ))
	{
		mMaxMana = (int) ( (float) mMaxMana * ANIMAL_RATE_ONE_GRADE );
	}

	if( ( mDATA.aAnimalNumber == ANIMAL_NUM_CHUNGHO ) 
		|| ( mDATA.aAnimalNumber == ANIMAL_NUM_TWODON ) 
		|| ( mDATA.aAnimalNumber == ANIMAL_NUM_BAIKLOK ))
	{
		mMaxMana = (int) ( (float) mMaxMana * ANIMAL_RATE_TWO_GRADE );
	}
	if( ( mDATA.aAnimalNumber == ANIMAL_NUM_CHUKHO ) 
		|| ( mDATA.aAnimalNumber == ANIMAL_NUM_SUNGDON ) 
		|| ( mDATA.aAnimalNumber == ANIMAL_NUM_SHINLOK ))
	{
		mMaxMana = (int) ( (float) mMaxMana * ANIMAL_RATE_THREE_GRADE );
	}
#endif

	if( mMaxMana < (int) mPAT.ReturnManaValue( tAvatarInfo->aEquip[8][0], tAvatarInfo->aEquip[8][2] ) )
	{
		mMaxMana += mMaxMana;
	}
	else
	{
		mMaxMana += (int) mPAT.ReturnManaValue( tAvatarInfo->aEquip[8][0], tAvatarInfo->aEquip[8][2] );
	}
	mDATA.aMaxManaValue = mMaxMana;
	mAttackPower = 0;
	tStrength = tAvatarInfo->aStrength;
	tKi = tAvatarInfo->aKi;
	for( index01 = 0 ; index01 < MAX_EQUIP_SLOT_NUM ; index01++ )
	{
		if( mEquipItem[index01] == NULL )
		{
			continue;
		}
		tStrength += mEquipItem[index01]->iStrength;
		tKi += mEquipItem[index01]->iKi;
	}
	if( mEquipItem[7] )
	{
		switch( mEquipItem[7]->iSort )
		{
		case 13 :
		case 17 :
		case 19 :
			mAttackPower += (int) ( (float) tStrength * 2.65f );
			mAttackPower += (int) ( (float) tKi * 1.43f );
			break;
		case 14 :
		case 16 :
		case 20 :
			mAttackPower += (int) ( (float) tStrength * 2.80f );
			mAttackPower += (int) ( (float) tKi * 1.51f );
			break;
		case 15 :
		case 18 :
		case 21 :
			mAttackPower += (int) ( (float) tStrength * 2.51f );
			mAttackPower += (int) ( (float) tKi * 1.35f );
			break;
		}
	}
	else
	{
		mAttackPower += (int) ( (float) tStrength * 1.25f );
		mAttackPower += (int) ( (float) tKi * 0.67f );
	}

	/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */
	// 장식 해방 시스템이 사용중인 경우 공격력 증가.
	if (IsUsedOrnament () == 1) // 금장서의 경우
	{
		mAttackPower += 413;
	}
	else if (IsUsedOrnament () == 2) 
	{
		mAttackPower += 275;
	}

	/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */
#ifdef __GOD__
	mAttackPower += mLEVEL.ReturnLevelFactorAttackPower( ( mDATA.aLevel1 + mDATA.aLevel2 ) );
#else
	mAttackPower += mLEVEL.ReturnLevelFactorAttackPower( mDATA.aLevel1 );
#endif
	
	for( index01 = 0 ; index01 < MAX_EQUIP_SLOT_NUM ; index01++ )
	{
		if( mEquipItem[index01] == NULL )
		{
			continue;
		}
		mAttackPower += mEquipItem[index01]->iAttackPower;
		if( index01 == 7 )
		{
			tIUValue = mUTIL.ReturnIUValueWithLevelLimit( tAvatarInfo->aLevel1, mUTIL.ReturnIUValue( mEquipValue[index01] ) );
			tIUEffectValue = mUTIL.ReturnIUEffectValue( mEquipItem[index01], 1 );
			mAttackPower += ( tIUValue * tIUEffectValue );
			tISValue = mUTIL.ReturnISValueWithLevelLimit( tAvatarInfo->aLevel1, mUTIL.ReturnISValue( mEquipValue[index01] ) );
			if( tISValue > 0 )
			{
				mAttackPower += (int) ( (float) ( mEquipItem[index01]->iAttackPower + ( tIUValue * tIUEffectValue ) ) * ( (float) tISValue * 0.03f ) );
			}
		}
	}

	mAttackPower += ( tAvatarInfo->aEatStrengthPotion * 3 );

#ifdef __ANIMAL__
	if( mDATA.aAnimalNumber == ANIMAL_NUM_BAKHO )
	{
		mAttackPower = (int) ( (float) mAttackPower * ANIMAL_RATE_ONE_GRADE );
	}
	if( mDATA.aAnimalNumber == ANIMAL_NUM_CHUNGHO )
	{
		mAttackPower = (int) ( (float) mAttackPower * ANIMAL_RATE_TWO_GRADE );
	}
	if( mDATA.aAnimalNumber == ANIMAL_NUM_CHUKHO )
	{
		mAttackPower = (int) ( (float) mAttackPower * ANIMAL_RATE_THREE_GRADE );
	}
	
	if( mDATA.aAnimalNumber == ANIMAL_NUM_CRISTMAS )
	{
		mAttackPower = (int) ( (float) mAttackPower * ANIMAL_RATE_THREE_GRADE );
	}
#endif
	mDefensePower = 0;
	tWisdom = tAvatarInfo->aWisdom;
	for( index01 = 0 ; index01 < MAX_EQUIP_SLOT_NUM ; index01++ )
	{
		if( mEquipItem[index01] == NULL )
		{
			continue;
		}
		tWisdom += mEquipItem[index01]->iWisdom;
	}
	mDefensePower += (int) ( (float) tWisdom * 1.63f );

	/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */
	// 장식 해방 시스템이 사용중인 경우 방어력 증가.
	if (IsUsedOrnament () == 1) 
	{
		mDefensePower += 825;
	}
	else if (IsUsedOrnament () == 2) 
	{
		mDefensePower += 550;
	}
	/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */

#ifdef __GOD__
	mDefensePower += mLEVEL.ReturnLevelFactorDefensePower( ( mDATA.aLevel1 + mDATA.aLevel2 ) );
#else
	mDefensePower += mLEVEL.ReturnLevelFactorDefensePower( mDATA.aLevel1 );
#endif
	for( index01 = 0 ; index01 < MAX_EQUIP_SLOT_NUM ; index01++ )
	{
		if( mEquipItem[index01] == NULL )
		{
			continue;
		}
		mDefensePower += mEquipItem[index01]->iDefensePower;
		switch( index01 )
		{
		case 1 :
			tIUValue = mUTIL.ReturnIUValueWithLevelLimit( tAvatarInfo->aLevel1, mUTIL.ReturnIUValue( mEquipValue[index01] ) );
			tIUEffectValue = mUTIL.ReturnIUEffectValue( mEquipItem[index01], 2 );
			mDefensePower += ( tIUValue * tIUEffectValue );
			tISValue = mUTIL.ReturnISValueWithLevelLimit( tAvatarInfo->aLevel1, mUTIL.ReturnISValue( mEquipValue[index01] ) );
			if( tISValue > 0 )
			{
				mDefensePower += ( tISValue * 6 );
			}
			break;
		case 2 :
			tIUValue = mUTIL.ReturnIUValueWithLevelLimit( tAvatarInfo->aLevel1, mUTIL.ReturnIUValue( mEquipValue[index01] ) );
			tIUEffectValue = mUTIL.ReturnIUEffectValue( mEquipItem[index01], 2 );
			mDefensePower += ( tIUValue * tIUEffectValue );
			tISValue = mUTIL.ReturnISValueWithLevelLimit( tAvatarInfo->aLevel1, mUTIL.ReturnISValue( mEquipValue[index01] ) );
			if( tISValue > 0 )
			{
				mDefensePower += (int) ( (float) ( mEquipItem[index01]->iDefensePower + ( tIUValue * tIUEffectValue ) ) * ( (float) tISValue * 0.03f ) );
			}
			break;
		case 3 :
			tIUValue = mUTIL.ReturnIUValueWithLevelLimit( tAvatarInfo->aLevel1, mUTIL.ReturnIUValue( mEquipValue[index01] ) );
			tIUEffectValue = mUTIL.ReturnIUEffectValue( mEquipItem[index01], 2 );
			mDefensePower += ( tIUValue * tIUEffectValue );
			break;
		case 5 :
			tIUValue = mUTIL.ReturnIUValueWithLevelLimit( tAvatarInfo->aLevel1, mUTIL.ReturnIUValue( mEquipValue[index01] ) );
			tIUEffectValue = mUTIL.ReturnIUEffectValue( mEquipItem[index01], 2 );
			mDefensePower += ( tIUValue * tIUEffectValue );
			break;
		}
	}
	mAttackSuccess = 0;
	tStrength = tAvatarInfo->aStrength;
	for( index01 = 0 ; index01 < MAX_EQUIP_SLOT_NUM ; index01++ )
	{
		if( mEquipItem[index01] == NULL )
		{
			continue;
		}
		tStrength += mEquipItem[index01]->iStrength;
	}
	mAttackSuccess += (int) ( (float) tStrength * 1.71f );
#ifdef __GOD__
	mAttackSuccess += mLEVEL.ReturnLevelFactorAttackSuccess( ( mDATA.aLevel1 + mDATA.aLevel2 ) );
#else
	mAttackSuccess += mLEVEL.ReturnLevelFactorAttackSuccess( mDATA.aLevel1 );
#endif
	for( index01 = 0 ; index01 < MAX_EQUIP_SLOT_NUM ; index01++ )
	{
		if( mEquipItem[index01] == NULL )
		{
			continue;
		}
		mAttackSuccess += mEquipItem[index01]->iAttackSucess;
		switch( index01 )
		{
		case 3 :
			tIUValue = mUTIL.ReturnIUValueWithLevelLimit( tAvatarInfo->aLevel1, mUTIL.ReturnIUValue( mEquipValue[index01] ) );
			tIUEffectValue = mUTIL.ReturnIUEffectValue( mEquipItem[index01], 3 );
			mAttackSuccess += ( tIUValue * tIUEffectValue );
			tISValue = mUTIL.ReturnISValueWithLevelLimit( tAvatarInfo->aLevel1, mUTIL.ReturnISValue( mEquipValue[index01] ) );
			if( tISValue > 0 )
			{
				mAttackSuccess += (int) ( (float) ( mEquipItem[index01]->iAttackSucess + ( tIUValue * tIUEffectValue ) ) * ( (float) tISValue * 0.03f ) );
			}
			break;
		case 7 :
			tIUValue = mUTIL.ReturnIUValueWithLevelLimit( tAvatarInfo->aLevel1, mUTIL.ReturnIUValue( mEquipValue[index01] ) );
			tIUEffectValue = mUTIL.ReturnIUEffectValue( mEquipItem[index01], 3 );
			mAttackSuccess += ( tIUValue * tIUEffectValue );
			break;
		}
	}
	//Original Korea code is not useable in battle zone.
#if 0
	if( ( !mGAME.mCheckZone049TypeServer ) && ( !mGAME.mCheckZone051TypeServer ) && ( !mGAME.mCheckZone053TypeServer ) )
	{
		mAttackSuccess += ( tAvatarInfo->aEatWisdomPotion * 2 );
	}
#endif
	mAttackSuccess += ( tAvatarInfo->aEatWisdomPotion * 2 );
	mAttackBlock = 0;
	tWisdom = tAvatarInfo->aWisdom;
	tVitality = tAvatarInfo->aVitality;
	for( index01 = 0 ; index01 < MAX_EQUIP_SLOT_NUM ; index01++ )
	{
		if( mEquipItem[index01] == NULL )
		{
			continue;
		}
		tWisdom += mEquipItem[index01]->iWisdom;
		tVitality += mEquipItem[index01]->iVitality;
	}
	mAttackBlock += (int) ( (float) tWisdom * 1.67f );
	mAttackBlock += (int) ( (float) tVitality * 0.90f );
	
#ifdef __GOD__
	mAttackBlock += mLEVEL.ReturnLevelFactorAttackBlock( ( mDATA.aLevel1 + mDATA.aLevel2 ) );
#else
	mAttackBlock += mLEVEL.ReturnLevelFactorAttackBlock( mDATA.aLevel1 );
#endif
	mAttackBlock *= 2;
	for( index01 = 0 ; index01 < MAX_EQUIP_SLOT_NUM ; index01++ )
	{
		if( mEquipItem[index01] == NULL )
		{
			continue;
		}
		mAttackBlock += mEquipItem[index01]->iAttackBlock;
		switch( index01 )
		{
		case 2 :
			tIUValue = mUTIL.ReturnIUValueWithLevelLimit( tAvatarInfo->aLevel1, mUTIL.ReturnIUValue( mEquipValue[index01] ) );
			tIUEffectValue = mUTIL.ReturnIUEffectValue( mEquipItem[index01], 4 );
			mAttackBlock += ( tIUValue * tIUEffectValue );
			break;
		case 5 :
			tIUValue = mUTIL.ReturnIUValueWithLevelLimit( tAvatarInfo->aLevel1, mUTIL.ReturnIUValue( mEquipValue[index01] ) );
			tIUEffectValue = mUTIL.ReturnIUEffectValue( mEquipItem[index01], 4 );
			mAttackBlock += ( tIUValue * tIUEffectValue );
			tISValue = mUTIL.ReturnISValueWithLevelLimit( tAvatarInfo->aLevel1, mUTIL.ReturnISValue( mEquipValue[index01] ) );
			if( tISValue > 0 )
			{
				mAttackBlock += (int) ( (float) ( mEquipItem[index01]->iAttackBlock + ( tIUValue * tIUEffectValue ) ) * ( (float) tISValue * 0.03f ) );
			}
			break;
		}
	}
	//Original Korea code is not useable in battle zone.
#if 0
	if( ( !mGAME.mCheckZone049TypeServer ) && ( !mGAME.mCheckZone051TypeServer ) && ( !mGAME.mCheckZone053TypeServer ) )
	{
		mAttackBlock += ( tAvatarInfo->aEatWisdomPotion * 2 );
	}
#endif
	mAttackBlock += ( tAvatarInfo->aEatWisdomPotion * 2 );
	
#ifdef __GOD__
	mElementAttackPower = mLEVEL.ReturnLevelFactorElementAttack( ( mDATA.aLevel1 + mDATA.aLevel2 ) );
#else
	mElementAttackPower = mLEVEL.ReturnLevelFactorElementAttack( mDATA.aLevel1 );
#endif
	for( index01 = 0 ; index01 < MAX_EQUIP_SLOT_NUM ; index01++ )
	{
		if( mEquipItem[index01] == NULL )
		{
			continue;
		}
		mElementAttackPower += mEquipItem[index01]->iElementAttackPower;
		switch( index01 )
		{
		case 4 :
			tIUValue = mUTIL.ReturnIUValueWithLevelLimit( tAvatarInfo->aLevel1, mUTIL.ReturnIUValue( mEquipValue[index01] ) );
			tIUEffectValue = mUTIL.ReturnIUEffectValue( mEquipItem[index01], 5 );
			mElementAttackPower += ( tIUValue * tIUEffectValue );
			break;
		}
	}

#ifdef __ANIMAL__
	if( ( mDATA.aAnimalNumber == ANIMAL_NUM_BAKHO ) || ( mDATA.aAnimalNumber == ANIMAL_NUM_CHUNLOK ) )
	{
		mElementAttackPower = (int) ( (float) mElementAttackPower * ANIMAL_RATE_ONE_GRADE );
	}
	if( ( mDATA.aAnimalNumber == ANIMAL_NUM_CHUNGHO ) || ( mDATA.aAnimalNumber == ANIMAL_NUM_BAIKLOK ) )
	{
		mElementAttackPower = (int) ( (float) mElementAttackPower * ANIMAL_RATE_TWO_GRADE );
	}
	if( ( mDATA.aAnimalNumber == ANIMAL_NUM_CHUKHO ) || ( mDATA.aAnimalNumber == ANIMAL_NUM_SHINLOK )|| ( mDATA.aAnimalNumber == ANIMAL_NUM_CRISTMAS ) )
	{
		mElementAttackPower = (int) ( (float) mElementAttackPower * ANIMAL_RATE_THREE_GRADE );
	}
#endif

	mElementDefensePower = 0;
	for( index01 = 0 ; index01 < MAX_EQUIP_SLOT_NUM ; index01++ )
	{
		if( mEquipItem[index01] == NULL )
		{
			continue;
		}
		mElementDefensePower += mEquipItem[index01]->iElementDefensePower;
		switch( index01 )
		{
		case 0 :
			tIUValue = mUTIL.ReturnIUValueWithLevelLimit( tAvatarInfo->aLevel1, mUTIL.ReturnIUValue( mEquipValue[index01] ) );
			tIUEffectValue = mUTIL.ReturnIUEffectValue( mEquipItem[index01], 6 );
			mElementDefensePower += ( tIUValue * tIUEffectValue );
			break;
		}
	}

#ifdef __ANIMAL__
	if( mDATA.aAnimalNumber == ANIMAL_NUM_DONWHOANG )
	{
		mElementDefensePower = (int) ( (float) mElementDefensePower * ANIMAL_RATE_ONE_GRADE );
	}
	if( mDATA.aAnimalNumber == ANIMAL_NUM_TWODON )
	{
		mElementDefensePower = (int) ( (float) mElementDefensePower * ANIMAL_RATE_TWO_GRADE );
	}
	if( mDATA.aAnimalNumber == ANIMAL_NUM_SUNGDON )
	{
		mElementDefensePower = (int) ( (float) mElementDefensePower * ANIMAL_RATE_THREE_GRADE );
	}
#endif


	mCritical = 2;
	for( index01 = 0 ; index01 < MAX_EQUIP_SLOT_NUM ; index01++ )
	{
		if( mEquipItem[index01] == NULL )
		{
			continue;
		}
		mCritical += mEquipItem[index01]->iCritical;
		switch( index01 )
		{
		case 4 :
			tISValue = mUTIL.ReturnISValueWithLevelLimit( tAvatarInfo->aLevel1, mUTIL.ReturnISValue( mEquipValue[index01] ) );
			if( tISValue > 0 )
			{
				mCritical += ( tISValue / 4 );
			}
			break;
		}
	}

#ifdef __ANIMAL__
	if( mDATA.aAnimalNumber == ANIMAL_NUM_CHUNLOK )
	{
		mCritical = (int) ( (float) mCritical * ANIMAL_RATE_ONE_GRADE );
	}
	if( mDATA.aAnimalNumber == ANIMAL_NUM_BAIKLOK )
	{
		mCritical = (int) ( (float) mCritical * ANIMAL_RATE_TWO_GRADE );
	}
	if( mDATA.aAnimalNumber == ANIMAL_NUM_SHINLOK )
	{
		mCritical = (int) ( (float) mCritical * ANIMAL_RATE_THREE_GRADE );
	}
#endif

	mCriticalDefense = 0;
	for( index01 = 0 ; index01 < MAX_EQUIP_SLOT_NUM ; index01++ )
	{
		if( mEquipItem[index01] == NULL )
		{
			continue;
		}
		mCriticalDefense += mEquipItem[index01]->iCapeInfo[1];
	}
	mLastAttackBonusProbability = 0;
	for( index01 = 0 ; index01 < MAX_EQUIP_SLOT_NUM ; index01++ )
	{
		if( mEquipItem[index01] == NULL )
		{
			continue;
		}
		mLastAttackBonusProbability += mEquipItem[index01]->iLastAttackBonusInfo[0];
	}
	mLastAttackBonusValue = 0.0f;
	for( index01 = 0 ; index01 < MAX_EQUIP_SLOT_NUM ; index01++ )
	{
		if( mEquipItem[index01] == NULL )
		{
			continue;
		}
		mLastAttackBonusValue += (float) mEquipItem[index01]->iLastAttackBonusInfo[1];
	}
	mLastAttackBonusValue *= 0.01f;
	mLuck = 0;
	for( index01 = 0 ; index01 < MAX_EQUIP_SLOT_NUM ; index01++ )
	{
		if( mEquipItem[index01] == NULL )
		{
			continue;
		}
		mLuck += mEquipItem[index01]->iLuck;
		switch( index01 )
		{
		case 0 :
			tISValue = mUTIL.ReturnISValueWithLevelLimit( tAvatarInfo->aLevel1, mUTIL.ReturnISValue( mEquipValue[index01] ) );
			if( tISValue > 0 )
			{
				mLuck += ( tISValue * 12 );
			}
			break;
		}
	}
}
//GET_WEAPON_CLASS
int AVATAR_OBJECT::GetWeaponClass( void )
{
	if( mEquipItem[7] == NULL )
	{
		return 0;
	}
	switch( mEquipItem[7]->iSort )
	{
	case 13 : return 1;
	case 14 : return 2;
	case 15 : return 3;
	case 16 : return 1;
	case 17 : return 2;
	case 18 : return 3;
	case 19 : return 1;
	case 20 : return 2;
	case 21 : return 3;
	}
	return 0;
}
//GET_MAX_LIFE
int AVATAR_OBJECT::GetMaxLife( void )
{
	return mMaxLife;
}
//GET_MAX_MANA
int AVATAR_OBJECT::GetMaxMana( void )
{
	return mMaxMana;
}
//GET_ATTACK_POWER
int AVATAR_OBJECT::GetAttackPower( void )
{
	int tValue = mAttackPower;

	if( mUSER[mIndex].mEffectValue[0][0] > 0 )
	{
		tValue = (int) ( (float) ( tValue * ( mUSER[mIndex].mEffectValue[0][0] + 100 ) ) * 0.01f );
	}
	if( tValue < (int) mPAT.ReturnAttackPower( mUSER[mIndex].mAvatarInfo.aEquip[8][0], mUSER[mIndex].mAvatarInfo.aEquip[8][2], mUSER[mIndex].mAvatarInfo.aEquip[8][1] ) )
	{
		tValue += tValue;
	}
	else
	{
		tValue += ( (int) mPAT.ReturnAttackPower( mUSER[mIndex].mAvatarInfo.aEquip[8][0], mUSER[mIndex].mAvatarInfo.aEquip[8][2], mUSER[mIndex].mAvatarInfo.aEquip[8][1] ) );
	}
	return tValue;
}
//GET_DEFENSE_POWER
int AVATAR_OBJECT::GetDefensePower( void )
{
	int tValue = mDefensePower;

	if( mUSER[mIndex].mEffectValue[1][0] > 0 )
	{
		tValue = (int) ( (float) ( tValue * ( mUSER[mIndex].mEffectValue[1][0] + 100 ) ) * 0.01f );
	}
	if( tValue < (int) mPAT.ReturnDefensePower( mUSER[mIndex].mAvatarInfo.aEquip[8][0], mUSER[mIndex].mAvatarInfo.aEquip[8][2], mUSER[mIndex].mAvatarInfo.aEquip[8][1] ) )
	{
		tValue += tValue;
	}
	else
	{
		tValue += ( (int) mPAT.ReturnDefensePower( mUSER[mIndex].mAvatarInfo.aEquip[8][0], mUSER[mIndex].mAvatarInfo.aEquip[8][2], mUSER[mIndex].mAvatarInfo.aEquip[8][1] ) );
	}
	return tValue;
}
//GET_ATTACK_SUCCESS
int AVATAR_OBJECT::GetAttackSuccess( void )
{
	int tValue = mAttackSuccess;

	if( mUSER[mIndex].mEffectValue[2][0] > 0 )
	{
		tValue = (int) ( (float) ( tValue * ( mUSER[mIndex].mEffectValue[2][0] + 100 ) ) * 0.01f );
	}
	return tValue;
}
//GET_ATTACK_BLOCK
int AVATAR_OBJECT::GetAttackBlock( void )
{
	int tValue = mAttackBlock;

	if( mUSER[mIndex].mEffectValue[3][0] > 0 )
	{
		tValue = (int) ( (float) ( tValue * ( mUSER[mIndex].mEffectValue[3][0] + 100 ) ) * 0.01f );
	}
	return tValue;
}
//GET_ELEMENT_ATTACK_POWER
int AVATAR_OBJECT::GetElementAttackPower( void )
{
	int tValue = mElementAttackPower;

	if( mUSER[mIndex].mEffectValue[4][0] > 0 )
	{
		tValue = (int) ( (float) ( tValue * ( mUSER[mIndex].mEffectValue[4][0] + 100 ) ) * 0.01f );
	}
	return tValue;
}
//GET_ELEMENT_DEFENSE_POWER
int AVATAR_OBJECT::GetElementDefensePower( void )
{
	int tValue = mElementDefensePower;

	if( mUSER[mIndex].mEffectValue[5][0] > 0 )
	{
		tValue = (int) ( (float) ( tValue * ( mUSER[mIndex].mEffectValue[5][0] + 100 ) ) * 0.01f );
	}
	return tValue;
}
//GET_CRITICAL
int AVATAR_OBJECT::GetCritical( void )
{
	int tValue = mCritical;

	if( mUSER[mIndex].mEffectValue[10][0] > 0 )
	{
		tValue = (int) ( (float) ( tValue * ( mUSER[mIndex].mEffectValue[10][0] + 100 ) ) * 0.01f );
	}
    // # Defense Hack # {
    if(tValue > 71) {
        mGAMELOG.GL_659_AUTO_CHECK(mIndex, 3, (float)tValue);
        tValue = 0;
    }
    // # }
	return tValue;
}
//GET_CRITICAL_DEFENSE
int AVATAR_OBJECT::GetCriticalDefense( void )
{
	return mCriticalDefense;
}
//GET_LAST_ATTACK_BONUS_PROBABILITY
int AVATAR_OBJECT::GetLastAttackBonusProbability( void )
{
	return mLastAttackBonusProbability;
}
//GET_LAST_ATTACK_BONUS_VALUE
float AVATAR_OBJECT::GetLastAttackBonusValue( void )
{
	return mLastAttackBonusValue;
}
//GET_LUCK
int AVATAR_OBJECT::GetLuck( void )
{
	int tValue = mLuck;

	if( mUSER[mIndex].mEffectValue[11][0] > 0 )
	{
		tValue = (int) ( (float) ( tValue * ( mUSER[mIndex].mEffectValue[11][0] + 100 ) ) * 0.01f );
	}
	return tValue;
}
//GET_BONUS_SKILL_VALUE
int AVATAR_OBJECT::GetBonusSkillValue( int sIndex )
{
	int index01;
	int tValue = 0;

	for( index01 = 0 ; index01 < MAX_EQUIP_SLOT_NUM ; index01++ )
	{
		if( mEquipItem[index01] == NULL )
		{
			continue;
		}
		if( mEquipItem[index01]->iBonusSkillInfo[0][0] == sIndex )
		{
			tValue += mEquipItem[index01]->iBonusSkillInfo[0][1];
		}
		if( mEquipItem[index01]->iBonusSkillInfo[1][0] == sIndex )
		{
			tValue += mEquipItem[index01]->iBonusSkillInfo[1][1];
		}
		if( mEquipItem[index01]->iBonusSkillInfo[2][0] == sIndex )
		{
			tValue += mEquipItem[index01]->iBonusSkillInfo[2][1];
		}
		if( mEquipItem[index01]->iBonusSkillInfo[3][0] == sIndex )
		{
			tValue += mEquipItem[index01]->iBonusSkillInfo[3][1];
		}
		if( mEquipItem[index01]->iBonusSkillInfo[4][0] == sIndex )
		{
			tValue += mEquipItem[index01]->iBonusSkillInfo[4][1];
		}
#ifdef __GOD__
		if( mEquipItem[index01]->iBonusSkillInfo[5][0] == sIndex )
		{
			tValue += mEquipItem[index01]->iBonusSkillInfo[5][1];
		}
		if( mEquipItem[index01]->iBonusSkillInfo[6][0] == sIndex )
		{
			tValue += mEquipItem[index01]->iBonusSkillInfo[6][1];
		}
		if( mEquipItem[index01]->iBonusSkillInfo[7][0] == sIndex )
		{
			tValue += mEquipItem[index01]->iBonusSkillInfo[7][1];
		}
#endif
		tValue += mEquipItem[index01]->iCapeInfo[2];
	}
	return tValue;
}
//GET_REDUCE_MANA_RATIO
int AVATAR_OBJECT::GetReduceManaRatio( void )
{
	int index01;
	int tValue = 0;

	for( index01 = 0 ; index01 < MAX_EQUIP_SLOT_NUM ; index01++ )
	{
		if( mEquipItem[index01] == NULL )
		{
			continue;
		}
		tValue += mEquipItem[index01]->iCapeInfo[0];
	}
	return tValue;
}
//CHECK_COMMUNITY_WORK
BOOL AVATAR_OBJECT::CheckCommunityWork( void )
{
	if( mDATA.aPShopState != 0 )
	{
		return TRUE;
	}
	if( mDuelProcessState != 0 )
	{
		return TRUE;
	}
	if( mTradeProcessState != 0 )
	{
		return TRUE;
	}
	if( mFriendProcessState != 0 )
	{
		return TRUE;
	}
	if( mPartyProcessState != 0 )
	{
		return TRUE;
	}
	if( mGuildProcessState != 0 )
	{
		return TRUE;
	}
	if( mTeacherProcessState != 0 )
	{
		return TRUE;
	}
	return FALSE;
}
//UPDATE
void AVATAR_OBJECT::Update( float tPostTime )
{
	if(!mCheckValidState) {
		return;
	}
	if((mUSER[mIndex].mMoveZoneResult == 1 )) {
		mUSER[mIndex].mHeartCheckCount = 0;
		mUSER[mIndex].mHeartCheckTime = ::time(NULL);
		return;
	}


	int index01;
	int tMaxAvatarLife;
	int tRecoverAvatarLife;
	int tMaxAvatarMana;
	int tRecoverAvatarMana;
	float tLifeRecoverTime;
	float tManaRecoverTime;
	int tEffectValueState[MAX_AVATAR_EFFECT_SORT_NUM];
	BOOL tCheckChangeEffectValue;
	int tOtherAvatarIndex;
	int iTemp_Auto_time;

	//SVR_TRACE();
	//LOG_TO_FILE_1("mUSER[mIndex].uUserSort : %d\n", mUSER[mIndex].uUserSort);
	if(mUSER[mIndex].mAuthInfo.AuthType == 0)
	{
		if( ( mGAME.mCheckZone049TypeServer ) || ( mGAME.mCheckZone051TypeServer ) || ( mGAME.mCheckZone053TypeServer ) )
		{
			if( ( mDATA.aAction.aLocation[0] < mWORLD.mQuadtree[0].mBoxMin[0] ) || ( mDATA.aAction.aLocation[0] > mWORLD.mQuadtree[0].mBoxMax[0] ) || ( mDATA.aAction.aLocation[1] < mWORLD.mQuadtree[0].mBoxMin[1] ) || ( mDATA.aAction.aLocation[1] > mWORLD.mQuadtree[0].mBoxMax[1] ) || ( mDATA.aAction.aLocation[2] < mWORLD.mQuadtree[0].mBoxMin[2] ) || ( mDATA.aAction.aLocation[2] > mWORLD.mQuadtree[0].mBoxMax[2] ) )
			{

				//night_walker
				//mUSER[mIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
				//return;
			}
		}
	}
	//LOG_TO_FILE_1("mUSER[mIndex].uUserSortp : %d\n", mDATA.aPShopState);
	if( mDATA.aPShopState == 1 )
	{
		if( !mWORK.CheckPossiblePShopRegion( mDATA.aTribe, mSERVER_INFO.mServerNumber, &mDATA.aAction.aLocation[0] ) )
		{
			mUSER[mIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
	}
	//@{ auto quiz
	mWORK.Sphinx_Question_TimeCheck( mIndex  );
	//@}
	//-----//
	//QUEST//
	//-----//
	SummonQuestBoss();
	//-----//
	//-----//
	//-----//
	//LOG_TO_FILE_1("mUSER[mIndex].uUserSortp1 : %d\n", mGAME.mTickCount - mUSER[mIndex].mTickCountFor01Second );

	 /** 2009. 10. 26 : DarkAttack Potion add (wolf) */
    if( ( mAVATAR_OBJECT[mIndex].mDATA.bStateDarkAttack == true ) && ( ( GetTickCount() - mUSER[mIndex].mTickCountForDarkAttack ) > 2000 ) )
    {
        //LOG_TO_FILE_2("![%s] : %s \n", __FUNCTION__, mUSER[mIndex].mAvatarInfo.aName);
        mAVATAR_OBJECT[mIndex].mDATA.bStateDarkAttack = false;
        mTRANSFER.B_AVATAR_CHANGE_INFO_2( 45, mAVATAR_OBJECT[mIndex].mDATA.bStateDarkAttack );
        mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
    }
    /** 2009. 10. 26 : DarkAttack Potion add(wolf) */

	if( ( mGAME.mTickCount - mUSER[mIndex].mTickCountFor01Second ) == 2 )
	{
		mUSER[mIndex].mTickCountFor01Second = mGAME.mTickCount;
	//LOG_TO_FILE_1("mUSER[mIndex].uUserSortp2 : %d\n", mUSER[mIndex].mAutoCheckState);
#ifdef __OLD_AUTO_CHECK__
		switch( mUSER[mIndex].mAutoCheckState )
		{
		case 0 :
			if( mUSER[mIndex].mAvatarInfo.aKillMonsterNum < 1000 )
			{
				break;
			}
			mUSER[mIndex].mAutoCheckState = 1;
			mUSER[mIndex].mAutoCheckAnswer = mGAME.mAutoCheckAnswer2;
			mUSER[mIndex].mAutoCheckTime = GetTickCount();
			mTRANSFER.B_AUTO_CHECK_ASK_SEND( mGAME.mAutoCheckDataSize, &mGAME.mAutoCheckData[0] );
			mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			break;
		case 1 :
			if( ( GetTickCount() - mUSER[mIndex].mAutoCheckTime ) < 60000 )
			{
				break;
			}
			mUSER[mIndex].mAutoCheckState = 0;
			mUSER[mIndex].mAvatarInfo.aKillMonsterNum = 0;
			mPLAYUSER_COM.U_ADD_AUTO_USER_LIST_SEND( &mUSER[mIndex].uID[0], &mUSER[mIndex].mAvatarInfo.aName[0], mSERVER_INFO.mWorldNumber, mSERVER_INFO.mServerNumber );
			mTRANSFER.B_RETURN_TO_AUTO_ZONE();
			mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			break;
		}
#endif
#ifdef __HACKINFO__
		if( ( GetTickCount() - mUSER[mIndex].mHackCheckTime ) > 300000 )
		{
			mUSER[mIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			return;
		}
#endif
		//---------//
		//N-PROTECT//
		//---------//
		/*
		if( mUSER[mIndex].mCheckNPROTECT )
		{
			if( ( GetTickCount() - mUSER[mIndex].mTickCountForNPROTECT ) > 180000 )
			{
				mGAMELOG.GL_642_N_PROTECT( mIndex, 104 );
				mUSER[mIndex].Quit(__FILE__, __FUNCTION__, __LINE__);
			               
			               
			               
				return;
			}
		}
		*/
		//---------//
		//---------//
		//---------//
		if( mDATA.aAction.aSort == 11 )
		{
			mDATA.aAction.aSkillValue--;
			if( mDATA.aAction.aSkillValue < 1 )
			{
				mDATA.aAction.aType = GetWeaponClass() * 2;
				mDATA.aAction.aSort = 1;
				mDATA.aAction.aFrame = 0.0f;
				mTRANSFER.B_AVATAR_ACTION_RECV( mIndex, mUniqueNumber, &mDATA, 1 );
                mUTIL.Broadcast( TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
			}
		}
		tMaxAvatarLife = GetMaxLife();
		tRecoverAvatarLife = 0;
		if( mDATA.aAction.aSort == 31 )
		{
			if( mDATA.aLifeValue < tMaxAvatarLife )
			{
				tLifeRecoverTime = mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 2 );
				if( tLifeRecoverTime > 0.0f )
				{
					tRecoverAvatarLife = (int) ( (float) tMaxAvatarLife / tLifeRecoverTime );
					if( tRecoverAvatarLife < 1 )
					{
						tRecoverAvatarLife = 1;
					}
					else
					{
						if( ( mDATA.aLifeValue + tRecoverAvatarLife ) > tMaxAvatarLife )
						{
							tRecoverAvatarLife = tMaxAvatarLife - mDATA.aLifeValue;
						}
					}
				}
			}
		}
		if( tRecoverAvatarLife > 0 )
		{
			mDATA.aLifeValue += tRecoverAvatarLife;
			mTRANSFER.B_AVATAR_CHANGE_INFO_2( 10, mDATA.aLifeValue );
			mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
		}
		tMaxAvatarMana = GetMaxMana();
		tRecoverAvatarMana = 0;
		if( mDATA.aAction.aSort == 31 )
		{
			if( mDATA.aManaValue < tMaxAvatarMana )
			{
				tManaRecoverTime = mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 3 );
				if( tManaRecoverTime > 0.0f )
				{
					tRecoverAvatarMana = (int) ( (float) tMaxAvatarMana / tManaRecoverTime );
					if( tRecoverAvatarMana < 1 )
					{
						tRecoverAvatarMana = 1;
					}
					else
					{
						if( ( mDATA.aManaValue + tRecoverAvatarMana ) > tMaxAvatarMana )
						{
							tRecoverAvatarMana = tMaxAvatarMana - mDATA.aManaValue;
						}
					}
				}
			}
			
		}
		if( tRecoverAvatarMana > 0 )
		{
			mDATA.aManaValue += tRecoverAvatarMana;
			mTRANSFER.B_AVATAR_CHANGE_INFO_2( 11, mDATA.aManaValue );
			mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
		}
		tCheckChangeEffectValue = FALSE;
		for( index01 = 0 ; index01 < MAX_AVATAR_EFFECT_SORT_NUM ; index01++ )
		{
			tEffectValueState[index01] = 0;
		}
		for( index01 = 0 ; index01 < MAX_AVATAR_EFFECT_SORT_NUM ; index01++ )
		{
			if( mUSER[mIndex].mEffectValue[index01][0] < 1 )
			{
				continue;
			}
			mUSER[mIndex].mEffectValue[index01][1]--;
			if( mUSER[mIndex].mEffectValue[index01][1] <= 0 )
			{
				tCheckChangeEffectValue = TRUE;
				mUSER[mIndex].mEffectValue[index01][0] = 0;
				mUSER[mIndex].mEffectValue[index01][1] = 0;
				mDATA.aEffectValueForView[index01] = 0;
				tEffectValueState[index01] = 2;

				/** 2009. 10. 26 : DarkAttack Potion add (wolf) */
                if( index01 == 15 )
                {
                    mUSER[mIndex].iKindDarkAttack = 0;
                }
                /** 2009. 10. 26 : DarkAttack Potion add (wolf) */

			}
		}
		if( tCheckChangeEffectValue )
		{
			mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO( mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0] );
            mUTIL.Broadcast( TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
		}
		if( mDuelProcessState == 3 )
		{
			if( mUTIL.SearchAvatar( mDuelProcessAvatarName, mIndex ) == -1 )
			{
				mDuelProcessState = 0;
			}
		}
		if(124 != mSERVER_INFO.mServerNumber) {
			if( mDATA.aDuelState[0] == 1 )
			{
				switch( mDATA.aDuelState[2] )
				{
					case 1 :
						tOtherAvatarIndex = mUTIL.SearchAvatar( mDuelProcessAvatarName, mIndex );
						if( tOtherAvatarIndex == -1 )
						{
							mTRANSFER.B_DUEL_END_RECV( 3 );
							mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
							mDATA.aDuelState[0] = 0;
							mDATA.aDuelState[1] = 0;
							mDATA.aDuelState[2] = 0;
							mCheckPossibleEatPotion = TRUE;
							mDuelProcessState = 0;
							mTRANSFER.B_AVATAR_CHANGE_INFO_1( mIndex, mUniqueNumber, 7, mDATA.aDuelState[0], mDATA.aDuelState[1], mDATA.aDuelState[2] );
							mUTIL.Broadcast( TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
							break;
						}
						if( mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aLifeValue < 1 )
						{
							mTRANSFER.B_DUEL_END_RECV( 1 );
							mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
							mTRANSFER.B_DUEL_END_RECV( 1 );
							mUSER[tOtherAvatarIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
							mDATA.aDuelState[0] = 0;
							mDATA.aDuelState[1] = 0;
							mDATA.aDuelState[2] = 0;
							mCheckPossibleEatPotion = TRUE;
							mDuelProcessState = 0;
							mTRANSFER.B_AVATAR_CHANGE_INFO_1( mIndex, mUniqueNumber, 7, mDATA.aDuelState[0], mDATA.aDuelState[1], mDATA.aDuelState[2] );
							mUTIL.Broadcast( TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
							mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[0] = 0;
							mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[1] = 0;
							mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[2] = 0;
							mAVATAR_OBJECT[tOtherAvatarIndex].mCheckPossibleEatPotion = TRUE;
							mAVATAR_OBJECT[tOtherAvatarIndex].mDuelProcessState = 0;
							mTRANSFER.B_AVATAR_CHANGE_INFO_1( tOtherAvatarIndex, mAVATAR_OBJECT[tOtherAvatarIndex].mUniqueNumber, 7, mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[0], mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[1], mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[2] );
							mUTIL.Broadcast( TRUE, &mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
							break;
						}
						if( mDATA.aLifeValue < 1 )
						{
							mTRANSFER.B_DUEL_END_RECV( 2 );
							mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
							mTRANSFER.B_DUEL_END_RECV( 2 );
							mUSER[tOtherAvatarIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
							mDATA.aDuelState[0] = 0;
							mDATA.aDuelState[1] = 0;
							mDATA.aDuelState[2] = 0;
							mCheckPossibleEatPotion = TRUE;
							mDuelProcessState = 0;
							mTRANSFER.B_AVATAR_CHANGE_INFO_1( mIndex, mUniqueNumber, 7, mDATA.aDuelState[0], mDATA.aDuelState[1], mDATA.aDuelState[2] );
							mUTIL.Broadcast( TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
							mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[0] = 0;
							mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[1] = 0;
							mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[2] = 0;
							mAVATAR_OBJECT[tOtherAvatarIndex].mCheckPossibleEatPotion = TRUE;
							mAVATAR_OBJECT[tOtherAvatarIndex].mDuelProcessState = 0;
							mTRANSFER.B_AVATAR_CHANGE_INFO_1( tOtherAvatarIndex, mAVATAR_OBJECT[tOtherAvatarIndex].mUniqueNumber, 7, mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[0], mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[1], mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[2] );
							mUTIL.Broadcast( TRUE, &mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
							break;
						}
						if( mDuelProcessRemainTime == 1 )
						{
							mTRANSFER.B_DUEL_END_RECV( 0 );
							mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
							mTRANSFER.B_DUEL_END_RECV( 0 );
							mUSER[tOtherAvatarIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
							mDATA.aDuelState[0] = 0;
							mDATA.aDuelState[1] = 0;
							mDATA.aDuelState[2] = 0;
							mCheckPossibleEatPotion = TRUE;
							mDuelProcessState = 0;
							mTRANSFER.B_AVATAR_CHANGE_INFO_1( mIndex, mUniqueNumber, 7, mDATA.aDuelState[0], mDATA.aDuelState[1], mDATA.aDuelState[2] );
							mUTIL.Broadcast( TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
							mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[0] = 0;
							mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[1] = 0;
							mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[2] = 0;
							mAVATAR_OBJECT[tOtherAvatarIndex].mCheckPossibleEatPotion = TRUE;
							mAVATAR_OBJECT[tOtherAvatarIndex].mDuelProcessState = 0;
							mTRANSFER.B_AVATAR_CHANGE_INFO_1( tOtherAvatarIndex, mAVATAR_OBJECT[tOtherAvatarIndex].mUniqueNumber, 7, mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[0], mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[1], mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aDuelState[2] );
							mUTIL.Broadcast( TRUE, &mAVATAR_OBJECT[tOtherAvatarIndex].mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
							break;
						}
						mDuelProcessRemainTime--;
						mTRANSFER.B_DUEL_TIME_INFO( mDuelProcessRemainTime );
						mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
						mUSER[tOtherAvatarIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
						break;
					case 2 :
						tOtherAvatarIndex = mUTIL.SearchAvatar( mDuelProcessAvatarName, mIndex );
						if( tOtherAvatarIndex == -1 )
						{
							mTRANSFER.B_DUEL_END_RECV( 3 );
							mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
							mDATA.aDuelState[0] = 0;
							mDATA.aDuelState[1] = 0;
							mDATA.aDuelState[2] = 0;
							mCheckPossibleEatPotion = TRUE;
							mDuelProcessState = 0;
							mTRANSFER.B_AVATAR_CHANGE_INFO_1( mIndex, mUniqueNumber, 7, mDATA.aDuelState[0], mDATA.aDuelState[1], mDATA.aDuelState[2] );
							mUTIL.Broadcast( TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
						}
						break;
				}
			}
		}
		else{
			if( mDATA.aLifeValue < 1 ) {
				if(1 == mDATA.aDuelState[0]){
					mGAME.mDuel_124_Pvp = TRUE;
					if(mDATA.aDuelState[2] == 1) {
						mGAME.mDuel_124_AvatarNum[0]--;
						mDATA.aAction.aLocation[0] = -157.0f;
						mDATA.aAction.aLocation[1] = 5.0f;
						mDATA.aAction.aLocation[2] = 1.0f;
#if 0
						mDATA.aAction.aLocation[0] = -232.0f;
						mDATA.aAction.aLocation[1] = 36.0f;
						mDATA.aAction.aLocation[2] = 2.0f;
#endif
					} else if(mDATA.aDuelState[2] == 2){
						mGAME.mDuel_124_AvatarNum[1]--;
						mDATA.aAction.aLocation[0] = 157.0f;
						mDATA.aAction.aLocation[1] = 5.0f;
						mDATA.aAction.aLocation[2] = 1.0f;
#if 0
						mDATA.aAction.aLocation[0] = 232.0f;
						mDATA.aAction.aLocation[1] = 36.0f;
						mDATA.aAction.aLocation[2] = 2.0f;
#endif
					}

					mDATA.aDuelState[0] = 0;
					mDATA.aDuelState[1] = 0;
					mDATA.aDuelState[2] = 0;
					mCheckPossibleEatPotion = TRUE;
					mDuelProcessState = 0;
					mTRANSFER.B_AVATAR_CHANGE_INFO_1( mIndex, mUniqueNumber, 7, mDATA.aDuelState[0], mDATA.aDuelState[1], mDATA.aDuelState[2] );
					mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize);
				}
			}
		}

		if( mTradeProcessState == 3 )
		{
			if( mUTIL.SearchAvatar( mTradeProcessAvatarName, mIndex ) == -1 )
			{
				mTradeProcessState = 0;
			}
		}
		if( mFriendProcessState == 3 )
		{
			if( mUTIL.SearchAvatar( mFriendProcessAvatarName, mIndex ) == -1 )
			{
				mFriendProcessState = 0;
			}
		}
		if( mTeacherProcessState == 3 )
		{
			if( mUTIL.SearchAvatar( mTeacherProcessAvatarName, mIndex ) == -1 )
			{
				mTeacherProcessState = 0;
			}
		}
		if( mPartyProcessState == 3 )
		{
			if( mUTIL.SearchAvatar( mPartyProcessAvatarName, mIndex ) == -1 )
			{
				mPartyProcessState = 0;
			}
		}
		if( mGuildProcessState == 3 )
		{
			if( mUTIL.SearchAvatar( mGuildProcessAvatarName, mIndex ) == -1 )
			{
				mGuildProcessState = 0;
			}
		}
	}
	if( ( mGAME.mTickCount - mUSER[mIndex].mTickCountFor30Second ) == 60 )
	{
		mUSER[mIndex].mTickCountFor30Second = mGAME.mTickCount;
		if( mUSER[mIndex].mAvatarInfo.aEquip[8][1] > 0 )
		{
			mUSER[mIndex].mAvatarInfo.aEquip[8][1]--;
			mTRANSFER.B_AVATAR_CHANGE_INFO_2( 12, mUSER[mIndex].mAvatarInfo.aEquip[8][1] );
			mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
		}
	}

	//-----------//
	//.._...//
	//-----------//
	if( ( mGAME.mTickCount - mUSER[mIndex].mTickCountFor01MinuteForHealth ) == 110 )
	{
		mUSER[mIndex].mTickCountFor01MinuteForHealth = mGAME.mTickCount;
		if( mUSER[mIndex].mCheckHealthState )
		{
			mUSER[mIndex].mAvatarInfo.aPlayTime2++;
			if( mUSER[mIndex].mAvatarInfo.aPlayTime2 > 300 )
			{
				mUSER[mIndex].mAvatarInfo.aPlayTime2 = 300;
			}
			mTRANSFER.B_AVATAR_CHANGE_INFO_2( 16, mUSER[mIndex].mAvatarInfo.aPlayTime2 );
			mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			mTRANSFER.B_AVATAR_CHANGE_INFO_2( 24, mUSER[mIndex].uLoginPremium );
			mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			mUSER[mIndex].mHealthState = mUTIL.ReturnHealthState( mUSER[mIndex].mAvatarInfo.aPlayTime2 );
		}
	}
	//-----------//
	//-----------//
	//-----------//	

	if( ( mGAME.mTickCount - mUSER[mIndex].mTickCountFor01Minute ) == 120 )
	{
		mUSER[mIndex].mTickCountFor01Minute = mGAME.mTickCount;
		mUSER[mIndex].mAvatarInfo.aPlayTime1++;
		++mUSER[mIndex].mAvatarInfo.aPlayTime3;
		mTRANSFER.B_AVATAR_CHANGE_INFO_2( 33, mUSER[mIndex].mAvatarInfo.aPlayTime3 );
		mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
		if(84 == mSERVER_INFO.mServerNumber){
			if(0 < (mUSER[mIndex].mAvatarInfo.aZone84Time & 0x0000000F)){
				--mUSER[mIndex].mAvatarInfo.aZone84Time;
				mTRANSFER.B_AVATAR_CHANGE_INFO_2( 42, mUSER[mIndex].mAvatarInfo.aZone84Time );
				mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			}
		}
		if( ( mSERVER_INFO.mServerNumber != 38 ) && ( mSERVER_INFO.mServerNumber != 37 ) && ( mSERVER_INFO.mServerNumber != 119 ) && ( mSERVER_INFO.mServerNumber != 124 ) )
		{

			int iExpTimePart1 = mUSER[mIndex].mAvatarInfo.aDoubleExpTime1 & 0xFFFF;
			int iExpTimePart2 = mUSER[mIndex].mAvatarInfo.aDoubleExpTime1 & 0xFFFF0000;

			int iDropTimePart1 = mUSER[mIndex].mAvatarInfo.aDoubleDropTime & 0xFFFF;
			int iDropTimePart2 = mUSER[mIndex].mAvatarInfo.aDoubleDropTime & 0xFFFF0000;

			int iPat_Exp_Time = mUSER[ mIndex ].mAvatarInfo.aPat_Exp_x2_Time;
			int iPat_User_Exp_Time1 = mUSER[ mIndex ].mAvatarInfo.aPat_User_Exp_x2_Time & 0xFFFF;
			int iPat_User_Exp_Time2 = mUSER[ mIndex ].mAvatarInfo.aPat_User_Exp_x2_Time & 0xFFFF0000;

			if(112 == mUSER[mIndex].mAvatarInfo.aLevel1){
				if(0 < mUSER[mIndex].mAvatarInfo.aFiveExpTime){
					--mUSER[mIndex].mAvatarInfo.aFiveExpTime;
					mTRANSFER.B_AVATAR_CHANGE_INFO_2( 40, mUSER[mIndex].mAvatarInfo.aFiveExpTime );
					mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
				}
			}

			if((iPat_User_Exp_Time1 == 0) && (iPat_User_Exp_Time2 == 0)) {
				if(iPat_Exp_Time > 0) {
					iPat_Exp_Time--;
					if(iPat_Exp_Time <= 0) {
						mUSER[mIndex].mAvatarInfo.aPat_Exp_x2_Time = 0;
						mTRANSFER.B_AVATAR_CHANGE_INFO_2( 37, mUSER[mIndex].mAvatarInfo.aPat_Exp_x2_Time );
						mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
					} else {
						mUSER[mIndex].mAvatarInfo.aPat_Exp_x2_Time = iPat_Exp_Time;
						mTRANSFER.B_AVATAR_CHANGE_INFO_2( 37, mUSER[mIndex].mAvatarInfo.aPat_Exp_x2_Time );
						mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
					}
				}
			}

			if (mUSER[mIndex].mAvatarInfo.aLevel1 < 113) { // . ... ...
				if(30 >= mUSER[mIndex].mAvatarInfo.aLevel1){
					if(0 < mUSER[mIndex].mAvatarInfo.aDoubleExpTime3){
						--mUSER[mIndex].mAvatarInfo.aDoubleExpTime3;
						mTRANSFER.B_AVATAR_CHANGE_INFO_2( 31, mUSER[mIndex].mAvatarInfo.aDoubleExpTime3 );
						mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
					}
					else if(iExpTimePart1 > 0 )
					{
						iExpTimePart1--;
						mUSER[mIndex].mAvatarInfo.aDoubleExpTime1 = (iExpTimePart2 | iExpTimePart1);
						mTRANSFER.B_AVATAR_CHANGE_INFO_2( 20, mUSER[mIndex].mAvatarInfo.aDoubleExpTime1 );
						mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );

						if (iExpTimePart1 <= 0) {
							mTRANSFER.B_AVATAR_CHANGE_INFO_2( 25, mUSER[mIndex].mAvatarInfo.aDoubleExpTime1 );
							mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
						}
					}
					else if( mUSER[mIndex].mAvatarInfo.aDoubleExpTime2 > 0 )
					{
						mUSER[mIndex].mAvatarInfo.aDoubleExpTime2--;
						mTRANSFER.B_AVATAR_CHANGE_INFO_2( 17, mUSER[mIndex].mAvatarInfo.aDoubleExpTime2 );
						mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
					}
				}
				else{
					if(iExpTimePart1 > 0 )
					{
						iExpTimePart1--;
						mUSER[mIndex].mAvatarInfo.aDoubleExpTime1 = (iExpTimePart2 | iExpTimePart1);
						mTRANSFER.B_AVATAR_CHANGE_INFO_2( 20, mUSER[mIndex].mAvatarInfo.aDoubleExpTime1 );
						mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );

						if (iExpTimePart1 <= 0) {
							mTRANSFER.B_AVATAR_CHANGE_INFO_2( 25, mUSER[mIndex].mAvatarInfo.aDoubleExpTime1 );
							mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
						}
					}
					else if( mUSER[mIndex].mAvatarInfo.aDoubleExpTime2 > 0 )
					{
						mUSER[mIndex].mAvatarInfo.aDoubleExpTime2--;
						mTRANSFER.B_AVATAR_CHANGE_INFO_2( 17, mUSER[mIndex].mAvatarInfo.aDoubleExpTime2 );
						mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
					}
				}

				if(0 < iPat_User_Exp_Time1){
					iPat_User_Exp_Time1--;
					if ( iPat_User_Exp_Time1 <= 0 )
					{
						iPat_User_Exp_Time1 = 0;
						mUSER[mIndex].mAvatarInfo.aPat_User_Exp_x2_Time = (iPat_User_Exp_Time2 | iPat_User_Exp_Time1);
						mTRANSFER.B_AVATAR_CHANGE_INFO_2( 38, mUSER[mIndex].mAvatarInfo.aPat_User_Exp_x2_Time );
						mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
					}
					else
					{
						mUSER[mIndex].mAvatarInfo.aPat_User_Exp_x2_Time = (iPat_User_Exp_Time2 | iPat_User_Exp_Time1);
						mTRANSFER.B_AVATAR_CHANGE_INFO_2( 38, mUSER[mIndex].mAvatarInfo.aPat_User_Exp_x2_Time );
						mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
					}
				}

				/// ... 2. ././. .. ..
				if( iDropTimePart1 > 0 )
				{
					iDropTimePart1--;
					mUSER[mIndex].mAvatarInfo.aDoubleDropTime = (iDropTimePart2 | iDropTimePart1);
					mTRANSFER.B_AVATAR_CHANGE_INFO_2( 35, mUSER[mIndex].mAvatarInfo.aDoubleDropTime );
					mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );	
				}
			} else { // .. ... ...
				iExpTimePart2 = iExpTimePart2 >> 16;
				iPat_User_Exp_Time2 = iPat_User_Exp_Time2 >> 16;
				if (iExpTimePart2 > 0) {
					iExpTimePart2--;
					mUSER[mIndex].mAvatarInfo.aDoubleExpTime1 = (iExpTimePart2 << 16) | iExpTimePart1;
					mTRANSFER.B_AVATAR_CHANGE_INFO_2 (20, mUSER[mIndex].mAvatarInfo.aDoubleExpTime1);
					mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );

					if (iExpTimePart2 <= 0) {
						mTRANSFER.B_AVATAR_CHANGE_INFO_2( 25, mUSER[mIndex].mAvatarInfo.aDoubleExpTime1 );
						mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
					}
				} else if (mUSER[mIndex].mAvatarInfo.aDoubleExpTime2 > 0 ) {
					mUSER[mIndex].mAvatarInfo.aDoubleExpTime2--;
					mTRANSFER.B_AVATAR_CHANGE_INFO_2( 17, mUSER[mIndex].mAvatarInfo.aDoubleExpTime2 );
					mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
				}
				/// ... 2. (.) .. ..
				iDropTimePart2 = iDropTimePart2 >> 16;
				if (iDropTimePart2 > 0) 
				{
					iDropTimePart2--;
					mUSER[mIndex].mAvatarInfo.aDoubleDropTime = (iDropTimePart2 << 16) | iDropTimePart1;
					mTRANSFER.B_AVATAR_CHANGE_INFO_2 ( 35, mUSER[mIndex].mAvatarInfo.aDoubleDropTime);
					mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
				}

				if (0 < iPat_User_Exp_Time2){
					iPat_User_Exp_Time2--;
					if (iPat_User_Exp_Time2 <= 0)
					{
						iPat_User_Exp_Time2 = 0;
						mUSER[mIndex].mAvatarInfo.aPat_User_Exp_x2_Time = (iPat_User_Exp_Time2 << 16) | iPat_User_Exp_Time1;
						mTRANSFER.B_AVATAR_CHANGE_INFO_2( 38, mUSER[mIndex].mAvatarInfo.aPat_User_Exp_x2_Time );
						mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
					}
					else
					{
						mUSER[mIndex].mAvatarInfo.aPat_User_Exp_x2_Time = (iPat_User_Exp_Time2 << 16) | iPat_User_Exp_Time1;
						mTRANSFER.B_AVATAR_CHANGE_INFO_2 (38, mUSER[mIndex].mAvatarInfo.aPat_User_Exp_x2_Time);
						mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
					}
				}

			}
		}
		iTemp_Auto_time = mGAME.Check_Use_Auto_Day(mIndex);
		if(0 == iTemp_Auto_time){
			mUSER[mIndex].mAvatarInfo.aAutoLifeRatio = 0;
			mUSER[mIndex].mAvatarInfo.aAutoManaRatio = 0;
		}
		mTRANSFER.B_AVATAR_CHANGE_INFO_2( 29, iTemp_Auto_time );
		mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );

		if(mUSER[mIndex].mAuthInfo.AuthType == 0)
		{
			if( mGAME.mCheckZone101TypeServer )
			{
#ifdef __GOD__
				if( mUSER[mIndex].mAvatarInfo.aLevel2 > 0 )
				{
#endif
					if( mUSER[mIndex].mAvatarInfo.aZone101TypeTime > 0 )
					{
						mUSER[mIndex].mAvatarInfo.aZone101TypeTime--;
					}
					else
					{
						mUSER[mIndex].mAvatarInfo.aZone101TypeTime = 0;
					}
					mTRANSFER.B_AVATAR_CHANGE_INFO_2( 18, mUSER[mIndex].mAvatarInfo.aZone101TypeTime );
					mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
#ifdef __GOD__
			}
#endif
			}
			if( mGAME.mCheckZone125Server )
			{
				if( mUSER[mIndex].mAvatarInfo.aZone125TypeTime > 0 )
				{
					mUSER[mIndex].mAvatarInfo.aZone125TypeTime--;
				}
				else
				{
					mUSER[mIndex].mAvatarInfo.aZone125TypeTime = 0;
				}
				mTRANSFER.B_AVATAR_CHANGE_INFO_2( 21, mUSER[mIndex].mAvatarInfo.aZone125TypeTime );
				mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
			}
			if( mGAME.mCheckZone126TypeServer )
			{
				if( mUSER[mIndex].uLoginPremiumPCRoom != 2 )
				{
					if( mUSER[mIndex].mAvatarInfo.aZone126TypeTime > 0 )
					{
						mUSER[mIndex].mAvatarInfo.aZone126TypeTime--;
					}
					else
					{
						mUSER[mIndex].mAvatarInfo.aZone126TypeTime = 0;
					}
					mTRANSFER.B_AVATAR_CHANGE_INFO_2( 22, mUSER[mIndex].mAvatarInfo.aZone126TypeTime );
					mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
				}
			}
		}
	}




	if( ( mGAME.mTickCount - mUSER[mIndex].mTickCountFor01Minute_2 ) == 120 )
	{
		mUSER[mIndex].mTickCountFor01Minute_2 = mGAME.mTickCount;
#ifdef __ANIMAL__
		
		if( ( mUSER[mIndex].mAvatarInfo.aAnimalIndex >= 0 ) 
			&& ( mUSER[mIndex].mAvatarInfo.aAnimalIndex <= ( MAX_AVATAR_ANIMAL_NUM + MAX_AVATAR_ANIMAL_NUM - 1 ) ) 
			&& (mAVATAR_OBJECT[mIndex].mDATA.aAnimalNumber != 0))
		{
			if( mUSER[mIndex].mAvatarInfo.aAnimalTime > 0 )
			{
				mUSER[mIndex].mAvatarInfo.aAnimalTime--;
			}
			else
			{
				mUSER[mIndex].mAvatarInfo.aAnimalTime = 0;
			}
			mTRANSFER.B_AVATAR_CHANGE_INFO_2( 30, mUSER[mIndex].mAvatarInfo.aAnimalTime );
			mUSER[mIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
		}
#endif
		//LOG_TO_BOTH_3("!Gold : %d, Silver : %d Use : %d\n", mUSER[mIndex].mAvatarInfo.iGoldOrnamentRemainsTime, mUSER[mIndex].mAvatarInfo.iSilverOrnamentRemainsTime, mUSER[mIndex].mAvatarInfo.iUseOrnament );

		/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */
		// 장식 해방 시스템을 사용 중인 경우 잔여 시간 감소.
		if (mUSER[mIndex].mAvatarInfo.iUseOrnament == 1) 
		{
			// 금장서 시간을 먼저 깎고 은장서 시간을 깎는다.
			if (IsUsedOrnament() == 1)
			{
				mUSER[mIndex].mAvatarInfo.iGoldOrnamentRemainsTime--;
				mTRANSFER.B_AVATAR_CHANGE_INFO_2 (62, mUSER[mIndex].mAvatarInfo.iGoldOrnamentRemainsTime);
			}
			else if (IsUsedOrnament() == 2)
			{
				mUSER[mIndex].mAvatarInfo.iSilverOrnamentRemainsTime--;
				mTRANSFER.B_AVATAR_CHANGE_INFO_2 (63, mUSER[mIndex].mAvatarInfo.iSilverOrnamentRemainsTime);
			}

			// 잔여 시간을 다 소모한 경우 장식 해방 시스템이 적용되지 않도록 한다.
			// 금장서 및 은장서 시간을 모두 체크한다.
			if (mUSER[mIndex].mAvatarInfo.iSilverOrnamentRemainsTime <= 0 && mUSER[mIndex].mAvatarInfo.iGoldOrnamentRemainsTime <= 0) 
			{
				mUSER[mIndex].mAvatarInfo.iUseOrnament = 0;
			}

			//mTRANSFER.B_AVATAR_CHANGE_INFO_2 (46, mUSER[mIndex].mAvatarInfo.iSilverOrnamentRemainsTime);
			mUSER[mIndex].Send (TRUE, reinterpret_cast<char*> (mTRANSFER.mOriginal), mTRANSFER.mOriginalSize);

			SetBasicAbilityFromEquip ();

			// ?????????
			//if (mDATA.aLifeValue > mDATA.aMaxLifeValue)
			//    mDATA.aLifeValue = mDATA.aMaxLifeValue;
			//if (mDATA.aManaValue > mDATA.aMaxManValue)
			//    mDATA.aManaValue = mDATA.aMaxManaValue;
		}
		/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */
	}


}
//PROCESS_FOR_CREATE_EFFECT_VALUE
void AVATAR_OBJECT::ProcessForCreateEffectValue( ACTION_INFO *tAction )
{
	if( ( mDATA.aAction.aSkillNumber != tAction->aSkillNumber ) || ( mDATA.aAction.aSkillGradeNum1 != tAction->aSkillGradeNum1 ) ||	( mDATA.aAction.aSkillGradeNum2 != tAction->aSkillGradeNum2 ) )
	{
		return;
	}
    if (tAction->aSkillNumber != 0)
    {
    	// 동행무공은 검사에서 제외.
        if ((tAction->aSkillNumber != 76) && (tAction->aSkillNumber != 77) && (tAction->aSkillNumber != 78) && (tAction->aSkillNumber != 79) && (tAction->aSkillNumber != 80) && (tAction->aSkillNumber != 81))
        {
            if ( (tAction->aSkillGradeNum1 > mUSER[mIndex].GetMaxSkillGradeNum(tAction->aSkillNumber)) || (tAction->aSkillGradeNum2 > this->GetBonusSkillValue(tAction->aSkillNumber)) )
            {
                return ;
            }
        }
    }

	int index01;
	int tEffectValueState[MAX_AVATAR_EFFECT_SORT_NUM];
	AVATAR_OBJECT *tAVATAR_OBJECT;
	int tRecoverLifeValue;
	int tRecoverManaValue;

	for( index01 = 0 ; index01 < MAX_AVATAR_EFFECT_SORT_NUM ; index01++ )
	{
		tEffectValueState[index01] = 0;
	}
	switch( mDATA.aAction.aSkillNumber )
	{
	case   6 : //[축경]
	case  25 :
	case  44 :
		mUSER[mIndex].mEffectValue[8][0] = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 11 );
		mUSER[mIndex].mEffectValue[8][1] = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 10 ) * mUSER[mIndex].mSupportSkillTimeUpRatio;
		mDATA.aEffectValueForView[8] = mUSER[mIndex].mEffectValue[ 8][0];
		tEffectValueState[8] = 1;
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO( mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0] );
        mUTIL.Broadcast( TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
		mDATA.aAction.aSkillNumber = 0;
		return;
	case   7 : //[속성공격력],[공격속도]
	case  26 :
	case  45 :
		mUSER[mIndex].mEffectValue[4][0] = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 16 );
		mUSER[mIndex].mEffectValue[4][1] = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 10 ) * mUSER[mIndex].mSupportSkillTimeUpRatio;
		mDATA.aEffectValueForView[4] = mUSER[mIndex].mEffectValue[4][0];
		tEffectValueState[4] = 1;
		mUSER[mIndex].mEffectValue[6][0] = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 18 );
		mUSER[mIndex].mEffectValue[6][1] = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 10 ) * mUSER[mIndex].mSupportSkillTimeUpRatio;
		mDATA.aEffectValueForView[6] = mUSER[mIndex].mEffectValue[6][0];
		tEffectValueState[6] = 1;
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO( mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0] );
        mUTIL.Broadcast( TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
		mDATA.aAction.aSkillNumber = 0;
		return;
	case  11 : //[방어력]
	case  34 :
	case  49 :
		if( ( mEquipItem[7] == NULL ) || ( ( mEquipItem[7]->iSort != 13 ) && ( mEquipItem[7]->iSort != 17 ) && ( mEquipItem[7]->iSort != 19 ) ) )
		{
			return;
		}
		mUSER[mIndex].mEffectValue[1][0] = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 13 );
		mUSER[mIndex].mEffectValue[1][1] = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 10 ) * mUSER[mIndex].mSupportSkillTimeUpRatio;
		mDATA.aEffectValueForView[1] = mUSER[mIndex].mEffectValue[1][0];
		tEffectValueState[1] = 1;
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO( mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0] );
        mUTIL.Broadcast( TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
		mDATA.aAction.aSkillNumber = 0;
		return;
	case  15 : //[공격력]
	case  30 :
	case  53 :
		if( ( mEquipItem[7] == NULL ) || ( ( mEquipItem[7]->iSort != 14 ) && ( mEquipItem[7]->iSort != 16 ) && ( mEquipItem[7]->iSort != 20 ) ) )
		{
			return;
		}
		mUSER[mIndex].mEffectValue[0][0] = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 12 );
		mUSER[mIndex].mEffectValue[0][1] = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 10 ) * mUSER[mIndex].mSupportSkillTimeUpRatio;
		mDATA.aEffectValueForView[0] = mUSER[mIndex].mEffectValue[0][0];
		tEffectValueState[0] = 1;
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO( mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0] );
        mUTIL.Broadcast( TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
		mDATA.aAction.aSkillNumber = 0;
		return;
	case  19 : //[이동속도],[공격회피]
	case  38 :
	case  57 :
		if( ( mEquipItem[7] == NULL ) || ( ( mEquipItem[7]->iSort != 15 ) && ( mEquipItem[7]->iSort != 18 ) && ( mEquipItem[7]->iSort != 21 ) ) )
		{
			return;
		}
		mUSER[mIndex].mEffectValue[3][0] = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 15 );
		mUSER[mIndex].mEffectValue[3][1] = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 10 ) * mUSER[mIndex].mSupportSkillTimeUpRatio;
		mDATA.aEffectValueForView[3] = mUSER[mIndex].mEffectValue[3][0];
		tEffectValueState[3] = 1;
		mUSER[mIndex].mEffectValue[7][0] = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 19 );
		mUSER[mIndex].mEffectValue[7][1] = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 10 ) * mUSER[mIndex].mSupportSkillTimeUpRatio;
		mDATA.aEffectValueForView[7] = mUSER[mIndex].mEffectValue[7][0];
		tEffectValueState[7] = 1;
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO( mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0] );
        mUTIL.Broadcast( TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
		mDATA.aAction.aSkillNumber = 0;
		return;
	case  82 : //[호신강기]
		mUSER[mIndex].mEffectValue[9][0] = (int) ( mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 20 ) * (float) GetMaxLife() * 0.01f );
		mUSER[mIndex].mEffectValue[9][1] = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 10 ) * mUSER[mIndex].mSupportSkillTimeUpRatio;
		mDATA.aEffectValueForView[9] = mUSER[mIndex].mEffectValue[9][0];
		tEffectValueState[9] = 1;
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO( mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0] );
        mUTIL.Broadcast( TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
		mDATA.aAction.aSkillNumber = 0;
		return;
	case  83 : //[치명]
		mUSER[mIndex].mEffectValue[10][0] = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 22 );
		mUSER[mIndex].mEffectValue[10][1] = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 10 ) * mUSER[mIndex].mSupportSkillTimeUpRatio;
		mDATA.aEffectValueForView[10] = mUSER[mIndex].mEffectValue[10][0];
		tEffectValueState[10] = 1;
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO( mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0] );
        mUTIL.Broadcast( TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
		mDATA.aAction.aSkillNumber = 0;
		return;
	case  84 : //[운]
		mUSER[mIndex].mEffectValue[11][0] = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 21 );
		mUSER[mIndex].mEffectValue[11][1] = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 10 ) * mUSER[mIndex].mSupportSkillTimeUpRatio;
		mDATA.aEffectValueForView[11] = mUSER[mIndex].mEffectValue[11][0];
		tEffectValueState[11] = 1;
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO( mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0] );
        mUTIL.Broadcast( TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
		mDATA.aAction.aSkillNumber = 0;
		return;
	case 106 : //[체력채우기]
	case 108 :
	case 110 :
		if( ( mIndex == mDATA.aAction.aTargetObjectIndex ) && ( mUniqueNumber == mDATA.aAction.aTargetObjectUniqueNumber ) )
		{
			mDATA.aAction.aSkillNumber = 0;
			return;
		}
		if( ( mDATA.aAction.aTargetObjectIndex < 0 ) || ( mDATA.aAction.aTargetObjectIndex > ( mSERVER.mMAX_USER_NUM - 1 ) ) )
		{
			mDATA.aAction.aSkillNumber = 0;
			return;
		}
		tAVATAR_OBJECT = &mAVATAR_OBJECT[mDATA.aAction.aTargetObjectIndex];
		if( ( !tAVATAR_OBJECT->mCheckValidState ) || ( tAVATAR_OBJECT->mUniqueNumber != mDATA.aAction.aTargetObjectUniqueNumber ) )
		{
			mDATA.aAction.aSkillNumber = 0;
			return;
		}
		if( tAVATAR_OBJECT->mDATA.aLifeValue < 1 )
		{
			mDATA.aAction.aSkillNumber = 0;
			return;
		}
		if( tAVATAR_OBJECT->mDATA.aPShopState == 1 )
		{
			mDATA.aAction.aSkillNumber = 0;
			return;
		}
		if( !mGAME.CheckPossibleAttackTarget( 1, tAVATAR_OBJECT->mDATA.aAction.aSort ) )
		{
			mDATA.aAction.aSkillNumber = 0;
			return;
		}
		if( tAVATAR_OBJECT->mDATA.aLifeValue >= tAVATAR_OBJECT->GetMaxLife() )
		{
			mDATA.aAction.aSkillNumber = 0;
			return;
		}
		tRecoverLifeValue = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 2 );
		if( ( tAVATAR_OBJECT->mDATA.aLifeValue + tRecoverLifeValue ) > tAVATAR_OBJECT->GetMaxLife() )
		{
			tRecoverLifeValue = tAVATAR_OBJECT->GetMaxLife() - tAVATAR_OBJECT->mDATA.aLifeValue;
		}
		if( tRecoverLifeValue < 1 )
		{
			mDATA.aAction.aSkillNumber = 0;
			return;
		}
		tAVATAR_OBJECT->mDATA.aLifeValue += tRecoverLifeValue;
		mTRANSFER.B_AVATAR_CHANGE_INFO_1( mDATA.aAction.aTargetObjectIndex, mDATA.aAction.aTargetObjectUniqueNumber, 8, tRecoverLifeValue, 0, 0 );
        mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
		mDATA.aAction.aSkillNumber = 0;
		return;
	case 107 : //[공력채우기]
	case 109 :
	case 111 :
		if( ( mIndex == mDATA.aAction.aTargetObjectIndex ) && ( mUniqueNumber == mDATA.aAction.aTargetObjectUniqueNumber ) )
		{
			mDATA.aAction.aSkillNumber = 0;
			return;
		}
		if( ( mDATA.aAction.aTargetObjectIndex < 0 ) || ( mDATA.aAction.aTargetObjectIndex > ( mSERVER.mMAX_USER_NUM - 1 ) ) )
		{
			mDATA.aAction.aSkillNumber = 0;
			return;
		}
		tAVATAR_OBJECT = &mAVATAR_OBJECT[mDATA.aAction.aTargetObjectIndex];
		if( ( !tAVATAR_OBJECT->mCheckValidState ) || ( tAVATAR_OBJECT->mUniqueNumber != mDATA.aAction.aTargetObjectUniqueNumber ) )
		{
			mDATA.aAction.aSkillNumber = 0;
			return;
		}
		if( tAVATAR_OBJECT->mDATA.aLifeValue < 1 )
		{
			mDATA.aAction.aSkillNumber = 0;
			return;
		}
		if( tAVATAR_OBJECT->mDATA.aPShopState == 1 )
		{
			mDATA.aAction.aSkillNumber = 0;
			return;
		}
		if( !mGAME.CheckPossibleAttackTarget( 1, tAVATAR_OBJECT->mDATA.aAction.aSort ) )
		{
			mDATA.aAction.aSkillNumber = 0;
			return;
		}
		if( tAVATAR_OBJECT->mDATA.aManaValue >= tAVATAR_OBJECT->GetMaxMana() )
		{
			mDATA.aAction.aSkillNumber = 0;
			return;
		}
		tRecoverManaValue = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 3 );
		if( ( tAVATAR_OBJECT->mDATA.aManaValue + tRecoverManaValue ) > tAVATAR_OBJECT->GetMaxMana() )
		{
			tRecoverManaValue = tAVATAR_OBJECT->GetMaxMana() - tAVATAR_OBJECT->mDATA.aManaValue;
		}
		if( tRecoverManaValue < 1 )
		{
			mDATA.aAction.aSkillNumber = 0;
			return;
		}
		tAVATAR_OBJECT->mDATA.aManaValue += tRecoverManaValue;
		mTRANSFER.B_AVATAR_CHANGE_INFO_1( mDATA.aAction.aTargetObjectIndex, mDATA.aAction.aTargetObjectUniqueNumber, 9, tRecoverManaValue, 0, 0 );
        mUTIL.Broadcast( TRUE, &tAVATAR_OBJECT->mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
		mDATA.aAction.aSkillNumber = 0;
		return;
	case 103 : //[대극방탄세]
		mUSER[mIndex].mEffectValue[12][0] = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 23 );
		mUSER[mIndex].mEffectValue[12][1] = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 10 ) * mUSER[mIndex].mSupportSkillTimeUpRatio;
		mDATA.aEffectValueForView[12] = mUSER[mIndex].mEffectValue[12][0];
		tEffectValueState[12] = 1;
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO( mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0] );
        mUTIL.Broadcast( TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
		mDATA.aAction.aSkillNumber = 0;
		return;
	case 104 : //[천문역근공]
		mUSER[mIndex].mEffectValue[13][0] = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 24 );
		mUSER[mIndex].mEffectValue[13][1] = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 10 ) * mUSER[mIndex].mSupportSkillTimeUpRatio;
		mDATA.aEffectValueForView[13] = mUSER[mIndex].mEffectValue[13][0];
		tEffectValueState[13] = 1;
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO( mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0] );
        mUTIL.Broadcast( TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
		mDATA.aAction.aSkillNumber = 0;
		return;
	case 105 : //[탈망소혼역]
		mUSER[mIndex].mEffectValue[14][0] = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 25 );
		mUSER[mIndex].mEffectValue[14][1] = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 10 ) * mUSER[mIndex].mSupportSkillTimeUpRatio;
		mDATA.aEffectValueForView[14] = mUSER[mIndex].mEffectValue[14][0];
		tEffectValueState[14] = 1;
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO( mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0] );
        mUTIL.Broadcast( TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
		mDATA.aAction.aSkillNumber = 0;
		return;
	case  76 : //[오성연환진-공격성공상승]
		mUSER[mIndex].mEffectValue[2][0] = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 14 );
		mUSER[mIndex].mEffectValue[2][1] = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 10 );
		mDATA.aEffectValueForView[2] = mUSER[mIndex].mEffectValue[2][0];
		tEffectValueState[2] = 1;
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO( mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0] );
        mUTIL.Broadcast( TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
		mDATA.aAction.aSkillNumber = 0;
		return;
	case  77 : //[오성환원진-공격회피상승]
		mUSER[mIndex].mEffectValue[3][0] = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 15 );
		mUSER[mIndex].mEffectValue[3][1] = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 10 );
		mDATA.aEffectValueForView[3] = mUSER[mIndex].mEffectValue[3][0];
		tEffectValueState[3] = 1;
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO( mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0] );
        mUTIL.Broadcast( TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
		mDATA.aAction.aSkillNumber = 0;
		return;
	case  79 : //[천왕호각진-호신강기]
		mUSER[mIndex].mEffectValue[9][0] = (int) ( mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 20 ) * (float) GetMaxLife() * 0.01f );
		mUSER[mIndex].mEffectValue[9][1] = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 10 );
		mDATA.aEffectValueForView[9] = mUSER[mIndex].mEffectValue[9][0];
		tEffectValueState[9] = 1;
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO( mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0] );
        mUTIL.Broadcast( TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
		mDATA.aAction.aSkillNumber = 0;
		return;
	case  81 : //[무극천살진-치명타상승]
		mUSER[mIndex].mEffectValue[10][0] = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 22 );
		mUSER[mIndex].mEffectValue[10][1] = (int) mSKILL.ReturnSkillValue( mDATA.aAction.aSkillNumber, ( mDATA.aAction.aSkillGradeNum1 + mDATA.aAction.aSkillGradeNum2 ), 10 );
		mDATA.aEffectValueForView[10] = mUSER[mIndex].mEffectValue[10][0];
		tEffectValueState[10] = 1;
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO( mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0] );
        mUTIL.Broadcast( TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
		mDATA.aAction.aSkillNumber = 0;
		return;
	}
}
//PROCESS_FOR_DELETE_EFFECT_VALUE
void AVATAR_OBJECT::ProcessForDeleteEffectValue( void )
{
	int index01;
	BOOL tCheckChangeEffectValueState;
	int tEffectValueState[MAX_AVATAR_EFFECT_SORT_NUM];

	tCheckChangeEffectValueState = FALSE;
	for( index01 = 0 ; index01 < MAX_AVATAR_EFFECT_SORT_NUM ; index01++ )
	{
		tEffectValueState[index01] = 0;
	}
	for( index01 = 0 ; index01 < MAX_AVATAR_EFFECT_SORT_NUM ; index01++ )
	{
		if( mUSER[mIndex].mEffectValue[index01][0] < 1 )
		{
			continue;
		}
		tCheckChangeEffectValueState = TRUE;
		mUSER[mIndex].mEffectValue[index01][0] = 0;
		mUSER[mIndex].mEffectValue[index01][1] = 0;
		mDATA.aEffectValueForView[index01] = 0;
		tEffectValueState[index01] = 2;
	}
	if( tCheckChangeEffectValueState )
	{
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO( mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0] );
        mUTIL.Broadcast( TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
	}
}
//PROCESS_FOR_DEATH
void AVATAR_OBJECT::ProcessForDeath( void )
{
	int index01;
    // # Defense Hack # {
    mCheckDeath = true;
    mCheckCount = 9;
    // # }
	BOOL tCheckChangeEffectValueState;
	int tEffectValueState[MAX_AVATAR_EFFECT_SORT_NUM];

	tCheckChangeEffectValueState = FALSE;
	for( index01 = 0 ; index01 < MAX_AVATAR_EFFECT_SORT_NUM ; index01++ )
	{
		tEffectValueState[index01] = 0;
	}
	for( index01 = 0 ; index01 < MAX_AVATAR_EFFECT_SORT_NUM ; index01++ )
	{
		if( mUSER[mIndex].mEffectValue[index01][0] < 1 )
		{
			continue;
		}
		tCheckChangeEffectValueState = TRUE;
		mUSER[mIndex].mEffectValue[index01][0] = 0;
		mUSER[mIndex].mEffectValue[index01][1] = 0;
		mDATA.aEffectValueForView[index01] = 0;
		tEffectValueState[index01] = 2;
	}
	if( tCheckChangeEffectValueState )
	{
		mTRANSFER.B_AVATAR_EFFECT_VALUE_INFO( mIndex, mUniqueNumber, mUSER[mIndex].mEffectValue, &tEffectValueState[0] );
        mUTIL.Broadcast( TRUE, &mDATA.aAction.aLocation[0], UNIT_SCALE_RADIUS1, false );
	}
}
//PROCESS_FOR_TRADE_INFO
void AVATAR_OBJECT::ProcessForTradeInfo( void )
{
	int tOtherAvatarIndex;

	if( mTradeProcessState != 4 )
	{
		return;
	}
	tOtherAvatarIndex = mUTIL.SearchAvatar( mTradeProcessAvatarName, mIndex );
	if( tOtherAvatarIndex == -1 )
	{
		return;
	}
	if( mAVATAR_OBJECT[tOtherAvatarIndex].mTradeProcessState != 4 )
	{
		return;
	}
	if( strcmp( mAVATAR_OBJECT[tOtherAvatarIndex].mTradeProcessAvatarName, mDATA.aName ) != 0 )
	{
		return;
	}
	mTRANSFER.B_TRADE_STATE_RECV( mUSER[mIndex].mAvatarInfo.aTradeMoney, mUSER[mIndex].mAvatarInfo.aTrade );
	mUSER[tOtherAvatarIndex].Send( TRUE, (char *) &mTRANSFER.mOriginal[0], mTRANSFER.mOriginalSize );
}
//RETURN_QUEST_PRESENT_STATE
int AVATAR_OBJECT::ReturnQuestPresentState( void )
{
	int index01;
	int index02;
	QUEST_INFO *tQUEST_INFO;

	if( ( mUSER[mIndex].mAvatarInfo.aQuestInfo[1] == 0 ) && ( mUSER[mIndex].mAvatarInfo.aQuestInfo[2] == 0 ) && ( mUSER[mIndex].mAvatarInfo.aQuestInfo[3] == 0 ) && ( mUSER[mIndex].mAvatarInfo.aQuestInfo[4] == 0 ) )
	{
		tQUEST_INFO = mQUEST.Search( mUSER[mIndex].mAvatarInfo.aTribe, ( mUSER[mIndex].mAvatarInfo.aQuestInfo[0] + 1 ) );
		if( tQUEST_INFO == NULL )
		{
			return 0;
		}
		if( mUSER[mIndex].mAvatarInfo.aLevel1 < tQUEST_INFO->qLevel )
		{
			return 0;
		}
		return 1;
	}
	if( mUSER[mIndex].mAvatarInfo.aQuestInfo[1] == 1 )
	{
		tQUEST_INFO = mQUEST.Search( mUSER[mIndex].mAvatarInfo.aTribe, mUSER[mIndex].mAvatarInfo.aQuestInfo[0] );
		if( tQUEST_INFO == NULL )
		{
			return 0;
		}
		switch( mUSER[mIndex].mAvatarInfo.aQuestInfo[2] )
		{
		case 1 : //[몬스터 죽이기]
			if( mUSER[mIndex].mAvatarInfo.aQuestInfo[3] != tQUEST_INFO->qSolution[0] )
			{
				return 0;
			}
			if( mUSER[mIndex].mAvatarInfo.aQuestInfo[4] < tQUEST_INFO->qSolution[1] )
			{
				return 2;
			}
			return 3;
		case 2 : //[아이템 입수]
			if( mUSER[mIndex].mAvatarInfo.aQuestInfo[3] != tQUEST_INFO->qSolution[0] )
			{
				return 0;
			}
			for( index01 = 0 ; index01 < 2 ; index01++ )
			{
				for( index02 = 0 ; index02 < MAX_INVENTORY_SLOT_NUM ; index02++ )
				{
					if( mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][0] == tQUEST_INFO->qSolution[0] )
					{
						break;
					}
				}
				if( index02 < MAX_INVENTORY_SLOT_NUM )
				{
					break;
				}
			}
			if( index01 == 2 )
			{
				return 2;
			}
			return 3;
		case 3 : //[아이템 전달]
			if( mUSER[mIndex].mAvatarInfo.aQuestInfo[3] != tQUEST_INFO->qSolution[0] )
			{
				return 0;
			}
			for( index01 = 0 ; index01 < 2 ; index01++ )
			{
				for( index02 = 0 ; index02 < MAX_INVENTORY_SLOT_NUM ; index02++ )
				{
					if( mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][0] == tQUEST_INFO->qSolution[0] )
					{
						break;
					}
				}
				if( index02 < MAX_INVENTORY_SLOT_NUM )
				{
					break;
				}
			}
			if( index01 == 2 )
			{
				return 2;
			}
			return 3;
		case 4 : //[아이템 받아오기]
			if( mUSER[mIndex].mAvatarInfo.aQuestInfo[3] != tQUEST_INFO->qSolution[0] )
			{
				return 0;
			}
			for( index01 = 0 ; index01 < 2 ; index01++ )
			{
				for( index02 = 0 ; index02 < MAX_INVENTORY_SLOT_NUM ; index02++ )
				{
					if( mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][0] == tQUEST_INFO->qSolution[0] )
					{
						break;
					}
				}
				if( index02 < MAX_INVENTORY_SLOT_NUM )
				{
					break;
				}
			}
			if( index01 == 2 )
			{
				return 2;
			}
			return 3;
		case 5 : //[대장 죽이기]
			if( mUSER[mIndex].mAvatarInfo.aQuestInfo[3] != tQUEST_INFO->qSolution[0] )
			{
				return 0;
			}
			if( mUSER[mIndex].mAvatarInfo.aQuestInfo[4] < 1 )
			{
				return 2;
			}
			return 3;
		case 6 : //[아이템 교환하기]
			switch( mUSER[mIndex].mAvatarInfo.aQuestInfo[3] )
			{
			case 1 : //[교환 전]
				if( mUSER[mIndex].mAvatarInfo.aQuestInfo[4] != tQUEST_INFO->qSolution[0] )
				{
					return 0;
				}
				for( index01 = 0 ; index01 < 2 ; index01++ )
				{
					for( index02 = 0 ; index02 < MAX_INVENTORY_SLOT_NUM ; index02++ )
					{
						if( mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][0] == tQUEST_INFO->qSolution[0] )
						{
							break;
						}
					}
					if( index02 < MAX_INVENTORY_SLOT_NUM )
					{
						break;
					}
				}
				if( index01 == 2 )
				{
					return 2;
				}
				return 3;
			case 2 : //[교환 후]
				if( mUSER[mIndex].mAvatarInfo.aQuestInfo[4] != tQUEST_INFO->qSolution[1] )
				{
					return 0;
				}
				for( index01 = 0 ; index01 < 2 ; index01++ )
				{
					for( index02 = 0 ; index02 < MAX_INVENTORY_SLOT_NUM ; index02++ )
					{
						if( mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][0] == tQUEST_INFO->qSolution[1] )
						{
							break;
						}
					}
					if( index02 < MAX_INVENTORY_SLOT_NUM )
					{
						break;
					}
				}
				if( index01 == 2 )
				{
					return 4;
				}
				return 5;
			}
			break;
		case 7 : //[NPC 만나기]
			if( mUSER[mIndex].mAvatarInfo.aQuestInfo[3] != tQUEST_INFO->qSolution[0] )
			{
				return 0;
			}
			return 2;
		case 8 : //[진 점령 참여]
			if( mUSER[mIndex].mAvatarInfo.aQuestInfo[3] != tQUEST_INFO->qSolution[0] )
			{
				return 0;
			}
			if( mUSER[mIndex].mAvatarInfo.aQuestInfo[4] < 1 )
			{
				return 2;
			}
			return 3;
		}
	}
	return 0;
}
//RETURN_QUEST_END_CONDITION_STATE
int AVATAR_OBJECT::ReturnQuestEndConditionState( void )
{
	switch( mUSER[mIndex].mAvatarInfo.aQuestInfo[2] )
	{
	case 1 :
		if( ReturnQuestPresentState() == 3 )
		{
			return 1;
		}
		return 0;
	case 2 :
		if( ReturnQuestPresentState() == 3 )
		{
			return 1;
		}
		return 0;
	case 3 :
		if( ReturnQuestPresentState() == 3 )
		{
			return 1;
		}
		return 0;
	case 4 :
		if( ReturnQuestPresentState() == 3 )
		{
			return 1;
		}
		return 0;
	case 5 :
		if( ReturnQuestPresentState() == 3 )
		{
			return 1;
		}
		return 0;
	case 6 :
		if( ReturnQuestPresentState() == 5 )
		{
			return 1;
		}
		return 0;
	case 7 :
		if( ReturnQuestPresentState() == 2 )
		{
			return 1;
		}
		return 0;
	case 8 :
		if( ReturnQuestPresentState() == 3 )
		{
			return 1;
		}
		return 0;
	}
	return 0;
}
//RETURN_QUEST_NEXT_NPC_NUMBER
int AVATAR_OBJECT::ReturnQuestNextNPCNumber( void )
{
	int index01;
	int index02;
	QUEST_INFO *tQUEST_INFO;

	if( ( mUSER[mIndex].mAvatarInfo.aQuestInfo[1] == 0 ) && ( mUSER[mIndex].mAvatarInfo.aQuestInfo[2] == 0 ) && ( mUSER[mIndex].mAvatarInfo.aQuestInfo[3] == 0 ) && ( mUSER[mIndex].mAvatarInfo.aQuestInfo[4] == 0 ) )
	{
		tQUEST_INFO = mQUEST.Search( mUSER[mIndex].mAvatarInfo.aTribe, ( mUSER[mIndex].mAvatarInfo.aQuestInfo[0] + 1 ) );
		if( tQUEST_INFO == NULL )
		{
			return 0;
		}
		if( mUSER[mIndex].mAvatarInfo.aLevel1 < tQUEST_INFO->qLevel )
		{
			return 0;
		}
		return tQUEST_INFO->qStartNPCNumber;
	}
	if( mUSER[mIndex].mAvatarInfo.aQuestInfo[1] == 1 )
	{
		tQUEST_INFO = mQUEST.Search( mUSER[mIndex].mAvatarInfo.aTribe, mUSER[mIndex].mAvatarInfo.aQuestInfo[0] );
		if( tQUEST_INFO == NULL )
		{
			return 0;
		}
		switch( mUSER[mIndex].mAvatarInfo.aQuestInfo[2] )
		{
		case 1 : //[몬스터 죽이기]
			if( mUSER[mIndex].mAvatarInfo.aQuestInfo[3] != tQUEST_INFO->qSolution[0] )
			{
				return 0;
			}
			if( mUSER[mIndex].mAvatarInfo.aQuestInfo[4] < tQUEST_INFO->qSolution[1] )
			{
				return tQUEST_INFO->qStartNPCNumber;
			}
			return tQUEST_INFO->qEndNPCNumber;
		case 2 : //[아이템 입수]
			if( mUSER[mIndex].mAvatarInfo.aQuestInfo[3] != tQUEST_INFO->qSolution[0] )
			{
				return 0;
			}
			for( index01 = 0 ; index01 < 2 ; index01++ )
			{
				for( index02 = 0 ; index02 < MAX_INVENTORY_SLOT_NUM ; index02++ )
				{
					if( mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][0] == tQUEST_INFO->qSolution[0] )
					{
						break;
					}
				}
				if( index02 < MAX_INVENTORY_SLOT_NUM )
				{
					break;
				}
			}
			if( index01 == 2 )
			{
				return tQUEST_INFO->qStartNPCNumber;
			}
			return tQUEST_INFO->qEndNPCNumber;
		case 3 : //[아이템 전달]
			if( mUSER[mIndex].mAvatarInfo.aQuestInfo[3] != tQUEST_INFO->qSolution[0] )
			{
				return 0;
			}
			for( index01 = 0 ; index01 < 2 ; index01++ )
			{
				for( index02 = 0 ; index02 < MAX_INVENTORY_SLOT_NUM ; index02++ )
				{
					if( mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][0] == tQUEST_INFO->qSolution[0] )
					{
						break;
					}
				}
				if( index02 < MAX_INVENTORY_SLOT_NUM )
				{
					break;
				}
			}
			if( index01 == 2 )
			{
				return tQUEST_INFO->qStartNPCNumber;
			}
			return tQUEST_INFO->qEndNPCNumber;
		case 4 : //[아이템 받아오기]
			if( mUSER[mIndex].mAvatarInfo.aQuestInfo[3] != tQUEST_INFO->qSolution[0] )
			{
				return 0;
			}
			for( index01 = 0 ; index01 < 2 ; index01++ )
			{
				for( index02 = 0 ; index02 < MAX_INVENTORY_SLOT_NUM ; index02++ )
				{
					if( mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][0] == tQUEST_INFO->qSolution[0] )
					{
						break;
					}
				}
				if( index02 < MAX_INVENTORY_SLOT_NUM )
				{
					break;
				}
			}
			if( index01 == 2 )
			{
				return tQUEST_INFO->qKeyNPCNumber[0];
			}
			return tQUEST_INFO->qEndNPCNumber;
		case 5 : //[대장 죽이기]
			if( mUSER[mIndex].mAvatarInfo.aQuestInfo[3] != tQUEST_INFO->qSolution[0] )
			{
				return 0;
			}
			if( mUSER[mIndex].mAvatarInfo.aQuestInfo[4] < 1 )
			{
				return tQUEST_INFO->qStartNPCNumber;
			}
			return tQUEST_INFO->qEndNPCNumber;
		case 6 : //[아이템 교환하기]
			switch( mUSER[mIndex].mAvatarInfo.aQuestInfo[3] )
			{
			case 1 : //[교환 전]
				if( mUSER[mIndex].mAvatarInfo.aQuestInfo[4] != tQUEST_INFO->qSolution[0] )
				{
					return 0;
				}
				for( index01 = 0 ; index01 < 2 ; index01++ )
				{
					for( index02 = 0 ; index02 < MAX_INVENTORY_SLOT_NUM ; index02++ )
					{
						if( mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][0] == tQUEST_INFO->qSolution[0] )
						{
							break;
						}
					}
					if( index02 < MAX_INVENTORY_SLOT_NUM )
					{
						break;
					}
				}
				if( index01 == 2 )
				{
					return tQUEST_INFO->qStartNPCNumber;
				}
				return tQUEST_INFO->qKeyNPCNumber[0];
			case 2 : //[교환 후]
				if( mUSER[mIndex].mAvatarInfo.aQuestInfo[4] != tQUEST_INFO->qSolution[1] )
				{
					return 0;
				}
				for( index01 = 0 ; index01 < 2 ; index01++ )
				{
					for( index02 = 0 ; index02 < MAX_INVENTORY_SLOT_NUM ; index02++ )
					{
						if( mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][0] == tQUEST_INFO->qSolution[1] )
						{
							break;
						}
					}
					if( index02 < MAX_INVENTORY_SLOT_NUM )
					{
						break;
					}
				}
				if( index01 == 2 )
				{
					return tQUEST_INFO->qStartNPCNumber;
				}
				return tQUEST_INFO->qEndNPCNumber;
			}
			break;
		case 7 : //[NPC 만나기]
			if( mUSER[mIndex].mAvatarInfo.aQuestInfo[3] != tQUEST_INFO->qSolution[0] )
			{
				return 0;
			}
			return tQUEST_INFO->qEndNPCNumber;
		case 8 : //[진 점령 참여]
			if( mUSER[mIndex].mAvatarInfo.aQuestInfo[3] != tQUEST_INFO->qSolution[0] )
			{
				return 0;
			}
			if( mUSER[mIndex].mAvatarInfo.aQuestInfo[4] < 1 )
			{
				return tQUEST_INFO->qStartNPCNumber;
			}
			return tQUEST_INFO->qEndNPCNumber;
		}
	}
	return 0;
}
//RETURN_ITEM_NUMBER_FOR_QUEST_REWARD
int AVATAR_OBJECT::ReturnItemNumberForQuestReward( void )
{
	int index01;
	QUEST_INFO *tQUEST_INFO;

	tQUEST_INFO = mQUEST.Search( mUSER[mIndex].mAvatarInfo.aTribe, mUSER[mIndex].mAvatarInfo.aQuestInfo[0] );
	if( tQUEST_INFO == NULL )
	{
		return 0;
	}
	for( index01 = 0 ; index01 < 3 ; index01++ )
	{
		if( tQUEST_INFO->qReward[index01][0] == 6 )
		{
			return tQUEST_INFO->qReward[index01][1];
		}
	}
	return 0;
}
//RETURN_ITEM_QUANTITY_FOR_QUEST_REWARD
int AVATAR_OBJECT::ReturnItemQuantityForQuestReward( void )
{
	int index01;
	QUEST_INFO *tQUEST_INFO;
	ITEM_INFO *tITEM_INFO;

	tQUEST_INFO = mQUEST.Search( mUSER[mIndex].mAvatarInfo.aTribe, mUSER[mIndex].mAvatarInfo.aQuestInfo[0] );
	if( tQUEST_INFO == NULL )
	{
		return 0;
	}
	for( index01 = 0 ; index01 < 3 ; index01++ )
	{
		if( tQUEST_INFO->qReward[index01][0] != 6 )
		{
			continue;
		}
		tITEM_INFO = mITEM.Search( tQUEST_INFO->qReward[index01][1] );
		if( tITEM_INFO == NULL )
		{
			return 0;
		}
		if( tITEM_INFO->iSort == 2 )
		{
			return 1;
		}
		return 0;
	}
	return 0;
}
//SUMMON_QUEST_BOSS
int AVATAR_OBJECT::SummonQuestBoss( void )
{
	QUEST_INFO *tQUEST_INFO;
	float tCoord[3];

	if( ( mUSER[mIndex].mAvatarInfo.aQuestInfo[2] != 5 ) || ( ReturnQuestPresentState() != 2 ) )
	{
		return 0;
	}
	tQUEST_INFO = mQUEST.Search( mUSER[mIndex].mAvatarInfo.aTribe, mUSER[mIndex].mAvatarInfo.aQuestInfo[0] );
	if( tQUEST_INFO == NULL )
	{
		return 0;
	}
	if( mSERVER_INFO.mServerNumber != tQUEST_INFO->qSummonInfo[0] )
	{
		return 0;
	}
	tCoord[0] = (float) tQUEST_INFO->qSummonInfo[1];
	tCoord[1] = (float) tQUEST_INFO->qSummonInfo[2];
	tCoord[2] = (float) tQUEST_INFO->qSummonInfo[3];
	if( mUTIL.GetLengthXYZ( &mDATA.aAction.aLocation[0], &tCoord[0] ) > 300.0f )
	{
		return 0;
	}
	mSUMMON.SummonMonsterForSpecial( tQUEST_INFO->qSolution[0], &tCoord[0], TRUE );
	return 0;
}
//CHANGE_QUEST_ITEM
int AVATAR_OBJECT::ChangeQuestItem( int tItemNumber1, int tItemNumber2 )
{
	int index01;
	int index02;

	for( index01 = 0 ; index01 < 2 ; index01++ )
	{
		for( index02 = 0 ; index02 < MAX_INVENTORY_SLOT_NUM ; index02++ )
		{
			if( mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][0] == tItemNumber1 )
			{
				mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][0] = tItemNumber2;
				mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][3] = 0;
				mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][4] = 0;
				mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][5] = 0;
				return index01;
			}
		}
	}
	return -1;
}
//DELETE_QUEST_ITEM
int AVATAR_OBJECT::DeleteQuestItem( int tItemNumber )
{
	int index01;
	int index02;

	for( index01 = 0 ; index01 < 2 ; index01++ )
	{
		for( index02 = 0 ; index02 < MAX_INVENTORY_SLOT_NUM ; index02++ )
		{
			if( mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][0] == tItemNumber )
			{
				mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][0] = 0;
				mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][1] = 0;
				mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][2] = 0;
				mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][3] = 0;
				mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][4] = 0;
				mUSER[mIndex].mAvatarInfo.aInventory[index01][index02][5] = 0;
				return index01;
			}
		}
	}
	return -1;
}

/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */
int AVATAR_OBJECT::IsUsedOrnament (void)
{
	if (mUSER[mIndex].mAvatarInfo.iUseOrnament == true && mUSER[mIndex].mAvatarInfo.aLevel1 > 112) 
	{
		if (mUSER[mIndex].mAvatarInfo.aEquip[9][0] > 0 &&   // 안면?
			mUSER[mIndex].mAvatarInfo.aEquip[10][0] > 0 &&  // 가슴?
			mUSER[mIndex].mAvatarInfo.aEquip[11][0] > 0 &&  // 복부?
			mUSER[mIndex].mAvatarInfo.aEquip[12][0] > 0) {  // 팔, 다리, 어깨?
				// 장식 해방 시스템 옵션이 켜져 있고 모든 장식 아이템을
				// 착용 중이면서 갑 레벨 이상인 경우에 활성화 된다.
				if(mUSER[mIndex].mAvatarInfo.iGoldOrnamentRemainsTime > 0)
				{
					return 1;
				}
				else if( mUSER[mIndex].mAvatarInfo.iSilverOrnamentRemainsTime > 0 )
				{
					return 2;
				}
				return 0;
		}

	}

	return 0;
}
/** 2009. 11. 18 : 장식 해방 시스템 (wolf) */

//-------------------------------------------------------------------------------------------------
