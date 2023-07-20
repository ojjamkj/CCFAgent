#ifndef		_INC_MRGBLVAR
#define		_INC_MRGBLVAR

//#include    "MTParm.h"
#include    "BRMObj.h"
#include    "MRSTRUCT.h"
#include    "MRSEARCH.h"

#ifdef	_TESTER
	#include    "BRMLoad.h"
#else
	#include    "MRLODRUL.h"
#endif

typedef struct _EXT_RULE_LIST EXT_RULE_LIST;
struct _EXT_RULE_LIST {
	RULE_LIST		*pRuleList;
	EXT_RULE_LIST	*pNext;
};


#define _TRACE_ROW   20

#include "MItem.h"



typedef struct _TRACE_LIST TRACE_LIST;
struct _TRACE_LIST {
	char		pgmId[11];
	long		itemCd;
	char		value[STR_VALUE_SIZE+1];
};


extern CMRLoadData *pCMRLoadData;

class   CMRGblVars : public CMRSearch
{
private:
	EXT_RULE_LIST *m_pExtRuleRoot;

#ifdef		_TRACE
	TRACE_LIST	  m_TraceList[_TRACE_ROW];
	int			  m_TraceListCnt;
#endif

	int			  m_ruleCount;
//	int			  m_ExtRuleCount;
	int			  m_AllocSize;

    long          m_rc;
	char		  m_errMessage[256];

public:
	unsigned char	m_TraceMode;   //  0 : Off   1 : On

    // constructors and destructor
    CMRGblVars();
    virtual ~CMRGblVars();

#ifdef		_RUNNER
	int				LoadPgmById( char *, char * );
	int				LoadRuleById( long, char, char, char * );

	PGM_LIST		*getPgmNode( char *pgmId );
	PGM_LIST		*getPgmNodeRuleId(long ruleID);
	PGM_LIST		*RemoveProgram(char *pgmId);
	int				GetPgmInfo( CBRMObj *pObjParm );
	RULE_LIST		*getRuleNode( int ruleId );
	RULE_LIST		*getRuleNode( char *ruleName );

	short			getRuleScope( int ruleId );

	void			setRuleInvalid( int );
	void			RemoveRule( long );
#endif

	RULE_LIST*		SearchById( long Id );

#ifdef		_TESTER
	RULE_LIST		*DbgLoadRule( int );
	RULE_LIST		*DbgLoadRule( char * );
#endif

	void *			MetisAlloc( int sz );
	void			MetisFree( void *p );
	void			MetisFree( void *p, int sz );
	void			MetisDummyAlloc( int sz );
	void			CreateRuleList( int cnt );
	void			DisplayAllocSize( char * );


#ifdef		_RUNNER
	int				SaveStatistics( char *errMessage );
#endif

/*
	void			TraceSet( unsigned char mode );
	unsigned char	getTraceList();
	void			clearTraceIndex( int idx);
	int				getTraceIndex(int idx, char *PgmId);
	int				getTraceValue(int idx, char *sVal);
*/

private:
    void		Delete( CRULE_STMT *p );

#ifdef		_TRACE
	int			buildTraceList( TRACE_LIST pList[] );
    int			buildTraceListProc( TRACE_LIST pList[] );
#endif

	void		freeRuleNode( RULE_LIST * );
	void		freeRuleVersion( short, RULE_VERS * );
	void		freeCRuleData( CRULE_DATA * );
//	void		freeCRuleLine( CRULE_LINE * );
	void		freeCRuleStmt( CRULE_STMT * );
	void		freeQRule( QBRULE_SUBSTR * );
	void		freeTRule( TRULE_DATA * );

};

extern  CMRGblVars *pCMRGblVars;

#endif
