#include    <stdio.h>												 
#include    <stdlib.h>
#include    <string.h>

#ifndef _WIN32
#include    <unistd.h>
#endif

#include    "BREXLog.h"
#include    "MRSTRUCT.h"
#include    "MRAgtMgr.h"

#define   CTX_TIMEOUT  3000
#define   INCTIME        50   //  0.05ÃÊ

//extern "C" {

typedef int (*IMPPROC_INIT)(char*, char*, char*, char*);

//}

CMTCriticalSection m_CSectCtx;

int AgtCnt_Ora = 0;
int AgtCnt_DB2 = 0;
int AgtCnt_Syb = 0;

CMRAgtMgr::CMRAgtMgr(DATABASE_LIST *pDBPtr) {
	m_stIdx = 0;

	strcpy(m_DBName, pDBPtr->DBName);
	strcpy(m_DBPName, pDBPtr->DBPName);
	strcpy(m_DBUser, pDBPtr->DBUser);
	strcpy(m_DBPwd, pDBPtr->DBPasswd);
	m_DBAgtCnt = pDBPtr->agents;

	m_DBMS = pDBPtr->dbms;

	char sDbms[10];

	switch (m_DBMS) {
	case 1:
		strcpy(sDbms, "Oracle");
		break;
	case 2: case 6:
		strcpy(sDbms, " DB2  ");
		break;
	case 3:
		strcpy(sDbms, "Sybase");
		break;
	case 4:
		strcpy(sDbms, "AltiBase");
		break;
	default:
		printf("Invalid DBMS (%d).", pDBPtr->dbms);
		exit(0);
	}

	m_lCode = 0;

	switch (m_DBMS) {
	case 1:
		strcpy(m_sModuleName, "MROraAgt");
		break;
	case 2:
		strcpy(m_sModuleName, "MRDB2Agt");
		break;
	case 3:
		strcpy(m_sModuleName, "MRSybAgt");
		break;
	case 4:
		strcpy(m_sModuleName, "MRAltAgt");
		break;
	default:
		break;
	}

	if (m_DBAgtCnt > 0)
		initialize();
	else {
		m_lCode = -1;
		strcpy(m_sMessage, "DB Agent count 0");
	}

	char msg[200];

	if (m_lCode != 0) {
		sprintf(msg, "    %-12s %8s  %-12s  %-10s  %2d  : Error", m_DBName,
				sDbms, m_DBPName, m_DBUser, m_DBAgtCnt);
	} else {
		sprintf(msg, "    %-12s %8s  %-12s  %-10s  %2d  : Success", m_DBName,
				sDbms, m_DBPName, m_DBUser, m_DBAgtCnt);
	}

	_WriteLogNo(_LOG_LOG, msg);
}

CMRAgtMgr::~CMRAgtMgr() {
	FreeFunction();
}

short CMRAgtMgr::getDBMSType() {
	return m_DBMS;
}

int CMRAgtMgr::getAgentCount() {
	return m_DBAgtCnt;
}

int CMRAgtMgr::getError(char *msg) {
	if (m_lCode != 0)
		strcpy(msg, m_sMessage);

	return m_lCode;
}

void CMRAgtMgr::initialize() {
	int accum;
	int i;

	m_DBAgent = (DB_AGENT*) malloc(sizeof(DB_AGENT) * m_DBAgtCnt);
	if (m_DBAgent == 0) {
		m_lCode = -1;
		sprintf(m_sMessage, "Memory alloc error for DataBase Agent.");
		return;
	}

	switch (m_DBMS) {
	case 1:
		accum = AgtCnt_Ora;
		AgtCnt_Ora += m_DBAgtCnt;
		break;
	case 2:
		accum = AgtCnt_DB2;
		AgtCnt_DB2 += m_DBAgtCnt;
		break;
	case 3:
		accum = AgtCnt_Syb;
		AgtCnt_Syb += m_DBAgtCnt;
		break;
	default:
		break;
	}

	for (i = 0; i < m_DBAgtCnt; i++)
		m_DBAgent[i].DllInstance = NULL;

	for (i = 0; i < m_DBAgtCnt; i++) {
		LoadAgentLib(accum, i); //   DLL Load
		if (m_lCode != 0)
			return;

//printf("m_DBAgent[i].DllInstance = %x\n", m_DBAgent[i].DllInstance );

		m_DBAgent[i].ImpFuncCurOpen = LoadFunction(i, "EXECSQLCUROPEN");
		if (m_lCode != 0)
			return;

		m_DBAgent[i].ImpFuncCurFetch = LoadFunction(i, "EXECSQLFETCH");
		if (m_lCode != 0)
			return;

		m_DBAgent[i].ImpFuncCurClose = LoadFunction(i, "EXECSQLCURCLOSE");
		if (m_lCode != 0)
			return;

		m_DBAgent[i].ImpFuncCUD = LoadFunction(i, "EXECSQLCUD");
		if (m_lCode != 0)
			return;

		InitAgent(i);
		if (m_lCode != 0)
			return;

		m_DBAgent[i].free = 1;
	}
}

