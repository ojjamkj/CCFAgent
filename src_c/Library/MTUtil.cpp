#define AES_BLOCK_SIZE  32
#define DIR_SEPARATOR '/'
#define MAX_FILES_SIZE 50000000
#define PATH_MAX 1024


#include 	<unistd.h> // For Linux/Unix
#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include 	<dirent.h>
#include    <ctype.h>
#include    "MTUtil.h"
#include 	<sys/types.h>
#include 	<sys/stat.h>
#include    <time.h>
#include 	<stdbool.h>
#include 	<regex.h>
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
    return last_sep ? last_sep + 1 : path;
}

void replaceDoubleSlash(char *str) {
    char *src = str;
    char *dst = str;
    bool prevSlash = false;

    while (*src != '\0') {
        if (*src == '/' && prevSlash) {
            // Skip the second slash in consecutive slashes
            src++;
        } else {
            *dst = *src;
            prevSlash = (*src == '/');
            src++;
            dst++;
        }
    }

    *dst = '\0'; // Add null terminator at the end
}

char* get_file_info(const char* path, char* msg) {
	struct stat fileInfo;
	int read=0;
	int write=0;
	int execute=0;
	int dir=0;
	char* jsonStr = NULL;

	if (stat(path, &fileInfo) != 0) {
		sprintf(msg,"Failed to get file attributes");
		return NULL;
	}

	if (S_ISDIR(fileInfo.st_mode)) {
		dir = 1;
	}

    read = fileInfo.st_mode & S_IRUSR ? 1 : 0;
    write = fileInfo.st_mode & S_IWUSR ? 1 : 0;
    execute = fileInfo.st_mode & S_IXUSR ? 1 : 0;

	const char* filename = getFileNameFromPath(path);


	int result = asprintf(&jsonStr,
			"{\"filename\":\"%s\",\"path\":\"%s\",\"size\":%lld,\"isdir\":%i,\"mtime\":%ld,\"read\":%i,\"write\":%i,\"execute\":%i,\"errmsg\":\"%s\"}",
		    filename, path, fileInfo.st_size, dir, (long)fileInfo.st_mtime, read, write, execute, (!read ? "can't read": ""));

	if (result == -1) {
		sprintf(msg, "Failed to allocate memory for JSON string");
		return NULL;
	}

	return jsonStr;
}

int directoryExists(const char* path) {
    struct stat info;
    return stat(path, &info) == 0 && S_ISDIR(info.st_mode);
}

