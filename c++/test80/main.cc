#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include <ctime>
#include <sys/time.h>
using namespace std;

typedef unsigned long long uint64_t;

uint64_t GetTimeUS()
{
    static struct timeval stTime;
    gettimeofday(&stTime, NULL);
    return stTime.tv_sec * 1000000 + stTime.tv_usec;
}

// 多分隔符，C++
int splitString1(const string &str, const char *delimiters, vector<string> &tokens)
{
    size_t start = 0, end = 0;
    while ((end = str.find_first_of(delimiters, start)) != string::npos)
    {
        if (end != start)
        {
            tokens.push_back(str.substr(start, end - start));
        }
        start = end + 1;
    }
    if (start < str.length())
    {
        tokens.push_back(str.substr(start));
    }
    return 0;
}

// 多分隔符，C++
int splitString12(const string &str, const string &delimiters, vector<string> &tokens)
{
    size_t start = 0, end = 0;
    while ((end = str.find_first_of(delimiters, start)) != string::npos)
    {
        if (end != start)
        {
            tokens.push_back(str.substr(start, end - start));
        }
        start = end + 1;
    }
    if (start < str.length())
    {
        tokens.push_back(str.substr(start));
    }
    return 0;
}

// 多分隔符，C，会破坏原始字符串
int splitString2(char *str, const char *delimiter, vector<string> &result)
{
    char *token = strtok(str, delimiter);
    while (token != NULL)
    {
        result.push_back(token);
        token = strtok(NULL, delimiter);
    }
    return 0;
}

int splitString2(const string &str, const char *delimiter, vector<string> &result)
{
    static char input[2048];
    strncpy(input, str.c_str(), str.length());
    char *token = strtok(input, delimiter);
    while (token != NULL)
    {
        result.push_back(token);
        token = strtok(NULL, delimiter);
    }
    return 0;
}

// 单字符分割，C++
int splitString3(const string &str, const char delimiter, vector<string> &result)
{
    stringstream ss(str);
    string substr;
    while (getline(ss, substr, delimiter))
    {
        result.push_back(substr);
    }
    return 0;
}

// 字符串分隔符，C++
int splitString4(const string &input, const string &delimiter, vector<string> &result)
{
    size_t start = 0, end = 0;
    while ((end = input.find(delimiter, start)) != std::string::npos)
    {
        result.push_back(input.substr(start, end - start));
        start = end + delimiter.length();
    }
    result.push_back(input.substr(start));
    return 0;
}

int showResult(vector<string> &result)
{
    for(vector<string>::iterator it = result.begin(); it != result.end(); ++it)
    {
        cout << it->c_str() << endl;
    }
    return 0;
}

int main()
{
    string str = "This is a long string,    separated by multiple delimiters; let's split it into pieces!";
    string delimiters = " ,;!'";
    vector<string> result;
    uint64_t start;

    // 有效性测试

    // {
    //     cout << "************" << endl;
    //     result.clear();
    //     splitString1(str, delimiters.c_str(), result);
    //     cout << str.c_str() << endl;
    //     showResult(result);
    //     cout << result.size() << endl;

    //     cout << "************" << endl;
    //     result.clear();
    //     splitString2(str, (char *)delimiters.c_str(), result);
    //     cout << str.c_str() << endl;
    //     showResult(result);
    //     cout << result.size() << endl;

    //     cout << "************" << endl;
    //     result.clear();
    //     splitString3(str, delimiters.c_str()[0], result);
    //     cout << str.c_str() << endl;
    //     showResult(result);
    //     cout << result.size() << endl;

    //     cout << "************" << endl;
    //     result.clear();
    //     splitString4(str, delimiters.substr(0, 1), result);
    //     cout << str.c_str() << endl;
    //     showResult(result);
    //     cout << result.size() << endl;
    // }

    // 效率测试

    // 多字节分割

    {

        cout << "************" << endl;
        start = GetTimeUS();
        for (int i = 0; i < 1000000; i++)
        {
            // str = "This is a long string, separated by multiple delimiters; let's split it into pieces!";
            result.clear();
            splitString1(str, delimiters.c_str(), result);
        }
        cout << GetTimeUS() - start << endl;

        cout << "************" << endl;
        start = GetTimeUS();
        for (int i = 0; i < 1000000; i++)
        {
            // str = "This is a long string, separated by multiple delimiters; let's split it into pieces!";
            result.clear();
            splitString12(str, delimiters, result);
        }
        cout << GetTimeUS() - start << endl;

        cout << "************" << endl;
        start = GetTimeUS();
        for (int i = 0; i < 1000000; i++)
        {
            // str = "This is a long string, separated by multiple delimiters; let's split it into pieces!";
            result.clear();
            // splitString2((char *)str.c_str(), (char *)delimiters.c_str(), result);
            splitString2(str, (char *)delimiters.c_str(), result);
        }
        cout << GetTimeUS() - start << endl;
    }

    // // 单字节分割

    // {
    //     string strFrist = delimiters.substr(0, 1);

    //     cout << "************" << endl;
    //     start = GetTimeUS();
    //     for (int i = 0; i < 1000000; i++)
    //     {
    //         // str = "This is a long string, separated by multiple delimiters; let's split it into pieces!";
    //         result.clear();
    //         splitString1(str, strFrist.c_str(), result);
    //     }
    //     cout << GetTimeUS() - start << endl;

    //     cout << "************" << endl;
    //     start = GetTimeUS();
    //     for (int i = 0; i < 1000000; i++)
    //     {
    //         // str = "This is a long string, separated by multiple delimiters; let's split it into pieces!";
    //         result.clear();
    //         // splitString2((char *)str.c_str(), (char *)strFrist.c_str(), result);
    //         splitString2(str, (char *)strFrist.c_str(), result);
    //     }
    //     cout << GetTimeUS() - start << endl;

    //     cout << "************" << endl;
    //     start = GetTimeUS();
    //     for (int i = 0; i < 1000000; i++)
    //     {
    //         // str = "This is a long string, separated by multiple delimiters; let's split it into pieces!";
    //         result.clear();
    //         splitString3(str, strFrist.c_str()[0], result);
    //     }
    //     cout << GetTimeUS() - start << endl;
    // }

    return 0;
}
