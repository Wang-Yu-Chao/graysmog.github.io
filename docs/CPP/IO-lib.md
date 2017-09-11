# IO库

## 1. IO类

部分已知的IO库设施 | 特点
:---- | :------:
cin  |  一个istream对象，从标准输入读取操作
cout  |  一个ostream对象，向标准输出写入数据
cerr  |  一个ostream对象，通常用于输出程序错误消息，写入到标准错误
\>\>运算符  | 用来从一个istream对象读取输入数据
<<运算符  |  用来向一个ostream对象写入输出数据
getline函数  |  从一个给定的istream读取一行数据

IO库头文件 | 类型
:----: | :-----:   
iostream | istream，wistream从流中读取数据<br>ostream，wostream向流中写入数据<br>iostream，wiostream读写流
fstream  | ifstream，wifstream从文件读取数据<br>ofstream，wofstream向文件写入数据<br>fstream，wfstream读写文件
sstream  | istringstream，wistringstream从string读取数据<br>ostringstream，wostringstream向string写入数据<br>stringstream，wstringstream读写string    

- 为了支持使用宽字符的语言，标准库定义了一组类型和对象来操纵wchar_t类型的数据。宽字符版本的类型和函数名字以一个w开始。例如，wcin、wcout和wcerr分别对应cin、cout和cerr的宽字符版对象。宽字符版本的类型和对象与其对应的普通char版本的类型定义在同一个头文件中。

##### IO类型间的关系

- 概念上，设备类型和字符大小都不会影响我们要执行的IO操作。可以用>>读取数据，不管是从控制台窗口、磁盘文件还是一个string读取。类似的，也不用管读取的字符能存入一个char对象内，还是需要一个wchar_t对象来存储。
- 标准库通过继承机制（inheritance）使我们能忽略这些不同类型的流之间的差异。利用模版，我们可以使用具有继承关系的类，而不必了解继承机制如何工作的细节。
- 剩下部分所介绍的标准库流特性都可以无差别地应用于普通流、文件流和string流，以及char或宽字符版本（如ifstream和istringstream都继承自istream）。

### IO对象无拷贝或赋值

- 不能拷贝或对IO对象赋值。由于不能拷贝IO对象，因此不能将形参或返回类型设置为流类型。进行IO操作的函数通常是以引用方式传递和返回流
```CPP
ofstream out1, out2;
out1 = out2;                // 错误：不能对流对象赋值
ofstream print(ofstream);   // 错误：不能初始化ofstream参数
out2 = print(out2);         // 错误：不能拷贝流对象
```
- 读写一个IO对象会改变其状态，因此传递和返回的引用不能是const的。

### 条件状态

- IO操作一个与生俱来的问题是可能发生错误。一些错误是可恢复的，而其他错误则发生在系统深处，已经超出了应用程序可以修正的范围。
- 下表列出了IO类所定义的一些**函数和标志**，可以帮助我们访问和操纵流的**条件状态**（condition state）。

IO库条件状态 | 特点
:------ | :------:
*strm*::iostate  | *strm*是一种IO类型。iostate是一种机器相关的类型，提供了表达条件状态的完整功能
*strm*::badbit  |  用来指出流已崩溃
*strm*::failbit  |  用来指出一个IO操作失败了
*strm*::eofbit  |  用来指出流到达了文件结束
*strm*::goodbit  |  用来指出流未处于错误状态。此值保证为零
s.eof()  |  若流s的eofbit置位，则返回true
s.fail()  |  若流s的failbit或badbit置位，则返回true
s.bad()  |  若流s的badbit置位，则返回true
s.good()  |  若流s处于有效状态，则返回true
s.clear()  |  将流s中所有条件状态位复位，将流的状态设置为有效。返回void
s.clear(flags)  |  根据给定的flags标志位，将流s中对应条件状态位复位。flags的类型为*strm*::iostate。返回void
s.setstate(flags)  |  根据给定的flags标志位，将流s中对应条件状态位复位。flags的类型为*strm*::iostate。返回void

- 下面是一个IO错误的例子。输入运算符期待读取一个int，如果此时在标准输入上键入字符，读操作就会失败，cin进入错误状态。
```CPP
int ival;
cin >> ival;
```
- 一个流一旦发生错误，其上后续的IO操作都会失败。只能向一个处于无错状态的流读写数据。
- 因为流可能处于错误状态，代码通常要在使用流之前检查它是否处于良好状态。确定一个流对象的状态的最简单的方法是将它当作一个条件来使用：
```CPP
while (cin >> word)
    // ok：读操作成功...
```
- while循环检查>>表达式返回的流的状态。如果输入操作成功，流保持有效状态，则条件为真。

##### 查询流的状态

- 将流作为条件使用，只能告诉我们流是否有效，而无法告诉我们具体发生了什么。有时我们也需要知道流为什么失败。
- IO库定义了一个与机器无关的iostate类型，它提供了表达流状态的完整功能。这个类型应作为一个位集合来使用。IO库定义了**4个iostate类型的constexpr值**，表示特定的位模式。这些值用来表示特定类型的IO条件，可以与位运算符一起使用来一次性检测或设置多个标志位。
    1. badbit表示系统级错误，如不可恢复的读写错误。通常情况下，一旦badbit被置位，流就无法再使用了。
    2. 在发生可恢复错误后，failbit被置位，如期望读取数值却读出一个字符等错误。这种问题通常是可以修正的，流还可以继续使用。
    3. 如果到达文件结束为止，eofbit和failbit都会被置位。
    4. goodbit的值为0，表示流未发生错误。如果badbit、failbit和eofbit任一个被置位，则检测流状态的条件会失败。
