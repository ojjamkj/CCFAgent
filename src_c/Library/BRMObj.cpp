/*******************************************************************/
#include <stdio.h>
#include	<stdlib.h>
#include	<string.h>

#ifdef		_AIX
#include	<strings.h>
#endif		//_AIX
#ifdef		__hpux
#include	<strings.h>
#endif		//__hpux
#ifdef		__GNUC__
#include	<strings.h>
#endif

#ifndef		_OS390
#include	<malloc.h>
#endif
#include    "MItem.h"
#include    "MTConvt.h"
#include    "BRMObj.h"
extern char _DEBUG_YN[2];
 
//#define  OBJ_MAX_LENGTH    500000

CBRMObj::CBRMObj() {
	m_mode = 1;

	m_readPtr = 0;
	m_length = 0;
	m_bConv = 0;

	m_Buffer = (unsigned char *) malloc(OBJ_INI_LENGTH + 1);

	m_allocLength = OBJ_INI_LENGTH;
}

CBRMObj::CBRMObj(long maxSize, char *conv) {
	m_mode = 1;

	m_readPtr = 0;
	m_length = 0;

	if (!strcmp(conv, "A2E"))
		m_bConv = 1;
	else if (!strcmp(conv, "E2A"))
		m_bConv = 2;
	else
		m_bConv = 0;

	printf(" maxSize(01) [%d] ====> \n", maxSize);
	m_Buffer = (unsigned char *) malloc(maxSize + 1);

	m_allocLength = maxSize;
}

CBRMObj::CBRMObj(char *pBuffer, long maxSize, char *conv) {
	m_mode = 0;

	m_readPtr = 0;
	m_length = 0;

	if (!strcmp(conv, "A2E"))
		m_bConv = 1;
	else if (!strcmp(conv, "E2A"))
		m_bConv = 2;
	else
		m_bConv = 0;

	m_Buffer = (unsigned char *) pBuffer;
	m_allocLength = maxSize;
	printf(" maxSize(02) [%d] ====> \n", maxSize);

}

CBRMObj::~CBRMObj() {
	if (m_mode) {
		free (m_Buffer);
	}
}

long CBRMObj::getLength() {
	return m_length;
}

long CBRMObj::setLength(long Leng) {
	if (Leng > m_allocLength)
		return -1;

 
	m_length = Leng;
	m_readPtr = 0;
	return 0;
}

void CBRMObj::Clear() {
	m_length = 0;
	m_readPtr = 0;

	if (m_Buffer) {
		free(m_Buffer);
		m_Buffer = (unsigned char *) malloc(m_allocLength+1);
		memset(m_Buffer, 0x00, m_allocLength);
	}
}

void CBRMObj::Clear1() {
	m_length = 50;
	m_readPtr = 0;

//    if ( m_Buffer ) {
//   		memset( m_Buffer, 0x00, 50 );
//    }
}

void CBRMObj::Free(){
    if (m_Buffer){
       free (m_Buffer);
    }
}
void CBRMObj::Clear2(long leng) {   //사용하는곳 없음. mbrscheck.에서만
	m_length = 0;
	m_readPtr = 0;

	if (m_Buffer)
		free (m_Buffer);

	m_Buffer = (unsigned char *) malloc(leng + 1);
	memset( m_Buffer, 0x00, leng+1 );
	m_allocLength = leng;
}

void CBRMObj::setMaxPramSize(int maxSize) {
	if (m_mode == 0)
		m_allocLength = maxSize;

}

void CBRMObj::WriteDataLength() {
	long leng;

	if (m_length <= 50)
		leng = 0;
	else
		leng = m_length - 50;
	printf(" writedatalength [%ld] \n", leng);
	m_Buffer[0] = (unsigned char)(leng / (65536*256));
	leng %= (65536*256);
	m_Buffer[1] = (unsigned char) (leng / 65536);
	leng %= (65536 );
	m_Buffer[2] = (unsigned char) (leng / 256 & 0xff);
	m_Buffer[3] = (unsigned char) (leng % 256 & 0xff);
}
void CBRMObj::setMaxLength(long newLength) {
	long leng = newLength-50;
	m_Buffer[0] = (unsigned char)(leng / (65536 * 256));
	leng %= (65536 * 256);
	m_Buffer[1] = (unsigned char)(leng / 65536);
	leng %= (65536);
	m_Buffer[2] = (unsigned char)(leng / 256 & 0xff);
	m_Buffer[3] = (unsigned char)(leng % 256 & 0xff);
}


