#include "ClassFileReader.h"

typedef enum {CFR_FALSE = 0, CFR_TRUE = 1} CFR_bool;
typedef union {
    CF_u8 l;
    CF_f8 f;
    CF_u4 i[2];
}CFR_TYPE_WIDE;

ClassFile* CFR_process(char* pathName);
FILE* CFR_openClassFile(const char* fileName);
void CFR_closeClassFile(FILE* fd);
ClassFile* CFR_parse(FILE* fd);
CFR_bool CFR_readHeader(FILE* fd, ClassFile* cf);
CFR_bool CFR_readConstantPool(FILE* fd, ClassFile* cf);
CFR_bool CFR_readThisClassInfo(FILE* fd, ClassFile* cf);
CFR_bool CFR_readInterfaces(FILE* fd, ClassFile* cf);
CFR_bool CFR_readFields(FILE* fd, ClassFile* cf);
CFR_bool CFR_readMethods(FILE* fd, ClassFile* cf);
CFR_bool CFR_readFieldAttributes(FILE* fd, ClassFile* cf, CF_Field* fb);
CFR_bool CFR_readMethodAttributes(FILE* fd, ClassFile* cf, CF_Method* mb);
CFR_bool CFR_readClassAttributes(FILE* fd, ClassFile* cf);
CFR_bool CFR_readAttribute(FILE* fd, ClassFile* cf, CF_Attribute* attr);
CFR_bool CFR_readConstantValueAttribute(FILE* fd, ClassFile* cf, CF_Attribute* attr);
CFR_bool CFR_readCodeAttribute(FILE* fd, ClassFile* cf, CF_Attribute* attr);
CFR_bool CFR_readExceptionsAttribute(FILE* fd, ClassFile* cf, CF_Attribute* attr);
CFR_bool CFR_readInnerClassesAttribute(FILE* fd, ClassFile* cf, CF_Attribute* attr);
CFR_bool CFR_readSyntheticAttribute(FILE* fd, ClassFile* cf, CF_Attribute* attr);
CFR_bool CFR_readSourceFileAttribute(FILE* fd, ClassFile* cf, CF_Attribute* attr);
CFR_bool CFR_readLineNumberTableAttribute(FILE* fd, ClassFile* cf, CF_Attribute* attr);
CFR_bool CFR_readLocalVariableTableAttribute(FILE* fd, ClassFile* cf, CF_Attribute* attr);
CFR_bool CFR_readDeprecatedAttribute(FILE* fd, ClassFile* cf, CF_Attribute* attr);
CFR_bool CFR_readOtherAttribute(FILE* fd, ClassFile* cf, CF_Attribute* attr);
CF_u1 read1byte(FILE* fd);
CF_u2 read2bytes(FILE* fd);
CF_u4 read4bytes(FILE* fd);
CF_u8 read8bytes(FILE* fd);
CF_u1* readBytes(FILE* fd, int length, MemoryManager* mm);

ClassFile* readClassFile(char* pathName) {
    ClassFile* cf;
    if(verbose) fprintf(stderr, "Class to Dex Translator %s\n", pathName);
    if(verbose) fprintf(stderr, "Start <<< Read Class File! >>>\n");
    cf = CFR_process(pathName);
    if(verbose) fprintf(stderr, "Finish <<< Read Class File! >>>\n");
    return cf;
}

ClassFile* CFR_process(char* pathName) {
    FILE* fd = CFR_openClassFile(pathName);
    ClassFile* cf;
    if(fd == NULL) return NULL;
    cf = CFR_parse(fd);
    cf->fileName = (CF_u1*)pathName;
    CFR_closeClassFile(fd);
    return cf;
}

FILE* CFR_openClassFile(const char* fileName) {
    FILE* fd;

    fd = fopen(fileName, "rb");
    if(fd == NULL) {
        if(verbose) fprintf(stderr, "Class File Open Error!\n");
        return NULL;
    }
    return fd;
}
void CFR_closeClassFile(FILE* fd) {
    if(fd != NULL) fclose(fd);
}

