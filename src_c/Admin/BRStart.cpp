#include    <stdio.h>
#include	<string.h>

#if defined(_AIX)
#include	<strings.h>
#endif

#ifndef   _OS390
#include  <malloc.h>
#endif

#include    <time.h>

#if defined(_OS390)
#include	<sys/time.h>
#endif
#if defined(WIN32)
#include	<sys/timeb.h>
#endif

//#include	"BRMObj.h"
#include "BRLine.h"
#include    "MTUtil.h"
#include    "MTSysMsg.h"

#include    "MRConfig.h"
#include    "MRCSMgr.h"
#include	"MRPGMEXP.h"
#include    "MRCSThr.h"
#include    "MRERROR.h"
#include    "MRDTLIST.h"
#include    "MRRUNNER.h"
#include	"MRGBLVAR.h"
#include    "MRADMIN.h"
#include    "BREXLog.h"
#include    "CFAPI.h"
#ifdef _WIN32
	#include <direct.h>
#else
	#include <dirent.h>
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
 
#ifdef _MSG_KO
#define   MSG_LISTENER_TIMEOUT    "리스너 시간 초과(m_ulThreadNo=%d).\n"
#define   MSG_SOCKET_CLOSE        "%d로부터 소켓 종료를 요청받았습니다.\n"
#else
#define   MSG_LISTENER_TIMEOUT    "Listener timeout(m_ulThreadNo=%d).\n"
#define   MSG_SOCKET_CLOSE        "Socket close requested from client : %d\n"
#endif

extern int g_Called;
 
long CCSThread::ProcInterval() {

	long sec, usec1, usec2;

#ifdef WIN32
	struct _timeb timebuffer;
	_ftime(&timebuffer);

	sec = (long) timebuffer.time- ((CCSManager*) m_pManager)->AcceptTimeSec(m_pChildSoc);
	usec2 = sec * 1000 + timebuffer.millitm;
//	usec2 = sec * 1000000 + timebuffer.millitm;	 //2017.05.04  :????????? ??? o?????? ????????? ???ð?(? ???ð?):o?????????? ?? o?? ???? ?ð?(o???ð?):
	usec1 = ((CCSManager*) m_pManager)->AcceptTimeUSec(m_pChildSoc);

#else
	struct timeval stamp1;

	gettimeofday(&stamp1, NULL);
	sec = stamp1.tv_sec - ((CCSManager*)m_pManager)->AcceptTimeSec(m_pChildSoc);//tv_usec;
        //       sec*1000000+tv_usec/1000 = 1/1000
	usec2 = (sec*1000000+stamp1.tv_usec)/1000;
        //       1000000/1000 = 1/1000
	usec1 = ((CCSManager*)m_pManager)->AcceptTimeUSec(m_pChildSoc)/1000; //2017.05.04
	// ( current sec - accept time sec )*1000000+(current tv_usec/1000) = 

        
#endif

//printf("usec1  %ld %ld \n",usec1 ,usec2 );

	return usec2 - usec1;
}

