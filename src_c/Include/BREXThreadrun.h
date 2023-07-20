#ifndef _INC_BREXTHREADRUN
#define _INC_BREXTHREADRUN

#include    "MTThr.h"

class CBREXThreadrun
{
public:
    CBREXThreadrun();
    virtual ~CBREXThreadrun();

// overrides
//protected:
    virtual void  Run( void );
    
};

#endif
