#ifndef TRANSLATOR_DEX_FORMAT_H
#define TRANSLATOR_DEX_FORMAT_H
#include "Common.h"

typedef unsigned char   DF_u1;
typedef unsigned short  DF_u2;
typedef unsigned int    DF_u4;
typedef unsigned long long  DF_u8;
typedef double DF_f8;

typedef char   DF_s1;
typedef short  DF_s2;
typedef int    DF_s4;
typedef long long  DF_s8;

typedef unsigned int DF_uleb128;
typedef signed int DF_sleb128;

extern DF_u4 getUleb128Size(DF_uleb128 value);
// MAP LIST TYPE
#define DF_MAP_HEADER_ITEM  0x0000, 
#define DF_MAP_STRING_ID_ITEM  0x0001,
#define DF_MAP_TYPE_ID_ITEM  0x0002,
#define DF_MAP_PROTO_ID_ITEM  0x0003,
#define DF_MAP_FIELD_ID_ITEM  0x0004,
#define DF_MAP_METHOD_ID_ITEM  0x0005,
#define DF_MAP_CLASS_DEF_ITEM  0x0006
#define DF_MAP_MAP_LIST  0x1000,
#define DF_MAP_TYPE_LIST  0x1001,
//#define DF_MAP_ANNOTATION_SET_REF_LIST  0x1002,
//#define DF_MAP_ANNOIATION_SET_ITEM  0x1003,
#define DF_MAP_CLASS_DATA_ITEM  0x2000,
#define DF_MAP_CODE_ITEM  0x2001,
#define DF_MAP_STRING_DATA_ITEM  0x2002,
//#define DF_MAP_DEBUG_INFO_ITEM  0x2003,
//#define DF_MAP_ANNOTATION_ITEM  0x2004,
//#define DF_MAP_ENCODED_ARRAY_ITEM  0x2005,
//#define DF_MAP_ANNOTATION_DIRECTORY_ITEM  0x2006

// ACCESS FLAGS
#define DF_ACC_PUBLIC 0x1
#define DF_ACC_PRIVATE 0x2
#define DF_ACC_PROTECTED 0x4
#define DF_ACC_STATIC 0x8
#define DF_ACC_FINAL 0x10
#define DF_ACC_SYNCHRONIZED 0x20
#define DF_ACC_VOLATILE 0x40
#define DF_ACC_BRIDGE 0x40
#define DF_ACC_TRANSIENT 0x80
#define DF_ACC_VARARGS 0x80
#define DF_ACC_NATIVE 0x100
#define DF_ACC_INTERFACE 0x200
#define DF_ACC_ABSTRACT 0x400
#define DF_ACC_STRICT 0x800
#define DF_ACC_SYNTHETIC 0x1000
#define DF_ACC_ANNOTATION 0x2000
#define DF_ACC_ENUM 0x4000
#define DF_ACC_CONSTRUCTOR 0x10000
#define DF_ACC_DECLARED_SYNCHRONIZED 0x20000

/*
#define DF_writeShortCodeV1(dcodes, idx, val) ((dcodes)[idx] = val)
#define DF_writeShortCodeV2(dcodes, idx, val1, val2) ((dcodes)[idx] = ( ((val2) << 8) | ((val1) & 0x00ff)))
#define DF_writeShortCodeV3(dcodes, idx, val1, val2, val3) ((dcodes)[idx] = ( (((val3) << 4 | (val2)) << 8) | ((val1) & 0x00ff)))
*/
#define DF_writeShortCode(dcodes, idx, val) ((dcodes)[idx] = val)
#define DF_write2BytesCode(dcodes, idx, val1, val2) ((dcodes)[idx] = (((val2) << 8) | ((val1) & 0x00ff)));
#define DF_writeIntCode(dcodes, idx, val) DF_writeShortCode(dcodes, idx, val & 0xffff); DF_writeShortCode(dcodes, idx + 1, val >> 16)

