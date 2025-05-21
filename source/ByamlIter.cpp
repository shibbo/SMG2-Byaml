#include "ByamlIter.h"
#include "ByamlContainerHeader.h"
#include "ByamlData.h"
#include "ByamlHeader.h"
#include "ByamlUtil.h"

ByamlIter::ByamlIter() {
    mData = 0;
    mRootNode = 0;
}

ByamlIter::ByamlIter(const u8 *pData) : mData(pData), mRootNode(0) {
    if (pData == 0) {
        return;
    }

    if (!MR::verifiByamlHeader(pData)) {
        mData = 0;
        mRootNode = 0;
        return;
    }

    const ByamlHeader* header = mHeader;
    if (!header->getDataOffset()) {
        return;
    }

    mRootNode = &mData[header->getDataOffset()];
}

ByamlIter::ByamlIter(const u8* pData, const u8 *pRoot) {
    mData = pData;
    mRootNode = pRoot;
}

bool ByamlIter::isValid() const {
    return mData != 0;
}

bool ByamlIter::isExistKey(const char* key) const {
    if (!mRootNode || *mRootNode != TYPE_HASH)
        return false;

    s32 index = getKeyIndex(key);
    if (index < 0)
        return false;

    ByamlHashIter iter(mRootNode);
    return iter.findPair(index);
}

s32 ByamlIter::getKeyIndex(const char* key) const {
    ByamlStringTableIter hash = MR::getHashKeyTable(mData);
    if (!hash.isValidate())
        return -1;

    return hash.findStringIndex(key);
}

bool ByamlIter::tryGetIterByKey(ByamlIter* iter, const char* key) const {
    *iter = getIterByKey(key);
    return iter->isValid();
}


ByamlIter ByamlIter::getIterByKey(const char* key) const {
    ByamlData data;
    if (!getByamlDataByKey(&data, key)) {
        return 0;
    }

    if (data.getType() != TYPE_ARRAY && data.getType() != TYPE_HASH) {
        if (data.getType() == TYPE_NULL) {
            return ByamlIter(mData, 0);
        }

        return ByamlIter();
    }
    
    return ByamlIter(mData, &mData[data.getValue()]);
}

bool ByamlIter::getByamlDataByKey(ByamlData* data, const char* key) const {
    if (!mRootNode || *mRootNode != TYPE_HASH) {
        return false;
    }

    ByamlStringTableIter hash_table = MR::getHashKeyTable(mData);
    if (!hash_table.isValidate()) {
        return false;
    }   

    ByamlHashIter iter(mRootNode);
    s32 lowerBound = 0;
    s32 upperBound = iter.getSize();
    while (lowerBound < upperBound) {
        s32 avg = (lowerBound + upperBound) / 2;
        const ByamlHashPair* pair = iter.getPairByIndex(avg);
        s32 result = strcmp(key, hash_table.getString(pair->getKey()));
        if (result == 0) {
            data->set(pair);
            return true;
        }

        if (result > 0)
            lowerBound = avg + 1;
        else
            upperBound = avg;
    }
    return false;
}

s32 ByamlIter::getSize() const {
    if (!mRootNode)
        return false;
    ByamlContainerHeader* header = (ByamlContainerHeader*)mRootNode;
    u32 type = *mRootNode;
    if (type == TYPE_ARRAY || type == TYPE_HASH)
        return header->getCount();
    return 0;
}

ByamlIter ByamlIter::getIterByIndex(s32 index) const {
    ByamlData data;
    if (!getByamlDataByIndex(&data, index))
        return 0;
    if (data.getType() != TYPE_ARRAY && data.getType() != TYPE_HASH) {
        if (data.getType() == TYPE_NULL)
            return ByamlIter(mData, 0);
        return ByamlIter();
    }
    return ByamlIter(mData, &mData[data.getValue()]);
}

bool ByamlIter::getByamlDataByIndex(ByamlData* data, s32 index) const {
    if (!mRootNode)
        return false;
    if (*mRootNode == TYPE_ARRAY) {
        ByamlArrayIter iter(mRootNode);
        return iter.getDataByIndex(data, index);
    }
    if (*mRootNode == TYPE_HASH) {
        ByamlHashIter iter(mRootNode);
        return iter.getDataByIndex(data, index);
    }
    return false;
}

bool ByamlIter::tryGetStringByKey(const char** value, const char* key) const {
    ByamlData data;
    if (!getByamlDataByKey(&data, key))
        return false;

    if (data.getType() == TYPE_NULL)
        return false;
    return tryConvertString(value, &data);
}

bool ByamlIter::tryConvertString(const char** value, const ByamlData* data) const {
    if (data->getType() != TYPE_STRING)
        return false;

    ByamlStringTableIter string_table = MR::getStringTable(mData);
    if (!string_table.isValidate())
        return false;
    *value = string_table.getString(data->getValue());

    return true;
}

bool ByamlIter::tryGetIntByKey(s32* value, const char* key) const {
    ByamlData data;
    if (!getByamlDataByKey(&data, key))
        return false;

    if (data.getType() == TYPE_NULL)
        return false;
    return tryConvertInt(value, &data);
}

bool ByamlIter::tryConvertInt(s32* value, const ByamlData* data) const {
    if (data->getType() != TYPE_INT)
        return false;

    *value = data->getValue();
    return true;
}

bool ByamlIter::tryGetUIntByKey(u32* value, const char* key) const {
    ByamlData data;
    if (!getByamlDataByKey(&data, key))
        return false;

    if (data.getType() == TYPE_NULL)
        return false;
    return tryConvertUInt(value, &data);
}

bool ByamlIter::tryConvertUInt(u32* value, const ByamlData* data) const {
    s32 val = data->getValue<s32>();
    if (data->getType() == TYPE_INT) {
        bool result = val >= 0;
        *value = val < 0 ? 0 : val;
        return result;
    }
    if (data->getType() == TYPE_UINT) {
        *value = val;
        return true;
    }

    return false;
}

bool ByamlIter::tryGetFloatByIndex(f32* value, s32 index) const {
    ByamlData data;
    if (!getByamlDataByIndex(&data, index))
        return false;

    return tryConvertFloat(value, &data);
}

bool ByamlIter::tryGetFloatByKey(f32* value, const char* key) const {
    ByamlData data;
    if (!getByamlDataByKey(&data, key))
        return false;

    if (data.getType() == TYPE_NULL)
        return false;
    return tryConvertFloat(value, &data);
}

bool ByamlIter::tryConvertFloat(f32* value, const ByamlData* data) const {
    if (data->getType() != TYPE_FLOAT)
        return false;

    *value = data->getValue<f32>();
    return true;
}

bool ByamlIter::tryGetBoolByIndex(bool* value, s32 index) const {
    ByamlData data;
    if (!getByamlDataByIndex(&data, index))
        return false;

    return tryConvertBool(value, &data);
}

bool ByamlIter::tryGetBoolByKey(bool* value, const char* key) const {
    ByamlData data;
    if (!getByamlDataByKey(&data, key))
        return false;

    if (data.getType() == TYPE_NULL)
        return false;
    return tryConvertBool(value, &data);
}

bool ByamlIter::tryConvertBool(bool* value, const ByamlData* data) const {
    if (data->getType() != TYPE_BOOL)
        return false;

    *value = data->getValue() != 0;
    return true;
}