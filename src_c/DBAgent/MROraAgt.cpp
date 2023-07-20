
/* Result Sets Interface */
#ifndef SQL_CRSR
#  define SQL_CRSR
  struct sql_cursor
  {
    unsigned int curocn;
    void *ptr1;
    void *ptr2;
    unsigned int magic;
  };
  typedef struct sql_cursor sql_cursor;
  typedef struct sql_cursor SQL_CURSOR;
#endif /* SQL_CRSR */

/* Thread Safety */
typedef void * sql_context;
typedef void * SQL_CONTEXT;

/* Object support */
struct sqltvn
{
  unsigned char *tvnvsn; 
  unsigned short tvnvsnl; 
  unsigned char *tvnnm;
  unsigned short tvnnml; 
  unsigned char *tvnsnm;
  unsigned short tvnsnml;
};
typedef struct sqltvn sqltvn;

struct sqladts
{
  unsigned int adtvsn; 
  unsigned short adtmode; 
  unsigned short adtnum;  
  sqltvn adttvn[1];       
};
typedef struct sqladts sqladts;

static struct sqladts sqladt = {
  1,1,0,
};

/* Binding to PL/SQL Records */
struct sqltdss
{
  unsigned int tdsvsn; 
  unsigned short tdsnum; 
  unsigned char *tdsval[1]; 
};
typedef struct sqltdss sqltdss;
static struct sqltdss sqltds =
{
  1,
  0,
};

/* File name & Package Name */
struct sqlcxp
{
  unsigned short fillen;
           char  filnam[12];
};
static const struct sqlcxp sqlfpn =
{
    11,
    "MROraAgt.pc"
};


static unsigned int sqlctx = 160011;


static struct sqlexd {
   unsigned int   sqlvsn;
   unsigned int   arrsiz;
   unsigned int   iters;
   unsigned int   offset;
   unsigned short selerr;
   unsigned short sqlety;
   unsigned int   occurs;
      const short *cud;
   unsigned char  *sqlest;
      const char  *stmt;
   sqladts *sqladtp;
   sqltdss *sqltdsp;
            void  **sqphsv;
   unsigned int   *sqphsl;
            int   *sqphss;
            void  **sqpind;
            int   *sqpins;
   unsigned int   *sqparm;
   unsigned int   **sqparc;
   unsigned short  *sqpadto;
   unsigned short  *sqptdso;
   unsigned int   sqlcmax;
   unsigned int   sqlcmin;
   unsigned int   sqlcincr;
   unsigned int   sqlctimeout;
   unsigned int   sqlcnowait;
              int   sqfoff;
   unsigned int   sqcmod;
   unsigned int   sqfmod;
            void  *sqhstv[4];
   unsigned int   sqhstl[4];
            int   sqhsts[4];
            void  *sqindv[4];
            int   sqinds[4];
   unsigned int   sqharm[4];
   unsigned int   *sqharc[4];
   unsigned short  sqadto[4];
   unsigned short  sqtdso[4];
} sqlstm = {12,4};

// Prototypes
extern "C" {
  void sqlcxt (void **, unsigned int *,
               struct sqlexd *, const struct sqlcxp *);
  void sqlcx2t(void **, unsigned int *,
               struct sqlexd *, const struct sqlcxp *);
  void sqlbuft(void **, char *);
  void sqlgs2t(void **, char *);
  void sqlorat(void **, unsigned int *, void *);
}

// Forms Interface
static const int IAPSUCC = 0;
static const int IAPFAIL = 1403;
static const int IAPFTL  = 535;
extern "C" { void sqliem(unsigned char *, signed int *); }

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
{12,4282,840,0,0,
5,0,0,0,0,0,27,143,0,0,4,4,0,1,0,1,9,0,0,1,9,0,0,1,9,0,0,1,10,0,0,
36,0,0,2,40,0,1,154,0,0,0,0,0,1,0,
51,0,0,0,0,0,60,174,0,0,0,0,0,1,0,
66,0,0,0,0,0,58,176,0,0,1,1,0,1,0,3,109,0,0,
85,0,0,3,0,0,30,220,0,0,0,0,0,1,0,
100,0,0,4,0,0,17,283,0,0,1,1,0,1,0,1,9,0,0,
119,0,0,4,0,0,17,307,0,0,1,1,0,1,0,1,9,0,0,
138,0,0,4,0,0,11,322,0,0,1,1,0,1,0,1,32,0,0,
157,0,0,4,0,0,45,324,0,0,0,0,0,1,0,
172,0,0,4,0,0,14,366,0,0,1,0,0,1,0,2,32,0,0,
191,0,0,4,0,0,15,444,0,0,0,0,0,1,0,
206,0,0,4,0,0,17,492,0,0,1,1,0,1,0,1,9,0,0,
225,0,0,4,0,0,17,501,0,0,1,1,0,1,0,1,9,0,0,
244,0,0,4,0,0,23,518,0,0,1,1,0,1,0,1,32,0,0,
263,0,0,4,0,0,21,521,0,0,0,0,0,1,0,
278,0,0,5,0,0,29,527,0,0,0,0,0,1,0,
293,0,0,6,0,0,31,529,0,0,0,0,0,1,0,
308,0,0,4,0,0,19,555,0,0,1,1,0,1,0,3,32,0,0,
327,0,0,4,0,0,20,644,0,0,1,1,0,1,0,3,32,0,0,
};


#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include <sqlcpr.h>

#include "MItem.h"
#include "MRERROR.h"
#include "MRSTRUCT.h"
#include "MRDBAgt.h"
#include "BRAgtDef.h"
#include "MTSqlCod.h"

//#define MAX_QUERY_SIZE 1000011

#include   <sqlca.h>
	DATABASE_LIST *pDBInfo;
	 
/*EXEC SQL INCLUDE sqlca.h;*/ 

#ifndef  SQLCODE
	#ifdef _ORACLE
		#define	     SQLMESSAGE	((char*)sqlca.sqlerrm.sqlerrmc)
		#define      SQLCODE    sqlca.sqlcode
		#define      NOTFOUND   1403
	#endif
#endif
 

/* EXEC SQL INCLUDE sqlda.h; 
 */ 
