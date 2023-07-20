// MRToken.cpp: implementation of the CMRToken class.
//
//////////////////////////////////////////////////////////////////////
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <ctype.h>

#ifndef _OS390
#include <malloc.h>
#endif

#include  "MRERROR.h"
#include  "MRDEFINE.h"
#include  "MRTOKEN.h"
#include  "MRFUNCTN.h"
#include  "MRPARSE.h"
#include  "MRGBLVAR.h"
#include  "MTUtil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//unsigned char	bIndexUsed;
char UsedIndex[5];

CMRToken::CMRToken(char *Buffer) {
	pos = Buffer;
	m_lCode = 0;

	prev_tokType = 0;
}

CMRToken::~CMRToken() {

}

long CMRToken::GetECode(void) const {
	return m_lCode;
}

CRULE_STMT * CMRToken::getToken() {
	CRULE_STMT *pNew;
	TOKEN_RULE *Rule;

	tokType = 0;
	ruleId = 0;
	tokInfo[0] = 0;
	resIndex = 0;
	token[0] = 0;
//	printf(" CMRToken::getToken() 01 \n");
	getNextToken();
//	printf(" CMRToken::getToken() 02 \n");

	if (m_lCode)
		return (CRULE_STMT *) -1;
	if (tokType == 0)
		return (CRULE_STMT *) 0;

	//getNextToken()에서 토큰단위로 분리해놓으면 아래에서
	//메모리를 할당하여 값을 Assign한후 리턴한다..
//	pNew = (CRULE_STMT *) malloc( sizeof(CRULE_STMT) );

	pNew = (CRULE_STMT *) pCMRGblVars->MetisAlloc(sizeof(CRULE_STMT));
//	printf(" CMRToken::getToken() 03 \n");

	pNew->pExpression = 0;

	pNew->pNext = 0;

	pNew->tokType = tokType;
 

//	printf(" CMRToken::getToken() 04 tokType[%d] \n", tokType);
	
	switch (tokType) {
	case TOKENTYPE_CONST_F:
		pNew->uni.dblVal = atof(tokInfo);
		break;
	case TOKENTYPE_CONST_N:
		pNew->uni.longVal = atoi(tokInfo);
		break;
	case TOKENTYPE_CONST_B:
		pNew->uni.boolVal = resIndex;
		break;
	case TOKENTYPE_CONST_C:
	case TOKENTYPE_CONST_L:
		pNew->uni.tokInfo = (char *) pCMRGblVars->MetisAlloc(strlen(tokInfo) + 1);
		strcpy((char *) pNew->uni.tokInfo, (char *) tokInfo);
		break;
	case TOKENTYPE_LOCAL:
		pNew->uni.longVal = atoi(tokInfo);
		break;
	case TOKENTYPE_GLOBAL:
		pNew->uni.longVal = atoi(tokInfo);
		break;
	case TOKENTYPE_RULE:
		Rule = (TOKEN_RULE *) pCMRGblVars->MetisAlloc(sizeof(TOKEN_RULE));
		Rule->ruleId = ruleId;

	//	printf(" CMRToken::getToken() 04-01[%d] \n", Rule->ruleId);
		if (tokInfo[0] != 0) {
			Rule->parmInfo = (char *) pCMRGblVars->MetisAlloc(strlen(tokInfo) + 1);

			strcpy((char *) Rule->parmInfo, (char *) tokInfo);
		//	printf(" CMRToken::getToken() 04-02[%s] \n", (char *)tokInfo);

		} else
			Rule->parmInfo = 0;

		Rule->resIndex = resIndex;

		pNew->uni.tokInfo = Rule;
		break;
	default:
		break;
	}

//	printf(" CMRToken::getToken() 05[%x] \n", pNew);
	prev_tokType = tokType;

	return pNew;
}

