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
}
//INIT

BOOL MyGame::Init( void )
{
	
	char tempString01[1000] = {0,};
	int index01 = 0;
	int index02 = 0;

	
	mTickCount = 0;
//-------------------------------------------------------------------------------------------------
// 2008.11.18 Kim SungSoo
// Intro : 1. Modify win32 file IO to linux c++ file IO
//-------------------------------------------------------------------------------------------------
#ifdef __WIN32__

	HANDLE hFile;
		
	hFile = CreateFile( "COST_INFO.TXT", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE )
	{
		return FALSE;
	}
	if( !GetOneValueFromFile( hFile, 1000, &tempString01[0] ) )
	{
		CloseHandle( hFile );
		return FALSE;
	}
	mTotalCostInfoNum = atoi( tempString01 );
	for( index01 = 0 ; index01 < mTotalCostInfoNum ; index01++ )
	{
		if( !GetOneValueFromFile( hFile, 1000, &tempString01[0] ) )
		{
			CloseHandle( hFile );
			return FALSE;
		}
		mCostInfoValue[index01][0] = atoi( tempString01 );
		if( !GetOneValueFromFile( hFile, 1000, &tempString01[0] ) )
		{
			CloseHandle( hFile );
			return FALSE;
		}
		mCostInfoValue[index01][1] = atoi( tempString01 );
		if( !GetOneValueFromFile( hFile, 1000, &tempString01[0] ) )
		{
			CloseHandle( hFile );
			return FALSE;
		}
		mCostInfoValue[index01][2] = atoi( tempString01 );
		if( !GetOneValueFromFile( hFile, 1000, &tempString01[0] ) )
		{
			CloseHandle( hFile );
			return FALSE;
		}
		mCostInfoValue[index01][3] = atoi( tempString01 );
		if( !GetOneValueFromFile( hFile, 1000, &tempString01[0] ) )
		{
			CloseHandle( hFile );
			return FALSE;
		}
		strcpy( mCostInfoCode[index01], tempString01 );
	}
	if( !CloseHandle( hFile ) )
	{
		return FALSE;
	}
	
#else
    // @ Cost Info Download @
    mIsSellCashItem = false;
    int tVersion = 0;

    LOG_TO_FILE("!GAME Init - Start.\n");

    LOG_TO_FILE("!GAME Init - Load Cost Info.\n");
    if (!LoadCostInfo(&mTotalCostInfoNum, &tVersion, mCostInfoValue, mCostInfoCode)) {
        return FALSE;
    }

    LOG_TO_FILE("!GAME Init - MakeCashItemInfo.\n");
    if (!MakeCashItemInfo(mCashItemInfo, mCostInfoValue, mTotalCostInfoNum)) {
        return FALSE;
    }

    mIsSellCashItem = true;
    mCashItemInfoVersion = tVersion;

    LOG_TO_FILE("!GAME Init - End.\n");
    // @
#endif

//-------------------------------------------------------------------------------------------------
	return TRUE;

}

//FREE
void MyGame::Free( void )
{
}
//GET_ONE_VALUE_FROM_FILE

