#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>
#include<unistd.h>
#include"rb_tree.h"
#include"CreateCache.h"


unsigned int getFileSize(const char* pPath)
{
    unsigned int uiFileSize = 0;      
    struct stat stStatBuff;

    memset(&stStatBuff, 0 , sizeof(struct stat));
    
    if(0 > stat(pPath, &stStatBuff))
    {  
        return uiFileSize;  
    }
    else
    {  
        uiFileSize = stStatBuff.st_size;  
    }

    return uiFileSize;   
}



int cacheKeyCmp(RB_NODE_S *pstKey1, RB_NODE_S *pstKey2)
{
    CACHE_NODE_S* pstData1 = RB_ENTRY(pstKey1, CACHE_NODE_S, stRbNode);
    CACHE_NODE_S* pstData2 = RB_ENTRY(pstKey2, CACHE_NODE_S, stRbNode);

    return memcmp(pstData2->acPathName, pstData1->acPathName, MAXPATH);
}

void cacheNodeFree(RB_NODE_S *pstKey)
{
    CACHE_NODE_S* pstData = RB_ENTRY(pstKey, CACHE_NODE_S, stRbNode);

    if (NULL != pstData->pData)
    {
       free(pstData->pData);
       pstData->pData = NULL;
    }

    free(pstData);
    pstData = NULL;

    return;
}

/* **********************************************************
  * Function Name : CacheNodeFind
  * Description   : ����PathName�����ֲ�����Ӧ�Ľڵ�
  * Author        : Internet
  * Input/OutPut  :
  * Return        :

********************************************************* */

CACHE_NODE_S* CacheNodeFind(RB_TREE_S *pstTree, char* pcPathName)
{    CACHE_NODE_S stCacheNode;
    RB_NODE_S *pstRbNode = NULL;
    CACHE_NODE_S pstCache = NULL;
    
    memset(&stCacheNode, 0, sizeof(CACHE_NODE_S));
    
    memcpy(stCacheNode->acPathName, acPathName, MAXPATH);
    
    pstRbNode = RB_Search(&(stCacheNode.stRbNode), pstTree);
    if (NULL == pstRbNode)
    {
       return FAILED;
    }
    
    pstCache = RB_ENTRY(pstRbNode, CACHE_NODE_S, stRbNode);

    return pstCache;
}

/* **********************************************************
  * Function Name : CacheNodeAdd
  * Description   : ����������µĽڵ�
  * Author        : Internet
  * Input/OutPut  :
  * Return        :

********************************************************* */
int CacheNodeAdd(RB_TREE_S *pstTree, char* pcPathName, unsigned int uiFileSize)
{
   CACHE_NODE_S *pstCacheNode = NULL;
   unsigned int uiRet = SUCCESS;

   /* ����·���Լ��ļ���С�����µĽڵ� */
   pstCacheNode = (CACHE_NODE_S *)malloc(sizeof(CACHE_NODE_S));
   if (NULL == pstCacheNode)
   {
       printf("cache node malloc failed! \r\n");
       return FAILED;
   }

   memset(pstCacheNode, 0, sizeof(CACHE_NODE_S));

   memcpy(pstCacheNode->acPathName, acPathName, MAXPATH);
   pstCacheNode->uiFileSize = uiFileSize;

   uiRet += RB_Insert(&(pstCacheNode->stRbNode), pstTree);

   /* �ڵ����ʧ�ܣ��ͷ��ڴ� */
   if (RB_OPEAT_SUCCESS != uiRet)
   {
      free(pstCacheNode);
      pstCacheNode = NULL;
   }
   
   return uiRet;
}

/* **********************************************************
  * Function Name : CacheNodeClean
  * Description   : �����нڵ��ϱ�������ݻ����ͷ�
  * Author        : Internet
  * Input/OutPut  :
  * Return        :

********************************************************* */
int CacheNodeClean(RB_TREE_S *pstTree, char* pcPathName)
{
    CACHE_NODE_S pstCache = CacheNodeFind(pstTree, pcPathName);
    if (NULL == pstCache)
    {
       return FAILED;
    }

    /* ������������������㣬�ڴ沢���ͷ� */
    if (NULL != pstCache->pData)
    {
      memset(pstCache->pData, 0, pstCache->uiFileSize);
      pstCache->pData = NULL;

      /* ���ʴ����������� */
      pstCache->uiCnt = 0;
      pstCache->iBuffIndex = -1; 
    }

    return SUCCESS;
}

/* **********************************************************
  * Function Name : CacheNodeDel
  * Description   : �����нڵ���ɾ��
  * Author        : Internet
  * Input/OutPut  :
  * Return        :

********************************************************* */
int CacheNodeDel(RB_TREE_S *pstTree, char* pcPathName)
{
    CACHE_NODE_S *pstCache = CacheNodeFind(pstTree, pcPathName);
    if (NULL == pstCache)
    {
       return FAILED;
    }

    /* ��������ջ��� */
    if (NULL != pstCache->pData)
    {
       memset(pstCache->pData, 0, pstCache->uiFileSize);
       pstCache->pData = NULL;
    }

    free(pstCache);
    return SUCCESS;
}

/* **********************************************************
  * Function Name : CacheTreeCreate
  * Description   : ����������
  * Author        : Internet
  * Input/OutPut  :
  * Return        :

********************************************************* */
RB_TREE_S * CacheTreeCreate()
{ 
   RB_TREE_S *pstRbTree = NULL;
   pstRbTree = RB_Init(cacheKeyCmp, cacheNodeFree);

   if (NULL == pstRbTree)
   {
       return NULL;
   }

   return pstRbTree;

}

