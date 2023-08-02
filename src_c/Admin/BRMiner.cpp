// MtsSvr.cpp : Defines the entry point for the console application.
//
#ifdef		_OS390
#pragma     runopts(TERMTHDACT(UADUMP),POSIX(ON))
#endif		//_OS390
#include	<stdio.h>
#include	<string.h>
#include	<signal.h>
#ifdef _WIN32
#include <direct.h>
#include <winsock2.h>
#endif

#include    "MRDEFINE.h"
#include    "MRCSComm.h"
#include    "MRCSMgr.h"

#if !defined(_OS400)
#include	"MTPrfile.h"
#endif

#include    "BREXDiag.h"
#include    "BREXLog.h"
#include    "MRADMIN.h"
#include    "MTUtil.h"
#ifdef _MSG_KO
#define   MSG_SET_ERR1      "SIGPIPE 세팅 불가\n"
#define   MSG_SET_ERR2      "SIGSEGV 세팅 불가\n"
#define   MSG_UNKNOWN_ERR   "알 수 없는 오류.\n\n"

#define   MSG_TM_SIGSEGV      "SIGSEGV에 의해 종료되었습니다.\n"
#define   MSG_TM_SIGBUS       "SIGBUS에 의해 종료되었습니다.\n"
#define   MSG_TERM            "종료되었습니다.\n"
#else
#define   MSG_SET_ERR1      "Could not set SIGPIPE\n"
#define   MSG_SET_ERR2      "Could not set SIGSEGV\n"
#define   MSG_UNKNOWN_ERR   "Unknown error.\n\n"

#define   MSG_TM_SIGSEGV      "Terminated by SIGSEGV.\n"
#define   MSG_TM_SIGBUS       "Terminated by SIGBUS.\n"
#define   MSG_TERM            "Terminated.\n"
#endif

void BuildDataStructure();

#ifdef _OS390
#ifdef __cplusplus
extern "C" {
#endif
	void handler(int);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
	void handler2(int);
#ifdef __cplusplus
}
#endif

void pipe_handler(int sig_num) {
	signal(SIGPIPE, handler);
//    printf("SIG Handler :%d\n", sig_num ); 
}

#else
#include <signal.h>

#ifdef _AIX
void trap_handler(int sig_num) {

//     printf("SIGTRAP Handler :%d\n", sig_num );

	g_pManager->DisplyLogWhenSignal(sig_num);

	exit(0);
}
#endif

#endif

void segv_handler(int sig_num) {
    printf("SIGSEGV Handler :%d\n", sig_num );

	g_pManager->DisplyLogWhenSignal(sig_num);

	//_WriteLog(_LOG_LOG, "Terminated by SIGSEGV.\n");
	_WriteLog(_LOG_LOG, MSG_TM_SIGSEGV);

	signal(SIGSEGV, SIG_DFL);

	//exit(0);   ==>  요거 있으면 core dump 안함

//    signal(SIGSEGV, SIG_IGN);

}

#ifndef _WIN32
void bus_handler(int sig_num) {
//    printf("SIGBUS Handler :%d\n", sig_num );

	g_pManager->DisplyLogWhenSignal(sig_num);

	//_WriteLog( _LOG_LOG,"Terminated by SIGBUS.\n");
	_WriteLog( _LOG_LOG, MSG_TM_SIGBUS);

	signal(SIGBUS, SIG_DFL);
}
#endif


int main(int argc, char* argv[]) {
	char configFileName[20];

#ifdef _OS390

	if (signal(SIGPIPE,pipe_handler) == SIG_ERR) {
		//printf("Could not set SIGPIPE\n");
		printf(MSG_SET_ERR1);
		exit(0);
	}

	if (signal(SIGSEGV,segv_handler) == SIG_ERR) {
		//printf("Could not set SIGSEGV\n");
		printf(MSG_SET_ERR2);
		exit(0);
	}

#else

#ifdef _AIX
	signal(SIGTRAP, trap_handler);
#endif

#ifndef _WIN32
	signal(SIGALRM, SIG_IGN);
	signal(SIGBUS, bus_handler);
#endif

	signal(SIGSEGV, segv_handler);

#endif

	/*
	 struct sigaction sact;

	 sigemptyset(&sact.sa_mask);
	 sact.sa_flags = 0;
	 sact.sa_handler = catcher;
	 sigaction(SIGALRM, &Timer, NULL);

	 alarm(5);
	 */

#ifdef      WIN32
	WSADATA wsaData;
	WSAStartup(0x0101, &wsaData);
#endif      //WIN32

	char strBuffer[200] = { 0, };
	char *pstrBuffer = NULL;
	
	strcpy(configFileName, DEFAULT_INIFILE);  

#if !defined(_OS400)
	if (argc > 1)
		strcpy(configFileName, argv[1]);
#endif

	configParm = new CONFIGPARM();

#if !defined(_OS400)
	configParm->ReadConfigParm(configFileName);
#endif

	printf("ChangeFlow Agent [%d]\n", (int) configRec.ulNoServer);

	try {
		BuildDataStructure();

		g_pManager = new CCSManager(0);

		g_pCMRAdmin = new CMRAdmin();

		g_pCBREXDiag = new CBREXDiag();
		g_pManager->Run();
		g_pManager->PreTerminate();

	} catch (CMTException* ignore) {
		printf("%s\n", ignore->GetEMessage());
		_WriteLog(_LOG_LOG, ignore->GetEMessage());
		ignore->Delete();

	} catch (...) {
		//printf("Unknown error.\n\n");
		printf(MSG_UNKNOWN_ERR);
	}

#ifdef      WIN32
	WSACleanup();
#endif      //WIN32
	//_WriteLog(_LOG_LOG, "Terminated.\n");
	_WriteLog(_LOG_LOG, MSG_TERM);

	delete g_pCBREXLog;

	return 0;

}
