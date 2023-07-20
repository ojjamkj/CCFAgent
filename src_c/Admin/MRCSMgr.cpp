#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>

#ifdef		WIN32
#define _WINSOCKAPI_   // WinSock.h가 Include되는것을 방지한다.
#include <sys/timeb.h>
#include <time.h>
#else
#include    <strings.h>
#endif

#include    "BRMObj.h"
#include    "MRCSComm.h"
#include    "MTCtSect.h"
#include    "MRCSMgr.h"
#include    "MRCSThr.h"

#include    "BREXLog.h"

#ifndef _OS390
#include	"MRQryRun.h"
#endif

#ifdef _MSG_KO
#define   MSG_READY               "준비.\n"
#define   MSG_THREAD_TERM         "스레드 %d가 종료되었습니다.\n"
#define   MSG_PENDINGREQ_THREAD   "%u번째 스레드에 pending 연결을 요청합니다."
#define   MSG_SIG_RULEROAD        "SIGNAL - %d : in rule loading."

#define   MSG_NORTM_REQ           "정상적인 종료를 요청받았습니다."
#define   MSG_WAIT_ERR            "대기 오류."
#else
#define   MSG_READY               "Ready.\n"
#define   MSG_THREAD_TERM         "Thread %d terminated\n"
#define   MSG_PENDINGREQ_THREAD   "Assigning pending request to %uth thread."
#define   MSG_SIG_RULEROAD        "SIGNAL - %d : in rule loading."

#define   MSG_NORTM_REQ           "normal terminate requested."
#define   MSG_WAIT_ERR            "wait error."
#endif

extern CONFIG_REC configRec;

FILE *fpOut;
unsigned long LogLevel;

CCSManager *g_pManager;
CMTCriticalSection m_sectAdmin; // administration section

unsigned char m_bInitial;
long m_queueCount;

ENGINE_INFO engine_info[100];
	
CCSManager::CCSManager(int dummy) :
		CMRCSQueue() {
			unsigned short nNumOfQues;
	m_bInitial = false;

	fpOut = stdout;
	m_AdminRunning = 0;

#ifndef _OS390
 

#endif

	if (configRec.ulNoThread < 1)
		m_ulNoWThreads = 1;
	else
		m_ulNoWThreads = configRec.ulNoThread;


	m_MaxPramSize = configRec.ulMaxParmSize;
	
	unsigned long ulNoServer = configRec.ulNoServer;
	nNumOfQues = configRec.nNumOfQues;
	unsigned long uiNoThread = configRec.ulNoThread;


	m_queueCount = setEngineInfo(ulNoServer, nNumOfQues, configRec.nDefaultQueueId, uiNoThread, engine_info);
	m_queueCount = 1;
	m_bTerminate = 0;
	m_InitCode = true;

	m_ppWorker = new CCSThread*[m_ulNoWThreads];

	for (unsigned long ul = 0; ul < m_ulNoWThreads; ul++) {
		m_ppWorker[ul] = new CCSThread(this, ul);
	}

	Init(5); //  CMTTcpServer Listener start

	//_WriteLogNo(_LOG_LOG, "Ready.\n");
	//printf("Ready.\n");
	

	_WriteLogNo(_LOG_LOG, "Module load success (MRFuns.dll)");

	_WriteLogNo(_LOG_LOG, MSG_READY);
	printf(MSG_READY);

	m_bInitial = true;
}

CCSManager::~CCSManager() {

	for (unsigned long ul = 0; ul < m_ulNoWThreads; ul++) {
		if (m_ppWorker[ul]) {
			delete m_ppWorker[ul];
			//printf("Thread %d terminated\n", ul);
			printf(MSG_THREAD_TERM, ul);
		}
	}

	delete[] m_ppWorker;
}

bool CCSManager::GetInitCode() {
	return m_InitCode;
}

void CCSManager::NormalTerminate() {
	//_WriteLog(_LOG_LOG, "normal terminate requested.");
	_WriteLog(_LOG_LOG, MSG_NORTM_REQ);

	m_bTerminate = 1;

	StopListen();
}
void CCSManager::SelfTerminate() {
	_WriteLog(_LOG_ERR, "Self terminate requested.");
#ifdef WIN32
			Sleep(1000);
#else
			usleep( 1000000 );
#endif
	m_bTerminate = 1;
	StopListen();
}


