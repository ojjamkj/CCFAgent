#include    <string.h>
#include    <stdio.h>
#include    <stdlib.h>


#include	"MRRUNNER.h"

#include    "MRTOKEN.h"
#include    "MRPARSE.h"
#include    "MRCALC.h"
#include    "MRSFUNC.h"
#include    "MRERROR.h"
#include    "MTExcept.h"
#include    "MRARRFN.h"
#include    "MREXPRES.h"

#include    "MRLODRUL.h"

#ifdef _TESTER
	#include    "BRMStart.h"
#endif

#define   _MEMORY_MGR

#ifdef _MSG_KO
#define   MSG_NOTEXIST_RULE   "   룰( %d )이 존재하지 않습니다.\n"
#else
#define   MSG_NOTEXIST_RULE   "   Rule ( %d ) not exist.\n"
#endif

int	CMRExpression::CalcResultExpress( CRULE_STMT *pStmt, TYPE_VALUE *pExpResult )
{
 
	long  id = 0;
	if ( ! pStmt ) {
		pExpResult->dataType = TOKENTYPE_NONE;
		return 0;
	}

	if ( ! pStmt->pNext ) {  //  단일 Token일때
		if ( pStmt->tokType < TOKENTYPE_CONST_V ) {
			ConvertExp2Value( pStmt, pExpResult );
			return 0;
		}

		switch ( pStmt->tokType ) {
			case TOKENTYPE_CONTINUE :
				return 1;
			case TOKENTYPE_BREAK : 
				return 2;
			case TOKENTYPE_EXIT : 
				return 4;
			case TOKENTYPE_RULE :
				getRuleResultValue( pStmt, pExpResult );
					return 0;
			case TOKENTYPE_GLOBAL :
				getGlobalItemValue( pStmt, pExpResult );
					return 0;
			case TOKENFUNC_EXEC :
				ExecRule( pStmt->pExpression );
					return 3;
			case TOKENFUNC_SETVAL :
				setGblValue( pStmt->pExpression );
					return 3;
			case TOKENFUNC_INCVAL :
				incGblValue( pStmt->pExpression );
					return 3;
			case TOKENFUNC_ADDVAL :
				addGblValue( pStmt->pExpression );
					return 3;
			case TOKENFUNC_PUSHVAL :
				pushGblValue( pStmt->pExpression );
					return 3;
			case TOKENFUNC_CLRVAL :
				clrGblValue( pStmt->pExpression );
					return 3;
			case TOKENFUNC_RCLEAR :
#ifndef _PARSER
				CRULE_STMT *p;
				if ( pStmt->pExpression ) {
					p = pStmt->pExpression->pNext;
					if ( p ) {
						if ( p->tokType != TOKENTYPE_RULE )
							throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_97 );

						TOKEN_RULE *Rule = (TOKEN_RULE*) p->uni.tokInfo;
						id = Rule->ruleId;
					}
				}

				m_pDataList->DelRuleResultAll( id );

#ifdef _TESTER
				m_pWorker->SuspendWork( _DBG_STEP_RCLEAR, id, 0, "" );
#endif

//				if ( id == 0 ) m_pParent->usedRuleCntClear();
//				else m_pParent->usedRuleClear( id );
#endif
				return 3;

			default :
				break;
		}
	}


//  단독이 아니면 Error인 기능
#ifdef _PARSER
	switch ( pStmt->tokType ) {
			case TOKENFUNC_SETVAL :
			case TOKENTYPE_CONTINUE :
			case TOKENTYPE_BREAK : 
			case TOKENTYPE_EXIT : 
			case TOKENFUNC_EXEC :
			case TOKENFUNC_INCVAL :
			case TOKENFUNC_ADDVAL :
			case TOKENFUNC_PUSHVAL :
			case TOKENFUNC_CLRVAL :
			case TOKENFUNC_RCLEAR :
				throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_98 );
				break;
			default :
				break;
	}
#endif

	TYPE_VALUE	*expResult = CalcExpression( pStmt );

	if ( pStmt->tokType == TOKENFUNC_DISPLAY ) return 3;

	if (expResult) {
		memcpy((char*)pExpResult, (char*)expResult, sizeof(TYPE_VALUE));
	}
	return 0;
}


