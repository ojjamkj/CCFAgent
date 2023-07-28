#if defined(__hpux) || (WIN32) || (__GNUC__)
#include	<openssl/evp.h>
#else
#include	<crypto/evp.h>
#endif
#if	defined(WIN32)
#define 	DEFAULT_INIFILE             "./agent.properties"
#define 	KEY_FILE             "./key.dat"

#elif defined(_OS390) || defined(_ZOS)
#define DEFAULT_INIFILE             "DD:BRMCONF"
#else
#define DEFAULT_INIFILE             "./agent.properties"
#define 	KEY_FILE             "./key.dat"
#endif



#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#if defined(_AIX) || defined(_OS390)
#include	<strings.h>
#endif		//_AIX
#ifdef		__hpux
#include	<strings.h>
#endif		//__hpux
#ifdef		__GNUC__
#include	<strings.h>
#endif		//
#ifdef  _OS390
#include <errno.h>
#else     
#include <malloc.h>
#endif

#include	<ctype.h>
#include	"MTPrfile.h"
#include    "MTUtil.h"
#ifdef _OS390
#define     SECTION_START   0x70
#define     SECTION_END     0x80
#else
#define     SECTION_START   '['
#define     SECTION_END     ']'
#endif

#ifdef _MSG_KO
#define     MSG_FILEOPEN_ERR  "파일을 열수 없습니다.\n"
#else
#define     MSG_FILEOPEN_ERR   "File open error.(%s)\n"
#endif

CMTProfile::CMTProfile(const char* lpszFileName) {
	m_lBufferSize = 256;
	m_pBuffer = new char[m_lBufferSize];
	m_lpszFileName = 0;
	m_pFile = 0;
	m_bFileExist = 0;

	if (lpszFileName) {
		m_lpszFileName = new char[strlen(lpszFileName) + 1];
		strcpy(m_lpszFileName, lpszFileName);
		if ((m_pFile = fopen(m_lpszFileName, "r")) != NULL) {
			m_bFileExist = 1;
		} else {
//			printf("File open error.(%s)\n",strerror(errno));
			//printf("File open error.(%s)\n", m_lpszFileName);
			printf(MSG_FILEOPEN_ERR, m_lpszFileName);
		}
	}
#ifdef		WIN32
	m_bReturnRead = 0; // false
#endif		//WIN32
	m_bEOFRead = 0;

}

CMTProfile::~CMTProfile() {
	delete[] m_pBuffer;
	if (m_pFile)
		fclose(m_pFile);
	if (m_lpszFileName)
		delete[] m_lpszFileName;
}

unsigned char CMTProfile::FileOpenCheck(void) {
	return m_bFileExist;
}

