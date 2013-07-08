#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "rb_tree.h"

/*-----------------------------------------------------------

|   node           right

|   / \    ==>     / \

|   a  right     node  y

|       / \      / \ 

|       b  y    a    b 

 -----------------------------------------------------------*/

/* **********************************************************
  * Function Name : rb_Rotate_Left
  * Description   : 函数主要实现红黑树调整过程中左旋的操作
                    需要调整的节点是node
  * Author        : Internet
  * Input/OutPut  :
  * Return        :

********************************************************** */
static void rb_Rotate_Left(RB_NODE_S *pstNode, RB_TREE_S *pstTree)
{
    RB_NODE_S  *pstRight = pstNode->pstRight;

    /* linux内核代码的写法 不推荐 */

    /*
     * node的右节点的左节点，成为待调整的节点的有节点
    */
    if ((pstNode->pstRight = pstRight->pstLeft))
    {
        pstRight->pstLeft->pstParent = pstNode;
    }

    pstRight->pstLeft = pstNode;

    /*
     * 待调整父节点成为新节点的父节点，如果父节点为NULL
     * 说明原来Node是树根
    */
    if ((pstRight->pstParent = pstNode->pstParent))
    {
        if (pstNode == pstNode->pstParent->pstRight)
        {
            pstNode->pstParent->pstRight = pstRight;

        }
        else
        {
            pstNode->pstParent->pstLeft = pstRight;
        }
    }
    else
    {
        pstTree->pstRoot = pstRight;
    }

    /* 更新待调整节点的父节点 */
    pstNode->pstParent = pstRight;

    return ;
}

/*-----------------------------------------------------------

|       node           left

|       / \            / \

|    left  y   ==>    a   node

|   / \                   / \

|  a   b                 b   y

-----------------------------------------------------------*/
/* **********************************************************
  * Function Name : rb_Rotate_Right
  * Description   : 函数主要实现红黑树调整过程中右旋的操作
                    右旋和左旋很相近
  * Author        : Internet
  * Input/OutPut  :
  * Return        :

********************************************************** */
static void rb_Rotate_Right(RB_NODE_S *pstNode, RB_TREE_S *pstTree)
{
    RB_NODE_S* pstLeft = pstNode->pstLeft;

    if ((pstNode->pstLeft = pstLeft->pstRight))
    {

        pstLeft->pstRight->pstParent = pstNode;
    }

    pstLeft->pstRight = pstNode;

    if ((pstLeft->pstParent = pstNode->pstParent))
    {
        if (pstNode == pstNode->pstParent->pstRight)
        {
            pstNode->pstParent->pstRight = pstLeft;
        }
        else
        {
            pstNode->pstParent->pstLeft = pstLeft;
        }
    }
    else
    {
        pstTree->pstRoot = pstLeft;
    }

    pstNode->pstParent = pstLeft;

    return ;
}

/* **********************************************************
  * Function Name : rb_Insert_Rebalance
  * Description   : 函数主要实现红黑树插入节点之后平衡调整
  * Author        : Internet
  * Input/OutPut  :
  * Return        :
********************************************************** */
static void rb_Insert_Rebalance(RB_NODE_S *pstNode, RB_TREE_S* pstTree)

{
    RB_NODE_S  *pstParent = NULL; 
    RB_NODE_S  *pstGparent = NULL;   /* 祖父节点*/ 
    RB_NODE_S  *pstUncle = NULL;     /* 叔叔节点*/
    RB_NODE_S  *pstTmp = NULL;

    while ((pstParent = pstNode->pstParent) && (pstParent->enColor == RED))
    {
        pstGparent = pstParent->pstParent;  /* 祖父节点赋值 */

        if (NULL == pstGparent)
        {
           break;
        }

        if (pstParent == pstGparent->pstLeft)
        {
            pstUncle = pstGparent->pstRight;

            if ((NULL != pstUncle) && (pstUncle->enColor == RED))
            {
                pstUncle->enColor = BLACK;

                pstParent->enColor = BLACK;

                pstGparent->enColor = RED;

                pstNode = pstGparent;
            }
            else
            {
                if (pstParent->pstRight == pstNode)
                {
                    rb_Rotate_Left(pstParent, pstTree);
                    pstTmp = pstParent;
                    pstParent = pstNode;
                    pstNode = pstTmp;
                }
                pstParent->enColor = BLACK;
                pstGparent->enColor = RED;
                rb_Rotate_Right(pstGparent, pstTree);
            }
        } 
        else 
        {
            pstUncle = pstGparent->pstLeft;
            if ((NULL != pstUncle) && (pstUncle->enColor == RED))
            {
                pstUncle->enColor = BLACK;
                pstParent->enColor = BLACK;
                pstGparent->enColor = RED;
                pstNode = pstGparent;
            }
            else
            {
                if (pstParent->pstLeft == pstNode)
                {
                    rb_Rotate_Right(pstParent, pstTree);
                    pstTmp = pstParent;
                    pstParent = pstNode;
                    pstNode = pstTmp;
                }
                pstParent->enColor = BLACK;
                pstGparent->enColor = RED;
                rb_Rotate_Left(pstGparent, pstTree);
            }
        }
    }

    /* 保证父节点的颜色是黑色 */
    pstTree->pstRoot->enColor = BLACK;

    return ;
}