void CMRAgtMgr::AgentStatus(CBRMObj *pObjParm) {
	struct tm stTempTime;
	time_t long_time;
	struct tm *newtime;
	char lpszDateTime[20];

	time(&long_time); /* Get time as long integer. */

	for (int i = 0; i < m_DBAgtCnt; i++) {
		pObjParm->WriteString(m_DBName);
		pObjParm->WriteLong(i + 1);

		if (m_DBAgent[i].free) {
			pObjParm->WriteLong(0);
			pObjParm->WriteString("");
			pObjParm->WriteLong(0);
			continue;
		}

		pObjParm->WriteLong(m_DBAgent[i].ruleId);


		stTempTime= localtime(&m_DBAgent[m_stIdx].stTime);
		memcpy(&newtime, stTempTime, sizeof(struct tm));

		sprintf(lpszDateTime, "%04d%02d%02d%02d%02d%02d",
				newtime->tm_year + 1900, newtime->tm_mon + 1, newtime->tm_mday,
				newtime->tm_hour, newtime->tm_min, newtime->tm_sec);

		pObjParm->WriteString(lpszDateTime);
		pObjParm->WriteLong(long_time - m_DBAgent[m_stIdx].stTime);
	}
}

void CMRAgtMgr::AgentCheck() {
	char msg[50];

	for (int i = 0; i < m_DBAgtCnt; i++) {
		sprintf(msg, "      %2d : %s", i + 1,
				m_DBAgent[i].free ? "FREE" : "USE");
		_WriteLogNo(_LOG_LOG, msg);
	}
}

int CMRAgtMgr::getAgent(int ruleId) {
	int j, waittime;

//	if ( m_DBAgtCnt == 0 ) {
//		sprintf( m_sMessage, "DataBase agent not initialized." );
//		return ( -1 );
//	}

	m_CSectCtx.Lock();

	j = 0;
	waittime = 0;

	while (1) {
		if (j == m_DBAgtCnt) {
			if (waittime >= CTX_TIMEOUT) {
				sprintf(m_sMessage, "getCtx timeout.");
				m_CSectCtx.Unlock();
				return (-1);
			}

#if     defined(WIN32)
			_sleep(INCTIME);
#elif   defined( _OS390 )
#define _XOPEN_SOURCE_EXTENDED 1
#include <unistd.h>
			usleep( INCTIME * 1000 );
#else
			usleep( INCTIME * 1000 );
#endif

			j = 0;
			waittime += INCTIME;
		} else {
			if (++m_stIdx == m_DBAgtCnt)
				m_stIdx = 0;
			if (m_DBAgent[m_stIdx].free)
				break;
			j++;
		}
	}

	j = m_stIdx; //  i ´Â staticÀÌ¶ó¼­ µý³ðÀÌ °øÀ¯ÇÏ·Áµë
	m_DBAgent[m_stIdx].free = 0;
	m_DBAgent[m_stIdx].ruleId = ruleId;
	time(&m_DBAgent[m_stIdx].stTime);

	m_CSectCtx.Unlock();

#ifdef _DEBUG
	printf("Agent using(%d) %x\n", j, m_DBAgent[j].DllInstance );
#endif

	return j;
}

void CMRAgtMgr::freeAgent(int idx) {
//    printf("Agent release(%d)\n", idx );

	m_DBAgent[idx].free = 1;
	m_DBAgent[idx].ruleId = 0;

}

void CMRAgtMgr::LoadAgentLib(int accum, int idx) {
	char sDllName[100];

#if defined(_AIX) || defined(__hpux) || defined(__GNUC__)
	void *DllInstance = NULL;

	sprintf(sDllName, "%s%02d.dll", m_sModuleName, idx);

	DllInstance = dlopen(sDllName, RTLD_LAZY);

#elif defined(_OS390)
	dllhandle *DllInstance = NULL;

	strcpy( sDllName, m_sModuleName );

	DllInstance = dllload(sDllName);

#elif defined(_OS400) || defined(WIN32)
	HINSTANCE DllInstance = NULL;

	sprintf( sDllName, "%s%02d.dll", m_sModuleName, accum+idx );
//printf("Fname = %s\n", sDllName );   

//	DllInstance = LoadLibrary( (LPCWSTR) sDllName );
	DllInstance = LoadLibrary( (LPCSTR) sDllName );

#endif

//printf("Fname = %s\n", sDllName );

	if (DllInstance == NULL) {
		sprintf(m_sMessage, "error : Module load failure (%s).", sDllName);
		m_lCode = -1;
		return;
	}

	m_DBAgent[idx].DllInstance = DllInstance;

}

