# test14: 标准输出的字符编码

[例子来源](https://www.cnblogs.com/dejavu/archive/2012/09/16/2687586.html)

注意，这里多个概念均拥有“字符编码”的属性：

1. 源文件的字符编码：*.c
2. 编辑器（将文件打开后用于显示）的字符编码：vim/vscode
3. 编译器（在编译时认定其处理文件）的字符编码：gcc
4. 二进制文件（中字符常量）的字符编码：ELF
5. 终端（bash会话）的字符编码：标准输出（stdout/stderr）

咋一看，只要是存储/处理字符的工具，都需要考虑“字符编码”的问题。

## 对于宽字符的处理

然鹅，对于这样简单的例子，ubuntu 下编译不成功。

```bash
# gcc -o main_gbk main_gbk.c
main_gbk.c: In function ‘main’:
main_gbk.c:5:22: error: converting to execution character set: Invalid or incomplete multibyte or wide character
     wchar_t wstr[] = L"����";
                      ^~~~~~~
```

参考 stone-age/saac 项目的编译方法，结果是一样的。

```bash
# gcc -g -w -O3 -g -rdynamic -I. -I./include -pipe -Wall -o main_gbk main_gbk.c
main_gbk.c: In function ‘main’:
main_gbk.c:5:22: error: converting to execution character set: Invalid or incomplete multibyte or wide character
     wchar_t wstr[] = L"����";
                      ^~~~~~~
```

仔细一看，问题都聚焦在 wchar_t 类型变量的声明上。

那么问题来了：**linux 对于宽字符变量是怎么处理的？**

### 方法1. 编译选项（弃用）

编译时，加上 `-finput-charset=GBK`

```bash
# gcc -g -w -O3 -g -rdynamic -I. -I./include -pipe -Wall -o main_gbk main_gbk.c -finput-charset=GBK
# ./main_gbk
1:中文
3:中文
```

做法很简单，但是宽字符那行输出居然没有了。

### 方法2. 
