#include  <stdlib.h>         /* For _MAX_PATH definition */
#include  <stdio.h>
#include  <string.h>

#ifndef   _OS390
#include  <malloc.h>
#endif 

#ifdef WIN32
#include <direct.h>
#endif
#include  "BRLine.h"
#include  "MRDEFINE.h"
#include  "MRERROR.h"
#include  "MRTOKEN.h"
#include  "MTUtil.h"
#include  "MRFUNCTN.h"
#include  "MTSysMsg.h"

#ifdef _TESTER
#include  "MRGBLVAR.h"
#include  "BRMFile.h"
#include  "BRMStart.h"
#else
#include  "MRGBLVAR.h"
#include  "MRConfig.h"
#include  "MRSTARES.h"
#endif

#include  "MRDTLIST.h"
#include  "BRTrace.h"

#ifdef  _RUNNER
#include  "BREXLog.h"
#endif

#define   _MEMORY_MGR

#ifdef _MSG_KO
#define   MSG_LOGINS_ERR         "·Î±× »ðÀÔ ¿À·ù : %s ( %d )\n"
#define   MSG_ITEM_NOTPASS_DEC   "¾ÆÀÌÅÛ [%d] °ªÀÌ ¸ÂÁö ¾Ê½À´Ï´Ù."
#define   MSG_ITEM_NOTPASS_STR   "¾ÆÀÌÅÛ[%s] °ªÀÌ ¸ÂÁö ¾Ê½À´Ï´Ù."
#define   MSG_NOTDEFINE_RET      "Á¤ÀÇµÇÁö ¾ÊÀº ¹ÝÈ¯°ªÀÌ »ç¿ëµÇ¾ú½À´Ï´Ù{%d::%d}."

#define   MSG_NOTEXIST_TRACEOUTPATH   "Trace out °æ·Î°¡ Á¸ÀçÇÏÁö ¾Ê½À´Ï´Ù.\n"
#else
#define   MSG_LOGINS_ERR         "Log Insert error : %s ( %d )\n"
#define   MSG_ITEM_NOTPASS_DEC   "Item[%d] value not passed"
#define   MSG_ITEM_NOTPASS_STR   "Item[%s] value not passed"
#define   MSG_NOTDEFINE_RET      "Used the returns {%d::%d} which is not defined."

#define   MSG_NOTEXIST_TRACEOUTPATH   "Trace out path not exist.\n"
#endif
 
CMRDataList::CMRDataList(unsigned long thrNo, void *ctx, int size1, int size2,
        int size3) {
    m_ctxQuery = ctx;
    m_DBName[0] = 0;
    m_MaxMemSize1 = size1;
    m_MaxMemSize2 = size2;
    m_MaxMemSize3 = size3;

    m_bValidItems = false;

#ifdef  _TRACE
    m_TraceFlag = false;
    memset(m_TraceMsg,0x00,sizeof(m_TraceMsg));
    memset(m_TraceID,0x00,sizeof(m_TraceID));

#endif

    m_ulThreadNo = thrNo;

//  printf("Size : %d %d %d\n", m_MaxMemSize1, m_MaxMemSize2, m_MaxMemSize3 );
    pMemory1 = (char *) malloc(m_MaxMemSize1);
    if (!pMemory1)
        throw new CMTException(SYSERR_MEMORYALLOCATEFAILURE,
                ERRMSG_MEMORYALLOCATEFAILURE_07);

    pMemory2 = (char *) malloc(m_MaxMemSize2);
    if (!pMemory2)
        throw new CMTException(SYSERR_MEMORYALLOCATEFAILURE,
                ERRMSG_MEMORYALLOCATEFAILURE_08);

#ifdef _MEMORY_MGR

    pMemory3 = (char *) malloc(m_MaxMemSize3);
    if (!pMemory3)
        throw new CMTException(SYSERR_MEMORYALLOCATEFAILURE,
                ERRMSG_MEMORYALLOCATEFAILURE_09);
#endif
	 memset(m_StartIP,0x00,sizeof(m_StartIP));
	 memset(m_PgmId,0x00,sizeof(m_PgmId));

}
 
CMRDataList::~CMRDataList() {
    if (pMemory1)
        free (pMemory1);
    if (pMemory2)
        free (pMemory2);

#ifdef _MEMORY_MGR
    if (pMemory3)
        free (pMemory3);
#endif

//  free( m_pErrorResult );

}



#ifdef _TESTER
void CMRDataList::Initialize( int ruleId )
{
    m_pPgmNode = 0;
    strcpy( m_PgmId, "Debugger" );
    m_RuleId = 0;

    m_bExit = 0;

    m_pRuleResult = 0;
    m_pGVList = 0;

    m_pErrorResult = 0;

    m_lRc = 0;
    m_lCode = 0;

    m_Pos1 = 0;
    m_Pos2 = 0;
    m_Pos3 = 0;

    m_HAHA[0] = 0;
    m_HAHA_rNode = 0;

    m_usedRuleClear = 0;

    m_cursorRuleId = 0;

    pStartRuleNode = 0; //  pCMRGblVars->DbgLoadRule( ruleId );
    m_ParmMode = 0;

    //  DEBUGGER¿¡¼­´Â ITEMÁ¤º¸°¡ ¿©·¯¹ø¿¡ ³ª´©¾î µé¾î¿ÈÀ¸·Î ±¸Á¶Ã¼¸¦ ¹Ì¸® ¸¸µé¾îµÐ´Ù.
    m_itemCnt = 0;
    m_itemValue = (ITEM_VALUE *) MRMemAlloc1( sizeof(ITEM_VALUE) * 100 );

    for ( int i=0; i<100; i++ ) {
        m_itemValue[i].mValue = 0;
        m_itemValue[i].itemCode = 0;
        m_itemValue[i].itemType = 0;
        m_itemValue[i].itemOcc = 0;
    }
}

#else

void CMRDataList::Initialize(PGM_LIST *pPgm, char *PgmId) {
    m_pPgmNode = pPgm;
    strcpy(m_PgmId, PgmId);
    m_RuleId = 0;

    m_bExit = 0;

    m_pRuleResult = 0;
    m_itemValue = 0;
    m_pGVList = 0;

    m_pErrorResult = 0;

    m_lRc = 0;
    m_lCode = 0;

    m_Pos1 = 0;
    m_Pos2 = 0;
    m_Pos3 = 0;

//  m_pReturnList = 0;

    m_HAHA[0] = 0;
    m_HAHA_rNode = 0;

    m_usedRuleClear = 0;

    m_cursorRuleId = 0;

    pStartRuleNode = 0;

#ifdef _RUNNER
//AD2001.	printf("MRDTLIST-Initialize-001  [%d]\n", pPgm->startRuleId);
    pStartRuleNode = pCMRGblVars->getRuleNode( pPgm->startRuleId );
	//AD2001.	printf("MRDTLIST-Initialize-002  [%d]\n", pPgm->startRuleId);
#endif
}

#endif

long CMRDataList::GetECode(void) const {
    return m_lCode;
}

const char* CMRDataList::GetEMessage(void) const {
    return m_lpszMessage;
}

void CMRDataList::DeleteStructure(void) {
    m_Pos1 = 0;
    m_Pos2 = 0;
    m_Pos3 = 0;
}

void CMRDataList::DeleteResRec(RESULT_REC *p) {
    if (!p)
        return;

    DeleteResRec(p->pNext);

    if (p->value)
        MRMemFree(p->value);

    MRMemFree(p);

}

void CMRDataList::DelRuleResult(RULE_RESULT *p) {
    p->ruleId = 0;
}

void CMRDataList::DelRuleResultAll(long id) {
    RULE_RESULT *pSelected, *pPrev;

// printf("DelRuleResultAll : %d\n", id );

    m_usedRuleClear = 1;

    if (id == 0) {

#ifdef _RUNNER
        g_CMRStaticResult.DelRuleResultAll();
#endif

        m_pRuleResult = 0;
        return;
    }

#ifdef _RUNNER
    if ( pCMRGblVars->getRuleScope( id ) ) g_CMRStaticResult.DelRuleResult( id );
#endif

    pSelected = m_pRuleResult;
    pPrev = 0;
    while (pSelected) {
        if (id == pSelected->ruleId) {
            if (pPrev)
                pPrev->pNext = pSelected->pNext;
            else
                m_pRuleResult = pSelected->pNext;
        } else
            pPrev = pSelected;

        pSelected = pSelected->pNext;
    }
}

#ifdef _TESTER
void CMRDataList::setAppDate( char *appDate )
{
    if ( strlen(appDate) == 8 )
    	strcpy( m_app_dt, appDate );
    else
    	CurrentDate( m_app_dt, "" );
 
}

#else

void CMRDataList::setAppDate() {
    int i;

//#ifdef DEBUG
	if (m_pPgmNode == NULL ) printf("m_pPgmNode null \n");
	if (m_pPgmNode != NULL && m_pPgmNode->app_dt_id == NULL) printf("m_pPgmNode->app_dt_id NULL \n");
	if (m_pPgmNode != NULL && m_pPgmNode->app_dt_id != NULL) printf("m_pPgmNode->app_dt_id  [%d] \n", m_pPgmNode->app_dt_id);
	if (m_pPgmNode == NULL) {
#ifdef _BRMLIB
		CurrentDate(m_app_dt, "");
#else
		CurrentDateTime(m_app_dt, "", configRec.usRuleApplyTime);
#endif
		return;
	}
	if (m_pPgmNode == NULL || m_pPgmNode->app_dt_id == NULL) {
#ifdef _BRMLIB
		CurrentDate(m_app_dt, "");
#else
		CurrentDateTime(m_app_dt, "", configRec.usRuleApplyTime);
#endif		return;
	}
	//#endif
	printf("setAppDate 100 \n");
	if (m_pPgmNode->app_dt_id > 0) {
        for (i = 0; i < m_itemCnt; i++) {
            if (m_itemValue[i].itemCode == m_pPgmNode->app_dt_id)
                break;
        }

        if (i < m_itemCnt) {
            if (m_itemValue[i].itemType != DATA_TYPE_STRING)
                throw new CMTException(UERR_DATETYPEERR, EMSG_DATETYPEERR);

            char *s = m_itemValue[i].mValue[0].strVal;
            int len = strlen(s);

            if (len > 0) {
                if (len == 8) {
                    strcpy(m_app_dt, s);
                } else if (len == 10) {
                    strncpy(m_app_dt, s, 4);
                    strncpy(m_app_dt + 4, s + 5, 2);
                    strncpy(m_app_dt + 6, s + 8, 2);
                    m_app_dt[8] = 0;
                } else
                    throw new CMTException(UERR_DATETYPEERR, EMSG_DATETYPEERR);

                if (!DateTypeCheck(m_app_dt))
                    throw new CMTException(UERR_DATETYPEERR, EMSG_DATETYPEERR);

                return;
            }
        }
    }

    //  Àû¿ëÀÏ ItemValue¸¦ Ã£Áö ¸øÇÏ¸é ÇöÀçÀÏ

#ifdef _BRMLIB
    CurrentDate(m_app_dt, "");
#else
    CurrentDateTime(m_app_dt, "",configRec.usRuleApplyTime);
#endif
	printf("setAppDate 200 \n");
}
#endif




void * CMRDataList::getVersionNode(RULE_LIST *pRule, int *rows) {
    RULE_VERS *p = pRule->pVers;

#ifdef _TESTER
  while ( p&& p->pNext) {
		#ifdef _DEBUG
//        printf("  ApplyDate 001 [%s]  Start[%s] End[%s]  rule_data_ptr[%d][%d][%d].\n",m_app_dt, p->stDate, p->endDate, p ,p->pNext,p->pPrev);
		#endif
    p = p->pNext;
  }
  while( p ){
 //       printf("  ApplyDate 002 [%s]  Start[%s] End[%s]  rule_data_ptr[%d][%d][%d].\n",m_app_dt, p->stDate, p->endDate, p ,p->pNext,p->pPrev);
        if (strcmp(m_app_dt, p->stDate) >= 0) {
            if (rows > 0)
                *rows = p->rows;

            if (p->endDate[0] == 0)
                return p->rule_data;
            if (strcmp(m_app_dt, p->endDate) <= 0)
                return p->rule_data;
        }
	 	p=p->pPrev;
  }
 

#else	 
  while (p) {
//  #ifdef _DEBUG
//        printf("  ApplyDate[%s]  Start[%s] End[%s]  rule_data_ptr[%d].\n",m_app_dt, p->stDate, p->endDate, p->rule_data );
//  #endif

        if (strcmp(m_app_dt, p->stDate) >= 0) {
            if (rows > 0)
                *rows = p->rows;

            if (p->endDate[0] == 0)
                return p->rule_data;
            if (strcmp(m_app_dt, p->endDate) <= 0)
                return p->rule_data;
        }

        p = p->pNext;
    }
#endif

  #ifdef _TRACE
    if ( m_TraceFlag )
    {
        if( !p ){  //  p  = NULL ÀÎ°æ¿ì
            sprintf(m_TraceMsg, "R(%d) NO VERSION (ApplyDate[%s] Run Error)", pRule->rule_id, m_app_dt );
            TraceOut( m_TraceMsg );
        }else{
            sprintf(m_TraceMsg, "R(%d) NO VERSION (ApplyDate[%s]  Start[%s] End[%s])", pRule->rule_id, m_app_dt, p->stDate, p->endDate );
            TraceOut( m_TraceMsg );
        }
    }
  #endif
    return 0;
}

