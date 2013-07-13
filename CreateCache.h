#ifndef __CREATE_CACHE_

#define MAXPATH 255
#define SUCCESS 0;
#define FAILED  1;

typedef struct CacheData
{
  RB_NODE_S    stRbNode;
  unsigned int uiFileSize; /* �ļ��Ĵ�С */
  void         *pData;     /* �ļ������� */
  unsigned int uiCnt;      /* �ļ���������� */
  char         acPathName[MAXPATH]; /* �ļ��� */
}CACHE_NODE_S;


typedef struct Cache_Buffer
{
    char* pcBuff;
    CACHE_NODE_S *pstCache;
}CACHE_BUFFER_S;


int CacheNodeAdd(RB_TREE_S *pstTree,
                 char* pcPathName, 
                 unsigned int uiFileSize);
                 
int CacheNodeClean(RB_TREE_S *pstTree, char* pcPathName);

int CacheNodeDel(RB_TREE_S *pstTree, char* pcPathName);

CACHE_NODE_S* CacheNodeFind(RB_TREE_S *pstTree, char* pcPathName);

RB_TREE_S * CacheTreeCreate();


#endif


