#include    <stdio.h>                                                
#include    <stdlib.h>
#include    <ctype.h>
#include    <string.h>

#include    "MItem.h"
#include    "MRCSComm.h"
#include    "MTUtil.h"

#include    "BRMLoad.h"

#include    "MRTOKEN.h"
#include    "MRSTRUCT.h"
#include    "MRPARSE.h"
#include    "MRDEFINE.h" 
#include    "MRDTypes.h"

#include    "MRDTLIST.h"
#include    "MRGBLVAR.h"
#include <direct.h>
#ifdef	_TESTER
	#include	"BRMStart.h"
#endif

/*   Data File Layout
**********************************
int		ruleId
int		ruleType
int		break

String	Default
int		Option
String	Group
String	Sort

int		return 갯수
	int		 타입 ( OCC 갯수 )
int		return 갯수
	int		 타입 ( OCC 갯수 )

int		버전 갯수
	Double   버전
	String	 적용시작일
	String	 적용종료일
***********************************
***********************************
** C-Rule
int		Line수
	String	반환 Exp
	String	조건 Exp

** Q-Rule
int		sType
String  DB Name
String  최대 조회Record수
String  Query문  

** B-Rule
String  Branch Name
String  NRF

** T-Rule
int     Column수
	int     id
	int     gbn
	int     pType
	int     idx
	int     rSeq
	int     range

int		    Data Line수
	String  반환 Expression

Occ - Line수	
  Occ - Column수 
    String  Min값
    String  Max값

** F-Rule
int			Node갯수
	int		Node타입
	String  Expression
	int  연결노드수
		int     연결 Node번호
		String  판단값

***********************************
*/


CMRLoadData *pCMRLoadData;

extern  RULE_LIST     *pRuleList;

long		rId_RuleName;
long		rId_DAMBOLIST;
long		rId_DAMBOGM;
long		rConf_DAMBO_POS;
long		rConf_DAMBO_LENG;


//extern unsigned char    bIndexUsed;
extern char         UsedIndex[5];

#ifdef _DEBUG
	extern void printLog( char *buf );
#endif

/**********************************************************************/

CMRLoadData::CMRLoadData()
{
	readRuleCnt = 0;

	pCMRGblVars->CreateRuleList( 0 );
}

CMRLoadData::~CMRLoadData()
{
//      freeRuleNode( pRuleList );
}
   

int CMRLoadData::LoadRule( RULE_LIST *pNodeNew,	 char *errMessage )
{
	int		rc;
	char pBuf[200];
	int  len;
	pReader = new BRFRead( _FILE_DATA );

	rc = LoadRuleHeader( pNodeNew, errMessage );

//printf("load header : id=%d  rc=%d\n", pNodeNew->rule_id, rc );

	if ( rc == 0 && pNodeNew->rule_id > 0 ) {
		rc = LoadRuleBody( pNodeNew );
		if ( rc == 0 ) readRuleCnt++;
		else strcpy( errMessage, m_errMessage );
	}

	delete	pReader;

    return rc;
}



