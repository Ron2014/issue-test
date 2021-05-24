/*
 * @Date: 2021-05-24 15:02:43
 * @Author: Ron
 * @LastEditors: Ron
 * @LastEditTime: 2021-05-24 19:42:52
 * @FilePath: \issue-test\c++\test51\main.cc
 * @Description: 
 */
#include <stdio.h>
#include <iostream>
#include <string.h>
using namespace std;

class B
{
public:
    B()
    {
        cout << "B ctor" << endl;
    }

    ~B()
    {
        cout << "B free" << endl;
    }
};

class A
{
public:
    A()
    {
        cout << "A ctor" << endl;
        pObj = new B();
    }

    ~A()
    {
        cout << "A free" << endl;
        delete pObj;
    }

private:
    B* pObj;
};

int main(int argc, char * argv[])
{
    A *srcA = new A();      // 已经创建好的对象
    printf("address of A is %p\n", srcA);

    A *ptrA = NULL;
    if (argc == 2 && strcmp(argv[1], "start") == 0)
    {
        // ptrA = srcA;
        A objA = *srcA;   // 编译器这里不报错？不会

        // 这里会析构？
    }

    printf("address of A is %p\n", srcA);
    return 0;
}