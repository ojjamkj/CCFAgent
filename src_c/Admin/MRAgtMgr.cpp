#include    <stdio.h>												 
#include    <stdlib.h>
#include    <string.h>

#ifndef _WIN32
#include    <unistd.h>
#endif
#include    "MItem.h"
#include    "BREXLog.h"
#include    "MRSTRUCT.h"
#include    "MRAgtMgr.h"

#define   CTX_TIMEOUT  3000
#define   INCTIME        50   //  0.05초

#ifdef _MSG_KO
#define   MSG_INVALID_DBMS          "유효하지 않은 DBMS (%d)."
#define   MSG_MODULELOAD_FAIL       "오류 : 모듈 로드가 실패하였습니다 (%s)."
#define   MSG_DYNLIK_ERR            "동적 연결 오류가 발생하였습니다.(%s): %s\n"
#define   MSG_NOTFOUND_ENTPOINT     "Entry point를 찾을 수 없습니다(%s)."
#define   MSG_DYNLIK_ERR1           "동적 연결 오류가 발생하였습니다(INITAGENT): %s\n"
#define   MSG_NOTFOUND_ENTPOINT1    "Entry point를 찾을 수 없습니다(INITAGENT)."
#define   MSG_MEMALLOC_DBAGENT_ERR  "DataBase Agent의 메모리 할당 오류."
#define   MSG_TIMEOUT               "getCtx 시간 초과."
#define   MSG_USE_AGENT             "Agent 사용 중입니다.(%d)\n"
#else
#define   MSG_INVALID_DBMS          "Invalid DBMS (%d)."
#define   MSG_MODULELOAD_FAIL       "error : Module load failure (%s)."
#define   MSG_DYNLIK_ERR            "A dynamic linking error occurred(%s): %s\n"
#define   MSG_NOTFOUND_ENTPOINT     "Entry point not found(%s)."
#define   MSG_DYNLIK_ERR1           "A dynamic linking error occurred(INITAGENT): %s\n"
#define   MSG_NOTFOUND_ENTPOINT1    "Entry point not found(INITAGENT)."
#define   MSG_MEMALLOC_DBAGENT_ERR  "Memory alloc error for DataBase Agent."
#define   MSG_TIMEOUT               "getCtx timeout."
#define   MSG_USE_AGENT             "Agent using(%d)\n"
#endif

CMTCriticalSection m_CSectCtx;

CMRAgtMgr::CMRAgtMgr(DATABASE_LIST *pDBPtr) {
	 
}

CMRAgtMgr::~CMRAgtMgr() {
 
}

int CMRAgtMgr::getError(char *msg) {
 
}

void CMRAgtMgr::LoadAgentLib() {
	 
}

void CMRAgtMgr::FreeAgentLib() {
 
}

IMPPROC_1 CMRAgtMgr::LoadFunction1(char *funcName) {
	return NULL;
}

IMPPROC_2 CMRAgtMgr::LoadFunction2(char *funcName) {
 
	return NULL;
}

void CMRAgtMgr::LoadFunctionInit() {
 
}

void CMRAgtMgr::initAgent() {
 
}

int CMRAgtMgr::CursorOpen(int Idx, QRULE_DATA *pQRule, char *Val[], char *msg) {
	 
}

int CMRAgtMgr::CursorFetch(int Idx, char *res, char *msg) {
	return ImpFuncCurFetch(&m_DBAgent[Idx], res, msg);
}

int CMRAgtMgr::CursorClose(int Idx) {
	char dummy1[10], dummy2[10];

	return ImpFuncCurClose(&m_DBAgent[Idx], dummy1, dummy2);
}

int CMRAgtMgr::ExecCUD(int Idx, QRULE_DATA *pQRule, char *Val[], char *msg) {
	switch (m_pDBInfo->dbms) {
	case 2:	case 6: //  DB2 일때
		ConvDB2String(pQRule);

		return ImpFuncCUD(m_pDBInfo, &m_DBAgent[Idx], pQRule->QueryStatement,
				pQRule->nBindVal, Val, msg);

	case 3: //  Sybase 일때
		char QryString[MAX_QUERY_SIZE];  //MItem.h 30000 MAX_QUERY_SIZE 
		BuildQueryString(pQRule, Val, QryString);
		QryString[MAX_QUERY_SIZE-1]=0;

		return ImpFuncCUD(m_pDBInfo, &m_DBAgent[Idx], QryString,
				pQRule->nBindVal, Val, msg);
	default:
		return ImpFuncCUD(m_pDBInfo, &m_DBAgent[Idx], pQRule->QueryStatement,
				pQRule->nBindVal, Val, msg);
	}
}

