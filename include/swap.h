#pragma once

#include "revolution.h"

static __inline u16 bswap_16(u16 __x) {
    return (__x << 8) | (__x >> 8);
}

static __inline u32 bswap_24(u32 __x) {
    return (__x & 0xFF00) | (((__x & 0xFF) << 16) & 0xFFFFFF00) | ((__x >> 16) & 0xFF);
}

static __inline u32 bswap_32_ignore_last(u32 __x) {
    return (__x >> 24) | (__x >> 8 & 0xff00) | (__x << 8 & 0xff0000);
}

static __inline u32 bswap_32(u32 __x) {
    return (__x >> 24) | (__x >> 8 & 0xff00) | (__x << 8 & 0xff0000) | (__x << 24);
}

static __inline u64 bswap_32_64(u64 x) {
    return ((((x)&0xff00000000000000ull) >> 56) | (((x)&0x00ff000000000000ull) >> 40) |
            (((x)&0x0000ff0000000000ull) >> 24) | (((x)&0x000000ff00000000ull) >> 8) |
            (((x)&0x00000000ff000000ull) << 8) | (((x)&0x0000000000ff0000ull) << 24) |
            (((x)&0x000000000000ff00ull) << 40) | (((x)&0x00000000000000ffull) << 56));
}

static __inline u64 bswap_64(u64 __x) {
    return ((bswap_32(__x) + 0ULL) << 32) | bswap_32(__x >> 32);
}