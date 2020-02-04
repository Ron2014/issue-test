# test14: 标准输出的字符编码

[例子来源](https://www.cnblogs.com/dejavu/archive/2012/09/16/2687586.html)

注意，这里多个概念均拥有“字符编码”的属性：

1. 源文件的字符编码：*.c
2. 编辑器（将文件打开后用于显示）的字符编码：vim/vscode
3. 编译器（在编译时认定其处理文件）的字符编码：gcc
4. 二进制文件（中字符常量）的字符编码：ELF
5. 终端（bash会话）的字符编码：标准输出（stdout/stderr）

咋一看，只要是存储/处理字符的工具，都需要考虑“字符编码”的问题。

## 准备工具

二进制文件查看器

linux: objdump -ds \<filename\>

windows: 
* WinHex，是个免费体验/付费软件，适合有钱佬/破解佬。
* 通过 vscode 插件——hexdump for VSCode。

## 源文件字符编码

gbk.c

```bash
  Offset: 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 	
00000000: 23 69 6E 63 6C 75 64 65 20 3C 73 74 64 69 6F 2E    #include.<stdio.
00000010: 68 3E 0A 23 69 6E 63 6C 75 64 65 20 3C 73 74 64    h>.#include.<std
00000020: 6C 69 62 2E 68 3E 0A 23 69 6E 63 6C 75 64 65 20    lib.h>.#include.
00000030: 3C 6C 6F 63 61 6C 65 2E 68 3E 0A 0A 69 6E 74 20    <locale.h>..int.
00000040: 6D 61 69 6E 28 76 6F 69 64 29 20 7B 0A 20 20 20    main(void).{....
00000050: 20 2F 2F 20 73 65 74 6C 6F 63 61 6C 65 28 4C 43    .//.setlocale(LC
00000060: 5F 41 4C 4C 2C 20 22 7A 68 5F 43 4E 2E 55 54 46    _ALL,."zh_CN.UTF
00000070: 2D 38 22 29 3B 0A 20 20 20 20 63 6F 6E 73 74 20    -8");.....const.
00000080: 63 68 61 72 20 73 74 72 5B 5D 20 3D 20 22 D6 D0    char.str[].=."VP
00000090: CE C4 22 3B 0A 20 20 20 20 63 6F 6E 73 74 20 63    ND";.....const.c
000000a0: 68 61 72 20 65 6E 67 5B 5D 20 3D 20 22 68 65 6C    har.eng[].=."hel
000000b0: 6C 6F 20 77 6F 72 6C 64 22 3B 0A 20 20 20 20 70    lo.world";.....p
000000c0: 72 69 6E 74 66 28 22 31 3A 25 73 20 25 73 5C 6E    rintf("1:%s.%s\n
000000d0: 22 2C 20 73 74 72 2C 20 65 6E 67 29 3B 0A 09 66    ",.str,.eng);..f
000000e0: 70 72 69 6E 74 66 28 73 74 64 6F 75 74 2C 20 22    printf(stdout,."
000000f0: 32 3A 25 73 20 25 73 5C 6E 22 2C 20 73 74 72 2C    2:%s.%s\n",.str,
00000100: 20 65 6E 67 29 3B 0A 09 66 70 72 69 6E 74 66 28    .eng);..fprintf(
00000110: 73 74 64 65 72 72 2C 20 22 33 3A 25 73 20 25 73    stderr,."3:%s.%s
00000120: 5C 6E 22 2C 20 73 74 72 2C 20 65 6E 67 29 3B 0A    \n",.str,.eng);.
00000130: 20 20 20 20 72 65 74 75 72 6E 20 30 3B 0A 7D       ....return.0;.}
```

utf8.c

```bash
  Offset: 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 	
00000000: 23 69 6E 63 6C 75 64 65 20 3C 73 74 64 69 6F 2E    #include.<stdio.
00000010: 68 3E 0A 23 69 6E 63 6C 75 64 65 20 3C 73 74 64    h>.#include.<std
00000020: 6C 69 62 2E 68 3E 0A 23 69 6E 63 6C 75 64 65 20    lib.h>.#include.
00000030: 3C 6C 6F 63 61 6C 65 2E 68 3E 0A 0A 69 6E 74 20    <locale.h>..int.
00000040: 6D 61 69 6E 28 76 6F 69 64 29 20 7B 0A 20 20 20    main(void).{....
00000050: 20 2F 2F 20 73 65 74 6C 6F 63 61 6C 65 28 4C 43    .//.setlocale(LC
00000060: 5F 41 4C 4C 2C 20 22 7A 68 5F 43 4E 2E 55 54 46    _ALL,."zh_CN.UTF
00000070: 2D 38 22 29 3B 0A 20 20 20 20 63 6F 6E 73 74 20    -8");.....const.
00000080: 63 68 61 72 20 73 74 72 5B 5D 20 3D 20 22 E4 B8    char.str[].=."d8
00000090: AD E6 96 87 22 3B 0A 20 20 20 20 63 6F 6E 73 74    -f..";.....const
000000a0: 20 63 68 61 72 20 65 6E 67 5B 5D 20 3D 20 22 68    .char.eng[].=."h
000000b0: 65 6C 6C 6F 20 77 6F 72 6C 64 22 3B 0A 20 20 20    ello.world";....
000000c0: 20 70 72 69 6E 74 66 28 22 31 3A 25 73 20 25 73    .printf("1:%s.%s
000000d0: 5C 6E 22 2C 20 73 74 72 2C 20 65 6E 67 29 3B 0A    \n",.str,.eng);.
000000e0: 09 66 70 72 69 6E 74 66 28 73 74 64 6F 75 74 2C    .fprintf(stdout,
000000f0: 20 22 32 3A 25 73 20 25 73 5C 6E 22 2C 20 73 74    ."2:%s.%s\n",.st
00000100: 72 2C 20 65 6E 67 29 3B 0A 09 66 70 72 69 6E 74    r,.eng);..fprint
00000110: 66 28 73 74 64 65 72 72 2C 20 22 33 3A 25 73 20    f(stderr,."3:%s.
00000120: 25 73 5C 6E 22 2C 20 73 74 72 2C 20 65 6E 67 29    %s\n",.str,.eng)
00000130: 3B 0A 20 20 20 20 72 65 74 75 72 6E 20 30 3B 0A    ;.....return.0;.
00000140: 7D                                                 }
```

区别就在于这两行了

```bash
00000080: 63 68 61 72 20 73 74 72 5B 5D 20 3D 20 22 D6 D0    char.str[].=."VP
00000090: CE C4 22 3B 0A 20 20 20 20 63 6F 6E 73 74 20 63    ND";.....const.c

00000080: 63 68 61 72 20 73 74 72 5B 5D 20 3D 20 22 E4 B8    char.str[].=."d8
00000090: AD E6 96 87 22 3B 0A 20 20 20 20 63 6F 6E 73 74    -f..";.....const
```

也就是对 "中文" 的编码：

1. gbk 为4个字节：D6 D0 CE C4
2. utf8 为6个字节：E4 B8 AD E6 96 87

彼此相差2个字节，不妨直接看看文件大小

```bash
>ls -al gbk.c utf8.c
-rw-r--r-- 1 Ron None 319 Jan 31 21:26 gbk.c
-rw-r--r-- 1 Ron None 321 Jan 31 21:26 utf8.c
```

数一下二进制查看器列出的字节矩阵：前者20行少1个字节，后者20行多1个字节，每行16字节

1. gbk:319=20*16-1
2. utf8:321=20*16+1

全都对应上了。再随便搜出几个字符编码查询网站 [gbk](http://www.mytju.com/classcode/tools/encode_gb2312.asp) [utf8](http://www.mytju.com/classcode/tools/encode_utf8.asp) 

得到这样的结果

字符 | GBK编码16进制(GBK内码) | utf8编码16进制 | Unicode编码16进制
--|--|--|--|
中 | D6D0 | E4B8AD | 4E2D
文 | CEC4 | E69687 | 4E2D

网站信息和 hexdump 查看的结果一一对应了。由此我们可以做出一个大胆的假设：

设置源文件的字符集做出的改变，基本上体现在两个地方：

1. 注释
2. 字符串（""内）中文本的编码

除此之外的地方，均是代码的天地。不论是变量名、类名，或是函数调用，用到字符和标点符号，永远都在ASCII表中可查（127个字符）。不论如何修改字符编码，它们的内容不会有任何变化。所以，任何编程语言的源文件，均可划分成这三部分：

1. 注释内容（字符编码）
2. 字符串内容（字符编码）
3. 代码（ASCII，不被修改字符集影响）

走到这里，我们不妨做个大胆假设：

**编译器没有所谓的默认字符集，也没有识别源文件字符集的选项，它只是简单得把注释内容过滤掉，把字符串内容那部分编码数据，搬运到ELF文件中而已。**

接下来，我们看看ELF文件中是否存在这些编码数据：

1. gbk：D6 D0 CE C4
2. utf8：E4 B8 AD E6 96 87
   
## ELF 可执行文件中的字符串常量

```bash
# gcc -o gbk gbk.c
# objdump -ds gbk > objdump_gbk
# gcc -o utf8 utf8.c
# objdump -ds utf8 > objdump_utf8
```

这样，我们可以在双方的 .text 看到这个字符串当做了立即数进行处理。

objdump_gbk

```bash
000000000000076a <main>:
 76a:	55                   	push   %rbp
 76b:	48 89 e5             	mov    %rsp,%rbp
 76e:	48 83 ec 20          	sub    $0x20,%rsp
 772:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
 779:	00 00 
 77b:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
 77f:	31 c0                	xor    %eax,%eax
 781:	c7 45 e7 d6 d0 ce c4 	movl   $0xc4ced0d6,-0x19(%rbp)
 788:	c6 45 eb 00          	movb   $0x0,-0x15(%rbp)
 78c:	48 b8 68 65 6c 6c 6f 	movabs $0x6f77206f6c6c6568,%rax
 793:	20 77 6f 
 796:	48 89 45 ec          	mov    %rax,-0x14(%rbp)
 79a:	c7 45 f4 72 6c 64 00 	movl   $0x646c72,-0xc(%rbp)
 7a1:	48 8d 55 ec          	lea    -0x14(%rbp),%rdx
 7a5:	48 8d 45 e7          	lea    -0x19(%rbp),%rax
 7a9:	48 89 c6             	mov    %rax,%rsi
 7ac:	48 8d 3d f1 00 00 00 	lea    0xf1(%rip),%rdi        # 8a4 <_IO_stdin_used+0x4>
 7b3:	b8 00 00 00 00       	mov    $0x0,%eax
 7b8:	e8 73 fe ff ff       	callq  630 <printf@plt>
 7bd:	48 8b 05 5c 08 20 00 	mov    0x20085c(%rip),%rax        # 201020 
```

objdump_utf8

```bash
000000000000076a <main>:
 76a:	55                   	push   %rbp
 76b:	48 89 e5             	mov    %rsp,%rbp
 76e:	48 83 ec 20          	sub    $0x20,%rsp
 772:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
 779:	00 00 
 77b:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
 77f:	31 c0                	xor    %eax,%eax
 781:	c7 45 e5 e4 b8 ad e6 	movl   $0xe6adb8e4,-0x1b(%rbp)
 788:	66 c7 45 e9 96 87    	movw   $0x8796,-0x17(%rbp)
 78e:	c6 45 eb 00          	movb   $0x0,-0x15(%rbp)
 792:	48 b8 68 65 6c 6c 6f 	movabs $0x6f77206f6c6c6568,%rax
 799:	20 77 6f 
 79c:	48 89 45 ec          	mov    %rax,-0x14(%rbp)
 7a0:	c7 45 f4 72 6c 64 00 	movl   $0x646c72,-0xc(%rbp)
 7a7:	48 8d 55 ec          	lea    -0x14(%rbp),%rdx
 7ab:	48 8d 45 e5          	lea    -0x1b(%rbp),%rax
 7af:	48 89 c6             	mov    %rax,%rsi
 7b2:	48 8d 3d fb 00 00 00 	lea    0xfb(%rip),%rdi        # 8b4 <_IO_stdin_used+0x4>
 7b9:	b8 00 00 00 00       	mov    $0x0,%eax
 7be:	e8 6d fe ff ff       	callq  630 <printf@plt>
 7c3:	48 8b 05 56 08 20 00 	mov    0x200856(%rip),%rax        # 201020 
 ```

重要看这两行。

 ```bash 
 781:	c7 45 e7 d6 d0 ce c4 	movl   $0xc4ced0d6,-0x19(%rbp)

 781:	c7 45 e5 e4 b8 ad e6 	movl   $0xe6adb8e4,-0x1b(%rbp)
 788:	66 c7 45 e9 96 87    	movw   $0x8796,-0x17(%rbp)
 ```

utf8 文件中还嫌这个立即数太大了，分了两个mov指令来赋值。先是 e4 b8 ad e6，再是 96 87。

所以，这里更新一下认识：

字符编码决定权，仅在于编辑器和终端。或者说，**字符编码对于 I/O GUI 来讲才有意义。**

当然，你也可以从不同视角来看待这个概念：

**我们在编辑器/终端上，肉眼看到的文本（源）文件，其实是经过一种类似于翻译器的编码器（encoder）处理后才显示出来的一个个字符。对于计算机内部、文件存储来讲，它们只认字节，就是二进制查看器输出的内容。对于编辑器、终端来讲，它们从文件中取出这些字节，经过 encoder 处理，才最终呈现出来。encoder 又能事先指定字符集（是 gbk 还是 utf8）。就像选择字典和查词算法一样，encoder 最终就能把字节表示的编号，转换成人类语言能够理解的字符图形（中文、日文或是其他）。我们在编辑器中输入的字符图形，也会通过 encoder 逆向逻辑变成对应字符集的编号，存储到文件中。**

编辑器的 encoding 已经很常见了，前面也解释了原理。所以这个问题最终变成了：攻略所有终端对字符编码的支持。

Windows 终端：

1. CMD 命令行提示符

编译

```
mkdir build-win-cmd
cd build-win-cmd
cmake ..
msbuild Encoding.sln
```

结果

```bash
E:\GitHub\issue-test\c\test14\build-win-cmd\Debug>chcp
Active code page: 936
E:\GitHub\issue-test\c\test14\build-win-cmd\Debug>gbk.exe
1:中文 hello world
2:中文 hello world
3:中文 hello world
E:\GitHub\issue-test\c\test14\build-win-cmd\Debug>utf8.exe
1:涓枃 hello world
2:涓枃 hello world
3:涓枃 hello world

E:\GitHub\issue-test\c\test14\build-win-cmd\Debug>chcp
Active code page: 65001
E:\GitHub\issue-test\c\test14\build-win-cmd\Debug>gbk.exe
1: hello world
2: hello world
3: hello world
E:\GitHub\issue-test\c\test14\build-win-cmd\Debug>utf8.exe
1:中文 hello world
2:中文 hello world
3:中文 hello world
```

2. PowerShell

编译

```bash
mkdir build-win-powershell
cd build-win-powershell
cmake ..
msbuild Encoding.sln
```

结果

```bash
PS E:\GitHub\issue-test\c\test14\build-win-powershell\Debug> chcp
Active code page: 936
PS E:\GitHub\issue-test\c\test14\build-win-powershell\Debug> .\gbk.exe
1:中文 hello world
2:中文 hello world
3:中文 hello world
PS E:\GitHub\issue-test\c\test14\build-win-powershell\Debug> .\utf8.exe
1:涓枃 hello world
2:涓枃 hello world
3:涓枃 hello world

PS E:\GitHub\issue-test\c\test14\build-win-powershell\Debug> chcp
Active code page: 65001
PS E:\GitHub\issue-test\c\test14\build-win-powershell\Debug> .\gbk.exe
1: hello world
2: hello world
3: hello world
PS E:\GitHub\issue-test\c\test14\build-win-powershell\Debug> .\utf8.exe
1:中文 hello world
2:中文 hello world
3:中文 hello world
```

1. Cygwin

编译

```bash
mkdir build-win-cygwin
cd build-win-cygwin
cmake ..
msbuild.exe Encoding.sln
```

结果

```bash
[Options > Text > Local:zh_CN Character set:GBK]
Ron@Ron-PC /cygdrive/e/GitHub/issue-test/c/test14/build-win-cygwin/Debug
$ ./gbk.exe
3:中文 hello world
1:中文 hello world
2:中文 hello world
Ron@Ron-PC /cygdrive/e/GitHub/issue-test/c/test14/build-win-cygwin/Debug
$ ./utf8.exe
3:涓枃 hello world
1:涓枃 hello world
2:涓枃 hello world

[Options > Text > Local:zh_CN Character set:UTF-8]
Ron@Ron-PC /cygdrive/e/GitHub/issue-test/c/test14/build-win-cygwin/Debug
$ ./gbk.exe
3:▒▒▒▒ hello world
1:▒▒▒▒ hello world
2:▒▒▒▒ hello world
Ron@Ron-PC /cygdrive/e/GitHub/issue-test/c/test14/build-win-cygwin/Debug
$ ./utf8.exe
3:中文 hello world
1:中文 hello world
2:中文 hello world
```

4. Msys2/MinGW

编译

```bash
mkdir build-win-Msys
cd build-win-Msys
cmake ..
make
```

结果

```bash
[Options > Text > Local:zh_CN Character set:GBK]
Ron@Ron-PC MINGW64 /e/GitHub/issue-test/c/test14/build-win-Msys2
$ ./gbk.exe
3:中文 hello world
1:中文 hello world
2:中文 hello world
Ron@Ron-PC MINGW64 /e/GitHub/issue-test/c/test14/build-win-Msys2
$ ./utf8.exe
3:涓枃 hello world
1:涓枃 hello world
2:涓枃 hello world

[Options > Text > Local:zh_CN Character set:UTF-8]
Ron@Ron-PC MINGW64 /e/GitHub/issue-test/c/test14/build-win-Msys2
$ ./gbk.exe
1:▒▒▒▒ hello world
2:▒▒▒▒ hello world
3:▒▒▒▒ hello world
Ron@Ron-PC MINGW64 /e/GitHub/issue-test/c/test14/build-win-Msys2
$ ./utf8.exe
1:中文 hello world
2:中文 hello world
3:中文 hello world
```

5. Localhost/SecureCRT

这个要到 Setting 里开启 OpenSSH 服务。

Setting > App > App & features > Optional features > Add a feature > OpenSSH Server

Linux 终端：

1. VirtualBox-Ubuntu
2. Windows 自带 Ubuntu

编译

```bash
mkdir build-win-ubuntu
cd build-win-ubuntu
cmake ..
make
```

结果( Ubuntu 锁死了 65001 的字符集 )

```bash
ron@Ron-PC:/mnt/e/GitHub/issue-test/c/test14/build-win-ubuntu$ ./gbk
1: hello world
2: hello world
3: hello world
ron@Ron-PC:/mnt/e/GitHub/issue-test/c/test14/build-win-ubuntu$ ./utf8
1:中文 hello world
2:中文 hello world
3:中文 hello world
```

3. [云服务器]ChicagoVPS-CentOS/SecureCRT
4. [云服务器]AWS-Lightsail-Ubuntu/SecureCRT

Windows 终端 + Docker/Ubuntu：

1. CMD + docker
2. PowerShell + docker
3. Cygwin + docker
4. Msys2/MinGW + docker
5. Localhost/SecureCRT + docker

Linux 终端 + Docker/Ubuntu：

1. VirtualBox-Ubuntu + docker
2. Windows 自带 Ubuntu + docker
3. [云服务器]ChicagoVPS-CentOS/SecureCRT + docker
4. [云服务器]AWS-Lightsail-Ubuntu/SecureCRT + docker