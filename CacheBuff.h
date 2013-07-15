#ifndef __CREATE_CACHE_

#define MAXPATH 255
#define SUCCESS 0;
#define FAILED  1;

#define CACHE_BUFF_SIZE 10


typedef struct CacheData
{
  RB_NODE_S    stRbNode;
  unsigned int uiFileSize; /* �ļ��Ĵ�С */
  char         *pcData;     /* �ļ������� */
  unsigned int uiCnt;      /* �ļ���������� */
  char         acPathName[MAXPATH]; /* �ļ��� */
}CACHE_NODE_S;

int CacheNodeAdd(RB_TREE_S *pstTree,
                 char* pcPathName, 
                 unsigned int uiFileSize);

int CacheNodeDel(RB_TREE_S *pstTree, char* pcPathName);

void CacheTreeBuild(RB_TREE_S *pstTree, char * pcPathname);

CACHE_NODE_S* CacheNodeFind(RB_TREE_S *pstTree, char* pcPathName);

RB_TREE_S * CacheTreeCreate();

void CacheTreeInit();

#endif

