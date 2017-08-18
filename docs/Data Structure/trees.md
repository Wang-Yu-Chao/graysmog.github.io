## Trees

### 框架

- Tree Traversal: preorder, inorder, postorder traversal
- Binary Trees:
    - Binary Search Tree:
    - (Unbalanced) Binary Search Tree
    - (Balanced) AVL Tree: single/double rotation
    - (Self-adjusting) Slay Tree: zig-zig, zig-zag
    - B-Tree

### 基本概念

- 引入树的原因：Access time:
    - linked lists: <a href="http://www.codecogs.com/eqnedit.php?latex=O(n)" target="_blank"><img src="http://latex.codecogs.com/png.latex?O(n)" title="O(n)" /></a>
    - trees: <a href="http://www.codecogs.com/eqnedit.php?latex=O(\log&space;n)" target="_blank"><img src="http://latex.codecogs.com/png.latex?O(\log&space;n)" title="O(\log n)" /></a>
- tree属于DAG，有向无环图
- depth: the length of the path from the root to <a href="http://www.codecogs.com/eqnedit.php?latex=n_{i}" target="_blank"><img src="http://latex.codecogs.com/png.latex?n_{i}" title="n_{i}" /></a>.
- height: the length of the longedst path from <a href="http://www.codecogs.com/eqnedit.php?latex=n_{i}" target="_blank"><img src="http://latex.codecogs.com/png.latex?n_{i}" title="n_{i}" /></a> to a leaf.

### 一般表示

```c

typedef struct TreeNode *PtrToNode;
struct TreeNode
{
    ElementType Element;
    PtrToNode FirstChild;
    PtrToNode NextSibling;
};

```

### 树的遍历(Tree Traversal)

#### preorder

list a directory in a hierarchical file system

```c
static void ListDir(DirectoryOrFile D, int Depth)
{
    if (D is a legitimate entry)
    {
        PrintName(D, Depth);
        if (D is a directory)
            for each child C of D
                ListDir(C, Depth+1);
    }
}
void ListDirectory(DirectoryOrFile D)
{
    ListDir(D, 0);
}
```

#### postorder

calculate the size of a directory

```c
static void SizeDirectory(DirectoryOrFile D)
{
    int TotalSize;
    TotalSize = 0;
    if (D is a legitimate entry)
    {
        TotalSize = FileSize(D);
        if (D is a directory)
            for each child C, of D
                TotalSize += SizeDirectory(C);
    }
    return TotalSize;
}
```

#### inorder

expression tree
