#ifndef     _INC_MRSTARES
#define     _INC_MRSTARES

#include  "MRSTRUCT.h"

class   CMRStaticResult
{
private: 
	RULE_RESULT	*m_pRuleResult;
	int			m_AllocSize;

public:
    // constructors and destructor
    CMRStaticResult();
    virtual ~CMRStaticResult();

    // operations
	RULE_RESULT *	getStaticResult();
	RULE_RESULT *	AddStaticResult( RULE_RESULT *pResultAdd );

	void			DelRuleResult( long id );
	void			DelRuleResultAll();

private:
	void			DeleteResRec( RESULT_REC *p, short cnt );

	void *		MetisAlloc( int sz );
	void			MetisFree( void *p );
	void			MetisFree( void *p, int sz );

};

extern  CMRStaticResult    g_CMRStaticResult;

#endif
