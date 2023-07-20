#include    <stdio.h>
#include    <stdlib.h>
#include    <ctype.h>
#include    <string.h>
#if defined(_AIX)
#include	<strings.h>
#endif
#include    <math.h>
#include    <time.h>

#include    "MTUtil.h"
void CurrentDate(char *dateStr, char *mark) {
	time_t localTime;
	struct tm *stTempTime;
	struct tm pTm2;
	localTime=time(&localTime);
#ifdef WIN32
	stTempTime= localtime(&localTime);
#else
	stTempTime=localtime_r(&localTime, &pTm2);
#endif


//	if ( bUseDash )
	sprintf(dateStr, "%04d%s%02d%s%02d", 1900 + stTempTime->tm_year, mark,stTempTime->tm_mon + 1,
			mark, stTempTime->tm_mday);
//	else
//		sprintf( dateStr, "%04d%02d%02d", 1900+t->tm_year, t->tm_mon + 1, t->tm_mday );
} 
void CurrentDateTime(char *dateStr, char *mark,int thour) {
	time_t localTime;
	struct tm *stTempTime;
	struct tm pTm2;
	localTime=time(&localTime);
#ifdef WIN32
	stTempTime= localtime(&localTime);
#else
	stTempTime=localtime_r(&localTime, &pTm2);
#endif 

// 
	 
	
	if(thour==0){
		sprintf(dateStr, "%04d%s%02d%s%02d", 1900 + stTempTime->tm_year, mark, stTempTime->tm_mon + 1,mark, stTempTime->tm_mday   );
	}else{
		stTempTime->tm_hour -=thour;
		mktime(stTempTime);
		sprintf(dateStr, "%04d%s%02d%s%02d", 1900 + stTempTime->tm_year, mark, stTempTime->tm_mon + 1,mark, stTempTime->tm_mday  );
	}
 
//	printf(" APPLY TIME [%d] [%s]\n",thour,dateStr);
 
}

static long LastDayOfMonth(long lYear, long lMonth) {
	if (lMonth == 4 || lMonth == 6 || lMonth == 9 || lMonth == 11)
		return 30;

	if (lMonth == 2) {
		if (!(lYear % 400))
			return 29;
		if (!(lYear % 100))
			return 28;
		if (!(lYear % 4))
			return 29;

		return 28;
	}

	return 31;
}

short makeDate(char *dateStr, struct tm *date1) {
	char temp[5];
	int m, n;
	bool bUseDash;

	bUseDash = !isdigit(dateStr[4]);

	if (bUseDash) {
		m = 5;
		n = 8;
	} else {
		m = 4;
		n = 6;
	}

	strncpy(temp, dateStr, 4);
	temp[4] = 0;
	date1->tm_year = atoi(temp) - 1900;
	strncpy(temp, dateStr + m, 2);
	temp[2] = 0;
	date1->tm_mon = atoi(temp) - 1;
	strncpy(temp, dateStr + n, 2);
	temp[2] = 0;
	date1->tm_mday = atoi(temp);

	date1->tm_sec = 2;
	date1->tm_min = 2;
	date1->tm_hour = 2;

	return bUseDash;
}

void DatePulsNumber(char *dateStr, long num) {
	struct tm date1, date2;
	struct tm *stTempTime;
	time_t time1;
	short bUseDash;

	bUseDash = makeDate(dateStr, &date1);
	time1 = mktime(&date1);

	time1 = time1 + 86400 * num;
 
#ifdef WIN32
	stTempTime = localtime(&time1);
	memcpy(&date2, stTempTime, sizeof(struct tm));
#else
	stTempTime = localtime_r(&time1, &date2);
#endif	



	if (bUseDash)
		sprintf(dateStr, "%04d-%02d-%02d", 1900 + stTempTime->tm_year,stTempTime->tm_mon + 1, stTempTime->tm_mday);
	else
		sprintf(dateStr, "%04d%02d%02d", 1900 + stTempTime->tm_year,stTempTime->tm_mon + 1, stTempTime->tm_mday);

}

