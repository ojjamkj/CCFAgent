#include    <string.h>
#include    <stdio.h>
#include    <stdlib.h>

#include    "MRTOKEN.h"
#include    "MRERROR.h"
#include    "MTExcept.h"
#include    "MRRUNNER.h"
#include    "MRTRINTR.h"
#include    "MRGBLVAR.h"
#include    "MRCRINTR.h"

#include    "MREXPRES.h"

#include    "MRCRPARS.h"

#include    "MRPARSE.h"
#include    "MREXPRES.h"
#include    "MTUtil.h"

#include    "MRQRINTR.h"

#ifdef _TESTER
#include    "BRMStart.h"
#endif

#ifdef _MSG_KO
#define   MSG_NOTFOUND_ITEM     "Item (%d)를 찾을 수 없습니다."
#define   MSG_COMP_VALTYPE_ERR  "값 형태 비교 오류."
#define   MSG_VALTYPE_ERR       "값 형태 오류\n"
#else
#define   MSG_NOTFOUND_ITEM     "Item (%d) not found."
#define   MSG_COMP_VALTYPE_ERR  "Compare value type error."
#define   MSG_VALTYPE_ERR       "Value type error\n"
#endif

 
CMRTInterpreter::CMRTInterpreter(CMRRunner *pParent, CMRDataList *pDataList,
		RULE_LIST *pRuleNode, CMRParam *pParm) {
	m_pParent = pParent;
	m_pDataList = pDataList;
	m_pRuleNode = pRuleNode;
	m_pParm = pParm;
/*
	if (m_pParm == NULL) {
//		printf("CMRTInterpreter  004-01 m_pParm=NULL \n");
	}
	else {
//		printf("CMRTInterpreter  004-01 m_pParm=NOT NULL pRuleNode->rule_id [%d] \n", pRuleNode->rule_id);
//		printf("CMRTInterpreter  004-02 m_pParm->m_ParmCnt =>[%d] \n", m_pParm->m_ParmCnt);
//		printf("CMRTInterpreter   m_pParm->m_ParmCnt =>[%s] \n", m_pParm->m_pParmVal->value.strVal);
//		printf("CMRTInterpreter   m_pParm->m_ParmCnt =>[%s] \n", m_pParm->m_pParmVal->value.pLStr);
		for (int i = 0; i < m_pParm->m_ParmCnt; i++) {
			TYPE_VALUE tVal;
			m_pParm->getParmValue(i+1, &tVal);
			switch (tVal.dataType) {
			case DATA_TYPE_LONG:
				printf("m_pParm([%d]:L)==>%ld \n",i, tVal.value.longVal);  //lf->ld로변경
				break;
			case DATA_TYPE_DOUBLE:
				printf("m_pParm([%d]:D)==>%d \n", i, tVal.value.dblVal);  //lf->ld로변경
				break;
			case DATA_TYPE_STRING:
				printf("m_pParm([%d]:S)==>%s \n", i, tVal.value.strVal);  //lf->ld로변경
				break;
			case DATA_TYPE_LSTRING:
				printf("m_pParm([%d]:LS)==>%s\n", i, tVal.value.pLStr);  //lf->ld로변경
				break;
			}
		}
		printf("CMRTInterpreter  004--03 m_pParm->m_ParmCnt =>[%d] \n", m_pParm->m_ParmCnt);
	}
*/

#ifdef _TRACE
	m_TraceFlag = m_pDataList->getTraceFlag();
#endif
}

CMRTInterpreter::~CMRTInterpreter() {
}


int CMRTInterpreter::getRuleCd( char *src) {
	char lpszSource[100];
	sprintf(lpszSource, " %s", src); lpszSource[99] = 0;
	int length=strlen(lpszSource);
	for (int i = 0; i < length; i++) {
		switch (lpszSource[i]) {
		case '{':	lpszSource[i] = ' '; break;
		case ':':
			lpszSource[i] = 0x00;
			i = length;
			break;
		}
	}
/*	for (int i = strlen(lpszSource) - 1; i >= 0; i--)
	{
		switch (lpszSource[i]) {
		case ':' : case '{' : case '}':
			lpszSource[i] = ' ';
			break;
		default :
			break;
		}
	}
*/
	char *p = lpszSource;
	while (*p) {
		if (*p != ' ') break;
		p++;
	}
	int ruleId = atoi(lpszSource);
//	printf("ruleid =====================>[%d]  \n", ruleId);
	return (ruleId);
}

RESULT_REC * CMRTInterpreter::SimpleResult() {
	RESULT_REC *pTotalResult = 0;
	RESULT_REC *pLastResult = 0;

	VALUE_UNION *p = (VALUE_UNION*) pTRuleCol[0].vMin;

#ifdef _DEBUG
	printf("[CMRTInterpreter::SimpleResult] begin\n");
#endif

	for (int n = 0; n < m_rows; n++) {

#ifdef _DEBUG
		printf("[CMRTInterpreter::SimpleResult] Line %d - %c\n", n,   p[n].strVal);
#endif

#ifdef _TESTER
		m_pWorker->SuspendWork( _DBG_STEP_NEXT, n+1, 2, "" );
#endif
		if (p[n].strVal) {
			if (CompareValue(&Value[0], &p[n]) != 0)
				continue;
		}

#ifdef _DEBUG
		printf("[CMRTInterpreter::SimpleResult] Matching\n");
#endif
		//  ADD Result
		if (!pTotalResult) {
			pTotalResult = pLastResult =
					(RESULT_REC *) m_pDataList->MRMemAlloc1(sizeof(RESULT_REC));
		} else {
			pLastResult->pNext = (RESULT_REC *) m_pDataList->MRMemAlloc1(
					sizeof(RESULT_REC));
			pLastResult = pLastResult->pNext;
		}
		pLastResult->pNext = 0;

		pLastResult->value = (VALUE_UNION *) m_pDataList->MRMemAlloc1(
				m_ResRecSize);
		memcpy((char *) pLastResult->value, (char *) pTRule->result[n],
				m_ResRecSize);

#ifdef _TESTER
		m_pDataList->DbgRuleReturn( m_pRuleNode, pLastResult, "" );
		m_pWorker->SuspendWork( _DBG_STEP_RET, 0, 0, "" );
#endif
	}

	return pTotalResult;
}

