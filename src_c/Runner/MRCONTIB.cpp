#ifndef _TBPC_COMMON_SRC_GEN
#define _TBPC_COMMON_SRC_GEN

 

#define TBSQLCODE_NOTFOUND 1403



/*
==========================================
        host variable type
==========================================
*/

typedef enum esql_hostvar_type_e {
	 
    /* string */
    ESQL_TYPE_CHARP = 0,        /* (signed) char* */
    ESQL_TYPE_UCHARP,           /* unsigned char* */
    ESQL_TYPE_CHAR_ARRAY,       /* (signed) char[n] */
    ESQL_TYPE_UCHAR_ARRAY,      /* unsigned char[n] */

    /* char */
    ESQL_TYPE_CHAR,             /* (signed) char */
    ESQL_TYPE_UCHAR,            /* unsigned char */

    /* int */
    ESQL_TYPE_INT,              /* (signed) int */
    ESQL_TYPE_UINT,             /* unsigned int */

    /* short */
    ESQL_TYPE_SHORT,            /* (signed) short (int) */
    ESQL_TYPE_USHORT,           /* unsigned short */

    /* long */
    ESQL_TYPE_LONG,             /* (signed) long (int) */
    ESQL_TYPE_ULONG,            /* unsigned long (int) */

    /* float/double */
    ESQL_TYPE_FLOAT,            /* float */
    ESQL_TYPE_DOUBLE,           /* double */

    /* VARCHAR */
    ESQL_TYPE_VARCHAR,          /* VARCHAR[n] */
    ESQL_TYPE_VARCHARP,         /* VARCHAR* */

    /* EXTERNAL DATATYPE */
    ESQL_TYPE_VARRAW,           /* VARRAW */
    ESQL_TYPE_LONG_VARRAW,      /* LONG VARRAW */

    /* EXTENED DATATYPE */
    ESQL_TYPE_NCHAR_ARRAY,      /* NCHAR */

    /* LOB */
    ESQL_TYPE_CLOB,             /* CLOB */
    ESQL_TYPE_BLOB,             /* BLOB */
    ESQL_TYPE_BFILE,            /* BFILE */

    /* COBOL COMP3 */
    ESQL_TYPE_CBL_COMP3,        /* COBOL COMP3 */
    ESQL_TYPE_EXT_VARCHAR,      /* external datatype */

    /* CURSOR */
    ESQL_TYPE_CURSOR,           /* SQL_CURSOR */

    ESQL_HVTYPE_EXT_NUMBER          = 30,
    ESQL_HVTYPE_EXT_VARNUM          = 31,
    ESQL_HVTYPE_EXT_INTEGER         = 32,
    ESQL_HVTYPE_EXT_FLOAT           = 33,
    ESQL_HVTYPE_EXT_LONG            = 34,
    ESQL_HVTYPE_EXT_UNSIGNED        = 35,
    ESQL_HVTYPE_EXT_CHAR            = 36,
    ESQL_HVTYPE_EXT_CHARZ           = 37,
    ESQL_HVTYPE_EXT_CHARF           = 38,
    ESQL_HVTYPE_EXT_STRING          = 39,
    ESQL_HVTYPE_EXT_VARCHAR         = 40,
    ESQL_HVTYPE_EXT_VARCHAR2        = 41,
    ESQL_HVTYPE_EXT_LONG_VARCHAR    = 42,
    ESQL_HVTYPE_EXT_VARRAW          = 43,
    ESQL_HVTYPE_EXT_LONG_VARRAW     = 44,
    ESQL_HVTYPE_EXT_RAW             = 45,
    ESQL_HVTYPE_EXT_ROWID           = 46 
} esql_hostvar_type_t;



/*
==========================================
       ESQL type 
==========================================
*/

