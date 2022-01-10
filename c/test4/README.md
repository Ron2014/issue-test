# 对随机函数的百万级测试

## usage

rand name0 name1 name2 ...

## output

nameX

## 100w test

写好程序后，对于 `rand() % count + 1` 这段代码还是比较心虚的，所以想弄个百万级测试，像这样写了 shell（先测100次）

```bash
$ for i in `seq 1 100`; do ./rand 1 2 3 4 5 6 7 8; done > log
```

得到的结果是随机结果都一样，为啥呢，因为每次 rand 前都 srand 设一次种子，种子取的是时间，因为执行太快，以time这个API的精度根本无法区分出来两次 ./rand 执行之间时间发生了变化。所以每次 srand 得到的种子都是一样的，每次都执行一次 rand，其结果肯定也都一样了。

shell 不行，那先打开 TEST_100W，在源码层套入循环实现我们的需求先。

```bash
$ gcc main.c -o rand -DTEST_100W
$ ./rand 1 2 3 4 5 6 7 8 > log
$ cat log | awk '{a[$2]++}END{for(i in a){print i,a[i] | "sort -k 1"}}'
1 125062
2 124871
3 125005
4 125012
5 125419
6 124683
7 125286
8 124662
```

从统计的次数来看，一共8个数字，概率比较均匀，算是测试通过了。

## problem

那么回到之前的问题，shell 循环因为 time 的精度导致测试无效，那么 time 的精度能不能调高呢？到达微秒，甚至纳秒的级别。那还得看看我们程序指令的复杂度够不够消耗掉CPU的一纳秒时间。即两个问题：

1. 程序一次执行时间如何测量？
2. 得到程序时间（的精度之后），就可以确定使用哪个系统API来做随机数种子。

先看汇编：

```bash
$ objdump -d rand
...
000000000000075a <main>:
 75a:   55                      push   %rbp
 75b:   48 89 e5                mov    %rsp,%rbp
 75e:   48 83 ec 20             sub    $0x20,%rsp
 762:   89 7d ec                mov    %edi,-0x14(%rbp)
 765:   48 89 75 e0             mov    %rsi,-0x20(%rbp)
 769:   83 7d ec 01             cmpl   $0x1,-0x14(%rbp)
 76d:   7f 13                   jg     782 <main+0x28>
 76f:   48 8d 3d 02 01 00 00    lea    0x102(%rip),%rdi        # 878 <_IO_stdin_used+0x8>
 776:   e8 75 fe ff ff          callq  5f0 <puts@plt>
 77b:   b8 00 00 00 00          mov    $0x0,%eax
 780:   eb 62                   jmp    7e4 <main+0x8a>
 782:   bf 00 00 00 00          mov    $0x0,%edi
 787:   e8 94 fe ff ff          callq  620 <time@plt>
 78c:   89 c7                   mov    %eax,%edi
 78e:   e8 7d fe ff ff          callq  610 <srand@plt>
 793:   8b 45 ec                mov    -0x14(%rbp),%eax
 796:   83 e8 01                sub    $0x1,%eax
 799:   89 45 fc                mov    %eax,-0x4(%rbp)
 79c:   e8 8f fe ff ff          callq  630 <rand@plt>
 7a1:   89 c6                   mov    %eax,%esi
 7a3:   48 8d 3d f0 00 00 00    lea    0xf0(%rip),%rdi        # 89a <_IO_stdin_used+0x2a>
 7aa:   b8 00 00 00 00          mov    $0x0,%eax
 7af:   e8 4c fe ff ff          callq  600 <printf@plt>
 7b4:   e8 77 fe ff ff          callq  630 <rand@plt>
 7b9:   99                      cltd
 7ba:   f7 7d fc                idivl  -0x4(%rbp)
 7bd:   89 d0                   mov    %edx,%eax
 7bf:   48 98                   cltq
 7c1:   48 83 c0 01             add    $0x1,%rax
 7c5:   48 8d 14 c5 00 00 00    lea    0x0(,%rax,8),%rdx
 7cc:   00
 7cd:   48 8b 45 e0             mov    -0x20(%rbp),%rax
 7d1:   48 01 d0                add    %rdx,%rax
 7d4:   48 8b 00                mov    (%rax),%rax
 7d7:   48 89 c7                mov    %rax,%rdi
 7da:   e8 11 fe ff ff          callq  5f0 <puts@plt>
 7df:   b8 00 00 00 00          mov    $0x0,%eax
 7e4:   c9                      leaveq
 7e5:   c3                      retq
 7e6:   66 2e 0f 1f 84 00 00    nopw   %cs:0x0(%rax,%rax,1)
 7ed:   00 00 00
```

