#ifndef     _INC_MRSEARCH
#define     _INC_MRSEARCH

#include    "MRSTRUCT.h"

typedef struct tagNameSearch
{
	char		rule_name[101];
	RULE_LIST*	ptr;
} RNAME_SEARCH_REC;

class   CMRSearch
{
private:
	RNAME_SEARCH_REC *m_NameList;
	int					m_MaxSize;

	long					m_lCode;
	char					m_sMessage[200];

public:
	int					m_LastIdx;
    // constructors and destructor
    CMRSearch();

	void				SetBufferSize( int nSize );
	int				AddRuleNodeLast( char *Name, RULE_LIST* ptr );
	int				AddRuleNodeSearch( int Id, char *Name, RULE_LIST* ptr );
	RULE_LIST*		SearchByName( char *Name );
	int				DeleteNode( RULE_LIST *pNodeNew , char *Name );  
//	int				DeleteNode( int Idx, char *Name );
//	void				disp();

	// initialize code and message
    virtual ~CMRSearch();

private:

};

#endif
