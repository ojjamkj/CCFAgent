#include    <string.h>
#include    <stdio.h>
#include    <stdlib.h>

#include    "MRERROR.h"
#include    "MTExcept.h"
#include    "MRTOKEN.h"
#include    "MRRUNNER.h"
#include    "MRFRINTR.h"
#include    "MRCRINTR.h"

#include    "MRGBLVAR.h"
#include    "MREXPRES.h"

#ifdef _TESTER
#include    "BRMStart.h"
#endif

CMRFInterpreter::CMRFInterpreter(CMRRunner *pParent, CMRDataList *pDataList,
		RULE_LIST *pRuleNode, CMRParam *pParm) {
	m_pParent = pParent;
	m_pDataList = pDataList;
	m_pRuleNode = pRuleNode;
	m_pParm = pParm;

	pResultList = 0;

	for (int i = 0; i < 100; i++) {
		m_Loop[i] = -1;
		m_useNode[i] = -1;
	}

#ifdef _TRACE
	m_TraceFlag = m_pDataList->getTraceFlag();
#endif
}

CMRFInterpreter::~CMRFInterpreter() {
}

RESULT_REC * CMRFInterpreter::Running() {
	CRULE_STMT *pStmt;
	RESULT_REC *m_pTotalResult; 
	RESULT_REC *pLastResult = 0;
	int nodeCnt;
	int nNode, iNode; //, i=0;
#ifdef _DEBUG			
//	printf("CMRFInterpreter::Running():01\n");
#endif			

	m_bBreak = 0;
	m_pTotalResult = 0;

	m_pFRuleData = (FRULE_DATA*) m_pDataList->getVersionNode(m_pRuleNode,
			&nodeCnt);

	if (!m_pFRuleData)
		return 0; //  적용버전 없음

	nNode = 1;
	while (nNode > 0) {
		pResultList = 0;

		if (nNode > nodeCnt)
		{
			printf("Rule Invalid1 = %d > %d", nNode, nodeCnt);
			throw new CMTException(UERR_RULEINVALID, EMSG_RULEINVALID);
		}

//printf("_DBG_STEP_NEXT : %d\n", nNode);

#ifdef _TESTER
		m_pWorker->SuspendWork( _DBG_STEP_NEXT, nNode, 0, "" );
#endif
//printf("_DBG_STEP_NEXT : end\n");

		iNode = nNode - 1;

		pStmt = (CRULE_STMT *) m_pFRuleData[iNode].exp;

		switch (m_pFRuleData[iNode].pType) {
		case FR_NODE_TP_START:
			nNode = m_pFRuleData[iNode].sw->Link;
			break;

		case FR_NODE_TP_END:
			nNode = 0;
			break;

		case FR_NODE_TP_IF: // 판단
			nNode = SearchLink_IF(iNode, pStmt);

			break;

		case FR_NODE_TP_EXEC: // 실행
#ifdef _TRACE
		if ( m_TraceFlag )
		{
			sprintf(m_TraceMsg, "   R(%d) NODE %d (EXEC) : START",
					m_pRuleNode->rule_id, iNode+1 );
			m_pDataList->TraceOut( m_TraceMsg );
		}
#endif
			BuildResult(pStmt);

			if (!m_pFRuleData[iNode].sw)
				nNode = 0;
			else
				nNode = m_pFRuleData[iNode].sw->Link;

			pResultList = 0;

#ifdef _TRACE
			if ( m_TraceFlag )
			{
				sprintf(m_TraceMsg, "   R(%d) NODE %d (EXEC) : Link %d",
						m_pRuleNode->rule_id, iNode+1, nNode );
				m_pDataList->TraceOut( m_TraceMsg );
			}
#endif

			if (m_bBreak == 4)
				m_pDataList->m_bExit = 1;

			break;

		case FR_NODE_TP_RET: // 반환
#ifdef _TRACE
		if ( m_TraceFlag )
		{
			sprintf(m_TraceMsg, "   R(%d) NODE %d (RETURN) : START",
					m_pRuleNode->rule_id, iNode+1 );
			m_pDataList->TraceOut( m_TraceMsg );
		}
#endif
			if (m_pFRuleData[iNode].dGbn) { // (상수값)
				int sz = sizeof(VALUE_UNION) * m_pRuleNode->retCnt;

				pResultList = (RESULT_REC *) m_pDataList->MRMemAlloc1(
						sizeof(RESULT_REC));
				pResultList->value = (VALUE_UNION *) m_pDataList->MRMemAlloc1(
						sz);
				memcpy((char *) pResultList->value, (char *) pStmt, sz);
				pResultList->pNext = 0;
			} else {
				BuildResult(pStmt);

				if (m_bBreak == 4) {
					m_pDataList->m_bExit = 1;
					break;
				}
			}

#ifdef _TESTER
			if ( pResultList ) {
				m_pDataList->DbgRuleReturn( m_pRuleNode, pResultList, "" );
				m_pWorker->SuspendWork( _DBG_STEP_RET, 0, 0, "" );
			}
#endif

			if (!m_pFRuleData[iNode].sw)
				nNode = 0;
			else
				nNode = m_pFRuleData[iNode].sw->Link;

#ifdef _TRACE
			if ( m_TraceFlag )
			{
				sprintf(m_TraceMsg, "   R(%d) NODE %d (RETURN) : Link %d",m_pRuleNode->rule_id, iNode+1, nNode );
				m_pDataList->TraceOut( m_TraceMsg );
			}
#endif

			break;

		case FR_NODE_TP_CASE: // 다중선택

			nNode = SearchLink_CASE(iNode, pStmt);

			break;

		case FR_NODE_TP_LOOP: // LOOP
		{
			FRULE_DATA_SW *p = m_pFRuleData[iNode].sw;


			int useIdx;
			int idx;

			if (!p)
				throw new CMTException(USERERR_GRAMMAR_118,
						"The start node of loop does not exist.");

			useIdx = atoi(p->caseExp) - 1;
			if (useIdx < 0)
				throw new CMTException(USERERR_GRAMMAR_119,
						"The start node of loop does not exist.");

#ifdef _DEBUG
//			printf( "m_Loop[useIdx] = %d    useIdx = %d\n", m_Loop[useIdx], useIdx );
//			printf( "m_useNode[useIdx] = %d   iNode = [%d]\n", m_useNode[useIdx], iNode );
#endif
			
			if (m_Loop[useIdx] < 0) {
				/*
				 #ifdef _TRACE
				 if ( m_TraceFlag )
				 {
				 sprintf(m_TraceMsg, "   R(%d) NODE %d (LOOP) START",
				 m_pRuleNode->rule_id, iNode+1 );
				 m_pDataList->TraceOut( m_TraceMsg );
				 }
				 #endif
				 */
				m_Loop[useIdx] = getLoopCnt(pStmt);

//		printf("_DBG_STEP_LOOP : %d  %d\n", useIdx, m_Loop[useIdx]);

#ifdef _TESTER
				m_pWorker->SuspendWork( _DBG_STEP_LOOP, useIdx, m_Loop[useIdx], "" );
#endif

//		printf("_DBG_STEP_LOOP : end\n");

#ifdef _TRACE
				if ( m_TraceFlag )
				{
					sprintf(m_TraceMsg, "   R(%d) NODE %d (LOOP) : %d times",m_pRuleNode->rule_id, iNode+1, m_Loop[useIdx] );
					m_pDataList->TraceOut( m_TraceMsg );
				}
#endif
				m_pParent->ClearIndexValue(useIdx + 1);

				m_useNode[useIdx] = iNode;
			} else {
				if (m_useNode[useIdx] != iNode)
					throw new CMTException(USERERR_GRAMMAR_120,
							"Duplicate use of Index");
			}
			idx = m_pParent->getNextIndexValue(useIdx + 1);

			if (idx <= m_Loop[useIdx]) { /// Loop count 이내일때
				nNode = p->Link;
#ifdef _DEBUG
//				printf("Loop idx = %d   Next = %d\n", idx, nNode);
#endif
			} else { //  Loop 벗어날때
				p = p->pNext;
				if (!p)
					nNode = 0;
				else
					nNode = p->Link;

				m_pParent->ResetIndexValue(useIdx + 1);

				m_Loop[useIdx] = -1;
				m_useNode[useIdx] = -1;
//		printf("Loop end\n");

			}

		} //  필요한 놈

			break;

		default:
			return m_pTotalResult;
		}

		if (pResultList) {
			if (!m_pTotalResult)
				m_pTotalResult = pResultList;
			else
				pLastResult->pNext = pResultList;

			pLastResult = pResultList;

			while (pLastResult->pNext)
				pLastResult = pLastResult->pNext;
		}

		if (m_pDataList->m_bExit == 1)
			return m_pTotalResult;
	}

	return m_pTotalResult;
}