int CMRLoadData::LoadRuleHeader( RULE_LIST *pHead,
								 char *errMessage )
{
	int rc;
	char brName[300];
	char dftStr[500];

	pHead->loaded = 'Y';
	pHead->rule_id = pReader->getNumber();					//  ruleId
	//modified by CMLEE 2016.01.13
	//CR8095 [브랜치룰] 브랜치 명명규칙에 외부입력 변수를 포함하는 경우 Not Found 명명규칙 적용 안됨
	//if ( pHead->rule_id <= 0 ) {
	if ( pHead->rule_id <= 1 ) {
		pHead->rule_id = 0;
		return 0;
	}

	pHead->r_type = (short) pReader->getNumber();			//  rule type
//	pHead->s_type = (short) pReader->getNumber();			//  Query룰 세부타입
	pHead->s_type = 0;

	pHead->scope = 0;										//  static 여부
	pHead->preLoad = 0;										//  preLoad;
	pHead->breakFlag = (short) pReader->getNumber();		//  break;

//	pHead->branchPtr = 0;
	pHead->pVers = 0;

	pHead->parmType = (char *) 0;
	pHead->dftResult = 0;

	pHead->retCnt = 0;
	pHead->retType = (char *) 0;

	pHead->pAggr = 0;

	pHead->parmType = (char *) 0;
	pHead->dftResult = 0;


	strcpy( dftStr, pReader->getString() );		//  Default

	//  Aggr Option
	pHead->pAggr = LoadRuleAggr();

#ifdef _DEBUG
	printf("LoadRuleHeader - 1\n");
#endif

	//  Default
	strcpy( brName, pReader->getString() );		//  Branch Name 있을때
	if ( brName[0] ) {
		try {
//			pCMRGblVars->AddRuleNode( pHead->rule_id, brName, pHead );
			pCMRGblVars->AddRuleNodeSearch( pHead->rule_id, brName, pHead );
		} catch( CMTException *e )
		{
			strcpy( errMessage, (char *) e->GetEMessage() );
			e->Delete();
			return -1;
		}		
	}

#ifdef _DEBUG
	printf("LoadRuleHeader - 2\n");
#endif
				
	if ( (rc=LoadRuleReturn( pHead )) != 0 ) {
		strcpy( errMessage, m_errMessage );
		return rc;
	}


	if ( dftStr[0] ) {
		int gbn;
		pHead->dftResult = buildDefaultResult( dftStr, pHead, & gbn );
		//  0:Rule,Item포함  1:상수Only  2:Array  -1:Error
		if ( gbn != 1 ) {
			sprintf( errMessage, "  NRF expression parsing error (%d).",
				     pHead->rule_id );
			return -1;
		}
	}

#ifdef _DEBUG
	printf("LoadRuleHeader - 3\n");
#endif

	return 0;
}



RULE_AGGR * CMRLoadData::LoadRuleAggr()
{
	RULE_AGGR *pAggr;

	pAggr = (RULE_AGGR *) pCMRGblVars->MetisAlloc( sizeof(RULE_AGGR) );

	pAggr->Option = (short) pReader->getNumber();		//	Option
	strcpy( pAggr->group, pReader->getString() );		//	Group
	strcpy( tempStr, pReader->getString() );			//	Sort


	int n = 0, idx;
	char *p = tempStr;
	
	while ( *p ) {
		while ( *p ) { if (isgraph( *p ) ) break;   p++; }

		idx = 0;
		while ( *p >= '0' && *p <= '9' ) {
			idx = idx * 10 + (*p - '0');
			p ++;
		}
		
		pAggr->nSort[n] = idx;
		pAggr->cOrder[n] = 'A';
		if ( *p == 'D' || *p == 'A' ) pAggr->cOrder[n] = *p++;
		
		while ( *p ) { if ( isgraph(*p) ) break;   p++; }
		
		if ( *p == ',' ) p++;

		n++;		
	}

	pAggr->nSort[n] = 0;
    
	return pAggr;
}



int CMRLoadData::LoadRuleReturn( RULE_LIST *pRulePtr )
{
    int     i;

	pRulePtr->retCnt = (short) pReader->getNumber();		//	반환 갯수

	pRulePtr->retType = (char*) pCMRGblVars->MetisAlloc ( pRulePtr->retCnt + 1 );

	for ( i=0; i<pRulePtr->retCnt; i++ ) {
		pRulePtr->retType[i] = (char) pReader->getNumber();
	}


	/*  Load Parameter  */
	int cnt = (short) pReader->getNumber();				//	Parm 갯수

	if ( cnt == 0 ) {
		pRulePtr->parmType = 0;
	} else {
		pRulePtr->parmType = (char*) pCMRGblVars->MetisAlloc ( cnt + 1 );
		for ( i=0; i<cnt; i++ ) {
			pRulePtr->parmType[i] = (char) pReader->getNumber();
		}
		pRulePtr->parmType[i] = 0;
	}

    return 0;
}