IMPPROC CMRAgtMgr::LoadFunction(int idx, char *funcName) {
#if defined(_AIX) || defined(__hpux) || defined(__GNUC__)
	void *DllInstance = m_DBAgent[idx].DllInstance;
	IMPPROC ImpFunc = (IMPPROC) dlsym(DllInstance, funcName);
#elif defined(_OS390)
	dllhandle *DllInstance = m_DBAgent[idx].DllInstance;
	IMPPROC ImpFunc = (IMPPROC) dllqueryfn(DllInstance, funcName);
#elif defined(_OS400) || defined(WIN32)
	HINSTANCE DllInstance = m_DBAgent[idx].DllInstance;
	IMPPROC ImpFunc = (IMPPROC) GetProcAddress( DllInstance, funcName );
#endif

	if (ImpFunc == NULL) {
#if defined(_AIX) || defined(__hpux) || defined(__GNUC__)
		sprintf(m_sMessage, "A dynamic linking error occurred(%s): %s\n",
				funcName, dlerror());
#else
		sprintf( m_sMessage, "Entry point not found(%s).", funcName );
#endif

		m_lCode = -1;

		return NULL;
	}

	return ImpFunc;
}

void CMRAgtMgr::FreeFunction() {
	for (int i = m_DBAgtCnt - 1; i >= 0; i--) {
		if (m_DBAgent[i].DllInstance == NULL)
			continue;

#if defined(_AIX) || defined(__hpux) || defined(__GNUC__)
		dlclose(m_DBAgent[i].DllInstance);
#elif defined(_OS390)
		dllfree(m_DBAgent[i].DllInstance);
#elif defined(_OS400) || defined(WIN32)
		FreeLibrary(m_DBAgent[i].DllInstance);
#endif
	}

	m_DBAgtCnt = 0;
}

void CMRAgtMgr::InitAgent(int idx) {
#if defined(_AIX) || defined(__hpux) || defined(__GNUC__)
	void *DllInstance = m_DBAgent[idx].DllInstance;
	IMPPROC_INIT ImpFunc = (IMPPROC_INIT) dlsym(DllInstance, "INITAGENT");
#elif defined(_OS390)
	dllhandle *DllInstance = m_DBAgent[idx].DllInstance;
	IMPPROC_INIT ImpFunc = (IMPPROC_INIT) dllqueryfn(DllInstance, "INITAGENT");
#elif defined(_OS400) || defined(WIN32)
	HINSTANCE DllInstance = m_DBAgent[idx].DllInstance;
	IMPPROC_INIT ImpFunc = (IMPPROC_INIT) GetProcAddress( DllInstance, "INITAGENT" );
#endif

	if (ImpFunc == NULL) {
#if defined(_AIX) || defined(__hpux) || defined(__GNUC__)
		sprintf(m_sMessage,
				"A dynamic linking error occurred (INITAGENT) : %s\n",
				dlerror());
#else
		sprintf( m_sMessage, "Entry point not found(INITAGENT)." );
#endif

		m_lCode = -1;

		return;
	}

	m_lCode = ImpFunc(m_DBPName, m_DBUser, m_DBPwd, m_sMessage);

}

int CMRAgtMgr::CursorOpen(int Idx, char *QS, char *msg) {
	IMPPROC ImpFunc = m_DBAgent[Idx].ImpFuncCurOpen;

	return ImpFunc(QS, msg);
}

int CMRAgtMgr::CursorFetch(int Idx, char *res, char *msg) {
	IMPPROC ImpFunc = m_DBAgent[Idx].ImpFuncCurFetch;

	return ImpFunc(res, msg);
}

int CMRAgtMgr::CursorClose(int Idx) {
	char dummy1[10], dummy2[10];

	IMPPROC ImpFunc = m_DBAgent[Idx].ImpFuncCurClose;

	return ImpFunc(dummy1, dummy2);
}

int CMRAgtMgr::ExecCUD(int Idx, char *QS, char *msg) {
	IMPPROC ImpFunc = m_DBAgent[Idx].ImpFuncCUD;

	return ImpFunc(QS, msg);
}
