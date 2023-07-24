#define AES_BLOCK_SIZE  32
#define DIR_SEPARATOR '/'

#ifdef _WIN32
#include <direct.h>
#include <winsock2.h>
#else
#include <unistd.h> // For Linux/Unix
#endif

#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include 	<jansson.h>
#include 	<dirent.h>
#include    <ctype.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include    "MTUtil.h"
#include <sys/types.h>
#include <sys/stat.h>
#ifdef WIN32
#include <sys/timeb.h>
#include <windows.h>
#endif

#include    <time.h>
#ifdef		_AIX
#include	<strings.h>
#endif		//_AIX
#ifdef		__hpux
#include	<strings.h>
#endif		//__hpux

#ifdef		__GNUC__
#include	<strings.h>
#include 	<sys/time.h>
#include	<time.h>
#endif		//__hpux

#ifdef		_OS390
#include    <sys/time.h>
#endif





const char* getFileNameFromPath(const char* path) {
    const char* last_sep = strrchr(path, '/');
#ifdef _WIN32
    const char* last_sep_windows = strrchr(path, '\\');
    if (last_sep_windows > last_sep) {
        last_sep = last_sep_windows;
    }
#endif

    return last_sep ? last_sep + 1 : path;
}

int printFileAttributes(const char* path, char* msg, size_t msg_size) {
/*#ifdef WIN32 (윈도우 필요 없음
	struct _stat fileInfo;
	if (_stat(path, &fileInfo) != 0) {
		printf("Failed to get file attributes\n");
		return false;
	}

	if (fileInfo.st_mode & _S_IFDIR) {
		printf("Type: Directory\n");
	}
	else {
		printf("Type: File\n");
	}

	printf("Attributes:\n");
	DWORD fileAttributes = GetFileAttributes(path);
	if (fileAttributes != INVALID_FILE_ATTRIBUTES) {
		if (fileAttributes & FILE_ATTRIBUTE_ARCHIVE) {
			printf("- Archive\n");
		}
		if (fileAttributes & FILE_ATTRIBUTE_HIDDEN) {
			printf("- Hidden\n");
		}
		if (fileAttributes & FILE_ATTRIBUTE_READONLY) {
			printf("- Read-only\n");
		}
		if (fileAttributes & FILE_ATTRIBUTE_SYSTEM) {
			printf("- System\n");
		}
		// 추가적인 속성들을 필요에 따라 확인하고 출력할 수 있습니다.
}
#else*/
	struct stat fileInfo;
	int read=0;
	int write=0;
	int execute=0;
	int dir=0;

	if (stat(path, &fileInfo) != 0) {
		snprintf(msg, msg_size, "Failed to get file attributes");
		return false;
	}

	if (S_ISDIR(fileInfo.st_mode)) {
		dir = 1;
	}

    read = fileInfo.st_mode & S_IRUSR ? 1 : 0;
    write = fileInfo.st_mode & S_IWUSR ? 1 : 0;
    execute = fileInfo.st_mode & S_IXUSR ? 1 : 0;

	const char* filename = getFileNameFromPath(path);
	printf("fileName:  %s\n", filename );
	printf("path:  %s\n", path );
	printf("size:  %lld\n", (long long)fileInfo.st_size );
	printf("isDir:  %i\n", dir );
	printf("modify_time:  %ld\n", (long)fileInfo.st_mtime);
	printf("read:  %i\n", read );
	printf("write:  %i\n", write );
	printf("execute:  %i\n", execute );
	printf("before msg:  %s\n", msg );

	int result = snprintf(msg, msg_size,
	                          "{\"filename\":\"%s\",\"path\":\"%s\",\"size\":%lld,\"is_dir\":%i,\"modify_time\":%ld,\"read\":%i,\"write\":%i,\"execute\":%i}",
	                          filename, path, (long long)fileInfo.st_size, dir, (long)fileInfo.st_mtime, read, write, execute);

	if (result >= (int)msg_size) {
	    snprintf(msg, msg_size, "JSON string too large for buffer");
	    return false;
	}

	printf("after msg:  %s\n", msg );

//#endif




	return true;
	// 추가적인 속성들을 필요에 따라 확인하고 출력할 수 있습니다.
}

int directoryExists(const char* path) {
    struct stat info;
    return stat(path, &info) == 0 && S_ISDIR(info.st_mode);
}

