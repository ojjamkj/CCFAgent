#ifndef     _INC_BRMDEBUG
#define     _INC_BRMDEBUG

#include	"BREXThread.h"
#include    "MRDTLIST.h"

#define		_MAX_BUFSIZE1		5000000
#define		_MAX_BUFSIZE2		50000
#define		_MAX_BUFSIZE3		50000

#define		_MAX_QRY_COUNT		500


/*   DEBUG STEP Definition    */
#define _DBG_STEP_ERR		-1

#define _DBG_STEP_LOAD		1		//  ������ Load
#define _DBG_STEP_CALL		2		//  �� ȣ��
#define _DBG_STEP_EOR		3		//  �� ����
#define _DBG_STEP_PARM		4		//  �� Parm���� Set
#define _DBG_STEP_RET		5		//  Return (line,Node)
#define _DBG_STEP_TERM		6		//  Debug����
#define _DBG_STEP_RCLEAR	7		//  �� �����̷� ����
#define _DBG_STEP_SETVAL	8		//  Global Variable �� ����
#define _DBG_STEP_GETVAL	9		//  Global Variable �� �Է� Dialog

#define _DBG_STEP_NEXT		11		//	Next Node positioning
#define _DBG_STEP_COND		12		//	���ǰ���� Set
#define _DBG_STEP_LOOP		13		//	LoopȽ�� Set
#define _DBG_STEP_QRY		14		//	������ Query Statement Set
#define _DBG_STEP_QRUN_CSR	15		//	Cursor Query ����
#define _DBG_STEP_QRUN_CUD	16		//	CUD    Query ����
#define _DBG_STEP_BRA		17		//  BranchName Set
#define _DBG_STEP_NRF		18		//  BranchName Not Found


/*   FILE NAME Definition    */
#define _FILE_DATA			"DATA.dat"
#define _FILE_PARM			"PARM.dat"


class   BRDebugger : public CBREXThreadrun, public CBREXThread
{
private:
	CMRDataList			*pCMRDataList;

	int					m_startRuleId;
	char				m_appDate[9];
	char				m_mode;

public:
    BRDebugger ( char, int, char * );
    ~BRDebugger();

	void			SuspendWork( int Step, int N1, int N2, char *buf );

	int				NextStep();

	virtual  void	Run( void );

private:
	TYPE_VALUE *	readRuleParmValues(int *parmCnt, char *sKey );

};

extern	BRDebugger	*m_pWorker;

#endif

