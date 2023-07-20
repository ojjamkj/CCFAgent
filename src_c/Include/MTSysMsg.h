#ifndef     _INC_MTSYSMSG
#define     _INC_MTSYSMSG

#include    "MTMsgDef.h"

/*
            THREAD ERROR                                   10000
            SOCKET ERROR                                   20000
            FILE   ERROR                                   30000
*/
#ifdef _MSG_KO
#define	    SYSERR_THREADSTARTFAILURE      ( SYSERR_BASE + 10001 )
#define     ERRMSG_THREADSTARTFAILURE	    "쓰레드 핸들 공간을 생성할 수 없습니다."

#define     SYSERR_COND_INIT_FAILURE       ( SYSERR_BASE + 10002 )
#define     ERRMSG_COND_INIT_FAILURE	    "Condition obj 초기화 실패."

#define     SYSERR_SYNC_INIT_FAILURE       ( SYSERR_BASE + 10003 )
#define     ERRMSG_SYNC_INIT_FAILURE	    "Sync obj 초기화 실패."


/*   SOCKET ERROR    */
#define     SYSERR_SOCKNOTECREATED         ( SYSERR_BASE + 20001 )
#define     ERRMSG_SOCKNOTCREATED           "소켓이 생성되지 않았습니다."

#define     SYSERR_SOCKCONNECTIONFAILURE   ( SYSERR_BASE + 20002 )
#define     ERRMSG_SOCKCONNECTIONFAILURE    "서버 연결 실패."

#define     SYSERR_SOCKNOTCONNECTED        ( SYSERR_BASE + 20003 )
#define     ERRMSG_SOCKNOTCONNECTED         "소켓이 연결되지 않았습니다."

#define     SYSERR_SOCKWRITEFAILURE        ( SYSERR_BASE + 20004 )
#define     ERRMSG_SOCKWRITEFAILURE         "소켓으로 데이터 전송 실패."

#define     SYSERR_SOCKREADFAILURE         ( SYSERR_BASE + 20005 )
#define     ERRMSG_SOCKREADFAILURE          "소켓으로부터 데이터 수신 실패."

#define     SYSERR_SOCKCLOSED              ( SYSERR_BASE + 20006 )
#define     ERRMSG_SOCKCLOSED               "소켓 통신이 종료되었습니다."

#define     SYSERR_SOCKREADRECVTIMEOUT     ( SYSERR_BASE + 20007 )
#define     ERRMSG_SOCKREADRECVTIMEOUT      "데이터 수신 시간 초과"

#define     SYSERR_SOCKINVALIDADDR         ( SYSERR_BASE + 20008 )
#define     ERRMSG_SOCKINVALIDADDR          "유효하지 않은 호스트 주소"

#define     SYSERR_SOCKWRITESENDTIMEOUT    ( SYSERR_BASE + 20009 )
#define     ERRMSG_SOCKWRITESENDTIMEOUT     "데이터 전송 시간 초과."

#define     SYSERR_SOCKBINDFAILURE         ( SYSERR_BASE + 20010 )
#define     ERRMSG_SOCKBINDFAILURE          "Socket bind 실패."

#define     SYSERR_SOCKLISTENFAILURE       ( SYSERR_BASE + 20011 )
#define     ERRMSG_SOCKLISTENFAILURE        "Socket listen 실패."

#define     SYSERR_SOCKACCEPTFAILURE       ( SYSERR_BASE + 20012 )
#define     ERRMSG_SOCKACCEPTFAILURE       "Socket accept 실패."

#define		SYSERR_SHMINITFAILURE			( SYSERR_BASE + 20013 )
#define		ERRMSG_SHMINITFAILURE			"공유 메모리 생성 실패."
 
#define		SYSERR_SHNOPENFAILURE			(SYSERR_BASE + 20014 )
#define		ERRMSG_SHMOPENFAILURE			"공유 메모리 열기 실패."

