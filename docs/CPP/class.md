# 类

#### 框架

1. 定义抽象数据类型
    - 设计Sales_data类
    - 定义Sales_data类
    - 定义类相关的非成员函数
    - 构造函数
    - 拷贝、赋值和析构
2. 访问控制与封装
    - 友元
3. 类的其他特性
    - 类成员再探
    - 返回*this的成员函数
    - 类类型
    - 友元再探
4. 类的作用域
    - 名字查找与类的作用域
5. 构造函数再探
    - 构造函数初始值列表
    - 委托构造函数
    - 默认构造函数的作用
    - 隐式的类类型转换
    - 聚合类
    - 字面值常量类
6. 类的静态成员

#### 概述
- 类的基本思想：数据抽象（data abstraction）和封装（encapsulation）。数据抽象是一种依赖于**接口**（interface）和**实现**（implementation）分离的编程（以及设计）技术。
- 类的接口包括用户能执行的**操作**（成员函数和相关的非成员函数）。
- 类的实现则包括类的**数据成员**、负责接口实现的**函数体**以及定义类所需的各种**私有函数**。

## 1. 定义抽象数据类型

### 设计Sales_data类

设计的Sales_data类的作用是表示一本书的ISBN书号、总销售额（revenue）、售出册数（units_sold）和平均售价（average price）。  
Sales_data的接口应该包含以下操作：
- 一个isbn成员函数，用于返回对象的ISBN编号。
- 一个combine成员函数，用于将一个Sales_data对象加到另一个对象上。
- 一个名为add的函数（非成员函数），执行两个Sales_data对象的加法。
- 一个read函数（非成员函数），将数据从istream读入到Sales_data对象中。
- 一个print函数（非成员函数），将Sales_data对象的值输出到ostream

### 定义Sales_data类

```CPP
struct Sales_data
{
    // 成员函数
    std::string isbn() const { return bookNo; }
    Sales_data& combine(const Sales_data&);
    double avg_price() const;
    // 数据成员
    std::string bookNo;
    unsigned units_sold = 0;
    double revenue = 0.0;
};
// Sales_data的非成员接口函数
Sales_data add(const Sales_data&, const Sales_data&);
std::ostream &print(std::ostream&, const Sales_data&);
std::istream &read(std::istream&, Sales_data&);
```
- 尽管所有成员函数都必须在类的内部**声明**，但是成员函数体可以**定义**在类内也可以定义在类外。
- 定义在类内部的函数是隐式的**内联（inline）函数**。

##### this

- 对象调用成员函数，而成员函数若想要访问调用它的那个对象，就要通过this这个隐式参数来访问。


- 在成员函数内部，可以直接使用对象的数据成员（如`return bookNo`），而不用通过成员访问运算符（.）（如`return object.bookNo`）来做到这一点。因为this指向了这个对象，在成员函数中是通过this隐式地引用了类成员。
- 我们可以在成员函数体内部使用this，虽然没有必要，但是isbn函数可以写成如下形式：
```CPP
std::string isbn() const { return this->bookNo };   // this指向对象的指针，所以用->运算符
```
- 因为this的目的总是指向“这个”对象（每个对象的this都是固定地指向自己），所以this是一个**常量指针**，不允许改变this中保存的地址。

###### 补充内容：**指向常量的指针**（pointer to const）和**常量指针**（const pointer）
- 指向常量的指针不能用于改变其所指向的对象的值，想要获取存放常量对象的地址，只能使用指向常量的指针。
```CPP
const double pi = 3.14;     // pi是一个常量，不能改变
const double *cptr = &pi;   // cptr是一个指向常量的指针
*cptr = 42;                 // 错误：不能通过指向常量的指针改变所指对象的值
```
- 指针也是一个对象，可以把指针本身设为常量。常量指针必须初始化，之后它的值是固定的，不能再改变（常量的性质）。也就是说，指针存储的地址是不变的，指针一直指向同一个位置。把*放在const关键字之前用以说明指针是一个常量。常量指针依然可以通过指针修改所指对象。
- 指向常量的指针和常量指针可以同时成立。
```CPP
int errNumb = 0;
int *const curErr = &errNumb;   // curErr一直指向errNumb
const double pi = 3.14;
const double *const pip = &pi;  // pip是一个指向常量对象的常量指针。
```

##### const成员函数／常量成员函数

