#include "ThreadClass.h"
// #include <iostream>
// #include <thread>
// using namespace std;

#include <stdlib.h>
#include <time.h>

static void urlThread(int *val) {
    cout << "url thread" << endl;
    delete val;
}

static void urlThread0() {
    cout << "url thread 0" << endl;
}

static void childThread() {
    while(true){
        cout << "child thread: deal with requests" << endl;
        int val = rand();
        if(val % 9 == 0) {
            // create url thread
            cout << "get " << val << endl;
            // std::thread t = std::thread(urlThread, new int(val));
            // std::thread t = std::thread(urlThread0);
            ThreadClass *t = new ThreadClass();
            t->Start();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }   
}

int main(int argc, char *argv[]){
	srand((unsigned int)time(NULL));
    
    std::thread child = std::thread(childThread);
    // std::thread(childThread);

    while(true){
        cout << "main thread" << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return 0;
}