/*
 * $Header: sqlda.h 08-may-2007.05:58:33 ardesai Exp $ sqlda.h 
 */

/***************************************************************
*      The SQLDA descriptor definition                         *
*--------------------------------------------------------------*
*      VAX/3B Version                                          *
*                                                              *
* Copyright (c) 1987, 2007, Oracle. All rights reserved.  *
***************************************************************/


/* NOTES
  **************************************************************
  ***                                                        ***
  *** This file is SOSD.  Porters must change the data types ***
  *** appropriately on their platform.  See notes/pcport.doc ***
  *** for more information.                                  ***
  ***                                                        ***
  **************************************************************
*/

/*  MODIFIED
    ardesai    05/08/07  - Bug[6037057] Undef Y
    apopat     05/08/02  - [2362423] MVS PE to make lines shorter than 79
    apopat     07/31/99 -  [707588] TAB to blanks for OCCS
    lvbcheng   10/27/98 -  change long to int for sqlda
    lvbcheng   08/15/97 -  Move sqlda protos to sqlcpr.h
    lvbcheng   06/25/97 -  Move sqlda protos to this file
    jbasu      01/29/95 -  correct typo
    jbasu      01/27/95 -  correct comment - ub2->sb2
    jbasu      12/12/94 - Bug 217878: note this is an SOSD file
    Morse      12/01/87 - undef L and S for v6 include files
    Richey     07/13/87 - change int defs to long 
    Clare      09/13/84 - Port: Ch types to match SQLLIB structs
    Clare      10/02/86 - Add ifndef SQLDA
*/

#ifndef SQLDA_
#define SQLDA_ 1
 
#ifdef T
# undef T
#endif
#ifdef F
# undef F
#endif

#ifdef S
# undef S
#endif
#ifdef L
# undef L
#endif

#ifdef Y
 # undef Y
#endif
 
struct SQLDA {
  /* ub4    */ int        N; /* Descriptor size in number of entries        */
  /* text** */ char     **V; /* Ptr to Arr of addresses of main variables   */
  /* ub4*   */ int       *L; /* Ptr to Arr of lengths of buffers            */
  /* sb2*   */ short     *T; /* Ptr to Arr of types of buffers              */
  /* sb2**  */ short    **I; /* Ptr to Arr of addresses of indicator vars   */
  /* sb4    */ int        F; /* Number of variables found by DESCRIBE       */
  /* text** */ char     **S; /* Ptr to Arr of variable name pointers        */
  /* ub2*   */ short     *M; /* Ptr to Arr of max lengths of var. names     */
  /* ub2*   */ short     *C; /* Ptr to Arr of current lengths of var. names */
  /* text** */ char     **X; /* Ptr to Arr of ind. var. name pointers       */
  /* ub2*   */ short     *Y; /* Ptr to Arr of max lengths of ind. var. names*/
  /* ub2*   */ short     *Z; /* Ptr to Arr of cur lengths of ind. var. names*/
  };
 
typedef struct SQLDA SQLDA;
 
#endif

/* ----------------- */
/* defines for sqlda */
/* ----------------- */

#define SQLSQLDAAlloc(arg1, arg2, arg3, arg4) sqlaldt(arg1, arg2, arg3, arg4) 

#define SQLSQLDAFree(arg1, arg2) sqlclut(arg1, arg2) 






// #define  _DISP_LOG			1 

int		InitBindDp( DB_AGENT *, int nVal, char *Val[], char *msg );
int		InitSelectDp( DB_AGENT *, char *msg );
void		getValue (SQLDA *select_dp, char *res);

char	*	rltrim( char *str );
char * rtrim(char *str);
int reconnectCnt=0;
/* EXEC SQL CONTEXT USE :ctxAgt; */ 



