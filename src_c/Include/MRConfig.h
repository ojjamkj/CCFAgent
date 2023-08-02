#ifndef     _INC_MRCONFIG
#define     _INC_MRCONFIG


typedef struct {
    char                lpszProdName[100];

    unsigned long       ulNoServer;
	
    unsigned long       ulNoThread;
    unsigned long       ulNoMaxQueueing;
    unsigned long       ulNoPendingRequests;

//	char				lpszAddress[21];
    unsigned short      usPort;
    unsigned short      amPort;
    unsigned long       ulMaxParmSize;
    unsigned long       ulBufferSize;
	unsigned short		ulKeepFlag;
    unsigned long       ulRecvTimeout;
    unsigned long       ulSendTimeout;
    unsigned long       ulSlTm;

    unsigned long       ulDiagIntv;

    char                lpszLogPath[257];
	char				lpszTrcPath[257];
	char				configFile[501];
    unsigned long       ulLogLevel;
    unsigned long       ulLogQueueSz;

    unsigned short		usStatFlag;  //  0.None  1.PGM  2.Rule(��)  3.Rule(�ð�)
    unsigned short		usDispTm;
    unsigned short		usTrace;
    unsigned short      usAllTracle; //0=��ü���� ����, 1=���� ���ϻ���
    unsigned short    usRuleApplyTime;
//    char				bStatFlag[20];

	char				PreLoad[4];
//    unsigned long       ulExtBufSize;
    unsigned long       ulRunTimeOut;

    unsigned long       ulMaxMemSize1;
    unsigned long       ulMaxMemSize2;
    unsigned long       ulMaxMemSize3;

    unsigned long       ulMaxQueryCount;

	char				lpszEncMethod[20]; //added by DSKIM 2016.11.23
	
	/*5.0 2018.05.25 �ӽ� 5.0 �ڵ� �߰� ���� */
		char				LogMode[5]; 
    unsigned long		ulSessionFetchCnt;
    unsigned short	nNumOfQues;
    unsigned int 		nDefaultQueueId;
    char				rpcPort[5];

	char				RuleMode[10];
	int					DBAgentCount;
		
		
	char        _DBNAME[201];     //  SUBSYSTEM  31->201(20200630)
	char        _DBUSER[31];     //  PACKAGE
	char        _DBPASSWORD[201]; //  QUALIFIER
	char        _DBPWD_ENC[3]; //  ENC
	char		_DBHOST[201];
	char		_DBTYPE[21];
	char		_DBCONNINFO[501];
	unsigned long	_DBPORT;
	char		startTime[80];

	
} CONFIG_REC;

typedef struct {
    unsigned long               m_queue_id;
	unsigned int                m_thread_count;
} ENGINE_INFO;
//

class   CONFIGPARM 
{
public:
    CONFIGPARM ();
    ~CONFIGPARM();

	int    ReadConfigParm( char *fileName );
	void   DisplayConfigParm();
 
};

extern      CONFIG_REC   configRec;
extern      CONFIGPARM   *configParm;

#ifdef _DEBUG
extern int  MtsDebugLevel;
extern int  MtsLoadDebugLevel;
#endif

#endif      //_INC_MRCONFIG

#define CHANGEFLOW_VERSION "1.0.0.013"