ClassFile* CFR_parse(FILE* fd) {
    MemoryManager* mm = initializeMem();
    ClassFile* cf = (ClassFile*)allocateMem(mm, sizeof(ClassFile));
    cf->mm = mm;
    //cf->sourceFile = NULL;
    if(!CFR_readHeader(fd, cf)) cf = NULL;
    if(!CFR_readConstantPool(fd, cf)) cf = NULL;
    if(!CFR_readThisClassInfo(fd, cf)) cf = NULL;
    if(!CFR_readInterfaces(fd, cf)) cf = NULL;
    if(!CFR_readFields(fd, cf)) cf = NULL;
    if(!CFR_readMethods(fd, cf)) cf = NULL;
    if(!CFR_readClassAttributes(fd, cf)) cf = NULL;
    if(cf == NULL) freeAllMem(mm);
    return cf;
}

CFR_bool CFR_readHeader(FILE* fd, ClassFile* cf) {
    //CF_u4 magic;
    //CF_u2 minor_version, major_version;
    cf->magic = read4bytes(fd);
    cf->minorVersion = read2bytes(fd);
    cf->majorVersion = read2bytes(fd);
    if(verbose) fprintf(stderr, "Magis Number: 0x%x\n", cf->magic);
    if(verbose) fprintf(stderr, "Minor Version: %d\n", cf->minorVersion);
    if(verbose) fprintf(stderr, "Major Version: %d\n", cf->majorVersion);
    assert(cf->magic == 0xcafebabe);
    if(cf->magic != 0xcafebabe) return CFR_FALSE;
    return CFR_TRUE;
}

