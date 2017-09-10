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
- 默认情况下，this的类型是**指向非常量的类类型**的**常量指针**。例如在Sales_data成员函数中，this的类型是Sales_data \*const。这意味着（在默认情况下）我们不能把this从一个非常量绑定到一个常量对象上。这一情况就使得我们不能在一个常量对象上调用普通的成员函数。
- C++语言的做法是允许把const关键字放在成员函数的参数列表之后，此时，紧跟在参数列表后面的const表示this是一个指向常量的指针。像这样使用const的成员函数被称作**常量成员函数**。因为this是指向常量的指针，所以常量成员函数不能改变调用它的对象的内容。
- 可以把isbn的函数想象成如下的形式：
```CPP
// 伪代码，说明隐式的this指针是如何使用的。下面的代码是非法的
// 此处this是一个指向常量的指针，因为isbn是一个常量成员函数
std::string Sales_data::isbn(const Sales_data *const this)
{ return this->isbn; }
```
- 这里，isbn可以读取它的对象的数据成员，但不能写入新值。
- **常量对象，以及常量对象的引用或指针都只能调用常量成员函数**。

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
    return \*this;
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
- 不同于其他成员函数，构造函数不能被声明为const的。当创建类的const对象时，直到构造函数完成初始化过程，对象才能真正取得其“常量”属性。因此，构造函数在const对象的构造过程中可以向其写值。

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
- Screen表示显示器中的一个窗口。每个Screen包含一个用于保存Screen内容的string成员和是哪个string::size_type类型的成员，分别表示光标的位置以及屏幕的高和宽。

##### 定义一个类型成员

- 除了定义数据和函数成员之外，类还可以自定义某种类型在类中的别名。由类定义的类型名字和其他成员函数一样存在访问限制，可以是public或者private中的一种。
```CPP
class Screen
{
public:
    typedef std::string::size_type pos;
private:
    pos cursor = 0;
    pos height = 0, weight = 0;
    std::string contents;
};
```
- Screen在public中定义了pos，所以用户可以使用这个名字。通过把pos定义成public成员，可以隐藏Screen实现的细节。
- 使用类型别名由两点注意：
    1. 既可以使用typedef也可以使用别名声明（using）：
    ```CPP
    class Screen
    {
    public:
        using pos = std::string::size_type;
    };
    ```
    2. 用来定义类型的成员必须先定义后使用，这一点与普通成员有所区别。

##### Screen类的成员函数

```CPP
class Screen
{
public:
    typedef std::string::size_type pos;
    screen() = default;
    Screen(pos ht, pos wd, char c) : height(ht), width(wd),
        contents(ht * wd, c) { }
    char get() const { return contents[cursor] };           // 隐式内联。读取光标处字符
    inline char get(pos ht, pos wd) const;                  // 显式内联。
    Screen &move(pos r, pos c);                             // 此处没有，但能在之后被设为内联
private:
    pos cursor = 0;
    pos height = 0, width = 0;
    std::string contents;
};
```

- 第二个构造函数为cursor成员隐式地使用了类内初始值。

##### 令成员作为内联函数

- 在类中，定义在类内部的成员函数是自动inline的。因此，Screen的构造函数和get函数默认是inline的。
- 也可以在类的内部把inline作为声明的一部分显式地声明成员函数。
- 同样的，也能在类的外部用inline关键字修饰函数的定义。
```CPP
inline Screen &Screen::move(pos r, pos s)       // 可以在函数的定义处指定inline
{
    pos row = r * width;        // 将二维的行数映射到一位数组上
    cursor = row + c;
    return \*this;
}

char Sceen::get(pos r, pos c) const     // 在类的内部声明成inline，定义出可以不用指定inline
{
    pos row = r * width;
    return contents[row + c];
}
```
- 在声明和定义的地方都可以说明inline，不过最好只在类外部定义的地方说明inline，这样可以使类更容易理解。

##### 重载成员函数

- 和非成员函数一样，成员函数可以被重载，只要函数之间在参数的数量或类型上有区别就可以（返回类型是否相同是无关的）。如两个版本的get函数。

##### 可变数据成员

