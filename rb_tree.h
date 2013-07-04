#ifndef _RB_TREE_H_
#define _RB_TREE_H_

/*
  * 红黑树颜色枚举
*/

typedef enum color_e
{
    RED = 0,
    BLACK = 1
}COLOR_E;

/*
  * 红黑树节点数据结构的定义
*/
typedef struct rb_node_t
{
    struct rb_node_t      *pstLeft;
    struct rb_node_t      *pstRight; 
    struct rb_node_t      *pstParent;
    COLOR_E                    enColor;
}RB_NODE_S;

#define RB_OPEAT_SUCCESS    0
#define RB_OPEAT_FAILED     1
#define RB_INSERT_EXITS     2
#define RB_ERASE_NOT_EXITS  3
typedef int (*RB_NODE_COMPARE)(RB_NODE_S *pstNode1, RB_NODE_S *pstNode2);
typedef void(*RB_NODE_FREE)(RB_NODE_S *pstNode);


typedef struct rb_tree
{
    RB_NODE_S        *pstRoot;
    unsigned int     uiCount;
    RB_NODE_COMPARE  pfCompare;
    RB_NODE_FREE     pfFree;
   
}RB_TREE_S;

#ifdef __compiler_offsetof
#define offsetof(TYPE,MEMBER) __compiler_offsetof(TYPE,MEMBER)
#else
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif
#define container_of(ptr, type, member) ({          \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - offsetof(type,member) );})


#define RB_ENTRY(ptr, type, member)  (container_of(ptr, type, member))

/* forward declaration */

unsigned int RB_Insert(RB_NODE_S* pstNode, RB_TREE_S *pstTree);

RB_NODE_S* RB_Search(RB_NODE_S* pstNode, RB_TREE_S *pstTree);

unsigned int RB_Erase(RB_NODE_S* pstNode, RB_TREE_S *pstTree);

RB_TREE_S * RB_Init();

void RB_DeInit(RB_TREE_S *pstTree);


#endif

