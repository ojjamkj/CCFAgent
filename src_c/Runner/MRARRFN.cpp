#include    <string.h>
#include    <stdio.h>
#include    <stdlib.h>

#include    "MTExcept.h"
#include	"MTUtil.h"
#include    "MRSTRUCT.h"
#include    "MRARRFN.h"
#include	"MRTOKEN.h"
#include	"MRDTLIST.h"
#include	"MRERROR.h"

/*
#include    <time.h>
int getRandom()
{
	static int n =0;

	return ++n;
}
*/

/*

	m_rand = getRandom();

	printf("%6d CMRCInterpreter Start\n", m_rand );
*/


//	printf("%6d CMRCInterpreter End\n", m_rand );


//  	int				m_rand;



CMRArrayFn::CMRArrayFn( CMRDataList *pDataList )
{

	m_pDataList = pDataList;

	vType1 = 0;
	vCnt1 = 0;
	vGbn1 = 0;
	vPtr1 = 0;

	vType2 = 0;
	vCnt2 = 0;
	vGbn2 = 0;
	vPtr2 = 0;

	m_pMemPos = m_pDataList->getMemPtr2();
}

CMRArrayFn::~CMRArrayFn()
{
	m_pDataList->setMemPtr2( m_pMemPos );
}


void  CMRArrayFn::AddValue( int idx, TYPE_VALUE *tVal )
{

	if ( idx == 1 ) {
		vType1 = tVal->dataType;
		vGbn1  = 1;
		vCnt1 = 1;
		vPtr1 = (VALUE_UNION *) m_pDataList->MRMemAlloc2( sizeof(VALUE_UNION) );

		memcpy( (char *) vPtr1, (char *) &tVal->value, sizeof(VALUE_UNION) );
	} else {
		vType2 = tVal->dataType;
		vGbn2  = 1;
		vCnt2 = 1;
		vPtr2 = (VALUE_UNION *) m_pDataList->MRMemAlloc2( sizeof(VALUE_UNION) );

		memcpy( (char *) vPtr2, (char *) &tVal->value, sizeof(VALUE_UNION) );
	}
}


void  CMRArrayFn::AddItemList( int idx, int col )
{

	if ( idx == 1 ) {
		vType1 = m_pDataList->getItemType( col );
		vGbn1  = 0;
		vCnt1 = m_pDataList->getItemValueCount( col );
		vPtr1 = m_pDataList->getItemValuePtr( col );
	} else {
		vType2 = m_pDataList->getItemType( col );
		vGbn2  = 0;		
		vCnt2 = m_pDataList->getItemValueCount( col );
		vPtr2 = m_pDataList->getItemValuePtr( col );		
	}
}	


void  CMRArrayFn::AddRuleList( int idx, RULE_RESULT *ruleResult, int col )
{
	RESULT_REC *p;
	int   i;
	char  retType;

	p = ruleResult->resRecord;
	if ( col > 0 ) col--;

	retType = ruleResult->pRuleNode->retType[col];

	if ( idx == 1 ) {
		vType1 = retType;
		vGbn1  = 1;		
		vCnt1 = 0;
		
		while ( p ) {
			vCnt1++;
			p = p->pNext;
		}
		
		if ( vCnt1 == 0 ) return;
		
		
		vPtr1 = (VALUE_UNION *) m_pDataList->MRMemAlloc2( sizeof(VALUE_UNION) * vCnt1 );
		
		p = ruleResult->resRecord;
		i = 0;
		while ( p ) {
			memcpy( (char*) &vPtr1[i], (char*) &p->value[col], sizeof(VALUE_UNION) );
			i++;
			p = p->pNext;
		}
	} else {
		vType2 = retType;
		vGbn2  = 1;
		vCnt2 = 0;
		
		while ( p ) {
			vCnt2++;
			p = p->pNext;
		}
		
		if ( vCnt2 == 0 ) return;
		
		
		vPtr2 = (VALUE_UNION *) m_pDataList->MRMemAlloc2( sizeof(VALUE_UNION) * vCnt2 );

		p = ruleResult->resRecord;
		i = 0;
		while ( p ) {
			memcpy( (char*) &vPtr2[i], (char*) &p->value[col], sizeof(VALUE_UNION) );
			i++;
			p = p->pNext;
		}		
	}
}


bool    CMRArrayFn::isEqualToEach()
{
	if ( vCnt1 != vCnt2 ) return false;
	if ( vType1 != vType2 ) return false;
		
	for( int i=0; i<vCnt1; i++ ) {
		if ( ! compareValues( i, &vPtr2[i] ) ) return false;
	}
	
	return true;
}	