int createSingleDirectory(const char* path) {
#ifdef _WIN32
    return _mkdir(path);
#else
    return mkdir(path, 0755); // 0755 is the octal value for read/write/execute for owner and read/execute for group and others
#endif
}

int createDirectoriesRecursively(const char* path) {
    // Check if the directory exists
    struct stat st;
    if (stat(path, &st) == 0) {
        if (S_ISDIR(st.st_mode)) {
            // The directory already exists
            return 0;
        } else {
            // A file with the same name exists, cannot create directory
            return 0;
        }
    }

    // If the path is root or empty, return
    if (path[0] == '\0' || (path[0] == DIR_SEPARATOR && path[1] == '\0')) {
        return 0;
    }

    // Find the last occurrence of the directory separator (DIR_SEPARATOR)
    const char* last_sep = path;
    for (const char* p = path; *p; p++) {
        if (*p == DIR_SEPARATOR) {
            last_sep = p;
        }
    }

    // Extract the parent directory path
    size_t parent_len = last_sep - path;
    char* parent_path = (char*)malloc(parent_len + 1);
    if (parent_path == NULL) {
        return -1; // Memory allocation failure
    }
    strncpy(parent_path, path, parent_len);
    parent_path[parent_len] = '\0';

    // Create the parent directory recursively
    int parent_result = createDirectoriesRecursively(parent_path);
    free(parent_path); // Free the dynamically allocated memory

    if (parent_result != 0) {
        return parent_result; // Propagate the error up
    }

    // Create the current directory
    return createSingleDirectory(path);
}

int createFile(char* filename, const unsigned char* data, size_t length, char *msg) {
	// 1. create directory
	char dir_name[300];

	int start_index = 0;
	char* last_slash = strrchr(filename, '/');

	if (last_slash != NULL) {
        size_t length = last_slash - filename + 1; // Calculate the length of the substring, including the slash
        if (length < sizeof(dir_name)) {
            // Copy the substring from position 0 to the last slash (including the slash)
            memcpy(dir_name, filename, length);
            dir_name[length] = '\0'; // Null-terminate the substring

            printf("Substring from 0 to last slash: %s\n", dir_name);
        } else {
			sprintf(msg,"Substring buffer too small");
            return false;
        }
    } else {
    	sprintf(msg,"No slash found in the path.");
	    return false;
    }

	printf("Directory name = %s\n", dir_name);

	if (directoryExists(dir_name)) {
		printf("Directory already exists.\n");
	}else{
		int mkdir_result = createDirectoriesRecursively(dir_name);
		if (!directoryExists(dir_name)) {
			sprintf(msg,"Error creating directory");
			return false;
		}
	}

	// 2. create file
	printf("I will create file. [%s]  \n",filename);
	FILE* file = fopen(filename, "wb");
	if (file == NULL) {
		sprintf(msg,"Failed to open file");
		return false;
	}

	size_t bytesWritten = fwrite(data, 1, length, file);
	if (bytesWritten != length) {
		sprintf(msg, "Failed to write file");
	}
	else {
		sprintf(msg, "File created: %s\n", filename);
	}

	fclose(file);
	return true;
}

json_t* get_directory_info(const char* dir_path) {
    json_t *dir_info = json_object();

    DIR *dir = opendir(dir_path);
    if (!dir) {
        fprintf(stderr, "Error opening directory: %s\n", dir_path);
        return dir_info;
    }

    struct dirent *entry;
    while ((entry = readdir(dir))) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char path[PATH_MAX];
        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

        if (entry->d_type == DT_DIR) {
            json_t *subdir_info = get_directory_info(path);
            json_object_set_new(dir_info, entry->d_name, subdir_info);
        } else if (entry->d_type == DT_REG) {
            json_object_set_new(dir_info, entry->d_name, json_string("file"));
        }
    }

    closedir(dir);
    return dir_info;
}
/*int calculate_md5(const char* file_path, const char* md5sumsrc, char* error_msg) {
	FILE* file = fopen(file_path, "rb");
	unsigned char digest[MD5_DIGEST_LENGTH];
	unsigned char buffer[CHECK_SUM_BUFFER_SIZE];
	if (!file) {
		strcpy(error_msg, "파일을 열 수 없습니다.");
		return 0;
	}
	MD5_CTX md5_ctx;
	MD5_Init(&md5_ctx);
	size_t bytes;
	while ((bytes = fread(buffer, 1, CHECK_SUM_BUFFER_SIZE, file)) != 0) {
		MD5_Update(&md5_ctx, buffer, bytes);
	}
	MD5_Final(digest, &md5_ctx);

	char md5String[MD5_DIGEST_LENGTH * 2 + 1];
	memset(md5String, 0xff, MD5_DIGEST_LENGTH * 2 + 1);
	for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
		sprintf(&md5String[i * 2], "%02X", (unsigned int)digest[i]);
	}
	fclose(file);

	if (strcmp((const char*)md5String, md5sumsrc) == 0) {
		return true;
	}
	else {
		sprintf(error_msg, "체크섬 오류.");
		return false;
	}	
}*/