void CMRToken::getNextToken() {

	while (*pos>0 && isspace (*pos))
		pos++;

	if (*pos == 0)
		return;

	if (*pos>0 && isalpha (*pos)) {
		getToken_ALPHA();
		return;
	}
 
	switch (*pos) {
	case '"': //  String
		getToken_String();
		break;
	case LOCAL_DELIMITER_LEFT: //  LOCAL VALUE
		getToken_Local();
		break;
	case ITEM_DELIMITER_LEFT: //  ITEM
		getToken_Item();
		break;
	case RULE_DELIMITER_LEFT: //  RULE
		getToken_Rule();
		break;
	case '(': //  LEFT (
		tokType = TOKENTYPE_LEFTP;
		pos++;
		break;
	case ')': //  RIGHT )
		tokType = TOKENTYPE_RIGHTP;
		pos++;
		break;
	case '*': //  MULTIPLY
		pos++;

		if (*pos == '*') {
			tokType = TOKENTYPE_POW;
			pos++;
		} else {
			tokType = TOKENTYPE_MULTIPLY;
		}
		break;
	case '/': //  DIVIDE
		tokType = TOKENTYPE_DIVIDE;
		pos++;
		break;
	case '%': //  MODULUS
		tokType = TOKENTYPE_MODULUS;
		pos++;
		break;
	case '+': //  PLUS
		pos++;
		if (!check_Unary())
			tokType = TOKENTYPE_PLUS;
		break;
	case '-': //  MINUS
		pos++;
		if (!check_Unary())
			tokType = TOKENTYPE_MINUS;
		else {
			tokInfo[0] = '-';
			strcpy(tokInfo + 1, token);
			strcpy(token, tokInfo);
		}
		break;

	case '@':
		if (!strncmp(pos, "@BREAK@", 7)) {
			pos += 7;
			tokType = TOKENTYPE_BREAK;
			break;
		} else if (!strncmp(pos, "@CONTINUE@", 10)) {
			pos += 10;
			tokType = TOKENTYPE_CONTINUE;
			break;
		} else if (!strncmp(pos, "@EXIT@", 6)) {
			pos += 6;
			tokType = TOKENTYPE_EXIT;
			break;
		}

		break;

	case '#': // Index
		if (!isdigit(*(pos + 1))) {
			m_lCode = USERERR_PARSE;
			break;
		}

		if (isalnum(*(pos + 2))) {
			m_lCode = USERERR_PARSE;
			break;
		}

		pos++;
		token[0] = *pos++;
		token[1] = 0;

		strcpy(tokInfo, token);

		switch ( atoi(token) ) {
//				case 0 :
//						tokType = TOKENTYPE_INDEX0;	
//						break;
			case 1 :
			tokType = TOKENTYPE_INDEX1;
			UsedIndex[1] = 'Y';
//						bIndexUsed = 1;
			break;
			case 2 :
			tokType = TOKENTYPE_INDEX2;
			UsedIndex[2] = 'Y';
//						bIndexUsed = 2;
			break;
			case 3 :
			tokType = TOKENTYPE_INDEX3;
			UsedIndex[3] = 'Y';
//						bIndexUsed = 3;
			break;
			default:
			m_lCode = USERERR_PARSE;
			break;
		}
		break;
		case ':' : //  COLON
		tokType = TOKENTYPE_COLON;
		pos++;
		break;
		case ',' ://  COMMA
		tokType = TOKENTYPE_COMMA;
		pos++;
		break;
		case '|' ://  Choice
		tokType = TOKENTYPE_CHOICE;
		pos++;
		break;

		default :
		if (*pos >0 && isdigit( *pos ) ) {
			getToken_Number();
			break;
		}

		if ( *pos == '=' ) {
			tokType = TOKENTYPE_EQUAL;
		} else if ( *pos == '<' ) {
			pos++;
			if ( *pos == '=' ) tokType = TOKENTYPE_LESSEQ;
			else if ( *pos == '>' ) tokType = TOKENTYPE_NOTEQ;
			else {
				tokType = TOKENTYPE_LESS;
				pos--;
			}
		} else if ( *pos == '>' ) {
			pos++;
			if ( *pos == '=' ) tokType = TOKENTYPE_GREATEREQ;
			else {
				tokType = TOKENTYPE_GREATER;
				pos--;
			}
		} else {
			m_lCode = USERERR_PARSE;
		}
		pos++;
		break;
	}
}