bool    CMRArrayFn::isInclude()
{
	if ( vType1 != vType2 ) return false;
	
	for( int i=0; i<vCnt1; i++ ) {
		for( int j=0; j<vCnt2; j++ )
			if ( compareValues( i, &vPtr2[j] ) ) return true;
	}
	
	return false;
}

bool    CMRArrayFn::isSubset()
{
	if ( vType1 != vType2 ) return false;
	
	int i, j;
	for( i=0; i<vCnt1; i++ ) {
		for( j=0; j<vCnt2; j++ )
			if ( compareValues( i, &vPtr2[j] ) ) break;

		if ( j == vCnt2 ) return false;
	}
	
	return true;
}


bool    CMRArrayFn::isIncludeList( CRULE_STMT *pTop )
{
	long   longVal2;
	double dblVal2;
	VALUE_UNION val;
	CRULE_STMT  *p;
//	if ( vType1 != vType2 ) return false;
	
	for ( int i=0; i<vCnt1; i++ ) {
		p = pTop;		
		while ( p ) {

			if ( vType1 == DATA_TYPE_DOUBLE && p->pNext->tokType == DATA_TYPE_LONG ) {
				p->pNext->tokType = DATA_TYPE_DOUBLE;
				p->pNext->uni.dblVal = (double) p->pNext->uni.longVal;
			}

			if (p->pNext->tokType != vType1) {
				if (p->pNext->tokType != 2) { //2=STRING ( val ) 넘겨온 데이타 ? 
					throw new CMTException(USERERR_GRAMMAR, EMSG_GRAMMAR_02);
				}else {
					getListValue(p->pNext, &val);
					switch (vType1) {
					case DATA_TYPE_LONG:
						longVal2 = atol((&val)->strVal);
						if (longVal2 == vPtr1[i].longVal) return true;
						break;
					case DATA_TYPE_DOUBLE:
						dblVal2 = atol((&val)->strVal);
						if (dblVal2 == vPtr1[i].dblVal) return true;
						break;
					}
				}
			}
			if ( getListValue( p->pNext, &val ) ) {
				if ( compareValues( i, &val ) ) return true;
			}
			
			p = p->pExpression;
		}
	}

	return false;
}

bool    CMRArrayFn::isSubsetList( CRULE_STMT *pTop )
{
	VALUE_UNION val;
	CRULE_STMT  *p;

//	if ( vType1 != vType2 ) return false;
	
	if(vCnt1==0) return false;

	for ( int i=0; i<vCnt1; i++ ) {
		p = pTop;		
		while ( p ) {
			if ( p->pNext->tokType != vType1 ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_03 );

			if ( getListValue( p->pNext, &val ) ) {
				if ( compareValues( i, &val ) ) break;
			}
			
			p = p->pExpression;
		}

		if ( ! p ) return false;
	}

	return true;
}



bool    CMRArrayFn::isEqualToList( CRULE_STMT *pTop )
{
	VALUE_UNION val;
	CRULE_STMT  *p, *q;
	bool		equal;
	int         i = 0;
	bool		toggle;
	
	p = pTop;
	while ( p ) {
		if ( i == vCnt1 ) return false;
		
		equal = false;
		toggle = 1;
		q = p->pNext;
		while ( q ) {
			if ( q->tokType == TOKENTYPE_CHOICE ) {
				if ( toggle ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_04 );
				toggle = 1;
				q = q->pNext;
				continue;
			}

			if ( ! toggle ) throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_05 );

			if ( ! getListValue( q, &val ) ) {
				throw new CMTException( USERERR_GRAMMAR, EMSG_GRAMMAR_06 );
			}

			equal = compareValues( i, &val );
			
			if ( equal ) break;
			
			q = q->pNext;
			toggle = 0;
		}
		
		if ( ! equal ) return false;
			
		p = p->pExpression;
		i++;
	}
	
	if ( i < vCnt1 ) return false;

	return true;
}
	

bool    CMRArrayFn::getListValue( CRULE_STMT *p, VALUE_UNION *pVal )
{
//	if ( ! p->tokInfo ) return false;
	
	memset( (char*) pVal, 0x00, sizeof( VALUE_UNION ) );
	
	switch ( p->tokType ) {
				case DATA_TYPE_STRING :
						strcpy( pVal->strVal, (char*) p->uni.tokInfo );
						break;
				case DATA_TYPE_DOUBLE :
						pVal->dblVal = p->uni.dblVal;
						break;
				case DATA_TYPE_LONG :
						pVal->longVal = p->uni.longVal;
						break;
				case DATA_TYPE_BOOL :
						pVal->boolVal = p->uni.boolVal;
				default :
						return false;
	}
	
	return true;
}	


