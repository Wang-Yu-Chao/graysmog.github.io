## List

### 基本操作

Find, Insert, Delete, FindKth

Array Implementation:
- Find: O(n)
- FindKth: O(1)
- Insert/Delete: O(n)
- Building with n successive inserts: O(n^2)

Linked Lists:
- Find: O(n)
- FindKth(i): O(i)
- Delete: O(n)

### 细节
1. A sentinel node (Header)
避免从list的开端insert/delete而丢失整个list。
2. FindPrevious
用于Delete操作，时间复杂度为O(n)。

### 易错点
1. 空指针，如未初始化的指针，未分配空间的指针。
2. malloc。加入stdlib.h，记得释放指针，并且该指针不能再直接使用。

### 代码：链表实现

```c

#ifndef _List_h

struct Node;
typedef struct Node *PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;

List MakeEmpty(List L);
int IsEmpty(List L);
int IsLast(Position P, List L);
Position Find(ElementType X, List L);
void Delete(ElementType X, List L);
Position FindPrevious(ElementType X, List L);
void Insert(ElementType X, List L, Position P);
void DeleteList(List L);
Position Header(List L);
Position First(List L);
Position Advance(Position P);
ElementType Retreve(Position P);

#endif

/* Place in implementation file */

typedef int ElementType;

struct Node
{
    ElementType Element;
    Node Next;
}

int IsEmpty(List L)
{
    return L->Next == NULL;
}

int IsLast(Postion P, List L)
{
    return P->Next == NULL;
}

Position Find(ElementType X, List L)
{
    Position P;

    P = L->Next;
    while (P != NULL && P->Element != X)
        P = P->Next;

    return P;

}

void Delete(ElementType X, List L)
{
    Position P, TmpCell;

    P = FindPrevious(X, L);
    if (!IsLast(P, L))
    {
        TmpCell = P->Next;
        P->Next = TmpCell->Next;
        free(TmpCell);
    }
}

Position FindPrevious(ElementType X, List L)
{
    Position P;

    P = L;
    while (P->Next != NULL && P->Next->Element != X)
        P = P->Next;

    return P;
}

void Insert(ElementType X, List L, Position P)
{
    Position TmpCell;

    TmpCell = (Position) malloc(sizeof(struct Node));
    if (TmpCell == NULL)
        FatalError("out of space!!!");
    else
    {
        TmpCell->Element = X;
        TmpCell->Next = P->Next;
        P->Next = TmpCell;
    }
}
```