void	CMRExpression::ConvertExp2Value( CRULE_STMT *p, TYPE_VALUE *pValue, unsigned char dType )
{
	pValue->dataType = p->tokType;
	switch ( p->tokType ) {
		case TOKENTYPE_CONST_F :
			pValue->value.dblVal = p->uni.dblVal;
			break;
		case TOKENTYPE_CONST_C :
			memset(pValue->value.strVal,0x00, STR_VALUE_SIZE );
			if ( dType == TOKENTYPE_CONST_L ) {
				pValue->value.pLStr = (char *) p->uni.tokInfo;
				pValue->dataType = TOKENTYPE_CONST_L;
			} else {
				if (p->uni.tokInfo) {
					strcpy(pValue->value.strVal, (char *)p->uni.tokInfo);
				}
				else
					pValue->value.strVal[0] = 0;
			}
			pValue->value.strVal[STR_VALUE_SIZE] = 0;
			break;
		case TOKENTYPE_CONST_L :
			if ( dType == TOKENTYPE_CONST_C ) {
				memset(pValue->value.strVal,0x00, STR_VALUE_SIZE );
				if ( p->uni.tokInfo )
					strcpy( pValue->value.strVal, (char *) p->uni.tokInfo );
				else
					pValue->value.strVal[0] = 0;

				pValue->dataType = TOKENTYPE_CONST_C;
			} else {
				pValue->value.pLStr = (char *) p->uni.tokInfo;
			}
			break;
		case TOKENTYPE_CONST_N :
			if(dType == TOKENTYPE_CONST_B)
			{
				pValue->value.boolVal = p->uni.boolVal;
				pValue->dataType = TOKENTYPE_CONST_B;
			}
			else
			{
				pValue->value.longVal = p->uni.longVal;
			}
			break;
		case TOKENTYPE_CONST_B :
//			pValue->value.boolVal = p->resIndex;
			pValue->value.boolVal = p->uni.boolVal;
			pValue->dataType = TOKENTYPE_CONST_B;
	}
}


extern CMRLoadData *pCMRLoadData;

void	CMRExpression::getGlobalItemValue( CRULE_STMT *p, TYPE_VALUE *pValue )
{
#ifndef _PARSER
	int rId, col;

	rId = p->uni.longVal;

	col = m_pDataList->getItemColumn( rId);
	if ( col < 0 ) {
		char msg[128];
//		char rName[101];

//		pCMRLoadData->getItemName( rId, rName );

		sprintf( msg,"Item (%d) not found.", rId );
		throw new CMTException( UERR_ITEMNOTFOUND, msg );
    }

	pValue->dataType = TOKENTYPE_GLOBAL;
	pValue->value.RuleItem.ptr = 0;
	pValue->value.RuleItem.i_col = col;

	pValue->value.RuleItem.i_row = getArrayIndex( p->pExpression );

#endif
}


