
#include "CFAPI.h"

CFAPI::CFAPI(){

}
 
CFAPI::~CFAPI() {
}
char* CFAPI::formatdate(char *str, time_t val)
{
	strftime(str, 36, "%Y:%m:%d %H:%M:%S", localtime(&val));
	return str;
}


void CFAPI::listFiles(const char* path, const char* regexPattern) {
	DIR* directory = opendir(path);
	if (directory == NULL) {
		perror("디렉토리를 열 수 없습니다.");
		return;
	}

	regex_t regex;
	int regexCompileResult = regcomp(&regex, regexPattern, REG_EXTENDED | REG_NOSUB);
	if (regexCompileResult != 0) {
		char regexErrorBuffer[100];
		regerror(regexCompileResult, &regex, regexErrorBuffer, sizeof(regexErrorBuffer));
		fprintf(stderr, "정규식 컴파일 에러: %s\n", regexErrorBuffer);
		closedir(directory);
		return;
	}

	struct dirent* entry;
	while ((entry = readdir(directory)) != NULL) {
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
			continue; // 현재 디렉토리와 상위 디렉토리 무시
		}

		char fullPath[PATH_MAX];
		snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);

		struct stat fileStat;
		if (stat(fullPath, &fileStat) == -1) {
			perror("파일 정보를 가져올 수 없습니다.");
			continue;
		}

		if (S_ISDIR(fileStat.st_mode)) {
			printf("디렉토리: %s\n", fullPath);
/*
			if (regexec(&regex, entry->d_name, 0, NULL, 0) == 0) {
				printf("디렉토리: %s\n", fullPath);  
			}
*/
			listFiles(fullPath, regexPattern); // 재귀적으로 하위 디렉토리 검색
		}
		else if (S_ISREG(fileStat.st_mode)) {
			printf("파일: %s\n", fullPath);
			/*
			if (regexec(&regex, entry->d_name, 0, NULL, 0) == 0) {
				printf("파일: %s\n", fullPath);
			}
*/
		}
	}

	regfree(&regex);
	closedir(directory);
}

#ifdef _WIN32
void CFAPI::listDirectories(const char* path) {
/*
	WIN32_FIND_DATAA findData;
	HANDLE findHandle;

	char searchPath[MAX_PATH];
	snprintf(searchPath, MAX_PATH, "%s\\*", path);
	char searchPath2[MAX_PATH];

	findHandle = FindFirstFileA(searchPath, &findData);
	if (findHandle == INVALID_HANDLE_VALUE) {
		printf("Failed to open directory: %s\n", path);
		return;
	}

	do {
		printf("fileCount [%d]\n", fileCount);
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0) {
				printf("DIR(0) ==> %s\\%s\n", path,findData.cFileName);
				snprintf(searchPath2, MAX_PATH, "%s\\%s", path, findData.cFileName);
				listDirectories(searchPath2);
			}
		}
		else {
			sprintf(fileList[fileCount], "%s\\%s", path, findData.cFileName);
			fileCount++;
			printf("(FILE(0)==>%s\\%s\n", path,findData.cFileName);
		}
	} while (FindNextFileA(findHandle, &findData));

	FindClose(findHandle);
*/
}
#else
void CFAPI::listDirectories(const char* path) {
/*
	char searchPath[1024];
	snprintf(searchPath, 1024, "%s\\*", path);
	char searchPath2[1024];

	DIR* directory = opendir(searchPath);
	if (directory == NULL) {
		printf("Failed to open directory: %s\n", searchPath);
		return;
	}

	struct dirent* entry;
	while ((entry = readdir(directory)) != NULL) {
		if (entry->d_type == DT_DIR) {
			if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
				printf("%s\n", entry->d_name);
				snprintf(searchPath2, MAX_PATH, "%s/%s", path, entry->d_name);
				listDirectories(searchPath2);
			}
		}
		else {
			printf("(FILE(0)==>%s\n", entry->d_name);
		}
	}

	closedir(directory);
*/
}
/*
int main() {
	const char* path = "/path/to/directory";
	listDirectories(path);

	return 0;
}
*/
#endif






void CFAPI::API02(CBRMObj  *m_ObjBuffer2, int m_itemCnt, int pChildSoc, CCSManager *pManage)
{

}

