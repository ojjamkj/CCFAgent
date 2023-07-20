#include    <string.h>
#include    <stdio.h>
#include    <stdlib.h>

#include    "MTUtil.h"
#include    "MRPGMEXP.h"
#include    "MRTOKEN.h"
#include    "MRBRINTR.h"
#include    "MRRUNNER.h"
#include    "MRERROR.h"
#include    "MTExcept.h"
#include    "MRGBLVAR.h"
#include    "MRCRINTR.h"
#include    "MREXPRES.h"

#ifdef _TESTER
#include    "BRMStart.h"
#endif

#ifdef _MSG_KO
#define   MSG_NOTFOUND_ITEM     "아이템 (%d)을 찾을 수 없습니다."
#define   MSG_NOTDEFINE_PARM_01    "\n       오류(01) : %d 번째 파라미터가 정의되지 않았습니다.\n"
#define   MSG_NOTDEFINE_PARM_02    "\n       오류(02) : %d 번째 파라미터가 정의되지 않았습니다.\n"
#define   MSG_NOTDEFINE_PARM_03    "\n       오류(03) : %d 번째 파라미터가 정의되지 않았습니다.\n"
#else
#define   MSG_NOTFOUND_ITEM     "Item (%d) not found."
#define   MSG_NOTDEFINE_PARM_01    "\n       Error(01) : %dth parm not defined.\n"
#define   MSG_NOTDEFINE_PARM_02    "\n       Error(02) : %dth parm not defined.\n"
#define   MSG_NOTDEFINE_PARM_03    "\n       Error(03) : %dth parm not defined.\n"
#endif

CMRBInterpreter::CMRBInterpreter(CMRDataList *pDataList, RULE_LIST *pRuleNode,
		CMRParam *pParm) {
	m_pDataList = pDataList;
	m_pRuleNode = pRuleNode;
	m_retCnt = m_pRuleNode->retCnt;
	m_LocalParm = pParm;

	m_lCode = 0;
	m_lpszMessage = "";

}

CMRBInterpreter::~CMRBInterpreter() {
	Delete();
//	if( m_lpszMessage )  delete[] m_lpszMessage;
}

long CMRBInterpreter::GetECode(void) const {
	return m_lCode;
}

const char* CMRBInterpreter::GetEMessage(void) const {
	return m_lpszMessage;
}

void CMRBInterpreter::Delete(void) {

}

