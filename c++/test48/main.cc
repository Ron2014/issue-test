/*
 * @Date: 2021-04-27 11:30:26
 * @Author: Ron
 * @LastEditors: Ron
 * @LastEditTime: 2021-04-27 11:52:49
 * @FilePath: \issue-test\c++\test48\main.cc
 * @Description: 
 */
#include <iostream>
using namespace std;

typedef unsigned char UINT8;
typedef unsigned int UINT32;
typedef int INT32;

enum ENUM1
{
    E11,
    E12,
    E13,
    E14,
};

enum ENUM2 : UINT8
{
    E2_LONGNAME_1,
    E2_LONGNAME_2,
    E2_LONGNAME_3,
    E2_LONGNAME_4,
};

class A
{
public:
    static void Test(UINT8 a)
    {
        cout << "UINT8" << endl;
    }
    static void Test(INT32 a)
    {
        cout << "INT32" << endl;
    }
    static void Test(UINT32 a)
    {
        cout << "unsigned INT32" << endl;
    }
};

int main(int argc, char *argv[])
{
    enum ENUM1 e1;
    enum ENUM2 e2;
    printf("sizeof ENUM1:%zd ENUM2:%zd \n", sizeof(e1), sizeof(e2));

    A::Test(e1);
    A::Test(e2);
    A::Test(ENUM2::E2_LONGNAME_1);
    return 0;
}