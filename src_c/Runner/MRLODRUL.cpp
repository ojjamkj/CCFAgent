
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
           char  filnam[38];
};
static const struct sqlcxp sqlfpn =
{
    37,
    "/home/bre/4.5//Src/Runner/MRLODRUL.pc"
};




static struct sqlexd {
   unsigned long  sqlvsn;
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
   unsigned char  **sqphsv;
   unsigned long  *sqphsl;
            int   *sqphss;
            short **sqpind;
            int   *sqpins;
   unsigned long  *sqparm;
   unsigned long  **sqparc;
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
   unsigned char  *sqhstv[12];
   unsigned long  sqhstl[12];
            int   sqhsts[12];
            short *sqindv[12];
            int   sqinds[12];
   unsigned long  sqharm[12];
   unsigned long  *sqharc[12];
   unsigned short  sqadto[12];
   unsigned short  sqtdso[12];
} sqlstm = {12,12};

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

 static const char *sq0003 = 
"select RS40_1 ,RS40_2 ,RS40_3 ,RS40_4  from BRS400TB  order by RS40_1 asc  \
           ";

 static const char *sq0004 = 
"select A.RS41_2 ,A.RS41_3 ,A.RS41_4 ,A.RS41_5 ,A.RS41_6 ,A.RS41_7 ,A.RS41_8\
 ,NVL(B.RS11_2,' ')  from BRS410TB A ,BRS110TB B where (A.RS40_1=:b0 and B.RS\
11_1(+)=A.RS41_2) order by A.RS41_1 asc             ";

 static const char *sq0005 = 
"select A.RS41_2 ,A.RS41_3 ,NVL(B.RS11_2,' ')  from BRS410TB A ,BRS110TB B w\
here (A.RS40_1=:b0 and B.RS11_1(+)=A.RS41_2) order by A.RS41_1 asc           \
  ";

 static const char *sq0007 = 
"select A.RS43_1 ,A.RS43_2 ,A.RS43_3 ,A.RS43_4 ,A.RS43_5 ,A.RS43_6 ,A.RS43_7\
 ,A.RS43_8 ,A.RS43_9 ,A.RS43_A ,A.RS43_B ,NVL(B.RS32_2,' ') RS32_2  from BRS4\
30TB A ,BRS320TB B where (B.RS31_1(+)=A.RS43_1 and B.RS32_1(+)=A.RS43_C) orde\
r by A.RS43_1 asc             ";

 static const char *sq0009 = 
"select RS43_1 ,RS43_3  from BRS430TB where RS43_3>' ' order by RS43_3 asc  \
           ";

 static const char *sq0013 = 
"select RS47_2  from BRS470TB where RS43_1=:b0 order by RS47_1 asc          \
   ";

 static const char *sq0014 = 
"select RS49_2  from BRS490TB where RS43_1=:b0 order by RS49_1 asc          \
   ";

 static const char *sq0015 = 
"select RS44_1 ,RS44_2 ,RS44_3 ,RS44_4  from BRS440TB where RS43_1=:b0 order\
 by RS44_2 desc             ";

 static const char *sq0016 = 
"select RS45_1 ,RS45_2  from BRS450TB where (RS43_1=:b0 and RS44_1=:b1) orde\
r by RS45_1 asc             ";

 static const char *sq0018 = 
"select RS4E_2  from BRS4E0TB where (RS43_1=:b0 and RS44_1=:b1) order by RS4\
E_1 asc             ";

 static const char *sq0021 = 
"select RS4B_2 ,RS4B_3 ,RS4B_4 ,RS4B_5 ,RS4B_6 ,RS4B_7 ,NVL(RS4B_C,' ')  fro\
m BRS4B0TB where (RS43_1=:b0 and RS44_1=:b1) order by RS4B_1 asc             ";

 static const char *sq0024 = 
"select RS45_1 ,RS45_2  from BRS450TB where (RS43_1=:b0 and RS44_1=:b1) orde\
r by RS45_1 asc             ";

 static const char *sq0025 = 
"select RS45_1 ,RS4A_1 ,RS4A_2 ,RS4A_3  from BRS4A0TB where (RS43_1=:b0 and \
RS44_1=:b1) order by RS45_1,RS4A_1 asc             ";

 static const char *sq0026 = 
