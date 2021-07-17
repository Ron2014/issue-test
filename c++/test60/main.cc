/*
 * @Date: 2021-07-08 13:03:38
 * @Author: Ron
 * @LastEditors: Ron
 * @LastEditTime: 2021-07-12 14:52:50
 * @FilePath: \issue-test\c++\test60\main.cc
 * @Description: 
 */

#include <limits>
#include <iostream>
#include <map>
#include <vector>
#include <cstring>
using namespace std;

#define INFO(type)                                                                                            \
    {                                                                                                         \
        type min = (numeric_limits<type>::min)();                                                             \
        type max = (numeric_limits<type>::max)();                                                             \
        cout << #type << " info: \n\tsize(" << sizeof(type) << ") \n\tmin(" << min << ") \n\tmax(" << max << ")" << endl; \
        /* printf("%s info: size(%zd) min(%zd) max(%zd)", #type, sizeof(type), min, max); */                  \
    }

std::map<int, std::string> datas;

struct B
{
    int mM0 = 100;
};

struct A {
    B mMB;
    // std::string mM0;
    // std::map<int, int> mM1;
    // std::vector<int> mM2;
};

int main(int argc, char *argv[])
{
    INFO(char)
    INFO(unsigned char)

    INFO(wchar_t)

    INFO(short)
    INFO(unsigned short)

    INFO(int)
    INFO(unsigned int)
    
    INFO(long)
    INFO(unsigned long)

    INFO(long long)
    INFO(unsigned long long)

    INFO(float)
    INFO(double)

    datas.insert(std::make_pair(1, std::move(std::string("hello"))));
    datas.insert(std::make_pair(1, std::move(std::string("world"))));
    cout << datas[1] << endl;

    A a;
    // memset(&a, 0, sizeof(A));

    cout << "----" << endl;

    // a.mMB.mM0 = 10;

    // a.mM0 = "hello";
    // a.mM0 = "world";

    // a.mM1.insert(std::make_pair(1,1));
    // a.mM1.insert(std::make_pair(2,2));

    // a.mM2.push_back(3);
    // a.mM2.push_back(4);

    cout << "----" << endl;

    printf("B.mM0[%d]\n", a.mMB.mM0);

    // printf("string[%s]\n", a.mM0.c_str());

    // for (auto pair : a.mM1)
    // {
    //     printf("map[%d,%d]\n", pair.first, pair.second);
    // }

    // for (auto val : a.mM2)
    // {
    //     printf("vector[%d]\n", val);
    // }
    return 0;
}