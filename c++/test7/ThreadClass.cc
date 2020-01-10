#include "ThreadClass.h"

ThreadClass::ThreadClass(){
    cout << "calling ThreadClass constructor" << endl;
}

ThreadClass::~ThreadClass(){
    cout << "calling ThreadClass destructor" << endl;
}

void
ThreadClass::ThreadMain(ThreadClass *self){
    cout << "ThreadClass::ThreadMain" << endl;
    delete(self);
}

void
ThreadClass::Start(){
    m_thread = thread(ThreadMain, this);
}