"select RS48_1 ,RS48_2 ,RS48_3  from BRS480TB where (RS43_1=:b0 and RS44_1=:\
b1) order by RS48_1 asc             ";

 static const char *sq0028 = 
"select RS4D_2 ,RS4D_3  from BRS4D0TB where ((RS43_1=:b0 and RS44_1=:b1) and\
 RS48_1=:b2) order by RS4D_1 asc             ";

 static const char *sq0029 = 
"select RS12_1 ,RS12_2 ,RS12_3 ,RS12_4 ,RS12_5 ,RS12_6  from BRS120TB       \
     ";

 static const char *sq0034 = 
"select RS16_1 ,RS16_3 ,RS16_5  from BRS160TB where (RS16_1>=10000 and RS16_\
1<=19999) order by RS16_1            ";

 static const char *sq0042 = 
"select RS11_1 ,RS1D_1  from BRS1D0TB where (RS1C_1=:b0 and RS21_1=:b1)     \
      ";

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
{12,4282,846,0,0,
5,0,0,1,41,0,4,156,0,0,1,0,0,1,0,2,3,0,0,
24,0,0,2,0,0,31,253,0,0,0,0,0,1,0,
39,0,0,3,86,0,9,372,0,0,0,0,0,1,0,
54,0,0,3,0,0,13,378,0,0,4,0,0,1,0,2,97,0,0,2,3,0,0,2,3,0,0,2,3,0,0,
85,0,0,3,0,0,15,417,0,0,0,0,0,1,0,
100,0,0,4,204,0,9,450,0,0,1,1,0,1,0,1,97,0,0,
119,0,0,4,0,0,13,456,0,0,8,0,0,1,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,
0,0,2,3,0,0,2,97,0,0,
166,0,0,4,0,0,15,499,0,0,0,0,0,1,0,
181,0,0,6,57,0,4,518,0,0,2,1,0,1,0,2,3,0,0,1,97,0,0,
204,0,0,5,154,0,9,536,0,0,1,1,0,1,0,1,97,0,0,
223,0,0,5,0,0,13,544,0,0,3,0,0,1,0,2,3,0,0,2,3,0,0,2,97,0,0,
250,0,0,5,0,0,15,561,0,0,0,0,0,1,0,
265,0,0,8,41,0,4,590,0,0,1,0,0,1,0,2,3,0,0,
284,0,0,7,259,0,9,603,0,0,0,0,0,1,0,
299,0,0,7,0,0,13,615,0,0,12,0,0,1,0,2,3,0,0,2,3,0,0,2,97,0,0,2,3,0,0,2,3,0,0,2,
3,0,0,2,3,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,3,0,0,2,97,0,0,
362,0,0,7,0,0,15,652,0,0,0,0,0,1,0,
377,0,0,10,57,0,4,808,0,0,1,0,0,1,0,2,3,0,0,
396,0,0,9,86,0,9,822,0,0,0,0,0,1,0,
411,0,0,9,0,0,13,829,0,0,2,0,0,1,0,2,3,0,0,2,97,0,0,
434,0,0,9,0,0,15,855,0,0,0,0,0,1,0,
449,0,0,11,78,0,4,876,0,0,4,1,0,1,0,2,3,0,0,2,3,0,0,2,3,0,0,1,97,0,0,
480,0,0,12,163,0,4,962,0,0,11,1,0,1,0,2,3,0,0,2,97,0,0,2,3,0,0,2,3,0,0,2,3,0,0,
2,3,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,3,0,0,1,3,0,0,
539,0,0,13,78,0,9,1043,0,0,1,1,0,1,0,1,3,0,0,
558,0,0,13,0,0,13,1053,0,0,1,0,0,1,0,2,3,0,0,
577,0,0,13,0,0,15,1061,0,0,0,0,0,1,0,
592,0,0,13,0,0,15,1075,0,0,0,0,0,1,0,
607,0,0,14,78,0,9,1082,0,0,1,1,0,1,0,1,3,0,0,
626,0,0,14,0,0,13,1092,0,0,1,0,0,1,0,2,3,0,0,
645,0,0,14,0,0,15,1099,0,0,0,0,0,1,0,
660,0,0,14,0,0,15,1106,0,0,0,0,0,1,0,
675,0,0,15,103,0,9,1153,0,0,1,1,0,1,0,1,3,0,0,
694,0,0,15,0,0,13,1161,0,0,4,0,0,1,0,2,4,0,0,2,97,0,0,2,97,0,0,2,3,0,0,
725,0,0,15,0,0,15,1206,0,0,0,0,0,1,0,
740,0,0,16,103,0,9,1236,0,0,2,2,0,1,0,1,3,0,0,1,4,0,0,
763,0,0,16,0,0,13,1249,0,0,2,0,0,1,0,2,3,0,0,2,97,0,0,
786,0,0,16,0,0,15,1282,0,0,0,0,0,1,0,
801,0,0,17,103,0,4,1336,0,0,4,3,0,1,0,2,97,0,0,1,3,0,0,1,4,0,0,1,3,0,0,
832,0,0,16,0,0,15,1358,0,0,0,0,0,1,0,
847,0,0,16,0,0,15,1364,0,0,0,0,0,1,0,
862,0,0,19,95,0,4,1393,0,0,5,2,0,1,0,2,97,0,0,2,3,0,0,2,3,0,0,1,3,0,0,1,4,0,0,
897,0,0,18,95,0,9,1419,0,0,2,2,0,1,0,1,3,0,0,1,4,0,0,
920,0,0,18,0,0,13,1431,0,0,1,0,0,1,0,2,9,0,0,
939,0,0,18,0,0,15,1451,0,0,0,0,0,1,0,
954,0,0,20,66,0,4,1478,0,0,3,1,0,1,0,2,97,0,0,2,97,0,0,1,3,0,0,
981,0,0,22,74,0,4,1552,0,0,3,2,0,1,0,2,3,0,0,1,3,0,0,1,4,0,0,
1008,0,0,23,74,0,4,1577,0,0,3,2,0,1,0,2,3,0,0,1,3,0,0,1,4,0,0,
1035,0,0,21,152,0,9,1605,0,0,2,2,0,1,0,1,3,0,0,1,4,0,0,
1058,0,0,21,0,0,13,1616,0,0,7,0,0,1,0,2,97,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,
2,3,0,0,2,97,0,0,
1101,0,0,21,0,0,15,1695,0,0,0,0,0,1,0,
1116,0,0,24,103,0,9,1740,0,0,2,2,0,1,0,1,3,0,0,1,4,0,0,
1139,0,0,24,0,0,13,1751,0,0,2,0,0,1,0,2,3,0,0,2,97,0,0,
1162,0,0,24,0,0,15,1787,0,0,0,0,0,1,0,
1177,0,0,25,126,0,9,1811,0,0,2,2,0,1,0,1,3,0,0,1,4,0,0,
1200,0,0,25,0,0,13,1825,0,0,4,0,0,1,0,2,3,0,0,2,3,0,0,2,97,0,0,2,97,0,0,
1231,0,0,25,0,0,15,1897,0,0,0,0,0,1,0,
1246,0,0,27,74,0,4,2039,0,0,3,2,0,1,0,2,3,0,0,1,3,0,0,1,4,0,0,
1273,0,0,26,111,0,9,2059,0,0,2,2,0,1,0,1,3,0,0,1,4,0,0,
1296,0,0,26,0,0,13,2069,0,0,3,0,0,1,0,2,3,0,0,2,3,0,0,2,97,0,0,
1323,0,0,26,0,0,15,2093,0,0,0,0,0,1,0,
1338,0,0,26,0,0,15,2102,0,0,0,0,0,1,0,
1353,0,0,26,0,0,15,2117,0,0,0,0,0,1,0,
1368,0,0,26,0,0,15,2126,0,0,0,0,0,1,0,
1383,0,0,26,0,0,15,2162,0,0,0,0,0,1,0,
1398,0,0,28,120,0,9,2182,0,0,3,3,0,1,0,1,3,0,0,1,4,0,0,1,3,0,0,
1425,0,0,28,0,0,13,2192,0,0,2,0,0,1,0,2,3,0,0,2,97,0,0,
1448,0,0,28,0,0,15,2216,0,0,0,0,0,1,0,
1463,0,0,29,80,0,9,2308,0,0,0,0,0,1,0,
1478,0,0,29,0,0,13,2314,0,0,6,0,0,1,0,2,97,0,0,2,3,0,0,2,97,0,0,2,97,0,0,2,3,0,
0,2,97,0,0,
1517,0,0,29,0,0,15,2353,0,0,0,0,0,1,0,
1532,0,0,30,92,0,4,2422,0,0,2,0,0,1,0,2,3,0,0,2,97,0,0,
1555,0,0,31,130,0,3,2448,0,0,9,9,0,1,0,1,97,0,0,1,97,0,0,1,3,0,0,1,97,0,0,1,3,
0,0,1,3,0,0,1,3,0,0,1,3,0,0,1,3,0,0,
1606,0,0,32,75,0,3,2482,0,0,4,4,0,1,0,1,97,0,0,1,3,0,0,1,3,0,0,1,97,0,0,
1637,0,0,33,0,0,29,2491,0,0,0,0,0,1,0,
1652,0,0,34,112,0,9,2518,0,0,0,0,0,1,0,
1667,0,0,34,0,0,13,2528,0,0,3,0,0,1,0,2,3,0,0,2,97,0,0,2,3,0,0,
1694,0,0,34,0,0,15,2648,0,0,0,0,0,1,0,
1709,0,0,35,84,0,4,2667,0,0,4,1,0,1,0,2,97,0,0,2,3,0,0,2,3,0,0,1,3,0,0,
1740,0,0,36,175,0,4,2731,0,0,10,6,0,1,0,2,3,0,0,2,3,0,0,2,4,0,0,2,4,0,0,1,97,0,
0,1,3,0,0,1,3,0,0,1,3,0,0,1,3,0,0,1,3,0,0,
1795,0,0,37,153,0,3,2760,0,0,11,11,0,1,0,1,97,0,0,1,3,0,0,1,3,0,0,1,3,0,0,1,3,
0,0,1,3,0,0,1,3,0,0,1,97,0,0,1,3,0,0,1,4,0,0,1,4,0,0,
1854,0,0,38,166,0,5,2765,0,0,10,10,0,1,0,1,3,0,0,1,3,0,0,1,4,0,0,1,4,0,0,1,97,
0,0,1,3,0,0,1,3,0,0,1,3,0,0,1,3,0,0,1,3,0,0,
1909,0,0,39,163,0,4,2837,0,0,9,6,0,1,0,2,3,0,0,2,3,0,0,2,4,0,0,1,3,0,0,1,3,0,0,
1,3,0,0,1,3,0,0,1,3,0,0,1,3,0,0,
1960,0,0,40,141,0,3,2867,0,0,10,10,0,1,0,1,3,0,0,1,3,0,0,1,3,0,0,1,3,0,0,1,3,0,
0,1,3,0,0,1,3,0,0,1,97,0,0,1,3,0,0,1,4,0,0,
2015,0,0,41,155,0,5,2872,0,0,9,9,0,1,0,1,3,0,0,1,3,0,0,1,4,0,0,1,3,0,0,1,3,0,0,
1,3,0,0,1,3,0,0,1,3,0,0,1,3,0,0,
2066,0,0,43,74,0,4,2934,0,0,3,2,0,1,0,2,3,0,0,1,97,0,0,1,97,0,0,
2093,0,0,42,81,0,9,2968,0,0,2,2,0,1,0,1,97,0,0,1,97,0,0,
2116,0,0,42,0,0,13,2979,0,0,2,0,0,1,0,2,3,0,0,2,97,0,0,
2139,0,0,42,0,0,15,2995,0,0,0,0,0,1,0,
2154,0,0,44,85,0,5,3031,0,0,4,4,0,1,0,1,3,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
};