/* **********************************************************
  * Function Name : rb_Erase_Rebalance
  * Description   : 函数主要实现红黑树插入节点之后平衡调整
  * Author        : Internet
  * Input/OutPut  :
  * Return        :

********************************************************** */
static void rb_Erase_Rebalance(RB_NODE_S *pstNode, RB_NODE_S *pstParent, RB_TREE_S *pstTree)
{
    RB_NODE_S *pstOther = NULL; 
    RB_NODE_S *pstOther_Left = NULL;
    RB_NODE_S *pstOther_Right = NULL;

    while (((NULL == pstNode) || (BLACK == pstNode->enColor)) && 
           (pstNode != pstTree))
    {
        if (pstParent->pstLeft == pstNode)
        {
            pstOther = pstParent->pstRight;
            if (pstOther->enColor == RED)
            {
                pstOther->enColor = BLACK;
                pstParent->enColor = RED;
                rb_Rotate_Left(pstParent, pstTree);
                pstOther = pstParent->pstRight;
            }

            if (((NULL == pstOther->pstLeft) ||( BLACK == pstOther->pstLeft->enColor)) &&

                ((NULL == pstOther->pstRight) ||(BLACK == pstOther->pstRight->enColor)))
            {
                pstOther->enColor = RED;
                pstNode = pstParent;
                pstParent = pstNode->pstParent;
            }
            else
            {
                if (!pstOther->pstRight || pstOther->pstRight->enColor == BLACK)
                {
                    if ((pstOther_Left = pstOther->pstLeft))
                    {
                        pstOther_Left->enColor = BLACK;
                    }
                    pstOther->enColor = RED;
                    rb_Rotate_Right(pstOther, pstTree);
                    pstOther = pstParent->pstRight;
                }
                pstOther->enColor = pstParent->enColor;
                pstParent->enColor = BLACK;
                if (pstOther->pstRight)
                {
                    pstOther->pstRight->enColor = BLACK;
                }
                rb_Rotate_Left(pstParent, pstTree);
                pstNode = pstTree->pstRoot;

                break;
            }
        }
        else
        {
            pstOther = pstParent->pstLeft;
            if (RED == pstOther->enColor)
            {
                pstOther->enColor = BLACK;
                pstParent->enColor = RED;
                rb_Rotate_Right(pstParent, pstTree);
                pstOther = pstParent->pstLeft;
            }
            if (((!pstOther->pstLeft) || (BLACK == pstOther->pstLeft->enColor)) &&
                ((!pstOther->pstRight) || (BLACK == pstOther->pstRight->enColor)))
            {
                pstOther->enColor = RED;
                pstNode = pstParent;
                pstParent = pstNode->pstParent;
            }
            else
            {
                if ((!pstOther->pstLeft) || (BLACK == pstOther->pstLeft->enColor))
                {
                    if ((pstOther_Right = pstOther->pstRight))
                    {
                        pstOther_Right->enColor = BLACK;
                    }
                    pstOther->enColor = RED;
                    rb_Rotate_Left(pstOther, pstTree);
                    pstOther = pstParent->pstLeft;
                }
                pstOther->enColor = pstParent->enColor;
                pstParent->enColor = BLACK;
                if (pstOther->pstLeft)
                {
                    pstOther->pstLeft->enColor = BLACK;
                }
                rb_Rotate_Right(pstParent, pstTree);
                pstNode = pstTree->pstRoot;

                break;
            }
        }
    }

    if (pstNode)
    {
        pstNode->enColor = BLACK;
    } 

    return ;
}