int calculate_sha256(const char* file_path, const char* sha256sum, char* error_msg) {
	unsigned char buffer[1024];
	size_t bytesRead;
	unsigned char shaData[1024];
	unsigned char digest[SHA256_DIGEST_LENGTH];
	char sha256String[SHA256_DIGEST_LENGTH * 2 + 1];

	printf("let's do checksum of %s", file_path);
	FILE* file = fopen(file_path, "rb");
	if (file == NULL) {
		strcpy(error_msg, "can't open file.");
		return false;
	}
	SHA256_CTX sha256Context;
	SHA256_Init(&sha256Context);
	while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
		SHA256_Update(&sha256Context, buffer, bytesRead);
	}
	SHA256_Final(digest, &sha256Context);

	memset(sha256String, 0xff, SHA256_DIGEST_LENGTH * 2 + 1);
	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
		sprintf(&sha256String[i * 2], "%02X", (unsigned int)digest[i]);
	}
	fclose(file);
	printf("file's checksum %s", sha256String);
	if (strcmp((const char*)sha256String, sha256sum) == 0) {
		return true;
	}
	else {
		sprintf(error_msg, "not matched file checksum");
		return false;
	}
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

unsigned int calculate_crc(const unsigned char* data, int length) {
	unsigned int crc = 0xFFFFFFFF;
	int i;
	for (i = 0; i < length; i++) {
		crc = (crc >> 8) ^ crc32_table[(crc & 0xFF) ^ data[i]];
	}
	return crc ^ 0xFFFFFFFF;
}

unsigned short calculate_crc32(const char* file_path, const char* crcsum, char* error_msg) {
	generate_crc32_table();
	char buffer[1024];
//	const char* file_path = "C:/cfagent/temp/AA.zip";  // 대상 파일 경로 설정

	FILE* file = fopen(file_path, "rb");
	if (file == NULL) {
		sprintf(error_msg,"파일을 열 수 없습니다.");
		return 1;
	}

	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	unsigned char* file_data = (unsigned char*)malloc(file_size);
	if (file_data == NULL) {
		sprintf(error_msg,"메모리 할당 오류.");
		fclose(file);
		return 1;
	}

	fread(file_data, 1, file_size, file);
	fclose(file);

	unsigned int crc32 = calculate_crc(file_data, file_size);
	free(file_data);
	sprintf(buffer, "%08X", crc32);
	if (strcmp((const char*)buffer, crcsum) == 0) {
		return true;
	}
	else {
		sprintf(error_msg, "체크섬 오류.");
		return false;
	}

}

char * removeSlash(char* str) {
	for (int i = strlen(str) - 1; i >= 0; i--)
	{
		if (str[i] == '\\' || str[i] == 0xcc)
			str[i] = 0x00;
		else if (str[i] == '/' || str[i] == 0xcc)
			str[i] = 0x00;
		else
			break;
	}

	char *p = str;

	while (*p) {
		if (*p != '\\') break;
		if (*p != '/') break;
		p++;
	}

	if (*p == 0) strcpy(p, " ");
	return p;
}

char * rltrim(char *str)
{
	for (int i = strlen(str) - 1; i >= 0; i--)
	{
		if (str[i] == ' ' || str[i] == 0xcc)
			str[i] = 0x00;
		else
			break;
	}

	char *p = str;

	while (*p) {
		if (*p != ' ') break;
		p++;
	}

	if (*p == 0) strcpy(p, " ");
	return p;
}

