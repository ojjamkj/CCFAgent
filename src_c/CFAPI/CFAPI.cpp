
#include "CFAPI.h"
#include "MRConfig.h"
#include <utime.h>
#include <zlib.h>

CFAPI::CFAPI(){

}

CFAPI::~CFAPI() {
}


void CFAPI::API03_PING(CBRMObj  *m_ObjBuffer2, int m_itemCnt, int pChildSoc, CCSManager *pManage)
{
	_WriteLog(_LOG_LOG, "[CMD_PING] \n");
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
	m_ObjBuffer->WriteString("true"); //result
	m_ObjBuffer->WriteString(""); //message

	m_ObjBuffer->WriteString(CHANGEFLOW_VERSION);
	m_ObjBuffer->WriteString(configRec.startTime);

	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		m_ObjBuffer->WriteString(cwd);
	}else{
		m_ObjBuffer->WriteString("");
	}



}



void CFAPI::API04_VIEWDIR(CBRMObj  *m_ObjBuffer2, int m_itemCnt, int pChildSoc, CCSManager *pManage)
{
	_WriteLog(_LOG_LOG, "[CMD_VIEWDIR] \n");
	m_pChildSoc=pChildSoc;
	m_pManager = pManage;
	m_ObjBuffer = m_ObjBuffer2;
	char logMsgTemp[10000];
	char temp[100];
	char targetPath[1000];
	char targetRegExp[200];
	int includeSub = 0;
	int defaultGetRows = -1;
	int includeMode = 0; //file and dir

	_WriteLog(_LOG_LOG, "[CMD_VIEWDIR ==> start to display params] \n");
	m_ObjBuffer->ReadString(temp);
	// 0. read parameter
	m_ObjBuffer->ReadString(targetPath);//
	sprintf(logMsgTemp, "param1: TARGET_PATH [%s]\n", targetPath);
	_WriteLog(_LOG_LOG, logMsgTemp);

	m_ObjBuffer->ReadString(targetRegExp);//
	printf("TARGET_REGEXP [%s] [%d]\n", targetRegExp, strlen(targetRegExp));

	sprintf(logMsgTemp, "param2: TARGET_REGEXP [%s]\n", targetRegExp);
	_WriteLog(_LOG_LOG, logMsgTemp);

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

	sprintf(logMsgTemp, "TARGET_REGEXP VALID [%d]\n", regExpValid);
	_WriteLog(_LOG_LOG, logMsgTemp);

	m_ObjBuffer->ReadString(temp);//
	if (strcmp(temp, "Y") == 0) {
		includeSub = 1;
		includeMode = 2; //only file
	}

	sprintf(logMsgTemp, "param3: INCLUDE_SUB_DIR [%s]\n", temp);
	_WriteLog(_LOG_LOG, logMsgTemp);

	m_ObjBuffer->ReadString(temp);//
	printf("DEFAULT_GET_ROWS [%s]\n", temp);
	defaultGetRows = atoi( temp );

	sprintf(logMsgTemp, "param4: DEFAULT_GET_ROWS [%s]\n", temp);
	_WriteLog(_LOG_LOG, logMsgTemp);

	_WriteLog(_LOG_LOG, "[CMD_VIEWDIR ==> end to display params] \n");
//	m_ObjBuffer->ReadString(temp);


	// 1. get directory info

	const char* tempDirPath = getenv("TMPDIR");
	if (tempDirPath == NULL) {
		tempDirPath = "/tmp"; // Default temporary directory path in Linux
	}
	char scanTempFilePath[100]; // Adjust the array size based on your needs
	sprintf(scanTempFilePath, "%s/ViewDirResult_XXXXXX", tempDirPath);
	int scanTempFileDescriptor = mkstemp(scanTempFilePath);
	sprintf(logMsgTemp, "tempFilePath is %s %d\n", scanTempFilePath, scanTempFileDescriptor);
	_WriteLog(_LOG_LOG, logMsgTemp);
	if (scanTempFileDescriptor == -1) {
	    m_ObjBuffer->Clear1();
	    m_ObjBuffer->WriteLong((long)0);
	    m_ObjBuffer->WriteLong((long)0);
	    m_ObjBuffer->WriteString("false"); //result
	    m_ObjBuffer->WriteString("Error creating the tempFile"); //message
	    return;
	}
	sprintf(logMsgTemp, "Temporary file created: %s\n", scanTempFilePath);
	_WriteLog(_LOG_LOG, logMsgTemp);
	FILE *fw = fdopen(scanTempFileDescriptor, "w");
	if (fw == NULL) {
	    m_ObjBuffer->Clear1();
	    m_ObjBuffer->WriteLong((long)0);
	    m_ObjBuffer->WriteLong((long)0);
	    m_ObjBuffer->WriteString("false"); //result
	    m_ObjBuffer->WriteString("Error opening the tempFile"); //message

	    if (scanTempFilePath != NULL) unlink(scanTempFilePath);
	    return;
	}


	int appendedCount = 0;
	fprintf(fw, "[");
	int result = get_directory_info(targetPath, fw, includeSub, defaultGetRows, regex, regExpValid, includeMode, &appendedCount);
	fprintf(fw, "]");
	fflush(fw);

	if(!result){
		if (fw != NULL) fclose(fw);
		if (scanTempFilePath != NULL) unlink(scanTempFilePath);

		m_ObjBuffer->Clear1();
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteString("false"); //result
		m_ObjBuffer->WriteString("fail to get directory info."); //message
		return;
	}


	// 2. read file contents
	char* fileContents = readFileContents(scanTempFilePath);
	// 3. write final result (success)
	m_ObjBuffer->Clear1();
	m_ObjBuffer->WriteLong((long)0);
	m_ObjBuffer->WriteLong((long)0);
	m_ObjBuffer->WriteString("true"); //result
	m_ObjBuffer->WriteString(""); //message
	m_ObjBuffer->WriteLongString(fileContents); //message

	free(fileContents);
	regfree(&regex);
	if (fw != NULL) fclose(fw);
	if (scanTempFilePath != NULL) unlink(scanTempFilePath);

}