- isbn函数的另一个关键之处是紧随参数列表之后的const关键字，这里，const的作用是修改隐式this指针的类型。
- 默认情况下，this的类型是**指向非常量的类类型**的**常量指针**。例如在Sales_data成员函数中，this的类型是Sales_data *const。这意味着（在默认情况下）我们不能把this从一个非常量绑定到一个常量对象上。这一情况就使得我们不能在一个常量对象上调用普通的成员函数。
- C++语言的做法是允许把const关键字放在成员函数的参数列表之后，此时，紧跟在参数列表后面的const表示this是一个指向常量的指针。像这样使用const的成员函数被称作**常量成员函数**。因为this是指向常量的指针，所以常量成员函数不能改变调用它的对象的内容。
- 可以把isbn的函数想象成如下的形式：
```CPP
// 伪代码，说明隐式的this指针是如何使用的。下面的代码是非法的
// 此处this是一个指向常量的指针，因为isbn是一个常量成员函数
std::string Sales_data::isbn(const Sales_data *const this)
{ return this->isbn; }
```
- 这里，isbn可以读取它的对象的数据成员，但不能写入新值。
- 常量对象，以及常量对象的引用或指针都只能调用常量成员函数。

##### 类作用域和成员函数

- 类本身就是一个作用域。类的成员函数的定义嵌套在类的作用域之内。
- 但是即使数据成员（如bookNo）定义在成员函数（如isbn）之后，成员函数依然可以使用数据成员。编译器分两部处理类：首先编译成员的声明，然后才轮到成员函数体。

##### 在类的外部定义成员函数

- 若在类的外部定义成员函数，函数定义必须和声明匹配。如果成员被声明为常量成员函数，那么它的定义也必须在参数列表后明确指定const属性。同时，类外部定义的成员的名字必须包含它所属的类名，如：
```CPP
double Sales_data::average_price() const
{
    if (units_sold)
        return revenue / units_sold;
    else
        return 0;
}
```

##### 定义一个返回this对象的函数

- 函数combine的设计初衷类似于复合函数赋值运算符+=，调用该函数的对象代表的是赋值运算符左侧的运算对象，右侧运算对象则通过显示的实参被传入函数：
```CPP
Sales_data& Sales_data::combine(const Sales_data &rhs)
{
    units_sold += rhs.units_sold;
    revenue += rhs.revenue;
    return *this;
}
```
- return语句解引用this指针以获得执行该函数的对象。
- 内置的赋值运算符把它的左侧运算对象当作左值返回，因此为了保持一致，combine函数必须返回引用类型（Sales_data&）。

### 定义类相关的非成员函数

- 一些辅助函数定义的操作从概念上来说属于类的接口的组成部分，但它们实际上并不属于类本身，即它们是**非成员函数**。
- 定义非成员函数的方式与定义其他函数一样，通常把函数的声明和定义分离开来。如果函数在概念上属于类但是不定义在类中，则它一般应与类声明（而非定义）在同一个头文件内。这样，若用户想要使用接口的任何部分都只需要引入一个文件。

##### 定义read和print函数

```CPP
istream &read(istream &is, Sales_data &item)
{
    double price = 0;
    is >> item.bookNo >> item.units_sold >> price;
    item.revenue = price * item.units_sold;
    return is;
}

ostream &print(ostream &os, const Sales_data &item)
{
    os << item.isbn() << " " << item.units_sold << " "
       << item.revenue << " " << item.avg_price();
    return os;
}
```

- 因为IO类属于不能拷贝的类型，因此只能通过引用来传递它们。而且，因为读取和写入的操作会改变流的内容，所以两个函数接受的都是普通的引用（istream &和ostream &），而不是对常量的引用（如const istream &或const ostream &）。
- 注意此处print函数不负责换行（末尾没有endl）。一般来说，执行输出任务的函数应该尽量减少对格式的控制，这样可以确保由用户代码来决定是否换行。

##### 定义add函数

- add函数接受两个Sales_data对象，返回一个新的Sales_data用来表示前两个对象的和。

```CPP
Sales_data add(const Sales_data &lhs, const Sales_data &rhs)
{
    Sales_data sum = lhs;       // 把lhs的数据成员拷贝给sum
    sum.combine(rhs);           // 把rhs的数据成员加到sum中
    return sum;
}
```

### 构造函数

- 构造函数：当类创建对象时，类通过一种特殊的成员函数来初始化对象，这种函数叫做**构造函数**。构造函数用来初始化对象的**数据成员**。并且只要类的对象被创建，构造函数就会被调用。
- 特点：构造函数的名字和类名相同。构造函数没有返回类型。构造函数已有一个（可能为空的）参数列表和一个（可能为空的）函数体。
- 类可以包含多个构造函数，和其他重载函数相似，不同的构造函数之间必须在参数数量或参数类型上有所区别。

