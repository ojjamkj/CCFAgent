#ifndef     _INC_MRTRINTR
#define     _INC_MRTRINTR

#include   "MRDTLIST.h"
#include   "MRSTRUCT.h"
#include   "MRPARAM.h"

#if defined(_AIX)
	#define	bool unsigned char
	#define	true 1
	#define false 0
#endif

// general exception class of EILibrary
class   CMRTInterpreter
{
private:
	CMRRunner		*m_pParent;
	CMRDataList     *m_pDataList;
	RULE_LIST       *m_pRuleNode;
	CMRParam 	 	*m_pParm;

	TRULE_DATA		*pTRule;
	TRULE_COL		*pTRuleCol;

	int				m_rows;

	TYPE_VALUE		*Value;        //  Value[10];

	int				 m_ResRecSize;

//	unsigned char	m_cType[11];
//	VALUE_UNION		sValue[11];

#ifdef _TRACE
	unsigned char	m_TraceFlag;
	char			m_TraceMsg[CONDITION_STR_SIZE];
#endif

//#ifdef _DEBUG
//	char MtsDebugString[1000];
//#endif


public:
    // constructors and destructor
    CMRTInterpreter( CMRRunner *, CMRDataList *pDataList, RULE_LIST *pRuleNode, CMRParam* );
    virtual ~CMRTInterpreter();

	RESULT_REC * Running();

private:
	void	BuildCompareValues();
	int		getIndexValue( int parmId );
	void	getItemValue( TRULE_COL *pTCol, TYPE_VALUE *pTVal );
	void	getParmValue( TRULE_COL *pTCol, TYPE_VALUE *pTVal );
	void	getRuleValue( TRULE_COL *pTCol, TYPE_VALUE *pTVal );
	void	getVarValue( TRULE_COL *pTCol, TYPE_VALUE *pTVal );

	int		getMatchingIdxFromSorted();
	int		getMatchingIdxFromSequencial(int st);
	int		CompareValue(TYPE_VALUE *, VALUE_UNION *);

	int		CalcOperator(TRULE_OPTR *p, int idx); 
	int		CalcOperatorRule(TRULE_COL *pTRuleCol,TRULE_OPTR *p, int idx); 
	void	ConvertData( unsigned char dt, VALUE_UNION *v, char *sVal );
	bool	Calc_Match( char *s1, char *s2 );
	bool	Calc_Not_Match( char *s1, char *s2 );
	int		getRuleCd( char *subStr);
	RESULT_REC * SimpleResult();

};

#endif