char* McRTrim(char *lpszSource) {
	int i;
	for (i = strlen(lpszSource) - 1; i > -1; i--) {
		//if( (lpszSource[i] == ' ') || (lpszSource[i] < 0x20) )
		if (lpszSource[i] == ' ')
			lpszSource[i] = 0x00;
		else
			break;
	}
	return lpszSource;
}
char * rtrim(char *str) {
	int i = 0;

	for (i = strlen(str) - 1; i >= 0; i--) {
		if (str[i] == ' ' || str[i] == 0xcc)
			str[i] = 0x00;
		else
			break;
	}
	return str;
}
char* NullCnv(char *lpszSource) {

	return lpszSource;
}
/*
char* NullCnv(char *lpszSource) {
	int i,j;
	for (i = strlen(lpszSource) - 1; i > -1; i--) {
		if (lpszSource[i] == 'L' ||lpszSource[i] == 'l'){
			if( i-3 > -1 ){
				if (   (lpszSource[i-1] == 'L' ||lpszSource[i-1] == 'l' )
				    && (lpszSource[i-2] == 'U' ||lpszSource[i-2] == 'u' )
				    && (lpszSource[i-3] == 'N' ||lpszSource[i-3] == 'n') ){
					lpszSource[i]=' ';
					lpszSource[i-1]=' ';
					lpszSource[i-2]='\'';
					lpszSource[i-3]='\'';
					i=i-3;
				}
			}
		}
	}
	return lpszSource;
}
*/
char *replaceAll(char *s, const char *olds, const char *news) {

  char *result, *sr;
  size_t i, count = 0;
  size_t oldlen = strlen(olds); if (oldlen < 1) return s;
  size_t newlen = strlen(news);


  if (newlen != oldlen) {
    for (i = 0; s[i] != '\0';) {
      if (memcmp(&s[i], olds, oldlen) == 0) count++, i += oldlen;
      else i++;
    }
  } else i = strlen(s);


  result = (char *) malloc(i + 1 + count * (newlen - oldlen));
  if (result == NULL) return NULL;


  sr = result;
  while (*s) {
    if (memcmp(s, olds, oldlen) == 0) {
      memcpy(sr, news, newlen);
      sr += newlen;
      s  += oldlen;
    } else *sr++ = *s++;
  }
  *sr = '\0';

  return result;
}
int  AGT_DB_Connect( void *p, void *ctx, char *msg )
{
/* EXEC SQL BEGIN DECLARE SECTION; */ 

	 sql_context    ctxAgt;

/*
    VARCHAR    dbuser[20];
    VARCHAR    dbpwd[20];
    VARCHAR    dbname[20];
*/    
    /* VARCHAR    dbuser[31]; */ 
struct { unsigned short len; unsigned char arr[31]; } dbuser;
  // 2011-01-25 vapor 20 -> 200
    /* VARCHAR    dbname[31]; */ 
struct { unsigned short len; unsigned char arr[31]; } dbname;
    
    /* VARCHAR    dbpwd[201]; */ 
struct { unsigned short len; unsigned char arr[201]; } dbpwd;


/* EXEC SQL END DECLARE SECTION; */ 
 

	struct sqlca  sqlca;

   ctxAgt = ctx;

	pDBInfo = (DATABASE_LIST*) p;

	strcpy( (char *) dbuser.arr, pDBInfo->DBUser );
	dbuser.len = strlen(pDBInfo->DBUser);
	strcpy( (char *) dbpwd.arr, pDBInfo->DBPasswd );
	dbpwd.len = strlen(pDBInfo->DBPasswd);
	strcpy( (char *) dbname.arr, pDBInfo->DBPName );
	dbname.len = strlen(pDBInfo->DBPName);

	/* EXEC SQL CONNECT :dbuser IDENTIFIED BY :dbpwd USING :dbname; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 4;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )10;
 sqlstm.offset = (unsigned int  )5;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)&dbuser;
 sqlstm.sqhstl[0] = (unsigned int  )33;
 sqlstm.sqhsts[0] = (         int  )33;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&dbpwd;
 sqlstm.sqhstl[1] = (unsigned int  )203;
 sqlstm.sqhsts[1] = (         int  )203;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)&dbname;
 sqlstm.sqhstl[2] = (unsigned int  )33;
 sqlstm.sqhsts[2] = (         int  )33;
 sqlstm.sqindv[2] = (         void  *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlstm.sqlcmax = (unsigned int )100;
 sqlstm.sqlcmin = (unsigned int )2;
 sqlstm.sqlcincr = (unsigned int )1;
 sqlstm.sqlctimeout = (unsigned int )0;
 sqlstm.sqlcnowait = (unsigned int )0;
 sqlcxt(&ctxAgt, &sqlctx, &sqlstm, &sqlfpn);
}



	if ( SQLCODE != 0  && SQLCODE !=-28002) {
		sprintf(msg, "DB(%s) Connect error. (%d)\n", pDBInfo->DBPName, SQLCODE );
		return -1;
	}
	 if( SQLCODE ==-28002 ) { //-28002 패스워드 변경주기 메시지
		printf( "DataBase(%s) Connect Error. ORA-[%d] MSG: [%s]\n",  pDBInfo->DBPName, SQLCODE, SQLMESSAGE );
	}
	 printf("CONNECT TO %s(%s) : %d\n", pDBInfo->DBName, pDBInfo->DBPName , reconnectCnt++);

	/* EXEC SQL alter session set cursor_sharing = FORCE; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 4;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "alter session set cursor_sharing = FORCE";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )36;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&ctxAgt, &sqlctx, &sqlstm, &sqlfpn);
}



	return 0;
 
}

 

int INITAGENT( void *p, void *pAgt, char *msg )
{
//printf("ORA-INITAGENT-001 \n");
/* EXEC SQL BEGIN DECLARE SECTION; */ 

	sql_context    ctxAgt;
/* EXEC SQL END DECLARE SECTION; */ 
 
	struct sqlca sqlca;
	DB_AGENT	*pAgtInfo = (DB_AGENT*) pAgt;
   int rc=0;

   pAgtInfo->m_bConnected = 0;

   /* EXEC SQL ENABLE THREADS; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 4;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )51;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



   /* EXEC SQL CONTEXT ALLOCATE :ctxAgt; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 4;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )66;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)&ctxAgt;
   sqlstm.sqhstl[0] = (unsigned int  )sizeof(void *);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         void  *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned int  )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


   
   pAgtInfo->ctx = ctxAgt;

   pAgtInfo->bind_dp = sqlaldt(ctxAgt, MAX_BIND_VARS, MAX_VNAME_LEN, MAX_INAME_LEN);
	if (pAgtInfo->bind_dp == NULL) {
		sprintf (msg, "out of memory error.   alloc sqlda");
		return -1;
	}    
	
	pAgtInfo->select_dp = sqlaldt(ctxAgt, MAX_SEL_VARS,MAX_VNAME_LEN, MAX_INAME_LEN);
	if (pAgtInfo->select_dp == NULL) {
		sprintf (msg, "out of memory error.   alloc sqlda");
		return -1;
	} 

    
   rc = AGT_DB_Connect( p, ctxAgt, msg );

   if ( rc == 0 )
   {
   		pAgtInfo->m_bConnected = 1;
   		return rc;
   }
   else
   {
  		 pAgtInfo->m_bConnected = 0;
    	return  UERR_DBCONNECT;;
   }	
   
}

int TERMAGENT( void *pAgt, char *msg, char *dummy1 )
{
//printf("ORA-TERMAGENT-001 \n");

/* EXEC SQL BEGIN DECLARE SECTION; */ 

	sql_context    ctxAgt;
/* EXEC SQL END DECLARE SECTION; */ 
 
	struct sqlca sqlca;
	DB_AGENT	*pAgtInfo = (DB_AGENT*) pAgt;

   ctxAgt = pAgtInfo->ctx;

   /* EXEC SQL COMMIT WORK RELEASE; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 4;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )85;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt(&ctxAgt, &sqlctx, &sqlstm, &sqlfpn);
}



   if ( pAgtInfo->bind_dp ) {
   	sqlclut(ctxAgt, (SQLDA*) pAgtInfo->bind_dp);
   	pAgtInfo->bind_dp = 0;
   }
   
   if ( pAgtInfo->select_dp ) {
   	sqlclut(ctxAgt, (SQLDA*) pAgtInfo->select_dp);
   	pAgtInfo->select_dp = 0;
   }

   return 0;
}


int	EXECSQLCUROPEN( void *p, void *pAgt, char *stmt, int nVal, char *Val[], char *msg )
{
// printf("ORA-EXECSQLCUROPEN-001 \n");
/* EXEC SQL BEGIN DECLARE SECTION; */ 

	sql_context    ctxAgt;
	/* VARCHAR			statement[20001]; */ 
struct { unsigned short len; unsigned char arr[20001]; } statement;
 //MItem.h 20000 MAX_QUERY_SIZE
/* EXEC SQL END DECLARE SECTION; */ 
 
	 struct sqlca sqlca;
	 SQLDA  *bind_dp;	
	 int query_size=0;
	 
	 DB_AGENT	*pAgtInfo = (DB_AGENT*) pAgt;	  
	 ctxAgt = pAgtInfo->ctx;
		 
#ifdef _DISP_LOG    
//	printf("\n%s\n\n", stmt );
#endif
	    
	    pAgtInfo->m_fetch_total = 0;
	    pAgtInfo->m_fetch_rows = 0;
	    pAgtInfo->m_idx = 0;
		/*
	    if ( pAgtInfo->m_bConnected == 0 ) {
			if ( AGT_DB_Connect(  p, ctxAgt, msg ) != 0 ) 
			{
				pAgtInfo->m_bConnected = 0;
	         	return UERR_DBCONNECT;
			}
			pAgtInfo->m_bConnected = 1;
		}*/
		McRTrim(stmt);

		NullCnv(stmt);

		query_size=strlen(stmt);
		 
 		printf("stmt [%s]\n",stmt);

		if( query_size >MAX_QUERY_SIZE){
			stmt[MAX_QUERY_SIZE -1]=0;
			strcpy ((char*)statement.arr, stmt );
			statement.len = strlen(stmt);
		}else{
			strcpy ((char*)statement.arr, stmt );
			statement.len = strlen(stmt);
		}

	    /* EXEC SQL PREPARE S FROM :statement; */ 

{
     struct sqlexd sqlstm;
     sqlstm.sqlvsn = 12;
     sqlstm.arrsiz = 4;
     sqlstm.sqladtp = &sqladt;
     sqlstm.sqltdsp = &sqltds;
     sqlstm.stmt = "";
     sqlstm.iters = (unsigned int  )1;
     sqlstm.offset = (unsigned int  )100;
     sqlstm.cud = sqlcud0;
     sqlstm.sqlest = (unsigned char  *)&sqlca;
     sqlstm.sqlety = (unsigned short)4352;
     sqlstm.occurs = (unsigned int  )0;
     sqlstm.sqhstv[0] = (         void  *)&statement;
     sqlstm.sqhstl[0] = (unsigned int  )20003;
     sqlstm.sqhsts[0] = (         int  )0;
     sqlstm.sqindv[0] = (         void  *)0;
     sqlstm.sqinds[0] = (         int  )0;
     sqlstm.sqharm[0] = (unsigned int  )0;
     sqlstm.sqadto[0] = (unsigned short )0;
     sqlstm.sqtdso[0] = (unsigned short )0;
     sqlstm.sqphsv = sqlstm.sqhstv;
     sqlstm.sqphsl = sqlstm.sqhstl;
     sqlstm.sqphss = sqlstm.sqhsts;
     sqlstm.sqpind = sqlstm.sqindv;
     sqlstm.sqpins = sqlstm.sqinds;
     sqlstm.sqparm = sqlstm.sqharm;
     sqlstm.sqparc = sqlstm.sqharc;
     sqlstm.sqpadto = sqlstm.sqadto;
     sqlstm.sqptdso = sqlstm.sqtdso;
     sqlcxt(&ctxAgt, &sqlctx, &sqlstm, &sqlfpn);
}


//	    if ( SQLCODE == -28 || SQLCODE==-1012 ||SQLCODE <= -12500 || SQLCODE==-3135 || SQLCODE==-3114 || SQLCODE ==-3113 ||SQLCODE ==-1092 ) {

	    if ( SQLCODE == -28 || SQLCODE==-1012 ||SQLCODE <= -12500 || SQLCODE==-3135 || SQLCODE==-3114 || SQLCODE ==-3113 ||SQLCODE ==-1092 ||SQLCODE ==-2396 ||SQLCODE ==-2391) {
			if(SQLCODE ==-1012 ||SQLCODE ==-2396 ||SQLCODE ==-2391 ){

			    SQLCODE=0;
				int ac =TERMAGENT( pAgtInfo, msg, msg);
				ac =INITAGENT( p, pAgtInfo, msg );
				ctxAgt = pAgtInfo->ctx; //없으면 오류 발생. -1012

				if( ac !=0 ){
		  			if ( AGT_DB_Connect( p, ctxAgt, msg ) != 0 ) {
	         			pAgtInfo->m_bConnected = 0;
	         			return UERR_DBCONNECT;
	      			}
				}
			}else{ 
		  		if ( AGT_DB_Connect( p, ctxAgt, msg ) != 0 ) {
	         		pAgtInfo->m_bConnected = 0;
	         		return UERR_DBCONNECT;
	      		}
			}
	      	//pAgtInfo->m_bConnected = 1;
	      	/* EXEC SQL PREPARE S FROM :statement; */ 

{
        struct sqlexd sqlstm;
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 4;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )119;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)&statement;
        sqlstm.sqhstl[0] = (unsigned int  )20003;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqphsv = sqlstm.sqhstv;
        sqlstm.sqphsl = sqlstm.sqhstl;
        sqlstm.sqphss = sqlstm.sqhsts;
        sqlstm.sqpind = sqlstm.sqindv;
        sqlstm.sqpins = sqlstm.sqinds;
        sqlstm.sqparm = sqlstm.sqharm;
        sqlstm.sqparc = sqlstm.sqharc;
        sqlstm.sqpadto = sqlstm.sqadto;
        sqlstm.sqptdso = sqlstm.sqtdso;
        sqlcxt(&ctxAgt, &sqlctx, &sqlstm, &sqlfpn);
}


	    }
     
	
	    if ( SQLCODE != 0 ) {
	       sprintf( msg, "SQL Prepare Error. (%d) %s", SQLCODE, SQLMESSAGE );
	
	       return SQLCODE;
	   }
	    /* EXEC SQL DECLARE CSR CURSOR FOR S; */ 
   
	    
	    if ( nVal > 0 ) {   /*  Describe Bind Variables.  */
	       if ( InitBindDp( pAgtInfo, nVal, Val, msg ) != 0 ) return -1;
	       bind_dp = (SQLDA*) pAgtInfo->bind_dp;
	       
	       /* EXEC SQL OPEN CSR USING DESCRIPTOR bind_dp; */ 

{
        struct sqlexd sqlstm;
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 4;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )138;
        sqlstm.selerr = (unsigned short)1;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqcmod = (unsigned int )0;
        sqlstm.sqhstv[0] = (         void  *)bind_dp;
        sqlstm.sqhstl[0] = (unsigned int  )0;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqphsv = sqlstm.sqhstv;
        sqlstm.sqphsl = sqlstm.sqhstl;
        sqlstm.sqphss = sqlstm.sqhsts;
        sqlstm.sqpind = sqlstm.sqindv;
        sqlstm.sqpins = sqlstm.sqinds;
        sqlstm.sqparm = sqlstm.sqharm;
        sqlstm.sqparc = sqlstm.sqharc;
        sqlstm.sqpadto = sqlstm.sqadto;
        sqlstm.sqptdso = sqlstm.sqtdso;
        sqlcxt(&ctxAgt, &sqlctx, &sqlstm, &sqlfpn);
}


	    } else {
	       /* EXEC SQL OPEN CSR; */ 

{
        struct sqlexd sqlstm;
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 4;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )157;
        sqlstm.selerr = (unsigned short)1;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqcmod = (unsigned int )0;
        sqlcxt(&ctxAgt, &sqlctx, &sqlstm, &sqlfpn);
}


	    }
	
	    if ( SQLCODE != 0 ) {
	       sprintf( msg, "Cursor open error(%d)  %s.", SQLCODE, SQLMESSAGE );      
	       return SQLCODE;
	    }  
	
	    if ( InitSelectDp( pAgtInfo, msg ) != 0 ) return -1;     
	        	 	 
	    return 0;
    
}


