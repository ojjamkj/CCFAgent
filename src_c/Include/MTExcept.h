#ifndef     _INC_MTEXCEPT
#define     _INC_MTEXCEPT

// general exception class of EILibrary
class   CMTException
{
protected:
    // data members
    long    m_lCode;
    char   *m_lpszMessage;

public:
    // constructors and destructor
    CMTException();
    CMTException( long lCode, const char* lpszMessage );
        // initialize code and message
    virtual ~CMTException();

    // attributes
    long            GetECode( void ) const;
    const char*     GetEMessage( void ) const;

    // operations
    void    Delete( void );         // delete this exception object
};

#endif
