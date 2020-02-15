# 内联函数在编译时的处理

在linux环境下做测试。编译器版本如下

```
$ gcc --version
gcc (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
Copyright (C) 2017 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

## 预处理 Preprocessing

```bash
gcc -E file.c -o 1Preprocessing/file.i
gcc -E main.c -o 1Preprocessing/main.i
```

这一阶段的处理可以理解成仅仅只是文本处理：

1. `#include`将头文件内容插入。
2. 宏定义的文本替换。

## 编译 Compilation

```bash
gcc -S 1Preprocessing/file.i -o 2Compilation/file.s
gcc -S 1Preprocessing/main.i -o 2Compilation/main.s
```

*.s 文件的内容就是汇编了，但是使用字符存储的，即文本文件（非二进制），所以还是可以用编辑器直接打开查看的。

在这里能看到 sizeof(xxx) 消失了，局部变量赋值成了栈上的操作 `movq	$4, -8(%rbp)`。

sizeof(xxx) 的结果，是在编译时就计算好了，而非运行时。这就充分说明了：

**数据类型的内存长度是编译器决定的，而非运行环境的操作系统/运行时库或是别的什么决定的。**

而且这里要注意 foo 函数符号 `call	foo@PLT`，PLT全称程序化链接表（Procedure Linkage Table），是动态链接延迟绑定（Lazy Binding）的概念。

延迟绑定就会涉及到一些独立的数据段 .got .got.plt：

.got保存全局变量引用地址，.got.plt保存函数引用地址，为了避免重复代码，.got.plt前三项还有特殊意义：

1. dynamic段地址
2. 本模块的ID
3. _dl_runtime_resolve的地址

但是看到 *.s 的内容，它离生成段结构还远。只是编译器有个警告：

```bash
In file included from main.c:2:0:
file.h:2:13: warning: inline function ‘foo’ declared but never defined
 inline void foo();
```

**这句警告隐晦得告诉我们，inline 内联函数的语法错误了，它不该是个声明了但是未定义（没有函数体）的状态。**

把 inline 的修饰符去掉，重新进行预处理（拷贝了 file.h 对 foo 的声明）、编译

```bash
gcc -E main.c -o 1Preprocessing/main.i
gcc -S 1Preprocessing/main.i -o 2Compilation/main.s
```

会得到一模一样的 main.s 汇编指令。这就表明 **编译器在警告的同时，inline 的内联修饰没有生效，编译器最终将其当做普通函数处理。**

真正生效的内联函数应该怎么写呢？生效之后，它在预处理、编译时的表现如何呢？待我走完整个流程一遍后，在新的小节讲解。

接下来的内容，继续以这个未生效的 inline 函数（即普通函数）进行。

## 汇编 Assembly

```bash
gcc -c 2Compilation/file.s -o 3Assembly/file.o
gcc -c 2Compilation/main.s -o 3Assembly/main.o
```

*.o 文件就是二进制文件了，需要通过一些反编译工具/二进制查看工具来查看里面内容。

```bash
$ objdump -ds -j .text main.o

main.o:     file format elf64-x86-64

Contents of section .text:
 0000 554889e5 4883ec20 897dec48 8975e048  UH..H.. .}.H.u.H
 0010 c745f804 000000b8 00000000 e8000000  .E..............
 0020 00b80000 0000c9c3                    ........

Disassembly of section .text:

0000000000000000 <main>:
   0:   55                      push   %rbp
   1:   48 89 e5                mov    %rsp,%rbp
   4:   48 83 ec 20             sub    $0x20,%rsp
   8:   89 7d ec                mov    %edi,-0x14(%rbp)
   b:   48 89 75 e0             mov    %rsi,-0x20(%rbp)
   f:   48 c7 45 f8 04 00 00    movq   $0x4,-0x8(%rbp)
  16:   00
  17:   b8 00 00 00 00          mov    $0x0,%eax
  1c:   e8 00 00 00 00          callq  21 <main+0x21>
  21:   b8 00 00 00 00          mov    $0x0,%eax
  26:   c9                      leaveq
  27:   c3                      retq
```

