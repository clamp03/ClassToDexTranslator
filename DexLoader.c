#include "DexLoader.h"
typedef enum {DFL_FALSE = 0, DFL_TRUE = 1}  DFL_bool;
//#if 0

typedef struct {
    DF_u4* stringTable;
    DF_u4* typeTable;
    DF_u4* typeListTable;
    DF_u4* protoTable;
    DF_u4* fieldTable;
    DF_u4* methodTable;
}DFL_SortTable;

typedef struct DFL_PatchNode{
    DF_u4 targetBpc;
    DF_u4 dpc;
    DF_u4 patchDpc;
    DF_u1 offsetSize;
    struct DFL_PatchNode* next;
}DFL_PatchNode;

int DFL_process(ClassFile* cf, MemMapping* pMap);
DexFormat* DFL_processDexFormat(ClassFile* cf);


DF_u4 DFL_parseUtf8Data(DexFormat* df, DF_u2 length, DF_u1* bytes);
DF_u4 DFL_parseStringData(DexFormat* df, CF_CONSTANT_Utf8Info utf8);
DF_u2 DFL_parseTypeUtf8(DexFormat* df, DF_u2 length, DF_u1* types);
DF_u2 DFL_parseClassData(DexFormat* df, CF_CONSTANT_Utf8Info classUtf8);
DF_uleb128 DFL_parseFieldDataUtf8(DexFormat* df, DF_u2 classLen, DF_u1* classUtf8, DF_u2 nameLen, DF_u1* nameUtf8, DF_u2 typeLen, DF_u1* typeUtf8);
DF_uleb128 DFL_parseFieldData(DexFormat* df, CF_CONSTANT_Utf8Info classUtf8, CF_CONSTANT_Utf8Info nameUtf8, CF_CONSTANT_Utf8Info typeUtf8);
DF_s8 DFL_parseParameters(DexFormat* df, DF_u2 size, DF_u2 length, DF_u1* parameter);
DF_u2 DFL_parseProtoData(DexFormat* df, CF_CONSTANT_Utf8Info protoUtf8);
DF_uleb128 DFL_parseMethodData(DexFormat* df, CF_CONSTANT_Utf8Info classUtf8, CF_CONSTANT_Utf8Info nameUtf8, CF_CONSTANT_Utf8Info protoUtf8);
DFL_bool DFL_parseConstantPool(ClassFile* cf, DexFormat* df);

DFL_bool DFL_parseFieldItems(ClassFile* cf, DexFormat* df);
DF_sleb128 DFL_parseCodeAttribute(ClassFile* cf, DexFormat* df, DF_u4 idx);
DFL_bool DFL_parseMethodItems(ClassFile* cf, DexFormat* df);
DFL_bool DFL_parseClassDef(ClassFile* cf, DexFormat* df);

DFL_bool DFL_parseExceptionPair(DexFormat* df, DF_DATA_Handler* handler, DF_sleb128 typeIdx, DF_uleb128 handlerAddr);
DFL_bool DFL_parseExceptionHandler(DexFormat* df, DF_DATA_Code* codeItem, DF_u4 startAddr, DF_u2 insnCount, DF_sleb128 typeIdx, DF_uleb128 handlerAddr, DFL_bool recursive);
DFL_bool DFL_parseExceptionTry(DexFormat* df, DF_DATA_Code* codeItem, DF_u4 startAddr, DF_u2 insnCount, DF_sleb128 typeIdx, DF_uleb128 handlerAddr);
DFL_bool DFL_sortTries(DexFormat* df, DF_DATA_Code* codeItem);
DFL_bool DFL_parseExceptionTable(ClassFile* cf, DexFormat* df, CF_ATTRIBUTE_Code* codeAttr, DF_DATA_Code* codeItem);

DFL_bool DFL_constructBB(ClassFile* cf, DexFormat* df, CF_ATTRIBUTE_Code* codeAttr, DF_IR_BB** table);
DFL_bool DFL_constructCFG(ClassFile* cf, DexFormat* df, CF_ATTRIBUTE_Code* codeAttr, DF_DATA_Code* codeItem);
DFL_bool DFL_emulateJavaStackBB(ClassFile* cf, DexFormat* df, CF_ATTRIBUTE_Code* codeAttr, DF_u4 bpc, DF_u4* stacks, DF_IR_BB** table, DF_u1* hasWide, DF_u1* needTemp);
DFL_bool DFL_emulateJavaStack(ClassFile* cf, DexFormat* df, CF_ATTRIBUTE_Code* codeAttr, DF_DATA_Code* codeItem);
DF_u4 DFL_restoreCopyOpt(DF_u2* insns, DF_u4 dpc, DF_u4* stacks, DF_u2 tos, DF_u2 restore);
DF_u4 DFL_restoreAllCopy(DF_u2* insns, DF_u4 dpc, DF_u4* stacks, DF_u2 tos);
DFL_bool DFL_parseBytecode(ClassFile* cf, DexFormat* df, CF_ATTRIBUTE_Code* codeAttr, DF_DATA_Code* codeItem);
DFL_bool DFL_parseExceptionTable(ClassFile* cf, DexFormat* df, CF_ATTRIBUTE_Code* codeAttr, DF_DATA_Code* codeItem);

DFL_bool DFL_sortStringIds(DexFormat* df, DFL_SortTable* sortTable);
DFL_bool DFL_sortTypeIds(DexFormat* df, DFL_SortTable* sortTable);
DFL_bool DFL_sortTypeList(DexFormat* df, DFL_SortTable* sortTable);
DFL_bool DFL_sortFieldIds(DexFormat* df, DFL_SortTable* sortTable);
DFL_bool DFL_sortProtoIds(DexFormat* df, DFL_SortTable* sortTable);
DFL_bool DFL_sortMethodIds(DexFormat* df, DFL_SortTable* sortTable);
DFL_bool DFL_sortIds(DexFormat* df);
DFL_bool DFL_sortClassDef(ClassFile* cf, DexFormat* df);

DFL_bool DFL_printString(DexFormat* df, DF_u4 i);
DFL_bool DFL_printType(DexFormat* df, DF_u4 i);
DFL_bool DFL_printTypeList(DexFormat* df, DF_s8 i);
DFL_bool DFL_printProtoType(DexFormat* df, DF_u8 i);
DFL_bool DFL_printField(DexFormat* df, DF_u4 i);
DFL_bool DFL_printMethod(DexFormat* df, DF_u4 i);
DFL_bool DFL_printStaticField(DexFormat* df, DF_u4 i);
DFL_bool DFL_printInstanceField(DexFormat* df, DF_u4 i);
DFL_bool DFL_printDirectMethod(DexFormat* df, DF_u4 i);
DFL_bool DFL_printVirtualMethod(DexFormat* df, DF_u4 i);
DFL_bool DFL_printClassData(DexFormat* df);
DFL_bool DFL_printClassDef(DexFormat* df);
DFL_bool DFL_printBinaryFile(DexFormat* df);

DFL_bool DFL_parseClassFile(ClassFile* cf, DexFormat* df);

DFL_bool DFL_writeDexFormat(DexFormat* df);
DFL_bool DFL_printDexFormat(DexFormat* df);

int loadDexFromClassFile(ClassFile* cf, MemMapping* pMap) {
    u1* bytes = NULL;
    if(cf == NULL) return -1;
    if(verbose) fprintf(stderr, "Start <<< Load Dex! >>>\n");
    DFL_process(cf, pMap);
    if(verbose) fprintf(stderr, "Finish <<< Loading Dex! >>>\n");
    return 0;
}
int DFL_process(ClassFile* cf, MemMapping* pMap) {
    DexFormat* df = (DexFormat*)DFL_processDexFormat(cf);
#ifdef HAVE_POSIX_FILEMAP
    void* memPtr = mmap(NULL, df->fileSize, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
#else
    void* memPtr = malloc(df->fileSize);
#endif
    memcpy(memPtr, df->bytes, df->fileSize);
    pMap->baseAddr = pMap->addr = memPtr;
    pMap->baseLength = pMap->length = df->fileSize;
    
    return 0;
}
DexFormat* DFL_processDexFormat(ClassFile* cf) {
    MemoryManager* mm = cf->mm;
    DexFormat* df = (DexFormat*)allocateMem(mm, sizeof(DexFormat));
    memset(df, 0, sizeof(DexFormat));
    df->mm = mm;
    df->fileName = cf->fileName;
    if(!DFL_parseClassFile(cf, df)) return NULL;
    if(!DFL_writeDexFormat(df)) return NULL;
    if(!DFL_printDexFormat(df)) return NULL;
    return df;
}

DF_u4 DFL_parseUtf8Data(DexFormat* df, DF_u2 length, DF_u1* bytes) {
    DF_u4 i, stringItemsSize = df->stringItemsSize;
    DF_DATA_String* strings = df->stringItems;
    for(i = 0 ; i < stringItemsSize ; i++) {
        DF_u1* data = strings[i].data;
        if(strings[i].size == length
            && strcmp((const char*)data, (const char*)bytes) == 0) {
            //if(data != bytes) freeMem(df->mm, bytes);
            return i;
        }
    }
    assert((size_t)length == strlen((const char*)bytes));
    if((stringItemsSize % 10) == 0) {
        int newSize = (stringItemsSize / 10 + 1) * 10;
        DF_DATA_String* newStrinsgs = (DF_DATA_String*)allocateMem(df->mm, sizeof(DF_DATA_String) * newSize);
        memcpy(newStrinsgs, strings, sizeof(DF_DATA_String) * stringItemsSize);
        freeMem(df->mm, strings);
        strings = newStrinsgs;
        df->stringItems = strings;
    }
    strings[stringItemsSize].data = bytes;
    strings[stringItemsSize].size = length;
    
    df->stringItemsLen += (getUleb128Size(length) + length + 1);
    
    return df->stringItemsSize++;
}

DF_u4 DFL_parseStringData(DexFormat* df, CF_CONSTANT_Utf8Info utf8) {
    return DFL_parseUtf8Data(df, utf8.length, utf8.bytes);
}

DF_u2 DFL_parseTypeUtf8(DexFormat* df, DF_u2 length, DF_u1* type) {
    DF_u4 stringIdx = DFL_parseUtf8Data(df, length, type);
    DF_u4 i, typeIdsSize = df->typeIdsSize;
    DF_ID_Type* types = df->typeIds;
    for(i = 0 ; i < typeIdsSize ; i++) {
        DF_u4 idx = types[i].descriptorIdx;
        if(idx == stringIdx) return i;
    }
    if((typeIdsSize % 10) == 0) {
        int newSize = (typeIdsSize / 10 + 1) * 10;
        DF_ID_Type* newTypes = (DF_ID_Type*)allocateMem(df->mm, sizeof(DF_ID_Type) * newSize);
        memcpy(newTypes, types, sizeof(DF_ID_Type) * typeIdsSize);
        freeMem(df->mm, types);
        types = newTypes;
        df->typeIds = types;
    }
    types[typeIdsSize].descriptorIdx = stringIdx;
    return (DF_u2)df->typeIdsSize++;
}

DF_u2 DFL_parseClassData(DexFormat* df, CF_CONSTANT_Utf8Info classUtf8) {
    DF_u1* newBytes = classUtf8.bytes;
    DF_u2 len = classUtf8.length;
    if(((char)newBytes[0]) == '[') {
        DF_u2 i = 0;
        DF_u2 newLen;
        while(((char)newBytes[i]) == '[') i++;
        newLen = len-i;
        if(newLen == 1) {
            switch((char)newBytes[i]) {
                case 'Z':
                {
                    DFL_parseFieldDataUtf8(df, 19, (DF_u1*)"Ljava/lang/Boolean;", 4, (DF_u1*)"TYPE", 17, (DF_u1*)"Ljava/lang/Class;");
                    break;
                }
                case 'C':
                {
                    DFL_parseFieldDataUtf8(df, 16, (DF_u1*)"Ljava/lang/Char;", 4, (DF_u1*)"TYPE", 17, (DF_u1*)"Ljava/lang/Class;");
                    break;
                }
                case 'F':
                {
                    DFL_parseFieldDataUtf8(df, 17, (DF_u1*)"Ljava/lang/Float;", 4, (DF_u1*)"TYPE", 17, (DF_u1*)"Ljava/lang/Class;");
                    break;
                }
                case 'D':
                {
                    DFL_parseFieldDataUtf8(df, 18, (DF_u1*)"Ljava/lang/Double;", 4, (DF_u1*)"TYPE", 17, (DF_u1*)"Ljava/lang/Class;");
                    break;
                }
                case 'B':
                {
                    DFL_parseFieldDataUtf8(df, 16, (DF_u1*)"Ljava/lang/Byte;", 4, (DF_u1*)"TYPE", 17, (DF_u1*)"Ljava/lang/Class;");
                    break;
                }
                case 'S':
                {
                    DFL_parseFieldDataUtf8(df, 17, (DF_u1*)"Ljava/lang/Short;", 4, (DF_u1*)"TYPE", 17, (DF_u1*)"Ljava/lang/Class;");
                    break;
                }
                case 'I':
                {
                    DFL_parseFieldDataUtf8(df, 19, (DF_u1*)"Ljava/lang/Integer;", 4, (DF_u1*)"TYPE", 17, (DF_u1*)"Ljava/lang/Class;");
                    break;
                }
                case 'J':
                {
                    DFL_parseFieldDataUtf8(df, 16, (DF_u1*)"Ljava/lang/Long;", 4, (DF_u1*)"TYPE", 17, (DF_u1*)"Ljava/lang/Class;");
                    break;
                }
            }
        }else {
            DFL_parseTypeUtf8(df, newLen, newBytes + i);
        }
    }else {
    //}else if(((char)newBytes[0]) != 'L') {
        len = len + 2;
        newBytes = (DF_u1*)allocateMem(df->mm, sizeof(u1) * (len + 1));
        strcpy((char*)newBytes, "L");
        strcpy((char*)(newBytes+1), (char*)classUtf8.bytes);
        newBytes[len - 1] = ';';
        newBytes[len] = '\0';
    }
    return (DF_u2)DFL_parseTypeUtf8(df, len, newBytes);
}

DF_u2 DFL_parseTypeData(DexFormat* df, CF_CONSTANT_Utf8Info typeUtf8) {
    return DFL_parseTypeUtf8(df, typeUtf8.length, typeUtf8.bytes);   
}
DF_uleb128 DFL_parseFieldDataUtf8(DexFormat* df, DF_u2 classLen, DF_u1* classUtf8, DF_u2 nameLen, DF_u1* nameUtf8, DF_u2 typeLen, DF_u1* typeUtf8) {
    DF_u2 classIdx = DFL_parseTypeUtf8(df, classLen, classUtf8);
    DF_u4 nameIdx = DFL_parseUtf8Data(df, nameLen, nameUtf8);
    DF_u2 typeIdx = DFL_parseTypeUtf8(df, typeLen, typeUtf8);

    DF_u4 i, fieldIdsSize = df->fieldIdsSize;
    DF_ID_Field* fields= df->fieldIds;
    for(i = 0 ; i < fieldIdsSize ; i++) {
        if(classIdx == fields[i].classIdx 
        && nameIdx == fields[i].nameIdx
        && typeIdx == fields[i].typeIdx) {
            return i;
        }
    }
    if((fieldIdsSize % 10) == 0) {
        int newSize = (fieldIdsSize / 10 + 1) * 10;
        DF_ID_Field* newFields = (DF_ID_Field*)allocateMem(df->mm, sizeof(DF_ID_Field) * newSize);
        memcpy(newFields, fields, sizeof(DF_ID_Field) * fieldIdsSize);
        freeMem(df->mm, fields);
        fields = newFields;
        df->fieldIds= fields;
    }
    fields[fieldIdsSize].classIdx = classIdx;
    fields[fieldIdsSize].nameIdx = nameIdx;
    fields[fieldIdsSize].typeIdx = typeIdx;
    return (DF_uleb128)df->fieldIdsSize++;
}

DF_uleb128 DFL_parseFieldData(DexFormat* df, CF_CONSTANT_Utf8Info classUtf8, CF_CONSTANT_Utf8Info nameUtf8, CF_CONSTANT_Utf8Info typeUtf8) {
    DF_u2 classIdx = DFL_parseClassData(df, classUtf8);
    DF_u4 nameIdx = DFL_parseStringData(df, nameUtf8);
    DF_u2 typeIdx = DFL_parseTypeData(df, typeUtf8);

    DF_u4 i, fieldIdsSize = df->fieldIdsSize;
    DF_ID_Field* fields= df->fieldIds;
    for(i = 0 ; i < fieldIdsSize ; i++) {
        if(classIdx == fields[i].classIdx 
        && nameIdx == fields[i].nameIdx
        && typeIdx == fields[i].typeIdx) {
            return i;
        }
    }
    if((fieldIdsSize % 10) == 0) {
        int newSize = (fieldIdsSize / 10 + 1) * 10;
        DF_ID_Field* newFields = (DF_ID_Field*)allocateMem(df->mm, sizeof(DF_ID_Field) * newSize);
        memcpy(newFields, fields, sizeof(DF_ID_Field) * fieldIdsSize);
        freeMem(df->mm, fields);
        fields = newFields;
        df->fieldIds= fields;
    }
    fields[fieldIdsSize].classIdx = classIdx;
    fields[fieldIdsSize].nameIdx = nameIdx;
    fields[fieldIdsSize].typeIdx = typeIdx;
    return (DF_uleb128)df->fieldIdsSize++;
}

DF_u1 DFL_getShortyType(DexFormat* df, DF_u1* type) {
    if(type[0] == '[') return 'L';
    return type[0];
}
DF_s8 DFL_parseParameters(DexFormat* df, DF_u2 size, DF_u2 length, DF_u1* parameter) {
    assert(size != 0);
    DF_u4 i, j, typeListSize = df->typeListSize;
    DF_DATA_Type* typeItems = (DF_DATA_Type*)allocateMem(df->mm, sizeof(DF_DATA_Type) * size);
    DF_DATA_TypeList* typeList = df->typeList;
    j = 0;
    for(i = 0 ; i < length ; i++) {
        switch(parameter[i]) {
            case '[':
            {
                DF_u2 start = i;
                DF_u2 len;
                while(parameter[i] == '[') i++;
                if(parameter[i] == 'L') {
                    while(parameter[i] != ';' && i < length) i++;
                }
                len = i - start + 1;

                DF_u1* paraType = (DF_u1*)allocateMem(df->mm, sizeof(DF_u1) * (len + 1));
                strncpy((char*)paraType, (const char*)(parameter + start), (size_t)len);
                paraType[len] = '\0';
                typeItems[j].typeIdx = DFL_parseTypeUtf8(df, len, paraType);
                break;
            }
            case 'L':
            {
                DF_u2 start = i;
                DF_u2 len;
                while(parameter[i] != ';' && i < length) i++;
                len = i - start + 1;
                
                DF_u1* paraType = (DF_u1*)allocateMem(df->mm, sizeof(DF_u1) * (len + 1));
                strncpy((char*)paraType, (const char*)(parameter + start), (size_t)len);
                paraType[len] = '\0';
                typeItems[j].typeIdx = DFL_parseTypeUtf8(df, len, paraType);
                break;
            }
            default:
            {
                DF_u1* paraType = (DF_u1*)allocateMem(df->mm ,sizeof(DF_u1) * 2);;
                paraType[0] = parameter[i];
                paraType[1] = '\0';
                typeItems[j].typeIdx = DFL_parseTypeUtf8(df, 1, paraType);
                break;
            }
        }
        j++;
    }
    for(i = 0 ; i < typeListSize ; i++) {
        if(size == df->typeList[i].size) {
            DF_u4 match = size;
            for(j = 0 ; j < size ; j++) {
                if(typeItems[j].typeIdx == df->typeList[i].typeItems[j].typeIdx) {
                    match--;
                }else break;
            }
            if(match == 0) goto isMatch;
        }
    }
    if((typeListSize % 10) == 0) {
        int newSize = (typeListSize / 10 + 1) * 10;
        DF_DATA_TypeList* newTypeList = (DF_DATA_TypeList*)allocateMem(df->mm, sizeof(DF_DATA_TypeList) * newSize);
        memcpy(newTypeList, typeList, sizeof(DF_DATA_TypeList) * typeListSize);
        /*
        if(typeListSize == 0) {
            df->typeListSize = 1;
            typeListSize = 1;
        }
        */
        freeMem(df->mm, typeList);
        typeList = newTypeList;
        df->typeList = typeList;
    }
    typeList[typeListSize].size = size;
    typeList[typeListSize].typeItems = typeItems;
    df->typeListLen += (4 + 2 * size + 4);
    if((size % 2) != 0) df->typeListLen += 2;
    return (DF_s8)df->typeListSize++;
isMatch:
    freeMem(df->mm, typeItems);
    if(length > 1 ||  parameter[0] != 'V') {
        freeMem(df->mm, parameter);
    }
    return i;
}
DF_u2 DFL_parseProtoDataUtf8(DexFormat* df, DF_u2 len, DF_u1* protoUtf8) {
    DF_u2 length = len;
    DF_u1* bytes = protoUtf8;
    DF_u1* shortType;
    DF_u4 shortStringIdx, returnTypeIdx; 
    DF_s8 parameterTypeListIdx = -1;
    // Parameter Count
    DF_u4 i;
    DF_u4 parameterCount = 0;
    for(i = 1 ; bytes[i] != ')' ; i++) {
        switch(bytes[i]) {
            case '[':
                while(bytes[i+1] == '[') i++;
                break;
            case 'L':
                while(bytes[i] != ';') i++;
            default:
                parameterCount++;
                break;
        }
    }
    shortType = allocateMem(df->mm, sizeof(u1) * (parameterCount + 2));
    shortType[parameterCount + 1] = '\0';
    // Parameter
    {
        // Parameters 
        DF_u2 paraLength = i - 1;
        if(paraLength != 0) {
            DF_u1* paraType = allocateMem(df->mm, sizeof(u1) * (paraLength + 1));
            strncpy((char*)paraType, (const char*)(bytes + 1), (size_t)paraLength);
            paraType[paraLength] = '\0';
            parameterTypeListIdx = DFL_parseParameters(df, parameterCount, paraLength, paraType);
        }
        // Parameter Shorty
        DF_u4 j = 1;
        for(i = 1 ; bytes[i] != ')' ; i++) {
            switch(bytes[i]) {
                case '[':
                    shortType[j++] = 'L';
                    while(bytes[i] == '[') i++;
                    if(bytes[i] == 'L') {while(bytes[i] != ';') i++;}
                    break;
                case 'L':
                    shortType[j++] = 'L';
                    while(bytes[i] != ';') i++;
                    break;
                default:
                    shortType[j++] = bytes[i];
                    break;
            }
        }
    }
    // Return
    DF_u1* retType;
    {
        DF_u2 retLength = length - (++i);
        retType = allocateMem(df->mm, sizeof(u1) * (retLength + 1));
        strncpy((char*)retType, (const char*)(bytes + i), (size_t)retLength);
        retType[retLength] = '\0';
        returnTypeIdx = DFL_parseTypeUtf8(df, retLength, retType);

        // Reutnr SHorty Type
        if(bytes[i] == '[') {
            shortType[0] = 'L';
        }else {
            shortType[0] = bytes[i];
        }
    }
    // Shorty
    shortStringIdx = DFL_parseUtf8Data(df, parameterCount + 1, shortType);
    {
        DF_ID_Proto* protoIds = df->protoIds;
        DF_u4 protoIdsSize = df->protoIdsSize;
        for(i = 0 ; i < protoIdsSize ; i++) {
            if(protoIds[i].shortyIdx == shortStringIdx
            && protoIds[i].returnTypeIdx == returnTypeIdx
            && protoIds[i].parametersOff == parameterTypeListIdx) {
                freeMem(df->mm, shortType);
                freeMem(df->mm, retType);
                return i;
            }
        }
        if((protoIdsSize % 10) == 0) {
            int newSize = (protoIdsSize / 10 + 1) * 10;
            DF_ID_Proto* newProtoIds = (DF_ID_Proto*)allocateMem(df->mm, sizeof(DF_ID_Proto) * newSize);
            memcpy(newProtoIds, protoIds, sizeof(DF_ID_Proto) * protoIdsSize);
            freeMem(df->mm, protoIds);
            protoIds = newProtoIds;
            df->protoIds = newProtoIds;
        }
        protoIds[protoIdsSize].shortyIdx = shortStringIdx;
        protoIds[protoIdsSize].returnTypeIdx = returnTypeIdx;
        protoIds[protoIdsSize].parametersOff = parameterTypeListIdx;
    }
    return df->protoIdsSize++;

}
DF_u2 DFL_parseProtoData(DexFormat* df, CF_CONSTANT_Utf8Info protoUtf8) {
    DF_u2 length = protoUtf8.length;
    DF_u1* bytes = protoUtf8.bytes;
    return DFL_parseProtoDataUtf8(df, length, bytes);
#if 0
    DF_u1* shortType;
    DF_u4 shortStringIdx, returnTypeIdx; 
    DF_s8 parameterTypeListIdx = -1;
    // Parameter Count
    DF_u4 i;
    DF_u4 parameterCount = 0;
    for(i = 1 ; bytes[i] != ')' ; i++) {
        switch(bytes[i]) {
            case '[':
            case 'L':
                while(bytes[i] != ';') i++;
            default:
                parameterCount++;
                break;
        }
    }
    shortType = allocateMem(df->mm, sizeof(DF_u1) * (parameterCount + 2));
    shortType[parameterCount + 1] = '\0';
    // Parameter
    {
        // Parameters 
        DF_u2 paraLength = i - 1;
        if(paraLength != 0) {
            DF_u1* paraType = allocateMem(df->mm, sizeof(u1) * (paraLength + 1));
            strncpy((char*)paraType, (const char*)(bytes + 1), (size_t)paraLength);
            paraType[paraLength] = '\0';
            parameterTypeListIdx = DFL_parseParameters(df, parameterCount, paraLength, paraType);
        }

        // Parameter Shorty
        DF_u4 j = 1;
        for(i = 1 ; bytes[i] != ')' ; i++) {
            switch(bytes[i]) {
                case '[':
                case 'L':
                    shortType[j++] = 'L';
                    while(bytes[i] != ';') i++;
                    break;
                default:
                    shortType[j++] = bytes[i];
                    break;
            }
        }
    }
    // Return
    DF_u1* retType;
    {
        DF_u2 retLength = length - (++i);
        retType = allocateMem(df->mm, sizeof(u1) * (retLength + 1));
        strncpy((char*)retType, (const char*)(bytes + i), (size_t)retLength);
        retType[retLength] = '\0';
        returnTypeIdx = DFL_parseTypeUtf8(df, retLength, retType);

        // Reutnr SHorty Type
        if(bytes[i] == '[') {
            shortType[0] = 'L';
        }else {
            shortType[0] = bytes[i];
        }
    }
    // Shorty
    shortStringIdx = DFL_parseUtf8Data(df, parameterCount + 1, shortType);
    {
        DF_ID_Proto* protoIds = df->protoIds;
        DF_u4 protoIdsSize = df->protoIdsSize;
        for(i = 0 ; i < protoIdsSize ; i++) {
            if(protoIds[i].shortyIdx == shortStringIdx
            && protoIds[i].returnTypeIdx == returnTypeIdx
            && protoIds[i].parametersOff == parameterTypeListIdx) {
                freeMem(df->mm, shortType);
                freeMem(df->mm, retType);
                return i;
            }
        }
        if((protoIdsSize % 10) == 0) {
            int newSize = (protoIdsSize / 10 + 1) * 10;
            DF_ID_Proto* newProtoIds = (DF_ID_Proto*)allocateMem(df->mm, sizeof(DF_ID_Proto) * newSize);
            memcpy(newProtoIds, protoIds, sizeof(DF_ID_Proto) * protoIdsSize);
            freeMem(df->mm, protoIds);
            protoIds = newProtoIds;
            df->protoIds = newProtoIds;
        }
        protoIds[protoIdsSize].shortyIdx = shortStringIdx;
        protoIds[protoIdsSize].returnTypeIdx = returnTypeIdx;
        protoIds[protoIdsSize].parametersOff = parameterTypeListIdx;
    }
    return df->protoIdsSize++;
#endif
}
DF_uleb128 DFL_parseMethodDataUtf8(DexFormat* df, DF_u2 classLen, DF_u1* classUtf8, DF_u2 nameLen, DF_u1* nameUtf8, DF_u2 protoLen, DF_u1* protoUtf8) {
    DF_u2 classIdx = DFL_parseTypeUtf8(df, classLen, classUtf8);
    DF_u4 nameIdx = DFL_parseUtf8Data(df, nameLen, nameUtf8);
    DF_u2 protoIdx = DFL_parseProtoDataUtf8(df, protoLen, protoUtf8);
    
    DF_u4 i, methodIdsSize = df->methodIdsSize;
    DF_ID_Method* methods = df->methodIds;
    for(i = 0 ; i < methodIdsSize ; i++) {
        if(classIdx == methods[i].classIdx 
        && nameIdx == methods[i].nameIdx
        && protoIdx == methods[i].protoIdx) {
            return i;
        }
    }
    if((methodIdsSize % 10) == 0) {
        int newSize = (methodIdsSize / 10 + 1) * 10;
        DF_ID_Method* newMethods = allocateMem(df->mm, sizeof(DF_ID_Method) * newSize);
        memcpy(newMethods, methods, sizeof(DF_ID_Method) * methodIdsSize);
        freeMem(df->mm, methods);
        methods = newMethods;
        df->methodIds= methods;
    }
    methods[methodIdsSize].classIdx = classIdx;
    methods[methodIdsSize].nameIdx = nameIdx;
    methods[methodIdsSize].protoIdx = protoIdx;
    return (DF_uleb128)df->methodIdsSize++;

}
DF_uleb128 DFL_parseMethodData(DexFormat* df, CF_CONSTANT_Utf8Info classUtf8, CF_CONSTANT_Utf8Info nameUtf8, CF_CONSTANT_Utf8Info protoUtf8) {
    DF_u2 classIdx = DFL_parseClassData(df, classUtf8);
    DF_u4 nameIdx = DFL_parseStringData(df, nameUtf8);
    DF_u2 protoIdx = DFL_parseProtoData(df, protoUtf8);

    DF_u4 i, methodIdsSize = df->methodIdsSize;
    DF_ID_Method* methods = df->methodIds;
    for(i = 0 ; i < methodIdsSize ; i++) {
        if(classIdx == methods[i].classIdx 
        && nameIdx == methods[i].nameIdx
        && protoIdx == methods[i].protoIdx) {
            return i;
        }
    }
    if((methodIdsSize % 10) == 0) {
        int newSize = (methodIdsSize / 10 + 1) * 10;
        DF_ID_Method* newMethods = allocateMem(df->mm, sizeof(DF_ID_Method) * newSize);
        memcpy(newMethods, methods, sizeof(DF_ID_Method) * methodIdsSize);
        freeMem(df->mm, methods);
        methods = newMethods;
        df->methodIds= methods;
    }
    methods[methodIdsSize].classIdx = classIdx;
    methods[methodIdsSize].nameIdx = nameIdx;
    methods[methodIdsSize].protoIdx = protoIdx;
    return (DF_uleb128)df->methodIdsSize++;
}

DFL_bool DFL_parseConstantPool(ClassFile* cf, DexFormat* df) {
    DF_s4 i = 0, j;
    DF_s4 count = cf->constantPoolCount;
    CF_ConstantPool* cp = cf->cp;
    if(verbose) fprintf(stderr, "Start <Parse Constant Pool>\n");
    //if(cf->sourceFile != NULL) DFL_parseStringData(df, *cf->sourceFile);
    for(i = 1 ; i <= count ; i++) {
        CF_u1 tag = cp[i].tag;
        if(verbose) fprintf(stderr, "[%2d/%2d] ", i, cp[i].tag);
        switch(tag) {
            case CF_CONSTANT_Utf8Type:
            {
                // DO NOTHING
                break;
            }
            case CF_CONSTANT_IntegerType:
            {
                // DO NOTHING
                //if(verbose) fprintf(stderr, "Not Yet Implemented!\n");
                break;
            }
            case CF_CONSTANT_FloatType:
            {
                // DO NOTHING
                //if(verbose) fprintf(stderr, "Not Yet Implemented!\n");
                break;
            }
            case CF_CONSTANT_LongType:
            {
                // DO NOTHING
                //if(verbose) fprintf(stderr, "Not Yet Implemented!\n");a
                i++;
                break;
            }
            case CF_CONSTANT_DoubleType:
            {
                // DO NOTHING
                //if(verbose) fprintf(stderr, "Not Yet Implemented!\n");
                i++;
                break;
            }
            case CF_CONSTANT_ClassrefType:
            {
                DF_u2 utf8Idx = cp[i].type.classInfo.nameIndex;
                CF_CONSTANT_Utf8Info classNameUtf8 = cp[utf8Idx].type.utf8Info;
                if(verbose) fprintf(stderr, "Class Constant\n");
                DFL_parseClassData(df, classNameUtf8);
                break;
            }
            case CF_CONSTANT_StringType:
            {
                DF_u2 utf8Idx = cp[i].type.stringInfo.stringIndex;
                CF_CONSTANT_Utf8Info utf8Const = cp[utf8Idx].type.utf8Info;
                if(verbose) fprintf(stderr, "String Constant %s\n", utf8Const.bytes);
                DFL_parseStringData(df, utf8Const);
                break;
            }
            case CF_CONSTANT_FieldrefType:
            {
                DF_u2 classIdx = cp[i].type.fieldInfo.classIndex;
                DF_u2 nameAndTypeIdx = cp[i].type.fieldInfo.nameAndTypeIndex;
                CF_CONSTANT_Utf8Info classNameUtf8 = cp[cp[classIdx].type.classInfo.nameIndex].type.utf8Info;
                DF_u2 nameIdx = cp[nameAndTypeIdx].type.nameAndTypeInfo.nameIndex;
                DF_u2 typeIdx = cp[nameAndTypeIdx].type.nameAndTypeInfo.descriptorIndex;
                CF_CONSTANT_Utf8Info nameUtf8 = cp[nameIdx].type.utf8Info;
                CF_CONSTANT_Utf8Info descriptorUtf8 = cp[typeIdx].type.utf8Info;
                if(verbose) fprintf(stderr, "Field Constant\n");
                DFL_parseFieldData(df, classNameUtf8, nameUtf8, descriptorUtf8);
                break;
            }
            case CF_CONSTANT_MethodrefType:
            {
                DF_u2 classIdx = cp[i].type.methodInfo.classIndex;
                DF_u2 nameAndTypeIdx = cp[i].type.methodInfo.nameAndTypeIndex;
                CF_CONSTANT_Utf8Info classNameUtf8 = cp[cp[classIdx].type.classInfo.nameIndex].type.utf8Info;
                DF_u2 nameIdx = cp[nameAndTypeIdx].type.nameAndTypeInfo.nameIndex;
                DF_u2 typeIdx = cp[nameAndTypeIdx].type.nameAndTypeInfo.descriptorIndex;
                CF_CONSTANT_Utf8Info nameUtf8 = cp[nameIdx].type.utf8Info;
                CF_CONSTANT_Utf8Info descriptorUtf8 = cp[typeIdx].type.utf8Info;
                if(verbose) fprintf(stderr, "Method Constant\n");
                DFL_parseMethodData(df, classNameUtf8, nameUtf8, descriptorUtf8);

                break;
            }
            case CF_CONSTANT_InterfaceMethodrefType:
            {
                DF_u2 classIdx = cp[i].type.interfaceInfo.classIndex;
                DF_u2 nameAndTypeIdx = cp[i].type.interfaceInfo.nameAndTypeIndex;
                CF_CONSTANT_Utf8Info classNameUtf8 = cp[cp[classIdx].type.classInfo.nameIndex].type.utf8Info;
                DF_u2 nameIdx = cp[nameAndTypeIdx].type.nameAndTypeInfo.nameIndex;
                DF_u2 typeIdx = cp[nameAndTypeIdx].type.nameAndTypeInfo.descriptorIndex;
                CF_CONSTANT_Utf8Info nameUtf8 = cp[nameIdx].type.utf8Info;
                CF_CONSTANT_Utf8Info descriptorUtf8 = cp[typeIdx].type.utf8Info;
                if(verbose) fprintf(stderr, "Interface Constant\n");
                DFL_parseMethodData(df, classNameUtf8, nameUtf8, descriptorUtf8);

                break;
            }
            case CF_CONSTANT_NameAndTypeType:
            {
                // DO NOTHING
                break;
            }
            default:
            {
                assert(0);
                break;
            }
        }
    }
#if 0
    {
        DF_u2 utf8Idx = cf->cp[cf->thisClass].type.classInfo.nameIndex;
        CF_CONSTANT_Utf8Info classUtf8 = cf->cp[utf8Idx].type.utf8Info;
        for(i = 0 ;  i < cf->methodsCount ; i++) {
            CF_Method method = cf->methods[i];
            CF_CONSTANT_Utf8Info nameUtf8 = cf->cp[method.nameIndex].type.utf8Info;
            CF_CONSTANT_Utf8Info typeUtf8 = cf->cp[method.descriptorIndex].type.utf8Info;
            DF_uleb128 methodIdx = DFL_parseMethodData(df, classUtf8, nameUtf8, typeUtf8);
            {
                DF_u4 bpc = 0;
                DF_u1* bcodes = method.codeAttr.code;
                DF_u4 bLength = method.codeAttr.codeLength;
                for(bpc = 0 ; bpc < bLength ; bpc += CF_getBytecodeLength(bcodes, bpc)) {
                    switch(bcodes[bpc]) {
                        case CF_NEWARRAY:
                        {
                            DF_u1 type = bcodes[bpc+1];
                            DF_u1* typeUtf8 = NULL;
                            switch(type) {
                                case 4:
                                {
                                    typeUtf8 = (DF_u1*)"[Z";
                                    break;
                                }
                                case 5:
                                {
                                    typeUtf8 = (DF_u1*)"[C";
                                    break;
                                }
                                case 6:
                                {
                                    typeUtf8 = (DF_u1*)"[F";
                                    break;
                                }
                                case 7:
                                {
                                    typeUtf8 = (DF_u1*)"[D";
                                    break;
                                }
                                case 8:
                                {
                                    typeUtf8 = (DF_u1*)"[B";
                                    break;
                                }
                                case 9:
                                {
                                    typeUtf8 = (DF_u1*)"[S";
                                    break;
                                }
                                case 10:
                                {
                                    typeUtf8 = (DF_u1*)"[I";
                                    break;
                                }
                                case 11:
                                {
                                    typeUtf8 = (DF_u1*)"[J";
                                    break;
                                }
                                default:
                                    assert(0);
                                    break;
                            }
                            DFL_parseTypeUtf8(df, 2, typeUtf8);
                            break;
                        }
                        case CF_ANEWARRAY:
                        {
                            CF_CONSTANT_Utf8Info classUtf8 = cp[cp[CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2])].type.classInfo.nameIndex].type.utf8Info;
                            DF_u2 len = classUtf8.length + 3;
                            DF_u1* newBytes = (DF_u1*)allocateMem(df->mm, sizeof(DF_u1) * (len + 1));
                            strcpy((char*)newBytes, "[L");
                            strcpy((char*)(newBytes + 2), (char*)classUtf8.bytes);
                            newBytes[len - 1] = ';';
                            newBytes[len] = '\0';
                            DFL_parseTypeUtf8(df, len, newBytes);
                            break;
                        }
                        case CF_MULTIANEWARRAY:
                        {
                            DFL_parseTypeUtf8(df, 2, (DF_u1*)"[I");
                            DFL_parseMethodDataUtf8(df, 25, (DF_u1*)"Ljava/lang/reflect/Array;", 11, (DF_u1*)"newInstance", 39, (DF_u1*)"(Ljava/lang/Class;[I)Ljava/lang/Object;");
                            break;
                        }
                        case CF_BALOAD:
                        case CF_BASTORE:
                        {
                            DFL_parseTypeUtf8(df, 2, (DF_u1*)"[B");
                            break;
                        }
                    }
                }
            }
        }
        for(i = 0 ; i < cf->fieldsCount ; i++) {
            CF_Field field = cf->fields[i];
            CF_CONSTANT_Utf8Info nameUtf8 = cf->cp[field.nameIndex].type.utf8Info;
            CF_CONSTANT_Utf8Info typeUtf8 = cf->cp[field.descriptorIndex].type.utf8Info;
            DFL_parseFieldData(df, classUtf8, nameUtf8, typeUtf8);
        }
    }
