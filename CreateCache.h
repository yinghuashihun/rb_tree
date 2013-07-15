#ifndef __CREATE_CACHE_

#define MAXPATH 255
#define SUCCESS 0;
#define FAILED  1;

#define CACHE_BUFF_SIZE 10

/* RB_TREE�ϵĽڵ�Ľṹ */
typedef struct CacheData
{
  RB_NODE_S    stRbNode;
  unsigned int uiFileSize; /* �ļ��Ĵ�С */
  void         *pData;     /* �ļ������� */
  unsigned int uiCnt;      /* �ļ���������� */
  int          iBuffIndex; /* �ڵ��ȡ�ļ�����Buffer������ */
  char         acPathName[MAXPATH]; /* �ļ��� */
}CACHE_NODE_S;

/* Buffer�ṹ  */
typedef struct Cache_Buffer
{
    CACHE_NODE_S *pstCache; /* ָ����Buffer���ĸ����ϵĽڵ�ռ�� */
    char* pcBuff;           /*  Buffer������ */
}CACHE_BUFFER_S;

/* BufferHead ��������Buff */
typedef struct Cache_Head
{
    CACHE_BUFFER_S* pstCacheBuff[CACHE_BUFF_SIZE];
    unsigned int uiSize;

}CACHE_HEAD_S;


int CacheNodeAdd(RB_TREE_S *pstTree,
                 char* pcPathName, 
                 unsigned int uiFileSize);
                 
int CacheNodeClean(RB_TREE_S *pstTree, char* pcPathName);

int CacheNodeDel(RB_TREE_S *pstTree, char* pcPathName);

void CacheTreeBuild(RB_TREE_S *pstTree, char * pcPathname);

CACHE_NODE_S* CacheNodeFind(RB_TREE_S *pstTree, char* pcPathName);

RB_TREE_S* CacheTreeCreate();

CACHE_HEAD_S* CacheHeadCreate();

void CacheTreeInit();

void CacheBuffInit(CACHE_HEAD_S *pstCacheHead,
                   unsigned int uiSize);

void CacheHeadInit(CACHE_HEAD_S *pstCacheHead);

void CacheHeadDeInit(CACHE_HEAD_S *pstCacheHead);

unsigned int GetVaildBuff(CACHE_HEAD_S* pstCacheHead,
                          CACHE_NODE_S* pstCacheNode);

void WriteDataToBuff(char* pcPathName, CACHE_NODE_S* pstCacheNode);

#endif