void CFAPI::API06_VIEWFILE6(CBRMObj  *m_ObjBuffer2, int m_itemCnt, int pChildSoc, CCSManager *pManage)
{
	_WriteLog(_LOG_LOG, "[CMD_VIEWFILE] \n");
	m_pChildSoc = pChildSoc;
	m_pManager = pManage;
	m_ObjBuffer = m_ObjBuffer2;
	char logMsgTemp[10000];
	char temp[10000];
	char errorMsg[1000];
	unsigned char *fileData;
	long fileLength = 0;
	char filePath[500];
	struct stat fileinfo;


	_WriteLog(_LOG_LOG, "[CMD_VIEWFILE ==> start to display params] \n");
	m_ObjBuffer->ReadString(temp);

	m_ObjBuffer->ReadString(filePath);
	sprintf(logMsgTemp, "param1: TARGET_FILE = [%s]\n", filePath);
	_WriteLog(_LOG_LOG, logMsgTemp);
//
	_WriteLog(_LOG_LOG, "[CMD_VIEWFILE ==> end to display params] \n");

	if (stat(filePath, &fileinfo) != 0) {
		m_ObjBuffer->Clear1();
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteLong((long)0);

		// final result
		m_ObjBuffer->WriteString("false"); //result
		m_ObjBuffer->WriteString("파일 속성 정보를 읽을 수 없습니다."); //message
		_WriteLog(_LOG_ERR, "[CMD_VIEWFILE ==> 파일 속성 정보를 읽을 수 없습니다.] \n");

		return;
	}

	FILE* file = fopen(filePath, "rb");
	if (file == NULL) {
		m_ObjBuffer->Clear1();
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteLong((long)0);

		// final result
		m_ObjBuffer->WriteString("false"); //result
		m_ObjBuffer->WriteString("파일을 열 수 없습니다."); //message
		_WriteLog(_LOG_ERR, "[CMD_VIEWFILE ==> 파일을 열 수 없습니다.] \n");

		return;
	}

	// 2. write file
//	json_t *jsonFileObject = get_file_info(filePath, errorMsg);
	char* json_str = get_file_info(filePath, errorMsg);
	_WriteLog(_LOG_LOG, json_str);

	if (json_str == NULL)
	{
		free(json_str);
		m_ObjBuffer->Clear1();
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteLong((long)0);

		m_ObjBuffer->WriteString("false"); //result
		m_ObjBuffer->WriteString(errorMsg); //error message
		return;
	}


//	char *json_str = json_dumps(jsonFileObject, JSON_COMPACT);

	// 1.send file information
	m_ObjBuffer->Clear1();
	m_ObjBuffer->WriteLong((long)0);
	m_ObjBuffer->WriteLong((long)0);
	m_ObjBuffer->WriteString("true"); //result
	m_ObjBuffer->WriteString(""); //message

	m_ObjBuffer->WriteString(json_str);

	free(json_str);

	int read = fileinfo.st_mode & S_IRUSR ? 1 : 0;

	if(read){
		// 2. write file
		m_ObjBuffer->WriteLong( fileinfo.st_size);
		sprintf(logMsgTemp, "[CMD_VIEWFILE] fileinfo.st_size =[%d] \n", fileinfo.st_size);
		_WriteLog(_LOG_LOG, logMsgTemp);

		long outLeng = m_ObjBuffer->getLength();
		m_ObjBuffer->setMaxLength(outLeng + fileinfo.st_size);


		((CCSManager*)m_pManager)->SocWrite(m_pChildSoc, (unsigned char *)m_ObjBuffer->m_Buffer, outLeng);
		unsigned char buffer[10240];
		size_t bytes_read;
		int firstWrite = 0;
		int st = 0;
		while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
			((CCSManager*)m_pManager)->SocWrite(m_pChildSoc, (unsigned char *)buffer, bytes_read);
			//		printf(" st [%d] \n", st);
			firstWrite += bytes_read;
		}

	}

	fclose(file);
}

void CFAPI::API07_CREATEFILE(CBRMObj  *m_ObjBuffer2, int m_itemCnt, int pChildSoc, CCSManager *pManage)
{
	_WriteLog(_LOG_LOG, "[CMD_CREATEFILE] \n");
	m_pChildSoc=pChildSoc;
	m_pManager = pManage;
	m_ObjBuffer = m_ObjBuffer2;
	char logMsgTemp[10000];
	char temp[10000];
	unsigned char *fileData;
	long fileLength=0;
	char filePath[1000];
	char fileChecksum[100];
	char fileLastModifiedDate[50];
	char filePermission[10];
	char fileChecksumType[10];

	_WriteLog(_LOG_LOG, "[CMD_CREATEFILE ==> start to display params] \n");
	// 0. read parameter
	m_ObjBuffer->ReadString(temp);//

	m_ObjBuffer->ReadString(filePath);//
	printf("TARGET_FILE [%s]\n", filePath);

	sprintf(logMsgTemp, "param1: TARGET_FILE = [%s]\n", filePath);
	_WriteLog(_LOG_LOG, logMsgTemp);

	m_ObjBuffer->ReadString(temp);//
	fileLength = atol(temp);
	sprintf(logMsgTemp, "param2: FILE_SIZE [%ld]\n", temp);
	_WriteLog(_LOG_LOG, logMsgTemp);
	fileData = (unsigned char *)malloc( fileLength+1);
	m_ObjBuffer->ReadByteFile(fileData, fileLength);


	m_ObjBuffer->ReadString(fileChecksum);// 빈값 0
	printf("FILE_CHECKSUM [%s]\n", fileChecksum);

	sprintf(logMsgTemp, "param3: FILE_CHECKSUM [%s]\n", fileChecksum);
	_WriteLog(_LOG_LOG, logMsgTemp);

	m_ObjBuffer->ReadString(fileChecksumType);// 빈값 0
	printf("CHECKSUM_TYPE [%s]\n", fileChecksumType);

	sprintf(logMsgTemp, "param4: CHECKSUM_TYPE [%s]\n", fileChecksumType);
	_WriteLog(_LOG_LOG, logMsgTemp);

	m_ObjBuffer->ReadString(fileLastModifiedDate);// 빈값 0
	printf("FILE_LAST_MODIFIED [%s]\n", fileLastModifiedDate);

	sprintf(logMsgTemp, "param5: FILE_LAST_MODIFIED [%s]\n", fileLastModifiedDate);
	_WriteLog(_LOG_LOG, logMsgTemp);

	m_ObjBuffer->ReadString(filePermission);// 빈값 0
	printf("FILE_PERMISSION [%s]\n", filePermission);

	sprintf(logMsgTemp, "param6: FILE_PERMISSION [%s]\n", filePermission);
	_WriteLog(_LOG_LOG, logMsgTemp);

	m_ObjBuffer->ReadString(temp);// 빈값 0
	printf("(03)[%s]\n", temp);

	m_ObjBuffer->ReadString(temp);// 빈값 0
	printf("(04)[%s]\n", temp);

	_WriteLog(_LOG_LOG, "[CMD_CREATEFILE ==> end to display params] \n");

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
		_WriteLog(_LOG_ERR, temp);

		return;
	}

	// 2. compare checksum with two file
