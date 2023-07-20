#include    <stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<errno.h>
#if defined(_AIX)
#include	<strings.h>
#endif
#include "BRLine.h"
#include    "MRERROR.h"
#include    "MRCSComm.h"
//#include    "MTParm.h"
#include    "BRMObj.h"

#include	"MTUtil.h"
#include    "MRCSMgr.h"
#include    "MRCSThr.h"
 
#include    "MRDTLIST.h"
#include	"BREXLog.h"

#ifdef _MSG_KO
#define   MSG_THREAD_FAIL      "스레드 시작 실패. Thread %u.\n"
#define   MSG_THREAD_TERM      "스레드 %d이 종료 중입니다. \n"
#define   MSG_SUCCESS_OPEN     "열기에 성공하였습니다.\n"
#define   MSG_DBOPEN_ERR       "DB 열기 오류.\n"
#define   MSG_RETERNCODE       "\tReturn Code: %X\n"
#define   MSG_REASONCODE       "\tReason Code: %X\n"
#define   MSG_EXPT_DELSOCKET   "child 소켓 삭제 시에 예외가 발생하였습니다.\n"
#define   MSG_EXPT_GNR         "GetNextRequest()에서 예외가 발생하였습니다.\n"

#define   MSG_UNKNOWNERR_PSREQ  "ProcessRequest에서 알 수 없는 오류."
#else
#define   MSG_THREAD_FAIL      "Thread start failure. Thread %u.\n"
#define   MSG_THREAD_TERM      "Thread %d terminating...\n"
#define   MSG_SUCCESS_OPEN     "Open succeeded.\n"
#define   MSG_DBOPEN_ERR       "DB Open Error.\n"
#define   MSG_RETERNCODE       "\tReturn Code: %X\n"
#define   MSG_REASONCODE       "\tReason Code: %X\n"
#define   MSG_EXPT_DELSOCKET   "Exception occurs in delete child socket.\n"
#define   MSG_EXPT_GNR         "Exception occurs in GetNextRequest().\n"

#define   MSG_UNKNOWNERR_PSREQ  "Unknown error in ProcessRequest."
#endif

// for Call Attachment Facility
 
#if defined(_OS390)
extern "OS"
{
	int DSNALI( char *functn, ... );
}
#endif

//int ConnectDatabase( void *ctxParm, char *DBName,
//                     char *DBUser, char *DBPasswd );

CCSThread::CCSThread(void* pManager, unsigned long ulThreadNo) :
		CBREXThreadrun(), CBREXThread(ulThreadNo) {
	m_pManager = pManager;
	m_ulThreadNo = ulThreadNo;
	m_MaxPramSize = ((CCSManager*) m_pManager)->m_MaxPramSize;
	m_pChildSoc = -1;
	strcpy(m_pgmId	,"");
	m_HitCount = 0;
	m_bUsing = 0;
	m_ctx = 0;
	bDisplayed = 0;
	m_TempString[0] = 0;

	m_ObjBuffer.setMaxPramSize(m_MaxPramSize);


	m_ObjBuffer.m_Buffer = (unsigned char *) malloc(m_MaxPramSize + 1);
	if (!m_ObjBuffer.m_Buffer) {
		throw new CMTException(SYSERR_, ERRMSG_MEMORYALLOCATEFAILURE_02);
	}

	if (Start(this)) {
		char lpsz[256];

		//sprintf(lpsz, "Thread start failure. Thread %u.\n", m_ulThreadNo);
		sprintf(lpsz, MSG_THREAD_FAIL, m_ulThreadNo);
		throw new CMTException(SYSERR_THREADSTARTFAILURE, lpsz);
	}


}

CCSThread::~CCSThread() {
//	if ( m_pChildSoc >= 0 ) g_pManager->CloseChildSoc(m_pChildSoc);

	if (m_ObjBuffer.m_Buffer)
		free(m_ObjBuffer.m_Buffer);

	delete m_ObjHeader;

	delete (CMRDataList*) pCMRDataList;

	//printf("Thread %d terminating...\n", m_ulThreadNo);
	printf(MSG_THREAD_TERM, m_ulThreadNo);
}

void CCSThread::Initialize() {

//	Terminate();

	m_pChildSoc = -1;
	m_HitCount = 0;
	m_bUsing = 0;
	bDisplayed = 0;
	m_TempString[0] = 0;
}

/****************************************/

extern char g_errMsg[200];