void CFAPI::API06_VIEWFILE6(CBRMObj  *m_ObjBuffer2, int m_itemCnt, int pChildSoc, CCSManager *pManage)
{
	m_pChildSoc = pChildSoc;
	m_pManager = pManage;
	m_ObjBuffer = m_ObjBuffer2;
	char path[1000];
	char temp[10000];
	unsigned char *fileData;
	long fileLength = 0;
	char filePath[1000];
	char fileName[1000];
	char *fileName2;
	char *temp2;
	struct stat fileinfo;
	char  *ptr;
	m_ObjBuffer->ReadString(temp);// 성공여부 0  printf("(01)[%s]\n", temp);
	printf("TARGET_FILE [%s]\n", temp);

	m_ObjBuffer->ReadString(filePath);// 빈값 0
	printf("TARGET_FILE [%s]\n", filePath);

	if (stat(filePath, &fileinfo) != 0) {
		m_ObjBuffer->Clear1();
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteLong((long)-1);
		m_ObjBuffer->WriteString("파일을 열 수 없습니다(size).");
		return;
	}
#ifdef WIN32
	 
	printf("[입력한 경로]=%s\n", filePath);
	ptr = strrchr(filePath, '/');     //문자열(path)의 뒤에서부터 '\'의 위치를 검색하여 반환

	if (ptr == NULL)
		strcpy(fileName, filePath);
	else
		strcpy(fileName, ptr + 1);

	printf("\n[실행결과]=%s\n", fileName);
#else
	fileName2 = basename(filePath);
	sprintf(fileName,"%s",fileName2);
#endif

	FILE* file = fopen(filePath, "rb");
	if (file == NULL) {
		perror("Failed to open file");
		m_ObjBuffer->Clear1();
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteLong((long)-1);
		m_ObjBuffer->WriteString("Failed to open file.");
		return;
	}
	m_ObjBuffer->Clear1();
	m_ObjBuffer->WriteLong((long)0);
	m_ObjBuffer->WriteLong((long)0);
	m_ObjBuffer->WriteString(fileName);

	printFileAttributes(filePath);

	m_ObjBuffer->WriteLong( fileinfo.st_size);
	printf("fileinfo.st_size =[%d] ", fileinfo.st_size);

	long outLeng = m_ObjBuffer->getLength();
	m_ObjBuffer->setMaxLength(outLeng + fileinfo.st_size);
	printf("tot fileinfo.st_size =[%d] \n",outLeng+ fileinfo.st_size);

	((CCSManager*)m_pManager)->SocWrite(m_pChildSoc, (unsigned char *)m_ObjBuffer->m_Buffer, outLeng);
	unsigned char buffer[10240];
	size_t bytes_read;
	int firstWrite = 0;
	int st = 0;
	while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
		((CCSManager*)m_pManager)->SocWrite(m_pChildSoc, (unsigned char *)buffer, bytes_read);
		printf(" st [%d] \n", st);
		firstWrite += bytes_read;
	}
	fclose(file);
