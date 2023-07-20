#ifndef     _INC_MRFUNCTN
#define     _INC_MRFUNCTN

#if defined(_AIX)
	#define	bool unsigned char
	#define	true 1
	#define false 0
#endif

//void             CurrentDate( short, char *dateStr );
void             CurrentDateTime( char *dateStr, char *, int time );
void             CurrentDate( char *dateStr, char * );
static long      LastDayOfMonth( long lYear, long lMonth );
short            makeDate( char *dateStr, struct tm  *date1 );
long			 months( char *dateStr1, char *dateStr2, short gbn );
long             DateDuration( char *dStr1, char *dStr2 );
void             DatePulsNumber( char *dateStr, long num );
void			 DatePulsMonth( char *dateStr, long num, long opt );

unsigned long    DateTypeCheck( const char* lpszDate );
long             Age( char* lpszBirthDay, char* lpszDate );
long			 CalcResAgeByDate( char* resNo, char* sDate, short gbn );
long			 CalcResAge( char* resNo );
long			 CalcCarAge( char* lpszInitDate, char* lpszTaxDate );

double           Sqrt( double dbl );
double			 RRound( double dVal, int off );
double			 RTrunc( double dVal, int off );

long			 Dbl2Long( double dblVal );
#endif
