#ifndef PROJECT_BASE_V1_1_SYSCALL_C_HPP
#define PROJECT_BASE_V1_1_SYSCALL_C_HPP
#include "../lib/hw.h"
#include "../h/PCB.hpp"
#include "../h/Sem.hpp"
typedef PCB* thread_t;
typedef Sem* sem_t;
const int EOF = -1;
void* mem_alloc (size_t size);
int mem_free (void*);
void thread_dispatch ();
int thread_create(thread_t* handle, void(*start_routine)(void *), void* arg);
int thread_exit ();
int sem_open (sem_t* handle, unsigned init);
int sem_close (sem_t handle);
int sem_wait (sem_t id);
int sem_signal (sem_t id);
char getc ();
void putc (char);







#endif //PROJECT_BASE_V1_1_SYSCALL_C_HPP
