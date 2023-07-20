#include    <string.h>
#include    <stdio.h>
#include    <stdlib.h>

#include    "MRERROR.h"
#include    "MTExcept.h"
#include    "MRTOKEN.h"
#include    "MRRUNNER.h"
#include    "MRCRINTR.h"
#include    "MRGBLVAR.h"
#include    "MREXPRES.h"

#ifdef _TESTER
#include    "BRMStart.h"
#endif

#ifdef _MSG_KO
#define   MSG_CONDITION   "   R(%d) 조건 %d : 시작"
#else
#define   MSG_CONDITION   "   R(%d) Condition %d : Start"
#endif

CMRCInterpreter::CMRCInterpreter(CMRRunner *pParent, CMRDataList *pDataList,
		RULE_LIST *pRuleNode, CMRParam *pParm) {
	m_pParent = pParent;
	m_pDataList = pDataList;
	m_pRuleNode = pRuleNode;
	m_retCnt = m_pRuleNode->retCnt;
	m_pParm = pParm;

	pResultList = 0;

#ifdef _TRACE
	m_TraceFlag = m_pDataList->getTraceFlag();
#endif

//	m_lCode = 0;
//	m_lpszMessage = "";

}

CMRCInterpreter::~CMRCInterpreter() {
	Delete();

//	if( m_lpszMessage )  delete[] m_lpszMessage;
}

/*
 long	CMRCInterpreter::GetECode( void ) const
 {
 return m_lCode;
 }

 const char*	CMRCInterpreter::GetEMessage( void ) const
 {
 return m_lpszMessage;
 }
 */

void CMRCInterpreter::Delete(void) {

}

