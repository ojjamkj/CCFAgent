#ifndef     _INC_CFAPI
#define     _INC_CFAPI

#include "BRMObj.h"  
#include    <stdio.h>
#include	<string.h>

#if defined(_AIX)
#include	<strings.h>
#endif

#ifndef   _OS390
#include  <malloc.h>
#endif

#include    <time.h>

#if defined(_OS390)
#include	<sys/time.h>
#endif
#if defined(WIN32)
#include	<sys/timeb.h>
#endif

//#include	"BRMObj.h"
#include "BRLine.h"
#include    "MTUtil.h"
#include    "MTSysMsg.h"

#include    "MRConfig.h"
#include    "MRCSMgr.h"
#include	"MRPGMEXP.h"
#include    "MRCSThr.h"
#include    "MRERROR.h"
#include    "MRDTLIST.h"
#include    "MRRUNNER.h"
#include	"MRGBLVAR.h"
#include    "MRADMIN.h"
#include    "BREXLog.h"
#include    "CFAPI.h"
#ifdef _WIN32
#include <direct.h>
#else
#include <dirent.h>
#include <libgen.h>
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>
#include <regex.h>
#include <pwd.h>
#include <grp.h>
#define MAX_PATH 260

struct FileNode {
	char filePath[1024];
	bool isFile;
	long filesize;
	struct FileNode* next;  // 다음 노드를 가리키는 포인터
};

class   CFAPI
{
public:
	char fileList[300][1024];
	int  fileCount=0;
	int	 m_pChildSoc=-1;
	CCSManager *m_pManager;
	CBRMObj  *m_ObjBuffer;
		CFAPI();
		virtual ~CFAPI();
		void API01(CBRMObj  *m_ObjBuffer, int m_itemCnt,int pChildSoc , CCSManager *pManage);
		void API02(CBRMObj  *m_ObjBuffer, int m_itemCnt,int pChildSoc , CCSManager *pManage);
		void API03_PING(CBRMObj  *m_ObjBuffer, int m_itemCnt, int pChildSoc, CCSManager *pManage);
		void API04_VIEWDIR(CBRMObj  *m_ObjBuffer2, int m_itemCnt, int pChildSoc, CCSManager *pManage);
		void API06_VIEWFILE6(CBRMObj  *m_ObjBuffer, int m_itemCnt, int pChildSoc, CCSManager *pManage);
//		void API03(CBRMObj  *m_ObjBuffer, int m_itemCnt, int pChildSoc, CCSManager *pManage);
//		void API03(CBRMObj  *m_ObjBuffer, int m_itemCnt, int pChildSoc, CCSManager *pManage);
//		void API03(CBRMObj  *m_ObjBuffer, int m_itemCnt, int pChildSoc, CCSManager *pManage);
//		void API03(CBRMObj  *m_ObjBuffer, int m_itemCnt, int pChildSoc, CCSManager *pManage);
		void API07_CREATEFILE(CBRMObj  *m_ObjBuffer, int m_itemCnt,int pChildSoc , CCSManager *pManage);
		void API27_DOSEARCH_ONLY_FILE(CBRMObj  *m_ObjBuffer, int m_itemCnt, int pChildSoc, CCSManager *pManage);
		void API28_DELETEFILE(CBRMObj  *m_ObjBuffer, int m_itemCnt, int pChildSoc, CCSManager *pManage);
		void API100(CBRMObj  *m_ObjBuffer, int m_itemCnt, int pChildSoc, CCSManager *pManage);
		void listDirectories(const char* path);
		bool compressFolder(const char* folderPath, const char* zipPath);
		void executeBatchFile(const char* batchFilePath);
		void generateRandomFileName(char* fileName);
		void listFiles(const char* path, const char* regexPattern);
		//void listFiles(struct FileNode** head ,const char* path, const char* regexPattern);
		char* formatdate(char *str, time_t val);
		struct rootNode* addNode(struct FileNode** head, int data);
		void removeAll();
};
 
#endif