/********************************************************************/
/******* COPY ( Oracle ) ********************************************/
/********************************************************************/
struct   sqlca
{
     char    sqlcaid[8];
     int     sqlabc;
     int     sqlcode;
     struct
     {
       unsigned short sqlerrml;
       char           sqlerrmc[70];
     } sqlerrm;
     char    sqlerrp[8];
     int     sqlerrd[6];
     char    sqlwarn[8];
     char    sqlext[8];
};

/********************************************************************/
#include	"MRCSMgr.h" 

#include    <stdio.h>                                                
#include    <stdlib.h>
#include    <ctype.h>
#include    <string.h>
#include "MItem.h"
#include    "MRCSComm.h"
#include    "MTUtil.h"
#include    "MTCtSect.h"
#include    "BREXLog.h"
#include "MRConfig.h"
#include    "MRLODRUL.h"
#include    "MRTOKEN.h"
#include    "MRSTRUCT.h"
#include    "MRPARSE.h"
#include    "MRDEFINE.h"
#include    "MRDTypes.h"

#include    "MRDTLIST.h"
#include    "MRGBLVAR.h"

#include    "BRTrace.h"


CMTCriticalSection      m_sect;


    #define	     SQLMESSAGE	((char*)sqlca.sqlerrm.sqlerrmc)
    #define      SQLCODE    sqlca.sqlcode
    #define      NOTFOUND   1403

	/* EXEC SQL BEGIN DECLARE SECTION; */ 
 
		sql_context     ctxLoad; 
	/* EXEC SQL END DECLARE SECTION; */ 


	/* EXEC SQL CONTEXT USE :ctxLoad; */ 

	