void CCSThread::ProcessRequest() {
	int inLeng;
	int rc;
	unsigned char Gbn;
	memset(m_ModuleName,0x00,21);
	int keepYN = configRec.ulKeepFlag;

	if (configRec.usStatFlag || configRec.usDispTm)
		m_bLogTime = 1;
	else
		m_bLogTime = 0;

	while (1) {

		m_bUsing = 0;

		((CMRDataList*) pCMRDataList)->setItemValid(false);

		if (((CCSManager*) m_pManager)->m_bTerminate)
			break;
		if (((CCSManager*) m_pManager)->m_AdminRunning) {
#ifdef WIN32
//			_sleep(10); // 0.01 Second
			Sleep(10);
#else
					usleep(10000); // 0.01 Second
#endif
			continue;
		}

#ifdef WIN32
		time_t ltime;
		time(&ltime);
		m_startSecond = (long) ltime;
#else
		struct timeval stamp1;
		gettimeofday(&stamp1, NULL);
		m_startSecond = stamp1.tv_sec;
#endif

		if (m_bLogTime) {
			delay0 = ProcInterval();
		}

		/****  READ HEADER 50 BYTE  ****************************************************/
		m_ObjHeader->Clear();

		rc = ((CCSManager*) m_pManager)->SocRead(m_pChildSoc,(unsigned char *) m_ObjHeader->m_Buffer, 50);
		//printf("rc = %d\n", rc );
		if (rc != 0) {

			switch (rc) {
			case SYSERR_SOCKREADRECVTIMEOUT:
#ifdef _DEBUG
				//printf("Listener timeout(m_ulThreadNo=%d).\n", m_ulThreadNo );
				printf(MSG_LISTENER_TIMEOUT, m_ulThreadNo );
#endif
				if (!keepYN){
					//BRS160TB.RS16_1=1004 AND RS16_3 <> 1 아니면 리턴 (대기하지 않음) 해당 경우 엔진이 다운되는 현상 발생.(2017.09.11)
					return;
				}
				break;

			case SYSERR_SOCKCLOSED:
			case SYSERR_SOCKREADFAILURE:
#ifdef _DEBUG
				//printf("Socket close requested from client : %d\n", rc );
///				printf(MSG_SOCKET_CLOSE, rc );
#endif
				return;
				/*
				 case SYSERR_SOCKREADFAILURE :
				 #ifdef _DEBUG
				 printf("Socket read failure : %d\n", rc );
				 #endif
				 return;
				 */
			default:
				break;
			}

			continue;
		}

		/*  Memory Test 용
		 m_ObjBuffer.Clear1();
		 m_ObjBuffer.WriteLong(MRC_INPUT_ERR);
		 m_ObjBuffer.WriteLong(UERR_PGMNFOUND);
		 m_ObjBuffer.WriteString("Memory Test.");

		 ReplyResultSet();

		 if ( keepYN != 1 ) break;
		 else continue;
		 */

		m_bUsing = 1;

		/*********************************************************************/
		m_ObjHeader->setLength(50);

		inLeng = m_ObjHeader->ReadDataLength(); //  0,1,2

		m_ObjHeader->ReadByte(); //  3  : pass

		Gbn = m_ObjHeader->ReadByte(); //  4

		m_parmMode = m_ObjHeader->ReadByte(); //  5

		if (Gbn != 0) {
			m_ObjBuffer.Clear1();
			m_ObjBuffer.WriteLong(1);
			m_ObjBuffer.WriteLong(-1);
			m_ObjBuffer.WriteString("Invalid process mode.");

			ReplyResultSet();

			break;
		}

		if (inLeng < 0 || inLeng >= (long) m_MaxPramSize) {
			m_ObjBuffer.Clear1();
			m_ObjBuffer.WriteLong(1);
			m_ObjBuffer.WriteLong(-1);
			m_ObjBuffer.WriteString("Data size error.");

			ReplyResultSet();

			break;
		}

		if (configRec.usStatFlag) {
			strncpy(m_ModuleName, (char*) m_ObjHeader->m_Buffer + 30, 10);
			m_ModuleName[10] = 0;
			McRTrim (m_ModuleName);
		}
		/*2015.01.7 이철민 추가: 로그에 호출된 서버IP세팅 해당부분은  AP에서 호출하는RAPI것이 아니라 클라이언트에서 호출시 클라이언트 IP 설정
		 C/JAVA 프로그램의 StartIP함수를 통해 설정되는것..
		 */
//		strncpy(m_StartIP, (char*) m_ObjHeader->m_Buffer + 8, 15);
//		m_StartIP[15] = 0;
//		McRTrim (m_StartIP);
//printf("CCSThread::ProcessRequest  60 m_StartIP[%s] \n", m_StartIP);

	//	if ( pCMRDataList )
	//	  ((CMRDataList*) pCMRDataList)->setStartIp(m_StartIP);
	 
		if (inLeng > 0) {
			rc = ((CCSManager*) m_pManager)->SocRead(m_pChildSoc,(unsigned char *) m_ObjBuffer.m_Buffer, inLeng);

			if (rc != 0)
				throw new CMTException(rc, "Socket read failure(01).");

			m_ObjBuffer.setLength(inLeng);
		}

		m_HitCount++;
		if (MainProcess(m_StartIP) != 0)
			ReplyResultSet();

#ifdef _TRACE
		((CMRDataList*)pCMRDataList)->TermTrace();
#endif

		if (keepYN != 1)
			break;
	}

	m_bUsing = 0;
}
void listDirectories(const char* path) {
	
}
long CCSThread::MainProcess(char *startIp) {
	char PgmId[100];
	bool bTrace = 0;	
	long rc, rr;
	int i;
	short m_itemCnt=0;
	char msg[500];
	char path[1000];
	m_ObjBuffer.ReadString(PgmId);
	printf("PgmId [%s] \n", PgmId);

	CFAPI cfapi;
	if (strcmp(PgmId, "3") == 0) {
		cfapi.API03_PING(&m_ObjBuffer, m_itemCnt, m_pChildSoc, (CCSManager*)m_pManager);
		ReplyResultSet();
		return 0;
	}
	if (strcmp(PgmId, "4") == 0) {
		cfapi.API04_VIEWDIR(&m_ObjBuffer, m_itemCnt, m_pChildSoc, (CCSManager*)m_pManager);
		ReplyResultSet();
		return 0;
	}
	if (strcmp(PgmId, "6") == 0) {
		cfapi.API06_VIEWFILE6(&m_ObjBuffer, m_itemCnt, m_pChildSoc, (CCSManager*)m_pManager);
		ReplyResultSet(); 
		return 0;
	}
	if (strcmp(PgmId, "7") == 0) {
		cfapi.API07_CREATEFILE(&m_ObjBuffer, m_itemCnt, m_pChildSoc,(CCSManager*)m_pManager);
		ReplyResultSet();
		return 0;
	}
	if (strcmp(PgmId, "15") == 0) {
		cfapi.API15_BUILD(&m_ObjBuffer, m_itemCnt, m_pChildSoc,(CCSManager*)m_pManager);
		ReplyResultSet();
		return 0;
	}
	if (strcmp(PgmId, "26") == 0) {
		cfapi.API26_DOSEARCH_ONLY_DIR(&m_ObjBuffer, m_itemCnt, m_pChildSoc, (CCSManager*)m_pManager);
		ReplyResultSet();
		return 0;
	}
	if (strcmp(PgmId, "27") == 0) {
		cfapi.API27_DOSEARCH_ONLY_FILE(&m_ObjBuffer, m_itemCnt, m_pChildSoc, (CCSManager*)m_pManager);
		ReplyResultSet();
		return 0;
	}
	if (strcmp(PgmId, "41") == 0) {
		cfapi.API27_DOSEARCH_ONLY_FILE_COLLECT(&m_ObjBuffer, m_itemCnt, m_pChildSoc, (CCSManager*)m_pManager);
		ReplyResultSet();
		return 0;
	}

	if (strcmp(PgmId, "28") == 0) {
		cfapi.API28_DELETEFILE(&m_ObjBuffer, m_itemCnt, m_pChildSoc, (CCSManager*)m_pManager);
		ReplyResultSet();
		return 0;
	}
	if (strcmp(PgmId, "40") == 0) {
		cfapi.API40_SCANDIR_TO_FILE(&m_ObjBuffer, m_itemCnt, m_pChildSoc, (CCSManager*)m_pManager);
		ReplyResultSet();
		return 0;
	}

	/*
	if (strcmp(PgmId, "26") == 0) {
		cfapi.API01(&m_ObjBuffer, m_itemCnt, m_pChildSoc,(CCSManager*)m_pManager);
		ReplyResultSet();
		return 0;
	}
	if (strcmp(PgmId, "27") == 0) {
		cfapi.API27(&m_ObjBuffer, m_itemCnt, m_pChildSoc, (CCSManager*)m_pManager);
		ReplyResultSet();
		return 0;
	}
	if (strcmp(PgmId, "100") == 0) {
		cfapi.API100(&m_ObjBuffer, m_itemCnt, m_pChildSoc, (CCSManager*)m_pManager);
	//	ReplyResultSet();
		return 0;
	}*/

	PGM_LIST *pPgmNode;
	memset(PgmId,0x00,21);
	try {
		switch (m_parmMode) {
		case 0:
			m_ObjBuffer.ReadString(PgmId);
			break;
		case 1:
			strncpy(PgmId, (char*) m_ObjBuffer.m_Buffer, 10); // 확인要
//					strncpy( PgmId, (char*) m_ObjBuffer.m_Buffer+3, 10 );
			McRTrim(PgmId);
			break;
		case 2:
			m_ObjBuffer.ReadString(PgmId);
			break;
		default:
			break;
		}
		PgmId[10] = 0;
		strncpy(m_ModuleName,PgmId,10);
		m_ModuleName[10]=0;
//		m_parmMode = 2; //현대제철 프로젝트에서 임의로 2지정 2을 사용하지 않는 경우가 없음.혹시 사용자의 실수를 배제하고자함.
		if (!(pPgmNode = pCMRGblVars->getPgmNode(PgmId))) {
			sprintf(m_sLogBuffer, "(%s) : %s", PgmId, (char *) EMSG_PGMNFOUND);
			//quit
			_WriteLog(_LOG_ERR, m_sLogBuffer);

			m_ObjBuffer.Clear1();
			m_ObjBuffer.WriteLong(MRC_INPUT_ERR);
			m_ObjBuffer.WriteLong(UERR_PGMNFOUND);
			m_ObjBuffer.WriteString(EMSG_PGMNFOUND);
			return MRC_INPUT_ERR;
		}

//***************************************************************************
//AD2001.printf("BRStart-100  [%s]\n",PgmId);
		((CMRDataList*) pCMRDataList)->Initialize(pPgmNode, PgmId);
		//AD2001.printf("BRStart-101 \n");

		if (m_parmMode == 0) {
			((CMRDataList*) pCMRDataList)->BuildPgmParms(&m_ObjBuffer);
			//AD2001.printf("BRStart-102 \n");

		} else if (m_parmMode == 1) { //  STREAM Type 일때
			if (pPgmNode->ParmGbn != '2') {
				m_ObjBuffer.Clear1();
				m_ObjBuffer.WriteLong(MRC_RULE_ERR);
				m_ObjBuffer.WriteLong(UERR_UNMATCH_PARMTP);
				m_ObjBuffer.WriteString(EMSG_UNMATCH_PARMTP);

//startip	_WriteLog(_LOG_ERR, EMSG_UNMATCH_PARMTP);
			  _WriteLogIP(_LOG_ERR, m_StartIP,EMSG_UNMATCH_PARMTP); 

				((CMRDataList*) pCMRDataList)->DeleteStructure();

				return MRC_RULE_ERR; 
			}
			//AD2001.printf("BRStart-103 \n");

			((CMRDataList*) pCMRDataList)->BuildCBLPgmParms(m_parmMode,	m_ObjBuffer.m_Buffer + 10);
			//AD2001.printf("BRStart-103 \n");

		} else { //  Item Info가 없음
				 //AD2001.printf("BRStart-105 \n");

			((CMRDataList*) pCMRDataList)->BuildPgmParmsNoInfo(m_parmMode,&m_ObjBuffer);
			//AD2001.printf("BRStart-106 \n");

		}

#ifdef _TRACE
		if ( ((CMRDataList*)pCMRDataList)->InitTrace() ) {	
			bTrace = 1;

			char logMsg[1000];//logmsg ->20210201 *trc 파일에 로그 잘못 찍힘. 
			sprintf(logMsg, "PROGRAM : %s\n", PgmId );//msg
		
			((CMRDataList*)pCMRDataList)->TraceOutIP( logMsg );
//startip			((CMRDataList*)pCMRDataList)->TraceOut( msg );

			((CMRDataList*) pCMRDataList)->LogItemValues(); //  Display Item Value
		}
#endif

		//  적용일자 Setting
		//AD2001.printf("BRStart-107 \n");
		((CMRDataList*) pCMRDataList)->setAppDate();

		((CMRDataList*) pCMRDataList)->setItemValid(true);
		RULE_RESULT *pPgmResult = 0;
try {
	//AD2001.printf("BRStart-108 \n");
			CMRRunner Runner(pPgmNode->startRuleId, "",(CMRDataList*) pCMRDataList, 0);
			//AD2001.printf("BRStart-110 \n");

			pPgmResult = Runner.getRuleResultPtr();
			//AD2001.printf("BRStart-111 \n");


		} catch (CMTPgmException *e) {

			rc = e->GetECode();
			strcpy(msg, (char *) e->GetEMessage());
			e->Delete();

			//AD2001.printf("BRStart-112 \n");
			if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]\n", __FILE__, __LINE__);
			rr = ((CMRDataList*) pCMRDataList)->LogErrorItems(1,rc, msg);
			if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]\n", __FILE__, __LINE__);
			m_ObjBuffer.Clear1();
			m_ObjBuffer.WriteLong(MRC_RULE_ERR);
			m_ObjBuffer.WriteLong(rc);
			m_ObjBuffer.WriteString(msg);

			((CMRDataList*) pCMRDataList)->DeleteStructure();

			return MRC_RULE_ERR;
		}
