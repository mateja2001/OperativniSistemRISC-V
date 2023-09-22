#include "../h/syscall_c.hpp"
#include "../h/syscall_cpp.hpp"
#include "../h/Riscv.hpp"
#include "../lib/console.h"

void* mem_alloc (size_t size){
    size_t sizeInBlocks;
    uint16 code=0x01;
    uint64* ret;//povratna vrednost
    if(size%MEM_BLOCK_SIZE==0)
    {
        sizeInBlocks=size/MEM_BLOCK_SIZE;
    }
    else
    {
        sizeInBlocks=size/MEM_BLOCK_SIZE+1;
    }
    __asm__ volatile("mv a1, %0" : : "r"(sizeInBlocks));
    __asm__ volatile("mv a0, %0" : : "r"(code));
    __asm__ volatile("ecall");
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

int mem_free(void * addr){
    uint64 ret;
    uint16 code=0x02;
    __asm__ volatile("mv a1, %0" : : "r"(addr));
    __asm__ volatile("mv a0, %0" : : "r"(code));
    __asm__ volatile("ecall");
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;

}

void thread_dispatch(){
    uint16 code=0x13;
    __asm__ volatile("mv a0, %0" : : "r"(code));
    __asm__ volatile("ecall");
}

int thread_create(thread_t* handle, void(*start_routine)(void *), void* arg){
    uint64 ret;
    uint64 code=0x11;
    uint64* stack=new uint64[DEFAULT_STACK_SIZE];
    if(stack==nullptr){
        *handle=nullptr;
        return -1;
    }

    __asm__ volatile("mv a4, %0" : : "r"(stack));
    __asm__ volatile("mv a3, %0" : : "r"(arg));
    __asm__ volatile("mv a2, %0" : : "r"(start_routine));
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    __asm__ volatile("mv a0, %0" : : "r"(code));

    __asm__ volatile("ecall");
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;

}

int thread_exit (){
    uint64 ret;
    uint16 code=0x12;
    __asm__ volatile("mv a0, %0" : : "r"(code));
    __asm__ volatile("ecall");
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

int sem_open (sem_t* handle, unsigned init){
    uint64 ret;
    uint16 code=0x21;
    __asm__ volatile("mv a2, %0" : : "r"(init));
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    __asm__ volatile("mv a0, %0" : : "r"(code));
    __asm__ volatile("ecall");
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

int sem_close (sem_t handle){
    uint64 ret;
    uint16 code=0x22;
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    __asm__ volatile("mv a0, %0" : : "r"(code));
    __asm__ volatile("ecall");
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

int sem_wait (sem_t id){
    uint64 ret;
    uint16 code=0x23;
    __asm__ volatile("mv a1, %0" : : "r"(id));
    __asm__ volatile("mv a0, %0" : : "r"(code));
    __asm__ volatile("ecall");
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

int sem_signal (sem_t id){
    uint64 ret;
    uint16 code=0x24;
    __asm__ volatile("mv a1, %0" : : "r"(id));
    __asm__ volatile("mv a0, %0" : : "r"(code));
    __asm__ volatile("ecall");
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

char getc(){

    uint64 ret;
    uint16 code=0x41;
    __asm__ volatile("mv a0, %0" : : "r"(code));
    __asm__ volatile("ecall");
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;


}

void putc (char c){

    uint16 code=0x42;
    __asm__ volatile("mv a1, %0" : : "r"(c));
    __asm__ volatile("mv a0, %0" : : "r"(code));
    __asm__ volatile("ecall");


}