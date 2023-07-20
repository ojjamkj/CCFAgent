#include    <stdio.h>												 
#include    <stdlib.h>
#include    <string.h>

#include    "MRSTRUCT.h"
#include    "MRDEFINE.h"
#include    "MRDTypes.h"
#include    "MRERROR.h"
#include    "MTSysMsg.h"
#include    "MTExcept.h" 
#include    "MTUtil.h"
#include    "MRCSComm.h"
#include    "MRGBLVAR.h"
#include    "MTCtSect.h"

#ifdef _TESTER
#include  "BRMFile.h"
#include  "BRMStart.h"
#endif

#ifdef _MSG_KO
#define   MSG_INVALID_RULE    "룰(%d)이 유효하지 않습니다."
#define   MSG_LOADRULE_ERR    "룰(%d) 로드 오류."
#define   MSG_NOTROAD_GRN     "getRuleNode(%s) 가 로드되지 않았습니다."
#define   MSG_LOADERR         "로드오류 : %s\n"
#else
#define   MSG_INVALID_RULE    "Rule(%d) Invalid."
#define   MSG_LOADRULE_ERR    "Rule(%d) load error."
#define   MSG_NOTROAD_GRN     "getRuleNode(%s) not loaded"
#define   MSG_LOADERR         "LoadError : %s\n"
#endif

CMRGblVars *pCMRGblVars;

PGM_LIST *pPgmList;
RULE_LIST *pRuleList;
//DATABASE_LIST *pDBList;

#ifdef _RUNNER
CMTCriticalSection m_TraceCSect;
#endif

//int  g_ThrBufSize1;
//int  g_ThrBufSize2;
//int  g_ThrBufSize3;

CMRGblVars::CMRGblVars() :
		CMRSearch() {

	pPgmList = 0;
	pRuleList = 0;
	m_pExtRuleRoot = 0;
	m_AllocSize = 0;

#ifdef	_TRACE
	m_TraceMode = false;
	m_TraceListCnt = 0;

	for ( int i=0; i<10; i++ ) m_TraceList[i].pgmId[0] = 0;
#endif

}

CMRGblVars::~CMRGblVars() {
//  	freeRuleNode( pRuleList );
}

void * CMRGblVars::MetisAlloc(int sz) {
	void *mm;

	if (sz == 0) {
		printf("");
		return 0;
	}

//printf("MetisAlloc 1: %d\n", sz );
#ifdef WIN32
	sz = (sz + 64 - (sz % 64));
#endif
//printf("MetisAlloc 2: %d\n", sz );

	mm = (void *) malloc(sz);
	if (!mm) {
		printf("current memory size = %d(+%d)\n", m_AllocSize, sz);
		throw new CMTException(SYSERR_MEMORYALLOCATEFAILURE,
				ERRMSG_MEMORYALLOCATEFAILURE_13);
	}

	m_AllocSize += sz;

	return mm;
}

void CMRGblVars::MetisFree(void *p) {
	if (p && p != (void*) -1)
		free(p);
}

void CMRGblVars::MetisFree(void *p, int sz) {
	if (p && p != (void*) -1) {
		free(p);
		m_AllocSize -= sz;
	}
}

void CMRGblVars::MetisDummyAlloc(int sz) {
	m_AllocSize += sz;
}

void CMRGblVars::DisplayAllocSize(char *msg) {
	sprintf(msg, "Memory allocation size : %d(KB)\n", m_AllocSize / 1024);
}

void CMRGblVars::CreateRuleList(int cnt) {
	m_ruleCount = cnt;

	if (cnt == 0)
		return;

	m_AllocSize = sizeof(RULE_LIST) * cnt;

	pRuleList = (RULE_LIST *) MetisAlloc(m_AllocSize);
}

#ifdef _RUNNER

int CMRGblVars::LoadPgmById( char *pgmId, char *errMessage )
{
	return pCMRLoadData->ReloadPgmById( pgmId, errMessage );
}