//	if (calculate_sha256(filePath, fileChecksum, temp)) {
//		printf("SHA-256 same file : [%s]\n", filePath);
//	}
//	else {
//		printf("SHA-256 error :[%s] \n", temp);
//		m_ObjBuffer->Clear1();
//		m_ObjBuffer->WriteLong((long)0);
//		m_ObjBuffer->WriteLong((long)0);
//
//		// final result
//		m_ObjBuffer->WriteString("false"); //result
//		m_ObjBuffer->WriteString("Error while comparing file checksum"); //message
//		printf("(end)[%s]\n", temp);
//
//		return;
//	}

	// 3. set last modified date of created file
	size_t modifiedDateLength = strlen(fileLastModifiedDate);
	if(modifiedDateLength > 0 ){
		struct utimbuf new_times;

		char* endptr;
		long modifiedDate;

		modifiedDate = strtol(fileLastModifiedDate, &endptr, 10);

		new_times.actime = time(NULL);  // Keep the access time the same
		new_times.modtime = modifiedDate/1000;

		if (utime(filePath, &new_times) == 0) {
			sprintf(logMsgTemp, "File modification date updated successfully. [%s]\n", filePath);
			_WriteLog(_LOG_LOG, logMsgTemp);
		} else {
			sprintf(logMsgTemp, "Error while setting file modification date. [%s]\n", filePath);
			_WriteLog(_LOG_ERR, logMsgTemp);

			m_ObjBuffer->Clear1();
			m_ObjBuffer->WriteLong((long)0);
			m_ObjBuffer->WriteLong((long)0);

			// final result
			m_ObjBuffer->WriteString("false"); //result
			m_ObjBuffer->WriteString("Error while setting file modification date"); //message

			return;
		}
	}

	// 4. set last modified date of created file
	size_t filePermissionLength = strlen(filePermission);
	if(filePermissionLength > 0){
		int octal_permissions = strtol(filePermission, NULL, 8);

		//		mode_t new_permissions = strtol(filePermission, NULL, 8);

		if (chmod(filePath, octal_permissions) == 0) {
			sprintf(logMsgTemp, "File permissions changed successfully. [%s]\n", filePath);
			_WriteLog(_LOG_LOG, logMsgTemp);
		} else {
			sprintf(logMsgTemp, "Error while changing file permissions. [%s]\n", filePath);
			_WriteLog(_LOG_LOG, logMsgTemp);

			m_ObjBuffer->Clear1();
			m_ObjBuffer->WriteLong((long)0);
			m_ObjBuffer->WriteLong((long)0);

			// final result
			m_ObjBuffer->WriteString("false"); //result
			m_ObjBuffer->WriteString("Error while changing file permissions"); //message

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

	//	for (int i = 0; i<1; i++) {
	//		m_ObjBuffer->WriteByte((unsigned char)DATA_TYPE_STRING);
	//	}
}

void CFAPI::API15_BUILD(CBRMObj  *m_ObjBuffer2, int m_itemCnt, int pChildSoc, CCSManager *pManage)
{
	_WriteLog(_LOG_LOG, "[CMD_BUILD] \n");
	m_pChildSoc=pChildSoc;
	m_pManager = pManage;
	m_ObjBuffer = m_ObjBuffer2;
	char temp[10000];
	char logMsgTemp[10000];

	char buildLoc[150];
	char buildFileType[1];

	_WriteLog(_LOG_LOG, "[CMD_BUILD ==> start to display params] \n");

	// 0. read parameter
	m_ObjBuffer->ReadString(temp);//

	m_ObjBuffer->ReadString(buildLoc);//
	printf("BUILD LOC [%s]\n", buildLoc);

	sprintf(logMsgTemp, "param1: BUILD LOC = [%s]\n", buildLoc);
	_WriteLog(_LOG_LOG, logMsgTemp);

	m_ObjBuffer->ReadString(buildFileType);//
	printf("BUILD FILE TYPE [%s]\n", buildFileType );

	sprintf(logMsgTemp, "param1: BUILD FILE TYPE = [%s]\n", buildFileType);
	_WriteLog(_LOG_LOG, logMsgTemp);

	_WriteLog(_LOG_LOG, "[CMD_BUILD ==> end to display params] \n");

	// 1. run shell
    char command[100];
    char *buffer = NULL;
    size_t buffer_size = 0;
    FILE *fp;
    int return_value;

    // Example shell command: "ls -l" (replace with your desired command)
//    const char *cmd = "ls -l";

    // Open a pipe to execute the shell command and read its output
    fp = popen(buildLoc, "r");
    if (fp == NULL) {
        m_ObjBuffer->Clear1();
        m_ObjBuffer->WriteLong((long)0);
        m_ObjBuffer->WriteLong((long)0);

        // final result
        m_ObjBuffer->WriteString("false"); //result
        m_ObjBuffer->WriteString("Failed to run the command."); //message
        _WriteLog(_LOG_ERR, "Failed to run the command.\n");
        return;
    }

    // Read the command output line by line and store in dynamically allocated memory
    while (1) {
        char *line = NULL;
        size_t line_size = 0;
        ssize_t read = getline(&line, &line_size, fp);
        if (read == -1) {
            // End of file or error
            free(line);
            break;
        }

        // Resize the buffer to accommodate the new line
        buffer = (char*)realloc(buffer, buffer_size + read + 1);
        if (buffer == NULL) {
            m_ObjBuffer->Clear1();
            m_ObjBuffer->WriteLong((long)0);
            m_ObjBuffer->WriteLong((long)0);

            // final result
            m_ObjBuffer->WriteString("false"); //result
            m_ObjBuffer->WriteString("Memory allocation failed."); //message
            _WriteLog(_LOG_ERR, "Memory allocation failed.\n");
            return;
        }

        // Copy the new line to the buffer
        strncpy(buffer + buffer_size, line, read);
        buffer_size += read;

        free(line);
    }

    // Close the pipe
    pclose(fp);

    // Null-terminate the buffer to make it a valid C string
    buffer[buffer_size] = '\0';

    // Print the console output
    printf("%s", buffer);

    // Get the return value of the shell command
    return_value = WEXITSTATUS(return_value);


    sprintf(logMsgTemp, "Return value: %d\n", return_value);
    _WriteLog(_LOG_LOG, logMsgTemp);
    sprintf(temp, "%d", return_value);
    // Free the allocated memory for the buffer

	// 2. send result
	m_ObjBuffer->Clear1();
	m_ObjBuffer->WriteLong((long)0);
	m_ObjBuffer->WriteLong((long)0);

	// final result
	m_ObjBuffer->WriteString("true"); //result
	m_ObjBuffer->WriteString(temp); //return value
	m_ObjBuffer->WriteLongString(buffer); //console output

	free(buffer);

}

void CFAPI::API28_DELETEFILE(CBRMObj  *m_ObjBuffer2, int m_itemCnt, int pChildSoc, CCSManager *pManage)
{
	_WriteLog(_LOG_LOG, "[CMD_DELETEFILE] \n");
	m_ObjBuffer = m_ObjBuffer2;
	char logMsgTemp[10000];
	char temp[10000];
	char filePath[1000];

	_WriteLog(_LOG_LOG, "[CMD_DELETEFILE ==> start to display params] \n");

	m_ObjBuffer->ReadString(temp);

	m_ObjBuffer->ReadString(filePath);

	sprintf(logMsgTemp, "param1: TARGET_FILE = [%s]\n", filePath);
	_WriteLog(_LOG_LOG, logMsgTemp);

	m_ObjBuffer->ReadString(temp);
	m_ObjBuffer->ReadString(temp);

	_WriteLog(_LOG_LOG, "[CMD_DELETEFILE ==> end to display params] \n");

	int status = remove(filePath);
	sprintf(logMsgTemp, "remove file status [%d]\n", status);
	_WriteLog(_LOG_LOG, logMsgTemp);

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

		// final result
		m_ObjBuffer->WriteString("false"); //result
		m_ObjBuffer->WriteString(temp); //message
	}
}
void CFAPI::API26_DOSEARCH_ONLY_DIR(CBRMObj  *m_ObjBuffer2, int m_itemCnt, int pChildSoc, CCSManager *pManage)
{
	_WriteLog(_LOG_LOG, "[CMD_DOSEARCH_ONLY_DIR] \n");
	m_pChildSoc=pChildSoc;
	m_pManager = pManage;
	m_ObjBuffer = m_ObjBuffer2;

	char logMsgTemp[10000];
	char temp[100];
	char targetPath[1000];
	char targetRegExp[200];
	int includeSub = true;
	int defaultGetRows = -1;
	int includeMode = 1; //only dir

	_WriteLog(_LOG_LOG, "[CMD_DOSEARCH_ONLY_DIR ==> start to display params] \n");
	m_ObjBuffer->ReadString(temp);
	// 0. read parameter
	m_ObjBuffer->ReadString(targetPath);//
	sprintf(logMsgTemp, "param1: TARGET_PATH [%s]\n", targetPath);
	_WriteLog(_LOG_LOG, logMsgTemp);

	regex_t regex;
	int regExpValid = 0;
	int ret = regcomp(&regex, "(.)*(.)*", REG_EXTENDED);

	_WriteLog(_LOG_LOG, "[CMD_DOSEARCH_ONLY_DIR ==> end to display params] \n");

	// 1. get directory info
	const char* tempDirPath = getenv("TMPDIR");
	if (tempDirPath == NULL) {
		tempDirPath = "/tmp"; // Default temporary directory path in Linux
	}
	char scanTempFilePath[100]; // Adjust the array size based on your needs
	sprintf(scanTempFilePath, "%s/SearchOnlyDirResult_XXXXXX", tempDirPath);
	int scanTempFileDescriptor = mkstemp(scanTempFilePath);
	sprintf(logMsgTemp, "tempFilePath is %s %d\n", scanTempFilePath, scanTempFileDescriptor);
	_WriteLog(_LOG_LOG, logMsgTemp);
	if (scanTempFileDescriptor == -1) {
	    m_ObjBuffer->Clear1();
	    m_ObjBuffer->WriteLong((long)0);
	    m_ObjBuffer->WriteLong((long)0);
	    m_ObjBuffer->WriteString("false"); //result
	    m_ObjBuffer->WriteString("Error creating the tempFile"); //message
	    return;
	}
	sprintf(logMsgTemp, "Temporary file created: %s\n", scanTempFilePath);
	_WriteLog(_LOG_LOG, logMsgTemp);
	FILE *fw = fdopen(scanTempFileDescriptor, "w");
	if (fw == NULL) {
	    m_ObjBuffer->Clear1();
	    m_ObjBuffer->WriteLong((long)0);
	    m_ObjBuffer->WriteLong((long)0);
	    m_ObjBuffer->WriteString("false"); //result
	    m_ObjBuffer->WriteString("Error opening the tempFile"); //message

	    if (scanTempFilePath != NULL) unlink(scanTempFilePath);
	    return;
	}


	int appendedCount = 0;
	fprintf(fw, "[");
	int result = get_directory_info(targetPath, fw, includeSub, defaultGetRows, regex, regExpValid, includeMode, &appendedCount);
	fprintf(fw, "]");
	fflush(fw);

	if(!result){
		if (fw != NULL) fclose(fw);
		if (scanTempFilePath != NULL) unlink(scanTempFilePath);

		m_ObjBuffer->Clear1();
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteString("false"); //result
		m_ObjBuffer->WriteString("fail to get directory info."); //message
		return;
	}

	// 2. read file contents
	char* fileContents = readFileContents(scanTempFilePath);
	// 3. write final result (success)
	m_ObjBuffer->Clear1();
	m_ObjBuffer->WriteLong((long)0);
	m_ObjBuffer->WriteLong((long)0);
	m_ObjBuffer->WriteString("true"); //result
	m_ObjBuffer->WriteString(""); //message
	m_ObjBuffer->WriteLongString(fileContents); //message

	free(fileContents);
	regfree(&regex);
	if (fw != NULL) fclose(fw);
	if (scanTempFilePath != NULL) unlink(scanTempFilePath);

}