main函数也就40行机器指令，非常短。当然一次 ./rand 调用还包含操作系统对可执行文件的验证、程序的装载、动态链接等步骤，main 的机器指令长度来评估的方式水分太大。

所以，我们需要一个科学的测量程序一次执行耗时的工具，那会是什么呢——time命令

```bash
$ time ./rand 1 2 3 4 5 6 7 8
716819005
1

real    0m0.003s
user    0m0.000s
sys     0m0.000s
```

1. real。就是整个运行这个进程从一开始到结束的整个时间（时钟墙）
2. user。是指在运行这个进程中花在用户模式下的时间，即不包括内核的运行时间
3. sys。是指在运行这个进程，花在内核调用的时间，而不是代码库调用

user+sys=这个进程花的时间。real则包含了进程调度等等一系列额外开销。

这样一看，time 命令的精度也没测出来时间，所以在源码级加上了 TEST_TIME 的方式，测出一个main执行的时间（约等于user时间）。

```bash
$ gcc main.c -o rand -DTEST_TIME
$./rand 1 2 3 4 5 6 7 8
result: 1
0.0000670000
```

所以这就清楚了：67微秒。然后我们知道有个结构出现过微秒单位。

```c
struct timeval{
long int tv_sec; // 秒数
long int tv_usec; // 微秒数
}
```

果断再加上宏 TEST_SHELL

```c
#ifdef TEST_SHELL
    struct timeval t3;
    gettimeofday( &t3, NULL );
    srand((unsigned int)t3.tv_usec);
#else
	srand((unsigned int)time(NULL));
#endif
```

跑一下，考虑到 real time 为 0.003秒，100W执行就是50分钟了。所以还是测1000次（3秒）好了。

```bash
$ gcc main.c -o rand -DTEST_SHELL
$ for i in `seq 1 1000`; do ./rand 1 2 3 4 5 6 7 8; done > log
$ cat log | awk '{a[$2]++}END{for(i in a){print i,a[i] | "sort -k 1"}}'
1 120
2 130
3 115
4 113
5 142
6 132
7 123
8 125
```

可以看到，因为对毫秒级时间是敏感的，得到了均匀的随机结果。

## 程序启动的系统开销居然这么大！

百万级测试我们总共用了两个方法

1. 源码实现

```
gcc main.c -o rand -DTEST_100W -DTEST_TIME
```

2. shell 实现

```
gcc main.c -o rand -DTEST_SHELL
```

哪种方式好呢？

对于源码实现，在main中执行完100W循环，共耗时53毫秒（每次53纳秒）。而 shell 实现离不开每次程序启动时带来的系统开销，每次3毫秒，与源码实现相差56603倍。

可见，对于不额外增加代码逻辑的小规模测试，shell 实现具有优势。如果想对测试频率提高一个数量级，建议还是源码实现好了。

## 静态编译

如何缩减 `程序启动的系统开销` 呢？首先想到的是静态编译（gcc编译默认是动态的）。

```bash
$ gcc -static main.c -o rands -DTEST_SHELL
$ ls -al
-rwxr-xr-x 1 root root   8512 Dec 31 10:00 rand*
-rwxr-xr-x 1 root root 849872 Dec 31 10:00 rands*
$ time ./rand 1 2 3 4 5 6 7 8
result: 1

real    0m0.003s
user    0m0.000s
sys     0m0.000s
$ time ./rands 1 2 3 4 5 6 7 8
result: 1

real    0m0.008s
user    0m0.000s
sys     0m0.000s
```

执行一看，静态编译还是比动态编译要慢。这时候就想到要不要给可执行文件瘦身？《程序员的自我修养——链接、装载和库》中有提到相关的内容。