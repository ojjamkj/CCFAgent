#ifndef     _INC_CFAPI
#define     _INC_CFAPI

#include "BRMObj.h"  
#include    <stdio.h>
#include	<string.h>
#include    <time.h>
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
#include <dirent.h>
#include <libgen.h>
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
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <zlib.h>
#include <jansson.h>
#include <utime.h>

#define MAX_PATH 260

//struct FileNode {
//	char filePath[1024];
//	bool isFile;
//	long filesize;
//	struct FileNode* next;  // 다음 노드를 가리키는 포인터
//};

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
		void API03_PING(CBRMObj  *m_ObjBuffer, int m_itemCnt, int pChildSoc, CCSManager *pManage);
		void API04_VIEWDIR(CBRMObj  *m_ObjBuffer2, int m_itemCnt, int pChildSoc, CCSManager *pManage);
		void API06_VIEWFILE6(CBRMObj  *m_ObjBuffer, int m_itemCnt, int pChildSoc, CCSManager *pManage);
		void API07_CREATEFILE(CBRMObj  *m_ObjBuffer, int m_itemCnt,int pChildSoc , CCSManager *pManage);
		void API15_BUILD(CBRMObj  *m_ObjBuffer2, int m_itemCnt, int pChildSoc, CCSManager *pManage);
		void API26_DOSEARCH_ONLY_DIR(CBRMObj  *m_ObjBuffer, int m_itemCnt, int pChildSoc, CCSManager *pManage);
		void API27_DOSEARCH_ONLY_FILE(CBRMObj  *m_ObjBuffer, int m_itemCnt, int pChildSoc, CCSManager *pManage);
		void API41_DOSEARCH_ONLY_FILE_COLLECT(CBRMObj  *m_ObjBuffer, int m_itemCnt, int pChildSoc, CCSManager *pManage);
		void API28_DELETEFILE(CBRMObj  *m_ObjBuffer, int m_itemCnt, int pChildSoc, CCSManager *pManage);
		void API40_SCANDIR_TO_FILE(CBRMObj  *m_ObjBuffer, int m_itemCnt, int pChildSoc, CCSManager *pManage);
};
 
#endif
