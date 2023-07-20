#include <stdio.h>
#include <stdlib.h>

#if defined(WIN32) || defined(_OS390) || defined(__GNUC__)
#include    <string.h>
#else
#include    <strings.h>
#endif

#include <locale.h>
#include <math.h>

#include   "MTUtil.h"
#include   "MRSTRUCT.h"
#include   "MRDTypes.h"
#include   "MRFUNCTN.h"

//#define		FORCE_ANSI      0x10000
//#define		FORCE_UNICODE   0x20000
//#define		FORCE_INT64     0x40000

//#define		WCHAR_ARG		wchar_t
//#define		CHAR_ARG		char

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern unsigned long calc_result_rc;
extern char calc_result_msg[128];

unsigned long CALC_NOT(OPERAND_STACK *val) {
#ifdef _PARSER
	if ( val->tVal.dataType != DATA_TYPE_BOOL && val->tVal.dataType != DATA_TYPE_LONG) {
		calc_result_rc = false;
		return false;
	}
#endif

	if (val->tVal.dataType == DATA_TYPE_BOOL) {
		val->tVal.value.boolVal = (!val->tVal.value.boolVal);
	} else {
		val->tVal.value.boolVal = (!val->tVal.value.longVal);
		val->tVal.dataType = DATA_TYPE_BOOL;
	}

	return true;
}

unsigned long CALC_EQUAL(OPERAND_STACK *val1, OPERAND_STACK *val2) {
	unsigned char t1, t2;

	calc_result_rc = true;
	t1 = val1->tVal.dataType;
	t2 = val2->tVal.dataType;


	//printf("EQUAL : %d %d\n", t1, t2 );

#ifdef _PARSER
	if ( t1 == DATA_TYPE_LSTRING || t2 == DATA_TYPE_LSTRING ) {
		calc_result_rc = false;
		return false;
	}

	val1->tVal.dataType = DATA_TYPE_BOOL;
	val1->tVal.value.boolVal = true;

	if ( t1 == t2 ) return true;

	if ( t1 == DATA_TYPE_STRING || t2 == DATA_TYPE_STRING ) {
		if ( t1 != DATA_TYPE_STRING || t2 != DATA_TYPE_STRING ) {
			calc_result_rc = false;
			return false;
		} else
		return true;
	}

	if ( t1 == DATA_TYPE_BOOL && t2 != DATA_TYPE_LONG ) {
		calc_result_rc = false;
		return false;
	}

	if ( t2 == DATA_TYPE_BOOL && t1 != DATA_TYPE_LONG ) {
		calc_result_rc = false;
		return false;
	}

#else

	if (t1 == DATA_TYPE_BOOL || t2 == DATA_TYPE_BOOL) {
		if (t1 == DATA_TYPE_BOOL)
			val1->tVal.value.longVal = val1->tVal.value.boolVal;
		if (t2 == DATA_TYPE_BOOL)
			val2->tVal.value.longVal = val2->tVal.value.boolVal;

		val1->tVal.value.boolVal = (val1->tVal.value.longVal
				== val2->tVal.value.longVal);
	} else if (t1 == DATA_TYPE_STRING || t2 == DATA_TYPE_STRING) {
		McRTrim(val1->tVal.value.strVal);
		McRTrim(val2->tVal.value.strVal);
		val1->tVal.value.boolVal = (strcmp(val1->tVal.value.strVal,
				val2->tVal.value.strVal) == 0);
	} else if (t1 == DATA_TYPE_DOUBLE || t2 == DATA_TYPE_DOUBLE) {
		if (t1 == DATA_TYPE_LONG)
			val1->tVal.value.dblVal = (double) val1->tVal.value.longVal;

		if (t2 == DATA_TYPE_LONG)
			val2->tVal.value.dblVal = (double) val2->tVal.value.longVal;

		val1->tVal.value.boolVal = (val1->tVal.value.dblVal
				== val2->tVal.value.dblVal);
	} else
		val1->tVal.value.boolVal = (val1->tVal.value.longVal
				== val2->tVal.value.longVal);

	val1->tVal.dataType = DATA_TYPE_BOOL;
#endif

	return (true);
}

