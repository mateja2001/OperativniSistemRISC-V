#ifndef PROJECT_BASE_V1_1_PCB_HPP
#define PROJECT_BASE_V1_1_PCB_HPP
#include "../lib/hw.h"
#include "../h/syscall_cpp.hpp"
#include "../h/Scheduler.hpp"

class Thread;
class PCB{
public:
    ~PCB(){ delete[] stack;}

    bool isFinished()const{ return finished;}

    void finish(){finished=true;}

    static void createThread();

    uint64 getTimeSlice()const{return timeSlice;}

    static void yield();

    static void threadExit();

    static PCB* createThread(void(*start_routine)(void*), void* arg);



    static PCB* running;
    static uint64 timeSliceCounter;
    void* operator new (size_t size);
    void operator delete (void* p);

private:

    friend class Riscv;
    friend class Sem;
    friend class Scheduler;
    explicit PCB(void(*start_routine)(void*), void* arg, uint64* stackSpace):start_routine(start_routine),
    stack(start_routine!= nullptr?stackSpace:nullptr),
    threadContext({
        (uint64)&threadWrapper,
        stack!=nullptr?(uint64)&stack[DEFAULT_STACK_SIZE]:0
    }),
    finished(false),arg(arg),nextThread(nullptr),flag(0),timeSlice(DEFAULT_TIME_SLICE){

    }

    explicit PCB(void(*start_routine)(void*), void* arg):start_routine(start_routine),
    stack(start_routine!= nullptr?new uint64[DEFAULT_STACK_SIZE]:nullptr),
    threadContext({
        (uint64)&threadWrapper,
        stack!=nullptr?(uint64)&stack[DEFAULT_STACK_SIZE]:0
    }),
    finished(false),arg(arg),nextThread(nullptr),flag(0),timeSlice(DEFAULT_TIME_SLICE){
    }






    struct Context{
        uint64 retAddr;
        uint64 sp;
    };


    void(*start_routine)(void* arg);
    uint64* stack;
    Context threadContext;
    bool finished;
    void* arg;
    PCB* nextThread;
    int flag;
    uint64 timeSlice;



    static void contextSwitch(Context* oldC, Context* runningC);

    static void dispatch();

    static void threadWrapper();


};




#endif //PROJECT_BASE_V1_1_PCB_HPP
