#include    <string.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <ctype.h>

#include	"MRRUNNER.h"
#include    "MRTOKEN.h"
#include    "MRPARSE.h"
#include    "MRCALC.h"
#include    "MRSFUNC.h"
#include    "MRERROR.h"
#include    "MTExcept.h"
#include    "MRARRFN.h"
#include    "MREXPRES.h"

#ifdef _RUNNER
#include    "BREXLog.h"
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

extern long rId_RuleName;
extern long rId_DAMBOLIST;
extern long rId_DAMBOGM;
extern long rConf_DAMBO_POS;
extern long rConf_DAMBO_LENG;

unsigned long calc_result_rc;
char calc_result_msg[128];

CMRExpression::CMRExpression(CMRRunner *pParent, CMRDataList *pDataList,
		RULE_LIST *pRuleNode, CMRParam* pParm) {
	m_pParent = pParent;

	m_pDataList = pDataList;
	m_pRuleNode = pRuleNode;
	pCParam = pParm;

	opndStack = 0;
	optrStack = 0;

//	m_pMemPos = m_pDataList->getMemPtr3();

}

CMRExpression::~CMRExpression() {
//	m_pDataList->setMemPtr3( m_pMemPos );
}

long CMRExpression::GetECode(void) const {
	return m_lCode;
}

const char* CMRExpression::GetEMessage(void) const {
	return m_lpszMessage;
}

void CMRExpression::PushOperand(TYPE_VALUE *Value) {
	OPERAND_STACK *pOpnd;

	pOpnd = (OPERAND_STACK *) m_pDataList->MRMemAlloc3(sizeof(OPERAND_STACK));

	memcpy((char *) &pOpnd->tVal, (char *) Value, sizeof(TYPE_VALUE));

	pOpnd->pNext = opndStack;

	opndStack = pOpnd;

}

OPERAND_STACK * CMRExpression::PopOperand() {
	OPERAND_STACK *pOpnd;

	if (opndStack == 0)
		return NULL;

	pOpnd = opndStack;
	opndStack = opndStack->pNext;

	pOpnd->pNext = 0;

	return (pOpnd);
}

void CMRExpression::PushOperator(unsigned char opType) {
	OPERATOR_STACK *pOptr;

	pOptr = (OPERATOR_STACK *) m_pDataList->MRMemAlloc3(sizeof(OPERATOR_STACK));

	pOptr->pNext = optrStack;
	pOptr->opType = opType;

	optrStack = pOptr;
}

unsigned char CMRExpression::PopOperator() {
	OPERATOR_STACK *pOptr;
	unsigned char rc;

	if (optrStack == 0)
		return 0;

	pOptr = optrStack;
	rc = pOptr->opType;

	optrStack = optrStack->pNext;

	return rc;
}

/***********************************************
 * Parm : CRULE_STMT *p : Condition Statement
 ************************************************/

TYPE_VALUE * CMRExpression::CalcExpression(CRULE_STMT *p) {
	TYPE_VALUE Value;
	char  m_TraceMsg[1024];
	char funcLeft = 0;

	m_pMemPos = m_pDataList->getMemPtr3();

	opndStack = 0;
	optrStack = 0;

	m_LeftPcnt = 0;

	m_lCode = 0;
	m_lpszMessage = (char *) "";
	/*
	 #ifdef _PARSER
	 if ( p->tokType == TOKENTYPE_BREAK || p->tokType == TOKENTYPE_CONTINUE ) {
	 if ( p->pNext ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR );

	 return (TYPE_VALUE *) 0;
	 }
	 #endif
	 */

	
	while (p) {
		if (p->tokType > TOKENFUNC_END) {
			CalcFunction(p);

			if (p->tokType == TOKENFUNC_DISPLAY)
				return (TYPE_VALUE *) 0;

			p = p->pNext;
			continue;
		}

		switch (p->tokType) {
//  발생하지 않을듯...
//			case TOKENTYPE_BREAK :
//			case TOKENTYPE_CONTINUE :
//				throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR );

		case TOKENTYPE_NONE:
			break;
		case TOKENTYPE_GLOBAL:
			getGlobalItemValue(p, &Value);
			PushOperand(&Value);
			break;
///////////////////////////////////////////////////////////////
#ifndef _PARSER
		case TOKENTYPE_LOCAL:
			if (m_pDataList->m_HAHA[0]) {
				memset(Value.value.strVal,0x00, STR_VALUE_SIZE );
				Value.dataType = TOKENTYPE_CONST_C;
				strcpy(Value.value.strVal, m_pDataList->m_HAHA);
				Value.value.strVal[STR_VALUE_SIZE]=0;
				PushOperand(&Value);
				break;
			}

			if (!pCParam) {/*
				printf("\n       Error : %dth parm not defined.\n",
						p->uni.longVal);*/
				printf(MSG_NOTDEFINE_PARM_02,
						p->uni.longVal);
				throw new CMTException(UERR_PARMNFOUND, EMSG_PARMNFOUND);
			}

			pCParam->getParmValue(p->uni.longVal, &Value);
			PushOperand(&Value);
			break;
#endif
///////////////////////////////////////////////////////////////				
		case TOKENTYPE_RULE:
			getRuleResultValue(p, &Value);
//printf("aft getRuleResultValue\n");
			PushOperand(&Value);
			break;
		case TOKENTYPE_CONST_F:
		case TOKENTYPE_CONST_C:
		case TOKENTYPE_CONST_N:
		case TOKENTYPE_CONST_L:
		case TOKENTYPE_CONST_B:
			ConvertExp2Value(p, &Value);
			PushOperand(&Value);
			break;
		case TOKENTYPE_CONST_V:
			Value.dataType = TOKENTYPE_CONST_V;
			PushOperand(&Value);
			break;
		case TOKENTYPE_LEFTP:
			PushOperator(p->tokType);
			m_LeftPcnt++;
			break;
		case TOKENTYPE_RIGHTP:
			CalcUntilLeftP();
			m_LeftPcnt--;
			break;
		case TOKENTYPE_INDEX0:
		case TOKENTYPE_INDEX1:
		case TOKENTYPE_INDEX2:
		case TOKENTYPE_INDEX3:
			CalRequestIndex(p->tokType);
			break;

		case TOKENTYPE_IN:
		case TOKENTYPE_NIN:
		case TOKENTYPE_ARREQUAL:
		case TOKENTYPE_SUBSET:
			CalcUntilGreater (TOKENTYPE_IN);

			Calc_Compare(p->tokType, p);

			break;

		case TOKENTYPE_MATCH:
			CalcUntilGreater(TOKENTYPE_MATCH);
			Calc_Match2(p->tokType, p); 
//			Calc_Match(p);

/*
			CalcUntilGreater (TOKENTYPE_MATCH);
			Calc_Match(p);
*/
			break;
/*		case TOKENTYPE_NOT_MATCH: //2020-06-09
			CalcUntilGreater (TOKENTYPE_NOT_MATCH);

			Calc_Not_Match(p);

			break;*/
		case TOKENTYPE_BETWEEN:
			CalcUntilGreater (TOKENTYPE_BETWEEN);

			Calc_Between(p);

			break;

		default:
			CalcUntilGreater(p->tokType);

			//***  긴 논리식의 빠른처리를 위해 추가   **************
#ifndef _PARSER
			if (!opndStack) {
				PushOperator(p->tokType);
				break;
			}

			if (opndStack->tVal.dataType == DATA_TYPE_BOOL && m_LeftPcnt == 0) {
				if (p->tokType == TOKENTYPE_AND) {
					if (!opndStack->tVal.value.boolVal) {

						m_pDataList->setMemPtr3(m_pMemPos);

						return &opndStack->tVal;
					}
				} else if (p->tokType == TOKENTYPE_OR) {
					if (opndStack->tVal.value.boolVal) {

						m_pDataList->setMemPtr3(m_pMemPos);

						return &opndStack->tVal;
					}
				}
			}
#endif
			//*****************************************************

			PushOperator(p->tokType);
			break;
		}
		p = p->pNext;
	}

	if (!opndStack)
		throw new CMTException(USERERR_GRAMMAR, EMSG_GRAMMAR_30);

	CalcUntilGreater (TOKENFUNC_END);

#ifdef _PARSER
	if ( opndStack->pNext ) {
		throw new CMTException(USERERR_GRAMMAR, EMSG_GRAMMAR_09 );
	}

	if ( optrStack ) {
		throw new CMTException(USERERR_GRAMMAR, EMSG_GRAMMAR_10 );
	}
#endif

	m_pDataList->setMemPtr3(m_pMemPos);

	return &opndStack->tVal;
}

void CMRExpression::CalcUntilLeftP() {
	unsigned char Optr;
	int cnt = 0;

	while (optrStack) {
		Optr = PopOperator();
		if (Optr == TOKENTYPE_LEFTP) {
			if (cnt > 0)
				return;
			if (opndStack)
				return;

#ifdef _PARSER
			throw new CMTException( USERERR_GRAMMAR, "GRAMMAR : Empty in parenthesis !" );
#endif
		}

		Calculate(Optr);
		cnt++;
	}

#ifdef _PARSER
	throw new CMTException( USERERR_GRAMMAR, "GRAMMAR : Too many right parenthesis !" );
#endif
}

void CMRExpression::CalcUntilGreater(unsigned char inOpType) {
	unsigned char Optr;

	if (!optrStack)
		return;
//printf("[CalcUntilGreater] begin : %d\n", optrStack->opType );

	while (optrStack) {
		if (optrStack->opType == TOKENTYPE_LEFTP)
			return;
		if ((optrStack->opType / 10) > (inOpType / 10))
			return;

		Optr = PopOperator();

		Calculate(Optr);
	}
//printf("[CalcUntilGreater] end : %d\n", optrStack );
}

