#ifndef   __INC_MTCONVT
#define   __INC_MTCONVT

#define    SO           ( 0x0E )
#define    SI           ( 0x0F )
#define    EBC_SPC      ( 0x40 )
#define    ASC_SPC      ( 0x20 )

#define    HI_BYTE(s)    ( (char)(((s) >> 8) & 0xff) )
#define    LO_BYTE(s)    ( (char)(s) )

#define    SOSI_NO      ( 0 )
#define    SOSI_INS     ( 1 )
#define    SOSI_UPD     ( 2 )
#define    SOSI_DEL     ( 3 )


#define    ENGLISH      ( 0 )
#define    HANGUL       ( 1 )

#ifndef    TRUE
#define    TRUE         ( 1 )
#define    FALSE        ( 0 )
#endif

    int ConvA2E(  int Pi_ConvMode,
                             unsigned char *Pcp_StrSrc,
                             int Pi_SrcSize,
                             unsigned char *Pcp_StrDest,
                             int Pi_DestSize
                           );
    int ConvE2A(  int Pi_ConvMode,
                             unsigned char *Pcp_StrSrc,
                             int Pi_SrcSize,
                             unsigned char *Pcp_StrDest,
                             int Pi_DestSize
                           );
	float  ReorderBytesFloat( float* pfValue );
	double ReorderBytesDouble( double* pdblValue );
	int    ReorderBytesInt( int* plValue );
	long   ReorderBytesLong( long* plValue );
	short  ReorderBytesShort( short* psValue );

#endif    //__INC_MTCONVT
