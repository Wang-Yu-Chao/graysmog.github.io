# 红黑树

## 性质

### 红黑性质

1. 每个结点或是红色的，或是黑色的。
2. 根结点是黑色的。
3. 每个叶结点（NIL）是黑色的。
4. 如果一个结点是红色的，则它的两个子结点都是黑色的。
5. 对每个结点，从该结点到其所有后代叶结点的简单路径上，均包含相同数目的黑色结点。

### 树高性质

- **黑高**：从某个结点x出发（不含该结点）到达一个叶结点的任意一条简单路径上的黑色结点个数称为该结点的黑高。
- 一棵有n个内部结点的红黑树的高度至多为2lg(n+1)。

### 操作

由树高性质可知，动态集合操作SEARCH、MINIMUM、MAXIMUM、SUCCESSOR、PREDECESSOR可在红黑树上在O(lgn)时间内执行。
一般二叉搜索树的INSERT和DELETE并不适用于红黑树，因为可能会破坏红黑性质。

### 哨兵

- 为了便于处理红黑树代码中的边界条件，使用一个哨兵来代表NIL。对于一棵红黑树T，哨兵T->nil是一个与树中普通结点有相同属性的对象。它的color属性为BLACK，而其他属性p、left、right和key可以设为任何值。所有指向NIL的指针都用指向哨兵T->nil的指针替换。

### 头文件

```c
/* 头文件 */
\#ifndef _RBTree_H
#define RED 1
#define BLACK 0

struct RBNODE_RECORD;
typedef RBNODE_RECORD *RBNODE;
struct RBNODE_RECORD
{
    RBNODE p;
    RBNODE left;
    RBNODE right;
    int key;    // 这里ElementType用int代替
    bool color;
};

typedef struct
{
    RBNODE root;
    RBNODE nil;
} *RBTREE;
/* RBTREE需要初始化 */
#endif
```

## 旋转

旋转是一种能保持二叉搜索树性质的搜索树局部操作。

```c
/* 左旋 */
void LEFT-ROTATE(RBTREE T, RBNODE x)
{
    RBNODE y;

    /* 设置x的right为y \*/
    y = x->right;
    /* 让y的left成为x的right \*/
    x->right = y->left;
    if (y->left != T->nil)
        y->left->p = x;
    /* 交换p \*/
    y->p = x->p;
    if (x->p == T->nil)
        T->root = y;
    else if (x == x->p->left)
        x->p->left = y;
    else
        x->p->right = y;
    /* 让x成为y的left \*/
    y->left = x;
    x->p = y;
}
```

## 插入

```c
void RB-INSERT(RBTREE T, RBNODE z)
{
    RBNODE y;
    RBNODE X;

    y = T->nil;
    x = T->root;
    while (x != T->nil)
    {
        y = x;
        if (z->key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    z->p = y;
    if (y == T->nil)
        T->root = z;
    else if (z->key < y->key)
        y->left = z;
    else
        y->right = z;
    z->left = T.nil;
    z->right = T.nil;
    z->color = RED;
    RB-INSERT-FIXUP(T, z);
}

void RB-INSERT-FIXUP(RBTREE T, RBNODE z)
{
    RBNODE y;

    while (z->p->color == RED)
    {
        /* 对称情况一 \*/
        if (z->p == z->p->p->left)
        {
            y = z->p->p->right;
            /* case 1 \*/
            if (y->color == RED)
            {
                z->p->color = BLACK;
                y->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            }
            /* case 2 \*/
            else if (z == z->p->right)
            {
                z = z->p;
                LEFT-ROTATE(T, z);
            }
            /* case 3 \*/
            else
            {
                z->p->color = BLACK;
                z->p->p->color = RED;
                RIGHT-ROTATE(T, z->p->p);
            }
        }

        /* 对称情况二 \*/
        else
        {
            y = z->p->p->left;
            /* case 1 \*/
            if (y->color == RED)
            {
                z->p->color = BLACK;
                y->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            }
            /* case 2 \*/
            else if (z == z->p->left)
            {
                z = z->p;
                RIGHT-ROTATE(T, z->p);
            }
            /* case 3 \*/
            else
            {
                z->p->color = BLACK;
                z->p->p->color = RED;
                LEFT-ROTATE(T, z->p->p);
            }
        }

        T->root->color = BLACK;
    }    
}    
```
