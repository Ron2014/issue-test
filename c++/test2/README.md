# usages

1. deault

g++ test2.cc -o test

2. use mutex

g++ test2.cc -o test -DUSE_MUTEX

3. use atomic

g++ test2.cc -o test -DUSE_ATOMIC

for linux, need -lpthread

# problems

compile on MacOS:

```
In file included from test2.cc:4:
In file included from ./test2.h:8:
./use_atomic.h:4:18: error: expected ';' after top level declarator
std::atomic_int i{0};
                 ^
                 ;
1 error generated.
```
# reference

[C++性能榨汁机之无锁编程](http://irootlee.com/juicer_lock_free/)