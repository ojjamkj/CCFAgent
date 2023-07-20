// MTTcpServer.cpp: implementation of the CMRCSQueue class.
//
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#ifdef WIN32
#include <sys/timeb.h>
#include <time.h>
#else
#include <errno.h>
extern int errno;
#endif

#include    "MRCSComm.h"
#include	"MTExcept.h"
#include	"MRCSQueu.h"
#include    "MTCtSect.h"
#include	"BREXLog.h"

#ifdef _MSG_KO
#define   MSG_SOCKET_ERR    "TCPIP 소켓 수신 오류.\n"
#define   MSG_TCPIP_EXPT    "TCPIP 수신 시에 예외가 발생하였습니다."

#define   MSG_WAITQ_LIMIT   "대기 큐가 한계에 도달하였습니다."
#else
#define   MSG_SOCKET_ERR    "TCPIP Socket Accept Error.\n"
#define   MSG_TCPIP_EXPT    "Exception occurs in TcpIp accept."

#define   MSG_WAITQ_LIMIT   "Arrived to limit of wait queue."
#endif

CMTCriticalSection m_CSect_Q;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMRCSQueue::CMRCSQueue() {
//  configRec.
//            ulBufferSize
//            ulRecvTimeout
//            ulSendTimeout
//            usPort
//            ulNoMaxQueueing

	m_ulClBufferSize = configRec.ulBufferSize;

	if (configRec.ulRecvTimeout == 0)
		m_bCheckRecvTimeout = 0;
	else {
		m_bCheckRecvTimeout = 1;
		m_recvTimeOut.tv_sec = configRec.ulRecvTimeout / 1000;
		m_recvTimeOut.tv_usec = (configRec.ulRecvTimeout % 1000) * 1000;
	}

	if (configRec.ulSendTimeout == 0)
		m_bCheckSendTimeout = 0;
	else {
		m_bCheckSendTimeout = 1;
		m_sendTimeOut.tv_sec = configRec.ulSendTimeout / 1000;
		m_sendTimeOut.tv_usec = (configRec.ulSendTimeout % 1000) * 1000;
	}

//printf("  %d %d %d\n", ulClBufferSize,
//        m_lClRecvTimeout, m_lClSendTimeout );

	m_usPort = configRec.usPort;

	m_MaxQueueCount = (long) configRec.ulNoMaxQueueing;

	m_pFirst = -1;
	m_pLast = 0;
	m_QueueCount = 0;

	m_socInfo = (SOC_INFO*) malloc(sizeof(SOC_INFO) * m_MaxQueueCount);

	for (int i = 0; i < m_MaxQueueCount; i++)
		m_socInfo[i].use = 0;
}

CMRCSQueue::~CMRCSQueue() {
	Close();
}

/*
 int		CMRCSQueue::getQueueCount()
 {
 int cnt=0;

 for ( int i=0; i<m_MaxQueueCount; i++ ) if ( m_socInfo[i].use == 1 ) cnt++;

 return cnt;
 }
 */

void CMRCSQueue::getQueueCount(int *nWait, int *nRun) {
	int cnt1 = 0, cnt2 = 0;

	for (int i = 0; i < m_MaxQueueCount; i++) {
		if (m_socInfo[i].use == 1)
			cnt1++;
		if (m_socInfo[i].use == 2)
			cnt2++;
	}

	*nWait = cnt1;
	*nRun = cnt2;
}

void CMRCSQueue::getQueueInfo(char *msg) {
	int cnt1 = 0, cnt2 = 0;

	for (int i = 0; i < m_MaxQueueCount; i++) {
		if (m_socInfo[i].use == 1)
			cnt1++;
		if (m_socInfo[i].use == 2)
			cnt2++;
	}

	sprintf(msg,
			"Queue Information\n            Max(%d)  Wait(%d)  Run(%d)  Front(%d)  Tail(%d)\n",
			m_MaxQueueCount, cnt1, cnt2, m_pFirst, m_pLast);
}

