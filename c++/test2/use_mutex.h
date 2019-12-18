#include <mutex>
using namespace std;
int  i = 0;
mutex mut; //互斥锁

void iplusplus() {
    int c = 10000000;  //循环次数
    while (c--) {
        mut.lock();  //互斥锁加锁
        i++;
        mut.unlock(); //互斥锁解锁
    }
}