double getDoubleToString(double dVal) {
	double dVal2;
	char sTemp[31];
	sprintf(sTemp, "%lf", dVal);
	sTemp[30] = 0;
	dVal2 = atof(sTemp);
	return dVal2;
}
char * rtrim(char *str) {
	int i = 0;

	for (i = strlen(str) - 1; i >= 0; i--) {
		if (str[i] == ' ' || str[i] == 0xcc)
			str[i] = 0x00;
		else
			break;
	}
	return str;
}

char* McRTrim(char *lpszSource) {
	int i;
	for (i = strlen(lpszSource) - 1; i > -1; i--) {
		//if( (lpszSource[i] == ' ') || (lpszSource[i] < 0x20) )
		if (lpszSource[i] == ' ')
			lpszSource[i] = 0x00;
		else
			break;
	}
	return lpszSource;
}
char* TrimToken(char *lpszSource, char tk) {
	int i;
	for (i = strlen(lpszSource) - 1; i > -1; i--) {
		if (lpszSource[i] == tk)
			lpszSource[i] = ' ';
	}
	McRTrim(lpszSource);
	return lpszSource;
}

int index(char *lpszSource, char tk, int index) {
	int i;
	if (lpszSource == 0x00)return -1;
	int length = strlen(lpszSource);
	if (index >= length)return -1;
	for (i = index; i < length; i++) {
		if (lpszSource[i] == tk) return i;
	}
	return -1;
}
void  getSubstr(char *lpszSource, char *queryTemp, int idxs, int idx2) {
	int  copyCnt = 0;
	if (lpszSource == 0x00)return;
	int length = strlen(lpszSource);


	for (int i = 0; i < length; i++) {
		if (i >= idxs && i <= idx2) {
			queryTemp[copyCnt] = lpszSource[i];
			copyCnt++;
		}
	}
	queryTemp[copyCnt] = 0;

	printf(queryTemp);

}
char* McRTrim2(char *lpszSource) {
	int i;
	for (i = strlen(lpszSource) - 1; i > -1; i--) {
		//if( (lpszSource[i] == ' ') || (lpszSource[i] < 0x20) )
		if (lpszSource[i] == ' ' || lpszSource[i] == '\n')
			lpszSource[i] = 0x00;
		else
			break;
	}
	return lpszSource;
}

unsigned char StringChange(char *pData, char *pSrc, char *pDest) {
	char *pPos;
	int leng;
	char tempString[3001];

	pPos = strstr(pData, pSrc);

	if (!pPos)
		return 0;

	leng = strlen(pSrc);

	strcpy(tempString, pPos + leng);

	strcpy(pPos, pDest);

	strcat(pPos, tempString);

	return 1;
}

char* DetailTimeStamp(char *lpszDateTime) {
	time_t localTime;
	struct tm *stTempTime;
	struct tm pTm2;
	int usec;

#ifdef WIN32
	struct _timeb timebuffer;
	_ftime(&timebuffer);
	usec = (long)timebuffer.millitm;
#else
	struct timeval stamp;
	stamp.tv_usec = 0;
	gettimeofday(&stamp, NULL);

	usec = stamp.tv_usec / 1000;
#endif

	localTime = time(&localTime); /* Get time as long integer. */

#ifdef WIN32
	stTempTime = localtime(&localTime);
#else
	stTempTime = localtime_r(&localTime, &pTm2);
#endif

	sprintf(lpszDateTime
		, "%4d%02d%02d%02d%02d%02d%03d"
		, stTempTime->tm_year + 1900
		, stTempTime->tm_mon + 1
		, stTempTime->tm_mday
		, stTempTime->tm_hour
		, stTempTime->tm_min
		, stTempTime->tm_sec, usec);

	return lpszDateTime;
}

void GetCurrentTimeStr(char *TimeBuffer) {
	time_t localTime;
	struct tm *stTempTime;
	struct tm pTm2;
	char CurrentTime[20];

	localTime = time(&localTime);
#if defined(_AIX) || defined(__hpux) || defined(__GNUC__)
	struct timeval tv;
	gettimeofday(&tv, 0);
	stTempTime = localtime_r(&tv.tv_sec, &pTm2);
#elif defined(_OS390) || defined(_OS400)  
	time(&localTime);
#elif  defined(WIN32)
	time(&localTime);
	stTempTime = localtime(&localTime);

#endif

	//	memset(CurrentTime, NULL, sizeof(CurrentTime));

	sprintf((char *)CurrentTime, "%4d%02d%02d %02d%02d%02d",
		stTempTime->tm_year + 1900, stTempTime->tm_mon + 1, stTempTime->tm_mday, stTempTime->tm_hour,
		stTempTime->tm_min, stTempTime->tm_sec);

	memcpy(TimeBuffer, (char *)CurrentTime, sizeof(CurrentTime));
	return;
}

