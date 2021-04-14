<!--
 * @Author: Ron
 * @LastEditors: Ron
 * @FilePath: \issue-test\c++\test43\README.md
-->

## Redis 发布订阅功能

下载 hiredis 源码 git@github.com:redis/hiredis.git

使用 hiredis 开发相关功能：

1. 同步方式实现的发布订阅                                       publisher.cc subscriber.cc
2. 使用 libevent（安装：libevent-devel）实现 redis GET/SET      redis.cc
3. 使用 libevent 实现订阅以及订阅的响应                          redis-libevent-sub.c

hiredis 本身就携带一个测试目录 examples，redis-libevent-sub.c 可直接拷至该目录使用

编译请用 `cmake .. -DENABLE_EXAMPLES=ON -Wno-dev`