void CMRAgtMgr::ConvDB2String(QRULE_DATA *pQRule) {
	QBRULE_SUBSTR *pLast;

	pLast = pQRule->pBindValue;
	while (pLast) {
		strncmp(pQRule->QueryStatement + pLast->pos, "?   ", 4);
		pLast = pLast->pNext;
	}
}
//확인사항2015.02
//2018.09.28 수정
void CMRAgtMgr::BuildQueryString(QRULE_DATA *pQRule, char *Val[], char *NewQuery) {
	QBRULE_SUBSTR *pLast;
	int i, j, k;
	char *p;
	char *token;
	char tempData[MAX_QUERY_SIZE+1000];

	pLast = pQRule->pBindValue;
	i = j = k = 0;
	while (pLast) {
		while (j < pLast->pos) {
			NewQuery[i++] = pQRule->QueryStatement[j++];
		}
		memset(tempData, 0x00, MAX_QUERY_SIZE+1000);
		strcpy(tempData, Val[k]);
		token = strtok(tempData, ",");
		if ('\'' == Val[k][0]) {
			if (token != NULL) {
				sprintf(NewQuery + i, "%s", token);
				i += strlen(token);
				token = strtok(NULL, ",");
			}
			else {
				sprintf(NewQuery + i, "%s", Val[k]);
				i += strlen(Val[k]);  // +2 ->7
			}
		}
		else {
			if (token != NULL) {
				sprintf(NewQuery + i, "'%s'", token);
				i += strlen(token) + 2;
				token = strtok(NULL, ",");
			}
			else {
				sprintf(NewQuery + i, "'%s'", Val[k]);
				i += strlen(Val[k]) + 2;  // +2 ->7
			}
		}

		j += 4;
		k++;

		pLast = pLast->pNext;
	}
	strcpy(NewQuery + i, pQRule->QueryStatement + j);
}

/*
void CMRAgtMgr::BuildQueryString(QRULE_DATA *pQRule, char *Val[], char *NewQuery) {
  QBRULE_SUBSTR *pLast;
  int i, j, k;
  char *p;
  char *token;
  char *tempData;

  pLast = pQRule->pBindValue;
  i = j = k = 0;
    while (pLast) {
        while (j < pLast->pos) {
            NewQuery[i++] = pQRule->QueryStatement[j++];
        }
        tempData=(char*)malloc( strlen(Val[k]) );
        strcpy(tempData,Val[k]);
        token=strtok(tempData,",");
        if('\''==Val[k][0]){
                if( token!=NULL){
                    sprintf(NewQuery + i, "%s",token);
                    i += strlen(token) ;
                    token = strtok(NULL, ",");
                }else{
                    sprintf(NewQuery + i, "%s", Val[k]);
                    i += strlen(Val[k])  ;  // +2 ->7
                }
        }else{
                if( token!=NULL){
                    sprintf(NewQuery + i, "'%s'",token);
                    i += strlen(token)+2 ;
                    token = strtok(NULL, ",");
                }else{
                    sprintf(NewQuery + i, "'%s'", Val[k]);
                    i += strlen(Val[k])+2  ;  // +2 ->7
                }
        }
        if( tempData!=NULL) free(tempData);

        j += 4;
        k++;

        pLast = pLast->pNext;
    }
    strcpy(NewQuery + i, pQRule->QueryStatement + j);
}
*/
/*
void CMRAgtMgr::BuildQueryString(QRULE_DATA *pQRule, char *Val[],
		char *NewQuery) {
	QBRULE_SUBSTR *pLast;
	int i, j, k;

	pLast = pQRule->pBindValue;
	i = j = k = 0;
	while (pLast) {
		while (j < pLast->pos) {
			NewQuery[i++] = pQRule->QueryStatement[j++];
		}

		strcpy(NewQuery + i, Val[k]);
		i += strlen(Val[k]);
		j += 4;
		k++;

		pLast = pLast->pNext;
	}

	strcpy(NewQuery + i, pQRule->QueryStatement + j);

}
*/

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
				//sprintf(m_sMessage, "getCtx timeout.");
				sprintf(m_sMessage, MSG_TIMEOUT);
				m_CSectCtx.Unlock();
				return (-1);
			}

#if     defined(WIN32)
//			_sleep(INCTIME);
			Sleep(INCTIME);
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

	j = m_stIdx; //  i 는 static이라서 딴놈이 공유하려듬
	m_DBAgent[m_stIdx].free = 0;
	m_DBAgent[m_stIdx].ruleId = ruleId;
	time(&m_DBAgent[m_stIdx].stTime);

	m_CSectCtx.Unlock();

#ifdef _DEBUG
	//printf("Agent using(%d)\n", j );
//	printf(MSG_USE_AGENT, j );
#endif

	return j;
}

void CMRAgtMgr::freeAgent(int idx) {
//    printf("Agent release(%d)\n", idx );

	m_DBAgent[idx].ruleId = 0;
	m_DBAgent[idx].free = 1;
}

void CMRAgtMgr::AgentStatus(CBRMObj *pObjParm) {
	time_t localTime;
	struct tm *stTempTime;
	struct tm pTm2;
	char lpszDateTime[20];

	for (int i = 0; i < m_DBAgtCnt; i++) {
		pObjParm->WriteString(m_pDBInfo->DBName);
		pObjParm->WriteLong(i + 1);

		if (m_DBAgent[i].free) {
			pObjParm->WriteLong(0);
			pObjParm->WriteString("");
			pObjParm->WriteLong(0);
			continue;
		}

		pObjParm->WriteLong(m_DBAgent[i].ruleId);
		localTime=time(&localTime);/* Get time as long integer. */

#ifdef WIN32
	stTempTime= localtime(&localTime);
#else
	stTempTime=localtime_r(&localTime, &pTm2);
#endif


		sprintf(lpszDateTime, "%04d%02d%02d%02d%02d%02d",
				stTempTime->tm_year + 1900, stTempTime->tm_mon + 1, stTempTime->tm_mday,
				stTempTime->tm_hour, stTempTime->tm_min, stTempTime->tm_sec);

		pObjParm->WriteString(lpszDateTime);
		pObjParm->WriteLong(localTime - m_DBAgent[i].stTime);
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