- 有时会发生这样一种情况，我们希望能修改类的某个数据成员，即使是在一个const成员函数内。可以通过在变量的声明中加入mutable关键字做到。
- 一个*可变数据成员*（mutable data member）永远不会是const，即使它是const对象的成员。因此，一个const成员函数可以改变一个可变成员的值。
- 举个例子，为Screen类添加一个名为access_ctr的可变成员，通过它我们可以追踪每个Screen的成员函数被调用了多少次：
```CPP
class Screen
{
public:
    void some_member() const;
private:
    mutable size_t access_ctr;     // 即使在一个const对象内也能被修改
};

void Screen::some_member() const
{
    ++access_ctr;
}
```
- 尽管some_member是一个const成员函数，它仍然能够改变access_ctr的值。该成员是个可变成员，因此任何成员函数都能改变它的值。

##### 类数据成员的初始值

- *补充：类内初始值*。C++11规定，可以为数据成员提供一个**类内初始值**（in-class initializer）。创建对象时，类内初始值将用于初始化数据成员。没有初始值的成员将被默认初始化。提供类内初始值必须使用**等号**或者**花括号**（列表初始化），不能使用圆括号。
- 我们继续定义一个窗口管理类Window_mgr来表示显示器上的一组Screen。这个类包含一个Screen类型的vector。我们希望每个Window_mgr类开始时总是拥有一个默认初始化的Screen，这一点可通过类内初始值实现：
```CPP
class Window_mgr
{
private:
    std::vector<Screen> screens{Screen(24, 80, ' ')};
    // 对vector对象进行列表初始化。默认情况下，一个Window_mgr包含一个标准尺寸的空白Screen   
}
```

### 返回*this的成员函数

- *补充:左值和右值*。C++表达式要不然是右值（rvalue），要不然是左值（lvalue）。一个左值表达式的求值结果是一个对象或者一个函数。当一个对象被用作右值时，用的是对象的值（内容）；当对象被用作左值时，用的是对象的身份（在内存中的位置）。
- *补充：引用返回左值*。函数的返回类型决定函数调用是否是左值。**调用一个返回引用的函数得到左值**，其他返回类型得到右值。可以像其他左值一样使用返回引用的函数，特别是将其赋值（除了返回常量引用）：
```CPP
char &get_val(string &str, string::size_type ix)    // 返回引用的函数，得到左值
{
    return str[ix];
}
int main()
{
    string s("a value");
    get_val(s, 0) = 'A';        // 将s[0]的值改为A，s为"A value"
    return 0;
}
```

- 接下来继续添加一些函数，它们负责设置光标所在位置的字符或者其他任一给定位置的字符（set函数）：
```CPP
class Screen
{
public:
    Screen &set(char);
    Screen &set(pos, pos, char);
    // 其他成员同上...
};
inline Screen &Screen::set(char c)
{
    contents[cursor] = c;
    return \*this;
}
inline Screen &Screen::set(pos r, pos col, char ch)
{
    contents[r * width + col] = ch;
    return \*this;
}
```
- set成员的返回值是调用set的对象的引用，返回引用的函数是左值的，意味着这些函数返回的是对象本身而非对象的副本。从而可以把一系列操作连接在一条表达式中，在同一个对象上执行：
```CPP
myScreen.move(4, 0).set('#');
```
- 如果令move和set返回Screen而非Screen&，则返回值将是*this的副本（拷贝），上述语句将不能改变myScreen的值。

##### 从const成员函数返回*this

- 一个const成员函数如果以引用的形式返回*this，那么它的返回类型将是常量引用。
- 接下来添加一个名为display的操作，它负责打印Screen的内容，同时它也能和move、set出现在同一序列中，所以display也应该返回执行它的对象的引用。并且，display也不需要改变它的内容，因此令display为一个const成员函数。此时this为指向常量的常量指针。
- 由此推断，display的返回类型应该是const Sales_data&，但是，如果真的令display返回一个const的引用，则我们将不能把display嵌入到一组动作的序列中去：
```CPP
Screen myScreen;
myScreen.display(cout).set('*');    // 如果display返回常量引用，则再调用set发生错误！
```

##### 基于const的重载

