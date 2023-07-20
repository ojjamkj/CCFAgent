// MTChiSoc.h: interface for the CMTChildSoc class.
//
//////////////////////////////////////////////////////////////////////
// note: link socket library

#ifndef     _INC_MTCHISOC
#define     _INC_MTCHISOC

#include    "MTSysMsg.h"
#include    "MTSocInc.h"

#ifndef  bool
	#define  bool   unsigned char
	#define  true   1
	#define  false  0
#endif

class CMTChildSoc
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
    int		    m_sockAddrLen;
//    unsigned long   m_bConnected;

    unsigned long   m_bEnableTimeout;

    fd_set          m_readFDs;
    struct timeval  m_recvTimeOut;
    unsigned long   m_bCheckRecvTimeout;

    fd_set          m_writeFDs;
    struct timeval  m_sendTimeOut;
    unsigned long   m_bCheckSendTimeout;

private:
	long			m_lRecvTimeOutInterval;
	long			m_lSendTimeOutInterval;

public:
    CMTChildSoc( int iSockFD,
                  sockaddr_in sockAddrIn,
                  unsigned long ulBufferSize = 4096,
                  long lClRecvTimeout = 0,
                  long lClSendTimeout = 0 );
    virtual ~CMTChildSoc();

private:
	bool CheckTimeOut( long TimeoutInterval );

public:

#if defined(_AIX) || defined(_OS390) || defined(__GNUC__)
    void    Close( void );
    void    Read( unsigned char* pBuffer, unsigned long ulSize );
    void    Write( unsigned char* pBuffer, unsigned long ulSize );
    void    Delete( void );
#else
    virtual void    Close( void );
    virtual void    Read( unsigned char* pBuffer, unsigned long ulSize );
    virtual void    Write( unsigned char* pBuffer, unsigned long ulSize );
    virtual void    Delete( void );
#endif

    inline void     EnableTimeout( void );
    inline void     DisableTimeout( void );
	void			SetTimeOutVar(long lClRecvTimeout, long lClSendTimeout);
public:
    inline const sockaddr_in&   GetSockAddr( void ) const;
    void                        GetRemoteAddr( char lpszAddr[16] ) const;

public:
    static CMTChildSoc*    Create( int iSockFD,
                                    sockaddr_in sockAddrIn,
                                    unsigned long ulBufferSize = 4096,
                                    long lClRecvTimeout = 0,
                                    long lClSendTimeout = 0
                                   );
};


inline const sockaddr_in&   CMTChildSoc::GetSockAddr( void ) const
{
    return m_sockAddrIn;
}


inline void     CMTChildSoc::EnableTimeout( void )
{
    m_bEnableTimeout = 1;
}


inline void     CMTChildSoc::DisableTimeout( void )
{
    m_bEnableTimeout = 0;
}


#endif

