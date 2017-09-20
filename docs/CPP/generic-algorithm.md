# 泛型算法

##### 框架

1. 概述
2. 初识泛型算法
    - 只读算法
    - 写容器元素的算法
    - 重排容器元素的算法
3. 定制操作
    - 向算法传递函数
    - lambda表达式
    - lambda捕获和返回
    - 参数绑定
4. 再探迭代器
    - 插入迭代器
    - iostream迭代器
    - 反向迭代器
5. 泛型算法结构
    - 5类迭代器
    - 算法形参模式
    - 算法命名规范
6. 特定容器算法

## 1. 概述

## 2. 初识泛型算法

### 只读算法

### 写容器元素的算法

### 重排容器元素的算法

## 3. 定制操作

### 向算法传递函数

### lambda表达式

##### 完整的biggies

```CPP
void biggies(vector<string> &words, vector<string>::size_type sz)
{
    elimDups(words);
    stable_sort(words.begin(), words.end(),
                [](const string &a, const string &b) { return a.size() < b.size(); });
    auto wc = find_if(words.begin(), words.end(),
                      [sz](const string &a) { return a.size() >= sz; });
    auto count = words.end() - wc;
    cout << count << " " << make_plural(count, "words", "s") << " of length "
                  << sz << " or longer" << endl;
    for_each(wc, words.end(),
             [](const string &s){ cout << s << " "; });
    cout << endl;
}
```

### lambda捕获和返回

### 参数绑定

## 4. 再探迭代器

### 插入迭代器

### iostream迭代器

##### 使用流迭代器处理类类型

```CPP
istream_iterator<Salse_item> item_iter(cin), eof;
ostream_iterator<Sales_item> out_iter(cout, "\n");
Sales_item sum = *item_iter++;
while (item_iter != eof)
{
    if (item_iter->isbn() == sum.isbn)
        sum += \*item_iter++;
    else
    {
        out_iter = sum;
        sum = \*item_iter++;
    }
}
out_iter = sum;
```
   
### 反向迭代器

## 5. 泛型算法结构

### 5类迭代器

### 算法形参模式

### 算法命名规范

## 6. 特定容器算法
