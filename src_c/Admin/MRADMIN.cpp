#include    <string.h>
#include    <stdio.h>
#include    <stdlib.h>

#include    "MItem.h"
#include    "MRCSComm.h"
//#include    "MTParm.h"
//#include    "MTObject.h"
#include    "BRMObj.h"
#include    "MTUtil.h"
#include    "MRADMIN.h"
#include    "MRSTRUCT.h"
#include    "MRGBLVAR.h"

#include	"MRCSMgr.h"
#include	"BREXLog.h"
#include    "BRTrace.h"
#include    "MTPrfile.h"
//#include    "MRAgtMgr.h"
#ifdef _MSG_KO
#define		MSG_THREAD_START_FAIL     "어드민 스레드 시작 실패.\n"
#define		MSG_RELOAD_PROG1          "  %d 프로그램을 재로딩합니다."
#define		MSG_RELOAD_PROG_ERR       "%s : 프로그램 (%s) 재로딩 오류.\n    %s"
#define		MSG_RELOAD_PROG2          "      [%s] 프로그램이 재로딩되었습니다."
#define		MSG_RELOAD_RULE           "  %d 룰을 재로딩합니다."
#define		MSG_MEMALLOC_ERR          "메모리 할당 오류."
#define		MSG_RULEID_EXCEPTION      "     RuleId [%d] 예외가 발생하였습니다."
#define		MSG_RULEID_NOT_RELOAD     "      RuleId [0] 를 재로딩할 수 없습니다."
#define		MSG_RULEID_RELOAD         "     Rule Id [%d] 가 재로딩되었습니다."
#define		MSG_RULEID_ERR            "      Rule Id [%d] 오류\n        %s"
#define		MSG_NOTFOUND_RULE         "NF-0: 룰을 찾을 수 없습니다. (%d, ...)"
#define		MSG_REMOVE_RULE           " %d 룰 삭제를 요청받았습니다."
#define		MSG_NOT_REMOVE_RULE       "     (%d)룰을 삭제할 수 없습니다."
#define		MSG_REMOVE_RULEID         " (%d) 룰 ID 삭제를 요청받았습니다."
#define		MSG_TRACE_ADD             "Trace 추가 ( %s_%s ).\n"

#define		MSG_INVALIDACCESS_ADMIN "Admin process invalid access."
#define		MSG_SIZEERROR_ADMIN     "Admin process data size error."
#define		MSG_START_DEPLOYPROG    "Start deploy program."
#define		MSG_END_DEPLOYPROG      "End deploy program.\n"
#define		MSG_START_RT_DEPLOY     "Start Runtime deploy."
#define		MSG_END_DEPLOY          "End Deploy.\n"
#else
#define		MSG_THREAD_START_FAIL     "Admin thread start failure.\n"
#define		MSG_RELOAD_PROG1          "  Reload %d program(s)."
#define		MSG_RELOAD_PROG_ERR       "%s : Program (%s) reload error.\n    %s"
#define		MSG_RELOAD_PROG2          "      Program [%s] reloaded."
#define		MSG_RELOAD_RULE           "  Reload %d rule(s)."
#define		MSG_MEMALLOC_ERR          "Memory allocation error."
#define		MSG_RULEID_EXCEPTION      "     RuleId [%d] Exception occurs."
#define		MSG_RULEID_NOT_RELOAD     "      RuleId [0] can't reload."
#define		MSG_RULEID_RELOAD         "     Rule Id [%d] reloaded."
#define		MSG_RULEID_ERR            "      Rule Id [%d] error\n        %s"
#define		MSG_NOTFOUND_RULE         "NF-0: Rule not found (%d, ...)"
#define		MSG_REMOVE_RULE           " Remove %d rule(s) requested."
#define		MSG_NOT_REMOVE_RULE       "     Can not remove rule(%d)."
#define		MSG_REMOVE_RULEID         " Remove ruleid (%d) requested."
#define		MSG_REMOVE_PROG      " Remove Program (%s) requested."
#define		MSG_TRACE_ADD             "Trace Add ( %s_%s ).\n"

