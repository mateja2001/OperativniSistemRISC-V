#include "../h/Scheduler.hpp"
#include "../h/syscall_cpp.hpp"
PCB* Scheduler::readyHead=nullptr;

void Scheduler::put(PCB *pcb) {
    if(readyHead==nullptr){
        readyHead=pcb;
    }
    else{
        PCB *p=readyHead;
        PCB *prev=nullptr;
        for(;p!=nullptr;prev=p,p=p->nextThread);
        prev->nextThread=pcb;
    }

}

PCB *Scheduler::get() {
    if(readyHead==nullptr) return nullptr;
    PCB *first=readyHead;
    readyHead=readyHead->nextThread;
    first->nextThread=nullptr;
    return first;
}