RESULT_REC * CMRCInterpreter::Running() {
	CRULE_DATA *pCRule;
	RESULT_REC *pTotalResult;
	RESULT_REC *pLastResult = 0;
	long rc, i = 0;

	pTotalResult = 0;

	m_bBreak = 0; 

	//Asign return, Condition Starting pointer to pCRule
	pCRule = (CRULE_DATA*) m_pDataList->getVersionNode(m_pRuleNode);

//	strcpy(pValue->value.strVal, (char *)p->uni.tokInfo);
//	pCRule->cond 

	//pCRule->cond : Condition Node(Rows)

//#ifdef _DEBUG
 

//		int mode;
//		VALUE_UNION *pResValue;
//		pResValue = buildResultRec((CRULE_STMT*)pCRule->result, pCRule, &mode);


		/*
			printf("MRCRINTR RULE ID (%d) \n", m_pRuleNode->retCnt);
			printf("MRCRINTR RULE ID (%d) \n", m_pRuleNode->retCnt);
			printf("MRCRINTR RULE ID (%d) \n", m_pRuleNode->retCnt);
			printf("MRCRINTR RULE ID (%d) \n", m_pRuleNode->retCnt);
		*/
		m_retCnt = m_pRuleNode->retCnt;
		
//#endif

	while (pCRule) {

		if (m_pDataList->m_bExit)
			break;

		i++;

#ifdef _TESTER
		m_pWorker->SuspendWork( _DBG_STEP_NEXT, i, 1, "" );
#endif

#ifdef _TRACE
		if ( m_TraceFlag )
		{
			//sprintf(m_TraceMsg, "   R(%d) Condition %d : Start", m_pRuleNode->rule_id, i );
			sprintf(m_TraceMsg, MSG_CONDITION, m_pRuleNode->rule_id, i );
			m_pDataList->TraceOut( m_TraceMsg );
		}
#endif
/*		struct _CRULE_DATA {
			char		 resGbn;    //   0:Normal, 1:Constant
			void         *result;
			CRULE_STMT   *cond;
			CRULE_DATA   *pNext;
		};
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
		}; */
		rc = CalculateCondition(pCRule->cond);

#ifdef _TRACE
		if ( m_TraceFlag )
		{
		    sprintf(m_TraceMsg, "R(%d) Condition %d : %s",  m_pRuleNode->rule_id, i, rc ? "TRUE":"FALSE" );
			m_pDataList->TraceOut( m_TraceMsg );
			sprintf(m_TraceMsg, "   R(%d) Result    %d : Start", m_pRuleNode->rule_id, i );
			m_pDataList->TraceOut( m_TraceMsg );
		}
#endif

#ifdef _TESTER

		m_pWorker->SuspendWork( _DBG_STEP_COND, i, rc, "" );

		m_pWorker->SuspendWork( _DBG_STEP_NEXT, i, 2, "" );

#endif

		if (!rc) {
			pCRule = pCRule->pNext;
			continue;
		}

		pResultList = 0;

		if (pCRule->resGbn == 1) {
			int sz = sizeof(VALUE_UNION) * m_pRuleNode->retCnt;

			if (!pTotalResult) {
				pTotalResult = pLastResult =
						(RESULT_REC *) m_pDataList->MRMemAlloc1(
								sizeof(RESULT_REC));
			} else {
				pLastResult->pNext = (RESULT_REC *) m_pDataList->MRMemAlloc1(
						sizeof(RESULT_REC));
				pLastResult = pLastResult->pNext;
			}
			pLastResult->pNext = 0;

			pLastResult->value = (VALUE_UNION *) m_pDataList->MRMemAlloc1(sz);
			memcpy((char *) pLastResult->value, (char *) pCRule->result, sz);

			pResultList = pLastResult;

		} else if (pCRule->resGbn == 2) { //  1차원 Array
			int sz = sizeof(VALUE_UNION) * m_pRuleNode->retCnt;

			pResultList = (RESULT_REC *) pCRule->result;

			RESULT_REC *pRecList = pResultList;

			while (pRecList) {
				if (!pTotalResult) {
					pTotalResult = pLastResult =
							(RESULT_REC *) m_pDataList->MRMemAlloc1(
									sizeof(RESULT_REC));
				} else {
					pLastResult->pNext =
							(RESULT_REC *) m_pDataList->MRMemAlloc1(
									sizeof(RESULT_REC));
					pLastResult = pLastResult->pNext;
				}
				pLastResult->pNext = 0;

				pLastResult->value = (VALUE_UNION *) m_pDataList->MRMemAlloc1(
						sz);
				memcpy((char *) pLastResult->value, (char *) pRecList->value,
						sz);

				pRecList = pRecList->pNext;
			}
		} else {
			BuildResult((CRULE_STMT*) pCRule->result);

			if (m_bBreak == 4) {
				m_pDataList->m_bExit = 1;
				break;
			}

			if (m_bBreak == 3) { //  SETVAL, INCVAL, RCLEAR일때는 다음Line으로...
				pCRule = pCRule->pNext;
				continue;
			}

			if (m_bBreak != 0)
				break;

			if (pResultList) {
				if (!pTotalResult)
					pTotalResult = pResultList;
				else
					pLastResult->pNext = pResultList;

				pLastResult = pResultList;

				while (pLastResult->pNext)
					pLastResult = pLastResult->pNext;
			}
		}

#ifdef _TESTER
		if ( pResultList ) {
			m_pDataList->DbgRuleReturn( m_pRuleNode, pResultList, "" );
			m_pWorker->SuspendWork( _DBG_STEP_RET, 0, 0, "" );
		}
#endif

		if (m_pRuleNode->breakFlag)
			break;

//		if ( m_pDataList->m_bExit ) break;

		pCRule = pCRule->pNext;
	}

	return pTotalResult;
}

/*  TRule에서만 쓰임  */
RESULT_REC * CMRCInterpreter::OneResult(CRULE_STMT *pStmt) {
	m_bBreak = 0;

	BuildResult(pStmt);

	if (m_bBreak == 4)
		m_pDataList->m_bExit = 1;

	return pResultList;
}

