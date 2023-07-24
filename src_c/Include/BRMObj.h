#ifndef     __INC_BRMOBJ
#define     __INC_BRMOBJ
#include "MItem.h"
#define  OBJ_INI_LENGTH    2048000 //1024000  //40960

#define  LONG_TYPE_SIZE     4
#define  PACK_TYPE_SIZE    10

class   CBRMObj
{
private:
	int       m_mode;
    long      m_readPtr;
    long      m_length;
    long      m_allocLength;
	char      m_tempstr[1000+1];//STR_VALUE_SIZE

    long      m_bConv;        //   0:  None
                              //   1: (Write "A2E", Read "E2A")
                              //   2: (Write "E2A", Read "A2E")
public:
	unsigned char     *m_Buffer;
    
protected:        

public:
    // constructors and destructor
	CBRMObj();
	CBRMObj( long maxSize, char *conv="" );
	CBRMObj( char *pBuffer, long maxSize, char *conv="" );
	virtual ~CBRMObj();

	void  Clear();
	void  Clear2(long leng );
	void  Clear1();
    void  Free();
	long  getLength();
	long  setLength(long Leng);
	void  setMaxLength( long Leng );
//	long  setDataLength(long newLength);
	void  setMaxPramSize( int maxSize );
	unsigned char * getBuffer();
	void  WriteDataLength();
	long  ReadDataLength();

//    void  Clear( long leng);

    // operations
	int   WriteByte( unsigned char );
    int   WriteLong( long );
//	void  WriteLongPosition( long lVal, int pos );
    int   WriteString( char* );
    int   WriteLongString(char* lpsz);
    int   WriteDouble( double );
    int   WritePack( double dbl, int leng, int m );
	int   WriteByteFile(char* lpsz, int size, int first, long totsize);
    unsigned char  ReadByte();
    long   ReadLong();
    long   ReadPString( char** );
    long   ReadString( char* );
    long   ReadNString( char*, int );     
    double ReadDouble();
	double ReadPack( int leng, int m );
	long   ReadByteFile(unsigned char* str, long length);
private:
    long  BufSizeCheck( long leng );
    long  LastReadCheck( long leng );

};

#endif    //   __INC_BRMOBJ
