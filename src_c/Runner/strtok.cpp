#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char* McRTrim(char *lpszSource) {
        int i;
        for (i = strlen(lpszSource) - 1; i > -1; i--) {
                if (lpszSource[i] == ' ')
                        lpszSource[i] = 0x00;
                else
                        break;
        }
        return lpszSource;
}
char* TrimToken(char *lpszSource, char tk) {
        int i;
        for (i = strlen(lpszSource) - 1; i > -1; i--) {
                if (lpszSource[i] == tk)
                        lpszSource[i] = ' ';
        }
        McRTrim(lpszSource);
        return lpszSource;
}
unsigned char StringChange(char *pData, char *pSrc, char *pDest) {
	char *pPos;
	int leng;
	char tempString[1001];

	pPos = strstr(pData, pSrc);

	if (!pPos)
		return 0;

	leng = strlen(pSrc);

	strcpy(tempString, pPos + leng);

	strcpy(pPos, pDest);

	strcat(pPos, tempString);

	return 1;
}
int main( void ){
/*
	//xchar queryTemp2[]="{1543575:$2$,$3$:}";
	char queryTemp2[]="$2$,$3$";
        TrimToken(queryTemp2,':');
printf("%s\n",queryTemp2);
        TrimToken(queryTemp2,'$');
printf("%s\n",queryTemp2);
        TrimToken(queryTemp2,' ');
printf("%s\n",queryTemp2);
    char *ptr=strtok(queryTemp2,",");
    int tokCnt=0;
	int breakIndex;
    TrimToken(queryTemp2,'$');
    char  indexStr[1001];
    while( ptr !=NULL ){
              printf("%s \n", ptr);
              sprintf(indexStr,"%s",ptr);
              printf("%s \n", indexStr);
              ptr = strtok(NULL,",");
              breakIndex = atoi(indexStr);
              printf("breakIndex %d \n", breakIndex);
    }
*/
	char ruleTemp[]="{1543576:$2$,$3$:}";
	char queryTemp[]=":$2$,$3$:"; 
        char queryTemp3[]=":2.000000,30.000000:";
	StringChange(ruleTemp, queryTemp, queryTemp3);
	printf("ruleTemp [%s] \n",ruleTemp);
}

