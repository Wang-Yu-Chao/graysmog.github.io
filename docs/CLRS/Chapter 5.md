# 第五章

## 5.1

### 5.1 - 3

```
UNBIASED-RANDOM()
    if BIASED-RANDOM() == 0
        if BIASED-RANDOM() == 1
            return 0
        else
            if BIASED-RANDOM() == 0
                return 1
            else
                return UNBIASED-RANDOM()
```

两次调用BIASED-RANDOM，输出0、1和1、0的概率都为p(1-p)，即每次调用UNBIASED-RANDOM有2p(1-p)的概率完成该算法；有<a href="http://www.codecogs.com/eqnedit.php?latex=2p^{2}&space;-&space;2p&space;&plus;&space;1" target="_blank"><img src="http://latex.codecogs.com/png.latex?2p^{2}&space;-&space;2p&space;&plus;&space;1" title="2p^{2} - 2p + 1" /></a>的概率未完成。

令<a href="http://www.codecogs.com/eqnedit.php?latex=P_{i}" target="_blank"><img src="http://latex.codecogs.com/png.latex?P_{i}" title="P_{i}" /></a>为第i次调用UNBIASED-RANDOM后未完成的概率

<a href="http://www.codecogs.com/eqnedit.php?latex=P_{i}&space;=&space;(2p^{2}&space;-&space;2p&space;&plus;&space;1)^{i}" target="_blank"><img src="http://latex.codecogs.com/gif.latex?P_{i}&space;=&space;(2p^{2}&space;-&space;2p&space;&plus;&space;1)^{i}" title="P_{i} = (2p^{2} - 2p + 1)^{i}" /></a>

<a href="http://www.codecogs.com/eqnedit.php?latex=P_{i}" target="_blank"><img src="http://latex.codecogs.com/png.latex?P_{i}" title="P_{i}" /></a>以指数形式递减。
