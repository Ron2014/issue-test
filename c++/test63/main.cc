/*
 * @Date: 2021-07-27 20:31:31
 * @Author: Ron
 * @LastEditors: Ron
 * @LastEditTime: 2021-07-27 20:37:24
 * @FilePath: \issue-test\c++\test63\main.cc
 * @Description: 
 */
#include <iostream>
using namespace std;

#define TEST(...) Test::foo(__VA_ARGS__)

struct Test
{
    static void foo()
    {
        cout << "foo0" << endl;
    }

    static void foo(int v1)
    {
        cout << "foo1:" << v1 << endl;
    }

    static void foo(int v1, int v2)
    {
        cout << "foo2:" << v1 << " " << v2 << endl;
    }
};

int main(int argc, const char *argv[])
{
    TEST();
    TEST(1);
    TEST(2,3);
    return 0;
}