#define DF_writeIntData(bytes, idx, val) DF_writeShortData(bytes, idx, val); DF_writeShortData(bytes, (idx + 2), (val >> 16));
#define DF_writeShortData(bytes, idx, val) (bytes)[(idx)] = ((val) & 0x00ff); (bytes)[(idx)+1] = (((val) >> 8) & 0x00ff);
#define DF_writeByteData(bytes, idx, val) (bytes)[idx] = ((val) & 0x00ff); 
extern DF_u4 DF_writeUleb128Data(DF_u1* bytes, DF_u4 idx, DF_uleb128 val);
extern DF_u4 DF_writeSleb128Data(DF_u1* bytes, DF_u4 idx, DF_sleb128 val);

#define DF_align4bytes(offset) if((offset % 4) != 0) offset = offset + (4 - (offset % 4));
/*
typedef struct {
    DF_u1  magic[8];
    DF_u4  checksum;
    DF_u1  signature[kSHA1DigestLen];
    DF_u4  fileSize;
    DF_u4  headerSize;
    DF_u4  endianTag;
    DF_u4  linkSize;
    DF_u4  linkOff;
    DF_u4  mapOff;
    DF_u4  stringIdsSize;
    DF_u4  stringIdsOff;
    DF_u4  typeIdsSize;
    DF_u4  typeIdsOff;
    DF_u4  protoIdsSize;
    DF_u4  protoIdsOff;
    DF_u4  fieldIdsSize;
    DF_u4  fieldIdsOff;
    DF_u4  methodIdsSize;
    DF_u4  methodIdsOff;
    DF_u4  classDefsSize;
    DF_u4  classDefsOff;
    DF_u4  dataSize;
    DF_u4  dataOff;
}DF_Header;
*/

typedef struct {
    DF_u4  stringDataOff;
}DF_ID_String;

typedef struct {
    DF_u4  descriptorIdx;
}DF_ID_Type;

typedef struct {
    DF_u4  shortyIdx;
    DF_u4  returnTypeIdx;
    DF_s8  parametersOff;
}DF_ID_Proto;

typedef struct {
    DF_u2  classIdx;
    DF_u2  typeIdx;
    DF_u4  nameIdx;
}DF_ID_Field;

typedef struct {
    DF_u2  classIdx;
    DF_u2  protoIdx;
    DF_u4  nameIdx;
}DF_ID_Method;

typedef struct {
    DF_u4  classIdx;
    DF_u4  accessFlags;
    DF_u4  superClassIdx;
    DF_u4  interfacesOff;
    DF_u4  sourceFileIdx;
    DF_u4  annotationsOff;
    DF_u4  classDataOff;
    DF_u4  staticValuesOff;
}DF_DEF_Class;

/*
typedef struct {
    DF_u2 type;
    //DF_u2 unused;
    DF_u4 size;
    DF_u4 offset;
}DF_DATA_Map;
*/
/*
typedef struct {
    DF_u4 size;
    DF_DATA_Map* list;
}DF_DATA_MapList;
*/
typedef struct {
    DF_uleb128 size;
    DF_u4 addr;
    DF_u1* data;
}DF_DATA_String;

typedef struct {
    DF_uleb128 idxDiff;
    DF_uleb128 accessFlags;
}DF_DATA_Field;

typedef struct {
    DF_uleb128 idxDiff;
    DF_uleb128 accessFlags;
    DF_sleb128 codeOff;
    DF_uleb128 addr;
}DF_DATA_Method;

typedef struct {
    DF_uleb128 staticFieldsSize;
    DF_uleb128 instanceFieldsSize;
    DF_uleb128 directMethodsSize;
    DF_uleb128 virtualMethodsSize;
    DF_DATA_Field* staticFields;
    DF_DATA_Field* instanceFields;
    DF_DATA_Method* directMethods;
    DF_DATA_Method* virtualMethods;
}DF_DATA_Class;

typedef struct {
    DF_u2 typeIdx;
}DF_DATA_Type;

typedef struct {
    DF_u4 size;
    DF_u4 addr;
    DF_DATA_Type* typeItems;
}DF_DATA_TypeList;


typedef struct {
    DF_u4 startAddr;
    DF_u2 insnCount;
    DF_u2 handlerOff;
}DF_DATA_Try;

