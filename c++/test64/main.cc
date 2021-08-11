/*
 * @Date: 2021-08-05 10:28:04
 * @Author: Ron
 * @LastEditors: Ron
 * @LastEditTime: 2021-08-05 10:33:17
 * @Description: 
 */
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

const char *Test(const char *msg)
{
    stringstream ss;
    ss << "HELLO " << msg;
    return ss.str().c_str();
}


int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "usage: proc msgmsgmsg..." << endl;
        return 1;
    }

    std::string msg = Test(argv[1]);
    cout << msg << endl;
    
    return 0;
}