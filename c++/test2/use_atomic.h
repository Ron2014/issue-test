#include <atomic>
using namespace std;
// atomic<int> i = 0;
std::atomic_int i{0};

void iplusplus() {
    int c = 10000000;  //循环次数
    while (c--) {
        i++;
    }
}