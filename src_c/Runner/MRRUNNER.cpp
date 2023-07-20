#include  <stdlib.h>         /* For _MAX_PATH definition */
#include  <stdio.h>
#include  <string.h>

#include  "MRPGMEXP.h"
#include  "MRERROR.h"
#include  "MRTOKEN.h"
#include  "MTUtil.h"

#include  "MRRUNNER.h"
#include  "MREXPRES.h"
#include  "MRCRINTR.h"
#include  "MRQRINTR.h"
#include  "MRBRINTR.h"
#include  "MRTRINTR.h"
#include  "MRFRINTR.h"

#include  "MRGBLVAR.h"

#ifdef	_RUNNER
#include  "BREXLog.h"
#endif
extern char _DEBUG_YN[2];
#include    <time.h>


#if defined(__GNUC__)
#include	<sys/time.h>
#endif
#if defined(_OS390)
#include	<sys/time.h>
#endif
#if defined(WIN32)
#include	<sys/timeb.h>
#endif

#ifdef _TESTER
#include    "BRMStart.h"
#endif


#ifdef _MSG_KO
#define   MSG_NOTEXIST_RULE     "RuleId ( %d )가 존재하지 않습니다."
#define   MSG_DIFFCNT_RETURN    "반환 개수가 다릅니다. (Rule %d)"
#define   MSG_RULEDATA_NULL     " ExecuteRule (%d) : 룰 데이터가 null 입니다.\n "
#define   MSG_NOTINIT_INDEX     "INDEX_%d가 초기화되지 않았습니다."
#else
#define   MSG_NOTEXIST_RULE     "RuleId ( %d ) not exist."
#define   MSG_DIFFCNT_RETURN    "Different return count. (Rule %d)"
#define   MSG_RULEDATA_NULL     " ExecuteRule (%d) : rule_data is null\n "
#define   MSG_NOTINIT_INDEX     "INDEX_%d not initialized."
#endif

double getMiliSecond() {
	double usec;

#ifdef WIN32
	struct _timeb timebuffer;
	_ftime(&timebuffer);

	usec = timebuffer.time * 1000.0 + timebuffer.millitm;

#else
	struct timeval stamp1;

	gettimeofday(&stamp1, NULL);

	usec = stamp1.tv_sec * 1000 + stamp1.tv_usec / 1000;

#endif

	return usec;
}

CMRRunner::CMRRunner(int ruleId, char *Parm, CMRDataList *pClass,
		CMRParam *pParmInfo, unsigned char saveFlag) {
	m_pDataList = pClass;
	m_pResultList = 0;
	m_LocalParm = pParmInfo;
	m_SaveFlag = saveFlag;

	m_IndexVal_1 = -1;
	m_IndexVal_2 = -1;
	m_IndexVal_3 = -1;

#ifdef _TRACE
	m_TraceFlag = m_pDataList->getTraceFlag();
#endif

#ifdef _TESTER

	if ( ! (m_pRuleNode = pCMRGblVars->DbgLoadRule( ruleId )) ) {
		m_bRuleExist = 0;
		//sprintf( m_lpszMessage, "RuleId ( %d ) not exist.", ruleId );
		sprintf( m_lpszMessage, MSG_NOTEXIST_RULE, ruleId );
		throw new CMTPgmException( UERR_RULENFOUND, m_lpszMessage );
		return;
	}

	m_pDataList->DbgClearFile();

	//  CALL
	m_pWorker->SuspendWork( _DBG_STEP_CALL, ruleId, 0, "" );

	m_pDataList->setItemValues();

	m_pWorker->SuspendWork( _DBG_STEP_NEXT, 0, 0, "" );

#else

	if (configRec.usStatFlag == 2) {
		m_stTm = getMiliSecond();
	}

	if (!(m_pRuleNode = pCMRGblVars->getRuleNode(ruleId))) {
		m_bRuleExist = 0;
		//sprintf(m_lpszMessage, "RuleId ( %d ) not exist.", ruleId);
		sprintf(m_lpszMessage, MSG_NOTEXIST_RULE, ruleId);
		printf("%s",m_lpszMessage);
		throw new CMTPgmException(UERR_RULENFOUND, m_lpszMessage);
		return;
	}

#endif

#ifdef _TRACE
	if ( m_TraceFlag )
	{
		switch (m_pRuleNode->r_type) {
			case 1 : /*   CONDITIONAL RULE  */
			sprintf(m_TraceMsg, "\nSTART RULE{%d} : Conditional, Break %s",
					ruleId, m_pRuleNode->breakFlag ? "yes" : "no" );
			break;
			case 2 : /*   DYNAMIC SQL RULE  */
			sprintf(m_TraceMsg, "\nSTART RULE{%d} : Query", ruleId );
			break;
			case 3 : /*   BRANCH RULE  */
			sprintf(m_TraceMsg, "\nSTART RULE{%d} : Branch", ruleId );
			break;
			case 4 : /*   External RULE  */
			break;
			case 5 : /*   Extended Table RULE  */
			sprintf(m_TraceMsg, "\nSTART RULE{%d} : Table, Break %s",
					ruleId, m_pRuleNode->breakFlag ? "yes" : "no" );
			break;
			case 6 : /*   Flow RULE  */
			sprintf(m_TraceMsg, "\nSTART RULE{%d} : Flow", ruleId );
			default: /*   Garbage  */
			break;
		}

		m_pDataList->TraceOut( m_TraceMsg );
	}
#endif

	m_pDataList->setRuleId(ruleId);

	m_bRuleExist = 3;

	MainProcess(Parm);

}