RESULT_REC * CMRTInterpreter::Running() {
	RESULT_REC *pTotalResult;
	RESULT_REC *pNewResult;
	RESULT_REC *pLastResult = 0;
	int idx;
	unsigned char resGbn;
	void *pExp;
	m_ResRecSize = sizeof(VALUE_UNION) * m_pRuleNode->retCnt;

	pTRule = (TRULE_DATA*) m_pDataList->getVersionNode(m_pRuleNode, &m_rows);

	if (!pTRule)
		return 0; //  적용버전 없음

	pTRuleCol = pTRule->cond1;

	if (m_rows == 0)
		return (RESULT_REC *) 0;

	BuildCompareValues();

	/****************************************************************************************************/
	//  컬럼이 상수형 하나이고 Matching행이 여럿일때
	if (pTRule->iCnt == 1 && m_pRuleNode->breakFlag == 0
			&& pTRuleCol[0].pType == TR_IN_TP_CONST) {
		return SimpleResult();
	}
	/****************************************************************************************************/
//printf("Sorted = %d\n", pTRule->sorted );
	pTotalResult = 0;
	idx = -1;
	while (1) {
		if (pTRule->iCnt == 0)
			idx++;
		else {
			if (pTRule->sorted) {
				idx = getMatchingIdxFromSorted();
			} else {
				idx = getMatchingIdxFromSequencial(idx + 1);
			}
		}
		if(idx >= m_pRuleNode->pVers->rows) break;

		if (idx < 0)
			break;

#ifdef _TRACE
		if ( m_TraceFlag )
		{
			sprintf(m_TraceMsg, "   R(%d) Matching line %d",
					m_pRuleNode->rule_id, idx+1 );
			m_pDataList->TraceOut( m_TraceMsg );
		}
#endif

#ifdef _TESTER
		m_pWorker->SuspendWork( _DBG_STEP_NEXT, idx+1, 2, "" );
#endif

		pExp = pTRule->result[idx];
		resGbn = pTRule->resGbn[idx];

		if (!pExp) {
			if (pTRule->sorted)
				break;
			else
				continue;
		}

		if (resGbn == 1) { //  Constant Result

			if (!pTotalResult) {
				pTotalResult = pLastResult =(RESULT_REC *) m_pDataList->MRMemAlloc1(sizeof(RESULT_REC));
			} else {
				pLastResult->pNext = (RESULT_REC *) m_pDataList->MRMemAlloc1(sizeof(RESULT_REC));
				pLastResult = pLastResult->pNext;
			}
			pLastResult->pNext = 0;

			pLastResult->value = (VALUE_UNION *) m_pDataList->MRMemAlloc1(m_ResRecSize);
			memcpy((char *) pLastResult->value, (char *) pExp, m_ResRecSize);

#ifdef _TESTER
			m_pDataList->DbgRuleReturn( m_pRuleNode, pLastResult, "" );
			m_pWorker->SuspendWork( _DBG_STEP_RET, 0, 0, "" );
#endif

		} else if (resGbn == 2) { //  1차원 Array

			RESULT_REC *pRecList = (RESULT_REC *) pExp;

#ifdef _TESTER
m_pDataList->DbgRuleReturn(m_pRuleNode, pRecList, "");
m_pWorker->SuspendWork(_DBG_STEP_RET, 0, 0, "");
#endif

while (pRecList) {
	if (!pTotalResult) {
		pTotalResult = pLastResult =(RESULT_REC *)m_pDataList->MRMemAlloc1(sizeof(RESULT_REC));
	}
	else {
		pLastResult->pNext =(RESULT_REC *)m_pDataList->MRMemAlloc1(sizeof(RESULT_REC));
		pLastResult = pLastResult->pNext;
	}
	pLastResult->pNext = 0;

	pLastResult->value = (VALUE_UNION *)m_pDataList->MRMemAlloc1(m_ResRecSize);
	memcpy((char *)pLastResult->value, (char *)pRecList->value,m_ResRecSize);

	pRecList = pRecList->pNext;
}

		}
 else { //  Expression Result

	 CMRCInterpreter CInterpreter(m_pParent, m_pDataList, m_pRuleNode,m_pParm);
	 pNewResult = CInterpreter.OneResult((CRULE_STMT*)pExp);

	 if (pNewResult) {
#ifdef _TESTER
		 m_pDataList->DbgRuleReturn(m_pRuleNode, pNewResult, "");
		 m_pWorker->SuspendWork(_DBG_STEP_RET, 0, 0, "");
#endif

		 if (!pTotalResult)
			 pTotalResult = pNewResult;
		 else
			 pLastResult->pNext = pNewResult;

		 pLastResult = pNewResult;

		 while (pLastResult->pNext)
			 pLastResult = pLastResult->pNext;
	 }
 }
 /*
  if ( pNewResult ) {
  if ( ! pTotalResult ) pTotalResult = pNewResult;
  else pLastResult->pNext = pNewResult;

  pLastResult = pNewResult;

  while ( pLastResult->pNext ) pLastResult = pLastResult->pNext;
  }
  */
 if (m_pRuleNode->breakFlag || m_pDataList->m_bExit)
	 break;

	}

	return pTotalResult;
}

void CMRTInterpreter::BuildCompareValues() {
	Value = (TYPE_VALUE *)m_pDataList->MRMemAlloc1(	sizeof(TYPE_VALUE) * pTRule->iCnt);

#ifdef _TRACE
	if (m_TraceFlag) m_pDataList->TraceOut("   COMPARE VALUE BEGIN");
#endif

	//printf("    iCNT = %d\n", pTRule->iCnt );

	for (int i = 0; i < pTRule->iCnt; i++) {

		//printf("       idx = %d   pType = %d   gbn = %d\n", i, pTRuleCol[i].pType, pTRuleCol[i].gbn );

		if (pTRuleCol[i].pType == TR_IN_TP_COND)
			continue;

		switch (pTRuleCol[i].gbn) {
		case TR_COL_TP_EXTV:
			getItemValue(&pTRuleCol[i], &Value[i]);
			break;
		case TR_COL_TP_INTV:
			#ifdef RULE_RE_RUN
				/*scope=2는 원래 없는 값(임시) 테이블 룰에서 파라미터 사용시 무조건 룰 재실행으로 변경.2017.08.29.중소기업중앙회*/
				if (m_pRuleNode->scope == 0)m_pRuleNode->scope = 2;
			#endif
			getParmValue(&pTRuleCol[i], &Value[i]);
			break;
		case TR_COL_TP_RULE:
		
			getRuleValue(&pTRuleCol[i], &Value[i]); 
			break;
		case TR_COL_TP_GBLV:
			#ifdef RULE_RE_RUN
				/*scope=2는 원래 없는 값(임시) 테이블 룰에서 파라미터 사용시 무조건 룰 재실행으로 변경.2017.08.29.중소기업중앙회*/
				if (m_pRuleNode->scope == 0)m_pRuleNode->scope = 2;
			#endif
			getVarValue(&pTRuleCol[i], &Value[i]);
			break;
		default:
						Value[i].dataType = 0;
			break; //  비교하지 않을 컬럼
		}

//printf( "      CompVal COL %d : %s\n", i, Value[i].value.strVal );

#ifdef _TRACE
		if ( m_TraceFlag )
		{
			switch ( Value[i].dataType ) {
				case DATA_TYPE_DOUBLE :
				sprintf(m_TraceMsg, "      COL %d : %lf", i+1, Value[i].value.dblVal ); break;
				case DATA_TYPE_STRING :
				sprintf(m_TraceMsg, "      COL %d : %s", i+1, Value[i].value.strVal ); break;
				case DATA_TYPE_LONG :
				sprintf(m_TraceMsg, "      COL %d : %ld", i+1, Value[i].value.longVal ); break;
				case DATA_TYPE_LSTRING :
				sprintf(m_TraceMsg, "      COL %d : %s", i+1, Value[i].value.pLStr ); break;
				case DATA_TYPE_BOOL :
				sprintf(m_TraceMsg, "      COL %d : %s", i+1, Value[i].value.longVal ? "TRUE" : "FALSE" ); break;
				default :
				sprintf(m_TraceMsg, "      COL %d : %s", i+1, "???" );
			}

			m_pDataList->TraceOut( m_TraceMsg );
		}
#endif

		/*
		 if ( pTRuleCol[i].gbn == 'I' ) {
		 getItemValue( &pTRuleCol[i], &Value[i] );
		 } else if ( pTRuleCol[i].gbn == 'P' ) {
		 getParmValue( &pTRuleCol[i], &Value[i] );
		 } else {
		 getRuleValue( &pTRuleCol[i], &Value[i] );
		 }
		 */
	}

#ifdef _TRACE
	if ( m_TraceFlag ) m_pDataList->TraceOut( "   COMPARE VALUE END\n" );
#endif

}

int CMRTInterpreter::getIndexValue(int parmId) {
	TYPE_VALUE tVal;

	if (!m_pParm) {
		//  에러처리가 맞을듯...
		return 0;
	}

	if (!m_pParm->getParmValue(parmId, &tVal)) {
		//  에러처리가 맞을듯...
		return 0;
	}

	if (tVal.dataType != DATA_TYPE_LONG) {
		//  에러처리가 맞을듯...
		return 0;
	}

	return tVal.value.longVal; 
}

