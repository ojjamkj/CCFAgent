
#include "CFAPI.h"
#include <utime.h>
#include <jansson.h>
#include <dirent.h>
#include <regex.h>

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


void CFAPI::listDirectories(const char* path) {

}

void CFAPI::API02(CBRMObj  *m_ObjBuffer2, int m_itemCnt, int pChildSoc, CCSManager *pManage)
{

}

void CFAPI::API04_VIEWDIR(CBRMObj  *m_ObjBuffer2, int m_itemCnt, int pChildSoc, CCSManager *pManage)
{
	printf("[CMD_VIEWDIR] \n");
	m_pChildSoc=pChildSoc;
	m_pManager = pManage;
	m_ObjBuffer = m_ObjBuffer2;
	char temp[100];
	char targetPath[1000];
	char targetRegExp[200];
	int includeSub = 0;
	int defaultGetRows = -1;


	m_ObjBuffer->ReadString(temp);
	// 0. read parameter
	m_ObjBuffer->ReadString(targetPath);//
	printf("TARGET_PATH [%s]\n", targetPath);

	m_ObjBuffer->ReadString(targetRegExp);//
	printf("TARGET_REGEXP [%s] [%d]\n", targetRegExp, strlen(targetRegExp));

	regex_t regex;
	int regExpValid = 0;
	if(strlen(targetRegExp)>0){
		// Compile the regular expression
		int ret = regcomp(&regex, targetRegExp, REG_EXTENDED);
		if (ret != 0) {
			printf("Error compiling regex\n");
		}else{
			regExpValid = 1;
		}
	}else{
		int ret = regcomp(&regex, "(.)*(.)*", REG_EXTENDED);
	}
	printf("TARGET_REGEXP VALID [%d]\n", regExpValid);

	m_ObjBuffer->ReadString(temp);//
	printf("INCLUDE_SUB_DIR [%s]\n", temp);
	if (strcmp(temp, "Y") == 0) {
		includeSub = 1;
	}

	m_ObjBuffer->ReadString(temp);//
	printf("DEFAULT_GET_ROWS [%s]\n", temp);
	defaultGetRows = atoi( temp );

//	m_ObjBuffer->ReadString(temp);


	// 1. get directory info
	json_t *dir_info = json_array();
	get_directory_info(targetPath, dir_info, includeSub, defaultGetRows, regex, regExpValid);
	char *json_str = json_dumps(dir_info, JSON_INDENT(2));

//	printf("json string = %s\n", json_str);
	printf("Size of JSON string: %zu\n", strlen(json_str));
	//	FILE *fp = fopen("directory_info.json", "w");
	//	if (fp) {
	//	        fprintf(fp, "%s\n", json_str);
	//	        fclose(fp);
	//	    }

	// 2. write final result (success)
	m_ObjBuffer->Clear1();
	m_ObjBuffer->WriteLong((long)0);
	m_ObjBuffer->WriteLong((long)0);
	m_ObjBuffer->WriteString("true"); //result
	m_ObjBuffer->WriteString(""); //message
	m_ObjBuffer->WriteLongString(json_str); //message

	free(json_str);
	json_decref(dir_info);
	regfree(&regex);

}


void CFAPI::API06_VIEWFILE6(CBRMObj  *m_ObjBuffer2, int m_itemCnt, int pChildSoc, CCSManager *pManage)
{
	m_pChildSoc = pChildSoc;
	m_pManager = pManage;
	m_ObjBuffer = m_ObjBuffer2;
	char path[1000];
	char temp[10000];
	char errorMsg[1000];
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
		m_ObjBuffer->WriteLong((long)0);

		// final result
		m_ObjBuffer->WriteString("false"); //result
		m_ObjBuffer->WriteString("파일을 열 수 없습니다(size)."); //message
		printf("(end)[%s]\n", "파일을 열 수 없습니다(size).");

		return;
	}

	fileName2 = basename(filePath);
	sprintf(fileName,"%s",fileName2);


	FILE* file = fopen(filePath, "rb");
	if (file == NULL) {
		m_ObjBuffer->Clear1();
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteLong((long)0);

		// final result
		m_ObjBuffer->WriteString("false"); //result
		m_ObjBuffer->WriteString("Failed to open file."); //message
		printf("(end)[%s]\n", "Failed to open file.");

		return;
	}

	// 2. write file
	json_t *jsonFileObject = get_file_info(filePath, errorMsg);
	if (jsonFileObject == NULL) {
		{
			m_ObjBuffer->Clear1();
			m_ObjBuffer->WriteLong((long)0);
			m_ObjBuffer->WriteLong((long)0);

			m_ObjBuffer->WriteString("false"); //result
			m_ObjBuffer->WriteString(errorMsg); //error message
			return;
		}
		char *json_str = json_dumps(jsonFileObject, JSON_INDENT(2));

		// 1.send file information
		m_ObjBuffer->Clear1();
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteString("true"); //result
		m_ObjBuffer->WriteString(""); //message

		m_ObjBuffer->WriteString(json_str);

		free(json_str);
		json_decref(jsonFileObject);

		// 2. write file
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
}