//	((CCSManager*)m_pManager)->SocWriteString(m_pChildSoc, (char *)"1234567890" );
//	printf("(end)[%s]\n", temp);

}
void CFAPI::API07_CREATEFILE(CBRMObj  *m_ObjBuffer2, int m_itemCnt, int pChildSoc, CCSManager *pManage)
{//createfile 
	printf("OJJAMKJ ADD DEBUG LONG AAA\n");
    m_pChildSoc=pChildSoc;
	m_pManager = pManage;
	m_ObjBuffer = m_ObjBuffer2;
	char path[1000];
	char temp[10000];
	unsigned char *fileData;
	long fileLength=0;
	char filePath[1000];
	m_ObjBuffer->ReadString(temp);//  

	m_ObjBuffer->ReadString(filePath);//  
	printf("TARGET_FILE [%s]\n", filePath);

	m_ObjBuffer->ReadString(temp);// 
	fileLength = atol(temp);
	printf("FILE_SIZE [%s] [%ld]\n", temp, fileLength );
	fileData = (unsigned char *)malloc( fileLength+1);
	m_ObjBuffer->ReadByteFile(fileData, fileLength);//  
	createFile(filePath, fileData, fileLength, temp);

	printf("createFile [%s]\n", temp);

	printf("FILE-DATA [%s]\n", fileData);

	m_ObjBuffer->ReadString(temp);// 빈값 0
	printf("FILE_CHECKSUM [%s]\n", temp);

	m_ObjBuffer->ReadString(temp);// 빈값 0
	printf("CHECKSUM_TYPE [%s]\n", temp);

	m_ObjBuffer->ReadString(temp);// 빈값 0
	printf("FILE_LAST_MODIFIED [%s]\n", temp);

	m_ObjBuffer->ReadString(temp);// 빈값 0
	printf("FILE_PERMISSION [%s]\n", temp);


	m_ObjBuffer->ReadString(temp);// 빈값 0
	printf("(03)[%s]\n", temp);

	m_ObjBuffer->ReadString(temp);// 빈값 0
	printf("(04)[%s]\n", temp);

	m_ObjBuffer->Clear1();
	m_ObjBuffer->WriteLong((long)0);
	m_ObjBuffer->WriteLong((long)0);

	m_ObjBuffer->WriteString("msg..");

	printf("(end)[%s]\n", temp);
	//	for (int i = 0; i<1; i++) {
//		m_ObjBuffer->WriteByte((unsigned char)DATA_TYPE_STRING);
//	}
}
void CFAPI::API28_DELETEFILE(CBRMObj  *m_ObjBuffer2, int m_itemCnt, int pChildSoc, CCSManager *pManage)
{//createfile 
	m_pChildSoc = pChildSoc;
	m_pManager = pManage;
	m_ObjBuffer = m_ObjBuffer2;
	char path[1000];
	char temp[10000];
	unsigned char *fileData;
	long fileLength = 0;
	char filePath[1000];
	m_ObjBuffer->ReadString(temp);// 성공여부 0  printf("(01)[%s]\n", temp);

	m_ObjBuffer->ReadString(filePath);// 빈값 0
	printf("TARGET_FILE [%s]\n", filePath);

	m_ObjBuffer->ReadString(temp);// CONNECT_TYPE
	m_ObjBuffer->ReadString(temp);// MACHINE_TYPE

	int status = remove(filePath);
	printf(" status [%d]\n", status);
	if (status == 0) {
		m_ObjBuffer->Clear1();
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteLong((long)0);

	}
	else {
		m_ObjBuffer->Clear1();
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteLong((long)status);
		sprintf(temp, "return code %d ", status);
		m_ObjBuffer->WriteString(temp);
	}
}
void CFAPI::API03_PING(CBRMObj  *m_ObjBuffer2, int m_itemCnt, int pChildSoc, CCSManager *pManage)
{//ping
	m_pChildSoc=pChildSoc;
	m_pManager = pManage;
	m_ObjBuffer = m_ObjBuffer2;
	char path[1000];
	char temp[100];

	m_ObjBuffer->ReadString(temp);// 빈값 0
	printf("[%s]\n", temp);

	m_ObjBuffer->ReadString(temp);// 빈값 0
	printf("[%s]\n", temp);
	m_ObjBuffer->Clear1();
	m_ObjBuffer->WriteLong((long)0);
	m_ObjBuffer->WriteLong((long)0);
	//	m_ObjBuffer->WriteLong((long)1);//rowcount
//	m_ObjBuffer->WriteLong((long)1);//colcount
//	m_ObjBuffer->WriteByte((unsigned char)1); // end 
//	m_ObjBuffer->WriteString("");

//	for (int i = 0; i<1; i++) {
//		m_ObjBuffer->WriteByte((unsigned char)DATA_TYPE_STRING);
//	}
	m_ObjBuffer->WriteString("I AM ALIVE");
}
 
void CFAPI::API01(CBRMObj  *m_ObjBuffer2, int m_itemCnt, int pChildSoc, CCSManager *pManage) {
	m_pChildSoc=pChildSoc;
	m_pManager = pManage;
	m_ObjBuffer = m_ObjBuffer2;
	char path[1000];
	char temp[100];
	int i;
	fileCount = 0;
	m_ObjBuffer->ReadString(temp);// 빈값 0
	// 함수 하나당 두번 Read
	m_ObjBuffer->ReadString(temp);	m_ObjBuffer->ReadString(path); //경로

	listDirectories(path);

	m_ObjBuffer->Clear1();
	m_ObjBuffer->WriteLong((long)0);
	m_ObjBuffer->WriteLong((long)fileCount);//rowcount
	m_ObjBuffer->WriteLong((long)1);//colcount
	m_ObjBuffer->WriteByte((unsigned char)1); // end 
	m_ObjBuffer->WriteString("");
	
 

	for (int i = 0; i<1; i++) {
		m_ObjBuffer->WriteByte((unsigned char)DATA_TYPE_STRING);
	}
	char str[20];
	for (int i = 0; i<fileCount; i++) {
		m_ObjBuffer->WriteString(fileList[i]);
	}

 
}

