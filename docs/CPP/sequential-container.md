# 顺序容器

## 概述

- 一个容器就是一些特定类型对象的集合。顺序容器（sequential container）为程序员提供了控制元素存储和访问顺序的能力。这种顺序不依赖于元素的值，而是与**元素加入容器时的位置**相对应。与之相对的，有序和无序关联容器，则根据关键字的值来存储元素。
- 标准库还提供了三种容器适配器，分别为容器操作定义了不同的接口，来与容器类型适配。
- 所有顺序容器都提供了快速顺序访问元素的能力，但是，这些容器在以下方面都有不同的性能折中：
    - 向容器添加或从容器中删除元素的代价。
    - 非顺序访问容器中元素的代价。
- 顺序容器类型：
    - vector：可变大小数组。支持快速随机访问。在尾部之外的位置插入或删除元素可能很慢。
    - deque：双端队列。支持快速随机访问。在头尾位置插入／删除元素速度很快。
    - list：双向链表。只支持双向顺序访问。在list中任何位置进行插入／删除操作速度都很快。
    - forward_list：单向链表。只支持单向顺序访问。在链表任何位置进行插入／删除操作速度都很快。
    - array：固定大小数组。支持快速随机访问。不能添加或删除元素。（相比内置数组更安全、更容易使用）
    - string：与vector相似的容器，专门用于保存字符。
- 新标准库的容器比旧版本的快得多，现代C++程序应该使用标准库容器，而不是更原始的数据结构，如内置数组。

##### 确定使用哪种顺序容器

- 通常，使用vector是最好的选择。
- 如果程序只有在读取输入时才需要在容器中间位置插入元素，随后需要随机访问元素，则
    - 首先，确定是否真的需要在容器中间位置添加元素。通常可以先向vector追加数据，然后再调用标准库的sort函数。
    - 如果必须在中间位置插入元素，考虑在输入阶段使用list，一旦输入完成，将list中的内容拷贝到一个vector中。

## 容器库概览

容器类型上的操作形成了一种层次：
- 某些操作是所有容器类型都提供的。
- 另外一些操作仅针对顺序容器、关联容器或无序容器。
- 还有一些操作只适用于一小部分容器。

一般来说，每个容器都定义在一个头文件中，文件名和类型名相同。如，deque定义在头文件deque中。容器均定义为模版类。我们必须提供额外信息来生成特定的容器类型。

##### 对容器可以保存的元素类型的限制

- 顺序容器几乎可以保存任意类型的元素。特别是，我们可以定义一个容器，其元素的类型是另一个容器。如`vector<vector<string>> lines;`。（较旧的编译器可能需要在两个尖括号之间键入空格，例如，`vector<vector<string> >`）
- 某些容器操作对元素类型有其自己的特殊要求。我们可以为不支持特定操作需求的类型定义容器，但这种情况就只能使用那些没有特殊要求的容器操作了。例如，顺序容器构造函数的一个版本接受容器大小参数，它使用了元素类型的默认构造函数。但某些类没有默认的构造函数。
```CPP
vector<noDefault> v1(10, init);    // 正确：提供了元素初始化器
vector<noDefault> v2(10);          // 错误：必须提供一个元素初始化器
```
- 容器操作分类：类型别名、构造函数、赋值与swap、大小、添加／删除元素（不适用于array）、获取迭代器、反向容器的额外成员（不支持forward_list）。
- 容器操作列表：见书P295。

### 迭代器

- 与容器一样，迭代器有着公共的接口：如果一个迭代器提供某个操作，那么所有提供相同操作的迭代器对这个操作的实现方式都是相同的。（如解引用、递增运算符）
- 有一个例外不符合公共接口的特点：forward_list迭代器不支持递减运算符（--）。
- 一个**迭代器范围**（iterator range）由一对迭代器表示，两个迭代器（begin和end）分别指向同一个容器中的元素或者是尾元素之后的位置。
- 迭代器范围中的元素包含first所表示的元素以及从begin开始直至end（但不包含end）之间的所有元素，这种元素范围被称为左闭合区间（left-inclusive interval），即[begin, end)。

##### 使用左闭合范围蕴含的编程假定

- 如果begin与end相等，则范围为空。
- 如果begin与end不等，则范围至少包含一个元素，且begin指向该范围中的第一个元素。
- 我们可以对begin递增若干次，使得begin == end。

### 容器类型成员

- 每个容器都定义了多个类型，如size_type、iterator和const_iterator。
- 大多数容器还提供反向迭代器，即一种反向遍历容器的迭代器，与正向迭代器相比，各种操作的含义也都发生了颠倒。例如，对一个反向迭代器执行++操作，会得到上一个元素。

### begin和end成员

- begin和end操作生成指向容器中第一个元素和尾元素之后位置的迭代器。
- begin和end有多个版本：带r的版本返回反向迭代器；以c开头的版本则返回const迭代器；以cr开头的版本返回const反向迭代器。
- 以c开头的版本是C++新标准引入的，用以支持auto与begin和end函数结合使用。过去只能显式声明希望使用哪种类型的迭代器。
```CPP
auto it1 = a.begin();       // 仅当a是const时，it1是const_iterator
auto it2 = a.cbegin();      // it2是const_iterator
```
- 当不需要写访问时，应使用cbegin和cend。

### 容器定义和初始化

