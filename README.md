# 测试用例集锦

## test0: isdigit 的参数好奇怪

观察如下函数，为什么 isdigit 传递的参数这么复杂？

```c
    static const char * scanformat(const char *strfrmt, char *form) {
        const char *p = strfrmt;
        while (*p != '\0' && strchr(FLAGS_CHARS, *p) != NULL) p++;  /* skip flags */
        if ((size_t)(p - strfrmt) >= sizeof(FLAGS_CHARS)) {
            Log::Error("invalid format (repeated flags) for %s", strfrmt);
            return nullptr;
        }

        if (isdigit((unsigned char)(*p))) p++;  /* skip width */
        if (isdigit((unsigned char)(*p))) p++;  /* (2 digits at most) */
        if (*p == '.') {
            p++;
            if (isdigit((unsigned char)(*p))) p++;  /* skip precision */
            if (isdigit((unsigned char)(*p))) p++;  /* (2 digits at most) */
        }
        if (isdigit((unsigned char)(*p))) {
            Log::Error("invalid format (width or precision too long) for %s", strfrmt);
            return nullptr;
        }

        *(form++) = '%';
        strncpy(form, strfrmt, p - strfrmt + 1);
        form += p - strfrmt + 1;
        *form = '\0';
        return p;
    }
```

## test1: 判断字符串b是否是字符串a的子串

char * strstr(char *a, char* b);

如果是，返回子串b在a中的起始指针，否则返回NULL。

## test2: 无锁编程下的线程安全

1. 使用原子操作

2. 生产者-消费者模型（双缓冲），如：

   一个链表A接收消息，一个链表B消费消息。主线程只会向链表A添加数据，另一个独立线程专门处理链表B的消息。
   
   独立线程每次唤醒时会交换AB链表头，仅这步操作需要加锁，其他情况无锁。

## test3: 总是找不到的_strnicmp

win能编过的项目，Linux上居然error: ‘_strnicmp’ was not declared in this scope

网上的说法是没有标准库文件，可是加上 #include <string.h> 依然没有效果

## test4: 对随机函数的百万级测试

年会分房间，2人成房，多出一个人不得已执行该程序。

功能其实很简单：`从一堆名字中随机出一个名字`。

因为程序的随机都是伪随机，rand 之前需要设种子，通常的做法都是拿时间做种子，为了确定写出来的代码随机概率是均匀分布的，我的兴趣放在了如何对这个小程序进行百万级测试上。

如何用源码实现？

如何用shell实现？

随机数种子获取正常？

如何对输出信息进行次数统计？

## test5: C++ lambda函数的实验

## test6: 空指针赋值给 std::string 会发生闪退？

因为 basic_string 包装 char * 时会调用 strlen(char *)

## test7: 线程对象：我杀我自己，不行？

线程分离/线程链接

## test8: 修改指针指向的数据

已知一个指针 q, 通过宏和q能改变其指向内存中的值么？

C语言传参可以传递引用么？

指针变量的连续声明应该怎么写？

## test9: 不设置 random seed 的后果

没有指定 random seed，就会导致每次执行程序的 random 结果一样？

## test10: 跨平台的文件、路径处理程序

windows 下处理需要

`#include <io.h>`

linux 下处理需要

`#include <dirent.h>`

各自的系统调用API又不一样（遍历或者删除）

编写一个满足跨平台需要的程序

## test11: 内存访问越界的危害 ------

该例是在做代码混淆时遇到的，这里使用简化的版本重现其算法：

利用内存工具 malloc/free 对一个已知数组添加无效的数据。

## test12: 研究换页键（ASCII 0x0C）------

## test13: 静态链接库的循环引用/依赖 ------

liba.a 的 xxa.o 中有对 libb.a 定义的符号的引用（函数定义），同理返回来也是

那么 gcc 应该怎么写呢？

## test14: 标准输出的字符编码 ------

在使用 docker 版的 ubuntu 时，发现，对于 gbk 源文件编译得到的程序，printf 出来的信息始终是乱码。

很明显这是个字符编码的问题。那么，我不禁要问：

用 gbk 字符集写的C文件，输出的字符就是 gbk 么？

此时用于执行该程序的终端（bash会话）的字符集果不是gbk，就一定会乱码么？

那么对于 docker 来讲，到底 tty 是终端，还是执行 docker run 的 CMD.exe 是终端？

在不修改任何编译选项的情况下，如何调整终端，才能正常显示 printf 文本呢？

假设真的是 CMD.exe 不行，那还得寻找替代方案 Cgywin 还是 Msys2/MinGGW，还是用 SecureCRT 连 127.0.0.1 的 ssh ？

## test15: C语言对宽字符的处理 ------

## test16: 高频函数中，局部变量指针的频繁 malloc/free 的优化

