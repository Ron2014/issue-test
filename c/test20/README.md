# test20：size_t/long 的内存大小 & lua 长字符串的长度上限

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

当然，好处很明显：**lua 的字符串长度几乎是无限制的**。

但是，我不禁要问：

1. 到底什么情况下才需要这么长的字符串呢？
2. 在需求明确的情况下，能否人为指定一个长字符串的长度上限。比如 uint32_t 最大可到42亿，uint16_t 最大可到 65536，感觉都已经够用了。
3. 像这种报警，真的有条件触发么？若真要创建出800万TB的字符串，报警之前内存就爆炸了吧。

```c
l_noret luaM_toobig (lua_State *L) {
  luaG_runerror(L, "memory allocation error: block too big");
}
```

面对这样的疑问，我将 MAX_SIZE 写成了固定的、较小的数值，这样其占用的内存长度不会在不同的编译环境、执行环境中飘忽不定，也更符合人类行为规范。

```c
/**
 * size for [long string] [udata] [buffer of llex]
 * don't need too big: 8 bytes to save? no
 * 4,294,967,295 is enough: 4G, maximum of [unsigned int]/[uint32_t]
*/
#define MAX_SIZE UINT_MAX
```

统计了一下 MAX_SIZE 使用的地方，无一例外每一处都加了溢出判断，所以这个值应该理解为：判断内存溢出的标准。

```c
  if (luaZ_sizebuffer(b) >= MAX_SIZE/2)
    lexerror(ls, "lexical element too long", 0);
    
  if (l >= (MAX_SIZE - sizeof(TString))/sizeof(char))
    luaM_toobig(L);

  if (s > MAX_SIZE - sizeof(Udata))
    luaM_toobig(L);

  size_t l = vslen(top - n - 1);
  if (l >= (MAX_SIZE/sizeof(char)) - tl)
    luaG_runerror(L, "string length overflow");
```

既然每一处都有 error/warning 那就放心改

## int/short/long 的长度

上例中用 `size_t 的长度` 这个问题引出了数据类型 `long unsigned int`，那我们就着重说说 long。

在这个例子中，`long unsigned int` 表示 【长的、无符号、整型】，既然都无符号了，也就是两部分 long 和 unsigned int。

`unsigned int` 好说，它和有符号整型 `int` 的内存长度一致，在16位机上是2字节，32位/64位机上是4字节。

加上 long 修饰之后，其长度就会随编译环境变化了。同理，`long int` 也是不固定的，而且 `long int` 又可以简写成 `long`。

所以，源码中 long 的含义也不同。前者 long 成了修饰符，后者 long 表示一个数据类型 `long int`。同理，`short` 也是。

### 针对各平台编译

#### 构建，生成 CMakeCache.txt

为了搞清楚各个平台的 int/short/long 的长度，我们需要用到 cmake 工具构建

1. Windows 32-bit: `cmake -G "Visual Studio 15 2017" ..`
2. Windows 64-bit: `cmake -G "Visual Studio 15 2017 Win64" ..`
3. Linux 32-bit: `cmake -DCMAKE_C_FLAGS=-m32 ..`
4. Linux 64-bit: `cmake -DCMAKE_C_FLAGS=-m64 ..`
5. MacOS 32-bit: `cmake -DCMAKE_OSX_ARCHITECTURES=i386 ..`
6. MAcOS 64-bit: `cmake -DCMAKE_OSX_ARCHITECTURES=x86_64 ..`
7. MAcOS 96-bit: `cmake -DCMAKE_OSX_ARCHITECTURES=x86_64;i386 ..` :)

PS. 64位Linux支持32位编译需要提前安装4个工具

```bash
sudo apt install -y build-essential module-assistant gcc-multilib g++-multilib
```

#### 编译，生成可执行程序

对于编译指令，可以选用跨平台的 cmake 命令 `cmake --build path/to/CMakeCache.txt --config [Debug|Release]`

也可以选择平台自己的工具

1. Windows: MSBuild SizeTest.sln
2. Linux/UNIX: make

PS. MSBuild.exe 存放在 `path\to\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin`，请事先将这个目录加入 %PATH%

