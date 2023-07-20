// MTTcpCli.h
//
//////////////////////////////////////////////////////////////////////
// note: link socket library

#ifndef     _INC_MTTCPCLI
#define     _INC_MTTCPCLI

#include    "MTSocInc.h"
//#include <io.h>
//#include <winsock2.h>


class CMTTcpClient
{
protected:
    int             m_iSockFD;
    unsigned long   m_bConnected;
    sockaddr_in     m_sockAddrIn;

    unsigned long   m_bEnableTimeout;

    fd_set          m_readFDs;
    struct timeval  m_recvTimeOut;
    unsigned long   m_bCheckRecvTimeout;

    fd_set          m_writeFDs;
    struct timeval  m_sendTimeOut;
    unsigned long   m_bCheckSendTimeout;
 
public:  
    // for client socket
    CMTTcpClient(   long lClRecvTimeout = 0, long lClSendTimeout = 0 );
    virtual ~CMTTcpClient();

public:
     void    Open( char *lpszServerAddress, unsigned short ulPort );
     void    Close( void );
     void    Write( unsigned char* pBuffer, unsigned long ulSize );
     void    Read( unsigned char* pBuffer, unsigned long ulSize );
    virtual void    Delete( void );

    inline void     EnableTimeout( void );
    inline void     DisableTimeout( void );

public:
    inline const sockaddr_in&   GetSockAddr( void ) const;
    void                        GetRemoteAddr( char lpszAddr[16] ) const;
};


inline const sockaddr_in&   CMTTcpClient::GetSockAddr( void ) const
{
    return m_sockAddrIn;
}


inline void     CMTTcpClient::EnableTimeout( void )
{
    m_bEnableTimeout = 1;
}


inline void     CMTTcpClient::DisableTimeout( void )
{
    m_bEnableTimeout = 0;
}


#endif