##### 合成的默认构造函数

- 之前的Sales_data类并没有定义任何构造函数，但是仍然可以正确编译运行，其中的数据成员都被默认初始化。类通过**默认构造函数**来默认初始化其数据成员。**默认构造函数无须任何实参**（即构建对象的时候不用传入参数）。如果类中没有显式地定义构造函数，编译器会隐式地定义一个**合成的默认构造函数**，并将按照如下规则初始化类的数据成员：
    - 如果存在类内的初始值，用它来初始化成员。（如`double revenue = 0.0`）
    - 否则，默认初始化该成员。

##### 某些类不能依赖于合成的默认构造函数

- 合成的默认构造函数只是和非常简单的类。对于一个普通的类来说，必须**自己定义默认构造函数**，原因有三：
    1. 编译器只有在发现类不包含**任何**构造函数的情况下才会生成一个默认的构造函数。一旦已经定义了其他的构造函数，除非再定义一个默认的构造函数，否则类将没有默认构造函数。
    2. 对于某些类来说，合成的默认构造函数可能执行错误的操作。如，定义在块中的内置类型或复合类型（比如数组和指针）的对象被默认初始化后的值将是未定义的。因此，含有内置类型或复合类型成员的类应该在类的内部初始化这些成员，或者定义一个自己的默认构造函数。（补充：定义于任何函数之外的变量会被初始化为0，但定义在函数体内部的内置类型变量将不被初始化）。
    3. 有的时候编译器不能为某些类合成默认的构造函数。例如，如果类中包含一个其他类类型的成员，而这种类没有默认构造函数，则编译器无法初始化该成员。所以需要自己定义默认构造函数。

##### 定义Sales_data的构造函数

```CPP
struct Sales_data
{
    // 新增的构造函数
    Sales_data() = default;
    Sales_data(const std::string &s) : bookNo(s) { }
    Sales_data(const std::string &s, unsigned n, double p) :
        bookNo(s), units_sold(n), revenue(p * n) { }
    Sales_data(std::istream &);

    // 之前已有的其他成员...
};
```
- =default的含义和构造函数初始值列表（冒号:后的部分）将在下面解释。

##### =default的含义

- 语句`Sales_data() = default;`不接受实参，所以是一个默认构造函数。根据上述的需要自定义默认构造函数的原因1，因为存在其他形式的构造函数，所以需要显式地定义默认构造函数。而我们有希望这个默认构造函数和编译器的合成默认构造函数作用一样。所以C++11标准中，通过在参数列表后写上`= default`来要求编译器成成默认构造函数。
- `= default`既可以和声明一起出现在类的内部，也可以作为定义出现在类的外部。
```CPP
struct Sales_data
{
    Sales_data();
    // .......
};
Sales_data::Sales_data() = default;
```
- 和其他函数一样，如果`= default`在类的内部，则默认构造函数是内联的（inline）；如果它在类的外部，则该成员默认不是内联的。

##### 构造函数初始值列表

- 上述有两个构造函数的定义中出现了新的部分，即冒号以及冒号和花括号之间的代码，其中花括号定义了（空的）函数体。我们把新出现的部分成为**构造函数初始值列表**。它负责为新创建的对象的一个或几个数据成员赋值。
- 形式：构造函数初始值是成员名字的一个列表，每个名字后面紧跟括号中的成员初始值。不同成员的初始化通过逗号（,）分割开。
- 语句`Sales_data(const std::string &s) : bookNo(s) { }`只用参数s初始化了成员bookNo，而另外两个数据成员units_sold和revenue则没有显式地初始化，此时，它们将与合成默认构造函数相同的方式隐式初始化（类内的初始值或默认初始化）。

##### 在类的外部定义构造函数

- 和其他几个构造函数不同，以istream为参数的构造函数需要执行一些实际的操作。在它的函数体内，调用了read函数以给数据成员以赋值：
```CPP
Sales_data::Sales_data(std::istream &is)
{
    read(is, *this);    // read函数从is读取一条信息，然后存入this对象中
}
```
- 当我们在类的外部定义构造函数时，和其他定义在外部的成员函数一样，需要指明属于哪个类的成员（如Sales_data::）。
- 注意，这里使用this来把对象当成一个整体访问，而非直接访问对象的某个成员。

