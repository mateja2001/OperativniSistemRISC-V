#include "../h/Riscv.hpp"
uint64 Riscv::a4=0;
uint64 Riscv::a3=0;
void Riscv::popSppSpie()
{
    __asm__ volatile ("csrw sepc, ra");
    Riscv::mc_sstatus(SSTATUS_SPP);
    __asm__ volatile ("sret");
}
