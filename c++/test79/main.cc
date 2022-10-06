#include <iostream>
#include <ctime>
#include <sys/time.h>
using namespace std;

uint64_t GetTimeUS()
{
    static struct timeval stTime;
    gettimeofday(&stTime, NULL);
    return stTime.tv_sec * 1000000 + stTime.tv_usec;
}

int main(int argc, char *argv[])
{
    struct timeval t;
    printf("sizeof(timeval::tv_sec)=%zd\n", sizeof(t.tv_sec));
    return 0;
}