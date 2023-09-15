//-------------------------------------------------------------------------------------------------
//HEADER
//-------------------------------------------------------------------------------------------------
#include "Shengming.h"
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_GAME
//-------------------------------------------------------------------------------------------------
//INSTANCE
MyGame mGAME;
//CREATE
MyGame::MyGame(void)
{
}

//INIT

BOOL MyGame::Init(void)
{
	char tempString01[1000] = {0,};
	int index01 = 0;
	int index02 = 0;


	mTickCount = 0;
	mIsSellCashItem = false;
	int tVersion = 0;

	//__LogMgr()->Log("!GAME Init - Start.\n");

	//__LogMgr()->Log("!GAME Init - Load Cost Info.\n");
	if (!LoadCostInfo(&mTotalCostInfoNum, &tVersion, mCostInfoValue, mCostInfoCode))
	{
		return FALSE;
	}

	//__LogMgr()->Log("!GAME Init - MakeCashItemInfo.\n");
	if (!MakeCashItemInfo(mCashItemInfo, mCostInfoValue, mTotalCostInfoNum))
	{
		return FALSE;
	}

	mIsSellCashItem = true;
	mCashItemInfoVersion = tVersion;

	//__LogMgr()->Log("!GAME Init - End.\n");
	return TRUE;
}

//FREE
void MyGame::Free(void)
{
}