#ifdef _TESTER

int CMRDataList::setItemValues()
{
    int i, leng;
    int pos, curCnt;
    int l_itemOcc;
    char tempStr[300];

    BRFRead Reader( _FILE_DATA );

    curCnt = (short) Reader.getNumber();

    if ( curCnt == 0 ) return 0;

    if ( m_itemCnt+curCnt > 100 ) {
        throw new CMTException( UERR_MAXITEMCOUNT, EMSG_MAXITEMCOUNT );
    }

    pos = m_itemCnt;
    for ( i=0; i<curCnt; i++,pos++ ) {
        m_itemValue[pos].itemCode = Reader.getNumber();
        m_itemValue[pos].itemType = (unsigned char) Reader.getNumber();
    }

    pos = m_itemCnt;
    for ( i=0; i<curCnt; i++,pos++ ) {
        l_itemOcc = Reader.getNumber(); // Row Count
        if ( l_itemOcc == 0 ) continue;

        if ( l_itemOcc < 0 || l_itemOcc > 2000 ) {
            sprintf(tempStr,"Index=[%d],Item ID[%d]=[%d] => Item value occurance must 0 ~ 2000.",(i+1),m_itemValue[pos].itemCode,l_itemOcc);
            throw new CMTException( UERR_MAXITEMCOUNT, tempStr);
//          throw new CMTException( UERR_MAXITEMCOUNT, "Item value occurance must 0 ~ 2000." );

        }

        m_itemValue[pos].itemOcc = l_itemOcc;

        m_itemValue[pos].mValue = (VALUE_UNION *) MRMemAlloc1( sizeof(VALUE_UNION) * l_itemOcc );
        if ( ! m_itemValue[pos].mValue ){
            sprintf(tempStr," pos[%d] itemCode[%d],l_itemOcc[%d] ",pos,m_itemValue[pos].itemCode,l_itemOcc);
            throw new CMTException( UERR_MAXITEMCOUNT, tempStr );
//           throw new CMTException( SYSERR_MEMORYALLOCATEFAILURE, ERRMSG_MEMORYALLOCATEFAILURE );

        }

        for ( int j=0; j<l_itemOcc; j++ ) {
            switch ( m_itemValue[pos].itemType ) {
                case DATA_TYPE_DOUBLE :
					m_itemValue[pos].mValue[j].dblVal = Reader.getDouble();
					break;
                case DATA_TYPE_BOOL :
                    m_itemValue[pos].mValue[j].boolVal = Reader.getNumber();
                    break;
                
                case DATA_TYPE_LONG :
					m_itemValue[pos].mValue[j].longVal = Reader.getNumber();
					break;

                case DATA_TYPE_STRING :
                strcpy( tempStr, Reader.getString() );
                McRTrim(tempStr);
                leng = strlen( tempStr );
                if ( leng > STR_VALUE_SIZE ) {
                    leng = STR_VALUE_SIZE;
                    tempStr[leng] = 0;
                }

//                  if ( leng == 0 )
//                      strcpy( m_itemValue[pos].mValue[j].strVal, " " );
//                  else
                strcpy( m_itemValue[pos].mValue[j].strVal, tempStr );

                break;

                case DATA_TYPE_LSTRING :
                strcpy( tempStr, Reader.getString() );
                McRTrim(tempStr);

                m_itemValue[pos].mValue[j].pLStr =
                (char*) MRMemAlloc1( strlen(tempStr)+1 );
                strcpy( m_itemValue[pos].mValue[j].pLStr, tempStr );

                break;
            }
        }
    }

    m_itemCnt += curCnt;

    /*
     //  Global Variable
     TYPE_VALUE tVal;
     char Name[100];
     int  vCnt;

     curCnt = (short) Reader.getNumber();

     if ( curCnt == 0 ) return 0;

     for ( i=0; i<curCnt; i++,pos++ ) {
     strcpy( Name, Reader.getString() );

     tVal.dataType = Reader.getNumber();        //  Type
     vCnt = Reader.getNumber();             //  value cnt

     for ( int k=0; k<vCnt; k++ ) {
     strcpy( tempStr, Reader.getString() );

     switch ( tVal.dataType ) {
     case DATA_TYPE_DOUBLE :
     tVal.value.dblVal = atof( tempStr );
     break;

     case DATA_TYPE_LONG :
     tVal.value.dblVal = atoi( tempStr );
     break;

     case DATA_TYPE_STRING :
     strcpy( tVal.value.strVal, tempStr );
     break;

     case DATA_TYPE_BOOL :
     tVal.value.boolVal = atoi( tempStr );
     break;

     default :
     strcpy( tVal.value.strVal, tempStr );
     break;
     }

     if ( k == 0 ) setGblVal( Name, &tVal, 1 );
     else pushGblVal( Name, &tVal );
     }
     }
     */

    return curCnt;
}

#endif

#ifdef _RUNNER

int CMRDataList::BuildPgmParms( CBRMObj *ObjBuffer )
{
    int i, leng;
    int l_itemOcc;
    char tempStr[500];

    m_ParmMode = 0;

    m_PageIndex = ObjBuffer->ReadLong();

    m_itemCnt = (short) ObjBuffer->ReadByte();

    if ( m_itemCnt < 0 || m_itemCnt > 100 ) {
        throw new CMTException( UERR_MAXITEMCOUNT, EMSG_MAXITEMCOUNT );
    }

    m_itemValue = (ITEM_VALUE *) MRMemAlloc1( sizeof(ITEM_VALUE) * m_itemCnt );

    for ( i=0; i<m_itemCnt; i++ ) {
        m_itemValue[i].mValue = 0;
        m_itemValue[i].itemCode = ObjBuffer->ReadLong();
        m_itemValue[i].itemType = ObjBuffer->ReadByte();

        m_itemValue[i].itemOcc = 0;
    }

    for ( i=0; i<m_itemCnt; i++ ) {
        l_itemOcc = ObjBuffer->ReadLong(); // Row Count
        if ( l_itemOcc == 0 ) continue;

        if ( l_itemOcc < 0 || l_itemOcc > 2000 ) {
            sprintf(tempStr,"Index=[%d],Item ID[%d]=[%d] => Item value occurance must 0 ~ 2000.",(i+1),m_itemValue[i].itemCode,l_itemOcc);
            throw new CMTException( UERR_MAXITEMCOUNT, tempStr);
//          throw new CMTException( UERR_MAXITEMCOUNT, "Item value occurance must 0 ~ 2000." );
        }

        m_itemValue[i].itemOcc = l_itemOcc;

        m_itemValue[i].mValue = (VALUE_UNION *) MRMemAlloc1( sizeof(VALUE_UNION) * l_itemOcc );
        if ( ! m_itemValue[i].mValue ){
                        sprintf(tempStr," pos[%d] itemCode[%d],l_itemOcc[%d] ",i,m_itemValue[i].itemCode,l_itemOcc);
                throw new CMTException( UERR_MAXITEMCOUNT, tempStr );
            // throw new CMTException( SYSERR_MEMORYALLOCATEFAILURE, ERRMSG_MEMORYALLOCATEFAILURE );
     }

        for ( int j=0; j<l_itemOcc; j++ ) {
            switch ( m_itemValue[i].itemType ) {
                case DATA_TYPE_DOUBLE :
                m_itemValue[i].mValue[j].dblVal = ObjBuffer->ReadDouble();
                break;
                case DATA_TYPE_BOOL :
                m_itemValue[i].mValue[j].boolVal = ObjBuffer->ReadLong();
                break;
                case DATA_TYPE_LONG :
                m_itemValue[i].mValue[j].longVal = ObjBuffer->ReadLong();
                break;

                case DATA_TYPE_STRING :
                ObjBuffer->ReadString(tempStr);
                McRTrim(tempStr);
                leng = strlen( tempStr );
                if ( leng > STR_VALUE_SIZE ) {
                    leng = STR_VALUE_SIZE;
                    tempStr[leng] = 0;
                }

//                  if ( leng == 0 )
//                      strcpy( m_itemValue[i].mValue[j].strVal, " " );
//                  else
                strcpy( m_itemValue[i].mValue[j].strVal, tempStr );

                break;

                case DATA_TYPE_LSTRING :
                ObjBuffer->ReadString(tempStr);
                McRTrim(tempStr);

                m_itemValue[i].mValue[j].pLStr =
                (char*) MRMemAlloc1( strlen(tempStr)+1 );
                strcpy( m_itemValue[i].mValue[j].pLStr, tempStr );

                break;
            }
        }
    }

//  if ( pCMRGblVars->m_TraceMode ) isTrace();    //  Trace¿©ºÎ¸¦ °áÁ¤ÇÑ´Ù.

    return m_itemCnt;
}

int CMRDataList::BuildPgmParmsNoInfo( unsigned char parmMode, CBRMObj *ObjBuffer )
{
    int i, leng;
    int l_itemOcc;
    char tempStr[500];
    ITEM_INFO *pItem;

	m_ParmMode = parmMode;

    m_PageIndex = ObjBuffer->ReadLong();
    if ( parmMode == 0 ) { //  Item Info°¡ µé¾î¿À¸é PassÇÑ´Ù.
        short cnt = (short) ObjBuffer->ReadByte();
        for ( i=0; i<cnt; i++ ) {
            ObjBuffer->ReadLong();
            ObjBuffer->ReadByte();

        }
    }
    m_itemCnt = m_pPgmNode->itemCnt;

    m_itemValue = (ITEM_VALUE *) MRMemAlloc1( sizeof(ITEM_VALUE) * m_itemCnt );
    pItem = (ITEM_INFO *) m_pPgmNode->pParm;
    for ( i=0; i<m_itemCnt; i++ ) {
        m_itemValue[i].mValue = 0;
        m_itemValue[i].itemCode = pItem[i].itemCode;
        m_itemValue[i].itemType = pItem[i].itemType;
        strcpy(m_itemValue[i].itemName, pItem[i].itemName);
        m_itemValue[i].itemName[200]=0;
        m_itemValue[i].itemOcc = 0;
    }

    for ( i=0; i<m_itemCnt; i++ ) {
        l_itemOcc = ObjBuffer->ReadLong(); // Row Count
//      sprintf(tempStr, "ITEM[%d] VALUE COUNT : %d\n", i, l_itemOcc );
//      _WriteLog( _LOG_LOG, tempStr );

		if (l_itemOcc < 0) {
//			m_itemValue[i].itemOcc = 0;
//			m_itemValue[i].mValue = (VALUE_UNION *)MRMemAlloc1(sizeof(VALUE_UNION) * 1);
			break;
		}

		if ( l_itemOcc == 0 ) continue;

        if ( l_itemOcc < 0 || l_itemOcc > 2000 ) {
            sprintf(tempStr,"Index=[%d], Item[%s] => Item value occurance must 0 ~ 2000.",(i+1),m_itemValue[i].itemName,l_itemOcc);
            throw new CMTException( UERR_ITEMVALOUTOFINDEX, tempStr);
//          throw new CMTException( UERR_MAXITEMCOUNT, "Item value occurance must 0 ~ 2000." );
        }

        m_itemValue[i].itemOcc = l_itemOcc;

		m_itemValue[i].mValue = (VALUE_UNION *) MRMemAlloc1( sizeof(VALUE_UNION) * l_itemOcc );

        if ( ! m_itemValue[i].mValue ){
             sprintf(tempStr,"ITEM [%d] occ[%d]", i,    l_itemOcc );        
             throw new CMTException( UERR_ITEMVALOUTOFINDEX, tempStr );
//           throw new CMTException( SYSERR_MEMORYALLOCATEFAILURE, ERRMSG_MEMORYALLOCATEFAILURE );
        }
		for ( int j=0; j<l_itemOcc; j++ ) {
            switch ( m_itemValue[i].itemType ) {
                case DATA_TYPE_DOUBLE :
                m_itemValue[i].mValue[j].dblVal = ObjBuffer->ReadDouble();
                break;

                case DATA_TYPE_LONG :
                m_itemValue[i].mValue[j].longVal = ObjBuffer->ReadLong();
                break;
                case DATA_TYPE_BOOL :
                m_itemValue[i].mValue[j].boolVal = ObjBuffer->ReadLong();
                break;
                case DATA_TYPE_STRING :

                if ( ObjBuffer->ReadString(tempStr) == -1 ) {
                     sprintf(tempStr,"ITEM [%s] occ[%d]=NULL", m_itemValue[i].itemName,j);
                    throw new CMTException( UERR_ITEMVALOUTOFINDEX, tempStr );
                };

                McRTrim(tempStr);
                leng = strlen( tempStr );
                if ( leng > STR_VALUE_SIZE ) {
                    leng = STR_VALUE_SIZE;
                    tempStr[leng] = 0;
                }

//                  if ( leng == 0 )
//                      strcpy( m_itemValue[i].mValue[j].strVal, " " );
//                  else
                strcpy( m_itemValue[i].mValue[j].strVal, tempStr );

                break;

                case DATA_TYPE_LSTRING :
                if ( ObjBuffer->ReadString(tempStr) == -1 ) {
                     sprintf(tempStr,"ITEM [%s] occ[%d]=NULL",m_itemValue[i].itemName, j  );

                    throw new CMTException( UERR_ITEMVALOUTOFINDEX, tempStr );
                    //throw new CMTException( EMSG_ITEMVALOUTOFINDEX, ERRMSG_MEMORYALLOCATEFAILURE );
                }

                McRTrim(tempStr);

                m_itemValue[i].mValue[j].pLStr =
                (char*) MRMemAlloc1( strlen(tempStr)+1 );
                strcpy( m_itemValue[i].mValue[j].pLStr, tempStr );
                break;
            }
        }
    }

//  if ( pCMRGblVars->m_TraceMode ) isTrace();    //  Trace¿©ºÎ¸¦ °áÁ¤ÇÑ´Ù.

    return m_itemCnt;
}

