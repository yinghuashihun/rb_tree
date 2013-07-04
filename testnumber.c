#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>

#include "rb_tree.h"

#define PATH_NUMBER 100000

typedef struct A
{
   RB_NODE_S stRbNode ;
   unsigned int uidata;
}DATA_S;


int KeyCompare(RB_NODE_S *pstData1, RB_NODE_S *pstData2)
{
    DATA_S* pstKey1 = RB_ENTRY(pstData1, DATA_S, stRbNode);
    DATA_S* pstKey2 = RB_ENTRY(pstData2, DATA_S, stRbNode);


    if (pstKey2->uidata > pstKey1->uidata)
    {
        return 1;
    }
    else if (pstKey2->uidata < pstKey1->uidata)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

void KeyFree(RB_NODE_S *pstData)
{
    DATA_S *pstKey = RB_ENTRY(pstData, DATA_S, stRbNode);
    free(pstKey);
    pstKey = NULL;
    
    return;
}

void DataFindTree(RB_TREE_S *pstTree, int iData)
{  
    RB_NODE_S *pstNode = NULL;

    DATA_S stData;
    DATA_S *pstData = NULL;

    memset(&stData, 0, sizeof(DATA_S));
    stData.uidata = iData;
    
    pstNode = RB_Search(&(stData.stRbNode), pstTree);
    if (NULL != pstNode)
    {
       pstData = RB_ENTRY(pstNode, DATA_S, stRbNode);
    }
    else
    {
       printf("Can't find the data in tree\r\n");
    }

    return;
}

void DataFind(int iData)
{  
    int i =0;
    for (i = 0; i < PATH_NUMBER; ++i)
    {
       if (i == iData)
       {
          break;
       }
    }

    return;
}


int main()
{
    int i, count = PATH_NUMBER;
    DATA_S    *pstData = NULL;
    RB_TREE_S *pstTree = NULL;
    char*      aucPath[PATH_NUMBER];
    char      *pPath = NULL;
    DATA_S     stData;
    unsigned int uiRet = 0;
    struct timeval stStart;
    struct timeval stStop;

    memset(&stData, 0, sizeof(DATA_S));

    pstTree = RB_Init(KeyCompare, KeyFree);

    if (NULL == pstTree)
    {
       printf("Create RB Tree Failed");
    }
   
    for (i = 1; i <= count; ++i)
    {
       pstData = (DATA_S *)malloc(sizeof(DATA_S));
       memset(pstData, 0, sizeof(DATA_S));

       pstData->uidata = i;

       RB_Insert(&(pstData->stRbNode), pstTree);
    }
    gettimeofday(&stStart, NULL);  
    for (i = 1; i <= count; ++i)
    {
       /* 查找指定的某一个 */
       DataFindTree(pstTree, i);
    }
    gettimeofday(&stStop, NULL);
    printf("Tree find the data number is %d use time:%f \r\n", count, 
           1000000*(stStop.tv_sec - stStart.tv_sec) + stStop.tv_usec - 
           stStart.tv_usec);

    gettimeofday(&stStart, NULL);  
    for (i = 1; i <= count; ++i)
    {
       /* 查找指定的某一个 */
       DataFind(100000-1);
    }
    gettimeofday(&stStop, NULL);
    printf("Area find the data number is %d use time:%f \r\n", count, 
           1000000*(stStop.tv_sec - stStart.tv_sec) + stStop.tv_usec - 
           stStart.tv_usec);


    stData.uidata = 999999;
    uiRet = RB_Erase(&(stData.stRbNode), pstTree);
    if (RB_OPEAT_SUCCESS == uiRet)
    {
       printf("Delete the data %d\r\n", stData.uidata);
    }
    RB_DeInit(pstTree);
    return 0;
}