void CMRToken::getToken_String() {
	int i = 0;

	pos++;
	while (*pos) {
		 
		if (*pos == '"') {
			token[i] = 0;
//   2006-11-15 문자열 Concat시 SPACE 잘리는것 때문에 막음
//   Compare시 고려가 충분한지 확인을 충분히 해야함.
//			McRTrim(token);
			strcpy(tokInfo, token);

			if (i < STR_VALUE_SIZE)
				tokType = TOKENTYPE_CONST_C;
			else
				tokType = TOKENTYPE_CONST_L;

			pos++;

			return;
		}
		token[i++] = *pos;
		pos++;
	}
	m_lCode = USERERR_PARSE;
}

void CMRToken::getToken_Number() {
	int i = 0;
	char gbn = 'N';

	while (*pos) {
		if ((!isdigit(*pos)) && *pos != '.' && *pos != '#')
			break;
		if (*pos == '.')
			gbn = 'F';
		if (*pos == '#') {
			gbn = '#';
			pos++;
		}

		token[i++] = *pos;
		pos++;
	}

	token[i] = 0;

	// 2010-4-1 vapor 크기가 long형의 최대크기보다 크면 정수로 넣지 않고 실수로 넣는다.
 
	if (token[0] != 0) {
		double  val1 = 2000000000;
		double  val2 = atof(token);
		if (gbn == 'N' && strlen(token) >= 10  ) {
			if ( val1 < val2) {
				gbn = 'F';
			}
		}
	}

	/* 	
	if (gbn == 'N' && strlen(token) >= 10 && strcmp("2000000000", token) < 0)
		gbn = 'F';
*/
	if (gbn == 'F')
		tokType = TOKENTYPE_CONST_F;
	else if (gbn == 'N')
		tokType = TOKENTYPE_CONST_N;
	else {
		if (atoi(token) == 1)
			tokType = TOKENTYPE_INDEX1;
		else if (atoi(token) == 2)
			tokType = TOKENTYPE_INDEX2;
		else
			tokType = TOKENTYPE_INDEX3;
	}
	if (token[0] != 0)

	if (strlen (token))
		strcpy(tokInfo, token);

}

bool CMRToken::check_Unary() {
	if (!isdigit(*pos))
		return false;

	if (prev_tokType != TOKENTYPE_NONE && prev_tokType != TOKENTYPE_LEFTP
			&& (prev_tokType < 30 || prev_tokType > 99))
		return false;

	getToken_Number();

	return true;
}

void CMRToken::getToken_Local() {
	int i = 0;

	pos++;
	while (*pos) {
		if (*pos == LOCAL_DELIMITER_RIGHT) {
			tokInfo[i] = 0;
			tokType = TOKENTYPE_LOCAL;
			pos++;
			return;
		}
		tokInfo[i++] = *pos;
		pos++;
	}
	m_lCode = USERERR_PARSE;
}

void CMRToken::getToken_Item() {
	int i = 0;

	pos++;
	while (*pos) {
		if (*pos == ITEM_DELIMITER_RIGHT) {
			tokInfo[i] = 0;
			tokType = TOKENTYPE_GLOBAL;
			pos++;
			return;
		}
		tokInfo[i++] = *pos;
		pos++;
	}

	m_lCode = USERERR_PARSE;
}