void CFAPI::API07_CREATEFILE(CBRMObj  *m_ObjBuffer2, int m_itemCnt, int pChildSoc, CCSManager *pManage)
{
	printf("[CMD_CREATEFILE] \n");
	m_pChildSoc=pChildSoc;
	m_pManager = pManage;
	m_ObjBuffer = m_ObjBuffer2;
	char path[1000];
	char temp[10000];
	unsigned char *fileData;
	long fileLength=0;
	char filePath[1000];
	char fileChecksum[100];
	char fileLastModifiedDate[50];
	char filePermission[10];
	char fileChecksumType[10];

	// 0. read parameter
	m_ObjBuffer->ReadString(temp);//

	m_ObjBuffer->ReadString(filePath);//
	printf("TARGET_FILE [%s]\n", filePath);

	m_ObjBuffer->ReadString(temp);//
	fileLength = atol(temp);
	printf("FILE_SIZE [%s] [%ld]\n", temp, fileLength );
	fileData = (unsigned char *)malloc( fileLength+1);
	m_ObjBuffer->ReadByteFile(fileData, fileLength);
	//printf("FILE-DATA [%s]\n", fileData);

	m_ObjBuffer->ReadString(fileChecksum);// 빈값 0
	printf("FILE_CHECKSUM [%s]\n", fileChecksum);

	m_ObjBuffer->ReadString(fileChecksumType);// 빈값 0
	printf("CHECKSUM_TYPE [%s]\n", fileChecksumType);

	m_ObjBuffer->ReadString(fileLastModifiedDate);// 빈값 0
	printf("FILE_LAST_MODIFIED [%s]\n", fileLastModifiedDate);

	m_ObjBuffer->ReadString(filePermission);// 빈값 0
	printf("FILE_PERMISSION [%s]\n", filePermission);

	m_ObjBuffer->ReadString(temp);// 빈값 0
	printf("(03)[%s]\n", temp);

	m_ObjBuffer->ReadString(temp);// 빈값 0
	printf("(04)[%s]\n", temp);

	// 1. create file
	int createFileResult = 0;
	createFileResult = createFile(filePath, fileData, fileLength, temp);

	if(createFileResult != 1){

		m_ObjBuffer->Clear1();
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteLong((long)0);

		// final result
		m_ObjBuffer->WriteString("false"); //result
		m_ObjBuffer->WriteString(temp); //message
		printf("(end)[%s]\n", temp);

		return;
	}

	// 2. compare checksum with two file
	if (calculate_sha256(filePath, fileChecksum, temp)) {
		printf("SHA-256 same file : [%s]\n", filePath);
	}
	else {
		printf("SHA-256 error :[%s] \n", temp);
		m_ObjBuffer->Clear1();
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteLong((long)0);

		// final result
		m_ObjBuffer->WriteString("false"); //result
		m_ObjBuffer->WriteString("Error while comparing file checksum"); //message
		printf("(end)[%s]\n", temp);

		return;
	}

	// 3. set last modified date of created file
	size_t modifiedDateLength = strlen(fileLastModifiedDate);
	if(modifiedDateLength){
		struct utimbuf new_times;

		char* endptr;
		long modifiedDate;

		modifiedDate = strtol(fileLastModifiedDate, &endptr, 10);

		new_times.actime = time(NULL);  // Keep the access time the same
		new_times.modtime = modifiedDate/1000;

		if (utime(filePath, &new_times) == 0) {
			printf("File modification date updated successfully. [%s]\n", filePath);
		} else {
			printf("Error while setting file modification date. [%s]\n", filePath);

			m_ObjBuffer->Clear1();
			m_ObjBuffer->WriteLong((long)0);
			m_ObjBuffer->WriteLong((long)0);

			// final result
			m_ObjBuffer->WriteString("false"); //result
			m_ObjBuffer->WriteString("Error while setting file modification date"); //message
			printf("(end)[%s]\n", temp);

			return;
		}
	}

	// 4. set last modified date of created file
	size_t filePermissionLength = strlen(filePermission);
	if(filePermissionLength){
		int octal_permissions = strtol(filePermission, NULL, 8);

		//		mode_t new_permissions = strtol(filePermission, NULL, 8);

		if (chmod(filePath, octal_permissions) == 0) {
			printf("File permissions changed successfully. [%s]\n", filePath);
		} else {
			printf("Error while changing file permissions. [%s]\n", filePath);

			m_ObjBuffer->Clear1();
			m_ObjBuffer->WriteLong((long)0);
			m_ObjBuffer->WriteLong((long)0);

			// final result
			m_ObjBuffer->WriteString("false"); //result
			m_ObjBuffer->WriteString("Error while changing file permissions"); //message
			printf("(end)[%s]\n", temp);

			return;

		}
	}


	// send result
	m_ObjBuffer->Clear1();
	m_ObjBuffer->WriteLong((long)0);
	m_ObjBuffer->WriteLong((long)0);

	// final result
	m_ObjBuffer->WriteString("true"); //result
	m_ObjBuffer->WriteString(""); //message

	printf("(end)\n");
	//	for (int i = 0; i<1; i++) {
	//		m_ObjBuffer->WriteByte((unsigned char)DATA_TYPE_STRING);
	//	}
}

void CFAPI::API28_DELETEFILE(CBRMObj  *m_ObjBuffer2, int m_itemCnt, int pChildSoc, CCSManager *pManage)
{
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
	printf("remove file status [%d]\n", status);
	if (status == 0) {
		m_ObjBuffer->Clear1();
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteLong((long)0);

		m_ObjBuffer->WriteString("true"); //result
		m_ObjBuffer->WriteString(""); //message

	}
	else {
		m_ObjBuffer->Clear1();
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteLong((long)0);
		sprintf(temp, "return code %d ", status);

		// final result
		m_ObjBuffer->WriteString("false"); //result
		m_ObjBuffer->WriteString(temp); //message
	}
}
void CFAPI::API03_PING(CBRMObj  *m_ObjBuffer2, int m_itemCnt, int pChildSoc, CCSManager *pManage)
{
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

	printf(" API100 end  \n");
}

void CFAPI::executeBatchFile(const char*  arguments) {

}
bool CFAPI::compressFolder(const char* folderPath, const char* zipPath) {
	bool success = false;

	return success;
}