- *补充：重载和const形参*。顶层const不影响传入函数的对象；而通过区分底层const和非底层const可以实现函数重载。
- 通过区分成员函数是否是const的，我们可以对其进行重载，其原因与我们之前根据指针参数是否指向const而重载函数的原因差不多。
- 一方面，只能在一个常量对象上调用const成员函数。另一方面，虽然可以在非常量对象上调用常量版本或非常量版本，但显然此时非常量版本是一个更好的选择。
- 下面这个例子中，定义了一个名为do_display的私有成员，由它负责打印Screen的实际工作。所有的display操作将调用这个函数，然后返回执行这个操作的对象：
```CPP
class Screen
{
public:
    Screen &display(std::ostream &os)
    {
        do_display(os);
        return  \*this;
    }
    const Screen &display(std::ostream &os) const
    {
        do_display(os);
        return \*this;
    }
private:
    void do_display(std::ostrem &os) const { os << contents; }
};
```
- 当一个成员调用另一个成员时，this指针在其中隐式地传递。因此，display调用do_display时，它的this指针隐式地传递给do_display，而当display的非常量版本调用do_display时，它的this指针将隐式地从指向非常量的指针转换成指向常量的指针。
- do_display完成后，两display分别返回解引用的非常量和常量引用。
- *建议：对于公共代码使用私有功能函数*。定义一个单独的do_display函数的原因：
    - 避免在多处使用同样的代码。
    - display可能会更加复杂，公共代码的作用将更加明显。
    - 添加调试信息更加容易。
    - **这个额外的函数调用不会增加任何开销。因为我们已经类内部定义了do_display，所以它隐式地被声明为内敛函数。**

### 类类型

- 没个类定义了唯一的类型。对于两个类来说，即使它们的成员完全一样，这两个类也是两个不同的类型。
- 我们可以把类名作为类型的名字使用，或者可以把类名跟在关键字class或struct之后：
```CPP
Sales_data item;
class Sales_data item;  // 相互等价的声明
```

##### 类的声明

- 同函数一样，可以仅声明类而暂时不定义它。
```CPP
class Screen;
```
- 这种声明有时被称为**前向声明**。在它声明之后，定义之前，它是一个*不完全类型*。
- 不完全声明只能在非常有限的情况下使用：
    - 可以定义指向这种类型的**指针或引用**。
    - 可以声明（但不能定义）用不完全类型作为**参数或返回值**的函数。
- 在创建一个类的对象之前这个类必须被定义过，而不能仅仅被声明。否则编译器无法了解这样的对象需要多少存储空间。
- 直到类被定义之后数据成员才能被声明成这种类类型，所以一个类的成员类型不能是该类自己。然而，一旦一个类的名字出现后，它就被认为是声明过了，因此类允许包含指向它自身的引用或指针：
```CPP
class Link_screen
{
    Screen window;
    Link_screen \*next;
    Link_screen \*prev;
};
```

### 友元再探

- 除了函数，类还可以把其他类定义成友元，也可以把其他类的成员函数定义成友元。

##### 类之间的友元关系

- 如果需要为Window_mgr添加一个名为clear的成员，它负责把一个指定的Screen的内容都设为空白，则clear需要访问Screen的私有成员。想要令这种访问合法，Screen需要把Window_mgr指定成它的友元
```CPP
class Screen
{
    friend class Window_mgr;
};

class Window_mgr
{
public:
    using ScreenIndex = std::vector<Screen>::size_type;
    void clear(ScreenIndex);
private:
    std::vector<Screen> screens{Screen(24, 80, ' ')};
};
void Window_mgr::clear(ScreenIndex i)
{
    Screen &s = screens[i];
    s.contents = string(s.height * s.width, ' ');
}
```
- 友元不存在传递性。每个类负责控制自己的友元类或友元函数。如果Window_mgr有它自己的友元，则这些友元不因此成为Screen的友元。

##### 令成员函数作为友元

- 当把一个成员函数声明为友元时，我们必须明确指出该成员函数属于哪个类：
```CPP
class Screen
{
    // Window_mgr::clear必须在Screen类之前被声明
    friend void Window_mgr:\:clear(ScreenIndex);
    // ......
};
```
- 如果想要令某个成员函数作为友元，我们必须**仔细组织程序的结构以满足声明和定义的彼此依赖关系**。这个例子中，我们必须按照如下方式设计程序：
    - 首先定义Window_mgr类，其中声明clear函数，但是不能定义它。在clear使用Screen的成员之前必须先声明Screen。
    - 接下来定义Screen，包括对于clear的友元声明。
    - 最后定于clear，此时它才可以使用Screen的成员。

##### 函数重载和友元

- 尽管重载函数的名字相同，但它们仍然是不同的函数。因此，如果一个类想把一组重载函数声明成它的友元，它需要对这组函数中的**每一个分别声明**：
```CPP
extern std::ostream& storeOn(std::ostream &, Screen &);
extern BitMap& storeOn(BitMap &, Screen &);
class Screen
{
    friend std::ostream& storeOn(std::ostream &, Screen &);
    // ......
};
```

