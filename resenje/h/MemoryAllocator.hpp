#ifndef PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP
#include "../lib/hw.h"

class MemoryAllocator{
private:
    struct BlockHeader{
        BlockHeader * next=nullptr;
        size_t size= (size_t)(((uint64)HEAP_END_ADDR-(uint64)HEAP_START_ADDR+1)-sizeof(BlockHeader));
        BlockHeader():next(nullptr){}
    };



    MemoryAllocator()=default;
    MemoryAllocator(const MemoryAllocator&)=delete;
    MemoryAllocator(MemoryAllocator&&)=delete;
    MemoryAllocator& operator =(const MemoryAllocator&)=delete;
    MemoryAllocator& operator =(MemoryAllocator&&)=delete;
    static BlockHeader* freeHead;
    static BlockHeader* allocatedHead;
public:

    static void initialization();
    static void mem_alloc();
    static void mem_free();
    static void* mem_alloc(size_t size);
    static int mem_free(void* p);

};

#endif //PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP
