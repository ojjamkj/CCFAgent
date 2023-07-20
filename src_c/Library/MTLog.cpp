#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <time.h>

#include    "MTUtil.h"
#include    "MTLog.h"

#ifdef _WIN32
#define   PATH_DELIM   '\\'
#else
#define   PATH_DELIM   '/'
#endif

static FILE *fpLog;
static char lpszFileName[256];
static char bLog2File;

void getCurrentTime(char* lpszTime) {
	time_t localTime;
	struct tm *stTempTime;
	struct tm pTm2;
	localTime=time(&localTime);
#ifdef WIN32
	stTempTime= localtime(&localTime);
#else
	stTempTime=localtime_r(&localTime, &pTm2);
#endif
	sprintf(lpszTime
				, "%4d%02d%02d %02d%02d%02d"
				, stTempTime->tm_year + 1900
				, stTempTime->tm_mon + 1
				, stTempTime->tm_mday
				, stTempTime->tm_hour
				, stTempTime->tm_min
				, stTempTime->tm_sec);
}

void InitLogInfo(char bLogTo, char *fileName) {

	bLog2File = 0;

#ifndef _OS390

	if (bLogTo != 1)
		return;

	char lpszTime[30];

	bLog2File = 1;

	getCurrentTime(lpszTime);

	sprintf(lpszFileName, "%s_%s.log", fileName, lpszTime);

#endif

}

void WriteLogBy(long mode, const char* lpszLog) {
	if (!bLog2File) {
		if (mode) {
			char lpszTime[26];
			GetCurrentTimeStr(lpszTime);
			printf("%s %s\n", lpszTime, lpszLog);
		} else
			printf("%s\n", lpszLog);

	} else {
//  Critical Section 처리 추가할것...

		if ((fpLog = fopen(lpszFileName, "a+")) == NULL) {
			printf("\nCan not open Log file (%s)\n\n", lpszFileName);

			bLog2File = 0;

//			return;
		}

		if (mode) {
			char lpszTime[15];
			GetCurrentTimeStr(lpszTime);
			fprintf(fpLog, "%s %s\n", lpszTime, lpszLog);
		} else
			fprintf(fpLog, "%s\n", lpszLog);

		fclose(fpLog);

//  Critical Section End
	}

	/*
	 WSADATA     wsaData;
	 WSAStartup( 0x0101, &wsaData );

	 try
	 {
	 CMTTcpClient    soccli(0,0);
	 soccli.Init( "127.0.0.1", 29999 );

	 soccli.Open();

	 soccli.Write((unsigned char *) lpszLog, strlen((char *) lpszLog));

	 soccli.Close();

	 }
	 catch( CMTException* e )
	 {
	 printf( e->GetEMessage() );
	 e->Delete();
	 }

	 WSACleanup();
	 */
}

void WriteLogNo(long lCode, const char* lpszLog) {
	WriteLogBy(0, lpszLog);
}

void WriteLog(long lCode, const char* lpszLog) {
	WriteLogBy(1, lpszLog);
}

CMessage::CMessage(int mode) {
	m_Buffer[0] = 0;
	m_Pos = 0;
}

CMessage::~CMessage() {
}

void CMessage::AddMessage(char *msg) {
	int len;

	len = strlen(msg);

	if (m_Pos + len > _LOGBUFFERSIZE) {
		WriteMessage();
	}

	strcpy(m_Buffer + m_Pos, msg);

	m_Pos += len;
}

void CMessage::WriteMessage(long mode) {
	WriteLogBy(mode, m_Buffer);

	m_Buffer[0] = 0;
	m_Pos = 0;
}