long CBRMObj::ReadDataLength() {
	long leng;

	char* str = (char*)malloc(sizeof(char) * (30 + 1));
	strncpy(str, (const char*)m_Buffer,30);

	printf("Result: %s\n", str);
	long num = strtol(str, NULL, 10);

	printf("total size Result: %ld\n", num);

	free(str);
	m_readPtr = 30;

	return num;
} 

long CBRMObj::BufSizeCheck(long leng) {
    //확인사항2015.02 leng ->4096	if( leng <(4096*2)) leng=4096*2;
	if (m_length + leng > m_allocLength) {
		//2015.03.10 modified
		/*
		if (m_allocLength + (leng) > OBJ_MAX_LENGTH)
			return -1;
		m_allocLength += (leng);
		*/

		if (m_allocLength + (4096) > OBJ_MAX_LENGTH)
			return -1;

		m_allocLength = OBJ_MAX_LENGTH;
		
		//realloc(m_Buffer, m_allocLength);
		unsigned char *q;
		q = (unsigned char *)realloc(m_Buffer, m_allocLength);
		if (!q) m_Buffer = q;
		else {
			q = (unsigned char *)realloc(m_Buffer, m_allocLength);
			if (!q) m_Buffer = q;
		}

	//printf("malloclength [%d][%d]",m_Buffer,m_allocLength);  
	}
	return 0;
}

unsigned char * CBRMObj::getBuffer() {
	return m_Buffer;
}
int CBRMObj::WriteByte(unsigned char c) {
	if (BufSizeCheck(1))
		return -1;

	m_Buffer[m_length] = c;

	m_length++;

	return 0;
}


/*
 int CBRMObj::WriteRows( long lVal )
 {
 sprintf( m_tempstr, "%6d", lVal );
 m_tempstr[6] = 0;
 return WriteString( m_tempstr );
 }
 */

int CBRMObj::WriteLong(long lVal) {
	sprintf(m_tempstr, "%ld", lVal);

	return WriteString(m_tempstr);
}

int CBRMObj::WriteDouble(double dbl) {
	if (_DEBUG_YN[0] == 'Y') printf("[%s][%d] \n", __FILE__, __LINE__  );
	sprintf(m_tempstr, "%lf", dbl);
	m_tempstr[30]=0;
	if (_DEBUG_YN[0] == 'Y') printf("[%s][%d] \n", __FILE__, __LINE__ );
	return WriteString(m_tempstr);
}

/*
 void CBRMObj::WriteLongPosition( long lVal, int pos )
 {

 if ( LONG_TYPE_SIZE == 4 ) {
 int intVal = lVal;

 if( m_bReorder ) ReorderBytesInt( &intVal );
 memcpy( m_Buffer+pos, (unsigned char*)&intVal, LONG_TYPE_SIZE );

 } else {
 if( m_bReorder ) ReorderBytesLong( &lVal );

 memcpy( m_Buffer+pos, (unsigned char*)&lVal, LONG_TYPE_SIZE );
 }
 }
 */

