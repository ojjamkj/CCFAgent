#ifndef     _INC_MRCRPARS
#define     _INC_MRCRPARS

// general exception class of EILibrary
class   CMRCRParser
{
private:
	char	m_Message[128];
		
public:
    // constructors and destructor
    CMRCRParser();
    virtual ~CMRCRParser();

	void	getMessage( char *errorMessage );
	int     CalculateCondition( char *StmtStr );
	int		CalcResult( char *StmtStr, char retTypes[] );
	int		CalculateExpression( int opt, char *StmtStr );

private:
	int		CalcConditionNoIntr( char *StmtStr );
	int		CalcResultNoIntr( char *StmtStr, char retTypes[] );
	int		CalcExpressionNoIntr( int opt, char *StmtStr );
};

#endif