/***********************************************
 * Parm : CRULE_STMT *pStmt : Condition Statement
 ************************************************/
int CMRFInterpreter::SearchLink_IF(int idx, CRULE_STMT *pStmt) {
	TYPE_VALUE *expResult;
	bool cond;

//	if ( ! pStmt ) return true;

#ifdef _TRACE
	if ( m_TraceFlag )
	{
		sprintf(m_TraceMsg, "   R(%d) NODE %d (IF) START",
				m_pRuleNode->rule_id, idx+1, m_pRuleNode->rule_id );
		m_pDataList->TraceOut( m_TraceMsg );
	}
#endif

	//Asigned Only Pointer for CMRExpression
	CMRExpression Expression(m_pParent, m_pDataList, m_pRuleNode, m_pParm);

//printf("MRFRINTR : Expression.CalcExpression - 1\n");

	expResult = Expression.CalcExpression(pStmt);
//printf("MRFRINTR : Expression.CalcExpression - 2\n");

	if (!expResult)
	{
//		printf("SearchLink_IF NODE %d : !expResult", idx+1, pStmt->pExpression);
		throw new CMTException(UERR_RULEINVALID, EMSG_RULEINVALID);
	}

	if (expResult->dataType == TOKENTYPE_RULE)
		m_pDataList->GetRuleValue(expResult);
	else if (expResult->dataType == TOKENTYPE_GLOBAL)
		m_pDataList->getItemValue(expResult);

	switch (expResult->dataType) {
	case DATA_TYPE_BOOL:
		cond = (bool) expResult->value.boolVal;
		break;
	case DATA_TYPE_LONG:
		cond = (bool) expResult->value.longVal;
		break;
	default:
		printf("SearchLink_IF NODE %d : DataType %d invalid", idx+1, expResult->dataType);
		throw new CMTException(UERR_RULEINVALID, EMSG_RULEINVALID);
	}
//printf("MRFRINTR : Expression.CalcExpression - 3\n");

	FRULE_DATA_SW *p;
	int next = 0;

	p = m_pFRuleData[idx].sw;

	while (p) {
		if (atoi(p->caseExp) == cond) {
			next = p->Link;
			break;
		}

		p = p->pNext;
	}

#ifdef _TRACE
	if ( m_TraceFlag )
	{
		sprintf(m_TraceMsg, "   R(%d) NODE %d (IF) : %s => %d",
				m_pRuleNode->rule_id, idx+1, cond?"TRUE":"FALSE", next );
		m_pDataList->TraceOut( m_TraceMsg );
	}
#endif

	return next;
}