void CMRCSQueue::Init(int iBackLog) {
	m_iSockFD = socket(AF_INET, SOCK_STREAM, 0);

	m_sockAddrIn.sin_family = AF_INET;
#ifdef		WIN32
	m_sockAddrIn.sin_addr.s_addr = htonl(INADDR_ANY);
#else
	m_sockAddrIn.sin_addr.s_addr = INADDR_ANY;
#endif

	m_sockAddrIn.sin_port = htons(m_usPort);

	m_iBackLog = iBackLog;
	m_bStop = 0;

//  2004.10.01 for Test @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	unsigned long bReuseAddr = 1;
	setsockopt(m_iSockFD, SOL_SOCKET, SO_REUSEADDR, (const char*) &bReuseAddr,
			sizeof(unsigned long));

	if (bind(m_iSockFD, (struct sockaddr*) &m_sockAddrIn, sizeof(m_sockAddrIn)))
		throw new CMTException(SYSERR_SOCKBINDFAILURE, ERRMSG_SOCKBINDFAILURE);

	if (listen(m_iSockFD, m_iBackLog))
		throw new CMTException(SYSERR_SOCKLISTENFAILURE,
				ERRMSG_SOCKLISTENFAILURE);
}

void CMRCSQueue::Run(void) {
	int iSockClient;
	sockaddr_in clSockAddr;
	int iSockAddrLen = sizeof(sockaddr_in);
	int iRetVal;
	int idx;

// printf("TCPIP wait request.\n");

	FD_ZERO (&m_acceptFD);
	try {
		while (!m_bStop) {

//printf("1");
            FD_ZERO (&m_acceptFD);
			FD_SET((unsigned int) m_iSockFD, &m_acceptFD);
        //2015.10.21 베트남 linux 에서 해당 값 reset 발생되어 추가 함.
    	m_timePollingAccept.tv_sec = 1;
    	m_timePollingAccept.tv_usec = 500000; //  0.5 second

		

#ifdef	WIN32
			iRetVal = select(2, &m_acceptFD, 0, 0, &m_timePollingAccept);
#else
			iRetVal = select( FD_SETSIZE, &m_acceptFD, 0, 0, &m_timePollingAccept );
#endif
//printf(" 2(%d)\n", iRetVal);
			if (iRetVal < 0) {
				//printf("TCPIP Socket Accept Error.\n", iSockClient);
				printf(MSG_SOCKET_ERR, iSockClient);
				throw new CMTException(SYSERR_SOCKACCEPTFAILURE,
						ERRMSG_SOCKACCEPTFAILURE);
			}

//printf(" 3(%d)\n", WSAGetLastError() );

			if (iRetVal == 0)
				continue;

#if defined(WIN32) || defined(__hpux)
			if ((iSockClient = accept(m_iSockFD, (struct sockaddr*) &clSockAddr,&iSockAddrLen))!= (int) (~0))
#else
			if( ( iSockClient = accept( m_iSockFD, (struct sockaddr*)&clSockAddr, (socklen_t*)&iSockAddrLen ) ) != (int)(~0) )
#endif
			{

				if ((idx = pushSocket(iSockClient, clSockAddr)) >= 0) {

					OnAccept(idx);

				} else {

//					printf("대기행렬이 Max에 도달했습니다.\n");
					//_WriteLog(_LOG_LOG, "Arrived to limit of wait queue.");
					_WriteLog(_LOG_ERR, MSG_WAITQ_LIMIT);
					StopListen();
#ifdef _WIN32
					closesocket(iSockClient);
#else
					shutdown( iSockClient, 0x02 );
					close( iSockClient );
#endif

				}
			}

//			OnAccept( CMTChildSoc::Create( iSockClient, clSockAddr, m_ulClBufferSize, m_lClRecvTimeout, m_lClSendTimeout ) );

		}
	} catch (...) {/*
		throw new CMTException(SYSERR_SOCKACCEPTFAILURE,
				"Exception occurs in TcpIp accept.");
		printf("Exception occurs in TcpIp accept.");*/
		throw new CMTException(SYSERR_SOCKACCEPTFAILURE,MSG_TCPIP_EXPT);
		printf(MSG_TCPIP_EXPT);
	}
}

void CMRCSQueue::Close(void) {
	if (m_iSockFD >= 0) {

#ifdef _WIN32
		closesocket (m_iSockFD);
#else
		shutdown( m_iSockFD, 0x02 );
		close( m_iSockFD );
#endif

		m_iSockFD = -1;
	}
}

void CMRCSQueue::OnAccept(int cliSoc) {
}