#endif
    return DFL_TRUE;
}

DFL_bool DFL_parseAdditionalConstant(ClassFile* cf, DexFormat* df) {
    DF_s4 i = 0, j;
    CF_ConstantPool* cp = cf->cp;
    DF_u2 utf8Idx = cp[cf->thisClass].type.classInfo.nameIndex;
    CF_CONSTANT_Utf8Info classUtf8 = cp[utf8Idx].type.utf8Info;
    // Method
    for(i = 0 ;  i < cf->methodsCount ; i++) {
        CF_Method method = cf->methods[i];
        CF_CONSTANT_Utf8Info nameUtf8 = cp[method.nameIndex].type.utf8Info;
        CF_CONSTANT_Utf8Info typeUtf8 = cp[method.descriptorIndex].type.utf8Info;
        DF_uleb128 methodIdx = DFL_parseMethodData(df, classUtf8, nameUtf8, typeUtf8);
        {
            DF_u4 bpc = 0;
            DF_u1* bcodes = method.codeAttr.code;
            DF_u4 bLength = method.codeAttr.codeLength;
            for(bpc = 0 ; bpc < bLength ; bpc += CF_getBytecodeLength(bcodes, bpc)) {
                switch(bcodes[bpc]) {
                    case CF_NEWARRAY:
                        {
                            DF_u1 type = bcodes[bpc+1];
                            DF_u1* typeUtf8 = NULL;
                            switch(type) {
                                case 4:
                                    {
                                        typeUtf8 = (DF_u1*)"[Z";
                                        break;
                                    }
                                case 5:
                                    {
                                        typeUtf8 = (DF_u1*)"[C";
                                        break;
                                    }
                                case 6:
                                    {
                                        typeUtf8 = (DF_u1*)"[F";
                                        break;
                                    }
                                case 7:
                                    {
                                        typeUtf8 = (DF_u1*)"[D";
                                        break;
                                    }
                                case 8:
                                    {
                                        typeUtf8 = (DF_u1*)"[B";
                                        break;
                                    }
                                case 9:
                                    {
                                        typeUtf8 = (DF_u1*)"[S";
                                        break;
                                    }
                                case 10:
                                    {
                                        typeUtf8 = (DF_u1*)"[I";
                                        break;
                                    }
                                case 11:
                                    {
                                        typeUtf8 = (DF_u1*)"[J";
                                        break;
                                    }
                                default:
                                    assert(0);
                                    break;
                            }
                            DFL_parseTypeUtf8(df, 2, typeUtf8);
                            break;
                        }
                    case CF_ANEWARRAY:
                        {
                            CF_CONSTANT_Utf8Info classUtf8 = cp[cp[CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2])].type.classInfo.nameIndex].type.utf8Info;
                            DF_u2 len = classUtf8.length + 3;
                            DF_u1* newBytes = (DF_u1*)allocateMem(df->mm, sizeof(DF_u1) * (len + 1));
                            strcpy((char*)newBytes, "[L");
                            strcpy((char*)(newBytes + 2), (char*)classUtf8.bytes);
                            newBytes[len - 1] = ';';
                            newBytes[len] = '\0';
                            DFL_parseTypeUtf8(df, len, newBytes);
                            break;
                        }
                    case CF_MULTIANEWARRAY:
                        {
                            DFL_parseTypeUtf8(df, 2, (DF_u1*)"[I");
                            DFL_parseMethodDataUtf8(df, 25, (DF_u1*)"Ljava/lang/reflect/Array;", 11, (DF_u1*)"newInstance", 39, (DF_u1*)"(Ljava/lang/Class;[I)Ljava/lang/Object;");
                            break;
                        }
                    case CF_BALOAD:
                    case CF_BASTORE:
                        {
                            DFL_parseTypeUtf8(df, 2, (DF_u1*)"[B");
                            break;
                        }
                }
            }
        }
    }
    // Field
    for(i = 0 ; i < cf->fieldsCount ; i++) {
        CF_Field field = cf->fields[i];
        CF_CONSTANT_Utf8Info nameUtf8 = cp[field.nameIndex].type.utf8Info;
        CF_CONSTANT_Utf8Info typeUtf8 = cp[field.descriptorIndex].type.utf8Info;
        DFL_parseFieldData(df, classUtf8, nameUtf8, typeUtf8);
    }
    // Interface
    if(cf->interfacesCount > 0) {
        DF_s4 size = cf->interfacesCount;
        DF_DATA_Type* typeItems = (DF_DATA_Type*)allocateMem(df->mm, sizeof(DF_DATA_Type) * size);
        for(i = 0 ; i < size ; i++) {
            DF_u2 utf8Idx = cp[cf->interfaces[i]].type.classInfo.nameIndex;
            DF_u2 idx = DFL_parseClassData(df, cp[utf8Idx].type.utf8Info);
            typeItems[i].typeIdx = idx;
        }
        DF_u4 typeListSize = df->typeListSize;
        DF_DATA_TypeList* typeList = df->typeList;
        if((typeListSize % 10) == 0) {
            int newSize = (typeListSize / 10 + 1) * 10;
            DF_DATA_TypeList* newTypeList = (DF_DATA_TypeList*)allocateMem(df->mm, sizeof(DF_DATA_TypeList) * newSize);
            memcpy(newTypeList, typeList, sizeof(DF_DATA_TypeList) * typeListSize);
            freeMem(df->mm, typeList);
            typeList = newTypeList;
            df->typeList = typeList;
        }
        typeList[typeListSize].size = size;
        typeList[typeListSize].typeItems = typeItems;
        df->typeListLen += (4 + 2 * size + 4);
        if((size % 2) != 0) df->typeListLen += 2;
        df->interfacesOff = df->typeListSize++;
    }else df->interfacesOff = -1;
    return DFL_TRUE;
}

DFL_bool DFL_parseFieldItems(ClassFile* cf, DexFormat* df) {
    DF_DATA_Class* classItems = df->classItems;
    DF_u2 utf8Idx = cf->cp[cf->thisClass].type.classInfo.nameIndex;
    CF_CONSTANT_Utf8Info classUtf8 = cf->cp[utf8Idx].type.utf8Info;
    
    DF_u2 staticFieldsSize = 0;
    DF_u4 staticIdx = 0, instanceIdx = 0;
    DF_s4 i, j;
    for(i = 0 ; i < (DF_s4)cf->fieldsCount ; i++) {
        CF_Field field = cf->fields[i];
        if((field.accessFlags & CF_ACC_STATIC) != 0) {
            staticFieldsSize++;
        }
    }
    // Static Field
    classItems->staticFieldsSize = staticFieldsSize;
    classItems->staticFields = (DF_DATA_Field*)allocateMem(df->mm, sizeof(DF_DATA_Field) * classItems->staticFieldsSize);
    df->classItemsLen += getUleb128Size(classItems->staticFieldsSize);
    // Instance Field
    classItems->instanceFieldsSize = cf->fieldsCount - staticFieldsSize;
    classItems->instanceFields = (DF_DATA_Field*)allocateMem(df->mm, sizeof(DF_DATA_Field) * classItems->instanceFieldsSize);
    df->classItemsLen += getUleb128Size(classItems->instanceFieldsSize);
    // DATA
    for(i = 0 ; i < cf->fieldsCount ; i++) {
        CF_Field cfField = cf->fields[i];
        CF_u2 cfAccessFlags = cfField.accessFlags;
        CF_CONSTANT_Utf8Info nameUtf8 = cf->cp[cfField.nameIndex].type.utf8Info;
        CF_CONSTANT_Utf8Info typeUtf8 = cf->cp[cfField.descriptorIndex].type.utf8Info;
        DF_DATA_Field* dfField;
        if((cfAccessFlags & CF_ACC_STATIC) != 0) {
            dfField = &classItems->staticFields[staticIdx];
            staticIdx++;
        }else {
            dfField = &classItems->instanceFields[instanceIdx];
            instanceIdx++;
        }
        dfField->idxDiff = DFL_parseFieldData(df, classUtf8, nameUtf8, typeUtf8);
        dfField->accessFlags = (DF_uleb128)cfAccessFlags;
        df->classItemsLen += (getUleb128Size(dfField->accessFlags) + getUleb128Size(dfField->idxDiff));
    }
    /*
    DF_uleb128 prevIdx = 0;
    for(i = 0 ; i < (DF_s4)classItems->staticFieldsSize; i++) {
        DF_DATA_Field* dfField = &classItems->staticFields[i];
        for(j = i + 1 ; j < (DF_s4)classItems->staticFieldsSize ; j++) {
            if(classItems->staticFields[i].idxDiff > classItems->staticFields[j].idxDiff) {
                DF_uleb128 idxDiff = dfField->idxDiff;
                DF_uleb128 accessFlags = dfField->accessFlags;

                dfField->idxDiff = classItems->staticFields[j].idxDiff;
                dfField->accessFlags = classItems->staticFields[j].accessFlags;

                classItems->staticFields[j].idxDiff = idxDiff;
                classItems->staticFields[j].accessFlags = accessFlags;
            }
        }
        dfField->idxDiff = dfField->idxDiff - prevIdx;
        prevIdx = dfField->idxDiff + prevIdx;
        df->classItemsLen += (getUleb128Size(dfField->accessFlags) + getUleb128Size(dfField->idxDiff));
    }
    prevIdx = 0;
    for(i = 0 ; i < (DF_s4)classItems->instanceFieldsSize; i++) {
        DF_DATA_Field* dfField = &classItems->instanceFields[i];
        for(j = i + 1 ; j < (DF_s4)classItems->instanceFieldsSize ; j++) {
            if(classItems->instanceFields[i].idxDiff > classItems->instanceFields[j].idxDiff) {
                DF_uleb128 idxDiff = dfField->idxDiff;
                DF_uleb128 accessFlags = dfField->accessFlags;

                dfField->idxDiff = classItems->instanceFields[j].idxDiff;
                dfField->accessFlags = classItems->instanceFields[j].accessFlags;

                classItems->instanceFields[j].idxDiff = idxDiff;
                classItems->instanceFields[j].accessFlags = accessFlags;
            }
        }
        dfField->idxDiff = dfField->idxDiff - prevIdx;
        prevIdx = dfField->idxDiff + prevIdx;
        df->classItemsLen += (getUleb128Size(dfField->accessFlags) + getUleb128Size(dfField->idxDiff));
    }
    */
    return DFL_TRUE;
}

DFL_bool DFL_constructBB(ClassFile* cf, DexFormat* df, CF_ATTRIBUTE_Code* codeAttr, DF_IR_BB** table) {
    DF_u4 bpc = 0;
    DF_u1* bcodes = codeAttr->code;
    DF_u4 bLength =codeAttr->codeLength;
    while(bpc < bLength) {
        DF_u4 nextBpc = bpc + CF_getBytecodeLength(bcodes, bpc);
        switch(bcodes[bpc]) {
            case CF_IFNULL:
            case CF_IFNONNULL:
            case CF_IFEQ:
            case CF_IFNE:
            case CF_IFLT:
            case CF_IFGE:
            case CF_IFGT:
            case CF_IFLE:
            case CF_IF_ICMPEQ:
            case CF_IF_ICMPNE:
            case CF_IF_ICMPLT:
            case CF_IF_ICMPGE:
            case CF_IF_ICMPGT:
            case CF_IF_ICMPLE:
            case CF_IF_ACMPEQ:
            case CF_IF_ACMPNE:
            {
                DF_s2 offset = CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2]);
                DF_u4 targetBpc = bpc + offset;
                if(targetBpc < bLength && table[targetBpc] == NULL) {
                    table[targetBpc] = (DF_IR_BB*)allocateMem(df->mm, sizeof(DF_IR_BB));
                    table[targetBpc]->bpc = targetBpc;
                    table[targetBpc]->stacks = (DF_u4*)allocateMem(df->mm, sizeof(DF_u4) * codeAttr->maxStack);
                }
                if(nextBpc < bLength && table[nextBpc] == NULL) {
                    table[nextBpc] = (DF_IR_BB*)allocateMem(df->mm, sizeof(DF_IR_BB));
                    table[nextBpc]->bpc = nextBpc;
                    table[nextBpc]->stacks = (DF_u4*)allocateMem(df->mm, sizeof(DF_u4) * codeAttr->maxStack);
                }
                break;
            }
            case CF_LOOKUPSWITCH:
            {
                DF_u4 addr = (bpc+4) & 0xFFFFFFFC;
                DF_u4 pads = addr - bpc - 1;
                DF_u4 npairs = CF_makeIntVal(bcodes[addr+4], bcodes[addr+5], bcodes[addr+6], bcodes[addr+7]);
                DF_s4 offset;
                //default offset
                offset = CF_makeIntVal(bcodes[addr], bcodes[addr+1], bcodes[addr+2], bcodes[addr+3]);
                DF_u4 targetBpc = bpc + offset;
                if(targetBpc < bLength && table[targetBpc] == NULL) {
                    table[targetBpc] = (DF_IR_BB*)allocateMem(df->mm, sizeof(DF_IR_BB));
                    table[targetBpc]->bpc = targetBpc;
                    table[targetBpc]->stacks = (DF_u4*)allocateMem(df->mm, sizeof(DF_u4) * codeAttr->maxStack);
                }
                // case statement
                DF_u4 start = addr + 12;
                DF_u4 i;
                for(i = 0 ; i < npairs ; i++) {
                    offset = CF_makeIntVal(bcodes[start + 8 * i], bcodes[start + 8 * i + 1], bcodes[start + 8 * i + 2], bcodes[start + 8 * i + 3]);
                    targetBpc = bpc + offset;
                    if(targetBpc < bLength && table[targetBpc] == NULL) {
                        table[targetBpc] = (DF_IR_BB*)allocateMem(df->mm, sizeof(DF_IR_BB));
                        table[targetBpc]->bpc = targetBpc;
                        table[targetBpc]->stacks = (DF_u4*)allocateMem(df->mm, sizeof(DF_u4) * codeAttr->maxStack);
                    }
                }
                break;
            }
            case CF_TABLESWITCH:
            {
                DF_u4 addr = (bpc+4) & 0xFFFFFFFC;
                DF_u4 pads = addr - bpc - 1;
                DF_u4 npairs = CF_makeIntVal(bcodes[addr+8], bcodes[addr+9], bcodes[addr+10], bcodes[addr+11]) - CF_makeIntVal(bcodes[addr+4], bcodes[addr+5], bcodes[addr+6], bcodes[addr+7]) + 1;
                DF_s4 offset;
                //default offset
                offset = CF_makeIntVal(bcodes[addr], bcodes[addr+1], bcodes[addr+2], bcodes[addr+3]);
                DF_u4 targetBpc = bpc + offset;
                if(targetBpc < bLength && table[targetBpc] == NULL) {
                    table[targetBpc] = (DF_IR_BB*)allocateMem(df->mm, sizeof(DF_IR_BB));
                    table[targetBpc]->bpc = targetBpc;
                    table[targetBpc]->stacks = (DF_u4*)allocateMem(df->mm, sizeof(DF_u4) * codeAttr->maxStack);
                }
                // case statement
                DF_u4 start = addr + 12;
                DF_u4 i;
                for(i = 0 ; i < npairs ; i++) {
                    offset = CF_makeIntVal(bcodes[start + 4 * i], bcodes[start + 4 * i + 1], bcodes[start + 4 * i + 2], bcodes[start + 4 * i + 3]);
                    targetBpc = bpc + offset;
                    if(targetBpc < bLength && table[targetBpc] == NULL) {
                        table[targetBpc] = (DF_IR_BB*)allocateMem(df->mm, sizeof(DF_IR_BB));
                        table[targetBpc]->bpc = targetBpc;
                        table[targetBpc]->stacks = (DF_u4*)allocateMem(df->mm, sizeof(DF_u4) * codeAttr->maxStack);
                    }
                }
                break;
            }
            case CF_GOTO:
            {
                DF_s2 offset = CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2]);
                DF_u4 targetBpc = bpc + offset;
                if(targetBpc < bLength && table[targetBpc] == NULL) {
                    table[targetBpc] = (DF_IR_BB*)allocateMem(df->mm, sizeof(DF_IR_BB));
                    table[targetBpc]->bpc = targetBpc;
                    table[targetBpc]->stacks = (DF_u4*)allocateMem(df->mm, sizeof(DF_u4) * codeAttr->maxStack);
                }
                break;
            }
            case CF_GOTO_W:
            {
                DF_s4 offset = CF_makeIntVal(bcodes[bpc+1], bcodes[bpc+2], bcodes[bpc+3], bcodes[bpc+4]);
                DF_u4 targetBpc = bpc + offset;
                if(targetBpc < bLength && table[targetBpc] == NULL) {
                    table[targetBpc] = (DF_IR_BB*)allocateMem(df->mm, sizeof(DF_IR_BB));
                    table[targetBpc]->bpc = targetBpc;
                    table[targetBpc]->stacks = (DF_u4*)allocateMem(df->mm, sizeof(DF_u4) * codeAttr->maxStack);
                }
                break;

            }
            case CF_IRETURN:
            case CF_LRETURN:
            case CF_FRETURN:
            case CF_DRETURN:
            case CF_ARETURN:
            case CF_RETURN:
            case CF_ATHROW:
            {
                break;
            }
            case CF_JSR:
            case CF_RET:
            case CF_JSR_W:
                assert(0);
                break;
        }
        bpc = nextBpc;
    }
    return DFL_TRUE;
}

#define DFL_TYPE_WIDE       0x10000
#define DFL_TYPE_SINGLE     0x20000
#define DFL_TYPE_OBJ        0x40000
#define DFL_TYPE_BYTE       0x100000
#define DFL_TYPE_BOOLEAN    0x200000
DFL_bool DFL_constructCFG(ClassFile* cf, DexFormat* df, CF_ATTRIBUTE_Code* codeAttr, DF_DATA_Code* codeItem) {
    DF_u4 i, j;
    DF_u4 bpc, dpc;
    CF_u4 bLength = codeAttr->codeLength;
    CF_u2 maxStack = codeAttr->maxStack, maxLocals = codeAttr->maxLocals;
    DF_u1* bcodes = codeAttr->code;
    DF_IR_BB** bTable = (DF_IR_BB**)allocateMem(df->mm, sizeof(DF_IR_BB*) * bLength);
    codeItem->cfg = bTable;
    if(verbose) fprintf(stderr, "Construct CFG\n");
    if(bLength == 0) return DFL_TRUE;
    memset(bTable, 0, sizeof(DF_IR_BB*) * bLength);
    // prologue
    {
        DF_IR_BB* prologue = (DF_IR_BB*)allocateMem(df->mm, sizeof(DF_IR_BB));
        prologue->bpc = 0;
        prologue->tos = 0;
        prologue->stacks = (DF_u4*)allocateMem(df->mm, sizeof(DF_u4) * maxStack);
        bTable[0] = prologue;
    }
    // exception try & catch
    {
        CF_ATTRIBUTE_ExceptionTable* exceptionTable = codeAttr->exceptionTable;
        for(i = 0 ; i < codeAttr->exceptionTableLength ; i++) {
            CF_u2 startPc = exceptionTable[i].startPc;
            CF_u2 endPc = exceptionTable[i].endPc;
            CF_u2 handlerPc = exceptionTable[i].handlerPc;
            
            if(bTable[startPc] == NULL) {
                DF_IR_BB* start = (DF_IR_BB*)allocateMem(df->mm, sizeof(DF_IR_BB));
                start->bpc = startPc;
                start->stacks = (DF_u4*)allocateMem(df->mm, sizeof(DF_u4) * maxStack);
                start->isTry = 1;
                bTable[startPc] = start;

            }
            if(bTable[endPc] == NULL) {
                DF_IR_BB* end = (DF_IR_BB*)allocateMem(df->mm, sizeof(DF_IR_BB));
                end->bpc = endPc;
                end->stacks = (DF_u4*)allocateMem(df->mm, sizeof(DF_u4) * maxStack);
                end->isTry = 1;
                bTable[endPc] = end;
            }
            
            if(bTable[handlerPc] == NULL) {
                DF_IR_BB* catch = (DF_IR_BB*)allocateMem(df->mm, sizeof(DF_IR_BB));
                catch->bpc = handlerPc;
                catch->isCatch = 1;
                catch->tos = 1;
                catch->stacks = (DF_u4*)allocateMem(df->mm, sizeof(DF_u4) * maxStack);
                catch->stacks[0] = DFL_TYPE_OBJ;
                bTable[handlerPc] = catch;
            }
        }
    }
    // construct control flow graph
    {
        DFL_constructBB(cf, df, codeAttr, bTable);
    }
    // initialize
    {
        DF_IR_BB* prevBB = bTable[0];
        prevBB->processed = 0;
        for(i = 1 ; i < bLength ; i++) {
            DF_IR_BB* bb = bTable[i];
            if(bb != NULL) {
                bb->processed = 0;
            }
        }
        //prevBB->bLength = bLength - prevBB->bpc;
    }
    return DFL_TRUE;
}
DFL_bool DFL_emulateJavaStackBB(ClassFile* cf, DexFormat* df, CF_ATTRIBUTE_Code* codeAttr, DF_u4 bpc, DF_u4* stacks, DF_IR_BB** table, DF_u1* hasWide, DF_u1* needTemp) {
    DF_IR_BB* bb = table[bpc];
    DF_u4 i, bLength = codeAttr->codeLength;
    DF_u4 tos;
    DF_u1* bcodes = codeAttr->code;
    assert(bb != NULL);
    
    if(bb->processed) return DFL_TRUE;
    if(verbose) fprintf(stderr, "Emulate Java Stack BB %d\n", bpc);
    tos = bb->tos;
    bb->processed = 1;
    memcpy(stacks, bb->stacks, sizeof(DF_u4) * codeAttr->maxStack);
    do {
        DF_u4 nextBpc = bpc + CF_getBytecodeLength(codeAttr->code, bpc);
        if(verbose) fprintf(stderr, "Emulate Stack BPC: %d %d %x => %d\n", bpc, tos, bcodes[bpc], nextBpc);
        switch(bcodes[bpc]) {
            case CF_L2D:
            case CF_D2L:
                (*hasWide) = 1;
                break;
            case CF_LALOAD: 
            case CF_DALOAD: 
                stacks[tos-2] = (tos - 2) | DFL_TYPE_WIDE;
                stacks[tos-1] = (tos - 1) | DFL_TYPE_WIDE;
                (*hasWide) = 1;
                break;
            case CF_SWAP: 
            {
                DF_u4 temp = stacks[tos - 2];
                stacks[tos-2] = stacks[tos-1] - 1;
                stacks[tos-1] = temp + 1;
                break;
            }
            case CF_NOP: 
            case CF_INEG:
            case CF_LNEG:
            case CF_FNEG:
            case CF_DNEG:
            case CF_IINC:
            case CF_I2F:
            case CF_F2I:
            case CF_I2B:
            case CF_I2C:
            case CF_I2S:
            case CF_CHECKCAST:
                break;
            case CF_ARRAYLENGTH:
            {
                stacks[tos-1] = (tos-1) | DFL_TYPE_SINGLE;
                break;
            }
            case CF_NEWARRAY:
            case CF_ANEWARRAY:
            {
                stacks[tos-1] = (tos-1) | DFL_TYPE_OBJ;
                break;
            }
            case CF_INSTANCEOF:
            {
                stacks[tos-1] = (tos-1) | DFL_TYPE_SINGLE;
                break;
            }
            case CF_DUP_X1: 
            {
                DF_u4 temp = stacks[tos-2];
                stacks[tos] = stacks[tos-1] + 1 ;
                stacks[tos-2] = stacks[tos-1] + 1;
                stacks[tos-1] = temp + 1;
                (*needTemp) = 1;
                tos++;
                break;
            }
            case CF_DUP_X2: 
            {
                DF_u4 temp1 = stacks[tos-3];
                DF_u4 temp2 = stacks[tos-2];
                stacks[tos-3] = stacks[tos-1] + 2;
                stacks[tos] = stacks[tos-1] + 1;
                stacks[tos-2] = temp1 + 1;
                stacks[tos-1] = temp2 + 1;
                (*needTemp) = 1;
                tos++;
                break;
            }
            case CF_ACONST_NULL: 
            case CF_ICONST_M1: 
            case CF_ICONST_0: 
            case CF_ICONST_1: 
            case CF_ICONST_2: 
            case CF_ICONST_3: 
            case CF_ICONST_4: 
            case CF_ICONST_5: 
            case CF_FCONST_0: 
            case CF_FCONST_1: 
            case CF_FCONST_2: 
            case CF_BIPUSH: 
            case CF_SIPUSH: 
            case CF_ILOAD: 
            case CF_FLOAD: 
            case CF_ILOAD_0: 
            case CF_ILOAD_1: 
            case CF_ILOAD_2: 
            case CF_ILOAD_3: 
            case CF_FLOAD_0: 
            case CF_FLOAD_1: 
            case CF_FLOAD_2: 
            case CF_FLOAD_3: 
            {
                stacks[tos] = tos | DFL_TYPE_SINGLE;
                tos++;
                break;
            }
            case CF_ALOAD: 
            case CF_ALOAD_0: 
            case CF_ALOAD_1: 
            case CF_ALOAD_2: 
            case CF_ALOAD_3: 
            case CF_NEW:
            {
                stacks[tos] = tos | DFL_TYPE_OBJ;
                tos++;
                break;
            }
            case CF_DUP: 
            {
                stacks[tos] = stacks[tos-1] + 1;
                tos++;
                break;
            }
            case CF_I2L:
            case CF_I2D:
            case CF_F2L:
            case CF_F2D:
            {
                stacks[tos-1] = (tos-1) | DFL_TYPE_WIDE;
                stacks[tos] = (tos) | DFL_TYPE_WIDE;
                tos++;
                (*hasWide) = 1;
                break;
            }
            case CF_LDC: 
            case CF_LDC_W: 
            {
                CF_u2 idx;
                if(bcodes[bpc] == CF_LDC_W) {
                    idx = CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2]);
                }else {
                    idx = bcodes[bpc+1];
                }
                if(cf->cp[idx].tag == CF_CONSTANT_StringType) {
                    stacks[tos] = tos | DFL_TYPE_OBJ;
                }else {
                    stacks[tos] = tos | DFL_TYPE_SINGLE;
                }
                tos++;
                break;
            }
            case CF_DUP2_X1: 
            {
                DF_u4 temp = stacks[tos-2];
                stacks[tos-3] = stacks[tos-2] - 1;
                //stacks[tos-2] = stacks[tos-2];
                stacks[tos] = stacks[tos-2] + 2;
                stacks[tos+1] = stacks[tos-2] + 3;
                stacks[tos-1] = temp + 1;
                (*needTemp) = 1;
                (*hasWide) = 1;
                tos += 2;
                break;
            }
            case CF_DUP2_X2:
            {
                DF_u4 temp1 = stacks[tos - 4];
                DF_u4 temp2 = stacks[tos - 3];
                stacks[tos-4] = stacks[tos-2] - 2;
                stacks[tos-3] = stacks[tos-2] - 1;
                stacks[tos] = stacks[tos-2] + 2;
                stacks[tos+1] = stacks[tos-2] + 3;
                stacks[tos-2] = temp1 + 2;
                stacks[tos-1] = temp2 + 2;
                (*needTemp) = 1;
                (*hasWide) = 1;
                tos += 2;
                break;
            }
            case CF_LCONST_0: 
            case CF_LCONST_1: 
            case CF_DCONST_0: 
            case CF_DCONST_1: 
            case CF_LDC2_W: 
            case CF_DLOAD: 
            case CF_LLOAD: 
            case CF_LLOAD_0: 
            case CF_LLOAD_1: 
            case CF_LLOAD_2: 
            case CF_LLOAD_3: 
            case CF_DLOAD_0: 
            case CF_DLOAD_1: 
            case CF_DLOAD_2: 
            case CF_DLOAD_3: 
            case CF_DUP2: 
                stacks[tos] = tos | DFL_TYPE_WIDE;
                stacks[tos+1] = (tos+1) | DFL_TYPE_WIDE;
                (*hasWide) = 1;
                tos += 2;
                break;
            case CF_L2I:
            case CF_L2F:
            case CF_D2I:
            case CF_D2F:
            {
                stacks[tos-2] = (tos - 2) | DFL_TYPE_SINGLE;
                tos--;
                (*hasWide) = 1;
                break;
            }
            case CF_LUSHR:
            case CF_LSHL:
            case CF_LSHR:
            {
                (*hasWide) = 1;
                tos--;
                break;
            }
            case CF_BALOAD: 
                (*needTemp) = 1;
            case CF_IALOAD: 
            case CF_FALOAD: 
            case CF_CALOAD: 
            case CF_SALOAD: 
            {
                stacks[tos-2] = (tos-2) | DFL_TYPE_SINGLE;
                tos--;
                break;
            }
            case CF_AALOAD:
            case CF_ISTORE: 
            case CF_FSTORE: 
            case CF_ASTORE: 
            case CF_ISTORE_0: 
            case CF_ISTORE_1: 
            case CF_ISTORE_2: 
            case CF_ISTORE_3: 
            case CF_FSTORE_0: 
            case CF_FSTORE_1: 
            case CF_FSTORE_2: 
            case CF_FSTORE_3: 
            case CF_ASTORE_0: 
            case CF_ASTORE_1: 
            case CF_ASTORE_2: 
            case CF_ASTORE_3: 
            case CF_POP: 
            case CF_IADD: 
            case CF_FADD:
            case CF_ISUB:
            case CF_FSUB:
            case CF_IMUL:
            case CF_FMUL:
            case CF_IDIV:
            case CF_FDIV:
            case CF_IREM:
            case CF_FREM:
            case CF_ISHL:
            case CF_ISHR:
            case CF_IUSHR:
            case CF_IAND:
            case CF_IOR:
            case CF_IXOR:
            case CF_FCMPL:
            case CF_FCMPG:
            case CF_MONITORENTER:
            case CF_MONITOREXIT:
                tos--;
                break;
            case CF_LSTORE: 
            case CF_DSTORE: 
            case CF_LSTORE_0: 
            case CF_LSTORE_1: 
            case CF_LSTORE_2: 
            case CF_LSTORE_3: 
            case CF_DSTORE_0: 
            case CF_DSTORE_1: 
            case CF_DSTORE_2: 
            case CF_DSTORE_3: 
            case CF_POP2: 
            case CF_LADD:
            case CF_DADD:
            case CF_LSUB:
            case CF_DSUB:
            case CF_LMUL:
            case CF_DMUL:
            case CF_LDIV:
            case CF_DDIV:
            case CF_LREM:
            case CF_DREM:
            case CF_LAND:
            case CF_LOR:
            case CF_LXOR:
                (*hasWide) = 1;
                tos -= 2;
                break;
            case CF_DCMPL:
            case CF_DCMPG:
            case CF_LCMP:
            {
                stacks[tos - 4] = (tos - 4) | DFL_TYPE_SINGLE;
                (*hasWide) = 1;
                tos-=3;
                break;
            }
            case CF_BASTORE: 
                (*needTemp) = 1;
            case CF_IASTORE: 
            case CF_FASTORE: 
            case CF_AASTORE: 
            case CF_CASTORE: 
            case CF_SASTORE: 
                tos -= 3;
                break;
            case CF_LASTORE: 
            case CF_DASTORE: 
                (*hasWide) = 1;
                tos -= 4;
                break;
            case CF_GETFIELD:
                tos--;
            case CF_GETSTATIC:
                {
                    DF_u2 idx = CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2]);
                    CF_ConstantPool* cp = cf->cp;
                    DF_u2 nameAndTypeIdx = cp[idx].type.fieldInfo.nameAndTypeIndex;
                    DF_u2 typeIdx = cp[nameAndTypeIdx].type.nameAndTypeInfo.descriptorIndex;
                    CF_CONSTANT_Utf8Info utf8 = cp[typeIdx].type.utf8Info;
                    DF_u1 type = utf8.bytes[0];
                    if(type == 'J'|| type == 'D') {
                        (*hasWide) = 1;
                        stacks[tos] = (tos) | DFL_TYPE_WIDE;
                        stacks[tos+1] = (tos+1) | DFL_TYPE_WIDE;
                        tos += 2;
                    }else if(type == '[' || type == 'L') {
                        stacks[tos] = (tos) | DFL_TYPE_OBJ;
                        tos ++;
                    }else {
                        stacks[tos] = (tos) | DFL_TYPE_SINGLE;
                        tos ++;
                    }
                    break;
                }
            case CF_PUTFIELD:
                tos--;
            case CF_PUTSTATIC:
                {
                    DF_u2 idx = CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2]);
                    CF_ConstantPool* cp = cf->cp;
                    DF_u2 nameAndTypeIdx = cp[idx].type.fieldInfo.nameAndTypeIndex;
                    DF_u2 typeIdx = cp[nameAndTypeIdx].type.nameAndTypeInfo.descriptorIndex;
                    CF_CONSTANT_Utf8Info utf8 = cp[typeIdx].type.utf8Info;
                    DF_u1 type = utf8.bytes[0];
                    if(type == 'J'|| type == 'D') {
                        (*hasWide) = 1;
                        tos -= 2;   
                    }else {
                        tos --;
                    }
                    break;
                }
                break;
            
            case CF_INVOKEINTERFACE:
            case CF_INVOKEVIRTUAL:
            case CF_INVOKESPECIAL:
                tos--;
            case CF_INVOKESTATIC:
            {
                    DF_u2 idx = CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2]);
                    CF_ConstantPool* cp = cf->cp;
                    DF_u2 nameAndTypeIdx = cp[idx].type.methodInfo.nameAndTypeIndex;
                    DF_u2 typeIdx = cp[nameAndTypeIdx].type.nameAndTypeInfo.descriptorIndex;
                    CF_CONSTANT_Utf8Info descriptorUtf8 = cp[typeIdx].type.utf8Info;
                    //DF_u4 shortyIdx = df->protoIds[DFL_parseProtoData(df, descriptorUtf8)].shortyIdx;
                    DF_u2 protoIdx = DFL_parseProtoData(df, descriptorUtf8);
                    DF_u4 shortyIdx = df->protoIds[protoIdx].shortyIdx;
                    DF_DATA_String shortyStr = df->stringItems[shortyIdx];

                    DF_u2 parameterSize = shortyStr.size;
                    DF_u4 arg;
                    for(arg = 1 ; arg < parameterSize ; arg++) {
                        if(shortyStr.data[arg] == 'J'
                            || shortyStr.data[arg] == 'D') {
                            (*hasWide) = 1;
                            tos -= 2;
                        }else {
                            tos --;
                        }
                    }
                    {
                        DF_u1 returnChar = shortyStr.data[0];
                        if(returnChar == 'J'
                                || returnChar == 'D') {
                            (*hasWide) = 1;
                            stacks[tos] = (tos) | DFL_TYPE_WIDE;
                            stacks[tos+1] = (tos+1) | DFL_TYPE_WIDE;
                            tos += 2;
                        }else if(returnChar != 'V') {
                            if(returnChar == 'L' || returnChar == '[') {
                                stacks[tos] = (tos) | DFL_TYPE_OBJ;
                            }else {
                                stacks[tos] = (tos) | DFL_TYPE_SINGLE;
                            }
                            tos +=1;
                        }
                    }
                break;
            }
            case CF_WIDE:
            {
                switch(bcodes[bpc+1]) {
                    case CF_ILOAD:
                    case CF_FLOAD:
                    {
                        stacks[tos] = (tos) | DFL_TYPE_SINGLE;
                        tos++;
                        break;
                    }
                    case CF_ALOAD:
                    {
                        stacks[tos] = (tos) | DFL_TYPE_OBJ;
                        tos++;
                        break;
                    }
                    case CF_DLOAD:
                    case CF_LLOAD:
                    {
                        stacks[tos] = (tos) | DFL_TYPE_WIDE;
                        stacks[tos+1] = (tos+1) | DFL_TYPE_WIDE;
                        (*hasWide) = 1;
                        tos += 2;
                        break;
                    }
                    case CF_ASTORE:
                    case CF_ISTORE:
                    case CF_FSTORE:
                        tos --;
                        break;
                    case CF_DSTORE:
                    case CF_LSTORE:
                        (*hasWide) = 1;
                        tos -= 2;
                        break;
                    case CF_RET:
                        assert(0);
                        break;
                    case CF_IINC:
                        break;
                }
                break;
            }
            case CF_MULTIANEWARRAY:
            {
                DF_u2 n = bcodes[bpc+3];
                tos -= n;
                stacks[tos] = tos | DFL_TYPE_OBJ;
                tos++;
                break;
            }
            case CF_IF_ICMPEQ:
            case CF_IF_ICMPNE:
            case CF_IF_ICMPLT:
            case CF_IF_ICMPGE:
            case CF_IF_ICMPGT:
            case CF_IF_ICMPLE:
            case CF_IF_ACMPEQ:
            case CF_IF_ACMPNE:
                tos--;
            case CF_IFNULL:
            case CF_IFNONNULL:
            case CF_IFEQ:
            case CF_IFNE:
            case CF_IFLT:
            case CF_IFGE:
            case CF_IFGT:
            case CF_IFLE:
            {
                DF_s2 offset = CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2]);
                DF_u4 targetBpc = bpc + offset;
                tos--;
                
                assert(targetBpc < bLength);
                assert(table[targetBpc] != NULL);
                assert(table[nextBpc] != NULL);
                
                table[targetBpc]->tos = tos;
                memcpy(table[targetBpc]->stacks, stacks, sizeof(DF_u4) * codeAttr->maxStack);
                
                table[nextBpc]->tos = tos;
                memcpy(table[nextBpc]->stacks, stacks, sizeof(DF_u4) * codeAttr->maxStack);

                /*
                bb->succNum = 2;
                bb->succs = (DF_IR_BB**)allocateMem(df->mm, sizeof(DF_IR_BB*) * bb->succNum);

                bb->succs[0] = table[nextBpc];
                bb->succs[1] = table[targetBpc];
                */
                DFL_emulateJavaStackBB(cf, df, codeAttr, nextBpc, stacks, table, hasWide, needTemp);
                DFL_emulateJavaStackBB(cf, df, codeAttr, targetBpc, stacks, table, hasWide, needTemp);
                
                return DFL_TRUE;
            }
            case CF_GOTO:
            {
                DF_s2 offset = CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2]);
                DF_u4 targetBpc = bpc + offset;
                assert(targetBpc < bLength);
                table[targetBpc]->tos = tos;
                memcpy(table[targetBpc]->stacks, stacks, sizeof(DF_u4) * codeAttr->maxStack);
                /*
                bb->succNum = 1;
                bb->succs = (DF_IR_BB**)allocateMem(df->mm, sizeof(DF_IR_BB*) * bb->succNum);
                bb->succs[0] = table[targetBpc];
                */
                DFL_emulateJavaStackBB(cf, df, codeAttr, targetBpc, stacks, table, hasWide, needTemp);
                return DFL_TRUE;
            }
            case CF_GOTO_W:
            {
                DF_s4 offset = CF_makeIntVal(bcodes[bpc+1], bcodes[bpc+2], bcodes[bpc+3], bcodes[bpc+4]);
                DF_u4 targetBpc = bpc + offset;
                
                assert(targetBpc < bLength);
                table[targetBpc]->tos = tos;
                memcpy(table[targetBpc]->stacks, stacks, sizeof(DF_u4) * codeAttr->maxStack);
                /*
                bb->succNum = 1;
                bb->succs = (DF_IR_BB**)allocateMem(df->mm, sizeof(DF_IR_BB*) * bb->succNum);
                bb->succs[0] = table[targetBpc];
                */
                DFL_emulateJavaStackBB(cf, df, codeAttr, targetBpc, stacks, table, hasWide, needTemp);
                return DFL_TRUE;
            }

            case CF_LOOKUPSWITCH:
            {
                tos--;
                DF_u4 addr = (bpc+4) & 0xFFFFFFFC;
                DF_u4 pads = addr - bpc - 1;
                DF_u4 npairs = CF_makeIntVal(bcodes[addr+4], bcodes[addr+5], bcodes[addr+6], bcodes[addr+7]);
                DF_s4 offset;
                /*
                bb->succNum = npairs + 1;
                bb->succs = (DF_IR_BB**)allocateMem(df->mm, sizeof(DF_IR_BB*) * bb->succNum);
                */
                //default offset
                offset = CF_makeIntVal(bcodes[addr], bcodes[addr+1], bcodes[addr+2], bcodes[addr+3]);
                DF_u4 targetBpc = bpc + offset;
                assert(targetBpc < bLength);
                table[targetBpc]->tos = tos;
                memcpy(table[targetBpc]->stacks, stacks, sizeof(DF_u4) * codeAttr->maxStack);
                //bb->succs[0] = table[targetBpc];
                DFL_emulateJavaStackBB(cf, df, codeAttr, targetBpc, stacks, table, hasWide, needTemp);
                
                // case statement
                DF_u4 start = addr + 12;
                for(i = 0 ; i < npairs ; i++) {
                    offset = CF_makeIntVal(bcodes[start + 8 * i], bcodes[start + 8 * i + 1], bcodes[start + 8 * i + 2], bcodes[start + 8 * i + 3]);
                    targetBpc = bpc + offset;
                    assert(targetBpc < bLength);
                    table[targetBpc]->tos = tos;
                    memcpy(table[targetBpc]->stacks, stacks, sizeof(DF_u4) * codeAttr->maxStack);
                    
                    //bb->succs[i+1] = table[targetBpc];
                    DFL_emulateJavaStackBB(cf, df, codeAttr, targetBpc, stacks, table, hasWide, needTemp);
                }
                return DFL_TRUE;
            }
            case CF_TABLESWITCH:
            {
                tos--;
                DF_u4 addr = (bpc+4) & 0xFFFFFFFC;
                DF_u4 pads = addr - bpc - 1;
                DF_u4 npairs = CF_makeIntVal(bcodes[addr+8], bcodes[addr+9], bcodes[addr+10], bcodes[addr+11]) - CF_makeIntVal(bcodes[addr+4], bcodes[addr+5], bcodes[addr+6], bcodes[addr+7]) + 1;
                DF_s4 offset;
                
                //bb->succNum = npairs + 1;
                //bb->succs = (DF_IR_BB**)allocateMem(df->mm, sizeof(DF_IR_BB*) * bb->succNum);
                //default offset
                offset = CF_makeIntVal(bcodes[addr], bcodes[addr+1], bcodes[addr+2], bcodes[addr+3]);
                DF_u4 targetBpc = bpc + offset;
                assert(targetBpc < bLength);
                table[targetBpc]->tos = tos;
                memcpy(table[targetBpc]->stacks, stacks, sizeof(DF_u4) * codeAttr->maxStack);
                
                //bb->succs[0] = table[targetBpc];
                DFL_emulateJavaStackBB(cf, df, codeAttr, targetBpc, stacks, table, hasWide, needTemp);
                
                // case statement
                DF_u4 start = addr + 12;
                for(i = 0 ; i < npairs ; i++) {
                    offset = CF_makeIntVal(bcodes[start + 4 * i], bcodes[start + 4 * i + 1], bcodes[start + 4 * i + 2], bcodes[start + 4 * i + 3]);
                    targetBpc = bpc + offset;
                    assert(targetBpc < bLength);
                    table[targetBpc]->tos = tos;
                    memcpy(table[targetBpc]->stacks, stacks, sizeof(DF_u4) * codeAttr->maxStack);
                    
                    //bb->succs[i+1] = table[targetBpc];
                    DFL_emulateJavaStackBB(cf, df, codeAttr, targetBpc, stacks, table, hasWide, needTemp);
                }
                return DFL_TRUE;
            }
            case CF_LRETURN:
            case CF_DRETURN:
                (*hasWide) = DFL_TRUE;
            case CF_IRETURN:
            case CF_FRETURN:
            case CF_ARETURN:
            case CF_RETURN:
            case CF_ATHROW:
                return DFL_TRUE;
            case CF_JSR:
            case CF_RET:
            case CF_JSR_W:
                assert(0);
                break;
        }
        assert(tos <= codeAttr->maxStack);
        if(table[nextBpc] != NULL) {
            table[nextBpc]->tos = tos;
            memcpy(table[nextBpc]->stacks, stacks, sizeof(DF_u4) * codeAttr->maxStack);
        }
        bpc = nextBpc;
    }while(table[bpc] == NULL && bpc < bLength);

    if(bpc < bLength) {
        table[bpc]->tos = tos;
        DFL_emulateJavaStackBB(cf, df, codeAttr, bpc, stacks, table, hasWide, needTemp);
    }
    return DFL_TRUE;
}
DFL_bool DFL_emulateJavaStack(ClassFile* cf, DexFormat* df, CF_ATTRIBUTE_Code* codeAttr, DF_DATA_Code* codeItem) {
    DF_u4 i;
    DF_IR_BB** table = codeItem->cfg;
    DF_u4* stacks = (DF_u4*)allocateMem(df->mm, sizeof(DF_u4) * codeAttr->maxStack);
    if(verbose) fprintf(stderr, "Emulate Java Stack\n");
    DFL_emulateJavaStackBB(cf, df, codeAttr, 0, stacks, table, &(codeItem->hasWide), &(codeItem->needTemp));
    CF_ATTRIBUTE_ExceptionTable* exceptionTable = codeAttr->exceptionTable;
    for(i = 0 ; i < codeAttr->exceptionTableLength ; i++) {
        CF_u2 handlerPc = exceptionTable[i].handlerPc;
        DFL_emulateJavaStackBB(cf, df, codeAttr, handlerPc, stacks, table, &codeItem->hasWide, &codeItem->needTemp);
    }
    freeMem(df->mm, stacks);
    return DFL_TRUE;
}
DF_u4 DFL_translateMoveInst(DF_u2* insns, DF_u4 dpc, DF_u4 from, DF_u2 to) {
    DF_u4 type = from & 0xf0000;
    to = to & 0xffff;
    from = from & 0xffff;
    if(((from | to) >> 4) == 0) {
        if((type & DFL_TYPE_SINGLE) != 0) {
            DF_write2BytesCode(insns, dpc, DF_MOVE, to | (from << 4));dpc++;
        }else if((type & DFL_TYPE_OBJ) != 0){
            DF_write2BytesCode(insns, dpc, DF_MOVE_OBJECT, to | (from << 4)); dpc++;
        }else {
            DF_write2BytesCode(insns, dpc, DF_MOVE_WIDE, to | (from << 4)); dpc++;
        }
    }else if(((to >> 8)) == 0) {
        if((type & DFL_TYPE_SINGLE) != 0) {
            DF_write2BytesCode(insns, dpc, DF_MOVE_FROM16, to); dpc++;
        }else if((type & DFL_TYPE_OBJ) != 0){
            DF_write2BytesCode(insns, dpc, DF_MOVE_OBJECT_FROM16, to); dpc++;
        }else {
            DF_write2BytesCode(insns, dpc, DF_MOVE_WIDE_FROM16, to); dpc++;
        }
        DF_writeShortCode(insns, dpc, from); dpc++;
    }else {
        if((type & DFL_TYPE_SINGLE) != 0) {
            DF_write2BytesCode(insns, dpc, DF_MOVE_16, 0); dpc++;
        }else if((type & DFL_TYPE_OBJ) != 0){
            DF_write2BytesCode(insns, dpc, DF_MOVE_OBJECT_16, 0); dpc++;
        }else {
            DF_write2BytesCode(insns, dpc, DF_MOVE_WIDE_16, 0); dpc++;
        }
        DF_writeShortCode(insns, dpc, to); dpc++;
        DF_writeShortCode(insns, dpc, from); dpc++;
    }
    return dpc;
}
DF_u4 DFL_restoreCopyOpt(DF_u2* insns, DF_u4 dpc, DF_u4* stacks, DF_u2 tos, DF_u2 restore) {
    DF_u2 i;
    for(i = 0 ; i < tos ; i++) {
        DF_u2 from = stacks[i] & 0xffff;
        if(from == restore){
            dpc = DFL_translateMoveInst(insns, dpc, stacks[i], i);
            stacks[i] = i | (stacks[i] & 0xf0000);
        }
    }
    return dpc;

}
DF_u4 DFL_restoreAllCopy(DF_u2* insns, DF_u4 dpc, DF_u4* stacks, DF_u2 tos) {
    DF_u2 i;
    for(i = 0 ; i < tos ; i++) {
        DF_u2 from = stacks[i] & 0xffff;
        if(from != i){
            dpc = DFL_translateMoveInst(insns, dpc, stacks[i], i);
            stacks[i] = i | (stacks[i] & 0xf0000);
        }
    }
    return dpc;
}