/***********************************************
 * Parm : CRULE_STMT *pStmt : Condition Statement
 ************************************************/
int CMRCInterpreter::CalculateCondition(CRULE_STMT *pStmt) {
	TYPE_VALUE *expResult;
	int cond;

	if (!pStmt)
		return true;

	//Asigned Only Pointer for CMRExpression
	CMRExpression Expression(m_pParent, m_pDataList, m_pRuleNode, m_pParm);
	  

	expResult = Expression.CalcExpression(pStmt);
	  
//	printf("CalculateCondition RULE_ID [%d]==================================>\n", m_pRuleNode->rule_id		);

//	printf("CalculateCondition RULE_ID [%d]==pStmt->tokType [%d]\n", m_pRuleNode->rule_id, pStmt->tokType);
//	printf("CalculateCondition RULE_ID [%d]==pStmt->tokType [%d]\n", pStmt->pExpression);

	if (expResult->dataType == TOKENTYPE_RULE)
		m_pDataList->GetRuleValue(expResult);
	if (expResult->dataType == TOKENTYPE_GLOBAL)
		m_pDataList->getItemValue(expResult);

#ifdef _PARSER	
	if ( expResult->dataType != DATA_TYPE_BOOL && expResult->dataType != DATA_TYPE_LONG) {
		throw new CMTException( UERR_CONDINVALID, EMSG_CONDINVALID );
	}
#endif

	if (expResult->dataType == DATA_TYPE_BOOL)
		cond = (bool) expResult->value.boolVal;
	else
		cond = (bool) expResult->value.longVal;
//	printf(" cond [%d] \n",cond);
	return cond;
}

void CMRCInterpreter::BuildResult(CRULE_STMT *pStmt) {
	CRULE_STMT *pExp;
	TYPE_VALUE expResultVal, expResultOcc;
	int usedRuleCnt = 0;
	int idxPosition;
	int cnv1, cnv2 = 0;

	pResultList = 0;


	if (!pStmt)
		return; //  Line empty

	m_pDataTypes = m_pRuleNode->retType;

	/*  SUBSTR All 처리 */
	if (!pStmt->pExpression) {
		pExp = pStmt->pNext;
		if (pExp->tokType == TOKENFUNC_SUBSTR && pExp->pNext == 0) {
			pExp = pExp->pExpression;

			if (pExp->pNext->tokType == TOKENTYPE_GLOBAL
					&& pExp->pNext->pExpression == 0) {
				getSubStrAll(pExp);
				return;
			}
		}
	}

	CMRExpression Expression(m_pParent, m_pDataList, m_pRuleNode, m_pParm);

	m_bBreak = Expression.CalcResultExpress(pStmt->pNext, &expResultVal);

	if (m_bBreak > 0)
		return;

	if (pStmt->pExpression == 0) {
		if (expResultVal.dataType == TOKENTYPE_RULE) {
			if (expResultVal.value.RuleItem.i_row == 0) {
				getOtherRuleResult(&expResultVal);
				return;
			}
		} else if (expResultVal.dataType == TOKENTYPE_GLOBAL) {
			pResultList = m_pDataList->getAllItemValues(&expResultVal,m_pDataTypes[0]);
			return;
		}
	}

	int sz = sizeof(VALUE_UNION) * m_retCnt;

	pResultList = (RESULT_REC *) m_pDataList->MRMemAlloc1(sizeof(RESULT_REC));

	pResultList->value = (VALUE_UNION *) m_pDataList->MRMemAlloc1(sz);

	pResultList->pNext = (RESULT_REC *) 0;

	pExp = pStmt;
	m_vIndex = 0;

	while (1) {
		if (expResultVal.dataType < TOKENTYPE_CONST_V) {
			moveValueToResult(&expResultVal, pResultList->value);
			if (!(pExp = pExp->pExpression))
				break;
			Expression.CalcResultExpress(pExp->pNext, &expResultVal);
			continue;
		}

		if (expResultVal.dataType == TOKENTYPE_RULE) {
			if (expResultVal.value.RuleItem.i_row == 0) {
				RULE_RESULT *pResult =
						(RULE_RESULT *) expResultVal.value.RuleItem.ptr;
				int bSingle = 0;

				if (pResult->resRecord) {
					if (!pResult->resRecord->pNext)
						bSingle = 1;
				}

				if (!bSingle) { //  내포하는 룰의 결과가 2라인 이상일때
					if (++usedRuleCnt == 1) {
						memcpy(&expResultOcc, &expResultVal,
								sizeof(TYPE_VALUE));
						idxPosition = m_vIndex;
					}
				}
			}

			//  나중에 버릴지라도 1Row로 간주하고 결과를 만든다.
			if (expResultVal.value.RuleItem.i_col == 0) {
				if ((cnv1 = AddRuleValue(&expResultVal, pResultList->value))
						== -1) {
					pResultList = 0;
					return;
				}
			} else {
				try {
					m_pDataList->GetRuleValue(&expResultVal);
					// cnv1 : 타입변경 여부
					cnv1 = moveValueToResult(&expResultVal, pResultList->value);
				} catch (CMTException *e) {
					pResultList = 0;
					e->Delete();
//					throw new CMTException( UERR_RESULTOUTOFBIND, EMSG_RESULTOUTOFBIND );
					return;
				}
			}

			if (cnv1)
				cnv2 = 1; //  현재 비사용

		} else { // TOKENTYPE_GLOBAL
			m_pDataList->getItemValue(&expResultVal);
			moveValueToResult(&expResultVal, pResultList->value);
		}

		if (!(pExp = pExp->pExpression))
			break;

		Expression.CalcResultExpress(pExp->pNext, &expResultVal);

	}

//	if ( usedRuleCnt == 1 && cnv2 == 0 ) ConstantPlusOtherRuleResult( &expResultOcc,idxPosition );
	if (usedRuleCnt == 1) {
		ConstantPlusOtherRuleResult(&expResultOcc, idxPosition);
	}
}

