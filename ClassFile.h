#ifndef TRANSLATOR_CLASS_FILE_H
#define TRANSLATOR_CLASS_FILE_H

#include "Common.h"
#include "MemoryManager.h"

#define CF_Interface CF_u2

typedef unsigned char   CF_u1;
typedef unsigned short  CF_u2;
typedef unsigned int    CF_u4;
typedef unsigned long long  CF_u8;
typedef double CF_f8;


#define CF_ACC_PUBLIC  0x0001
#define CF_ACC_PRIVATE  0x0002
#define CF_ACC_PROTECTED  0x0004
#define CF_ACC_STATIC  0x0008
#define CF_ACC_FINAL  0x0010
#define CF_ACC_SUPER  0x0020
#define CF_ACC_SYNCRONIZED  0x0020
#define CF_ACC_VOLATILE  0x0040
#define CF_ACC_TRANSIENT  0x0080
#define CF_ACC_NATIVE  0x0100
#define CF_ACC_INTERFACE  0x0200
#define CF_ACC_ABSTRACT  0x0400

#define CF_CONSTANT_Utf8Type 1
#define CF_CONSTANT_IntegerType 3
#define CF_CONSTANT_FloatType 4
#define CF_CONSTANT_LongType 5
#define CF_CONSTANT_DoubleType 6
#define CF_CONSTANT_ClassrefType 7
#define CF_CONSTANT_StringType 8
#define CF_CONSTANT_FieldrefType 9
#define CF_CONSTANT_MethodrefType 10
#define CF_CONSTANT_InterfaceMethodrefType 11
#define CF_CONSTANT_NameAndTypeType 12

#define CF_ATTRIBUTE_ConstantValueType 1
#define CF_ATTRIBUTE_CodeType 2
#define CF_ATTRIBUTE_ExceptionsType 3
#define CF_ATTRIBUTE_InnerClassesType 4
#define CF_ATTRIBUTE_SyntheticType 5
#define CF_ATTRIBUTE_SourceFileType 6
#define CF_ATTRIBUTE_LineNumberTableType 7
#define CF_ATTRIBUTE_LocalVariableTableType 8
#define CF_ATTRIBUTE_DeprecatedType 9
#define CF_ATTRIBUTE_OtherType 10

typedef struct{
    CF_u2 nameIndex;
}CF_CONSTANT_ClassInfo;

typedef struct {
    CF_u2 classIndex;
    CF_u2 nameAndTypeIndex;
} CF_CONSTANT_FieldrefInfo, CF_CONSTANT_MethodrefInfo, CF_CONSTANT_InterfaceMethodrefInfo;

typedef struct {
    CF_u2 stringIndex;
}CF_CONSTANT_StringInfo;

typedef struct {
    CF_u4 bytes;
}CF_CONSTANT_IntegerInfo, CF_CONSTANT_FloatInfo;

typedef struct {
    CF_u8 bytes;
}CF_CONSTANT_LongInfo, CF_CONSTANT_DoubleInfo;

typedef struct {
    CF_u2 nameIndex;
    CF_u2 descriptorIndex;
}CF_CONSTANT_NameAndTypeInfo;

typedef struct {
    CF_u2 length;
    CF_u1* bytes;
}CF_CONSTANT_Utf8Info;

typedef struct ConstantPool {
    CF_u1 tag;
    union {
        CF_CONSTANT_Utf8Info utf8Info;
        CF_CONSTANT_IntegerInfo integerInfo;
        CF_CONSTANT_FloatInfo floatInfo;
        CF_CONSTANT_LongInfo longInfo;
        CF_CONSTANT_DoubleInfo doubleInfo;
        CF_CONSTANT_StringInfo stringInfo;
        CF_CONSTANT_ClassInfo classInfo;
        CF_CONSTANT_FieldrefInfo fieldInfo;
        CF_CONSTANT_MethodrefInfo methodInfo;
        CF_CONSTANT_InterfaceMethodrefInfo interfaceInfo;
        CF_CONSTANT_NameAndTypeInfo nameAndTypeInfo;
    } type;
}CF_ConstantPool;

typedef struct {
    CF_u2 constantValueIndex;
}CF_ATTRIBUTE_ConstantValue;

