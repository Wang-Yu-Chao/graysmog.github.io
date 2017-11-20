---
    author: GraySmog
    comments: true
    date: 2017-10-20
    layout: post
    title: Disjoint Set
    categories:
    - Data Structure
    - C
    tags:
    - data structure
    - c
    - blog
---

# 互斥集

## 关系（Relation)

- A relation R is defined on a set S if for every pair of elements (a, b), a, b belongs to S, a R b is either true or false. If a R b is true, then we say that a is related to b.
- An equivalence relation is a relation R that satisfies three properties:
    1. (Reflexive) a R a, for all a belongs to S.
    2. (Symmetric) a R b if and only if b R a.
    3. (Transitive) a R b and b R c implies that a R c

## 操作

**Find**: It is not necessary that a Find operation return any specific name, just that Finds on two elements return the same answer if and only if they are in the same set. A Find(X) on element X is performed by returning the root of the tree containing X. The worst-case running time of a Find is O(N).

```c
/* 头文件 */
#ifndef _DisjSet_H

typedef int DisjSet[NumSets + 1];
typedef int SetType;
typedef int ElementType;

void Initialize(DisjSet S);
void SetUnion(DisjSet S, SetType Root1, SetType Root2);
SetType Find(ElementType X, DisjSet S);

#endif

void Initialize(DisjSet S)
{
    int i;

    for (i = NumSets; i > 0; i--);
        S[i] = 0;
}

/* Assumes Root1 and Root2 are roots */
/* union is a C keyword, so this routine */
/* is named SetUnion */
void SetUnion(DisjSet S, SetType Root1, SetType Root2)
{
    S[Root2] = Root1;
}


SetType Find(ElementType X, DisjSet S)
{
    if (S[X] <= 0)
        return X;
    else
        return Find(S[X], S);
}
```

## 进阶的合并算法（Smart Union Algorithms）

- **Union-by-size**: A simple improvement is always to make the smaller tree a subtree of the larger, breaking ties by any method; we call this approach union-by-size. If Unions are done by size, the depth of any node is never more than logN. This implies that the running time for a Find operation is O(logN), and a sequence of M operations takes O(MlogN).
- **Union-by-height**: Union-by-height guarantees that all the trees will have depth at most O(logN). We keep track of the height, instead of the size, of each tree and perform Unions by making the shallow tree a subtree of the deeper tree. The height of a tree increases only when two equally deep trees are joined (and then the height goes up by one).

```c
void setUnion(DisjSet S, SetType Root1, SetType Root2)
{
    if (S[Root2] < S[Root1])
        S[Root1] = Root2;
    else
    {
        if (S[Root1] == S[Root2])
            S[Root1]--;
        S[Root2] = Root1;
    }
}
```

## （路径压缩）Path Compression

#### Algorithm

Path compression is performed during a Find operation and is independent of the strategy used to perform Unions. The effect of path compression is that every node on the path from X to the root has its parent changed to the root. And the fast future access on these nodes will pay for the extra word to do the path compression.  
The only change to the Find routine is that S[X] is made equal to the value returned by Find.

```C
SetType Find(ElementType X, DisjSet S)
{
    if (S[X] <= 0)
        return X;
    else
        return S[X] = Find(S[X], S);
}
```

#### Analysis

- If we use the Union/Find alogrithm, the worst case of `$O(Mlog{N})$` can occur fairly easily and naturally.
- So when *Unions* are done arbitrarily, path compression is a good idea, because there is an abundance of deep nodes. When path compression is done in this case, a sequence of M operations requires at most `$O(Mlog{N})$` time.
- Path compression is perfectly compatible with union-by-size, because path compression can change the heights of the trees. Then the heights stored for each tree become estimated heights (sometimes known as ranks), but it turns out that union-by-rank is just as efficient in theory as union-by-size.
- The worst case for union-by-rank and path compression: The running time of a sequence of M *Unions* and *Finds* is `$O(mlog^{*}n)$`