int CMRLoadData::LoadRuleBody( RULE_LIST *pRule )
{
    RULE_VERS	   *pVer;
	double			nVer;
    int				rc=0;

	short verCnt = (short) pReader->getNumber();		//   Version갯수 ( Branch일때는 항상 0 )

#ifdef _DEBUG
printf("LoadRuleBody - 1 : %d\n", verCnt);
#endif

	if ( pRule->r_type == 3 ) {  // BRANCH Rule일때는 Version이 없다

		pRule->pVers = (RULE_VERS *) pCMRGblVars->MetisAlloc( sizeof(RULE_VERS) );
		pVer = pRule->pVers;
		pVer->stDate[0] = 0;
		pVer->endDate[0] = 0;
		pVer->rule_data = 0;
		pVer->rows = 0;
		pVer->pNext = 0;
		pVer->pPrev = 0;
		rc = loadBranchRule( pRule );

	} else {

		if ( verCnt == 0 ) {
			sprintf( m_errMessage,
					 "Version not defined for rule [%d]", pRule->rule_id );			
			return -1;
		}

		pVer = 0;
	
		for ( int i=0; i<verCnt; i++ ) {     
			if ( ! pVer ) {
				pVer = (RULE_VERS *) pCMRGblVars->MetisAlloc( sizeof(RULE_VERS) );
				pRule->pVers = pVer;
				pVer->pPrev = 0;
			} else {
				pVer->pNext = (RULE_VERS *) pCMRGblVars->MetisAlloc( sizeof(RULE_VERS) );
				pVer->pNext->pPrev = pVer; 
				pVer = pVer->pNext;
			}

			nVer = (short) pReader->getDouble();            //  버전
			strcpy( pVer->stDate, pReader->getString() );	//  적용시작일
			strcpy( pVer->endDate, pReader->getString() );	//  적용종료일

			pVer->rule_data = 0;
			pVer->rows = 0;
			pVer->pNext = 0;
		}

#ifdef _DEBUG
		printf("LoadRuleBody - 2 : %d\n", pRule->r_type );
#endif

		pVer = pRule->pVers;
		while ( pVer ) {
			switch (pRule->r_type) {
				case 1 : /*   CONDITIONAL RULE  */
					rc = loadConditionalRule( pRule, pVer, nVer );
					break;
				case 2 : /*   DYNAMIC SQL RULE  */
					rc = loadDynaSqlRule( pRule, pVer, nVer );
					break;
				case 4 : /*   External RULE  */
					rc = loadExternalRule( pRule );
					break;
				case 5 : /*   Table RULE  */
					rc = loadTableRule( pRule, pVer, nVer, (unsigned char) 0 );
					break;
				case 6 : /*   Flow RULE  */
					rc = loadFlowRule( pRule, pVer  );
					break;
				default : /* Garbage  */
					rc = -1;
					break;
			}
			
			if ( rc != 0 ) break;

			pVer = pVer->pNext;
		}
	}

#ifdef _DEBUG
printf("LoadRuleBody - 3\n");
#endif

	if ( rc != 0 ) {
		sprintf(m_errMessage, "NF-1: Rule (%d) not found." , pRule->rule_id );
    }
   
    return rc;
}


int CMRLoadData::loadConditionalRule( RULE_LIST *pRule, RULE_VERS *pVer, double nVer )
{
    CRULE_DATA     *pCRule;
    int				nLine;

    CMRParser  Parsing;

	nLine = pReader->getNumber();

//printf("loadConditionalRule - 1 : %d\n", nLine);

	for ( int i=0; i<nLine; i++ ) {
        if ( ! pVer->rule_data ) {
            pCRule = (CRULE_DATA *)
				pCMRGblVars->MetisAlloc( sizeof(CRULE_DATA) );
            pVer->rule_data = pCRule;
        } else {
            pCRule->pNext = (CRULE_DATA *)
				pCMRGblVars->MetisAlloc( sizeof(CRULE_DATA));
            pCRule = pCRule->pNext;
        }

        pCRule->resGbn = 0;
		pCRule->cond = 0;
        pCRule->pNext = 0;
        

//****************************************************************************************
        strcpy( tempStr, pReader->getString() );	//  반환 Expression

#ifdef _DEBUG
printf("loadConditionalRule - return : %s\n", tempStr);
#endif
printf("CMRLoadData::loadConditionalRule::Parsing.ParseResult \n");

        if ( (pCRule->result = Parsing.ParseResult( tempStr )) == (CRULE_STMT *) -1) {
			sprintf( m_errMessage, "Parsing error.");
			pCRule->result = 0;
            return -1;
        }

//printf("loadConditionalRule - aft parsing\n");

		if ( pCRule->result ) {
		
			int mode;
			VALUE_UNION *pResValue;
			pResValue = buildResultRec((CRULE_STMT*)pCRule->result, pRule, &mode);

//printf("loadConditionalRule - buildResultRec : %d\n", mode);

			if ( mode < 0 ) {
				sprintf( m_errMessage,
					 "Result expression syntax error.  rule=%d  line=%d", pRule->rule_id, nLine );			
				return -1;
			}

			//  2차원Array 아직 처리못함
			if ( mode > 2 ) {
				sprintf( m_errMessage,
					 "Result expression syntax error.  rule=%d  line=%d", pRule->rule_id, nLine );			
				return -1;
			}
			
						
			if ( mode > 0 ) {
				CRULE_STMT *p, *q;
				int len;

				p = (CRULE_STMT *) pCRule->result;
				while ( p ) {
					q = p->pNext;
					if ( (q->tokType == TOKENTYPE_CONST_C ||
						  q->tokType == TOKENTYPE_CONST_L )
						  && q->uni.tokInfo ) {
						len = (int) strlen((char*) q->uni.tokInfo);

						pCMRGblVars->MetisFree( q->uni.tokInfo, len );
					}

					q = p->pExpression;
					pCMRGblVars->MetisFree( p, sizeof(CRULE_STMT) );
					p = q;
				}

				pCRule->result = (void *) pResValue;
				pCRule->resGbn = mode;
			}
		}

#ifdef _DEBUG
printf("  Gbn = %d   result = %d\n", pCRule->resGbn, pCRule->result );
#endif

//***********************************************************************************
  
		strcpy( tempStr, pReader->getString() );	//  조건 Expression

#ifdef _DEBUG
printf("loadConditionalRule - cond : %s\n", tempStr);
#endif
printf("CMRLoadData::loadConditionalRule:Call:CalcExpressionNoIntr: \n");
        if ( (pCRule->cond = Parsing.ParseExpression( tempStr )) == (CRULE_STMT *) -1 ) {
			pCRule->cond = 0;
            return -1;
        }
    }   

	return 0;
}


