#ifndef __CREATE_CACHE_

#define MAXPATH 255
#define SUCCESS 0;
#define FAILED  1;

typedef struct CacheData
{
  RB_NODE_S    stRbNode;
  unsigned int uiFileSize;
  void         *pData;
  char         acPathName[MAXPATH];
}CACHE_NODE_S;

int CacheNodeAdd(RB_TREE_S *pstTree,
                 char acPathName[], 
                 unsigned int uiFileSize);
int CacheNodeClean(RB_TREE_S *pstTree, char acPathName[]);

int CacheNodeDel(RB_TREE_S *pstTree, char acPathName[]);

CACHE_NODE_S* CacheNodeFind(RB_TREE_S *pstTree, char* pcPathName);


RB_TREE_S * CacheTreeCreate();




#endif


