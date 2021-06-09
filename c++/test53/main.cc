/*
 * @Date: 2021-06-09 10:06:38
 * @Author: Ron
 * @LastEditors: Ron
 * @LastEditTime: 2021-06-09 11:29:58
 * @FilePath: \issue-test\c++\test53\main.cc
 * @Description: 
 */

#include <iostream>
#include <vector>
#include <string>
using namespace std;

typedef unsigned char UINT8;
typedef std::string String;
typedef unsigned int UINT32;

namespace Protocol
{
    enum EnumList : UINT8
    {
        ENUM1,
        ENUM2,
        ENUM3,
    };
}

using namespace Protocol;

struct MyStruct
{
    UINT32 Value1;
    UINT8 Value2;
    EnumList Value3;
    String Value4;
};

class Stream
{
public:
    template <typename T>
    Stream &operator<<(const T &t)
    {
        cout << "this is work! " << sizeof(t) << endl;
        return *this;
    };

    // template <>
    inline Stream &operator<<(const unsigned char &v)
    {
        cout << "this is work! unsigned char " << sizeof(v) << endl;
        return *this;
    }

    // template <>
    inline Stream &operator<<(const unsigned int &v)
    {
        cout << "this is work! unsigned int " << sizeof(v) << endl;
        return *this;
    }

    // template <>
    inline Stream &operator<<(const std::string &v)
    {
        cout << "this is work! std::string " << v.size() << endl;
        return *this;
    }
};

class Field_Base
{
public:
    virtual bool WriteToStream(const void *obj, Stream &stream) { return true;};
};

template <typename T, typename V1>
class Field: public Field_Base
{
public:
    Field(V1 T::*field)
        : mField(field)
    {
    }

    Field(V1 T::**field)
        : mField(field)
    {
    }

    bool WriteToStream(const void* obj, Stream &stream)
    {
        T *t = (T *)obj;
        stream << (t->*mField);
        return true;
    };

private:
    V1 T::*mField;
};

template <typename T>
class ObjectSerializer
{
public:
    void Reg() {}
    template <typename V, typename... Args>
    void Reg(V T::*field, Args &&...args)
    {
        mFields.push_back(new Field<T, V>(field)); 
        Reg(args...);
    }

    bool WriteToStream(const void *obj, Stream &stream)
    {
        typename std::vector<Field_Base*>::iterator iter = mFields.begin();
        for (; iter != mFields.end(); ++iter)
        {
            Field_Base *pBase = *iter;
            pBase->WriteToStream(obj, stream);
        }
        return true;
    }

private:
    std::vector<Field_Base*> mFields;
};

int main(int argc, char *argv[])
{
    ObjectSerializer<MyStruct> serializer;
    serializer.Reg(
        &MyStruct::Value1,
        &MyStruct::Value2,
        &MyStruct::Value3,
        &MyStruct::Value4
    );

    struct MyStruct obj;
    Stream stream;

    serializer.WriteToStream(&obj, stream);
    
    return 0;
}