/* **********************************************************
  * Function Name : rb_Search_Auxiliary
  * Description   : 函数主要实现待插入数据的查找
  * Author        : Internet
  * Input/OutPut  :
  * Return        :

********************************************************** */
static RB_NODE_S* rb_Search_Auxiliary(RB_NODE_S *pstDataNode, RB_TREE_S* pstTree, RB_NODE_S  ** pstSave)

{
    RB_NODE_S *pstNode = pstTree->pstRoot; 
    RB_NODE_S *pstParent = NULL;

    int ret = 0;

    while (pstNode)
    {
        pstParent = pstNode;

        ret = pstTree->pfCompare(pstDataNode, pstNode);

        if (0 < ret)
        {
            pstNode = pstNode->pstLeft;
        }
        else if (0 > ret)
        {
            pstNode = pstNode->pstRight;
        }
        else
        {
            return pstNode;
        }
    }

    if (pstSave)
    {
        *pstSave = pstParent;
    }

    return NULL;
}

/* **********************************************************
  * Function Name : rb_Successor
  * Description   : 查找待删除节点的前驱
  * Author        : Internet
  * Input/OutPut  :
  * Return        :

********************************************************** */
RB_NODE_S* rb_Successor(RB_NODE_S *pstNodeData, RB_NODE_S **ppstParent, RB_TREE_S *pstTree)
{
    RB_NODE_S *pstNode = NULL;
    RB_NODE_S *pstLeft = NULL;
    RB_NODE_S *pstChild = NULL;
    RB_NODE_S *pstParent = NULL;  
    COLOR_E       enColor;
    RB_NODE_S *pstOld = pstNodeData; 

    /*
      * 之前后驱是右子树中最左边的节点
      * 获取直接后驱节点
    */
    pstNode = pstNodeData->pstRight;
    while (NULL != (pstLeft = pstNode->pstLeft))
    {
        pstNode = pstLeft;
    }

   /* 记录后驱节点的基本信息 */
    pstChild = pstNode->pstRight;
    pstParent = pstNode->pstParent;
    enColor = pstNode->enColor;
    
    if (pstChild)
    {
        pstChild->pstParent = pstParent;
    }
    
    if (pstParent)
    {
        if (pstParent->pstLeft == pstNode)
        {
            pstParent->pstLeft = pstChild;
        }
        else
        {
            pstParent->pstRight = pstChild;
        }
    }
    else 
    {
        /* 只有根结点的父节点才是空节点 */
        pstTree->pstRoot = pstChild;
    }
    
    if (pstNode->pstParent == pstOld)
    {
        pstParent = pstNode;
    }

    /*
      * 将点删除节点信息复制给新的节点
    */
    pstNode->pstParent = pstOld->pstParent;
    pstNode->enColor = pstOld->enColor;
    pstNode->pstRight = pstOld->pstRight;
    pstNode->pstLeft = pstOld->pstLeft;
    
    if (pstOld->pstParent)
    {
        if (pstOld->pstParent->pstLeft == pstOld)
        {
             pstOld->pstParent->pstLeft = pstNode;
        }
        else
        {
            pstOld->pstParent->pstRight = pstNode;
        }
    } 
    else
    {
        pstTree->pstRoot = pstNode;
    }

    /* 函数调用已经保证pstOld的左右子树不为NULL */
    pstOld->pstLeft->pstParent = pstNode;
    
    pstOld->pstRight->pstParent = pstNode;
    *ppstParent = pstParent;

    return pstChild;
}


void rb_DeleteNode(RB_NODE_S *pstNode, RB_TREE_S *pstTree)
{
    if (NULL != pstNode->pstLeft)
    {
       rb_DeleteNode(pstNode->pstLeft, pstTree);
    }
    if (NULL != pstNode->pstRight)
    {
        rb_DeleteNode(pstNode->pstRight, pstTree);
    }

    pstTree->pfFree(pstNode);

    return;

}

