## Hash

### 目的
1. 哈希(Hash)函数是一个映象，即： 将关键字的集合映射到某个地址集合上，它的设置很灵活，只要这个地址集合的大小不超出允许范围即可。
2. 由于哈希函数是一个压缩映象，因此，在一般情况下，很容易产生“冲突”现象，即： key1!=key2，而f(key1) = f(key2)。
3. 只能尽量减少冲突而不能完全避免冲突，这是因为通常关键字集合比较大，其元素包括所有可能的关键字， 而地址集合的元素仅为哈希表中的地址值。
在构造这种特殊的“查找表” 时，除了需要选择一个“好”(尽可能少产生冲突)的哈希函数之外；还需要找到一 种“处理冲突” 的方法

### 框架
- Hashing:
    - Hash Function
    - Hash Table (ADT):
        - Seperated Chaining
        - Open Adressing:
            - Linear
            - Quadratic
            - Double Hashing
- Rehashing
- Extensible Hashing

### Hash Function

```c
/* Hash function example
     The keys are strings */
typedef unsigned int Index;
Index Hash(const char *Key, int TableSize)
{
    unsigned int HashVal = 0;

    while (*Key != '\0')
        HashVal = (HashVal << 5) + *Key++;

    return HashVal % TableSize;
}
```

### Hash Table

- **Separate chaining**: It has the disadvantage of requiring pointers. This tends to slow the algorithm down a bit because of the time required to allocate new cells,and also essentially requires the implementation of a second data structure.
- **Open addressing**: If a collision occurs, alternative cells are tried until an empty cell is found. More formally, cells h0(X), h1(X), h2(X), ... are tried in succession, where hi(X) = (Hash(X) + F(i)) mod TableSize, with F(0) = 0. The function, F, is the collision resolution strategy. Because all the data go inside the table, a bigger table is needed for open addressing hashing than for separate chaining hashing. Generally, the load factor should be below λ ＝ 0.5 for open addressing hashing.

#### Separate Chaining

可以改变table的大小。

```c
/* Hash Table: Separate Chaining Hash Table */
#ifndef _HashSeq_H
#define MinTableSize 5

struct ListNode;
typedef struct ListNode *Position;
struct HashTbl;
typedef struct HashTbl *HashTable;

HashTable InitializeTable(int TableSize);
void DestroyTable(HashTable H);
Position Find(ElementType Key, HashTable H);
void Insert(ElementType Key, HashTable H);
ElementType Retrieve(Position P);
/* Routines such as Delete and MakeEmpty are omitted */

#endif

struct ListNode
{
   ElementType Element;
   Position Next;
};

typedef Position List;

struct HashTbl
{
   int TableSize;
   List *TheLists;
};

/* add head files <stdlib.h>, <math.h>, <time.h> */
/*  Fermat's little theorem:
   if n is prime, then for every 1 <= a < n:
   a^(n-1) % n = 1
*/
static int IsPrime(int n)
{
   int rand_num;

   srand(time(NULL));
   for (int i = 0; i < 100; i++)
   {
       rand_num = rand() % n;
       if (pow(rand_num, n-1) % n != 1)
           return 0;
   }
   return 1;
}

static int NextPrime(int n)
{
   if (n % 2 == 0)
       n++;
   while (!IsPrime(n))
       n += 2;

   return n;
}  

HashTable InitializeTable(int TableSize)
{
   HashTable H;

   if (TableSize < MinTableSize)
   {
       fprintf(stderr, "Table size too small!");
       return NULL;
   }

   H = (HashTable) malloc(sizeof(struct HashTbl));
   if (H == NULL)
   {
       fprintf(stderr, "Out of space!");
       return NULL;
   }

   H->TableSize = NextPrime(TableSize); // The TableSize should be prime!
   H->TheLists = (List *) malloc(sizeof(struct ListNode) * TableSize);
   if (H->TheLists == NULL)
   {
       fprintf(stderr, "Out of space!");
       return NULL;
   }

   for (int i = 0; i < H->TableSize; i++)
       H->TheLists[i]->Next = NULL;

   return H;
}

Position Find(ElementType Key, HashTable H)
{
   List L;
   L = H->TheLists[Hash(Key, H->TableSize)];

   Position P = L->Next;
   while (P != NULL && P->Element != Key)
       P = P->Next;

   return P;
}

/* Insert new node at the end of the list */
/* Also can insert at the front of the list */
void Insert(ElementType Key, HashTable H)
{
   Position P;

   P = Find(Key, H);
   if (P == NULL)
   {
       P = (Position) mallco(sizeof(struct ListNode));
       P->Element = Key;
       P->Next = NULL;
   }
}  
```

