#ifndef		_INC_MTPRFILE
#define		_INC_MTPRFILE

#include	<stdio.h>

class	CMTProfile
{
private:
#ifdef	WIN32		// for processing \r\n
	unsigned long	m_bReturnRead;
#endif	//WIN32
	unsigned char	m_bFileExist;
protected:
	char		*m_lpszFileName;
	FILE		*m_pFile;
	char		*m_pBuffer;
	long		m_lBufferSize;
	unsigned long	m_bEOFRead;

public:
	CMTProfile( const char* lpszFileName );
	virtual    ~CMTProfile();

public:
	long	ReadLong( const char* lpszSection,
	                  const char* lpszEntry, long lDef );
	const char*	ReadString(
		const char* lpszSection, const char*lpszEntry,
		const char* lpszDefault,
		char* lpszVal, long lLength, bool enc);
	const char*	WriteString(
		const char* lpszSection, const char*lpszEntry,
		const char* lpszDefault,
		char* lpszVal, long lLength, bool enc);
	unsigned char	FileOpenCheck( void );
	//modified by DSKIM 2016.11.23
	//void   enc(char* lpszVal);
	void  enc(char* lpszVal, char *lpszEncMethod);
	//modified by DSKIM 2016.11.23: AES256 Àû¿ë
	int GetIniKeyString(const char *title, const char *key,  char *buf);

	int PutIniKeyString(const char *title, const  char *key, char *val);

protected:
	unsigned long	GoSection( const char* lpszSection );
	unsigned long	GoEntry( const char* lpszEntry );
	unsigned long	GetValue( void );
	void		SkipSpace( void );
	void		SkipLine( void );
	int		Read( void );
	void		Unread();
	
};

#endif		// _INC_MTPRFILE