/* **********************************************************
  * Function Name : RB_Insert
  * Description   : 函数主要实现红黑树中插入节点
  * Author        : Internet
  * Input/OutPut  :
  * Return        :

********************************************************** */
unsigned int RB_Insert(RB_NODE_S *pstData, RB_TREE_S* pstTree)
{
    RB_NODE_S   *pstParent = NULL; 
    RB_NODE_S   *pstNode = NULL;
    unsigned int uiRet = RB_OPEAT_SUCCESS;
    int  iComRet = 0;

    /* 查找当前节点是否已经存在 */
    if ((pstNode = rb_Search_Auxiliary(pstData, pstTree, &pstParent)))
    {
        return RB_INSERT_EXITS;
    }

    /* 新插入的节点颜色置为红色 */
    pstNode = pstData;
    pstNode->pstParent = pstParent; 
    pstNode->enColor = RED;

    if (pstParent)
    {
        iComRet = pstTree->pfCompare(pstData, pstParent);
        if (0 < iComRet)
        {
            pstParent->pstLeft = pstNode;
        }
        else if (0 > iComRet)
        {
            pstParent->pstRight = pstNode;
        }
    }
    else
    {
       pstTree->pstRoot = pstNode;
    }

    /* 该函数还有调整节点颜色的功能，红黑树根节点必须是黑的 */
    rb_Insert_Rebalance(pstNode, pstTree);

    pstTree->uiCount +=1 ;

    return uiRet;

}


/* **********************************************************
  * Function Name : RB_Search
  * Description   : 函数主要实现红黑树中指定节点的查找
  * Author        : Internet
  * Input/OutPut  :
  * Return        :

********************************************************** */
RB_NODE_S * RB_Search(RB_NODE_S *pstData, RB_TREE_S* pstTree)
{
    return rb_Search_Auxiliary(pstData, pstTree, NULL);
}

/* **********************************************************
  * Function Name : RB_Erase
  * Description   : 函数主要实现红黑树中指定节点的删除
  * Author        : Internet
  * Input/OutPut  :
  * Return        :

********************************************************* */
unsigned int RB_Erase(RB_NODE_S *pstData, RB_TREE_S *pstTree)
{

    RB_NODE_S *pstChild = NULL;
    RB_NODE_S *pstParent = NULL; 
    RB_NODE_S *pstOld = NULL; 
    RB_NODE_S *pstLeft = NULL; 
    RB_NODE_S *pstNode = NULL;
    unsigned int uiRet = RB_OPEAT_SUCCESS;
    COLOR_E       enColor;

    if (!(pstNode = rb_Search_Auxiliary(pstData, pstTree, NULL)))
    {
        uiRet = RB_ERASE_NOT_EXITS;
        return uiRet;
    }

    pstOld = pstNode;

    if ((NULL != pstNode->pstLeft) && (NULL != pstNode->pstRight))
    {
        /* 找到新的节点代替待删除节点 */
       pstChild = rb_Successor(pstNode, &pstParent, pstTree);
    }
    else
    {
        if (!pstNode->pstLeft)
        {
            pstChild = pstNode->pstRight;
        }
        else if (!pstNode->pstRight)
        {
            pstChild = pstNode->pstLeft;
        }

        /*  pstChild是新选举的节点 */
        pstParent = pstNode->pstParent;
        enColor = pstNode->enColor;

        if (pstChild)
        {
            pstChild->pstParent = pstParent;
        }

        if (pstParent)
        {
            if (pstParent->pstLeft == pstNode)
            {
                pstParent->pstLeft = pstChild;
            }
            else
            {
                pstParent->pstRight = pstChild;
            }
        }
        else
        {
            pstTree->pstRoot = pstChild;
        }
    }

    pstTree->pfFree(pstOld);

    if (enColor == BLACK)
    {
        rb_Erase_Rebalance(pstChild, pstParent, pstTree);
    }
    pstTree->uiCount -= 1;
    
    return uiRet;

}

/* **********************************************************
  * Function Name : RB_Init
  * Description   : 实现红黑树的初始化，创建一个树
  * Author        : Internet
  * Input/OutPut  :
  * Return        :

********************************************************* */
RB_TREE_S * RB_Init(RB_NODE_COMPARE pfCompare, RB_NODE_FREE pfFree)
{
    RB_TREE_S *pstTree = NULL;

    pstTree = (RB_TREE_S*)malloc(sizeof(RB_TREE_S));
    if (NULL != pstTree)
    {
       memset(pstTree , 0, sizeof(RB_TREE_S));
       pstTree->pfCompare = pfCompare;
       pstTree->pfFree = pfFree;
    }

    return pstTree;
}

/* **********************************************************
  * Function Name : RB_DeInit
  * Description   : 实现红黑树的去初始化释放所有的内存
  * Author        : Internet
  * Input/OutPut  :
  * Return        :

********************************************************* */
void RB_DeInit(RB_TREE_S *pstTree)
{
    if (NULL == pstTree->pstRoot)
    {
        return ;
    }

    rb_DeleteNode(pstTree->pstRoot, pstTree);

    free(pstTree);
    pstTree = NULL;

    return;
}