void CMRExpression::Calc_Compare(int opt, CRULE_STMT *p) {
	bool b_Result = false;
	CRULE_STMT *pExp;

//printf("Calc_Compare : %d\n", opt );

#ifdef _PARSER	
	if ( ! opndStack )
	throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_11 );

	if ( opndStack->tVal.dataType > TOKENTYPE_RULE )
	throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_12 );

	if ( ! p->pExpression ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_13 );

	pExp = p->pExpression->pNext;
	if ( ! pExp ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_14 );

	if ( pExp->tokType == TOKENTYPE_GLOBAL || pExp->tokType == TOKENTYPE_RULE ) {
		if ( pExp->pNext ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_15 );

		if ( p->pExpression->pExpression ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_16 );
	} else {
		pExp = p->pExpression;

		while ( pExp ) {
			if ( ! pExp->pNext ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_17 );
			if ( pExp->pNext->tokType > TOKENTYPE_CONST_V ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_18 );

			pExp = pExp->pExpression;
		}
	}
//printf("Calc_Compare : end.\n" );

#else

	TYPE_VALUE *expResult;

	CMRArrayFn *pArrayFn = new CMRArrayFn(m_pDataList);

	/* Compare source list  */
	if (opndStack->tVal.dataType == TOKENTYPE_GLOBAL) {
		if (opndStack->tVal.value.RuleItem.i_row == 0)
			pArrayFn->AddItemList(1, opndStack->tVal.value.RuleItem.i_col);
		else {
			m_pDataList->getItemValue(&opndStack->tVal);
			pArrayFn->AddValue(1, &opndStack->tVal);
		}
	}

	if (opndStack->tVal.dataType == TOKENTYPE_RULE) {
		if (opndStack->tVal.value.RuleItem.i_row == 0)
			pArrayFn->AddRuleList(1,
					(RULE_RESULT*) opndStack->tVal.value.RuleItem.ptr,
					opndStack->tVal.value.RuleItem.i_col);
		else {
			m_pDataList->GetRuleValue(&opndStack->tVal);
			pArrayFn->AddValue(1, &opndStack->tVal);
		}
	}

	if (opndStack->tVal.dataType < TOKENTYPE_CONST_V) {
		pArrayFn->AddValue(1, &opndStack->tVal);
	}

	/* Compare pool  */
	pExp = p->pExpression->pNext;

	if (pExp->tokType == TOKENTYPE_GLOBAL || pExp->tokType == TOKENTYPE_RULE) {

		if (opt == TOKENTYPE_MATCH) {
			char msg[100];
			sprintf(msg, "%s (MATCH)", EMSG_GRAMMAR_19);
			throw new CMTException(USERERR_GRAMMAR, msg);
		}
/*		if (opt == TOKENTYPE_NOT_MATCH) {
			char msg[100];
			sprintf(msg, "%s (NOT MATCH)", EMSG_GRAMMAR);
			throw new CMTException(USERERR_GRAMMAR, msg);
		}
*/
		CMRExpression Expression(m_pParent, m_pDataList, m_pRuleNode, pCParam);

		expResult = Expression.CalcExpression(pExp);

		if (!expResult) {
//			delete Expression;
			delete pArrayFn;

			throw new CMTException(USERERR_GRAMMAR, EMSG_GRAMMAR_20);
		}

		if (expResult->dataType == TOKENTYPE_RULE)
			pArrayFn->AddRuleList(2,
					(RULE_RESULT*) expResult->value.RuleItem.ptr,
					expResult->value.RuleItem.i_col);
		else if (expResult->dataType == TOKENTYPE_GLOBAL)
			pArrayFn->AddItemList(2, expResult->value.RuleItem.i_col);

//		delete Expression;

		switch (opt) {
		case TOKENTYPE_IN:
			b_Result = pArrayFn->isInclude();
			break;
		case TOKENTYPE_NIN:
			b_Result = !pArrayFn->isInclude();
			break;
		case TOKENTYPE_ARREQUAL:
			b_Result = pArrayFn->isEqualToEach();
			break;
		case TOKENTYPE_SUBSET:
			b_Result = pArrayFn->isSubset();
		}
	} else {
		switch (opt) {
		case TOKENTYPE_IN:
			b_Result = pArrayFn->isIncludeList(p->pExpression);
			break;
		case TOKENTYPE_NIN:
			b_Result = !pArrayFn->isIncludeList(p->pExpression);
			break;
		case TOKENTYPE_ARREQUAL:
			b_Result = pArrayFn->isEqualToList(p->pExpression);
			break;
		case TOKENTYPE_SUBSET:
			b_Result = pArrayFn->isSubsetList(p->pExpression);
		}
	}

	delete pArrayFn;

#endif

	opndStack->tVal.dataType = DATA_TYPE_BOOL;
	opndStack->tVal.value.boolVal = b_Result;

}


void CMRExpression::Calc_Match2(int opt, CRULE_STMT *p) {
	bool b_Result = false;
	CRULE_STMT *pExp;

	//printf("Calc_Compare : %d\n", opt );

#ifdef _PARSER	
	if (!opndStack)
		throw new CMTException(USERERR_GRAMMAR, EMSG_GRAMMAR_31);

	if (opndStack->tVal.dataType > TOKENTYPE_RULE)
		throw new CMTException(USERERR_GRAMMAR, EMSG_GRAMMAR_32);

	if (!p->pExpression) throw new CMTException(USERERR_GRAMMAR, EMSG_GRAMMAR_33);

	pExp = p->pExpression->pNext;
	if (!pExp) throw new CMTException(USERERR_GRAMMAR, EMSG_GRAMMAR_34);

	if (pExp->tokType == TOKENTYPE_GLOBAL || pExp->tokType == TOKENTYPE_RULE) {
		if (pExp->pNext) throw new CMTException(USERERR_GRAMMAR, EMSG_GRAMMAR_35);

		if (p->pExpression->pExpression) throw new CMTException(USERERR_GRAMMAR, EMSG_GRAMMAR_36);
	}
	else {
		pExp = p->pExpression;

		while (pExp) {
			if (!pExp->pNext) throw new CMTException(USERERR_GRAMMAR, EMSG_GRAMMAR_37);
			if (pExp->pNext->tokType > TOKENTYPE_CONST_V) throw new CMTException(USERERR_GRAMMAR, EMSG_GRAMMAR_38);

			pExp = pExp->pExpression;
		}
	}
	//printf("Calc_Compare : end.\n" );

#else

	TYPE_VALUE *expResult;

	CMRArrayFn *pArrayFn = new CMRArrayFn(m_pDataList);

	/* Compare source list  */
	if (opndStack->tVal.dataType == TOKENTYPE_GLOBAL) {
		if (opndStack->tVal.value.RuleItem.i_row == 0)
			pArrayFn->AddItemList(1, opndStack->tVal.value.RuleItem.i_col);
		else {
			m_pDataList->getItemValue(&opndStack->tVal);
			pArrayFn->AddValue(1, &opndStack->tVal);
		}
	}

	if (opndStack->tVal.dataType == TOKENTYPE_RULE) {
		if (opndStack->tVal.value.RuleItem.i_row == 0)
			pArrayFn->AddRuleList(1,
				(RULE_RESULT*)opndStack->tVal.value.RuleItem.ptr,
				opndStack->tVal.value.RuleItem.i_col);
		else {
			m_pDataList->GetRuleValue(&opndStack->tVal);
			pArrayFn->AddValue(1, &opndStack->tVal);
		}
	}

	if (opndStack->tVal.dataType < TOKENTYPE_CONST_V) {
		pArrayFn->AddValue(1, &opndStack->tVal);
	}

	/* Compare pool  */
	pExp = p->pExpression->pNext;

	if (pExp->tokType == TOKENTYPE_GLOBAL || pExp->tokType == TOKENTYPE_RULE) {
		CMRExpression Expression(m_pParent, m_pDataList, m_pRuleNode, pCParam);

		expResult = Expression.CalcExpression(pExp);

		if (!expResult) {
			//			delete Expression;
			delete pArrayFn;

			throw new CMTException(USERERR_GRAMMAR, EMSG_GRAMMAR_118);
		}

		if (expResult->dataType == TOKENTYPE_RULE)
			pArrayFn->AddRuleList(2,
				(RULE_RESULT*)expResult->value.RuleItem.ptr,
				expResult->value.RuleItem.i_col);
		else if (expResult->dataType == TOKENTYPE_GLOBAL)
			pArrayFn->AddItemList(2, expResult->value.RuleItem.i_col);

//		//		delete Expression;
		 
		b_Result = pArrayFn->isMatch();
/*		switch (opt) {
		case TOKENTYPE_IN:
			b_Result = pArrayFn->isMatch();
			break;
		case TOKENTYPE_NIN:
			b_Result = !pArrayFn->isInclude();
			break;
		case TOKENTYPE_ARREQUAL:
			b_Result = pArrayFn->isEqualToEach();
			break;
		case TOKENTYPE_SUBSET:
			b_Result = pArrayFn->isSubset();
		}*/
	}
	else {
		b_Result = pArrayFn->isMatchList(p->pExpression);
/*
		switch (opt) {
		case TOKENTYPE_IN:
			b_Result = pArrayFn->isIncludeList(p->pExpression);
			break;
		case TOKENTYPE_NIN:
			b_Result = !pArrayFn->isIncludeList(p->pExpression);
			break;
		case TOKENTYPE_ARREQUAL:
			b_Result = pArrayFn->isEqualToList(p->pExpression);
			break;
		case TOKENTYPE_SUBSET:
			b_Result = pArrayFn->isSubsetList(p->pExpression);
		}
		*/
	}

	delete pArrayFn;

#endif

	opndStack->tVal.dataType = DATA_TYPE_BOOL;
	opndStack->tVal.value.boolVal = b_Result;

}
void CMRExpression::Calc_Match(CRULE_STMT *p) {
	bool b_Result = false;
	CRULE_STMT *pExp;

#ifdef _PARSER	
	if ( ! opndStack ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_21 );
#endif

	/* Compare source list  */
	if (opndStack->tVal.dataType == TOKENTYPE_GLOBAL)
		m_pDataList->getItemValue(&opndStack->tVal);
	else if (opndStack->tVal.dataType == TOKENTYPE_RULE)
		m_pDataList->GetRuleValue(&opndStack->tVal);

	if (opndStack->tVal.dataType != TOKENTYPE_CONST_C) {
		char msg[100];
		sprintf(msg, "%s (MATCH)", EMSG_GRAMMAR_22);
		throw new CMTException(USERERR_GRAMMAR, msg);
	}

#ifdef _PARSER
	//  Parm이 존재하지 않으면 Error
//printf("p->pExpression - 1 \n");
	if ( ! p->pExpression ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_23 );
//printf("p->pExpression - 2 \n");
	if ( ! p->pExpression->pNext ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_24 );
//printf("p->pExpression->pExpression = %d\n", p->pExpression->pExpression );
	if ( p->pExpression->pExpression ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_25 );//  Parm하나여야함

	pExp = p->pExpression->pNext;
	//  Parm이 String이 아니면 Error
	if ( pExp->tokType != TOKENTYPE_CONST_C ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_26 );

	//  Matching Format이 지정되지 않으면 Error
	char *s = (char*) pExp->uni.tokInfo;
	if ( ! s ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_27 );
//printf("==> %s\n", s );
	if ( ! *s ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_28 );
	if ( strcmp(s,"1900-01-01") == 0 ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_29 );// ITEM이면 Error

#else

	/* Compare pool  */
	pExp = p->pExpression->pNext;

	bool b_TermFlag = false;
	char *s1, *s2, *search, *t;
	char token[50];
	int i;
	bool b_lastTokYn;

	s1 = opndStack->tVal.value.strVal;
	s2 = (char*) pExp->uni.tokInfo;

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
				if (*s2 == '*' || *s2 == '?' || *s2 == '%' || *s2 == '_' )
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

	if (*s2 && ( *s2 != '*' &&  *s2 != '%' ))
		b_Result = false;

#endif

	opndStack->tVal.dataType = DATA_TYPE_BOOL;
	opndStack->tVal.value.boolVal = b_Result;

}

