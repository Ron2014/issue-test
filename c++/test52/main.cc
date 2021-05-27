/*
 * @Date: 2021-05-27 16:17:52
 * @Author: Ron
 * @LastEditors: Ron
 * @LastEditTime: 2021-05-27 16:25:47
 * @FilePath: \issue-test\c++\test52\main.cc
 * @Description: 
 */
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main(int argc, char *argv[])
{
    vector<int> arr;
    vector<int>::iterator it = std::find(arr.begin(), arr.end(), 0);
    arr.erase(it);
    cout << "it's ok!" << endl;
    return 0;
}