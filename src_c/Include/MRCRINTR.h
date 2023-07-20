#ifndef     _INC_MRCRINTR
#define     _INC_MRCRINTR

#include   "MRDTLIST.h"
#include   "MRSTRUCT.h"
#include   "MRPARAM.h"

#if defined(_AIX)
	#define	bool unsigned char
	#define	true 1
	#define false 0
#endif


class   CMRCInterpreter
{
private:
	CMRRunner		*m_pParent;
	CMRDataList     *m_pDataList;
	RULE_LIST       *m_pRuleNode;
	CRULE_DATA		*m_pCRuleData;

	CMRParam 	 	*m_pParm;

	char *			 m_pDataTypes;

	short            m_retCnt;
	short            m_vIndex;

	RESULT_REC		*pResultList;

#ifdef _TRACE
	unsigned char	m_TraceFlag;
	char			m_TraceMsg[CONDITION_STR_SIZE];
#endif

public:
	unsigned char	 m_bBreak;

//protected:
//    long    m_lCode;                // exception code
//    char*   m_lpszMessage;          // exception message

public:
    // constructors and destructor
    CMRCInterpreter( CMRRunner *, CMRDataList *pDataList, RULE_LIST *pRuleNode, CMRParam* );
    virtual ~CMRCInterpreter();

	RESULT_REC * Running();
	RESULT_REC * OneResult( CRULE_STMT *pStmt );

//	void		BuildParmResult( int *cnt, CRULE_STMT *, VALUE_UNION *pParmVal, char* );


    // attributes
    long            GetECode( void ) const;
    const char*     GetEMessage( void ) const;

    // operations
    void    Delete( void );         // delete this exception object

private:
	int		CalculateCondition( CRULE_STMT * );
	void	BuildResult( CRULE_STMT * );

	int		AddRuleValue( TYPE_VALUE *p, VALUE_UNION *pValues );

	void	getOtherRuleResult( TYPE_VALUE * );
	void	ConstantPlusOtherRuleResult( TYPE_VALUE *expResult, int Cnt1 );

	int		convertValueToReturnType( TYPE_VALUE *pTval );
	int		moveValueToResult( TYPE_VALUE *,VALUE_UNION *pValues );

	void	setInitValue( VALUE_UNION *Value, char retType );

	void	getSubStrAll( CRULE_STMT *p );
};

#endif
