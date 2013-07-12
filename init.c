#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>
#include<unistd.h>
#include"rb_tree.h"

#define MAX 512

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

int main(){
    RB_TREE_S *pstTree= NULL;

    pstTree = CacheTreeCreate();

    if (NULL == pstTree)
    {
       printf("Create cache failed\r\n");
       return 1;
    }
    
    listallfiles(pstTree, "/opt/apache-tomcat-6.0.36/webapps/website");
    return 0;
}