void CFAPI::API27_DOSEARCH_ONLY_FILE(CBRMObj  *m_ObjBuffer2, int m_itemCnt, int pChildSoc, CCSManager *pManage)
{
	_WriteLog(_LOG_LOG, "[CMD_DOSEARCH_ONLY_FILE] \n");
	m_pChildSoc=pChildSoc;
	m_pManager = pManage;
	m_ObjBuffer = m_ObjBuffer2;
	unsigned char *fileData;
	struct stat fileinfo;

	char temp[100];
	char logMsgTemp[10000];
	char targetPath[1000];
	char targetRegExp[200];
	int includeSub = true;
	int defaultGetRows = -1;
	int includeMode = 2; // only file

	_WriteLog(_LOG_LOG, "[CMD_DOSEARCH_ONLY_FILE ==> start to display params] \n");
	m_ObjBuffer->ReadString(temp);
	// 0. read parameter
	m_ObjBuffer->ReadString(targetPath);//

	sprintf(logMsgTemp, "param1: TARGET_PATH [%s]\n", targetPath);
    _WriteLog(_LOG_LOG, logMsgTemp);

	m_ObjBuffer->ReadString(targetRegExp);//
	sprintf(logMsgTemp, "param2: TARGET_REGEXP [%s] [%d]\n", targetRegExp, strlen(targetRegExp));
    _WriteLog(_LOG_LOG, logMsgTemp);


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
	sprintf(logMsgTemp, "param2: TARGET_REGEXP VALID [%d]\n", regExpValid);
    _WriteLog(_LOG_LOG, logMsgTemp);


	m_ObjBuffer->ReadString(temp);//

	if (strcmp(temp, "Y") == 0) {
		includeSub = 1;
	}
	sprintf(logMsgTemp, "param3: INCLUDE_SUB_DIR [%s]\n", temp);
    _WriteLog(_LOG_LOG, logMsgTemp);

	_WriteLog(_LOG_LOG, "[CMD_DOSEARCH_ONLY_FILE ==> end to display params] \n");

	// 1. get directory info
	const char* tempDirPath = getenv("TMPDIR");
	if (tempDirPath == NULL) {
		tempDirPath = "/tmp"; // Default temporary directory path in Linux
	}
	char scanTempFilePath[100]; // Adjust the array size based on your needs
	sprintf(scanTempFilePath, "%s/SearchOnlyFileResult_XXXXXX", tempDirPath);
	int scanTempFileDescriptor = mkstemp(scanTempFilePath);
	sprintf(logMsgTemp, "tempFilePath is %s %d\n", scanTempFilePath, scanTempFileDescriptor);
	_WriteLog(_LOG_LOG, logMsgTemp);
	if (scanTempFileDescriptor == -1) {
		m_ObjBuffer->Clear1();
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteString("false"); //result
		m_ObjBuffer->WriteString("Error creating the tempFile"); //message
		return;
	}
	sprintf(logMsgTemp, "Temporary file created: %s\n", scanTempFilePath);
	_WriteLog(_LOG_LOG, logMsgTemp);
	FILE *fw = fdopen(scanTempFileDescriptor, "w");
	if (fw == NULL) {
		m_ObjBuffer->Clear1();
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteString("false"); //result
		m_ObjBuffer->WriteString("Error opening the tempFile"); //message

		if (scanTempFilePath != NULL) unlink(scanTempFilePath);
		return;
	}


	int appendedCount = 0;
	fprintf(fw, "[");
	int result = get_directory_info(targetPath, fw, includeSub, defaultGetRows, regex, regExpValid, includeMode, &appendedCount);
	fprintf(fw, "]");
	fflush(fw);

	if(!result){
		if (fw != NULL) fclose(fw);
		if (scanTempFilePath != NULL) unlink(scanTempFilePath);

		m_ObjBuffer->Clear1();
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteString("false"); //result
		m_ObjBuffer->WriteString("fail to get files info."); //message
		return;
	}

    // 2. read file contents
    char* fileContents = readFileContents(scanTempFilePath);
    // 3. write final result (success)
    m_ObjBuffer->Clear1();
    m_ObjBuffer->WriteLong((long)0);
    m_ObjBuffer->WriteLong((long)0);
    m_ObjBuffer->WriteString("true"); //result
    m_ObjBuffer->WriteString(""); //message
    m_ObjBuffer->WriteLongString(fileContents); //message

    free(fileContents);
    regfree(&regex);
    if (fw != NULL) fclose(fw);
    if (scanTempFilePath != NULL) unlink(scanTempFilePath);
}


