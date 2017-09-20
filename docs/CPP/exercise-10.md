*本练习答案仅作参考，代码中未包含头文件和对应的输入输出文件，命名空间默认为std，书中展示过的函数和类的定义此处不再重复*

#### 10.1

```CPP
int main()
{
    ifstream istrm("./10.1.1_f");
    std::vector<int> vi;
    int num;
    int val;
    while (istrm >> num)
        vi.push_back(num);
    cin >> val;

    cout << count(vi.cbegin(), vi.cend(), val) << endl;

    return 0;
}
```

#### 10.2

```CPP
int main()
{
    ifstream ifstrm("./10.1.2_f");
    list<string> ls;
    string tmpStr;
    string val;

    while (ifstrm >> tmpStr)
        ls.push_back(tmpStr);

    cin >> val;
    cout << count(ls.cbegin(), ls.cend(), val) << endl;

    return 0;
}
```

#### 10.3

```CPP
int main(int argc, char const *argv[])
{
    ifstream ifstrm("10.3_f");
    vector<int> vi;
    int i;
    int sum;

    while (ifstrm >> i)
        vi.push_back(i);

    sum = accumulate(vi.cbegin(), vi.cend(), 0);
    cout << sum << endl;

    return 0;
}
```

#### 10.4

```CPP
int main(int argc, char const *argv[])
{
    ifstream ifstrm("10.3_f");
    vector<double> vd;
    double d;
    double sum;

    while (ifstrm >> d)
        vd.push_back(d);

    /* 错误：因为第三个参数的类型决定了函数使用哪个加法以及返回值的类型，
            所以该语句使用了整型加法，返回整型数。应该用0.0 */
    sum = accumulate(vd.cbegin(), vd.cend(), 0);
    cout << sum << endl;

    return 0;
}
```

#### 10.5

```CPP
int main(int argc, char const *argv[])
{
    vector<const char *> roster1 = {"gray", "smog"};
    list<const char *> roster2 = {"gray", "smog"};

    const char *a = "ab";
    const char *b = "ab";

    const char c[] = "ab";
    const char d[] = "ab";

    cout << boolalpha << equal(roster1.cbegin(), roster1.cend(), roster2.cbegin()) << " "
         << (a == b) << " " << (c == d) << endl;

    // 运行结果：true true flase
    // 用==符号比较两个C风格字符串，实际上比较的是指针而不是字符串本身。需要使用strcmp函数，此时就不是比较指针了。
    // 虽然第一个和第二个结果返回的是true，但实际上因为等号两边都指向同一个地址。
    // 第三个结果是false，数组上的==运算符总是得到false的结果。

    return 0;
}
```

#### 10.6

```CPP
int main()
{
    vector<int> vi = { 1, 2, 3, 4, 5 };
    fill_n(vi.begin(), vi.size(), 0);

    for (const auto &i : vi)
        cout << i << " ";
    cout << endl;

    return 0;
}
```

#### 10.7

```CPP
// (a)
vector<int> vec;
list<int> lst;
int i;
while (cin >> i)
    lst.push_back(i);
// copy(lst.cbegin(), lst.cend(), vec.begin()); 错误，vec中至少要包含和lst一样多的元素。
// 使用插入迭代器可以扩大vec
copy(lst.cbegin(), lst.cend(), back_inserter(vec));

// (b)
vector<int> vec;
vec.reserve(10);
// fill_n(vec.begin(), 10, 0); 错误，reserve函数仅为vec预留10个元素的空间，vec中并无元素
fill_n(back_inserter(vec), 10, 0);
```

#### 10.8

改变容器大小的是插入迭代器back_inserter，算法之后再应用到已经变大的容器中。算法本身并未改变容器大小。

#### 10.9

```CPP
// elimDups函数见书p343
int main()
{
    ifstream ifstrm("10.9_f");
    if (!ifstrm)
    {
        cerr << "No input file." << endl;
        return EXIT_FAILURE;
    }

    string word;
    vector<string> vs;
    while (ifstrm >> word)
    {
        vs.push_back(word);
        cout << word << " ";
    }
    cout << endl;

    elimDups(vs);

    return 0;
}
```