CMRRunner::CMRRunner(char *ruleName, char *Parm, CMRDataList *pClass,
		CMRParam *pParmInfo) {
	int cnt;

	m_pDataList = pClass;
	m_pResultList = 0;
	m_LocalParm = pParmInfo;
	m_SaveFlag = 0;

	m_IndexVal_1 = -1;
	m_IndexVal_2 = -1;
	m_IndexVal_3 = -1;

#ifdef _TRACE
	m_TraceFlag = m_pDataList->getTraceFlag();
#endif

#ifdef _TESTER

	if ( ! (m_pRuleNode = pCMRGblVars->DbgLoadRule( ruleName )) ) {
		m_bRuleExist = 0;
		return;
	}

	m_pDataList->DbgClearFile();

	//  CALL
	m_pWorker->SuspendWork( _DBG_STEP_CALL, m_pRuleNode->rule_id, 0, "" );

	m_pDataList->setItemValues();

	m_pWorker->SuspendWork( _DBG_STEP_NEXT, 0, 0, "" );

#else

	if (configRec.usStatFlag == 2) {
		m_stTm = getMiliSecond();
	}

	if (!(m_pRuleNode = pCMRGblVars->getRuleNode(ruleName))) {
		m_bRuleExist = 0;

#ifdef _TRACE
		if ( m_TraceFlag )
		{
			sprintf( m_TraceMsg, "START RULE{%s} : branch name not exist.", ruleName );
			m_pDataList->TraceOut( m_TraceMsg );
		}
#endif

		return;
	}

#endif

#ifdef _TRACE
	if ( m_TraceFlag )
	{
		switch (m_pRuleNode->r_type) {
			case 1 : /*   CONDITIONAL RULE  */
			sprintf(m_TraceMsg, "START RULE{%d} : Conditional, Break %s\n",
					m_pRuleNode->rule_id, m_pRuleNode->breakFlag ? "yes" : "no" );
			break;
			case 2 : /*   DYNAMIC SQL RULE  */
			sprintf(m_TraceMsg, "START RULE{%d} : Query\n", m_pRuleNode->rule_id );
			break;
			case 3 : /*   BRANCH RULE  */
			sprintf(m_TraceMsg, "START RULE{%d} : Branch\n", m_pRuleNode->rule_id );
			break;
			case 4 : /*   External RULE  */
			break;
			case 5 : /*   Extended Table RULE  */
			sprintf(m_TraceMsg, "START RULE{%d} : Table, Break %s\n",
					m_pRuleNode->rule_id, m_pRuleNode->breakFlag ? "yes" : "no" );
			break;
			case 6 : /*   Flow RULE  */
			sprintf(m_TraceMsg, "START RULE{%d} : Flow\n", m_pRuleNode->rule_id );
			default: /*   Garbage  */
			break;
		}

		m_pDataList->TraceOut( m_TraceMsg );
	}
#endif

	//  Branch에서  Name으로 실행할때는 Parm에 return type이 넘어온다.
	cnt = strlen(Parm);
	if (m_pRuleNode->retCnt != cnt) {/*
		sprintf(m_lpszMessage, "Different return count. (Rule %d)",
				m_pRuleNode->rule_id);*/
		sprintf(m_lpszMessage, MSG_DIFFCNT_RETURN,
				m_pRuleNode->rule_id);
		throw new CMTPgmException(UERR_RESULTCOUNT, m_lpszMessage);
	}

	m_bDifferent = 0;
	for (int i = 0; i < cnt; i++) {
		if (m_pRuleNode->retType[i] != Parm[i]) {
			m_bDifferent = 1;

			if ((m_pRuleNode->retType[i] == DATA_TYPE_LSTRING
					|| m_pRuleNode->retType[i] == DATA_TYPE_STRING)
					&& (Parm[i] == DATA_TYPE_LSTRING
							|| Parm[i] == DATA_TYPE_STRING))
				continue;

			sprintf(m_lpszMessage, "Different return types. (Rule %d)",
					m_pRuleNode->rule_id);
			throw new CMTPgmException(UERR_RESULTCOUNT, m_lpszMessage);
		}
	}

	m_pDataList->setRuleId(m_pRuleNode->rule_id);

	m_bRuleExist = 1;

	MainProcess(Parm);

}

