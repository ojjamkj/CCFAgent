#ifndef     _INC_MRRUNNER
#define     _INC_MRRUNNER

#include  "MRDTLIST.h"
#include  "MRPARAM.h"


class   CMRRunner
{
private:
    char   m_pgmId[11];
    char   m_appDate[11];
    int    m_itemCnt;
	int    m_bDifferent;

	double			m_stTm;

	unsigned char	m_SaveFlag;

	CMRDataList		*m_pDataList;

    RULE_LIST		*m_pRuleNode;
//	RULE_VERS		*pVersion;

	RESULT_REC		*m_pResultList, *m_pSingleResult;
	RULE_RESULT		*pRuleResultPtr;

	char			m_compGroup[11];
	int				m_CmpCnt;

	unsigned char	m_bBreak;


	long			m_UsedRuleCnt;
	RULE_RESULT		*m_UsedRulesPtr[200];

//  long			m_lCode;                // exception code
    char			m_lpszMessage[512];     // exception message
    CMRParam		*m_LocalParm;

#ifdef _TRACE
	unsigned char	m_TraceFlag;
	char			m_TraceMsg[CONDITION_STR_SIZE];
#endif

	//    INDEX value used by LOOP
	int			m_IndexVal_1;
	int			m_IndexVal_2;
	int			m_IndexVal_3;

public:
    // constructors and destructor
    CMRRunner();
    CMRRunner( int ruleId, char* Parm, CMRDataList *pClass, CMRParam*, unsigned char saveFlag=1 );
    CMRRunner( char *ruleName, char* Parm, CMRDataList *pClass, CMRParam* );
    virtual ~CMRRunner();
    
	void    Delete( void );         // delete this exception object

	RESULT_REC  * getResultList();
	RULE_LIST   * getRuleInfo();
	RULE_RESULT * getRuleResultPtr();
	int           chkReturnTypeDiff( char *);

	void		  addRuleToRunList( RULE_RESULT *pRuleResult );
	RULE_RESULT * isAlreadyRun( long ruleId, char *parm );

//    long            GetECode( void ) const;
//    const char*     GetEMessage( void ) const;

    // attributes
	unsigned char	m_bRuleExist;

	int		getIndexValue( unsigned char idx );
	int		getNextIndexValue( unsigned char idx );
	void	ClearIndexValue( unsigned char idx );
	void	ResetIndexValue( unsigned char idx );
//	void	IncreaseIndexValue( unsigned char idx );

private:
//    RULE_VERS * getVersionNode( char *appDate );
	void MainProcess( char *Parm );
	RESULT_REC * ExecuteRule();

	void	EtcFunction();
	void	Sorting();
	unsigned char sortCompare( RESULT_REC*, RESULT_REC* );
	void	SumResult( short opt );
	void	MinMaxResult( short opt );
	unsigned char resultCompare( RESULT_REC*, RESULT_REC* );
	void	VerticalResultSum( RESULT_REC*, RESULT_REC* );
	void	DistinctResult(); //added by DSKIM 2016.03.09

};

#endif