typedef struct {
    CF_u2 startPc;
    CF_u2 endPc;
    CF_u2 handlerPc;
    CF_u2 catchType;
}CF_ATTRIBUTE_ExceptionTable;

typedef struct {
    CF_u2 maxStack;
    CF_u2 maxLocals;
    CF_u4 codeLength;
    CF_u1* code;
    CF_u2 exceptionTableLength;
    CF_ATTRIBUTE_ExceptionTable* exceptionTable;
    CF_u2 attributesCount;
    struct CF_Attribute* attributes;
}CF_ATTRIBUTE_Code;

typedef struct {
    CF_u2 number;
    CF_u2* exceptionIndexTable;
}CF_ATTRIBUTE_Exceptions;

typedef struct {
    CF_u2 innerClassInfoIndex;
    CF_u2 outerClassInfoIndex;
    CF_u2 innerNameIndex;
    CF_u2 innerClassAccessFlags;
}CF_ATTRIBUTE_InnerClassesEntry;

typedef struct {
    CF_u2 number;
    CF_ATTRIBUTE_InnerClassesEntry* entries;
}CF_ATTRIBUTE_InnerClasses;

typedef struct {
    CF_u2 sourceFileIndex;
}CF_ATTRIBUTE_SourceFile;

typedef struct {
    CF_u2 startPc;
    CF_u2 lineNumber;
}CF_ATTRIBUTE_LineNumberTableEntry;

typedef struct {
    CF_u2 length;
    CF_ATTRIBUTE_LineNumberTableEntry* entries;
}CF_ATTRIBUTE_LineNumberTable;

typedef struct {
    CF_u2 startPc;
    CF_u2 length;
    CF_u2 nameIndex;
    CF_u2 descriptorIndex;
    CF_u2 slot;
}CF_ATTRIBUTE_LocalVariableTableEntry;

typedef struct {
    CF_u2 length;
    CF_ATTRIBUTE_LocalVariableTableEntry* entries;
}CF_ATTRIBUTE_LocalVariableTable;

typedef struct {
}CF_ATTRIBUTE_Deprecated, CF_ATTRIBUTE_Synthetic;

typedef struct {
    CF_u1* info;
}CF_ATTRIBUTE_Other;

typedef struct CF_Attribute{
    CF_u2 attributeNameIndex;
    CF_u4 attributeLength;
    CF_u4 attributeType;
    union {
        CF_ATTRIBUTE_ConstantValue constantValueAttr;
        CF_ATTRIBUTE_Code codeAttr;
        CF_ATTRIBUTE_Exceptions exceptionsAttr;
        CF_ATTRIBUTE_InnerClasses innerClassesAttr;
        CF_ATTRIBUTE_Synthetic syntheticAttr;
        CF_ATTRIBUTE_SourceFile sourceFileAttr;
        CF_ATTRIBUTE_LineNumberTable lineNumberTableAttr;
        CF_ATTRIBUTE_LocalVariableTable localVariableTableAttr;
        CF_ATTRIBUTE_Deprecated deprecatedAttr;
        CF_ATTRIBUTE_Other otherAttr;
    } type;
}CF_Attribute;

typedef struct{
    CF_u2 accessFlags;
    CF_u2 nameIndex;
    CF_u2 descriptorIndex;
    CF_u2 attributesCount;
    CF_Attribute* attributes;
    CF_ATTRIBUTE_Code codeAttr;
}CF_Field, CF_Method;

typedef struct ClassFile {
    //CF_CONSTANT_Utf8Info* sourceFile;
    CF_u4 magic;
    CF_u2 minorVersion;
    CF_u2 majorVersion;

    CF_u2 constantPoolCount;
    CF_ConstantPool* cp;

    CF_u2 accessFlags;
    CF_u2 thisClass;
    CF_u2 superClass;

    CF_u2 interfacesCount;
    CF_Interface* interfaces;

    CF_u2 fieldsCount;
    CF_Field* fields;

    CF_u2 methodsCount;
    CF_Method* methods;

    CF_u2 attributesCount;
    CF_Attribute* attributes;
    MemoryManager* mm;

    CF_u1* fileName;

}ClassFile;