##### 友元声明和作用域

- 类和非成员函数的声明不是必须在它们的友元声明之前。当一个名字第一次出现在一个友元声明中时，我们隐式地假定该名字在当前作用域中是可见的。然而，友元本身不一定真的声明在当前作用域中。
- 甚至就算在类的内部定义该函数，我们也必须在类的外部提供相应的声明从而使得函数可见。换句话说，即使我们仅仅是用声明友元的类的成员调用该友元函数，它也必须是被声明过的。

```CPP
struct X
{
    friend void f() { /* 友元函数可以定义在类的内部 \*/ }
    X() { f(); }
    void g();
    void h();
};
void X::g() { return f(); }     // 错误：f还没有被声明
void f();                       // 声明那个定义在X中的函数
void X::h() { return f(); }     // 正确：现在f在作用域中了
```
- 关于这段代码最重要的是理解友元声明的作用是影响访问权限，它本身并非普通意义上的声明。

## 4. 类的作用域

- 在类的作用域之外：
    1. 对于**普通的数据和函数成员**只能由对象、引用或者指针使用成员访问运算符（.和->）来访问。
    2. 对于**类的类型成员**则使用作用域运算访问符（::）访问。
```CPP
Screen::pos ht = 24, wd = 80;   // 使用Screen定义的pos类型
Screen scr(ht, wd, ' ');
Screen *p = &scr;
char c = scr.get();
c = p->get();
```

##### 作用域和定义在类外部的成员

- 一个类就是一个作用域。在类的外部，成员的名字被隐藏起来了。一旦遇到了类名，定义的剩余部分（参数列表和函数体）就在类的作用域之内了，可以直接使用类的其他成员。
```CPP
void Window_mgr::clear(ScreenIndex i)
{
    Screen &s = screens[i];
    s.contents = string(s.height * s.width, ' ');
}
```
- 因为编译器在处理参数列表之前已经明确了我们当前正位于Window_mgr类的作用域中，所以不必再专门说明ScreenIndex是Window_mgr类定义的。screens也是在Window_mgr类中定义的。
- 另一方面，函数的返回类型通常出现在函数名之前。因此当成员函数定义在类的外部时，返回类型中使用的名字都位于类的作用域之外。这时，**返回类型必须指明它是哪个类的成员**。
- 例如加入addScreen新函数，用来向显示器添加一个新的屏幕，返回类型为ScreenIndex：
```CPP
class Window_mgr
{
public:
    ScreenIndex addScreen(const Screen&);
    // ......
};
Window_mgr::ScreenIndex Window_mgr::addScreen(const Screen &s)
{
    screens.push_back(s);
    return screens.size() - 1;
}
```

### 名字查找与类的作用域

- 类的定义分为两步处理：
    - 首先，编译成员的声明。
    - 知道类全部可见后才编译函数体。
    - 所以，当编译器处理完类中的全部声明后才会处理成员函数的定义。

##### 用于类成员声明的名字查找

- 这种两阶段的处理方式只适用于成员函数中使用的名字。声明中使用的名字，包括返回类型或者参数列表中使用的名字，都必须在使用前确保可见。

```CPP
typedef double Money;
string bal;
class Account
{
public:
    Money balance() { return bal; }
private:
    Money bal;
};
```

- 编译器对于成员函数中使用的名字，首先会在类中的成员中查找，若找到则返回；没有找到则在类之前的外层作用域中查找。
- 当编译器看到balance函数的声明语句时，它将在Account类的范围内寻找对Money的声明，没有找到后它会在Account的外层作用域中查找。
- balance函数返回的bal是类中的成员，而不是外层作用域的string对象。

##### 类型名要特殊处理

- 一般来说，内层作用域可以重新定义外层作用域中的名字，即使该名字已经在内层作用域中使用过。然而在类中，如果成员使用了外层作用域中的某个名字，而该名字代表一种类型，则类不能在之后重新定义该名字：
```CPP
typedef double Money;
class Account
{
public:
    Money balance() { return bal; } // 使用外层作用域的Money
private:
    typedef double Money;           // 错误：不能重新定义Money
    Money bal;
    // ...
}
```

##### 成员定义中的普通块作用域的名字查找

