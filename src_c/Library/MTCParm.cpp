/*******************************************************************/
#include	<stdio.h>
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

#include    "MTParm2.h"
#include    "MTConvt.h"
#include	"MTExcept.h"

CMTParm2::CMTParm2()
{
    m_readPtr = 0;
    m_length = 0;
    m_allocLength = 0;
    m_bReorder = 0;
    m_bConv = 0; 
	m_Buffer = 0;
}

CMTParm2::CMTParm2( int maxSize, char *pBuffer )
{
    m_readPtr = 0;
    m_length = 0;
    m_allocLength = maxSize;
    m_bReorder = 0;
    m_bConv = 0; 
	m_Buffer = (unsigned char *) pBuffer;
}


CMTParm2::CMTParm2( int maxSize, char *pBuffer, long reOrder, char *conv )
{
    m_readPtr = 0;
    m_length = 0;
    m_allocLength = maxSize;
    m_bReorder = reOrder;
        
    if ( ! strcmp( conv, "A2E" )) m_bConv = 1;
    else if ( ! strcmp( conv, "E2A" )) m_bConv = 2;
    else m_bConv = 0;
        
	m_Buffer = (unsigned char *) pBuffer;
		
}

CMTParm2::~CMTParm2()
{
}

long  CMTParm2::getLength()
{
	return m_length;
}

void  CMTParm2::setMaxPramSize( int maxSize )
{
    m_allocLength = maxSize;
}

long  CMTParm2::setLength( long Leng )
{
    if ( Leng > m_allocLength ) return -1;
    m_length = Leng;
	m_readPtr = 0;
    return 0;
}

void CMTParm2::Clear()
{
    m_length = 0;
    m_readPtr = 0;
}	


long   CMTParm2::BufSizeCheck( long leng )
{
    if ( m_length + leng >= m_allocLength )
		return -1;
    else
		return 0;
}



void CMTParm2::WriteString( char* lpsz, int leng )
{
    long  ll;

	if ( BufSizeCheck( leng ) ) throw new CMTException( 9999, "Max length exceeded." );

    memset( m_Buffer+m_length, 0x00, leng+1 );

	if ( lpsz == NULL ) {
		ll = strlen( lpsz );
		memcpy( m_Buffer+m_length, lpsz, ll );
	}

	m_length += leng;
}


void CMTParm2::WriteLong( long lVal, int leng )
{
	int  i, ll;
	char sTemp[30];
	char sFormat[5];
	unsigned char bSign = (lVal<0) ? 0xDF : 0xCF;

    if ( BufSizeCheck( leng ) ) throw new CMTException( 9999, "Max length exceeded." );
    
	sprintf( sFormat,"%%0%d", leng );
	sprintf( sTemp, sFormat, lVal );

	ll = strlen(sTemp);
    for ( i=0; i<ll; i++ ) {
		sTemp[i] = ( sTemp[i] - '0' ) | 0xf0;
	}

	sTemp[ll-1] &= bSign;

	if ( ll > leng ) i = ll - leng;
	else i = 0;

	memcpy( m_Buffer+m_length, sTemp+i, leng );

	m_length += leng;
}


void CMTParm2::WriteDbl( double dbl, int leng, int m )
{
	int      i, point, pos;
	unsigned char sVal[31];
	char sFormat[10];
	unsigned char bSign = (dbl<0) ? 0xDF : 0xCF;

    if ( BufSizeCheck( leng ) ) throw new CMTException( 9999, "Max length exceeded." );

	if ( m >= leng ) m = leng - 1;

	sprintf( sFormat,"%%0%d.%dlf", leng, m );
   
	sprintf( (char*)sVal, sFormat, dbl );
	sVal[30]=0;
	
	for( i=0; sVal[i]; i++ ) {
		if ( sVal[i] != '.' ) {
		   sVal[i] = sVal[i] - '0';
		   sVal[i] |= 0xf0;
		} else {
		   point = i;
		}
	}

	if ( sVal[i-1] == '.' )
		sVal[i-2] &= bSign;
	else
		sVal[i-1] &= bSign;

	if ( point > leng - m ) pos = i - leng + m;
	else pos = 0;

	memcpy( m_Buffer+m_length, sVal+pos, leng-m );
	m_length += ( leng-m );

	memcpy( m_Buffer+m_length, sVal+point+1, m );
	m_length += m;
}


