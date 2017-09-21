# 练习答案

## 第15章：动态规划

#### 15.4-1

```CPP
enum Dir { Up_Left, Up, Left, Default };

// 自上而下带备忘版本
// 书中序列从1到n，0代表空序列。实际代码中序列从0到size-1，所以-1代表空序列。
int LCSLength(vector<int> &x, vector<int> &y, vector<vector<int>> &LenTbl, vector<vector<Dir>> &DirTbl, int i, int j)
{
    int rec;
    // LenTbl[-1][-1]不存在，所以要注意判断顺序
    if (i == -1 || j == -1)
    {
        return 0;
    }
    if ((rec = LenTbl[i][j]) > INT_MIN)
        return rec;


    if (x[i] == y[j])
    {
        auto mx = LCSLength(x, y, LenTbl, DirTbl, i-1, j-1) + 1;
        LenTbl[i][j] = mx;
        DirTbl[i][j] = Up_Left;
        return mx;
    }
    else
    {
        auto l1 = LCSLength(x, y, LenTbl, DirTbl, i-1, j);
        auto l2 = LCSLength(x, y, LenTbl, DirTbl, i, j-1);
        if (l1 > l2)
        {
            LenTbl[i][j] = l1;
            DirTbl[i][j] = Up;
            return l1;
        }
        else
        {
            LenTbl[i][j] = l2;
            DirTbl[i][j] = Left;
            return l2;
        }
    }
}

void printLCS(vector<vector<Dir>> &DirTbl, vector<int> &x, int i, int j)
{
    if (i == -1 || j == -1)
        return;
    if (DirTbl[i][j] == Up_Left)
    {
        printLCS(DirTbl, x, i-1, j-1);
        cout << x[i] << " ";
    }
    else if (DirTbl[i][j] == Up)
        printLCS(DirTbl, x, i-1, j);
    else
        printLCS(DirTbl, x, i, j-1);
}

int main()
{
    vector<int> a = { 1, 0, 0, 1, 0, 1, 0, 1 };
    vector<int> b = { 0, 1, 0, 1, 1, 0, 1, 1, 0 };
    vector<vector<int>> LenTbl(a.size(), vector<int> (b.size(), INT_MIN));
    vector<vector<Dir>> DirTbl(a.size(), vector<Dir> (b.size(), Default));

    cout << LCSLength(a, b, LenTbl, DirTbl, a.size()-1, b.size()-1) << endl;
    printLCS(DirTbl, a, a.size()-1, b.size()-1);
    cout << endl;

    return 0;
}
```

#### 15.4-2

*伪代码*
```
PRINT-LCS(c, X, i, j)
if i == 0 or j == 0
    return
if c[i-1, j-1] + 1 == c[i, j]
    PRINT-LCS(c, X, i-1, j-1)
    print X[i]
else if c[i-1, j] > c[i, j-1]
    PRINT-LCS(c, X, i-1, j)
else
    PRINT-LCS(c, X, i, j-1)
```

#### 15.4-3

见练习15.4-1，使用了带备忘的版本。

#### 15.4-4

```CPP
// 只使用2 * small.size()个表项
int LCSLength(vector<int> &small, vector<int> &large, vector<vector<int>> &lenTbl)
{
    lenTbl[0][0] = lenTbl[1][0] = 0;
    for (vector<int>::size_type i = 1; i <= large.size(); i++)
    {
        for (vector<int>::size_type j = 1; j <= small.size(); j++)
        {
            // 表格到序列的映射：减1
            if (large[i-1] == small[j-1])
                lenTbl[1][j] = lenTbl[0][j-1] + 1;
            else
                lenTbl[1][j] = max(lenTbl[1][j-1], lenTbl[0][j]);
        }
        copy(lenTbl[1].begin(), lenTbl[1].end(), lenTbl[0].begin());
    }

    return lenTbl[1][small.size()];
}

// 只使用small.size()个表项
int LCSLength(vector<int> &small, vector<int> &large, vector<int> &lenTbl)
{
    for (auto &t : lenTbl)
        t = 0;

    // i方向的顺序更新
    for (vector<int>::size_type i = 1; i <= large.size(); i++)
    {
        // j方向的逆序更新
        for (vector<int>::size_type j = small.size(); j >= 1; j--)
        {
            // 注意1~n到0~(n-1)的映射
            if (small[j-1] == large[i-1])
                lenTbl[j] = lenTbl[j-1] + 1;
            else
                lenTbl[j] = max(lenTbl[j], lenTbl[j-1]);
        }
    }

    return lenTbl[small.size()];
}

int main()
{
    // a和b从0到size-1
    vector<int> a = { 1, 0, 0, 1, 0, 1, 0, 1 };
    vector<int> b = { 0, 1, 0, 1, 1, 0, 1, 1, 0 };

    // lenTbl从0到minSize
    vector<vector<int>> lenTbl(2, vector<int> (min(a.size(), b.size()) + 1, 0));
    int ans;
    if (a.size() < b.size())
        ans = LCSLength(a, b, lenTbl);
    else
        ans = LCSLength(b, a, lenTbl);

    cout << "LCSLength_1: " << ans << endl;

    vector<int> lenTbl2(min(a.size(), b.size()) + 1, 0);
    cout << "LCSLength_2: " << LCSLength(a, b, lenTbl2) << endl;

    return 0;
}
```