容器的构造函数 | 解释
:----- | :-----:
`C c` | 默认构造函数。如果C是一个array，则c中元素按默认方式初始化；否则c为空。
`C c1(c2)`<br>`C c1=c2` | c1初始化为c2的拷贝。c1和c2必须是相同类型（相同的容器类型和元素类型，对于array类型，还必须是相同大小）
`C c{a,b,c...}`<br>`C c={a,b,c...}` | c1初始化为初始化列表中元素的拷贝。元素类型必须相容。对于array来说，列表中元素必须等于或小于array的大小。
`C c(b,e)` | c初始化为迭代器b和e指定范围中的元素的拷贝。范围中元素的类型必须与c的元素类型相容（array不适用）。
`C seq(n)` | seq包含n个元素，这些元素进行了值初始化；此构造函数是explicit的。（string不适用）
`C seq(n,t)` | seq包含n个初始化为值t的元素。

##### 将一个容器初始化为另一个容器的拷贝

- 讲一个新容器创建为另一个容器的拷贝的方法有两种：
    - 可以直接拷贝整个容器。
    - 或者（array除外）拷贝由一个迭代器对指定的元素范围。
- 容器拷贝：为创建一个容器对另一个容器的拷贝，两个**容器的类型**及其**元素类型**必须匹配。
- 迭代器范围拷贝：不过，当传递迭代器参数来拷贝一个范围时，就**不要求**容器类型是相同的了。而且两个容器中的元素类型也可以不同，只要能将要拷贝的元素转换。
```CPP
vector<const char*> articles = {"a", "an", "the"};
vector<string> words1(articles);                                // 错误：容器的拷贝，容器类型和元素类型必须匹配
forward_list<string> words2(articles.begin(), articles.end());  // 正确：迭代器范围拷贝，可以将const char*元素转换为string
```
- 注意：迭代器范围拷贝中，两个迭代器分别标记想要拷贝的第一个元素和**最后一个元素之后**的位置。（左闭合区间）

##### 列表初始化

- 在新标准中，可以对一个容器进行列表初始化。
```CPP
vector<const char*> articles = {"a", "an", "the"};
```
- 这样做显式地指定了容器中每个元素的值。对于除array之外的容器类型，还隐含地指定了容器的大小。

##### 与顺序容器大小相关的构造函数

- 除了与关联容器相同的构造函数外，顺序容器（array除外）还提供另一个构造函数，它接受一个容器大小和一个（可选的）元素初始值。如果我们不提供元素初始值，则标准库会创建一个值初始化器。如果元素类型没有默认构造函数，除了大小参数外，还必须指定一个显式的元素初始值。
- 只有顺序容器的构造函数才接受大小参数，关联容器并不支持。
```CPP
vector<int> ivec(10, -1);   // 提供元素初始值。10个元素都初始化为-1
forward_list<int> ivec(10); // 不提供元素初始值。10个元素，每个都默认初始化为0
```

##### 标准库array具有固定大小

- 和内置数组一样，标准库array的大小也是类型的一部分。当定义一个array时，除了指定元素类型，还要指定容器大小。
- 使用array类型也要同时指定元素类型和大小。
```CPP
array<string, 10>
array<int, 10>::size_type i;    // 数组类型包括元素类型和大小
```
- array不支持普通的容器构造函数，这些构造函数都或显式地，或隐式地确定容器的大小。
- 与其他容器不同，一个默认构造的array是非空的，它包含了和其大小一样多的被默认初始化的元素。
- 如果对array进行列表初始化，初始值的数目必须等于或小于array的大小。若小于，则剩余元素会进行值初始化。
```CPP
array<int, 10> ia1; // 10个默认初始化的int
array<int, 10> ia3; // ia[0]为422，剩余元素为0
```
- 虽然我们不能对内置数组类型进行拷贝或对象赋值操作，但array并无此限制。
```CPP
int digs[5] = {0, 1, 2, 3, 4};
int cpy[5] = digs;              // 错误：内置数组不支持拷贝或赋值
array<int, 5> digits = {0, 1, 2, 3, 4};
array<int, 5> copy = digits;    // 正确：只要类型匹配（大小和元素类型），array可以拷贝或赋值
```

### 赋值和swap

容器赋值运算语句 | 解释
:---- | :-----:
`c1=c2` | 将c1中的元素替换为c2中元素的拷贝
`c={a,b,c...}` | 将c1中元素替换为初始化列表中元素的拷贝（array不适用）
`swap(c1,c2)` | 交换c1和c2中的元素。c1和c2必须有相同的类型。swap通常比从c2向c1拷贝元素快得多
`seq.assign(b,e)` ｜ 将seq中的元素替换为迭代器b和e所表示的范围中的元素。迭代器b和e不能指向seq中的元素
`seq.assign(il)` | 将seq中的元素替换为初始化列表il中的元素
`seq.assign(n,t)` | 将seq中的元素替换为n个值为t的元素

- assign操作不适用于关联容器和array。
- 赋值相关运算会导致指向左边容器内部的迭代器、引用和指针失效。而swap操作将容器内容交换不会导致指向容器的迭代器、引用和指针失效（容器类型为array和string的情况除外）。

##### 使用assign（仅顺序容器）

- 赋值运算符（=）要求左边和右边的运算对象具有相同的类型。顺序容器（array除外）还定义了一个名为assign的成员，允许我们从一个不同但相容的类型赋值，或者从容器的一个子序列赋值。
```CPP
list<string> names;
vector<const char*> oldstyle;
names = oldstyle;                                   // 错误：容器类型不匹配
names.assign(oldstyle.cbegin(), oldstyle.cend());   // 正确：可以将const char*转换为string
```
- assign还有两种其他版本，分别接受一个整形值加一个元素值，和一个初始化列表。

