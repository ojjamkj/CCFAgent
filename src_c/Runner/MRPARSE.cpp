#include  <stdlib.h>         /* For _MAX_PATH definition */
#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>

#include    "MRTOKEN.h"
#include    "MRPARSE.h"
#include    "MRGBLVAR.h"
#include  "MTUtil.h"
CMRParser::CMRParser()
{
}

CMRParser::~CMRParser()
{
}

void  CMRParser::Delete( CRULE_STMT *p )
{
	if ( ! p || p == (CRULE_STMT*) -1 ) return;

	Delete(p->pNext);
	Delete(p->pExpression);
    free( p );

}

CRULE_STMT * CMRParser::ParseExpression( char *StmtStr )
{
	CRULE_STMT *pStmt;
	CRULE_STMT *pLast, *pNew, *pPrev;
	CRULE_STMT *pTemp;
	char       *ExpressionStr;

	CMRParser  *Parsing;
	CMRToken   *CToken;
	TOKEN_RULE *Rule;
	if ( *StmtStr == 0 ) return (CRULE_STMT *) 0;
	Parsing = new CMRParser();
	CToken = new CMRToken(StmtStr);
	//printf("CMRParser::ParseExpression:START: [%s]\n ", StmtStr );
	pStmt = 0;
	pPrev = 0;
	m_funcLeft = 0;
	while ( pNew = CToken->getToken() ) {

		if ( (long) pNew == -1 ) {
			m_lCode = CToken->GetECode();
			if ( pStmt ) Delete( pStmt );

			delete Parsing;
			delete CToken;

			return (CRULE_STMT *) -1;
		}

        if ( ! pStmt ) {
            pStmt = pLast = pNew;
        } else {
            pLast->pNext = pNew;

			pLast = pLast->pNext;
        }
		

		Rule = (TOKEN_RULE*) pNew->uni.tokInfo;

	    pLast->pNext = 0;


		//  Rule,Item의 Index expression 처리
		if ( pPrev && pNew->tokType == TOKENTYPE_LEFTP ) {
		//	printf("CMRParser::ParseExpression:02: [%s]\n ", StmtStr);
			if ( pPrev->tokType == TOKENTYPE_RULE ||		pPrev->tokType == TOKENTYPE_GLOBAL ) {
			//	printf("CMRParser::ParseExpression:03: [%s]\n ", StmtStr);
				ExpressionStr = CToken->getParenExpression();

				if ( ! ExpressionStr ) {
					m_lCode = CToken->GetECode();
					if ( pStmt ) Delete( pStmt );

					delete Parsing;
					delete CToken;

					return (CRULE_STMT *) -1;
				}


				free( pNew );
				pNew = pLast = pPrev;    pNew->pNext = 0;
		//		printf("CMRParser::ParseExpression:Call: \n");
				if ( (pTemp = Parsing->ParseExpression( ExpressionStr )) == (CRULE_STMT *) -1) {
					m_lCode = CToken->GetECode();
				if ( pStmt ) Delete( pStmt );

					delete Parsing;
					delete CToken;

					return (CRULE_STMT *) -1;
				
				}

				if ( pPrev->tokType == TOKENTYPE_RULE ) {
			//		printf("CMRParser::ParseExpression:04: [%s]\n ", StmtStr);
					if ( ! pNew->pExpression ) {
						pNew->pExpression = (CRULE_STMT *) pCMRGblVars->MetisAlloc( sizeof(CRULE_STMT) );
						pNew->pExpression->pExpression = 0;
					}

					pNew->pExpression->pNext = pTemp;
				} else   //  ITEM일때는 IndexExpression이 바로 붙는다.
					pNew->pExpression = pTemp;
			}
		} else if ( pNew->tokType == TOKENTYPE_RULE && Rule->parmInfo ) {  // 룰의 Parm처리
		//	printf("CMRParser::ParseExpression:05: [%s]\n ", StmtStr);
//if ( Rule->ruleId == 24537 ) {
//   printf("Pars 24537\n");
//}
			pNew->pExpression = (CRULE_STMT *) pCMRGblVars->MetisAlloc( sizeof(CRULE_STMT) );
			pNew->pExpression->pNext = 0;
		if ((pNew->pExpression->pExpression = Parsing->ParseResult( Rule->parmInfo )) == (CRULE_STMT *) -1) {
				pNew->pExpression->pExpression = 0;
				m_lCode = CToken->GetECode();
				if ( pStmt ) Delete( pStmt );

				delete Parsing;
				delete CToken;
	
				return (CRULE_STMT *) -1;
			}
		} else if ( pNew->tokType > TOKENFUNC_END ||
				pNew->tokType >= TOKENTYPE_IN &&
				pNew->tokType <= TOKENTYPE_MATCH ) {
		//	printf("CMRParser::ParseExpression:06: [%s]\n ", StmtStr);
			ExpressionStr = CToken->getParenExpression();

			if ( (pNew->pExpression = Parsing->ParseResult( ExpressionStr )) == (CRULE_STMT *) -1) {
				pNew->pExpression = 0;
				m_lCode = CToken->GetECode();
				if ( pStmt ) Delete( pStmt );

				delete Parsing;
				delete CToken;

				return (CRULE_STMT *) -1;
			}
		}

		pPrev = pNew;
	}
	delete CToken;
	delete Parsing;
	//	printf("CMRParser::ParseExpression:END: [%s]\n ", StmtStr);

	return pStmt;
}


