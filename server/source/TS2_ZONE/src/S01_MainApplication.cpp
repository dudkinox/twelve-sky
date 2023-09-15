//-------------------------------------------------------------------------------------------------
//HEADER
//-------------------------------------------------------------------------------------------------
#include "H01_MainApplication.h"
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CALLBACK_FUNCTION
//-------------------------------------------------------------------------------------------------
//LRESULT CALLBACK WinMainProcedure( HWND hWnd, UINT nMss, WPARAM wPrm, LPARAM lPrm );
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//APPLICATION_FUNCTION
//-------------------------------------------------------------------------------------------------
#if 0
BOOL ReadServerInfo( void );
BOOL GetOneValueFromFile( HANDLE hFile, char *tBuffer, int tBufferSize );
BOOL ApplicationInit( void );
void ApplicationFree( void );
void BugTrapInit( void );
#endif
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//GLOBAL_VARIABLES
//-------------------------------------------------------------------------------------------------
#if 0
HWND hMainWindow;
SERVER_INFO mSERVER_INFO;
DWORD mBaseTickCountForLogic;
DWORD mPostTickCountForLogic;
BOOL mCheckLogicFlag;
#endif
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//WIN_MAIN
//-------------------------------------------------------------------------------------------------
#if 0
int WINAPI WinMain( HINSTANCE hThis, HINSTANCE hPrev, LPSTR lArgs, int nMode )
{
	BugTrapInit ();
	if( !ReadServerInfo() )
	{
		MessageBox( NULL, "[Error::ReadServerInfo()]", "SERVER", ( MB_OK | MB_SYSTEMMODAL ) );
		return FALSE;
	}

	WNDCLASSEX wCls;
	MSG nMss;

	wCls.cbSize	= sizeof( WNDCLASSEX );
	wCls.style = CS_HREDRAW | CS_VREDRAW;
	wCls.lpfnWndProc = WinMainProcedure;
	wCls.cbClsExtra = 0;
	wCls.cbWndExtra = 0;
	wCls.hInstance = hThis;
	wCls.hIcon = LoadIcon( hThis, MAKEINTRESOURCE(IDI_SERVER) );
	wCls.hCursor = LoadCursor( NULL, IDC_ARROW );
	wCls.hbrBackground = (HBRUSH) GetStockObject( BLACK_BRUSH );
	wCls.lpszMenuName = NULL;
	wCls.lpszClassName = mSERVER_INFO.mServerName;
	wCls.hIconSm = NULL;
	if( !RegisterClassEx( &wCls ) )
	00{
		MessageBox( NULL, "[Error::RegisterClassEx()]", mSERVER_INFO.mServerName, ( MB_OK | MB_SYSTEMMODAL ) );
		return FALSE;
	}
	hMainWindow = CreateWindowEx( NULL, mSERVER_INFO.mServerName, mSERVER_INFO.mServerName, ( WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE ), ( GetSystemMetrics( SM_CXSCREEN ) / 2 - ( MAIN_WINDOW_SIZE_X / 2 ) ), ( GetSystemMetrics( SM_CYSCREEN ) / 2 - ( MAIN_WINDOW_SIZE_Y / 2 ) ), MAIN_WINDOW_SIZE_X, MAIN_WINDOW_SIZE_Y, HWND_DESKTOP, NULL, hThis, NULL );
	if( !hMainWindow )
	{
		MessageBox( NULL, "[Error::CreateWindow()]", mSERVER_INFO.mServerName, ( MB_OK | MB_SYSTEMMODAL ) );
		return FALSE;
	}
	ShowWindow( hMainWindow, SW_MINIMIZE );
	UpdateWindow( hMainWindow );
	if( !ApplicationInit() )
	{
		ApplicationFree();
		MessageBox( NULL, "[Error::ApplicationInit()]", mSERVER_INFO.mServerName, ( MB_OK | MB_SYSTEMMODAL ) );
		return FALSE;
	}
	mBaseTickCountForLogic = GetTickCount();
	mPostTickCountForLogic = mBaseTickCountForLogic;
	mCheckLogicFlag = FALSE;
	while( GetMessage( &nMss, NULL, 0, 0 ) )
	{
		TranslateMessage( &nMss );
		DispatchMessage( &nMss );
		if( mCheckLogicFlag )
		{
			mCheckLogicFlag = FALSE;
			mGAME.Logic( 0.5f );
		}
	}
	ApplicationFree();
	return nMss.wParam;
}
#endif
//-------------------------------------------------------------------------------------------------

