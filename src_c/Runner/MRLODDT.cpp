#if	defined(WIN32) 
#define		KEY_FILE	".\\key.dat"	   
// #define		KEY_FILE	"E:\\00000000000000\\Bin\\key.dat"	   
#else
#define		KEY_FILE	"./key.dat"
#endif
#include    "MItem.h"
#include    <stdio.h>                                                
#include    <stdlib.h>
#include    <ctype.h>
#include    <string.h>

#include    "MRCSComm.h"
#include    "MRTOKEN.h"
#include    "MRSTRUCT.h"
#include    "MRPARSE.h"
#include    "MRDEFINE.h"
#include    "MRDTypes.h"

#ifdef _OS390
#include    "MRDTLIST.h"
#endif

#ifdef _TESTER
#include    "BRMLoad.h"
#else
#include    "MRLODRUL.h"
#endif

#include    "MRERROR.h"
#include    "MTSysMsg.h"
#include    "MTExcept.h"
#include    "MTUtil.h"
#include    "MRGBLVAR.h"

#ifdef _MSG_KO
#define   MSG_RULE_PARSING_ERR	 "猷�(%d) : �빐�꽍 �삤瑜�.  %s\n"
#define   MSG_RULE_DATATYPE_ERR  "猷�(%d) : �뜲�씠�꽣 �삎�깭 �삤瑜�.  %s\n"
#define   MSG_RULE_PARSE_ERR1	 "猷� �빐�꽍 �삤瑜� : %s\n"
#define   MSG_RULE_PARSE_ERR2	 "猷� �빐�꽍 �삤瑜� : %d %s\n"
#define   MSG_RULE_PARSE_ERR3	 "猷� �빐�꽍 �삤瑜�.\n"
#else
#define   MSG_RULE_PARSING_ERR   "Rule(%d) : Parsing error.  %s\n"
#define   MSG_RULE_DATATYPE_ERR  "Rule(%d) : Data type error.  %s\n"
#define   MSG_RULE_PARSE_ERR1    "Rule Parse Error : %s\n"
#define   MSG_RULE_PARSE_ERR2    "Rule Parse Error : %d %s\n"
#define   MSG_RULE_PARSE_ERR3    "Rule Parse Error.\n"
#endif

#ifndef _TESTER

extern CONFIG_REC configRec; //added by DSKIM 2016.11.23