unsigned long CALC_NEQUAL(OPERAND_STACK *val1, OPERAND_STACK *val2) {
	if (!CALC_EQUAL(val1, val2))
		return false;

	val1->tVal.value.boolVal = (!val1->tVal.value.boolVal);
	return true;
}

unsigned long CALC_LESS(OPERAND_STACK *val1, OPERAND_STACK *val2) {
	unsigned char t1, t2;

	calc_result_rc = true;
	t1 = val1->tVal.dataType;
	t2 = val2->tVal.dataType;

#ifdef _PARSER
	val1->tVal.dataType = DATA_TYPE_BOOL;
	val1->tVal.value.boolVal = true;

	if ( t1 == DATA_TYPE_BOOL || t2 == DATA_TYPE_BOOL ) {
		calc_result_rc = false;
		return false;
	}

	if ( t1 == t2 ) return true;

	if ( t1 == DATA_TYPE_LSTRING || t2 == DATA_TYPE_LSTRING ) {
		calc_result_rc = false;
		return false;
	}

	if ( t1 == DATA_TYPE_STRING || t2 == DATA_TYPE_STRING ) {
		if ( t1 != DATA_TYPE_STRING || t2 != DATA_TYPE_STRING ) {
			calc_result_rc = false;
			return false;
		} else
		return true;
	}

#else

	if (t1 == DATA_TYPE_STRING || t2 == DATA_TYPE_STRING) {
		McRTrim(val1->tVal.value.strVal);
		McRTrim(val2->tVal.value.strVal);
		val1->tVal.value.boolVal = (strcmp(val1->tVal.value.strVal,
				val2->tVal.value.strVal) < 0);
	} else if (t1 == DATA_TYPE_DOUBLE || t2 == DATA_TYPE_DOUBLE) {
		if (t1 == DATA_TYPE_LONG)
			val1->tVal.value.dblVal = (double) val1->tVal.value.longVal;

		if (t2 == DATA_TYPE_LONG)
			val2->tVal.value.dblVal = (double) val2->tVal.value.longVal;

		val1->tVal.value.boolVal = (val1->tVal.value.dblVal
				< val2->tVal.value.dblVal);
	} else
		val1->tVal.value.boolVal = (val1->tVal.value.longVal
				< val2->tVal.value.longVal);

	val1->tVal.dataType = DATA_TYPE_BOOL;
#endif

	return (true);
}

unsigned long CALC_GREATER(OPERAND_STACK *val1, OPERAND_STACK *val2) {
	unsigned char t1, t2;

	calc_result_rc = true;
	t1 = val1->tVal.dataType;
	t2 = val2->tVal.dataType;

#ifdef _PARSER
	val1->tVal.dataType = DATA_TYPE_BOOL;
	val1->tVal.value.boolVal = true;

	if ( t1 == DATA_TYPE_BOOL || t2 == DATA_TYPE_BOOL ) {
		calc_result_rc = false;
		return false;
	}

	if ( t1 == t2 ) return true;

	if ( t1 == DATA_TYPE_LSTRING || t2 == DATA_TYPE_LSTRING ) {
		calc_result_rc = false;
		return false;
	}

	if ( t1 == DATA_TYPE_STRING || t2 == DATA_TYPE_STRING ) {
		if ( t1 != DATA_TYPE_STRING || t2 != DATA_TYPE_STRING ) {
			calc_result_rc = false;
			return false;
		} else
		return true;
	}

#else

	if (t1 == DATA_TYPE_STRING || t2 == DATA_TYPE_STRING) {
		McRTrim(val1->tVal.value.strVal);
		McRTrim(val2->tVal.value.strVal);
		val1->tVal.value.boolVal = (strcmp(val1->tVal.value.strVal,
				val2->tVal.value.strVal) > 0);
	} else if (t1 == DATA_TYPE_DOUBLE || t2 == DATA_TYPE_DOUBLE) {
		if (t1 == DATA_TYPE_LONG)
			val1->tVal.value.dblVal = (double) val1->tVal.value.longVal;

		if (t2 == DATA_TYPE_LONG)
			val2->tVal.value.dblVal = (double) val2->tVal.value.longVal;
//syslog(LOG_INFO, "HAHAHAHAHA\n");

		val1->tVal.value.boolVal = (val1->tVal.value.dblVal
				> val2->tVal.value.dblVal);
	} else
		val1->tVal.value.boolVal = (val1->tVal.value.longVal
				> val2->tVal.value.longVal);

	val1->tVal.dataType = DATA_TYPE_BOOL;
#endif

	return (true);

}

