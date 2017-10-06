*本练习答案仅作参考，代码中未包含头文件和对应的输入输出文件，命名空间默认为std，书中展示过的函数和类的定义此处不再重复*

#### 12.1

b1包含4个元素；b2被销毁，不包含元素。

#### 12.2

```CPP
string& StrBlob::front() const
{
    check(0, "front on empty StrBlob");
    return data->front();
}

string& StrBlob::back() const
{
    check(0, "back on empty StrBlob");
    return data->back();
}
```

#### 12.3

```CPP
不需要。const成员函数不能改变调用它的对象的内容，而push_back和pop_back都会改变数据成员data。
```

#### 12.4

1. i可以等于0，因为当data不为空时，data[0]存在。当data为空时，i>=data->size()条件满足，抛出异常。
2. i小于0时，i>=data->size()条件满足，抛出异常。

#### 12.5

发生隐式转换的一种情况是当执行拷贝初始化时（使用=）。此时，只能使用直接初始化而不能使用explicit构造函数。  

所以，如果编写一个接受initializer_list的explicit的构造函数，12.1中的`StrBlob b2 = { "a", "an", "the" };`
语句将无法实现，必须使用`StrBlob b2({ "a", "an", "the" });`，使得StrBlob的行为不再类似与指针。

#### 12.6

```CPP
vector<int>* getVecPtr();
vector<int>* getElem(vector<int> *viPtr);
void printVec(vector<int> *viPtr);

int main()
{
    auto vip = getVecPtr();
    getElem(vip);
    printVec(vip);
    delete vip;

    return 0;
}

vector<int>* getVecPtr()
{
    vector<int>* viPtr = new vector<int>();
    return viPtr;
}

vector<int>* getElem(vector<int> *viPtr)
{
    int elem;
    while (cin >> elem)
        viPtr->push_back(elem);
    return viPtr;
}

void printVec(vector<int> *viPtr)
{
    for_each(viPtr->cbegin(), viPtr->cend(), [](const int &i){ cout << i << " "; });
    cout << endl;
}
```

#### 12.7

```CPP
shared_ptr<vector<int>> getVecPtr();
shared_ptr<vector<int>> getElem(shared_ptr<vector<int>> viPtr);
void printVec(shared_ptr<vector<int>> viPtr);

int main()
{
    auto vip = getVecPtr();
    getElem(vip);
    printVec(vip);

    return 0;
}

shared_ptr<vector<int>> getVecPtr()
{
    shared_ptr<vector<int>> viPtr = make_shared<vector<int>>();
    return viPtr;
}

shared_ptr<vector<int>> getElem(shared_ptr<vector<int>> viPtr)
{
    int elem;
    while (cin >> elem)
        viPtr->push_back(elem);
    return viPtr;
}

void printVec(shared_ptr<vector<int>> viPtr)
{
    for_each(viPtr->cbegin(), viPtr->cend(), [](const int &i){ cout << i << " "; });
    cout << endl;
}
```

#### 12.8

有错误。函数的返回值类型和实际的返回值类型不匹配。且非空指针不为0，函数b总是返回true。

#### 12.9

指针r被拷贝为指针q的值，r原来指向的地址没有被释放，造成内存泄漏。应该在赋值前添加：`delete r`  
在重新赋值后，r2的引用计数为0，自动释放分配的内存。

#### 12.10

正确。此调用先创建了一个临时的shared_ptr并用p直接初始化，然后将这个作为实参的临时的shared_ptr拷贝到局部变量ptr中。  
临时的shared_ptr被销毁。process结束后，ptr被销毁。p指向的内存不会被销毁。
