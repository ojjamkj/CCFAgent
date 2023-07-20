#ifndef     _INC_MTTHR
#define     _INC_MTTHR

#if defined(WIN32)
    #include    <windows.h>
    #include    <process.h>
#elif defined(_OS390)
	#ifndef _OPEN_THREADS
		#define _OPEN_THREADS
	#endif

    #include    <pthread.h>         
#else                               
    #include    <pthread.h>         
#endif                                  

#if defined(_AIX_GCC)    /* g++  */
	typedef		pthread_t			MTTHREAD_T;
	typedef		pthread_attr_t      MTTATTR_P;
	typedef		pthread_cond_t		MTTHREAD_COND_T;
	typedef		pthread_mutex_t		MTTHREAD_SYNC_T;
	typedef		void* (*MTTHREADPROC)(void*);
#elif defined(_AIX) || defined(_OS390)
	typedef		pthread_t			MTTHREAD_T;
	typedef		pthread_cond_t		MTTHREAD_COND_T;
	typedef		pthread_mutex_t		MTTHREAD_SYNC_T;
	typedef		void* (__cdecl *MTTHREADPROC)( void* );
#elif defined(__hpux) || defined(_OS400)  || defined(__GNUC__)
	typedef		pthread_t			MTTHREAD_T;
	typedef		pthread_attr_t      MTTATTR_P;
	typedef		pthread_cond_t		MTTHREAD_COND_T;
	typedef		pthread_mutex_t		MTTHREAD_SYNC_T;
	typedef		void* (*MTTHREADPROC)( void* );
#elif defined(WIN32)
	typedef		HANDLE			MTTHREAD_T;
	typedef		HANDLE			MTTHREAD_COND_T;
	typedef		CRITICAL_SECTION	MTTHREAD_SYNC_T;
	typedef		unsigned (__stdcall *MTTHREADPROC)( void* );	
#else
	#error	This class is supported by HPUX, AIX, OS390, GNUC, WIN32 only.
#endif

#endif      //_INC_MTTHR