##### 使用swap

- swap操作交换两个相同类型容器的内容。调用swap之后，两个容器中的元素将会交换。（容器大小性质同时也会交换）
- 除array外，交换两个容器内容的操作保证会很快——元素本身并未交换，swap只是交换了两个容器的数据结构（不对元素进行拷贝、删除或插入操作）。
- 元素不会被移动的事实意味着，除string外，指向容器的迭代器、引用和指针在swap操作之后都不会失效。它们仍指向swap操作之前所指向的那些元素。但是，在swap之后，这些元素已经属于不同的容器了（如，iter在swap之前指向svec[3]的元素，那么swap之后它指向svec2[3]的元素）。与其他容器不同，对一个string调用swap会导致迭代器、引用和指针失效。
- 特别地是，swap两个array会真正交换它们的元素。因此，交换两个array所需的时间与array中元素的数目成正比。因此，对于swap后的array，指针、引用和迭代器所绑定的元素保持不变，但元素值已经与另一个array中对应元素的值进行了交换。
- 在新标准库中，容器既提供成员函数版本的swap，也提供非成员函数版本的swap。早期标准库只提供成员函数版本的。非成员版本的swap在泛型编程中是非常重要的。统一使用非成员版本的swap是一个好习惯。

### 容器大小操作

- 每个容器都有三个与大小相关的操作。成员函数size返回容器中元素的数目；empty在size为0时返回true，否则返回false；max_size返回一个大于或等于该类型容器所能容纳的最大元素数的值。（forward_list不支持size）

### 关系运算符

- 每个容器类型都支持相等运算符（==和=）；除了无序关联容器外的所有容器都支持关系运算符（>、>=、<、<=）。关系运算符左右两边的运算对象必须是相同的类型容器，并保存相同类型的元素。
- 比较两个容器实际上是进行元素的逐对比较。这些运算符的工作方式与string的关系运算类似：
    - 如果两个容器具有相同大小且所有元素都两两对应相等，则这两个容器相等；否则两个容器不等。
    - 如果两个容器大小不同，但较小容器中每个元素都等于较大容器中的对应元素，则较小容器小于较大容器。
    - 如果两个容器都不是另一个容器的前缀子序列，则它们的比较结果取决于第一个不相等的元素的比较结果。

##### 容器的关系运算符使用元素的关系运算符完成比较

- 容器的相等运算符实际上是使用元素的==运算符实现比较的，而其他关系运算符是使用元素的<运算符。
- 如果元素类型不支持所需运算符，那么保存这种元素的容器就不能使用相应的关系运算符。

## 顺序容器操作

上一节介绍的是所拥有容器都支持的操作，本节将介绍顺序容器所特有的操作。

### 向顺序容器添加元素

向顺序容器添加元素的语句 | 解释
:--- | :---:
`c.push_back(t)`<br>`c.emplace_back(args)` | 在c的尾部创建一个值为t或由args创建的元素。返回void
`c.push_front(t)`<br>`c.emplace_front(args)` | 在c的头部创建一个值为t或由args创建的元素。返回void
`c.insert(p,t)`<br>`c.emplace(p,args)` | 在迭代器p指向的元素之前创建一个值为t或由args创建的元素。返回指向新添加的元素的迭代器
`c.insert(p,n,t)` | 在迭代器p指向的元素之前插入n个值为t的元素。返回指向新添加的第一个元素的迭代器；若n为0，则返回p
`c.insert(p,b,e)` | 将迭代器b和e指定的范围内的元素插入到迭代器p指向的元素之前。b和e不能指向c中的元素。返回指向新添加的第一个元素的迭代器；若范围为空，返回p
`c.insert(p,il)` | il是一个花括号包围的元素值列表。将这些给定值插入到迭代器p指向的元素之前。返回值同上。

- 这些操作会改变容器的大小；array不支持这些操作。
- forward_list有自己专有版本的insert和emplace，不支持push_back和emplace_back。
- vector和string不支持push_front和emplace_front。
- 向一个vector、string或deque插入元素会使所有指向容器的迭代器、引用和指针失效。
- 使用这些操作时，必须记得不同的容器使用不同的策略来分配元素空间，而这些策略直接影响性能。
- 注意：当我们用一个对象来初始化容器时，或将一个对象插入到容器中时，实际上放入到容器中的是对象值的一个拷贝，而不是对象本身。

##### 使用push_back

- 除array和forward_list之外，每个顺序容器（包括string)都支持push_back。由于sting是一个字符容器，我们也可以用push_back在string末尾添加字符。


##### 使用push_front

- list、forward_list和deque容器支持push_front的类似操作。此操作将元素插入到容器头部。

##### 在容器中的特定位置添加元素

- insert成员提供了更一般的添加功能，它允许我们在容器中任意位置插入0个或多个元素。vector、deque、list和string都支持insert成员。forward_list提供了特殊版本的insert成员。
- 每个insert函数都接受一个迭代器作为其第一个参数。由于迭代器可能指向容器尾部之后不存在的元素的位置，而且在容器开始位置插入元素是很有用的功能，所以insert函数将元素插入到迭代器所指定的位置**之前**。
- 虽然某些容器不支持push_front操作（vector、string），但它们对于insert操作并无类似的限制（可以insert插入开始的位置），但可能很耗时。

##### 插入范围内元素

```CPP
svec.insert(svec.end(), 10, "Anna");
slist.insert(slist.end(), {"these", "words", "will", "go", "at", "the", "end"});
```

##### 使用insert的返回值

- 通过使用insert的返回值，可以在容器中一个特定位置反复插入元素。