int CMRDataList::BuildCBLPgmParms( unsigned char parmMode, unsigned char *pBuffer )
{
    m_ParmMode = parmMode;

    m_itemCnt = (short) m_pPgmNode->itemCnt;

    m_itemValue = (ITEM_VALUE *) MRMemAlloc1( sizeof(ITEM_VALUE) * m_itemCnt );

    m_lastItemIdx = 0;

    ParseItemStream( (PGM_PARM *) m_pPgmNode->pParm, (char*) pBuffer );

    return 0;
}

void CMRDataList::ParseItemStream( PGM_PARM *pParm, char *pBuffer )
{
    PGM_PARM *p;
    char *off;
    int Rows;
    int idx;

    p = pParm;
    off = pBuffer;

    while ( p ) {
        if ( p->MaxOcc > 1 && p->ValidOcc_Icd > 0 ) {
            int n = getItemColumn( p->ValidOcc_Icd );
            Rows = m_itemValue[n].mValue[0].longVal;

            if ( p->pChild ) ParseChildItemStream( p->pChild, off, Rows );
        } else {
            Rows = 1;

            if ( p->pChild ) ParseItemStream( p->pChild, off );
        }

        if ( p->itemCode > 0 ) {
            idx = m_lastItemIdx;
            m_itemValue[m_lastItemIdx].itemCode = p->itemCode;
            m_itemValue[m_lastItemIdx].itemType = p->itemType;
            m_itemValue[m_lastItemIdx].itemOcc = Rows;
            m_itemValue[m_lastItemIdx].mValue = (VALUE_UNION *) MRMemAlloc1( sizeof(VALUE_UNION) * Rows );
            m_lastItemIdx++;

            ParseItemStreamOcc( m_itemValue[idx].mValue, p, off, Rows );
        }

        off += p->L1 * p->MaxOcc;
        p = p->pNext;
    }
}

void CMRDataList::ParseItemStreamOcc( VALUE_UNION *mValue, PGM_PARM *p, char *pBuffer, int Rows )
{
    char *off;
    int sz;
    char str[STR_VALUE_SIZE+1];

    off = pBuffer;

    for ( int vIdx=0; vIdx<Rows; vIdx++ ) {

        switch ( p->itemType ) {
            case DATA_TYPE_DOUBLE :
            if ( m_ParmMode == 1 ) {
                double sign = 1.0;
                short n;

                n = p->L1 - p->L2;
                memcpy( str, off, n );
                if ( p->L2 > 0 ) {
                    str[n] = '.';
                    memcpy( str+n+1, off+n, p->L2 );
                    n = p->L1 + 1;
                }
                str[n] = 0;

                /*printf("ParseItemStreamOcc bef = %lf\n", atof(str));
                 if ( str[n-1] < 240 ) {
                 str[n-1] |= 0xF0;
                 sign = -1.0;
                 }

                 mValue[vIdx].dblVal = atof( (char*) str ) * sign;

                 printf("ParseItemStreamOcc aft = %lf\n", mValue[vIdx].dblVal);
                 */
                mValue[vIdx].dblVal = atof(str);
            } else {
                memcpy( (char*) &mValue[vIdx].dblVal, off, p->L1 ); //  8 Byte
            }
            break;

            case DATA_TYPE_LONG :
            if ( m_ParmMode == 1 ) {
                memcpy( str, off, p->L1 ); str[p->L1] = 0;
                McRTrim( str );
                mValue[vIdx].longVal = atoi( str );
            } else {
                memcpy( (char*) &mValue[vIdx].longVal, off, p->L1 ); //  4 Byte
            }
            break;

            case DATA_TYPE_STRING :
            if ( p->L1 > STR_VALUE_SIZE ) sz = STR_VALUE_SIZE;
            else sz = p->L1;
            memcpy( str, off, sz ); str[sz] = 0;

            McRTrim( str );
            strcpy( mValue[vIdx].strVal, str );
        }

        off += p->L1;
    }
}

void CMRDataList::ParseChildItemStream( PGM_PARM *pParm, char *pBuffer, int itemOcc )
{
    PGM_PARM *p;
    char *off;
    int idx, i, sz;
    char str[STR_VALUE_SIZE+1];

    p = pParm;
    idx = m_lastItemIdx;

    while ( p ) {
        m_itemValue[m_lastItemIdx].itemCode = p->itemCode;
        m_itemValue[m_lastItemIdx].itemType = p->itemType;
        m_itemValue[m_lastItemIdx].itemOcc = itemOcc;
        m_itemValue[m_lastItemIdx].mValue = (VALUE_UNION *) MRMemAlloc1( sizeof(VALUE_UNION) * itemOcc );
        m_lastItemIdx++;

        p = p->pNext;
    }

    off = pBuffer;
    for ( int vIdx=0; vIdx<itemOcc; vIdx++ ) {
        i = idx;
        p = pParm;

        while ( p ) {

            if ( p->itemCode > 0 ) {

                switch ( p->itemType ) {
                    case DATA_TYPE_DOUBLE :
                    if ( m_ParmMode == 1 ) {
                        double sign = 1.0;
                        short n;

                        n = p->L1 - p->L2;
                        memcpy( str, off, n );
                        if ( p->L2 > 0 ) {
                            str[n] = '.';
                            memcpy( str+n+1, off+n, p->L2 );
                            n = p->L1 + 1;
                        }
                        str[n] = 0;
                        /*
                         printf("ParseChildItemStream bef = %lf\n", atof(str));
                         if ( str[n-1] < 240 ) {
                         str[n-1] |= 0xF0;
                         sign = -1.0;
                         }

                         m_itemValue[i].mValue[vIdx].dblVal = atof( (char*) str ) * sign;
                         printf("ParseChildItemStream aft = %lf\n", m_itemValue[i].mValue[vIdx].dblVal);
                         */
                        m_itemValue[i].mValue[vIdx].dblVal = atof(str);
                    } else {
                        memcpy( (char*) &m_itemValue[i].mValue[vIdx].dblVal, off, p->L1 ); //  8 Byte
                    }
                    break;

                    case DATA_TYPE_LONG :
                    if ( m_ParmMode == 1 ) {
                        memcpy( str, off, p->L1 ); str[p->L1] = 0;
                        McRTrim( str );
                        m_itemValue[i].mValue[vIdx].longVal = atoi( str );
                    } else {
                        memcpy( (char*) &m_itemValue[i].mValue[vIdx].longVal, off, p->L1 ); //  4 Byte
                    }
                    break;

                    case DATA_TYPE_STRING :
                    if ( p->L1 > STR_VALUE_SIZE ) sz = STR_VALUE_SIZE;
                    else sz = p->L1;

                    memcpy( str, off, sz ); str[sz] = 0;
                    McRTrim( str );
                    strcpy( m_itemValue[i].mValue[vIdx].strVal, str );
                }
            }

            off += p->L1;
            p = p->pNext;
        }

        i++;
    }
}

#endif

void CMRDataList::setItemValid(bool bFlag) {
    m_bValidItems = bFlag;
}

void CMRDataList::setRuleId(long Id) {
    m_RuleId = Id;
}

long CMRDataList::LogErrorItems(int idx, long rc, char *msg) {
#ifndef _TESTER
#ifdef _RUNNER
	char LogMsg[600];
	char LogMsg2[600];
	memset(LogMsg,0x00,600);
	memset(LogMsg2,0x00,600);
	strncpy(LogMsg2, msg,strlen(msg)>500 ? 500 : strlen(msg) ); LogMsg2[500] = 0;
	sprintf(LogMsg, "PGM(%s::%d) : %d. %s",m_PgmId, m_RuleId, rc, LogMsg2); LogMsg[500] = 0; 
	_WriteLogIP( _LOG_ERR,m_StartIP, LogMsg );

    if ( rc == UERR_DBCONNECT ) return 0;
		long rr = DispItems( m_PgmId, rc, LogMsg );

		if ( rr != 0 ) {
        sprintf(LogMsg, MSG_LOGINS_ERR, LogMsg, rr ); LogMsg[599] = 0;
//startip     _WriteLog( SYSERR_, LogMsg );
		_WriteLogIP( SYSERR_,m_StartIP, LogMsg );
        return -1;
    }
		if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]\n", __FILE__, __LINE__);

#endif
#endif

    return 0;
}

int CMRDataList::getItemColumn(int ItemCd) {
    for (int i = 0; i < m_itemCnt; i++) {
        if (m_itemValue[i].itemCode == ItemCd) {
            return i;
        }
    }
    return -1;
}

int CMRDataList::getItemValueCount(int col) {
    return (int) m_itemValue[col].itemOcc;
}

int CMRDataList::getRuleValueIndex(VALUE_UNION *v, TYPE_VALUE *Value) {
    RULE_RESULT *pResult;
    RESULT_REC *pRec;
    int col;
    int i = 0;

    pResult = (RULE_RESULT*) v->RuleItem.ptr;
    pRec = pResult->resRecord;
    col = v->RuleItem.i_col;
    if (col > 0)
        col--;

    switch (Value->dataType) {
    case DATA_TYPE_STRING:
        while (pRec) {
            char *p1 = pRec->value[col].strVal;
            char *p2 = Value->value.strVal;

            McRTrim(p1);
            McRTrim(p2);
            if (!strcmp(p1, p2))
                return i + 1;

            pRec = pRec->pNext;
            i++;
        }
        break;

    case DATA_TYPE_LONG:
        while (pRec) {
            if (pRec->value[col].longVal == Value->value.longVal)
                return i + 1;

            pRec = pRec->pNext;
            i++;
        }

        break;
    default:
        break;
    }

    return -1;
}

int CMRDataList::isItemValueExist(int itemCd, char * str) {
    int col;

    if (!str) {
        printf("  Error ( RiderCode is NULL )\n");
        return 0;
    }

    McRTrim(str);

    if ((col = getItemColumn(itemCd)) < 0) {
//          printf("\n     Error : Item (%d) not found.\n", col );
//          throw new CMTException( UERR_ITEMNOTFOUND, EMSG_ITEMNOTFOUND );
        return -1;
    }

    for (int i = 0; i < m_itemValue[col].itemOcc; i++) {
        char *p = m_itemValue[col].mValue[i].strVal;
        if (strcmp(p, str) == 0)
            return 1;
    }

    return 0;
}

int CMRDataList::getItemValueIndex(int col, TYPE_VALUE *Value) {
    int i;

    switch (m_itemValue[col].itemType) {
    case DATA_TYPE_STRING:
        for (i = 0; i < m_itemValue[col].itemOcc; i++) {
            char *p1 = m_itemValue[col].mValue[i].strVal;
            char *p2 = Value->value.strVal;

            McRTrim(p1);
            McRTrim(p2);
            if (strcmp(p1, p2) == 0)
                return i + 1;
        }
        break;
    case DATA_TYPE_LONG:
        for (i = 0; i < m_itemValue[col].itemOcc; i++) {
            if (m_itemValue[col].mValue[i].longVal == Value->value.longVal)
                return i + 1;
        }
        break;
    default:
        break;
    }

    return -1;
}