void CCSManager::PreTerminate(void) {

#ifndef _OS390

 

#endif

	/*
	 int cnt;

	 //  Wait until all thread Suspended
	 for( unsigned long ul = 0; ul < m_ulNoWThreads; ul++ ) {
	 //printf("    wait thread %d\n", ul );

	 cnt = 0;

	 while( 1 )
	 {
	 if( ! m_ppWorker[ul]->IsUsing() ) {
	 //	        if( m_ppWorker[ul]->Terminated() ) {
	 //				TerminateThreads( ul );
	 break;
	 }

	 if ( cnt++ == 20 ) {
	 printf("Abnormal terminate.\n");
	 break;
	 }

	 #ifdef WIN32
	 _sleep(100);
	 #else
	 usleep(100000);
	 #endif
	 }
	 }
	 */

//	printf("    Terminate all thread\n" );
	RemoveAllQueue();

	Close();
}

long CCSManager::TerminateThreads(unsigned long ulThreadNo) {
	long rc = 1;

	m_ppWorker[ulThreadNo]->Terminate();

	/*
	 if( !m_ppWorker[ulThreadNo]->Terminate() )
	 {
	 char	lpsz[256];
	 sprintf( lpsz, "Thread termination failure. Thread %u", ulThreadNo );
	 _WriteLog( _LOG_LOG, lpsz );
	 rc = 0;
	 }
	 */

	return rc;
}

void CCSManager::OnAccept(int pChild) {

}

int CCSManager::GetNextRequest(unsigned long ulThreadNo) {
	int pChild;

	pChild = popSocket();

#ifdef _DEBUG
	if( pChild != -1 )
	{
		char lpszLogMsg[256];
		/*
		sprintf( lpszLogMsg,
				"Assigning pending request to %uth thread.",
				ulThreadNo );
		sprintf( lpszLogMsg,
				MSG_PENDINGREQ_THREAD,
				ulThreadNo );
		_WriteLog( _LOG_LOG, lpszLogMsg );*/
	}
#endif

	return pChild;
}
void fn1() {
	exit(0);
}
void CCSManager::StartAdminProcess() {
	int delay;
	char msg[200];
	m_sectAdmin.Lock();

	m_AdminRunning = 1;

	for (unsigned long ul = 0; ul < m_ulNoWThreads; ul++) {
		delay = 0;

		if (!m_ppWorker[ul])
			continue;

		while (m_ppWorker[ul]->IsUsing()) {

			if (delay > 1000) { //1000*1000=10초 
				//printf( "wait error.");
				_WriteLog(_LOG_ERR, MSG_WAIT_ERR);
				printf(MSG_WAIT_ERR);
/*				for (unsigned long ul2 = 0; ul2 < m_ulNoWThreads; ul2++) {
					sprintf(msg, "Thread(%d) (%d)  \n", ul2, m_ppWorker[ul2]->IsUsing());
					_WriteLog(_LOG_ERR, msg);
					printf(msg);
				}*/

				SelfTerminate(); //해당로직 넣으면 core 없이 종료.
//				PreTerminate();
				//atexit(fn1);
				
				break;
				
			}
#ifdef WIN32
//			_sleep(10);
			Sleep(10);  //10*100 == 1000=1
			delay++;
#else
			usleep( 1000 );
			delay ++;
#endif
		}
	}
}

void CCSManager::EndAdminProcess() {
	m_AdminRunning = 0;

	m_sectAdmin.Unlock();
}

void CCSManager::CheckAdminProcess(void) {
	m_sectAdmin.Lock();
	m_sectAdmin.Unlock();
}

/*
 void    CCSManager::DisplayConfiguration( void )
 {
 _WriteLog( _LOG_LOG, "Server information" );
 _WriteLog( _LOG_LOG, "    Thread settings" );

 }
 */

void CCSManager::DisplyLogWhenSignal(int sig_num) {
	char msg[100];

	if (!m_bInitial) {
		//sprintf(msg, "SIGNAL - %d : in rule loading.", sig_num);
		sprintf(msg, MSG_SIG_RULEROAD, sig_num);
		_WriteLog(_LOG_ERR, msg);
		return;
	}

	sprintf(msg, "SIGNAL : %d.", sig_num);
	_WriteLog(_LOG_ERR, msg);

	for (unsigned long ul = 0; ul < m_ulNoWThreads; ul++) {
		m_ppWorker[ul]->DisplayLog(sig_num);

//		delete m_ppWorker[ul];

//		m_ppWorker[ul] = new CCSThread( this, ul );
	}
}