假设存在一个字符串加密函数 foo，它会在运行时被频繁调用（高频函数）。

foo 函数使用的加密算法是 openSSH 提供的 evp 库。使用 evp 的特点是，需要创建一个上下文对象 EVP_CIPHER_CTX 进行加密。

对象（结构体）的创建，在高频函数中使用 malloc/free 并不是很好的选择。

对于 foo 局部变量 EVP_CIPHER_CTX 指针，将其声明成 static 可以缓解这个问题么？

请比较 有/没有 static 这两种做法，在运行时的表现。

## test17: 字符串位运算加密

将一个数据块（32位）的每一位乱序，用一个类似于数组的数据结构（key）去记录正确的顺序和乱序的对应关系。

使得满足这样的功能，数据块在内存中是乱序的（乱码），但是经过 key 的读取之后就是可读的。

## test18: 二进制运算

1. 求一个数字有多少位
2. 求二进制中1的数量
3. 二进制打印

## test19：内联函数在编译时的处理

1. inline 的正确语法(无warning)？
2. inline 的类型？
3. inline 在编译的哪个步骤起作用的？

## test20：size_t/long 的内存大小 & lua 长字符串的长度上限

lua 有一处代码

```c
/* get string length from 'TString *s' */
#define tsslen(s)	((s)->tt == LUA_TSHRSTR ? (s)->shrlen : (s)->u.lnglen)
```

shrlen/lnglen 出自 lobject.h

```c
/*
** Header for string value; string bytes follow the end of this structure
** (aligned according to 'UTString'; see next).
*/
typedef struct TString {
  CommonHeader;
  lu_byte extra;  /* reserved words for short strings; "has hash" for longs */
  lu_byte shrlen;  /* length for short strings */
  unsigned int hash;
  union {
    size_t lnglen;  /* length for long strings */
    struct TString *hnext;  /* linked list for hash table */
  } u;
} TString;
```

前者 lu_byte 是1个无符号字节，后者 size_t 是C运行时库定义的数据类型（很可能不是1个字节）。

```c
/* chars used as small naturals (so that 'char' is reserved for characters) */
typedef unsigned char lu_byte;
```

对于 tsslen(s) 的调用，应该注意什么呢？比如，有一处代码是这样

```c
static void DumpString (const TString *s, DumpState *D) {
  if (s == NULL)
    DumpByte(0, D);
  else {
    size_t size = tsslen(s) + 1;  /* include trailing '\0' */
    const char *str = getstr(s);
    if (size < 0xFF)
      DumpByte(cast_int(size), D);
    else {
      DumpByte(0xFF, D);
      DumpVar(size, D);
    }
    DumpVector(str, size - 1, D);  /* no need to save '\0' */
  }
}
```

假设 size_t 是个 **8字节无符号整型**，那么 lua 的 `long strings` 的长度，真的需要这么大的空间来存储么？

lua 支持的短字符串长度为40（不包含结束符\0），那么长字符串的长度上限为多少？

PS. 短字符串长度上限 LUAI_MAXSHORTLEN 定义见 llimits.h

```c
/*
** Maximum length for short strings, that is, strings that are
** internalized. (Cannot be smaller than reserved words or tags for
** metamethods, as these strings must be internalized;
** #("function") = 8, #("__newindex") = 10.)
*/
#if !defined(LUAI_MAXSHORTLEN)
#define LUAI_MAXSHORTLEN	40
#endif
```

## test21: 不同的数据类型作为布尔表达式的表现 (所有语言)

Different data type converted to BOOL expression: C C++ Lua Python

## test22: TypeScriptToLua如何支持循环引用

循环引用（Circular Require, Circular dependencies），在lua环境中，指的是这样的情况：

有两个lua文件A和B，文件A中require了B，文件B中require了A，这样在lua解析时会陷入死循环。

很容易想到，在文件require（也就是加载）的时候，应该有三种状态。
1. 未加载
2. 加载中
3. 加载完成

但是lua原生的代码 package.loaded 仅支持1、3两种状态。
1. LOADED[name] = nil
3. LOADED[name] = loader返回值 / true

如何保证 require 顺序 A->B->A->... 时不陷入死循环.

## test23: C++ 线程池的实现

## test24：简易的技能、buff系统

## test25: C++ 的 struct 中定义一个 const 数组，如何初始化？

## test26: CPU的1/2/3级缓存的概念, 对程序设计有什么影响? 和ECS架构, 内存池有什么联系?

## test27: 粘包问题

## test28: shell sort 希尔排序

本质上是【分组的插入排序】

## test29: 除0零一定crash么?

## test30: cflow 使用

## test31: lua热更新

## test32: clock

## test33: 打印数组的全排列

## test34: 判断一个table是不是数组

## test35: 共享内存