int CMRLoadData::loadDynaSqlRule(RULE_LIST *pRule, RULE_VERS *pVer, double nVer )
{
    QRULE_DATA		*pQRule;
                   
    pQRule = (QRULE_DATA *)	pCMRGblVars->MetisAlloc( sizeof(QRULE_DATA));
	pQRule->QueryStatement = 0;
    pQRule->pBindValue = 0;
    pRule->s_type = pReader->getNumber();			//  subType

	strcpy( pQRule->DBName, pReader->getString() );	//  DB Name
    pQRule->MaxRows = pReader->getNumber();			//  최대 조회Record수
	strcpy( tempStr, pReader->getString() );		//  Query문  
	
	ParseQueryRule( pQRule, tempStr );
//    pQRule->QuerySubString = ParseBnQRule( tempStr );

    pVer->rule_data = pQRule;
    
    return 0;    
}


int CMRLoadData::loadBranchRule( RULE_LIST *pRule )
{
    BRULE_DATA     *pBRule;
    pBRule = (BRULE_DATA *)	pCMRGblVars->MetisAlloc( sizeof(BRULE_DATA) );
	
	strcpy( tempStr, pReader->getString() );	//  Branch Name
	
printf("[SO] BR Exp : [%s]\n", tempStr );

    pBRule->BranchSubString = ParseBranchRule( tempStr );

//printf("[SO] BR Parsing\n");
    
	pBRule->resGbn = 0;
    pBRule->result = 0;

	strcpy( tempStr, pReader->getString() );	//  NRF

printf("[SO] DFLT [%s] %d\n", tempStr, tempStr[0] );

	if ( tempStr[0] ) {
        int gbn;

		pBRule->result = buildDefaultResult( tempStr, pRule, & gbn );
		//  0:Rule,Item포함  1:상수Only  2:Array  -1:Error		
		if ( gbn < 0 ) {
			sprintf(m_errMessage, "  Branch default result syntax error." );
			return -1;
		}
		
		//  2차원Array 아직 처리못함
		if ( gbn > 2 ) {
			sprintf(m_errMessage, "  Branch default result syntax error." );		
			return -1;
		}
					
        pBRule->resGbn = (char) gbn;
	}

    pRule->pVers->rule_data = pBRule;

    return 0;
}