unsigned long DateTypeCheck(const char* lpszDate) {
	char temp[5];
	long yy, mm, dd;
	int m, n;

	if (strlen(lpszDate) < 8)
		return 0;

	if (isdigit(lpszDate[4])) {
		m = 4;
		n = 6;
	} else {
		m = 5;
		n = 8;
	}

	strncpy(temp, lpszDate, 4);
	temp[4] = 0;
	yy = atoi(temp);
	if (yy < 0 || yy > 2999)
		return 0;

	strncpy(temp, lpszDate + m, 2);
	temp[2] = 0;
	mm = atoi(temp);
	strncpy(temp, lpszDate + n, 2);
	temp[2] = 0;
	dd = atoi(temp);

	if (mm < 1 || mm > 12)
		return 0;

	if (dd < 1 || dd > LastDayOfMonth(yy, mm))
		return 0;

	return 1;
}

//  0 : 만 ( 월이 바뀌고 날짜가 크거나 같아야 1개월로 간주함 )
//  1 : 1일이라도 지나면 1개월 간주함.   말일적용.
//  2 : 1일이라도 지나면 1개월 간주함.   말일적용안함.
long months(char *dateStr1, char *dateStr2, short gbn) {
	char yy[5], mm[3], dd[3];
	int month1, month2, dd1, dd2, m;

	if (gbn == 1) {
		DatePulsNumber(dateStr1, 1);
		DatePulsNumber(dateStr2, 1);
	}

	strncpy(yy, dateStr1, 4);
	yy[4] = 0;
	if (isdigit(dateStr1[4])) {
		strncpy(mm, dateStr1 + 4, 2);
		mm[2] = 0;
		strncpy(dd, dateStr1 + 6, 2);
		dd[2] = 0;
	} else {
		strncpy(mm, dateStr1 + 5, 2);
		mm[2] = 0;
		strncpy(dd, dateStr1 + 8, 2);
		dd[2] = 0;
	}

	month1 = atoi(yy) * 12 + atoi(mm);
	dd1 = atoi(dd);

	strncpy(yy, dateStr2, 4);
	yy[4] = 0;
	if (isdigit(dateStr2[4])) {
		strncpy(mm, dateStr2 + 4, 2);
		mm[2] = 0;
		strncpy(dd, dateStr2 + 6, 2);
		dd[2] = 0;
	} else {
		strncpy(mm, dateStr2 + 5, 2);
		mm[2] = 0;
		strncpy(dd, dateStr2 + 8, 2);
		dd[2] = 0;
	}

	month2 = atoi(yy) * 12 + atoi(mm);
	dd2 = atoi(dd);

	if (month1 < month2 || (month1 == month2 && dd1 < dd2)) {
		m = month2 - month1;
		if (gbn == 0) { // 만.    말일적용안함.
			if (dd2 < dd1)
				m--;
		} else if (gbn == 1) { // 올림.  말일적용
			if (dd2 >= dd1)
				m++;
		} else { // 올림.  말일적용안함.
			if (dd2 >= dd1)
				m++;
		}
	} else {
		m = month1 - month2;
		if (gbn == 0) { // 만
			if (dd1 < dd2)
				m--;
		} else if (gbn == 1) { // 올림.  말일적용
			if (dd1 >= dd2)
				m++;
		} else { // 올림.  말일적용안함.
			if (dd1 >= dd2)
				m++;
		}

		m *= -1;
	}

	return m;
}

long DateDuration(char *dStr1, char *dStr2) {
	struct tm date1, date2;
	time_t start, finish;

	makeDate(dStr1, &date1);
	makeDate(dStr2, &date2);

	start = mktime(&date1);
	finish = mktime(&date2);

	return ((long) (difftime(finish, start) / 86400));
}

