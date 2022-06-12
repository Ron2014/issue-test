#include <iostream>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cxxabi.h>

using namespace std;

#define SIZE 100

void myfunc3(void)
{
    int j, nptrs;
    void *buffer[100];
    char **strings;

    nptrs = backtrace(buffer, SIZE);
    printf("backtrace() returned %d addresses\n", nptrs);

    /* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
       would produce similar output to the following: */

    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL)
    {
        perror("backtrace_symbols");
        exit(EXIT_FAILURE);
    }

    for (j = 0; j < nptrs; j++)
        printf("%s\n", strings[j]);

    char buff[SIZE];
    size_t buff_size;
    char *ret;
    int stat = 0;
    for (j = 0; j < nptrs; j++)
    {
        ret = abi::__cxa_demangle(strings[j], buff, &buff_size, &stat);

        if (stat == 0 && ret)
        {
            printf("%s\n", ret);
            printf("[%zd]%s", buff_size, buff);
            free(ret);
        }
        else
        {
            printf("error %d\n", stat);
        }
    }

    free(strings);
}

/* "static" means don't export the symbol... */
static void myfunc2(void)
{
    myfunc3();
}

void myfunc(int ncalls)
{
    if (ncalls > 1)
        myfunc(ncalls - 1);
    else
        myfunc2();
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "%s num-calls\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    myfunc(atoi(argv[1]));
    exit(EXIT_SUCCESS);
}