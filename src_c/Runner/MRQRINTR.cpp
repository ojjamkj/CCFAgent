#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <memory.h>

#include    "MItem.h"

#include    "MRQRINTR.h"
#include    "MRRUNNER.h"
#include    "MRTOKEN.h"
#include    "MRERROR.h"
#include    "MTExcept.h"
#include    "MRPGMEXP.h"
#include    "BRAgtDef.h"

#ifdef _TESTER
#include  "BRMFile.h"
#include  "BRMStart.h"
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
extern char _DEBUG_YN[2];
CMRQInterpreter::CMRQInterpreter(CMRDataList *pDataList, RULE_LIST *pRuleNode,
		CMRParam *pParm) {
	m_pDataList = pDataList;
	m_pRuleNode = pRuleNode;
	m_retCnt = m_pRuleNode->retCnt;
	m_LocalParm = pParm;
	 
	m_lCode = 0;
	m_lpszMessage = "";

	if (m_LocalParm == NULL) {
	//	printf("CMRQInterpreter  01 m_pParm=NULL \n");
	}
	else {
	//	printf("CMRQInterpreter  01 m_pParm=not NULL cnt=[%d] val-[%s] \n", m_LocalParm->m_ParmCnt, m_LocalParm->m_pParmVal);
		/* 
		TYPE_VALUE *valTemp = 0;
		TYPE_VALUE *pParmVal = 0;
		printf("CMRQInterpreter  02 m_pParm \n");
		printf("CMRQInterpreter  02 m_pParmCount [%d] \n", m_LocalParm->m_ParmCnt);

		pParmVal = getRuleParmValues(p, sKey, &parmCnt);

		m_LocalParm->getParmValue(1, &valTemp);
	//	printf("CMRQInterpreter  03 m_pParm \n");
		if (valTemp!=NULL) {
			printf("CMRQInterpreter  04 m_pParm \n");
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
			default:
				printf("DATA_TYPE_LONG=  null \n ");
				break;
			}
		}
		*/
	//	printf("CMRQInterpreter  004-01 m_pParm=NOT NULL \n");
	}

}

CMRQInterpreter::~CMRQInterpreter() {
	Delete();
//	if( m_lpszMessage )  delete[] m_lpszMessage;
}

long CMRQInterpreter::GetECode(void) const {
	return m_lCode;
}

const char* CMRQInterpreter::GetEMessage(void) const {
	return m_lpszMessage;
}

void CMRQInterpreter::Delete(void) {

}

void CMRQInterpreter::convQueryString(char *org, char *rep) {
	 

}

RESULT_REC * CMRQInterpreter::BuildResult() { 
	
	return NULL;
}

void CMRQInterpreter::GetGlobalItemValue(int ItemCd, char *tokInfo,
		char *itemValue) {
	unsigned char col;
	long row;

	if ((col = m_pDataList->getItemColumn(ItemCd)) < 0) {
		//printf("\n       Error : Item %d not found.\n", ItemCd);
		printf(MSG_NOTFOUND_ITEM, ItemCd);
		throw new CMTException(UERR_ITEMNOTFOUND, EMSG_ITEMNOTFOUND);
	}

	if (tokInfo > 0) {
		row = atoi(tokInfo);
	} else {
		row = 0;
	}

//  여기 고칠것
	m_pDataList->getItemValuesByStr(col, row, itemValue);
}

