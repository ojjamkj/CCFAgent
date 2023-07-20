#ifndef     _INC_MRPARAM
#define     _INC_MRPARAM

#include   "MRSTRUCT.h"

#if defined(_AIX)
	#define	bool unsigned char
	#define	true 1
	#define	false 0
#endif

// general exception class of EILibrary
class   CMRParam
{
public:
	TYPE_VALUE		*m_pParmVal;
	int				 m_ParmCnt;

public:
    // constructors and destructor
	CMRParam( TYPE_VALUE *pParmVal, int );
    virtual ~CMRParam();

    // operations
	bool    getParmValue( int, TYPE_VALUE *tVal );

#ifdef _TESTER
	void    DbgParmInfo();
#endif
};

#endif
