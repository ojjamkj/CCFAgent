#if defined(__hpux) || (WIN32) || (__GNUC__)
#include        <openssl/evp.h>
#else
#include        <crypto/evp.h>
#endif
#include 	<jansson.h>


#ifndef   __INC_MTUTIL 
#define   __INC_MTUTIL

#ifndef _POSTGRE

#endif
char * rltrim( char *str );
double getDoubleToString(double dVal);
char *  rtrim( char *str );
char *  McRTrim(char *lpszSource);
char *  McRTrim2(char *lpszSource);
char* TrimToken(char *lpszSource, char tk);
int index(char *lpszSource, char tk, int index);
unsigned char StringChange( char *pData, char *pSrc, char *pDest );
void getSubstr(char *lpszSource, char *desc, int idxs, int idx2);

char*   DetailTimeStamp(char *lpszDateTime);
void	PrintTimeStamp1(char *msg1, char *msg2);
void	GetCurrentTimeStr(char *TimeBuffer);
void	GetCurrentDateStr(char *DateBuffer);
char*	CreateVersionString(char *);

char*	GetSysEnv(char* ObjectName);

long	McRandom( long );
long    mIsNumeric(char *buf, long n);
long	mIsAlpha(char *buf, long n);

#define  BASESIZE 201
unsigned char *Base64Encode(const unsigned char *str, int length, int *ret_length);
unsigned char *Base64Decode(const unsigned char *str, int length, int *ret_length);

#ifndef _BRMLIB
//modified by DSKIM 2016.11.23: AES256     
int aes_init256(unsigned char* key, unsigned char* iv, EVP_CIPHER_CTX * e_ctx, EVP_CIPHER_CTX * d_ctx);
int aes_init128(unsigned char* key, unsigned char* iv, EVP_CIPHER_CTX * e_ctx, EVP_CIPHER_CTX * d_ctx);
unsigned char* aes_encrypt(EVP_CIPHER_CTX* e, unsigned char* plaintext, int* len);
unsigned char* aes_decrypt(EVP_CIPHER_CTX* e, unsigned char* ciphertext, int* len);
#endif

static char Base64Table[] ={
'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/', '\0'
};

static char Base64Pad = '=';

#define CHECK_SUM_BUFFER_SIZE 1024

json_t* get_directory_info(const char* dir_path);
//int calculate_md5(const char* filename, const char* md5sum, char* error_msg);
int calculate_sha256(const char* filename, const char* sha256sum, char* error_msg);
unsigned short calculate_crc32(const char* filename, const char* crcsum, char* error_msg);
int createFile(char* filename, const unsigned char* data, size_t length, char *msg);
int printFileAttributes(const char* path, char* msg, size_t msg_size);
char * removeSlash(char* str);
#endif    //   __INC_MTUTIL