void	CMRExpression::getRuleResultValue( CRULE_STMT *p, TYPE_VALUE *pValue )
{
	RULE_RESULT  *pRuleResult; 
	TOKEN_RULE   *Rule;

	Rule = (TOKEN_RULE*) p->uni.tokInfo;

	pValue->dataType = TOKENTYPE_RULE;
//	pValue->value.RuleItem.ptr = ???;
	pValue->value.RuleItem.i_col = Rule->resIndex;
//	pValue->value.RuleItem.i_row = ???;

#ifdef _PARSER
	RULE_LIST	 *pRuleNode;

	if (! (pRuleResult = m_pDataList->GetRuleResult( Rule->ruleId, 0 )) ) {
		CRULE_STMT   *pStmt;
		RESULT_REC   *pResultList;
		VALUE_UNION   val;
		int cnt = 0;
		char retType[1001]; // 100->1001 룰 리턴갯수 최대 100->1001로 임시 변경(2015.04.08)
		pStmt = p->pExpression->pExpression;
//	Rule = (TOKEN_RULE*) p->uni.tokInfo;
		while ( pStmt ) {

			retType[cnt] = (char) pStmt->pNext->uni.longVal;
			pStmt = pStmt->pExpression;
			cnt++;
		}
		retType[cnt] = 0;


		pResultList = (RESULT_REC *) m_pDataList->MRMemAlloc1( sizeof(RESULT_REC) );
		pResultList->value = (VALUE_UNION *) m_pDataList->MRMemAlloc1(sizeof(VALUE_UNION) * cnt);
		pResultList->pNext = 0;

		for ( int i=0; i<cnt; i++ ) {
			switch ( retType[i] ) {
				case DATA_TYPE_STRING :
						strcpy( val.strVal, " " );
						break;
				case DATA_TYPE_LSTRING :
						val.pLStr = 0;
						break;
				case DATA_TYPE_DOUBLE :
						val.dblVal = 1.0;
						break;
				case DATA_TYPE_LONG :
						val.longVal = 1;
						break;
				case DATA_TYPE_BOOL :
						val.boolVal = true;
			}

			memcpy ( (char *) & pResultList->value[i], (char *) &val, 
					sizeof(VALUE_UNION) );

		}
//printf("     ====> retType bef AddResultForRule !%d!%d!\n", retType[0], retType[1] );

		pRuleNode = (RULE_LIST *) m_pDataList->MRMemAlloc1( sizeof(RULE_LIST) );
		pRuleNode->retCnt = cnt;
		pRuleNode->retType = (char *) m_pDataList->MRMemAlloc1( cnt+1 );
		memcpy( pRuleNode->retType, retType, cnt );

		pRuleResult = m_pDataList->AddResultForRule( Rule->ruleId, "", pResultList, pRuleNode, retType );
	}

	pValue->value.RuleItem.ptr = pRuleResult;
	pValue->value.RuleItem.i_row = 0;

#else   //  _RUNNER

	TYPE_VALUE *pParmVal = 0;
	int			parmCnt;
	char		sKey[200];//2017.09.28일 전까지 200 ,산업은행 E-상시감사 10000 인경우 Down 발생


	pParmVal = getRuleParmValues( p, sKey, &parmCnt );
 
	pRuleResult = 0;

	if ( m_pParent ) {  //  현재룰에서 실행된 룰이면 결과Pointer를 얻는다.
//*****************************************************
//    20080625 잠시 막아본다.
//		pRuleResult = m_pParent->isAlreadyRun( Rule->ruleId, (char*) Rule->parmInfo );
	}

	if ( ! pRuleResult ) {  //  현재룰에서 처음 사용될때
		pRuleResult = m_pDataList->GetRuleResult( Rule->ruleId, sKey );
	}
//	pRuleResult=NULL;  
	if (! pRuleResult ) {
		CMRParam *pNewParm = (CMRParam *) 0;

		if ( pParmVal ) {
			CMRParam NewParm( pParmVal, parmCnt );
			pNewParm = & NewParm;
		}

//  왜 있었는지 몰라서 막음...  2005-12-16
//		if ( ! pParmVal && p->tokInfo ) strcpy( sKey, (char *) p->tokInfo );

		m_pDataList->m_HAHA[0] = 0;


		CMRRunner Runner( Rule->ruleId, sKey,
						  m_pDataList, pNewParm );

		pRuleResult = Runner.getRuleResultPtr();

		if ( pParmVal ) m_pDataList->MRMemFree ( pParmVal );

		if (m_pParent && pRuleResult) {
			m_pParent->addRuleToRunList(pRuleResult);
		}
	}

	if ( ! pRuleResult ) {
		throw new CMTException( 77777, "Rule running error." );
	}


	pValue->value.RuleItem.ptr = pRuleResult;

	/*  Row(Index)가 존재할때 값을 구한다.   */
	if (!p->pExpression) {
		pValue->value.RuleItem.i_row = 0;
	}
	else {
		pValue->value.RuleItem.i_row = getArrayIndex(p->pExpression->pNext);
	}
#endif
}


