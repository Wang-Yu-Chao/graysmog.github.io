# 拷贝控制

- 当定义一个类时，我们显式地或隐式地指定在此类型的对象拷贝、移动、赋值和销毁时做什么。一个类通过定义五种特殊的成员函数来控制这些操作，包括：拷贝构造函数（copy constructor）、拷贝赋值运算符（copy-assignment operator）、移动构造函数（move constructor）、移动赋值运算符（move-assignment operator）和析构函数（destructor）。
- 拷贝和移动构造函数定义了当用同类型的另一个对象初始化本对象时做什么；拷贝和移动赋值运算符定义了将一个对象赋予同类型的另一个对象时做什么；析构函数定义了当此类型对象销毁时做什么。我们称这些操作为拷贝控制操作。

## 1. 拷贝、赋值与销毁

### 拷贝构造函数

- 如果一个构造函数的第一个参数是**自身类类型的引用**，且任何额外参数都由默认值，则此构造函数是**拷贝构造函数**。
```CPP
class Foo
{
public:
    Foo();              // 默认构造函数
    Foo(const Foo&);    // 拷贝构造函数
    // ...
};
```

- 拷贝构造函数的第一个参数必须是一个引用类型，此参数几乎总是一个**const的引用**。拷贝构造函数在几种情况下都会被隐式地使用。因此，拷贝构造函数通常不应该是explicit的。

##### 合成拷贝构造函数

- 如果没有为类定义一个拷贝构造函数，编译器会为我们定义一个。与合成默认构造函数不同，即使我们定义了其他构造函数，编译器也会为我们合成一个拷贝构造函数。
- 对某些类来说，*\*合成拷贝构造函数**用来阻止我们拷贝该类类型的对象。而一般情况，合成的拷贝构造函数会将其参数的成员逐个拷贝到正在创建的对象中。编译器从给定对象中一次将每个非static成员拷贝到正在创建的对象中。
- 每个成员的类型决定了它如何拷贝：对类类型（如Sales_data）的成员，使用它自己的拷贝构造函数来拷贝；对于内置类型（如int）的成员则直接拷贝。
```CPP
class Sales_data
{
public:
    // 与合成的拷贝构造函数等价的声明
    Sales_data(const Sales_data&);
private:
    std::string bookNo;
    int units_sold = 0;
    double revenue = 0.0;
};
// 该拷贝构造函数和合成拷贝构造函数等价
Sales_data::Sales_data(const Sales_data &orig) :
    bookNo(orig.bookNo),            
    units_sold(orig.units_sold),
    revenue(orig.revenue)
    {   }                           // 空函数体
```

##### 拷贝初始化

- 直接初始化和拷贝初始化的差异：
    - 使用直接初始化时，实际上是要求编译器使用普通的函数匹配来选择与我们提供的参数最匹配的构造函数。
    - 使用拷贝初始化时，我们要求编译器将右侧运算对象拷贝到正在创建的对象中，如果需要的话还要进行类型转换。
- 如果一个类由一个移动构造函数，则拷贝初始化有时使用移动构造函数而非拷贝构造函数来完成。
- 拷贝初始化在以下情况会发生：
    - 用=定义变量
    - 将一个对象作为实参传递给（函数的）非引用类型的形参。（引用类型不需要拷贝）
    - 从一个返回类型为非引用类型的函数返回一个对象。（同上）
    - 用花括号列表初始化一个数组中的元素或一个聚合类中的成员。
    - 某些类类型还会对它们所分配的对象使用拷贝初始化。如，当我们初始化标准库容器或是调用其insert或push成员时，容器会对其元素进行拷贝初始化。与之相对，用emplace成员创建的元素都进行直接初始化。

##### 参数和返回值

- 在函数调用的过程中，具有非引用类型的参数要进行拷贝初始化。类似的，当一个函数具有非引用的返回类型时，返回值会被用来初始化调用方的结果。
- 拷贝构造函数被用来初始化非引用类类型参数，这一特性解释了为什么拷贝构造函数自己的参数必须是引用类型：如果参数不是引用类型，则调用永远也不会成功——为了调用拷贝构造函数，我们必须拷贝这个函数的实参，但为了拷贝实参，有需要调用拷贝构造函数，如此无限循环。

##### 拷贝初始化的限制


### 拷贝赋值函数

### 析构函数

### 三／五法则

### 使用=default

### 阻止拷贝

## 2. 拷贝控制和资源管理

### 行为像值的类

### 定义行为像指针的类

## 3. 交换操作

## 4. 拷贝控制示例

##### Message类

```CPP
class Message
{
    friend class Folder;
public:
    // folders被隐式初始化为空集合
    explicit Message(const std::string &str = "") : contents(str) { }
    Message(const Message&);
    Message& operator=(const Message&);
    ~Message();
    // 从给定Folder集合中添加／删除本Message
    void save(Folder&);
    void remove(Folder&);
private:
    std::string contents;       // 实际消息文本
    std::set<Folder*> folders;  // 包含本Message的Folder
    // 拷贝构造函数、拷贝赋值运算符和析构函数所使用的工具函数
    // 将本Message添加到指向参数的Folder中
    void add_to_Folders(const Message&);
    // 从folders中的每个Folder中删除本Message
    void remove_from_Folders();
};
```

##### save和remove成员

```CPP
void Message::save(Folder &f)
{
    folders.insert(&f);
    f.addMsg(this);
}

void Message::remove(Folder &f)
{
    folders.erase(&f);
    f.remMsg(this);
}
```