extern char               _DBPASSWORD[201]; //  QUALIFIER
extern char               _DBPWD_ENC[3]; //  ENC
	

/* EXEC SQL BEGIN DECLARE SECTION; */ 

    char         RS40_1[21];         // Pgm Id
    long      RS40_2;             // Start Rule Id
    long      RS40_3;             // Max Ret Count  
    short        RS40_4;             // Parm援щ텇 : 1.Object  2.Stream
    long      RS40_5;             // �떎�뻾湲곗��씪 ItemCd
    
    long      RS41_2;             // Item Cd
    short        RS41_3;             // Item Type
    short        RS41_4;             // Level   
    short        RS41_5;             // L1
    short        RS41_6;             // L2
    short        RS41_7;             // Max Occ
    long      RS41_8;             // Occ-Item Cd
	char         RS11_2[200+1];      // Item Name
	char		 RS32_2[200+1];      // Rule Name

    long      RS43_1;             // Rule No
    short        RS43_2;             // Rule �삎�깭
    char         RS43_3[201];        // Branch Name
    short        RS43_4;             // Preload
    short        RS43_5;             // reRun �뿬遺�
    short        RS43_6;             // Break �뿬遺�
    short        RS43_7;			 // Agg_Option  
	char         RS43_8[31];         // Group    
    char		 RS43_9[31];         // �젙�젹諛⑸쾿
	char		 RS43_A[201];        // Default
	short		 RS43_B;			 // Static�뿬遺�

	double		 RS44_1;
	
	char		 RS44_2[9];          // Start Date
	char		 RS44_3[9];          // End Date
	short        RS44_4;             // Sort�뿬遺�
	
	short        RS45_1;             // Seq
    char         RS45_2[3000+1];       // Result CONDITION_COLUMN_SIZE==> MITEM.H _KBIZ=100  �븘�땲硫� 50

	short		 RS4A_1;
    char         RS4A_2[3000+1];       // Value1
    char         RS4A_3[3000+1];       // Value2

    short        RS48_1;             // SEQ
    short        RS48_2;             // Gbn
    char		 RS48_3[3000+1];		 // Expression

	short        RS4D_2;             // Seq
    char         RS4D_3[101];        // �뙋�떒媛�
    