- 标准库还定义了一组函数来查询这些标志位的状态。操作good、bad、fail和eof在对应位被置位时返回true。此外，在badbit被置位时，fail也会返回true。这意味着，good和fail是确定流的总体状态的正确方法。实际上，将流当作条件使用的代码就等价于`!fail()`。而eof和bad操作只能表示特定的错误。

##### 管理条件状态

- 流对象的rdstate成员返回一个iostate，对应流的当前状态。setstate操作将给定条件位置位，表示发生了对应错误。clear成员是一个重载的成员，见上述表格。
```CPP
auto old_state = cin.rdstate();     // 记住cin的当前状态
cin.clear();                        // 使cin有效
process_input(cin);                 // 使用cin
cin.setstate(old_state);            // 将cin置为原有状态
```
- 带参数的clear版本接受一个iostate值，表示流的新状态。为了复位单一的条件状态位，我们首先用rdstate读出当前条件状态，然后用位操作将所需位复位来生成新的状态。例如，下述代码将failbit和badbit复位，但保持eofbit不变。
```CPP
cin.clear(cin.rdstate() & ~cin.failbit & ~cin.badbit);
```

### 管理缓冲输入

- 每个输出流都管理一个缓冲区，用来保存程序读写的数据。
- 向输出流写入的数据可能立即打印出来，但也有可能被操作系统保存在缓冲区中，随后打印。有了缓冲机制，操作系统就可以将程序的多个输出操作组合成单一的系统级写操作。由于设备的写操作可能很耗时，允许操作系统将多个输出操作组合为单一的设备写操作可以带来很大的性能提升。
- 导致缓冲刷新（即，数据真正写到输出设备或文件）的原因有很多：
    - 程序正常结束，作为main函数的return操作的一部分，缓冲刷新被执行。
    - 缓冲区满时，需要刷新缓冲，而后新的数据才能继续写入缓冲区。
    - 使用操纵符如endl来显示刷新缓冲区。
    - 在每个输出操作之后，我们可以用操纵符unitbuf设置流的内部状态，来清空缓冲区。默认情况下，对cerr的设置是unitbuf的，因此写到cerr的内容都是立即刷新的。
    - 一个输出流可能被关联到另一个流。当读写被关联的流时，关联到的流的缓冲区会被刷新。例如，默认情况下，cin和cerr都关联到cout。因此，读cin或写cerr都会导致cout的缓冲区被刷新。

##### 刷新输出缓冲区

- 我们已经使用过操纵符endl，它完成换行并刷新缓冲区的工作。IO库中还有两个类似的操纵符：flush和ends。flush刷新缓冲区，但不输出任何额外的字符；ends向缓冲区插入一个空字符，然后刷新缓冲区。
```CPP
cout << "hi!" << flush; // 输出hi，然后刷新缓冲区，不附加任何额外字符（不换行）
cout << "hi!" << ends;  // 输出hi和一个空字符，然后刷新缓冲区
```

##### unitbuf操纵符

- 如果想在每次输出操作后都刷新缓冲区，我们可以使用unitbuf操纵符。它告诉流在接下来的每次写操作之后都进行一次flush操作。而nounibuf操纵符则重置流，使其恢复使用正常的系统管理的缓冲区刷新机制：
```CPP
cout << unitbuf;    // 任何输出都立即刷新，无缓冲
cout << nounitbuf;  // 回到正常的缓冲方式
```
- 如果程序崩溃，输出缓冲区不会被刷新。当调试一个已经崩溃的程序时，需要确认那些被认为已经输出的数据确实已经刷新了。否则，可能将大量时间浪费在追踪代码为什么没有执行上，而实际上代码已经执行了，只是程序崩溃后缓冲区没有被刷新，输出数据被挂起没有打印而已。

##### 关联输入和输出流

- 当一个输入流被关联到一个输出流时，任何试图从输入流读取数据的操作都会先刷新关联的输出流。标准库将cout和cin关联在一起，所以`cin >> ival`将导致cout缓冲区被刷新。
- tie有两个重载版本：一个版本不带参数，返回指向输出流的指针。如果该对象当前关联到一个输出流，则返回指向这个流的指针；如果对象未关联到流，则返回空指针；另一个版本接受一个指向ostream的指针，将自己关联到此ostream。即，`x.tie(&o)`将流x关联到输出流o。可以将输出流关联到输出流。
```CPP
cin.tie(&cout);
ostream *old_tie = cin.tie(nullptr);    // old_tie指向当前关联到cin的流
cin.tie(&cerr);                         // 读取cin会刷新cerr而不是cout
cin.tie(old_tie)
```
- 每个流同时最多关联到一个流，但多个流可以同时关联到同一个ostream。

## 2. 文件输入输出

- 头文件fstream定义了三个类型来支持文件IO：ifstream从一个给定文件读取数据，ofstream向一个给定文件写入数据，以及fstream可以读写给定文件。
- 这些类型提供和cin、cout相同的操作，如<<、>>、getlined等。
- 除了继承自iostream类型的行为之外，fstream中定义的类型还增加了一些新的成员来管理与流关联的文件。

fstream特有的操作 | 特点
:------ | :--------:
*fstream* fstrm;  |  创建一个未绑定的文件流。fstream是三种类型之一
*fstream* fstrm(s);  |  创建一个fstream，并打开名为s的文件。s可以是string或指向C风格字符串的指针。这些构造函数都是explicit的。默认的文件模式依赖于fstream的类型
*fstream* fstrm(s, mode)  |  与前一个构造函数类似，但按指定的mode打开文件
fstrm.open(s)  |  关闭与fstrm绑定的文件。返回void
fstrm.is_open()  |  返回一个bool值，指出与fstrm关联的文件是否成功打开且尚未关闭

### 使用文件流对象



### 文件模式

## 3. string流

### 使用istringstream

### 使用ostringstream
