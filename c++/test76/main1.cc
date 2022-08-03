#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <typeinfo>
#include <cxxabi.h>

std::string demangle(const char *mangleName)
{
    assert(mangleName != nullptr);

    int status;
    char *demangleName = abi::__cxa_demangle(mangleName, nullptr, nullptr, &status);

    if (demangleName != nullptr)
    {
        std::string realName = demangleName;
        std::free(demangleName);
        return realName;
    }
    else
    {
        return mangleName;
    }
}


#define TN(t) (demangle(typeid(t).name()))
#define TEST(t) std::cout << TN(t) << std::endl

int f(int arg0, double arg1, std::string arg2, const char *arg3, int *arg4, std::string &arg5);

int main(int argc, char *argv[])
{
    TEST(int);
    TEST(std::basic_string<char>);

    std::cout << typeid(f).name() << std::endl;
    TEST(f);

    return 0;
}