/* EXEC SQL END DECLARE SECTION; */ 
	


CMRLoadData *pCMRLoadData;

extern  PGM_LIST      *pPgmList;
extern  RULE_LIST     *pRuleList;

long		rId_RuleName;
long		rId_DAMBOLIST;
long		rId_DAMBOGM;
long		rConf_DAMBO_POS;
long		rConf_DAMBO_LENG;
int         isConnected;


#ifdef	_TRACE
extern	CBRTrace	*g_pCBRTrace;
#endif


int ConnectLoadDatabase( char *msg )
{

}


void  DB_Lock()
{
}
void  DB_Unlock()
{
}


void   BuildDataStructure()
{


	pCMRGblVars = new CMRGblVars();

	pCMRLoadData = new CMRLoadData();
	
#ifdef	_TRACE	
	g_pCBRTrace = new CBRTrace();
#endif


	pCMRLoadData->BuildConfiguration();

	g_pCBREXLog = new CBREXLog();
	
	_WriteLogNo( _LOG_LOG, "\n**************************************************\n" );
	_WriteLog( _LOG_LOG, "ChangeFlow Agent Start.\n" );
		
	configParm->DisplayConfigParm();

/***************************************************/

    _WriteLogNo( _LOG_LOG, "**************************************************"); 
    _WriteLog( _LOG_LOG, "Start Loading." ); 
            
    pCMRLoadData->BuildDataStructure();


    _WriteLogNo( _LOG_LOG, "**************************************************\n");  
}


