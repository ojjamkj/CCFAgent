#ifndef     _INC_MRSTRUCT
#define     _INC_MRSTRUCT

#ifndef bool
	#define	bool unsigned char
#endif

#ifndef true
	#define	true 1
	#define false 0
#endif

#include  "MRDTypes.h"

/*****************************************************
****     RULE LIST    ********************************    
******************************************************/

//#pragma pack (1)
/*
#if defined(_AIX)
	#pragma options align=packed
#endif
*/
/*****************************************************
****     VALUE        ********************************    
******************************************************/

typedef union {
    long   longVal;
    double dblVal;
	unsigned char   boolVal;
	char   *pLStr;
    char   strVal[100+1]; //100

	struct {
		void             * ptr;
		long             i_row;
		unsigned char    i_col;
	} RuleItem;
} VALUE_UNION;


struct _TYPE_VALUE {
	unsigned char  dataType;
    VALUE_UNION    value;
};	
typedef struct _TYPE_VALUE TYPE_VALUE;


/****************************************************
****    RULE TYPE     *******************************
*****************************************************/
typedef struct _TOKEN_RULE TOKEN_RULE;
struct _TOKEN_RULE {
	long           ruleId;
	char          *parmInfo;
	unsigned char  resIndex;
};

typedef struct _CRULE_STMT CRULE_STMT;
struct _CRULE_STMT {
	unsigned char  tokType;

	union {
		long			longVal;
		double			dblVal;
		unsigned char   boolVal;
		void			*tokInfo;
	} uni;

	CRULE_STMT    *pNext;
	CRULE_STMT    *pExpression;
};


typedef struct  _CRULE_DATA CRULE_DATA;
struct _CRULE_DATA	{
	char		 resGbn;    //   0:Normal, 1:Constant
	void         *result;
	CRULE_STMT   *cond;
	CRULE_DATA   *pNext;
};


typedef struct _QBRULE_SUBSTR QBRULE_SUBSTR;
struct _QBRULE_SUBSTR {
	unsigned char  tokType;
	long           ItemCd;
//	char           ItemName[200+1];
	char          *tokInfo;
	int			   pos;
	QBRULE_SUBSTR *pNext;
};

typedef struct  _QRULE_DATA QRULE_DATA;
struct _QRULE_DATA	{
	char           DBName[31];
//	char           flagNextPage;
	long           MaxRows;
	char          *QueryStatement;
	short		   nBindVal;
	QBRULE_SUBSTR *pBindValue;    // QuerySubString;
};


typedef struct _BRULE_DATA BRULE_DATA;
struct _BRULE_DATA {
	char		   resGbn;    //   0:None, 1:Normal, 2:Constant
	void           *result;
	_QBRULE_SUBSTR *BranchSubString;
};

typedef struct  _TRULE_OPTR TRULE_OPTR;
struct _TRULE_OPTR	{
	short		optr;    //  연산자
	short		cnt;     //  비교값 갯수
	char		**sVal;  //  비교값
};

typedef struct  _TRULE_COL TRULE_COL;
struct _TRULE_COL	{
	short		gbn;     //  Compare Type ( 1:RULE 2:EXTV 3:INTV 4:GBLV )
	int			id;
	short		rSeq;
	short		pType;   //  0.상수  1.범위   2.연산자   3.조건식
	short		range;   //  0.모두포함   1.최소제외   2.최대제외
	char		vName[101];
	short		idx;
	void	   *vMin;    //  연산자, 조건식
	void	   *vMax;    //  범위형일때만
	char       param[201]; 
};

typedef struct  _TRULE_DATA TRULE_DATA;
struct _TRULE_DATA	{
	unsigned char sorted;
//	short         rows;
	short		  iCnt;
	TRULE_COL     *cond1;
	unsigned char *resGbn;    //   0:Normal, 1:Constant
	VALUE_UNION	  **result;   //   Expression일때는 CRULE_STMT임
};


typedef struct _FRULE_DATA_SW FRULE_DATA_SW;
struct _FRULE_DATA_SW {
	short		   Link;     //  연결Node
	char           caseExp[101];
	FRULE_DATA_SW *pNext;
};

