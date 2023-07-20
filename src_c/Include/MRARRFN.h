#ifndef     _INC_MRARRFN
#define     _INC_MRARRFN

#include   "MRSTRUCT.h"
#include   "MRDTLIST.h"

#if defined(_AIX)
	#define	bool unsigned char
	#define	true 1
	#define false 0
#endif

// general exception class of EILibrary
class   CMRArrayFn
{
private:
	CMRDataList		*m_pDataList;

	unsigned char	vType1;
	int				vCnt1;
	int				vGbn1;
	VALUE_UNION		*vPtr1;

	unsigned char	vType2;
	int				vCnt2;
	int				vGbn2;
	VALUE_UNION		*vPtr2;

	int				m_pMemPos;

public:
    // constructors and destructor
	CMRArrayFn(CMRDataList *pDataList);
    ~CMRArrayFn();

    // operations
	void	AddValue( int idx, TYPE_VALUE *tVal );
	void	AddItemList( int idx, int col );
	void	AddRuleList( int idx, RULE_RESULT *ruleResult, int col );
	bool	isInclude();
	bool	isSubset();
	bool    isEqualToEach();
	bool	isIncludeList( CRULE_STMT *pTop );
	bool	isEqualToList( CRULE_STMT *pTop );
	bool    isSubsetList( CRULE_STMT *pTop );
	bool    matchValues(int idx, VALUE_UNION *pVal);
	bool    isMatch();
	bool	isMatchList(CRULE_STMT *pTop);
private:

	void    Delete();
	bool	getListValue( CRULE_STMT *p, VALUE_UNION *pVal );
	bool	compareValues( int idx, VALUE_UNION *pVal );
	
};

#endif
