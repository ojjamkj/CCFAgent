#ifndef     _INC_MRTOKEN
#define     _INC_MRTOKEN

/*   TOKEN TYPE : OPERAND      */
#define TOKENTYPE_NONE          0

#define TOKENTYPE_CONST_F       1
#define TOKENTYPE_CONST_C       2
#define TOKENTYPE_CONST_N       3
#define TOKENTYPE_CONST_L       4   //  Long String
#define TOKENTYPE_CONST_B       5   //  Boolean
#define TOKENTYPE_CONST_V       6   //  Void
//#define TOKENTYPE_CONST_D       7   //  Date

#define TOKENTYPE_GLOBAL        8
#define TOKENTYPE_LOCAL         9
#define TOKENTYPE_RULE         10

#define TOKENTYPE_COLON        11
#define TOKENTYPE_COMMA        12
#define TOKENTYPE_CHOICE       13

#define TOKENTYPE_INDEX0       14
#define TOKENTYPE_INDEX1       15
#define TOKENTYPE_INDEX2       16
#define TOKENTYPE_INDEX3       17

#define TOKENTYPE_BREAK        20
#define TOKENTYPE_CONTINUE     21
#define TOKENTYPE_EXIT         22

/*   TOKEN TYPE : Pharenthesys   */
#define TOKENTYPE_LEFTP        25
#define TOKENTYPE_RIGHTP       26

/*   TOKEN TYPE : OPERATOR     */
#define TOKENTYPE_POW          31

#define TOKENTYPE_MULTIPLY     41
#define TOKENTYPE_DIVIDE       42
#define TOKENTYPE_MODULUS      43
#define TOKENTYPE_PLUS         51
#define TOKENTYPE_MINUS        52

#define TOKENTYPE_EQUAL        61
#define TOKENTYPE_LESS         62
#define TOKENTYPE_GREATER      63
#define TOKENTYPE_LESSEQ       64
#define TOKENTYPE_GREATEREQ    65
#define TOKENTYPE_NOTEQ        66


//  From === Function으로 옮겨가야...
#define TOKENTYPE_IN           71
#define TOKENTYPE_NIN          72
#define TOKENTYPE_ARREQUAL     73
#define TOKENTYPE_SUBSET       74
#define TOKENTYPE_BETWEEN      75
#define TOKENTYPE_MATCH        76
//  To


#define TOKENTYPE_NOT          80

#define TOKENTYPE_AND          91  //  81
#define TOKENTYPE_OR           92  //  91


/*******************************/
/*   Functions                 */
/*******************************/
#define TOKENFUNC_END         100

#define TOKENFUNC_SUBSTR      101 

#define TOKENFUNC_AGE         110 
#define TOKENFUNC_TODAY       111
#define TOKENFUNC_MONTH       112
#define TOKENFUNC_RES_AGE     113 
#define TOKENFUNC_DURATION    114
#define TOKENFUNC_DATEPLUSMON 115 
#define TOKENFUNC_DATE        116

#define TOKENFUNC_EXPRESS     120
#define TOKENFUNC_LIST        121

#define TOKENFUNC_ROUND       130
#define TOKENFUNC_TONUMBER    131
#define TOKENFUNC_TODOUBLE    132
#define TOKENFUNC_TRUNC       133
#define TOKENFUNC_ISNUM       134
#define TOKENFUNC_ISALPHA     135
#define TOKENFUNC_TOSTRING    136

#define TOKENFUNC_ROWS        140
#define TOKENFUNC_SUM         141
#define TOKENFUNC_LEN         142
#define TOKENFUNC_MAX         143
#define TOKENFUNC_MIN         144
//#define TOKENFUNC_STRCOMP     145

#define TOKENFUNC_HD_SPC1     150
#define TOKENFUNC_HD_SPC2     151
#define TOKENFUNC_HD_SPC3     152
#define TOKENFUNC_HD_SPC4     153
#define TOKENFUNC_HD_SPC5     154

#define TOKENFUNC_SETVAL      160
#define TOKENFUNC_GETSVAL     161
#define TOKENFUNC_GETNVAL     162
#define TOKENFUNC_INCVAL      163
#define TOKENFUNC_ADDVAL      164
#define TOKENFUNC_PUSHVAL     165
#define TOKENFUNC_CLRVAL      166
#define TOKENFUNC_FINDVAL     167

#define TOKENFUNC_RCLEAR      170
#define TOKENFUNC_EXEC        171
#define TOKENFUNC_DISPLAY     172
#define TOKENFUNC_EXTERN      173

#define TOKENFUNC_INDEXOF     200

#define TOKEN_DUMMY	          255
/*******************************/

#include    "MRSTRUCT.h"

#if ! defined(bool)
	#define	bool unsigned char
	#define	true 1
	#define false 0
#endif

class CMRToken  
{
public:
	CMRToken( char * );
	virtual ~CMRToken();

	long         GetECode( void ) const;
	CRULE_STMT * getToken();
	char *		 getParenExpression();

private:
	unsigned char tokType;
	int           ruleId;
	char          tokInfo[CONDITION_STR_SIZE];
	unsigned char resIndex;

	char         *pos;
	char          token[CONDITION_STR_SIZE];

	unsigned char prev_tokType;

#ifdef _DEBUG
	char MtsDebugString[CONDITION_STR_SIZE];
#endif

protected:
    // data members
    long          m_lCode;                // exception code
    char*         m_lpszMessage;          // exception message

private:
	void            getNextToken();
	bool			check_Unary();
    void            getToken_Item();
    void			getToken_Local();
    void            getToken_Rule();
    void            getToken_String();
    void            getToken_Number();
	void			getToken_ALPHA();
	void			getToken_ALPHA_A();
	void			getToken_ALPHA_B();
	void			getToken_ALPHA_C();
	void			getToken_ALPHA_D();
	void			getToken_ALPHA_E();
	void			getToken_ALPHA_F();
	void			getToken_ALPHA_G();
	void			getToken_ALPHA_I();
	void			getToken_ALPHA_L();
	void			getToken_ALPHA_M();
	void			getToken_ALPHA_N();
	void			getToken_ALPHA_O();
	void			getToken_ALPHA_P();
	void			getToken_ALPHA_R();
	void			getToken_ALPHA_S();
	void			getToken_ALPHA_T();

	void			checkFuncBegin();
	long			parseRuleInfo( char**, char* );
};

#endif