#### 15.4-5

```CPP
// 求最长单调递增子序列的算法
// O(n^2)时间的算法
void LIS(vector<int> &seq, vector<int> &lenTbl, vector<int> &dirTbl)
{
    auto n = seq.size();
    for (decltype(n) i = 0; i < n; i++)
    {
        lenTbl[i] = 1;
        dirTbl[i] = -1;
        for (decltype(n) j = 0; j < i; j++)
            if ((seq[j] < seq[i]) && (lenTbl[i] < lenTbl[j] + 1))
            {
                lenTbl[i] = lenTbl[j] + 1;
                dirTbl[i] = j;
            }
    }
}

void printLIS(vector<int> &seq, vector<int> &lenTbl, vector<int> &dirTbl)
{
    int max = 0;
    vector<int>::size_type maxIndex;
    deque<int> subseq;

    for (decltype(maxIndex) i = 0; i < lenTbl.size(); i++)
        if (max < lenTbl[i])
        {
            max = lenTbl[i];
            maxIndex = i;
        }
    cout << "The length of longest increase subsequence is: " << max << endl;

    for (int i = maxIndex; i != -1; i = dirTbl[i])
        subseq.push_front(seq[i]);
    cout << "And it is: ";
    for (auto const val : subseq)
        cout << val << " ";
    cout << endl;
}

int main()
{
    vector<int> a = { 1, 2, 4, 7, 6, 3, 4 };
    vector<int> lenTbl(a.size());
    vector<int> dirTbl(a.size());

    LIS(a, lenTbl, dirTbl);
    printLIS(a, lenTbl, dirTbl);

    return 0;
}
```

#### 15.4-6

算法暂未想出

#### 15.5-1

*伪代码*
```
CONSTRUCT-OPTIMAL-BST(root)
n = root.size
r = root[1, n]
print k[r]为根
if r == 1
    print d[0]为k[r]的左孩子
else
    print k[root[1, r-1]]为k[r]的左孩子
    CONSTRUCT-RECURSIVE(root, 1, r-1)
if r == n
    print d[n]为k[r]的右孩子
else
    print k[root[r+1, n]]为k[r]的右孩子
    CONSTRUCT-RECURSIVE(root, r+1, n)

CONSTRUCT-RECURSIVE(root, left, right)
r = root[left, right]
if r == left
    print d[r-1]为k[r]的左孩子
else
    print k[root[left, r-1]]为k[r]的左孩子
    CONSTRUCT-RECURSIVE(root, left, r-1)
if r == right
    print d[r]为k[r]的右孩子
else
    print k[root[r+1, right]]为k[r]的右孩子
    CONSTRUCT-RECURSIVE(root, r+1, right)
```

