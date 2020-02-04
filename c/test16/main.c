#include <stdio.h>
#include <time.h>
#include "part.h"

#ifdef _WIN32
#include <windows.h>
#define sleep(x) Sleep((x)*1000)
#else
#include <unistd.h>
#endif

int main(int argc, char *argv[]) {
    clock_t t1, t2;
    t1 = clock();

    int i = 1000;
    while(i--) {
        foo();
    }

    t2 = clock();
    printf("%.10f\n", (double)(t2-t1)/CLOCKS_PER_SEC);
    return 0;
}