#define		MSG_INVALIDACCESS_ADMIN "Admin process invalid access."
#define		MSG_SIZEERROR_ADMIN     "Admin process data size error."
#define		MSG_START_DEPLOYPROG    "Start deploy program."
#define		MSG_END_DEPLOYPROG      "End deploy program.\n"
#define		MSG_START_RT_DEPLOY     "Start Runtime deploy."
#define		MSG_END_DEPLOY          "End Deploy.\n"
#endif


#define ADMIN_REQ_THREAD_INF		20
#define	ADMIN_REQ_THREAD_MODIFY		21
#define ADMIN_REQ_THREAD_DEL		22
#define	ADMIN_REQ_QUEUE_INF			23
#define ADMIN_REQ_QUEUE_ADD			24
#define ADMIN_REQ_QUEUE_DEL			25

CMRAdmin *g_pCMRAdmin; 



extern CONFIG_REC configRec;

extern CMTProfile *fileIni;

#ifdef	_TRACE
extern CBRTrace *g_pCBRTrace;
#endif
CMRAdmin::CMRAdmin() :
		CBREXThreadrun(), CBREXThread(102), CBREXListen() {
	m_ObjBuffer = new CBRMObj(_ADMIN_PARM_SZ);

	if (!m_ObjBuffer->m_Buffer) {
		throw new CMTException(SYSERR_, ERRMSG_MEMORYALLOCATEFAILURE_01);
	}

	if (Start(this)) {
		char lpsz[256];

		//sprintf(lpsz, "Admin thread start failure.\n");
		sprintf(lpsz, MSG_THREAD_START_FAIL);
		throw new CMTException(SYSERR_THREADSTARTFAILURE, lpsz);
	}

}

CMRAdmin::~CMRAdmin() {
}

void CMRAdmin::Run(void) {
	Init(configRec.amPort);
}

void CMRAdmin::OnAccept(CBREXChiSoc *pChild) {
//	unsigned char c;
	int inLeng;

	m_pChildSoc = pChild;

	m_Rc = 0;
	m_Message[0] = 0;

	while (1) {
		try {
			m_pChildSoc->Read((unsigned char *) m_ObjBuffer->m_Buffer, 50);
			m_ObjBuffer->setLength(50);

			if (m_ObjBuffer->m_Buffer[4] != 1) {
				//_WriteLog(_LOG_ERR, "Admin process invalid access.");
				_WriteLog(_LOG_ERR, MSG_INVALIDACCESS_ADMIN);
				continue;
			}

			inLeng = m_ObjBuffer->ReadDataLength();

			if (inLeng < 0 || inLeng >= _ADMIN_PARM_SZ) {
				//_WriteLog(_LOG_ERR, "Admin process data size error.");
				_WriteLog(_LOG_ERR, MSG_SIZEERROR_ADMIN);
				continue;
			}

			if (inLeng > 0) {
				m_pChildSoc->Read((unsigned char *) m_ObjBuffer->m_Buffer,inLeng);
			}

			m_ObjBuffer->setLength(inLeng);

		} catch (CMTException *e) {

//			if ( e->GetECode() == SYSERR_SOCKREADFAILURE ) break;

//			_WriteLog( _LOG_ERR, (char *) e->GetEMessage() );

			e->Delete();

			break;
		}
		printf("MRADMIN:001\n");
		mainProcess();

		if (m_Rc != 0) {
			m_ObjBuffer->Clear1();
			m_ObjBuffer->WriteLong(m_Rc);
			m_ObjBuffer->WriteLong(-1);
			m_ObjBuffer->WriteString(m_Message);
		}

		if (m_Rc != 99)
			ReplyResultSet();
	}

	delete m_pChildSoc;
}

void CMRAdmin::ReplyResultSet() {
	try {
		long outLeng = m_ObjBuffer->getLength();

//		printf("Reply : %d\n", outLeng );

		m_ObjBuffer->WriteDataLength();

		m_pChildSoc->Write((unsigned char *) m_ObjBuffer->m_Buffer, outLeng);

	} catch (CMTException *e) {
		m_Rc = e->GetECode();
		strcpy(m_Message, (char *) e->GetEMessage());
		e->Delete();

		_WriteLog(_LOG_ERR, m_Message);
	}
}