void CMRExpression::Calc_Between(CRULE_STMT *p) {
	CRULE_STMT *pExp;
	TYPE_VALUE *expResult;
	TYPE_VALUE Opnd1, Opnd2;

#ifdef _PARSER	
	if ( ! opndStack )
	throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_39 );

	if ( opndStack->tVal.dataType > TOKENTYPE_RULE )
	throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_40 );
#endif

	if (opndStack->tVal.dataType == TOKENTYPE_RULE)
		m_pDataList->GetRuleValue(&opndStack->tVal);
	if (opndStack->tVal.dataType == TOKENTYPE_GLOBAL)
		m_pDataList->getItemValue(&opndStack->tVal);

	CMRExpression Expression(m_pParent, m_pDataList, m_pRuleNode, pCParam);

	//  FROM Expression
	pExp = p->pExpression;
#ifdef _PARSER
	if ( ! pExp ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_41 );
#endif

	expResult = Expression.CalcExpression(pExp->pNext);

#ifdef _PARSER
	if ( ! expResult ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_42 );
#endif

	memcpy((char*) &Opnd1, (char*) expResult, sizeof(TYPE_VALUE));

	if (Opnd1.dataType == TOKENTYPE_RULE)
		m_pDataList->GetRuleValue(&Opnd1);
	if (Opnd1.dataType == TOKENTYPE_GLOBAL)
		m_pDataList->getItemValue(&Opnd1);

	//  TO Expression
	pExp = pExp->pExpression;
#ifdef _PARSER
	if ( ! pExp ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_43 );
#endif

	expResult = Expression.CalcExpression(pExp->pNext);

#ifdef _PARSER
	if ( ! expResult ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_44 );
#endif

	memcpy((char*) &Opnd2, (char*) expResult, sizeof(TYPE_VALUE));

	if (Opnd2.dataType == TOKENTYPE_RULE)
		m_pDataList->GetRuleValue(&Opnd2);
	if (Opnd2.dataType == TOKENTYPE_GLOBAL)
		m_pDataList->getItemValue(&Opnd2);

#ifdef _PARSER
	//  Parm이 2개 이상이면 에러
	if ( pExp->pExpression ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_45 );

	if ( opndStack->tVal.dataType < TOKENTYPE_CONST_F || opndStack->tVal.dataType > TOKENTYPE_CONST_N) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_46 );
	if ( Opnd1.dataType < TOKENTYPE_CONST_F || Opnd1.dataType > TOKENTYPE_CONST_N) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_47 );
	if ( Opnd2.dataType < TOKENTYPE_CONST_F || Opnd2.dataType > TOKENTYPE_CONST_N) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_48 );

	if ( Opnd1.dataType != Opnd2.dataType ) {
		if ( Opnd1.dataType == TOKENTYPE_CONST_C || Opnd2.dataType == TOKENTYPE_CONST_C )
		throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_49 );
	}
	if ( Opnd1.dataType != opndStack->tVal.dataType ) {
		if ( Opnd1.dataType == TOKENTYPE_CONST_C || opndStack->tVal.dataType == TOKENTYPE_CONST_C )
		throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_50 );
	}

	opndStack->tVal.dataType = DATA_TYPE_BOOL;
	opndStack->tVal.value.boolVal = true;
#else
	if (opndStack->tVal.dataType == TOKENTYPE_CONST_C) {
		if (strcmp(opndStack->tVal.value.strVal, Opnd1.value.strVal) < 0) {
			opndStack->tVal.dataType = DATA_TYPE_BOOL;
			opndStack->tVal.value.boolVal = false;
			return;
		}

		if (strcmp(opndStack->tVal.value.strVal, Opnd2.value.strVal) > 0) {
			opndStack->tVal.dataType = DATA_TYPE_BOOL;
			opndStack->tVal.value.boolVal = false;
			return;
		}
	} else {
		if (opndStack->tVal.dataType == TOKENTYPE_CONST_N)
			opndStack->tVal.value.dblVal =
					(double) opndStack->tVal.value.longVal;

		//  최소값 비교
		if (Opnd1.dataType == TOKENTYPE_CONST_N)
			Opnd1.value.dblVal = (double) Opnd1.value.longVal;
		if (opndStack->tVal.value.dblVal < Opnd1.value.dblVal) {
			opndStack->tVal.dataType = DATA_TYPE_BOOL;
			opndStack->tVal.value.boolVal = false;
			return;
		}

		//  최대값 비교
		if (Opnd2.dataType == TOKENTYPE_CONST_N)
			Opnd2.value.dblVal = (double) Opnd2.value.longVal;
		if (opndStack->tVal.value.dblVal > Opnd2.value.dblVal) {
			opndStack->tVal.dataType = DATA_TYPE_BOOL;
			opndStack->tVal.value.boolVal = false;
			return;
		}
	}

	opndStack->tVal.dataType = DATA_TYPE_BOOL;
	opndStack->tVal.value.boolVal = true;
#endif
}

void CMRExpression::CalcFunction(CRULE_STMT *p) {
	OPERAND_STACK *pOpnd1, *pOpnd2;
	TYPE_VALUE Value;
	char m_TraceMsg[512];
	unsigned long rc = 0;
	int parmCnt = 0;

	CRULE_STMT *pExp;
	TYPE_VALUE *expResult, expResultVal;

	CMRExpression Expression(m_pParent, m_pDataList, m_pRuleNode, pCParam);

	pExp = p->pExpression;

	while (pExp) {

		expResult = Expression.CalcExpression(pExp->pNext);

#ifdef _PARSER
		if ( ! expResult ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_51 );
#endif
		memcpy((char*) &expResultVal, (char*) expResult, sizeof(TYPE_VALUE));

		PushOperand(&expResultVal);

		parmCnt++;
		pExp = pExp->pExpression;
	}

	/*  Rule, Item Array 관련 Unary Function일때  */
	bool bProcessed = true;
	rc = 1;

	switch (p->tokType) {
	case TOKENFUNC_ROWS:
#ifdef _PARSER
		if ( parmCnt != 1 ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_52 );
#endif
		Func_ROWS();
		return;

	case TOKENFUNC_SUM:
#ifdef _PARSER
//printf("opndStack->tVal.dataType %d = %d\n", parmCnt, opndStack->tVal.dataType );
		if ( parmCnt != 1 ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_53 );
		if ( opndStack->tVal.dataType != TOKENTYPE_RULE &&
				opndStack->tVal.dataType != TOKENTYPE_CONST_N &&
				opndStack->tVal.dataType != TOKENTYPE_CONST_F ) {
			throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_54 );
		}

		opndStack->tVal.dataType = TOKENTYPE_CONST_N;
		opndStack->tVal.value.longVal = 1;
#else
		SumArray();
#endif
		return;

	case TOKENFUNC_INDEXOF:
#ifdef _PARSER
		if ( parmCnt != 2 ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_55 );
#endif
		Func_INDEXOF();

		return;

	case TOKENFUNC_LEN:
#ifdef _PARSER
		if ( parmCnt != 1 ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_56 );
#endif
		if (opndStack->tVal.dataType == TOKENTYPE_RULE)
			m_pDataList->GetRuleValue(&opndStack->tVal);
		if (opndStack->tVal.dataType == TOKENTYPE_GLOBAL)
			m_pDataList->getItemValue(&opndStack->tVal);

		FUNC_TOSTRING (opndStack);
		/*
		 #ifdef _PARSER
		 if ( opndStack->tVal.dataType != TOKENTYPE_CONST_C &&
		 opndStack->tVal.dataType != TOKENTYPE_CONST_L )
		 throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_0057 );
		 #endif
		 */
		opndStack->tVal.dataType = TOKENTYPE_CONST_N;
		opndStack->tVal.value.longVal = strlen(opndStack->tVal.value.strVal);

		return;

	case TOKENFUNC_ISNUM:
#ifdef _PARSER
		if ( parmCnt != 1 ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_58 );
#endif
		if (opndStack->tVal.dataType == TOKENTYPE_RULE)
			m_pDataList->GetRuleValue(&opndStack->tVal);
		if (opndStack->tVal.dataType == TOKENTYPE_GLOBAL)
			m_pDataList->getItemValue(&opndStack->tVal);

		rc = FUNC_STR_TYPE(1, opndStack);

		break;

	case TOKENFUNC_ISALPHA:
#ifdef _PARSER
		if ( parmCnt != 1 ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_59 );
#endif
		if (opndStack->tVal.dataType == TOKENTYPE_RULE)
			m_pDataList->GetRuleValue(&opndStack->tVal);
		if (opndStack->tVal.dataType == TOKENTYPE_GLOBAL)
			m_pDataList->getItemValue(&opndStack->tVal);

		rc = FUNC_STR_TYPE(2, opndStack);

		break;

	case TOKENFUNC_MAX:
#ifdef _PARSER
		if ( parmCnt != 1 ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_60 );
#endif
		CalMaxMin(1); //  MAX
		return;

	case TOKENFUNC_MIN:
#ifdef _PARSER
		if ( parmCnt != 1 ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_61 );
#endif
		CalMaxMin(2); //  MIN
		return;

	case TOKENFUNC_DISPLAY:
#ifdef _PARSER
		if ( parmCnt < 1 ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_62 );
#endif

#ifdef _TRACE
		m_pDataList->TraceOut( "   DISPLAY DATA" );
#endif
		Func_Display(parmCnt);

		return;

	case TOKENFUNC_EXTERN:
		if (parmCnt < 2)
			throw new CMTException(USERERR_GRAMMAR, EMSG_GRAMMAR_63);

		Func_Extern(parmCnt);

		return;

	default:
		bProcessed = false;
		break;
	}

	if (!rc) {
		throw new CMTException(USERERR_GRAMMAR, EMSG_GRAMMAR_64);
	}

	if (bProcessed)
		return;

	rc = 0;

