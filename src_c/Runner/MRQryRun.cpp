#include    <stdio.h>												 
#include    <stdlib.h>
#include    <time.h>

#ifndef	WIN32
#include    <strings.h>
#include		<unistd.h>
#endif


#ifdef __GNUC__
#include   <string.h>
	#include   <time.h>
	#include   <sys/time.h>
#endif
#include    "BREXLog.h"
#include		"MRERROR.h"
#include    "MRDTLIST.h"
#include    "MRAgtMgr.h"
#include    "MRQryRun.h"
#include    "MRConfig.h"
#include	"MTSqlCod.h"

#ifdef _MSG_KO
#define   MSG_NOTDEFINE_DB    "DataBase(%s)가 정의되지 않았습니다."
#define   MSG_NOTINIT_DBAGENT " %s를 위한 DataBase agent가 초기화되지 않았습니다."
#else
#define   MSG_NOTDEFINE_DB    "DataBase(%s) not defined."
#define   MSG_NOTINIT_DBAGENT "DataBase agent not initialized for %s."
#endif

extern DB_AGENT *m_DBAgent;

DATABASE_LIST *pDBList;
CMRQryRun *g_pCQryRun;

CMRQryRun::CMRQryRun() {
	initialize();
}

CMRQryRun::~CMRQryRun() {
	DATABASE_LIST *pDBPtr;
	CMRAgtMgr *pAgtMgr;

	pDBPtr = pDBList;
	while (pDBPtr) {
		if (pDBPtr->pAgtMgr) {
			pAgtMgr = (CMRAgtMgr*) pDBPtr->pAgtMgr;
			delete pAgtMgr;
		}

		pDBPtr = pDBPtr->pNext;
	}
}

void CMRQryRun::initialize() {
	DATABASE_LIST *pDBPtr;
	CMRAgtMgr *pAgtMgr;

	char msg[300];
//	char		cmd[200];

	_WriteLogNo(_LOG_LOG, "Database Agents.");
	_WriteLogNo(_LOG_LOG,
			"===================================================================");
	_WriteLogNo(_LOG_LOG,
			"    Logical       DBMS     Physical      User      Count");
	_WriteLogNo(_LOG_LOG,
			"===================================================================");

	pDBPtr = pDBList;
	while (pDBPtr) {
		pAgtMgr = new CMRAgtMgr(pDBPtr);

		if (pAgtMgr->getError(msg) != 0) {
			char dispMsg[300];

			sprintf(dispMsg, "       %s", msg);
			_WriteLogNo(_LOG_LOG, dispMsg);

			delete pAgtMgr;

			pDBPtr->pAgtMgr = 0;
		} else {
			pDBPtr->pAgtMgr = pAgtMgr;
		}

		pDBPtr = pDBPtr->pNext;
	}

	_WriteLogNo(_LOG_LOG,
			"===================================================================\n");
}

DATABASE_LIST * CMRQryRun::getDataBaseInfo(char *DBName) {
	DATABASE_LIST *pDBPtr;

	pDBPtr = pDBList;

	while (pDBPtr) {
		if (!strcmp(DBName, pDBPtr->DBName))
			break;
		pDBPtr = pDBPtr->pNext;

	}

	return pDBPtr;
}

void CMRQryRun::DbAgentStatus(CBRMObj *pObjParm) {
	DATABASE_LIST *pDBPtr;
	CMRAgtMgr *pCAgtMgr;
	int rows = 0, idx = 0;

	pDBPtr = pDBList;
	while (pDBPtr) {
		pCAgtMgr = (CMRAgtMgr *) pDBPtr->pAgtMgr;

		if (pCAgtMgr)
			rows += pDBPtr->agents; //  pCAgtMgr->getAgentCount();

		pDBPtr = pDBPtr->pNext;
	}

	pObjParm->WriteLong(rows);

	pDBPtr = pDBList;
	while (pDBPtr) {
		pCAgtMgr = (CMRAgtMgr *) pDBPtr->pAgtMgr;

		if (pCAgtMgr)
			pCAgtMgr->AgentStatus(pObjParm);

		pDBPtr = pDBPtr->pNext;
	}
}

/*
 void  CMRQryRun::DiagnoseDbAgent()
 {
 DATABASE_LIST	*pDBPtr;
 CMRAgtMgr		*pCAgtMgr;
 char msg[50];

 _WriteLog( _LOG_LOG, "DB Agent Check" );

 pDBPtr = pDBList;

 while ( pDBPtr ) {
 sprintf( msg, "   DATABASE [%s]", pDBPtr->DBName );
 _WriteLogNo( _LOG_LOG, msg );

 pCAgtMgr = (CMRAgtMgr *) pDBPtr->pAgtMgr;

 if ( pCAgtMgr )	pCAgtMgr->AgentCheck();

 pDBPtr = pDBPtr->pNext;
 }
 }


 void	DiagnoseDbAgent()
 {
 g_pCQryRun->DiagnoseDbAgent();
 }
 */

