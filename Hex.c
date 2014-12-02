#include "Hex.h"

CF_u8 toByte8(CF_u8 v) {
    CF_u8 result = 0;
    int i;
    for(i = 0 ; i < 8 ; i++) {
        result = result | (((int)v & 0xff) << (56-i*8));
        v >>= 8;
    }
    return result;
}
CF_u4 toByte4(CF_u4 v) {
    CF_u4 result = 0;
    int i;
    for(i = 0 ; i < 4 ; i++) {
        result = result | (((int)v & 0xff) << (24-i*8));
        v >>= 8;
    }
    return result;
}
CF_u2 toByte2(CF_u2 v) {
    CF_u2 result = 0;
    int i;
    for(i = 0 ; i < 2 ; i++) {
        result = result | (((int)v & 0xff) << (8-i*8));
        v >>= 8;
    }
    return result;
}
