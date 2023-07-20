#ifdef _MSG_KO
#define MSG_DBCONN_ERR    "DataBase(%s) 연결 오류. %d"
#define MSG_DB_CONNECT    "DB 연결 : %s %s ???"
#else
#define MSG_DBCONN_ERR    "DataBase(%s) Connect Error. %d"
#define MSG_DB_CONNECT    "DB Connect : %s %s ???"
#endif

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
    "MRCONORA.pc"
};


static unsigned int sqlctx = 157859;


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
{12,4274,846,0,0,
5,0,0,0,0,0,572,41,0,0,0,0,0,1,0,
20,0,0,0,0,0,570,43,0,0,1,1,0,1,0,3,109,0,0,
39,0,0,1,0,0,541,64,0,0,0,0,0,1,0,
54,0,0,2,0,0,544,80,0,0,0,0,0,1,0,
69,0,0,0,0,0,571,82,0,0,1,1,0,1,0,3,109,0,0,
88,0,0,0,0,0,539,112,0,0,4,4,0,1,0,1,9,0,0,1,9,0,0,1,9,0,0,1,10,0,0,
119,0,0,4,0,0,542,115,0,0,0,0,0,1,0,
134,0,0,0,0,0,539,117,0,0,4,4,0,1,0,1,9,0,0,1,9,0,0,1,9,0,0,1,10,0,0,
165,0,0,6,0,0,542,144,0,0,0,0,0,1,0,
};


#include   <stdio.h>
#include   <stdlib.h>

#ifdef  __hpux
	#include   <strings.h>
#else	
	#include   <string.h>
#endif

#include   <sqlca.h>

//EEXEC SQL INCLUDE SQLCA.H;

#ifndef  SQLCODE
		#define  SQLCODE   sqlca.sqlcode
#endif

#ifndef  SQLMESSAGE
	#define  SQLMESSAGE   ((char*)sqlca.sqlerrm.sqlerrmc)
#endif

/*2013.10.17 ad2001 암호화로 길이를 32->201으로 늘린다.
char               _DBPASSWORD[32]; //  QUALIFIER
*/    
char               _DBNAME[31];     //  SUBSYSTEM
char               _DBUSER[31];     //  PACKAGE
char               _DBPASSWORD[201]; //  QUALIFIER
char               _DBPWD_ENC[3]; //  ENC

/* EXEC SQL CONTEXT USE :ctxConn; */ 


//#include   "MTCSComm.h"