void CFAPI::API41_DOSEARCH_ONLY_FILE_COLLECT(CBRMObj  *m_ObjBuffer2, int m_itemCnt, int pChildSoc, CCSManager *pManage)
{
	_WriteLog(_LOG_LOG, "[CMD_DOSEARCH_ONLY_FILE_COLLECT] \n");
	m_pChildSoc=pChildSoc;
	m_pManager = pManage;
	m_ObjBuffer = m_ObjBuffer2;
	unsigned char *fileData;
	struct stat fileinfo;

	char logMsgTemp[10000];
	char temp[100];
	char targetPath[1000];
	char targetRegExp[200];
	int startRow = 0;
	int currentCount = 0;
	long addedFileSize = 0;
	int filterInCount= 0 ;
	int filterIgCount= 0 ;

	_WriteLog(_LOG_LOG, "[CMD_DOSEARCH_ONLY_FILE_COLLECT ==> start to display params] \n");
	m_ObjBuffer->ReadString(temp);
	// 0. read parameter
	m_ObjBuffer->ReadString(targetPath);//
	sprintf(logMsgTemp, "param1: TARGET_PATH [%s]\n", targetPath);
	_WriteLog(_LOG_LOG, logMsgTemp);

	m_ObjBuffer->ReadString(temp);//
	sprintf(logMsgTemp, "param2: START_ROW [%s]\n", temp);
	_WriteLog(_LOG_LOG, logMsgTemp);

	startRow = atoi( temp );

	m_ObjBuffer->ReadString(temp);//
	sprintf(logMsgTemp, "param3: INCLUDE FILTER COUNT [%s]\n", temp);
	_WriteLog(_LOG_LOG, logMsgTemp);

	filterInCount = atoi( temp );

	// Create a char array to store the converted strings
	char **filter_include;

	if(filterInCount>0){
		filter_include = (char **)malloc(filterInCount * sizeof(char *));

		for(int i=0; i<filterInCount; i++){
			m_ObjBuffer->ReadString(temp);//
			sprintf(logMsgTemp, "param3-1: INCLUDE FILTER [%s]\n", temp);
			_WriteLog(_LOG_LOG, logMsgTemp);

			filter_include[i] = strdup(temp);
		}
	}
	// 0. read parameter ===> read ignore filter

	m_ObjBuffer->ReadString(temp);//
	sprintf(logMsgTemp, "param3: IGNORE FILTER COUNT [%s]\n", temp);
	_WriteLog(_LOG_LOG, logMsgTemp);

	filterIgCount = atoi( temp );

	char **filter_ignore;
	if(filterIgCount>0){
		// Create a char array to store the converted strings
		filter_ignore = (char **)malloc(filterIgCount * sizeof(char *));

		for(int i=0; i<filterIgCount; i++){
			m_ObjBuffer->ReadString(temp);//
			sprintf(logMsgTemp, "param4-1: IGNORE FILTER [%s]\n", temp);
			_WriteLog(_LOG_LOG, logMsgTemp);

			filter_ignore[i] = strdup(temp);
		}

	}

	_WriteLog(_LOG_LOG, "[CMD_DOSEARCH_ONLY_FILE_COLLECT ==> end to display params] \n");


	// 1. get directory info
	const char* tempDirPath = getenv("TMPDIR");
	if (tempDirPath == NULL) {
		tempDirPath = "/tmp"; // Default temporary directory path in Linux
	}
	char scanTempFilePath[100]; // Adjust the array size based on your needs
	sprintf(scanTempFilePath, "%s/SearchOnlyFileCollect_XXXXXX", tempDirPath);
	int scanTempFileDescriptor = mkstemp(scanTempFilePath);
	sprintf(logMsgTemp, "tempFilePath is %s %d\n", scanTempFilePath, scanTempFileDescriptor);
	_WriteLog(_LOG_LOG, logMsgTemp);
	if (scanTempFileDescriptor == -1) {
		m_ObjBuffer->Clear1();
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteString("false"); //result
		m_ObjBuffer->WriteString("Error creating the tempFile"); //message
		return;
	}
	sprintf(logMsgTemp, "Temporary file created: %s\n", scanTempFilePath);
	_WriteLog(_LOG_LOG, logMsgTemp);
	FILE *fw = fdopen(scanTempFileDescriptor, "w");
	if (fw == NULL) {
		m_ObjBuffer->Clear1();
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteString("false"); //result
		m_ObjBuffer->WriteString("Error opening the tempFile"); //message

		if (scanTempFilePath != NULL) unlink(scanTempFilePath);
		return;
	}

	int num_include = filterInCount;
	int num_ignore = filterIgCount;

	int appendedCount = 0;
	fprintf(fw, "[");
	int result = get_files_info(targetPath, targetPath, fw, startRow, &currentCount, &addedFileSize, filter_include, filter_ignore, num_include,  num_ignore);
	fprintf(fw, "]");
	fflush(fw);

	// release resource
	// Free the memory allocated for the char array and the JSON object
	for (size_t i = 0; i < filterInCount; i++) {
		free(filter_include[i]);
	}
	for (size_t i = 0; i < filterIgCount; i++) {
		free(filter_ignore[i]);
	}
	if(filterInCount>0) free(filter_include);
	if(filterIgCount>0) free(filter_ignore);

    if(!result){
        if (fw != NULL) fclose(fw);
        if (scanTempFilePath != NULL) unlink(scanTempFilePath);

        m_ObjBuffer->Clear1();
        m_ObjBuffer->WriteLong((long)0);
        m_ObjBuffer->WriteLong((long)0);
        m_ObjBuffer->WriteString("false"); //result
        m_ObjBuffer->WriteString("fail to get files info."); //message
        return;
    }


    // 2. read file contents
    char* fileContents = readFileContents(scanTempFilePath);

    if(fileContents==NULL){
    	if (fw != NULL) fclose(fw);
    	if (scanTempFilePath != NULL) unlink(scanTempFilePath);

    	 m_ObjBuffer->Clear1();
    	 m_ObjBuffer->WriteLong((long)0);
    	 m_ObjBuffer->WriteLong((long)0);
    	 m_ObjBuffer->WriteString("false"); //result
    	 m_ObjBuffer->WriteString("can't open temporary file"); //message
    }

    // 3. write final result (success)
    m_ObjBuffer->Clear1();
    m_ObjBuffer->WriteLong((long)0);
    m_ObjBuffer->WriteLong((long)0);
    m_ObjBuffer->WriteString("true"); //result
    m_ObjBuffer->WriteString(""); //message
    m_ObjBuffer->WriteLongString(fileContents); //message

    free(fileContents);
    if (fw != NULL) fclose(fw);
    if (scanTempFilePath != NULL) unlink(scanTempFilePath);
}



