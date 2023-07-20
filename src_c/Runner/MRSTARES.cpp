#include  <stdlib.h>
#include  <stdio.h>
#include  <string.h>

#ifndef   _OS390
#include  <malloc.h>
#endif

#include  "MRERROR.h"
#include  "MTSysMsg.h"
#include  "MTExcept.h"
#include  "MRSTARES.h"
#include    "MTCtSect.h"

CMRStaticResult g_CMRStaticResult;

CMTCriticalSection m_CSectStatic;

CMRStaticResult::CMRStaticResult() {
	m_pRuleResult = 0;
	m_AllocSize = 0;
}

CMRStaticResult::~CMRStaticResult() {
}

RULE_RESULT * CMRStaticResult::getStaticResult() {
	RULE_RESULT *p;

	m_CSectStatic.Lock();

	p = m_pRuleResult;

	m_CSectStatic.Unlock();

	return p;
}

RULE_RESULT * CMRStaticResult::AddStaticResult(RULE_RESULT *pResultAdd) {
	int cols;
	RULE_RESULT *pNew;
	RESULT_REC *pRec1, *pRec2;

	m_CSectStatic.Lock();

	pNew = (RULE_RESULT *) MetisAlloc(sizeof(RULE_RESULT));
	memcpy((char *) pNew, (char *) pResultAdd, sizeof(RULE_RESULT));
	pNew->resRecord = 0;

	if (!m_pRuleResult) {
		pNew->pNext = 0;
		m_pRuleResult = pNew;
	} else {
		pNew->pNext = m_pRuleResult;
		m_pRuleResult = pNew;
	}

	cols = pResultAdd->pRuleNode->retCnt;
	pRec1 = pResultAdd->resRecord;
	pRec2 = 0;

	while (pRec1) {
		if (!pRec2) {
			pRec2 = (RESULT_REC *) MetisAlloc(sizeof(RESULT_REC));
			pNew->resRecord = pRec2;
		} else {
			pRec2->pNext = (RESULT_REC *) MetisAlloc(sizeof(RESULT_REC));
			pRec2 = pRec2->pNext;
		}

		pRec2->value = (VALUE_UNION *) MetisAlloc(sizeof(VALUE_UNION) * cols);
		memcpy((char *) pRec2->value, (char *) pRec1->value,
				sizeof(VALUE_UNION) * cols);
		pRec2->pNext = 0;

		pRec1 = pRec1->pNext;
	}

	m_CSectStatic.Unlock();

	return pNew;
}

void CMRStaticResult::DeleteResRec(RESULT_REC *p, short cnt) {
	if (!p)
		return;

	DeleteResRec(p->pNext, cnt);

	if (p->value)
		MetisFree(p->value, sizeof(VALUE_UNION) * cnt);

	MetisFree(p, sizeof(RESULT_REC));

}

void CMRStaticResult::DelRuleResult(long id) {
	RULE_RESULT *pSelected, *pPrev, *pDelete;

// printf("DelRuleResult : %d\n", id );

	if (id == 0)
		return;

	pSelected = m_pRuleResult;
	pPrev = 0;
	while (pSelected) {
		if (id == pSelected->ruleId) {
			if (pPrev)
				pPrev->pNext = pSelected->pNext;
			else
				m_pRuleResult = pSelected->pNext;

			pDelete = pSelected;
			pSelected = pSelected->pNext;

			DeleteResRec(pDelete->resRecord, pDelete->pRuleNode->retCnt);
			MetisFree(pDelete, sizeof(RULE_RESULT));
		} else {
			pPrev = pSelected;
			pSelected = pSelected->pNext;
		}
	}
}

void CMRStaticResult::DelRuleResultAll() {
	RULE_RESULT *pDelete;

	while (m_pRuleResult) {
		pDelete = m_pRuleResult;
		m_pRuleResult = m_pRuleResult->pNext;

		DeleteResRec(pDelete->resRecord, pDelete->pRuleNode->retCnt);
	}
}

void * CMRStaticResult::MetisAlloc(int sz) {
	void *mm;

	if (sz == 0) {
		printf("CMRStaticResult:MetisAlloc size(0) exist\n");
		return 0;
	}

#ifdef WIN32
	sz = (sz + 64 - (sz % 64));
#endif

	mm = (void *) malloc(sz);
	if (!mm) {
		printf("current memory size = %d(+%d)\n", m_AllocSize, sz);
		throw new CMTException(SYSERR_MEMORYALLOCATEFAILURE,
				ERRMSG_MEMORYALLOCATEFAILURE_14);
	}

	m_AllocSize += sz;

	return mm;
}

void CMRStaticResult::MetisFree(void *p) {
	if (p && p != (void*) -1)
		free(p);
}

void CMRStaticResult::MetisFree(void *p, int sz) {
	if (p && p != (void*) -1) {
		free(p);
		m_AllocSize -= sz;
	}
}
