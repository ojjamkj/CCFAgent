#ifndef     _INC_MRPARSE
#define     _INC_MRPARSE

#include  "MRSTRUCT.h"

class   CMRParser
{
private:
	char          m_funcLeft;
    long          m_lCode;

#ifdef _DEBUG
	char MtsDebugString[CONDITION_STR_SIZE];
#endif

public:
    // constructors and destructor
    CMRParser();
    CRULE_STMT * ParseExpression( char *StmtStr );
    CRULE_STMT * ParseResult( char *StmtStr );
    char *       getExpression( char *StmtStr );

	// initialize code and message
    virtual ~CMRParser();

private:
    void    Delete( CRULE_STMT *p );

};

#endif
