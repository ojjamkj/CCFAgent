#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
 
#ifndef WIN32
#include    <unistd.h>
#endif
 

#include    "BRLine.h"

#include    "MRCSComm.h"
/*
 #include    "MRERROR.h"
 #include    "MTSysMsg.h"
 #include    "MTExcept.h"
 */ 
#include    "MTCtSect.h"
#include    "BREXLog.h" 

#ifdef _MSG_KO
#define   MSG_THREAD_FAIL   "로그 스레드 시작 실패.\n"
#define   MSG_LOG_ERR       "\n로그 파일을 열수 없습니다. (%s)\n\n"
#else
#define   MSG_THREAD_FAIL   "Log thread start failure.\n"
#define   MSG_LOG_ERR       "\nCan not open Log file (%s)\n\n"
#endif

CMTCriticalSection m_LogCSect;

CBREXLog *g_pCBREXLog;

/**************************************************************************************************/

CBREXLog::CBREXLog() :
		CBREXThreadrun(), CBREXThread(100) {
#ifdef _OS390
	m_bLog2File = 0;
#else
	m_bLog2File = 1;
#endif

	char tempName[200];
	int len;

	strcpy(tempName, configRec.lpszLogPath);

	len = strlen(tempName);
	if (tempName[len - 1] != PATH_DELIM) {
		tempName[len] = PATH_DELIM;
		tempName[len + 1] = 0;
	}
	sprintf(m_fName, "%sAgent%02d", tempName, (int) configRec.ulNoServer);

	/*****************************************************************/

	m_qSize = configRec.ulLogQueueSz;
	if (m_qSize < 500)
		m_qSize = 500;
	else if (m_qSize > _MAX_QUEUE)
		m_qSize = _MAX_QUEUE;

	m_MsgRec = (LOG_REC*) malloc(sizeof(LOG_REC) * m_qSize);

	/*****************************************************************/

	getCurrentTime (m_lpszTime);
	m_lpszTime[8] = 0;

	m_pop = 0;
	m_push = 0;

	if (Start(this)) {
		char lpsz[256];

		//sprintf(lpsz, "Log thread start failure.\n");
		sprintf(lpsz, MSG_THREAD_FAIL);
		throw new CMTException(SYSERR_THREADSTARTFAILURE, lpsz);
	}

}

CBREXLog::~CBREXLog() {
	if (m_pop != m_push)
		WriteMessage();

	Terminate();

//	printf("Terminate CBREXLog\n");
}

void CBREXLog::Run(void) {
	int nLoop = 0;

	try {
		while (1) {

#ifdef WIN32
//			_sleep(100); // 0.1 Second
			Sleep(100);
#else
					usleep( 100000 ); // 0.1 Second
#endif
//			printf("cnt : %d %d\n", m_pop, m_push );
			if (m_pop != m_push)
				WriteMessage();

		}
	} catch (...) {
		throw new CMTException(SYSERR_, "Exception occurs in log thread.");
	}
}  

void CBREXLog::WriteMessage() {
	FILE *fpLog;
	unsigned char bFileOpened;
	char lpszFileName[100];
	char msgGbn[4];
	char m_lpszTimeDate[20];
	
		
/*	getCurrentTime (m_lpszTimeDate);
	m_lpszTimeDate[8] = 0;
*/
 
	while (m_pop != m_push) {


		if(m_MsgRec[m_pop].proctime[0]==0 ||m_MsgRec[m_pop].proctime[0]==' ' ){

		}else{
			strncpy(m_lpszTime, m_MsgRec[m_pop].proctime, 8);
			m_lpszTime[8] = 0;
			if(m_lpszTime[0]==0 ||m_lpszTime[0]==' ' ){
					getCurrentTime (m_lpszTime);
					m_lpszTime[8] = 0;
			}
		}

		switch (m_MsgRec[m_pop].msgGbn) {
		case _LOG_LOG:
			strcpy(msgGbn, "log");
			break;
		case _LOG_ERR:
			strcpy(msgGbn, "err");
			break;
		case _LOG_DAT:
			strcpy(msgGbn, "run");
			break;
		default:
			strcpy(msgGbn, "log");
		}

		sprintf(lpszFileName, "%s_%s.%s", m_fName, m_lpszTime, msgGbn);

		if ((fpLog = fopen(lpszFileName, "a+")) == NULL) {
			//printf("\nCan not open Log file (%s)\n\n", lpszFileName);
			printf(MSG_LOG_ERR, lpszFileName);
			bFileOpened = 0;
		} else {
			bFileOpened = 1;
		}

		if (bFileOpened) {
			if (m_MsgRec[m_pop].proctime[0]) {
				if( m_MsgRec[m_pop].msg )
						fprintf(fpLog, "[%s] %s\n", m_MsgRec[m_pop].proctime,m_MsgRec[m_pop].msg);
				else
						fprintf(fpLog, "[%s] \n", m_MsgRec[m_pop].proctime );
			} else {
				if( m_MsgRec[m_pop].msg )
					fprintf(fpLog, "%s\n", m_MsgRec[m_pop].msg);
			} 
		} else {
			if (m_MsgRec[m_pop].proctime[0] ) {
				if ( m_MsgRec[m_pop].msg ) 
					printf("[%s] %s\n", m_MsgRec[m_pop].proctime,m_MsgRec[m_pop].msg);
				else
					printf("[%s] \n", m_MsgRec[m_pop].proctime );
			} else {
				if ( m_MsgRec[m_pop].msg ) 
					printf("%s\n", m_MsgRec[m_pop].msg);
			}
		}

		if (++m_pop == m_qSize)
			m_pop = 0;
   
		if (bFileOpened)
			fclose(fpLog);
	} 

}