##### Message类的拷贝控制成员

```CPP
void Message::add_to_Folders(const Message &m)
{
    for (auto f : m.folders)
        f->addMsg(this);
}
```

```CPP
Message::Message(const Message &m) :
    contents(m.contents), folders(m.folders)
{
    add_to_Folders(m);
}
```

##### Message的析构函数

```CPP
void Message::remove_from_Folders()
{
    for (auto f : m.folders)
        f->remMsg(this);
}
```

```CPP
Message::~Message()
{
    remove_from_Folders();
}
```

##### Message的拷贝赋值运算符

```CPP
Message& Message::operator=(const Message &rhs)
{
    remove_from_Folders();
    contents = rhs.contents;
    folders = rhs.folders;
    add_to_Folders(rhs);
    return \*this;
}
```

##### Message的swap函数

```CPP
void swap(Message &lhs, Message &rhs)
{
    using std::swap;
    for (auto f : lhs.folders)
        f->remMsg(&lhs);
    for (auto f : rhs.folders)
        f->remMsg(&rhs);
    swap(lhs.contents, rhs.contents);
    swap(lhs.folders, rhs.folders);
    for (auto f : lhs.folders)
        f->addMsg(&lhs);
    for (auto f : rhs.folders)
        f->addMsg(&rhs);
}
```

## 5. 动态内存管理

- 某些类需要在运行时分配可变大小的内存空间。这种类通常可以使用标准库容器来保存它们的数据。
- 但是某些类需要自己进行内存分配。这些类一般来说必须定义自己的拷贝控制成员来管理所分配的内存。
- 这里将实现标准库vector类的一个只适用于string的简化版本，命名为StrVec。

##### StrVec类的设计

- 每个StrVec有三个指针成员指向其元素所使用的内存：
    - elements，指向分配的内存中的首元素
    - first_free，指向最后一个实际元素之后的位置
    - cap，指向分配的内存末尾之后的位置
- 除了这些指针外，StrVec还有一个名为alloc的静态成员，其类型为allocator<string>。alloc成员会分配StrVec使用的内存。
- 4个工具函数：
    - alloc_n_copy分配内存，并拷贝一个给定范围中的元素
    - free会销毁构造的元素并释放内存
    - chk_n_alloc保证StrVec至少有容纳一个新元素的空间。如果没有空间添加新元素，chk_n_alloc会调用reallocate来分配更多内存
    - reallocate在内存用完时为StrVec分配内存
- 虽然关注的是类的实现，但也将定义vector接口中的一些成员。

##### StrVec类定义

```CPP
class StrVec
{
public:
    StrVec() : elements(nullptr), first_free(nullptr), cap(nullptr)  { }
    StrVec(const StrVec&);
    StrVec& operator=(const StrVec&);
    ~StrVec();
    void push_back(const std::string&);
    size_t size() const { return first_free - elements; }
    size_t capacity() const { return cap - elements; }
    std::string* begin() const { return elements; }
    std::string* end() const { return first_free; }
    // ...
private:
    static std::alllocator<string> alloc;
    void chk_n_alloc()
        { if (size() == capacity()) reallocate(); }
    std::pair<std::string*, std::string*> alloc_n_copy(const std::string*, const std::string*);
    void free();
    void reallocate();
    std::string *elements;
    std::string *first_free;
    std::string *cap;
};
```

##### 使用construct

```CPP
void StrVec::push_back(const string& s)
{
    chk_n_alloc();
    alloc.construct(first_free++, s);
}
```

##### alloc_n_copy成员

```CPP
pair<string*, string*> StrVec::alloc_n_copy(const string *b, const string *e)
{
    auto data = alloc.allocate(e - b);
    return {data, uninitialized_copy(b, e, data)};
}
```

##### free成员

```CPP
void StrVec::free()
{
    // 不能传递给deallocate一个空指针，如果elements为0（nullptr），函数什么也不做
    if (elements)  
    {
        auto p = first_free;
        while (p-- != elements)
            alloc.destroy(p);
        alloc.deallocate(elements, cap - elements);
    }
}
```

##### 拷贝控制成员

```CPP
StrVec::StrVec(const StrVec &s)
{
    auto newdata = alloc_n_copy(s.elements, s.first_free);
    elements = newdata.first;
    cap = first_free = newdata.second;
}

StrVec& StrVec::operator=(const StrVec &s)
{
    if (this != &s)
    {
        free();
        auto newdata = alloc_n_copy(s.elements, s.first_free);
        elements = newdata.first;
        cap = first_free = newdata.second;
    }
    return \*this;
}

StrVec::~StrVec()
{
    free();
}
```

##### reallocate成员

```CPP
void StrVec::reallocate()
{
    auto newcapacity = size() ? 2 * size() : 1;
    auto newdata = alloc.allocate(newcapacity);
    auto dest = newdata;    // 指向新数组中下一个空闲位置
    auto elem = elements;   // 指向旧数组中下一个元素
    for (size_t i = 0; i != size(); i++)
        alloc.construct(dest++, std::move(*elem++));
    free();                 // 一旦移动完元素就释放旧内存空间
    elements = newdata;
    first_free = dest;
    cap = elements + newcapacity;
}
```

## 6. 对象移动

### 右值引用

### 移动构造函数和移动赋值运算符

### 右值引用和成员函数