void CCSManager::Diagnose() {
	unsigned long sec;

#ifdef WIN32
	time_t ltime;
	time(&ltime);
	sec = (long) ltime;
#else
	struct timeval stamp1;
	gettimeofday(&stamp1, NULL);
	sec = stamp1.tv_sec;
#endif

	for (int i = 0; i < (int) m_ulNoWThreads; i++) {
		if (!m_ppWorker[i])
			continue;

//		if ( m_ppWorker[i]->Suspended() ) continue;
		if (!m_ppWorker[i]->IsUsing())
			continue;

//		printf("Locked\n" );

		if (sec - m_ppWorker[i]->m_startSecond < configRec.ulRunTimeOut)
			continue;

		m_ppWorker[i]->DisplayLogLock();

//		m_ppWorker[i]->Cancel();

//printf("bef ReplyTimeOut\n");

		m_ppWorker[i]->ReplyTimeOut();

//printf("bef delete\n");
		delete m_ppWorker[i];
		m_ppWorker[i] = 0;
//printf("aft delete\n");

		m_ppWorker[i] = new CCSThread(this, i);

//printf("recreated %d\n", m_ppWorker[i]);

	}
}
 

void CCSManager::GetEngineInfo(CBRMObj *pObjParm) {
	long sec;
	int cnt;
	int nWait, nRun;

#ifdef WIN32
	time_t ltime;
	time(&ltime);
	sec = (long) ltime;
#else
	struct timeval stamp1;
	gettimeofday(&stamp1, NULL);
	sec = stamp1.tv_sec;
#endif

	pObjParm->Clear1();
	pObjParm->WriteLong(0);

	cnt = 0;
	for (int i = 0; i < (int) m_ulNoWThreads; i++) {
		if (!m_ppWorker[i])
			continue;

//		if ( m_ppWorker[i]->Suspended() ) {
		if (m_ppWorker[i]->IsUsing()) {
			if (sec - m_ppWorker[i]->m_startSecond > 5)
				cnt++;
		}
	}

	pObjParm->WriteLong(m_ulNoWThreads); //  thread cnt
	pObjParm->WriteLong(cnt); //  Locked thread cnt

	getQueueCount(&nWait, &nRun);
	pObjParm->WriteLong(configRec.ulNoMaxQueueing); //  Queue Max Count
	pObjParm->WriteLong(nWait); //  Queue 대기 Count
	pObjParm->WriteLong(nRun); //  Queue 실행 Count
	pObjParm->WriteLong(m_queueCount 	);	// 큐 개수
	pObjParm->WriteLong(engine_info[0].m_queue_id);			// 큐 아이디
	pObjParm->WriteLong(nWait); 		// Queue 대기 Count
	pObjParm->WriteLong(nRun); 			// Queue 실행 Count

}

void CCSManager::GetQueueInfo(CBRMObj *pObjParm) {  //처음.  23
	long sec;
	int cnt;
	int nWait=0, nRun=0;

#ifdef WIN32
	time_t ltime;
	time(&ltime);
	sec = (long) ltime;
#else
	struct timeval stamp1;
	gettimeofday(&stamp1, NULL);
	sec = stamp1.tv_sec;
#endif
	getQueueCount(&nWait, &nRun);
	
	pObjParm->Clear1();
	pObjParm->WriteLong(0);
	pObjParm->WriteLong(1); //  Queue 실행 Count
	pObjParm->WriteLong(m_ulNoWThreads); 	 // 큐 하나당 들어갈 수 있는 메시지 최대 개수
	pObjParm->WriteLong( 0 );// 리스너 포트 정보
		pObjParm->WriteLong(engine_info[0].m_queue_id);			// 큐 아이디
		pObjParm->WriteLong(engine_info[0].m_thread_count);	// 스레드 풀 크기
		pObjParm->WriteLong(nWait); 		// Queue 대기 Count
		pObjParm->WriteLong(nRun); 			// Queue 실행 Count


} 
  