void CMRTInterpreter::getParmValue(TRULE_COL *pTCol, TYPE_VALUE *pTVal) {
	/*
	 if ( ! m_pParm ) {
	 printf("\n       Error : %dth parm not defined.\n", pTCol->id );
	 throw new CMTException( UERR_PARMNFOUND, EMSG_PARMNFOUND );
	 }
	 */
	if (!m_pParm) {
		pTVal->dataType = 0;
		return;
	}

	if (!m_pParm->getParmValue(pTCol->id, pTVal)) {
		memset((char*) &pTVal->value, 0, sizeof(VALUE_UNION));
	}
}

void CMRTInterpreter::getItemValue(TRULE_COL *pTCol, TYPE_VALUE *pTVal) {
	int col;

	if ((col = m_pDataList->getItemColumn(pTCol->id)) < 0) {
		char msg[128];
		//sprintf(msg, "Item (%d) not found.", pTCol->id);
		sprintf(msg, MSG_NOTFOUND_ITEM, pTCol->id);
		throw new CMTException(UERR_ITEMNOTFOUND, msg);
	}

	pTVal->value.RuleItem.i_col = col;

	if (pTCol->idx == 0)
		pTVal->value.RuleItem.i_row = 0;
	else {
		pTVal->value.RuleItem.i_row = getIndexValue(pTCol->idx);
	}

	m_pDataList->getItemValue(pTVal);
}

void CMRTInterpreter::getRuleValue(TRULE_COL *pTCol, TYPE_VALUE *pTVal) {
	RULE_RESULT *pRuleResult;
	char tempVal[101];

	  if (m_pParm != NULL) {
/*      	printf("getRuleValue pTCol->gbn\t=[%d] \n", pTCol->gbn);
      	printf("getRuleValue pTCol->id\t=[%d]  \n", pTCol->id);
      	printf("getRuleValue pTCol->rSeq\t=[%d]  \n", pTCol->rSeq);
      	printf("getRuleValue pTCol->pType\t=[%d]  \n", pTCol->pType);
      	printf("getRuleValue pTCol->idrange\t=[%d]  \n", pTCol->range);
      	printf("getRuleValue pTCol->vName\t=[%s]  \n", pTCol->vName);
      	printf("getRuleValue pTCol->idx=\t[%d]  \n", pTCol->idx);
      	printf("getRuleValue pTCol->param=\t[%s]  \n", pTCol->param);
*/
        sprintf(tempVal,"%s",pTCol->param);
        if( tempVal==0x00){
    		  CMRRunner Runner(pTCol->id, "", (CMRDataList*)m_pDataList, 0 );
    		  pRuleResult = Runner.getRuleResultPtr();
        }else{
    		  TrimToken(tempVal,'$');
    		  printf("TrimToken  ==> [%s][%s]\n",tempVal, pTCol->param );
    		  int idx = atoi(tempVal);
    
    		  TYPE_VALUE tVal;
    		  m_pParm->getParmValue(idx, &tVal);
    		  char tValTemp[201];
    		  switch (tVal.dataType) {
    		  case DATA_TYPE_LONG:
    			  sprintf(tValTemp, "%ld", tVal.value.longVal);  //lf->ld로변경
    			  break;
    		  case DATA_TYPE_DOUBLE:
    			  sprintf(tValTemp, "%lf", tVal.value.dblVal);  //lf->ld로변경
    			  break;
    		  case DATA_TYPE_STRING:
    			  sprintf(tValTemp, "%s", tVal.value.strVal);  //lf->ld로변경
    			  break;
    		  case DATA_TYPE_LSTRING:
    			  sprintf(tValTemp, "%s", tVal.value.pLStr);  //lf->ld로변경
    			  tValTemp[200] = 0;
    			  break;
    		  }
    		  CMRParam *pNewParm = 0;
    		  TYPE_VALUE ParmVal;
    		  ParmVal.dataType = DATA_TYPE_STRING;
    		  strcpy(ParmVal.value.strVal, tValTemp);
    		  CMRParam NewParm(&ParmVal, m_pParm->m_ParmCnt);
//    		  printf("rule ID [%d] [%s] \n", pTCol->id, tValTemp);
      		  CMRRunner Runner(pTCol->id, tValTemp, (CMRDataList*)m_pDataList, (CMRParam*)&NewParm);
      		  pRuleResult = Runner.getRuleResultPtr();
      }
	 

//	CMRCRParser CRParser;
//	int rc = CRParser.CalculateCondition((char*)pTCol->param);
//	printf(" RC [%d] \ n", rc);
//	pRuleResult = m_pDataList->GetRuleResult(pTCol->id, 0);

/*
	if ((pStmt = Parsing.ParseExpression(pTCol->param)) == (CRULE_STMT *)-1) {
		printf("MRTRINTR = [%s]\n", EMSG_PARSE_01);
		return ;
	}
//	CMRExpression CCM;
//	CMRExpression Expression(NULL, m_pDataList, m_pRuleNode, pNewParm);
	TYPE_VALUE ParmVal;

	switch (pTCol->pType) {

		}
	printf("val= [%s]  \n", pTCol->param );
	ParmVal.dataType = DATA_TYPE_STRING;
	strcpy(ParmVal.value.strVal, pTCol->param );
	CMRParam NewParm(&ParmVal, 1);
	CMRRunner Runner( pTCol->id, pTCol->param,(CMRDataList*)m_pDataList, (CMRParam*)&NewParm);
	pRuleResult = Runner.getRuleResultPtr();
*/

/*
		CMRRunner::CMRRunner(int ruleId, char *Parm, CMRDataList *pClass,
		CMRParam *pParmInfo, unsigned char saveFlag) {
pParmVal = Expression.getRuleParmValues(pStmt, sKey, &parmCnt);

	CMRRunner Runner(pTCol->id, sKey, m_pDataList, pNewParm);
	CMRRunner Runner(pTCol->id, (char *) "", m_pDataList, 0);

 

	TYPE_VALUE *pParmVal = 0;
	int			parmCnt;
	char		sKey[200];
	pParmVal = readRuleParmValues(&parmCnt, sKey);
	pRuleResult = m_pDataList->GetRuleResult(pTCol->id,pTCol->param);
	*/
  	
  	
  	
  }else{
	pRuleResult = m_pDataList->GetRuleResult(pTCol->id, 0);
   }  


	if (!pRuleResult) {
		CMRRunner Runner(pTCol->id, (char *) "", m_pDataList, 0);

		pRuleResult = Runner.getRuleResultPtr();
	}

//for ( int x=0; x< pRuleResult->pRuleNode->retCnt; x++ ) printf("%d : %d\n", x, pRuleResult->pRuleNode->retType[x] );

	pTVal->value.RuleItem.ptr = pRuleResult;

	if (pTCol->rSeq > 0)
		pTVal->value.RuleItem.i_col = (unsigned char) pTCol->rSeq;
	else
		pTVal->value.RuleItem.i_col = 1;

	if (pTCol->idx == 0)
		pTVal->value.RuleItem.i_row = 0;
	else {
		pTVal->value.RuleItem.i_row = getIndexValue(pTCol->idx);
	}

	m_pDataList->GetRuleValue(pTVal);
}

void CMRTInterpreter::getVarValue(TRULE_COL *pTCol, TYPE_VALUE *pTVal) {
	int mode;

	if (pTCol->rSeq == DATA_TYPE_DOUBLE || pTCol->rSeq == DATA_TYPE_LONG)
		mode = TOKENFUNC_GETNVAL;
	else
		mode = TOKENFUNC_GETSVAL;

	m_pDataList->getGblValByName(pTVal, pTCol->vName, mode, 0);

}