bool    CMRArrayFn::compareValues( int idx, VALUE_UNION *pVal )
{
	bool  cmp;
	
	switch ( vType1 ) {
				case DATA_TYPE_STRING :
						McRTrim( pVal->strVal );
						McRTrim( vPtr1[idx].strVal );
						cmp = (bool) ! strcmp( pVal->strVal, vPtr1[idx].strVal );
						break;
				case DATA_TYPE_DOUBLE :
						cmp = (pVal->dblVal == vPtr1[idx].dblVal);
						break;
				case DATA_TYPE_LONG :
						cmp = (pVal->longVal == vPtr1[idx].longVal);
						break;
				case DATA_TYPE_BOOL :
						cmp = (pVal->boolVal == vPtr1[idx].boolVal);
				default :
						return false;
	}
	
	return cmp;
}


bool    CMRArrayFn::isMatch()
{
	if (vType1 != vType2) return false;

	for (int i = 0; i<vCnt1; i++) {
		for (int j = 0; j < vCnt2; j++) {
			if (matchValues(i, &vPtr2[j])) return true;
		}
	}

	return false;
}
bool    CMRArrayFn::matchValues(int idx, VALUE_UNION *pVal)
{
//	bool  cmp=false;
	bool b_TermFlag = false;;
	char *s1, *s2, *search, *t;
	char token[50];
	int i;
	bool b_Result = true;;;
	bool b_lastTokYn;
	s2 = pVal->strVal;
	s1 = vPtr1[idx].strVal;
	if (*s1 == NULL) b_Result = false;
	while (*s1) {
		switch (*s2) {
		case '_':
		case '?':
			s1++;
			s2++;
			break;
		case '%':
		case '*':
			s2++;
			while (*s2) {
				if (*s2 == '*' || *s2 == '?' || *s2 == '%'  || *s2 == '_' )
					s2++;
				else
					break;
			}

			if (!*s2) {
				b_TermFlag = true;
				break;
			}

			i = 0;
			while (*s2) {
				if (*s2 == '*' || *s2 == '?' || *s2 == '%' || *s2 == '_'  )
					break;
				token[i] = *s2;
				i++;
				s2++;
			}

			token[i] = 0;

			if (*s2)
				b_lastTokYn = 0;
			else
				b_lastTokYn = 1;

			search = strstr(s1, token);
			if (!search) {
				b_Result = false;
				b_TermFlag = true;
				break;
			}

			s1 = search + strlen(token);
			if (!b_lastTokYn)
				break;

			//  최종토큰일때 마지막을 Search한다.
			t = search + 1;
			while (*t) {
				search = strstr(t, token);
				if (!search)
					break;

				s1 = search + strlen(token);
				t = search + 1;
			}

			break;

		default:
			if (*s1++ != *s2++) {
				b_Result = false;
				b_TermFlag = true;
			}
		}

		if (b_TermFlag)
			break;
	}

	if  ( *s2 && ( *s2 != '*' &&  *s2 != '%') )
		b_Result = false;

	s1 = pVal->strVal;//
	s2 = vPtr1[idx].strVal;	
//	printf("[%d] [%d]==>s1[%s]s2[%s]==>[%s]\n",i, idx, vPtr1[idx].strVal,pVal->strVal,  b_Result == true ? "TRUE" : "FASLE");
	return b_Result;
}
bool    CMRArrayFn::isMatchList(CRULE_STMT *pTop)
{
	VALUE_UNION val;
	CRULE_STMT  *p;

	//	if ( vType1 != vType2 ) return false;

	for (int i = 0; i<vCnt1; i++) {
		p = pTop;
		while (p) {

			if (vType1 == DATA_TYPE_DOUBLE && p->pNext->tokType == DATA_TYPE_LONG) {
				p->pNext->tokType = DATA_TYPE_DOUBLE;
				p->pNext->uni.dblVal = (double)p->pNext->uni.longVal;
			}

			if (p->pNext->tokType != vType1) throw new CMTException(USERERR_GRAMMAR, EMSG_GRAMMAR_07);

			if (getListValue(p->pNext, &val)) {
//				printf("isMatchList call->matchValues i[%d] vCnt1[%d] \n", i, vCnt1);

				if (matchValues(i, &val)) return true;
			}

			p = p->pExpression;
		}
	}

	return false;
}