int CBRMObj::WriteString(char* lpsz) {
	long leng;
	if (lpsz == NULL) {
		m_Buffer[m_length] = 0;
		m_Buffer[m_length + 1] = 0;
		m_length += 2;
		return -1;
	}
	if ((leng = strlen(lpsz)) == 0) {
		m_Buffer[m_length] = 0;
		m_Buffer[m_length + 1] = 0;
		m_length += 2;
		return 0;
	}
	if (BufSizeCheck(leng))
		return -1;
	if (m_bConv != 0) {
		long convLeng;
		unsigned char pConvStr[CONDITION_STR_SIZE];

		if (m_bConv == 1) {
			convLeng = ConvA2E(SOSI_INS, (unsigned char*)lpsz, leng, pConvStr, CONDITION_STR_SIZE - 1);
		}else {
			convLeng = ConvE2A(SOSI_DEL, (unsigned char*)lpsz, leng, (unsigned char*)pConvStr, CONDITION_STR_SIZE - 1);
		}
		m_Buffer[m_length] = (unsigned char) (convLeng / 256);
		m_Buffer[m_length + 1] = (unsigned char) (convLeng % 256);
		m_length += 2;

		memcpy(m_Buffer + m_length, (char*) pConvStr, convLeng);
		m_length += convLeng;

	} else {
		m_Buffer[m_length] = (unsigned char) (leng / 256);
		m_Buffer[m_length + 1] = (unsigned char) (leng % 256);
		m_length += 2;

		memcpy(m_Buffer + m_length, (char*) lpsz, leng);
		printf("leng=[%d] %c %c \n", m_Buffer[m_length], m_Buffer[m_length + 1]);
		printf("leng=[%d] %d %d \n", m_Buffer[m_length], m_Buffer[m_length + 1]);
		m_length += leng;
	}
	if (_DEBUG_YN[0] == 'Y') printf("m_allocLength =[%d] [%s][%d] \n", m_allocLength, __FILE__, __LINE__);

	return 0;
}
/*
int CBRMObj::WriteByteFile(char* lpsz, int leng, int first, long file_size2) {
	printf(" WriteByteFile  001 \n");
	long file_size = file_size2;
	printf(" WriteByteFile  002 \n");
	if (lpsz == NULL) {
		m_Buffer[m_length] = 0;
		m_Buffer[m_length + 1] = 0;
		m_Buffer[m_length + 2] = 0;
		m_Buffer[m_length + 3] = 0;
		m_length += 4;
		printf(" WriteByteFile  003 \n");
		return -1;
	}
	printf(" WriteByteFile  004 \n");
	if (leng <= 0) {
		return 0;
	}
	printf(" WriteByteFile  005 \n");
	if (BufSizeCheck(leng))
		return -1;
	printf(" WriteByteFile  006 \n");
	if (first == 0) {
		printf(" WriteByteFile  007 \n");
		m_Buffer[m_length] = (unsigned char)(file_size2 >> 8 >> 8 >> 8);
		file_size = (unsigned char)(file_size2 >> 8 >> 8 >> 8);
		printf("fileSize[0] [%d] \n", file_size);

		m_Buffer[m_length + 1] = (unsigned char)(file_size2 << 8 >> 8 >> 8 >> 8);
		file_size = (unsigned char)(file_size2 << 8 >> 8 >> 8 >> 8);
		printf("fileSize[1] [%d] \n", file_size);


		m_Buffer[m_length + 2] = (unsigned char)(file_size2 << 8 << 8 >> 8 >> 8 >> 8);
		file_size = (unsigned char)(file_size2 << 8 << 8 >> 8 >> 8 >> 8);
		printf("fileSize[2] [%d] \n", file_size);


		m_Buffer[m_length + 3] = (unsigned char)(file_size2 << 8 << 8 << 8 >> 8 >> 8 >> 8);
		file_size = (unsigned char)(file_size2 << 8 << 8 << 8 >> 8 >> 8 >> 8);
		printf("fileSize[2] [%d] \n", file_size);
		m_length += 4;
	}
	printf(" WriteByteFile  008 \n");
	printf("first[%d],file_size[all] =[%d] [%d][%d][%d] [%d] \n", first, file_size2, m_Buffer[m_length], m_Buffer[m_length + 1], m_Buffer[m_length + 2], m_Buffer[m_length + 3]);

	printf(" WriteByteFile  008--01 m_length=[%d] leng= [%d]  \n", m_length, leng);
	printf(" m_Buffer szie = [%d]  :  m_allocLength [%d]  m_length = [%d] \n", sizeof(m_Buffer), m_allocLength, m_length);
	memcpy(m_Buffer + m_length, (char*)lpsz, leng);
	printf(" WriteByteFile  009 \n");
	m_length += leng;
	if (_DEBUG_YN[0] == 'Y') printf("m_allocLength =[%d] [%s][%d] \n", m_allocLength, __FILE__, __LINE__);
	printf("WriteByteFile  end ==>leng[%d], first= [%d] \n", leng, first);
	return 0;
}*/
int CBRMObj::WriteByteFile(char* lpsz, int leng,int first, long file_size2) {
	long file_size = file_size2;
	if (lpsz == NULL) {
		WriteLong(file_size2);
		printf(" WriteByteFile  003 \n");
		return -1;
	}
	printf(" WriteByteFile  004 \n");
	if (leng <= 0) {
		return 0;
	}
	printf(" WriteByteFile  005 \n");
	if (BufSizeCheck(leng))
		return -1;
	printf(" WriteByteFile  006 \n");
	printf(" WriteByteFile  008--01 m_length=[%d] leng= [%d]  \n", m_length , leng );
	printf(" m_Buffer szie = [%d]  :  m_allocLength [%d]  m_length = [%d] \n", sizeof(m_Buffer), m_allocLength , m_length);
	memcpy(m_Buffer + m_length, (char*)lpsz, leng);
	printf(" WriteByteFile  009 \n");
	m_length += leng;
	if (_DEBUG_YN[0] == 'Y') printf("m_allocLength =[%d] [%s][%d] \n", m_allocLength, __FILE__, __LINE__);
	printf("WriteByteFile  end ==>leng[%d], first= [%d] \n", leng, first);
	return 0;
}
int CBRMObj::WritePack(double dbl, int leng, int m) {
	char sFormat[20];
	char sTemp[STR_VALUE_SIZE+1];
	char *pData;
	unsigned char lData[PACK_TYPE_SIZE];
	unsigned char c;
	int sign;
	int Idx;
	int digit;

	if (BufSizeCheck(STR_VALUE_SIZE))
		return -1;

	memset(lData, 0x00, PACK_TYPE_SIZE);

	if (dbl < 0) {
		sign = 13;
		dbl *= -1;
	} else
		sign = 12;

	if (leng > 18)
		leng = 18;
	if (m >= leng)
		m = leng - 1;

	sprintf(sFormat, "%%0%d.%dlf", leng + 1, m);
	sprintf(sTemp, sFormat, dbl);
	pData = sTemp + strlen(sTemp) - leng - 1;

	Idx = 0;
	if (leng % 2)
		digit = 1;
	else
		digit = 0;

	while (*pData) {
		if (*pData != '.') {
			c = *pData - '0';
			if (digit)
				c = c << 4;

			lData[Idx] |= c;

			if (!digit)
				Idx++;

			digit = !digit;
		}

		pData++;
	}

	lData[Idx] |= sign;

	memcpy(m_Buffer + m_length, lData, PACK_TYPE_SIZE);

	m_length += PACK_TYPE_SIZE;

	return 0;
}

