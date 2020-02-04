#include <stdio.h>
#include "part.h"

#ifdef _WIN32
#include <windows.h>
#define sleep(x) Sleep((x)*1000)
#else
#include <unistd.h>
#endif

int main(int argc, char *argv[]) {
    int i = 10;
    while(i--) {
        foo();
        sleep(1);
    }
    return 0;
}