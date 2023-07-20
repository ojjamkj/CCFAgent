#ifndef     _INC_MRERROR
#define     _INC_MRERROR

#include "MTSysMsg.h"

#ifdef _MSG_KO
#define     UERR_PGMNFOUND               ( USERERR_PARSE  +    1 )
#define     EMSG_PGMNFOUND               "메인 룰이 정의되지 않았습니다."

#define     UERR_STARTRULENOTRUN         ( USERERR_PARSE  +    2 )
#define     EMSG_STARTRULENOTRUN         "메인 룰이 실행되고 있지 않습니다."
#define     UERR_RULENFOUND              ( USERERR_PARSE  +    3 )
#define     EMSG_RULENFOUND              "룰이 정의되지 않았습니다."
#define     UERR_RULEINVALID             ( USERERR_PARSE  +    4 )
#define     EMSG_RULEINVALID             "룰이 유효하지 않습니다."

#define     UERR_RULELOADFAIL            ( USERERR_PARSE  +  101 )
#define     EMSG_RULELOADFAIL            "룰 로드 오류."

#define     UERR_ITEMNOTFOUND            ( USERERR_PARSE  + 1001 )
#define     EMSG_ITEMNOTFOUND            "아이템이 존재하지 않습니다 !"
#define     UERR_DATETYPEERR             ( USERERR_PARSE  + 1002 )
#define     EMSG_DATETYPEERR             "날짜 타입 오류 !"
#define     UERR_ITEMVALOUTOFINDEX       ( USERERR_PARSE  + 1003 )
#define     EMSG_ITEMVALOUTOFINDEX       "아이템 값 인덱스 범위 초과 !"
#define     UERR_ITEMVALNOTPASSED        ( USERERR_PARSE  + 1004 )
#define     EMSG_ITEMVALNOTPASSED        "Item value not passed !"
#define     UERR_MAXITEMCOUNT            ( USERERR_PARSE  + 1005 )
#define     EMSG_MAXITEMCOUNT            "아이템 개수가 너무 많습니다 !"

#define     UERR_UNMATCH_PARMTP          ( USERERR_PARSE  + 1006 )
#define     EMSG_UNMATCH_PARMTP          "매치되지 않는 파라미터 타입 !"

#define     UERR_NORESULT                ( USERERR_PARSE  + 1010 )
#define     EMSG_NORESULT                "결과값이 존재하지 않습니다 !"
#define     UERR_NORESULTERR             ( USERERR_PARSE  + 1011 )
#define     EMSG_NORESULTERR             "룰에 의해 계산된 결과값이 없습니다 !"
#define     UERR_CONDINVALID             ( USERERR_PARSE  + 1012 )
#define     EMSG_CONDINVALID             "조건 데이터의 형태가 boolean이 아닙니다 !"
#define     UERR_RESULTSTMTEMPTY         ( USERERR_PARSE  + 1013 )
#define     EMSG_RESULTSTMTEMPTY         "결과값 설명이 없습니다 !"
#define     UERR_RESULTOUTOFBIND         ( USERERR_PARSE  + 1014 )
#define     EMSG_RESULTOUTOFBIND         "결과값 인덱스 범위 초과!"
#define     UERR_RESULTCOUNT             ( USERERR_PARSE  + 1015 )
#define     EMSG_RESULTCOUNT             "반환값의 개수가 다릅니다 !"
#define     UERR_RESULTTYPES             ( USERERR_PARSE  + 1016 )
#define     EMSG_RESULTTYPES             "반환값의 형태가 다릅니다 !"

#define     UERR_PARMCOUNT				 ( USERERR_PARSE  + 1020 )
#define     EMSG_PARMCOUNT			     "룰 매개변수 개수 오류 !"
#define     UERR_PARMTYPE				 ( USERERR_PARSE  + 1021 )
#define     EMSG_PARMTYPE			     "룰 매개변수 형태 오류 !"
#define     UERR_PARMNFOUND				 ( USERERR_PARSE  + 1022 )
#define     EMSG_PARMNFOUND			     "룰 매개변수가 정의되지 않았습니다 !"
#define     UERR_PARMUSEEXP				 ( USERERR_PARSE  + 1023 )
#define     EMSG_PARMUSEEXP			     "룰 매개변수가 계산식을 사용할 수 없습니다 !"
#define     UERR_PARMNPASS				 ( USERERR_PARSE  + 1024 )
#define     EMSG_PARMNPASS			     "Rule parameter not passed !"

