#ifndef     _INC_MRDataList
#define     _INC_MRDataList

//#include  "MTObject.h"
//#include  "MTParm.h"
#include  "MItem.h"
#include  "BRMObj.h"

#include  "MTExcept.h"
#include  "MRSTRUCT.h"
#include  "MRDTypes.h"
#include  "MRConfig.h"


//#pragma pack (1)
//#if defined(_AIX)
//	#pragma options align=packed
//#endif

typedef struct {
	 long           itemCode;
     unsigned char  itemType;
     long			itemOcc;
	 char			itemName[200+1];
     VALUE_UNION   *mValue;
} ITEM_VALUE;


typedef struct _RULE_RETURN_LIST RULE_RETURN_LIST;
struct _RULE_RETURN_LIST {
	int           rule_id;
	short         retCnt;
	char          *retType;

	RULE_RETURN_LIST   *pNext;
};

/*
typedef struct _GBL_VALUE_LIST GBL_VALUE_LIST;
struct _GBL_VALUE_LIST {
	char          gblName[31];
	TYPE_VALUE    tVal;
	GBL_VALUE_LIST   *pNext;
};
*/

typedef struct _GBL_VALUE_LIST GBL_VALUE_LIST;
struct _GBL_VALUE_LIST {
	char          gblName[51];
	char		  vType;      //  1,2,3,4,5
	int			  cnt;
	VALUE_LIST    *Val;
	GBL_VALUE_LIST   *pNext;
};


//#if defined(WIN32)
//	#pragma pack ()
//#endif

class   CMRDataList
{
  public :
  char clocaltime[20];
private:
	unsigned long	m_MaxMemSize1;
	unsigned long	m_MaxMemSize2;
	unsigned long	m_MaxMemSize3;
	void		*pMemory1;
	void		*pMemory2;
	void		*pMemory3;
	int			m_Pos1;
	int			m_Pos2;
	int			m_Pos3;

	PGM_LIST	*m_pPgmNode;

	char			m_app_dt[9];
	int				m_itemCnt;
//	int				m_resultRowCnt;

	unsigned char	*m_pBuffer;
	short			m_readPos;
  
	ITEM_VALUE		*m_itemValue;
	RULE_RESULT		*m_pRuleResult;
//	RULE_RETURN_LIST *m_pReturnList;
	GBL_VALUE_LIST *m_pGVList;

	OPERAND_STACK  *m_pErrorResult, *m_pErrorResultLast;

	unsigned char	m_bValidItems;

    unsigned long   m_ulThreadNo; 
	void			*m_ctxQuery;
	char			m_DBName[31];

	char			m_PgmId[11];
    char		    m_StartIP[24];
	long			m_RuleId;
	int				m_usedRuleClear;
	int				m_lastItemIdx;
	unsigned char	m_ParmMode;

	long			m_usedMemory;
		
//  Trace
#ifdef _TRACE
	unsigned char	m_TraceFlag;
	char			m_TraceMsg[MAX_QUERY_SIZE]; //MItem.h 30000 MAX_QUERY_SIZE
	FILE			*fp_trc;
#endif


protected:
    // data members
    long		m_lRc;
    long		m_lCode;                // exception code
    char		m_lpszMessage[128];     // exception message

public:
	char			m_TraceID[40];
	long		m_PageIndex;
	int			m_bExit;

	RULE_LIST   *pStartRuleNode;
	long		m_cursorRuleId;

//  HDMarine을 위해 추가함( Parm이 String 하나일때 )
	char		m_HAHA[20];
	RULE_RESULT *m_HAHA_rNode;

//	int			m_HD_ITEM1;
//	int			m_HD_ITEM2;


public:
    // constructors and destructor
    CMRDataList( unsigned long thrNo, void*, int size1, int size2, int size3 );
    virtual ~CMRDataList();

    // operations
#ifdef _TESTER
	void			Initialize( int ruleId );
	void			setAppDate( char *);
#else
	void			Initialize( PGM_LIST *, char * );
	void			setAppDate();
#endif

    void			DeleteStructure( void );
	void			setRuleId( long );

	void *			getVersionNode(RULE_LIST *, int *rows=0);

	void *			MRMemAlloc1( int mSize );
	void *			MRMemAlloc2( int mSize );
	void *			MRMemAlloc3( int mSize );
	void			MRMemFree( void *p );
	int				getMemPtr2();
	void			setMemPtr2(int pos);
	int				getMemPtr3();
	void			setMemPtr3(int pos);

	int				getItemColumn( int ItemCd );
	int				getItemValueCount( int col );
	int				getItemValueIndex( int col, TYPE_VALUE *tVal );
	VALUE_UNION *   getItemValuePtr( int col );
	unsigned char	getItemType( int col );
	void			getItemValue( TYPE_VALUE *tVal );
	bool			getItemValueByID( int ItemCd, TYPE_VALUE *Value );
	RESULT_REC *	getAllItemValues( TYPE_VALUE *Value, char );
	void			getItemValuesByStr( unsigned char col, long row, char *val );
	long			LogErrorItems(int idx, long rc, char *msg );
    long            DispItems( char *PgmId, long rc, char *msg );

