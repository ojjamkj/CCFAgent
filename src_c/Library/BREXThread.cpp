#include <stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<errno.h>
#if defined(_AIX)
#include	<strings.h>
#endif
#include    <time.h>
#ifndef WIN32
#include    <sys/time.h>
#endif

#include	"MTMsgDef.h"
#include	"MTExcept.h"
#include	"BREXThread.h"

#ifdef _MSG_KO
#define   MSG_THREAD_TERM_FAIL       "스레드 종료 실패. Thread %u"
#define   MSG_NOT_RESUME_THREAD      "스레드를 다시 점유할 수 없습니다. rc = %d.\n"
#define   MSG_THREAD_LOCK_ERR        "pthread_mutex_lock() 오류"
#define   MSG_THREAD_UNLOCK_ERR      "pthread_mutex_unlock() 오류"
#define   MSG_JOIN_ERR               "JOIN 오류\n"
#define   MSG_THREAD_CANCEL          "스레드 취소 %d\n"
#define   MSG_SUCC_RETURN            "성공 반환\n"
#define   MSG_FAIL_RETURN            "실패 반환 \n"
#else
#define   MSG_THREAD_TERM_FAIL       "Thread termination failure. Thread %u"
#define   MSG_NOT_RESUME_THREAD      "Can not Resume Thread. rc = %d.\n"
#define   MSG_THREAD_LOCK_ERR        "pthread_mutex_lock() error"
#define   MSG_THREAD_UNLOCK_ERR      "pthread_mutex_unlock() error"
#define   MSG_JOIN_ERR               "JOIN ERROR\n"
#define   MSG_THREAD_CANCEL          "Thread Cancel %d\n"
#define   MSG_SUCC_RETURN            "SUCC RETURN\n"
#define   MSG_FAIL_RETURN            "FAIL RETURN \n"
#endif

CBREXThread::CBREXThread(unsigned long ulThreadNo) {
	m_ulThreadNo = ulThreadNo;
	m_bSuspended = 0;
	m_bTerminate = 0;
}

CBREXThread::~CBREXThread() {
	if (m_bTerminate)
		return;

	if (!Terminate()) {
		char lpsz[256];
		//sprintf(lpsz, "Thread termination failure. Thread %u", m_ulThreadNo);
		sprintf(lpsz, MSG_THREAD_TERM_FAIL, m_ulThreadNo);
		printf(lpsz);

//		WriteLog( SYSERR_, lpsz );
	}
}

unsigned long CBREXThread::Start(void *pParent) {
	unsigned long rc;

//	MTTHREAD_T			d_thread;

	m_pParent = pParent;

#if defined(WIN32)
	m_thread = (MTTHREAD_T) _beginthreadex(NULL, 0, RunThread,
			(void *) m_pParent, NULL, &m_dwThreadId);
	if (m_thread)
		rc = 0;
	else
		rc = -1;

#elif defined(_OS390) || defined(_AIX) || defined(__GNUC__)
	if (pthread_mutex_init(&m_thread_sync, NULL) != 0) {
		perror("pthread_mutex_init() error");
		return(1);
	}
	if (pthread_cond_init(&m_thread_cond, NULL) != 0) {
		perror("pthread_cond_init() error");
		return(2);
	}

	rc = pthread_create( &m_thread,
			NULL,
			RunThread,
			(void *) m_pParent );

#elif defined(__hpux) || defined(_OS400)          
	rc = pthread_create( &m_thread,
			NULL,
			RunThread,
			(void *) m_pParent );
#else
#error	This class is supported by HPUX, AIX, OS390, GNUC, WIN32 only.
#endif

	return rc;
}