unsigned char CMRDataList::getItemType(int col) {
    return m_itemValue[col].itemType;
}

VALUE_UNION * CMRDataList::getItemValuePtr(int col) {
    return m_itemValue[col].mValue;
}

void CMRDataList::setInitValue(TYPE_VALUE *Value, char retType) {
    switch (retType) {
    case DATA_TYPE_LONG:
        Value->dataType = TOKENTYPE_CONST_N;
#ifdef _PARSER
        Value->value.longVal = 1;
#else
        Value->value.longVal = 0;
#endif
        break;
    case DATA_TYPE_STRING:
        Value->dataType = TOKENTYPE_CONST_C;
        Value->value.strVal[0] = 0;
        break;
    case DATA_TYPE_DOUBLE:
        Value->dataType = TOKENTYPE_CONST_F;
#ifdef _PARSER
        Value->value.dblVal = 1.0;
#else
        Value->value.dblVal = 0.0;
#endif
        break;
    case DATA_TYPE_LSTRING:
        Value->dataType = TOKENTYPE_CONST_L;
        Value->value.pLStr = 0;
//                  Value->value.LString.len = 0;
        break;
    case DATA_TYPE_BOOL:
        Value->dataType = TOKENTYPE_CONST_B;
        Value->value.boolVal = false;
        break;
    default:
        Value->dataType = TOKENTYPE_NONE;
        break;
    }
}

void CMRDataList::getItemValue(TYPE_VALUE *Value) {
#ifndef _PARSER
    int row, col;

    col = Value->value.RuleItem.i_col;
    row = (int) Value->value.RuleItem.i_row;

    if (m_itemValue[col].itemOcc == 0) {
        char msg[500];
//      char rName[101];
//      pCMRLoadData->getItemName( m_itemValue[col].itemCode, rName );
//          sprintf(msg, "Item[%d] value not passed", m_itemValue[col].itemCode);
        if( m_itemValue[col].itemName!=NULL){
            //sprintf(msg, "Item[%s] value not passed", m_itemValue[col].itemName); //itemName  °ª¾øÀ½
            sprintf(msg, MSG_ITEM_NOTPASS_STR, m_itemValue[col].itemName);
      }else{
            //sprintf(msg, "Item[%d] value not passed", m_itemValue[col].itemCode);
            sprintf(msg, MSG_ITEM_NOTPASS_DEC, m_itemValue[col].itemCode);
      }
 

        throw new CMTException(UERR_ITEMVALOUTOFINDEX, msg);
    }


    if (row < 0) {
        setInitValue(Value, m_itemValue[col].itemType);
        return;
    }

    if (row > 0)
        row--;

    if (row >= m_itemValue[col].itemOcc) {
        char msg[127];
        sprintf(msg, "%s (%s)", EMSG_ITEMVALOUTOFINDEX,m_itemValue[col].itemName);
        throw new CMTException(UERR_ITEMVALOUTOFINDEX, msg);
    }

    Value->dataType = m_itemValue[col].itemType;
    memcpy((unsigned char *) &Value->value,
            (unsigned char *) &m_itemValue[col].mValue[row],
            sizeof(VALUE_UNION));
    if (Value->dataType == DATA_TYPE_LSTRING) {
        int leng = strlen(m_itemValue[col].mValue[row].pLStr);
        Value->value.pLStr = (char*) MRMemAlloc1(leng + 1);
        strcpy(Value->value.pLStr, m_itemValue[col].mValue[row].pLStr);
    }
#endif
}

bool CMRDataList::getItemValueByID(int ItemCd, TYPE_VALUE *Value) {
    int i;

    for (i = 0; i < m_itemCnt; i++) {
        if (m_itemValue[i].itemCode == ItemCd)
            break;
    }

    if (i == m_itemCnt)
        return 0;

    if (m_itemValue[i].itemOcc == 0)
        return 0;

    Value->dataType = m_itemValue[i].itemType;
    memcpy((unsigned char *) &Value->value,
            (unsigned char *) &m_itemValue[i].mValue[0], sizeof(VALUE_UNION));
    if (Value->dataType == DATA_TYPE_LSTRING) {
        char temp[STR_VALUE_SIZE+1];
        strncpy(temp, m_itemValue[i].mValue[0].pLStr, STR_VALUE_SIZE);
        temp[STR_VALUE_SIZE] = 0;

        Value->dataType = DATA_TYPE_STRING;
        strcpy(Value->value.strVal, temp);
    }

    return 1;
}

void CMRDataList::convertValueToReturnType(VALUE_UNION *val, char retType,
        int col, int row) {
    char *p;
    unsigned char iType = m_itemValue[col].itemType;

    memcpy((char *) val, (char *) &m_itemValue[col].mValue[row],
            sizeof(VALUE_UNION));
		

    if (retType == iType)
        return;

    switch (retType) {
    case TOKENTYPE_CONST_F:
        if (iType == TOKENTYPE_CONST_N)
            val->dblVal = (double) val->longVal;
        else if (iType == TOKENTYPE_CONST_B)
            val->dblVal = (double) val->boolVal;
        else
            val->dblVal = 0.0;

        break;

    case TOKENTYPE_CONST_N:
        if (iType == TOKENTYPE_CONST_F) {
            char *p, temp[31];
            int ch = '.';
            sprintf(temp, "%lf", val->dblVal);
            temp[30]=0;
            p = strchr(temp, ch);
            *p = 0;
            val->longVal = atoi(temp);

        } else if (iType == TOKENTYPE_CONST_B)
            val->longVal = val->boolVal;
        else
            val->longVal = 0;

        break;

    case TOKENTYPE_CONST_C:
    	memset(val->strVal,0x00, STR_VALUE_SIZE );
        if (iType == TOKENTYPE_CONST_L) {
            if (val->pLStr) {
                strncpy(val->strVal, val->pLStr, STR_VALUE_SIZE);
                val->strVal[STR_VALUE_SIZE] = 0;
            } else
                val->strVal[0] = 0;
        } else if (iType == TOKENTYPE_CONST_F)
            sprintf(val->strVal, "%lf", val->dblVal);
        else if (iType == TOKENTYPE_CONST_N)
            sprintf(val->strVal, "%ld", val->longVal);
        else if (iType == TOKENTYPE_CONST_B)
            sprintf(val->strVal, "%s", val->boolVal ? "TRUE" : "FALSE");

        break;

    case TOKENTYPE_CONST_L:
        p = (char *) MRMemAlloc1(STR_VALUE_SIZE + 1);

        if (iType == TOKENTYPE_CONST_C)
            strcpy(p, val->strVal);
        else if (iType == TOKENTYPE_CONST_F)
            sprintf(p, "%lf", val->dblVal);
        else if (iType == TOKENTYPE_CONST_N)
            sprintf(p, "%ld", val->longVal);
        else if (iType == TOKENTYPE_CONST_B)
            sprintf(p, "%s", val->boolVal ? "TRUE" : "FALSE");

        val->pLStr = p;
        break;

    case TOKENTYPE_CONST_B:
        if (iType != TOKENTYPE_CONST_B)
            val->boolVal = (bool) val->longVal;
        break;

    }
}

RESULT_REC * CMRDataList::getAllItemValues(TYPE_VALUE *Value, char retType) {
    RESULT_REC *pResultList = 0;

#ifndef _PARSER
    RESULT_REC *pLast=NULL;
    int col, row;

    col = Value->value.RuleItem.i_col;

    if (col >= m_itemCnt) {
        throw new CMTException(UERR_ITEMNOTFOUND, EMSG_ITEMNOTFOUND);
    }

    row = Value->value.RuleItem.i_row;
    if (row == 0) {
        pResultList = 0;
        for (int i = 0; i < m_itemValue[col].itemOcc; i++) {
            if (i == 0) {
                pResultList = (RESULT_REC *) MRMemAlloc1(sizeof(RESULT_REC));
                pLast = pResultList;
            } else {
                pLast->pNext = (RESULT_REC *) MRMemAlloc1(sizeof(RESULT_REC));
                pLast = pLast->pNext;
            }
            pLast->pNext = 0;

            pLast->value = (VALUE_UNION *) MRMemAlloc1(sizeof(VALUE_UNION));

            convertValueToReturnType(pLast->value, retType, col, i);

        }
    } else {
        if (row > m_itemValue[col].itemOcc) {
            char msg[500];
            if( m_itemValue[col].itemName!=NULL){
                sprintf(msg, "%s [%s]", EMSG_ITEMVALOUTOFINDEX,m_itemValue[col].itemName);
        }else{
                sprintf(msg, "%s [%d]", EMSG_ITEMVALOUTOFINDEX,m_itemValue[col].itemCode);
        }
            throw new CMTException(UERR_ITEMVALOUTOFINDEX, msg);
        }

        pResultList = (RESULT_REC *) MRMemAlloc1(sizeof(RESULT_REC));
        pResultList->value = (VALUE_UNION *) MRMemAlloc1(sizeof(VALUE_UNION));
        pResultList->pNext = 0;

        if (row > 0) {
            convertValueToReturnType(pResultList->value, retType, col, row - 1);
        } else { //  row < 0ÀÏ¶§
            TYPE_VALUE tVal;
            setInitValue(&tVal, retType);
            memcpy((char *) pResultList->value, (char *) &tVal.value,
                    sizeof(VALUE_UNION));
        }
    }

#endif

    return pResultList;
}

void CMRDataList::GetRuleValue(TYPE_VALUE *Value) {
    RULE_RESULT *p;
    RESULT_REC *pRec;
    int row, col;
    char retType;

    p = (RULE_RESULT *) Value->value.RuleItem.ptr;
    
#ifndef _BRMLIB
	if (p->pRuleNode->rule_id <= 0) { /*2019.03.30 »ê¾÷ÀºÇà¿¡¼­ 0ÀÎ °æ¿ì ¹ß»ý.*/
		return;
	}
#endif

    row = Value->value.RuleItem.i_row;
    col = Value->value.RuleItem.i_col;
    if (col > 0)
        col--;

//printf("CMRDataList::GetRuleValue] col(%d)\n",  col );

    if (col > p->pRuleNode->retCnt) {
        char msg[200];/*
        sprintf(msg, "Used the returns {%d::%d} which is not defined.",
                p->pRuleNode->rule_id, col + 1);*/
        sprintf(msg, MSG_NOTDEFINE_RET,p->pRuleNode->rule_id, col + 1);
        throw new CMTException(-1, msg);
    }

#ifndef _PARSER
    /*
     RULE_LIST   *pRuleNode;
     pRuleNode = pCMRGblVars->getRuleNode( p->ruleId );
     if ( ! pRuleNode ) {
     char msg[100];
     sprintf( msg,"Rule not defined : %d", p->ruleId );
     throw new CMTException( 88888, msg );
     }

     retType = pRuleNode->retType[col];
     */
    retType = p->pRuleNode->retType[col];
#else
    retType = p->pRuleNode->retType[col];

#endif

//  printf("CMRDataList::GetRuleValue] - getReturnType = %d\n", retType );

#ifndef _PARSER
    if (row < 0) {
#endif
        setInitValue(Value, retType);
        return;

#ifndef _PARSER
    }
#endif

#ifndef _PARSER

    pRec = p->resRecord;
    if (!pRec) {

        // 2010-2-11 vapor ¾øÀ» °æ¿ì ¿¡·¯³¯¸®Áö ¾Ê°í ¾È³Ö´Â´Ù.
        return;

        char msg[100];
        sprintf(msg, "%s : %d", EMSG_NORESULTERR, p->ruleId);
        throw new CMTException(UERR_NORESULTERR, msg);
    }

    for (int i = 1; i < row; i++) {
        if (!pRec->pNext) {
            char msg[100];
            sprintf(msg, "%s : %d", EMSG_RESULTOUTOFBIND, p->ruleId);
            throw new CMTException(UERR_RESULTOUTOFBIND, msg);
        }
        pRec = pRec->pNext;
    }

    Value->dataType = (unsigned char) retType;

    /*  Test·Î ¸·À½ ..  ¹®Á¦ ÀÖÀ¸¸é Ç®°Í..  2005-12-13
     if ( retType == DATA_TYPE_LSTRING ) {
     int len;

     if ( pRec->value[col].LString.len > LSTR_VALUE_SIZE )
     len = pRec->value[col].LString.len + 1;
     else
     len = LSTR_VALUE_SIZE + 1;

     Value->value.pLStr = (char *) MRMemAlloc1( len );
     if ( pRec->value[col].pLStr ) {
     strcpy( Value->value.pLStr, pRec->value[col].pLStr );
     Value->value.LString.len = len;
     } else {
     Value->value.pLStr[0] = 0;
     Value->value.LString.len = 0;
     }
     } else   */
    memcpy(&Value->value, &pRec->value[col], sizeof(VALUE_UNION));

#endif
}

