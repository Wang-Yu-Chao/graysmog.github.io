# 关联容器

#### 框架

1. 使用关联容器
2. 关联容器概述
    - 定义关联容器
    - 关键字类型的要求
    - pair类型
3. 关联容器操作
    - 关联容器迭代器
    - 添加元素
    - 删除元素
    - map的下标操作
    - 访问元素
    - 一个单词转换的map
4. 无序容器

## 使用关联容器

## 关联容器概述

### 定义关联容器

### 关键字类型的要求

### pair类型

## 关联容器操作

### 关联容器迭代器

### 添加元素

### 删除元素

### map的下标操作

### 访问元素

### 一个单词转换的map

##### 单词转换程序

```CPP
void word_transform(ifstream &map_file, ifstream &input)
{
    auto trans_map = buildMap(map_file);
    string text;
    while (getline(input, text))
    {
        istringstream stream(text);
        string word;
        bool firstword = true;
        while (stream >> word)
        {
            if (firstword)
                firstword = false;
            else
                cout << " ";
            cout << transform(word, trans_map);
        }
        cout << endl;
    }
}
```

##### 建立转换映射

```CPP
map<string, string> buildMap(ifstream &map_file)

    map<string, string> trans_map;
    string key;
    string value;
    while (map_file >> key && getline(map_file, value))
    {
        if (value.size() > 1)
            trans_map[key] = value.substr(1);
        else
            throw runtime_error("no rule for " + key);
    }
    return trans_map;
}
```

##### 生成转换文本

```CPP
const string& transform(const string &s, const map<string, string> &m)
{
    auto map_it = m.find(s);
    if (map_it != m.cend())
        return map_it->second;
    else
        return s;
}

## 无序容器
