#include "Translator.h"
/*
const char* fileNames[5] = {
    "AWTBench.class",
    "AWTBenchOld.class",
    "AWTBenchOldPaintTest.class",
    "IniFile.class",
    "Result.class"
};
*/
int vmoClassFileOpen(const char* fileName, const char* odexOutputName, RawDexFile** ppRawDexFile, bool isBootstrap) {
    DvmDex* pDvmDex = NULL;
    char* cachedName = NULL;
    //bool locked = false;
    int result = -1;
    //int i;
    //for(i = 0 ; i < 5 ; i++) {
    if(translateClassFile(fileName, &pDvmDex)) {
    //if(translateClassFile(fileNames[i], &pDvmDex)) {
        LOGI("Unable to map classes.dex in %s\n", fileName);
        goto bail;
    }
    //}
    LOGV("Successfully opend '%s' in '%s'\n", fileName);
    *ppRawDexFile = (RawDexFile*) calloc(1, sizeof(RawDexFile));
    //(*ppRawDexFile)->archive = archive;
    (*ppRawDexFile)->cacheFileName = NULL;
    (*ppRawDexFile)->pDvmDex = pDvmDex;
    cachedName = NULL;
    
    result = 0;
bail:
    return result;
}

static DvmDex* allocateAuxStructures(DexFile* pDexFile)
{
    DvmDex* pDvmDex;
    const DexHeader* pHeader;
    u4 stringCount, classCount, methodCount, fieldCount;

    pDvmDex = (DvmDex*) calloc(1, sizeof(DvmDex));
    if (pDvmDex == NULL)
        return NULL;

    pDvmDex->pDexFile = pDexFile;
    pDvmDex->pHeader = pDexFile->pHeader;

    pHeader = pDvmDex->pHeader;

    stringCount = pHeader->stringIdsSize;
    classCount = pHeader->typeIdsSize;
    methodCount = pHeader->methodIdsSize;
    fieldCount = pHeader->fieldIdsSize;

#if (DVM_RESOLVER_CACHE == DVM_RC_REDUCING) || \
    (DVM_RESOLVER_CACHE == DVM_RC_EXPANDING)
    if (pDexFile->indexMap.stringReducedCount > 0)
        stringCount = pDexFile->indexMap.stringReducedCount;
    if (pDexFile->indexMap.classReducedCount > 0)
        classCount = pDexFile->indexMap.classReducedCount;
    if (pDexFile->indexMap.methodReducedCount > 0)
        methodCount = pDexFile->indexMap.methodReducedCount;
    if (pDexFile->indexMap.fieldReducedCount > 0)
        fieldCount = pDexFile->indexMap.fieldReducedCount;
#elif (DVM_RESOLVER_CACHE == DVM_RC_NO_CACHE)
    stringCount = classCount = methodCount = fieldCount = 0;
#endif

    pDvmDex->pResStrings = (struct StringObject**)
        calloc(stringCount, sizeof(struct StringObject*));

    pDvmDex->pResClasses = (struct ClassObject**)
        calloc(classCount, sizeof(struct ClassObject*));

    pDvmDex->pResMethods = (struct Method**)
        calloc(methodCount, sizeof(struct Method*));

    pDvmDex->pResFields = (struct Field**)
        calloc(fieldCount, sizeof(struct Field*));

    LOGV("+++ DEX %p: allocateAux %d+%d+%d+%d * 4 = %d bytes\n",
        pDvmDex, stringCount, classCount, methodCount, fieldCount,
        (stringCount + classCount + methodCount + fieldCount) * 4);

    pDvmDex->pInterfaceCache = dvmAllocAtomicCache(DEX_INTERFACE_CACHE_SIZE);

    if (pDvmDex->pResStrings == NULL ||
        pDvmDex->pResClasses == NULL ||
        pDvmDex->pResMethods == NULL ||
        pDvmDex->pResFields == NULL ||
        pDvmDex->pInterfaceCache == NULL)
    {
        LOGE("Alloc failure in allocateAuxStructures\n");
        free(pDvmDex->pResStrings);
        free(pDvmDex->pResClasses);
        free(pDvmDex->pResMethods);
        free(pDvmDex->pResFields);
        free(pDvmDex);
        return NULL;
    }

    return pDvmDex;

}

int translateClassFile(const char* fileName, DvmDex** ppDvmDex) {
    int result = -1;
    DvmDex* pDvmDex;
    DexFile* pDexFile;
    MemMapping memMap;
    int parseFlags = kDexParseDefault;
    
    if(gDvm.verifyDexChecksum) 
        parseFlags |= kDexParseVerifyChecksum;
    
    if(verbose) fprintf(stderr, "Translate Class File %s\n", fileName);
    ClassFile* cf = readClassFile(fileName);
    if(cf == NULL) goto bail;
    loadDexFromClassFile(cf, &memMap);
    freeAllMem(cf->mm);
    
    pDexFile = dexFileParse(memMap.addr, memMap.length, parseFlags);
    if(pDexFile == NULL) {
        LOGE("DEX parse failed\n");
        goto bail;
    }
    pDvmDex = allocateAuxStructures(pDexFile);
    if(pDvmDex == NULL) {
        dexFileFree(pDexFile);
        sysReleaseShmem(&memMap);
        goto bail;
    }
    sysCopyMap(&pDvmDex->memMap, &memMap);
    *ppDvmDex = pDvmDex;
    result = 0;

bail:
    return result;
}