int CMRGblVars::LoadRuleById( long ruleId, char reloadHeader,
		char reloadBody, char *errMessage )
{
	int rc = 0;

	RULE_LIST *pRulePtr;

	pRulePtr = SearchById( ruleId );

	if ( pRulePtr ) {
//		if ( pRulePtr->loaded == 'Y' && reloadBody == 'Y') {
//			try {
//				freeRuleNode( pRulePtr );
//			} catch ( ... ) {
//				printf( "Exception Occurs in freeRuleNode.\n" );
//			}

		pRulePtr->pVers = 0;
		pRulePtr->dftResult = 0;
  printf("LoadRuleById   pRulePtr->loaded = 'N'  \n");
		pRulePtr->loaded = 'N';
		pRulePtr->pAggr = 0;
//		}

//		if ( reloadHeader == 'Y' ) {
		rc = pCMRLoadData->ReloadRuleHeader ( pRulePtr, errMessage );
//		}

	} else {
		EXT_RULE_LIST *m_pExtRuleNode;

		pRulePtr = (RULE_LIST *) MetisAlloc( sizeof(RULE_LIST) );
		pRulePtr->rule_id = ruleId;
		pRulePtr->pVers = 0;
		pRulePtr->dftResult = 0;
		pRulePtr->loaded = 'N';
		pRulePtr->branchIdx = -1;
		pRulePtr->pAggr = 0;

		pRulePtr->hit = 0;
		pRulePtr->totTm = 0.0;
		pRulePtr->maxTm = 0;

		rc = pCMRLoadData->ReloadRuleHeader( pRulePtr, errMessage );

		if ( rc == -1 ) {
			MetisFree( pRulePtr );
			return -1;
		}

		m_pExtRuleNode = (EXT_RULE_LIST *) MetisAlloc( sizeof(EXT_RULE_LIST) );
		m_pExtRuleNode->pRuleList = pRulePtr;
		m_pExtRuleNode->pNext = m_pExtRuleRoot;

		m_pExtRuleRoot = m_pExtRuleNode;

//		m_ExtRuleCount ++;
	}

	return rc;
}

PGM_LIST * CMRGblVars::getPgmNode( char *pgmId )
{
	PGM_LIST *pPgmPtr;
	pPgmPtr = (PGM_LIST *)pPgmList;

	while ( pPgmPtr ) {
//printf( " %s!!%s\n", pgmId, pPgmPtr->PgmId );
		if ( ! strcmp( pgmId, pPgmPtr->PgmId ) ) break;

		pPgmPtr = pPgmPtr->pNext;
	}

	return pPgmPtr;
}

int CMRGblVars::GetPgmInfo( CBRMObj *pObjParm )
{
	char PgmId[11];
	pObjParm->ReadString( PgmId );

	PGM_LIST * pPgmNode = getPgmNode( PgmId );
	if ( ! pPgmNode ) {
		pObjParm->Clear1();
		pObjParm->WriteLong(MRC_INPUT_ERR);
		pObjParm->WriteLong(UERR_PGMNFOUND);
		pObjParm->WriteString(EMSG_PGMNFOUND);
		return MRC_INPUT_ERR;
	}

	ITEM_INFO *pItem = (ITEM_INFO *) pPgmNode->pParm;

	pObjParm->Clear1();
	pObjParm->WriteLong( 0 );

	pObjParm->WriteLong( pPgmNode->itemCnt );

	for ( int i=0; i<pPgmNode->itemCnt; i++ ) {
		pObjParm->WriteLong( pItem[i].itemCode );
		pObjParm->WriteByte( pItem[i].itemType );
	}

	RULE_LIST *pRuleNode = getRuleNode( pPgmNode->startRuleId );
	pObjParm->WriteLong( pRuleNode->retCnt );

	for ( int j=0; j<pRuleNode->retCnt; j++ ) {
		pObjParm->WriteByte( (unsigned char) pRuleNode->retType[j] );
	}

	return 0;
}