//2011-01-21 vapor decode  
void CMRLoadData::DecodeString( char * enc , char * lpszVal)
{ 
	//modified by DSKIM 2016.11.23: AES256 �쟻�슜
	/*
    int dec_size;
    unsigned char *tempDec;
    strcpy(dec , enc);
 
    tempDec = Base64Decode((unsigned char *)enc,strlen((const char*)enc), &dec_size);
    if(dec_size == 0){
        strcpy(dec ,(char*) dec);
    }
    if( tempDec!=NULL ){
        strcpy(dec ,(char*) tempDec);
        free(tempDec);
    }
	*/
	int dec_size;
	unsigned char *tempDec;
	memset(lpszVal,0x00,sizeof(lpszVal));
	strcpy(lpszVal , enc);

  if( configRec.lpszEncMethod[0] == 'A') {
		FILE *fp;
		fp = fopen(KEY_FILE, "r");
		char tempKey[201];
		memset( tempKey , 0x00, sizeof(tempKey ));
		if(!fp == NULL) {
			fscanf(fp, "%s", &tempKey);
		}
		fclose(fp);
		int tempSize;
		unsigned char *key;
//		unsigned char *key = Base64Decode((unsigned char *)tempKey ,strlen(tempKey), &tempSize);
		unsigned char iv[17];
        memset( iv , 0x00, sizeof(iv ));
		strncpy((char *)iv, (char *)key, 16 );

//		EVP_CIPHER_CTX en , de  ;

//		if( strcmp(configRec.lpszEncMethod,"AES256")==0 ){
//			aes_init256((unsigned char *)key, (unsigned char *)iv, &en, &de);
//		}else if( strcmp(configRec.lpszEncMethod,"AES128")==0 ){
//			aes_init128((unsigned char *)key, (unsigned char *)iv, &en, &de);
//		}

//		tempDec = Base64Decode((unsigned char*) lpszVal,strlen(lpszVal), &dec_size);
//printf("Prfile 01 enc[%s] [%s ][%s]\n ",enc, lpszVal, tempDec);		
		int len = dec_size;
//		tempDec = (unsigned char *)aes_decrypt(&de, tempDec, &len);
//printf("Prfile 02 [%s]\n ", tempDec);		
	} else if ( strcmp(configRec.lpszEncMethod,"BASE64")==0) {
//		tempDec = Base64Decode((unsigned char *)enc,strlen((const char*)enc), &dec_size);
	}
	/*
	if(configRec.lpszEncMethod[0] == 'A') {
//	tempDec = Base64Decode((unsigned char *)enc,strlen((const char*)enc), &dec_size);
		FILE *fp;
		fp = fopen(KEY_FILE, "r");
		char tempKey[201];
		memset( tempKey , 0x00, sizeof(tempKey ));
		if(!fp == NULL) {
			fscanf(fp, "%s", &tempKey);
		}
		fclose(fp);
    printf("tempkey[dec:%s][enc:%s][%s]\n",dec,enc, tempKey );
     
		int tempSize;
		unsigned char *key = Base64Decode((unsigned char *)tempKey ,strlen(tempKey), &tempSize);
		unsigned char iv[17];
		memset( iv , 0x00, sizeof(iv ));
		strncpy((char *)iv, (char *)key, 16);

		EVP_CIPHER_CTX en, de;
		aes_init((unsigned char *)key, (unsigned char *)iv, &en, &de);
		tempDec = Base64Decode((unsigned char*) enc,strlen((const char*)enc), &dec_size);
		
		int len = dec_size;
		tempDec = (unsigned char *)aes_decrypt(&de, tempDec, &len);
   printf("( AES [%s] [%s] )\n",dec,tempDec);

	} else if (configRec.lpszEncMethod[0] == 'B') {
		tempDec = Base64Decode((unsigned char *)enc,strlen((const char*)enc), &dec_size);
   printf("BAS [%s] [%s] \n",dec,tempDec);
	} 
*/
	if(dec_size == 0){
			strcpy(lpszVal ,(char*) lpszVal);
	}
	if( tempDec!=NULL ){
		strcpy(lpszVal ,(char*) tempDec);
		free(tempDec);
	}//
}
#endif
VALUE_UNION * CMRLoadData::buildDefaultResult(char *pExpress, RULE_LIST *pRule,
        int *gbn) {
    CMRParser Parsing;
    CRULE_STMT *pStmt;

    if ((pStmt = Parsing.ParseResult(pExpress)) == (CRULE_STMT *) -1) {
        *gbn = -1;
        return (VALUE_UNION *) 0;
    }

    VALUE_UNION *pResValue;
    pResValue = buildResultRec((CRULE_STMT*) pStmt, pRule, gbn);

    if (*gbn < 0)
        return (VALUE_UNION *) 0;

    if (*gbn == 0) { //  猷�,�븘�씠�뀥�씠 �룷�븿�맂 援ъ꽦
        return (VALUE_UNION *) pStmt;
    }

    return pResValue;

    /*  MEMORY Free�븯吏� �븡�븘�룄 遺��떞 �뾾�쓬

     CRULE_STMT *p, *q;
     int len;

     p = pStmt;
     while ( p ) {
     q = p->pNext;

     if ( (q->tokType == TOKENTYPE_CONST_C ||
     q->tokType == TOKENTYPE_CONST_L )
     && q->uni.tokInfo ) {
     len = strlen((char*) q->uni.tokInfo);

     pCMRGblVars->MetisFree( q->uni.tokInfo, len );
     }

     q = p->pExpression;
     pCMRGblVars->MetisFree( p, sizeof(CRULE_STMT) );
     p = q;
     }
     */
}

VALUE_UNION * CMRLoadData::buildResultRec(CRULE_STMT *pStmt, RULE_LIST *pRule,
        int *mode) {
    VALUE_UNION *pResValue;

    *mode = getResulRecType(pStmt, pRule);

//printf("buildResultRec a: %d\n", *mode );

    if (*mode <= 0)
        return NULL;

    if (*mode == 1) {
        pResValue = buildResultSingleRec(pStmt, pRule, mode);
    } else if (*mode == 2) {
        pResValue = (VALUE_UNION *) buildResultArrayRec(pStmt, pRule);
        if (!pResValue)
            *mode = -1;
    } else {
        pResValue = (VALUE_UNION *) buildResultArray2Rec(pStmt, pRule);
        if (!pResValue)
            *mode = -1;
    }

    return pResValue;
}

