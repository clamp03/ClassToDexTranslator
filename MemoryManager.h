#ifndef TRANSLATOR_MEMORY_MANAGER_H
#define TRANSLATOR_MEMORY_MANAGER_H
#include "Common.h"
#define DEFAULT_MEMORY_ALLOCATION_SIZE 1024 

typedef struct MemorySpace {
    struct MemorySpace* next;
    int size;
    void* space;
}MemorySpace;

typedef struct {
    MemorySpace* space;
    int index;
}MemoryManager;

MemoryManager* initializeMem();
void* allocateMem(MemoryManager* mm, int size);
void freeMem(MemoryManager* mm, void* mem);
void freeAllMem(MemoryManager* mm);
#endif