#define		SYSERR_GETDUPLSOCKFAILURE		(SYSERR_BASE + 20015 )
#define		ERRMSG_GETDUPLSOCKFAILURE		"소켓 복사 실패."

#define		SYSERR_CMOBJFAILURE				(SYSERR_BASE + 20016 )
#define		ERRMSG_CMOBJFAILURE				"CMObject 제어 실패."

#define		SYSERR_OPENPROCESSFAILURE		(SYSERR_BASE + 20017 )
#define		ERRMSG_OPENPROCESSFAILURE		"OpenProcess 호출 실패."

#define		SYSERR_GETMEMORYINFOFAILURE		(SYSERR_BASE + 20018 )
#define		ERRMSG_GETMEMORYINFOFAILURE		"GetMemoryInfo 호출 실패."

#define		SYSERR_STOPTHREADFILEFAILURE	(SYSERR_BASE + 20019 )
#define		ERRMSG_STOPTHREADFILEFAILURE	"Stop Thread 호출 실패."



#define		SYSERR_OPENMAPFILEFAILURE		(SYSERR_BASE + 20050 )
#define		ERRMSG_OPENMAPFILEFAILURE		"Open MapFile 호출 실패."

#define		SYSERR_MAPVIEWOFFILEFAILURE		(SYSERR_BASE + 20051 )
#define		ERRMSG_MAPVIEWOFFILEFAILURE		"MapViewOfFile 호출 실패."

#define		SYSERR_WSASOCKETFAILURE			(SYSERR_BASE + 20052 )
#define		ERRMSG_WSASOCKETFAILURE			"Set WSASocket 호출 실패."

#define		SYSERR_WAITEVENTFAILURE			(SYSERR_BASE + 20053 )
#define		ERRMSG_WAITEVENTFAILURE			"WaitForEvent 호출 실패."

#define		SYSERR_THREADSUSPENDFAILURE		(SYSERR_BASE + 20054 )
#define		ERRMSG_THREADSUSPENDFAILURE		"Thread Suspend 호출 실패."

#define		SYSERR_WSARECVSOCKETFAILURE     (SYSERR_BASE + 20055 )
#define		ERRMSG_WSARECVSOCKETFAILURE		"WSARecvSocket 호출 실패."

#define		SYSERR_WSASENDSOCKETFAILURE		(SYSERR_BASE + 20056 )
#define		ERRMSG_WSASENDSOCKETFAILURE		"WSASendSocket 호출 실패."

#define		SYSERR_MEMORYALLOCATEFAILURE	(SYSERR_BASE + 20057 )
#define		ERRMSG_MEMORYALLOCATEFAILURE	"MemoryAlloc 호출 실패."

#define		SYSERR_WAITFOROBJECTFAILURE		(SYSERR_BASE + 20058 )
#define		ERRMSG_WAITFOROBJECTFAILURE		"GetMemoryInfo 호출 실패."
#else
/*   THREAD ERROR   */
#define	    SYSERR_THREADSTARTFAILURE      ( SYSERR_BASE + 10001 )
#define     ERRMSG_THREADSTARTFAILURE	    "Cannot create space for thread handles."

#define     SYSERR_COND_INIT_FAILURE       ( SYSERR_BASE + 10002 )
#define     ERRMSG_COND_INIT_FAILURE	    "Condition obj init failure."

#define     SYSERR_SYNC_INIT_FAILURE       ( SYSERR_BASE + 10003 )
#define     ERRMSG_SYNC_INIT_FAILURE	    "Sync obj init failure."


/*   SOCKET ERROR    */
#define     SYSERR_SOCKNOTECREATED         ( SYSERR_BASE + 20001 )
#define     ERRMSG_SOCKNOTCREATED           "Socket not created."

#define     SYSERR_SOCKCONNECTIONFAILURE   ( SYSERR_BASE + 20002 )
#define     ERRMSG_SOCKCONNECTIONFAILURE    "Server connection failure."

#define     SYSERR_SOCKNOTCONNECTED        ( SYSERR_BASE + 20003 )
#define     ERRMSG_SOCKNOTCONNECTED         "Socket not connected."

