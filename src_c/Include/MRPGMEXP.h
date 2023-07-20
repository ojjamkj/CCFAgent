#ifndef     _INC_MTPGMEXP
#define     _INC_MTPGMEXP

#include	"MTExcept.h"

class   CMTPgmException : public CMTException
{
public:
    CMTPgmException ( long lCode, const char* lpszMessage );
    ~CMTPgmException();
};

#endif
