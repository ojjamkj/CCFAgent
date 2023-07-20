// MTCicsTc.h
//
//////////////////////////////////////////////////////////////////////
// note: link socket library

#ifndef     _INC_MTCICSTC
#define     _INC_MTCICSTC


unsigned long   SOC_Open( char *lpszServerAddress, unsigned short ulPort );
void			SOC_Close( void );
unsigned long	SOC_Write( unsigned char* pBuffer, unsigned long ulSize );
unsigned long	SOC_Read( unsigned char* pBuffer, unsigned long ulSize );
char *			getSocketMessage( char *msg );


#endif

