/*
 * @Date: 2021-04-29 11:12:50
 * @Author: Ron
 * @LastEditors: Ron
 * @LastEditTime: 2021-04-29 11:38:05
 * @FilePath: \issue-test\c++\test49\main.cc
 * @Description: 
 */
#include <iostream>
using namespace std;


void foo0(int a)
{
    printf("foo0: %p: %d\n", &a, a);
}

void foo1(int &a)
{
    printf("foo1: %p: %d\n", &a, a);
}

void foo2(int &&a)
{
    printf("foo2: %p: %d\n", &a, a);
}

struct A
{
    static void foo()
    {
    }

    template <typename V, typename... Args>
    static void foo(const V &v, Args... args)
    {
        printf("A:foo: %p: %d\n", &v, v);
        foo(args...);
    }
};

struct B
{
    static void foo()
    {
    }

    template <typename V, typename... Args>
    static void foo(const V &v, Args&... args)
    {
        printf("B:foo: %p: %d\n", &v, v);
        foo(args...);
    }
};

struct C
{
    static void foo()
    {
    }

    template <typename V, typename... Args>
    static void foo(const V &v, Args&&... args)
    {
        printf("C:foo: %p: %d\n", &v, v);
        foo(args...);
    }
};

int main(int argc, char *argv[])
{
    int a = 1;
    int b = 2;
    int c = 3;

    printf("main: %p: %d\n", &a, a);
    printf("main: %p: %d\n", &b, b);
    printf("main: %p: %d\n", &c, c);

    foo0(a);
    foo0(0);
    
    foo1(a);
    // foo1(0);    //cannot bind non-const lvalue reference of type ‘int&’ to an rvalue of type ‘int’

    // foo2(a);    //cannot bind rvalue reference of type ‘int&&’ to lvalue of type ‘int’
    foo2(0);

    A::foo(a, 0, c);
    A::foo(a, b, 0);
    A::foo(0, b, c);

    // B::foo(a, 0, c);    //cannot bind non-const lvalue reference of type ‘int&’ to an rvalue of type ‘int’
    // B::foo(a, b, 0);
    // B::foo(0, b, c);
    B::foo(a, b, c);

    C::foo(a, 0, c);
    C::foo(a, b, 0);
    C::foo(0, b, c);

    return 0;
}