#### Open Addressing

hi(X) = (Hash(X) + F(i))。根据collision resolution method划分为F的三种实现。

##### Linear Probing

F is a linear function of i, typically F(i) = i. This amounts to trying cells sequentially in search of an empty cell.<br>
But the searching time can get quite large. Worse, even if the table is relatively empty, blocks of  occupied cells start forming known as primary clustering.

##### Quadratic Probing

Quadratic probing is a collision resolution method that eliminates the primary clustering problem of linear probing. The popular choice if F(i) = i^2.<br>
If quadratic probing is used, and the table size is prime, then a new element can always be inserted if the table is at least half empty.

##### Double Hashing

F(i) = i * hash2(X)<br>
We apply a second hash function to X and probe at a distance hash2(X), 2hash2(X), ..., and so on. The function must never evaluate to zero. It is also important to make sure all cells can be probed.<br>
A function such as hash2(X) = R - (X mod R), with R a prime smaller than TableSize, will work well.

```c
/* Hash Table: Open Addressing Hash Table */
#ifndef _HashQuad_H

typedef unsigned int Index;
typedef Index Position;

struct HashTbl;
typedef struct HashTbl *HashTable;

HashTable InitializeTable(int TableSize);
void DestroyTable(HashTable H);
Position Find(ElementType Key, HashTable H);
void Insert(ElementType Key, HashTable H);
ElementType Retrieve(Position P, HashTable H);
HashTable Rehash(HashTable H);
/* Routines such as Delete and MakeEmpty are omitted */

enum KindOfEntry {Legitimate, Empty, Deleted};

struct HashEntry
{
   ElementType Element;
   enum KindOfEntry Info;
};

typedef struct HashEntry Cell;

/* Cell *TheCells will be an array of HashEntry cells, allocated later */
struct HashTbl
{
   int TableSize;
   Cell *TheCells;
};

HashTable InitializeTable(int TableSize)
{
   HashTable H;
   int i;

   if (TableSize < MinTableSize)
   {
       fprintf(stderr, "Table size too small");
       return NULL;
   }

   H = (HashTable) malloc(sizeof(struct HashTbl));
   if (H == NULL)
   {
       fprintf(stderr, "Out of space!");
       return NULL;
   }

   H->TableSize = NextPrime(TableSize);
   H->TheCells = (Cell *) malloc(sizeof(struct HashEntry) * H->TableSize);
   if (H->TheCells == NULL)
   {
       fprintf(stderr, "Out of space");
       return NULL;
   }

   for (int i = 0; i < H->TableSize; i++)
       H->TheCells[i].Info = Empty;

   return H;
}

/* Quadratic Probing */
Position Find(ElementType Key, HashTable H)
{
   Position i;
   int count = 1;

   i = Hash(Key, H->TableSize);
   while (H->TheCells[i].Info != Empty && H->TheCells[i].Element != Key)
   {
       i = (i + count*count) % H->TableSize;
       count++;
   }

   return i;
}

void Insert(ElementType Key, HashTable H)
{  
   Position Pos;

   Pos = Find(Key, H);
   if (H->TheCells[Pos].Info != Legitimate)
   {
       H->TheCells[Pos].Info = Legitimate;
       H->TheCells[Pos].Element = Key;
   }
}  
```

### Rehashing
- Hash table空间不足时，insert可能会失败，所以需要新建一个两倍大的table，插入原有的table的元素。<br>
- Rehashing can be implemented in several ways with quadratic probing.
    1. Rehash as soon as the table is half full.
    2. Rehash only when an insertion fails.
    3. A middle-of-the-road strategy is to rehash when the table reaches a certain load factor. This strategy, implemented with a good cutoff, could be the best.

```c
/* Rehashing for open addressing hash tables */
HashTable Rehash(HashTable H)
{  
   int i, OldSize;
   Cell *OldCells;

   OldCells = H->TheCells;
   OldSize = H->TableSize;

   H = InitializeTable(2 * OldSize);

   for (i = 0; i < OldSize; i++)
       if (OldCells[i].Info == Legitimate)
           Insert(OldCells[i].Element, H);

   free(OldCells);

   return H;
}
```

### Extendible Hashing

- 目的：To deal with the case where the amount of data is too large to fit in main memory.

- Extendible hashing allows a Find to be performed in two disk accesses. Insertion also require few disk accesses.
- 当B-tree的深度为1时，根存储在内存里，Find就可以仅通过一次disk access。但是branching factor会很大从而会花很多时间寻找对应的树叶。如果使用extendible hashing就可以解决这个问题。
- 具体内容见书p168。