void CMRAdmin::mainProcess() {
	try {
		m_Rc = 0;
		m_mode = m_ObjBuffer->ReadLong();
		printf("MRADMIN:002 m_mode[%d]\n", m_mode);


		if (m_mode < 100)
			NormalProcess();
		else {
			CTSectProcess();
		}

	} catch (CMTException *e) {
		m_Rc = e->GetECode();
		strcpy(m_Message, (char *) e->GetEMessage());
		e->Delete();
	}
}
 
long CMRAdmin::NormalProcess() {   
	switch (m_mode) {
	case 1:
		g_pManager->GetEngineInfo(m_ObjBuffer);
		break;
	case 2:
		g_pManager->GetEngineDetail(m_ObjBuffer);
		break;

	case 3://로그 on restart:
		g_pCBRTrace->setTraceLog(m_ObjBuffer);
	break;

#ifdef _TRACE
		case 11 :
		ReadTraceFile();
		break;
#endif
	case 12:
//			pCMRGblVars->GetPgmInfo( & m_ObjBuffer );
		break;
 	case ADMIN_REQ_THREAD_INF : 	//20
		g_pManager->GetThrInfo(m_ObjBuffer);
		break;

	case ADMIN_REQ_QUEUE_INF : //23
		g_pManager->GetQueueInfo(m_ObjBuffer);
		break;
		
	/*	
		
	case ADMIN_REQ_THREAD_MODIFY : //21
		break;
	case ADMIN_REQ_THREAD_DEL : 22
		break;
	case ADMIN_REQ_QUEUE_INF : 23
		break;
	case ADMIN_REQ_QUEUE_ADD : 24
		break;
	case ADMIN_REQ_QUEUE_DEL : 25
		break;
	*/
		
		
		
		
	case 99: //  Normal Termination
		m_ObjBuffer->Clear1();
		ReplyResultSet();

		g_pManager->NormalTerminate();

		Stop(); //  Admin Listen stop

		m_Rc = 99;
		m_ObjBuffer->Clear1();
		m_ObjBuffer->WriteLong(0);

		break;
	default:
		m_Rc = MRC_INPUT_ERR;
		strcpy(m_Message, "Invalid command code.");
		break;
	}

	return 0;
}

int ConnectLoadDatabase(char *msg);
void DisConnectLoadDatabase();

long CMRAdmin::CTSectProcess() {

	g_pManager->StartAdminProcess();

#ifdef WIN32
	//_sleep(100);
	Sleep(100);
#else
	usleep(10000);  //100000->10000 > 4.0 10000
#endif
	if ((m_Rc = ConnectLoadDatabase(m_Message)) != 0) {
		if ((m_Rc = ConnectLoadDatabase(m_Message)) != 0) {
			_WriteLog(_LOG_LOG, m_Message);
//			DisConnectLoadDatabase();
			g_pManager->EndAdminProcess();
//			_WriteLog(_LOG_LOG, "End Admin Unlock");
			return -1;
		}
	}

	try {
		switch (m_mode) {
		case 101:
			DeployRule();
			break;
		case 102:
			DeployProgram();
			break;
		case 103:
			DeleteRule();
			break;  
		case 107:
			//DeleteRuleProgram ();
			break;  
//			case 104 : ChangeConfig();		break;
#ifdef	_TRACE
		case 105 : TraceStart(); break;
#endif
		case 106 : PwdChange(); break;

		default:
			m_Rc = MRC_INPUT_ERR;
			strcpy(m_Message, "Invalid command code.");
		}
	} catch (CMTException *e) {
		m_Rc = e->GetECode();
		strcpy(m_Message, (char *) e->GetEMessage());
		e->Delete();
	}

	if (m_Rc == 0) {
		m_ObjBuffer->Clear1();
		m_ObjBuffer->WriteLong(0);
	}

	g_pManager->EndAdminProcess();

	return 0;
}