/*
 void CMRCInterpreter::BuildErrorResult( CRULE_STMT *pStmt )
 {
 CRULE_STMT	*pExp;
 TYPE_VALUE	*expResult, expResultVal;
 int          rc;
 int			 rCnt = 0;

 #ifdef _DEBUG
 printf("   Error Result Calculate.\n" );
 #endif

 if ( ! pStmt ) return;

 pExp = pStmt;

 CMRExpression Expression( this, m_pDataList, m_pRuleNode, m_pParm );

 expResult = Expression.CalcExpression( pExp->pNext );
 memcpy( (char*) &expResultVal, (char*) expResult, sizeof(TYPE_VALUE) );

 if ( pExp->pExpression == NULL ) {
 if ( expResultVal.dataType == TOKENTYPE_RULE &&
 expResultVal.value.RuleItem.i_row == 0 )
 {
 expResultVal.value.RuleItem.i_row = 1;
 }
 }

 while ( 1 ) {
 if ( expResultVal.dataType == TOKENTYPE_RULE &&
 expResultVal.value.RuleItem.i_col == 0 ) {

 rc = AddErrorResultValue( &expResultVal );
 if ( rc == -1 ) {
 throw new CMTException( UERR_RESULTOUTOFBIND, EMSG_RESULTOUTOFBIND );
 }

 } else {
 if ( expResultVal.dataType == TOKENTYPE_RULE ) {
 try {
 m_pDataList->GetRuleValue( &expResultVal );
 } catch( CMTException *e ) {
 e->Delete();

 throw new CMTException( UERR_RESULTOUTOFBIND, EMSG_RESULTOUTOFBIND );
 }

 } else if ( expResultVal.dataType == TOKENTYPE_GLOBAL )
 m_pDataList->getItemValue( &expResultVal );

 m_pDataList->PutErrResultColumn( &expResultVal );
 }

 pExp = pExp->pExpression;
 if ( ! pExp ) break;

 expResult = Expression.CalcExpression( pExp->pNext );
 memcpy( (char*) &expResultVal, (char*) expResult, sizeof(TYPE_VALUE) );
 }

 throw new CMTException( 0, "User Exit Occurs" );
 }

 */