//  0:Rule,Item�룷�븿  1:�긽�닔Only  2:Array  3:2李⑥썝Array(�썑�뿉 2濡쒕컮��)  -1:Error
int CMRLoadData::getResulRecType(CRULE_STMT *pStmt, RULE_LIST *pRule) {
    CRULE_STMT *p;
    bool bCheck = 1;

    //  2 expression�씠�긽.   �긽�닔�삎�씤吏� 泥댄겕
    p = pStmt;
    while (p) {
        if (!p->pNext)
            return -1;

        if (p->pNext->tokType > TOKENTYPE_CONST_B || p->pNext->pNext) {
            bCheck = 0;
            break;
        }

        p = p->pExpression;
    }

    if (bCheck)
        return 1;

    int nLeftP = 0, nRightP = 0;
    bool bArray = 1;

    p = pStmt;
    if (p->pNext->tokType != TOKENTYPE_LEFTP)
        return 0;

    p = pStmt->pNext->pNext;
    while (p) {
        if (p->pExpression)
            return 0;

        if (p->tokType <= TOKENTYPE_CONST_B) {
            p = p->pNext;
            continue;
        }

        switch (p->tokType) {
        case TOKENTYPE_COMMA:
            break;
        case TOKENTYPE_LEFTP:
            nLeftP++;
            break;
        case TOKENTYPE_RIGHTP:
            if (p->pNext)
                nRightP++;
            break;
        default:
            bArray = 0;
            break; //  湲고� �넗�겙�씠硫� Array�삎 �븘�떂
        }

        if (!bArray)
            return 0;

        p = p->pNext;
    }

    if (nLeftP != nRightP)
        return -1;

    if (nLeftP == 0)
        return 2;

    return 3;
}

int CMRLoadData::setResultValue(int retType, CRULE_STMT *p, VALUE_UNION *val) {
    switch (retType) {
    case TOKENTYPE_CONST_F:
        if (p->tokType == TOKENTYPE_CONST_N){
            val->dblVal = p->uni.longVal;
		}else if (p->tokType == TOKENTYPE_CONST_F){
            val->dblVal = p->uni.dblVal;
		}else
            val->dblVal = 0.0;
        break;
    case TOKENTYPE_CONST_C:
        // 2010-4-15 vapor �삎泥댄겕 �븞�븿.
        //if ( p->tokType != TOKENTYPE_CONST_C && p->tokType != TOKENTYPE_CONST_L ) return -1;
				memset( val->strVal,0x00, STR_VALUE_SIZE );
        if (p->uni.tokInfo) {
            strncpy(val->strVal, (char *) p->uni.tokInfo, STR_VALUE_SIZE);
        }
        val->strVal[STR_VALUE_SIZE] = 0;
        break;

    case TOKENTYPE_CONST_L:
        // 2010-4-15 vapor �삎泥댄겕 �븞�븿.
        //if ( p->tokType != TOKENTYPE_CONST_C && p->tokType != TOKENTYPE_CONST_L ) return -1;

        if (p->uni.tokInfo) {
            val->pLStr = (char *) pCMRGblVars->MetisAlloc(
                    strlen((char *) p->uni.tokInfo) + 1);
            strcpy(val->pLStr, (char *) p->uni.tokInfo);
        } else {
            val->pLStr = 0;
        }
        break;

    case TOKENTYPE_CONST_N:
        if (p->tokType == TOKENTYPE_CONST_F){
            val->longVal = (long) p->uni.dblVal;
        }else if (p->tokType == TOKENTYPE_CONST_N)
            val->longVal = p->uni.longVal;
        else
            val->longVal = 0;

        break;

    case TOKENTYPE_CONST_B:
        if (p->tokType == TOKENTYPE_CONST_B)
            val->boolVal = p->uni.boolVal;
        else {
            if (p->uni.longVal == 0)
                val->boolVal = false;
            else
                val->boolVal = true;
        }

        break;
    }

    return 0;
}

RESULT_REC * CMRLoadData::buildResultArrayRec(CRULE_STMT *pStmt,
        RULE_LIST *pRule) {
    CRULE_STMT *p;
    RESULT_REC *pValHead, *pValLast;
//  VALUE_UNION *pResValue;

    short retCnt = pRule->retCnt;

    if (retCnt != 1)
        return NULL;

    pValHead = pValLast = 0;

    p = pStmt->pNext->pNext;

    while (p) {
        if (p->tokType > TOKENTYPE_CONST_B)
            return NULL;

        if (!pValHead) {
            pValHead = pValLast = (RESULT_REC *) pCMRGblVars->MetisAlloc(sizeof(RESULT_REC));
        } else {
            pValLast->pNext = (RESULT_REC *) pCMRGblVars->MetisAlloc(sizeof(RESULT_REC));
            pValLast = pValLast->pNext;
        }
        pValLast->value = (VALUE_UNION *) pCMRGblVars->MetisAlloc(sizeof(VALUE_UNION));

        pValLast->pNext = 0;

        if (setResultValue(pRule->retType[0], p, pValLast->value) != 0)
            return NULL;

        p = p->pNext;
        if (!p)
            return NULL;

        if (p->tokType == TOKENTYPE_RIGHTP)
            break;

        if (p->tokType != TOKENTYPE_COMMA)
            return NULL;

        p = p->pNext;
    }

    if (!p)
        return NULL;

    return pValHead;
}

