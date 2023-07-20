#include	"MTCtSect.h"

CMTCriticalSection::CMTCriticalSection() {
#if defined(WIN32)
	::InitializeCriticalSection (&m_sect);
#elif defined(_OS390)
	pthread_mutex_init( &m_sect, 0 );
#else
	pthread_mutex_init( &m_sect, 0 );
#endif
}

CMTCriticalSection::~CMTCriticalSection() {
#if defined(WIN32)
	::DeleteCriticalSection (&m_sect);
#elif defined(_OS390)
	pthread_mutex_destroy( &m_sect );
#else
	pthread_mutex_destroy( &m_sect );
#endif
}

void CMTCriticalSection::Lock() {
#if defined(WIN32)
	::EnterCriticalSection (&m_sect);
#elif defined(_OS390)
	pthread_mutex_lock( &m_sect );
#else
	pthread_mutex_lock( &m_sect );
#endif
}

void CMTCriticalSection::Unlock() {
#if defined(WIN32)
	::LeaveCriticalSection (&m_sect);
#elif defined(_OS390)
	pthread_mutex_unlock( &m_sect );
#else
	pthread_mutex_unlock( &m_sect );
#endif
}
