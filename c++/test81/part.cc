#include "part.h"
#include "stdio.h"

int foo()
{
    const char *szTestInfo = "这里是中文";
    printf("%s\n", szTestInfo);
    return 0;
}