DFL_PatchNode* DFL_translateBytecode(ClassFile* cf, DexFormat* df, CF_ATTRIBUTE_Code* codeAttr, DF_DATA_Code* codeItem) {
    DF_u4 bpc = 0, dpc = 0, tos = 0, i;
    DF_u4 maxStack = codeAttr->maxStack, maxLocals = codeAttr->maxLocals, insCount = codeItem->insCount;
    DF_u1* bcodes = codeAttr->code;
    DF_u4 bLength = codeAttr->codeLength;
    DF_u2* insns = (DF_u2*)allocateMem(df->mm, sizeof(DF_u2) * (bLength * 2));
    DF_u4* locals = (DF_u4*)allocateMem(df->mm, sizeof(DF_u4) * maxLocals);
    DF_u4* stacks = NULL;
    DF_u4 temps[2];
    DF_u1 tempSize = 0;
    DF_IR_BB** table = codeItem->cfg;
    DF_u1 hasWide = codeItem->hasWide;
    DF_u1 needTemp = codeItem->needTemp;
    DFL_PatchNode* header = NULL;
    codeItem->insns = insns;
    if(verbose) fprintf(stderr, "Translate Bytecode\n");
    if(needTemp) {
        if(hasWide) {
            temps[0] = maxStack;
            temps[1] = (maxStack + 2);
            tempSize = 4;
        }else {
            temps[0] = maxStack;
            temps[1] = maxStack + 1;
            tempSize = 2;
        }
    }
    codeItem->registersSize = codeAttr->maxStack + codeAttr->maxLocals + tempSize;

    for(i = 0 ; i < insCount ; i++) {
        locals[i] = maxStack + (maxLocals - insCount) + i + tempSize;
    }
    for(i = 0 ; i < maxLocals - insCount ; i++) {
        locals[insCount + i] = maxStack + i + tempSize;
    }
#if 0
    for(i = 0 ; i < maxStack ; i++) {
        stacks[i] = i;
    }
#endif
    while(bpc < bLength) {
        DF_u4 nextBpc = bpc + CF_getBytecodeLength(codeAttr->code, bpc);
        if(verbose) fprintf(stderr, "Translate BPC: %d %d %x => %d\n", bpc, tos, bcodes[bpc], nextBpc);
        if(table[bpc] != NULL) {
            if(verbose) fprintf(stderr, "Basic Block\n");
            tos = table[bpc]->tos;
            table[bpc]->dPc = dpc;
            if(table[bpc]->isTry == 1) {
                dpc = DFL_restoreAllCopy(insns, dpc, stacks, tos);
            }
            stacks = table[bpc]->stacks;
            if(table[bpc]->isCatch == 1) {
                DF_write2BytesCode(insns, dpc, DF_MOVE_EXCEPTION, 0);dpc++;
            }
        }
        switch(bcodes[bpc]) {
            case CF_NOP:
                break;
            case CF_ACONST_NULL:
            {
                if((tos >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_CONST_4, tos);dpc++;
                }else {
                    assert((tos >> 8) == 0);
                    DF_write2BytesCode(insns, dpc, DF_CONST_16, tos); dpc++;
                    DF_writeShortCode(insns, dpc, 0); dpc++;
                }
                stacks[tos] = tos | DFL_TYPE_OBJ;
                tos++;
                break;
            }
            case CF_ICONST_M1:
            case CF_ICONST_0:
            case CF_ICONST_1:
            case CF_ICONST_2:
            case CF_ICONST_3:
            case CF_ICONST_4:
            case CF_ICONST_5:
            {
                DF_s2 val = (bcodes[bpc] - CF_ICONST_0);
                if((tos >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_CONST_4, tos | (val << 4));dpc++;
                }else {
                    assert((tos >> 8) == 0);
                    DF_write2BytesCode(insns, dpc, DF_CONST_16, tos); dpc++;
                    DF_writeShortCode(insns, dpc, val); dpc++;
                }
                stacks[tos] = tos | DFL_TYPE_SINGLE;
                tos++;
                break;
            }   
            case CF_LCONST_0:
            case CF_LCONST_1:
            {
                DF_u2 val = bcodes[bpc] - CF_LCONST_0;
                assert((tos >> 8) == 0);
                DF_write2BytesCode(insns, dpc, DF_CONST_WIDE_16, tos); dpc++;
                DF_writeShortCode(insns, dpc, val); dpc++;
                stacks[tos] = tos | DFL_TYPE_WIDE;
                stacks[tos+1] = (tos+1) | DFL_TYPE_WIDE;
                tos+=2;
                break;
            }
            case CF_FCONST_0:
            {
                if((tos >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_CONST_4, tos);dpc++;
                }else {
                    assert((tos >> 8) == 0);
                    DF_write2BytesCode(insns, dpc, DF_CONST_16, tos); dpc++;
                    DF_writeShortCode(insns, dpc, 0); dpc++;
                }
                stacks[tos] = tos | DFL_TYPE_SINGLE;
                tos++;
                break;
            }
            case CF_FCONST_1:
            {
                assert((tos >> 8) == 0);
                DF_write2BytesCode(insns, dpc, DF_CONST_HIGH16, tos);dpc++;
                DF_writeShortCode(insns, dpc, 0x3f80); dpc++;
                stacks[tos] = tos | DFL_TYPE_SINGLE;
                tos++;
                break;
            }
            case CF_FCONST_2:
            {
                assert((tos >> 8) == 0);
                DF_write2BytesCode(insns, dpc, DF_CONST_HIGH16, tos);dpc++;
                DF_writeShortCode(insns, dpc, 0x4000); dpc++;
 
                stacks[tos] = tos | DFL_TYPE_SINGLE;
                tos++;
                break;
            }
            case CF_DCONST_0:
            {
                assert((tos >> 8) == 0);
                DF_write2BytesCode(insns, dpc, DF_CONST_WIDE_16, tos); dpc++;
                DF_writeShortCode(insns, dpc, 0); dpc++;
                stacks[tos] = tos | DFL_TYPE_WIDE;
                stacks[tos+1] = (tos+1) | DFL_TYPE_WIDE;
                tos+=2;
                break;
            }
            case CF_DCONST_1:
            {
                assert((tos >> 8) == 0);
                DF_write2BytesCode(insns, dpc, DF_CONST_WIDE_HIGH16, tos); dpc++;
                DF_writeShortCode(insns, dpc, 0x3ff0); dpc++;
                stacks[tos] = tos | DFL_TYPE_WIDE;
                stacks[tos+1] = (tos+1) | DFL_TYPE_WIDE;
                tos+=2;
                break;
            }
            case CF_BIPUSH:
            {
                DF_s1 val = (char)(bcodes[bpc+1]);
                if((tos >> 4) == 0 && (val >> 3) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_CONST_4, tos | (val << 4));dpc++;
                }else {
                    assert((tos >> 8) == 0);
                    DF_write2BytesCode(insns, dpc, DF_CONST_16, tos); dpc++;
                    DF_writeShortCode(insns, dpc, val); dpc++;
                }
                stacks[tos] = tos | DFL_TYPE_SINGLE;
                tos++;
                break;
            }
            case CF_SIPUSH:
            {
                DF_s2 val = CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2]);
                if((tos >> 4) == 0 && (val >> 3) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_CONST_4, tos | (val << 4));dpc++;
                }else {
                    assert((tos >> 8) == 0);
                    DF_write2BytesCode(insns, dpc, DF_CONST_16, tos); dpc++;
                    DF_writeShortCode(insns, dpc, val); dpc++;
                }
                stacks[tos] = tos | DFL_TYPE_SINGLE;
                tos++;
                break;
            }
            case CF_LDC:
            case CF_LDC_W:
            {
                CF_u2 idx;
                if(bcodes[bpc] == CF_LDC_W) {
                    idx = CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2]);
                }else {
                    idx = bcodes[bpc+1];
                }
                CF_ConstantPool cp = cf->cp[idx];
                assert((tos >> 8) == 0);
                switch(cp.tag) {
                    case CF_CONSTANT_StringType:
                    {
                        DF_u4 strIdx = DFL_parseStringData(df, cf->cp[cp.type.stringInfo.stringIndex].type.utf8Info);
                        if(strIdx >> 16 == 0) {
                            DF_write2BytesCode(insns, dpc, DF_CONST_STRING, tos); dpc++;
                            DF_writeShortCode(insns, dpc, strIdx); dpc ++;
                        }else {
                            DF_write2BytesCode(insns, dpc, DF_CONST_STRING_JUMBO, tos); dpc++;
                            DF_writeIntCode(insns, dpc, strIdx); dpc += 2;
                        }
                        stacks[tos] = tos | DFL_TYPE_OBJ;
                        break;
                    }
                    case CF_CONSTANT_IntegerType:
                    {
                        DF_u4 val = cp.type.integerInfo.bytes;
                        if((tos >> 4) == 0 && (val >> 4) == 0) {
                            DF_write2BytesCode(insns, dpc, DF_CONST_4, tos | (val << 4)); dpc++;
                        }else if((val >> 16) == 0) {
                            DF_write2BytesCode(insns, dpc, DF_CONST_16, tos); dpc++;
                            DF_writeShortCode(insns, dpc, val); dpc++;
                        }else {
                            DF_write2BytesCode(insns, dpc, DF_CONST, tos); dpc++;
                            DF_writeIntCode(insns, dpc, val); dpc += 2;
                        }
                        stacks[tos] = tos | DFL_TYPE_SINGLE;
                        break;
                    }
                    case CF_CONSTANT_FloatType:
                    {
                        DF_u4 val = cp.type.floatInfo.bytes;
                        if((tos >> 4) == 0 && (val >> 4) == 0) {
                            DF_write2BytesCode(insns, dpc, DF_CONST_4, tos | (val << 4)); dpc++;
                        }else if((val >> 16) == 0) {
                            DF_write2BytesCode(insns, dpc, DF_CONST_16, tos); dpc++;
                            DF_writeShortCode(insns, dpc, val); dpc++;
                        }else {
                            DF_write2BytesCode(insns, dpc, DF_CONST, tos); dpc++;
                            DF_writeIntCode(insns, dpc, val); dpc += 2;
                        }
                        stacks[tos] = tos | DFL_TYPE_SINGLE;
                        break;
                    }
                }
                tos++;
                break;
            }
            case CF_LDC2_W:
            {
                CF_u2 idx = CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2]);
                CF_ConstantPool cp = cf->cp[idx];
                DF_u8 val;
                assert((tos >> 8) == 0);
                if(cp.tag == CF_CONSTANT_LongType) {
                    val = cp.type.longInfo.bytes;
                }else {
                    val = cp.type.doubleInfo.bytes;
                }
                if((val >> 15) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_CONST_WIDE_16, tos); dpc++;
                    DF_writeShortCode(insns, dpc, val); dpc++;
                }else if((val >> 31) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_CONST_WIDE_32, tos); dpc++;
                    DF_writeIntCode(insns, dpc, val); dpc += 2 ;
                }else {
                    DF_write2BytesCode(insns, dpc, DF_CONST_WIDE, tos); dpc++;
                    DF_writeIntCode(insns, dpc, val & 0xffffffff); dpc += 2;
                    DF_writeIntCode(insns, dpc, val >> 32); dpc += 2;
                }
                stacks[tos] = tos | DFL_TYPE_WIDE;
                stacks[tos+1] = (tos+1) | DFL_TYPE_WIDE;
                tos+=2;
                break;
            }
            case CF_ILOAD:
            case CF_FLOAD:
            {
                DF_u1 val = bcodes[bpc+1];
                stacks[tos] = locals[val] | DFL_TYPE_SINGLE;
                tos++;
                break;
            }
            case CF_LLOAD:
            case CF_DLOAD:
            {
                DF_u1 val = bcodes[bpc+1];
                stacks[tos] = locals[val] | DFL_TYPE_WIDE;
                stacks[tos+1] = locals[val+1] | DFL_TYPE_WIDE;
                tos+=2;
                break;
            }
            case CF_ALOAD:
            {
                DF_u1 val = bcodes[bpc+1];
                stacks[tos] = locals[val] | DFL_TYPE_OBJ;
                tos++;
                break;
            }
            case CF_ILOAD_0:
            case CF_ILOAD_1:
            case CF_ILOAD_2:
            case CF_ILOAD_3:
            {
                DF_u1 val = bcodes[bpc] - CF_ILOAD_0;
                stacks[tos] = locals[val] | DFL_TYPE_SINGLE;
                tos++;
                break;
            }
            case CF_LLOAD_0:
            case CF_LLOAD_1:
            case CF_LLOAD_2:
            case CF_LLOAD_3:
            {
                DF_u1 val = bcodes[bpc] - CF_LLOAD_0;
                stacks[tos] = locals[val] | DFL_TYPE_WIDE;
                stacks[tos+1] = locals[val+1] | DFL_TYPE_WIDE;
                tos+=2;
                break;
            }
            case CF_FLOAD_0:
            case CF_FLOAD_1:
            case CF_FLOAD_2:
            case CF_FLOAD_3:
            {
                DF_u1 val = bcodes[bpc] - CF_FLOAD_0;
                stacks[tos] = locals[val] | DFL_TYPE_SINGLE;
                tos++;
                break;
            }
            case CF_DLOAD_0:
            case CF_DLOAD_1:
            case CF_DLOAD_2:
            case CF_DLOAD_3:
            {
                DF_u1 val = bcodes[bpc] - CF_DLOAD_0;
                stacks[tos] = locals[val] | DFL_TYPE_WIDE;
                stacks[tos+1] = locals[val+1] | DFL_TYPE_WIDE;
                tos+=2;
                break;
            }
            case CF_ALOAD_0:
            case CF_ALOAD_1:
            case CF_ALOAD_2:
            case CF_ALOAD_3:
            {
                DF_u1 val = bcodes[bpc] - CF_ALOAD_0;
                stacks[tos] = locals[val] | DFL_TYPE_OBJ;
                tos++;
                break;
            }
            case CF_IALOAD:
            case CF_FALOAD:
            {
                tos -= 2;
                assert(((stacks[tos] & 0xffff) >> 8) == 0 && ((stacks[tos+1] & 0xffff)>> 8) == 0);
                DF_write2BytesCode(insns, dpc, DF_AGET, tos); dpc++;
                DF_write2BytesCode(insns, dpc, (stacks[tos] & 0xffff), (stacks[tos+1] & 0xffff)); dpc++;
                stacks[tos] = tos | DFL_TYPE_SINGLE;
                tos++;
                break;
            }
            case CF_LALOAD:
            case CF_DALOAD:
            {
                tos -= 2;
                assert(((stacks[tos] & 0xffff) >> 8) == 0 && ((stacks[tos+1] & 0xffff)>> 8) == 0);
                DF_write2BytesCode(insns, dpc, DF_AGET_WIDE, tos); dpc++;
                DF_write2BytesCode(insns, dpc, (stacks[tos] & 0xffff), (stacks[tos+1] & 0xffff)); dpc++;
                stacks[tos] = tos | DFL_TYPE_WIDE;
                stacks[tos+1] = (tos+1) | DFL_TYPE_WIDE;
                tos+=2;
                break;
            }
            case CF_AALOAD:
            {
                tos -= 2;
                assert(((stacks[tos] & 0xffff) >> 8) == 0 && ((stacks[tos+1] & 0xffff)>> 8) == 0);
                DF_write2BytesCode(insns, dpc, DF_AGET_OBJECT, tos); dpc++;
                DF_write2BytesCode(insns, dpc, (stacks[tos] & 0xffff), (stacks[tos+1] & 0xffff)); dpc++;
                stacks[tos] = tos | DFL_TYPE_OBJ;
                tos++;
                break;
            }
            case CF_BALOAD:
            {
                tos -= 2;
                assert(((stacks[tos] & 0xffff) >> 8) == 0 && ((stacks[tos+1] & 0xffff)>> 8) == 0);
                /*
                DF_u2 typeIdx = DFL_parseTypeUtf8(df, 2, (DF_u1*)"[B");
                DF_write2BytesCode(insns, dpc, DF_INSTANCE_OF, (temps[0]) | (stacks[tos] & 0xffff) << 4); dpc++;
                DF_writeShortCode(insns, dpc, typeIdx); dpc++;
                DF_write2BytesCode(insns, dpc, DF_IF_EQZ, temps[0]); dpc++;
                DF_writeShortCode(insns, dpc, 5);dpc++;
                */
                {
                    DF_write2BytesCode(insns, dpc, DF_AGET_BOOLEAN, tos); dpc++;
                    DF_write2BytesCode(insns, dpc, (stacks[tos] & 0xffff), (stacks[tos+1] & 0xffff)); dpc++;
                }
                /*
                DF_write2BytesCode(insns, dpc, DF_GOTO, 3)dpc++;
                {
                    DF_write2BytesCode(insns, dpc, DF_AGET_BYTE, tos); dpc++;
                    DF_write2BytesCode(insns, dpc, (stacks[tos] & 0xffff), (stacks[tos+1] & 0xffff)); dpc++;
                }
                */
                stacks[tos] = tos | DFL_TYPE_SINGLE;
                tos++;
                break;
            }
            case CF_CALOAD:
            {
                tos -= 2;
                assert(((stacks[tos] & 0xffff) >> 8) == 0 && ((stacks[tos+1] & 0xffff)>> 8) == 0);
                DF_write2BytesCode(insns, dpc, DF_AGET_CHAR, tos); dpc++;
                DF_write2BytesCode(insns, dpc, (stacks[tos] & 0xffff), (stacks[tos+1] & 0xffff)); dpc++;
                stacks[tos] = tos | DFL_TYPE_SINGLE;
                tos++;
                break;
            }
            case CF_SALOAD:
            {
                tos -= 2;
                assert(((stacks[tos] & 0xffff) >> 8) == 0 && ((stacks[tos+1] & 0xffff)>> 8) == 0);
                DF_write2BytesCode(insns, dpc, DF_AGET_SHORT, tos); dpc++;
                DF_write2BytesCode(insns, dpc, (stacks[tos] & 0xffff), (stacks[tos+1] & 0xffff)); dpc++;
                stacks[tos] = tos | DFL_TYPE_SINGLE;
                tos++;
                break;
            }
            case CF_ISTORE:
            case CF_FSTORE:
            tos--;
            {
                DF_u2 to = locals[bcodes[bpc+1]] & 0xffff;
                dpc = DFL_restoreCopyOpt(insns, dpc, stacks, tos, to);
                dpc = DFL_translateMoveInst(insns, dpc, stacks[tos],to);
                break;
            }
            case CF_LSTORE:
            case CF_DSTORE:
            tos-=2;
            {
                DF_u2 to = locals[bcodes[bpc+1]] & 0xffff;
                dpc = DFL_restoreCopyOpt(insns, dpc, stacks, tos, to);
                dpc = DFL_translateMoveInst(insns, dpc, stacks[tos],to);
                break;
            }
            case CF_ASTORE:
            tos--;
            {
                DF_u2 to = locals[bcodes[bpc+1]] & 0xffff;
                dpc = DFL_restoreCopyOpt(insns, dpc, stacks, tos, to);
                dpc = DFL_translateMoveInst(insns, dpc, stacks[tos],to);
                break;
            }
            case CF_ISTORE_0:
            case CF_ISTORE_1:
            case CF_ISTORE_2:
            case CF_ISTORE_3:
            tos--;
            {
                DF_u2 to = locals[bcodes[bpc] - CF_ISTORE_0] & 0xffff;
                dpc = DFL_restoreCopyOpt(insns, dpc, stacks, tos, to);
                dpc = DFL_translateMoveInst(insns, dpc, stacks[tos],to);
                break;
            }

            case CF_LSTORE_0:
            case CF_LSTORE_1:
            case CF_LSTORE_2:
            case CF_LSTORE_3:
            tos-=2;
            {
                DF_u2 to = locals[bcodes[bpc] - CF_LSTORE_0] & 0xffff;
                dpc = DFL_restoreCopyOpt(insns, dpc, stacks, tos, to);
                dpc = DFL_translateMoveInst(insns, dpc, stacks[tos],to);
                break;
            }

            case CF_FSTORE_0:
            case CF_FSTORE_1:
            case CF_FSTORE_2:
            case CF_FSTORE_3:
            tos--;
            {
                DF_u2 to = locals[bcodes[bpc] - CF_FSTORE_0] & 0xffff;
                dpc = DFL_restoreCopyOpt(insns, dpc, stacks, tos, to);
                dpc = DFL_translateMoveInst(insns, dpc, stacks[tos],to);
                break;
            }
            case CF_DSTORE_0:
            case CF_DSTORE_1:
            case CF_DSTORE_2:
            case CF_DSTORE_3:
            tos-=2;
            {
                DF_u2 to = locals[bcodes[bpc] - CF_DSTORE_0] & 0xffff;
                dpc = DFL_restoreCopyOpt(insns, dpc, stacks, tos, to);
                dpc = DFL_translateMoveInst(insns, dpc, stacks[tos],to);
                break;
            }
            case CF_ASTORE_0:
            case CF_ASTORE_1:
            case CF_ASTORE_2:
            case CF_ASTORE_3:
            tos--;
            {
                DF_u2 to = locals[bcodes[bpc] - CF_ASTORE_0] & 0xffff;
                dpc = DFL_restoreCopyOpt(insns, dpc, stacks, tos, to);
                dpc = DFL_translateMoveInst(insns, dpc, stacks[tos],to);
                break;
            }
            case CF_IASTORE:
            case CF_FASTORE:
            {
                tos -= 3;
                assert((((stacks[tos] & 0xffff) >> 8)) == 0 && ((stacks[tos+1] & 0xffff) >> 8) == 0 && ((stacks[tos+2] & 0xffff) >> 8) == 0);
                DF_write2BytesCode(insns, dpc, DF_APUT, (stacks[tos+2] & 0xffff)); dpc++;
                DF_write2BytesCode(insns, dpc, (stacks[tos] & 0xffff), (stacks[tos+1] & 0xffff)); dpc++;
                break;
            }
            case CF_LASTORE:
            case CF_DASTORE:
            {
                tos -= 4;
                assert((((stacks[tos] & 0xffff) >> 8)) == 0 && ((stacks[tos+1] & 0xffff) >> 8) == 0 && ((stacks[tos+2] & 0xffff) >> 8) == 0);
                DF_write2BytesCode(insns, dpc, DF_APUT_WIDE, (stacks[tos+2] & 0xffff)); dpc++;
                DF_write2BytesCode(insns, dpc, (stacks[tos] & 0xffff), (stacks[tos+1] & 0xffff)); dpc++;
                break;
            }
            case CF_AASTORE:
            {
                tos -= 3;
                assert((((stacks[tos] & 0xffff) >> 8)) == 0 && ((stacks[tos+1] & 0xffff) >> 8) == 0 && ((stacks[tos+2] & 0xffff) >> 8) == 0);
                DF_write2BytesCode(insns, dpc, DF_APUT_OBJECT, (stacks[tos+2] & 0xffff)); dpc++;
                DF_write2BytesCode(insns, dpc, (stacks[tos] & 0xffff), (stacks[tos+1] & 0xffff)); dpc++;
                break;
            }
            case CF_BASTORE:
            {
                tos -= 3;
                assert((((stacks[tos] & 0xffff) >> 8)) == 0 && ((stacks[tos+1] & 0xffff) >> 8) == 0 && ((stacks[tos+2] & 0xffff) >> 8) == 0);
                /*
                DF_u2 typeIdx = DFL_parseTypeUtf8(df, 2, (DF_u1*)"[B");
                DF_write2BytesCode(insns, dpc, DF_INSTANCE_OF, (temps[0]) | (stacks[tos] & 0xffff) << 4); dpc++;
                DF_writeShortCode(insns, dpc, typeIdx); dpc++;
                DF_write2BytesCode(insns, dpc, DF_IF_EQZ, temps[0]); dpc++;
                DF_writeShortCode(insns, dpc, 5);dpc++;
                */
                {
                    DF_write2BytesCode(insns, dpc, DF_APUT_BOOLEAN, (stacks[tos+2] & 0xffff)); dpc++;
                    DF_write2BytesCode(insns, dpc, (stacks[tos] & 0xffff), (stacks[tos+1] & 0xffff)); dpc++;
                }
                /*
                DF_write2BytesCode(insns, dpc, DF_GOTO, 3)dpc++;
                {
                    DF_write2BytesCode(insns, dpc, DF_APUT_BYTE, (stacks[tos+2] & 0xffff)); dpc++;
                    DF_write2BytesCode(insns, dpc, (stacks[tos] & 0xffff), (stacks[tos+1] & 0xffff)); dpc++;
                }
                */
                break;
            }
            case CF_CASTORE:
            {
                tos -= 3;
                assert((((stacks[tos] & 0xffff) >> 8)) == 0 && ((stacks[tos+1] & 0xffff) >> 8) == 0 && ((stacks[tos+2] & 0xffff) >> 8) == 0);
                DF_write2BytesCode(insns, dpc, DF_APUT_CHAR, (stacks[tos+2] & 0xffff)); dpc++;
                DF_write2BytesCode(insns, dpc, (stacks[tos] & 0xffff), (stacks[tos+1] & 0xffff)); dpc++;
                break;
            }
            case CF_SASTORE:
            {
                tos -= 3;
                assert((((stacks[tos] & 0xffff) >> 8)) == 0 && ((stacks[tos+1] & 0xffff) >> 8) == 0 && ((stacks[tos+2] & 0xffff) >> 8) == 0);
                DF_write2BytesCode(insns, dpc, DF_APUT_SHORT, (stacks[tos+2] & 0xffff)); dpc++;
                DF_write2BytesCode(insns, dpc, (stacks[tos] & 0xffff), (stacks[tos+1] & 0xffff)); dpc++;
                break;
            }
            case CF_POP:
            {
                tos--;
                break;
            }
            case CF_POP2:
            {
                tos-=2;
                break;
            }
            case CF_DUP:
            {
                if((stacks[tos-1] & 0xffff)>= maxStack ) {
                    stacks[tos] = stacks[tos-1];
                } else {
                    dpc = DFL_translateMoveInst(insns, dpc, stacks[tos-1], tos);
                    stacks[tos] = tos | (stacks[tos-1] & 0xf0000);
                }
                tos++;
                break;
            }
            case CF_DUP_X1:
            {
                tos -=2 ;
                DF_u4 w1 = stacks[tos+1];
                DF_u4 w2 = stacks[tos];
                if((w1 & 0xffff) >= maxStack) {
                    stacks[tos] = w1;
                    stacks[tos+2] = w1;
                }else {
                    if((w2 & 0xffff) < maxStack) {
                        dpc = DFL_translateMoveInst(insns, dpc, w2, temps[0]);
                    }
                    dpc = DFL_translateMoveInst(insns, dpc, w1, tos);
                    dpc = DFL_translateMoveInst(insns, dpc, w1, tos + 2);
                    stacks[tos] = tos | (w1 & 0xf0000);
                    stacks[tos+2] = (tos + 2) | (w1 & 0xf0000);
                }
                if((w2 & 0xffff) >= maxStack) {
                    stacks[tos+1] = w2;
                }else {
                    if((w1 & 0xffff) < maxStack) {
                        dpc = DFL_translateMoveInst(insns, dpc, (temps[0] | (w2 & 0xf0000)), tos+1);
                    }else {
                        dpc = DFL_translateMoveInst(insns, dpc, w2, tos+1);
                    }
                    stacks[tos+1] = (tos+1) | (w2 & 0xf0000);
                }
                tos += 3;
                break;
            }
            case CF_DUP_X2:
            {
                tos -=3 ;
                DF_u4 w1 = stacks[tos+2];
                DF_u4 w2 = stacks[tos+1];
                DF_u4 w3 = stacks[tos];
                if((w1 & 0xffff) >= maxStack) {
                    stacks[tos] = w1;
                    stacks[tos+3] = w1;
                }else {
                    if((w3 & 0xffff) < maxStack) {
                        dpc = DFL_translateMoveInst(insns, dpc, w3, temps[1]);
                    }
                    dpc = DFL_translateMoveInst(insns, dpc, w1, tos);
                    dpc = DFL_translateMoveInst(insns, dpc, w1, tos + 3);
                    stacks[tos] = tos | (w1 & 0xf0000);
                    stacks[tos+3] = (tos + 3) | (w1 & 0xf0000);
                }


                if((w3 & 0xffff) >= maxStack) {
                    stacks[tos+1] = w3;
                }else {
                    if((w2 & 0xffff) < maxStack) {
                        dpc = DFL_translateMoveInst(insns, dpc, w2, temps[0]);
                    }
                    if((w1 & 0xffff) < maxStack) {
                        dpc = DFL_translateMoveInst(insns, dpc, (temps[1] | (w3 & 0xf0000)), tos+1);
                    }else {
                        dpc = DFL_translateMoveInst(insns, dpc, w3, tos+1);
                    }
                    stacks[tos+1] = (tos+1) | (w3 & 0xf0000);
                }


                if((w2 & 0xffff) >= maxStack) {
                    stacks[tos+2] = w2;
                }else {
                    if((w3 & 0xffff) < maxStack) {
                        dpc = DFL_translateMoveInst(insns, dpc, (temps[0] | (w2 & 0xf0000)), tos+2);
                    }else {
                        dpc = DFL_translateMoveInst(insns, dpc, w2, tos+2);
                    }
                    stacks[tos+2] = (tos+2) | (w2 & 0xf0000);
                }
                tos += 4;
                break;
            }
            case CF_DUP2:
            {
                if((stacks[tos-1] & 0xffff)>= maxStack ) {
                    stacks[tos] = stacks[tos-1];
                    stacks[tos+1] = stacks[tos-1];
                } else {
                    dpc = DFL_translateMoveInst(insns, dpc, stacks[tos-1], tos);
                    stacks[tos] = tos | DFL_TYPE_WIDE;
                    stacks[tos+1] = (tos+1) | DFL_TYPE_WIDE;
                }
                tos+=2;
                break;

            }
            case CF_DUP2_X1:
            {
                tos -=3 ;
                DF_u4 w1 = stacks[tos+1]; // WIDE
                DF_u4 w2 = stacks[tos];
                if((w1 & 0xffff) >= maxStack) {
                    stacks[tos] = w1;
                    stacks[tos+1] = w1;
                    stacks[tos+3] = w1;
                    stacks[tos+4] = w1;
                }else {
                    if((w2 & 0xffff) < maxStack) {
                        dpc = DFL_translateMoveInst(insns, dpc, w2, temps[0]);
                    }
                    dpc = DFL_translateMoveInst(insns, dpc, w1, tos);
                    dpc = DFL_translateMoveInst(insns, dpc, w1, tos + 3);
                    stacks[tos] = tos | (w1 & 0xf0000);
                    stacks[tos+1] = tos | (w1 & 0xf0000);

                    stacks[tos+3] = (tos + 3) | (w1 & 0xf0000);
                    stacks[tos+4] = (tos + 3) | (w1 & 0xf0000);
                }
                if((w2 & 0xffff) >= maxStack) {
                    stacks[tos+2] = w2;
                }else {
                    if((w1 & 0xffff) < maxStack) {
                        dpc = DFL_translateMoveInst(insns, dpc, (temps[0] | (w2 & 0xf0000)), tos+2);
                    }else {
                        dpc = DFL_translateMoveInst(insns, dpc, w2, tos + 2);
                    }
                    stacks[tos+2] = (tos+2) | (w2 & 0xf0000);
                }
                tos += 5;
                break;
            }
            case CF_DUP2_X2:
            {
                tos -=4 ;
                DF_u4 w1 = stacks[tos+2];
                DF_u4 w2 = stacks[tos+1];
                DF_u4 w3 = stacks[tos];
                if((w1 & 0xffff) >= maxStack) {
                    stacks[tos] = w1;
                    stacks[tos+1] = w1;
                    stacks[tos+4] = w1;
                    stacks[tos+5] = w1;
                }else {
                    if((w2 & 0xffff) < maxStack) {
                        dpc = DFL_translateMoveInst(insns, dpc, w2, temps[0]);
                    }
                    if((w3 & 0xffff) < maxStack) {
                        dpc = DFL_translateMoveInst(insns, dpc, w3, temps[1]);
                    }
                    dpc = DFL_translateMoveInst(insns, dpc, w1, tos);
                    dpc = DFL_translateMoveInst(insns, dpc, w1, tos + 4);
                    stacks[tos] = tos | DFL_TYPE_WIDE;
                    stacks[tos+1] = (tos|1) | DFL_TYPE_WIDE;
                    stacks[tos+4] = (tos + 4) | DFL_TYPE_WIDE;
                    stacks[tos+5] = (tos + 5) | DFL_TYPE_WIDE;
                }
                if((w3 & 0xffff) >= maxStack) {
                    stacks[tos+2] = w3;
                }else {
                    if((w1 & 0xffff) < maxStack) {
                        dpc = DFL_translateMoveInst(insns, dpc, (temps[1] | (w3 & 0xf0000)), tos+2);
                    }else {
                        dpc = DFL_translateMoveInst(insns, dpc, w3, tos+2);
                    }
                    stacks[tos+2] = (tos+2) | (w3 & 0xf0000);
                }


                if((w2 & 0xffff) >= maxStack) {
                    stacks[tos+3] = w2;
                }else {
                    if((w1 & 0xffff) < maxStack) {
                        dpc = DFL_translateMoveInst(insns, dpc, (temps[0] | (w2 & 0xf0000)), tos+3);
                    }else {
                        dpc = DFL_translateMoveInst(insns, dpc, w2, tos+3);
                    }
                    stacks[tos+3] = (tos+3) | (w2 & 0xf0000);
                }
                tos += 6;
                break;
            }
            case CF_SWAP:
            {
                dpc = DFL_translateMoveInst(insns, dpc, stacks[tos-1], temps[0]);
                dpc = DFL_translateMoveInst(insns, dpc, stacks[tos-2], stacks[tos-1]);
                dpc = DFL_translateMoveInst(insns, dpc, (temps[0] | (stacks[tos-1] & 0xf0000)), stacks[tos-2]);
                break;
            }
            case CF_IADD:
            {
                assert(((stacks[tos-2] & 0xffff) >> 8) == 0 && ((stacks[tos-1] & 0xffff) >> 8) == 0);
                if((stacks[tos-2] & 0xffff) < maxStack  && ((stacks[tos-1] & 0xffff) >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_ADD_INT_2ADDR, (tos-2) | ((stacks[tos-1] & 0xffff) << 4));dpc++;
                }else {
                    DF_write2BytesCode(insns, dpc, DF_ADD_INT, (tos-2));dpc++;
                    DF_write2BytesCode(insns, dpc, stacks[tos-2] & 0xffff, stacks[tos-1] & 0xffff);dpc++;
                }
                stacks[tos-2] = (tos-2) | DFL_TYPE_SINGLE;
                tos--;
                break;
            }
            case CF_LADD:
            {
                assert(((stacks[tos-4] & 0xffff) >> 8) == 0 && ((stacks[tos-2] & 0xffff) >> 8) == 0);
                if((stacks[tos-4] & 0xffff) < maxStack && ((stacks[tos-2] & 0xffff) >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_ADD_LONG_2ADDR, (tos-4) | ((stacks[tos-2] & 0xffff) << 4)); dpc++;         
                }else {
                    DF_write2BytesCode(insns, dpc, DF_ADD_LONG, (tos-4));dpc++;
                    DF_write2BytesCode(insns, dpc, stacks[tos-4] & 0xffff, stacks[tos-2] & 0xffff);dpc++;
                }
                stacks[tos-4] = (tos-4) | DFL_TYPE_WIDE;
                stacks[tos-3] = (tos-3) | DFL_TYPE_WIDE;
                tos -=2;
                break;
            }
            case CF_FADD:
            {
                assert(((stacks[tos-2] & 0xffff) >> 8) == 0 && ((stacks[tos-1] & 0xffff) >> 8) == 0);
                if((stacks[tos-2] & 0xffff) < maxStack  && ((stacks[tos-1] & 0xffff) >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_ADD_FLOAT_2ADDR, (tos-2) | ((stacks[tos-1] & 0xffff) << 4));dpc++;         
                }else {
                    DF_write2BytesCode(insns, dpc, DF_ADD_FLOAT, (tos-2));dpc++;
                    DF_write2BytesCode(insns, dpc, stacks[tos-2] & 0xffff, stacks[tos-1] & 0xffff);dpc++;
                }
                stacks[tos-2] = (tos-2) | DFL_TYPE_SINGLE;
                tos--;
                break;
            }
            case CF_DADD:
            {
                assert(((stacks[tos-4] & 0xffff) >> 8) == 0 && ((stacks[tos-2] & 0xffff) >> 8) == 0);
                if((stacks[tos-4] & 0xffff) < maxStack && ((stacks[tos-2] & 0xffff) >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_ADD_DOUBLE_2ADDR, (tos-4) | ((stacks[tos-2] & 0xffff) << 4));          dpc++;
                }else {
                    DF_write2BytesCode(insns, dpc, DF_ADD_DOUBLE, (tos-4));dpc++;
                    DF_write2BytesCode(insns, dpc, stacks[tos-4] & 0xffff, stacks[tos-2] & 0xffff);dpc++;
                }
                stacks[tos-4] = (tos-4) | DFL_TYPE_WIDE;
                stacks[tos-3] = (tos-3) | DFL_TYPE_WIDE;
                tos -=2;
                break;
            }
            case CF_ISUB:
            {
                assert(((stacks[tos-2] & 0xffff) >> 8) == 0 && ((stacks[tos-1] & 0xffff) >> 8) == 0);
                if((stacks[tos-2] & 0xffff) < maxStack  && ((stacks[tos-1] & 0xffff) >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_SUB_INT_2ADDR, (tos-2) | ((stacks[tos-1] & 0xffff) << 4));          dpc++;
                }else {
                    DF_write2BytesCode(insns, dpc, DF_SUB_INT, (tos-2));dpc++;
                    DF_write2BytesCode(insns, dpc, stacks[tos-2] & 0xffff, stacks[tos-1] & 0xffff);dpc++;
                }
                stacks[tos-2] = (tos-2) | DFL_TYPE_SINGLE;
                tos--;
                break;
            }
            case CF_LSUB:
            {
                assert(((stacks[tos-4] & 0xffff) >> 8) == 0 && ((stacks[tos-2] & 0xffff) >> 8) == 0);
                if((stacks[tos-4] & 0xffff) < maxStack && ((stacks[tos-2] & 0xffff) >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_SUB_LONG_2ADDR, (tos-4) | ((stacks[tos-2] & 0xffff) << 4));          dpc++;
                }else {
                    DF_write2BytesCode(insns, dpc, DF_SUB_LONG, (tos-4));dpc++;
                    DF_write2BytesCode(insns, dpc, stacks[tos-4] & 0xffff, stacks[tos-2] & 0xffff);dpc++;
                }
                stacks[tos-4] = (tos-4) | DFL_TYPE_WIDE;
                stacks[tos-3] = (tos-3) | DFL_TYPE_WIDE;
                tos -=2;
                break;
            }
            case CF_FSUB:
            {
                assert(((stacks[tos-2] & 0xffff) >> 8) == 0 && ((stacks[tos-1] & 0xffff) >> 8) == 0);
                if((stacks[tos-2] & 0xffff) < maxStack  && ((stacks[tos-1] & 0xffff) >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_SUB_FLOAT_2ADDR, (tos-2) | ((stacks[tos-1] & 0xffff) << 4));          dpc++;
                }else {
                    DF_write2BytesCode(insns, dpc, DF_SUB_FLOAT, (tos-2));dpc++;
                    DF_write2BytesCode(insns, dpc, stacks[tos-2] & 0xffff, stacks[tos-1] & 0xffff);dpc++;
                }
                stacks[tos-2] = (tos-2) | DFL_TYPE_SINGLE;
                tos--;
                break;
            }
            case CF_DSUB:
            {
                assert(((stacks[tos-4] & 0xffff) >> 8) == 0 && ((stacks[tos-2] & 0xffff) >> 8) == 0);
                if((stacks[tos-4] & 0xffff) < maxStack && ((stacks[tos-2] & 0xffff) >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_SUB_DOUBLE_2ADDR, (tos-4) | ((stacks[tos-2] & 0xffff) << 4));          dpc++;
                }else {
                    DF_write2BytesCode(insns, dpc, DF_SUB_DOUBLE, (tos-4));dpc++;
                    DF_write2BytesCode(insns, dpc, stacks[tos-4] & 0xffff, stacks[tos-2] & 0xffff);dpc++;
                }
                stacks[tos-4] = (tos-4) | DFL_TYPE_WIDE;
                stacks[tos-3] = (tos-3) | DFL_TYPE_WIDE;
                tos -=2;
                break;
            }
            case CF_IMUL:
            {
                assert(((stacks[tos-2] & 0xffff) >> 8) == 0 && ((stacks[tos-1] & 0xffff) >> 8) == 0);
                if((stacks[tos-2] & 0xffff) < maxStack  && ((stacks[tos-1] & 0xffff) >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_MUL_INT_2ADDR, (tos-2) | ((stacks[tos-1] & 0xffff) << 4));          dpc++;
                }else {
                    DF_write2BytesCode(insns, dpc, DF_MUL_INT, (tos-2));dpc++;
                    DF_write2BytesCode(insns, dpc, stacks[tos-2] & 0xffff, stacks[tos-1] & 0xffff);dpc++;
                }
                stacks[tos-2] = (tos-2) | DFL_TYPE_SINGLE;
                tos--;
                break;
            }
            case CF_LMUL:
            {
                assert(((stacks[tos-4] & 0xffff) >> 8) == 0 && ((stacks[tos-2] & 0xffff) >> 8) == 0);
                if((stacks[tos-4] & 0xffff) < maxStack && ((stacks[tos-2] & 0xffff) >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_MUL_LONG_2ADDR, (tos-4) | ((stacks[tos-2] & 0xffff) << 4));          dpc++;
                }else {
                    DF_write2BytesCode(insns, dpc, DF_MUL_LONG, (tos-4));dpc++;
                    DF_write2BytesCode(insns, dpc, stacks[tos-4] & 0xffff, stacks[tos-2] & 0xffff);dpc++;
                }
                stacks[tos-4] = (tos-4) | DFL_TYPE_WIDE;
                stacks[tos-3] = (tos-3) | DFL_TYPE_WIDE;
                tos -=2;
                break;
            }
            case CF_FMUL:
            {
                assert(((stacks[tos-2] & 0xffff) >> 8) == 0 && ((stacks[tos-1] & 0xffff) >> 8) == 0);
                if((stacks[tos-2] & 0xffff) < maxStack  && ((stacks[tos-1] & 0xffff) >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_MUL_FLOAT_2ADDR, (tos-2) | ((stacks[tos-1] & 0xffff) << 4));          dpc++;
                }else {
                    DF_write2BytesCode(insns, dpc, DF_MUL_FLOAT, (tos-2));dpc++;
                    DF_write2BytesCode(insns, dpc, stacks[tos-2] & 0xffff, stacks[tos-1] & 0xffff);dpc++;
                }
                stacks[tos-2] = (tos-2) | DFL_TYPE_SINGLE;
                tos--;
                break;
            }
            case CF_DMUL:
            {
                assert(((stacks[tos-4] & 0xffff) >> 8) == 0 && ((stacks[tos-2] & 0xffff) >> 8) == 0);
                if((stacks[tos-4] & 0xffff) < maxStack && ((stacks[tos-2] & 0xffff) >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_MUL_DOUBLE_2ADDR, (tos-4) | ((stacks[tos-2] & 0xffff) << 4));          dpc++;
                }else {
                    DF_write2BytesCode(insns, dpc, DF_MUL_DOUBLE, (tos-4));dpc++;
                    DF_write2BytesCode(insns, dpc, stacks[tos-4] & 0xffff, stacks[tos-2] & 0xffff);dpc++;
                }
                stacks[tos-4] = (tos-4) | DFL_TYPE_WIDE;
                stacks[tos-3] = (tos-3) | DFL_TYPE_WIDE;
                tos -=2;
                break;
            }            
            case CF_IDIV:
            {
                assert(((stacks[tos-2] & 0xffff) >> 8) == 0 && ((stacks[tos-1] & 0xffff) >> 8) == 0);
                if((stacks[tos-2] & 0xffff) < maxStack  && ((stacks[tos-1] & 0xffff) >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_DIV_INT_2ADDR, (tos-2) | ((stacks[tos-1] & 0xffff) << 4));          dpc++;
                }else {
                    DF_write2BytesCode(insns, dpc, DF_DIV_INT, (tos-2));dpc++;
                    DF_write2BytesCode(insns, dpc, stacks[tos-2] & 0xffff, stacks[tos-1] & 0xffff);dpc++;
                }
                stacks[tos-2] = (tos-2) | DFL_TYPE_SINGLE;
                tos--;
                break;
            }
            case CF_LDIV:
            {
                assert(((stacks[tos-4] & 0xffff) >> 8) == 0 && ((stacks[tos-2] & 0xffff) >> 8) == 0);
                if((stacks[tos-4] & 0xffff) < maxStack && ((stacks[tos-2] & 0xffff) >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_DIV_LONG_2ADDR, (tos-4) | ((stacks[tos-2] & 0xffff) << 4));          dpc++;
                }else {
                    DF_write2BytesCode(insns, dpc, DF_DIV_LONG, (tos-4));dpc++;
                    DF_write2BytesCode(insns, dpc, stacks[tos-4] & 0xffff, stacks[tos-2] & 0xffff);dpc++;
                }
                stacks[tos-4] = (tos-4) | DFL_TYPE_WIDE;
                stacks[tos-3] = (tos-3) | DFL_TYPE_WIDE;
                tos -=2;
                break;
            }
            case CF_FDIV:
            {
                assert(((stacks[tos-2] & 0xffff) >> 8) == 0 && ((stacks[tos-1] & 0xffff) >> 8) == 0);
                if((stacks[tos-2] & 0xffff) < maxStack  && ((stacks[tos-1] & 0xffff) >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_DIV_FLOAT_2ADDR, (tos-2) | ((stacks[tos-1] & 0xffff) << 4));          dpc++;
                }else {
                    DF_write2BytesCode(insns, dpc, DF_DIV_FLOAT, (tos-2));dpc++;
                    DF_write2BytesCode(insns, dpc, stacks[tos-2] & 0xffff, stacks[tos-1] & 0xffff);dpc++;
                }
                stacks[tos-2] = (tos-2) | DFL_TYPE_SINGLE;
                tos--;
                break;
            }
            case CF_DDIV:
            {
                assert(((stacks[tos-4] & 0xffff) >> 8) == 0 && ((stacks[tos-2] & 0xffff) >> 8) == 0);
                if((stacks[tos-4] & 0xffff) < maxStack && ((stacks[tos-2] & 0xffff) >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_DIV_DOUBLE_2ADDR, (tos-4) | ((stacks[tos-2] & 0xffff) << 4));          dpc++;
                }else {
                    DF_write2BytesCode(insns, dpc, DF_DIV_DOUBLE, (tos-4));dpc++;
                    DF_write2BytesCode(insns, dpc, stacks[tos-4] & 0xffff, stacks[tos-2] & 0xffff);dpc++;
                }
                stacks[tos-4] = (tos-4) | DFL_TYPE_WIDE;
                stacks[tos-3] = (tos-3) | DFL_TYPE_WIDE;
                tos -=2;
                break;
            }                   
            case CF_IREM:
            {
                assert(((stacks[tos-2] & 0xffff) >> 8) == 0 && ((stacks[tos-1] & 0xffff) >> 8) == 0);
                if((stacks[tos-2] & 0xffff) < maxStack  && ((stacks[tos-1] & 0xffff) >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_REM_INT_2ADDR, (tos-2) | ((stacks[tos-1] & 0xffff) << 4));          dpc++;
                }else {
                    DF_write2BytesCode(insns, dpc, DF_REM_INT, (tos-2));dpc++;
                    DF_write2BytesCode(insns, dpc, stacks[tos-2] & 0xffff, stacks[tos-1] & 0xffff);dpc++;
                }
                stacks[tos-2] = (tos-2) | DFL_TYPE_SINGLE;
                tos--;
                break;
            }
            case CF_LREM:
            {
                assert(((stacks[tos-4] & 0xffff) >> 8) == 0 && ((stacks[tos-2] & 0xffff) >> 8) == 0);
                if((stacks[tos-4] & 0xffff) < maxStack && ((stacks[tos-2] & 0xffff) >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_REM_LONG_2ADDR, (tos-4) | ((stacks[tos-2] & 0xffff) << 4));          dpc++;
                }else {
                    DF_write2BytesCode(insns, dpc, DF_REM_LONG, (tos-4));dpc++;
                    DF_write2BytesCode(insns, dpc, stacks[tos-4] & 0xffff, stacks[tos-2] & 0xffff);dpc++;
                }
                stacks[tos-4] = (tos-4) | DFL_TYPE_WIDE;
                stacks[tos-3] = (tos-3) | DFL_TYPE_WIDE;
                tos -=2;
                break;
            }
            case CF_FREM:
            {
                assert(((stacks[tos-2] & 0xffff) >> 8) == 0 && ((stacks[tos-1] & 0xffff) >> 8) == 0);
                if((stacks[tos-2] & 0xffff) < maxStack  && ((stacks[tos-1] & 0xffff) >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_REM_FLOAT_2ADDR, (tos-2) | ((stacks[tos-1] & 0xffff) << 4));          dpc++;
                }else {
                    DF_write2BytesCode(insns, dpc, DF_REM_FLOAT, (tos-2));dpc++;
                    DF_write2BytesCode(insns, dpc, stacks[tos-2] & 0xffff, stacks[tos-1] & 0xffff);dpc++;
                }
                stacks[tos-2] = (tos-2) | DFL_TYPE_SINGLE;
                tos--;
                break;
            }
            case CF_DREM:
            {
                assert(((stacks[tos-4] & 0xffff) >> 8) == 0 && ((stacks[tos-2] & 0xffff) >> 8) == 0);
                if((stacks[tos-4] & 0xffff) < maxStack && ((stacks[tos-2] & 0xffff) >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_REM_DOUBLE_2ADDR, (tos-4) | ((stacks[tos-2] & 0xffff) << 4));          dpc++;
                }else {
                    DF_write2BytesCode(insns, dpc, DF_REM_DOUBLE, (tos-4));dpc++;
                    DF_write2BytesCode(insns, dpc, stacks[tos-4] & 0xffff, stacks[tos-2] & 0xffff);dpc++;
                }
                stacks[tos-4] = (tos-4) | DFL_TYPE_WIDE;
                stacks[tos-3] = (tos-3) | DFL_TYPE_WIDE;
                tos -=2;
                break;
            }                   
            case CF_INEG:
            {
                assert(((stacks[tos-1] & 0xffff)>> 4) == 0);
                DF_write2BytesCode(insns, dpc, DF_NEG_INT, (tos - 1) | (stacks[tos-1] & 0xffff) << 4);dpc++;
                break;
            }
            case CF_LNEG:
            {
                assert(((stacks[tos-1] & 0xffff)>> 4) == 0);
                DF_write2BytesCode(insns, dpc, DF_NEG_LONG, (tos - 2) | (stacks[tos-2] & 0xffff) << 4);dpc++;
                break;
            }
            case CF_FNEG:
            {
                assert(((stacks[tos-1] & 0xffff)>> 4) == 0);
                DF_write2BytesCode(insns, dpc, DF_NEG_FLOAT, (tos - 1) | (stacks[tos-1] & 0xffff) << 4);dpc++;
                break;
            }
            case CF_DNEG:
            {
                assert(((stacks[tos-1] & 0xffff)>> 4) == 0);
                DF_write2BytesCode(insns, dpc, DF_NEG_DOUBLE, (tos - 2) | (stacks[tos-2] & 0xffff) << 4);dpc++;
                break;
            }
            case CF_ISHL:
            {
                assert(((stacks[tos-2] & 0xffff) >> 8) == 0 && ((stacks[tos-1] & 0xffff) >> 8) == 0);
                if((stacks[tos-2] & 0xffff) < maxStack  && ((stacks[tos-1] & 0xffff) >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_SHL_INT_2ADDR, (tos-2) | ((stacks[tos-1] & 0xffff) << 4));          dpc++;
                }else {
                    DF_write2BytesCode(insns, dpc, DF_SHL_INT, (tos-2));dpc++;
                    DF_write2BytesCode(insns, dpc, stacks[tos-2] & 0xffff, stacks[tos-1] & 0xffff);dpc++;
                }
                stacks[tos-2] = (tos-2) | DFL_TYPE_SINGLE;
                tos--;
                break;
            }
            case CF_LSHL:
            {
                assert(((stacks[tos-3] & 0xffff) >> 8) == 0 && ((stacks[tos-1] & 0xffff) >> 8) == 0);
                if((stacks[tos-3] & 0xffff) < maxStack && ((stacks[tos-1] & 0xffff) >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_SHL_LONG_2ADDR, (tos-3) | ((stacks[tos-1] & 0xffff) << 4));          dpc++;
                }else {
                    DF_write2BytesCode(insns, dpc, DF_SHL_LONG, (tos-3));dpc++;
                    DF_write2BytesCode(insns, dpc, stacks[tos-3] & 0xffff, stacks[tos-1] & 0xffff);dpc++;
                }
                stacks[tos-3] = (tos-3) | DFL_TYPE_WIDE;
                stacks[tos-2] = (tos-2) | DFL_TYPE_WIDE;
                tos --;
                break;
            }            
            case CF_ISHR:
            {
                assert(((stacks[tos-2] & 0xffff) >> 8) == 0 && ((stacks[tos-1] & 0xffff) >> 8) == 0);
                if((stacks[tos-2] & 0xffff) < maxStack  && ((stacks[tos-1] & 0xffff) >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_SHR_INT_2ADDR, (tos-2) | ((stacks[tos-1] & 0xffff) << 4));          dpc++;
                }else {
                    DF_write2BytesCode(insns, dpc, DF_SHR_INT, (tos-2));dpc++;
                    DF_write2BytesCode(insns, dpc, stacks[tos-2] & 0xffff, stacks[tos-1] & 0xffff);dpc++;
                }
                stacks[tos-2] = (tos-2) | DFL_TYPE_SINGLE;
                tos--;
                break;
            }
            case CF_LSHR:
            {
                assert(((stacks[tos-3] & 0xffff) >> 8) == 0 && ((stacks[tos-1] & 0xffff) >> 8) == 0);
                if((stacks[tos-3] & 0xffff) < maxStack && ((stacks[tos-1] & 0xffff) >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_SHR_LONG_2ADDR, (tos-3) | ((stacks[tos-1] & 0xffff) << 4));          dpc++;
                }else {
                    DF_write2BytesCode(insns, dpc, DF_SHR_LONG, (tos-3));dpc++;
                    DF_write2BytesCode(insns, dpc, stacks[tos-3] & 0xffff, stacks[tos-1] & 0xffff);dpc++;
                }
                stacks[tos-3] = (tos-3) | DFL_TYPE_WIDE;
                stacks[tos-2] = (tos-2) | DFL_TYPE_WIDE;
                tos --;
                break;
            }                
            case CF_IUSHR:
            {
                assert(((stacks[tos-2] & 0xffff) >> 8) == 0 && ((stacks[tos-1] & 0xffff) >> 8) == 0);
                if((stacks[tos-2] & 0xffff) < maxStack  && ((stacks[tos-1] & 0xffff) >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_USHR_INT_2ADDR, (tos-2) | ((stacks[tos-1] & 0xffff) << 4));          dpc++;
                }else {
                    DF_write2BytesCode(insns, dpc, DF_USHR_INT, (tos-2));dpc++;
                    DF_write2BytesCode(insns, dpc, stacks[tos-2] & 0xffff, stacks[tos-1] & 0xffff);dpc++;
                }
                stacks[tos-2] = (tos-2) | DFL_TYPE_SINGLE;
                tos--;
                break;
            }
            case CF_LUSHR:
            {
                assert(((stacks[tos-3] & 0xffff) >> 8) == 0 && ((stacks[tos-1] & 0xffff) >> 8) == 0);
                if((stacks[tos-3] & 0xffff) < maxStack && ((stacks[tos-1] & 0xffff) >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_USHR_LONG_2ADDR, (tos-3) | ((stacks[tos-1] & 0xffff) << 4));          dpc++;
                }else {
                    DF_write2BytesCode(insns, dpc, DF_USHR_LONG, (tos-3));dpc++;
                    DF_write2BytesCode(insns, dpc, stacks[tos-3] & 0xffff, stacks[tos-1] & 0xffff);dpc++;
                }
                stacks[tos-3] = (tos-3) | DFL_TYPE_WIDE;
                stacks[tos-2] = (tos-2) | DFL_TYPE_WIDE;
                tos --;
                break;
            }            
            case CF_IAND:
            {
                assert(((stacks[tos-2] & 0xffff) >> 8) == 0 && ((stacks[tos-1] & 0xffff) >> 8) == 0);
                if((stacks[tos-2] & 0xffff) < maxStack  && ((stacks[tos-1] & 0xffff) >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_AND_INT_2ADDR, (tos-2) | ((stacks[tos-1] & 0xffff) << 4));          dpc++;
                }else {
                    DF_write2BytesCode(insns, dpc, DF_AND_INT, (tos-2));dpc++;
                    DF_write2BytesCode(insns, dpc, stacks[tos-2] & 0xffff, stacks[tos-1] & 0xffff);dpc++;
                }
                stacks[tos-2] = (tos-2) | DFL_TYPE_SINGLE;
                tos--;
                break;
            }
            case CF_LAND:
            {
                assert(((stacks[tos-4] & 0xffff) >> 8) == 0 && ((stacks[tos-2] & 0xffff) >> 8) == 0);
                if((stacks[tos-4] & 0xffff) < maxStack && ((stacks[tos-2] & 0xffff) >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_AND_LONG_2ADDR, (tos-4) | ((stacks[tos-2] & 0xffff) << 4));          dpc++;
                }else {
                    DF_write2BytesCode(insns, dpc, DF_AND_LONG, (tos-4));dpc++;
                    DF_write2BytesCode(insns, dpc, stacks[tos-4] & 0xffff, stacks[tos-2] & 0xffff);dpc++;
                }
                stacks[tos-4] = (tos-4) | DFL_TYPE_WIDE;
                stacks[tos-3] = (tos-3) | DFL_TYPE_WIDE;
                tos -=2;
                break;
            }      
            case CF_IOR:
            {
                assert(((stacks[tos-2] & 0xffff) >> 8) == 0 && ((stacks[tos-1] & 0xffff) >> 8) == 0);
                if((stacks[tos-2] & 0xffff) < maxStack  && ((stacks[tos-1] & 0xffff) >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_OR_INT_2ADDR, (tos-2) | ((stacks[tos-1] & 0xffff) << 4));          dpc++;
                }else {
                    DF_write2BytesCode(insns, dpc, DF_OR_INT, (tos-2));dpc++;
                    DF_write2BytesCode(insns, dpc, stacks[tos-2] & 0xffff, stacks[tos-1] & 0xffff);dpc++;
                }
                stacks[tos-2] = (tos-2) | DFL_TYPE_SINGLE;
                tos--;
                break;
            }
            case CF_LOR:
            {
                assert(((stacks[tos-4] & 0xffff) >> 8) == 0 && ((stacks[tos-2] & 0xffff) >> 8) == 0);
                if((stacks[tos-4] & 0xffff) < maxStack && ((stacks[tos-2] & 0xffff) >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_OR_LONG_2ADDR, (tos-4) | ((stacks[tos-2] & 0xffff) << 4));          dpc++;
                }else {
                    DF_write2BytesCode(insns, dpc, DF_OR_LONG, (tos-4));dpc++;
                    DF_write2BytesCode(insns, dpc, stacks[tos-4] & 0xffff, stacks[tos-2] & 0xffff);dpc++;
                }
                stacks[tos-4] = (tos-4) | DFL_TYPE_WIDE;
                stacks[tos-3] = (tos-3) | DFL_TYPE_WIDE;
                tos -=2;
                break;
            }            
            case CF_IXOR:
            {
                assert(((stacks[tos-2] & 0xffff) >> 8) == 0 && ((stacks[tos-1] & 0xffff) >> 8) == 0);
                if((stacks[tos-2] & 0xffff) < maxStack  && ((stacks[tos-1] & 0xffff) >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_XOR_INT_2ADDR, (tos-2) | ((stacks[tos-1] & 0xffff) << 4));          dpc++;
                }else {
                    DF_write2BytesCode(insns, dpc, DF_XOR_INT, (tos-2));dpc++;
                    DF_write2BytesCode(insns, dpc, stacks[tos-2] & 0xffff, stacks[tos-1] & 0xffff);dpc++;
                }
                stacks[tos-2] = (tos-2) | DFL_TYPE_SINGLE;
                tos--;
                break;
            }
            case CF_LXOR:
            {
                assert(((stacks[tos-4] & 0xffff) >> 8) == 0 && ((stacks[tos-2] & 0xffff) >> 8) == 0);
                if((stacks[tos-4] & 0xffff) < maxStack && ((stacks[tos-2] & 0xffff) >> 4) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_XOR_LONG_2ADDR, (tos-4) | ((stacks[tos-2] & 0xffff) << 4));          dpc++;
                }else {
                    DF_write2BytesCode(insns, dpc, DF_XOR_LONG, (tos-4));dpc++;
                    DF_write2BytesCode(insns, dpc, stacks[tos-4] & 0xffff, stacks[tos-2] & 0xffff);dpc++;
                }
                stacks[tos-4] = (tos-4) | DFL_TYPE_WIDE;
                stacks[tos-3] = (tos-3) | DFL_TYPE_WIDE;
                tos -=2;
                break;
            }            
            case CF_IINC:
            {
                DF_u1 to = locals[bcodes[bpc+1]] & 0xffff;
                DF_s1 val = bcodes[bpc+2];
                assert((to >> 8) == 0);
                dpc = DFL_restoreCopyOpt(insns, dpc, stacks, tos, to); 
                DF_write2BytesCode(insns, dpc, DF_ADD_INT_LIT8, to)dpc++;
                DF_write2BytesCode(insns, dpc, to, val);dpc++;
                break;
            }
            case CF_I2L:
            {
                if((((stacks[tos-1]) & 0xffff) >> 4) != 0) {
                    dpc = DFL_translateMoveInst(insns, dpc, stacks[tos-1], (tos-1));
                    stacks[tos-1] = (tos-1) | DFL_TYPE_SINGLE;
                }
                assert(((stacks[tos-1] & 0xffff)>> 4) == 0);
                DF_write2BytesCode(insns, dpc, DF_INT_TO_LONG, (tos - 1) | (stacks[tos-1] & 0xffff) << 4);dpc++;
                stacks[tos-1] = (tos - 1) | DFL_TYPE_WIDE;
                stacks[tos] = tos | DFL_TYPE_WIDE;
                tos++;
                break;
            }
            case CF_I2F:
            {
                assert(((stacks[tos-1] & 0xffff)>> 4) == 0);
                DF_write2BytesCode(insns, dpc, DF_INT_TO_FLOAT, (tos - 1) | (stacks[tos-1] & 0xffff) << 4);dpc++;
                break;
            }
            case CF_I2D:
            {
                assert(((stacks[tos-1] & 0xffff)>> 4) == 0);
                DF_write2BytesCode(insns, dpc, DF_INT_TO_DOUBLE, (tos - 1) | (stacks[tos-1] & 0xffff) << 4);dpc++;
                stacks[tos-1] = (tos - 1) | DFL_TYPE_WIDE;
                stacks[tos] = tos | DFL_TYPE_WIDE;
                tos++;
                break;
            }
            case CF_L2I:
            {
                assert(((stacks[tos-2] & 0xffff)>> 4) == 0);
                DF_write2BytesCode(insns, dpc, DF_LONG_TO_INT, (tos - 2) | (stacks[tos-2] & 0xffff) << 4);dpc++;
                stacks[tos-2] = (tos - 2) | DFL_TYPE_SINGLE;
                tos--;
                break;
            }
            case CF_L2F:
            {
                if((((stacks[tos-2]) & 0xffff) >> 4) != 0) {
                    dpc = DFL_translateMoveInst(insns, dpc, stacks[tos-2], (tos-2));
                    stacks[tos-2] = (tos-2) | DFL_TYPE_WIDE;
                    stacks[tos-1] = (tos-1) | DFL_TYPE_WIDE;
                }
                assert(((stacks[tos-2] & 0xffff)>> 4) == 0);
                DF_write2BytesCode(insns, dpc, DF_LONG_TO_FLOAT, (tos - 2) | (stacks[tos-2] & 0xffff) << 4);dpc++;
                stacks[tos-2] = (tos - 2) | DFL_TYPE_SINGLE;
                tos--;
                break;
            }
            case CF_L2D:
            {
                assert(((stacks[tos-2] & 0xffff)>> 4) == 0);
                DF_write2BytesCode(insns, dpc, DF_LONG_TO_DOUBLE, (tos - 2) | (stacks[tos-2] & 0xffff) << 4);dpc++;
                stacks[tos-2] = (tos - 2) | DFL_TYPE_WIDE;
                stacks[tos-1] = (tos - 1) | DFL_TYPE_WIDE;
                break;
            }
            case CF_F2I:
            {
                assert(((stacks[tos-1] & 0xffff)>> 4) == 0);
                DF_write2BytesCode(insns, dpc, DF_FLOAT_TO_INT, (tos - 1) | (stacks[tos-1] & 0xffff) << 4);dpc++;
                break;
            }
            case CF_F2L:
            {
                assert(((stacks[tos-1] & 0xffff)>> 4) == 0);
                DF_write2BytesCode(insns, dpc, DF_FLOAT_TO_LONG, (tos - 1) | (stacks[tos-1] & 0xffff) << 4);dpc++;
                stacks[tos-1] = (tos - 1) | DFL_TYPE_WIDE;
                stacks[tos] = tos | DFL_TYPE_WIDE;
                tos++;
                break;
            }
            case CF_F2D:
            {
                assert(((stacks[tos-1] & 0xffff)>> 4) == 0);
                DF_write2BytesCode(insns, dpc, DF_FLOAT_TO_DOUBLE, (tos - 1) | (stacks[tos-1] & 0xffff) << 4);dpc++;
                stacks[tos-1] = (tos - 1) | DFL_TYPE_WIDE;
                stacks[tos] = tos | DFL_TYPE_WIDE;
                tos++;
                break;
            }
            case CF_D2I:
            {
                assert(((stacks[tos-2] & 0xffff)>> 4) == 0);
                DF_write2BytesCode(insns, dpc, DF_DOUBLE_TO_INT, (tos - 2) | (stacks[tos-2] & 0xffff) << 4);dpc++;
                stacks[tos-2] = (tos - 2) | DFL_TYPE_SINGLE;
                tos--;
                break;
            }
            case CF_D2L:
            {
                assert(((stacks[tos-2] & 0xffff)>> 4) == 0);
                DF_write2BytesCode(insns, dpc, DF_DOUBLE_TO_LONG, (tos - 2) | (stacks[tos-2] & 0xffff) << 4);dpc++;
                stacks[tos-2] = (tos - 2) | DFL_TYPE_WIDE;
                stacks[tos-1] = (tos - 1) | DFL_TYPE_WIDE;
                break;
            }
            case CF_D2F:
            {
                assert(((stacks[tos-2] & 0xffff)>> 4) == 0);
                DF_write2BytesCode(insns, dpc, DF_DOUBLE_TO_FLOAT, (tos - 2) | (stacks[tos-2] & 0xffff) << 4);dpc++;
                stacks[tos-2] = (tos - 2) | DFL_TYPE_SINGLE;
                tos--;
                break;
            }
            case CF_I2B:
            {
                assert(((stacks[tos-1] & 0xffff)>> 4) == 0);
                DF_write2BytesCode(insns, dpc, DF_INT_TO_BYTE, (tos - 1) | (stacks[tos-1] & 0xffff) << 4);dpc++;
                stacks[tos-1] = (tos - 1) | DFL_TYPE_SINGLE;
                break;
            }
            case CF_I2C:
            {
                assert(((stacks[tos-1] & 0xffff)>> 4) == 0);
                DF_write2BytesCode(insns, dpc, DF_INT_TO_CHAR, (tos - 1) | (stacks[tos-1] & 0xffff) << 4);dpc++;
                stacks[tos-1] = (tos - 1) | DFL_TYPE_SINGLE;
                break;
            }
            case CF_I2S:
            {
                assert(((stacks[tos-1] & 0xffff)>> 4) == 0);
                DF_write2BytesCode(insns, dpc, DF_INT_TO_SHORT, (tos - 1) | (stacks[tos-1] & 0xffff) << 4);dpc++;
                stacks[tos-1] = (tos - 1) | DFL_TYPE_SINGLE;
                break;
            }
            case CF_IRETURN:
            case CF_FRETURN:
            {
                assert(((stacks[tos-1] & 0xffff) >> 8) == 0);
                DF_write2BytesCode(insns, dpc, DF_RETURN, (stacks[tos-1] & 0xffff));dpc++;
                tos--;
                break;
            }
            case CF_LRETURN:
            case CF_DRETURN:
            {
                assert(((stacks[tos-2] & 0xffff) >> 8) == 0);
                DF_write2BytesCode(insns, dpc, DF_RETURN_WIDE, (stacks[tos-2] & 0xffff));dpc++;
                tos-=2;
                break;
            }
            case CF_ARETURN:
            {
                assert(((stacks[tos-1] & 0xffff) >> 8) ==0);
                DF_write2BytesCode(insns, dpc, DF_RETURN_OBJECT, (stacks[tos-1] & 0xffff));dpc++;
                tos--;
                break;
            }
            case CF_RETURN:
            {
                DF_write2BytesCode(insns, dpc, DF_RETURN_VOID, 0);dpc++;
                break;
            }
            case CF_ATHROW:{
                assert(((stacks[tos-1] & 0xffff) >> 8) ==0);
                DF_write2BytesCode(insns, dpc, DF_THROW, (stacks[tos-1] & 0xffff));dpc++;
                tos--;
                break;
            }
            case CF_MONITORENTER:
            {
                assert(((stacks[tos-1] & 0xffff) >> 8) ==0);
                DF_write2BytesCode(insns, dpc, DF_MONITOR_ENTER, (stacks[tos-1] & 0xffff));dpc++;
                tos--;
                break;
            }
            case CF_MONITOREXIT:
            {
                assert(((stacks[tos-1] & 0xffff) >> 8) ==0);
                DF_write2BytesCode(insns, dpc, DF_MONITOR_EXIT, (stacks[tos-1] & 0xffff));dpc++;
                tos--;
                break;
            }
            case CF_LCMP:
            {
                assert(((stacks[tos-2] & 0xffff) >> 8) == 0 && ((stacks[tos-4] & 0xffff) >> 8) == 0);
                DF_write2BytesCode(insns, dpc, DF_CMP_LONG, tos-4);dpc++;
                DF_write2BytesCode(insns, dpc, stacks[tos-4] & 0xffff, stacks[tos-2] & 0xffff);dpc++;
                stacks[tos-4] = (tos - 4) | DFL_TYPE_SINGLE;
                tos -= 3;
                break;
            }
            case CF_FCMPL:
            {
                assert(((stacks[tos-1] & 0xffff) >> 8) == 0 && ((stacks[tos-2] & 0xffff) >> 8) == 0);
                DF_write2BytesCode(insns, dpc, DF_CMPL_FLOAT, tos-2);dpc++;
                DF_write2BytesCode(insns, dpc, stacks[tos-2] & 0xffff, stacks[tos-1] & 0xffff);dpc++;
                stacks[tos-2] = (tos - 2) | DFL_TYPE_SINGLE;
                tos -= 1;
                break;
            }
            case CF_FCMPG:
            {
                assert(((stacks[tos-1] & 0xffff) >> 8) == 0 && ((stacks[tos-2] & 0xffff) >> 8) == 0);
                DF_write2BytesCode(insns, dpc, DF_CMPG_FLOAT, tos-2);dpc++;
                DF_write2BytesCode(insns, dpc, stacks[tos-2] & 0xffff, stacks[tos-1] & 0xffff);dpc++;
                stacks[tos-2] = (tos - 2) | DFL_TYPE_SINGLE;
                tos -= 1;
                break;
            }
            case CF_DCMPL:
            {
                assert(((stacks[tos-2] & 0xffff) >> 8) == 0 && ((stacks[tos-4] & 0xffff) >> 8) == 0);
                DF_write2BytesCode(insns, dpc, DF_CMPL_DOUBLE, tos-4);dpc++;
                DF_write2BytesCode(insns, dpc, stacks[tos-4] & 0xffff, stacks[tos-2] & 0xffff);dpc++;
                stacks[tos-4] = (tos - 4) | DFL_TYPE_SINGLE;
                tos -= 3;
                break;
            }
            case CF_DCMPG:
            {
                assert(((stacks[tos-2] & 0xffff) >> 8) == 0 && ((stacks[tos-4] & 0xffff) >> 8) == 0);
                DF_write2BytesCode(insns, dpc, DF_CMPG_DOUBLE, tos-4);dpc++;
                DF_write2BytesCode(insns, dpc, stacks[tos-4] & 0xffff, stacks[tos-2] & 0xffff);dpc++;
                stacks[tos-4] = (tos - 4) | DFL_TYPE_SINGLE;
                tos -= 3;
                break;
            }
            case CF_WIDE:
            {
                switch(bcodes[bpc+1]) {
                    case CF_ALOAD:
                    {
                        DF_u2 val = CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2]);
                        stacks[tos] = locals[val] | DFL_TYPE_OBJ;
                        tos++;
                        break;
                    }
                    case CF_ILOAD:
                    case CF_FLOAD:
                    {
                        DF_u2 val = CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2]);
                        stacks[tos] = locals[val] | DFL_TYPE_SINGLE;
                        tos++;
                        break;
                    }
                    case CF_DLOAD:
                    case CF_LLOAD:
                    {
                        DF_u2 val = CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2]);
                        stacks[tos] = locals[val] | DFL_TYPE_WIDE;
                        stacks[tos+1] = locals[val+1] | DFL_TYPE_WIDE;
                        tos+=2;
                        break;
                    }
                    case CF_ASTORE:
                    tos--;
                    {
                        DF_u2 to = locals[CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2])] & 0xffff;
                        dpc = DFL_restoreCopyOpt(insns, dpc, stacks, tos, to);
                        dpc = DFL_translateMoveInst(insns, dpc, stacks[tos],to);
                        break;
                    }
                    case CF_ISTORE:
                    case CF_FSTORE:
                    tos--;
                    {
                        DF_u2 to = locals[CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2])] & 0xffff;
                        dpc = DFL_restoreCopyOpt(insns, dpc, stacks, tos, to);
                        dpc = DFL_translateMoveInst(insns, dpc, stacks[tos],to);
                        break;
                    }
                    case CF_DSTORE:
                    case CF_LSTORE:
                    tos-=2;
                    {
                        DF_u2 to = locals[CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2])] & 0xffff;
                        dpc = DFL_restoreCopyOpt(insns, dpc, stacks, tos, to);
                        dpc = DFL_translateMoveInst(insns, dpc, stacks[tos],to);
                        break;
                    }
                    case CF_IINC:
                    {
                        DF_u2 to = locals[CF_makeShortVal(bcodes[bpc+1],bcodes[bpc+2])] & 0xffff;
                        DF_s2 val = CF_makeShortVal(bcodes[bpc+3], bcodes[bpc+4]);
                        assert((to >> 4) == 0);
                        dpc = DFL_restoreCopyOpt(insns, dpc, stacks, tos, to); 
                        DF_write2BytesCode(insns, dpc, DF_ADD_INT_LIT16, to | (to<<4))dpc++;
                        DF_writeShortCode(insns, dpc, val);dpc++;
                        break;
                    }
                    case CF_RET:
                        assert(0);
                        break;
                }
            }
            case CF_ARRAYLENGTH:
            {
                assert(((stacks[tos-1] & 0xffff) >> 4) == 0);
                DF_write2BytesCode(insns, dpc, DF_ARRAY_LENGTH, (tos-1) | ((stacks[tos-1] & 0xffff) << 4));dpc++;
                stacks[tos-1] = (tos-1) | DFL_TYPE_SINGLE;
                break;
            }
            case CF_IFEQ:
            case CF_IFNE:
            case CF_IFLT:
            case CF_IFGE:
            case CF_IFGT:
            case CF_IFLE:
            {
                dpc = DFL_restoreAllCopy(insns, dpc, stacks, tos);
                assert(((stacks[tos-1] & 0xffff) >> 8) == 0);
                DF_s2 bpcOffset = CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2]);
                DF_u2 opcode = DF_IF_EQZ + (bcodes[bpc] - CF_IFEQ);
                DF_s2 dpcOffset = 0;
                if(bpcOffset < 0) {
                    dpcOffset = table[(bpc + bpcOffset)]->dPc - dpc;
                }else {
                    DFL_PatchNode* node = (DFL_PatchNode*)allocateMem(df->mm, sizeof(DFL_PatchNode));
                    node->targetBpc = (bpc+bpcOffset);
                    node->dpc = dpc;
                    node->patchDpc = dpc+1;
                    node->offsetSize = 2;
                    node->next = header;
                    header = node;
                }
                DF_write2BytesCode(insns, dpc, opcode, stacks[tos-1] & 0xffff);dpc++;
                DF_writeShortCode(insns, dpc, dpcOffset);dpc++;
                tos--;
                break;
            }
            case CF_IF_ICMPEQ:
            case CF_IF_ICMPNE:
            case CF_IF_ICMPLT:
            case CF_IF_ICMPGE:
            case CF_IF_ICMPGT:
            case CF_IF_ICMPLE:
            {
                dpc = DFL_restoreAllCopy(insns, dpc, stacks, tos);
                
                assert(((stacks[tos-1] & 0xffff) >> 4) == 0 && ((stacks[tos-2] & 0xffff) >> 4) == 0);
                DF_s2 bpcOffset = CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2]);
                DF_u2 opcode = DF_IF_EQ + (bcodes[bpc] - CF_IF_ICMPEQ);
                DF_s2 dpcOffset = 0;
                if(bpcOffset < 0) {
                    dpcOffset = table[(bpc + bpcOffset)]->dPc - dpc;
                }else {
                    DFL_PatchNode* node = (DFL_PatchNode*)allocateMem(df->mm, sizeof(DFL_PatchNode));
                    node->targetBpc = (bpc+bpcOffset);
                    node->dpc = dpc;
                    node->patchDpc = dpc + 1;
                    node->offsetSize = 2;
                    node->next = header;
                    header = node;
                }
                DF_write2BytesCode(insns, dpc, opcode, (stacks[tos-2] & 0xffff) | ((stacks[tos-1] & 0xffff) << 4));dpc++;
                DF_writeShortCode(insns, dpc, dpcOffset);dpc++;
                tos-=2;
                break;
            }
            case CF_IF_ACMPEQ:
            case CF_IF_ACMPNE:
            {
                dpc = DFL_restoreAllCopy(insns, dpc, stacks, tos);
                assert(((stacks[tos-1] & 0xffff) >> 4) == 0 && ((stacks[tos-2] & 0xffff) >> 4) == 0);
                DF_s2 bpcOffset = CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2]);
                DF_u2 opcode = DF_IF_EQ + (bcodes[bpc] - CF_IF_ACMPEQ);
                DF_s2 dpcOffset = 0;
                if(bpcOffset < 0) {
                    dpcOffset = table[(bpc + bpcOffset)]->dPc - dpc;
                }else {
                    DFL_PatchNode* node = (DFL_PatchNode*)allocateMem(df->mm, sizeof(DFL_PatchNode));
                    node->targetBpc = (bpc+bpcOffset);
                    node->dpc = dpc;
                    node->patchDpc = dpc + 1;
                    node->offsetSize = 2;
                    node->next = header;
                    header = node;
                }
                DF_write2BytesCode(insns, dpc, opcode, (stacks[tos-2] & 0xffff) | ((stacks[tos-1] & 0xffff) << 4));dpc++;
                DF_writeShortCode(insns, dpc, dpcOffset);dpc++;
                tos-=2;
                break;
            }
            case CF_GOTO:
            {
                dpc = DFL_restoreAllCopy(insns, dpc, stacks, tos);
                DF_s2 bpcOffset = CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2]);
                DF_s2 dpcOffset = 0;
                if(bpcOffset < 0) {
                    dpcOffset = table[(bpc + bpcOffset)]->dPc - dpc;
                }else {
                    DFL_PatchNode* node = (DFL_PatchNode*)allocateMem(df->mm, sizeof(DFL_PatchNode));
                    node->targetBpc = (bpc+bpcOffset);
                    node->dpc = dpc;
                    node->patchDpc = dpc + 1;
                    node->offsetSize = 2;
                    node->next = header;
                    header = node;
                    dpcOffset = 0xfff;
                }
                if((((DF_u2)dpcOffset) >> 8) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_GOTO, dpcOffset);dpc++;
                }else if((((DF_u2)dpcOffset) >> 16) == 0){
                    DF_write2BytesCode(insns, dpc, DF_GOTO_16, 0);dpc++;
                    DF_writeShortCode(insns, dpc, dpcOffset);dpc++;
                }else {
                    assert(0);
                }
                break;
            }
            case CF_GETSTATIC:
            {
                DF_u2 idx = CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2]);
                CF_ConstantPool* cp = cf->cp;
                DF_u2 classIdx = cp[idx].type.fieldInfo.classIndex;
                DF_u2 nameAndTypeIdx = cp[idx].type.fieldInfo.nameAndTypeIndex;
                CF_CONSTANT_Utf8Info classNameUtf8 = cp[cp[classIdx].type.classInfo.nameIndex].type.utf8Info;
                DF_u2 nameIdx = cp[nameAndTypeIdx].type.nameAndTypeInfo.nameIndex;
                DF_u2 typeIdx = cp[nameAndTypeIdx].type.nameAndTypeInfo.descriptorIndex;
                CF_CONSTANT_Utf8Info nameUtf8 = cp[nameIdx].type.utf8Info;
                CF_CONSTANT_Utf8Info descriptorUtf8 = cp[typeIdx].type.utf8Info;
                DF_uleb128 fieldIdx = DFL_parseFieldData(df, classNameUtf8, nameUtf8, descriptorUtf8);
                DF_u1 type = descriptorUtf8.bytes[0];
                assert(((tos & 0xffff)>> 8) == 0);
                switch(type) {
                    case 'L':
                    case '[':
                    {
                        DF_write2BytesCode(insns, dpc, DF_SGET_OBJECT, tos); dpc++;
                        DF_writeShortCode(insns, dpc, fieldIdx); dpc++;
                        stacks[tos] = (tos) | DFL_TYPE_OBJ;
                        tos ++;
                        break;
                    }
                    case 'D':
                    case 'J':
                    {
                        DF_write2BytesCode(insns, dpc, DF_SGET_WIDE, tos); dpc++;
                        DF_writeShortCode(insns, dpc, fieldIdx); dpc++;
                        stacks[tos] = (tos) | DFL_TYPE_WIDE;
                        stacks[tos+1] = (tos+1) | DFL_TYPE_WIDE;
                        tos += 2;
                        break;
                    }
                    case 'I':
                    {
                        DF_write2BytesCode(insns, dpc, DF_SGET, tos); dpc++;
                        DF_writeShortCode(insns, dpc, fieldIdx); dpc++;
                        stacks[tos] = (tos) | DFL_TYPE_SINGLE;
                        tos ++;
                        break;
                    }
                    case 'Z':
                    {
                        DF_write2BytesCode(insns, dpc, DF_SGET_BOOLEAN, tos); dpc++;
                        DF_writeShortCode(insns, dpc, fieldIdx); dpc++;
                        stacks[tos] = (tos) | DFL_TYPE_SINGLE;
                        tos ++;
                        break;
                    }
                    case 'B':
                    {
                        DF_write2BytesCode(insns, dpc, DF_SGET_BYTE, tos); dpc++;
                        DF_writeShortCode(insns, dpc, fieldIdx); dpc++;
                        stacks[tos] = (tos) | DFL_TYPE_SINGLE;
                        tos ++;
                        break;
                    }
                    case 'C':
                    {
                        DF_write2BytesCode(insns, dpc, DF_SGET_CHAR, tos); dpc++;
                        DF_writeShortCode(insns, dpc, fieldIdx); dpc++;
                        stacks[tos] = (tos) | DFL_TYPE_SINGLE;
                        tos ++;
                        break;
                    }
                    case 'S':
                    {
                        DF_write2BytesCode(insns, dpc, DF_SGET_SHORT, tos); dpc++;
                        DF_writeShortCode(insns, dpc, fieldIdx); dpc++;
                        stacks[tos] = (tos) | DFL_TYPE_SINGLE;
                        tos ++;
                        break;
                    }
                }
                break;
            }
            case CF_PUTSTATIC:
            {
                DF_u2 idx = CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2]);
                CF_ConstantPool* cp = cf->cp;
                DF_u2 classIdx = cp[idx].type.fieldInfo.classIndex;
                DF_u2 nameAndTypeIdx = cp[idx].type.fieldInfo.nameAndTypeIndex;
                CF_CONSTANT_Utf8Info classNameUtf8 = cp[cp[classIdx].type.classInfo.nameIndex].type.utf8Info;
                DF_u2 nameIdx = cp[nameAndTypeIdx].type.nameAndTypeInfo.nameIndex;
                DF_u2 typeIdx = cp[nameAndTypeIdx].type.nameAndTypeInfo.descriptorIndex;
                CF_CONSTANT_Utf8Info nameUtf8 = cp[nameIdx].type.utf8Info;
                CF_CONSTANT_Utf8Info descriptorUtf8 = cp[typeIdx].type.utf8Info;
                DF_uleb128 fieldIdx = DFL_parseFieldData(df, classNameUtf8, nameUtf8, descriptorUtf8);
                DF_u1 type = descriptorUtf8.bytes[0];
                assert(((stacks[tos-1] & 0xffff)>> 8) == 0);
                switch(type) {
                    case 'L':
                    case '[':
                    {
                        DF_write2BytesCode(insns, dpc, DF_SPUT_OBJECT, (stacks[tos-1] & 0xffff)); dpc++;
                        DF_writeShortCode(insns, dpc, fieldIdx); dpc++;
                        tos --;
                        break;
                    }
                    case 'D':
                    case 'J':
                    {
                        DF_write2BytesCode(insns, dpc, DF_SPUT_WIDE, (stacks[tos-2] & 0xffff)); dpc++;
                        DF_writeShortCode(insns, dpc, fieldIdx); dpc++;
                        tos -= 2;
                        break;
                    }
                    case 'I':
                    {
                        DF_write2BytesCode(insns, dpc, DF_SPUT, (stacks[tos-1] & 0xffff)); dpc++;
                        DF_writeShortCode(insns, dpc, fieldIdx); dpc++;
                        tos --;
                        break;
                    }
                    case 'Z':
                    {
                        DF_write2BytesCode(insns, dpc, DF_SPUT_BOOLEAN, (stacks[tos-1] & 0xffff)); dpc++;
                        DF_writeShortCode(insns, dpc, fieldIdx); dpc++;
                        tos --;
                        break;
                    }
                    case 'B':
                    {
                        DF_write2BytesCode(insns, dpc, DF_SPUT_BYTE, (stacks[tos-1] & 0xffff)); dpc++;
                        DF_writeShortCode(insns, dpc, fieldIdx); dpc++;
                        tos --;
                        break;
                    }
                    case 'C':
                    {
                        DF_write2BytesCode(insns, dpc, DF_SPUT_CHAR, (stacks[tos-1] & 0xffff)); dpc++;
                        DF_writeShortCode(insns, dpc, fieldIdx); dpc++;
                        tos--;
                        break;
                    }
                    case 'S':
                    {
                        DF_write2BytesCode(insns, dpc, DF_SPUT_SHORT, (stacks[tos-1] & 0xffff)); dpc++;
                        DF_writeShortCode(insns, dpc, fieldIdx); dpc++;
                        tos --;
                        break;
                    }
                }
                break;
            }
            case CF_GETFIELD:
            {
                DF_u2 idx = CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2]);
                CF_ConstantPool* cp = cf->cp;
                DF_u2 classIdx = cp[idx].type.fieldInfo.classIndex;
                DF_u2 nameAndTypeIdx = cp[idx].type.fieldInfo.nameAndTypeIndex;
                CF_CONSTANT_Utf8Info classNameUtf8 = cp[cp[classIdx].type.classInfo.nameIndex].type.utf8Info;
                DF_u2 nameIdx = cp[nameAndTypeIdx].type.nameAndTypeInfo.nameIndex;
                DF_u2 typeIdx = cp[nameAndTypeIdx].type.nameAndTypeInfo.descriptorIndex;
                CF_CONSTANT_Utf8Info nameUtf8 = cp[nameIdx].type.utf8Info;
                CF_CONSTANT_Utf8Info descriptorUtf8 = cp[typeIdx].type.utf8Info;
                DF_uleb128 fieldIdx = DFL_parseFieldData(df, classNameUtf8, nameUtf8, descriptorUtf8);
                DF_u1 type = descriptorUtf8.bytes[0];
                if((((stacks[tos-1]) & 0xffff) >> 4) != 0) {
                    dpc = DFL_translateMoveInst(insns, dpc, stacks[tos-1], (tos-1));
                    stacks[tos-1] = (tos-1) | DFL_TYPE_OBJ;
                }
                assert(((stacks[tos-1] & 0xffff)>> 4) == 0);
                tos--;
                switch(type) {
                    case 'L':
                    case '[':
                    {
                        DF_write2BytesCode(insns, dpc, DF_IGET_OBJECT, (tos | (stacks[tos] & 0xffff) << 4)); dpc++;
                        DF_writeShortCode(insns, dpc, fieldIdx); dpc++;
                        stacks[tos] = tos | DFL_TYPE_OBJ;
                        tos ++;
                        break;
                    }
                    case 'D':
                    case 'J':
                    {
                        DF_write2BytesCode(insns, dpc, DF_IGET_WIDE, (tos | (stacks[tos] & 0xffff) << 4)); dpc++;
                        DF_writeShortCode(insns, dpc, fieldIdx); dpc++;
                        stacks[tos] =  tos | DFL_TYPE_WIDE;
                        stacks[tos+1] = (tos+1) | DFL_TYPE_WIDE;
                        tos += 2;
                        break;
                    }
                    case 'I':
                    case 'F':
                    {
                        DF_write2BytesCode(insns, dpc, DF_IGET, (tos | (stacks[tos] & 0xffff) << 4)); dpc++;
                        DF_writeShortCode(insns, dpc, fieldIdx); dpc++;
                        stacks[tos] = tos | DFL_TYPE_SINGLE;
                        tos ++;
                        break;
                    }
                    case 'Z':
                    {
                        DF_write2BytesCode(insns, dpc, DF_IGET_BOOLEAN, (tos | (stacks[tos] & 0xffff) << 4)); dpc++;
                        DF_writeShortCode(insns, dpc, fieldIdx); dpc++;
                        stacks[tos] = tos | DFL_TYPE_SINGLE;
                        tos ++;
                        break;
                    }
                    case 'B':
                    {
                        DF_write2BytesCode(insns, dpc, DF_IGET_BYTE, (tos | (stacks[tos] & 0xffff) << 4)); dpc++;
                        DF_writeShortCode(insns, dpc, fieldIdx); dpc++;
                        stacks[tos] = tos | DFL_TYPE_SINGLE;
                        tos ++;
                        break;
                    }
                    case 'C':
                    {
                        DF_write2BytesCode(insns, dpc, DF_IGET_CHAR, (tos | (stacks[tos] & 0xffff) << 4)); dpc++;
                        DF_writeShortCode(insns, dpc, fieldIdx); dpc++;
                        stacks[tos] = tos | DFL_TYPE_SINGLE;
                        tos ++;
                        break;
                    }
                    case 'S':
                    {
                        DF_write2BytesCode(insns, dpc, DF_IGET_SHORT, (tos | (stacks[tos] & 0xffff) << 4)); dpc++;
                        DF_writeShortCode(insns, dpc, fieldIdx); dpc++;
                        stacks[tos] = tos | DFL_TYPE_SINGLE;
                        tos ++;
                        break;
                    }
                    default:
                        assert(0);
                }
                break;
            }
            case CF_PUTFIELD:
            {
                DF_u2 idx = CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2]);
                CF_ConstantPool* cp = cf->cp;
                DF_u2 classIdx = cp[idx].type.fieldInfo.classIndex;
                DF_u2 nameAndTypeIdx = cp[idx].type.fieldInfo.nameAndTypeIndex;
                CF_CONSTANT_Utf8Info classNameUtf8 = cp[cp[classIdx].type.classInfo.nameIndex].type.utf8Info;
                DF_u2 nameIdx = cp[nameAndTypeIdx].type.nameAndTypeInfo.nameIndex;
                DF_u2 typeIdx = cp[nameAndTypeIdx].type.nameAndTypeInfo.descriptorIndex;
                CF_CONSTANT_Utf8Info nameUtf8 = cp[nameIdx].type.utf8Info;
                CF_CONSTANT_Utf8Info descriptorUtf8 = cp[typeIdx].type.utf8Info;
                DF_uleb128 fieldIdx = DFL_parseFieldData(df, classNameUtf8, nameUtf8, descriptorUtf8);
                DF_u1 type = descriptorUtf8.bytes[0];
                if(type == 'D' || type == 'J') {
                    if((((stacks[tos-3]) & 0xffff) >> 4) != 0) {
                        dpc = DFL_translateMoveInst(insns, dpc, stacks[tos-3], (tos-3));
                        stacks[tos-3] = (tos-3) | DFL_TYPE_OBJ;
                    }
                    if((((stacks[tos-2]) & 0xffff) >> 4) != 0) {
                        dpc = DFL_translateMoveInst(insns, dpc, stacks[tos-2], (tos-2));
                        stacks[tos-2] = (tos-2) | DFL_TYPE_WIDE;
                        stacks[tos-1] = (tos-1) | DFL_TYPE_WIDE;
                    }
                    assert(((stacks[tos-3] & 0xffff)>> 4) == 0);
                    assert(((stacks[tos-2] & 0xffff)>> 4) == 0);
                }else {
                    if((((stacks[tos-1]) & 0xffff) >> 4) != 0) {
                        dpc = DFL_translateMoveInst(insns, dpc, stacks[tos-1], (tos-1));
                        stacks[tos-1] = (tos-1) | (stacks[tos-1] & 0xf0000);
                    }
                    if((((stacks[tos-2]) & 0xffff) >> 4) != 0) {
                        dpc = DFL_translateMoveInst(insns, dpc, stacks[tos-2], (tos-2));
                        stacks[tos-2] = (tos-2) | DFL_TYPE_OBJ;
                    }
                    assert(((stacks[tos-1] & 0xffff)>> 4) == 0);
                    assert(((stacks[tos-2] & 0xffff)>> 4) == 0);

                }
                switch(type) {
                    case 'L':
                    case '[':
                    {
                        DF_write2BytesCode(insns, dpc, DF_IPUT_OBJECT, (stacks[tos-1] & 0xffff) | (stacks[tos-2] & 0xffff) << 4); dpc++;
                        DF_writeShortCode(insns, dpc, fieldIdx); dpc++;
                        tos -=2;
                        break;
                    }
                    case 'D':
                    case 'J':
                    {
                        DF_write2BytesCode(insns, dpc, DF_IPUT_WIDE, (stacks[tos-2] & 0xffff) | (stacks[tos-3] & 0xffff) << 4); dpc++;
                        DF_writeShortCode(insns, dpc, fieldIdx); dpc++;
                        tos -= 3;
                        break;
                    }
                    case 'I':
                    case 'F':
                    {
                        DF_write2BytesCode(insns, dpc, DF_IPUT, (stacks[tos-1] & 0xffff) | (stacks[tos-2] & 0xffff) << 4); dpc++;
                        DF_writeShortCode(insns, dpc, fieldIdx); dpc++;
                        tos -=2;
                        break;
                    }
                    case 'Z':
                    {
                        DF_write2BytesCode(insns, dpc, DF_IPUT_BOOLEAN, (stacks[tos-1] & 0xffff) | (stacks[tos-2] & 0xffff) << 4); dpc++;
                        DF_writeShortCode(insns, dpc, fieldIdx); dpc++;
                        tos -=2;
                        break;
                    }
                    case 'B':
                    {
                        DF_write2BytesCode(insns, dpc, DF_IPUT_BYTE, (stacks[tos-1] & 0xffff) | (stacks[tos-2] & 0xffff) << 4); dpc++;
                        DF_writeShortCode(insns, dpc, fieldIdx); dpc++;
                        tos -=2;
                        break;
                    }
                    case 'C':
                    {
                        DF_write2BytesCode(insns, dpc, DF_IPUT_CHAR, (stacks[tos-1] & 0xffff) | (stacks[tos-2] & 0xffff) << 4); dpc++;
                        DF_writeShortCode(insns, dpc, fieldIdx); dpc++;
                        tos -=2;
                        break;
                    }
                    case 'S':
                    {
                        DF_write2BytesCode(insns, dpc, DF_IPUT_SHORT, (stacks[tos-1] & 0xffff) | (stacks[tos-2] & 0xffff) << 4); dpc++;
                        DF_writeShortCode(insns, dpc, fieldIdx); dpc++;
                        tos -=2;
                        break;
                    }
                }
                break;
            }            
            case CF_INVOKEVIRTUAL:
            {
                DF_u2 idx = CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2]);
                CF_ConstantPool* cp = cf->cp;
                DF_u2 classIdx = cp[idx].type.methodInfo.classIndex;
                DF_u2 nameAndTypeIdx = cp[idx].type.methodInfo.nameAndTypeIndex;
                CF_CONSTANT_Utf8Info classNameUtf8 = cp[cp[classIdx].type.classInfo.nameIndex].type.utf8Info;
                DF_u2 nameIdx = cp[nameAndTypeIdx].type.nameAndTypeInfo.nameIndex;
                DF_u2 typeIdx = cp[nameAndTypeIdx].type.nameAndTypeInfo.descriptorIndex;
                CF_CONSTANT_Utf8Info nameUtf8 = cp[nameIdx].type.utf8Info;
                CF_CONSTANT_Utf8Info descriptorUtf8 = cp[typeIdx].type.utf8Info;
                DF_u4 shortyIdx = df->protoIds[DFL_parseProtoData(df, descriptorUtf8)].shortyIdx;
                DF_DATA_String shortyStr = df->stringItems[shortyIdx];
                DF_uleb128 methodIdx = DFL_parseMethodData(df, classNameUtf8, nameUtf8, descriptorUtf8);

                DF_u2 parameterSize = 1;
                DF_u1 ranged = 0;
                for(i = 1 ; i < shortyStr.size ; i++) {
                    if(shortyStr.data[i] == 'J'
                        || shortyStr.data[i] == 'D') {
                        parameterSize += 2;
                    }else parameterSize++;
                }
                if(parameterSize <= 5) {
                    for(i = 0; i < parameterSize ; i++) {
                        if((stacks[tos - i - 1] & 0xffff) >> 4 != 0) {
                            ranged = 1;
                            break;
                        }
                    }
                }else ranged = 1;

                if(ranged == 0) {
                    DF_u2 args = 0;
                    if(parameterSize == 5) {
                        DF_write2BytesCode(insns, dpc, DF_INVOKE_VIRTUAL, (parameterSize << 4) | (stacks[tos-1] & 0xffff)); dpc++;
                        i = 1;
                    }else {
                        DF_write2BytesCode(insns, dpc, DF_INVOKE_VIRTUAL, parameterSize<< 4); dpc++;
                        i = 0;
                    }
                    DF_writeShortCode(insns, dpc, methodIdx); dpc++;

                    for(; i < parameterSize ; i++) {
                        args = (args << 4) | (stacks[tos - i - 1] & 0xffff);
                    }
                    DF_writeShortCode(insns, dpc, args);dpc++;
                }else {
                    dpc = DFL_restoreAllCopy(insns, dpc, stacks, tos);
                    DF_write2BytesCode(insns, dpc, DF_INVOKE_VIRTUAL_RANGE, parameterSize);dpc++;
                    DF_writeShortCode(insns, dpc, methodIdx); dpc++;
                    DF_writeShortCode(insns, dpc, tos - parameterSize); dpc++;
                }
                tos -= parameterSize;
                {
                    DF_u1 returnChar = shortyStr.data[0];
                    if(returnChar == 'J'
                            || returnChar == 'D') {
                        DF_write2BytesCode(insns, dpc, DF_MOVE_RESULT_WIDE, tos);dpc++;
                        stacks[tos] = (tos) | DFL_TYPE_WIDE;
                        stacks[tos+1] = (tos+1) | DFL_TYPE_WIDE;
                        tos += 2;
                    }else if(returnChar != 'V') {
                        if(returnChar == 'L' || returnChar == '[') {
                            DF_write2BytesCode(insns, dpc, DF_MOVE_RESULT_OBJECT, tos);dpc++;
                            stacks[tos] = (tos) | DFL_TYPE_OBJ;
                        }else {
                            DF_write2BytesCode(insns, dpc, DF_MOVE_RESULT, tos);dpc++;
                            stacks[tos] = (tos) | DFL_TYPE_SINGLE;
                        }
                        tos +=1;
                    }
                }
                if(parameterSize > codeItem->outsSize) { codeItem->outsSize = parameterSize; }
                break;
            }
            case CF_INVOKESPECIAL:
            {
                DF_u2 idx = CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2]);
                CF_ConstantPool* cp = cf->cp;
                DF_u2 classIdx = cp[idx].type.methodInfo.classIndex;
                DF_u2 nameAndTypeIdx = cp[idx].type.methodInfo.nameAndTypeIndex;
                CF_CONSTANT_Utf8Info classNameUtf8 = cp[cp[classIdx].type.classInfo.nameIndex].type.utf8Info;
                DF_u2 nameIdx = cp[nameAndTypeIdx].type.nameAndTypeInfo.nameIndex;
                DF_u2 typeIdx = cp[nameAndTypeIdx].type.nameAndTypeInfo.descriptorIndex;
                CF_CONSTANT_Utf8Info nameUtf8 = cp[nameIdx].type.utf8Info;
                CF_CONSTANT_Utf8Info descriptorUtf8 = cp[typeIdx].type.utf8Info;
                DF_u4 shortyIdx = df->protoIds[DFL_parseProtoData(df, descriptorUtf8)].shortyIdx;
                DF_DATA_String shortyStr = df->stringItems[shortyIdx];
                DF_uleb128 methodIdx = DFL_parseMethodData(df, classNameUtf8, nameUtf8, descriptorUtf8);

                DF_u2 parameterSize = 1;
                DF_u1 ranged = 0;
                for(i = 1 ; i < shortyStr.size ; i++) {
                    if(shortyStr.data[i] == 'J'
                        || shortyStr.data[i] == 'D') {
                        parameterSize += 2;
                    } else parameterSize++;
                }
                if(parameterSize <= 5) {
                    for(i = 0; i < parameterSize ; i++) {
                        if((stacks[tos - i - 1] & 0xffff) >> 4 != 0) {
                            ranged = 1;
                            break;
                        }
                    }
                }else ranged = 1;

                if(ranged == 0) {
                    DF_u2 args = 0;
                    if(parameterSize == 5) {
                        DF_write2BytesCode(insns, dpc, DF_INVOKE_DIRECT, (parameterSize << 4) | (stacks[tos-1] & 0xffff)); dpc++;
                        i = 1;
                    }else {
                        DF_write2BytesCode(insns, dpc, DF_INVOKE_DIRECT, parameterSize << 4); dpc++;
                        i = 0;
                    }
                    DF_writeShortCode(insns, dpc, methodIdx); dpc++;

                    for(; i < parameterSize ; i++) {
                        args = (args << 4) | (stacks[tos - i - 1] & 0xffff);
                    }
                    DF_writeShortCode(insns, dpc, args);dpc++;
                }else {
                    dpc = DFL_restoreAllCopy(insns, dpc, stacks, tos);
                    DF_write2BytesCode(insns, dpc, DF_INVOKE_DIRECT_RANGE, parameterSize);dpc++;
                    DF_writeShortCode(insns, dpc, methodIdx); dpc++;
                    DF_writeShortCode(insns, dpc, tos - parameterSize); dpc++;
                }
                tos -= parameterSize;
                {
                    DF_u1 returnChar = shortyStr.data[0];
                    if(returnChar == 'J'
                            || returnChar == 'D') {
                        DF_write2BytesCode(insns, dpc, DF_MOVE_RESULT_WIDE, tos);dpc++;
                        stacks[tos] = (tos) | DFL_TYPE_WIDE;
                        stacks[tos+1] = (tos+1) | DFL_TYPE_WIDE;
                        tos += 2;
                    }else if(returnChar != 'V') {
                        if(returnChar == 'L' || returnChar == '[') {
                            DF_write2BytesCode(insns, dpc, DF_MOVE_RESULT_OBJECT, tos);dpc++;
                            stacks[tos] = (tos) | DFL_TYPE_OBJ;
                        }else {
                            DF_write2BytesCode(insns, dpc, DF_MOVE_RESULT, tos);dpc++;
                            stacks[tos] = (tos) | DFL_TYPE_SINGLE;
                        }
                        tos +=1;
                    }
                }
                if(parameterSize > codeItem->outsSize) { codeItem->outsSize = parameterSize; }
                break;
            }
            case CF_INVOKESTATIC:
            {
                DF_u2 idx = CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2]);
                CF_ConstantPool* cp = cf->cp;
                DF_u2 classIdx = cp[idx].type.methodInfo.classIndex;
                DF_u2 nameAndTypeIdx = cp[idx].type.methodInfo.nameAndTypeIndex;
                CF_CONSTANT_Utf8Info classNameUtf8 = cp[cp[classIdx].type.classInfo.nameIndex].type.utf8Info;
                DF_u2 nameIdx = cp[nameAndTypeIdx].type.nameAndTypeInfo.nameIndex;
                DF_u2 typeIdx = cp[nameAndTypeIdx].type.nameAndTypeInfo.descriptorIndex;
                CF_CONSTANT_Utf8Info nameUtf8 = cp[nameIdx].type.utf8Info;
                CF_CONSTANT_Utf8Info descriptorUtf8 = cp[typeIdx].type.utf8Info;
                DF_u4 shortyIdx = df->protoIds[DFL_parseProtoData(df, descriptorUtf8)].shortyIdx;
                DF_DATA_String shortyStr = df->stringItems[shortyIdx];
                DF_uleb128 methodIdx = DFL_parseMethodData(df, classNameUtf8, nameUtf8, descriptorUtf8);

                DF_u2 parameterSize = 0;
                DF_u1 ranged = 0;
                for(i = 1 ; i < shortyStr.size ; i++) {
                    if(shortyStr.data[i] == 'J'
                        || shortyStr.data[i] == 'D') {
                        parameterSize += 2;
                    } else parameterSize++;
                }

                if(parameterSize <= 5) {
                    for(i = 0; i < parameterSize ; i++) {
                        if((stacks[tos - i - 1] & 0xffff) >> 4 != 0) {
                            ranged = 1;
                            break;
                        }
                    }
                }else ranged = 1;

                if(ranged == 0) {
                    DF_u2 args = 0;
                    if(parameterSize == 5) {
                        DF_write2BytesCode(insns, dpc, DF_INVOKE_STATIC, (parameterSize << 4) | (stacks[tos-1] & 0xffff)); dpc++;
                        i = 1;
                    }else {
                        DF_write2BytesCode(insns, dpc, DF_INVOKE_STATIC, parameterSize << 4); dpc++;
                        i = 0;
                    }
                    DF_writeShortCode(insns, dpc, methodIdx); dpc++;

                    for(; i < parameterSize ; i++) {
                        args = (args << 4) | (stacks[tos - i - 1] & 0xffff);
                    }
                    DF_writeShortCode(insns, dpc, args);dpc++;
                }else {
                    dpc = DFL_restoreAllCopy(insns, dpc, stacks, tos);
                    DF_write2BytesCode(insns, dpc, DF_INVOKE_STATIC_RANGE, parameterSize);dpc++;
                    DF_writeShortCode(insns, dpc, methodIdx); dpc++;
                    DF_writeShortCode(insns, dpc, tos - parameterSize); dpc++;
                }
                tos -= parameterSize;
                {
                    DF_u1 returnChar = shortyStr.data[0];
                    if(returnChar == 'J'
                            || returnChar == 'D') {
                        DF_write2BytesCode(insns, dpc, DF_MOVE_RESULT_WIDE, tos);dpc++;
                        stacks[tos] = (tos) | DFL_TYPE_WIDE;
                        stacks[tos+1] = (tos+1) | DFL_TYPE_WIDE;
                        tos += 2;
                    }else if(returnChar != 'V') {
                        if(returnChar == 'L' || returnChar == '[') {
                            DF_write2BytesCode(insns, dpc, DF_MOVE_RESULT_OBJECT, tos);dpc++;
                            stacks[tos] = (tos) | DFL_TYPE_OBJ;
                        }else {
                            DF_write2BytesCode(insns, dpc, DF_MOVE_RESULT, tos);dpc++;
                            stacks[tos] = (tos) | DFL_TYPE_SINGLE;
                        }
                        tos +=1;
                    }
                }
                if(parameterSize > codeItem->outsSize) { codeItem->outsSize = parameterSize; }
                break;
            }
            case CF_INVOKEINTERFACE:
            {
                DF_u2 idx = CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2]);
                CF_ConstantPool* cp = cf->cp;
                DF_u2 classIdx = cp[idx].type.methodInfo.classIndex;
                DF_u2 nameAndTypeIdx = cp[idx].type.methodInfo.nameAndTypeIndex;
                CF_CONSTANT_Utf8Info classNameUtf8 = cp[cp[classIdx].type.classInfo.nameIndex].type.utf8Info;
                DF_u2 nameIdx = cp[nameAndTypeIdx].type.nameAndTypeInfo.nameIndex;
                DF_u2 typeIdx = cp[nameAndTypeIdx].type.nameAndTypeInfo.descriptorIndex;
                CF_CONSTANT_Utf8Info nameUtf8 = cp[nameIdx].type.utf8Info;
                CF_CONSTANT_Utf8Info descriptorUtf8 = cp[typeIdx].type.utf8Info;
                DF_u4 shortyIdx = df->protoIds[DFL_parseProtoData(df, descriptorUtf8)].shortyIdx;
                DF_DATA_String shortyStr = df->stringItems[shortyIdx];
                DF_uleb128 methodIdx = DFL_parseMethodData(df, classNameUtf8, nameUtf8, descriptorUtf8);

                DF_u2 parameterSize = 1;
                DF_u1 ranged = 0;
                for(i = 1 ; i < shortyStr.size ; i++) {
                    if(shortyStr.data[i] == 'J'
                        || shortyStr.data[i] == 'D') {
                        parameterSize += 2;
                    } else parameterSize++;
                }
                if(parameterSize <= 5) {
                    for(i = 0; i < parameterSize ; i++) {
                        if((stacks[tos - i - 1] & 0xffff) >> 4 != 0) {
                            ranged = 1;
                            break;
                        }
                    }
                }else ranged = 1;

                if(ranged == 0) {
                    DF_u2 args = 0;
                    if(parameterSize == 5) {
                        DF_write2BytesCode(insns, dpc, DF_INVOKE_DIRECT, (parameterSize << 4) | (stacks[tos-1] & 0xffff)); dpc++;
                        i = 1;
                    }else {
                        DF_write2BytesCode(insns, dpc, DF_INVOKE_INTERFACE, parameterSize << 4); dpc++;
                        i = 0;
                    }
                    DF_writeShortCode(insns, dpc, methodIdx); dpc++;

                    for(; i < parameterSize ; i++) {
                        args = (args << 4) | (stacks[tos - i - 1] & 0xffff);
                    }
                    DF_writeShortCode(insns, dpc, args);dpc++;
                }else {
                    dpc = DFL_restoreAllCopy(insns, dpc, stacks, tos);
                    DF_write2BytesCode(insns, dpc, DF_INVOKE_INTERFACE_RANGE, parameterSize);dpc++;
                    DF_writeShortCode(insns, dpc, methodIdx); dpc++;
                    DF_writeShortCode(insns, dpc, tos - parameterSize); dpc++;
                }
                tos -= parameterSize;
                {
                    DF_u1 returnChar = shortyStr.data[0];
                    if(returnChar == 'J'
                            || returnChar == 'D') {
                        DF_write2BytesCode(insns, dpc, DF_MOVE_RESULT_WIDE, tos);dpc++;
                        stacks[tos] = (tos) | DFL_TYPE_WIDE;
                        stacks[tos+1] = (tos+1) | DFL_TYPE_WIDE;
                        tos += 2;
                    }else if(returnChar != 'V') {
                        if(returnChar == 'L' || returnChar == '[') {
                            DF_write2BytesCode(insns, dpc, DF_MOVE_RESULT_OBJECT, tos);dpc++;
                            stacks[tos] = (tos) | DFL_TYPE_OBJ;
                        }else {
                            DF_write2BytesCode(insns, dpc, DF_MOVE_RESULT, tos);dpc++;
                            stacks[tos] = (tos) | DFL_TYPE_SINGLE;
                        }
                        tos +=1;
                    }
                }
                if(parameterSize > codeItem->outsSize) { codeItem->outsSize = parameterSize; }
                break;
            }
            case CF_NEW:
            {
                CF_ConstantPool* cp = cf->cp;
                CF_CONSTANT_Utf8Info classUtf8 = cp[cp[CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2])].type.classInfo.nameIndex].type.utf8Info;
                DF_u2 idx = DFL_parseClassData(df, classUtf8);
                assert((tos >> 8) == 0);
                DF_write2BytesCode(insns, dpc, DF_NEW_INSTANCE, tos); dpc++;
                DF_writeShortCode(insns, dpc, idx); dpc++;
                stacks[tos] = tos | DFL_TYPE_OBJ;
                tos++;
                break;
            }
            case CF_NEWARRAY:
            {
                DF_u1 type = bcodes[bpc+1];
                DF_u1* typeUtf8 = NULL;
                DF_u2 typeIdx;
                assert(((stacks[tos-1] & 0xffff) >> 4) == 0);
                switch(type) {
                    case 4:
                        {
                            typeUtf8 = (DF_u1*)"[Z";
                            break;
                        }
                    case 5:
                        {
                            typeUtf8 = (DF_u1*)"[C";
                            break;
                        }
                    case 6:
                        {
                            typeUtf8 = (DF_u1*)"[F";
                            break;
                        }
                    case 7:
                        {
                            typeUtf8 = (DF_u1*)"[D";
                            break;
                        }
                    case 8:
                        {
                            typeUtf8 = (DF_u1*)"[B";
                            break;
                        }
                    case 9:
                        {
                            typeUtf8 = (DF_u1*)"[S";
                            break;
                        }
                    case 10:
                        {
                            typeUtf8 = (DF_u1*)"[I";
                            break;
                        }
                    case 11:
                        {
                            typeUtf8 = (DF_u1*)"[J";
                            break;
                        }
                    default:
                        assert(0);
                        break;
                }
                typeIdx = DFL_parseTypeUtf8(df, 2, typeUtf8);
                DF_write2BytesCode(insns, dpc, DF_NEW_ARRAY, (tos - 1) | (stacks[tos-1] & 0xffff) << 4);dpc++;
                DF_writeShortCode(insns, dpc, typeIdx); dpc++;
                stacks[tos-1] = (tos-1) | DFL_TYPE_OBJ;
                break;
            }
            case CF_ANEWARRAY:
            {
                assert(((stacks[tos-1] & 0xffff) >> 4) == 0);
                CF_ConstantPool* cp = cf->cp;
                CF_CONSTANT_Utf8Info classUtf8 = cp[cp[CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2])].type.classInfo.nameIndex].type.utf8Info;
                DF_u2 len = classUtf8.length + 3;
                DF_u1* newBytes = (DF_u1*)allocateMem(df->mm, sizeof(DF_u1) * (len + 1));
                {
                    strcpy((char*)newBytes, "[L");
                    strcpy((char*)(newBytes + 2), (char*)classUtf8.bytes);
                    newBytes[len - 1] = ';';
                    newBytes[len] = '\0';
                }
                DF_u2 typeIdx = DFL_parseTypeUtf8(df, len, newBytes);
                DF_write2BytesCode(insns, dpc, DF_NEW_ARRAY, (tos - 1) | (stacks[tos-1] & 0xffff) << 4);dpc++;
                DF_writeShortCode(insns, dpc, typeIdx); dpc++;
                stacks[tos-1] = (tos-1) | DFL_TYPE_OBJ;
                break;
            }
            case CF_CHECKCAST:
            {
                CF_ConstantPool* cp = cf->cp;
                
                CF_CONSTANT_Utf8Info classUtf8 = cp[cp[CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2])].type.classInfo.nameIndex].type.utf8Info;
                DF_u2 typeIdx = DFL_parseClassData(df, classUtf8);
                DF_write2BytesCode(insns, dpc, DF_CHECK_CAST, stacks[tos-1] & 0xffff); dpc++;
                DF_writeShortCode(insns, dpc, typeIdx); dpc++;
                break;

            }
            case CF_INSTANCEOF:
            {
                CF_ConstantPool* cp = cf->cp;
                CF_CONSTANT_Utf8Info classUtf8 = cp[cp[CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2])].type.classInfo.nameIndex].type.utf8Info;
                DF_u2 typeIdx = DFL_parseClassData(df, classUtf8);
                DF_write2BytesCode(insns, dpc, DF_INSTANCE_OF, (tos - 1) | (stacks[tos-1] & 0xffff) << 4); dpc++;
                DF_writeShortCode(insns, dpc, typeIdx); dpc++;
                stacks[tos-1] = (tos-1) | DFL_TYPE_SINGLE;
                break;
            }
            case CF_MULTIANEWARRAY:
            {
                DF_u1 size = bcodes[bpc+3];
                DF_u2 typeIdx = DFL_parseTypeUtf8(df, 2, (DF_u1*)"[I");
                DF_u1 ranged = 0;
                CF_ConstantPool* cp = cf->cp;
                // filled-new-array
                for(i = 0 ; i < size ; i++) {
                    if (((stacks[tos-i] & 0xffff) >> 4) != 0 || size > 5) {
                        ranged = 1;
                        break;
                    }
                }
                if(ranged == 0) {
                    if(size == 5) {
                        DF_write2BytesCode(insns, dpc, DF_FILLED_NEW_ARRAY,size << 4 | (stacks[tos-1] & 0xffff)); dpc++;
                        i = 1;
                    }else {
                        DF_write2BytesCode(insns, dpc, DF_FILLED_NEW_ARRAY,size << 4); dpc++;
                        i = 0;
                    }
                    DF_writeShortCode(insns, dpc, typeIdx);dpc++;
                    DF_u2 args = 0;

                    for(; i < size ; i++) {
                        args = (args << 4) | (stacks[tos - i -1] & 0xffff);
                    }
                    DF_writeShortCode(insns, dpc, args); dpc++;
                }else {
                    dpc = DFL_restoreAllCopy(insns, dpc, stacks, tos);
                    DF_write2BytesCode(insns, dpc, DF_FILLED_NEW_ARRAY_RANGE, size);dpc++;
                    DF_writeShortCode(insns, dpc, typeIdx);dpc++;
                    DF_writeShortCode(insns, dpc, tos - size);dpc++;

                }
                tos -= size;
                // move-result-object
                DF_write2BytesCode(insns, dpc, DF_MOVE_RESULT_OBJECT, tos+1);dpc++;
                // const-class or sget-object
                CF_CONSTANT_Utf8Info classUtf8 = cp[cp[CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2])].type.classInfo.nameIndex].type.utf8Info;
                DF_u1* bytes = classUtf8.bytes;
                DF_u4 len = classUtf8.length;
                if(((char)bytes[0]) == '[') {
                    i = 0;
                    DF_u2 newLen;
                    while(((char)bytes[i]) == '[') i++;
                    newLen = len-i;
                    if(newLen == 1) {
                        switch((char)bytes[i]) {
                            case 'Z':
                                {
                                    typeIdx = DFL_parseFieldDataUtf8(df, 19, (DF_u1*)"Ljava/lang/Boolean;", 4, (DF_u1*)"TYPE", 17, (DF_u1*)"Ljava/lang/Class;");
                                    break;
                                }
                            case 'C':
                                {
                                    typeIdx = DFL_parseFieldDataUtf8(df, 16, (DF_u1*)"Ljava/lang/Char;", 4, (DF_u1*)"TYPE", 17, (DF_u1*)"Ljava/lang/Class;");
                                    break;
                                }
                            case 'F':
                                {
                                    typeIdx = DFL_parseFieldDataUtf8(df, 17, (DF_u1*)"Ljava/lang/Float;", 4, (DF_u1*)"TYPE", 17, (DF_u1*)"Ljava/lang/Class;");
                                    break;
                                }
                            case 'D':
                                {
                                    typeIdx = DFL_parseFieldDataUtf8(df, 18, (DF_u1*)"Ljava/lang/Double;", 4, (DF_u1*)"TYPE", 17, (DF_u1*)"Ljava/lang/Class;");
                                    break;
                                }
                            case 'B':
                                {
                                    typeIdx = DFL_parseFieldDataUtf8(df, 16, (DF_u1*)"Ljava/lang/Byte;", 4, (DF_u1*)"TYPE", 17, (DF_u1*)"Ljava/lang/Class;");
                                    break;
                                }
                            case 'S':
                                {
                                    typeIdx = DFL_parseFieldDataUtf8(df, 17, (DF_u1*)"Ljava/lang/Short;", 4, (DF_u1*)"TYPE", 17, (DF_u1*)"Ljava/lang/Class;");
                                    break;
                                }
                            case 'I':
                                {
                                    typeIdx = DFL_parseFieldDataUtf8(df, 19, (DF_u1*)"Ljava/lang/Integer;", 4, (DF_u1*)"TYPE", 17, (DF_u1*)"Ljava/lang/Class;");
                                    break;
                                }
                            case 'J':
                                {
                                    typeIdx = DFL_parseFieldDataUtf8(df, 16, (DF_u1*)"Ljava/lang/Long;", 4, (DF_u1*)"TYPE", 17, (DF_u1*)"Ljava/lang/Class;");
                                    break;
                                }
                        }
                        DF_write2BytesCode(insns, dpc, DF_SGET_OBJECT, tos);dpc++;
                        DF_writeShortCode(insns, dpc, typeIdx);dpc++;
                    }else {
                        typeIdx = DFL_parseTypeUtf8(df, newLen, (bytes + i));
                        DF_write2BytesCode(insns, dpc, DF_CONST_CLASS, tos);dpc++;
                        DF_writeShortCode(insns, dpc, typeIdx); dpc++;
                    }
                }
                // invoke-static
                {
                    typeIdx = DFL_parseMethodDataUtf8(df, 25, (DF_u1*)"Ljava/lang/reflect/Array;", 11, (DF_u1*)"newInstance", 39, (DF_u1*)"(Ljava/lang/Class;[I)Ljava/lang/Object;");
                    if(((tos+1) >> 4) == 0) {
                        DF_write2BytesCode(insns, dpc, DF_INVOKE_STATIC, 2<<4);dpc++;
                        DF_writeShortCode(insns, dpc, typeIdx);dpc++;
                        DF_writeShortCode(insns, dpc, tos | (tos+1) << 4);dpc++;
                    }else {
                        DF_write2BytesCode(insns, dpc, DF_INVOKE_STATIC_RANGE, 2);dpc++;
                        DF_writeShortCode(insns, dpc, typeIdx);dpc++;
                        DF_writeShortCode(insns, dpc, tos);dpc++;
                    }
                    if(2> codeItem->outsSize) { codeItem->outsSize = 2; }
                }
                // move-result-object
                DF_write2BytesCode(insns, dpc, DF_MOVE_RESULT_OBJECT, tos);dpc++;
                // check-cast
                DF_write2BytesCode(insns, dpc, DF_CHECK_CAST, tos);dpc++;
                typeIdx = DFL_parseTypeUtf8(df, len, bytes);
                DF_writeShortCode(insns, dpc, typeIdx);dpc++;

                stacks[tos] = tos | DFL_TYPE_OBJ;
                tos++;
                break;
            }
            case CF_TABLESWITCH:
            {
                DF_u4 addr = (bpc+4) & 0xFFFFFFFC;
                DF_u4 pads = addr - bpc - 1;
                CF_u4 low = CF_makeIntVal(bcodes[addr+4], bcodes[addr+5], bcodes[addr+6], bcodes[addr+7]);
                CF_u4 high = CF_makeIntVal(bcodes[addr+8], bcodes[addr+9], bcodes[addr+10], bcodes[addr+11]);
                DF_u4 npairs = high - low + 1;
                DF_s4 offset;
                DF_u4 targetBpc;
                dpc = DFL_restoreAllCopy(insns, dpc, stacks, tos);
                DF_u4 startDpc = dpc;
                DF_write2BytesCode(insns, dpc, DF_PACKED_SWITCH, stacks[tos-1] & 0xffff);dpc++;
                DF_writeIntCode(insns, dpc, 5); dpc+=2;
                
                //default offset
                offset = CF_makeIntVal(bcodes[addr], bcodes[addr+1], bcodes[addr+2], bcodes[addr+3]);
                targetBpc = bpc + offset;
                {
                    DF_s4 dpcOffset = 0;
                    if(offset < 0) {
                        dpcOffset = table[targetBpc]->dPc - dpc;
                    }else {
                        DFL_PatchNode* node = (DFL_PatchNode*)allocateMem(df->mm, sizeof(DFL_PatchNode));
                        node->targetBpc = targetBpc;
                        node->dpc = dpc;
                        node->patchDpc = dpc + 1;
                        node->offsetSize = 2;
                        node->next = header;
                        header = node;
                        dpcOffset = 0xfff;
                    }
                    //if((dpcOffset >> 8) == 0) {
                        //DF_write2BytesCode(insns, dpc, DF_GOTO, dpcOffset);dpc++;
                    //}else if((dpcOffset >> 16) == 0){
                    if((dpcOffset >> 16) == 0){
                        DF_write2BytesCode(insns, dpc, DF_GOTO_16, 0);dpc++;
                        DF_writeShortCode(insns, dpc, dpcOffset);dpc++;
                    }else {
                        assert(0);
                    }
                }
                DF_writeShortCode(insns, dpc, 0x0100);dpc++;
                DF_writeShortCode(insns, dpc, npairs);dpc++;
                DF_writeIntCode(insns, dpc, low);dpc+=2;
                
                // case statement
                DF_u4 start = addr + 12;
                DF_u4 i;
                for(i = 0 ; i < npairs ; i++) {
                    offset = CF_makeIntVal(bcodes[start + 4 * i], bcodes[start + 4 * i + 1], bcodes[start + 4 * i + 2], bcodes[start + 4 * i + 3]);
                    targetBpc = bpc + offset;
                    if(offset < 0) {
                        DF_writeIntCode(insns, dpc, table[targetBpc]->dPc);dpc+=2;
                    }else {
                        DFL_PatchNode* node = (DFL_PatchNode*)allocateMem(df->mm, sizeof(DFL_PatchNode));
                        node->targetBpc = targetBpc;
                        node->dpc = startDpc;
                        node->patchDpc = dpc;
                        node->offsetSize = 4;
                        node->next = header;
                        header = node;
                        DF_writeIntCode(insns, dpc, 0);dpc+=2;
                    }
                }

                
                tos--;
                break;
            }
            case CF_LOOKUPSWITCH:
            {
                DF_u4 addr = (bpc+4) & 0xFFFFFFFC;
                DF_u4 pads = addr - bpc - 1;
                DF_u4 npairs = CF_makeIntVal(bcodes[addr+4], bcodes[addr+5], bcodes[addr+6], bcodes[addr+7]);
                DF_s4 offset;
                DF_u4 targetBpc;
                dpc = DFL_restoreAllCopy(insns, dpc, stacks, tos);
                DF_u4 startDpc = dpc;
                DF_write2BytesCode(insns, dpc, DF_SPARSE_SWITCH, stacks[tos-1] & 0xffff);dpc++;
                DF_writeIntCode(insns, dpc, 5); dpc+=2;
                
                //default offset
                offset = CF_makeIntVal(bcodes[addr], bcodes[addr+1], bcodes[addr+2], bcodes[addr+3]);
                targetBpc = bpc + offset;
                {
                    DF_s2 dpcOffset = 0;
                    if(offset < 0) {
                        dpcOffset = table[targetBpc]->dPc - dpc;
                    }else {
                        DFL_PatchNode* node = (DFL_PatchNode*)allocateMem(df->mm, sizeof(DFL_PatchNode));
                        node->targetBpc = targetBpc;
                        node->dpc = dpc;
                        node->patchDpc = dpc + 1;
                        node->offsetSize = 2;
                        node->next = header;
                        header = node;
                        dpcOffset = 0xfff;
                    }
                    //if((dpcOffset >> 8) == 0) {
                        //DF_write2BytesCode(insns, dpc, DF_GOTO, dpcOffset);dpc++;
                    //}else if((dpcOffset >> 16) == 0){
                    if((dpcOffset >> 16) == 0){
                        DF_write2BytesCode(insns, dpc, DF_GOTO_16, 0);dpc++;
                        DF_writeShortCode(insns, dpc, dpcOffset);dpc++;
                    }else {
                        assert(0);
                    }
                }

                DF_writeShortCode(insns, dpc, 0x0200);dpc++;
                DF_writeShortCode(insns, dpc, npairs);dpc++;
                
                // case statement
                DF_u4 start = addr + 12;
                DF_u4 i;
                for(i = 0 ; i < npairs ; i++) {
                    DF_s4 key = CF_makeIntVal(bcodes[start + 8 * i - 4], bcodes[start + 8 * i - 3 ], bcodes[start + 8 * i - 2], bcodes[start + 8 * i - 1]);
                    DF_writeIntCode(insns, dpc, key);dpc+=2;
                }
                for(i = 0 ; i < npairs ; i++) {
                    offset = CF_makeIntVal(bcodes[start + 8 * i], bcodes[start + 8 * i + 1], bcodes[start + 8 * i + 2], bcodes[start + 8 * i + 3]);
                    targetBpc = bpc + offset;
                    if(offset < 0) {
                        DF_writeIntCode(insns, dpc, table[targetBpc]->dPc);dpc+=2;
                    }else {
                        DFL_PatchNode* node = (DFL_PatchNode*)allocateMem(df->mm, sizeof(DFL_PatchNode));
                        node->targetBpc = targetBpc;
                        node->dpc = startDpc;
                        node->patchDpc = dpc;
                        node->offsetSize = 4;
                        node->next = header;
                        header = node;
                        DF_writeIntCode(insns, dpc, 0);dpc+=2;
                    }
                }
                
                tos--;
                break;
            }
            case CF_IFNULL:
            case CF_IFNONNULL:
            {
                assert(((stacks[tos-1] & 0xffff) >> 8) == 0);
                dpc = DFL_restoreAllCopy(insns, dpc, stacks, tos);
                DF_s2 bpcOffset = CF_makeShortVal(bcodes[bpc+1], bcodes[bpc+2]);
                DF_u2 opcode = DF_IF_EQZ + (bcodes[bpc] - CF_IFNULL);
                DF_s2 dpcOffset = 0;
                if(bpcOffset < 0) {
                    dpcOffset = dpc - table[(bpc + bpcOffset)]->dPc;
                }else {
                    DFL_PatchNode* node = (DFL_PatchNode*)allocateMem(df->mm, sizeof(DFL_PatchNode));
                    node->targetBpc = (bpc+bpcOffset);
                    node->dpc = dpc;
                    node->patchDpc = dpc+1;
                    node->offsetSize = 2;
                    node->next = header;
                    header = node;
                }
                DF_write2BytesCode(insns, dpc, opcode, stacks[tos-1] & 0xffff);dpc++;
                DF_writeShortCode(insns, dpc, dpcOffset);dpc++;
                tos--;
                break;
            }
            case CF_GOTO_W:
            {
                DF_s4 bpcOffset = CF_makeIntVal(bcodes[bpc+1], bcodes[bpc+2], bcodes[bpc+3], bcodes[bpc+4]);
                dpc = DFL_restoreAllCopy(insns, dpc, stacks, tos);
                DF_s4 dpcOffset = 0;
                if(bpcOffset < 0) {
                    dpcOffset = dpc - table[(bpc + bpcOffset)]->dPc;
                }else {
                    DFL_PatchNode* node = (DFL_PatchNode*)allocateMem(df->mm, sizeof(DFL_PatchNode));
                    node->targetBpc = (bpc+bpcOffset);
                    node->dpc = dpc;
                    node->patchDpc = dpc + 1;
                    node->offsetSize = 4;
                    node->next = header;
                    header = node;
                    dpcOffset = 0xfffffff;
                }
                if((dpcOffset >> 8) == 0) {
                    DF_write2BytesCode(insns, dpc, DF_GOTO, dpcOffset);dpc++;
                }else if((dpcOffset >> 16) == 0){
                    DF_write2BytesCode(insns, dpc, DF_GOTO_16, 0);dpc++;
                    DF_writeShortCode(insns, dpc, dpcOffset);dpc++;
                }else {
                    DF_write2BytesCode(insns, dpc, DF_GOTO_32, 0);dpc++;
                    DF_writeIntCode(insns, dpc, dpcOffset);dpc+=2;
                }
                tos--;
                break;
            }
            case CF_JSR:
            case CF_RET:
            case CF_JSR_W:
            {
                assert(0);
                break;
            }
        }
        assert(tos <= codeAttr->maxStack);
        bpc = nextBpc;
    }
    codeItem->insnsSize = dpc;
    freeMem(df->mm, locals);
    return header;
}