//---------//
//N-PROTECT//
//---------//
#ifdef __N_PROTECT__
GGAUTHS_API void NpLog( int tMode, char *tMsg ){}
GGAUTHS_API void GGAuthUpdateCallback( PGG_UPREPORT tReport ){}
#endif
//---------//
//---------//
//---------//

//-------------------------------------------------------------------------------------------------
//CALLBACK_FUNCTION
//-------------------------------------------------------------------------------------------------
//WIN_MAIN_PROCEDURE

int ProcessResponse(UINT nMss, SOCKET *cli_fd, UINT fd_status, void *arg)
{
	int n_result = 0;

	switch( nMss )
	{
	case WM_NETWORK_MESSAGE_1 :
		mBaseTickCountForLogic = GetTickCount();
		if( ( mBaseTickCountForLogic - mPostTickCountForLogic ) >= 500 )
		{
			mPostTickCountForLogic = mBaseTickCountForLogic;
			mCheckLogicFlag = TRUE;
		}
		n_result = mSERVER.PROCESS_FOR_NETWORK(cli_fd, fd_status, arg);
		return n_result;
	case WM_NETWORK_MESSAGE_2 :
		mBaseTickCountForLogic = GetTickCount();
		if( ( mBaseTickCountForLogic - mPostTickCountForLogic ) >= 500 )
		{
			mPostTickCountForLogic = mBaseTickCountForLogic;
			mCheckLogicFlag = TRUE;
		}
		n_result = mCENTER_COM.PROCESS_FOR_NETWORK(*cli_fd, fd_status);
		return n_result;
	case WM_NETWORK_MESSAGE_3 :
		mBaseTickCountForLogic = GetTickCount();
		if( ( mBaseTickCountForLogic - mPostTickCountForLogic ) >= 500 )
		{
			mPostTickCountForLogic = mBaseTickCountForLogic;
			mCheckLogicFlag = TRUE;
		}
		n_result = mRELAY_COM.PROCESS_FOR_NETWORK(*cli_fd, fd_status);
		return n_result;
    case WM_HEARTBEAT_MESSAGE:
        //g_BroadCastMgr.Process (wPrm, lPrm);
        mBaseTickCountForLogic = GetTickCount();
        if( ( mBaseTickCountForLogic - mPostTickCountForLogic ) >= 500 )
        {
            mPostTickCountForLogic = mBaseTickCountForLogic;
            mCheckLogicFlag = TRUE;
        }
        g_BroadCastMgr.Process (*cli_fd, fd_status);
		return 0;
#if 0
	case WM_TIMER :
		switch( wPrm )
		{
		case 1 :
			mBaseTickCountForLogic = GetTickCount();
			if( ( mBaseTickCountForLogic - mPostTickCountForLogic ) > 500 )
			{
				mPostTickCountForLogic = mBaseTickCountForLogic;
				mCheckLogicFlag = TRUE;
			}
			return 0;
		case 2 :
#ifdef __N_PROTECT__
			//---------//
			//N-PROTECT//
			//---------//
			GGAuthUpdateTimer();
			//---------//
			//---------//
			//---------//
#endif
			return 0;
		}
		return 0;
	case WM_DESTROY :
		PostQuitMessage( 0 );
		return 0;
#endif
	}
}
#if 0
LRESULT CALLBACK WinMainProcedure( HWND hWnd, UINT nMss, WPARAM wPrm, LPARAM lPrm )
{
	switch( nMss )
	{
	case WM_NETWORK_MESSAGE_1 :
		mSERVER.PROCESS_FOR_NETWORK( hWnd, wPrm, lPrm );
		mBaseTickCountForLogic = GetTickCount();
		if( ( mBaseTickCountForLogic - mPostTickCountForLogic ) > 500 )
		{
			mPostTickCountForLogic = mBaseTickCountForLogic;
			mCheckLogicFlag = TRUE;
		}
		return 0;
	case WM_NETWORK_MESSAGE_2 :
		mCENTER_COM.PROCESS_FOR_NETWORK( hWnd, wPrm, lPrm );
		mBaseTickCountForLogic = GetTickCount();
		if( ( mBaseTickCountForLogic - mPostTickCountForLogic ) > 500 )
		{
			mPostTickCountForLogic = mBaseTickCountForLogic;
			mCheckLogicFlag = TRUE;
		}
		return 0;
	case WM_NETWORK_MESSAGE_3 :
		mRELAY_COM.PROCESS_FOR_NETWORK( hWnd, wPrm, lPrm );
		mBaseTickCountForLogic = GetTickCount();
		if( ( mBaseTickCountForLogic - mPostTickCountForLogic ) > 500 )
		{
			mPostTickCountForLogic = mBaseTickCountForLogic;
			mCheckLogicFlag = TRUE;
		}
		return 0;
    case WM_HEARTBEAT_MESSAGE:
        g_BroadCastMgr.Process (wPrm, lPrm);
        mBaseTickCountForLogic = GetTickCount();
        if( ( mBaseTickCountForLogic - mPostTickCountForLogic ) > 500 )
        {
            mPostTickCountForLogic = mBaseTickCountForLogic;
            mCheckLogicFlag = TRUE;
        }
        return 0;
	case WM_TIMER :
		switch( wPrm )
		{
		case 1 :
			mBaseTickCountForLogic = GetTickCount();
			if( ( mBaseTickCountForLogic - mPostTickCountForLogic ) > 500 )
			{
				mPostTickCountForLogic = mBaseTickCountForLogic;
				mCheckLogicFlag = TRUE;
			}
			return 0;
		case 2 :
			//---------//
			//N-PROTECT//
			//---------//
			GGAuthUpdateTimer();
			//---------//
			//---------//
			//---------//
			return 0;
		}
		return 0;
	case WM_DESTROY :
		PostQuitMessage( 0 );
		return 0;
	}
	return DefWindowProc( hWnd,	nMss, wPrm,	lPrm );
}
#endif
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//APPLICATION_FUNCTION
//-------------------------------------------------------------------------------------------------
//READ_SERVER_INFO
#if 0
BOOL ReadServerInfo( void )
{
	HANDLE hFile;
	char tempString01[100];

	hFile = CreateFile( "CONFIG\\SERVER_INFO.TXT", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE )
	{
		MessageBox( NULL, "[#001]", "SERVER", ( MB_OK | MB_SYSTEMMODAL ) );
		return FALSE;
	}
	if( !GetOneValueFromFile( hFile, &tempString01[0], 100 ) )
	{
		MessageBox( NULL, "[#002]", "SERVER", ( MB_OK | MB_SYSTEMMODAL ) );
		CloseHandle( hFile );
		return FALSE;
	}
	strcpy( mSERVER_INFO.mServerName, tempString01 );
	if( strcmp( mSERVER_INFO.mServerName, "" ) == 0 )
	{
		MessageBox( NULL, "[#003]", "SERVER", ( MB_OK | MB_SYSTEMMODAL ) );
		CloseHandle( hFile );
		return FALSE;
	}
	if( !GetOneValueFromFile( hFile, &tempString01[0], 100 ) )
	{
		MessageBox( NULL, "[#004]", "SERVER", ( MB_OK | MB_SYSTEMMODAL ) );
		CloseHandle( hFile );
		return FALSE;
	}
	mSERVER_INFO.mServerPort = atoi( tempString01 );
	if( ( mSERVER_INFO.mServerPort < 10000 ) || ( mSERVER_INFO.mServerPort > 99999 ) )
	{
		MessageBox( NULL, "[#005]", "SERVER", ( MB_OK | MB_SYSTEMMODAL ) );
		CloseHandle( hFile );
		return FALSE;
	}
	if( !GetOneValueFromFile( hFile, &tempString01[0], 100 ) )
	{
		MessageBox( NULL, "[#006]", "SERVER", ( MB_OK | MB_SYSTEMMODAL ) );
		CloseHandle( hFile );
		return FALSE;
	}
	mSERVER_INFO.mServerMaxUserNum = atoi( tempString01 );
	if( ( mSERVER_INFO.mServerMaxUserNum < 1 ) || ( mSERVER_INFO.mServerMaxUserNum > MAX_AVATAR_OBJECT_NUM ) )
	{
		MessageBox( NULL, "[#007]", "SERVER", ( MB_OK | MB_SYSTEMMODAL ) );
		CloseHandle( hFile );
		return FALSE;
	}
	if( !GetOneValueFromFile( hFile, &tempString01[0], 100 ) )
	{
		MessageBox( NULL, "[#008]", "SERVER", ( MB_OK | MB_SYSTEMMODAL ) );
		CloseHandle( hFile );
		return FALSE;
	}
	mSERVER_INFO.mServerNumber = atoi( tempString01 );
	if( ( mSERVER_INFO.mServerNumber < MIN_VALID_ZONE_NUMBER ) || ( mSERVER_INFO.mServerNumber > MAX_VALID_ZONE_NUMBER ) )
	{
		MessageBox( NULL, "[#009]", "SERVER", ( MB_OK | MB_SYSTEMMODAL ) );
		CloseHandle( hFile );
		return FALSE;
	}
	if( !CloseHandle( hFile ) )
	{
		MessageBox( NULL, "[#010]", "SERVER", ( MB_OK | MB_SYSTEMMODAL ) );
		return FALSE;
	}
	hFile = CreateFile( "..\\CONFIG\\SERVER_INFO.TXT", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE )
	{
		MessageBox( NULL, "[#011]", "SERVER", ( MB_OK | MB_SYSTEMMODAL ) );
		return FALSE;
	}
	if( !GetOneValueFromFile( hFile, &tempString01[0], 100 ) )
	{
		MessageBox( NULL, "[#012]", "SERVER", ( MB_OK | MB_SYSTEMMODAL ) );
		CloseHandle( hFile );
		return FALSE;
	}
	mSERVER_INFO.mWorldNumber = atoi( tempString01 );
	if( ( mSERVER_INFO.mWorldNumber < 1 ) || ( mSERVER_INFO.mWorldNumber > 999 ) )
	{
		MessageBox( NULL, "[#013]", "SERVER", ( MB_OK | MB_SYSTEMMODAL ) );
		CloseHandle( hFile );
		return FALSE;
	}
	if( !GetOneValueFromFile( hFile, &tempString01[0], 100 ) )
	{
		MessageBox( NULL, "[#014]", "SERVER", ( MB_OK | MB_SYSTEMMODAL ) );
		CloseHandle( hFile );
		return FALSE;
	}
	strcpy( mSERVER_INFO.mUpperServer_1_IP, tempString01 );
	if( !GetOneValueFromFile( hFile, &tempString01[0], 100 ) )
	{
		MessageBox( NULL, "[#015]", "SERVER", ( MB_OK | MB_SYSTEMMODAL ) );
		CloseHandle( hFile );
		return FALSE;
	}
	mSERVER_INFO.mUpperServer_1_Port = atoi( tempString01 );
	if( !GetOneValueFromFile( hFile, &tempString01[0], 100 ) )
	{
		MessageBox( NULL, "[#016]", "SERVER", ( MB_OK | MB_SYSTEMMODAL ) );
		CloseHandle( hFile );
		return FALSE;
	}
	strcpy( mSERVER_INFO.mUpperServer_2_IP, tempString01 );
	if( !GetOneValueFromFile( hFile, &tempString01[0], 100 ) )
	{
		MessageBox( NULL, "[#017]", "SERVER", ( MB_OK | MB_SYSTEMMODAL ) );
		CloseHandle( hFile );
		return FALSE;
	}
	mSERVER_INFO.mUpperServer_2_Port = atoi( tempString01 );
	if( !GetOneValueFromFile( hFile, &tempString01[0], 100 ) )
	{
		MessageBox( NULL, "[#018]", "SERVER", ( MB_OK | MB_SYSTEMMODAL ) );
		CloseHandle( hFile );
		return FALSE;
	}
	strcpy( mSERVER_INFO.mUpperServer_3_IP, tempString01 );
	if( !GetOneValueFromFile( hFile, &tempString01[0], 100 ) )
	{
		MessageBox( NULL, "[#019]", "SERVER", ( MB_OK | MB_SYSTEMMODAL ) );
		CloseHandle( hFile );
		return FALSE;
	}
	mSERVER_INFO.mUpperServer_3_Port = atoi( tempString01 );
	if( !GetOneValueFromFile( hFile, &tempString01[0], 100 ) )
	{
		MessageBox( NULL, "[#020]", "SERVER", ( MB_OK | MB_SYSTEMMODAL ) );
		CloseHandle( hFile );
		return FALSE;
	}
	strcpy( mSERVER_INFO.mUpperServer_4_IP, tempString01 );
	if( !GetOneValueFromFile( hFile, &tempString01[0], 100 ) )
	{
		MessageBox( NULL, "[#021]", "SERVER", ( MB_OK | MB_SYSTEMMODAL ) );
		CloseHandle( hFile );
		return FALSE;
	}
	mSERVER_INFO.mUpperServer_4_Port = atoi( tempString01 );
	if( !GetOneValueFromFile( hFile, &tempString01[0], 100 ) )
	{
		MessageBox( NULL, "[#022]", "SERVER", ( MB_OK | MB_SYSTEMMODAL ) );
		CloseHandle( hFile );
		return FALSE;
	}
	strcpy( mSERVER_INFO.mUpperServer_5_IP, tempString01 );
	if( !GetOneValueFromFile( hFile, &tempString01[0], 100 ) )
	{
		MessageBox( NULL, "[#023]", "SERVER", ( MB_OK | MB_SYSTEMMODAL ) );
		CloseHandle( hFile );
		return FALSE;
	}
	mSERVER_INFO.mUpperServer_5_Port = atoi( tempString01 );
	if( !CloseHandle( hFile ) )
	{
		MessageBox( NULL, "[#024]", "SERVER", ( MB_OK | MB_SYSTEMMODAL ) );
		return FALSE;
	}
	return TRUE;
}
//GET_ONE_VALUE_FROM_FILE
BOOL GetOneValueFromFile( HANDLE hFile, char *tBuffer, int tBufferSize )
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
//APPLICATION_INIT
BOOL ApplicationInit( void )
{
	char tempString01[1000];
	int tResult;

	if( !mSERVER.Init( &tResult ) )
	{
		sprintf( tempString01, "[Error::mSERVER.Init()]-(%d)", tResult );
		MessageBox( NULL, tempString01, mSERVER_INFO.mServerName, ( MB_OK | MB_SYSTEMMODAL ) );
		return FALSE;
	}
	return TRUE;
}
//APPLICATION_FREE
void ApplicationFree( void )
{
	mSERVER.Free();
}
//BUG_TRAP_INIT
void BugTrapInit( void )
{
	BT_InstallSehFilter();
	BT_SetFlags( BTF_DETAILEDMODE );
	BT_SetAppName( "TS2_ZONE" );
}
#endif
//-------------------------------------------------------------------------------------------------