```CPP
list<string> lst;
auto iter = lst.begin();
while (cin >> word)
    iter = lst.insert(iter, word);
```

##### 使用emplace操作

- 新标准引入了三个新成员——emplace_front、emplace、emplace_back，这些操作构造而不是拷贝元素。这些操作分别对应push_front、insert、push_back。
- 当调用push或insert成员函数时，我们将元素类型的对象传递给它们。而当调用一个emplace成员函数时，则是将参数传递给元素类型的构造函数。emplace成员使用这些参数在容器管理的内存空间中直接构造元素。
```CPP
// 在c的末尾构造一个Sales_data对象
c.emplace_back("345－93247342", 25, 15.99);
// 相同效果
c.push_back(Sales_data("345－93247342", 25, 15.99));
```
- 在调用emplace_back时，会在容器管理的内存空间中直接创建对象。而调用push_back则会创建一个局部临时对象，并将其压入容器中。
- emplace函数的参数根据元素类型而变化，参数必须于元素类型的构造函数相匹配。

### 访问元素

在顺序容器中访问元素的操作 | 解释
:----- | :-----:
`c.back()` |  返回c中尾元素的引用。若c为空，函数行为未定义
`c.front()` | 返回c中首元素的引用。若c为空，函数行为未定义
`c[n]` | 返回c中下标为n的元素的引用。n是一个无符号整数，若，n>=c.size()，函数行为未定义
`c.at(n)` | 返回下标为n的元素的引用。如果下标越界，则抛出一out_of_range异常。

- at和下标操作只适用于string、vector、deque和array。（不适用于list、forward_list）
- back不适用于forward_list。
- 包括array在哪的每个顺序容器都有一个front成员函数，而除forward_list之外的所有顺序容器都有一个back成员函数。这两个操作分别返回**首元素**和**尾元素**的**引用**。
```CPP
if (!c.empty())
{
    auto val = *c.begin(), val2 = c.front();   // 两种获取对首元素的引用的方式
    auto last = c.end();
    auto val3 = *(--last);
    auto val4 = c.back();                       // 两种获取对尾元素的引用的方式（不适用于forward_list）
}
```
- 这个程序有两点注意事项：迭代器end指向的是尾后元素，为获取尾元素，必须递减该迭代器。另一个是，在调用front和back之前，要确保c非空。如果容器为空，if中操作的行为将是未定义的。

##### 下标操作和安全的随机访问

- 使用越界的下标是一种严重的程序设计错误，而且编译器并不检查这种错误。
- 如果希望确保下表是合法的，可以使用at成员函数。at成员函数类似下标运算符，但如果下标越界，at会抛出一个out_of_range异常。

### 删除元素

顺序容器的删除操作 | 解释
:--- | :----:
`c.pop_back()` | 删除c中尾元素。若c为空，则函数行为未定义。函数返回void
`c.pop_front()` | 删除c中首元素。若c为空，则函数行为未定义。函数返回void
`c.erase(p)`<br>`c.erase(b,e)` | 删除迭代器p所指定的元素／迭代器b和e所指定范围内的元素，返回一个指向**被删元素之后**元素的迭代器，若p指向尾元素，则返回尾后迭代器。若p是尾后迭代器，则函数行为未定义
`c.clear()` | 删除c中的所有元素。返回void

- 这些操作会改变容器的大小，所以不适用于array。
- forward_list有特殊版本的erase。
- forward_list不支持pop_back；vector和string不支持pop_front。
- 删除deque中除首尾位置之外的任何元素都会使所有迭代器、引用和指针失效。指向vector或string中删除点之后位置的迭代器、引用和指针都会失效。

##### pop_front和pop_back成员函数

- 这些操作返回void。如果你需要弹出的元素的值，就必须在执行弹出操作之前保存它。
```CPP
while (!ilist.empty())
{
    process(ilist.front());     // 对ilist的首元素进行一些处理
    ilist.pop_front();          // 完成处理后删除首元素
}
```

##### 从容器内部删除一个元素

- 成员函数erase从容器中指定位置删除元素。我们可以删除由一个迭代器指定的单个元素，也可以删除由一对迭代器指定的范围内的所有元素。两种形式的erase都返回指向被删除的（最后一个）元素**之后**位置的迭代器。
- 下面的程序循环删除一个list中的所有奇数元素。
```CPP
list<int> lst = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
auto it = lst.begin()
while (it != 1st.end())
    if (*it % 2)
        it = lst.erase(it);     // 指向之后的位置，不需要递增it
    else
        ++it;
```

##### 删除多个元素

```CPP
elem1 = slist.erase(elem1, elem2);      // 调用后， elem1 === elem2
```
- 迭代器elem1指向要删除的第一个元素，elem2指向要删除的最后一个元素之后的位置。
```CPP
slist.clear();                              // 删除容器中所有元素
slist.erase(slist.begin(), slist.end());    // 等价调用
```

### 特殊的forward_list操作

- forward_list并未定义insert、emplace和erase，而是定义了名为insert_after、emplace_after和erase_after的操作。为了删除elem2，应该用指向其之前的elem1的迭代器调用erase_after。因为在一个单向链表中，没有简单的方法来获取一个元素的前驱，所以forward_list中添加或删除元素的操作是通过改变给定元素之后的元素来完成的。
- 为了支持这些操作，forward_list也定义了before_begin，它返回一个首前（off-the-beginning）迭代器。这个迭代器允许我们在链表首元素之前并不存在的元素“之后”添加或删除元素。