int CMRLoadData::loadTableRule( RULE_LIST *pRule, RULE_VERS *pVer, double nVer, unsigned char bSorted )
{
	TRULE_DATA     *pTRule;
	int				i = 0;
	short			Cols, Lines;
	char			sID[200];
	int				rc;

    pTRule = (TRULE_DATA *)	pCMRGblVars->MetisAlloc( sizeof(TRULE_DATA) );
    
    pTRule->sorted = bSorted;
	pTRule->cond1 = 0;
	pTRule->resGbn = 0;
	pTRule->result = 0;
    pVer->rule_data = pTRule;

	Cols = (short) pReader->getNumber();		//   Column갯수 
	pTRule->iCnt = Cols;


	if ( pTRule->iCnt > 0 ) {
		pTRule->cond1 = (TRULE_COL*) pCMRGblVars->MetisAlloc( sizeof(TRULE_COL) * pTRule->iCnt );
	}
	
	for (i=0; i<Cols; i++) {
		pTRule->cond1[i].vMin = 0;
		pTRule->cond1[i].vMax = 0;

		//  Setting COLUMN INFO
		strcpy( sID, pReader->getString() );	// ID
		pTRule->cond1[i].gbn   = (short) pReader->getNumber();
		
		if ( pTRule->cond1[i].gbn == TR_COL_TP_GBLV ) {
				pTRule->cond1[i].id = 0;
				strcpy( pTRule->cond1[i].vName, sID );
		} else {
				pTRule->cond1[i].id    = atoi(sID);
				pTRule->cond1[i].vName[0] = 0;
		}


		pTRule->cond1[i].pType = (short) pReader->getNumber();
		pTRule->cond1[i].idx   = (short) pReader->getNumber();
		pTRule->cond1[i].rSeq  = (short) pReader->getNumber();
		pTRule->cond1[i].range = (short) pReader->getNumber();
	}


	Lines = (short) pReader->getNumber();		//   Line수
	pVer->rows = Lines;

#ifdef _DEBUG
	printf("[CMRLoadData::loadTableRule] Col = %d   Lines = %d\n", Cols, Lines );
#endif

	if ( Lines == 0 ) return 0;

/*  Column 정보 생성  */
	for (i=0; i<Cols; i++) {
		if ( pTRule->cond1[i].pType == 2 ) {		//  연산자형
			pTRule->cond1[i].vMin = (TRULE_OPTR **)
				pCMRGblVars->MetisAlloc(sizeof(TRULE_OPTR*) * Lines);
		} else if ( pTRule->cond1[i].pType == 3 ) {	//  조건형
			pTRule->cond1[i].vMin = (CRULE_STMT **)
				pCMRGblVars->MetisAlloc(sizeof(CRULE_STMT*) * Lines);	
		} else {					//  상수, 범위형
			pTRule->cond1[i].vMin  = (VALUE_UNION*)
				pCMRGblVars->MetisAlloc(sizeof(VALUE_UNION) * Lines);

			if ( pTRule->cond1[i].pType == 1 ) {
				pTRule->cond1[i].vMax = (VALUE_UNION*)
					pCMRGblVars->MetisAlloc(sizeof(VALUE_UNION) * Lines);
			}
		}
    }   

/*  result 생성  */
	pTRule->resGbn = (unsigned char *) pCMRGblVars->MetisAlloc(pVer->rows);

	pTRule->result = (VALUE_UNION **)  pCMRGblVars->MetisAlloc(sizeof(RESULT_REC *) * pVer->rows);

//printf("   RESULT START ============================\n" );
    for (i=0; i<Lines; i++) {

       strcpy( tempStr, pReader->getString() );			//  반환 Expression
//printf("         [%s]\n", tempStr );
        
		//  Build Result
		if ((pTRule->result[i]	= buildTRuleResultRec(pRule, tempStr))
				== (VALUE_UNION*) -1 ) {
			rc = -1;
			break;
		}


		//  2차원Array 아직 처리못함
		if ( tempStr[0] > 2 ) {
		    sprintf( m_errMessage, "Not applied Two-dimensional array" );
			rc = -1;
			break;
		}

		pTRule->resGbn[i] = tempStr[0];
	}
//printf("   RESULT END  ============================\n" );

    for (i=0; i<Lines; i++) {
		if ( (rc = loadTableRuleColData( pTRule, i )) != 0 ) break;
    }

	return rc;
}


