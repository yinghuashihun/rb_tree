#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>
#include<unistd.h>
#include"rb_tree.h"

#define MAX 255

RB_TREE_S* g_pstTree = NULL;

RB_TREE_S* GetCacheTree()
{
   return g_pstTree;
}

void SetCacheTree(RB_TREE_S *pstTree)
{
   g_pstTree = pstTree;

   return;
}


unsigned int RequestProc(int fd, char* pcPath)
{
   unsigned int uiRet = FAILED;
   CACHE_NODE_S* pstCacheNode = NULL;
   RB_TREE_S* pstTree = GetCacheTree();

   if (NULL == pstTree)
   {
       return uiRet;
   }
   
   pstCacheNode = CacheNodeFind(pstTree,pcPath);
   if (NULL == pstCacheNode)
   {
      /*  the request data not exist, now read from hard*/
      uiRet = CacheTreeBuild(pstTree, pcPath); 
   }

   if (SUCCESS != uiRet)
   {
      return uiRet;
   }

   /* Responding to requests  */

   return SUCCESS;
}

int main()
{
    RB_TREE_S *pstTree = NULL;

    CacheTreeInit();
    pstTree = GetCacheTree();

    if (NULL == pstTree)
    {
       return;
    }
    CacheTreeBuild(pstTree, "/opt/apache-tomcat-6.0.36/webapps/website");

    return 0;
}