在forward_list中插入或删除元素的操作 | 解释
:---- | :-----:
`lst.before_begin()`<br>`lst.cbefore_begin()` | 返回首前迭代器。此迭代器不能解引用。cbefore_begin()返回一个const_iterator
`lst.insert_after(p,t)`<br>`lst.insert_after(p,n,t)`<br>`lst.insert_after(p,b,e)`<br>`lst.insert_after(p,il)` | 在迭代器p之后的位置插入元素，t是一个对象，n是数量。b和e是表示范围的一对迭代器（b和e不能指向lst内）。il是一个花括号列表。返回一个指向最后一个插入元素的迭代器。如果范围为空，则返回p。若p为尾后迭代器，则函数行为未定义
`lst.emplace_after(p,args)` | 使用args在p指定的位置之后创建一个元素。返回一个指向这个新元素的迭代器。若p为尾后迭代器，则函数行为未定义
`lst.erase_after(p)`<br>`lst.erase_after(b,e)` | 删除p指向的位置之后的元素，或删除从b之后直到（但不包含）e之间的元素。返回一个指向被删元素之后元素的迭代器（或尾后迭代器）如果p指向lst的尾元素或者是尾后迭代器，则函数行为未定义。

### 改变容器大小

- 可以用resize来增大或缩小容器。array不支持resize。如果当前大小大于所要求的大小，容器后部的元素会被删除；如果当前大小小于新大小，会将新元素添加到容器后部
- 如果resize缩小容器，则指向被删除元素的迭代器、引用和指针都会失效；对vector、string或deque进行resize可能导致迭代器、指针和引用失效。

```CPP
ilist<int> ilist(10, 42);   // 10个int，每个的值都是42
ilist.resize(15);           // 将5个值为0的元素添加到ilist的末尾（默认初始化）
ilist.resize(25, -1);       // 将10个值为-1的元素添加到ilist的末尾
ilist.resize(5);            // 从ilist末尾删除20个元素
```

顺序容器大小操作 | 解释
:---- | :----:
`c.resize(n)` | 调整c的大小为n个元素
`c.resize(n,t)` | 调整c的大小为n个元素，任何新添加的元素都初始化为值t

### 容器操作可能使迭代器失效

- 添加和删除操作可能会使指向容器元素的指针、引用或迭代器失效。在向容器添加元素后：
    - 如果容器是vector或string，且存储空间被重新分配，则指向容器的迭代器、指针和引用都会失效。如果存储空间未重新分配，指向插入位置之前的元素的迭代器、指针和引用仍然有效，但指向插入位置之后元素的迭代器、指针和引用将会失效。
    - 对于deque，插入到除首尾位置之外的任何位置都会导致迭代器、指针和引用失效。如果在首尾位置添加元素，迭代器会失效，但指向存在的元素的引用和指针不会失效。
    - 对于list和forward_list，指向容器的迭代器、指针和引用仍然有效。
- 建议：管理迭代器：当使用迭代器（或引用和指针）时，最小化要求迭代器必须保持有效的程序片段是一个好方法。每次改变容器的操作之后都正确地重新定位迭代器，这个建议对vector、string和deque尤为重要。

##### 编写改变容器的循环程序

- 如果循环调用的是insert或erase，那么更新迭代器很容易。这些操作都返回迭代器，可以用来更新。
```CPP
// 删除偶数元素，复制每个奇数元素
vector<int> vi = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
auto iter = vi.begin();
while (iter != vi.end())
{
    if (*iter % 2)
    {
        iter = vi.insert(iter, *iter);
        iter += 2;                      // 跳过插入的元素（插到前面）和当前元素
    }
    else
        iter = vi.erase(iter);          // 不应向前移动迭代器，因为iter指向被删元素之后的元素
}
```

##### 不要保存end返回的迭代器

- 当添加／删除vector或string的元素后，或在deque中首元素之外任何位置添加／删除元素后，原来end返回的迭代器总是会失效。因此，添加／删除元素的循环程序必须反复调用end，而不能在循环之前保存end返回的迭代器，一只当作容器末尾使用。通常C++标准库的实现中end()操作都很快，部分就是因为这个原因。
```CPP
auto begin = v.begin(), end = v.end();  // 保存尾迭代器是一个坏主意
while (begin != end)    // 错误做法
{
    // ......
}
```

## vector对象是如何增长的

- 通常情况下，我们不必关心一个标准库类型是如何实现的，而只需关心它如何使用。然而，对于vector和string，其部分实现渗透到了接口中。
- 如果每添加一个新元素，vector就执行一次这样的内存分配和释放操作，性能会慢到不可接受。
- vector和string的实现通常会分配比新的空间需求更大的内存空间。容器预留这些空间作为备用，可用来保存更多的新元素。这样，就不需要每次添加新元素都重新分配容器的内存空间了。

##### 管理容量的成员函数

- vector和string类型提供了一些成员函数，允许我们与它的实现中内存分配部分互动。capacity操作告诉我们容器在不扩张内存空间的情况下可以容纳多少个元素。reserve操作允许我们通知容器它应该准备保存多少个元素。

容器大小管理操作 | 解释
:---- | :-----:
`c.shrink_to_fit()` | 将capacity()减少为与size()相同大小的请求
`c.capacity()` | 不重新分配内存空间的话，c可以保存多少元素
`c.reserve()` | 分配至少能容纳n个元素的内存空间

