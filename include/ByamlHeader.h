#pragma once

#include "revolution.h"

class ByamlHeader {
public:
    u16 getTag() const;
    bool isInvertOrder() const;
    u16 getVersion() const;
    u32 getHashKeyTableOffset() const;
    u32 getStringTableOffset() const;
    u32 getDataOffset() const;

    union {
        s32 _0;
        u16 mTag, mVersion;
    };

    s32 mHashKeyOffset;
    s32 mStringTableOffset;
    s32 mDataOffset;
};

class ByamlStringTableIter {
public:
    ByamlStringTableIter();
    ByamlStringTableIter(const u8* data);

    s32 getSize() const;
    const u32* getAddressTable() const;
    u32 getStringAddress(s32 index) const;
    u32 getEndAddress() const;
    const char* getString(s32 index) const;
    s32 getStringSize(s32 index) const;
    s32 findStringIndex(const char* str) const;
    bool isValidate() const;

    const u8* mData;
};