RESULT_REC * CMRBInterpreter::BuildResult() {
	BRULE_DATA *pBRule;
	QBRULE_SUBSTR *pSubStr;
	char RuleName[200+1];  // 2017-09-27 이전 60자리 임 DB는 200임.
	char tempStr[200+1];

	RuleName[0] = 0;

//	pBRule = (BRULE_DATA *) m_pRuleNode->pVers;
	pBRule = (BRULE_DATA *) m_pDataList->getVersionNode(m_pRuleNode);

//printf("pBRule : %d\n", pBRule );

	if (!pBRule)
		return 0;

	pSubStr = pBRule->BranchSubString;
//printf("pSubStr : %s\n", pSubStr );
	if (!pSubStr)
		return 0;

	while (pSubStr) {
//printf("Type(%d)  Info(%s)\n", pSubStr->tokType, pSubStr->tokInfo );

		switch (pSubStr->tokType) {
		case TOKENTYPE_GLOBAL:
			GetGlobalItemValue(pSubStr->ItemCd, pSubStr->tokInfo, tempStr);
			strcat(RuleName, tempStr);
			break;

		case TOKENTYPE_RULE:
			GetRuleValue(pSubStr->ItemCd, pSubStr->tokInfo, tempStr);
			strcat(RuleName, tempStr);
//			strcat( RuleName, pSubStr->tokInfo );
			break;

		case TOKENTYPE_LOCAL:
			GetParmValue(pSubStr->ItemCd, tempStr);
			strcat(RuleName, tempStr);
			break;

		default:
			strcat(RuleName, pSubStr->tokInfo);
		}

		pSubStr = pSubStr->pNext;
	}

//printf("RuleName = %s\n", RuleName );

#ifdef _TESTER
	m_pWorker->SuspendWork( _DBG_STEP_BRA, 0, 0, RuleName );
#endif

	m_pRuleNode->retType[m_pRuleNode->retCnt] = 0;

	CMRRunner Runner(RuleName, (char *) m_pRuleNode->retType, m_pDataList,
			m_LocalParm);
//    CMRRunner  Runner( RuleName, (char *) 0, m_pDataList, m_LocalParm );

//printf("Runner.m_bRuleExist = %d\n", Runner.m_bRuleExist );

	if (Runner.m_bRuleExist == 1) {
//printf(" aft Call m_bRuleExist \n");

		char tgtRetTypes[128];
		RESULT_REC *pLast;
		VALUE_UNION *pTemp;

		RESULT_REC *p = Runner.getResultList();

		memset(tgtRetTypes, 0, 128);

//printf(" bef Call chkReturnTypeDiff \n");
		int cmp = Runner.chkReturnTypeDiff(tgtRetTypes);
//printf(" aft Call chkReturnTypeDiff \n");
//printf(" %d %d %s\n", cmp, p, tgtRetTypes );

		if (cmp == 0 || p == 0)
			return p;
//printf("here\n");

		pResultList = 0;

		while (p) {
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
					sizeof(VALUE_UNION) * m_retCnt);

			pTemp = p->value;

			for (int i = 0; i < m_retCnt; i++) {
				if (tgtRetTypes[i] == m_pRuleNode->retType[i])
					memcpy((char *) &pLast->value[i], (char *) &pTemp[i],
							sizeof(VALUE_UNION));
				else {
					TYPE_VALUE Tval;
					Tval.dataType = tgtRetTypes[i];
					memcpy((char*) &Tval.value, (char *) &pTemp[i],
							sizeof(VALUE_UNION));

					matchingReturnTypes(&Tval, m_pRuleNode->retType[i]);

					memcpy((char *) &pLast->value[i], (char *) &Tval.value,
							sizeof(VALUE_UNION));
				}
			}

			p = p->pNext;
		}

		return pResultList;
	}

	if (!pBRule->result)
		return 0;

#ifdef _TESTER
	m_pWorker->SuspendWork( _DBG_STEP_NRF, 0, 0, "" );
#endif

	if (pBRule->resGbn == 0) { //  20090322 : 1 이었음
		RESULT_REC *pNewResult;

//printf(" pBRule->resGbn == 1 bef \n");
		CMRCInterpreter CInterpreter(0, m_pDataList, m_pRuleNode, m_LocalParm);

		pNewResult = CInterpreter.OneResult((CRULE_STMT*) pBRule->result);

//printf(" pBRule->resGbn == 1 aft \n");

		return pNewResult;
	}

//	printf(" pBRule->resGbn == 2 \n");

	if (pBRule->resGbn == 1) { //  20090322 : 2 이었음 : Constant
		RESULT_REC *pNewResult;
		int sz = sizeof(VALUE_UNION) * m_pRuleNode->retCnt;

		pNewResult = (RESULT_REC *) m_pDataList->MRMemAlloc1(
				sizeof(RESULT_REC));
		pNewResult->value = (VALUE_UNION *) m_pDataList->MRMemAlloc1(sz);
		memcpy((char *) pNewResult->value, (char *) pBRule->result, sz);
//		pNewResult->value = pBRule->result;
		pNewResult->pNext = 0;

		return pNewResult;
	}

	/*  Alternate Result가 Constant 아닐때....  */
	return (RESULT_REC *) 0;
}

int CMRBInterpreter::matchingReturnTypes(TYPE_VALUE *pTval, char tp) {

	switch (tp) {

	case TOKENTYPE_CONST_C:
		memset(pTval->value.strVal, 0x00,STR_VALUE_SIZE);
		if (pTval->value.pLStr) {
			strncpy(pTval->value.strVal, pTval->value.pLStr, STR_VALUE_SIZE);
			pTval->value.strVal[STR_VALUE_SIZE] = 0;
		} else
			pTval->value.strVal[0] = 0;

		break;

	case TOKENTYPE_CONST_L:
		char *p = (char *) m_pDataList->MRMemAlloc1(STR_VALUE_SIZE + 1);

		if (pTval->dataType == TOKENTYPE_CONST_C)
			strcpy(p, pTval->value.strVal);

		pTval->value.pLStr = p;
		break;
	}

	pTval->dataType = tp;

	return 1;
}