/////////////////////////////////////////////////////////////////////
	/*  Rule, Item Array 관련 Function아닐때  */

	if (parmCnt > 0 && p->tokType != TOKENFUNC_HD_SPC4
			&& p->tokType != TOKENFUNC_HD_SPC5) {
		if (opndStack->tVal.dataType == TOKENTYPE_RULE)
			m_pDataList->GetRuleValue(&opndStack->tVal);
		if (opndStack->tVal.dataType == TOKENTYPE_GLOBAL)
			m_pDataList->getItemValue(&opndStack->tVal);
	}

	switch (p->tokType) {
	case TOKENFUNC_HD_SPC1:
	case TOKENFUNC_HD_SPC2:
	case TOKENFUNC_HD_SPC3:
	case TOKENFUNC_HD_SPC4:
	case TOKENFUNC_HD_SPC5:
		rc = Func_SPECIAL(p->tokType, parmCnt);
		break;

	case TOKENFUNC_SUBSTR:
#ifdef _PARSER
		if ( parmCnt != 3 ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_55 );
#endif
		pOpnd2 = PopOperand();

		if (opndStack->tVal.dataType == TOKENTYPE_RULE)
			m_pDataList->GetRuleValue(&opndStack->tVal);
		if (opndStack->tVal.dataType == TOKENTYPE_GLOBAL)
			m_pDataList->getItemValue(&opndStack->tVal);

		pOpnd1 = PopOperand();

		if (opndStack->tVal.dataType == TOKENTYPE_RULE)
			m_pDataList->GetRuleValue(&opndStack->tVal);
		if (opndStack->tVal.dataType == TOKENTYPE_GLOBAL)
			m_pDataList->getItemValue(&opndStack->tVal);

		char sBuf[500];

		rc = FUNC_SUBSTR(opndStack, pOpnd1, pOpnd2, sBuf);
		
		
		
		if (rc && sBuf[0]) {
			opndStack->tVal.value.pLStr = (char*) m_pDataList->MRMemAlloc1(
					strlen(sBuf) + 1);
			strcpy(opndStack->tVal.value.pLStr, sBuf);
		}

		break;
	case TOKENFUNC_TODAY:
#ifdef _PARSER
		if ( parmCnt > 1 ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_56 );
#endif
		char mark[2];
		if (parmCnt == 1) {
			pOpnd1 = PopOperand();
			if (pOpnd1->tVal.dataType != TOKENTYPE_CONST_C)
				throw new CMTException(USERERR_GRAMMAR, EMSG_GRAMMAR_57);
			mark[0] = pOpnd1->tVal.value.strVal[0];
			mark[1] = 0;
		} else {
			mark[0] = 0;
		}

		rc = FUNC_TODAY(&Value, mark);
		PushOperand(&Value);
		break;

	case TOKENFUNC_DATEPLUSMON:
#ifdef _PARSER
		if ( parmCnt < 2 || parmCnt > 3 ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_58 );
#endif
		if (parmCnt == 3) {
			pOpnd2 = PopOperand();

			if (opndStack->tVal.dataType == TOKENTYPE_RULE)
				m_pDataList->GetRuleValue(&opndStack->tVal);
			if (opndStack->tVal.dataType == TOKENTYPE_GLOBAL)
				m_pDataList->getItemValue(&opndStack->tVal);
		}

		pOpnd1 = PopOperand();

		if (opndStack->tVal.dataType == TOKENTYPE_RULE)
			m_pDataList->GetRuleValue(&opndStack->tVal);
		if (opndStack->tVal.dataType == TOKENTYPE_GLOBAL)
			m_pDataList->getItemValue(&opndStack->tVal);

		if (parmCnt == 3)
			rc = FUNC_DATEPLUSMON(opndStack, pOpnd1, pOpnd2);
		else
			rc = FUNC_DATEPLUSMON(opndStack, pOpnd1, 0);
		break;

	case TOKENFUNC_DATE:
#ifdef _PARSER
		if ( parmCnt != 2 ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_59 );
#endif
		pOpnd1 = PopOperand();
		if (opndStack->tVal.dataType == TOKENTYPE_RULE)
			m_pDataList->GetRuleValue(&opndStack->tVal);
		if (opndStack->tVal.dataType == TOKENTYPE_GLOBAL)
			m_pDataList->getItemValue(&opndStack->tVal);

		rc = FUNC_DATE(opndStack, pOpnd1);

 

		break;

	case TOKENFUNC_DURATION:
#ifdef _PARSER
		if ( parmCnt != 2 ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_60 );
#endif
		pOpnd1 = PopOperand();
		if (opndStack->tVal.dataType == TOKENTYPE_RULE)
			m_pDataList->GetRuleValue(&opndStack->tVal);
		if (opndStack->tVal.dataType == TOKENTYPE_GLOBAL)
			m_pDataList->getItemValue(&opndStack->tVal);

		rc = FUNC_DURATION(opndStack, pOpnd1);

		break;

	case TOKENFUNC_AGE:
#ifdef _PARSER
		if ( parmCnt != 2 ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_61 );
#endif
		pOpnd1 = PopOperand();
		if (opndStack->tVal.dataType == TOKENTYPE_RULE)
			m_pDataList->GetRuleValue(&opndStack->tVal);
		if (opndStack->tVal.dataType == TOKENTYPE_GLOBAL)
			m_pDataList->getItemValue(&opndStack->tVal);

		rc = FUNC_AGE(opndStack, pOpnd1);

		break;

	case TOKENFUNC_RES_AGE:
		if (parmCnt == 1)
			rc = FUNC_RES_AGE(opndStack);
		else if (parmCnt == 2) {
			pOpnd1 = PopOperand();

			if (opndStack->tVal.dataType == TOKENTYPE_RULE)
				m_pDataList->GetRuleValue(&opndStack->tVal);
			if (opndStack->tVal.dataType == TOKENTYPE_GLOBAL)
				m_pDataList->getItemValue(&opndStack->tVal);

			rc = FUNC_RESAGE_BYDATE(opndStack, pOpnd1, 2); // 만연령

		} else if (parmCnt == 3) {
			short gbn;

			pOpnd2 = PopOperand();
#ifdef _PARSER
			if ( pOpnd2->tVal.dataType != DATA_TYPE_LONG ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_62 );
#endif
			if (opndStack->tVal.dataType == TOKENTYPE_RULE)
				m_pDataList->GetRuleValue(&opndStack->tVal);
			if (opndStack->tVal.dataType == TOKENTYPE_GLOBAL)
				m_pDataList->getItemValue(&opndStack->tVal);

			pOpnd1 = PopOperand();

			if (opndStack->tVal.dataType == TOKENTYPE_RULE)
				m_pDataList->GetRuleValue(&opndStack->tVal);
			if (opndStack->tVal.dataType == TOKENTYPE_GLOBAL)
				m_pDataList->getItemValue(&opndStack->tVal);

			gbn = (short) pOpnd2->tVal.value.longVal;
			if (gbn != 1)
				gbn = 2;

			rc = FUNC_RESAGE_BYDATE(opndStack, pOpnd1, gbn);

		} else
			throw new CMTException(USERERR_GRAMMAR, EMSG_GRAMMAR_63);

		break;
		/*
		 case TOKENFUNC_CAR_AGE :
		 #ifdef _PARSER
		 if ( parmCnt != 2 ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR );
		 #endif
		 pOpnd1 = PopOperand();
		 if ( opndStack->tVal.dataType == TOKENTYPE_RULE ) m_pDataList->GetRuleValue( & opndStack->tVal );
		 if ( opndStack->tVal.dataType == TOKENTYPE_GLOBAL ) m_pDataList->getItemValue( & opndStack->tVal );

		 rc = FUNC_CARAGE( opndStack, pOpnd1);

		 break;
		 */
	case TOKENFUNC_MONTH:
		short gbn;

#ifdef _PARSER
		if ( parmCnt < 2 || parmCnt > 3 ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_64 );
#endif
		if (parmCnt == 2) {
			gbn = 0;
		} else {
			pOpnd2 = PopOperand();
#ifdef _PARSER
			if ( pOpnd2->tVal.dataType != DATA_TYPE_LONG ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_65 );
#endif
			gbn = (short) pOpnd2->tVal.value.longVal;
			if (gbn < 1 || gbn > 2)
				gbn = 0;
		}

//printf("parmCnt = %d  gbn = %d\n", parmCnt, gbn );

		if (opndStack->tVal.dataType == TOKENTYPE_RULE)
			m_pDataList->GetRuleValue(&opndStack->tVal);
		else if (opndStack->tVal.dataType == TOKENTYPE_GLOBAL)
			m_pDataList->getItemValue(&opndStack->tVal);

		pOpnd1 = PopOperand();

		if (opndStack->tVal.dataType == TOKENTYPE_RULE)
			m_pDataList->GetRuleValue(&opndStack->tVal);
		else if (opndStack->tVal.dataType == TOKENTYPE_GLOBAL)
			m_pDataList->getItemValue(&opndStack->tVal);

		rc = FUNC_MONTHS(opndStack, pOpnd1, gbn);

		break;

	case TOKENFUNC_ROUND:
#ifdef _PARSER
		if ( parmCnt != 2 ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_66 );
#endif
		pOpnd1 = PopOperand();

		if (opndStack->tVal.dataType == TOKENTYPE_RULE)
			m_pDataList->GetRuleValue(&opndStack->tVal);
		if (opndStack->tVal.dataType == TOKENTYPE_GLOBAL)
			m_pDataList->getItemValue(&opndStack->tVal);

		rc = FUNC_ROUND(opndStack, pOpnd1);

		break;

	case TOKENFUNC_TRUNC:
#ifdef _PARSER
		if ( parmCnt != 2 ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_67 );
#endif
		pOpnd1 = PopOperand();

		if (opndStack->tVal.dataType == TOKENTYPE_RULE)
			m_pDataList->GetRuleValue(&opndStack->tVal);
		if (opndStack->tVal.dataType == TOKENTYPE_GLOBAL)
			m_pDataList->getItemValue(&opndStack->tVal);

		rc = FUNC_TRUNC(opndStack, pOpnd1);

		break;

	case TOKENFUNC_TONUMBER:
#ifdef _PARSER
		if ( parmCnt != 1 ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_68 );
#endif
		rc = FUNC_TONUMBER(opndStack);

		break;

	case TOKENFUNC_TODOUBLE:
#ifdef _PARSER
		if ( parmCnt != 1 ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_69 );
#endif
		rc = FUNC_TODOUBLE(opndStack);

		break;

	case TOKENFUNC_TOSTRING:
#ifdef _PARSER
		if ( parmCnt != 1 ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_70 );
#endif
		rc = FUNC_TOSTRING(opndStack);

		break;

	case TOKENFUNC_GETNVAL:
	case TOKENFUNC_GETSVAL:
#ifdef _PARSER
		if ( parmCnt == 0 || parmCnt > 2 ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_71 );

		if ( parmCnt == 2 ) {
			pOpnd1 = PopOperand();
			if ( pOpnd1->tVal.dataType != TOKENTYPE_CONST_N &&
					(pOpnd1->tVal.dataType < TOKENTYPE_INDEX0 || pOpnd1->tVal.dataType > TOKENTYPE_INDEX3 ) )
			throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_72 );
		}

		if ( opndStack->tVal.dataType != TOKENTYPE_CONST_C )
		throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_73 );

		if ( strcmp(opndStack->tVal.value.strVal,"1900-01-01") == 0 )
		throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_74 );

		if ( p->tokType == TOKENFUNC_GETNVAL ) {
			opndStack->tVal.dataType = DATA_TYPE_DOUBLE;
			opndStack->tVal.value.dblVal = 1.0;
		} else {
			opndStack->tVal.dataType = DATA_TYPE_STRING;
			strcpy(opndStack->tVal.value.strVal, "A");
		}

		rc = true;
