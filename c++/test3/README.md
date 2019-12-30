# test3: 总是找不到的_strnicmp

win能编过的项目，Linux上居然error: ‘_strnicmp’ was not declared in this scope

网上的说法是没有标准库文件，可是加上 #include <string.h> 依然没有效果

## usage

test name1 name2

## problems

_strnicmp，顾名思义，是字符串比较函数（…str…cmp），其做法是取两个字符串的前n个字符，以忽略大小写的方式（i = ignore case）进行比较，即

```txt
参数：Hello hell 3

返回: 0 (equal)
```

找不到 _strnicmp 的问题在于，Linux 端这个函数确实不存在 string.h 中。

```bash
find /usr/include -type f | xargs grep ' _strnicmp\s*\('
```

因为同样的功能 Windows 函数名为 strnicmp，Linux 函数名为 strncasecmp。

作为C标准库，API居然互不兼容，这个BUG是GNU来接还是微软来接？？？

## solved

思路：判断非 Windows 平台，对函数名进行宏定义

源文件做法：

```c
#ifndef WIN32
#define _strnicmp strncasecmp
#endif
```

CMake做法：

```c
if(NOT WIN32)
add_definitions(-D_strnicmp=strncasecmp)
endif()
```

## 更正

考虑到 C 和 UNIX 的血缘关系，UNIX 和 POSIX 标准的历史渊源。如今所说的“C标准库”的概念可能需要咬文嚼字以下。

根据目前所了解的情况来看，可以确定，所谓“C标准库”的概念，是相对于POSIX系的操作系统来说的。这么理解的话，微软确实不在其列。

所以 Windows 和 Linux 向来都是水火不容的，编译器不一样、C运行时库、操作系统的API也不一样，那还谈什么“C标准”呢？

纵然两者系统目录都拥有 string.h 文件，因为系统API不一样、结构不一样导致的文件中代码实现不一样，也完全情有可原。

所以这里再理一理，所谓“C标准库”并不是文件级别的，而是函数级别的。也就是说，strlen strcmp 这些属于C标准库函数，它们存在于C标准库头文件 string.h 中。

至于 Windows 的 _stricmp stricmp strmpi 和 Linux 的 strcasecmp，都只能算是 C标准库函数的扩展，简称 “非标准扩展”，它们出现在 string.h 中仅仅因为它们确实应该放在字符串处理文件中，病不表示它们就是标准库函数。标准库头文件中的内容应该是这样的

> 标准库头文件 = 标准库函数 + 非标准扩展函数

所以，不要再吐槽名字不一致就是BUG了（显得没文化），真正的标准做法是：将字符串 tolower 后调用标准库函数 strncmp。