int CMRRunner::chkReturnTypeDiff(char *tp) {
	memcpy(tp, m_pRuleNode->retType, m_pRuleNode->retCnt);
	return m_bDifferent;
}

CMRRunner::~CMRRunner() {
}

void CMRRunner::Delete(void) {
	delete this;
}

void CMRRunner::MainProcess(char *Parm) {

#ifdef _TESTER
	if ( m_LocalParm ) {
		m_LocalParm->DbgParmInfo();
		m_pWorker->SuspendWork( _DBG_STEP_PARM, 0, 0, "" );
	}
#endif

	m_UsedRuleCnt = 0;

	try {

		if (!m_pRuleNode->pVers) {
			m_pDataList->AddResultForRule(m_pRuleNode->rule_id, Parm,(RESULT_REC *) 0, m_pRuleNode);
			/*
			printf(" ExecuteRule (%d) : rule_data is null\n ",
					m_pRuleNode->rule_id);*/
			printf(MSG_RULEDATA_NULL,m_pRuleNode->rule_id);
		} else {

			m_pResultList = ExecuteRule();

			if (m_pResultList && m_pRuleNode->pAggr) {
				if (m_pResultList->pNext && m_pRuleNode->pAggr->Option) {
#ifdef _TESTER
					m_pWorker->SuspendWork( _DBG_STEP_NEXT, 0, 1, "" );
#endif
					EtcFunction();
				}

				if (m_pRuleNode->pAggr->nSort[0] > 0)
					Sorting();
			}
		}

	} catch (CMTException *e) {
		long rc;
		rc = e->GetECode();
		sprintf(m_lpszMessage, "%s in Rule(%d)", e->GetEMessage(),m_pRuleNode->rule_id);
		e->Delete();

		throw new CMTPgmException(rc, m_lpszMessage);
	}

	// Default Value처리
	if (!m_pResultList && m_pRuleNode->dftResult) {

#ifdef _TESTER
		m_pWorker->SuspendWork( _DBG_STEP_NEXT, 0, 2, "" );
#endif

//  상수만 가능하게 고침.
//
//		CRULE_STMT *p;
//		p = (CRULE_STMT *) m_pRuleNode->dftResult;
//
//		if ( p->pNext ) {
//			CMRCInterpreter CInterpreter( m_pDataList, m_pRuleNode, m_LocalParm );
//
//			m_pResultList = CInterpreter.OneResult( (CRULE_STMT*) m_pRuleNode->dftResult );
//		} else {
		int sz = sizeof(VALUE_UNION) * m_pRuleNode->retCnt;

		m_pResultList = (RESULT_REC *) m_pDataList->MRMemAlloc1(sizeof(RESULT_REC));

		m_pResultList->value = (VALUE_UNION *) m_pDataList->MRMemAlloc1(sz);
		memcpy((char *) m_pResultList->value, (char *) m_pRuleNode->dftResult,sz);
//printf("     Ok!\n");
		m_pResultList->pNext = 0;
//		}
	}

	if (m_SaveFlag) { //  Branch rule에서 Call되면 저장하지 않는다.
		pRuleResultPtr = m_pDataList->AddResultForRule(m_pRuleNode->rule_id,Parm, m_pResultList, m_pRuleNode);
	}

#ifdef _TESTER
	m_pDataList->DbgRuleReturn( m_pRuleNode, m_pResultList, Parm );
	m_pWorker->SuspendWork( _DBG_STEP_EOR, m_pRuleNode->rule_id, 0, "" );
#endif

#ifdef _TRACE
	if ( m_TraceFlag )
	{
		m_pDataList->LogRuleReturn(m_pRuleNode, m_pResultList);

		sprintf(m_TraceMsg, "END RULE{%d}.\n", m_pRuleNode->rule_id );
		m_pDataList->TraceOut( m_TraceMsg );
	}
#endif

	//  Add Statistics
#ifdef	_RUNNER
	if ( configRec.usStatFlag == 2 ) {
		m_stTm = getMiliSecond() - m_stTm;
		m_pRuleNode->totTm += m_stTm;
		if ( m_stTm > m_pRuleNode->maxTm ) m_pRuleNode->maxTm = (long) m_stTm;

		m_pRuleNode->hit ++;
	}
#endif

}

