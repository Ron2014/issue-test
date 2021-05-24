/*
 * @Date: 2021-05-21 14:35:43
 * @Author: Ron
 * @LastEditors: Ron
 * @LastEditTime: 2021-05-21 17:11:25
 * @FilePath: \issue-test\c++\test50\main.cc
 * @Description: 
 */
#include <iostream>
#include <vector>
#include <functional>
#include <string>
using namespace std;
 
using std::function;
using std::bind;

typedef std::function<int(int, int)>   HandlerEvent;
// typedef std::function<int()> HandlerEvent1;
 
int func(int x,int y) {
    printf("nimei x:%d y:%d\n", x, y);
    return 3;
}

typedef int (*FUN)(const char *, ...);
void func1(FUN fp)
{
    fp("hello world\n");
}

class A
{
public:
    bool operator() ()
    {
        cout << "this is work" << endl;
        return true;
    }
};

static int foo0(const char *fmt)
{
    cout << fmt << endl;
    cout << "foo0 ok!" << endl;
    return 0;
}

template<typename V1>
static int foo1(const char *fmt, V1& v1)
{
    // printf(fmt, v1);
    cout << fmt << endl;
    cout << "foo1 ok!" << endl;
    printf("%p---\n", &v1);
    return 0;
}

template <typename V1, typename V2>
static int foo2(const char *fmt, V1& v1, V2& v2)
{
    cout << fmt << endl;
    cout << "foo2 ok!" << endl;
    return 0;
}

class B
{
public:
    template<typename T>
    B(void(T::*method)())
    {
        _invoker = (FUN)foo0;
    }

    template<typename T, typename V1>
    B(void(T::*method)(V1))
    {
        _invoker = (FUN)foo1<V1>;
    }

    template<typename T, typename V1, typename V2>
    B(void(T::*method)(V1, V2))
    {
        _invoker = (FUN)foo2<V1, V2>;
    }

    template<typename ...Args>
    bool operator() (const char *fmt, Args&&... args)
    {
        cout << fmt << endl;
        cout << sizeof...(args) << endl;
        // _invoker(fmt, args...);
        return true;
    }

    // bool operator() (const char *fmt, ...)
    // {
    //     _invoker(fmt, ...);
    //     return true;
    // }

private:
    FUN _invoker;
};

class st
{
public:
    st()
    {
        cout << "st crt" << endl;
    }
    ~st()
    {
        cout << "st des" << endl;
    }
};

struct RPC
{
    virtual void Call00() = 0;

    virtual void Call10(int) = 0;
    virtual void Call11(vector<int> &) = 0;
    virtual void Call12(vector<unsigned char> &) = 0;
    virtual void Call13(std::string &) = 0;
    virtual void Call14(st &v);


    virtual void Call20(int, int) = 0;
    virtual void Call21(const char*, int) = 0;
    virtual void Call22(unsigned char, vector<unsigned char> &) = 0;

    virtual void Call30(int, int, int) = 0;
};

int main() {
    printf("%d\n", bind<int>(&func, 3, 4)());

    HandlerEvent event = HandlerEvent(&func);
    event(5, 6);

    // HandlerEvent1 event1 = bind<int>(&func, std::placeholders::_1, std::placeholders::_2);
    // event1(7, 8);

    // 
    func1(printf);

    A a;
    a();

    B b(&RPC::Call21);
    int v1 = 1;
    b("%s %d\n", "abc", v1);

    vector<int> test = {1,2,3};
    B b1(&RPC::Call11);
    b1("Call11", test);

    vector<unsigned char> test1 = {1,2,3};
    printf("%p xxx\n", &test1);
    B b2(&RPC::Call12);
    b2("Call12", test1);

    unsigned char v = 4;
    B b3(&RPC::Call22);
    b3("Call22", v, test1);

    std::string str = "random string";
    B b4(&RPC::Call22);
    b4("Call22", str);

    st v2;
    printf("%p xxx\n", &v2);
    B b5(&RPC::Call14);
    b5("Call14", v2);

    B b6(&RPC::Call14);
    b6("Call14", st());

    return 0;
}