- 成员函数中使用的名字按照如下方式解析：
    - 首先**在成员函数内**查找该名字的声明。只有在函数使用之前出现的声明才被考虑。
    - 如果在成员函数内没有找到，则**在类内**继续查找，这时类的所有成员都可以被考虑。
    - 如果类内也没找到该名字的声明，在成员函数定义**之前的作用域内**继续查找。
- 一般来说，不建议使用其他成员的名字作为某个成员函数的参数。最好使用其他名字。
```CPP
// 不好的例子
int height;
class Screen{
public:
    typedef std::string::size_type pos;
    void dummy_fcn(pos height)
    {
        cursor = width * height;     // 这里的height是成员函数参数height
    }
private:
    pos cursor = 0;
    pos height = 0, width = 0;
}
```
- 尽管类的成员被隐藏了，仍然可以通过加上类的名字(Screen::height)或显式地使用this指针(this->height)来强制访问成员。

##### 类作用域之后，在外围的作用域中查找

- 外层作用域中的名字可以显式地通过作用域运算符来进行请求：
```CPP
// 不建议的写法：不要隐藏外层作用域中可能被用到的名字
void Screen::dummy_fcn(pos height)
{
    cursor = width * ::height;      // 全局的height
}
```

## 5. 构造函数再探

### 构造函数初始值列表

- 当我们定义变量时习惯于立即对其进行初始化，而非先定义，再赋值。就对象的数据成员而言，初始化和赋值也有类似的区别。
- **如果没有在构造函数的初始值列表中显式地初始化成员，则该成员将在构造函数体之前执行默认初始化**。
```CPP
Sales_data::Sales_data(const string &s, unsigned cnt, double price)
{
    bookNo = s;
    units_sold = cnt;
    revenue = cnt * price;
}
```
- 这个版本的Sales_data构造函数是对成员进行了赋值操作，而非直接初始化。

##### 构造函数的初始值有时必不可少

- 数据成员初始化和赋值并不总是相同。如果成员是**const**或者是**引用**，必须将其初始化。类似的，类成员属于**某种类类型且该类型没有定义默认构造函数**时，也必须将这个成员初始化。
- 随着构造函数体一开始执行，初始化就完成了。我们初始化const或者引用类型的数据成员的唯一计划就是通过构造函数初始值。

##### 成员初始化的顺序

- 构造函数初始值列表只说明用于初始化成员的值，而不限定初始化的具体执行顺序。一般来说，初始化的顺序没什么特别要求，不过如果一个成员是用另一个成员来初始化的，那么这两个成员的初始化顺序就很关键了。

```CPP
class X
{
    int i;
    int j;
public:
    // 未定义的：i在j之前被初始化
    x(int val) : j(val), i(j) { }
}
```
- 上述构造函数初始值的形式上来看仿佛是先用val初始化了j，然后再用j初始化i。实际上，i先被初始化，因此会用未定义的值j初始化i。
- 最好令构造函数初始值的顺序与成员声明的顺序保持一致。并且尽量避免使用某些成员初始化其他成员。


##### 默认实参和构造函数

- 如果一个构造函数为所有参数都提供了默认参数，则它实际上也定义了默认构造函数。因为即使不提供实参，也能调用该构造函数。
```CPP
class Sales_data
{
public:
    // 定义默认构造函数，使其与只接受一个string实参的构造函数功能相同
    Sales_data(std::string s = "") : bookNo(s) { }
    // 等价于：
    Sales_data() = default;
    Sales_data(std::string s) : bookNo(s) { }
}
```

### 委托构造函数

- C++11拓展了构造函数初始值的功能，使得我们可以定义所谓的**委托构造函数**（delegating constructor）。一个委托构造函数使用它所属类的其他构造函数执行它自己的初始化过程，或者说它自己的一些（或者全部）职责委托给了其他构造函数。
- 委托构造函数的成员初始值列表只有唯一一个入口，就是类名本身。参数列表必须与类中另外一个构造函数匹配。
```CPP
class Sales_data
{
public:
    // 非委托构造函数使用对应的实参初始化成员
    Sales_data(std::string s, unsigned cnt, double price) ;
        bookNo(s), units_sold(cnt), revenue(cnt * price) {}
    Sales_data() : Sales_data("", 0, 0) {}
    Sales_data(std::string s) : Sales_data(s, 0, 0) {}
    Sales_data(std::istream &is) : Sales_data() { read(is, *this); }
    ///......
};
```
- 当一个构造函数委托给另一个构造函数时，受委托的构造函数的初始值列表和函数体被依次执行，然后执行委托构造函数本身的函数体。