int CMRCSQueue::SocRead(int idx, unsigned char* pBuffer, unsigned long ulSize) {
	if (ulSize == 0)
		return 0;

	fd_set m_readFDs;
	int iReceived, iRetVal;
	unsigned long ulTotalReceived = 0;

	FD_ZERO(&m_readFDs);
	FD_SET((unsigned int) m_socInfo[idx].soc, &m_readFDs);
  
    m_recvTimeOut.tv_sec = 1;
 	m_recvTimeOut.tv_usec =500000;	
 	
//m_recvTimeOut.tv_sec = 1;

//printf(" Wait : %d.%d\n", m_recvTimeOut.tv_sec, m_recvTimeOut.tv_usec );

#ifdef	WIN32
	iRetVal = select(2, &m_readFDs, 0, 0,	m_bCheckRecvTimeout ? &m_recvTimeOut : 0);
#else
//  DGB Test를 위해서 잠시...
	iRetVal = select( FD_SETSIZE, &m_readFDs, 0, 0,    &m_recvTimeOut  );
//	iRetVal = select( 1, &m_readFDs, 0, 0, m_bCheckRecvTimeout ? &m_recvTimeOut : 0 );
#endif	//__hpux
	if (iRetVal == 0) {
		return SYSERR_SOCKREADRECVTIMEOUT;
//		throw new CMTException( SYSERR_SOCKREADRECVTIMEOUT, ERRMSG_SOCKREADRECVTIMEOUT );
	}

	if (iRetVal < 0) {
		return SYSERR_SOCKREADFAILURE;
//		throw new CMTException( SYSERR_SOCKREADFAILURE, ERRMSG_SOCKREADFAILURE );
	}

	do {
		iReceived = recv(m_socInfo[idx].soc, (char *) pBuffer + ulTotalReceived,
				ulSize - ulTotalReceived, 0);

		if (iReceived < 0) {
			return SYSERR_SOCKREADFAILURE;
//			throw new CMTException( SYSERR_SOCKREADFAILURE, ERRMSG_SOCKREADFAILURE );
		}

		if (iReceived == 0) {
			return SYSERR_SOCKCLOSED;
//			throw new CMTException( SYSERR_SOCKCLOSED, ERRMSG_SOCKCLOSED );
		}

		ulTotalReceived += iReceived;
	} while (ulTotalReceived < ulSize);

	return 0;
}

void CMRCSQueue::SocWriteString(int idx, char* lpsz) {

	if (lpsz == NULL)return;
	int leng = strlen(lpsz);
	leng = leng + 2 + 1;
	char *m_BufferTemp= (char*)malloc(leng);
	m_BufferTemp[0] = (unsigned char)(leng / 256);
	m_BufferTemp[ 1] = (unsigned char)(leng % 256);
	memcpy(m_BufferTemp + 2, (char*)lpsz, leng-2);
	m_BufferTemp[leng-1] = 0;
	printf("==>%s \n", m_BufferTemp);
	SocWrite(idx,(unsigned char*) m_BufferTemp, leng);
	printf("==>%s \n", m_BufferTemp+2);
	free(m_BufferTemp);
}
void CMRCSQueue::SocWrite(int idx, unsigned char* pBuffer,
		unsigned long ulSize) {
	if (ulSize == 0)
		return;

	int iSent;
	unsigned long ulTotalSent = 0;
//    fd_set          m_writeFDs;

	do {
		iSent = send(m_socInfo[idx].soc, (char *) pBuffer + ulTotalSent,
				ulSize - ulTotalSent, 0);
		printf("\tSocWrite  iSent[%d] \n", iSent);
		if (iSent < 0) {
			printf("\tSocWrite  iSent[%d] exception \n", iSent);
			throw new CMTException(SYSERR_SOCKWRITEFAILURE,
					ERRMSG_SOCKWRITEFAILURE);
		}

		ulTotalSent += iSent;

	} while (ulTotalSent < ulSize);
}