int CMRFInterpreter::SearchLink_CASE(int idx, CRULE_STMT *pStmt) {
	TYPE_VALUE *pLocalValue, pExpValue;
	char strVal[101];

#ifdef _TRACE
	if ( m_TraceFlag )
	{
		sprintf(m_TraceMsg, "   R(%d) NODE %d (CASE) START",
				m_pRuleNode->rule_id, idx+1, m_pRuleNode->rule_id );
		m_pDataList->TraceOut( m_TraceMsg );
	}
#endif

	CMRExpression pExpress(0, m_pDataList, m_pRuleNode, m_pParm);

	pLocalValue = pExpress.CalcExpression(pStmt);

	if (!pLocalValue)
	{
		printf("SearchLink_CASE NODE %d !pLocalValue", idx+1);
		throw new CMTException(UERR_RULEINVALID, EMSG_RULEINVALID);
	}

	memcpy((char*) &pExpValue, (char*) pLocalValue, sizeof(TYPE_VALUE));

	if (pExpValue.dataType == TOKENTYPE_RULE)
		m_pDataList->GetRuleValue(&pExpValue);
	else if (pExpValue.dataType == TOKENTYPE_GLOBAL)
		m_pDataList->getItemValue(&pExpValue);

	switch (pExpValue.dataType) {
	case DATA_TYPE_STRING:
		strcpy(strVal, pExpValue.value.strVal);
		break;
	case DATA_TYPE_LSTRING:
		strncpy(strVal, pExpValue.value.pLStr, 100);
		strVal[100] = 0;
		break;
	case DATA_TYPE_DOUBLE:
		sprintf(strVal, "%lf", pExpValue.value.dblVal);
		strVal[100] = 0;
		break;
	case DATA_TYPE_LONG:
		// syk change
		//ltoa(pExpValue.value.longVal, strVal, 10);
		sprintf(strVal, "%ld", pExpValue.value.longVal);
		break;
	case DATA_TYPE_BOOL:
		sprintf(strVal, "%d", pExpValue.value.boolVal);
		break;
	default:
		sprintf(strVal , "%s, CASE NODE(%d) -> NO CHOICE",EMSG_RULEINVALID, idx+1 );
		throw new CMTException(UERR_RULEINVALID, strVal);
	}

	FRULE_DATA_SW *p = m_pFRuleData[idx].sw;
	int next = 0;
	int defaultNext = 0;
	while (p) {
//printf("[MRFRINTR] |%s|%s|\n", p->caseExp, strVal );
		if (strcmp(p->caseExp, strVal) == 0) {
			next = p->Link;
			break;
		}
		if(strcmp(p->caseExp, "*") == 0) {
		 
			defaultNext = p->Link;
		}
		p = p->pNext;
	}

#ifdef _TRACE
	if ( m_TraceFlag )
	{
		sprintf(m_TraceMsg, "   R(%d) NODE %d (CASE) : %s => %d",
				m_pRuleNode->rule_id, idx+1, strVal, next );
		m_pDataList->TraceOut( m_TraceMsg );
	}
#endif
	if(next==0 && defaultNext != 0)
	{
		return defaultNext;
	}

	return next;
}

void CMRFInterpreter::BuildResult(CRULE_STMT *pStmt) {
	CMRCInterpreter CInterpreter(m_pParent, m_pDataList, m_pRuleNode, m_pParm);
	pResultList = CInterpreter.OneResult(pStmt);
	m_bBreak = CInterpreter.m_bBreak;
}

int CMRFInterpreter::getLoopCnt(CRULE_STMT *pStmt) {
	TYPE_VALUE *pLocalValue, pExpValue;

	CMRExpression pExpress(m_pParent, m_pDataList, m_pRuleNode, m_pParm);

	pLocalValue = pExpress.CalcExpression(pStmt);

	if (!pLocalValue)
		return 0;

	memcpy((char*) &pExpValue, (char*) pLocalValue, sizeof(TYPE_VALUE));

	if (pExpValue.dataType == TOKENTYPE_RULE)
		m_pDataList->GetRuleValue(&pExpValue);
	else if (pExpValue.dataType == TOKENTYPE_GLOBAL)
		m_pDataList->getItemValue(&pExpValue);

	if (pExpValue.dataType != DATA_TYPE_LONG)
		return 0;

	return pExpValue.value.longVal;
}

