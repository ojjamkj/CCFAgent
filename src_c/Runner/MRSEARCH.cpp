#include  <stdlib.h>
#include  <stdio.h>
#include  <string.h>

#include    "MRSEARCH.h"
#include    "MRGBLVAR.h"
#include    "MRERROR.h"
#include    "MTExcept.h"

#ifdef _MSG_KO
#define     MSG_DUPL_BRANCHNAME   "(%s) 브랜치 이름이 중복되었습니다. : ruleId %d,%d"
#else
#define     MSG_DUPL_BRANCHNAME   "Duplicate branch name (%s) : ruleId %d,%d"
#endif

CMRSearch::CMRSearch() {
	m_NameList = 0;
	m_MaxSize = 0;
	m_LastIdx = -1;
}

CMRSearch::~CMRSearch() {
	if (m_NameList)
		pCMRGblVars->MetisFree(m_NameList);

	m_NameList = 0;
}

void CMRSearch::SetBufferSize(int nSize) {
	m_MaxSize = nSize *2;/*nSize=(SELECT CONT(*)FROM BRS430TB) + 500-> 10000 개로 변경(2018-09-04) ad2001*/
	m_NameList = (RNAME_SEARCH_REC *) pCMRGblVars->MetisAlloc(sizeof(RNAME_SEARCH_REC) * m_MaxSize);
	if (!m_NameList) {
		strcpy(m_sMessage, "Memory allocation error.");

		throw new CMTException(UERR_RULELOADFAIL, m_sMessage);
	}
}

int CMRSearch::AddRuleNodeLast(char *Name, RULE_LIST* ptr) {
	if (!m_NameList) {
		SetBufferSize(1);
	}
	m_LastIdx++;

	strcpy(m_NameList[m_LastIdx].rule_name, Name);
	m_NameList[m_LastIdx].ptr = ptr;
	ptr->branchIdx = m_LastIdx;

	return 0;
}

int CMRSearch::AddRuleNodeSearch(int Id, char *Name, RULE_LIST* ptr) {
	int cmp;
	int nFirst, nLast, nPos;

	if (m_LastIdx <= 0)
		return AddRuleNodeLast(Name, ptr);

	if (m_LastIdx >= m_MaxSize - 1) {
		strcpy(m_sMessage, "Hot deploy로 추가할 수 있는 최대갯수에 도달했습니다.");
		throw new CMTException(UERR_RULELOADFAIL, m_sMessage);
	}

	nFirst = 0;
	nLast = m_LastIdx;

	while (1) {
		if (nLast==0 || nFirst > nLast)
			break;

		nPos = (nFirst + nLast) / 2;

		cmp = strcmp(Name, m_NameList[nPos].rule_name);

		if (cmp == 0) {/*
			sprintf(m_sMessage, "Duplicate branch name (%s) : ruleId %d,%d",
					Name, Id, ptr->rule_id);*/
			sprintf(m_sMessage, MSG_DUPL_BRANCHNAME,
					Name, Id, ptr->rule_id);

			throw new CMTException(UERR_RULELOADFAIL, m_sMessage);
		}

		if (cmp < 0)
			nLast = nPos - 1;
		else
			nFirst = nPos + 1;

	}

	if (cmp > 0)
		nPos++;
 
	//  하나씩 뒤로 밀고 nPos자리를 비운다.
	for (int i = m_LastIdx; i >= nPos; i--) {
		strcpy(m_NameList[i + 1].rule_name, m_NameList[i].rule_name);
		m_NameList[i + 1].ptr = m_NameList[i].ptr;
		m_NameList[i+1].ptr->branchIdx=i+1;  
	}

	//  nPos자리에 추가된 명칭을 넣는다.
	strcpy(m_NameList[nPos].rule_name, Name);
	m_NameList[nPos].ptr = ptr;
	ptr->branchIdx = nPos;

	m_LastIdx++;

	return 0;
}