#### 参考

[How to build x86 and/or x64 on Windows from command line with CMAKE?](https://stackoverflow.com/questions/28350214/how-to-build-x86-and-or-x64-on-windows-from-command-line-with-cmake)
[指定CMAKE构建32位/64位版本的程序](https://www.cnblogs.com/youxiao/p/3240220.html)
[GCC 64位程序的makefile条件编译心得——32位版与64位版、debug版与release版（兼容MinGW、TDM-GCC）](https://www.cnblogs.com/zyl910/archive/2012/08/14/gcc64_make.html)
[在64位linux下编译32位程序](https://blog.csdn.net/XscKernel/article/details/38045783)

#### 结果

数据类型 | win 32 | win 64 | linux 32 | linux 64 | MacOS 32 | MacOS 64
--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|
pointer |4|8|4|8
int |4|4|4|4
short |2|2|2|2
long |4|4|4|8
long long |8|8|8|8
long unsigned int |4|4|4|8
size_t |4|8|4|8
const char* in macro |7, len 6|7, len 6|7, len 6|7, len 6

如果我们将**可移植性定义为：数据类型放在任何平台，其内存长度都不变。**

在上面的结果中，仅有三个数据类型满足：

1. int
2. short
3. long long

### 应用场景埋下的巨坑：lua 字节码文件头

所有的文件都有文件头，用来交给操作系统进行文件辨识，从而采取与之关联的工具来处理这个文件。这是我们在GUI环境下双击某个文件图标后，在我们看不到的地方进行的工作；是Windows系统菜单项文件`打开方式`的由来；是Linux下ELF可执行程序装载运行时库，最终创建进程执行的理论依据。

无一例外，对于lua字节码文件与lua解释器，也是这样的工作机制：

1. lua对文件进行解析，luaL_loadfile -> ... -> f_parser
2. 判断它是文本文件还是字节码文件：对于后者，文件的第一个字节是否是LUA_SIGNATURE[0]
3. 对于两种情况分别采取不同的策略进行处理
   1. 文本处理 luaY_parser：`递归下降法（可以理解成是回溯法的变种）`实现的【文件扫描 + 语法解析（字符串->opcode） + 虚拟机执行】
   2. 字节码文件处理 luaU_undump：文件头检查 + 加载字节生成opcode + 虚拟机执行

**我们花了这么长时间研究的 long 的巨坑就存在这个文件头里。**

文件头编码 ldump.c

```c
static void DumpHeader (DumpState *D) {
  DumpLiteral(LUA_SIGNATURE, D);
  DumpByte(LUAC_VERSION, D);
  DumpByte(LUAC_FORMAT, D);
  DumpLiteral(LUAC_DATA, D);
  DumpByte(sizeof(int), D);
  DumpByte(sizeof(size_t), D);
  DumpByte(sizeof(Instruction), D);
  DumpByte(sizeof(lua_Integer), D);
  DumpByte(sizeof(lua_Number), D);
  DumpInteger(LUAC_INT, D);
  DumpNumber(LUAC_NUM, D);
}
```

文件头解码 lundump.c

```c
static void checkHeader (LoadState *S) {
  checkliteral(S, LUA_SIGNATURE + 1, "not a");  /* 1st char already checked */
  if (LoadByte(S) != LUAC_VERSION)
    error(S, "version mismatch in");
  if (LoadByte(S) != LUAC_FORMAT)
    error(S, "format mismatch in");
  checkliteral(S, LUAC_DATA, "corrupted");
  checksize(S, int);
  checksize(S, size_t);
  checksize(S, Instruction);
  checksize(S, lua_Integer);
  checksize(S, lua_Number);
  if (LoadInteger(S) != LUAC_INT)
    error(S, "endianness mismatch in");
  if (LoadNumber(S) != LUAC_NUM)
    error(S, "float format mismatch in");
}
```

问题就出在和平台相关的数据类型 size_t 放入了字节码文件头 `DumpByte(sizeof(size_t), D);`，这就埋下了隐患：**某平台编译得到的lua字节码，放到另一个平台就无法通过check，执行不了。这种情况会发生在什么时候呢？答：lua的热更新。**

lua 为什么要检查 size_t 的长度呢？肯定这个变量在编解码有用武之地。

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

static TString *LoadString (LoadState *S) {
  size_t size = LoadByte(S);
  if (size == 0xFF)
    LoadVar(S, size);
  if (size == 0)
    return NULL;
  else if (--size <= LUAI_MAXSHORTLEN) {  /* short string? */
    char buff[LUAI_MAXSHORTLEN];
    LoadVector(S, buff, size);
    return luaS_newlstr(S->L, buff, size);
  }
  else {  /* long string */
    TString *ts = luaS_createlngstrobj(S->L, size);
    LoadVector(S, getstr(ts), size);  /* load directly in final place */
    return ts;
  }
}
```

对于长字符串的编解码，会事先存储表示其长度的变量，再存入字符串内容。所以注意上面的 DumpVar/LoadVar

```c
DumpVar(size, D);
LoadVar(S, size);
```

**存入/读取的字节长度是按照变量 size 的类型来的，size 的类型是 size_t。对于跨平台来讲，会有内存错位的风险。**

### clock_t 留下的巨坑

对于加密，通常会涉及到 random 计算，random 会需要设置 seed，seed 为了保证每次随机的序列不一样，会取和时间有关的值。所以 clock_t 就被用上了。对于对称加密来说，这个 clock_t 肯定要保存下来，交给解密程序读取。当加密程序、解密程序分别运行在不同平台，坑就来了。因为 clock_t 的定义是(time.h)

```c
typedef long clock_t;
```

Linux 的定义，通常都喜欢饶弯（写这些代码的程序员真的不是在玩漂移？）

```bash
bits/types/clock_t.h:typedef __clock_t clock_t;
bits/types.h:__STD_TYPE __CLOCK_T_TYPE __clock_t; /* Type of CPU usage counts.  */
bits/typesizes.h:#define __CLOCK_T_TYPE           __SYSCALL_SLONG_TYPE
bits/typesizes.h:
#if defined __x86_64__ && defined __ILP32__
# define __SYSCALL_SLONG_TYPE   __SQUAD_TYPE
#else
# define __SYSCALL_SLONG_TYPE   __SLONGWORD_TYPE
#endif
bits/types.h:#define __SLONGWORD_TYPE     long int
bits/types.h:
#if __WORDSIZE == 32
# define __SQUAD_TYPE           __quad_t
#elif __WORDSIZE == 64
# define __SQUAD_TYPE                long int
#endif
bits/types.h:
#if __WORDSIZE == 64
typedef long int __quad_t;
#else
__extension__ typedef long long int __quad_t;
#endif
```

定义链是这样的，也就是说32位下 clock_t = long int，64位下 clock_t 还是 = long int

```
clock_t -> __clock_t -> __CLOCK_T_TYPE -> __SYSCALL_SLONG_TYPE -> 
  32:__SLONGWORD_TYPE -> long int
  64:__SQUAD_TYPE -> 
    32: __quad_t -> 
      32: long long int
      64: long int
    64: long int
```

两个平台都是 long int， 这就让操作系统有了很多发挥空间了 :)


## 长整型的真正含义

`long` 或者说 `long int` 真是个让人捉摸不透的存在。

```
如果我们把数据类型比作人，把数据类型的内存长度比作这些人丁丁的长度。

long 可以指着 short 比2根手指说“你只有这么长”，对着int比4根手指说“你是这个”，对着long long要用两只手来比划。

然后 short/int/long long 凑过来一起问：那你有多长？

long 自信一笑：反正不比 int 短，也不比 long long 长，总之，可长可短。

short/int/long long：艹。

```

事到如今，对于 long/长整型 的真正含义，我真的开始怀疑所有的教科书是不是都印错了，long 和 long int 之间很容易产生歧义，所有的博客、专家的解释都太无力。

我所感受到的，“长”整型的真正含义是————

```
要花较“长”的时间、几番周折才能确定其在对应平台的内存长度的整型；也是要花较“长”时间才能彻底解决跨平台移植问题的整型”。
```