void CMRBInterpreter::GetGlobalItemValue(int ItemCd, char *tokInfo,
		char *itemValue) {
	TYPE_VALUE Value;
	int col;

	if ((col = m_pDataList->getItemColumn(ItemCd)) < 0) {
		char msg[128];
//		char rName[101];

		itemValue[0] = 0;

//		pCMRLoadData->getItemName( ItemCd, rName );

		//sprintf(msg, "Item (%d) not found.", ItemCd);
		sprintf(msg, MSG_NOTFOUND_ITEM, ItemCd);

		throw new CMTException(UERR_ITEMNOTFOUND, msg);
	}

	Value.dataType = TOKENTYPE_GLOBAL;
	Value.value.RuleItem.i_row = 0;
	Value.value.RuleItem.i_col = col;

	if (tokInfo > 0) {
		Value.value.RuleItem.i_row = atoi(tokInfo);
	}

//  여기 고칠것
	m_pDataList->getItemValue(&Value);

	switch (Value.dataType) {
	case 1:
		sprintf(itemValue, "%lf", Value.value.dblVal);
		break;
	case 2:
		sprintf(itemValue, "%s", Value.value.strVal);
		break;
	case 3:
		sprintf(itemValue, "%ld", Value.value.longVal);
		break;
	case 4:
		sprintf(itemValue, "%s", Value.value.pLStr);
	}

	rtrim(itemValue);
}

void CMRBInterpreter::GetRuleValue(int RuleCd, char *pTokInfo,
		char *itemValue) {
	RULE_RESULT *pRuleResult;
	TYPE_VALUE Value;

	pRuleResult = 0;

	if (!pRuleResult) { //  처음 사용될때
		pRuleResult = m_pDataList->GetRuleResult(RuleCd, (void*) 0); // 아직 룰에 Parm을 쓰지 못함
	}

	if (!pRuleResult) {
		CMRRunner Runner(RuleCd, "", m_pDataList, m_LocalParm);
		pRuleResult = Runner.getRuleResultPtr();
	}

	if (pRuleResult->resRecord == 0) {
		char msg[100];
		sprintf(msg, "%s : %d", EMSG_NORESULTERR, RuleCd);

		throw new CMTException(UERR_NORESULTERR, msg);
	}

	Value.dataType = TOKENTYPE_RULE;
	Value.value.RuleItem.ptr = pRuleResult;
	Value.value.RuleItem.i_col = 0;
	Value.value.RuleItem.i_row = 0;

	if (pTokInfo) {
		//  Col 두자리 + Index ( #n, 99999 )
		char tokInfo[10];

		tokInfo[0] = pTokInfo[0];
		tokInfo[1] = pTokInfo[1];
		tokInfo[2] = 0;
		Value.value.RuleItem.i_col = atoi(tokInfo);
		Value.value.RuleItem.i_row = atoi(pTokInfo + 2);
	}

//  여기 고칠것
	m_pDataList->GetRuleValue(&Value);

	switch (Value.dataType) {
	case 1:
		sprintf(itemValue, "%lf", Value.value.dblVal);
		break;
	case 2:
		sprintf(itemValue, "%s", Value.value.strVal);
		break;
	case 3:
		sprintf(itemValue, "%ld", Value.value.longVal);
		break;
	case 4:
		sprintf(itemValue, "%s", Value.value.pLStr);
	}
}

void CMRBInterpreter::GetParmValue(int pParmIdx, char *itemValue) {
	TYPE_VALUE Value;

	if (!m_LocalParm) {
		//printf("\n       Error : %dth parm not defined.\n", pParmIdx);
		printf(MSG_NOTDEFINE_PARM_01, pParmIdx);
		throw new CMTException(UERR_PARMNFOUND, EMSG_PARMNFOUND);
	}

	m_LocalParm->getParmValue(pParmIdx, &Value);

	switch (Value.dataType) {
	case 1:
		sprintf(itemValue, "%lf", Value.value.dblVal);
		break;
	case 2:
		sprintf(itemValue, "%s", Value.value.strVal);
		break;
	case 3:
		sprintf(itemValue, "%ld", Value.value.longVal);
		break;
	case 4:
		sprintf(itemValue, "%s", Value.value.pLStr);
	}
}