typedef struct {
	short		   pType;    //   1:판단  2:실행  3:반환
	short		   dGbn;     //   0:Exp   1:상수
	void          *exp;
	FRULE_DATA_SW *sw;
} FRULE_DATA;


typedef struct {
	short		 Option;
	char		 group[11];

	short		 nSort[10];   //   1 ~ 9
	char         cOrder[10];  //  'A','D'
} RULE_AGGR;

/********************************************/
typedef struct _RULE_VERS RULE_VERS;
struct _RULE_VERS {
    char		  stDate[9];
    char		  endDate[9];
	void		  *rule_data;
	short         rows;
	RULE_VERS     *pNext;
	RULE_VERS	    *pPrev;
};

typedef struct _RULE_LIST RULE_LIST;
struct _RULE_LIST {
    char		  loaded;       //  Y/N/X
	char          ruleName[200+1];
	long          rule_id;
	short         r_type;
	short         s_type;		//  Query룰 세부타입
    short		  scope;		//  1:Static
	short		  preLoad;
	short         breakFlag;

	void		  *dftResult;
	RULE_AGGR	  *pAggr;

	char          *parmType;
	short         retCnt;
	char          *retType;

	int           branchIdx;
//	void          *branchPtr;
	RULE_VERS	  *pVers;

	int           hit;
	double        totTm;
	int           maxTm;
};


typedef struct _PGM_PARM PGM_PARM;
struct _PGM_PARM {
	long		 itemCode;
	char         itemType;
	char         itemName[200+1];
	short        L1;
	short        L2;
	short        MaxOcc;
	long		 ValidOcc_Icd;
	PGM_PARM	*pNext;
	PGM_PARM	*pChild;
};

typedef struct {
	 long           itemCode;
     unsigned char  itemType;
	 char		    itemName[200+1];
} ITEM_INFO;

typedef struct _PGM_LIST PGM_LIST;
struct _PGM_LIST {
	char         PgmId[11];
	long         startRuleId;
	short		 app_dt_id;
	short        maxRowCnt;
	short		 itemCnt;
	char         ParmGbn;    //   1:Object   2:Stream
	void		*pParm;      //   ITEM_INFO, PGM_PARM

	int          hit;
	double       totTm;
	double       totWait;
	int          maxTm;

	PGM_LIST    *pNext;
};	


/*****************************************************
****     RESULT           ****************************    
******************************************************/

typedef struct _VALUE_LIST VALUE_LIST;
struct _VALUE_LIST {
    VALUE_UNION  value;
	VALUE_LIST   *pNext;
};

typedef struct _RESULT_REC RESULT_REC;
struct _RESULT_REC {
    VALUE_UNION  *value;
	RESULT_REC   *pNext;
};

typedef struct _RULE_RESULT RULE_RESULT;
struct _RULE_RESULT {
	long          ruleId;
    char          parmInfo[128];
	RULE_LIST     *pRuleNode;
    RESULT_REC    *resRecord;
	RULE_RESULT   *pNext;
};


/*****************************************************
****     OPERATOR & OPERAND   ************************    
******************************************************/

typedef struct _OPERAND_STACK OPERAND_STACK;
struct _OPERAND_STACK {
	TYPE_VALUE      tVal;
//	void *          ptr;
	OPERAND_STACK * pNext;
};

typedef struct _OPERATOR_STACK OPERATOR_STACK;
struct _OPERATOR_STACK {
	unsigned char     opType;
	OPERATOR_STACK   *pNext;
};
 

/****************************************************
****    DATABASE LIST   *****************************
*****************************************************/
typedef struct _DATABASE_LIST DATABASE_LIST;
struct _DATABASE_LIST {
	short    dbms;
	char     DBName[30];
	char     DBPName[30];
	char     DBUser[20];
	char     DBPasswd[201]; 
	char     DBHOST[201];	// added for MYSQL 2019.06.12
	char     DBTYPE[21];
	unsigned long  DBPORT;
	unsigned long  ServerID;
	char     conninfo[501];
	short    agents;
	void*    pAgtMgr; 
	DATABASE_LIST *pNext; 
};

#if defined(WIN32)
	#pragma pack ()
#endif

#endif
