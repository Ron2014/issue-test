# usages

1. deault

g++ test2.cc -o test

2. use mutex

g++ test2.cc -o test -DUSE_MUTEX

3. use atomic

g++ test2.cc -o test -DUSE_ATOMIC

for linux, need -lpthread

# problems

## error: use of deleted function

compile(-DUSE_ATOMIC):

```
In file included from test2.h:8,
                 from test2.cc:4:
use_atomic.h:3:17: error: use of deleted function 'std::atomic<int>::atomic(const std::atomic<int>&)'
 atomic<int> i = 0;
                 ^
In file included from use_atomic.h:1,
                 from test2.h:8,
                 from test2.cc:4:
D:/msys64/mingw64/lib/gcc/x86_64-w64-mingw32/8.1.0/include/c++/atomic:668:7: note: declared here
       atomic(const atomic&) = delete;
       ^~~~~~
D:/msys64/mingw64/lib/gcc/x86_64-w64-mingw32/8.1.0/include/c++/atomic:672:17: note:   after user-defined conversion: 'constexpr std::atomic<int>::atomic(std::atomic<int>::__integral_type)'
       constexpr atomic(__integral_type __i) noexcept : __base_type(__i) { }
                 ^~~~~~
```

change 

```
atomic<int> i = 0;
```

into 

```
std::atomic_int i{0};
```

## error: expected ';' after top level declarator

compile on MacOS(-DUSE_ATOMIC):

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