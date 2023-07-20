#ifndef     _INC_MRQRYRUN
#define     _INC_MRQRYRUN

#include  "MRDTLIST.h"
#include  "BRMObj.h"

class   CMRQryRun 
{
private:
//	char			m_DBName[20];
//	char			m_DBPName[20];	
//	char			m_DBUser[20];
//	char			m_DBPwd[20];

	void			initialize();
	DATABASE_LIST * getDataBaseInfo( char * );

public:
	CMRQryRun ();
	~CMRQryRun();
    
	void		DbAgentStatus( CBRMObj *pObjParm );
//	void		DiagnoseDbAgent();

	RESULT_REC* ExecuteDynamicSqlProc( CMRDataList *, RULE_LIST *, QRULE_DATA *pQRule, char *sVal[] );
	RESULT_REC* ExecuteDynamicSqlCUDProc( CMRDataList *, RULE_LIST *, QRULE_DATA *pQRule, char *sVal[] );
 
};

 

#endif