DFL_bool DFL_patchBranch(ClassFile* cf, DexFormat* df, CF_ATTRIBUTE_Code* codeAttr, DF_DATA_Code* codeItem, DFL_PatchNode* patchs) {
    DF_u2* insns = codeItem->insns;
    DF_IR_BB** table = codeItem->cfg;
    if(verbose) fprintf(stderr, "Patch Bytecode\n");
    while(patchs != NULL) {
        DFL_PatchNode* next = patchs->next;
        if(patchs->offsetSize == 2) {
            DF_writeShortCode(insns, patchs->patchDpc, (DF_s2)(table[patchs->targetBpc]->dPc - patchs->dpc));
        }else if(patchs->offsetSize == 4) {
            DF_writeIntCode(insns, patchs->patchDpc, (DF_s4)(table[patchs->targetBpc]->dPc - patchs->dpc));
        }else {
            assert(0);
        }
        freeMem(df->mm, patchs);
        patchs = next;
    }
    return DFL_TRUE;
}
DFL_bool DFL_parseBytecode(ClassFile* cf, DexFormat* df, CF_ATTRIBUTE_Code* codeAttr, DF_DATA_Code* codeItem) {
    DFL_constructCFG(cf, df, codeAttr, codeItem);
    DFL_emulateJavaStack(cf, df, codeAttr, codeItem);
    DFL_PatchNode* patchs = DFL_translateBytecode(cf, df, codeAttr, codeItem);
    DFL_patchBranch(cf, df, codeAttr, codeItem, patchs);
    return DFL_TRUE;
}
// TODO
DFL_bool DFL_parseExceptionPair(DexFormat* df, DF_DATA_Handler* handler, DF_sleb128 typeIdx, DF_uleb128 handlerAddr) {
    DF_sleb128 handlerSize = handler->size;
    if(typeIdx == -1) {
        handler->catchAllAddr = handlerAddr;
    }else {
        if((handlerSize % 5) == 0) {
            DF_u4 newSize = (handlerSize / 5 + 1) * 5;
            DF_DATA_TypeAddrPair* newPairs = (DF_DATA_TypeAddrPair*) allocateMem(df->mm, sizeof(DF_DATA_TypeAddrPair) * newSize);
            memcpy(newPairs, handler->pairs, sizeof(DF_DATA_TypeAddrPair) * handlerSize);
            freeMem(df->mm, handler->pairs);
            handler->pairs= newPairs;
        }
        {
            df->codeItemsLen += 8;
            handler->pairs[handlerSize].typeIdx = (DF_uleb128)typeIdx;
            handler->pairs[handlerSize].addr = handlerAddr;
            (handler->size) ++;
        }
    }
    return DFL_TRUE;
}
DFL_bool DFL_parseExceptionHandler(DexFormat* df, DF_DATA_Code* codeItem, DF_u4 startAddr, DF_u2 insnCount, DF_sleb128 typeIdx, DF_uleb128 handlerAddr, DFL_bool recursive) {
    DF_u4 triesSize = codeItem->triesSize;
    DF_DATA_HandlerList* handlers = &codeItem->handlers;
    if((triesSize % 5) == 0) {
        DF_u4 newSize = (triesSize / 5 + 1) * 5;
        DF_DATA_Handler* newList = (DF_DATA_Handler*)allocateMem(df->mm ,sizeof(DF_DATA_Handler) * newSize);
        DF_DATA_Try* newTries = (DF_DATA_Try*)allocateMem(df->mm, sizeof(DF_DATA_Try) * newSize);
        memcpy(newList, handlers->list, sizeof(DF_DATA_Handler) * codeItem->triesSize);
        memcpy(newTries, codeItem->tries, sizeof(DF_DATA_Try) * codeItem->triesSize);
        freeMem(df->mm, handlers->list);
        freeMem(df->mm, codeItem->tries);
        handlers->list = newList;
        codeItem->tries = newTries;
    }
    codeItem->tries[triesSize].startAddr = startAddr;
    codeItem->tries[triesSize].insnCount = insnCount;
    codeItem->tries[triesSize].handlerOff = triesSize;
    handlers->list[triesSize].size = 0;
    handlers->list[triesSize].catchAllAddr = 0;
    df->codeItemsLen += 16;
    DFL_parseExceptionPair(df, &handlers->list[triesSize], typeIdx, handlerAddr);
    if(recursive == DFL_TRUE) DFL_parseExceptionTry(df, codeItem, startAddr, insnCount, typeIdx, handlerAddr);
    codeItem->triesSize = triesSize + 1;
    return DFL_TRUE;   
}

