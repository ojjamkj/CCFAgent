#ifndef     _INC_MRAGTMGR
#define     _INC_MRAGTMGR

#ifdef WIN32
	#include <sys/timeb.h>
#endif
#include    <time.h>


#if defined(__hpux) || defined(_AIX) || defined(__GNUC__)
	#include	<strings.h>
	#include	<dlfcn.h>
#elif defined(_OS390)
	#include	<dll.h>
#endif	


#include    "MTCtSect.h"
#include    "BRMObj.h"
#include    "BRAgtDef.h"


//extern "C" {

	typedef int(*IMPPROC_INIT)( void *, void*, char* );
	typedef int(*IMPPROC_1)( void *, void *, char*, int, char *v[], char* );
	typedef int(*IMPPROC_2)( void *, char*, char* );
//}



class   CMRAgtMgr 
{
private:

	DATABASE_LIST	*m_pDBInfo;

	#if defined(_AIX) || defined(__hpux) || defined(__GNUC__)
		void *DllInstance;
	#elif defined(_OS390)
		dllhandle *DllInstance;
	#elif defined(_OS400) || defined(WIN32)
		HINSTANCE DllInstance;
	#endif
	
	IMPPROC_1		ImpFuncCurOpen;
	IMPPROC_2		ImpFuncCurFetch;
	IMPPROC_2		ImpFuncCurClose;
	IMPPROC_1		ImpFuncCUD;
	IMPPROC_INIT	ImpFuncINIT;
	
	int			m_lCode;
	char		m_sMessage[200];

	
	int			m_DBAgtCnt;
	DB_AGENT	*m_DBAgent;

	int			m_stIdx;

	void		ConvDB2String( QRULE_DATA *pQRule );
	void		BuildQueryString( QRULE_DATA *pQRule, char *Val[], char *NewQuery );

	void		LoadAgentLib();
	void		FreeAgentLib();

	IMPPROC_1	LoadFunction1( char *funcName );
	IMPPROC_2	LoadFunction2( char *funcName );
	void		LoadFunctionInit();


public:
	CMRAgtMgr ( DATABASE_LIST * );
	~CMRAgtMgr();

	int			getError( char *msg );
    
	int			getAgent(int ruleId);
	void		freeAgent( int idx );

//	short			getDBMSType();
//	int			getAgentCount();

	void		AgentStatus( CBRMObj *pObjParm );
	void		AgentCheck();

	void			initAgent();
	int			CursorOpen( int Idx, QRULE_DATA *pQRule, char *Val[], char *msg );
	int			CursorFetch( int Idx, char *res, char *msg );
	int			CursorClose( int Idx );
	int			ExecCUD( int Idx, QRULE_DATA *pQRule, char *Val[], char *msg );
};


#endif

