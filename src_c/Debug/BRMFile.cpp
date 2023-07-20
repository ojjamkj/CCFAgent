#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>

#include	"BRMFile.h"
#include	"MTExcept.h"
#include	"MTUtil.h"

#define	END_DELIMETER	0
extern char         filepath[1024];
BRFRead::BRFRead( char *fName )
{
	char fPath[2048];
	sprintf(fPath, "%s%s", filepath, fName);
//	printf("BRFRead[%s]\n", fPath);

	if( ( fp=fopen(fPath,"r")) == 0 ) {
		char msg[200];
		sprintf( msg, "BRFRead:File open error fp[%d],(%s)",fp, fName );
		throw new CMTException( -1, msg );	
	}
}

BRFRead::~BRFRead()
{
	fclose( fp );
}


char* BRFRead::getString()
{
	char *p = m_Buffer;

	while ( ! feof(fp) ) {
		*p = fgetc(fp);
		if ( *p == END_DELIMETER ) break;

		if ( *p == EOF ) break;

		p++;
	}

	*p = 0;

	McRTrim(m_Buffer);

#ifdef _DEBUG
printf("    Read [%s]\n", m_Buffer );
#endif

	return m_Buffer;
}


int   BRFRead::getNumber()
{
	getString();

	return atoi( m_Buffer );
}

double   BRFRead::getDouble()
{
	getString();

	return atof( m_Buffer );
}



BRFWrite::BRFWrite( char *fName )
{
	char fPath[2048];
	 
	sprintf(fPath, "%s%s", filepath, fName);
//	printf("BRFWrite[%s]\n", fPath);
	if( ( fp=fopen(fPath,"w+")) == 0 ) {
		char msg[200];
		sprintf(msg, "BRFWrite:File open error fp[%d],(%s)", fp, fPath);
		throw new CMTException( -1, msg );	
	}
}


BRFWrite::~BRFWrite()
{
	fclose( fp );
}


int   BRFWrite::setString( char *val)
{
	fprintf(fp,"%s%c", val, END_DELIMETER );
	return 0;
}


int   BRFWrite::setNumber( int val )
{
	fprintf(fp,"%d%c", val, END_DELIMETER );
	return 0;
}

int   BRFWrite::setDouble( double val )
{
	fprintf(fp,"%f%c", val, END_DELIMETER );
	return 0;
}

void printLog( char *buf )
{
	FILE *fp;

	fp = fopen("BRMERR.txt","a+");

	fprintf(fp,"[%s]\n", buf );

	fclose(fp);
}

/*
int main( int argc, char *argv[])
{
	if ( *argv[1] == 'R' ) {
		BRFRead *pRead = new BRFRead("DAT.dat");

		printf("N = %d\n", pRead->getNumber() );
		printf("N = %d\n", pRead->getNumber() );
		printf("N = %d\n", pRead->getNumber() );
		printf("N = %d\n", pRead->getNumber() );
		printf("N = %d\n", pRead->getNumber() );

		printf("N = %d\n", pRead->getNumber() );
		printf("S = %s\n", pRead->getString() );
		printf("N = %d\n", pRead->getNumber() );
		printf("S = %s\n", pRead->getString() );

		delete pRead;

	} else {
		BRFWrite *pRead = new BRFWrite("DAT.dat");

		pRead->setNumber( 2 );
		pRead->setNumber( 3 );
		pRead->setNumber( 2 );
		pRead->setNumber( 5 );
		pRead->setNumber( 2 );
		pRead->setNumber( 1 );
		pRead->setString( "AAA" );
		pRead->setNumber( 1 );
		pRead->setString( "BBB" );

		delete pRead;
	}

	return 0;
}

*/