	RULE_RESULT	   *AddResultForRule( int, char *, RESULT_REC *, RULE_LIST *, char *retTypes="" );
	int				getRuleResultCount( RULE_RESULT * ptr );
	int				getRuleValueIndex( VALUE_UNION *, TYPE_VALUE * );
    RULE_RESULT    *GetRuleResult( int ruleId, void *parm );
	void			GetRuleValue( TYPE_VALUE *Value );
	void			GetRuleValueAll( TYPE_VALUE *Value, char *val );
	void			DelRuleResult( RULE_RESULT *p );
	
    long            GetECode( void ) const;
    const char*     GetEMessage( void ) const;


#ifdef _TESTER
	int				setItemValues();

#endif

#ifdef _RUNNER
    int				BuildPgmParms( CBRMObj *ObjBuffer );
	int				BuildPgmParmsNoInfo( unsigned char parmMode, CBRMObj *ObjBuffer );
	int				BuildCBLPgmParms( unsigned char, unsigned char *pBuffer );
	long			BuildPgmReturn( CBRMObj *ObjBuffer, RULE_RESULT *pPgmResult );
#endif

	RESULT_REC*		getRuleResult(long ruleId);
	short			getParmCount( long );

#ifndef	_PARSER
	RULE_LIST*		getRuleNodeById( long ruleId );
	RULE_LIST*		getRuleNodeByName( char *ruleName );
#endif

	int				isItemValueExist( int col, char * str );

	void			getRuleName( long, char * );
	void			getItemName( long, char * );

	long			setGblVal( char *Name, TYPE_VALUE *tVal, int idx );
	long			getGblVal( TYPE_VALUE *tVal, int mode, int idx );
	long			getGblValByName( TYPE_VALUE *tVal, char *Name, int mode, int idx );
	long			addGblVal( char *Name, TYPE_VALUE *tVal );
	long			incGblVal( char *name );
	long			pushGblVal( char *Name, TYPE_VALUE *tVal );
	long			getGblRows( char *name );
	long			clearGbl( char *name );
	bool			findGblVal( char *Name, TYPE_VALUE *tVal );

	RESULT_REC*     ExecuteDynamicSql( RULE_LIST *pRuleNode, QRULE_DATA *pQRule, char *sVal[] );
	void			BuildQueryString( QRULE_DATA *pQRule, char *Val[], char *NewQuery, char mode );

	void			DelRuleResultAll( long id );
	int				usedRuleCleared();

	RESULT_REC*		createRecord( RULE_LIST *pRuleNode, char *resString );

	void			setItemValid( bool );
	void			setStartIp( char *ip);
	//   Trace function
#ifdef	_TRACE
	bool			InitTrace();
	void			TermTrace();
	unsigned char	getTraceFlag();

	void			LogItemValues();
#endif
	void			TraceOut( char *msg );
	void			TraceOutIP( char *msg );


#ifdef _TESTER
	void			DbgRuleReturn( RULE_LIST *pRuleNode, RESULT_REC *pResultRec, char *sParm );
	void			DbgQueryReturn( char *Qry );
	void			DbgSetGblVar( GBL_VALUE_LIST *p );
	void			DbgGetGblVar( char *Name );
	void			DbgClearFile();
#else
	void			LogRuleReturn( RULE_LIST *pRuleNode, RESULT_REC *pResultRec );
#endif

private:
	void			DeleteResRec( RESULT_REC  *p );

//	long			getNumber( short n );
//	double			getFraction( short n1, short n2 );
//	void			getString( char *str, short n );

#ifdef _RUNNER
	void			ParseItemStream( PGM_PARM *pParm, char *pBuffer );
	void			ParseItemStreamOcc( VALUE_UNION *mValue, PGM_PARM *pParm, char *pBuffer, int Rows );
	void			ParseChildItemStream( PGM_PARM *pParm, char *pBuffer, int itemOcc );
#endif

	void			setInitValue( TYPE_VALUE *Value, char retType );

	void			Val2String( VALUE_UNION *Value, char retType, char *s, char op );
	void			convertValueToReturnType( VALUE_UNION *val, char retType, int col, int row );

	GBL_VALUE_LIST *addGblValNode( char *Name );

	long			CheckThreadCtx( char *msg );

#if defined(_TESTER) || defined(_OS390)
	RESULT_REC*		ExecuteDynamicSqlProc( RULE_LIST *pRuleNode, char *DBName, char *QS );
	RESULT_REC*		ExecuteDynamicSqlCUDProc( char *DBName, char *QS );
	void			DbgQueryRun( char *dbn, char *Qry );
#endif

	//   Write to DB
	long			DispItemsProc( char *PgmId, long cd, char *msg );

	long			SaveTraceInfo( char *dt, char *pgmId );

};

#endif
