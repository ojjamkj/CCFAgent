#ifndef     _INC_BRTRACE
#define     _INC_BRTRACE
#include    "BRMObj.h"

typedef struct {
	long			itemCode;
	char			strVal[STR_VALUE_SIZE+1];
} TRC_ITEM;

typedef struct _TRC_PGM TRC_PGM;
struct _TRC_PGM {
	char		TraceId[15];
	char		PgmId[21];
//	short		mode;     //  0:°è¼Ó
	short		itemCnt;
	TRC_ITEM    *pItem;
	TRC_PGM		*pNext;
};	


// general exception class of EILibrary
class   CBRTrace
{
private:
	TRC_PGM			*m_pTrcRoot;

public:
    // constructors and destructor
	CBRTrace();
    ~CBRTrace();

    // operations
//	void	buildTraceInfo( CMTParm *ObjBuffer );
	int		AddTraceInfo( char *dt, char *pgmId );
	void	DelTraceInfo( char *dt, char *pgmId );

	bool	isTrace( char *PgmId, void *, char *dt );
	void    setTraceLog(CBRMObj *pObjParm);
	
};

#endif