#define     SYSERR_SOCKWRITEFAILURE        ( SYSERR_BASE + 20004 )
#define     ERRMSG_SOCKWRITEFAILURE         "Socket write failure."

#define     SYSERR_SOCKREADFAILURE         ( SYSERR_BASE + 20005 )
#define     ERRMSG_SOCKREADFAILURE          "Socket read failure."
#define     ERRMSG_SOCKREADFAILURE_02          "Socket read failure(02)."
#define     ERRMSG_SOCKREADFAILURE_03          "Socket read failure(03)."
#define     ERRMSG_SOCKREADFAILURE_04          "Socket read failure(04)."

#define     SYSERR_SOCKCLOSED              ( SYSERR_BASE + 20006 )
#define     ERRMSG_SOCKCLOSED               "Socket closed."

#define     SYSERR_SOCKREADRECVTIMEOUT     ( SYSERR_BASE + 20007 )
#define     ERRMSG_SOCKREADRECVTIMEOUT      "Data receive timeout."

#define     SYSERR_SOCKINVALIDADDR         ( SYSERR_BASE + 20008 )
#define     ERRMSG_SOCKINVALIDADDR          "Invalid host address."

#define     SYSERR_SOCKWRITESENDTIMEOUT    ( SYSERR_BASE + 20009 )
#define     ERRMSG_SOCKWRITESENDTIMEOUT     "Data send timeout."

#define     SYSERR_SOCKBINDFAILURE         ( SYSERR_BASE + 20010 )
#define     ERRMSG_SOCKBINDFAILURE          "Socket bind failure."

#define     SYSERR_SOCKLISTENFAILURE       ( SYSERR_BASE + 20011 )
#define     ERRMSG_SOCKLISTENFAILURE        "Socket listen failure."

#define     SYSERR_SOCKACCEPTFAILURE       ( SYSERR_BASE + 20012 )
#define     ERRMSG_SOCKACCEPTFAILURE       "Socket accept failure."

#define		SYSERR_SHMINITFAILURE			( SYSERR_BASE + 20013 )
#define		ERRMSG_SHMINITFAILURE			"Shared Memory Creation failure."

#define		SYSERR_SHNOPENFAILURE			(SYSERR_BASE + 20014 )
#define		ERRMSG_SHMOPENFAILURE			"Open Shared Memory failure."

#define		SYSERR_GETDUPLSOCKFAILURE		(SYSERR_BASE + 20015 )
#define		ERRMSG_GETDUPLSOCKFAILURE		"Duplication Socket failure."

#define		SYSERR_CMOBJFAILURE				(SYSERR_BASE + 20016 )
#define		ERRMSG_CMOBJFAILURE				"CMObject Control failure."

#define		SYSERR_OPENPROCESSFAILURE		(SYSERR_BASE + 20017 )
#define		ERRMSG_OPENPROCESSFAILURE		"OpenProcess Call failure."

/*사용하지 않음 2014-12-12 확인
#define		SYSERR_GETMEMORYINFOFAILURE		(SYSERR_BASE + 20018 )
#define		ERRMSG_GETMEMORYINFOFAILURE		"GetMemoryInfo Call failure."

#define		SYSERR_STOPTHREADFILEFAILURE	(SYSERR_BASE + 20019 )
#define		ERRMSG_STOPTHREADFILEFAILURE	"Stop Thread Call failure."

#define		SYSERR_OPENMAPFILEFAILURE		(SYSERR_BASE + 20050 )
#define		ERRMSG_OPENMAPFILEFAILURE		"Open MapFile Call failure."

#define		SYSERR_MAPVIEWOFFILEFAILURE		(SYSERR_BASE + 20051 )
#define		ERRMSG_MAPVIEWOFFILEFAILURE		"MapViewOfFile Call failure."

#define		SYSERR_WSASOCKETFAILURE			(SYSERR_BASE + 20052 )
#define		ERRMSG_WSASOCKETFAILURE			"Set WSASocket Call failure."

#define		SYSERR_WAITEVENTFAILURE			(SYSERR_BASE + 20053 )
#define		ERRMSG_WAITEVENTFAILURE			"WaitForEvent Call failure."

#define		SYSERR_THREADSUSPENDFAILURE		(SYSERR_BASE + 20054 )
#define		ERRMSG_THREADSUSPENDFAILURE		"Thread Suspend Call failure."

#define		SYSERR_WSARECVSOCKETFAILURE     (SYSERR_BASE + 20055 )
#define		ERRMSG_WSARECVSOCKETFAILURE		"WSARecvSocket Call failure."

#define		SYSERR_WSASENDSOCKETFAILURE		(SYSERR_BASE + 20056 )
#define		ERRMSG_WSASENDSOCKETFAILURE		"WSASendSocket Call failure."


*/

