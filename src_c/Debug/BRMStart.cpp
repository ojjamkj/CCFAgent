#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#include    "MTObject.h"
//#include    "MTParm.h"

#include	"MRPGMEXP.h"
#include    "MRERROR.h"
#include    "MRCSComm.h"
#include	"MTUtil.h"

#include	"BRMFile.h"
#include	"BRMStart.h"
#include	"MRTOKEN.h"
#include	"MRRunner.h"
#include    "MRGBLVAR.h"

extern  int			g_dbg_step;
extern  int			g_dbg_n1;
extern  int			g_dbg_n2;
extern  char		*g_dbg_buf;


BRDebugger::BRDebugger( char mode, int ruleId, char *appDate ) : CBREXThreadrun(), CBREXThread( 0 )
{
	g_dbg_step = 0;

	m_startRuleId = ruleId;
	strncpy( m_appDate, appDate, 8 );	m_appDate[8] = 0;
	m_mode = mode;

	pCMRGblVars = new CMRGblVars();
	pCMRGblVars->SetBufferSize( 100 );

	pCMRLoadData = new CMRLoadData();

	pCMRDataList = new CMRDataList( 0, 0, _MAX_BUFSIZE1, _MAX_BUFSIZE2, _MAX_BUFSIZE3 );

	pCMRDataList->Initialize( m_startRuleId );

		//  적용일자 Setting
	pCMRDataList->setAppDate( m_appDate );

	if( Start( this ) ) {
		g_dbg_step = _DBG_STEP_ERR;
		sprintf( g_dbg_buf, "Thread start failure." );
	}

	//	Sleep 0.5sec for SYNC
//	_sleep( 500 );
	Sleep( 500 );
}


BRDebugger::~BRDebugger()
{
//printf("[SO] ~BRDebugger begin.\n");

	delete pCMRDataList;
	delete pCMRLoadData;
	delete pCMRGblVars;

//printf("[SO] ~BRDebugger end.\n");
}


/*********************************************************/

void    BRDebugger::SuspendWork( int Step, int N1, int N2, char *buf )
{
	g_dbg_step = Step;
	g_dbg_n1 = N1;
	g_dbg_n2 = N2;
	strcpy( g_dbg_buf, buf );

	Suspend();
}

int    BRDebugger::NextStep()
{
	g_dbg_step = 0;
	g_dbg_buf[0] = 0;

	Resume();

	while ( ! g_dbg_step ) {

//		_sleep(10);        // 0.01 Second
		Sleep(10);        // 0.01 Second
	}

	return 0;
}

void    BRDebugger::Run( void )
{
	TYPE_VALUE *pParmVal = 0;
	int			parmCnt;
	char		sKey[200];

	try
	{
		Suspend();

		CMRParam *pNewParm=0;

		if ( m_mode == 'R' ) {
			pParmVal = readRuleParmValues( &parmCnt, sKey );

			if ( pParmVal ) {
				CMRParam NewParm( pParmVal, parmCnt );
				pNewParm = & NewParm;
			}

#ifdef _DEBUG
printf("Start(%d) parm(%s)\n", m_startRuleId, sKey );
#endif

		} else {
			pCMRDataList->setItemValues();
		}

		CMRRunner Runner( m_startRuleId, sKey, pCMRDataList, pNewParm );

	} catch( CMTPgmException *e ) {
		g_dbg_step = -1;
		sprintf(g_dbg_buf, "[%d] %s", e->GetECode(), (char *) e->GetEMessage() );
		e->Delete();
		return;
	} catch( CMTException *e ) {
		g_dbg_step = -1;
		sprintf(g_dbg_buf, "[%d] %s", e->GetECode(), (char *) e->GetEMessage() );
		e->Delete();
		return;
	} catch( ... ) {
		g_dbg_step = -1;
		sprintf(g_dbg_buf, "Exception occurs (Unknown)" );
		return;
	}

	g_dbg_step = _DBG_STEP_TERM;
}

	
TYPE_VALUE *	BRDebugger::readRuleParmValues(int *parmCnt, char *sKey )
{
	TYPE_VALUE *pParmVal=0;

	RULE_LIST	*pCalledRule;
	char		*pKey;
	int			len;

	sKey[0] = 0;
	pKey = sKey;

	if ( ! (pCalledRule = pCMRDataList->getRuleNodeById( m_startRuleId )) ) {
		char msg[100];
		sprintf( msg, "Rule(%d) not found.", m_startRuleId );
		throw new CMTException( UERR_RULENFOUND, msg );
	}

	if ( ! pCalledRule->parmType ) return (TYPE_VALUE *) 0;

	*parmCnt = strlen(pCalledRule->parmType);

	if ( *parmCnt == 0 ) return 0;


	BRFRead Reader( _FILE_PARM );

	int readParmCnt = Reader.getNumber();
//printf("Parm  def(%d)  input(%d)\n", *parmCnt, readParmCnt);

	if ( readParmCnt != *parmCnt ) throw new CMTException( UERR_PARMCOUNT, EMSG_PARMCOUNT );


	pParmVal = (TYPE_VALUE *) pCMRDataList->MRMemAlloc1(sizeof(TYPE_VALUE) * readParmCnt);

	for ( int i=0; i<readParmCnt; i++ ) {
		if ( i > 0 ) {
			strcpy ( pKey, "||" );   pKey += 2;
		}

		strcpy( pKey, Reader.getString() );
		len = strlen(pKey);

		pParmVal[i].dataType = pCalledRule->parmType[i];
		switch( pCalledRule->parmType[i] ) {
			case TOKENTYPE_CONST_F :
				pParmVal[i].value.dblVal = atof( pKey );
				break;
			case TOKENTYPE_CONST_C :
				memset(pParmVal[i].value.strVal, 0x00,STR_VALUE_SIZE);
				strncpy( pParmVal[i].value.strVal, pKey, STR_VALUE_SIZE );
				pParmVal[i].value.strVal[STR_VALUE_SIZE] = 0;
				break;
			case TOKENTYPE_CONST_L :
				pParmVal[i].value.pLStr = (char *) (char*) malloc( len + 1 );
				strcpy( pParmVal[i].value.pLStr, pKey );
				break;
			case TOKENTYPE_CONST_N :
				pParmVal[i].value.longVal = atoi( pKey );
				break;
			case TOKENTYPE_CONST_B :
				pParmVal[i].value.boolVal = atoi( pKey );
		}

		pKey += len;
	}
	return pParmVal;
}