void DatePulsMonth(char *dateStr, long num, long opt) {
	struct tm date1;
	long yy, mm, dd;
	short bUseDash;
	int lastDD, last = 0;

	if (!DateTypeCheck(dateStr)) {
		dateStr[0] = 0;
		return;
	}

	bUseDash = makeDate(dateStr, &date1);

	yy = date1.tm_year + 1900;
	mm = date1.tm_mon + 1;
	dd = date1.tm_mday;

	if (opt) {
		if (dd == LastDayOfMonth(yy, mm))
			last = 1;
	}

	mm += num;

	while (mm > 12) {
		yy++;
		mm -= 12;
	}

	while (mm < 1) {
		yy--;
		mm += 12;
	}

	lastDD = LastDayOfMonth(yy, mm);

	if (last || dd > lastDD)
		dd = lastDD;

	if (bUseDash)
		sprintf(dateStr, "%04d-%02d-%02d", yy, mm, dd);
	else
		sprintf(dateStr, "%04d%02d%02d", yy, mm, dd);
}

long Age(char* lpszBirthDay, char* lpszDate) {
	struct tm date1, date2;

	if (!DateTypeCheck(lpszBirthDay))
		return -1;

	if (!DateTypeCheck(lpszDate))
		return -1;

	makeDate(lpszBirthDay, &date1);
	makeDate(lpszDate, &date2);

	long ulAge = date2.tm_year - date1.tm_year;

	if (date2.tm_mon < date1.tm_mon)
		ulAge--;
	else if (date2.tm_mon == date1.tm_mon && date2.tm_mday < date1.tm_mday)
		ulAge--;

	if (ulAge < 0)
		return -2;

	return ulAge;
}

long CalcResAgeByDate(char* resNo, char* sDate, short gbn)
//  gbn : 1.보험연령(반올림)   0, 2. 만연령
		{
	struct tm date1, date2;
	time_t time1, time2;
	char lpszBirthDay[11];
	char temp[3];
	int yy;
	int i;
	char ss;

 
  
	if (isdigit(resNo[6]))
		i = 6;
	else
		i = 7;
	ss = resNo[i];

	strncpy(temp, resNo, 2);
	temp[2] = 0;
	yy = atoi(temp);

	if (ss == '1' || ss == '2' || ss == '5' || ss == '6')
		strcpy(lpszBirthDay, "19");
	else if (ss == '3' || ss == '4') {
		if (yy <= 10)
			strcpy(lpszBirthDay, "20");
		else
			strcpy(lpszBirthDay, "19");
	} else if (ss == '7' || ss == '8') {
		if (yy <= 10)
			strcpy(lpszBirthDay, "20");
		else
			strcpy(lpszBirthDay, "19");
	} else
		strcpy(lpszBirthDay, "18");

	strncpy(lpszBirthDay + 2, resNo, 2);
	lpszBirthDay[4] = '-';
	strncpy(lpszBirthDay + 5, resNo + 2, 2);
	lpszBirthDay[7] = '-';
	strncpy(lpszBirthDay + 8, resNo + 4, 2);
	lpszBirthDay[10] = 0;

	makeDate(lpszBirthDay, &date1);
	makeDate(sDate, &date2);

	time1 = mktime(&date1);
	time2 = mktime(&date2);

	if (time1 > time2)
		return 0;

	long ulAge = date2.tm_year - date1.tm_year;

	if (date2.tm_mon < date1.tm_mon)
		ulAge--;
	else if (date2.tm_mon == date1.tm_mon && date2.tm_mday < date1.tm_mday)
		ulAge--;

	if (gbn == 1) { //  반올림연령
		int mm = date2.tm_mon - date1.tm_mon;
		if (date2.tm_mday < date1.tm_mday)
			mm--;

		if (mm < 0)
			mm += 12;

		if (mm >= 6)
			ulAge++;
//		else if( mm == 6 && date2.tm_mday >= date1.tm_mday ) ulAge++;
	}

	return ulAge;
}

long CalcResAge(char* resNo) {
	char lpszCurrDate[11];

	CurrentDate(lpszCurrDate, "-");

	return CalcResAgeByDate(resNo, lpszCurrDate, 0);
}