void CMRDataList::Val2String(VALUE_UNION *Value, char retType, char *s,
        char op) {
    switch (retType) {
    case DATA_TYPE_LONG:
        sprintf(s, "%ld", Value->longVal);
        break;
    case DATA_TYPE_STRING:
        if (op == 0)
            sprintf(s, "%s", Value->strVal);
        else
            sprintf(s, "%c%s%c", op, Value->strVal, op);
        break;
    case DATA_TYPE_DOUBLE:
        sprintf(s, "%lf", Value->dblVal);
        break;
    case DATA_TYPE_LSTRING:
        if (op == 0)
            sprintf(s, "%s", Value->pLStr);
        else
            sprintf(s, "%c%s%c", op, Value->pLStr, op);
        break;
    case DATA_TYPE_BOOL:
        sprintf(s, "%d", Value->boolVal);
        break;
    default:
        break;
    }
}

void CMRDataList::getItemValuesByStr(unsigned char col, long row, char *val) {
    VALUE_UNION Value;
    char retType;

    if (col >= m_itemCnt) {
        throw new CMTException(UERR_ITEMNOTFOUND, EMSG_ITEMNOTFOUND);
    }

    if (m_itemValue[col].itemOcc == 0) {
        char msg[500];
        if( m_itemValue[col].itemName!=NULL){
            sprintf(msg, "%s [%s]", EMSG_ITEMVALNOTPASSED,m_itemValue[col].itemName);
      }else{
            sprintf(msg, "%s [%d]", EMSG_ITEMVALNOTPASSED,m_itemValue[col].itemCode);
      }
        throw new CMTException(UERR_ITEMVALNOTPASSED, msg);
    }

    if (row > m_itemValue[col].itemOcc) {
        char msg[500];
        if( m_itemValue[col].itemName!=NULL){
            sprintf(msg, "%s [%s]", EMSG_ITEMVALOUTOFINDEX,m_itemValue[col].itemName);
      }else{
            sprintf(msg, "%s [%d]", EMSG_ITEMVALOUTOFINDEX,m_itemValue[col].itemCode);
      }
 
        throw new CMTException(UERR_ITEMVALOUTOFINDEX, msg);
    }

    retType = m_itemValue[col].itemType;

    if (m_itemValue[col].itemOcc == 1)
        row = 1;

    if (row > 0) {
        memcpy((char *) &Value, (char *) &m_itemValue[col].mValue[row - 1],
                sizeof(VALUE_UNION));

        Val2String(&Value, retType, val, '\0');
    } else {
        char *s = val;
        *s = 0;

        for (int i = 0; i < m_itemValue[col].itemOcc; i++) {
            memcpy((char *) &Value, (char *) &m_itemValue[col].mValue[i],
                    sizeof(VALUE_UNION));
            Val2String(&Value, retType, s, '\'');
            s += strlen(s);

            if (i + 1 < m_itemValue[col].itemOcc) {
                strcat(s, ",");
                s++;
            }
        }
    }

}

void CMRDataList::GetRuleValueAll(TYPE_VALUE *Value, char *val) {
    RULE_RESULT *p;
    RESULT_REC *pRec;
    int col;
    char retType;
    char *s;

    p = (RULE_RESULT *) Value->value.RuleItem.ptr;

    col = Value->value.RuleItem.i_col;

    if (col > 0)
        col--;

    retType = p->pRuleNode->retType[col];

    pRec = p->resRecord;
    if (!pRec) {
        char msg[100];
        sprintf(msg, "%s : %d", EMSG_NORESULTERR, p->ruleId);
        throw new CMTException(UERR_NORESULTERR, msg);
    }

    if (!pRec->pNext) { //  °ªÀÌ ÇÏ³ªÀÌ¸é
        Val2String(&pRec->value[col], retType, val, '\0');
        return;
    }

    s = val;
    *s = 0;
    while (pRec) {
        Val2String(&pRec->value[col], retType, s, '\'');

        pRec = pRec->pNext;

        if (pRec) {
            s += strlen(s);
            strcat(s, ",");
            s++;
        }
    }

//  printf("[%s]\n", val );
}

RULE_RESULT *CMRDataList::GetRuleResult(int ruleId, void *parm) {
	RULE_RESULT *pRuleResult, *pSelected, *pPrev;
	char *pString;
	short scope;

	pString = (char*)parm;
//	printf(" ruleid 001 [%d] param [%s]  \n", ruleId, parm);   
//	pRuleResult = g_CMRStaticResult.getStaticResult();//2022-06-02ÀÓ½ÃÃß°¡
#ifdef _RUNNER
//	printf(" ruleid 002 [%d] param [%s]  \n", ruleId, parm);   
	scope = pCMRGblVars->getRuleScope(ruleId);
//	printf(" ruleid 003 scope[%d] [%d] param [%s]  \n", scope ,ruleId, parm);   
	#ifdef RULE_RE_RUN
		switch (scope) {
		case 0:
			pRuleResult = m_pRuleResult;
			break;
		case 1:
		case 2:
		default:
			pRuleResult = g_CMRStaticResult.getStaticResult();
		}
//	printf(" ruleid 004 [%d] param [%s]  \n", ruleId, parm);   
	#else
		if (scope) {
			pRuleResult = g_CMRStaticResult.getStaticResult();
		}
		else{
			pRuleResult = m_pRuleResult;
		}
	#endif

#else
	#ifdef RULE_RE_RUN
		//	pRuleResult = 0;  // 	pRuleResult = m_pRuleResult; ?”ë²„ê·¸ì‹œ  GET?VAL, ?Œë¼ë¯¸í„° ... ë£??¬ì‹¤???¤ì •  
		RULE_LIST *pRulePtrx= pCMRGblVars->SearchById(ruleId);
		if (pRulePtrx) scope= pRulePtrx->scope;
		else scope = 0;
		switch (scope) {
		case 0:
			pRuleResult = m_pRuleResult; break;
		default:
			pRuleResult = 0; break;
		}
	#else
		scope = 0;
		pRuleResult = m_pRuleResult;
	#endif

#endif

    pSelected = pRuleResult;
    pPrev = 0;
    while (pSelected) {
        if (ruleId == pSelected->ruleId) {
            if (pString != 0 && pString[0]) {
                if (strcmp(pString, pSelected->parmInfo) == 0)
                    break;
                else { //  ParmÀÌ ´Ù¸¥ °á°ú°¡ ÀÌ¹Ì Á¸ÀçÇÏ¸é Áö¿î´Ù. ( 2008-12-27 ¼º´É¶§¹®¿¡ Ãß°¡ÇÔ )
                    if (pPrev) {
                        pPrev->pNext = pSelected->pNext;
                    } else {
                        m_pRuleResult = m_pRuleResult->pNext;
                    }
                    return 0;
                }
            } else {
                if (pSelected->parmInfo[0] == 0)
                    break;
            }
        }
        pPrev = pSelected;
        pSelected = pSelected->pNext;
    }

    return pSelected;
}

int CMRDataList::getRuleResultCount(RULE_RESULT * ptr) {
    int rows = 0;
    RESULT_REC *pResultRec;

    if (!ptr)
        return 0;

    pResultRec = ptr->resRecord;
    while (pResultRec) {
        rows++;
        pResultRec = pResultRec->pNext;
    }

    return rows;
}

RULE_RESULT* CMRDataList::AddResultForRule(int ruleId, char *parm,
        RESULT_REC *pResultRec, RULE_LIST *pNode, char *retTypes) {
    RULE_RESULT *pNew;

    pNew = (RULE_RESULT *) MRMemAlloc1(sizeof(RULE_RESULT));

    pNew->ruleId = ruleId;
    if (parm)
        strcpy(pNew->parmInfo, parm);
    else
        pNew->parmInfo[0] = 0;

    pNew->pRuleNode = pNode;
    pNew->resRecord = pResultRec;

#ifdef _RUNNER
	#ifdef RULE_RE_RUN
		if ( pNode->scope==1  ) { //2017.08.17  ë£??¬ì‹¤?? if(pNode->scope)=> if ( pNode->scope==1)ë³€ê²½í•¨. 
			return g_CMRStaticResult.AddStaticResult(pNew);
		} else {
	#else
		if(pNode->scope){
			return g_CMRStaticResult.AddStaticResult(pNew);
		}else{
	#endif

#endif
    pNew->pNext = m_pRuleResult;
    m_pRuleResult = pNew;
    return pNew;

#ifdef _RUNNER
	}
#endif

}

#ifdef _RUNNER

long CMRDataList::BuildPgmReturn( CBRMObj *ObjBuffer, RULE_RESULT *pPgmResult )
{
    RESULT_REC *pRec;
    int ResultRowCnt = 0;

    if ( ! pPgmResult ) {
        ObjBuffer->Clear1();
        ObjBuffer->WriteLong(MRC_RULE_ERR);
        ObjBuffer->WriteLong(UERR_STARTRULENOTRUN);
        ObjBuffer->WriteString(EMSG_STARTRULENOTRUN);
        return -1;
    }

    ObjBuffer->Clear1();
    ObjBuffer->WriteLong( (long) 0 );

    pRec = pPgmResult->resRecord;
    while ( pRec ) {
        ResultRowCnt++;
        pRec = pRec->pNext;
    }

    /*
     if ( ResultRowCnt == 0 )
     throw new CMTException( UERR_NORESULT, EMSG_NORESULT );
     */
	printf("ResultRowCnt [%d] \n", ResultRowCnt);
    ObjBuffer->WriteLong( ResultRowCnt ); //row

#ifdef ENG40
    ObjBuffer->WriteByte( (unsigned char) pStartRuleNode->retCnt );//colunm
#elif ENG45
    //4.0    ObjBuffer->WriteByte( (unsigned char) pStartRuleNode->retCnt );//colunm
    //5.0
    ObjBuffer->WriteLong( pStartRuleNode->retCnt ); //colunm
    ObjBuffer->WriteByte((unsigned char)1); // end 
	ObjBuffer->WriteString("");
#else
    //4.0    ObjBuffer->WriteByte( (unsigned char) pStartRuleNode->retCnt );//colunm
    //5.0
    ObjBuffer->WriteLong( pStartRuleNode->retCnt ); //colunm
    ObjBuffer->WriteByte((unsigned char)1); // end 
    ObjBuffer->WriteString("");

#endif
    
		
    for ( int i=0; i<pStartRuleNode->retCnt; i++ ) {
        if ( pStartRuleNode->retType[i] == DATA_TYPE_BOOL )
        ObjBuffer->WriteByte( (unsigned char) DATA_TYPE_LONG );
        else
        ObjBuffer->WriteByte( (unsigned char) pStartRuleNode->retType[i] );
    }

    pRec = pPgmResult->resRecord;

    VALUE_UNION *p;
	if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]\n", __FILE__, __LINE__);
	int count = 0;
    while ( pRec ) {
		if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]cnt[%d]\n", __FILE__, __LINE__, count++ );

		for ( int i=0; i<pStartRuleNode->retCnt; i++ ) {
			if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]cnt[%d]\n", __FILE__, __LINE__, count);
			p = &pRec->value[i];
			if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]cnt[%d]\n", __FILE__, __LINE__, count);
//			if (_DEBUG_YN[0] == 'Y') printf("pStartRuleNode->retCnt [%d]\n", pStartRuleNode->retCnt);
//			if (_DEBUG_YN[0] == 'Y') printf("pStartRuleNode->retType[i]  [%d]\n", pStartRuleNode->retType[i]);

			
			switch ( pStartRuleNode->retType[i] ) {
                case DATA_TYPE_LONG :
					if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]cnt[%d]\n", __FILE__, __LINE__, count);
					ObjBuffer->WriteLong( p->longVal );
					if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]cnt[%d]\n", __FILE__, __LINE__, count);
					break;
                case DATA_TYPE_STRING :
					if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]cnt[%d]\n", __FILE__, __LINE__, count);
//					if (_DEBUG_YN[0] == 'Y') printf("p->strVal [%s]\n", p->strVal);
					ObjBuffer->WriteString ( p->strVal );
					if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]cnt[%d]\n", __FILE__, __LINE__, count);
					break;
                case DATA_TYPE_LSTRING :
					ObjBuffer->WriteString( p->pLStr );
					break;
                case DATA_TYPE_DOUBLE :
					if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]cnt[%d]\n", __FILE__, __LINE__, count);
					ObjBuffer->WriteDouble( p->dblVal );
					if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]cnt[%d]\n", __FILE__, __LINE__, count);
					break;
                case DATA_TYPE_BOOL :
	                ObjBuffer->WriteLong( (long) p->boolVal );
		            break;
                default :
                break;
            }
			if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]cnt[%d]\n", __FILE__, __LINE__, count);
		}
		if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]cnt[%d]\n", __FILE__, __LINE__, count);
		pRec = pRec->pNext;
		if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]cnt[%d]\n", __FILE__, __LINE__, count);
	}
	if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]cnt[%d]\n", __FILE__, __LINE__, count);

    return 0;
}