CFR_bool CFR_readConstantPool(FILE* fd, ClassFile* cf) {
    int i;
    //CF_u2 constant_pool_count;
    cf->constantPoolCount = read2bytes(fd) - 1;
    cf->cp = (CF_ConstantPool*)allocateMem(cf->mm, (sizeof(CF_ConstantPool) * (cf->constantPoolCount + 1)));
    if(verbose) fprintf(stderr, "Constant Pool Count: %d\n", cf->constantPoolCount);
    for(i = 1 ; i <= cf->constantPoolCount ; i++) {
        CF_u1 tag = read1byte(fd);
        if(verbose) fprintf(stderr, "[%2d/%2d] ", i, tag);
        switch(tag) {
            case CF_CONSTANT_Utf8Type:
                {
                    cf->cp[i].tag = tag;
                    cf->cp[i].type.utf8Info.length = read2bytes(fd);
                    cf->cp[i].type.utf8Info.bytes = readBytes(fd, cf->cp[i].type.utf8Info.length, cf->mm);
                    if(verbose) fprintf(stderr, "Utf8 Constant %d \"%s\"\n", cf->cp[i].type.utf8Info.length, cf->cp[i].type.utf8Info.bytes);
                    break;
                }
            case CF_CONSTANT_IntegerType:
                {
                    cf->cp[i].tag = tag;
                    cf->cp[i].type.integerInfo.bytes = read4bytes(fd);
                    if(verbose) fprintf(stderr,"Integer Constant %d\n", cf->cp[i].type.integerInfo.bytes);
                    break;
                }
            case CF_CONSTANT_FloatType:
                {
                    cf->cp[i].tag = tag;
                    cf->cp[i].type.floatInfo.bytes = read4bytes(fd);
                    if(verbose) fprintf(stderr,"Float Constant %f\n", (float)cf->cp[i].type.floatInfo.bytes);
                    break;
                }
            case CF_CONSTANT_LongType:
                {
                    cf->cp[i].tag = tag;
                    cf->cp[i].type.longInfo.bytes = read8bytes(fd);
                    if(verbose) fprintf(stderr,"Long Constant %Ld\n", (CF_u8)cf->cp[i].type.longInfo.bytes);
                    i++;
                    break;
                }
            case CF_CONSTANT_DoubleType:
                {
                    CFR_TYPE_WIDE wide;
                    cf->cp[i].tag = tag;
                    //cf->cp[i].type.doubleInfo.bytes = read8bytes(fd);
                    wide.i[1] = read4bytes(fd);
                    wide.i[0] = read4bytes(fd);
                    cf->cp[i].type.doubleInfo.bytes = wide.l;
                    if(verbose) fprintf(stderr,"Double Constant %f\n", (CF_f8)wide.f);
                    i++;
                    break;
                }
            case CF_CONSTANT_ClassrefType:
                {
                    cf->cp[i].tag = tag;
                    cf->cp[i].type.classInfo.nameIndex = read2bytes(fd);
                    if(verbose) fprintf(stderr,"Classref Constant %d\n", cf->cp[i].type.classInfo.nameIndex);
                    break;
                }
            case CF_CONSTANT_StringType:
                {
                    cf->cp[i].tag = tag;
                    cf->cp[i].type.stringInfo.stringIndex = read2bytes(fd);
                    if(verbose) fprintf(stderr,"String Constant %d\n", cf->cp[i].type.stringInfo.stringIndex);
                    break;
                }
            case CF_CONSTANT_FieldrefType:
                {
                    cf->cp[i].tag = tag;
                    cf->cp[i].type.fieldInfo.classIndex = read2bytes(fd);
                    cf->cp[i].type.fieldInfo.nameAndTypeIndex = read2bytes(fd);
                    if(verbose) fprintf(stderr,"Fieldref Constant %d %d\n", cf->cp[i].type.fieldInfo.classIndex, cf->cp[i].type.fieldInfo.nameAndTypeIndex);
                    break;
                }
            case CF_CONSTANT_MethodrefType:
                {
                    cf->cp[i].tag = tag;
                    cf->cp[i].type.methodInfo.classIndex = read2bytes(fd);
                    cf->cp[i].type.methodInfo.nameAndTypeIndex = read2bytes(fd);
                    if(verbose) fprintf(stderr,"Methodref Constant %d %d\n", cf->cp[i].type.fieldInfo.classIndex, cf->cp[i].type.methodInfo.nameAndTypeIndex);
                    break;
                }
            case CF_CONSTANT_InterfaceMethodrefType:
                {
                    cf->cp[i].tag = tag;
                    cf->cp[i].type.interfaceInfo.classIndex = read2bytes(fd);
                    cf->cp[i].type.interfaceInfo.nameAndTypeIndex = read2bytes(fd);
                    if(verbose) fprintf(stderr,"Interface Methodref Constant %d %d\n", cf->cp[i].type.interfaceInfo.classIndex, cf->cp[i].type.interfaceInfo.nameAndTypeIndex);
                    break;
                }
            case CF_CONSTANT_NameAndTypeType:
                {
                    cf->cp[i].tag = tag;
                    cf->cp[i].type.nameAndTypeInfo.nameIndex = read2bytes(fd);
                    cf->cp[i].type.nameAndTypeInfo.descriptorIndex = read2bytes(fd);
                    if(verbose) fprintf(stderr,"Name And Type %d %d\n", cf->cp[i].type.nameAndTypeInfo.nameIndex, cf->cp[i].type.nameAndTypeInfo.descriptorIndex);
                    break;
                }
            default:
            {
                if(verbose) fprintf(stderr,"Invalid Constant Pool Entry %d\n", tag);
                return CFR_FALSE;
            }
            
        }
    }
    return CFR_TRUE;
}

CFR_bool CFR_readThisClassInfo(FILE* fd, ClassFile* cf) {
    cf->accessFlags = read2bytes(fd);
    cf->thisClass = read2bytes(fd);
    cf->superClass = read2bytes(fd);
    if(verbose) fprintf(stderr, "Access Flags: 0x%x\n", cf->accessFlags);
    if(verbose) fprintf(stderr, "This Class: %d\n", cf->thisClass);
    if(verbose) fprintf(stderr, "Super Class: %d\n", cf->superClass);
    return CFR_TRUE;
}