RESULT_REC * CMRRunner::ExecuteRule() {
	m_bBreak = 0;
#ifdef _DEBUG
// 	printf("[CMRRunner::ExecuteRule] Id[%d] type[%d]\n", m_pRuleNode->rule_id, m_pRuleNode->r_type );
#endif

	switch (m_pRuleNode->r_type) {
	case 1: /*   CONDITIONAL RULE  */
	{
		CMRCInterpreter CInterpreter(this, m_pDataList, m_pRuleNode, m_LocalParm);
		m_pSingleResult = CInterpreter.Running();
		m_bBreak = CInterpreter.m_bBreak;
	}
		break;
	case 2: /*   DYNAMIC SQL RULE  */
	{

//	  printf("mrrunner  m_LocalParm[%s] \n",(char*)m_LocalParm);
		CMRQInterpreter QInterpreter(m_pDataList, m_pRuleNode, m_LocalParm);
		if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]\n", __FILE__, __LINE__);   
		m_pSingleResult = (RESULT_REC *) QInterpreter.BuildResult();
		if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]\n", __FILE__, __LINE__);
	}
		break;
	case 3: /*   BRANCH RULE  */
	{
		CMRBInterpreter BInterpreter(m_pDataList, m_pRuleNode, m_LocalParm);
		m_pSingleResult = (RESULT_REC *) BInterpreter.BuildResult();

#ifdef _TESTER
		if ( m_pSingleResult ) {
			m_pDataList->DbgRuleReturn( m_pRuleNode, m_pSingleResult, "" );
			m_pWorker->SuspendWork( _DBG_STEP_RET, 0, 0, "" );
		}
#endif
	}
		break;
	case 4: /*   External RULE  */