#### 10.10

为了保持通用性，算法通过迭代器直接作用于元素而不是容器。所以不改变容器的大小。

#### 10.11

```CPP
// isShorter函数见书p344
int main()
{
    ifstream ifstrm("10.9_f");
    if (!ifstrm)
    {
        cerr << "No input file." << endl;
        return EXIT_FAILURE;
    }

    string word;
    vector<string> vs;
    while (ifstrm >> word)
    {
        vs.push_back(word);
    }

    elimDups(vs);
    stable_sort(vs.begin(), vs.end(), isShorter);
    for (const auto &s : vs)
        cout << s << " ";
    cout << endl;

    return 0;
}

#### 10.12

```CPP
int main()
{
    vector<Sales_data> vDatas = { Sales_data("123"), Sales_data("asdf"), Sales_data("321"), Sales_data("fdsa") };
    sort(vDatas.begin(), vDatas.end(), compareIsbn);

    for (const auto &d : vDatas)
        cout << d.isbn() << " ";
    cout << endl;

    return 0;
}
```

#### 10.13

```CPP
int main()
{
    std::vector<string> words = { "the", "quick", "red", "fox", "jumps", "over", "the", "slow", "red", "turtle" };
    auto part_end = partition(words.begin(), words.end(), isLongerThanFive);
    for (auto iter = words.cbegin(); iter != part_end; iter++)
        cout << *iter << " ";
    cout << endl;

    return 0;
}
```

#### 10.14

```CPP
int main()
{
    auto f = [](const int a, const int b){ return a + b; };
    cout << f(1, 2) << endl;

    return 0;
}
```

#### 10.15

```CPP
int main()
{
    int a, b;
    cin >> a >> b;

    auto f = [a](const int b){ return a + b; };
    cout << f(b) << endl;

    return 0;
}
```

#### 10.16

```CPP
// biggies函数见书p349
int main()
{
    vector<string> words = { "the", "quick", "red", "fox", "jumps", "over", "the", "slow", "red", "turtle" };

    biggies(words, 5);

    return 0;
}
```

#### 10.17

```CPP
sort(vDatas.begin(), vDatas.end(), [](const Sales_data &s1, const Sales_data &s2){ return s1.isbn() < s2.isbn(); });
```

#### 10.18

```CPP
void biggies(vector<string> &words, vector<string>::size_type sz)
{
    elimDups(words);
    stable_sort(words.begin(), words.end(), [](const string &s1, const string &s2){ return s1.size() < s2.size(); });

    auto part_end = partition(words.begin(), words.end(), [sz](const string &s){ return s.size() >= sz; });
    auto count = part_end - words.begin();
    auto make_plural = [](int count, string s){ return s + ((count > 1) ? "s" : ""); };
    cout << count << " " << make_plural(count, "words") << " of length " << sz << " or longer" << endl;
    for_each(words.begin(), part_end, [](const string &s){ cout << s << " "; });
    cout << endl;
}
```

#### 10.19

```CPP
void biggies(vector<string> &words, vector<string>::size_type sz)
{
    elimDups(words);
    stable_sort(words.begin(), words.end(), [](const string &s1, const string &s2){ return s1.size() < s2.size(); });
    auto big_begin = find_if(words.begin(), words.end(), [sz](const string &s){ return s.size() >= sz; });
    auto count = words.end() - big_begin;
    auto f = [count]() -> string { return string("word") + ((count > 1) ? "s" : ""); };
    cout << count << " " << f() << " of length " << sz << " or longer" << endl;
    for_each(big_begin, words.end(), [](const string &s){ cout << s << " "; });
    cout << endl;
}
```

#### 10.20

```CPP
void biggies(vector<string> &words, vector<string>::size_type sz)
{
    // 排序、删除重复...
    count_if(words.cbegin(), words.cend(), [sz](const string &s) -> bool { return s.size() > 6; });
    // ...
}
```

#### 10.21

```CPP
int main()
{
    int a;
    cin >> a;
    auto f = [&]() -> bool { return (a-- == 0) ? true : false;  };
    cout << boolalpha << f() << endl;
    return 0;
}
```

#### 10.22

```CPP
bool isNotShorter(const string &s, string::size_type sz)
{
    return s.size() >= sz;
}

