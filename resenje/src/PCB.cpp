#include "../h/PCB.hpp"
#include "../h/Riscv.hpp"
#include "../h/MemoryAllocator.hpp"

PCB * PCB::running=nullptr;
uint64 PCB::timeSliceCounter=0;

void PCB::createThread() {
    PCB** handle;
    void(*start_routine)(void*);
    void* arg;
    uint64* stackSpace;

    __asm__ volatile("mv %0, a2": "=r"(start_routine));
    __asm__ volatile("mv %0, a1": "=r"(handle));

    arg=(void*)Riscv::a3;
    stackSpace=(uint64*)Riscv::a4;
    *handle=new PCB(start_routine,arg,stackSpace);
    if(*handle==nullptr)
    {
        //greska pri kreiranju
        uint64 pom=-1;
        __asm__ volatile("mv a0, %0" : : "r"(pom));
    }
    else
    {
        if(start_routine!=nullptr){
            Scheduler::put(*handle);
        }
        uint64 pom=0;
        __asm__ volatile("mv a0, %0" : : "r"(pom));
    }

}

PCB* PCB::createThread(void(*start_routine)(void*), void* arg){
    return new PCB(start_routine,arg);
}


void PCB::yield() {
    PCB* old=running;
    if(!old->isFinished())
    {
        Scheduler::put(old);
    }
    running=Scheduler::get();

    PCB::contextSwitch(&old->threadContext, &running->threadContext);
}

void PCB::dispatch(){
    uint16 code=0x13;
    __asm__ volatile("mv a0, %0" : : "r"(code));
    __asm__ volatile("ecall");
}

void PCB::threadWrapper() {
    Riscv::popSppSpie();
    running->start_routine(running->arg);
    running->finish();
    dispatch();
}


void* PCB::operator new(size_t size) {
    size_t sizeInBlocks;
    if(size%MEM_BLOCK_SIZE==0)
    {
        sizeInBlocks=size/MEM_BLOCK_SIZE;
    }
    else
    {
        sizeInBlocks=size/MEM_BLOCK_SIZE+1;
    }

    return MemoryAllocator::mem_alloc(sizeInBlocks);

}

void PCB::operator delete (void* p){
    MemoryAllocator::mem_free(p);
}

void PCB::threadExit(){
    running->finish();
    yield();
}