注意 foo 函数调用变成了 `callq  21 <main+0x21>`，这个 `main+0x21` 就是下一个汇编指令的地址。

没有毛病，这个地址会在（动态）链接时进行重定向，

## 链接 Linking

```bash
gcc 3Assembly/file.o 3Assembly/main.o -o 4Linking/main
```

依然只看代码段内容，而且过滤掉运行时库装载相关的指令、字节展示，简化如下：

```bash
$ objdump -ds -j .text main

Disassembly of section .text:

000000000000063a <foo>:
 63a:   55                      push   %rbp
 63b:   48 89 e5                mov    %rsp,%rbp
 63e:   48 8d 3d bf 00 00 00    lea    0xbf(%rip),%rdi        # 704 <_IO_stdin_used+0x4>
 645:   e8 c6 fe ff ff          callq  510 <puts@plt>
 64a:   90                      nop
 64b:   5d                      pop    %rbp
 64c:   c3                      retq

000000000000064d <main>:
 64d:   55                      push   %rbp
 64e:   48 89 e5                mov    %rsp,%rbp
 651:   48 83 ec 20             sub    $0x20,%rsp
 655:   89 7d ec                mov    %edi,-0x14(%rbp)
 658:   48 89 75 e0             mov    %rsi,-0x20(%rbp)
 65c:   48 c7 45 f8 04 00 00    movq   $0x4,-0x8(%rbp)
 663:   00
 664:   b8 00 00 00 00          mov    $0x0,%eax
 669:   e8 cc ff ff ff          callq  63a <foo>
 66e:   b8 00 00 00 00          mov    $0x0,%eax
 673:   c9                      leaveq
 674:   c3                      retq
 675:   66 2e 0f 1f 84 00 00    nopw   %cs:0x0(%rax,%rax,1)
 67c:   00 00 00
 67f:   90                      nop
```

可以看到 `callq  63a <foo>` 这个 foo 函数正确定位到了。63a 刚好就是 foo 函数的地址。至于怎么来的呢，看字节表示部分 ccffffff （相对偏移地址）其实表示的是个负数，10进制的-52，拿 callq 指令的下个指令地址 66e(16) - 52(10) = 63a(16)

## 内联函数的正确语法与编译的表现

内联函数的语法同时满足下面几个条件：

1. 内联函数必须写在头文件中。
2. 内联函数的声明和定义（函数体）必须同时存在。
3. 定义内联函数时，必须加上 inline 修饰符。如下风格是无效的：

```c
inline void foo(); // inline 仅与函数声明放在一起
void foo()
{
    printf("foo\n");
}
```

上面的例子可以测试一下。既然要满足条件1和条件2，表明file.c文件已经没有用了，所以很容易测试出来

```bash
gcc -E main.c -o 1Preprocessing/main.i
gcc -S 1Preprocessing/main.i -o 2Compilation/main.s
gcc -c 2Compilation/main.s -o 3Assembly/main.o
gcc 3Assembly/main.o -o 4Linking/main
objdump -ds -j .text 3Assembly/main.o > dump_o
objdump -ds -j .text 4Linking/main > dump_e

```

既然预处理只是文本的简单拷贝，这表明 main.s 中是存在 foo 函数定义的。

