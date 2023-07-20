#ifndef     _INC_MRAdmin
#define     _INC_MRAdmin

//#include    "MTParm.h"
#include    "BRMObj.h"
#include	"BREXListen.h"
#include	"BREXThread.h"

class   CMRAdmin : public CBREXThreadrun, public CBREXThread, public CBREXListen
{
private:
	CBRMObj      *m_ObjBuffer;
//	CMTObject    *m_ObjHeader;

    CBREXChiSoc  *m_pChildSoc;

	int			  m_mode;

	long		  m_Rc;
	char		  m_Message[500];


public:
    // constructors and destructor
	CMRAdmin();
    virtual ~CMRAdmin();

    // operations
	virtual void	Run( void );
	virtual	void    OnAccept( CBREXChiSoc *pChild );
	long    PwdChange();

private:
	void    mainProcess();

	void	ReplyResultSet();

	long	NormalProcess();
	long	CTSectProcess();

	long    DeployProgram();
	long    DeployRule();
	long    DeleteRule();
	long    DeleteRuleID(long ruleID);
	long	DeleteProgramID();
	long    DeleteProgramStr();
#ifdef	_TRACE
	long    TraceStart();
	void	ReadTraceFile();
#endif

};


extern	CMRAdmin    *g_pCMRAdmin;

#endif