DFL_bool DFL_parseExceptionTry(DexFormat* df, DF_DATA_Code* codeItem, DF_u4 startAddr, DF_u2 insnCount, DF_sleb128 typeIdx, DF_uleb128 handlerAddr) {
    //DF_DATA_Try* tries = codeItem->tries;
    //DF_DATA_TypeAddrPair* handlers = codeItem->handlers.handlers;
    // check overlap
    DF_u4 i;
    DF_u4 endAddr = startAddr + insnCount;
    DFL_bool processed = DFL_FALSE;
    DF_u4 triesSize = codeItem->triesSize;
    for(i = 0 ; i < triesSize ; i++) {
        DF_DATA_Try* try = &codeItem->tries[i];
        DF_u4 tryStartAddr = try->startAddr;
        DF_u2 tryInsnCount = try->insnCount;
        DF_u4 tryEndAddr = tryStartAddr + tryInsnCount;
        DF_u2 tryHandlerOff = try->handlerOff;
        DF_DATA_Handler* tryHandler = &codeItem->handlers.list[tryHandlerOff];
        if(tryStartAddr == startAddr && tryInsnCount == insnCount) {
            DFL_parseExceptionPair(df, tryHandler, typeIdx, handlerAddr);
            return DFL_TRUE;
        }else if((tryStartAddr >= startAddr && tryStartAddr < endAddr)) {
            if(startAddr != tryStartAddr) {
                DFL_parseExceptionHandler(df, codeItem, startAddr, (tryStartAddr - startAddr), typeIdx, handlerAddr, DFL_TRUE);
            }
            if(tryEndAddr < endAddr) {
                DFL_parseExceptionHandler(df, codeItem, tryEndAddr, endAddr - tryEndAddr, typeIdx, handlerAddr, DFL_TRUE);
            }else if(tryEndAddr > endAddr) {
                int j;
                DF_DATA_TypeAddrPair* tryPairs = tryHandler->pairs;
                try->insnCount = (endAddr - tryStartAddr);
                DFL_parseExceptionHandler(df, codeItem, endAddr, tryEndAddr - endAddr, -1, tryHandler->catchAllAddr, DFL_FALSE);
                DF_DATA_Handler* handler = &codeItem->handlers.list[codeItem->triesSize - 1];
                for(j = 0 ; j < handler->size ; j++) {
                    DFL_parseExceptionPair(df, handler, tryPairs[j].typeIdx, tryPairs[j].addr);
                }
            }
            DFL_parseExceptionPair(df, tryHandler, typeIdx, handlerAddr);
            return DFL_TRUE;
        }else if((startAddr > tryStartAddr && startAddr < tryEndAddr)) {
            DF_DATA_Handler* handler;
            try->insnCount = (startAddr - tryStartAddr);
            if(tryEndAddr <= endAddr) {
                int j;
                DF_DATA_TypeAddrPair* tryPairs = tryHandler->pairs;
                try->insnCount = (endAddr - tryStartAddr);
                DFL_parseExceptionHandler(df, codeItem, startAddr, tryEndAddr - startAddr, -1, tryHandler->catchAllAddr, DFL_FALSE);
                handler = &codeItem->handlers.list[codeItem->triesSize - 1];
                for(j = 0 ; j < handler->size ; j++) {
                    DFL_parseExceptionPair(df, handler, tryPairs[j].typeIdx, tryPairs[j].addr);
                }
                if(tryEndAddr != endAddr) DFL_parseExceptionHandler(df, codeItem, tryEndAddr, endAddr - tryEndAddr, typeIdx, handlerAddr, DFL_TRUE);
            }else if(tryEndAddr > endAddr) {
                int j;
                DF_DATA_TypeAddrPair* tryPairs = tryHandler->pairs;
                DFL_parseExceptionHandler(df, codeItem, startAddr, insnCount, -1, tryHandler->catchAllAddr, DFL_FALSE);
                
                handler = &codeItem->handlers.list[codeItem->triesSize - 1];
                DFL_parseExceptionHandler(df, codeItem, endAddr, tryEndAddr - endAddr, -1, tryHandler->catchAllAddr, DFL_FALSE);
                DF_DATA_Handler* nextHandler = &codeItem->handlers.list[codeItem->triesSize - 1];
                for(j = 0 ; j < handler->size ; j++) {
                    DFL_parseExceptionPair(df, handler, tryPairs[j].typeIdx, tryPairs[j].addr);
                    DFL_parseExceptionPair(df, nextHandler, tryPairs[j].typeIdx, tryPairs[j].addr);
                }
            }
            DFL_parseExceptionPair(df, handler, typeIdx, handlerAddr);
            return DFL_TRUE;
        }
    }
    return DFL_FALSE;
}