#else
		int idx;

		if (parmCnt == 1) {
			idx = 1;
		} else {
			pOpnd1 = PopOperand();
			int nTokType = pOpnd1->tVal.dataType;

			switch (nTokType) {
			case TOKENTYPE_CONST_N:
				idx = pOpnd1->tVal.value.longVal;
				break;
			case TOKENTYPE_INDEX1:
			case TOKENTYPE_INDEX2:
			case TOKENTYPE_INDEX3:
				idx = m_pParent->getIndexValue(nTokType);
				break;
			default:
				throw new CMTException(USERERR_GRAMMAR, EMSG_GRAMMAR_75);
			}
		}

		if (opndStack->tVal.dataType != TOKENTYPE_CONST_C) {
			char msg[100];
			sprintf(msg, "%s (GET?VAL)", EMSG_GRAMMAR_76);
			throw new CMTException(USERERR_GRAMMAR, msg);
		}

	#ifdef RULE_RE_RUN
		if (m_pRuleNode->scope == 0) {/* getVal사용하는 함수는 무조건 재 실행 옵션으로 변경 */
			m_pRuleNode->scope =2;
		}
	#endif

		rc = m_pDataList->getGblVal(&opndStack->tVal, p->tokType, idx);
#endif
		break;

	case TOKENFUNC_FINDVAL:
#ifdef _PARSER
		if ( parmCnt != 2 ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_77 );
#endif
		pOpnd1 = PopOperand();

		if (pOpnd1->tVal.dataType == TOKENTYPE_RULE)
			m_pDataList->GetRuleValue(&pOpnd1->tVal);
		if (pOpnd1->tVal.dataType == TOKENTYPE_GLOBAL)
			m_pDataList->getItemValue(&pOpnd1->tVal);

		if (opndStack->tVal.dataType != TOKENTYPE_CONST_C) {
			throw new CMTException(USERERR_GRAMMAR, EMSG_GRAMMAR_78);
		}

		rc = true;
#ifdef _PARSER
		opndStack->tVal.value.boolVal = true;
#else
		opndStack->tVal.value.boolVal = m_pDataList->findGblVal(
				opndStack->tVal.value.strVal, &pOpnd1->tVal);
#endif
		opndStack->tVal.dataType = TOKENTYPE_CONST_B;

		break;

	default:
		break;
	}

	if (!rc) {
		throw new CMTException(USERERR_GRAMMAR, EMSG_GRAMMAR_79);
	}
}

void CMRExpression::Func_ROWS() {
#ifdef _PARSER
	if ( opndStack->tVal.dataType != TOKENTYPE_RULE &&
			opndStack->tVal.dataType != TOKENTYPE_GLOBAL ) {
		if ( opndStack->tVal.dataType > TOKENTYPE_CONST_V ) {
			throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_80 );
		}
	}

	opndStack->tVal.dataType = TOKENTYPE_CONST_N;
	opndStack->tVal.value.longVal = 1;
#else
	switch (opndStack->tVal.dataType) {
	case TOKENTYPE_RULE:
		opndStack->tVal.value.longVal = m_pDataList->getRuleResultCount(
				(RULE_RESULT *) opndStack->tVal.value.RuleItem.ptr);
		break;
	case TOKENTYPE_GLOBAL:
		opndStack->tVal.value.longVal = m_pDataList->getItemValueCount(
				opndStack->tVal.value.RuleItem.i_col);
		break;
	case TOKENTYPE_CONST_C:
		opndStack->tVal.value.longVal = m_pDataList->getGblRows(opndStack->tVal.value.strVal);
		break;
	default:
		opndStack->tVal.value.longVal = 0;
	}

	opndStack->tVal.dataType = TOKENTYPE_CONST_N;
#endif
}

void CMRExpression::Func_INDEXOF() {
	OPERAND_STACK *pOpnd2;
	pOpnd2 = PopOperand();

	if (pOpnd2->tVal.dataType == TOKENTYPE_RULE)
		m_pDataList->GetRuleValue(&pOpnd2->tVal);

	if (pOpnd2->tVal.dataType == TOKENTYPE_GLOBAL)
		m_pDataList->getItemValue(&pOpnd2->tVal);

#ifdef _PARSER
	if ( opndStack->tVal.dataType == TOKENTYPE_RULE ) m_pDataList->GetRuleValue( & opndStack->tVal );

	if ( opndStack->tVal.dataType != pOpnd2->tVal.dataType )
	{
		throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_81 );
	}
	opndStack->tVal.value.longVal = 0;
#else
	if (opndStack->tVal.dataType == TOKENTYPE_RULE) {

		opndStack->tVal.value.longVal = m_pDataList->getRuleValueIndex(&opndStack->tVal.value, &pOpnd2->tVal);
	} else if (opndStack->tVal.dataType == TOKENTYPE_GLOBAL) {
		opndStack->tVal.value.longVal = m_pDataList->getItemValueIndex(
				opndStack->tVal.value.RuleItem.i_col, &pOpnd2->tVal);
	} else {
		throw new CMTException(USERERR_GRAMMAR, EMSG_GRAMMAR_82);
	}
#endif

	opndStack->tVal.dataType = TOKENTYPE_CONST_N;
}

/**************************************************************************/

void CMRExpression::Calculate(unsigned char Optr) {
	OPERAND_STACK *pOpnd1; //, *pOpnd2;
	char sBuf[3001];
//	TYPE_VALUE    Value;

	unsigned long rc;
	calc_result_msg[0] = 0;

#ifdef _PARSER
	if ( ! opndStack )
	throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_83 );
#endif

	if (opndStack->tVal.dataType == TOKENTYPE_RULE)
		m_pDataList->GetRuleValue(&opndStack->tVal);
	if (opndStack->tVal.dataType == TOKENTYPE_GLOBAL)
		m_pDataList->getItemValue(&opndStack->tVal);
		



	/***   Unary Operator   **********************************************/
	if (Optr == TOKENTYPE_NOT) {
		if (!CALC_NOT(opndStack)) {
			throw new CMTException(USERERR_GRAMMAR, EMSG_GRAMMAR_84);
		}
		return;
	}

	/*********************************************************************/
	pOpnd1 = PopOperand();