*C++实现*
```CPP
typedef vector<vector<double>> twoDimDbl;
typedef vector<vector<size_t>> twoDimSzt;

pair<twoDimDbl, twoDimSzt> optimalBST(vector<double> &p, vector<double> &q, size_t n);
vector<string> constructOBST(twoDimSzt &root);
void constructRecurive(vector<string> &info, twoDimSzt &root, size_t left, size_t right);

int main()
{
    // 数组p从1开始，索引0无效
    vector<double> p = { 0.0, 0.15, 0.10, 0.05, 0.10, 0.20 };
    // 数组q从0开始，索引0代表q0
    vector<double> q = { 0.05, 0.10, 0.05, 0.05, 0.05, 0.10 };
    auto n = p.size()-1;
    auto e_and_root = optimalBST(p, q, n);
    auto e = e_and_root.first;
    auto root = e_and_root.second;

    cout << "最优二叉搜索树\n" << endl;
    cout << "期望代价为: " << e[1][n] << endl;
    auto info = constructOBST(root);
    cout << "\n结构为:" << endl;
    for_each(info.cbegin(), info.cend(), [](const string &s){ cout << s << endl; });

    return 0;
}

pair<twoDimDbl, twoDimSzt> optimalBST(vector<double> &p, vector<double> &q, size_t n)
{
    // e[1..n+1, 0..n], w[1..n+1, 0..n], root[1..n, 1..n]
    twoDimDbl e(n+2, vector<double>(n+1, DBL_MAX)), w(n+2, vector<double>(n+1, 0.0));
    twoDimSzt root(n+1, vector<size_t>(n+1, 0));
    for (size_t i = 1; i <= n+1; i++)
    {
        e[i][i-1] = q[i-1];
        w[i][i-1] = q[i-1];
    }
    for (size_t l = 1; l <= n; l++)
        for (size_t i = 1; i <= n - l + 1; i++)
        {
            size_t j = i + l - 1;
            w[i][j] = w[i][j-1] + p[j] + q[j];
            for (size_t r = i; r <= j; r++)
            {
                auto min_e = e[i][r-1] + e[r+1][j] + w[i][j];
                if (min_e < e[i][j])
                {
                    e[i][j] = min_e;
                    root[i][j] = r;
                }
            }
        }
    return {e, root};
}

vector<string> constructOBST(twoDimSzt &root)
{
    vector<string> info;
    auto n = root.size() - 1;
    auto r = root[1][n];
    auto r_str = to_string(r);
    info.push_back(string("k") + r_str + "为根");
    if (r == 1)
        info.push_back(string("d0为k") + r_str + "的左孩子");
    else
    {
        info.push_back(string("k") + to_string(root[1][r-1]) + "为k" + r_str + "的左孩子");
        constructRecurive(info, root, 1, r-1);
    }
    if (r == n)
        info.push_back(string("dn为k") + r_str + "的右孩子");
    else
    {
        info.push_back(string("k") + to_string(root[r+1][n]) + "为k" + r_str + "的右孩子");
        constructRecurive(info, root, r+1, n);
    }

    return info;
}

void constructRecurive(vector<string> &info, twoDimSzt &root, size_t left, size_t right)
{
    auto r = root[left][right];
    auto r_str = to_string(r);
    if (r == left)
        info.push_back(string("d") + to_string(r-1) + "为k" + r_str + "的左孩子");
    else
    {
        info.push_back(string("k") + to_string(root[left][r-1]) + "为k" + r_str + "的左孩子");
        constructRecurive(info, root, left, r-1);
    }
    if (r == right)
        info.push_back(string("d") + to_string(r) + "为k" + r_str + "的右孩子");
    else
    {
        info.push_back(string("k") + to_string(root[r+1][right]) + "为k" + r_str + "的右孩子");
        constructRecurive(info, root, r+1, right);
    }
}
```

#### 15.5-2

将数据代入上一题代码中，得到：
```
最优二叉搜索树

期望代价为: 3.12

结构为:
k5为根
k2为k5的左孩子
k1为k2的左孩子
d0为k1的左孩子
d1为k1的右孩子
k3为k2的右孩子
d2为k3的左孩子
k4为k3的右孩子
d3为k4的左孩子
d4为k4的右孩子
k7为k5的右孩子
k6为k7的左孩子
d5为k6的左孩子
d6为k6的右孩子
d7为k7的右孩子
```

#### 15.5-3

对渐进时间的复杂性无影响，OPTIMAL-BST的时间复杂度依然是Θ(n<sup>3</sup>)。
```
......
for l=1 to n
    for i = 1 to n-l+1
        j = i+l-1
        e[i, j] = ∞
        w[i, j] = q[i-1]
        for m = i to j
            w[i, j] = w[i, j] + p[m] + q[m]
......
```

#### 15.5-4

*最优二叉树伪代码——Θ(n<sup>2</sup>)*
```
OPTIMAL-BST(p, q, n)
let e[1..n+1 0..n], w[1..n+1, 0..n], and root[1..n, 1..n] be new tables
for i = 1 to n+1
    e[i, i-1] = q[i-1]
    w[i, i-1] = q[i-1]
for l = 1 to n
    for i = 1 to n-l+1
        j = i+l-1
        e[i, j] = ∞
        w[i, j] = w[i, j-1] + p[j] + q[j]
        for r = root[i, j-1] to root[i+1, j]
            t = e[i, r-1] + e[r, j] + w[i, j]
            if t < e[i, j]
                e[i, j] = t
                root[i, j] = r
return e and root
```
证明方法暂未想出