DFL_bool DFL_sortTries(DexFormat* df, DF_DATA_Code* codeItem) {
    DF_u4 triesSize = codeItem->triesSize;
    DF_s4 i, j;
    for(i = 0 ; i < (DF_s4)triesSize ; i++) {
        for(j = i + 1 ; j < (DF_s4)triesSize ; j++) {
            if(codeItem->tries[i].startAddr > codeItem->tries[j].startAddr) {
                DF_u4 tempStartAddr = codeItem->tries[j].startAddr;
                DF_u2 tempInsnCount = codeItem->tries[j].insnCount;
                DF_u2 tempHandlerOff = codeItem->tries[j].handlerOff;

                codeItem->tries[j].startAddr = codeItem->tries[i].startAddr;
                codeItem->tries[j].insnCount = codeItem->tries[i].insnCount;
                codeItem->tries[j].handlerOff = codeItem->tries[i].handlerOff;

                codeItem->tries[i].startAddr = tempStartAddr;
                codeItem->tries[i].insnCount = tempInsnCount;
                codeItem->tries[i].handlerOff = tempHandlerOff;
            }
        }
    }
    return DFL_TRUE;
}
DFL_bool DFL_parseExceptionTable(ClassFile* cf, DexFormat* df, CF_ATTRIBUTE_Code* codeAttr, DF_DATA_Code* codeItem) {
    DF_u2 length = codeAttr->exceptionTableLength;
    CF_ATTRIBUTE_ExceptionTable* exceptionTable = codeAttr->exceptionTable;
    DF_u4 triesSize = 0;
    codeItem->triesSize = 0;
    if(length == 0) {
        return DFL_TRUE;
    }
    DF_u4 i;
    DF_IR_BB** table = codeItem->cfg;
    for(i = 0 ; i < length ; i++) {
        CF_ATTRIBUTE_ExceptionTable* exception = &exceptionTable[i];
        DF_u4 startAddr = table[exception->startPc]->dPc;
        DF_u2 insnCount = table[exception->endPc]->dPc - startAddr;
        DF_uleb128 handlerAddr = table[exception->handlerPc]->dPc;
        DF_sleb128 typeIdx = exception->catchType;
        if(insnCount == 0) continue;
        if(typeIdx == 0) typeIdx = -1;
        else {
            DF_u2 utf8Idx = cf->cp[typeIdx].type.classInfo.nameIndex;
            CF_CONSTANT_Utf8Info classNameUtf8 = cf->cp[utf8Idx].type.utf8Info;
            typeIdx = DFL_parseClassData(df, classNameUtf8);
        }
        if(DFL_parseExceptionTry(df, codeItem, startAddr, insnCount, typeIdx, handlerAddr) == DFL_FALSE) {
            DFL_parseExceptionHandler(df, codeItem, startAddr, insnCount, typeIdx, handlerAddr, DFL_FALSE);
        }
    }
    DFL_sortTries(df, codeItem);
    return DFL_TRUE;
}