typedef enum esql_type_t {
    /* CONNECT */
    ESQL_TYPE_CONNECT = 1,

    /* DML */
    ESQL_TYPE_SELECT,
    ESQL_TYPE_EXECUTE_DML,

    /* cursor */
    ESQL_TYPE_OPEN_CURSOR,
    ESQL_TYPE_CLOSE_CURSOR,
    ESQL_TYPE_FETCH,

    ESQL_TYPE_COMMIT,
    ESQL_TYPE_COMMIT_RELEASE,
    ESQL_TYPE_ROLLBACK,
    ESQL_TYPE_ROLLBACK_RELEASE,
    ESQL_TYPE_ROLLBACK_TO_SAVEPOINT,

    /* DDL */
    ESQL_TYPE_DDL,  

    /* PSM */
    ESQL_TYPE_PSM,  

    ESQL_TYPE_DYN_EXEC_IMMED,
    ESQL_TYPE_DYN_EXECUTE,
    ESQL_TYPE_DYN_PREPARE,
    ESQL_TYPE_DYN_OPEN_CURSOR,
    ESQL_TYPE_DYN_DESC_BIND,
    ESQL_TYPE_DYN_DESC_SELECT,

    ESQL_TYPE_ANSIDYN_ALLOC_DESC,
    ESQL_TYPE_ANSIDYN_DEALLOC_DESC,
    ESQL_TYPE_ANSIDYN_SET_DESC,
    ESQL_TYPE_ANSIDYN_GET_DESC,
    ESQL_TYPE_ANSIDYN_DESC_INPUT,
    ESQL_TYPE_ANSIDYN_DESC_OUTPUT,

    /* THREAD & CONTEXT */
    ESQL_TYPE_ENABLE_THREADS,
    ESQL_TYPE_CONTEXT_ALLOCATE,
    ESQL_TYPE_CONTEXT_USE,
    ESQL_TYPE_CONTEXT_FREE,

    /* ALLOCATE & FREE */
    ESQL_TYPE_ALLOCATE,
    ESQL_TYPE_FREE,

    /* LOB */
    ESQL_TYPE_LOB_APPEND,
    ESQL_TYPE_LOB_ASSIGN,
    ESQL_TYPE_LOB_CLOSE,
    ESQL_TYPE_LOB_COPY,
    ESQL_TYPE_LOB_CREATE_TEMP,
    ESQL_TYPE_LOB_DESCRIBE,
    ESQL_TYPE_LOB_ERASE,
    ESQL_TYPE_LOB_FREE_TEMP,
    ESQL_TYPE_LOB_OPEN,
    ESQL_TYPE_LOB_READ,
    ESQL_TYPE_LOB_TRIM,
    ESQL_TYPE_LOB_WRITE,

    /* ETC */
    ESQL_TYPE_START_SQL,
    ESQL_TYPE_END_SQL,

    ESQL_TYPE_REGISTER_CONNECT,
    ESQL_TYPE_XA_SET_CONNECTION
} esql_type_t;



/*
==========================================
       char_map 
==========================================


CHAR_MAP  fixed-length  blank-padded  null-terminated  NULL is fixed-length
(DBMS)                                                      blank-padded
---------------------------------------------------------------------------
VARCHAR2        o           o             x               o
CHARF           o           o             x               x
CHARZ           o           o             o               o
STRING          x           x             o               x
*/

typedef enum char_map_e {
    CHAR_MAP_DEFAULT = 0,
    CHAR_MAP_VARCHAR2,
    CHAR_MAP_CHARF,
    CHAR_MAP_CHARZ,
    CHAR_MAP_STRING
} char_map_t;



typedef enum esql_desc_item_e {
    ESQL_DESC_ITEM_NONE = 0,
    ESQL_DESC_ITEM_TYPE,
    ESQL_DESC_ITEM_LENGTH,
    ESQL_DESC_ITEM_INDICATOR,
    ESQL_DESC_ITEM_INDICATOR_P,
    ESQL_DESC_ITEM_DATA,
    ESQL_DESC_ITEM_DATA_P,
    ESQL_DESC_ITEM_OCTET_LENGTH,
    ESQL_DESC_ITEM_RETURNED_LENGTH,
    ESQL_DESC_ITEM_RETURNED_LENGTH_P,
    ESQL_DESC_ITEM_RETURNED_OCTET_LENGTH,
    ESQL_DESC_ITEM_PRECISION,
    ESQL_DESC_ITEM_SCALE,
    ESQL_DESC_ITEM_NULLABLE,
    ESQL_DESC_ITEM_NAME,
    ESQL_DESC_ITEM_CHARACTER_SET_NAME,
    ESQL_DESC_COUNT
} esql_desc_item_t;



#include "sqlca.h"


typedef void * sql_context;
typedef void * SQL_CONTEXT;



typedef void * sql_cursor;
typedef void * SQL_CURSOR;



typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;




static struct esql_sqlctx {
	esql_type_t     stmt_type;

	char           *db_name;
	char           *cursor_name;
	char           *pstmt_name;
	char           *savepoint;
	char           *stmt;

