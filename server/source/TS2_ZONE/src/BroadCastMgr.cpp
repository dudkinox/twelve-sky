#include "H01_MainApplication.h"

CBroadCastMgr g_BroadCastMgr;

CBroadCastMgr::CBroadCastMgr(void) : m_hSock (-1)
{
}

CBroadCastMgr::~CBroadCastMgr(void)
{
    Close ();
}

bool CBroadCastMgr::Prepare (int iPort)
{
	mPort = iPort;

	if(pthread_create(&m_handle, NULL, CBroadCastMgr::Start, this) != 0) {
		LOG_TO_FILE_2("![%s] : %s\n", __FUNCTION__, strerror(errno));
		return FALSE;
	}
	pthread_detach(m_handle);
	return TRUE;

	/*
	   if (::WSAAsyncSelect (m_hSock, hMainWindow, WM_HEARTBEAT_MESSAGE, 
	   FD_READ) == SOCKET_ERROR) {
	   closesocket (m_hSock);
	   return false;
	   }
    return true;
	*/

}

void *CBroadCastMgr::Start(void *arg)
{
AGAIN:
	CBroadCastMgr *thd = reinterpret_cast<CBroadCastMgr *>(arg); 

	thd->m_hSock = socket (AF_INET, SOCK_DGRAM, 0);
    if (thd->m_hSock < 0) return NULL;

    SOCKADDR_IN s_addr_in;
    s_addr_in.sin_family = AF_INET;
    s_addr_in.sin_addr.s_addr = htonl (INADDR_ANY);
    s_addr_in.sin_port = htons (thd->mPort);

    if (bind (thd->m_hSock, LPSOCKADDR&s_addr_in, 
                sizeof (SOCKADDR_IN)) < 0) {
        closesocket (thd->m_hSock);
        return NULL;
    }

	fd_set read_fds;
	FD_ZERO(&read_fds);

	while(1) {
		struct timeval tv = {1, 0};
		FD_ZERO(&read_fds);
		FD_SET(thd->m_hSock, &read_fds);
		if(select((thd->m_hSock) + 1, &read_fds, NULL, NULL, &tv) < 0) {
			if(errno == EINTR)
				continue;
			LOG_TO_FILE_2("![%s] - udp select failed(%s)\n", __FUNCTION__, strerror(errno));
			close(thd->m_hSock);
			goto AGAIN;
		}

		if(FD_ISSET(thd->m_hSock, &read_fds)) {
			ProcessResponse(WM_HEARTBEAT_MESSAGE, &(thd->m_hSock), FD_READ);
		}
	}

}

//bool CBroadCastMgr::Process (WPARAM wParam, LPARAM lParam)
//bool CBroadCastMgr::Process (void)
bool CBroadCastMgr::Process (SOCKET cli_fd, UINT fd_status)
{
    //WSABUF wsabuf;
    //int iFromAddrSize = 0;
	bool bResult = false;
    DWORD dwReceived = 0L;
    SOCKADDR_IN s_from_in;
    char buffer[MAX_AVATAR_NAME_LENGTH + 1] = {0, };
    socklen_t iFromAddrSize = sizeof(struct sockaddr_in);

	dwReceived = recvfrom(m_hSock, buffer, MAX_AVATAR_NAME_LENGTH + 1, 0
			, (struct sockaddr *)&s_from_in, &iFromAddrSize);

	if (dwReceived >= CTOZ_UDP_UPDATE_SIZE) {
		char *aName = buffer + 1;
		switch (static_cast<BYTE> (buffer[0])) {
			case CTOZ_UDP_UPDATE_TYPE:       
				bResult = UpdateAddress (aName, &s_from_in);           
				break;
			case CTOZ_ACK_INCOMING_UDP_TYPE: 
				bResult = CheckArrivedAtClient (aName);    
				break;
			default:
				break;
		}
	}

	return bResult;
}

void CBroadCastMgr::Close (void)
{
    closesocket (m_hSock);

    m_hashNameToIndex.clear ();
}

int CBroadCastMgr::Update (std::string name)
{
    map<string, int>::const_iterator iter;

    iter = m_hashNameToIndex.find (name);
    if (iter != m_hashNameToIndex.end ()) {
        return iter -> second;
    }

    pair<map<string, int>::iterator, bool> pr;

    for (int i = 0; i < mSERVER.mMAX_USER_NUM; i++) {
        if (name.compare (mUSER[i].mAvatarInfo.aName) == 0) {
            pr = m_hashNameToIndex.insert (std::pair<std::string, int> (name, i));
            return i;
        }
    }

    return -1;
}

bool CBroadCastMgr::Delete (std::string name)
{
    map<string, int>::iterator iter;

    iter = m_hashNameToIndex.find (name);
    if (iter != m_hashNameToIndex.end ()) {
        m_hashNameToIndex.erase (iter);
        return true;
    }

    return false;
}

bool CBroadCastMgr::UpdateAddress (std::string name, SOCKADDR_IN* pAddr)
{
	int iUserIndex = Update (name);
	socklen_t sock_len = sizeof(struct sockaddr_in);
	if (iUserIndex != -1 && mUSER[iUserIndex].mCheckConnectState == TRUE) {
		if (mUSER[iUserIndex].mCheckValidState == TRUE && 
				mUSER[iUserIndex].mMoveZoneResult == 0) {
			mUSER[iUserIndex].m_bUDPActivated = true;
			::CopyMemory (&mUSER[iUserIndex].m_AddrFrom, pAddr, sizeof (SOCKADDR_IN));

			if (mUSER[iUserIndex].m_bUDPArrivedAtClient == false) {
				ZTOC_REQUEST_INCOMING_UDP pack;
				//LOG_TO_FILE_1("addr : %s\n", inet_ntoa(pAddr->sin_addr));
				if(sendto(m_hSock, (char *)&pack, ZTOC_REQUEST_INCOMING_UDP_SIZE, 0
						, (struct sockaddr *)pAddr, sock_len) < 0) {
					LOG_TO_FILE_2("![%s] : %s\n", __FUNCTION__, strerror(errno));
					return false;
				}
				//LOG_TO_FILE_1("udp(send) : %d\n", ZTOC_REQUEST_INCOMING_UDP_SIZE);
			}

			return true;
		}
	}

	return false;
}

bool CBroadCastMgr::CheckArrivedAtClient (std::string name)
{
	//LOG_TO_FILE("2\n");
	//LOG_TO_FILE_2("----> 1 %d %s\n", 0, name.c_str());
	int iUserIndex = Update (name);
	//LOG_TO_FILE_2("----> 2 %d %s\n", iUserIndex, name.c_str());
	if (iUserIndex != -1 && mUSER[iUserIndex].mCheckConnectState == TRUE &&
			mUSER[iUserIndex].m_bUDPActivated == true) {
			//LOG_TO_FILE("4\n");
		if (mUSER[iUserIndex].mCheckValidState == TRUE && 
				mUSER[iUserIndex].mMoveZoneResult == 0) {
			// ....... ... UDP ... .... ... ack. .... 
			// . ...... UDP. .. . ... .. ...
			mUSER[iUserIndex].m_bUDPArrivedAtClient = true;
			//LOG_TO_FILE("3\n");
			return true;
		}
	}

	return false;
}