### 拷贝、赋值和析构

- 定义类的行为：初始化、拷贝、赋值、销毁。如果我们不主动定义这些操作，则编译器将替我们合成它们。一般来说，编译器合成的版本会将对象中的每个成员执行初始化、拷贝、赋值和销毁。
- 但对于某些类来说，合成的版本无法正常工作。特别地，当类需要分配类对象之外的资源时，合成的版本将会失效。不过，很多需要动态内存的类能（而且应该）通过使用vector或者string管理必要的存储空间，使用它们能避免分配和释放内存带来的复杂性。

## 2. 访问控制与封装

- 到现在为止，Sales_data类已经定义了接口，但并没有任何机制强制用户使用这些接口而不是直接访问类里面的成员数据。也就是说，这个类还没有**封装**，导致用户可以直达类的内部并且控制具体实现细节。在C++中，可以使用**访问说明符**（access specifiers）加强类的封装性。
    - 定义在public说明符之后的成员在整个程序内可被访问，**public成员定义类的接口**。
    - 定义在private说明符之后的成员可以被类的成员函数访问，但是不能被使用该类的代码访问，**private部分封装（隐藏）了类的实现细节**。
- 一个类可以包含0个或多个访问说明符，而且对于某个访问说明符能出现多少次也没有严格规定。
- Sales_data的新形式：
```CPP
class Sales_data
{
public:
    Sales_data() = default;
    Sales_data(const std::string &s) : bookNo(s) { }
    Sales_data(const std::string &s, unsigned n, double p) :
        bookNo(s), units_sold(n), revenue(p * n) { }
    Sales_data(std::istream &);
    std::string isbn() const { return bookNo; }
    Sales_data &combine(const Sales_data&);

private:
    double avg_price() const { return units_sold ? revenue / units_sold : 0; }  // 除法中注意要判断除数不为0
    std::string bookNo;
    unsigned units_sold = 0;
    double revenue = 0.0;
};
```

##### 使用class或struct关键字

- 上述代码中由一个微妙变化，使用了**class**关键字而不是**struct**来开始类的定义。class和struct作用基本相同，都可以用来定义类。唯一一点区别是，struct和class的默认访问权限不同。
    - 类可以在它的第一个访问说明符之前定义成员。如果使用struct，则定义在第一个访问说明符之前的成员是public的。
    - 相反，若使用class，则这些成员是private的。

### 友元

- 既然Sales_data的数据成员是private的，我们的read、print和add函数也就无法正常编译了，因为它们不是类的成员，即使它们是类的接口的一部分。
- 类可以允许其他类或者函数访问它的非公有成员，方法是令其他**类**或者**函数**成为它的**友元**（friend）。如果类想要把一个函数作为它的友元，只需要增加一条以friend关键字开始的函数声明语句即可：
```CPP
class Sales_data
{
    // 为Sales_data的非成员函数所做的友元声明
    friend Sales_data add(const Sales_data&, const Sales_data&);
    friend std::istream &read(std::istream&, Sales_data&);
    friend std::ostream &print(std::ostream&, const Sales_data&);

    // ......
};
// Sales_data接口的非成员组成部分的函数声明
Sales_data add(const Sales_data&, const Sales_data&);
std::istream &read(std::istream&, Sales_data&);
std::ostream &print(std::ostream&, const Sales_data&);
```
- 友元声明只能出现在类定义的内部，但是在类内出现的具体位置不限。友元不是类的成员，也不接受它所在的区域访问控制级别的约束。
- 一般来说，最好在类定义开始或结束前的位置集中声明友元。

##### 友元的声明

- 友元的声明仅仅指定了访问的权限，而非正常的函数声明，所以不能替代函数声明。必须在友元声明之外再专门对函数进行一次声明。
- 通常把友元的函数声明和类本身放置在同一个头文件中（类的外部）。

## 3. 类的其他特性

- 本节主要介绍：类型成员、类的成员的类内初始值、可变数据成员、内联函数成员、从成员函数返回*this、关于如何定义并使用类类型及友元类。

### 类成员再探

- 这里需要两个新类作为例子：Screen和Window_mgr。

##### 定义一个类型成员



### 返回*this的成员函数

### 类类型

### 友元再探

## 4. 类的作用域

### 名字查找与类的作用域

## 5. 构造函数再探

### 构造函数初始值列表

### 委托构造函数

### 默认构造函数的作用

### 隐式的类类型转换

### 聚合类

### 字面值常量类

## 6. 类的静态成员