unsigned long CALC_GREATEREQ(OPERAND_STACK *val1, OPERAND_STACK *val2) {
	if (!CALC_LESS(val1, val2))
		return false;

	val1->tVal.value.boolVal = (!val1->tVal.value.boolVal);

	return true;
}

unsigned long CALC_LESSEQ(OPERAND_STACK *val1, OPERAND_STACK *val2) {
	if (!CALC_GREATER(val1, val2))
		return false;

	val1->tVal.value.boolVal = (!val1->tVal.value.boolVal);

	return true;
}

void CONV_F2S(char *sVal, double dVal) {
	char buf[STR_VALUE_SIZE+1], *p;
	int i, j, len, zz;

	sprintf(buf, "%lf", dVal);
	buf[STR_VALUE_SIZE]=0;
	len = strlen(buf);

	while (1) {
		if (buf[len - 1] != '0')
			break;
		len--;
	}
	buf[len] = 0;

	if (buf[len - 1] == '.') {
		buf[--len] = 0;
	} else {
		p = strchr(buf, '.');
		if (p)
			len = p - buf;
	}

	if (len < 4) {
		strcpy(sVal, buf);

		return;
	}

	if ((zz = len % 3) == 0)
		zz = 3;
	else if (zz == 1 && buf[0] == '-')
		zz = 4;

	i = j = 0;
	while (i < len - zz) {
		strncpy(sVal + j, buf + i, zz);
		i += zz;
		j += zz;
		zz = 3;
		sVal[j++] = ',';
	}

	strcpy(sVal + j, buf + i);
}

unsigned long CALC_PLUS(OPERAND_STACK *val1, OPERAND_STACK *val2, char *sTemp) {
	unsigned char t1, t2;
	VALUE_UNION *v1, *v2;
	t1 = val1->tVal.dataType;
	t2 = val2->tVal.dataType;
	v1 = &val1->tVal.value;
	v2 = &val2->tVal.value;
	calc_result_rc = true;

	sTemp[0] = 0;

#ifdef _PARSER
	if ( t1 == DATA_TYPE_BOOL || t2 == DATA_TYPE_BOOL ) {
		calc_result_rc = false;
		return false;
	}

	if ( t1 == t2 ) return true;
#endif

	if (t1 == DATA_TYPE_STRING || t1 == DATA_TYPE_LSTRING
			|| t2 == DATA_TYPE_STRING || t2 == DATA_TYPE_LSTRING) {

		switch (t1) {
		case DATA_TYPE_LONG:
			sprintf(sTemp, "%ld", v1->longVal);  //lf->ld·Îº¯°æ
			break;
		case DATA_TYPE_DOUBLE:
			CONV_F2S(sTemp, v1->dblVal);
			break;
		case DATA_TYPE_STRING:
			strcpy(sTemp, v1->strVal);
			break;
		case DATA_TYPE_LSTRING:
			if (v1->pLStr)
				strcpy(sTemp, v1->pLStr);
			else
				sTemp[0] = 0;
			break;
		}

		int nn = strlen(sTemp);

		switch (t2) {
		case DATA_TYPE_LONG:
			sprintf(sTemp + nn, "%ld", v2->longVal);
			break;
		case DATA_TYPE_DOUBLE:
//			sprintf( sTemp+nn,"%lf",val2->tVal.value.dblVal );
			CONV_F2S(sTemp + nn, v2->dblVal);
			break;
		case DATA_TYPE_STRING:
			strcpy(sTemp + nn, v2->strVal);
			break;
		case DATA_TYPE_LSTRING:
			if (v2->pLStr)
				strcpy(sTemp + nn, v2->pLStr);
			break;
		}

		if (strlen(sTemp) > STR_VALUE_SIZE) {
			val1->tVal.dataType = DATA_TYPE_LSTRING;
		} else {
			val1->tVal.dataType = DATA_TYPE_STRING;
			strcpy(v1->strVal, sTemp);
			sTemp[0] = 0;
		}
	} else if (t1 == DATA_TYPE_DOUBLE || t2 == DATA_TYPE_DOUBLE) {
		if (t1 == DATA_TYPE_LONG)
			val1->tVal.value.dblVal = (double) val1->tVal.value.longVal;

		if (t2 == DATA_TYPE_LONG)
			val2->tVal.value.dblVal = (double) val2->tVal.value.longVal;

		val1->tVal.value.dblVal += val2->tVal.value.dblVal;
		val1->tVal.dataType = DATA_TYPE_DOUBLE;
	} else {
		val1->tVal.value.longVal += val2->tVal.value.longVal;
		val1->tVal.dataType = DATA_TYPE_LONG;
	}

	return (true);
}