#ifdef _PARSER
	if ( ! pOpnd1 ) {
		throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_85 );
	}

	if ( ! opndStack ) {
		throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_86 );
	}
#endif

	if (opndStack->tVal.dataType == TOKENTYPE_RULE)
		m_pDataList->GetRuleValue(&opndStack->tVal);
	if (opndStack->tVal.dataType == TOKENTYPE_GLOBAL)
		m_pDataList->getItemValue(&opndStack->tVal);

	/***   Binary Operator   **********************************************/
	switch (Optr) {
	case TOKENTYPE_POW:
		rc = CALC_POW(opndStack, pOpnd1);
		break;
	case TOKENTYPE_MULTIPLY:
		rc = CALC_MULTI(opndStack, pOpnd1);
		break;
	case TOKENTYPE_DIVIDE:
		rc = CALC_DIVIDE(opndStack, pOpnd1);
		break;
	case TOKENTYPE_MODULUS:
		rc = CALC_MODULUS(opndStack, pOpnd1);
		break;
	case TOKENTYPE_PLUS:
		rc = CALC_PLUS(opndStack, pOpnd1, sBuf);
		if (rc && sBuf[0]) {
			opndStack->tVal.value.pLStr = (char*) m_pDataList->MRMemAlloc1(strlen(sBuf) + 1);
			strcpy(opndStack->tVal.value.pLStr, sBuf);
		}
		break;
	case TOKENTYPE_MINUS:
		rc = CALC_MINUS(opndStack, pOpnd1);
		break;
	case TOKENTYPE_EQUAL:
		rc = CALC_EQUAL(opndStack, pOpnd1);
		break;
	case TOKENTYPE_LESS:
		rc = CALC_LESS(opndStack, pOpnd1);
		break;
	case TOKENTYPE_GREATER:
		rc = CALC_GREATER(opndStack, pOpnd1);
		break;
	case TOKENTYPE_LESSEQ:
		rc = CALC_LESSEQ(opndStack, pOpnd1);
		break;
	case TOKENTYPE_GREATEREQ:
		rc = CALC_GREATEREQ(opndStack, pOpnd1);
		break;
	case TOKENTYPE_NOTEQ:
		rc = CALC_NEQUAL(opndStack, pOpnd1);
		break;
	case TOKENTYPE_AND:
		rc = CALC_AND(opndStack, pOpnd1);
		break;
	case TOKENTYPE_OR:
		rc = CALC_OR(opndStack, pOpnd1);
		break;
	default:
		break;
	}

	/*********************************************************************/
	if (!rc) {
		if (calc_result_msg[0] == 0){
			sprintf(sBuf,"%s::%s::%d\n", calc_result_msg, EMSG_GRAMMAR_88 ,Optr);
			strcpy(calc_result_msg, EMSG_GRAMMAR_88);
		}
		throw new CMTException(USERERR_GRAMMAR, sBuf);
		
//		throw new CMTException(USERERR_GRAMMAR, calc_result_msg);
	}
 
}

/**********************************************************************************/

void CMRExpression::CalRequestIndex(unsigned char pTokType) {
	TYPE_VALUE Value;

	Value.dataType = TOKENTYPE_CONST_N;

#ifdef _PARSER
	Value.value.longVal = 1;
#else
	Value.value.longVal = (long) m_pParent->getIndexValue(pTokType);
#endif

	PushOperand(&Value);
}

void CMRExpression::clearTValue(TYPE_VALUE *Value, int opt) {
	switch (Value->dataType) {
	case DATA_TYPE_STRING:
//				case DATA_TYPE_DATE :
		Value->value.strVal[0] = 0;
		break;
	case DATA_TYPE_LSTRING:
//						Value->value.LString.len = 0;
		Value->value.pLStr = 0;
		break;
	case DATA_TYPE_DOUBLE:
		if (opt == 1)
			Value->value.dblVal = -9999999999999999.0;
		else
			Value->value.dblVal = 9999999999999999.0;
		break;
	case DATA_TYPE_LONG:
		if (opt == 1)
			Value->value.longVal = -2100000000;
		else
			Value->value.longVal = 2100000000;
		break;
	case DATA_TYPE_BOOL:
		Value->value.boolVal = false;
	}
}

//******************************************************************
bool CMRExpression::CalMaxMin(int opt) {
	bool copyFlag = false;

	if (opndStack->tVal.dataType == TOKENTYPE_RULE) {
		RULE_RESULT* pRuleResult;
		RESULT_REC* pResList;

		pRuleResult = (RULE_RESULT*) opndStack->tVal.value.RuleItem.ptr;

		int col = opndStack->tVal.value.RuleItem.i_col;
		if (col > 0)
			col--;

		opndStack->tVal.dataType = pRuleResult->pRuleNode->retType[col];

		pResList = m_pDataList->getRuleResult(pRuleResult->ruleId);

		if (!pResList) {
			clearTValue(&opndStack->tVal, opt);
			return false;
		}

		memcpy((char *) &opndStack->tVal.value, (char *) &pResList->value[col],
				sizeof(VALUE_UNION));

		pResList = pRuleResult->resRecord;

		while (pResList) {
			copyFlag = false;

			switch (opndStack->tVal.dataType) {
			case DATA_TYPE_STRING:
//				case DATA_TYPE_DATE :
				if (strcmp(opndStack->tVal.value.strVal,
						pResList->value[col].strVal) < 0)
					copyFlag = true;
				else
					copyFlag = false;
				break;
			case DATA_TYPE_LSTRING:
				if (strcmp(opndStack->tVal.value.pLStr,
						pResList->value[col].pLStr) < 0)
					copyFlag = true;
				else
					copyFlag = false;
				break;
			case DATA_TYPE_DOUBLE:
				copyFlag = opndStack->tVal.value.dblVal
						< pResList->value[col].dblVal;
				break;
			case DATA_TYPE_LONG:
				copyFlag = opndStack->tVal.value.longVal
						< pResList->value[col].longVal;
				break;
			default:
				break;
			}

			if (opt == 1) {
				if (copyFlag)
					memcpy((char *) &opndStack->tVal.value,
							(char *) &pResList->value[col],
							sizeof(VALUE_UNION));
			} else {
				if (!copyFlag)
					memcpy((char *) &opndStack->tVal.value,
							(char *) &pResList->value[col],
							sizeof(VALUE_UNION));
			}

			pResList = pResList->pNext;
		}

	} else if (opndStack->tVal.dataType == TOKENTYPE_GLOBAL) {
		TYPE_VALUE Value;
		int col = opndStack->tVal.value.RuleItem.i_col;
		int row = m_pDataList->getItemValueCount(col);

		opndStack->tVal.dataType = m_pDataList->getItemType(col);
		if (row == 0) {
			clearTValue(&opndStack->tVal, opt);
			return false;
		}

		opndStack->tVal.value.RuleItem.i_row = 0;
		m_pDataList->getItemValue(&opndStack->tVal);

		for (int j = 1; j <= row; j++) {
			Value.value.RuleItem.i_row = j;
			Value.value.RuleItem.i_col = col;
			m_pDataList->getItemValue(&Value);

			switch (Value.dataType) {
			case DATA_TYPE_STRING:
//				case DATA_TYPE_DATE :
				if (strcmp(opndStack->tVal.value.strVal, Value.value.strVal)
						< 0)
					copyFlag = true;
				else
					copyFlag = false;
				break;
			case DATA_TYPE_LSTRING: //  ITEM은 LSTRING 없다 ?
				if (strcmp(opndStack->tVal.value.pLStr, Value.value.pLStr) < 0)
					copyFlag = true;
				else
					copyFlag = false;
				break;

			case DATA_TYPE_DOUBLE:
				copyFlag = opndStack->tVal.value.dblVal < Value.value.dblVal;
				break;
			case DATA_TYPE_LONG:
				copyFlag = opndStack->tVal.value.longVal < Value.value.longVal;
				break;
			default:
				break;
			}

			if (opt == 1) {
				if (copyFlag)
					memcpy((char *) &opndStack->tVal.value,
							(char *) &Value.value, sizeof(VALUE_UNION));
			} else {
				if (!copyFlag)
					memcpy((char *) &opndStack->tVal.value,
							(char *) &Value.value, sizeof(VALUE_UNION));
			}
		}

	} else if (opndStack->tVal.dataType > TOKENTYPE_CONST_B) {
		throw new CMTException(USERERR_GRAMMAR, EMSG_GRAMMAR_89);
	}

	return true;
}

void CMRExpression::SumArray() {
	if (opndStack->tVal.dataType == TOKENTYPE_RULE) {
		RULE_RESULT* pRuleResult;
		RESULT_REC* pResList;

		pRuleResult = (RULE_RESULT*) opndStack->tVal.value.RuleItem.ptr;
		pResList = pRuleResult->resRecord;

		int col = opndStack->tVal.value.RuleItem.i_col;
		if (col > 0)
			col--;

		opndStack->tVal.dataType = pRuleResult->pRuleNode->retType[col];
		if (opndStack->tVal.dataType == DATA_TYPE_DOUBLE)
			opndStack->tVal.value.dblVal = 0.0;
		else
			opndStack->tVal.value.longVal = 0;

		while (pResList) {
			if (opndStack->tVal.dataType == DATA_TYPE_DOUBLE)
				opndStack->tVal.value.dblVal += pResList->value[col].dblVal;
			else
				opndStack->tVal.value.longVal += pResList->value[col].longVal;

			pResList = pResList->pNext;
		}

	} else { //  TOKENTYPE_GLOBAL
		TYPE_VALUE Value;
		int col = opndStack->tVal.value.RuleItem.i_col;
		int row = m_pDataList->getItemValueCount(col);

		opndStack->tVal.dataType = m_pDataList->getItemType(col);
		if (opndStack->tVal.dataType == DATA_TYPE_DOUBLE)
			opndStack->tVal.value.dblVal = 0.0;
		else
			opndStack->tVal.value.longVal = 0;

		for (int j = 1; j <= row; j++) {
			Value.value.RuleItem.i_row = j;
			Value.value.RuleItem.i_col = col;
			m_pDataList->getItemValue(&Value);

			if (Value.dataType == DATA_TYPE_DOUBLE)
				opndStack->tVal.value.dblVal += Value.value.dblVal;
			else
				opndStack->tVal.value.longVal += Value.value.longVal;

		}
	}
}

