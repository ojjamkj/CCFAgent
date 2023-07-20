#include    <stdio.h>
#include    <stdlib.h>
#include    <ctype.h>
#include    <string.h>
#include    <math.h>
#include    <time.h>

#include   "MRSTRUCT.h"
#include   "MRFUNCTN.h"

extern unsigned long calc_result_rc;
extern char calc_result_msg[256];

unsigned long FUNC_SUBSTR(OPERAND_STACK *val1, OPERAND_STACK *val2,
		OPERAND_STACK *val3, char *sTemp) {
	calc_result_rc = true;
	sTemp[0] = 0;

#ifdef _PARSER
	if ( val1->tVal.dataType != DATA_TYPE_STRING &&
			val1->tVal.dataType != DATA_TYPE_LSTRING ) {
		calc_result_rc = false;
		return false;
	}
	if ( val2->tVal.dataType != DATA_TYPE_LONG ) {
		calc_result_rc = false;
		return false;
	}
	if ( val3->tVal.dataType != DATA_TYPE_LONG ) {
		calc_result_rc = false;
		return false;
	}
#else
	int len2 = val2->tVal.value.longVal;
	int len3 = val3->tVal.value.longVal;

	if (len2 < 1)
		len2 = 1;
	if (len3 < 1)
		return true;

	if (val1->tVal.dataType == DATA_TYPE_STRING) {
		int len1 = strlen(val1->tVal.value.strVal);
		if (len2 > len1) {
			val1->tVal.value.strVal[0] = 0;
		} else {
			len2--;

			strncpy(val1->tVal.value.strVal, val1->tVal.value.strVal + len2,
					len3);

			val1->tVal.value.strVal[len3] = 0;
		}
	} else { // DATA_TYPE_LSTRING
		if (!val1->tVal.value.pLStr)
			return true;

		int len1 = strlen(val1->tVal.value.pLStr);

		if (len2 > len1) {
			val1->tVal.value.pLStr = 0;
		} else {
			len2--;

			strncpy(sTemp, val1->tVal.value.pLStr + len2, len3);
			sTemp[len3] = 0;
		}
	}
#endif

	return true;
}

unsigned long FUNC_TODAY(TYPE_VALUE *Value, char *mark) {
	Value->dataType = DATA_TYPE_STRING;
	CurrentDate(Value->value.strVal, mark);

	return true;
}

unsigned long FUNC_DATEPLUSMON(OPERAND_STACK *val1, OPERAND_STACK *val2,
		OPERAND_STACK *val3) {
//  Date(val1->tVal.value.strVal) + Month(val2->tVal.value.longVal) =>  날짜
//  Opt(val3->tVal.value.longVal) : 'Z' 말일적용
	long nVal = 0;

#ifdef _PARSER
	if ( val1->tVal.dataType != DATA_TYPE_STRING) {
		calc_result_rc = false;
		return false;
	}

	strcpy( val1->tVal.value.strVal,"2000-01-01");

	if ( val2->tVal.dataType != DATA_TYPE_LONG ) {
		calc_result_rc = false;
		return false;
	}
	val2->tVal.value.longVal = 0;

	if ( val3 ) {
		if ( val3->tVal.dataType != DATA_TYPE_LONG ) {
			calc_result_rc = false;
			return false;
		}
	}
#else
	if (val3)
		nVal = val3->tVal.value.longVal;
#endif

	val1->tVal.dataType = DATA_TYPE_STRING;
	DatePulsMonth(val1->tVal.value.strVal, val2->tVal.value.longVal, nVal);

	if (!val1->tVal.value.strVal[0]) {
		calc_result_rc = false;
		return false;
	}

	return true;
}

unsigned long FUNC_AGE(OPERAND_STACK *val1, OPERAND_STACK *val2) {
	calc_result_rc = true;

#ifdef _PARSER
	if ( val1->tVal.dataType != DATA_TYPE_STRING ) {
		calc_result_rc = false;
		return false;
	}
	if ( val2->tVal.dataType != DATA_TYPE_STRING ) {
		calc_result_rc = false;
		return false;
	}

	strcpy( val1->tVal.value.strVal, "2000-01-01" );
	strcpy( val2->tVal.value.strVal, "2000-01-01" );
#endif

	val1->tVal.value.longVal = Age(val1->tVal.value.strVal,
			val2->tVal.value.strVal);

	if (val1->tVal.value.longVal < 0) {
		calc_result_rc = false;
		return false;
	}

	val1->tVal.dataType = DATA_TYPE_LONG;

	return (true);
}

