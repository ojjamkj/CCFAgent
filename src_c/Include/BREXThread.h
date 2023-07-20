#ifndef     _INC_BREXTHREAD
#define     _INC_BREXTHREAD

#include	"BREXThreadrun.h"

class   CBREXThread 
{
private:
    unsigned long			m_ulThreadNo;

	MTTHREAD_T              m_thread;
	MTTHREAD_COND_T			m_thread_cond;
	MTTHREAD_SYNC_T			m_thread_sync;
	unsigned char			m_bSuspended;
	unsigned char			m_bTerminate;

	void					*m_pParent;

#ifdef WIN32
	unsigned m_dwThreadId;
#endif


public:
    CBREXThread (unsigned long ulThreadNo );
    ~CBREXThread();

	unsigned long	Start(  void * );
    unsigned char	Resume( void );
    int 			Suspend(unsigned char mode = 0 );
//	void			SuspendReset();
	unsigned char	Terminate( void );
	unsigned char	Cancel( void );

//	unsigned char	Suspended(void);
	unsigned char	Terminated(void);

//protected:
//    virtual void    Run( void );

private:
	#if defined(_AIX_GCC) /* g++ */
		static void*					RunThread(void *);
	#elif defined(_AIX) || defined(_OS390)
		static void* __cdecl			RunThread( void * );
	#elif defined(__hpux) || defined(__GNUC__)
		static void*					RunThread( void * );
	#elif defined(WIN32)
		static unsigned  __stdcall	RunThread( void * );
	#else
		#error	This program supports AIX, OS390, HPUX, GNUC, WIN32 only
	#endif

};

#endif