RESULT_REC * CMRLoadData::buildResultArray2Rec(CRULE_STMT *pStmt,
        RULE_LIST *pRule) {
    return NULL;
}

VALUE_UNION * CMRLoadData::buildResultSingleRec(CRULE_STMT *pStmt,
        RULE_LIST *pRule, int *mode) {
    CRULE_STMT *p, *q;
    VALUE_UNION *pResValue;
    short retCnt = pRule->retCnt;
    int sz, i;

    sz = sizeof(VALUE_UNION) * retCnt;
    pResValue = (VALUE_UNION *) pCMRGblVars->MetisAlloc(sz);

    i = 0;
    q = pStmt;
    while (q) {
        p = q->pNext;

        if (setResultValue(pRule->retType[i], p, &pResValue[i]) != 0) {
            pCMRGblVars->MetisFree(pResValue, sz);
            *mode = -1;
            return NULL;
        }

        q = q->pExpression;
        i++;
    }

    return pResValue;
}

//******************************************************************

VALUE_UNION * CMRLoadData::buildTRuleResultRec(RULE_LIST *pRule, char *sBuf) {
    CMRParser Parsing;
    CRULE_STMT *p;

    if (!sBuf[0]) {
        sBuf[0] = 1;
        return (VALUE_UNION*) 0;
    }
	if ((p = Parsing.ParseResult(sBuf)) == (CRULE_STMT *) -1) {
        //printf("Rule(%d) : Parsing error.  %s\n", pRule->rule_id, sBuf);
        printf(MSG_RULE_PARSING_ERR, pRule->rule_id, sBuf);
        return (VALUE_UNION*) -1;
    }

    int mode;
    VALUE_UNION *pResValue;
    pResValue = buildResultRec(p, pRule, &mode);

    if (mode < 0) {
        //printf("Rule(%d) : Data type error.  %s\n", pRule->rule_id, sBuf);
        printf(MSG_RULE_DATATYPE_ERR, pRule->rule_id, sBuf);
        return (VALUE_UNION*) -1;
    }

    if (mode > 0) {
        CRULE_STMT *q;
        int len;

        while (p) {
            q = p->pNext;
            if ((q->tokType == TOKENTYPE_CONST_C|| q->tokType == TOKENTYPE_CONST_L) && q->uni.tokInfo) {
                len = strlen((char*) q->uni.tokInfo);
                pCMRGblVars->MetisFree(q->uni.tokInfo, len);
            }

            q = p->pExpression;
            pCMRGblVars->MetisFree(p, sizeof(CRULE_STMT));
            p = q;
        }

        sBuf[0] = mode;
        return pResValue;
    } else {
        sBuf[0] = 0;
        return (VALUE_UNION*) p;
    }
}

int CMRLoadData::getItemCd(char **pS, char *subStr) {
    int itemCd, i;
    char temp[2000];

    subStr[0] = 0;

    (*pS)++;
    i = 0;
    while (**pS != ITEM_DELIMITER_RIGHT) {
        if (!**pS)
            return 0;

        temp[i++] = **pS;
        (*pS)++;
    }

    temp[i] = 0;
    itemCd = atoi(temp);

    (*pS)++;

//  strcpy( temp, *pS );

    if (**pS != '(')
        return itemCd;

    (*pS)++;
    while (**pS == ' ')
        (*pS)++;

    i = 0;
    while (**pS != ')') {
        if (!**pS)
            return 0;

        temp[i++] = **pS;
        (*pS)++;
    }
    temp[i] = 0;
    (*pS)++;

    if (temp[0] == '#') {
        strncpy(subStr, temp, 2);
        subStr[2] = 0;
    } else {
        sprintf(subStr, "%d", atoi(temp));
    }

    return (itemCd);
}

int CMRLoadData::getParmCd(char **pS) {
    int i = 0;
    char temp[101];

    (*pS)++;
    while (**pS != LOCAL_DELIMITER_RIGHT) {
        temp[i++] = **pS;

        (*pS)++;
    }
    temp[i] = 0;

    if (!**pS)
        return 0;

    (*pS)++;

    return (atoi(temp));
}