unsigned long FUNC_RES_AGE(OPERAND_STACK *val1) {
//  resNo(val1->tVal.value.strVal)  의 만연령

#ifdef _PARSER
	if ( val1->tVal.dataType != DATA_TYPE_STRING ) {
		calc_result_rc = false;
		return false;
	}

	val1->tVal.value.longVal = 1;
#else
	val1->tVal.value.longVal = CalcResAge(val1->tVal.value.strVal);

	if (val1->tVal.value.longVal < 0) {
		calc_result_rc = false;
		return false;
	}
#endif

	val1->tVal.dataType = DATA_TYPE_LONG;
	return true;
}

unsigned long FUNC_RESAGE_BYDATE(OPERAND_STACK *val1, OPERAND_STACK *val2,
		short gbn) {
//  resNo(val1->tVal.value.strVal) ~ val2->tVal.value.strVal  의 만연령(보험연령)

#ifdef _PARSER
	if ( val1->tVal.dataType != DATA_TYPE_STRING ) {
		calc_result_rc = false;
		return false;
	}

	if ( val2->tVal.dataType != DATA_TYPE_STRING) {
		calc_result_rc = false;
		return false;
	}
	val1->tVal.value.longVal = 1;
#else
	val1->tVal.value.longVal = CalcResAgeByDate(val1->tVal.value.strVal,
			val2->tVal.value.strVal, gbn);

	if (val1->tVal.value.longVal < 0) {
		calc_result_rc = false;
		return false;
	}
#endif

	val1->tVal.dataType = DATA_TYPE_LONG;
	return true;
}

/*
 unsigned long	FUNC_RESAGE_BYDATE2( OPERAND_STACK *val1, OPERAND_STACK *val2 )
 {
 //  resNo(val1->tVal.value.strVal) ~ val2->tVal.value.strVal  의 반올림연령

 #ifdef _PARSER
 if ( val1->tVal.dataType == DATA_TYPE_VOID ) {
 val1->tVal.dataType = DATA_TYPE_STRING;
 strcpy( val1->tVal.value.strVal, "600101123467" );
 }
 if ( val2->tVal.dataType == DATA_TYPE_VOID ) {
 val2->tVal.dataType = DATA_TYPE_STRING;
 strcpy( val2->tVal.value.strVal, "2000-01-01" );
 }

 if ( val1->tVal.dataType != DATA_TYPE_STRING ) {
 calc_result_rc = false;
 return false;
 }

 strcpy( val1->tVal.value.strVal,"6710202092827");


 if ( val2->tVal.dataType != DATA_TYPE_STRING) {
 calc_result_rc = false;
 return false;
 }

 strcpy( val2->tVal.value.strVal,"1990-01-01");
 #endif

 val1->tVal.dataType = DATA_TYPE_LONG;
 val1->tVal.value.longVal = CalcResAgeByDate( val1->tVal.value.strVal, val2->tVal.value.strVal, 0 );

 if ( val1->tVal.value.longVal < 0 ) {
 calc_result_rc = false;
 return false;
 }

 return true;
 }
 */

unsigned long FUNC_DATE(OPERAND_STACK *val1, OPERAND_STACK *val2) {
//  val1->tVal.value.strVal + val2->tVal.value.longVal 의 날짜

#ifdef _PARSER
	if ( val1->tVal.dataType != DATA_TYPE_STRING) {
		calc_result_rc = false;
		return false;
	}
	if ( val2->tVal.dataType != DATA_TYPE_LONG) {
		calc_result_rc = false;
		return false;
	}

	strcpy( val1->tVal.value.strVal, "2000-01-01" );
#else
	if (!DateTypeCheck(val1->tVal.value.strVal)) {
		calc_result_rc = false;
		return false;
	}

	DatePulsNumber(val1->tVal.value.strVal, val2->tVal.value.longVal);
#endif

	return true;
}

