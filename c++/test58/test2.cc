/*
 * @Date: 2021-07-06 18:36:57
 * @Author: Ron
 * @LastEditors: Ron
 * @LastEditTime: 2021-07-06 19:18:43
 * @FilePath: \issue-test\c++\test58\test2.cc
 * @Description: 
 */
#include <iostream>
template< class T >
struct identity {
typedef T type;
};

template<bool, typename T, typename F>
struct if_c : identity<F> {};

template< typename T, typename F>
struct if_c<true,T,F> : identity<T> {};

template<typename Bool, typename T, typename F>
struct if_ : if_c< Bool::value, T, F> {};

template< class T >
struct is_default_constructible_;

template< class T >
struct is_default_constructible :
  if_< std::is_arithmetic<T>,
    std::true_type,
    is_default_constructible_<T> >::type { };


template< class T >
struct is_default_constructible_ {

    template<class D> class Acessible : public D
    {
      friend class is_default_constructible_<D>;
      public:
      //using D::D; may be needed once N2540 is implemented 
    };

    template<int x>
    class receive_size{};

    template< class U >
    static int sfinae( receive_size< sizeof Acessible<U>() > * );

    template< class U >
    static char sfinae( ... );

public:
    enum { value = sizeof( sfinae<T>(0) ) == sizeof(int) };

};

struct p { p(); };
class q { q(); };
class r { r(int); };

#include <iostream>
using namespace std;

int main() {
    cerr << is_default_constructible<int>::value << endl // outputs 1
        << is_default_constructible<p>::value << endl
        << is_default_constructible<q>::value << endl
        << is_default_constructible<r>::value << endl; // outputs 0