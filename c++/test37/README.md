# 一致性哈希

参考 [一致性哈希算法的原理与实现](https://kefeng.wang/2018/08/10/consistent-hashing/#1-2-%E4%B8%80%E8%87%B4%E6%80%A7%E5%93%88%E5%B8%8C-Consistent-Hashing)

写一个 C++ 版本

用于解决一个问题：在分布式系统中，对于同一来源的请求，总是唯一的转发至指定的服务中处理。

示例中，node 代表服务，object 代表请求
