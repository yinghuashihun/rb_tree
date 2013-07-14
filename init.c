#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>
#include<unistd.h>
#include"rb_tree.h"

#define MAX 255

RB_TREE_S* g_pstTree = NULL;

CACHE_HEAD_S* g_pstCacheHead = NULL;

RB_TREE_S* GetCacheTree()
{
   return g_pstTree;
}

void SetCacheTree(RB_TREE_S *pstTree)
{
   g_pstTree = pstTree;
   return;
}

CACHE_HEAD_S* GetCacheHead()
{
   return g_pstCacheHead;
}

void SetCacheHead(CACHE_HEAD_S* pstCacheHead)
{
   g_pstCacheHead = pstCacheHead;
   return;
}


unsigned int RequestProc(char* pcPath)
{
   RB_TREE_S *pstTree = NULL;

   unsigned int uiRet = 0;

   CACHE_NODE_S* pstCache = NULL;
   pstTree = GetCacheTree();
   if (NULL != pstTree)
   {
      return FAILED;
   }

   pstCache = CacheNodeFind(pstTree, pcPath);

   /* ����ڵ�û��Ԥ�ȶ��뻺������ */
   if (NULL == pstCache)
   {
      /* ��Ӳ�̶�ȡ���� */
      uiRet = GetVaildBuff(pstCache);

      if(SUCCESS != uiRet)
      {
         printf("Get buffer failed \r\n");

         return;
      }

      WriteDataToBuff(pcPath, pstCache);
   }

   if ((0 == pstCache->uiCnt) && 
       (NULL == pstCache->pData))
   {
       /* ����Ѿ��Ѿ������������ǻ�û�ж�ȡ������ */
       uiRet = GetVaildBuff(pstCache);
       if(SUCCESS != uiRet)
       {
          printf("Get buffer failed \r\n");

          return;
       }

       /* ֱ�ӽ����ݶ�ȡ���������� */
       WriteDataToBuff(pcPath, pstCache);
   }

   /* �������߷������� */
   
}

int main()
{
    RB_TREE_S *pstTree = NULL;
    CACHE_HEAD_S* pstCacheHead = NULL;

    CacheHeadInit();
    pstCacheHead = GetCacheHead();
    if (NULL == pstCacheHead)
    {
       return;
    }

    CacheTreeInit();
    pstTree = GetCacheTree();

    if (NULL == pstTree)
    {
       return;
    }
    CacheTreeBuild(pstTree, "/opt/apache-tomcat-6.0.36/webapps/website");
    return 0;
}
