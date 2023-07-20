 // MtsSvr.cpp : Defines the entry point for the console application.
//
#include	<string.h>
#include <stdio.h>
#if !defined(_OS400) 
#include	"MTPrfile.h"
#endif
#include   "MItem.h"

#include		"MTMsgDef.h"
#include		"MRConfig.h"
#include		"BREXLog.h"

#define     DEFAULT_PORT                35400
#define     DEFAULT_ADMPORT             35401

#define     DEFAULT_MAXPARMSIZE         50000
#define     DEFAULT_BUFFERSIZE          4096
#define     DEFAULT_RECEIVETIMEOUT      5000000
#define     DEFAULT_SENDTIMEOUT         20

#define     DEFAULT_NOTHREAD            3
#define		DEFAULT_NOMAXQUEUEING		50
#define		DEFAULT_SLEEP_TIME			10000

//#define     DEFAULT_EXTBUFSIZE          5000000
#define     DEFAULT_RUNTIMEOUT          30
#define		DEFAULT_MAXMEMSIZE1			500
#define		DEFAULT_MAXMEMSIZE2			30
#define		DEFAULT_MAXMEMSIZE3			30

#define     DEFAULT_FETCH_COUNT			100

#define     DEFAULT_DBNAME               "DSN9"
#define     DEFAULT_PLAN                 "PLDB2RL"

//#define     DEFAULT_DEBUGLEVEL           0
//#define     DEFAULT_LOADDEBUGLEVEL       0

#define     DEFAULT_LOGPATH              "."
#define     DEFAULT_LOG_QSIZE            1000

#define     MSG_FILEOPEN_ERR            "File Open Error : %s\n"

#ifdef _MSG_KO
#define   MSG_CONFIG    "환경설정\n"
#else
#define   MSG_CONFIG    "Configuration\n"
#endif

#if	defined(_OS390) || defined(_ZOS)
char DD_MRSOUT[10][12] = {"DD:MRSOUT0",
	"DD:MRSOUT1",
	"DD:MRSOUT2",
	"DD:MRSOUT3",
	"DD:MRSOUT4",
	"DD:MRSOUT5",
	"DD:MRSOUT6",
	"DD:MRSOUT7",
	"DD:MRSOUT8",
	"DD:MRSOUT9"};
#endif
  
char   _DEBUG_YN[2];




CONFIG_REC configRec;
CONFIGPARM *configParm;
CMTProfile *fileIni;
CONFIGPARM::CONFIGPARM() {
	configRec.usPort = DEFAULT_PORT;
	configRec.amPort = DEFAULT_ADMPORT;

	configRec.ulMaxParmSize = DEFAULT_MAXPARMSIZE;
	configRec.ulKeepFlag = 1;

	configRec.ulRecvTimeout = DEFAULT_RECEIVETIMEOUT;
	configRec.ulSendTimeout = DEFAULT_SENDTIMEOUT;

	configRec.ulNoThread = DEFAULT_NOTHREAD;
	configRec.ulNoMaxQueueing = DEFAULT_NOMAXQUEUEING;
	configRec.ulSlTm = DEFAULT_SLEEP_TIME;
	configRec.ulDiagIntv = 0;

	strcpy(configRec.lpszLogPath, DEFAULT_LOGPATH);
	configRec.lpszTrcPath[0] = 0;

	configRec.ulLogLevel = LOGLEVEL_ALL;
	configRec.ulLogQueueSz = DEFAULT_LOG_QSIZE;

	configRec.usStatFlag = 2;
	configRec.usDispTm = 1;
	configRec.usTrace = 0;
	configRec.usAllTracle = 1;
 

	strcpy(configRec.PreLoad, "Y");
	configRec.ulRunTimeOut = DEFAULT_RUNTIMEOUT;

	configRec.ulMaxMemSize1 = DEFAULT_MAXMEMSIZE1 * 1024;
	configRec.ulMaxMemSize2 = DEFAULT_MAXMEMSIZE2 * 1024;
	configRec.ulMaxMemSize3 = DEFAULT_MAXMEMSIZE3 * 1024;

	configRec.ulMaxQueryCount = DEFAULT_FETCH_COUNT;
	configRec.usRuleApplyTime=0;
//	_DEBUG_YN[0] = 'N';
//	_DEBUG_YN[1] = 0X00;

}
char decode_log_ch(char x) {
	if('0'<=x&&x<='7') {
		return (x-'0');
	}
	return 0x02; // 
}

 
int CONFIGPARM::ReadConfigParm(char *fileName) {
 
	_DEBUG_YN[0] = 'Y'; _DEBUG_YN[1] = 0; 

	fileIni = new CMTProfile(fileName);
	strcpy(configRec.configFile, fileName);
	char buf[100];
	if (!fileIni->FileOpenCheck()) {
		//printf("File Open Error : %s\n", fileName);
		printf(MSG_FILEOPEN_ERR, fileName);
		return -1;
	}

	  

	fileIni->ReadString("BRMINER", "DEBUG", "N", _DEBUG_YN, 2, false);
 
	if (_DEBUG_YN[0] == 'Y') {
		printf("004==>%s\n", fileName);
		printf("DEBUG => [%s]\n", _DEBUG_YN);
		printf("005==>%s\n", fileName);
		printf("[%s][%d]\n", __FILE__, __LINE__);
		printf("006==>%s\n", fileName);
	}
	  
	//	if (_DEBUG_YN[0] == 'Y')
	if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]\n", __FILE__, __LINE__);
	fileIni->ReadString("BRMINER", "Method", "", configRec.lpszEncMethod, 20, false); //added by DSKIM
 
																					  //	if (_DEBUG_YN[0] == 'Y')
	if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]\n", __FILE__, __LINE__);
	fileIni->ReadString("BRMINER", "RuleMode", "AML", configRec.RuleMode, 5, true);

	if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]\n", __FILE__, __LINE__);
	 

	if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]\n", __FILE__, __LINE__);
	fileIni->ReadString("BRMINER", "LogMode", "", buf, 5 , false);
	if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]\n", __FILE__, __LINE__);

	configRec.DBAgentCount = 3;
