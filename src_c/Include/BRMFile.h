#ifndef     _INC_BRMFILE
#define     _INC_BRMFILE

class   BRFRead
{
private:
	FILE				*fp;

	char				m_Buffer[10001];

public:
	BRFRead ( char *fName );
	~BRFRead();

	char*		getString();
	int			getNumber();
	double		getDouble();
};

class   BRFWrite
{
private:
	FILE				*fp;

public:
	BRFWrite ( char *fName );
	~BRFWrite();

	int			setString( char *);
	int			setNumber( int );
	int			setDouble( double );
};

#endif

