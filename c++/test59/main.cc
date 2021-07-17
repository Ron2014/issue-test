/*
 * @Date: 2021-07-07 16:06:57
 * @Author: Ron
 * @LastEditors: Ron
 * @LastEditTime: 2021-07-07 22:44:57
 * @FilePath: \issue-test\c++\test59\main.cc
 * @Description: 
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <typeinfo>
#include <stdio.h>
using namespace std;

struct A
{
    string m1;
    vector<int> m2;
    vector<unsigned char> m3;
    map<int, string> m4;
    map<unsigned char, string> m5;
};

template<typename T>
void ShowSize(T& t)
{
    printf("sizeof %s: %lu\n", typeid(T).name(), sizeof(t));
    printf("sizeof %s: %lu\n", typeid(t.m1).name(), t.m1.capacity());
    printf("sizeof %s: %lu\n", typeid(t.m2).name(), t.m2.capacity());
    printf("sizeof %s: %lu\n", typeid(t.m3).name(), t.m3.capacity());
    printf("sizeof %s: %lu\n", typeid(t.m4).name(), t.m4.size());
    printf("sizeof %s: %lu\n", typeid(t.m5).name(), t.m5.size());
}

int main(int argc, char *argv[])
{
    A a;
    ShowSize<A>(a);
    a.m1 = "hello world";
    ShowSize<A>(a);
    a.m1.clear();
    ShowSize<A>(a);

    a.m2.push_back(1);
    a.m2.push_back(2);
    a.m2.push_back(3);
    ShowSize<A>(a);

    a.m3.push_back(1);
    a.m3.push_back(2);
    a.m3.push_back(3);
    a.m3.push_back(4);
    a.m3.push_back(5);
    ShowSize<A>(a);

    a.m2.clear();
    a.m3.clear();
    ShowSize<A>(a);

    a.m4.insert(make_pair(0, "hello"));
    a.m4.insert(make_pair(1, "world"));

    a.m5.insert(make_pair(0, "hello"));
    a.m5.insert(make_pair(1, "world"));
    ShowSize<A>(a);

    printf("%d\n", sizeof(size_t));
    printf("%d\n", sizeof(unsigned long));
    return 0;
}