unsigned long FUNC_DURATION(OPERAND_STACK *val1, OPERAND_STACK *val2) {
//  val1->tVal.value.strVal ~ val2->tVal.value.strVal 의 일수

#ifdef _PARSER
	if ( val1->tVal.dataType != DATA_TYPE_STRING) {
		calc_result_rc = false;
		return false;
	}
	if ( val2->tVal.dataType != DATA_TYPE_STRING) {
		calc_result_rc = false;
		return false;
	}

	val1->tVal.value.longVal = 1;
#else
	if (!DateTypeCheck(val1->tVal.value.strVal)) {
		calc_result_rc = false;
		return false;
	}

	if (!DateTypeCheck(val2->tVal.value.strVal)) {
		calc_result_rc = false;
		return false;
	}

	val1->tVal.value.longVal = DateDuration(val1->tVal.value.strVal,
			val2->tVal.value.strVal);
#endif

	val1->tVal.dataType = DATA_TYPE_LONG;

	return true;
}

//  수정해야함
unsigned long FUNC_MONTHS(OPERAND_STACK *val1, OPERAND_STACK *val2, short gbn) {
	calc_result_rc = true;

#ifdef _PARSER
	if ( val1->tVal.dataType != DATA_TYPE_STRING ) {
		calc_result_rc = false;
		return false;
	}
	if ( val2->tVal.dataType != DATA_TYPE_STRING ) {
		calc_result_rc = false;
		return false;
	}

	strcpy( val1->tVal.value.strVal, "2000-01-01" );
	strcpy( val2->tVal.value.strVal, "2000-01-01" );
#endif

//printf("    MONTHS !%s!%s!%d\n", val1->tVal.value.strVal, val2->tVal.value.strVal, gbn ); 

	val1->tVal.value.longVal = months(val1->tVal.value.strVal,
			val2->tVal.value.strVal, gbn);
	val1->tVal.dataType = DATA_TYPE_LONG;

	return (true);
}

unsigned long FUNC_ROUND_KS(OPERAND_STACK *val1, OPERAND_STACK *val2) {
	long m, ldiv = 1;
	double ddiv = 1.0;
	calc_result_rc = true;

#ifdef _PARSER
	if ( val2->tVal.dataType != DATA_TYPE_LONG ) {
		calc_result_rc = false;
		return false;
	}
#endif

	m = val2->tVal.value.longVal;

	if (val1->tVal.dataType == DATA_TYPE_DOUBLE) {
		val1->tVal.value.dblVal = RRound(val1->tVal.value.dblVal, m);
	} else if (val1->tVal.dataType == DATA_TYPE_LONG) {
		if (m >= 0)
			return true;

		for (int i = m; i < 0; i++)
			ldiv *= 10;

		val1->tVal.value.longVal = (val1->tVal.value.longVal / ldiv) * ldiv;
	} else {
		calc_result_rc = false;
		return false;
	}

	return (true);
}

unsigned long FUNC_ROUND(OPERAND_STACK *val1, OPERAND_STACK *val2) {
	long m, ldiv = 1;
	double ddiv = 1.0;
	calc_result_rc = true;

#ifdef _PARSER
	if ( val2->tVal.dataType != DATA_TYPE_LONG ) {
		calc_result_rc = false;
		return false;
	}
#endif

	m = val2->tVal.value.longVal;

	if (val1->tVal.dataType == DATA_TYPE_DOUBLE) {
		val1->tVal.value.dblVal = RRound(val1->tVal.value.dblVal, m);
	} else if (val1->tVal.dataType == DATA_TYPE_LONG) {
		if (m >= 0)
			return true;

		for (int i = m; i < -1; i++) {
			ldiv *= 10;
			val1->tVal.value.longVal /= 10;
		}

		val1->tVal.value.longVal += 5;

		ldiv *= 10;
		val1->tVal.value.longVal /= 10;

		val1->tVal.value.longVal *= ldiv;
	} else {
		calc_result_rc = false;
		return false;
	}

	return (true);
}

unsigned long FUNC_TRUNC(OPERAND_STACK *val1, OPERAND_STACK *val2) {
	long m, ldiv = 1;
	double ddiv = 1.0;
	calc_result_rc = true;

#ifdef _PARSER
	if ( val2->tVal.dataType != DATA_TYPE_LONG ) {
		calc_result_rc = false;
		return false;
	}
#endif

	m = val2->tVal.value.longVal;

	if (val1->tVal.dataType == DATA_TYPE_DOUBLE) {
		val1->tVal.value.dblVal = RTrunc(val1->tVal.value.dblVal, m);

	} else if (val1->tVal.dataType == DATA_TYPE_LONG) {
		if (m >= 0)
			return true;

		for (int i = m; i < 0; i++)
			ldiv *= 10;

		val1->tVal.value.longVal = (val1->tVal.value.longVal / ldiv) * ldiv;
	} else {
		calc_result_rc = false;
		return false;
	}

	return (true);
}