RULE_LIST * CMRGblVars::getRuleNode( int ruleId )
{
	RULE_LIST *pRulePtr;
	//AD2001.printf("MRGBLVAR-001 \n");
	pRulePtr = SearchById( ruleId );
	//AD2001.printf("MRGBLVAR-002 \n");

	if ( ! pRulePtr ) return ((RULE_LIST *) 0);
	//AD2001.printf("MRGBLVAR-003 rule_id [%d] \n", pRulePtr->rule_id);

	if ( pRulePtr->loaded == 'X' ) {
		//AD2001.printf("MRGBLVAR-004 \n");
		//sprintf( m_errMessage, "Rule(%d) Invalid.", ruleId );
		sprintf( m_errMessage, MSG_INVALID_RULE, ruleId );
//		throw new CMTPgmException( UERR_RULEINVALID, m_errMessage );	
		throw new CMTException( UERR_RULEINVALID, m_errMessage );
	}

	//AD2001.printf("MRGBLVAR-005 \n");
	if ( pRulePtr->loaded != 'Y' ) {
//printf("bef pCMRLoadData->ReloadRuleBody\n" );
//AD2001.printf("MRGBLVAR-006 \n");
		if ( pCMRLoadData->ReloadRuleBody( pRulePtr ) != 0 ) {
			//AD2001.printf("MRGBLVAR-007 \n");
			//sprintf( m_errMessage, "Rule(%d) load error.", ruleId );
			sprintf( m_errMessage, MSG_LOADRULE_ERR, ruleId );
//			throw new CMTPgmException( UERR_RULELOADFAIL, m_errMessage );

			throw new CMTException( UERR_RULELOADFAIL, m_errMessage );
		}
	}
	//AD2001.printf("MRGBLVAR-008 \n");

	return pRulePtr;
}

RULE_LIST * CMRGblVars::getRuleNode( char *ruleName )
{
	RULE_LIST *pRulePtr;
//	printf("getRule Node [%s]  \n ", ruleName);
	pRulePtr = (RULE_LIST *) SearchByName( ruleName ); // m_pCMRSearch->...

	if ( ! pRulePtr ) return ((RULE_LIST *) 0);

	if ( pRulePtr->loaded == 'X' ) {
		//printf("getRuleNode(%s) not loaded", ruleName);
		printf(MSG_NOTROAD_GRN, ruleName);

		throw new CMTException( UERR_RULEINVALID, EMSG_RULEINVALID );
	}

	if ( pRulePtr->loaded != 'Y' ) {
		if ( pCMRLoadData->ReloadRuleBody( pRulePtr ) != 0 ) {
			throw new CMTException( UERR_RULELOADFAIL, EMSG_RULELOADFAIL );
		}
	}

	return ( pRulePtr );
}

void CMRGblVars::setRuleInvalid( int ruleId )
{
	RULE_LIST *pRulePtr;

	pRulePtr = SearchById( ruleId );
	//AD2001.printf("setRuleInvalid   pRulePtr->loaded = 'X'  \n");
	pRulePtr->loaded = 'X';
}

short CMRGblVars::getRuleScope( int ruleId )
{
	RULE_LIST * pRulePtr = getRuleNode( ruleId );
	if ( pRulePtr ) return pRulePtr->scope;
	else return 0;
}

void CMRGblVars::RemoveRule( long ruleId )
{
	RULE_LIST *pRulePtr;

	pRulePtr = SearchById( ruleId );

	if ( ! pRulePtr ) return;

	DeleteNode( pRulePtr, "" ); //  BranchNameList에서 삭제

	pRulePtr->pVers = 0;
	pRulePtr->branchIdx = -1;
	//AD2001.printf("RemoveRule   pRulePtr->loaded = 'D'  \n");
	pRulePtr->loaded = 'D';
}

#endif

#ifdef	_TESTER
RULE_LIST * CMRGblVars::DbgLoadRule( int ruleId )
{
	int rc = 0;
	char errMessage[300];

	RULE_LIST *pRulePtr;

	pRulePtr = SearchById( (long) ruleId );

	if ( pRulePtr ) return pRulePtr;

	m_pWorker->SuspendWork( _DBG_STEP_LOAD, ruleId, 0, "" );

	EXT_RULE_LIST *m_pExtRuleNode;

	pRulePtr = (RULE_LIST *) MetisAlloc( sizeof(RULE_LIST) );
	pRulePtr->rule_id = ruleId;

	rc = pCMRLoadData->LoadRule( pRulePtr, errMessage );

	if ( rc != 0 ) {
		//printf("LoadError : %s\n", errMessage );
		printf(MSG_LOADERR, errMessage );
		MetisFree( pRulePtr );
		return 0;
	}

	m_pExtRuleNode = (EXT_RULE_LIST *) MetisAlloc( sizeof(EXT_RULE_LIST) );
	m_pExtRuleNode->pRuleList = pRulePtr;
	m_pExtRuleNode->pNext = m_pExtRuleRoot;

	m_pExtRuleRoot = m_pExtRuleNode;

	return pRulePtr;
}

