#include "../h/syscall_cpp.hpp"
#include "../h/syscall_c.hpp"
void* operator new (size_t size){
    return mem_alloc(size);
}
void operator delete (void* p){
    mem_free(p);
}
void* operator new[] (size_t size){
    return mem_alloc(size);
}
void operator delete [](void* p){
    mem_free(p);
}


void Thread::dispatch() {
    thread_dispatch();
}

Thread::Thread(void (*body)(void *), void *arg) {
    myHandle=PCB::createThread(body,arg);
}

int Thread::start() {
    Scheduler::put(myHandle);
    return 0;
}

void Thread::wrapper(void *arg) {
    if(arg)
    {
        ((Thread*)arg)->run();
    }
}

Thread::Thread() {
    myHandle=PCB::createThread(&wrapper,this);
}

Thread::~Thread() {
    delete myHandle;
    myHandle=nullptr;
}


Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle,init);
}
int Semaphore::wait() {
    return sem_wait(myHandle);
}
int Semaphore::signal() {
    return sem_signal(myHandle);
}
Semaphore::~Semaphore() {
    sem_close(myHandle);
    delete myHandle;
    myHandle=nullptr;
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}