#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <map>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

map<string, string> mapResult;
#define SIZE_BUF (1024*1024)
char buffer[SIZE_BUF];

char *getKey(int m, int n)
{
    // M->N
    // M个物体用N个槽分
    snprintf(buffer, sizeof(buffer), "%d->%d", m, n);
    return buffer;
}

void checkMemory(const char *key)
{
    string &val = mapResult[key];
    if (val.length() >= SIZE_BUF)
    {
        printf("key:%s out of memory\n", key);
        exit(-1);
    }
}

void printResult(int m, int n)
{
    std::string::size_type pos, pos0, pos1, prev_pos0, prev_pos1;
    stringstream ss;

    for (int i = 1; i <= m; i++)
    {
        // 任意i给1分，只有一种结果 i:1
        string key = getKey(i, 1);
        snprintf(buffer, sizeof(buffer), "%d:%d \n", i, 1);
        mapResult[key] = buffer;
    }

    for (int j = 1; j <= n; j++)
    {
        // 1给任意j分，只有一种结果 1:1
        string key = getKey(1, j);
        snprintf(buffer, sizeof(buffer), "%d:%d \n", 1, 1);
        mapResult[key] = buffer;
    }

    for (int i = 2; i <= m; i++)
    {
        for (int j = 2; j <= n; j ++)
        {
            string key = getKey(i, j);
            if (i < j)
            {
                string oldKey = getKey(i, j-1);
                mapResult[key] = mapResult[oldKey];
            }
            else if (i == j)
            {
                // 至少有一个空的情况
                string oldKey = getKey(i, j-1);
                string &oneEmpty = mapResult[oldKey];

                // 铺满j的情况，就是 1:j
                snprintf(buffer, sizeof(buffer), "%s%d:%d \n", oneEmpty.c_str(), 1, j);
                mapResult[key] = buffer;
            }
            else // i > j
            {
                // 至少有一个空的情况
                string oldKey = getKey(i, j-1);
                string &oneEmpty = mapResult[oldKey];

                ss.str("");
                ss << oneEmpty;

                // 铺满j的情况
                oldKey = getKey(i-j, j);
                string &leftResult = mapResult[oldKey];

                prev_pos0 = 0;
                while( (pos0 = leftResult.find('\n', prev_pos0 + 1)) != std::string::npos)
                {
                    string line = leftResult.substr(prev_pos0, pos0 - prev_pos0);

                    int sum = 0;
                    prev_pos1 = 0;
                    while( (pos1 = line.find(' ', prev_pos1 + 1)) != std::string::npos)
                    {
                        string count = line.substr(prev_pos1, pos1 - prev_pos1);
                        pos = count.find(':', 0);

                        long elem = strtol(count.substr(0, pos).c_str(), NULL, 10);
                        long num = strtol(count.substr(pos+1, count.length() - pos - 1).c_str(), NULL, 10);
                        ss << elem+1 << ":" << num << " ";
                        sum += num;
                        prev_pos1 = pos1;
                    }

                    if (j - sum > 0)
                    {
                        ss << 1 << ":" << j - sum << " \n";
                    }
                    else
                    {
                        ss << "\n";
                    }
                    prev_pos0 = pos0;
                }
                mapResult[key] = ss.str().c_str();
            }
            checkMemory(key.c_str());
        }
    }
    
    string key = getKey(m, n);
    std::cout << key << "======\n" << mapResult[key];
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("usage: proc M N\n");
        return -1;
    }

    int m = atoi(argv[1]);
    int n = atoi(argv[2]);
    printResult(m, n);

    return 0;
}