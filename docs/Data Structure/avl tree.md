**Avl Tree在插入元素时保持树的平衡。Splay Tree在查找元素时保持树的平衡。**

## Avl Tree

### 性质
- An Avl Tree is identical to a binary search tree, except that for every node in the tree, the height of the Left and right subtree can differ by at most 1.
- All the operation: <a href="http://www.codecogs.com/eqnedit.php?latex=O(\log&space;n)" target="_blank"><img src="http://latex.codecogs.com/png.latex?O(\log&space;N)" title="O(\log N)" /></a> time, except possibly insertion (assuming lazy deletion).

### 关键点
- Single Rotation, Double Rotation
- 当插入后树不平衡时，使用这两种（实际上有四种）rotation是树维持avl tree的性质。
- 注意Insert是通过递归实现的，现插入后旋转，所以检查性质是否改变和进行rotate都在postorder位置。

### 代码

```c
#ifndef

struct AvlNode;
typedef struct AvlNode *Position;
typedef struct AvlNode *AvlTree;

AvlTree MakeEmpty(AvlTree T);
Position Find(ElementType X, AvlTree T);
Position FindMax(AvlTree T);
Position FindMin(AvlTree T);
AvlTree Insert(ElementType X, AvlTree T);
AvlTree Delete(ElementType X, AvlTree T);
ElementType Retrieve(Position P);

#endif

struct AvlNode
{
    ElementType Element;
    AvlTree Left;
    AvlTree Right;
    int Height;
};

static int Height(Position P)
{
    if (P == NULL)
        return -1;
    else
        return P->Height;
}

/* Find, FindMin, FindMax are in accordance with Binary Search Tree */

AvlTree Insert(ElementType X, AvlTree T)
{
    if (T == NULL)
    {
        T = (AvlTree) malloc(sizeof(struct AvlNode));
        if (T == NULL)
            error("Out of space");
        else
        {
            T->Element = X;
            T->Left = T->Right = NULL;
            T->Height = 0;
        }   
    }   
    else if (X < T->Element)
    {   
        T->Left = Insert(X, T->Left);
        if (Height(T->Left) - Height(T->Right) == 2)
        {   
            if (X < T->Left->Element)
                SingleRotateWithLeft(T);
            else
                DoubleRotateWithLeft(T);
        }
    }
    else if (X > T->Element)
    {
        T->Right = Insert(X, T->Right);
        if (Height(T->Right) - Height(T->Left) == 2)
        {
            if (X > T->Right->Element)
                SingleRotateWithRight(T);
            else
                DoubleRotateWithRight(T);
        }
    }

    T->Height = Max(Height(T->Left), Height(T->Right)) + 1;

    return T;
}

void SingleRotateWithLeft(AvlTree K2)
{
    Position K1 = K2->Left;

    K2->Left = K1->Right;
    K1->Right = K2;

    /* Only the Height of K1 and K2 has been changed */
    K2->Height = Max(Height(K2->Left), Height(K2->Right)) + 1;
    K1->Height = Max(Height(K1->Left), Height(K1->Right)) + 1;
}

void DoubleRotateWithLeft(AvlTree K3)
{
    SingleRotateWithRight(K3->Left);
    SingleRotateWithLeft(K3);
}

/* Lazy Deletion */
```