void CMRToken::getToken_Rule() {

	int l_ruleItem_Id;
	char l_tokInfo[2000];
	int opCnt = 0;

	memset(l_tokInfo, 0x00, sizeof(l_tokInfo));
	memset(tokInfo, 0x00, sizeof(tokInfo));

	pos++;
	getToken_Number();
	if ((l_ruleItem_Id = atoi(token)) == 0) {
		m_lCode = USERERR_PARSE;
		return;
	}

	pos++; //  ':' Pass
	/************ Parameter 처리 ***************/
	parseRuleInfo(&pos, l_tokInfo);

//	pos++;
	getToken_Number();

	resIndex = atoi(token);
	tokType = TOKENTYPE_RULE;
	ruleId = l_ruleItem_Id;
	strcpy(tokInfo, l_tokInfo);
	pos++; //  '}' Pass

}

void CMRToken::getToken_ALPHA() {
	switch (*pos) {
	case 'A':
		getToken_ALPHA_A();
		break;
	case 'B':
		getToken_ALPHA_B();
		break;
	case 'C':
		getToken_ALPHA_C();
		break;
	case 'D':
		getToken_ALPHA_D();
		break;
	case 'E':
		getToken_ALPHA_E();
		break;
	case 'F':
		getToken_ALPHA_F();
		break;
	case 'G':
		getToken_ALPHA_G();
		break;
	case 'I':
		getToken_ALPHA_I();
		break;
	case 'L':
		getToken_ALPHA_L();
		break;
	case 'M':
		getToken_ALPHA_M();
		break;
	case 'N':
		getToken_ALPHA_N();
		break;
	case 'O':
		getToken_ALPHA_O();
		break;
	case 'P':
		getToken_ALPHA_P();
		break;
	case 'R':
		getToken_ALPHA_R();
		break;
	case 'S':
		getToken_ALPHA_S();
		break;
	case 'T':
		getToken_ALPHA_T();
		break;
	default:
		m_lCode = USERERR_PARSE;
		break;
	}
}

void CMRToken::getToken_ALPHA_A() {
	if (!strncmp(pos, "AND", 3)) {
		pos += 3;
		tokType = TOKENTYPE_AND;
		return;
	}

	if (!strncmp(pos, "AGE", 3)) {
		pos += 3;
		tokType = TOKENFUNC_AGE;

		checkFuncBegin();

		return;
	}

	if (!strncmp(pos, "ADDVAL", 6)) {
		pos += 6;
		tokType = TOKENFUNC_ADDVAL;

		checkFuncBegin();

		return;
	}

	m_lCode = USERERR_PARSE;
}

void CMRToken::getToken_ALPHA_B() {
	if (!strncmp(pos, "BETWEEN", 7)) {
		pos += 7;
		tokType = TOKENTYPE_BETWEEN;

		checkFuncBegin();

		return;
	}
	m_lCode = USERERR_PARSE;
}

void CMRToken::getToken_ALPHA_C() {
	if (!strncmp(pos, "CLEAR", 5)) {
		pos += 5;
		tokType = TOKENFUNC_RCLEAR;

		checkFuncBegin();

		return;
	}

	if (!strncmp(pos, "CLRVAL", 6)) {
		pos += 6;
		tokType = TOKENFUNC_CLRVAL;

		checkFuncBegin();

		return;
	}

	m_lCode = USERERR_PARSE;
}