RULE_LIST * CMRGblVars::DbgLoadRule( char *ruleName )
{
	int rc = 0;
	char errMessage[300];

	RULE_LIST *pRulePtr;

	pRulePtr = SearchByName( ruleName );

	if ( pRulePtr ) return pRulePtr;

//	m_pWorker->SuspendWork( _DBG_STEP_LOAD, 85, 0, "" );
	m_pWorker->SuspendWork( _DBG_STEP_LOAD, 0, 0, ruleName );

	pRulePtr = (RULE_LIST *) MetisAlloc( sizeof(RULE_LIST) );

	rc = pCMRLoadData->LoadRule( pRulePtr, errMessage );

	if ( rc != 0 ) {
		MetisFree( pRulePtr );
		throw new CMTException( UERR_RULELOADFAIL, errMessage );
	}

	if ( pRulePtr->rule_id == 0 ) {
		MetisFree( pRulePtr );
		pRulePtr = 0;
	}

	return pRulePtr;
}
#endif

void CMRGblVars::freeRuleNode(RULE_LIST *pRulePtr) {
//	if ( pRulePtr->pVers ) freeRuleVersion( pRulePtr->r_type, pRulePtr->pVers );

//	if ( pRulePtr->pEtc) MetisFree( pRulePtr->pEtc );

	pRulePtr->pVers = 0;
}

void CMRGblVars::freeRuleVersion(short r_type, RULE_VERS *p) {
	if (p->pNext)
		freeRuleVersion(r_type, p->pNext);

	switch (r_type) {
	case 1: /*   CONDITIONAL RULE  */
		freeCRuleData((CRULE_DATA*) p->rule_data);
		break;
	case 2: /*   DYNAMIC SQL RULE  */
		freeQRule(((QRULE_DATA*) p->rule_data)->pBindValue);
		MetisFree(p->rule_data);
		break;
	case 3: /*   BRANCH RULE  */
		freeQRule(((BRULE_DATA*) p->rule_data)->BranchSubString);
		MetisFree(p->rule_data);
		break;
	case 4: /*   External RULE  */
		break;
	case 5: /*   Table RULE  */
		freeTRule((TRULE_DATA*) p->rule_data);
		break;
	default:
		break;
	}

	MetisFree(p);
}

void CMRGblVars::freeCRuleData(CRULE_DATA *p) {
	if (!p)
		return;
//	if ( p == (CRULE_LINE*) -1 ) return;

	freeCRuleData(p->pNext);

	/*  포기한다.   2006-01-13
	 if ( p->resGbn ) {
	 MetisFree( p->result );
	 } else
	 freeCRuleStmt( ( CRULE_STMT *) p->result );
	 */

	MetisFree(p->result);

	freeCRuleStmt(p->cond);

	MetisFree(p);
}

void CMRGblVars::freeCRuleStmt(CRULE_STMT *p) {
	if (!p)
		return;

	freeCRuleStmt(p->pExpression);
	freeCRuleStmt(p->pNext);

	if (p->tokType != DATA_TYPE_DOUBLE && p->tokType != DATA_TYPE_LONG
			&& p->tokType != DATA_TYPE_BOOL)
		MetisFree(p->uni.tokInfo);

	MetisFree(p);
}

void CMRGblVars::freeQRule(QBRULE_SUBSTR *p) {
	if (!p)
		return;

	freeQRule(p->pNext);

	MetisFree(p);
}

void CMRGblVars::freeTRule(TRULE_DATA *p) {
	if (!p)
		return;

	MetisFree(p->cond1);
	MetisFree(p->resGbn);
	MetisFree(p->result);
	MetisFree(p);
}

