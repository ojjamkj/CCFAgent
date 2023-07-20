#include    "MRPGMEXP.h"

CMTPgmException::CMTPgmException(long lCode, const char* lpszMessage) :
		CMTException(lCode, lpszMessage) {
}

CMTPgmException::~CMTPgmException() {
}

