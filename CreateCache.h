#ifndef __CREATE_CACHE_

#define MAXPATH 255
#define SUCCESS 0;
#define FAILED  1;

#define CACHE_BUFF_SIZE 10

/* RB_TREE上的节点的结构 */
typedef struct CacheData
{
  RB_NODE_S    stRbNode;
  unsigned int uiFileSize; /* 文件的大小 */
  void         *pData;     /* 文件的内容 */
  unsigned int uiCnt;      /* 文件的请求计数 */
  int          iBuffIndex; /* 节点读取文件所用Buffer的索引 */
  char         acPathName[MAXPATH]; /* 文件名 */
}CACHE_NODE_S;

/* Buffer结构  */
typedef struct Cache_Buffer
{
    CACHE_NODE_S *pstCache; /* 指明该Buffer被哪个树上的节点占用 */
    char* pcBuff;           /*  Buffer缓存区 */
}CACHE_BUFFER_S;

/* BufferHead 用来管理Buff */
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