void CMRCSQueue::CloseChildSoc(int idx) {
#ifdef _WIN32
	LINGER ling = { 0, };
#else
	linger ling = {0,};
#endif

	ling.l_onoff = 1; // LINGER 옵션 사용 여부
	ling.l_linger = 0; // LINGER Timeout 설정

	// LINGER 옵션을 Socket에 적용   
	setsockopt(m_socInfo[idx].soc, SOL_SOCKET, SO_LINGER, (char*) &ling,
			sizeof(ling));

#ifdef _WIN32
	closesocket(m_socInfo[idx].soc);
#else
	shutdown( m_socInfo[idx].soc, 0x02 );
	close( m_socInfo[idx].soc );
#endif  

	m_CSect_Q.Lock();

	m_socInfo[idx].use = 0;
	m_QueueCount--;

//printf("CloseChildSoc  m_QueueCount : %d\n", m_QueueCount);

	m_CSect_Q.Unlock();

}

int CMRCSQueue::popSocket(void) {
	m_CSect_Q.Lock();

	if (m_QueueCount <= 0) {
		m_CSect_Q.Unlock();
		return -1;
	}

	int pos = m_pFirst;
	int selIndex = -1;

	do {
		if (++m_pFirst == m_MaxQueueCount)
			m_pFirst = 0;

		if (m_socInfo[m_pFirst].use == 1) {
			m_socInfo[m_pFirst].use = 2;
			selIndex = m_pFirst;
			break;
		}
	} while (pos != m_pFirst);

	m_CSect_Q.Unlock();

	return selIndex;
}

int CMRCSQueue::pushSocket(int SocCli, sockaddr_in clSockAddr) {
//printf("push Q : %d\n", m_QueueCount);

	m_CSect_Q.Lock();

	int pos = m_pLast;
	int selIndex = -1;
	int chkCnt = 0;

	do {
		if (!m_socInfo[m_pLast].use) {
			m_socInfo[m_pLast].soc = SocCli;
#ifndef _RUNNER
			memcpy((char*) &m_socInfo[m_pLast].sockAddr, (char*) &clSockAddr,
					sizeof(sockaddr_in));
#endif
			m_socInfo[m_pLast].use = 1;
			m_QueueCount++;

#ifdef WIN32
			struct _timeb timebuffer;
			_ftime(&timebuffer);
			m_socInfo[m_pLast].accStamp.tv_sec = (long) timebuffer.time;
			m_socInfo[m_pLast].accStamp.tv_usec = (long) timebuffer.millitm;
#else
			gettimeofday(&m_socInfo[m_pLast].accStamp, NULL);
#endif			
			selIndex = m_pLast;
			if (++m_pLast == m_MaxQueueCount)
				m_pLast = 0;
			break;
		}

		if (++m_pLast == m_MaxQueueCount)
			m_pLast = 0;

		if (++chkCnt == m_MaxQueueCount)
			break;

	} while (pos != m_pLast);

	m_CSect_Q.Unlock();

	return selIndex;
}

void CMRCSQueue::StopListen(void) {
	m_bStop = 1;
}

void CMRCSQueue::RemoveAllQueue(void) {
//	printf("\nClose child socket\n" );

	for (int idx = 0; idx < m_MaxQueueCount; idx++) {

		if (m_socInfo[idx].use) {

			CloseChildSoc(idx);

//printf("    child %02d\n", idx );

//			m_socInfo[idx].use = 0;
//			shutdown( m_socInfo[idx].soc, 0x02 );
//			close( m_socInfo[idx].soc );
		}
	}

//	printf("\n");
}

void CMRCSQueue::GetRemoteAddr(int idx, char lpszAddr[16]) const {
#ifndef _RUNNER
	struct in_addr *psin_addr;

	psin_addr = (struct in_addr*) &m_socInfo[idx].sockAddr.sin_addr;

#ifdef		WIN32
	sprintf(lpszAddr, "%u.%u.%u.%u", psin_addr->S_un.S_un_b.s_b1,
			psin_addr->S_un.S_un_b.s_b2, psin_addr->S_un.S_un_b.s_b3,
			psin_addr->S_un.S_un_b.s_b4);
#else
	unsigned char *pCh = (unsigned char*)& psin_addr->s_addr;

	sprintf( lpszAddr,
			"%u.%u.%u.%u",
			pCh[0],
			pCh[1],
			pCh[2],
			pCh[3]
	);
#endif
#endif
}

long CMRCSQueue::AcceptTimeSec(int idx) {
	return m_socInfo[idx].accStamp.tv_sec;
}

long CMRCSQueue::AcceptTimeUSec(int idx) {
	return m_socInfo[idx].accStamp.tv_usec;
}

