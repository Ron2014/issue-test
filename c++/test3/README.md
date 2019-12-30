# usage

test name1 name2

# problems

_strnicmp，顾名思义，是字符串比较函数（…str…cmp），其做法是取两个字符串的前n个字符，以忽略大小写的方式（i = ignore case）进行比较，即

```
参数：Hello hell 3

返回: 0 (equal)
```

找不到 _strnicmp 的问题在于，Linux 端这个函数确实不存在 string.h 中。

```
find /usr/include -type f | xargs grep ' _strnicmp\s*\('
```

因为同样的功能 Windows 函数名为 strnicmp，Linux 函数名为 strncasecmp。

作为C标准库，API居然互不兼容，这个BUG是GNU来接还是微软来接？？？

# solved

思路：判断非 Windows 平台，对函数名进行宏定义

源文件做法：

```
#ifndef WIN32
#define _strnicmp strncasecmp
#endif
```

CMake做法：

```
if(NOT WIN32)
add_definitions(-D_strnicmp=strncasecmp)
endif()
```
