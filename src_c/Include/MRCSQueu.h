#ifndef _INC_MRCSQUEUE
#define _INC_MRCSQUEUE

#include    "MTSysMsg.h"
#include    "MTSocInc.h"

typedef struct _SOC_INFO SOC_INFO;
struct _SOC_INFO {
	unsigned char  use;
	int            soc;
#ifndef _RUNNER
	sockaddr_in    sockAddr;
#endif
	struct timeval accStamp;
};

class CMRCSQueue
{
protected:
    int             m_iSockFD;
    int             m_iBackLog;
    sockaddr_in     m_sockAddrIn;
	unsigned short  m_usPort;
    fd_set          m_acceptFD;

    volatile unsigned long  m_bStop;
    struct timeval  m_timePollingAccept;
    
private:
	SOC_INFO	    *m_socInfo;

	unsigned long   m_ulClBufferSize;

    struct timeval  m_recvTimeOut;
    unsigned long   m_bCheckRecvTimeout;

    struct timeval  m_sendTimeOut;
    unsigned long   m_bCheckSendTimeout;

	long			m_MaxQueueCount;
    int             m_pFirst;          // first node
    int             m_pLast;           // last node
	int				m_QueueCount;

public:
    CMRCSQueue();
    virtual ~CMRCSQueue();

public:

#ifdef _AIX
    void    Run( void );
#else
    virtual void    Run( void );
#endif

    void    Init( int iBackLog = 5 );
    void    Close( void );

    void	CloseChildSoc( int idx );
	void	SocWrite( int idx, unsigned char* pBuffer, unsigned long ulSize );
	int		SocRead( int idx, unsigned char* pBuffer, unsigned long ulSize );
	long    AcceptTimeSec( int SocCli );
	long    AcceptTimeUSec( int SocCli );

//	int		getQueueCount();
	void	getQueueCount( int *nWait, int *nRun );
	void	getQueueInfo( char *msg );
 
	void    SocWriteString(int idx, char* lpsz);

// overrides
protected:
    virtual void    OnAccept( int chiSoc );
    
    int				popSocket( void );
    void            GetRemoteAddr( int soc, char lpszAddr[16] ) const; 
    void			StopListen( void );
    void			RemoveAllQueue( void );
	
private:    
	int				pushSocket( int SocCli, sockaddr_in clSockAddr );
	
};

#endif