/*
 void  CMRCInterpreter::BuildParmResult( int *parmCnt,
 CRULE_STMT *pStmt,
 VALUE_UNION *pParmVal,
 char * pDataTypes)
 {
 CRULE_STMT	*pExp;
 TYPE_VALUE	expResultVal;
 int          rc = 0;

 #ifdef _DEBUG
 if ( MtsDebugLevel > 2 ) {
 printf("   Parameter Calculate.\n" );
 }
 #endif

 if ( ! pStmt->pNext ) {
 printf("Expression required in result line.\n" );
 return;
 }

 m_pDataTypes = pDataTypes;

 CMRExpression Expression( m_pParent, m_pDataList, m_pRuleNode, m_pParm );

 pExp = pStmt;
 m_vIndex = 0;

 while ( pExp ) {
 m_bBreak = Expression.CalcResultExpress( pExp->pNext, &expResultVal );
 //		if ( m_bBreak > 0 ) return;

 if ( expResultVal.dataType < TOKENTYPE_CONST_V ) {
 moveValueToResult( &expResultVal, pParmVal );
 pExp = pExp->pExpression;
 continue;
 }

 if ( expResultVal.dataType == TOKENTYPE_RULE ) {
 if ( expResultVal.value.RuleItem.i_col == 0 ) {
 AddRuleValue( &expResultVal, pParmVal );
 } else {
 try {
 m_pDataList->GetRuleValue( &expResultVal );
 moveValueToResult( &expResultVal, pParmVal );
 } catch( CMTException *e ) {
 e->Delete();
 throw new CMTException( UERR_RESULTOUTOFBIND, EMSG_RESULTOUTOFBIND );
 }
 }
 } else {
 if ( expResultVal.dataType == TOKENTYPE_GLOBAL )
 m_pDataList->getItemValue( &expResultVal );

 moveValueToResult( &expResultVal, pParmVal );
 }

 pExp = pExp->pExpression;
 }
 }
 */

