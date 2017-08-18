## Stack

### 基本操作

Push, Pop, Top

### 头文件

```c
#ifndef _Stack_h

struct Node;
typedef struct Node *PtrToNode;
typedef PtrToNode Stack;

int IsEmpty(Stack S);
Stack CreateStack(void);
void DisposeStack(Stack S);
void MakeEmpty(Stack S);
void Push(ElementType X, Stack S);
ElementType Top(Stack S);
void Pop(Stack S);

#endif
```

### 链表实现(Linked List Implementation)

Header作为Stack的指针，栈顶在头部（Header之后），位置不变。

```c
/* Linked list implementation */

typedef int ElementType;

struct Node
{
    ElementType Element;
    PtrToNode Next;
};

int IsEmpty(Stack S)
{
    return S->Next == NULL;
}

Stack CreateStack(void)
{
    Stack S;

    S = (Stack) malloc(sizeof(struct Node));
    if (S == NULL)
        FatalError("out of space!!!");
    MakeEmpty(S);
    return S;
}

void MakeEmpty(Stack S)
{
    if (S == NULL)
        Error("Must use CreateStack first");
    else
        while (!IsEmpty(S))
            Pop(S);
}

void Push(ElementType X, Stack S)
{
    PtrToNode TmpCell;

    TmpCell = (PtrToNode) malloc(sizeof(struct Node));
    if (TmpCell == NULL)
        FatalError("out of space!!!");
    else
    {
        TmpCell->Element = X;
        TmpCell->Next = S->Next;
        S->Next = TmpCell;
    }
}

ElementType Top(Stack S)
{
    if (IsEmpty(S))
        Error("Stack is empty");
    else
        return S->Next->Element;
}

void Pop(Stack S)
{
    PtrToNode TmpCell;

    if (IsEmpty(S))
        Error("Stack is empty");
    else
    {
        TmpCell = S->Next;
        S->Next = TmpCell->Next;
        free(TmpCell);
    }
}
```


### 数组实现(Array Implementation)
- 栈作为一个结构，其中有栈的大小，栈顶指针和数组（栈主体）三个元素。
- 需要提前声明数组的大小，所以应尽可能大。
- 用TopOfStack（索引）指明栈顶的位置，所以栈顶不断变化。需要错误检查：空栈、满栈。

```c
/* Array implementation */

struct StackRecord;
typedef struct StackRecord *Stack;

#define EmptyTOS (-1)
#define MinStackSize (5)

typedef int ElementType;

struct StackRecord
{
    int Capacity;
    int TopOfStack;
    ElementType *Array;
};

Stack CreateStack(int MaxElements)
{
    Stack S;

    if (MaxElements < MinStackSize)
        Error("Stack size is too small");

    S = (Stack) malloc(sizeof(struct StackRecord));
    if (S == NULL)
        FatalError("out of space");

    S->Array = (ElementType *) malloc(sizeof(ElementType) * MaxElements);
    if (S->Array == NULL)
        FatalError("out of space");
    S->Capacity = MaxElements;
    MakeEmpty(Stack S);
    return S;
}

int IsEmpty(Stack S)
{
    return S->TopOfStack == EmptyTOS;
}

int IsFull(Stack S)
{
    return S->TopOfStack == S->Capacity-1;
}

void DisposeStack(Stack S)
{
    free(S->Array);
    free(S);
}

void MakeEmpty(Stack S)
{
    S->TopOfStack = EmptyTOS;
}

void Push(ElementType X, Stack S)
{
    if (!IsFull(S))
        S->Array[++TopOfStack] = X;
    else
        Error("Full Stack");
}

ElementType Top(Stack S)
{
    if (!IsEmpty(S))
        return S->Array[TopOfStack];
    else
        Error("Empty Stack");
}

void Pop(Stack S)
{
    if (IsEmpty(S))
        Error("Empty Stack");
    else
        S->TopOfStack--;
}

ElementType TopAndPop(Stack S)
{
    if (IsEmpty(S))
        Error("Empty Stack");
    else
        return S->Array[TopOfStack--];
}
```