int CMRTInterpreter::getMatchingIdxFromSorted() {
	int nFirst, nLast, n, i, cmp;
	VALUE_UNION *p;

	nFirst = 0;
	nLast = m_rows - 1;

	while (nFirst <= nLast) {
		n = (nFirst + nLast) / 2;
		cmp = 0;

		for (i = 0; i < pTRule->iCnt; i++) {
			if (pTRuleCol[i].gbn == 0)
				continue; // 해당컬럼의 모든 Row에 값이 없을때

			p = (VALUE_UNION *) pTRuleCol[i].vMin;

			if (pTRuleCol[i].pType == TR_IN_TP_RANGE) {

				if (*(p[n].strVal)) {
					cmp = CompareValue(&Value[i], &p[n]);

					if (cmp == 0 && pTRuleCol[i].range == 1)
						cmp = -1; //  최소제외일때

					if (cmp < 0) {
						nLast = n - 1;
						break;
					}
				}

				p = (VALUE_UNION *) pTRuleCol[i].vMax;

				if (*(p[n].strVal)) {
					cmp = CompareValue(&Value[i], &p[n]);

					if (cmp == 0 && pTRuleCol[i].range == 2)
						cmp = 1; //  최대제외일때

					if (cmp > 0) {
						nFirst = n + 1;
						break;
					}
				}

				cmp = 0;
			} else {

				if (!*(p[n].strVal)) {
					cmp = 0;
					continue;
				}

				if ((cmp = CompareValue(&Value[i], &p[n])) == 0)
					continue;

				if (cmp < 0)
					nLast = n - 1;
				else
					nFirst = n + 1;

				break;
			}
		}

		if (cmp == 0)
			return n;

	}

	return -1;
}

int CMRTInterpreter::getMatchingIdxFromSequencial(int st) {
	int n, i, cmp;
	VALUE_UNION *p;
	CRULE_STMT **q;
	TRULE_OPTR **r;

	TYPE_VALUE *expResult;


	for (n = st; n < m_rows; n++) {

#ifdef _TESTER
		m_pWorker->SuspendWork( _DBG_STEP_NEXT, n+1, 1, "" );
#endif

		cmp = 0;
		for (i = 0; i < pTRule->iCnt; i++) {
#ifdef _DEBUG
//			printf("CMRTInterpreter::getMatchingIdxFromSequencial : st = %d  row = %d i=[%d]\n", st, m_rows,i );
#endif
			//printf("   n = %d    i = %d\n", n, i );

			if (pTRuleCol[i].gbn == 0)
				continue; // 해당컬럼의 모든 Row에 값이 없을때

			switch (pTRuleCol[i].pType) {
			case TR_IN_TP_RONLY: // 2010-3-23 vapor 스킵되는 문제 처리
			case TR_IN_TP_CONST:
				p = (VALUE_UNION *) pTRuleCol[i].vMin;

				if (*(p[n].strVal)) {
					if ((cmp = CompareValue(&Value[i], &p[n])) != 0)
						break;
				} else
					cmp = 0;
				break;

			case TR_IN_TP_RANGE:

				p = (VALUE_UNION *) pTRuleCol[i].vMin;

				if (*(p[n].strVal)) {
					cmp = CompareValue(&Value[i], &p[n]);

					if (cmp == 0 && pTRuleCol[i].range == 1)
						cmp = -1; //  최소제외일때

					if (cmp < 0)
						break;
				}

				p = (VALUE_UNION *) pTRuleCol[i].vMax;

				if (*(p[n].strVal)) {
					cmp = CompareValue(&Value[i], &p[n]);

					if (cmp == 0 && pTRuleCol[i].range == 2)
						cmp = 1; //  최대제외일때

					if (cmp > 0)
						break;
				}

				cmp = 0;
				break;

			case TR_IN_TP_OPTR:

				r = (TRULE_OPTR **) pTRuleCol[i].vMin;

				if (!r) {
					cmp = 0;
					break;
				}

				if (!r[n]) {
					cmp = 0;
					break;
				}

				//cmp = CalcOperator(r[n], i);//2022-06-01  c
				cmp= CalcOperatorRule(pTRuleCol, r[n], i);
				break;

			case TR_IN_TP_COND:
				q = (CRULE_STMT **) pTRuleCol[i].vMin;
 
 
				//syk 조건이 없을 때 에러 방지 2014.1.16
				if (!q || q[n]==0) {
					cmp = 0;
					break;
				}

				{
					CMRExpression Expression(0, m_pDataList, m_pRuleNode,
							m_pParm);
//	printf("CMRTInterpreter::getMatchingIdxFromSequencial] n = %d  i = %d  p(%d)\n", n, i, (int) q[n] );
					expResult = Expression.CalcExpression(q[n]);
//	printf("CMRTInterpreter::getMatchingIdxFromSequencial] afr Calc\n");

					if (expResult->dataType == TOKENTYPE_RULE)
						m_pDataList->GetRuleValue(expResult);
					else if (expResult->dataType == TOKENTYPE_GLOBAL)
						m_pDataList->getItemValue(expResult);
				}

				if (expResult->dataType == DATA_TYPE_BOOL)
					cmp = expResult->value.boolVal == 0;
				else
					cmp = expResult->value.longVal == 0;

				break;

			default:
				cmp = 0;
			}

			if (cmp != 0)
				break;

		}

		if (cmp == 0)
			return n;
	}

	return -1;
}

