#include    <stdio.h>

#include    <stdlib.h>
#include    <string.h>
#include    <time.h>

#ifndef WIN32
#include    <unistd.h>
#endif

#include    "MTUtil.h"
#include    "MRCSComm.h"
#include    "BREXLog.h"
#include	"MRGBLVAR.h"
#include    "BREXDiag.h"

#ifdef _MSG_KO
#define   MSG_THREAD_FAIL    "쓰레드 시작이 실패하였습니다.\n"
#define   MSG_TERMINATE      "CBREXDiag가 종료됩니다.\n"
#define   MSG_SAVESTA_TERR   "SaveStatistics 오류 (%d) %s\n"
#define   MSG_DIAG_ENGINE    "엔진 확인.\n"
#else
#define   MSG_THREAD_FAIL    "Diagnose thread start failure.\n"
#define   MSG_TERMINATE      "Terminate CBREXDiag\n"
#define   MSG_SAVESTA_TERR   "SaveStatistics Error (%d) %s\n"
#define   MSG_DIAG_ENGINE    "DiagnoseEngine.\n"
#endif

CBREXDiag *g_pCBREXDiag;
int g_Called;

/**************************************************************************************************/
int ConnectLoadDatabase(char *msg);
void DisConnectLoadDatabase();
void DB_Lock();
void DB_Unlock();

CBREXDiag::CBREXDiag() :
		CBREXThreadrun(), CBREXThread(101) {
	if (Start(this)) {
		char lpsz[256];

		//sprintf(lpsz, "Diagnose thread start failure.\n");
		sprintf(lpsz, MSG_THREAD_FAIL);
		throw new CMTException(SYSERR_THREADSTARTFAILURE, lpsz);
	}

}

CBREXDiag::~CBREXDiag() {
	Terminate();

	//printf("Terminate CBREXDiag\n");
	printf(MSG_TERMINATE);
}

void CBREXDiag::Run(void) {
	int nLoop = 0;

	try {
		while (1) {
			//  Sleep  5 Sec 
#ifdef WIN32
			//_sleep(1000);
			Sleep(1000);
#else
//			usleep(1000000);
			sleep(1);
#endif

			if (++nLoop == 600)
				nLoop = 0;

			if (nLoop % 5 == 0)
				Statistics();

			if (configRec.ulDiagIntv > 0) {
				if (nLoop % configRec.ulDiagIntv == 2)
					Diagnose();
//				if ( nLoop % configRec.ulDiagIntv == 5 ) DiagnoseAgent();
			}
		}
	} catch (...) {
		throw new CMTException(SYSERR_SOCKACCEPTFAILURE,
				"Exception occurs in Diagnosis thread.");
	}
}

void CBREXDiag::Statistics(void) {
	int rc;
	char msg[200];

	if (!g_Called)
		return;

	if (configRec.usStatFlag == 0)
		return;

	g_Called = 0;

	DB_Lock();

	rc = ConnectLoadDatabase(msg);
	if (rc == 0) {

		rc = pCMRGblVars->SaveStatistics(msg);
	}

	DB_Unlock();

	if (rc != 0) {
		printf(MSG_SAVESTA_TERR, rc, msg);
	}
}

void DiagnoseEngine();

void CBREXDiag::Diagnose(void) {
	//printf("DiagnoseEngine.\n");
	printf(MSG_DIAG_ENGINE);

	DiagnoseEngine();

}

/*
 void	DiagnoseDbAgent();



 void   CBREXDiag::DiagnoseAgent( void )
 {

 DiagnoseDbAgent();

 }
 */