CRULE_STMT * CMRParser::ParseResult( char *StmtStr )
{
	CRULE_STMT *pStmt;
	CRULE_STMT *pLast, *pNew;
	char       *nextStr;
	//printf("CMRParser::ParseResult:START:\n");
	pStmt = 0;

	if ( ! StmtStr ) return (CRULE_STMT *) -1;

	while ( *StmtStr ) {
	//	printf("	CMRParser::ParseResult:01: [%s]\n ", StmtStr);
		nextStr = getExpression( StmtStr );  //   , 로 분리
//printf("stmt = !!%s!!   ", StmtStr );
	//	printf("	CMRParser::ParseResult:02: [%s] [%s]\n ", StmtStr, nextStr);

		pNew = ParseExpression( StmtStr );
		/*
		if (pNew) {
		 
			printf("	CMRParser::ParseResult:tokInfo:01: [%s] \n ", ((char*)pNew->uni.tokInfo));
			if ( pNew->pNext) {
				printf("	CMRParser::ParseResult:tokInfo:02: [%c] \n ", ((CRULE_STMT*)pNew->pNext)->tokType);
				printf("	CMRParser::ParseResult:tokInfo:03: [%s] \n ", (char*)((CRULE_STMT*)pNew->pNext)->uni.tokInfo);
			}
			if (  pNew->pExpression) {
				printf("	CMRParser::ParseResult:tokInfo:04: [%s] \n ", (char*)((CRULE_STMT*)pNew->pExpression)->uni.tokInfo);
			}
		}*/
//		printf("	CMRParser::ParseResult:03: [%s] [%s]\n ", StmtStr, pNew);

//printf("pNew = %d\n", pNew );

		if ( (long) pNew == -1 ) {
			if ( pStmt ) Delete( pStmt );
	//		printf("	CMRParser::ParseResult:return:\n");
			return (CRULE_STMT *) -1;
		}
			
        if ( ! pStmt ) {
			pStmt = pLast = (CRULE_STMT *) pCMRGblVars->MetisAlloc( sizeof(CRULE_STMT) );
        } else {
            pLast->pExpression = (CRULE_STMT *) pCMRGblVars->MetisAlloc( sizeof(CRULE_STMT) );
			pLast = pLast->pExpression;
        }

		pLast->tokType = 0;
        pLast->pNext = pNew;
	    pLast->pExpression = 0;
		
		StmtStr = nextStr;
		
	}
	return pStmt;
}

char * CMRParser::getExpression( char *StmtStr )
{
	int   pCnt = 0;
	int   bCnt = 0;
	char *p;
	int   QuoteOpen = 0;
	
	p = StmtStr;

	while ( *p ) {
		if ( QuoteOpen ) {
			if ( *p++ == '\"' ) QuoteOpen = 0;

			continue;
		}

		switch ( *p ) {
			case '{'  : bCnt++; 	break;
			case '}'  : bCnt--;     break;
			case '('  : pCnt++;     break;
			case ')'  : pCnt--;     break;
			case '\"' : QuoteOpen = 1;  break;
			case ','  : if( !bCnt ) {
							if ( pCnt == 0 ) {
								*p++ = 0;
								return p;
							}
						} else 
							break;
							

			default   : break;
		}
		
		p++;
	}

	return p;
}