### 默认构造函数的作用

- 当对象被默认初始化或值初始化时自动执行默认构造函数。
- 默认初始化：
    - 在块作用域内不使用任何初始值定义一个非静态变量或数组时。
    - 当一个类本身含有类类型的成员且使用合成的默认构造函数时。
    - 当类类型的成员没有在构造函数初始值列表中显式地初始化时。
- 值初始化：
    - 在数组初始化时提供的初始值数量小于数组的大小。
    - 不使用初始值定义一个局部静态变量。
    - 通过书写形如T()的表达式显式地请求值初始化时，其中T是类型名。（例外：vector的一个构造函数接受一个实参用于说明vector的大小，但仍进行值初始化）。

##### 使用默认构造函数

- 如果想定义一个使用默认构造函数进行初始化的对象，正确的方式是去掉对象名之后的空的括号对。
- 如果加上括号对（错误的做法）则变成声明了一个函数。

```CPP
Sales_data obj();       // 错误：声明了一个函数而非一个对象，该函数无参数且返回Sales_data类型
Sales_data obj2;        // 正确：定义了一个默认初始化的对象
```

### 隐式的类类型转换

- 内置类型之间有自动转换的规则，同样，也能为类定义隐式转换规则。如果构造函数只接受一个实参，则它实际上定义了转换为此类型的隐式转换机制，有时把这种构造函数称作**转换构造函数**（converting constructor）。
- 在Sales_data类中，接受string的构造函数和接受istream的构造函数分别定义了从这两种类型向Sales_data隐式转换的规则。
```CPP
string null_book = "9-999-99999-9";     // 字符串字面值隐式转换为string
item.combine(null_book);                // string隐式转换为Sales_data
```

##### 只允许一步类类型转化

- 编译器只会自动地执行一步类型转换。
```CPP
item.combine("9-999-99999-9");              // 错误：进行了两步隐式转换
item.combine(string("9-999-99999-9"));      // 正确：显式转换为string，隐式转换为Sales_data
item.combine(Sales_data("9-999-99999-9"));  // 正确：隐式转换为string，显式转换为Sales_data
```
- 实际上，我们创建了一个临时的Sales_data对象，一旦combine完成我们就不能再访问它。实际上，我们构建了一个对象，先将它的值加到item中，随后将其丢弃。

##### 抑制构造函数定义的隐式转换

- 在要求隐式转换的程序上下文中，我们可以通过将构造函数声明为explicit加以阻止：
```CPP
class Sales_data
{
public:
    // ......
    explicit Sales_data(const std::string &s) : bookNo(s) {}
    explicit Sales_data(std::istream&);
    // ......
};
```
- 此时，没有任何构造函数能用于隐式地创建Sales_data对象。
- 关键字explicit只对一个实参的构造函数有效。需要多个实参的构造函数不能用于执行隐式转换，所以无须指定为explicit的。
- 只能在类内声明构造函数时使用explicit关键字，在类外部定义时不应重复：
```CPP
错误：explicit只允许出现在类内的构造函数声明处
explicit Sales_data::Sales_data(istream &is)
{
    read(is, *this);
}
```

##### explicit构造函数只能用于直接初始化

- *补充：直接初始化和拷贝初始化*：使用等号（=）初始化变量，实际上执行的是**拷贝初始化**，右侧的初始值将拷贝到左侧新对象中。如果不使用等号，则执行**直接初始化**。
- 发生隐式转换的一种情况是当我们执行拷贝形式的初始化时（使用=）。此时，我们只能使用直接初始化而不能使用explicit构造函数：
```CPP
Sales_data item1(null_book);        // 正确：直接初始化
Sales_data item2 = null_book;       // 错误：不能将explicit构造函数用于拷贝形式的初始化过程
```
- **总结**：当我们用explicit声明构造函数时，它将只能以**直接初始化**的形式使用。而且，该构造函数**不能进行隐式转换**。

##### 为转换显式地使用构造函数

- 尽管编译器不会将explicit的构造函数用于隐式转换过程，但是我们可以使用这样的构造函数显式地强制进行转换：
```CPP
item.combine(Sales_data(null_book));        // 正确：null_book是string，显式构造Sales_data对象
item.combine(static_cast<Sales_data>(cin)); // 正确：static_cast可以使用explicit的构造函数
```