DF_sleb128 DFL_parseCodeAttribute(ClassFile* cf, DexFormat* df, DF_u4 idx) {
    CF_Method method = cf->methods[idx];
    CF_Attribute* attributes = method.attributes;
    DF_u4 i, attributesCount = method.attributesCount;
    CF_ATTRIBUTE_Code codeAttr = method.codeAttr;
    DF_DATA_Code* codeItem = df->codeItems + idx;
#if 0
    CF_ATTRIBUTE_Code codeAttr = {0, 0, 0, NULL, 0, NULL, 0, NULL};
    DF_DATA_Code* codeItem = NULL;
    for(i = 0 ; i < attributesCount ; i++) {
        CF_u1* attrName = cf->cp[attributes[i].attributeNameIndex].type.utf8Info.bytes;
        if(strcmp((const char*)attrName, "Code") == 0) {
            codeAttr = attributes[i].type.codeAttr;
            codeItem = df->codeItems + idx;
            break;
        }
    }
#endif
    if(codeAttr.code == NULL) {
        return -1; 
    }
    df->codeItemsSize++;
    codeItem->methodIdx = idx;
    // registers size
    {
        //codeItem->registersSize = codeAttr.maxStack + codeAttr.maxLocals;
    }
    // ins size
    {
        DF_u2 protoIdx = DFL_parseProtoData(df, cf->cp[method.descriptorIndex].type.utf8Info);
        DF_u4 shortyIdx = df->protoIds[protoIdx].shortyIdx;
        DF_u4 shortySize = df->stringItems[shortyIdx].size;
        DF_u1* shortyStr = df->stringItems[shortyIdx].data;
        for(i = 1 ; i < shortySize ; i++) {
            if(shortyStr[i] == 'D'
                || shortyStr[i] == 'J') {
                (codeItem->insSize) += 2;
            }else (codeItem->insSize)++;
        }
        if((method.accessFlags & CF_ACC_STATIC) != 0) {
            codeItem->insCount = df->stringItems[shortyIdx].size - 1;
        }else {
            codeItem->insCount = df->stringItems[shortyIdx].size;
            codeItem->insSize++;
        }
    }
    // out size
    {
        codeItem->outsSize = 0;   
    }
    
    // debug info off
    {
        // DO NOTHING
    }
    // insns size
    {
        codeItem->insnsSize = 0;
    }
    // insns[]
    {
        DFL_parseBytecode(cf, df, &codeAttr, codeItem);
    }
    // tries size
    // tries & handlers
    {
        DFL_parseExceptionTable(cf, df, &codeAttr, codeItem); 
    }
    df->codeItemsLen += (16 + codeItem->insnsSize * 2 + 4);
    if(codeItem->triesSize != 0 && (df->codeItemsLen % 2) != 0) df->codeItemsLen += 2;
    // TODO: tries & handlers length
    return idx;
}

DFL_bool DFL_parseMethodItems(ClassFile* cf, DexFormat* df) {
    DF_DATA_Class* classItems = df->classItems;
    
    DF_u2 utf8Idx = cf->cp[cf->thisClass].type.classInfo.nameIndex;
    CF_CONSTANT_Utf8Info classUtf8 = cf->cp[utf8Idx].type.utf8Info;
    
    DF_u2 directMethodsSize = 0;
    DF_u4 directIdx = 0, virtualIdx = 0;
    DF_s4 i, j;
    for(i = 0 ; i < cf->methodsCount ; i++) {
        CF_Method method = cf->methods[i];
        CF_u1* name = cf->cp[method.nameIndex].type.utf8Info.bytes;
        if((method.accessFlags & CF_ACC_STATIC) != 0 
                || (method.accessFlags & CF_ACC_PRIVATE) != 0 
                || name[0] == '<') {
            directMethodsSize++;
        }
    }
    // Direct Method
    classItems->directMethodsSize = directMethodsSize;
    classItems->directMethods = (DF_DATA_Method*)allocateMem(df->mm, sizeof(DF_DATA_Method) * classItems->directMethodsSize);
    df->classItemsLen += getUleb128Size(classItems->directMethodsSize);
    // Virtual Method
    classItems->virtualMethodsSize = cf->methodsCount - directMethodsSize;
    classItems->virtualMethods = (DF_DATA_Method*)allocateMem(df->mm, sizeof(DF_DATA_Method) * classItems->virtualMethodsSize);
    df->classItemsLen += getUleb128Size(classItems->virtualMethodsSize);
    // DATA
    df->codeItems = (DF_DATA_Code*)allocateMem(df->mm, sizeof(DF_DATA_Code) * cf->methodsCount); // TODO
    for(i = 0 ; i < cf->methodsCount ; i++) {
        CF_Method cfMethod = cf->methods[i];
        DF_uleb128 cfAccessFlags = (DF_uleb128)cfMethod.accessFlags;
        CF_CONSTANT_Utf8Info nameUtf8 = cf->cp[cfMethod.nameIndex].type.utf8Info;
        CF_CONSTANT_Utf8Info typeUtf8 = cf->cp[cfMethod.descriptorIndex].type.utf8Info;
        CF_u1* name = nameUtf8.bytes;
        DF_DATA_Method* dfMethod;
        if((cfAccessFlags & CF_ACC_STATIC) != 0 
                || (cfMethod.accessFlags & CF_ACC_PRIVATE) != 0 
                || name[0] == '<') {
            dfMethod = &classItems->directMethods[directIdx];
            if(name[0] == '<') {
                cfAccessFlags = (cfAccessFlags | DF_ACC_CONSTRUCTOR);
            }
            directIdx++;
        }else {
            dfMethod = &classItems->virtualMethods[virtualIdx];
            virtualIdx++;
        }
        dfMethod->idxDiff = DFL_parseMethodData(df, classUtf8, nameUtf8, typeUtf8);
        dfMethod->accessFlags = cfAccessFlags;
        dfMethod->codeOff = DFL_parseCodeAttribute(cf, df, i);
        df->classItemsLen += (8 + getUleb128Size(dfMethod->accessFlags) + getUleb128Size(dfMethod->idxDiff));
    }
    /*
    DF_uleb128 prevIdx = 0;
    for(i = 0 ; i < (DF_s4)classItems->directMethodsSize; i++) {
        DF_DATA_Method* dfMethod = &classItems->directMethods[i];
        for(j = i + 1 ; j < (DF_s4)classItems->directMethodsSize ; j++) {
            if(classItems->directMethods[i].idxDiff > classItems->directMethods[j].idxDiff) {
                DF_uleb128 idxDiff = dfMethod->idxDiff;
                DF_uleb128 accessFlags = dfMethod->accessFlags;
                DF_sleb128 codeOff = dfMethod->codeOff;

                dfMethod->idxDiff = classItems->directMethods[j].idxDiff;
                dfMethod->accessFlags = classItems->directMethods[j].accessFlags;
                dfMethod->codeOff = classItems->directMethods[j].codeOff;

                classItems->directMethods[j].idxDiff = idxDiff;
                classItems->directMethods[j].accessFlags = accessFlags;
                classItems->directMethods[j].codeOff = codeOff;
            }
        }
        dfMethod->idxDiff = dfMethod->idxDiff - prevIdx;
        prevIdx = dfMethod->idxDiff + prevIdx;
        df->classItemsLen += (8 + getUleb128Size(dfMethod->accessFlags) + getUleb128Size(dfMethod->idxDiff));
    }
    prevIdx = 0;
    for(i = 0 ; i < (DF_s4)classItems->virtualMethodsSize; i++) {
        DF_DATA_Method* dfMethod = &classItems->virtualMethods[i];
        for(j = i + 1 ; j < (DF_s4)classItems->virtualMethodsSize ; j++) {
            if(classItems->virtualMethods[i].idxDiff > classItems->virtualMethods[j].idxDiff) {
                DF_uleb128 idxDiff = dfMethod->idxDiff;
                DF_uleb128 accessFlags = dfMethod->accessFlags;
                DF_sleb128 codeOff = dfMethod->codeOff;

                dfMethod->idxDiff = classItems->virtualMethods[j].idxDiff;
                dfMethod->accessFlags = classItems->virtualMethods[j].accessFlags;
                dfMethod->codeOff = classItems->virtualMethods[j].codeOff;

                classItems->virtualMethods[j].idxDiff = idxDiff;
                classItems->virtualMethods[j].accessFlags = accessFlags;
                classItems->virtualMethods[j].codeOff = codeOff;
            }
        }
        dfMethod->idxDiff = dfMethod->idxDiff - prevIdx;
        prevIdx = dfMethod->idxDiff + prevIdx;
        df->classItemsLen += (8 + getUleb128Size(dfMethod->accessFlags) + getUleb128Size(dfMethod->idxDiff));
    }
    */
    return DFL_TRUE;
}

DFL_bool DFL_sortClassDef(ClassFile* cf, DexFormat* df) {
    DF_DATA_Class* classItems = df->classItems;
    DF_u2 directMethodsSize = classItems->directMethodsSize;
    DF_u2 virtualMethodsSize = classItems->virtualMethodsSize;
    DF_u2 staticFieldsSize = classItems->staticFieldsSize;
    DF_u2 instanceFieldsSize = classItems->instanceFieldsSize;

    DF_s4 i,j;
    DF_uleb128 prevIdx = 0;
    for(i = 0 ; i < (DF_s4)directMethodsSize; i++) {
        DF_DATA_Method* dfMethod = &classItems->directMethods[i];
        for(j = i + 1 ; j < (DF_s4)directMethodsSize ; j++) {
            if(classItems->directMethods[i].idxDiff > classItems->directMethods[j].idxDiff) {
                DF_uleb128 idxDiff = dfMethod->idxDiff;
                DF_uleb128 accessFlags = dfMethod->accessFlags;
                DF_sleb128 codeOff = dfMethod->codeOff;

                dfMethod->idxDiff = classItems->directMethods[j].idxDiff;
                dfMethod->accessFlags = classItems->directMethods[j].accessFlags;
                dfMethod->codeOff = classItems->directMethods[j].codeOff;

                classItems->directMethods[j].idxDiff = idxDiff;
                classItems->directMethods[j].accessFlags = accessFlags;
                classItems->directMethods[j].codeOff = codeOff;
            }
        }
        dfMethod->idxDiff = dfMethod->idxDiff - prevIdx;
        prevIdx = dfMethod->idxDiff + prevIdx;
    }
    prevIdx = 0;
    for(i = 0 ; i < (DF_s4)virtualMethodsSize; i++) {
        DF_DATA_Method* dfMethod = &classItems->virtualMethods[i];
        for(j = i + 1 ; j < (DF_s4)virtualMethodsSize ; j++) {
            if(classItems->virtualMethods[i].idxDiff > classItems->virtualMethods[j].idxDiff) {
                DF_uleb128 idxDiff = dfMethod->idxDiff;
                DF_uleb128 accessFlags = dfMethod->accessFlags;
                DF_sleb128 codeOff = dfMethod->codeOff;

                dfMethod->idxDiff = classItems->virtualMethods[j].idxDiff;
                dfMethod->accessFlags = classItems->virtualMethods[j].accessFlags;
                dfMethod->codeOff = classItems->virtualMethods[j].codeOff;

                classItems->virtualMethods[j].idxDiff = idxDiff;
                classItems->virtualMethods[j].accessFlags = accessFlags;
                classItems->virtualMethods[j].codeOff = codeOff;
            }
        }
        dfMethod->idxDiff = dfMethod->idxDiff - prevIdx;
        prevIdx = dfMethod->idxDiff + prevIdx;
    }
    prevIdx = 0;
    for(i = 0 ; i < (DF_s4)staticFieldsSize; i++) {
        DF_DATA_Field* dfField = &classItems->staticFields[i];
        for(j = i + 1 ; j < (DF_s4)staticFieldsSize ; j++) {
            if(classItems->staticFields[i].idxDiff > classItems->staticFields[j].idxDiff) {
                DF_uleb128 idxDiff = dfField->idxDiff;
                DF_uleb128 accessFlags = dfField->accessFlags;

                dfField->idxDiff = classItems->staticFields[j].idxDiff;
                dfField->accessFlags = classItems->staticFields[j].accessFlags;

                classItems->staticFields[j].idxDiff = idxDiff;
                classItems->staticFields[j].accessFlags = accessFlags;
            }
        }
        dfField->idxDiff = dfField->idxDiff - prevIdx;
        prevIdx = dfField->idxDiff + prevIdx;
        df->classItemsLen += (getUleb128Size(dfField->accessFlags) + getUleb128Size(dfField->idxDiff));
    }
    prevIdx = 0;
    for(i = 0 ; i < (DF_s4)instanceFieldsSize; i++) {
        DF_DATA_Field* dfField = &classItems->instanceFields[i];
        for(j = i + 1 ; j < (DF_s4)instanceFieldsSize ; j++) {
            if(classItems->instanceFields[i].idxDiff > classItems->instanceFields[j].idxDiff) {
                DF_uleb128 idxDiff = dfField->idxDiff;
                DF_uleb128 accessFlags = dfField->accessFlags;

                dfField->idxDiff = classItems->instanceFields[j].idxDiff;
                dfField->accessFlags = classItems->instanceFields[j].accessFlags;

                classItems->instanceFields[j].idxDiff = idxDiff;
                classItems->instanceFields[j].accessFlags = accessFlags;
            }
        }
        dfField->idxDiff = dfField->idxDiff - prevIdx;
        prevIdx = dfField->idxDiff + prevIdx;
        df->classItemsLen += (getUleb128Size(dfField->accessFlags) + getUleb128Size(dfField->idxDiff));
    }
    return DFL_TRUE;
}

DFL_bool DFL_parseClassDef(ClassFile* cf, DexFormat* df) {
    DF_u4 i;
    DF_DEF_Class* clDef = (DF_DEF_Class*)allocateMem(df->mm, sizeof(DF_DEF_Class));
    df->classDefs = clDef;
    df->classDefsSize = 1; // TODO
    // Class Idx
    {
        DF_u2 utf8Idx = cf->cp[cf->thisClass].type.classInfo.nameIndex;
        CF_CONSTANT_Utf8Info classUtf8 = cf->cp[utf8Idx].type.utf8Info;
        clDef->classIdx  = DFL_parseClassData(df, classUtf8);
    }
    // Access Flags
    {
        DF_u2 cfAccessFlags = cf->accessFlags;
        clDef->accessFlags = cfAccessFlags & ~(CF_ACC_SUPER);

    }
    // superclassIdx
    {
        DF_u2 utf8Idx = cf->cp[cf->superClass].type.classInfo.nameIndex;
        CF_CONSTANT_Utf8Info classNameUtf8 = cf->cp[utf8Idx].type.utf8Info;
        clDef->superClassIdx = DFL_parseClassData(df, classNameUtf8);
    }
    // interfacesOff
    {
        // DO NOTHING
    }
    // sourceFileIdx
    {
        // DO NOTHING
    }
    // annotationOff
    {
        // DO NOTHING
    } 
    // classData
    {
        df->classItems = (DF_DATA_Class*)allocateMem(df->mm, sizeof(DF_DATA_Class));
        df->classItemsSize = 1; // TODO
        DFL_parseFieldItems(cf, df);
        DFL_parseMethodItems(cf, df);
    }
    // staticValueOff;
    {
        // TODO 
    }
    return DFL_TRUE;
}

DFL_bool DFL_sortStringIds(DexFormat* df, DFL_SortTable* sortTable) {
    DF_u4 stringItemsSize = df->stringItemsSize;
    DF_DATA_String* strings = df->stringItems;
    DF_u4* stringTable = (DF_u4*)allocateMem(df->mm, sizeof(DF_u4*) * stringItemsSize);

    DF_s4 i, j;
    for(i = 0 ; i < (DF_s4)stringItemsSize ; i++) stringTable[i] = i;
    for(i = 0 ; i < (DF_s4)stringItemsSize - 1 ; i++) {
        for(j = i + 1 ; j < (DF_s4)stringItemsSize ; j++) {
            int cmp = strncmp((const char*)strings[i].data, (const char*)strings[j].data, (size_t)(strings[i].size < strings[j].size ? strings[j].size : strings[i].size));
            if(cmp > 0) {
                // swap
                DF_u4 size = strings[i].size;
                DF_u1* data= strings[i].data;
                DF_u4 temp = stringTable[i];
                stringTable[i] = stringTable[j];
                stringTable[j] = temp; 
                strings[i].size = strings[j].size;
                strings[i].data= strings[j].data;
                strings[j].size = size;
                strings[j].data = data;
            }
        }
    }
    sortTable->stringTable = (DF_u4*)allocateMem(df->mm, sizeof(DF_u4*) * stringItemsSize);
    for(i = 0 ; i < (DF_s4)stringItemsSize ; i++) {
        sortTable->stringTable[stringTable[i]] = i;
    }
    freeMem(df->mm, stringTable);
    return DFL_TRUE;
}
DFL_bool DFL_sortTypeIds(DexFormat* df, DFL_SortTable* sortTable) {
    DF_u4 typeIdsSize = df->typeIdsSize;
    DF_ID_Type* types = df->typeIds;
    DF_u4* typeTable = (DF_u4*)allocateMem(df->mm, sizeof(DF_u4*) * typeIdsSize);

    DF_s4 i, j;
    for(i = 0 ; i < (DF_s4)typeIdsSize ; i++) {
        types[i].descriptorIdx = sortTable->stringTable[types[i].descriptorIdx];
        typeTable[i] = i;
    }
    for(i = 0 ; i < (DF_s4)typeIdsSize - 1 ; i++) {
        for(j = i + 1 ;  j < (DF_s4)typeIdsSize ; j++) {
            if(types[i].descriptorIdx > types[j].descriptorIdx) {
                DF_u4 temp = typeTable[i];
                typeTable[i] = typeTable[j];
                typeTable[j] = temp;
                temp = types[i].descriptorIdx;
                types[i].descriptorIdx = types[j].descriptorIdx;
                types[j].descriptorIdx = temp;
            }
        }
    }
    sortTable->typeTable = (DF_u4*)allocateMem(df->mm, sizeof(DF_u4*) * typeIdsSize);
    for(i = 0 ; i < (DF_s4)typeIdsSize ; i++) {
        sortTable->typeTable[typeTable[i]] = i;
    }
    freeMem(df->mm, typeTable);

    return DFL_TRUE;
}
DFL_bool DFL_sortTypeList(DexFormat* df, DFL_SortTable* sortTable) {
    DF_u4 typeListSize = df->typeListSize;
    DF_DATA_TypeList* typeList= df->typeList;
    DF_u4* typeListTable = (DF_u4*)allocateMem(df->mm, sizeof(DF_u4*) * typeListSize);
    DF_s4 i, j, k;
    for(i = 0 ; i < (DF_s4)typeListSize ; i++) {
        DF_DATA_Type* typeItems = typeList[i].typeItems;
        DF_u4 typeItemSize = typeList[i].size;
        for(j = 0 ; j < (DF_s4)typeItemSize ; j++) {
            typeItems[j].typeIdx = sortTable->typeTable[typeItems[j].typeIdx];
        }
        typeListTable[i] = i;
    }
    for(i = 0 ; i < (DF_s4)typeListSize - 1 ; i++) {
        DF_DATA_Type* typeItem1 = typeList[i].typeItems;
        DF_u4 typeItemSize1 = typeList[i].size;
        for(j = i + 1 ; j < (DF_s4)typeListSize ; j++) {
            DF_DATA_Type* typeItem2 = typeList[j].typeItems;
            DF_u4 typeItemSize2 = typeList[j].size;
            DF_u4 typeItemSize = typeItemSize1 > typeItemSize2 ? typeItemSize2:typeItemSize1;
            
            for(k = 0 ; k < (DF_s4)typeItemSize ; k++) {
                if(typeItem1[k].typeIdx < typeItem2[k].typeIdx) {
                    break;
                }else if(typeItem1[k].typeIdx == typeItem2[k].typeIdx) {
                    continue;
                }else {
                    // Swap
                    DF_u4 temp;
                    typeList[j].size = typeItemSize1;
                    typeList[j].typeItems = typeItem1;
                    typeList[i].size = typeItemSize2;
                    typeList[i].typeItems = typeItem2;
                    
                    temp = typeListTable[i];
                    typeListTable[i] = typeListTable[j];
                    typeListTable[j] = temp;

                    typeItem1 = typeItem2;
                    typeItemSize1 = typeItemSize2;
                }
            }
        }
    }
    sortTable->typeListTable = (DF_u4*)allocateMem(df->mm, sizeof(DF_u4*) * typeListSize);
    for(i = 0 ; i < (DF_s4)typeListSize ; i++) {
        sortTable->typeListTable[typeListTable[i]] = i;
    }
    if(df->interfacesOff != -1) {
        df->interfacesOff = sortTable->typeListTable[df->interfacesOff];
    }

    freeMem(df->mm, typeListTable);

    return DFL_TRUE;
}

DFL_bool DFL_sortFieldIds(DexFormat* df, DFL_SortTable* sortTable) {
    DF_u4 fieldIdsSize = df->fieldIdsSize;
    DF_ID_Field* fields = df->fieldIds;
    DF_u4* fieldTable = (DF_u4*)allocateMem(df->mm, sizeof(DF_u4*) * fieldIdsSize);

    DF_s4 i, j;
    for(i = 0 ; i < (DF_s4)fieldIdsSize ; i++) {
        fields[i].classIdx = sortTable->typeTable[fields[i].classIdx];
        fields[i].nameIdx = sortTable->stringTable[fields[i].nameIdx];
        fields[i].typeIdx = sortTable->typeTable[fields[i].typeIdx];
        fieldTable[i] = i;
    }
    for(i = 0 ; i < (DF_s4)fieldIdsSize - 1 ; i++) {
        DF_u2 cIdx1 = fields[i].classIdx;
        DF_u4 nIdx1 = fields[i].nameIdx; 
        DF_u2 tIdx1 = fields[i].typeIdx; 
        for(j = i + 1 ;  j < (DF_s4)fieldIdsSize ; j++) {
            DF_u2 cIdx2 = fields[j].classIdx;
            DF_u4 nIdx2 = fields[j].nameIdx;
            DF_u2 tIdx2 = fields[j].typeIdx;
            if(cIdx1 > cIdx2) {
                DF_u4 temp;
                fields[i].classIdx = cIdx2;
                fields[i].nameIdx = nIdx2;
                fields[i].typeIdx = tIdx2;
                fields[j].classIdx = cIdx1;
                fields[j].nameIdx = nIdx1;
                fields[j].typeIdx = tIdx1;
                temp = fieldTable[i];
                fieldTable[i] = fieldTable[j];
                fieldTable[j] = temp;
                cIdx1 = cIdx2;
                nIdx1 = nIdx2;
                tIdx1 = tIdx2;
            }else if(cIdx1 == cIdx2) {
                if(nIdx1 > nIdx2) {
                    DF_u4 temp;
                    fields[i].classIdx = cIdx2;
                    fields[i].nameIdx = nIdx2;
                    fields[i].typeIdx = tIdx2;
                    fields[j].classIdx = cIdx1;
                    fields[j].nameIdx = nIdx1;
                    fields[j].typeIdx = tIdx1;
                    temp = fieldTable[i];
                    fieldTable[i] = fieldTable[j];
                    fieldTable[j] = temp;
                    cIdx1 = cIdx2;
                    nIdx1 = nIdx2;
                    tIdx1 = tIdx2;
                }else if(nIdx1 == nIdx2) {
                    if(tIdx1 > tIdx2) {
                        DF_u4 temp;
                        fields[i].classIdx = cIdx2;
                        fields[i].nameIdx = nIdx2;
                        fields[i].typeIdx = tIdx2;
                        fields[j].classIdx = cIdx1;
                        fields[j].nameIdx = nIdx1;
                        fields[j].typeIdx = tIdx1;
                        temp = fieldTable[i];
                        fieldTable[i] = fieldTable[j];
                        fieldTable[j] = temp;
                        cIdx1 = cIdx2;
                        nIdx1 = nIdx2;
                        tIdx1 = tIdx2;
                    }
                }
            }
        }
    }
    sortTable->fieldTable = (DF_u4*)allocateMem(df->mm, sizeof(DF_u4*) * fieldIdsSize);
    for(i = 0 ; i < (DF_s4)fieldIdsSize ; i++) {
        sortTable->fieldTable[fieldTable[i]] = i;
    }
    freeMem(df->mm, fieldTable);
    return DFL_TRUE;
}
DFL_bool DFL_sortProtoIds(DexFormat* df, DFL_SortTable* sortTable) {
    DF_s4 i, j ;
    DF_u4 protoIdsSize = df->protoIdsSize;
    DF_ID_Proto* protos = df->protoIds;
    DF_u4* protoTable = (DF_u4*)allocateMem(df->mm, sizeof(DF_u4*) * protoIdsSize);
    for(i = 0 ; i < (DF_s4)protoIdsSize ; i++) {
        protos[i].shortyIdx = sortTable->stringTable[protos[i].shortyIdx];
        protos[i].returnTypeIdx = sortTable->typeTable[protos[i].returnTypeIdx];
        if(protos[i].parametersOff != -1) {
            protos[i].parametersOff = sortTable->typeListTable[protos[i].parametersOff];
        }
        protoTable[i] = i;
    }
    for(i = 0 ; i < (DF_s4)protoIdsSize - 1 ; i++) {
        DF_u4 rIdx1 = protos[i].returnTypeIdx;
        DF_s8 pIdx1 = protos[i].parametersOff; 
        for(j = i + 1 ;  j < (DF_s4)protoIdsSize ; j++) {
            DF_u4 rIdx2 = protos[j].returnTypeIdx;
            DF_s8 pIdx2 = protos[j].parametersOff; 
            if(rIdx1 > rIdx2) {
                DF_u4 temp;
                protos[i].returnTypeIdx = rIdx2;
                protos[i].parametersOff = pIdx2;
                protos[j].returnTypeIdx = rIdx1;
                protos[j].parametersOff = pIdx1;
                temp = protoTable[i];
                protoTable[i] = protoTable[j];
                protoTable[j] = temp;
                rIdx1 = rIdx2; 
                pIdx1 = pIdx2; 
            }else if(rIdx1 == rIdx2) {
                if(pIdx1 > pIdx2) {
                    DF_u4 temp;
                    protos[i].returnTypeIdx = rIdx2;
                    protos[i].parametersOff = pIdx2;
                    protos[j].returnTypeIdx = rIdx1;
                    protos[j].parametersOff = pIdx1;
                    temp = protoTable[i];
                    protoTable[i] = protoTable[j];
                    protoTable[j] = temp;
                    rIdx1 = rIdx2; 
                    pIdx1 = pIdx2; 
                }
            }
        }
    }
    sortTable->protoTable = (DF_u4*)allocateMem(df->mm, sizeof(DF_u4*) * protoIdsSize);
    for(i = 0 ; i < (DF_s4)protoIdsSize ; i++) {
        sortTable->protoTable[protoTable[i]] = i;
    }
    freeMem(df->mm, protoTable);

    return DFL_TRUE;
}
DFL_bool DFL_sortMethodIds(DexFormat* df, DFL_SortTable* sortTable) {
    DF_u4 methodIdsSize = df->methodIdsSize;
    DF_ID_Method* methods = df->methodIds;
    DF_u4* methodTable = (DF_u4*)allocateMem(df->mm, sizeof(DF_u4*) * methodIdsSize);

    DF_s4 i, j;
    for(i = 0 ; i < (DF_s4)methodIdsSize ; i++) {
        methods[i].classIdx = sortTable->typeTable[methods[i].classIdx];
        methods[i].nameIdx = sortTable->stringTable[methods[i].nameIdx];
        methods[i].protoIdx = sortTable->protoTable[methods[i].protoIdx];
        methodTable[i] = i;
    }
    for(i = 0 ; i < (DF_s4)methodIdsSize - 1 ; i++) {
        DF_u2 cIdx1 = methods[i].classIdx;
        DF_u4 nIdx1 = methods[i].nameIdx; 
        DF_u2 pIdx1 = methods[i].protoIdx; 
        for(j = i + 1 ;  j < (DF_s4)methodIdsSize ; j++) {
            DF_u2 cIdx2 = methods[j].classIdx;
            DF_u4 nIdx2 = methods[j].nameIdx;
            DF_u2 pIdx2 = methods[j].protoIdx;
            if(cIdx1 > cIdx2) {
                DF_u4 temp;
                methods[i].classIdx = cIdx2;
                methods[i].nameIdx = nIdx2;
                methods[i].protoIdx = pIdx2;

                methods[j].classIdx = cIdx1;
                methods[j].nameIdx = nIdx1;
                methods[j].protoIdx = pIdx1;
                
                temp = methodTable[i];
                methodTable[i] = methodTable[j];
                methodTable[j] = temp;
                
                cIdx1 = cIdx2;
                nIdx1 = nIdx2;
                pIdx1 = pIdx2;
            }else if(cIdx1 == cIdx2) {
                if(nIdx1 > nIdx2) {
                    DF_u4 temp;
                    methods[i].classIdx = cIdx2;
                    methods[i].nameIdx = nIdx2;
                    methods[i].protoIdx = pIdx2;
                    methods[j].classIdx = cIdx1;
                    methods[j].nameIdx = nIdx1;
                    methods[j].protoIdx = pIdx1;
                    
                    temp = methodTable[i];
                    methodTable[i] = methodTable[j];
                    methodTable[j] = temp;
                    
                    cIdx1 = cIdx2;
                    nIdx1 = nIdx2;
                    pIdx1 = pIdx2;
                }else if(nIdx1 == nIdx2) {
                    if(pIdx1 > pIdx2) {
                        DF_u4 temp;
                        methods[i].classIdx = cIdx2;
                        methods[i].nameIdx = nIdx2;
                        methods[i].protoIdx = pIdx2;
                        methods[j].classIdx = cIdx1;
                        methods[j].nameIdx = nIdx1;
                        methods[j].protoIdx = pIdx1;
                        
                        temp = methodTable[i];
                        methodTable[i] = methodTable[j];
                        methodTable[j] = temp;
                        
                        cIdx1 = cIdx2;
                        nIdx1 = nIdx2;
                        pIdx1 = pIdx2;
                    }
                }
            }
        }
    }
    sortTable->methodTable = (DF_u4*)allocateMem(df->mm, sizeof(DF_u4*) * methodIdsSize);
    for(i = 0 ; i < (DF_s4)methodIdsSize ; i++) {
        sortTable->methodTable[methodTable[i]] = i;
    }
    freeMem(df->mm, methodTable);

    return DFL_TRUE;
}

