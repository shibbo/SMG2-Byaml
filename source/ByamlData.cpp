#include "ByamlData.h"
#include "swap.h"

ByamlData::ByamlData() {
    mValue = 0;
    mType = TYPE_INVALID;
}

void ByamlData::set(const ByamlHashPair *pPair) {
    mType = pPair->getType();
    mValue = pPair->getValue();
}

void ByamlData::set(u8 type, u32 val) {
    mType = (ByamlDataType)type;
    mValue = val;
}

ByamlDataType ByamlData::getType() const {
    return mType;
}

u32 ByamlData::getValue() const {
    return mValue;
}

s32 ByamlHashPair::getKey() const {
    return mData >> 8;
}

ByamlDataType ByamlHashPair::getType() const {
    return (ByamlDataType)(mData & 0xFF);
}

s32 ByamlHashPair::getValue() const {
    return mValue;
}

ByamlHashIter::ByamlHashIter(const u8* data) : mData(data) {

}

ByamlHashIter::ByamlHashIter() : mData(0) {

}

const ByamlHashPair* ByamlHashIter::findPair(s32 key) const {
    const ByamlHashPair* pairTable = getPairTable();
    if (!mData)
        return 0;

    s32 lowerBound = 0;
    s32 upperBound = getSize();
    while (lowerBound < upperBound) {
        s32 avg = (lowerBound + upperBound) / 2;
        const ByamlHashPair* pair = &pairTable[avg];
        s32 result = key - pair->getKey();
        if (result == 0)
            return pair;

        if (result > 0)
            lowerBound = avg + 1;
        else
            upperBound = avg;
    }
    return 0;
}

bool ByamlHashIter::getDataByIndex(ByamlData* data, s32 index) const {
    if (!mData)
        return false;
    if (((s32)getSize()) < 1)
        return false;

    data->set(&getPairTable()[index]);
    return true;
}

bool ByamlHashIter::getDataByKey(ByamlData* data, s32 key) const {
    if (!mData)
        return false;
    if (((s32)getSize()) < 1)
        return false;

    const ByamlHashPair* pairTable = getPairTable();
    if (!mData)
        return false;
    s32 lowerBound = 0;
    s32 upperBound = getSize();
    const ByamlHashPair* pair;

    if (lowerBound >= upperBound)
        return false;
    while (true) {
        s32 avg = (lowerBound + upperBound) / 2;
        pair = &pairTable[avg];
        s32 result = key - pair->getKey();
        if (result == 0)
            break;

        if (result > 0)
            lowerBound = avg + 1;
        else
            upperBound = avg;
        if (lowerBound >= upperBound)
            return false;
    }
    if (pair == 0)
        return false;
    data->set(pair);
    return true;
}

const u8* ByamlHashIter::getOffsetData(u32 off) const {
    return &mData[off];
}

const ByamlHashPair* ByamlHashIter::getPairByIndex(s32 index) const {
    if (index < 0)
        return 0;
    if (((s32)getSize()) <= index)
        return 0;

    return &getPairTable()[index];
}

const ByamlHashPair* ByamlHashIter::getPairTable() const {
    if (!mData)
        return 0;
    return reinterpret_cast<const ByamlHashPair*>(mData + 4);
}

u32 ByamlHashIter::getSize() const {
    if (!mData)
        return 0;
    u32 val = *reinterpret_cast<const u32*>(mData);
    return val & 0xFFFFFF;
}

ByamlArrayIter::ByamlArrayIter(const u8* pData) : mData(pData) {

}

ByamlArrayIter::ByamlArrayIter() {
    mData = 0;
}

bool ByamlArrayIter::getDataByIndex(ByamlData* data, s32 index) const {
    if (index < 0)
        return false;
    if (((s32)getSize()) <= index)
        return false;

    data->set(getTypeTable()[index], getDataTable()[index]);
    return true;
}

const u32* ByamlArrayIter::getDataTable() const {
    return reinterpret_cast<const u32*>(getOffsetData((getSize() + 7) & 0xFFFFFFFC));
}

const u8* ByamlArrayIter::getOffsetData(u32 off) const {
    return &mData[off];
}

u32 ByamlArrayIter::getSize() const {
    u32 val = *reinterpret_cast<const u32*>(mData);
    return val & 0xFFFFFF;
}

const u8* ByamlArrayIter::getTypeTable() const {
    return mData + 4;
}