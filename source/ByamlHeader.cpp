#include "ByamlHeader.h"
#include "swap.h"

u16 ByamlHeader::getTag() const {
    return isInvertOrder() ? bswap_16(mTag) : mTag;
}

bool ByamlHeader::isInvertOrder() const {
    return mTag == 'YB';
}

u16 ByamlHeader::getVersion() const {
    if ((_0 & 0xFFFF) == 'YB') 
        return bswap_16(_0 >> 16);

    return _0 & 0xFFFF;
}

u32 ByamlHeader::getHashKeyTableOffset() const {
    return isInvertOrder() ? bswap_32(mHashKeyOffset) : mHashKeyOffset;
}

u32 ByamlHeader::getStringTableOffset() const {
    return isInvertOrder() ? bswap_32(mStringTableOffset) : mStringTableOffset;
}

u32 ByamlHeader::getDataOffset() const {
    return isInvertOrder() ? bswap_32(mDataOffset) : mDataOffset;
}

ByamlStringTableIter::ByamlStringTableIter() {
    mData = 0;
}

ByamlStringTableIter::ByamlStringTableIter(const u8* pData) {
    mData = pData;
}

const u32* ByamlStringTableIter::getAddressTable() const {
    return reinterpret_cast<const u32*>(mData + 4);
}

u32 ByamlStringTableIter::getStringAddress(s32 idx) const {
    return getAddressTable()[idx];
}


s32 ByamlStringTableIter::getSize() const {
    u32 type_and_size = *reinterpret_cast<const u32*>(mData);
    return type_and_size & 0xFFFFFF;
}

const char* ByamlStringTableIter::getString(s32 index) const {
    return reinterpret_cast<const char*>(&mData[getStringAddress(index)]);
}

s32 ByamlStringTableIter::findStringIndex(const char* str) const {
    s32 lowerBound = 0;
    s32 upperBound = getSize();
    while (lowerBound < upperBound) {
        s32 avg = (lowerBound + upperBound) / 2;
        s32 result = strcmp(str, getString(avg));
        if (result == 0)
            return avg;

        if (result > 0)
            lowerBound = avg + 1;
        else
            upperBound = avg;
    }
    return -1;
}

bool ByamlStringTableIter::isValidate() const {
    return mData != 0;
}