void CMRToken::getToken_ALPHA_D() {
	if (!strncmp(pos, "DOUBLE", 6)) {
		pos += 6;
		tokType = TOKENFUNC_TODOUBLE;

		checkFuncBegin();

		return;
	}

	if (!strncmp(pos, "DATEPLUSMON", 11)) {
		pos += 11;
		tokType = TOKENFUNC_DATEPLUSMON;

		checkFuncBegin();

		return;
	}

	if (!strncmp(pos, "DATE", 4)) {
		pos += 4;
		tokType = TOKENFUNC_DATE;

		checkFuncBegin();

		return;
	}

	if (!strncmp(pos, "DAYS", 4)) {
		pos += 4;
		tokType = TOKENFUNC_DURATION;

		checkFuncBegin();

		return;
	}

	if (!strncmp(pos, "DISPLAY", 7)) {
		pos += 7;
		tokType = TOKENFUNC_DISPLAY;

		checkFuncBegin();

		return;
	}

	if (!strncmp(pos, "DAMBONAME", 9)) {
		if (pos[9] == '2') {
			pos += 10;
			tokType = TOKENFUNC_HD_SPC3;
		} else {
			pos += 9;
			tokType = TOKENFUNC_HD_SPC1;
		}

		checkFuncBegin();

		return;
	}

	if (!strncmp(pos, "DAMBOGAIP", 9)) {
		pos += 9;
		tokType = TOKENFUNC_HD_SPC2;

		checkFuncBegin();

		return;
	}

	if (!strncmp(pos, "DAMBOGM", 7)) {
		pos += 7;
		tokType = TOKENFUNC_HD_SPC4;

		checkFuncBegin();

		return;
	}

	m_lCode = USERERR_PARSE;
}

void CMRToken::getToken_ALPHA_E() {
	if (!strncmp(pos, "EXEC", 4)) {
		pos += 4;
		tokType = TOKENFUNC_EXEC;

		checkFuncBegin();

		return;
	}

	if (!strncmp(pos, "EQ", 2)) {
		pos += 2;
		tokType = TOKENTYPE_ARREQUAL;

		checkFuncBegin();

		return;
	}

	if (!strncmp(pos, "EXTERN", 6)) {
		pos += 6;
		tokType = TOKENFUNC_EXTERN;

		checkFuncBegin();

		return;
	}

	m_lCode = USERERR_PARSE;
}

void CMRToken::getToken_ALPHA_F() {
	if (!strncmp(pos, "FALSE", 5)) {
		if (isalnum(*(pos + 5))) {
			m_lCode = USERERR_PARSE;
			return;
		}
		pos += 5;
		tokType = TOKENTYPE_CONST_B;
		resIndex = 0;
		return;
	}

	if (!strncmp(pos, "FINDVAL", 7)) {
		pos += 7;
		tokType = TOKENFUNC_FINDVAL;

		checkFuncBegin();

		return;
	}

	m_lCode = USERERR_PARSE;
}

void CMRToken::getToken_ALPHA_G() {
	if (!strncmp(pos, "GETSVAL", 7)) {
		pos += 7;
		tokType = TOKENFUNC_GETSVAL;

		checkFuncBegin();

		return;
	}

	if (!strncmp(pos, "GETNVAL", 7)) {
		pos += 7;
		tokType = TOKENFUNC_GETNVAL;

		checkFuncBegin();

		return;
	}

	m_lCode = USERERR_PARSE;
}

void CMRToken::getToken_ALPHA_I() {
	if (!strncmp(pos, "INDEXOF", 7)) {
		pos += 7;
		tokType = TOKENFUNC_INDEXOF;

		checkFuncBegin();

		return;
	}

	if (!strncmp(pos, "INCVAL", 6)) {
		pos += 6;
		tokType = TOKENFUNC_INCVAL;

		checkFuncBegin();

		return;
	}

	if (!strncmp(pos, "IN", 2)) {
		pos += 2;
		tokType = TOKENTYPE_IN;

		checkFuncBegin();

		return;
	}

	if (!strncmp(pos, "ISNUM", 5)) {
		pos += 5;
		tokType = TOKENFUNC_ISNUM;

		checkFuncBegin();

		return;
	}

	if (!strncmp(pos, "ISALPHA", 7)) {
		pos += 7;
		tokType = TOKENFUNC_ISALPHA;

		checkFuncBegin();

		return;
	}

	m_lCode = USERERR_PARSE;
}

void CMRToken::getToken_ALPHA_L() {
	if (!strncmp(pos, "LASTDAMBO", 9)) {
		pos += 9;
		tokType = TOKENFUNC_HD_SPC5;

		checkFuncBegin();

		return;
	}

	m_lCode = USERERR_PARSE;
}