//extern unsigned char    bIndexUsed;
extern char             UsedIndex[5];

/**********************************************************************/
//  Running�떆 Load�븞�맂�냸, Deploy濡� �궘�젣�맂�냸 Load 
int CMRLoadData::ReloadRuleBody( RULE_LIST *pRule )
{
    int    rc = 0;
	char   msg[255];

	if ( pRule->loaded == 'X' )	return -1;
	
//printf("here 0\n");
    m_sect.Lock();
    
//printf("here 1\n");
	if ( pRule->loaded == 'N' ) {

		if ( ConnectLoadDatabase( msg ) != 0 ) {
			m_sect.Unlock();
			return -1;
		}
//printf("after conn\n");
//		bIndexUsed = 0;
		strcpy(UsedIndex,"_NNN");

    	rc = LoadRuleBody( pRule );
//printf("aft load : %d\n", rc);

	}

    m_sect.Unlock();

    return rc;
}

/**********************************************************************/

CMRLoadData::CMRLoadData()
{
    pgmCnt = 0;
    readRuleCnt = 0;
	readRuleBodyCnt = 0;
}

CMRLoadData::~CMRLoadData()
{
//      freeRuleNode( pRuleList );
}




void   CMRLoadData::BuildDataStructure()
{                                           
    char msg[128];
    
    BuildPgmStruct();

    BuildRuleStruct();

	BuildNameList();

    BuildDataBaseStruct();

    _WriteLogNo( _LOG_LOG, " " );
    _WriteLog( _LOG_LOG, "End Loading." ); 
    _WriteLogNo( _LOG_LOG, "**************************************************\n");

    sprintf( msg, "Program Loaded : %d", pgmCnt );
    _WriteLogNo( _LOG_LOG, msg );
    sprintf( msg, "Rule    Loaded : Header(%d)  Body(%d)\n", readRuleCnt, readRuleBodyCnt );
    _WriteLogNo( _LOG_LOG, msg );

	pCMRGblVars->DisplayAllocSize( msg );
    _WriteLogNo( _LOG_LOG, msg );
    
/*    
    printf( "     Conditional Rule : %d\n", cRuleCnt );     
    printf( "     Query       Rule : %d\n", qRuleCnt );     
    printf( "     Brench      Rule : %d\n", bRuleCnt );     
    printf( "     External    Rule : %d\n", eRuleCnt );     
*/
         
}


void   CMRLoadData::BuildPgmStruct( void )
{
}


int   CMRLoadData::BuildStreamParms( PGM_LIST *pPgmPtr )
{
	return 0;
}

int   CMRLoadData::BuildObjParms( PGM_LIST *pPgmPtr )
{
	return 0;
}


void   CMRLoadData::BuildRuleStruct( void )
{

}



int CMRLoadData::LoadRuleHeader( RULE_LIST *pHead,
								 char *errMessage )
{
return 0;
}

RULE_AGGR * CMRLoadData::LoadRuleAggr()
{
	RULE_AGGR *pAggr;

	rtrim(RS43_8);   rtrim(RS43_9);
		
	if ( RS43_7 == 0 && RS43_9[0] == 0 ) return 0;

	pAggr = (RULE_AGGR *) pCMRGblVars->MetisAlloc( sizeof(RULE_AGGR) );

	pAggr->Option = RS43_7;
	strcpy(pAggr->group, RS43_8);


	int n = 0, idx;
	char *p;
	
	p = RS43_9;
	
	while ( *p ) {
		while ( *p ) { if (isgraph( *p ) ) break;   p++; }

		idx = 0;
		while ( *p >= '0' && *p <= '9' ) {
			idx = idx * 10 + (*p - '0');
			p ++;
		}
		
		pAggr->nSort[n] = idx;
		pAggr->cOrder[n] = 'A';
		if ( *p == 'D' || *p == 'A' ) pAggr->cOrder[n] = *p++;
		
		while ( *p ) { if ( isgraph(*p) ) break;   p++; }
		
		if ( *p == ',' ) p++;

		n++;		
	}

	pAggr->nSort[n] = 0;
    
	return pAggr;
}