int createSingleDirectory(const char* path) {
    return mkdir(path, 0755); // 0755 is the octal value for read/write/execute for owner and read/execute for group and others
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

int get_directory_info(const char* dir_path, FILE *fw, int includeSub, int defaultGetRows, regex_t regex,
		int regExpValid, int includeMode, int* appendedCount )
{
	if(defaultGetRows>-1){
		if(*appendedCount >= defaultGetRows){
			printf("JSON Array Length: %zu\n", *appendedCount);
			return false;
		}
	}

	typedef struct {
		DIR* dir;
		char path[PATH_MAX];
	} DirInfo;

	DirInfo stack[100000]; // Stack to hold directory information
	int top = -1; // Top of the stack

    DIR *dir = opendir(dir_path);
    if (!dir) {
        fprintf(stderr, "Error opening directory: %s\n", dir_path);
        return false;
    }

    stack[++top].dir = dir;
    strcpy(stack[top].path, dir_path);

    int currentStack=0;
    char errorMsg[200];

    if(includeMode==1){ //only dir
    	char* json_str = get_file_info(dir_path, errorMsg);

    	if(json_str==NULL){
    		return false;
    	}
    	fprintf(fw, "%s,", json_str);
    	free(json_str);
    }

	while (top >= currentStack) {
		DIR* current_dir = stack[currentStack].dir;
    	char* current_path = stack[currentStack].path;

    	currentStack++;

		struct dirent *entry;
		while ((entry = readdir(current_dir))) {

			if (entry == NULL) {
//				closedir(current_dir);
				printf("close: %s", current_dir);
				continue;
			}

			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
				continue;
			}

			char path[PATH_MAX];

			snprintf(path, sizeof(path), "%s/%s", current_path, entry->d_name);
			replaceDoubleSlash(path);

			struct stat file_stat;
			if (stat(path, &file_stat) == 0) {
				int dir = 0;
				if (S_ISDIR(file_stat.st_mode)) dir=1;

				if(!dir && regExpValid){
					printf("Regular Expression evaluate...!\n");
					int ret = regexec(&regex, path, 0, NULL, 0);
					if (ret == 0) {
						printf("Matched!\n");
					} else if (ret == REG_NOMATCH) {
						printf("No match!\n");
						continue;
					} else {
						char error_buffer[100];
						regerror(ret, &regex, error_buffer, sizeof(error_buffer));
						printf("Regex match failed: %s\n", error_buffer);
						continue;
					}
				}

				if(( dir && (includeMode==0 || includeMode==1) ) || (!dir && (includeMode==0 || includeMode==2)) ){

					char* json_str = get_file_info(path, errorMsg);

					if(json_str==NULL){
						return false;
					}
					fprintf(fw, "%s,", json_str);
					free(json_str);

					(*appendedCount)++;
				}

				if(defaultGetRows>-1 && *appendedCount >= defaultGetRows){
					printf("JSON Array Length: %zu\n", *appendedCount);
					break;
				}


				if (dir && includeSub) {
					DIR* new_dir = opendir(path);
					if (new_dir) {
						stack[++top].dir = new_dir;
						strcpy(stack[top].path, path);

						printf("%d %s: \n", top, stack[top].path);
					}
				}
			}
		}
    	printf("%d  %d", top, currentStack);
    	closedir(current_dir);
    }

	return true;
}

int get_files_info(const char* root_path, const char* dir_path, FILE *fw, int startRow, int* currentCount, long* allFileSize,
		char **filter_include, char **filter_ignore, int num_include, int num_ignore)
{

	typedef struct {
		DIR* dir;
	    char path[PATH_MAX];
	} DirInfo;

	DirInfo stack[100000]; // Stack to hold directory information
	int top = -1; // Top of the stack

	DIR* dir = opendir(dir_path);
	if (!dir) {
		printf("Error opening directory: %s\n", dir_path);
	    return false;
	}

	stack[++top].dir = dir;
	strcpy(stack[top].path, dir_path);

	int currentStack=0;

	while (top >= currentStack) {
		DIR* current_dir = stack[currentStack].dir;
    	char* current_path = stack[currentStack].path;

    	currentStack++;


    	struct dirent *entry;
    	while ((entry = readdir(current_dir))) {

    		if (entry == NULL) {
//				closedir(current_dir);
				printf("close: %s", current_dir);
				continue;
			}

			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
				continue;
			}

			char path[PATH_MAX];
			char errorMsg[200];
			snprintf(path, sizeof(path), "%s/%s", current_path, entry->d_name);
			replaceDoubleSlash(path);
			printf("cur path : %s\n", path);

			struct stat file_stat;
			if (stat(path, &file_stat) == 0) {
				int dir = 0;
				if (S_ISDIR(file_stat.st_mode)) dir=1;

				if(!dir && (num_include > 0 || num_ignore > 0)) {
					if (match_filters(path, filter_include, filter_ignore, num_include, num_ignore, true)) {
						printf("%s : Match found.\n", path);
					} else {
	//        			printf("%s : No match found.\n", path);
						continue;
					}
				}

				if(!dir){
					(*currentCount)++;

					if( *currentCount <= startRow )
					{
						continue;
					}

					int filesize = (int)file_stat.st_size;

					if( (*allFileSize + filesize) >  MAX_FILES_SIZE) {
						closedir(current_dir);
						return true;
					}

					(*allFileSize)+=filesize;

					char* json_str = get_file_info(path, errorMsg);

					if(json_str==NULL){
						return false;
					}
					fprintf(fw, "%s,", json_str);
					free(json_str);

					printf("added file : %s \n", path);
				}else {
					(*currentCount)++;

					if( *currentCount <= startRow )
					{
						continue;
					}

					char* json_str = get_file_info(path, errorMsg);

					if(json_str==NULL){
						return false;
					}
					fprintf(fw, "%s,", json_str);
					free(json_str);

					printf("added dir : %s \n", path);
					//하위 포함 하지 않음
					/*DIR* new_dir = opendir(path);
					if (new_dir) {
						stack[++top].dir = new_dir;
						strcpy(stack[top].path, path);

						printf("%d %s: \n", top, stack[top].path);
					}*/
				}
			}
    	}
    	printf("%d  %d\n", top, currentStack);
    	closedir(current_dir);
    }

    return true;
}