void CMRCInterpreter::getOtherRuleResult(TYPE_VALUE *expResult) {
	RESULT_REC *pLast=NULL, *pTemp;
	RULE_RESULT *pRuleResult;
	char retType[101];
	short bCmp = 1;

	pRuleResult = (RULE_RESULT *) expResult->value.RuleItem.ptr;
	if (!pRuleResult)
		return;

	int i_col = expResult->value.RuleItem.i_col;

	if (i_col == 0 && m_retCnt != pRuleResult->pRuleNode->retCnt) {
		char msg[200];

		sprintf(msg, "%s %d %d ( %d ).", EMSG_RESULTCOUNT, m_retCnt,
				pRuleResult->pRuleNode->retCnt, pRuleResult->ruleId);
		throw new CMTException(UERR_RESULTCOUNT, msg);
	}

	memcpy(retType, pRuleResult->pRuleNode->retType,
			pRuleResult->pRuleNode->retCnt);

	if (i_col == 0)
		bCmp = memcmp(retType, m_pDataTypes, pRuleResult->pRuleNode->retCnt);

	pTemp = pRuleResult->resRecord;

	while (pTemp) {
		if (!pResultList) {
			pResultList = (RESULT_REC *) m_pDataList->MRMemAlloc1(sizeof(RESULT_REC));
			pLast = pResultList;
		} else {
			pLast->pNext = (RESULT_REC *) m_pDataList->MRMemAlloc1(sizeof(RESULT_REC));
			pLast = pLast->pNext;
		}
		pLast->pNext = 0;

		if (bCmp == 0) {
			pLast->value = pTemp->value;
			pTemp = pTemp->pNext;
			continue;
		}

		if (i_col == 0) {
			pLast->value = (VALUE_UNION *) m_pDataList->MRMemAlloc1(sizeof(VALUE_UNION) * m_retCnt);

			for (int i = 0; i < m_retCnt; i++) {
				if (retType[i] == m_pDataTypes[i])
					memcpy((char *) &pLast->value[i], (char *) &pTemp->value[i],sizeof(VALUE_UNION));
				else {
					TYPE_VALUE Tval;
					Tval.dataType = retType[i];
					memcpy((char*) &Tval.value, (char *) &pTemp->value[i],sizeof(VALUE_UNION));
					m_vIndex = i;
					convertValueToReturnType(&Tval);
					memcpy((char *) &pLast->value[i], (char *) &Tval.value,	sizeof(VALUE_UNION));
				}
			}
		} else {
			int col = i_col - 1;

			pLast->value = (VALUE_UNION *) m_pDataList->MRMemAlloc1(sizeof(VALUE_UNION));

			if (retType[col] == m_pDataTypes[col]) {
				memcpy((char *) pLast->value, (char *) &pTemp->value[col],	sizeof(VALUE_UNION));
			} else {
				TYPE_VALUE Tval;
				Tval.dataType = retType[col];
				memcpy((char*) &Tval.value, (char *) &pTemp->value[col],	sizeof(VALUE_UNION));
				m_vIndex = 0;
				convertValueToReturnType(&Tval);
				memcpy((char *) pLast->value, (char *) &Tval.value,			sizeof(VALUE_UNION));

			}
		}

		pTemp = pTemp->pNext;
	}
}

void CMRCInterpreter::getSubStrAll(CRULE_STMT *p) {
	RESULT_REC *pLast;

	unsigned int st, len;
	int tok;

	int ItemCol = m_pDataList->getItemColumn(p->pNext->uni.longVal);
	if (ItemCol < 0)
		return;

	VALUE_UNION *pVal = m_pDataList->getItemValuePtr(ItemCol);
	int rows = m_pDataList->getItemValueCount(ItemCol);

	p = p->pExpression;

	tok = p->pNext->tokType;
	if (tok == TOKENTYPE_CONST_N)
		st = p->pNext->uni.longVal;
	else
		throw new CMTException(USERERR_GRAMMAR, EMSG_GRAMMAR_08);
	p = p->pExpression;
	len = p->pNext->uni.longVal;

	for (int i = 0; i < rows; i++) {
		if (strlen(pVal[i].strVal) < st)
			continue;

		if (!pResultList) {
			pResultList = (RESULT_REC *) m_pDataList->MRMemAlloc1(
					sizeof(RESULT_REC));
			pLast = pResultList;
		} else {
			pLast->pNext = (RESULT_REC *) m_pDataList->MRMemAlloc1(
					sizeof(RESULT_REC));
			pLast = pLast->pNext;
		}
		pLast->pNext = 0;

		pLast->value = (VALUE_UNION *) m_pDataList->MRMemAlloc1(
				sizeof(VALUE_UNION));
		strncpy(pLast->value->strVal, pVal[i].strVal + st - 1, len);
		pLast->value->strVal[len] = 0;
	}
}