#define     UERR_GBLVAL_NOTSET			 ( USERERR_PARSE  + 1030 )
#define     EMSG_GBLVAL_NOTSET		     "GBL(%s) 값이 초기화되지 않았습니다 !"
#define     UERR_GBLVAL_TYPE			 ( USERERR_PARSE  + 1031 )
#define     EMSG_GBLVAL_TYPE		     "GBL 값 형태 오류 !"
#define     UERR_GBLVAL_INDEX			 ( USERERR_PARSE  + 1032 )
#define     EMSG_GBLVAL_INDEX		     "GBL 값 인덱스 범위 초과!"

#define     UERR_INDEX_NOTINIT			 ( USERERR_PARSE  + 1040 )
#define     EMSG_INDEX_NOTINIT		     "INDEX 가 초기화되지 않았습니다 !"

#define     UERR_DBNOTDEFINED            ( USERERR_PARSE  + 2001 )
#define     EMSG_DBNOTDEFINED            "DataBase가 정의되지 않았습니다 !"
#define     UERR_DBCONNECT               ( USERERR_PARSE  + 2002 )
#define     EMSG_DBCONNECT               "DataBase 연결 오류 !"
#define     UERR_DBPREPARE               ( USERERR_PARSE  + 2003 )
#define     EMSG_DBPREPARE               "DYNAMIC SQL Prepare 오류 !"
#define     UERR_DBCURSOROPEN            ( USERERR_PARSE  + 2004 )
#define     EMSG_DBCURSOROPEN            "커서 열기 오류 !"
#define     UERR_DBERROR                 ( USERERR_PARSE  + 2999 )
#define     EMSG_DBERROR                 "DATABASE 오류 !"

#define     UERR_ENV_FUNC                ( USERERR_PARSE  + 3001 )
#define     EMSG_ENV_FUNC                "Environment Error. (Function Spec)"

#define     UERR_RUN_TIMEOUT             ( USERERR_RUN  + 1001 )
#define     EMSG_RUN_TIMEOUT			"대기시간이 초과되었습니다."
#else

#define     UERR_PGMNFOUND               ( USERERR_PARSE  +    1 )
#define     EMSG_PGMNFOUND               "Main Rule Not Defined."

#define     UERR_STARTRULENOTRUN         ( USERERR_PARSE  +    2 )
#define     EMSG_STARTRULENOTRUN         "Main Rule Not Running."
#define     UERR_RULENFOUND              ( USERERR_PARSE  +    3 )
#define     EMSG_RULENFOUND              "Rule Not Defined."
#define     UERR_RULEINVALID             ( USERERR_PARSE  +    4 )
#define     EMSG_RULEINVALID             "Rule Invalid."

#define     UERR_RULELOADFAIL            ( USERERR_PARSE  +  101 )
#define     EMSG_RULELOADFAIL            "Rule body load error."

#define     UERR_ITEMNOTFOUND            ( USERERR_PARSE  + 1001 )
#define     EMSG_ITEMNOTFOUND            "Item not exist !"
#define     UERR_DATETYPEERR             ( USERERR_PARSE  + 1002 )
#define     EMSG_DATETYPEERR             "Date Type Error !"
#define     UERR_ITEMVALOUTOFINDEX       ( USERERR_PARSE  + 1003 )
#define     EMSG_ITEMVALOUTOFINDEX       "Item value out of index !"
#define     UERR_ITEMVALNOTPASSED        ( USERERR_PARSE  + 1004 )
#define     EMSG_ITEMVALNOTPASSED        "Item value not passed !"
#define     UERR_MAXITEMCOUNT            ( USERERR_PARSE  + 1005 )
#define     EMSG_MAXITEMCOUNT            "Item count too large !"

#define     UERR_UNMATCH_PARMTP          ( USERERR_PARSE  + 1006 )
#define     EMSG_UNMATCH_PARMTP          "Unmatching parameter type !"