/*
		try {

			CMRRunner Runner(pPgmNode->startRuleId, "",(CMRDataList*) pCMRDataList, 0);
			pPgmResult = Runner.getRuleResultPtr();
		} catch (CMTPgmException *e) {
			rc = e->GetECode();
			if( rc==3020000){
				try {
					e->Delete();
						 #ifdef WIN32
							 _sleep(100);
						 #else
							 usleep(100000);
						 #endif
					CMRRunner Runner2(pPgmNode->startRuleId, "",(CMRDataList*) pCMRDataList, 0);
					pPgmResult = Runner2.getRuleResultPtr();
				} catch (CMTPgmException *e2) {
						rc = e->GetECode();
						strcpy(msg, (char *) e2->GetEMessage());
						e2->Delete();

						rr = ((CMRDataList*) pCMRDataList)->LogErrorItems(2,rc, msg);

						m_ObjBuffer.Clear1();
						m_ObjBuffer.WriteLong(MRC_RULE_ERR);
						m_ObjBuffer.WriteLong(rc);
						m_ObjBuffer.WriteString(msg);

						((CMRDataList*) pCMRDataList)->DeleteStructure();

						return MRC_RULE_ERR;
				}
			}else{
				strcpy(msg, (char *) e->GetEMessage());
				e->Delete();

				rr = ((CMRDataList*) pCMRDataList)->LogErrorItems(3,rc, msg);

				m_ObjBuffer.Clear1();
				m_ObjBuffer.WriteLong(MRC_RULE_ERR);
				m_ObjBuffer.WriteLong(rc);
				m_ObjBuffer.WriteString(msg);

				((CMRDataList*) pCMRDataList)->DeleteStructure();

				return MRC_RULE_ERR;
			}

		}
*/
		g_Called = 1;
		if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]\n", __FILE__, __LINE__);
		if (((CMRDataList*) pCMRDataList)->BuildPgmReturn(&m_ObjBuffer,pPgmResult))
			return -1;
		if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]\n", __FILE__, __LINE__);
