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

/*
 void Timer(int signum) {
 printf("Timer 5 Second  %d\n", signum );
 }
 */


/* 

#define BUFFER_SIZE 1024

int main() {
	const char* file_path = "C:/cfagent/temp/AA.zip";  // 대상 파일 경로 설정
	char msg[1024];
	char *charValue = "9F4726BACB67649E6AB854B4E5F9F3D4";
	printf("vlaue [%d]  \n", calculate_crc32(file_path, msg, charValue));
	printf("vlaue [%s]  \n", msg );

}
int mains() {
	const char* file_path = "C:/cfagent/temp/AA.zip";  // 대상 파일 경로 설정

	HANDLE file = CreateFileA(file_path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (file == INVALID_HANDLE_VALUE) {
		printf("파일을 열 수 없습니다.\n");
		return 1;
	}

	HCRYPTPROV hProv;
	if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
		printf("CSP를 초기화할 수 없습니다.\n");
		CloseHandle(file);
		return 1;
	}

	HCRYPTHASH hHash;
	if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
		printf("해시 객체를 생성할 수 없습니다.\n");
		CryptReleaseContext(hProv, 0);
		CloseHandle(file);
		return 1;
	}

	BYTE buffer[BUFFER_SIZE];
	DWORD bytesRead;
	BOOL readResult;
	while ((readResult = ReadFile(file, buffer, BUFFER_SIZE, &bytesRead, NULL)) && bytesRead > 0) {
		if (!CryptHashData(hHash, buffer, bytesRead, 0)) {
			printf("데이터를 해시할 수 없습니다.\n");
			CryptDestroyHash(hHash);
			CryptReleaseContext(hProv, 0);
			CloseHandle(file);
			return 1;
		}
	}

	if (!readResult) {
		printf("파일을 읽는 도중 오류가 발생했습니다.\n");
		CryptDestroyHash(hHash);
		CryptReleaseContext(hProv, 0);
		CloseHandle(file);
		return 1;
	}

	BYTE hashValue[16];
	DWORD hashSize = sizeof(hashValue);
	if (!CryptGetHashParam(hHash, HP_HASHVAL, hashValue, &hashSize, 0)) {
		printf("해시 값을 가져올 수 없습니다.\n");
		CryptDestroyHash(hHash);
		CryptReleaseContext(hProv, 0);
		CloseHandle(file);
		return 1;
	}

	char hValue[100];
	memset(hValue, 0xff, 100);
	for (int i = 0; i < hashSize; i++) {
		if (i == 0) {
			sprintf(hValue, "%02X",hashValue[i]);
		}
		else {
			sprintf(hValue, "%s%02X", hValue, hashValue[i]);
		}
	}
	printf(" md5: %s \n", hValue);


	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);
	CloseHandle(file);

	return 0;
}
#define BUFFER_SIZE 1024

int mainSHA256() {
	const char* file_path = "C:/cfagent/temp/AA.zip";  // 대상 파일 경로 설정

	HANDLE file = CreateFileA(file_path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (file == INVALID_HANDLE_VALUE) {
		printf("파일을 열 수 없습니다.\n");
		return 1;
	}

	HCRYPTPROV hProv;
	if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
		printf("CSP를 초기화할 수 없습니다.\n");
		CloseHandle(file);
		return 1;
	}

	HCRYPTHASH hHash;
	if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
		printf("해시 객체를 생성할 수 없습니다.\n");
		CryptReleaseContext(hProv, 0);
		CloseHandle(file);
		return 1;
	}

	BYTE buffer[BUFFER_SIZE];
	DWORD bytesRead;
	BOOL readResult;
	while ((readResult = ReadFile(file, buffer, BUFFER_SIZE, &bytesRead, NULL)) && bytesRead > 0) {
		if (!CryptHashData(hHash, buffer, bytesRead, 0)) {
			printf("데이터를 해시할 수 없습니다.\n");
			CryptDestroyHash(hHash);
			CryptReleaseContext(hProv, 0);
			CloseHandle(file);
			return 1;
		}
	}

	if (!readResult) {
		printf("파일을 읽는 도중 오류가 발생했습니다.\n");
		CryptDestroyHash(hHash);
		CryptReleaseContext(hProv, 0);
		CloseHandle(file);
		return 1;
	}

	BYTE hashValue[32];
	DWORD hashSize = sizeof(hashValue);
	if (!CryptGetHashParam(hHash, HP_HASHVAL, hashValue, &hashSize, 0)) {
		printf("해시 값을 가져올 수 없습니다.\n");
		CryptDestroyHash(hHash);
		CryptReleaseContext(hProv, 0);
		CloseHandle(file);
		return 1;
	}

	printf("파일 SHA-256 체크섬: ");
	for (int i = 0; i < hashSize; i++) {
		printf("%02X", hashValue[i]);
	}
	printf("\n");

	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);
	CloseHandle(file);

	return 0;
}

#define CRC32_POLYNOMIAL 0xEDB88320
unsigned int crc32_table[256];
void generate_crc32_table() {
	unsigned int crc, i, j;
	for (i = 0; i < 256; i++) {
		crc = i;
		for (j = 0; j < 8; j++) {
			if (crc & 1)
				crc = (crc >> 1) ^ CRC32_POLYNOMIAL;
			else
				crc >>= 1;
		}
		crc32_table[i] = crc;
	}
}

unsigned int calculate_crc32(const unsigned char* data, int length) {
	unsigned int crc = 0xFFFFFFFF;
	int i;
	for (i = 0; i < length; i++) {
		crc = (crc >> 8) ^ crc32_table[(crc & 0xFF) ^ data[i]];
	}
	return crc ^ 0xFFFFFFFF;
}

int mainCRC() {
	generate_crc32_table();

	const char* file_path = "C:/cfagent/temp/AA.zip";  // 대상 파일 경로 설정

	FILE* file = fopen(file_path, "rb");
	if (file == NULL) {
		printf("파일을 열 수 없습니다.\n");
		return 1;
	}

	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	unsigned char* file_data = (unsigned char*)malloc(file_size);
	if (file_data == NULL) {
		printf("메모리 할당 오류\n");
		fclose(file);
		return 1;
	}

	fread(file_data, 1, file_size, file);
	fclose(file);

	unsigned int crc32 = calculate_crc32(file_data, file_size);

	printf("파일 CRC-32: 0x%08X\n", crc32);

	free(file_data);

	return 0;
}

*/
//int mainCheckSum(int argc, char*argv[]) {
//	const char* file_path = "C:/cfagent/temp/AA.zip";  // 대상 파일 경로 설정
//	char msg[1000];
//	printf("ss\n");
////	if (calculate_md5(file_path, (const char*)"9F4726BACB67649E6AB854B4E5F9F3D4", msg)) {
////		printf("MD5 정상 : \n" );
////	}
////	else {
////		printf("MD5 error :[%s] \n", msg);
////	}
//	if (calculate_sha256(file_path, (const char*)"3419FEF9BC2985578ED76C3B1985A5B6A19DF16091BCCB766B617F656B757C1A", msg)) {
//		printf("SHA-256 정상 : \n");
//	}
//	else {
//		printf("SHA-256error :[%s] \n", msg);
//	}
//	if (calculate_crc32(file_path, (const char*)"9F02F8AE", msg)) {
//		printf("CRC 정상 : \n");
//	}
//	else {
//		printf("CR error :[%s] \n", msg);
//	}
//	return 1;
//}
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