void CMRToken::getToken_ALPHA_M() {
	if (!strncmp(pos, "MONTHS", 6)) {
		pos += 6;
		tokType = TOKENFUNC_MONTH;

		checkFuncBegin();

		return;
	}

	if (!strncmp(pos, "MAX", 3)) {
		pos += 3;
		tokType = TOKENFUNC_MAX;

		checkFuncBegin();

		return;
	}

	if (!strncmp(pos, "MIN", 3)) {
		pos += 3;
		tokType = TOKENFUNC_MIN;

		checkFuncBegin();

		return;
	}

	if (!strncmp(pos, "MATCH", 5)) {
		pos += 5;
		tokType = TOKENTYPE_MATCH;

		checkFuncBegin();

		return;
	}

	m_lCode = USERERR_PARSE;
}

void CMRToken::getToken_ALPHA_N() {
	if (!strncmp(pos, "NIN", 3)) {
		pos += 3;
		tokType = TOKENTYPE_NIN;

		checkFuncBegin();

		return;
	}

	if (!strncmp(pos, "NUMBER", 6)) {
		pos += 6;
		tokType = TOKENFUNC_TONUMBER;

		checkFuncBegin();

		return;
	}

	if (!strncmp(pos, "NOT", 3)) {
		pos += 3;
		tokType = TOKENTYPE_NOT;
		return;
	}

	m_lCode = USERERR_PARSE;
}

void CMRToken::getToken_ALPHA_O() {
	if (!strncmp(pos, "OR", 2)) {
		pos += 2;
		tokType = TOKENTYPE_OR;
		return;
	}

	m_lCode = USERERR_PARSE;
}

void CMRToken::getToken_ALPHA_P() {
	if (!strncmp(pos, "PUSHVAL", 7)) {
		pos += 7;
		tokType = TOKENFUNC_PUSHVAL;

		checkFuncBegin();

		return;
	}

	m_lCode = USERERR_PARSE;
}

void CMRToken::getToken_ALPHA_R() {
	if (!strncmp(pos, "RES_AGE", 7)) {
		pos += 7;
		tokType = TOKENFUNC_RES_AGE;

		checkFuncBegin();

		return;
	}

	if (!strncmp(pos, "ROUND", 5)) {
		pos += 5;
		tokType = TOKENFUNC_ROUND;

		checkFuncBegin();

		return;
	}

	if (!strncmp(pos, "ROWS", 4)) {
		pos += 4;
		tokType = TOKENFUNC_ROWS;

		checkFuncBegin();

		return;
	}

	m_lCode = USERERR_PARSE;
}

void CMRToken::getToken_ALPHA_S() {
	if (!strncmp(pos, "SETVAL", 6)) {
		pos += 6;
		tokType = TOKENFUNC_SETVAL;

		checkFuncBegin();

		return;
	}

	if (!strncmp(pos, "STRLEN", 6)) {
		pos += 6;
		tokType = TOKENFUNC_LEN;

		checkFuncBegin();

		return;
	}

	if (!strncmp(pos, "STRING", 6)) {
		pos += 6;
		tokType = TOKENFUNC_TOSTRING;

		checkFuncBegin();

		return;
	}

	if (!strncmp(pos, "SUBSET", 6)) {
		pos += 6;
		tokType = TOKENTYPE_SUBSET;

		checkFuncBegin();

		return;
	}

	if (!strncmp(pos, "SUBSTR", 6)) {
		pos += 6;
		tokType = TOKENFUNC_SUBSTR;

		checkFuncBegin();

		return;
	}

	if (!strncmp(pos, "SUM", 3)) {
		pos += 3;
		tokType = TOKENFUNC_SUM;

		checkFuncBegin();

		return;
	}

	m_lCode = USERERR_PARSE;
}