//				    RunExternalRule();
		break;
	case 5: /*   Extended Table RULE  */
	{
		CMRTInterpreter pRunTableRule(this, m_pDataList, m_pRuleNode,
				m_LocalParm);
		m_pSingleResult = pRunTableRule.Running();
	}
		break;
	case 6: /*   Flow RULE  */
	{
		CMRFInterpreter pRunFlowRule(this, m_pDataList, m_pRuleNode,
				m_LocalParm);
		m_pSingleResult = pRunFlowRule.Running();
	}
		break;
	default: /*   Garbage  */
		break;
	}

	return m_pSingleResult;
}

RULE_LIST * CMRRunner::getRuleInfo() {
	return m_pRuleNode;
}

RESULT_REC * CMRRunner::getResultList() {
	return m_pResultList;
}

RULE_RESULT * CMRRunner::getRuleResultPtr() {
	return pRuleResultPtr;
}

/********************************************************************************************/
/********************************************************************************************/

void CMRRunner::addRuleToRunList(RULE_RESULT *pRuleResult) {
	if (m_UsedRuleCnt == 200)
		return;

	m_UsedRulesPtr[m_UsedRuleCnt] = pRuleResult;

	m_UsedRuleCnt++;
}

RULE_RESULT * CMRRunner::isAlreadyRun(long ruleId, char *parm) {
	if (m_pDataList->usedRuleCleared())
		return 0;

	for (int i = 0; i < m_UsedRuleCnt; i++) {
		if (!m_UsedRulesPtr[i])
			continue;

		if (ruleId == m_UsedRulesPtr[i]->ruleId) {
			if (parm != 0) {
				if (strcmp(parm, m_UsedRulesPtr[i]->parmInfo) == 0)
					return m_UsedRulesPtr[i];
			} else if (m_UsedRulesPtr[i]->parmInfo[0] == 0)
				return m_UsedRulesPtr[i];

		}
	}

	return 0;
}

/********************************************************************************************/
/********************************************************************************************/

void CMRRunner::EtcFunction() {
	memcpy(m_compGroup, m_pRuleNode->pAggr->group, 11);
	m_CmpCnt = strlen(m_compGroup);

/* 에이스프레임웍 소스에 해당 로직 존재함 2017.05.31 해당 로직 분석해야함. ad2001 	
	if ( m_CmpCnt == 0 ) {
		m_CmpCnt = m_pRuleNode->retCnt;
		if ( m_CmpCnt > 9 ) m_CmpCnt = 9;
		for ( int i=0; i<m_CmpCnt; i++ ) {
			m_compGroup[i] = '1' + i;
		}
	}
*/
	switch (m_pRuleNode->pAggr->Option) {
	case 2: // Distinct
//			     m_CmpCnt = 1;
//			     strcpy( m_pRuleNode->pAggr->group, "1" );
		//added by DSKIM 2016.03.09
		DistinctResult();
		break;
	case 1: // SUM
		SumResult(m_pRuleNode->pAggr->Option);
		break;
	case 3: // MIN
	case 4: // MAX
		MinMaxResult(m_pRuleNode->pAggr->Option);
	default:
		break;
	}
}

