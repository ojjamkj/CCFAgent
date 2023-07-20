#ifndef     _INC_MRBRINTR
#define     _INC_MRBRINTR

#include   "MRDTLIST.h"
#include   "MRSTRUCT.h"
#include   "MRPARAM.h"

#if defined(_AIX)
	#define	bool unsigned char
	#define	true 1
	#define false 0
#endif

// general exception class of EILibrary
class   CMRBInterpreter
{
private:
	CMRDataList     *m_pDataList;
	RULE_LIST       *m_pRuleNode;
	RESULT_REC		*pResultList;
	CMRParam 	 	*m_LocalParm;

	short            m_retCnt;
	short            m_vIndex;

	int				 matchingReturnTypes( TYPE_VALUE *pTval, char tp );

protected:
    long    m_lCode;                // exception code
    char*   m_lpszMessage;          // exception message

public:
    // constructors and destructor
    CMRBInterpreter( CMRDataList *pDataList, RULE_LIST *pRuleNode, CMRParam* );
    virtual ~CMRBInterpreter();

	RESULT_REC * BuildResult();

    // attributes
    long            GetECode( void ) const;
    const char*     GetEMessage( void ) const;

    // operations
    void    Delete( void );
	void    GetGlobalItemValue( int ItemCd, char *tokInfo, char *itemValue );
	void	GetRuleValue( int RuleCd, char *pTokInfo, char *itemValue );
	void	GetParmValue( int pParmIdx, char *itemValue );
};

#endif
