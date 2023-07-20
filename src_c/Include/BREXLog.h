#ifndef     _INC_BREXLOG
#define     _INC_BREXLOG

#include	"BREXThread.h"

#define     _MAX_QUEUE    5000
#define     _MSG_LENG      500

//  mode
#define		_LOG_LOG			1
#define		_LOG_ERR			2
#define		_LOG_DAT			3

#ifdef WIN32
	#define   PATH_DELIM   '\\'
#else
	#define   PATH_DELIM   '/'
#endif

typedef struct {
	unsigned char msgGbn;
	char	proctime[16];
	char	msg[_MSG_LENG+1];
} LOG_REC;


class CBREXLog  : public CBREXThreadrun, public CBREXThread
{
private:
	LOG_REC		*m_MsgRec;
	int			m_qSize;
	int			m_pop,  m_push;
	unsigned char	m_bLog2File;

	char		m_fName[200];
	char		m_lpszTime[16];

	void		WriteMessage();
	void		getCurrentTime( char* lpszTime );

public:
    CBREXLog();
    virtual ~CBREXLog();

	virtual void	Run( void );
	void		pushMessage( unsigned char msgGbn, bool bDispTime, const char* lpszLog );
	void		pushMessageIP( unsigned char msgGbn, const char* startip,bool bDispTime, const char* lpszLog );

};

extern	CBREXLog    *g_pCBREXLog;

void		_WriteLogNo( unsigned char msgGbn, const char* lpszLog );
void		_WriteLog( unsigned char msgGbn, const char* lpszLog );

void		_WriteLogNoIP( unsigned char msgGbn, const char* startip,const char* lpszLog );
void		_WriteLogIP( unsigned char msgGbn,const char* startip, const char* lpszLog );
#endif      //  _INC_BREXLOG


