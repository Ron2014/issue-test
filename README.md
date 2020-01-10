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