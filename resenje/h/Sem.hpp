#ifndef PROJECT_BASE_V1_1_SEM_HPP
#define PROJECT_BASE_V1_1_SEM_HPP
#include "../h/PCB.hpp"
class PCB;
class Sem{
private:
    PCB* blockedQueue;
    int val;
public:
    Sem(int init=1):blockedQueue(nullptr),val(init){}
    static void wait();
    static void signal();
    static void semOpen();
    static void semClose();
    void* operator new (size_t size);
    void operator delete (void* p);



};

#endif //PROJECT_BASE_V1_1_SEM_HPP
