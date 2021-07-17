/*
 * @Date: 2021-07-06 18:24:44
 * @Author: Ron
 * @LastEditors: Ron
 * @LastEditTime: 2021-07-06 18:33:44
 * @FilePath: \issue-test\c++\test58\test1.cc
 * @Description: 
 */
#include <iostream>
#include <experimental/type_traits>
using namespace std;

struct Archive {};
struct Foo
{
    Foo(Archive &) {}
};

struct Bar
{
    // unsupported
};

template <class T>
using HasUnarchiveConstructorImpl =
    decltype(T{std::declval<Archive &>()});

template <class T>
using HasUnarchiveConstructor =
    std::experimental::is_detected<HasUnarchiveConstructorImpl, T>;

int main(int argc, char *argv[])
{
    static_assert(HasUnarchiveConstructor<Foo>::value);
    static_assert(!HasUnarchiveConstructor<Bar>::value);
    return 0;
}