```


000000000000063a <foo>:
 63a:	55                   	push   %rbp
 63b:	48 89 e5             	mov    %rsp,%rbp
 63e:	48 8d 3d bf 00 00 00 	lea    0xbf(%rip),%rdi        # 704 <_IO_stdin_used+0x4>
 645:	e8 c6 fe ff ff       	callq  510 <puts@plt>
 64a:	90                   	nop
 64b:	5d                   	pop    %rbp
 64c:	c3                   	retq   

000000000000064d <main>:
 64d:	55                   	push   %rbp
 64e:	48 89 e5             	mov    %rsp,%rbp
 651:	48 83 ec 20          	sub    $0x20,%rsp
 655:	89 7d ec             	mov    %edi,-0x14(%rbp)
 658:	48 89 75 e0          	mov    %rsi,-0x20(%rbp)
 65c:	48 c7 45 f8 04 00 00 	movq   $0x4,-0x8(%rbp)
 663:	00 
 664:	b8 00 00 00 00       	mov    $0x0,%eax
 669:	e8 cc ff ff ff       	callq  63a <foo>
 66e:	b8 00 00 00 00       	mov    $0x0,%eax
 673:	c9                   	leaveq 
 674:	c3                   	retq   
 675:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 67c:	00 00 00 
 67f:	90                   	nop
```

main 函数对它的调用是 `call	foo`，看上去很像一个普通的局部函数调用。

试试下面的情况：

```c
void foo();
inline void foo() // inline 与函数定义体放在一起
{
    printf("foo\n");
}
```

这里是声明成普通函数，定义成内联函数，最后的结果和上面一样。foo 和 main 是独立的两个函数，对 foo 的调用很像是局部函数的调用。

去掉声明试试：

```c
inline void foo() // inline 声明与函数定义体放在一起
{
    printf("foo\n");
}
```

在链接阶段报错，没有成功生成可执行文件 main。这说明，**声明和定义一个都不能少。**

```
3Assembly/main.o: In function `main':
main.c:(.text+0x1d): undefined reference to `foo'
collect2: error: ld returned 1 exit status
```

那给声明也加上 inline 修饰呢：也会像上面报错。这表明，**inline 是一种“用于实现的关键字”，而不是一种“用于声明的关键字”。**

```c
inline void foo();
inline void foo() // inline 声明与函数定义体放在一起
{
    printf("foo\n");
}
```

所以对关键字 inline 的语法，完整的规则如下：

1. 内联函数必须写在头文件中。
2. 内联函数的声明和定义（函数体）必须同时存在。
3. 内联函数的声明，不能有 inline 修饰符
3. 内联函数的定义，必须加上 inline 修饰符。

当然，**在上面的所有试验当中，都没有成功实现所谓的内联：函数不生成独立的汇编指令，而是在调用的时候将汇编指令内联展开。**

这是为什么呢？

原来 gcc 编译器对 C语言的 inline 做了自己的扩展，其行为与C99标准中的inline有较大的不同。

也就说，C语言的 inline 心法，总共分六大部分（不考虑windows编译）：

gcc 的 inline:

1. static inline
2. inline
3. extern inline

C99 的 inline:

1. static inline
2. inline
3. extern inline。据说C99没有这种用法。

鉴于时间关系，更多的测试、inline的意义探讨等就暂且搁置了。感兴趣的朋友可以继续阅读以下的资料，来尝试写出真正的内联：

1. [Inline function](https://en.wikipedia.org/wiki/Inline_function)
2. [C 语法中static 和inline联合使用](https://www.cnblogs.com/thrillerz/p/5208579.html)
3. [C语言inline详细讲解](https://blog.csdn.net/Loving_Forever_/article/details/51567854)
4. [c语言： inline（gcc）](https://www.cnblogs.com/mylinux/p/5782119.html)
5. [[c&cpp] 内联函数(inline)总结](https://www.cnblogs.com/alex-tech/archive/2011/03/24/1994273.html)
6. [深入理解C语言](https://coolshell.cn/articles/5761.html)
7. [代码优化概要](https://coolshell.cn/articles/2967.html)

关于资料，其实也不是很好早。inline 其实是个很入门的概念，大多数资料都是止步于对概念的理解、使用的场合。

真正有人去做**反汇编的探索**，甚至像我一样，去**抠编译器处理阶段细节**的，我可以很自信得说，极少。

所以，如果有人偶遇到相关的探讨，不论哪国语言，欢迎推荐拍砖。