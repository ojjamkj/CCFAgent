#include    <string.h>
#include    <stdio.h>
#include    <stdlib.h>

#include    "MRERROR.h"
#include    "MRPARAM.h"

#ifdef      _PARSER
#include    "MREXPREP.h"
#else
#include    "MRRUNNER.h"
#include    "MREXPRES.h"
#endif

#ifdef		_TESTER
#include	"BRMStart.h"
#include	"BRMFile.h"
#endif

CMRParam::CMRParam(TYPE_VALUE *pParmVal, int cnt) {
	m_pParmVal = pParmVal;
	m_ParmCnt = cnt;
}

CMRParam::~CMRParam() {
}

bool CMRParam::getParmValue(int index, TYPE_VALUE *tVal) {
	if (index > m_ParmCnt) {
		char msg[200];
		sprintf(msg, "%s (%dth) :", EMSG_PARMNPASS, index);
		throw new CMTException(UERR_PARMNPASS, msg);
	}

	memcpy((char *) tVal, (char *) &m_pParmVal[index - 1], sizeof(TYPE_VALUE));

	return true;
}

#ifdef _TESTER
void CMRParam::DbgParmInfo()
{
	TYPE_VALUE *tVal;
	char buf[500];

	BRFWrite Writer( _FILE_PARM );

	Writer.setNumber( m_ParmCnt );

	for ( int i=0; i<m_ParmCnt; i++ ) {

		tVal = & m_pParmVal[i];

		switch( tVal->dataType ) {
			case DATA_TYPE_LONG :
			sprintf( buf, "%ld", tVal->value.longVal );
			break;
			case DATA_TYPE_STRING :
			strcpy( buf, tVal->value.strVal );
			break;
			case DATA_TYPE_DOUBLE :
			sprintf( buf, "%lf", tVal->value.dblVal );
			break;
			case DATA_TYPE_LSTRING :
			strcpy( buf, tVal->value.pLStr );
			break;
			case DATA_TYPE_BOOL :
			sprintf( buf, "%d", tVal->value.boolVal );
			default : break;
		}

		Writer.setString( buf );
	}
}

#endif
