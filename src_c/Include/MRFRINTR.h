#ifndef     _INC_MRFRINTR
#define     _INC_MRFRINTR

#include   "MRDTLIST.h"
#include   "MRSTRUCT.h"
#include   "MRPARAM.h"

#if defined(_AIX)
	#define	bool unsigned char
	#define	true 1
	#define false 0
#endif

class   CMRFInterpreter
{
private:
	CMRRunner		*m_pParent;
	CMRDataList     *m_pDataList;
	RULE_LIST       *m_pRuleNode;
	FRULE_DATA		*m_pFRuleData;

	RESULT_REC		*pResultList;
	CMRParam 	 	*m_pParm;

	int				m_Loop[10];
	int				m_useNode[10];

#ifdef _TRACE
	unsigned char	m_TraceFlag;
	char			m_TraceMsg[CONDITION_STR_SIZE];
#endif


public:
	unsigned char	 m_bBreak;

public:
    CMRFInterpreter( CMRRunner *, CMRDataList *pDataList, RULE_LIST *pRuleNode, CMRParam* );
    virtual ~CMRFInterpreter();

	RESULT_REC * Running();

private:
	void	BuildResult( CRULE_STMT * );

	int		SearchLink_IF( int idx, CRULE_STMT *pStmt );
	int		SearchLink_CASE( int idx, CRULE_STMT *pStmt );

	int		getLoopCnt( CRULE_STMT *pStmt );

};

#endif