//-------------------------------------------------------------------------------------------------
// 2008.11.18 Kim SungSoo
// Intro : 1. Modify win32 function to linux c++ 
//-------------------------------------------------------------------------------------------------
#ifdef __WIN32__
BOOL MyGame::GetOneValueFromFile( HANDLE hFile, int tBufferSize, char *tBuffer )
{
	int index01;
	DWORD nReadByte;
	BOOL tCheckSaveState;
	char tData;

	index01 = 0;
	tCheckSaveState = FALSE;
	while( TRUE )
	{
		if( !ReadFile( hFile, &tData, 1, &nReadByte, NULL ) )
		{
			return FALSE;
		}
		if( nReadByte != 1 )
		{
			return FALSE;
		}
		if( !tCheckSaveState )
		{
			if( tData == '[' )
			{
				tCheckSaveState = TRUE;
			}
		}
		else
		{
			if( tData == ']' )
			{
				tBuffer[index01] = '\0';
				break;
			}
			else
			{
				if( index01 < ( tBufferSize - 1 ) )
				{
					tBuffer[index01] = tData;
					index01++;
				}
				else
				{
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}
#else
// @ Cost Info Download @ {
//int MyGame::GetOneValueFromFile(const char *pBuffer , int iParsorIndex)
bool MyGame::GetOneValueFromFile(const char *pBuffer , int iParsorIndex, int *tResult)
{
    int i = 0;
    int chk = 0;
    char tbuf[20];
    int chk_cnt = 0;
    char *ptr_tmp = (char *)pBuffer;

    while(*ptr_tmp != '\0') {
        if(!chk) {
            if(*ptr_tmp == '[') {
                ptr_tmp++;
                chk_cnt++;
                if(chk_cnt == iParsorIndex)
                    chk = 1;
            } else {
                ptr_tmp++;
                continue;
            }
        } else {
            if(chk_cnt == iParsorIndex)
            {
                tbuf[i++] = *ptr_tmp;
                if(*ptr_tmp == ']') {
                    tbuf[--i] = '\0';
                    *tResult = atoi(tbuf);
                    //LOG_TO_FILE_1("GetOneValueFromFile---> %s\n", tbuf);
                    return true;
                    //break;
                }
            }
            ptr_tmp++;
        }
    }

    return false;
    //return atoi(tbuf);
} // @

bool MyGame::GetOneValueFromFile(char *pValue , const char *pBuffer , int iParsorIndex)
{
	if(pValue == NULL)
	{
		return false;
	}


	int i = 0;
	int chk = 0;
	char tbuf[51];
	int chk_cnt = 0;
	char *ptr_tmp = (char *)pBuffer;

	while(*ptr_tmp != '\0') {
		if(!chk) {
			if(*ptr_tmp == '[') {
				ptr_tmp++;
				chk_cnt++;
				if(chk_cnt == iParsorIndex)
					chk = 1;
			} else {
				ptr_tmp++;
				continue;
			}
		} else {
			if(chk_cnt == iParsorIndex) {
				tbuf[i++] = *ptr_tmp;
				if(*ptr_tmp == ']') {
					tbuf[--i] = '\0';
					break;
				}
			}
			ptr_tmp++;
		}
	}
	//LOG_TO_FILE_1("---> %s\n", tbuf);

	strncpy(pValue , (char*)tbuf , sizeof(tbuf) );

	return true;
}
#endif

// @ Cost Info Download @ {
int MyGame::GetCostInfoVersion()
{
    FILE *r_fp = NULL;

    if (!(r_fp = fopen("/user/service/bin/DATA/COST_INFO.TXT", "r")))
    {
        LOG_TO_FILE_1("!GetCostInfoVersion - file open false : %s\n",  "/user/service/bin/DATA/COST_INFO.TXT\n");
        return mCashItemInfoVersion; // 파일 열기 실패 시 기존 버전을 리턴.
    }

    char tempString01[1000] = {0};
    char *readResult = fgets(tempString01, 1000, r_fp);
    fclose(r_fp);

    if (readResult != NULL)
    {
        // COST_INFO.TXT 버전 추출
        int tVersion = -1;
        if (GetOneValueFromFile(tempString01 , 2, &tVersion))
        {
            return tVersion;
        }
    }

    LOG_TO_FILE("!GetCostInfoVersion - Version read fail\n");

    return -1;
}

bool MyGame::LoadCostInfo(int *pTotalCostInfoNum, int *pVersion, int tCostInfoValue[MAX_COST_INFO_NUM][MAX_COST_INFO_TYPE], char tCostInfoCode[MAX_COST_INFO_NUM][MAX_COST_INFO_CODE_LEN])
{
    char tempString01[1000] = {0};
    int index01 = 0;
    int index02 = 0;
    FILE *r_fp = NULL;

    if(!(r_fp = fopen("/user/service/bin/DATA/COST_INFO.TXT", "r"))) {
        LOG_TO_FILE_1("file open false : %s\n",  "/user/service/bin/DATA/COST_INFO.TXT\n");
        return false;
    }

    if(fgets(tempString01, 1000, r_fp) != NULL) {
        if (!GetOneValueFromFile(tempString01 , 1, pTotalCostInfoNum)) {
            LOG_TO_FILE_1("!Error - Get Total Cost Info Num - %s\n", tempString01);
            return false;
        }
        //LOG_TO_FILE_2("!totalcostinfonum[%d] , %s\n", *pTotalCostInfoNum , tempString01);

        // COST_INFO.TXT 버전 추출
        if (!GetOneValueFromFile(tempString01 , 2, pVersion)) {
            LOG_TO_FILE("!Error - Get Cost Info Version.\n");
            return false;
        }
        if (*pVersion <= 0) {
            LOG_TO_FILE_1("!Error - COST_INFO.TXT Version[%d]\n", *pVersion);
            return false;
        }
        LOG_TO_FILE_1("!COST_INFO.TXT Version[%d]\n", *pVersion);
    } else {
        LOG_TO_FILE_1("file load false : %s \n",  tempString01);
        return false;
    }

    // Extra 서버에서 사용할 CostInfo 추출.
    index01 = 0;
    while(fgets(tempString01, 1000, r_fp) != NULL) {
        //LOG_TO_FILE_1("file load value for one line : %s\n",  tempString01);
        for (index02 = 0; index02 < MAX_COST_INFO_TYPE; ++index02) {
            if (!GetOneValueFromFile(tempString01 , index02+1, &tCostInfoValue[index01][index02])) {
                LOG_TO_FILE_2("Error - CostInfoValue[%d][%d]\n", index01 ,index02);
                return false;
            }
            //LOG_TO_FILE_3("CostInfoValue[%d][%d] = %d \n", index01 ,index02 , tCostInfoValue[index01][index02] );
        }

        memset(tCostInfoCode[index01] , 0x00 , MAX_COST_INFO_CODE_LEN);
        GetOneValueFromFile(tCostInfoCode[index01] , tempString01 , index02+1 );
        //LOG_TO_FILE_2("CostInfoCode[%d] = %s \n", index01  , tCostInfoCode[index01] );
        index01++;
    }
    fclose(r_fp);

    if (*pTotalCostInfoNum != index01) {
        LOG_TO_FILE_2("Error - CostInfoNum Total(%d) / Load(%d)\n", *pTotalCostInfoNum, index01);
        return false;
    }

    return true;
}

bool MyGame::MakeCashItemInfo(int tCashItemInfo[4][10][10][4], const int tCostInfoValue[MAX_COST_INFO_NUM][MAX_COST_INFO_TYPE], int totalCostInfoNum)
{
    // 클라이언트에서 사용하는 캐쉬 정보 셋팅.
    const int MAX_CASH_ITEM_NUM = totalCostInfoNum;
    ::memset(tCashItemInfo, -1, MAX_CASH_ITEM_INFO_SIZE);

    int showType = 0;
    int pageCount[4] = {0};
    int itemCount[4] = {0};

    for (int costInfoIndex = 0; costInfoIndex < MAX_CASH_ITEM_NUM; ++costInfoIndex) {
        if(tCostInfoValue[costInfoIndex][4] <= 0) {
            LOG_TO_FILE("!MakeCashItemInfo Error - tCostInfoValue[costInfoIndex][4] < 0\n");
            return false;
        }
        showType = tCostInfoValue[costInfoIndex][4] - 1;

        if (pageCount[showType] > 9) {
            LOG_TO_FILE("!MakeCashItemInfo Error - over max page count\n");
            return false;
        }

        tCashItemInfo[showType][pageCount[showType]][itemCount[showType]][0] = costInfoIndex;                     // Cost Info Num - 1
        tCashItemInfo[showType][pageCount[showType]][itemCount[showType]][1] = tCostInfoValue[costInfoIndex][1];  // Item Number.
        tCashItemInfo[showType][pageCount[showType]][itemCount[showType]][2] = tCostInfoValue[costInfoIndex][2];  // Quantity.
        tCashItemInfo[showType][pageCount[showType]][itemCount[showType]][3] = tCostInfoValue[costInfoIndex][0];  // Cost.

        ++itemCount[showType];
        if (itemCount[showType] > 9) {
            ++pageCount[showType];
            itemCount[showType] = 0;
        }
    }

    return true;
} // @
//-------------------------------------------------------------------------------------------------