	char_map_t      char_map;
	int             stmt_cache_size;
	int             fetch_type;
	int             abs_rel_nth;
	int             flag;
	int             option;

	struct sqlca   *sqlca;
	void           *sqlda;

	char           *desc_name;
	char           *desc_into;
	int             value;

	sql_context    *sql_ctx;
	char           *sqlstate;

	unsigned int    file_id;
	unsigned int    prefetch;

	int             rmid;

	unsigned int    dummy[2];

	esql_sqlctx_ext_t *sqlctx_ext;

	void           *oraca;
	void           *ep_ctx;

	unsigned int    iter_cnt;               /* count of iteration */
	unsigned int    host_var_total_cnt;     /* total # of host var */
	unsigned int    host_var_out_cnt;       /* # of out host var */

	void               **p_hostvar;         /* array of host var */
	unsigned int        *p_hostvar_len;     /* array of length of host var */
	unsigned int        *p_hostvar_stride;  /* array of stride of host var */
	esql_hostvar_type_t *p_hostvar_type;    /* array of host var type */
	unsigned int        *p_param_type;      /* array of param type */
	esql_desc_item_t    *p_ansi_dyn_type;   /* array of ansi dyn item type */
	void               **p_hostind;         /* array of host ind */
	unsigned int        *p_hostind_stride;  /* array of stride of host ind */
	long long           *p_ind;             /* array of indicator */


	void                *hostvar[12];        /* array of host var */
	unsigned int         hostvar_len[12];    /* array of length of host var */
	unsigned int         hostvar_stride[12]; /* array of stride of host var */
	esql_hostvar_type_t  hostvar_type[12];   /* array of host var type */
	unsigned int         param_type[12];     /* array of param type */
	esql_desc_item_t     ansi_dyn_type[12];  /* array of ansi dyn item type */
	void                *hostind[12];        /* array of host ind */
	unsigned int         hostind_stride[12]; /* array of stride of host ind */
	long long            ind[12];            /* array of indicator */

} sqlstm;



extern "C" { 
        void esql_do(struct esql_sqlctx *_sqlctx);
}



#endif



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


	/*
		EXEC SQL CONTEXT USE :ctxConn; 
	 
{
    struct esql_sqlctx  __sqlctx;
    memset(&__sqlctx, 0, sizeof(struct esql_sqlctx));
    __sqlctx.stmt_type   = ESQL_TYPE_CONTEXT_USE;
    __sqlctx.db_name     = "";
    __sqlctx.cursor_name = "";
    __sqlctx.pstmt_name  = "";
    __sqlctx.savepoint   = "";
    __sqlctx.stmt        = "";
    __sqlctx.char_map    = CHAR_MAP_CHARZ;
    __sqlctx.stmt_cache_size = 10;
    __sqlctx.fetch_type  = 0;
    __sqlctx.abs_rel_nth = 0;
    __sqlctx.flag        = 0;
    __sqlctx.option      = 268435552;
    __sqlctx.sqlca       = (struct sqlca *)&sqlca;
    __sqlctx.oraca       = (void *)0;
    __sqlctx.sqlda       = (void *)0;
    __sqlctx.desc_name   = "";
    __sqlctx.desc_into   = "";
    __sqlctx.value       = 0;
    __sqlctx.sql_ctx     = &ctxConn;
    __sqlctx.ep_ctx      = (void *)0;
    __sqlctx.sqlstate    = (char *)0;
    __sqlctx.file_id     = 112;
    __sqlctx.prefetch    = 1;
    __sqlctx.rmid        = -1;
      sqlstm.stmt        = __sqlctx.stmt;
    esql_do(&__sqlctx);
}*/
 
//#include   "MTCSComm.h"

