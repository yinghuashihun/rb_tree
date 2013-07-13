#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>
#include<unistd.h>
#include"rb_tree.h"

#define MAX 255
#define CACHE_BUFF_SIZE 10

RB_TREE_S* g_pstTree = NULL;

CACHE_BUFFER_S* g_pacCacheBuff[CACHE_BUFF_SIZE] = {0};


unsigned int GetVaildBuff(CACHE_NODE_S* pstCacheNode)
{
   unsigned int uiRet = FAILED;
   int i = 0;
   for(i = 0; i < CACHE_BUFF_SIZE; ++i)
   {
      if ((NULL == g_pacCacheBuff[i].pstCache) ||
          (0 == g_pacCacheBuff[i].pstCache.uiCnt))
      {
          pstCacheNode->pData = g_pacCacheBuff[i].pcBuff;

          g_pacCacheBuff[i].pstCache = pstCacheNode;

          uiRet = SUCCESS;
          break;
      }
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

      /* Buff的数据清零 */
      memset(pstCacheNode->pData, 0, uiReadFileLen);
      return;
   }
   pstCacheNode->uiCnt += 1;

   close(pFileDes);

   return;
}

unsigned int GetFileSize(const char* pPath)
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


void listallfiles(RB_TREE_S *pstTree, char * pathname)
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

        /* 递归遍历指定目录子目录下的所有文件 */
        if(DT_DIR == ptr->d_type)
        {
            listallfiles(path);
        }
        if(DT_REG == ptr->d_type)
        {
            sprintf(fullname, "%s/%s", pathname, ptr->d_name);
            /* 获取文件的大小 */
            uiFileSize = GetFileSize(fullname);
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

RB_TREE_S* GetCacheTree()
{
   return g_pstTree;
}

void SetCacheTree(RB_TREE_S *pstTree)
{
   g_pstTree = pstTree;
   return;
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


unsigned int RequestProc(char* pcPath)
{
   RB_TREE_S *pstTree = NULL;

   CACHE_NODE_S* pstCache = NULL;
   pstTree = GetCacheTree();
   if (NULL != pstTree)
   {
      return FAILED;
   }

   pstCache = CacheNodeFind(pstTree, pcPath);

   if (NULL == pstCache)
   {
     /* 从硬盘读取数据 */
   }

   GetVaildBuff(pstCache);
   /* 直接将数据读取保存在树中 */
   WriteDataToBuff(pcPath, pstCache);
   
}

int main()
{
    RB_TREE_S *pstTree = NULL;
    CacheTreeInit();
    CacheBuffInit();

    pstTree = GetCacheTree();
    listallfiles(pstTree, "/opt/apache-tomcat-6.0.36/webapps/website");
    return 0;
}