void CCSThread::Run(void) {
	long lReqNo = 0;

#if defined(_OS390)
	long retcode, // return code
	reascode;// reason code

	// open DB
	DSNALI( "OPEN        ",
			configParm->_DBNAME, configParm->_DBUSER,
			&retcode, &reascode );

	if( retcode == 0 ) {
 
	} else { /*
		printf( "DB Open Error.\n" );
		printf( "\tReturn Code: %X\n", retcode );
		printf( "\tReason Code: %X\n", reascode );*/
		printf( MSG_DBOPEN_ERR );
		printf( MSG_RETERNCODE, retcode );
		printf( MSG_REASONCODE, reascode );
	}

#else

#endif

	pCMRDataList = new CMRDataList(m_ulThreadNo, m_ctx, configRec.ulMaxMemSize1,	configRec.ulMaxMemSize2, configRec.ulMaxMemSize3);

	m_ObjHeader = new CBRMObj(50);

	while (1) {
		if (m_pChildSoc == -1) {
			if (((CCSManager*) m_pManager)->m_bTerminate)
				break;
#ifdef WIN32
//			_sleep(10);        // 0.01 Second
			Sleep(10); // 0.01 Second
#else
					usleep( configRec.ulSlTm ); // 0.01 Second
#endif
			if (((CCSManager*) m_pManager)->m_AdminRunning)
				continue;

			m_pChildSoc = ((CCSManager*) m_pManager)->GetNextRequest(
					m_ulThreadNo);
			if (m_pChildSoc == -1)
				continue;

		}

//   ProcessRequest 안으로 이동함
//		m_bUsing = 1;
 
			try {

			ProcessRequest();

		} catch (CMTException *e) {

#ifdef _DEBUG
			char lpsz[256];
			char datetime[26];
			long rc = e->GetECode();

			GetCurrentTimeStr(datetime);

			sprintf(lpsz, "%s : (%d) %s (ThrRun)", datetime, rc,
					(char *) e->GetEMessage() );
			_WriteLog( _LOG_LOG, lpsz );
#endif

			e->Delete();

		} catch (...) {
			//_WriteLog(_LOG_LOG, "Unknown error in ProcessRequest.");
			_WriteLog(_LOG_LOG, MSG_UNKNOWNERR_PSREQ);
		}

		try {
			((CCSManager*) m_pManager)->CloseChildSoc(m_pChildSoc);
		} catch (...) {
			//printf("Exception occurs in delete child socket.\n");
			printf(MSG_EXPT_DELSOCKET);
		}

		m_pChildSoc = -1;

//   ProcessRequest 안으로 이동함
//		m_bUsing = 0;

		if (((CCSManager*) m_pManager)->m_bTerminate)
			break;

		if (((CCSManager*) m_pManager)->m_AdminRunning)
			continue;

		try {
			m_pChildSoc = ((CCSManager*) m_pManager)->GetNextRequest(
					m_ulThreadNo);
		} catch (...) {
			m_pChildSoc = -1;
			//printf("Exception occurs in GetNextRequest().\n");
			printf(MSG_EXPT_GNR);
		}
	}

#if defined(_OS390)
	long retcode, // return code
	reascode;// reason code
	DSNALI( "CLOSE       ", "ABRT", &retcode, &reascode );
#endif

//	printf("%d terminate\n", m_ulThreadNo);
//	Terminate();

}

void CCSThread::StartAdminProcess(void) {
	((CCSManager*) m_pManager)->StartAdminProcess();
}

void CCSThread::EndAdminProcess(void) {
	((CCSManager*) m_pManager)->EndAdminProcess();
}

void CCSThread::DisplayLog(int sig_num) { 
	if (bDisplayed)
		return;
	 
	if (m_bUsing) {
		if (_DEBUG_YN[0] == 'Y')	printf("[%s][%d]\n", __FILE__, __LINE__);
		((CMRDataList*) pCMRDataList)->LogErrorItems(7,sig_num,"SIGNAL Occurs (maybe).");

		if (m_pChildSoc >= 0)
			g_pManager->CloseChildSoc(m_pChildSoc);
		m_pChildSoc = -1;
	}

	bDisplayed = 1;
}

void CCSThread::DisplayLogLock() {
	 

}

long CCSThread::GetRunInfo(char *PgmId) {
	return 0;
}

long CCSThread::GetRunInfo2() {
	return 0;
}
void CCSThread::GetPgmId(char * str) { strcpy(str, m_pgmId);}

/*
long CCSThread::GetUseMemorys() {
	return ((CMRDataList*)pCMRDataList)->GetUseMemory();
}
*/