unsigned long FUNC_STR_TYPE(short gbn, OPERAND_STACK *val1) {
#ifdef _PARSER
	if ( val1->tVal.dataType != DATA_TYPE_STRING &&
			val1->tVal.dataType != DATA_TYPE_LSTRING ) {
		calc_result_rc = false;
		return false;
	}

	val1->tVal.value.boolVal = 1;
#else
	char *s;
	if (val1->tVal.dataType == DATA_TYPE_STRING)
		s = val1->tVal.value.strVal;
	else
		s = val1->tVal.value.pLStr;

	while (*s) {
		if (gbn == 1) {
			if (!isdigit(*s))
				break;
		} else {
			if (!isalpha(*s))
				break;
		}
		s++;
	}

	if (*s)
		val1->tVal.value.boolVal = false;
	else
		val1->tVal.value.boolVal = true;
#endif

	val1->tVal.dataType = DATA_TYPE_BOOL;
	return true;
}

unsigned long FUNC_TONUMBER(OPERAND_STACK *val1) {
//	long lVal;

#ifdef _PARSER
	if ( val1->tVal.dataType != DATA_TYPE_STRING &&
			val1->tVal.dataType != DATA_TYPE_DOUBLE &&
			val1->tVal.dataType != DATA_TYPE_LONG ) {
		calc_result_rc = false;
		return false;
	}
#endif

	if (val1->tVal.dataType == DATA_TYPE_LONG)
		return true;

	if (val1->tVal.dataType == DATA_TYPE_STRING)
		val1->tVal.value.longVal = atoi(val1->tVal.value.strVal);
	else {
		/*
		 lVal = val1->tVal.value.dblVal;
		 if ( lVal == val1->tVal.value.dblVal )
		 val1->tVal.value.longVal = lVal;
		 if ( lVal < val1->tVal.value.dblVal )
		 val1->tVal.value.longVal = lVal + 1;
		 else
		 val1->tVal.value.longVal = lVal - 1;
		 */
		char *p, temp[31];
		int ch = '.';
		sprintf(temp, "%lf", val1->tVal.value.dblVal);

		p = strchr(temp, ch);
		*p = 0;
		val1->tVal.value.longVal = atoi(temp);
	}

	val1->tVal.dataType = DATA_TYPE_LONG;
	return true;
}

unsigned long FUNC_TODOUBLE(OPERAND_STACK *val1) {
//	long lVal;

#ifdef _PARSER
	if ( val1->tVal.dataType != DATA_TYPE_STRING &&
			val1->tVal.dataType != DATA_TYPE_DOUBLE &&
			val1->tVal.dataType != DATA_TYPE_LONG ) {
		calc_result_rc = false;
		return false;
	}
#endif

	if (val1->tVal.dataType == DATA_TYPE_DOUBLE)
		return true;

	if (val1->tVal.dataType == DATA_TYPE_STRING){
		val1->tVal.value.dblVal = atof(val1->tVal.value.strVal);

	}else {
		val1->tVal.value.dblVal = val1->tVal.value.longVal;
	
	}

	val1->tVal.dataType = DATA_TYPE_DOUBLE;
	return true;
}

unsigned long FUNC_TOSTRING(OPERAND_STACK *val1) {
	if (val1->tVal.dataType == DATA_TYPE_LSTRING)
		return true;

	char *p = val1->tVal.value.strVal;

	switch (val1->tVal.dataType) {
	case DATA_TYPE_DOUBLE:
		sprintf(p, "%lf", val1->tVal.value.dblVal);

		p = p + strlen(p) - 1;

		while (*p) {
			if (*p == '.') {
				*p = 0;
				break;
			}

			if (*p != '0')
				break;

			*p-- = 0;
		}
		break;
	case DATA_TYPE_LONG:
		sprintf(p, "%ld", val1->tVal.value.longVal);
		break;
	case DATA_TYPE_BOOL:
		sprintf(p, "%d", val1->tVal.value.boolVal);
		break;
	default:
		break;
	}

	val1->tVal.dataType = DATA_TYPE_STRING;
	return true;
}