##### 标准库中含有显式构造函数的类

- 接受一个单参数的const char*的string构造函数不是explicit的。
- 接受一个容量参数的vector的构造函数是explicit的。

### 聚合类

- **聚合类**（aggregate class）使得用户可以**直接访问其成员**，并且具有**特殊的初始化语法**形式。当一个类满足如下条件时，我们说它是聚合的：
    - 所有成员都是public的。
    - 没有定义任何构造函数。
    - 没有类内初始值。
    - 没有基类，也没有virtual函数。
```CPP
struct Data
{
    int ival;
    string s;
};
```
- 我们可以提供一个花括号括起来的成员初始值列表，并且用它初始化聚合类的数据成员，初始值的顺序必须与声明的顺序一致：
```CPP
Data val1 = { 0, "Anna" };
```
- 如果初始值列表元素少于类的成员数量，则靠后的成员被值初始化；不能多于。

### 字面值常量类

- *补充：常量表达式*：常量表达式（const expression）是指值不会改变并且在编译过程就能得到计算结果的表达式。字面值属于常量表达式，用常量表达式初始化的const对象也是常量表达式。
```CPP
const int max_files = 20;           // max_files是常量表达式
const int limit = max_files + 1;    // limit是常量表达式
const int sz = get_size();          // sz不是常量表达式。尽管sz本身是一个常量，但它的具体只知道运行时才能获取到
```
- *补充：constexpr变量*：C++规定，允许将变量声明为constexpr类型以便由编译器来验证变量的值是否是一个常量表达式。声明为constexpr的变量一定是一个常量，而且必须用常量表达式初始化。
```CPP
constexpr int max_files = 20;
```
- *补充：constexpr函数*：constexpr函数是指能用于常量表达式的函数。定义constexpr函数要遵循几项约定：函数的返回类型及所有形参的类型都是字面值类型；函数体中有且只有一条return语句。constexpr函数被隐式地指定为内联函数。constexpr函数体内也可以包含其他不执行任何操作的语句，如空语句、类型别名以及using声明。
```CPP
constexpr int new_sz() { return 42; }
constexpr int foo = new_sz();   // 正确：foo是一个常量表达式
constexpr size_t scale(size_t cnt) { return new_sz() * cnt; }
```
- constexpr函数的参数和返回类型必须是字面值类型。除了算术类型、引用和指针外，某些类也是字面值类型。与其他类不同，**字面值类型的类可以含有constexpr函数成员**。
- **数据成员都是字面值类型的聚合类**是字面值常量类。如果一个类**不是聚合类，但它符合下述要求**，则它也是一个字面值常量类：
    - 数据成员都必须是**字面值类型**。
    - 类必须至少含有一个**constexpr构造函数**。
    - 如果一个数据成员含有类内初始值，则内置类型的初始值必须是一条常量表达式；或者如果成员属于某种类类型，则初始值必须使用成员自己的constexpr构造函数。
    - 类必须使用析构函数的默认定义，该成员负责销毁类的对象。

##### constexpr构造函数

- 尽管构造函数不能是const的，但字面值常量类的构造函数可以是constexpr函数，而且必须至少提供一个constexpr构造函数。constexpr构造函数可以声明成=default的形式（或者是删除函数的形式）。否则constexpr函数体就只能为空（构造函数无return语句）。
```CPP
class Debug
{
public:
    constexpr Debug(bool b = true) : hw(b), io(b), other(b) {}
    constexpr Debug(bool h, bool i, bool 0) :
        hw(h), io(i), other(o) {}
    constexpr bool any() { return hw || io || other; }
    void set_io(bool b) { io = b; }
    void set_hw(bool b) { hw = b; }
    void set_other(bool b) { other = b; }
private:
    bool hw;        // 硬件错误
    bool io;        // io错误
    bool other;     // 其他错误
};
```
- constexpr构造函数必须初始化所有数据成员，初始值或者使用constexpr构造函数，或者是一条常量表达式。constexpr构造函数用于生成constexpr对象以及constexpr函数的参数或者返回类型。
```CPP
constexpr Debug io_sub(false, true, false); // 调试IO
if (io_sub.any())                           // 等价于if(true)
    cerr << "print appropriate error messages" << end;
constexpr Debug prod(false);                // 无调试
if (prod.any())                             // 等价于if(false)
    cerr << "print an error message" << end;
```