void CMRCInterpreter::ConstantPlusOtherRuleResult(TYPE_VALUE *expResult,
		int Cnt1) {
	RESULT_REC *pResultListTemp, *pLast, *pTemp;
	RULE_RESULT *pRuleResult;

	pResultListTemp = 0;

	pRuleResult = (RULE_RESULT *) expResult->value.RuleItem.ptr;

	if (!pRuleResult) {
		pResultList = 0;
		return;
	}

	int retCnt = pRuleResult->pRuleNode->retCnt;
	int col = expResult->value.RuleItem.i_col;
	int CntTail;

	if (col > 0) {
		CntTail = Cnt1 + 1;
	} else {
		CntTail = Cnt1 + retCnt;
	}

	pTemp = pRuleResult->resRecord;

	while (pTemp) {
		if (!pResultListTemp) {
			pResultListTemp = (RESULT_REC *) m_pDataList->MRMemAlloc1(
					sizeof(RESULT_REC));
			pLast = pResultListTemp;
		} else {
			pLast->pNext = (RESULT_REC *) m_pDataList->MRMemAlloc1(
					sizeof(RESULT_REC));
			pLast = pLast->pNext;
		}
		pLast->pNext = 0;

		pLast->value = (VALUE_UNION *) m_pDataList->MRMemAlloc1(
				sizeof(VALUE_UNION) * m_retCnt);

		//  룰 앞의 상수들 move
		if (Cnt1 > 0) {
			memcpy((char *) pLast->value, (char *) pResultList->value,
					sizeof(VALUE_UNION) * Cnt1);
		}

		//  룰 value move
		if (col == 0) {

			TYPE_VALUE Tval;
			m_vIndex = Cnt1;

			for (int z = 0; z < retCnt; z++) {
				Tval.dataType = pRuleResult->pRuleNode->retType[z];
				memcpy(&Tval.value, &pTemp->value[z], sizeof(VALUE_UNION));

				moveValueToResult(&Tval, pLast->value);
			}

		} else {
//			int col = expResult->value.RuleItem.i_col - 1;
//			retCnt = 1;

			col--;
			memcpy((char *) &pLast->value[Cnt1], (char *) &pTemp->value[col],
					sizeof(VALUE_UNION));
		}

		//  룰 뒤의 상수들 move
		if (CntTail < m_retCnt) {
			memcpy((char *) &pLast->value[CntTail],
					(char *) &pResultList->value[CntTail],
					sizeof(VALUE_UNION) * (m_retCnt - CntTail));
		}

		pTemp = pTemp->pNext;
	}

	pResultList = pResultListTemp;
}

int CMRCInterpreter::moveValueToResult(TYPE_VALUE *pTval,
		VALUE_UNION *pValues) {
	int cnv = convertValueToReturnType(pTval);

	memcpy((char *) &pValues[m_vIndex], (char *) &pTval->value,
			sizeof(VALUE_UNION));

	m_vIndex++;

	return cnv;
}

