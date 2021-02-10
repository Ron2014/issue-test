#include <iostream>
#include "IntegerPartition.h"
using namespace std;

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        cout << "usage: ./main [count]" << endl;
        return 0;
    }

    UINT count = atoi(argv[argc-1]);

    CIntegerPartition *pIP = new CIntegerPartition();

    pIP->Update(count);
    pIP->ShowResult();

    delete pIP;

    return 0;
}