// 版本一
void biggies(vector<string> &words, vector<string>::size_type sz = 6)
{
    // ...
    auto isNotShorterThanSix = bind(isNotShorter, _1, sz);
    count_if(words.cbegin(), words.cend(), isNotShorterThanSix);
    // ...
}

// 版本二
void biggies2(vector<string> &words, vector<string>::size_type sz)
{
    // ...
    count_if(words.cbegin(), words.cend(), bind(isNotShorter, _1, sz));
}
```

#### 10.23

bind接受第一个函数参数的参数数加1

#### 10.24

```CPP
bool check_size(const string &s, string::size_type sz)
{
    return s.size() < sz;
}

int main()
{
    string str;
    cin >> str;

    vector<int> vi = { 2, 4, 7, 8, 5, 9, 150 };

    auto firstIter = find_if(vi.cbegin(), vi.cend(), bind(check_size, str, _1));
    cout << *firstIter << endl;

    return 0;
}
```

#### 10.25

```CPP
bool check_size(const string s, string::size_type sz)
{
    return s.size() >= sz;
}

void biggies(vector<string> &words, vector<string>::size_type sz)
{
    elimDups(words);
    stable_sort(words.begin(), words.end(), [](const string &s1, const string &s2){ return s1.size() < s2.size(); });

    auto part_end = partition(words.begin(), words.end(), bind(check_size, _1, sz));

    auto count = part_end - words.begin();
    auto make_plural = [](int count, string s){ return s + ((count > 1) ? "s" : ""); };
    cout << count << " " << make_plural(count, "words") << " of length " << sz << " or longer" << endl;
    for_each(words.begin(), part_end, [](const string &s){ cout << s << " "; });
    cout << endl;
}
```

#### 10.26

back_inserter创建一个使用push_back的迭代器  
front_inserter创建一个使用push_front的迭代器  
inserter创建一个使用insert的迭代器。此函数接受第二个参数，这个参数必须是一个指向给定容器的迭代器。元素将被插入到给定迭代器所表示的元素之前。返回指向原来的元素的迭代器。

#### 10.27

```CPP
int main()
{
    vector<int> vi1 = { 1, 2, 3, 3, 2, 1, 4, 5, 4 };
    vector<int> vi2;

    sort(vi1.begin(), vi1.end());
    unique_copy(vi1.begin(), vi1.end(), back_inserter(vi2));
    for_each(vi2.begin(), vi2.end(), [](int &i){ cout << i << " "; });
    cout << endl;

    return 0;
}
```

### 10.28

```CPP
void printVec(vector<int> vi)
{
    for_each(vi.begin(), vi.end(), [](const int &i){ cout << i << " "; });
    cout << endl;
}

void printList(list<int> lst)
{
    for_each(lst.begin(), lst.end(), [](const int &i){ cout << i << " "; });
    cout << endl;
}

