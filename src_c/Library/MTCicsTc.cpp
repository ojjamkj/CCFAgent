// MTCicsTc.cpp: implementation of the CMTTcpClient class.
//               OS/390  CICS Socket
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#ifdef _RENT                 
//   #include <cmanifes.h>     
//#else                        
#include <manifest.h>
//#endif                       

#include <bsdtypes.h>        
#include <in.h>              
#include <socket.h>          

//#ifdef _RENT                 
//   #include <errno.h>        
//#else                        
extern char tok;
extern long int dzero;
extern int fzero;
extern short int hzero;
extern int socksize;
extern char sock;
extern int nonzero;
extern int errno;
#pragma checkout(resume)
//#endif  

#include "MTSysMsg.h"
#include "MTExcept.h"
#include "MTCicsTc.h"

#ifndef true
#define   true    1
#define   false   0
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

struct sockaddr_in m_sockAddrIn;
int m_iSockFD;
unsigned long m_bConnected;
char m_errMessage[256];

unsigned int getAddr(char *IpAddr) {
	union {
		unsigned int addr;
		unsigned char addc[4];
	} u;

	char buf[20], *p, *q;
	int ch = '.';

	strcpy(buf, IpAddr);
	p = buf;

	q = strchr(p, ch);
	*q = 0;
	u.addc[0] = (unsigned char) atoi(p);

	p = q + 1;
	q = strchr(p, ch);
	*q = 0;
	u.addc[1] = (unsigned char) atoi(p);

	p = q + 1;
	q = strchr(p, ch);
	*q = 0;
	u.addc[2] = (unsigned char) atoi(p);

	p = q + 1;
	u.addc[3] = (unsigned char) atoi(p);

	return u.addr;
}

unsigned long SOC_Open(char *lpszServerAddress, unsigned short ulPort) {
	long rc;

	if (m_bConnected)
		return 0;

	m_sockAddrIn.sin_family = AF_INET;

	if (ulPort < 1000 || ulPort > 99999)
		ulPort = 33333;
	m_sockAddrIn.sin_port = htons(ulPort);

	/*
	 if( u.addr == INADDR_NONE )
	 {
	 struct hostent	*pHostEnt;

	 pHostEnt = gethostbyname( lpszServerAddress );
	 if( pHostEnt )
	 u.addr = *(unsigned long*)pHostEnt->h_addr_list[0];
	 else
	 throw new CMTException( SYSERR_SOCKINVALIDADDR, ERRMSG_SOCKINVALIDADDR );
	 }
	 */

	m_sockAddrIn.sin_addr.s_addr = (unsigned long) getAddr(lpszServerAddress);

//   printf("MTCICSTC Socket.\n" );

	m_iSockFD = socket(AF_INET, SOCK_STREAM, 0);

//   printf("MTCICSTC Socket : %d\n", errno );

	if (m_iSockFD == -1) {
		sprintf(m_errMessage, "Socket not created : %d\n", errno);
		return -1;
	}

#ifdef _DEBUG
	printf("Socket created : %d\n", m_iSockFD );
#endif

	rc = connect(m_iSockFD, (struct sockaddr *) &m_sockAddrIn,
			sizeof(m_sockAddrIn));

	if (rc) {
		sprintf(m_errMessage, "Socket connect fail : %d %d\n", rc, errno);
		return -1;
	}

	m_bConnected = 1;

	return 0;
}

// TIME_WAIT관련 수정해야 ....
void SOC_Close(void) {
	if (m_bConnected) {
//		shutdown( m_iSockFD, 0x02 );

		close(m_iSockFD);
		m_iSockFD = -1;
		m_bConnected = 0;
	}
}

unsigned long SOC_Write(unsigned char* pBuffer, unsigned long ulSize) {
	if (!m_bConnected) {
		strcpy(m_errMessage, ERRMSG_SOCKNOTCONNECTED);
		return SYSERR_SOCKNOTCONNECTED;
	}

	if (ulSize == 0)
		return 0;

	int iSent, iRetVal;
	unsigned long ulTotalSent = 0;

	do {
		/*
		 FD_ZERO( &m_writeFDs );
		 FD_SET( (unsigned int)m_iSockFD, &m_writeFDs );

		 iRetVal = select( FD_SETSIZE, 0, &m_writeFDs, 0, m_bCheckSendTimeout ? &m_sendTimeOut : 0 );

		 if( iRetVal == 0 )
		 throw new CMTException( SYSERR_SOCKWRITESENDTIMEOUT, ERRMSG_SOCKWRITESENDTIMEOUT );
		 if( iRetVal < 0 )
		 throw new CMTException( SYSERR_SOCKWRITEFAILURE, ERRMSG_SOCKWRITEFAILURE );
		 */
		iSent = send(m_iSockFD, (char *) pBuffer + ulTotalSent,
				ulSize - ulTotalSent, 0);

		if (iSent < 0) {
			strcpy(m_errMessage, ERRMSG_SOCKWRITEFAILURE);
			return SYSERR_SOCKWRITEFAILURE;
		}

		ulTotalSent += iSent;

	} while (ulTotalSent < ulSize);

	return 0;
}

unsigned long SOC_Read(unsigned char* pBuffer, unsigned long ulSize) {
	if (!m_bConnected) {
		strcpy(m_errMessage, ERRMSG_SOCKNOTCONNECTED);
		return SYSERR_SOCKNOTCONNECTED;
	}

	if (ulSize == 0)
		return 0;

	int iReceived, iRetVal;
	unsigned long ulTotalReceived = 0;

	do {
		/*
		 FD_ZERO( &m_readFDs );
		 FD_SET( (unsigned int)m_iSockFD, &m_readFDs );


		 iRetVal = select( FD_SETSIZE, &m_readFDs, 0, 0, m_bCheckRecvTimeout ? &m_recvTimeOut : 0 );


		 if( iRetVal == 0 )
		 throw new CMTException( SYSERR_SOCKREADRECVTIMEOUT, ERRMSG_SOCKREADRECVTIMEOUT );
		 if( iRetVal < 0 )
		 throw new CMTException( SYSERR_SOCKREADFAILURE, ERRMSG_SOCKREADFAILURE );
		 */
		iReceived = recv(m_iSockFD, (char *) pBuffer + ulTotalReceived,
				ulSize - ulTotalReceived, 0);

		if (iReceived < 0) {
			strcpy(m_errMessage, ERRMSG_SOCKREADFAILURE);
			return SYSERR_SOCKREADFAILURE;
		}

		if (iReceived == 0) {
			strcpy(m_errMessage, ERRMSG_SOCKCLOSED);
			return SYSERR_SOCKCLOSED;
		}

		ulTotalReceived += iReceived;

	} while (ulTotalReceived < ulSize);

	return 0;
}

char * getSocketMessage(char *msg) {
	strcpy(msg, m_errMessage);

	return msg;
}