int		CMRExpression::getArrayIndex( CRULE_STMT *pIdx )
{
#ifdef _PARSER

	return 0;

#else

	if ( ! pIdx ) return 0;

	if ( ! pIdx->pNext ) {
		switch( pIdx->tokType ) {
			case TOKENTYPE_INDEX1:
			case TOKENTYPE_INDEX2:
			case TOKENTYPE_INDEX3:
				return m_pParent->getIndexValue( pIdx->tokType );
			case TOKENTYPE_CONST_N :
				return pIdx->uni.longVal;
			default :
				break;
		}
	}


	TYPE_VALUE    *expResult, expResultVal;
	CMRExpression Expression( m_pParent, m_pDataList, m_pRuleNode, pCParam );

	expResult = Expression.CalcExpression( pIdx );

	memcpy( (char*) &expResultVal, (char*) expResult, sizeof(TYPE_VALUE) );

	if ( expResultVal.dataType == TOKENTYPE_GLOBAL )
		m_pDataList->getItemValue( & expResultVal );
	else if ( expResultVal.dataType == TOKENTYPE_RULE )
		m_pDataList->GetRuleValue( & expResultVal );

	return expResultVal.value.longVal;
#endif
}


TYPE_VALUE *	CMRExpression::getRuleParmValues( CRULE_STMT *p, char *sKey, int *cnt )
{

	TYPE_VALUE *pParmVal=0;

#ifndef _PARSER

	RULE_LIST	*pCalledRule;
	CRULE_STMT	*p1, *p2;
	TOKEN_RULE	*Rule;
	char		*pKey;

	sKey[0] = 0;
	pKey = sKey;

	if ( ! p->pExpression ) {
		return (TYPE_VALUE *) 0;
	}

	if ( ! p->pExpression->pExpression ) {
		return (TYPE_VALUE *) 0;
	}

	Rule = (TOKEN_RULE*) p->uni.tokInfo;

	if ( ! (pCalledRule = m_pDataList->getRuleNodeById( Rule->ruleId )) ) {
		//printf( "   Rule ( %d ) not exist.\n", Rule->ruleId );
		printf( MSG_NOTEXIST_RULE, Rule->ruleId );
		throw new CMTException( UERR_RULENFOUND, EMSG_RULENFOUND );			
	}

	if ( ! pCalledRule->parmType ) return (TYPE_VALUE *) 0;

	int parmCnt = (int) strlen(pCalledRule->parmType);

	pParmVal = (TYPE_VALUE *) m_pDataList->MRMemAlloc1(sizeof(TYPE_VALUE) * parmCnt);

	p1 = p->pExpression->pExpression;

	int i = 0;
	while ( p1 ) {
		p2 = p1->pNext;

		if ( ! p2 ) {
			pParmVal[i].dataType = 0;
			i++;
			p1 = p1->pExpression;
			continue;
		}

		if ( i == parmCnt )
			throw new CMTException( UERR_PARMCOUNT, EMSG_PARMCOUNT );

		if ( p2->pNext )
			throw new CMTException( UERR_PARMUSEEXP, EMSG_PARMUSEEXP );

		switch( p2->tokType ) {
			case TOKENTYPE_CONST_F :
			case TOKENTYPE_CONST_C :
			case TOKENTYPE_CONST_N :
			case TOKENTYPE_CONST_L :
			case TOKENTYPE_CONST_B :
				ConvertExp2Value( p2, & pParmVal[i], pCalledRule->parmType[i] );
				break;

			case TOKENTYPE_INDEX1 :
			case TOKENTYPE_INDEX2 :
			case TOKENTYPE_INDEX3 :
				pParmVal[i].dataType = TOKENTYPE_CONST_N;
				pParmVal[i].value.longVal = m_pParent->getIndexValue( p2->tokType );
				break;

			case TOKENTYPE_GLOBAL :
                getGlobalItemValue( p2, & pParmVal[i] );
				m_pDataList->getItemValue( & pParmVal[i]);
				break;
			case TOKENTYPE_LOCAL :
				if ( ! pCParam ) {
					throw new CMTException( UERR_PARMNPASS, EMSG_PARMNPASS );
				}

				pCParam->getParmValue( p2->uni.longVal, & pParmVal[i] );
				break;
			case TOKENTYPE_RULE :
				getRuleResultValue( p2, & pParmVal[i] );

				m_pDataList->GetRuleValue( & pParmVal[i] );
				break;
			default :
				break;
		}

		if ( p2->tokType > TOKENFUNC_END ) {
			CMRExpression Expression( m_pParent, m_pDataList, m_pRuleNode, pCParam );

			TYPE_VALUE	*expResult = Expression.CalcExpression( p2 );

			if ( expResult ) {
				memcpy( (char*) & pParmVal[i], (char*) expResult, sizeof(TYPE_VALUE) );
			}
		}


		//  Parm type이 틀릴때 
		if ( pParmVal[i].dataType != pCalledRule->parmType[i] ) {

			if ( pParmVal[i].dataType == TOKENTYPE_CONST_C && pCalledRule->parmType[i] == TOKENTYPE_CONST_L ) {
				pParmVal[i].dataType = TOKENTYPE_CONST_L;
				char * pLStr = (char*) m_pDataList->MRMemAlloc1( (int) strlen(pParmVal[i].value.strVal) + 1 );
				strcpy( pLStr, pParmVal[i].value.strVal );
				pParmVal[i].value.pLStr = pLStr;
			} else if ( pParmVal[i].dataType == TOKENTYPE_CONST_L && pCalledRule->parmType[i] == TOKENTYPE_CONST_C ) {
				pParmVal[i].dataType = TOKENTYPE_CONST_C;
				char * pLStr = pParmVal[i].value.pLStr;

				memset( pParmVal[i].value.strVal,0x00, STR_VALUE_SIZE );
				strncpy( pParmVal[i].value.strVal, pLStr, STR_VALUE_SIZE );
				pParmVal[i].value.strVal[STR_VALUE_SIZE] = 0;

			} else {
				throw new CMTException( UERR_PARMTYPE, EMSG_PARMTYPE );
			}

		}


		if ( i > 0 ) {
			strcat ( pKey, "||" );   pKey += 2;
		}

//  일단 하나만....
		switch ( pParmVal[i].dataType ) {
			case DATA_TYPE_LONG   : sprintf(pKey,"%ld", pParmVal[i].value.longVal);
									break;
			case DATA_TYPE_DOUBLE : sprintf(pKey,"%.0f", pParmVal[i].value.dblVal);
									break;
			case DATA_TYPE_STRING : strcpy(pKey,pParmVal[i].value.strVal);
									break;
			case DATA_TYPE_LSTRING : strcpy(pKey,pParmVal[i].value.pLStr);
									break;
			default :
									break;
		}

		pKey += strlen(pKey);
		
		i++;
		p1 = p1->pExpression;
	}
	*cnt = i;

#endif

	return pParmVal;
}


