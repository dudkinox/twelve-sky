//-------------------------------------------------------------------------------------------------
//MY_SERVER_H
//-------------------------------------------------------------------------------------------------
#ifndef MY_SERVER_H
#define MY_SERVER_H
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//CLASS_OF_MY_SERVER
//-------------------------------------------------------------------------------------------------
//DEFINE

class MyServer
{
private:
	WSADATA mWsaData;
	SOCKET mSocket;
	SOCKADDR_IN mAddress;

public:
	bool mRankCheck;
	int mMAX_USER_NUM;
	int mMAX_SEND_BUFFER_SIZE;
	int mMAX_RECV_BUFFER_SIZE;

	MyServer(void);

	BOOL Init(int* tResult);
	void Free(void);

	//void PROCESS_FOR_NETWORK( HWND hWnd, WPARAM wPrm, LPARAM lPrm );
	int PROCESS_FOR_NETWORK(HWND hWnd, WPARAM wPrm, LPARAM lPrm);

	void SetSocket(SOCKET listen_socket) { mSocket = listen_socket; }
	void HexDump(char* buf, int len, int addr);
};

//INSTANCE
extern MyServer mSERVER;
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------------
