#ifndef     _INC_MTCSMGR
#define     _INC_MTCSMGR

#include    "MRCSQueu.h"
#include    "BRMObj.h"
#include    "MRConfig.h"

#ifndef  bool
	#define  bool   unsigned char
	#define  true   1
	#define  false  0
#endif


class	CCSThread;

class   CCSManager    : public CMRCSQueue {

public:	
	int				m_MaxPramSize;
	unsigned int	m_SvrPos;
	bool			m_AdminRunning;
	bool			ENGINE_CFG_CTL;	

	unsigned int  	m_bTerminate;
		

private:
    bool					m_InitCode;
    unsigned long           m_ulNoWThreads;
    CCSThread				**m_ppWorker;
               
	unsigned long			m_ulNoPendingRequests;


public:
	CCSManager( int dummy );
    ~CCSManager();

public:
    int						GetNextRequest( unsigned long );
	void					DisplyLogWhenSignal(int);

    virtual void            OnAccept( int );     
	
    bool    				GetInitCode();
    long                    TerminateThreads( unsigned long ulThreadNo );
    void                    PreTerminate( void );
    
    void                    StartAdminProcess();
    void                    EndAdminProcess();
    void                    CheckAdminProcess( void );

#ifndef _RUNNER
	void					DisplayDisconnectMsg( int idx );
#endif

//	void                    DisplayConfiguration( void );
	void					Diagnose( void );
//	void					GetThreadInfo( CBRMObj *pObjParm );
	void					GetEngineInfo( CBRMObj *pObjParm );
	void					GetEngineDetail( CBRMObj *pObjParm );

	void 					GetThrInfo(CBRMObj *pObjParm);
	void 					GetQueueInfo(CBRMObj *pObjParm);
	
	void    				NormalTerminate();
	void    				SelfTerminate();
	
	unsigned int	setEngineInfo( unsigned long param_engineId, unsigned short param_queueSize, unsigned long param_defaultQueueID, 
							unsigned long param_threadCount, ENGINE_INFO param_engine_info[] ) ;	
};

extern  CCSManager    *g_pManager;

#endif
