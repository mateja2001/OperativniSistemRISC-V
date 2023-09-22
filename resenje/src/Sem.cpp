#include "../h/Sem.hpp"
#include "../h/MemoryAllocator.hpp"


void Sem::wait() {
    Sem* semaphore;
    __asm__ volatile("mv %0, a1": "=r"(semaphore));
    if(semaphore== nullptr){
        //greska
        uint64 pom=-1;
        __asm__ volatile("mv a0, %0" : : "r"(pom));
    }
    else{
        semaphore->val=semaphore->val-1;
        if(semaphore->val<0){
            PCB* old=PCB::running;
            if(semaphore->blockedQueue==nullptr)
            {
                semaphore->blockedQueue=old;
            }
            else
            {
                PCB* p=semaphore->blockedQueue;
                PCB* prev=nullptr;
                for(;p!=nullptr;prev=p,p=p->nextThread);
                prev->nextThread=old;
            }
            PCB::running=Scheduler::get();
            PCB::contextSwitch(&old->threadContext, &PCB::running->threadContext);
            if(PCB::running->flag==1){
                uint64 pom=-1;
                __asm__ volatile("mv a0, %0" : : "r"(pom));

            }
            else{
                uint64 pom=0;
                __asm__ volatile("mv a0, %0" : : "r"(pom));
            }

        }

    }



}

void Sem::signal() {
    Sem* semaphore;
    __asm__ volatile("mv %0, a1": "=r"(semaphore));
    if(semaphore== nullptr){
        //greska;
        uint64 pom=-1;
        __asm__ volatile("mv a0, %0" : : "r"(pom));
    }
    else{
        semaphore->val=semaphore->val+1;
        if(semaphore->val<=0){
            PCB* blocked=semaphore->blockedQueue;
            semaphore->blockedQueue=semaphore->blockedQueue->nextThread;
            blocked->nextThread=nullptr;
            Scheduler::put(blocked);
        }
        uint64 pom=0;
        __asm__ volatile("mv a0, %0" : : "r"(pom));

    }

}

void Sem::semOpen() {
    Sem** handle;
    uint64 init;
    __asm__ volatile("mv %0, a2": "=r"(init));
    __asm__ volatile("mv %0, a1": "=r"(handle));
    *handle=new Sem(init);
    if(*handle==nullptr)
    {
        //greska
        uint64 pom=-1;
        __asm__ volatile("mv a0, %0" : : "r"(pom));
    }
    else
    {
        uint64 pom=0;
        __asm__ volatile("mv a0, %0" : : "r"(pom));

    }
}

void Sem::semClose() {
    Sem* handle;
    __asm__ volatile("mv %0, a1": "=r"(handle));
    if(handle==nullptr){
        uint64 pom=-1;
        __asm__ volatile("mv a0, %0" : : "r"(pom));
    }
    else{
        if(handle->blockedQueue!=nullptr){
            PCB*p=handle->blockedQueue;
            while(p!=nullptr){
                PCB* old=p;
                p=p->nextThread;
                old->nextThread=nullptr;
                old->flag=1;
                Scheduler::put(old);
            }

        }
        uint64 pom=0;
        __asm__ volatile("mv a0, %0" : : "r"(pom));

    }

}

void* Sem::operator new(size_t size) {

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

void Sem::operator delete (void* p){
    MemoryAllocator::mem_free(p);
}