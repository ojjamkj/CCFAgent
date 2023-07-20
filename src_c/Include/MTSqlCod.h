#ifdef _DB2
	#ifndef  SQLCODE
		#define  SQLCODE   sqlca.sqlcode
	#endif
	#ifndef  SQLMESSAGE
		#define  SQLMESSAGE   ((char*)sqlca.sqlerrmc)
	#endif
	
	#define  SQL_OK          0
	#define  SQL_ERR        -1	
	#define  NOTFOUND      100
	#define  SQL_DUP   	   -803
#endif

#ifdef _ORACLE
	#ifndef  SQLCODE
		#define  SQLCODE   sqlca.sqlcode
	#endif
	#ifndef  SQLMESSAGE
		#define  SQLMESSAGE   ((char*)sqlca.sqlerrm.sqlerrmc)
	#endif

	#define  SQL_OK          0
	#define  SQL_ERR        -1	
	#define  NOTFOUND        1403
	#define  SQL_DUP   	     -1

#endif
 
#ifdef _MYSQL 
#ifndef  SQLMESSAGE
#define  SQLMESSAGE   ((char*)sqlca.sqlerrm.sqlerrmc)
#endif
#define  SQL_OK          0
#define  SQL_ERR        -1	
#define  NOTFOUND        1403
#define  SQL_DUP   	     -1
#endif

#ifdef _MYSQL50 
#ifndef  SQLMESSAGE
#define  SQLMESSAGE   ((char*)sqlca.sqlerrm.sqlerrmc)
#endif
#define  SQL_OK          0
#define  SQL_ERR        -1	
#define  NOTFOUND        1403
#define  SQL_DUP   	     -1
#endif

#ifdef _MYSQL80 
#ifndef  SQLMESSAGE
#define  SQLMESSAGE   ((char*)sqlca.sqlerrm.sqlerrmc)
#endif
#define  SQL_OK          0
#define  SQL_ERR        -1	
#define  NOTFOUND        1403
#define  SQL_DUP   	     -1
#endif

#ifdef _MARIA
#ifndef  SQLMESSAGE
#define  SQLMESSAGE   ((char*)sqlca.sqlerrm.sqlerrmc)
#endif
#define  SQL_OK          0
#define  SQL_ERR        -1	
#define  NOTFOUND        1403
#define  SQL_DUP   	     -1
#endif

#ifdef _SYBASE
	#ifndef  SQLCODE
		#define  SQLCODE   sqlca.sqlcode
	#endif
	#ifndef  SQLMESSAGE
		#define  SQLMESSAGE   ((char*)sqlca.sqlerrm.sqlerrmc)
	#endif

	#define  SQL_OK          0
	#define  SQL_ERR        -1
	#define  NOTFOUND        100
	#define  SQL_DUP   	   -1

#endif
#ifdef _TIBERO
	#ifndef  SQLCODE
		#define  SQLCODE   sqlca.sqlcode
	#endif
	#ifndef  SQLMESSAGE
		#define  SQLMESSAGE   ((char*)sqlca.sqlerrm.sqlerrmc)
	#endif

	#define  SQL_OK       0  
	#define  SQLOK        0

	#define  SQL_ERR      -1

	#define  NOTFOUND     1403
	#define  SQLNOTFOUND  1403

	#define  SQL_DUP   	  1
	#define  SQLDUP   	  1

  #define  MAXLEN 			50
  #define  NAMELEN 			11
  #define  COMMIT				1
  #define  ROLLBACK 		2
#endif

#ifdef _POSTGRE
#ifndef  SQLMESSAGE
#define  SQLMESSAGE   ((char*)sqlca.sqlerrm.sqlerrmc)
#endif
#define  SQL_OK          0
#define  SQL_ERR        -1	
#define  NOTFOUND        2000
#define  SQL_DUP   	     -1
#endif