/*
void CMRRunner::SumResult(short opt) {
	RESULT_REC *pCur, *pNew, *pLast;
	int sz = sizeof(VALUE_UNION) * m_pRuleNode->retCnt;

	if (opt == 1) { //  SUM일때만
		pNew = m_pResultList;

		m_pResultList = (RESULT_REC *) m_pDataList->MRMemAlloc1(
				sizeof(RESULT_REC));

		m_pResultList->value = (VALUE_UNION *) m_pDataList->MRMemAlloc1(sz);
		memcpy((char *) m_pResultList->value, (char *) pNew->value, sz);
		m_pResultList->pNext = 0;

		pNew = pNew->pNext;
	} else {
		pNew = m_pResultList->pNext;
		//modified by DSKIM 2016.02.17 -- CR 8025 집합함수 Distinct 사용시 결과가 한건만 나타남  -- 기존 코드를 주석처리함.
		//m_pResultList->pNext = 0;

	}

	while (pNew) {
		pCur = m_pResultList;

		while (pCur) {
			if (resultCompare(pNew, pCur) == 0) {
				if (opt == 1)
					VerticalResultSum(pCur, pNew);
				break;
			}

			pLast = pCur;
			pCur = pCur->pNext;
		}

		if (!pCur) {
			pLast->pNext = (RESULT_REC *) m_pDataList->MRMemAlloc1(
					sizeof(RESULT_REC));
			pLast = pLast->pNext;

			pLast->value = (VALUE_UNION *) m_pDataList->MRMemAlloc1(sz);
			memcpy((char *) pLast->value, (char *) pNew->value, sz);
			pLast->pNext = 0;
		}

		pNew = pNew->pNext;
	}
}
*/
void CMRRunner::SumResult(short opt) {
	RESULT_REC *pCur, *pNew, *pLast;
	int sz = sizeof(VALUE_UNION) * m_pRuleNode->retCnt;

//	if (opt == 1) { //  SUM일때만
		pNew = m_pResultList;

		m_pResultList = (RESULT_REC *)
				m_pDataList->MRMemAlloc1(sizeof(RESULT_REC));

		m_pResultList->value = (VALUE_UNION *)
				m_pDataList->MRMemAlloc1(sz);
		memcpy((char *) m_pResultList->value
				, (char *) pNew->value
				, sz);
		m_pResultList->pNext = 0;

		pNew = pNew->pNext; 

	while (pNew) {
		pCur = m_pResultList;
		while (pCur) {
			if (resultCompare(pNew, pCur) == 0) {
				if (opt == 1)
					VerticalResultSum(pCur, pNew);
				break;
	
			}
				pLast = pCur;
				pCur = pCur->pNext;

		}

		if (!pCur) {
			pLast->pNext = (RESULT_REC *)
					m_pDataList->MRMemAlloc1(sizeof(RESULT_REC));
			pLast = pLast->pNext;

			pLast->value = (VALUE_UNION *)
					m_pDataList->MRMemAlloc1(sz);
			memcpy((char *) pLast->value
					, (char *) pNew->value
					, sz);
			pLast->pNext = 0;
		}

		pNew = pNew->pNext;
	}
//	}
}

//added by DSKIM 2016.03.09 CR 8025 집합함수 Distinct 사용시 결과가 한건만 나타남(distinct 처리 함수 추가)
void CMRRunner::DistinctResult() {
	RESULT_REC	*m_tempList1 = (RESULT_REC *)m_pDataList->MRMemAlloc1(sizeof(RESULT_REC));
	RESULT_REC	*m_tempList2 = (RESULT_REC *)m_pDataList->MRMemAlloc1(sizeof(RESULT_REC));
	RESULT_REC	*m_dupList = (RESULT_REC *)m_pDataList->MRMemAlloc1(sizeof(RESULT_REC));

	m_tempList1 = m_pResultList;
	while(m_tempList1 != NULL && m_tempList1->pNext != NULL) {
		m_tempList2 = m_tempList1;

		while(m_tempList2->pNext != NULL) {
			if(strcmp((char *)m_tempList1->value, (char *)m_tempList2->pNext->value) == 0) {
				m_dupList = m_tempList2->pNext;
				m_tempList2->pNext = m_tempList2->pNext->pNext;
				//free(m_dupList);
				m_dupList = 0;
			} else {
				m_tempList2 = m_tempList2->pNext;
			}
		}
		m_tempList1 = m_tempList1->pNext;
	}
}

void CMRRunner::Sorting() {
	RESULT_REC *pCur, *pPrev, *pList;
	int cmp;

	pList = m_pResultList->pNext;
	m_pResultList->pNext = 0;

	while (pList) {
		pPrev = 0;
		pCur = m_pResultList;
		while (pCur) {
			cmp = sortCompare(pCur, pList);

			if (cmp) {
				if (pPrev) {
					pPrev->pNext = pList;
					pList = pList->pNext;
					pPrev->pNext->pNext = pCur;
				} else {
					pCur = m_pResultList;
					m_pResultList = pList;
					pList = pList->pNext;
					m_pResultList->pNext = pCur;
				}

				break;
			}

			pPrev = pCur;
			pCur = pCur->pNext;
		}

		if (!pCur) {
			pPrev->pNext = pList;
			pList = pList->pNext;
			pPrev->pNext->pNext = 0;
		}
	}
}