- shrink_to_fit只适用于vector、string和deque。（不适用于list、forward_list和array）
- capacity和reserve只适用于vector和string
- reserve并不改变容器中元素的数量，仅影响vector预先分配多大的内存空间。
- 调用reserve永远也不会减少容器**占用的**（仅影响**预留的**）内存空间。类似的，resize成员函数只改变容器中元素的数目，而不是容器的容量。我们同样不能使用resize来减少容器预留的内存空间。
- shrink_to_fit是一个请求，具体的实现可以选择忽略此请求，不保证退回内存空间。

##### capacity和size

- 理解capacity和size的区别非常重要。容器的size是指它已经保存的元素的数目；而capacity则是在不分配新的内存空间的前提下它最多可以保存多少元素。
- 实际上，只要没有操作需求超出vector的容量，vector就不能重新分配内存空间。内存分配的原则是：只有当迫不得已时才可以分配新的内存空间。
- 不同的分配策略都应遵守高效的原则。就是说，通过在一个初始为空的vector上调用n次push_back来创建一个n个元素的vector，所花费的时间不能超过n的常数倍。

## 额外的string操作

- string类型还提供了一些额外的操作。这些操作大部分：
    1. 提供string类和C风格字符数组之间的相互转换；
    2. 增加了允许我们用下标代替迭代器的版本。

##### 构造string的其它方法

已知的构造string的方法 | 解释
:--- | :----:
`string s1` | 默认初始化，s1是一个空串
`string s2(s1)`<br>`string s2 = s1` | s2是s1的副本
`string s3("value")`<br>`string s3 = "value"` | s3是字面值"value"的副本
`string s4(n, 'c')` | 把s4初始化为由连续n个字符c组成的字符串


构造string的其它方法 | 解释
:---- | :-----:
`string s(cp,n)` | s是cp指向的数组中前n个字符的拷贝。此数组至少应该包含n个字符
`string s(s2,pos2)` | s是string s2从下标pos2开始的字符的拷贝。若pos2>s2.size()，构造函数的行为未定义
`string s(s2,pos2,len2)` | s是string s2从下标pos2开始len2个字符的拷贝。不管len2的值是多少，构造函数至多拷贝s2.size()-pos2个字符（拷贝到string末尾）

- string构造函数接受string或const char*参数时，可接受（可选的）指定拷贝多少个字符的参数；接受string参数时，还可以给定一个下标来指出从哪里开始拷贝（const char*通过增加第一个参数，移动指针来实现）。
```CPP
// 接受const char*的构造函数
const char *cp = "Hello World!!!";  // 以空字符结束的数组
char noNull[] = {'H', 'i'};         // 不是以空字符结束的数组
string s1(cp ＋ 6, 5);              // 从cp[6]开始拷贝5个字符
string s2(noNull);                  // 未定义：noNull不是以空字符结束
string s3(noNull, 2);               // 靠背两个字符
// 接受string的构造函数
string s4(s1, 6);                   // 从s1[6]开始拷贝，直至s1末尾
string s5(s1, 6, 5);                // 从s1[6]开始拷贝5个字符
string s6(s1, 6, 20);               // 正确，只拷贝到s1末尾
string s7(s1, 16);                  // 抛出一个out_of_range异常
```

##### substr操作

- substr操作返回一个string，它是原始string的一部分或全部的拷贝。可以传递给substr一个可选的开始位置和计数值。若无计数值，则拷贝到末尾。
```CPP
string s("hello world");
string s2 = s.substr(6);        // 从s[6]开始拷贝到s末尾并返回
string s3 = s.substr(6, 11);    // 从s[6]开始拷贝11个字符并返回
```
- 如果开始位置加上计数位置大于string的大小，则substr会调整计数值，只拷贝到string的末尾。

### 改变string的其它方法

- string类型支持顺序容器的赋值运算符以及assign、insert和erase操作。除此之外，它还定义了额外的insert和erase版本，即接受下标的版本。下标指出了**开始删除**的位置，或是insert到**给定值之前**的位置。
```CPP
s.insert(s.size(), 5, '!');     // 在s末尾插入5个感叹号。在此处用下标s.size()，而不是迭代器
s.erase(s.size()-5, 5);         // 从s删除最后5个字符。
```
- 标准库string还提供了接受C风格字符数组的insert和assign版本。例如，我们可以将以空字符结尾的字符数组insert到或assign给一个string。
- 接下来在s上调用insert，意图是将字符插入到s[size()]处（不存在的）元素之前的位置。
```CPP
const char *cp = "Stately, plump Back";
s.assign(cp, 7);                // s == "Stately"
s.insert(s.size(), cp + 7);     // s == "Stately, plump Back"
```
- 也可以指定将来自其他string或子字符串的字符插入到当前string中或赋予当前string。
```CPP
string s1 = "some string", s2 = "some other string";
s.insert(0, s2, 0, s2.size());  // 在s[0]之前插入s2中s2[0]开始的s2.size()个字符
```

##### append和replace函数  

- string类定义了两个额外的成员函数：append和replace，这两个函数可以改变string的内容。
```CPP
string s("C++ Primer"), s2 = s;
s.insert(s.size(), " 4th Ed."); // 在末尾追加字符（串），s == "C++ Primer 4th Ed."
s2.append(" 4th Ed.");          // 等价方法
```
- replace操作是调用erase和insert的一种简写形式。插入的文本和删除的文本长度可以不同。
```CPP
s.erase(11, 3);             // s == "C++ Primer Ed."
s.insert(11, "5th");        // s == "C++ Primer 5th Ed."
s2.replace(11, 3, "5th");   // 等价方法，在s2[11]位置处把3个字符替换为"5th"
```

##### 总结：修改string的操作