#endif

RESULT_REC * CMRDataList::createRecord(RULE_LIST *pRuleNode, char *resString) {
    RESULT_REC *pResultRec;
    VALUE_UNION Value;
	char tokenStr[4096 *4];
    int tOff = 0, Off = 0;
    memset(tokenStr,0x00,sizeof(tokenStr));
    pResultRec = (RESULT_REC *) MRMemAlloc1(sizeof(RESULT_REC));
    pResultRec->value = (VALUE_UNION *) MRMemAlloc1(
            sizeof(VALUE_UNION) * pRuleNode->retCnt);

    pResultRec->pNext = 0;

//printf("!!%s!!\n", resString );
//printf("%02x %02x %02x %02x %02x %02x %02x %02x\n", resString[9],resString[10],resString[11],
//     resString[12],resString[13],resString[14],resString[15],resString[16] );

    for (int j = 0; j < pRuleNode->retCnt; j++) {
        if (pRuleNode->retCnt > 1) {
            tOff = 0;
            while (resString[Off]) {
                if (resString[Off] == '|' && resString[Off + 1] == '^') {
                    Off += 2;
                    break;
                } else {
                    tokenStr[tOff++] = resString[Off++];
                }
            }
            tokenStr[tOff] = 0x00;
        } else {
            strcpy(tokenStr, resString);
            tOff = strlen(tokenStr);
        }

        McRTrim(tokenStr);
/*
printf("createRecord[%d] ",j);
        switch (pRuleNode->retType[j]) {
				case TOKENTYPE_CONST_F:	printf("TOKENTYPE_CONST_F ");break;
				case TOKENTYPE_CONST_C:	printf("TOKENTYPE_CONST_C ");break;
				case TOKENTYPE_CONST_N:	printf("TOKENTYPE_CONST_N ");break;
				case TOKENTYPE_CONST_L:	printf("TOKENTYPE_CONST_L ");break;
				case TOKENTYPE_CONST_B:	printf("TOKENTYPE_CONST_B ");break;
        }		 
printf("[%s]\n", tokenStr);		 
*/
        switch (pRuleNode->retType[j]) {
				case TOKENTYPE_CONST_F:
            Value.dblVal = atof(tokenStr);
            break;
        case TOKENTYPE_CONST_C:
	       	memset(Value.strVal,0x00, STR_VALUE_SIZE );
			if (tOff == 0) {
				strcpy(Value.strVal, " ");
				// Value.strVal[0] = 0;
			}
			else {
				strncpy(Value.strVal, tokenStr, STR_VALUE_SIZE);
                Value.strVal[STR_VALUE_SIZE] = 0;
            }
            break;
        case TOKENTYPE_CONST_N:
						Value.longVal = atol(tokenStr);
            break;
        case TOKENTYPE_CONST_L:
			//              Value.LString.len = tOff;
            Value.pLStr = (char*) MRMemAlloc1(tOff + 1);
            strcpy(Value.pLStr, tokenStr);
            break;
        case TOKENTYPE_CONST_B:
						Value.boolVal = (unsigned char) atol(tokenStr);
            break;
        }
        memcpy((char *) &pResultRec->value[j], (char *) &Value,sizeof(VALUE_UNION));
    }

    return pResultRec;
}

RESULT_REC* CMRDataList::getRuleResult(long ruleId) {
    RULE_RESULT *pPgmResult;
    RESULT_REC *pRec;

    pPgmResult = GetRuleResult(ruleId, (void*) 0);

    if (!pPgmResult)
        return 0;

    pRec = pPgmResult->resRecord;

    return (RESULT_REC*) pRec;
}

short CMRDataList::getParmCount(long RuleId) {
    RULE_LIST *p;

#if defined(_TESTER)
    p = pCMRGblVars->DbgLoadRule( RuleId );
#elif defined(_RUNNER)
    p = pCMRGblVars->getRuleNode( RuleId );
#else
    return 0;
#endif

    if (!p->parmType)
        return 0;
    return strlen(p->parmType);
}

void * CMRDataList::MRMemAlloc1(int mSize) {
    char *vPtr;
	char LogMsg[100];
	memset(LogMsg, 0xff, 100);
    if (m_Pos1 + mSize + 8 > (int) m_MaxMemSize1) {
#ifndef _TESTER
        sprintf(LogMsg, "MEMORY_1 RULE[%d] Max size : %d    Alloc size : %d\n",
                m_RuleId, m_MaxMemSize1, m_Pos1 + mSize + 8);
//BRMLIB¿¡¼­´Â Á¦°ÅµÇ¾î¾ßÇÔ.
	#ifndef _BRMLIB
	//      _WriteLog(_LOG_ERR, LogMsg);
	#endif
		throw new CMTException(SYSERR_MEMORYALLOCATEFAILURE, LogMsg);
#else
		throw new CMTException(SYSERR_MEMORYALLOCATEFAILURE, ERRMSG_MEMORYALLOCATEFAILURE_10);
#endif
    }

    vPtr = (char*) pMemory1 + m_Pos1;

    int mm = mSize % 8;

    m_Pos1 += (mSize + 8 - mm);

    return vPtr;
}

void * CMRDataList::MRMemAlloc2(int mSize) {
    char *vPtr;
	char LogMsg[100];
    if (m_Pos2 + mSize + 8 > (int) m_MaxMemSize2) {
#ifndef _TESTER
        sprintf(LogMsg, "MEMORY_2 RULE[%d] Max size : %d    Alloc size : %d\n",
                m_RuleId, m_MaxMemSize2, m_Pos2 + mSize + 8);
//BRMLIB¿¡¼­´Â Á¦°ÅµÇ¾î¾ßÇÔ.
#ifndef _BRMLIB
//      _WriteLog(_LOG_ERR, LogMsg);
#endif
		throw new CMTException(SYSERR_MEMORYALLOCATEFAILURE, LogMsg);
#else
		throw new CMTException(SYSERR_MEMORYALLOCATEFAILURE, ERRMSG_MEMORYALLOCATEFAILURE_11);
#endif
    }

    vPtr = (char*) pMemory2 + m_Pos2;

    int mm = mSize % 8;

    m_Pos2 += (mSize + 8 - mm);

    return vPtr;
}

void * CMRDataList::MRMemAlloc3(int mSize) {

    char *vPtr;
	char LogMsg[100];

    if (m_Pos3 + mSize + 8 > (int) m_MaxMemSize3) {
#ifndef _TESTER
        sprintf(LogMsg, "MEMORY_3 RULE[%d] Max size : %d    Alloc size : %d\n",m_RuleId, m_MaxMemSize3, m_Pos3 + mSize + 8);
//BRMLIB¿¡¼­´Â Á¦°ÅµÇ¾î¾ßÇÔ.
#ifndef _BRMLIB
//      _WriteLog(_LOG_ERR, LogMsg);
#endif
		throw new CMTException(SYSERR_MEMORYALLOCATEFAILURE, LogMsg);//ERRMSG_MEMORYALLOCATEFAILURE_12);
#else
		throw new CMTException(SYSERR_MEMORYALLOCATEFAILURE, ERRMSG_MEMORYALLOCATEFAILURE_12);
#endif
    }

    vPtr = (char*) pMemory3 + m_Pos3;

    int mm = mSize % 8;

    m_Pos3 += (mSize + 8 - mm);

    return vPtr;
}

void CMRDataList::MRMemFree(void *p) {

}

int CMRDataList::getMemPtr2() {
    return m_Pos2;
}

void CMRDataList::setMemPtr2(int pos) {
    m_Pos2 = pos;
}

int CMRDataList::getMemPtr3() {
    return m_Pos3;
}

void CMRDataList::setMemPtr3(int pos) {
    m_Pos3 = pos;
}

#ifndef _PARSER

RULE_LIST * CMRDataList::getRuleNodeById(long ruleId) {
#ifdef  _TESTER
    return pCMRGblVars->DbgLoadRule(ruleId );
#else
    return pCMRGblVars->getRuleNode(ruleId);
#endif
}

RULE_LIST * CMRDataList::getRuleNodeByName(char *ruleName) {
#ifdef  _TESTER
    return pCMRGblVars->DbgLoadRule(ruleName );
#else
    return pCMRGblVars->getRuleNode(ruleName);
#endif
}

#endif

GBL_VALUE_LIST * CMRDataList::addGblValNode(char *Name) {
    GBL_VALUE_LIST *p;

    p = (GBL_VALUE_LIST *) MRMemAlloc1(sizeof(GBL_VALUE_LIST));
    strcpy(p->gblName, Name);
    p->cnt = 0;
    p->Val = (VALUE_LIST *) MRMemAlloc1(sizeof(VALUE_LIST));
    p->Val->pNext = 0;
    p->pNext = m_pGVList;
    m_pGVList = p;

    return p;
}

long CMRDataList::setGblVal(char *name, TYPE_VALUE *tVal, int idx) {
    GBL_VALUE_LIST *p;
    p = m_pGVList;
    while (p) {
			if (strcmp(p->gblName, name) == 0) {
				break;
			}
        p = p->pNext;
    }

    if (!p) {
        p = addGblValNode(name);

        p->cnt = 1;
        p->vType = tVal->dataType;
        memcpy((char*) &p->Val->value, (char*) &tVal->value, sizeof(VALUE_UNION) );
        
    } else {
        if (p->vType != tVal->dataType)
            throw new CMTException(UERR_GBLVAL_TYPE, EMSG_GBLVAL_TYPE_01);
        if (idx > p->cnt)
            throw new CMTException(UERR_GBLVAL_INDEX, EMSG_GBLVAL_INDEX);

        VALUE_LIST *pVal = p->Val;
        for (int i = 1; i < idx; i++)
            pVal = pVal->pNext;

        memcpy((char*) &pVal->value, (char*) &tVal->value, sizeof(VALUE_UNION));
    }

#ifdef  _TESTER
    DbgSetGblVar( p );
    m_pWorker->SuspendWork( _DBG_STEP_SETVAL, 1, p->vType, name );
#endif

    return 1;
}

long CMRDataList::addGblVal(char *name, TYPE_VALUE *tVal) {
    GBL_VALUE_LIST *p;

//  ½ÇÇà½Ã¿¡´Â ¹ß»ý¾ÈÇÔ
//  if ( tVal->dataType != DATA_TYPE_LONG && tVal->dataType != DATA_TYPE_DOUBLE )
//      throw new CMTException( UERR_GBLVAL_TYPE, EMSG_GBLVAL_TYPE );

    p = m_pGVList;
    while (p) {
        if (strcmp(p->gblName, name) == 0)
            break;
        p = p->pNext;
    }

    if (!p) { //  Init
        p = addGblValNode(name);
        p->cnt = 1;
        p->vType = tVal->dataType;
        memcpy((char*) &p->Val->value, (char*) &tVal->value,
                sizeof(VALUE_UNION));

#ifdef  _TESTER
        DbgSetGblVar( p );
        m_pWorker->SuspendWork( _DBG_STEP_SETVAL, 1, p->vType, name );
#endif
        return 1;
    }

    unsigned char t1, t2;

    t1 = p->vType;
    t2 = tVal->dataType;

    if (t1 == DATA_TYPE_DOUBLE) {
        if (t2 == DATA_TYPE_LONG)
            p->Val->value.dblVal += (double) tVal->value.longVal;
        else
            p->Val->value.dblVal += tVal->value.dblVal;
    } else if (t1 == DATA_TYPE_LONG) {
        if (t2 == DATA_TYPE_LONG)
            p->Val->value.longVal += tVal->value.longVal;
        else
            p->Val->value.longVal += (long) tVal->value.dblVal;
    } else
        throw new CMTException(UERR_GBLVAL_TYPE, EMSG_GBLVAL_TYPE_02);

#ifdef  _TESTER
    DbgSetGblVar( p );
    m_pWorker->SuspendWork( _DBG_STEP_SETVAL, 1, p->vType, name );
#endif

    return 1;
}

