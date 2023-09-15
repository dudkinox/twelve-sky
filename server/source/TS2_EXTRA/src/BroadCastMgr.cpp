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
	CBroadCastMgr *thd = reinterpret_cast<CBroadCastMgr *>(arg); 

	thd->m_hSock = socket (AF_INET, SOCK_DGRAM, 0);
    if (thd->m_hSock < 0) return false;

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
		struct timeval tv = {0, 500};
		FD_SET(thd->m_hSock, &read_fds);
		if(select((thd->m_hSock) + 1, &read_fds, NULL, NULL, &tv) < 0) {
			close(thd->m_hSock);
			return NULL;
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
    socklen_t iFromAddrSize = 0;
    DWORD dwReceived = 0L;
    char buffer[MAX_AVATAR_NAME_LENGTH] = {0, };
    SOCKADDR_IN s_from_in;

    //wsabuf.buf = buffer;
    //wsabuf.len = sizeof (buffer);
    iFromAddrSize = sizeof (SOCKADDR_IN);

	/*
	::WSARecvFrom (m_hSock, &wsabuf, 1, &dwReceived, &dwFlags, 
	reinterpret_cast<LPSOCKADDR> (&s_from_in), 
	&iFromAddrSize, NULL, NULL);
	*/
	dwReceived = recvfrom(cli_fd, buffer, MAX_AVATAR_NAME_LENGTH, 0
			, (struct sockaddr *)&s_from_in, &iFromAddrSize);

#ifdef __ORG__
    if (dwReceived > 0) {
        int iUserIndex = Update (buffer);
        if (iUserIndex != -1 && mUSER[iUserIndex].mCheckConnectState == TRUE) {
            if (mUSER[iUserIndex].mCheckValidState == TRUE && 
                mUSER[iUserIndex].mMoveZoneResult == 0) {
                // 동일 무명인 경우 보내기 주소를 저장한다.
                mUSER[iUserIndex].m_bUDPActivated = true;
                ::CopyMemory (&mUSER[iUserIndex].m_AddrFrom, &s_from_in, sizeof (SOCKADDR_IN));
                return true;
            }
        }
    }
    return false;
#else
	if (dwReceived >= CTOZ_UDP_UPDATE_SIZE) {
		switch (static_cast<BYTE> (buffer[0])) {
			case CTOZ_UDP_UPDATE_TYPE:       
				bResult = UpdateAddress (buffer, &s_from_in);           
				break;
			case CTOZ_ACK_INCOMING_UDP_TYPE: 
				bResult = CheckArrivedAtClient (buffer);    
				break;
			default:
				break;
		}
	}
#endif

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
#ifndef __ORG__
bool CBroadCastMgr::UpdateAddress (std::string name, SOCKADDR_IN* pAddr)
{
	int iUserIndex = Update (name);
	if (iUserIndex != -1 && mUSER[iUserIndex].mCheckConnectState == TRUE) {
		if (mUSER[iUserIndex].mCheckValidState == TRUE && 
				mUSER[iUserIndex].mMoveZoneResult == 0) {
			// .. ... .. ... ... .....
			mUSER[iUserIndex].m_bUDPActivated = true;
			::CopyMemory (&mUSER[iUserIndex].m_AddrFrom, pAddr, sizeof (SOCKADDR_IN));

			if (mUSER[iUserIndex].m_bUDPArrivedAtClient == false) {
				ZTOC_REQUEST_INCOMING_UDP pack;
#ifdef __WIN32__
				WSABUF wsabuf;
				DWORD dwWritten = 0L;

				wsabuf.buf = reinterpret_cast<char*> (&pack);
				wsabuf.len = ZTOC_REQUEST_INCOMING_UDP_SIZE;

				::WSASendTo (m_hSock, &wsabuf, 1, &dwWritten, 0L, 
						reinterpret_cast<LPSOCKADDR> (pAddr), 
						sizeof (SOCKADDR_IN), NULL, NULL);
#else
				sendto(m_hSock, (char *)&pack, ZTOC_REQUEST_INCOMING_UDP_SIZE, 0
						, LPSOCKADDR&pAddr, sizeof(SOCKADDR_IN));
#endif
			}

			return true;
		}
	}

	return false;
}

bool CBroadCastMgr::CheckArrivedAtClient (std::string name)
{
	int iUserIndex = Update (name);
	if (iUserIndex != -1 && mUSER[iUserIndex].mCheckConnectState == TRUE &&
			mUSER[iUserIndex].m_bUDPActivated == true) {
		if (mUSER[iUserIndex].mCheckValidState == TRUE && 
				mUSER[iUserIndex].mMoveZoneResult == 0) {
			// ....... ... UDP ... .... ... ack. .... 
			// . ...... UDP. .. . ... .. ...
			mUSER[iUserIndex].m_bUDPArrivedAtClient = true;
			return true;
		}
	}

	return false;
}
#endif