void CMRExpression::Func_Display(int idx) {
	OPERAND_STACK *pOpnd1;

	pOpnd1 = PopOperand();

	if (idx > 1)
		Func_Display(idx - 1);

#ifdef _RUNNER
	char msg[500+1];

	if ( pOpnd1->tVal.dataType == TOKENTYPE_RULE )
	m_pDataList->GetRuleValue( & pOpnd1->tVal );
	if ( pOpnd1->tVal.dataType == TOKENTYPE_GLOBAL )
	m_pDataList->getItemValue( & pOpnd1->tVal );

	switch ( pOpnd1->tVal.dataType ) {
		case TOKENTYPE_CONST_F :
		sprintf( msg, "      %lf", pOpnd1->tVal.value.dblVal );
		break;
		case TOKENTYPE_CONST_C :
		sprintf( msg, "      %s", pOpnd1->tVal.value.strVal );
		break;
		case TOKENTYPE_CONST_L :
		sprintf( msg, "      %s", pOpnd1->tVal.value.pLStr );
		break;
		case TOKENTYPE_CONST_N :
		sprintf( msg, "      %ld", pOpnd1->tVal.value.longVal );
		break;
		case TOKENTYPE_CONST_B :
		sprintf( msg, "      %s", pOpnd1->tVal.value.boolVal ? "TRUE" : "FALSE" );
	}
	msg[500]=0;
	m_pDataList->TraceOut( msg );

#endif
}



bool CMRExpression::Func_Extern(int parmCnt) {
	OPERAND_STACK *pOpnd1;
//	bool   rc = true;
	char externId[200];
	bool bWait;
	char *argBuffer[STR_VALUE_SIZE+1];
	if (parmCnt > STR_VALUE_SIZE)
		throw new CMTException(USERERR_GRAMMAR, EMSG_GRAMMAR_90);


	for (int i = parmCnt - 1; i >= 0; i--) {
		pOpnd1 = PopOperand();

		if (pOpnd1->tVal.dataType == TOKENTYPE_RULE)
			m_pDataList->GetRuleValue(&pOpnd1->tVal);
		else if (pOpnd1->tVal.dataType == TOKENTYPE_GLOBAL)
			m_pDataList->getItemValue(&pOpnd1->tVal);

		if (i == 0) {
			if (pOpnd1->tVal.dataType != TOKENTYPE_CONST_C)
				throw new CMTException(USERERR_GRAMMAR, EMSG_GRAMMAR_91);
			strcpy(externId, pOpnd1->tVal.value.strVal);
			strcpy(argBuffer[0], externId);
		} else if (i == 1) {
			if (pOpnd1->tVal.dataType != TOKENTYPE_CONST_N)
				throw new CMTException(USERERR_GRAMMAR, EMSG_GRAMMAR_92);
			bWait = (bool) pOpnd1->tVal.value.longVal;
		} else {
			switch (pOpnd1->tVal.dataType) {
			case TOKENTYPE_CONST_F:
				sprintf(argBuffer[i - 1], "%lf", pOpnd1->tVal.value.dblVal);
				break;
			case TOKENTYPE_CONST_C:
				sprintf(argBuffer[i - 1], "%s", pOpnd1->tVal.value.strVal);
				break;
			case TOKENTYPE_CONST_L:
				sprintf(argBuffer[i - 1], "%s", pOpnd1->tVal.value.pLStr);
				break;
			case TOKENTYPE_CONST_N:
				sprintf(argBuffer[i - 1], "%ld", pOpnd1->tVal.value.longVal);
				break;
			case TOKENTYPE_CONST_B:
				sprintf(argBuffer[i - 1], "%d", pOpnd1->tVal.value.boolVal);
			}
			argBuffer[STR_VALUE_SIZE]=0;
		}
	}

#ifdef _TRACE
	if ( m_pDataList->getTraceFlag() )
	{
		char msg[MAX_QUERY_SIZE]; //MItem.h 30000 MAX_QUERY_SIZE
		char *p = msg;
		int length=0;
		sprintf( p, "EXTERN : %s", externId );
		p += strlen(p);

		for ( int j=0; j<parmCnt-2; j++ ) {
			sprintf( p, " %s", argBuffer[j] );
			length+=strlen(p);
			if(length >MAX_QUERY_SIZE){
				break;
			}else{
				p += strlen(p);
			}
		}
		msg[MAX_QUERY_SIZE-1]=0;
		m_pDataList->TraceOut( msg );
	}
#endif

#ifdef _RUNNER

	argBuffer[parmCnt-1] = NULL;

//	rc = EXT_CALL( externId, bWait, parmCnt-2, argBuffer );
//int EXT_CALL( char *ID, bool bWait, int argc, char **argv )
	/*
	 if ( _execv( externId, argBuffer ) == -1 ) {
	 char msg[100];
	 sprintf(msg, "EXTERNAL Call Error(%d).", errno );
	 throw new CMTException( USERERR_GRAMMAR, msg );
	 }
	 */
#endif

	return true;
}

bool CMRExpression::Func_SPECIAL(unsigned char tokType, int parmCnt) {
	OPERAND_STACK *pOpnd;

	char buf[PARAM_MAX_COUNT][STR_VALUE_SIZE+1];	// 2017-09-27 이전 buf[20]임  ??=>일단 100으로 변경 : 현재 사용하지 않는 함수 같음.
	char concatStr[STR_VALUE_SIZE+1];

	int cnt;
	bool rc;

	cnt = parmCnt;
#ifdef _PARSER

	if ( cnt == 0 )
	throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_93 );

	if (opndStack->tVal.dataType == TOKENTYPE_RULE ) {
		if ( cnt > 1 ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_94 );

		pOpnd = PopOperand();

		RULE_RESULT *p = (RULE_RESULT *) pOpnd->tVal.value.RuleItem.ptr;
		int col = pOpnd->tVal.value.RuleItem.i_col;
		if ( col > 0 ) col--;

		unsigned char retType = p->pRuleNode->retType[col];

		if ( retType != DATA_TYPE_STRING )
		throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_95 );
	} else {
		//	모든 Parm이 String인지 체크한다.
		for ( int i=0; i<cnt; i++ ) {
			pOpnd = PopOperand();

			if (pOpnd->tVal.dataType != DATA_TYPE_STRING )
			throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_96 );

		}
	}

#else
	RULE_RESULT *pRuleResult;
	int i;

	if (rId_DAMBOGM == 0) {
		throw new CMTException(UERR_ENV_FUNC, EMSG_ENV_FUNC);
	}

	if (opndStack->tVal.dataType == TOKENTYPE_RULE) {
		pOpnd = PopOperand();
		pRuleResult = (RULE_RESULT *) pOpnd->tVal.value.RuleItem.ptr;
		RESULT_REC *p = pRuleResult->resRecord;

		i = 0;
		while (p) {
			strcpy(buf[i], p->value->strVal);
			p = p->pNext;
			i++;
		}

		cnt = i;

	} else {

		for (i = cnt - 1; i >= 0; i--) {
			pOpnd = PopOperand();
			strcpy(buf[i], pOpnd->tVal.value.strVal + rConf_DAMBO_POS);
//printf("    %d : %s\n", i, buf[i] );
		}

		strcpy(concatStr, " ");

		if (cnt > 2) {
			i = cnt - 1;
			if (!isalnum(buf[i][0])) {
				strcpy(concatStr, buf[i]);
				cnt--;
			}
		}

		/*
		 printf( "cnt = %d\n",cnt);

		 for ( i=0; i<cnt; i++ ) {
		 pOpnd = PopOperand();
		 strcpy( buf[i], pOpnd->tVal.value.strVal+rConf_DAMBO_POS );
		 printf("    %d : %s\n", i, buf[i] );
		 }

		 strcpy( concatStr, " " );

		 if ( cnt > 2 ) {
		 i = cnt - 1;
		 if ( ! isalnum( buf[i][0] )) {
		 strcpy( concatStr, buf[i] );
		 cnt--;
		 }
		 }
		 */

	}