void CMTParm2::WritePack( double dbl, int leng, int m )
{
	char  sFormat[20];
	char  sTemp[31];
	char *pData;
	unsigned char lData[CMTParm2_PACK_TYPE_SIZE];
	unsigned char c;
	int   sign;
	int   Idx;
	int   digit;

	if ( BufSizeCheck( leng ) ) throw new CMTException( 9999, "Max length exceeded." );

	memset( lData, 0x00, CMTParm2_PACK_TYPE_SIZE );
	
    if ( dbl < 0 ) {
    	sign = 13;
    	dbl *= -1;
    }
	else sign = 12;

	if ( leng > 18 ) leng = 18;
	if ( m >= leng ) m = leng - 1;

    sprintf( sFormat, "%%0%d.%dlf", leng+1, m );
    sprintf( sTemp, sFormat, dbl );
    sTemp[30]=0;
	pData = sTemp + strlen(sTemp) - leng - 1; 


	
	Idx = 0;
	if ( leng % 2 ) digit = 1;
	else digit = 0;
	
	while ( *pData ) {
		if ( *pData != '.' ) {
			c = *pData - '0';
			if ( digit ) c = c << 4;
			
			lData[Idx] |= c;
			
			if ( ! digit ) Idx++;
			
			digit = ! digit;
		}
	
		pData++;	
	}
	
	lData[Idx] |= sign;

	memcpy( m_Buffer+m_length, lData, CMTParm2_PACK_TYPE_SIZE );

	m_length += CMTParm2_PACK_TYPE_SIZE;
}


long   CMTParm2::LastReadCheck( long leng )
{
    if ( m_readPtr + leng > m_length ) return -1;
    return 0;
}


char * CMTParm2::ReadString( char* str, int leng )
{
    if ( LastReadCheck(leng) ) throw new CMTException( 9999, "Max length exceeded." );

	memcpy( str, m_Buffer+m_readPtr, leng );

	str[leng] = 0;

    m_readPtr += leng;

	return str;
}


double CMTParm2::ReadPack( int leng, int m )
{
	double  dData = 0.0;
	unsigned char sBuf[10];
	unsigned char sDigit[20];
	int     i, j, pos;
	double  prec;

	if ( LastReadCheck(leng) ) throw new CMTException( 9999, "Max length exceeded." );

	memcpy( (char*) sBuf, m_Buffer+m_readPtr, CMTParm2_PACK_TYPE_SIZE );
    m_readPtr += leng;

    j = 0;
	for ( i=0; i<10; i++ ) {
		sDigit[j] = sBuf[i] >> 4;
		j++;
		
		sDigit[j] = sBuf[i] & 0x0f;
		j++;
	}

	for ( j=0; j<20; j++ ) {
		if ( sDigit[j] == 12 || sDigit[j] == 13 ) break;
	}
	
	if ( j == 20 ) return 0.0;
	
	pos = j - m;
	
	prec = 0.1;
	for ( i=0; i<pos; i++ )
		dData = dData * 10.0 + sDigit[i];

	for ( ; i<j; i++ ) {
		dData = dData + sDigit[i] * prec;
		prec *= 0.1;
	}
	
	if ( sDigit[i] == 13 ) dData *= -1.0;
	
	return dData;
}

double CMTParm2::ReadNum( int leng, int m )
{
	double        dData = 0.0;
	unsigned char bDigit, bSign;
	int		      i;
	double  	  prec = 0.1;

	if ( LastReadCheck(leng) ) throw new CMTException( 9999, "Max length exceeded." );

	bDigit =  m_Buffer[m_readPtr+leng-1];
	bSign  = bDigit >> 4;

	for ( i=0; i<leng-m; i++ ) {
		bDigit = m_Buffer[m_readPtr] & 0x0f;

		dData = dData * 10.0 + bDigit;
		m_readPtr++;
	}

	for ( ; i<leng; i++ ) {
		bDigit = m_Buffer[m_readPtr] & 0x0f;
		dData = dData + bDigit * prec;
		m_readPtr++;
	}

	if ( bSign == 13 ) dData *= -1.0;

	return dData;
}

long CMTParm2::ReadNum( int leng )
{
	double  dData = ReadNum( leng, 0 );

	return( (long) dData );
}