void CCSManager::GetEngineDetail(CBRMObj *pObjParm) {  //20 
	long sec;
	char PgmId[11], temp[50];
	long RuleId;
	int nRun, nWait;
	
	
#ifdef WIN32
	time_t ltime;
	time(&ltime);
	sec = (long) ltime;
#else
	struct timeval stamp1;
	gettimeofday(&stamp1, NULL);
	sec = stamp1.tv_sec;
#endif

 
	pObjParm->Clear1();
	pObjParm->WriteLong(0);
	pObjParm->WriteLong(m_queueCount);//큐 개수


	
	pObjParm->WriteLong(configRec.nDefaultQueueId);	 //큐 아이디
	pObjParm->WriteLong(m_ulNoWThreads);  // 큐에 할당된 스레드 개수


/*
	printf(" m_queueCount [%d]\n", m_queueCount);
	printf(" configRec.nDefaultQueueId [%d]\n", configRec.nDefaultQueueId);
	printf(" m_ulNoWThreads [%d]\n", m_ulNoWThreads);

	pObjParm->WriteLong(m_ulThreadNo); 	// 스레드 아이디

	getQueueCount(&nWait, &nRun);
	pObjParm->WriteLong(nWait);						// 유휴 스레드 개수
	pObjParm->WriteLong(nRun);						// 활성 스레드 개수
	*/		
	long useMemory;
	for (int i = 0; i < (int) m_ulNoWThreads; i++) {
		pObjParm->WriteLong(m_ppWorker[i]->m_ulThreadNo+100); 	// 스레드 아이디
//		useMemory  = m_ppWorker[i]->GetRunInfo2();
//printf(" m_ppWorker[i]->m_ulThreadNo [%d]  , m_ppWorker[%d]->IsUsing() [%s]\n",m_ppWorker[i]->m_ulThreadNo, i, m_ppWorker[i]->IsUsing()==1 ? "사용중" : "비사용" );


		if (!m_ppWorker[i]) {
			pObjParm->WriteString("Stop"); //	Status
			pObjParm->WriteString(""); //	Run Program
			pObjParm->WriteLong(0);
			pObjParm->WriteString(""); //	Start Timestamp
			pObjParm->WriteString(""); //	Delay Time

			continue;
		}
//printf(" GetEngineDetail  002 [%d] sec[%d][%d]==> (%d) \n",m_ppWorker[i]->IsUsing(),sec,m_ppWorker[i]->m_startSecond,(sec-m_ppWorker[i]->m_startSecond));

//		if ( m_ppWorker[i]->Suspended() ) {
		if ( !m_ppWorker[i]->IsUsing()) {
//printf(" GetEngineDetail  003 \n");
			pObjParm->WriteString("Wait");
			pObjParm->WriteString("");						//	Run Program
			pObjParm->WriteLong(0);             	//룰 id
				sprintf(temp, "%d", sec  );
			pObjParm->WriteString(temp); //	Start Timestamp
			pObjParm->WriteString("0"); //	Delay Time
//			pObjParm->WriteLong(useMemory); //	Delay Time
		} else {
			RuleId = m_ppWorker[i]->GetRunInfo(PgmId);
//printf(" GetEngineDetail  004 \n");

			if (sec - m_ppWorker[i]->m_startSecond <= 5) {
//printf(" GetEngineDetail  005 \n");
				pObjParm->WriteString("Run"); //	Status
				pObjParm->WriteString(PgmId);
				pObjParm->WriteLong(RuleId);
				sprintf(temp, "%d", m_ppWorker[i]->m_startSecond);
				pObjParm->WriteString(temp); //	Start Timestamp
				sprintf(temp, "%d", sec - m_ppWorker[i]->m_startSecond);
				pObjParm->WriteString(temp); //	Delay Time
//			pObjParm->WriteLong(useMemory); //	Delay Time
			} else {
//printf(" GetEngineDetail  006 \n");
				pObjParm->WriteString("Lock"); //	Status
				pObjParm->WriteString(PgmId); //	Run Program
				pObjParm->WriteLong(RuleId);
				pObjParm->WriteString(""); //	Start Timestamp
				sprintf(temp, "%d", sec - m_ppWorker[i]->m_startSecond);
				pObjParm->WriteString(temp); //	Delay Time
//				pObjParm->WriteLong(useMemory); //	Delay Time

			}
		}
	}
}
void CCSManager::GetThrInfo(CBRMObj *pObjParm) {  //20 
	long sec;
	char PgmId[11], temp[50];
	long RuleId;
	int nRun, nWait;
	
	
#ifdef WIN32
	time_t ltime;
	time(&ltime);
	sec = (long) ltime;
#else
	struct timeval stamp1;
	gettimeofday(&stamp1, NULL);
	sec = stamp1.tv_sec;
#endif

 
	pObjParm->Clear1();
	pObjParm->WriteLong(0);
	pObjParm->WriteLong(m_queueCount);//큐 개수


	
	pObjParm->WriteLong(configRec.nDefaultQueueId);	 //큐 아이디
	pObjParm->WriteLong(m_ulNoWThreads);  // 큐에 할당된 스레드 개수


/*
	printf(" m_queueCount [%d]\n", m_queueCount);
	printf(" configRec.nDefaultQueueId [%d]\n", configRec.nDefaultQueueId);
	printf(" m_ulNoWThreads [%d]\n", m_ulNoWThreads);

	pObjParm->WriteLong(m_ulThreadNo); 	// 스레드 아이디

	*/		
	getQueueCount(&nWait, &nRun);
	pObjParm->WriteLong(nWait);						// 유휴 스레드 개수
	pObjParm->WriteLong(nRun);						// 활성 스레드 개수

	long useMemory;
	for (int i = 0; i < (int) m_ulNoWThreads; i++) {
		pObjParm->WriteLong(m_ppWorker[i]->m_ulThreadNo+100); 	// 스레드 아이디
//		useMemory  = m_ppWorker[i]->GetRunInfo2();
//printf(" m_ppWorker[i]->m_ulThreadNo [%d]  , m_ppWorker[%d]->IsUsing() [%s]\n",m_ppWorker[i]->m_ulThreadNo, i, m_ppWorker[i]->IsUsing()==1 ? "사용중" : "비사용" );



//printf(" GetThrInfo  002 [%d] sec[%d][%d]==> (%d) \n",m_ppWorker[i]->IsUsing(),sec,m_ppWorker[i]->m_startSecond,(sec-m_ppWorker[i]->m_startSecond));

///printf(" GetThrInfo  004 [%s][%d]\n",PgmId,RuleId);

//		if ( m_ppWorker[i]->Suspended() ) {
			if (!m_ppWorker[i]) {
				pObjParm->WriteString("Stop"); //	Status
			}else{
				if ( !m_ppWorker[i]->IsUsing()) {
					pObjParm->WriteString("Wait");
				} else {
						if (sec - m_ppWorker[i]->m_startSecond <= 5) {
							pObjParm->WriteString("Run"); //	Status
						} else {
							pObjParm->WriteString("Lock"); //	Status
						}
				}
			}
		  m_ppWorker[i]->GetPgmId(PgmId);
			RuleId = m_ppWorker[i]->GetRunInfo(PgmId);
				pObjParm->WriteString(PgmId); //	Run Program
				pObjParm->WriteLong(RuleId);
				sprintf(temp, "%ld", m_ppWorker[i]->m_startSecond );
				pObjParm->WriteString(temp); //	Delay Time
				if ( !m_ppWorker[i]->IsUsing()) { //wait
	 					sprintf(temp, "%ld"  , 0 );
						pObjParm->WriteString(temp); //	Delay Time
				}else{
			 			sprintf(temp, "%ld"  , (sec-m_ppWorker[i]->m_startSecond) );
						pObjParm->WriteString(temp); //	Delay Time
				}
				 
				pObjParm->WriteLong(0); //	Memory

	}
//	g_pCQryRun->DbAgentStatus(pObjParm);

}

