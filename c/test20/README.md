# test20：size_t 的内存大小 & lua 长字符串的长度上限

数据类型的内存大小，通常和操作系统/编译器有关。

windows 下，见 `D:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.16.27023\include\vcruntime.h`

```c
// Definitions of common types
#ifdef _WIN64
    typedef unsigned __int64 size_t;
    typedef __int64          ptrdiff_t;
    typedef __int64          intptr_t;
#else
    typedef unsigned int     size_t;
    typedef int              ptrdiff_t;
    typedef int              intptr_t;
#endif
```

linux 下，没有很好的代码查看器，以标准库文件 `stddef.h` 为起点查看，wsl2-Ubuntu-18.04 下这个文件在 `/usr/lib/gcc/x86_64-linux-gnu/7/include/stddef.h`

```c
#if !(defined (__GNUG__) && defined (size_t))
typedef __SIZE_TYPE__ size_t;
```

所以我们搞明白这个 __SIZE_TYPE__ 是啥就好了。还是在 `stddef.h` 文件中

```c
#ifndef __SIZE_TYPE__
#define __SIZE_TYPE__ long unsigned int
#endif
```

PS. 讲道理 `stddef.h` 就是为了解决 ptrdiff_t size_t wchar_t 这三个数据类型的定义而生的。

## 结论

Windows 和 Linux 的结果都是一样的，都是一个 **8字节的无符号整型**。

只是 Linux 端 这个 `long unsigned int` 的类型，看着头很大（这里的 long 更像是对 int 的修饰符）。毕竟 long 在不同编译器环境中的长度还不一样，之所以被如此诟病，Windows 才拿出了 `unsigned __int64` 的方案。

## 为啥 long 让人头大

用 cmake 将该项目分别在 Windows/MSBuild 和 Linux/make 下编译得到

```bash
E:\GitHub\issue-test\c\test20\build_win>rm -rf *
E:\GitHub\issue-test\c\test20\build_win>cmake -G "Visual Studio 15 Win64" ..
E:\GitHub\issue-test\c\test20\build_win>MSBuild SizeTest.sln
E:\GitHub\issue-test\c\test20\build_win>Debug\main.exe
size: pointer 8
size: long 4
size: int 4
size: long unsigned int 4
size: size_t 8
```

Windows 下编译要注意必须加上 `-G "Visual Studio 15 Win64"` 指定64位编译，否则 cmake 生成的编译配置文件 CMakeCache.txt 是32位：

`CMAKE_MODULE_LINKER_FLAGS:STRING=/machine:x64`

MSBuild.exe 是预先加到 %PATH% 中的，用 Everything 找到 Visual Studio 安装目录即可，我的是 `D:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\`

PS. 如果是 cmake 构建工程，编出来32位，真的不怪 MSBuild，所以别看它放在 `Program Files (x86)`目录，下面又有个 `amd64` 目录，都没关系。**从 CMakeCache.txt 找原因**。

```bash
E:\GitHub\issue-test\c\test20\build_win>rm -rf *
E:\GitHub\issue-test\c\test20\build_win>cmake ..
E:\GitHub\issue-test\c\test20\build_win>make
ron@PC-20191115GQZJ:/mnt/e/GitHub/issue-test/c/test20/build_linux$ ./main
size: pointer 8
size: long 8
size: int 4
size: long unsigned int 8
size: size_t 8
```

这样一看就知道 long 和 long unsigned int 有多让人头大了吧。

扪心自问一下：有 long 的数据类型都是多少字节的？是不是已经晕了？

## lua 长字符串的长度上限

具体看字符串创建接口：luaS_new -> luaS_newlstr

分两段的原因是 luaS_new 做了优化：用 hash 值去 global_State->strcache 中查找缓存的字符串，此时有两个结果：

1. 找到（hit）：返回该 TString
2. 未找到：
   1. 移除 global_State->strcache 末尾的 TString（内存搬运，此处是个遍历）
   2. 调用 luaS_newlstr 创建新的 TString
   3. 头插入：global_State->strcache 的 0 号位置

```c
/*
** new string (with explicit length)
*/
TString *luaS_newlstr (lua_State *L, const char *str, size_t l) {
  if (l <= LUAI_MAXSHORTLEN)  /* short string? */
    return internshrstr(L, str, l);
  else {
    TString *ts;
    if (l >= (MAX_SIZE - sizeof(TString))/sizeof(char))
      luaM_toobig(L);
    ts = luaS_createlngstrobj(L, l);
    memcpy(getstr(ts), str, l * sizeof(char));
    return ts;
  }
}
```

可知长字符串最长可以达到 `MAX_SIZE - sizeof(TString))/sizeof(char) - 1`（为什么要 -1 呢？因为这里的长度 l 不包括结束符\0）。

sizeof(char) 就是 1

MAX_SIZE 定义为（64位操作系统下。以下 size_t lua_Integer 均和操作系统有关）

```c
#define LUA_MAXINTEGER		LLONG_MAX

/* maximum value for size_t */
#define MAX_SIZET	((size_t)(~(size_t)0))

/* maximum size visible for Lua (must be representable in a lua_Integer */
#define MAX_SIZE	(sizeof(size_t) < sizeof(lua_Integer) ? MAX_SIZET \
                          : (size_t)(LUA_MAXINTEGER))