int CMRLoadData::loadTableRuleColData( TRULE_DATA *pTR, int row )
{
    VALUE_UNION		*p;
	CRULE_STMT		**q;
	TRULE_OPTR		**t;
	TRULE_COL		*pTRCol;
	short			optr;
//printf("   COL START ============================\n" );

	for ( int i=0; i<pTR->iCnt; i++ ) {

		pTRCol = & pTR->cond1[i];

		strcpy( tempStr, pReader->getString() );	   //  MIN값

		//modified by DSKIM 2016.02.22
		//CR8083 [테이블룰] 입력형태가 readOnly인 경우 디버그 실행 시 반환행이 없음 
		//if ( pTRCol->pType <= TR_IN_TP_RANGE ) {       //  --- 상수,범위형
		if ( (pTRCol->pType <= TR_IN_TP_RANGE) || (pTRCol->pType == TR_IN_TP_RONLY) ) { //ReadOnly 추가
			p = (VALUE_UNION*) pTRCol->vMin;
			strcpy( p[row].strVal, tempStr );
//printf("         [%s]\n", tempStr );

			strcpy( tempStr, pReader->getString() );   //  MAX값

			if ( pTRCol->pType == TR_IN_TP_RANGE ) {   //  --- 범위형이면 vMax Set
				p = (VALUE_UNION*) pTRCol->vMax;
				strcpy( p[row].strVal, tempStr );
			}
		} else if ( pTRCol->pType == TR_IN_TP_OPTR ) { //  --- 연산자형일때

			optr = atoi( tempStr );

			strcpy( tempStr, pReader->getString() );   //  비교값

#ifdef _DEBUG
printf("[CMRLoadData::loadTableRule] optr %d  val [%s]\n", optr, tempStr );
#endif

			t = (TRULE_OPTR **) pTRCol->vMin;
			
			if ( optr == 0 ) {
				t[row] = 0;
			} else {		
				t[row] = (TRULE_OPTR *) pCMRGblVars->MetisAlloc(sizeof(TRULE_OPTR));
				
				t[row]->optr = optr;
			
				if ( parseTRuleValue( tempStr, t[row], optr ) != 0 ) {
					return -1;
				}
			}

		} else {									   //  --- 조건형일때 : TR_IN_TP_COND
			q = (CRULE_STMT **) pTRCol->vMin;

#ifdef _DEBUG
printf("[CMRLoadData::loadTableRule] cond  val [%s]\n", tempStr );
#endif
			if ( tempStr[0] == 0 ) {
				q[row] = 0;
			} else {
				CMRParser  Parsing;

				if ( (q[row] = Parsing.ParseExpression( tempStr )) == (CRULE_STMT *) -1 ) {
//					q[row] = 0;
					return -1;
				}
			}

			strcpy( tempStr, pReader->getString() );   //  MAX값 - 버림
		}			
    }
//printf("   COL END ============================\n" );

	return 0;
}


int  CMRLoadData::parseTRuleValue(char *str, TRULE_OPTR *v, short optr)
{
	char  s[PARAM_MAX_COUNT][STR_VALUE_SIZE+1];
	char  buf[500];
	char *p;
	short n=0, i;

/*
	if ( v->optr == 0 ) {
		v->cnt = 0;
		v->sVal = 0;
		return 0;
	}
*/

	p = str;
	while ( *p ) {
		while ( *p ) {
			if ( isgraph( *p ) ) break;
			p++;
		}

		if ( ! *p ) break;

		i = 0;
		while ( *p ) {
			if ( *p == ',' ) {
				p++;
				break;
			}

			buf[i++] = *p++;
		}
		
		if ( i > 0 ) {
			buf[i] = 0;   buf[STR_VALUE_SIZE+1] = 0;
			McRTrim( buf );
			strcpy( s[n], buf );
			n++;
		}
		
		if ( n == PARAM_MAX_COUNT ) break;
	}


#ifdef _DEBUG
	for ( i=0; i<n; i++ ) printf("parseTRuleValue [%s]\n", s[i] );
#endif

	
	if ( optr < 7 ) {
		if ( n != 1 ) return -1;
	} else if ( optr < 9 ) {
		if ( n != 2 ) return -1;
	} else {
		if ( n == 0 ) return -1;
	}
			
	v->cnt = n;
	v->sVal = (char **) pCMRGblVars->MetisAlloc(sizeof(char*) * n);
	
	for ( int i=0; i<n; i++ ) {
		v->sVal[i] = (char *) pCMRGblVars->MetisAlloc( (int) strlen(s[i]) + 1 );
		strcpy( v->sVal[i], s[i] );
	}

	return 0;
}
	 


