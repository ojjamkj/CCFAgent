#ifndef     _INC_MREXPRESSION
#define     _INC_MREXPRESSION

#include   "MRDTLIST.h"
#include   "MRSTRUCT.h"
#include   "MRPARAM.h"

// general exception class of EILibrary
class   CMRExpression
{
private:
	CMRDataList     *m_pDataList;
	RULE_LIST       *m_pRuleNode;
	OPERAND_STACK   *opndStack;
	OPERATOR_STACK  *optrStack;
	
	CMRRunner		*m_pParent;
	CMRParam		*pCParam;

	short			 m_LeftPcnt;
	int				 m_pMemPos;

public:   //  Result
	TYPE_VALUE		 m_TValue;

protected:
    long    m_lCode;                // exception code
    char*   m_lpszMessage;          // exception message

public:
    // constructors and destructor
    CMRExpression( CMRRunner *, CMRDataList* , RULE_LIST* , CMRParam* );
    virtual ~CMRExpression();

	TYPE_VALUE *	CalcExpression( CRULE_STMT * );
	int				CalcResultExpress( CRULE_STMT *pStmt, TYPE_VALUE *pExpResult );

    // attributes
    long            GetECode( void ) const;
    const char*     GetEMessage( void ) const;

private:
	TYPE_VALUE * getRuleParmValues( CRULE_STMT *p, char*, int * );

	void    CalcUntilLeftP();
	void    Calc_Compare(int opt, CRULE_STMT *p);
	void    Calc_Match2(int opt, CRULE_STMT *p);
	void    Calc_Match( CRULE_STMT *p);
	void    Calc_Between( CRULE_STMT *p);
	void    CalcFunction( CRULE_STMT * );
	void    CalcUntilGreater( unsigned char inOpType );
	void    Calculate( unsigned char Optr );

	void    PushOperand( TYPE_VALUE *Value );
    OPERAND_STACK * PopOperand();
	void    PushOperator( unsigned char opType );
	unsigned char   PopOperator();
	void	CalRequestIndex( unsigned char );

	void	Func_Display( int idx );
	bool	Func_Extern( int parmCnt );
	void    Func_ROWS();
	void    Func_INDEXOF();

	bool	Func_SPECIAL( unsigned char tokType, int parmCnt );
	bool	Func_HD_SPC1( int cnt, char buf[][STR_VALUE_SIZE+1], char *concatStr );
	bool	Func_HD_SPC2( int cnt, char buf[][STR_VALUE_SIZE+1] );
	bool	Func_HD_SPC3( int cnt, char buf[][STR_VALUE_SIZE+1], char *concatStr );
	bool	Func_HD_SPC4( int cnt, char buf[][STR_VALUE_SIZE+1] );
	bool	Func_HD_SPC5( int cnt, char buf[][STR_VALUE_SIZE+1] );


	void	ExecRule( CRULE_STMT *p );

	void	setGblValue( CRULE_STMT *p );
	void	incGblValue( CRULE_STMT *p );
	void	addGblValue( CRULE_STMT *p );
	void	pushGblValue( CRULE_STMT *p );
	void	clrGblValue( CRULE_STMT *p );
	void	parseGblValue( CRULE_STMT *p, char *Name, TYPE_VALUE *tVal );

	void	clearTValue( TYPE_VALUE *Value, int opt );
	bool	CalMaxMin( int opt );
	void	SumArray();

	void	getRuleResultValue( CRULE_STMT *p, TYPE_VALUE *pValue );
	void	getGlobalItemValue( CRULE_STMT *p, TYPE_VALUE *pValue );
	void	ConvertExp2Value( CRULE_STMT *p, TYPE_VALUE *pValue, unsigned char dType=0 );

	int		getArrayIndex( CRULE_STMT *pIdx );

};

#endif