unsigned long CALC_MINUS(OPERAND_STACK *val1, OPERAND_STACK *val2) {
	unsigned char t1, t2;
	t1 = val1->tVal.dataType;
	t2 = val2->tVal.dataType;

	calc_result_rc = true;

#ifdef _PARSER
	if ( (t1 == DATA_TYPE_LONG || t1 == DATA_TYPE_DOUBLE) &&
			(t2 == DATA_TYPE_LONG || t2 == DATA_TYPE_DOUBLE) ) {

	} else {
		calc_result_rc = false;
		return false;
	}
#endif

	if (t1 == DATA_TYPE_DOUBLE || t2 == DATA_TYPE_DOUBLE) {
		if (t1 == DATA_TYPE_LONG)
			val1->tVal.value.dblVal = (double) val1->tVal.value.longVal;

		if (t2 == DATA_TYPE_LONG)
			val2->tVal.value.dblVal = (double) val2->tVal.value.longVal;

		val1->tVal.value.dblVal -= val2->tVal.value.dblVal;
		val1->tVal.dataType = DATA_TYPE_DOUBLE;
	} else {
		val1->tVal.value.longVal -= val2->tVal.value.longVal;
		val1->tVal.dataType = DATA_TYPE_LONG;
	}

	return (true);
}

double FP_Round(double dVal) {
	double dTemp, prec = 100000.0, sign = 1.0;
    double dVal2;
	if (dVal < 0.0) {
		sign = -1.0;
		dVal *= -1.0;
	}
	dVal2=dVal * prec;
	dTemp = floor(getDoubleToString(dVal2)); //floor(dVal * prec);

	if ((dVal * prec - dTemp) < 0.5)
		dVal = dTemp / prec;
	else
		dVal = (dTemp + 1.0) / prec;

	return dVal * sign;
}

