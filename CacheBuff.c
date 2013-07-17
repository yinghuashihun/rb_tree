#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include"rb_tree.h"
#include"CacheBuff.h"

RB_TREE_S* g_pstTree = NULL;


/* **********************************************************
  * Function Name : writetoBuff
  * Description   : read the file into the cache node
  * Author        : Internet
  * Input/OutPut  :
  * Return        :

********************************************************* */
unsigned int writetoBuff(char* pcPathName, CACHE_NODE_S* pstCacheNode)
{
   unsigned int uiRet = FAILED;
   FILE *pFileDes = fopen(pcPathName, "rb");
   unsigned int uiFileLen = pstCacheNode->uiFileSize;
   unsigned int uiReadFileLen = 0;

   if (NULL == pFileDes)
   {
      printf("open the file %s failed \r\n", pcPathName);
      return uiRet;
   }

   uiReadFileLen = fread((char*)pstCacheNode->pcData,
                          sizeof(char), uiFileLen,
                          pFileDes);

   if (uiReadFileLen != uiFileLen)
   {
      printf("read file failed \r\n");

      return uiRet;
   }
   pstCacheNode->uiCnt += 1;

   fclose(pFileDes);

   return SUCCESS;
}

/* **********************************************************
  * Function Name : getFileSize
  * Description   : get file size
  * Author        : Internet
  * Input/OutPut  :
  * Return        :

********************************************************* */
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

    if (NULL != pstData->pcData)
    {
       free(pstData->pcData);
       pstData->pcData = NULL;
    }

    free(pstData);
    pstData = NULL;

    return;
}

/* **********************************************************
  * Function Name : cacheNodeCreate
  * Description   : create a cache node
  * Author        : Internet
  * Input/OutPut  :
  * Return        :

********************************************************* */
CACHE_NODE_S* cacheNodeCreate(char* pcPathName, unsigned int uiFileSize)
{
   CACHE_NODE_S *pstCacheNode = NULL;
   unsigned int uiRet = SUCCESS;
   char *pcBuff = NULL;

   /* ����·���Լ��ļ���С�����µĽڵ� */
   pstCacheNode = (CACHE_NODE_S *)malloc(sizeof(CACHE_NODE_S));
   if (NULL == pstCacheNode)
   {
       printf("cache node malloc failed! \r\n");
       return pstCacheNode;;
   }

   pcBuff = (char*)malloc(uiFileSize);

   if (NULL == pcBuff)
   {
       printf("Buffer malloc failed free the cache node buff! \r\n");

       free(pstCacheNode);
       pstCacheNode = NULL;
       
       return pstCacheNode;
   }

   memset(pstCacheNode, 0, sizeof(CACHE_NODE_S));

   memcpy(pstCacheNode->acPathName, pcPathName, MAXPATH);
   pstCacheNode->uiFileSize = uiFileSize;
   pstCacheNode->pcData = pcBuff;

   return pstCacheNode;
}

/* **********************************************************
  * Function Name : CacheNodeInsert
  * Description   : ����������µĽڵ�
  * Author        : Internet
  * Input/OutPut  :
  * Return        :

********************************************************* */
unsigned int cacheNodeInsert(RB_TREE_S *pstTree, CACHE_NODE_S *pstCacheNode)
{
   unsigned int uiRet = FAILED;
   uiRet = RB_Insert(&(pstCacheNode->stRbNode), pstTree);

   return uiRet;
}

/* **********************************************************
  * Function Name : CacheNodeFind
  * Description   : ����PathName�����ֲ�����Ӧ�Ľڵ�
  * Author        : Internet
  * Input/OutPut  :
  * Return        :

********************************************************* */
CACHE_NODE_S* CacheNodeFind(RB_TREE_S *pstTree, char* pcPathName)
{    
    CACHE_NODE_S stCacheNode;
    RB_NODE_S *pstRbNode = NULL;
    CACHE_NODE_S *pstCache = NULL;
    
    memset(&stCacheNode, 0, sizeof(CACHE_NODE_S));
    
    memcpy(stCacheNode.acPathName, pcPathName, MAXPATH);
    
    pstRbNode = RB_Search(&(stCacheNode.stRbNode), pstTree);
    if (NULL == pstRbNode)
    {
       return FAILED;
    }
    
    pstCache = RB_ENTRY(pstRbNode, CACHE_NODE_S, stRbNode);

    return pstCache;
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
    pstTree->pfFree(&(pstCache->stRbNode));
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

/* **********************************************************
  * Function Name : CacheNodeAdd
  * Description   : ��������ڵ㲢�Ҽ��뵽����
  * Author        : Internet
  * Input/OutPut  :
  * Return        :

********************************************************* */
unsigned int CacheNodeAdd(RB_TREE_S *pstTree, const char* pPath)
{
    unsigned int uiFileSize = 0;
    unsigned int uiRet = FAILED;
    CACHE_NODE_S* pstCache = NULL;

    uiFileSize = getFileSize(pPath);
    if (0 == uiFileSize)
    {
        return uiRet;
    }
    
    /* Create a cache node */
    pstCache = cacheNodeCreate(pPath, uiFileSize);
    if (NULL == pstCache)
    {
       printf("Create cache node failed \r\n");
    
       return uiRet;
    }
    
    /*  read the file to cache node 
     *  if failed free the cache buff
    */
    uiRet = writetoBuff(pPath, pstCache);
    if (SUCCESS != uiRet)
    {
       pstTree->pfFree(&(pstCache->stRbNode));
       
       return uiRet;
    }
    
    /* add the cache node to the cache tree */
    uiRet = cacheNodeInsert(pstTree, pstCache);
    if (SUCCESS != uiRet)
    {
       pstTree->pfFree(&(pstCache->stRbNode));
    }

    return uiRet;
}

/* **********************************************************
  * Function Name : CacheTreeBuild
  * Description   : ����ָ����·����ʼ����
  * Author        : Internet
  * Input/OutPut  :
  * Return        :

********************************************************* */
void CacheTreeBuild(RB_TREE_S *pstTree, char * pcPathname)
{
    DIR * pdir = NULL;
    struct dirent *ptr = NULL;
    unsigned int uiRet = 0;
    CACHE_NODE_S *pstCache = NULL;

    unsigned int uiFileSize = 0;

    char fullname[MAX];
    bzero(fullname,MAX);
    char path[MAX];
    bzero(path,MAX);

    pdir = opendir(pcPathname);
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
            CacheTreeBuild(pstTree, path);
        }
        if(DT_REG == ptr->d_type)
        {
            sprintf(fullname, "%s/%s", pcPathname, ptr->d_name);
            
            uiRet = CacheNodeAdd(pstTree, fullname);
            if (SUCCESS != uiRet)
            {
               break;
            }
        }
    }
    closedir(pdir);
}

RB_TREE_S* GetCacheTree()
{
   return g_pstTree;
}

void SetCacheTree(RB_TREE_S *pstTree)
{
   g_pstTree = pstTree;

   return;
}


/* **********************************************************
  * Function Name : CacheTreeInit
  * Description   : ����һ����
  * Author        : Internet
  * Input/OutPut  :
  * Return        :

********************************************************* */
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

/* **********************************************************
  * Function Name : CacheTreeDeInit
  * Description   : ����һ����
  * Author        : Internet
  * Input/OutPut  :
  * Return        :

********************************************************* */
void CacheTreeDeInit(RB_TREE_S *pstTree)
{
    RB_DeInit(pstTree);
    return;
}