int CMRCInterpreter::convertValueToReturnType(TYPE_VALUE *pTval) {
	char *p;

	if (pTval->dataType == m_pDataTypes[m_vIndex])
		return 0;

	switch (m_pDataTypes[m_vIndex]) {
	case TOKENTYPE_CONST_F:
		if (pTval->dataType == TOKENTYPE_CONST_N)
			pTval->value.dblVal = (double) pTval->value.longVal;
		else if (pTval->dataType == TOKENTYPE_CONST_B)
			pTval->value.dblVal = (double) pTval->value.boolVal;
		else
			pTval->value.dblVal = 0.0;

		break;

	case TOKENTYPE_CONST_N:
		if (pTval->dataType == TOKENTYPE_CONST_F) {
			char *p, temp[31];
			int ch = '.';
			sprintf(temp, "%lf", pTval->value.dblVal);
			temp[30]=0;
			p = strchr(temp, ch);
			*p = 0;
			pTval->value.longVal = atoi(temp);

		} else if (pTval->dataType == TOKENTYPE_CONST_B)
			pTval->value.longVal = pTval->value.boolVal;
		else
			pTval->value.longVal = 0;

		break;

	case TOKENTYPE_CONST_C:
		memset(pTval->value.strVal,0x00, STR_VALUE_SIZE );
		if (pTval->dataType == TOKENTYPE_CONST_L) { 
			if (pTval->value.pLStr) {
				strncpy(pTval->value.strVal, pTval->value.pLStr,STR_VALUE_SIZE);
			}else
				pTval->value.strVal[0] = 0;
		} else if (pTval->dataType == TOKENTYPE_CONST_F)
			sprintf(pTval->value.strVal, "%lf", pTval->value.dblVal);
		else if (pTval->dataType == TOKENTYPE_CONST_N)
			sprintf(pTval->value.strVal, "%ld", pTval->value.longVal);
		else if (pTval->dataType == TOKENTYPE_CONST_B) {
			sprintf(pTval->value.strVal, "%s", pTval->value.boolVal ? "TRUE" : "FALSE");
		} 
		pTval->value.strVal[STR_VALUE_SIZE] = 0;
		break;

	case TOKENTYPE_CONST_L:
		p = (char *) m_pDataList->MRMemAlloc1(STR_VALUE_SIZE + 1);

		if (pTval->dataType == TOKENTYPE_CONST_C)
			strcpy(p, pTval->value.strVal);
		else if (pTval->dataType == TOKENTYPE_CONST_F)
			sprintf(p, "%lf", pTval->value.dblVal);
		else if (pTval->dataType == TOKENTYPE_CONST_N)
			sprintf(p, "%ld", pTval->value.longVal);
		else if (pTval->dataType == TOKENTYPE_CONST_B)
			sprintf(p, "%s", pTval->value.boolVal ? "TRUE" : "FALSE");

		pTval->value.pLStr = p;
//			pTval->value.LString.len = strlen( p );
		break;

	case TOKENTYPE_CONST_B:
		if (pTval->dataType != TOKENTYPE_CONST_B)
			pTval->value.boolVal = (bool) pTval->value.longVal;
		break;

	}

	pTval->dataType = m_pDataTypes[m_vIndex];

	return 1;
}

void CMRCInterpreter::setInitValue(VALUE_UNION *Value, char retType) {
	switch (retType) {
	case DATA_TYPE_LONG:
		Value->longVal = 0;
		break;
	case DATA_TYPE_STRING:
		Value->strVal[0] = 0;
		break;
	case DATA_TYPE_DOUBLE:
		Value->dblVal = 0.0;
		break;
	case DATA_TYPE_LSTRING:
		Value->pLStr = 0;
//					Value->LString.len = 0;
		break;
	case DATA_TYPE_BOOL:
		Value->boolVal = false;
		break;
	default:
		break;
	}
}

int CMRCInterpreter::AddRuleValue(TYPE_VALUE *expResult, VALUE_UNION *pValues) {
	RULE_RESULT *pRuleResult;
	RESULT_REC *pRec;
	RULE_LIST *pRuleNode;
	TYPE_VALUE vTval;
	int i;
	int cnv = 0;

	pRuleResult = (RULE_RESULT *) expResult->value.RuleItem.ptr;

	if (!pRuleResult)
		return -1;

	if (expResult->value.RuleItem.i_row < 0) {
		pRuleNode = pRuleResult->pRuleNode;

		for (i = 0; i < pRuleNode->retCnt; i++) {
			setInitValue(&pValues[i], m_pDataTypes[i]);
		}

		return 0;
	}

	pRec = pRuleResult->resRecord;
	if (!pRec)
		return -1;

	for (i = 1; i < expResult->value.RuleItem.i_row; i++) {
		if (!pRec->pNext)
			return -1;

		pRec = pRec->pNext;
	}

	pRuleNode = pRuleResult->pRuleNode;

	for (i = 0; i < pRuleNode->retCnt; i++) {
		memcpy(&vTval.value, &pRec->value[i], sizeof(VALUE_UNION));
		vTval.dataType = pRuleNode->retType[i];

		if (moveValueToResult(&vTval, pValues))
			cnv = 1;
	}

	return cnv;
}

