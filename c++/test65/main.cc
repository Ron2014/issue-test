/*
 * @Date: 2021-08-10 14:38:25
 * @Author: Ron
 * @LastEditors: Ron
 * @LastEditTime: 2021-08-11 22:20:29
 * @Description: 
 */
#ifdef __NO_PADDING__
#pragma pack(1)
#endif

#include <iostream>
using namespace std;

#define offsetof(s, m) ((size_t)((char*)(&((s*)0)->m)))
typedef unsigned int UINT32;

struct normal {
    int val0;
    int val1;
    int val2;
};

struct special {
    int val0;
    char val1;
    short val2;
};

struct Db_Serialize_Header
{
    bool bZip;        //是否压缩
    UINT32 nCrc32;    //CRC校验码
    UINT32 nVersion;  //版本号
    size_t nSize;     //数据块大小
    size_t nUnZip;    //压缩前大小

    Db_Serialize_Header()
    {
        this->bZip     = false;
        this->nCrc32   = 0;
        this->nVersion = 0;
        this->nSize    = 0;
        this->nUnZip   = 0;
    }
};

int main(int argc, char *argv[])
{
    cout << sizeof(normal) << endl;
    cout << sizeof(special) << endl;
    cout << sizeof(Db_Serialize_Header) << endl;

    normal n = {1, 2, 3};
    special s = {4, 5, 6};

    cout << offsetof(Db_Serialize_Header, bZip) << endl;        // 0
    cout << offsetof(Db_Serialize_Header, nCrc32) << endl;      // 4
    cout << offsetof(Db_Serialize_Header, nVersion) << endl;    // 8
    cout << offsetof(Db_Serialize_Header, nSize) << endl;       // 16
    cout << offsetof(Db_Serialize_Header, nUnZip) << endl;      // 24

    int val;
    for (int i=0; i<10000000; i++)
    {
        val = s.val0;
        val = s.val1;
        val = s.val2;
    }

    return 0;
}