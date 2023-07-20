#include    <string.h>
#include    <stdio.h>
#include    <stdlib.h>

#include    "MTExcept.h"
#include    "MRSTRUCT.h"
#include	"MRERROR.h"
#include	"MTUtil.h"
#include	"MRDTLIST.h"
#include    "BRTrace.h"

CBRTrace *g_pCBRTrace;

CBRTrace::CBRTrace() {
	m_pTrcRoot = 0;
}

CBRTrace::~CBRTrace() {
}

/*
 void  CBRTrace::buildTraceInfo( CMTParm *ObjBuffer )
 {
 TRC_PGM  *pTrcNew;
 TRC_ITEM *pItem;
 int  leng;
 char tempStr[500];

 pTrcNew = (TRC_PGM *) malloc( sizeof(TRC_PGM) );
 if ( ! pTrcNew ) {
 throw new CMTException( SYSERR_MEMORYALLOCATEFAILURE,
 ERRMSG_MEMORYALLOCATEFAILURE );
 }

 pTrcNew->pNext = m_pTrcRoot;
 m_pTrcRoot = pTrcNew;

 ObjBuffer->ReadString( pTrcNew->PgmId );

 pTrcNew->itemCnt = (short) ObjBuffer->ReadByte();

 if ( pTrcNew->itemCnt == 0 ) return;


 pItem = (TRC_ITEM *) malloc( sizeof(TRC_ITEM) * pTrcNew->itemCnt );
 if ( ! pTrcNew->pItem ) {
 throw new CMTException( SYSERR_MEMORYALLOCATEFAILURE,
 ERRMSG_MEMORYALLOCATEFAILURE );
 }

 pTrcNew->pItem = pItem;

 for ( int i=0; i<pTrcNew->itemCnt; i++ ) {
 pItem[i].itemCode = ObjBuffer->ReadLong();

 ObjBuffer->ReadString(tempStr);
 McRTrim(tempStr);
 leng = strlen( tempStr );
 if ( leng > STR_VALUE_SIZE ) {
 leng = STR_VALUE_SIZE;
 tempStr[leng] = 0;
 }

 strcpy( pItem[i].strVal, tempStr );
 }
 }
 */

 /*
 int  CBRTrace::AddTraceInfo(  char *dt, char *pgmId )
 {
 TRC_PGM  *pTrcNew;
 TRC_ITEM *pItem;
 int  leng;
 char tempStr[500];

 pTrcNew = (TRC_PGM *) malloc( sizeof(TRC_PGM) );
 if ( ! pTrcNew ) {
 throw new CMTException( SYSERR_MEMORYALLOCATEFAILURE,
 ERRMSG_MEMORYALLOCATEFAILURE );
 }

 pTrcNew->pNext = m_pTrcRoot;
 m_pTrcRoot = pTrcNew;

 sprintf( pTrcNew->TraceId,"%s_%s", pgmId, dt );
 strcpy( pTrcNew->PgmId, pgmId );


 pTrcNew->itemCnt = (short) ObjBuffer->ReadByte();

 if ( pTrcNew->itemCnt == 0 ) {
 pTrcNew->pItem = 0;
 return 0;
 }


 pItem = (TRC_ITEM *) malloc( sizeof(TRC_ITEM) * pTrcNew->itemCnt );
 if ( ! pItem ) {
 throw new CMTException( SYSERR_MEMORYALLOCATEFAILURE,
 ERRMSG_MEMORYALLOCATEFAILURE );
 }

 pTrcNew->pItem = pItem;

 for ( int i=0; i<pTrcNew->itemCnt; i++ ) {
 pItem[i].itemCode = ObjBuffer->ReadLong();

 ObjBuffer->ReadString(tempStr);
 McRTrim(tempStr);
 leng = strlen( tempStr );
 if ( leng > STR_VALUE_SIZE ) {
 leng = STR_VALUE_SIZE;
 tempStr[leng] = 0;
 }

 strcpy( pItem[i].strVal, tempStr );
 }

 return 0;
 }
 
 */
void CBRTrace::DelTraceInfo(char *dt, char *pgmId) {
	TRC_PGM *pNode, *pPrev;
	char ID[40];

	sprintf(ID, "%s_%s", pgmId, dt);

	pNode = m_pTrcRoot;
	pPrev = 0;

	while (pNode) {
		if (strcmp(pNode->TraceId, ID) == 0)
			break;

		pPrev = pNode;
		pNode = pNode->pNext;
	}

	if (!pNode)
		return;

	if (pPrev) {
		pPrev->pNext = pNode->pNext;
	} else {
		m_pTrcRoot = pNode->pNext;
	}

	free(pNode->pItem);
	free(pNode);
}

void CBRTrace::setTraceLog(CBRMObj *m_ObjBuffer) {
		int logOnOff;
		logOnOff= m_ObjBuffer->ReadLong();
		configRec.usTrace=(short)logOnOff;
		m_ObjBuffer->Clear1();
		m_ObjBuffer->WriteLong(0);  
		m_ObjBuffer->WriteLong(99);
		
}

bool CBRTrace::isTrace(char *PgmId, void *p, char *dt) {
	TRC_PGM *pNode, *pPrev;
	TYPE_VALUE tVal;

	unsigned char bEqual;
	CMRDataList *pData = (CMRDataList*) p;
	
	if( pData ==NULL ){
	   printf("isTreace pData=NULL \n");
	   return false;
	}
	if(  pData->m_TraceID==NULL ){
	   printf("isTreace pData->m_TraceID=NULL \n");
	   return false;
	}
	if (configRec.usTrace) {
		if (configRec.usAllTracle) {
			strcpy(pData->m_TraceID,"00000000");
//			strcpy(dt, "00000000");
			dt[8] = 0;
		}else{
//			strcpy(pData->m_TraceID,"00000000");
		  DetailTimeStamp( pData->m_TraceID );
		}
		return true;
	}

	pNode = m_pTrcRoot;
	pPrev = 0;

	while (pNode) {
		if (strcmp(pNode->PgmId, PgmId) != 0) {
			pPrev = pNode;
			pNode = pNode->pNext;
			continue;
		}

		bEqual = true;

		for (int i = 0; i < pNode->itemCnt; i++) {

			bEqual = pData->getItemValueByID(pNode->pItem[i].itemCode, &tVal);
			if (!bEqual)
				break;

			switch (tVal.dataType) {
			case DATA_TYPE_LONG:
				bEqual = tVal.value.longVal == atol(pNode->pItem[i].strVal);
				break;
			case DATA_TYPE_STRING:
				bEqual = !strcmp(tVal.value.strVal, pNode->pItem[i].strVal);
				break;
				break;
			case DATA_TYPE_DOUBLE:
				bEqual = tVal.value.dblVal == atof(pNode->pItem[i].strVal);
			   	break;
			case DATA_TYPE_BOOL:
				bEqual = tVal.value.boolVal == atol(pNode->pItem[i].strVal);
				break;
			default:
				bEqual = false;
				break;
			}

			if (!bEqual)
				break;
		}

		if (bEqual) {
			strcpy(dt, pNode->TraceId);

			if (pPrev) {
				pPrev->pNext = pNode->pNext;
				free(pNode);
			} else {
				m_pTrcRoot = pNode->pNext;
				free(pNode);
			}

			return true;
		}

		pPrev = pNode;
		pNode = pNode->pNext;
	}

	return false;
}

