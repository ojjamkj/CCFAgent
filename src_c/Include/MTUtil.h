#include <regex.h>
#include <zlib.h>


#ifndef   __INC_MTUTIL 
#define   __INC_MTUTIL

#ifndef _POSTGRE

#endif
//typedef struct {
//    char** array;
//    size_t size;
//    size_t capacity;
//} StringArrayList;
//void initStringArrayList(StringArrayList* list);
//void addStringElement(StringArrayList* list, const char* element);
//void freeStringArrayList(StringArrayList* list);

char * rltrim( char *str );
double getDoubleToString(double dVal);
char *  rtrim( char *str );
char *  McRTrim(char *lpszSource);
char *  McRTrim2(char *lpszSource);
char* TrimToken(char *lpszSource, char tk);
int index(char *lpszSource, char tk, int index);
unsigned char StringChange( char *pData, char *pSrc, char *pDest );
void getSubstr(char *lpszSource, char *desc, int idxs, int idx2);

//char*   DetailTimeStamp(char *lpszDateTime);
//void	PrintTimeStamp1(char *msg1, char *msg2);
//void	GetCurrentTimeStr(char *TimeBuffer);
//void	GetCurrentDateStr(char *DateBuffer);
//char*	CreateVersionString(char *);

//char*	GetSysEnv(char* ObjectName);

//long	McRandom( long );
long    mIsNumeric(char *buf, long n);
long	mIsAlpha(char *buf, long n);

#define  BASESIZE 201
//unsigned char *Base64Encode(const unsigned char *str, int length, int *ret_length);
//unsigned char *Base64Decode(const unsigned char *str, int length, int *ret_length);

//#ifndef _BRMLIB
////modified by DSKIM 2016.11.23: AES256
//int aes_init256(unsigned char* key, unsigned char* iv, EVP_CIPHER_CTX * e_ctx, EVP_CIPHER_CTX * d_ctx);
//int aes_init128(unsigned char* key, unsigned char* iv, EVP_CIPHER_CTX * e_ctx, EVP_CIPHER_CTX * d_ctx);
//unsigned char* aes_encrypt(EVP_CIPHER_CTX* e, unsigned char* plaintext, int* len);
//unsigned char* aes_decrypt(EVP_CIPHER_CTX* e, unsigned char* ciphertext, int* len);
//#endif

//static char Base64Table[] ={
//'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
//'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
//'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
//'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
//'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/', '\0'
//};
//
//static char Base64Pad = '=';

#define CHECK_SUM_BUFFER_SIZE 1024

//int calculate_sha256(const char* filename, const char* sha256sum, char* error_msg);
void convertToHexString(char *output, unsigned long value);
unsigned long calculateCRC32(const char *file_path);
int createFile(char* filename, const unsigned char* data, size_t length, char *msg);
char* get_file_info(const char* path, char* msg);
int get_directory_info(const char* dir_path, FILE *fw, int includeSub, int defaultGetRows, regex_t regex,
		int regExpValid, int includeMode, int* appendedCount);
int get_files_info(const char* root_path, const char* dir_path, FILE *fw, int startRow, int* currentCount, long* allFileSize,
		char **filter_include, char **filter_ignore, int num_include, int num_ignore);
int scan_directory_info(const char* root_path, const char* dir_path, FILE *fw, char **filter_include, char **filter_ignore, int num_include, int num_ignore);
bool match_filters(const char *source, char **filter_include, char **filter_ignore, int num_include, int num_ignore, bool is_case_sensitive);
bool match(const char *source, const char *filter, bool is_case_sensitive);
const char* get_relative_path(const char* filePath, const char* rootPath );
char* readFileContents(const char* filePath);
char * removeSlash(char* str);
#endif    //   __INC_MTUTIL

