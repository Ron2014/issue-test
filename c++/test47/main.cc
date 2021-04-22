/*
 * @Date: 2021-04-22 16:25:28
 * @Author: Ron
 * @LastEditors: Ron
 * @LastEditTime: 2021-04-22 16:53:53
 * @FilePath: \issue-test\c++\test47\main.cc
 * @Description: 
 */
#include <iostream>
#include <map>
using namespace std;

enum NUMS
{
    NUM0,
    NUM1,
    NUM2,
};

struct ITest
{
    virtual void func0(int a);
    virtual void func1(int a);
};

class A
{
typedef void (A:: *Method)();
public:
    A()
    {
        m = &A::foo;
    }

    void goo()
    {
        void *ptr = (void *)this;
        (ptr->*m)();
    }

private:
    void foo()
    {
        cout << "hello world" << endl;
    }

private:
    Method m;
};

typedef void (ITest:: *method)(int a);

int main(int argc, char *argvp[])
{
    map<NUMS, method> m;
    m[NUM0] = &ITest::func0;

    printf("%p\n", m[NUM0]);
    printf("%p\n", &ITest::func0);

    A a;
    a.goo();

    return 0;
}