void CBREXLog::pushMessage(unsigned char msgGbn, bool bDispTime,const char* lpszLog) {
	if (m_bLog2File) {

//  Critical Section Begin
		m_LogCSect.Lock();

		m_MsgRec[m_push].msgGbn = msgGbn;

		if (bDispTime) {
			getCurrentTime(m_MsgRec[m_push].proctime);
		} else {
			m_MsgRec[m_push].proctime[0] = 0;
		}

		strncpy(m_MsgRec[m_push].msg, lpszLog, _MSG_LENG);
		m_MsgRec[m_push].msg[_MSG_LENG] = 0;

		if (m_push + 1 == m_qSize)
			m_push = 0;
		else
			++m_push;

//  Critical Section End
		m_LogCSect.Unlock();

	} else {
		char timeStr[20];

		if (bDispTime) {
			getCurrentTime(timeStr);
			printf("[%s] ", timeStr);
		}

		printf("%s\n", lpszLog);
	}
}
//startip
void CBREXLog::pushMessageIP(unsigned char msgGbn, const char* IP, bool bDispTime,
		const char* lpszLog) {
	if (m_bLog2File) {

//  Critical Section Begin
		m_LogCSect.Lock();

		m_MsgRec[m_push].msgGbn = msgGbn;

		if (bDispTime) {
			getCurrentTime(m_MsgRec[m_push].proctime);
		} else {
			m_MsgRec[m_push].proctime[0] = 0;
		}

//		strncpy(m_MsgRec[m_push].msg, lpszLog, _MSG_LENG);500-23
/*
	  if( IP ){
			sprintf(m_MsgRec[m_push].msg,"[%15s]%s",IP,lpszLog);
	  }else{
			sprintf(m_MsgRec[m_push].msg,"%s",lpszLog);
	  }*/
			sprintf(m_MsgRec[m_push].msg,"%s",lpszLog);
		m_MsgRec[m_push].msg[_MSG_LENG] = 0;

		if (m_push + 1 == m_qSize)
			m_push = 0;
		else
			++m_push;

//  Critical Section End
		m_LogCSect.Unlock();

	} else {
		char timeStr[20];

		if (bDispTime) {
			getCurrentTime(timeStr);
			printf("[%s] ", timeStr);
		}
		printf("%s\n", lpszLog);
	}
}

void CBREXLog::getCurrentTime(char* lpszTime) {
	time_t localTime;
	struct tm *stTempTime;
	struct tm pTm2;
	localTime=time(&localTime);
#ifdef WIN32
	stTempTime= localtime(&localTime);
#else
	stTempTime=localtime_r(&localTime, &pTm2);
#endif
	sprintf(lpszTime
				, "%4d%02d%02d%02d%02d%02d"
				, stTempTime->tm_year + 1900
				, stTempTime->tm_mon + 1
				, stTempTime->tm_mday
				, stTempTime->tm_hour
				, stTempTime->tm_min
				, stTempTime->tm_sec);

/*
	time_t localTime;

	struct tm *stTempTime;
	struct tm pTm;

	localTime=time(&localTime);
//	pTm = localtime(&localTime);

//	stTempTime = localtime(&localTime);
// 	memcpy(&pTm, stTempTime, sizeof(struct tm));
#ifdef WIN32
	stTempTime = localtime(&localTime);
	memcpy(&pTm, stTempTime, sizeof(struct tm));
#else
	stTempTime=localtime_r(&localTime, &pTm);
#endif	

	sprintf(lpszTime, "%4d%02d%02d %02d%02d%02d", stTempTime->tm_year + 1900,
			stTempTime->tm_mon + 1, stTempTime->tm_mday, stTempTime->tm_hour, stTempTime->tm_min,
			stTempTime->tm_sec);
			*/
}

void _WriteLogNo(unsigned char msgGbn, const char* lpszLog) {
//	if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]\n", __FILE__, __LINE__);
	g_pCBREXLog->pushMessage(msgGbn, 1, lpszLog);
}
void _WriteLogNoIP(unsigned char msgGbn,const char* ip, const char* lpszLog) {
//		if (_DEBUG_YN[0] == 'Y')printf("[%s][%d]\n", __FILE__, __LINE__);
	g_pCBREXLog->pushMessageIP(msgGbn, ip,1, lpszLog);
}

void _WriteLog(unsigned char msgGbn, const char* lpszLog) {
//		if (_DEBUG_YN[0] == 'Y')	printf("[%s][%d]\n", __FILE__, __LINE__);
	g_pCBREXLog->pushMessage(msgGbn, 1, lpszLog);
}
void _WriteLogIP(unsigned char msgGbn, const char* ip, const char* lpszLog) {
//		if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]\n", __FILE__, __LINE__);
	g_pCBREXLog->pushMessageIP(msgGbn, ip,1, lpszLog);
}
