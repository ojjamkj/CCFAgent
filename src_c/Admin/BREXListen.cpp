// BREXListen.cpp: implementation of the CBREXListen class.
//
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

#ifdef WIN32
#include <sys/timeb.h>
#include <time.h>
#else
extern int errno;
#endif

#include "MTExcept.h"
#include "BREXListen.h"

#ifdef _MSG_KO
#define   MSG_SOCKET_ERR    "TCP/IP 소켓 수신 오류.\n"
#else
#define   MSG_SOCKET_ERR    "TCPIP Socket Accept Error.\n"
#endif

static struct timeval m_recvTimeOut;
static struct timeval m_sendTimeOut;

CBREXListen::CBREXListen() {
	m_timePollingAccept.tv_sec = _ACC_TIMEOUT;
	m_timePollingAccept.tv_usec = 0;

	m_recvTimeOut.tv_sec = 1;
	m_recvTimeOut.tv_usec = _RECV_TIMEOUT;

	m_sendTimeOut.tv_sec = 0;
	m_sendTimeOut.tv_usec = _SEND_TIMEOUT;
}

CBREXListen::~CBREXListen() {
	Close();
}

void CBREXListen::Init(unsigned short usPort) {
	m_usPort = usPort;

	m_iSockFD = socket(AF_INET, SOCK_STREAM, 0);

	m_sockAddrIn.sin_family = AF_INET;
#ifdef		WIN32
	m_sockAddrIn.sin_addr.s_addr = htonl(INADDR_ANY);
#else
	m_sockAddrIn.sin_addr.s_addr = INADDR_ANY;
#endif

	m_sockAddrIn.sin_port = htons(m_usPort);

	m_bStop = 0;

	FD_ZERO (&m_acceptFD);
	FD_SET((unsigned int) m_iSockFD, &m_acceptFD);

	unsigned long bReuseAddr = 1;
	setsockopt(m_iSockFD, SOL_SOCKET, SO_REUSEADDR, (const char*) &bReuseAddr,
			sizeof(unsigned long));

	if (bind(m_iSockFD, (struct sockaddr*) &m_sockAddrIn, sizeof(m_sockAddrIn)))
		throw new CMTException(SYSERR_SOCKBINDFAILURE, ERRMSG_SOCKBINDFAILURE);

	if (listen(m_iSockFD, _BACK_LOG))
		throw new CMTException(SYSERR_SOCKLISTENFAILURE,
				ERRMSG_SOCKLISTENFAILURE);

	StartListen();
}

void CBREXListen::StartListen(void) {
	int iSockClient;
	sockaddr_in clSockAddr;
	int iSockAddrLen = sizeof(sockaddr_in), iRetVal;

	try {
		while (!m_bStop) {


        //2015.10.21 베트남 linux 에서 해당 값 reset 발생되어 추가 함.
			FD_SET((unsigned int) m_iSockFD, &m_acceptFD);
		  m_timePollingAccept.tv_sec = _ACC_TIMEOUT;
		  m_timePollingAccept.tv_usec =0;	
        //2015.10.21 베트남 linux 에서 해당 값 reset 발생되어 추가 .
//printf("BREXListen select \n");
#ifdef	WIN32
			iRetVal = select(2, &m_acceptFD, 0, 0, &m_timePollingAccept);
#else
			iRetVal = select( FD_SETSIZE, &m_acceptFD, 0, 0, &m_timePollingAccept );
#endif
//printf("BREXListen select end  iRetVal[%d]\n", iRetVal);
//printf("iRetVal(%d)\n", iRetVal);
			if (iRetVal < 0) {
				//printf("TCPIP Socket Accept Error.\n", iSockClient);
				printf(MSG_SOCKET_ERR, iSockClient);
				throw new CMTException(SYSERR_SOCKACCEPTFAILURE,
						ERRMSG_SOCKACCEPTFAILURE);
			}

			if (iRetVal == 0)
				continue;

#if defined(WIN32) || defined(__hpux)
			if ((iSockClient = accept(m_iSockFD, (struct sockaddr*) &clSockAddr,&iSockAddrLen))!= (int) (~0))
#else
				if( ( iSockClient = accept( m_iSockFD, (struct sockaddr*)&clSockAddr, (socklen_t*)&iSockAddrLen ) ) != (int)(~0) )
#endif
					{
				OnAccept(new CBREXChiSoc(iSockClient, clSockAddr));
			}
		}
	} catch (...) {
		throw new CMTException(SYSERR_SOCKACCEPTFAILURE,
				"Exception occurs in TcpIp accept.");
	}
}