void CFAPI::API40_SCANDIR_TO_FILE(CBRMObj  *m_ObjBuffer2, int m_itemCnt, int pChildSoc, CCSManager *pManage)
{
	_WriteLog(_LOG_LOG, "[CMD_SCANDIR_TO_FILE] \n");
	m_pChildSoc=pChildSoc;
	m_pManager = pManage;
	m_ObjBuffer = m_ObjBuffer2;
	unsigned char *fileData;
	struct stat fileinfo;

	char logMsgTemp[10000];
	char temp[1000];
	char targetPath[1000];
	int filterInCount= 0 ;
	int filterIgCount= 0 ;

	_WriteLog(_LOG_LOG, "[CMD_SCANDIR_TO_FILE ==> start to display params] \n");
	m_ObjBuffer->ReadString(temp);
	// 0. read parameter
	m_ObjBuffer->ReadString(targetPath);//
	sprintf(logMsgTemp, "param1: TARGET_PATH [%s]\n", targetPath);
	_WriteLog(_LOG_LOG, logMsgTemp);

	m_ObjBuffer->ReadString(temp);//
	sprintf(logMsgTemp, "param2: INCLUDE FILTER COUNT [%s]\n", temp);
	_WriteLog(_LOG_LOG, logMsgTemp);

	filterInCount = atoi( temp );
	char **filter_include;
	if(filterInCount>0)
	{
		// Create a char array to store the converted strings
		filter_include = (char **)malloc(filterInCount * sizeof(char *));

		for(int i=0; i<filterInCount; i++){
			m_ObjBuffer->ReadString(temp);//
			sprintf(logMsgTemp, "param2-1: INCLUDE FILTER [%s]\n", temp);
			_WriteLog(_LOG_LOG, logMsgTemp);

			filter_include[i] = strdup(temp);
		}

		// Print the converted strings
		for (int i = 0; i < filterInCount; i++) {
			printf("%s\n", filter_include[i]);
		}
	}
	// 0. read parameter ===> read ignore filter

	m_ObjBuffer->ReadString(temp);//
	sprintf(logMsgTemp, "param3: IGNORE FILTER COUNT [%s]\n", temp);
	_WriteLog(_LOG_LOG, logMsgTemp);

	filterIgCount = atoi( temp );
	char **filter_ignore;
	if(filterIgCount > 0){
		// Create a char array to store the converted strings
		filter_ignore = (char **)malloc(filterIgCount * sizeof(char *));

		for(int i=0; i<filterIgCount; i++){
			m_ObjBuffer->ReadString(temp);//
			sprintf(logMsgTemp, "param3-1: IGNORE FILTER [%s]\n", temp);
			_WriteLog(_LOG_LOG, logMsgTemp);

			filter_ignore[i] = strdup(temp);
		}

		// Print the converted strings
		for (int i = 0; i < filterIgCount; i++) {
			printf("%s\n", filter_ignore[i]);
		}
	}
	_WriteLog(_LOG_LOG, "[CMD_DOSEARCH_ONLY_FILE_COLLECT ==> end to display params] \n");

	// 0. Create the scanTempFile
	const char* tempDirPath = getenv("TMPDIR");

	if (tempDirPath == NULL) {
		tempDirPath = "/tmp"; // Default temporary directory path in Linux
	}

	char scanTempFilePath[100]; // Adjust the array size based on your needs
	sprintf(scanTempFilePath, "%s/CFAgentScanResult_XXXXXX", tempDirPath);
	int scanTempFileDescriptor = mkstemp(scanTempFilePath);
	sprintf(logMsgTemp, "scanTempFilePath is %s %d\n", scanTempFilePath, scanTempFileDescriptor);
	_WriteLog(_LOG_LOG, logMsgTemp);

	if (scanTempFileDescriptor == -1) {
		m_ObjBuffer->Clear1();
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteString("false"); //result
		m_ObjBuffer->WriteString("Error creating the scanTempFile"); //message
		return;
	}
	sprintf(logMsgTemp, "Temporary file created: %s\n", scanTempFilePath);
	_WriteLog(_LOG_LOG, logMsgTemp);

	FILE *fw = fdopen(scanTempFileDescriptor, "w");
	if (fw == NULL) {
		m_ObjBuffer->Clear1();
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteString("false"); //result
		m_ObjBuffer->WriteString("Error opening the scanTempFile"); //message

		if (scanTempFilePath != NULL) unlink(scanTempFilePath);
		return;
	}
	// 1. get directory info and write right away
	int num_include = filterInCount;
	int num_ignore = filterIgCount;

	scan_directory_info(targetPath, targetPath, fw, filter_include, filter_ignore, num_include,  num_ignore);
	fflush(fw);
//	printf("Write file scanned resource\n");


	// release resource
	// Free the memory allocated for the char array and the JSON object
	for (size_t i = 0; i < filterInCount; i++) {
		free(filter_include[i]);
	}
	for (size_t i = 0; i < filterIgCount; i++) {
		free(filter_ignore[i]);
	}
	if(filterInCount>0) free(filter_include);
	if(filterIgCount>0) free(filter_ignore);




	// 2. Create the zipTempFile
	char zipTempFilePath[100]; // Adjust the array size based on your needs
	sprintf(zipTempFilePath, "%s/CFAgentScanResult_XXXXXX", tempDirPath);
	int zipTempFileDescriptor = mkstemp(zipTempFilePath);
	sprintf(logMsgTemp, "zipTempFilePath is %s %d\n", zipTempFilePath, zipTempFileDescriptor);
	_WriteLog(_LOG_LOG, logMsgTemp);

	if (zipTempFileDescriptor == -1) {
		m_ObjBuffer->Clear1();
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteString("false"); //result
		m_ObjBuffer->WriteString("Error creating the zipTempFile"); //message

		if (fw != NULL) fclose(fw);
		if (scanTempFilePath != NULL) unlink(scanTempFilePath);

		return;
	}
	gzFile zipOut = gzdopen(zipTempFileDescriptor, "wb");
	if (zipOut == NULL) {
		m_ObjBuffer->Clear1();
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteString("false"); //result
		m_ObjBuffer->WriteString("Error opening the zipTempFile"); //message

		if (fw != NULL) fclose(fw);
		if (scanTempFilePath != NULL) unlink(scanTempFilePath);
		return;
	}


	_WriteLog(_LOG_LOG, "gzFile zipOut is opened\n");

	// 3. Add the scanTempFile to the zipTempFile
	char buf[1024];
	FILE *in = fopen(scanTempFilePath, "r");
	if (in == NULL) {
		m_ObjBuffer->Clear1();
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteString("false"); //result
		m_ObjBuffer->WriteString("Error opening the scanTempFile for zipping"); //message


		if (fw != NULL) fclose(fw);
		if (zipOut != NULL) gzclose(zipOut);
		if (scanTempFilePath != NULL) unlink(scanTempFilePath);
		if (zipTempFilePath != NULL) unlink(zipTempFilePath);
		return;
	}

	sprintf(logMsgTemp, "open scanTempFilePath file = %s\n" , scanTempFilePath);
	_WriteLog(_LOG_LOG, logMsgTemp);

	int len;
	while ((len = fread(buf, 1, sizeof(buf), in)) > 0) {
		if (gzwrite(zipOut, buf, len) != len) {
			m_ObjBuffer->Clear1();
			m_ObjBuffer->WriteLong((long)0);
			m_ObjBuffer->WriteLong((long)0);
			m_ObjBuffer->WriteString("false"); //result
			m_ObjBuffer->WriteString("Error zipping the scanTempFile"); //message

			if (fw != NULL) fclose(fw);
			if (in != NULL) fclose(in);
			if (zipOut != NULL) gzclose(zipOut);
			if (scanTempFilePath != NULL) unlink(scanTempFilePath);
			if (zipTempFilePath != NULL) unlink(zipTempFilePath);

			return;
		}
	}

	sprintf(logMsgTemp, "write to zip = %s\n" , zipTempFilePath);
	_WriteLog(_LOG_LOG, logMsgTemp);

	if (fw != NULL) fclose(fw);
	if (in != NULL) fclose(in);
	if (zipOut != NULL) gzclose(zipOut);
	if (scanTempFilePath != NULL) unlink(scanTempFilePath);


	// 4. Write file
	if (stat(zipTempFilePath, &fileinfo) != 0) {
		m_ObjBuffer->Clear1();
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteLong((long)0);

		// final result
		m_ObjBuffer->WriteString("false"); //result
		m_ObjBuffer->WriteString("can't open compressed file."); //message

		if (zipTempFilePath != NULL) unlink(zipTempFilePath);
		return;
	}


	FILE* file = fopen(zipTempFilePath, "rb");
	if (file == NULL) {
		m_ObjBuffer->Clear1();
		m_ObjBuffer->WriteLong((long)0);
		m_ObjBuffer->WriteLong((long)0);

		// final result
		m_ObjBuffer->WriteString("false"); //result
		m_ObjBuffer->WriteString("Failed to open zip file."); //message

		if (zipTempFilePath != NULL) unlink(zipTempFilePath);

		return;
	}


	//final result
	m_ObjBuffer->Clear1();
	m_ObjBuffer->WriteLong((long)0);
	m_ObjBuffer->WriteLong((long)0);
	m_ObjBuffer->WriteString("true"); //result
	m_ObjBuffer->WriteString(""); //message

	m_ObjBuffer->WriteLong( fileinfo.st_size);
//	printf("fileinfo.st_size =[%d] ", fileinfo.st_size);

	long outLeng = m_ObjBuffer->getLength();
	m_ObjBuffer->setMaxLength(outLeng + fileinfo.st_size);
//	printf("tot fileinfo.st_size =[%d] \n",outLeng+ fileinfo.st_size);

	((CCSManager*)m_pManager)->SocWrite(m_pChildSoc, (unsigned char *)m_ObjBuffer->m_Buffer, outLeng);
	unsigned char buffer[10240];
	size_t bytes_read;
	int firstWrite = 0;
	int st = 0;
	while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
		((CCSManager*)m_pManager)->SocWrite(m_pChildSoc, (unsigned char *)buffer, bytes_read);
//		printf(" st [%d] \n", st);
		firstWrite += bytes_read;
	}
	fclose(file);

//	printf("close file\n" );


	if (zipTempFilePath != NULL) unlink(zipTempFilePath);
//	printf("release zipTempFilePath resource\n" );

//	printf("release all resource\n" );

}



