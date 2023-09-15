//-------------------------------------------------------------------------------------------------
//MY_USER_H
//-------------------------------------------------------------------------------------------------
#ifndef MY_USER_H
#define MY_USER_H

#include <pthread.h>
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_USER
//-------------------------------------------------------------------------------------------------
//DEFINE
class MyUser
{
public:
	// # X-Trap # {
	char mXTrapSessionBuf[XTRAP_SESSION_BUF_SIZE];
	char mXTrapStep1_SendBuf[XTRAP_STEP_1_BUF_SIZE];
	unsigned long mXTrapTick;
	// # }

	int mEPFD;
	int* mCliFd;
	int mUserIndex;

	BOOL mCheckConnectState;
	BOOL mCheckValidState;

	DWORD mConnectTime;
	SOCKET mSocket;
	char mIP[16];

	char mPacketEncryptionValue[2];

	char* mBUFFER_FOR_SEND;
	int mTotalSendSize;
	char* mBUFFER_FOR_RECV;
	int mTotalRecvSize;

	//------------------------//
	//VARIABLE_FOR_THIS_SERVER//
	//------------------------//
	time_t mCheckTime;
	BOOL mCheckTempRegister;
	int mTempRegisterTribe;
	DWORD mRegisterTime;
	int mPlayUserIndex;
	char uID[MAX_USER_ID_LENGTH];
	int uUserSort;
	int uGoodFellow;
	int uLoginPlace;
	int uLoginPremium;
	int uLoginPremiumPCRoom;
	int uTraceState;
	char uBonus100Money;
	int uPremiumServerExpirationDate;
	AVATAR_INFO mAvatarInfo;
	int mEffectValue[MAX_AVATAR_EFFECT_SORT_NUM][2];
	float mGeneralExperienceUpRatio;
	float mItemDropUpRatio;
	float mGeneralExperienceDownRatio;
	float mSelfPatExperienceUpRatio;
	int mSupportSkillTimeUpRatio;
	BOOL mCheckHealthState;
	int mHealthState;

	/** 2009. 10. 26 : DarkAttack Potion add (wolf) */
	DWORD mTickCountForDarkAttack;
	int iKindDarkAttack;
	/** 2009. 10. 26 : DarkAttack Potion add (wolf) */

	DWORD mTickCountFor01Second;
	DWORD mTickCountFor01SecondForProtect;
	DWORD mTickCountFor30Second;
	DWORD mTickCountFor01MinuteForHealth;
	DWORD mTickCountFor01Minute;

	DWORD mTickCountFor01Minute_2;
	DWORD mTickCountFor03Second;
	DWORD mUniqueNumber;
	DWORD mAttackTickCount;
	unsigned int mSpeedTickCount;
	int mCheckMaxAttackPacketNum;
	int mMaxAttackPacketNum;
	int mNowAttackPacketNum;
	DWORD mHackCheckTime;
	int mMoveZoneResult;
	AUTH_INFO mAuthInfo;
#ifdef __LOCK__
	pthread_mutex_t m_user_lock;
	pthread_mutex_t m_user_quit_lock; // MyUser::Quit 함수 동기화 처리용.
#endif

#ifdef __GOD__
	time_t mEffectCheckTime;
	int mAttackPacketSort;
#endif

	int mHeartCheckCount;
	time_t mHeartCheckTime;

	bool m_bUDPActivated; /// UDP 주소 확인 여부.
	SOCKADDR_IN m_AddrFrom; ///< Address Info to broadcast by UDP.
	int m_iTryCountForAuth;
#ifndef __ORG__
	bool m_bUDPArrivedAtClient;
#endif

	MOVE_ITEM_INFO mMoveItemInfo; // 이동서 사용 정보.

	MyUser(void);

	BOOL Init(int tUserIndex, int tSendBufferSize, int tRecvBufferSize);
	void Free(void);

	void Send(BOOL tCheckValidBuffer, char* tBuffer, int tBufferSize, bool bUDPSend = false);
	//void Quit( void );
	void Quit(const char* tFileName = __FILE__, const char* tFuncName = nullptr, int tLine = __LINE__);


	// Cost Info Download
	bool mIsValidCashItemInfo;

	bool IsValidSkillNumber(int skillNumber);
	int GetMaxSkillGradeNum(int skillNumber);
};

//INSTANCE
extern MyUser* mUSER;
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------------