修改string的操作 | 解释
:----- | :-----:
`s.insert(pos,args)` | 在pos之前插入args指定的字符。pos可以为一个下标或一个迭代器。接受下标的版本返回一个指向s的引用；接受迭代器的版本返回指向第一个插入字符的迭代器
`s.erase(pos,len)` | 删除从位置pos开始的len个字符。如果len被省略，则删除从pos开始直至s末尾的所有字符。返回一个指向s的引用
`s.assign(args)` | 将s中的字符替换为args指定的字符。返回一个指向s的引用
`s.append(args)` | 将args追加到s。返回一个指向s的引用
`s.replace(range,args)` | 删除s中范围range内的字符，替换为args指定的字符。range或者是一个下标和一个长度，或者是一对指向s的迭代器。返回一个指向s的引用

*args可以是下列形式之一：append和assign可以使用所有形式。  
str不能与s相同，迭代器b和e不能指向s。*

args的形式 | 解释
:---- | :----:
`str` | 字符串str
`str,pos,len` | str中从pos开始最多len个字
`cp,len` | 从cp指向的字符数组的前（最多）len个字符
`cp` | cp指向的以空字符结尾的字符数组
`n,c` | n个字符c
`b,e` | 迭代器b和e指定的范围内的字符
`初始化列表` | 花括号包围的，以逗号分割的字符列表

*replace和insert所允许的args形式依赖于range和pos是如何指定的。*  
见书P324。

##### 改变string的多种重载函数

- 上表列出的append、assign、insert和replace函数有多个重载版本。
- assign和append函数无须指定要替换string中的哪个部分：assign总是替换string中的所有内容，append总是将新字符追加到string末尾。
- replace函数提供位置加范围、一对迭代器两种方式来指定删除元素的范围。insert函数提供一个下标或一个迭代器两种方式来指定插入点。
- 指定要添加到string中的字符（insert、assign和replace），可以来自：另一个string、字符指针（指向字符数组）、字符列表、一个字符加一个计数值。

### string搜索操作

- string类提供了6个不同的搜索函数，每个函数都有4个重载版本。
- 每个搜索操作都返回一个string::size_type值，表示匹配发生位置的下标。如果搜索失败，则返回一个名为string::npos的static成员。标准库将npos定义为一个const string::size_type类型，并初始化为-1。由于npos是一个unsigned类型，此初始值意味着npos等于任何string最大的可能大小。

string搜索操作 | 解释
:---- | :-----:
`s.find(args)` | 查找s中args第一次出现的位置
`s.rfind(args)` | 查找s中args最后一次出现的位置
`s.find_first_of(args)` | 在s中查找args中任何一个字符第一次出现的位置
`s.find_last_of(args)` | 在s中查找args中任何一个字符最后一个出现的位置
`s.find_first_not_of(args)` | 在s中查找第一个不在args中的字符
`s.find_last_not_of(args)` | 在s中查找最后一个不在args中的字符

*args必须是以下形式之一*  
*若不显式指定pos参数，pos默认为0*

args形式 | 解释
:---- | :-----:
`c,pos` | 从s中位置pos开始查找字符c。
`s2,pos` | 从s中位置pos开始查找字符串s2。
`cp,pos` | 从s中位置pos开始查找指针cp指向的以空字符结尾的C风格字符串。
`cp,pos,n` | 从s中位置pos开始查找指针cp指向的数组的前n个字符。

```CPP
string name("AnnaBelle");
auto pos1 = name.find("Anna");  // pos1 == 0
```
- 搜索操作是大小写敏感的。

```CPP
string lowercase("AnnaBelle");
auto pos2 = lowercase.find("Anna");  // pos2 = npos
```
```CPP
string numbers("0123456789");
string name("r2d2"), dept("03714p3");
auto pos3 = name.find_first_of(numbers);     // pos3 == 1
auto pos4 = dept.find_first_not_of(numbers); // pos4 == 5
```

##### 指定从哪里开始搜索

```CPP
// 每步循环查找name中下一个数
string::size_type pos = 0;
while ((pos = name.find_first_of(numbers, pos)) != string::npos)
{
    cout << "found number at index: " << pos << " element is " << name[pos] << endl;
    ++pos;      // 移动到下一个字符
}
```
- 注意，如果我们忽略了递增pos，循环就永远不会终止，一直查找到同一个数字。

##### 逆向搜索

- find系列操作都是由左至右搜索。而rfind则是由右至左搜索。rfind成员函数搜索最后一个匹配。
- find_last_of搜索与给定的string中任何一个字符匹配的最后一个字符。
- find_last_not_of搜索最后一个不出现在给定string中的字符。

```CPP
string river("Mississippi");
auto last_pos = river.rfind("is");  // last_pos == 4
```

### compare函数

- 除了关系运算符外，标准库string类型还提供了一组compare函数，这些函数与C标准库的strcmp函数很相似，根据s是等于、大于还是小于参数指定的字符串，s.compare返回0、正数或负数。

s.compare的几种参数形式 | 解释
:---- | :-----:
`s2` | 比较s和s2
`pos1,n1,s1` | 提取s中从pos1开始的n1个字符和s2比较
`pos1,n1,s2,pos2,n2` | s中从pos1开始的n1个字符，和s2中从pos2开始的n2个字符比较
`cp` | 比较s和以空字符结尾的字符数组cp
`pos1,n1,cp` | s中从pos1开始的n1个字符，和cp比较
`pos1,n1,cp,n2` | s中从pos1开始的n1个字符，和cp指向的地址开始的n2个字符比较