void   CMRLoadData::BuildNameList( void )
{
}



//   Deploy�떆�뿉留� Reload
int   CMRLoadData::ReloadPgmById( char *pgmId, char *errMessage )
{
    return 0;
}


    
//  Deploy�떆 Header Reload
int CMRLoadData::ReloadRuleHeader( RULE_LIST *pNodeNew,
								 char *errMessage )
{
    return 0;
}


int CMRLoadData::LoadRuleReturn( RULE_LIST *pRulePtr )
{
    return 0;
}


int CMRLoadData::LoadRuleBody( RULE_LIST *pRule )
{

    return 0;
}

int CMRLoadData::loadConditionalRule( RULE_LIST *pRule, RULE_VERS *pVer )
{

	return 0;
}


int CMRLoadData::loadDynaSqlRule(RULE_LIST *pRule, RULE_VERS *pVer )
{
    
    return 0;    
}


int CMRLoadData::loadBranchRule( RULE_LIST *pRule )
{

    return 0;
}


int CMRLoadData::loadTableRule( RULE_LIST *pRule, RULE_VERS *pVer, unsigned char bSorted )
{

	return 0;
}


int CMRLoadData::loadTableRuleResData( RULE_LIST *pRule, TRULE_DATA *pTR )
{
	return 0;
}


int CMRLoadData::loadTableRuleColData( TRULE_DATA *pTR )
{
	return 0;
}


int  CMRLoadData::parseTRuleValue(char *str, TRULE_OPTR *v, short optr)
{
	return 0;
}




int CMRLoadData::loadFlowRule( RULE_LIST *pRule, RULE_VERS *pVer )
{
	return 0;
}


int CMRLoadData::loadFlowRuleSw( FRULE_DATA *pFR )
{
	return 0;
}


int CMRLoadData::loadExternalRule( RULE_LIST *pRule )
{
    return 0;
}


/****************************************************************/

extern  DATABASE_LIST *pDBList;

void   CMRLoadData::BuildDataBaseStruct( void )
{

}


/****************************************************************/
long CMRDataList::DispItems( char *PgmId, long cd, char *msg )
{
}

long CMRDataList::DispItemsProc( char *PgmId, long cd, char *msg )
{


	return 0;
}


/********************************************************************/

