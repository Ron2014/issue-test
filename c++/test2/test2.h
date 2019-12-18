
#ifdef USE_MUTEX
    // 有锁
    #include "use_mutex.h"
#else
    #ifdef USE_ATOMIC
        // 原子操作
        #include "use_atomic.h"
    #else
        // 非线程安全
        #include "default.h"
    #endif
#endif