int EXECSQLFETCH( void *pAgt, char *res, char *msg )
{
printf("ORA-EXECSQLFETCH-001-001 \n");
/* EXEC SQL BEGIN DECLARE SECTION; */ 

	sql_context    ctxAgt;
	int				array_size;
/* EXEC SQL END DECLARE SECTION; */ 
 

	struct sqlca sqlca;
	DB_AGENT	*pAgtInfo = (DB_AGENT*) pAgt;
	SQLDA		*sdp = (SQLDA*) pAgtInfo->select_dp;
	int		 idx = pAgtInfo->m_idx;
   short    *iptr;
   char     *cptr;
   int		 sz;
   char		 strVal[20001];//MAX_QUERY_SIZE
   char		*p = res;
   
#ifdef _DISP_LOG     
 //printf("Idx = %d\n", idx );
#endif

	if ( idx == 0 ) {
		ctxAgt = pAgtInfo->ctx;
		array_size = MAX_FETCH_ROWS;
//printf("ORA-EXECSQLFETCH-001-MAX_FETCH_ROWS [%d] \n",MAX_FETCH_ROWS);

      /* EXEC SQL FOR :array_size FETCH CSR USING DESCRIPTOR sdp; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 12;
      sqlstm.arrsiz = 4;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )array_size;
      sqlstm.offset = (unsigned int  )172;
      sqlstm.selerr = (unsigned short)1;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)4352;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqfoff = (           int )0;
      sqlstm.sqfmod = (unsigned int )2;
      sqlstm.sqhstv[0] = (         void  *)sdp;
      sqlstm.sqhstl[0] = (unsigned int  )0;
      sqlstm.sqhsts[0] = (         int  )0;
      sqlstm.sqindv[0] = (         void  *)0;
      sqlstm.sqinds[0] = (         int  )0;
      sqlstm.sqharm[0] = (unsigned int  )0;
      sqlstm.sqadto[0] = (unsigned short )0;
      sqlstm.sqtdso[0] = (unsigned short )0;
      sqlstm.sqphsv = sqlstm.sqhstv;
      sqlstm.sqphsl = sqlstm.sqhstl;
      sqlstm.sqphss = sqlstm.sqhsts;
      sqlstm.sqpind = sqlstm.sqindv;
      sqlstm.sqpins = sqlstm.sqinds;
      sqlstm.sqparm = sqlstm.sqharm;
      sqlstm.sqparc = sqlstm.sqharc;
      sqlstm.sqpadto = sqlstm.sqadto;
      sqlstm.sqptdso = sqlstm.sqtdso;
      sqlcxt(&ctxAgt, &sqlctx, &sqlstm, &sqlfpn);
}


      
		if ( SQLCODE != 0 && SQLCODE != NOTFOUND ) {
			sprintf( msg, "SQL Cursor Fetch Error.  %d",	SQLCODE );
			return UERR_DBERROR;
		}

		pAgtInfo->m_fetch_rows = sqlca.sqlerrd[2] - pAgtInfo->m_fetch_total;
		pAgtInfo->m_fetch_total = sqlca.sqlerrd[2];

#ifdef _DISP_LOG 
//printf("   fetch rows = %d\n\n", pAgtInfo->m_fetch_rows );
#endif

		if ( pAgtInfo->m_fetch_rows <= 0 ) return 1;
	}

	if ( idx == pAgtInfo->m_fetch_rows ) return 1;
	
	
   for (int i=0; i<sdp->F; i++)
   {
      sz = (int)sdp->L[i];

		iptr = sdp->I[i] + idx;
		cptr = sdp->V[i] + idx * sz;

   	if ( i > 0 ) {
   		strcpy( p,"|^" );
   		p += 2;
   	}
   			
      if (*iptr < 0) {
          strVal[0] = 0;
      } else {
//printf("    sz[%d][%s]\n", sz, cptr );      
          sprintf(strVal, "%-*.*s",  sz, sz, cptr );
          strcpy( p, rtrim(strVal) );
          p += strlen( p );
      }		
   }

	*p = 0;
	
	if ( ++idx == MAX_FETCH_ROWS ) idx = 0;

	pAgtInfo->m_idx = idx;

   return 0;
}
 

int EXECSQLCURCLOSE( void *pAgt, char *dummy1, char *dummy2 )
{
//printf("ORA-EXECSQLCURCLOSE-001 \n");
/* EXEC SQL BEGIN DECLARE SECTION; */ 

	sql_context    ctxAgt;
/* EXEC SQL END DECLARE SECTION; */ 
 

	struct sqlca sqlca;
	DB_AGENT	*pAgtInfo = (DB_AGENT*) pAgt;

	ctxAgt = pAgtInfo->ctx;

	SQLDA *sdp = (SQLDA*) pAgtInfo->select_dp;
	
	for (int i=0; i<sdp->F; i++)
	{
        free (sdp->V[i]);
		free (sdp->I[i]);
	}
 /*
	for (int i=0; i<bind_dp->F; i++)
	{
        if( bind_dp->V[i]!=NULL)free (bind_dp->V[i]);
		free (bind_dp->I[i]);
	}*/
 
	/* EXEC SQL CLOSE CSR; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 4;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )191;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&ctxAgt, &sqlctx, &sqlstm, &sqlfpn);
}



	return 0;
}


int EXECSQLCUD( void *p, void *pAgt, char *stmt, int nVal, char *Val[], char *msg )
{
printf("ORA-EXECSQLCUD-001 \n");
/* EXEC SQL BEGIN DECLARE SECTION; */ 

	sql_context    ctxAgt;
	/* VARCHAR			statement[20001]; */ 
struct { unsigned short len; unsigned char arr[20001]; } statement;
 //MItem.h 20000 MAX_QUERY_SIZE
	
/* EXEC SQL END DECLARE SECTION; */ 
 

	struct sqlca sqlca;
	DB_AGENT	*pAgtInfo = (DB_AGENT*) pAgt;
	SQLDA  *bind_dp;
	int     rc;
	int 	query_size;	 
#ifdef _DEBUG
	printf ( "ExecSqlCUD(%s) => %s\n", pDBInfo->DBPName, stmt );
#endif


   ctxAgt = pAgtInfo->ctx;

	if ( ! pAgtInfo->m_bConnected ) {
		if ( AGT_DB_Connect( p, ctxAgt, msg ) != 0 )
		{
			pAgtInfo->m_bConnected = 0;
			return UERR_DBCONNECT;
		}
		
		pAgtInfo->m_bConnected = 1;
	}

		McRTrim(stmt);
		query_size=strlen(stmt);
		if( query_size >MAX_QUERY_SIZE){
			stmt[MAX_QUERY_SIZE -1]=0;
			strcpy ((char*)statement.arr, stmt );
			statement.len = strlen(stmt);
		}else{
			strcpy ((char*)statement.arr, stmt );
			statement.len = strlen(stmt);
		}	

   /* EXEC SQL PREPARE S FROM :statement; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 4;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )206;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)&statement;
   sqlstm.sqhstl[0] = (unsigned int  )20003;
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         void  *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned int  )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqphsv = sqlstm.sqhstv;
   sqlstm.sqphsl = sqlstm.sqhstl;
   sqlstm.sqphss = sqlstm.sqhsts;
   sqlstm.sqpind = sqlstm.sqindv;
   sqlstm.sqpins = sqlstm.sqinds;
   sqlstm.sqparm = sqlstm.sqharm;
   sqlstm.sqparc = sqlstm.sqharc;
   sqlstm.sqpadto = sqlstm.sqadto;
   sqlstm.sqptdso = sqlstm.sqtdso;
   sqlcxt(&ctxAgt, &sqlctx, &sqlstm, &sqlfpn);
}


	if ( SQLCODE != 0 ) {
//   	if ( SQLCODE == -28 || SQLCODE==-1012 ||SQLCODE <= -12500 || SQLCODE==-3135 || SQLCODE==-3114 || SQLCODE ==-3113 ||SQLCODE ==-1092  ) {
   	if ( SQLCODE == -28 || SQLCODE==-1012 ||SQLCODE <= -12500 || SQLCODE==-3135 || SQLCODE==-3114 || SQLCODE ==-3113 ||SQLCODE ==-1092  ||SQLCODE ==-2396 ||SQLCODE ==-2391) {
      	if ( AGT_DB_Connect( p, ctxAgt, msg ) != 0 ) {
         	pAgtInfo->m_bConnected = 0;
         	return UERR_DBCONNECT;
      	}
      	pAgtInfo->m_bConnected = 1;
      	/* EXEC SQL PREPARE S FROM :statement; */ 

{
       struct sqlexd sqlstm;
       sqlstm.sqlvsn = 12;
       sqlstm.arrsiz = 4;
       sqlstm.sqladtp = &sqladt;
       sqlstm.sqltdsp = &sqltds;
       sqlstm.stmt = "";
       sqlstm.iters = (unsigned int  )1;
       sqlstm.offset = (unsigned int  )225;
       sqlstm.cud = sqlcud0;
       sqlstm.sqlest = (unsigned char  *)&sqlca;
       sqlstm.sqlety = (unsigned short)4352;
       sqlstm.occurs = (unsigned int  )0;
       sqlstm.sqhstv[0] = (         void  *)&statement;
       sqlstm.sqhstl[0] = (unsigned int  )20003;
       sqlstm.sqhsts[0] = (         int  )0;
       sqlstm.sqindv[0] = (         void  *)0;
       sqlstm.sqinds[0] = (         int  )0;
       sqlstm.sqharm[0] = (unsigned int  )0;
       sqlstm.sqadto[0] = (unsigned short )0;
       sqlstm.sqtdso[0] = (unsigned short )0;
       sqlstm.sqphsv = sqlstm.sqhstv;
       sqlstm.sqphsl = sqlstm.sqhstl;
       sqlstm.sqphss = sqlstm.sqhsts;
       sqlstm.sqpind = sqlstm.sqindv;
       sqlstm.sqpins = sqlstm.sqinds;
       sqlstm.sqparm = sqlstm.sqharm;
       sqlstm.sqparc = sqlstm.sqharc;
       sqlstm.sqpadto = sqlstm.sqadto;
       sqlstm.sqptdso = sqlstm.sqtdso;
       sqlcxt(&ctxAgt, &sqlctx, &sqlstm, &sqlfpn);
}


      
      }
	}
      
   if ( SQLCODE != 0 ) {
       sprintf( msg, "SQL Prepare Error. (%d) %s", SQLCODE, SQLMESSAGE );

       return SQLCODE;
   }


	if ( nVal > 0 ) {   /*  Describe Bind Variables.  */
		if ( InitBindDp( pAgtInfo, nVal, Val, msg ) != 0 ) return -1;
       
		bind_dp = (SQLDA*) pAgtInfo->bind_dp;
       
		/* EXEC SQL EXECUTE S USING DESCRIPTOR bind_dp; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 4;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )244;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (         void  *)bind_dp;
  sqlstm.sqhstl[0] = (unsigned int  )0;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         void  *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&ctxAgt, &sqlctx, &sqlstm, &sqlfpn);
}


             
	} else {
		/* EXEC SQL EXECUTE S; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 4;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )263;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)4352;
  sqlstm.occurs = (unsigned int  )0;
  sqlcxt(&ctxAgt, &sqlctx, &sqlstm, &sqlfpn);
}


	}
	
	rc = SQLCODE;

	if ( SQLCODE == 0 )
	    /* EXEC SQL COMMIT; */ 

{
     struct sqlexd sqlstm;
     sqlstm.sqlvsn = 12;
     sqlstm.arrsiz = 4;
     sqlstm.sqladtp = &sqladt;
     sqlstm.sqltdsp = &sqltds;
     sqlstm.iters = (unsigned int  )1;
     sqlstm.offset = (unsigned int  )278;
     sqlstm.cud = sqlcud0;
     sqlstm.sqlest = (unsigned char  *)&sqlca;
     sqlstm.sqlety = (unsigned short)4352;
     sqlstm.occurs = (unsigned int  )0;
     sqlcxt(&ctxAgt, &sqlctx, &sqlstm, &sqlfpn);
}


	else
	    /* EXEC SQL ROLLBACK; */ 

{
     struct sqlexd sqlstm;
     sqlstm.sqlvsn = 12;
     sqlstm.arrsiz = 4;
     sqlstm.sqladtp = &sqladt;
     sqlstm.sqltdsp = &sqltds;
     sqlstm.iters = (unsigned int  )1;
     sqlstm.offset = (unsigned int  )293;
     sqlstm.cud = sqlcud0;
     sqlstm.sqlest = (unsigned char  *)&sqlca;
     sqlstm.sqlety = (unsigned short)4352;
     sqlstm.occurs = (unsigned int  )0;
     sqlcxt(&ctxAgt, &sqlctx, &sqlstm, &sqlfpn);
}


	    
	    	
	if ( rc != 0 ) {
	   sprintf( msg, "SQL Error. (RC=%d) SQLCODE ORA[%d]", rc ,SQLCODE);
	}
	return rc;
}


