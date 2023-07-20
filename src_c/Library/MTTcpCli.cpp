// MTTcpCli.cpp: implementation of the CMTTcpClient class.
//
//////////////////////////////////////////////////////////////////////

#include	<stdio.h>
#include	<string.h>
#include    <errno.h>

#include "MTSysMsg.h"
#include "MTExcept.h"
#include "MTUtil.h"
#include "MTTcpCli.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMTTcpClient::CMTTcpClient(long lClRecvTimeout, long lClSendTimeout) {
	m_iSockFD = -1;
	m_bConnected = 0;

	m_recvTimeOut.tv_sec = lClRecvTimeout / 1000;
	m_recvTimeOut.tv_usec = (lClRecvTimeout % 1000) % 1000;
	if (lClRecvTimeout > 0)
		m_bCheckRecvTimeout = 1;
	else
		m_bCheckRecvTimeout = 0;

	m_sendTimeOut.tv_sec = lClSendTimeout / 1000;
	m_sendTimeOut.tv_usec = (lClSendTimeout % 1000) * 1000;
	if (lClSendTimeout > 0)
		m_bCheckSendTimeout = 1;
	else
		m_bCheckSendTimeout = 0;

#ifdef WIN32
	WSADATA wsaData;
	WSAStartup(0x0101, &wsaData);
#endif

}

CMTTcpClient::~CMTTcpClient() {
	if (m_bConnected) {
		try {
			Close();
		} catch (CMTException *e) {
			close (m_iSockFD);
			m_iSockFD = -1;
			e->Delete();
		}
	}
	/*
	 #ifdef WIN32
	 WSACleanup();
	 #endif
	 */
}

void CMTTcpClient::Open(char *lpszServerAddress, unsigned short ulPort) {
	unsigned long addr;

	if (m_bConnected)
		Close();

	m_sockAddrIn.sin_family = AF_INET;
	m_sockAddrIn.sin_port = htons(ulPort);

	addr = inet_addr(lpszServerAddress);

	m_sockAddrIn.sin_addr.s_addr = addr;
	/*
	 if( addr == INADDR_NONE )
	 {
	 struct hostent	*pHostEnt;

	 pHostEnt = gethostbyname( lpszServerAddress );
	 if( pHostEnt )
	 addr = *(unsigned long*)pHostEnt->h_addr_list[0];
	 else
	 throw new CMTException( SYSERR_SOCKINVALIDADDR, ERRMSG_SOCKINVALIDADDR );
	 }

	 #ifdef		WIN32
	 m_sockAddrIn.sin_addr.S_un.S_addr = addr;
	 #endif		// WIN32

	 #ifdef		_AIX
	 m_sockAddrIn.sin_addr.s_addr = addr;
	 #endif		// _AIX

	 #ifdef		__hpux
	 m_sockAddrIn.sin_addr.s_addr = addr;
	 #endif		// __hpux
	 */

	m_iSockFD = socket(AF_INET, SOCK_STREAM, 0);

	if (m_iSockFD == -1)
		throw new CMTException(SYSERR_SOCKNOTECREATED, ERRMSG_SOCKNOTCREATED);

	if (connect(m_iSockFD, (struct sockaddr *) &m_sockAddrIn,
			sizeof(m_sockAddrIn))) {
//		printf("connect error %d: (%x) %s\n", errno, m_iSockFD, strerror(errno));
		throw new CMTException(SYSERR_SOCKCONNECTIONFAILURE,
				ERRMSG_SOCKCONNECTIONFAILURE);
	}

	m_bConnected = 1;
}

// Close 루틴 및 그와 연루된 루틴 다시 작성 요망
// TIME_WAIT과 shutdown의 문제
void CMTTcpClient::Close(void) {
	if (!m_bConnected)
		return;

#ifdef _WIN32
	LINGER ling = { 0, };
#else
	linger ling = {0,};
#endif

	ling.l_onoff = 1; // LINGER 옵션 사용 여부
	ling.l_linger = 0; // LINGER Timeout 설정

	// LINGER 옵션을 Socket에 적용   
	setsockopt(m_iSockFD, SOL_SOCKET, SO_LINGER, (char*) &ling, sizeof(ling));

#ifdef _WIN32
	closesocket (m_iSockFD);
#else
	//  문제나면 shutdown 막아야할 수도
	shutdown( m_iSockFD, 0x02 );
	close( m_iSockFD );
#endif

	m_iSockFD = -1;
	m_bConnected = 0;
}