unsigned char CMRRunner::sortCompare(RESULT_REC* pNewData,
		RESULT_REC* pListData) {
	int idx;
	double cmp = 0.0;

	for (int i = 0; m_pRuleNode->pAggr->nSort[i] > 0; i++) {
		idx = m_pRuleNode->pAggr->nSort[i] - 1;

		switch (m_pRuleNode->retType[idx]) {
		case DATA_TYPE_DOUBLE:
			cmp = pNewData->value[idx].dblVal - pListData->value[idx].dblVal;
			break;
		case DATA_TYPE_STRING:
			cmp = (double) strcmp(pNewData->value[idx].strVal,
					pListData->value[idx].strVal);
			break;
		case DATA_TYPE_LSTRING:
			cmp = (double) strcmp(pNewData->value[idx].pLStr,
					pListData->value[idx].pLStr);
			break;
		case DATA_TYPE_LONG:
			cmp = (double) (pNewData->value[idx].longVal
					- pListData->value[idx].longVal);
			break;
		case DATA_TYPE_BOOL:
			cmp = (double) (pNewData->value[idx].boolVal
					- pListData->value[idx].boolVal);
			break;
		}

		if (cmp == 0.0)
			continue;

		if (m_pRuleNode->pAggr->cOrder[i] == 'D') {
			if (cmp < 0.0)
				return 1;
		} else {
			if (cmp > 0.0)
				return 1;
		}

		return 0;
	}

	return 0;
}

void CMRRunner::MinMaxResult(short opt) {
	RESULT_REC *pCur;
	int cmp;

	if (m_CmpCnt == 0) {
		m_CmpCnt = m_pRuleNode->retCnt;
		if (m_CmpCnt > 9)
			m_CmpCnt = 9;
		for (int i = 0; i < m_CmpCnt; i++) {
			m_compGroup[i] = '1' + i;
		}
	}

	pCur = m_pResultList->pNext;

	while (pCur) {
		cmp = resultCompare(pCur, m_pResultList);

		if (cmp == 2 && opt == 3)
			m_pResultList = pCur; // MIN
		if (cmp == 1 && opt == 4)
			m_pResultList = pCur; // MAX

		pCur = pCur->pNext;
	}

	m_pResultList->pNext = 0;
}

unsigned char CMRRunner::resultCompare(RESULT_REC* pNewData,
		RESULT_REC* pListData) {
	int idx;
	double cmp;

	for (int i = 0; i < m_CmpCnt; i++) {
		idx = m_compGroup[i] - '0' - 1;

		switch (m_pRuleNode->retType[idx]) {
		case DATA_TYPE_DOUBLE:
			cmp = pNewData->value[idx].dblVal - pListData->value[idx].dblVal;
			break;
		case DATA_TYPE_STRING:
			cmp = (double) strcmp(pNewData->value[idx].strVal,
					pListData->value[idx].strVal);
			break;
		case DATA_TYPE_LSTRING:
			cmp = (double) strcmp(pNewData->value[idx].pLStr,
					pListData->value[idx].pLStr);
			break;
		case DATA_TYPE_LONG:
			cmp = (double) (pNewData->value[idx].longVal
					- pListData->value[idx].longVal);
			break;
		case DATA_TYPE_BOOL:
			cmp = (double) (pNewData->value[idx].boolVal
					- pListData->value[idx].boolVal);
			break;
		}

		if (cmp < 0.0)
			return 2;
		if (cmp > 0.0)
			return 1;
	}

	return 0;
}

