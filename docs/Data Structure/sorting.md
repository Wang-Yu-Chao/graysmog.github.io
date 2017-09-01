# Sorting

## 结构

- Insertion Sort
- Shell Sort
    - Shell's increment
    - Hibbard's increment
    - Sedgwick's increment
- Heap Sort
- Merge Sort
- Quick Sort
- Bucket Sort
- External Sort

## Insertion Sort

代码：
```c
void InsertionSort(ElementType A[], int N)
{
	int j, P;

	ElementType Tmp;
	for (P = 1; P < N; P++)
	{
		Tmp = A[P];
		for (j = P; j > 0 && A[j-1] > Tmp; j--)
			A[j] = A[j-1];
		A[j] = Tmp;
	}
}
```

## Shellsort

### Shell's increment

Shellsort routine using Shell's increment. The worst-case running time of Shellsort, using Shell's increment, is O(N^2).

```c
void Shellsort(ElementType A[], int N)
{
    int i, j, Increment;
    ElementType Tmp;

    for (Increment = N/2; Increment > 0; Increment /= 2)
        for (i = Increment; i < N; i++)
        {
            Tmp = A[i];
            for (j = i; j >= Increment; j -= Increment)
                if (Tmp < A[j - Increment])
                    A[j] = A[j - Increment];
                else
                    break;
            A[j] = Tmp;
        }
}
```

The problem with Shell's increments is that pairs of increments are not necessarily relatively prime, and thus the smaller increment can have little effect.

### Hibbard's increment

Hibbard's increment gives better results in practice. His increment are of the form 1, 3, 7, ..., <a href="http://www.codecogs.com/eqnedit.php?latex=2^{k-1}" target="_blank"><img src="http://latex.codecogs.com/gif.latex?2^{k-1}" title="2^{k-1}" /></a>.  The key difference is that consecutive increments have no common factors. The worst-case running time of Shellsort using Hibbard's increment is O(<a href="http://www.codecogs.com/eqnedit.php?latex=N^{\frac{3}{2}}" target="_blank"><img src="http://latex.codecogs.com/gif.latex?N^{\frac{3}{2}}" title="N^{\frac{3}{2}}" /></a>).

### Sedgwick's increment

Sedgewick has proposed several increment sequences that give an O(<a href="http://www.codecogs.com/eqnedit.php?latex=N^{\frac{4}{3}}" target="_blank"><img src="http://latex.codecogs.com/gif.latex?N^{\frac{4}{3}}" title="N^{\frac{4}{3}}" /></a>) worst-case running time. The average running time is conjectured to be O(<a href="http://www.codecogs.com/eqnedit.php?latex=N^{\frac{7}{6}}" target="_blank"><img src="http://latex.codecogs.com/gif.latex?N^{\frac{7}{6}}" title="N^{\frac{7}{6}}" /></a>). The best of these is the sequence {1, 5, 19, 41, 109, ...}, in which the terms are either of the form <a href="http://www.codecogs.com/eqnedit.php?latex=9&space;\times&space;4^{i}&space;-&space;9&space;\times&space;2^{i}&space;&plus;&space;1" target="_blank"><img src="http://latex.codecogs.com/gif.latex?9&space;\times&space;4^{i}&space;-&space;9&space;\times&space;2^{i}&space;&plus;&space;1" title="9 \times 4^{i} - 9 \times 2^{i} + 1" /></a> or <a href="http://www.codecogs.com/eqnedit.php?latex=4^{i}&space;-&space;3&space;\times&space;2^{i}&space;&plus;&space;1" target="_blank"><img src="http://latex.codecogs.com/gif.latex?4^{i}&space;-&space;3&space;\times&space;2^{i}&space;&plus;&space;1" title="4^{i} - 3 \times 2^{i} + 1" /></a>.

## Heapsort

- The basic strategy is to build a binary heap of N elements. The stage takes O(N) time. We then perform N DeleteMin operations. By recording these elements in a second array and then copying the array back, we sort N elements. Since each DeleteMin takes O(logN) time, the total running time is O(NlogN).
- A clever way to avoid using a second array makes use of the fact that after each DeleteMin, the heap shrinks by 1. Thus the cell that was last in the heap can be used to store the element that was just deleted.

```c
#define LeftChild(i) (2 * (i) + 1)

void PercDown(ElementType A[], int i, int N)
{
    int Child;
    ElementType Tmp;

    for (Tmp = A[i]; LeftChild(i) < N; i = Child)
    {   
        Child = LeftChild(i);
        if (Child != N - 1 && A[Child + 1] > A[Child])
            Child++;
        if (Tmp < A[Child])
            A[i] = A[Child];
        else
            break;
    }   
    A[i] = Tmp;
}

void Heapsort(ElementType A[], int N)
{
    int i;

    for (i = N/2; i >= 0; i--)
        PercDown(A, i, N);
    for (i = N-1; i > 0; i--)
    {   
        Swap(&A[0], &A[i]);
        PercDown(A, 0, i);
    }   
}  
```

