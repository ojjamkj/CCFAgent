#ifndef		_INC_BRMLOAD
#define		_INC_BRMLOAD

#include    "MRSTRUCT.h"
#include    "MRSEARCH.h"
#include	"BRMFile.h"
#include	"MItem.h"
class   CMRLoadData
{
private:
	BRFRead		*pReader;
	char		 tempStr[10001];


	char         m_funcLeft;

	int          readRuleCnt;

//	int			  m_sorted;
//	unsigned char m_cmp[50];

    long         m_rc;
	char		 m_errMessage[256];

public:
    // constructors and destructor
    CMRLoadData();
    virtual ~CMRLoadData();
	 static void		DecodeString(char * enc , char * dec);
	int			LoadRule( RULE_LIST *pNodeNew,	 char *errMessage );
 
private:

	int			LoadRuleHeader( RULE_LIST*, char * );
	RULE_AGGR * LoadRuleAggr();
	int			LoadRuleReturn( RULE_LIST * );
	int			LoadRuleBody( RULE_LIST * );

	int			loadConditionalRule( RULE_LIST *pRule, RULE_VERS *pVer, double nVer );
	int			loadDynaSqlRule(RULE_LIST *pRule, RULE_VERS *pVer, double nVer);
	int			loadBranchRule(RULE_LIST *pRule);
	int			loadExternalRule(RULE_LIST *pRule);
	int			loadTableRule(RULE_LIST *pRule, RULE_VERS *pVer, double nVer, unsigned char bSorted );

	int			loadTableRuleColData( TRULE_DATA *pTR, int row );
	int			parseTRuleValue(char *str, TRULE_OPTR *v, short optr);
    int			loadFlowRule(RULE_LIST *pRule, RULE_VERS *pVer);
	int			loadFlowRuleSw(FRULE_DATA *pFR);


	VALUE_UNION * buildDefaultResult( char *pExpress, RULE_LIST *pRule, int * );

	int			getResulRecType( CRULE_STMT *pStmt, RULE_LIST *pRule );
	int			setResultValue( int retType, CRULE_STMT *p, VALUE_UNION *val );

	VALUE_UNION * buildResultRec( CRULE_STMT *pStmt, RULE_LIST *pRule, int * );
	VALUE_UNION * buildResultSingleRec( CRULE_STMT *pStmt, RULE_LIST *pRule, int * );
	RESULT_REC  * buildResultArrayRec( CRULE_STMT *pStmt, RULE_LIST *pRule );
	RESULT_REC  * buildResultArray2Rec( CRULE_STMT *pStmt, RULE_LIST *pRule );

	VALUE_UNION *buildTRuleResultRec( RULE_LIST *pRule, char *sBuf );

	int			getItemCd( char **pS, char *subStr );
	int			getParmCd( char **pS );
	int			getRuleCd( char **pS, char *retId );

	QBRULE_SUBSTR * createQBNode( unsigned char tokType, int ItemCd, char *subStr );
	void		ParseQueryRule( QRULE_DATA *pQRule, char *source );
	QBRULE_SUBSTR * ParseBranchRule( char *source );
	
//	QBRULE_SUBSTR * createQBNode( unsigned char tokType, int ItemCd, char *subStr );
//	QBRULE_SUBSTR * ParseBnQRule( char *source );
	void		DestroyDependentDS( void );
	int         FindComments(char * line);
     char*  CMRLoadData::RemoveComments(char* orgSql);
};

#endif