RESULT_REC* CMRQryRun::ExecuteDynamicSqlProc(CMRDataList *pDTList,
		RULE_LIST *pRuleNode, QRULE_DATA *pQRule, char *sVal[]) {
	int rc;
	char msg[CONDITION_STR_SIZE];

	RESULT_REC *pResultList = 0, *pLastResult, *pNewResult;
	char resString[MAX_QUERY_SIZE];  //MItem.h 30000 MAX_QUERY_SIZE 
	int i;
    int  maxQueryCount=0;
    int  valSize=0;
	DATABASE_LIST *pDBPtr;
	CMRAgtMgr *pAgtMgr;
	int IdxAgt;

	pDBPtr = getDataBaseInfo(pQRule->DBName);
	if (!pDBPtr) {
		//sprintf(msg, "DataBase(%s) not defined.", pQRule->DBName);
		sprintf(msg, MSG_NOTDEFINE_DB, pQRule->DBName);
		throw new CMTException(UERR_DBERROR, msg);
	}

	pAgtMgr = (CMRAgtMgr *) pDBPtr->pAgtMgr;

	if (!pAgtMgr) {
		//sprintf(msg, "DataBase agent not initialized for %s.", pQRule->DBName);
		sprintf(msg, MSG_NOTINIT_DBAGENT, pQRule->DBName);
		throw new CMTException(UERR_DBERROR, msg);
	}

//	if ( pDBPtr->dbms != 3 ) {  //  Sybase아닐때

	if ((IdxAgt = pAgtMgr->getAgent(pRuleNode->rule_id)) < 0) {
		pAgtMgr->getError(msg);

		throw new CMTException(-1, msg);
	}
//	} else {
//		IdxAgt = pDTList->m_ulThreadNo;
//	}

//  지워야함
//	_WriteLogNo( _LOG_LOG, QS );

//		printf("RuleId(%d) : %s  Agent(%d)\n", pRuleNode->rule_id, pDBPtr->DBName, IdxAgt );

	if ((rc = pAgtMgr->CursorOpen(IdxAgt, pQRule, sVal, msg)) != 0) {
		char buf[_MSG_LENG + 1];
		char *p = pQRule->QueryStatement;
		int L1 = strlen(p);
		int L2 = 0;

		_WriteLog(_LOG_ERR, msg);

		while (L2 < L1) {
			strncpy(buf, p + L2, _MSG_LENG);
			buf[_MSG_LENG] = 0;
			_WriteLogNo(_LOG_ERR, buf);
			L2 += _MSG_LENG;
		}
		_WriteLogNo(_LOG_ERR, "");
		/*2019.03.06 산업은행 에서 Query Rule 결과 값으로 Insert 하는경우
		  해당 결과가 Array로 반환되고 해당 Array 를 insert 하려고 하는 경우
		  오류가 발생되어야 하나 그전에 메모리 영역 침범하여 Down 발생
		   */
		for (i = 0; i < pQRule->nBindVal; i++) {
			valSize=strlen(sVal[i]);
			if( valSize < _MSG_LENG-16 ){
				sprintf(buf, "Bind var[%2d] : %s", i + 1, sVal[i]);
			}else{
				sprintf(buf, "Bind var[%2d] : ", i + 1);
				strncat(buf, sVal[i],_MSG_LENG-16);
				buf[_MSG_LENG]=0;
			}
			_WriteLogNo(_LOG_ERR, buf);
		}

		pAgtMgr->freeAgent(IdxAgt);
		throw new CMTException(rc, msg);
	}
 
//	for( i = 0; i < 100; i++ ){
	maxQueryCount=configRec.ulMaxQueryCount;
	
	if( maxQueryCount>=pQRule->MaxRows){
		if(pQRule->MaxRows>0){
			maxQueryCount=pQRule->MaxRows;
		}
	}	
	for (i = 0; i < (int) maxQueryCount; i++) {
		if ((rc = pAgtMgr->CursorFetch(IdxAgt, resString, msg)) != 0)
			break;

#ifdef _DEBUG		
//		printf("  DynaSql Fetch : !%s!\n", resString );
#endif

		try {
			pNewResult = pDTList->createRecord(pRuleNode, resString);
		} catch (CMTException *e) {
			rc = e->GetECode();
			strcpy(msg, (char *) e->GetEMessage());
			pResultList = 0;
			break;
		}

		if (!pResultList)
			pResultList = pNewResult;
		else
			pLastResult->pNext = pNewResult;

		pLastResult = pNewResult;

	}

//  no result일때 Query display
	/*
	 if ( i == 0 ) {
	 printf("MRQR : no result.\n");
	 _WriteLogNo( _LOG_ERR, QS );
	 }
	 */
	pAgtMgr->CursorClose(IdxAgt);

//	if ( pDBPtr->dbms != 3 ) {  //  Sybase아닐때
	pAgtMgr->freeAgent(IdxAgt);
//	}

	if (rc < 0 || rc > 10) {
		_WriteLog(_LOG_ERR, "Query error.");
		_WriteLogNo(_LOG_ERR, pQRule->QueryStatement);
		throw new CMTException(rc, msg);
	}

	return pResultList;
}