void  CMRExpression::ExecRule( CRULE_STMT *p )
{
#ifdef _PARSER
	if ( p->pNext->tokType != TOKENTYPE_RULE ) {
		char msg[100];
		sprintf(msg,"%s (EXEC)", EMSG_GRAMMAR_99 );
		throw new CMTException( USERERR_GRAMMAR, msg );
	}

	if ( p->pExpression ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_100 );

#else

	//  기존 결과 있으면 삭제
	TOKEN_RULE   *Rule;

	Rule = (TOKEN_RULE*) p->pNext->uni.tokInfo;

	m_pDataList->DelRuleResultAll( Rule->ruleId );



	//  룰 실행
	TYPE_VALUE    expResultVal;

	CMRExpression Expression( m_pParent, m_pDataList, m_pRuleNode, pCParam );

	if ( CalcResultExpress( p->pNext, & expResultVal ) > 0 )
		throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_101 );
#endif
}


void  CMRExpression::setGblValue( CRULE_STMT *p )
{
	TYPE_VALUE    expResultVal;
	char          Name[STR_VALUE_SIZE+1];
	parseGblValue( p, Name, &expResultVal );

	int idx = 1;

	p = p->pExpression;

	if ( p->pExpression ) {
		p = p->pExpression->pNext;

#ifdef _PARSER
		if ( p->pExpression || p->pNext) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_102 );

		if ( p->tokType != TOKENTYPE_CONST_N &&
			(p->tokType < TOKENTYPE_INDEX0 || p->tokType > TOKENTYPE_INDEX3 ) )
			throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_103 );

		idx = 1;