void GetCurrentDateStr(char *DateBuffer) {
	time_t localTime;
	struct tm *stTempTime;
	struct tm pTm2;
	char CurrentDate[8 + 1];

	localTime = time(&localTime);
#if defined(_AIX) || defined(__hpux) || defined(__GNUC__)
	struct timeval tv;
	stTempTime = localtime_r(&tv.tv_sec, &pTm2);
#elif defined(_OS390) || defined(_OS400)  
	stTempTime = localtime_r(&localTime, &pTm2);
#elif  defined(WIN32)
	stTempTime = localtime(&localTime);

#endif


	sprintf((char *)CurrentDate, "%4d%02d%02d",
		stTempTime->tm_year + 1900, stTempTime->tm_mon + 1, stTempTime->tm_mday, stTempTime->tm_hour,
		stTempTime->tm_min, stTempTime->tm_sec);

	memcpy(DateBuffer, (char *)CurrentDate, sizeof(CurrentDate));
	return;
}

char* CreateVersionString(char *lpszDateTime) {
	time_t localTime;
	struct tm *stTempTime;
	struct tm pTm2;
	localTime = time(&localTime);

#ifdef WIN32
	stTempTime = localtime(&localTime);
#else
	stTempTime = localtime_r(&localTime, &pTm2);
#endif
	sprintf(lpszDateTime
		, "%4d%02d%02d%02d%02d%02d"
		, stTempTime->tm_year + 1900
		, stTempTime->tm_mon + 1
		, stTempTime->tm_mday
		, stTempTime->tm_hour
		, stTempTime->tm_min
		, stTempTime->tm_sec);

	return lpszDateTime;
}

char* GetSysEnv(char* ObjectName) {

	if (!ObjectName)
		return NULL;

	return (getenv(ObjectName));
}

long McRandom(long Range) {
	long r = 0;

#ifdef _WIN32
	time_t localTime;
	//	LARGE_INTEGER start,end,frequency;
	//	LONGLONG	m_llTimer;		

	time(&localTime);

	r = (long)localTime % Range;

#else
	struct timeval stamp;

	stamp.tv_usec = 0;
	gettimeofday(&stamp, NULL);

	r = stamp.tv_usec % Range;

#endif

	return r;

}


/*       Check                                 */
/*  sbuf  : Check ?       string buffer          */
/*  n     :                                       */
/* RETURN VALUE :    ?  1,  ??  0              */
/*------------------------------------------------*/
long mIsAlpha(char *buf, long n) {
	long i;

	for (i = 0; i < n; i++)
		if (!isalpha(buf[i]))
			return (1);
	return (0);
}
/*------------------------------------------------*/
/*  Numeric Check                                 */
/*  sbuf  : Check ?       string buffer          */
/*  n     :                                       */
/* RETURN VALUE :    ?  1,  ??  0              */
/*------------------------------------------------*/
long mIsNumeric(char *buf, long n) {
	long i;
	for (i = 0; i < n; i++) {
		if (!isdigit(buf[i]))
			return (0);
	}
	return (1);
}
unsigned char *Base64Encode(const unsigned char *str, int length, int *ret_length) {
	const unsigned char *current = str;
	int i = 0;
	unsigned char *result = (unsigned char *)malloc(((length + 3 - length % 3) * 4 / 3 + 1) * sizeof(char));
	while (length > 2) {
		result[i++] = Base64Table[current[0] >> 2];
		result[i++] = Base64Table[((current[0] & 0x03) << 4) + (current[1] >> 4)];
		result[i++] = Base64Table[((current[1] & 0x0f) << 2) + (current[2] >> 6)];
		result[i++] = Base64Table[current[2] & 0x3f];

		current += 3;
		length -= 3;
	}

	if (length != 0) {
		result[i++] = Base64Table[current[0] >> 2];
		if (length > 1) {
			result[i++] = Base64Table[((current[0] & 0x03) << 4) + (current[1] >> 4)];
			result[i++] = Base64Table[(current[1] & 0x0f) << 2];
			result[i++] = Base64Pad;
		}
		else {
			result[i++] = Base64Table[(current[0] & 0x03) << 4];
			result[i++] = Base64Pad;
			result[i++] = Base64Pad;
		}
	}
	if (ret_length) {
		*ret_length = i;
	}
	result[i] = '\0';
	return result;
}