void CMRQInterpreter::GetLocalItemValue(int ItemCd, char *tokInfo,char *itemValue) {
	TYPE_VALUE Value;

	if (m_pDataList->m_HAHA[0]) {
		sprintf(itemValue, "\'%s\'", m_pDataList->m_HAHA);
		return;
	}

	if (!m_LocalParm) {
		//printf("\n       Error : %dth parm not defined.\n", ItemCd);
		printf(MSG_NOTDEFINE_PARM_03, ItemCd);
		throw new CMTException(UERR_PARMNFOUND, EMSG_PARMNFOUND);
	}

	m_LocalParm->getParmValue(ItemCd, &Value);
	if (Value.dataType == DATA_TYPE_DOUBLE){  //   기존 1
		sprintf(itemValue, "%lf", Value.value.dblVal);
		
	}else if (Value.dataType == 2)
//20100603	   sprintf(itemValue, "\'%s\'", Value.value.strVal);
		sprintf(itemValue, "%s", Value.value.strVal);
	else if (Value.dataType == DATA_TYPE_LONG) //   기존 3
		sprintf(itemValue, "%ld", Value.value.longVal);
	else if (Value.dataType == 4) {
//20100603	   sprintf(itemValue, "\'%s\'", Value.value.pLStr);
		sprintf(itemValue, "%s", Value.value.pLStr);
	}
	else if (Value.dataType == 5) {
//20100603	   sprintf(itemValue, "\'%s\'", Value.value.pLStr);
		sprintf(itemValue, "%d", Value.value.boolVal);
	}
}

/*
 void	CMRQInterpreter::GetRuleValue( int RuleCd, char *pTokInfo, char *RuleValue )
 {
 RULE_RESULT  *pRuleResult;
 TYPE_VALUE   Value;

 pRuleResult = 0;

 CMRExpression Expression( this, m_pDataList, m_pRuleNode, m_LocalParm );

 Expression.CalcResultExpress( pExp->pNext, &Value );

 try {
 m_pDataList->GetRuleValue( &Value );
 } catch( CMTException *e ) {
 e->Delete();
 throw new CMTException( UERR_RESULTOUTOFBIND, EMSG_RESULTOUTOFBIND );
 }


 switch ( Value.dataType ) {
 case 1 :
 sprintf(RuleValue, "%lf", Value.value.dblVal);
 break;
 case 2 :
 sprintf(RuleValue, "\'%s\'", Value.value.strVal);
 break;
 case 3 :
 sprintf(RuleValue, "%ld", Value.value.longVal);
 break;
 case 4 :
 sprintf(RuleValue, "\'%s\'", Value.value.pLStr );
 break;
 default :
 RuleValue[0] = 0;
 }
 }
 */

void CMRQInterpreter::GetRuleValue(int RuleCd, char *pTokInfo,
		char *RuleValue) {
	RULE_RESULT *pRuleResult;
	TYPE_VALUE Value;

	pRuleResult = 0;

//	if ( m_pParent ) {  //  현재룰에서 실행된 룰은 재실행하지 않는다. 
//		pRuleResult = m_pParent->isAlreadyRun( p->ruleItem_Id, p->tokInfo );
//	}
//printf("pTokInfo = %s\n", pTokInfo );
	if (!pRuleResult) { //  현재룰에서 처음 사용될때
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
//printf("pTokInfo ===> %s!!\n", pTokInfo );

	if (pTokInfo) {
		char tokInfo[10];
		char *p = pTokInfo;

		strncpy(tokInfo, p, 2);
		tokInfo[2] = 0;
		Value.value.RuleItem.i_col = atoi(tokInfo);
		p += 2;

		Value.value.RuleItem.i_row = atoi(p);
	}

//  여기 고칠것
	//20100603 추가함
//	Value.value.RuleItem.i_row = 1;

	if (Value.value.RuleItem.i_row > 0) {

		m_pDataList->GetRuleValue(&Value);
 

		switch (Value.dataType) {
		case 1:
			sprintf(RuleValue, "%lf", Value.value.dblVal);
			break;
		case 2:
//20100603				sprintf(RuleValue, "\'%s\'", Value.value.strVal);
			sprintf(RuleValue, "%s", Value.value.strVal);
			break;
		case 3:
			sprintf(RuleValue, "%ld", Value.value.longVal);
			break;
		case 4:
//20100603				sprintf(RuleValue, "\'%s\'", Value.value.pLStr );
			sprintf(RuleValue, "%s", Value.value.pLStr);
			break;
        case 5:
            sprintf(RuleValue, "%d", Value.value.boolVal);
            break;
		default:
			RuleValue[0] = 0;
		}
	} else {
		m_pDataList->GetRuleValueAll(&Value, RuleValue);
	}
}