long CalcCarAge(char* lpszInitDate, char* lpszTaxDate) {
	char initYYYY[5], initMM[3], taxYYYY[5];
	int p;
	long ulAge;

	if (!DateTypeCheck(lpszInitDate))
		return -1;
	if (!DateTypeCheck(lpszTaxDate))
		return -1;

	if (isdigit(lpszInitDate[4]))
		p = 4;
	else
		p = 5;

	strncpy(initYYYY, lpszInitDate, 4);
	initYYYY[4] = 0;
	strncpy(initMM, lpszInitDate + p, 2);
	initMM[2] = 0;
	strncpy(taxYYYY, lpszTaxDate, 4);
	taxYYYY[4] = 0;

	ulAge = atoi(taxYYYY) - atoi(initYYYY);
	if (ulAge < 0)
		return -1;

	if (atoi(initMM) < 7)
		ulAge++;

	return ulAge;
}

double Sqrt(double dbl) {
	if (dbl < 0.0) {
		return 0.0;
	}

	return dbl > 0.0 ? sqrt(dbl) : 0.0;
}

double RRound(double tVal, int off) {
	double dTemp, prec = 1.0;
	double dVal,dVal2;
	int i;
	dVal2= getDoubleToString(tVal);

	if (off > 0) {
		for (i = 0; i < off; i++)
			prec *= 10.0;
	  
		
		if( dVal2 < 0.0 ){
			dTemp = floor(getDoubleToString(dVal2 * prec - 0.5));
		}else{
			dTemp = floor(getDoubleToString(dVal2 * prec + 0.5));
		}
		dVal = dTemp / prec;
	} else {
		for (i = off; i < 0; i++)
			prec *= 10.0;

		if( dVal2 < 0.0 ){
			dTemp = floor(getDoubleToString(dVal2 / prec - 0.5));
		}else{
			dTemp = floor(getDoubleToString(dVal2 / prec + 0.5));
		}
		dVal = dTemp * prec;
	}
	return dVal;
}
/*
double RRound(double dVal, int off) {
	double dTemp, prec = 1.0;
	int i;

	if (off > 0) {
		for (i = 0; i < off; i++)
			prec *= 10.0;

		dTemp = floor(dVal * prec + 0.5);
		dVal = dTemp / prec;
	} else {
		for (i = off; i < 0; i++)
			prec *= 10.0;

		dTemp = floor(dVal / prec + 0.5);
		dVal = dTemp * prec;
	}

	return dVal;
}
*/
double RTrunc(double dVal2, int off) {
	double dTemp, prec = 1.0;
	int i;
	double dVal; 
  dVal= getDoubleToString(dVal2);
	
	if (off > 0) {
		for (i = 0; i < off; i++)
			prec *= 10.0;


		if (dVal > 0) {
			dTemp = dVal * prec;
			dTemp += 0.01;
			dVal = floor(getDoubleToString(dTemp) ) / prec;
		} else {
			dTemp = dVal * prec;
			dVal = ceil(getDoubleToString(dTemp)) / prec;
		}
	} else {
		for (i = off; i < 0; i++)
			prec *= 10.0;

		dTemp = dVal / prec;

		if (dVal > 0)
			dVal = floor(getDoubleToString(dTemp)) * prec;
		else
			dVal = ceil(getDoubleToString(dTemp)) * prec;
	}

	return dVal;
}
/*

double RTrunc(double dVal, int off) {
	double dTemp, prec = 1.0;
	int i;
	if (off > 0) {
		for (i = 0; i < off; i++)
			prec *= 10.0;

		dTemp = dVal * prec;

		if (dVal > 0) {
			dTemp += 0.01;
			dVal = floor(dTemp) / prec;
		} else {
			dVal = ceil(dTemp) / prec;
		}
	} else {
		for (i = off; i < 0; i++)
			prec *= 10.0;

		dTemp = dVal / prec;

		if (dVal > 0)
			dVal = floor(dTemp) * prec;
		else
			dVal = ceil(dTemp) * prec;
	}
	return dVal;
}
*/

long Dbl2Long(double dblVal) {
	char *p, temp[31];
	int ch = '.';
	sprintf(temp, "%lf", dblVal);
	temp[30]=0;
	p = strchr(temp, ch);
	*p = 0;
	return atoi(temp);
}