RULE_LIST* CMRSearch::SearchByName(char *Name) {
	//modified: 브랜치룰 찾지 못하는 오류(카카오뱅크)
	int i,cmp;
	for(i=0; i<=m_LastIdx;i++){
		cmp = strcmp(Name, m_NameList[i].rule_name);

		if (cmp == 0)
			return m_NameList[i].ptr;
	}
	return 0;
	/*
	int cmp;
	int nFirst, nLast, nPos;

	nFirst = 0;
	nLast = m_LastIdx;

#ifdef _DEBUG
	printf("Searching [%s]\n", Name );
#endif

	while (1) {
		if (nFirst > nLast)
			return 0;

		nPos = (nFirst + nLast) / 2;

		cmp = strcmp(Name, m_NameList[nPos].rule_name);

#ifdef _DEBUG
		printf("  %d [%s]\n", cmp, m_NameList[nPos].rule_name );
#endif

		if (cmp == 0)
			return m_NameList[nPos].ptr;

		if (cmp < 0)
			nLast = nPos - 1;
		else
			nFirst = nPos + 1;

	}

	return 0;
	*/
}
//int CMRSearch::DeleteNode(int Idx, char *Name) {
int CMRSearch::DeleteNode(RULE_LIST *pNodeNew , char *Name) {
	int Idx =pNodeNew->branchIdx; 
	int status=0;
	if (Idx < 0)
		return 0;
/*  기존로직 ABL 에서 오류 발생. 
	if (Idx > m_LastIdx) {
		return -1;
	}
*/
//	printf("idx[%d] > m_LastIdx[%d] == Name[%s]\n", Idx, m_LastIdx, Name);
//	printf("m_LastIdx[%d] :  == Name[%s]\n", m_LastIdx, m_NameList[m_LastIdx].rule_name );
	if (Idx > m_LastIdx) {
		pNodeNew->branchIdx=-1;
		Idx = -1;
		for (int i = 0; i <=m_LastIdx; i++) {
			if( strcmp(m_NameList[i].rule_name,Name)==0){
				if(i==m_LastIdx){
					strcpy(m_NameList[i].rule_name, "");
					m_NameList[i].ptr->branchIdx=i; 
				}else{
					strcpy(m_NameList[i].rule_name, m_NameList[i + 1].rule_name);
					m_NameList[i].ptr = m_NameList[i + 1].ptr;
					m_NameList[i].ptr->branchIdx=i; 
				}
				status=1;
			//	printf("idx[%d] > m_LastIdx[%d] == Name[%s]\n",i, m_LastIdx, Name);
			}				
		}
		if( status == 0 ){
			return  0;
		}
//		return -1;
	}
/*	if (strcmp(Name, m_NameList[Idx].rule_name) == 0 ){
		return 1; //  명칭 동일하면 삭제안함
	}*/
	
	for (int i = Idx; i <m_LastIdx; i++) {
		strcpy(m_NameList[i].rule_name, m_NameList[i + 1].rule_name);
		m_NameList[i].ptr = m_NameList[i + 1].ptr;
		m_NameList[i].ptr->branchIdx=i; 
	}
//	printf("DeleteNode 001 m_NameList[%d] =[%s]\n", m_LastIdx-1, m_NameList[m_LastIdx-1].rule_name);
//	printf("DeleteNode 001 m_NameList[%d] =[%s]\n", m_LastIdx, m_NameList[m_LastIdx].rule_name);
	m_LastIdx--;
//	printf("DeleteNode 002 m_NameList[%d] =[%s]\n", m_LastIdx - 1, m_NameList[m_LastIdx - 1].rule_name);
//	printf("DeleteNode 002 m_NameList[%d] =[%s]\n", m_LastIdx, m_NameList[m_LastIdx].rule_name);
//	printf("DELETE NODE m_LastIdx[%d] : == Name[%s]\n", m_LastIdx, m_NameList[m_LastIdx].rule_name, Name);

	return 0; 
}

/*
 void CMRSearch::disp()
 {
 for ( int i=0; i<=m_LastIdx; i++ ) {
 printf("[%6d] %s\n", m_NameList[i].ptr->rule_id, m_NameList[i].rule_name );
 }
 }
 */