long CMRDataList::getGblVal(TYPE_VALUE *tVal, int mode, int idx) {
    GBL_VALUE_LIST *p;
    char Name[51];
  char msg[100];
    strcpy(Name, tVal->value.strVal);

    p = m_pGVList;
    while (p) {
		if (strcmp(p->gblName, Name) == 0) {
			break;
		}
        p = p->pNext;
    }

    if (!p) {
#ifdef  _TESTER

        int vType = (mode == TOKENFUNC_GETNVAL) ? 3 : 2;

        m_pWorker->SuspendWork( _DBG_STEP_GETVAL, 0, vType, Name );
        DbgGetGblVar( Name );

        p = m_pGVList;
        while ( p ) {
            if ( strcmp(p->gblName, Name) == 0 ) break;
            p = p->pNext;
        }

        if ( ! p ){
             sprintf(msg,EMSG_GBLVAL_NOTSET,Name);
             throw new CMTException( UERR_GBLVAL_NOTSET, msg );
        }
#else
        if ( ! p ){
             sprintf(msg,EMSG_GBLVAL_NOTSET,Name);
             throw new CMTException( UERR_GBLVAL_NOTSET, msg );
        }
#endif
    }

    /*      2010-2-11 vapor ¸ðµå»ó°ü¾ø°Ô ¸·À½.
     if ( mode == TOKENFUNC_GETNVAL ) {
     if ( p->vType == DATA_TYPE_STRING || p->vType == DATA_TYPE_LSTRING ) {
     throw new CMTException( UERR_GBLVAL_TYPE, EMSG_GBLVAL_TYPE );
     }
     } else {  // TOKENFUNC_GETSVAL
     if ( p->vType != DATA_TYPE_STRING && p->vType != DATA_TYPE_LSTRING ) {
     throw new CMTException( UERR_GBLVAL_TYPE, EMSG_GBLVAL_TYPE );
     }
     }
     */

    if (idx > p->cnt)
        throw new CMTException(UERR_GBLVAL_INDEX, EMSG_GBLVAL_INDEX);

    VALUE_LIST *pVal = p->Val;
    for (int i = 1; i < idx; i++)
        pVal = pVal->pNext;

    tVal->dataType = p->vType;
    memcpy((char*) &tVal->value, (char*) &pVal->value, sizeof(VALUE_UNION));


    return 1;
}

long CMRDataList::getGblValByName(TYPE_VALUE *tVal, char *Name, int mode,
        int idx) {
    GBL_VALUE_LIST *p;
  char msg[100];
    p = m_pGVList;
    while (p) {
        if (strcmp(p->gblName, Name) == 0)
            break;
        p = p->pNext;
    }

        if ( ! p ){
             sprintf(msg,EMSG_GBLVAL_NOTSET,Name);
             throw new CMTException( UERR_GBLVAL_NOTSET, msg );
        }

    /*      2010-2-11 vapor ¸ðµå»ó°ü¾ø°Ô ¸·À½.
     if ( mode == TOKENFUNC_GETNVAL ) {
     if ( p->vType == DATA_TYPE_STRING || p->vType == DATA_TYPE_LSTRING ) {
     throw new CMTException( UERR_GBLVAL_TYPE, EMSG_GBLVAL_TYPE );
     }
     } else {  // TOKENFUNC_GETSVAL
     if ( p->vType != DATA_TYPE_STRING && p->vType != DATA_TYPE_LSTRING ) {
     throw new CMTException( UERR_GBLVAL_TYPE, EMSG_GBLVAL_TYPE );
     }
     }
     */

    if (idx > p->cnt)
        throw new CMTException(UERR_GBLVAL_INDEX, EMSG_GBLVAL_INDEX);

    VALUE_LIST *pVal = p->Val;
    for (int i = 1; i < idx; i++)
        pVal = pVal->pNext;

    tVal->dataType = p->vType;
    memcpy((char*) &tVal->value, (char*) &pVal->value, sizeof(VALUE_UNION));

    return 1;
}

long CMRDataList::incGblVal(char *name) {
    GBL_VALUE_LIST *p;

    p = m_pGVList;
    while (p) {
        if (strcmp(p->gblName, name) == 0)
            break;
        p = p->pNext;
    }

    if (!p) { //  Init
        p = addGblValNode(name);

        p->cnt = 1;
        p->vType = DATA_TYPE_LONG;
        p->Val->value.longVal = 1;
    } else {
        if (p->vType != DATA_TYPE_LONG)
            throw new CMTException(UERR_GBLVAL_TYPE, EMSG_GBLVAL_TYPE_03);
        p->Val->value.longVal++;
    }

#ifdef  _TESTER
    DbgSetGblVar( p );
    m_pWorker->SuspendWork( _DBG_STEP_SETVAL, 1, p->vType, name );
#endif

    return 1;
}

long CMRDataList::pushGblVal(char *name, TYPE_VALUE *tVal) {
    GBL_VALUE_LIST *p;

    p = m_pGVList;
    while (p) {
        if (strcmp(p->gblName, name) == 0)
            break;
        p = p->pNext;
    }

    if (!p) {
        p = addGblValNode(name);
        p->cnt = 1;
        p->vType = tVal->dataType;
        memcpy((char*) &p->Val->value, (char*) &tVal->value,
                sizeof(VALUE_UNION));
    } else {
        if (p->vType != tVal->dataType)
            throw new CMTException(UERR_GBLVAL_TYPE, EMSG_GBLVAL_TYPE_04);

        VALUE_LIST *pVal = p->Val;
        while (pVal->pNext)
            pVal = pVal->pNext;

        pVal->pNext = (VALUE_LIST *) MRMemAlloc1(sizeof(VALUE_LIST));
        pVal = pVal->pNext;
        p->cnt++;
        memcpy((char*) &pVal->value, (char*) &tVal->value, sizeof(VALUE_UNION));
        pVal->pNext = 0;
    }

#ifdef  _TESTER
    DbgSetGblVar( p );
    m_pWorker->SuspendWork( _DBG_STEP_SETVAL, 1, p->vType, name );
#endif

    return 1;
}

long CMRDataList::clearGbl(char *name) {
    GBL_VALUE_LIST *p, *q;

    p = q = m_pGVList;
    while (p) {
        if (strcmp(p->gblName, name) == 0) {
            if (p == m_pGVList)
                m_pGVList = p->pNext;
            else
                q->pNext = p->pNext;
            return 1;
        }
        q = p;
        p = p->pNext;
    }

#ifdef  _TESTER
    m_pWorker->SuspendWork( _DBG_STEP_SETVAL, 2, 0, name );
#endif

    return 1;
}

long CMRDataList::getGblRows(char *name) {
    GBL_VALUE_LIST *p;

    p = m_pGVList;
    while (p) {
        if (strcmp(p->gblName, name) == 0)
            return p->cnt;
        p = p->pNext;
    }

    return 0;
}

bool CMRDataList::findGblVal(char *Name, TYPE_VALUE *tVal) {
    GBL_VALUE_LIST *p;
    VALUE_LIST *v;
    char *s1, *s2;

    p = m_pGVList;
    while (p) {
        if (strcmp(p->gblName, Name) == 0)
            break;
        p = p->pNext;
    }

    if (!p)
        return false;

    if (p->vType != DATA_TYPE_STRING && p->vType != DATA_TYPE_LSTRING) {
        throw new CMTException(UERR_GBLVAL_TYPE, EMSG_GBLVAL_TYPE_05);
    }

    switch (tVal->dataType) {
    case DATA_TYPE_STRING:
        s1 = tVal->value.strVal;
        break;
    case DATA_TYPE_LSTRING:
        s1 = tVal->value.pLStr;
        break;
    default:
        throw new CMTException(UERR_GBLVAL_TYPE, EMSG_GBLVAL_TYPE_06);
    }

    v = p->Val;
    while (v) {
        if (p->vType == DATA_TYPE_STRING)
            s2 = v->value.strVal;
        else
            s2 = v->value.pLStr;

        if (strcmp(s1, s2) == 0)
            return true;

        v = v->pNext;
    }

    return false;
}

int CMRDataList::usedRuleCleared() {
    return m_usedRuleClear;
}  

 

void CMRDataList::BuildQueryString(QRULE_DATA *pQRule, char *Val[],
        char *NewQuery, char mode) {
    QBRULE_SUBSTR *pLast;
    int i, j, k;
    char *p;

	if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]\n", __FILE__, __LINE__);

//printf("BuildQueryString [%c]\n", mode );

    pLast = pQRule->pBindValue;
    i = j = k = 0;
    while (pLast) {
        while (j < pLast->pos) {
            NewQuery[i++] = pQRule->QueryStatement[j++];
        }
        if (mode == '1') { //  CRUDÀÏ¶§
            p = Val[k];

            NewQuery[i++] = '\'';
            while (*p) {
                if (*p == '\'') {
                    NewQuery[i++] = '\'';
                    NewQuery[i++] = '\'';
                } else {
                    NewQuery[i++] = *p;

                }

                p++;
            }

            NewQuery[i++] = '\'';
        } else { //  STATEMENTÀÏ¶§ 

/*          sprintf(NewQuery + i, "'%s'", Val[k]);
            i += strlen(Val[k]) + 2;
*/
//          sprintf(NewQuery + i, "'%s'", Val[k]); 2014.10.14 ¹®ÀÚ¿­ ¹è¿­ Ã³¸®½Ã ¿À·ù ¼öÁ¤.
            if('\''==Val[k][0]){
                sprintf(NewQuery + i, "%s", Val[k]);
            i += strlen(Val[k]) ;
            }else{
                sprintf(NewQuery + i, "'%s'", Val[k]);
            i += strlen(Val[k]) + 2;
            }
        }

        j += 4;
        k++;

        pLast = pLast->pNext;
    }
//printf("NewQuery [%s]\n", NewQuery );
	if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]\n", __FILE__, __LINE__);
	if (_DEBUG_YN[0] == 'Y') printf("pQRule->QueryStatement   [%s] \n", pQRule->QueryStatement);
	if (_DEBUG_YN[0] == 'Y') printf(" NewQuery [%s] \n", NewQuery);

	if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]\n", __FILE__, __LINE__);
	strcpy(NewQuery + i, pQRule->QueryStatement + j);
	if (_DEBUG_YN[0] == 'Y') printf("[%s][%d]\n", __FILE__, __LINE__);

} 

/******************************************************************/

#ifdef  _TRACE

extern CBRTrace *g_pCBRTrace;

bool CMRDataList::InitTrace()
{
    char fName[300];
    char tempName[200];
    char sysRun[500];
    int len;
//    if( strcmp(m_PgmId,"RIFA0001")==0) return false;
//    if( strcmp(m_PgmId,"RIFA0004")==0) return false;
//    if( strcmp(m_PgmId,"RIFA0005")==0) return false;
        
    if ( ! g_pCBRTrace->isTrace( m_PgmId, this, m_TraceID   ) ) return false;

    m_TraceFlag = true;

    strcpy( tempName, configRec.lpszTrcPath );

    len = strlen( tempName );
    if ( tempName[len-1] != PATH_DELIM ) {
        tempName[len] = PATH_DELIM;
        tempName[len+1] = 0;
    }
 		sprintf( fName, "%s%s_%s.trc", tempName,m_PgmId , m_TraceID ); 
 	
 
//    sprintf( fName, "%s%s_%s.trc", tempName, m_TraceID, m_PgmId );
/*
#ifndef WIN32
    sprintf(sysRun,"find %s/*.trc  -mtime +7 -ls | xargs rm -f ",tempName);
    sytem(sysRun);
#endif
*/

    if ( (fp_trc = fopen(fName,"w+")) == NULL ) {
        m_TraceFlag = false;

        _WriteLog( _LOG_ERR, MSG_NOTEXIST_TRACEOUTPATH );

        return false;
    }

    return true;
}

void CMRDataList::setStartIp( char *ip )
{
		if( ip ){
			sprintf(m_StartIP,"%15s",ip);
		}
}

void CMRDataList::TermTrace()
{
    if ( m_TraceFlag ) {
        fclose( fp_trc );

        if ( ! configRec.usTrace ) SaveTraceInfo( m_TraceID, m_PgmId );

        m_TraceFlag = 0;
    }
}

unsigned char CMRDataList::getTraceFlag()
{
    return m_TraceFlag;
}

void CMRDataList::TraceOut( char *msg )
{ 
    if ( m_TraceFlag ) fprintf( fp_trc, "%s\n", msg );
}
//startip
void CMRDataList::TraceOutIP( char *msg )
{ 
    if ( m_TraceFlag ){
		if( m_StartIP ){
			fprintf( fp_trc, "[%15s]%s\n",m_StartIP, msg );
		}else{
			fprintf( fp_trc, "NULL:%s\n", msg );
		}
	}
}

