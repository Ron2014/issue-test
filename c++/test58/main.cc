/*
 * @Date: 2021-07-06 11:56:16
 * @Author: Ron
 * @LastEditors: Ron
 * @LastEditTime: 2021-07-06 17:41:24
 * @FilePath: \issue-test\c++\test58\main.cc
 * @Description: 
 */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <memory>
using namespace std;

#define UINT8 unsigned char
#define UINT32 unsigned int
#define UINT64 unsigned long long

class A
{
public:
    UINT8 a1;
    UINT32 a2;
    UINT64 a3;

    void Show()
    {
        printf("A.a1 = %d\n", a1);
        printf("A.a2 = %d\n", a2);
        printf("A.a3 = %d\n", a3);
    }
};

struct B
{
    UINT8 b1;
    UINT32 b2;
    UINT64 b3;

    void Show()
    {
        printf("B.b1 = %d\n", b1);
        printf("B.b2 = %d\n", b2);
        printf("B.b3 = %d\n", b3);
    }
};

struct C
{
};

struct D
{
    D()
    {
        printf("D ctor!\n");
    }
};

int main(int argc, char *argv[])
{
    A a; B b;
    a.Show();
    b.Show();

    A *pa = new A();
    B *pb = new B();
    pa->Show();
    pb->Show();

    A *ptrA = (A *)malloc(sizeof(A));
    B *ptrB = (B *)malloc(sizeof(B));
    ptrA->Show();
    ptrB->Show();

    C c;
    D d;

    delete pa;
    delete pb;
    free(ptrA);
    free(ptrB);
    return 0;
}