int CMRLoadData::getRuleCd(char **pS, char *subStr) {
    int i;
    char temp[101];
    int ruleId;

    subStr[0] = 0;

    (*pS)++;
    i = 0;
    while (**pS && **pS != ':') {
        temp[i++] = **pS;
        (*pS)++;
    }
    temp[i] = 0;

    if (**pS == 0) {
        //printf("Rule Parse Error : %s\n", temp);
        printf(MSG_RULE_PARSE_ERR1, temp);
        throw new CMTException(USERERR_PARSE, EMSG_PARSE_05);
    }

    ruleId = atoi(temp);

//  if ( ruleId == 9141 ) {
//printf("here\n");
//}
    (*pS)++;

    while (**pS && **pS != ':') {
        (*pS)++;
    }

    (*pS)++; //    Parameter Pass

    i = 0;
    while (**pS && **pS != RULE_DELIMITER_RIGHT) {
        temp[i++] = **pS;
        (*pS)++;
    }
    temp[i] = 0;

    if (**pS == 0) {
        //printf("Rule Parse Error : %d %s\n", ruleId, temp);
        printf(MSG_RULE_PARSE_ERR2, ruleId, temp);
        throw new CMTException(USERERR_PARSE, EMSG_PARSE_06);
    }

    (*pS)++;

    if (**pS != '(') {
        if (i > 0)
            sprintf(subStr, "%02d", atoi(temp));
        return ruleId;
    }

    (*pS)++;
    if (**pS == '#') {
        sprintf(subStr, "%02d#%d", atoi(temp), atoi(*pS + 1));
    } else {
        sprintf(subStr, "%02d%d", atoi(temp), atoi(*pS));
    }

    while (**pS && **pS != ')')
        (*pS)++;

    if (**pS == 0) {
        //printf("Rule Parse Error.\n");
        printf(MSG_RULE_PARSE_ERR3);
        throw new CMTException(USERERR_PARSE, EMSG_PARSE_07);
    }

    (*pS)++;

    return (ruleId);
}

QBRULE_SUBSTR * CMRLoadData::createQBNode(unsigned char tokType, int ItemCd,
        char *subStr) {
    QBRULE_SUBSTR *p;

    p = (QBRULE_SUBSTR *) pCMRGblVars->MetisAlloc(sizeof(QBRULE_SUBSTR));

    p->tokType = tokType;
    p->ItemCd = ItemCd;
    p->tokInfo = 0;
    p->pNext = 0;

    if (!subStr)
        return p;

    rtrim(subStr);
    if (strlen(subStr) > 0) {
        p->tokInfo = (char *) pCMRGblVars->MetisAlloc(strlen(subStr) + 1);

        strcpy(p->tokInfo, subStr);
    }

    return p;
}