#define     UERR_NORESULT                ( USERERR_PARSE  + 1010 )
#define     EMSG_NORESULT                "Result not exist !"
#define     UERR_NORESULTERR             ( USERERR_PARSE  + 1011 )
#define     EMSG_NORESULTERR             "Calculate by Rule that has no result !"
#define     UERR_CONDINVALID             ( USERERR_PARSE  + 1012 )
#define     EMSG_CONDINVALID             "Condition not boolean !"
#define     UERR_RESULTSTMTEMPTY         ( USERERR_PARSE  + 1013 )
#define     EMSG_RESULTSTMTEMPTY         "Result Statement Empty !"
#define     UERR_RESULTOUTOFBIND         ( USERERR_PARSE  + 1014 )
#define     EMSG_RESULTOUTOFBIND         "Result out of index !"
#define     UERR_RESULTCOUNT             ( USERERR_PARSE  + 1015 )
#define     EMSG_RESULTCOUNT             "Different return count !"
#define     UERR_RESULTTYPES             ( USERERR_PARSE  + 1016 )
#define     EMSG_RESULTTYPES             "Different return Types !"

#define     UERR_NOMATCH_VERSION             ( USERERR_PARSE  + 1017 )
#define     EMSG_NOMATCH_VERSION         "Version date of the error rule.!"
#define     UERR_PARMCOUNT				 ( USERERR_PARSE  + 1020 )
#define     EMSG_PARMCOUNT			     "Rule parameter count error !"
#define     UERR_PARMTYPE				 ( USERERR_PARSE  + 1021 )
#define     EMSG_PARMTYPE			     "Rule parameter type error !"
#define     UERR_PARMNFOUND				 ( USERERR_PARSE  + 1022 )
#define     EMSG_PARMNFOUND			     "Rule parameter not defined !"
#define     UERR_PARMUSEEXP				 ( USERERR_PARSE  + 1023 )
#define     EMSG_PARMUSEEXP			     "Rule parameter can not use expression !"
#define     UERR_PARMNPASS				 ( USERERR_PARSE  + 1024 )
#define     EMSG_PARMNPASS			     "Rule parameter not passed !"

#define     UERR_GBLVAL_NOTSET			 ( USERERR_PARSE  + 1030 )
#define     EMSG_GBLVAL_NOTSET		     "GBL(%s) value not initialized !"
#define     UERR_GBLVAL_TYPE			 ( USERERR_PARSE  + 1031 )
#define     EMSG_GBLVAL_TYPE_01		     "GBL value type error(01) !"
#define     EMSG_GBLVAL_TYPE_02		     "GBL value type error(02) !"
#define     EMSG_GBLVAL_TYPE_03		     "GBL value type error(03) !"
#define     EMSG_GBLVAL_TYPE_04		     "GBL value type error(04) !"
#define     EMSG_GBLVAL_TYPE_05		     "GBL value type error(05) !"
#define     EMSG_GBLVAL_TYPE_06		     "GBL value type error(06) !"
#define     EMSG_GBLVAL_TYPE_07		     "GBL value type error(07) !"
#define     EMSG_GBLVAL_TYPE_08		     "GBL value type error(08) !"
#define     EMSG_GBLVAL_TYPE_09		     "GBL value type error(09) !"
#define     EMSG_GBLVAL_TYPE_10		     "GBL value type error(1) !"
#define     UERR_GBLVAL_INDEX			 ( USERERR_PARSE  + 1032 )
#define     EMSG_GBLVAL_INDEX		     "GBL value out of index !"

#define     UERR_INDEX_NOTINIT			 ( USERERR_PARSE  + 1040 )
#define     EMSG_INDEX_NOTINIT		     "INDEX not initialize !"

#define     UERR_DBNOTDEFINED            ( USERERR_PARSE  + 2001 )
#define     EMSG_DBNOTDEFINED            "DataBase Not Defined !"
#define     UERR_DBCONNECT               ( USERERR_PARSE  + 2002 )
#define     EMSG_DBCONNECT               "DataBase Connection Error !"
#define     UERR_DBPREPARE               ( USERERR_PARSE  + 2003 )
#define     EMSG_DBPREPARE               "DYNAMIC SQL Prepare Error !"
#define     UERR_DBCURSOROPEN            ( USERERR_PARSE  + 2004 )
#define     EMSG_DBCURSOROPEN            "CURSOR OPEN Error !"
#define     UERR_DBERROR                 ( USERERR_PARSE  + 2999 )
#define     EMSG_DBERROR                 "DATABASE Error !"

#define     UERR_ENV_FUNC                ( USERERR_PARSE  + 3001 )
#define     EMSG_ENV_FUNC                "Environment Error. (Function Spec)"

#define     UERR_RUN_TIMEOUT             ( USERERR_RUN  + 1001 )
#define     EMSG_RUN_TIMEOUT			"Exceeded a waiting time."
#endif
#endif