long CMTProfile::ReadLong(const char* lpszSection,
	const char*lpszEntry, long lDef) {
	char lpszVal[100];
	if (m_pFile) {
		fseek(m_pFile, 0, SEEK_SET);
	}
	int ret = GetIniKeyString(lpszSection, lpszEntry, lpszVal);
	if (ret == 0) {
		return atoi(lpszVal);
	}
	return lDef;
}
/*
long CMTProfile::ReadLong(const char* lpszSection, const char* lpszEntry,
		long lDef) {
	if (m_pFile) {
		fseek(m_pFile, 0, SEEK_SET);
		if (GoSection(lpszSection) && GoEntry(lpszEntry) && GetValue())
			return atoi(m_pBuffer);
	}

	return lDef;
}
*/
#ifndef _TESTER
void  CMTProfile::enc(char* lpszVal, char *lpszEncMethod)
{
	//modified by DSKIM 2016.11.23: AES256 �쟻�슜
	/*
	int dec_size = 0;
	unsigned char * tempDec;
	tempDec = Base64Decode((unsigned char*) lpszVal,strlen(lpszVal), &dec_size);
	// if decoded
	if (dec_size > 0) {
		memset(lpszVal, 0x00, 200); //200�쑝濡� �꽑�뼵�뻽�쓬.
		memcpy(lpszVal, tempDec, dec_size);
	}
	if (tempDec != NULL)
		free(tempDec);
	*/
	int dec_size = 0;
	unsigned char * tempDec=NULL;
//	if(lpszEncMethod[0] == 'A') {
	if( strcmp(lpszEncMethod,"AES256")==0  || strcmp(lpszEncMethod,"AES128")==0) {
		FILE *fp;
		char tempKey[201];
        memset(tempKey,0x00,sizeof(tempKey));
		if ((fp = fopen(KEY_FILE, "r")) != NULL) {
				fscanf(fp, "%s", &tempKey);
		} else {
				printf(MSG_FILEOPEN_ERR, KEY_FILE);
				return;
		}
		fclose(fp);

		int tempSize;
		unsigned char *key = Base64Decode((unsigned char *)tempKey ,strlen(tempKey), &tempSize);
		unsigned char iv[17];
        memset( iv , 0x00, sizeof(iv ));
		strncpy((char *)iv, (char *)key, 16);

		EVP_CIPHER_CTX en, de;
		if( strcmp(lpszEncMethod,"AES256")==0 ){
			aes_init256((unsigned char *)key, (unsigned char *)iv, &en, &de);
		}else if( strcmp(lpszEncMethod,"AES128")==0 ){
			aes_init128((unsigned char *)key, (unsigned char *)iv, &en, &de);
		}

		tempDec = Base64Decode((unsigned char*) lpszVal,strlen(lpszVal), &dec_size);
		int len = dec_size;
		tempDec = (unsigned char *)aes_decrypt(&de, tempDec, &len);

		if (key != NULL)	free(key); //2017-09-28 異붽� MEMORY AD2001
		 
		if (len > 0) {
			memset(lpszVal, 0x00, 200);
			memcpy(lpszVal, tempDec, len);
			lpszVal[len] = 0;
		}
	}
	else if (strcmp(lpszEncMethod, "BASE64") == 0) {
		
			tempDec = Base64Decode((unsigned char*)lpszVal, strlen(lpszVal), &dec_size);
		
			if (dec_size > 0) {
				memset(lpszVal, 0x00, 200);
				memcpy(lpszVal, tempDec, dec_size);
				lpszVal[dec_size] = 0;
			}
	}
	else {
		
			if (dec_size > 0) {
				int size = sizeof(tempDec);
				memset(lpszVal, 0x00, size);
				memcpy(lpszVal, tempDec, size);
				lpszVal[size] = 0;
			}
	}
	if (tempDec != NULL)
		free(tempDec);
	//
}
#endif

int CMTProfile::PutIniKeyString(const char *title,const  char *key, char *val)
{
/*	FILE *fpr, *fpw;
	int  flag = 0;
	char sLine[1024], sTitle[32], *wTmp;
	sprintf(sTitle, "[%s]", title);
	if (NULL == (fpr = fopen(m_lpszFileName, "r")))
		return - 1;// read the original file  
	sprintf(sLine, "%s.tmp", m_lpszFileName);
	if (NULL == (fpw = fopen(sLine, "w")))
		return - 1;// write temporary files        
	while (NULL != fgets(sLine, 1024, fpr)) {
		if(2 != flag) { // If the line to be modified is found, no internal operations will be performed  
			wTmp = strchr(sLine, '=');
			if ((NULL != wTmp) && (1 == flag)) {
				if(0 == strncmp(key, sLine, strlen(key))) { // The length depends on the file read 
					flag = 2;// Change the value for easy writing to the file  
					sprintf(wTmp + 1, " %s\n", val);
				}
			}
			else {
				if(0 == strncmp(sTitle, sLine, strlen(sTitle))) { // The length depends on the file read
					flag = 1; // find the title location  
				}
			}
		}
		fputs(sLine, fpw); // Write temporary files 
	}
	fclose(fpr);
	fclose(fpw);
	sprintf(sLine, "%s.tmp", filename);
	return rename(sLine, filename);// Update the temporary file to the original file 
	*/
	return -1;
}


 int CMTProfile::GetIniKeyString(const char *title, const char *key, char *buf)
{
	int  flag = 0;
	char sTitle[64], *wTmp;
	char sLine[1024];
	sprintf(sTitle, "[%s]", title);
/*
	if (NULL == (fp = fopen(m_lpszFileName, "r"))) {
		perror("fopen");
		return -1;
	}
*/
	while (NULL != fgets(sLine, 1024, m_pFile)) {
		// This is the comment line  
		if (0 == strncmp("//", sLine, 2)) continue;
		if ('#' == sLine[0])              continue;
		wTmp = strchr(sLine, '=');
		if ((NULL != wTmp) && (1 == flag)) {
			if (0 == strncmp(key, sLine, strlen(key))) { // The length depends on the file read  
//				if (0 == strncmp(key, sLine, strlen(key))) { // The length depends on the file read  
					sLine[strlen(sLine) - 1] = 0x00;
				while (*(wTmp + 1) == ' ') {
					*(wTmp + 1) = 0x00;
					wTmp++;
				}
				strcpy(buf, wTmp + 1);
				return 0;
			}
		}
		else {
			if (0 == strncmp(sTitle, sLine, strlen(sTitle))) { // The length depends on the file read  
//			if(0 == strncmp(sTitle, sLine, strlen(sTitle))) { // The length depends on the file read  
				flag = 1; // find the title location  
			}
		}
	}
	return -1;
}

