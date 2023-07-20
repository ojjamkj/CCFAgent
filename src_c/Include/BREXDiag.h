#ifndef     _INC_BREXDIAG
#define     _INC_BREXDIAG

#include	"BREXThread.h"


class CBREXDiag  : public CBREXThreadrun, public CBREXThread
{
private:
	void		Statistics( void );
	void		Diagnose( void );
//	void		DiagnoseAgent( void );

public:
    CBREXDiag();
    virtual ~CBREXDiag();

	virtual void	Run( void );

};

extern  CBREXDiag  *g_pCBREXDiag;

#endif      //  _INC_BREXDIAG
