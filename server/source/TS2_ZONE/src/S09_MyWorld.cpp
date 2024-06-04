//-------------------------------------------------------------------------------------------------
//HEADER
//-------------------------------------------------------------------------------------------------
#include "H01_MainApplication.h"
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_WORLD_FOR_GXD
//-------------------------------------------------------------------------------------------------
//INSTANCE
WORLD_FOR_GXD mWORLD;
//CREATE
WORLD_FOR_GXD::WORLD_FOR_GXD( void )
{
	mCheckValidState = FALSE;
	mWorldTris = NULL;
	mQuadtree = NULL;
}
//INIT
void WORLD_FOR_GXD::Init( void )
{
	mCheckValidState = FALSE;
	mWorldTris = NULL;
	mQuadtree = NULL;
}
//FREE
void WORLD_FOR_GXD::Free( void )
{
	int index01;

	mCheckValidState = FALSE;
	if( mWorldTris != NULL )
	{
		free( mWorldTris );
		mWorldTris = NULL;
	}
	if( mQuadtree != NULL )
	{
		for( index01 = 0 ; index01 < mTotalQuadtreeNodeNum ; index01++ )
		{
			if( mQuadtree[index01].mTrisIndex != NULL )
			{
				free( mQuadtree[index01].mTrisIndex );
				mQuadtree[index01].mTrisIndex = NULL;
			}
		}
		free( mQuadtree );
		mQuadtree = NULL;
	}
}
//LOAD_WM
#define MAX_PATH 1024
BOOL WORLD_FOR_GXD::LoadWM( void )
{
	if( mCheckValidState )
	{
		return FALSE;
	}

	int index01;
	char tFileName[MAX_PATH];
	DWORD nReadByte;
	BOOL tCheckExistTrisIndex;
	DWORD tOriginalIndex;
	DWORD tOriginalSize;
	BYTE *tOriginal;
	DWORD tCompressSize;
	BYTE *tCompress;

	/*
	char tFileNamePart01[4];
	tFileNamePart01[0] = ( mSERVER_INFO.mServerNumber % 1000 ) / 100 + '0';
	tFileNamePart01[1] = ( mSERVER_INFO.mServerNumber % 100 ) / 10 + '0';
	tFileNamePart01[2] = ( mSERVER_INFO.mServerNumber % 10 ) / 1 + '0';
	tFileNamePart01[3] = '\0';
	*/
#ifdef __WIN32__
	HANDLE hFile;
	sprintf( tFileName, "DATA\\Z%s.WM", tFileNamePart01 );

	hFile = CreateFile( tFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE )
	{
		Free();
		return FALSE;
	}
	if( !ReadFile( hFile, &tOriginalSize, 4, &nReadByte, NULL ) )
	{
		CloseHandle( hFile );
		Free();
		return FALSE;
	}
	if( nReadByte != 4 )
	{
		CloseHandle( hFile );
		Free();
		return FALSE;
	}
	tOriginal = (BYTE *) GlobalAlloc( GMEM_FIXED, tOriginalSize );
	if( tOriginal == NULL )
	{
		CloseHandle( hFile );
		Free();
		return FALSE;
	}
	if( !ReadFile( hFile, &tCompressSize, 4, &nReadByte, NULL ) )
	{
		GlobalFree( tOriginal );
		CloseHandle( hFile );
		Free();
		return FALSE;
	}
	if( nReadByte != 4 )
	{
		GlobalFree( tOriginal );
		CloseHandle( hFile );
		Free();
		return FALSE;
	}
	tCompress = (BYTE *) GlobalAlloc( GMEM_FIXED, tCompressSize );
	if( tCompress == NULL )
	{
		GlobalFree( tOriginal );
		CloseHandle( hFile );
		Free();
		return FALSE;
	}
	if( !ReadFile( hFile, &tCompress[0], tCompressSize, &nReadByte, NULL ) )
	{
		GlobalFree( tCompress );
		GlobalFree( tOriginal );
		CloseHandle( hFile );
		Free();
		return FALSE;
	}
	if( nReadByte != tCompressSize )
	{
		GlobalFree( tCompress );
		GlobalFree( tOriginal );
		CloseHandle( hFile );
		Free();
		return FALSE;
	}
	if( !mGAME.Decompress( tCompressSize, &tCompress[0], tOriginalSize, &tOriginal[0] ) )
	{
		GlobalFree( tCompress );
		GlobalFree( tOriginal );
		CloseHandle( hFile );
		Free();
		return FALSE;
	}
	GlobalFree( tCompress );
	tOriginalIndex = 0;
	CopyMemory( &mWorldTrisNum, &tOriginal[tOriginalIndex], 4 );
	tOriginalIndex += 4;
	mWorldTris = (WORLDTRIS_FOR_GXD *) GlobalAlloc( GMEM_FIXED, ( mWorldTrisNum * sizeof( WORLDTRIS_FOR_GXD ) ) );
	if( mWorldTris == NULL )
	{
		GlobalFree( tOriginal );
		CloseHandle( hFile );
		Free();
		return FALSE;
	}
	CopyMemory( &mWorldTris[0], &tOriginal[tOriginalIndex], ( mWorldTrisNum * sizeof( WORLDTRIS_FOR_GXD ) ) );
	tOriginalIndex += ( mWorldTrisNum * sizeof( WORLDTRIS_FOR_GXD ) );
	CopyMemory( &mTotalQuadtreeNodeNum, &tOriginal[tOriginalIndex], 4 );
	tOriginalIndex += 4;
	CopyMemory( &mMaxQuadtreeNodeLeafNum, &tOriginal[tOriginalIndex], 4 );
	tOriginalIndex += 4;
	mQuadtree = (QUADTREENODE_FOR_GXD *) GlobalAlloc( GMEM_FIXED, ( mTotalQuadtreeNodeNum * sizeof( QUADTREENODE_FOR_GXD ) ) );
	if( mQuadtree == NULL )
	{
		GlobalFree( tOriginal );
		CloseHandle( hFile );
		Free();
		return FALSE;
	}
	for( index01 = 0 ; index01 < mTotalQuadtreeNodeNum ; index01++ )
	{
		mQuadtree[index01].mTrisNum = 0;
		mQuadtree[index01].mTrisIndex = NULL;
	}
	for( index01 = 0 ; index01 < mTotalQuadtreeNodeNum ; index01++ )
	{
		CopyMemory( &mQuadtree[index01].mBoxMin[0], &tOriginal[tOriginalIndex], 12 );
		tOriginalIndex += 12;
		CopyMemory( &mQuadtree[index01].mBoxMax[0], &tOriginal[tOriginalIndex], 12 );
		tOriginalIndex += 12;
		CopyMemory( &mQuadtree[index01].mTrisNum, &tOriginal[tOriginalIndex], 4 );
		tOriginalIndex += 4;
		CopyMemory( &tCheckExistTrisIndex, &tOriginal[tOriginalIndex], 4 );
		tOriginalIndex += 4;
		if( tCheckExistTrisIndex )
		{
			mQuadtree[index01].mTrisIndex = (int *) GlobalAlloc( GMEM_FIXED, ( mQuadtree[index01].mTrisNum * 4 ) );
			if( mQuadtree[index01].mTrisIndex == NULL )
			{
				GlobalFree( tOriginal );
				CloseHandle( hFile );
				Free();
				return FALSE;
			}
			CopyMemory( &mQuadtree[index01].mTrisIndex[0], &tOriginal[tOriginalIndex], ( mQuadtree[index01].mTrisNum * 4 ) );
			tOriginalIndex += ( mQuadtree[index01].mTrisNum * 4 );
		}
		CopyMemory( &mQuadtree[index01].mChildNodeIndex[0], &tOriginal[tOriginalIndex], 16 );
		tOriginalIndex += 16;
	}
	GlobalFree( tOriginal );
	if( !CloseHandle( hFile ) )
	{
		Free();
		return FALSE;
	}
#else 
	FILE *r_fp = NULL;
	sprintf( tFileName, "/home/guitar/12sky/twelve-sky/server/service/bin/DATA/Z%03d.WM", mSERVER_INFO.mServerNumber );

	if(!(r_fp = fopen(tFileName, "r"))) {
		Free();
		return FALSE;
	}


	if((nReadByte = fread(&tOriginalSize, 1, 4, r_fp)) < 0) {
		fclose(r_fp);
		Free();
		return FALSE;
	}

	if( nReadByte != 4 )
	{
		fclose(r_fp);
		Free();
		return FALSE;
	}

	tOriginal = (BYTE *)calloc(1, tOriginalSize );
	if( tOriginal == NULL )
	{
		fclose(r_fp);
		Free();
		return FALSE;
	}

	if((nReadByte = fread(&tCompressSize, 1, 4, r_fp)) < 0) {
		free(tOriginal);
		fclose(r_fp);
		Free();
		return FALSE;
	}

	if( nReadByte != 4 )
	{
		free( tOriginal );
		fclose(r_fp);
		Free();
		return FALSE;
	}

	tCompress = (BYTE *)calloc(1, tCompressSize );
	if( tCompress == NULL )
	{
		free( tOriginal );
		fclose(r_fp );
		Free();
		return FALSE;
	}

	if((nReadByte = fread(tCompress, 1, tCompressSize, r_fp)) < 0) {
		free( tCompress );
		free( tOriginal );
		fclose(r_fp);
		Free();
		return FALSE;
	}

	if( nReadByte != tCompressSize )
	{
		free( tCompress );
		free( tOriginal );
		fclose(r_fp );
		Free();
		return FALSE;
	}

	if( !mGAME.Decompress( tCompressSize, tCompress, tOriginalSize, tOriginal) )
	{
		free( tCompress );
		free( tOriginal );
		fclose(r_fp );
		Free();
		return FALSE;
	}

	free( tCompress );
	tOriginalIndex = 0;
	CopyMemory( &mWorldTrisNum, &tOriginal[tOriginalIndex], 4 );
	tOriginalIndex += 4;
	mWorldTris = (WORLDTRIS_FOR_GXD *)calloc(1, ( mWorldTrisNum * sizeof( WORLDTRIS_FOR_GXD ) ) );
	if( mWorldTris == NULL )
	{
		free( tOriginal );
		fclose(r_fp);
		Free();
		return FALSE;
	}
	CopyMemory( &mWorldTris[0], &tOriginal[tOriginalIndex], ( mWorldTrisNum * sizeof( WORLDTRIS_FOR_GXD ) ) );
	tOriginalIndex += ( mWorldTrisNum * sizeof( WORLDTRIS_FOR_GXD ) );
	CopyMemory( &mTotalQuadtreeNodeNum, &tOriginal[tOriginalIndex], 4 );
	tOriginalIndex += 4;
	CopyMemory( &mMaxQuadtreeNodeLeafNum, &tOriginal[tOriginalIndex], 4 );
	tOriginalIndex += 4;
	mQuadtree = (QUADTREENODE_FOR_GXD *)calloc(1, ( mTotalQuadtreeNodeNum * sizeof( QUADTREENODE_FOR_GXD ) ) );
	if( mQuadtree == NULL )
	{
		free( tOriginal );
		fclose(r_fp);
		Free();
		return FALSE;
	}
	
	for( index01 = 0 ; index01 < mTotalQuadtreeNodeNum ; index01++ )
	{
		mQuadtree[index01].mTrisNum = 0;
		mQuadtree[index01].mTrisIndex = NULL;
	}

	for( index01 = 0 ; index01 < mTotalQuadtreeNodeNum ; index01++ )
	{
		CopyMemory( &mQuadtree[index01].mBoxMin[0], &tOriginal[tOriginalIndex], 12 );
		tOriginalIndex += 12;
		CopyMemory( &mQuadtree[index01].mBoxMax[0], &tOriginal[tOriginalIndex], 12 );
		tOriginalIndex += 12;
		CopyMemory( &mQuadtree[index01].mTrisNum, &tOriginal[tOriginalIndex], 4 );
		tOriginalIndex += 4;
		tCheckExistTrisIndex = tOriginal[tOriginalIndex];
		//CopyMemory( &tCheckExistTrisIndex, &tOriginal[tOriginalIndex], 4 );
		tOriginalIndex += 4;
		//LOG_TO_FILE_3("NUM = %d %s %d\n", index01, strerror(errno), tCheckExistTrisIndex);

		if( tCheckExistTrisIndex )
		{
			mQuadtree[index01].mTrisIndex = (int *)calloc(1, ( mQuadtree[index01].mTrisNum * 4 ) );
			if( mQuadtree[index01].mTrisIndex == NULL )
			{
				free( tOriginal );
				fclose(r_fp);
				Free();
				return FALSE;
			}
			CopyMemory( &mQuadtree[index01].mTrisIndex[0], &tOriginal[tOriginalIndex], ( mQuadtree[index01].mTrisNum * 4 ) );
			tOriginalIndex += ( mQuadtree[index01].mTrisNum * 4 );
		}

		CopyMemory( &mQuadtree[index01].mChildNodeIndex[0], &tOriginal[tOriginalIndex], 16 );
		tOriginalIndex += 16;
	}

	if(tOriginal)
		free( tOriginal );

	fclose(r_fp);

#endif
	mCheckValidState = TRUE;

	return TRUE;
}
//GET_Y_COORD_WITH_TRIS
BOOL WORLD_FOR_GXD::GetYCoordWithTris( int tTrisIndex, float x, float z, float *y )
{
	if( mWorldTris[tTrisIndex].mPlaneInfo[1] == 0.0f )
	{
		return FALSE;
	}
	*y = ( mWorldTris[tTrisIndex].mPlaneInfo[3] - mWorldTris[tTrisIndex].mPlaneInfo[0] * x - mWorldTris[tTrisIndex].mPlaneInfo[2] * z ) / mWorldTris[tTrisIndex].mPlaneInfo[1];
	return TRUE;
}
//CHECK_POINT_IN_TRIS
BOOL WORLD_FOR_GXD::CheckPointInTris( int tTrisIndex, float tPoint[3] )
{
	float tV0[3];
	float tV1[3];
	float tV2[3];
	float tDot00;
	float tDot01;
	float tDot02;
	float tDot11;
	float tDot12;
	float tInvDenom;
	float tU;
	float tV;

	tV0[0] = mWorldTris[tTrisIndex].mVertex[1].mV[0] - mWorldTris[tTrisIndex].mVertex[0].mV[0];
	tV0[1] = mWorldTris[tTrisIndex].mVertex[1].mV[1] - mWorldTris[tTrisIndex].mVertex[0].mV[1];
	tV0[2] = mWorldTris[tTrisIndex].mVertex[1].mV[2] - mWorldTris[tTrisIndex].mVertex[0].mV[2];
	tV1[0] = mWorldTris[tTrisIndex].mVertex[2].mV[0] - mWorldTris[tTrisIndex].mVertex[0].mV[0];
	tV1[1] = mWorldTris[tTrisIndex].mVertex[2].mV[1] - mWorldTris[tTrisIndex].mVertex[0].mV[1];
	tV1[2] = mWorldTris[tTrisIndex].mVertex[2].mV[2] - mWorldTris[tTrisIndex].mVertex[0].mV[2];
	tV2[0] = tPoint[0] - mWorldTris[tTrisIndex].mVertex[0].mV[0];
	tV2[1] = tPoint[1] - mWorldTris[tTrisIndex].mVertex[0].mV[1];
	tV2[2] = tPoint[2] - mWorldTris[tTrisIndex].mVertex[0].mV[2];
	tDot00 = tV0[0] * tV0[0] + tV0[1] * tV0[1] + tV0[2] * tV0[2];
	tDot01 = tV0[0] * tV1[0] + tV0[1] * tV1[1] + tV0[2] * tV1[2];
	tDot02 = tV0[0] * tV2[0] + tV0[1] * tV2[1] + tV0[2] * tV2[2];
	tDot11 = tV1[0] * tV1[0] + tV1[1] * tV1[1] + tV1[2] * tV1[2];
	tDot12 = tV1[0] * tV2[0] + tV1[1] * tV2[1] + tV1[2] * tV2[2];
	if( ( tDot00 * tDot11 - tDot01 * tDot01 ) == 0.0f )
	{
		return FALSE;
	}
	tInvDenom = 1.0f / ( tDot00 * tDot11 - tDot01 * tDot01 );
	tU = ( tDot11 * tDot02 - tDot01 * tDot12 ) * tInvDenom;
	if( tU < 0.0f )
	{
		return FALSE;
	}
	tV = ( tDot00 * tDot12 - tDot01 * tDot02 ) * tInvDenom;
	if( tV < 0.0f )
	{
		return FALSE;
	}
	if( ( tU + tV ) > 1.0f )
	{
		return FALSE;
	}
	return TRUE;
}
//CHECK_POINT_IN_TRIS_WITHOUT_Y_COORD
BOOL WORLD_FOR_GXD::CheckPointInTrisWithoutYCoord( int tTrisIndex, float tPoint[3] )
{
	float tV0[2];
	float tV1[2];
	float tV2[2];
	float tDot00;
	float tDot01;
	float tDot02;
	float tDot11;
	float tDot12;
	float tInvDenom;
	float tU;
	float tV;

	tV0[0] = mWorldTris[tTrisIndex].mVertex[1].mV[0] - mWorldTris[tTrisIndex].mVertex[0].mV[0];
	tV0[1] = mWorldTris[tTrisIndex].mVertex[1].mV[2] - mWorldTris[tTrisIndex].mVertex[0].mV[2];
	tV1[0] = mWorldTris[tTrisIndex].mVertex[2].mV[0] - mWorldTris[tTrisIndex].mVertex[0].mV[0];
	tV1[1] = mWorldTris[tTrisIndex].mVertex[2].mV[2] - mWorldTris[tTrisIndex].mVertex[0].mV[2];
	tV2[0] = tPoint[0] - mWorldTris[tTrisIndex].mVertex[0].mV[0];
	tV2[1] = tPoint[2] - mWorldTris[tTrisIndex].mVertex[0].mV[2];
	tDot00 = tV0[0] * tV0[0] + tV0[1] * tV0[1];
	tDot01 = tV0[0] * tV1[0] + tV0[1] * tV1[1];
	tDot02 = tV0[0] * tV2[0] + tV0[1] * tV2[1];
	tDot11 = tV1[0] * tV1[0] + tV1[1] * tV1[1];
	tDot12 = tV1[0] * tV2[0] + tV1[1] * tV2[1];
	if( ( tDot00 * tDot11 - tDot01 * tDot01 ) == 0.0f )
	{
		return FALSE;
	}
	tInvDenom = 1.0f / ( tDot00 * tDot11 - tDot01 * tDot01 );
	tU = ( tDot11 * tDot02 - tDot01 * tDot12 ) * tInvDenom;
	if( tU < 0.0f )
	{
		return FALSE;
	}
	tV = ( tDot00 * tDot12 - tDot01 * tDot02 ) * tInvDenom;
	if( tV < 0.0f )
	{
		return FALSE;
	}
	if( ( tU + tV ) > 1.0f )
	{
		return FALSE;
	}
	return TRUE;
}
//CHECK_POINT_IN_WORLD_WITHOUT_Y_COORD
BOOL WORLD_FOR_GXD::CheckPointInWorldWithoutYCoord( float tPoint[3] )
{
	if( !mCheckValidState )
	{
		return FALSE;
	}

	int index01;
	int tSelectNodeIndex;
	int tSelectChildIndex;

	tSelectNodeIndex = 0;
	while( TRUE )
	{
		if( mQuadtree[tSelectNodeIndex].mChildNodeIndex[0] != -1 )
		{
			for( index01 = 0 ; index01 < 4 ; index01++ )
			{
				tSelectChildIndex = mQuadtree[tSelectNodeIndex].mChildNodeIndex[index01];
				if( ( tPoint[0] >= mQuadtree[tSelectChildIndex].mBoxMin[0] ) && ( tPoint[0] <= mQuadtree[tSelectChildIndex].mBoxMax[0] ) && ( tPoint[2] >= mQuadtree[tSelectChildIndex].mBoxMin[2] ) && ( tPoint[2] <= mQuadtree[tSelectChildIndex].mBoxMax[2] ) )
				{
					tSelectNodeIndex = tSelectChildIndex;
					break;
				}
			}
			if( index01 == 4 )
			{
				return FALSE;
			}
		}
		else
		{
			break;
		}
	}
	for( index01 = 0 ; index01 < mQuadtree[tSelectNodeIndex].mTrisNum ; index01++ )
	{
		if( CheckPointInTrisWithoutYCoord( mQuadtree[tSelectNodeIndex].mTrisIndex[index01], &tPoint[0] ) )
		{
			return TRUE;
		}
	}
	return FALSE;
}
//GET_Y_COORD
BOOL WORLD_FOR_GXD::GetYCoord( float x, float z, float *y, BOOL tCheckExistPostYCoord, float tPostYCoord, BOOL tCheckTwoSide, BOOL tCheckOnlyOne )
{
	if( !mCheckValidState )
	{
		return FALSE;
	}

	int index01;
	int tSelectNodeIndex;
	int tSelectChildIndex;
	BOOL tCheckExistYCoord;
	float tCoord[3];

	tSelectNodeIndex = 0;
	while( TRUE )
	{
		if( mQuadtree[tSelectNodeIndex].mChildNodeIndex[0] != -1 )
		{
			for( index01 = 0 ; index01 < 4 ; index01++ )
			{
				tSelectChildIndex = mQuadtree[tSelectNodeIndex].mChildNodeIndex[index01];
				if( ( x >= mQuadtree[tSelectChildIndex].mBoxMin[0] ) && ( x <= mQuadtree[tSelectChildIndex].mBoxMax[0] ) && ( z >= mQuadtree[tSelectChildIndex].mBoxMin[2] ) && ( z <= mQuadtree[tSelectChildIndex].mBoxMax[2] ) )
				{
					tSelectNodeIndex = tSelectChildIndex;
					break;
				}
			}
			if( index01 == 4 )
			{
				return FALSE;
			}
		}
		else
		{
			break;
		}
	}
	if( !tCheckExistPostYCoord )
	{
		tPostYCoord = mQuadtree[0].mBoxMax[1];
	}
	tCheckExistYCoord = FALSE;
	tCoord[0] = x;
	tCoord[1] = 0.0f;
	tCoord[2] = z;
	for( index01 = 0 ; index01 < mQuadtree[tSelectNodeIndex].mTrisNum ; index01++ )
	{
		if( !tCheckTwoSide )
		{
			if( mWorldTris[mQuadtree[tSelectNodeIndex].mTrisIndex[index01]].mPlaneInfo[1] <= 0.0f )
			{
				continue;
			}
		}
		if( !GetYCoordWithTris( mQuadtree[tSelectNodeIndex].mTrisIndex[index01], tCoord[0], tCoord[2], &tCoord[1] ) )
		{
			continue;
		}
		if( tCoord[1] > tPostYCoord )
		{
			continue;
		}
		if( !CheckPointInTris( mQuadtree[tSelectNodeIndex].mTrisIndex[index01], &tCoord[0] ) )
		{
			continue;
		}
		if( !tCheckExistYCoord )
		{
			tCheckExistYCoord = TRUE;
			*y = tCoord[1];
			if( tCheckOnlyOne )
			{
				break;
			}
		}
		else
		{
			if( tCoord[1] > *y )
			{
				*y = tCoord[1];
			}
		}
	}
	return tCheckExistYCoord;
}
//PATH
void WORLD_FOR_GXD::Path( float tSCoord[3], float tECoord[3], float tSpeed, float tPostTime, float tResult[3] )
{
	if( ( !mCheckValidState ) || ( tSpeed <= 0.0f ) || ( tPostTime <= 0.0f ) )
	{
		CopyMemory( &tResult[0], &tSCoord[0], 12 );
		return;
	}

	float tPostLength = tSpeed * tPostTime;
	float tDirection[3];
	float tLength;
	float tCoord[3];

	tResult[0] = tSCoord[0];
	tResult[2] = tSCoord[2];
	while( TRUE )
	{
		tDirection[0] = tECoord[0] - tResult[0];
		tDirection[2] = tECoord[2] - tResult[2];
		tLength = sqrtf( tDirection[0] * tDirection[0] + tDirection[2] * tDirection[2] );
		if( tLength <= tPostLength )
		{
			tCoord[0] = tECoord[0];
			tCoord[2] = tECoord[2];
			if( !CheckPointInWorldWithoutYCoord( &tCoord[0] ) )
			{
				break;
			}
			tResult[0] = tCoord[0];
			tResult[2] = tCoord[2];
			break;
		}
		tDirection[0] /= tLength;
		tDirection[2] /= tLength;
		tCoord[0] = tResult[0] + tPostLength * tDirection[0];
		tCoord[2] = tResult[2] + tPostLength * tDirection[2];
		if( !CheckPointInWorldWithoutYCoord( &tCoord[0] ) )
		{
			break;
		}
		tResult[0] = tCoord[0];
		tResult[2] = tCoord[2];
	}
	if( !GetYCoord( tResult[0], tResult[2], &tResult[1], FALSE, 0.0f, FALSE, TRUE ) )
	{
		CopyMemory( &tResult[0], &tSCoord[0], 12 );
		return;
	}
}
//PATH_FOR_MONSTER_ATTACK
BOOL WORLD_FOR_GXD::PathForMonsterAttack( float tSCoord[3], float tECoord[3], float tSpeed, float tPostTime, float tLimitCoord[3], float tLimitLength, float tResult[3] )
{
	if( ( !mCheckValidState ) || ( tSpeed <= 0.0f ) || ( tPostTime <= 0.0f ) )
	{
		CopyMemory( &tResult[0], &tSCoord[0], 12 );
		return FALSE;
	}

	float tPostLength = tSpeed * tPostTime;
	float tDirection[3];
	float tLength;
	float tCoord[3];

	tResult[0] = tSCoord[0];
	tResult[2] = tSCoord[2];
	while( TRUE )
	{
		tLength = ( tLimitCoord[0] - tResult[0] ) * ( tLimitCoord[0] - tResult[0] ) + ( tLimitCoord[2] - tResult[2] ) * ( tLimitCoord[2] - tResult[2] );
		if( tLength <= ( tLimitLength * tLimitLength ) )
		{
			break;
		}
		tDirection[0] = tECoord[0] - tResult[0];
		tDirection[2] = tECoord[2] - tResult[2];
		tLength = sqrtf( tDirection[0] * tDirection[0] + tDirection[2] * tDirection[2] );
		if( tLength <= tPostLength )
		{
			break;
		}
		tDirection[0] /= tLength;
		tDirection[2] /= tLength;
		tCoord[0] = tResult[0] + tPostLength * tDirection[0];
		tCoord[2] = tResult[2] + tPostLength * tDirection[2];
		if( !CheckPointInWorldWithoutYCoord( &tCoord[0] ) )
		{
			CopyMemory( &tResult[0], &tSCoord[0], 12 );
			return FALSE;
		}
		tResult[0] = tCoord[0];
		tResult[2] = tCoord[2];
	}
	if( !GetYCoord( tResult[0], tResult[2], &tResult[1], FALSE, 0.0f, FALSE, TRUE ) )
	{
		CopyMemory( &tResult[0], &tSCoord[0], 12 );
		return FALSE;
	}
	return TRUE;
}
//MOVE
BOOL WORLD_FOR_GXD::Move( float tSCoord[3], float tECoord[3], float tSpeed, float tPostTime, BOOL *tCheckArrival )
{
	if( ( !mCheckValidState ) || ( tSpeed <= 0.0f ) || ( tPostTime <= 0.0f ) )
	{
		*tCheckArrival = FALSE;
		return FALSE;
	}

	float tPostLength = tSpeed * tPostTime;
	float tDirection[3];
	float tLength;
	float tCoord[3];

	tDirection[0] = tECoord[0] - tSCoord[0];
	tDirection[2] = tECoord[2] - tSCoord[2];
	tLength = sqrtf( tDirection[0] * tDirection[0] + tDirection[2] * tDirection[2] );
	if( tLength <= tPostLength )
	{
		tCoord[0] = tECoord[0];
		tCoord[2] = tECoord[2];
		if( !GetYCoord( tCoord[0], tCoord[2], &tCoord[1], FALSE, 0.0f, FALSE, TRUE ) )
		{
			*tCheckArrival = FALSE;
			return FALSE;
		}
		CopyMemory( &tSCoord[0], &tCoord[0], 12 );
		*tCheckArrival = TRUE;
		return TRUE;
	}
	tDirection[0] /= tLength;
	tDirection[2] /= tLength;
	tCoord[0] = tSCoord[0] + tPostLength * tDirection[0];
	tCoord[2] = tSCoord[2] + tPostLength * tDirection[2];
	if( !GetYCoord( tCoord[0], tCoord[2], &tCoord[1], FALSE, 0.0f, FALSE, TRUE ) )
	{
		*tCheckArrival = FALSE;
		return FALSE;
	}
	CopyMemory( &tSCoord[0], &tCoord[0], 12 );
	*tCheckArrival = FALSE;
	return TRUE;
}
//-------------------------------------------------------------------------------------------------