long CBRMObj::LastReadCheck(long leng) {

	if (m_readPtr + leng > m_length)
		return -1;

	return 0;
}

unsigned char CBRMObj::ReadByte() {
	unsigned char c;

	if (LastReadCheck(1))
		return 0;
	c = m_Buffer[m_readPtr];
	m_readPtr++;

	return c;
}

long CBRMObj::ReadLong() {
	long lData;

	if (ReadString(m_tempstr) == -1) {
		lData = 0;
	}
	else {
		if ((m_tempstr != NULL) && (m_tempstr[0] == '\0')) {
			lData = 0;
		}
		else {
			lData = atoi(m_tempstr);
		}
	}
	return lData;
}

double CBRMObj::ReadDouble() {
	double dData;
	if (ReadString(m_tempstr) == -1) {
		dData = 0.0;
	}
	else {
		if ((m_tempstr != NULL) && (m_tempstr[0] == '\0')) {
			dData = 0;
		}
		else {
			dData = atof(m_tempstr);
		}
	}

	return dData;
}

long CBRMObj::ReadPString(char** pStr) {
	long leng;

	leng = (long) (m_Buffer[m_readPtr] * 256 + m_Buffer[m_readPtr + 1]);

	if (LastReadCheck(leng))
		return -1;

	m_readPtr += 2;

	if (m_bConv != 0) {
		long convLeng;
		unsigned char p_tempstr[CONDITION_STR_SIZE];
		unsigned char pConvStr[CONDITION_STR_SIZE];

		memcpy(p_tempstr, m_Buffer + m_readPtr, leng);

		if (m_bConv == 1)
			convLeng = ConvE2A(SOSI_DEL, p_tempstr, leng,
					(unsigned char*) pConvStr, CONDITION_STR_SIZE-1);
		else
			convLeng = ConvA2E(SOSI_INS, (unsigned char*) p_tempstr, leng,
					pConvStr, CONDITION_STR_SIZE-1);

		*pStr = (char *) malloc(convLeng + 1);
		memcpy(*pStr, pConvStr, convLeng);
		*pStr[convLeng] = 0;
	} else {
		*pStr = (char *) malloc(leng + 1);
		memcpy(*pStr, m_Buffer + m_readPtr, leng);
		(*pStr)[leng] = 0;
	}

	m_readPtr += leng;

	return 0;
}