void CMRLoadData::ParseQueryRule(QRULE_DATA *pQRule, char *source) {
    QBRULE_SUBSTR *pLast, *pTemp;
    char *pSource, *pQry, *pSource1;
	char QryStr[MAX_QUERY_SIZE]; //MItem.h 30000 MAX_QUERY_SIZE
	char tempStr[MAX_QUERY_SIZE]; //MItem.h 30000 MAX_QUERY_SIZE
    char subStr[10];
    int Code, i = 0;
    int bindIdx = 0;
    char Gbn = ' ';

    pSource = source; 
	pSource1 = source; 
    pQry = QryStr;
//	printf("CMRLoadData::ParseQueryRule\n");
    while (*pSource) {
        if (*pSource == 0x0e) {
            while (*pSource != 0x0f) {
                tempStr[i++] = *pSource++;
            }
            tempStr[i++] = *pSource++;
            continue;
        }

        if (*pSource == '-' && *(pSource + 1) == '-') {  // Line�걹源뚯� 二쇱꽍
            while (*pSource && *pSource != '\n') {
                pSource++;
            }
            continue;
        }
 
        while ((*pSource && *pSource == '\n') && (*(pSource+1) && *(pSource+1) == '\n') ) { // \n\n �씪寃쎌슦 �젣嫄�   2014-11-06 ad2001
                    pSource++;
        }

		if ((*pSource && *pSource == '/') &&(*(pSource+1) && *(pSource + 1) == '*') ) { //   二쇱꽍 Hint�쐞�빐 �궡�젮�몺
 //           tempStr[i++] = *pSource++; //二쇱꽍泥섎━ dskim
 //           tempStr[i++] = *pSource++; //二쇱꽍泥섎━ dskim

			//modified by DSKiM 2015.07.09: 二쇱꽍 泥섎━ �빐�젣�븿
            if ((*(pSource+2) && *(pSource+2) != '+')) { //   / * + �엺�듃�젅�씠 �븘�땲硫� �젣嫄� 2014-11-06 ad2001
                *pSource++;
                *pSource++;
                while (*pSource ) {
                    if ( (*pSource && *pSource == '*') && (*(pSource+1) && *(pSource+1) == '/') ) {
                            *pSource++;
                            *pSource++;
                            break;
                    }else{
                        *pSource++;
                    }
                }
                
            }else{
                tempStr[i++] = *pSource++;
                tempStr[i++] = *pSource++;
            } // �씠 �씪�씤源뚯� 二쇱꽍泥섎━ �빐�젣�븿

			//modified by DSKIM 2015.07.09:  二쇱꽍 泥섎━�븿
			/*
			/*
            while (*pSource) {
                if (*pSource == '*' && *(pSource + 1) == '/') {
                    tempStr[i++] = *pSource++;
                    tempStr[i++] = *pSource++;
                    break;
                }

                tempStr[i++] = *pSource++;
            }
            continue; */ //�씠 �씪�씤源뚯� 二쇱꽍泥섎━�븿
			//modified by DSKIM 2015.12.08 -- continue瑜� 留됱쑝硫� 諛붿씤�뵫 蹂��닔�뿉 �쇅遺��엯�젰蹂��닔 �삉�뒗 �궡遺��엯�젰蹂��닔 泥섎━媛� �븞�맖
			// continue瑜� �떎�떆 �궡�졇�쓬
			continue;
        }

		//added by DSKIM 2015.07.09
/*		else {
			tempStr[i++] = *pSource++;
			continue;
		}//// */ //異붽� �썑�뿉 蹂��닔 泥섎━ �삤瑜섍� �엳�뼱 二쇱꽍 泥섎━�븿 2015.07.10

/*         if (*(pSource+2) && *(pSource+2) != '+') { //   /*+ �엺�듃�젅�씠 �븘�땲硫� �젣嫄� 2014-11-06 ad2001
                pSource++;
                pSource++;
                pSource++;
                while (*pSource ) {
                    if (*pSource == '*' && *(pSource + 1) == '/') {
                            *pSource++;
                            *pSource++;
                            break;
                    }
                    *pSource++;
                }

      }else{
                tempStr[i++] = *pSource++;
                tempStr[i++] = *pSource++;
    
                while (*pSource) {
                    if (*pSource == '*' && *(pSource + 1) == '/') {
                        tempStr[i++] = *pSource++;
                        tempStr[i++] = *pSource++;
                        break;
                    }
                    tempStr[i++] = *pSource++;
                }
                continue;
      }
*/
	
		if (*pSource != ITEM_DELIMITER_LEFT && *pSource != RULE_DELIMITER_LEFT
                && *pSource != LOCAL_DELIMITER_LEFT) {
            tempStr[i++] = *pSource++;
            continue;
        }
		
        tempStr[i] = 0;
		if (i > 0) {
            strcpy(pQry, tempStr);
            pQry += i;
            i = 0;
        }

		if (*pSource == ITEM_DELIMITER_LEFT) { 
	        if ((Code = getItemCd(&pSource, subStr)) == 0)
                return;

            pTemp = createQBNode(TOKENTYPE_GLOBAL, Code, subStr);
        } else if (*pSource == RULE_DELIMITER_LEFT) {
	        if ((Code = getRuleCd(&pSource, subStr)) == 0)
                return;

            pTemp = createQBNode(TOKENTYPE_RULE, Code, subStr);
        } else { //  PARM_DELIMITER
            char c = *(pSource + 1);

            if (c >= '0' && c <= '9') {
                if ((Code = getParmCd(&pSource)) == 0)
                    return;
                pTemp = createQBNode(TOKENTYPE_LOCAL, Code, NULL);
            } else {
                tempStr[i++] = *pSource++;
                continue;
            }
        }

		if (pQRule->pBindValue) {
            pLast->pNext = pTemp;
            pLast = pLast->pNext;
        } else
            pQRule->pBindValue = pLast = pTemp;

        pTemp->pos = pQry - QryStr;
// 20190624 param character replace by question mark for MySQL
#if defined(_MYSQL) || (_MARIA)    || (_POSTGRE)  
//#ifdef _MYSQL
		sprintf(pQry, "?   ", ++bindIdx);
		//sprintf(pQry, ":v%02d", ++bindIdx);
#else
		sprintf(pQry, ":v%02d", ++bindIdx);
#endif
		pQry += 4;
        
    }

    if (i > 0) {
        tempStr[i] = 0;
        strcpy(pQry, tempStr);
    }

    pQRule->QueryStatement = (char *) pCMRGblVars->MetisAlloc(
            strlen(QryStr) + 1);
    strcpy(pQRule->QueryStatement, QryStr);
	
    pQRule->nBindVal = bindIdx;
}

