#ifndef     __MTCRITICALSECTION
#define     __MTCRITICALSECTION

#include    "MTThr.h"

class   CMTCriticalSection
{
protected:
#if defined(WIN32)
    CRITICAL_SECTION    m_sect;
#else
    pthread_mutex_t     m_sect;
#endif

public:
    CMTCriticalSection();
    virtual ~CMTCriticalSection();

// Operations
public:
    void    Lock();
    void    Unlock();
};

#endif      //__MTCRITICALSECTION