long  AllocThreadCtx( void **ctxParm )
{
/* EXEC SQL    BEGIN DECLARE SECTION; */ 

	sql_context ctxConn;
/* EXEC SQL    END DECLARE SECTION; */ 

	struct sqlca    sqlca;

	/* EXEC SQL ENABLE THREADS; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 0;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )5;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



	/* EXEC SQL CONTEXT ALLOCATE :ctxConn; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 1;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )20;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)&ctxConn;
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


	
	*ctxParm = ctxConn;

	return SQLCODE;
}

long  AttatchThreadCtx( void *ctxParm, char *msg )
{
	return 0;
}
	
int DetachThreadCtx( void *ctxParm )
{
/* EXEC SQL    BEGIN DECLARE SECTION; */ 

	sql_context ctxConn;
/* EXEC SQL    END DECLARE SECTION; */ 

	struct sqlca    sqlca;
	
	ctxConn = ctxParm;
	
	/* EXEC SQL COMMIT; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 1;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )39;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&ctxConn, &sqlctx, &sqlstm, &sqlfpn);
}


	
	return 0;
}	

void FreeCtx( void **ctxParm )
{
/* EXEC SQL    BEGIN DECLARE SECTION; */ 

	 sql_context ctxConn;
/* EXEC SQL    END DECLARE SECTION; */ 

	struct sqlca    sqlca;

	if ( ! *ctxParm ) return;
	
	ctxConn = *ctxParm;

	/* EXEC SQL ROLLBACK WORK RELEASE; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 1;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )54;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&ctxConn, &sqlctx, &sqlstm, &sqlfpn);
}



   /* EXEC SQL CONTEXT FREE :ctxConn; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 1;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )69;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)&ctxConn;
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


}


/*
int ConnectDatabase( sql_context ctxParm,
						   char *DBName, char *DBUser,
                     char *DBPasswd, char *msg )
*/    

int ConnectDatabase( sql_context ctxParm, char *msg )
{
/* EXEC SQL BEGIN DECLARE SECTION; */ 

    /* VARCHAR        uid[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } uid;

    /* VARCHAR        pwd[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } pwd;

    /* VARCHAR        dbName[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } dbName;

    sql_context    ctxConn;
/* EXEC SQL END DECLARE SECTION; */ 
 

    struct sqlca	sqlca;

    strcpy( (char *) uid.arr, _DBUSER );
    uid.len = strlen((char *) uid.arr);
    strcpy( (char *) pwd.arr, _DBPASSWORD );
    pwd.len = strlen((char *) pwd.arr);
    strcpy( (char *) dbName.arr, _DBNAME );
    dbName.len = strlen((char *) dbName.arr);

    ctxConn = ctxParm;	

    /* EXEC SQL CONNECT :uid IDENTIFIED BY :pwd USING :dbName; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 4;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )10;
    sqlstm.offset = (unsigned int  )88;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (         void  *)&uid;
    sqlstm.sqhstl[0] = (unsigned int  )34;
    sqlstm.sqhsts[0] = (         int  )34;
    sqlstm.sqindv[0] = (         void  *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned int  )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqhstv[1] = (         void  *)&pwd;
    sqlstm.sqhstl[1] = (unsigned int  )34;
    sqlstm.sqhsts[1] = (         int  )34;
    sqlstm.sqindv[1] = (         void  *)0;
    sqlstm.sqinds[1] = (         int  )0;
    sqlstm.sqharm[1] = (unsigned int  )0;
    sqlstm.sqadto[1] = (unsigned short )0;
    sqlstm.sqtdso[1] = (unsigned short )0;
    sqlstm.sqhstv[2] = (         void  *)&dbName;
    sqlstm.sqhstl[2] = (unsigned int  )34;
    sqlstm.sqhsts[2] = (         int  )34;
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
    sqlcxt(&ctxConn, &sqlctx, &sqlstm, &sqlfpn);
}



    if( SQLCODE != 0 ) {
        /* EXEC SQL COMMIT WORK RELEASE; */ 

{
        struct sqlexd sqlstm;
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 4;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )119;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlcxt(&ctxConn, &sqlctx, &sqlstm, &sqlfpn);
}


        
        /* EXEC SQL CONNECT :uid IDENTIFIED BY :pwd USING :dbName; */ 

{
        struct sqlexd sqlstm;
        sqlstm.sqlvsn = 12;
        sqlstm.arrsiz = 4;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.iters = (unsigned int  )10;
        sqlstm.offset = (unsigned int  )134;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (         void  *)&uid;
        sqlstm.sqhstl[0] = (unsigned int  )34;
        sqlstm.sqhsts[0] = (         int  )34;
        sqlstm.sqindv[0] = (         void  *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned int  )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (         void  *)&pwd;
        sqlstm.sqhstl[1] = (unsigned int  )34;
        sqlstm.sqhsts[1] = (         int  )34;
        sqlstm.sqindv[1] = (         void  *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned int  )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (         void  *)&dbName;
        sqlstm.sqhstl[2] = (unsigned int  )34;
        sqlstm.sqhsts[2] = (         int  )34;
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
        sqlcxt(&ctxConn, &sqlctx, &sqlstm, &sqlfpn);
}


    }
    
    if( SQLCODE != 0 ) {    /*
		sprintf( msg, "DataBase(%s) Connect Error. %d",
				 _DBNAME, SQLCODE ); */
		sprintf( msg, MSG_DBCONN_ERR,
				 _DBNAME, SQLCODE );
//		printf( "%s : %s\n\n", msg, SQLMESSAGE );		

        return -1;
    }
	
	//sprintf( msg, "DB Connect : %s %s ???", _DBNAME, _DBUSER );
	sprintf( msg, MSG_DB_CONNECT, _DBNAME, _DBUSER );
//	WriteLog( 0, ss );

	return 0;
}


void  DisconnectDatabase( sql_context ctxParm )
{
/* EXEC SQL    BEGIN DECLARE SECTION; */ 

	 sql_context ctxConn;
/* EXEC SQL    END DECLARE SECTION; */ 

	struct sqlca	sqlca;
   
   ctxConn = ctxParm;
   
	/* EXEC SQL COMMIT WORK RELEASE; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 4;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )165;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&ctxConn, &sqlctx, &sqlstm, &sqlfpn);
}



}