CFR_bool CFR_readInterfaces(FILE* fd, ClassFile* cf) {
    int i;
    cf->interfacesCount = read2bytes(fd);
    if(verbose) fprintf(stderr, "Interfaces Count: %d\n", cf->interfacesCount);
    cf->interfaces = (CF_Interface*)allocateMem(cf->mm, sizeof(CF_Interface) * cf->interfacesCount);
    for(i = 0 ; i < cf->interfacesCount ; i++) {
        cf->interfaces[i] = read2bytes(fd);
        if(verbose) fprintf(stderr, "[%2d] %d\n", (i+1), cf->interfaces[i]);
    }
    return CFR_TRUE;
}
CFR_bool CFR_readFields(FILE* fd, ClassFile* cf) {
    int i,j;
    cf->fieldsCount = read2bytes(fd);
    if(verbose) fprintf(stderr, "Fields Count: %d\n", cf->fieldsCount);
    cf->fields = (CF_Field*)allocateMem(cf->mm, sizeof(CF_Field) * cf->fieldsCount);
    for(i = 0 ; i < cf->fieldsCount ; i++) {
        cf->fields[i].accessFlags = read2bytes(fd);
        cf->fields[i].nameIndex = read2bytes(fd);
        cf->fields[i].descriptorIndex = read2bytes(fd);
        if(verbose) fprintf(stderr, "[%2d] 0x%x %d %d\n", (i+1), cf->fields[i].accessFlags, cf->fields[i].nameIndex, cf->fields[i].descriptorIndex);
        if(!CFR_readFieldAttributes(fd, cf, &cf->fields[i])) return CFR_FALSE;
    }
    return CFR_TRUE;
}
CFR_bool CFR_readMethods(FILE* fd, ClassFile* cf) {
    int i,j;
    cf->methodsCount = read2bytes(fd);
    if(verbose) fprintf(stderr, "Methods Count: %d\n", cf->methodsCount);
    cf->methods = (CF_Method*)allocateMem(cf->mm, sizeof(CF_Method) * cf->methodsCount);
    for(i = 0 ; i < cf->methodsCount ; i++) {
        cf->methods[i].accessFlags = read2bytes(fd);
        cf->methods[i].nameIndex = read2bytes(fd);
        cf->methods[i].descriptorIndex = read2bytes(fd);
        if(verbose) fprintf(stderr, "[%2d] 0x%x %d %d\n", (i+1), cf->methods[i].accessFlags, cf->methods[i].nameIndex, cf->methods[i].descriptorIndex);
        if(!CFR_readMethodAttributes(fd, cf, &cf->methods[i])) return CFR_FALSE;
    }
    return CFR_TRUE;
}
CFR_bool CFR_readFieldAttributes(FILE* fd, ClassFile* cf, CF_Field* fb) {
    int i;
    fb->attributesCount = read2bytes(fd);
    fb->attributes = (CF_Attribute*)allocateMem(cf->mm, sizeof(CF_Attribute) * fb->attributesCount);
    if(verbose) fprintf(stderr, "Field Attributes Count: %d\n", fb->attributesCount);
    for(i = 0 ; i < fb->attributesCount ; i++) {
        if(!CFR_readAttribute(fd, cf, &fb->attributes[i])) return CFR_FALSE;
    }
    return CFR_TRUE;
}
CFR_bool CFR_readMethodAttributes(FILE* fd, ClassFile* cf, CF_Method* mb) {
    int i;
    mb->attributesCount = read2bytes(fd);
    mb->attributes = (CF_Attribute*)allocateMem(cf->mm, sizeof(CF_Attribute) * mb->attributesCount);
    if(verbose) fprintf(stderr, "Method Attributes Count: %d\n", mb->attributesCount);
    for(i = 0 ; i < mb->attributesCount ; i++) {
        if(!CFR_readAttribute(fd, cf, &mb->attributes[i])) return CFR_FALSE;
        if(mb->attributes[i].attributeType == CF_ATTRIBUTE_CodeType) {
            mb->codeAttr = mb->attributes[i].type.codeAttr;
        }
    }
    return CFR_TRUE;
}
CFR_bool CFR_readClassAttributes(FILE* fd, ClassFile* cf) {
    int i;
    cf->attributesCount = read2bytes(fd);
    cf->attributes = (CF_Attribute*)allocateMem(cf->mm, sizeof(CF_Attribute) * cf->attributesCount);
    if(verbose) fprintf(stderr, "Class Attributes Count: %d\n", cf->attributesCount);
    for(i = 0 ; i < cf->attributesCount ; i++) {
        if(!CFR_readAttribute(fd, cf, &cf->attributes[i])) return CFR_FALSE;
    }
    return CFR_TRUE;
}
CFR_bool CFR_readAttribute(FILE* fd, ClassFile* cf, CF_Attribute* attr) {
    int i;
    char* attributeName;
    attr->attributeNameIndex = read2bytes(fd);
    attr->attributeLength = read4bytes(fd);
    attributeName = (char*)cf->cp[attr->attributeNameIndex].type.utf8Info.bytes;

    if(verbose) fprintf(stderr, "Attribute Index: %d %d (%s)\n", attr->attributeNameIndex, attr->attributeLength, attributeName);
    if(strcmp((const char*)attributeName, "ConstantValue") == 0) {
        return CFR_readConstantValueAttribute(fd, cf, attr);
    }else if(strcmp((const char*)attributeName, "Code") == 0) {
        return CFR_readCodeAttribute(fd, cf, attr);
    }else if(strcmp((const char*)attributeName, "Exceptions") == 0) {
        return CFR_readExceptionsAttribute(fd, cf, attr);
    }else if(strcmp((const char*)attributeName, "InnerClasses") == 0) {
        return CFR_readInnerClassesAttribute(fd, cf, attr);
    }else if(strcmp((const char*)attributeName, "Synthetic") == 0) {
        return CFR_readSyntheticAttribute(fd, cf, attr);
    }else if(strcmp((const char*)attributeName, "SourceFile") == 0) {
        return CFR_readSourceFileAttribute(fd, cf, attr);
    }else if(strcmp((const char*)attributeName, "LineNumberTable") == 0) {
        return CFR_readLineNumberTableAttribute(fd, cf, attr);
    }else if(strcmp((const char*)attributeName, "LocalVariableTable") == 0) {
        return CFR_readLocalVariableTableAttribute(fd, cf, attr);
    }else if(strcmp((const char*)attributeName, "Deprecated") == 0) {
        return CFR_readDeprecatedAttribute(fd, cf, attr);
    }else {
        return CFR_readOtherAttribute(fd, cf, attr);
    }
}