QBRULE_SUBSTR * CMRLoadData::ParseBranchRule(char *source) {
    QBRULE_SUBSTR *pRoot, *pLast, *pTemp;
	char *pSource;
	char tempStr[MAX_QUERY_SIZE]; //MItem.h 30000 MAX_QUERY_SIZE
    char subStr[10];
    int Code, i = 0;
    char Gbn = ' ';

    pRoot = 0;
    pSource = source;

    while (*pSource) {
        if (*pSource == 0x0e) {
            while (*pSource != 0x0f) {
                tempStr[i++] = *pSource++;
            }
            tempStr[i++] = *pSource++;
            continue;
        }

        if (*pSource != ITEM_DELIMITER_LEFT && *pSource != RULE_DELIMITER_LEFT
                && *pSource != LOCAL_DELIMITER_LEFT) {
            tempStr[i++] = *pSource++;
            continue;
        }

// printf("=========================>>> %s\n", pSource );
        if (i > 0) {
            tempStr[i] = 0;

            pTemp = createQBNode(TOKENTYPE_NONE, 0, tempStr);
            if (pRoot) {
                pLast->pNext = pTemp;
                pLast = pLast->pNext;
            } else {
                pRoot = pLast = pTemp;
            }

            i = 0;
        }

        if (*pSource == ITEM_DELIMITER_LEFT) {
            if ((Code = getItemCd(&pSource, subStr)) == 0)
                return 0;

            pTemp = createQBNode(TOKENTYPE_GLOBAL, Code, subStr);
        } else if (*pSource == RULE_DELIMITER_LEFT) {
            if ((Code = getRuleCd(&pSource, subStr)) == 0)
                return 0;

            pTemp = createQBNode(TOKENTYPE_RULE, Code, subStr);
        } else { //  PARM_DELIMITER
            char c = *(pSource + 1);

            if (c >= '0' && c <= '9') {
                if ((Code = getParmCd(&pSource)) == 0)
                    return 0;
                pTemp = createQBNode(TOKENTYPE_LOCAL, Code, NULL);
            } else {
                tempStr[i++] = *pSource++;
                continue;
            }
        }

        if (pRoot) {
            pLast->pNext = pTemp;
            pLast = pLast->pNext;
        } else {
            pRoot = pLast = pTemp;
        }
    }

    if (i > 0) {
        tempStr[i] = 0;
        pTemp = createQBNode(TOKENTYPE_NONE, 0, tempStr);
        if (pRoot)
            pLast->pNext = pTemp;
        else
            pRoot = pTemp;
    }

    return pRoot;
}

// 2010-2-10 vapor 二쇱꽍臾� 泥섎━�븿�닔 2010-2-24 �뿬湲곕줈 �씠�룞
int CMRLoadData::FindComments(char * line) {
    const char comment[] = "--";

    int size = strlen(line);
    char * pos = strstr(line, comment);

    char * open, *close;

    int i;

    //援щ텇�옄 [n][0] 怨� [n][1] �궗�씠�뿉 �뱾�뼱媛�硫� 二쇱꽍�븘�떂
    char delimiter[5][2] =
            { '(', ')', '{', '}', '[', ']', '\'', '\'', '"', '"' };

    while (pos) {
        // comment �쐞移섍� 愿꾪샇�굹 �뵲�샂�몴�븞�뿉 �엳�뒗吏� 寃��궗.

        for (i = 0; i < 5; i++) {
            // �뿬�뒗寃껋씠 二쇱꽍臾� �븵�씪 寃쎌슦�뿉留�
            if ((open = strchr(line, delimiter[i][0])) != NULL) {
                if (open < pos) {
                    // 援щ텇�옄�븞�뿉 �뱾�뼱媛� 寃쎌슦 二쇱꽍臾� �븘�떂.
                    if (((close = strrchr(line, delimiter[i][1])) != NULL)
                            && open < pos && pos < close)
                        break;
                }
            }
        }

        // 援щ텇�옄�븞�뿉 �뱾吏� �븡�쓣 寃쎌슦 二쇱꽍臾�
        if (i == 5) {
            // 怨듬갚臾몄옄濡쒕쭔 �씠猷⑥뼱�졇 �엳�쓣寃쎌슦 �씠�쓣 �븘�슂 �뾾�쓬.
            int size = pos - line;
            for (i = 0; i < pos - line; i++) {
                if (line[i] != ' ')
                    return size;
            }

            return 0;
        }

        // �떎�쓬 二쇱꽍湲고샇
        if (pos + 2 >= line + size)
            break;
        pos = strstr(pos + 2, comment);
    }

    // 二쇱꽍臾몄씠 �룷�븿�릺吏� �븡�븯�쓣 寃쎌슦
    return strlen(line);
}