long CMRAdmin::DeployProgram() {
	long deployCnt;
	char pgmId[11];
	char errMessage[256];
	char LogBuffer[512];
//	int  rc;

	m_Rc = 0;

	//_WriteLog(_LOG_LOG, "Start deploy program.");
	_WriteLog(_LOG_LOG, MSG_START_DEPLOYPROG);

	deployCnt = m_ObjBuffer->ReadLong();

	if (deployCnt > 0) {
		//sprintf(LogBuffer, "  Reload %d program(s).", deployCnt);
		sprintf(LogBuffer, MSG_RELOAD_PROG1, deployCnt);
		_WriteLogNo(_LOG_LOG, LogBuffer);
	}

	for (int i = 0; i < deployCnt; i++) {
		m_ObjBuffer->ReadString(pgmId);

		if (pCMRGblVars->LoadPgmById(pgmId, errMessage) != 0) {/*
			sprintf(LogBuffer, "%s : Program (%s) reload error.\n    %s", pgmId,
					errMessage);*/
			sprintf(LogBuffer, MSG_RELOAD_PROG_ERR, pgmId,
				errMessage);

			_WriteLogNo(_LOG_LOG, LogBuffer);

			m_Rc = MRC_RULE_ERR;
			strcpy(m_Message, errMessage);
			return -1;

		} else {
			//sprintf(LogBuffer, "      Program [%s] reloaded.", pgmId);
			sprintf(LogBuffer, MSG_RELOAD_PROG2, pgmId);
			_WriteLogNo(_LOG_LOG, LogBuffer);
		}
	}

	//_WriteLog(_LOG_LOG, "End deploy program.\n");
	_WriteLog(_LOG_LOG, MSG_END_DEPLOYPROG);

	return m_Rc;
}

long CMRAdmin::DeployRule() {
	long deployCnt;
//	char sTemp[3];
	char errMessage[256];
	char LogBuffer[512];
	int i, rc;

	m_Rc = 0;

	//_WriteLog(_LOG_LOG, "Start Runtime deploy.");
	_WriteLog(_LOG_LOG, MSG_START_RT_DEPLOY);

	//   Rule Deploy
	deployCnt = m_ObjBuffer->ReadLong();
	if (deployCnt > 0) {
		//sprintf(LogBuffer, "  Reload %d rule(s).", deployCnt);
		sprintf(LogBuffer, MSG_RELOAD_RULE, deployCnt);
		_WriteLogNo(_LOG_LOG, LogBuffer);
	} else {
		//_WriteLog(_LOG_LOG, "End Deploy.\n");
		_WriteLog(_LOG_LOG, MSG_END_DEPLOY);
		return 0;
	}

	int *ruleArray;
	ruleArray = (int *) malloc(sizeof(int) * deployCnt);
	if (!ruleArray) {
		m_Rc = MRC_SYS_ERR;
		//sprintf(m_Message, "Memory allocation error.");
		sprintf(m_Message, MSG_MEMALLOC_ERR);
		return m_Rc;
	}

//	int rows=0;

	for (i = 0; i < deployCnt; i++) {
		ruleArray[i] = m_ObjBuffer->ReadLong();

//		m_ObjBuffer->ReadString(sTemp);

		//  Header가 바뀌면 Body도 다시 Load해야 하더라...
//		if ( sTemp[0] == 'Y' ) sTemp[1] = 'Y';

	}

	for (i = 0; i < deployCnt; i++) {
		if (ruleArray[i] == 0) {
			//sprintf(LogBuffer, "      RuleId [0] can't reload.");
			sprintf(LogBuffer, MSG_RULEID_NOT_RELOAD);
			_WriteLog(_LOG_LOG, LogBuffer);
			continue;
		}

		try {
//			rc = pCMRGblVars->LoadRuleById( ruleId, sTemp[0], sTemp[1], errMessage );
			rc = pCMRGblVars->LoadRuleById(ruleArray[i], 'Y', 'Y', errMessage);
		} catch (...) {
			rc = -1; /*
			sprintf(errMessage, "     RuleId [%d] Exception occurs.",
					ruleArray[i]);*/
			sprintf(errMessage, MSG_RULEID_EXCEPTION,
					ruleArray[i]);
		}

		if (rc == 0) {
			//sprintf(LogBuffer, "     Rule Id [%d] reloaded.", ruleArray[i]);
			sprintf(LogBuffer, MSG_RULEID_RELOAD, ruleArray[i]);
			_WriteLogNo(_LOG_LOG, LogBuffer);

		} else {
//			char rName[101];
//			pCMRLoadData->getRuleName( ruleArray[i], rName );
/*
			sprintf(LogBuffer, "      Rule Id [%d] error\n        %s",
					ruleArray[i], errMessage);*/
			sprintf(LogBuffer, MSG_RULEID_ERR,
					ruleArray[i], errMessage);
			_WriteLogNo(_LOG_LOG, LogBuffer);

			m_Rc = MRC_RULE_ERR;
			//sprintf(m_Message, "NF-0: Rule not found (%d, ...)", ruleArray[i]);
			sprintf(m_Message, MSG_NOTFOUND_RULE, ruleArray[i]);
		}
	}

	//_WriteLog(_LOG_LOG, "End Deploy.\n");
	_WriteLog(_LOG_LOG, MSG_END_DEPLOY);

	free(ruleArray);

	return m_Rc;
}