void CacheTreeBuild(RB_TREE_S *pstTree, char * pcPathname)
{
    DIR * pdir = NULL;
    struct dirent *ptr = NULL;
    unsigned int uiRet = 0;

    unsigned int uiFileSize = 0;

    char fullname[MAX];
    bzero(fullname,MAX);
    char path[MAX];
    bzero(path,MAX);

    pdir = opendir(pathname);
    if(NULL == pdir)
    {
        exit(1);
    }
    
    while( (ptr = readdir(pdir)) !=NULL )
    {
        if(0 == strcmp(ptr->d_name,".") ||  0 == strcmp(ptr->d_name,".."))
        {
            continue;
        }

        /* �ݹ����ָ��Ŀ¼��Ŀ¼�µ������ļ� */
        if(DT_DIR == ptr->d_type)
        {
            CacheTreeBuild(path);
        }
        if(DT_REG == ptr->d_type)
        {
            sprintf(fullname, "%s/%s", pathname, ptr->d_name);
            /* ��ȡ�ļ��Ĵ�С */
            uiFileSize = getFileSize(fullname);
            if (0 == uiFileSize)
            {
                continue;
            }

            uiRet = CacheNodeAdd(pstTree, fullname, uiFileSize);
            if (SUCCESS != uiRet)
            {
               printf("add path to tree failed \r\n");

               break;
            }
        }
    }
    closedir(pdir);
}

unsigned int GetVaildBuff(CACHE_HEAD_S* pstCacheHead, CACHE_NODE_S* pstCacheNode)
{
   unsigned int uiRet = FAILED;
   CACHE_BUFFER_S* pstCacheBuff = NULL;
   int i = 0;
   for(i = 0; i < pstCacheHead->uiSize; ++i)
   {
      pstCacheBuff = pstCacheHead->pstCacheBuff[i];
      if ((NULL == pstCacheBuff)->pstCache) ||
          (0 == pstCacheBuff->pstCache.uiCnt))
      {
          pstCacheNode->pData = pstCacheBuff->pcBuff;
          pstCacheNode->uiBuffIndex = i;

          pstCacheBuff->pstCache = pstCacheNode;

          uiRet = SUCCESS;
          break;
      }
   }

   /* ��ǰ���е�Buffer���Ѿ��������ˣ�����һ���������� */
   if (SUCCESS != uiRet)
   {
     
   }
   return uiRet;
}

void WriteDataToBuff(char* pcPathName, CACHE_NODE_S* pstCacheNode)
{

   FILE *pFileDes = open(pcPathName, "rb");
   unsigned int uiFileLen = pstCacheNode->uiFileSize;
   unsigned int uiReadFileLen = 0;

   if (NULL == pFileDes)
   {
      printf("open the file %s failed \r\n", pcPathName);
      return;
   }

   uiReadFileLen = fread((char*)pstCacheNode->pData,
                          sizeof(char), uiFileLen,
                          pFileDes);

   if (uiReadFileLen != uiFileLen)
   {
      printf("read file failed \r\n");

      /* Buff���������� */
      memset(pstCacheNode->pData, 0, uiReadFileLen);
      return;
   }
   pstCacheNode->uiCnt += 1;

   close(pFileDes);

   return;
}


/* **********************************************************
  * Function Name : CacheHeadCreate
  * Description   : ����������
  * Author        : Internet
  * Input/OutPut  :
  * Return        :

********************************************************* */
CACHE_HEAD_S* CacheHeadCreate()
{ 
   CACHE_HEAD_S *pstCacheHead = NULL;

   pstCacheHead = (CACHE_HEAD_S *)malloc(sizeof(CACHE_HEAD_S));
   if (NULL == pstCacheHead)
   {
       return NULL;
   }

   return pstCacheHead;
}

void CacheTreeInit()
{
    RB_TREE_S *pstTree= NULL;

    pstTree = CacheTreeCreate();

    if (NULL == pstTree)
    {
       printf("Create cache failed\r\n");
       return;
    }

    SetCacheTree(pstTree);
    return;
}


/* ���뻺��Ĵ�С */
void CacheBuffInit(CACHE_HEAD_S *pstCacheHead, unsigned int uiSize)
{
    CACHE_BUFFER_S* pstCacheBuff = NULL;
    
    int i = 0;
    for(i = 0; i < uiSize; +i)
    {
       pstCacheBuff = (CACHE_BUFFER_S*)malloc(sizeof(CACHE_BUFFER_S));

       if (NULL == pstCacheBuff)
       {
          break;
       }

       memset(pstCacheBuff, 0, sizeof(CACHE_BUFFER_S));
       
       pstCacheHead->pstCacheBuff[i] = pstCacheBuff;
       pstCacheHead->uiSize += 1;
    }

    return;
}


void CacheHeadInit()
{
    CACHE_HEAD_S* pstCacheHead = NULL;

    pstCacheHead = CacheHeadCreate();

    if (NULL == pstCacheHead)
    {
       printf("Create cache failed\r\n");
       return;
    }

    SetCacheHead(pstCacheHead);

    CacheBuffInit(pstCacheHead, CACHE_BUFF_SIZE);

    return;
}

void CacheHeadDeInit(CACHE_HEAD_S *pstCacheHead)
{
    int i = 0;
    for(i = 0; i < pstCacheHead->uiSize; +i)
    {
       if (NULL != pstCacheHead->pstCacheBuff[i])
       {
           free(pstCacheHead->pstCacheBuff[i]);
           pstCacheHead->pstCacheBuff[i] = NULL;
       }
    }

    free(pstCacheHead);
    pstCacheHead = NULL;

    return;
}



