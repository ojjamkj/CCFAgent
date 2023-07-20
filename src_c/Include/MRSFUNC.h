#ifndef     _INC_MRSFUNC
#define     _INC_MRSFUNC

#include   "MRSTRUCT.h"

unsigned long	FUNC_SUBSTR( OPERAND_STACK *val1, OPERAND_STACK *val2, OPERAND_STACK *val3, char * );
unsigned long	FUNC_TODAY( TYPE_VALUE *Value, char* );
unsigned long   FUNC_AGE( OPERAND_STACK *val1, OPERAND_STACK *val2 );
unsigned long	FUNC_RESAGE_BYDATE( OPERAND_STACK *val1, OPERAND_STACK *val2, short );
//unsigned long	FUNC_RESAGE_BYDATE2( OPERAND_STACK *val1, OPERAND_STACK *val2 );
unsigned long	FUNC_RES_AGE( OPERAND_STACK *val1 );
unsigned long	FUNC_DURATION( OPERAND_STACK *val1, OPERAND_STACK *val2 );
unsigned long	FUNC_DATE( OPERAND_STACK *val1, OPERAND_STACK *val2 );

unsigned long   FUNC_MONTHS( OPERAND_STACK *val1, OPERAND_STACK *val2, short );
unsigned long	FUNC_DATEPLUSMON( OPERAND_STACK *val1, OPERAND_STACK *val2, OPERAND_STACK *val3 );
unsigned long	FUNC_ROUND( OPERAND_STACK *val1, OPERAND_STACK *val2 );
unsigned long	FUNC_TRUNC( OPERAND_STACK *val1, OPERAND_STACK *val2 );
unsigned long	FUNC_TONUMBER( OPERAND_STACK *val1 );
unsigned long	FUNC_TODOUBLE( OPERAND_STACK *val1 );
unsigned long	FUNC_TOSTRING( OPERAND_STACK *val1 );

unsigned long	FUNC_STR_TYPE( short gbn, OPERAND_STACK *val1 );

#endif
