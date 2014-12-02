#include "Common.h"

int verbose = 0;
#if 0
int memoryIndex = 0;
typedef struct MemoryManager{
    struct MemoryManager* next;
    void* space;
}MemoryManager;
MemoryManager* header = NULL;

void* allocateMem(int size) {
    void* result;
    if(size == 0) return NULL;
    if(header == NULL || (memoryIndex + size) > DEFAULT_MEMORY_ALLOCATION_SIZE) {
        MemoryManager* mm = (MemoryManager*)malloc(sizeof(MemoryManager));
        if(size > DEFAULT_MEMORY_ALLOCATION_SIZE) {
            mm->space = (void*)malloc(size);
        }else {
            mm->space = (void*)malloc(DEFAULT_MEMORY_ALLOCATION_SIZE);
        }
        if(verbose) fprintf(stderr, "Allocated Memory %p %p (alloc size: %d)\n", mm, mm->space , size);
        mm->next = header;
        header = mm;
        memoryIndex = 0;
    }
    result = (void*)(header->space + memoryIndex);
    memoryIndex += size;
    return result;
}
void freeAllMem() {
    MemoryManager* curr = header;
    
    while(curr != NULL) {
        MemoryManager* next = curr->next;
        if(verbose) fprintf(stderr, "Free All Memory %p %p\n", curr, curr->space);
        free(curr->space);
        free(curr);
        curr = next;
    }
    header = NULL;
}

#if 0
typedef struct MemoryManager{
    struct MemoryManager* prev;
    struct MemoryManager* next;
    int size;
    void* mem;
}MemoryManager;
MemoryManager* header = NULL;
int allocatedSize = 0;
int allocCount = 0;
void* allocateMem(int size) {
    if(size == 0) return NULL;
    MemoryManager* newMem = (MemoryManager*)malloc(sizeof(struct MemoryManager));
    newMem->mem = (void*)malloc(size);
    newMem->size = size;
    newMem->prev = NULL;
    newMem->next = header;
    if(header != NULL) {
        header->prev = newMem;
    }
    header = newMem;
    allocatedSize += size;
    fprintf(stderr, "%d\n", ++allocCount);
    if(verbose) fprintf(stderr, "Allocated Memory Size is %d bytes %p %p (alloc size: %d)\n", allocatedSize, newMem, newMem->mem, size);
    return newMem->mem;
}
/*
void freeMem(void* mem) {
    MemoryManager* iter = header;
    int freeSize;
    while(iter != NULL) {
        if(iter->mem == mem) {
            MemoryManager* next = iter->next;
            MemoryManager* prev = iter->prev;
            freeSize = iter->size;
            prev->next = next;
            next->prev = prev;
            free(iter->mem);
            free(iter);
            break;
        }
        iter = iter->next;
    }
    allocatedSize -= freeSize;
    if(verbose) fprintf(stderr, "Allocated Memory Size is %d bytes (free: %d)\n", allocatedSize, freeSize);
}
*/
void freeAllMem() {
    MemoryManager* curr = header;
    
    while(curr != NULL) {
        MemoryManager* next = curr->next;
        allocatedSize -= curr->size;
        if(verbose) fprintf(stderr, "Allocated Memory Size is %d bytes %p %p (size: %d)\n", allocatedSize, curr, curr->mem, curr->size);
        free(curr->mem);
        free(curr);
        curr = next;
    }
}
#endif
#endif
