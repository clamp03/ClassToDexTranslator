#ifndef TRANSLATOR_H
#define TRANSLATOR_H
#include "Common.h"
#include "ClassFileReader.h"
#include "DexLoader.h"
int vmoClassFileOpen(const char* fileName, const char* odexOutputName, RawDexFile** ppRawDexFile, bool isBootstrap);
int translateClassFile(const char* fileName, DvmDex** ppDvmDex);
#endif