//GET_ONE_VALUE_FROM_FILE
BOOL MyGame::GetOneValueFromFile(HANDLE hFile, int tBufferSize, char* tBuffer)
{
	int index01;
	DWORD nReadByte;
	BOOL tCheckSaveState;
	char tData;

	index01 = 0;
	tCheckSaveState = FALSE;
	while (TRUE)
	{
		if (!ReadFile(hFile, &tData, 1, &nReadByte, nullptr))
		{
			return FALSE;
		}
		if (nReadByte != 1)
		{
			return FALSE;
		}
		if (!tCheckSaveState)
		{
			if (tData == '[')
			{
				tCheckSaveState = TRUE;
			}
		}
		else
		{
			if (tData == ']')
			{
				tBuffer[index01] = '\0';
				break;
			}
			if (index01 < (tBufferSize - 1))
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
	return TRUE;
}

bool MyGame::GetOneValueFromFile(const char* pBuffer, int iParsorIndex, int* tResult)
{
	int i = 0;
	int chk = 0;
	char tbuf[20];
	int chk_cnt = 0;
	auto ptr_tmp = (char*)pBuffer;

	while (*ptr_tmp != '\0')
	{
		if (!chk)
		{
			if (*ptr_tmp == '[')
			{
				ptr_tmp++;
				chk_cnt++;
				if (chk_cnt == iParsorIndex)
					chk = 1;
			}
			else
			{
				ptr_tmp++;
			}
		}
		else
		{
			if (chk_cnt == iParsorIndex)
			{
				tbuf[i++] = *ptr_tmp;
				if (*ptr_tmp == ']')
				{
					tbuf[--i] = '\0';
					*tResult = atoi(tbuf);
					//__LogMgr()->Log("GetOneValueFromFile---> %s\n", tbuf);
					return true;
					//break;
				}
			}
			ptr_tmp++;
		}
	}

	return false;
	//return atoi(tbuf);
}

bool MyGame::GetOneValueFromFile(char* pValue, const char* pBuffer, int iParsorIndex)
{
	if (pValue == nullptr)
	{
		return false;
	}


	int i = 0;
	int chk = 0;
	char tbuf[51];
	int chk_cnt = 0;
	auto ptr_tmp = (char*)pBuffer;

	while (*ptr_tmp != '\0')
	{
		if (!chk)
		{
			if (*ptr_tmp == '[')
			{
				ptr_tmp++;
				chk_cnt++;
				if (chk_cnt == iParsorIndex)
					chk = 1;
			}
			else
			{
				ptr_tmp++;
			}
		}
		else
		{
			if (chk_cnt == iParsorIndex)
			{
				tbuf[i++] = *ptr_tmp;
				if (*ptr_tmp == ']')
				{
					tbuf[--i] = '\0';
					break;
				}
			}
			ptr_tmp++;
		}
	}
	//__LogMgr()->Log("---> %s\n", tbuf);

	strncpy(pValue, tbuf, sizeof(tbuf));

	return true;
}


// @ Cost Info Download @ {
int MyGame::GetCostInfoVersion()
{
	FILE* r_fp = nullptr;

	if (!(r_fp = fopen("DATA/COST_INFO.TXT", "r")))
	{
		__LogMgr()->Log("!GetCostInfoVersion - file open false : %s\n", "DATA/COST_INFO.TXT\n");
		return mCashItemInfoVersion;
	}

	char tempString01[1000] = {0};
	char* readResult = fgets(tempString01, 1000, r_fp);
	fclose(r_fp);

	if (readResult != nullptr)
	{
		int tVersion = -1;
		if (GetOneValueFromFile(tempString01, 2, &tVersion))
		{
			return tVersion;
		}
	}

	__LogMgr()->Log("!GetCostInfoVersion - Version read fail\n");

	return -1;
}

bool MyGame::LoadCostInfo(int* pTotalCostInfoNum, int* pVersion,
                          int tCostInfoValue[MAX_COST_INFO_NUM][MAX_COST_INFO_TYPE],
                          char tCostInfoCode[MAX_COST_INFO_NUM][MAX_COST_INFO_CODE_LEN])
{
	char tempString01[1000] = {0};
	int index01 = 0;
	int index02 = 0;
	FILE* r_fp = nullptr;

	if (!(r_fp = fopen("DATA/COST_INFO.TXT", "r")))
	{
		__LogMgr()->Log("file open false : %s\n", "DATA/COST_INFO.TXT\n");
		return false;
	}

	if (fgets(tempString01, 1000, r_fp) != nullptr)
	{
		if (!GetOneValueFromFile(tempString01, 1, pTotalCostInfoNum))
		{
			__LogMgr()->Log("!Error - Get Total Cost Info Num - %s\n", tempString01);
			return false;
		}
		//__LogMgr()->Log("!totalcostinfonum[%d] , %s\n", *pTotalCostInfoNum , tempString01);

		if (!GetOneValueFromFile(tempString01, 2, pVersion))
		{
			__LogMgr()->Log("!Error - Get Cost Info Version.\n");
			return false;
		}
		if (*pVersion <= 0)
		{
			__LogMgr()->Log("!Error - COST_INFO.TXT Version[%d]\n", *pVersion);
			return false;
		}
		//__LogMgr()->Log("!COST_INFO.TXT Version[%d]\n", *pVersion);
	}
	else
	{
		__LogMgr()->Log("file load false : %s \n", tempString01);
		return false;
	}

	index01 = 0;
	while (fgets(tempString01, 1000, r_fp) != nullptr)
	{
		//__LogMgr()->Log("file load value for one line : %s\n",  tempString01);
		for (index02 = 0; index02 < MAX_COST_INFO_TYPE; ++index02)
		{
			if (!GetOneValueFromFile(tempString01, index02 + 1, &tCostInfoValue[index01][index02]))
			{
				__LogMgr()->Log("Error - CostInfoValue[%d][%d]\n", index01, index02);
				return false;
			}
			//__LogMgr()->Log_3("CostInfoValue[%d][%d] = %d \n", index01 ,index02 , tCostInfoValue[index01][index02] );
		}

		memset(tCostInfoCode[index01], 0x00, MAX_COST_INFO_CODE_LEN);
		GetOneValueFromFile(tCostInfoCode[index01], tempString01, index02 + 1);
		//__LogMgr()->Log_2("CostInfoCode[%d] = %s \n", index01  , tCostInfoCode[index01] );
		index01++;
	}
	fclose(r_fp);

	if (*pTotalCostInfoNum != index01)
	{
		__LogMgr()->Log("Error - CostInfoNum Total(%d) / Load(%d)\n", *pTotalCostInfoNum, index01);
		return false;
	}

	return true;
}

bool MyGame::MakeCashItemInfo(int tCashItemInfo[4][14][10][4],
                              const int tCostInfoValue[MAX_COST_INFO_NUM][MAX_COST_INFO_TYPE], int totalCostInfoNum)
{
	const int MAX_CASH_ITEM_NUM = totalCostInfoNum;
	memset(tCashItemInfo, -1, 8960);

	int showType = 0;
	int pageCount[4] = {0};
	int itemCount[4] = {0};

	for (int costInfoIndex = 0; costInfoIndex < MAX_CASH_ITEM_NUM; ++costInfoIndex)
	{
		if (tCostInfoValue[costInfoIndex][4] <= 0)
		{
			__LogMgr()->Log("!MakeCashItemInfo Error - tCostInfoValue[costInfoIndex][4] < 0\n");
			return false;
		}
		showType = tCostInfoValue[costInfoIndex][4] - 1;

		if (pageCount[showType] > 9)
		{
			__LogMgr()->Log("!MakeCashItemInfo Error - over max page count\n");
			return false;
		}

		tCashItemInfo[showType][pageCount[showType]][itemCount[showType]][0] = costInfoIndex; // Cost Info Num - 1
		tCashItemInfo[showType][pageCount[showType]][itemCount[showType]][1] = tCostInfoValue[costInfoIndex][1];
		// Item Number.
		tCashItemInfo[showType][pageCount[showType]][itemCount[showType]][2] = tCostInfoValue[costInfoIndex][2];
		// Quantity.
		tCashItemInfo[showType][pageCount[showType]][itemCount[showType]][3] = tCostInfoValue[costInfoIndex][0];
		// Cost.

		++itemCount[showType];
		if (itemCount[showType] > 9)
		{
			++pageCount[showType];
			itemCount[showType] = 0;
		}
	}

	return true;
} // @
//-------------------------------------------------------------------------------------------------