RULE_LIST* CMRGblVars::SearchById(long Id) {
	EXT_RULE_LIST *m_pExtRuleNode;
	int idx;
	int fIdx, tIdx;

	m_pExtRuleNode = m_pExtRuleRoot;

	while (m_pExtRuleNode) {
		if (Id == m_pExtRuleNode->pRuleList->rule_id) {
			if (m_pExtRuleNode->pRuleList->loaded == 'D')
				return ((RULE_LIST*) 0);
			return m_pExtRuleNode->pRuleList;
		}

		m_pExtRuleNode = m_pExtRuleNode->pNext;
	}

	if (m_ruleCount <= 0)
		return ((RULE_LIST*) 0);

	fIdx = 0;
	tIdx = m_ruleCount - 1;

	while (1) {
		idx = (fIdx + tIdx) / 2;

//printf("Search : %d  %d\n", Id, pRuleList[idx].rule_id );

		if (Id == pRuleList[idx].rule_id) {
			if (pRuleList[idx].loaded == 'D')
				return ((RULE_LIST*) 0);
			else
				return &pRuleList[idx];
		}

		if (Id < pRuleList[idx].rule_id)
			tIdx = idx - 1;
		else
			fIdx = idx + 1;

		if (fIdx > tIdx)
			break;
	}

	return ((RULE_LIST*) 0);
}

#ifdef _RUNNER
int CMRGblVars::SaveStatistics( char *errMessage )
{
	int rc;
	time_t localTime;
	struct tm *stTempTime;
	struct tm pTm2;
	localTime=time(&localTime);
#ifdef WIN32
	stTempTime= localtime(&localTime);
#else
	stTempTime=localtime_r(&localTime, &pTm2);
#endif
 

//	stTempTime= localtime(&long_time);
//	memcpy(&newtime, stTempTime, sizeof(struct tm));

	if ( (rc = pCMRLoadData->SaveStatisticsPgm( stTempTime, errMessage )) != 0 ) return rc;

	/*
	 if ( configRec.bStatFlag[4] == 'Y' ) {
	 if ( (rc = pCMRLoadData->SaveStatisticsMod( errMessage )) != 0 ) return rc;
	 }
	 */

	if ( configRec.usStatFlag == 2 ) {

		for ( int i=0; i<m_ruleCount; i++ ) {
			if ( pRuleList[i].hit == 0 ) continue;

			if ( (rc = pCMRLoadData->SaveStatisticsRule( stTempTime, & pRuleList[i], errMessage )) != 0 ) return rc;
		}

		EXT_RULE_LIST *pExt = m_pExtRuleRoot;
		while ( pExt ) {
			if ( pExt->pRuleList->hit > 0 ) {
				if ( (rc = pCMRLoadData->SaveStatisticsRule( stTempTime, pExt->pRuleList, errMessage )) != 0 ) return rc;
			}

			pExt = pExt->pNext;
		}

	}

	return 0;
}
#endif

/*

 void	CMRGblVars::TraceSet( unsigned char mode )
 {
 #ifdef _RUNNER
 m_TraceCSect.Lock();

 m_TraceMode = mode;
 m_TraceListCnt = 0;

 m_TraceCSect.Unlock();
 #endif
 }


 unsigned char  CMRGblVars::getTraceList()
 {
 #ifdef _RUNNER
 if ( m_TraceListCnt > 0 ) return true;

 m_TraceCSect.Lock();

 if ( ! m_TraceMode ) {
 m_TraceCSect.Unlock();
 return false;
 }

 m_TraceListCnt = buildTraceList( m_TraceList );

 if ( m_TraceListCnt <= 0 ) {
 m_TraceMode = 0;
 m_TraceListCnt = 0;
 }

 m_TraceCSect.Unlock();

 if ( m_TraceListCnt < 0 ) {
 m_TraceListCnt = 0;
 throw new CMTException( UERR_DBERROR,
 m_errMessage );
 }
 #endif

 return m_TraceMode;
 }


 void	CMRGblVars::clearTraceIndex( int idx)
 {
 //  Table에서 삭제해야함.
 m_TraceList[idx].pgmId[0] = 0;
 }


 int		CMRGblVars::getTraceIndex(int idx, char *PgmId)
 {
 for ( int i=idx; i<m_TraceListCnt; i++ ) {
 if ( ! strcmp( PgmId, m_TraceList[i].pgmId )) return i;
 }

 return -1;
 }

 int		CMRGblVars::getTraceValue(int idx, char *sVal)
 {
 strcpy( sVal, m_TraceList[idx].value );
 return m_TraceList[idx].itemCd;
 }

 */

