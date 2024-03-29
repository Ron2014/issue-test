/*
 * @Date: 2021-07-17 20:55:30
 * @Author: Ron
 * @LastEditors: Ron
 * @LastEditTime: 2021-07-19 11:09:18
 * @FilePath: \issue-test\c++\test62\main.cc
 * @Description: 
 */
#include <iostream>
#include <stdio.h>
using namespace std;

///////////////////////////// 零散的协议
struct A1
{
    virtual void fooA0() = 0;
    virtual void fooA1() = 0;
    virtual void fooA2() = 0;
};

struct A2
{
    virtual void fooA3() = 0;
    virtual void fooA4() = 0;
    virtual void fooA5() = 0;
};

struct A3
{
    virtual void fooA6() = 0;
    virtual void fooA7() = 0;
    virtual void fooA8() = 0;
};

///////////////////////////// 协议合并在一起，用这个类来注册协议

/**
 * @brief 
 * 继承之后不定义虚函数
 * 那么自己的虚函数表是空的
 */
struct B1: A1, A2, A3
{
};

struct B2: A1, A2, A3
{
    virtual void fooA0() {};
    virtual void fooA1() {};
    virtual void fooA2() {};
    virtual void fooA3() {};
    virtual void fooA4() {};
    virtual void fooA5() {};
    virtual void fooA6() {};
    virtual void fooA7() {};
    virtual void fooA8() {};
};

///////////////////////////// 其他的类

template<typename OT, typename IT>
struct D1
{

};

struct E1
{
    virtual void fooE0() = 0;
    virtual void fooE1() = 0;
    virtual void fooE2() = 0;
};

struct E2
{
    virtual void fooE3() = 0;
    virtual void fooE4() = 0;
    virtual void fooE5() = 0;
};

struct E3
{
    virtual void fooE6() = 0;
    virtual void fooE7() = 0;
    virtual void fooE8() = 0;
};

struct F1
{
};

struct G1 : F1
{
    virtual void fooA0 () { printf("this is G1::E0\n"); }
    virtual void fooA1 () { printf("this is G1::E1\n"); }
    virtual void fooA2 () { printf("this is G1::E2\n"); }
    virtual void fooA3 () { printf("this is G1::E3\n"); }
    virtual void fooA4 () { printf("this is G1::E4\n"); }
    virtual void fooA5 () { printf("this is G1::E5\n"); }
    virtual void fooA6 () { printf("this is G1::E6\n"); }
    virtual void fooA7 () { printf("this is G1::E7\n"); }
    virtual void fooA8 () { printf("this is G1::E8\n"); }
};

///////////////////////////// Stub 协议的实现
struct C1: B1, D1<C1, B1>, G1
{
    virtual void fooA0 () { printf("this is C1::A0\n"); }
    virtual void fooA1 () { printf("this is C1::A1\n"); }
    virtual void fooA2 () { printf("this is C1::A2\n"); }
    virtual void fooA3 () { printf("this is C1::A3\n"); }
    virtual void fooA4 () { printf("this is C1::A4\n"); }
    virtual void fooA5 () { printf("this is C1::A5\n"); }
    virtual void fooA6 () { printf("this is C1::A6\n"); }
    virtual void fooA7 () { printf("this is C1::A7\n"); }
    virtual void fooA8 () { printf("this is C1::A8\n"); }

    virtual void fooB1 () { printf("this is C1::B1\n"); }
};

struct C2: B2, D1<C2, B2>, G1
{
    virtual void fooA0 () { printf("this is C2::A0\n"); }
    virtual void fooA1 () { printf("this is C2::A1\n"); }
    virtual void fooA2 () { printf("this is C2::A2\n"); }
    virtual void fooA3 () { printf("this is C2::A3\n"); }
    virtual void fooA4 () { printf("this is C2::A4\n"); }
    virtual void fooA5 () { printf("this is C2::A5\n"); }
    virtual void fooA6 () { printf("this is C2::A6\n"); }
    virtual void fooA7 () { printf("this is C2::A7\n"); }
    virtual void fooA8 () { printf("this is C2::A8\n"); }
};

// 协议的函数地址
typedef void (B1::*B1Method)();
typedef void (B2::*B2Method)();

// 调用函数
template<typename T>
void Invoke(T *obj, void (T::*method)())
{
    (((T *)obj)->*method)();
}

template<typename T>
void Invoke1(void *obj, void (T::*method)())
{
    (((T *)obj)->*method)();
}

