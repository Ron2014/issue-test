/*
 * @Date: 2021-07-06 17:54:22
 * @Author: Ron
 * @LastEditors: Ron
 * @LastEditTime: 2021-07-06 17:55:03
 * @FilePath: \issue-test\c++\test58\test.cc
 * @Description: 
 */

template< class T >
class is_default_constructible {
    template<int x>
    class receive_size{};

    template< class U >
    static int sfinae( receive_size< sizeof U() > * );

    template< class U >
    static char sfinae( ... );

public:
    enum { value = sizeof( sfinae<T>(0) ) == sizeof(int) };
};

class q { q(); };
class r { r(int); };

#include <iostream>
using namespace std;

int main() {
    cerr << is_default_constructible<int>::value << endl // outputs 1
        // fails to compile: access violation
        // FCD demands that access violations be unrecoverable
        // indeed, it's murky: q is default-constructible, but only "rarely"
        << is_default_constructible<q>::value << endl
        << is_default_constructible<r>::value << endl; // outputs 0
}