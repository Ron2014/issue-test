/*
 * @Date: 2021-06-09 10:06:38
 * @Author: Ron
 * @LastEditors: Ron
 * @LastEditTime: 2021-06-10 15:15:03
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
    UINT32 Value1;          // 4
    UINT8 Value2;           // 1 -> 4
    EnumList Value3;        // 1 -> 4
    String Value4;          // 4
};

struct MyStruct2
{
    virtual void foo1() = 0;
    virtual void foo2() = 0;
    virtual void foo3() = 0;
    virtual void foo4() = 0;
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
        printf("Registed %p\n", field);
    }

    Field(V1 T::**field)
        : mField(field)
    {
        printf("Registed %p\n", field);
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
class ObjSerializer
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

class Method
{
public:
    Method()
    {
    }

    template <typename T>
    Method(void(T:: *method) ())
    {
        printf("Method %p\n", method);
    }
};

template <typename T>
class RPCSerializer
{
public:
    void Reg() {}
    template <typename V, typename... Args>
    void Reg(V v, Args &&...args)
    {
        mMethods.push_back(new Method(v)); 
        Reg(args...);
    }

    bool CallMethod(void* obj, int index)
    {
         if (index < mMethods.size())
         {
             // call mMethods[index]
             return true;
         }
         return false;
    }
private:
    std::vector<Method*> mMethods;
};


int main(int argc, char *argv[])
{
    ObjSerializer<MyStruct> serializer;
    serializer.Reg(
        &MyStruct::Value1,
        &MyStruct::Value2,
        &MyStruct::Value3,
        &MyStruct::Value4
    );

    RPCSerializer<MyStruct2> serializer2;
    serializer2.Reg(
        &MyStruct2::foo1,
        &MyStruct2::foo2,
        &MyStruct2::foo3,
        &MyStruct2::foo4
    );

    printf("sizeof MyStruct=%zd\n", sizeof(MyStruct));
    printf("sizeof MyStruct2=%zd\n", sizeof(MyStruct2));

    struct MyStruct obj;
    Stream stream;

    serializer.WriteToStream(&obj, stream);
    
    return 0;
}
