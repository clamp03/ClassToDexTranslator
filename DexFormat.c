#include "DexFormat.h"
DF_u4 getUleb128Size(DF_uleb128 value) {
    DF_u4 size = 0;
    DF_uleb128 tmp = value;
    do {
        size++;
        tmp = tmp >> 7;
    }while(tmp != 0);
    return size;
}
DF_u4 DF_writeSleb128Data(DF_u1* bytes, DF_u4 idx, DF_sleb128 val) {
    DF_u4 next;
    do {
        next = val >> 7;
        if(next != (DF_u4)-1 && next != 0) {
            bytes[idx] = (val & 0x007f) | 0x0080;
        }else {
            bytes[idx] = (val & 0x007f);
        }
        val = next;
        idx++;
    }while(val != (DF_u4)-1 && val != 0);
    return idx;
}
DF_u4 DF_writeUleb128Data(DF_u1* bytes, DF_u4 idx, DF_uleb128 val) {
    DF_u4 next;
    do {
        next = val >> 7;
        if(next > 0) {
            bytes[idx] = (val & 0x007f) | 0x0080;
        }else {
            bytes[idx] = (val & 0x007f);
        }
        val = next;
        idx++;
    }while(val != 0);
    return idx;
}
