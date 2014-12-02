#ifndef TRANSLATOR_DEX_LOADER_H
#define TRANSLATOR_DEX_LOADER_H
#include "Common.h"
#include "ClassFile.h"
#include "DexFormat.h"
#include "../../libdex/sha1.h"
#include <zlib.h>

int loadDexFromClassFile(ClassFile* cf, MemMapping* pMap);
#endif