unsigned long CALC_MULTI(OPERAND_STACK *val1, OPERAND_STACK *val2) {
	unsigned char t1, t2;
	t1 = val1->tVal.dataType;
	t2 = val2->tVal.dataType;

	calc_result_rc = true;

#ifdef _PARSER
	if ( (t1 == DATA_TYPE_LONG || t1 == DATA_TYPE_DOUBLE) &&
			(t2 == DATA_TYPE_LONG || t2 == DATA_TYPE_DOUBLE) ) {

	} else {
		calc_result_rc = false;
		return false;
	}
#endif

	if (t1 == DATA_TYPE_DOUBLE || t2 == DATA_TYPE_DOUBLE) {
		if (t1 == DATA_TYPE_LONG)
			val1->tVal.value.dblVal = (double) val1->tVal.value.longVal;

		if (t2 == DATA_TYPE_LONG)
			val2->tVal.value.dblVal = (double) val2->tVal.value.longVal;

		val1->tVal.value.dblVal *= val2->tVal.value.dblVal;

#ifdef _OS390
		val1->tVal.value.dblVal = FP_Round( val1->tVal.value.dblVal );
#endif

		val1->tVal.dataType = DATA_TYPE_DOUBLE;
	} else {
		val1->tVal.value.longVal *= val2->tVal.value.longVal;
		val1->tVal.dataType = DATA_TYPE_LONG;
	}

	return (true);

}

unsigned long CALC_DIVIDE(OPERAND_STACK *val1, OPERAND_STACK *val2) {
	unsigned char t1, t2;
	t1 = val1->tVal.dataType;
	t2 = val2->tVal.dataType;
	calc_result_rc = true;

#ifdef _PARSER
	if ( (t1 == DATA_TYPE_LONG || t1 == DATA_TYPE_DOUBLE) &&
			(t2 == DATA_TYPE_LONG || t2 == DATA_TYPE_DOUBLE) ) {

	} else {
		calc_result_rc = false;
		return false;
	}
#endif

	if (t1 == DATA_TYPE_DOUBLE || t2 == DATA_TYPE_DOUBLE) {
		if (t1 == DATA_TYPE_LONG)
			val1->tVal.value.dblVal = (double) val1->tVal.value.longVal;

		if (t2 == DATA_TYPE_LONG)
			val2->tVal.value.dblVal = (double) val2->tVal.value.longVal;

		if (val2->tVal.value.dblVal == 0.0) {
			calc_result_rc = false;
			strcpy(calc_result_msg, "Division by zero.");
			return false;
		}

		val1->tVal.value.dblVal /= val2->tVal.value.dblVal;
#ifdef _OS390
		val1->tVal.value.dblVal = FP_Round( val1->tVal.value.dblVal );
#endif
		val1->tVal.dataType = DATA_TYPE_DOUBLE;
	} else {
		if (val2->tVal.value.longVal == 0) {
			calc_result_rc = false;
			strcpy(calc_result_msg, "Division by zero.");
			return false;
		}

		val1->tVal.value.longVal /= val2->tVal.value.longVal;
		val1->tVal.dataType = DATA_TYPE_LONG;
	}

	return (true);

}

unsigned long CALC_MODULUS(OPERAND_STACK *val1, OPERAND_STACK *val2) {
	unsigned char t1, t2;
	t1 = val1->tVal.dataType;
	t2 = val2->tVal.dataType;
	calc_result_rc = true;

#ifdef _PARSER
	if ( (t1 == DATA_TYPE_LONG || t1 == DATA_TYPE_DOUBLE) &&
			(t2 == DATA_TYPE_LONG || t2 == DATA_TYPE_DOUBLE) ) {
		val1->tVal.dataType = DATA_TYPE_LONG;
		val1->tVal.value.longVal = 0;
	} else {
		calc_result_rc = false;
		return false;
	}

#else
	if (t1 == DATA_TYPE_DOUBLE || t2 == DATA_TYPE_DOUBLE) {
		if (t1 == DATA_TYPE_LONG) {
			val1->tVal.dataType = DATA_TYPE_DOUBLE;
			val1->tVal.value.dblVal = (double) val1->tVal.value.longVal;
		}

		if (t2 == DATA_TYPE_LONG)
			val2->tVal.value.dblVal = (double) val2->tVal.value.longVal;

		if (val2->tVal.value.dblVal == 0.0) {
			calc_result_rc = false;
			strcpy(calc_result_msg, "Division by zero.");
			return false;
		}

		val1->tVal.value.dblVal = fmod(val1->tVal.value.dblVal,
				val2->tVal.value.dblVal);

	} else {
		if (val2->tVal.value.longVal == 0) {
			calc_result_rc = false;
			strcpy(calc_result_msg, "Division by zero.");
			return false;
		}

		val1->tVal.value.longVal %= val2->tVal.value.longVal;
	}

#endif

	return (true);

}

