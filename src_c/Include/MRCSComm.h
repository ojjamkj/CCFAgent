#ifndef     _INC_MRCSCOMM
#define     _INC_MRCSCOMM

#if defined(_OS390)
	#pragma     runopts(TERMTHDACT(UADUMP),POSIX(ON))

    #ifndef _OPEN_THREADS
       #define     POSIX_SOURCE
       #define     _XOPEN_SOURCE_EXTENDED      1
       #define     _OPEN_THREADS
    #endif
#endif

#include 	<time.h>
#include 	<errno.h>

#include    "MRERROR.h"
#include    "MTSysMsg.h"
#include    "MTExcept.h"
#include    "MRConfig.h"
#include    "MItem.h"
#if ! defined(WIN32)
	#define MAX_PATH          200
#endif

#endif      //_INC_MRCSCOMM