int CMRTInterpreter::CalcOperator(TRULE_OPTR *p, int idx) {
	VALUE_UNION v1, v2;
//	CMRCRParser CRParser;
	char cmpVal[101];
	int i,rc;
	double tCValue;
	//2022-05-24
	 switch(p->optr ){
	 	case 9: case 10: case 11 : case 14: case 15: case 17:
				switch (Value[idx].dataType) {
				case DATA_TYPE_STRING:	strcpy(cmpVal, Value[idx].value.strVal);			break;
				case DATA_TYPE_DOUBLE:	sprintf(cmpVal, "%lf", Value[idx].value.dblVal);	break;
				case DATA_TYPE_LONG:	sprintf(cmpVal, "%ld", Value[idx].value.longVal);	break;
				case DATA_TYPE_LSTRING:	strcpy(cmpVal, Value[idx].value.pLStr);				break;
				default:	cmpVal[0] = 0;
				}
				break;
		case 16://FUCNTION
			switch (Value[idx].dataType) {
			case DATA_TYPE_STRING:	strcpy(cmpVal, Value[idx].value.strVal);			break;
			case DATA_TYPE_DOUBLE:	sprintf(cmpVal, "%lf", Value[idx].value.dblVal);	break;
			case DATA_TYPE_LONG:	sprintf(cmpVal, "%ld", Value[idx].value.longVal);	break;
			case DATA_TYPE_LSTRING:	strcpy(cmpVal, Value[idx].value.pLStr);				break;
			default: strcpy(cmpVal, Value[idx].value.strVal);			break;
			}
			break;
		default:strcpy(v1.strVal, p->sVal[0]);	
			break;
	 }
 	 switch ( p->optr ){
		  case 7: case 8: case 12: case 13:	strcpy(v2.strVal, p->sVal[1]);	break;
	 }


/*	
	if (p->optr < 9) {
		strcpy(v1.strVal, p->sVal[0]);
//		ConvertData( Value[idx].dataType, &v1, p->sVal[0] );
		
	 
		if (p->optr > 6) {
			strcpy(v2.strVal, p->sVal[1]);
//			ConvertData( Value[idx].dataType, &v2, p->sVal[1] );
		} 
	} else {
		switch (Value[idx].dataType) {
		case DATA_TYPE_STRING:
			strcpy(cmpVal, Value[idx].value.strVal);
			break;
		case DATA_TYPE_DOUBLE:
			sprintf(cmpVal, "%lf", Value[idx].value.dblVal);
			break;
		case DATA_TYPE_LONG:
			sprintf(cmpVal, "%ld", Value[idx].value.longVal);
			break;
		case DATA_TYPE_LSTRING:
			strcpy(cmpVal, Value[idx].value.pLStr);
			break;
		default:
			cmpVal[0] = 0;
		}
	}
*/
	switch (p->optr) {
	case 1: //  Equal(=)
		if (CompareValue(&Value[idx], &v1) == 0)
			return 0;
		break;
	case 2: //  Not equal(<>)
		if (CompareValue(&Value[idx], &v1) != 0)
			return 0;
		break;
	case 3: //  Less than(<)
		if (CompareValue(&Value[idx], &v1) < 0)
			return 0;
		break;
	case 4: //  Less equal(≤)
		if (CompareValue(&Value[idx], &v1) <= 0)
			return 0;
		break;
	case 5: //  Greater than(>)
		if (CompareValue(&Value[idx], &v1) > 0)
			return 0;
		break;
	case 6: //  Greater equal(≥)
		if (CompareValue(&Value[idx], &v1) >= 0)
			return 0;
		break;

	case 7: //  Between(≤≤)
		if (CompareValue(&Value[idx], &v1) < 0)
			break;
		if (CompareValue(&Value[idx], &v2) > 0)
			break;
		return 0;

	case 8: //  Between(≤<)
		if (CompareValue(&Value[idx], &v1) < 0)
			break;
		if (CompareValue(&Value[idx], &v2) >= 0)
			break;
		return 0;
	case 9: //  IN
		for (i = 0; i < p->cnt; i++) {
				switch (Value[idx].dataType) {
				case DATA_TYPE_DOUBLE:	
					tCValue = atof( p->sVal[i] );
					if (tCValue==Value[idx].value.dblVal) return 0;
					break;
				default :
					if (strcmp(cmpVal, p->sVal[i]) == 0)
					return 0;
				}
		}
		return 1;
	case 10: //  NIN
		for (i = 0; i < p->cnt; i++) {
				switch (Value[idx].dataType) {
				case DATA_TYPE_DOUBLE:	
					tCValue = atof( p->sVal[i] );
					if (tCValue==Value[idx].value.dblVal) return 1;
					break;
				default :
					if (strcmp(cmpVal, p->sVal[i]) == 0)
					return 1;
				}
		}
		return 0;
	case 11: //  LIKE
		if (Value[idx].dataType != DATA_TYPE_STRING) {
			char msg[128];
			//sprintf(msg, "Compare value type error.");
			sprintf(msg, MSG_COMP_VALTYPE_ERR);
			throw new CMTException(USERERR_GRAMMAR_121, msg);
		}
		for (i = 0; i < p->cnt; i++) {
			if (Calc_Match(Value[idx].value.strVal, p->sVal[i]))
				return 0;
		}
		break;
	case 17: // NOT  LIKE
		if (Value[idx].dataType != DATA_TYPE_STRING) {
			char msg[128];
			//sprintf(msg, "Compare value type error.");
			sprintf(msg, MSG_COMP_VALTYPE_ERR);
			throw new CMTException(USERERR_GRAMMAR_121, msg);
		}
		for (i = 0; i < p->cnt; i++) {
			if (!Calc_Match(Value[idx].value.strVal, p->sVal[i]))
				return 0;
		}
		break;
	case 12: //    10 <  x <= 20
		if (CompareValue(&Value[idx], &v1) != 1)  // 11 < 10 
			break;
		if ( CompareValue(&Value[idx], &v2) == 1  ) //  -1,0 == true
			break;
		return 0;
	case 13:   
		if (CompareValue(&Value[idx], &v1) != 1)
			break;
		if ( !(CompareValue(&Value[idx], &v2) ==-1) )
			break;
		return 0;
	case 14: // Exist(Data존재)  0이면 null,
//		printf("14 cmpVal =[%s]  =========================================>\n", cmpVal);
//		printf("14 cmpVal[0]!=0  					[%d]\n", (cmpVal[0]!=0 ));
//		printf("14 strcmp(cmpVal, 0)  		[%d]\n", strcmp(cmpVal, "0")  );
//		printf("14 strcmp(cmpVal, null)  	[%d]\n", strcmp(cmpVal, "null")  );
		if ( cmpVal[0]!=0 ){ //데이타  존재 하면
			//strcmp 데이가 같으면 0을 반환
			if ( strcmp(cmpVal, "0")==0 ){ // 0 이면 데이타가 없으므로  false
				break;
			}else if (strcmp(cmpVal, "0.000000")==0) { //0.000000 이면 데이타가 없으므로  false
				break;
			}else if (strcmp(cmpVal, "null")==0 ){ //혹시 null 로 들어오면 데이타가 없으므로 false
				break;
			}else{ //나머지 경우는 데이타가 있다로 판단함.
				return 0;
			}
		}
	 	break;
	case 15: // is Null
//		printf("15 cmpVal =[%s] =========================================>\n", 			cmpVal );
//		printf("15 (cmpVal[0]==0)  				[%d]\n", (cmpVal[0]==0) );
//		printf("15 strcmp(cmpVal, null)  	[%d]\n", strcmp(cmpVal, "null")  );
//		printf("15 strcmp(cmpVal, 0)  		[%d]\n", strcmp(cmpVal, "0")   );
		if (cmpVal[0] == 0)	return 0;  
		if (strcmp(cmpVal, "null")==0) return 0;   
		if (strcmp(cmpVal, "0")==0   ) return 0;
		if (strcmp(cmpVal, "0.000000")==0	)return 0;
	 	break;
	case 16: //Function  
		printf("16 cmpVal =[%s]   p->sVal[0]= %s ==> strcmp [%d]\n", cmpVal, p->sVal[0], strcmp(cmpVal, p->sVal[0]));
		 
		break;
//	default:
//		break;
	}

	return 1; //  만족하지 않음
}

 
int CMRTInterpreter::CalcOperatorRule(TRULE_COL *pTRuleCol,  TRULE_OPTR *p, int idx){
	VALUE_UNION v1, v2;
	//	CMRCRParser CRParser;
	char cmpVal[101];
	int i, retValue=1;
		double tCValue;
	//2022-05-24
	switch (p->optr) {
	case 9: case 10: 
		case 11: //LIKE
		case 14: case 15: 
		case 17:  //NOT LIKE
		switch (Value[idx].dataType) {
		case DATA_TYPE_STRING:	strcpy(cmpVal, Value[idx].value.strVal);			break;
		case DATA_TYPE_DOUBLE:	sprintf(cmpVal, "%lf", Value[idx].value.dblVal);	break;
		case DATA_TYPE_LONG:	sprintf(cmpVal, "%ld", Value[idx].value.longVal);	break;
		case DATA_TYPE_LSTRING:	strcpy(cmpVal, Value[idx].value.pLStr);				break;
		default:	cmpVal[0] = 0;
		}
//		printf(" p->optr [%d] ,  cmpVal =[%s] \n", p->optr, cmpVal);
		break;
	case 16://FUCNTION
		switch (Value[idx].dataType) {
		case DATA_TYPE_STRING:	strcpy(cmpVal, Value[idx].value.strVal);			break;
		case DATA_TYPE_DOUBLE:	sprintf(cmpVal, "%lf", Value[idx].value.dblVal);	break;
		case DATA_TYPE_LONG:	sprintf(cmpVal, "%ld", Value[idx].value.longVal);	break;
		case DATA_TYPE_LSTRING:	strcpy(cmpVal, Value[idx].value.pLStr);				break;
		default: strcpy(cmpVal, Value[idx].value.strVal);			break;
		}
		break;
	default:strcpy(v1.strVal, p->sVal[0]);
		break;
	}
	switch (p->optr) {
	case 7: case 8: case 12: case 13:	strcpy(v2.strVal, p->sVal[1]);	break;
	}
	/*

	TYPE_VALUE *ValueX = (TYPE_VALUE *)m_pDataList->MRMemAlloc1(sizeof(TYPE_VALUE) * pTRule->iCnt);
	TRULE_COL *pTCol = &pTRuleCol[idx];
	printf("IDX[%d] ,TYPE[%d] pTCol->gbn\t=[%d] \n",idx, p->optr,pTCol->gbn);
	printf("IDX[%d] ,TYPE[%d] pTCol->id\t=[%d]  \n", idx, p->optr,pTCol->id);
	printf("IDX[%d] ,TYPE[%d] pTCol->rSeq\t=[%d]  \n", idx, p->optr, pTCol->rSeq);
	printf("IDX[%d] ,TYPE[%d] pTCol->pType\t=[%d]  \n", idx, p->optr, pTCol->pType);
	printf("IDX[%d] ,TYPE[%d] pTCol->idrange\t=[%d]  \n", idx, p->optr, pTCol->range);
	printf("IDX[%d] ,TYPE[%d] pTCol->vName\t=[%s]  \n", idx, p->optr, pTCol->vName);
	printf("IDX[%d] ,TYPE[%d] pTCol->idx=\t[%d]  \n", idx, p->optr, pTCol->idx);

 
	if (p->optr < 9) {
	strcpy(v1.strVal, p->sVal[0]);
	//		ConvertData( Value[idx].dataType, &v1, p->sVal[0] );


	if (p->optr > 6) {
	strcpy(v2.strVal, p->sVal[1]);
	//			ConvertData( Value[idx].dataType, &v2, p->sVal[1] );
	}
	} else {
	switch (Value[idx].dataType) {
	case DATA_TYPE_STRING:
	strcpy(cmpVal, Value[idx].value.strVal);
	break;
	case DATA_TYPE_DOUBLE:
	sprintf(cmpVal, "%lf", Value[idx].value.dblVal);
	break;
	case DATA_TYPE_LONG:
	sprintf(cmpVal, "%ld", Value[idx].value.longVal);
	break;
	case DATA_TYPE_LSTRING:
	strcpy(cmpVal, Value[idx].value.pLStr);
	break;
	default:
	cmpVal[0] = 0;
	}
	}
	*/
	switch (p->optr) {
	case 1: //  Equal(=)
		if (CompareValue(&Value[idx], &v1) == 0)
			return 0;
		break;
	case 2: //  Not equal(<>)
		if (CompareValue(&Value[idx], &v1) != 0)
			return 0;
		break;
	case 3: //  Less than(<)
		if (CompareValue(&Value[idx], &v1) < 0)
			return 0;
		break;
	case 4: //  Less equal(≤)
		if (CompareValue(&Value[idx], &v1) <= 0)
			return 0;
		break;
	case 5: //  Greater than(>)
		if (CompareValue(&Value[idx], &v1) > 0)
			return 0;
		break;
	case 6: //  Greater equal(≥)
		if (CompareValue(&Value[idx], &v1) >= 0)
			return 0;
		break;

	case 7: //  Between(≤≤)
		if (CompareValue(&Value[idx], &v1) < 0)
			break;
		if (CompareValue(&Value[idx], &v2) > 0)
			break;
		return 0;

	case 8: //  Between(≤<)
		if (CompareValue(&Value[idx], &v1) < 0)
			break;
		if (CompareValue(&Value[idx], &v2) >= 0)
			break;
		return 0;
	case 9: //  IN
		for (i = 0; i < p->cnt; i++) {
				switch (Value[idx].dataType) {
				case DATA_TYPE_DOUBLE:	
					tCValue = atof( p->sVal[i] );
					if (tCValue==Value[idx].value.dblVal) return 0;
					break;
				default :
					if (strcmp(cmpVal, p->sVal[i]) == 0)
					return 0;
				}
		}
		return 1;
	case 10: //  NIN
		for (i = 0; i < p->cnt; i++) {
				switch (Value[idx].dataType) {
				case DATA_TYPE_DOUBLE:	
					tCValue = atof( p->sVal[i] );
					if (tCValue==Value[idx].value.dblVal) return 1;
					break;
				default :
					if (strcmp(cmpVal, p->sVal[i]) == 0)
					return 1;
				}
		}
		return 0;

	case 11: //  LIKE
		if (Value[idx].dataType != DATA_TYPE_STRING) {
			char msg[128];
			//sprintf(msg, "Compare value type error.");
			sprintf(msg, MSG_COMP_VALTYPE_ERR);
			throw new CMTException(USERERR_GRAMMAR_121, msg);
		}
		for (i = 0; i < p->cnt; i++) {
			if (Calc_Match(Value[idx].value.strVal, p->sVal[i]))
				return 0;
		}
		break;
	case 17: // NOT LIKE
		if (Value[idx].dataType != DATA_TYPE_STRING) {
			char msg[128];
			//sprintf(msg, "Compare value type error.");
			sprintf(msg, MSG_COMP_VALTYPE_ERR);
			throw new CMTException(USERERR_GRAMMAR_121, msg);
		}
		for (i = 0; i < p->cnt; i++) {
			if (!Calc_Match(Value[idx].value.strVal, p->sVal[i]))
				return 0;
		}
		break;
	case 12: //    10 <  x <= 20
		if (CompareValue(&Value[idx], &v1) != 1)  // 11 < 10 
			break;
		if (CompareValue(&Value[idx], &v2) == 1) //  -1,0 == true
			break;
		return 0;
	case 13:
		if (CompareValue(&Value[idx], &v1) != 1)
			break;
		if (!(CompareValue(&Value[idx], &v2) == -1))
			break;
		return 0;
	case 14: // Exist(Data존재)  0이면 null,
			 //		printf("14 cmpVal =[%s]  =========================================>\n", cmpVal);
			 //		printf("14 cmpVal[0]!=0  					[%d]\n", (cmpVal[0]!=0 ));
			 //		printf("14 strcmp(cmpVal, 0)  		[%d]\n", strcmp(cmpVal, "0")  );
			 //		printf("14 strcmp(cmpVal, null)  	[%d]\n", strcmp(cmpVal, "null")  );
		if (cmpVal[0] != 0) { //데이타  존재 하면
							  //strcmp 데이가 같으면 0을 반환
			if (strcmp(cmpVal, "0") == 0) { // 0 이면 데이타가 없으므로  false
				break;
			}
			else if (strcmp(cmpVal, "0.000000") == 0) { //0.000000 이면 데이타가 없으므로  false
				break;
			}
			else if (strcmp(cmpVal, "null") == 0) { //혹시 null 로 들어오면 데이타가 없으므로 false
				break;
			}
			else { //나머지 경우는 데이타가 있다로 판단함.
				return 0;
			}
		}
		break;
	case 15: // is Null
			 //		printf("15 cmpVal =[%s] =========================================>\n", 			cmpVal );
			 //		printf("15 (cmpVal[0]==0)  				[%d]\n", (cmpVal[0]==0) );
			 //		printf("15 strcmp(cmpVal, null)  	[%d]\n", strcmp(cmpVal, "null")  );
			 //		printf("15 strcmp(cmpVal, 0)  		[%d]\n", strcmp(cmpVal, "0")   );
		if (cmpVal[0] == 0)	return 0;
		if (strcmp(cmpVal, "null") == 0) return 0;
		if (strcmp(cmpVal, "0") == 0) return 0;
		if (strcmp(cmpVal, "0.000000") == 0)return 0;
		break;
	case 16: //Function
//		printf("16 001 cmpVal [%s]\n", cmpVal);
//		printf("16 002 cmpVal [%s]\n", p->sVal[0]);
//		printf("16 cmpVal [%s]\n", p->sVal[1]);
		/*
		printf("DATA_TYPE_LONG (00)=  null \n ");
		CMRParam *pParm = m_pParm;
		printf("DATA_TYPE_LONG (01)=  null \n ");
		TYPE_VALUE *valTemp;
		printf("DATA_TYPE_LONG (02)=  null \n ");
		pParm->getParmValue(0, valTemp);
		printf("DATA_TYPE_LONG (03)=  null \n ");

		switch (valTemp->dataType) {
		case DATA_TYPE_LSTRING:
		case DATA_TYPE_STRING:
			printf("TYPE_VALUE=[%s] \n", valTemp->value.strVal);
			break;
		case DATA_TYPE_BOOL:
			printf("TYPE_VALUE=[%d] \n", valTemp->value.boolVal);
			break;
		case DATA_TYPE_DOUBLE:
			printf("DATA_TYPE_DOUBLE=[%f] \n", valTemp->value.dblVal);
			break;
		case DATA_TYPE_LONG:
			printf("DATA_TYPE_LONG=[%d] \n", valTemp->value.longVal);
			break;
		default :
			printf("DATA_TYPE_LONG=  null \n " );
			break;
		}
		*/
		CRULE_STMT *pStmt;
		TYPE_VALUE *expResult;
		char queryTemp[3001];
		char queryTemp2[3001];
		char queryTemp3[3001];
		char queryTemp4[3001];
		char ruleTemp[3001];
		sprintf(queryTemp, "%s", p->sVal[0]);
		CMRParser Parsing;
		
		int idx1, idx2;
		idx1 = index(p->sVal[0], ':', 0);
		int breakIndex=0;
		if (idx1 >= 0) {
			idx2 = index(p->sVal[0], ':', idx1 + 1);
		}
		if (idx1 >= 0 && idx2 >= 0) {
			getSubstr(p->sVal[0], queryTemp, idx1, idx2);
		}
		strcpy(queryTemp4,queryTemp);
		TrimToken(queryTemp4,':');
		TrimToken(queryTemp4,'$');
		breakIndex = atoi(queryTemp4);
printf("000-queryTemp [%s] \n", queryTemp);
printf("000-cmpVal [%s] \n", cmpVal);
printf("000-queryTemp breakIndex [%s] \n", queryTemp4);

    char *ptr=strtok(queryTemp4,",");
    int tokCnt=0;
    char indexStr[1001];
    while( ptr !=NULL ){
              printf("%s \n", ptr);
              sprintf(indexStr,"%s",ptr);
              rltrim(indexStr);
              printf("indexStr %d \n", indexStr );
              breakIndex = atoi(indexStr);
              printf("breakIndex %d \n", breakIndex );
              ptr = strtok(NULL,",");

              for (int i = 0; i < m_pParm->m_ParmCnt; i++) {
              			TYPE_VALUE tVal;
              			m_pParm->getParmValue(i + 1, &tVal);
              			if( i+1 > breakIndex) break;
              			switch (tVal.dataType) {
              			case DATA_TYPE_BOOL:
              			case DATA_TYPE_LONG:
              				sprintf( queryTemp2,"%ld", tVal.value.longVal);  //lf->ld로변경
              				if (strcmp(cmpVal, queryTemp2) == 0) {
              					sprintf(queryTemp2, "%s", cmpVal);
              					break;
              				}
              				break;
              			case DATA_TYPE_DOUBLE:
              				sprintf(queryTemp2, "%lf", tVal.value.dblVal);  //lf->ld로변경
              				if (strcmp(cmpVal, queryTemp2) == 0) {
              					sprintf(queryTemp2, "%s", cmpVal);
              					break;
              				}
              				break;
              			case DATA_TYPE_STRING:
              				sprintf(queryTemp2, "%s",tVal.value.strVal);  //lf->ld로변경
              				if (strcmp(cmpVal, queryTemp2) == 0) {
              					sprintf(queryTemp2, "\"%s\"", cmpVal);
              					break;
              				}
              				break;
              			case DATA_TYPE_LSTRING:
              				sprintf(queryTemp2, "%s", tVal.value.pLStr);  //lf->ld로변경
              				if (strcmp(cmpVal, queryTemp2) == 0) {
              					sprintf(queryTemp2, "\"%s\"", cmpVal);
              					break;
              				}
              			}
             }              
             if(tokCnt==0){
               sprintf(queryTemp3,"%s",queryTemp2);
             }else{
               sprintf(queryTemp3,"%s,%s",queryTemp3,queryTemp2);
             }
	     printf("\t tokCnt[%d] q2 [%s] \n",tokCnt,queryTemp2); // -1447458960J
	     printf("\t tokCnt[%d] q3 (%s) \n",tokCnt,queryTemp3); // -1447458960J
             tokCnt++;
    }
		
		strcpy(ruleTemp,p->sVal[0]);
printf("queryTemp3 [%s] \n",queryTemp3); // -1447458960J
sprintf(queryTemp4,":%s:",queryTemp3);
sprintf(queryTemp3,"%s",queryTemp4);
printf("queryTemp3 [%s] \n",queryTemp3); // -1447458960J
printf("ruleTemp [%s] \n",ruleTemp); //  {1542796:$10$:}
printf("queryTemp [%s] \n",queryTemp);   //  :$10$: 
printf("queryTemp3 [%s] \n",queryTemp3); // -1447458960J

		StringChange(ruleTemp, queryTemp, queryTemp3);

printf("ruleTemp [%s] \n",ruleTemp);


//printf("CalcOperatorRule 50 \n");
		if ((pStmt = Parsing.ParseExpression(ruleTemp)) == (CRULE_STMT *)-1) {
//printf("CalcOperatorRule 50-1 \n");
			return -1;
		}

//printf("CalcOperatorRule 51 \n");
		CMRExpression Expression(0, m_pDataList, 0, 0);

//printf("CalcOperatorRule 52 \n");
		expResult = Expression.CalcExpression(pStmt);
//printf("CalcOperatorRule 53 \n");

		if (expResult->dataType == TOKENTYPE_RULE) {
//printf("CalcOperatorRule 54 \n");
			RULE_RESULT *pResult;
			int col;
			pResult = (RULE_RESULT *)expResult->value.RuleItem.ptr;
			col = expResult->value.RuleItem.i_col;
			if (col > 0)col--;
			int expType = pResult->pRuleNode->retType[0];
//			printf(" pResult->pRuleNode->retType[%d] =[%d] \n", 0, pResult->pRuleNode->retType[0] );
//			printf(" rule_id  = [%d]  \n", pResult->ruleId );
//			printf(" rule_id  = [%d]  \n", pResult->pRuleNode->rule_id);
//			printf(" ruleName = [%s]  \n", pResult->pRuleNode->ruleName);
			
//printf("CalcOperatorRule 55 \n");
			if (pResult->resRecord != NULL) {
				// 0 이면 만족
				VALUE_UNION *val = pResult->resRecord->value;
				switch (pResult->pRuleNode->retType[0]) {
				case DATA_TYPE_BOOL:
					retValue = (val->boolVal == 1 ? 0 : 1);
//					printf(" expResult->value.boolVal [%d] retValue [%d] \n", val->boolVal, retValue);
					return retValue;
				case DATA_TYPE_DOUBLE:
					retValue = val->dblVal == 1 ? 0 : 1;
//					printf(" expResult->value.dblVal [%d] retValue  [%d] \n", val->dblVal, retValue );
					return retValue;
				case DATA_TYPE_LONG:
					retValue = val->longVal == 1 ? 0 : 1;
//					printf(" expResult->value.longVal [%d] retValue[%d] \n", val->longVal, retValue );
					return retValue;
				default: return 1;
				}
			}
			else {
				return 1;
			}
		}
		/*
		printf("16 004  \n");
		CMRExpression Expression(0, m_pDataList, 0, 0);
		printf("16 005  \n");
		expResult = Expression.CalcExpression(pStmt);
		printf("16 006  \n");

		// TOKENTYPE_RULE  ==10 
		int retValue = 1;
		if ( expResult!=NULL) {
			printf("16 006-001  expResult->dataType[%d] \n", expResult->dataType );
			switch (expResult->dataType) {
			case DATA_TYPE_LONG:
				retValue = expResult->value.longVal == 0 ? 0 : 1;
				printf("16 007  retValue[%ld]\n", expResult->value.longVal);
				break;
			case DATA_TYPE_DOUBLE:
				retValue = expResult->value.dblVal == 0 ? 0 : 1;
				printf("16 008  retValue[%d]\n", expResult->value.dblVal);
				break;
			case DATA_TYPE_STRING: 
				printf("16 009  retValue[%s]\n", expResult->value.strVal);
				retValue = 1;
				break;
			case DATA_TYPE_LSTRING:
				printf("16 010  retValue[%s]\n", expResult->value.pLStr);
				retValue = 1;
				break;
			case DATA_TYPE_BOOL:
				retValue = expResult->value.boolVal == 0 ? 1 : 0;  //0이 만족
				printf("16 011  retValue[%s]\n", expResult->value.boolVal);
				break;
			default:
				break;
			}
			printf("16 012  expResult->dataType[%d] , retValue[%d]\n", expResult->dataType ,retValue);
			return retValue;
		}


		TYPE_VALUE *valTemp = 0;
		TYPE_VALUE *pParmVal = 0;
		printf("CMRQInterpreter  02 m_pParm \n");
		printf("CMRQInterpreter  02 m_pParmCount [%d] \n",  );
		int			parmCnt;
		char		sKey[200];//2017.09.28일 전까지 200 ,산업은행 E-상시감사 10000 인경우 Down 발생

		pParmVal = getRuleParmValues(p->sVal[0], sKey, &parmCnt);

		printf("16  001 \n");
//		TYPE_VALUE *ValueX = (TYPE_VALUE *)m_pDataList->MRMemAlloc1(sizeof(TYPE_VALUE) * pTRule->iCnt);
		TRULE_COL *pTCol = &pTRuleCol[idx];
		printf("16  002 \n");
		if (p->sVal[0] == NULL)return 1;
		printf("16  003 \n");
		int ruleId = getRuleCd(p->sVal[0]);
//		if (ruleId < 0) return 1;
		RULE_LIST *m_pRuleNodeTemp = pCMRGblVars->getRuleNode(ruleId);
//		RULE_LIST *m_pRuleNodeTemp = pCMRGblVars->getRuleNode(p->sVal[0]);
		printf("16  004 \n");
		if (m_pParm == NULL) {
			printf("16  004-01 m_pParm=NULL \n");
		}else{
			printf("16  004-01 m_pParm=NOT NULL \n");
		}
		CMRQInterpreter QInterpreter(m_pDataList, m_pRuleNodeTemp, m_pParm );
		printf("16  005 \n");
		//		CMRQInterpreter QInterpreter(m_pDataList, m_pRuleNodeTemp, m_pParm);

		RESULT_REC *p = (RESULT_REC *)QInterpreter.BuildResult();
		printf("16  006 \n");
		int retValue=1;
		printf("16  007 \n");
		if(p) {
			switch (m_pRuleNodeTemp->retType[0]) {
			case DATA_TYPE_LONG:
				retValue =((VALUE_UNION*)p->value)->longVal == 0 ? 0 : 1;
				break;
			case DATA_TYPE_DOUBLE:
				retValue =((VALUE_UNION*)p->value)->dblVal == 0 ? 0 : 1;
				break;
			case DATA_TYPE_STRING: case DATA_TYPE_LSTRING:
				retValue = 1;
				break;
			case DATA_TYPE_BOOL:
				retValue =((VALUE_UNION*)p->value)->boolVal==0 ? 0: 1;
				break;
			default:
				break;
			}
		}
		return retValue;
		break;*/
	}

	return 1; //  만족하지 않음
}
void CMRTInterpreter::ConvertData(unsigned char dt, VALUE_UNION *v,
		char *sVal) {
	switch (dt) {
	case DATA_TYPE_DOUBLE:
		v->dblVal = atof(sVal);
		break;
	case DATA_TYPE_STRING:
		strcpy(v->strVal, sVal);
		break;
	case DATA_TYPE_LONG:
		v->longVal = atoi(sVal);
		break;
	case DATA_TYPE_LSTRING:
		strcpy(v->pLStr, sVal);
		break;
	case DATA_TYPE_BOOL:
		v->boolVal = atoi(sVal);
		break;
	default:
		//printf("Value type error\n");
		printf(MSG_VALTYPE_ERR);
	}
}