int  InitBindDp( DB_AGENT *pAgtInfo, int nVal, char *Val[], char *msg )
{
//printf("ORA-InitBindDp-001 \n");
/* EXEC SQL BEGIN DECLARE SECTION; */ 

	sql_context    ctxAgt;
/* EXEC SQL END DECLARE SECTION; */ 
 

	 struct sqlca sqlca;
	 SQLDA  *bind_dp = (SQLDA*) pAgtInfo->bind_dp;
   char *token;
   char *tempData;
   char  descData[200];
	 ctxAgt = pAgtInfo->ctx;

    bind_dp->N = nVal; 
        
    /* EXEC SQL DESCRIBE BIND VARIABLES FOR S INTO bind_dp; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 4;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )308;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (         void  *)bind_dp;
    sqlstm.sqhstl[0] = (unsigned int  )0;
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         void  *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned int  )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt(&ctxAgt, &sqlctx, &sqlstm, &sqlfpn);
}


    
    if (bind_dp->F < 0)
    {
       sprintf( msg, "Too many bind variables for descriptor.");
       return -1;
    }
  
        
    bind_dp->N = bind_dp->F;
/* ARRAY 아닐경우 되는 경우.	 
#ifdef _DISP_LOG 
//    printf( "bind variables (%d).\n", bind_dp->F);        
#endif    
    for (int i=0; i<bind_dp->F; i++)
    {
#ifdef _DISP_LOG     
//       printf( "   %d : C[%d]   S[%s]\n", i, (int)bind_dp->C[i], bind_dp->S[i] );
#endif

       bind_dp->I[i] = 0;
       bind_dp->T[i] = 1;    //  CHAR : 1    NUM : 2   LONG : 8   ...
       
       bind_dp->L[i] = strlen(Val[i]);
       bind_dp->V[i] = Val[i];
		}
*/		
#ifdef _DISP_LOG 
//    printf( "bind variables2 (%d).\n", bind_dp->F);        
#endif    
    for (int i=0; i<bind_dp->F; i++)
    {
#ifdef _DISP_LOG     
//       printf( "   %d : C[%d]   S[%s]\n", i, (int)bind_dp->C[i], bind_dp->S[i] );
#endif

       bind_dp->I[i] = 0;
       bind_dp->T[i] = 1;    //  CHAR : 1    NUM : 2   LONG : 8   ...
       bind_dp->L[i] = strlen(Val[i]);
       bind_dp->V[i] = Val[i];
//	   printf(" val [%d] ==> [%s]\n", i, Val[i]);
/*
       if( strlen(Val[i]) > 0 ){
				 	tempData=(char*)malloc( strlen(Val[i])+1 );
				 	strcpy(tempData,Val[i]);
				 	token=strtok(tempData,",");
				 	sprintf(descData,"%s",token);
			   	if(tempData!=NULL)free(tempData);
       		bind_dp->L[i] = strlen(descData);
       		bind_dp->V[i]=  (char*) realloc(bind_dp->V[i],(bind_dp->L[i]+1));
       		printf("MROraAgt [%s][%d]\n",descData, strlen(descData));
       		strncpy(bind_dp->V[i],descData,strlen(descData));
       		
       }else{
		bind_dp->L[i] = strlen(Val[i]);
       		bind_dp->V[i] = Val[i];
       }*/
    }
    
#ifdef _DISP_LOG 
//	 printf("\n" );
#endif


	 if ( bind_dp->F != nVal )
	 {
       sprintf( msg, "bind variables unmatch.");
       return -1;
    }
        
    return 0;
}