//  현재 비사용
/*
void  CMRLoadData::setColData(char *str, unsigned char dType, VALUE_UNION *v)
{

	switch ( dType ) {
		case DATA_TYPE_STRING :
				strcpy( v->strVal,str );	break;
		case DATA_TYPE_DOUBLE :
				v->dblVal = atof( str );	break;
		case DATA_TYPE_LONG :
				v->longVal = atoi( str );	break;
		default :
				break;
	}
}
*/

 
 
int CMRLoadData::loadFlowRule( RULE_LIST *pRule, RULE_VERS *pVer  )
{
    FRULE_DATA     *pFRuleData;
    CRULE_STMT	   *pStmt;
	short			Nodes;
	short			NodeType;

    CMRParser  Parsing;

	Nodes = (short) pReader->getNumber();		           //   Node수
	if ( Nodes == 0 ) return 0;

	pVer->rows = Nodes;

	pFRuleData = (FRULE_DATA *)
		pCMRGblVars->MetisAlloc( sizeof(FRULE_DATA) * Nodes);

	pVer->rule_data = pFRuleData;

	for ( int idx=0; idx<Nodes; idx++ ) {
		NodeType = (short) pReader->getNumber();         //  Node타입
		strcpy( tempStr, pReader->getString() );       //  Expression
        pFRuleData[idx].pType = NodeType;
        pFRuleData[idx].dGbn = 0;
        pFRuleData[idx].exp = 0;
        pFRuleData[idx].sw = 0;
        
        if ( NodeType == 2 || NodeType == 5) { // IF, CASE문
			if ( (pStmt = Parsing.ParseExpression( tempStr )) == (CRULE_STMT *) -1 ) {
				return -1;
			}
			
			pFRuleData[idx].exp = pStmt;

        } else {
			if ( (pStmt = Parsing.ParseResult( tempStr )) == (CRULE_STMT *) -1) {
				return -1;
			}

			pFRuleData[idx].exp = pStmt;

			if ( NodeType == 4 && pStmt ) {  //  반환일때
				int		mode;
				VALUE_UNION *pResValue = buildResultRec(pStmt, pRule, &mode);
				
				if ( mode < 0 ) {
					sprintf( m_errMessage, "  Result expression syntax error." );
					pFRuleData[idx].exp = 0;				 		
					return -1;
				}

				//  2차원Array 아직 처리못함
				if ( mode > 1 ) {
					sprintf( m_errMessage, "  Result expression syntax error." );
					pFRuleData[idx].exp = 0;			
					return -1;
				}

				if ( mode == 1 ) {   //  상수일때만
					CRULE_STMT *p, *q;
					int len;

					p = (CRULE_STMT *) pStmt;
					while ( p ) {
						q = p->pNext;
						if ( (q->tokType == TOKENTYPE_CONST_C ||
							  q->tokType == TOKENTYPE_CONST_L )
							  && q->uni.tokInfo ) {
							len = (int) strlen((char*) q->uni.tokInfo);

							pCMRGblVars->MetisFree( q->uni.tokInfo, len );
						}

						q = p->pExpression;
						pCMRGblVars->MetisFree( p, sizeof(CRULE_STMT) );
						p = q;
					}

					pFRuleData[idx].exp = (void *) pResValue;

					pFRuleData[idx].dGbn = 1;
				}
			}
		}

		if ( loadFlowRuleSw( &pFRuleData[idx] ) != 0 ) break;
    }   

	return 0;
}


int CMRLoadData::loadFlowRuleSw( FRULE_DATA *pFR )
{
	FRULE_DATA_SW  *pLast=0;
	short			Links;
	
	Links = (short) pReader->getNumber();		//   연결노드수

	for ( int i=0; i<Links; i++ ) {
		if ( ! pLast ) {
			pFR->sw = pLast = (FRULE_DATA_SW*) pCMRGblVars->MetisAlloc( sizeof(FRULE_DATA_SW) );
		} else {
			pLast->pNext = (FRULE_DATA_SW*) pCMRGblVars->MetisAlloc( sizeof(FRULE_DATA_SW) );
			pLast = pLast->pNext;
		}

		//  Link < 0이면 0으로 변경.   룰을 종료해야 하는가...		
		pLast->Link = (short) pReader->getNumber();         //  연결 Node번호
		strcpy( pLast->caseExp, pReader->getString() );   //  판단값
		pLast->pNext = 0;
    }   

	return 0;
}
 

int CMRLoadData::loadExternalRule(RULE_LIST *pRule)
{
	return 0;
}

