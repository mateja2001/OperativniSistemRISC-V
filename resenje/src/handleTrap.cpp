#include "../lib/hw.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/PCB.hpp"
#include "../h/Riscv.hpp"
#include "../lib/console.h"
#include "../h/Sem.hpp"

extern "C" void handleTrap(){
    uint64 volatile scauseVar;
    __asm__ volatile("csrr %0, scause": "=r"(scauseVar));
    __asm__ volatile("mv %0, a4": "=r"(Riscv::a4));
    __asm__ volatile("mv %0, a3": "=r"(Riscv::a3));
    // softerski prekid iz sistemskog rezima ili iz korisixnog rezima
    if(scauseVar==0x09 || scauseVar==0x08){
        uint64 volatile opCode;
        __asm__ volatile("mv %0, a0": "=r"(opCode));
        switch(opCode)
        {
            case 0x01:
                MemoryAllocator::mem_alloc();
                break;
            case 0x02:
                MemoryAllocator::mem_free();
                break;
            case 0x13:
                //cuvanje sepc-a i sstatus-a na steku tekuce niti
                volatile uint64 sepc;
                __asm__ volatile ("csrr %0, sepc" :  "=r"(sepc));
                volatile uint64 sstatus;
                __asm__ volatile ("csrr %0, sstatus" :  "=r"(sstatus));
                PCB::timeSliceCounter=0;
                PCB::yield();
                __asm__ volatile ("csrw sstatus, %0" : : "r"(sstatus));
                __asm__ volatile ("csrw sepc, %0" : : "r"(sepc));
                break;
            case 0x11:
                PCB::createThread();
                break;
            case 0x12:
                volatile uint64 sepcE;
                __asm__ volatile ("csrr %0, sepc" :  "=r"(sepcE));
                volatile uint64 sstatusE;
                __asm__ volatile ("csrr %0, sstatus" :  "=r"(sstatusE));
                PCB::threadExit();
                __asm__ volatile ("csrw sstatus, %0" : : "r"(sstatusE));
                __asm__ volatile ("csrw sepc, %0" : : "r"(sepcE));
                break;
            case 0x21:
                Sem::semOpen();
                break;
            case 0x22:
                Sem::semClose();
                break;
            case 0x23:
                volatile uint64 sepcW;
                __asm__ volatile ("csrr %0, sepc" :  "=r"(sepcW));
                volatile uint64 sstatusW;
                __asm__ volatile ("csrr %0, sstatus" :  "=r"(sstatusW));
                Sem::wait();
                __asm__ volatile ("csrw sstatus, %0" : : "r"(sstatusW));
                __asm__ volatile ("csrw sepc, %0" : : "r"(sepcW));
                break;
            case 0x24:
                Sem::signal();
                break;
            case 0x41:
                volatile uint64 sepcG;
                __asm__ volatile ("csrr %0, sepc" :  "=r"(sepcG));
                volatile uint64 sstatusG;
                __asm__ volatile ("csrr %0, sstatus" :  "=r"(sstatusG));
                __getc();
                __asm__ volatile ("csrw sstatus, %0" : : "r"(sstatusG));
                __asm__ volatile ("csrw sepc, %0" : : "r"(sepcG));
                break;
            case 0x42:
                volatile uint64 sepcP;
                __asm__ volatile ("csrr %0, sepc" :  "=r"(sepcP));
                volatile uint64 sstatusP;
                __asm__ volatile ("csrr %0, sstatus" :  "=r"(sstatusP));
                char c;
                __asm__ volatile("mv %0, a1": "=r"(c));
                __putc(c);
                __asm__ volatile ("csrw sstatus, %0" : : "r"(sstatusP));
                __asm__ volatile ("csrw sepc, %0" : : "r"(sepcP));
                break;
            default:
                break;
        }

    }

    else if(scauseVar==(0x01UL<<63|0x09))
    {
        //smanjuje sepc za 4 jer sam po defaultu u trapu povecavao za 4
        volatile uint64 sepc;
        __asm__ volatile ("csrr %0, sepc" :  "=r"(sepc));
        sepc=sepc-4;
        __asm__ volatile ("csrw sepc, %0" : : "r"(sepc));
        volatile uint64 sstatus;
        __asm__ volatile ("csrr %0, sstatus" :  "=r"(sstatus));
        console_handler();
        __asm__ volatile ("csrw sstatus, %0" : : "r"(sstatus));
        __asm__ volatile ("csrw sepc, %0" : : "r"(sepc));


    }
    else if(scauseVar==(0x01UL<<63|0x01)){
        PCB::timeSliceCounter++;
        volatile uint64 sepc;
        __asm__ volatile ("csrr %0, sepc" :  "=r"(sepc));
        sepc=sepc-4;
        volatile uint64 sstatus;
        __asm__ volatile ("csrr %0, sstatus" :  "=r"(sstatus));

        if(PCB::timeSliceCounter>=PCB::running->getTimeSlice())
        {
            PCB::timeSliceCounter=0;
            PCB::yield();
        }

        __asm__ volatile ("csrw sstatus, %0" : : "r"(sstatus));
        __asm__ volatile ("csrw sepc, %0" : : "r"(sepc));
        __asm__ volatile("csrc sip,0x02");
    }else if(scauseVar==0x02){
        //jer sam u samoj prekidnoj rutini po defaultu povecavao sepc za 4, a ukoliko je bilo koji drugi prekid sem ecall
        //treba se sepc vratiti na normalnu vrednost, tj ne treba biti promenjen
        //ilegalna instrukcija
        volatile uint64 sepc;
        __asm__ volatile ("csrr %0, sepc" :  "=r"(sepc));
        sepc=sepc-4;
        __asm__ volatile ("csrw sepc, %0" : : "r"(sepc));


    }







}