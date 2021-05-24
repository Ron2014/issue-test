/*
 * @Date: 2021-04-29 11:12:50
 * @Author: Ron
 * @LastEditors: Ron
 * @LastEditTime: 2021-05-14 11:20:31
 * @FilePath: \issue-test\c++\test49\main.cc
 * @Description: 
 */
#include <iostream>
#include <string>
#include <vector>
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
        printf("------\n");
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
        printf("------\n");
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
        printf("------\n");
    }

    template <typename V, typename... Args>
    static void foo(const V &v, Args&&... args)
    {
        printf("C:foo: %p: %d\n", &v, v);
        foo(args...);
    }
};

struct D
{
    static void foo()
    {
        printf("------\n");
    }

    template <typename V, typename... Args>
    static void foo(V v, Args&&... args)
    {
        printf("D:foo: %p: %d\n", &v, v);
        foo(args...);
    }
};

struct E
{
    static void foo()
    {
        printf("------\n");
    }

    template <typename V, typename... Args>
    static void foo(V &v, Args&&... args)
    {
        printf("E:foo: %p: %d\n", &v, v);
        foo(args...);
    }
};

struct STR
{
    int a;
    char b;
    std::string c;
};

int main(int argc, char *argv[])
{
    int a = 1;
    int b = 2;
    int c = 3;
    STR d;
    string e = "abc";
    vector<int> f;
    vector<STR> g;

    printf("main: %p: %d\n", &a, a);
    printf("main: %p: %d\n", &b, b);
    printf("main: %p: %d\n", &c, c);
    printf("main: %p: %d\n", &d, sizeof(d));
    printf("main: %p: %d\n", &e, sizeof(e));
    printf("main: %p: %d\n", &f, sizeof(f));
    printf("main: %p: %d\n", &g, sizeof(g));

    foo0(a);
    foo0(0);
    
    foo1(a);
    // foo1(0);    //cannot bind non-const lvalue reference of type ‘int&’ to an rvalue of type ‘int’

    // foo2(a);    //cannot bind rvalue reference of type ‘int&&’ to lvalue of type ‘int’
    foo2(0);

    A::foo(0, b, c, d, e, f, g);
    A::foo(a, 0, c, d, e, f, g);
    A::foo(a, b, 0, d, e, f, g);
    A::foo(a, b, c, 0, e, f, g);
    A::foo(a, b, c, d, "abc", f, g);

    // B::foo(a, 0, c);    //cannot bind non-const lvalue reference of type ‘int&’ to an rvalue of type ‘int’
    // B::foo(a, b, 0);
    // B::foo(0, b, c);
    B::foo(a, b, c, d, e, f, g);

    C::foo(0, b, c, d, e, f, g);
    C::foo(a, 0, c, d, e, f, g);
    C::foo(a, b, 0, d, e, f, g);
    C::foo(a, b, c, 0, e, f, g);
    C::foo(a, b, c, d, "abc", f, g);

    D::foo(0, b, c, d, e, f, g);
    D::foo(a, 0, c, d, e, f, g);
    D::foo(a, b, 0, d, e, f, g);
    D::foo(a, b, c, 0, e, f, g);
    D::foo(a, b, c, d, "abc", f, g);

    // E::foo(0, b, c, d, e, f, g);
    E::foo(a, 0, c, d, e, f, g);
    E::foo(a, b, 0, d, e, f, g);
    E::foo(a, b, c, 0, e, f, g);
    E::foo(a, b, c, d, "abc", f, g);

    return 0;
}