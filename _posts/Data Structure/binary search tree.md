## The Binary Search Tree

### 性质
left < middle < right

### 基本操作
Find, FindMin, FindMax, Insert, Delete in <a href="http://www.codecogs.com/eqnedit.php?latex=O(\log&space;n)" target="_blank"><img src="http://latex.codecogs.com/png.latex?O(\log&space;n)" title="O(\log n)" /></a> on average

### 代码

```c
#ifndef _Tree_H

struct TreeNode;
typedef struct TreeNode *Position;
typedef struct TreeNode *SearchTree;

SearchTree MakeEmpty(SearchTree T);
Position Find(ElementType X, SearchTree T);
Position FindMin(SearchTree T);
Position FindMax(SearchTree T);
SearchTree Insert(ElementType X, SearchTree T);
SearchTree Delete(ElementType X, SearchTree T);
ElementType Retrieve(Position P);

#endif

struct TreeNode
{
    ElementType Element;
    SearchTree Left;
    SearchTree Right;
};

SearchTree MakeEmpty(SearchTree T)
{
    if (T != NULL)
    {
        MakeEmpty(T->Left);
        MakeEmpty(T->Right);
        free(T);
    }
    return NULL;
}

Position Find(ElementType X, SearchTree T)
{
    if (T == NULL)
        return NULL;
    else if (X < T->Element)
        return Find(X, T->Left)
    else if (X > T->Element)
        return Find(X, T->Right)
    else
        return T;
}

Position FindMin(SearchTree T)
{
    if (T == NULL)
        return NULL;
    else if (T->Left == NULL)
        return T;
    else
        return FindMin(T->Left);
}

Position FindMax(SearchTree T)
{
    if (T != NULL)
        while (T->Right != NULL)
            T = T->Right;
    return T;
}

SearchTree Insert(ElementType X, SearchTree T)
{
    if (T == NULL)
    {
        Position Tmp;
        Tmp = (Position)malloc(sizeof(struct TreeNode));
        if (Tmp != NULL)
            Tmp->Element = X;
            Tmp->Left = Tmp->Right = NULL;
        else
            Error("out of space!!");
    }
    else if (X < T->Element)
        T->Left = Insert(X, T->Left);
    else if (X > T->Element)
        T->Right = Insert(X, T->Right);
    return T;
}

SearchTree Delete(ElementType X, SearchTree T)
{
    Position TmpCell;

    if (T == NULL)
        Error("Element not found.");
    else if (X < T->Element)
        T->Left = Delete(X, T->Left);
    else if (X > T->Element)
        T->Right = Delete(X, T->Right);
    else if (T->Left && T->Right)
    {
        TmpCell = FindMin(T->Right);
        T->Element = TmpCell->Element;
        T->Right = Delete(T->Element, T->Right);
    }
    else
    {
        TmpCell = T;
        if (T->Left)
            T = T->Left;
        else if (T->Right)
            T = T->Right;
        free(TmpCell);
    }
    return T;
}
```

### 注意点

- Find: 先检查T为NULL的情况，即找不到则返回NULL。
- Insert: 返回值是新树的根。
每次递归都将子树当作新树的根调用，并返回给当前树。
- Delete: 找到要删除的结点后有三种情况：
    1. 结点是叶子：直接删除。
    2. 结点有一个子结点：将结点替换为它的子结点。
    3. 结点有两个子结点：用右子树中最小的结点值代替它的值，然后递归地delete这个新值。

### 问题

1. 这种Delete算法会倾向于让左子树比右子树深。
    <br>解决方法：从左子树最大元素和右子树最小元素中随机选择。

2. 对于预先排序过的输入，一连串的Insert会使树不平衡，朝一边倾斜。
    <br>解决方法: <br>1) balanced search tree: the AVL tree
    <br>2) self-adjusting: the splay tree
    这两种树都属于二叉搜索树。

### lazy deletion
- When an element is to be deleted, it is left in the tree and merely marked as being deleted. This is especially popular if duplicate keys are present, because then the field that keeys count of the frequency of apperance can be decremented.
- If the number of real nodes in the tree is only expected to go up by a small constant, so there is a very small time penalty associated with lazy deletion.
- Also, if a deleted key is reinserted, the overhead of allocating a new cell is avoided.