## 6. 类的静态成员

- 有时候类需要它的一些成员与类本身直接相关，而不是与类的各个对象保持关联。

##### 声明静态成员

- 通过在成员的声明之前加上关键字static使得其与类关联在一起。和其他成员一样，静态成员可以是public的或private的。
- 举个例子，定义一个表示银行账户记录的类：
```CPP
class Account
{
public:
    void calculate() { amount += amount * interestRate; }
    static void rate() { return interestRate; }
private:
    std::string owner;
    double amount;
    static double interestRate;     // 银行基准利率

    static double initRate();
};
```
- 类的静态成员存在于任何对象之外，对象中不包含任何与**静态数据成员**有关的数据。因此，每个Account对象将包含两个数据成员：owner和amount。interestRate被所有对象共享。
- 而**静态成员函数**不与任何对象绑定在一起，它们**不包含this指针**。所以，静态成员函数不能声明成const的。

##### 使用类的静态成员

- 通过使用作用域运算符直接访问静态成员：
```CPP
double r;
r = Account::rate();
```
- 虽然静态成员不属于类的某个对象，但仍然可以使用类的对象、引用或者指针来访问静态成员：
```CPP
Account ac1;
Account *ac2 = &ac1;
r = ac1.rate();         // 通过Account的对象或引用调用静态成员函数rate
r = ac2->rate();        // 通过指向Account对象的指针
```
- 成员函数不通过作用域运算符就能直接使用静态成员。如上述代码中成员函数calculate直接使用静态数据成员interestRate。

##### 定义静态成员

- 静态函数：和其他成员函数一样，可以在类的内部或外部定义static函数。但static关键字只能出现在类内部的声明语句中：
```CPP
void Account::rate(double newRate)  // 在类外部，不能使用static关键字
{
    interestRate = newRate;
}
```
- 静态数据成员：静态数据成员不属于任何一个对象，所以不是由构造函数初始化的。一般来说，不能在类的内部初始化静态成员，必须在外部定义和初始化每个静态成员。一个静态数据成员只能定义一次。
```CPP
double Account::interestRate = initRate();  // 在类的外部定义并初始化一个静态成员
```
- 类似于全局变量，静态数据成员定义在任何函数之外。因此一旦被定义，就将存在于程序的整个生命周期中。
- 如前所述，从类名开始，语句剩余部分就处在类的作用域之内了。因此可以直接使用initRate私有函数。
- 想要确保对象只定义一次，最好的办法是把静态数据成员的定义与其他非内联函数的定义放在同一个文件中。

##### 静态成员的类内初始化

- 通常情况下，类的静态成员不应该在类的内部初始化。然而，可以为静态成员提供**const整数类型的类内初始值**，不过要求静态成员必须是字面值常量类型的constexpr。初始值必须是常量表达式。
```CPP
class Account
{
public:
    static double rate() { return interestRate; }
    static void rate(double);
private:
    static constexpr int period = 30;   // period是常量表达式
    double daily_tbl[period];
};
```
- 不过，即使一个常量静态数据成员在类内部被初始化了，通常情况下也应该在类的外部定义一下该成员。但是定义不能再指定初始值了。
```CPP
constexpr int Account::period;      // 初始值在类的定义内提供，此处为放在外部的声明
```

##### 静态成员能用于某些场景，而普通成员不能

- 静态成员独立于任何对象，因此，在某些非静态数据成员可能非法的场合，静态成员却可以正常地使用。
- 例如，静态数据成员**可以是不完全类型**。特别地，静态数据成员的类型**可以就是它所属的类类型**。而非静态数据成员则受到限制，只能声明成它所属类的指针或引用。
```CPP
class Bar
{
public:
    // ......
private:
    static Bar mem1;    // 正确：静态成员可以是不完全类型
    Bar \*mem2;         // 正确：指针成员可以是不完全类型
    Bar mem3;           // 错误：数据成员必须是完全类型
};
```

- 静态成员和普通成员的另外一个区别是我们可以使用静态成员**作为默认实参**。非静态数据成员不能作为实参，因为它的值本身属于对象的一部分，这么做的结果是无法真正提供一个对象以便从中获取成员的值，从而引发错误。
```CPP
class Screen
{
public:
    Screen &clear(char = bkground); // bkground表示一个类中稍后定义的静态成员
private:
    static const char bkground;
};
```