void CFAPI::API27_DOSEARCH_ONLY_FILE(CBRMObj  *m_ObjBuffer2, int m_itemCnt, int pChildSoc, CCSManager *pManage)
{
	m_pChildSoc = pChildSoc;
	m_pManager = pManage;
	m_ObjBuffer = m_ObjBuffer2;
	char path[1000];
	char temp[100];
	char regexPattern[100];
	int i;
	fileCount = 0;
	m_ObjBuffer->ReadString(temp);// 빈값 0

	m_ObjBuffer->ReadString(path);// TARGET_PATH
	
	printf("PATH :[%s] \n", path);
	m_ObjBuffer->ReadString(regexPattern);// TARGET_REGEXP
	printf("regexPattern :[%s] \n", regexPattern);


	listFiles(path , regexPattern);

}
void CFAPI::generateRandomFileName(char* fileName) {
	// 시간 기반 시드 설정
	char tempfile[100];
	srand(time(NULL));

	const char* chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	int charsLength = strlen(chars);

	for (int i = 0; i < 30; i++) {
		// chars 배열에서 무작위 문자 선택
		int randomIndex = rand() % charsLength;
		tempfile[i] = chars[randomIndex];
	}
	tempfile[30] = 0;
	// 파일 확장자 추가 (예시: .txt)
	sprintf(fileName, "%s", tempfile);
}
	void CFAPI::API100(CBRMObj  *m_ObjBuffer2, int m_itemCnt, int pChildSoc, CCSManager *pManage)
	{
		m_pChildSoc = pChildSoc;
		m_pManager=pManage;
		m_ObjBuffer = m_ObjBuffer2;

		char *batchFilePath = "c:\\cfagent\\azip.cmd ";
//		const char* folderPath = "C:\\eclise_Engine\\W_BR\\BRMiner_engine4.5\\00SrcCFAgent\\Admin";
		char zipPath[1000]  ;
		char path[1000];
		char temp[100];
		char batchCmd[1000];
		int i;
		printf("0001: \n");
		generateRandomFileName(path);
		printf("0001: zipPath[%s]\n", path);
		sprintf(zipPath, "c:\\cfagent\\temp\\%s.zip", path);
		printf("0001:zipPath [%s]\n" , zipPath);
		printf("0001: \n");
		fileCount = 0;
		m_ObjBuffer->ReadString(temp);// 빈값 0
		printf("001:%s \n", temp);
		m_ObjBuffer->ReadString(temp);// 빈값 0
		printf("002:%s \n", temp);
		m_ObjBuffer->ReadString(path); //경로
		printf("003:%s \n", path);
#ifdef _WIN32
//			compressFolder(folderPath, zipPath);
		sprintf(batchCmd,"%s   %s   %s", batchFilePath, zipPath , path );
		printf("004:%s \n", batchCmd);
		executeBatchFile(batchCmd);
		
			
			
			HANDLE directory_handle;
		WIN32_FIND_DATA file_data;

		FILE* file = fopen(zipPath, "rb");
		char search_path[MAX_PATH];
		snprintf(search_path, sizeof(search_path), "%s", zipPath);
		printf("(%s) \n", search_path);
		directory_handle = FindFirstFile(search_path, &file_data);

		if (directory_handle == INVALID_HANDLE_VALUE) {
			printf("Failed to open directory\n");
			return;
		}
		if (!(file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			ULONGLONG file_size = ((ULONGLONG)file_data.nFileSizeHigh << 32) | file_data.nFileSizeLow;
			printf("File: %s, Size: %llu bytes\n", file_data.cFileName, file_size);

			m_ObjBuffer->Clear1();
			m_ObjBuffer->WriteLong((long)0);
			m_ObjBuffer->WriteLong((long)99);//rowcount
			m_ObjBuffer->WriteLong((long)1);//colcount
			m_ObjBuffer->WriteByte((unsigned char)1); // end 
			m_ObjBuffer->WriteString("");
			printf("API100-100  \n");

			for (int i = 0; i < 1; i++) {
				m_ObjBuffer->WriteByte((unsigned char)DATA_TYPE_STRING);
			}
			printf("API100-101 file_size =[%d] \n", file_size);
			m_ObjBuffer->WriteLong(file_size);//colcount
			printf("API100-102  \n");

			long outLeng = m_ObjBuffer->getLength();
			printf("API100-103  outLeng + file_size= [%d]\n", outLeng + file_size);
			m_ObjBuffer->setMaxLength(outLeng + file_size);

			printf("API100-104  \n");
			((CCSManager*)m_pManager)->SocWrite(m_pChildSoc, (unsigned char *)m_ObjBuffer->m_Buffer, outLeng);
			printf("API100-105  \n");
			unsigned char buffer[10240];
			size_t bytes_read;
			int firstWrite = 0;
			while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
				printf("bytes_read [%d]\n", bytes_read);
//				st = m_ObjBuffer->WriteByteFile(buffer, bytes_read, firstWrite, file_size);
				printf("API100-106  \n");
				((CCSManager*)m_pManager)->SocWrite(m_pChildSoc, (unsigned char *)buffer, bytes_read);
				firstWrite += bytes_read;
			}
			fclose(file);
			printf(" firstWrite [%d] \n", firstWrite);
		}
#else
		printf(" API100 end  \n");
#endif
}
	void CFAPI::executeBatchFile(const char*  arguments) {
		/*
		FILE* fp = NULL;
		char buffer[1024];

		STARTUPINFOA si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof(STARTUPINFOA));
		si.cb = sizeof(STARTUPINFOA);
		ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

		// CMD 파일 실행
		if (CreateProcessA(NULL, (LPSTR)arguments, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
			WaitForSingleObject(pi.hProcess, INFINITE);
			 
			// CMD 결과 읽기
			DWORD bytesRead;
			if (ReadFile(pi.hProcess, buffer, 1024 - 1, &bytesRead, NULL)) {
				buffer[bytesRead] = '\0';
				printf("CMD output:\n%s\n", buffer);
			}
			else {
			 
			}

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
		else {
			printf("Failed to execute CMD file: %s\n", arguments);
		} */
	}
	bool CFAPI::compressFolder(const char* folderPath, const char* zipPath) {
		bool success = false;
		/*
		printf("001 \n");
		HANDLE hZipFile = CreateFile(zipPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		printf("002 \n");
		if (hZipFile == INVALID_HANDLE_VALUE) {
			printf("Failed to create zip file: %s\n", zipPath);
			return false;
		}
		printf("003 \n");

		bool success = false;

		WIN32_FIND_DATAA findData;
		HANDLE hFind = FindFirstFileA((char*)folderPath, &findData);
		printf("004 \n");
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				printf("005 \n");
				if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					printf("006 \n");
					char filePath[MAX_PATH];
					snprintf(filePath, MAX_PATH, "%s\\%s", folderPath, findData.cFileName);

					HANDLE hFile = CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
					printf("007 \n");
					if (hFile != INVALID_HANDLE_VALUE) {
						printf("008 \n");
						DWORD fileSize = GetFileSize(hFile, NULL);
						printf("009 \n");
						if (fileSize != INVALID_FILE_SIZE) {
							BYTE* buffer = (BYTE*)malloc(fileSize);
							DWORD bytesRead;

							printf("010 \n");
							if (ReadFile(hFile, buffer, fileSize, &bytesRead, NULL)) {
								printf("011 \n");
								DWORD bytesWritten;

								if (WriteFile(hZipFile, buffer, bytesRead, &bytesWritten, NULL) && bytesWritten == bytesRead) {
									printf("012 \n");
									success = true;
								}
								else {
									printf("Failed to write to zip file\n");
									success = false;
									break;
								}
							}

							free(buffer);
							printf("013 \n");
						}

						printf("014 \n");
						CloseHandle(hFile);
					}
				}
			} while (FindNextFileA(hFind, &findData));
			printf("020 \n");

			FindClose(hFind);
			printf("021 \n");
		}

		CloseHandle(hZipFile);
		printf("030 \n");
		*/
		return success;
	}