```

因为代码已经深入到对操作系统的依赖了，下面引出 lua 对64位开发、执行环境做出的支持。

在64位开发环境下，得到的 LLONG_MAX 就是 2^63-1 (8字节有符号整型的最大值)。

size_t 的长度通常是在C标准库中定义的（当然，标准库中通常是 #ifndef size_t 的写法，支持你在自己的工程里将其定义）。

lua_Integer 的长度则是在 lua 项目中通过宏开关来定义的，其中包括两个数据类型的宏定义、三个标准。这些内容，其实也就是 **lua 对64位开发、执行环境做出的支持**。

两个数据类型的宏定义包括：

1. 整型 LUA_INT_TYPE
2. 浮点型 LUA_FLOAT_TYPE

官方解释如下

```c
/*
@@ LUA_INT_TYPE defines the type for Lua integers.
@@ LUA_FLOAT_TYPE defines the type for Lua floats.
** Lua should work fine with any mix of these options (if supported
** by your C compiler). The usual configurations are 64-bit integers
** and 'double' (the default), 32-bit integers and 'float' (for
** restricted platforms), and 'long'/'double' (for C compilers not
** compliant with C99, which may not have support for 'long long').
*/
```

他们的值有各自对应的预选列表。最终会根据标准，从里面选出一个值。

```c
/* predefined options for LUA_INT_TYPE */
#define LUA_INT_INT		1
#define LUA_INT_LONG		2
#define LUA_INT_LONGLONG	3

/* predefined options for LUA_FLOAT_TYPE */
#define LUA_FLOAT_FLOAT		1
#define LUA_FLOAT_DOUBLE	2
#define LUA_FLOAT_LONGDOUBLE	3
```

三个标准分别为

1. LUA_32BITS
2. 非Windows环境的LUA_USE_C89，此时会开启 LUA_C89_NUMBERS
3. 默认标准

官方解释如下

```c
/*
@@ LUA_32BITS enables Lua with 32-bit integers and 32-bit floats. You
** can also define LUA_32BITS in the make file, but changing here you
** ensure that all software connected to Lua will be compiled with the
** same configuration.
*/
/* #define LUA_32BITS */


/*
@@ LUA_USE_C89 controls the use of non-ISO-C89 features.
** Define it if you want Lua to avoid the use of a few C99 features
** or Windows-specific features on Windows.
*/
/* #define LUA_USE_C89 */
```

默认标准好理解，就是预处理走完前两个步骤之后，还是发现 `#if !define(xxx)` 时做的处理。

```c
/*
** default configuration for 64-bit Lua ('long long' and 'double')
*/
#if !defined(LUA_INT_TYPE)
#define LUA_INT_TYPE	LUA_INT_LONGLONG
#endif

#if !defined(LUA_FLOAT_TYPE)
#define LUA_FLOAT_TYPE	LUA_FLOAT_DOUBLE
#endif
```

### MAX_SIZE

最后，我们还是回到 MAX_SIZE 的定义

```c
#define LUA_MAXINTEGER		LLONG_MAX

/* maximum value for size_t */
#define MAX_SIZET	((size_t)(~(size_t)0))

/* maximum size visible for Lua (must be representable in a lua_Integer */
#define MAX_SIZE	(sizeof(size_t) < sizeof(lua_Integer) ? MAX_SIZET \
                          : (size_t)(LUA_MAXINTEGER))
```

留意这个条件 `sizeof(size_t) < sizeof(lua_Integer)`

什么时候 size_t 的长度会比 lua_Integer 小呢？个人猜测会有这样的情况：

1. 项目编译成32位程序（gcc -m32 ...），但是 lua 的整型数据需要支持64位。
2. 在32位操作系统/编译器下编译项目

在上述情况下，MAX_SIZE 取的是两者中的最小值 size_t 能表示的最大值。**注意，这里的 size_t 是无符号整型，所以能做~0运算**。

```c
/* maximum value for size_t */
#define MAX_SIZET	((size_t)(~(size_t)0))
```

大部分情况，两者其实是相等的，此时取的是 lua 整型 lua_Integer 能表示的最大值 LUA_MAXINTEGER。**注意，lua_Integer 能表示负数，所以这里是有符号整型的上限**。

### 总结

lua 的长字符串数据的长度上限，就是【lua_Integer 能表示的最大整数 - sizeof(TString) - 1】，如果把 sizeof(TString) 理解成字符串数据类型的头部，则最终结果为

**lua_Integer 能表示的最大整数 - TString头部空间 - 1**

### 实际应用的问题

现在64位开发、运行环境已经很普及了，所以 MAX_SIZE 的值是 2^63-1。

我们很难想象这么长的字符串会造成多大的影响。

若将这个字符串写入文件，每个字符占1个字节，文件大小就约有2^23TB，也就是 8388608 TB。

这么大的数据量，对于搜索、排序、I/O 读写、网络传输都会造成不少的压力。

当然，好处很明显：lua 提供给我们一个几乎无限制的字符串创建接口。

但是，我不禁要问：

1. 到底什么情况下才需要这么长的字符串呢？
2. 在需求明确的情况下，能否人为指定一个长字符串的长度上限。比如 uint32_t 最大可到42亿，uint16_t 最大可到 65536，感觉都已经够用了。