#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
    cout << "hello world" << endl;
    cout << "hello world" << '\n';
    cout << "hello world" << "\n\r";
    cout << "hello world" << '\n' << flush;
    cout << "hello world" << '\n' << '\r'; fflush(stdout);

    cout << "hello" << '\014' << " world" << endl;
    // printf("\n");
    cout << "hello" << '\014' << " world" << '\n';
    // printf("\n");
    cout << "hello" << '\014' << " world" << "\n\r";
    // printf("\n");
    cout << "hello" << '\014' << " world" << '\n' << flush;
    // printf("\n");
    cout << "hello" << '\014' << " world" << '\n' << '\r'; fflush(stdout);
    // printf("\n");

    cout << "hello\014 world" << endl;
    // printf("\n");
    cout << "hello\014 world" << '\n';
    // printf("\n");
    cout << "hello\014 world" << "\n\r";
    // printf("\n");
    cout << "hello\014 world" << '\n' << flush;
    // printf("\n");
    cout << "hello\014 world" << '\n' << '\r'; fflush(stdout);
    // printf("\n");

    return 0;
}