#include	<stdio.h>
#include	<string.h>
#include    <errno.h>
#include    <sys/types.h>
#ifdef WIN32
#include <sys/timeb.h>
#include <time.h>
#endif

#include "MTExcept.h"
#include "MTUtil.h"
#include "MTSysMsg.h"
#include "MTChiSoc.h"

//#define  _RCV_SEL_TIME   5000000
//#define  _SND_SEL_TIME   5000000

extern int errno;

CMTChildSoc::CMTChildSoc(int iSockFD, sockaddr_in sockAddrIn,
		unsigned long ulBufferSize, long lClRecvTimeout, long lClSendTimeout) {
	m_iSockFD = iSockFD;
	m_sockAddrIn = sockAddrIn;
	m_sockAddrLen = sizeof(sockaddr);

//printf("soc create : %x\n", m_iSockFD );

#ifdef WIN32
	struct _timeb timebuffer;
	_ftime(&timebuffer);
	m_acceptStamp.tv_sec = timebuffer.time;
	m_acceptStamp.tv_usec = timebuffer.millitm * 1000;
#else		   
	gettimeofday(&m_acceptStamp, NULL);
#endif

	if (lClRecvTimeout == 0)
		m_bCheckRecvTimeout = 0;
	else {
		m_bCheckRecvTimeout = 1;
		m_recvTimeOut.tv_sec = lClRecvTimeout / 1000;
		m_recvTimeOut.tv_usec = (lClRecvTimeout % 1000) * 1000;
	}

	if (lClSendTimeout == 0)
		m_bCheckSendTimeout = 0;
	else {
		m_bCheckSendTimeout = 1;
		m_sendTimeOut.tv_sec = lClSendTimeout / 1000;
		m_sendTimeOut.tv_usec = (lClSendTimeout % 1000) * 1000;
	}

//	SetTimeOutVar(lClRecvTimeout, lClSendTimeout);

}

CMTChildSoc::~CMTChildSoc() {
	try {
		if (m_iSockFD != -1)
			Close();
	} catch (...) {
		printf("Unknown exceptions...\n");
	}
}

void CMTChildSoc::Write(unsigned char* pBuffer, unsigned long ulSize) {
	if (ulSize == 0)
		return;

	int iSent, iRetVal;
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
		iSent = send(m_iSockFD, (char *) pBuffer + ulTotalSent,
				ulSize - ulTotalSent, 0);

		if (iSent < 0) {
//			printf("Write errno 3: %d %s\n", errno, strerror(errno));
			throw new CMTException(SYSERR_SOCKWRITEFAILURE,
					ERRMSG_SOCKWRITEFAILURE);
		}

		ulTotalSent += iSent;

	} while (ulTotalSent < ulSize);
}

void CMTChildSoc::Close(void) {
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

void CMTChildSoc::Read(unsigned char* pBuffer, unsigned long ulSize) {
	if (ulSize == 0)
		return;

	int iReceived, iRetVal;
	unsigned long ulTotalReceived = 0;

	FD_ZERO (&m_readFDs);
	FD_SET((unsigned int) m_iSockFD, &m_readFDs);
    m_recvTimeOut.tv_sec = 1;
 	m_recvTimeOut.tv_usec =_RECV_TIMEOUT;	
#ifdef	WIN32
	iRetVal = select(2, &m_readFDs, 0, 0,	m_bCheckRecvTimeout ? &m_recvTimeOut : 0);
#else
//  DGB Test를 위해서 잠시...
	iRetVal = select( FD_SETSIZE, &m_readFDs, 0, 0,  &m_recvTimeOut  );
//	iRetVal = select( 1, &m_readFDs, 0, 0, m_bCheckRecvTimeout ? &m_recvTimeOut : 0 );
#endif	//__hpux
	if (iRetVal == 0) {
		throw new CMTException(SYSERR_SOCKREADRECVTIMEOUT,
				ERRMSG_SOCKREADRECVTIMEOUT);
	}

	if (iRetVal < 0) {
		throw new CMTException(SYSERR_SOCKREADFAILURE, ERRMSG_SOCKREADFAILURE_04);
	}

	do {
//		if ( CheckTimeOut( m_lRecvTimeOutInterval ) ) {
//			throw new CMTException( SYSERR_SOCKREADRECVTIMEOUT, ERRMSG_SOCKREADRECVTIMEOUT );
//		}
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

void CMTChildSoc::Delete(void) {
	delete this;
}

void CMTChildSoc::GetRemoteAddr(char lpszAddr[16]) const {
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

bool CMTChildSoc::CheckTimeOut(long TimeoutInterval) {
#ifdef _OS390
	struct timeval stamp;

	gettimeofday(&stamp, NULL);

	if ( (stamp.tv_sec - m_acceptStamp.tv_sec ) < TimeoutInterval ) return false;
	else return true;
#else
	return false;
#endif
}

/*
 void CMTChildSoc::SetTimeOutVar(long lClRecvTimeout, long lClSendTimeout)
 {
 m_lRecvTimeOutInterval = lClRecvTimeout;
 m_lSendTimeOutInterval = lClSendTimeout;
 }
 */

CMTChildSoc* CMTChildSoc::Create(int iSockFD, sockaddr_in sockAddrIn,
		unsigned long ulBufferSize, long lClRecvTimeout, long lClSendTimeout) {
	return new CMTChildSoc(iSockFD, sockAddrIn, ulBufferSize, lClRecvTimeout,
			lClSendTimeout);
}