#define CF_NOP 0x00
#define CF_ACONST_NULL 0x01
#define CF_ICONST_M1 0x02
#define CF_ICONST_0 0x03
#define CF_ICONST_1 0x04
#define CF_ICONST_2 0x05
#define CF_ICONST_3 0x06
#define CF_ICONST_4 0x07
#define CF_ICONST_5 0x08
#define CF_LCONST_0 0x09
#define CF_LCONST_1 0x0a
#define CF_FCONST_0 0x0b
#define CF_FCONST_1 0x0c
#define CF_FCONST_2 0x0d
#define CF_DCONST_0 0x0e
#define CF_DCONST_1 0x0f
#define CF_BIPUSH 0x10
#define CF_SIPUSH 0x11
#define CF_LDC 0x12
#define CF_LDC_W 0x13
#define CF_LDC2_W 0x14
#define CF_ILOAD 0x15
#define CF_LLOAD 0x16
#define CF_FLOAD 0x17
#define CF_DLOAD 0x18
#define CF_ALOAD 0x19
#define CF_ILOAD_0 0x1a
#define CF_ILOAD_1 0x1b
#define CF_ILOAD_2 0x1c
#define CF_ILOAD_3 0x1d
#define CF_LLOAD_0 0x1e
#define CF_LLOAD_1 0x1f
#define CF_LLOAD_2 0x20
#define CF_LLOAD_3 0x21
#define CF_FLOAD_0 0x22
#define CF_FLOAD_1 0x23
#define CF_FLOAD_2 0x24
#define CF_FLOAD_3 0x25
#define CF_DLOAD_0 0x26
#define CF_DLOAD_1 0x27
#define CF_DLOAD_2 0x28
#define CF_DLOAD_3 0x29
#define CF_ALOAD_0 0x2a
#define CF_ALOAD_1 0x2b
#define CF_ALOAD_2 0x2c
#define CF_ALOAD_3 0x2d
#define CF_IALOAD 0x2e
#define CF_LALOAD 0x2f
#define CF_FALOAD 0x30
#define CF_DALOAD 0x31
#define CF_AALOAD 0x32
#define CF_BALOAD 0x33
#define CF_CALOAD 0x34
#define CF_SALOAD 0x35
#define CF_ISTORE 0x36
#define CF_LSTORE 0x37
#define CF_FSTORE 0x38
#define CF_DSTORE 0x39
#define CF_ASTORE 0x3a
#define CF_ISTORE_0 0x3b
#define CF_ISTORE_1 0x3c
#define CF_ISTORE_2 0x3d
#define CF_ISTORE_3 0x3e
#define CF_LSTORE_0 0x3f
#define CF_LSTORE_1 0x40
#define CF_LSTORE_2 0x41
#define CF_LSTORE_3 0x42
#define CF_FSTORE_0 0x43
#define CF_FSTORE_1 0x44
#define CF_FSTORE_2 0x45
#define CF_FSTORE_3 0x46
#define CF_DSTORE_0 0x47
#define CF_DSTORE_1 0x48
#define CF_DSTORE_2 0x49
#define CF_DSTORE_3 0x4a
#define CF_ASTORE_0 0x4b
#define CF_ASTORE_1 0x4c
#define CF_ASTORE_2 0x4d
#define CF_ASTORE_3 0x4e
#define CF_IASTORE 0x4f
#define CF_LASTORE 0x50
#define CF_FASTORE 0x51
#define CF_DASTORE 0x52
#define CF_AASTORE 0x53
#define CF_BASTORE 0x54
#define CF_CASTORE 0x55
#define CF_SASTORE 0x56
#define CF_POP 0x57
#define CF_POP2 0x58
#define CF_DUP 0x59
#define CF_DUP_X1 0x5a
#define CF_DUP_X2 0x5b
#define CF_DUP2 0x5c
#define CF_DUP2_X1 0x5d
#define CF_DUP2_X2 0x5e
#define CF_SWAP 0x5f
#define CF_IADD 0x60
#define CF_LADD 0x61
#define CF_FADD 0x62
#define CF_DADD 0x63
#define CF_ISUB 0x64
#define CF_LSUB 0x65
#define CF_FSUB 0x66
#define CF_DSUB 0x67
#define CF_IMUL 0x68
#define CF_LMUL 0x69
#define CF_FMUL 0x6a
#define CF_DMUL 0x6b
#define CF_IDIV 0x6c
#define CF_LDIV 0x6d
#define CF_FDIV 0x6e
#define CF_DDIV 0x6f
#define CF_IREM 0x70
#define CF_LREM 0x71
#define CF_FREM 0x72
#define CF_DREM 0x73
#define CF_INEG 0x74
#define CF_LNEG 0x75
#define CF_FNEG 0x76
#define CF_DNEG 0x77
#define CF_ISHL 0x78
#define CF_LSHL 0x79
#define CF_ISHR 0x7a
#define CF_LSHR 0x7b
#define CF_IUSHR 0x7c
#define CF_LUSHR 0x7d
#define CF_IAND 0x7e
#define CF_LAND 0x7f
#define CF_IOR 0x80
#define CF_LOR 0x81
#define CF_IXOR 0x82
#define CF_LXOR 0x83
#define CF_IINC 0x84
#define CF_I2L 0x85
#define CF_I2F 0x86
#define CF_I2D 0x87
#define CF_L2I 0x88
#define CF_L2F 0x89
#define CF_L2D 0x8a
#define CF_F2I 0x8b
#define CF_F2L 0x8c
#define CF_F2D 0x8d
#define CF_D2I 0x8e
#define CF_D2L 0x8f
#define CF_D2F 0x90
#define CF_I2B 0x91
#define CF_I2C 0x92
#define CF_I2S 0x93
#define CF_LCMP 0x94
#define CF_FCMPL 0x95
#define CF_FCMPG 0x96
#define CF_DCMPL 0x97
#define CF_DCMPG 0x98
#define CF_IFEQ 0x99
#define CF_IFNE 0x9a
#define CF_IFLT 0x9b
#define CF_IFGE 0x9c
#define CF_IFGT 0x9d
#define CF_IFLE 0x9e
#define CF_IF_ICMPEQ 0x9f
#define CF_IF_ICMPNE 0xa0
#define CF_IF_ICMPLT 0xa1
#define CF_IF_ICMPGE 0xa2
#define CF_IF_ICMPGT 0xa3
#define CF_IF_ICMPLE 0xa4
#define CF_IF_ACMPEQ 0xa5
#define CF_IF_ACMPNE 0xa6
#define CF_GOTO 0xa7
#define CF_JSR 0xa8
#define CF_RET 0xa9
#define CF_TABLESWITCH 0xaa
#define CF_LOOKUPSWITCH 0xab
#define CF_IRETURN 0xac
#define CF_LRETURN 0xad
#define CF_FRETURN 0xae
#define CF_DRETURN 0xaf
#define CF_ARETURN 0xb0
#define CF_RETURN 0xb1
#define CF_GETSTATIC 0xb2
#define CF_PUTSTATIC 0xb3
#define CF_GETFIELD 0xb4
#define CF_PUTFIELD 0xb5
#define CF_INVOKEVIRTUAL 0xb6
#define CF_INVOKESPECIAL 0xb7
#define CF_INVOKESTATIC 0xb8
#define CF_INVOKEINTERFACE 0xb9
#define CF_NEW 0xbb
#define CF_NEWARRAY 0xbc
#define CF_ANEWARRAY 0xbd
#define CF_ARRAYLENGTH 0xbe
#define CF_ATHROW 0xbf
#define CF_CHECKCAST 0xc0
#define CF_INSTANCEOF 0xc1
#define CF_MONITORENTER 0xc2
#define CF_MONITOREXIT 0xc3
#define CF_WIDE 0xc4
#define CF_MULTIANEWARRAY 0xc5
#define CF_IFNULL 0xc6
#define CF_IFNONNULL 0xc7
#define CF_GOTO_W 0xc8
#define CF_JSR_W 0xc9

#define CF_makeShortVal(a, b) ((CF_u2)((a << 8)|(b&0x00ff))) 
#define CF_makeIntVal(a, b, c, d) (CF_u4)((((CF_u4)CF_makeShortVal(a, b)) << 16) | (CF_u4)(CF_makeShortVal(c, d) & 0x0000ffff))

CF_u1 CF_opcLengths[256];
CF_u1 CF_getBytecodeLength(CF_u1* code, CF_u4 bpc);
#endif
