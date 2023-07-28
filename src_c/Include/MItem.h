#ifndef     _INC_MITEM
#define     _INC_MITEM

//#define 	DEFAULT_INIFILE             "C:\\eclise_Engine\\W_BR\\BRMiner_engine4.5\\Bin\\WIN\\MySQL57\\BRMCONF.ini"
//#define 	DEFAULT_INIFILE             "C:\\eclise_Engine\\W_BR\\BRMiner_engine4.5\\Bin\\WIN\\POSTGRE\\AML60_SBS.INI"
//#define 	KEY_FILE                    "C:\\eclise_Engine\\W_BR\\BRMiner_engine4.5\\Bin\\WIN\\POSTGRE\\key.dat"

#if	defined(WIN32)
#define 	DEFAULT_INIFILE             "./agent.properties"
#define 	KEY_FILE             "./key.dat"

#elif defined(_OS390) || defined(_ZOS)
    #define DEFAULT_INIFILE             "DD:BRMCONF"
#else
    #define DEFAULT_INIFILE             "./agent.properties"
	#define 	KEY_FILE             "./key.dat"
#endif

//char _DEBUG_YN[2];

#ifdef _KBIZ
  #define STR_VALUE_SIZE 100
#else
  #define STR_VALUE_SIZE 50
#endif


// QUERY  2020.06.01

#define  OBJ_MAX_LENGTH    2048000 //1024000 /*500000*/


/* 파싱 컬럼 사이즈 1000 
BRS360TB    RS36_2    VARCHAR2	  -- 값
BRS360TB    RS36_3    VARCHAR2    -- 값  
BRS390TB    RS39_3    VARCHAR2	  -- 판단값
BRS3A0TB    RS3A_1    VARCHAR2	  -- 값 
BRS3I0TB    RS3I_2    VARCHAR2    -- 값 MONTHS({69::2}(#1),TODAY())
BRS3N0TB    RS32_B    VARCHAR2    -- DEFAULT 값
	BRS450TB    RS45_2    VARCHAR2  -- Result 
	BRS480TB    RS48_3    VARCHAR2  -- {30487::1},GETNVAL("total score"),{30487::3}
	BRS4A0TB    RS4A_2    VARCHAR2	--  RS360TB.RS36_2
	BRS4A0TB    RS4A_3    VARCHAR2  --  RS360TB.RS36_3 
BRS5I0TB    RS5I_4    VARCHAR2;

BRS4D0TB RS4D_3 100 
*/

#define CONDITION_COLUMN_SIZE  3001

#define PARAM_MAX_COUNT 200	/*100*/


#define MAX_QUERY_SIZE 10000   /* MAX_QUERY_SIZE 10000 */

//#define CONDITION_STR_SIZE 1000  /*1000,4000,20000*/
#define CONDITION_STR_SIZE 3001  /*1000,4000,20000*/

//#define MAX_QUERY_SIZE 20000   /* MAX_QUERY_SIZE 10000 */
#define	_ADMIN_PARM_SZ 30000   /*	_ADMIN_PARM_SZ 10000 */
#define  _PARM_SIZE    50000    /* _PARM_SIZE   50000   */


#define  JNI_PROGRAMID_COUNT_PARM_SIZE 1000    /* JNI_PROGRAMID_COUNT_PARM_SIZE   1000   */

#define QUERY_COL_RESULT_SIZE 4000 /* 1000 */

#endif   
 
 
  	 
 