//	printf("RuleMode  %s\n", configRec.RuleMode);
//	printf("_DBTYPE %s\n", configRec._DBTYPE);
//	printf("DBAgentCount  %d\n", configRec.DBAgentCount);




	return 0;
}

 
void CONFIGPARM::DisplayConfigParm() {
	char msg[128];

//	sprintf(msg, "%s\n", configRec.lpszProdName); 
//	sprintf(msg, "%s\n", "BRMiner 5.01 (2020.07.01) "); 
//	sprintf(msg, "%s\n", "BRMiner 5.02 (2020.12.01) ");
//	sprintf(msg, "%s\n", "BRMiner 5.03 (2021.06.30) ");
	sprintf(msg, "%s\n", "BRMiner 5.03 (2021.09.01) ");
	sprintf(msg, "%s\n", "BRMiner 5.04 (2021.12.28) ");
	_WriteLogNo(_LOG_LOG, msg);

	//_WriteLogNo(_LOG_LOG, "Configuration\n");
	_WriteLogNo(_LOG_LOG, MSG_CONFIG);

	sprintf(msg, "     Port                = %d", configRec.usPort);
	_WriteLogNo(_LOG_LOG, msg);
	sprintf(msg, "     Admin Port          = %d", configRec.amPort);
	sprintf(msg, "     RuleMode            = %s", configRec.RuleMode);
	printf("     RuleMode            = %s\n", configRec.RuleMode);

	_WriteLogNo(_LOG_LOG, msg);
	sprintf(msg, "     Max Parm Size       = %5d KB",
			configRec.ulMaxParmSize / 1024);
	_WriteLogNo(_LOG_LOG, msg);
	sprintf(msg, "     Keep Connection     = %s",
			configRec.ulKeepFlag ? "Yes" : "No");
	_WriteLogNo(_LOG_LOG, msg);
	sprintf(msg, "     RecvTimeout         = %d", configRec.ulRecvTimeout);
	_WriteLogNo(_LOG_LOG, msg);
	sprintf(msg, "     SendTimeout         = %d \n", configRec.ulSendTimeout);
	_WriteLogNo(_LOG_LOG, msg);

	sprintf(msg, "     Thread no           = %d", configRec.ulNoThread);
	_WriteLogNo(_LOG_LOG, msg);

	sprintf(msg, "     Max Queueing        = %d\n", configRec.ulNoMaxQueueing);
	_WriteLogNo(_LOG_LOG, msg);


	sprintf(msg, "     Diagnose Interval   = %d Sec\n", configRec.ulDiagIntv);
	_WriteLogNo(_LOG_LOG, msg);

	sprintf(msg, "     Log File Path       = %s", configRec.lpszLogPath);
	_WriteLogNo(_LOG_LOG, msg);

//	sprintf(msg, "     Log Level           = %d", configRec.ulLogLevel);
//	_WriteLogNo(_LOG_LOG, msg);

	sprintf(msg, "     Log Queue Size      = %d", configRec.ulLogQueueSz);
	_WriteLogNo(_LOG_LOG, msg);

	sprintf(msg, "     Trace File Path     = %s", configRec.lpszTrcPath);
	_WriteLogNo(_LOG_LOG, msg);

	sprintf(msg, "     Trace Always        = %s\n", configRec.usTrace ? "Yes" : "No");
	_WriteLogNo(_LOG_LOG, msg);
	sprintf(msg, "     Trace Last API       = %s\n", configRec.usAllTracle ? "Yes" : "No");
	_WriteLogNo(_LOG_LOG, msg);

	sprintf(msg, "     RPC Enable       = No\n");
	_WriteLogNo(_LOG_LOG, msg);

//	sprintf(msg, "     Rule Apply Time      = %02d\n", configRec.usRuleApplyTime  );
//	_WriteLogNo(_LOG_LOG, msg);

	 
	_WriteLogNo(_LOG_LOG, "");

	sprintf(msg, "     PreLoad                    = %s", configRec.PreLoad);
	_WriteLogNo(_LOG_LOG, msg);
	sprintf(msg, "     Running TimeOut            = %4d Sec",
			configRec.ulRunTimeOut);
	_WriteLogNo(_LOG_LOG, msg);
//	sprintf(msg, "     Extended Buffer Size       = %4d KB",configRec.ulExtBufSize / 1024);
//	_WriteLogNo( _LOG_LOG, msg );
	sprintf(msg, "     Global vars heap size      = %4d KB",
			configRec.ulMaxMemSize1 / 1024);
	_WriteLogNo(_LOG_LOG, msg);
	sprintf(msg, "     Array comparison heap size = %4d KB",
			configRec.ulMaxMemSize2 / 1024);
	_WriteLogNo(_LOG_LOG, msg);
	sprintf(msg, "     Calculate queue heap size  = %4d KB",
			configRec.ulMaxMemSize3 / 1024);
	_WriteLogNo(_LOG_LOG, msg);
	sprintf(msg, "     Max query count            = %4d",		configRec.ulMaxQueryCount);
	_WriteLogNo(_LOG_LOG, msg);

	sprintf(msg, "     Log Mode                   = %d%d%d%d",		configRec.LogMode[0],configRec.LogMode[1],configRec.LogMode[2],configRec.LogMode[3]);
	_WriteLogNo(_LOG_LOG, msg);


	_WriteLogNo(_LOG_LOG, "");
}