void CMRDataList::LogItemValues()
{
    char msg[CONDITION_STR_SIZE];
    char sType[10];

    if ( ! m_TraceFlag ) return;

    TraceOut( "EXTERNAL VARIABLE BEGIN" );

    for ( int i=0; i<m_itemCnt; i++ ) {
        switch ( m_itemValue[i].itemType ) {
            case DATA_TYPE_LONG :
            strcpy( sType, "INTEGER" );
            break;
            case DATA_TYPE_DOUBLE :
            strcpy( sType, "FLOAT" );
            break;
            default :
            strcpy( sType, "STRING" );
            break;
        }

        sprintf(msg, "   Code=%-4d   Type=%s    Occ=%d",
                m_itemValue[i].itemCode, sType, m_itemValue[i].itemOcc );
        TraceOut( msg );

        for ( int j=0; j<m_itemValue[i].itemOcc; j++ ) {
            switch ( m_itemValue[i].itemType ) {
                case DATA_TYPE_LONG :
                sprintf(msg, "     %d. [%ld]", j+1, m_itemValue[i].mValue[j].longVal );
                break;
                case DATA_TYPE_STRING :
                sprintf(msg, "     %d. [%s]", j+1, m_itemValue[i].mValue[j].strVal );
                break;
                case DATA_TYPE_DOUBLE :
                sprintf(msg, "     %d. [%lf]", j+1, m_itemValue[i].mValue[j].dblVal );
                break;
                case DATA_TYPE_LSTRING :
                sprintf(msg, "     %d. [%s]", j+1, m_itemValue[i].mValue[j].pLStr );
                break;
            }

            TraceOut( msg );
        }
    }

    TraceOut( "EXTERNAL VARIABLE END" );
}
#endif

#ifdef _TESTER

void CMRDataList::DbgRuleReturn( RULE_LIST *pRuleNode, RESULT_REC *pResultRec, char *sParm )
{
    RESULT_REC *pRec;
    int ResultRowCnt = 0;

    pRec = pResultRec;
    while ( pRec ) {
        ResultRowCnt++;
        pRec = pRec->pNext;
    }
    BRFWrite Writer( _FILE_DATA );

    Writer.setString( sParm );

    Writer.setNumber( pRuleNode->retCnt );
    Writer.setNumber( ResultRowCnt );

    pRec = pResultRec;

    VALUE_UNION *p;

    while ( pRec ) {
        for ( int i=0; i<pRuleNode->retCnt; i++ ) {
            p = &pRec->value[i];

            switch ( pRuleNode->retType[i] ) {
                case DATA_TYPE_LONG :
                Writer.setNumber( p->longVal );
                break;
                case DATA_TYPE_STRING :
                Writer.setString( p->strVal );
                break;
                case DATA_TYPE_LSTRING :
                Writer.setString( p->pLStr );
                break;
                case DATA_TYPE_DOUBLE :
                Writer.setDouble( p->dblVal );
                break;
                case DATA_TYPE_BOOL :
                Writer.setNumber( p->boolVal );
                break;
                default :
                Writer.setString( "" );
                break;
            }
        }
        pRec = pRec->pNext;
    }
}

void CMRDataList::DbgQueryReturn( char *Qry )
{
    BRFWrite Writer( _FILE_DATA );

    Writer.setString( Qry );

}

void CMRDataList::DbgClearFile()
{

    BRFWrite Writer( _FILE_DATA );

    Writer.setNumber( 0 ); //   p->cnt
}

void CMRDataList::DbgSetGblVar( GBL_VALUE_LIST *p )
{

    //  ÀÏ´Ü ÇÏ³ª¸¸ Ã³¸®ÇÏÁö¸¸ ÈÄ¿¡ ArrayÃ³¸®ÇÏµµ·Ï ¹Ù²ã¾ß ÇÒµí...
    BRFWrite Writer( _FILE_DATA );

    Writer.setNumber( 1 );//   p->cnt

    switch( p->vType ) {
        case DATA_TYPE_LONG :
        Writer.setNumber( p->Val->value.longVal );
        break;
        case DATA_TYPE_STRING :
        Writer.setString( p->Val->value.strVal );
        break;
        case DATA_TYPE_DOUBLE :
        Writer.setDouble( p->Val->value.dblVal );
        break;
        case DATA_TYPE_BOOL :
        Writer.setNumber( p->Val->value.boolVal );
        break;
        default :
        Writer.setString( "" );
        break;
    }

}

void CMRDataList::DbgGetGblVar( char *Name )
{
    TYPE_VALUE tVal;
    char tempStr[300];
    int vCnt;

    //  ÀÏ´Ü ÇÏ³ª¸¸ Ã³¸®ÇÏÁö¸¸ ÈÄ¿¡ ArrayÃ³¸®ÇÏµµ·Ï ¹Ù²ã¾ß ÇÒµí...
    {
        BRFRead Reader( _FILE_DATA );

        tVal.dataType = Reader.getNumber(); //  Type

        vCnt = Reader.getNumber();//  value cnt

        if ( vCnt == 0 ) return;

//      for ( int k=0; k<vCnt; k++ ) {
        strcpy( tempStr, Reader.getString() );
        printf("GBL type[%d] cnt[%d] val[%s]\n", tVal.dataType, vCnt, tempStr );

        switch ( tVal.dataType ) {
            case DATA_TYPE_DOUBLE :
            tVal.value.dblVal = atof( tempStr );
            break;

            case DATA_TYPE_LONG :
			//modified by DSKIM 2016.01.13
			//CR8082 [Å×ÀÌºí·ê] IntegerÇüÀÇ Àü¿ªº¯¼ö¸¦ »ç¿ëÇÏ´Â °æ¿ì µð¹ö±× ½Ã Àü¿ªº¯¼öÀÇ °ªÀÌ 0À¸·Î ³Ñ¾î°¨ 
            //tVal.value.dblVal = atoi( tempStr );
			tVal.value.longVal = atoi( tempStr );
            break;

            case DATA_TYPE_STRING :
            strcpy( tVal.value.strVal, tempStr );
            break;

            case DATA_TYPE_BOOL :
            tVal.value.boolVal = atoi( tempStr );
            break;

            default :
            strcpy( tVal.value.strVal, tempStr );
            break;
        }

//          if ( k == 0 ) setGblVal( Name, &tVal, 1 );
//          else pushGblVal( Name, &tVal );
//      }
    }

    setGblVal( Name, &tVal, 1 );

}

#else

void CMRDataList::LogRuleReturn(RULE_LIST *pRuleNode, RESULT_REC *pResultRec) {
#ifdef _TRACE

    RESULT_REC *pRec;
    int ResultRowCnt = 0;

    pRec = pResultRec;
    while ( pRec ) {
        ResultRowCnt++;
        pRec = pRec->pNext;
    }

    sprintf(m_TraceMsg, "\n   RETURN BEGIN ( %d rows )", ResultRowCnt );
    TraceOut( m_TraceMsg );

    pRec = pResultRec;

    VALUE_UNION *p;

    while ( pRec ) {
        for ( int i=0; i<pRuleNode->retCnt; i++ ) {
            p = &pRec->value[i];
    	 
            switch ( pRuleNode->retType[i] ) {
                case DATA_TYPE_LONG :
	                sprintf(m_TraceMsg, "     NUM: %ld", p->longVal );
	                TraceOut( m_TraceMsg );
								break;
                case DATA_TYPE_STRING :
	                sprintf(m_TraceMsg, "     STR: %s", p->strVal );
	                TraceOut( m_TraceMsg );
                break;
                case DATA_TYPE_LSTRING :
	                sprintf(m_TraceMsg, "    LSTR: %s", p->pLStr );
	                TraceOut( m_TraceMsg );

                break;
                case DATA_TYPE_DOUBLE :
	                sprintf(m_TraceMsg, "     DBL: %lf", p->dblVal );
	                TraceOut( m_TraceMsg );
                break;
                case DATA_TYPE_BOOL :
	                sprintf(m_TraceMsg, "     BOO: %ld", p->boolVal );
	                TraceOut( m_TraceMsg );
                break;
                default :
	                TraceOut( "     Result Type Error." );
                break;
            }
        }
        pRec = pRec->pNext;

        if ( pRec ) TraceOut( "" );
    }

    TraceOut( "   RETURN END" );

#endif

}
#endif

#if defined(_OS390)

RESULT_REC* CMRDataList::ExecuteDynamicSql( RULE_LIST *pRuleNode,
        char *DBName, char flagNextPage,
        long maxRows, char *QS )
{
    return 0;
}

#endif

#if defined(_TESTER)

RESULT_REC* CMRDataList::ExecuteDynamicSql( RULE_LIST *pRuleNode, QRULE_DATA *pQRule, char *sVal[] )
//                               short gbn, RULE_LIST *pRuleNode,
//                                 char *DBName, char *QS )
{
    RESULT_REC *pResult;
    char QueryString[MAX_QUERY_SIZE];

    BuildQueryString( pQRule, sVal, QueryString, '2' );

    DbgQueryReturn( QueryString );
    m_pWorker->SuspendWork( _DBG_STEP_QRY, 0, 0, "" );

    if ( pRuleNode->s_type == QR_TP_CUD )
    pResult = ExecuteDynamicSqlCUDProc( pQRule->DBName, QueryString );
    else
    pResult = ExecuteDynamicSqlProc( pRuleNode, pQRule->DBName, QueryString );

    return pResult;
}

RESULT_REC* CMRDataList::ExecuteDynamicSqlProc( RULE_LIST *pRuleNode, char *DBName, char *QS )
{
    int rc;
    char msg[512]; 

    RESULT_REC *pResultList = 0, *pLastResult, *pNewResult;
    VALUE_UNION Value;
    char resString[MAX_QUERY_SIZE];
    int i;
    int len;

    DbgQueryRun( DBName, QS );
    m_pWorker->SuspendWork( _DBG_STEP_QRUN_CSR, 0, 0, "" );

    BRFRead Reader( _FILE_DATA );

    rc = Reader.getNumber();

    if ( rc != 0 ) {
        strcpy( msg, Reader.getString() );
        throw new CMTException( rc, msg );
    }

    int rows = Reader.getNumber();
    int cols = Reader.getNumber();

    if ( cols != pRuleNode->retCnt ) {
        throw new CMTException( UERR_RESULTCOUNT, EMSG_RESULTCOUNT );
    }

//  for( i = 0; i < 100; i++ )
    for( i = 0; i < rows; i++ )
    {
        if ( i >= _MAX_QRY_COUNT ) break;

        pNewResult = (RESULT_REC *) MRMemAlloc1( sizeof(RESULT_REC) );
        pNewResult->value = (VALUE_UNION *) MRMemAlloc1(sizeof(VALUE_UNION) * cols);
        pNewResult->pNext = 0;

        for( int j = 0; j < pRuleNode->retCnt; j++ )
        {
            strcpy( resString, Reader.getString() );
            len = strlen(resString);

            switch( pRuleNode->retType[j] ) {
                case TOKENTYPE_CONST_F:
                Value.dblVal = atof( resString );
                break;
                case TOKENTYPE_CONST_C:
		               	memset(Value.strVal,0x00, STR_VALUE_SIZE );
		                if ( len == 0 )
		                // strcpy( Value.strVal, " " );
		                Value.strVal[0] = 0;
		                else {
		                    strncpy( Value.strVal, resString, STR_VALUE_SIZE );
		                    Value.strVal[STR_VALUE_SIZE] = 0;
		                }
                break;
                case TOKENTYPE_CONST_N:
                Value.longVal = atol( resString );
                break;
                case TOKENTYPE_CONST_L:
//                  Value.LString.len = tOff;
                Value.pLStr = (char*) MRMemAlloc1( len+1 );
                strcpy( Value.pLStr, resString );
                break;
                case TOKENTYPE_CONST_B:
                Value.boolVal = (unsigned char) atol( resString );
                break;
            }

            memcpy ( (char *) & pNewResult->value[j],
                    (char *) & Value, sizeof(VALUE_UNION) );
        }

        if ( ! pResultList ) pResultList = pNewResult;
        else pLastResult->pNext = pNewResult;

        pLastResult = pNewResult;

    }

    return pResultList;
}

RESULT_REC* CMRDataList::ExecuteDynamicSqlCUDProc( char *DBName, char *QS )
{
    RESULT_REC *pResultList = 0;
    char msg[256];
    int rc;

    DbgQueryRun( DBName, QS );
    m_pWorker->SuspendWork( _DBG_STEP_QRUN_CUD, 0, 0, "" );

    BRFRead Reader( _FILE_DATA );

    rc = Reader.getNumber();

    if ( rc < 0 || rc > 2) {
        strcpy( msg, Reader.getString() );
        throw new CMTException( rc, msg );
    }

    try
    {
        pResultList = (RESULT_REC *) MRMemAlloc1( sizeof(RESULT_REC) );
        pResultList->value = (VALUE_UNION *) MRMemAlloc1( sizeof(VALUE_UNION) );
        pResultList->value->longVal = rc;
        pResultList->pNext = 0;
    }
    catch( CMTException *e )
    {
        rc = e->GetECode();
        strcpy ( msg, (char *) e->GetEMessage() );
        delete e;

        throw new CMTException( rc, msg );
    }

    return pResultList;
}

void CMRDataList::DbgQueryRun( char *dbn, char *Qry )
{
    BRFWrite Writer( _FILE_DATA );

//  Writer.setNumber( m_RuleId );
//  Writer.setString( dbn );
    Writer.setString( Qry );
}




#endif