int scan_directory_info(const char* root_path, const char* dir_path, FILE *fw, char **filter_include, char **filter_ignore, int num_include, int num_ignore) {

	typedef struct {
		DIR* dir;
	    char path[PATH_MAX];
	} DirInfo;

	DirInfo stack[100000]; // Stack to hold directory information
	int top = -1; // Top of the stack

	DIR* dir = opendir(dir_path);
	if (!dir) {
		printf("Error opening directory: %s\n", dir_path);
	    return false;
	}

	stack[++top].dir = dir;
	strcpy(stack[top].path, dir_path);

	int currentStack=0;

	while (top >= currentStack) {
		DIR* current_dir = stack[currentStack].dir;
    	char* current_path = stack[currentStack].path;

    	currentStack++;


		struct dirent *entry;
		while ((entry = readdir(current_dir))) {
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
				continue;
			}

			char path[PATH_MAX];
			//char errorMsg[200];
			snprintf(path, sizeof(path), "%s/%s", current_path, entry->d_name);
			replaceDoubleSlash(path);
			struct stat file_stat;
			if (stat(path, &file_stat) == 0) {
				int dir = 0;
				if (S_ISDIR(file_stat.st_mode)) dir=1;

				if(!dir && (num_include > 0 || num_ignore > 0)) {
					if (match_filters(path, filter_include, filter_ignore, num_include, num_ignore, true)) {
						printf("%s : Match found.\n", path);
					} else {
						printf("%s : No match found.\n", path);
						continue;
					}
				}

				if(!dir){
					int read = file_stat.st_mode & S_IRUSR ? 1 : 0;
					int write = file_stat.st_mode & S_IWUSR ? 1 : 0;
					//        	int execute = file_stat.st_mode & S_IXUSR ? 1 : 0;
					const char* filename = getFileNameFromPath(path);
					char crc32_hex_string[8];
					if(read)
						convertToHexString(crc32_hex_string, calculateCRC32(path));

					char* jsonStr = NULL;

					int result = asprintf(&jsonStr,
							"{\"NAME\":\"%s\",\"PATH\":\"%s\",\"RELPATH\":\"%s\",\"ROOTPATH\":\"%s\",\"CHECKSUM\":\"%s\",\"SIZE\":%lld,\"ISDIR\":%i,\"MDATE\":%ld,\"READ\":%i,\"WRITE\":%i, \"ERRMSG\":\"%s\" }",
							filename, path, get_relative_path(path, root_path), "", crc32_hex_string, file_stat.st_size, dir, (long)file_stat.st_mtime*1000, read, write, (read? "" : "can't read" ));

					if (result == -1) {
						printf("Failed to allocate memory for JSON string");
						return false;
					}


					fprintf(fw, "%s\n",jsonStr);
					free(jsonStr);
				}

				if (dir) {
					DIR* new_dir = opendir(path);
					if (new_dir) {
						stack[++top].dir = new_dir;
						strcpy(stack[top].path, path);

						printf("%d %s: \n", top, stack[top].path);
					}
				}
			}
		}

    	printf("%d  %d", top, currentStack);
    	closedir(current_dir);
    }

    return true;

}

