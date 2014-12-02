#include "ClassFile.h"
CF_u1 CF_opcLengths[256] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 2, 3, 2, 3,
    3, 2, 2, 2, 2, 2, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 2, 2, 2, 2, 2, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 3, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 2,
    0, 0, 1, 1, 1, 1, 1, 1, 3, 3,
    3, 3, 3, 3, 3, 5, 0, 3, 2, 3,
    1, 1, 3, 3, 1, 1, 0, 4, 3, 3,
    5, 5, 1, 2, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 1,
    3, 3, 3, 5, 2, 2, 1, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 4,
    0, 0, 0, 0, 0, 0,
};

CF_u1 CF_getBytecodeLength(CF_u1* code, CF_u4 bpc) {
    CF_u1 bcode = code[bpc];
    CF_u1 len = CF_opcLengths[code[bpc]];
    if(bcode == CF_WIDE) {
        if(code[bpc + 1] == (CF_u1)CF_IINC) return 6;
        return 4;
    }
    if(len != 0) return len;
    CF_u4 addr = (bpc+4) & 0xFFFFFFFC;
    CF_u4 pads = addr - bpc - 1;
    if(bcode == CF_LOOKUPSWITCH) {
        CF_u4 npairs = CF_makeIntVal(code[addr+4], code[addr+5], code[addr+6], code[addr+7]);
        len = 1 + pads + 8 + npairs * 8;
    }else if(bcode == CF_TABLESWITCH) {
        CF_u4 low = CF_makeIntVal(code[addr+4], code[addr+5], code[addr+6], code[addr+7]);
        CF_u4 high = CF_makeIntVal(code[addr+8], code[addr+9], code[addr+10], code[addr+11]);
        len = 1 + pads + 12 + (high - low + 1) * 4;
    }
    return len;
}
/*
typedef union {
    CF_u1 c[4];
    CF_u4 i;
}CF_INT_TYPE;

CF_u4 CF_makeIntVal(CF_u1 a, CF_u1 b, CF_u1 c, CF_u1 d) {
    CF_INT_TYPE t;
    t.c[0] = d;
    t.c[1] = c;
    t.c[2] = b;
    t.c[3] = a;
    return t.i;
}
*/
