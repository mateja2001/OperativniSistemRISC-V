#ifndef PROJECT_BASE_V1_1_SYSCALL_CPP_HPP
#define PROJECT_BASE_V1_1_SYSCALL_CPP_HPP
#include "../lib/hw.h"
#include "../h/PCB.hpp"
#include "../h/Sem.hpp"
class PCB;
class Sem;
void* operator new (size_t);
void operator delete (void*);
void* operator new [](size_t);
void operator delete[](void *);

typedef PCB* thread_t;

class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    int start ();
    static void dispatch ();
    //static int sleep (time_t);
protected:
    Thread ();
    virtual void run () {}
private:
    thread_t myHandle;
    static void wrapper(void* arg);
};


typedef Sem* sem_t;
class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    int wait ();
    int signal ();

private:
    sem_t myHandle;
};

class Console {
public:
    static char getc ();
    static void putc (char);
};

#endif //PROJECT_BASE_V1_1_SYSCALL_CPP_HPP
