#pragma once

#include "revolution.h"

class ByamlHashPair;

enum ByamlDataType {
    TYPE_INVALID = 0,
    TYPE_STRING = 0xA0,
    TYPE_BINARY = 0xA1,
    TYPE_ARRAY = 0xC0,
    TYPE_HASH = 0xC1,
    TYPE_STRING_TABLE = 0xC2,
    TYPE_BOOL = 0xD0,
    TYPE_INT = 0xD1,
    TYPE_FLOAT = 0xD2,
    TYPE_UINT = 0xD3,
    TYPE_LONG = 0xD4,
    TYPE_ULONG = 0xD5,
    TYPE_DOUBLE = 0xD6,
    TYPE_NULL = 0xFF
};

class ByamlData {
public:
    ByamlData();

    void set(const ByamlHashPair *);
    void set(u8, u32);
    ByamlDataType getType() const;
    u32 getValue() const;

    template<typename T>
    T getValue() const {
        u32 val = getValue();
        return *reinterpret_cast<const T*>(&val);
    }

    u32 mValue;
    ByamlDataType mType;
};

class ByamlHashPair {
public:
    s32 getKey() const;
    ByamlDataType getType() const;
    s32 getValue() const;

    const s32 mData;
    const s32 mValue;
};

class ByamlHashIter {
public:
    ByamlHashIter(const u8* data);
    ByamlHashIter();

    const ByamlHashPair* findPair(s32 key) const;
    bool getDataByIndex(ByamlData* data, s32 index) const;
    bool getDataByKey(ByamlData* data, s32 key) const;
    const u8* getOffsetData(u32 off) const;
    const ByamlHashPair* getPairByIndex(s32 index) const;
    const ByamlHashPair* getPairTable() const;
    u32 getSize() const;

    const u8* mData;
};

class ByamlArrayIter {
public:
    ByamlArrayIter(const u8* data);
    ByamlArrayIter();

    bool getDataByIndex(ByamlData* data, s32 index) const;
    const u32* getDataTable() const;
    const u8* getOffsetData(u32 off) const;
    u32 getSize() const;
    const u8* getTypeTable() const;

    const u8* mData;
};