char* readFileContents(const char *filePath) {
    FILE* file = fopen(filePath, "r");
    if (file == NULL) {
        printf("Error opening the file: %s\n", filePath);
        return NULL;
    }

    // Determine the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for reading the file content
    char* buffer = (char*)malloc(file_size + 1);
    if (buffer == NULL) {
        printf("Memory allocation failed.\n");
        fclose(file);
        return NULL;
    }

    // Read the file content using fread
    size_t bytes_read = fread(buffer, 1, file_size, file);

    if (bytes_read != file_size) {
        printf("Error reading the file.\n");
        free(buffer);
        fclose(file);
        return NULL;
    }
    // Null-terminate the buffer to treat it as a C-style string
    buffer[bytes_read] = '\0';
    // Close the file
    fclose(file);
    // Do something with the file content in the buffer
    // Don't forget to free the allocated memory
    // free(buffer);

    return buffer;
}

bool match(const char *source, const char *filter, bool is_case_sensitive) {
    int flags = REG_EXTENDED | REG_NOSUB;
    if (!is_case_sensitive) {
        flags |= REG_ICASE;
    }

    regex_t regex;
    if (regcomp(&regex, filter, flags) != 0) {
        return false; // Failed to compile regex
    }

    int result = regexec(&regex, source, 0, NULL, 0);
    regfree(&regex);
    return (result == 0);
}

// Function to match file path against a list of inclusive and exclusive filters
bool match_filters(const char *source, char **filter_include, char **filter_ignore, int num_include, int num_ignore, bool is_case_sensitive) {
    bool default_case = true;

    if (source[strlen(source) - 1] != '/') {
        // Check for ignore filters first
        for (int i = 0; i < num_ignore; i++) {
        	printf("ignore filter %s %s \n" , source, filter_ignore[i]);
            if (match(source, filter_ignore[i], is_case_sensitive)) {
            	printf("ignore filter matched. %s %s \n" , source, filter_ignore[i]);
                return false;
            }
        }
    }

    // Check for include filters
    for (int i = 0; i < num_include; i++) {
        default_case = false;
        printf("include filter %i,  %s,  %s,  \n" , i,  source, filter_include[i]);
        if (match(source, filter_include[i], is_case_sensitive)) {
        	printf("include filter matched. %i, %s, %s, \n" , i, source, filter_include[i]);
            return true;
        }
    }

    return default_case;
}


const char* get_relative_path(const char* filePath, const char* rootPath ) {

	// Find the position where 'rootPath' occurs in 'filePath'
	const char* relativePath = strstr(filePath, rootPath);

	if (relativePath != NULL) {
		// If 'rootPath' is found in 'filePath', move 'relativePath' to the position after 'rootPath'
		relativePath += strlen(rootPath);

		// Check if 'relativePath' starts with a slash '/'
		if (*relativePath == '/') {
			return relativePath+1; // Return the substring starting with the slash
		}

		return relativePath;
	}

	return "";
}

void trim(char *str) {
    int len = strlen(str);

    // Trim leading spaces
    int start = 0;
    while (isspace(str[start])) {
        start++;
    }

    // Trim trailing spaces
    int end = len - 1;
    while (end >= 0 && isspace(str[end])) {
        end--;
    }

    // Adjust the end to the actual content in the string
    end++;

    // Move the content to the beginning of the string
    memmove(str, str + start, end - start);
    str[end - start] = '\0';
}
void convertToHexString(char *output, unsigned long value) {
     sprintf(output, "%8lx", value);

     trim(output);
}
unsigned long calculateCRC32(const char *file_path) {
    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", file_path);
        return 0;
    }

    unsigned long crc = 0;
    unsigned char buffer[1024];
    size_t read_len;

    while ((read_len = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        crc = crc32(crc, buffer, read_len);
    }

    fclose(file);

    return crc;
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

#ifndef _BRMLIB


#ifndef _TESTER

#endif
#endif
