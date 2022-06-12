/*
 * @Date: 2021-08-31 13:51:20
 * @Author: Ron
 * @LastEditors: Ron
 * @LastEditTime: 2021-09-05 17:46:46
 * @Description: 
 */
#include <iostream>
#include <thread>
#include <chrono>
#include "test2.h"

using namespace std;

#define VALUE_I i
void iplusplus();

int main()
{
    chrono::steady_clock::time_point start_time = chrono::steady_clock::now();//开始时间

    thread thread1(iplusplus);  //建立并运行线程1
    thread thread2(iplusplus);  //建立并运行线程2
    thread1.join();  // 等待线程1运行完毕
    thread2.join();  // 等待线程2运行完毕
    cout << "i = " << i << endl;

    chrono::steady_clock::time_point stop_time = chrono::steady_clock::now();//结束时间
    chrono::duration<double> time_span = chrono::duration_cast<chrono::microseconds>(stop_time - start_time);
    std::cout << "共耗时：" << time_span.count() << " ms" << endl; // 耗时

    return 0;
}