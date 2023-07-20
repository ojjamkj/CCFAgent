#ifndef     _INC_MTSOCINC
#define     _INC_MTSOCINC

#if defined(WIN32)
    #include <io.h>
    #include <winsock2.h>

    typedef		int		MTSOCKLEN_T;
#elif defined(_AIX)
    #include <stdlib.h>
    #include <sys/types.h>
    #include <sys/libcsys.h>
    #include <sys/time.h>
    #include <sys/socket.h>
    #include <sys/file.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <arpa/inet.h>

    typedef		socklen_t	MTSOCKLEN_T;
#elif defined(_OS390)
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <stdlib.h>
    #include <sys/time.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <arpa/inet.h>
    #include <unistd.h>

    typedef		socklen_t	MTSOCKLEN_T;
#elif defined(_OS400)
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <stdlib.h>
	#include <sys/time.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <arpa/inet.h>
	#include <unistd.h>

    typedef     int   MTSOCKLEN_T;
#elif defined(__hpux)
    #include <string.h>
    #include <sys/time.h>
    #include <errno.h>
    #include <stdlib.h>
    #include <sys/types.h>
    #include <unistd.h>
    #include <sys/socket.h>
    #include <sys/file.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <sys/wait.h>
    #include <arpa/inet.h>

    typedef		int		MTSOCKLEN_T;
#elif defined(__GNUC__)
    #include <sys/time.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>

    typedef		socklen_t	MTSOCKLEN_T;
#endif

#endif  //_INC_MTSOCINC
 