RESULT_REC* CMRQryRun::ExecuteDynamicSqlCUDProc(CMRDataList *pDTList,
		RULE_LIST *pRuleNode, QRULE_DATA *pQRule, char *sVal[]) {
	RESULT_REC *pResultList = 0;
	char msg[CONDITION_STR_SIZE];
    int  valSize=0;
	DATABASE_LIST *pDBPtr;
	CMRAgtMgr *pAgtMgr;
	int IdxAgt;
	int rc;
	int i;
	pDBPtr = getDataBaseInfo(pQRule->DBName);
	if (!pDBPtr) {
		//sprintf(msg, "DataBase(%s) not defined.", pQRule->DBName);
		sprintf(msg, MSG_NOTDEFINE_DB, pQRule->DBName);
		throw new CMTException(UERR_DBERROR, msg);
	}

	pAgtMgr = (CMRAgtMgr *) pDBPtr->pAgtMgr;

	IdxAgt = pAgtMgr->getAgent(pRuleNode->rule_id);

	rc = pAgtMgr->ExecCUD(IdxAgt, pQRule, sVal, msg);

	pAgtMgr->freeAgent(IdxAgt);

	if (rc != 0) {
		_WriteLog(_LOG_ERR, msg);

		if (rc != NOTFOUND && rc != SQL_DUP) {
			char buf[_MSG_LENG + 1];
			char *p = pQRule->QueryStatement;
			int L1 = strlen(p);
			int L2 = 0;

			while (L2 < L1) {
				strncpy(buf, p + L2, _MSG_LENG);
				buf[_MSG_LENG] = 0;
				_WriteLogNo(_LOG_ERR, buf);
				L2 += _MSG_LENG;
			}

			_WriteLogNo(_LOG_ERR, "");

		/*2019.03.06 산업은행 에서 Query Rule 결과 값으로 Insert 하는경우
		  해당 결과가 Array로 반환되고 해당 Array 를 insert 하려고 하는 경우
		  오류가 발생되어야 하나 그전에 메모리 영역 침범하여 Down 발생
		   */
			for (i = 0; i < pQRule->nBindVal; i++) {
				valSize=strlen(sVal[i]);
				if( valSize < _MSG_LENG-16 ){
					sprintf(buf, "Bind var[%2d] : %s", i + 1, sVal[i]);
				}else{
					sprintf(buf, "Bind var[%2d] : ", i + 1);
					strncat(buf, sVal[i],_MSG_LENG-16);
					buf[_MSG_LENG]=0;
				}
				_WriteLogNo(_LOG_ERR, buf);
			}

			throw new CMTException(rc, msg);
		}
	}

//	if ( rc < 0 || rc > 10 ) throw new CMTException( rc, msg );

	try {
		pResultList = (RESULT_REC *) pDTList->MRMemAlloc1(sizeof(RESULT_REC));
		pResultList->value = (VALUE_UNION *) pDTList->MRMemAlloc1(
				sizeof(VALUE_UNION));
		pResultList->value->longVal = rc;
		pResultList->pNext = 0;
	} catch (CMTException *e) {
		rc = e->GetECode();
		strcpy(msg, (char *) e->GetEMessage());
		delete e;

		_WriteLog(_LOG_ERR,
				"Memory alloc error (CMRQryRun::ExecuteDynamicSqlCUDProc).");

		throw new CMTException(rc, msg);
	}

	return pResultList;
}

RESULT_REC* CMRDataList::ExecuteDynamicSql(RULE_LIST *pRuleNode,
		QRULE_DATA *pQRule, char *sVal[]) {
	RESULT_REC *pResult;

	if (pRuleNode->s_type == QR_TP_CUD){
		pResult = g_pCQryRun->ExecuteDynamicSqlCUDProc(this, pRuleNode, pQRule,
				sVal);
	}
	else {
		pResult = g_pCQryRun->ExecuteDynamicSqlProc(this, pRuleNode, pQRule,
			sVal);
	}
	return pResult;
}

