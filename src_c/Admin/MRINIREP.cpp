#include	<stdio.h>
#include	<string.h>

#include    "MTMsgDef.h"
#include    "MRCSComm.h"

/*
 void    DispRuleConfigParm()
 {
 char msg[128];

 _WriteLog( _LOG_LOG,"RBMS Engine Configuration\n");

 sprintf(msg, "    Max rule load count       = %d",configParm->ulMaxLoading);
 _WriteLog( _LOG_LOG, msg );
 sprintf(msg, "    Global vars heap size     = %d",configParm->ulMaxMemSize1);
 _WriteLog( _LOG_LOG, msg );
 sprintf(msg, "    Calculate queue heap size = %d",configParm->ulMaxMemSize2);
 _WriteLog( _LOG_LOG, msg );
 sprintf(msg, "    Comparison heap size      = %d\n",configParm->ulMaxMemSize3);
 _WriteLog( _LOG_LOG, msg );
 sprintf(msg, "    Max query count           = %d\n",configParm->ulMaxQueryCount);
 _WriteLog( _LOG_LOG, msg );

 sprintf(msg, "    Debug level               = %d",MtsDebugLevel);
 _WriteLog( _LOG_LOG, msg );
 sprintf(msg, "    Load debug level          = %d\n",MtsLoadDebugLevel);
 _WriteLog( _LOG_LOG, msg );
 }
 */

#include    "MRGBLVAR.h"

void BuildDataStructure();

int InitialProc() {

	pCMRGblVars = new CMRGblVars();

	BuildDataStructure();

	return 0;
}

