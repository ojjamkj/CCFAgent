#ifndef     _INC_MTLOG
#define     _INC_MTLOG

#include    "MTSysMsg.h"

#define		_LOGBUFFERSIZE	4096

void    InitLogInfo( char, char * );
void    WriteLog( long, const char* );
void    WriteLogNo( long, const char* );

class   CMessage 
{
private:
	unsigned char	m_STDOUT;
	char			m_Buffer[_LOGBUFFERSIZE];
	int				m_Pos;

public:
    CMessage ( int mode );
    ~CMessage();

	void	AddMessage( char *msg );
	void	WriteMessage( long mode=1 );
};

#endif      //  _INC_MTLOG