typedef struct {
    DF_uleb128 typeIdx;
    DF_uleb128 addr;
}DF_DATA_TypeAddrPair;

typedef struct {
    DF_sleb128 size;
    DF_DATA_TypeAddrPair* pairs;
    DF_uleb128 catchAllAddr;
    DF_u2 offset;
}DF_DATA_Handler;

typedef struct {
    DF_uleb128 size;
    DF_DATA_Handler* list;
}DF_DATA_HandlerList;

typedef struct {
    DF_u4 methodIdx;
    DF_u2 registersSize;
    DF_u2 insSize;
    DF_u2 insCount;
    DF_u2 outsSize;
    DF_u2 triesSize;
    DF_u4 debugInfoOff;
    DF_u4 insnsSize;
    DF_u2* insns;
    //DF_u2 padding;
    DF_DATA_Try* tries;
    DF_DATA_HandlerList handlers;
    struct DF_IR_BB** cfg;
    DF_u1 hasWide;
    DF_u1 needTemp;
}DF_DATA_Code;
//typedef struct { ... } DF_DATA_DebugInfo;

//typedef struct { ... } DF_DATA_Link;
typedef struct {
    //DF_Header* header;
    DF_u4 mapOff;
    DF_u4 mapSize;
    /*
    DF_u4  stringIdsSize;
    DF_u4  stringIdsLen;
    DF_u4  stringIdsOff;
    DF_ID_String* stringIds;
    */
    DF_u4  stringIdsOff;

    DF_u4  typeIdsSize;
    DF_u4  typeIdsOff;
    DF_ID_Type* typeIds;
    
    DF_u4  protoIdsSize;
    DF_u4  protoIdsOff;
    DF_ID_Proto* protoIds;

    DF_u4  fieldIdsSize;
    DF_u4  fieldIdsOff;
    DF_ID_Field* fieldIds;
    
    DF_u4  methodIdsSize;
    DF_u4  methodIdsOff;
    DF_ID_Method* methodIds;

    DF_u4  classDefsSize;
    DF_u4  classDefsOff;
    DF_DEF_Class* classDefs;

    DF_u4  stringItemsSize;
    DF_u4  stringItemsLen;
    DF_u4  stringItemsOff;
    DF_DATA_String* stringItems;
    
    // DF_DATA_DebugInfo* debugInfos;
    
    DF_u4  codeItemsSize;
    DF_u4  codeItemsOff;
    DF_u4  codeItemsLen;
    DF_DATA_Code* codeItems;
    
    DF_u4 typeListSize;
    DF_u4 typeListOff;
    DF_u4 typeListLen;
    DF_DATA_TypeList* typeList;

    DF_u4  classItemsSize;
    DF_u4  classItemsOff;
    DF_u4  classItemsLen;
    DF_DATA_Class* classItems;
    
    DF_u4  dataSize;
    DF_u4  dataOff;
    DF_u4  fileSize;
    DF_u1* bytes;
    
    DF_u4  debugOff;
    MemoryManager* mm;

    DF_s4  interfacesOff;

    DF_u1* fileName;
}DexFormat;