DFL_bool DFL_sortIds(DexFormat* df) {
    DFL_SortTable* sortTable = (DFL_SortTable*)allocateMem(df->mm, sizeof(DFL_SortTable));
    DFL_sortStringIds(df, sortTable);
    DFL_sortTypeIds(df, sortTable);
    DFL_sortTypeList(df, sortTable);
    DFL_sortFieldIds(df, sortTable);
    DFL_sortProtoIds(df, sortTable);
    DFL_sortMethodIds(df, sortTable);
    freeMem(df->mm, sortTable->stringTable);
    freeMem(df->mm, sortTable->typeTable);
    freeMem(df->mm, sortTable->typeListTable);
    freeMem(df->mm, sortTable->protoTable);
    freeMem(df->mm, sortTable->fieldTable);
    freeMem(df->mm, sortTable->methodTable);
    freeMem(df->mm, sortTable);
    return DFL_TRUE;
}
DFL_bool DFL_parseClassFile(ClassFile* cf, DexFormat* df) {
    if(!DFL_parseConstantPool(cf, df)) return DFL_FALSE;
    if(!DFL_parseAdditionalConstant(cf, df)) return DFL_FALSE;
    if(!DFL_sortIds(df)) return DFL_FALSE;
    if(!DFL_parseClassDef(cf, df)) return DFL_FALSE;
    if(!DFL_sortClassDef(cf, df)) return DFL_FALSE;
    return DFL_TRUE;
}

DF_u4 DFL_findDataOffset(DexFormat* df) {
    DF_u4 size = 0;
    // header size
    size += 0x70;
    // string ids
    size += (df->stringItemsSize * 4);
    // type ids
    size += (df->typeIdsSize * 4);
    // proto ids
    size += (df->protoIdsSize * 12);
    // field ids
    size += (df->fieldIdsSize * 8);
    // methods ids
    size += (df->methodIdsSize * 8);
    // class defs
    size += 32;

    return size;
}

DF_u4 DFL_writeCodeItems(DexFormat* df, DF_u4 offset) {
    DF_s4 i, j, k;
    DF_DATA_Class* classItems = df->classItems;
    DF_u4 triesOff, handlerOff;
    DF_align4bytes(offset);
    df->codeItemsOff = offset;
    // direct method
    for(i = 0 ; i < (DF_s4)classItems->directMethodsSize ; i++) {
        DF_s4 codeOffset = classItems->directMethods[i].codeOff;
        if(codeOffset == -1) continue;
        DF_align4bytes(offset);
        classItems->directMethods[i].addr = offset;
        
        DF_DATA_Code* codeItem = df->codeItems + codeOffset;
        DF_writeShortData(df->bytes, offset, codeItem->registersSize); offset += 2;
        DF_writeShortData(df->bytes, offset, codeItem->insSize); offset += 2;
        DF_writeShortData(df->bytes, offset, codeItem->outsSize); offset += 2;
        DF_writeShortData(df->bytes, offset, codeItem->triesSize); offset += 2;
        DF_writeIntData(df->bytes, offset, codeItem->debugInfoOff); offset += 4;
        DF_writeIntData(df->bytes, offset, codeItem->insnsSize); offset += 4;
        for(j = 0 ; j < (DF_s4)codeItem->insnsSize ; j++) {
            DF_writeShortData(df->bytes, offset, codeItem->insns[j]); offset += 2;
        }
        if(codeItem->triesSize == 0) continue;
        DF_align4bytes(offset);
        triesOff = offset;
        // handlers
        offset += (8 * codeItem->triesSize);
        handlerOff = offset;
        offset = DF_writeUleb128Data(df->bytes, offset, codeItem->triesSize); // TODO
        for(j = 0 ; j < (DF_s4)codeItem->triesSize; j++) {
            DF_DATA_Handler* handler = &codeItem->handlers.list[j];
            handler->offset = (offset - handlerOff);
            if(handler->catchAllAddr != 0) {
                offset = DF_writeSleb128Data(df->bytes, offset, -(handler->size));
            }else {
                offset = DF_writeSleb128Data(df->bytes, offset, handler->size);
            }
            DF_DATA_TypeAddrPair* tryPairs = handler->pairs;
            for(k = 0 ; k < handler->size ; k++) {
                offset = DF_writeUleb128Data(df->bytes, offset, tryPairs[k].typeIdx);
                offset = DF_writeUleb128Data(df->bytes, offset, tryPairs[k].addr);
            }
            if(handler->catchAllAddr != 0) {
                offset = DF_writeUleb128Data(df->bytes, offset, handler->catchAllAddr);
            }
        }
        // tries
        for(j = 0 ; j < (DF_s4)codeItem->triesSize ; j++) {
            DF_DATA_Try* try = &codeItem->tries[j];
            DF_writeIntData(df->bytes, triesOff, try->startAddr); triesOff += 4;
            DF_writeShortData(df->bytes, triesOff, try->insnCount); triesOff += 2;
            DF_writeShortData(df->bytes, triesOff, codeItem->handlers.list[try->handlerOff].offset); triesOff += 2;
        }
    }
    // virtual method
    for(i = 0 ; i < (DF_s4)classItems->virtualMethodsSize ; i++) {
        DF_s4 codeOffset = classItems->virtualMethods[i].codeOff;
        if(codeOffset == -1) continue;
        DF_align4bytes(offset);
        classItems->virtualMethods[i].addr = offset;
        
        DF_DATA_Code* codeItem = df->codeItems + codeOffset;
        DF_writeShortData(df->bytes, offset, codeItem->registersSize); offset += 2;
        DF_writeShortData(df->bytes, offset, codeItem->insSize); offset += 2;
        DF_writeShortData(df->bytes, offset, codeItem->outsSize); offset += 2;
        DF_writeShortData(df->bytes, offset, codeItem->triesSize); offset += 2;
        DF_writeIntData(df->bytes, offset, codeItem->debugInfoOff); offset += 4;
        DF_writeIntData(df->bytes, offset, codeItem->insnsSize); offset += 4;
        for(j = 0 ; j < (DF_s4)codeItem->insnsSize ; j++) {
            DF_writeShortData(df->bytes, offset, codeItem->insns[j]); offset += 2;
        }
        if(codeItem->triesSize == 0) continue;
        DF_align4bytes(offset);
        triesOff = offset;
        
        // handlers
        offset += (8 * codeItem->triesSize);
        handlerOff = offset;
        offset = DF_writeUleb128Data(df->bytes, offset, codeItem->triesSize); // TODO
        for(j = 0 ; j < (DF_s4)codeItem->triesSize; j++) {
            DF_DATA_Handler* handler = &codeItem->handlers.list[j];
            handler->offset = (offset - handlerOff);
            if(handler->catchAllAddr != 0) {
                offset = DF_writeSleb128Data(df->bytes, offset, -(handler->size));
            }else {
                offset = DF_writeSleb128Data(df->bytes, offset, handler->size);
            }
            DF_DATA_TypeAddrPair* tryPairs = handler->pairs;
            for(k = 0 ; k < handler->size ; k++) {
                offset = DF_writeUleb128Data(df->bytes, offset, tryPairs[k].typeIdx);
                offset = DF_writeUleb128Data(df->bytes, offset, tryPairs[k].addr);
            }
            if(handler->catchAllAddr != 0) {
                offset = DF_writeUleb128Data(df->bytes, offset, handler->catchAllAddr);
            }
        }
        // tries
        for(j = 0 ; j < (DF_s4)codeItem->triesSize ; j++) {
            DF_DATA_Try* try = &codeItem->tries[j];
            DF_writeIntData(df->bytes, triesOff, try->startAddr); triesOff += 4;
            DF_writeShortData(df->bytes, triesOff, try->insnCount); triesOff += 2;
            DF_writeShortData(df->bytes, triesOff, codeItem->handlers.list[try->handlerOff].offset); triesOff += 2;
        }
    }
    return offset;
}

DF_u4 DFL_writeTypeLists(DexFormat* df, DF_u4 offset) {
    DF_u4 i, j;
    DF_align4bytes(offset);
    df->typeListOff = offset;
    for(i = 0 ; i < df->typeListSize ; i++) {
        DF_DATA_Type* typeItems = df->typeList[i].typeItems;
        DF_align4bytes(offset);
        df->typeList[i].addr = offset;
        DF_writeIntData(df->bytes, offset, df->typeList[i].size); offset += 4;
        for(j = 0 ; j < df->typeList[i].size ; j++) {
            DF_writeShortData(df->bytes, offset, typeItems[j].typeIdx); offset += 2;
        }
    }
    return offset;
}
DF_u4 DFL_writeStringData(DexFormat* df, DF_u4 offset) {
    DF_u4 i, j;
    DF_DATA_String* strings = df->stringItems;
    df->stringItemsOff = offset;
    for(i = 0 ; i < df->stringItemsSize ; i++) {
        DF_uleb128 size = strings[i].size;
        DF_u1* data = strings[i].data;
        strings[i].addr = offset;
        offset = DF_writeUleb128Data(df->bytes, offset, size);
        for(j = 0 ; j < size ; j++) {
            DF_writeByteData(df->bytes, offset, data[j]); offset++;
        }
        DF_writeByteData(df->bytes, offset, 0); offset++;
    }
    return offset;
}

DF_u4 DFL_writeClassDataItems(DexFormat* df, DF_u4 offset) {
    DF_u4 i,j;
    DF_DATA_Class* classItems= df->classItems;
    DF_u2 directMethodsSize = classItems->directMethodsSize;
    DF_u2 virtualMethodsSize = classItems->virtualMethodsSize;
    DF_u2 staticFieldsSize = classItems->staticFieldsSize;
    DF_u2 instanceFieldsSize = classItems->instanceFieldsSize;

    df->classDefs->classDataOff = offset;
    df->classItemsOff = offset;
    
    offset = DF_writeUleb128Data(df->bytes, offset, staticFieldsSize);
    offset = DF_writeUleb128Data(df->bytes, offset, instanceFieldsSize);
    offset = DF_writeUleb128Data(df->bytes, offset, directMethodsSize);
    offset = DF_writeUleb128Data(df->bytes, offset, virtualMethodsSize);

    for(i = 0 ; i < staticFieldsSize ; i++) {
        offset = DF_writeUleb128Data(df->bytes, offset, classItems->staticFields[i].idxDiff);
        offset = DF_writeUleb128Data(df->bytes, offset, classItems->staticFields[i].accessFlags);
    }
    for(i = 0 ; i < instanceFieldsSize ; i++) {
        offset = DF_writeUleb128Data(df->bytes, offset, classItems->instanceFields[i].idxDiff);
        offset = DF_writeUleb128Data(df->bytes, offset, classItems->instanceFields[i].accessFlags);
    }

    for(i = 0 ; i < directMethodsSize; i++) {
        offset = DF_writeUleb128Data(df->bytes, offset, classItems->directMethods[i].idxDiff);
        offset = DF_writeUleb128Data(df->bytes, offset, classItems->directMethods[i].accessFlags);
        offset = DF_writeUleb128Data(df->bytes, offset, classItems->directMethods[i].addr);
    }
    for(i = 0 ; i < virtualMethodsSize; i++) {
        offset = DF_writeUleb128Data(df->bytes, offset, classItems->virtualMethods[i].idxDiff);
        offset = DF_writeUleb128Data(df->bytes, offset, classItems->virtualMethods[i].accessFlags);
        offset = DF_writeUleb128Data(df->bytes, offset, classItems->virtualMethods[i].addr);
    }
    return offset;
}
DF_u4 DFL_writeMapData(DexFormat* df, DF_u4 offset) {
    DF_u4 mapItem = 0;
    DF_align4bytes(offset);
    df->mapOff = offset;
    offset += 4;
    // header item
    mapItem++;
    DF_writeShortData(df->bytes, offset, 0); offset += 2;// type
    DF_writeShortData(df->bytes, offset, 0); offset += 2;// unused
    DF_writeIntData(df->bytes, offset, 1); offset += 4; // size
    DF_writeIntData(df->bytes, offset, 0); offset += 4; // offset
    
    // string id
    if(df->stringItemsSize > 0) {
        mapItem++;
        DF_writeShortData(df->bytes, offset, 1); offset += 2;// type
        DF_writeShortData(df->bytes, offset, 0); offset += 2;// unused
        DF_writeIntData(df->bytes, offset, df->stringItemsSize); offset += 4; // size
        DF_writeIntData(df->bytes, offset, df->stringIdsOff); offset += 4; // offset
    }
    
    //type id
    if(df->typeIdsSize > 0) {
        mapItem++;
        DF_writeShortData(df->bytes, offset, 2); offset += 2;// type
        DF_writeShortData(df->bytes, offset, 0); offset += 2;// unused
        DF_writeIntData(df->bytes, offset, df->typeIdsSize); offset += 4; // size
        DF_writeIntData(df->bytes, offset, df->typeIdsOff); offset += 4; // offset
    }
    
    //proto id
    if(df->protoIdsSize > 0) {
        mapItem++;
        DF_writeShortData(df->bytes, offset, 3); offset += 2;// proto
        DF_writeShortData(df->bytes, offset, 0); offset += 2;// unused
        DF_writeIntData(df->bytes, offset, df->protoIdsSize); offset += 4; // size
        DF_writeIntData(df->bytes, offset, df->protoIdsOff); offset += 4; // offset
    }

    //field id 
    if(df->fieldIdsSize > 0) {
        mapItem++;
        DF_writeShortData(df->bytes, offset, 4); offset += 2;// field
        DF_writeShortData(df->bytes, offset, 0); offset += 2;// unused
        DF_writeIntData(df->bytes, offset, df->fieldIdsSize); offset += 4; // size
        DF_writeIntData(df->bytes, offset, df->fieldIdsOff); offset += 4; // offset
    }

    //method item 
    if(df->methodIdsSize > 0) {
        mapItem++;
        DF_writeShortData(df->bytes, offset, 5); offset += 2;// method
        DF_writeShortData(df->bytes, offset, 0); offset += 2;// unused
        DF_writeIntData(df->bytes, offset, df->methodIdsSize); offset += 4; // size
        DF_writeIntData(df->bytes, offset, df->methodIdsOff); offset += 4; // offset
    }


    //classDefs id 
    if(df->classDefsSize > 0) {
        mapItem++;
        DF_writeShortData(df->bytes, offset, 6); offset += 2;// classDefs
        DF_writeShortData(df->bytes, offset, 0); offset += 2;// unused
        DF_writeIntData(df->bytes, offset, df->classDefsSize); offset += 4; // size
        DF_writeIntData(df->bytes, offset, df->classDefsOff); offset += 4; // offset
    }


    //code item 
    if(df->codeItemsSize > 0) {
        mapItem++;
        DF_writeShortData(df->bytes, offset, 0x2001); offset += 2;// type
        DF_writeShortData(df->bytes, offset, 0); offset += 2;// unused
        DF_writeIntData(df->bytes, offset, df->codeItemsSize); offset += 4; // size
        DF_writeIntData(df->bytes, offset, df->codeItemsOff); offset += 4; // offset
    }


    //typeList id 
    if(df->typeListSize > 0) {
        mapItem++;
        DF_writeShortData(df->bytes, offset, 0x1001); offset += 2;// type
        DF_writeShortData(df->bytes, offset, 0); offset += 2;// unused
        DF_writeIntData(df->bytes, offset, df->typeListSize); offset += 4; // size
        DF_writeIntData(df->bytes, offset, df->typeListOff); offset += 4; // offset
    }


    //string item 
    if(df->stringItemsSize > 0) {
        mapItem++;
        DF_writeShortData(df->bytes, offset, 0x2002); offset += 2;// type
        DF_writeShortData(df->bytes, offset, 0); offset += 2;// unused
        DF_writeIntData(df->bytes, offset, df->stringItemsSize); offset += 4; // size
        DF_writeIntData(df->bytes, offset, df->stringItemsOff); offset += 4; // offset
    }
    
    // class item
    /*
    if(df->classItemsSize > 0) {
        mapItem++;
        DF_writeShortData(df->bytes, offset, 0x2003); offset += 2;// type
        DF_writeShortData(df->bytes, offset, 0); offset += 2;// unused
        DF_writeIntData(df->bytes, offset, df->codeItemsSize); offset += 4; // size
        DF_writeIntData(df->bytes, offset, df->debugOff); offset += 4; // offset
    }
    */


    // class item
    if(df->classItemsSize > 0) {
        mapItem++;
        DF_writeShortData(df->bytes, offset, 0x2000); offset += 2;// type
        DF_writeShortData(df->bytes, offset, 0); offset += 2;// unused
        DF_writeIntData(df->bytes, offset, df->classItemsSize); offset += 4; // size
        DF_writeIntData(df->bytes, offset, df->classItemsOff); offset += 4; // offset
    }


    //map item 
    if(mapItem > 0) {
        mapItem++;
        DF_writeShortData(df->bytes, offset, 0x1000); offset += 2;// type
        DF_writeShortData(df->bytes, offset, 0); offset += 2;// unused
        DF_writeIntData(df->bytes, offset, 1); offset += 4; // size
        DF_writeIntData(df->bytes, offset, df->mapOff); offset += 4; // offset
    }



    DF_writeIntData(df->bytes, df->mapOff, mapItem);
    df->mapSize = mapItem;
    return offset;
}

DFL_bool DFL_writeDexData(DexFormat* df, DF_u4 offset) {
    DF_u4 off = offset;
    offset = DFL_writeCodeItems(df, offset);
    off = offset;
    offset = DFL_writeTypeLists(df, offset);
    off = offset;
    offset = DFL_writeStringData(df, offset);
    off = offset;
    offset = DFL_writeClassDataItems(df, offset);
    df->mapOff = offset;
    //df->fileSize = offset;
    //df->dataSize = offset - df->dataOff;
    return DFL_TRUE;
}

DF_u4 DFL_writeStringIds(DexFormat* df, DF_u4 offset) {
    DF_u4 i;
    df->stringIdsOff = offset;
    for(i = 0 ; i < df->stringItemsSize ; i++) {
        DF_writeIntData(df->bytes, offset, df->stringItems[i].addr); offset += 4;
    }
    return offset;
}
DF_u4 DFL_writeTypeIds(DexFormat* df, DF_u4 offset) {
    DF_u4 i;
    df->typeIdsOff = offset;
    for(i = 0 ; i < df->typeIdsSize ; i++) {
        DF_writeIntData(df->bytes, offset, df->typeIds[i].descriptorIdx); offset += 4;
    }
    return offset;
}
DF_u4 DFL_writeProtoIds(DexFormat* df, DF_u4 offset) {
    DF_u4 i;
    df->protoIdsOff = offset;
    for(i = 0 ; i < df->protoIdsSize ; i++) {
        DF_s8 parametersOff = df->protoIds[i].parametersOff;
        DF_writeIntData(df->bytes, offset, df->protoIds[i].shortyIdx); offset += 4;
        DF_writeIntData(df->bytes, offset, df->protoIds[i].returnTypeIdx); offset += 4;
        if(parametersOff >= 0) {
            DF_writeIntData(df->bytes, offset, df->typeList[parametersOff].addr);
        }else {
            DF_writeIntData(df->bytes, offset, 0);
        }
        offset += 4;
    }
    return offset;
}

DF_u4 DFL_writeFieldIds(DexFormat* df, DF_u4 offset) {
    DF_u4 i;
    df->fieldIdsOff = offset;
    for(i = 0 ; i < df->fieldIdsSize ; i++) {
        DF_writeShortData(df->bytes, offset, df->fieldIds[i].classIdx);offset += 2;
        DF_writeShortData(df->bytes, offset, df->fieldIds[i].typeIdx);offset += 2;
        DF_writeIntData(df->bytes, offset, df->fieldIds[i].nameIdx);offset += 4;
    }
    return offset;
}
DF_u4 DFL_writeMethodIds(DexFormat* df, DF_u4 offset) {
    DF_u4 i;
    df->methodIdsOff = offset;
    for(i = 0 ; i < df->methodIdsSize ; i++) {
        DF_writeShortData(df->bytes, offset, df->methodIds[i].classIdx);offset += 2;
        DF_writeShortData(df->bytes, offset, df->methodIds[i].protoIdx);offset += 2;
        DF_writeIntData(df->bytes, offset, df->methodIds[i].nameIdx);offset += 4;
    }
    return offset;
}
DF_u4 DFL_writeClassDefs(DexFormat* df, DF_u4 offset) {
    DF_u4 i;
    DF_DEF_Class* classItem = df->classDefs;
    df->classDefsOff = offset;
    DF_writeIntData(df->bytes, offset, classItem->classIdx); offset += 4;
    DF_writeIntData(df->bytes, offset, classItem->accessFlags); offset += 4;
    DF_writeIntData(df->bytes, offset, classItem->superClassIdx); offset += 4;
    if(df->interfacesOff >=0) {
        DF_writeIntData(df->bytes, offset, df->typeList[df->interfacesOff].addr); offset += 4;
    }else {
        DF_writeIntData(df->bytes, offset, 0); offset += 4;
    }
    DF_writeIntData(df->bytes, offset, -1); offset += 4; // source file
    DF_writeIntData(df->bytes, offset, 0); offset += 4; // annoation off
    DF_writeIntData(df->bytes, offset, classItem->classDataOff); offset += 4;
    DF_writeIntData(df->bytes, offset, classItem->staticValuesOff); offset += 4;
    return offset;
}

DFL_bool DFL_writeDexIds(DexFormat* df, DF_u4 offset) {
    offset = DFL_writeStringIds(df, offset);
    offset = DFL_writeTypeIds(df, offset);
    offset = DFL_writeProtoIds(df, offset);
    offset = DFL_writeFieldIds(df, offset);
    offset = DFL_writeMethodIds(df, offset);
    offset = DFL_writeClassDefs(df, offset);
    assert(df->dataOff == offset);
    return DFL_TRUE;
}
DFL_bool DFL_writeDexHeader(DexFormat* df, DF_u4 offset) {
    // Magic
    memcpy(df->bytes, DEX_MAGIC, 4);
    memcpy(df->bytes + 4, DEX_MAGIC_VERS, 4);
    offset += 8;
    // CheckSum & Signature
    offset += 24;
    // fileSize
    DF_writeIntData(df->bytes, offset, df->fileSize); offset += 4;
    // headerSize
    DF_writeIntData(df->bytes, offset, 0x70); offset += 4;
    // Endian Tag
    DF_writeIntData(df->bytes, offset, kDexEndianConstant); offset += 4;
    // Link Size & Off
    DF_writeIntData(df->bytes, offset, 0); offset += 4;
    DF_writeIntData(df->bytes, offset, 0); offset += 4;
    // Map Off
    DF_writeIntData(df->bytes, offset, df->mapOff); offset += 4;
    // String Ids
    DF_writeIntData(df->bytes, offset, df->stringItemsSize); offset += 4;
    DF_writeIntData(df->bytes, offset, df->stringIdsOff); offset += 4;
    // Type Ids
    DF_writeIntData(df->bytes, offset, df->typeIdsSize); offset += 4;
    DF_writeIntData(df->bytes, offset, df->typeIdsOff); offset += 4;
    // Proto Ids
    DF_writeIntData(df->bytes, offset, df->protoIdsSize); offset += 4;
    DF_writeIntData(df->bytes, offset, df->protoIdsOff); offset += 4;
    // Field Ids
    DF_writeIntData(df->bytes, offset, df->fieldIdsSize); offset += 4;
    DF_writeIntData(df->bytes, offset, df->fieldIdsOff); offset += 4;
    // Method Ids
    DF_writeIntData(df->bytes, offset, df->methodIdsSize); offset += 4;
    DF_writeIntData(df->bytes, offset, df->methodIdsOff); offset += 4;
    // Class Defs
    DF_writeIntData(df->bytes, offset, 1); offset += 4;
    DF_writeIntData(df->bytes, offset, df->classDefsOff); offset += 4;
    // Data
    df->dataSize = df->fileSize - df->dataOff;
    DF_writeIntData(df->bytes, offset, df->dataSize); offset += 4;
    DF_writeIntData(df->bytes, offset, df->dataOff); offset += 4;
    // Signature
    {
        unsigned char sha1Digest[kSHA1DigestLen];
        DF_u4 i;
        {
            SHA1_CTX context;
            SHA1Init(&context);
            SHA1Update(&context, (df->bytes + 32), (df->fileSize - 32));
            SHA1Final(sha1Digest, &context);
        }   
        for(i = 12 ; i < 32 ; i++) {
            DF_writeByteData(df->bytes, i, sha1Digest[i]);
        }
    }
    // Check Sum
    {   
        DF_u4 adler = (DF_u4)adler32(adler32(0L, Z_NULL, 0), df->bytes + 12, df->fileSize - 12);
        if(verbose) fprintf(stderr, "CHECK SUM is %08x\n", adler);
        DF_writeIntData(df->bytes, 8, adler);
    }
    return DFL_TRUE;
}

DF_u4 DFL_guessFileSize(DexFormat* df) {
    // calcualted length
    DF_u4 temp = df->dataOff + df->stringItemsLen + df->codeItemsLen + df->typeListLen + df->classItemsLen;
    temp += (12 * 13 + 4);
    return temp;
}

DFL_bool DFL_writeDexFormat(DexFormat* df) {
    DF_u4 tempSize;
    df->dataOff = DFL_findDataOffset(df);
    tempSize = DFL_guessFileSize(df);
    df->bytes = (DF_u1*)allocateMem(df->mm, sizeof(DF_u1*) * tempSize);
    DFL_writeDexData(df, df->dataOff);
    DFL_writeDexIds(df, 0x70);
    df->fileSize = DFL_writeMapData(df, df->mapOff);
    DFL_writeDexHeader(df, 0);
    if(verbose)
        if(tempSize < df->fileSize) {
            fprintf(stderr, "tmp %d file %d\n", tempSize, df->fileSize);
        }
    assert(tempSize > df->fileSize);
    return DFL_TRUE;
}

DFL_bool DFL_printString(DexFormat* df, DF_u4 i) {
    if(verbose) fprintf(stderr, "%2x(%s, %d)", i, df->stringItems[i].data, df->stringItems[i].size);
    return DFL_TRUE;
}
DFL_bool DFL_printType(DexFormat* df, DF_u4 i) {
    if(verbose) fprintf(stderr, "%02x(", i);
    DFL_printString(df, df->typeIds[i].descriptorIdx);
    if(verbose) fprintf(stderr, ")");
    return DFL_TRUE;
}
DFL_bool DFL_printTypeList(DexFormat* df, DF_s8 i) {
    DF_u4 j;
    if(i == -1) {
        if(verbose) fprintf(stderr, "NAN()");
        return DFL_TRUE;
    }
    if(verbose) fprintf(stderr, "%02x(", (DF_u4)i);
    for(j = 0 ; j < df->typeList[i].size ; j++) {
        DFL_printType(df, df->typeList[i].typeItems[j].typeIdx);
        if(verbose) if(j != df->typeList[i].size - 1) fprintf(stderr, " ");
    }
    if(verbose) fprintf(stderr, ")");
    return DFL_TRUE;
}
DFL_bool DFL_printProtoType(DexFormat* df, DF_u8 i) {
    
    DFL_printString(df, df->protoIds[i].shortyIdx);
    DFL_printType(df, df->protoIds[i].returnTypeIdx);
    DFL_printTypeList(df, df->protoIds[i].parametersOff);
    return DFL_TRUE;
}
DFL_bool DFL_printField(DexFormat* df, DF_u4 i) {
    DFL_printType(df, df->fieldIds[i].classIdx);
    DFL_printType(df, df->fieldIds[i].typeIdx);
    DFL_printString(df, df->fieldIds[i].nameIdx);
    return DFL_TRUE;
}
DFL_bool DFL_printMethod(DexFormat* df, DF_u4 i) {
    DFL_printType(df, df->methodIds[i].classIdx);
    DFL_printProtoType(df, df->methodIds[i].protoIdx);
    DFL_printString(df, df->methodIds[i].nameIdx);
    return DFL_TRUE;
}
DFL_bool DFL_printStaticField(DexFormat* df, DF_u4 i) {
    // Not Yet Implemented
    return DFL_TRUE;
}

DFL_bool DFL_printInstanceField(DexFormat* df, DF_u4 i) {
    // Not Yet Implemented
    return DFL_TRUE;
}
DFL_bool DFL_printDirectMethod(DexFormat* df, DF_u4 i) {
    DF_DATA_Class* classItem = df->classItems;
    if(classItem->directMethods[i].codeOff == -1) return DFL_TRUE;
    DF_DATA_Code code = df->codeItems[classItem->directMethods[i].codeOff];
    if(verbose) fprintf(stderr, "%02x\n", code.registersSize);
    if(verbose) fprintf(stderr, "%02x\n", code.insSize);
    if(verbose) fprintf(stderr, "%02x\n", code.outsSize);
    if(verbose) fprintf(stderr, "%02x\n", code.triesSize);
    if(verbose) fprintf(stderr, "%02x\n", code.debugInfoOff);
    if(verbose) fprintf(stderr, "%02x\n", code.insnsSize);
    for(i = 0 ; i < code.insnsSize ; i++) {
        if(verbose) fprintf(stderr, "%02x", code.insns[i] & 0xff);
        if(verbose) fprintf(stderr, "%02x", code.insns[i] >> 8);
        if(i != code.insnsSize - 1) if(verbose) fprintf(stderr, " ");
    }    
    if(verbose) fprintf(stderr, "\n");
    return DFL_TRUE;
}

DFL_bool DFL_printVirtualMethod(DexFormat* df, DF_u4 i) {
    DF_DATA_Class* classItem = df->classItems;
    if(classItem->virtualMethods[i].codeOff == -1) return DFL_TRUE;
    DF_DATA_Code code = df->codeItems[classItem->virtualMethods[i].codeOff];
    if(verbose) fprintf(stderr, "%02x\n", code.registersSize);
    if(verbose) fprintf(stderr, "%02x\n", code.insSize);
    if(verbose) fprintf(stderr, "%02x\n", code.outsSize);
    if(verbose) fprintf(stderr, "%02x\n", code.triesSize);
    if(verbose) fprintf(stderr, "%02x\n", code.debugInfoOff);
    if(verbose) fprintf(stderr, "%02x\n", code.insnsSize);
    for(i = 0 ; i < code.insnsSize ; i++) {
        if(verbose) fprintf(stderr, "%02x", code.insns[i] & 0xff);
        if(verbose) fprintf(stderr, "%02x", code.insns[i] >> 8);
        if(i != code.insnsSize - 1) if(verbose) fprintf(stderr, " ");
    }    
    if(verbose) fprintf(stderr, "\n");
    return DFL_TRUE;
}
DFL_bool DFL_printClassData(DexFormat* df) {
    DF_DATA_Class* classItem = df->classItems;
    DF_u4 i;
    if(verbose) fprintf(stderr, "Class Data\n%02x\n%02x\n%02x\n%02x\n", classItem->staticFieldsSize, classItem->instanceFieldsSize, classItem->directMethodsSize, classItem->virtualMethodsSize);
    if(verbose) fprintf(stderr, "Static Field\n");
    for(i = 0 ; i < classItem->staticFieldsSize; i++) {
        if(verbose) fprintf(stderr, "[%d] %02x %02x\n", i, classItem->staticFields[i].idxDiff, classItem->staticFields[i].accessFlags);
        DFL_printStaticField(df, i);
    }
    if(verbose) fprintf(stderr, "Instance Field\n");
    for(i = 0 ; i < classItem->instanceFieldsSize; i++) {
        if(verbose) fprintf(stderr, "[%d] %02x %02x\n", i, classItem->instanceFields[i].idxDiff, classItem->instanceFields[i].accessFlags);
        DFL_printInstanceField(df, i);
    }
    if(verbose) fprintf(stderr, "Direct Method\n");
    for(i = 0 ; i < classItem->directMethodsSize; i++) {
        if(verbose) fprintf(stderr, "[%d] %02x %02x\n", i, classItem->directMethods[i].idxDiff, classItem->directMethods[i].accessFlags);
        DFL_printDirectMethod(df, i);
    }
    if(verbose) fprintf(stderr, "Virtual Method\n");
    for(i = 0 ; i < classItem->virtualMethodsSize; i++) {
        if(verbose) fprintf(stderr, "[%d] %02x %02x\n", i, classItem->virtualMethods[i].idxDiff, classItem->virtualMethods[i].accessFlags);
        DFL_printVirtualMethod(df, i);
    }
    return DFL_TRUE;
}
DFL_bool DFL_printClassDef(DexFormat* df) {
   DFL_printType(df, df->classDefs->classIdx); 
   if(verbose) fprintf(stderr, "\n%02x\n", df->classDefs->accessFlags);
   DFL_printType(df, df->classDefs->superClassIdx); 
   DFL_printTypeList(df, df->interfacesOff); 
   //DFL_printType(df, df->classDefs->sourceFileIdx); 
   //DFL_printType(df, df->classDefs->annotationsOff); 
   if(verbose) fprintf(stderr, "\n00\n00\n00\n");
   DFL_printClassData(df);
   if(verbose) fprintf(stderr, "00\n");
   return DFL_TRUE;
}
DFL_bool DFL_printBinaryFile(DexFormat* df) {
    DF_s4 len = strlen((const char*)df->fileName);
    DF_s4 i;
    DF_u1* fileName = (DF_u1*)allocateMem(df->mm, len);
    for(i = (len - 1) ; i >= 0 ; i--) {
        if(df->fileName[i] == '.')break;
    }
    memcpy(fileName, df->fileName, i);
    memcpy(fileName + i, ".dex\0", 5);
    FILE* fd = fopen((const char*)fileName, "wb");
    if(fd == NULL) {
        fprintf(stderr, "FILE OPEN ERROR!\n");
        return DFL_FALSE;
    }
    fwrite(df->bytes, 1, df->fileSize, fd);
    fclose(fd);
    return DFL_TRUE;
}

DFL_bool DFL_printDexFormat(DexFormat* df) {
    DF_u4 i,j;
    if(verbose) fprintf(stderr, "String %u\n", df->stringItemsSize);
    for(i = 0 ; i < df->stringItemsSize ; i++) {
        if(verbose) fprintf(stderr, "[%d] ", i);
        DFL_printString(df, i);
        if(verbose) fprintf(stderr, "\n");
    }
    if(verbose) fprintf(stderr, "Type %u\n", df->typeIdsSize);
    for(i = 0 ; i < df->typeIdsSize ; i++) {
        if(verbose) fprintf(stderr, "[%d] ", i);
        DFL_printType(df, i);
        if(verbose) fprintf(stderr, "\n");
    }
    if(verbose) fprintf(stderr, "ProtoType %u\n", df->protoIdsSize);
    for(i = 0 ; i < df->protoIdsSize; i++) {
        if(verbose) fprintf(stderr, "[%d] ", i);
        DFL_printProtoType(df, i);
        if(verbose) fprintf(stderr, ")\n");
    }
    if(verbose) fprintf(stderr, "Field %u\n", df->fieldIdsSize);
    for(i = 0 ; i < df->fieldIdsSize ; i++) {
        if(verbose) fprintf(stderr, "[%d] ", i);
        DFL_printField(df, i);
        if(verbose) fprintf(stderr, ")\n");
    }
    if(verbose) fprintf(stderr, "Method %u\n", df->methodIdsSize);
    for(i = 0 ; i < df->methodIdsSize ; i++) {
        if(verbose) fprintf(stderr, "[%d] ", i);
        DFL_printMethod(df, i);
        if(verbose) fprintf(stderr, ")\n");
    }
    if(verbose) fprintf(stderr, "Class Def\n");
    DFL_printClassDef(df);
    if(verbose) fprintf(stderr, "String Len: %u, Code Len: %u, TypeList Len: %u, Class Len: %u\n", df->stringItemsLen, df->codeItemsLen, df->typeListLen, df->classItemsLen);
    if(verbose) fprintf(stderr, "Write Dex Format to File\n");
    if(verbose) DFL_printBinaryFile(df);   
    return DFL_TRUE;
}