void CMRRunner::VerticalResultSum(RESULT_REC* pParent, RESULT_REC* pChild) {
	int i;

	for (int ColIdx = 0; ColIdx < m_pRuleNode->retCnt; ColIdx++) {
		for (i = 0; i < m_CmpCnt; i++) {
			if ((m_compGroup[i] - '0' - 1) == ColIdx)
				break;
		}

		if (i < m_CmpCnt)
			continue; //  Group으로 지정된 놈은 Pass한다.

		switch (m_pRuleNode->retType[ColIdx]) {
		case DATA_TYPE_LONG:
			pParent->value[ColIdx].longVal += pChild->value[ColIdx].longVal;
			break;
		case DATA_TYPE_STRING:
		case DATA_TYPE_LSTRING:
			break;
		case DATA_TYPE_DOUBLE:
			pParent->value[ColIdx].dblVal += pChild->value[ColIdx].dblVal;
			break;
		case DATA_TYPE_BOOL:
			pParent->value[ColIdx].boolVal = (pParent->value[ColIdx].boolVal
					|| pChild->value[ColIdx].boolVal);
			break;
		default:
			break;
		}
	}
}

/*****************************************************************/
/*  Index 처리                                                   */
/*****************************************************************/

int CMRRunner::getIndexValue(unsigned char idx) {
	int rc = -1;

	if (idx < 10)
		idx += TOKENTYPE_INDEX1 - 1;

	switch (idx) {
	case TOKENTYPE_INDEX1:
		rc = m_IndexVal_1;
		break;
	case TOKENTYPE_INDEX2:
		rc = m_IndexVal_2;
		break;
	case TOKENTYPE_INDEX3:
		rc = m_IndexVal_3;
		break;
	default:
		break;
	}

//  아래 주석을 제거함 2008-10-16
#ifdef _RUNNER
	if ( rc < 0 ) {
		char msg[200];
		//sprintf( msg, "INDEX_%d not initialized.", idx - 14 );
		sprintf( msg, MSG_NOTINIT_INDEX, idx - 14 );
		throw new CMTException( UERR_INDEX_NOTINIT, msg );
	}
#endif

	return rc;
}

int CMRRunner::getNextIndexValue(unsigned char idx) {
	int rc = -1;

	if (idx < 10)
		idx += TOKENTYPE_INDEX1 - 1;

	switch (idx) {
	case TOKENTYPE_INDEX1:
		rc = ++m_IndexVal_1;
		break;
	case TOKENTYPE_INDEX2:
		rc = ++m_IndexVal_2;
		break;
	case TOKENTYPE_INDEX3:
		rc = ++m_IndexVal_3;
		break;
	default:
		break;
	}

	return rc;
}

/*
 void  CMRRunner::IncreaseIndexValue( unsigned char idx )
 {
 if ( idx < 10 ) idx += TOKENTYPE_INDEX1 - 1;

 switch ( idx ) {
 case TOKENTYPE_INDEX1 : m_IndexVal_1 ++;	break;
 case TOKENTYPE_INDEX2 : m_IndexVal_2 ++;	break;
 case TOKENTYPE_INDEX3 : m_IndexVal_3 ++;	break;
 default : break;
 }
 }
 */

void CMRRunner::ClearIndexValue(unsigned char idx) {
	if (idx < 10)
		idx += TOKENTYPE_INDEX1 - 1;

	switch (idx) {
	case TOKENTYPE_INDEX1:
		m_IndexVal_1 = 0;
		break;
	case TOKENTYPE_INDEX2:
		m_IndexVal_2 = 0;
		break;
	case TOKENTYPE_INDEX3:
		m_IndexVal_3 = 0;
		break;
	default:
		break;
	}
}

void CMRRunner::ResetIndexValue(unsigned char idx) {
	if (idx < 10)
		idx += TOKENTYPE_INDEX1 - 1;

	switch (idx) {
	case TOKENTYPE_INDEX1:
		m_IndexVal_1 = -1;
		break;
	case TOKENTYPE_INDEX2:
		m_IndexVal_2 = -1;
		break;
	case TOKENTYPE_INDEX3:
		m_IndexVal_3 = -1;
		break;
	default:
		break;
	}
}