class MethodProxy
{
private:
    typedef void (MethodProxy::*METHOD)();
    typedef bool (__cdecl *FUNC)(void*, METHOD);

    METHOD _RealMethod;
    FUNC _Invoker;

public:
    MethodProxy()
        : _Invoker(0)
        , _RealMethod(0)
    {
    }

    template <typename T>
    MethodProxy(void(T:: *method) ())
    {
        _RealMethod = (METHOD)method;
        _Invoker = (FUNC)MethodInvoker<T>;
    }

private:
    template <typename T>
    static bool __cdecl MethodInvoker(void *obj, METHOD method)
    {
        typedef void (T::*MethodType)();
        ((T *)obj->*(MethodType)method)();
        return true;
    }

public:
    bool operator()(void* obj)	{ return _Invoker(obj, _RealMethod); }
};

int main(int argc, char *argv[])
{
    C1 *pC1 = new C1;
    C2 *pC2 = new C2;

    cout << "--------------------1" << endl;

    (pC1->*(B1Method)(&B1::fooA0))();
    (pC1->*(B1Method)(&B1::fooA1))();
    (pC1->*(B1Method)(&B1::fooA2))();
    (pC1->*(B1Method)(&B1::fooA3))();
    (pC1->*(B1Method)(&B1::fooA4))();
    (pC1->*(B1Method)(&B1::fooA5))();
    (pC1->*(B1Method)(&B1::fooA6))();
    (pC1->*(B1Method)(&B1::fooA7))();
    (pC1->*(B1Method)(&B1::fooA8))();

    cout << "--------------------1" << endl;

    (pC2->*(B2Method)(&B2::fooA0))();
    (pC2->*(B2Method)(&B2::fooA1))();
    (pC2->*(B2Method)(&B2::fooA2))();
    (pC2->*(B2Method)(&B2::fooA3))();
    (pC2->*(B2Method)(&B2::fooA4))();
    (pC2->*(B2Method)(&B2::fooA5))();
    (pC2->*(B2Method)(&B2::fooA6))();
    (pC2->*(B2Method)(&B2::fooA7))();
    (pC2->*(B2Method)(&B2::fooA8))();

    cout << "--------------------2" << endl;
    
    Invoke<B1>(pC1, &B1::fooA0);
    Invoke<B1>(pC1, &B1::fooA1);
    Invoke<B1>(pC1, &B1::fooA2);
    Invoke<B1>(pC1, &B1::fooA3);
    Invoke<B1>(pC1, &B1::fooA4);
    Invoke<B1>(pC1, &B1::fooA5);
    Invoke<B1>(pC1, &B1::fooA6);
    Invoke<B1>(pC1, &B1::fooA7);
    Invoke<B1>(pC1, &B1::fooA8);

    cout << "--------------------2" << endl;
    
    Invoke<B2>(pC2, &B2::fooA0);
    Invoke<B2>(pC2, &B2::fooA1);
    Invoke<B2>(pC2, &B2::fooA2);
    Invoke<B2>(pC2, &B2::fooA3);
    Invoke<B2>(pC2, &B2::fooA4);
    Invoke<B2>(pC2, &B2::fooA5);
    Invoke<B2>(pC2, &B2::fooA6);
    Invoke<B2>(pC2, &B2::fooA7);
    Invoke<B2>(pC2, &B2::fooA8);

    cout << "--------------------3" << endl;

    Invoke1(pC1, &B1::fooA0);
    Invoke1(pC1, &B1::fooA1);
    Invoke1(pC1, &B1::fooA2);
    Invoke1(pC1, &B1::fooA3);
    Invoke1(pC1, &B1::fooA4);
    Invoke1(pC1, &B1::fooA5);
    Invoke1(pC1, &B1::fooA6);
    Invoke1(pC1, &B1::fooA7);

    cout << "--------------------3" << endl;

    Invoke1(pC2, &B2::fooA0);
    Invoke1(pC2, &B2::fooA1);
    Invoke1(pC2, &B2::fooA2);
    Invoke1(pC2, &B2::fooA3);
    Invoke1(pC2, &B2::fooA4);
    Invoke1(pC2, &B2::fooA5);
    Invoke1(pC2, &B2::fooA6);
    Invoke1(pC2, &B2::fooA7);

    cout << "--------------------4" << endl;

    MethodProxy m1(&B1::fooA0);
    MethodProxy m2(&B2::fooA0);
    m1(pC1);
    m2(pC2);

    delete pC1;
    delete pC2;
}