#else
		switch ( p->tokType ) {
			case TOKENTYPE_CONST_N :
				idx = p->uni.longVal;    break;
			case TOKENTYPE_INDEX1 :
			case TOKENTYPE_INDEX2 :
			case TOKENTYPE_INDEX3 :
				idx = m_pParent->getIndexValue( p->tokType );
				break;
			default :
				throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_104 );
		}
#endif

	}

	m_pDataList->setGblVal( Name, &expResultVal, idx );

}


void  CMRExpression::addGblValue( CRULE_STMT *p )
{
	TYPE_VALUE    expResultVal;
	char          Name[STR_VALUE_SIZE+1];
	char		  msg[1000];
	parseGblValue( p, Name, &expResultVal );

#ifdef _PARSER
	if ( expResultVal.dataType != DATA_TYPE_LONG && expResultVal.dataType != DATA_TYPE_DOUBLE ){
		sprintf(msg," [%d]!=[%d]  [%d]!=[%d] \n",expResultVal.dataType, DATA_TYPE_LONG, expResultVal.dataType,DATA_TYPE_DOUBLE);
		throw new CMTException( UERR_GBLVAL_TYPE, msg );//EMSG_GBLVAL_TYPE_07 );
	}
#endif

	m_pDataList->addGblVal( Name, &expResultVal );

}


void  CMRExpression::incGblValue( CRULE_STMT *p )
{
	char          Name[STR_VALUE_SIZE+1];
	if ( p->pNext->tokType != TOKENTYPE_CONST_C ) {
		char msg[100];
		sprintf(msg,"%s (INCVAL)", EMSG_GRAMMAR_105 );
		throw new CMTException( USERERR_GRAMMAR, msg );
	}

	strcpy( Name, (char*) p->pNext->uni.tokInfo );

#ifdef _PARSER
	if ( ! Name[0] ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_106 );
	if ( strcmp(Name,"1900-01-01") == 0 ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_107 );
	if ( p->pExpression ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_108 );
#endif

	m_pDataList->incGblVal( Name );

}

void  CMRExpression::pushGblValue( CRULE_STMT *p )
{
	TYPE_VALUE    expResultVal;
	char          Name[STR_VALUE_SIZE+1];

	parseGblValue( p, Name, &expResultVal );

	m_pDataList->pushGblVal( Name, &expResultVal );

}


void  CMRExpression::clrGblValue( CRULE_STMT *p )
{
	char          Name[STR_VALUE_SIZE+1];

	if ( p->pNext->tokType != TOKENTYPE_CONST_C ) {
		char msg[100];
		sprintf(msg,"%s (CLRVAL)", EMSG_GRAMMAR_109 );
		throw new CMTException( USERERR_GRAMMAR, msg );
	}

	strcpy( Name, (char*) p->pNext->uni.tokInfo );

#ifdef _PARSER
	if ( ! Name[0] ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_110 );
	if ( strcmp(Name,"1900-01-01") == 0 ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_111 );
	if ( p->pExpression ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_112 );
#endif

	m_pDataList->clearGbl( Name );

}


void  CMRExpression::parseGblValue( CRULE_STMT *p, char *Name, TYPE_VALUE *tVal )
{
	if ( ! p->pNext ) {
//printf("CMRExpression::parseGblValue ==> null\n");

		throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_113 );
	}

#ifdef _PARSER
	if ( p->pNext->tokType != TOKENTYPE_CONST_C ) {
		throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_114 );
	}
#endif

	strcpy( Name, (char*) p->pNext->uni.tokInfo );
	
#ifdef _PARSER
	if ( strcmp(Name,"1900-01-01") == 0 ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_115 );
#endif

	p = p->pExpression;

#ifdef _PARSER
	if ( p->pExpression ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_116 );
#endif

	CMRExpression Expression( m_pParent, m_pDataList, m_pRuleNode, pCParam );

	if ( CalcResultExpress( p->pNext, tVal ) > 0 )
		throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_117 );

	if ( tVal->dataType == TOKENTYPE_GLOBAL )
		m_pDataList->getItemValue( tVal );
	else if ( tVal->dataType == TOKENTYPE_RULE )
		m_pDataList->GetRuleValue( tVal );
}