unsigned char CBREXThread::Resume(void) {
	int rc;

	if (!m_bSuspended)
		return 0;

	m_bSuspended = 0;

#if defined(WIN32)	
	if (ResumeThread(m_thread) == 0xFFFFFFFF) {
		rc = -1;
#elif defined(_AIXXXX)
		if ( ( rc = pthread_continue_np ( m_thread )) != 0 ) {
#elif defined(_OS390) || defined(_AIX) || defined(__GNUC__)
			if ( ( rc = pthread_cond_signal(&m_thread_cond)) != 0 ) {
#elif defined(__hpux) || defined(_OS400)		
				if ( pthread_continue ( m_thread ) != 0 ) {
#else
#error	This class is supported by HPUX, AIX, OS390, GNUC, WIN32 only.
#endif
		m_bSuspended = 1;
		char errMsg[50];
		//sprintf(errMsg, "Can not Resume Thread. rc = %d.\n", rc);
		sprintf(errMsg, MSG_NOT_RESUME_THREAD, rc);
		throw new CMTException(SYSERR_, errMsg);
	}

	return 1;
}

int CBREXThread::Suspend(unsigned char mode) {
	int rc = 1;

// printf("Thread %2d waiting.\n", m_ulThreadNo ); 

#if defined(WIN32)
	m_bSuspended = 1;
	if (SuspendThread(m_thread) == 0xFFFFFFFF)
		rc = 0;
#elif defined(_AIXXXX)
	if ( pthread_suspend_np ( m_thread ) !=0 ) rc = 0;
#elif defined(_OS390) || defined(_AIX) || defined(__GNUC__)
	if (pthread_mutex_lock( &m_thread_sync ) != 0) {
		//printf("pthread_mutex_lock() error");
		printf(MSG_THREAD_LOCK_ERR);
		return 0;
	}

	m_bSuspended = 1;
	if ( pthread_cond_wait(&m_thread_cond, &m_thread_sync) != 0 ) rc = 0;
	m_bSuspended = 0;

	if (pthread_mutex_unlock( &m_thread_sync ) != 0) {
		//printf("pthread_mutex_unlock() error");
		printf(MSG_THREAD_UNLOCK_ERR);
		rc = 0;
	}

#elif defined(__hpux) || defined(_OS400)
	m_bSuspended = 1;
	if ( pthread_suspend ( m_thread ) !=0 ) rc = 0;
#else
#error	This class is supported by HPUX, AIX, OS390, GNUC, WIN32 only.
#endif

// printf("Thread %2d running.\n", m_ulThreadNo );

	return rc;
}

/*
 void     CBREXThread::SuspendReset()
 {
 m_bSuspended = 1;
 }
 */

unsigned char CBREXThread::Terminate(void) {
	unsigned char rc=0;

#if defined(WIN32)

	if (TerminateThread(m_thread, 0) == TRUE) {
		rc = CloseHandle(m_thread);
	}

//	rc = CloseHandle( m_thread );

//	printf("CloseHandle = %d\n", rc );

//    _endthreadex( m_dwThreadId );    작동안함

#else
//	 void   *ret = NULL;
//     pthread_exit( ret );

	int nStatus;
	void * vpResult = NULL;

//    printf("STEP 1\n");

	pthread_cancel(m_thread);

//    printf("STEP 2\n");

	if( (nStatus = pthread_join(m_thread, &vpResult)) != 0)
	{
		//printf("JOIN ERROR\n");
		printf(MSG_JOIN_ERR);
		return 0;
	}
	/*
	 if (vpResult == NULL)
	 printf("SUCC RETURN\n");
	 else
	 printf("FAIL RETURN \n");
	 */

	rc = 1;
#endif

	m_bTerminate = 1;

	return rc;
}

unsigned char CBREXThread::Cancel(void) {
	//printf("Thread Cancel %d\n", m_ulThreadNo);
	printf(MSG_THREAD_CANCEL, m_ulThreadNo);

#if defined(WIN32)

	_endthreadex (m_dwThreadId);

//	unsigned long dwExitCode;
//	BOOL rc = TerminateThread( m_thread, DWORD dwExitCode );

#else
	int nStatus;
	void * vpResult = NULL;

	pthread_cancel(m_thread);
	printf("STEP 2\n");

	if( (nStatus = pthread_join(m_thread, &vpResult)) != 0)
	{
		//printf("JOIN ERROR\n");
		printf(MSG_JOIN_ERR);
		return 0;
	}
	if (vpResult == NULL){
		//printf("SUCC RETURN\n");
		printf(MSG_SUCC_RETURN);
	}else{
		//printf("FAIL RETURN \n");
		printf(MSG_FAIL_RETURN);
	}
#endif

	return 0;
}

/*
 unsigned char	CBREXThread::Suspended( void )
 {
 return m_bSuspended;
 }
 */

unsigned char CBREXThread::Terminated(void) {
	return m_bTerminate;
}

#if defined(_AIX_GCC) /* g++ */
	void* CBREXThread::RunThread(void *p)
#elif defined(_AIX) || defined(_OS390)
	void* __cdecl CBREXThread::RunThread( void *p)
#elif defined(__hpux) || defined(__GNUC__) || defined(_OS400)
void* CBREXThread::RunThread(void *p)
#elif defined(WIN32)
		unsigned __stdcall CBREXThread::RunThread( void *p )
#else
#error	This program supports AIX, OS390, HPUX, GNUC, WIN32 only
#endif
		{

	CBREXThreadrun* pWThread = (CBREXThreadrun*) p;

	pWThread->Run();

	return 0;
}