CFR_bool CFR_readConstantValueAttribute(FILE* fd, ClassFile* cf, CF_Attribute* attr) {
    assert(attr->attributeLength == 2);
    attr->attributeType = CF_ATTRIBUTE_ConstantValueType;
    attr->type.constantValueAttr.constantValueIndex = read2bytes(fd);
    if(verbose) fprintf(stderr, "Constant Value Attribute: %d\n", attr->type.constantValueAttr.constantValueIndex);
    if(attr->attributeLength != 2) return CFR_FALSE;
    return CFR_TRUE;
}
CFR_bool CFR_readCodeAttribute(FILE* fd, ClassFile* cf, CF_Attribute* attr) {
    int i;
    attr->attributeType = CF_ATTRIBUTE_CodeType;
    attr->type.codeAttr.maxStack = read2bytes(fd);
    attr->type.codeAttr.maxLocals = read2bytes(fd);
    attr->type.codeAttr.codeLength = read4bytes(fd);
    attr->type.codeAttr.code = readBytes(fd, attr->type.codeAttr.codeLength, cf->mm);
    if(verbose) {
        CF_u4 j;
        fprintf(stderr, "Code Attribute: %d %d %d\n", attr->type.codeAttr.maxStack, attr->type.codeAttr.maxLocals, attr->type.codeAttr.codeLength);
        fprintf(stderr, "Code Attribute: ");
        for(j = 0 ; j < attr->type.codeAttr.codeLength ; j++) {
            fprintf(stderr, "%02x ", attr->type.codeAttr.code[j]);
        }
        fprintf(stderr, "\n");
    }
    {
        CF_u2 length = read2bytes(fd);
        CF_ATTRIBUTE_ExceptionTable* entries = (CF_ATTRIBUTE_ExceptionTable*)allocateMem(cf->mm, sizeof(CF_ATTRIBUTE_ExceptionTable) * length);
        for(i = 0 ; i < length ; i++) {
            entries[i].startPc = read2bytes(fd);
            entries[i].endPc = read2bytes(fd);
            entries[i].handlerPc = read2bytes(fd);
            entries[i].catchType = read2bytes(fd);
            if(verbose) fprintf(stderr, "Code Attribute: %d %d %d %d\n", entries[i].startPc, entries[i].endPc, entries[i].handlerPc, entries[i].catchType);
        }
        attr->type.codeAttr.exceptionTableLength = length;
        attr->type.codeAttr.exceptionTable = entries;
    }
    {
        int count = read2bytes(fd);
        CF_Attribute* entries = (CF_Attribute*)allocateMem(cf->mm, sizeof(CF_Attribute) * count);
        if(verbose) fprintf(stderr, "Code Attributes: %d\n", count);
        for(i = 0 ; i < count ; i++) {
            if(!CFR_readAttribute(fd, cf, &entries[i])) return CFR_FALSE;
        }
        attr->type.codeAttr.attributesCount = count;
        attr->type.codeAttr.attributes = entries;
    }
    return CFR_TRUE;
}
CFR_bool CFR_readExceptionsAttribute(FILE* fd, ClassFile* cf, CF_Attribute* attr) {
    int i;
    CF_u2 number = read2bytes(fd);
    CF_u2* entries = (CF_u2*)allocateMem(cf->mm, sizeof(CF_u2) * number);
    if(verbose) fprintf(stderr, "Exceptions Attribute: %d\n", number);
    for(i = 0 ; i < number; i++) {
        entries[i] = read2bytes(fd);
        if(verbose) fprintf(stderr, "Exceptions Attribute: %d\n", entries[i]);
    }
    attr->attributeType = CF_ATTRIBUTE_ExceptionsType;
    attr->type.exceptionsAttr.number = number;
    attr->type.exceptionsAttr.exceptionIndexTable = entries;
    return CFR_TRUE;
}
CFR_bool CFR_readInnerClassesAttribute(FILE* fd, ClassFile* cf, CF_Attribute* attr) {
    int i;
    CF_u2 number = read2bytes(fd);
    CF_ATTRIBUTE_InnerClassesEntry* entries = (CF_ATTRIBUTE_InnerClassesEntry*)allocateMem(cf->mm, sizeof(CF_ATTRIBUTE_InnerClassesEntry) * number);
    if(verbose) fprintf(stderr, "InnerClasses Attribute: %d\n", number);
    for(i = 0 ; i < number ; i++) {
        entries[i].innerClassInfoIndex = read2bytes(fd);
        entries[i].outerClassInfoIndex = read2bytes(fd);
        entries[i].innerNameIndex = read2bytes(fd);
        entries[i].innerClassAccessFlags = read2bytes(fd);
        if(verbose) fprintf(stderr, "InnerClasses Attribute: %d %d %d %x\n", entries[i].innerClassInfoIndex, entries[i].outerClassInfoIndex, entries[i].innerNameIndex, entries[i].innerClassInfoIndex);
    }
    attr->attributeType = CF_ATTRIBUTE_InnerClassesType;
    attr->type.innerClassesAttr.number = number;
    attr->type.innerClassesAttr.entries = entries;
    return CFR_TRUE;
}
CFR_bool CFR_readSyntheticAttribute(FILE* fd, ClassFile* cf, CF_Attribute* attr) {
    assert(attr->attributeLength == 0);
    attr->attributeType = CF_ATTRIBUTE_SyntheticType;
    if(verbose) fprintf(stderr, "Synthetic Attribute:\n");
    if(attr->attributeLength != 0) return CFR_FALSE;
    return CFR_TRUE;
}
CFR_bool CFR_readSourceFileAttribute(FILE* fd, ClassFile* cf, CF_Attribute* attr) {
    CF_u2 idx = read2bytes(fd);
    attr->attributeType = CF_ATTRIBUTE_SourceFileType;
    attr->type.sourceFileAttr.sourceFileIndex = idx;
    if(verbose) fprintf(stderr, "SourceFile Attribute: %d\n", attr->type.sourceFileAttr.sourceFileIndex);
    //cf->sourceFile = &(cf->cp[idx].type.utf8Info);
    return CFR_TRUE;
}
CFR_bool CFR_readLineNumberTableAttribute(FILE* fd, ClassFile* cf, CF_Attribute* attr) {
    int i;
    CF_u2 length = read2bytes(fd);
    CF_ATTRIBUTE_LineNumberTableEntry* entries = (CF_ATTRIBUTE_LineNumberTableEntry*)allocateMem(cf->mm, sizeof(CF_ATTRIBUTE_LineNumberTableEntry) * length);
    if(verbose) fprintf(stderr, "LineNumber Table Attribute: %d\n", length);
    for(i = 0 ; i < length ; i++) {
        entries[i].startPc = read2bytes(fd);
        entries[i].lineNumber = read2bytes(fd);
        if(verbose) fprintf(stderr, "Line Number Table Attribute: %d %d\n", entries[i].startPc, entries[i].lineNumber);
    }
    attr->attributeType = CF_ATTRIBUTE_LineNumberTableType;
    attr->type.lineNumberTableAttr.length = length;
    attr->type.lineNumberTableAttr.entries = entries;
    return CFR_TRUE;
}
CFR_bool CFR_readLocalVariableTableAttribute(FILE* fd, ClassFile* cf, CF_Attribute* attr) {
    int i;
    CF_u2 length = read2bytes(fd);
    CF_ATTRIBUTE_LocalVariableTableEntry* entries = (CF_ATTRIBUTE_LocalVariableTableEntry*)allocateMem(cf->mm, sizeof(CF_ATTRIBUTE_LocalVariableTableEntry) * length);
    if(verbose) fprintf(stderr, "Local Variable Table Attribute: %d\n", length);
    for(i = 0 ; i < length ; i++) {
        entries[i].startPc = read2bytes(fd);
        entries[i].length = read2bytes(fd);
        entries[i].nameIndex = read2bytes(fd);
        entries[i].descriptorIndex = read2bytes(fd);
        entries[i].slot = read2bytes(fd);
        if(verbose) fprintf(stderr, "Local Variable Table Attribute: %d %d %d %d %d\n", entries[i].startPc, entries[i].length, entries[i].nameIndex, entries[i].descriptorIndex, entries[i].slot);
    }
    attr->attributeType = CF_ATTRIBUTE_LocalVariableTableType;
    attr->type.localVariableTableAttr.length = length;
    attr->type.localVariableTableAttr.entries = entries;
    return CFR_TRUE;
}
CFR_bool CFR_readDeprecatedAttribute(FILE* fd, ClassFile* cf, CF_Attribute* attr) {
    if(verbose) fprintf(stderr, "Deprecated Attribute:\n");
    assert(attr->attributeLength == 0);
    attr->attributeType = CF_ATTRIBUTE_DeprecatedType;
    if(attr->attributeLength != 0) return CFR_FALSE;
    return CFR_TRUE;
}
CFR_bool CFR_readOtherAttribute(FILE* fd, ClassFile* cf, CF_Attribute* attr) {
    attr->attributeType = CF_ATTRIBUTE_OtherType;
    attr->type.otherAttr.info = readBytes(fd, attr->attributeLength, cf->mm);
    if(verbose) {
        CF_u4 j;
        fprintf(stderr, "Other Attribute:");
        for(j = 0 ; j < attr->attributeLength ; j++) {
            fprintf(stderr, "%02x ", attr->type.otherAttr.info[j]);
        }
        fprintf(stderr, "\n");
    }
    return CFR_TRUE;
}

CF_u1 read1byte(FILE* fd) {
    CF_u1 byte;
    size_t size = fread(&byte,1,1,fd);
    return byte;
}

CF_u2 read2bytes(FILE* fd) {
    CF_u2 bytes;
    size_t size = fread(&bytes, 2, 1, fd);
    return toByte2(bytes);
}

CF_u4 read4bytes(FILE* fd) {
    CF_u4 bytes;
    size_t size = fread(&bytes, 4, 1, fd);
    return toByte4(bytes);
}

CF_u8 read8bytes(FILE* fd) {
    CF_u8 bytes;
    size_t size = fread(&bytes, 8, 1, fd);
    return toByte8((CF_u8)bytes);
}

CF_u1* readBytes(FILE* fd, int length, MemoryManager* mm) {
    CF_u1* bytes = (CF_u1*)allocateMem(mm, length+1);
    size_t size = fread(bytes, 1, length, fd);
    bytes[length] = '\0';
    return bytes;
}