const char* CMTProfile::ReadString(const char* lpszSection,
		 const char*lpszEntry, const char* lpszDefault, char* lpszVal,
		long lLength, bool enc) {
	char buf[1000];
	if (m_pFile) {
		fseek(m_pFile, 0, SEEK_SET);
	}
	int ret = GetIniKeyString(lpszSection, lpszEntry, buf);  
	//	printf("retf ==> [%s],[%d] \n", lpszEntry, ret);
	if (ret == 0) {
		buf[lLength - 1] = 0x00;
	}else {
		sprintf(buf, "%s", lpszDefault);
		buf[lLength - 1] = 0x00;
	}
	for (int i = 0; i < lLength; i++) {
		if (buf[i] == '\r')buf[i] = 0x00;
        if (buf[i] == '\n')buf[i] = 0x00;
        if (buf[i] == ' ') buf[i] = 0x00;
	}
    sprintf(lpszVal, "%s", buf);
//    rtrim(lpszVal);

	return lpszVal;
/*
	unsigned char * tempDec;
	if (m_pFile) {

		fseek(m_pFile, 0, SEEK_SET);
		if (GoSection(lpszSection) && GoEntry(lpszEntry) && GetValue()) {
			strncpy(lpszVal, m_pBuffer, lLength - 1);
			lpszVal[lLength - 1] = '\0';
 

			if (enc) {
				//modified by DSKIM 2016.11.23: 占쏙옙�슜�븯吏� 占쏙옙占쏙옙占쎈줈 二쇱꽍 泥섎━占�
				/ *
				int dec_size = 0;
				tempDec = Base64Decode((unsigned char*) lpszVal,strlen(lpszVal), &dec_size);
				// if decoded
				if (dec_size > 0) {
					memset(lpszVal, 0x00, 200); // 200占쏙옙占� 占쏙옙占쏀뻽占쏙옙.
					memcpy(lpszVal, tempDec, dec_size);
				}
				if (tempDec != NULL)
					free(tempDec);
				* /

			}
			//////////////////////////////////////////////////////

			return lpszVal;
		}
	} 
		strncpy(lpszVal, lpszDefault, lLength - 1);
		lpszVal[lLength - 1] = '\0';
		if (enc) {
			//modified by DSKIM 2016.11.23:  �궗�슜�븯吏� �븡�쑝誘�濡� 二쇱꽍 泥섎━�븿
			/ *
			int dec_size = 0;
			tempDec = Base64Decode((unsigned char*) lpszVal, strlen(lpszVal),&dec_size);
			// if decoded
			if (dec_size > 0) {
				memset(lpszVal, 0x00, 200); // 200�쑝濡� �꽑�뼵�뻽�쓬.
				memcpy(lpszVal, tempDec, dec_size);
			}

			if (tempDec != NULL)
				free(tempDec);
			* /
		}
		 
		return lpszVal;
*/
	}

	unsigned long CMTProfile::GoSection(const char* lpszSection) {
		int c;
		long lBufferCounter;

		do {
			SkipSpace();
			if ((c = Read()) != SECTION_START && c != EOF) {
				Unread();
				SkipLine();
				continue;
			}
			if (c == EOF)
				return 0;

			lBufferCounter = 0;
			while ((c = Read()) != '\n' && c != EOF && c != SECTION_END) {
				if (m_lBufferSize - 1 == lBufferCounter) {
					m_lBufferSize *= 2;
					m_pBuffer = (char *) realloc(m_pBuffer, m_lBufferSize);
				}

				m_pBuffer[lBufferCounter++] = c;
			}
			m_pBuffer[lBufferCounter] = '\0';

			if (c == EOF)
				return 0;
			else if (c == '\n') {
				Unread();
				SkipLine();
				continue;
			}

			SkipLine();

#ifdef	WIN32
			if (!_stricmp(m_pBuffer, lpszSection))
			return 1;
#else
			if (!strcasecmp(m_pBuffer, lpszSection))
				return 1;
#endif
		} while (1);
	}

	unsigned long CMTProfile::GoEntry(const char* lpszEntry) {
		int c;
		long lBufferCounter;

		do {
			SkipSpace();

			lBufferCounter = 0;
			while (isalnum(c = Read())) {
				if (m_lBufferSize - 1 == lBufferCounter) {
					m_lBufferSize *= 2;
					m_pBuffer = (char *) realloc(m_pBuffer, m_lBufferSize);
				}

				m_pBuffer[lBufferCounter++] = c;
			}

			if (c == EOF)
				return 0;
			else if (c == SECTION_START) {
				Unread();
				return 0;
			}

			Unread();
			m_pBuffer[lBufferCounter] = '\0';

#ifdef	WIN32
			if (!_stricmp(m_pBuffer, lpszEntry))
			return 1;
#else
			if (!strcasecmp(m_pBuffer, lpszEntry))
				return 1;
#endif

			SkipLine();
		} while (1);
	}

	unsigned long CMTProfile::GetValue(void) {
		int c;
		long lBufferCounter;

		SkipSpace();

		if (Read() != '=')
			return 0;
		SkipSpace();

		lBufferCounter = 0;
		while ((c = Read()) != '\n' && c != ';' && c != EOF) {
			if ((m_lBufferSize - 1) == lBufferCounter) {
				m_lBufferSize *= 2;
				m_pBuffer = (char *) realloc(m_pBuffer, m_lBufferSize);
			}

			m_pBuffer[lBufferCounter++] = c;
		}

		m_pBuffer[lBufferCounter] = '\0';

		while (lBufferCounter > 0) {
			lBufferCounter--;
			if (!isgraph(m_pBuffer[lBufferCounter]))
				m_pBuffer[lBufferCounter] = 0;
		}

		return 1;
	}

	void CMTProfile::SkipSpace(void) {
		int c;
		while ((c = Read()) == ' ' || c == '\t')
			;

		Unread();
	}

	void CMTProfile::SkipLine(void) {
		int c;
		while ((c = Read()) != '\n' && c != EOF)
			;

		if (c == EOF)
			Unread();
	}

	int CMTProfile::Read(void) {
		int c;

		if (m_pFile) {
			c = fgetc(m_pFile);

#ifdef		WIN32
			if (c == '\n')
			m_bReturnRead = 1;
			else
			m_bReturnRead = 0;
#endif		//WIN32
			if (c == EOF)
				m_bEOFRead = 1;

			return c;
		} else
			return EOF;
	}

	void CMTProfile::Unread() {
		if (m_pFile) {
			if (m_bEOFRead) {
				m_bEOFRead = 0;
				return;
			}

#ifdef		WIN32
			if (m_bReturnRead) {
				fseek(m_pFile, -2, SEEK_CUR);
				m_bReturnRead = 0; // false
			} else
#endif		//WIN32
			fseek(m_pFile, -1, SEEK_CUR);
		}
	}
	
	

