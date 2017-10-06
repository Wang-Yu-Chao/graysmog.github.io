# 动态内存

#### 框架

1. 动态内存与智能指针
    - shared_ptr类
    - 直接管理内存
    - shared_ptr和new结合使用
    - 智能指针和异常
    - unique_ptr
    - weak_ptr
2. 动态数组
    - new和数组
    - allocator类
3. 使用标准库：文本查询程序
    - 文本查询程序设计
    - 文本查询程序类的定义

## 1. 动态内存与智能指针

### shared_ptr类

##### 定义StrBlob类

```CPP
class StrBlob
{
public:
    typedef std::vector<std::string>::size_type size_type;
    StrBlob();
    StrBlob(std::initializer_list<std::string> il);
    size_type size() const { return data->size(); }
    bool empty() const { return data->empty(); }
    // 添加和删除元素
    void push_back(const std::string &t) { data->push_back(t); }
    void pop_back();
    // 元素访问
    std::string& front();
    std::string& back();
private:
    std::shared_ptr<std::vector<std::string>> data;
    // 如果data[i]不合法，抛出一个异常
    void check(size_type i, const std::string &msg) const;
};
```

##### StrBlob构造函数

```CPP
StrBlob::StrBlob() : data(make_shared<vector<string>()) { }
StrBlob::StrBlob(std::initializer_list<string> il) :
    data(make_shared<vector<string>>(il)) { }
```

##### 元素访问成员函数

```CPP
void StrBlob::check(size_type i, const string &msg) const
{
    if (i >= data->size())
        throw out_of_range(msg);
}
string& StrBlob::front()
{
    check(0, "front on empty StrBlob");
    return data->front();
}
string& StrBlob::back()
{
    check(0, "back on empty StrBlob");
    return data->back();
}
void StrBlob::pop_back()
{
    check(0, "pop_back on empty StrBlob");
    data->pop_back();
}
```

### 直接管理内存

##### 使用new动态分配和初始化对象

- *补充：默认初始化*。如果定义变量时没有指定初始值，则变量被默认初始化。默认值由变量类型和定义变量的位置共同决定。定义于任何函数体之外的变量被初始化为0。定义在函数体内部的内置类型变量将不被初始化，其值是未定义的。
- *补充：直接初始化和拷贝初始化*。如果使用等号初始化一个变量，执行拷贝初始化，等号右侧的初始值将被拷贝到新创建的对象中。不使用等号，则执行直接初始化，即圆括号和花括号（列表初始化）。
- *补充：值初始化*。创建一个有大小但无给定初始值的容器时，会对其中所有元素进行值初始化。值初始化由变量类型决定，如果是内置类型，如int，会自动设为0。类类型会调用默认构造函数进行默认初始化。

### shared_ptr和new结合使用

### 智能指针和异常

##### 智能指针和哑类

```CPP
struct destination;                 // 表示正在连接什么
struct connection;                  // 使用连接所需的信息
connection connect(destination*);   // 打开连接
void disconnect(connection);        // 关闭给定的连接
void f(destination &d)
{
    // 获得一个连接；记住使用后要关闭它
    connection c = connect(&d);
    // 使用连接
    // 如果在f推出前忘记调用disconnect，就无法关闭c了
}
```

##### 使用我们自己的释放操作

- 默认情况下，shared_ptr假定它们指向的是动态内存。因此，当一个shared_ptr被销毁时，它默认地对它管理的指针进行delete操作。
- 为了用shared_ptr来管理一个connection，我们必须首先定义一个函数来代替delete。这个**删除器**函数必须能够完成对shared_ptr中保存的指针进行释放的操作。本例中，删除器要接受单个类型为connection*的参数。

```CPP
void end_connection(connection *p)
{
    disconnect(*p);
}
void f(destination &d)
{
    connection c = connect(&d);
    shared_ptr<connection> p(&c, end_connection);
    // 使用连接
    // 当f退出时（即使是由于异常而退出），connection会被正确关闭
}
```

### unique_ptr

### weak_ptr

##### 核查指针类

- 为StrBlob定义一个伴随指针类StrBlobPtr。
- 通过使用weak_ptr，不会影响一个给定的StrBlob所指向的vector的生命周期，但是，可以组织用户访问一个不再存在的vector的企图。

```CPP
class StrBlobPtr
{
public:
    StrBlobPtr() : curr(0) { }
    StrBlobPtr(StrBlob &a, size_t sz = 0) : wptr(a.data), curr(sz) { }
    std::string& deref() const;
    StrBlobPtr& incr();
private:
    std::shared_ptr<std::vector<std::string>> check(std::size_t, const std::string&) const;
    std::weak_ptr<std::vector<std::string>> wptr;
    std::size_t curr;
};
```

```CPP
std::shared_ptr<std::vector<std::string>> StrBlobPtr::check(std::size_t i, const std::string &msg) const
{
    auto ret = wptr.lock();     // vector还存在吗
    if (!ret)
        throw std::runtime_error("unbound StrBlobPtr");
    if (i >= ret->size())
        throw std::out_of_range(msg);
    return ret;                 // 否则，返回指向vector的shared_ptr
}
```

##### 指针操作

- deref成员调用check，检查使用vector是否安全以及curr是否在合法范围内。
```CPP
std::string& StrBlobPtr::deref() const
{
    auto p = check(curr, "dereference past end");
    return (*p)[curr];
}
```

- incr成员也调用check，作为前缀递增，返回递增后对象的引用。
```CPP
StrBlobPtr& StrBlobPtr::incr()
{
    check(curr, "increment past end of StrBlobPtr");
    ++curr;
    return \*this;
}
```

- 为了访问data成员，指针类StrBlobPtr必须声明为StrBlob的friend。我们还要为StrBlob定义begin和end操作，返回一个指向它自身的StrBlobPtr。
```
class StrBlobPtr;
class StrBlob
{
    friend class StrBlobPtr;

    // 其他成员...

    // 返回指向首元素和尾后元素的StrBlobPtr
    StrBlobPtr begin() { return StrBlobPtr(*this); }
    StrBlobPtr end()
        { auto ret = StrBlobPtr(*this, data->size());
          return ret; }
}
```

## 2. 动态数组

### new和数组

### allocator类

```CPP
allocator<string> alloc;            // 可以分配string的allocator对象
auto const p = alloc.allocate(n);   // 分配n个未初始化的string
```

##### allocator分配未构造的内存

```CPP
auto q = p;
alloc.construct(q++);           // *q为空字符串
alloc.construct(q++, 10, 'c');  // *q为cccccccccc
alloc.construct(q++, "hi!");    // *q为hi!
// q指向最后构造的元素之后的位置
```

```CPP
while (q != p)
    alloc.destroy(--q);     // 释放我们真正构造的string
```

```CPP
alloc.deallocate(p, n);
```

##### 拷贝和填充未初始化内存的算法

```CPP
auto p = alloc.allocate(vi.size() * 2);
auto p = uninitialize_copy(vi.begin(), vi.end(), p);
uninitialized_fill_n(q, vi.size(), 42);
```

## 3. 使用标准库：文本查询程序

### 文本查询程序设计

### 文本查询程序类的定义