int CMRTInterpreter::CompareValue(TYPE_VALUE *tv, VALUE_UNION *v) {
	double dblVal;
	long longVal;
//printf("COMPARE Info ==> %d %d\n", tv->dataType, DATA_TYPE_STRING );

	switch (tv->dataType) {
	case DATA_TYPE_STRING:
#ifdef _DEBUG
// 		printf("COMPARE val ==> %s %s\n", tv->value.strVal, v->strVal );
#endif
		return strcmp(tv->value.strVal, v->strVal);

	case DATA_TYPE_DOUBLE:
		dblVal = atof(v->strVal);
#ifdef _DEBUG
//		printf("COMPARE val ==> %lf %lf\n", tv->value.dblVal, dblVal);
#endif
		if (tv->value.dblVal < dblVal)
			return -1;
		if (tv->value.dblVal > dblVal)
			return 1;
		return 0;

	case DATA_TYPE_LONG:
		longVal = atol(v->strVal);
		if (tv->value.longVal < longVal)
			return -1;
		if (tv->value.longVal > longVal)
			return 1;
		return 0;

	case DATA_TYPE_LSTRING:
		return strcmp(tv->value.pLStr, v->strVal);

	default:
		return 0;
	}
}

bool CMRTInterpreter::Calc_Match(char *s1, char *s2) {
	bool b_Result = true;
	bool b_TermFlag = false;
	char *search, *t;
	char token[PARAM_MAX_COUNT];
	int i;
	bool b_lastTokYn = false;

	b_Result = true;
  if( *s1==NULL ) b_Result=false;
  
	while (*s1) { 
		switch (*s2) {
		case '_':
		case '?':
			s1++;
			s2++;
			break;
		case '%':
		case '*':
			s2++;
			while (*s2) {
				if (*s2 == '*' || *s2 == '?' || *s2 == '%'  || *s2 == '_')
					s2++;
				else
					break;
			}

			if (!*s2) {
				b_TermFlag = true;
				break;
			}

			i = 0;
			while (*s2) {
				if (*s2 == '*' || *s2 == '?' || *s2 == '%'  || *s2 == '_')
					break;

				token[i] = *s2;
				i++;
				s2++;
			}

			token[i] = 0;

			if (*s2)
				b_lastTokYn = 0;
			else
				b_lastTokYn = 1;

			search = strstr(s1, token);
			if (!search) {
				b_Result = false;
				b_TermFlag = true;
				break;
			}

			s1 = search + strlen(token);
			if (!b_lastTokYn)
				break;

			//  최종토큰일때 마지막을 Search한다.
			t = search + 1;
			while (*t) {
				search = strstr(t, token);
				if (!search)
					break;

				s1 = search + strlen(token);
				t = search + 1;
			}

			break;

		default:
			if (*s1++ != *s2++) {
				b_Result = false;
				b_TermFlag = true;
			}
		}

		if (b_TermFlag)
			break;
	}

	if  (*s2 && ( *s2 != '*' &&  *s2 != '%' ) )
		b_Result = false;

	return b_Result;
}