// 2010-2-10 vapor 二쇱꽍臾� 泥섎━�븿�닔 2010-2-24 �뿬湲곕줈 �씠�룞
char* CMRLoadData::RemoveComments(char* orgSql) {
    char saveSql[MAX_QUERY_SIZE]=""; //MItem.h 30000 MAX_QUERY_SIZE[MAX_QUERY_SIZE] = "";
    strcpy(saveSql, orgSql);

    memset(orgSql, 0x00, strlen(orgSql));

    const char linetok[] = "\n\r";
    int validsize = 0;

    char * line = strtok(saveSql, linetok);

    // line 蹂꾨줈 �굹�늿�떎.
    while (line != NULL) {
        // line �뿉�꽌 �쑀�슚�븳 臾몄옄媛��닔 李얜뒗�떎.
        if ((validsize = FindComments(line)) > 0) {
            // 媛쒗뻾臾몄옄�뒗 �뱾�뼱媛��빞 �븯誘�濡�
            line[validsize] = '\n';
            strncat(orgSql, line, validsize + 1);
        }

        line = strtok(NULL, linetok);
    }

    // 2010-2-25 vapor 留덉�留� �꺆 , 媛쒗뻾, 怨듬갚臾몄옄�� �꽭誘몄퐳濡� �뾾�븻�떎. �꽭誘몄퐳濡� �꽆�뼱媛�硫� sql �뿉�윭�궓.
    for (int i = strlen(orgSql) - 1; i >= 0; i--) {
        if (orgSql[i] != ' ' && orgSql[i] != ';' && orgSql[i] != '\n'
                && orgSql[i] != '\r' && orgSql[i] != '\t')
            break;
        orgSql[i] = 0;
    }

    return orgSql;

}

/*
 QBRULE_SUBSTR * CMRLoadData::ParseBnQRule( char *source )
 {
 // 2010-2-24 vapor 二쇱꽍臾� �뾾�븷怨� 荑쇰━臾� 留덉�留됱뿉 遺숈� �씠嫄곗�嫄� �옉 �꽭誘몄퐳濡� �뾾�븻�떎.
 source = RemoveComments(source);


 QBRULE_SUBSTR *pRoot, *pLast, *pTemp;
 char  *pSource, tempStr[MAX_QUERY_SIZE];
 char   subStr[10];
 int    Code, i=0;
 char   Gbn = ' ';

 pRoot = 0;
 pSource = source;

 while( *pSource )
 {

 if ( *pSource == 0x0e ) {
 while ( *pSource != 0x0f ) {
 tempStr[i++] = *pSource++;
 }
 tempStr[i++] = *pSource++;
 continue;
 }

 if( *pSource != ITEM_DELIMITER_LEFT &&
 *pSource != RULE_DELIMITER_LEFT &&
 *pSource != LOCAL_DELIMITER_LEFT )
 {
 tempStr[i++] = *pSource++;
 continue;
 }

 // printf("=========================>>> %s\n", pSource );
 tempStr[i] = 0;
 i = 0;
 rtrim(tempStr);

 if ( strlen(tempStr) > 0 ) {
 pTemp = createQBNode( TOKENTYPE_NONE, 0, tempStr );
 if ( pRoot ) {
 pLast->pNext = pTemp;
 pLast = pLast->pNext;
 } else
 pRoot = pLast = pTemp;
 i = 0;
 }

 if( *pSource == ITEM_DELIMITER_LEFT )
 {
 if ( (Code = getItemCd( &pSource,subStr )) == 0 )
 {
 return 0;
 }

 pTemp = createQBNode( TOKENTYPE_GLOBAL, Code, subStr );
 } else if ( *pSource == RULE_DELIMITER_LEFT )
 {
 if ( (Code = getRuleCd(&pSource,subStr))==0 )
 return 0;

 pTemp = createQBNode( TOKENTYPE_RULE, Code, subStr );
 } else {  //  PARM_DELIMITER
 char c = *(pSource+1);

 if ( c >= '0' && c <= '9') {
 if ( (Code = getParmCd( &pSource )) == 0 )
 return 0;
 pTemp = createQBNode( TOKENTYPE_LOCAL, Code, NULL );
 } else {
 tempStr[i++] = *pSource++;
 continue;
 }
 }

 if ( pRoot ) {
 pLast->pNext = pTemp;
 pLast = pLast->pNext;
 } else
 pRoot = pLast = pTemp;
 }


 if ( i > 0 ) {
 tempStr[i] = 0;
 pTemp = createQBNode( TOKENTYPE_NONE, 0, tempStr );
 if ( pRoot )
 pLast->pNext = pTemp;
 else
 pRoot = pTemp;
 }

 return pRoot;
 }
 */

void CMRLoadData::DestroyDependentDS(void) {
}

/****************************************************************/