unsigned long CALC_POW(OPERAND_STACK *val1, OPERAND_STACK *val2) {
	unsigned char t1, t2;
	t1 = val1->tVal.dataType;
	t2 = val2->tVal.dataType;
	calc_result_rc = true;

#ifdef _PARSER
	if ( (t1 == DATA_TYPE_LONG || t1 == DATA_TYPE_DOUBLE) &&
			(t2 == DATA_TYPE_LONG || t2 == DATA_TYPE_DOUBLE) ) {

	} else {
		calc_result_rc = false;
		return false;
	}
#endif

	if (t1 == DATA_TYPE_LONG)
		val1->tVal.value.dblVal = (double) val1->tVal.value.longVal;

	if (t2 == DATA_TYPE_LONG)
		val2->tVal.value.dblVal = (double) val2->tVal.value.longVal;

	val1->tVal.value.dblVal = pow(val1->tVal.value.dblVal,
			val2->tVal.value.dblVal);
	val1->tVal.dataType = DATA_TYPE_DOUBLE;

	return (true);
}

unsigned long CALC_AND(OPERAND_STACK *val1, OPERAND_STACK *val2) {
	calc_result_rc = true;

#ifdef _PARSER
	if ( val1->tVal.dataType != DATA_TYPE_BOOL && val1->tVal.dataType != DATA_TYPE_LONG ) {
		calc_result_rc = false;
		return false;
	}

	if ( val2->tVal.dataType != DATA_TYPE_BOOL && val2->tVal.dataType != DATA_TYPE_LONG ) {
		calc_result_rc = false;
		return false;
	}

	val1->tVal.value.boolVal = true;

#else

	if (val1->tVal.dataType == DATA_TYPE_LONG) {
		if (val1->tVal.value.longVal == 0)
			val1->tVal.value.boolVal = 0;
		else
			val1->tVal.value.boolVal = 1;
	}

	if (val2->tVal.dataType == DATA_TYPE_LONG) {
		if (val2->tVal.value.longVal == 0)
			val2->tVal.value.boolVal = 0;
		else
			val2->tVal.value.boolVal = 1;
	}

	val1->tVal.value.boolVal = (val1->tVal.value.boolVal
			&& val2->tVal.value.boolVal);
#endif

	val1->tVal.dataType = DATA_TYPE_BOOL;
	return (true);
}

unsigned long CALC_OR(OPERAND_STACK *val1, OPERAND_STACK *val2) {
	calc_result_rc = true;

#ifdef _PARSER
	if ( val1->tVal.dataType != DATA_TYPE_BOOL && val1->tVal.dataType != DATA_TYPE_LONG ) {
		calc_result_rc = false;
		return false;
	}

	if ( val2->tVal.dataType != DATA_TYPE_BOOL && val2->tVal.dataType != DATA_TYPE_LONG ) {
		calc_result_rc = false;
		return false;
	}

	val1->tVal.value.boolVal = true;
#else

	if (val1->tVal.dataType == DATA_TYPE_LONG) {
		if (val1->tVal.value.longVal == 0)
			val1->tVal.value.boolVal = 0;
		else
			val1->tVal.value.boolVal = 1;
	}

	if (val2->tVal.dataType == DATA_TYPE_LONG) {
		if (val2->tVal.value.longVal == 0)
			val2->tVal.value.boolVal = 0;
		else
			val2->tVal.value.boolVal = 1;
	}

	val1->tVal.value.boolVal = (val1->tVal.value.boolVal
			|| val2->tVal.value.boolVal);
#endif

	val1->tVal.dataType = DATA_TYPE_BOOL;

	return (true);
}

