// R2CValue.h: interface for the CR2CValue class.
// UniCode not supported
//////////////////////////////////////////////////////////////////////

#ifndef	__INC_MRCALC
#define	__INC_MRCALC

unsigned long	CALC_NOT( OPERAND_STACK *s1 );

unsigned long	CALC_EQUAL( OPERAND_STACK *s1, OPERAND_STACK *s2 );
unsigned long	CALC_NEQUAL( OPERAND_STACK *s1, OPERAND_STACK *s2 );
unsigned long	CALC_LESS( OPERAND_STACK *s1, OPERAND_STACK *s2 );
unsigned long	CALC_GREATER( OPERAND_STACK *s1, OPERAND_STACK *s2 );
unsigned long	CALC_GREATEREQ( OPERAND_STACK *s1, OPERAND_STACK *s2 );
unsigned long	CALC_LESSEQ( OPERAND_STACK *s1, OPERAND_STACK *s2 );
unsigned long   CALC_PLUS( OPERAND_STACK *val1, OPERAND_STACK *val2, char * );
unsigned long   CALC_MINUS( OPERAND_STACK *val1, OPERAND_STACK *val2 );
unsigned long   CALC_MULTI( OPERAND_STACK *val1, OPERAND_STACK *val2 );
unsigned long   CALC_DIVIDE( OPERAND_STACK *val1, OPERAND_STACK *val2 );
unsigned long   CALC_MODULUS( OPERAND_STACK *val1, OPERAND_STACK *val2 );
unsigned long   CALC_POW( OPERAND_STACK *val1, OPERAND_STACK *val2 );
unsigned long   CALC_AND( OPERAND_STACK *val1, OPERAND_STACK *val2 );
unsigned long   CALC_OR( OPERAND_STACK *val1, OPERAND_STACK *val2 );

#endif