void CMTTcpClient::Write(unsigned char* pBuffer, unsigned long ulSize) {
	if (!m_bConnected)
		throw new CMTException(SYSERR_SOCKNOTCONNECTED,
				ERRMSG_SOCKNOTCONNECTED);

	if (ulSize == 0)
		return;

	int iSent, iRetVal;
	unsigned long ulTotalSent = 0;

	do {
		/*
		 FD_ZERO( &m_writeFDs );
		 FD_SET( (unsigned int)m_iSockFD, &m_writeFDs );

		 #ifdef	WIN32
		 iRetVal = select( 2, 0, &m_writeFDs, 0, m_bCheckSendTimeout ? &m_sendTimeOut : 0 );
		 #else
		 iRetVal = select( FD_SETSIZE, 0, &m_writeFDs, 0, m_bCheckSendTimeout ? &m_sendTimeOut : 0 );
		 #endif	//

		 if( iRetVal == 0 )
		 throw new CMTException( SYSERR_SOCKWRITESENDTIMEOUT, ERRMSG_SOCKWRITESENDTIMEOUT );
		 if( iRetVal < 0 )
		 throw new CMTException( SYSERR_SOCKWRITEFAILURE, ERRMSG_SOCKWRITEFAILURE );
		 */
		iSent = send(m_iSockFD, (char *) pBuffer + ulTotalSent,
				ulSize - ulTotalSent, 0);

		if (iSent < 0)
			throw new CMTException(SYSERR_SOCKWRITEFAILURE,
					ERRMSG_SOCKWRITEFAILURE);

		ulTotalSent += iSent;

	} while (ulTotalSent < ulSize);

}

void CMTTcpClient::Read(unsigned char* pBuffer, unsigned long ulSize) {
printf("CMTTcpClient::Read(01) ulSize =[%d] \n",ulSize);
	if (!m_bConnected)
		throw new CMTException(SYSERR_SOCKNOTCONNECTED,
				ERRMSG_SOCKNOTCONNECTED);
	if (ulSize == 0)
		return;

	int iReceived, iRetVal;
	unsigned long ulTotalReceived = 0;

	do {
		FD_ZERO (&m_readFDs);
		FD_SET((unsigned int) m_iSockFD, &m_readFDs);

	    m_recvTimeOut.tv_sec = _ACC_TIMEOUT;
	    m_recvTimeOut.tv_usec =_RECV_TIMEOUT;	

#ifdef	WIN32
		iRetVal = select(2, &m_readFDs, 0, 0,	m_bCheckRecvTimeout ? &m_recvTimeOut : 0);
#else
		iRetVal = select( FD_SETSIZE, &m_readFDs, 0, 0, &m_recvTimeOut );// m_bCheckRecvTimeout ? &m_recvTimeOut : 0 );
#endif	//
		if (iRetVal == 0) {
			throw new CMTException(SYSERR_SOCKREADRECVTIMEOUT,
					ERRMSG_SOCKREADRECVTIMEOUT);
		}

		if (iRetVal < 0) {
			throw new CMTException(SYSERR_SOCKREADFAILURE,
					ERRMSG_SOCKREADFAILURE);
		}

		iReceived = recv(m_iSockFD, (char *) pBuffer + ulTotalReceived,
				ulSize - ulTotalReceived, 0);

		if (iReceived < 0) {
//			printf("recv : %d %d\n", iReceived, WSAGetLastError() );
//			printf("Receive error %d: (%x) %s\n", errno, m_iSockFD, strerror(errno));

			throw new CMTException(SYSERR_SOCKREADFAILURE,
					ERRMSG_SOCKREADFAILURE);
		}

		if (iReceived == 0) {
			throw new CMTException(SYSERR_SOCKCLOSED, ERRMSG_SOCKCLOSED);
		}

		ulTotalReceived += iReceived;

	} while (ulTotalReceived < ulSize);
}

void CMTTcpClient::Delete(void) {
	delete this;
}

void CMTTcpClient::GetRemoteAddr(char lpszAddr[16]) const {
#ifdef		WIN32
	sprintf(lpszAddr, "%u.%u.%u.%u", m_sockAddrIn.sin_addr.S_un.S_un_b.s_b1,
			m_sockAddrIn.sin_addr.S_un.S_un_b.s_b2,
			m_sockAddrIn.sin_addr.S_un.S_un_b.s_b3,
			m_sockAddrIn.sin_addr.S_un.S_un_b.s_b4);
#else
	unsigned char *pCh = (unsigned char*)&m_sockAddrIn.sin_addr.s_addr;
	sprintf( lpszAddr, "%u.%u.%u.%u", pCh[0], pCh[1], pCh[2], pCh[3]);
#endif		//__hpux
}