void   CMRLoadData::BuildConfiguration( void )
{
/*
    int RS16_3=0; 
    int RS16_1=0; 
    ReadEngineConfig();
   
        
        rtrim(RS16_3);


		switch ( RS16_1 ) {
		   case 10000 : //   Product Name
						strcpy( configRec.lpszProdName, RS16_3 );
						break;
		   case 10001 : //   Thread count per Engine
						configRec.ulNoThread = atoi( RS16_3 );
						if ( configRec.ulNoThread < 1 ) configRec.ulNoThread = 1;
						if ( configRec.ulNoThread > 50 ) configRec.ulNoThread = 50;
						configRec.DBAgentCount = configRec.ulNoThread;
						break;
		   case 10002 : //   Max Listening Queue
						configRec.ulNoMaxQueueing = atoi( RS16_3 );
						if ( configRec.ulNoMaxQueueing < 10 ) configRec.ulNoMaxQueueing = 10;
						if ( configRec.ulNoMaxQueueing > 500 ) configRec.ulNoMaxQueueing = 500;						
						break;
		   case 10003 : //   Sleep Time
						configRec.ulSlTm = atoi( RS16_3 );
						if ( configRec.ulSlTm <= 0 ) configRec.ulSlTm = 0;
						else if ( configRec.ulSlTm < 5000 ) configRec.ulSlTm = 5000;
						else if ( configRec.ulSlTm > 20000 ) configRec.ulSlTm = 20000;
						break;
		   case 10004 : //   Keep connection
						configRec.ulKeepFlag = atoi( RS16_3 );
						break;						
		   case 10005 : //   Parm Size
						configRec.ulMaxParmSize = atoi( RS16_3 );
						if ( configRec.ulMaxParmSize < 1024 ) configRec.ulMaxParmSize = 1024; 
						else if ( configRec.ulMaxParmSize > 300000 ) configRec.ulDiagIntv = 300000; 
						break;
		   case 10006 : //   Diagnose Interval
						configRec.ulDiagIntv = atoi( RS16_3 );
						
						if ( configRec.ulDiagIntv <= 0 ) configRec.ulDiagIntv = 0;
						else if ( configRec.ulDiagIntv < 5 ) configRec.ulDiagIntv = 5;
						else if ( configRec.ulDiagIntv > 120 ) configRec.ulDiagIntv = 120;
						break;						
		   case 10010 : //   Log Path
						strcpy( configRec.lpszLogPath, RS16_3 );						
						if ( configRec.lpszLogPath[0] == 0 ) strcpy(configRec.lpszLogPath,".");
	      			#ifdef WIN32
						if ( configRec.lpszLogPath[0] == '.' && configRec.lpszLogPath[1] == '/' ){
							configRec.lpszLogPath[1] = '\\';
		      				}
	      			#else
		      				if ( configRec.lpszLogPath[0] == '.' && configRec.lpszLogPath[1] == '\\' ){
							configRec.lpszLogPath[1] = '/';
		      				}
	      			#endif
						break;
		   case 10011 : //   Trace Path
						strcpy( configRec.lpszTrcPath, RS16_3 );						
						if ( configRec.lpszTrcPath[0] == 0 ) strcpy(configRec.lpszTrcPath,".");
	      			#ifdef WIN32
			      			if ( configRec.lpszTrcPath[0] == '.' && configRec.lpszTrcPath[1] == '/' ){
							configRec.lpszTrcPath[1] = '\\';
			      			}
	      			#else
			      			if ( configRec.lpszTrcPath[0] == '.' && configRec.lpszTrcPath[1] == '\\' ){
							configRec.lpszTrcPath[1] = '/';
		      				}
	      			#endif   
	      			break;
						break;
		   case 10012 : //   Log Queue Size
						configRec.ulLogQueueSz = atoi( RS16_3 );
						
						if ( configRec.ulLogQueueSz < 500 ) configRec.ulLogQueueSz = 500;
						if ( configRec.ulLogQueueSz > 5000 ) configRec.ulLogQueueSz = 5000;						
						break;
		   case 10013 : //   Disp Elapse Time
						configRec.usDispTm = atoi( RS16_3 );
						break;
		   case 10014 : //   Preload
						configRec.PreLoad[0] = RS16_3[0];
						break;	
		   case 10015 : //   Max Fetch Count
						configRec.ulMaxQueryCount = atoi( RS16_3 );
						break;
		   case 10016 : //   Run Timeout
						configRec.ulRunTimeOut = atoi( RS16_3 );
						break;
		   case 10017 : //   Thread Buffer Size
						configRec.ulMaxMemSize1 = atoi( RS16_3 ) * 1024;
						break;																																					
		   case 10018 : //   Thread Temp Buffer 1
						configRec.ulMaxMemSize2 = atoi( RS16_3 ) * 1024;
						break;																																					
		   case 10019 : //   Thread Temp Buffer 2
						configRec.ulMaxMemSize3 = atoi( RS16_3 ) * 1024;
						break;
		   case 10020 : //   Trace Always
						configRec.usTrace = atoi( RS16_3 );
                        break;
		   case 10021 : //   Trace Always
						configRec.usAllTracle = atoi( RS16_3 );
						break;
		   case 10022 : //   us Rule Apply Time 
						configRec.usRuleApplyTime =atoi( RS16_3 );
						break;
		   default    :
						break;
		}
*/
}


//  Engine �젙蹂�
void   CMRLoadData::ReadEngineConfig( void )
{

	configRec.usPort = 34001;
	configRec.amPort = 34002;
}




int CMRLoadData::SaveStatisticsPgm( struct tm *newtime, char *errMessage )
{
	return 0;
}


int	CMRLoadData::SaveStatisticsRule( struct tm *newtime, RULE_LIST *pRulePtr, char *errMessage )
{
	return 0;
}


int  CBRTrace::AddTraceInfo( char *dt, char *pgmId )
{
			
	return 0;
}


long CMRDataList::SaveTraceInfo( char *dt, char *pgmId )
{
	return 0;
}

unsigned int CCSManager::setEngineInfo( unsigned long param_engineId, unsigned short param_queueSize, unsigned long param_defaultQueueID, 
						unsigned long param_threadCount, ENGINE_INFO param_engine_info[] ) {
						
	return 0;
}
