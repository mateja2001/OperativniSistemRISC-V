#include "../h/MemoryAllocator.hpp"

MemoryAllocator::BlockHeader* MemoryAllocator::freeHead=(BlockHeader*)HEAP_START_ADDR;
MemoryAllocator::BlockHeader* MemoryAllocator::allocatedHead=nullptr;


void MemoryAllocator::mem_free() {
    uint64* addr;
    __asm__ volatile("mv %0, a1": "=r"(addr));
    if(allocatedHead==nullptr){
        //greska, ako se oslobadja prostor a nema nista u listi alociranih segm.
        uint64 pom=-1;
        __asm__ volatile("mv a0, %0" : : "r"(pom));

    }
    else{
        BlockHeader* blk=allocatedHead;
        BlockHeader* prev=nullptr;
        for(;blk!=nullptr;prev=blk,blk=blk->next)
        {
            if((uint64*)((uint64*)blk+sizeof(BlockHeader))==addr)
            {
                break;
            }
        }
        //adresa nije iz segmenta alociranih adresa
        if(blk==nullptr){
            uint64 pom=-2;
            __asm__ volatile("mv a0, %0" : : "r"(pom));
        }
        //adresa je iz segmenta alciranih adresa
        else{
            if(prev){
                prev->next=blk->next;
            }
            else{
                allocatedHead=blk->next;
            }
            //ulancavanje na pocetak
            blk->next=freeHead;
            freeHead=blk;
            uint64 pom=0;
            __asm__ volatile("mv a0, %0" : : "r"(pom));
        }


    }

}

void MemoryAllocator::mem_alloc() {
    size_t size;
    __asm__ volatile("mv %0, a1" : "=r"(size));
    //size je vec poravnat na velicinu bloka MEM_BLOCK_SIZE


     BlockHeader *blk=freeHead;
     BlockHeader *prev=nullptr;
    for(;blk!= nullptr;prev=blk,blk=blk->next){
        if(blk->size >= size*MEM_BLOCK_SIZE) break;
    }
    //ako nema slobodnog mesta
    if(blk==nullptr){
        uint64 pom=0;
        __asm__ volatile("mv a0, %0" : : "r"(pom));

    }
    else{
        size_t remaining= blk->size - size*MEM_BLOCK_SIZE;
        if(remaining<=sizeof(BlockHeader)){
            if(prev){
                prev->next=blk->next;
            }
            else
            {
                freeHead=blk->next;
            }
            blk->size=size*MEM_BLOCK_SIZE;
            blk->next=nullptr;

        }
        else{
            blk->size=size*MEM_BLOCK_SIZE;
            size_t off= sizeof(BlockHeader)+size*MEM_BLOCK_SIZE;
            BlockHeader* newBlk=(BlockHeader*)((uint64*)blk+off);
            if(prev) prev->next=newBlk;
            else
                freeHead=newBlk;
            newBlk->size=remaining-sizeof(BlockHeader);
            newBlk->next=blk->next;
            blk->next=nullptr;
        }


        if(allocatedHead)
        {
            BlockHeader* ablk=allocatedHead;
            BlockHeader* aprev=nullptr;
            //ulancavanje na kraj
            for(;ablk!= nullptr;aprev=ablk,ablk=ablk->next);
            aprev->next=blk;
        }
        else
        {
            allocatedHead=blk;
        }
        uint64* pom=(uint64*)((uint64*)blk+sizeof(BlockHeader));
        __asm__ volatile("mv a0, %0" : : "r"(pom));
    }


}

void MemoryAllocator::initialization() {
    allocatedHead= nullptr;
    freeHead= (BlockHeader*)HEAP_START_ADDR;
    freeHead->size=(size_t)(((uint64)HEAP_END_ADDR-(uint64)HEAP_START_ADDR+1)-sizeof(BlockHeader));
    freeHead->next=nullptr;

}

void* MemoryAllocator::mem_alloc(size_t size){

    BlockHeader *blk=freeHead;
    BlockHeader *prev=nullptr;
    for(;blk!= nullptr;prev=blk,blk=blk->next){
        if(blk->size >= size*MEM_BLOCK_SIZE) break;
    }
    //ako nema slobodnog mesta
    if(blk==nullptr){
       return nullptr;
    }
    else{
        size_t remaining= blk->size - size*MEM_BLOCK_SIZE;
        if(remaining<=sizeof(BlockHeader)){
            if(prev){
                prev->next=blk->next;
            }
            else
            {
                freeHead=blk->next;
            }
            blk->size=size*MEM_BLOCK_SIZE;
            blk->next=nullptr;

        }
        else{
            blk->size=size*MEM_BLOCK_SIZE;
            size_t off= sizeof(BlockHeader)+size*MEM_BLOCK_SIZE;
            BlockHeader* newBlk=(BlockHeader*)((uint64*)blk+off);
            if(prev) prev->next=newBlk;
            else
                freeHead=newBlk;
            newBlk->size=remaining-sizeof(BlockHeader);
            newBlk->next=blk->next;
            blk->next=nullptr;
        }


        if(allocatedHead)
        {
            BlockHeader* ablk=allocatedHead;
            BlockHeader* aprev=nullptr;
            //ulancavanje na kraj
            for(;ablk!= nullptr;aprev=ablk,ablk=ablk->next);
            aprev->next=blk;
        }
        else
        {
            allocatedHead=blk;
        }
        uint64* pom=(uint64*)((uint64*)blk+sizeof(BlockHeader));
        return pom;
    }



}
int MemoryAllocator::mem_free(void* addr){
    if(allocatedHead==nullptr){
        //greska, ako se oslobadja prostor a nema nista u listi alociranih segm.
        return -1;

    }
    else{
        BlockHeader* blk=allocatedHead;
        BlockHeader* prev=nullptr;
        for(;blk!=nullptr;prev=blk,blk=blk->next)
        {
            if((uint64*)((uint64*)blk+sizeof(BlockHeader))==addr)
            {
                break;
            }
        }
        //adresa nije iz segmenta alociranih adresa
        if(blk==nullptr){
            return -2;
        }
            //adresa je iz segmenta alciranih adresa
        else{
            if(prev){
                prev->next=blk->next;
            }
            else{
                allocatedHead=blk->next;
            }
            //ulancavanje na pocetak
            blk->next=freeHead;
            freeHead=blk;
            return 0;
        }


    }

}