int InitSelectDp( DB_AGENT *pAgtInfo, char *msg )
{
//printf("ORA-InitSelectDp-001 \n");
/* EXEC SQL BEGIN DECLARE SECTION; */ 

	sql_context    ctxAgt;
/* EXEC SQL END DECLARE SECTION; */ 
 

	 struct sqlca sqlca;
	 int	precision, scale;
	 int	null_ok;
	 
	 SQLDA  *select_dp = (SQLDA*) pAgtInfo->select_dp;

	 ctxAgt = pAgtInfo->ctx;

    select_dp->N = MAX_SEL_VARS; 
    /* EXEC SQL DESCRIBE SELECT LIST FOR S INTO select_dp; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 4;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )327;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (         void  *)select_dp;
    sqlstm.sqhstl[0] = (unsigned int  )0;
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         void  *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned int  )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlcxt(&ctxAgt, &sqlctx, &sqlstm, &sqlfpn);
}


         if (select_dp->F < 0)
    {
       sprintf (msg, "Too many select list variables for descriptor: %d",
         -(select_dp->F));
       return -1;
    }
    
    select_dp->N = select_dp->F;
 /*4.0 20181224수정 ad2001 (원래 없음 .. 추가하였으나 문제발생하여 제거(2019-01-01)
    for(int i=0; i < MAX_SEL_VARS; i++){
            if( select_dp->I[i]!=NULL){
                    free(select_dp->I[i]);
                    select_dp->I[i]=(short*) malloc(sizeof(short));
            }
            if( select_dp->V[i]!=NULL){
                   free(select_dp->V[i]);
                   select_dp->V[i]=(char *) malloc(1);
            }
     }
    */
#ifdef _DISP_LOG         
//    printf( "select variables.\n");
#endif  
    
    for ( int i=0; i<select_dp->F; i++)
    {
        sqlnult(ctxAgt, (unsigned short*) &(select_dp->T[i]), (unsigned short*) &(select_dp->T[i]), &null_ok);

#ifdef _DISP_LOG 
//	 printf("   Type(%d) %d  %d\n", i, (short) select_dp->T[i], select_dp->L[i] );
#endif
        
        switch ( select_dp->T[i] )
        {
            case 1 :  /* CHAR 데이타 타입 : 변경 필요없음. */
            break;
            
            case 2 : /* NUMBER 데이타 타입 : "sqlprc"를 이용 precision과 scale을 setting */
               sqlprct( ctxAgt, (unsigned int*) &(select_dp->L[i]), &precision, &scale);

#ifdef _DISP_LOG                
//printf("      precision = %d   scale = %d\n", precision, scale );
#endif
               if (precision == 0) precision = 40;   /* Table컬럼 단독이 아닐때 =>  NUMBER의 최대 크기 */

               select_dp->L[i] = 40;
//               select_dp->L[i] = precision + 2;
//               if (scale < 0) select_dp->L[i] += -scale;
               
            break;
            
            case 8 :  /* LONG 데이타 타입 */
               select_dp->L[i] = 240;
            break;

            case 11 : /* ROWID datatype */
               select_dp->L[i] = 18;
            break;
                        
            case 12 :/* DATE 데이타 타입 */
               select_dp->L[i] = 9;
            break;
            
            case 23 :/* RAW 데이타 타입 */
            break;
            
            case 24 :  /* LONG RAW 데이타 타입 */
               select_dp->L[i] = 240;
            break;
        }
        
        /* select-list  item에 space 할당 */
		
		/*산업은행에서 Query 문장에 LTRIM(....) 긴 문장 존재시 32767 로 길이가 설정되면서 메모리 오류 발생. 하여 룰에서는 1000까지만 반환되게 변경함.2018.10.11*/
		if( select_dp->L[i]>QUERY_COL_RESULT_SIZE){
			select_dp->L[i]=QUERY_COL_RESULT_SIZE;
		} 

//        select_dp->V[i] = (char *)calloc(MAX_FETCH_ROWS, select_dp->L[i]+1);

        if ((select_dp->V[i] = (char *)calloc(MAX_FETCH_ROWS, select_dp->L[i]+1)) == NULL)
        {
           sprintf( msg, "** error in calloc() for select-list");
           return -1;
        }     
        
        /*indicator변수를 위한 space 할당 */
        
        if ((select_dp->I[i] = (short *)calloc(MAX_FETCH_ROWS,sizeof(short))) == NULL)
        {
           sprintf( msg, "** error in calloc() for indicator variables");
           return -1;
        }

               
        /* LONG RAW를 제외하고 모든 데이타 타입이 character로 coerce */            
        if (select_dp->T[i] != 24) select_dp->T[i] = 1;
    
    }

#ifdef _DISP_LOG 
//	 printf("\n" );
#endif

    return 0;	 
}

	     
char * rltrim( char *str )
{
    for( int i = strlen(str) - 1; i >= 0; i-- )
    {
        if ( str[i] == ' ' || str[i] == 0xcc)
            str[i] = 0x00;
        else
            break;
    }
    
    char *p = str;

	 while ( *p ) {
	     if ( *p != ' ' ) break;
	     p++;
	 }
	 
	 if ( *p == 0 ) strcpy( p, " " );
    return p;    
}


/*
#include    <time.h>

#if defined(_OS390)
	#include	<sys/time.h>
#endif
#if defined(WIN32)
	#include	<sys/timeb.h>
#endif

double   getMiliSecond()
{
	double   usec;

#ifdef WIN32
   	struct _timeb timebuffer;
	_ftime( &timebuffer );

	usec = timebuffer.time * 1000.0 + timebuffer.millitm;

#else
	struct timeval stamp1; 

	gettimeofday(&stamp1, NULL) ;

	usec = stamp1.tv_sec * 1000 + stamp1.tv_usec / 1000;

#endif
 
	return usec;
}
*/
 
