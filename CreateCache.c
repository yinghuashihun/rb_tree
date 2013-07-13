#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>
#include<unistd.h>
#include"rb_tree.h"
#include"CreateCache.h"


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
  * Function Name : CacheNodeDel
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

