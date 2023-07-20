#ifndef		_INC_MRLODDB2
#define		_INC_MRLODDB2

#ifdef _POSTGRE
#include <libpq-fe.h>
#endif

#include    "MRSTRUCT.h"
#include    "MRSEARCH.h"
//#include    "MTCtSect.h"

class   CMRLoadData
{
private:
	char          m_funcLeft;
	int           pgmCnt;
	int           readRuleCnt;
	int			  readRuleBodyCnt;
	int			  readBranchCnt;

//	unsigned char m_sorted;
//	unsigned char m_cmp[50];

    long          m_rc;

//protected:
//    CMTCriticalSection      m_sect;

public:
	char		  m_errMessage[256];
    // constructors and destructor
    CMRLoadData();
    virtual ~CMRLoadData();
    static void		DecodeString(char * enc , char * dec); // 2011-01-21 vapor 데이터베이스 접속정보 암호화 디코딩함수.
	void		BuildConfiguration( void );
	void		BuildDataStructure();

	int			ReloadPgmById( char *, char * );
	int			removePgmId(char *);
	int			ReloadRuleHeader( RULE_LIST*, char * );
	int			ReloadRuleBody( RULE_LIST * );
	void		setStartIp2( char *ip);
	int			SaveStatisticsPgm( struct tm *newtime, char *errMessage );
//	int			SaveStatisticsMod( struct tm *newtime, char *errMessage );
	int			SaveStatisticsRule( struct tm *newtime, RULE_LIST *pRulePtr, char *errMessage );

private:
	void		ReadEngineConfig( void );

	void		BuildPgmStruct( void ); 
	int			BuildStreamParms( PGM_LIST * );
	int			BuildObjParms( PGM_LIST * );
	void		BuildRuleStruct( void );
	void		BuildNameList( void );
	void		BuildDataBaseStruct( void );

	int			LoadRuleBody( RULE_LIST * );
	int			LoadRuleHeader( RULE_LIST*, char * );
	RULE_AGGR * LoadRuleAggr();
	int			LoadRuleReturn( RULE_LIST * );

	int			loadConditionalRule( RULE_LIST *pRule, RULE_VERS *pVer );
	int			loadDynaSqlRule(RULE_LIST *pRule, RULE_VERS *pVer);
	int			loadBranchRule(RULE_LIST *pRule);
	int			loadExternalRule(RULE_LIST *pRule);
	int			loadTableRule(RULE_LIST *pRule, RULE_VERS *pVer, unsigned char bSorted );
	int			loadFlowRule(RULE_LIST *pRule, RULE_VERS *pVer );

	int			loadTableRuleResData( RULE_LIST *pRule, TRULE_DATA *pTR );
	int			loadTableRuleColData( TRULE_DATA *pTR );
	int			parseTRuleValue(char *str, TRULE_OPTR *v, short optr);

	int			loadFlowRuleSw( FRULE_DATA *pFR );


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


	// 2010-2-10 vapor 브랜치와 쿼리룰 같이 처리. 
	//QBRULE_SUBSTR * ParseBnQRule( char *source );
	void		ParseQueryRule( QRULE_DATA *pQRule, char *source );
	QBRULE_SUBSTR * ParseBranchRule( char *source );
	
	void		DestroyDependentDS( void );

  

	// 2010-2-10 vapor 주석문 처리함수
	int FindComments(char * line);
	char* RemoveComments(char* orgSql);
};

#endif
