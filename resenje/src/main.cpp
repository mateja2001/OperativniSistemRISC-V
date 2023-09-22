#include "../lib/hw.h"
#include "../lib/console.h"
#include "../h/syscall_c.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/print.hpp"
#include "../h/PCB.hpp"
#include "../h/syscall_cpp.hpp"
#include "../h/Sem.hpp"
void userMain();
extern "C" void trap();


void wrapper(void* p)
{
    userMain();
}
void main(){
    __asm__ volatile("csrw stvec, %0" : : "r"(&trap));
    MemoryAllocator::initialization();

    //pokretanje i izvrsavanje userMain-a u korisnickom rezimu
    PCB* thread1;
    PCB* thread2;
    thread_create(&thread1,nullptr,nullptr);
    thread_create(&thread2,&wrapper,nullptr);
    PCB::running=thread1;
    __asm__ volatile("csrs sstatus, 0x02");


    while(!thread2->isFinished()){
        thread_dispatch();

    }

    delete thread1;
    delete thread2;



}