long CMRAdmin::PwdChange() {
	 

	return 0;
}
long CMRAdmin::DeleteRule() {
	long Cnt, ruleId;
	char LogBuffer[128];

	Cnt = m_ObjBuffer->ReadLong();

	//sprintf(LogBuffer, " Remove %d rule(s) requested.", Cnt);
	sprintf(LogBuffer, MSG_REMOVE_RULE, Cnt);
	_WriteLog(_LOG_LOG, LogBuffer);

	int rows = 0;
	for (int i = 0; i < Cnt; i++) {
		ruleId = m_ObjBuffer->ReadLong();

		if (ruleId == 0) {
			//sprintf(LogBuffer, "     Can not remove rule(%d).", ruleId);
			sprintf(LogBuffer, MSG_NOT_REMOVE_RULE, ruleId);
			_WriteLogNo(_LOG_LOG, LogBuffer);

			continue;
		}

		pCMRGblVars->RemoveRule(ruleId);

		//sprintf(LogBuffer, " Remove ruleid (%d) requested.", ruleId);
		sprintf(LogBuffer, MSG_REMOVE_RULEID, ruleId);
		_WriteLogNo(_LOG_LOG, LogBuffer);

	}

	_WriteLogNo(_LOG_LOG, "");

	return 0;
}

#ifdef	_TRACE
 
long CMRAdmin::TraceStart()
{
	char TraceId[20];
	char PgmId[21];

	char LogBuffer[128];

	m_ObjBuffer->ReadString(TraceId);
	m_ObjBuffer->ReadString(PgmId);

	g_pCBRTrace->AddTraceInfo( TraceId, PgmId );

	//sprintf(LogBuffer,"Trace Add ( %s_%s ).\n", TraceId, PgmId );
	sprintf(LogBuffer,MSG_TRACE_ADD, TraceId, PgmId );
	_WriteLog( _LOG_LOG, LogBuffer );

	return 0;
}

void CMRAdmin::ReadTraceFile()
{
	FILE *fp;
	char TraceId[20];
	char PgmId[21];

	char fName[300];
	char tempName[200];
	char buf[501];
	int len;

	m_ObjBuffer->ReadString(TraceId);
	m_ObjBuffer->ReadString(PgmId);

	strcpy( tempName, configRec.lpszTrcPath );

	len = strlen( tempName );
	if ( tempName[len-1] != PATH_DELIM ) {
		tempName[len] = PATH_DELIM;
		tempName[len+1] = 0;
	}
 
 	sprintf( fName, "%s%s_%s.trc", tempName,PgmId , TraceId ); 
	if ( (fp = fopen(fName,"r")) == NULL ) {
		m_Rc = MRC_INPUT_ERR;
		strcpy( m_Message, "Trace file not exist." );
		return;
	}

	m_ObjBuffer->Clear1();
	m_ObjBuffer->WriteLong(0);

	while ( 1 ) {
		if ( fgets( buf, 500, fp ) == NULL ) break;

		m_ObjBuffer->WriteString( buf );

//		printf( "[%3d]%s", strlen(buf), buf );
	}

	m_ObjBuffer->WriteString(""); //  끝을 알림

	fclose(fp);
}

#endif