int main()
{
    vector<int> vi1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    vector<int> vi2, vi3, vi4;
    list<int> lst;

    // vi2 == { 1, 2, 3, 4, 5, 6, 7, 8, 9 }
    copy(vi1.begin(), vi1.end(), inserter(vi2, vi2.begin()));
    printVec(vi2);
    // vi3 == { 1, 2, 3, 4, 5, 6, 7, 8, 9 }
    copy(vi1.begin(), vi1.end(), inserter(vi3, vi3.end()));
    printVec(vi3);
    // vi4 == { 1, 2, 3, 4, 5, 6, 7, 8, 9 }
    copy(vi1.begin(), vi1.end(), back_inserter(vi4));
    printVec(vi4);
    // lst == { 9, 8, 7, 6, 5, 4, 3, 2, 1 }
    copy(vi1.begin(), vi1.end(), front_inserter(lst));
    printList(lst);

    return 0;
}
```

#### 10.29

```CPP
int main()
{
    ifstream infile("10.29_f");
    istream_iterator<string> file_iter(infile), eof;
    ostream_iterator<string> out_iter(cout, " ");
    vector<string> contents(file_iter, eof);
    for (auto c : contents)
        *out_iter++ = c;
    cout << endl;

    return 0;
}
```

#### 10.30

```CPP
int main()
{
    istream_iterator<int> intIter(cin), eof;
    ostream_iterator<int> outIter(cout, " ");
    vector<int> vi;
    copy(intIter, eof, back_inserter(vi));
    sort(vi.begin(), vi.end());
    for (const auto i : vi)
        *outIter++ = i;

    return 0;
}
```

#### 10.31

```CPP
int main()
{
    istream_iterator<int> intIter(cin), eof;
    ostream_iterator<int> outIter(cout, " ");
    vector<int> vi;
    copy(intIter, eof, back_inserter(vi));
    sort(vi.begin(), vi.end());
    unique_copy(vi.begin(), vi.end(), outIter);

    return 0;
}
```

#### 10.32

```CPP
int main()
{
    ifstream inFile("10.32_f");
    istream_iterator<Sales_data> dataIter(inFile), eof;
    ostream_iterator<Sales_data> outIter(cout, "\n");
    vector<Sales_data> vDatas(dataIter, eof);
    vector<Sales_data> dataSums;

    sort(vDatas.begin(), vDatas.end(), [](const Sales_data &s1, const Sales_data &s2){ return s1.isbn() < s2.isbn(); });

    auto dataEqual = [](const Sales_data &s, string aIsbn){ return s.isbn() == aIsbn; };
    vector<Sales_data>::reverse_iterator endIter;
    for (auto beginIter = vDatas.begin(); beginIter != vDatas.end(); beginIter = endIter.base())
    {
        endIter = find_if(vDatas.rbegin(), vDatas.rend(), bind(dataEqual, _1, beginIter->isbn()));
        dataSums.push_back(accumulate(beginIter, endIter.base(), Sales_data(beginIter->isbn()), add));
    }

    copy(dataSums.begin(), dataSums.end(), outIter);

    return 0;
}
```

#### 10.33

```CPP
int main()
{
    ifstream inFile("10.33_f1");
    ofstream outFile1("10.33_f2");
    ofstream outFile2("10.33_f3");

    istream_iterator<int> inIter(inFile), eof;
    ostream_iterator<int> oddIter(outFile1, " "), evenIter(outFile2, "\n");

    for_each(inIter, eof, [&](const int &a){ (a % 2) ? (*oddIter++ = a) : (*evenIter++ = a); });

    return 0;
}
```

#### 10.34

```CPP
int main()
{
    vector<int> vi = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    ostream_iterator<int> outIter(cout, " ");
    copy(vi.rbegin(), vi.rend(), outIter);
    cout << endl;

    return 0;
}
```

#### 10.35

```CPP
int main()
{
    vector<int> vi = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    auto iter = vi.end();
    do
    {
        cout << *--iter << " ";
    }
    while (iter != vi.begin());
    cout << endl;

    return 0;
}
```

#### 10.36

```CPP
int main()
{
    list<int> li = { 1, 0, 23, 0, 5, 9, 76, 0, 2 };
    if (find(li.rbegin(), li.rend(), 0) != li.crend())
        cout << "Found last zero." << endl;

    return 0;
}
```

#### 10.37

```CPP
int main()
{
    vector<string> vs = { "abc", "sd", "f3e", "wang", "yu", "chao", "li", "yuxuan", "32d", "4" };
    list<string> rev_list;

    copy(vs.rbegin() + 2, vs.rend() - 3, back_inserter(rev_list));
    for_each(rev_list.begin(), rev_list.end(), [](const string &s){ cout << s << " "; });
    cout << endl;

    return 0;
}
```