#define DF_NOP 0x00
#define DF_MOVE 0x01
#define DF_MOVE_FROM16 0x02
#define DF_MOVE_16 0x03
#define DF_MOVE_WIDE 0x04
#define DF_MOVE_WIDE_FROM16 0x05
#define DF_MOVE_WIDE_16 0x06
#define DF_MOVE_OBJECT 0x07
#define DF_MOVE_OBJECT_FROM16 0x08
#define DF_MOVE_OBJECT_16 0x09
#define DF_MOVE_RESULT 0x0a
#define DF_MOVE_RESULT_WIDE 0x0b
#define DF_MOVE_RESULT_OBJECT 0x0c
#define DF_MOVE_EXCEPTION 0x0d
#define DF_RETURN_VOID 0x0e
#define DF_RETURN 0x0f
#define DF_RETURN_WIDE 0x10
#define DF_RETURN_OBJECT 0x11
#define DF_CONST_4 0x12
#define DF_CONST_16 0x13
#define DF_CONST 0x14
#define DF_CONST_HIGH16 0x15
#define DF_CONST_WIDE_16 0x16
#define DF_CONST_WIDE_32 0x17
#define DF_CONST_WIDE 0x18
#define DF_CONST_WIDE_HIGH16 0x19
#define DF_CONST_STRING 0x1a
#define DF_CONST_STRING_JUMBO 0x1b
#define DF_CONST_CLASS 0x1c
#define DF_MONITOR_ENTER 0x1d
#define DF_MONITOR_EXIT 0x1e
#define DF_CHECK_CAST 0x1f
#define DF_INSTANCE_OF 0x20
#define DF_ARRAY_LENGTH 0x21
#define DF_NEW_INSTANCE 0x22
#define DF_NEW_ARRAY 0x23
#define DF_FILLED_NEW_ARRAY 0x24
#define DF_FILLED_NEW_ARRAY_RANGE 0x25
#define DF_FILL_ARRAY_DATA 0x26
#define DF_THROW 0x27
#define DF_GOTO 0x28
#define DF_GOTO_16 0x29
#define DF_GOTO_32 0x2a
#define DF_PACKED_SWITCH 0x2b
#define DF_SPARSE_SWITCH 0x2c
#define DF_CMPL_FLOAT 0x2d
#define DF_CMPG_FLOAT 0x2e
#define DF_CMPL_DOUBLE 0x2f
#define DF_CMPG_DOUBLE 0x30
#define DF_CMP_LONG 0x31
#define DF_IF_EQ 0x32
#define DF_IF_NE 0x33
#define DF_IF_LT 0x34
#define DF_IF_GE 0x35
#define DF_IF_GT 0x36
#define DF_IF_LE 0x37
#define DF_IF_EQZ 0x38
#define DF_IF_NEZ 0x39
#define DF_IF_LTZ 0x3a
#define DF_IF_GEZ 0x3b
#define DF_IF_GTZ 0x3c
#define DF_IF_LEZ 0x3d
#define DF_UNUSED_3E 0x3e
#define DF_UNUSED_3F 0x3f
#define DF_UNUSED_40 0x40
#define DF_UNUSED_41 0x41
#define DF_UNUSED_42 0x42
#define DF_UNUSED_43 0x43
#define DF_AGET 0x44
#define DF_AGET_WIDE 0x45
#define DF_AGET_OBJECT 0x46
#define DF_AGET_BOOLEAN 0x47
#define DF_AGET_BYTE 0x48
#define DF_AGET_CHAR 0x49
#define DF_AGET_SHORT 0x4a
#define DF_APUT 0x4b
#define DF_APUT_WIDE 0x4c
#define DF_APUT_OBJECT 0x4d
#define DF_APUT_BOOLEAN 0x4e
#define DF_APUT_BYTE 0x4f
#define DF_APUT_CHAR 0x50
#define DF_APUT_SHORT 0x51
#define DF_IGET 0x52
#define DF_IGET_WIDE 0x53
#define DF_IGET_OBJECT 0x54
#define DF_IGET_BOOLEAN 0x55
#define DF_IGET_BYTE 0x56
#define DF_IGET_CHAR 0x57
#define DF_IGET_SHORT 0x58
#define DF_IPUT 0x59
#define DF_IPUT_WIDE 0x5a
#define DF_IPUT_OBJECT 0x5b
#define DF_IPUT_BOOLEAN 0x5c
#define DF_IPUT_BYTE 0x5d
#define DF_IPUT_CHAR 0x5e
#define DF_IPUT_SHORT 0x5f
#define DF_SGET 0x60
#define DF_SGET_WIDE 0x61
#define DF_SGET_OBJECT 0x62
#define DF_SGET_BOOLEAN 0x63
#define DF_SGET_BYTE 0x64
#define DF_SGET_CHAR 0x65
#define DF_SGET_SHORT 0x66
#define DF_SPUT 0x67
#define DF_SPUT_WIDE 0x68
#define DF_SPUT_OBJECT 0x69
#define DF_SPUT_BOOLEAN 0x6a
#define DF_SPUT_BYTE 0x6b
#define DF_SPUT_CHAR 0x6c
#define DF_SPUT_SHORT 0x6d
#define DF_INVOKE_VIRTUAL 0x6e
#define DF_INVOKE_SUPER 0x6f
#define DF_INVOKE_DIRECT 0x70
#define DF_INVOKE_STATIC 0x71
#define DF_INVOKE_INTERFACE 0x72
#define DF_UNUSED_73 0x73
#define DF_INVOKE_VIRTUAL_RANGE 0x74
#define DF_INVOKE_SUPER_RANGE 0x75
#define DF_INVOKE_DIRECT_RANGE 0x76
#define DF_INVOKE_STATIC_RANGE 0x77
#define DF_INVOKE_INTERFACE_RANGE 0x78
#define DF_UNUSED_79 0x79
#define DF_UNUSED_7A 0x7a
#define DF_NEG_INT 0x7b
#define DF_NOT_INT 0x7c
#define DF_NEG_LONG 0x7d
#define DF_NOT_LONG 0x7e
#define DF_NEG_FLOAT 0x7f
#define DF_NEG_DOUBLE 0x80
#define DF_INT_TO_LONG 0x81
#define DF_INT_TO_FLOAT 0x82
#define DF_INT_TO_DOUBLE 0x83
#define DF_LONG_TO_INT 0x84
#define DF_LONG_TO_FLOAT 0x85
#define DF_LONG_TO_DOUBLE 0x86
#define DF_FLOAT_TO_INT 0x87
#define DF_FLOAT_TO_LONG 0x88
#define DF_FLOAT_TO_DOUBLE 0x89
#define DF_DOUBLE_TO_INT 0x8a
#define DF_DOUBLE_TO_LONG 0x8b
#define DF_DOUBLE_TO_FLOAT 0x8c
#define DF_INT_TO_BYTE 0x8d
#define DF_INT_TO_CHAR 0x8e
#define DF_INT_TO_SHORT 0x8f
#define DF_ADD_INT 0x90
#define DF_SUB_INT 0x91
#define DF_MUL_INT 0x92
#define DF_DIV_INT 0x93
#define DF_REM_INT 0x94
#define DF_AND_INT 0x95
#define DF_OR_INT 0x96
#define DF_XOR_INT 0x97
#define DF_SHL_INT 0x98
#define DF_SHR_INT 0x99
#define DF_USHR_INT 0x9a
#define DF_ADD_LONG 0x9b
#define DF_SUB_LONG 0x9c
#define DF_MUL_LONG 0x9d
#define DF_DIV_LONG 0x9e
#define DF_REM_LONG 0x9f
#define DF_AND_LONG 0xa0
#define DF_OR_LONG 0xa1
#define DF_XOR_LONG 0xa2
#define DF_SHL_LONG 0xa3
#define DF_SHR_LONG 0xa4
#define DF_USHR_LONG 0xa5
#define DF_ADD_FLOAT 0xa6
#define DF_SUB_FLOAT 0xa7
#define DF_MUL_FLOAT 0xa8
#define DF_DIV_FLOAT 0xa9
#define DF_REM_FLOAT 0xaa
#define DF_ADD_DOUBLE 0xab
#define DF_SUB_DOUBLE 0xac
#define DF_MUL_DOUBLE 0xad
#define DF_DIV_DOUBLE 0xae
#define DF_REM_DOUBLE 0xaf
#define DF_ADD_INT_2ADDR 0xb0
#define DF_SUB_INT_2ADDR 0xb1
#define DF_MUL_INT_2ADDR 0xb2
#define DF_DIV_INT_2ADDR 0xb3
#define DF_REM_INT_2ADDR 0xb4
#define DF_AND_INT_2ADDR 0xb5
#define DF_OR_INT_2ADDR 0xb6
#define DF_XOR_INT_2ADDR 0xb7
#define DF_SHL_INT_2ADDR 0xb8
#define DF_SHR_INT_2ADDR 0xb9
#define DF_USHR_INT_2ADDR 0xba
#define DF_ADD_LONG_2ADDR 0xbb
#define DF_SUB_LONG_2ADDR 0xbc
#define DF_MUL_LONG_2ADDR 0xbd
#define DF_DIV_LONG_2ADDR 0xbe
#define DF_REM_LONG_2ADDR 0xbf
#define DF_AND_LONG_2ADDR 0xc0
#define DF_OR_LONG_2ADDR 0xc1
#define DF_XOR_LONG_2ADDR 0xc2
#define DF_SHL_LONG_2ADDR 0xc3
#define DF_SHR_LONG_2ADDR 0xc4
#define DF_USHR_LONG_2ADDR 0xc5
#define DF_ADD_FLOAT_2ADDR 0xc6
#define DF_SUB_FLOAT_2ADDR 0xc7
#define DF_MUL_FLOAT_2ADDR 0xc8
#define DF_DIV_FLOAT_2ADDR 0xc9
#define DF_REM_FLOAT_2ADDR 0xca
#define DF_ADD_DOUBLE_2ADDR 0xcb
#define DF_SUB_DOUBLE_2ADDR 0xcc
#define DF_MUL_DOUBLE_2ADDR 0xcd
#define DF_DIV_DOUBLE_2ADDR 0xce
#define DF_REM_DOUBLE_2ADDR 0xcf
#define DF_ADD_INT_LIT16 0xd0
#define DF_RSUB_INT 0xd1
#define DF_MUL_INT_LIT16 0xd2
#define DF_DIV_INT_LIT16 0xd3
#define DF_REM_INT_LIT16 0xd4
#define DF_AND_INT_LIT16 0xd5
#define DF_OR_INT_LIT16 0xd6
#define DF_XOR_INT_LIT16 0xd7
#define DF_ADD_INT_LIT8 0xd8
#define DF_RSUB_INT_LIT8 0xd9
#define DF_MUL_INT_LIT8 0xda
#define DF_DIV_INT_LIT8 0xdb
#define DF_REM_INT_LIT8 0xdc
#define DF_AND_INT_LIT8 0xdd
#define DF_OR_INT_LIT8 0xde
#define DF_XOR_INT_LIT8 0xdf
#define DF_SHL_INT_LIT8 0xe0
#define DF_SHR_INT_LIT8 0xe1
#define DF_USHR_INT_LIT8 0xe2
#define DF_UNUSED_E3 0xe3
#define DF_UNUSED_E4 0xe4
#define DF_UNUSED_E5 0xe5
#define DF_UNUSED_E6 0xe6
#define DF_UNUSED_E7 0xe7
#define DF_UNUSED_E8 0xe8
#define DF_UNUSED_E9 0xe9
#define DF_UNUSED_EA 0xea
#define DF_UNUSED_EB 0xeb
#define DF_UNUSED_EC 0xec
#define DF_UNUSED_ED 0xed
#define DF_UNUSED_EE 0xee
#define DF_UNUSED_EF 0xef
#define DF_UNUSED_F0 0xf0
#define DF_UNUSED_F1 0xf1
#define DF_UNUSED_F2 0xf2
#define DF_UNUSED_F3 0xf3
#define DF_UNUSED_F4 0xf4
#define DF_UNUSED_F5 0xf5
#define DF_UNUSED_F6 0xf6
#define DF_UNUSED_F7 0xf7
#define DF_UNUSED_F8 0xf8
#define DF_UNUSED_F9 0xf9
#define DF_UNUSED_FA 0xfa
#define DF_UNUSED_FB 0xfb
#define DF_UNUSED_FC 0xfc
#define DF_UNUSED_FD 0xfd
#define DF_UNUSED_FE 0xfe
#define DF_UNUSED_FF 0xff

typedef struct DF_IR_BB {
    DF_u4   bpc;
    //DF_u4   bLength;
    DF_u4   tos;
    DF_u1   isCatch;
    DF_u4*  stacks;
    DF_u1   isTry;

    //DF_u1*  dexBlock;
    DF_u4   dPc;
    //DF_u4   dLength;
    DF_u1   processed;
    //DF_u4   succNum;
    
    //struct DF_IR_BB* next;
    //struct DF_IR_BB** succs;
}DF_IR_BB;

#endif