void CBREXListen::Close(void) {
	if (m_iSockFD <= 0)
		return;

	m_bStop = 1;

#ifdef _WIN32
	closesocket (m_iSockFD);
#else
	shutdown( m_iSockFD, 0x02 );
	close( m_iSockFD );
#endif  

	m_iSockFD = -1;
}

void CBREXListen::OnAccept(CBREXChiSoc* pChild) {
}

void CBREXListen::Stop(void) {
	m_bStop = 1;
}

/*****************************************************************************/

CBREXChiSoc::CBREXChiSoc(int iSockFD, sockaddr_in sockAddrIn) {
	m_iSockFD = iSockFD;
	m_sockAddrIn = sockAddrIn;
	m_sockAddrLen = sizeof(sockaddr);

//printf("soc create : %x\n", m_iSockFD );

#ifdef WIN32
	struct _timeb timebuffer;
	_ftime(&timebuffer);
	m_acceptStamp.tv_sec = (long) timebuffer.time;
	m_acceptStamp.tv_usec = (long) timebuffer.millitm;
#else
	gettimeofday(&m_acceptStamp, NULL);
#endif	

}

CBREXChiSoc::~CBREXChiSoc() {
	Close();
}

void CBREXChiSoc::Close(void) {
	if (m_iSockFD == -1)
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

//printf("soc close : %x\n", m_iSockFD );
	m_iSockFD = -1;
}

void CBREXChiSoc::Write(unsigned char* pBuffer, unsigned long ulSize) {
	if (ulSize == 0)
		return;

	int iSent;
	int				iRetVal;
	unsigned long ulTotalSent = 0;
	do {
		/*		if ( CheckTimeOut( m_lSendTimeOutInterval ) ) {
		 throw new CMTException( SYSERR_SOCKREADRECVTIMEOUT, ERRMSG_SOCKREADRECVTIMEOUT );
		 }

		 FD_ZERO( &m_writeFDs );
		 FD_SET( (unsigned int)m_iSockFD, &m_writeFDs );

		 #ifdef	WIN32
		 iRetVal = select( 2, 0, &m_writeFDs, 0, &m_sendTimeOut );
		 #else
		 iRetVal = select( FD_SETSIZE, 0, &m_writeFDs, 0, &m_sendTimeOut );
		 #endif

		 if( iRetVal == 0 ){
		 throw new CMTException( SYSERR_SOCKWRITESENDTIMEOUT, ERRMSG_SOCKWRITESENDTIMEOUT );
		 }
		 if( iRetVal < 0 ) {
		 throw new CMTException( SYSERR_SOCKWRITEFAILURE, ERRMSG_SOCKWRITEFAILURE );
		 }
		 */
		 //2022-11-01 추가 
		 FD_ZERO( &m_writeFDs );
		 FD_SET( (unsigned int)m_iSockFD, &m_writeFDs );
		 m_sendTimeOut.tv_sec = 1;
   	 	 m_sendTimeOut.tv_usec =_SEND_TIMEOUT;	
   	 
		 #ifdef	WIN32
  		 iRetVal = select( 2, 0, &m_writeFDs, 0, &m_sendTimeOut );
		 #else
	  	 iRetVal = select( FD_SETSIZE, 0, &m_writeFDs, 0, &m_sendTimeOut );
		 #endif

		 if( iRetVal == 0 ){
  		 throw new CMTException( SYSERR_SOCKWRITESENDTIMEOUT, ERRMSG_SOCKWRITESENDTIMEOUT );
		 }
		 if( iRetVal < 0 ) {
	  	 throw new CMTException( SYSERR_SOCKWRITEFAILURE, ERRMSG_SOCKWRITEFAILURE );
		 }

		iSent = send(m_iSockFD, (char *) pBuffer + ulTotalSent,	ulSize - ulTotalSent, 0);

		if (iSent < 0) {
//			printf("Write errno 3: %d %s\n", errno, strerror(errno));
			throw new CMTException(SYSERR_SOCKWRITEFAILURE,
					ERRMSG_SOCKWRITEFAILURE);
		}

		ulTotalSent += iSent;

	} while (ulTotalSent < ulSize);
}