## Mergesort
- Mergesort runs in O(NlogN) worst-case running time. The fundamental operation in this algorithm is merging two sorted lists. Because the lists are sorted, this can be done in one pass through the input, if the output is put in a thirt list. The basic merging algorithm takes two input arrays A and B, an output array C, and three counters, Aptr, Bptr, Cptr, which are initially set to the beginning of their respective arrays. The smaller of A[Aptr] and B[Bptr] is copied to the next entry in C, and the appropriate counters are advanced. When either input list is exhausted, the reminder of the other list is copied to C.
- The mergesort algorithm is a classic divide-and-conquer strategy. if N = 1, there is only one element to sort, and the answer is at hand. Otherwise, recursively mergesort the first half and the second half. This gives two sorted halves, which can then be merged together using the merging algorithm described above.

```c
void Msort(ElementType A[], ElementType TmpArray[], int Left, int Right)
{
    int Center;

    if (Left < Right)
    {   
        Center = (Left + Right) / 2;
        Msort(A, TmpArray, Left, Center);
        Msort(A, TmpArray, Center + 1, Right);  
        Merge(A, TmpArray, Left, Center + 1, Right);
    }   
}

void Mergesort(ElementType A[], int N)
{
    ElementType *TmpArray;  

    TmpArray = malloc(N * sizeof(ElementType));
    if (TmpArray != NULL)
    {   
        MSort(A, TmpArray, 0, N - 1);   
        free(TmpArray);
    }   
    else
        fprintf(stderr, "No space for temp array!");
}

/* Lpos = start of left half, Rpos = start of right half */
void Merge(ElementType A[], ElementType TmpArray[], int Lpos, int Rpos, int RightEnd)
{
    int i, LeftEnd, NumElements, TmpPos;

    LeftEnd = Rpos - 1;
    TmpPos = Lpos;
    NumElements = RightEnd - Lpos + 1;

    /* main loop */
    while (Lpos <= LeftEnd && Rpos <= RightEnd)
        if (A[Lpos] <= A[Rpos])
            TmpArray[TmpPos++] = A[Lpos++];
        else
            TmpArray[TmpPos++] = A[Rpos++];

    while (Lpos <= LeftEnd)
        TmpArray[TmpPos++] = A[Lpos++];
    while (Rpos <= RightEnd)
        TmpArray[TmpPos++] = A[Rpos++];

    /* Copy TmpArray back */
    for (i = 0; i < NumElements; i++, RightEnd--)
        A[RightEnd] = TmpArray[RightEnd];
}
```

## Quicksort

- Quicksort is the fastest known sorting algorithm in practice. Its average running time is O(NlogN).
- Steps:
    1. If the number of elements in S is 0 or 1, then return.
    2. Pick any element v in S. This is called the pivot.
    3. Partition S - {v} (the remaining elements in S) into two disjoint groups S1 and S2.
    4. Return {quicksort(S1) followed by v followed by quicksort(S2)}.

```c
/* Return median of Left, Center, and Right */
/* Order these and hide the pivot */
ElementType Median3(ElementType A[], int Left, int Right)
{
    int Center = (Left + Right) / 2;

    if (A[Left] > A[Center])
        Swap(&A[Left], &A[Center]);
    if (A[Left] > A[Right])
        Swap(&A[left], &A[Right]);  
    if (A[Center] > A[Right])
        Swap(&A[Center], &A[Right]);
    /* Invariant: A[Left] <= A[Center] <= A[Right] */

    Swap(&A[Center], &A[Right - 1]); /* Hide pivot */
    return A[Right - 1];             /* Return pivot */
}   

/* Main quicksort routine */
#define Cutoff (3)

void Qsort(ElementType A[], int Left, int Right)
{
    int i, j;
    ElementType Pivot;

    if (Left + Cutoff <= Right)
    {   
        Pivot = Median3(A, Left, Right);
        i = Left; j = Right - 1;
        for (; ;)
        {   
            while (A[++i] < Pivot) { }
            while (A[--j] > Pivot) { }
            if (i < j)
                Swap(&A[i], &A[j]);
            else
                break;
        }   
        Swap(&A[i], &A[Right - 1]); /* Restore pivot */

        Qsort(A, Left, i - 1);  
        Qsort(A, i + 1, Right);
    }   
    else /* Do an insertion sort on the subarray */
        InsertionSort(A + Left, Right - Left + 1);
}
```

## Bucket Sort
- Any general sorting algorithm that uses only comparisons requires Ω(NlogN) time in the worst case, but it is still possible to sort in linear time in some special cases.
- For bucket sort to work, extra information must be available. The input A1, A2, ..., An must consist of only positive integers smaller than M. Keep an array called Count, of size M, which is initialized to all 0's. Thus, Count has M cells, or buckets, which are initially empty. When Ai is read, increment Count[Ai] by 1. After all the input is read, scan the Count array, printing out a representation of the sorted list. This algorithm takes O(M + N). if M is O(N), then the total is O(N).

## External Sorting
The basic external sorting algorithm uses the Merge routine from mergesort.
