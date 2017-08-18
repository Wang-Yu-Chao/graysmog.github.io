## Splay Tree

### 性质
- Any M consecutive tree operations starting from an empty tree take at most <a href="http://www.codecogs.com/eqnedit.php?latex=O(M\log&space;n)" target="_blank"><img src="http://latex.codecogs.com/png.latex?O(M\log&space;n)" title="O(M\log n)" /></a> time.
- 当一个结点被查找到后，将它旋转到树根。

### 关键点
- 根据X, P, G的关系分三种情况进行rotate（P为X的父结点，G为X的子结点）:
    1. 如果P是树根，仅交换X和P。
    2. 如果X是左子结点而P是右子结点（对称情况亦然），则进行一次avl树的double rotation，即zig-zag。
    3. 如果X和P都是左子结点（对称情况亦然），则进行两次avl树的single rotation，即zig-zig。
- SplayNode结构中加入Parent。
- Insert是普通的insert，但是会改变Parent性质。
- Delete通过将要删除的结点移到树根，然后将左子树TL的最大结点移到左子树根，右子树TR作为左子树TL的右子树，最后释放树根结点。

### 代码

```c
#ifndef

struct SplayNode;
typedef struct SplayNode *SplayTree;
typedef struct SplayNode *Position;

SplayTree MakeEmpty(SplayTree T);
Position Find(ElementType X, SplayTree T);
Position FindMax(SplayTree T);
Position FindMin(SplayTree T);
SplayTree Insert(ElementType A, SplayTree T);
SplayTree Delete(ElementType A, SplayTree T);
ElementType Retrieve(Position P);

#endif

struct SplayNode
{
    ElementType Element;
    SplayTree Left;
    SplayTree Right;
    SplayTree Parent;
};

SplayTree MakeEmpty(SplayTree T)
{
    if (T != NULL)
    {
        MakeEmpty(T->Left);
        MakeEmpty(T->Right);
        free(T);
    }
    return NULL;
}

static BinarySearch(ElementType X ,SplayTree T)
{
    if (T == NULL)
        return NULL;
    else if (X < T->Element)
        return BinarySearch(X, T->Left);
    else if (X > T->Element)
        return BinarySearch(X, T->Right);
    else
        return T;
}

static ChangeParent(Position P)
{
    P->Left->Parent = P->Right->Parent = P;
}

static Zig-zagWithLeft(SplayTree T)
{
    SingleRotateWithRight(T->Left);
    SingleRotateWithLeft(T);
}

static Zig-zigWithLeft(SplayTree T)
{
    Position P = T->Left;
    SingleRotateWithLeft(T);
    SingleRotateWithLeft(P);
}

static RotateToRoot(Position X)
{
    Position P, G;

    while (X->Parent != NULL)
    {
        P = X->Parent;
        if (P->Parent == NULL)
        {
            if (P->Left == X)
            {
                P->Left = X->Right;
                X->Right = P;
                ChangeParent(X); ChangeParent(P);
            }
            else
            {
                P->Right = X->Left;
                X->Left = P;
                ChangeParent(X); ChangeParent(P);
            }
        }
else
        {
            G = P->Parent;
            if (G->Left == P)
            {
                if (P->Left == X)
                {
                    Zig-zigWithLeft(G);
                    X->Parent = G->Parent;
                    ChangeParent(X); ChangeParent(P); ChangeParent(G);
                }
                else
                {
                    Zig-zagWithLeft(G);
                    X->Parent = G->Parent;
                    ChangeParent(X); ChangeParent(P); ChangeParent(G);
                }
            }
            else
            {
                if (P->Left == X)
                {
                    Zig-zigWithRight(G);
                    X->Parent = G->Parent;
                    ChangeParent(X); ChangeParent(P); ChangeParent(G);
                }
                else
                {
                    Zig-zagWithRight(G);
                    X->Parent = G->Parent;
                    ChangeParent(X); ChangeParent(P); ChangeParent(G);
                }
            }
        }
    }
}

Position Find(ElementType A, SplayTree T)
{
    Position X = BinarySearch(A, T);
    RotateToRoot(X);
    return X;
}

/* Normal Insert with Parent Property Changed */
SplayTree Insert(ElementType A, SplayTree T)
{
    if (A < T->Element)
    {
        if (T->Left == NULL)
        {
            T->Left = (SplayTree) malloc(sizeof(struct SplayNode));
            if (T->Left == NULL)
                Error("Out of space!");
            T->Left->Element = A;
            T->Left->Left = T->Left->Right = NULL;
            T->Left->Parent = T;
        }
        else
            T->Left = Insert(ElementType A, SplayTree T);
    }
    else if (A > T->Element)
    {
        if (T->Right == NULL)
        {
            T->Right = (SplayTree) malloc(sizeof(struct SplayNode));
            if (T->Right == NULL)
                Error("Out of space!");
            T->Right->Element = A;
            T->Right->Left = T->Right->Right = NULL;
            T->Right->Parent = T;
        }
        else
            T->Right = Insert(ElementType A, SplayTree T);
    }
    return T;
}

static Position BinarySearchMax(SplayTree T)
{
    while (T->Right != NULL)
        T = T->Right;
    return T;
}

static Position BinarySearchMin(SplayTree T)
{
    while (T->Left != NULL)
        T = T->Left;
    return T;
}

Position FindMax(SplayTree T)
{
    Position X = BinarySearchMax(T);
    RotateToRoot(X);
}

Position FindMin(SplayTree T)
{
    Position X = BinarySearchMin(T);
    RotateToRoot(X);
}

SplayTree Delete(ElementType A, SplayTree T)
{
    SplayTree X = Find(A, T);
    SplayTree TL = FindMax(X->Left);
    TL->Right = X->Right;
    free(X);
    return TL;
}
```
