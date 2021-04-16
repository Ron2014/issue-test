/*
 * @Author: X.Q Chen 465276914@im.com
 * @LastEditors: Ron
 * @FilePath: \issue-test\c++\test45\MyStr.h
 */
#ifndef _MY_STR_H_
#define _MY_STR_H_

#include <string.h>
#include <string>

enum CMyStr_Size : int
{
    CMyStr_Size_32          =32,
    CMyStr_Size_64          =64,
    CMyStr_Size_512         =512
};

template <CMyStr_Size _Size>
class CMyStr
{
    public :
        CMyStr();
        CMyStr(const char * str);
        CMyStr(const std::string & str);
        CMyStr(const CMyStr<_Size> & str);

        ~CMyStr();

        const char * c_str() const;
        int size() const;
        int max_size() const;
        bool empty() const;

        char * raw_data()
        {
            return m_Data;
        }

        void clear();

        CMyStr<_Size> & operator = (const CMyStr<_Size> & str);
        CMyStr<_Size> & operator = (const std::string & str);

        bool operator == (const CMyStr<_Size> & str) const;
        bool operator != (const CMyStr<_Size> & str) const;

        bool operator == (const std::string & str) const;
        bool operator != (const std::string & str) const;

        bool operator > ( const CMyStr<_Size> & str ) const;
        bool operator < ( const CMyStr<_Size> & str ) const;

    private :
        char m_Data[_Size];
};

template <CMyStr_Size _Size>
CMyStr<_Size>::CMyStr()
{
    // 因为要在共享内存中使用，所以这个值不能重置 
    // m_Data[0] = '\0';
}

template <CMyStr_Size _Size>
CMyStr<_Size>::CMyStr(const char * str)
{
    if (NULL != str)
    {
        const int len = strlen(str);
        if (len < _Size)
        {
            strncpy(m_Data, str, len);
            m_Data[len] = '\0';
        }
        else
        {
            // @TODO derri expection
        }
    }
}

template <CMyStr_Size _Size>
CMyStr<_Size>::CMyStr(const std::string & str)
{
    new (this) CMyStr(str.c_str());
}

template <CMyStr_Size _Size>
CMyStr<_Size>::CMyStr(const CMyStr<_Size> & str)
{
    new (this) CMyStr(str.c_str());
}

template <CMyStr_Size _Size>
CMyStr<_Size>::~CMyStr()
{
    m_Data[0] = '\0';
}

template <CMyStr_Size _Size>
const char * CMyStr<_Size>::c_str() const
{
    return m_Data;
}

template <CMyStr_Size _Size>
int CMyStr<_Size>::size() const
{
    return (int) strlen(m_Data);
}

template <CMyStr_Size _Size>
int CMyStr<_Size>::max_size() const
{
    return _Size;
}

template <CMyStr_Size _Size>
bool CMyStr<_Size>::empty() const
{
    return (m_Data[0] == '\0');
}

template <CMyStr_Size _Size>
CMyStr<_Size> & CMyStr<_Size>::operator = (const CMyStr<_Size> & str) 
{
    new (this) CMyStr(str.c_str());
    return * this;
}

template <CMyStr_Size _Size>
CMyStr<_Size> & CMyStr<_Size>::operator = (const std::string & str) 
{
    new (this) CMyStr(str.c_str());
    return * this;
}

template <CMyStr_Size _Size>
bool CMyStr<_Size>::operator == (const CMyStr<_Size> & str) const
{
    return (str.size() == size() && 0 == strcmp(m_Data, str.c_str()));
}

template <CMyStr_Size _Size>
bool CMyStr<_Size>::operator != (const CMyStr<_Size> & str) const
{
    return !(* this == str);
}

template <CMyStr_Size _Size>
bool CMyStr<_Size>::operator == (const std::string & str) const
{
    return (str.size() == size() && 0 == strcmp(c_str(), str.c_str()));
}

template <CMyStr_Size _Size>
bool CMyStr<_Size>::operator != (const std::string & str) const
{
    return !(* this == str);
}

template <CMyStr_Size _Size>
void CMyStr<_Size>::clear()
{
    m_Data[0] = '\0';
}

template <CMyStr_Size _Size>
bool CMyStr<_Size>::operator > (const CMyStr<_Size> & str ) const
{
    int nSize = size();
    if ( str.size() < nSize )
    {
        nSize = str.size();
    }

    for ( int i = 0; i < nSize; ++i )
    {
        if ( m_Data[i] == str.m_Data[i] )
        {
            continue;
        }
        else if( m_Data[i] > str.m_Data[i] )
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    if ( size() > nSize )
    {
        return true;
    }

    return false;
}

template <CMyStr_Size _Size>
bool CMyStr<_Size>::operator < (const CMyStr<_Size> & str ) const
{
    int nSize = size();
    if ( str.size() < nSize )
    {
        nSize = str.size();
    }

    for ( int i = 0; i < nSize; ++i )
    {
        if ( m_Data[i] == str.m_Data[i] )
        {
            continue;
        }
        else if( m_Data[i] < str.m_Data[i] )
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    if ( size() < nSize )
    {
        return true;
    }

    return false;
}

#endif

