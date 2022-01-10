/*
 * @Date: 2021-09-22 21:00:53
 * @Author: Ron
 * @LastEditors: Ron
 * @LastEditTime: 2021-09-28 00:17:13
 * @Description: 
 */
#include <iostream>
#include <string>
#include <string.h>
using namespace std;

int main(int argc, char* argv[])
{
    const char msg[20] = "hello\0world";
    string str = string(msg, 20);
    cout << strlen(msg) << endl;
    cout << str.size() << endl;
    cout << str.length() << endl;
    printf("msg = %p\n", msg);
    printf("str = %p\n", str.data());

    str = "0000";
    int i = std::stoi(str);
    cout << i << endl;
    return 0;
}