int CBREXChiSoc::Read(unsigned char* pBuffer, unsigned long ulSize) {

	if (ulSize == 0)
		return 0;

	fd_set m_readFDs;
	int iReceived, iRetVal;
	unsigned long ulTotalReceived = 0;

	FD_ZERO(&m_readFDs);
	FD_SET((unsigned int) m_iSockFD, &m_readFDs);
    m_recvTimeOut.tv_sec = 1;
 	m_recvTimeOut.tv_usec =_RECV_TIMEOUT;	

#ifdef	WIN32
	iRetVal = select(2, &m_readFDs, 0, 0, &m_recvTimeOut);
#else
//  DGB Test를 위해서 잠시...
	iRetVal = select( FD_SETSIZE, &m_readFDs, 0, 0, &m_recvTimeOut );
//	iRetVal = select( 1, &m_readFDs, 0, 0, m_bCheckRecvTimeout ? &m_recvTimeOut : 0 );
#endif	//__hpux
	if (iRetVal == 0) {
		throw new CMTException(SYSERR_SOCKREADRECVTIMEOUT,
				ERRMSG_SOCKREADRECVTIMEOUT);
	}

	if (iRetVal < 0) {
		throw new CMTException(SYSERR_SOCKREADFAILURE, ERRMSG_SOCKREADFAILURE_02);
	}

	do {
		iReceived = recv(m_iSockFD, (char *) pBuffer + ulTotalReceived,	ulSize - ulTotalReceived, 0);

		if (iReceived < 0) {
			throw new CMTException(SYSERR_SOCKREADFAILURE, ERRMSG_SOCKREADFAILURE_03);
		}

		if (iReceived == 0) {
			throw new CMTException(SYSERR_SOCKCLOSED, ERRMSG_SOCKCLOSED);
		}

		ulTotalReceived += iReceived;
	} while (ulTotalReceived < ulSize);

	return 0;
}

void CBREXChiSoc::Delete(void) {
	delete this;
}

void CBREXChiSoc::GetRemoteAddr(char lpszAddr[16]) const {
#ifdef		WIN32
	sprintf(lpszAddr, "%u.%u.%u.%u", m_sockAddrIn.sin_addr.S_un.S_un_b.s_b1,
			m_sockAddrIn.sin_addr.S_un.S_un_b.s_b2,
			m_sockAddrIn.sin_addr.S_un.S_un_b.s_b3,
			m_sockAddrIn.sin_addr.S_un.S_un_b.s_b4);
#else
	unsigned char *pCh = (unsigned char*)&m_sockAddrIn.sin_addr.s_addr;

	sprintf( lpszAddr,
			"%u.%u.%u.%u",
			pCh[0],
			pCh[1],
			pCh[2],
			pCh[3]
	);
#endif
}

long CBREXChiSoc::AcceptTimeSec() {
	return m_acceptStamp.tv_sec;
}

long CBREXChiSoc::AcceptTimeUSec() {
	return m_acceptStamp.tv_usec;
}

CBREXChiSoc* CBREXChiSoc::Create(int iSockFD, sockaddr_in sockAddrIn) {
	return new CBREXChiSoc(iSockFD, sockAddrIn);
}