long  AllocThreadCtx( void **ctxParm )
{

	/*
		EXEC SQL    BEGIN DECLARE SECTION;
	 */
	sql_context ctxConn;

	/*
		EXEC SQL    END DECLARE SECTION;
	 */
	struct sqlca    sqlca;

	
	/*
		EXEC SQL ENABLE THREADS;
	 */
{
    struct esql_sqlctx  __sqlctx;
    memset(&__sqlctx, 0, sizeof(struct esql_sqlctx));
    __sqlctx.stmt_type   = ESQL_TYPE_ENABLE_THREADS;
    __sqlctx.db_name     = "";
    __sqlctx.cursor_name = "";
    __sqlctx.pstmt_name  = "";
    __sqlctx.savepoint   = "";
    __sqlctx.stmt        = "";
    __sqlctx.char_map    = CHAR_MAP_CHARZ;
    __sqlctx.stmt_cache_size = 10;
    __sqlctx.fetch_type  = 0;
    __sqlctx.abs_rel_nth = 0;
    __sqlctx.flag        = 0;
    __sqlctx.option      = 268435552;
    __sqlctx.sqlca       = (struct sqlca *)&sqlca;
    __sqlctx.oraca       = (void *)0;
    __sqlctx.sqlda       = (void *)0;
    __sqlctx.desc_name   = "";
    __sqlctx.desc_into   = "";
    __sqlctx.value       = 0;
    __sqlctx.sql_ctx     = (sql_context *)0;
    __sqlctx.ep_ctx      = (void *)0;
    __sqlctx.sqlstate    = (char *)0;
    __sqlctx.file_id     = 112;
    __sqlctx.prefetch    = 1;
    __sqlctx.rmid        = -1;
      sqlstm.stmt        = __sqlctx.stmt;
    esql_do(&__sqlctx);
}

	
	/*
		EXEC SQL CONTEXT ALLOCATE :ctxConn;
	 */
{
    struct esql_sqlctx  __sqlctx;
    memset(&__sqlctx, 0, sizeof(struct esql_sqlctx));
    __sqlctx.stmt_type   = ESQL_TYPE_CONTEXT_ALLOCATE;
    __sqlctx.db_name     = "";
    __sqlctx.cursor_name = "";
    __sqlctx.pstmt_name  = "";
    __sqlctx.savepoint   = "";
    __sqlctx.stmt        = "";
    __sqlctx.char_map    = CHAR_MAP_CHARZ;
    __sqlctx.stmt_cache_size = 10;
    __sqlctx.fetch_type  = 0;
    __sqlctx.abs_rel_nth = 0;
    __sqlctx.flag        = 0;
    __sqlctx.option      = 268435552;
    __sqlctx.sqlca       = (struct sqlca *)&sqlca;
    __sqlctx.oraca       = (void *)0;
    __sqlctx.sqlda       = (void *)0;
    __sqlctx.desc_name   = "";
    __sqlctx.desc_into   = "";
    __sqlctx.value       = 0;
    __sqlctx.sql_ctx     = &ctxConn;
    __sqlctx.ep_ctx      = (void *)0;
    __sqlctx.sqlstate    = (char *)0;
    __sqlctx.file_id     = 112;
    __sqlctx.prefetch    = 1;
    __sqlctx.rmid        = -1;
      sqlstm.stmt        = __sqlctx.stmt;
    esql_do(&__sqlctx);
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

	/*
		EXEC SQL    BEGIN DECLARE SECTION;
	 */
	sql_context ctxConn;

	/*
		EXEC SQL    END DECLARE SECTION;
	 */
	struct sqlca    sqlca;
	
	ctxConn = ctxParm;
	
	
	/*
		EXEC SQL COMMIT;
	 */
{
    struct esql_sqlctx  __sqlctx;
    memset(&__sqlctx, 0, sizeof(struct esql_sqlctx));
    __sqlctx.stmt_type   = ESQL_TYPE_COMMIT;
    __sqlctx.db_name     = "";
    __sqlctx.cursor_name = "";
    __sqlctx.pstmt_name  = "";
    __sqlctx.savepoint   = "";
    __sqlctx.stmt        = "";
    __sqlctx.char_map    = CHAR_MAP_CHARZ;
    __sqlctx.stmt_cache_size = 10;
    __sqlctx.fetch_type  = 0;
    __sqlctx.abs_rel_nth = 0;
    __sqlctx.flag        = 0;
    __sqlctx.option      = 268435552;
    __sqlctx.sqlca       = (struct sqlca *)&sqlca;
    __sqlctx.oraca       = (void *)0;
    __sqlctx.sqlda       = (void *)0;
    __sqlctx.desc_name   = "";
    __sqlctx.desc_into   = "";
    __sqlctx.value       = 0;
    __sqlctx.sql_ctx     = &ctxConn;
    __sqlctx.ep_ctx      = (void *)0;
    __sqlctx.sqlstate    = (char *)0;
    __sqlctx.file_id     = 112;
    __sqlctx.prefetch    = 1;
    __sqlctx.rmid        = -1;
      sqlstm.stmt        = __sqlctx.stmt;
    esql_do(&__sqlctx);
}
	
	return 0;
}	

void FreeCtx( void **ctxParm )
{

	/*
		EXEC SQL    BEGIN DECLARE SECTION;
	 */
	 sql_context ctxConn;

	/*
		EXEC SQL    END DECLARE SECTION;
	 */
	struct sqlca    sqlca;

	if ( ! *ctxParm ) return;
	
	ctxConn = *ctxParm;

	
	/*
		EXEC SQL ROLLBACK WORK RELEASE;
	 */
{
    struct esql_sqlctx  __sqlctx;
    memset(&__sqlctx, 0, sizeof(struct esql_sqlctx));
    __sqlctx.stmt_type   = ESQL_TYPE_ROLLBACK_RELEASE;
    __sqlctx.db_name     = "";
    __sqlctx.cursor_name = "";
    __sqlctx.pstmt_name  = "";
    __sqlctx.savepoint   = "";
    __sqlctx.stmt        = "";
    __sqlctx.char_map    = CHAR_MAP_CHARZ;
    __sqlctx.stmt_cache_size = 10;
    __sqlctx.fetch_type  = 0;
    __sqlctx.abs_rel_nth = 0;
    __sqlctx.flag        = 0;
    __sqlctx.option      = 268435552;
    __sqlctx.sqlca       = (struct sqlca *)&sqlca;
    __sqlctx.oraca       = (void *)0;
    __sqlctx.sqlda       = (void *)0;
    __sqlctx.desc_name   = "";
    __sqlctx.desc_into   = "";
    __sqlctx.value       = 0;
    __sqlctx.sql_ctx     = &ctxConn;
    __sqlctx.ep_ctx      = (void *)0;
    __sqlctx.sqlstate    = (char *)0;
    __sqlctx.file_id     = 112;
    __sqlctx.prefetch    = 1;
    __sqlctx.rmid        = -1;
      sqlstm.stmt        = __sqlctx.stmt;
    esql_do(&__sqlctx);
}

   
	/*
		EXEC SQL CONTEXT FREE :ctxConn;
	 */
{
    struct esql_sqlctx  __sqlctx;
    memset(&__sqlctx, 0, sizeof(struct esql_sqlctx));
    __sqlctx.stmt_type   = ESQL_TYPE_CONTEXT_FREE;
    __sqlctx.db_name     = "";
    __sqlctx.cursor_name = "";
    __sqlctx.pstmt_name  = "";
    __sqlctx.savepoint   = "";
    __sqlctx.stmt        = "";
    __sqlctx.char_map    = CHAR_MAP_CHARZ;
    __sqlctx.stmt_cache_size = 10;
    __sqlctx.fetch_type  = 0;
    __sqlctx.abs_rel_nth = 0;
    __sqlctx.flag        = 0;
    __sqlctx.option      = 268435552;
    __sqlctx.sqlca       = (struct sqlca *)&sqlca;
    __sqlctx.oraca       = (void *)0;
    __sqlctx.sqlda       = (void *)0;
    __sqlctx.desc_name   = "";
    __sqlctx.desc_into   = "";
    __sqlctx.value       = 0;
    __sqlctx.sql_ctx     = &ctxConn;
    __sqlctx.ep_ctx      = (void *)0;
    __sqlctx.sqlstate    = (char *)0;
    __sqlctx.file_id     = 112;
    __sqlctx.prefetch    = 1;
    __sqlctx.rmid        = -1;
      sqlstm.stmt        = __sqlctx.stmt;
    esql_do(&__sqlctx);
}
}


/*
int ConnectDatabase( sql_context ctxParm,
						   char *DBName, char *DBUser,
                     char *DBPasswd, char *msg )
*/    

int ConnectDatabase( sql_context ctxParm, char *msg )
{

	/*
		EXEC SQL BEGIN DECLARE SECTION;
	 */
	/*     VARCHAR        uid[32];
	*/
struct __tag01 { unsigned short len; unsigned char arr[32]; } uid;
	/*     VARCHAR        pwd[32];
	*/
struct __tag02 { unsigned short len; unsigned char arr[32]; } pwd;
	/*     VARCHAR        dbName[32];
	*/
struct __tag03 { unsigned short len; unsigned char arr[32]; } dbName;
    sql_context    ctxConn;

	/*
		EXEC SQL END DECLARE SECTION; 
	 */

    struct sqlca	sqlca;

    strcpy( (char *) uid.arr, _DBUSER );
    uid.len = strlen((char *) uid.arr);
    strcpy( (char *) pwd.arr, _DBPASSWORD );
    pwd.len = strlen((char *) pwd.arr);
    strcpy( (char *) dbName.arr, _DBNAME );
    dbName.len = strlen((char *) dbName.arr);

    ctxConn = ctxParm;	

    
	/*
		EXEC SQL CONNECT :uid IDENTIFIED BY :pwd USING :dbName;
	 */
{
    struct esql_sqlctx  __sqlctx;
    memset(&__sqlctx, 0, sizeof(struct esql_sqlctx));
    __sqlctx.stmt_type   = ESQL_TYPE_CONNECT;
    __sqlctx.db_name     = "";
    __sqlctx.cursor_name = "";
    __sqlctx.pstmt_name  = "";
    __sqlctx.savepoint   = "";
    __sqlctx.stmt        = "";
    __sqlctx.char_map    = CHAR_MAP_CHARZ;
    __sqlctx.stmt_cache_size = 10;
    __sqlctx.fetch_type  = 0;
    __sqlctx.abs_rel_nth = 0;
    __sqlctx.flag        = 1245184;
    __sqlctx.option      = 268435552;
    __sqlctx.sqlca       = (struct sqlca *)&sqlca;
    __sqlctx.oraca       = (void *)0;
    __sqlctx.sqlda       = (void *)0;
    __sqlctx.desc_name   = "";
    __sqlctx.desc_into   = "";
    __sqlctx.value       = 0;
    __sqlctx.sql_ctx     = &ctxConn;
    __sqlctx.ep_ctx      = (void *)0;
    __sqlctx.sqlstate    = (char *)0;
    __sqlctx.file_id     = 112;
    __sqlctx.prefetch    = 1;
    __sqlctx.rmid        = -1;
    __sqlctx.iter_cnt           = 1;
    __sqlctx.host_var_total_cnt = 3;
    __sqlctx.host_var_out_cnt   = 0;
    __sqlctx.hostvar[0]         = (void *)(&(uid));
    __sqlctx.hostvar_len[0]     = (unsigned int)sizeof(uid.arr);
    __sqlctx.hostvar_stride[0]  = (unsigned int)0;
    __sqlctx.hostvar_type[0]    = ESQL_TYPE_VARCHAR;
    __sqlctx.param_type[0]      = 0;
    __sqlctx.ansi_dyn_type[0]   = ESQL_DESC_ITEM_NONE;
    __sqlctx.hostind[0]         = (void *)0;
    __sqlctx.hostind_stride[0]  = (unsigned int)0;
    __sqlctx.hostvar[1]         = (void *)(&(pwd));
    __sqlctx.hostvar_len[1]     = (unsigned int)sizeof(pwd.arr);
    __sqlctx.hostvar_stride[1]  = (unsigned int)0;
    __sqlctx.hostvar_type[1]    = ESQL_TYPE_VARCHAR;
    __sqlctx.param_type[1]      = 0;
    __sqlctx.ansi_dyn_type[1]   = ESQL_DESC_ITEM_NONE;
    __sqlctx.hostind[1]         = (void *)0;
    __sqlctx.hostind_stride[1]  = (unsigned int)0;
    __sqlctx.hostvar[2]         = (void *)(&(dbName));
    __sqlctx.hostvar_len[2]     = (unsigned int)sizeof(dbName.arr);
    __sqlctx.hostvar_stride[2]  = (unsigned int)0;
    __sqlctx.hostvar_type[2]    = ESQL_TYPE_VARCHAR;
    __sqlctx.param_type[2]      = 0;
    __sqlctx.ansi_dyn_type[2]   = ESQL_DESC_ITEM_NONE;
    __sqlctx.hostind[2]         = (void *)0;
    __sqlctx.hostind_stride[2]  = (unsigned int)0;
    __sqlctx.p_hostvar        = (void **)&__sqlctx.hostvar[0];
    __sqlctx.p_hostvar_len    = (unsigned int *)&__sqlctx.hostvar_len[0];
    __sqlctx.p_hostvar_stride = (unsigned int *)&__sqlctx.hostvar_stride[0];
    __sqlctx.p_hostvar_type   = (esql_hostvar_type_t *)&__sqlctx.hostvar_type[0];
    __sqlctx.p_param_type     = (unsigned int *)&__sqlctx.param_type[0];
    __sqlctx.p_ansi_dyn_type  = (esql_desc_item_t *)&__sqlctx.ansi_dyn_type[0];
    __sqlctx.p_hostind        = (void **)&__sqlctx.hostind[0];
    __sqlctx.p_hostind_stride = (unsigned int *)&__sqlctx.hostind_stride[0];
    __sqlctx.p_ind            = (long *)&__sqlctx.ind[0];
      sqlstm.stmt        = __sqlctx.stmt;
    esql_do(&__sqlctx);
}

    if( SQLCODE != 0 ) {
        
	/*
		EXEC SQL COMMIT WORK RELEASE;
	 */
{
    struct esql_sqlctx  __sqlctx;
    memset(&__sqlctx, 0, sizeof(struct esql_sqlctx));
    __sqlctx.stmt_type   = ESQL_TYPE_COMMIT_RELEASE;
    __sqlctx.db_name     = "";
    __sqlctx.cursor_name = "";
    __sqlctx.pstmt_name  = "";
    __sqlctx.savepoint   = "";
    __sqlctx.stmt        = "";
    __sqlctx.char_map    = CHAR_MAP_CHARZ;
    __sqlctx.stmt_cache_size = 10;
    __sqlctx.fetch_type  = 0;
    __sqlctx.abs_rel_nth = 0;
    __sqlctx.flag        = 0;
    __sqlctx.option      = 268435552;
    __sqlctx.sqlca       = (struct sqlca *)&sqlca;
    __sqlctx.oraca       = (void *)0;
    __sqlctx.sqlda       = (void *)0;
    __sqlctx.desc_name   = "";
    __sqlctx.desc_into   = "";
    __sqlctx.value       = 0;
    __sqlctx.sql_ctx     = &ctxConn;
    __sqlctx.ep_ctx      = (void *)0;
    __sqlctx.sqlstate    = (char *)0;
    __sqlctx.file_id     = 112;
    __sqlctx.prefetch    = 1;
    __sqlctx.rmid        = -1;
      sqlstm.stmt        = __sqlctx.stmt;
    esql_do(&__sqlctx);
}
        
        
	/*
		EXEC SQL CONNECT :uid IDENTIFIED BY :pwd USING :dbName;
	 */
{
    struct esql_sqlctx  __sqlctx;
    memset(&__sqlctx, 0, sizeof(struct esql_sqlctx));
    __sqlctx.stmt_type   = ESQL_TYPE_CONNECT;
    __sqlctx.db_name     = "";
    __sqlctx.cursor_name = "";
    __sqlctx.pstmt_name  = "";
    __sqlctx.savepoint   = "";
    __sqlctx.stmt        = "";
    __sqlctx.char_map    = CHAR_MAP_CHARZ;
    __sqlctx.stmt_cache_size = 10;
    __sqlctx.fetch_type  = 0;
    __sqlctx.abs_rel_nth = 0;
    __sqlctx.flag        = 1245184;
    __sqlctx.option      = 268435552;
    __sqlctx.sqlca       = (struct sqlca *)&sqlca;
    __sqlctx.oraca       = (void *)0;
    __sqlctx.sqlda       = (void *)0;
    __sqlctx.desc_name   = "";
    __sqlctx.desc_into   = "";
    __sqlctx.value       = 0;
    __sqlctx.sql_ctx     = &ctxConn;
    __sqlctx.ep_ctx      = (void *)0;
    __sqlctx.sqlstate    = (char *)0;
    __sqlctx.file_id     = 112;
    __sqlctx.prefetch    = 1;
    __sqlctx.rmid        = -1;
    __sqlctx.iter_cnt           = 1;
    __sqlctx.host_var_total_cnt = 3;
    __sqlctx.host_var_out_cnt   = 0;
    __sqlctx.hostvar[0]         = (void *)(&(uid));
    __sqlctx.hostvar_len[0]     = (unsigned int)sizeof(uid.arr);
    __sqlctx.hostvar_stride[0]  = (unsigned int)0;
    __sqlctx.hostvar_type[0]    = ESQL_TYPE_VARCHAR;
    __sqlctx.param_type[0]      = 0;
    __sqlctx.ansi_dyn_type[0]   = ESQL_DESC_ITEM_NONE;
    __sqlctx.hostind[0]         = (void *)0;
    __sqlctx.hostind_stride[0]  = (unsigned int)0;
    __sqlctx.hostvar[1]         = (void *)(&(pwd));
    __sqlctx.hostvar_len[1]     = (unsigned int)sizeof(pwd.arr);
    __sqlctx.hostvar_stride[1]  = (unsigned int)0;
    __sqlctx.hostvar_type[1]    = ESQL_TYPE_VARCHAR;
    __sqlctx.param_type[1]      = 0;
    __sqlctx.ansi_dyn_type[1]   = ESQL_DESC_ITEM_NONE;
    __sqlctx.hostind[1]         = (void *)0;
    __sqlctx.hostind_stride[1]  = (unsigned int)0;
    __sqlctx.hostvar[2]         = (void *)(&(dbName));
    __sqlctx.hostvar_len[2]     = (unsigned int)sizeof(dbName.arr);
    __sqlctx.hostvar_stride[2]  = (unsigned int)0;
    __sqlctx.hostvar_type[2]    = ESQL_TYPE_VARCHAR;
    __sqlctx.param_type[2]      = 0;
    __sqlctx.ansi_dyn_type[2]   = ESQL_DESC_ITEM_NONE;
    __sqlctx.hostind[2]         = (void *)0;
    __sqlctx.hostind_stride[2]  = (unsigned int)0;
    __sqlctx.p_hostvar        = (void **)&__sqlctx.hostvar[0];
    __sqlctx.p_hostvar_len    = (unsigned int *)&__sqlctx.hostvar_len[0];
    __sqlctx.p_hostvar_stride = (unsigned int *)&__sqlctx.hostvar_stride[0];
    __sqlctx.p_hostvar_type   = (esql_hostvar_type_t *)&__sqlctx.hostvar_type[0];
    __sqlctx.p_param_type     = (unsigned int *)&__sqlctx.param_type[0];
    __sqlctx.p_ansi_dyn_type  = (esql_desc_item_t *)&__sqlctx.ansi_dyn_type[0];
    __sqlctx.p_hostind        = (void **)&__sqlctx.hostind[0];
    __sqlctx.p_hostind_stride = (unsigned int *)&__sqlctx.hostind_stride[0];
    __sqlctx.p_ind            = (long *)&__sqlctx.ind[0];
      sqlstm.stmt        = __sqlctx.stmt;
    esql_do(&__sqlctx);
}
    }
    
    if( SQLCODE != 0 ) {    
		sprintf( msg, "DataBase(%s) Connect Error. %d",
				 _DBNAME, SQLCODE );
//		printf( "%s : %s\n\n", msg, SQLMESSAGE );		

        return -1;
    }
	
	sprintf( msg, "DB Connect : %s %s ???", _DBNAME, _DBUSER );
//	WriteLog( 0, ss );

	return 0;
}


void  DisconnectDatabase( sql_context ctxParm )
{

	/*
		EXEC SQL    BEGIN DECLARE SECTION;
	 */
	 sql_context ctxConn;

	/*
		EXEC SQL    END DECLARE SECTION;
	 */
	struct sqlca	sqlca;
   
   ctxConn = ctxParm;
   
	
	/*
		EXEC SQL COMMIT WORK RELEASE;
	 */
{
    struct esql_sqlctx  __sqlctx;
    memset(&__sqlctx, 0, sizeof(struct esql_sqlctx));
    __sqlctx.stmt_type   = ESQL_TYPE_COMMIT_RELEASE;
    __sqlctx.db_name     = "";
    __sqlctx.cursor_name = "";
    __sqlctx.pstmt_name  = "";
    __sqlctx.savepoint   = "";
    __sqlctx.stmt        = "";
    __sqlctx.char_map    = CHAR_MAP_CHARZ;
    __sqlctx.stmt_cache_size = 10;
    __sqlctx.fetch_type  = 0;
    __sqlctx.abs_rel_nth = 0;
    __sqlctx.flag        = 0;
    __sqlctx.option      = 268435552;
    __sqlctx.sqlca       = (struct sqlca *)&sqlca;
    __sqlctx.oraca       = (void *)0;
    __sqlctx.sqlda       = (void *)0;
    __sqlctx.desc_name   = "";
    __sqlctx.desc_into   = "";
    __sqlctx.value       = 0;
    __sqlctx.sql_ctx     = &ctxConn;
    __sqlctx.ep_ctx      = (void *)0;
    __sqlctx.sqlstate    = (char *)0;
    __sqlctx.file_id     = 112;
    __sqlctx.prefetch    = 1;
    __sqlctx.rmid        = -1;
      sqlstm.stmt        = __sqlctx.stmt;
    esql_do(&__sqlctx);
}

}
