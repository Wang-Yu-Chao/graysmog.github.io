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
类的基本思想是数据抽象（data abstraction）和封装（encapsulation）。数据抽象是一种依赖于**接口**（interface）和**实现**（implementation）分离的编程（以及设计）技术。类的接口包括用户能执行的**操作**（成员函数和相关的非成员函数）；类的实现则包括类的**数据成员**、负责接口实现的**函数体**以及定义类所需的各种**私有函数**。

## 1. 定义抽象数据类型

### 设计Sales_data类

设计的Sales_data类的作用是表示一本书的IBSN书号、总销售额（revenue）、售出册数（units_sold）和平均售价（average price）。  
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

##### this

- 对象调用成员函数，而成员函数若想要访问调用它的那个对象，就要通过this这个隐式参数来访问。


- 在成员函数内部，可以直接使用对象的数据成员（如`return bookNo`），而不用通过成员访问运算符（.）（如`return object.bookNo`）来做到这一点。因为this指向了这个对象，在成员函数中是通过this隐式地引用了类成员。
- 我们可以在成员函数体内部使用this，虽然没有必要，但是isbn函数可以写成如下形式：
```CPP
std::string isbn() const { return this->bookNo };   // this指向对象的指针，所以用->运算符
```
- 因为this的目的总是指向“这个”对象（每个对象的this都是固定地指向自己），所以this是一个**常量指针**，不允许改变this中保存的地址。

##### const成员函数

- isbn函数的另一个关键之处是紧随参数列表之后的const关键字，这里，const的作用是修改隐式this指针的类型。
- 默认情况下，this的类型是**指向非常量的类类型**的**常量指针**。例如在Sales_data成员函数中，this的类型是Sales_data \*const。这意味着（在默认情况下）我们不能把this从一个非常量绑定到一个常量对象上。这一情况就使得我们不能在一个常量对象上调用普通的成员函数。
- C++语言的做事发允许把const关键字放在成员




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

### 定义类相关的非成员函数

### 构造函数

### 拷贝、赋值和析构

## 2. 访问控制与封装

### 友元

## 3. 类的其他特性

### 类成员再探

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
