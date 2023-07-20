#ifndef _INC_BREXLISTEN
#define _INC_BREXLISTEN

#include    "MTSysMsg.h"
#include    "MTSocInc.h"

#ifndef  bool
	#define  bool   unsigned char
	#define  true   1
	#define  false  0
#endif

//#define	_TCP_BUFFER		2048		// Byte
//2022-11-01 변경함.
#define	_ACC_TIMEOUT	   1		//  Second
#define	_RECV_TIMEOUT	  50000		//  miliSecond
#define	_SEND_TIMEOUT	  50000		//  miliSecond
#define	_BACK_LOG		   5
/*2022-11-01 이전
#define	_ACC_TIMEOUT	   1		//  Second
#define	_RECV_TIMEOUT	  50		//  miliSecond
#define	_SEND_TIMEOUT	  50		//  miliSecond
#define	_BACK_LOG		   5
*/

class CBREXChiSoc
{
public:
    typedef     struct tagSockInit
    {
        char            *lpszServerAddress;
        unsigned short  usPort;
    }   SOCKINIT, *LPSOCKINIT;

    int             m_iSockFD;
    sockaddr_in     m_sockAddrIn;

	struct timeval	m_acceptStamp;
    
protected:
    int				m_sockAddrLen;

    fd_set          m_readFDs;
    fd_set          m_writeFDs;

private:
	long			m_lRecvTimeOutInterval;
	long			m_lSendTimeOutInterval;

public:
    CBREXChiSoc( int iSockFD, sockaddr_in sockAddrIn );
    virtual ~CBREXChiSoc();

public:

    void    Close( void );
    int		Read( unsigned char* pBuffer, unsigned long ulSize );
    void    Write( unsigned char* pBuffer, unsigned long ulSize );
    void    Delete( void );

	long	AcceptTimeSec();
	long	AcceptTimeUSec();

public:
    inline const sockaddr_in&   GetSockAddr( void ) const;
    void                        GetRemoteAddr( char lpszAddr[16] ) const;

    static CBREXChiSoc*    Create( int iSockFD, sockaddr_in sockAddrIn );
};


inline const sockaddr_in&   CBREXChiSoc::GetSockAddr( void ) const
{
    return m_sockAddrIn;
}


/*****************************************************************************/


class CBREXListen
{
protected:
	unsigned short  m_usPort;
	int             m_iSockFD;

	sockaddr_in     m_sockAddrIn;
	unsigned long   m_ulClBufferSize;
	fd_set          m_acceptFD;

	volatile unsigned long  m_bStop;
	struct timeval  m_timePollingAccept;

public:
    CBREXListen();
    virtual ~CBREXListen();

public:
    void    Init( unsigned short usPort );
    void    Close( void );
    void    Stop( void );

// overrides
protected:
    virtual void    OnAccept( CBREXChiSoc* pChild );

private:
    void    StartListen( void );
};

#endif