void CMRToken::getToken_ALPHA_T() {
	if (!strncmp(pos, "TRUE", 4)) {
		if (isalnum(*(pos + 4))) {
			m_lCode = USERERR_PARSE;
			return;
		}
		pos += 4;
		tokType = TOKENTYPE_CONST_B;
		resIndex = 1;
		return;
	}

	if (!strncmp(pos, "TODAY", 5)) {
		pos += 5;
		tokType = TOKENFUNC_TODAY;

		checkFuncBegin();

		return;
	}

	if (!strncmp(pos, "TRUNC", 5)) {
		pos += 5;
		tokType = TOKENFUNC_TRUNC;

		checkFuncBegin();

		return;
	}

	m_lCode = USERERR_PARSE;
}

void CMRToken::checkFuncBegin() {
	while (isspace (*pos))
		pos++;

	if (*pos != '(') {
		m_lCode = USERERR_PARSE;
		return;
	}

	pos++;
}

char * CMRToken::getParenExpression() {
	int pCnt = 0;
	char *pStr;

	while (isspace (*pos))
		pos++;

	pStr = pos;

	while (*pos) {
		if (*pos == '(')
			pCnt++;

		if (*pos == ')') {
			if (pCnt == 0) {
				*pos++ = 0;
				return pStr;
			} else
				pCnt--;
		}

		pos++;
	}

	return 0;
}

long CMRToken::parseRuleInfo(char **pQ, char *pR) {
	int i;
	int leftQ = 0, leftPcnt = 0;
	int SOSI_OPEN = 0;

	if (*pos == NULL) {
		*pR = 0;
		return 1;
	}

	i = 0;
	while (*pos) {
		if (++i == 1000)
			return 0;

		if (leftQ) {
			if (*pos == '\"')
				leftQ = 0;
			*pR++ = *pos;
			pos++;

			continue;
		}

		if (SOSI_OPEN) {
			if (*pos == 0x0f)
				SOSI_OPEN = 0;
			*pR++ = *pos;
			pos++;
			continue;
		}

		if (*pos == RULE_DELIMITER_RIGHT || *pos == ':') {
			if (leftPcnt == 0) {
				pos++;
				*pR = 0;
				return 1;
			}
		}

		switch (*pos) {
		case RULE_DELIMITER_LEFT:
			leftPcnt++;
			break;
		case RULE_DELIMITER_RIGHT:
			leftPcnt--;
			break;
		case 0x0e:
			SOSI_OPEN = 1;
			break;
		case '\"':
			leftQ = 1;
			break;
		default:
			break;
		}

		*pR++ = *pos;
		pos++;
	}

	return 0;
}

/*
 long    CMRToken::parseRuleInfo( char **pS, char *pR )
 {
 int	 i;
 int  leftQ=0, leftPcnt=0;
 int  SOSI_OPEN = 0;

 if ( **pS == NULL ) {
 *pR = 0;
 return 1;
 }

 i = 0;
 while( **pS )
 {
 if( ++i == 100 ) return 0;

 if ( leftQ ) {
 if( **pS == '\"' ) leftQ = 0;
 *pR++ = **pS;
 (*pS)++;

 continue;
 }

 if ( SOSI_OPEN  ) {
 if( **pS == 0x0f ) SOSI_OPEN = 0;
 *pR++ = **pS;
 (*pS)++;
 continue;
 }


 if ( **pS == RULE_DELIMITER_RIGHT || **pS == ':' ) {
 if ( leftPcnt == 0 ) {
 (*pS)++;
 *pR = 0;
 return 1;
 }
 }


 switch( **pS ) {
 case RULE_DELIMITER_LEFT  : leftPcnt++;    break;
 case RULE_DELIMITER_RIGHT : leftPcnt--;    break;
 case 0x0e : SOSI_OPEN = 1;    break;
 case '\"' : leftQ = 1;        break;
 default   : break;
 }


 *pR++ = **pS;   (*pS)++;
 }

 return 0;
 }

 */
