/*
 * @Date: 2021-04-21 20:39:55
 * @Author: Ron
 * @LastEditors: Ron
 * @LastEditTime: 2021-04-21 20:57:35
 * @FilePath: \issue-test\c++\test46\main.cc
 * @Description: 
 */
#include <iostream>
#include <vector>
using namespace std;

typedef unsigned int UINT32;

#define LOG_ERR printf

static vector<string> sepstr(const string &sStr, const string &sSep)
{
    vector<string> vt;

    string::size_type pos = 0;
    string::size_type pos1 = 0;

    while (true)
    {
        string s;
        pos1 = sStr.find_first_of(sSep, pos);
        if (pos1 == string::npos)
        {
            if (pos + 1 <= sStr.length())
            {
                s = sStr.substr(pos);
            }
        }
        else if (pos1 == pos)
        {
            s = "";
        }
        else
        {
            s = sStr.substr(pos, pos1 - pos);
            pos = pos1;
        }

        if (!s.empty())
        {
            vt.push_back(s);
        }

        if (pos1 == string::npos)
        {
            break;
        }

        pos++;
    }

    return vt;
}

UINT32 IPAddress2Uint(string ipaddr)
{
    int bytes = 4;
    UINT32 ret = 0;

    vector<string> subKeys = sepstr(ipaddr, ".");
    if (subKeys.size() != 4)
    {
        LOG_ERR("IPAddress2Uint invalid ip address[%s]", ipaddr.c_str());
        return ret;
    }

    unsigned char *p = (unsigned char *)&ret;
    for (int i = 0; i < bytes; i++)
    {
        *(p + bytes - i - 1) = atoi(subKeys[i].c_str());
    }
    return ret;
}

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        cout << "usage: proc xxx.xxx.xxx.xxx" << endl;
    }

    for (int i = 1; i < argc; i++)
    {
        cout << argv[i] << " conver to " << IPAddress2Uint(argv[i]) << endl;
    }

    return 0;
}