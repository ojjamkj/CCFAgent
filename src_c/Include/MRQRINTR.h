#ifndef     _INC_MRQRINTR
#define     _INC_MRQRINTR

#include   "MRDTLIST.h"
#include   "MRSTRUCT.h"
#include   "MRPARAM.h"

// general exception class of EILibrary
class   CMRQInterpreter
{
private:
	CMRDataList       *m_pDataList;
	RULE_LIST       *m_pRuleNode;
	short            m_retCnt;
	CMRParam 	 	*m_LocalParm;

protected:
    long    m_lCode;                // exception code
    char*   m_lpszMessage;          // exception message

public:
    // constructors and destructor
    CMRQInterpreter( CMRDataList *pRoot, RULE_LIST *pRuleNode, CMRParam* );
    virtual ~CMRQInterpreter();

	RESULT_REC * BuildResult();

    // attributes
    long            GetECode( void ) const;
    const char*     GetEMessage( void ) const;

    // operations
    void    Delete( void );         // delete this exception object

private:
	void    GetGlobalItemValue( int ItemCd, char *tokInfo, char *itemValue );
	void    GetLocalItemValue( int ItemCd, char *tokInfo, char *itemValue );
	void    GetRuleValue( int RuleCd, char *tokInfo, char *RuleValue );
	void    convQueryString( char *org, char *rep );

//    void	RunChildRules( CRULE_STMT *pStmt );

};

#endif