#ifdef _TRACE
		if ( bTrace )
		{
//		((CMRDataList*)pCMRDataList)->LogRuleReturn( ((CMRDataList*)pCMRDataList)->pStartRuleNode, pPgmResult->resRecord );
			((CMRDataList*)pCMRDataList)->TraceOutIP( "END PROGRAM" );
		}
#endif
		if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]\n", __FILE__, __LINE__);
		((CMRDataList*) pCMRDataList)->DeleteStructure();

	} catch (CMTException *e) {
		rc = e->GetECode();
		strcpy(msg, (char *) e->GetEMessage());
	    sprintf(msg,"[%-16s]%s",m_StartIP, msg);
		e->Delete();

//		sprintf(m_sLogBuffer, "(%s) : %s (Repo8)", PgmId, msg );
//		_WriteLog( _LOG_ERR, m_sLogBuffer );
		if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]\n", __FILE__, __LINE__);
		((CMRDataList*) pCMRDataList)->LogErrorItems(4,rc, msg);
		if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]\n", __FILE__, __LINE__);
		m_ObjBuffer.Clear1();
		m_ObjBuffer.WriteLong(MRC_RULE_ERR);
		m_ObjBuffer.WriteLong(rc);
		m_ObjBuffer.WriteString(msg);

		return MRC_RULE_ERR;
	} catch (...) {
		sprintf(m_sLogBuffer, "(%s) : Exception occurs (Unknown)", PgmId);
//		_WriteLogIP(_LOG_ERR,m_StartIP, m_sLogBuffer);
		if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]\n", __FILE__, __LINE__);
		((CMRDataList*) pCMRDataList)->LogErrorItems(5,-1, m_sLogBuffer);
		if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]\n", __FILE__, __LINE__);
		m_ObjBuffer.Clear1();
		m_ObjBuffer.WriteLong(MRC_RULE_ERR);
		m_ObjBuffer.WriteLong(99999);
		m_ObjBuffer.WriteString("Unknown errors.");
		return MRC_RULE_ERR;
	}

	/*   SIGSEGV Generate
	 printf("here 1\n");

	 char *p;
	 p = 0;
	 strcpy( p, "abcdefg" );

	 printf("here 2\n");
	 */

	ReplyResultSet();

	if (m_bLogTime) {
		long delay1;
		int tm;

		delay1 = ProcInterval();
//		tm = delay1;// - delay0;
#ifdef WIN32
		tm = (delay1 - delay0);// / 1000; //2017.05.04
#else
		tm = (delay1 - delay0); //2017.05.04
#endif
		if (configRec.usStatFlag) {
			pPgmNode->hit++;
			pPgmNode->totTm += tm;
			pPgmNode->totWait += delay0;
			if (tm > pPgmNode->maxTm)
				pPgmNode->maxTm = tm;
		}

		if (configRec.usDispTm) {
			char tStamp[26];
			char logMsg[100];

			GetCurrentTimeStr(tStamp);
			//AD2001.printf("logmsgxxxx\n"  );
			if (configRec.ulKeepFlag == 1)
				sprintf(logMsg, "(%02d) %s : %-10s : %04d", m_ulThreadNo,tStamp, PgmId,  tm);
			else  
				sprintf(logMsg, "(%02d) %s : %-10s : %04d", m_ulThreadNo,tStamp, PgmId,  tm);
			//AD2001.printf("logmsg (%s)\n", logMsg);
			_WriteLog(_LOG_DAT, logMsg);
//			_WriteLogNoIP(_LOG_DAT,m_StartIP, msg);

		}
	}

	return 0;
}

/***********************************************************/

void CCSThread::ReplyResultSet() {
	long outLeng = m_ObjBuffer.getLength();

//	printf("Reply : %d [%s]\n", outLeng, m_ObjBuffer.m_Buffer );

//	m_ObjBuffer.WriteLongPosition( outLeng - 50, 0 );
	m_ObjBuffer.WriteDataLength();

	((CCSManager*) m_pManager)->SocWrite(m_pChildSoc,(unsigned char *) m_ObjBuffer.m_Buffer, outLeng);
}

void CCSThread::ReplyTimeOut() {
	m_ObjBuffer.Clear1();
	m_ObjBuffer.WriteLong(MRC_RULE_ERR);
	m_ObjBuffer.WriteLong(-1);
	m_ObjBuffer.WriteString(EMSG_RUN_TIMEOUT);

	ReplyResultSet();
//	printf("CCSThread::ReplyTimeOut()\n");
	((CCSManager*) m_pManager)->CloseChildSoc(m_pChildSoc);
}
int CCSThread::getChildSockdet() {
	return m_pChildSoc;
}