long CBRMObj::ReadString(char* str) {
	long leng;

	
	leng = (long) (m_Buffer[m_readPtr] * 256 + m_Buffer[m_readPtr + 1]);
	printf("m_readPtr [%d] leng [%ld] \n", m_readPtr, leng);
	if (LastReadCheck(leng))
		return -1;
	printf("m_readPtr [%d] leng [%ld] \n", m_readPtr, leng);

	m_readPtr += 2;

	if (m_bConv != 0) {
		long convLeng;
		unsigned char pConvStr[CONDITION_STR_SIZE];

		memcpy(pConvStr, m_Buffer + m_readPtr, leng);

		if (m_bConv == 1)
			convLeng = ConvE2A(SOSI_DEL, pConvStr, leng, (unsigned char*) str,
					CONDITION_STR_SIZE-1);
		else
			convLeng = ConvA2E(SOSI_INS, pConvStr, leng, (unsigned char*) str,
					CONDITION_STR_SIZE-1);

		str[convLeng] = 0;
	} else {
		memcpy(str, m_Buffer + m_readPtr, leng);
		str[leng] = 0;
	}

	m_readPtr += leng;
	printf("m_readPtr [%d] leng [%ld] \n", m_readPtr, leng);

	return 0;
}

long CBRMObj::ReadByteFile(unsigned char* str,long length) {
	long leng;


	leng = length; // (long)(m_Buffer[m_readPtr] * 256 + m_Buffer[m_readPtr + 1]);

	if (LastReadCheck(leng))
		return -1;


	memcpy(str, m_Buffer + m_readPtr, leng);
	str[leng] = 0;

	m_readPtr += leng;

	return 0;
}
long CBRMObj::ReadNString(char* str, int maxLen) {
	long leng;

	leng = (long) (m_Buffer[m_readPtr] * 256 + m_Buffer[m_readPtr + 1]);

	if (LastReadCheck(leng))
		return -1;

	m_readPtr += 2;

	if (m_bConv != 0) {
		long convLeng;
		unsigned char pConvStr[CONDITION_STR_SIZE];

		memcpy(pConvStr, m_Buffer + m_readPtr, leng);

		if (m_bConv == 1)
			convLeng = ConvE2A(SOSI_DEL, pConvStr, leng, (unsigned char*) str,
					maxLen);
		else
			convLeng = ConvA2E(SOSI_INS, pConvStr, leng, (unsigned char*) str,
					maxLen);

		str[convLeng] = 0;
	} else {
		memcpy(str, m_Buffer + m_readPtr, maxLen);
		str[maxLen] = 0;
	}

	m_readPtr += leng;

	return 0;
}

double CBRMObj::ReadPack(int leng, int m) {
	double dData = 0.0;
	unsigned char sBuf[10];
	unsigned char sDigit[20];
	int i, j, pos;
	double prec;

	if (LastReadCheck (PACK_TYPE_SIZE))
		return 0;

	memcpy((char*) sBuf, m_Buffer + m_readPtr, PACK_TYPE_SIZE);
	m_readPtr += PACK_TYPE_SIZE;

	j = 0;
	for (i = 0; i < 10; i++) {
		sDigit[j] = sBuf[i] >> 4;
		j++;

		sDigit[j] = sBuf[i] & 0x0f;
		j++;
	}

	for (j = 0; j < 20; j++) {
		if (sDigit[j] == 12 || sDigit[j] == 13)
			break;
	}

	if (j == 20)
		return 0.0;

	pos = j - m;

	prec = 0.1;
	for (i = 0; i < pos; i++)
		dData = dData * 10.0 + sDigit[i];

	for (; i < j; i++) {
		dData = dData + sDigit[i] * prec;
		prec *= 0.1;
	}

	if (sDigit[i] == 13)
		dData *= -1.0;

	return dData;
}
