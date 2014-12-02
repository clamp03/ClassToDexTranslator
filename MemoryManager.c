#include "MemoryManager.h"
int totalAllocSize = 0;
#if 0
MemoryManager* initializeMem() {
    MemoryManager* mm = (MemoryManager*)malloc(sizeof(MemorySpace));
    mm->index = 0;
    mm->space = NULL;
    return mm;
}
void* allocateMem(MemoryManager* mm, int size) {
    void* result;
    if(size == 0) return NULL;
    if(mm->space == NULL || (mm->index + size) > DEFAULT_MEMORY_ALLOCATION_SIZE) {
        MemorySpace* newSpace = (MemorySpace*)malloc(sizeof(MemorySpace));
        if(size > DEFAULT_MEMORY_ALLOCATION_SIZE) {
            newSpace->space = (void*)malloc(size);
        }else {
            newSpace->space = (void*)malloc(DEFAULT_MEMORY_ALLOCATION_SIZE);
        }
        if(verbose) fprintf(stderr, "Allocated Memory %p %p (alloc size: %d)\n", newSpace, newSpace->space , size);
        newSpace->next = mm->space;
        mm->space = newSpace;
        mm->index = 0;
    }
    result = (void*)(mm->space->space + mm->index);
    mm->index = mm->index + size;
    return result;
}
void freeAllMem(MemoryManager* mm) {
    MemorySpace* curr = mm->space;
    
    while(curr != NULL) {
        MemorySpace* next = curr->next;
        if(verbose) fprintf(stderr, "Free All Memory %p %p\n", curr, curr->space);
        free(curr->space);
        free(curr);
        curr = next;
    }
    mm->space = NULL;
    free(mm);
}
#endif

MemoryManager* initializeMem() {
    MemoryManager* mm = (MemoryManager*)malloc(sizeof(MemorySpace));
    //mm->index = 0;
    mm->space = NULL;
    return mm;
}

void* allocateMem(MemoryManager* mm, int size) {
    void* result;
    if(size == 0) return NULL;
    MemorySpace* newSpace = (MemorySpace*)malloc(sizeof(MemorySpace));
    newSpace->space = (void*)malloc(size);
    memset(newSpace->space, 0, size);
    newSpace->size = size;
    newSpace->next = mm->space;
    mm->space = newSpace;
    totalAllocSize += size;
    if(verbose) fprintf(stderr, "Allocated Memory %p %p (alloc size: %d, total size: %d)\n", newSpace, newSpace->space , size, totalAllocSize);
    return mm->space->space;
}

void freeMem(MemoryManager* mm, void* mem) {
    MemorySpace* curr;
    if(mm == NULL || mm->space == NULL) return;
    curr = mm->space;
    if(curr->space == mem) {
        totalAllocSize -= curr->size;
        mm->space = curr->next;
        if(verbose) fprintf(stderr, "Freed Memory %p %p (free size: %d, total size: %d)\n", curr, curr->space, curr->size, totalAllocSize);
        free(curr->space);
        free(curr);
        return;
    }
    while(curr != NULL) {
        MemorySpace* next = curr->next;
        if(next != NULL && next->space == mem) {
            curr->next = next->next;
            totalAllocSize -= next->size;
            if(verbose) fprintf(stderr, "Freed Memory %p %p (free size: %d, total size: %d)\n", next, next->space, next->size, totalAllocSize);
            free(next->space);
            free(next);
            return;
        }
        curr = next;
    }
}

void freeAllMem(MemoryManager* mm) {
    MemorySpace* curr = mm->space;
    
    while(curr != NULL) {
        MemorySpace* next = curr->next;
        totalAllocSize -= curr->size;
        if(verbose) fprintf(stderr, "Free All Memory %p %p (free size: %d, total size: %d\n", curr, curr->space, curr->size, totalAllocSize);
        free(curr->space);
        free(curr);
        curr = next;
    }
    mm->space = NULL;
    free(mm);
}