#endif

	switch (tokType) {
	case TOKENFUNC_HD_SPC1:
		rc = Func_HD_SPC1(cnt, buf, concatStr);
		break;

	case TOKENFUNC_HD_SPC2:
		rc = Func_HD_SPC2(cnt, buf);
		break;

	case TOKENFUNC_HD_SPC3:
		rc = Func_HD_SPC3(cnt, buf, concatStr);
		break;

	case TOKENFUNC_HD_SPC4:
		rc = Func_HD_SPC4(cnt, buf);
		break;

	case TOKENFUNC_HD_SPC5:
		rc = Func_HD_SPC5(cnt, buf);
		break;
	}

	return rc;
}

	bool CMRExpression::Func_HD_SPC1(int cnt, char buf[][STR_VALUE_SIZE+1], char *concatStr) {


//  DAMBONAME : 담보명칭 만들기

	TYPE_VALUE MsgTVal;

	MsgTVal.dataType = TOKENTYPE_CONST_L;

#ifdef _PARSER

	MsgTVal.value.pLStr = (char*) m_pDataList->MRMemAlloc1(STR_VALUE_SIZE);
	MsgTVal.value.pLStr[0] = 0;

#else

	VALUE_UNION *pVal;
	RULE_RESULT *pRuleResult;
	char msgBuf[600];
	int i;

	if (rId_RuleName == 0) {
		throw new CMTException(UERR_ENV_FUNC, EMSG_ENV_FUNC);
	}

	msgBuf[0] = 0;
	for (i = 0; i < cnt; i++) {

		pRuleResult = m_pDataList->GetRuleResult(rId_RuleName, buf[i]);

		if (!pRuleResult) {
			TYPE_VALUE ParmVal;
			ParmVal.dataType = DATA_TYPE_STRING;
			strcpy(ParmVal.value.strVal, buf[i]);

			CMRParam NewParm(&ParmVal, 1);

			CMRRunner Runner(rId_RuleName, buf[i], m_pDataList,
					(CMRParam*) &NewParm);

			pRuleResult = Runner.getRuleResultPtr();
		}

		if (!pRuleResult) {
			throw new CMTException(77777, "Rule running error.");
		}

		if (!pRuleResult->resRecord)
			continue;

		pVal = pRuleResult->resRecord->value;

		strcat(msgBuf, pVal->pLStr);

//		if ( i > 0 )
		strcat(msgBuf, concatStr);
	}

	int z = strlen(msgBuf);
	if (z > 0)
		msgBuf[z - strlen(concatStr)] = 0;

	MsgTVal.value.pLStr = (char*) m_pDataList->MRMemAlloc1(z + 1);
	strcpy(MsgTVal.value.pLStr, msgBuf);

#endif

	PushOperand(&MsgTVal);

	return true;
}

	bool CMRExpression::Func_HD_SPC2(int cnt, char buf[][STR_VALUE_SIZE+1]) {

//  DAMBOGAIP : 가입한 Count

//	OPERAND_STACK *pOpnd;
	TYPE_VALUE CntTVal;

	CntTVal.dataType = DATA_TYPE_LONG;
	CntTVal.value.longVal = 0;

#ifndef _PARSER

//	char			DamboCd[5];
	RULE_RESULT *pRuleResult;
	RESULT_REC *pRec;

	if (rId_DAMBOLIST == 0) {
		throw new CMTException(UERR_ENV_FUNC, EMSG_ENV_FUNC);
	}

	pRuleResult = m_pDataList->GetRuleResult(rId_DAMBOLIST, (void*) 0);

	if (!pRuleResult) {

		CMRRunner Runner(rId_DAMBOLIST, "", m_pDataList, 0);

		pRuleResult = Runner.getRuleResultPtr();

		if (!pRuleResult)
			throw new CMTException(77777, "Rule running error.");
	}

	for (int i = 0; i < cnt; i++) {
		pRec = pRuleResult->resRecord;

		while (pRec) {
			if (strncmp(pRec->value[0].strVal, buf[i], strlen(buf[i])) == 0) {
				CntTVal.value.longVal++;
				break;
			}
			pRec = pRec->pNext;
		}
	}
#endif

	PushOperand(&CntTVal);

	return true;
}

	bool CMRExpression::Func_HD_SPC3(int cnt, char buf[][STR_VALUE_SIZE+1], char *concatStr) {

//  DAMBONAME : Parm목록(코드)의 담보명칭을 구하여 Concat한다.

	TYPE_VALUE MsgTVal;

	MsgTVal.dataType = TOKENTYPE_CONST_L;

#ifdef _PARSER

	MsgTVal.value.pLStr = (char*) m_pDataList->MRMemAlloc1(STR_VALUE_SIZE);
	MsgTVal.value.pLStr[0] = 0;

#else
	RULE_RESULT *pRuleResult;
	RESULT_REC *pRec;
	VALUE_UNION *pVal;
	char msgBuf[600];
	int i;

	if (rId_RuleName == 0) {
		throw new CMTException(UERR_ENV_FUNC, EMSG_ENV_FUNC);
	}

	if (!m_pDataList->m_HAHA_rNode)
		m_pDataList->m_HAHA_rNode = m_pDataList->GetRuleResult(rId_DAMBOLIST,
				(void*) 0);

	if (!m_pDataList->m_HAHA_rNode) {

		CMRRunner Runner(rId_DAMBOLIST, "", m_pDataList, 0);

		m_pDataList->m_HAHA_rNode = Runner.getRuleResultPtr();

		if (!m_pDataList->m_HAHA_rNode)
			throw new CMTException(77777, "Rule running error.");
	}

	msgBuf[0] = 0;
//	for ( i=cnt-1; i>=0; i-- ) {
	for (i = 0; i < cnt; i++) {

		pRec = m_pDataList->m_HAHA_rNode->resRecord;
		while (pRec) {
			if (strncmp(pRec->value[0].strVal, buf[i], strlen(buf[i])) == 0)
				break;
			pRec = pRec->pNext;
		}

		if (!pRec)
			continue;

		pRuleResult = m_pDataList->GetRuleResult(rId_RuleName, buf[i]);

		if (!pRuleResult) {

			TYPE_VALUE ParmVal;
			ParmVal.dataType = DATA_TYPE_STRING;
			strcpy(ParmVal.value.strVal, buf[i]);

			CMRParam NewParm(&ParmVal, 1);

			CMRRunner Runner(rId_RuleName, buf[i], m_pDataList,
					(CMRParam*) &NewParm);

			pRuleResult = Runner.getRuleResultPtr();

		}

		if (!pRuleResult) {
			throw new CMTException(77777, "Rule running error.");
		}

		if (!pRuleResult->resRecord)
			continue;

		pVal = pRuleResult->resRecord->value;

		strcat(msgBuf, pVal->pLStr);

//		if ( i < cnt-1 )
		strcat(msgBuf, concatStr);
	}

	int z = strlen(msgBuf);
	if (z > 0)
		msgBuf[z - strlen(concatStr)] = 0;

	MsgTVal.value.pLStr = (char*) m_pDataList->MRMemAlloc1(z + 1);
	strcpy(MsgTVal.value.pLStr, msgBuf);

#endif

	PushOperand(&MsgTVal);

	return true;
}

	bool CMRExpression::Func_HD_SPC4(int cnt, char buf[][STR_VALUE_SIZE+1]) {


// DAMBOGM : Parm목록의 가입금액SUM구하기

	TYPE_VALUE MsgTVal;

	MsgTVal.dataType = TOKENTYPE_CONST_F;
	MsgTVal.value.dblVal = 0.0;

#ifndef _PARSER
	RULE_RESULT *pRuleResult;
	RESULT_REC *pRecSpec, *pRecAmt;
	int i;

	if (rId_DAMBOGM == 0) {
		throw new CMTException(UERR_ENV_FUNC, EMSG_ENV_FUNC);
	}

	// {전체특약SPEC} 구하기
	if (!m_pDataList->m_HAHA_rNode)
		m_pDataList->m_HAHA_rNode = m_pDataList->GetRuleResult(rId_DAMBOLIST,
				(void*) 0);

	if (!m_pDataList->m_HAHA_rNode) {

		CMRRunner Runner(rId_DAMBOLIST, "", m_pDataList, 0);

		m_pDataList->m_HAHA_rNode = Runner.getRuleResultPtr();

		if (!m_pDataList->m_HAHA_rNode)
			throw new CMTException(77777, "Rule running error.");
	}

	//  {전체가입금액} 구하기
	pRuleResult = m_pDataList->GetRuleResult(rId_DAMBOGM, (void*) 0);

	if (!pRuleResult) {

		CMRRunner Runner(rId_DAMBOGM, "", m_pDataList, 0);

		pRuleResult = Runner.getRuleResultPtr();

		if (!pRuleResult)
			throw new CMTException(77777, "Rule running error.");
	}

//	if ( ! pRuleResult->resRecord ) continue;

	for (i = cnt - 1; i >= 0; i--) {
		pRecSpec = m_pDataList->m_HAHA_rNode->resRecord;
		pRecAmt = pRuleResult->resRecord;
		while (pRecSpec) {

//      같은 담보코드가 중복해서 나올때 Sum하도록 수정함.   2006.11.05
//			if ( strncmp( pRecSpec->value[0].strVal, buf[i],strlen(buf[i]) ) == 0 )	break;

			if (strncmp(pRecSpec->value[0].strVal, buf[i], strlen(buf[i]))
					== 0) {
				MsgTVal.value.dblVal += pRecAmt->value[0].dblVal;
			}

			pRecSpec = pRecSpec->pNext;
			pRecAmt = pRecAmt->pNext;
		}

//		if ( ! pRecSpec ) continue;

//		MsgTVal.value.dblVal += pRecAmt->value[0].dblVal;
	}

#endif

	PushOperand(&MsgTVal);

	return true;
}

	bool CMRExpression::Func_HD_SPC5(int cnt, char buf[][STR_VALUE_SIZE+1]) {



//  DAMBOLAST : 가입한 약관코드(마지막놈)

	TYPE_VALUE CntTVal;

	CntTVal.dataType = DATA_TYPE_STRING;
	CntTVal.value.strVal[0] = 0;

#ifndef _PARSER
	RULE_RESULT *pRuleResult;
	RESULT_REC *pRec;

	if (rId_DAMBOLIST == 0) {
		throw new CMTException(UERR_ENV_FUNC, EMSG_ENV_FUNC);
	}

	pRuleResult = m_pDataList->GetRuleResult(rId_DAMBOLIST, (void*) 0);

	if (!pRuleResult) {

		CMRRunner Runner(rId_DAMBOLIST, "", m_pDataList, 0);

		pRuleResult = Runner.getRuleResultPtr();

		if (!pRuleResult)
			throw new CMTException(77777, "Rule running error.");
	}

	for (int i = cnt - 1; i >= 0; i--) {
		pRec = pRuleResult->resRecord;

		while (pRec) {
			if (strncmp(pRec->value[0].strVal, buf[i], strlen(buf[i])) == 0) {
				strcpy(CntTVal.value.strVal, buf[i]);
				i = 0; //  종료하기 위함
				break;
			}
			pRec = pRec->pNext;
		}
	}

#endif

	PushOperand(&CntTVal);

	return true;
}

