#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    int x = 100;
    printf("十进制：%d;八进制：%o;十六进制：%x\n", x, x, x);
    printf("十进制：%d;八进制：%#o;十六进制：%#x\n", x, x, x);

    long long lla = -2147483647;
    printf("%02x\n", (unsigned)lla);

    long la = (long)lla;
    printf("%02x\n", (unsigned)la);

    int ia = (int)la;
    printf("%02x\n", (unsigned)ia);

    short sa = (short)ia;
    printf("%02x\n", (unsigned)sa);

    char ca = (char)sa;
    printf("%02x\n", (unsigned)ca);

    if (argc > 1)
    {
        // std::cout << std::stoi(argv[1], NULL, 8) << std::endl;
        // std::cout << atoi(argv[1]) << std::endl;
        std::cout << strtol(argv[1], NULL, 8) << std::endl;
    }

    return 0;
}