void DiagnoseEngine() {
	g_pManager->Diagnose();
}

/*
 기존 실행되던 Function
 void    CCSManager::GetThreadInfo( CMTParm *pObjParm )
 {
 struct timeval stamp1;
 long   sec;
 char   PgmId[11];
 long   RuleId;

 #ifdef WIN32
 struct _timeb timebuffer;
 _ftime( &timebuffer );

 stamp1.tv_sec = timebuffer.time;
 stamp1.tv_usec = timebuffer.millitm;
 #else
 gettimeofday(&stamp1, NULL) ;
 #endif

 pObjParm->Clear1();
 pObjParm->WriteLong( 0 );

 pObjParm->WriteLong( m_ulNoWThreads );

 for( int i = 0; i < (int)m_ulNoWThreads; i++ )
 {
 if ( ! m_ppWorker[i] ) {
 pObjParm->WriteLong( 999 );
 pObjParm->WriteLong( 0 );
 pObjParm->WriteString( "" );
 pObjParm->WriteLong( 0 );
 continue;
 }

 //		if ( m_ppWorker[i]->Suspended() ) {
 if ( ! m_ppWorker[i]->IsUsing() ) {
 pObjParm->WriteLong( -1 );
 pObjParm->WriteLong( m_ppWorker[i]->m_HitCount );
 pObjParm->WriteString( "" );
 pObjParm->WriteLong( 0 );
 } else {
 sec = stamp1.tv_sec - AcceptTimeSec(m_ppWorker[i]->GetSocketId());
 pObjParm->WriteLong( sec );

 pObjParm->WriteLong( m_ppWorker[i]->m_HitCount );
 RuleId = m_ppWorker[i]->GetRunInfo( PgmId );
 pObjParm->WriteString( PgmId );
 pObjParm->WriteLong( RuleId );
 }
 }

 pObjParm->WriteLong( 0 );   //  DB Session cnt

 pObjParm->WriteLong( getQueueCount() );   //  Queue 대기 Count

 }

 */
