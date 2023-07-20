#ifndef     _INC_MRCSTHR
#define     _INC_MRCSTHR

#include    "BRMObj.h"
//#include    "MTParm.h"

#include	"BREXThread.h"

class   CCSThread : public CBREXThreadrun, public CBREXThread
{
// permanent
protected:
    void					*m_pManager;
//    unsigned long			m_bProcessAdmin;
    char                    m_lpszDate[11];

public:
    unsigned long			m_ulThreadNo;
//	int						m_delay;
	long					m_startSecond;
	int						m_HitCount;

public:
//	CMTParm					 m_ObjBuffer;
//	CMTObject				*m_ObjHeader;
	CBRMObj					 m_ObjBuffer;
	CBRMObj					*m_ObjHeader;

    int						m_pChildSoc;
	void					*m_ctx;

	unsigned char			m_bUsing;
	unsigned char			m_bLogTime;
	long					delay0;

	unsigned char			bDisplayed;

	unsigned char			m_parmMode;
	char					m_ModuleName[21];
    char				  m_StartIP[24];

	void		 			*pCMRDataList;

	unsigned char			 m_ClientLanguage;
	unsigned long			 m_MaxPramSize;

	char					 m_TempString[11];
	char					 m_sLogBuffer[512];
	char					 m_pgmId[20];
protected:
    void    ProcessRequest();
    long    MainProcess(char *startIp);

private:
    int		CheckHeader( char *data );
	void	ReplyResultSet();
	long    ProcInterval();

public:
    CCSThread ( void* pManager, unsigned long ulThreadNo );
    ~CCSThread();

	void			Initialize();

    virtual  void	Run( void );

    void			StartAdminProcess();
    void			EndAdminProcess();
 

	unsigned long   GetThreadNo( void ) const	{ return m_ulThreadNo; }

    unsigned long   GetSocketId( void )       { return m_pChildSoc; }

    unsigned char   IsUsing( void )       { return m_bUsing; }

	long			GetRunInfo( char *PgmId );
	long			GetRunInfo2( );

	void			DisplayLog( int );

	void			DisplayLogLock();

	void			ReplyTimeOut();
	void			GetPgmId			(char * str);
	int				getChildSockdet();
};

#endif

