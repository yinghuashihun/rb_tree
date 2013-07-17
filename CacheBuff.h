#ifndef __CREATE_CACHE_

#define MAXPATH 255
#define SUCCESS 0;
#define FAILED  1;

#define MAX 255

typedef struct CacheData
{
  RB_NODE_S    stRbNode;
  unsigned int uiFileSize; /* 文件的大小 */
  char         *pcData;     /* 文件的内容 */
  unsigned int uiCnt;      /* 文件的请求计数 */
  char         acPathName[MAXPATH]; /* 文件名 */
}CACHE_NODE_S;

unsigned int CacheNodeAdd(RB_TREE_S *pstTree, const char* pPath);


int CacheNodeDel(RB_TREE_S *pstTree, char* pcPathName);

void CacheTreeBuild(RB_TREE_S *pstTree, char * pcPathname);

CACHE_NODE_S* CacheNodeFind(RB_TREE_S *pstTree, char* pcPathName);

RB_TREE_S * CacheTreeCreate();

void CacheTreeInit();

void CacheTreeDeInit(RB_TREE_S *pstTree);

#endif