#define		SYSERR_MEMORYALLOCATEFAILURE	(SYSERR_BASE + 20057 )
#define		ERRMSG_MEMORYALLOCATEFAILURE	"MemoryAlloc Call failure."
#define		ERRMSG_MEMORYALLOCATEFAILURE_01	"MemoryAlloc Call failure(01)."
#define		ERRMSG_MEMORYALLOCATEFAILURE_02	"MemoryAlloc Call failure(02)."
#define		ERRMSG_MEMORYALLOCATEFAILURE_03	"MemoryAlloc Call failure(03)."
#define		ERRMSG_MEMORYALLOCATEFAILURE_04	"MemoryAlloc Call failure(04)."
#define		ERRMSG_MEMORYALLOCATEFAILURE_05	"MemoryAlloc Call failure(05)."
#define		ERRMSG_MEMORYALLOCATEFAILURE_06	"MemoryAlloc Call failure(06)."
#define		ERRMSG_MEMORYALLOCATEFAILURE_07	"MemoryAlloc Call failure(07)."
#define		ERRMSG_MEMORYALLOCATEFAILURE_08	"MemoryAlloc Call failure(08)."
#define		ERRMSG_MEMORYALLOCATEFAILURE_09	"MemoryAlloc Call failure(09)."
#define		ERRMSG_MEMORYALLOCATEFAILURE_10	"MemoryAlloc Call failure(10)."
#define		ERRMSG_MEMORYALLOCATEFAILURE_11	"MemoryAlloc Call failure(11)."
#define		ERRMSG_MEMORYALLOCATEFAILURE_12	"MemoryAlloc Call failure(12)."
#define		ERRMSG_MEMORYALLOCATEFAILURE_13	"MemoryAlloc Call failure(13)."
#define		ERRMSG_MEMORYALLOCATEFAILURE_14	"MemoryAlloc Call failure(14)."
#define		ERRMSG_MEMORYALLOCATEFAILURE_15	"MemoryAlloc Call failure(15)."
#define		ERRMSG_MEMORYALLOCATEFAILURE_16	"MemoryAlloc Call failure(16)."
#define		ERRMSG_MEMORYALLOCATEFAILURE_17	"MemoryAlloc Call failure(17)."
#define		ERRMSG_MEMORYALLOCATEFAILURE_18	"MemoryAlloc Call failure(18)."
#define		ERRMSG_MEMORYALLOCATEFAILURE_19	"MemoryAlloc Call failure(19)."
#define		ERRMSG_MEMORYALLOCATEFAILURE_20	"MemoryAlloc Call failure(20)."
#endif

/*사용하지 않음 2014-12-12 확인
#define		SYSERR_WAITFOROBJECTFAILURE		(SYSERR_BASE + 20058 )
#define		ERRMSG_WAITFOROBJECTFAILURE		"GetMemoryInfo Call failure."

#define		SYSERR_SENDDATAERRORTOQUEUE		(SYSERR_BASE + 20059 )
#define		SYSERR_READDATAERRORQUEUE		(SYSERR_BASE + 20060 )
*/
#endif
