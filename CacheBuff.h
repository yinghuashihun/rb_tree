#ifndef __CACHE_BUFF_


#define MAXPATH 255
#define SUCCESS 0
#define FAILED  1


#define MAX 255

typedef struct CacheData
{
  RB_NODE_S    stRbNode;
  unsigned int uiFileSize; 
  char         *pcData;     
  unsigned int uiCnt;      
  char         acPathName[MAXPATH]; 
}CACHE_NODE_S;

unsigned int CacheNodeAdd(RB_TREE_S *pstTree, const char* pPath);


int CacheNodeDel(RB_TREE_S *pstTree, char* pcPathName);

void CacheTreeBuild(RB_TREE_S *pstTree, char * pcPathname);

CACHE_NODE_S* CacheNodeFind(RB_TREE_S *pstTree, char* pcPathName);

RB_TREE_S * CacheTreeCreate();

void CacheTreeInit();

void CacheTreeDeInit(RB_TREE_S *pstTree);

RB_TREE_S* GetCacheTree();

void SetCacheTree(RB_TREE_S *pstTree);


#endif

