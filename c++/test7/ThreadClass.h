#include <iostream>
#include <thread>
using namespace std;

class ThreadClass {
    thread m_thread;

public:
    ThreadClass();
    ~ThreadClass();
    void Start();

    static void ThreadMain(ThreadClass*);
};