### 数值转换

- 字符串中常常包含表示数值的字符，但一般情况下，一个数的字符表示不同于其数值。C++11引入了多个函数，可实现数值数据和标准库string之间的转换。

string和数值之间的转换 | 解释
:----- | :-----:
`to_string(val)` | 一组重载函数，返回数值val的string表示。val可以是任何算术类型
`stoi(s,p,b)`<br>`stol(s,p,b)`<br>`stoul(s,p,b)`<br>`stoll(s,p,b)`<br>`stoull(s,p,b)` | 返回s的起始子串（表示整数内容）的数值，返回类型分表为int、long、unsigned long、long long、unsigned long long。b表示转换所用的基数（将字符串参数看作几进制），默认值为10。p是size_t指针，用来保存s中第一个非数值字符的下标，p默认为0，即函数不保存下标
`stof(s,p)`<br>`stod(s,p)`<br>`stold(s,p)` | 返回s的起始子串（表示浮点数内容）的数值，返回值类型分别是float、double、long double。参数p的作用同上

- 如果string不能转换为一个数值，这些函数抛出一个invalid_argument异常。如果转换得到的数值无法用任何类型来表示，则抛出一个out_of_range异常。

```CPP
int i = 42;
string s = to_string(i);    // 整数-->字符串
double d = stod(s);         // 字符串-->浮点数

// 转换s中以数字开始的第一个子串
string s2 = "pi = 3.14";
double d2 = stod(s2.substr(s2.find_first_of("+-.0123456789")));     // d == 3.14
```

- string参数中的第一个非空白符必须是符号（+或-）或数字。它可以以0x或0X开头来表示十六进制数。对字符串到浮点值的转换，string参数也可以以小数点（.）开头，并可以包含e或E来表示指数部分。根据基数b不同，string参数可以包含字母字符。

## 容器适配器

- 除了顺序容器外，标准库还定义了三个顺序容器适配器：stack、queue和priority_queue。
- 适配器（adaptor）是标准库中的一个通用概念。容器、迭代器和函数都由适配器。本质上，一个是适配器是一种机制，能使某种事物的行为看起来像另外一种事物一样。一个容器适配器接受一种已有的容器类型，使其行为看起来像一种不同的类型。
- 例如，stack适配器接受一个顺序容器（除array或forward_list外），并使其操作看起来像一个stack一样。

所有容器适配器都支持的操作和类型 | 解释
:---- | :------:
`size_type` | 一种类型，足以保存当前类型的最大对象的大小
`value_type` | 元素类型
`container_type` | 实现适配器的底层容器类型
`A a;` | 创建一个名为a的空适配器（使用默认的容器实现）
`A a(c);` | 创建一个名为a的适配器，带有容器c的一个拷贝
`关系运算符` | 每个适配器都支持所有关系运算符：==、!=、<、<=、>、>=。这些运算符返回底层容器的比较结果
`a.empty()` | 若a包含任何元素，返回false，否则返回true
`a.size()` | 返回a中的元素数目
`swap(a,b)`<br>`a.swap(b)` | 交换a和b的内容，a和b必须有相同的类型，包括底层容器类型。

##### 定义一个适配器

- 每个适配器都定义两个构造函数：默认构造函数创建一个空对象；接受一个容器的构造函数拷贝该容器来初始化适配器。
```CPP
stack<int> std(deq);    // 从deq拷贝元素到stack
```

- 默认情况下，stack和queue是基于deque实现的，priority_queue是基于vector实现的。
- 我们可以在创建一个适配器时将一个命名的顺序容器作为第二个类型参数，来重载默认容器类型。
```CPP
// 在vector上实现的空栈
stack<string, vecotr<string>> str_stk;
// str_stk2在vector上实现，初始化时保存svec的拷贝
stack<string, vector<string>> str_stk2(svec);
```

- 适配器使用的容器是有限制的。因为所有适配器都要求容器具有添加、删除和访问尾元素的能力，所以不能用array和forward_list来构造适配器。

##### 栈适配器

- stack类型定义在stack头文件中。栈默认用deque实现，也可以在list或vector之上实现。

上表未列出的栈操作 | 解释
`s.pop()` | 删除栈顶元素，但不返回该元素值
`s.push(item)`<br>`s.emplace(args)` | 创建一个新元素压入栈顶，该元素通过拷贝或移动item而来，或者由args构造
`s.top()` | 返回栈顶元素，但不将其弹出栈

- 只可以使用适配器操作，而不能使用底层容器类型的操作。如`intStack.push(ix);`语句在底层deque对象上调用push_back，但不能在stack上调用push_back。

##### 队列适配器

- queue和priority_queue适配器定义在queue头文件上。

上表未列出的queue和priority_queue操作 | 解释
:----- | :-----:
`q.pop()` | 删除queue的首元素或priority_queue的最高优先级元素，但不返回此元素
`q.front()`<br>`q.back()` | 返回首元素或尾元素，但不删除此元素。只适用于queue
`q.top()` | 返回最高优先级元素，但不删除该元素。只适用于priority_queue
`q.push(item)`<br>`q.emplace(args)` | 在queue末尾或priority_queue中恰当的位置创建一个元素。其值为item，或者由args构造

- 标准库queue使用一种先进先出（FIFO）的存储和访问策略。进入队列的对象被放置到队尾（通过q.back()访问），而离开队列的对象则从队首删除（通过q.front()访问）。
- priority_queue允许我们为队列中的元素建立优先级。默认情况下，标准库在元素类型上使用<运算符来确定相对优先级。