unsigned char *Base64Decode(const unsigned char *str, int length, int *ret_length) {
	const unsigned char *current = str;
	int ch, i = 0, j = 0, k;
	static short reverse_table[BASESIZE];
	static int table_built;
	unsigned char *result;

	if (++table_built == 1) {
		char *chp;
		for (ch = 0; ch < BASESIZE; ch++) {
			chp = strchr(Base64Table, ch);
			if (chp) {
				reverse_table[ch] = chp - Base64Table;
			}
			else {
				reverse_table[ch] = -1;
			}
		}
	}

	result = (unsigned char *)malloc(length + 1);
	if (result == NULL) {
		return NULL;
	}

	while ((ch = *current++) != '\0') {
		if (ch == Base64Pad) break;
		if (ch == ' ') ch = '+';

		ch = reverse_table[ch];
		if (ch < 0) continue;

		switch (i % 4) {
		case 0:
			result[j] = ch << 2;
			break;
		case 1:
			result[j++] |= ch >> 4;
			result[j] = (ch & 0x0f) << 4;
			break;
		case 2:
			result[j++] |= ch >> 2;
			result[j] = (ch & 0x03) << 6;
			break;
		case 3:
			result[j++] |= ch;
			break;
		}
		i++;
	}

	k = j;

	if (ch == Base64Pad) {
		switch (i % 4) {
		case 0:
		case 1:
			free(result);
			return NULL;
		case 2:
			k++;
		case 3:
			result[k++] = 0;
		}
	}
	if (ret_length) {
		*ret_length = j;
	}
	result[k] = '\0';
	return result;
}
#ifndef _BRMLIB


#ifndef _TESTER
//modified by DSKIM 2016.11.23: AES256   
int aes_init256(unsigned char* key, unsigned char* iv, EVP_CIPHER_CTX * e_ctx, EVP_CIPHER_CTX * d_ctx)
{
	EVP_CIPHER_CTX_init(e_ctx);
	EVP_EncryptInit_ex(e_ctx, EVP_aes_256_cbc(), NULL, key, iv);
	EVP_CIPHER_CTX_init(d_ctx);
	EVP_DecryptInit_ex(d_ctx, EVP_aes_256_cbc(), NULL, key, iv);

	return 0;
}
int aes_init128(unsigned char* key, unsigned char* iv, EVP_CIPHER_CTX * e_ctx, EVP_CIPHER_CTX * d_ctx)
{
	EVP_CIPHER_CTX_init(e_ctx);
	EVP_EncryptInit_ex(e_ctx, EVP_aes_128_cbc(), NULL, key, iv);
	EVP_CIPHER_CTX_init(d_ctx);
	EVP_DecryptInit_ex(d_ctx, EVP_aes_128_cbc(), NULL, key, iv);

	return 0;
}

unsigned char* aes_encrypt(EVP_CIPHER_CTX* e, unsigned char* plaintext, int* len)
{
	int c_len = *len + AES_BLOCK_SIZE, f_len = 0;
	unsigned char *ciphertext = (unsigned char *)malloc(c_len);

	EVP_EncryptInit_ex(e, NULL, NULL, NULL, NULL);
	EVP_EncryptUpdate(e, ciphertext, &c_len, plaintext, *len);
	EVP_EncryptFinal_ex(e, ciphertext + c_len, &f_len);

	*len = c_len + f_len;
	return ciphertext;
}

unsigned char* aes_decrypt(EVP_CIPHER_CTX* e, unsigned char* ciphertext, int* len)
{
	int p_len = *len, f_len = 0;
	unsigned char *plaintext = (unsigned char *)malloc(p_len);

	EVP_DecryptInit_ex(e, NULL, NULL, NULL, NULL);
	EVP_DecryptUpdate(e, plaintext, &p_len, ciphertext, *len);
	EVP_DecryptFinal_ex(e, plaintext + p_len, &f_len);

	*len = p_len + f_len;
	return plaintext;
}
#endif
#endif
